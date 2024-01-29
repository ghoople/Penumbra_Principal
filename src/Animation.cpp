#include "main.h"

// Velocities, in pulses/second, for the light movements. 
#define fast 2000
#define med 1000
#define slow 500

// DMX Brightness Levels, range is 0 -> 255
#define maxBright 40
#define midBright 20
#define lowBright 2
#define off 0

// By Convention all animations should end at the bottom (So you can assume you are starting from the bottom) 
// After homing the motor will start at the bottom. 

void Animation(int showNum) {
bool debug = true;
// Define variables
int moveVel; // How fast the move
int halA [halIndexLength];
int halB [halIndexLength];

switch (showNum) {
  case 1: // Move to the Top and back to the Bottom.
        moveVel = fast;
        // Step 1: Move to the top, lights max
        // Set halA and halB to the max brightness everywhere
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = maxBright;
            halB[i] = maxBright;
        }
        MoveTarget(Top,moveVel,halA,halB);
        if(debug){
            Serial.println("Running Animation 1: Max and Low");
            Serial.println("Target: Top, A: Max, B: Max");
        }

        // Step 2: Pause
        Pause(2,halA,halB); // pause for 2 seconds
        if(debug){Serial.println("Target: Pause, A: Max, B: Max");}
        
        // Step 3: Move to the bottom, lights low
        // Set halA and halB to the max brightness everywhere
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = lowBright;
            halB[i] = lowBright;
        }
        MoveTarget(Bot,moveVel,halA,halB);
        if(debug){Serial.println("Target: Bot, A: Low, B: Low");

    break;
  case 2: // Lights are on when they are moving up only. 
        moveVel = med;

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
        if(debug){
            Serial.println("Running Animation 2: Swap Lights");
            Serial.println("Target: Top, A: Max, B: Off");
        }
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
        if(debug){Serial.println("Target: Bot, A: Off, B: Max");}

    break;
  case 3: // Ramp the brightness of A on way up, B on way down.  
        moveVel = slow;

        // Step 1: Move to the top with the lights on
        // Turn halA on for entire move
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = i/halIndexLength * maxBright;
        }

        // Turn halB off for entire move 
        for (int i = 0; i < halIndexLength; i++) {
            halB[i] = off;
        }        
        MoveTarget(Top,moveVel,halA,halB);
        if(debug){
            Serial.println("Running Animation 3: Ramps");
            Serial.println("Target: Top, A: Ramp, B: Off");
        }
        // Step 2: Move to the bottom, swap the lights
        // Turn halA off for entire move
        for (int i = 0; i < halIndexLength; i++) {
            halA[i] = off;
        }

        // Turn halB on for entire move 
        for (int i = 0; i < halIndexLength; i++) {
            halB[i] = i/halIndexLength * maxBright; // Start off, ramp up to full
        }        

        MoveTarget(Bot,moveVel,halA,halB);
        if(debug){Serial.println("Target: Bot, A: Off, B: Ramp");}

    break;
  case 4:
    moveVel = slow;
    // Step 1: Move to the top with the lights on
    // Ramp halA and halB for entire move
    for (int i = 0; i < halIndexLength; i++) {
        halA[i] = i/halIndexLength * midBright;
        halB[i] = i/halIndexLength * midBright; // Ramp up halB as well
    }
    MoveTarget(Top,moveVel,halA,halB);

    // Step 2: Move to the bottom, keep ramping the lights
    // Ramp halA and halB for entire move
    for (int i = 0; i < halIndexLength; i++) {
        halA[i] = midBright + i/halIndexLength * midBright; // Ramp up halA as well
        halB[i] = midBright + i/halIndexLength * midBright; // Continue ramping up halB
    }
    MoveTarget(Bot,moveVel,halA,halB);




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

