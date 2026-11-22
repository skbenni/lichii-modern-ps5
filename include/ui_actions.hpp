#pragma once

#include <cstdint>

enum class UiAction : std::uint8_t {
    None,
    Up,
    Down,
    Left,
    Right,
    Confirm,
    Back,
    NextTab,
    PrevTab,
    Quit,
};

struct PadState {
    std::uint32_t buttons = 0;
    std::int8_t left_x = 0;
    std::int8_t left_y = 0;
    bool connected = false;
};

class PadBackend {
public:
    virtual ~PadBackend() = default;
    virtual bool Open() = 0;
    virtual bool Read(PadState& state) = 0;
    virtual void Rumble(std::uint8_t large, std::uint8_t small) = 0;
    virtual void LightBar(std::uint8_t r, std::uint8_t g, std::uint8_t b) = 0;
    virtual void Close() = 0;
};

class NullPadBackend final : public PadBackend {
public:
    bool Open() override {
        return true;
    }
    bool Read(PadState& state) override {
        state = PadState{};
        return true;
    }
    void Rumble(std::uint8_t, std::uint8_t) override {}
    void LightBar(std::uint8_t, std::uint8_t, std::uint8_t) override {}
    void Close() override {}
};
