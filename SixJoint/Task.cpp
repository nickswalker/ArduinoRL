#include <Arduino.h>
#include "Learning.h"

extern ArmState currentState;
ArmState targetState;

void pickNewRandomTarget() {
  chooseRandomState(targetState);
}

bool stateIsTerminal(const ArmState &state) {
    for (int i = 0; i < NUM_JOINTS; i++) {
      if (currentState.jointAngles[i] != targetState.jointAngles[i]) {
        return false;
      }
    }
    return true;
}

int8_t reward(const ArmState &state, const ArmAction action, const ArmState &statePrime) {
    if (stateIsTerminal(statePrime)) {
        return 50;
    }
    return -1;
}

