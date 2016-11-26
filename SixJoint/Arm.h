#ifndef ARM_H
#define ARM_H

#define NUM_JOINTS 6u
#define MAX_JOINT_MOVE_AMOUNT 20
struct ArmAction;
struct ArmState;

void resetArm();
void resetArmToRandomPosition();
void logArmState();
void chooseRandomState(ArmState &state);
void chooseRandomAction(ArmAction &action);
void actionBetweenStates(const ArmState &from, const ArmState &to, ArmAction &action);
/// Adjusts the arm's state.
void apply(ArmAction& action);
#endif
