/*


FOR THE GHOSTS, RESTRAINING THEM TO THE PATHS, THIS IS WHAT TO DO:
CHANGE 'cursorX' AND cursorY TO GHOSTX AND GHOSTY
CHANGE THE if (xVal > 555)
TO if (ghostpos == 1, 2, 3)
ETC ::::::: 1 IF THE GHOST IS BELOW, 2 IF THE GHOST
IS ABOVE, 3 IF ... ETC




I THINK CAN DO SAME WITH THE PLAYER - COULD PROLLY MAKE INTO A ONE METHOD,
FOR THE PLAYER MAKE IF XVAL > 555 : variable = 1
METHOD WOULD BE
movement(xVAR, yVAR)
change the If (xVal > 555) to if (xvar == 1)


For the score dots, when pacman moves over them they don't get
redrawn, perfect for this project
  simply make a counter
  if (position of pacman % 2 [or something] == number ) {
  score counter += 10
}
do this for each line of dots



for the rewind part
  make it push the position of the player and ghosts to the stack every
  once in a while etc every 5 seconds

  to actually do the rewind part, take from off the stack the positions,
  and one at a time adjust the positions of the players and ghosts to match
  the popped position. after that is reached, take the next position, and
  repeat, going to that position






ORIENTATION

(0,            (240,
0)                  0)
  _______________
 |               |
 |               |
 |               |  YYYYYYYYYY
 |               |
 |               |
 |               |
 |_______________| (240, 320)
(0,320)
      XXXXXXXXXX



don't have to make the pathways exactly the size of pacman,
actually preferabbly not so user can have some wiggle room
all we have to do is make the ghosts follow the centre of the
path, like

|        .        |
|        .        |
|        .        |
|        .        |
|        .        |

make paths narrow enough so that pacman would obviously touch
ghosts


LETS ADD THE "1 UP" FEATURE >> IF SCORE == 100, LIVES++; SCORE = 0;
*/




#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <SD.h>
#include <TouchScreen.h>

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6


#define PACMAN_SIZE 7
#define GHOST_SIZE 7

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


// physical dimensions of the tft display (# of pixels)
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240


#define JOY_VERT  A1
#define JOY_HORIZ A0
#define JOY_SEL   2

#define JOY_CENTER   512
#define JOY_DEADZONE 64

// Pin definings from the lecture code for screen touching
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM  5  // can be a digital pin
#define XP  4  // can be a digital pin


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define MINPRESSURE   10
#define MAXPRESSURE 1000



int cursorX, cursorY;
int xmove, ymove;
int screenmidX = DISPLAY_WIDTH/2;
int screenmidY = DISPLAY_HEIGHT/2;




// struct dotsstruct {
//   int xcoord;
//   int ycoord;
//   bool passed;
//   bool operator==(const dotsstruct &other) const {
//     return (xcoord == other.xcoord
//             && ycoord == other.ycoord
//             && passed == other.passed);
//   }
// };





/*

struct ghoststruct {
  int xpos;
  int ypos;
  string colour; <- not sure if string type works here
}



*/

