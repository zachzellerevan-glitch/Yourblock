#pragma once

#include "YBpch.h"
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <future>

//TODO:a better threadpool

namespace Engine{
    class ThreadPool{
        public:
            using Taskqueue = std::queue<std::function<void()>>;
            using TaskFunc = std::function<void()>;
            ThreadPool(int ThreadCounts = 10);
            ~ThreadPool();

            template<class F,class ... Args>
            void EnTaskQueue(F && f,Args && ...args){
                auto task = [f = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable{
                    std::invoke(f,args...);
                };
                {
                    std::unique_lock<std::mutex> lock(m_Mutex);
                    m_Taskqueue.emplace(task);
                }
                m_ConditionVar.notify_one();
            }

            //return a future obj
            //async
            template<class F,class ... Args>
            auto Submit(F && f,Args && ... args) -> std::future<std::invoke_result_t<F,Args...>>{ //decltype(F(Args...))
                using ReturnType = std::invoke_result_t<F,Args...>;

                //pack task
                auto task = std::make_shared<std::packaged_task<ReturnType()>>([
                    f = std::forward<F>(f),...args = std::forward<Args>(args)]
                    () mutable -> ReturnType{
                        return std::invoke(f,args...);
                });

                //get future
                std::future<ReturnType> result = task->get_future();

                //enqueue
                {
                    std::lock_guard<std::mutex> lock(m_Mutex);
                    if(m_PoolStop){
                        throw std::runtime_error("Submit on a stopped threadpool.");
                    }
                    m_Taskqueue.emplace([task](){(*task)();});
                }
                m_ConditionVar.notify_one();
                return result;
            }

            
        private:
            void WorkerLoop();
            int m_ThreadCounts;
            std::vector<std::thread> m_Threads;
            Taskqueue m_Taskqueue;
            std::mutex m_Mutex;
            std::condition_variable m_ConditionVar;
            std::atomic<bool> m_PoolStop;
    };
}