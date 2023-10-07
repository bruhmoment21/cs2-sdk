#pragma once

class CEngineClient {
   public:
    static CEngineClient* Get();

    bool IsInGame();
    int GetLocalPlayer();
    int GetEngineBuildNumber();
};
