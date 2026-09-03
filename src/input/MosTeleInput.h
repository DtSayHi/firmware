#pragma once

#include <cstdint>

enum class MosTeleKey : uint8_t {
    NONE,
    WHEEL_UP,
    WHEEL_DOWN,
    WHEEL_SELECT,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SELECT,
    BUTTON_47,
    BUTTON_48,
};

enum class MosTeleAction : uint8_t {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SELECT,
    CANCEL,
    BACK,
};

namespace MosTeleInput
{
MosTeleKey classifyWheel(uint16_t millivolts);
MosTeleKey classifyJoystick(uint16_t xMillivolts, uint16_t yMillivolts);
MosTeleAction actionForKey(MosTeleKey key);
} // namespace MosTeleInput
