// #include <Arduino.h>
// #include "soc/rtc.h"
// #include "HX711.h"
// #include <WiFi.h>

// // HX711 circuit wiring
// const int LOADCELL_DOUT_PIN = 16;
// const int LOADCELL_SCK_PIN = 4;
// const long DIFF = 30000;
// long reading = 0;
// int status = 0;
// int detect = 0;
// const char *networkName = "BestHotPotAA";
// const char *networkPswd = "BestDanBaoFanAA";

// HX711 scale;

// void setup() {
//   Serial.begin(9600);
//   Serial.println("Booting");

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(networkName, networkPswd);
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
//   scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
// }

// void loop() {
//   if (scale.is_ready()) {
//     long new_reading = scale.read();
//     if ((reading - new_reading > DIFF) && status == 1 && detect >= 3) {
//       Serial.println("CUP LIFTED");
//       status = 0;
//       detect = 0;
//     } else if ((reading - new_reading < -DIFF) && status == 0 && detect >= 3) {
//       Serial.println("CUP SET");
//       status = 1;
//       detect = 0;
//     } else {
//       detect++;
//     }
//     reading = new_reading;
//   } else {
//     Serial.println("HX711 not found.");
//   }
//   delay(200);
// }

#include <Arduino.h>
#include "soc/rtc.h"
#include "HX711.h"
#include <WiFi.h>
#include <HTTPClient.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;
const long DIFF = 30000;
long reading = 0;
int status = 0;
int detect = 0;
const char* ssid     = "uarts_AP";
const char* password = "worldwidetea";
String serverPath = "http://192.168.4.2:5000/";
      
HX711 scale;

void setup() {
  Serial.begin(9600);
    Serial.println("\n[*] Creating AP");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    Serial.print("[+] AP Created with IP Gateway ");
    Serial.println(WiFi.softAPIP());
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {
  if (scale.is_ready()) {
    long new_reading = scale.read();
    // Serial.println(new_reading);
    if ((reading - new_reading > DIFF) && status == 1 && detect >= 3) {
      Serial.println("CUP LIFTED");
      status = 0;
      detect = 0;
        HTTPClient http;
        String request = serverPath + "lift";
        http.begin(request.c_str());
        int httpResponseCode = http.GET();
        
        if (httpResponseCode>0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payload = http.getString();
          Serial.println(payload);
        }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        http.end();
    } else if ((reading - new_reading < -DIFF) && status == 0 && detect >= 3) {
      Serial.println("CUP SET");
      status = 1;
      detect = 0;
        HTTPClient http;
        String request = serverPath + "set";
        http.begin(request.c_str());
        int httpResponseCode = http.GET();
        
        if (httpResponseCode>0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payload = http.getString();
          Serial.println(payload);
        }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        http.end();
    } else {
      detect++;
    }
    reading = new_reading;
  } else {
    Serial.println("HX711 not found.");
  }
  delay(200);
}