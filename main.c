#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/drip.h"
#include "images/crewmate.h"
#include "images/victory.h"
#include "images/defeat.h"


enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
};

int main(void) {
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;

  struct position crewboi;
  crewboi.row = 100;
  crewboi.col = 70;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons
  
    waitForVBlank();

    switch (state) {
      case START:
        drawFullScreenImageDMA(drip);
        char *title = "Press START to begin   (Hint: Red Sus)";
        drawString(150, 5, title, BLACK);
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          state = PLAY;
          vBlankCounter = 0;
        } // press start (enter) to play
        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
        state = START;
        vBlankCounter = 0;
        crewboi.row = 100;
          crewboi.col = 70;
      } 
        break;
      case PLAY:
        waitForVBlank();
        fillScreenDMA(GREEN); // green background
        drawRectDMA(130, 200, 20, 20, RED); // red square
        drawRectDMA(50, 200, 20, 20, BLUE); // blue square
        int sec = (int) (vBlankCounter / 120); // seconds
        char buff[200];
        snprintf(buff, 200, "%d", sec);
        drawString(150, 5, buff, BLACK); // This is will draw time elapsed
        drawImageDMA(crewboi.row, crewboi.col, CREWMATE_WIDTH, CREWMATE_HEIGHT, crewmate); // draw crewmate
        if ((KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) && crewboi.row > 10) {
          crewboi.row = crewboi.row - 10;
        }
        if ((KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) && (crewboi.row + 47) < HEIGHT) {
          crewboi.row = crewboi.row + 10;
        }
        if ((KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) && (crewboi.col + 40) < WIDTH) {
          crewboi.col = crewboi.col + 10;
        }
        if ((KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) && crewboi.col > 0) {
          crewboi.col = crewboi.col - 10;
        }
        if (((crewboi.row >= 130 && crewboi.row <= 150) || (crewboi.row + 45 >= 130 && crewboi.row + 45 <= 150)) && 
        ((crewboi.col >= 200 && crewboi.col <= 220) || (crewboi.col + 36 >= 200 && crewboi.col + 36 <= 220))) {
          state = LOSE;
        }
        if (((crewboi.row >= 50 && crewboi.row <= 70) || (crewboi.row + 45 >= 50 && crewboi.row + 45 <= 70)) && 
        ((crewboi.col >= 200 && crewboi.col <= 220) || (crewboi.col + 36 >= 200 && crewboi.col + 36 <= 220))) {
          state = WIN;
        }
        if (sec >= 21) {
          state = LOSE;
        }
        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
          state = START;
          vBlankCounter = 0;
          crewboi.row = 100;
          crewboi.col = 70;
        }
        
        break;
      case WIN:
        drawFullScreenImageDMA(victory);
        char *win = "Good Job! Blue was IMPOSTOR!";
        drawString(150, 5, win, BLUE);
        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
          state = START;
          vBlankCounter = 0;
          crewboi.row = 100;
          crewboi.col = 70;
        }
        break;
      case LOSE:
        drawFullScreenImageDMA(defeat);
        char *lose = "You got ejected for being sus";
        drawString(150, 5, lose, RED);
        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
          state = START;
          vBlankCounter = 0;
          crewboi.row = 100;
          crewboi.col = 70;
        }
        break;
        
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  return 0;
}