void travelling() {
  // starting line
  tft.drawLine(60, 240, 180, 240, ILI9341_WHITE);
  /*

 _____________
|             |

  */
  tft.drawLine(60, 240, 60, 270, ILI9341_WHITE);
  tft.drawLine(180, 240, 180, 270, ILI9341_WHITE);
/*
    _____________
___|             |___
*/
  tft.drawLine(60, 270, 20, 270, ILI9341_WHITE);
  tft.drawLine(180, 270, 220, 270, ILI9341_WHITE);
  /*
      _____________
  ___|             |___
  |                   |
  */
  tft.drawLine( 20, 270, 20, 300, ILI9341_WHITE);
  tft.drawLine( 220, 270, 220, 300, ILI9341_WHITE);
  /*
      _____________
  ___|             |___
  |                   |
  _____________________
  */
  tft.drawLine( 20, 300, 220, 300, ILI9341_WHITE);
  /*
      _____________
  ___|             |___
  |                   |
  _____________________
  */
  tft.drawLine( 90, 240,90, 270, ILI9341_WHITE);
  tft.drawLine( 150, 240, 150, 270, ILI9341_WHITE);

  tft.drawLine(90, 270, 110, 270, ILI9341_WHITE );
  tft.drawLine( 150, 270, 130, 270, ILI9341_WHITE);

  tft.drawLine(110, 270, 110, 300, ILI9341_WHITE);
  tft.drawLine(130, 270, 130, 300, ILI9341_WHITE);

  tft.drawLine(110, 240, 110,210, ILI9341_WHITE);
  tft.drawLine( 130, 240, 130, 210, ILI9341_WHITE);

  tft.drawLine(110, 210, 20, 210 ,ILI9341_WHITE);
  tft.drawLine( 130, 210, 220, 210,ILI9341_WHITE);

  tft.drawLine(20, 210, 20, 240 ,ILI9341_WHITE);
  tft.drawLine(220, 210, 220, 240 ,ILI9341_WHITE);

  tft.drawLine(20, 240, 40, 240 ,ILI9341_WHITE);
  tft.drawLine(220, 240, 200, 240 ,ILI9341_WHITE);

  tft.drawLine(40, 240, 40, 270 ,ILI9341_WHITE);
  tft.drawLine(200, 240, 200, 270 ,ILI9341_WHITE);

  tft.drawLine(60, 240, 60, 30 ,ILI9341_WHITE);
  tft.drawLine(180, 240, 180, 30 ,ILI9341_WHITE);

  tft.drawLine(20,30,110,30 ,ILI9341_WHITE);
  tft.drawLine(130,30,220,30 ,ILI9341_WHITE);

  tft.drawLine(110, 30, 110, 70 ,ILI9341_WHITE);
  tft.drawLine(130, 30, 130, 70 ,ILI9341_WHITE);

  tft.drawLine(20,30,20,110 ,ILI9341_WHITE);
  tft.drawLine(220,30,220,110 ,ILI9341_WHITE);

  tft.drawLine(20,70,220,70 ,ILI9341_WHITE);

  tft.drawLine(20,110,60,110 ,ILI9341_WHITE);
  tft.drawLine(180,110,220,110 ,ILI9341_WHITE);

  tft.drawLine(90,70,90,110 ,ILI9341_WHITE);
  tft.drawLine(150,70,150,110 ,ILI9341_WHITE);

  tft.drawLine(90,110,110,110 ,ILI9341_WHITE);
  tft.drawLine(130,110,150,110,ILI9341_WHITE);

  tft.drawLine(0,160,90,160 ,ILI9341_YELLOW);
  tft.drawLine(150,160,240,160 ,ILI9341_YELLOW);

  tft.drawLine(90,130,90,210,ILI9341_YELLOW);
  tft.drawLine(150,130,150,210 ,ILI9341_YELLOW);

  tft.drawLine(90,130,150,130 ,ILI9341_YELLOW);

  tft.drawLine(110,110,110,130,ILI9341_YELLOW);
  tft.drawLine(130,110,130,130,ILI9341_YELLOW);

  tft.drawLine(90,186,150,186 ,ILI9341_YELLOW);

}



void setup() {
  init();
  Serial.begin(9600);

  pinMode(JOY_SEL, INPUT_PULLUP);

  tft.begin();

  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(4);

  travelling();

  tft.setTextSize(1);
  tft.setTextWrap(false);

  cursorX = 120;
  cursorY = 240;

  tft.println("Size: 1");
  tft.println("");
}


void screenlayout() {
  // Pacman starting spot
  tft.fillCircle(cursorX, cursorY, PACMAN_SIZE, ILI9341_YELLOW);
  // Borders of map
  tft.fillRect(0, DISPLAY_WIDTH - 10, DISPLAY_HEIGHT, 4, ILI9341_BLUE);

  // // walls square
  // tft.fillRect(120,120,50,50,ILI9341_BLUE);

  tft.fillRect(0, 10, DISPLAY_HEIGHT, 4, ILI9341_BLUE);
  tft.fillRect(0, 10, 3, 30, ILI9341_BLUE);

  // Squares inside borders



  // Score, lives counters
  tft.setCursor(0,0);
  tft.setTextSize(1);

}



