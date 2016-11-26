#include "Pins.h"
#include "Strings.h"
#include <Arduino.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#define WINDOW 10
#define SAMPLE_PERIOD_MICROSECONDS 100000
volatile uint8_t measurement_index;
volatile uint32_t measurement_buffer;
volatile float power_use;
float last_bench = 0;



extern const char spaceString[];

uint16_t lastCurrentReading = 0;

IntervalTimer measurementTimer;
void sampleCurrent() {
  measurement_buffer += analogRead(currentSensorPin);
  measurement_index += 1;
  if (measurement_index == WINDOW - 1) {
    power_use += (float)measurement_buffer / (1024.0 * WINDOW); 
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


void logSensations() {
    Serial.println(lastCurrentReading);    
}


void resetPowerMeasurement() {
  cli();
  measurement_index = 0;
  measurement_buffer = 0;
  power_use = 0;
  lastCurrentReading = 0;
  sei();
  last_bench = 0;
}

float getPowerDelta() {
  float reading = getPowerUsage();
  float diff = reading - last_bench;
  last_bench = reading;
  return diff;
}

