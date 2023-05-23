#include <Dolphin/OS.h>

#include <SMS/Manager/FlagManager.hxx>
#include <SMS/System/MarDirector.hxx>
#include <SMS/MoveBG/NozzleBox.hxx>
#include <SMS/raw_fn.hxx>

#include <BetterSMS/module.hxx>
#include <BetterSMS/player.hxx>
#include <BetterSMS/stage.hxx>

#include "settings.hxx"

extern GamePlusSetting sGamePlusMode;

BETTER_SMS_FOR_CALLBACK void checkFor120ShinesCollected(TMarDirector *director) {
    if (!sGamePlusMode.isUnlocked()) {
        if (TFlagManager::smInstance->getFlag(0x40000) >= 120) {
            sGamePlusMode.unlock();
        }
    }
}

BETTER_SMS_FOR_CALLBACK void forceParams(TMario *player, bool isMario) {
    auto *stage_config = Stage::getStageConfiguration();
    if (isMario && isGamePlusMode() && stage_config) {
        player->mDeParams.mHPMax.set(stage_config->mPlayerMaxHealth.get() >> 1);
        player->mHealth = Min(player->mHealth, 4);
        if (player->mFludd->mSecondNozzle == 0 || player->mFludd->mSecondNozzle == 4)
            player->mFludd->mSecondNozzle = 5;
    }
}

static void replaceAllHoversWithTurbos() {
    TNozzleBox *box;
    SMS_FROM_GPR(30, box);

    if (isGamePlusMode()) {
        box->mNozzleType = TWaterGun::Turbo;
        box->mMatColor.r = 90;
        box->mMatColor.g = 90;
        box->mMatColor.b = 120;
        box->mBoxName    = "back_nozzle_item";
    } else {
        box->mNozzleType = TWaterGun::Hover;
        box->mMatColor.r = 0;
        box->mMatColor.g = 0;
        box->mMatColor.b = 255;
    }
}
SMS_PATCH_BL(SMS_PORT_REGION(0x801BB308, 0, 0, 0), replaceAllHoversWithTurbos);
SMS_WRITE_32(SMS_PORT_REGION(0x801BB30C, 0, 0, 0), 0x480000CC);

static void forceTurboNozzleOnHover1() {
    TWaterGun::TNozzleType nozzleKind;
    SMS_FROM_GPR(30, nozzleKind);

    TWaterGun *fludd;
    SMS_FROM_GPR(31, fludd);

    if (isGamePlusMode() && nozzleKind == 4) {
        fludd->mSecondNozzle = 5;
    } else {
        fludd->mSecondNozzle = nozzleKind;
    }
}
SMS_PATCH_BL(SMS_PORT_REGION(0x8026A1F0, 0, 0, 0), forceTurboNozzleOnHover1);

static void forceTurboNozzleOnHover2() {
    TWaterGun::TNozzleType nozzleKind;
    SMS_FROM_GPR(30, nozzleKind);

    TWaterGun *fludd;
    SMS_FROM_GPR(31, fludd);

    if (isGamePlusMode() && nozzleKind == 4) {
        fludd->mCurrentNozzle = 5;
    } else {
        fludd->mCurrentNozzle = nozzleKind;
    }
}
SMS_PATCH_BL(SMS_PORT_REGION(0x8026A204, 0, 0, 0), forceTurboNozzleOnHover2);

SMS_WRITE_32(SMS_PORT_REGION(0x801E68A0, 0, 0, 0), 0x60000000);