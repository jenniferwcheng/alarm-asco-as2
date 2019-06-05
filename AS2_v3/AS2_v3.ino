/*#include <DS3231.h>
#include <LiquidCrystal.h>*/
#include <Stepper.h>

unsigned long previousMillis = 0;
int enVal = 0;

// Stepper motor: blanket snatcher
const int PUL = 4;
const int DIR = 5;
const int ENA = 6;
  
// Stepper motor: slapper 
const int stepsSlapper = 100; // Steps/rev
const int stepperEn = 2; // Stepper motor enable pin
Stepper slapper(stepsSlapper, 4,5,6,7);

// Slapper variables
int xPos = 0;
int currPos = 0;
int slapperDelay = 7500; // 5 seconds
const int LEFT = -1;
const int RIGHT = 1;
const int MIDDLE = 0;
const int stepperSpeed = 300;

// DC Motor: slapper
const int enblSlap = 12;
const int inSlap1 = 48;
const int inSlap2 = 49;

// DC Motor: sprayer
const int enlbSpray = 0;
const int inSpray1 = 0;
const int inSpray2 = 0;

// Pressor sensor analog pins
const int FSR_PIN1 = A0;
const int FSR_PIN2 = A1;
const int FSR_PIN3 = A2;
const int FSR_PIN4 = A3;
const int FSR_PIN5 = A4;

// Pressure sensor variables
const float VCC = 5; // Ardunio 5V line
const float R_DIV = 10000.0; // 10k resistor
float fsrR1; float fsrV1; 
float fsrR2; float fsrV2;
float fsrR3; float fsrV3; 
float fsrR4; float fsrV4;
float fsrR5; float fsrV5; 
int fsrADC1; int fsrADC2;
int fsrADC3; int fsrADC4;
int fsrADC5;

void setup()
{  
  // Initialize inputs and outputs
  pinMode(FSR_PIN1, INPUT);
  pinMode(FSR_PIN2, INPUT);
  pinMode(FSR_PIN3, INPUT);
  pinMode(FSR_PIN4, INPUT);
  pinMode(FSR_PIN5, INPUT);
  pinMode(enblSlap, OUTPUT);
  pinMode(enlbSpray, OUTPUT);
  pinMode(inSlap1, OUTPUT);
  pinMode(inSlap2, OUTPUT);
  pinMode(inSpray1, OUTPUT);
  pinMode(inSpray2, OUTPUT);
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(stepperEn, OUTPUT);

  // Motors initially off
  digitalWrite(inSpray1, LOW);
  digitalWrite(inSpray2, LOW);
  digitalWrite(inSlap1, LOW);
  digitalWrite(inSlap2, LOW);
  digitalWrite(DIR,LOW);
  digitalWrite(ENA,LOW);
  digitalWrite(PUL,LOW);
  
  Serial.begin(9600);
  Serial.print("Initializing");

  slapper.setSpeed(stepperSpeed);

  int x = 1;
  while(x <10) {
  Serial.print(".");
  delay(1000);
  x++;
  }
  Serial.println(".");
  delay(1000);
}

void loop()
{
  // Turn motors off
  digitalWrite(inSpray1, LOW);
  digitalWrite(inSpray2, LOW);
  digitalWrite(inSlap1, LOW);
  digitalWrite(inSlap2, LOW);
  if(isOnBed()){
    Serial.println("Someone is on the bed... starting alarm");
    //delay(300);
    //xPos = getXPos();
    //Serial.print("Position: ");
    //Serial.println(xPos);
    startAlarm();
  }
  else{
    Serial.println("No one is on the bed");
  }
  delay (1000);
}

// Checks if user is on bed based on pressure sensor data
bool isOnBed(){
  fsrADC1 = analogRead(FSR_PIN1);
  fsrADC2 = analogRead(FSR_PIN2);
  fsrADC3 = analogRead(FSR_PIN3);
  fsrADC4 = analogRead(FSR_PIN4);
  fsrADC5 = analogRead(FSR_PIN5);
  
  if (fsrADC1 != 0) // If the analog reading is non-zero
  {
    fsrV1 = fsrADC1 * VCC / 1023.0;
    fsrR1 = R_DIV * (VCC / fsrV1 - 1.0);
  }

  if (fsrADC2 != 0) 
  {
    fsrV2 = fsrADC2 * VCC / 1023.0;
    fsrR2 = R_DIV * (VCC / fsrV2 - 1.0);
  }

  if (fsrADC3 != 0)
  {
    fsrV3 = fsrADC3 * VCC / 1023.0;
    fsrR3 = R_DIV * (VCC / fsrV3 - 1.0);
  }

  if (fsrADC4 != 0)
  {
    fsrV4 = fsrADC4 * VCC / 1023.0;
    fsrR4 = R_DIV * (VCC / fsrV4 - 1.0);
  }

  if (fsrADC5 != 0) 
  {
    fsrV5 = fsrADC5 * VCC / 1023.0;
    fsrR5 = R_DIV * (VCC / fsrV5 - 1.0);
  }
  
  if((fsrR1 < 50000 || fsrR2 < 50000 || fsrR3 < 50000) && fsrR4 < 50000 && fsrR5 <  50000){
    return true;
  }

  return false;
}

