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

extern const char spaceString[];
extern const char cumulativeRewardString[];
extern const char stepsString[];

Servo servos[] = {Servo(), Servo(), Servo(), Servo(), Servo(), Servo()};


extern ArmState currentState;
extern ArmState previousState;
extern ArmState startState;

ArmAction nextAction = {{0,0,0,0,0,0}};

float lastReward = 0.0;

uint16_t currentEpisode = 0;

void setup() {
    randomSeed(analogRead(A3));
    analogReadResolution(16);
    analogReadAveraging(16);
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

    setupPowerMeasurement();

    moveSmoothlyTo(startState);
}

void logEpisodeInformation(float totalReturn) {
    Serial.print(currentEpisode);
    Serial.print(",");
    Serial.println(totalReturn);

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
        currentEpisode = 0;
        alpha = DEFAULT_ALPHA;
        markTrialStart();
    }

    if (EVALUATION_MODE && currentEpisode > EVALUATION_SWITCH_POINT) {
        chirp();
        alpha = 0.0;
        float totalReturn = evaluatePolicy();
        logEpisodeInformation(totalReturn);
        markEpisodeEnd();
        currentEpisode += 1;
         
    } else {
      iterate();
      float totalReturn = evaluatePolicy();
      logEpisodeInformation(totalReturn);
      markEpisodeEnd();
      currentEpisode += 1;
    }

    

    
    
} 

