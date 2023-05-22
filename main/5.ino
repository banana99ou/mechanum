#define PI 3.1415926535897932384626433832795
#define IR_FL A4
#define IR_FR A5
#define IR_BL A6
#define IR_BR A7
#define TRIG_FL 27
#define ECHO_FL 26
#define TRIG_FF 8
#define ECHO_FF 9
#define TRIG_FR 10
#define ECHO_FR 11
#define TRIG_L 28
#define ECHO_L 29
#define TRIG_R 23
#define ECHO_R 22
#define MASTER_ARM 43
#define ENCB_ch1 14
#define ENCA_ch1 2
#define ENCB_ch2 15
#define ENCA_ch2 3  //left??
#define dir1 4
#define pwm1 5
#define dir2 6
#define pwm2 7

int rematch_condition = 600;  //mm
bool rematch = false;

int pos_ch1 = 0;  //motor position
int pos_ch2 = 0;
int target_ch1 = 0;
int target_ch2 = 0;
long prevT = 0;       //previous time
float preve_ch1 = 0;  //previous error
float preve_ch2 = 0;
int ppr = 459;
int Ping_bias = 0;
int sonardata;
int i = 0;

//testing condition
float MY_pos[2] = { 250, 0 };
float MY_heading = 90;
float EM_pos[2] = { -250, 0 };
float EM_heading = 270;

int dis = 50;
float turn = 0;

int IR_threshold_FL = 0;  //need calibration //calibration function added
int IR_threshold_FR = 0;
int IR_threshold_BL = 0;
int IR_threshold_BR = 0;

int Ping_FFresultP = 0; //previous sensor value
int Ping_LresultP = 0;
int Ping_RresultP = 0;

void setup() {
  Serial.begin(9600);
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
  pinMode(MASTER_ARM, INPUT);
  pinMode(ENCA_ch1, INPUT);
  pinMode(ENCB_ch1, INPUT);
  pinMode(ENCA_ch2, INPUT);
  pinMode(ENCB_ch2, INPUT);
  pinMode(IR_FL, INPUT);
  pinMode(IR_FR, INPUT);
  pinMode(IR_BL, INPUT);
  pinMode(IR_BR, INPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA_ch1), readEncoderch1, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA_ch2), readEncoderch2, RISING);

  for (i = 0; i < 20; i++) {
    if (IR_threshold_FL < analogRead(IR_FL)) {
      IR_threshold_FL = analogRead(IR_FL) + 100;
    }
    if (IR_threshold_FR < analogRead(IR_FR)) {
      IR_threshold_FR = analogRead(IR_FR) + 100;
    }
    if (IR_threshold_BL < analogRead(IR_BL)) {
      IR_threshold_BL = analogRead(IR_BL) + 100;
    }
    if (IR_threshold_BR < analogRead(IR_BR)) {
      IR_threshold_BR = analogRead(IR_BR) + 100;
    }
    Serial.print("IR Calibrating ");
    Serial.print(IR_threshold_FL);
    Serial.print(" ");
    Serial.print(IR_threshold_FR);
    Serial.print(" ");
    Serial.print(IR_threshold_BL);
    Serial.print(" ");
    Serial.println(IR_threshold_BR);
  }

  // while (true) {
  //   Serial.println("waiting for switch");
  //   if (digitalRead(MASTER_ARM) == 1) {  //wait until MASTER_ARM is armed
  //     sonardata = Ping(ECHO_FF, TRIG_FF);
  //     break;
  //   }
  // }
  if (sonardata < rematch_condition) {
    //rematch
    rematch = true;
    MY_pos[0] = 0;  //mm
    MY_pos[1] = 250;
    EM_pos[0] = 0;  //mm
    EM_pos[1] = -250;
  } else {
    //attack vector
    rematch = false;
    MY_pos[0] = 950;  //mm
    MY_pos[1] = 0;
    EM_pos[0] = -950;  //mm
    EM_pos[1] = 0;
  }
}

void loop() {
  //time flow
  long currT = micros();
  float deltaT = ((float)(currT - prevT)) / 1.0e6;
  prevT = currT;
  float turn = 0;

  //enemy detection
  int Ping_FFresult = Ping(ECHO_FF, TRIG_FF);
  int Ping_Lresult = Ping(ECHO_L, TRIG_L);
  int Ping_Rresult = Ping(ECHO_R, TRIG_R);
  Serial.print((Ping_FFresult - Ping_FFresultP)/deltaT);
  Serial.print(" ");
  Serial.print(Ping_FFresult);
  Serial.print(" ");
  Serial.print(Ping_FFresultP);
  Serial.print(" ");
  Serial.print(deltaT);
  Serial.print(" ");
  if((abs(Ping_FFresult - Ping_FFresultP)/deltaT) < 0.5){
    if ((Ping_Lresult > Ping_FFresult) && (Ping_Rresult > Ping_FFresult)) {
    // if ((Ping(ECHO_FL, TRIG_FL) > Ping_bias) && (Ping(ECHO_FR, TRIG_FR) < Ping_bias)){

    // }
    turn = 0;
  }
  if(abs((Ping_Lresult - Ping_LresultP)/deltaT) < 0.5){
    } else if ((Ping_Rresult > Ping_Lresult) && (Ping_FFresult > Ping_Lresult)) {
    turn = 90;
  }
  if(abs((Ping_Rresult - Ping_RresultP)/deltaT) < 0.5){
    } else if ((Ping_Lresult > Ping_Rresult) && (Ping_FFresult > Ping_Rresult)) {
    turn = -90;
  }
  } else {
    //when not1hing found, turn xx then ping again
    turn = 30;
  }

  Serial.print(" ");
  Serial.println(turn);

  Ping_FFresultP = Ping_FFresult; //previous sensor value
  Ping_LresultP = Ping_Lresult;
  Ping_RresultP = Ping_Rresult;

  turnXdegree(turn, ppr, deltaT);

  Ping_FFresult = Ping(ECHO_FF, TRIG_FF);
  if((abs(Ping_FFresult - Ping_FFresultP)/deltaT) < 0.5){
    onward(Ping_FFresult, ppr, deltaT);
  }
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
  return (distance);               //mm
}

