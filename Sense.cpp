#include "Pins.h"
#include <Arduino.h>

uint8_t lastCellOneReading = 200;
uint8_t lastCellTwoReading = 200;
uint8_t lastCellThreeReading = 200;

int sense(){
    lastCellOneReading = analogRead(photoCellOnePin);
    lastCellTwoReading = analogRead(photoCellTwoPin);
    lastCellThreeReading = analogRead(photoCellThreePin);
    
}

void logSensations() {
    Serial.println(lastCellOneReading);
}

