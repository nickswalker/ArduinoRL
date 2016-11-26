#include <Arduino.h>
#include "Learning.h"

ArmState startState = {50,50,50,50,50,100};
ArmState targetState = {100,100,100,100,100,130};

void pickNewRandomTarget() {
  chooseRandomState(targetState);
}
