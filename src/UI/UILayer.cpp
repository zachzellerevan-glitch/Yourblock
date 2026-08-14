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

    void UILayer::DrawHotBar(){
        const float slot = 40.0f, gap = 4.0f, pad = 4.0f;
        const unsigned int SlotCount = 9;
        const float barW = SlotCount * slot + (SlotCount - 1) * gap + 2 * pad;   // 9*48 + 8*4 + 8 = 472
        const float barH = slot + 2 * pad;                      // 56
        const float barX = (m_Width  - barW) / 2.0f;            // 水平居中
        const float barY = m_Height - barH - 8.0f;              // 底部留 8px
        m_Renderer->DrawQuad(barX, barY, barW, barH, glm::vec4(0.0f,0.0f,0.0f,0.4f));

        for(int i = 0; i < SlotCount; i++){
            const float sx = barX + pad + i * (slot + gap);    // 第 i 格左边缘
            const float sy = barY + pad;

            if(i == m_Hotbar->SelectedSlot)                    // L3 选中描边
                m_Renderer->DrawQuad(sx-2, sy-2, slot+4, slot+4, glm::vec4(1,1,1,1));

            m_Renderer->DrawQuad(sx, sy, slot, slot,           // L2 格子底色
                                glm::vec4(0.1f, 0.1f, 0.1f, 0.7f));

            // L4 图标 —— 先用纯色占位,以后换贴图(见第 3 步)
            m_Renderer->DrawQuad(sx+8, sy+8, slot-16, slot-16, glm::vec4(1,0,0,1));
        }
    }

    void UILayer::OnUpdate(float dt){
        m_Width = (float)Application::GetApp().GetWindow().GetWidth();
        m_Height = (float)Application::GetApp().GetWindow().GetHeight();

        m_Renderer->BeginFrame(m_Width, m_Height);
        // 热栏/按钮/文字
        const float barLen = 6.0f, thick = 2.0f;
        float cx = m_Width / 2.0f, cy = m_Height / 2.0f;
        m_Renderer->DrawQuad(cx - barLen, cy - thick/2, barLen*2, thick, glm::vec4(1,1,1,1));
        m_Renderer->DrawQuad(cx - thick/2, cy - barLen, thick, barLen*2, glm::vec4(1,1,1,1));
        if(m_HotbarState == true)
            DrawHotBar();
        
        m_Renderer->EndFrame();
    }

}