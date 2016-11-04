#include "Pins.h"
#include "Strings.h"
#include <Arduino.h>

extern const char spaceString[];

uint16_t lastCurrentReading = 0;


void updateCurrentSensation()  {
    lastCurrentReading = analogRead(currentSensorPin);
}


void logSensations() {
    Serial.print(lastCurrentReading);

    
}

