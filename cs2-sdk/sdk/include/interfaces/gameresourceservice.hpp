#pragma once

class CGameEntitySystem;

class CGameResourceService {
   public:
    static CGameResourceService* Get();

    CGameEntitySystem* GetGameEntitySystem();
};
