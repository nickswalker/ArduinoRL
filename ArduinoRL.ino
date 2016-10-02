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
    pinMode(INPUT, photoCellOnePin);
    pinMode(INPUT, photoCellTwoPin);
    pinMode(INPUT, photoCellThreePin);
    pinMode(OUTPUT, ledPin);
    pinMode(OUTPUT, buzzerPin);
    
    baseJoint.attach(baseJointPin);
    elbowJoint.attach(elbowJointPin); 

    // Uncomment if you intend to use the deubbing print statements
    Serial.begin(9600);
} 


void logEpisodeEnd() {
    Serial.println("e");
    Serial.println(cumulativeReward);
    Serial.println();
}

void buzz() {
    digitalWrite(buzzerPin, HIGH);
    delay(4000);
    digitalWrite(buzzerPin, LOW);
}

void loop() {   
    // `apply` populates currentState and previous state for us.
    apply(nextAction);

    sense();
    //logSensations();
    
    // The SARSA learner will populate nextAction for us
    ArmAction previousAction = nextAction;
    learnerUpdate(previousState, previousAction, currentState);
    logArmState();

    currentEpisodeStep += 1;
    cumulativeReward += lastReward;

    if (stateIsTerminal(currentState)) {
        logEpisodeEnd();
        buzz();
        resetArm();
        cumulativeReward = 0.0;
        currentEpisodeStep = 0;
    }

} 

