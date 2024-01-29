#include "main.h"

void Pause(int pauseTime, int* halA, int* halB){
  int currentPos = motor.PositionRefCommanded();
  if(currentPos>Mid){ // In the top half
    MoveTarget(currentPos-pauseTime,1,halA,halB); // Move at 1 pulse per second for pauseTime steps
    }
  else{ // In bottom half
    MoveTarget(currentPos+pauseTime,1,halA,halB); // Move at 1 pulse per second for pauseTime steps
  }
}

void MoveTarget(int target, int velocityLimit, int* halA, int* halB) {
    bool debug = false;
    int wheelThreshold = 200; // Define what speed the encoder needs to move at for it to be considered a user input (200 pulses/second) 
    int currentPos=0; // Where halA currently is. 
    int startPos=0; // Where halA ligth starts. 
    float moveDist; // Distance of the commanded move. 
    int halAIndex=0; // Index of the halA array
    int halBIndex=0; // Index of the halB array
    unsigned long updateInterval; // Update interval for arduino agent. Declare the variable outside the if-else blocks

    // Set the update interval for the arduino agent, based on debug mode.
    if(debug){
      updateInterval = 1000; // 1 second update interval
    }
    else{
      updateInterval = 100; // 50 ms update interval
    }

    if(debug){
      Serial.println("Moving to absolute target: " + String(target));
      Serial.print("Current position is: ");
      Serial.println(motor.PositionRefCommanded());
    }

    // Moves the motor to an absolute target, target is relative to our home of 0. 
    startPos = motor.PositionRefCommanded(); // Where is the motor now? 
    moveDist = abs(target - startPos)+1; // Calculate the distance of the prescribed move, add 1 to avoid divide by 0 error.

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


// Before calculations
//Serial.println("Before calculations:");
//Serial.print("currentPos: "); Serial.println(currentPos);
//Serial.print("target: "); Serial.println(target);
//Serial.print("moveDist: "); Serial.println(moveDist);
//Serial.print("halIndexLength: "); Serial.println(halIndexLength);
//Serial.print("halAIndex: "); Serial.println(halAIndex);
//Serial.print("halBIndex: "); Serial.println(halBIndex);


        // Calculate the percentage of the way you are to the end of the move, this is your index 
        halAIndex = halIndexLength - round(abs(currentPos - target)/moveDist * halIndexLength);
        halBIndex = halIndexLength - round(abs(currentPos - target)/moveDist * halIndexLength);

// After calculations
//Serial.println("After calculations:");
//Serial.print("halAIndex: "); Serial.println(halAIndex);
//Serial.print("halBIndex: "); Serial.println(halBIndex);

        // Send the data to the agent arduino
        Serial1.print(currentPos); // Tell the Agent where the light is. 
        Serial1.print(","); 
        Serial1.print(halA[halAIndex]);// Tell the agent what the intensity should be for halA
        Serial1.print(","); 
        Serial1.println(halB[halBIndex]);// Tell the agent what the intensity should be for halB

        // Print debug statements to Serial Monitor
        if(debug){
          
          //Serial.println("Current Position: " + String(currentPos) + ", Intensity for halA: " + 
          //String(halA[halAIndex]) + ", Intensity for halB: " + String(halB[halBIndex]));
          
          
          Serial.print(currentPos); // Tell the Agent where the light is. 
          Serial.print(","); 
          Serial.print(halA[halAIndex]);// Tell the agent what the intensity should be for halA
          Serial.print(","); 
          Serial.println(halB[halBIndex]);// Tell the agent what the intensity should be for halB
          Serial.print("halA index:" + String(halAIndex));
          Serial.println("halB index:" + String(halBIndex));
        
        }
    
        lastUpdateTime = millis(); // Reset the timer
      }
    }

		if(debug){Serial.println("Move Complete");}
    
}