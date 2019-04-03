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
#include <string.h>

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


bool sendScore = false;
bool comDone = false;

int cursorX, cursorY;
int xmove, ymove;
int screenmidX = DISPLAY_WIDTH/2;
int screenmidY = DISPLAY_HEIGHT/2;

unsigned long score;
#define PAC_SPEED 5
#define GHOST_SPEED 1
#define TOUCH_SIZE 7

#define START_RED_X 160
#define START_RED_Y 200
#define START_PINK_X 120
#define START_PINK_Y 200
#define START_CYAN_X 100
#define START_CYAN_Y 70
#define START_ORANGE_X 160
#define START_ORANGE_Y 100

// make reset positions for the ghosts for each corner but realisticly red doesnt need to go anywhere
#define CORNER_1_X 20
#define CORNER_1_Y 30
#define CORNER_2_X 220
#define CORNER_2_Y 30
#define CORNER_3_X 20
#define CORNER_3_Y 300
#define CORNER_4_X 220
#define CORNER_4_Y 300

int rCursorX, rCursorY, pCursorX, pCursorY, cCursorX, cCursorY, oCursorX, oCursorY;
int rXMove,rYMove, pXMove, pYMove, cXMove, cYMove, oXMove, oYMove;

bool ghost = true;

#define MAX_SCORES 100
struct Rankings{
  String names;
  int points;
};
Rankings playerRecord[MAX_SCORES];

int numberOfScores = 100;
int numCounter = 0;
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
  Serial.flush(); // get rid of any leftover bits
  pinMode(JOY_SEL, INPUT_PULLUP);

  tft.begin();

}

void menu(){
  tft.fillScreen(ILI9341_BLACK);// draw the screen all black first
  tft.setCursor(10,80);
  tft.setTextSize(4);
  tft.setTextColor(ILI9341_BLACK, ILI9341_YELLOW);
  tft.println(" PAC-MAN ");

  tft.fillRect(62-2, 200-1, 19, 18, ILI9341_RED);
  tft.fillRect(62-1, 200-2, 17, 14, ILI9341_RED);
  tft.fillRect(62 + 1, 200 - 1, 6, 7, ILI9341_WHITE);
  tft.fillRect(62 + 7, 200 - 1, 6, 7, ILI9341_WHITE);
  tft.fillRect(62 + 3, 200, 4, 4, ILI9341_BLACK);
  tft.fillRect(62 + 9, 200, 4, 4, ILI9341_BLACK);

  tft.fillRect(92-2, 200-1, 19, 18, ILI9341_CYAN);
  tft.fillRect(92-1, 200-2, 17, 14, ILI9341_CYAN);
  tft.fillRect(92 + 1, 200 - 1, 6, 7, ILI9341_WHITE);
  tft.fillRect(92 + 7, 200 - 1, 6, 7, ILI9341_WHITE);
  tft.fillRect(92 + 3, 200, 4, 4, ILI9341_BLACK);
  tft.fillRect(92 + 9, 200, 4, 4, ILI9341_BLACK);

  tft.fillRect(122-2, 200-1, 19, 18, ILI9341_MAGENTA);
  tft.fillRect(122-1, 200-2, 17, 14, ILI9341_MAGENTA);
  tft.fillRect(122 + 1, 200 - 1, 6, 7, ILI9341_WHITE);
  tft.fillRect(122 + 7, 200 - 1, 6, 7, ILI9341_WHITE);
  tft.fillRect(122 + 3, 200, 4, 4, ILI9341_BLACK);
  tft.fillRect(122 + 9, 200, 4, 4, ILI9341_BLACK);

  tft.fillRect(152-2, 200-1, 19, 18, ILI9341_ORANGE);
  tft.fillRect(152-1, 200-2, 17, 14, ILI9341_ORANGE);
  tft.fillRect(152 + 1, 200 - 1, 6, 7, ILI9341_WHITE);
  tft.fillRect(152 + 7, 200 - 1, 6, 7, ILI9341_WHITE);
  tft.fillRect(152 + 3, 200, 4, 4, ILI9341_BLACK);
  tft.fillRect(152 + 9, 200, 4, 4, ILI9341_BLACK);

  tft.fillCircle(192, 207, 9, ILI9341_YELLOW);

  tft.setCursor(40,280);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.println(" PRESS JOYSTICK TO START ! ");

  Serial.println(" almost at while");
  while (true){
    //processJoystick();
    int checkButton = digitalRead(JOY_SEL);
    //Serial.println(checkButton);
    if (checkButton == LOW){
      Serial.println("button pressed");
      break;
    }
  }
}


