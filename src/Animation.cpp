#include "main.h"

int moveVel; // How fast the move
int halA [halIndexLength];
int halB [halIndexLength];

// Velocities, in pulses/second, for the light movements. 
#define fast 3000
#define med 2000
#define slow 1000

// DMX Brightness Levels, range is 0 -> 255
#define maxBright 255
#define midBright 127
#define lowBright 50
#define off 0

// By Convention all animations should end at the bottom (So you can assume you are starting from the bottom) 
// After homing the motor will start at the bottom. 

void Animation(int showNum) {
switch (showNum) {
  case 1: // Move to the Top and back to the Bottom. Lights on only when going up.  
        moveVel = fast;
        // Step 1: Move to the top, lights max
        // Set halA and halB to the max brightness everywhere
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = maxBright;
            halB[i] = maxBright;
        }
        MoveTarget(Top,moveVel,halA,halB);

        // Step 2: Pause
        Pause(10,halA,halB); // pause for 10 seconds
        
        // Step 3: Move to the bottom, lights low
        // Set halA and halB to the max brightness everywhere
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = lowBright;
            halB[i] = lowBright;
        }
        MoveTarget(Bot,moveVel,halA,halB);

    break;
  case 2: // Lights are on when they are moving up only. 
        moveVel = fast;

        // Step 1: Move to the top, halA on

        // Turn halA on for entire move
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = maxBright;
        }

        // Turn halB off for entire move 
        for (int i = 0; i < halIndexLength; i++) {
            halB[i] = off;
        }        
        MoveTarget(Top,moveVel,halA,halB);

        // Step 2: Move to the bottom, swap the lights
        // Turn halA off for entire move
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = off;
        }

        // Turn halB on for entire move 
        for (int i = 0; i < halIndexLength; i++) {
            halB[i] = maxBright;
        }        

        MoveTarget(Bot,moveVel,halA,halB);
    break;
  case 3: // Ramp the brightness as the light moves up 
        moveVel = fast;

        // Step 1: Move to the top with the lights on

        // Turn halA on for entire move
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = i/halIndexLength * midBright;
        }

        // Turn halB off for entire move 
        for (int i = 0; i < halIndexLength; i++) {
            halB[i] = off;
        }        
        MoveTarget(Top,moveVel,halA,halB);

        // Step 2: Move to the bottom, swap the lights
        // Turn halA off for entire move
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = off;
        }

        // Turn halB on for entire move 
        for (int i = 0; i < halIndexLength; i++) {
            halB[i] = midBright + i/halIndexLength * midBright; // Start at the middle brightness, ramp up to full
        }        

        MoveTarget(Bot,moveVel,halA,halB);
    break;
  case 4:
    // statements
    break;
  case 5:
    // statements
    break;
  case 6:
    // statements
    break;
  default:
    // statements
    break;
}





}

