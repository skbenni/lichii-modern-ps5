#include "ps5_pad_backend.hpp"

#if defined(LICHII_PS5)

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

namespace ps5_interop {

inline constexpr std::int32_t kPortTypeStandard = 0;
inline constexpr std::size_t kSampleSize = 120;

struct Stick {
    std::uint8_t x;
    std::uint8_t y;
};

struct AnalogButtons {
    std::uint8_t l2;
    std::uint8_t r2;
    std::array<std::uint8_t, 2> reserved;
};

struct Quaternion {
    float x;
    float y;
    float z;
    float w;
};

struct Vector3 {
    float x;
    float y;
    float z;
};

struct Touch {
    std::uint16_t x;
    std::uint16_t y;
    std::uint8_t id;
    std::array<std::uint8_t, 3> reserved;
};

struct TouchData {
    std::uint8_t count;
    std::array<std::uint8_t, 7> reserved;
    std::array<Touch, 2> points;
};

struct ExtensionData {
    std::uint32_t unit_id;
    std::uint8_t reserved;
    std::uint8_t length;
    std::array<std::uint8_t, 10> data;
};

struct Data {
    std::uint32_t buttons;
    Stick left_stick;
    Stick right_stick;
    AnalogButtons triggers;
    Quaternion orientation;
    Vector3 acceleration;
    Vector3 angular_velocity;
    TouchData touch;
    std::int32_t connected;
    std::uint64_t timestamp_us;
    ExtensionData extension;
    std::uint8_t connected_count;
    std::array<std::uint8_t, 2> reserved;
    std::uint8_t device_unique_data_length;
    std::array<std::uint8_t, 12> device_unique_data;
};

struct StickInformation {
    std::uint8_t dead_zone_left;
    std::uint8_t dead_zone_right;
};

struct TouchpadInformation {
    float pixel_density;
    std::uint16_t resolution_x;
    std::uint16_t resolution_y;
};

struct ControllerInformation {
    TouchpadInformation touchpad;
    StickInformation sticks;
    std::uint8_t connection_type;
    std::uint8_t connected_count;
    std::int32_t connected;
    std::int32_t device_class;
    std::array<std::uint8_t, 8> reserved;
};

struct Vibration {
    std::uint8_t large_motor;
    std::uint8_t small_motor;
};

struct Color {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t reserved;
};

static_assert(sizeof(Data) == kSampleSize);
static_assert(offsetof(Data, touch) == 0x34);
static_assert(offsetof(Data, connected) == 0x4c);
static_assert(offsetof(Data, timestamp_us) == 0x50);
static_assert(sizeof(ControllerInformation) == 28);

extern "C" {

std::int32_t sceUserServiceInitialize(void* parameters);
std::int32_t sceUserServiceGetInitialUser(std::int32_t* user_id);
std::int32_t sceUserServiceTerminate();
std::int32_t scePadInit();
std::int32_t scePadOpen(std::int32_t user_id, std::int32_t port_type,
                        std::int32_t index, const void* parameters);
std::int32_t scePadClose(std::int32_t handle);
std::int32_t scePadReadState(std::int32_t handle, Data* data);
std::int32_t scePadGetControllerInformation(
    std::int32_t handle, ControllerInformation* information);
std::int32_t scePadSetVibration(std::int32_t handle, const Vibration* vibration);
std::int32_t scePadSetLightBar(std::int32_t handle, const Color* color);

}  // extern "C"

}  // namespace ps5_interop

bool Ps5PadBackend::Open() {
    using namespace ps5_interop;
    if (sceUserServiceInitialize(nullptr) < 0) {
        return false;
    }
    if (sceUserServiceGetInitialUser(&user_) < 0) {
        sceUserServiceTerminate();
        return false;
    }
    if (scePadInit() < 0) {
        sceUserServiceTerminate();
        return false;
    }
    handle_ = scePadOpen(user_, kPortTypeStandard, 0, nullptr);
    if (handle_ < 0) {
        sceUserServiceTerminate();
        return false;
    }
    ControllerInformation info{};
    if (scePadGetControllerInformation(handle_, &info) == 0 &&
        info.sticks.dead_zone_left >= 24) {
        deadzone_left_ = info.sticks.dead_zone_left;
    }
    return true;
}

bool Ps5PadBackend::Read(PadState& state) {
    using namespace ps5_interop;
    Data data{};
    if (scePadReadState(handle_, &data) < 0) {
        return false;
    }
    state = PadState{};
    state.buttons = data.buttons;
    state.connected = data.connected != 0;
    const auto axis = [this](std::uint8_t raw) -> std::int8_t {
        const int value = static_cast<int>(raw) - 128;
        if (std::abs(value) < deadzone_left_) {
            return 0;
        }
        return static_cast<std::int8_t>(std::clamp(value, -127, 127));
    };
    state.left_x = axis(data.left_stick.x);
    state.left_y = axis(data.left_stick.y);
    return true;
}

void Ps5PadBackend::Rumble(std::uint8_t large, std::uint8_t small) {
    const ps5_interop::Vibration vibration{large, small};
    ps5_interop::scePadSetVibration(handle_, &vibration);
}

void Ps5PadBackend::LightBar(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
    const ps5_interop::Color color{r, g, b, 0};
    ps5_interop::scePadSetLightBar(handle_, &color);
}

void Ps5PadBackend::Close() {
    if (handle_ >= 0) {
        ps5_interop::scePadClose(handle_);
        handle_ = -1;
    }
    ps5_interop::sceUserServiceTerminate();
}

#endif
