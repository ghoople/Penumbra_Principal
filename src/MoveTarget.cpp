#include "main.h"

int wheelThreshold = 200; // Define what speed the encoder needs to move at for it to be considered a user input (200 pulses/second) 

// Fix this function definition to reflect my arrays

void MoveTarget(int target, int velocityLimit, int halogenA, int halogenB) {
    // Moves the motor to an absolute target, target is relative to our home of 0. 
    // target can be reset 

    //Serial.print("Current target is: ");
    //Serial.println(motor.PositionRefCommanded());
    //Serial.print("Moving to absolute target: ");
    //Serial.println(target);

    // Sets the maximum velocity for this move
    motor.VelMax(velocityLimit);

    // Command the move of absolute distance
    motor.Move(target, MotorDriver::MOVE_TARGET_ABSOLUTE);

    /* Waits for all step pulses to output While waiting it needs to: 
    1. Monitor the encoder for velocity changes
    2. Track the position of the motor
    3. Send position and show number information to the agent arduino.
    */

    int32_t lastUpdateTime = millis();
    int32_t updateInterval = 50; //  ms delay between updates to the arduino. Could go faster depending on the Baudrate? May not even need this?  

    while (!motor.StepsComplete()) {

      // Monitor Encoder for Velocity Changes
      int32_t Wheel_move = EncoderIn.Velocity(); // Read the encoder velocity
      if(Wheel_move > wheelThreshold){ // Choose an optimal wheel_velocity to use for this
        motor.MoveStopDecel(motorDecel); // Stop the motor
        WheelControl(); // Go to wheel control
      }
      // Timer for updating the arduino agent with position and show data. 
      if (millis() - lastUpdateTime >= updateInterval) {
        // Send target and show data to the Agent Arduino for lighting
        Serial1.print(motor.PositionRefCommanded()); // Tell the Agent where the light is. 
        Serial1.print(","); 
        Serial1.println(showNum);// Tell the agent what show we are doing
        lastUpdateTime = millis(); 
      }

      // Update the HalogenA


    }
		Serial.println("Absolute target Move Complete");
}