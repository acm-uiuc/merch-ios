void setup() {
  Serial.begin(115200);
}

void loop() {
  int v0 = analogRead(0);
  int v1 = analogRead(1);
  int v2 = analogRead(2);
  
  Serial.print(v0);
  Serial.print("\0");
  Serial.print(v1);
  Serial.print("\0");
  Serial.print(v2);
  Serial.print("\0");
}
