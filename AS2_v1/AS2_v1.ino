#include <DS3231.h>
#include <LiquidCrystal.h>
//#include <DateTime.h>
//#include <DateTimeStrings.h>

// Pressor sensor analog pins
const int FSR_PIN1 = A0; // Pin connected to FSR/resistor divider
const int FSR_PIN2 = A2;
const int FSR_PIN3 = A1;

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL); // SDA: 20, SCL: 21

// Initialize LCD pins
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

// Clock pins
const int hourPin = A15; // Potentiometer
const int minPin = A14; // Potentiometer
const int CONFIRM = 52; // Button

// Booleans for clock
bool timeSet = false;
bool hourSet = false;
bool minuteSet = false;
bool alarmSet = false;

// Constants
int hourTemp = 0;
int hourVal = 0;
int minTemp = 0;
int minVal = 0;
int hourAlarm = 0;
int minAlarm = 0;

void setup()
{  
  // Initialize inputs
  pinMode(ALARM, INPUT);
  pinMode(hourPin, INPUT);
  pinMode(minPin, INPUT);
  
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //lcd.print("ASCO AS2");
  
  // Setup Serial connection
  //Serial.begin(9600);

  // Initialize the rtc object
  rtc.begin();

  // Ask user to set time
  lcd.print("Set current time");

  delay(3000);

  // Call function to begin setting the current time
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
  while(timeSet == false){ // Stay in loop until CONFIRM button is pressed
    if(digitalRead(CONFIRM) == HIGH){
      timeSet = true;
    }
    
    // Read values from potentiometers
    hourTemp = analogRead(hourPin);
    hourVal = map(hourTemp, 0, 1023, 0, 28) - 4;
    minTemp = analogRead(minPin);
    minVal = map(minTemp, 0, 1023, 0, 71)-11;

    // Display on lcd
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

  // Set time when out of loop
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
  delay(3000);
}
