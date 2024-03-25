#define PI 3.1415926535897932384626433832795
#define IR_FL A4
#define IR_FR A5
#define IR_BL A6
#define IR_BR A7
#define TRIG_FL 26
#define ECHO_FL 27
#define TRIG_FF 8
#define ECHO_FF 9
#define TRIG_FR 10
#define ECHO_FR 11
#define TRIG_L 28
#define ECHO_L 29
#define TRIG_R 22
#define ECHO_R 23
#define MASTER_ARM 17
#define ENCA_ch1 3
#define ENCB_ch1 2
#define ENCA_ch2 1
#define ENCB_ch2 0

bool rematch = false;
int rematch_condition = 600; //mm
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
int distance = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MASTER_ARM, INPUT);
  pinMode(TRIG_FL, INPUT);
  pinMode(ECHO_FL, INPUT);
  pinMode(TRIG_FF, INPUT);
  pinMode(ECHO_FF, INPUT);
  pinMode(TRIG_FR, INPUT);
  pinMode(ECHO_FR, INPUT);
  pinMode(TRIG_L, INPUT);
  pinMode(ECHO_L, INPUT);
  pinMode(TRIG_R, INPUT);
  pinMode(ECHO_R, INPUT);
  pinMode(IR_FL, INPUT);
  pinMode(IR_FR, INPUT);
  pinMode(IR_BL, INPUT);
  pinMode(IR_BR, INPUT);

  for(i=0; i<20; i++){
    if(IR_threshold_FL < analogRead(IR_FL)){
        IR_threshold_FL = analogRead(IR_FL) + 100;
    }
    if(IR_threshold_FR < analogRead(IR_FR)){
        IR_threshold_FR = analogRead(IR_FR) + 100;
    }
    if(IR_threshold_BL < analogRead(IR_BL)){
        IR_threshold_BL = analogRead(IR_BL) + 100;
    }
    if(IR_threshold_BR < analogRead(IR_BR)){
        IR_threshold_BR = analogRead(IR_BR) + 100;
    }
    Serial.println("IR Calibrating ");
    Serial.print("IR_threshold_FL ");
    Serial.print(IR_threshold_FL);
    Serial.print(" IR_threshold_FR ");
    Serial.print(IR_threshold_FR);
    Serial.print(" IR_threshold_BL ");
    Serial.print(IR_threshold_BL);
    Serial.print(" IR_threshold_BR ");
    Serial.println(IR_threshold_BR);
  }

  while(true) {
      Serial.println("waiting for switch ");
      if (digitalRead(MASTER_ARM) == 1){ //wait until MASTER_ARM is armed
        Serial.print("armed sonardata: ");
        sonardata = Ping(ECHO_FF, TRIG_FF);
        Serial.print(sonardata);
        delay(5000);
        break;
      }
  }
  if (sonardata < rematch_condition) {
      Serial.println(" rematch");
      rematch = true;
      MY_pos[0] = 0;  //mm 
      MY_pos[1] = 250;
      EM_pos[0] = 0;  //mm
      EM_pos[1] = -250;
  }
  else{
      Serial.println(" Not rematch");
      rematch = false;
      MY_pos[0] = 950;  //mm 
      MY_pos[1] = 0;
      EM_pos[0] = -950;  //mm
      EM_pos[1] = 0;
  }
}

void loop() {
    int result1 = Ping(ECHO_FL, TRIG_FL);
    int result2 = Ping(ECHO_FF, TRIG_FF);
    int result3 = Ping(ECHO_FR, TRIG_FR);
    int result4 = Ping(ECHO_L, TRIG_L);
    int result5 = Ping(ECHO_R, TRIG_R);
    if (result1 > Ping_bias) {
        EM_pos[0] = MY_pos[0] + cos((MY_heading - 0) * 180/3.14) * result1;
        EM_pos[1] = MY_pos[1] + sin((MY_heading - 0) * 180/3.14) * result1;
    }
    else if (result2 > Ping_bias) {
        EM_pos[0] = MY_pos[0] + cos((MY_heading + 90) * 180/3.14) * result2;
        EM_pos[1] = MY_pos[1] + sin((MY_heading + 90) * 180/3.14) * result2;
    }
    else if (result3 > Ping_bias) {
        EM_pos[0] = MY_pos[0] + cos((MY_heading - 90) * 180/3.14) * result3;
        EM_pos[1] = MY_pos[1] + sin((MY_heading - 90) * 180/3.14) * result3;
    }
    else if (result4 > Ping_bias) {
        EM_pos[0] = MY_pos[0] + cos((MY_heading - 180) * 180/3.14) * result4;
        EM_pos[1] = MY_pos[1] + sin((MY_heading - 180) * 180/3.14) * result4;
    }

    int Edge_detection[2][2];
    if(analogRead(IR_FL) > IR_threshold_FL){   //[BL][BR]
        Edge_detection[0][0] = 1;
    }
    else{
        Edge_detection[0][0] = 0;
    }
    if(analogRead(IR_FR) > IR_threshold_FR){
        Edge_detection[0][1] = 1;
    }
    else{
        Edge_detection[0][1] = 0;
    }
    if(analogRead(IR_BL) > IR_threshold_BL){
        Edge_detection[1][0] = 1;
    }
    else{
        Edge_detection[1][0] = 0;
    }
    if(analogRead(IR_BR) > IR_threshold_BR){
        Edge_detection[1][1] = 1;
    }
    else{
        Edge_detection[1][1] = 0;
    }
    if(Edge_detection[0][0] == 1 && Edge_detection[0][1] != 1 && Edge_detection[1][0] != 1) {
        //if only FL not detected go back a little and then turn away from edge
        distance = -100;
        turn = -135;
    }
    if(Edge_detection[0][0] == 1 && Edge_detection[0][1] == 1) {
        //if FR & FL not detected go back a little and then turn away from edge
        distance = -100;
        turn = -180;
    }
    if(Edge_detection[0][0] != 1 && Edge_detection[0][1] == 1 && Edge_detection[1][1] != 1) {
        //if only FR not detected go back a little and then turn away from edge
        distance = -100;
        turn = 135;
    }
    if(Edge_detection[1][0] == 1 && Edge_detection[1][1] != 1 && Edge_detection[0][0] != 1) {
        //if only BL not detected go forward a little and then turn away from edge
        distance = 100;
        turn = -45;
    }
    if(Edge_detection[1][0] == 1 && Edge_detection[1][1] == 1) {
        //if BR & BL not detected go forward a little and then turn away from edge
        distance = 100;
        turn = 0;
    }
    if(Edge_detection[1][0] != 1 && Edge_detection[1][1] == 1 && Edge_detection[0][1] != 1) {
        //if only BR not detected go forward a little and then turn away from edge
        distance = 100;
        turn = 45;
    }

    if(Edge_detection[0][0] == 1 && Edge_detection[1][0] == 1) {
        //if BL & FL not detected go forward a little and then turn away from edge
        distance = 0;
        turn = -90;
    }
    if(Edge_detection[0][1] == 1 && Edge_detection[1][1] == 1) {
        //if FR & BR not detected go forward a little and then turn away from edge
        distance = 0;
        turn = 90;
    }
    Serial.print(" distance ");
    Serial.print(distance);
    Serial.print(" turn ");
    Serial.println(turn);
}

int Ping(int echo, int trig){
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
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    return(distance); //cm
}

int IR_Processing() {                       //[FL][FR]
    
    int result_IR[2];
    result_IR[0] = distance;
    result_IR[1] = turn;
    return result_IR;
    //reset my pos to nearst edge
    //flag to run away from edge
}