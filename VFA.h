#ifndef VFA_H
#define VFA_H
#include <Arduino.h>
#include "Learning.h"
#define NUM_FEATURES 68
void extractFeatures(const ArmState &state, const ArmAction action, float phi[]);

float value(const ArmState &state, ArmAction action);

void squareBin(const float xValue, const float yValue, const float binBy, const uint8_t sideLength, float vector[], size_t length);

void squareBinBinaryAugment(const float xValue, const float yValue, const bool binaryValue, const float binBy, const uint8_t sideLength, float vector[], size_t length);

float distanceToGoal(const ArmState &state);
#endif
