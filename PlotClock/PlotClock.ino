// Plotclock
// for PandoraLab
//
// Interactive calibration, interface translations and modifications by
// Mau Jabur - maumaker -at- pandoralab -dot- com -dot- br
// Version 1.0
//
// // LICENSE:
// Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
//[https://creativecommons.org/licenses/by-sa/4.0/]
//
// based on version 1.03 code by 
// Johannes Heberlein 2014
// v 1.03
// thingiverse.com/joo   wiki.fablab-nuernberg.de
// units: mm; microseconds; radians
// origin: bottom left of drawing surface
// time library see http://playground.arduino.cc/Code/time
// RTC  library see http://playground.arduino.cc/Code/time
//               or http://www.pjrc.com/teensy/td_libs_DS1307RTC.html
// Change log:
// 1.01  Release by joo at https://github.com/9a/plotclock
// 1.02  Additional features implemented by Dave (https://github.com/Dave1001/):
//       - added ability to calibrate servofaktor seperately for left and right servos
//       - added code to support DS1307, DS1337 and DS3231 real time clock chips
//       - see http://www.pjrc.com/teensy/td_libs_DS1307RTC.html for how to hook up the real time clock
// 1.03  Fixed the length bug at the servo2 angle calculation, other fixups

#define VERSION 1.00

// choose a translation
#include "Translation_PT.h"
//#include "Translation_EN.h"

// delete or mark the next line as comment if you don't need these
//#define REALTIMECLOCK    // enable real time clock

#define ERASE_WIDTH 62.0
#define ERASE_HEIGHT 22.0
#define ERASE_SWEEPS 3

#define SERVOPINLIFT  2
#define SERVOPINLEFT  3
#define SERVOPINRIGHT 4

// speed of liftimg arm, higher is slower
#define LIFTSPEED 1500

// length of arms
#define L1 35
#define L2 55.1
#define L3 13.2
#define L4 45

// origin points of left and right servo
#define O1X 24//22
#define O1Y -25
#define O2X 48//47
#define O2Y -25

#ifdef REALTIMECLOCK
// for instructions on how to hook up a real time clock,
// see here -> http://www.pjrc.com/teensy/td_libs_DS1307RTC.html
// DS1307RTC works with the DS1307, DS1337 and DS3231 real time clock chips.
// Please run the SetTime example to initialize the time on new RTC chips and begin running.

#include <Wire.h>
#include <DS1307RTC.h> // see http://playground.arduino.cc/Code/time    
#endif

#include <Time.h> // see http://playground.arduino.cc/Code/time 

#include <Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;

// Calibration data is stored on EEPROM
#include "Config.h"
Config cfg;
// change the base address for the EEPROM if you have any problems
int cfg_addr = 0;

boolean adjust;

int servoLift = cfg.lift_draw;
double lastX = cfg.park_x - 0.25;
double lastY = cfg.park_y - 0.25;

int last_min = -1;

void setup()
{
  Serial.begin(9600);

  CREDITS();

  readConfig(cfg_addr, cfg);

  initServos();

  int Hour, Min, Sec;
  sscanf(__TIME__, "%d:%d:%d", &Hour, &Min, &Sec);

  //while (!Serial) { ; } // wait for serial port to connect. Needed for Leonardo only

#ifdef REALTIMECLOCK

  // Set current time only the first to values, hh,mm are needed
  tmElements_t tm;
  if (RTC.read(tm))
  {
    setTime(tm.Hour, tm.Minute, tm.Second, tm.Day, tm.Month, tm.Year);
    RTC_OK();
  }
  else
  {
    if (RTC.chipPresent())
    {
      RTC_STOPPED();
    }
    else
    {
      RTC_MISSING();
    }
    // Set current time only the first to values, hh,mm are needed



    setTime(Hour, Min, Sec, 0, 0, 0);
  }
#else
  // Set current time only the first to values, hh,mm are needed
  setTime(Hour, Min, Sec, 0, 0, 0);
#endif

  setAdjust(false);
}

void loop()
{
  checkSerial();

  if (adjust == false)  { // operation
    if (last_min != minute()) {
      last_min = minute();

      attachAll();

      number(3, 3, 111, 0.9);
      number(cfg.start_x, cfg.start_y, hour() / 10, 0.9);
      number(cfg.start_x + 13, cfg.start_y, hour() % 10, 0.9);
      number(cfg.start_x + 22, cfg.start_y, 11, 0.9);

      number(cfg.start_x + 28, cfg.start_y, minute() / 10, 0.9);
      number(cfg.start_x + 41, cfg.start_y, minute() % 10, 0.9);

      lift(cfg.lift_park);
      park();
      lift(cfg.lift_draw);
      delay(580);

      detachAll();
    }
  }
}

void initServos() {
  // assume that the pen in in parking position
  servo1.writeMicroseconds(cfg.lift_draw);
  servo1.attach(SERVOPINLIFT);
  lift(cfg.lift_park);
  park();
  attachAll();
}

void attachAll() {
  if (!servo1.attached()) servo1.attach(SERVOPINLIFT);
  if (!servo2.attached()) servo2.attach(SERVOPINLEFT);
  if (!servo3.attached()) servo3.attach(SERVOPINRIGHT);
}

