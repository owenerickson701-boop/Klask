#include <Arduino.h>
int Rx1 = 15; // A0
int Tx1 = 16; // A1

int Rx2 = 17; // A2
int Tx2 = 18; // A3

long rateToPc = 115200;
long rateToBoard = 9600;

int xSpeed;
int ySpeed;

void setup(void)
{

  Serial.begin(rateToPc);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  pinMode(Rx1, INPUT);
  pinMode(Tx1, OUTPUT);
  pinMode(Rx2, INPUT);
  pinMode(Tx2, OUTPUT);

  Serial1.setPins(Rx1, Tx1);
  Serial1.begin(rateToBoard);

  Serial2.setPins(Rx2, Tx2);
  Serial2.begin(rateToBoard);
}

void loop()
{

  xSpeed = millis() % 255;
  Serial.print(xSpeed);
  Serial.print("     ");
  Serial1.print(xSpeed);

  delay(10);
  ySpeed = Serial2.parseInt();
  Serial.println(ySpeed);
}

/*
void writePin(int xSpeed)
{

  while (Serial1.available())
  {
    char c = (char)(xSpeed);
    if (c == '\n')
    {
      boardbuffer.trim();
      if (boardbuffer.length() > 0)
      {
        Serial.println(boardbuffer.toInt());
      }
      boardbuffer = "";
    }
    else if (c != '\r')
    {
      boardbuffer += c;
    }
  }
}

float readPin()
{
  while (Serial1.available())
  {
    char c = Serial1.read();
    if (c == '\n')
    {
      boardbuffer.trim();
      if (boardbuffer.length() > 0)
      {
        // Serial1.println(boardbuffer);
        Serial1.println(DoSomeMath(boardbuffer.toInt()));
      }
      boardbuffer = "";
    }
    else if (c != '\r')
    {
      boardbuffer += c;
    }
  }
}
  */