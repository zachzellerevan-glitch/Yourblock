#pragma once

#include "YBpch.h"
#include "Block.h"
#include "Render/Shader.h"

namespace Engine{
    class ChunkMesh;
    class Chunk{
        public:
            static constexpr int WIDTH = 16;
            static constexpr int HEIGHT = 256;
            static constexpr int DEPTH = 16;
            Chunk(int chunkx,int chunkz);
            ~Chunk();
            void SetBlock(int x,int y,int z,BlockType block);
            void GenerateFlatChunk();
            BlockType GetBlockType(int x,int y,int z) const;
            void RebuildMesh();
            void Render(Shader & shader);
            //const BlockAttribution & GetBlockAttribution(int x,int y,int z) const;
            inline static uint16_t GetIndex(int x,int y,int z){ return (y * DEPTH + z) * WIDTH + x;}

        private:
            std::array<uint16_t,WIDTH * HEIGHT * DEPTH> m_Blocks;
            std::unique_ptr<ChunkMesh> m_Mesh;
            bool m_IsMeshDirty = true;
            int m_ChunkX,m_ChunkZ; //Chunk index
    };
}