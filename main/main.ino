/*
모터 드라이브 : MDD10A
모터:
적외선 센서:  GP2Y0D805Z0F
아두이노: 아두이노 우노(아마)
*/

//sensor pins
#define infrared_L A7 // 왼쪽 적외선 센서
#define infrared_R A6 // 오른쪽 적외선 센서

int trig_ff = 6; // 정면 초음파 트리거
int echo_ff = 5; // 정면 초음파 에코
int trig_fr = 4; // 우측 초음파 트리거
int echo_fr = 3;// 우측 초음파 에코
int trig_fl = 2; // 좌측 초음파 트리거
int echo_fl = 1;// 좌측 초음파 에코
//motor pins(dir = 방향, pwm = 속도(0~255))
int L_dir = 10;
int L_pwm = 9;
int R_dir = 8;
int R_pwm = 7;


//목표물과의 최대 거리, 400은 임시로 넣은 것
int enemy_dis = 400;
int vh = 500;
//경기장의 출발선을 고려해서 회전은 오른쪽으로 해야함. 
//우측 회전을 기본으로 하되 정면센서가 감지가 안되고 좌측센서가 감지가 되었다면 정면이 감지될 때까지 좌회전
//회전 => 목표물 발견 => 모터 작동 
void setup() {
  pinMode(echo_ff, INPUT);
  pinMode(trig_ff, OUTPUT);
  pinMode(echo_fr, INPUT);
  pinMode(trig_fr, OUTPUT);
  pinMode(echo_fl, INPUT);
  pinMode(trig_fl, OUTPUT);
  pinMode(infrared_L, INPUT);
  pinMode(infrared_R, INPUT);

  pinMode(L_dir, OUTPUT);
  pinMode(R_dir, OUTPUT);
  pinMode(L_pwm, OUTPUT);
  pinMode(R_pwm, OUTPUT);

  analogWrite(L_pwm, 0);
  analogWrite(R_pwm, 0);
}

//거리 측정 함수, 단위 cm
float dis_m(int trig, int echo) {
  float len, dis;

  digitalWrite(trig, LOW);
  delay(2);
  digitalWrite(trig, HIGH);
  delay(2);
  digitalWrite(trig, LOW);
  
  len = pulseIn(echo, HIGH);
  dis = ((float)(340 * len) / 10000) / 2;

  return dis;
}

// 도는 함수
void rotate(){
  digitalWrite(L_dir, HIGH);
  digitalWrite(R_dir, LOW);
  analogWrite(L_pwm, 255);
  analogWrite(R_pwm, 255);
}

void rotate_left(){
  digitalWrite(L_dir, LOW);
  digitalWrite(R_dir, HIGH);
  analogWrite(L_pwm, 255);
  analogWrite(R_pwm, 255);
}

//앞으로 가는 함수
void move_forward(){
  digitalWrite(L_dir, HIGH);
  digitalWrite(R_dir, HIGH);

  analogWrite(L_pwm, 255);
  analogWrite(R_pwm, 255);
}

void loop() {
  //정면 감지를 최우선으로 함.
  if(dis_m(trig_ff, echo_ff) < enemy_dis){
    move_forward();
  }
  else{
    //우측센서가 감지했을 때를 두번째 우선순위로 둔다.(회전 방향을 바꿔야 하기 때문);
    if (dis_m(trig_fr, echo_fr) > dis_m(trig_fl, echo_fr)){
      //좌측센서나 정면센서가 감지될 때까지 반복한다.
      while(dis_m(trig_fl, echo_fr) < enemy_dis || dis_m(trig_ff, echo_ff) < enemy_dis){
        rotate_left();
      }
    }
    else rotate();
  }
  delay(100);
}

