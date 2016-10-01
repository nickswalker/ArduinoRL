#include <Servo.h> 
#include "Pins.h"
#include "Learning.h"
#include "Arm.h"
 
Servo baseJoint;
Servo elbowJoint;         


 
void setup() { 
    pinMode(INPUT, photoCellOnePin);
    pinMode(INPUT, photoCellTwoPin);
    pinMode(INPUT, photoCellThreePin);
    pinMode(OUTPUT, ledPin);
    
    baseJoint.attach(baseJointPin);
    elbowJoint.attach(elbowJointPin); 

    Serial.begin(9600);
} 

int sense(){
    
}


ArmAction chooseRandomAction() {
    return (ArmAction)random(0, 19);
}

void loop() 
{ 
    //apply(StayRightOn);
    apply(chooseRandomAction());
    logArmState();

} 

