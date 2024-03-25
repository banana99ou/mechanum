#define IR_FL_PIN A0
#define IR_FR_PIN A1
#define IR_BL_PIN A2
#define IR_BR_PIN A3 //적외선센서4개
#define TRIG_PIN1 9
#define ECHO_PIN1 8    //초음파센서111111
#define MOTOR1_DIR_PIN 4   //모터
#define MOTOR2_DIR_PIN 6
#define MOTOR1_PWM_PIN 5
#define MOTOR2_PWM_PIN 7
#define THRESHOLD_DISTANCE 30 //초음파센서인지거리
#define IR_THRESHOLD 40       //적외선센서인지거리
#define FORWARD_SPEED 150     //전진속도 조절
#define BACKWARD_SPEED 150    //후진속도 조절

float Vcc = 5.0;   //적외선센서 거리측정을 위한 변수
float dig_v1;
float IRFL;
float dig_v2;
float IRFR;
float dig_v3;
float IRBL;
float dig_v4;
float IRBR;        //여기까지
int IR_result[4];

void setup() {

  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(MOTOR1_DIR_PIN, OUTPUT);
  pinMode(MOTOR2_DIR_PIN, OUTPUT);
  pinMode(MOTOR1_PWM_PIN, OUTPUT);
  pinMode(MOTOR2_PWM_PIN, OUTPUT);
  pinMode(IR_FL_PIN, INPUT);
  pinMode(IR_FR_PIN, INPUT);
  pinMode(IR_BL_PIN, INPUT);
  pinMode(IR_BR_PIN, INPUT);

  IR_result[0] = analogRead(IR_FL_PIN);
  IR_result[1] = analogRead(IR_FR_PIN);
  IR_result[2] = analogRead(IR_BL_PIN);
  IR_result[3] = analogRead(IR_BR_PIN);

  Serial.begin(9600);

  // 오른쪽으로 90도 회전 후 로봇을 150cm만큼 전진
  turnright();
  delay(3000);    //150cm움직일때까기 직진. 값수정필요
  stopMotors();

}

void loop() {
  long distance = getDistance();  // 거리 측정
  long FLdis = IR_dist1();
  long FRdis = IR_dist2();
  long BLdis = IR_dist3();
  long BRdis = IR_dist4();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm\n");

  Serial.println(FLdis);
  Serial.println(", ");
  Serial.println(FRdis);
  Serial.println(", ");
  Serial.println(BLdis);
  Serial.println(", ");
  Serial.println(BRdis);
  Serial.println(", ");

  if (distance < THRESHOLD_DISTANCE) {
    // 상대방이 탐지되면 해당 방향으로 0.5초간 전진하여 밀어냄
    moveForward();
    delay(500);  // 0.5초간 전진
    stopMotors();  // 전진 후 정지
  } else {
    // 상대방이 탐지되지 않은 경우 회전 및 탐지
    rotateAndDetect();
  }

  if ( FLdis > IR_THRESHOLD || FRdis > IR_THRESHOLD || BLdis > IR_THRESHOLD || BRdis > IR_THRESHOLD) {
    turn();
  }
}

long getDistance() {
  digitalWrite(TRIG_PIN1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN1, LOW);

  long duration = pulseIn(ECHO_PIN1, HIGH);
  return duration * 0.034 / 2;  // 음속을 이용한 거리 계산
}

void moveForward() {
  if( getDistance() < THRESHOLD_DISTANCE )
  {
    digitalWrite(MOTOR1_DIR_PIN, LOW); //전진
    digitalWrite(MOTOR2_DIR_PIN, LOW);
    analogWrite(MOTOR1_PWM_PIN, FORWARD_SPEED);
    analogWrite(MOTOR2_PWM_PIN, FORWARD_SPEED);
  }
  else if( getDistance() > THRESHOLD_DISTANCE) {
    rotateAndDetect();
  }
  else {
    digitalWrite(MOTOR1_DIR_PIN, LOW); //LOW?? HIGH??
    digitalWrite(MOTOR2_DIR_PIN, LOW);  //전진?
    analogWrite(MOTOR1_PWM_PIN, FORWARD_SPEED);
    analogWrite(MOTOR2_PWM_PIN, FORWARD_SPEED);
  }
}

void moveBackward() {

  digitalWrite(MOTOR1_DIR_PIN, HIGH);
  digitalWrite(MOTOR2_DIR_PIN, HIGH);
  analogWrite(MOTOR1_PWM_PIN, BACKWARD_SPEED);
  analogWrite(MOTOR2_PWM_PIN, BACKWARD_SPEED);
}

void turnright() {
  if ( getDistance() > THRESHOLD_DISTANCE) {
    digitalWrite(MOTOR1_DIR_PIN, LOW);
    digitalWrite(MOTOR2_DIR_PIN, HIGH);
    analogWrite(MOTOR1_PWM_PIN, FORWARD_SPEED);
    analogWrite(MOTOR2_PWM_PIN, FORWARD_SPEED);
    delay(3000);
  }
  else {
    moveForward();
  }
}

