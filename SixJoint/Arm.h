#ifndef ARM_H
#define ARM_H

#define NUM_JOINTS 6
struct ArmAction;

void resetArm();
void resetArmToRandomPosition();
void logArmState();
/// Adjusts the arm's state.
void apply(ArmAction& action);
#endif
