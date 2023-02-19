#include <Maxbotix.h>
#include <SR04.h>
#define TRIG_PIN 7
#define ECHO_PIN 6
#define VOLT_PIN A1
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
int pw_pin = 9;
int buttonPin = 8;
int motor_pin = 5;
int debounceTime = 50;
//Maxbotix rangeSensorPW(pw_pin, Maxbotix::PW, Maxbotix::LV);
double distance;
int toggleState;
int lastButtonState = 1;
long unsigned int lastPress;
long unsigned int lastTimeBMS = 0;
double max_dist = 100;
double kaldist;
double old_kaldist = 10;
double fil_dist;
double battery_volt;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   pinMode(motor_pin, OUTPUT);
   pinMode(buttonPin, INPUT);
}

void loop() {
  toggleButton(lastButtonState, toggleState);
  //Serial.println(toggleState);
  if (toggleState ==0) {
  battery_volt = readVoltage(VOLT_PIN);
  //Serial.println(battery_volt);
  //toggleButton(lastButtonState, toggleState);
  //distance = rangeSensorPW.getRange();
  
  distance = readSensor();
  //Serial.println(distance);
  kaldist = dataFiltering(distance);
  //Serial.println(kaldist);
  
  //delay(50);
  old_kaldist = kaldist;
  fil_dist = expo_filter(old_kaldist, kaldist);
  Serial.println(fil_dist);
  vibrateMotors(fil_dist);
  BMS(lastTimeBMS);
  }


}

void toggleButton(int &lastButtonState, int &toggleState) {
  // TODO - make a button reader that changes flag state
  // to start/stop the entire system.
  int buttonState = digitalRead(buttonPin);//read the state of buttonPin and store it as buttonState (0 or 1)
  long unsigned curr_time = millis();
  if((millis() - lastPress) > debounceTime)//if the time between the last buttonChange is greater than the debounceTime
  {
    lastPress = millis();//update lastPress                                                     
    if(buttonState == 0 && lastButtonState == 1)//if button is pressed and was released last change
    {
       
      toggleState =! toggleState;
      lastButtonState = 0;    //record the lastButtonState
      while (curr_time + 1000 > millis()) {
        analogWrite(motor_pin, 200);
      }
      analogWrite(motor_pin, 0);
    }
  
    if(buttonState == 1 && lastButtonState == 0)//if button is not pressed, and was pressed last change
    {
      lastButtonState = 1;//record the lastButtonState
    }
  }
}

double readSensor() {
  // TODO
  // Get values from the sonar and do the if-else arguments here
  double sense_dist = sr04.Distance();
  return sense_dist;
  
}

void vibrateMotors(double distance) {
  // TODO
  // Based on the sonar sensor values vibrate n number of motors
  if (distance > max_dist) {
    distance = max_dist;
  }
  double vibe_val = 250 - (distance * (250.0/ max_dist));
  //Serial.println(vibe_val);
  analogWrite(motor_pin, vibe_val);
  delay(distance * 0.5 + 0.1);
  analogWrite(motor_pin, 0);
  delay(distance * 0.5 + 0.1);
//  if (distance * 0.5) {
//    Serial.println("Running Case 1");
//    analogWrite(motor_pin, 200);
//  }
//  else {
//    analogWrite(motor_pin, LOW);
//  }
}

double readVoltage(int pin) { //make a double and make the pin input a parameter (int pin)
  // TODO
  // read the voltage from the batteries using analog read
  double volt_reading = analogRead(pin);
  float voltage = volt_reading * (5.0 / 1023.0);
  return voltage;
  
}

void BMS(long unsigned &last_time) {
  // TODO - Battery management system
  // run readVoltage and also run vibrations in low battery mode
  // Morse maybe?
  // This function runs everytime loop is run. If it detects low voltage, a 12 pulse LED flash with occur using delays. 
  // This can only happen once every specified amount
  long unsigned warning_delay = 10000; //10 Second delay
  long unsigned curr_time = millis(); 
  
  double curr_voltage = readVoltage(VOLT_PIN);
  double low_volt = 2;
  //sensorValue = analogRead(sensorPin);
  if ((curr_voltage < low_volt) && ((curr_time - last_time > warning_delay) || (last_time == 0))) {
    Serial.print("Low Power");
    analogWrite(motor_pin, 0);
    for(int i=1; i < 12; i++) {
  
    analogWrite(motor_pin, 250);
   // stop the program for <200> milliseconds:
    delay(200);
   // turn the ledPin off:
    analogWrite(motor_pin, 0);
   // stop the program for for <200> milliseconds:
    delay(200);
    
    }
    last_time = millis();
    
//  if ((curr_voltage <= low_volt)&((millis()-last)>10000)) {
//    for (int i=1; i < 3; i++) {
//      last = millis();
//      analogWrite(motor_pin, warning_val);
//      if (millis() - last > 100) {
//        continue;
//      }
//    }
//  }
  
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
