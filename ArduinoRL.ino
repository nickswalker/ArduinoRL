#include <Servo.h> 
#include "Pins.h"
#include "Learning.h"
#include "Arm.h"
#include "Task.h"
#include "Sense.h"
#include "Strings.h"
#include "Output.h"

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
 
void setup() { 
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

void loop() { 
    // `apply` populates currentState and previousState for us.
    apply(nextAction);
    //logArmState();

    sense();
    logSensations();
    
    // The SARSA learner will populate nextAction for us
    ArmAction previousAction = nextAction;
    learnerUpdate(previousState, previousAction, currentState);
    //logWeights();


    currentEpisodeStep += 1;
    cumulativeReward += lastReward;
    logStepInformation();

    if (stateIsTerminal(currentState)) {
        logStepInformation();
        chirp();
        resetArm();
        cumulativeReward = 0.0;
        currentEpisodeStep = 0;
        markEpisodeEnd();
    }

} 

