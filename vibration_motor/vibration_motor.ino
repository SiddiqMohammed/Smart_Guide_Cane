int motorPin = 3; //motor transistor is connected to pin 3
int n_pulse = 3;
int pulse_strength = 0;

void setup()
{
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("<Arduino Ready>");
}

void loop()
{
  vibrate(3, 80);
  delay(1000);
  vibrate(3, 120);
  delay(1000);
  vibrate(3, 200);
  delay(1000);
  vibrate(4, 250);
  delay(1000);
}

void vibrate(int n_pulse, int pulse_strength) {
  Serial.println(pulse_strength);
  for (int x = 0; x < n_pulse; x++) {
    analogWrite(motorPin, pulse_strength); // set strength

    // lower strength needs longer pulses and higher one's need shorter pulses
    delay(400 - pulse_strength);

    // TODO: make the pulse_delay a function of both strength and n_pulses
    // Use something like --> delay(pulse_delay);

    // TODO: add n_motors and run 'n' amount of motors based on pulse_strength

    digitalWrite(motorPin, LOW);  //stop vibrating
    delay(100); //wait 50 seconds.
  }
}
