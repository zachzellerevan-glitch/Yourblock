#pragma once

#include "Core/Core.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/TextureArray.h"
#include "World/ChunkMesher.h"
#include "World/Chunk.h"
#include "World/World.h"
#include "World/Raycaster.h"
#include "YBpch.h"
#include "Player/Player.h"

namespace Engine{
    class GameLayer : public Layer{
        public:
            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate(float dt) override;
            virtual void OnEvent(Event & event) override;
            virtual bool IsEnabled() override {return m_Enable;}
        private:
            void BreakBlock();
            void PlaceBlock();
    
        private:
            bool m_MouseLeftHold = false;
            bool m_MouseRightHold = false;
            float m_LastBreakTime = 0.0f;
            float m_LastPlaceTime = 0.0f;
            float m_BreakInterval = 0.15f;
            float m_PlaceInterval = 0.2f;

            std::unique_ptr<Shader> m_Shader;
            std::unique_ptr<Camera> m_Camera;
            std::unique_ptr<Texture> m_Texture;
            std::unique_ptr<Chunk> m_Chunk;
            std::unique_ptr<Player> m_Player;
            ChunkCoord m_ChunkCoord = {0,0};
            std::unique_ptr<World> m_World;
            bool m_FirstFrame = true;
            unsigned int m_VAO,m_VBO,m_EBO;
            bool m_ESC = false;
            bool m_Enable = true;
    };
}