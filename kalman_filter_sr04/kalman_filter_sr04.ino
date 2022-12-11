/* credits to VDEngineering video for the kalman filter algorithm
    youtube video: https://www.youtube.com/watch?v=ruB917YmtgE
*/


#include <ezButton.h>


// ULTRASONIC //
// Pins
const int echo = 3;
const int trig = 2;
// Constrains
const int maxdist = 335;
const float mindist = 2.5;

double distance, duration;
double kaldist;
double filtered_val;
double old_kaldist;

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

ezButton button(11);
int ledPin = 9;
int lastButtonState = 1;
bool pressedState;
bool toggleState = 0;


void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  button.setDebounceTime(50);
}

void usonic_transmit() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
}

void loop() {
   button.loop();
   int btnState = button.getState();
  
  usonic_transmit();

  duration = pulseIn(echo, HIGH);
  distance = (duration * .034) / 2;
  kaldist = kalman(distance);

  //Serial.println(distance);

  //Serial.println(kaldist);

  int filtered_val = expo_filter(old_kaldist,kaldist);
  Serial.println(toggleState);
  if (btnState == 0) {
    if (filtered_val < 300) {
      Serial.println(filtered_val);
   }
   
   analogWrite(ledPin, (200 - (kaldist))/5);
   old_kaldist = kaldist;
   
  }
  else {
   analogWrite(ledPin,0);
  }
   
   delay(50);
}
