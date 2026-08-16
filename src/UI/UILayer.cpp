#include "UILayer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstddef>
#include "Render/UIIconAtlas.h"
#include "Render/GlyphAtlas.h"
#include <cmath>
#include <cstdio>
#include <cstdarg>

namespace Engine{
    void DrawText(Renderer2D & r, const char* text, float x, float y,
              const glm::vec4 & color, float scale, float lineHeightScale = 1.4f){
        auto & atlas = GlyphAtlas::Get();
        const float baked = atlas.GetPixelSize();
        const float lineH = baked * scale * lineHeightScale;   // 行高也随 scale

        float cx = x;
        float baseline = y + baked * scale;                    // 基线随 scale
        const char* p = text;

        while(*p){
            unsigned int cp = DecodeUTF8(p);
            if(cp == '\n'){ baseline += lineH; cx = x; continue; }

            const Glyph & g = atlas.GetGlyph(cp);
            if(g.Size.x <= 0.0f && g.Size.y <= 0.0f){ cx += g.Advance * scale; continue; }

            // ★ 三处全乘 scale:尺寸、bearing、advance
            float w = g.Size.x * scale;
            float h = g.Size.y * scale;
            float qx = std::floor(cx + g.Bearing.x * scale + 0.5f);
            float qy = std::floor(baseline - g.Bearing.y * scale + 0.5f);
            r.DrawQuad(qx, qy, w, h, color,
                    glm::vec2(g.UV.x, g.UV.y), glm::vec2(g.UV.z, g.UV.w),
                    atlas.GetTextureID());
            cx += g.Advance * scale;
        }
    }

    void DrawTextF(Renderer2D & r, float x, float y, const glm::vec4 & color,
               float scale, float lightHeightScale, const char* fmt, ...){
        char buf[256];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);   // 格式化进缓冲,超长自动截断(安全)
        va_end(args);
        DrawText(r, buf, x, y, color, scale, lightHeightScale);
    }
    void UILayer::OnAttach(){
        m_Renderer = std::make_unique<Renderer2D>();
        UIIconAtlas::Get().Build();
        GlyphAtlas::Get().Build("assets/Font/Font.ttf", 48);
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

            // L4 图标 —— 先用纯色占位,以后换贴图
            const glm::vec4 uv = UIIconAtlas::Get().GetUV(m_Hotbar->Slots[i]);
            m_Renderer->DrawQuad(sx+8, sy+8, slot-16, slot-16, glm::vec4(1.0f),
                                glm::vec2(uv.x, uv.y), glm::vec2(uv.z, uv.w),
                                UIIconAtlas::Get().GetTextureID());
        }
    }

    void UILayer::OnUpdate(float dt){
        if(dt > 0.0001f)
            m_FPS = m_FPS * 0.9f + (1.0f / dt) * 0.1f;
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
        DrawTextF(*m_Renderer, 10, 10, glm::vec4(1,1,1,1), 0.6f, 1.5f, "FPS: %.1f", m_FPS);
        m_Renderer->EndFrame();
    }

}