/*해당 코드는 현재 알고리즘에서 사용하지 않음.
void move_backward(){
  digitalWrite(LMF_dir, LOW);
  digitalWrite(RMF_dir, LOW);
  digitalWrite(LMB_dir, LOW);
  digitalWrite(RMB_dir, LOW);

  analogWrite(LMF_pwm, 255);
  analogWrite(RMF_pwm, 255);
  analogWrite(LMB_pwm, 255);
  analogWrite(RMB_pwm, 255);
}

void escape_right(){
  digitalWrite(LMF_dir, HIGH);
  digitalWrite(RMF_dir, HIGH);
  digitalWrite(LMB_dir, HIGH);
  digitalWrite(RMB_dir, HIGH);

  analogWrite(LMF_pwm, 255);
  analogWrite(RMF_pwm, 120);
  analogWrite(LMB_pwm, 255);
  analogWrite(RMB_pwm, 120);

  delay(1000);
}

void escape_left(){
  digitalWrite(LMF_dir, HIGH);
  digitalWrite(RMF_dir, HIGH);
  digitalWrite(LMB_dir, HIGH);
  digitalWrite(RMB_dir, HIGH);

  analogWrite(LMF_pwm, 120);
  analogWrite(RMF_pwm, 255);
  analogWrite(LMB_pwm, 120);
  analogWrite(RMB_pwm, 255);

  delay(1000);
}
*/

#define TRIG_FF 7
#define ECHO_FF 6
#define TRIG_FR 
#define ECHO_FR 
#define TRIG_FL 
#define ECHO_FL 
#define IR_L A7 
#define IR_R A6

#define motor_FL_dir_Pin 4
#define motor_FL_pwm_Pin 3
#define motor_FR_dir_Pin 2
#define motor_FR_pwm_Pin 5
// #define motor_BL_dir_Pin 6
// #define motor_BL_pwm_Pin 7
// #define motor_BR_dir_Pin 8 
// #define motor_BR_pwm_Pin 44

int threshold = 400; // threshold of sonar

void accelerate(int speed){
    /* 
    accelerate to int speed
    -255 < speed < 255
    */
    if(speed<0){ // set dir pin low when speed is smaller than 0
        digitalWrite(motor_FL_dir_Pin, LOW);
        digitalWrite(motor_FR_dir_Pin, LOW);
        digitalWrite(motor_BL_dir_Pin, HIGH);
        digitalWrite(motor_BR_dir_Pin, HIGH);
    }
    else if(speed>0){
        digitalWrite(motor_FL_dir_Pin, HIGH);
        digitalWrite(motor_FR_dir_Pin, HIGH);
        digitalWrite(motor_BL_dir_Pin, LOW);
        digitalWrite(motor_BR_dir_Pin, LOW);
    }
    else{   // set motor speed to 0 when speed is 0
        analogWrite(motor_FL_pwm_Pin, 0);
        analogWrite(motor_FR_pwm_Pin, 0);
        analogWrite(motor_BL_pwm_Pin, 0);
        analogWrite(motor_BR_pwm_Pin, 0);
    }
    analogWrite(motor_FL_pwm_Pin, abs(speed));
    analogWrite(motor_FR_pwm_Pin, abs(speed));
    analogWrite(motor_BL_pwm_Pin, abs(speed));
    analogWrite(motor_BR_pwm_Pin, abs(speed));
}

void turn(int speed){
    /* 
    rotate in int speed
    ccw -255 < speed < 255 cw
    */
    if(speed<0){ // turn left when speed is smaller than 0
        digitalWrite(motor_FL_dir_Pin, HIGH);  // left side go backward
        digitalWrite(motor_FR_dir_Pin, LOW); // right side go forward
        digitalWrite(motor_BL_dir_Pin, LOW);
        digitalWrite(motor_BR_dir_Pin, HIGH);
    }
    else if(speed>0){
        digitalWrite(motor_FL_dir_Pin, LOW);
        digitalWrite(motor_FR_dir_Pin, HIGH);
        digitalWrite(motor_BL_dir_Pin, HIGH);
        digitalWrite(motor_BR_dir_Pin, LOW);
    }
    else{   // set motor speed to 0 when speed is 0
        analogWrite(motor_FL_pwm_Pin, 0);
        analogWrite(motor_FR_pwm_Pin, 0);
        analogWrite(motor_BL_pwm_Pin, 0);
        analogWrite(motor_BR_pwm_Pin, 0);
    }
    analogWrite(motor_FL_pwm_Pin, abs(speed));
    analogWrite(motor_FR_pwm_Pin, abs(speed));
    analogWrite(motor_BL_pwm_Pin, abs(speed));
    analogWrite(motor_BR_pwm_Pin, abs(speed));
}