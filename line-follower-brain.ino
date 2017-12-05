#include "Wire.h"
#include "sensorbar.h"

// Clockwise and counter-clockwise definitions.
// Depending on how you wired your motors, you may need to swap.
#define FORWARD  0
#define REVERSE 1

// Motor definitions to make life easier:
#define MOTOR_A 0
#define MOTOR_B 1

// Pin Assignments //
//Default pins:
#define DIRA 2 // Direction control for motor A
#define PWMA 3  // PWM control (speed) for motor A
#define DIRB 4 // Direction control for motor B
#define PWMB 11 // PWM control (speed) for motor B

////Alternate pins:
//#define DIRA 8 // Direction control for motor A
//#define PWMA 9 // PWM control (speed) for motor A
//#define DIRB 7 // Direction control for motor B
//#define PWMB 10 // PWM control (speed) for motor B

const uint8_t SX1509_ADDRESS = 0x3E;  // SX1509 I2C address (00)
SensorBar mySensorBar(SX1509_ADDRESS);

void setup()
{
  setupArdumoto(); // Set all pins as outputs
  Serial.begin(9600);  // start serial for output
  Serial.println("Program started.");
  Serial.println();
  
  //For this demo, the IR will only be turned on during reads.
  mySensorBar.setBarStrobe();
  //Other option: Command to run all the time
  //mySensorBar.clearBarStrobe();

  //Default dark on light
  mySensorBar.clearInvertBits();
  //Other option: light line on dark
  //mySensorBar.setInvertBits();
  
  //Don't forget to call .begin() to get the bar ready.  This configures HW.
  uint8_t returnStatus = mySensorBar.begin();
  if(returnStatus){
    Serial.println("sx1509 IC communication OK");
  }
  else{
    Serial.println("sx1509 IC communication FAILED!");
    while(1);
  }
  Serial.println();
}

void loop()
{
  int pos = mySensorBar.getPosition();
    
  Serial.println("Position");
  Serial.println(pos);

  float left_motor_speed_for_sensor_reading[37] = {255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 251.94444444444446, 248.88888888888889, 245.83333333333334, 242.77777777777777, 239.72222222222223, 236.66666666666666, 233.61111111111111, 230.55555555555554, 227.5, 224.44444444444446, 221.38888888888889, 218.33333333333334, 215.27777777777777, 212.22222222222223, 209.16666666666666, 206.11111111111111, 203.05555555555554, 200.0}
  float right_motor_speed_for_sensor_reading[37] = {200.0, 203.05555555555554, 206.11111111111111, 209.16666666666666, 212.22222222222223, 215.27777777777777, 218.33333333333334, 221.38888888888889, 224.44444444444446, 227.5, 230.55555555555554, 233.61111111111111, 236.66666666666666, 239.72222222222223, 242.77777777777777, 245.83333333333334, 248.88888888888889, 251.94444444444446, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0}

  driveArdumoto(MOTOR_A, FORWARD, left_motor_speed_for_sensor_reading[pos]);
  driveArdumoto(MOTOR_B, FORWARD, right_motor_speed_for_sensor_reading[pos]);
}

// driveArdumoto drives 'motor' in 'dir' direction at 'spd' speed
void driveArdumoto(byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_A)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == MOTOR_B)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}

// stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}

// setupArdumoto initialize all pins
void setupArdumoto()
{
  // All pins should be setup as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
