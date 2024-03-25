#define MASTER_ARM 43
#define TRIG_FL 27
#define ECHO_FL 26
#define TRIG_FF 8
#define ECHO_FF 9
#define TRIG_FR 10
#define ECHO_FR 11
#define TRIG_L 28
#define ECHO_L 29
#define TRIG_R 14
#define ECHO_R 15

#define dir1 4
#define pwm1 5
#define dir2 6
#define pwm2 7

#define PI 3.1415926535897932384626433832795
#define IRFL A4
#define IRFR A5
#define IRBL A6
#define IRBR A7

// defines variables
float drivedirection;
float temp;
int IRThreshold = 0;
int Ping_bias = 30;
int i = 0;

int IR_threshold_FL = 0;  //need calibration //calibration function added
int IR_threshold_FR = 0;
int IR_threshold_BL = 0;
int IR_threshold_BR = 0;

int Ping_bias1 = 500;

void setup() {
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  pinMode(MASTER_ARM, INPUT);
  pinMode(TRIG_FR, OUTPUT);
  pinMode(ECHO_FR, INPUT);
  pinMode(TRIG_FF, OUTPUT);
  pinMode(ECHO_FF, INPUT);
  pinMode(TRIG_FL, OUTPUT);
  pinMode(ECHO_FL, INPUT);
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);

  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);

  pinMode(IRFL, INPUT);
  pinMode(IRFR, INPUT);
  pinMode(IRBL, INPUT);
  pinMode(IRBR, INPUT);

  for (i = 0; i < 30; i++) {
    if (IR_threshold_FL < analogRead(IRFL)) {
      IR_threshold_FL = analogRead(IRFL) + 5;
    }
    if (IR_threshold_FR < analogRead(IRFR)) {
      IR_threshold_FR = analogRead(IRFR) + 5;
    }
    if (IR_threshold_BL < analogRead(IRBL)) {
      IR_threshold_BL = analogRead(IRBL) + 5;
    }
    if (IR_threshold_BR < analogRead(IRBR)) {
      IR_threshold_BR = analogRead(IRBR) + 5;
    }
    Serial.print("IR Calibrating ");
    Serial.print(IR_threshold_FL);
    Serial.print(" ");
    Serial.print(IR_threshold_FR);
    Serial.print(" ");
    Serial.print(IR_threshold_BL);
    Serial.print(" ");
    Serial.println(IR_threshold_BR);
    delay(100);
  } 
  // delay(10000); // delay 10sec
}

