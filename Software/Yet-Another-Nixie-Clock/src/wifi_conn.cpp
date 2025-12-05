#include "wifi_conn.h"
#include <string>

Preferences preferences;



void setupProvisioning(){
  WiFiProvisioner::Config customCfg(
      "Nixie Clock",   // Access Point Name
      "Nixie Clock Configuration", // HTML Page Title
      "#0989d8",                     // Theme Color
      "",
      "Nixie Clock", // Project Title
      "Wifi Setup",       // Project Sub-title
      "Follow the steps to connect.",                             // Project Information
      "Afonso Muralha", // Footer
      "The device is now visible in your online dashboard.", // Success Message
      "This action will erase all stored settings, including "
      "API key.", // Reset Confirmation Text
      "API Key",  // Input Field Text
      30,          // Input Field Length
      true,       // Show Input Field
      true        // Show Reset Field
  );



  // Create the WiFiProvisioner instance with the custom configuration
  WiFiProvisioner provisioner(customCfg);

  // Set up callbacks
  provisioner.onSuccess([](const char *ssid, const char *password, const char *input) {
  Serial.printf("Provisioning successful! SSID: %s\n", ssid);
  preferences.begin("wifi-provision", false);
  // Store the credentials and API key in preferences
  preferences.putString("ssid", String(ssid));
  if (password) {
    preferences.putString("password", String(password));
  }
  if (input) {
    preferences.putString("input", String(input));
  }
  preferences.end();
  Serial.println("Credentials and input key saved.");
  });  



  // Start provisioning
  provisioner.startProvisioning();
}

uint8_t connectWifi(){
  preferences.begin("wifi-provision", false);
  String savedSSID = preferences.getString("ssid", "");
  String savedPassword = preferences.getString("password", "");
  preferences.end();

  if (savedSSID.isEmpty()) {
    Serial.println("No saved Wi-Fi credentials found.");
    return 1;
  }

  Serial.printf("Connecting to saved Wi-Fi: %s\n", savedSSID.c_str());
  if (savedPassword.isEmpty()) {
    WiFi.begin(savedSSID.c_str());
  } else {
    WiFi.begin(savedSSID.c_str(), savedPassword.c_str());
  }

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > 10000) {
      Serial.println("❌ WiFi connection timeout.");
      break;
    }
    Serial.print(".");
    delay(250);  // MUST yield for WiFi driver
  }

  Serial.printf("Successfully connected to %s\n", savedSSID.c_str());
  return 0;
  
}

void setupWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);   // Clear old state
  delay(100);
  if(connectWifi()){
    setupProvisioning();
  }
}