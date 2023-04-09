#pragma once

#define MAX_PLAYERS 64

enum EEconItemQuality {
    IQ_UNDEFINED = -1,
    IQ_NORMAL,
    IQ_GENUINE,
    IQ_VINTAGE,
    IQ_UNUSUAL,
    IQ_UNIQUE,
    IQ_COMMUNITY,
    IQ_DEVELOPER,
    IQ_SELFMADE,
    IQ_CUSTOMIZED,
    IQ_STRANGE,
    IQ_COMPLETED,
    IQ_HAUNTED,
    IQ_TOURNAMENT,
    IQ_FAVORED
};

enum EEconItemRarity {
    IR_DEFAULT,
    IR_COMMON,
    IR_UNCOMMON,
    IR_RARE,
    IR_MYTHICAL,
    IR_LEGENDARY,
    IR_ANCIENT,
    IR_IMMORTAL
};

// https://gitlab.com/KittenPopo/csgo-2018-source/-/blob/main/game/shared/econ/econ_item_constants.h#L39
enum EEconTypeID {
    k_EEconTypeItem = 1,
    k_EEconTypePersonaDataPublic = 2,
    k_EEconTypeGameAccountClient = 7,
    k_EEconTypeGameAccount = 8,
    k_EEconTypeEquipInstance = 31,
    k_EEconTypeDefaultEquippedDefinitionInstance = 42,
    k_EEconTypeDefaultEquippedDefinitionInstanceClient = 43,
    k_EEconTypeCoupon = 45,
    k_EEconTypeQuest = 46,
};
