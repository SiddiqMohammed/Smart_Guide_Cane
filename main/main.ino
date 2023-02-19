#include <Maxbotix.h>

int pw_pin = 9;
int motor_pin = A0;
Maxbotix rangeSensorPW(pw_pin, Maxbotix::PW, Maxbotix::LV);
double distance;

double kaldist;
double old_kaldist = 10;
double fil_dist;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   pinMode(motor_pin, OUTPUT);
}

void loop() {
  
  distance = rangeSensorPW.getRange();
  //Serial.println(distance);
  kaldist = dataFiltering(distance);
  //Serial.println(kaldist);
  // put your main code here, to run repeatedly:
  //delay(50);
  old_kaldist = kaldist;
  fil_dist = expo_filter(old_kaldist, kaldist);
  Serial.println(fil_dist);
  vibrateMotors(fil_dist);

}

void toggleButton() {
  // TODO - make a button reader that changes flag state
  // to start/stop the entire system.
  
}

void readSensor() {
  // TODO
  // Get values from the sonar and do the if-else arguments here
  
  
}

void vibrateMotors(double distance) {
  // TODO
  // Based on the sonar sensor values vibrate n number of motors
  // double curr_voltage = readVoltage();
  long last = millis();
  if (distance > 30 && distance < 50) {
    Serial.println("Running");
    analogWrite(motor_pin, 200);
  }
  else {
    analogWrite(motor_pin, LOW);
  }
}

double readVoltage(int pin) { //make a double and make the pin input a parameter (int pin)
  // TODO
  // read the voltage from the batteries using analog read
  double volt_reading = analogRead(pin);
  float voltage = volt_reading * (5.0 / 1023.0);
  return voltage;
  
}

void BMS(int motor_pin) {
  // TODO - Battery management system
  // run readVoltage and also run vibrations in low battery mode
  // Morse maybe?
  // Run this function at startup and once after every n minutes.
  long unsigned last = millis(); 
  int warning_val = 1000;
  
  double curr_voltage = readVoltage(50);
  double low_volt = 4.1;
  
  if ((curr_voltage <= low_volt)&((millis()-last)>10000)) {
    for (int i=1; i < 3; i++) {
      last = millis();
      analogWrite(motor_pin, warning_val);
      if (millis() - last > 100) {
        continue;
      }
    }
  }
  
  
  
}

double dataFiltering(double data){
  // TODO
  // Write all the filtering related code here.
  // This function should return filtered values.
  // Kalman
  double filtered = kalman(data);
  return filtered;
  

  
}

void sysStop(){
  // TODO
  // Stops everything, resets all states and also turns off any running vibrations
  
}

double kalman(double U) {
  static const double R = 40;
  static const double H = 1.00;
  static double Q = 10;
  static double P = 0;
  static double U_hat = 0;
  static double K = 0;
  K = P * H / (H * P * H + R);
  U_hat += + K * (U - H * U_hat);
  P = (1 - K * H) * P + Q;
  return U_hat;
}
double expo_filter(double old_dist, double curr_dist) {
  double old_proportion = 0.8, curr_proportion = 0.2;
  
  return (old_proportion * old_kaldist) + (curr_proportion * curr_dist);
  
}
