#pragma once

#include <cstddef>
#include <cstdint>

namespace ST7305Frame
{
constexpr uint16_t WIDTH = 384;
constexpr uint16_t HEIGHT = 168;
constexpr size_t BUFFER_SIZE = WIDTH * HEIGHT / 8;

void pack(const uint8_t *source, uint8_t *destination);
} // namespace ST7305Frame
