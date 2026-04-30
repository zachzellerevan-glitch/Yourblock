#pragma once

#include "YBpch.h"
#include "Block.h"
#include "Render/Shader.h"
#include "Render/Vertex.h"

namespace Engine{
     struct ChunkCoord{
        int x,z;
        bool operator==(const ChunkCoord & other) const{
            return x == other.x && z == other.z;
        };
    };
    
    class ChunkMesh;
    class Chunk{
        public:
            static constexpr int WIDTH = 16;
            static constexpr int HEIGHT = 256;
            static constexpr int DEPTH = 16;
            Chunk(const ChunkCoord & Coord);
            ~Chunk();
            void SetBlock(int x,int y,int z,BlockType block);
            void GenerateFlatChunk();
            BlockType GetBlockType(int x,int y,int z) const;
            void RebuildMesh();
            void Render();
            void UploadMesh(std::vector<Vertex> &&vertices,std::vector<uint32_t> &&indices);
            void Render(Shader & shader);
            //const BlockAttribution & GetBlockAttribution(int x,int y,int z) const;
            inline static uint16_t GetIndex(int x,int y,int z){ return (y * DEPTH + z) * WIDTH + x;}

        private:
            std::array<uint16_t,WIDTH * HEIGHT * DEPTH> m_Blocks;
            std::unique_ptr<ChunkMesh> m_Mesh;
            bool m_IsMeshDirty = true;
            int m_ChunkX,m_ChunkZ; //Chunk index
            ChunkCoord m_ChunkCoord;
    };
}


namespace std{
    template<>
    struct hash<Engine::ChunkCoord>{
        size_t operator()(const Engine::ChunkCoord & c) const {
            uint64_t ux = static_cast<uint64_t>(c.x);
            uint64_t uz = static_cast<uint64_t>(c.z);
            // 2. 可选：偏移到非负区间（如果希望正负对称映射）
            // ux += 0x80000000ULL;
            // uz += 0x80000000ULL;
            // 3. 组合成 64 位哈希
            return (ux << 32) ^ uz;
        }
    };
}