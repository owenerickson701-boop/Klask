#include <Arduino.h>
#include <math.h>
#include <AccelStepper.h>

// Secondary code. Read speeds, run motors. It's a simple life.

int pinStep = 9;
int pinDir = 10;
int pinStep2 = 11;
int pinDir2 = 12;

int Rx1 = 15; // A0
int Tx1 = 16; // A1
int Rx2 = 17; // A2
int Tx2 = 18; // A3

int xSpeed; // NOT ACTUALLY X AND Y
int ySpeed;

bool buttonPressed = false;      // dummy input
bool otherButtonPressed = false; // also dummy input

AccelStepper stepper1(1, pinStep, pinDir);
AccelStepper stepper2(1, pinStep2, pinDir2);

int stepPerIter = 100; // Maximum steps per poll of the BNO bc each call of run() can only go one step and reading sensors is slow

long rateToBoard = 9600;

void setup(void)
{
  pinMode(Rx1, INPUT);
  pinMode(Tx1, OUTPUT);
  pinMode(Rx2, INPUT);
  pinMode(Tx2, OUTPUT);

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
  pinMode(pinStep, OUTPUT);
  pinMode(pinDir, OUTPUT);
  pinMode(pinDir2, OUTPUT);
  pinMode(pinStep2, OUTPUT);
}

void loop()
{
  if (Serial1.available() || Serial2.available())
  {
    if (Serial1.available())
    {
      xSpeed = Serial1.parseInt(); // get values
      stepper1.setSpeed((float)(xSpeed));
    }

    if (Serial2.available())
    {
      ySpeed = Serial2.parseInt(); // get values
      stepper2.setSpeed((float)(ySpeed));
    }

    /* Serial1.print("Received ");
   Serial1.print(xSpeed);
   Serial1.print(", ");
   Serial1.println(ySpeed);*/
  }

  stepper1.runSpeed();
  stepper2.runSpeed();
}