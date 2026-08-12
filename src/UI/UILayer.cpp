#include "UILayer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstddef>

namespace Engine{
    void UILayer::OnAttach(){
        m_Renderer = std::make_unique<Renderer2D>();
    }

    void UILayer::OnDetach(){
        m_Renderer.reset();
    }

    void UILayer::OnEvent(Event & event){}

    void UILayer::OnUpdate(float dt){
        float w = (float)Application::GetApp().GetWindow().GetWidth();
        float h = (float)Application::GetApp().GetWindow().GetHeight();

        m_Renderer->BeginFrame(w, h);
        // 热栏/按钮/文字
        const float barLen = 6.0f, thick = 2.0f;
        float cx = w / 2.0f, cy = h / 2.0f;
        m_Renderer->DrawQuad(cx - barLen, cy - thick/2, barLen*2, thick, glm::vec4(1,1,1,1));
        m_Renderer->DrawQuad(cx - thick/2, cy - barLen, thick, barLen*2, glm::vec4(1,1,1,1));
        m_Renderer->EndFrame();
    }

}