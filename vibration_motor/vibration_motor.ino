/*
  The vibration motor is enabled and disabled with different intensities and for different time durations.
*/

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
  // Random Number Generator
  sonar_dist = rand(0, 300);
  
  // Get pulse strength
  pulse_strength = map(sonar_dist, 0, 300, 80, 255);
    
  // Depending on values, give a certain number of pulses and forward the pulse_strength
  if (pulse_strength > 250);
  {
    vibrate_old(4, pulse_strength);
  }
  else
    vibrate_old(3, pulse_strength);
  
  // TODO: Create a similar function as vibrate_old() but make it without delay
  // Currently the vibrate_old function uses delays and this blocks the code
  // update it so that it runs concurrently with the rest of the code
  // HINT: use millis() function.

  // vibrate();


  vibrate_old(3, 80);
  delay(1000);
  vibrate_old(3, 120);
  delay(1000);
  vibrate_old(3, 200);
  delay(1000);
  vibrate_old(4, 250);
  delay(1000);

}

void vibrate_old(int n_pulse, int pulse_strength) {
  Serial.println(pulse_strength);
  for (int x = 0; x < n_pulse; x++) {
    analogWrite(motorPin, pulse_strength); // set strength

    // lower strength needs longer pulses and higher one's need shorter pulses
    delay(400 - pulse_strength);

    // TODO: make a variable called "pulse_delay" which is a function of both strength and n_pulses
    // Use something like --> delay(pulse_delay);

    // TODO: add n_motors and run 'n' amount of motors based on pulse_strength

    digitalWrite(motorPin, LOW);  //stop vibrating
    delay(100); //wait 50 seconds.
  }
}


void vibrate() 
{
  
}
