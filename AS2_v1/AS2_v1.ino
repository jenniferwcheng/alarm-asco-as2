#include <DS3231.h>
#include <LiquidCrystal.h>

// Pressor sensor analog pins
const int FSR_PIN1 = A0; // Pin connected to FSR/resistor divider
const int FSR_PIN2 = A2;
const int FSR_PIN3 = A1;
const int FSR_PIN4 = A3;
const int FSR_PIN5 = A4;

// Pressure sensor constants
const float VCC = 5; // Measured voltage of Ardunio 5V line
const float R_DIV = 10000.0; // Resistance of 10k resistor
float fsrR1; float fsrV1; 
float fsrR2; float fsrV2;
float fsrR3; float fsrV3; 
float fsrR4; float fsrV4;
float fsrR5; float fsrV5; 

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
const int buzzer = 40; // Pin

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
  
  //Serial.println("out of loop");
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(12, 42, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(4, 22, 2019);   // Set the date to January 1st, 2014


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
  unsigned long StartTime = millis();
  unsigned long CurrentTime = 0;
  unsigned long ElapsedTime = 0;
  while(isOnBed){ 
    // Time elapsed since alarm started
    CurrentTime = millis();
    ElapsedTime = CurrentTime - StartTime;

    // Less than 5 minutes
    while(ElapsedTime < 300000){
      // Buzzer
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(1000);        
      noTone(buzzer);     // Stop sound...
      delay(1000); 
    }
    while(ElapsedTime >= 300000 && ElapsedTime < 100000000){
      // other stuff
    }
  }
}

// Checks if user is on bed based on pressure sensor data
bool isOnBed(){
  int fsrADC1 = analogRead(FSR_PIN1);
  int fsrADC2 = analogRead(FSR_PIN2);
  int fsrADC3 = analogRead(FSR_PIN3);
  int fsrADC4 = analogRead(FSR_PIN4);
  int fsrADC5 = analogRead(FSR_PIN5);
  
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
