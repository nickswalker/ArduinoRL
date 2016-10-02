#include "VFA.h"
#include "Learning.h"
#include "Vector.h"

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

