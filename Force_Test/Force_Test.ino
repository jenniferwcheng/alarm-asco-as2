/******************************************************************************
Force_Sensitive_Resistor_Example.ino
Example sketch for SparkFun's force sensitive resistors
  (https://www.sparkfun.com/products/9375)
Jim Lindblom @ SparkFun Electronics
April 28, 2016

Create a voltage divider circuit combining an FSR with a 3.3k resistor.
- The resistor should connect from A0 to GND.
- The FSR should connect from A0 to 3.3V
As the resistance of the FSR decreases (meaning an increase in pressure), the
voltage at A0 should increase.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int FSR_PIN1 = A0; // Pin connected to FSR/resistor divider
const int FSR_PIN2 = A1;
const int FSR_PIN3 = A2;
const int FSR_PIN4 = A3;
const int FSR_PIN5 = A4;

// Measure the voltage at 5V and resistance of your 3.3k resistor, and enter
// their value's below:
const float VCC = 5; // Measured voltage of Ardunio 5V line
const float R_DIV = 10000.0; // Measured resistance of 3.3k resistor

void setup() 
{
  Serial.begin(9600);
  pinMode(FSR_PIN1, INPUT);
  pinMode(FSR_PIN2, INPUT);
  pinMode(FSR_PIN3, INPUT);
  pinMode(FSR_PIN4, INPUT);
  pinMode(FSR_PIN5, INPUT);
}

void loop() 
{
  int fsrADC1 = analogRead(FSR_PIN1);
  int fsrADC2 = analogRead(FSR_PIN2);
  int fsrADC3 = analogRead(FSR_PIN3);
  int fsrADC4 = analogRead(FSR_PIN4);
  int fsrADC5 = analogRead(FSR_PIN5);
  
  // If the FSR has no pressure, the resistance will be
  // near infinite. So the voltage should be near 0.
  if (fsrADC1 != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC1 * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    //Serial.print("Resistance 1: " + String(fsrR) + " ohms | ");
    Serial.print(String(fsrR) + " ");

    delay(50);
  }
  else
  {
    Serial.print("100000 ");// No pressure detected
  }
  
  if (fsrADC2 != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC2 * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    //Serial.print("Resistance 2: " + String(fsrR) + " ohms | ");
    Serial.print(String(fsrR) + " ");
    delay(50);
  }
  else
  {
    Serial.print("100000 ");// No pressure detected
  }

  if (fsrADC3 != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC3 * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    //Serial.println("Resistance 3: " + String(fsrR) + " ohms");
    Serial.print(String(fsrR) + " ");
    delay(50);
  }
  else
  {
    Serial.print("100000 ");// No pressure detected
  }
  
  if (fsrADC4 != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC4 * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    //Serial.println("Resistance 3: " + String(fsrR) + " ohms");
    Serial.print(String(fsrR) + " ");
    delay(50);
  }
  else
  {
    Serial.print("100000 ");// No pressure detected
  }
  
  if (fsrADC5 != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC5 * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    //Serial.println("Resistance 3: " + String(fsrR) + " ohms");
    Serial.println(String(fsrR) + " ");
    delay(50);
  }
  else
  {
    Serial.println("100000 ");// No pressure detected
  }
}