/*

void scoreDots() {

  // dotsstruct dots;
  // make a struct for each dot
  // just make a hard coded list
  int firstcolumndots[100][100];
  for (int i = 0; i < DISPLAY_WIDTH; i+=4) {
    tft.fillCircle(5, i+2, 2, ILI9341_WHITE);
    // dots.xcoord = 5;
    // dots.ycoord = i;
    // dots.passed = false;
    // xcoordinates
    firstcolumndots[i][0] = 5;
    // ycoordinates
    firstcolumndots[i][1] = i;
  }
  // for (int j = 0; j < 100; j++) {
  //   Serial.println(firstcolumndots[j][0]);
  //   Serial.println(firstcolumndots[j][1]);
  //   Serial.println();
  // }
}

*/





void redrawPacman (int newX, int newY, int oldX, int oldY) {
  tft.fillCircle(oldX, oldY, PACMAN_SIZE, ILI9341_BLACK);
  tft.fillCircle(newX, newY, PACMAN_SIZE, ILI9341_YELLOW);
}




/*
void walls(int positionX, int positionY) {
  // concept of wall barrers is starting here, working to not be able to
  // pass from left or right of wall

  if (cursorY >120 && cursorY < 170) {
    if (cursorX > 0 && cursorX < 170) {
    cursorX = constrain(cursorX, 0, 120);
    }
    // else if (cursorX > 180 && cursorX < DISPLAY_WIDTH - PACMAN_SIZE) {
      else {
      cursorX = constrain(cursorX, 180, DISPLAY_WIDTH - PACMAN_SIZE);
    }
  }



  if (cursorY >120 && cursorY < 170) {
    if (cursorX == 50) {
      // this doesn't make it follow the x = 50 line cus you set it to 50 once,
      // it's gonna instantly change with the joystick function
    cursorX = 50;
    }
    // else if (cursorX > 180 && cursorX < DISPLAY_WIDTH - PACMAN_SIZE) {
    //   else {
    //   cursorX = constrain(cursorX, 180, DISPLAY_WIDTH - PACMAN_SIZE);
    // }
  }

}
*/





