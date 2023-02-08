void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void toggleButton() {
  // TODO - make a button reader that changes flag state
  // to start/stop the entire system.
}

void readSensor() {
  // TODO
  // Get values from the sonar and do the if-else arguments here
}

void vibrateMotors() {
  // TODO
  // Based on the sonar sensor values vibrate n number of motors
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
