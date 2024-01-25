// Penumbra_Principal.h

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ClearCore.h>


// My code (and example code) uses motor instead of the connector name. 
// Options are: ConnectorM0, ConnectorM1, ConnectorM2, or ConnectorM3.
#define motor ConnectorM0
#define motorDecel 4000 //Defines how quickly the motor should decelerate when switching into/out of user mode. 
#define accelerationLimit 9000 // pulses per sec^2

// Define the physical relationship between steps and light position
// See "Penumbra Motor Calculations" google sheet for value calculator
#define Bot 0 
#define Top 7840 // Blackbox Demo
#define Mid Top/2
#define Home_Offset 0 // Blackbox Demo offset (manual home button)

// Define how long the index is for the halogen lighting array. Can't be an int. 
#define halIndexLength 100


// Function Declarations
void Pause(int pauseTime, int* halA, int* halB);
void MoveTarget(int target, int velocityLimit, int* halA, int* halB);
void BottomHardStop();
void TopHardStop();
void WheelControl();
void Animation(int showNum);

#endif // MAIN_H
