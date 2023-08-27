#include <SMS/raw_fn.hxx>

#include <BetterSMS/module.hxx>

using namespace BetterSMS;

// 0x80299230
// extern -> SME.cpp
static void setStageOnExit(TGameSequence *gpSequence, s8 stage, s8 episode,
                           JDrama::TFlagT<u16> flag) {
    TMarioGamePad *gpGamePad = gpApplication.mGamePads[0];

    if (gpGamePad->mButtons.mInput & TMarioGamePad::EButtons::Z) {
        if (SMS_isExMap__Fv() || gpApplication.mPrevScene.mAreaID != 1)  // Delfino Plaza
            stage = gpApplication.mPrevScene.mAreaID;
        else
            stage = gpApplication.mCurrentScene.mAreaID;
        episode = -1;
    }
    gpSequence->set(stage, episode, flag);
}
SMS_PATCH_BL(SMS_PORT_REGION(0x80299230, 0x802910C8, 0, 0), setStageOnExit);