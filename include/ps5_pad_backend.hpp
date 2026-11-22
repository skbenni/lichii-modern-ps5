#pragma once

#include "ui_actions.hpp"

class Ps5PadBackend final : public PadBackend {
public:
    bool Open() override;
    bool Read(PadState& state) override;
    void Rumble(std::uint8_t large, std::uint8_t small) override;
    void LightBar(std::uint8_t r, std::uint8_t g, std::uint8_t b) override;
    void Close() override;

private:
    int handle_ = -1;
    int user_ = -1;
    std::uint8_t deadzone_left_ = 24;
};
