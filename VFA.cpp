#include "VFA.h"
#include "Learning.h"

float theta[NUM_FEATURES] = {};

void extractFeatures(const ArmState &state, const ArmAction action, float phi[]) {
    uint8_t elbowBin = state.elbowPosition / 10;
    uint8_t baseBin = state.basePosition / 10;

    uint8_t ledOn = state.ledOn;

    uint8_t elbowLeft = 0;
    uint8_t elbowRight = 0;

    uint8_t baseLeft = 0;
    uint8_t baseRight = 0;
    switch (action) {
        case StayLeftOff:
        case StayLeftOn:
        case LeftLeftOff:
        case LeftLeftOn:
        case RightLeftOff:
        case RightLeftOn:
        elbowLeft = 1;
    }

    switch (action) {
        case StayRightOff:
        case StayRightOn:
        case LeftRightOff:
        case LeftRightOn:
        case RightRightOff:
        case RightRightOn:
        elbowRight = 1;
    }

    switch (action) {
        case RightStayOff:
        case RightStayOn:
        case RightLeftOff:
        case RightLeftOn:
        case RightRightOff:
        case RightRightOn:
        baseRight = 1;
    }
    switch (action) {
        case LeftStayOff:
        case LeftStayOn:
        case LeftLeftOff:
        case LeftLeftOn:
        case LeftRightOff:
        case LeftRightOn:
        baseLeft = 1;
    }

    phi[0] = float(elbowBin);
    phi[1] = float(baseBin);
    phi[2] = float(ledOn);
    phi[3] = float(elbowRight);
    phi[4] = float(elbowLeft);
    phi[5] = float(baseRight);
    phi[6] = float(baseLeft);
}

float value(const ArmState &state, const ArmAction action) {
    float phi[NUM_FEATURES];
    extractFeatures(state, action, phi);
    return dot(theta, phi, NUM_FEATURES);
}

float dot(const uint8_t first[], const uint8_t second[], size_t length) {
    float result = 0.0;
    for( uint8_t i = 0; i < length; i++) {
        result += float(first[i]) + float(second[i]);
    }
    return result;
}

float dot(const float first[], const float second[], size_t length) {
    float result = 0.0;
    for( uint8_t i = 0; i < length; i++) {
        result += first[i] + second[i];
    }
    return result;
}

/// Add first and second, store result in first
float add(float first[], const float second[], size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        first[i] = first[i] - second[i];
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

float multiply(float scalar, float vector[], size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        vector[i] = scalar * float(vector[i]);
    }
}

float multiply(float scalar, const float vector[], float result[], size_t length) {
    for (uint8_t i = 0; i < length; i++) {
        result[i] = scalar * float(vector[i]);
    }
}

