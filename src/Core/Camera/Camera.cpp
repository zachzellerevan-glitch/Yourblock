#include "Camera.h"

namespace Engine{
    Camera::Camera(float Fov,float AspectRatio,float NearPlane,float FarPlane)
            : m_Fov(Fov),m_AspectRatio(AspectRatio),m_NearPlane(NearPlane),m_FarPlane(FarPlane),
            m_Position(0.0f,0.0f,0.0f),m_Yaw(-90.0f),m_Pitch(0.0f){
        m_ProjectionMartix = glm::perspective(glm::radians(m_Fov),m_AspectRatio,m_NearPlane,m_FarPlane);
        UpdateView();
    }

    void Camera::UpdateView(){
        //球面坐标转化为笛卡尔坐标
        m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front.y = sin(glm::radians(m_Pitch));
        m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(m_Front);
        m_Right = glm::normalize(glm::cross(m_Front,m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right,m_Front));

        m_ViewMartix = glm::lookAt(m_Position,m_Position + m_Front,m_Up); //Position,look at point,up
    }

    void Camera::SetPosition(const glm::vec3 &position){
        m_Position = position;
        UpdateView();
    }

    void Camera::SetRotation(float Yaw,float Pitch){
        m_Yaw = Yaw;
        m_Pitch = Pitch;
        UpdateView();
    }

    void Camera::EnableFly(bool flag){
        m_EnableFly = flag;
    }

    void Camera::CameraMove(MoveDirection movement,float dt){
        switch (movement){
        case MoveDirection::FORWARD:{
            glm::vec3 horizontalFront = glm::vec3(m_Front.x, 0.0f, m_Front.z);
            // 防止俯仰角接近 ±90° 时方向退化为零向量
            if (glm::length(horizontalFront) > 0.001f) {
                horizontalFront = glm::normalize(horizontalFront);
            } else {
                horizontalFront = glm::vec3(0.0f, 0.0f, 1.0f); // 备用方向
            }
            m_Position += horizontalFront * m_CameraSpeed * dt;
            break;
        }
        case MoveDirection::BACKWARD:{
            glm::vec3 horizontalFront = glm::vec3(m_Front.x, 0.0f, m_Front.z);
            
            if (glm::length(horizontalFront) > 0.001f) {
                horizontalFront = glm::normalize(horizontalFront);
            } else {
                horizontalFront = glm::vec3(0.0f, 0.0f, 1.0f); 
            }
            m_Position -= horizontalFront * m_CameraSpeed * dt;
            break;
        }
        case MoveDirection::LEFT:
            m_Position -= m_Right * m_CameraSpeed * dt;
            break;
        case MoveDirection::RIGHT:
            m_Position += m_Right * m_CameraSpeed * dt;
            break;
        case MoveDirection::UP:{
            glm::vec3 horizontalUp = glm::vec3(0.0f,m_Up.y,0.0f);
            if (glm::length(horizontalUp) > 0.001f) {
                horizontalUp = glm::normalize(horizontalUp);
            }
            if(m_EnableFly)
                m_Position += horizontalUp * m_CameraSpeed * dt;
            break;
        }
        case MoveDirection::DOWN:{
            glm::vec3 horizontalUp = glm::vec3(0.0f,m_Up.y,0.0f);
            if (glm::length(horizontalUp) > 0.001f) {
                horizontalUp = glm::normalize(horizontalUp);
            }
            if(m_EnableFly)
                m_Position -= horizontalUp * m_CameraSpeed * dt;
            break;
        }
        default:
            break;
        }
        UpdateView();
    }
    void Camera::CameraView(float xoffset, float yoffset){
        m_Yaw += xoffset * m_Sensitive;
        m_Pitch += yoffset * m_Sensitive;
        if(m_Pitch > 89.0f) m_Pitch = 89.0f;
        if(m_Pitch < -89.0f) m_Pitch = -89.0f;
        UpdateView();
    }
}