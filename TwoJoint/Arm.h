#ifndef ARM_H
#define ARM_H

void resetArm();
void resetArmToRandomPosition();
void logArmState();
/// Adjusts the arm's state.
void apply(ArmAction action);
#endif
