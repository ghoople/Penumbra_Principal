/*
 * Penumbra_Principal
 *
 *  This is the primary code base that will run on the clear core.
 * 
 */
#include "main.h"

// Configure Interrupts
// Pins that support digital interrupts are:
// DI-6, DI-7, DI-8, A-9, A-10, A-11, A-12
#define BotInterruptPin DI6
#define TopInterruptPin DI7

// Select the baud rate for the serial device
#define usbBaudRate 9600
#define agentBaudRate 28800

// Variables to initialize the encoder
// Set to true if index detection should occur on the falling edge, rather than the rising edge.
bool indexInverted = false; 
// Set to true if the sense of encoder direction should be inverted.
bool swapDirection = false; 


// Define the initial velocity limit, acceleration limit, and commanded deceleration to be used for default moves
int velocityLimit = 2300; // pulses per sec
int accelerationLimit = 9000; // pulses per sec^2
int motorDecel = 4000; //Defines how quickly the motor should decelerate when switching to user mode. 
int homingVelocity = -2000; //Velocity to home the motor. 

// Define the physical relationship between steps and light position
// See "Penumbra Motor Calculations" google sheet for value calculator
int Pos_Bottom = 0; 
int Pos_Top = 7858;
int Pos_Middle = Pos_Top/2;
int Home_Offset = 291;

void setup() {
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
    
    // Motor Setup
        // Sets the input clocking rate.
        MotorMgr.MotorInputClocking(MotorManager::CLOCK_RATE_LOW);

        // Sets all motor connectors into step and direction mode.
        MotorMgr.MotorModeSet(MotorManager::MOTOR_ALL,
                            Connector::CPM_MODE_STEP_AND_DIR);

        // Sets the maximum velocity for each move
        motor.VelMax(velocityLimit);

        // Set the maximum acceleration for each move
        motor.AccelMax(accelerationLimit);

        // Enables output to the motor (must have, even if not using the enable pin on the motor)
        motor.EnableRequest(true);

        // Serial.println("Motor Ready");

        // Home the motor
        Serial.println("Homing Motor");
        motor.MoveVelocity(homingVelocity); // Move down, will stop when the hard stop is tripped.  
        // Wait for interrupt to trigger when hard stop reached, interrupt code will automatically set the 0 position.
        while (!motor.StepsComplete()) {
            continue;
        }
        
        Serial.println("Homing Complete");

    // Encoder Setup
        // Enable the encoder input feature
        EncoderIn.Enable(true);
        // Set the encoder direction
        EncoderIn.SwapDirection(swapDirection);
        // Set the sense of index detection (true = rising edge, false = falling edge)
        EncoderIn.IndexInverted(indexInverted);
        // Zero the encoder position to match the zero of the motor. 
        EncoderIn.Position(0);

    delay(1000); // Probably don't need this. 
}

void loop() {
    //Put your main code here, it will run repeatedly:

    String showName = "MyShow";
        // Move to middle
        MoveTarget(Pos_Middle,2000,showName);

        // "Pause" for 5 seconds by going really slowly. Delay only happens if you are close to the commanded position (Jog mode can make the motor move reaaaallly slowly)
        if(abs(motor.PositionRefCommanded()-Pos_Middle)<10){
            MoveTarget(Pos_Middle+5,1,showName);
        }
        
        // Move to the top
        MoveTarget(Pos_Top,2000,showName);

        //Move to the bottom
        MoveTarget(Pos_Bottom,2000,showName);

        // "Pause" for 5 seconds by going really slow
        if(abs(motor.PositionRefCommanded()-Pos_Bottom)<10){
            MoveTarget(Pos_Bottom+5,1,showName);
        }  

}
