#include <Maxbotix.h>
#define VOLT_PIN A1

int pw_pin = 11;
Maxbotix rangeSensorPW(pw_pin, Maxbotix::PW, Maxbotix::LV);
int buttonPin = 8;
int motor_pin1 = 15;
int motor_pin2 = 16;
int motor_pin3 = 17;
int motor_pin4 = 18;

int debounceTime = 50;
double distance;

int toggleState;
int lastButtonState = 1;
long unsigned int lastPress;

long unsigned int lastTimeBMS = 0;
long unsigned int lastTimeMotor = 0;

int toggleMotor = 0;

double max_dist = 250;
double kaldist;
double old_kaldist = 50;
double fil_dist;

double battery_volt;
double low_volt = 3.7;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   pinMode(motor_pin1, OUTPUT);
   pinMode(motor_pin2, OUTPUT);
   pinMode(motor_pin3, OUTPUT);
   pinMode(motor_pin4, OUTPUT);
   pinMode(buttonPin, INPUT);
   pinMode(VOLT_PIN, INPUT);
   
}

void loop() {
  //toggleButton(lastButtonState, toggleState);
  //Serial.println(toggleState);
  //if (toggleState == 0) {
  //battery_volt = readVoltage(VOLT_PIN);
  //Serial.println(battery_volt);
  //toggleButton(lastButtonState, toggleState);
  //distance = rangeSensorPW.getRange();
  
  distance = readSensor();
  //Serial.println(distance);
  kaldist = dataFiltering(distance);
  //Serial.println(kaldist);
  
  //delay(50);
  
  fil_dist = expo_filter(old_kaldist, kaldist);
  //Serial.println(kaldist);
  vibrateMotors(fil_dist,lastTimeMotor,toggleMotor);
  
  //BMS(lastTimeBMS);
  old_kaldist = kaldist;
  //}
}

//void toggleButton(int &lastButtonState, int &toggleState) {
//  // TODO - make a button reader that changes flag state
//  // to start/stop the entire system.
//  int buttonState = digitalRead(buttonPin);//read the state of buttonPin and store it as buttonState (0 or 1)
//  long unsigned curr_time = millis();
//  if((millis() - lastPress) > debounceTime)//if the time between the last buttonChange is greater than the debounceTime
//  {
//    lastPress = millis();//update lastPress                                                     
//    if(buttonState == 0 && lastButtonState == 1)//if button is pressed and was released last change
//    {
//       
//      toggleState =! toggleState;
//      lastButtonState = 0;    //record the lastButtonState
//      while (curr_time + 1000 > millis()) {
//        analogWrite(motor_pin, 200);
//      }
//      analogWrite(motor_pin, 0);
//    }
//  
//    if(buttonState == 1 && lastButtonState == 0)//if button is not pressed, and was pressed last change
//    {
//      lastButtonState = 1;//record the lastButtonState
//    }
//  }
//}

double readSensor() {
  // TODO
  // Get values from the sonar and do the if-else arguments here
  double sense_dist = rangeSensorPW.getRange();
  return sense_dist;
  
}

void vibrateMotors(double distance, long unsigned &last_time_motor, int &on_off_toggle) {
  // TODO
  // Based on the sonar sensor values vibrate n number of motors
  
  if (distance > max_dist) {
    distance = max_dist;
  }
  
  // vibe_val = 250 - (distance * (250.0/ max_dist));
  int vibe_val = 255 - map(distance, 10, 250, 0, 255);
  double vibe_delay = ((distance * distance) * 0.01);
  analogWrite(motor_pin1, vibe_val);
  analogWrite(motor_pin2, vibe_val);
  analogWrite(motor_pin3, vibe_val);
  analogWrite(motor_pin4, vibe_val);
  
  Serial.println(vibe_val);
//  long unsigned curr_time = millis();
//  if (((curr_time - last_time_motor > vibe_delay) || (last_time_motor == 0))) {
//    if (on_off_toggle == 0) {
//      analogWrite(motor_pin, vibe_val);
//    }
//    else {
//      analogWrite(motor_pin, 0);
//    }
//    on_off_toggle =! on_off_toggle;
//    last_time_motor = millis();
//  }
}

double readVoltage(int pin) { //make a double and make the pin input a parameter (int pin)
  // TODO
  // read the voltage from the batteries using analog read
  double volt_reading = analogRead(pin);
  float voltage = volt_reading * (5.0 / 1023.0);
  return voltage;
  //return volt_reading;
  
}

//void BMS(long unsigned &last_time_battery) {
//  // TODO - Battery management system
//  // run readVoltage and also run vibrations in low battery mode
//  // Morse maybe?
//  // This function runs everytime loop is run. If it detects low voltage, a 12 pulse LED flash with occur using delays. 
//  // This can only happen once every specified amount
//  long unsigned warning_delay = 10000; //10 Second delay
//  long unsigned curr_time = millis(); 
//  
//  double curr_voltage = readVoltage(VOLT_PIN);
//  //Serial.println(curr_voltage);
//  //sensorValue = analogRead(sensorPin);
//  if ((curr_voltage < low_volt) && ((curr_time - last_time_battery > warning_delay) || (last_time_battery == 0))) {
//    Serial.print("Low Power");
//    analogWrite(motor_pin1, 0);
//    analogWrite(motor_pin2, 0);
//    analogWrite(motor_pin3, 0);
//    analogWrite(motor_pin4, 0);
//    for(int i=1; i < 12; i++) {
//  
//    analogWrite(motor_pin, 250);
//   // stop the program for <200> milliseconds:
//    delay(200);
//   // turn the ledPin off:
//    analogWrite(motor_pin, 0);
//   // stop the program for for <200> milliseconds:
//    delay(200);
//    
//    }
//    last_time_battery = millis();
//    
//  if ((curr_voltage <= low_volt)&((millis()-last)>10000)) {
//    for (int i=1; i < 3; i++) {
//      last = millis();
//      analogWrite(motor_pin, warning_val);
//      if (millis() - last > 100) {
//        continue;
//      }
//    }
//}
  
 // }
  
//}

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
