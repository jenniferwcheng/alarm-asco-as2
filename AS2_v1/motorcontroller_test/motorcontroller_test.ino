#define enA 10
#define in1 9
#define in2 8

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Set initial rotation direction
  analogWrite(enA, 255); // Send PWM signal to L298N Enable pin
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}
void loop() {
  
}
