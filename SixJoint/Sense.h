#ifndef SENSE_H
#define SENSE_H

void setupPowerMeasurement();

void sampleCurrent();

float getPowerUsage();

void logSensations();

void resetPowerMeasurement();

float getPowerDelta();

#endif
