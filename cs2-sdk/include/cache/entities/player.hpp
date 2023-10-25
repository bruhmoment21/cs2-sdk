#pragma once

#include <cache/entities/base.hpp>

class CCachedPlayer : public CCachedBaseEntity {
   public:
    Type GetType() const override { return Type::PLAYER; }

    bool CanDrawESP() override;
    void RenderESP() override;
    void UpdateESP() override;

    uint8_t GetTeam();
    bool IsEnemyWithLocalPlayer();
    bool IsLocalPlayer();

   private:
    void UpdateBoxColor();
};
