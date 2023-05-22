/* 
base unit = mm
arduino should be always on waiting for MASTER_ARM switch to turned on (5V)
upon initialization it will check if theres something infront to determin rematch

reset my position after ir response
check if wawypoint is in the feild
*/
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
#define TRIG_L 29
#define ECHO_L 28
#define TRIG_R 23
#define ECHO_R 22
#define MASTER_ARM 21
#define ENCB_ch1 14
#define ENCA_ch1 2
#define ENCB_ch2 15
#define ENCA_ch2 3  //left??
#define dir1 4
#define dir2 5
#define pwm1 6
#define pwm2 7

float field_radius = 1500;
int rematch_condition = 600; //mm
bool rematch = false;
int IR_threshold_FL = 0; //need calibration //calibration function added
int IR_threshold_FR = 0; 
int IR_threshold_BL = 0; 
int IR_threshold_BR = 0; 
int Ping_bias = 0;
int sonardata;

float MY_pos[2] = {0, 0};
float MY_heading = 90;
float EM_pos[2] = {0, 0};
float EM_heading = 270;

//ch1 left
//ch2 right
int pos_ch1 = 0; //motor position
int pos_ch2 = 0;
int target_ch1 = 0;
int target_ch2 = 0;
long prevT = 0; //previous time
float preve_ch1 = 0; //previous error
float preve_ch2 = 0;
float eintegral_ch1 = 0; //error integral
float eintegral_ch2 = 0;

float turn = 0; //amount to turn in degree

int ppr = 459; //pulse per revolution of encoder

void setup(){
    Serial.begin(9600);
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
        Serial.print("IR Calibrating ");
        Serial.print(IR_threshold_FL);
        Serial.print(" ");
        Serial.print(IR_threshold_FR);
        Serial.print(" ");
        Serial.print(IR_threshold_BL);
        Serial.print(" ");
        Serial.println(IR_threshold_BR);
    }

    while(true) {
        Serial.print("waiting for switch");
        if (digitalRead(MASTER_ARM) == 1){ //wait until MASTER_ARM is armed
            sonardata = Ping(ECHO_F, TRIG_F);
            break;
        }
    }
    if (sonardata < rematch_condition) {
        //rematch
        rematch = true;
        MY_pos[0] = 0;  //mm 
        MY_pos[1] = 250;
        EM_pos[0] = 0;  //mm
        EM_pos[1] = -250;
    }
    else{
        //attack vector
        rematch = false;
        MY_pos[0] = 950;  //mm 
        MY_pos[1] = 0;
        EM_pos[0] = -950;  //mm
        EM_pos[1] = 0;
    }
}

