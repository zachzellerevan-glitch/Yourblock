#pragma once
#include "YBpch.h"
#include "Render/Shader.h"
#include "Core/Camera/Camera.h"

namespace Engine{
    class WireframeRenderer{
    public:
        WireframeRenderer();   // 编译 shader + 建方体线框 VAO
        ~WireframeRenderer();

        // 画任意位置/大小的线框盒
        void DrawBox(const glm::vec3 & center, float halfExtent,
                     const glm::vec4 & color,
                     const glm::mat4 & view, const glm::mat4 & projection);
        // 便捷版：画一个方块（移到格子中心 + 外扩防 z-fighting）
        void DrawBlockBox(const glm::ivec3 & blockPos, const glm::vec4 & color,
                          const glm::mat4 & view, const glm::mat4 & projection);
    private:
        std::unique_ptr<Shader> m_Shader;
        unsigned int m_VAO = 0, m_VBO = 0, m_EBO = 0;
    };
}