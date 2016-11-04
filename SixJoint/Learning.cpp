#include "Learning.h"
#include <Arduino.h>
#include "VFA.h"
#include "Task.h"
#include <math.h>
#include "Vector.h"
#include "Output.h"

extern float lastReward;
extern ArmAction nextAction;
float theta[NUM_FEATURES * 2];
extern float valueWeights[];
extern uint8_t jointRangeMin[];
extern uint8_t jointRnageMax[];

float alpha = 0.2;
float beta = 0.2;
float rl_gamma = 0.99;


void logParameters() {
    logVector(theta, NUM_JOINTS * 2);
}

/// Episodic One-step Actor Critic
void learnerUpdate(const ArmState &state, const ArmAction &action, const ArmState &statePrime){
    ArmAction actionPrime;
    chooseAction(statePrime, actionPrime);
    float r = reward(state, action, statePrime);
    float x[NUM_FEATURES];
    // place phi_t+1 into x
    extractFeatures(statePrime, actionPrime, x);

    float v_t1 = dot(theta, x, NUM_FEATURES);


    // Update theta
    // fill x with phi_t
    extractFeatures(state, action, x);

    float error = r + rl_gamma * v_t1 - dot(theta, x, NUM_FEATURES);

    // Overwrite x with the final weight error
    multiply(beta * error, x, NUM_FEATURES);

    add(theta, x, NUM_FEATURES);
    // Now we're done with x...
  
    // Update w (state-value function approximator weights)
    // We're recalculating this to conserve memory
    extractFeatures(state, action, x);

    // Overwrite x with the final weight error
    multiply(alpha * error, x, NUM_FEATURES);

    add(theta, x, NUM_FEATURES);
        
    nextAction = actionPrime;
    lastReward = r;
}


void chooseRandomAction(ArmAction &action) {
    for (int i = 0; i < NUM_JOINTS; i++) {
      action.jointAngles[i] = random(jointRangeMin[i], jointRangeMax[i]);
    }
}


void chooseAction(const ArmState &state, ArmAction &action) {
    for (uint8_t i = 0; i < NUM_JOINTS; i++) {
      action.jointDeltas[i] = sampleNormalDistribution(mean(i), stdDev(i));
    }
}

void calculatePolicyVariables(float result[], const float theta[], const float stateFeatures[]) {
  for (uint8_t i = 0; i < NUM_JOINTS; i++) {
    result[i] = dot(theta, stateFeatures, NUM_WEIGHTS);
  }
  for (uint8_t i = NUM_JOINTS; i < NUM_JOINTS * 2; i++) {
    result[i] = exp(dot(theta, stateFeatures, NUM_WEIGHTS));
  }
}

float stdDev(uint8_t jointNum){
  return theta[(jointNum * 2) + 1 ];
}

float mean(uint8_t jointNum) {
  return theta[jointNum * 2];
}

