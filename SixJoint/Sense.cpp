#include "Pins.h"
#include "Strings.h"
#include <Arduino.h>


#define WINDOW 10
#define SAMPLE_PERIOD_MICROSECONDS 10000
volatile uint8_t measurement_index;
volatile uint32_t measurement_buffer;
volatile float power_use;
float last_bench = 0;
#define AS712_30A_COEFFICIENT_12_BIT 0.0146484375f


extern const char spaceString[];

IntervalTimer measurementTimer;
void sampleCurrent() {
  measurement_buffer += analogRead(currentSensorPin);
  measurement_index += 1;
  if (measurement_index == WINDOW - 1) {
    const float average = (float)measurement_buffer / ((float)WINDOW);
    const float v = (average/ 65536.0) * 3.30;
    power_use += v; 
    measurement_buffer = 0;
    measurement_index = 0;
  }
}

void setupPowerMeasurement() {
  measurementTimer.begin(sampleCurrent, SAMPLE_PERIOD_MICROSECONDS);
}


float getPowerUsage() {
  cli();
  float use = power_use;
  sei();
  return use;
}

void resetPowerMeasurement() {
  cli();
  measurement_index = 0;
  measurement_buffer = 0;
  power_use = 0;
  sei();
  last_bench = 0;
}

float getPowerDelta() {
  float reading = getPowerUsage();
  float diff = reading - last_bench;
  last_bench = reading;
  return diff;
}

