#pragma once

#include "YBpch.h"
#include "Core/Core.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Engine{
    class UILayer : public Layer{
        public:
            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate(float dt) override;
            virtual void OnEvent(Event & event) override;
        private:
    };
}