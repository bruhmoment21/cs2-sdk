#pragma once

class CVulkanHooks {
   public:
    static CVulkanHooks& Get() {
        static CVulkanHooks inst;
        return inst;
    }

    void Initialize();

   private:
    ~CVulkanHooks();
};
