#ifndef CFG_H
#define CFG_H

//#include <EEPROM.h>
#include "EEPROM.h"

struct Config {
  float version; // Used to check if the data stored is compatible
  bool rtc; // true to enable RTC
  
  // adjustments for calibration mode:
  // _null -> make the arms parallel to x or y axis
  // _factor ->make the arm movement  90 degree wide
  int left_null;
  int left_factor;
  int right_null;
  int right_factor;

  // coordinates of the eraser holder
  float park_x;
  float park_y;

  // coordinates of the eraser holder
  float start_x;
  float start_y;

  // positions of the lifting arm
  int lift_draw;
  int lift_between;
  int lift_erase;
  int lift_park;


  // set defaults
  Config():
    rtc(false),
    left_null(1940),
    left_factor(595),
    right_null(1180),
    right_factor(560),
    park_x(67.0),
    park_y(42.75),
    start_x(-3.0),
    start_y(25.0),
    lift_draw(670),
    lift_between(1000),
    lift_erase(680),
    lift_park(1200),

    // finalizing defaults
    version (VERSION)
  {}
};

void dumpConfig(Config c) {
  Serial.print(F(" .version: "));
  Serial.println(c.version);
  Serial.print(F(" .rtc: "));
  Serial.println(c.rtc);
  Serial.print(F(" .left_null: "));
  Serial.println(c.left_null);
  Serial.print(F(" .left_factor: "));
  Serial.println(c.left_factor);
  Serial.print(F(" .right_null: "));
  Serial.println(c.right_null);
  Serial.print(F(" .right_factor: "));
  Serial.println(c.right_factor);
  Serial.print(F(" .park_x: "));
  Serial.println(c.park_x);
  Serial.print(F(" .park_y: "));
  Serial.println(c.park_y);

  Serial.print(F(" .start_x: "));
  Serial.println(c.start_x);
  Serial.print(F(" .start_y: "));
  Serial.println(c.start_y);

  Serial.print(F(" .lift_draw: "));
  Serial.println(c.lift_draw);
  Serial.print(F(" .lift_between: "));
  Serial.println(c.lift_between);
  Serial.print(F(" .lift_erase: "));
  Serial.println(c.lift_erase);
  Serial.print(F(" .lift_park: "));
  Serial.println(c.lift_park);

  Serial.println(F("______________"));
  Serial.print(F("bytes: "));
  Serial.println(sizeof(c));
  Serial.println();
}


#define saveConfig(address, c)   EEPROM.put(address, c)

#define validConfig(c) (c.version == VERSION)

void readConfig(int address, Config &c) {
  EEPROM.get(address, c);
  if (!validConfig(c)) {
    c = Config();
  }
}


#endif // #ifndef CFG_H