void movement() {
  int yVal = analogRead(JOY_HORIZ);
  int xVal = analogRead(JOY_VERT);
  int buttonVal = digitalRead(JOY_SEL);
  // copy the cursor position (to check later if it changed)
  int oldX = cursorX;
  int oldY = cursorY;
  if (455 < xVal && 555 > xVal && yVal < 567 && 467 < yVal) {
    return;
  }
  // // For pacman we don't want to have variable speed movement
  if (yVal > 567) {
    ymove = 2;
  }
  else if (yVal < 467) {
    ymove = -2;
  }
  else {
    ymove = 0;
  }
  if (xVal > 555) {
    xmove = 2;
  }
  else if (xVal < 455) {
    xmove = -2;
  }
  else {
    xmove = 0;
  }


  // // BOTTTOM HALF

//HORIZTONAL lines
// line 1
if (cursorX > 60 && cursorX < 180) {
  if (cursorY == 240) {
    cursorY = constrain(cursorY, 240, 240);
    ymove = 0;
    cursorX = constrain(cursorX, 60, 180);
  }

}

if (cursorX >20 && cursorX < 60) {
  if (cursorY == 270) {
    cursorY = constrain(cursorY, 270, 270);
    ymove = 0;
    cursorX = constrain(cursorX, 20, 60);
  }
}
if (cursorX >180 && cursorX < 220) {
  if (cursorY == 270) {
    cursorY = constrain(cursorY, 270, 270);
    ymove = 0;
    cursorX = constrain(cursorX, 180, 270);
  }
}


if (cursorX >90 && cursorX < 110) {
  if (cursorY == 270) {
    cursorY = constrain(cursorY, 270, 270);
    ymove = 0;
    cursorX = constrain(cursorX, 90, 110);
  }
}
if (cursorX >130 && cursorX < 150) {
  if (cursorY == 270) {
    cursorY = constrain(cursorY, 270, 270);
    ymove = 0;
    cursorX = constrain(cursorX, 130, 150);
  }
}
if (cursorX > 20 && cursorX < 300) {
  if (cursorY == 300) {
    cursorY = constrain(cursorY, 300, 300);
    ymove = 0;
    cursorX = constrain(cursorX, 20, 300);
  }
}



if (cursorY == 240) {
  if (cursorX >20 && cursorX < 40) {
    ymove = 0;
    cursorY = constrain(cursorY, 240, 240);
    cursorX = constrain(cursorX, 20, 40);
  } else if (cursorX > 200 && cursorX < 240) {
    ymove = 0;
    cursorY = constrain(cursorY, 240, 240);
    cursorX = constrain(cursorX, 200, 240);
  }
}

if (cursorY == 210) {
  if (cursorX >20 && cursorX < 110) {

    cursorY = constrain(cursorY, 210, 210);
    ymove = 0;
    cursorX = constrain(cursorX, 20, 110);
  } else if (cursorX > 130 && cursorX < 220) {

    cursorY = constrain(cursorY, 210, 210);
    ymove = 0;
    cursorX = constrain(cursorX, 130, 220);
  }
}

if (cursorY == 186) {
  if (cursorX > 90 && cursorX < 150) {
    cursorY = constrain(cursorY, 186, 186);
    ymove = 0;
    cursorX = constrain(cursorX, 90, 150);
  }
}

if (cursorY == 160) {
  if (cursorX > 0 && cursorX < 90) {
    cursorY = constrain(cursorY, 160, 160);
    ymove = 0;
    cursorX = constrain(cursorX, 0, 90);
  } else if (cursorX > 150 && cursorX < 240) {
    cursorY = constrain(cursorY, 160, 160);
    ymove = 0;
    cursorX = constrain(cursorX, 150, 240);
  }
}
























//VERTICAL lines
// line 1
if (cursorY < 270 && cursorY > 30) {
  if (cursorX == 60) {
    cursorX = constrain(cursorX, 60, 60);
    xmove = 0;
    cursorY = constrain(cursorY, 30, 270);
  } else if (cursorX == 180) {
    cursorX = constrain(cursorX, 180, 180);
    xmove = 0;
    cursorY = constrain(cursorY, 30, 270);
  }
}

//line 3
if (cursorY < 300 && cursorY > 270) {
  if (cursorX == 20) {
    cursorX = constrain(cursorX, 20, 20);
    xmove = 0;
    cursorY = constrain(cursorY, 270, 300);
  } else if (cursorX == 220) {
    cursorX = constrain(cursorX, 220, 220);
    xmove = 0;
    cursorY = constrain(cursorY, 270, 300);
  }
}
//line 4
if (cursorY < 270 && cursorY > 240) {
  if (cursorX == 90) {
    cursorX = constrain(cursorX, 90, 90);
    xmove = 0;
    cursorY = constrain(cursorY, 240, 270);
  } else if (cursorX == 150) {
    cursorX = constrain(cursorX, 150, 150);
    xmove = 0;
    cursorY = constrain(cursorY, 240, 270);
  }
}


if (cursorY < 300 && cursorY > 270) {
  if (cursorX == 110) {
    cursorX = constrain(cursorX, 110, 110);
    xmove = 0;
    cursorY = constrain(cursorY, 270, 300);
  } else if (cursorX == 130) {
    cursorX = constrain(cursorX, 130, 130);
    xmove = 0;
    cursorY = constrain(cursorY, 270, 300);
  }
}
if (cursorY < 270 && cursorY > 240) {
  if (cursorX == 40) {
    cursorX = constrain(cursorX, 40, 40);
    xmove = 0;
    cursorY = constrain(cursorY, 240, 270);
  } else if (cursorX == 200) {
    cursorX = constrain(cursorX, 200, 200);
    xmove = 0;
    cursorY = constrain(cursorY, 240, 270);
  }
}

if (cursorY < 240 && cursorY > 210) {
  if (cursorX == 20) {
    cursorX = constrain(cursorX, 20, 20);
    xmove = 0;
    cursorY = constrain(cursorY, 210, 240);
  } else if (cursorX == 220) {
    cursorX = constrain(cursorX, 220, 220);
    xmove = 0;
    cursorY = constrain(cursorY, 210, 240);
  } else if (cursorX == 110) {
    cursorX = constrain(cursorX, 110, 110);
    xmove = 0;
    cursorY = constrain(cursorY, 210, 240);
  } else if (cursorX == 130) {
    cursorX = constrain(cursorX, 130, 130);
    xmove = 0;
    cursorY = constrain(cursorY, 210, 240);
  }
}

if (cursorY < 210 && cursorY > 186) {
  if (cursorX == 90) {
    xmove = 0;
    cursorX = constrain(cursorX, 90, 90);
    cursorY = constrain(cursorY, 186, 210);
  } else if (cursorX == 150) {
    xmove = 0;
    cursorX = constrain(cursorX, 150, 150);
    cursorY = constrain(cursorY, 186, 210);
  }
}

if (cursorY < 186 && cursorY > 160) {
  if (cursorX == 90) {
    xmove = 0;
    cursorX = constrain(cursorX, 90, 90);
    cursorY = constrain(cursorY, 160, 186);
  } else if (cursorX == 150) {
    xmove = 0;
    cursorX = constrain(cursorX, 150, 150);
    cursorY = constrain(cursorY, 160, 186);
  }
}

if (cursorY < 160 && cursorY > 130) {
  if (cursorX == 90) {
    xmove = 0;
    cursorX = constrain(cursorX, 90, 90);
    cursorY = constrain(cursorY, 130, 160);
  } else if (cursorX == 150) {
    xmove = 0;
    cursorX = constrain(cursorX, 150, 150);
    cursorY = constrain(cursorY, 130, 160);
  }
}
























//INTERSECTIONS
// inter 1 ***************
  if (cursorX == 60 && cursorY == 240) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 240, 240);
      xmove = 2;
      ymove =0;
    }
    if (yVal > 567 || yVal < 467) {
      cursorX = constrain(cursorX, 60, 60);
      xmove = 0;
    }
    if (xVal < 455) {
      xmove = 0;
    }
  }
  // inter 2
    if (cursorX == 180 && cursorY == 240) {
      if (xVal > 555) {
        xmove = 0;
      }
      if (yVal > 567 || yVal < 467) {
        cursorX = constrain(cursorX, 180, 180);
        xmove = 0;
      }
      if (xVal < 455) {
        xmove = -2;
        ymove = 0;
      }
    }
  // inter 3
    if (cursorX == 60 && cursorY == 270) {
      if (xVal > 555) {
        cursorY = constrain(cursorY, 270, 270);

        xmove = 0;
      }
      if (yVal > 567) {
        ymove = 0;
      }
      if (xVal < 455) {
        cursorY = constrain(cursorY, 270, 270);
        xmove = -2;
        ymove = 0;
      }
    }
  // inter 4
    if (cursorX == 180 && cursorY == 270) {
      if (xVal > 555) {
        cursorY = constrain(cursorY, 270, 270);
        xmove = 2;
        ymove = 0;
      }
      if (yVal > 567) {
        ymove = 0;
      }
      if (xVal < 455) {
        cursorY = constrain(cursorY, 270, 270);
        xmove = 0;
      }
    }
  // inter 5
    if (cursorX == 20 && cursorY == 270) {
      if (xVal > 555) {
        cursorY = constrain(cursorY, 270, 270);
        xmove = 2;
        ymove = 0;
      }
      if (yVal > 567) {
        ymove = 2;
      } else if (yVal < 467) {
        ymove = 0;
      }
      if (xVal < 455) {
        cursorY = constrain(cursorY, 270, 270);
        xmove = 0;
      }
    }
  if (cursorX == 220 && cursorY == 270) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 270, 270);
      xmove = 0;
    }
    if (yVal > 567) {
      ymove = 2;
    } else if (yVal < 467) {
      ymove = 0;
    }
    if (xVal < 455) {
      cursorY = constrain(cursorY, 270, 270);
      xmove = -2;
      ymove = 0;
    }
  }
  // inter 6
    if (cursorX == 20 && cursorY == 300) {
      if (xVal > 555) {
        cursorY = constrain(cursorY, 300, 300);
        xmove = 2;
        ymove = 0;
      }
      if (yVal > 567) {
        ymove = 0;
      } else if (yVal < 467) {
        ymove = -2;
      }
      if (xVal < 455) {
        xmove=0;
      }
    }
  // inter 6
    if (cursorX == 220 && cursorY == 300) {
      if (xVal > 555) {
        xmove = 0;
      }
      if (yVal > 567) {
        ymove = 0;
      } else if (yVal < 467) {
        ymove = -2;
      }
      if (xVal < 455) {
        cursorY = constrain(cursorY, 300, 300);
        xmove = -2;
        ymove = 0;
      }
    }
