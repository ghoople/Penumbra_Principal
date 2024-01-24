// Interrupt functions that are called when a sensor at either hardstop is tripped. 

#include "main.h"

void BottomHardStop() {
    // Called via interrupt when the hard stop pin is tripped. 
    motor.MoveStopAbrupt();

    Serial.println("Bottom Hard Stop Triggered");
    
    // Move away from the hard stop
    motor.VelMax(2000); // Move at 2000 pulse/sec away from hard stop

    motor.Move(Home_Offset);
        Serial.println("Moving away from the hard stop... Waiting for the step output to finish...");
    while (!motor.StepsComplete()) {
        continue;
    }

    // Zero the motor's reference position after homing to allow for accurate absolute position moves

    motor.PositionRefSet(0);      
}

void TopHardStop() {
    // Called via interrupt when the hard stop pin is tripped. 
    motor.MoveStopAbrupt();

    Serial.println("Top Hard Stop Triggered");
    
    // Move away from the hard stop

    motor.Move(-1*Home_Offset);
        Serial.println("Moving away from the hard stop... Waiting for the step output to finish...");
    while (!motor.StepsComplete()) {
        continue;
    }

    // Zero the motor's reference position after homing to allow for accurate absolute position moves

    motor.PositionRefSet(Top);   //Defined in global variables.  
}