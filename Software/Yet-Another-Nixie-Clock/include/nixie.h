#ifndef __NIXIE__H__
#define __NIXIE__H__

#include <Arduino.h>

#define G_A0 4
#define G_A1 16
#define G_A2 27
#define G_A3 25
#define G_A4 32
#define G_A5 17

#define N0 18
#define N1 19
#define N2 26
#define N3 33

#define N_LE 35

void setupNixie();
void enableDigit(uint8_t digit);
void setDecoderOutput(uint8_t value);
void updateTime();
void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
void disableDigits();
void disableDigit(uint8_t digit);

#endif  //!__NIXIE__H__