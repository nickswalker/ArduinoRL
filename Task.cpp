#include <Arduino.h>
#include "Learning.h"

extern uint8_t lastCellOneReading;

bool stateIsTerminal(const ArmState &state) {
    if (lastCellOneReading > 190 && state.ledOn) {
        return true;   
    }
    return false;
}

int8_t reward(const ArmState &state, const ArmAction action, const ArmState &statePrime) {
    if (stateIsTerminal(statePrime)) {
        return 50;
    }
    return -1;
}

