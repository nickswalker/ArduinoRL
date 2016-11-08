#include <Servo.h>
#include "Pins.h"
#include "Learning.h"
#include "Arm.h"
#include "Task.h"
#include "Sense.h"
#include "Strings.h"
#include "Output.h"

#define EVALUATION_MODE 1
#define EVALUATION_SWITCH_POINT 50
#define EVALUATION_MAX_STEPS 200

extern float alpha;
extern float beta;
extern float I;

extern const char spaceString[];
extern const char cumulativeRewardString[];
extern const char stepsString[];

Servo servos[] = {Servo(), Servo(), Servo(), Servo(), Servo(), Servo()};


extern ArmState currentState;
extern ArmState previousState;

ArmAction nextAction = {{0,0,0,0,0,0}};

float lastReward = 0.0;

int32_t cumulativeReward = 0.0;
uint32_t currentEpisodeStep = 0;
uint16_t currentEpisode = 0;

void setup() {
    randomSeed(analogRead(A3));
    pinMode(currentSensorPin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    
    servos[0].attach(jointZeroPin);
    servos[1].attach(jointOnePin);
    servos[2].attach(jointTwoPin);
    servos[3].attach(jointThreePin);
    servos[4].attach(jointFourPin);
    servos[5].attach(jointFivePin);

    // Uncomment if you intend to use the debugging print statements
    Serial.begin(115200);

}

void logStepInformation() {
    Serial.print(currentEpisodeStep);
    Serial.print(",");
    Serial.println(cumulativeReward);

}

void markEpisodeEnd() {
    Serial.println("***");
}

void markTrialStart() {
    Serial.println("!!!");
}

bool checkForResetSignal(){
    bool received = false;
    if (Serial.available() == 2) {
        // read the incoming byte:
        uint8_t incomingByte = Serial.read();
        if(incomingByte == 35) {
            received = true;
        }
    }
    return received;

}

void loop() { 
    if (checkForResetSignal()) {
        chirpN(3,5);
        resetArmToRandomPosition();
        cumulativeReward = 0.0;
        currentEpisodeStep = 0;
        currentEpisode = 0;
        alpha = DEFAULT_ALPHA;
        beta = DEFAULT_BETA;
        I = 1.0;
        markTrialStart();
    }
    // `apply` populates currentState and previousState for us. It will also measure the current
    apply(nextAction);

    logArmState();
    //logSensations();

    // The learner will populate nextAction for us
    ArmAction previousAction = nextAction;
    learnerUpdate(previousState, previousAction, currentState);
    
    //logWeights();
    //logPolicyParameters();

    currentEpisodeStep += 1;
    cumulativeReward += lastReward;
    logStepInformation();

    if (stateIsTerminal(currentState) || (EVALUATION_MODE && currentEpisodeStep > EVALUATION_MAX_STEPS)) {
        chirp();
        resetArmToRandomPosition();
        pickNewRandomTarget();
        cumulativeReward = 0.0;
        currentEpisodeStep = 0;
        markEpisodeEnd();
        currentEpisode += 1;
        if (EVALUATION_MODE && currentEpisode > 50) {
            alpha = 0.0;
            beta = 0.0;
        }
    }

} 
