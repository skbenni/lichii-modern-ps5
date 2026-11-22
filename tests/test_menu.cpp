#include "menu_model.hpp"
#include "pad_input.hpp"

#include <cassert>
#include <cstdint>

namespace {

constexpr std::uint32_t kUp = 0x00000010U;
constexpr std::uint32_t kDown = 0x00000040U;
constexpr std::uint32_t kLeft = 0x00000080U;
constexpr std::uint32_t kRight = 0x00000020U;
constexpr std::uint32_t kCross = 0x00004000U;
constexpr std::uint32_t kCircle = 0x00002000U;
constexpr std::uint32_t kR1 = 0x00000800U;
constexpr std::uint32_t kOptions = 0x00000008U;

class FakeBackend final : public PadBackend {
public:
    bool Open() override {
        return true;
    }
    bool Read(PadState& state) override {
        state = state_;
        return true;
    }
    void Rumble(std::uint8_t, std::uint8_t) override {}
    void LightBar(std::uint8_t, std::uint8_t, std::uint8_t) override {}
    void Close() override {}

    void Set(std::uint32_t buttons) {
        state_.buttons = buttons;
    }
    void Stick(std::int8_t x, std::int8_t y) {
        state_.left_x = x;
        state_.left_y = y;
    }

private:
    PadState state_;
};

MenuModel MakeModel() {
    MenuModel model;
    model.AddGame("/data/lichii/roms/a.nup", "A", "NUP");
    model.AddGame("/data/lichii/roms/b.nup", "B", "NUP");
    model.AddGame("/data/lichii/roms/c.nup", "C", "NUP");
    model.AddSetting("Renderer", "Vulkan", {"Vulkan", "OpenGL"});
    model.AddSetting("Aufloesung", "Skalierung", {"1x", "2x", "4x"}, 2);
    return model;
}

void TestGameNavigation() {
    auto model = MakeModel();
    assert(model.View().game_index == 0);
    model.Apply(UiAction::Up);
    assert(model.View().game_index == 2);
    model.Apply(UiAction::Down);
    assert(model.View().game_index == 0);
    model.Apply(UiAction::Right);
    assert(model.View().game_index == 1);
    assert(model.SelectedGamePath() == "/data/lichii/roms/b.nup");
}

void TestSettingsAndTabs() {
    auto model = MakeModel();
    model.Apply(UiAction::NextTab);
    assert(model.View().tab == MenuTab::Settings);
    model.Apply(UiAction::Right);
    assert(model.View().settings[0].value == 1);
    model.Apply(UiAction::Right);
    assert(model.View().settings[0].value == 0);
    model.Apply(UiAction::Down);
    assert(model.View().setting_index == 1);
    model.Apply(UiAction::Left);
    assert(model.View().settings[1].value == 1);
    model.Apply(UiAction::Back);
    assert(model.View().tab == MenuTab::Games);
}

void TestActions() {
    auto model = MakeModel();
    model.Apply(UiAction::Confirm);
    assert(model.View().message == "Start: A");
    model.Apply(UiAction::Quit);
    assert(model.QuitRequested());
}

void TestPadEdges() {
    FakeBackend backend;
    PadInput input(backend);
    assert(input.Initialize());

    backend.Set(kCross);
    input.Poll();
    assert(input.Consume() == UiAction::Confirm);
    input.Poll();
    assert(input.Consume() == UiAction::None);
    assert(input.Down(UiAction::Confirm));

    backend.Set(0);
    input.Poll();
    assert(!input.Down(UiAction::Confirm));

    backend.Set(kUp);
    input.Poll();
    assert(input.Consume() == UiAction::Up);
    int repeats = 0;
    for (int i = 0; i < 40; ++i) {
        input.Poll();
        if (input.Consume() == UiAction::Up) {
            ++repeats;
        }
    }
    assert(repeats > 1);

    backend.Set(0);
    input.Poll();
    backend.Set(kR1);
    input.Poll();
    assert(input.Consume() == UiAction::NextTab);
    backend.Set(0);
    input.Poll();
    backend.Set(kCircle);
    input.Poll();
    assert(input.Consume() == UiAction::Back);
    backend.Set(0);
    input.Poll();
    backend.Set(kOptions);
    input.Poll();
    assert(input.Consume() == UiAction::Quit);

    input.Shutdown();
}

void TestAnalogStick() {
    FakeBackend backend;
    PadInput input(backend);
    assert(input.Initialize());
    backend.Stick(-100, 0);
    input.Poll();
    assert(input.Consume() == UiAction::Left);
    backend.Stick(0, 100);
    input.Poll();
    assert(input.Consume() == UiAction::Down);
    backend.Stick(10, -10);
    input.Poll();
    assert(input.Consume() == UiAction::None);
    input.Shutdown();
}

}  // namespace

int main() {
    TestGameNavigation();
    TestSettingsAndTabs();
    TestActions();
    TestPadEdges();
    TestAnalogStick();
    return 0;
}
