#define dir1 4
#define pwm1 5
#define dir2 6
#define pwm2 7
#define IR_FL A1
#define IR_FR A2
#define IR_BL A3
#define IR_BR A4

int i = 0;
int IR_threshold_FL = 0; //need calibration //calibration function added
int IR_threshold_FR = 0; 
int IR_threshold_BL = 0; 
int IR_threshold_BR = 0; 

void setup() {
  Serial.begin(9600);
  pinMode(IR_FL, INPUT);
  pinMode(IR_FR, INPUT);
  pinMode(IR_BL, INPUT);
  pinMode(IR_BR, INPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);

  // for(i=0; i<20; i++){
  //   if(IR_threshold_FL < analogRead(IR_FL)){
  //       IR_threshold_FL = analogRead(IR_FL) + 100;
  //   }
  //   if(IR_threshold_FR < analogRead(IR_FR)){
  //       IR_threshold_FR = analogRead(IR_FR) + 100;
  //   }
  //   if(IR_threshold_BL < analogRead(IR_BL)){
  //       IR_threshold_BL = analogRead(IR_BL) + 100;
  //   }
  //   if(IR_threshold_BR < analogRead(IR_BR)){
  //       IR_threshold_BR = analogRead(IR_BR) + 100;
  //   }
  //   Serial.print("IR Calibrating ");
  //   Serial.print(IR_threshold_FL);
  //   Serial.print(" ");
  //   Serial.print(IR_threshold_FR);
  //   Serial.print(" ");
  //   Serial.print(IR_threshold_BL);
  //   Serial.print(" ");
  //   Serial.println(IR_threshold_BR);
  // }
}

void loop() {
  Serial.write("wtf");
  analogWrite(pwm1, 200);
  analogWrite(pwm2, 200);
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  delay(3000);
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  delay(3000);
}