void screenlayout() {
  tft.fillScreen(ILI9341_BLACK);
  // Pacman starting spot
  cursorX = 120;
  cursorY = 240;
  tft.fillCircle(cursorX, cursorY, PACMAN_SIZE, ILI9341_YELLOW);
  // Borders of map
  tft.fillRect(0, DISPLAY_WIDTH - 10, DISPLAY_HEIGHT, 4, ILI9341_BLUE);

  // // walls square
  // tft.fillRect(120,120,50,50,ILI9341_BLUE);

  tft.fillRect(0, 10, DISPLAY_HEIGHT, 4, ILI9341_BLUE);
  tft.fillRect(0, 10, 3, 30, ILI9341_BLUE);

  // Squares inside borders

  travelling();

  tft.setTextSize(1);
  tft.setTextWrap(false);

  if (ghost == true){
    // draw the starting positions of the ghosts
    rCursorX = START_RED_X;
    rCursorY = START_RED_Y;

    pCursorX = START_PINK_X;
    pCursorY = START_PINK_Y;

    cCursorX = START_CYAN_X;
    cCursorY = START_CYAN_Y;

    oCursorX = START_ORANGE_X;
    oCursorY = START_ORANGE_Y;



    //tft.fillCircle(cursorX, cursorY, PACMAN_SIZE, ILI9341_YELLOW);
    //tft.fillRect(0, DISPLAY_WIDTH - 10, DISPLAY_HEIGHT, 5, ILI9341_BLUE);
    //tft.fillRect(120,120,50,50,ILI9341_BLUE);

    // draw red ghost
    tft.fillCircle(rCursorX, rCursorY, GHOST_SIZE, ILI9341_RED);

    // // if we want to make them look like ghosts leave for later
    // tft.fillRect(rCursorX-2, rCursorY-1, 8, 7, ILI9341_RED);
    // tft.fillRect(rCursorX-1, rCursorY-2, 6, 3, ILI9341_RED);

    // draw pink ghost
    tft.fillCircle(pCursorX, pCursorY, GHOST_SIZE, ILI9341_MAGENTA);

    tft.fillCircle(cCursorX, cCursorY, GHOST_SIZE, ILI9341_CYAN);

    tft.fillCircle(oCursorX, oCursorY, GHOST_SIZE, ILI9341_ORANGE);
  }

  // Score, lives counters
  tft.setCursor(0,0);
  tft.setTextSize(1);

  tft.println("Size: 1");
  tft.println("");
}

void inputs(char str[], int len) {
	// user defined function that takes in player name
  Serial.println("Press the Enter key after inputing name:");
    int index = 0;
    while (index < len - 1) {
        // if something is waiting to be read on Serial0
        if (Serial.available() > 0) {
            char name = Serial.read();
            // did the user press enter?
            if (name == '\r') {
                break;
            } else {
                Serial.print(name);
                str[index] = name;
                index += 1;
            }
        }
    }
    str[index] = '\0';
}

String name() {
  char str[32];
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(4);
  tft.setCursor(10,80);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
  tft.println(" TYPE YOUR NAME ");
	// function for startup that prompts user to press enter for their name
	inputs(str, 32);
  Serial.println();
  Serial.print("Your Name is: ");
  String username = String(str);
  Serial.print(username);
  Serial.println();

  return username;
}

String endGame(int points){
  // output a different screen if you won
  tft.fillScreen(ILI9341_BLACK);// draw the screen all black first
  String value = String(points);
  tft.setCursor(10,80);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
  tft.print("you scored: ");
  tft.println(value);
  return value;
}

void clientCommunication(String score){
  // if (numCounter == numberOfScores){
  //   comDone = true;
  // }
  if ((sendScore == false) && (comDone == false)) {
    //Serial.flush();
    Serial.print("Score");
    Serial.print(" ");
    Serial.println(score);
    Serial.flush();
    sendScore = true;
  }
  else if((sendScore == true)&& (comDone == false)){
    // charecter array that will store the inputs
    char line[500];
    int words = 0;
    char *split;
    while (true) {
      while (Serial.available() == 0);
        // read in words from serial.read
        line[words] = Serial.read();
        words++;
        // if newline is found
        if (line[words-1] == '\n') {
            // process buffer
            line[words - 1] = '\0';
            words = 0;
            break;
        }
    }
    // split strings into tokens using space as delimeter
    split = strtok(line, " ");
    // if case if first value read in is a N
    if (split[0] == 'N') {
      split = strtok(NULL, " ");
      // convert the char into a string
      String strNumber = String(split);
      // convert the string to an integer
      numberOfScores = strNumber.toInt();
      // send an A as acknowledgement
      Serial.flush();
      Serial.print("A\n");
      // sets the maximum milliseconds to wait for serial data for 1 second
      Serial.setTimeout(1000);
      Serial.flush();
    }
    else if (split[0] == 'W'){
      split = strtok(NULL," ");
      // convert the latitude into int32_t
      String pNames = String(split);
      split = strtok(NULL," ");
      // convert the longintude into int32_t
      String strVals = String(split);
      int pScore = strVals.toInt();
      // store into shared variable along with a counter that stores the lon lat sent over from server
      playerRecord[numCounter].names = pNames;
      playerRecord[numCounter].points = pScore;
      Serial.flush();
      // send acknowledgement after you recieve lon lat
      Serial.print("A\n");
      // sets the maximum milliseconds to wait for serial data for 1 second for new point
      Serial.setTimeout(1000);
      Serial.flush();
      numCounter++;
    }
    else if (split[0] == 'E'){
      comDone = true;
    }
  }
}


