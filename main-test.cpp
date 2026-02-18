#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.drawString("ESP32-2432S032", 40, 100);
  tft.drawString("DISPLAY OK", 70, 130);
}

void loop() {
}
