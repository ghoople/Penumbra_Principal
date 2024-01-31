#include "main.h"

/* WheelControl() is the function that is called when a user starts to move the wheel. 
It allows the user to control the light for a defined interval of time. 
*/

/* Set the Maximum speeds for mapping the encoder speed to the motor speed. 
  For the Encoder: 
    1 revolution in 5 seconds on the wheel = 200 counts/second
    250 RPM = 4.2 rev/s (Seems hard to imagine going that fast) = 42,000 counts/second  
  For the motor: (Pulses/s output)
    0 - 5000 seems reasonable to start, may want to adjust
*/
#define encoderMaxSpeed 42000 
#define motorMaxSpeed 5000  // 5000 is for the chain system, 3000 better for the wire system. 

// Set the update intervals for the timer loops. 
#define wheelTimeout 10000 // 10 seconds
#define motorUpdateInterval 200 // 200 ms
#define encoderAverageInterval 100 // 100 ms
#define motorVelocityReset 2000 // Resets the motor velocity at the end of user interaction. 


void WheelControl() {
    bool debug = false;
    // The motor will follow user input on a wheel as measured by an encoder. 
    if(debug){Serial.println("Entering the Wheel Control Loop ");}

    // Declare variables needed for this function only. 
    int32_t wheelPosition = 0;
    int32_t lastWheelPosition = 0;
    int32_t wheelVelocity = 0;
    int totalVelocity = 0;
    int readingsCount = 0;
    int userSpeed = 0; 

/*
    //Code to read the potentiometers
    int potA = analogRead(potAPin);
    int potB = analogRead(potBPin);

    potA = map(potA,0,1023,0,255); // Map the potentiometer values to the range of 0-255. Will need to adapt based on actual potentiometer readings. 
    potB = map(potB,0,1023,0,255);
*/

// Temp code until I get potentiometers working
    int potA = 120;
    int potB = 0; 

    // This timer keeps us in a while loop for wheelTimeout seconds. This is how long the user can play with the wheel before reverting back to the original code.   
    uint32_t wheelStartTime, encoderStartTime, motorStartTime;
    wheelStartTime = encoderStartTime = motorStartTime = millis();
    
    while (millis() - wheelStartTime < wheelTimeout) {
      
      wheelPosition = EncoderIn.Position(); // Read the encoder position
      wheelVelocity = EncoderIn.Velocity(); // Read the encoder velocity

      totalVelocity += wheelVelocity; // Add the wheel velocity to the totalVelocity variable. 
      readingsCount++; // Increment the number of readings we have taken

      // This timer calculates the average velocity over a window of 0.1 seconds
      if (millis() - encoderStartTime >= encoderAverageInterval) {
        // Calculate average velocity over 0.1 seconds
        float averageVelocity = totalVelocity / readingsCount;

        // Print the average velocity
        // Serial.print("Average Wheel Velocity: ");
        // Serial.println(averageVelocity);
        
        //Convert the averageVelocity (counts/second) to an appropriate motor speed (Pulses/sec).
        userSpeed = map(abs(averageVelocity),0,encoderMaxSpeed,0,motorMaxSpeed);

        // Reset variables for the next interval
        totalVelocity = 0;
        readingsCount = 0;
        encoderStartTime = millis();
      }

      // This timer makes it so the motor gets updated position commands every motorUpdateInterval seconds
      // Going faster seems to cause unwanted behavior.  
      // It also tells the agent where the motor is and that a user is controlling it.
      if (millis() - motorStartTime >= motorUpdateInterval) {
        if(wheelPosition > lastWheelPosition){ // Go UP
          motor.VelMax(userSpeed); // Sets the maximum velocity for this move based on the potentiometer
          motor.Move(Top, MotorDriver::MOVE_TARGET_ABSOLUTE); // Commands the motor move (with no waiting step before commanding additional moves!)
          //Serial.print("Moving up at user speed: ");
          //Serial.println(userSpeed);
        }
        else{ // Go Down
          motor.VelMax(userSpeed); // Sets the maximum velocity for this move based on the userSpeed
          motor.Move(Bot, MotorDriver::MOVE_TARGET_ABSOLUTE); // Commands the motor move (with no waiting step before commanding additional moves!)
        }
        lastWheelPosition = wheelPosition;
        motorStartTime = millis();
        
        // Send the intesity data to the halogen lights

// Add this Code! 

        // Send the required data to the agent arduino
        int currentPos = motor.PositionRefCommanded();

        Serial1.print(currentPos); // Tell the Agent where the light is. 
        Serial1.print(","); 
        Serial1.print(potA);// Tell the agent what the intensity should be for halA
        Serial1.print(","); 
        Serial1.println(potB);// Tell the agent what the intensity should be for halB


      }
    
    motor.MoveStopDecel(motorDecel); // Stop the motor at the deceleration limit (set in main.cpp)
    motor.VelMax(motorVelocityReset); // Reset the motor velocity to something reasonable (in case wheel was at 0)
    if(debug){Serial.println("Exiting Wheel Control Mode ");}
    }

}