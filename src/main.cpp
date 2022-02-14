#include <Arduino.h>
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
//#include "GyverUART.h";
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <FreeDefaultFonts.h>

// размер экрана отверстие 52 на 88


// All the mcufriend.com UNO shields have the same pinout.
// i.e. control pins A0-A4.  Data D2-D9.  microSD D10-D13.
// Touchscreens are normally A1, A2, D7, D6 but the order varies
//
// This demo should work with most Adafruit TFT libraries
// If you are not using a shield,  use a full Adafruit constructor()
// e.g. Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//https://alexgyver.ru/lessons/arduino-libraries/

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define PORTRAIT 0
#define LANDSCAPE 1
#define PORTRAIT_REV 2
#define LANDSCAPE_REV 3

MCUFRIEND_kbv tft;

/*------------------------------------------------------*/
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ORANGE  0xFD20
/*------------------------------------------------------*/
int speed = 0;
char cstr[80];

/*------------------------------------------------------*/
String speedprev="0";
String speedcur="0";



/*------------------------------------------------------*/

void analogMeter()
{
  const  char labels[6][4] = {"  0", " 25", " 50", " 75", "100", "%DC"};
  // Meter outline
  tft.fillRect(0, 0, 319, 168, LIGHTGREY);
  tft.fillRect(5, 3, 309, 162, WHITE);  
  tft.setTextColor(BLACK);  // Text colour
  
  
  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for( int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 20;   
    // Coodinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (133 + tl) + 160;
    uint16_t y0 = sy * (133 + tl) + 187;
    uint16_t x1 = sx * 133 + 160;
    uint16_t y1 = sy * 133 + 187;
    
    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = sx2 * (133 + tl) + 160;
    int y2 = sy2 * (133 + tl) + 187;
    int x3 = sx2 * 133 + 160;
    int y3 = sy2 * 133 + 187;

    // Draw scale arc, don't draw the last part
    if (i < 50) tft.drawLine(x3, y3, x1, y1, BLACK);

    // Yellow zone limits
    if((i >= -50) && (i < 0)) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, YELLOW);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, YELLOW);
      }

    // Green zone limits
    if((i >= 0) && (i < 25)) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, GREEN);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, GREEN);
      }

    // Orange zone limits
    if((i >= 25) && (i < 50)) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, ORANGE);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, ORANGE);
      }
    
    // Short scale tick length
    if (i % 25 != 0) {
      tl = 11; 
      // Recalculate coords incase tick lenght changed
      x0 = sx * (133 + tl) + 160;
      y0 = sy * (133 + tl) + 187;
      }
    // Draw tick
    tft.drawLine(x0, y0, x1, y1, BLACK);
    
    // Check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {

      x0 = sx * (133 + tl + 10) + 160 -18; //corr
      y0 = sy * (133 + tl + 10) + 187;
      int labp = (i / 25) + 2;
      switch (labp) {
        case 0: tft.setCursor( x0, y0-16); break;
        case 1: tft.setCursor( x0, y0-12); break;
        case 2: tft.setCursor( x0, y0 -8); break;
        case 3: tft.setCursor( x0, y0-12); break;
        case 4: tft.setCursor( x0, y0-16); break;
      }
    tft.setFont();  
    tft.setTextColor(BLACK);  
    tft.setTextSize(2);
    tft.println(labels[labp]);
    }
  }  
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, int col,  const char *msg)
{
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(col);
    tft.setTextSize(sz);
    tft.print(msg);
}

void sound(int s)// звук
{

}

void davarika(int s) // аварийка
{
 if (s==1){ 
 tft.fillTriangle(349, 50, 399, 50, 375, 1, RED);  
 tft.fillTriangle(352, 47, 396, 47, 375, 4, BLACK);  
 tft.fillTriangle(358, 44, 390, 44, 375, 10, RED);
 tft.fillTriangle(362, 42, 387, 42, 375, 15, WHITE);
 }
 else{
  tft.fillRect(349, 1, 50/*ширина*/, 50/*высота*/, BLACK); 
 }
}

void dpovorot(int s) // поворотники
{
  sound(s);
  tft.fillTriangle(249, 25, 269, 4, 269, 46, GREEN);  //l
  tft.fillTriangle(280, 4, 300, 25, 280, 46, GREEN);//r
   if(s==1){
     tft.fillTriangle(249, 25, 269, 4, 269, 46, GREEN);  //l
     tft.fillTriangle(280, 4, 300, 25, 280, 46, GREEN);//r
   }
   else{
    tft.fillTriangle(249, 25, 269, 4, 269, 46, BLACK);  //l
    tft.fillTriangle(280, 4, 300, 25, 280, 46, BLACK);//r
   }

}

