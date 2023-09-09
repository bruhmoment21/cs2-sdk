#pragma once

#include "../../virtual.hpp"

class CCSPlayerController;

struct UnkGameEventStruct_t {
    UnkGameEventStruct_t(const char* keyName) {
        m_Unk = 0;
        m_Key = keyName;
    }

    uint64_t m_Unk;
    const char* m_Key;
};

class CGameEvent {
   public:
    auto GetName() { return CALL_VIRTUAL(const char*, 1, this); }

    auto GetPlayerController(const char* keyName) {
        // I don't know the real name of this function. It calls some function
        // with the value returned by GetInt().

        UnkGameEventStruct_t arg(keyName);
        return CALL_VIRTUAL(CCSPlayerController*, 16, this, &arg);
    }

    auto SetString(const char* keyName, const char* value) {
        // I think "value" type has became a "CBufferString".

#if 0
        UnkGameEventStruct_t arg(keyName);
        return CALL_VIRTUAL(void, 24, this, &arg, value);
#endif
    }
};
