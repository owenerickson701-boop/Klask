#include <Arduino.h>
#include <math.h>
#include <Adafruit_BNO08x.h>
#include <AccelStepper.h>

#define BNO08X_RESET -1

int pinStep = 9;
int pinDir = 10;
int xSpeed;
int ySpeed;

AccelStepper stepper1(1, pinStep, pinDir);

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

int k = 2500;          // Tuning. Speed multiplier.
int stepPerIter = 200; // Maximum steps per poll of the BNO bc each call of run() can only go one step and reading sensors is slow

void setReports();

void setup(void)
{
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  if (!bno08x.begin_I2C())
  {
    Serial.println("Failed to find BNO08x chip");
  }
  Serial.println("BNO08x Found!");
  setReports();
  stepper1.setAcceleration(10000);
  stepper1.setSpeed(1500);
  stepper1.setMaxSpeed(1500);
}

void loop()
{

  if (bno08x.wasReset())
  {
    Serial.println("sensor was reset ");
    setReports();
  }

  if (!bno08x.getSensorEvent(&sensorValue))
  {
    return;
  }

  float alpha = sensorValue.un.gyroscope.x;
  float beta = sensorValue.un.gyroscope.y;

  Serial.print(alpha);
  Serial.print("   ");
  Serial.println(beta);

  xSpeed = alpha * k;
  ySpeed = beta * k;

  Serial.println(xSpeed);

  stepper1.setSpeed(xSpeed);
  for (int i = 0; i < stepPerIter; i++)
  {
    stepper1.run();
  }
}

void setReports(void)
{
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_GYRO_INTEGRATED_RV))
  {
    Serial.println("Could not enable accelerometer");
  }
  else
  {
    Serial.println("Success!");
  }
}