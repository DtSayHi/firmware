#pragma once

#define MOS_TELE 1

// Display
#define HAS_SCREEN 1
#define USE_ST7305 1
#define OLED_GEOMETRY_OVERRIDE GEOMETRY_RAWMODE
#define ST7305_CS 15
#define ST7305_DC 8
#define ST7305_RESET 7

// Shared SPI2 bus: Ra-01SC and ST7305 use separate chip selects.
#define LORA_SCK 18
#define LORA_MOSI 16
#define LORA_MISO 17

// Ra-01SC / LLCC68 (410-525 MHz), crystal version without TCXO.
#define USE_LLCC68
#define LORA_CS 11
#define LORA_RESET 19
#define LORA_DIO1 20
#define LORA_BUSY 46

#define SX126X_CS LORA_CS
#define SX126X_RESET LORA_RESET
#define SX126X_DIO1 LORA_DIO1
#define SX126X_BUSY LORA_BUSY
#define SX126X_DIO2_AS_RF_SWITCH
#define SX126X_TXEN RADIOLIB_NC
#define SX126X_RXEN RADIOLIB_NC
#define SX126X_MAX_POWER 22

// I2C peripherals include QMI8658, RX8010SJ, ES8311 and ES7210.
#define I2C_SDA 1
#define I2C_SCL 2

// Battery divider from the reference firmware.
#define BATTERY_PIN 6
#define ADC_CHANNEL ADC_CHANNEL_5
#define ADC_ATTENUATION ADC_ATTEN_DB_12
#define ADC_MULTIPLIER 2.0f

// InputBroker is provided by MosTeleInputSource. GPIO0 is a boot strap pin,
// but the reference board already uses it as the joystick switch.
#define HAS_BUTTON 1
#define MOS_TELE_WHEEL_PIN 3
#define MOS_TELE_JOYSTICK_Y_PIN 4
#define MOS_TELE_JOYSTICK_X_PIN 5
#define MOS_TELE_JOYSTICK_Y_CHANNEL ADC_CHANNEL_3
#define MOS_TELE_JOYSTICK_X_CHANNEL ADC_CHANNEL_4
#define MOS_TELE_JOYSTICK_SELECT_PIN 0
#define MOS_TELE_BUTTON_47_PIN 47
#define MOS_TELE_BUTTON_48_PIN 48

#define LED_PIN 42
#define LED_STATE_ON HIGH

// ES8311 codec and speaker amplifier.
#define MOS_TELE_AUDIO_POWER 10
#define MOS_TELE_AUDIO_ENABLE 21
#define MOS_TELE_I2S_DIN 12
#define MOS_TELE_I2S_WS 13
#define MOS_TELE_I2S_BCLK 14
#define MOS_TELE_I2S_MCLK 38
#define MOS_TELE_I2S_DOUT 45
#define HAS_I2S
#define DAC_I2S_BCK MOS_TELE_I2S_BCLK
#define DAC_I2S_WS MOS_TELE_I2S_WS
#define DAC_I2S_DOUT MOS_TELE_I2S_DOUT
#define DAC_I2S_DIN MOS_TELE_I2S_DIN
#define DAC_I2S_MCLK MOS_TELE_I2S_MCLK
#define AUDIO_AMP_ENABLE(on) digitalWrite(MOS_TELE_AUDIO_ENABLE, (on) ? HIGH : LOW)

// Reserved peripheral wiring.
#define MOS_TELE_SD_D0 39
#define MOS_TELE_SD_CLK 40
#define MOS_TELE_SD_CMD 41
#define MOS_TELE_RX8010SJ_ADDR 0x32
