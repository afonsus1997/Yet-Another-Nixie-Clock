#include "main.h"
// put function declarations here:

TaskHandle_t nixieUpdateTaskHandle;

void nixieUpdateTask(void *pvParameters) {
  while (true) {
    nixieLoop();
  }
}

void setup() {
  // put your setup code here, to run once:
  setupLeds();
  setupNixie();
  setTime(12, 23, 34);
  xTaskCreatePinnedToCore(
      nixieUpdateTask,        // Task function
      "NixieUpdateTask",      // Name
      4096,            // Stack size
      NULL,            // Parameter
      1,               // Priority
      &nixieUpdateTaskHandle, // Handle
      1               // Core 1
  );
}

void loop() {

}
