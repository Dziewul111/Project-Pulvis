#include <Wire.h>
#include <Servo.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SparkFun_BNO085_Arduino_Library.h>

// ===================== SERWA =====================
Servo servoAZ;
Servo servoEL;

// do utalenia
#define SERVO_AZ_PIN 
#define SERVO_EL_PIN 

#define SERVO_AZ_ZERO 90
#define SERVO_EL_ZERO 90

// ===================== GPS ======================
SoftwareSerial gpsSerial(4, 3);
TinyGPSPlus gps;

// Pozycja stacji 
double gs_lat, gs_lon, gs_alt;

// Pozycja CanSata 
double sat_lat, sat_lon, sat_alt;

// ===================== IMU ======================
BNO085 imu;

// ===================== SETUP =====================
void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Wire.begin();

  servoAZ.attach(SERVO_AZ_PIN);
  servoEL.attach(SERVO_EL_PIN);

  servoAZ.write(SERVO_AZ_ZERO);
  servoEL.write(SERVO_EL_ZERO);

  if (!imu.begin()) {
    Serial.println("BNO085 nie wykryty");
    while (1);
  }

  imu.enableRotationVector(50); // 50 ms

  Serial.println("stacja gotowa :>");
}

// ===================== LOOP =====================
void loop() {

  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    gs_lat = gps.location.lat();
    gs_lon = gps.location.lng();
    gs_alt = gps.altitude.meters();
  }

  float bearingWorld = calculateBearing(gs_lat, gs_lon, sat_lat, sat_lon);
  float heading = getCompassHeading();

  float bearingRelative = normalizeAngle(bearingWorld - heading);

  float elevation = calculateElevation(
    gs_lat, gs_lon, gs_alt,
    sat_lat, sat_lon, sat_alt
  );

  int azServo = constrain(SERVO_AZ_ZERO + bearingRelative, 0, 180);
  int elServo = constrain(SERVO_EL_ZERO + elevation, 0, 180);

  servoAZ.write(azServo);
  servoEL.write(elServo);

  Serial.print("World AZ: "); Serial.print(bearingWorld);
  Serial.print(" | Heading: "); Serial.print(heading);
  Serial.print(" | Rel AZ: "); Serial.print(bearingRelative);
  Serial.print(" | EL: "); Serial.println(elevation);

  delay(200);
}

// ===================== FUNKCJE =====================

float getCompassHeading() {
  if (imu.dataAvailable()) {
    float heading = degrees(imu.getYaw());
    if (heading < 0) heading += 360;
    return heading;
  }
  return 0;
}

float calculateBearing(double lat1, double lon1, double lat2, double lon2) {
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  double dLon = radians(lon2 - lon1);

  double y = sin(dLon) * cos(lat2);
  double x = cos(lat1)*sin(lat2) -
             sin(lat1)*cos(lat2)*cos(dLon);

  return fmod(degrees(atan2(y, x)) + 360, 360);
}

float calculateElevation(
  double lat1, double lon1, double alt1,
  double lat2, double lon2, double alt2
) {
  double d = distanceMeters(lat1, lon1, lat2, lon2);
  return degrees(atan2(alt2 - alt1, d));
}

double distanceMeters(double lat1, double lon1, double lat2, double lon2) {
  const double R = 6371000.0;
  double dLat = radians(lat2 - lat1);
  double dLon = radians(lon2 - lon1);

  lat1 = radians(lat1);
  lat2 = radians(lat2);

  double a = sin(dLat/2)*sin(dLat/2) +
             cos(lat1)*cos(lat2)*sin(dLon/2)*sin(dLon/2);

  return R * 2 * atan2(sqrt(a), sqrt(1-a));
}

float normalizeAngle(float a) {
  while (a < -180) a += 360;
  while (a > 180) a -= 360;
  return a;
}