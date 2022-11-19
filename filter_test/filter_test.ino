/*
  The sensor outputs provided by the library are the raw
  16-bit values obtained by concatenating the 8-bit high and
  low accelerometer and gyro data registers. They can be
  converted to units of g and dps (degrees per second) using
  the conversion factors specified in the datasheet for your
  particular device and full scale setting (gain).

  Example: An LSM6DS33 gives an accelerometer Z axis reading
  of 16276 with its default full scale setting of +/- 2 g. The
  LA_So specification in the LSM6DS33 datasheet (page 15)
  states a conversion factor of 0.061 mg/LSB (least
  significant bit) at this FS setting, so the raw reading of
  16276 corresponds to 16276 * 0.061 = 992.8 mg = 0.9928 g.
*/

#include <Wire.h>
#include <LSM6.h>

LSM6 imu;

char report[80];
double kaldist;
double filtered_val;
double old_kaldist;


void setup()
{
  Serial.begin(9600);
  Wire.begin();

  if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  imu.enableDefault();
}

void loop()
{
  imu.read();

  //  snprintf(report, sizeof(report), "A: %6d %6d %6d    G: %6d %6d %6d",
  //    imu.a.x, imu.a.y, imu.a.z,
  //    imu.g.x, imu.g.y, imu.g.z);
  
  kaldist = imu.a.y;
  int filtered_val = expo_filter(old_kaldist, kaldist);
  double kaldist_kalman = kalman(kaldist);
  double kaldist_kalman_comb = kalman(filtered_val);
  delay(200);
Serial.print(imu.a.y);
Serial.print("  ");
//Serial.print(filtered_val);
//Serial.print("  ");
Serial.print(kaldist_kalman);
Serial.print("  ");
Serial.println(kaldist_kalman_comb);
  old_kaldist = kaldist;
}

double expo_filter(double old_dist, double curr_dist) {
  double old_proportion = 0.8, curr_proportion = 0.2;

  return (old_proportion * old_kaldist) + (curr_proportion * curr_dist);

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
