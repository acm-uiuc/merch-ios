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
  _1F_28_77,
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
  CLEAR_SCREEN,
  USE_MULTIBYTE_CHARS,
  SET_MULTIBYTE_CHARSET,
  DEFINE_CUSTOM_CHAR1,
  DEFINE_CUSTOM_CHAR2,
  DEFINE_CUSTOM_CHAR3,
  DEFINE_CUSTOM_CHAR4,
  DEFINE_CUSTOM_CHAR5,
  SET_COMPOSITION_MODE,
  SET_SCREEN_BRIGHTNESS,
  TIME_WAIT,
  SCROLL_SCREEN1,
  SCROLL_SCREEN2,
  SCROLL_SCREEN3,
  SCROLL_SCREEN4,
  SCROLL_SCREEN5,
  BLINK_SCREEN1,
  BLINK_SCREEN2,
  BLINK_SCREEN3,
  BLINK_SCREEN4,
  SET_FONT_STYLE,
  SET_FONT_SIZE1,
  SET_FONT_SIZE2,
  SET_FONT_SIZE_TALL,
  SELECT_WINDOW0,
  SELECT_WINDOW1,
  SELECT_WINDOW2,
  SELECT_WINDOW3,
  SELECT_WINDOW4,
  DEFINE_WINDOW0,
  DEFINE_WINDOW1,
  DEFINE_WINDOW_X_L,
  DEFINE_WINDOW_X_H,
  DEFINE_WINDOW_Y_L,
  DEFINE_WINDOW_Y_H,
  DEFINE_WINDOW_WIDTH_L,
  DEFINE_WINDOW_WIDTH_H,
  DEFINE_WINDOW_HEIGHT_L,
  DEFINE_WINDOW_HEIGHT_H,
  JOIN_SCREENS,
  CURSOR_ON,
  CURSOR_OFF,
} state_t;

typedef state_t transition_func_t(char code);

state_t do_WAIT(char code);
state_t do_1F(char code);
state_t do_1F_28(char code);
state_t do_1F_28_61(char code);
state_t do_1F_28_66(char code);
state_t do_1F_28_67(char code);
state_t do_1F_28_77(char code);
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
state_t do_CLEAR_SCREEN(char code);
state_t do_USE_MULTIBYTE_CHARS(char code);
state_t do_SET_MULTIBYTE_CHARSET(char code);
state_t do_DEFINE_CUSTOM_CHAR_1(char code);
state_t do_DEFINE_CUSTOM_CHAR_2(char code);
state_t do_DEFINE_CUSTOM_CHAR_3(char code);
state_t do_DEFINE_CUSTOM_CHAR_4(char code);
state_t do_DEFINE_CUSTOM_CHAR_5(char code);
state_t do_SET_COMPOSITION_MODE(char code);
state_t do_SET_SCREEN_BRIGHTNESS(char code);
state_t do_TIME_WAIT(char code);
state_t do_SCROLL_SCREEN1(char code);
state_t do_SCROLL_SCREEN2(char code);
state_t do_SCROLL_SCREEN3(char code);
state_t do_SCROLL_SCREEN4(char code);
state_t do_SCROLL_SCREEN5(char code);
state_t do_BLINK_SCREEN1(char code);
state_t do_BLINK_SCREEN2(char code);
state_t do_BLINK_SCREEN3(char code);
state_t do_BLINK_SCREEN4(char code);
state_t do_SET_FONT_STYLE(char code);
state_t do_SET_FONT_SIZE1(char code);
state_t do_SET_FONT_SIZE2(char code);
state_t do_SET_FONT_SIZE_TALL(char code);
state_t do_SELECT_WINDOW0(char code);
state_t do_SELECT_WINDOW1(char code);
state_t do_SELECT_WINDOW2(char code);
state_t do_SELECT_WINDOW3(char code);
state_t do_SELECT_WINDOW4(char code);
state_t do_DEFINE_WINDOW0(char code);
state_t do_DEFINE_WINDOW1(char code);
state_t do_DEFINE_WINDOW_X_L(char code);
state_t do_DEFINE_WINDOW_X_H(char code);
state_t do_DEFINE_WINDOW_Y_L(char code);
state_t do_DEFINE_WINDOW_Y_H(char code);
state_t do_DEFINE_WINDOW_WIDTH_L(char code);
state_t do_DEFINE_WINDOW_WIDTH_H(char code);
state_t do_DEFINE_WINDOW_HEIGHT_L(char code);
state_t do_DEFINE_WINDOW_HEIGHT_H(char code);
state_t do_JOIN_SCREENS(char code);
state_t do_CURSOR_ON(char code);
state_t do_CURSOR_OFF(char code);

