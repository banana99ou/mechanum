#define dir1 4
#define pwm1 5
#define ENCB_ch1 14
#define ENCA_ch1 2
 
// pid stuff for motor
int pos_ch1 = 0;  // left motor position
int target_ch1 = 0; // left motor position target
long prevT = 0;       // previous time
float preve_ch1 = 0;  // previous error
float eintegral_ch1 = 0; //error integral
int ppr = 459;  // pulse per revolution

void setup(){
    pinMode(ENCA_ch1, INPUT);
    pinMode(ENCB_ch1, INPUT);
    pinMode(pwm1, OUTPUT);
    pinMode(dir1, OUTPUT);
    pinMode(pwm2, OUTPUT);
    pinMode(dir2, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ENCA_ch1), readEncoderch1, RISING);
}

void loop() {
  // time flow
  long currT = micros();
  float deltaT = ((float)(currT - prevT)) / 1.0e6;
  prevT = currT;

  preve_ch1 =  motor_pid(target_ch1, pos_ch1, deltaT, dir1, pwm1, preve_ch1);
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