// inter 6 NOT WORKING
      if (cursorX == 90 && cursorY == 240) {
        if (xVal > 555) {
          cursorY = constrain(cursorY, 240,240);
          ymove = 0;
          xmove = 2;
        } else if (xVal < 455) {
          cursorY = constrain(cursorY, 240,240);
          ymove = 0;
          xmove = -2;
        }
        if (yVal > 567) {
          cursorX = constrain(cursorX, 90, 90);
          xmove = 0;
          ymove = 2;
      }
  }
  else if (cursorX == 150 && cursorY == 240) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 240,240);
      ymove = 0;
      xmove = 2;
    } else if (xVal < 455) {
      cursorY = constrain(cursorY, 240,240);
      ymove = 0;
      xmove = -2;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 150, 150);
      xmove = 0;
      ymove = 2;
  }
}

if (cursorX == 90 && cursorY == 270) {
  if (xVal > 555) {
    cursorY = constrain(cursorY, 270,270);
    ymove = 0;
    xmove = 2;
  } else if (xVal < 455) {
    ymove =0;
    xmove = 0;
  }
  if (yVal > 567) {
    ymove = 0;
    xmove =0;
  } else if (yVal < 467) {
    ymove = -2;
    xmove = 0;
  }
}

if (cursorX == 150 && cursorY == 270) {
  if (xVal < 455) {
    cursorY = constrain(cursorY, 270,270);
    ymove = 0;
    xmove = -2;
  } else if (xVal > 555) {
    ymove =0;
    xmove = 0;
  }
  if (yVal > 567) {
    ymove = 0;
    xmove =0;
  } else if (yVal < 467) {
    ymove = -2;
    xmove = 0;
  }
}


