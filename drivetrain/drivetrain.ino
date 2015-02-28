#define A_DIR 2
#define A_ENABLE 3
#define B_DIR 8
#define B_ENABLE 9
// Characterized duty cycles
#define A_DUTY 70
#define B_DUTY 70
// States
#define FORWARD 0
#define BACK 1
#define LEFT 2
#define RIGHT 3
#define STOP 4

int state;

void setup() {
  state = FORWARD;
  Serial.begin(9600);
  pinMode(A_DIR, OUTPUT);
  pinMode(A_ENABLE, OUTPUT);
  pinMode(B_DIR, OUTPUT);
  pinMode(B_ENABLE, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    unsigned char useless = Serial.read();
    if (useless == 102) state = FORWARD;  // 'f'
    if (useless == 98) state = BACK;      // 'b'
    if (useless == 108) state = LEFT;     // 'l'
    if (useless == 114) state = RIGHT;    // 'r'
    if (useless == 115) state = STOP;     // 's'
  }
  if (state == FORWARD) {
    digitalWrite(A_DIR, HIGH);
    digitalWrite(B_DIR, HIGH);
    analogWrite(A_ENABLE, A_DUTY);
    analogWrite(B_ENABLE, B_DUTY);
  }
  else if (state == BACK) {
    digitalWrite(A_DIR, LOW);
    digitalWrite(B_DIR, LOW);
    analogWrite(A_ENABLE, A_DUTY);
    analogWrite(B_ENABLE, B_DUTY);
  }
  else if (state == LEFT) {
    digitalWrite(A_DIR, LOW);
    digitalWrite(B_DIR, HIGH);
    analogWrite(A_ENABLE, A_DUTY);
    analogWrite(B_ENABLE, B_DUTY);
  }
  else if (state == RIGHT) {
    digitalWrite(A_DIR, HIGH);
    digitalWrite(B_DIR, LOW);
    analogWrite(A_ENABLE, A_DUTY);
    analogWrite(B_ENABLE, B_DUTY);
  }
  else if (state == STOP) {
    digitalWrite(A_DIR, HIGH);
    digitalWrite(B_DIR, HIGH);
    analogWrite(A_ENABLE, 0);
    analogWrite(B_ENABLE, 0);
  }
}
