#include <Arduino.h>
#include "Learning.h"

ArmState startState =  { 60, 65, 50, 50, 50,100};
ArmState targetState = {100,80, 85,100,100,130};



void pickNewRandomTarget() {
  chooseRandomState(targetState);
}
