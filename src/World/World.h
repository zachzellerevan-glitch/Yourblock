#pragma once

#include "YBpch.h"
#include <glm/glm.hpp>
#include "Chunk.h"
#include "ChunkMesher.h"
#include "Core/ThreadPool/ThreadPool.h"
#include "Render/Vertex.h"
#include <unordered_set>
#include <mutex>
#include <queue>

namespace Engine{

    struct ChunkBuildResult{
        ChunkCoord coord;
        std::unique_ptr<Chunk> chunk;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    class World{
        public:
            using ChunkMap = std::unordered_map<ChunkCoord,std::unique_ptr<Chunk>>;
            
            World(int LoadRadius = 10,int ThreadCount = 4);
            ~World() = default;
            void LoadChunks(const ChunkCoord & Coord);
            void UnloadChunks(const ChunkCoord & Coord);
            void Update(const glm::vec3 & PlayerPos);
            void Render(Shader & shader);
            BlockType GetBlock(int WorldX,int WorldY,int WorldZ) const;
            void SetBlock(int WorldX,int WorldY,int WorldZ,BlockType block);
            void SetBlock(glm::vec3 DestPos,BlockType block);
            Chunk * GetChunk(const ChunkCoord & Coord);
            const Chunk * GetChunk(const ChunkCoord & Coord) const;
            bool IsChunkVisible() const;
        private:
            void ProcessCompleteChunk();
            void GenerateChunkData(const ChunkCoord & Coord);
            //int Gravity;
            std::mutex m_LoadingMutex,m_CompleteMutex;
            std::queue<ChunkBuildResult> m_CompleteQueue;
            std::unordered_set<ChunkCoord> m_LoadingSet;
            std::unique_ptr<ThreadPool> m_Threadpool;

            int m_LoadRadius,m_ThreadCount;
            ChunkMap m_ChunkHash;
    };
}