void redrawPacman (int newX, int newY, int oldX, int oldY) {
  tft.fillCircle(oldX, oldY, PACMAN_SIZE, ILI9341_BLACK);
  tft.fillCircle(newX, newY, PACMAN_SIZE, ILI9341_YELLOW);
}

void redrawRedGhost(int newX, int newY, int oldX, int oldY) {
  tft.fillCircle(oldX, oldY, GHOST_SIZE, ILI9341_BLACK);
  tft.fillCircle(newX, newY, GHOST_SIZE, ILI9341_RED);
}

void redrawPinkGhost(int newX, int newY, int oldX, int oldY) {
  tft.fillCircle(oldX, oldY, GHOST_SIZE, ILI9341_BLACK);
  tft.fillCircle(newX, newY, GHOST_SIZE, ILI9341_MAGENTA);
}

void redrawCyanGhost(int newX, int newY, int oldX, int oldY) {
  tft.fillCircle(oldX, oldY, GHOST_SIZE, ILI9341_BLACK);
  tft.fillCircle(newX, newY, GHOST_SIZE, ILI9341_CYAN);
}

void redrawOrangeGhost(int newX, int newY, int oldX, int oldY) {
  tft.fillCircle(oldX, oldY, GHOST_SIZE, ILI9341_BLACK);
  tft.fillCircle(newX, newY, GHOST_SIZE, ILI9341_ORANGE);
}

void redGhostMove(int pacX, int pacY, int ghostX, int ghostY){
  // move ghost to the left if it is to the right
  if ((pacX - ghostX) < 0){
    rXMove = -GHOST_SPEED;
  }
  // move ghost to the right if it is to the left
  else if ((pacX - ghostX) > 0){
    rXMove = GHOST_SPEED;
  }
  if ((pacY - ghostY) < 0){
    rYMove = -GHOST_SPEED;
  }
  else if ((pacY - ghostY) > 0){
    rYMove = GHOST_SPEED;
  }
  rCursorX += rXMove;
  rCursorY += rYMove;
}

void pinkGhostMove(int pacX, int pacY, int ghostX, int ghostY){
  // move ghost to the left if it is to the right
  if ((pacX - ghostX) < 0){
    pXMove = -GHOST_SPEED;
  }
  // move ghost to the right if it is to the left
  else if ((pacX - ghostX) > 0){
    pXMove = GHOST_SPEED;
  }
  if ((pacY - ghostY) < 0){
    pYMove = -GHOST_SPEED;
  }
  else if ((pacY - ghostY) > 0){
    pYMove = GHOST_SPEED;
  }
  pCursorX += pXMove;
  pCursorY += pYMove;
}

void cyanGhostMove(int pacX, int pacY, int ghostX, int ghostY){
  // move ghost to the left if it is to the right
  if ((pacX - ghostX) < 0){
    cXMove = -GHOST_SPEED;
  }
  // move ghost to the right if it is to the left
  else if ((pacX - ghostX) > 0){
    cXMove = GHOST_SPEED;
  }
  if ((pacY - ghostY) < 0){
    cYMove = -GHOST_SPEED;
  }
  else if ((pacY - ghostY) > 0){
    cYMove = GHOST_SPEED;
  }
  cCursorX += cXMove;
  cCursorY += cYMove;
}

void orangeGhostMove(int pacX, int pacY, int ghostX, int ghostY){
  // move ghost to the left if it is to the right
  if ((pacX - ghostX) < 0){
    oXMove = -GHOST_SPEED;
  }
  // move ghost to the right if it is to the left
  else if ((pacX - ghostX) > 0){
    oXMove = GHOST_SPEED;
  }
  if ((pacY - ghostY) < 0){
    oYMove = -GHOST_SPEED;
  }
  else if ((pacY - ghostY) > 0){
    oYMove = GHOST_SPEED;
  }
  oCursorX += oXMove;
  oCursorY += oYMove;
}

