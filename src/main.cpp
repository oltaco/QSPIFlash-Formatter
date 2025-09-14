#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "CustomLFS_QSPIFlash.h"

bool formatted = false;

void setup() {
  Serial.begin(115200);

  while(!Serial) delay(10);

  #ifdef LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, !LED_STATE_ON); // Turn off LED
  #endif

  if (!(QSPIFlash.begin(PIN_QSPI_SCK, PIN_QSPI_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3))) {
    Serial.println("ERROR: QSPI initialisation failed!");
    while (1);
    } else {
      Serial.println("QSPI flash initialised successfully!");
  }
}

void formatQSPIFlash() {
    #ifdef LED_BUILTIN
    digitalWrite(LED_BUILTIN, LED_STATE_ON); // Turn on LED
    #endif
  if (QSPIFlash.lowLevelFormat()) {
    Serial.println("Flash formatted successfully!");
    #ifdef LED_BUILTIN
    digitalWrite(LED_BUILTIN, !LED_STATE_ON); // Turn off LED
    #endif
    formatted = true;
  } else {
    Serial.println("ERROR: Flash format failed!");
    while(1);
  }
}

void loop() {
  if(!formatted) {
    formatQSPIFlash();
  }
}
