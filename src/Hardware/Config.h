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

constexpr uint8_t FLOAT_SWITCH_PIN = 6;

// Light relay output, pull HIGH to trigger
constexpr uint8_t LIGHTS_PIN = 9;

// Fan relay output, pull HIGH to trigger
constexpr uint8_t FANS_PIN = 10;

// Water relay output, pull HIGH to trigger
constexpr uint8_t WATER_PIN = 11;

// -------------------------
// OLED / Rotary encoder
// -------------------------

constexpr uint8_t OLED_CONFIRM_PIN = 15;
constexpr uint8_t OLED_PUSH_PIN = 16;
constexpr uint8_t OLED_ROTARY_CW_PIN = 17;
constexpr uint8_t OLED_ROTARY_CCW_PIN = 18;
constexpr uint8_t OLED_BACK_PIN = 8;

// -------------------------
// Debounce timing
// -------------------------
constexpr unsigned long DEBOUNCE_MS = 10;

// -------------------------
// I2C addresses
// -------------------------

constexpr uint8_t SHT31_ADDRESS = 0x44;
constexpr uint8_t OLED_ADDRESS = 0x3C;

// -------------------------
// WiFi credentials
// -------------------------
#define WIFI_SSID "Verizon_6DG6DW"
#define WIFI_PASSWORD "retail6bug5air"