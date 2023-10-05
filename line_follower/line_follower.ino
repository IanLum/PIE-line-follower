/*
PIE MP3: This code makes a robot follow along a path by using data collected by two sensors as it drives.
*/

#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();


// Assigns ports to motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(4); //assigns the left motor to port M4
Adafruit_DCMotor *rightMotor = AFMS.getMotor(3); //assigns the right motor to port M3

// Assigns IR sensor components to pins
int leftSigPin = A5; // For sensor signal
int rightSigPin = A4; // For sensor signal
int leftValue = 0; // Holds the returned value
int rightValue = 0; // Holds the returned value

// Assigns cut off readings for each sensor. If the sensor reads this value or higher then, based on our measurements, it is on the line.
int rightCutOff = 800; // Cut off reading for the right sensor.
int leftCutOff = 800; // Cut off reading for the left sensor.

void setup() {
  Serial.begin(9600);

  pinMode(leftSigPin, INPUT); // signal pin as input
  pinMode(rightSigPin, INPUT); // signal pin as input

  Serial.println("Code Begin!");

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
  rightMotor->run(BACKWARD); //Right motor's "backward" is actually forward
  // turn on motor
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}

void loop() {
  uint8_t i;

  leftValue = digitalRead(leftSigPin); // Read the sensor
  rightValue = digitalRead(rightSigPin); // Read the sensor

  // Go straight
  if (leftValue < leftCutOff and rightValue < rightCutOff) { // Neither sensor is on the line
    leftMotor->run(FORWARD);
    rightMotor->run(BACKWARD);
    leftMotor->setSpeed(100);
    rightMotor->setSpeed(100);
    delay(5);

    Serial.println("Straight"); // Write on console
  } 

  // Turn right
  else if (leftValue < leftCutOff and rightValue >= rightCutOff) { // Right sensor is on the line
    leftMotor->run(FORWARD);
    rightMotor->run(FORWARD);
    leftMotor->setSpeed(100);
    rightMotor->setSpeed(100);
    delay(5);
    Serial.println("Right"); // Write on console
  } 

  // Turn left
  else if (leftValue >= leftCutOff and rightValue < rightCutOff) { // Left sensor is on the line
    leftMotor->run(BACKWARD);
    rightMotor->run(BACKWARD);
    leftMotor->setSpeed(100);
    rightMotor->setSpeed(100);
    delay(5);
    Serial.println("Left"); // Write on console
  } 

  // Double line detected: go forward slowly
  else if (leftValue >= leftCutOff and rightValue >= rightCutOff) { // Both sensors are on the line?
    leftMotor->run(FORWARD);
    rightMotor->run(BACKWARD);
    leftMotor->setSpeed(100);
    rightMotor->setSpeed(100);
    delay(5);
    Serial.println("Double Line Detected"); // Write on console
  } 

}
