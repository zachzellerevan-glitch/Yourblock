#include "Render/Renderer2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstddef>

namespace Engine{
    Renderer2D::Renderer2D(){
        m_Shader = std::make_unique<Shader>("assets/Shader/UIVertex.glsl", "assets/Shader/UIFragment.glsl");
        m_Shader->SetUniform("u_Texture", 0);

        glGenTextures(1, &m_WhiteTexture);
        glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);
        unsigned char white[4] = {255,255,255,255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);

        glGenVertexArrays(1, &m_QuadVAO);
        glGenBuffers(1, &m_QuadVBO);
        glBindVertexArray(m_QuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, Position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, UV));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, Color));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    Renderer2D::~Renderer2D(){
        glDeleteVertexArrays(1, &m_QuadVAO);
        glDeleteBuffers(1, &m_QuadVBO);
        glDeleteTextures(1, &m_WhiteTexture);
    }

    void Renderer2D::BeginFrame(float w, float h){
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_Ortho = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
        m_Shader->Use();
        m_Shader->SetUniform("u_Transform", m_Ortho);
    }

    void Renderer2D::EndFrame(){
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer2D::DrawQuad(float x, float y, float w, float h,
                              const glm::vec4 & color,
                              const glm::vec2 & uvMin,
                              const glm::vec2 & uvMax,
                              unsigned int texture){
        if(texture == 0) texture = m_WhiteTexture;
        UIVertex verts[6] = {
            {{x,   y  }, {uvMin.x, uvMin.y}, color},
            {{x+w, y  }, {uvMax.x, uvMin.y}, color},
            {{x+w, y+h}, {uvMax.x, uvMax.y}, color},
            {{x,   y  }, {uvMin.x, uvMin.y}, color},
            {{x+w, y+h}, {uvMax.x, uvMax.y}, color},
            {{x,   y+h}, {uvMin.x, uvMax.y}, color},
        };
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(m_QuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
};
