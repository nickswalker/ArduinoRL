#include "Learning.h"
#include <Arduino.h>
#include "Task.h"
#include <math.h>
#include "Vector.h"
#include "Output.h"
#include "math.h"
#include "Polynomial.h"
#include "Sense.h"
#include "Debug.h"


#define NUM_PERTURBATIONS 2 * NUM_POLICY_FEATURES

extern ArmState startState;
extern ArmState currentState;
extern ArmState targetState;
// Policy weights are stored per each joint, as a vector of weights. 

#define P_A(parameters, index) parameters[NUM_JOINTS * index + 0u]
#define P_B(parameters, index) parameters[NUM_JOINTS * index + 1u]
#define P_C(parameters, index) parameters[NUM_JOINTS * index + 2u]
#define P_MV(parameters, index) parameters[NUM_JOINTS * index + 3u]

float theta[NUM_POLICY_FEATURES] = {.5};
float actingTheta[NUM_POLICY_FEATURES] = {0};
float perturbations[NUM_PERTURBATIONS][NUM_POLICY_FEATURES] = {0};

extern uint8_t jointRangeMin[];
extern uint8_t jointRnageMax[];

float alpha = 0.001;
float rl_gamma = 0.9999999;
float I;

void logPolicyParameters() {
  logVector(theta, NUM_POLICY_FEATURES);
    
}

float evaluatePolicy() {
  ArmAction deltaToGoal;
  actionBetweenStates(currentState, startState, deltaToGoal);
  D_LOG_V("act", deltaToGoal.jointDeltas, 6);
  apply(deltaToGoal);
  delay(1000);
  actionBetweenStates(currentState, targetState, deltaToGoal);
  D_LOG_V("cur", currentState.jointAngles, 6);
  D_LOG_V("act", deltaToGoal.jointDeltas, 6);
  float equations[NUM_JOINTS][4];
   uint32_t maxIterations = 0;
   for (uint16_t j = 0; j < NUM_JOINTS; j++) {
      float a = exp(P_A(actingTheta, j));
      float b = exp(P_B(actingTheta, j));
      float c = exp(P_C(actingTheta, j));
     

      const float target = deltaToGoal.jointDeltas[j];
      const float sum = a + b + c;
      const float alpha = (a / sum) * target;
      const float beta = (b / sum) * target;
      const float gamma = (c / sum) * target;

      
      float maximizingT;
      const float maximumVelocity = maximizeQuadratic(3.0 * alpha, 2.0 * beta, gamma, maximizingT);
      D_LOG("max", maximumVelocity);
      const float percentMax =  maximumVelocity / 10.0;

      equations[j][0] = alpha;
      equations[j][1] = beta;
      equations[j][2] = gamma;


      const float velocityFactor = exp(P_MV(actingTheta, j)) + 1;

      D_LOG("vf", velocityFactor);
      D_LOG("percent max", percentMax);
      const float iterations = 100.0 * (1.0 / percentMax) * velocityFactor;
      equations[j][3] = iterations;

         D_LOG_V("eq", equations[j], 4);
      maxIterations = max(maxIterations, (uint32_t)iterations);
      
   }
   maxIterations = min(maxIterations, 254);
    D_LOG("--------", maxIterations);
   resetPowerMeasurement();
   
   ArmAction moveTo;
    for (uint8_t i = 0; i < maxIterations; i++) {
      for (uint8_t j = 0; j < NUM_JOINTS; j++) {
        if (i < equations[j][i]) {
          float* e = equations[j];
          const float delta = cubic(e[0],e[1],e[2], (float)i /e[3]);
          moveTo.jointDeltas[j] = delta;
        } else {
          moveTo.jointDeltas[j] = 0.0;
        }
      apply(moveTo);
      delay(50);
    }
  }
  return getPowerUsage();
}

void iterate() {
    float evaluations[NUM_PERTURBATIONS] = {0};
    generatePerturbations();
    for (uint8_t i = 0; i < NUM_PERTURBATIONS; i++) {
      add(perturbations[i], theta, actingTheta, NUM_POLICY_FEATURES);
      evaluatePolicy();
    }
    float numUp[NUM_POLICY_FEATURES] = {0};
    float numDown[NUM_POLICY_FEATURES] = {0};
    float numNone[NUM_POLICY_FEATURES] = {0};
    float averageUp[NUM_POLICY_FEATURES] = {0};
    float averageDown[NUM_POLICY_FEATURES] = {0};
    float averageNone[NUM_POLICY_FEATURES] = {0};
    for (uint8_t i = 0; i < NUM_PERTURBATIONS; i++) {
      for (uint8_t j = 0; j < NUM_POLICY_FEATURES; j++) {
        const float direction = perturbations[i][j];
        if (direction > 0.0) {
          numUp[j] += 1;
          averageUp[j] += evaluations[i];
        } else if (direction < 0.0) {
            numDown[j] += 1;
            averageDown[j] += evaluations[i];
        } else {
            numNone[j] += 1;
            averageNone[j] += evaluations[i];
        }
      }
    }

    for (uint8_t i = 0; i < NUM_PERTURBATIONS; i++) {
      for (uint8_t j = 0; j < NUM_POLICY_FEATURES; j++) {
        averageUp[j] /= numUp[j];
        averageDown[j] /= numDown[j];
        averageNone[j] /= numNone[j];
      }
    }

    float delta[NUM_POLICY_FEATURES] = {0};
    for (uint8_t j = 0; j < NUM_POLICY_FEATURES; j++) {
      if ( averageNone[j] > averageUp[j] && averageNone[j] > averageDown[j]) {
        delta[j] = 0;
      } else {
        delta[j] = averageUp[j] - averageDown[j];
      }
    }
    norm(delta, NUM_POLICY_FEATURES);
    multiply(0.10, delta, NUM_POLICY_FEATURES);
    add(theta, delta, NUM_POLICY_FEATURES);
    copy(theta, actingTheta, NUM_POLICY_FEATURES);

}

void generatePerturbations() {
  for (uint8_t i = 0; i < NUM_PERTURBATIONS; i++) {
    for (uint8_t j = 0; j < NUM_POLICY_FEATURES; j++) {
      perturbations[i][j] = (float)((int)random(3) - 1) * PERTURBATION_STEP;
    }
  }
}
