#pragma once

// ------------------------
// Default system settings
// -------------------------
constexpr bool DEFAULT_MODE_AUTO = true;
constexpr bool DEFAULT_USE_FAHRENHEIT = true;

// default uninitialized temp and humidity values
constexpr float DEFAULT_TEMP_HUMID = -100.0f;

// Default light schedule
constexpr int DEFAULT_LIGHT_START_HOUR = 8;
constexpr int DEFAULT_LIGHT_START_MINUTE = 0;
constexpr int DEFAULT_LIGHT_END_HOUR = 22;
constexpr int DEFAULT_LIGHT_END_MINUTE = 0;

// Default fan schedule
constexpr int DEFAULT_FANS_DURATION_MINUTES = 10;
constexpr int DEFAULT_FANS_FREQUENCY_HOURS = 1;

// Default water schedule
constexpr int DEFAULT_WATER_DURATION_MINUTES = 5;
constexpr int DEFAULT_WATER_FREQUENCY_HOURS = 12;
