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
    BetterSMS::registerModule(&gModuleInfo);

    BetterSMS::Stage::registerInitCallback("Sunburn_YoshiFlagFix", enableYoshiAfterBowserDefeat);
    BetterSMS::Stage::registerInitCallback("Sunburn_NokiBayFix", enableNokiBayEveryWhere);
    BetterSMS::Stage::registerUpdateCallback("Sunburn_NewGamePlusUnlocker",
                                             checkFor120ShinesCollected);
    BetterSMS::Player::registerUpdateCallback("Sunburn_NewGamePlusParams", forceParams);
    BetterSMS::Player::registerUpdateCallback("Sunburn_AdjustYoshiTongue", adjustYoshiTounge);
}

static void deinitModule() {
    // Cleanup callbacks
    BetterSMS::Stage::deregisterInitCallback("Sunburn_YoshiFlagFix");
    BetterSMS::Stage::deregisterInitCallback("Sunburn_NokiBayFix");
    BetterSMS::Stage::deregisterUpdateCallback("Sunburn_NewGamePlusUnlocker");
    BetterSMS::Player::deregisterUpdateCallback("Sunburn_NewGamePlusParams");
    BetterSMS::Player::deregisterUpdateCallback("Sunburn_AdjustYoshiTongue");

    BetterSMS::deregisterModule(&gModuleInfo);
}

// Definition block
KURIBO_MODULE_BEGIN("Super Mario Sunburn", "Wade & JoshuaMK", "v1.8") {
    // Set the load and unload callbacks to our registration functions
    KURIBO_EXECUTE_ON_LOAD { initModule(); }
    KURIBO_EXECUTE_ON_UNLOAD { deinitModule(); }
}
KURIBO_MODULE_END()
