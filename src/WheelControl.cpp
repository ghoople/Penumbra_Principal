#include "main.h"
#include <Arduino.h>

void WheelControl() {
    // The motor will follow user input on a wheel as measured by an encoder. 
    Serial.println("Entering the Wheel Control Loop ");

    // Declare variables needed for this function only. 
    int32_t Wheel_position = 0;
    int32_t last_Wheel_position = 0;
    int32_t Wheel_velocity = 0;
    int totalVelocity;
    int readingsCount;
    //int32_t indexPosition = 0;
    //int32_t lastIndexPosition = 0;
    //bool quadratureError = false;

    // Variables for Calculating encoder. 

    // Create a timer to keep us in the while loop for X seconds
    uint32_t Wheel_timeout = 10000; //10 seconds
    uint32_t Wheel_startTime = millis();
    while (millis() - Wheel_startTime < Wheel_timeout) {
      
      Wheel_position = EncoderIn.Position(); // Read the encoder position
      Wheel_velocity = EncoderIn.Velocity(); // Read the encoder velocity
      //Serial.print("Wheel Position: ");
      //Serial.println(Wheel_position);
      //Serial.print("Wheel Velocity: ");

      totalVelocity += Wheel_velocity;
      readingsCount++;

      int Wheel_currentTime = millis();
      int Wheel_startTime = Wheel_currentTime;
      int motorStartTime = Wheel_currentTime;
      int motorTime = Wheel_currentTime;
      int averageVelocity = 0;

      // Check if 0.1 seconds have passed
      if (Wheel_currentTime - Wheel_startTime >= 100) {
        // Calculate average velocity over 0.1 seconds
        int averageVelocity = totalVelocity / readingsCount;

        // Print the average velocity
        Serial.print("Average Wheel Velocity: ");
        Serial.println(averageVelocity);

        // Reset variables for the next interval
        totalVelocity = 0.0;
        readingsCount = 0;
        Wheel_startTime = Wheel_currentTime;
      }

      // Map the wheel velocity (counts/second) to an appropriate motor speed (Pulses/sec).
      // Encoder: 
        // 1 revolution in 5 seconds on the wheel = 200 counts/second
        // 250 RPM = 4.2 rev/s (Seems hard to imagine going that fast) = 42,000 counts/second  
      // Motor: (Pulse output)
        // 0 - 3000 seems reasonable to start, may want to adjust
      int User_Speed = map(abs(averageVelocity),0,42000,0,3000);


      // This Loop Occurs Every 200 ms. Updating faster seems to cause the motor to freak out. 
      if (motorTime - motorStartTime >= 200) {
        if(Wheel_position > last_Wheel_position){ // Go UP
          motor.VelMax(User_Speed); // Sets the maximum velocity for this move based on the potentiometer
          motor.Move(Pos_Top, MotorDriver::MOVE_TARGET_ABSOLUTE); // Commands the motor move (with no waiting step before commanding additional moves!)
          //Serial.print("Moving up at user speed: ");
          //Serial.println(User_Speed);
        }
        else{ // Go Down
          motor.VelMax(User_Speed); // Sets the maximum velocity for this move based on the potentiometer
          motor.Move(Pos_Bottom, MotorDriver::MOVE_TARGET_ABSOLUTE); // Commands the motor move (with no waiting step before commanding additional moves!)
        }
        last_Wheel_position = Wheel_position;
        motorStartTime = motorTime;       

    }
    
    motor.MoveStopDecel(4000); // Stop the motor
    motor.VelMax(2000); // Reset the motor velocity to something reasonable (in case wheel was at 0)
    Serial.println("Exiting Wheel Control Mode ");
}

}