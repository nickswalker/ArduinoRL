#include "Learning.h"
#include <Arduino.h>
#include "VFA.h"
#include "Task.h"
#include <math.h>
#include "Vector.h"
#include "Output.h"

extern float lastReward;
extern ArmAction nextAction;
extern float theta[];

// A percentage (between 0 - 100)
uint8_t epsilon = 10;
float alpha = 0.3;
float gamma = 0.99;


void logWeights() {
    logVector(theta, NUM_FEATURES);
}

/// Episodic Semi-gradient Sarsa for Control. 
/// See Reinforcement Learning: An Introduction 2nd ed. (Sutton and Barto) page 230.
void learnerUpdate(const ArmState &state, const ArmAction action, const ArmState &statePrime){
    ArmAction actionPrime = chooseAction(statePrime);
    float r = reward(state, action, statePrime);
    float phi[NUM_FEATURES];
    float phiPrime[NUM_FEATURES];
    extractFeatures(state, action, phi);
    extractFeatures(statePrime, actionPrime, phiPrime);

    float error = r + gamma * dot(theta, phiPrime, NUM_FEATURES) - dot(theta, phi, NUM_FEATURES);

    // Overwrite phi with the final weight error
    multiply(alpha * error, phi, NUM_FEATURES);

    add(theta, phi, NUM_FEATURES);
        
    nextAction = actionPrime;
    lastReward = r;
}


inline ArmAction chooseRandomAction() {
    return (ArmAction)random(0, NUM_ACTIONS + 1);
}


ArmAction chooseAction(const ArmState &state) {
    if (random(101) < epsilon) {
        toggleLed();
        return chooseRandomAction();
    } 

    float maxValue = -100000.0;
    ArmAction bestAction;
    for (uint8_t i = 0; i < NUM_ACTIONS; i++) {
        float candidateValue = value(state, (ArmAction)i);
        if (candidateValue > maxValue) {
            maxValue = candidateValue;
            bestAction = (ArmAction)i;
        }
    }
    return bestAction;
}