if (cursorX == 110 && cursorY == 270) {
  if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  } else if (xVal > 555) {
    xmove = 0;
    ymove = 0;
  }
  if (yVal > 567) {
    cursorX = constrain(cursorX, 110, 110);
    xmove = 0;
    ymove = 2;
  } else if (yVal < 467) {
    ymove = 0;
  }
}

if (cursorX == 130 && cursorY == 270) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = 0;
    ymove = 0;
  }
  if (yVal > 567) {
    cursorX = constrain(cursorX, 130, 130);
    xmove = 0;
    ymove = 2;
  } else if (yVal < 467) {
    ymove = 0;
  }
}



if (cursorX == 110 && cursorY == 300) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  }
  if (yVal < 467) {
    cursorX = constrain(cursorX, 110, 110);
    xmove = 0;
    ymove  = -2;
  } else if (yVal > 567) {
    ymove = 0;
  }
}
if (cursorX == 130 && cursorY == 300) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  }
  if (yVal < 467) {
    cursorX = constrain(cursorX, 130, 130);
    xmove = 0;
    ymove  = -2;
  } else if (yVal > 567) {
    ymove = 0;
  }
}


if (cursorX == 40 && cursorY == 270) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  }
  if (yVal < 467) {
    cursorX = constrain(cursorX, 40, 40);
    xmove = 0;
    ymove  = -2;
  } else if (yVal > 567) {
    ymove = 0;
  }
}
if (cursorX == 200 && cursorY == 270) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  }
  if (yVal < 467) {
    cursorX = constrain(cursorX, 200, 200);
    xmove = 0;
    ymove  = -2;
  } else if (yVal > 567) {
    ymove = 0;
  }
}

if (cursorX == 40 && cursorY == 240) {
  if (xVal > 555) {
    xmove = 0;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  }
  if (yVal < 467) {
    ymove=0;
  } else if (yVal > 567) {
    cursorX = constrain(cursorX, 40, 40);
    xmove = 0;
    ymove  = 2;
  }
}
if (cursorX == 200 && cursorY == 240) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = 0;
    ymove = 0;
  }
  if (yVal < 467) {
    ymove=0;
  } else if (yVal > 567) {
    cursorX = constrain(cursorX, 200, 200);
    xmove = 0;
    ymove  = 2;
  }
}


