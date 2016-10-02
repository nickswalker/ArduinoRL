#include "Pins.h"
#include "Strings.h"
#include <Arduino.h>

extern const char spaceString[];

uint8_t lastCellOneReading = 0;
uint8_t lastCellTwoReading = 0;
uint8_t lastCellThreeReading = 0;

int sense(){
    delay(50);
    lastCellOneReading = analogRead(photoCellOnePin);
    lastCellTwoReading = analogRead(photoCellTwoPin);
    lastCellThreeReading = analogRead(photoCellThreePin);
    
}

void logSensations() {
    Serial.print(lastCellOneReading);
    Serial.print(spaceString);
    Serial.print(lastCellTwoReading);
    Serial.print(spaceString);
    Serial.println(lastCellThreeReading);
    
}

