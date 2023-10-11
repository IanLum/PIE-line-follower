/*
PIE MP3: This code makes a robot follow along a path by using data collected by two sensors as it drives.
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

//sets speed of all wheels
int speed = 25;

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

  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);

}

void loop() {
  uint8_t i;

  leftValue = analogRead(leftSigPin); // Read the sensor
  rightValue = analogRead(rightSigPin); // Read the sensor

  int leftDir = FORWARD;
  int rightDir = FORWARD;
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
  // Double line detected: go left slowly
  else if (leftValue >= leftCutOff and rightValue >= rightCutOff) { // Both sensors are on the line?
    leftDir = BACKWARD;
    serialOut = "Double Line";
  } 

  leftMotor->run(leftDir);
  rightMotor->run(rightDir);
  leftMotor->setSpeed(speed);
  rightMotor->setSpeed(speed);
  delay(5);
  Serial.println(serialOut);
}