void dspeed(String s)// спидометр
{
    speedcur=s;
    speedcur=" "+speedcur;
    if (speedcur!=speedprev){
        speedprev.toCharArray(cstr, speedprev.length()+1);
        showmsgXY(1, 51, 1, &FreeSevenSegNumFont,BLACK, cstr);
        speedcur.toCharArray(cstr, speedcur.length()+1);
        showmsgXY(1, 51, 1, &FreeSevenSegNumFont,GREEN, cstr);
        speedprev=speedcur;
    }
}

void dtaho(String t) // тахометр
{
    t.toCharArray(cstr, t.length()+1);
    tft.fillRect(96,1, 95, 50, BLACK);
    showmsgXY(110, 51, 1, &FreeSevenSegNumFont,YELLOW, cstr);
}

void dstop(int s) // ручник
{
  tft.drawLine(317,1,333,1,RED);//up 304 346
  tft.drawLine(317,50,333,50,RED);//bottom
  tft.drawLine(304,16,304,35,RED);//left
  tft.drawLine(346,16,346,35,RED);//right
  tft.drawLine(304,16,317,1,RED);//from 10 clock
  tft.drawLine(333,1,346,16,RED);//bottom
  tft.drawLine(346,35,333,50,RED);//left
  tft.drawLine(317,50,304,35,RED);//right  
  
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(315, 37);
  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.print("P");
  sound(s);
}

void dlight(int s)// дальний
{
 if (s==1){
  tft.fillRect(299, 104, 49/*ширина*/, 49/*высота*/, BLUE);
  tft.drawLine(324,110,324,145,WHITE);//
  tft.drawLine(320,110,310,145,WHITE);//
  tft.drawLine(328,110,338,145,WHITE);//
 }
 else{
  tft.fillRect(299, 104, 49/*ширина*/, 49/*высота*/, BLACK);
 }
}

void dback(int s)// заднее стекло
{
  if(s==1){
   tft.drawRect(350, 104, 49/*ширина*/, 49/*высота*/, LIGHTGREY);
   tft.drawRect(355, 114, 39/*ширина*/, 29/*высота*/, BLUE);
   tft.drawRect(355, 124, 39/*ширина*/, 9/*высота*/, BLUE);
   tft.drawFastVLine(364,120,15,RED);
   tft.drawFastVLine(374,120,15,RED);
   tft.drawFastVLine(384,120,15,RED);
  }
  else
  {
  tft.fillRect(350, 104, 49/*ширина*/, 49/*высота*/, BLACK);
  }
}

void dtemp(String s)// температура ОЖ
{
    int x0=0;
    int y0=0;
    
    s.toCharArray(cstr, s.length()+1);
    tft.drawRect(102+x0, 54+y0, 80, 38, DARKGREY);// второй ширина потом высота
    tft.setFont(&FreeSans9pt7b);
    tft.setCursor(107+x0, 84+y0);
    tft.setTextColor(DARKGREY);
    tft.setTextSize(2);
    tft.print(cstr);
}

void dvolt(String s)// напряжение
{
    int x0=0;
    int y0=0;
    s=s+'v';
    s.toCharArray(cstr, s.length()+1);
    tft.drawRect(1+x0, 54+y0, 99, 38, DARKGREY);// второй ширина потом высота
    tft.setFont(&FreeSans9pt7b);
    tft.setCursor(3+x0, 84+y0);
    tft.setTextColor(DARKGREY);
    tft.setTextSize(2);
    tft.print(cstr);
}

void dfuel(String s)// топливо
{
    int x0=0;
    int y0=0;
    int x1=0;
    int y1=0;

    s.toCharArray(cstr, s.length()+1);
    tft.drawRect(184+x0, 54+y0, 52, 38, DARKGREY);// второй ширина потом высота
    tft.setFont(&FreeSans9pt7b);
    tft.setCursor(188+x0, 84+y0);
    tft.setTextColor(DARKGREY);
    tft.setTextSize(2);
    tft.print(cstr);
    int v=40;
    tft.drawRect(1+x1, 94+y1, 35, 144, DARKGREY);
    tft.fillRect(3+x1, 96+y1+v, 31, 140-v, YELLOW);
}

