#include <stdint.h>
const int CLK=2;
const int RESET=3;
const int SIGNAL=6;
const int BUSY=7;


enum {
  PIN2Interrupt = 0,
  PIN3Interrupt = 1,
};

void setup() {
    pinMode(CLK, INPUT_PULLUP);
    pinMode(RESET,INPUT_PULLUP);
    pinMode(SIGNAL, INPUT);
    pinMode(BUSY, OUTPUT);
    attachInterrupt(PIN2Interrupt, readbit, RISING);
    attachInterrupt(PIN3Interrupt, do_reset, FALLING);
    Serial.begin(115200);
}

int bufferchar = 0;
char lastChar = 0;
int lastCharValid = 0;
int count = 0;
int reset = 0;

typedef struct {
  uint16_t x;
  uint16_t y;
} Cursor;

Cursor cursor = {0, 0};



void readbit(){
  int bit = digitalRead(SIGNAL);
  bufferchar = (bufferchar >> 1) | (bit << 7);
  count += 1;
  if (count == 8) {
    lastChar = bufferchar;
    count = 0;
    lastCharValid = 1;
  }
}

typedef enum {
  WAIT,
  _1F,
  _1F_28,
  _1F_28_61,
  _1F_28_66,
  _1F_28_67,
  _1B,
  SET_POS_X_L,
  SET_POS_X_H,
  SET_POS_Y_L,
  SET_POS_Y_H,
  SET_BRIGHTNESS,
  SCREENSAVER,
  DISPLAY_BLINK_1,
  DISPLAY_BLINK_2,
  DISPLAY_BLINK_3,
  DISPLAY_BLINK_4,
  BIT_IMAGE_DISPLAY_GROUP_1,
  BIT_IMAGE_DISPLAY_GROUP_2,
  FONT_MAGNIFIED_DISPLAY_1,
  FONT_MAGNIFIED_DISPLAY_2,
  SPECIFY_INTERNATIONAL_FONT,
} state_t;

#define NUM_STATES 24


typedef state_t transition_func_t(char code);

state_t do_WAIT(char code);
state_t do_1F(char code);
state_t do_1F_28(char code);
state_t do_1F_28_61(char code);
state_t do_1F_28_66(char code);
state_t do_1F_28_67(char code);
state_t do_1B(char code);
state_t do_SET_POS_X_L(char code);
state_t do_SET_POS_X_H(char code);
state_t do_SET_POS_Y_L(char code);
state_t do_SET_POS_Y_H(char code);
state_t do_SET_BRIGHTNESS(char code);
state_t do_SCREENSAVER(char code);
state_t do_DISPLAY_BLINK_1(char code);
state_t do_DISPLAY_BLINK_2(char code);
state_t do_DISPLAY_BLINK_3(char code);
state_t do_DISPLAY_BLINK_4(char code);
state_t do_BIT_IMAGE_DISPLAY_GROUP_1(char code);
state_t do_BIT_IMAGE_DISPLAY_GROUP_2(char code);
state_t do_FONT_MAGNIFIED_DISPLAY_1(char code);
state_t do_FONT_MAGNIFIED_DISPLAY_2(char code);
state_t do_SPECIFY_INTERNATIONAL_FONT(char code);

transition_func_t * const transition_table[] = {
  do_WAIT,
  do_1F,
  do_1F_28,
  do_1F_28_61,
  do_1F_28_66,
  do_1F_28_67,
  do_1B,
  do_SET_POS_X_L,
  do_SET_POS_X_H,
  do_SET_POS_Y_L,
  do_SET_POS_Y_H,
  do_SET_BRIGHTNESS,
  do_SCREENSAVER,
  do_DISPLAY_BLINK_1,
  do_DISPLAY_BLINK_2,
  do_DISPLAY_BLINK_3,
  do_DISPLAY_BLINK_4,
  do_BIT_IMAGE_DISPLAY_GROUP_1,
  do_BIT_IMAGE_DISPLAY_GROUP_2,
  do_FONT_MAGNIFIED_DISPLAY_1,
  do_FONT_MAGNIFIED_DISPLAY_2,
  do_SPECIFY_INTERNATIONAL_FONT,
};

state_t state = WAIT;
char brightness = 0;
char font = 0;
char screensaver = 0;

char blink_enable_reverse = 0;
char blink_ontime = 0;
char blink_offtime = 0;
char blink_times = 0;

uint16_t display_group = 0;

char font_magnified_display_x = 0;
char font_magnified_display_y = 0;

char data[2][140] = {0};





