//#include <Time.h>

// Plotclock
// cc - by Johannes Heberlein 2014
// v 1.02
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


#include <Time.h> // see http://playground.arduino.cc/Code/time 
#include <Servo.h>

// delete or mark the next line as comment if you don't need these
//#define CALIBRATION      // enable calibration mode
//#define REALTIMECLOCK    // enable real time clock

#define HOUR 13
#define MINUTE 35

#define WISHY 6 // Offset of the Y coordinats of the plate-wisher

// When in calibration mode, adjust the following factors until the servos move exactly 90 degrees
#define SERVOFAKTORLEFT 570
#define SERVOFAKTORRIGHT 570

// Zero-position of left and right servo
// When in calibration mode, adjust the NULL-values so that the servo arms are at all times parallel
// either to the X or Y axis
#define SERVOLEFTNULL 1510
#define SERVORIGHTNULL 600

#define SERVOPINLIFT  2
#define SERVOPINLEFT  3
#define SERVOPINRIGHT 4

#define ZOFF 40
// lift positions of lifting servo
#define LIFT0 950+ZOFF // on drawing surface
#define LIFT1 1200+ZOFF  // between numbers
#define LIFT2 1400+ZOFF  // going towards sweeper

// speed of liftimg arm, higher is slower
#define LIFTSPEED 2000

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

#define START_X -4

#define PARK_X 68.0
#define PARK_Y 49 //47.5
#define ERASE_WIDTH 63


#ifdef REALTIMECLOCK
// for instructions on how to hook up a real time clock,
// see here -> http://www.pjrc.com/teensy/td_libs_DS1307RTC.html
// DS1307RTC works with the DS1307, DS1337 and DS3231 real time clock chips.
// Please run the SetTime example to initialize the time on new RTC chips and begin running.

#include <Wire.h>
#include <DS1307RTC.h> // see http://playground.arduino.cc/Code/time    
#endif

int servoLift = 1500;

Servo servo1;  //
Servo servo2;  //
Servo servo3;  //

volatile double lastX = PARK_X;
volatile double lastY = PARK_Y;

int last_min = 0;

void setup()
{

  pinMode(7, INPUT_PULLUP);
#ifdef REALTIMECLOCK
  Serial.begin(9600);
  //while (!Serial) { ; } // wait for serial port to connect. Needed for Leonardo only

  // Set current time only the first to values, hh,mm are needed
  tmElements_t tm;
  if (RTC.read(tm))
  {
    setTime(tm.Hour, tm.Minute, tm.Second, tm.Day, tm.Month, tm.Year);
    Serial.println("DS1307 time is set OK.");
  }
  else
  {
    if (RTC.chipPresent())
    {
      Serial.println("DS1307 is stopped.  Please run the SetTime example to initialize the time and begin running.");
    }
    else
    {
      Serial.println("DS1307 read error!  Please check the circuitry.");
    }
    // Set current time only the first to values, hh,mm are needed



    setTime(HOUR, MINUTE, 0, 0, 0, 0);
  }
#else
  // Set current time only the first to values, hh,mm are needed
    setTime(HOUR, MINUTE, 0, 0, 0, 0);
#endif




  drawTo(PARK_X, PARK_Y);
  lift(LIFT0);
  //servo1.attach(SERVOPINLIFT);  //  lifting servo
  //servo2.attach(SERVOPINLEFT);  //  left servo
  //servo3.attach(SERVOPINRIGHT);  //  right servo
  delay(1000);

}

void loop()
{

#ifdef CALIBRATION

  if (!servo1.attached()) servo1.attach(SERVOPINLIFT);
  if (!servo2.attached()) servo2.attach(SERVOPINLEFT);
  if (!servo3.attached()) servo3.attach(SERVOPINRIGHT);

  // Servohorns will have 90° between movements, parallel to x and y axis
  drawTo(-3, 29.2);
  delay(500);
  drawTo(74.1, 28);
  delay(500);

#else


  int i = 0;
  if (last_min != minute()) {

    if (!servo1.attached()) servo1.attach(SERVOPINLIFT);
    if (!servo2.attached()) servo2.attach(SERVOPINLEFT);
    if (!servo3.attached()) servo3.attach(SERVOPINRIGHT);

    lift(LIFT0);

    delay(1000);

    hour();
    while ((i + 1) * 10 <= hour())
    {
      i++;
    }

    number(3, 3, 111, 1);
    number(START_X, 25, i, 0.9);
    number(START_X + 14, 25, (hour() - i * 10), 0.9);
    number(START_X + 24, 25, 11, 0.9);

    i = 0;
    while ((i + 1) * 10 <= minute())
    {
      i++;
    }
    number(START_X + 29, 25, i, 0.9);
    number(START_X + 43, 25, (minute() - i * 10), 0.9);
    lift(LIFT2);
    drawTo(PARK_X-5, PARK_Y);
   drawTo(PARK_X, PARK_Y);
    lift(LIFT0);
    last_min = minute();
    delay(580);
    servo1.detach();
    servo2.detach();
    servo3.detach();
  }

#endif

}



