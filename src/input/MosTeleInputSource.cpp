#include "MosTeleInputSource.h"

#ifdef MOS_TELE

#include "Throttle.h"
#include "UptimeClock.h"
#include <Arduino.h>

namespace
{
constexpr uint32_t POLL_INTERVAL_MS = 20;
constexpr uint32_t DEBOUNCE_MS = 40;
constexpr uint32_t REPEAT_MS = 500;
const char sourceName[] = "MOS-TELE";
} // namespace

MosTeleInputSource::MosTeleInputSource() : concurrency::OSThread(sourceName)
{
    pinMode(MOS_TELE_JOYSTICK_SELECT_PIN, INPUT_PULLUP);
    pinMode(MOS_TELE_BUTTON_47_PIN, INPUT_PULLUP);
    pinMode(MOS_TELE_BUTTON_48_PIN, INPUT_PULLUP);
    analogSetPinAttenuation(MOS_TELE_WHEEL_PIN, ADC_11db);
    analogSetPinAttenuation(MOS_TELE_JOYSTICK_X_PIN, ADC_11db);
    analogSetPinAttenuation(MOS_TELE_JOYSTICK_Y_PIN, ADC_11db);
    inputBroker->registerSource(this);
}

MosTeleKey MosTeleInputSource::readKey()
{
    if (digitalRead(MOS_TELE_JOYSTICK_SELECT_PIN) == LOW)
        return MosTeleKey::SELECT;
    if (digitalRead(MOS_TELE_BUTTON_47_PIN) == LOW)
        return MosTeleKey::BUTTON_47;
    if (digitalRead(MOS_TELE_BUTTON_48_PIN) == LOW)
        return MosTeleKey::BUTTON_48;

    MosTeleKey key = MosTeleInput::classifyWheel(analogReadMilliVolts(MOS_TELE_WHEEL_PIN));
    if (key != MosTeleKey::NONE)
        return key;

    return MosTeleInput::classifyJoystick(analogReadMilliVolts(MOS_TELE_JOYSTICK_X_PIN),
                                          analogReadMilliVolts(MOS_TELE_JOYSTICK_Y_PIN));
}

bool MosTeleInputSource::isRepeatable(MosTeleKey key) const
{
    return key == MosTeleKey::WHEEL_UP || key == MosTeleKey::WHEEL_DOWN || key == MosTeleKey::UP || key == MosTeleKey::DOWN ||
           key == MosTeleKey::LEFT || key == MosTeleKey::RIGHT;
}

void MosTeleInputSource::sendKey(MosTeleKey key)
{
    InputEvent event = {};
    event.source = sourceName;
    switch (key) {
    case MosTeleKey::WHEEL_UP:
    case MosTeleKey::UP:
        event.inputEvent = INPUT_BROKER_UP;
        break;
    case MosTeleKey::WHEEL_DOWN:
    case MosTeleKey::DOWN:
        event.inputEvent = INPUT_BROKER_DOWN;
        break;
    case MosTeleKey::WHEEL_SELECT:
    case MosTeleKey::SELECT:
        event.inputEvent = INPUT_BROKER_SELECT;
        break;
    case MosTeleKey::LEFT:
        event.inputEvent = INPUT_BROKER_LEFT;
        break;
    case MosTeleKey::RIGHT:
        event.inputEvent = INPUT_BROKER_RIGHT;
        break;
    case MosTeleKey::BUTTON_47:
        event.inputEvent = INPUT_BROKER_BACK;
        break;
    case MosTeleKey::BUTTON_48:
        event.inputEvent = INPUT_BROKER_ALT_PRESS;
        break;
    default:
        return;
    }
    notifyObservers(&event);
}

int32_t MosTeleInputSource::runOnce()
{
    const uint32_t now = Time::getMillis();
    const MosTeleKey raw = readKey();

    if (raw != candidate) {
        candidate = raw;
        candidateSinceMs = now;
        return POLL_INTERVAL_MS;
    }

    if (candidate != stable && Throttle::hasElapsed(candidateSinceMs, DEBOUNCE_MS)) {
        stable = candidate;
        if (stable != MosTeleKey::NONE) {
            sendKey(stable);
            lastRepeatMs = now;
        }
    } else if (stable != MosTeleKey::NONE && isRepeatable(stable) && Throttle::hasElapsed(lastRepeatMs, REPEAT_MS)) {
        sendKey(stable);
        lastRepeatMs = now;
    }

    return POLL_INTERVAL_MS;
}

MosTeleInputSource *mosTeleInputSource = nullptr;

#endif
