#include <DS3231.h>
#include <LiquidCrystal.h>
#include <Stepper.h>

// Stepper motor: blanket snatcher
const int stepsBlanket = 200; // Steps/rev
Stepper blanket(stepsBlanket, 4,5,6,7);

// Stepper motor: slapper 
const int stepsSlapper = 100; // Steps/rev
Stepper slapper(stepsSlapper, 8,9,10,11);

// Slapper variables
int xPos = 0;
int currPos = 0;
int slapperDelay = 5000; // 5 seconds

// DC Motor: slapper
const int enblSlap = 0;
const int inSlap1 = 0;
const int inSlap2 = 0;

// DC Motor: sprayer
const int enlbSpray = 0;
const int inSpray1 = 0;
const int inSpray2 = 0;
bool sprayed = false;

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

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL); // SDA: 20, SCL: 21

// Initialize LCD pins
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

// Clock pins
const int hourPin = A15; // Potentiometer
const int minPin = A14; // Potentiometer
const int CONFIRM = 52; // Button

// Booleans for alarm setting
bool timeSet = false;
bool hourSet = false;
bool minuteSet = false;
bool alarmSet = false;

// Clock Constants
int hourTime = 0;
int minTime = 0;
Time t;

// Alarm constants
int hourAlarm = 0;
int minAlarm = 0;

// Temp variables
int hourTemp = 0;
int minTemp = 0;

// Buzzer
const int buzzer = 40; 