if (cursorX == 20 && cursorY == 240) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = 0;
    ymove = 0;
  }
  if (yVal < 467) {
    cursorX = constrain(cursorX, 20, 20);
    ymove = -2;
    xmove = 0;
  } else if (yVal > 567) {
    ymove = 0;
  }
}
if (cursorX == 220 && cursorY == 240) {
  if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  } else if (xVal > 555) {
    xmove = 0;
    ymove = 0;
  }
  if (yVal < 467) {
    cursorX = constrain(cursorX, 220, 220);
    ymove = -2;
    xmove = 0;
  } else if (yVal > 567) {
    ymove = 0;
  }
}


if (cursorX == 20 && cursorY == 210) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = 0;
    ymove = 0;
  }
  if (yVal > 567) {
    cursorX = constrain(cursorX, 20, 20);
    ymove = 2;
    xmove = 0;
  } else if (yVal < 467) {
    ymove = 0;
  }
}
if (cursorX == 220 && cursorY == 210) {
  if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  } else if (xVal > 555) {
    xmove = 0;
    ymove = 0;
  }
  if (yVal > 567) {
    cursorX = constrain(cursorX, 220, 220);
    ymove = 2;
    xmove = 0;
  } else if (yVal < 467) {
    ymove = 0;
  }
}


if (cursorX == 110 && cursorY == 240) {
  if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  } else if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  }
  if (yVal < 467) {
    cursorX = constrain(cursorX, 110, 110);
    ymove = -2;
    xmove = 0;
  } else if (yVal > 567) {
    ymove = 0;
  }
}

if (cursorX == 130 && cursorY == 240) {
  if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  } else if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  }
  if (yVal < 467) {
    cursorX = constrain(cursorX, 130, 130);
    ymove = -2;
    xmove = 0;
  } else if (yVal > 567) {
    ymove = 0;
  }
}

if (cursorX == 110 && cursorY == 210) {
  if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  } else if (xVal > 555) {
    xmove = 0;
    ymove = 0;
  }
  if (yVal > 567) {
    cursorX = constrain(cursorX, 110, 110);
    ymove = 2;
    xmove = 0;
  } else if (yVal < 467) {
    ymove = 0;
  }
}
if (cursorX == 130 && cursorY == 210) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = 0;
    ymove = 0;
  }
  if (yVal > 567) {
    cursorX = constrain(cursorX, 130, 130);
    ymove = 2;
    xmove = 0;
  } else if (yVal < 467) {
    ymove = 0;
  }
}



// FOR SOME REASON AT THESE INTERSECTIONS IT HAS A TOUGH TIME GOING LEFT AND RIGHT, BUT IT DOES WORK IF YOU WIGGLE THE STICK
if (cursorX == 60 && cursorY == 210) {
  if (xVal > 555) {
    cursorY = constrain(cursorY, 210, 210);
    xmove = 2;
    ymove =0;
  }
  if (xVal < 455) {
    cursorY = constrain(cursorY, 210, 210);
    xmove = -2;
    ymove =0;
  }
  if (yVal > 567) {
    ymove = 2;
    xmove = 0;
    cursorX = constrain(cursorX, 60, 60);
  }
  if (yVal < 467) {
    ymove = -2;
    xmove = 0;
    cursorX = constrain(cursorX, 60, 60);
  }
}
if (cursorX == 180 && cursorY == 210) {
  if (xVal > 555) {
    cursorY = constrain(cursorY, 210, 210);
    xmove = 2;
    ymove =0;
  }
  if (xVal < 455) {
    cursorY = constrain(cursorY, 210, 210);
    xmove = -2;
    ymove =0;
  }
  if (yVal > 567) {
    ymove = 2;
    xmove = 0;
    cursorX = constrain(cursorX, 180, 180);
  }
  if (yVal < 467) {
    ymove = -2;
    xmove = 0;
    cursorX = constrain(cursorX, 180, 180);
  }
}



if (cursorX == 90 && cursorY == 210) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  }
  if (yVal > 567) {
    ymove = 0;
    xmove = 0;
  } else if (yVal < 467) {
    cursorX = constrain(cursorX, 90, 90);
    xmove = 0;
    ymove = -2;
  }
}

if (cursorX == 150 && cursorY == 210) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  }
  if (yVal > 567) {
    xmove = 0;
  } else if (yVal < 467) {
    cursorX = constrain(cursorX, 150, 150);
    xmove = 0;
    ymove = -2;
  }
}



