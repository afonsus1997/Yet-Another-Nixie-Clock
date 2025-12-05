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

//   WiFiProvisioner::Config customCfg(
//     "MyCustomAP",                    // Access Point Name
//     "Welcome!",                      // HTML Page Title
//     "#007BFF",                       // Theme Color
//     "<svg>...</svg>",                // SVG Logo
//     "Custom Project",                // Project Title
//     "Setup Your Device",             // Project Sub-title
//     "Follow these steps:",           // Project Info
//     "All rights reserved © MyProject", // Footer Text
//     "Connected!",                    // Success Message
//     "Reset all?",                    // Reset Confirmation Text
//     "Enter Key:",                    // Input Field Label
//     6,                               // Input Field Length
//     true,                            // Show Input Field
//     true                             // Show Reset Field
// );


  // Create the WiFiProvisioner instance with the custom configuration
  WiFiProvisioner provisioner(customCfg);

  // Set up callbacks
  



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

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startTime > 10000) {
      Serial.println("Failed to connect to saved Wi-Fi.");
      return false;
    }
    delay(500);
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

  

  // Serial.println("Connecting to Wifi");
  

  // WiFi.begin("SpinWorks Guest", "Spin!Works");
  

  // unsigned long start = millis();
  // while (WiFi.status() != WL_CONNECTED) {
    // if (millis() - start > 10000) {
      // Serial.println("❌ WiFi connection timeout.");
      // break;
    // }
    // Serial.print(".");
    // delay(250);  // MUST yield for WiFi driver
  // }
// 
  // Serial.println("Connected!");

}