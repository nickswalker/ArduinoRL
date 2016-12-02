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
#include "Arm.h"
#include "Gaussian.h"


#define NUM_PERTURBATIONS NUM_POLICY_FEATURES

extern ArmState startState;
extern ArmState currentState;
extern ArmState targetState;
// Policy weights are stored per each joint, as a vector of weights. 

#define P_A(parameters, index) parameters[4 * index + 0u]
#define P_B(parameters, index) parameters[4 * index + 1u]
#define P_C(parameters, index) parameters[4 * index + 2u]
#define P_MV(parameters, index) parameters[4 * index + 3u]

float theta[NUM_POLICY_FEATURES] = {0};
float actingTheta[NUM_POLICY_FEATURES] = {0};
float perturbations[NUM_PERTURBATIONS][NUM_POLICY_FEATURES] = {0};

extern uint8_t jointRangeMin[];
extern uint8_t jointRnageMax[];

float alpha = DEFAULT_ALPHA;
float rl_gamma = 0.9999999;



void logPolicyParameters() {
  logVector(theta, NUM_POLICY_FEATURES);
    
}

float evaluatePolicy() {
  moveSmoothlyTo(startState);
  ArmAction deltaToGoal;
  actionBetweenStates(currentState, targetState, deltaToGoal);

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
      const float curveMaxVelocity = maximizeQuadratic(3.0 * alpha, 2.0 * beta, gamma, maximizingT);
      const float percentMax =  fabs(curveMaxVelocity) / 10.0;

      //D_LOG("CVMT", maximizingT);
      //D_LOG("CVM", curveMaxVelocity);    

      equations[j][0] = alpha;
      equations[j][1] = beta;
      equations[j][2] = gamma;

      const float velocityFactor = exp(P_MV(actingTheta, j)) + 1;
      const float iterations = 5.0 * percentMax * velocityFactor;
      equations[j][3] = ceil(iterations);
      maxIterations = max(maxIterations, (uint32_t)(iterations));

      //D_LOG_V("EQ", equations[j], 4);
      
   }
   maxIterations = min(maxIterations, 2000u);
   //D_LOG("iterations", maxIterations);
   resetPowerMeasurement();
   
   ArmAction movement;
    for (uint32_t i = 0; i <= maxIterations; i++) {
      for (uint8_t j = 0; j < NUM_JOINTS; j++) {
        float* e = equations[j];
        if ((float)i <= e[3]) {
 
          const float firstSample = currentState.jointAngles[j];
          const float nextSample = cubic(e[0],e[1],e[2], startState.jointAngles[j], (float) i/ e[3]);
          const int8_t delta = max(min(nextSample - firstSample, 126), -126);
          movement.jointDeltas[j] = delta;
        } else {
          movement.jointDeltas[j] = 0.0;
        }
      }
      
      apply(movement);
      
      delay(30);
  }
  return -1.0 * getPowerUsage();
}

void iterate() {
    float evaluations[NUM_PERTURBATIONS] = {0};
    generatePerturbations();
    for (uint8_t i = 0; i < NUM_PERTURBATIONS; i++) {
      add(perturbations[i], theta, actingTheta, NUM_POLICY_FEATURES);
      const float evaluation = evaluatePolicy();
      evaluations[i] = evaluation;
    }
    D_LOG_V("evals", evaluations, NUM_PERTURBATIONS);
    uint8_t numUp[NUM_POLICY_FEATURES] = {0};
    uint8_t numDown[NUM_POLICY_FEATURES] = {0};
    uint8_t numNone[NUM_POLICY_FEATURES] = {0};
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

    for (uint8_t j = 0; j < NUM_POLICY_FEATURES; j++) {
      if (numUp[j] > 0) {
        averageUp[j] /= (float)numUp[j];
      }
      if (numDown[j] > 0) {
        averageDown[j] /= (float)numDown[j];
      }
      if (numNone[j] > 0) {
        averageNone[j] /= (float)numNone[j];
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
    multiply(alpha, delta, NUM_POLICY_FEATURES);
    D_LOG_V("step", delta, NUM_POLICY_FEATURES);

    add(theta, delta, NUM_POLICY_FEATURES);
    copy(theta, actingTheta, NUM_POLICY_FEATURES);
    D_LOG_V("acting theta", actingTheta, NUM_POLICY_FEATURES);

}

void generatePerturbations() {
  for (uint8_t i = 0; i < NUM_PERTURBATIONS; i++) {
    for (uint8_t j = 0; j < NUM_POLICY_FEATURES; j++) {
      perturbations[i][j] = (float)((int)random(3) - 1) * PERTURBATION_STEP;
    }
  }
}

void randomlyInitializeWeights() {
  for (uint8_t j = 0; j < NUM_POLICY_FEATURES; j++) {
      theta[j] = randomf(-0.5, 0.5);
    }
}

void initializeLinearWeights() {
  for (uint8_t j = 0; j < NUM_POLICY_FEATURES; j++) {
    if (j % 3 == 0) {
      theta[j] = 1.0;
    } else {
      theta[j] = 0.0;
    }
  }
}

void initializeBestWeights() {
}
