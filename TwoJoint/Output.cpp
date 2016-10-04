#include "Output.h"

bool ledState = false;
void chirp() {
    digitalWrite(buzzerPin, HIGH);
    delay(5);
    digitalWrite(buzzerPin, LOW);
}

void toggleLed() {
    ledState = !ledState;
    digitalWrite(onBoardLedPin, ledState);
}
