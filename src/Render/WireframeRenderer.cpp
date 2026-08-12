#include "WireframeRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine{
    WireframeRenderer::WireframeRenderer(){
        m_Shader = std::make_unique<Shader>("assets/Shader/OutlineVertex.glsl","assets/Shader/OutlineFragment.glsl");

        // 单位立方体 8 顶点 + 12 条边 24 索引
        float cubeVerts[] = {
           -0.5f,-0.5f,-0.5f,   0.5f,-0.5f,-0.5f,   0.5f, 0.5f,-0.5f,  -0.5f, 0.5f,-0.5f,
           -0.5f,-0.5f, 0.5f,   0.5f,-0.5f, 0.5f,   0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,
        };
        unsigned int cubeEdges[] = {
            0,1, 1,2, 2,3, 3,0,
            4,5, 5,6, 6,7, 7,4,
            0,4, 1,5, 2,6, 3,7,
        };
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeEdges), cubeEdges, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    WireframeRenderer::~WireframeRenderer(){
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void WireframeRenderer::DrawBox(const glm::vec3 & center, float halfExtent,
                                    const glm::vec4 & color,
                                    const glm::mat4 & view, const glm::mat4 & projection){
        m_Shader->Use();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), center)
                        * glm::scale(glm::mat4(1.0f), glm::vec3(halfExtent * 2.0f));
        m_Shader->SetUniform("u_Model", model);
        m_Shader->SetUniform("u_View", view);
        m_Shader->SetUniform("u_Projection", projection);
        m_Shader->SetUniform("u_Color", color);
        glBindVertexArray(m_VAO);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void WireframeRenderer::DrawBlockBox(const glm::ivec3 & blockPos, const glm::vec4 & color,
                                         const glm::mat4 & view, const glm::mat4 & projection){
        DrawBox(glm::vec3(blockPos) + glm::vec3(0.5f), 0.51f, color, view, projection);  // 0.51 = 外扩 2%
    }
}