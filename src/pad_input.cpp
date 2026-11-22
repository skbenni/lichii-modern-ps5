#include "pad_input.hpp"

namespace {

std::size_t ActionIndex(UiAction action) {
    return static_cast<std::size_t>(action);
}

struct DirectionBinding {
    std::uint32_t bit;
    UiAction action;
};

}  // namespace

PadInput::PadInput(PadBackend& backend) : backend_(backend) {}

bool PadInput::Initialize() {
    if (!backend_.Open()) {
        return false;
    }
    ready_ = true;
    backend_.LightBar(240, 56, 107);
    return true;
}

std::uint32_t PadInput::ToMask(const PadState& state) const {
    std::uint32_t mask = state.buttons;
    if (state.left_x < -kDeadzone) {
        mask |= kButtonLeft;
    }
    if (state.left_x > kDeadzone) {
        mask |= kButtonRight;
    }
    if (state.left_y < -kDeadzone) {
        mask |= kButtonUp;
    }
    if (state.left_y > kDeadzone) {
        mask |= kButtonDown;
    }
    return mask;
}

void PadInput::Queue(UiAction action) {
    queued_ = action;
}

void PadInput::Direction(std::uint32_t mask, std::uint32_t previous) {
    const DirectionBinding bindings[] = {
        {kButtonUp, UiAction::Up},
        {kButtonDown, UiAction::Down},
        {kButtonLeft, UiAction::Left},
        {kButtonRight, UiAction::Right},
    };
    for (const auto& binding : bindings) {
        const bool pressed = (mask & binding.bit) != 0;
        const bool was_pressed = (previous & binding.bit) != 0;
        std::uint32_t& held = hold_[ActionIndex(binding.action)];
        if (pressed && !was_pressed) {
            held = 1;
            Queue(binding.action);
        } else if (pressed) {
            ++held;
            if (held > kRepeatDelay &&
                (held - kRepeatDelay) % kRepeatRate == 0) {
                Queue(binding.action);
            }
        } else {
            held = 0;
        }
    }
}

void PadInput::Button(UiAction action, bool pressed) {
    std::uint32_t& held = hold_[ActionIndex(action)];
    if (pressed && held == 0) {
        Queue(action);
    }
    held = pressed ? held + 1 : 0;
}

void PadInput::Poll() {
    if (!ready_) {
        return;
    }
    PadState state;
    if (!backend_.Read(state)) {
        return;
    }
    const std::uint32_t mask = ToMask(state);
    Direction(mask, previous_);
    Button(UiAction::Confirm, (mask & kButtonCross) != 0);
    Button(UiAction::Back, (mask & kButtonCircle) != 0);
    Button(UiAction::NextTab, (mask & kButtonR1) != 0);
    Button(UiAction::PrevTab, (mask & kButtonL1) != 0);
    Button(UiAction::Quit, (mask & kButtonOptions) != 0);
    previous_ = mask;
    ++tick_;
}

UiAction PadInput::Consume() {
    const UiAction action = queued_;
    queued_ = UiAction::None;
    return action;
}

bool PadInput::Down(UiAction action) const {
    return hold_[ActionIndex(action)] != 0;
}

void PadInput::Feedback(UiAction action) {
    switch (action) {
        case UiAction::Confirm:
            backend_.Rumble(48, 24);
            break;
        case UiAction::Back:
            backend_.Rumble(16, 8);
            break;
        case UiAction::Quit:
            backend_.LightBar(201, 93, 93);
            break;
        case UiAction::Up:
        case UiAction::Down:
        case UiAction::Left:
        case UiAction::Right:
            backend_.LightBar(244, 90, 133);
            break;
        default:
            break;
    }
}

void PadInput::Shutdown() {
    if (ready_) {
        backend_.Close();
        ready_ = false;
    }
    hold_.fill(0);
    queued_ = UiAction::None;
    previous_ = 0;
    tick_ = 0;
}
