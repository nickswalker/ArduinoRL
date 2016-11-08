#include "VFA.h"
#include "Learning.h"
#include "Vector.h"

extern ArmState targetState;

float valueWeights[NUM_STATE_FEATURES] = {0};

void extractFeatures(const ArmState &state, const ArmAction action, float phi[]) {
   for(uint8_t i = 0; i < NUM_JOINTS; i++) {
      phi[i] = (targetState.jointAngles[i] - state.jointAngles[i]) / 10.0;
   }
   for(uint8_t i = 0; i < NUM_JOINTS; i++) {
      phi[NUM_JOINTS + i] = state.jointAngles[i] / 10.0;
   }
}

float value(const ArmState &state, const ArmAction action) {
    float phi[NUM_STATE_FEATURES];
    extractFeatures(state, action, phi);
    return dot(valueWeights, phi, NUM_STATE_FEATURES);
}

void squareBin(const float xValue, const float yValue, const float binBy, const uint8_t sideLength, float vector[], size_t length) {
    uint8_t xBin = xValue / binBy;
    uint8_t yBin = yValue / binBy;

    uint8_t binIndex = sideLength * yBin + xBin;

    for (uint8_t i = 0; i < length; i++) {
        if (i == binIndex) {
            vector[binIndex] = 1.0;
        } else {
            vector[i] = 0.0;
        }
    }
}



