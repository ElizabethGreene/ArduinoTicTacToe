// Based on the Drawline demo code from the seeduino website.
// My first Arduino project, please offer constructive criticism.
// Copyright 2012 Elizabeth Greene <Elizabeth.a.Greene@gmail.com>
// Permission to use granted under the Creative Commons Attribution 2.0 Generic license.

#include <stdint.h>
#include <TouchScreen.h> 
#include <TFT.h>

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM A0   // can be a digital pin, this is A0
  #define XP A3   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 


//Measured ADC values for (0,0) and (240-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1
 
#define TS_MINX 140
#define TS_MAXX 900

#define TS_MINY 120
#define TS_MAXY 940

#define XMARK 1
#define OMARK 0
#define NOMARK -1
#define DRAW -2

int Board[10];
int turndicator = OMARK;

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void setup()
{
  Tft.init();  //init TFT library
  DrawSplash();
  delay(3000);
  ResetGame();
}

void DrawSplash() {
  DrawBoard(); 
  drawX(5);
  delay(500);
  drawO(1);
  delay(500);
  drawX(4);
  delay(500);
  drawO(3);
  delay(500);
  drawX(6);
  Tft.drawString ("TIC-TAC-TOE", 30,152,2, WHITE);  
}  
  
void loop() {
  int MapX1 = 239;
  int MapX2 = 0;
  int MapY1 = 319;
  int MapY2 = 0;

  // a point object holds x y and z coordinates
  Point p = ts.getPoint();
 
  p.x = map(p.x, TS_MINX, TS_MAXX, MapX1, MapX2);
  p.y = map(p.y, TS_MINY, TS_MAXY, MapY1, MapY2);
 
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > ts.pressureThreshhold) 
  {
    //p.x;
    //p.y;
    //p.z;
     
    int pos = GetFingerPos(p.x,p.y);
    if (Board[pos] == -1) {
      if (turndicator == OMARK ) {
        drawO(pos);
        Board[pos] = turndicator;
        turndicator = XMARK;
      } else {
        drawX(pos);
        Board[pos] = turndicator;
        turndicator = OMARK;
      }
      int winner = CheckWinner();
      if (winner != NOMARK) {
        if (winner == XMARK) { Tft.drawString ("X WINS", 20,152,4, BLUE); } 
        if (winner == OMARK) { Tft.drawString ("O WINS", 22,152,4, BLUE); } 
        if (winner == DRAW) { Tft.drawString ("DRAW", 35,152,5,BLUE); } 
        delay(2000);
        ResetGame();
      }
    }   
   //  Tft.fillCircle(p.x,p.y,2,GREEN);
  }
}

int CheckWinner() {

  int winner = NOMARK;
  //Surely there is a better way?
  
  //Across
  if (Board[1] == Board[2] && Board[1] == Board[3] && winner == NOMARK) {winner=Board[1]; }
  if (Board[4] == Board[5] && Board[4] == Board[6] && winner == NOMARK) {winner=Board[4]; }
  if (Board[7] == Board[8] && Board[7] == Board[9] && winner == NOMARK) {winner=Board[7]; }  

  //down
  if (Board[1] == Board[4] && Board[1] == Board[7] && winner == NOMARK) {winner=Board[1]; }
  if (Board[2] == Board[5] && Board[2] == Board[8] && winner == NOMARK) {winner=Board[2]; }
  if (Board[3] == Board[6] && Board[3] == Board[9] && winner == NOMARK) {winner=Board[3]; }

  //diagonal
  if (Board[1] == Board[5] && Board[1] == Board[9] && winner == NOMARK) {winner=Board[1]; }
  if (Board[3] == Board[5] && Board[3] == Board[7] && winner == NOMARK) {winner=Board[3]; }
  
  // finally, check and see if all of the spaces are occupied.. 
  if (winner == NOMARK && Board[1] > -1 && Board[2] > -1 && Board[3] > -1 && Board[4] > -1 && Board[5] > -1 && Board[6] > -1 && Board[7] > -1 && Board[8] > -1 && Board[9] > -1 ) { winner = DRAW; }
  
  return winner;
}

void ResetGame() {
  DrawBoard(); // Draw the board
  for (int i=0;i<10;i++) { Board[i] = NOMARK; }
}

void DrawBoard() {
 // I draw the game board First, blank the screen Then draw the board
 // X blocks are 80px wide
 // y blocks are 106px wide
 // x's and o's are 75% of block width, so call it 60x60 square.  

  Tft.fillRectangle(0,0,239,319,BLACK);
  Tft.drawLine(0,106,239,106,GREEN);  
  Tft.drawLine(0,212,239,212,GREEN);  
  Tft.drawLine(80,0,80,319,GREEN);  
  Tft.drawLine(160,0,160,319,GREEN);    
}

int GetFingerPos(int x, int y) {
  int row =0;
  int col =0;

  col = x/80;
  row = y/106;

  int pos = row * 3 + col +1 ;
return pos; 
}
 
void drawX(int pos) { 
// I draw an X at an arbitrary position on the screen
  Tft.drawLine(getCenterX(pos)-30,getCentery(pos)-30, getCenterX(pos)+30, getCentery(pos)+30,RED);
  Tft.drawLine(getCenterX(pos)+30,getCentery(pos)-30, getCenterX(pos)-30, getCentery(pos)+30,RED);

}

void drawO(int pos) { 
  Tft.drawCircle(getCenterX(pos),getCentery(pos), 30 ,RED);
}

int getCenterX( int pos ) {
// I return the center x position for a given spot on the board.

if (pos == 1) return 40;
if (pos == 4) return 40;
if (pos == 7) return 40;
if (pos == 2) return 120;
if (pos == 5) return 120;
if (pos == 8) return 120;
if (pos == 3) return 200;
if (pos == 6) return 200;
if (pos == 9) return 200;

}

int getCentery ( int pos ) {
// I return the center y position for a given spot on the board.

if (pos == 1) return 53;
if (pos == 2) return 53;
if (pos == 3) return 53;
if (pos == 4) return 159;
if (pos == 5) return 159;
if (pos == 6) return 159;
if (pos == 7) return 265;
if (pos == 8) return 265;
if (pos == 9) return 265;
}

