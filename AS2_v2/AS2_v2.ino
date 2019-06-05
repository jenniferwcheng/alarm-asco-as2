#include <DS3231.h>
#include <LiquidCrystal.h>
#include <Stepper.h>

// Stepper motor: blanket snatcher
const int PUL = 4;
const int DIR = 5;
const int ENA = 6;
  
// Stepper motor: slapper 
const int stepsSlapper = 100; // Steps/rev
Stepper slapper(stepsSlapper, 4,5,6,7);

// Slapper variables
int xPos = 0;
int currPos = -1;
int slapperDelay = 5000; // 5 seconds
const int LEFT = -1;
const int RIGHT = 1;
const int MIDDLE = 0;
const int stepperSpeed = 100;

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
const int FSR_PIN2 = A2;
const int FSR_PIN3 = A1;
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

  // Motors initially off
  digitalWrite(inSpray1, LOW);
  digitalWrite(inSpray2, LOW);
  digitalWrite(inSlap1, LOW);
  digitalWrite(inSlap2, LOW);
  digitalWrite(DIR,LOW);
  digitalWrite(ENA,LOW);
  digitalWrite(PUL,LOW);
  
  Serial.begin(9600);
  Serial.println("Initializing..");

  delay(3000);
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
    delay(300);
    xPos = getXPos();
    Serial.print("Position: ");
    Serial.println(xPos);
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
    //Serial.print(String(fsrR1) + "| ");
    delay(50);
  }

  if (fsrADC2 != 0) 
  {
    fsrV2 = fsrADC2 * VCC / 1023.0;
    fsrR2 = R_DIV * (VCC / fsrV2 - 1.0);
    //Serial.print("String(fsrR2) + "| ");
    delay(50);
  }

  if (fsrADC3 != 0)
  {
    fsrV3 = fsrADC3 * VCC / 1023.0;
    fsrR3 = R_DIV * (VCC / fsrV3 - 1.0);
    //Serial.print(String(fsrR3) + "| ");
    delay(50);
  }

  if (fsrADC4 != 0)
  {
    fsrV4 = fsrADC4 * VCC / 1023.0;
    fsrR4 = R_DIV * (VCC / fsrV4 - 1.0);
    //Serial.print(String(fsrR4) + "| ");
    delay(50);
  }

  if (fsrADC5 != 0) 
  {
    fsrV5 = fsrADC5 * VCC / 1023.0;
    fsrR5 = R_DIV * (VCC / fsrV5 - 1.0);
    //Serial.println(String(fsrR5));
    delay(50);
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
    //Serial.print(String(fsrR1) + "| ");
    delay(50);
  }

  if (fsrADC2 != 0) 
  {
    fsrV2 = fsrADC2 * VCC / 1023.0;
    fsrR2 = R_DIV * (VCC / fsrV2 - 1.0);
    //Serial.print("String(fsrR2) + "| ");
    delay(50);
  }

  if (fsrADC3 != 0)
  {
    fsrV3 = fsrADC3 * VCC / 1023.0;
    fsrR3 = R_DIV * (VCC / fsrV3 - 1.0);
    //Serial.print(String(fsrR3) + "| ");
    delay(50);
  }

  if (fsrADC4 != 0)
  {
    fsrV4 = fsrADC4 * VCC / 1023.0;
    fsrR4 = R_DIV * (VCC / fsrV4 - 1.0);
    //Serial.print(String(fsrR4) + "| ");
    delay(50);
  }

  if (fsrADC5 != 0) {
    fsrV5 = fsrADC5 * VCC / 1023.0;
    fsrR5 = R_DIV * (VCC / fsrV5 - 1.0);
    //Serial.println(String(fsrR5));
    delay(50);
  }
  
  if(fsrR1 > (fsrR3 + 3000)){
    return LEFT;
  }
  if(fsrR3 > (fsrR1 + 3000)){
    return RIGHT;
  }

  return MIDDLE;
}

void startAlarm(){
  unsigned long StartTime = millis();
  unsigned long CurrentTime = 0;
  unsigned long ElapsedTime = 0;
  
  while(isOnBed()){
    // Time elapsed since alarm started
    CurrentTime = millis();
    ElapsedTime = CurrentTime - StartTime;

    /*if(ElapsedTime < 300000){ // 5 min
      // Blanket snatcher starts
      Serial.println("Starting blanket snatching");

      // Towards foot
      digitalWrite(DIR,HIGH);
      digitalWrite(ENA,HIGH);
      digitalWrite(PUL,HIGH);
      delayMicroseconds(50);
      digitalWrite(PUL,LOW);
      delayMicroseconds(50);
      delay(5000);
      
      digitalWrite(DIR,LOW);
      digitalWrite(ENA,HIGH);
      digitalWrite(PUL,HIGH);
      delayMicroseconds(50);
      digitalWrite(PUL,LOW);
      delayMicroseconds(50);
      delay(5000);
    }*/
    if(ElapsedTime < 300000){//else if(ElapsedTime >= 300000 && ElapsedTime < 600000){ // 10 minutes 
      // Pull blanket all the way
      /*digitalWrite(DIR,HIGH);
      digitalWrite(ENA,HIGH);
      digitalWrite(PUL,HIGH);
      delayMicroseconds(50);
      digitalWrite(PUL,LOW);
      delayMicroseconds(50);*/
      
      Serial.println("Starting slapper");   
      // Slapper positioning
      xPos = getXPos();
      if(currPos != xPos){
        // Left
        if(xPos == LEFT){
          slapper.setSpeed(stepperSpeed);
          slapper.step(-stepsSlapper);
          if(currPos == MIDDLE){
            delay(slapperDelay); // delay
          }
          if(currPos == RIGHT){
            delay(2*slapperDelay); // delay
          }
          slapper.setSpeed(0); // stop moving
        }
        // Right
        else if(xPos == RIGHT){
          slapper.setSpeed(stepperSpeed);
          slapper.step(stepsSlapper);
          if(currPos == MIDDLE){
            delay(slapperDelay); // delay
          }
          if(currPos == LEFT){
            delay(2*slapperDelay); // delay
          }
          slapper.setSpeed(0); // stop moving
        }
        // Middle
        else{
          slapper.setSpeed(stepperSpeed);
          if(currPos == LEFT){
            slapper.step(-stepsSlapper); 
          }
          else{
            slapper.step(stepsSlapper);
          }
          delay(5000); // delay
          slapper.setSpeed(0); // stop moving
        }
        currPos = xPos;
      }

      // Slapping
      //analogWrite(enblSlap, 100);
      //digitalWrite(inSlap1, HIGH);
      //digitalWrite(inSlap2, LOW);
    }
    /*else if(ElapsedTime >= 900000){ // 15 minutes   
      // Spray fart every 10 seconds
      Serial.println("Starting fart spray");
      digitalWrite(inSpray1, HIGH);
      digitalWrite(inSpray2, LOW);
      delay(1000);
      digitalWrite(inSpray1, LOW);
      delay(10000);
      }
    }
    // Slapper
    analogWrite(enblSlap, 255); // full speed
    */
  }
}
