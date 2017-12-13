#include <iostream>
#include <math.h>

/*
Hi Jon.

I just updated this so that it should work -- theoretically. Obviously, I can't
compile this to test it. Feel free to call me if you need help.

- Jeremy
*/

// Speed assignment
int MAXSPEED = 255;
int MINSPEED = 25;
int diff = MAXSPEED - MINSPEED;
float left_motor_speed_for_sensor_reading[37] = {};
float right_motor_speed_for_sensor_reading[37] = {};

// Clockwise and counter-clockwise definitions, and others
#define FORWARD  0
#define REVERSE 1
#define MOTOR_A 0
#define MOTOR_B 1

// Pin Assignments
#define DIRA 2 // Direction control for motor A
#define PWMA 3  // PWM control (speed) for motor A
#define DIRB 4 // Direction control for motor B
#define PWMB 11 // PWM control (speed) for motor B

const uint8_t SX1509_ADDRESS = 0x3E;  // SX1509 I2C address (00)
SensorBar mySensorBar(SX1509_ADDRESS);

void setup() {
  setupMotorSpeed();
  setupArdumoto(); // Set all pins as outputs
  Serial.begin(9600);  // start serial for output
  Serial.println("Program started.");
  Serial.println();
  
  mySensorBar.setBarStrobe();
  mySensorBar.clearInvertBits();
  
  uint8_t returnStatus = mySensorBar.begin();
  if(returnStatus){
    Serial.println("sx1509 IC communication OK");
  }
  else {
    Serial.println("sx1509 IC communication FAILED!");
    while(1);
  }
  
  Serial.println();
}

void loop() {
  int pos = mySensorBar.getPosition();
    
  //Serial.println("Position");
  //Serial.println(pos);

  driveArdumoto(MOTOR_A, FORWARD, left_motor_speed_for_sensor_reading[pos]);
  driveArdumoto(MOTOR_B, FORWARD, right_motor_speed_for_sensor_reading[pos]);
}

void driveArdumoto(byte motor, byte dir, byte spd) {
  if (motor == MOTOR_A) {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == MOTOR_B) {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}

// stopArdumoto makes a motor stop
void stopArdumoto(byte motor) {
  driveArdumoto(motor, 0, 0);
}

// setupArdumoto initialize all pins
void setupArdumoto() {
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

void setupMotorSpeed(){
    for (int i = 0; i < 37; i = i + 1){
    if (i > 16) {
      left_motor_speed_for_sensor_reading[i] = MAXSPEED;
      right_motor_speed_for_sensor_reading[i] = MINSPEED - diff * sin(1.5*i);
    }
    else if (i == 16) {
      left_motor_speed_for_sensor_reading[i] = MAXSPEED;
      right_motor_speed_for_sensor_reading[i] = MAXSPEED;
    }
    else if (i < 16) {
      left_motor_speed_for_sensor_reading[i] = MAXSPEED - diff * sin(1.5*i);;
      right_motor_speed_for_sensor_reading[i] = MINSPEED;
    }
  }
}
