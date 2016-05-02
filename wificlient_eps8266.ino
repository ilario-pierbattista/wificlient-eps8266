#include <Arduino.h>

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.print("Hello \n");
    delay(500);
}