void ghostMovements(){
  int oldRedX = rCursorX;
  int oldRedY = rCursorY;

  int oldPinkX = pCursorX;
  int oldPinkY = pCursorY;

  int oldCyanX = cCursorX;
  int oldCyanY = cCursorY;

  int oldOrangeX = oCursorX;
  int oldOrangeY = oCursorY;

  // maybe have ghosts move at all times when we get paths done
  redGhostMove(cursorX,cursorY,rCursorX,rCursorY);
  redrawRedGhost(rCursorX,rCursorY, oldRedX,oldRedY);

  pinkGhostMove(cursorX,cursorY,pCursorX,pCursorY);
  redrawPinkGhost(pCursorX,pCursorY, oldPinkX,oldPinkY);

  cyanGhostMove(cursorX,cursorY,cCursorX,cCursorY);
  redrawCyanGhost(cCursorX,cCursorY, oldCyanX,oldCyanY);

  orangeGhostMove(cursorX,cursorY,oCursorX,oCursorY);
  redrawOrangeGhost(oCursorX,oCursorY, oldOrangeX,oldOrangeY);

  // if red overlaps pink
  if ((abs(rCursorX - pCursorX) < TOUCH_SIZE) && (abs(rCursorY - pCursorY) < TOUCH_SIZE)){
    // need this since without recovering up
    tft.fillCircle(pCursorX,pCursorY, GHOST_SIZE, ILI9341_BLACK);
    pCursorX = CORNER_1_X;
    pCursorY = CORNER_1_Y;
  }
  // if red overlaps Cyan
  if ((abs(rCursorX - cCursorX) < TOUCH_SIZE) && (abs(rCursorY - cCursorY) < TOUCH_SIZE)){
    // need this since without recovering up
    tft.fillCircle(cCursorX,cCursorY, GHOST_SIZE, ILI9341_BLACK);
    cCursorX = CORNER_2_X;
    cCursorY = CORNER_2_Y;
  }
  // if red overlaps orange
  if ((abs(rCursorX - oCursorX) < TOUCH_SIZE) && (abs(rCursorY - oCursorY) < TOUCH_SIZE)){
    // need this since without recovering up
    tft.fillCircle(oCursorX,oCursorY, GHOST_SIZE, ILI9341_BLACK);
    oCursorX = CORNER_3_X;
    oCursorY = CORNER_3_Y;
  }
  // if pink ovelaps cyan
  if ((abs(pCursorX - cCursorX) < TOUCH_SIZE) && (abs(pCursorY - cCursorY) < TOUCH_SIZE)){
    // need this since without recovering up
    tft.fillCircle(cCursorX,cCursorY, GHOST_SIZE, ILI9341_BLACK);
    cCursorX = CORNER_2_X;
    cCursorY = CORNER_2_Y;
  }
  // if pink overlaps orange
  if ((abs(pCursorX - oCursorX) < TOUCH_SIZE) && (abs(pCursorY - oCursorY) < TOUCH_SIZE)){
    // need this since without recovering up
    tft.fillCircle(oCursorX,oCursorY, GHOST_SIZE, ILI9341_BLACK);
    oCursorX = CORNER_3_X;
    oCursorY = CORNER_3_Y;
  }
  // if cyan overlaps orange
  if ((abs(cCursorX - oCursorX) < TOUCH_SIZE) && (abs(cCursorY - oCursorY) < TOUCH_SIZE)){
    // need this since without recovering up
    tft.fillCircle(oCursorX,oCursorY, GHOST_SIZE, ILI9341_BLACK);
    oCursorX = CORNER_3_X;
    oCursorY = CORNER_3_Y;
  }

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
    ymove = PAC_SPEED;
  }
  else if (yVal < 467) {
    ymove = -PAC_SPEED;
  }
  else {
    ymove = 0;
  }
  if (xVal > 555) {
    xmove = PAC_SPEED;
  }
  else if (xVal < 455) {
    xmove = -PAC_SPEED;
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

  if (cursorY == 130) {
    if (cursorX > 90 && cursorX < 150) {
      cursorY = constrain(cursorY, 130, 130);
      ymove = 0;
      cursorX = constrain(cursorX, 90, 150);
    }
  }

  if (cursorY == 110) {
    if (cursorX > 90 && cursorX < 110) {
      cursorY = constrain(cursorY, 110, 110);
      ymove = 0;
      cursorX = constrain(cursorX, 90, 110);
    } else if (cursorX > 130 && cursorX < 150) {
      cursorY = constrain(cursorY, 110, 110);
      ymove = 0;
      cursorX = constrain(cursorX, 130, 150);
    } else if (cursorX > 20 && cursorX < 60) {
      cursorY = constrain(cursorY, 110, 110);
      ymove = 0;
      cursorX = constrain(cursorX, 20, 60);
    } else if (cursorX > 180 && cursorX < 220) {
      cursorY = constrain(cursorY, 110, 110);
      ymove = 0;
      cursorX = constrain(cursorX, 180, 220);
    }
  }

  if (cursorY == 70) {
    if (cursorX > 20 && cursorX < 220) {
      cursorY = constrain(cursorY, 70, 70);
      ymove = 0;
      cursorX = constrain(cursorX, 20, 220);
    }
  }

  if (cursorY == 30) {
    if (cursorX > 20 && cursorX < 110) {
      cursorY = constrain(cursorY, 30, 30);
      ymove = 0;
      cursorX = constrain(cursorX, 20, 110);
    } else if (cursorX > 130 && cursorX < 220) {
      cursorY = constrain(cursorY, 30, 30);
      ymove = 0;
      cursorX = constrain(cursorX, 130, 220);
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

  if (cursorY > 110 && cursorY < 130) {
    if (cursorX == 110) {
      xmove = 0;
      cursorX = constrain(cursorX, 110, 110);
      cursorY = constrain(cursorY, 110, 130);
    } else if (cursorX == 130) {
      xmove = 0;
      cursorX = constrain(cursorX, 130, 130);
      cursorY = constrain(cursorY, 110, 130);
    }
  }

  if (cursorY > 30 && cursorY < 110) {
    if (cursorX == 20) {
      xmove = 0;
      cursorX = constrain(cursorX, 20, 20);
      cursorY = constrain(cursorY, 30, 110);
    } else if (cursorX == 60) {
      xmove = 0;
      cursorX = constrain(cursorX, 60, 60);
      cursorY = constrain(cursorY, 30, 110);
    } else if (cursorX == 180) {
      xmove = 0;
      cursorX = constrain(cursorX, 180, 180);
      cursorY = constrain(cursorY, 30, 110);
    } else if (cursorX == 220) {
      xmove = 0;
      cursorX = constrain(cursorX, 220, 220);
      cursorY = constrain(cursorY, 30, 110);
    }
  }

  if (cursorY > 30 && cursorY < 70) {
    if (cursorX == 110) {
      xmove = 0;
      cursorX = constrain(cursorX, 110, 110);
      cursorY = constrain(cursorY, 30, 70);
    } else if (cursorX == 130) {
      xmove = 0;
      cursorX = constrain(cursorX, 130, 130);
      cursorY = constrain(cursorY, 30, 70);
    }
  }

  if (cursorY > 70 && cursorY < 110) {
    if (cursorX == 90) {
      xmove = 0;
      cursorX = constrain(cursorX, 90, 90);
      cursorY = constrain(cursorY, 70, 110);
    } else if (cursorX == 150) {
      xmove = 0;
      cursorX = constrain(cursorX, 150, 150);
      cursorY = constrain(cursorY, 70, 110);
    }
  }
















  //INTERSECTIONS
  // inter 1 ***************
    if (cursorX == 60 && cursorY == 240) {
      if (xVal > 555) {
        cursorY = constrain(cursorY, 240, 240);
        xmove = PAC_SPEED;
        ymove = 0;
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
          xmove = -PAC_SPEED;
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
          xmove = -PAC_SPEED;
          ymove = 0;
        }
      }
    // inter 4
      if (cursorX == 180 && cursorY == 270) {
        if (xVal > 555) {
          cursorY = constrain(cursorY, 270, 270);
          xmove = PAC_SPEED;
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
          xmove = PAC_SPEED;
          ymove = 0;
        }
        if (yVal > 567) {
          ymove = PAC_SPEED;
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
        ymove = PAC_SPEED;
      } else if (yVal < 467) {
        ymove = 0;
      }
      if (xVal < 455) {
        cursorY = constrain(cursorY, 270, 270);
        xmove = -PAC_SPEED;
        ymove = 0;
      }
    }
    // inter 6
      if (cursorX == 20 && cursorY == 300) {
        if (xVal > 555) {
          cursorY = constrain(cursorY, 300, 300);
          xmove = PAC_SPEED;
          ymove = 0;
        }
        if (yVal > 567) {
          ymove = 0;
        } else if (yVal < 467) {
          ymove = -PAC_SPEED;
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
          ymove = -PAC_SPEED;
        }
        if (xVal < 455) {
          cursorY = constrain(cursorY, 300, 300);
          xmove = -PAC_SPEED;
          ymove = 0;
        }
      }
  // inter 6 NOT WORKING
        if (cursorX == 90 && cursorY == 240) {
          if (xVal > 555) {
            cursorY = constrain(cursorY, 240,240);
            ymove = 0;
            xmove = PAC_SPEED;
          } else if (xVal < 455) {
            cursorY = constrain(cursorY, 240,240);
            ymove = 0;
            xmove = -PAC_SPEED;
          }
          if (yVal > 567) {
            cursorX = constrain(cursorX, 90, 90);
            xmove = 0;
            ymove = PAC_SPEED;
        }
    }
    else if (cursorX == 150 && cursorY == 240) {
      if (xVal > 555) {
        cursorY = constrain(cursorY, 240,240);
        ymove = 0;
        xmove = PAC_SPEED;
      } else if (xVal < 455) {
        cursorY = constrain(cursorY, 240,240);
        ymove = 0;
        xmove = -PAC_SPEED;
      }
      if (yVal > 567) {
        cursorX = constrain(cursorX, 150, 150);
        xmove = 0;
        ymove = PAC_SPEED;
    }
  }

  if (cursorX == 90 && cursorY == 270) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 270,270);
      ymove = 0;
      xmove = PAC_SPEED;
    } else if (xVal < 455) {
      ymove =0;
      xmove = 0;
    }
    if (yVal > 567) {
      ymove = 0;
      xmove =0;
    } else if (yVal < 467) {
      ymove = -PAC_SPEED;
      xmove = 0;
    }
  }

  if (cursorX == 150 && cursorY == 270) {
    if (xVal < 455) {
      cursorY = constrain(cursorY, 270,270);
      ymove = 0;
      xmove = -PAC_SPEED;
    } else if (xVal > 555) {
      ymove =0;
      xmove = 0;
    }
    if (yVal > 567) {
      ymove = 0;
      xmove =0;
    } else if (yVal < 467) {
      ymove = -PAC_SPEED;
      xmove = 0;
    }
  }


  if (cursorX == 110 && cursorY == 270) {
    if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    } else if (xVal > 555) {
      xmove = 0;
      ymove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 110, 110);
      xmove = 0;
      ymove = PAC_SPEED;
    } else if (yVal < 467) {
      ymove = 0;
    }
  }

  if (cursorX == 130 && cursorY == 270) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = 0;
      ymove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 130, 130);
      xmove = 0;
      ymove = PAC_SPEED;
    } else if (yVal < 467) {
      ymove = 0;
    }
  }



  if (cursorX == 110 && cursorY == 300) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 110, 110);
      xmove = 0;
      ymove = -PAC_SPEED;
    } else if (yVal > 567) {
      ymove = 0;
    }
  }
  if (cursorX == 130 && cursorY == 300) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 130, 130);
      xmove = 0;
      ymove = -PAC_SPEED;
    } else if (yVal > 567) {
      ymove = 0;
    }
  }


  if (cursorX == 40 && cursorY == 270) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 40, 40);
      xmove = 0;
      ymove = -PAC_SPEED;
    } else if (yVal > 567) {
      ymove = 0;
    }
  }
  if (cursorX == 200 && cursorY == 270) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 200, 200);
      xmove = 0;
      ymove = -PAC_SPEED;
    } else if (yVal > 567) {
      ymove = 0;
    }
  }

  if (cursorX == 40 && cursorY == 240) {
    if (xVal > 555) {
      xmove = 0;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal < 467) {
      ymove=0;
    } else if (yVal > 567) {
      cursorX = constrain(cursorX, 40, 40);
      xmove = 0;
      ymove = PAC_SPEED;
    }
  }
  if (cursorX == 200 && cursorY == 240) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
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
      ymove = PAC_SPEED;
    }
  }


  if (cursorX == 20 && cursorY == 240) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = 0;
      ymove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 20, 20);
      ymove = -PAC_SPEED;
      xmove = 0;
    } else if (yVal > 567) {
      ymove = 0;
    }
  }
  if (cursorX == 220 && cursorY == 240) {
    if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    } else if (xVal > 555) {
      xmove = 0;
      ymove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 220, 220);
      ymove = -PAC_SPEED;
      xmove = 0;
    } else if (yVal > 567) {
      ymove = 0;
    }
  }


  if (cursorX == 20 && cursorY == 210) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = 0;
      ymove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 20, 20);
      ymove = PAC_SPEED;
      xmove = 0;
    } else if (yVal < 467) {
      ymove = 0;
    }
  }
  if (cursorX == 220 && cursorY == 210) {
    if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    } else if (xVal > 555) {
      xmove = 0;
      ymove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 220, 220);
      ymove = PAC_SPEED;
      xmove = 0;
    } else if (yVal < 467) {
      ymove = 0;
    }
  }


  if (cursorX == 110 && cursorY == 240) {
    if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    } else if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 110, 110);
      ymove = -PAC_SPEED;
      xmove = 0;
    } else if (yVal > 567) {
      ymove = 0;
    }
  }

  if (cursorX == 130 && cursorY == 240) {
    if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    } else if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 130, 130);
      ymove = -PAC_SPEED;
      xmove = 0;
    } else if (yVal > 567) {
      ymove = 0;
    }
  }

  if (cursorX == 110 && cursorY == 210) {
    if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    } else if (xVal > 555) {
      xmove = 0;
      ymove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 110, 110);
      ymove = PAC_SPEED;
      xmove = 0;
    } else if (yVal < 467) {
      ymove = 0;
    }
  }
  if (cursorX == 130 && cursorY == 210) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = 0;
      ymove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 130, 130);
      ymove = PAC_SPEED;
      xmove = 0;
    } else if (yVal < 467) {
      ymove = 0;
    }
  }



  // FOR SOME REASON AT THESE INTERSECTIONS IT HAS A TOUGH TIME GOING LEFT AND RIGHT, BUT IT DOES WORK IF YOU WIGGLE THE STICK
  if (cursorX == 60 && cursorY == 210 || cursorX == 60 && cursorY == 211 || cursorX == 60 && cursorY == 209) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 210, 210);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      cursorY = constrain(cursorY, 210, 210);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal > 567) {
      ymove = PAC_SPEED;
      xmove = 0;
      cursorX = constrain(cursorX, 60, 60);
    }
    if (yVal < 467) {
      ymove = -PAC_SPEED;
      xmove = 0;
      cursorX = constrain(cursorX, 60, 60);
    }
  }
  if (cursorX == 180 && cursorY == 210) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 210, 210);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      cursorY = constrain(cursorY, 210, 210);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal > 567) {
      ymove = PAC_SPEED;
      xmove = 0;
      cursorX = constrain(cursorX, 180, 180);
    }
    if (yVal < 467) {
      ymove = -PAC_SPEED;
      xmove = 0;
      cursorX = constrain(cursorX, 180, 180);
    }
  }



  if (cursorX == 90 && cursorY == 210) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal > 567) {
      ymove = 0;
      xmove = 0;
    } else if (yVal < 467) {
      cursorX = constrain(cursorX, 90, 90);
      xmove = 0;
      ymove = -PAC_SPEED;
    }
  }

  if (cursorX == 150 && cursorY == 210) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal > 567) {
      xmove = 0;
    } else if (yVal < 467) {
      cursorX = constrain(cursorX, 150, 150);
      xmove = 0;
      ymove = -PAC_SPEED;
    }
  }



  if (cursorX == 90 && cursorY == 186) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 186, 186);
      xmove = PAC_SPEED;
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
      xmove = -PAC_SPEED;
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
      xmove = -PAC_SPEED;
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
      xmove = PAC_SPEED;
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
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 60, 60);
      ymove = PAC_SPEED;
      xmove = 0;
    } else if (yVal < 467) {
      cursorX = constrain(cursorX, 60, 60);
      xmove = 0;
      ymove = -PAC_SPEED;
    }
  }
  if (cursorX == 180 && cursorY == 160) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
      ymove = 0;
    } else if (xVal < 455) {
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 180, 180);
      ymove = PAC_SPEED;
      xmove = 0;
    } else if (yVal < 467) {
      cursorX = constrain(cursorX, 180, 180);
      xmove = 0;
      ymove = -PAC_SPEED;
    }
  }

  if (cursorX == 0 && cursorY == 160) {
    if (xVal > 555) {
      xmove = PAC_SPEED;
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
      xmove = -PAC_SPEED;
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


  if (cursorX == 90 && cursorY == 130) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 130, 130);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 90, 90);
      xmove = 0;
    }
    if (xVal < 455) {
      xmove = 0;
    }
    if (yVal < 467) {
      ymove = 0;
    }
  }
  if (cursorX == 150 && cursorY == 130) {
    if (xVal < 455) {
      cursorY = constrain(cursorY, 130, 130);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 150, 150);
      xmove = 0;
    }
    if (xVal > 555) {
      xmove = 0;
    }
    if (yVal < 467) {
      ymove = 0;
    }
  }

  if (cursorX == 110 && cursorY == 130) {
    if (xVal < 455 || xVal > 555) {
      cursorY = constrain(cursorY, 130, 130);
      ymove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 110, 110);
      ymove = -PAC_SPEED;
      xmove = 0;
    }
    if (yVal > 567) {
      ymove = 0;
    }
  }

  if (cursorX == 130 && cursorY == 130) {
    if (xVal < 455 || xVal > 555) {
      cursorY = constrain(cursorY, 130, 130);
      ymove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 130, 130);
      ymove = -PAC_SPEED;
      xmove = 0;
    }
    if (yVal > 567) {
      ymove = 0;
    }
  }

  if (cursorX == 110 && cursorY == 110) {
    if (xVal < 455) {
      cursorY = constrain(cursorY, 110, 110);
      ymove = 0;
    }
    if (xVal > 555) {
      xmove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 110, 110);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
      ymove = 0;
    }
  }
  if (cursorX == 130 && cursorY == 110) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 110, 110);
      ymove = 0;
    }
    if (xVal < 455) {
      xmove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 130, 130);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
      ymove = 0;
    }
  }

  if (cursorX == 90 && cursorY == 110) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 110, 110);
      ymove = 0;
    }
    if (xVal < 455) {
      xmove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 90, 90);
      ymove = -PAC_SPEED;
      xmove = 0;
    }
    if (yVal > 567) {
      ymove = 0;
    }
  }
  if (cursorX == 150 && cursorY == 110) {
    if (xVal < 455) {
      cursorY = constrain(cursorY, 110, 110);
      ymove = 0;
    }
    if (xVal > 555) {
      xmove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 150, 150);
      ymove = -PAC_SPEED;
      xmove = 0;
    }
    if (yVal > 567) {
      ymove = 0;
    }
  }

  if (cursorX == 20 && cursorY == 110) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 110, 110);
      ymove = 0;
    }
    if (xVal < 455) {
      xmove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 20, 20);
      ymove = -PAC_SPEED;
      xmove = 0;
    }
    if (yVal > 567) {
      ymove = 0;
    }
  }
  if (cursorX == 220 && cursorY == 110) {
    if (xVal < 455) {
      cursorY = constrain(cursorY, 110, 110);
      ymove = 0;
    }
    if (xVal > 555) {
      xmove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 220, 220);
      ymove = -PAC_SPEED;
      xmove = 0;
    }
    if (yVal > 567) {
      ymove = 0;
    }
  }

  if (cursorX == 60 && cursorY == 110) {
    if (xVal < 455) {
      cursorY = constrain(cursorY, 110, 110);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (xVal > 555) {
      xmove = 0;
    }
    if (yVal > 567 || yVal < 467) {
      cursorX = constrain(cursorX, 60, 60);
      xmove = 0;
    }
  }
  if (cursorX == 180 && cursorY == 110) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 110, 110);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      xmove = 0;
    }
    if (yVal > 567 || yVal < 467) {
      cursorX = constrain(cursorX, 180, 180);
      xmove = 0;
    }
  }

  if (cursorX == 20 && cursorY == 70) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      xmove = 0;
    }
    if (yVal > 567 || yVal < 467) {
      cursorX = constrain(cursorX, 20, 20);
      xmove = 0;
    }
  }

  if (cursorX == 220 && cursorY == 70) {
    if (xVal < 455) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
    if (xVal > 555) {
      xmove = 0;
    }
    if (yVal > 567 || yVal < 467) {
      cursorX = constrain(cursorX, 220, 220);
      xmove = 0;
    }
  }

  if (cursorX == 60 && cursorY == 70) {
    if (yVal > 567) {
      cursorX = constrain(cursorX, 60, 60);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 60, 60);
      ymove = -PAC_SPEED;
      xmove = 0;
    }
    if (xVal > 555) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
  }

  if (cursorX == 90 && cursorY == 70) {
    if (yVal > 567) {
      cursorX = constrain(cursorX, 90, 90);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
      ymove = 0;
    }
    if (xVal > 555) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
  }

  if (cursorX == 110 && cursorY == 70) {
    if (yVal < 467) {
      cursorX = constrain(cursorX, 110, 110);
      ymove = -PAC_SPEED;
      xmove = 0;
    }
    if (yVal > 567) {
      ymove = 0;
    }
    if (xVal > 555) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
  }

  if (cursorX == 130 && cursorY == 70) {
    if (yVal < 467) {
      cursorX = constrain(cursorX, 130, 130);
      ymove = -PAC_SPEED;
      xmove = 0;
    }
    if (yVal > 567) {
      ymove = 0;
    }
    if (xVal > 555) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
  }

  if (cursorX == 150 && cursorY == 70) {
    if (yVal > 567) {
      cursorX = constrain(cursorX, 150, 150);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
      ymove = 0;
    }
    if (xVal > 555) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
  }

  if (cursorX == 180 && cursorY == 70) {
    if (yVal > 567) {
      cursorX = constrain(cursorX, 180, 180);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
      cursorX = constrain(cursorX, 180, 180);
      ymove = -PAC_SPEED;
      xmove = 0;
    }
    if (xVal > 555) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      cursorY = constrain(cursorY, 70, 70);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
  }

  if (cursorX == 20 && cursorY == 30) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 30, 30);
      ymove = 0;
    }
    if (xVal < 455) {
      xmove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 20, 20);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
      ymove = 0;
    }
  }

  if (cursorX == 60 && cursorY == 30) {
    if (yVal > 567) {
      cursorX = constrain(cursorX, 60, 60);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
      ymove = 0;
    }
    if (xVal > 555) {
      cursorY = constrain(cursorY, 30, 30);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      cursorY = constrain(cursorY, 30, 30);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
  }

  if (cursorX == 180 && cursorY == 30) {
    if (yVal > 567) {
      cursorX = constrain(cursorX, 180, 180);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
      ymove = 0;
    }
    if (xVal > 555) {
      cursorY = constrain(cursorY, 30, 30);
      xmove = PAC_SPEED;
      ymove = 0;
    }
    if (xVal < 455) {
      cursorY = constrain(cursorY, 30, 30);
      xmove = -PAC_SPEED;
      ymove = 0;
    }
  }

  if (cursorX == 220 && cursorY == 30) {
    if (xVal < 455) {
      cursorY = constrain(cursorY, 30, 30);
      ymove = 0;
    }
    if (xVal > 555) {
      xmove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 220, 220);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
      ymove = 0;
    }
  }

  if (cursorX == 110 && cursorY == 30) {
    if (xVal < 455) {
      cursorY = constrain(cursorY, 30, 30);
      ymove = 0;
    }
    if (xVal > 555) {
      xmove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 110, 110);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
      ymove = 0;
    }
  }

  if (cursorX == 130 && cursorY == 30) {
    if (xVal > 555) {
      cursorY = constrain(cursorY, 30, 30);
      ymove = 0;
    }
    if (xVal < 455) {
      xmove = 0;
    }
    if (yVal > 567) {
      cursorX = constrain(cursorX, 130, 130);
      ymove = PAC_SPEED;
      xmove = 0;
    }
    if (yVal < 467) {
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

void ghostMovement(){
  if (ghost == true){
    ghostMovements();
  }
  travelling();
  delay(10);
}


void highScoreScreen(int iteration){
  // draw the screen all black first
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.println("Rank            Username         Score");
  for (int i = 0; i < (iteration-1); i++) {
    String strIter = String(i+1);
    tft.print(strIter);
    tft.print("           ");
    tft.print(playerRecord[i].names);
    tft.print("           ");
    String strPScore = String(playerRecord[i].points);
    tft.println(strPScore);
  }
}



int main() {
  unsigned long startTime, endTime;
  unsigned long delta = 0;
  setup();
  // for later
  menu();
  //String username = name();
  startTime = millis();
  screenlayout();
  // scoreDots();
  // place holder score
  //score = 100;
  while (true) {
    ghostMovement();
    movement();
    // // just testing for now
    // if (delta > 3000){
    //   break;
    // }
    // for later

    int checkButton = digitalRead(JOY_SEL);
    //Serial.println(checkButton);
    if (checkButton == LOW){
      endTime = millis();
      delta = endTime-startTime;
      Serial.println("button pressed");
      break;
    }

  }
  score = delta;
  String strScore = endGame(score);
  // just keep sending it
  Serial.println("after endgame");
  while (comDone == false){
    clientCommunication(strScore);
  }
  Serial.println("after cleint com");
  highScoreScreen(numberOfScores);
  Serial.flush();

  Serial.end();
  return 0;
}
