#define LAST_STATE 6

int i_state;

// this is called just when changing the adjust
void setAdjust(boolean m) {
  adjust = m;
  if (adjust == true) {
    lift(cfg.lift_park);
    attachAll();

    setState(0);  // first state of the interactive mode
  }
  else {
    lift(cfg.lift_park);
    park();
    lift(cfg.lift_draw);

    OPERATION_MSG();

  }
}

void checkSerial() {
  if (Serial.available() > 0) {
    char incoming = Serial.read();
    Serial.print(incoming);
    Serial.print(F(" "));

    if (incoming == 'h' || incoming == 'H') {
      setHour();
      return;
    }

    if (adjust == true) { // interactive adjust
      interactive(incoming);
    }
    else { // operation
      if (incoming == 'i' || incoming == 'I') {
        setAdjust(true);
      }
      Serial.println();
    }
  }
}

inline void setHour() {
  SET_HOUR_MSG();
  int h, m;
  for (int i = 0; i < 4; i++) {
    int incoming  = -1;
    while (!(incoming >= '0' && incoming <= '9')) {
      incoming  = Serial.read();
    }
    Serial.print((char)incoming);
    incoming -= '0';
    switch (i) {
      case 0:
        h = incoming * 10;
        break;
      case 1:
        h += incoming;
        break;
      case 2:
        m = incoming * 10;
        break;
      case 4:
        m += incoming;
        break;
    }
  }
  h = h % 24;
  m = m % 60;
  last_min = -1;
  Serial.println();
  Serial.print(h);
  Serial.print(':');
  Serial.println(m);
  setTime(h, m, 0, 0, 0, 0);
}

inline void interactive(char incoming) {
  switch (incoming) {
    case 'o':
    case 'O':
      //go back to operation
      setAdjust(false);
      return;
      break;

    case 'p':
    case 'P':
      saveConfig(cfg_addr, cfg);
      setAdjust(false);
      return;
      break;

    case '*':
    case 'n':
    case 'N':
      setState(++i_state);
      return;
      break;
    case '/':
    case 'b':
    case 'B':
      setState(--i_state);
      return;
      break;
  }
  switch (i_state) {
    case 1:
      adjVar(incoming, "left_null: ", cfg.left_null, -5);
      drawUpdate();
      break;
    case 2:
      adjVar(incoming, "left_factor: ", cfg.left_factor, +5);
      drawUpdate();
      break;
    case 3:
      adjVar(incoming, "right_null: ", cfg.right_null, +5);
      drawUpdate();
      break;
    case 4:
      adjVar(incoming, "right_factor: ", cfg.right_factor, -5);
      drawUpdate();
      break;
    case 5:
      adjVar(incoming, "lift_draw: ", cfg.lift_draw, +5);
      lift(cfg.lift_draw);
      cfg.lift_between = cfg.lift_draw + 300;
      cfg.lift_erase = cfg.lift_draw;
      cfg.lift_park = cfg.lift_draw + 500;

      if (incoming == '7' || incoming == 'q') {
        drawTo(cfg.start_x, cfg.start_y + 20 * 0.9);
        drawTo(cfg.start_x + 50, cfg.start_y + 20 * 0.9);
        drawTo(cfg.start_x + 50, cfg.start_y);
        drawTo(cfg.start_x, cfg.start_y);
      }
      break;
    case 6:
      adjPark(incoming);
      break;
  } // switch (i_stage)
}

void adjVar(char incoming, char* head, int &var, int increment) {
  switch (incoming) {
    case '+':
    case '6':
    case '8':
    case 'd':
    case 'w':
      break;
    case '-':
    case '4':
    case '2':
    case 'a':
    case 's':
      increment = -increment;
      break;
    default: return;
  }
  var += increment;
  Serial.print(head);
  Serial.println(var);
}

void adjPark(char incoming) {
  switch (incoming) {
    case '8':
    case 'w':
    case 'W':
      cfg.park_y += 0.25;
      break;
    case '2':
    case 's':
    case 'S':
      cfg.park_y -= 0.25;
      break;
    case '6':
    case 'd':
    case 'D':
      cfg.park_x += 0.25;
      break;
    case '4':
    case 'a':
    case 'A':
      cfg.park_x -= 0.25;
      break;
    case '7':
    case 'q':
    case 'Q':
      lift(cfg.lift_park);
      break;
    case '1':
    case 'z':
    case 'Z':
      lift(cfg.lift_draw);
      break;
    default:
      return;
  }
  Serial.println();
  drawTo(cfg.park_x, cfg.park_y);
  //debug
  //  Serial.print(F("("));
  //  Serial.print(cfg.park_x);
  //  Serial.print(F(","));
  //  Serial.print(cfg.park_y);
  //  Serial.println(F(")"));
}

// this is called just when changing interactive state
void setState(int s) {
  if (s < 1 || s > LAST_STATE) {
    INTERACTIVE_MSG();
    s = 1;
  }

  i_state = s;
  //debug
  //  Serial.print(F("Interactive state: "));
  //  Serial.println(i_state);

  lift(cfg.lift_park);

  switch (i_state) {
    case 1:
      LEFT_X_MSG();
      drawTo(-3, 29.2);
      break;
    case 2:
      LEFT_Y_MSG();
      drawTo(74.1, 28);
      break;
    case 3:
      RIGHT_X_MSG();
      drawTo(74.1, 28);
      break;
    case 4:
      RIGHT_Y_MSG();
      drawTo(-3, 29.2);
      break;
    case 5:
      PEN_ADJ_MSG();
      drawTo(cfg.start_x, cfg.start_y);
      break;
    case 6:
      PARK_ADJ();
      park();
      break;
    default:
      setAdjust(1);
      break;
  }
  lift(cfg.lift_draw);
}

// used to update the position if some config changes
void drawUpdate() {
  drawTo(lastX + 0.25, lastY + 0.25);
  drawTo(lastX - 0.25, lastY - 0.25);
}

