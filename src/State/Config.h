#pragma once

// ------------------------
// Default system settings
// -------------------------
constexpr bool DEFAULT_MODE_AUTO = true;
constexpr bool DEFAULT_USE_FAHRENHEIT = true;
constexpr int DEFAULT_BRIGHTNESS = 50;     // 0 to 100
constexpr int DEFAULT_TIMEOUT_MINUTES = 3; // 0 to 99
constexpr int DEFAULT_TIMEOUT_SECONDS = 0; // 0 to 59

// Default light schedule
constexpr int DEFAULT_LIGHT_START_HOUR = 8;
constexpr int DEFAULT_LIGHT_START_MINUTE = 0;
constexpr int DEFAULT_LIGHT_END_HOUR = 22;
constexpr int DEFAULT_LIGHT_END_MINUTE = 0;
constexpr int DEFAULT_LIGHT_FREQUENCY_DAYS = 1;