void dhot(int s)// перегрев ОЖ
{
   int xt=0;
   int yt=0;
 if(s==1){
    tft.fillRect(249, 54, 48, 49, RED);
    tft.setFont(&FreeSans9pt7b);
    tft.setCursor(255+xt, 76+yt);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("HOT");
    tft.setCursor(255+xt, 93+yt);
    tft.setTextColor(BLACK);
    tft.print("XXX");    
 }
 else{
  tft.fillRect(249, 54, 48, 49, BLACK);
 }
}

void dnakal(int s)// свечи накала
{
   int xt=0;
   int yt=0;
 if(s==1){
    tft.fillRect(249, 104, 48, 49, YELLOW);
    tft.setFont(&FreeSans9pt7b);
    tft.setCursor(261+xt, 139+yt);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print("D");    
 }
 else{
  tft.fillRect(249, 104, 48, 49, BLACK);
 }
}

void dlevel(int s) // уровень ОЖ
{
   int xt=0;
   int yt=0;
  if(s==1){
    tft.fillRect(299, 54, 49, 49, RED);
    tft.setFont(&FreeSans9pt7b);
    tft.setCursor(305+xt, 76+yt);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.print("HOT");
    tft.setCursor(303+xt, 93+yt);
    tft.print("LOW");
  }
  else{
    tft.fillRect(298, 54, 49, 49, BLACK);
  }
  sound(s);
}

void dmaslo(int s) // давление масла
{
 int x0=-5;
 int y0=-20;
 int xt=0;
 int yt=60;
 if(s==1){
   tft.fillRect(350, 54, 49/*ширина*/, 49/*высота*/, RED);
   tft.drawLine(364+x0,94+y0,384+x0,94+y0,BLACK);//bottom
   tft.drawLine(364+x0,94+y0,364+x0,90+y0,BLACK);//left
   tft.drawLine(364+x0,90+y0,360+x0,85+y0,BLACK);// 3 линии левая ручка
   tft.drawLine(360+x0,85+y0,362+x0,80+y0,BLACK);//2
   tft.drawLine(362+x0,80+y0,364+x0,90+y0,BLACK);//3
   tft.drawLine(364+x0,90+y0,367+x0,85+y0,BLACK);//
   tft.drawLine(367+x0,85+y0,378+x0,85+y0,BLACK);//
   tft.drawLine(378+x0,85+y0,380+x0,88+y0,BLACK);//
   tft.drawLine(380+x0,88+y0,395+x0,86+y0,BLACK);// nose 
   tft.drawLine(395+x0,86+y0,384+x0,94+y0,BLACK);//
   tft.drawLine(396+x0,88+y0,395+x0,90+y0,BLACK);// капля
   tft.drawLine(396+x0,88+y0,397+x0,90+y0,BLACK);
   tft.drawLine(373+x0,84+y0,373+x0,81+y0,BLACK);// крышка
   tft.drawLine(371+x0,81+y0,375+x0,81+y0,BLACK);
   tft.setFont(&FreeSans9pt7b);
   tft.setCursor(360+xt, 37+yt);
   tft.setTextColor(WHITE);
   tft.setTextSize(1);
   tft.print("OIL");
 }
 else{
   tft.fillRect(349, 54, 50, 50, BLACK);
 }
 sound(s);
}

void dclock(String s)// отображение времени
{


}
/*------------------------------------------------------*/

void setup(void) 
{
    Serial.begin(9600); 
    uint16_t ID = tft.readID(); //
    Serial.println(ID, HEX);
    tft.begin(ID);  // my is 9327
    tft.setRotation(LANDSCAPE);
    tft.fillScreen(BLACK);
    tft.setFont();
    tft.setTextSize(2);

tft.drawRect(38,94,198,144,DARKGREY);

davarika(1);
dpovorot(1);
dstop(1);
speed=67;
dspeed(String(98));
dtaho("3500");
dlight(1);
dtemp("99 c");
dmaslo(1);
dlevel(1);
dhot(1);
dnakal(1);
dfuel("40");
dvolt("13.7");
dback(1);
dclock("23:59");
}

void loop(void) 
{  

  while (Serial.available() > 0) {
    char inChar = Serial.read();
    //inString += inChar;
    if (inChar == '\n') {    
    
    }
  }  

dspeed(String(speed));

}
