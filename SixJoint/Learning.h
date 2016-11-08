#ifndef LEARNING_H
#define LEARNING_H
#include <stdint.h>
#include "Arm.h"
#include "VFA.h"

#define NUM_POLICY_FEATURES NUM_STATE_FEATURES * 2

#define DEFAULT_BETA 0.1
#define DEFAULT_ALPHA 0.2

typedef struct ArmState {
    uint8_t jointAngles[NUM_JOINTS];
} ArmState;

typedef struct ArmAction {
  int8_t jointDeltas[NUM_JOINTS];
} ArmAction;

void logPolicyParameters();
void logWeights();
void calculateGradient(const float a, const float theta[], const float state_features[], float result[]);
void calculatePolicyParameters(float result[], const float theta[][NUM_POLICY_FEATURES], const float stateFeatures[]);
void chooseAction(const ArmState &state, ArmAction &action);
void learnerUpdate(const ArmState &state, const ArmAction &action, const ArmState &statePrime);
#endif
