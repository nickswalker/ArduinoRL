#include "Output.h"

bool ledState = false;
void chirp() {
    digitalWrite(buzzerPin, HIGH);
    delay(10);
    digitalWrite(buzzerPin, LOW);
}

void toggleLed() {
    ledState = !ledState;
    digitalWrite(onBoardLedPin, ledState);
}
