#include "MosTeleInput.h"

namespace MosTeleInput
{
MosTeleKey classifyWheel(uint16_t millivolts)
{
    if (millivolts < 300)
        return MosTeleKey::WHEEL_DOWN;
    if (millivolts < 500)
        return MosTeleKey::WHEEL_SELECT;
    if (millivolts < 600)
        return MosTeleKey::WHEEL_UP;
    return MosTeleKey::NONE;
}

MosTeleKey classifyJoystick(uint16_t xMillivolts, uint16_t yMillivolts)
{
    if (xMillivolts < 800)
        return MosTeleKey::RIGHT;
    if (xMillivolts > 2400)
        return MosTeleKey::LEFT;
    if (yMillivolts < 800)
        return MosTeleKey::DOWN;
    if (yMillivolts > 2400)
        return MosTeleKey::UP;
    return MosTeleKey::NONE;
}
} // namespace MosTeleInput
