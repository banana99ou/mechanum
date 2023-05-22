#define PI 3.1415926535897932384626433832795
#define IRFL A4
#define IRFR A5
#define IRBL A6
#define IRBR A7
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
#define MASTER_ARM 17

int IR_threshold_FL = 0; //need calibration //calibration function added
int IR_threshold_FR = 0; 
int IR_threshold_BL = 0; 
int IR_threshold_BR = 0; 
int Ping_bias = 0;
int sonardata;
int i = 0;

float MY_pos[2] = {0, 0};
float MY_heading = 90;
float EM_pos[2] = {0, 0};
float EM_heading = 270;
float turn = 0;
//int distance = 0;

long duration; // variable for the duration of sound wave travel
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(MASTER_ARM, INPUT);
  pinMode(TRIG_FL, OUTPUT);
  pinMode(ECHO_FL, INPUT);
  pinMode(TRIG_FF, OUTPUT);
  pinMode(ECHO_FF, INPUT);
  pinMode(TRIG_FR, OUTPUT);
  pinMode(ECHO_FR, INPUT);
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);
  pinMode(IRFL, INPUT);
  pinMode(IRFR, INPUT);
  pinMode(IRBL, INPUT);
  pinMode(IRBR, INPUT);

  for (i = 0; i < 20; i++) {
    if (IR_threshold_FL < analogRead(IRFL)) {
      IR_threshold_FL = analogRead(IRFL) + 10;
    }
    if (IR_threshold_FR < analogRead(IRFR)) {
      IR_threshold_FR = analogRead(IRFR) + 10;
    }
    if (IR_threshold_BL < analogRead(IRBL)) {
      IR_threshold_BL = analogRead(IRBL) + 10;
    }
    if (IR_threshold_BR < analogRead(IRBR)) {
      IR_threshold_BR = analogRead(IRBR) + 10;
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
}

void loop() {
  Serial.print(" Ping FF: ");
  Serial.print(Ping(ECHO_FF, TRIG_FF));
  Serial.print(" Ping L: ");
  Serial.print(Ping(ECHO_L, TRIG_L));
  Serial.print(" Ping R: ");
  Serial.print(Ping(ECHO_R, TRIG_R));
  Serial.print(" IR ");
  Serial.print(analogRead(IRFL));
  Serial.print(" ");
  Serial.print(analogRead(IRFR));
  Serial.print(" ");
  Serial.print(analogRead(IRBL));
  Serial.print(" ");
  Serial.println(analogRead(IRBR));
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