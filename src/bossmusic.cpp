#include <SMS/System/MarDirector.hxx>
#include <SMS/MSound/MSBGM.hxx>
#include <SMS/MSound/MSound.hxx>
#include <SMS/MSound/MSoundSESystem.hxx>

#include <BetterSMS/module.hxx>

s32 bossKillDemoCameraCallback(u32 value, u32 state) {
    if (state == 1) {
        MSBgm::startBGM(gStageBGM);
    }
    return 0;
}

// the name is short because kuribo symbol shit (too long = fail)
static void i(TMarDirector *director, const char *demo_path, const TVec3f *pos, s32 unk_1, f32 unk_2, bool unk_3,
    s32(*cb)(u32, u32), u32 unk_4, JDrama::TActor *target,
    JDrama::TFlagT<u16> flags) {
    director->fireStartDemoCamera(demo_path, pos, unk_1, unk_2, unk_3, bossKillDemoCameraCallback,
                                  unk_4, target, flags);
}
SMS_PATCH_BL(SMS_PORT_REGION(0x80073908, 0, 0, 0), i);
SMS_PATCH_BL(SMS_PORT_REGION(0x80073960, 0, 0, 0), i);
SMS_PATCH_BL(SMS_PORT_REGION(0x80073998, 0, 0, 0), i);