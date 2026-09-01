#include "ST7305Display.h"

#ifdef USE_ST7305

#include "SPILock.h"
#include "concurrency/LockGuard.h"
#include <Arduino.h>
#include <SPI.h>

namespace
{
constexpr uint32_t SPI_FREQUENCY = 30000000;

struct InitCommand {
    uint8_t command;
    const uint8_t *data;
    uint8_t length;
    uint16_t delayMs;
};

const uint8_t initD6[] = {0x13, 0x02};
const uint8_t initD1[] = {0x01};
const uint8_t initC0[] = {0x08, 0x06};
const uint8_t initC1[] = {0x3F, 0x3E, 0x3C, 0x3C};
const uint8_t initC2[] = {0x23, 0x21, 0x23, 0x23};
const uint8_t initC4[] = {0x5F, 0x5C, 0x5A, 0x5A};
const uint8_t initC5[] = {0x37, 0x35, 0x37, 0x37};
const uint8_t initB2[] = {0x10};
const uint8_t initB3[] = {0xE5, 0xF6, 0x17, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x71};
const uint8_t initB4[] = {0x05, 0x46, 0x77, 0x77, 0x77, 0x77, 0x76, 0x45};
const uint8_t init62[] = {0x32, 0x03, 0x1F};
const uint8_t initB7[] = {0x13};
const uint8_t initB0[] = {0x60};
const uint8_t initC9[] = {0x00};
const uint8_t init36[] = {0x00};
const uint8_t init3A[] = {0x11};
const uint8_t initB9[] = {0x20};
const uint8_t initB8[] = {0x29};
const uint8_t init2A[] = {0x17, 0x24};
const uint8_t init2B[] = {0x00, 0xBF};
const uint8_t initD0[] = {0x00};

const InitCommand initCommands[] = {
    {0xD6, initD6, sizeof(initD6), 0},
    {0xD1, initD1, sizeof(initD1), 0},
    {0xC0, initC0, sizeof(initC0), 0},
    {0xC1, initC1, sizeof(initC1), 0},
    {0xC2, initC2, sizeof(initC2), 0},
    {0xC4, initC4, sizeof(initC4), 0},
    {0xC5, initC5, sizeof(initC5), 0},
    {0xB2, initB2, sizeof(initB2), 0},
    {0xB3, initB3, sizeof(initB3), 0},
    {0xB4, initB4, sizeof(initB4), 0},
    {0x62, init62, sizeof(init62), 0},
    {0xB7, initB7, sizeof(initB7), 0},
    {0xB0, initB0, sizeof(initB0), 0},
    {0x11, nullptr, 0, 100},
    {0xC9, initC9, sizeof(initC9), 0},
    {0x36, init36, sizeof(init36), 0},
    {0x3A, init3A, sizeof(init3A), 0},
    {0xB9, initB9, sizeof(initB9), 0},
    {0xB8, initB8, sizeof(initB8), 0},
    {0x2A, init2A, sizeof(init2A), 0},
    {0x2B, init2B, sizeof(init2B), 0},
    {0xD0, initD0, sizeof(initD0), 0},
    {0x39, nullptr, 0, 0},
    {0x29, nullptr, 0, 100},
};
} // namespace

ST7305Display::ST7305Display()
{
    setGeometry(GEOMETRY_RAWMODE, ST7305Frame::WIDTH, ST7305Frame::HEIGHT);
    wireBuffer = new uint8_t[ST7305Frame::BUFFER_SIZE];
}

ST7305Display::~ST7305Display()
{
    delete[] wireBuffer;
}

bool ST7305Display::connect()
{
    if (!wireBuffer)
        return false;

    pinMode(ST7305_CS, OUTPUT);
    pinMode(ST7305_DC, OUTPUT);
    pinMode(ST7305_RESET, OUTPUT);
    digitalWrite(ST7305_CS, HIGH);
    digitalWrite(LORA_CS, HIGH);

    digitalWrite(ST7305_RESET, HIGH);
    delay(1);
    digitalWrite(ST7305_RESET, LOW);
    delay(10);
    digitalWrite(ST7305_RESET, HIGH);
    delay(10);

    for (const auto &entry : initCommands) {
        sendPanelCommand(entry.command, entry.data, entry.length);
        if (entry.delayMs)
            delay(entry.delayMs);
    }
    return true;
}

void ST7305Display::sendPanelCommandUnlocked(uint8_t command, const uint8_t *data, size_t length)
{
    digitalWrite(ST7305_CS, LOW);
    digitalWrite(ST7305_DC, LOW);
    SPI.transfer(command);
    if (length) {
        digitalWrite(ST7305_DC, HIGH);
        SPI.transferBytes(data, nullptr, length);
    }
    digitalWrite(ST7305_CS, HIGH);
}

void ST7305Display::sendPanelCommand(uint8_t command, const uint8_t *data, size_t length)
{
    concurrency::LockGuard guard(spiLock);
    digitalWrite(LORA_CS, HIGH);
    SPI.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
    sendPanelCommandUnlocked(command, data, length);
    SPI.endTransaction();
}

void ST7305Display::sendCommand(uint8_t command)
{
    switch (command) {
    case DISPLAYON:
        sendPanelCommand(0x29);
        break;
    case DISPLAYOFF:
        sendPanelCommand(0x28);
        break;
    case INVERTDISPLAY:
        sendPanelCommand(0x21);
        break;
    case NORMALDISPLAY:
        sendPanelCommand(0x20);
        break;
    default:
        break;
    }
}

void ST7305Display::display()
{
    if (!buffer || !wireBuffer)
        return;
    if (digitalRead(LORA_DIO1) == HIGH)
        return;

    ST7305Frame::pack(buffer, wireBuffer);

    const uint8_t column[] = {0x17, 0x24};
    const uint8_t row[] = {0x00, 0xBF};
    concurrency::LockGuard guard(spiLock);
    digitalWrite(LORA_CS, HIGH);
    SPI.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
    sendPanelCommandUnlocked(0x2A, column, sizeof(column));
    sendPanelCommandUnlocked(0x2B, row, sizeof(row));

    digitalWrite(ST7305_CS, LOW);
    digitalWrite(ST7305_DC, LOW);
    SPI.transfer(0x2C);
    digitalWrite(ST7305_DC, HIGH);
    SPI.transferBytes(wireBuffer, nullptr, ST7305Frame::BUFFER_SIZE);
    digitalWrite(ST7305_CS, HIGH);
    SPI.endTransaction();
}

#endif
