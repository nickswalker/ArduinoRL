#include "Vector.h"

void logVector(const float vector[], size_t length) {
    Serial.print("[");
    for( uint8_t i = 0; i < length; i++) {
        Serial.print(" ");
        Serial.print(vector[i]);
    }
    Serial.println("]");  
}

float dot(const uint8_t first[], const uint8_t second[], size_t length) {
    float result = 0.0;
    for( uint8_t i = 0; i < length; i++) {
        result += float(first[i]) * float(second[i]);
    }
    return result;
}

float dot(const float first[], const float second[], size_t length) {
    float result = 0.0;
    for( uint8_t i = 0; i < length; i++) {
        result += first[i] * second[i];
    }
    return result;
}

/// Add first and second, store result in first
float add(float first[], const float second[], size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        first[i] = first[i] + second[i];
    }
}


float subtract(const uint8_t first[], const uint8_t second[], float result[], size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        result[i] = float(first[i]) - float(second[i]);
    }
}

/// Subtract the second from the first, storing the result in the first
float subtract(float first[], const float second[], size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        first[i] = float(first[i]) - float(second[i]);
    }
}

float multiply(const float scalar, float vector[], size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        vector[i] = scalar * float(vector[i]);
    }
}

float multiply(const float scalar, const float vector[], float result[], size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        result[i] = scalar * float(vector[i]);
    }
}