void detachAll() {
  servo1.detach();
  servo2.detach();
  servo3.detach();
}
void park() {
  drawTo(cfg.park_x - 20, cfg.park_y - 5);
  drawTo(cfg.park_x + 3, cfg.park_y);
  delay(100);
  drawTo(cfg.park_x, cfg.park_y);
  delay(100);
}

void unpark() {
  drawTo(cfg.park_x - 20, cfg.park_y); // out of the eraser holder
}

// Writing numeral with bx by being the bottom left originpoint. Scale 1 equals a 20 mm high font.
// The structure follows this principle: move to first startpoint of the numeral, lift down, draw numeral, lift up
void number(float bx, float by, int num, float scale) {

  switch (num) {

    case 0:
      drawTo(bx + 12.0 * scale, by + 6.0 * scale);
      lift(cfg.lift_draw);
      bogenGZS(bx + 7.0 * scale, by + 10 * scale, 10.0 * scale, -0.8, 6.7, 0.5);
      lift(cfg.lift_between);
      break;
    case 1:

      //      drawTo(bx + 3 * scale, by + 15 * scale);
      drawTo(bx + 10.0 * scale, by + 20.0 * scale);
      lift(cfg.lift_draw);
      drawTo(bx + 10.0 * scale, by + 0.0 * scale);
      lift(cfg.lift_between);
      break;
    case 2:
      drawTo(bx + 2.0 * scale, by + 12.0 * scale);
      lift(cfg.lift_draw);
      bogenUZS(bx + 8.0 * scale, by + 14.0 * scale, 6.0 * scale, 3, -0.8, 1);
      drawTo(bx + 0.0 * scale, by + 0.0 * scale);
      drawTo(bx + 14.0 * scale, by + 0.0 * scale);
      lift(cfg.lift_between);
      break;
    case 3:
      drawTo(bx + 2.0 * scale, by + 20.0 * scale);
      lift(cfg.lift_draw);
      drawTo(bx + 12.0 * scale, by + 20.0 * scale);
      drawTo(bx + 2.0 * scale, by + 10.0 * scale);

      //      drawTo(bx + 2.0 * scale, by + 17.0 * scale);
      //      lift(cfg.lift_draw);
      //      bogenUZS(bx + 5.0 * scale, by + 15.0 * scale, 5.0 * scale, 3, -2, 1);
      bogenUZS(bx + 5.0 * scale, by + 5.0 * scale, 5.0 * scale, 1.57, -3, 1);
      lift(cfg.lift_between);
      break;
    case 4:
      drawTo(bx + 10.0 * scale, by + 2.0 * scale);
      lift(cfg.lift_draw);
      drawTo(bx + 10.0 * scale, by + 22.0 * scale);
      drawTo(bx + 0.0 * scale, by + 8.0 * scale);
      drawTo(bx + 14.0 * scale, by + 8.0 * scale);
      lift(cfg.lift_between);
      break;
    case 5:
      drawTo(bx + 2.0 * scale, by + 5.0 * scale);
      lift(cfg.lift_draw);
      bogenGZS(bx + 5.0 * scale, by + 6.0 * scale, 6.0 * scale, -2.5, 2, 1);
      drawTo(bx + 3.0 * scale, by + 20.0 * scale);
      drawTo(bx + 12.0 * scale, by + 20.0 * scale);
      lift(cfg.lift_between);
      break;
    case 6:
      drawTo(bx + 2.0 * scale, by + 10.0 * scale);
      lift(cfg.lift_draw);
      bogenUZS(bx + 7.0 * scale, by + 6.0 * scale, 6.0 * scale, 2, -4.4, 1);
      drawTo(bx + 11.0 * scale, by + 20.0 * scale);
      lift(cfg.lift_between);
      break;
    case 7:
      drawTo(bx + 2.0 * scale, by + 20.0 * scale);
      lift(cfg.lift_draw);
      drawTo(bx + 12.0 * scale, by + 20.0 * scale);
      drawTo(bx + 2.0 * scale, by + 0.0);
      lift(cfg.lift_between);
      break;
    case 8:
      drawTo(bx + 5.0 * scale, by + 10.0 * scale);
      lift(cfg.lift_draw);
      bogenUZS(bx + 5.0 * scale, by + 15.0 * scale, 5.0 * scale, 4.7, -1.6, 1);
      bogenGZS(bx + 5.0 * scale, by + 5.0 * scale, 5.0 * scale, -4.7, 2, 1);
      lift(cfg.lift_between);
      break;

    case 9:
      drawTo(bx + 9.0 * scale, by + 11.0 * scale);
      lift(cfg.lift_draw);
      bogenUZS(bx + 7.0 * scale, by + 15.0 * scale, 5.0 * scale, 4, -0.5, 1);
      drawTo(bx + 5.0 * scale, by + 0);
      lift(cfg.lift_between);
      break;

    case 111: // erase

      lift(cfg.lift_erase);
      unpark();

      for (float i = 0; i < ERASE_SWEEPS; i++) {
        drawTo((cfg.start_x - 4.0), (cfg.start_y - 2.0) + ERASE_HEIGHT - i * ERASE_HEIGHT / ERASE_SWEEPS);
        drawTo((cfg.start_x - 4.0), (cfg.start_y - 2.0) + ERASE_HEIGHT - (i + 0.5)* ERASE_HEIGHT / ERASE_SWEEPS);
        drawTo((cfg.start_x - 4.0) + ERASE_WIDTH, (cfg.start_y - 2.0) + ERASE_HEIGHT - (i + 0.5)* ERASE_HEIGHT / ERASE_SWEEPS);
        drawTo((cfg.start_x - 4.0) + ERASE_WIDTH, (cfg.start_y - 2.0) + ERASE_HEIGHT - (i + 1.0)* ERASE_HEIGHT / ERASE_SWEEPS);
      }
      drawTo(cfg.park_x - 3.0, cfg.start_y - 2.0);

      for (float i = 0; i < ERASE_SWEEPS; i++) {
        drawTo((cfg.start_x - 4.0), (cfg.start_y - 2.0) +  i * ERASE_HEIGHT / ERASE_SWEEPS);
        drawTo((cfg.start_x - 4.0), (cfg.start_y - 2.0) +  (i + 0.5)* ERASE_HEIGHT / ERASE_SWEEPS);
        drawTo((cfg.start_x - 4.0) + ERASE_WIDTH, (cfg.start_y - 2.0) +  (i + 0.5)* ERASE_HEIGHT / ERASE_SWEEPS);
        drawTo((cfg.start_x - 4.0) + ERASE_WIDTH, (cfg.start_y - 2.0) +  (i + 1.0)* ERASE_HEIGHT / ERASE_SWEEPS);
      }


      delay (200);
      park();
      lift(cfg.lift_park);

      break;

    case 11:
      drawTo(bx + 5.0 * scale, by + 15.0 * scale);
      lift(cfg.lift_draw);
      bogenGZS(bx + 5.0 * scale, by + 15.0 * scale, 0.1 * scale, 1, -1, 1);
      delay(10);
      lift(cfg.lift_between);
      drawTo(bx + 5.0 * scale, by + 5.0 * scale);
      lift(cfg.lift_draw);
      bogenGZS(bx + 5.0 * scale, by + 5.0 * scale, 0.1 * scale, 1, -1, 1);
      delay(10);
      lift(cfg.lift_between);
      break;

  }
}

