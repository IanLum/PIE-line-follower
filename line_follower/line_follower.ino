/*
PIE Mini-Project 3: This code makes a robot follow along a path by using data collected by two sensors as it drives.
*/

#include <string.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();


// Assigns ports to motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4); //assigns the left motor to port M4
Adafruit_DCMotor *rightMotor = AFMS.getMotor(3); //assigns the right motor to port M3

// Assigns IR sensor components to pins
int leftSigPin = A1; // For sensor signal
int rightSigPin = A0; // For sensor signal
int leftValue = 0; // Holds the returned value
int rightValue = 0; // Holds the returned value

// Assigns cut off readings for each sensor. If the sensor reads this value or higher then, based on our measurements, it is on the line.
int rightCutOff = 800; // Cut off reading for the right sensor.
int leftCutOff = 800; // Cut off reading for the left sensor.

float speedMultiplier = 1;

void setup() {
  Serial.begin(9600);

  pinMode(leftSigPin, INPUT); // signal pin as input
  pinMode(rightSigPin, INPUT); // signal pin as input

  Serial.println("Code Begin!");

  // checks the connection of the Motor Shield
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // turn on motors
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);

}
void loop() {
  leftValue = analogRead(leftSigPin); // Read the sensor
  rightValue = analogRead(rightSigPin); // Read the sensor

  if (Serial.available() > 0) {
    speedMultiplier = Serial.parseFloat(); // SET SERIAL INPUT TO "No Line Ending"
  }

  // sets variables to be the base speed and directions for the wheels in the "forward" case.
  // these variables might be modified later on in the loop based on the sensor values collected.
  int leftDir = FORWARD;
  int rightDir = FORWARD;
  int leftSpeed = 25;
  int rightSpeed = 25;
  // sets base delay time
  int duration = 5;
  String serialOut;

  // Go straight
  if (leftValue < leftCutOff and rightValue < rightCutOff) { // Neither sensor is on the line
    serialOut = "Straight";
  } 
  // Turn right
  else if (leftValue < leftCutOff and rightValue >= rightCutOff) { // Right sensor is on the line
    rightDir = BACKWARD;
    serialOut = "Right";
  } 
  // Turn left
  else if (leftValue >= leftCutOff and rightValue < rightCutOff) { // Left sensor is on the line
    leftDir = BACKWARD;
    serialOut = "Left";
  } 
  // Double line detected: go right for a set amount of time
  else if (leftValue >= leftCutOff and rightValue >= rightCutOff) { // Both sensors are on the line -- used for sharp corners
    rightSpeed = 20;
    rightDir = BACKWARD;
    leftSpeed = 30;
    duration = 600;
    serialOut = "Double Line";
  } 

  // move along the line with the newly defined speeds and wheel directions for a specified duration.
  leftMotor->run(leftDir);
  rightMotor->run(rightDir);
  leftMotor->setSpeed(leftSpeed*speedMultiplier);
  rightMotor->setSpeed(rightSpeed*speedMultiplier);
  Serial.println(serialOut);
  delay(duration);
}
