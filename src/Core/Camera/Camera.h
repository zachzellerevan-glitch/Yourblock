#pragma once

#include "YBpch.h"
#include "Core/Timer/Timer.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



namespace Engine{
    class Camera{
        public:
            enum class MoveDirection{
                FORWARD,BACKWARD,LEFT,RIGHT,UP,DOWN
            };
            Camera(float Pov,float AspectRatio,float NearPlane,float FarPlane);
            void SetPosition(const glm::vec3 & position);
            void SetRotation(float Yaw,float Pitch);
            void EnableFly(bool flag);
            inline glm::mat4 GetViewMartix() const {return m_ViewMartix;}
            inline glm::mat4 GetProjectionMartix() const {return m_ProjectionMartix;}

            void CameraMove(MoveDirection movement,float dt);
            void CameraView(float xoffset,float yoffset);

        private:
            void UpdateView();
            float m_Yaw,m_Pitch;
            float m_Fov,m_AspectRatio,m_NearPlane,m_FarPlane;
            float m_CameraSpeed = 1.0f,m_Sensitive = 0.2f;
            bool m_EnableFly = false;
            glm::vec3 m_WorldUp = glm::vec3(0.0f,1.0f,0.0f);
            glm::vec3 m_Up,m_Right,m_Front;
            glm::vec3 m_Position;
            glm::mat4 m_ViewMartix;
            glm::mat4 m_ProjectionMartix; //投影
    };
}
