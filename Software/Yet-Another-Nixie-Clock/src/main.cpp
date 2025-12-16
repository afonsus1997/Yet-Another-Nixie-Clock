#include "main.h"
// put function declarations here:

TaskHandle_t nixieUpdateTaskHandle;

void nixieUpdateTask(void *pvParameters) {
  while (true) {
    nixieLoop();
  }
}

void writeClockToNixie() {
  static unsigned long lastPrint = 0;

  if (millis() - lastPrint < 250) return;
  lastPrint = millis();

  if (!clockIsValid()) {
    Serial.println("[CLOCK] --:--:--");
    return;
  }

  struct tm tm;
  if (!clockGetLocal(tm)) {
    Serial.println("[CLOCK] --:--:--");
    return;
  }

  char buf[32];
  strftime(buf, sizeof(buf), "%H:%M:%S", &tm);
  Serial.println(buf);

  // Call setTime() with the CURRENT time
  setTime(tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupLeds();
  btn_init();
  setupNixie();
  xTaskCreatePinnedToCore(
      nixieUpdateTask,        // Task function
      "NixieUpdateTask",      // Name
      4096,            // Stack size
      NULL,            // Parameter
      1,               // Priority
      &nixieUpdateTaskHandle, // Handle
      1               // Core 1
  );

  setupWifi();
  
  initRTC();
  initSystemTime();
  clockBootSync();

  clockPrint();
}

void loop() {
  clockLoop();
  writeClockToNixie();

}
