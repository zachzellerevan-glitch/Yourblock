#pragma once

#include "YBpch.h"
#include <glm/glm.hpp>
#include "World.h"

namespace Engine{
    struct RaycastHit{
        bool Hit = false;
        glm::ivec3 HitPos = glm::ivec3(0);
        glm::ivec3 HitFaceNormal = glm::ivec3(0);
        BlockType Block = BlockType::AIR;
        float Distance = 0.0f;
    };

    class Raycaster{
        public:
            //Raycaster(uint8_t MaxDist = 20);
            static RaycastHit TraverseRay(uint8_t MaxDist,const glm::vec3 &Origin, const glm::vec3 &Dir, const World * world);
        private:
    };
}