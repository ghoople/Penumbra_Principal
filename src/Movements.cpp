#include "main.h"

int wheelThreshold = 200; // Define what speed the encoder needs to move at for it to be considered a user input (200 pulses/second) 
int currentPos; // Where halA currently is. 
int startPos; // Where halA ligth starts. 
int moveDist; // Distance of the commanded move. 
int halAIndex;
int halBIndex;

void Pause(int pauseTime, int* halA, int* halB){
  currentPos = motor.PositionRefCommanded();
  if(currentPos>Pos_Middle){ // In the top half
    MoveTarget(currentPos-pauseTime,1,halA,halB); // Move at 1 pulse per second for pauseTime steps
    }
  else{ // In bottom half
    MoveTarget(currentPos+pauseTime,1,halA,halB); // Move at 1 pulse per second for pauseTime steps
  }
}

void MoveTarget(int target, int velocityLimit, int* halA, int* halB) {
    // Moves the motor to an absolute target, target is relative to our home of 0. 
    // target can be reset 

    //Serial.print("Current target is: ");
    //Serial.println(motor.PositionRefCommanded());
    //Serial.print("Moving to absolute target: ");
    //Serial.println(target);

    startPos = motor.PositionRefCommanded(); // Where is the motor now? 
    moveDist = abs(target - startPos); // Calculate the distance of the prescribed move.

    // Sets the maximum velocity for this move
    motor.VelMax(velocityLimit);

    // Command the move of absolute distance
    motor.Move(target, MotorDriver::MOVE_TARGET_ABSOLUTE);

    /* Waits for all step pulses to output. While waiting it needs to: 
    1. Monitor the encoder for velocity changes (so user can take over)
    2. Track the position of the motor
    3. Send the motor position and corresponding light data to the agent arduino.
    */

    int32_t lastUpdateTime = millis();
    unsigned long updateInterval = 50; //  ms delay between updates to the arduino. Could go faster depending on the Baudrate? May not even need this?  

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
        currentPos = motor.PositionRefCommanded();

        // Calculate the percentage of the way you are to the end of the move, this is your index 
        halAIndex = round(abs(currentPos - target)/moveDist * halIndexLength); // 
        halBIndex = round(abs(currentPos - target)/moveDist * halIndexLength);

        // Write this logic, will need to know where it started. Calculates from start position, current position, and target position. 

        Serial1.print(currentPos); // Tell the Agent where the light is. 
        Serial1.print(","); 
        Serial1.println(halA[halAIndex]);// Tell the agent what the intensity should be for halA
        Serial1.print(","); 
        Serial1.println(halB[halBIndex]);// Tell the agent what the intensity should be for halB
        lastUpdateTime = millis(); 
      }
    }
		Serial.println("Move Complete");
}