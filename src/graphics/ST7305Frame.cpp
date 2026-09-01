#include "ST7305Frame.h"

namespace ST7305Frame
{
void pack(const uint8_t *source, uint8_t *destination)
{
    size_t output = 0;
    for (uint16_t x = 0; x < WIDTH; x += 2) {
        for (uint16_t page = 0; page < HEIGHT / 8; page++) {
            uint8_t left = source[page * WIDTH + x];
            uint8_t right = source[page * WIDTH + x + 1];

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