void loop() {
  int Ping_FFresult = Ping(ECHO_FF, TRIG_FF);
  // int Ping_FRresult = Ping(ECHO_FR, TRIG_FR);
  // int Ping_FLresult = Ping(ECHO_FR, TRIG_FL);
  int Ping_Lresult = Ping(ECHO_L, TRIG_L);
  int Ping_Rresult = Ping(ECHO_R, TRIG_R);
  if ((Ping_Rresult > Ping_FFresult)) {
    if(Ping_FFresult > Ping_bias) {
      // Serial.println("ping forward");      
      go_forward(255);
    }
    else if(Ping_FFresult < Ping_bias) {
      // Serial.println("ping ramming");
      go_forward(-255);
      delay(100);
      go_forward(255);
    }
  }

  else if ((Ping_FFresult > Ping_Rresult)) {
    Serial.println("ping right");
    turn(-180);
  }
  else {
    Serial.println("ping left");
    turn(180);
  }

  // else if((Ping_FRresult - Ping_FLresult) > ){
  //   turn((Ping_FRresult - Ping_FLresult)*0.5);
  // }

  // Serial.print(" PING ");
  // Serial.print(Ping_FFresult);
  // Serial.print(" ");
  // Serial.println(Ping_Rresult);

  int Edge_detection[2][2];
  if(analogRead(IRFL) > IR_threshold_FL){   //[BL][BR]
      Edge_detection[0][0] = 1;
  }
  else{
      Edge_detection[0][0] = 0;
  }
  if(analogRead(IRFR) > IR_threshold_FR){
      Edge_detection[0][1] = 1;
  }
  else{
      Edge_detection[0][1] = 0;
  }
  if(analogRead(IRBL) > IR_threshold_BL){
      Edge_detection[1][0] = 1;
  }
  else{
      Edge_detection[1][0] = 0;
  }
  if(analogRead(IRBR) > IR_threshold_BR){
      Edge_detection[1][1] = 1;
  }
  else{
      Edge_detection[1][1] = 0;
  }
  if(Edge_detection[0][0] == 1 && Edge_detection[0][1] == 1) {
      //if FR & FL not detected go back a little and then turn away from edge
      Serial.println("IR goback FF");
      go_forward(-112);
      delay(200);
      turn(-100);
      delay(200);
      //delay(600);
  }
  else if(Edge_detection[0][0] == 1 && Edge_detection[0][1] != 1 && Edge_detection[1][0] != 1) {
      //if only FL not detected go back a little and then turn away from edge
      Serial.println("IR goback FL");
      go_forward(-112);
      delay(200);
      turn(100);
      delay(200);
      //delay(800);
  }
  else if(Edge_detection[0][0] != 1 && Edge_detection[0][1] == 1 && Edge_detection[1][1] != 1) {
      //if only FR not detected go back a little and then turn away from edge
      Serial.println("IR goback FR");
      go_forward(-112);
      delay(250);
      turn(-100);
      delay(250);
      //delay(800);
  }
  else if(Edge_detection[1][0] == 1 && Edge_detection[1][1] != 1 && Edge_detection[0][0] != 1) {
      //if only BL not detected go forward a little and then turn away from edge
      go_forward(200);
      //turn(50);
  }
  else if(Edge_detection[1][0] == 1 && Edge_detection[1][1] == 1) {
      //if BR & BL not detected go forward a little and then turn away from edge
      go_forward(200);
      //turn(0);
  }
  else if(Edge_detection[1][0] != 1 && Edge_detection[1][1] == 1 && Edge_detection[0][1] != 1) {
      //if only BR not detected go forward a little and then turn away from edge
      go_forward(200);
      //turn(50);
  }

  else if(Edge_detection[0][0] == 1 && Edge_detection[1][0] == 1) {
      //if BL & FL not detected go forward a little and then turn away from edge
      //go_forward(0);
      Serial.println("IR turnright");
      turn(-80);
  }
  else if(Edge_detection[0][1] == 1 && Edge_detection[1][1] == 1) {
      //if FR & BR not detected go forward a little and then turn away from edge
      //go_forward(0);
      Serial.println("IR turnleft");
      turn(80);
  }
  //delay(100); 

  Serial.print(" IR ");
  Serial.print(analogRead(IRFL));
  Serial.print(" ");
  Serial.print(IR_threshold_FL);  //need calibration //calibration function added
  Serial.print(" ");
  Serial.print(analogRead(IRFR));
  Serial.print(" ");
  Serial.print(IR_threshold_FR);
  Serial.print(" ");
  Serial.print(analogRead(IRBL));
  Serial.print(" ");
  Serial.print(IR_threshold_BL);
  Serial.print(" ");
  Serial.print(analogRead(IRBR));
  Serial.print(" ");
  Serial.println(IR_threshold_BR);
}

int Ping(int echo, int trig) {
  long duration;  //variable for the duration of sound wave travel
  int distance;
  digitalWrite(trig, LOW);
  delayMicroseconds(1);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  // Calculating the distance
  distance = duration * 0.34 / 2;  // Speed of sound wave divided by 2 (go and back)
  return(distance);//mm
}

void go_forward(int pwm){
  int dir = 1;
  if(pwm < 0){
    dir = -1;
    pwm *= -1;
  }
  setMotor(dir1, dir, pwm1, pwm);
  setMotor(dir2, dir, pwm2, pwm);
}
void turn(int pwm){
  int dir = 1;
  if(pwm < 0){
    dir = -1;
    pwm *= -1;
  }
  setMotor(dir1, dir, pwm1, pwm);
  setMotor(dir2, dir * -1, pwm2, pwm);
}

void setMotor(int dirPin, int dir, int pwmPin, int pwmVal) {
    if(dir == 1){
        digitalWrite(dirPin, HIGH);
    }
    else if(dir == -1){
        digitalWrite(dirPin, LOW);
    }
    else{
        pwmVal = 0;
    }
    analogWrite(pwmPin, pwmVal);
}