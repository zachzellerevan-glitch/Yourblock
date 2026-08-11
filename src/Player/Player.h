#pragma once

#include "YBpch.h"
#include "Core/Core.h"
#include "World/World.h"

namespace Engine{
    class Player{
        public:
            static constexpr float Weight = 0.6f;
            static constexpr float Height = 1.8f;
            static constexpr float EyeHeight = 1.6f;
            static constexpr float HalfWidth = Weight / 2.0f;
            static constexpr float WalkSpeed = 2.0f;
            static constexpr float RunSpeed = 4.0f;
            static constexpr float JumpVelocity = 8.0f;
            static constexpr float Gravity = 18.6f;
            static constexpr float MaxFallVelocity = -100.0f;

            Player(const glm::vec3 SpawnPos);
            
            void Update(float dt, const glm::vec3 & moveDir, bool jump, bool sprint, World & world);
            void Teleport(const glm::vec3 & pos);
            glm::vec3 GetMinVertex() const{
                return m_Position - glm::vec3(HalfWidth, 0.0f, HalfWidth);
            }
            glm::vec3 GetMaxVertex() const{
                return m_Position + glm::vec3(HalfWidth, Height, HalfWidth);
            }
            glm::vec3 GetEyePosition() const{
                return m_Position + glm::vec3(0.0f, EyeHeight, 0.0f);
            }

            const glm::vec3 GetPosition() const{
                return m_Position;
            }

            
            const bool IsOnGround() const{
                return m_OnGround;
            }

        private:
            void MoveAndCollideX(float dt, World & world);
            void MoveAndCollideZ(float dt, World & world);
            void MoveAndCollideY(float dt, World & world);
            glm::vec3 m_Position;
            glm::vec3 m_Velocity;
            std::vector<glm::ivec3> QuerySolidBlock(const glm::vec3 & pos, World & world) const;
            bool m_OnGround;
    };
}