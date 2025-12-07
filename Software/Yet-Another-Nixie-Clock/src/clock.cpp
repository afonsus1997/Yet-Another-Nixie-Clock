#include "clock.h"

// https://randomnerdtutorials.com/esp32-ntp-timezones-daylight-saving/

RTC_DS3231 rtc;

// -----------------------------------------------------
// TIMEZONE & DST
// -----------------------------------------------------

void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(), 1);
  tzset();  // Apply timezone & DST rules
}

void initTime(String timezone){
  struct tm timeinfo;

  Serial.println("Setting up time");
  configTime(0, 0, "pool.ntp.org");  // Get UTC from NTP

  if (!getLocalTime(&timeinfo)) {
    Serial.println("  Failed to obtain time from NTP");
    return;
  }

  Serial.println("  Got the time from NTP");

  // Now adjust timezone
  setTimezone(timezone);
}

void printLocalTime(){
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z");
}

// -----------------------------------------------------
// MANUAL SET TIME (optional)
// -----------------------------------------------------

void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst){
  struct tm tm;

  tm.tm_year = yr - 1900;
  tm.tm_mon  = month - 1;
  tm.tm_mday = mday;
  tm.tm_hour = hr;
  tm.tm_min  = minute;
  tm.tm_sec  = sec;
  tm.tm_isdst = isDst;

  time_t t = mktime(&tm);

  Serial.printf("Setting time: %s", asctime(&tm));

  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}

// -----------------------------------------------------
// RTC INIT
// -----------------------------------------------------

void initRTC(){
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
  } else {
    Serial.println("RTC found");
  }
}

// -----------------------------------------------------
// NTP → RTC (stores UTC only)
// -----------------------------------------------------

void syncNTPToRTC() {
  struct tm timeinfo;

  // 1. Get the current time (Timezone and DST applied by system)
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain local time for RTC sync");
    return;
  }

  // 2. Convert Local tm structure to UTC epoch timestamp
  // mktime() automatically subtracts the timezone offset to give you UTC.
  time_t now_utc = mktime(&timeinfo);

  // 3. Update the RTC with the UTC timestamp
  rtc.adjust(DateTime(now_utc));

  Serial.println("RTC updated from NTP (stored as UTC)");
}

// -----------------------------------------------------
// RTC → System Clock (apply timezone + DST automatically)
// -----------------------------------------------------

bool syncRTCToSystemClock() {
  if (!rtc.begin()) {
    Serial.println("RTC not found");
    return false;
  }

  DateTime rtcTime = rtc.now();
  time_t t = rtcTime.unixtime();

  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);

  Serial.println("System clock updated from RTC");
  return true;
}

// -----------------------------------------------------
// FULL TIME SYNC ROUTINE (try NTP first, fallback to RTC)
// -----------------------------------------------------

void syncTime() {
  Serial.println("Attempting NTP sync...");

  struct tm timeinfo;
  if (getLocalTime(&timeinfo, 5000)) {
    Serial.println("NTP time obtained");
    syncNTPToRTC();
    printLocalTime();
    return;
  }

  Serial.println("NTP failed, trying RTC...");

  if (syncRTCToSystemClock()) {
    Serial.println("Time loaded from RTC");
    printLocalTime();
  } else {
    Serial.println("RTC also failed. Time is not set.");
  }
}

// -----------------------------------------------------
// CLOCK SYSTEM INIT
// -----------------------------------------------------

void initClock(){
  initRTC();

  // Lisbon timezone (WET/WEST)
  initTime("WET0WEST,M3.5.0/1,M10.5.0");

  printLocalTime();
}