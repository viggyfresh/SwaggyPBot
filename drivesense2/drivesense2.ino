// Includes
#include <Servo.h>
// Motor pins
#define A_DIR 2
#define A_ENABLE 3
#define B_DIR 8
#define B_ENABLE 11
// Servo pin
#define S_PIN 9
// Characterized duty cycles
#define A_DUTY 80
#define B_DUTY 80
// Tape sensor pins
#define FLS 6
#define FRS 7
// Bumper pins
#define LB 4
#define RB 5
// Super states
#define INIT 0
#define RELOAD 1
#define UP 2
#define SHOOT 3
#define DOWN 4
// States
#define FORWARD 0
#define BACK 1
#define LEFT 2
#define RIGHT 3
#define STOP 4

// Prototypes
void turnAround();
void turnLeft();
void turnRight();
void moveForward();
void moveBackward();
void halt();
void reverseHalt();

int super_state;
int state;
Servo servo;

void setup() {
  Serial.begin(9600);
  super_state = INIT;
  state = STOP;
  pinMode(A_DIR, OUTPUT);
  pinMode(A_ENABLE, OUTPUT);
  pinMode(B_DIR, OUTPUT);
  pinMode(B_ENABLE, OUTPUT);
  pinMode(FLS, INPUT);
  pinMode(FRS, INPUT);
  pinMode(LB, INPUT);
  pinMode(RB, INPUT);
  servo.attach(S_PIN);
  servo.write(8);
}

void loop() {
  if (super_state == INIT) {
    Serial.println("FINDING WALL");
    state = FORWARD;
    int lb = digitalRead(LB);
    int rb = digitalRead(RB);
    while (true) {
      moveForward();
      if (lb == LOW || rb == LOW) {
        halt();
        state = STOP;
        break;
      }
      lb = digitalRead(LB);
      rb = digitalRead(RB);
    }
    Serial.println("WALL FOUND");
    Serial.println("REVERSING TO TAPE");
    int fl = digitalRead(FLS);
    int fr = digitalRead(FRS);
    while (fl == LOW) {
      moveBackward();
      fl = digitalRead(FLS);
      fr = digitalRead(FRS);
    }
    Serial.println("TAPE FOUND - REORIENTING");
    reverseHalt();
    delay(500);
    moveForward();
    delay(250);
    halt();
    fr = digitalRead(FRS);
    while (fr == LOW) {
      turnLeft();
      fr = digitalRead(FRS);
    }
    halt();
    Serial.println("REORIENTING COMPLETE");
    super_state = DOWN;
  }
  else if (super_state == RELOAD) {
    Serial.println("RELOADING");
    for (int i = 0; i < 2; i++) {
      int lb = digitalRead(LB);
      int rb = digitalRead(RB);
      moveBackward();
      delay(350);
      reverseHalt();
      while (true) {
        moveForward();
        if (lb == LOW || rb == LOW) {
          halt();
          break;
        }
        lb = digitalRead(LB);
        rb = digitalRead(RB);
      }
      delay(5000);
    }
    Serial.println("TURNING AROUND");
    turnAround();
    super_state = UP;
  }
  else if (super_state == UP) {
    Serial.println("FORWARD PASS");
    move();
  }
  else if (super_state == SHOOT) {
    Serial.println("SHOOTING");
    delay(1000);
    servo.write(90);
    delay(5000);
    for (int i = 90; i >= 8; i -= 10) {
      servo.write(i);
      delay(50);
    }
    delay(1000);
    turnAround();
    super_state = DOWN; 
  }
  else if (super_state == DOWN) {
    Serial.println("BACKWARD PASS");
    move();
  }
}

void move() {
  int fl = digitalRead(FLS);
  int fr = digitalRead(FRS);
  int lb = digitalRead(LB);
  int rb = digitalRead(RB);
  if (lb == LOW || rb == LOW) {
      halt();
      state = STOP;
      if (super_state == UP) super_state = SHOOT;
      else if (super_state == DOWN) {
        delay(5000);
        super_state = RELOAD;
      }
  }
  else if (fl == LOW && fr == LOW) {
    if (state != FORWARD) {
      Serial.println("FORWARD");
    }
    state = FORWARD;
    moveForward();
  }
  else if (fl == LOW && fr == HIGH) {
    if (state != RIGHT) {
      Serial.println("RIGHT");
    }
    halt();
    state = RIGHT;
    turnRight();
    delay(175);
    moveForward();
    delay(225);
  }
  else if (fl == HIGH && fr == LOW) {
    if (state != LEFT) {
      Serial.println("LEFT");
    }
    halt();
    state = LEFT;
    turnLeft();
    delay(175);
    moveForward();
    delay(225);
  }
  else if (fl == HIGH && fr == HIGH) {
    if (state != FORWARD) {
     Serial.println("FORWARD");
    }
    state = FORWARD;
    moveForward();
  }
}

void turnAround() {
  moveBackward();
  delay(200);
  reverseHalt();
  turnLeft();
  delay(150);
  int fl = digitalRead(FLS);
  while (fl == LOW) {
      turnLeft();
      fl = digitalRead(FLS);
  }
  halt();
  moveBackward();
  delay(150);
  reverseHalt();
}

void turnLeft() {
  digitalWrite(A_DIR, LOW);
  digitalWrite(B_DIR, HIGH);
  analogWrite(A_ENABLE, A_DUTY);
  analogWrite(B_ENABLE, B_DUTY);
}

void turnRight() {
  digitalWrite(A_DIR, HIGH);
  digitalWrite(B_DIR, LOW);
  analogWrite(A_ENABLE, A_DUTY);
  analogWrite(B_ENABLE, B_DUTY);
}

void moveForward() {
  digitalWrite(A_DIR, HIGH);
  digitalWrite(B_DIR, HIGH);
  analogWrite(A_ENABLE, A_DUTY);
  analogWrite(B_ENABLE, B_DUTY);
}

void moveBackward() {
  digitalWrite(A_DIR, LOW);
  digitalWrite(B_DIR, LOW);
  analogWrite(A_ENABLE, A_DUTY);
  analogWrite(B_ENABLE, B_DUTY);
}

void halt() {
  digitalWrite(A_DIR, LOW);
  digitalWrite(B_DIR, LOW);
  delay(150);
  analogWrite(A_ENABLE, 0);
  analogWrite(B_ENABLE, 0);
}

void reverseHalt() {
  digitalWrite(A_DIR, HIGH);
  digitalWrite(B_DIR, HIGH);
  delay(350);
  analogWrite(A_ENABLE, 0);
  analogWrite(B_ENABLE, 0);
}
