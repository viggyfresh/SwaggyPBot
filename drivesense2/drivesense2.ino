// Motor pins
#define A_DIR 2
#define A_ENABLE 3
#define B_DIR 8
#define B_ENABLE 9
// Characterized duty cycles
#define A_DUTY 80
#define B_DUTY 80
// Sensor pins
#define FLS 6
#define FRS 7
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
  Serial.begin(9600);
  prev_state = STOP;
  state = FORWARD;
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
    if (state != FORWARD) {
      prev_state = state;
      Serial.println("FORWARD");
    }
    state = FORWARD;
    moveForward();
  }
  else if (fl == LOW && fr == HIGH) {
    if (state != RIGHT) {
      prev_state = state;
      Serial.println("RIGHT");
    }
    halt();
    state = RIGHT;
    turnRight();
    delay(175);
    moveForward();
    delay(250);
  }
  else if (fl == HIGH && fr == LOW) {
    if (state != LEFT) {
      prev_state = state;
      Serial.println("LEFT");
    }
    halt();
    state = LEFT;
    turnLeft();
    delay(175);
    moveForward();
    delay(250);
  }
  else if (fl == HIGH && fr == HIGH) {
    if (state != FORWARD) {
      prev_state = state;
      Serial.println("FORWARD");
    }
    state = FORWARD;
    moveForward();
  }
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
