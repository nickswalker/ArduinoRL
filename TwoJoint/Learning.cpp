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
uint8_t epsilon = DEFAULT_EPSILON;
float alpha = DEFAULT_ALPHA;
float gamma = 0.99;


void logWeights() {
    logVector(theta, NUM_FEATURES);
}

/// Episodic Semi-gradient Sarsa for Control. 
/// See Reinforcement Learning: An Introduction 2nd ed. (Sutton and Barto) page 230.
void learnerUpdate(const ArmState &state, const ArmAction action, const ArmState &statePrime){
    ArmAction actionPrime = chooseAction(statePrime);
    float r = reward(state, action, statePrime);
    float x[NUM_FEATURES];
    // place phi_t+1 into x
    extractFeatures(statePrime, actionPrime, x);

    float v_t1 = dot(theta, x, NUM_FEATURES);
    
    // fill x with phi_t
    extractFeatures(state, action, x);

    float error = r + rl_gamma * v_t1 - dot(theta, x, NUM_FEATURES);

    // Overwrite x with the final weight error
    multiply(alpha * error, x, NUM_FEATURES);

    add(theta, x, NUM_FEATURES);
        
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
    ArmAction bestActions[NUM_ACTIONS] = {};
    uint8_t numBestActions = -1;
    for (uint8_t i = 0; i < NUM_ACTIONS; i++) {
        float candidateValue = value(state, (ArmAction)i);
        if (candidateValue > maxValue) {
            maxValue = candidateValue;
            bestActions[0] = (ArmAction)i;
            numBestActions = 1;
        } else if (candidateValue == maxValue) {
            bestActions[numBestActions] = (ArmAction)i;
            numBestActions +=1;
        }
    }

    return bestActions[random(numBestActions)];
}

