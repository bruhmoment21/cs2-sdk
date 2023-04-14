#pragma once

#include "../../virtual.hpp"

class CCSPlayerController;

class CGameEvent {
   public:
    auto GetName() { return CALL_VIRTUAL(const char*, 1, this); }

    auto GetPlayerController(const char* keyName) {
        // I don't know the real name of this function. It calls some function
        // with the value returned by GetInt().
        return CALL_VIRTUAL(CCSPlayerController*, 16, this, keyName);
    }

    auto SetString(const char* keyName, const char* value) {
        return CALL_VIRTUAL(void, 24, this, keyName, value);
    }
};
