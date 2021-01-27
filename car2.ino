#include <Wire.h>

// Based on the code showcased in EEEnthusiast


//Conect the SDA, SCL pins

//Connect AD0 to ground
//Connect to 3.3V pin for VCC


#define enA  11
#define enB  3
#define in1  9
#define in2  8
#define in3  7
#define in4  6

long accelX, accelY, accelZ;
float gX, gY, gZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  initMPU();

}

void loop() {
  // put your main code here, to run repeatedly:

  recordAccelRegisters();
  recordGyroRegisters();

  if (gX > 0)
  {
    forward(round(gX*255));
  }

  else
  {
    backward(abs(round(gX*255)));
  }
  

  delay(50);

}

void initMPU()
{
  Wire.beginTransmission(0b1101000);
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();

  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission();
}


void recordAccelRegisters()
{
  Wire.beginTransmission(0b1101000);
  Wire.write(0x3B);
  Wire.endTransmission();

  Wire.requestFrom(0b1101000, 6);
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read();
  accelY = Wire.read()<<8|Wire.read();
  accelZ = Wire.read()<<8|Wire.read();
  processAccel();
  
}

void recordGyroRegisters()
{
  Wire.beginTransmission(0b1101000);
  Wire.write(0x43);
  Wire.endTransmission();

  Wire.requestFrom(0b1101000, 6);
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read();
  gyroY = Wire.read()<<8|Wire.read();
  gyroZ = Wire.read()<<8|Wire.read();
  processGyro();
  
}

void processAccel()
{
  gX = accelX/16384.0;
  gY = accelY/16384.0;
  gZ = accelZ/16384.0;
}

void processGyro()
{
  rotX = gyroX/131.0;
  rotY = gyroY/131.0;
  rotZ = gyroZ/131.0;
}

void backward(int spid)
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, spid);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, spid);
}

void forward(int spid)
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, spid);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, spid);
}
