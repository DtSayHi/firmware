#include "MosTeleInputSource.h"

#ifdef MOS_TELE

#include "Throttle.h"
#include "UptimeClock.h"
#include "platform/extra_variants/mos_tele/MosTeleAdc.h"
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

    adcReady = mosTeleAdcInit();

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

    uint16_t xMillivolts = 0;
    uint16_t yMillivolts = 0;
    const bool xValid = adcReady && mosTeleAdcReadMilliVolts(MOS_TELE_JOYSTICK_X_CHANNEL, xMillivolts);
    const bool yValid = adcReady && mosTeleAdcReadMilliVolts(MOS_TELE_JOYSTICK_Y_CHANNEL, yMillivolts);
    return MosTeleInput::classifyJoystickSample(xValid, xMillivolts, yValid, yMillivolts);
}

bool MosTeleInputSource::isRepeatable(MosTeleKey key) const
{
    return key == MosTeleKey::UP || key == MosTeleKey::DOWN || key == MosTeleKey::LEFT || key == MosTeleKey::RIGHT;
}

void MosTeleInputSource::sendKey(MosTeleKey key)
{
    InputEvent event = {};
    event.source = sourceName;
    switch (MosTeleInput::actionForKey(key)) {
    case MosTeleAction::UP:
        event.inputEvent = INPUT_BROKER_UP;
        break;
    case MosTeleAction::DOWN:
        event.inputEvent = INPUT_BROKER_DOWN;
        break;
    case MosTeleAction::SELECT:
        event.inputEvent = INPUT_BROKER_SELECT;
        break;
    case MosTeleAction::LEFT:
        event.inputEvent = INPUT_BROKER_LEFT;
        break;
    case MosTeleAction::RIGHT:
        event.inputEvent = INPUT_BROKER_RIGHT;
        break;
    case MosTeleAction::BACK:
        event.inputEvent = INPUT_BROKER_BACK;
        break;
    case MosTeleAction::CANCEL:
        event.inputEvent = INPUT_BROKER_CANCEL;
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
