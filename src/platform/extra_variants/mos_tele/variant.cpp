#include "configuration.h"

#ifdef MOS_TELE

#include <Arduino.h>

void earlyInitVariant()
{
    pinMode(LORA_CS, OUTPUT);
    digitalWrite(LORA_CS, HIGH);
    pinMode(ST7305_CS, OUTPUT);
    digitalWrite(ST7305_CS, HIGH);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, !LED_STATE_ON);

    pinMode(MOS_TELE_AUDIO_POWER, OUTPUT);
    digitalWrite(MOS_TELE_AUDIO_POWER, LOW);
    pinMode(MOS_TELE_AUDIO_ENABLE, OUTPUT);
    digitalWrite(MOS_TELE_AUDIO_ENABLE, LOW);
}

#endif
