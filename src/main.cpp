/*
 * Penumbra_Principal
 *
 *  This is the primary code base that will run on the clear core.
 * 
 */
#include <Arduino.h>
#include <ClearCore.h>

// Specifies which motor to move.
// Options are: ConnectorM0, ConnectorM1, ConnectorM2, or ConnectorM3.
#define motor ConnectorM0

// Select the baud rate to match the target serial device
#define baudRate 9600

// Configure Interrupts
// Pins that support digital interrupts are:
// DI-6, DI-7, DI-8, A-9, A-10, A-11, A-12
#define BotInterruptPin DI6
#define TopInterruptPin DI7

// Variables to store encoder state
int32_t Wheel_position = 0;
int32_t last_Wheel_position = 0;
int32_t Wheel_velocity = 0;
int32_t indexPosition = 0;
int32_t lastIndexPosition = 0;
bool quadratureError = false;

// Set to true if index detection should occur on the falling edge, rather than the rising edge.
bool indexInverted = false; 
// Set to true if the sense of encoder direction should be inverted.
bool swapDirection = false; 

// Create Variables for the state of the inputs
int User_Speed = 0; 

// Define the initial velocity and acceleration limits to be used for default moves
int velocityLimit = 2300; // pulses per sec
int accelerationLimit = 4600; // pulses per sec^2

// Define the physical relationship between steps and light position
// See "Penumbra Motor Calculations" google sheet for value calculator
int Pos_Bottom = 0; 
int Pos_Top = 7858;
int Pos_Middle = Pos_Top/2;
int Home_Offset = 291;


// Declare functions included in this file
void MoveAbsolutePosition(int position, int velocity); //Function to command movement
void BottomHardStop(); // Interrupt for when the bottom hard stop is triggered
void TopHardStop();  // Interrupt for when the top hard stop is triggered
void WheelControl(); // Loop to enter for user control


void setup() {
    // This setup function only runs once

    // Sets the input clocking rate.
    MotorMgr.MotorInputClocking(MotorManager::CLOCK_RATE_LOW);

    // Sets all motor connectors into step and direction mode.
    MotorMgr.MotorModeSet(MotorManager::MOTOR_ALL,
                          Connector::CPM_MODE_STEP_AND_DIR);

    // Sets the maximum velocity for each move
    motor.VelMax(velocityLimit);

    // Set the maximum acceleration for each move
    motor.AccelMax(accelerationLimit);

    // Sets up serial communication and waits up to 5 seconds for a port to open.
    // Serial communication is not required for this example to run.
    Serial.begin(baudRate);
    uint32_t timeout = 5000;
    uint32_t startTime = millis();
    while (!Serial && millis() - startTime < timeout) {
        continue;
    }

    // Configure the input pins
    pinMode(BotInterruptPin, INPUT);
    pinMode(TopInterruptPin, INPUT);

    // Setup Interrupt for Hard Stop at bottom
    // Might need to be low, depending on how the switch is configured.)
    // Attaches to Digital Input 6 
    attachInterrupt(digitalPinToInterrupt(BotInterruptPin), BottomHardStop, RISING);
    attachInterrupt(digitalPinToInterrupt(TopInterruptPin), TopHardStop, RISING);

    // Enables output to the motor (must have, even if not using the enable pin on the motor)
    motor.EnableRequest(true);

    Serial.println("Motor Ready");
    
    // Enable the encoder input feature
    EncoderIn.Enable(true);
    // Set the encoder direction
    EncoderIn.SwapDirection(swapDirection);
    // Set the sense of index detection (true = rising edge, false = falling edge)
    EncoderIn.IndexInverted(indexInverted);

    // Home the motor
    Serial.println("Homing Motor");
    motor.MoveVelocity(-1000); // Move down at 1000 pulses/sec, speed may need to be tweaked for safety.  
      // Wait for interrupt to trigger when hard stop reached, interrupt code will automatically set the 0 position.
      while (!motor.StepsComplete()) {
        continue;
      }
    // Zero the encoder position to match the zero of the motor. 
    EncoderIn.Position(0);

    Serial.println("Homing Complete");
    delay(3000);
}

void loop() {
    //Put your main code here, it will run repeatedly:

    // Move to middle
    MoveAbsolutePosition(Pos_Middle,2000);

    // "Pause" for 5 seconds by going really slowly. Delay only happens if you are close to the commanded position (Jog mode can make the motor move reaaaallly slowly)
    if(abs(motor.PositionRefCommanded()-Pos_Middle)<10){
        MoveAbsolutePosition(Pos_Middle+5,1);
    }
    
    // Move to the top
    MoveAbsolutePosition(Pos_Top,2000);

    //Move to the bottom
    MoveAbsolutePosition(Pos_Bottom,2000);

    // "Pause" for 5 seconds by going really slow
    if(abs(motor.PositionRefCommanded()-Pos_Bottom)<10){
        MoveAbsolutePosition(Pos_Bottom+5,1);
    }  
}
