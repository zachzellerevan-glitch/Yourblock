#pragma once

#include "YBpch.h"
#include "Block.h"
#include "Render/Vertex.h"
#include <glm/glm.hpp>


namespace Engine{
    class World;
    class Chunk;
    //TODO:Frustum culling
    class ChunkMesher{
        public:
            static void GenerateMesh(const Chunk & chunk,std::vector<Vertex>& Verticies,std::vector<uint32_t> & Indicies,const World *world = nullptr);
        private:
            static bool IsFaceVisible(const Chunk & chunk,int x,int y,int z,Face face,const World *world = nullptr);
            static void AddFace(std::vector<Vertex> & Vertices,std::vector<uint32_t> & Indicies,int x,int y,int z,Face face,int Layer);
    };

   
}