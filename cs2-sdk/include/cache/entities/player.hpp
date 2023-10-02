#pragma once

#include <cache/entities/base.hpp>

class CCachedPlayer : public CCachedBaseEntity {
   public:
    CCachedPlayer(CBaseHandle handle);

    bool CanDrawESP() override;
    void RenderESP() override;
    void UpdateESP() override;

    uint8_t GetTeam();
    bool IsEnemyWithLocalPlayer();
    bool IsLocalPlayer();

   private:
    void UpdateBoxColor();
};