int getXPos(){
  fsrADC1 = analogRead(FSR_PIN1);
  fsrADC2 = analogRead(FSR_PIN2);
  fsrADC3 = analogRead(FSR_PIN3);
  fsrADC4 = analogRead(FSR_PIN4);
  fsrADC5 = analogRead(FSR_PIN5);
  
  if (fsrADC1 != 0){
    fsrV1 = fsrADC1 * VCC / 1023.0;
    fsrR1 = R_DIV * (VCC / fsrV1 - 1.0);
  }

  if (fsrADC2 != 0) 
  {
    fsrV2 = fsrADC2 * VCC / 1023.0;
    fsrR2 = R_DIV * (VCC / fsrV2 - 1.0);
  }

  if (fsrADC3 != 0)
  {
    fsrV3 = fsrADC3 * VCC / 1023.0;
    fsrR3 = R_DIV * (VCC / fsrV3 - 1.0);
  }

  if (fsrADC4 != 0)
  {
    fsrV4 = fsrADC4 * VCC / 1023.0;
    fsrR4 = R_DIV * (VCC / fsrV4 - 1.0);
  }

  if (fsrADC5 != 0) {
    fsrV5 = fsrADC5 * VCC / 1023.0;
    fsrR5 = R_DIV * (VCC / fsrV5 - 1.0);
  }
  
  /*Serial.print("FSR1: ");
  Serial.print(fsrR1);
  Serial.print(" | FSR2: ");
  Serial.print(fsrR2);
  Serial.print(" | FSR3: ");
  Serial.println(fsrR3);*/

  
  if(fsrR2 < fsrR1 && fsrR2 < fsrR3){
    return MIDDLE;
  }
  else if(fsrR3 < fsrR1 && fsrR3 < fsrR2){
    return LEFT;
  } else if(fsrR1 < fsrR2 && fsrR1 < fsrR3){
    return RIGHT;
  }
}

void startAlarm(){
  unsigned long StartTime = millis();
  unsigned long CurrentTime = 0;
  unsigned long ElapsedTime = 0;
  int moveTime = 0; // Keeps track of how long to move motor
  
  while(isOnBed()){
    xPos = getXPos();
    if(currPos != xPos){
      Serial.print("Current Position: ");
      Serial.print(currPos);
      Serial.print(" | Next Position: ");
      Serial.println(xPos);

      slapper.setSpeed(stepperSpeed); // Set stepper speed

      // If person's head is in middle
      if(xPos == MIDDLE){
        
        // If slapper is on the left
        if(currPos == LEFT){
          slapper.step(stepsSlapper); // Pos to turn stepper to move slapper right (towards middle)
          unsigned long currentMillis = millis();
          if (currentMillis - previousMillis >= slapperDelay) {
            previousMillis = currentMillis;
            if (enVal == 0) {
              enVal = 255;
            } 
            else {
            enVal = 0;
            }
          }
          
          analogWrite(stepperEn, enVal); 
        } 
        // If slapper is on the right
        else if (currPos == RIGHT) {
          slapper.step(-stepsSlapper); // Neg to turn stepper to move slapper left (towards middle)
          unsigned long currentMillis = millis();
          if (currentMillis - previousMillis >= slapperDelay) {
            previousMillis = currentMillis;
            if (enVal == 0) {
              enVal = 255;
            } 
            else {
            enVal = 0;
            }

            analogWrite(stepperEn, enVal);
          }
        }
      }
      
      // If person's head is on the left
      if(xPos == LEFT){
        
        // If motor is coming from middle
        if(currPos == MIDDLE){
          slapper.step(-stepsSlapper); // Pos to turn stepper to move slapper right (towards middle)
          unsigned long currentMillis = millis();
          if (currentMillis - previousMillis >= slapperDelay) {
            previousMillis = currentMillis;
            if (enVal == 0) {
              enVal = 255;
            } 
            else {
            enVal = 0;
            }
          }
          
          analogWrite(stepperEn, enVal); 
        } 
        // If motor is coming from right
        else if (currPos == RIGHT) {
          slapper.step(-stepsSlapper); // Neg to turn stepper to move slapper left (towards middle)
          unsigned long currentMillis = millis();
          if (currentMillis - previousMillis >= 2*slapperDelay) {
            previousMillis = currentMillis;
            if (enVal == 0) {
              enVal = 255;
            } 
            else {
            enVal = 0;
            }

            analogWrite(stepperEn, enVal);
          }
        }
      }
      
      // If person's head is on right
      if(xPos == RIGHT){
        
        // If motor is coming from middle
        if(currPos == MIDDLE){
          slapper.step(stepsSlapper); // Pos to turn stepper to move slapper right (towards middle)
          unsigned long currentMillis = millis();
          if (currentMillis - previousMillis >= slapperDelay) {
            previousMillis = currentMillis;
            if (enVal == 0) {
              enVal = 255;
            } 
            else {
            enVal = 0;
            }
          }
          
          analogWrite(stepperEn, enVal); 
        } 
        // If motor is coming from right
        else if (currPos == LEFT) {
          slapper.step(stepsSlapper); // Neg to turn stepper to move slapper left (towards middle)
          unsigned long currentMillis = millis();
          if (currentMillis - previousMillis >= 2*slapperDelay) {
            previousMillis = currentMillis;
            if (enVal == 0) {
              enVal = 255;
            } 
            else {
            enVal = 0;
            }

            analogWrite(stepperEn, enVal);
          }
        }
      }
    }
    //currPos = xPos;
  }
  //analogWrite(stepperEn, 0);
}
