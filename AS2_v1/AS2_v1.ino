#include <DS3231.h>
#include <LiquidCrystal.h>

// Pressor sensor analog pins
const int FSR_PIN1 = A0; // Pin connected to FSR/resistor divider
const int FSR_PIN2 = A2;
const int FSR_PIN3 = A1;

// Pressure sensor constants
const float VCC = 5; // Measured voltage of Ardunio 5V line
const float R_DIV = 10000.0; // Resistance of 10k resistor
float fsrR;
float fsrV;

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
bool onBed = false;

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
  delay(3000);
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
    if(ElapsedTime < 300000){
      // Buzzer
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(1000);        
      noTone(buzzer);     // Stop sound...
      delay(1000); 
    }
    else if(ElapsedTime >= 300000){
      // other stuff
    }
  }
}

// Checks if user is on bed based on pressure sensor data
bool isOnBed(){
  onBed = false;

  // Read resistance data
  int fsrADC1 = analogRead(FSR_PIN1);

  // If the FSR has no pressure, the resistance will be
  // near infinite. So the voltage should be near 0.
  if (fsrADC1 != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    fsrV = fsrADC1 * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    fsrR = R_DIV * (VCC / fsrV - 1.0);
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    delay(50);
  }
  
  if(fsrR < 6000){
    onBed = true;
  }

  return onBed;
}
