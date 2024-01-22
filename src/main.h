// Penumbra_Principal.h

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ClearCore.h>

// My code (and example code) uses motor instead of the connector name. 
// Options are: ConnectorM0, ConnectorM1, ConnectorM2, or ConnectorM3.
#define motor ConnectorM0


// Alert compiler to key global variables. 
// These don't get passed to functions, but are used by them. 
extern int velocityLimit;
extern int accelerationLimit;
extern int motorDecel;
extern int Pos_Bottom;
extern int Pos_Top;
extern int Pos_Middle;
extern int Home_Offset;

// Function Declarations

void MoveAbsolutePosition(int position, int velocity, int brightA, int brightB);
void BottomHardStop();
void TopHardStop();
void WheelControl();

#endif // MAIN_H
