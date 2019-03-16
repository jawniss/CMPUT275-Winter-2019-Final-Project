/*


Need to figure out how to make "barriers" the walls that the
pacman can't move past

For the score dots, when pacman moves over them they don't get
redrawn, perfect for this project
  simply make a counter
  if (position of pacman % 2 [or something] == number ) {
  score counter += 10
}
do this for each line of dots


ORIENTATION

(0,            (maxX,
0)                  0)
  _______________
 |               |
 |               |
 |               |  YYYYYYYYYY
 |               |
 |               |
 |               |
 |_______________| (maxX, maxY)
(0,maxY)
      XXXXXXXXXX








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




void setup() {
  init();
  Serial.begin(9600);

  pinMode(JOY_SEL, INPUT_PULLUP);

  tft.begin();

  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(4);

  tft.setTextSize(1);
  tft.setTextWrap(false);

  cursorX = (DISPLAY_WIDTH/4);
  cursorY = DISPLAY_HEIGHT/2;

  tft.fillCircle(cursorX, cursorY, PACMAN_SIZE, ILI9341_YELLOW);
  tft.fillRect(0, DISPLAY_WIDTH - 10, DISPLAY_HEIGHT, 5, ILI9341_BLUE);
  tft.fillRect(120,120,50,50,ILI9341_BLUE);




  tft.setCursor(0,0);
  tft.setTextSize(1);

  tft.println("Size: 1");
  tft.println("");
}


void scoreDots() {
  for (int i = 10; i < 20; i+=3) {
    tft.fillCircle(screenmidX + i, screenmidY + i, 2, ILI9341_WHITE);
  }
}





void redrawPacman (int newX, int newY, int oldX, int oldY) {
  tft.fillCircle(oldX, oldY, PACMAN_SIZE, ILI9341_BLACK);
  tft.fillCircle(newX, newY, PACMAN_SIZE, ILI9341_YELLOW);
}




void processJoystick() {
  int xVal = analogRead(JOY_HORIZ);
  int yVal = analogRead(JOY_VERT);
  int buttonVal = digitalRead(JOY_SEL);
  // copy the cursor position (to check later if it changed)
  int oldX = cursorX;
  int oldY = cursorY;
  if (455 < yVal && 555 > yVal && xVal < 567 && 467 < xVal) {
    return;
  }



  // For pacman we don't want to have variable speed movement
  if (yVal > 555) {
    xmove = 2;
  }
  else if (yVal < 455) {
    xmove = -2;
  }
  else {
    xmove = 0;
  }
  if (xVal > 567) {
    ymove = 2;
  }
  else if (xVal < 467) {
    ymove = -2;
  }
  else {
    ymove = 0;
  }


  cursorX += xmove;
  cursorY += ymove;
  // constrain so the cursor does not go off of the map display window
  // the ending "-1"s make it perfectly not go off the edges
  cursorX = constrain(cursorX, PACMAN_SIZE, DISPLAY_HEIGHT - PACMAN_SIZE - 1);
  cursorY = constrain(cursorY, PACMAN_SIZE, DISPLAY_WIDTH - PACMAN_SIZE - 1);


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



  // redraw the cursor only if its position actually changed
  if (cursorX != oldX || cursorY != oldY) {
    redrawPacman(cursorX, cursorY, oldX, oldY);
  }
  delay(10);
}




// function to calculate manhatten distance
// I'LL NEED LATER TO DO DIJKSTRA FOR GHOST
int manhatten(int currentx, int restx, int currenty, int resty){
  int distance;
  distance = abs(currentx - restx) + abs(currenty - resty);
  return distance;
}




int main() {
  setup();
  scoreDots();
  while (true) {
    processJoystick();

  }
  Serial.end();
  return 0;
}
