#ifndef VFA_H
#define VFA_H
#include <Arduino.h>
#include "Learning.h"
#define NUM_FEATURES 7

void extractFeatures(const ArmState &state, const ArmAction action, float phi[]);

float value(const ArmState &state, ArmAction action);
#endif