void setup()
{  
  // Initialize inputs
  pinMode(CONFIRM, INPUT);
  pinMode(hourPin, INPUT);
  pinMode(minPin, INPUT);
  pinMode(FSR_PIN1, INPUT);
  pinMode(FSR_PIN2, INPUT);
  pinMode(FSR_PIN3, INPUT);
  pinMode(FSR_PIN4, INPUT);
  pinMode(FSR_PIN5, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(enlbSlap, OUTPUT);
  pinMode(enlbSpray, OUTPUT);
  pinMode(inSlap1, OUTPUT);
  pinMode(inSlap2, OUTPUT);
  pinMode(inSpray1, OUTPUT);
  pinMode(inSpray2, OUTPUT);

  // Motors initially off
  digitalWrite(inSpray1, LOW);
  digitalWrite(inSpray2, LOW);
  digitalWrite(inSlap1, LOW);
  digitalWrite(inSlap2, LOW);
  
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //lcd.print("ASCO AS2");
  
  // Setup Serial connection
  //Serial.begin(9600);

  // Initialize the rtc object
  rtc.begin();

  // Ask user to set time
  lcd.print("Set current time:");

  delay(3000);

  // Call function to begin setting the current time
  setCurrTime();

  lcd.clear();
  lcd.print("Time was set.");
  delay(1000);
}

void loop()
{
  lcd.clear();
  
  // Get data from the DS3231
  t = rtc.getTime();

  lcd.clear();
  lcd.print(t.hour, DEC);
  lcd.print(":");
  lcd.print(t.min, DEC);
  lcd.print(":");
  lcd.print(t.sec, DEC);

  // See if user wants to set new alarm
  if(digitalRead(CONFIRM) == HIGH){
      setAlarm();
      lcd.clear();
      lcd.print("Alarm was set.");
      delay(2000);
  }

  if(alarmMatch()){
    startAlarm();
  }
  delay (1000);
}

void setCurrTime(){
  while(timeSet == false){ // Stay in loop until CONFIRM button is pressed
    if(digitalRead(CONFIRM) == HIGH){
      timeSet = true;
    }
    
    // Read values from potentiometers
    hourTemp = analogRead(hourPin);
    hourTime = map(hourTemp, 0, 1023, 0, 28) - 4;
    minTemp = analogRead(minPin);
    minTime = map(minTemp, 0, 1023, 0, 71) - 11;

    // Display on lcd
    lcd.clear();
    if(hourTime < 10){
      lcd.print(0);
    }
    lcd.print(hourTime);
    lcd.print(":");
    if(minTime < 10){
      lcd.print(0);
    }
    lcd.print(minTime);
    delay(200);
  }

  // Set time when out of loop
  rtc.setTime(hourTime, minTime, 0);
}

void setAlarm(){
  lcd.clear();
  lcd.print('Set an alarm:');
  delay(3000);
  
  while(alarmSet == false){
    if(digitalRead(CONFIRM) == HIGH){
      timeSet = true;
    }
    hourTemp = analogRead(hourPin);
    hourAlarm = map(hourTemp, 0, 1023, 0, 28) - 4;
    minTemp = analogRead(minPin);
    minAlarm = map(minTemp, 0, 1023, 0, 71)-11;

    lcd.clear();
    if(hourAlarm < 10){
      lcd.print(0);
    }
    lcd.print(hourAlarm);
    lcd.print(":");
    if(minAlarm < 10){
      lcd.print(0);
    }
    lcd.print(minAlarm);
    delay(200);
  }
  lcd.clear();
  lcd.print('Alarm set for: ');
  if(hourAlarm < 10){
      lcd.print(0);
  }
  lcd.print(hourAlarm);
  lcd.print(":");
  if(minAlarm < 10){
    lcd.print(0);
  }
  lcd.print(minAlarm);

  //Set alarm on RTC
  //rtc.setAlarmTime(hourAlarm, minAlarm, 0);
  //rtc.enableAlarm(rtc.MATCH_HHMMSS);
  delay(3000);
}

bool alarmMatch(){
  if(t.hour == hourAlarm && t.min == minAlarm){
    return true;
  }
  return false;
}

void startAlarm(){
  lcd.clear();

  // For debugging purposes
  lcd.print("Wake up!");
  delay(1000);
  
  unsigned long StartTime = millis();
  unsigned long CurrentTime = 0;
  unsigned long ElapsedTime = 0;
  
  while(isOnBed){ 
    // Time elapsed since alarm started
    CurrentTime = millis();
    ElapsedTime = CurrentTime - StartTime;

    while(ElapsedTime < 300000){ // 5 min
      // Buzzer
      tone(buzzer, 1000); // kHz
      delay(1000);        
      noTone(buzzer);     // Stop sound
      delay(1000); 

      // Blanket snatcher
      blanket.setSpeed(200); // rpm
      blanket.step(stepBlanket);
    }
    while(ElapsedTime >= 300000 && ElapsedTime < 600000){ // 10 minutes
      // Buzzer
      tone(buzzer, 3000); // kHz
      delay(500);        
      noTone(buzzer);     // Stop sound
      delay(500);      

      // Blanket snatcher

      // Slapper positioning
      xPos = getXPos();
      if(currPos != xPos){
        // Left
        if(xPos == -1){
          slapper.speed(100);
          slapper.step(-stepSlapper);
          if(currPos == 0){
            delay(slapperDelay); // delay
          }
          if(currPos == 1){
            delay(2*slapperDelay); // delay
          }
          slapper.speed(0); // stop moving
        }
        // Right
        else if(xPos == 1){
          slapper.speed(100);
          slapper.step(stepSlapper);
          if(currPos == 0){
            delay(slapperDelay); // delay
          }
          if(currPos == 1){
            delay(2*slapperDelay); // delay
          }
          slapper.speed(0); // stop moving
        }
        // Middle
        else{
          slapper.speed(100);
          if(currPos == -1){
            slapper.step(-stepSlapper); 
          }
          else{
            slapper.step(stepSlapper);
          }
          delay(5000); // delay
          slapper.speed(0); // stop moving
        }
      }

      // Slapping
      
      // Spray fart once
      if(!sprayed){
        digitalWrite(inSpray1, HIGH);
        digitalWrite(inSpray2, LOW);
        delay(1000);
        digitalWrite(inSpray1, LOW);
      }
    }
    while(ElapsedTime >= 900000){ // 15 minutes
      // Buzzer
      tone(buzzer, 7000); // Send kHz
      delay(100);        
      noTone(buzzer);     // Stop sound
      delay(100);      

      // Blanket snatcher

      // Slapper
 
      // Fart
    }
  }
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
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    delay(50);
  }

  if (fsrADC2 != 0) // If the analog reading is non-zero
  {
    fsrV2 = fsrADC2 * VCC / 1023.0;
    fsrR2 = R_DIV * (VCC / fsrV2 - 1.0);
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    delay(50);
  }

  if (fsrADC3 != 0) // If the analog reading is non-zero
  {
    fsrV3 = fsrADC3 * VCC / 1023.0;
    fsrR3 = R_DIV * (VCC / fsrV3 - 1.0);
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    delay(50);
  }

  if (fsrADC4 != 0) // If the analog reading is non-zero
  {
    fsrV4 = fsrADC4 * VCC / 1023.0;
    fsrR4 = R_DIV * (VCC / fsrV4 - 1.0);
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    delay(50);
  }

  if (fsrADC5 != 0) // If the analog reading is non-zero
  {
    fsrV5 = fsrADC5 * VCC / 1023.0;
    fsrR5 = R_DIV * (VCC / fsrV5 - 1.0);
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    delay(50);
  }
  
  if(fsrR1 < 3000 || fsrR2 < 3000 || fsrR3 < 3000 || fsrR4 < 3000 || fsrR5 <  3000){
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
  
  if (fsrADC1 != 0) // If the analog reading is non-zero
  {
    fsrV1 = fsrADC1 * VCC / 1023.0;
    fsrR1 = R_DIV * (VCC / fsrV1 - 1.0);
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    delay(50);
  }

  if (fsrADC2 != 0) // If the analog reading is non-zero
  {
    fsrV2 = fsrADC2 * VCC / 1023.0;
    fsrR2 = R_DIV * (VCC / fsrV2 - 1.0);
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    delay(50);
  }

  if (fsrADC3 != 0) // If the analog reading is non-zero
  {
    fsrV3 = fsrADC3 * VCC / 1023.0;
    fsrR3 = R_DIV * (VCC / fsrV3 - 1.0);
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    delay(50);
  }

  if (fsrADC4 != 0) // If the analog reading is non-zero
  {
    fsrV4 = fsrADC4 * VCC / 1023.0;
    fsrR4 = R_DIV * (VCC / fsrV4 - 1.0);
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    delay(50);
  }

  if (fsrADC5 != 0) // If the analog reading is non-zero
  {
    fsrV5 = fsrADC5 * VCC / 1023.0;
    fsrR5 = R_DIV * (VCC / fsrV5 - 1.0);
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    delay(50);
  }
  
  if(fsrR1 > (fsrR3 + 3000)){
    return -1;
  }
  if(fsrR3 > (fsrR1 + 3000)){
    return 1;
  }

  return 0;
}
