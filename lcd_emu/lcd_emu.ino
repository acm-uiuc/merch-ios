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
  _1B,
  INITIALIZE,
  SET_POS_X_L,
  SET_POS_X_H,
  SET_POS_Y_L,
  SET_POS_Y_H,
} State;

typedef struct {
  uint16_t x;
  uint16_t y;
} Cursor;

Cursor cursor = {0, 0};

State state = WAIT;


char data[2][50] = {0};

void HandleChar(char code) {
  /*Serial.print(code, HEX);
  Serial.print(' ');
  Serial.print(state);
  Serial.print('\n');*/
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
          /*Serial.println("Print ");
          Serial.print(code);
          Serial.print(" at (");
          Serial.print(cursor.x);
          Serial.print(", ");
          Serial.print(cursor.y);
          Serial.print(")\n");*/
          
          break;
      }
      break;
    case _1F:
      switch(code) {
        case 0x24:
          state = SET_POS_X_L;
          data[cursor.y][cursor.x] = '\0';
          break;
        case 0x02:
          break;
        case 0x28:
          break;
        case 0x58:
          break;
      }
      break;
    case _1B:
      break;
    case INITIALIZE:
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
      /*Serial.print("Moved cursor to (");
      Serial.print(cursor.x);
      Serial.print(", ");
      Serial.print(cursor.y);
      Serial.print(")\n");*/
      Serial.print(data[0]);
      Serial.print('\n');
      break;
  }
}
void loop() {
    noInterrupts();
    if (lastCharValid == 1) {      
      digitalWrite(BUSY, HIGH);
      char toWrite = lastChar;
      lastCharValid = 0;
      interrupts();
      HandleChar(toWrite);
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

