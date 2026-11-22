#pragma once

#include "ui_actions.hpp"

#include <array>
#include <cstdint>

class PadInput {
public:
    explicit PadInput(PadBackend& backend);

    bool Initialize();
    void Poll();
    UiAction Consume();
    bool Down(UiAction action) const;
    void Feedback(UiAction action);
    void Shutdown();

private:
    static constexpr std::uint32_t kButtonCreate = 0x00000001U;
    static constexpr std::uint32_t kButtonL3 = 0x00000002U;
    static constexpr std::uint32_t kButtonR3 = 0x00000004U;
    static constexpr std::uint32_t kButtonOptions = 0x00000008U;
    static constexpr std::uint32_t kButtonUp = 0x00000010U;
    static constexpr std::uint32_t kButtonRight = 0x00000020U;
    static constexpr std::uint32_t kButtonDown = 0x00000040U;
    static constexpr std::uint32_t kButtonLeft = 0x00000080U;
    static constexpr std::uint32_t kButtonL1 = 0x00000400U;
    static constexpr std::uint32_t kButtonR1 = 0x00000800U;
    static constexpr std::uint32_t kButtonTriangle = 0x00001000U;
    static constexpr std::uint32_t kButtonCircle = 0x00002000U;
    static constexpr std::uint32_t kButtonCross = 0x00004000U;
    static constexpr std::uint32_t kButtonSquare = 0x00008000U;

    static constexpr std::uint32_t kActionCount = 10;
    static constexpr std::uint32_t kRepeatDelay = 18;
    static constexpr std::uint32_t kRepeatRate = 4;
    static constexpr std::int8_t kDeadzone = 24;

    std::uint32_t ToMask(const PadState& state) const;
    void Direction(std::uint32_t mask, std::uint32_t previous);
    void Button(UiAction action, bool pressed);
    void Queue(UiAction action);

    PadBackend& backend_;
    std::uint32_t previous_ = 0;
    std::array<std::uint32_t, kActionCount> hold_{};
    UiAction queued_ = UiAction::None;
    std::uint32_t tick_ = 0;
    bool ready_ = false;
};
