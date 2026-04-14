#pragma once

#include "YBpch.h"
#include "ChunkMesher.h"
#include "Vertex.h"

namespace Engine{
     class ChunkMesh{
        public:
            ChunkMesh() = default;
            ~ChunkMesh();

            void Upload(const std::vector<Vertex> & Verticies,const std::vector<uint32_t> & Indicies);
            void Draw() const;
            bool IsValid() const {return m_IndexCount > 0;}

        private:
            unsigned int m_VAO = 0;
            unsigned int m_VBO = 0;
            unsigned int m_EBO = 0;
            GLsizei m_IndexCount = 0;
    };
}