/*
PIE MP3: This code makes a robot follow along a path by using data collected by two sensors as it drives.
*/

#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();


// Assigns ports to motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1); //assigns the left motor to port M1
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2); //assigns the right motor to port M2

// Assigns IR sensor components to pins
int leftSigPin = 4; // For sensor signal
int rightSigPin = 4; // For sensor signal
int leftValue = 0; // Holds the returned value
int rightValue = 0; // Holds the returned value


void setup() {
  Serial.begin(9600);
  pinMode(leftSigPin, INPUT); // signal pin as input
  pinMode(rightSigPin, INPUT); // signal pin as input

  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  leftMotor->setSpeed(20);
  leftMotor->run(FORWARD);
  rightMotor->setSpeed(50);
  rightMotor->run(FORWARD);
  // turn on motor
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}

void loop() {
  uint8_t i;

  leftValue = digitalRead(leftSigPin); // Read the sensor
  rightValue = digitalRead(rightSigPin); // Read the sensor

  // Go straight
  if (leftValue == LOW and rightValue == LOW) { 
    leftMotor->run(FORWARD);
    rightMotor->run(FORWARD);
    leftMotor->setSpeed(20);
    rightMotor->setSpeed(20);
    delay(5);

    Serial.println("Straight"); // Write on console
  } 

  // Turn right
  else if (leftValue == LOW and rightValue == HIGH) { 
    leftMotor->run(FORWARD);
    rightMotor->run(BACKWARD);
    leftMotor->setSpeed(20);
    rightMotor->setSpeed(20);
    delay(5);
    Serial.println("Right"); // Write on console
  } 

  // Turn left
  else if (leftValue == HIGH and rightValue == LOW) { 
    leftMotor->run(BACKWARD);
    rightMotor->run(FORWARD);
    leftMotor->setSpeed(20);
    rightMotor->setSpeed(20);
    delay(5);
    Serial.println("Left"); // Write on console
  } 

  // Double line detected 
  else if (leftValue == HIGH and rightValue == HIGH) {
    leftMotor->run(FORWARD);
    rightMotor->run(FORWARD);
    leftMotor->setSpeed(10);
    rightMotor->setSpeed(10);
    delay(5);
    Serial.println("Double Line Detected"); // Write on console
  } 

}