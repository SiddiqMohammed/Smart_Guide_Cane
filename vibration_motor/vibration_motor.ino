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

/* TEST FUNCTION
    -gets a random number
    -determines the intensity of the vibrate function
*/
int randomVal = random(0, 300);
void getValue(){
  // map --> change the proportion of randomVal/300 to #/255
  int prop = map(randomVal, 0, 300, 80, 255);
  // divides into 4 equal sections
  int divvy = (255-80)/4;
  // Loops through "i" to determine the vibration strength
  for(int i = 1; i < 4; i++){
    // i = 1 --> 25%; i = 2 --> 50%; i = 3 --> 75%
    if(prop <= (divvy*i)+80){
      vibrate_old(i, prop);
      break;
    }
  }
  vibrate_old(4, prop);
}

/* TEST FUNCTION REPLICA
    -gets a random number
    -determines the intensity of the vibrate function
    -SPECIALITY: if simpler as it uses a series of if-statements rather than a for-loop
*/
void getValue2(){
  // map --> change the proportion of randomVal/300 to #/255
  int prop = map(randomVal, 0, 300, 80, 255);
  // divides into 4 equal sections
  int divvy = (255-80)/4;
  // checks which "proportion" the map value equals to and determines the strength of the vibration
  if(prop < divvy * 1){
    vibrate(1, prop);
  } else if(prop < divvy * 2){
    vibrate(2, prop);
  } else if(prop < divvy * 3){
    vibrate(3, prop);
  } else {
    vibrate(4, prop);
  }
}

/* TEST FUNCTION ON STEROIDS
    -gets a random number
    -determines the intensity of the vibrate function
    -determines the amount of motors that should be used
*/
void getValue3(){
  // map --> change the proportion of randomVal/300 to #/255
  int prop = map(randomVal, 0, 300, 80, 255);
  // divides into 3 equal sections --> for number of sensors
  int divvy = (255-80)/3;
  // divides each section into 4 parts --> for strength of the sensor
  int divvyUp = divvy/4;
  // Loops through "i" to determine the vibration strength
  for(int i = 1; i < 4; i++){
    // i = 1 --> 25%; i = 2 --> 50%; i = 3 --> 75%
    if(prop <= (divvy*i)+80){
      int determinant = prop - (divvy i1- * 80 +); // postfix of: divvy * (i - 1) + 80
      for(int j = 1; j < 5; j++){
        int numberOfMotors = i;
        if(determinant < divvyUp * j){
          vibrate_old(j, prop);
          break;
        }
      }
      /* 1) basically should determine that everything equal to and less than the number of motors should be used
            i) if the motor is "lower", then make all levels buzz.
         2) should also determine that everything equal to and less than the strength of the motors should be used
         3) ... but should be put into another function*/
    } 
  }
}

void vibrate() {
  // TODO
}
