#include <Arduino.h>
#include <math.h>
#include <AccelStepper.h>

// MAIN file. Read accel, calculate speed, send to second board.

int pinStep = 9;
int pinDir = 10;
int pinStep2 = 11;
int pinDir2 = 12;

int Rx1 = 15; // A0
int Tx1 = 16; // A1

int Rx2 = 17; // A2
int Tx2 = 18; // A3

int xSpeed;
int ySpeed;

bool buttonPressed = false;      // dummy input
bool otherButtonPressed = false; // also dummy input

AccelStepper stepper1(1, pinStep, pinDir);
AccelStepper stepper2(1, pinStep2, pinDir2);

int k = 2500;          // Tuning. Speed multiplier.
int stepPerIter = 200; // Maximum steps per poll of the BNO bc each call of run() can only go one step and reading sensors is slow

int Rx = 15; // A0
int Tx = 16; // A1

long rateToPc = 115200;
long rateToBoard = 9600;

int xSpeed;
int ySpeed;

void setup(void)
{
  Serial1.setPins(Rx1, Tx1);
  Serial1.begin(rateToBoard);
  Serial2.setPins(Rx2, Tx2);
  Serial2.begin(rateToBoard);

  stepper1.setAcceleration(10000);
  stepper1.setSpeed(1500);
  stepper1.setMaxSpeed(1500);
  stepper2.setAcceleration(10000);
  stepper2.setSpeed(1500);
  stepper2.setMaxSpeed(1500);
  pinMode(Tx, OUTPUT);
  pinMode(Rx, INPUT);
}

void loop()
{
  xSpeed = Serial1.parseInt();
  ySpeed = Serial2.parseInt();

  stepper1.setSpeed(xSpeed);
  stepper2.setSpeed(ySpeed);
  for (int i = 0; i < stepPerIter; i++)
  {
    stepper1.run();
    stepper2.run();
  }
}