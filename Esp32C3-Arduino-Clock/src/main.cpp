#include "LuatOS_C3.h"
#include <Arduino.h>

TFT_eSPI tft = TFT_eSPI();
char buf[32] = {0};
unsigned long lastMs = 0;
long check1s = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  initTFT();
  initLEDs();
  tft.println("Start Config WiFi!");
  autoConfigWifi();
  tft.println("Wifi Connected!");
  sprintf(buf, "IP: %s", WiFi.localIP().toString().c_str());
  tft.println(buf);
  configTime(TZ_SEC, DST_SEC, "ntp.ntsc.ac.cn", "ntp1.aliyun.com");
  delay(2000);
  setupOTAConfig();
  tft.fillScreen(TFT_BLACK);
}

inline void showCurrentTime() {
  struct tm info;
  getLocalTime(&info);
  strftime(buf, 32, "%T", &info);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN);
  tft.drawCentreString(buf, 80, 10, 4);
  strftime(buf, 32, "%F", &info);
  tft.setTextColor(TFT_PINK);
  digitalWrite(PIN_LED1, HIGH);
  tft.drawCentreString(buf, 80, 50, 2);
  digitalWrite(PIN_LED1, LOW);
}

void loop() {
  auto ms = millis();
  if (ms - check1s > 1000) {
    check1s = ms;
    showCurrentTime();
    ArduinoOTA.handle();
  }
}