const int CLK=2;
const int RESET=3;
const int SIGNAL=6;
const int BUSY=7;

void setup() {
    pinMode(CLK, INPUT_PULLUP);
    pinMode(RESET,INPUT_PULLUP);
    pinMode(SIGNAL, INPUT);
    pinMode(BUSY, OUTPUT);
    attachInterrupt(CLK, readbit, RISING);
    attachInterrupt(RESET, do_reset, FALLING);
    Serial.begin(9600);  
}

char bufferchar = 0;
char lastChar = 0;
int lastCharValid = 0;
int count = 0;

void readbit(){
  int bit = digitalRead(SIGNAL);
  bufferchar = (bufferchar << 1) | (bit&1);
  count += 1;
  if (count == 8) {
    lastChar = bufferchar;
    count = 0;
    lastCharValid = 1;
  }
}

void loop() {
    noInterrupts();
    if (lastCharValid == 1) {
      char toWrite = lastChar;
      lastCharValid = 0;
      interrupts();
      Serial.write(toWrite);
    }
    interrupts();
}

void do_reset(){
  count = 0;
  lastChar = 0;
  bufferchar = 0;
  lastCharValid = 0; 
}

