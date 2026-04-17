#pragma once

#include "YBpch.h"
#include <glm/glm.hpp>
#include "Chunk.h"

namespace Engine{
    class World{
        public:
            using ChunkMap = std::unordered_map<ChunkCoord,std::unique_ptr<Chunk>>;
            
            World(int LoadRadius = 10);
            ~World() = default;
            void LoadChunks(const ChunkCoord & Coord);
            void UnloadChunks(const ChunkCoord & Coord);
            void Update(const glm::vec3 & PlayerPos);
            void Render(Shader & shader);
            BlockType GetBlock(int WorldX,int WorldY,int WorldZ) const;
            void SetBlock(int WorldX,int WorldY,int WorldZ,BlockType block);
            Chunk * GetChunk(const ChunkCoord & Coord);
            const Chunk * GetChunk(const ChunkCoord & Coord) const;
            bool IsChunkVisible() const;
        private:
            int m_LoadRadius;
            ChunkMap m_ChunkHash;
    };
}