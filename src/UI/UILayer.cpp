#include "UILayer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstddef>

namespace Engine{
    void UILayer::OnAttach(){
        m_UIShader = std::make_unique<Shader>("assets/Shader/UIVertex.glsl", "assets/Shader/UIFragment.glsl");
        m_UIShader->SetUniform("u_Texture",0);

        //1x1 white texture for pure color block
        glGenTextures(1, &m_WhiteTexture);
        glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);
        unsigned char white[4] = {255,255,255,255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glGenVertexArrays(1, &m_QuadVAO);
        glGenBuffers(1,&m_QuadVBO);
        glBindVertexArray(m_QuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);

        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(UIVertex),(void*)offsetof(UIVertex,Position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(UIVertex),(void*)offsetof(UIVertex,UV));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,4,GL_FLOAT,GL_FALSE,sizeof(UIVertex),(void*)offsetof(UIVertex,Color));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void UILayer::OnDetach(){
        glDeleteVertexArrays(1, &m_QuadVAO);
        glDeleteBuffers(1, &m_QuadVBO);
        glDeleteTextures(1, &m_WhiteTexture);
        m_UIShader.reset();
    }

    void UILayer::OnEvent(Event & event){}

    void UILayer::OnUpdate(float dt){
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        m_UIShader->Use();
        float w = (float)Application::GetApp().GetWindow().GetWidth();
        float h = (float)Application::GetApp().GetWindow().GetHeight();
        glm::mat4 ortho = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);

        
        m_UIShader->SetUniform("u_Transform", ortho);
        const float barLen = 6.0f, thick = 1.2f;
        float cx = w / 2.0f, cy = h / 2.0f;
        DrawQuad(cx - barLen, cy - thick/2, barLen*2, thick, glm::vec4(1.0f,1.0f,1.0f,1.0f)); // 横条
        DrawQuad(cx - thick/2, cy - barLen, thick, barLen*2, glm::vec4(1.0f,1.0f,1.0f,1.0f)); // 竖条

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }

    void UILayer::DrawQuad(float x, float y, float w, float h, const glm::vec4 & color, const glm::vec2 & uvMin,
                        const glm::vec2 & uvMax, unsigned int texture){
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
}