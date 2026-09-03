#include "graphics/ST7305Frame.h"
#include "input/MosTeleInput.h"

#include <array>
#include <unity.h>

void setUp(void) {}
void tearDown(void) {}

void test_st7305_frame_rotates_pixels_180_degrees()
{
    std::array<uint8_t, ST7305Frame::BUFFER_SIZE> source{};
    std::array<uint8_t, ST7305Frame::BUFFER_SIZE> wire{};

    source[0] = 0x01;                             // (0, 0)
    source[1] = 0x01;                             // (1, 0)
    source[ST7305Frame::WIDTH * 20 + 383] = 0x80; // (383, 167)

    ST7305Frame::pack(source.data(), wire.data());

    TEST_ASSERT_EQUAL_HEX8(0x80, wire[0]);
    TEST_ASSERT_EQUAL_HEX8(0x00, wire[1]);
    TEST_ASSERT_EQUAL_HEX8(0x03, wire[ST7305Frame::BUFFER_SIZE - 1]);
}

void test_st7305_frame_reverses_pixel_pair_order()
{
    std::array<uint8_t, ST7305Frame::BUFFER_SIZE> source{};
    std::array<uint8_t, ST7305Frame::BUFFER_SIZE> wire{};
    source[2] = 0x01; // (2, 0)

    ST7305Frame::pack(source.data(), wire.data());

    TEST_ASSERT_EQUAL_HEX8(0x00, wire[ST7305Frame::BUFFER_SIZE - 1]);
    TEST_ASSERT_EQUAL_HEX8(0x01, wire[ST7305Frame::BUFFER_SIZE - 43]);
}

void test_buttons_map_to_confirm_and_cancel()
{
    TEST_ASSERT_EQUAL(MosTeleAction::SELECT, MosTeleInput::actionForKey(MosTeleKey::BUTTON_47));
    TEST_ASSERT_EQUAL(MosTeleAction::CANCEL, MosTeleInput::actionForKey(MosTeleKey::BUTTON_48));
}

void test_joystick_select_maps_to_text_backspace()
{
    TEST_ASSERT_EQUAL(MosTeleAction::BACK, MosTeleInput::actionForKey(MosTeleKey::SELECT));
}

void test_only_joystick_directions_produce_navigation_actions()
{
    TEST_ASSERT_EQUAL(MosTeleAction::UP, MosTeleInput::actionForKey(MosTeleKey::UP));
    TEST_ASSERT_EQUAL(MosTeleAction::DOWN, MosTeleInput::actionForKey(MosTeleKey::DOWN));
    TEST_ASSERT_EQUAL(MosTeleAction::LEFT, MosTeleInput::actionForKey(MosTeleKey::LEFT));
    TEST_ASSERT_EQUAL(MosTeleAction::RIGHT, MosTeleInput::actionForKey(MosTeleKey::RIGHT));
    TEST_ASSERT_EQUAL(MosTeleAction::NONE, MosTeleInput::actionForKey(MosTeleKey::WHEEL_UP));
    TEST_ASSERT_EQUAL(MosTeleAction::NONE, MosTeleInput::actionForKey(MosTeleKey::WHEEL_DOWN));
    TEST_ASSERT_EQUAL(MosTeleAction::NONE, MosTeleInput::actionForKey(MosTeleKey::WHEEL_SELECT));
}

void test_joystick_uses_reference_directions_and_x_axis_priority()
{
    TEST_ASSERT_EQUAL(MosTeleKey::RIGHT, MosTeleInput::classifyJoystick(799, 2500));
    TEST_ASSERT_EQUAL(MosTeleKey::LEFT, MosTeleInput::classifyJoystick(2401, 700));
    TEST_ASSERT_EQUAL(MosTeleKey::DOWN, MosTeleInput::classifyJoystick(1600, 799));
    TEST_ASSERT_EQUAL(MosTeleKey::UP, MosTeleInput::classifyJoystick(1600, 2401));
    TEST_ASSERT_EQUAL(MosTeleKey::NONE, MosTeleInput::classifyJoystick(1600, 1600));
}

void test_failed_joystick_adc_read_does_not_generate_direction()
{
    TEST_ASSERT_EQUAL(MosTeleKey::NONE, MosTeleInput::classifyJoystickSample(false, 0, true, 1600));
    TEST_ASSERT_EQUAL(MosTeleKey::NONE, MosTeleInput::classifyJoystickSample(true, 1600, false, 0));
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_st7305_frame_rotates_pixels_180_degrees);
    RUN_TEST(test_st7305_frame_reverses_pixel_pair_order);
    RUN_TEST(test_buttons_map_to_confirm_and_cancel);
    RUN_TEST(test_joystick_select_maps_to_text_backspace);
    RUN_TEST(test_only_joystick_directions_produce_navigation_actions);
    RUN_TEST(test_joystick_uses_reference_directions_and_x_axis_priority);
    RUN_TEST(test_failed_joystick_adc_read_does_not_generate_direction);
    return UNITY_END();
}