void loop(){
    //time flow
    long currT = micros();
    float deltaT = ((float)(currT - prevT)) / 1.0e6;
    prevT = currT;

    //enemy detection
    int result1 = Ping(ECHO_FF, TRIG_FF);
    int result2 = Ping(ECHO_L, TRIG_L);
    int result3 = Ping(ECHO_R, TRIG_R);
    if (result1 > Ping_bias) {
        // if ((Ping(ECHO_FL, TRIG_FL) > Ping_bias) && (Ping(ECHO_FR, TRIG_FR) < Ping_bias)){
            
        // }
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
    else{
        //when not1hing found, turn xx then ping again
        turn = turn - 30;
    }   

    //turn to face the enemy
    turn = AttackVectorCal(EM_pos, MY_pos, MY_heading);

    //edge detection
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

    onward(distance);
    turnXdegree(turn);

    //after turining, check if enemy is still there
    result1 = Ping(ECHO_FF, TRIG_FF);
    if(result1 > Ping_bias){
        //then go forward portion of distance to enemy
        int dis = result/4;
        onward(dis);
    }   
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
    distance = duration * 0.34 / 2; // Speed of sound wave divided by 2 (go and back)
    return(distance); //mm
}


int IR_Processing() {                       //[FL][FR]
    int Edge_detection[2][2];
    if(analogRead(IR_FL) > IR_threshold){   //[BL][BR]
        Edge_detection[0][0] = 1;
    }
    else{
        Edge_detection[0][0] = 0;
    }
    if(analogRead(IR_FR) > IR_threshold){
        Edge_detection[0][1] = 1;
    }
    else{
        Edge_detection[0][1] = 0;
    }
    if(analogRead(IR_BL) > IR_threshold){
        Edge_detection[1][0] = 1;
    }
    else{
        Edge_detection[1][0] = 0;
    }
    if(analogRead(IR_BR) > IR_threshold){
        Edge_detection[1][1] = 1;
    }
    else{
        Edge_detection[1][1] = 0;
    }
    return Edge_detection;
    //reset my pos to nearst edge
    //flag to run away from edge
}


// int Ping_processing() {
//     //!!should remove old data!!//
//     int Ping_bias = 0;
//     int E_count = 0;
//     int result[2][2];
//     result[0][0] = Ping(ECHO_F, TRIG_F);
//     result[0][1] = Ping(ECHO_L, TRIG_L);
//     result[1][0] = Ping(ECHO_R, TRIG_R);
//     result[1][1] = Ping(ECHO_B, TRIG_B);
//     if (result[0][0] > Ping_bias){      //ignor noisy close quater data      
//         result[0][0] = 0;
//     }
//     if (result[0][1] < Ping_bias){
//         result[0][1] = 0;
//     }
//     if (result[1][0] < Ping_bias){
//         result[1][0] = 0;
//     }
//     if (result[1][1] < Ping_bias){
//         result[1][1] = 0;
//     }
//     //ping every direction
//     return result;
// }


int AttackVectorCal(float EM_pos[2], float MY_pos[2], float MY_heading) {
    float DirToE = atan((EM_pos[1] - MY_pos[1])/(EM_pos[0] - MY_pos[0])*PI/180);
    float speed = 1 / sqrt(pow((EM_pos[0] - MY_pos[0]), 2) + pow((EM_pos[1] - MY_pos[1]), 2)); //speed 1 over distance to Enemy
    float turn = DirToE - MY_heading;
    return turn;
}

void readEncoderch1() {
    int b = digitalRead(ENCB_ch1);
    if(b>0){
        pos_ch1++;
    }
    else{
        pos_ch1--;
    }
}

void readEncoderch2() {
    int b = digitalRead(ENCB_ch2);
    if(b>0){
        pos_ch2++;
    }
    else{
        pos_ch2--;
    }
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

int motor_pid(int target, int pos, float deltaT, int dir1, int pwm1, int preve) {
  float kp = 1;
  float kd = 0;
  float ki = 0;
  int e = target - pos;
  float dedt = (e - preve)/(deltaT);
  float eintegral = eintegral + e * deltaT;
  float u = kp*e + kd*dedt + ki*eintegral;
  float pwr = fabs(u);
  if(pwr > 255){
      pwr = 255;
  }
  int dir = 1;
  if(u < 0){
      dir = -1;
  }
  setMotor(dir1, dir, pwm1, pwr);
  int preve = e;
  return preve;
}
//make simplyfied code that only does movement
void turnXdegree(float turn) {
    target_ch1 = target_ch1 + turn * PI * sqrt(pow(175, 2) + pow(240 + alpha, 2)) / 360 *  1 / 65*PI * ppr;  //amount to turn motor 1 = amount to turn in degree * amount motor need to turn per degree * direction / circumference of wheel * pulse per revolution
    target_ch2 = target_ch2 + turn * PI * sqrt(pow(175, 2) + pow(240 + alpha, 2)) / 360 * -1 / 65*PI * ppr;
    preve_ch1 = motor_pid(target_ch1, pos_ch1, deltaT, dir1, pwm1, preve_ch1);
    preve_ch2 = motor_pid(target_ch2, pos_ch2, deltaT, dir2, pwm2, preve_ch2);
    //append turn value to heading
    MY_heading = MY_heading + turn;
}

void onward(float dis, int ppr) {
    target_ch1 = dis / 56 * PI * ppr;
    target_ch2 = target_ch1
    preve_ch1 = motor_pid(target_ch1, pos_ch1, deltaT, dir1, pwm1, preve_ch1);
    preve_ch2 = motor_pid(target_ch2, pos_ch2, deltaT, dir2, pwm2, preve_ch2);
    //append dis to MY_pos
    MY_pos[0] = MY_pos[0] + dis * cos((MY_heading) * 180/3.14)
    MY_pos[1] = MY_pos[1] + dis * sin((MY_heading) * 180/3.14)
}