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
extern uint8_t epsilon;

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
    pinMode(photoCellOnePin, INPUT);
    pinMode(photoCellTwoPin, INPUT);
    pinMode(photoCellThreePin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    
    servos[0].attach(jointZeroPin);
    servos[1].attach(jointOnePin);  // attaches the servo on pin 9 to the servo object
    servos[2].attach(jointTwoPin);
    servos[3].attach(jointThreePin);
    servos[4].attach(jointFourPin);
    servos[5].attach(jointFivePin);

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
        tripleChirp();
        resetArmToRandomPosition();
        cumulativeReward = 0.0;
        currentEpisodeStep = 0;
        currentEpisode = 0;
        alpha = DEFAULT_ALPHA;
        epsilon = DEFAULT_EPSILON;
        markTrialStart();
    }
    // `apply` populates currentState and previousState for us.
    apply(nextAction);
    //logArmState();

    sense();
    //logSensations();
    
    // The SARSA learner will populate nextAction for us
    ArmAction previousAction = nextAction;
    learnerUpdate(previousState, previousAction, currentState);
    //logWeights();


    currentEpisodeStep += 1;
    cumulativeReward += lastReward;
    logStepInformation();

    if (stateIsTerminal(currentState) || (EVALUATION_MODE && currentEpisodeStep > EVALUATION_MAX_STEPS)) {
        chirp();
        resetArmToRandomPosition();
        cumulativeReward = 0.0;
        currentEpisodeStep = 0;
        markEpisodeEnd();
        currentEpisode += 1;
        if (EVALUATION_MODE && currentEpisode > 50) {
            epsilon = 0;
            alpha = 0.0;
        }
    }

} 
