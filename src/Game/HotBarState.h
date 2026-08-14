#pragma once
#include <array>
#include "World/Block.h"

namespace Engine{
    struct HotbarState {
        std::array<BlockType, 9> Slots;
        int SelectedSlot = 0;
    };
}