state_t do_WAIT(char code) {
  switch(code) {
    case 0x1F:
      return _1F;
    case 0x1B:
      return _1B;
  }
  // Handle the character
  data[cursor.y][cursor.x] = code;
  cursor.x++;
  return WAIT;
}

state_t do_1F(char code) {
  switch(code) {
    case 0x1F:
      return _1F;
    case 0x24:
      return SET_POS_X_L;
  }
  // Unknown sequence
  return WAIT;
}
state_t do_1F_28(char code) {
  switch(code) {
    case 0x61:
      return _1F_28_61;
    case 0x66:
      return _1F_28_66;
    case 0x67:
      return _1F_28_67;
  }
  // Unknown sequence
  return WAIT;
}
state_t do_1F_28_61(char code) {
  switch(code) {
    case 0x40:
      return SCREENSAVER;
  }
  return WAIT;
}
state_t do_1F_28_66(char code) {
  switch(code) {
    case 0x11:
      return BIT_IMAGE_DISPLAY_GROUP_1;
  }
  return WAIT;
}
state_t do_1F_28_67(char code) {
  switch(code) {
    case 0x40:
      return FONT_MAGNIFIED_DISPLAY_1;
  }
  return WAIT;
}
state_t do_1B(char code) {
  switch(code) {
    case 0x40:
      // Initialize display
      do_reset();
      return WAIT;
    case 0x52:
      return SPECIFY_INTERNATIONAL_FONT;
  }
  return WAIT;
}
state_t do_SET_POS_X_L(char code) {
  data[cursor.y][cursor.x] = '\0';
  Serial.print(data[cursor.y]);
  cursor.x = 0;
  cursor.x |= (uint16_t)code & 0xFF;
  return SET_POS_X_H;
}
state_t do_SET_POS_X_H(char code) {
  cursor.x |= (uint16_t)code << 8;
  return SET_POS_Y_L;
}
state_t do_SET_POS_Y_L(char code) {
  cursor.y = (uint16_t)code & 0xFF;
  return SET_POS_Y_H;
}
state_t do_SET_POS_Y_H(char code) {
  cursor.y |= (uint16_t)code << 8;
  Serial.print("cursor set\n");
  return WAIT;
}
state_t do_SET_BRIGHTNESS(char code) {
  brightness = code;
  return WAIT;
}
state_t do_SCREENSAVER(char code) {
  screensaver = code;
  return WAIT;
}
state_t do_DISPLAY_BLINK_1(char code) {
  blink_enable_reverse = code;
  return DISPLAY_BLINK_2;
}
state_t do_DISPLAY_BLINK_2(char code) {
  blink_ontime = code;
  state = DISPLAY_BLINK_3;
}
state_t do_DISPLAY_BLINK_3(char code) {
  blink_ontime = code;
  return DISPLAY_BLINK_4;
}
state_t do_DISPLAY_BLINK_4(char code) {
  blink_times = code;
  return WAIT;
}
state_t do_BIT_IMAGE_DISPLAY_GROUP_1(char code) {
  display_group = (uint16_t)code & 0xFF;
  return BIT_IMAGE_DISPLAY_GROUP_2;
}
state_t do_BIT_IMAGE_DISPLAY_GROUP_2(char code) {
  display_group |= (uint16_t)code << 8;
  return WAIT;
}
state_t do_FONT_MAGNIFIED_DISPLAY_1(char code) {
  font_magnified_display_x = code;
  return FONT_MAGNIFIED_DISPLAY_2;
}
state_t do_FONT_MAGNIFIED_DISPLAY_2(char code) {
  font_magnified_display_y = code;
  return WAIT;
}
state_t do_SPECIFY_INTERNATIONAL_FONT(char code) {
  font = code;
  return WAIT;
}



