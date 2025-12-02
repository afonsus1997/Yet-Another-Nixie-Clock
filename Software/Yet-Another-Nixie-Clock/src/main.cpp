#include "main.h"
// put function declarations here:

TaskHandle_t nixieUpdateTaskHandle;

void nixieUpdateTask(void *pvParameters) {
  // while (true) {
  //   for (int i = 0; i < 6; i++) {
  //     enableDigit(i);
  //     vTaskDelay(pdMS_TO_TICKS(2));
  //   }
  // }
}

void setup() {
  // put your setup code here, to run once:
  setupLeds();
  setupNixie();
  // setDecoderOutput(2);
  setTime(12, 34, 56);
  // enableDigit(0);
  // xTaskCreatePinnedToCore(
  //     nixieUpdateTask,        // Task function
  //     "NixieUpdateTask",      // Name
  //     4096,            // Stack size
  //     NULL,            // Parameter
  //     1,               // Priority
  //     &nixieUpdateTaskHandle, // Handle
  //     1               // Core 1
  // );
}

void loop() {
  // for (int i = 0; i < 6; i++) {
      // enableDigit(i);
      // delay(1);
    // }
  updateTime();
}
