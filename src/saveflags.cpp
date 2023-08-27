#include <Dolphin/OS.h>

#include <SMS/Manager/FlagManager.hxx>
#include <SMS/System/MarDirector.hxx>
#include <SMS/raw_fn.hxx>

#include <BetterSMS/module.hxx>

#include "settings.hxx"


BETTER_SMS_FOR_CALLBACK void enableYoshiAfterBowserDefeat(TMarDirector *director) {
	const bool is_bowser_defeated = TFlagManager::smInstance->getBool(0x10077);

    if (!is_bowser_defeated)
        return;

    TFlagManager::smInstance->setBool(true, 0x1038F);
}

BETTER_SMS_FOR_CALLBACK void enableNokiBayEveryWhere(TMarDirector *director) {
    if (director->mAreaID != 1)
        return;

    TFlagManager::smInstance->setBool(true, 0x50004);
}

static bool enableModelGatesAfterBowserDefeat(bool gate_unlocked) {
    const bool is_bowser_defeated = TFlagManager::smInstance->getBool(0x10077);
    return gate_unlocked || is_bowser_defeated;
}
SMS_PATCH_BL(SMS_PORT_REGION(0x801EC858, 0, 0, 0), enableModelGatesAfterBowserDefeat);
SMS_WRITE_32(SMS_PORT_REGION(0x801EC85C, 0, 0, 0), 0x28030001);

static u8 handleProgressionType(u8 area) {
    const bool is_corona_visited = TFlagManager::smInstance->getBool(0x103AE);
    if (is_corona_visited)
        return decideNextScenario__FUc(area);

    switch (getWinCondition()) {
    case WinConditionFlag::SHINES_70: {
        return TFlagManager::smInstance->getFlag(0x40000) >= 70 ? 9 : decideNextScenario__FUc(area);
    }
    case WinConditionFlag::SHADOW_MARIO: {
        return decideNextScenario__FUc(area);
    }
    }
}
SMS_PATCH_BL(SMS_PORT_REGION(0x802976AC, 0, 0, 0),  handleProgressionType);