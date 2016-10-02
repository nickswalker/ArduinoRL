#ifndef VFA_H
#define VFA_H
#include <Arduino.h>
#include "Learning.h"
#define NUM_FEATURES 7

void extractFeatures(const ArmState &state, const ArmAction action, float phi[]);

float value(const ArmState &state, ArmAction action);

float dot(const uint8_t first[], const uint8_t second[], size_t length);

float dot(const float first[], const float second[], size_t length);

float add(float first[], const float second[], size_t length);

float subtract(const uint8_t first[], const uint8_t second[], float result[], size_t length);

/// Subtract the second from the first, storing the result in the first
float subtract(float first[], const float second[], size_t length);

float multiply(float scalar, float vector[], size_t length);

float multiply(float scalar, const float vector[], float result[], size_t length);
#endif
