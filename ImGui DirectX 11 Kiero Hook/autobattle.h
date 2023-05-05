#pragma once

namespace globals {

    extern bool auto_battle;
}

namespace abel {
    void setAutoBattle();
}

typedef enum GamePhase
{
    RPG_LOGIN = 7,
    RPG_WORLD = 12,
    RPG_BATTLE = 15,
};