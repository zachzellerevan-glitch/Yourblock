#pragma once

#include "YBpch.h"
#include "Core/Timer/Timer.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ostream>



namespace Engine{
    class Camera{
        public:
            enum class MoveDirection{
                FORWARD,BACKWARD,LEFT,RIGHT,UP,DOWN
            };
            Camera(float Pov,float AspectRatio,float NearPlane,float FarPlane);
            void SetPosition(const glm::vec3 & position);
            void SetRotation(float Yaw,float Pitch);
            void SetAspectRatio(float AspectRatio);
            void EnableFly(bool flag);
            void SetCameraSpeed(float Speed){m_CameraSpeed = Speed;};
            inline glm::mat4 GetViewMartix() const {return m_ViewMartix;}
            inline glm::mat4 GetProjectionMartix() const {return m_ProjectionMartix;}
            inline glm::vec3 GetPosition() const {return m_Position;}
            inline glm::vec3 GetFront() const {return m_Front;}
            inline glm::vec3 GetRight() const {return m_Right;}
            void CameraMove(MoveDirection movement,float dt);
            void CameraView(float xoffset,float yoffset);

        private:
            void UpdateView();
            float m_Yaw,m_Pitch;
            float m_Fov,m_AspectRatio,m_NearPlane,m_FarPlane;
            float m_CameraSpeed = 5.0f,m_Sensitive = 0.2f;
            bool m_EnableFly = true;
            glm::vec3 m_WorldUp = glm::vec3(0.0f,1.0f,0.0f);
            glm::vec3 m_Up,m_Right,m_Front;
            glm::vec3 m_Position;
            glm::mat4 m_ViewMartix;
            glm::mat4 m_ProjectionMartix; //投影
    };

    inline std::ostream & operator<<(std::ostream & os,glm::vec3 vec){
        return os<<std::floor(vec.x)<<","<<std::floor(vec.y)<<","<<std::floor(vec.z);
    }

    // inline bool operator==(glm::vec3 v1,glm::vec3 v2){
    //     if(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z) return true;
    //     return false;
    // }
}
