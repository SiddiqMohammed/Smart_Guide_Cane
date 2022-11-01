int red = 6;
int blue = 3;
int green = 5;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(red, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  ledStuff(green);
  ledStuff(blue);
  ledStuff(red);
}

void ledStuff(int x) {

  digitalWrite(x, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(x, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

}
