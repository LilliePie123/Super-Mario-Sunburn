#include <Dolphin/types.h>
#include <Dolphin/CARD.h>
#include <Dolphin/math.h>
#include <Dolphin/OS.h>
#include <Dolphin/string.h>

#include <JSystem/J2D/J2DOrthoGraph.hxx>
#include <JSystem/J2D/J2DTextBox.hxx>

#include <SMS/System/Application.hxx>

#include <BetterSMS/game.hxx>
#include <BetterSMS/module.hxx>
#include <BetterSMS/stage.hxx>
#include <BetterSMS/loading.hxx>
#include <BetterSMS/settings.hxx>
#include <BetterSMS/player.hxx>
#include <BetterSMS/stage.hxx>

#include "coop.hxx"
#include "settings.hxx"

extern Settings::SettingsGroup gSettingsGroup;

// Module definition
BetterSMS::ModuleInfo gModuleInfo("Super Mario Sunburn", 1, 8, &gSettingsGroup);

// Init settings
extern void initializeSettings(Settings::SettingsGroup &group);

// Save Flag Fixes
extern void enableYoshiAfterBowserDefeat(TMarDirector *);
extern void enableNokiBayEveryWhere(TMarDirector *);

// New Game Plus
extern void checkFor120ShinesCollected(TMarDirector *director);
extern void forceParams(TMario *player, bool isMario);

// Cutscene Skip
BETTER_SMS_FOR_CALLBACK static void CutsceneSkip(TApplication* cutscene) { //These functions type cast and roll new values on loop using TMarDirector (light level)
    BetterSMS::PowerPC::writeU32((u32*)0x802B5EF4, 0x38600001);
    BetterSMS::PowerPC::writeU32((u32*)0x802B5E8C, 0x38600001);
}

// Yoshi
extern void adjustYoshiTounge(TMario *player, bool isMario);

static void initModule() {
    // Register callbacks
    initializeSettings(gSettingsGroup);
    {
        auto &saveInfo        = gSettingsGroup.getSaveInfo();
        saveInfo.mSaveName    = BetterSMS::Settings::getGroupName(gSettingsGroup);
        saveInfo.mBlocks      = 1;
        saveInfo.mGameCode    = 'GMSE';
        saveInfo.mCompany     = 0x3033;  // '01'
        saveInfo.mBannerFmt   = CARD_BANNER_CI;
        saveInfo.mBannerImage = reinterpret_cast<const ResTIMG *>(gSaveBnr);
        saveInfo.mIconFmt     = CARD_ICON_CI;
        saveInfo.mIconSpeed   = CARD_SPEED_SLOW;
        saveInfo.mIconCount   = 2;
        saveInfo.mIconTable   = reinterpret_cast<const ResTIMG *>(gSaveIcon);
        saveInfo.mSaveGlobal  = true;
    }
    BetterSMS::registerModule(gModuleInfo);

    BetterSMS::Game::addBootCallback(CutsceneSkip);
    BetterSMS::Stage::addInitCallback(enableYoshiAfterBowserDefeat);
    BetterSMS::Stage::addInitCallback(enableNokiBayEveryWhere);
    BetterSMS::Stage::addUpdateCallback(checkFor120ShinesCollected);
    BetterSMS::Player::addUpdateCallback(forceParams);
    BetterSMS::Player::addUpdateCallback(adjustYoshiTounge);
}

// Definition block
KURIBO_MODULE_BEGIN("Super Mario Sunburn", "EpicWade", "v1.0") {
    // Set the load and unload callbacks to our registration functions
    KURIBO_EXECUTE_ON_LOAD { initModule(); }
}
KURIBO_MODULE_END()
