#pragma once

float celsiusToFahrenheit(float celsius, float calibration = 0.0f)
{
    return (celsius * 9.0 / 5.0) + 32.0 + calibration;
}