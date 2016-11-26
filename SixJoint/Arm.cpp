#include <Arduino.h>
#include <Servo.h>
#include "Learning.h"
#include "Pins.h"
#include "Strings.h"
#include "Sense.h"
#include "Vector.h"
#include "Debug.h"


extern const char spaceString[];

extern Servo servos[];

uint8_t jointRangeMin[] = {10,30,50,50,50,100};
uint8_t jointRangeMax[] = {170,150,100,100,100,130};

ArmState currentState = {{0,0,0,0,0,0}};
ArmState previousState = {{0,0,0,0,0,0}};

void resetArm() {
    ArmState middlePosition;
    for (uint i = 0; i < NUM_JOINTS; i++) {
      middlePosition.jointAngles[i] = ((uint16_t)jointRangeMin[i] + (uint16_t)jointRangeMax[i]) / 2;
    }
    // Could have to move quite a bit to reset.
    delay(700);
    currentState = previousState = middlePosition;
    
}

void resetArmToRandomPosition() {
    ArmAction randomPosition;
    chooseRandomAction(randomPosition);
    apply(randomPosition);
    // Could have to move quite a bit to reset.
    delay(700);
    previousState = currentState;
    
}

void chooseRandomAction(ArmAction &action) {
    for (uint i = 0; i < NUM_JOINTS; i++) {
      action.jointDeltas[i] = random(-MAX_JOINT_MOVE_AMOUNT, MAX_JOINT_MOVE_AMOUNT);
    }
}

void chooseRandomState(ArmState &state) {
  for (uint i = 0; i < NUM_JOINTS; i++) {
      state.jointAngles[i] = random(jointRangeMin[i], jointRangeMax[i]);
    }
}

void actionBetweenStates(const ArmState &from, const ArmState &to, ArmAction &action) {
  for (uint i = 0; i < NUM_JOINTS; i++) {
      action.jointDeltas[i] = to.jointAngles[i] - from.jointAngles[i];
  }
}

void logArmState() {
    logVector(currentState.jointAngles, NUM_JOINTS);
}

/// Adjusts the arm's state.
void apply(ArmAction& action) {

    // Let's keep track of this for the learner.
    previousState = currentState;
    for (uint i = 0; i < NUM_JOINTS; i++) {
        currentState.jointAngles[i] += action.jointDeltas[i];
        // Check to see if the movements caused an underflow
        const uint8_t jointMin = jointRangeMin[i];
        const uint8_t jointMax = jointRangeMax[i];
        if (action.jointDeltas[i] < 0 && currentState.jointAngles[i] > jointMax) {
            currentState.jointAngles[i] = jointMin;
        }
        
        // Limit the range of the joints before we send them the new state
        if (currentState.jointAngles[i] < jointMin) {
            currentState.jointAngles[i] = jointMin;
        }
    
        if (currentState.jointAngles[i] > jointMax) {
            currentState.jointAngles[i] = jointMax;
        }

    }
    // If you get a lot of servo jitter, it's likely because your power supply can't handle the initial current spike.
    // Try adding another delay here avoid moving both motors at once.
    for (uint i = 0; i < NUM_JOINTS; i++) {
      servos[i].write(currentState.jointAngles[i]);
    }
}
