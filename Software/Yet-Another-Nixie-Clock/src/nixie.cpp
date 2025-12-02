#include "nixie.h"

// Anode pins for each nixie digit
uint8_t anodes[] = {G_A0, G_A1, G_A2, G_A3, G_A4, G_A5};
const uint8_t kNumAnodes = 6;

// Cathode BCD lines going to the latch → Nixie decoder
uint8_t cathodes[] = {N0, N1, N2, N3};
const uint8_t kNumCathodes = 4;

// Time buffer: HHMMSS
uint8_t currentTime[] = {1, 2, 3, 4, 5, 6};


void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  currentTime[5] = seconds % 10;
  currentTime[4] = seconds / 10;
  currentTime[3] = minutes % 10;
  currentTime[2] = minutes / 10;
  currentTime[1] = hours % 10;
  currentTime[0] = hours / 10;
}


void setupNixie() {

  for (int i = 0; i < kNumAnodes; i++)
    pinMode(anodes[i], OUTPUT);

  for (int i = 0; i < kNumCathodes; i++)
    pinMode(cathodes[i], OUTPUT);

  pinMode(N_LE, OUTPUT);
  digitalWrite(N_LE, HIGH);  // latch disabled (inactive-high latch)

  disableDigits();
}


// ------------------------------------------------------------
// BRIGHT MULTIPLEXING
// ------------------------------------------------------------

void updateTime() {

  // 600–900 µs = perfect brightness, no flicker
  const uint16_t onTime = 1000; // microseconds

  for (int d = 0; d < 6; d++) {

    // Set decoder for this digit value
    setDecoderOutput(currentTime[d]);

    // Light this digit
    enableDigit(d);

    // Allow discharge to fully strike
    delayMicroseconds(onTime);

    // Turn off only this digit
    disableDigit(d);
  }
}


// Writes BCD to latch → to Nixie decoder
void setDecoderOutput(uint8_t value) {

  value &= 0x0F; // ensure 0–9

  digitalWrite(cathodes[0], (value >> 0) & 1);
  digitalWrite(cathodes[1], (value >> 1) & 1);
  digitalWrite(cathodes[2], (value >> 2) & 1);
  digitalWrite(cathodes[3], (value >> 3) & 1);

  // Pulse latch (simple latch)
  digitalWrite(N_LE, LOW);
  digitalWrite(N_LE, HIGH);
}


// ------------------------------------------------------------
// DIGIT SELECT (ANODES)
// ------------------------------------------------------------

void disableDigits() {
  for (int i = 0; i < kNumAnodes; i++)
    digitalWrite(anodes[i], LOW);
}

void disableDigit(uint8_t digit) {
  digitalWrite(anodes[digit], LOW);
}

void enableDigit(uint8_t digit) {
  for (int i = 0; i < kNumAnodes; i++)
    digitalWrite(anodes[i], (i == digit) ? HIGH : LOW);
}