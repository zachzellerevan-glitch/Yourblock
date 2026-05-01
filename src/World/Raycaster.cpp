#include "Raycaster.h"

namespace Engine{

    RaycastHit Raycaster::TraverseRay(uint8_t MaxDist,const glm::vec3 &Origin, const glm::vec3 &Dir,const World * world){
        int X = std::floor(Origin.x),Y = std::floor(Origin.y),Z = std::floor(Origin.z);
        
        int stepX,stepY,stepZ;
        
        float tDeltaX,tDeltaY,tDeltaZ;

        float tMaxX,tMaxY,tMaxZ;
        
        if (Dir.x > 0) {
            stepX = 1;
            tMaxX = ((X + 1) - Origin.x) / Dir.x;
            tDeltaX = 1.0f / Dir.x;
        } else if (Dir.x < 0) {
            stepX = -1;
            tMaxX = (X - Origin.x) / Dir.x;
            tDeltaX = 1.0f / -Dir.x;
        } else {
            stepX = 0;
            tMaxX = INFINITY;
            tDeltaX = INFINITY;
        }

        if (Dir.y > 0) {
            stepY = 1;
            tMaxY = ((Y + 1) - Origin.y) / Dir.y;
            tDeltaY = 1.0f / Dir.y;
        } else if (Dir.y < 0) {
            stepY = -1;
            tMaxY = (Y - Origin.y) / Dir.y;
            tDeltaY = 1.0f / -Dir.y;
        } else {
            stepY = 0;
            tMaxY = INFINITY;
            tDeltaY = INFINITY;
        }

        if (Dir.z > 0) {
            stepZ = 1;
            tMaxZ = ((Z + 1) - Origin.z) / Dir.z;
            tDeltaZ = 1.0f / Dir.z;
        } else if (Dir.z < 0) {
            stepZ = -1;
            tMaxZ = (Z - Origin.z) / Dir.z;
            tDeltaZ = 1.0f / -Dir.z;
        } else {
            stepZ = 0;
            tMaxZ = INFINITY;
            tDeltaZ = INFINITY;
        }

        int distance = 0;
        RaycastHit m_RaycastHit;
        while(distance < MaxDist){
            distance++;
            if(world->GetBlock(X,Y,Z) != BlockType::AIR){
                m_RaycastHit.Hit = true;
                m_RaycastHit.HitPos = {X,Y,Z};
                break;
            }

            if(tMaxX < tMaxY && tMaxX < tMaxZ){
                X += stepX;
                tMaxX += tDeltaX;
                m_RaycastHit.HitFaceNormal = glm::vec3(-stepX,0,0);
            }else if(tMaxY < tMaxZ){
                Y += stepY;
                tMaxY += tDeltaY;
                m_RaycastHit.HitFaceNormal = glm::vec3(0,-stepY,0);
            }else{
                Z += stepZ;
                tMaxZ += tDeltaZ;
                m_RaycastHit.HitFaceNormal = glm::vec3(0,0,-stepZ);
            }
        }
        return m_RaycastHit;
    }
}