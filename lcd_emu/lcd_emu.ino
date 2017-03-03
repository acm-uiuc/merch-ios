#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

const int CLK=2;
const int RESET=3;
const int SIGNAL=6;
const int BUSY=7;

#define DIM_Y 2
#define DIM_X 140

enum {
  PIN2Interrupt = 0,
  PIN3Interrupt = 1,
};

void setup() {
  pinMode(CLK, INPUT);
  pinMode(RESET, INPUT);
  pinMode(SIGNAL, INPUT);
  pinMode(BUSY, OUTPUT);
  attachInterrupt(PIN2Interrupt, readbit, RISING);
  attachInterrupt(PIN3Interrupt, do_reset, FALLING);
  Serial.begin(115200);


  // Setup timers to use for timeout
  timer_init();
}

volatile int bufferchar = 0;
volatile char lastChar = 0;
volatile int lastCharValid = 0;
volatile int count = 0;
volatile int reset = 0;

typedef struct {
  uint16_t x;
  uint16_t y;
} Cursor;

Cursor cursor = {0, 0};



typedef enum {
  WAIT,
  _1F,
  _1F_43,
  _1F_72,
  _1F_73,
  _1F_28,
  _1F_28_61,
  _1F_28_66,
  _1F_28_67,
  _1F_28_77,
  _1B,
  _1B_3F,
  _1B_74,
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
  INVERT_ON,
  INVERT_OFF,
  DELETE_CUSTOM_CHAR,
} state_t;

typedef state_t transition_func_t(char code);

state_t do_WAIT(char code);
state_t do_1F(char code);
state_t do_1F_43(char code);
state_t do_1F_72(char code);
state_t do_1F_73(char code);
state_t do_1F_28(char code);
state_t do_1F_28_61(char code);
state_t do_1F_28_66(char code);
state_t do_1F_28_67(char code);
state_t do_1F_28_77(char code);
state_t do_1B(char code);
state_t do_1B_3F(char code);
state_t do_1B_74(char code);
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
state_t do_USE_MULTIBYTE_CHARS(char code);
state_t do_SET_MULTIBYTE_CHARSET(char code);
state_t do_DEFINE_CUSTOM_CHAR1(char code);
state_t do_DEFINE_CUSTOM_CHAR2(char code);
state_t do_DEFINE_CUSTOM_CHAR3(char code);
state_t do_DEFINE_CUSTOM_CHAR4(char code);
state_t do_DEFINE_CUSTOM_CHAR5(char code);
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
state_t do_INVERT_ON(char code);
state_t do_INVERT_OFF(char code);
state_t do_DELETE_CUSTOM_CHAR(char code);

transition_func_t * const transition_table[] = {
  do_WAIT,
  do_1F,
  do_1F_43,
  do_1F_72,
  do_1F_73,
  do_1F_28,
  do_1F_28_61,
  do_1F_28_66,
  do_1F_28_67,
  do_1F_28_77,
  do_1B,
  do_1B_3F,
  do_1B_74,
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
  do_USE_MULTIBYTE_CHARS,
  do_SET_MULTIBYTE_CHARSET,
  do_DEFINE_CUSTOM_CHAR1,
  do_DEFINE_CUSTOM_CHAR2,
  do_DEFINE_CUSTOM_CHAR3,
  do_DEFINE_CUSTOM_CHAR4,
  do_DEFINE_CUSTOM_CHAR5,
  do_SET_COMPOSITION_MODE,
  do_SET_SCREEN_BRIGHTNESS,
  do_TIME_WAIT,
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
  do_INVERT_ON,
  do_INVERT_OFF,
  do_DELETE_CUSTOM_CHAR,
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

uint8_t cursor_on; // I don't know what default value to set this as
uint8_t invert_on;
char horizontal_speed;


void cursor_inc() {
  cursor.x++;
  if(cursor.x >= DIM_X) {
    cursor.y++;
    if(cursor.y >= DIM_Y) {
      cursor.y = DIM_Y-1;
    }
  }
}
void cursor_dec() {
  cursor.x--;
  if (cursor.x < 0) {
    cursor.y--;
    if (cursor.y < 0) {
      cursor.y = 0;
    }
  } 
}
void cursor_newline() {
  cursor.y++;
  if(cursor.y >= DIM_Y) {
    cursor.y = DIM_Y - 1;
  }
}

state_t do_WAIT(char code) {
  switch(code) {
  case 0x1F:
    return _1F;
  case 0x1B:
    return _1B;
  case 0x0C:
    //clears the screen and resets the cursor
    Serial.print("Clear screen\n");
    cursor.x = 0;
    cursor.y = 0;
    memset(&data[0][0], 0, 2*140);
    return WAIT;
  }
  switch(code) {
  case 0x08:
    cursor_dec();
    break;
  case 0x09:
    cursor_inc;
    break;
  case 0x0A:
    cursor_newline();
    break;
  case 0x0B:
    cursor.x = 0;
    cursor.y = 0;
    break;
  case 0x0C:
    cursor.x = 0;
    break;
  }
  if(code >= 0x20 && !(code < 0x80 == 0)){
    // Handle the character
    data[cursor.y][cursor.x] = code;
    cursor.x++;
    if(cursor.x >= DIM_X) {
      cursor.y++;
      if(cursor.y >= DIM_Y) {
	cursor.y = DIM_Y - 1;
      }
    }
    Serial.print(cursor.x);
    Serial.print(",");
    Serial.print(cursor.y);
    Serial.print(" ");
    Serial.print(code);
    Serial.print('\n');
    /* Serial.print(&data[0][0]); */
    /* Serial.print('\n'); */
  }

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
  case 0x28:
    return _1F_28;
  case 0x77:
    return SET_COMPOSITION_MODE;
  case 0x58:
    return SET_SCREEN_BRIGHTNESS;
  case 0x43:
    return _1F_43;
  case 0x73:
    return _1F_73;
  case 0x72:
    return _1F_72;

  }
  // Unknown sequence
  return WAIT;
}

