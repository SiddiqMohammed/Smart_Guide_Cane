include<ezButton.h>

  ezButton button(7);                 // create ezButton object that attach to pin 7;
volatile bool buttonPressed = false;  // A flag to indicate whether the button has been pressed

int motorPin = 3;
int n_motors = 0;
int n_pulses = 4;
int pulse_strength = 0;

int pulse_min = 80;
int pulse_max = 255;
unsigned long previousTime = 0;
unsigned long currentTime = 2000;
unsigned long timeDifference;

unsigned long BMSpreviousMillis = 0;
const long BMSinterval = 300000;  // interval in milliseconds (5 minutes)
unsigned long BMScurrentMillis = 0;

// Quadrant Divisions
float q_increment = (pulse_max - pulse_min) / 4;
int q1_max = (int)(pulse_min + q_increment);
int q2_max = (int)(q1_max + q_increment);
int q3_max = (int)(q2_max + q_increment);

// Define the pattern for the SOS signal
int sosPattern[] = { 200, 200, 200, 600, 600, 600, 200, 200, 200 };


void setup() {
  // put your setup code here, to run once:
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);

  Serial.println("Running Battery Management System");
  BMS();
  Serial.println("<Arduino Ready>");
}

void loop() {
  button.loop();  // MUST call the loop() function first

  if (button.isPressed()) {
    Serial.println("The button is pressed");
    buttonPressed = !buttonPressed;
    vi
  }

  if (buttonPressed) {
    currentTime = millis();
    BMScurrentMillis = currentTime;

    if (BMScurrentMillis - BMSpreviousMillis >= BMSinterval) {
      BMSpreviousMillis = BMScurrentMillis;
      BMS();
    }
    readSensor();
  }
}

void toggleButton() {
  // TODO - make a button reader that changes flag state
  // to start/stop the entire system.
}

void readSensor() {
  // TODO
  // Get values from the sonar and do the if-else arguments here
  int sonar_dist = 0;
  sonar_dist = random(0, 300);  // Change this to ez sensor thing
  sonar_dist = dataFiltering(sonar_dist);
  convert(sonar_dist);
}

int dataFiltering(int sonar_dist) {
  // TODO
  // Write all the filtering related code here.
  // This function should return filtered values.
  return sonar_dist;
}

void convert(int conversion) {
  pulse_strength = map(conversion, 0, 300, pulse_min, pulse_max);
  if (pulse_strength > q3_max)
    n_motors = map(pulse_strength, q3_max, pulse_max, 1, 4);
  else if (pulse_strength > q2_max)
    n_motors = map(pulse_strength, q2_max, q3_max, 1, 4);
  else if (pulse_strength > q1_max)
    n_motors = map(pulse_strength, q1_max, q2_max, 1, 4);
  else
    n_motors = map(pulse_strength, pulse_min, q1_max, 1, 4);
  vibrateMotors();
}

void vibrateMotors() {
  // TODO
  // Activate more than one motor based on n_motors
  timeDifference = currentTime - previousTime;
  if (timeDifference >= 2000 - pulse_strength) {
    Serial.println("Motor beeped");
    analogWrite(motorPin, pulse_strength);
    previousTime = currentTime;
  }
}

void vibrateMotorPttern(int patternNumber) {
  // Low Voltage pattern
  if (patternNumber == 1) {
    for (int i = 0; i < 9; i++) {
      // Turn the motor on
      digitalWrite(motorPin, HIGH);
      delay(sosPattern[i]);
      // Turn the motor off
      digitalWrite(motorPin, LOW);
      // Wait for a short time before the next pulse
      delay(100);
    }
  }
  // Start/Stop Pattern
  if (patternNumber == 2) {
    for (int i = 0; i < n_motors; i++) {
      // Turn the motor on
      digitalWrite(motorPin, HIGH);
      delay(250);
      // Turn the motor off
      digitalWrite(motorPin, LOW);
      delay(100);

      digitalWrite(motorPin, HIGH);
      delay(250);
      // Turn the motor off
      digitalWrite(motorPin, LOW);
    }
  }
}

float readVoltage() {
  // TODO
  // read the voltage from the batteries using analog read
  int sensorValue = analogRead(A0);              // Read the voltage from analog input pin A0
  float voltage = sensorValue * (5.0 / 1023.0);  // Convert the sensor value to voltage (assuming 5V reference voltage)
  return voltage;
}

void BMS() {
  // TODO - Battery management system
  // run readVoltage and also run vibrations in low battery mode
  // Morse maybe?
  // Run this function at startup and once after every n minutes.
  float voltage = readVoltage();
  float minBatteryVoltage = 0.5;

  Serial.print("Battery Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  if (voltage < minBatteryVoltage) {
    Serial.println("Battery Voltage Low!");
    vibrateMotorPttern(1);
  }
}

void sysStop() {
  // TODO
  // Stops everything, resets all states and also turns off any running vibrations
}