/*void handleChar(char code) {
  Serial.print(code & 0xFF, HEX);
  Serial.print(' ');
  Serial.print(state);
  Serial.print('\n');
  switch(state) {
    case WAIT:
      switch(code) {
        case 0x1F:
          state = _1F;
          break;
        case 0x1B:
          state = _1B;
          break;
        default:
          // Write the character to display
          data[cursor.y][cursor.x] = code;
          cursor.x++;
          Serial.print(code);
          Serial.print('\n');
          Serial.println("Print ");
          Serial.print(code);
          Serial.print(" at (");
          Serial.print(cursor.x);
          Serial.print(", ");
          Serial.print(cursor.y);
          Serial.print(")\n");
          break;
      }
      break;
    case _1F:
      switch(code) {
        case 0x1F:
          state = _1F;
          break;
        case 0x24:
          state = SET_POS_X_L;
          data[cursor.y][cursor.x] = '\0';
          break;
        case 0x02:
          state = WAIT;
          Serial.print("Vertical Scroll mode");
          break;
        case 0x03:
          state = WAIT;
          Serial.print("Horizontal Scroll mode");
          break;
        case 0x28:
          state = _1F_28;
          break;
        case 0x58:
          state = SET_BRIGHTNESS;
          Serial.print("Set brightness ");
          Serial.print(code);
          Serial.print('\n');
          break;
        default:
          state = WAIT;
          break;
      }
      break;
    case _1F_28:
      switch(code) {
        case 0x61:
          state = _1F_28_61;
          break;
        case 0x66:
          state = _1F_28_66;
          break;
        case 0x67:
          state = _1F_28_67;
          break;
        default:
          state = WAIT;
          break;
      }
    case _1F_28_61:
      switch(code) {
        case 0x40:
          // Screen saver
          state = SCREENSAVER;
          Serial.print("Screensaver mode");
          break;
        default:
          state = WAIT;
          break;
      }
      break;
    case _1F_28_66:
      switch(code) {
        case 0x11:
          state = BIT_IMAGE_DISPLAY_GROUP_1;
          break;
        default:
          state = WAIT;
          break;
      }
      break;
    case _1F_28_67:
      switch(code) {
        case 0x40:
          state = FONT_MAGNIFIED_DISPLAY_1;
          break;
        default:
          state = WAIT;
          break;
      }
      break;
    case _1B:
      switch(code) {
        case 0x40:
          // Do initialize display
          do_reset();
          state = WAIT;
          break;
        case 0x52:
          state = SPECIFY_INTERNATIONAL_FONT;
          break;
        default:
          state = WAIT;
          break;
      }
      break;
    case SPECIFY_INTERNATIONAL_FONT:
      font = code;
      state = WAIT;
      break;
    case SCREENSAVER:
      screensaver = code;
      state = WAIT;
      break;
    case SET_BRIGHTNESS:
      brightness = code;
      state = WAIT;
      break;
    case DISPLAY_BLINK_1:
      blink_enable_reverse = code;
      state = DISPLAY_BLINK_2;
      break;
    case DISPLAY_BLINK_2:
      blink_ontime = code;
      state = DISPLAY_BLINK_3;
      break;
    case DISPLAY_BLINK_3:
      blink_ontime = code;
      state = DISPLAY_BLINK_4;
      break;
    case DISPLAY_BLINK_4:
      blink_times = code;
      state = WAIT;
      break;
    case BIT_IMAGE_DISPLAY_GROUP_1:
      display_group = (uint16_t)code & 0xFF;
      state = BIT_IMAGE_DISPLAY_GROUP_2;
      break;
    case BIT_IMAGE_DISPLAY_GROUP_2:
      display_group |= (uint16_t)code << 8;
      state = WAIT;
      break;
    case FONT_MAGNIFIED_DISPLAY_1:
      font_magnified_display_x = code;
      state = FONT_MAGNIFIED_DISPLAY_2;
      break;
    case FONT_MAGNIFIED_DISPLAY_2:
      font_magnified_display_y = code;
      state = WAIT;
      break;


    case SET_POS_X_L:
      cursor.x = 0;
      cursor.x |= (uint16_t)code & 0xFF;
      state = SET_POS_X_H;
      break;
    case SET_POS_X_H:
      cursor.x |= (uint16_t)code << 8;
      state = SET_POS_Y_L;
      break;
    case SET_POS_Y_L:
      cursor.y = (uint16_t)code & 0xFF;
      state = SET_POS_Y_H;
      break;
    case SET_POS_Y_H:
      cursor.y |= (uint16_t)code << 8;
      state = WAIT;
      Serial.print(data[0]);
      Serial.print("Moved cursor to (");
      Serial.print(cursor.x);
      Serial.print(", ");
      Serial.print(cursor.y);
      Serial.print(")\n");
      break;
  }
}*/


void handleChar(char code) {
  state = transition_table[(int)state](code);
}

void loop() {
    noInterrupts();
    if (lastCharValid == 1) {
      digitalWrite(BUSY, HIGH);
      char toWrite = lastChar;
      lastCharValid = 0;
      handleChar(toWrite);
      interrupts();
      digitalWrite(BUSY, LOW);
    }
    else if(reset == 1) {
      Serial.print("Got reset\n");
      reset = 0;
    }
    interrupts();
}

void do_reset(){
  count = 0;
  lastChar = 0;
  bufferchar = 0;
  lastCharValid = 0;
  reset = 1;
  state = WAIT;
}

