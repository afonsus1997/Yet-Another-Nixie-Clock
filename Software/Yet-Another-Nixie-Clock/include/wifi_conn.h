#ifndef __WIFI__H__
#define __WIFI__H__

#include <WiFiProvisioner.h>
#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>


void setupProvisioning();
uint8_t connectWifi();
void setupWifi();

#endif  //!__WIFI__H__