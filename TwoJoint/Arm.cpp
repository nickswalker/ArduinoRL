#include <Arduino.h>
#include <Servo.h>
#include "Learning.h"
#include "Pins.h"
#include "Strings.h"

#define JOINT_MIN 15
#define JOINT_MAX 170
#define JOINT_MOVE_AMOUNT 20

extern const char spaceString[];

extern Servo elbowJoint;
extern Servo baseJoint;

ArmState currentState = {JOINT_MIN, JOINT_MIN, 0};
ArmState previousState = {JOINT_MIN, JOINT_MIN, 0};

void resetArm() {
    elbowJoint.write(JOINT_MIN);
    baseJoint.write(JOINT_MIN);
    digitalWrite(ledPin, LOW);
    // Could have to move quite a bit to reset. Also, this delay lets the photocells get back to regular levels
    delay(700);
    currentState = previousState = {JOINT_MIN, JOINT_MIN, 0};
    
}


void resetArmToRandomPosition() {
    uint8_t elbow = random(JOINT_MIN, JOINT_MAX);
    uint8_t base = random(JOINT_MIN, JOINT_MAX);
    digitalWrite(ledPin, LOW);

    baseJoint.write(base);
    elbowJoint.write(elbow);
    delay(700);
    currentState = previousState = {base, elbow, 0};
}

void logArmState() {
    Serial.print(currentState.basePosition);
    Serial.print(spaceString);
    Serial.print(currentState.elbowPosition);
    Serial.print(spaceString);
    Serial.println(currentState.ledOn); 
}

/// Adjusts the arm's state.
void apply(ArmAction action) {

    // Let's keep track of this for the learner.
    previousState = currentState;
    // We've carefully ensured that the ledOn component is the lowest bit
    currentState.ledOn = (uint8_t)action & 1;

    int8_t baseMovement = 0;
    int8_t elbowMovement = 0;
    switch (action) {
        // No need for StayStay cases. Default values handle that.
        // Just here to silence compile warnings
        case StayStayOn:
        case StayStayOff:
            break;
        case StayLeftOff:
        case StayLeftOn:
            elbowMovement -= JOINT_MOVE_AMOUNT;
        break;
        case StayRightOff:
        case StayRightOn:
            elbowMovement += JOINT_MOVE_AMOUNT;
        break;
        case LeftStayOff:
        case LeftStayOn:
            baseMovement -= JOINT_MOVE_AMOUNT;
        break;
        case LeftLeftOff:
        case LeftLeftOn:
            baseMovement -= JOINT_MOVE_AMOUNT;
            elbowMovement -= JOINT_MOVE_AMOUNT;
        break;
        case LeftRightOff:
        case LeftRightOn:
            baseMovement -= JOINT_MOVE_AMOUNT;
            elbowMovement += JOINT_MOVE_AMOUNT;
        break;
        case RightStayOff:
        case RightStayOn:
            baseMovement += JOINT_MOVE_AMOUNT;
        break;
        case RightLeftOff:
        case RightLeftOn:
            baseMovement += JOINT_MOVE_AMOUNT;
            elbowMovement -= JOINT_MOVE_AMOUNT;
        break;
        case RightRightOff:
        case RightRightOn:
            baseMovement += JOINT_MOVE_AMOUNT;
            elbowMovement -= JOINT_MOVE_AMOUNT;
        break;
        
    }
    currentState.basePosition += baseMovement;
    currentState.elbowPosition += elbowMovement;

    // Check to see if the movements caused an underflow
    if (elbowMovement < 0 && currentState.elbowPosition > JOINT_MAX) {
        currentState.elbowPosition = JOINT_MIN;
    }

    if (baseMovement < 0 && currentState.basePosition > JOINT_MAX) {
        currentState.basePosition = JOINT_MIN;
    }

    // Limit the range of the joints before we send them the new state
    if (currentState.basePosition < JOINT_MIN) {
        currentState.basePosition = JOINT_MIN;
    }

    if (currentState.elbowPosition < JOINT_MIN) {
        currentState.elbowPosition = JOINT_MIN;
    }

    if (currentState.basePosition > JOINT_MAX) {
        currentState.basePosition = JOINT_MAX;
    }

    if (currentState.elbowPosition > JOINT_MAX) {
        currentState.elbowPosition = JOINT_MAX;
    }
    
    baseJoint.write(currentState.basePosition);
    // If you get a lot of servo jitter, it's likely because your power supply can't handle the initial current spike.
    // Try adding another delay here avoid moving both motors at once.
    elbowJoint.write(currentState.elbowPosition);
    digitalWrite(ledPin, currentState.ledOn);
    delay(150);
}
