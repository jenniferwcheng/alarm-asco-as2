// DS3231_Serial_Easy
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo of how to use my DS3231-library to 
// quickly send time and date information over a serial link
//
// To use the hardware I2C (TWI) interface of the Arduino you must connect
// the pins as follows:
//
// Arduino Uno/2009:
// ----------------------
// DS3231:  SDA pin   -> Arduino Analog 4 or the dedicated SDA pin
//          SCL pin   -> Arduino Analog 5 or the dedicated SCL pin
//
// Arduino Leonardo:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 2 or the dedicated SDA pin
//          SCL pin   -> Arduino Digital 3 or the dedicated SCL pin
//
// Arduino Mega:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA pin
//          SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL pin
//
// Arduino Due:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA1 (Digital 70) pin
//          SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL1 (Digital 71) pin
//
// The internal pull-up resistors will be activated when using the 
// hardware I2C interfaces.
//
// You can connect the DS3231 to any available pin but if you use any
// other than what is described above the library will fall back to
// a software-based, TWI-like protocol which will require exclusive access 
// to the pins used, and you will also have to use appropriate, external
// pull-up resistors on the data and clock signals.
//

#include <DS3231.h>
#include <LiquidCrystal.h>
//#include <DateTime.h>
//#include <DateTimeStrings.h>

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

// Initialize LCD pins
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

// Pressor sensor analog pins
const int FSR_PIN1 = A0; // Pin connected to FSR/resistor divider
const int FSR_PIN2 = A2;
const int FSR_PIN3 = A1;

// Clock pins
const int hourPin = A15;
const int minPin = A14;
const int CONFIRM = 52;

// Booleans
bool timeSet = false;
bool hourSet = false;
bool minuteSet = false;
bool alarmSet = false;

// Constants
// Measure the voltage at 5V and resistance of your 3.3k resistor, and enter
// their value's below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 10000.0; // Measured resistance of 3.3k resistor
int hourTemp = 0;
int hourVal = 0;
int minTemp = 0;
int minVal = 0;
int hourAlarm = 0;
int minAlarm = 0;

void setup()
{  
  // Buttons
  pinMode(ALARM, INPUT);
  pinMode(hourPin, INPUT);
  pinMode(minPin, INPUT);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //lcd.print("ASCO AS2");
  
  // Setup Serial connection
  //Serial.begin(9600);

  // Initialize the rtc object
  rtc.begin();

  // Ask user to set time
  lcd.print("Set current time");

  delay(2000);

  setCurrTime();
  //Serial.println("out of loop");
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(12, 42, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(4, 22, 2019);   // Set the date to January 1st, 2014


}

void loop()
{
  lcd.clear();

  // Print time to LCD
  DateTime t = rtc.getTimeStr();
  lcd.print(rtc.getTimeStr());
  
 /* Serial.print(rtc.getDOWStr());
    Serial.print(rtc.getDateStr());
    Serial.println(rtc.getTimeStr());*/

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);

  delay (1000);
}

void setCurrTime(){
  while(timeSet == false){
    if(digitalRead(CONFIRM) == HIGH){
      timeSet = true;
    }
    hourTemp = analogRead(hourPin);
    hourVal = map(hourTemp, 0, 1023, 0, 28) - 4;
    minTemp = analogRead(minPin);
    minVal = map(minTemp, 0, 1023, 0, 71)-11;
    //Serial.println(testHour);
    //Serial.print(hourCount);
    //Serial.print(":");
    //Serial.println(minCount);
    //Serial.println(timeSet);
    //lcd.setCursor(0, 0);
    //lcd.print(hourCount + ":" + minCount);
    lcd.clear();
    if(hourVal < 10){
      lcd.print(0);
    }
    lcd.print(hourVal);
    lcd.print(":");
    if(minVal < 10){
      lcd.print(0);
    }
    lcd.print(minVal);
    delay(200);
  }

  rtc.setTime(hourVal, minVal, 0);
}

void setAlarm(){
  while(alarmSet == false){
    if(digitalRead(CONFIRM) == HIGH){
      timeSet = true;
    }
    hourTemp = analogRead(hourPin);
    hourAlarm = map(hourTemp, 0, 1023, 0, 28) - 4;
    minTemp = analogRead(minPin);
    minAlarm = map(minTemp, 0, 1023, 0, 71)-11;
    //Serial.println(testHour);
    //Serial.print(hourCount);
    //Serial.print(":");
    //Serial.println(minCount);
    //Serial.println(timeSet);
    //lcd.setCursor(0, 0);
    //lcd.print(hourCount + ":" + minCount);
    lcd.clear();
    if(hourVal < 10){
      lcd.print(0);
    }
    lcd.print(hourVal);
    lcd.print(":");
    if(minVal < 10){
      lcd.print(0);
    }
    lcd.print(minVal);
    delay(200);
  }

  rtc.setTime(hourVal, minVal, 0);
}
