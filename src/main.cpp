#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "CustomLFS.h"
#include "CustomLFS_QSPIFlash.h"
#include "main.h"

bool formatted = false;

void setup() {
  Serial.begin(115200);

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

void formatAll() {
  formatUserData();
  formatExtraFS(); 
  formatQSPIFlash();
}

void formatUserData() {
   CustomLFS UserData = CustomLFS(0xED000, 0x7000);
   if (UserData.format()) {
    Serial.println("UserData formatted successfully!");
   } else {
    Serial.println("ERROR: UserData format failed!");
    while(1);
   }
}

void formatExtraFS() {
   CustomLFS ExtraFS = CustomLFS(0xD4000, 19000);
   if (ExtraFS.format()) {
    Serial.println("ExtraFS formatted successfully!");
   } else {
    Serial.println("ERROR: ExtraFS format failed!");
    while(1);
   }
}

void formatQSPIFlash() {
  if (QSPIFlash.format()) {
    Serial.println("QSPIFlash formatted successfully!");
  } else {
    Serial.println("ERROR: QSPIFlash format failed!");
    while(1);
  }
}

void loop() {
  if(!formatted) {
    #ifdef LED_BUILTIN
    digitalWrite(LED_BUILTIN, LED_STATE_ON); // Turn on LED
    #endif
    formatAll();
    #ifdef LED_BUILTIN
    digitalWrite(LED_BUILTIN, !LED_STATE_ON); // Turn off LED
    #endif
    formatted = true;
  }
  if(formatted) {
    Serial.println("All formatted, rebooting into serial DFU mode...");
    delay(500);
    enterSerialDfu();
  }
}
