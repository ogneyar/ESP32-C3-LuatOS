#ifndef __LUAT_OS_C3_H__
#define __LUAT_OS_C3_H__

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <TFT_eSPI.h>

#define TZ 8            // 中国时区为8
#define DST_MN 0        // 默认为0
#define TZ_MN ((TZ)*60) //时间换算
#define TZ_SEC ((TZ)*3600)
#define DST_SEC ((DST_MN)*60)

#define PIN_LED1 12
#define PIN_LED2 13

extern TFT_eSPI tft;
extern char buf[];

void inline setupOTAConfig() {
  ArduinoOTA.onStart([] {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawCentreString("OTA Update", 80, 0, 2);
    tft.drawRoundRect(30, 20, 100, 20, 3, TFT_ORANGE);
  });
  ArduinoOTA.onProgress([](u32_t pro, u32_t total) {
    tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
    sprintf(buf, "%d / %d", pro, total);
    tft.drawCentreString(buf, 80, 40, 2);
    int pros = pro * 100 / total;
    tft.fillRoundRect(30, 20, pros, 20, 3, TFT_ORANGE);
  });
  ArduinoOTA.onEnd([] {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawCentreString("Update Succeed!!", 80, 0, 2);
    tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
    tft.drawCentreString("Restarting...", 80, 20, 2);
  });
  ArduinoOTA.begin();
}

void inline autoConfigWifi() {
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin();
  for (int i = 0; WiFi.status() != WL_CONNECTED && i < 100; i++) {
    delay(100);
  }
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.beginSmartConfig();
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }
    WiFi.stopSmartConfig();
    WiFi.mode(WIFI_MODE_STA);
  }
}

void inline initTFT() {
  tft.begin();
  tft.setRotation(3);
  tft.setTextFont(2);
  tft.fillScreen(TFT_BLACK);
}

void inline initLEDs() {
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);
}

#endif