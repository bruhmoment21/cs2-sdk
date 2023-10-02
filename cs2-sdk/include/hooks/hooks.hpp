#pragma once

class CHooks {
   public:
    static CHooks& Get() {
        static CHooks inst;
        return inst;
    }

    void Initialize();
    void Shutdown();

   private:
    ~CHooks();
};
