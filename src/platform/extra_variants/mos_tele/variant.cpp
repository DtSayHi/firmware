#include "configuration.h"

#ifdef MOS_TELE

#include <Arduino.h>
#include <Wire.h>

#include "AudioBoard.h"
#include "DebugConfiguration.h"

DriverPins mosTeleAudioPins;
AudioBoard mosTeleAudioBoard(AudioDriverES8311, mosTeleAudioPins);

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

void lateInitVariant()
{
    digitalWrite(MOS_TELE_AUDIO_POWER, HIGH);
    delay(10);

    mosTeleAudioPins.addI2C(PinFunction::CODEC, Wire);
    mosTeleAudioPins.addI2S(PinFunction::CODEC, DAC_I2S_MCLK, DAC_I2S_BCK, DAC_I2S_WS, DAC_I2S_DOUT, DAC_I2S_DIN);

    CodecConfig cfg;
    cfg.input_device = ADC_INPUT_LINE1;
    cfg.output_device = DAC_OUTPUT_ALL;
    cfg.i2s.bits = BIT_LENGTH_16BITS;
    cfg.i2s.rate = RATE_44K;
    mosTeleAudioBoard.begin(cfg);
    mosTeleAudioBoard.setVolume(80);
    LOG_INFO("MOS-TELE ES8311 audio codec initialized");
}

#endif
