// Import Class's
#include <ESP8266WiFi.h>          // Wifi
#include <ESP8266WiFiMulti.h>     // Multi Wifi
#include <ArduinoOTA.h>           // OTA

// Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WiFiMulti wifiMulti;


void setup() {
  // Start the Serial communication to send messages to the computer
  Serial.begin(115200);
  Serial.println('\n');
  Serial.println("Booting ...");
  
  
  // ******************
  // *** Multi WiFi ***
  // ******************
  
  WiFi.mode(WIFI_STA);
  
  // Add Wi-Fi networks you want to connect to
  wifiMulti.addAP("Redmi", "B.Costa_1991");
  wifiMulti.addAP("NOS", "querias_pass");
  wifiMulti.addAP("NUMERICABLE-9438", "0A18E839BA");
  
  Serial.println("");
  Serial.print("Connecting Wifi ");
  
  // Delay to reconnect to WiFi
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  
  // Print what network are connected
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  
  // Print the IP of the interface
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  
  // *****************
  // *** Change IP ***
  // *****************
  
  // Check actual gateway + netmask + ip
  IPAddress gateway = WiFi.gatewayIP();
  IPAddress subnet = WiFi.subnetMask();
  IPAddress myIP = WiFi.localIP();
  
  // New IP Assignment
  IPAddress ip(myIP[0], myIP[1], myIP[2], 200);
  
  // Restart WiFi to assigment new IP
  WiFi.config(ip, gateway, subnet);
  WiFi.begin();
  Serial.println("");
  Serial.print("Reconnecting to Wifi ");

  // Delay to reconnect to WiFi
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  
  // Print the NEW IP of the interface
  Serial.println("");
  Serial.print("IP change to: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  
  
  // ***********
  // *** OTA ***
  // ***********

  // Set Hostname and password to OTA
  ArduinoOTA.setHostname("ESP8266");
  ArduinoOTA.setPassword("password123");
  
  ArduinoOTA.onStart([]() {
    String type;
    
    if (ArduinoOTA.getCommand() == U_FLASH) type = "sketch";
    else type = "filesystem";
    
    Serial.println("Start updating " + type);
  });
  
  // Print end of OTA
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  
  // Print OTA progress
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  // Print OTA error's
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
   });
  
  ArduinoOTA.begin();
}


void loop() {
  ArduinoOTA.handle();

  // Print the received signal strength:
  Serial.print("Signal: ");

  if (WiFi.RSSI() <= -100) Serial.println("0%");
  else if (WiFi.RSSI() >= -20) Serial.println("100%");
  else {
    int quality = 1.25 * (WiFi.RSSI() + 100) + 0.5; // +0.5 to round number to int
    Serial.print(quality);
    Serial.println("%");
  }

  //Update the quality of WiFi every 5s
  delay(5000);
}
