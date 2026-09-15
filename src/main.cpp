#include <Arduino.h>
#include "App/App.h"

App app;

void setup()
{
  app.begin();
}

void loop()
{
  app.update();
}