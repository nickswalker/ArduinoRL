#ifndef VECTOR_H
#define VECTOR_H

#include <Arduino.h>

void logVector(const int8_t vector[], const size_t length);

void logVector(const uint8_t vector[], const size_t length);

void logVector(const float vector[], const size_t length);

float absMaxComponent(const float vector[], const size_t length);

void norm(float vector[], const size_t length);

void copy(const float first[], float second[], const size_t length);

float dot(const uint8_t first[], const uint8_t second[], const size_t length);

float dot(const float first[], const float second[], const size_t length);

void add(float first[], const float second[], const size_t length);

void add(const float first[], const float second[], float result[], const size_t length);

void subtract(const uint8_t first[], const uint8_t second[], float result[], const size_t length);

/// Subtract the second from the first, storing the result in the first
void subtract(float first[], const float second[], const size_t length);

void multiply(const float scalar, float vector[], const size_t length);

void multiply(const float scalar, const float vector[], float result[], const size_t length);

#endif
