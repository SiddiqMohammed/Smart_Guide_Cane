/*
  The vibration motor is enabled and disabled with different intensities and for different time durations.
*/

int motorPin = 3; //motor transistor is connected to pin 3

// Pulse Vars (May need to delete some for memory purposes)
int pulse_Min = 80;
int pulse_Max = 255;
int pulse_Num = 0;
int min_pulses = 1;
int max_pulses = 4;
int q_increment = (pulse_Max - pulse_Min) / 4;  // quadrant incrementor
int q1_max = (int) (pulse_Min + q_increment);   // quadrant bounds
int q2_max = (int) (q1_max + q_increment);
int q3_max = (int) (q2_max + q_increment);



void setup()
{
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("<Arduino Ready>");
}

void loop()
{
  // Generate a sonar distance and map it to pulse_strength.
  int sonar_Dist = rand(0, 300);
  int pulse_Strength = map(sonar_Dist, 0, 300, pulseMin, pulseMax);
 

  // Map pulse_Strength to pulse_Num with proper ranges.
  if(pulse_Strength > q3_max)
    pulse_Num = map(pulse_Strength, q3_max, pulse_Max, min_pulses, max_pulses);
  else if(pulse_Strength > q2_max)
    pulse_Num = map(pulse_Strength, q2_max, q3_max, min_pulses, max_pulses);
  else if(pulse_Strength > q1_max)
    pulse_Num = map(pulse_Strength, q1_max, q2_max, min_pulses, max_pulses);
  else
    pulse_Num = map(pulse_Strength, pulse_Min, q1_max, min_pulses, max_pulses);

  // Vibrate with achieved values.
  vibrate_old(pulse_Num, pulse_Strength);
  
  
  
  // TODO: Create a similar function as vibrate_old() but make it without delay
  // Currently the vibrate_old function uses delays and this blocks the code
  // update it so that it runs concurrently with the rest of the code
  // HINT: use millis() function.

  // vibrate();


  /*vibrate_old(3, 80);
  delay(1000);
  vibrate_old(3, 120);
  delay(1000);
  vibrate_old(3, 200);
  delay(1000);
  vibrate_old(4, 250);
  delay(1000);*/

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


void vibrate() {
  // TODO
}
