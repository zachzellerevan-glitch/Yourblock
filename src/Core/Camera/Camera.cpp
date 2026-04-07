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
        case MoveDirection::FORWARD:
            m_Position += m_Front * m_CameraSpeed * dt;
            break;
        case MoveDirection::BACKWARD:
            m_Position -= m_Front * m_CameraSpeed * dt;
            break;
        case MoveDirection::LEFT:
            m_Position -= m_Right * m_CameraSpeed * dt;
            break;
        case MoveDirection::RIGHT:
            m_Position += m_Right * m_CameraSpeed * dt;
            break;
        case MoveDirection::UP:
            break;
        case MoveDirection::DOWN:
            break;
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