int AttackVectorCal(float EM_pos[2], float MY_pos[2], float MY_heading) {
  float DirToE = atan(((EM_pos[1] - MY_pos[1]) / (EM_pos[0] - MY_pos[0]))) * (180 / PI);
  if (DirToE < 0) {
    DirToE = DirToE;
  }
  float speed = 1 / sqrt(pow((EM_pos[0] - MY_pos[0]), 2) + pow((EM_pos[1] - MY_pos[1]), 2));  //speed 1 over distance to Enemy
  float turn = DirToE - MY_heading;
  // Serial.print(turn);
  // Serial.print(" = ");
  Serial.print(0);
  Serial.print(" ");
  Serial.print(DirToE);
  Serial.print(" ");
  Serial.println(100);
  // Serial.print(" - ");
  // Serial.print(MY_heading);
  return turn;
}

void readEncoderch1() {
  //Serial.print("encoderch1 read");
  int b = digitalRead(ENCB_ch1);
  if (b > 0) {
    pos_ch1++;
  } else {
    pos_ch1--;
  }
  //Serial.println(pos_ch1);
}

void readEncoderch2() {
  //Serial.print("encoderch1 read");
  int b = digitalRead(ENCB_ch2);
  if (b > 0) {
    pos_ch2--;
  } else {
    pos_ch2++;
  }
  //Serial.println(pos_ch2);
}

void setMotor(int dirPin, int dir, int pwmPin, int pwmVal) {
  // Serial.print("motor pid");
  // Serial.print(" target ");
  // Serial.print(target);
  // Serial.print(" pos ");
  // Serial.print(pos);
  // Serial.print(" deltaT");
  // Serial.print(deltaT);
  // Serial.print(" dirPIN ");
  // Serial.print(dirPIN);
  // Serial.print(" pwmPIN ");
  // Serial.print(pwmPIN);
  if (dir == 1) {
    digitalWrite(dirPin, HIGH);
  } else if (dir == -1) {
    digitalWrite(dirPin, LOW);
  } else {
    pwmVal = 0;
  }
  analogWrite(pwmPin, pwmVal);
}

int motor_pid(int target, int pos, float deltaT, int dirPIN, int pwmPIN, int preve) {
  // Serial.print("motor pid");
  // Serial.print(" target ");
  // Serial.print(target);
  // Serial.print(" pos ");
  // Serial.print(pos);
  // Serial.print(" deltaT");
  // Serial.print(deltaT);
  // Serial.print(" dirPIN ");
  // Serial.print(dirPIN);
  // Serial.print(" pwmPIN ");
  // Serial.print(pwmPIN);
  float kp = 1;
  float kd = 0;
  float ki = 0;
  int e = target - pos;
  float dedt = (e - preve) / (deltaT);
  float eintegral = eintegral + e * deltaT;
  float u = kp * e + kd * dedt + ki * eintegral;
  // Serial.print(" u = kp*");
  // Serial.print(" + ");
  // Serial.print(dedt);
  float pwr = fabs(u);
  if (pwr > 255) {
    pwr = 255;
  }
  int dir = 1;
  if (u < 0) {
    dir = -1;
  }
  setMotor(dirPIN, dir, pwmPIN, pwr);
  preve = e;
  return preve;
}

void turnXdegree(float turn, int ppr, float deltaT) {
  target_ch1 = turn * PI * sqrt(pow(175, 2) + pow(300, 2)) / 360 * 1 / 65 * PI * ppr / 8;  //amount to turn motor 1 = amount to turn in degree * amount motor need to turn per degree * direction / circumference of wheel * pulse per revolution
  target_ch2 = -1 * target_ch1;
  preve_ch1 = motor_pid(target_ch1, pos_ch1, deltaT, dir1, pwm1, preve_ch1);
  preve_ch2 = motor_pid(target_ch2, pos_ch2, deltaT, dir2, pwm2, preve_ch2);
  //append turn value to heading
  MY_heading = MY_heading + turn;
}

void onward(float dis, int ppr, float deltaT) {
  // Serial.print(" onward ");
  // Serial.println(dis);
  target_ch1 = dis / 56 * PI * ppr;
  target_ch2 = target_ch1;
  preve_ch1 = motor_pid(target_ch1, pos_ch1, deltaT, dir1, pwm1, preve_ch1);
  preve_ch2 = motor_pid(target_ch2, pos_ch2, deltaT, dir2, pwm2, preve_ch2);
  //append dis to MY_pos
  MY_pos[0] = MY_pos[0] + dis * cos((MY_heading)*180 / 3.14);
  MY_pos[1] = MY_pos[1] + dis * sin((MY_heading)*180 / 3.14);
}