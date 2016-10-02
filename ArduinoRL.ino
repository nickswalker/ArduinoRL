#include <Servo.h> 
#include "Pins.h"
#include "Learning.h"
#include "Arm.h"
#include "Task.h"
#include "Sense.h"
 
Servo baseJoint;
Servo elbowJoint;         

extern ArmState currentState;
extern ArmState previousState;

ArmAction nextAction = StayStayOff;

float lastReward = 0.0;

uint32_t cumulativeReward = 0.0;

float currentEpisodeStep = 0.0;
 
void setup() { 
    pinMode(photoCellOnePin, INPUT);
    pinMode(photoCellTwoPin, INPUT);
    pinMode(photoCellThreePin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    
    baseJoint.attach(baseJointPin);
    elbowJoint.attach(elbowJointPin); 

    // Uncomment if you intend to use the debugging print statements
    Serial.begin(57600);
} 


void logEpisodeEnd() {
    Serial.print("cr");
    Serial.print(cumulativeReward);
    Serial.print("st");
    Serial.println(currentEpisodeStep);
    Serial.println();
}

void chirp() {
    digitalWrite(buzzerPin, HIGH);
    delay(10);
    digitalWrite(buzzerPin, LOW);
}

void loop() { 
    // `apply` populates currentState and previousState for us.
    apply(nextAction);
    //logArmState();

    sense();
    //logSensations();
    
    // The SARSA learner will populate nextAction for us
    ArmAction previousAction = nextAction;
    learnerUpdate(previousState, previousAction, currentState);
    logWeights();


    currentEpisodeStep += 1;
    cumulativeReward += lastReward;

    if (stateIsTerminal(currentState)) {
        logEpisodeEnd();
        chirp();
        resetArm();
        cumulativeReward = 0.0;
        currentEpisodeStep = 0;
    }

} 

