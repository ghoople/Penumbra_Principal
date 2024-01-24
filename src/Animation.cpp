#include "main.h"

int moveVel; // Defined here as it is ONLY used by this function, but we don't need to initialize it every time the function is called. 
int halA [halIndexLength];
int halB [halIndexLength];

// Velocities, in pulses/second, for the light movements. 
#define fast 4000
#define med 2000
#define slow 1000

// Brightness Levels
#define maxBright 255
#define midBright 125
#define lowBright 50

void Animation(int showNum) {
switch (showNum) {
  case 1: // Move Bottom -> Top -> Bottom. Lights on 
        
        // Step 1: Move to the bottom with the lights off
        moveVel = fast;
        // Set halA and halB off everywhere
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = 0;
            halB[i] = 0;
        }
        // Move to the bottom
        MoveTarget(Pos_Bottom,moveVel,halA,halB);
    
        // Step 2: Pause
        Pause(10,halA,halB); // pause for 10 seconds

        // Step 3: Move to the top, lights on
        // Set halA and halB to the max brightness everywhere
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = maxBright;
            halB[i] = maxBright;
        }
        MoveTarget(Pos_Top,moveVel,halA,halB);

        // Step 4: Pause
        Pause(10,halA,halB); // pause for 10 seconds
        
        // Step 5: Move to the bottom, lights medium
        // Set halA and halB to the max brightness everywhere
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = midBright;
            halB[i] = midBright;
        }
        MoveTarget(Pos_Bottom,moveVel,halA,halB);

    break;
  case 2:
    // statements
    break;
  case 3:
    // statements
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

