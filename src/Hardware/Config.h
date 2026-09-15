#pragma once
#include <cstdint>

// -------------------------
// I2C
// -------------------------

constexpr uint8_t SDA_PIN = 5;
constexpr uint8_t SCL_PIN = 4;

// -------------------------
// Float switch
// -------------------------

constexpr uint8_t FLOAT_PIN = 6;

// -------------------------
// OLED / Rotary encoder
// -------------------------

constexpr uint8_t OLED_CONFIRM_PIN = 15;
constexpr uint8_t OLED_PUSH_PIN = 16;
constexpr uint8_t OLED_ROTARY_CW_PIN = 17;
constexpr uint8_t OLED_ROTARY_CCW_PIN = 18;
constexpr uint8_t OLED_BACK_PIN = 8;

// -------------------------
// I2C addresses
// -------------------------

constexpr uint8_t SHT31_ADDRESS = 0x44;
constexpr uint8_t OLED_ADDRESS = 0x3C;