transition_func_t * const transition_table[] = {
  do_WAIT,
  do_1F,
  do_1F_28,
  do_1F_28_61,
  do_1F_28_66,
  do_1F_28_67,
  do_1F_28_77,
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
  do_CLEAR_SCREEN,
  do_USE_MULTIBYTE_CHARS,
  do_SET_MULTIBYTE_CHARSET,
  do_SET_COMPOSITION_MODE,
  do_SET_SCREEN_BRIGHTNESS,
  do_SCROLL_SCREEN1,
  do_SCROLL_SCREEN2,
  do_SCROLL_SCREEN3,
  do_SCROLL_SCREEN4,
  do_SCROLL_SCREEN5,
  do_BLINK_SCREEN1,
  do_BLINK_SCREEN2,
  do_BLINK_SCREEN3,
  do_BLINK_SCREEN4,
  do_SET_FONT_STYLE,
  do_SET_FONT_SIZE1,
  do_SET_FONT_SIZE2,
  do_SET_FONT_SIZE_TALL,
  do_SELECT_WINDOW0,
  do_SELECT_WINDOW1,
  do_SELECT_WINDOW2,
  do_SELECT_WINDOW3,
  do_SELECT_WINDOW4,
  do_DEFINE_WINDOW0,
  do_DEFINE_WINDOW1,
  do_DEFINE_WINDOW_X_L,
  do_DEFINE_WINDOW_X_H,
  do_DEFINE_WINDOW_Y_L,
  do_DEFINE_WINDOW_Y_H,
  do_DEFINE_WINDOW_WIDTH_L,
  do_DEFINE_WINDOW_WIDTH_H,
  do_DEFINE_WINDOW_HEIGHT_L,
  do_DEFINE_WINDOW_HEIGHT_H,
  do_JOIN_SCREENS,
  do_CURSOR_ON,
  do_CURSOR_OFF,
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

bool cursorOn; // I don't know what default value to set this as





state_t do_WAIT(char code) {
  switch(code) {
    case 0x1F:
      return _1F;
    case 0x1B:
      return _1B;
    case 0x0C:
      return CLEAR_SCREEN;
  }
  // Handle the character
  data[cursor.y][cursor.x] = code;
  cursor.x++;
  return WAIT;
}

state_t do_1F(char code) {
  switch(code) {
    case 0x10:
      return SELECT_WINDOW0;
    case 0x11:
      return SELECT_WINDOW1;
    case 0x12:
      return SELECT_WINDOW2;
    case 0x13:
      return SELECT_WINDOW3;
    case 0x14:
      return SELECT_WINDOW4;
    case 0x1F:
      return _1F;
    case 0x24:
      return SET_POS_X_L;
    case 0x77:
      return SET_COMPOSITION_MODE;
    case 0x58:
      return SET_SCREEN_BRIGHTNESS;
    case 0x43:
      return do_1F_43;
  }
  // Unknown sequence
  return WAIT;
}

state_t do_1F_43(char code) {
  switch(code) {
    case 0x01:
      return do_CURSOR_ON;
    case 0x00:
      return do_CURSOR_OFF;
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
    case 0x77:
      return _1F_28_77;
  }
  // Unknown sequence
  return WAIT;
}
state_t do_1F_28_77(char code) {
  switch(code) {
    case 0x02:
      return DEFINE_WINDOW0;
    case 0x10:
      return JOIN_SCREENS;
  }
}
state_t do_1F_28_61(char code) {
  switch(code) {
    case 0x40:
      return SCREENSAVER;
    case 0x01:
      return TIME_WAIT;
    case 0x10:
      return SCROLL_SCREEN1;
    case 0x11:
      return BLINK_SCREEN_1;
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
    case 0x01:
      return SET_FONT_SIZE_TALL;
    case 0x02:
      return USE_MULTIBYTE_CHARS;
    case 0x03:
      return SET_FONT_STYLE;
    case 0x0F:
      return SET_MULTIBYTE_CHARSET;
    case 0x40:
      return FONT_MAGNIFIED_DISPLAY_1;
  }
  return WAIT;
}
state_t do_USE_MULTIBYTE_CHARS(char code) {
  // bool enable = code;
  return WAIT;
}
state_t do_SET_MULTIBYTE_CHARSET(char code) {
  // code = the charset code
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
  // 0 = display off
  // 1 = display on
  // otherwise I'm not sure
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

state_t do_CLEAR_SCREEN(char code) {
  return WAIT;
}
state_t do_DEFINE_CUSTOM_CHAR1(char code) {
  return DEFINE_CUSTOM_CHAR2;
}
state_t do_DEFINE_CUSTOM_CHAR2(char code) {
  return DEFINE_CUSTOM_CHAR3;
}
state_t do_DEFINE_CUSTOM_CHAR3(char code) {
  return DEFINE_CUSTOM_CHAR4;
}
state_t do_DEFINE_CUSTOM_CHAR4(char code) {
  return DEFINE_CUSTOM_CHAR5;
}
state_t do_DEFINE_CUSTOM_CHAR5(char code) {
  return WAIT;
}
state_t do_SET_COMPOSITION_MODE(char code) {
  return WAIT;
}
state_t do_SET_SCREEN_BRIGHTNESS(char code) {
  return WAIT;
}
state_t do_TIME_WAIT(char code) {
  return WAIT;
}
state_t do_SCROLL_SCREEN1(char code) {
  return SCROLL_SCREEN2;
}
state_t do_SCROLL_SCREEN2(char code) {
  return SCROLL_SCREEN3;
}
state_t do_SCROLL_SCREEN3(char code) {
  return SCROLL_SCREEN4;
}
state_t do_SCROLL_SCREEN4(char code) {
  return SCROLL_SCREEN5;
}
state_t do_SCROLL_SCREEN5(char code) {
  return WAIT;
}
state_t do_BLINK_SCREEN1(char code) {
  return BLINK_SCREEN2;
}
state_t do_BLINK_SCREEN2(char code) {
  return BLINK_SCREEN3;
}
state_t do_BLINK_SCREEN3(char code) {
  return BLINK_SCREEN4;
}
state_t do_BLINK_SCREEN4(char code) {
  return WAIT;
}
state_t do_SET_FONT_STYLE(char code) {
  return WAIT;
}
state_t do_SET_FONT_SIZE1(char code) {
  return SET_FONT_SIZE2;
}
state_t do_SET_FONT_SIZE2(char code) {
  return WAIT;
}
state_t do_SET_FONT_SIZE_TALL(char code) {
  return WAIT;
}
state_t do_SELECT_WINDOW0(char code) {
  return WAIT;
}
state_t do_SELECT_WINDOW1(char code) {
  return WAIT;
}
state_t do_SELECT_WINDOW2(char code) {
  return WAIT;
}
state_t do_SELECT_WINDOW3(char code) {
  return WAIT;
}
state_t do_SELECT_WINDOW4(char code) {
  return WAIT;
}

state_t do_DEFINE_WINDOW0(char code) {
  return DEFINE_WINDOW1;
}
state_t do_DEFINE_WINDOW1(char code) {
  return DEFINE_WINDOW_X_L;
}
state_t do_DEFINE_WINDOW_X_L(char code) {
  return DEFINE_WINDOW_X_H;
}
state_t do_DEFINE_WINDOW_X_H(char code) {
  return DEFINE_WINDOW_Y_L;
}
state_t do_DEFINE_WINDOW_Y_L(char code) {
  return DEFINE_WINDOW_Y_H;
}
state_t do_DEFINE_WINDOW_Y_H(char code) {
  return DEFINE_WINDOW_WIDTH_L;
}
state_t do_DEFINE_WINDOW_WIDTH_L(char code) {
  return DEFINE_WINDOW_WIDTH_L;
}
state_t do_DEFINE_WINDOW_WIDTH_H(char code) {
  return DEFINE_WINDOW_HEIGHT_L;
}
state_t do_DEFINE_WINDOW_HEIGHT_L(char code) {
  return DEFINE_WINDOW_HEIGHT_H;
}
state_t do_DEFINE_WINDOW_HEIGHT_H(char code) {
  return WAIT;
}
state_t do_JOIN_SCREENS(char code ) {
  // 0 = separate screens
  // 1 = join screens
  return WAIT;
}

state_t do_CURSOR_ON(char code) {
  cursorOn = True;
  return WAIT;
}
state_t do_CURSOR_OFF(char code) {
  cursorOn = False;
  return WAIT;
}




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

