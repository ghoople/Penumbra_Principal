
#include "main.h"

void MoveAbsolutePosition(int position, int velocityLimit) {
    // Moves the motor to an absolute position, input is the number of steps away from 0. 
    // Position can be reset 

    //Serial.print("Current position is: ");
    //Serial.println(motor.PositionRefCommanded());
    //Serial.print("Moving to absolute position: ");
    //Serial.println(position);

    // Sets the maximum velocity for this move
    motor.VelMax(velocityLimit);
    // Command the move of absolute distance
    motor.Move(position, MotorDriver::MOVE_TARGET_ABSOLUTE);

    // Waits for all step pulses to output, but it needs to be on the lookout for changes to the encoder in case a user comes up to the piece. 
    while (!motor.StepsComplete()) {
      
      // Monitor Encoder on Wheel for Changes
      Wheel_position = EncoderIn.Position(); // Read the encoder position
      Wheel_velocity = EncoderIn.Velocity(); // Read the encoder velocity
            if(Wheel_velocity > 20){ // Choose an optimal wheel_velocity to use for this
        motor.MoveStopDecel(4000); // Stop the motor
        WheelControl();
      }

      // Monitor the Motor Position to send for the LED array in the control tower. I need to figure out if this actually output the current position of the motor.
      // I think it's reading where the step generator currently is and NOT the final target. But it is not completely clear from the documentation.  If this actually
      // follows along with the motor position then I could use this to control the LED. Alternatively I'd need to setup an encorder. 

      // motor.PositionRefCommanded()
      // Add mapping command here. 
      // Add serial coms here. 

    }

		Serial.println("Absolute Position Move Complete");
}