#include "ThreadPool.h"

namespace Engine{
    ThreadPool::ThreadPool(int ThreadCounts) : m_PoolStop(false){
        m_ThreadCounts = std::max(ThreadCounts,1);
        for(int i = 0;i < ThreadCounts;i++){
            m_Threads.emplace_back([this](){this->WorkerLoop();});
        }
    }

    ThreadPool::~ThreadPool(){
        {
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_PoolStop = true;
        }
        m_ConditionVar.notify_all();
        for(auto & thread : m_Threads){
            thread.join();
        }
    }

    void ThreadPool::WorkerLoop(){
        while(true){
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_ConditionVar.wait(lock,[this](){
                return !m_Taskqueue.empty() || m_PoolStop;
            });
            if(m_Taskqueue.empty() && m_PoolStop){
                return;
            }
            TaskFunc task(std::move(m_Taskqueue.front()));
            m_Taskqueue.pop();
            lock.unlock();
            task();
        }
    }

    
}