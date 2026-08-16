#pragma once

namespace Engine{
    enum class GameState { Playing, Paused, InMenu };
    struct GameUIState { GameState State = GameState::Playing; };
}

