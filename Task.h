#ifndef TASK_H
#define TASK_H

bool stateIsTerminal(const ArmState &state);

int reward(const ArmState &state, const ArmAction action, const ArmState &statePrime);
#endif
