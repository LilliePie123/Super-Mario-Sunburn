#pragma once

#include <Dolphin/CARD.h>
#include <Dolphin/types.h>

#include <BetterSMS/memory.hxx>
#include <BetterSMS/module.hxx>
#include <BetterSMS/settings.hxx>

using namespace BetterSMS;

extern const u8 gSaveBnr[];
extern const u8 gSaveIcon[];

class GamePlusSetting final : public Settings::SwitchSetting {
public:
    GamePlusSetting() : SwitchSetting("New Game Plus", &mGamePlusValue) {
        mValueChangedCB = valueChanged;
    }
    ~GamePlusSetting() override {}

    bool isUnlocked() const override { return sIsUnlocked; }

    void load(JSUMemoryInputStream &in) override {
        in.read(&sIsUnlocked, 1);
        {
            bool b;
            in.read(&b, 1);
            setBool(b);
        }
    }

    void save(JSUMemoryOutputStream &out) override {
        out.write(&sIsUnlocked, 1);
        out.write(&mGamePlusValue, 1);
    }

    void getValueName(char *dst) const override {
        getBool() ? strncpy(dst, "ON", 3) : strncpy(dst, "OFF", 4);
    }

    inline void lock() { sIsUnlocked = false; }
    inline void unlock() { sIsUnlocked = true; }

#define U32P(addr) reinterpret_cast<u32 *>(addr)

    static void valueChanged(void *old, void *cur, ValueKind kind) {
        if (*reinterpret_cast<bool *>(cur))
            PowerPC::writeU32(U32P(0x8026A3C8), 0x39000005);
        else
            PowerPC::writeU32(U32P(0x8026A3C8), 0x39000004);
    }

#undef U32P

private:
    bool mGamePlusValue;
    static bool sIsUnlocked;
};

// Invisible flag that doesn't show in settings menu
class WinConditionFlag final : public Settings::IntSetting {
public:
    enum Condition {
        SHINES_70,
        SHADOW_MARIO
    };

    WinConditionFlag() : IntSetting("Win Condition", &mWinConditionFlag) {
        mValueRange     = {0, 1, 1};
    }

    ~WinConditionFlag() override {}

    void getValueName(char *dst) const override {
        getInt() == 0 ? strncpy(dst, "70 Shines", 10) : strncpy(dst, "All Shadow Mario", 17);
    }

    void load(JSUMemoryInputStream &in) override {
        IntSetting::load(in);
    }

private:
    bool mWinConditionFlag;
};

bool isGamePlusMode();
WinConditionFlag::Condition getWinCondition();