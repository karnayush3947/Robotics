#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>
//comment
Adafruit_MPU6050 mpu;

// LED pins
const int GREEN_LED = D5;
const int RED_LED = D6;

// Experimental threshold
const float MOTION_THRESHOLD = 12.0;

void setup() {

  Serial.begin(115200);

  // LED setup
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Start with normal state
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);

  // Start I2C
  Wire.begin(D2, D1);

  // Start MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    while (1) {
      delay(10);
    }
  }

  // Sensor settings
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("System Ready");
  Serial.println("-------------------------");
}

void loop() {

  sensors_event_t a, g, temp;

  mpu.getEvent(&a, &g, &temp);

  // Calculate total acceleration magnitude
  float accelerationMagnitude =
      sqrt(
        a.acceleration.x * a.acceleration.x +
        a.acceleration.y * a.acceleration.y +
        a.acceleration.z * a.acceleration.z
      );

  Serial.print("Acceleration magnitude: ");
  Serial.print(accelerationMagnitude);
  Serial.println(" m/s^2");

  // Decision logic
  if (accelerationMagnitude < MOTION_THRESHOLD) {

    // NORMAL
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);

    Serial.println("STATUS: NORMAL");

  } else {

    // STRONG MOVEMENT
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    Serial.println("STATUS: STRONG MOVEMENT!");

  }

  Serial.println();

  delay(200);
}
