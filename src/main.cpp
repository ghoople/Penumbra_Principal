/*
 * Penumbra_Principal
 *
 *  This is the primary code base that will run on the clear core.
 * 
 */
#include "main.h"
// Test

// Configure Interrupt Pins for the hard stops
// Pins that support digital interrupts on clear core are:
// DI-6, DI-7, DI-8, A-9, A-10, A-11, A-12
#define BotInterruptPin A10
#define TopInterruptPin A11

// Define the baud rate for the serial devices
#define usbBaudRate 9600
#define agentBaudRate 9600

// Define the initial velocity limit, acceleration limit, and commanded deceleration to be used for default moves
#define initialVelocityLimit 2000 // pulses per sec

#define homingVelocity 500 //Velocity to home the motor. 

void setup() {
    bool debug = true;
    // Communications Setup
        // Serial Coms to the USB Port (with timeout)
        Serial.begin(usbBaudRate);
        uint32_t timeout = 5000; // 5 second timeout
        uint32_t startTime = millis();
        while (!Serial && millis() - startTime < timeout) {
            continue;
        }

        // Serials Coms to the agent arduino via Serial 1 port. 
        Serial1.begin(agentBaudRate);
        Serial1.ttl(true); // Tells serial one to use TTL logiv (5V signals)
        startTime = millis();
        while (!Serial1 && millis() - startTime < timeout) {
            continue;
        }

    // Configure the hard stops
        pinMode(BotInterruptPin, INPUT);
        pinMode(TopInterruptPin, INPUT);
        attachInterrupt(digitalPinToInterrupt(BotInterruptPin), BottomHardStop, RISING);
        attachInterrupt(digitalPinToInterrupt(TopInterruptPin), TopHardStop, RISING);
    
    //Configure Potentiometers as inputs
        pinMode(potAPin, INPUT);
        pinMode(potBPin, INPUT);
    
    // Motor Setup
        // Sets the input clocking rate.
        MotorMgr.MotorInputClocking(MotorManager::CLOCK_RATE_LOW);

        // Sets all motor connectors into step and direction mode.
        MotorMgr.MotorModeSet(MotorManager::MOTOR_ALL,
                            Connector::CPM_MODE_STEP_AND_DIR);

        // Sets the maximum velocity for each move
        motor.VelMax(initialVelocityLimit);

        // Set the maximum acceleration for each move
        motor.AccelMax(accelerationLimit);

        // Enables output to the motor (must have, even if not using the enable pin on the motor)
        motor.EnableRequest(true);

        /* Disable Homing While Testing Lights

        // Home the motor
        if(debug){Serial.println("Homing Motor");}
        motor.MoveVelocity(homingVelocity); // Move down, will stop when the hard stop is tripped.  
        // Wait for interrupt to trigger when hard stop reached, interrupt code will automatically set the 0 position.
        while (!motor.StepsComplete()) {
            continue;
        }
        
        if(debug){Serial.println("Homing Complete");}
        */
        motor.PositionRefSet(0); // Remove when adding homing code back in above
    
    // Encoder Setup
        EncoderIn.Enable(true); // Enable the encoder input feature      
        EncoderIn.SwapDirection(false); // Swap direction if you want for the encoder
        EncoderIn.IndexInverted(false); // Set the sense of index detection (true = rising edge, false = falling edge)
        EncoderIn.Position(0); // Zero the encoder position to match the zero of the motor. 

    delay(1000); // Probably don't need this. But can give it all a second to catchup. 
    if(debug){Serial.println("Setup complete");}
}

void loop() {
    //Animation Order Defined Here

// This sets the motor to go to the bottom, just needed for blackbox demo. 
    motor.VelMax(1000);
    // Command the move of absolute distance
    motor.Move(Bot, MotorDriver::MOVE_TARGET_ABSOLUTE);
    while (!motor.StepsComplete()) {
        continue;}

    //Animation(1);
    //Animation(2);
    Animation(3);

    Serial.println("Animation Loop Complete");
    delay(5000);
}
