#include "Output.h"

bool ledState = false;

void chirp() {
    digitalWrite(buzzerPin, HIGH);
    delay(5);
    digitalWrite(buzzerPin, LOW);
}

void chirp(int duration) {
    digitalWrite(buzzerPin, HIGH);
    delay(duration);
    digitalWrite(buzzerPin, LOW);  
}

void chirpN(int n, int duration) {
  for (int i = 0; i < n; i++) {
    chirp(duration);
    delay(20); 
  }
}

void toggleLed() {
    ledState = !ledState;
    digitalWrite(onBoardLedPin, ledState);
}
