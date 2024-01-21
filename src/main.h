// Penumbra_Principal.h

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ClearCore.h>

#define motor ConnectorM0

extern int32_t Wheel_position;
extern int32_t last_Wheel_position;
extern int32_t Wheel_velocity;
extern int32_t indexPosition;
extern int32_t lastIndexPosition;
extern bool quadratureError;

extern bool indexInverted;
extern bool swapDirection;

extern int User_Speed;

extern int velocityLimit;
extern int accelerationLimit;

extern int Pos_Bottom;
extern int Pos_Top;
extern int Pos_Middle;
extern int Home_Offset;

void MoveAbsolutePosition(int position, int velocity);
void BottomHardStop();
void TopHardStop();
void WheelControl();

#endif // MAIN_H
