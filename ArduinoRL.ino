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
 
Servo baseJoint;
Servo elbowJoint;         

extern ArmState currentState;
extern ArmState previousState;

ArmAction nextAction = StayStayOff;

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
    
    baseJoint.attach(baseJointPin);
    elbowJoint.attach(elbowJointPin); 

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

