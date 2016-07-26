#ifndef TRANSLATION

#define TRANSLATION "EN"

#define CREDITS() {\
    Serial.println(F("Pandora Lab's PlotClock"));\
    Serial.println(F("Original code by Johannes Heberlein 2014"));\
    Serial.println(F("based on version 1.03"));\
    Serial.println();\
    Serial.println(F("Interactive calibration and modifications by"));\
    Serial.println(F("Mau Jabur - maumaker -at- pandoralab -dot- com -dot- br"));\
    Serial.print(F("Version "));\
    Serial.print(VERSION);\
    Serial.print(F(" "));\
    Serial.print(TRANSLATION);\
    Serial.println();\
    Serial.println(F("it may take a while to recognize commands."));\
    Serial.println(F("The clock has to finish the current task"));\
  }

#define RTC_OK(){\
    Serial.println(F("DS1307 time is set OK."));\
  }
#define RTC_STOPPED(){\
    Serial.println(F("DS1307 is stopped."));\
  }

#define RTC_MISSING(){\
    Serial.println(F("DS1307 not found."));\
  }

#define OPERATION_MSG() {\
    Serial.println(F("\nOPERATION"));\
    Serial.println(F("send [I] to enter interactive adjust"));\
  }

#define SET_HOUR_MSG() {\
Serial.println(F("\nSET HOUR"));\
  }

#define INTERACTIVE_MSG() {\
    Serial.println();\
    Serial.println(F("INTERACTIVE ADJUST"));\
    Serial.println(F("[N] advance to the NEXT adjust"));\
    Serial.println(F("[B] go BACK to the previous adjust"));\
    Serial.println(F("[O] OPERATION mode"));\
    Serial.println(F("[P] to SAVE and return to OPERATION mode"));\
  }

#define LEFT_X_MSG(){\
    Serial.println(F("\n[W],[S] align LEFT arm to X axis"));\
  }

#define LEFT_Y_MSG(){\
    Serial.println(F("\n[A],[D] align LEFT arm to Y axis"));\
  }

#define RIGHT_X_MSG(){\
    Serial.println(F("\n[W],[S] align RIGHT arm to X axis"));\
  }

#define RIGHT_Y_MSG(){\
    Serial.println(F("\n[A],[D] align RIGHT arm to Y axis"));\
  }

#define PEN_ADJ_MSG(){\
    Serial.println(F("\n[W],[S] adjust pen on surface"));\
    Serial.println(F("[Q] draw a test"));\
  }

#define PARK_ADJ(){\
    Serial.println(F("\n[W],[A],[S],[D] adjust pen on eraser"));\
    Serial.println(F("[Q],[Z] pen up/down"));\
  }

#endif //TRANSLATION