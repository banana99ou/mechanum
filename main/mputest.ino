#define ENCA 2  // YELLOW
#define ENCB 14  // GREEN
#define dir1 4
#define pwm1 5

volatile int pos = 0;
long prevT = 0;       //previous time
float preve = 0;      //previous error
float eintegral = 0;  //error integral

void setup() {
  Serial.begin(9600);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
}

void loop() {
  // Serial.print("ENCA ");
  // Serial.print(digitalRead(ENCA));
  // Serial.print(" ENCB ");
  // Serial.println(digitalRead(ENCB));
  //set target position
  int target = 250*sin(prevT/1e6);

  //PID constants
  float kp = 0.9;
  float kd = 0.1;
  float ki = 0.4;

  //time difference
  long currT = micros();

  float deltaT = ((float)(currT - prevT)) / 1.0e6;
  prevT = currT;

  //error
  int e = target - pos;

  //derivative
  float dedt = (e - preve) / (deltaT);

  //integral
  eintegral = eintegral + e * deltaT;

  //control signal
  float u = kp * e + kd * dedt + ki * eintegral;

  //motor power
  float pwr = fabs(u);
  if (pwr > 255) {
    pwr = 255;
  }

  //motor direction
  int dir = 1;
  if (u < 0) {
    dir = -1;
  }

  setMotor(dir1, dir, pwm1, pwr);

  //store prevous error
  preve = e;

  Serial.print(target);
  Serial.print(" ");
  Serial.print(pos);
  Serial.println();
}

void setMotor(int dirPin, int dir, int pwmPin, int pwmVal) {
  if (dir == 1) {
    digitalWrite(dirPin, HIGH);
  } else if (dir == -1) {
    digitalWrite(dirPin, LOW);
  } else {
    pwmVal = 0;
  }
  analogWrite(pwmPin, pwmVal);
}

void readEncoder() {
  int b = digitalRead(ENCB);
  if (b > 0) {
    pos++;
  } else {
    pos--;
  }
}

void motorpid(int target) {
  //PID constants
  float kp = 1;
  float kd = 0;
  float ki = 0;

  long currT = micros();
  float deltaT = ((float)(currT - prevT)) / 1.0e6;
  long prevT = currT;
  int e = target - pos;
  float dedt = (e - preve) / (deltaT);
  float eintegral = eintegral + e * deltaT;
  float u = kp * e + kd * dedt + ki * eintegral;
  float pwr = fabs(u);
  if (pwr > 255) {
    pwr = 255;
  }
  int dir = 1;
  if (u < 0) {
    dir = -1;
  }
  setMotor(dir1, dir, pwm1, pwr);
  int preve = e;
}