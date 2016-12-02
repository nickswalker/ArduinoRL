#ifndef LEARNING_H
#define LEARNING_H
#include <stdint.h>
#include "Arm.h"

#define NUM_POLICY_FEATURES NUM_JOINTS * 4
#define PERTURBATION_STEP 0.1
#define DEFAULT_ALPHA 0.1

typedef struct ArmState {
    uint8_t jointAngles[NUM_JOINTS];
} ArmState;

typedef struct ArmAction {
  int8_t jointDeltas[NUM_JOINTS];
} ArmAction;

void logPolicyParameters();
void iterate();
float evaluatePolicy();
void generatePerturbations();
void randomlyInitializeWeights();
void initializeLinearWeights();
void initializeBestWeights();
#endif
