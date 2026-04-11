#pragma once

#include "YBpch.h"
#include "Chunk.h"
#include <glm/glm.hpp>


namespace Engine{
    struct Vertex{
        glm::vec3 Position;
        glm::vec2 TexCoord;        
    };
    class ChunkMesher{
        public:
            static void GenerateMesh(const Chunk & chunk,std::vector<Vertex>& Verticies,std::vector<uint32_t> & Indicies);
        private:
            static bool IsFaceVisible(const Chunk & chunk,int x,int y,int z,Face face);
            static void AddFace(std::vector<Vertex> & Vertices,std::vector<uint32_t> & Indicies,int x,int y,int z,Face face,const glm::vec4 & uvRect);
    };
}