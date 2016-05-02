#include <Arduino.h>
#include "SoftwareSerial.h"

#define RFID_RESET_PIN 2
#define RFID_RX_PIN 11
#define RFID_TX_PIN 12
#define RFID_BPS 9600

#define SERIAL_BPS 115200

#define DEBUG_RX_PIN 9
#define DEBUG_TX_PIN 10
#define SERIAL_DEBUG 19200

SoftwareSerial rfidSerial(RFID_RX_PIN, RFID_TX_PIN);
SoftwareSerial debugSerial(DEBUG_RX_PIN, DEBUG_TX_PIN);

/**
* Conversione ASCII (esadecimale) - intero
* @param  char v        Carattere
* @return int           Valore intero
*/
int ascii2hex(char v);

void setup() {
    Serial.begin(SERIAL_BPS);
    rfidSerial.begin(RFID_BPS);
    debugSerial.begin(SERIAL_DEBUG);

    debugSerial.print("MANNAGGIAACRISTO\n");

    pinMode(RFID_RESET_PIN, OUTPUT);
    digitalWrite(RFID_RESET_PIN, HIGH);
}

void loop() {
    byte i = 0;
    byte val = 0;
    byte code[6];
    byte checksum = 0;
    byte bytesread = 0;
    byte tempbyte = 0;

    if (rfidSerial.available() > 0) {
        if ((val = rfidSerial.read()) == 2) {                  // check for header
            bytesread = 0;
            while (bytesread < 12) {                        // read 10 digit code + 2 digit checksum
                if (rfidSerial.available() > 0) {
                    val = rfidSerial.read();
                    if ((val == 0x0D) || (val == 0x0A) || (val == 0x03) ||
                        (val == 0x02)) { // if header or stop bytes before the 10 digit reading
                        break;                                    // stop reading
                    }

                    // Do Ascii/Hex conversion:
                    val = ascii2hex(val);

                    // Every two hex-digits, add byte to code:
                    if (bytesread & 1 == 1) {
                        // make some space for this hex-digit by
                        // shifting the previous hex-digit with 4 bits to the left:
                        code[bytesread >> 1] = (val | (tempbyte << 4));

                        if (bytesread >> 1 != 5) {                // If we're at the checksum byte,
                            checksum ^= code[bytesread >> 1];       // Calculate the checksum... (XOR)
                        };
                    } else {
                        tempbyte = val;                           // Store the first hex digit first...
                    };

                    bytesread++;                                // ready to read next digit
                }
            }

            // Output to Serial:

            if (bytesread == 12) {                          // if 12 digit read is complete
                debugSerial.print("5-byte code: ");
                for (i = 0; i < 5; i++) {
                    if (code[i] < 16) debugSerial.print("0");
                    debugSerial.print(code[i], HEX);
                    debugSerial.print(" ");
                }
                debugSerial.println();

                debugSerial.print("Checksum: ");
                debugSerial.print(code[5], HEX);
                debugSerial.println(code[5] == checksum ? " -- passed." : " -- error.");
                debugSerial.println();
            }

            bytesread = 0;
        }
    }
}

int ascii2hex(char val) {
    if ((val >= '0') && (val <= '9')) {
        val = val - '0';
    } else if ((val >= 'A') && (val <= 'F')) {
        val = (char) 10 + val - 'A';
    }
    return val;
}