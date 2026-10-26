#include <Arduino.h>
#include <math.h>
#include <Adafruit_BNO08x.h>

// MAIN file. Read accel, calculate speed, send to second board.

#define BNO08X_RESET -1

int Rx1 = 15; // A0
int Tx1 = 16; // A1
int Rx2 = 17; // A2
int Tx2 = 18; // A3

int xSpeed = 0;
int ySpeed = 0;

bool buttonPressed = false;      // dummy input
bool otherButtonPressed = false; // dummy input

long rateToPc = 115200;
long rateToBoard = 9600;

int k = 10;          // Tuning. Speed multiplier.
int stepsToAvg = 10; // Cancel out the IMU being a @$#&^%# at rest

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;
sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;

struct euler_t
{
  float yaw;
  float pitch;
  float roll;
} ypr;

void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t *ypr, bool degrees = false)
{

  float sqr = sq(qr);
  float sqi = sq(qi);
  float sqj = sq(qj);
  float sqk = sq(qk);

  ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
  ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
  ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

  if (degrees)
  {
    ypr->yaw *= RAD_TO_DEG;
    ypr->pitch *= RAD_TO_DEG;
    ypr->roll *= RAD_TO_DEG;
  }
}

void quaternionToEulerRV(sh2_RotationVectorWAcc_t *rotational_vector, euler_t *ypr, bool degrees = false)
{
  quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
}

void setReports();

void setup(void)
{

  Serial.begin(rateToPc);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.print("IM ALIVE");

  pinMode(Rx1, INPUT);
  pinMode(Tx1, OUTPUT);
  pinMode(Rx2, INPUT);
  pinMode(Tx2, OUTPUT);

  Serial1.setPins(Rx1, Tx1);
  Serial1.begin(rateToBoard);
  Serial2.setPins(Rx2, Tx2);
  Serial2.begin(rateToBoard);

  if (!bno08x.begin_I2C())
  {
    Serial.println("Failed to find BNO08x chip");
  }

  Serial.println("BNO08x Found!");
  setReports();
}

void loop()
{
  if (bno08x.wasReset())
  {
    // Serial.println("sensor was reset ");
    setReports();
  }

  float alphas = 0; // sum of alphas
  float betas = 0;  // sum of betas

  for (int i = 0; i < stepsToAvg; i++)
  {
    while (!bno08x.getSensorEvent(&sensorValue))
    {
      delay(1); // wait for event
    }
    quaternionToEulerRV(&sensorValue.un.arvrStabilizedRV, &ypr, true);
    alphas += ypr.roll;
    betas += ypr.pitch;
  }

  float alpha = alphas / stepsToAvg;
  float beta = betas / stepsToAvg;

  if (buttonPressed) // Macro 1: FULL SPEED AHEAD
  {
    xSpeed = 1000.0;
    ySpeed = -1000.0;
  }
  else if (otherButtonPressed) // Brakes/Estop
  {
    xSpeed = 0.0;
    ySpeed = 0.0;
  }
  else
  {
    xSpeed = (alpha + beta) * k; // Math this out later
    ySpeed = (alpha - beta) * k;
  }

  Serial1.println((int)(xSpeed)); // send speeds
  Serial2.println((int)(ySpeed));

  // Serial.print(alpha);
  // Serial.print("    ");
  // Serial.println(beta);

  /*
  if (Serial1.available())
  {
    Serial.println(Serial1.readStringUntil('\n'));
  }*/
}

void setReports(void)
{
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_ARVR_STABILIZED_RV))
  {
    Serial.println("Could not enable accelerometer");
  }
  else
  {
    Serial.println("Success!");
  }
}