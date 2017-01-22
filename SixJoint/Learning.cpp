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
#include "Output.h"

#define NUM_PERTURBATIONS NUM_POLICY_FEATURES
#define BASE_SPEED 5.0f

extern ArmState startState;
extern ArmState currentState;
extern ArmState targetState;
// Policy weights are stored per each joint, as a vector of weights. 

#define P_A(parameters, index) parameters[4 * index + 0u]
#define P_B(parameters, index) parameters[4 * index + 1u]
#define P_C(parameters, index) parameters[4 * index + 2u]
#define P_MV(parameters, index) parameters[4 * index + 3u]

float theta[NUM_POLICY_FEATURES] = {0};
float bestWeights[NUM_POLICY_FEATURES] = {0.05, -0.02, 0.95, 4.21, 0.06, 0.04, 1.13, 4.13, 0.15, 0.15, 1.18, 4.05, -0.06, 0.10, 1.27, 4.03, -0.15, 0.18, 1.23, 4.05, 0.03, -0.05, 1.01, 3.98};
float actingTheta[NUM_POLICY_FEATURES] = {0};
float perturbations[NUM_PERTURBATIONS][NUM_POLICY_FEATURES] = {0};

extern uint8_t jointRangeMin[];
extern uint8_t jointRnageMax[];

float alpha = DEFAULT_ALPHA;



void logPolicyParameters() {
  logVector(theta, NUM_POLICY_FEATURES);
    
}

float evaluatePolicy() {
  chirpN(2, 20);
  moveSmoothlyTo(startState);
  chirpN(1, 20);
  ArmAction deltaToGoal;
  actionBetweenStates(currentState, targetState, deltaToGoal);

  float equations[NUM_JOINTS][4];
   uint32_t maxIterations = 0;
   for (uint16_t j = 0; j < NUM_JOINTS; j++) {

      float a = P_A(actingTheta, j);
      float b = P_B(actingTheta, j);
      float c = P_C(actingTheta, j);

     
      const float target = deltaToGoal.jointDeltas[j];
      float sum = a + b + c;
      if (sum == 0) {
        a = 0;
        b = 0;
        c = 1;
        sum = 1;
      }
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

      // What percentage of the maximum speed should we go?
      const float velocityFactor = 1.0 / (exp(-P_MV(actingTheta, j)) + 1.0);
      // If the policy has less than max speed, iterations should be lower.
      // If the velocity factor is anything less than 1, the number of iterations should be higher.
      const float iterations = BASE_SPEED * percentMax * (1.0 / velocityFactor);
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
        const float t = (float) i/ e[3];
        if ((float)i <= e[3]) {
 
          const float firstSample = currentState.jointAngles[j];
          const float nextSample = cubic(e[0],e[1],e[2], startState.jointAngles[j], t);
          const int8_t delta = max(min(nextSample - firstSample, 126), -126);
          movement.jointDeltas[j] = delta;
        } else {
          movement.jointDeltas[j] = 0.0;
        }
      }
      
      apply(movement);
      
      delay(30);
  }
  //D_LOG_V("END",currentState.jointAngles, 6);
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
    // Make gamma 1
    if ((j + 2) % 4 == 0) {
      theta[j] = 1.0;
    } else if ((j + 1) % 4 == 0) {
      // Make delta 4, effectively maxing out the velocity factor.
      theta[j] = 4.0;
    } else {
      theta[j] = 0.0;
    }
  }
}

void initializeBestWeights() {
  for (uint8_t j = 0; j < NUM_POLICY_FEATURES; j++) {
    theta[j] = bestWeights[j];
  }
}
