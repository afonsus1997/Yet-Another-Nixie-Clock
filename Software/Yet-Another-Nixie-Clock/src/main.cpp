#include "main.h"
// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  setupLeds();
}

void loop() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  delay(500);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  delay(500);
  
}
