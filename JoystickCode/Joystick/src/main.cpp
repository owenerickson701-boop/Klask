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

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

int k = 1; // Tuning. Speed multiplier.

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
    Serial.println("sensor was reset ");
    setReports();
  }
  if (!bno08x.getSensorEvent(&sensorValue))
  {
    return;
  }

  int alpha = 255 * sensorValue.un.gyroscope.x;
  int beta = 255 * sensorValue.un.gyroscope.y;

  if (buttonPressed) // Macro 1
  {
  }
  else if (otherButtonPressed) // Macro 2
  {
  }
  else
  {
    xSpeed = (alpha + beta) * k;
    ySpeed = (alpha - beta) * k;
  }

  Serial1.println(xSpeed); // send speeds
  Serial2.println(ySpeed);

  Serial.print("Echo: ");

  Serial.print(Serial1.parseInt());
  Serial.println(Serial2.parseInt());
}

void setReports(void)
{
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_GYROSCOPE_UNCALIBRATED))
  {
    Serial.println("Could not enable accelerometer");
  }
  else
  {
    Serial.println("Success!");
  }
}