#include "wifi_conn.h"

Preferences preferences;

void setupProvisioning(){
  WiFiProvisioner::Config customCfg(
      "Nixie Clock",   // Access Point Name
      "Nixie Clock Configuration", // HTML Page Title
      "#0989d8",                     // Theme Color
      "",
      "Nixie Clock", // Project Title
      "Wifi Setup",       // Project Sub-title
      "Follow the steps to connect. Obtain your API key from "
      "the 'User' page.",                             // Project Information
      "Afonso Muralha", // Footer
                                                      // Text
      "The device is now visible in your online dashboard.", // Success Message
      "This action will erase all stored settings, including "
      "API key.", // Reset Confirmation Text
      "API Key",  // Input Field Text
      4,          // Input Field Length
      true,       // Show Input Field
      true        // Show Reset Field
  );

  // Create the WiFiProvisioner instance with the custom configuration
  WiFiProvisioner provisioner(customCfg);

  // Set up callbacks
  provisioner.onProvision([]() { Serial.println("Provisioning started."); })
      .onInputCheck([](const char *input) -> bool {
      })
      .onSuccess([](const char *ssid, const char *password, const char *input) {
        preferences.begin("wifi-provision", false);
        Serial.printf("Connected to SSID: %s\n", ssid);
        preferences.putString("ssid", String(ssid));
        if (password) {
          preferences.putString("password", String(password));
          Serial.printf("Password: %s\n", password);
        }
        if (input) {
          Serial.printf("Input: %s\n", input);
          preferences.putString("apikey", String(input));
        }
        Serial.println("Provisioning completed successfully!");

        preferences.end();
      })
      .onFactoryReset([]() { 
        Serial.println("Factory reset triggered!"); 
        preferences.clear();
      });

  // Start provisioning
  provisioner.startProvisioning();
}