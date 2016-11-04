#ifndef LEARNING_H
#define LEARNING_H
#include <stdint.h>
#include "Arm.h"

typedef struct ArmState {
    uint8_t jointAngles[NUM_JOINTS];
} ArmState;


/// Each joint has three options, and the LED has two options, thus we have
/// 18 actions. Specified BaseElbowLED, so RightLeftOff move the base joint right, the elbow
/// left and turns the LED off.
/// For ordering purposes, Stay < Left < Right and Off < On
typedef struct ArmAction {
  int8_t jointDeltas[NUM_JOINTS];
} ArmAction;

void logParameters();
void chooseRandomAction(ArmAction &action);
ArmAction chooseAction(const ArmState &state);
void learnerUpdate(const ArmState &state, const ArmAction &action, const ArmState &statePrime);
#endif
