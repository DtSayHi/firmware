#include "ST7305Frame.h"

namespace ST7305Frame
{
namespace
{
uint8_t reverseBits(uint8_t value)
{
    value = ((value & 0x55) << 1) | ((value & 0xAA) >> 1);
    value = ((value & 0x33) << 2) | ((value & 0xCC) >> 2);
    return (value << 4) | (value >> 4);
}
} // namespace

void pack(const uint8_t *source, uint8_t *destination)
{
    size_t output = 0;
    for (uint16_t x = 0; x < WIDTH; x += 2) {
        for (uint16_t page = 0; page < HEIGHT / 8; page++) {
            const size_t sourcePage = HEIGHT / 8 - 1 - page;
            uint8_t left = reverseBits(source[sourcePage * WIDTH + WIDTH - 1 - x]);
            uint8_t right = reverseBits(source[sourcePage * WIDTH + WIDTH - 2 - x]);

            destination[output++] = ((left & 0x01) << 7) | ((right & 0x01) << 6) | ((left & 0x02) << 4) | ((right & 0x02) << 3) |
                                    ((left & 0x04) << 1) | (right & 0x04) | ((left & 0x08) >> 2) | ((right & 0x08) >> 3);

            left >>= 4;
            right >>= 4;
            destination[output++] = ((left & 0x01) << 7) | ((right & 0x01) << 6) | ((left & 0x02) << 4) | ((right & 0x02) << 3) |
                                    ((left & 0x04) << 1) | (right & 0x04) | ((left & 0x08) >> 2) | ((right & 0x08) >> 3);
        }
    }
}
} // namespace ST7305Frame