void lift(int lift_to) {
  if (servoLift >= lift_to) {
    while (servoLift > lift_to)
    {
      servoLift--;
      servo1.writeMicroseconds(servoLift);
      delayMicroseconds(LIFTSPEED);
    }
  }
  else {
    while (servoLift < lift_to) {
      servoLift++;
      servo1.writeMicroseconds(servoLift);
      delayMicroseconds(LIFTSPEED);
    }
  }
}


void bogenUZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = -0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
           radius * sin(start + count) + by);
    count += inkr;
  }
  while ((start + count) > ende);

}

void bogenGZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = 0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
           radius * sin(start + count) + by);
    count += inkr;
  }
  while ((start + count) <= ende);
}


void drawTo(double pX, double pY) {
  double dx, dy, c;
  int i;

  // dx dy of new point
  dx = pX - lastX;
  dy = pY - lastY;
  //path lenght in mm, times 4 equals 4 steps per mm
  c = floor(7 * sqrt(dx * dx + dy * dy));

  if (c < 1) c = 1;

  for (i = 0; i <= c; i++) {
    // draw line point by point
    set_XY(lastX + (i * dx / c), lastY + (i * dy / c));

  }

  lastX = pX;
  lastY = pY;
}

double return_angle(double a, double b, double c) {
  // cosine rule for angle between c and a
  return acos((a * a + c * c - b * b) / (2 * a * c));
}

void set_XY(double Tx, double Ty)
{
  delay(1);
  double dx, dy, c, a1, a2, Hx, Hy;

  // calculate triangle between pen, servoLeft and arm joint
  // cartesian dx/dy
  dx = Tx - O1X;
  dy = Ty - O1Y;

  // polar length (c) and angle (a1)
  c = sqrt(dx * dx + dy * dy); //
  a1 = atan2(dy, dx); //
  a2 = return_angle(L1, L2, c);

  servo2.writeMicroseconds(floor(((a2 + a1 - M_PI) * cfg.left_factor) + cfg.left_null));

  // calculate joinr arm point for triangle of the right servo arm
  a2 = return_angle(L2, L1, c);
  Hx = Tx + L3 * cos((a1 - a2 + 0.621) + M_PI); //36,5°
  Hy = Ty + L3 * sin((a1 - a2 + 0.621) + M_PI);

  // calculate triangle between pen joint, servoRight and arm joint
  dx = Hx - O2X;
  dy = Hy - O2Y;

  c = sqrt(dx * dx + dy * dy);
  a1 = atan2(dy, dx);
  a2 = return_angle(L1, L4, c);

  servo3.writeMicroseconds(floor(((a1 - a2) * cfg.right_factor) + cfg.right_null));

}
