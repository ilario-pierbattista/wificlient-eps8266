#include "SoftwareSerial.h"

#define BPS_DEBUG 4800

SoftwareSerial portOne(6, 7);

void setup () {
 Serial.begin(BPS_DEBUG);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
    // Start each software serial port
  portOne.begin(BPS_DEBUG);
  }

void loop ()
{
  portOne.listen();
  while (portOne.available() > 0) {
    char inByte = portOne.read();
    Serial.write(inByte);
  }
}