state_t do_1F_43(char code) {
  switch(code) {
  case 0x01:
    return CURSOR_ON;
  case 0x00:
    return CURSOR_OFF;
  }
  // Unknown sequence
  return WAIT;
}


state_t do_1F_72(char code) {
  switch(code) {
  case 0x01:
    return INVERT_ON;
  case 0x00:
    return INVERT_OFF;
  }
  // Unknown sequence
  return WAIT;
}


state_t do_1F_73(char code) {
  horizontal_speed = code;
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
    return BLINK_SCREEN1;
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
  case 0x3F:
    return _1B_3F;
  case 0x40:
    // Initialize display
    Serial.print("Initializing\n");
    return WAIT;
  case 0x52:
    return SPECIFY_INTERNATIONAL_FONT;
  case 0x74:
    return _1B_74;
  }
  return WAIT;
}
state_t do_1B_3F(char code) {
  switch(code) {
  case 0x01:
    return DELETE_CUSTOM_CHAR;
  }
  return WAIT;
}
state_t do_1B_74(char code) {
  //I DONT KNOW WHAT TO DO HERE. OOPS CAPS
  return WAIT;
}
state_t do_SET_POS_X_L(char code) {
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
  Serial.print("cursor set ");
  Serial.print(cursor.x);
  Serial.print(",");
  Serial.print(cursor.y);
  Serial.print("\n");
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
state_t do_JOIN_SCREENS(char code) {
  // 0 = separate screens
  // 1 = join screens
  return WAIT;
}

state_t do_CURSOR_ON(char code) {
  cursor_on = 1;
  return WAIT;
}
state_t do_CURSOR_OFF(char code) {
  cursor_on = 0;
  return WAIT;
}
state_t do_INVERT_ON(char code) {
  invert_on = 1;
  return WAIT;
}
state_t do_INVERT_OFF(char code) {
  invert_on = 0;
  return WAIT;
}
state_t do_DELETE_CUSTOM_CHAR(char code) {

  return WAIT;
}



void handleChar(char code) {
  Serial.print(state);
  Serial.print(" ");
  Serial.print((uint8_t)code, HEX);
  Serial.print("\n");
  state = transition_table[(int)state](code);

} 

void loop() {
  Serial.print("Waiting for reset\n");
  /* noInterrupts(); */
  interrupts();
  while(reset != 1) {}
  Serial.print("Done waiting for reset\n");
  reset = 0;
  while(1) {
    if (lastCharValid == 1) {
      /* digitalWrite(BUSY, HIGH); */
      char toWrite = lastChar;
      lastCharValid = 0;
      /* digitalWrite(BUSY, LOW); */
      /* interrupts(); */
      handleChar(toWrite);
    }
    else if(reset == 1) {
      Serial.print("Got reset\n");
      reset = 0;
    }
  }
}

void do_reset() {
  count = 0;
  lastChar = 0;
  bufferchar = 0;
  lastCharValid = 0;
  reset = 1;
  state = WAIT;
  Serial.println("Got reset");
}

void readbit(){
  int bit = digitalRead(SIGNAL);
  bufferchar = ((bufferchar >> 1) & ~0x80)| (bit << 7);
  if(count == 0) {
    start_recv_timeout();
  }
  count += 1;
  if (count == 8) {
    stop_recv_timeout();
    lastChar = bufferchar;
    count = 0;
    lastCharValid = 1;
  }
}


void timer_init() {
  cli();
  TCCR1A = 0;
  TCCR1B = 0;

  // Turn on CTC (Clear Timer on Compare) mode so that the timer gets cleared
  // when the interrupt is triggered
  TCCR1B |= (1 << WGM12);
  // Set the prescaler to 1024
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);

  // Clear the count register
  TCNT1L = 0;
  TCNT1H = 0;

  // Set the compare match register to count to
  // 16MHz with 1024 prescaler = 15,624 ticks/second
  // 13/15624 = 832 microseconds
  OCR1A = 80;

  // Disable the timer compare interrupt
  TIMSK1 &= ~(1 << OCIE1A);
  sei();
}



void start_recv_timeout() {
  // Clear the current count
  TCNT1L = 0;
  TCNT1H = 0;
  // Enable the timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  digitalWrite(BUSY, HIGH);
}

void stop_recv_timeout() {
  // Disable the timer compare interrupt
  TIMSK1 &= ~(1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  // Timeout on the character being read
  digitalWrite(BUSY, HIGH);
  /* Serial.println("Timed out!"); */
  stop_recv_timeout();
  count = 0;
  lastChar = 0;
  bufferchar = 0;
  lastCharValid = 0;
}
