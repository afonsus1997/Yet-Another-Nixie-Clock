#include <Arduino.h>
#include <Wire.h>
#include <time.h>
#include <sys/time.h>
#include <RTClib.h>

// -----------------------------------------------------
// CONFIG
// -----------------------------------------------------

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

#define NTP_SERVER "pool.ntp.org"
#define NTP_TIMEOUT_MS 5000
#define NTP_RESYNC_INTERVAL (6 * 3600)  // seconds

// Reject RTC times before ~2023
#define MIN_VALID_EPOCH 1700000000

// Lisbon timezone (WET/WEST)
#define CLOCK_TIMEZONE "WET0WEST,M3.5.0/1,M10.5.0"

// -----------------------------------------------------
// RTC
// -----------------------------------------------------

RTC_DS3231 rtc;

// -----------------------------------------------------
// CLOCK STATE
// -----------------------------------------------------

enum ClockSource {
  CLOCK_NONE,
  CLOCK_RTC,
  CLOCK_NTP
};

struct ClockStatus {
  ClockSource source;
  bool valid;
  time_t lastSync;
};

static ClockStatus clockStatus = {
  .source = CLOCK_NONE,
  .valid = false,
  .lastSync = 0
};

// -----------------------------------------------------
// INTERNAL HELPERS
// -----------------------------------------------------

void setTimezone(const char *tz) {
  Serial.printf("[CLOCK] Setting TZ: %s\n", tz);
  setenv("TZ", tz, 1);
  tzset();
}

bool rtcTimeLooksValid(time_t t) {
  return (t > MIN_VALID_EPOCH);
}

// -----------------------------------------------------
// RTC INIT
// -----------------------------------------------------

void initRTC() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  if (!rtc.begin()) {
    Serial.println("[CLOCK] RTC not found");
  } else {
    Serial.println("[CLOCK] RTC found");
  }
}

// -----------------------------------------------------
// SYSTEM TIME INIT (NTP CONFIG ONLY)
// -----------------------------------------------------

void initSystemTime() {
  Serial.println("[CLOCK] Initializing system time");
  configTime(0, 0, NTP_SERVER);  // UTC only
  setTimezone(CLOCK_TIMEZONE);
}

// -----------------------------------------------------
// NTP → RTC (store UTC)
// -----------------------------------------------------

bool syncNTPToRTC() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo, NTP_TIMEOUT_MS)) {
    Serial.println("[CLOCK] NTP failed");
    return false;
  }

  time_t utc = mktime(&timeinfo); // converts local -> UTC epoch

  rtc.adjust(DateTime(utc));

  Serial.println("[CLOCK] RTC updated from NTP");
  return true;
}

// -----------------------------------------------------
// RTC → SYSTEM
// -----------------------------------------------------

bool syncRTCToSystemClock() {
  if (!rtc.begin()) {
    Serial.println("[CLOCK] RTC missing");
    return false;
  }

  DateTime rtcTime = rtc.now();
  time_t t = rtcTime.unixtime();

  if (!rtcTimeLooksValid(t)) {
    Serial.println("[CLOCK] RTC time invalid");
    return false;
  }

  struct timeval now = { .tv_sec = t };
  settimeofday(&now, nullptr);

  Serial.println("[CLOCK] System clock updated from RTC");
  return true;
}

// -----------------------------------------------------
// BOOT SYNC (AUTHORITATIVE)
// -----------------------------------------------------

void clockBootSync() {
  Serial.println("[CLOCK] Boot sync");

  struct tm timeinfo;

  // 1️⃣ Try NTP
  if (getLocalTime(&timeinfo, NTP_TIMEOUT_MS)) {
    Serial.println("[CLOCK] NTP OK");

    syncNTPToRTC();

    clockStatus.source = CLOCK_NTP;
    clockStatus.valid = true;
    clockStatus.lastSync = time(nullptr);
    return;
  }

  // 2️⃣ Fallback to RTC
  Serial.println("[CLOCK] NTP failed, trying RTC");

  if (syncRTCToSystemClock()) {
    Serial.println("[CLOCK] RTC OK");

    clockStatus.source = CLOCK_RTC;
    clockStatus.valid = true;
    clockStatus.lastSync = time(nullptr);
    return;
  }

  // 3️⃣ No time
  Serial.println("[CLOCK] No valid time source");

  clockStatus.source = CLOCK_NONE;
  clockStatus.valid = false;
}

// -----------------------------------------------------
// CLOCK LOOP (NON-BLOCKING)
// -----------------------------------------------------

void clockLoop() {
  static unsigned long lastTick = 0;

  if (millis() - lastTick < 1000) return;
  lastTick = millis();

  if (!clockStatus.valid) return;

  time_t now = time(nullptr);

  // Periodic NTP resync if we started from RTC
  if (clockStatus.source == CLOCK_RTC &&
      (now - clockStatus.lastSync) > NTP_RESYNC_INTERVAL) {

    Serial.println("[CLOCK] Periodic NTP resync");

    if (syncNTPToRTC()) {
      clockStatus.source = CLOCK_NTP;
      clockStatus.lastSync = now;
    }
  }
}

// -----------------------------------------------------
// PUBLIC CLOCK API
// -----------------------------------------------------

bool clockIsValid() {
  return clockStatus.valid;
}

ClockSource clockGetSource() {
  return clockStatus.source;
}

time_t clockGetUTC() {
  if (!clockStatus.valid) return 0;
  return time(nullptr);
}

bool clockGetLocal(struct tm &timeinfo) {
  if (!clockStatus.valid) return false;
  return getLocalTime(&timeinfo);
}

String clockGetTimeString() {
  struct tm tm;
  if (!clockGetLocal(tm)) return "--:--:--";

  char buf[16];
  strftime(buf, sizeof(buf), "%H:%M:%S", &tm);
  return String(buf);
}

String clockGetDateString() {
  struct tm tm;
  if (!clockGetLocal(tm)) return "----/--/--";

  char buf[16];
  strftime(buf, sizeof(buf), "%Y-%m-%d", &tm);
  return String(buf);
}

// -----------------------------------------------------
// DEBUG
// -----------------------------------------------------

void clockPrint() {
  struct tm tm;
  if (!clockGetLocal(tm)) {
    Serial.println("[CLOCK] Invalid");
    return;
  }

  Serial.println(&tm, "%A, %B %d %Y %H:%M:%S %Z %z");
}
