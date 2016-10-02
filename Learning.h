#ifndef LEARNING_H
#define LEARNING_H
#include <stdint.h>

typedef struct ArmState {
    uint8_t basePosition;
    uint8_t elbowPosition;
    bool ledOn;
} ArmState;


#define NUM_ACTIONS 18
/// Each joint has three options, and the LED has two options, thus we have
/// 18 actions. Specified BaseElbowLED, so RightLeftOff move the base joint right, the elbow
/// left and turns the LED off.
/// For ordering purposes, Stay < Left < Right and Off < On
typedef enum ArmAction {
    StayStayOff,
    StayStayOn,
    StayLeftOff,
    StayLeftOn,
    StayRightOff,
    StayRightOn,
    LeftStayOff,
    LeftStayOn,
    LeftLeftOff,
    LeftLeftOn,
    LeftRightOff,
    LeftRightOn,
    RightStayOff,
    RightStayOn,
    RightLeftOff,
    RightLeftOn,
    RightRightOff,
    RightRightOn
} ArmAction;

ArmAction chooseRandomAction();
ArmAction chooseAction(const ArmState &state);
void learnerUpdate(const ArmState &state, const ArmAction action, const ArmState &statePrime);
#endif
