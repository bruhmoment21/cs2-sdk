#pragma once

#include <cache/entities/base.hpp>

class CCSPlayerController;

class CCachedPlayer : public CCachedBaseEntity {
   public:
    enum class Team { UNKNOWN = 0, SPECTATOR, TERRORIST, COUNTER_TERRORIST };

    Type GetType() const override { return Type::PLAYER; }

    auto Get() const { return CCachedBaseEntity::Get<CCSPlayerController>(); }

    bool CanDoESP() override;
    void DrawESP() override;
    void CalculateDrawInfo() override;

    Team GetTeam();
    bool IsEnemyWithTeam(Team team);
    bool IsLocalPlayer();
};
