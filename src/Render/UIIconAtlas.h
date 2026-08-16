#pragma once
#include "glm/glm.hpp"
#include "World/Block.h"

namespace Engine{
    class UIIconAtlas{
        public:
            static UIIconAtlas & Get(){
                static UIIconAtlas Instance;
                return Instance;
            };
            void Build();
            glm::vec4 GetUV(BlockType block) const;
            unsigned int GetTextureID() const;
        private:
            UIIconAtlas() = default;
            unsigned int m_TextureID = 0;
            std::unordered_map<BlockType, glm::vec4> m_BlockUV;
            bool m_Built = false;
    };
}