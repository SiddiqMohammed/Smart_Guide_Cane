
int motorPin = 3;
int n_motors = 0;
int n_pulses = 4;
int pulse_strength = 0;
int sonar_dist = 0;
int pulse_min = 80;
int pulse_max = 255;
unsigned long previousTime = 0;
unsigned long currentTime = 2000;
unsigned long timeDifference;

// Quadrant Divisions
float q_increment = (pulse_max - pulse_min) / 4;
int q1_max = (int) (pulse_min + q_increment);
int q2_max = (int) (q1_max + q_increment);
int q3_max = (int) (q2_max + q_increment);



void setup() {
  // put your setup code here, to run once:
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("<Arduino Ready>");
}

void loop() {
  currentTime = millis();
  readSensor();
}

void toggleButton() {
  // TODO - make a button reader that changes flag state
  // to start/stop the entire system.
}

void readSensor() {
  // TODO
  // Get values from the sonar and do the if-else arguments here
  sonar_dist = random(0, 300);
  convert(sonar_dist);
}

void convert(int conversion) {
  pulse_strength = map(conversion, 0, 300, pulse_min, pulse_max);
  if(pulse_strength > q3_max)
    n_motors = map(pulse_strength, q3_max, pulse_max, 1, 4);
  else if(pulse_strength > q2_max)
    n_motors = map(pulse_strength, q2_max, q3_max, 1, 4);
  else if(pulse_strength > q1_max)
    n_motors = map(pulse_strength, q1_max, q2_max, 1, 4);
  else
    n_motors = map(pulse_strength, pulse_min, q1_max, 1, 4);
  vibrateMotors();
}

void vibrateMotors() {
  // TODO
  // Activate more than one motor based on n_motors 
  timeDifference = currentTime - previousTime;
  if(timeDifference >= 2000 - pulse_strength)
  {
    Serial.println("Motor beeped");
    analogWrite(motorPin, pulse_strength);
    previousTime = currentTime;
  }
}

void readVoltage() {
  // TODO
  // read the voltage from the batteries using analog read
}

void BMS() {
  // TODO - Battery management system
  // run readVoltage and also run vibrations in low battery mode
  // Morse maybe?
  // Run this function at startup and once after every n minutes.
}

void dataFiltering(){
  // TODO
  // Write all the filtering related code here.
  // This function should return filtered values.
}

void sysStop(){
  // TODO
  // Stops everything, resets all states and also turns off any running vibrations
}
