#include "graphics/ST7305Frame.h"
#include "input/MosTeleInput.h"

#include <array>
#include <unity.h>

void setUp(void) {}
void tearDown(void) {}

void test_st7305_frame_packs_pixel_pairs_in_reference_order()
{
    std::array<uint8_t, ST7305Frame::BUFFER_SIZE> source{};
    std::array<uint8_t, ST7305Frame::BUFFER_SIZE> wire{};

    source[0] = 0x01;                             // (0, 0)
    source[1] = 0x01;                             // (1, 0)
    source[ST7305Frame::WIDTH * 20 + 383] = 0x80; // (383, 167)

    ST7305Frame::pack(source.data(), wire.data());

    TEST_ASSERT_EQUAL_HEX8(0xC0, wire[0]);
    TEST_ASSERT_EQUAL_HEX8(0x00, wire[1]);
    TEST_ASSERT_EQUAL_HEX8(0x01, wire[ST7305Frame::BUFFER_SIZE - 1]);
}

void test_st7305_frame_places_next_pixel_pair_after_42_bytes()
{
    std::array<uint8_t, ST7305Frame::BUFFER_SIZE> source{};
    std::array<uint8_t, ST7305Frame::BUFFER_SIZE> wire{};
    source[2] = 0x01; // (2, 0)

    ST7305Frame::pack(source.data(), wire.data());

    TEST_ASSERT_EQUAL_HEX8(0x00, wire[0]);
    TEST_ASSERT_EQUAL_HEX8(0x80, wire[42]);
}

void test_wheel_thresholds_match_reference_firmware()
{
    TEST_ASSERT_EQUAL(MosTeleKey::WHEEL_DOWN, MosTeleInput::classifyWheel(299));
    TEST_ASSERT_EQUAL(MosTeleKey::WHEEL_SELECT, MosTeleInput::classifyWheel(300));
    TEST_ASSERT_EQUAL(MosTeleKey::WHEEL_SELECT, MosTeleInput::classifyWheel(499));
    TEST_ASSERT_EQUAL(MosTeleKey::WHEEL_UP, MosTeleInput::classifyWheel(500));
    TEST_ASSERT_EQUAL(MosTeleKey::NONE, MosTeleInput::classifyWheel(600));
}

void test_joystick_uses_reference_directions_and_x_axis_priority()
{
    TEST_ASSERT_EQUAL(MosTeleKey::RIGHT, MosTeleInput::classifyJoystick(799, 2500));
    TEST_ASSERT_EQUAL(MosTeleKey::LEFT, MosTeleInput::classifyJoystick(2401, 700));
    TEST_ASSERT_EQUAL(MosTeleKey::DOWN, MosTeleInput::classifyJoystick(1600, 799));
    TEST_ASSERT_EQUAL(MosTeleKey::UP, MosTeleInput::classifyJoystick(1600, 2401));
    TEST_ASSERT_EQUAL(MosTeleKey::NONE, MosTeleInput::classifyJoystick(1600, 1600));
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_st7305_frame_packs_pixel_pairs_in_reference_order);
    RUN_TEST(test_st7305_frame_places_next_pixel_pair_after_42_bytes);
    RUN_TEST(test_wheel_thresholds_match_reference_firmware);
    RUN_TEST(test_joystick_uses_reference_directions_and_x_axis_priority);
    return UNITY_END();
}
