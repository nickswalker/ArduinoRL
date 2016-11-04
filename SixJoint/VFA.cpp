#include "VFA.h"
#include "Learning.h"
#include "Vector.h"

float valueWeights[NUM_FEATURES] = {0};

void extractFeatures(const ArmState &state, const ArmAction action, float phi[]) {

    uint8_t ledOn = state.ledOn;
    bool turnsLedOn = action & 1;

    int8_t elbowDirection = 0;

    int8_t baseDirection = 0;
    switch (action) {
        case StayLeftOff:
        case StayLeftOn:
        case LeftLeftOff:
        case LeftLeftOn:
        case RightLeftOff:
        case RightLeftOn:
        elbowDirection = -1;
    }

    switch (action) {
        case StayRightOff:
        case StayRightOn:
        case LeftRightOff:
        case LeftRightOn:
        case RightRightOff:
        case RightRightOn:
        elbowDirection = 1;
    }

    switch (action) {
        case RightStayOff:
        case RightStayOn:
        case RightLeftOff:
        case RightLeftOn:
        case RightRightOff:
        case RightRightOn:
        baseDirection = 1;
    }
    switch (action) {
        case LeftStayOff:
        case LeftStayOn:
        case LeftLeftOff:
        case LeftLeftOn:
        case LeftRightOff:
        case LeftRightOn:
        baseDirection = -1;
    }
    //squareBin(float(state.basePosition), float(state.elbowPosition), 20, 8, phi, 64);
    //phi[64] = state.ledOn ? 1.0 : 0.0; 
    //phi[65] = float(elbowDirection);
    //phi[66] = float(baseDirection);

    // size of the vector should be: sideLength * sideLength * 2
    squareBinBinaryAugment(float(state.basePosition), float(state.elbowPosition), state.ledOn, 20, 8, phi, 128);
    phi[128] = float(elbowDirection);
    phi[129] = float(baseDirection);
    phi[130] = float(turnsLedOn);
    phi[131] = float(turnsLedOn && (state.basePosition > 90));
   

}

float value(const ArmState &state, const ArmAction action) {
    float phi[NUM_FEATURES];
    extractFeatures(state, action, phi);
    return dot(theta, phi, NUM_FEATURES);
}

void squareBin(const float xValue, const float yValue, const float binBy, const uint8_t sideLength, float vector[], size_t length) {
    uint8_t xBin = xValue / binBy;
    uint8_t yBin = yValue / binBy;

    uint8_t binIndex = sideLength * yBin + xBin;

    for (uint8_t i = 0; i < length; i++) {
        if (i == binIndex) {
            vector[binIndex] = 1.0;
        } else {
            vector[i] = 0.0;
        }
    }
}

void squareBinBinaryAugment(const float xValue, const float yValue, const bool binaryValue, const float binBy, const uint8_t sideLength, float vector[], size_t length) {
    uint8_t xBin = xValue / binBy;
    uint8_t yBin = yValue / binBy;

    uint8_t binIndex = sideLength * yBin + xBin;

    if (binaryValue) {
        binIndex += sideLength * sideLength;
    }
    for (uint8_t i = 0; i < length; i++) {
        if (i == binIndex) {
            vector[binIndex] = 1.0;
        } else {
            vector[i] = 0.0;
        }
    }    
}

float distanceToGoal(const ArmState &state) {
    return 0.0001 * abs(float(state.basePosition) - 30.0) * abs(float(state.elbowPosition) - 30.0);
}



