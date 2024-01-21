#include "main.h"

void WheelControl() {
    // The motor will follow user input on a wheel as measured by an encoder. 
    Serial.println("Entering the Wheel Control Loop ");

    // Create a timer to keep us in the while loop for X seconds
    uint32_t Wheel_timeout = 10000; //10 seconds
    uint32_t Wheel_startTime = millis();
    while (millis() - Wheel_startTime < Wheel_timeout) {
      
      Wheel_position = EncoderIn.Position(); // Read the encoder position
      Wheel_velocity = EncoderIn.Velocity(); // Read the encoder velocity

      
      // Map the wheel velocity (counts/second) to an appropriate motor speed (Pulses/sec).
      // Encoder: 
        // 1 revolution in 5 seconds on the wheel = 200 counts/second
        // 250 RPM (Seems hard to imagine going that fast) = 4167 counts/second  
      // Motor: (Pulse output)
        // 0 - 3000 seems reasonable to start, may want to adjust
      User_Speed = map(Wheel_velocity,0,4000,0,3000);

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

    }
    
    motor.MoveStopDecel(4000); // Stop the motor
    motor.VelMax(2000); // Reset the motor velocity to something reasonable (in case wheel was at 0)
    Serial.println("Exiting Wheel Control Mode ");
}