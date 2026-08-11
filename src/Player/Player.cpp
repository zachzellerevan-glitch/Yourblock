#include "Player.h"
#include "Core/Camera/Camera.h"

namespace Engine{
    void Player::Update(float dt, const glm::vec3 & moveDir, bool jump, World & world){
        m_Velocity.x = moveDir.x * WalkSpeed;
        m_Velocity.z = moveDir.z * WalkSpeed;
        if(jump && m_OnGround){
            m_Velocity.y = JumpVelocity;
            m_OnGround = false;
        }
        m_Velocity.y -= Gravity * dt;
        if(m_Velocity.y < MaxFallVelocity){
            m_Velocity.y = MaxFallVelocity;
        }
        MoveAndCollideX(dt, world);
        MoveAndCollideZ(dt, world);
        MoveAndCollideY(dt, world);
    }

    std::vector<glm::ivec3> Player::QuerySolidBlock(const glm::vec3 & pos, World & world) const{
        glm::vec3 min = pos - glm::vec3(HalfWidth, 0.0f, HalfWidth);
        glm::vec3 max = pos - glm::vec3(HalfWidth, Height, HalfWidth);

        std::vector<glm::ivec3> hits;
        for(int bx = (int)std::floor(min.x); bx <= (int)std::floor(max.x); bx++)
            for(int by = (int)std::floor(min.y); by <= (int)std::floor(max.y); by++)
                for(int bz = (int)std::floor(min.z); bz <= (int)std::floor(max.z); bz++){
                    
                }
    }
}