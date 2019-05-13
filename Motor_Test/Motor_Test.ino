    /* 
 Stepper Motor Control - one revolution
 
 This program drives a unipolar or bipolar stepper motor. 
 The motor is attached to digital pins 8 - 11 of the Arduino.
 
 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.  
 
  
 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe
 
 */

#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
                                     // for your motor

//Motor A
const int enblA = 12;
int spdA = 255; // PWM value between 0 and 255
const int motorPin1  = 8;  // Pin 14 of L293
const int motorPin2  = 9;  // Pin 10 of L293

// Motor B
const int motorPin3  = 10;  // Pin 14 of L293
const int motorPin4  = 11;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 4,5,6,7);            

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(10);
  // initialize the serial port:
  Serial.begin(9600);

    //Set pins as outputs
    pinMode(enblA, OUTPUT);
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
    
    //Motor Control - Motor A: motorPin1,motorpin2

    //This code  will turn Motor A clockwise for 2 sec.
    analogWrite(enblA, spdA);
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
    delay(2000); 
    
    /*//This code will turn Motor A counter-clockwise for 2 sec.
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    delay(2000);*/

    //delay(100);

    //And this code will stop motors
    //digitalWrite(motorPin3, LOW);
    //digitalWrite(motorPin4, LOW);
}

void loop() {
  // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);
  
   // step one revolution in the other direction:
  //Serial.println("counterclockwise");
  //myStepper.step(-stepsPerRevolution);
  //delay(500); 
}
