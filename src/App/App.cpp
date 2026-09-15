#include "App.h"

// -------------------------
// WiFi
// -------------------------

const char *ssid = "Verizon_6DG6DW";
const char *password = "retail6bug5air";

// -------------------------
// Begin
// -------------------------

void App::begin()
{
    Serial.begin(9600);

    hardware.begin();

    initWiFi();
    initWebServer();

    hardware.updateSensors(state.sensorReadings);
}

// -------------------------
// Main update loop
// -------------------------

void App::update()
{
    server.handleClient();

    unsigned long now = millis();

    if (now - lastSensorUpdate >= SENSOR_INTERVAL)
    {
        lastSensorUpdate = now;
        hardware.updateSensors(state.sensorReadings);
    }

    if (now - lastDisplayUpdate >= DISPLAY_INTERVAL)
    {
        lastDisplayUpdate = now;
        updateDisplay();
    }
}

// -------------------------
// WiFi
// -------------------------

void App::initWiFi()
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

// -------------------------
// Web server
// -------------------------

void App::initWebServer()
{
    server.on("/", [this]()
              { handleRoot(); });

    server.begin();

    Serial.println("Web server started");
}

// -------------------------
// OLED
// -------------------------

void App::updateDisplay()
{
    if (isnan(state.sensorReadings.temperatureC) || isnan(state.sensorReadings.humidity))
    {
        showSensorError();
        return;
    }

    hardware.getDisplay().clearBuffer();

    hardware.getDisplay().setFont(u8g2_font_ncenB08_tr);
    hardware.getDisplay().drawStr(0, 12, "HERB HUB");

    hardware.getDisplay().setFont(u8g2_font_ncenB14_tr);

    char tempText[20];
    sprintf(tempText, "%.1f F", state.sensorReadings.temperatureF);
    hardware.getDisplay().drawStr(0, 34, tempText);

    char humidityText[20];
    sprintf(humidityText, "%.1f %%", state.sensorReadings.humidity);
    hardware.getDisplay().drawStr(0, 56, humidityText);

    hardware.getDisplay().sendBuffer();
}

// -------------------------
// Sensor error screen
// -------------------------

void App::showSensorError()
{
    hardware.getDisplay().clearBuffer();

    hardware.getDisplay().setFont(u8g2_font_ncenB08_tr);

    hardware.getDisplay().drawStr(0, 20, "SHT31 ERROR");
    hardware.getDisplay().drawStr(0, 40, "Check sensor");

    hardware.getDisplay().sendBuffer();
}

// -------------------------
// Web page
// -------------------------

void App::handleRoot()
{
    String html = "<html>";
    html += "<head>";
    html += "<meta http-equiv='refresh' content='2'>";
    html += "<title>Herb Hub</title>";
    html += "</head>";

    html += "<body>";
    html += "<h1>🌱 Herb Hub</h1>";

    html += "<h2>Temperature</h2>";
    html += "<p>" + String(state.sensorReadings.temperatureF, 1) + " °F</p>";

    html += "<h2>Humidity</h2>";
    html += "<p>" + String(state.sensorReadings.humidity, 1) + " %</p>";

    html += "<h2>Float Switch</h2>";
    html += "<p>";
    html += state.sensorReadings.floatClosed ? "CLOSED" : "OPEN";
    html += "</p>";

    html += "</body>";
    html += "</html>";

    server.send(200, "text/html", html);
}