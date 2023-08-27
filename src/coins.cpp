#include <Dolphin/OS.h>

#include <SMS/Manager/FlagManager.hxx>
#include <SMS/System/MarDirector.hxx>
#include <SMS/raw_fn.hxx>
#include <BetterSMS/module.hxx>

static void incGoldCoinsOnRedCoin(TFlagManager *manager, size_t amount, s32 flag) {
    manager->incFlag(amount, flag);
    manager->incGoldCoinFlag(SMS_getShineStage__FUc(gpApplication.mCurrentScene.mAreaID), 2);
}
SMS_PATCH_BL(SMS_PORT_REGION(0x801BE454, 0, 0, 0), incGoldCoinsOnRedCoin);