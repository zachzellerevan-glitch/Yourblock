#include "UILayer.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine{
    void UILayer::OnAttach(){
        m_UIShader = std::make_unique<Shader>("assets/Shader/UIVertex.glsl", "assets/Shader/UIFragment.glsl");
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        const float barLen = 5.0f, thick = 1.0f;
        float crosshair[] = {
            // 水平条
           -barLen, -thick/2,   barLen, -thick/2,   barLen,  thick/2,
           -barLen, -thick/2,   barLen,  thick/2,  -barLen,  thick/2,
            // 垂直条
           -thick/2, -barLen,   thick/2, -barLen,   thick/2,  barLen,
           -thick/2, -barLen,   thick/2,  barLen,  -thick/2,  barLen,
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(crosshair), crosshair,GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void UILayer::OnDetach(){
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        m_UIShader.reset();
    }

    void UILayer::OnEvent(Event & event){}

    void UILayer::OnUpdate(float dt){
        glDisable(GL_DEPTH_TEST);
        m_UIShader->Use();
        float w = (float)Application::GetApp().GetWindow().GetWidth();
        float h = (float)Application::GetApp().GetWindow().GetHeight();
        glm::mat4 ortho = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);

        glm::mat4 transform = ortho * glm::translate(glm::mat4(1.0f), glm::vec3(w / 2.0f + 0.5f, h / 2.0f + 0.5f, 0.0f));
        m_UIShader->SetUniform("u_Transform", transform);
        m_UIShader->SetUniform("u_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0 ,12);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }
}