void rotateAndDetect() {
  // 회전 동작 수행
  // 예시: 왼쪽으로 회전
  digitalWrite(MOTOR1_DIR_PIN, HIGH);
  digitalWrite(MOTOR2_DIR_PIN, LOW);  //2번 4번 우측모터
  analogWrite(MOTOR1_PWM_PIN, FORWARD_SPEED);
  analogWrite(MOTOR2_PWM_PIN, FORWARD_SPEED);

  // 최대 3초간 탐지 시도
  unsigned long startTime = millis();
  while (millis() - startTime < 3000) {
    long distance = getDistance();  // 거리 측정

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < THRESHOLD_DISTANCE) {
      // 상대방이 탐지되면 0.5초간 전진하여 밀어냄
      moveForward();
      delay(500);  // 0.5초간 전진
      stopMotors();  // 전진 후 정지
      return;  // 함수 종료
    }
  }

}

void stopMotors() {
  digitalWrite(MOTOR1_PWM_PIN, LOW);
  digitalWrite(MOTOR2_PWM_PIN, LOW);
}

void turn() {

    if(IR_result[0] > IR_THRESHOLD){ // turn left when speed is smaller than 0
        moveBackward();
        delay(1000);  //값 수정 필요
        digitalWrite(MOTOR1_DIR_PIN, LOW);  // left side go backward
        digitalWrite(MOTOR2_DIR_PIN, HIGH); // right side go forward
        delay(3000);  //값 수정 필요
        rotateAndDetect();
    }
    else if(IR_result[1] > IR_THRESHOLD){
        moveBackward();
        delay(1000);
        digitalWrite(MOTOR1_DIR_PIN, HIGH);    
        digitalWrite(MOTOR2_DIR_PIN, LOW);
        delay(3000);
        rotateAndDetect();
    }
    else if(IR_result[2] > IR_THRESHOLD){
        moveForward();
        delay(1000);
        digitalWrite(MOTOR1_DIR_PIN, LOW);    
        digitalWrite(MOTOR2_DIR_PIN, HIGH);
        delay(3000);
        rotateAndDetect();
    }
    else if(IR_result[3] > IR_THRESHOLD){
        moveForward();
        delay(1000);
        digitalWrite(MOTOR1_DIR_PIN, HIGH);    
        digitalWrite(MOTOR2_DIR_PIN, LOW);
        delay(3000);
        rotateAndDetect();
    }
    else if(IR_result[0] > IR_THRESHOLD && IR_result[1] > IR_THRESHOLD){
        moveBackward();
        delay(1000);
        digitalWrite(MOTOR1_DIR_PIN, HIGH);    
        digitalWrite(MOTOR2_DIR_PIN, LOW);
        delay(6000);
        rotateAndDetect();
    }
      else if(IR_result[2] > IR_THRESHOLD && IR_result[3] > IR_THRESHOLD){
        moveForward();
        delay(1000);
        digitalWrite(MOTOR1_DIR_PIN, HIGH);    
        digitalWrite(MOTOR2_DIR_PIN, LOW);
        delay(6000);
        rotateAndDetect();
    }
    else{   // set motor speed to 0 when speed is 0
        
    }
    analogWrite(MOTOR1_PWM_PIN, abs(FORWARD_SPEED));
    analogWrite(MOTOR2_PWM_PIN, abs(FORWARD_SPEED));
}

long IR_dist1() {
  dig_v1 = Vcc * analogRead(A0)/1023;
  long IRFL = 3.6473 * (1/pow(dig_v1, 3)) - 9.6037 * (1/pow(dig_v1, 2)) + 36.39 * (1/dig_v1)- 4.1143;
  return IRFL;
}
long IR_dist2() {
  dig_v2 = Vcc * analogRead(A1)/1023;
  long IRFR = 3.6473 * (1/pow(dig_v2, 3)) - 9.6037 * (1/pow(dig_v2, 2)) + 36.39 * (1/dig_v2)- 4.1143;
  return IRFR;
}
long IR_dist3() {
  dig_v3 = Vcc * analogRead(A2)/1023;
  long IRBL = 3.6473 * (1/pow(dig_v3, 3)) - 9.6037 * (1/pow(dig_v3, 2)) + 36.39 * (1/dig_v3)- 4.1143;
  return IRBL;
}
long IR_dist4() {
  dig_v4 = Vcc * analogRead(A3)/1023;
  long IRBR = 3.6473 * (1/pow(dig_v4, 3)) - 9.6037 * (1/pow(dig_v4, 2)) + 36.39 * (1/dig_v4)- 4.1143;
  return IRBR;
}