#ifndef VECTOR_H
#define VECTOR_H

#include <Arduino.h>

void logVector(const float vector[], size_t length);

float dot(const uint8_t first[], const uint8_t second[], size_t length);

float dot(const float first[], const float second[], size_t length);

void add(float first[], const float second[], size_t length);

void subtract(const uint8_t first[], const uint8_t second[], float result[], size_t length);

/// Subtract the second from the first, storing the result in the first
void subtract(float first[], const float second[], size_t length);

void multiply(const float scalar, float vector[], size_t length);

void multiply(const float scalar, const float vector[], float result[], size_t length);

#endif