if (cursorX == 90 && cursorY == 186) {
  if (xVal > 555) {
    cursorY = constrain(cursorY, 186, 186);
    xmove = 2;
    ymove = 0;
  }
  if (xVal < 455) {
    xmove = 0;
  }
  if (yVal > 567 || yVal < 467) {
    cursorX = constrain(cursorX, 90, 90);
    xmove = 0;
  }
}
if (cursorX == 150 && cursorY == 186) {
  if (xVal < 455) {
    cursorY = constrain(cursorY, 186, 186);
    xmove = -2;
    ymove = 0;
  } else if (xVal > 555) {
    xmove = 0;
  }
  if (yVal > 567 || yVal < 467) {
    cursorX = constrain(cursorX, 150, 150);
    xmove = 0;
  }
}

if (cursorX == 90 && cursorY == 160) {
  if (xVal < 455) {
    cursorY = constrain(cursorY, 160, 160);
    xmove = -2;
    ymove = 0;
  } else if (xVal > 555) {
    xmove = 0;
  }
  if (yVal > 567 || yVal < 467) {
    cursorX = constrain(cursorX, 90, 90);
    xmove = 0;
  }
}

if (cursorX == 150 && cursorY == 160) {
  if (xVal > 555) {
    cursorY = constrain(cursorY, 160, 160);
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = 0;
  }
  if (yVal > 567 || yVal < 467) {
    cursorX = constrain(cursorX, 150, 150);
    xmove = 0;
  }
}

if (cursorX == 60 && cursorY == 160) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  }
  if (yVal > 567) {
    cursorX = constrain(cursorX, 60, 60);
    ymove = 2;
    xmove = 0;
  } else if (yVal < 467) {
    cursorX = constrain(cursorX, 60, 60);
    xmove = 0;
    ymove = -2;
  }
}
if (cursorX == 180 && cursorY == 160) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  }
  if (yVal > 567) {
    cursorX = constrain(cursorX, 180, 180);
    ymove = 2;
    xmove = 0;
  } else if (yVal < 467) {
    cursorX = constrain(cursorX, 180, 180);
    xmove = 0;
    ymove = -2;
  }
}

if (cursorX == 0 && cursorY == 160) {
  if (xVal > 555) {
    xmove = 2;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = 0;
    cursorX = 240;
    ymove = 0;
  }
  if (yVal > 567) {
    ymove = 0;
    xmove = 0;
  } else if (yVal < 467) {
    xmove = 0;
    ymove = 0;
  }
}
if (cursorX == 240 && cursorY == 160) {
  if (xVal > 555) {
    cursorX = 0;
    xmove = 0;
    ymove = 0;
  } else if (xVal < 455) {
    xmove = -2;
    ymove = 0;
  }
  if (yVal > 567) {
    ymove = 0;
    xmove = 0;
  } else if (yVal < 467) {
    xmove = 0;
    ymove = 0;
  }
}








  cursorX += xmove;
  cursorY += ymove;
  xmove = 0;
  ymove = 0;


  // constrain so the cursor does not go off of the map display window
  // the ending "-1"s make it perfectly not go off the edges
  // cursorX = constrain(cursorX, PACMAN_SIZE, DISPLAY_HEIGHT - PACMAN_SIZE - 1);
  // cursorY = constrain(cursorY, PACMAN_SIZE, DISPLAY_WIDTH - PACMAN_SIZE - 1);
// // concept of wall barrers is starting here, working to not be able to
// // pass from left or right of wall
// if (cursorY >120 && cursorY < 170) {
//   if (cursorX > 0 && cursorX < 170) {
//   cursorX = constrain(cursorX, 0, 120);
//   }
//   // else if (cursorX > 180 && cursorX < DISPLAY_WIDTH - PACMAN_SIZE) {
//     else {
//     cursorX = constrain(cursorX, 180, DISPLAY_WIDTH - PACMAN_SIZE);
//   }
// }
  // walls(cursorX, cursorY);
  // redraw the cursor only if its position actually changed
  if (cursorX != oldX || cursorY != oldY) {
    redrawPacman(cursorX, cursorY, oldX, oldY);
  }
  delay(10);





}








int main() {
  setup();
  screenlayout();
  // scoreDots();
  while (true) {
    movement();

  }
  Serial.end();
  return 0;
}
