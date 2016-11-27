#include <Arduino.h>
#include "Learning.h"

ArmState startState = {40,  60, 50,50,50,100};
ArmState targetState = {170,100,100,100,100,110};

void pickNewRandomTarget() {
  chooseRandomState(targetState);
}
