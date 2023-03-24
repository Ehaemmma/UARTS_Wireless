#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;
const long DIFF = 30000;
long reading = 0;
int status = 0;
int detect = 0;

HX711 scale;

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {
  if (scale.is_ready()) {
    long new_reading = scale.read();
    if ((reading - new_reading > DIFF) && status == 1 && detect >= 3) {
      Serial.println("CUP LIFTED");
      status = 0;
      detect = 0;
    } else if ((reading - new_reading < -DIFF) && status == 0 && detect >= 3) {
      Serial.println("CUP SET");
      status = 1;
      detect = 0;
    } else {
      detect++;
    }
    reading = new_reading;
  } else {
    Serial.println("HX711 not found.");
  }
  delay(200);
}
