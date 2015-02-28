// Includes
#include <Servo.h>
// Definitions
#define MOTOR_PIN 11
// Module variables
Servo servo;

// Arduino Main Functions
void setup() {
  servo.attach(MOTOR_PIN);
}

void loop() {
  servo.write(180);
  delay(2000);
  servo.write(0);
  delay(2000);
}
