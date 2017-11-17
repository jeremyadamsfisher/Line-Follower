    /* Ardumoto Example Sketch
  by: Jim Lindblom
  date: November 8, 2013
  license: Public domain. Please use, reuse, and modify this 
  sketch!

  Adapted to v20 hardware by: Marshall Taylor
  date: March 31, 2017

  Three useful functions are defined:
    setupArdumoto() -- Setup the Ardumoto Shield pins
    driveArdumoto([motor], [direction], [speed]) -- Drive [motor] 
      (0 for A, 1 for B) in [direction] (0 or 1) at a [speed]
      between 0 and 255. It will spin until told to stop.
    stopArdumoto([motor]) -- Stop driving [motor] (0 or 1).

  setupArdumoto() is called in the setup().
  The loop() demonstrates use of the motor driving functions.
*/

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
  if(returnStatus)
  {
    Serial.println("sx1509 IC communication OK");
  }
  else
  {
    Serial.println("sx1509 IC communication FAILED!");
    while(1);
  }
  Serial.println();
}

void loop()
{
  //uint8_t rawValue = mySensorBar.getRaw();
  int motorSpeed = 255;
  
  Serial.println("Position");
  int pos = mySensorBar.getPosition();

  Serial.println(pos);

  int sensor_neutral_range_left = 4;
  int sensor_neutral_range_right = -1 * sensor_neutral_range_left;

  if(pos > sensor_neutral_range_left){
    // Drive motor A (and only motor A) at various speeds, then stop.
    driveArdumoto(MOTOR_A, REVERSE, motorSpeed*0.5); // Set motor A to REVERSE at max
    driveArdumoto(MOTOR_B, FORWARD, motorSpeed);  // Set motor B to FORWARD at half
    //delay(1000);  // Motor A will spin as set for 1 second
  }
  
  else if(pos < sensor_neutral_range_right){
     driveArdumoto(MOTOR_A, FORWARD, motorSpeed*0.5); // Set motor A to REVERSE at max
    driveArdumoto(MOTOR_B, REVERSE, motorSpeed);  // Set motor B to FORWARD at half
    //delay(1000);  // Motor A will keep trucking for 1 second  
  }

  else {
         driveArdumoto(MOTOR_A, FORWARD, motorSpeed); // Set motor A to REVERSE at max
      driveArdumoto(MOTOR_B, FORWARD, motorSpeed);  // Set motor B to FORWARD at half
      //delay(1000);  // Motor A will keep trucking for 1 second 
    }
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