// Writing numeral with bx by being the bottom left originpoint. Scale 1 equals a 20 mm high font.
// The structure follows this principle: move to first startpoint of the numeral, lift down, draw numeral, lift up
void number(float bx, float by, int num, float scale) {

  switch (num) {

    case 0:
      drawTo(bx + 12 * scale, by + 6 * scale);
      lift(LIFT0);
      bogenGZS(bx + 7 * scale, by + 10 * scale, 10 * scale, -0.8, 6.7, 0.5);
      lift(LIFT1);
      break;
    case 1:

//      drawTo(bx + 3 * scale, by + 15 * scale);
      drawTo(bx + 10 * scale, by + 20 * scale);
      lift(LIFT0);
      drawTo(bx + 10 * scale, by + 0 * scale);
      lift(LIFT1);
      break;
    case 2:
      drawTo(bx + 2 * scale, by + 12 * scale);
      lift(LIFT0);
      bogenUZS(bx + 8 * scale, by + 14 * scale, 6 * scale, 3, -0.8, 1);
      drawTo(bx + 1 * scale, by + 0 * scale);
      drawTo(bx + 14 * scale, by + 0 * scale);
      lift(LIFT1);
      break;
    case 3:
      drawTo(bx + 2 * scale, by + 17 * scale);
      lift(LIFT0);
      bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 3, -2, 1);
      bogenUZS(bx + 5 * scale, by + 5 * scale, 5 * scale, 1.57, -3, 1);
      lift(LIFT1);
      break;
    case 4:
      drawTo(bx + 10 * scale, by + 0 * scale);
      lift(LIFT0);
      drawTo(bx + 10 * scale, by + 20 * scale);
      drawTo(bx + 2 * scale, by + 6 * scale);
      drawTo(bx + 12 * scale, by + 6 * scale);
      lift(LIFT1);
      break;
    case 5:
      drawTo(bx + 2 * scale, by + 5 * scale);
      lift(LIFT0);
      bogenGZS(bx + 5 * scale, by + 6 * scale, 6 * scale, -2.5, 2, 1);
      drawTo(bx + 5 * scale, by + 20 * scale);
      drawTo(bx + 12 * scale, by + 20 * scale);
      lift(LIFT1);
      break;
    case 6:
      drawTo(bx + 2 * scale, by + 10 * scale);
      lift(LIFT0);
      bogenUZS(bx + 7 * scale, by + 6 * scale, 6 * scale, 2, -4.4, 1);
      drawTo(bx + 11 * scale, by + 20 * scale);
      lift(LIFT1);
      break;
    case 7:
      drawTo(bx + 2 * scale, by + 20 * scale);
      lift(LIFT0);
      drawTo(bx + 12 * scale, by + 20 * scale);
      drawTo(bx + 2 * scale, by + 0);
      lift(LIFT1);
      break;
    case 8:
      drawTo(bx + 5 * scale, by + 10 * scale);
      lift(LIFT0);
      bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 4.7, -1.6, 1);
      bogenGZS(bx + 5 * scale, by + 5 * scale, 5 * scale, -4.7, 2, 1);
      lift(LIFT1);
      break;

    case 9:
      drawTo(bx + 9 * scale, by + 11 * scale);
      lift(LIFT0);
      bogenUZS(bx + 7 * scale, by + 15 * scale, 5 * scale, 4, -0.5, 1);
      drawTo(bx + 5 * scale, by + 0);
      lift(LIFT1);
      break;

    case 111:

      lift(LIFT0);
      drawTo(PARK_X - 5, PARK_Y);
      //drawTo(START_X+ERASE_WIDTH-WISHY, 43);

      drawTo(START_X + ERASE_WIDTH - WISHY, 46);
      drawTo(START_X, 49);
      drawTo(START_X, 46);
      drawTo(START_X + ERASE_WIDTH - WISHY, 46);
      drawTo(START_X + ERASE_WIDTH - WISHY, 42);

      drawTo(START_X, 42);
      drawTo(START_X, 38);
      drawTo(START_X + ERASE_WIDTH - WISHY, 38);
      drawTo(START_X + ERASE_WIDTH - WISHY, 34);

      drawTo(START_X, 34);
      drawTo(START_X, 29);
      drawTo(8+START_X + ERASE_WIDTH - WISHY, 26);

      drawTo(START_X, 26);
      drawTo(60 - WISHY, 40);

      drawTo(START_X + ERASE_WIDTH - WISHY, 46);
      drawTo(START_X, 49);
      drawTo(START_X, 46);
      drawTo(START_X + ERASE_WIDTH - WISHY, 46);
      drawTo(START_X + ERASE_WIDTH - WISHY, 42);

      drawTo(START_X, 42);
      drawTo(START_X, 38);
      drawTo(START_X + ERASE_WIDTH - WISHY, 38);
      drawTo(START_X + ERASE_WIDTH - WISHY, 34);

      drawTo(START_X, 34);
      drawTo(START_X, 29);
      drawTo(START_X, 29);
      drawTo(START_X + ERASE_WIDTH - WISHY, 26);

      drawTo(START_X, 26);
      drawTo(60 - WISHY, 40);

      drawTo(PARK_X+7, PARK_Y);
      drawTo(PARK_X, PARK_Y);
      //drawTo(73.2, 44.0);
      lift(LIFT2);

      break;

    case 11:
      drawTo(bx + 5 * scale, by + 15 * scale);
      lift(LIFT0);
      bogenGZS(bx + 5 * scale, by + 15 * scale, 0.1 * scale, 1, -1, 1);
      delay(10);
      lift(LIFT1);
      drawTo(bx + 5 * scale, by + 5 * scale);
      lift(LIFT0);
      bogenGZS(bx + 5 * scale, by + 5 * scale, 0.1 * scale, 1, -1, 1);
      delay(10);
      lift(LIFT1);
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

  // polar lemgth (c) and angle (a1)
  c = sqrt(dx * dx + dy * dy); //
  a1 = atan2(dy, dx); //
  a2 = return_angle(L1, L2, c);

  servo2.writeMicroseconds(floor(((a2 + a1 - M_PI) * SERVOFAKTORLEFT) + SERVOLEFTNULL));

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

  servo3.writeMicroseconds(floor(((a1 - a2) * SERVOFAKTORRIGHT) + SERVORIGHTNULL));

}
