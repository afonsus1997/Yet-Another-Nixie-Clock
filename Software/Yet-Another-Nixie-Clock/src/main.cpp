#include "main.h"
// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  setupLeds();
  setupNixie();
  setDecoderOutput(0);
  enableDigit(0);
}

void loop() {
  for (int i = 0; i < 6; i++)
  {
    enableDigit(i);
    delay(3);
  }
}
