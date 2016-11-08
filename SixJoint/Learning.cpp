#include "Learning.h"
#include <Arduino.h>
#include "VFA.h"
#include "Task.h"
#include <math.h>
#include "Vector.h"
#include "Output.h"
#include "Gaussian.h"
#include "math.h"


#define NUM_MEAN_FEATURES NUM_STATE_FEATURES
#define NUM_STD_DEV_FEATURES NUM_STATE_FEATURES
#define STD_DEV_FEATURES_OFFSET NUM_STATE_FEATURES

#define THETA_MU(weights) (weights)
#define THETA_SIGMA(weights) (weights + NUM_STATE_FEATURES)
#define MU(parameters, index) parameters[i]
#define SIGMA(parameters, index) parameters[NUM_JOINTS + i] 

#define D_LOG(str, num) Serial.print(str);Serial.print(": ");if(isnan(num)) Serial.println("NaN");else if (isinf(num)) Serial.println("inf");else Serial.println(num);
#define D_LOG_V(str,vec, len) Serial.print(str);Serial.print(": ");logVector(vec, len);

#define EULER 2.718281828459045235360287471352f

extern float lastReward;
extern ArmAction nextAction;
// Policy weights are stored per each joint, as a vector of weights. Top half is for means, bottom for std deviations
float theta[NUM_JOINTS][NUM_POLICY_FEATURES] = {0};
extern float valueWeights[];
extern uint8_t jointRangeMin[];
extern uint8_t jointRnageMax[];

float alpha = 0.001;
float beta = 0.001;
float rl_gamma = 0.9999999;
float I;

void logPolicyParameters() {
    float policyParameters[NUM_JOINTS * 2];
    float stateFeatures[NUM_STATE_FEATURES];
    calculatePolicyParameters(policyParameters, theta, stateFeatures);

    Serial.print("[");
    for (int i = 0; i < NUM_JOINTS; i++) {
      Serial.print("m"); 
      Serial.print(MU(policyParameters, i));
      Serial.print(" s");
      Serial.print(SIGMA(policyParameters, i));
      Serial.print(", ");
    }
    Serial.println("]");
    
}

/// Episodic One-step Actor Critic
void learnerUpdate(const ArmState &state, const ArmAction &action, const ArmState &statePrime){
    ArmAction actionPrime;
    chooseAction(statePrime, actionPrime);
    float r = reward(state, action, statePrime);
    float x[NUM_STATE_FEATURES];
    
    // place phi_t+1 into x
    extractFeatures(statePrime, actionPrime, x);

    float v_t1 = dot(valueWeights, x, NUM_STATE_FEATURES);

    // fill x with phi_t
    extractFeatures(state, action, x);

    float v_t = dot(valueWeights, x, NUM_STATE_FEATURES);
    float error = r + rl_gamma * v_t1 - v_t;
    D_LOG("err", error);
    D_LOG_V("w", valueWeights, NUM_STATE_FEATURES);
    // Overwrite x with the final weight error
    multiply(beta * error, x, NUM_STATE_FEATURES);

    add(valueWeights, x, NUM_STATE_FEATURES);
    // Now we're done with x...

    extractFeatures(state, action, x);
    // Update theta. This is essentially six independent learning updates.
    for (int i = 0; i < NUM_JOINTS; i++) {
        extractFeatures(state, action, x);
        float gradient[NUM_POLICY_FEATURES];
        //D_LOG_V("grad", gradient, NUM_POLICY_FEATURES);
        calculateGradient(action.jointDeltas[i], theta[i], x, gradient);

        // Calculate the actual weight update
        multiply(I * alpha * error, gradient, NUM_STATE_FEATURES);
        add(theta[i], gradient, NUM_POLICY_FEATURES);
        D_LOG_V("theta",theta[i], NUM_POLICY_FEATURES);
    }
    I *= rl_gamma;
    nextAction = actionPrime;
    lastReward = r;
}
/// Gradient of the normal distribution wrt theta
/// Theta is the weight vector for both mu and sigma of a single joint
void calculateGradient(const float a, const float theta[], const float state_features[], float result[]) {
  const float mu = dot(THETA_MU(theta), state_features, NUM_STATE_FEATURES);
  const float sigma = dot(THETA_SIGMA(theta), state_features, NUM_STATE_FEATURES);

  const float numerator = pow(a - mu, 2.0);
  const float denominator = 2.0 * pow(EULER, sigma);
  const float phi_coefficient = numerator/denominator;
  D_LOG("cof", phi_coefficient);


  multiply(-1 * phi_coefficient, state_features, THETA_MU(result), NUM_STATE_FEATURES);
  multiply(phi_coefficient, state_features, THETA_SIGMA(result), NUM_STATE_FEATURES);
  // Subtract phi from the the theta_sigma
  subtract(THETA_SIGMA(result), state_features, NUM_STATE_FEATURES);

  
}


void chooseAction(const ArmState &state, ArmAction &action) {
    D_LOG("choice",0);
    float policyParameters[NUM_JOINTS * 2];
    float stateFeatures[NUM_STATE_FEATURES];
    calculatePolicyParameters(policyParameters, theta, stateFeatures);
    for (uint8_t i = 0; i < NUM_JOINTS; i++) {
      action.jointDeltas[i] = sampleNormalDistribution(MU(policyParameters, i), SIGMA(policyParameters, i));
    }
}

void calculatePolicyParameters(float result[], const float theta[][NUM_POLICY_FEATURES], const float stateFeatures[]) {
  delay(1000);
  D_LOG_V("state features", stateFeatures, NUM_STATE_FEATURES);
  // Calculate means
  for (uint8_t i = 0; i < NUM_JOINTS; i++) {
    MU(result,i) = dot(THETA_MU(theta[i]), stateFeatures, NUM_STATE_FEATURES);
  }
  // Calculate std_devs
  for (uint8_t i = 0; i < NUM_JOINTS; i++) {
    float sigmaDot = dot(THETA_SIGMA(theta[i]), stateFeatures, NUM_STATE_FEATURES);
    D_LOG("sd", sigmaDot);
    SIGMA(result, i) = exp(sigmaDot);
  }
  D_LOG_V("params", result, NUM_JOINTS * 2);
}

