// Motor pins
#define A_DIR 2
#define A_ENABLE 3
#define B_DIR 8
#define B_ENABLE 9
// Characterized duty cycles
#define A_DUTY 50
#define B_DUTY 50
// Sensor pins
#define FLS 4
#define FRS 5
#define FLL 6
#define FRR 7
// States
#define FORWARD 0
#define BACK 1
#define LEFT 2
#define RIGHT 3
#define STOP 4

// Prototypes
void turnLeft();
void turnRight();
void moveForward();
void moveBackward();
void halt();

int state;
int prev_state;

void setup() {
  prev_state = STOP;
  state = FORWARD;
  Serial.begin(9600);
  pinMode(A_DIR, OUTPUT);
  pinMode(A_ENABLE, OUTPUT);
  pinMode(B_DIR, OUTPUT);
  pinMode(B_ENABLE, OUTPUT);
  pinMode(FLS, INPUT);
  pinMode(FRS, INPUT);
}

void loop() {
  int fl = digitalRead(FLS);
  int fr = digitalRead(FRS);
  if (fl == LOW && fr == LOW) {
    if (prev_state == RIGHT) state = LEFT;
    if (prev_state == LEFT) state = RIGHT;
    if (prev_state == FORWARD) state = FORWARD;
    if (prev_state == STOP) state = FORWARD;
  }
  else if (fl == LOW && fr == HIGH) {
    if (prev_state != state) {
      halt();
      prev_state = state;
    }
    state = RIGHT;
  }
  else if (fr == LOW && fl == HIGH) {
    if (prev_state != state) {
      halt();
      prev_state = state;
    }
    state = LEFT;
  }
  else if (fl == HIGH && fr == HIGH) {
    if (prev_state != state) prev_state = state;
    state = FORWARD;
  }
  if (state == FORWARD) {
    moveForward();
  }
  else if (state == BACK) {
    moveBackward();
  }
  else if (state == LEFT) {
    turnLeft();
  }
  else if (state == RIGHT) {
    turnRight();
  }
  else if (state == STOP) {
    halt();
  }
}

void turnLeft() {
  digitalWrite(A_DIR, HIGH);
  digitalWrite(B_DIR, HIGH);
  analogWrite(A_ENABLE, 200);
  analogWrite(B_ENABLE, 200);
  delay(10);
  analogWrite(A_ENABLE, A_DUTY - 30);
  analogWrite(B_ENABLE, B_DUTY - 10);
}

void turnRight() {
  digitalWrite(A_DIR, HIGH);
  digitalWrite(B_DIR, HIGH);
  analogWrite(A_ENABLE, 200);
  analogWrite(B_ENABLE, 200);
  delay(10);
  analogWrite(A_ENABLE, A_DUTY - 10);
  analogWrite(B_ENABLE, B_DUTY - 30);  
}

void moveForward() {
  digitalWrite(A_DIR, HIGH);
  digitalWrite(B_DIR, HIGH);
  analogWrite(A_ENABLE, 200);
  analogWrite(B_ENABLE, 200);
  delay(10);
  analogWrite(A_ENABLE, A_DUTY);
  analogWrite(B_ENABLE, B_DUTY);
}

void moveBackward() {
  digitalWrite(A_DIR, LOW);
  digitalWrite(B_DIR, LOW);
  analogWrite(A_ENABLE, 200);
  analogWrite(B_ENABLE, 200);
  delay(10);
  analogWrite(A_ENABLE, A_DUTY);
  analogWrite(B_ENABLE, B_DUTY);
}

void halt() {
  digitalWrite(A_DIR, LOW);
  digitalWrite(B_DIR, LOW);
  delay(150);
}
