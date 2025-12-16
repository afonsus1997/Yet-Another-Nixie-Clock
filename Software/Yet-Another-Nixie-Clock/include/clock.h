#ifndef __CLOCK__H__
#define __CLOCK__H__

#include <Arduino.h>
#include <time.h>

// -----------------------------------------------------
// I2C (RTC)
// -----------------------------------------------------

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// -----------------------------------------------------
// CLOCK SOURCE
// -----------------------------------------------------

enum ClockSource {
  CLOCK_NONE,
  CLOCK_RTC,
  CLOCK_NTP
};

void initRTC();
void initSystemTime();
void clockBootSync();
void clockLoop();
bool clockIsValid();
ClockSource clockGetSource();

bool clockGetLocal(struct tm &timeinfo);
time_t clockGetUTC();

String clockGetTimeString();   // HH:MM:SS
String clockGetDateString();   // YYYY-MM-DD

void clockPrint();

#endif  // __CLOCK__H__
