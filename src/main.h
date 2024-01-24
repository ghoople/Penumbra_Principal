// Penumbra_Principal.h

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ClearCore.h>

// My code (and example code) uses motor instead of the connector name. 
// Options are: ConnectorM0, ConnectorM1, ConnectorM2, or ConnectorM3.
#define motor ConnectorM0
#define motorDecel 4000 //Defines how quickly the motor should decelerate when switching into/out of user mode. 

// Define the physical relationship between steps and light position
// See "Penumbra Motor Calculations" google sheet for value calculator
#define Bot 0 
#define Top 7858
#define Mid Top/2
#define Home_Offset 291

// Define how long the index is for the halogen lighting array. Can't be an int. 
#define halIndexLength 100

// Alert compiler to key global variables 
// These don't get passed to functions, but are used by them. 
extern int velocityLimit;
extern int accelerationLimit;

// Function Declarations
void Pause(int pauseTime, int* halA, int* halB);
void MoveTarget(int target, int velocityLimit, int* halA, int* halB);
void BottomHardStop();
void TopHardStop();
void WheelControl();
void Animation(int showNum);

#endif // MAIN_H
