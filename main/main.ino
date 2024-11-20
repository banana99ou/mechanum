#define motor_FL_dir_Pin 4
#define motor_FL_pwm_Pin 5
#define motor_FR_dir_Pin 6
#define motor_FR_pwm_Pin 7

#define TRIG_FF 23 //10
#define ECHO_FF 22 //11

#define TRIG_2 25
#define ECHO_2 24

float result1;
float result2;
int speed;
int Sonar_threshold = 500;// about 1000 should be right

#define IR_FL A0
#define IR_FR A1
#define IR_BL A6
#define IR_BR A7

// Threshold of IR sensors
int IR_threshold_FL = 1023;
int IR_threshold_FR = 1023;
int IR_threshold_BL = 1023;
int IR_threshold_BR = 1023;

int IR_result[4];
int IR_threshold[4] = {1023, 1023, 1023, 1023};

int i = 0; 

void setup(){
    Serial.begin(115200);

    for(i=2; i<6; i++){
        pinMode(i, OUTPUT);
    }
    pinMode(TRIG_FF, OUTPUT);
    pinMode(ECHO_FF, INPUT);
    pinMode(TRIG_2, OUTPUT);
    pinMode(ECHO_2, INPUT);
    pinMode(IR_FL, INPUT);
    pinMode(IR_FR, INPUT);
    // pinMode(IR_BL, INPUT);
    // pinMode(IR_BR, INPUT);

    // calibrate IR sensor
    for(i=0; i<20; i++){
        // use lowest value in current situation
        IR_result[0] = analogRead(IR_FL);
        IR_result[1] = analogRead(IR_FR);
        // IR_result[2] = analogRead(IR_BL);
        // IR_result[3] = analogRead(IR_BR);
      
        if(IR_threshold[0] > IR_result[0]){
            IR_threshold[0] = IR_result[0] - 80;
        }
        if(IR_threshold[1] > IR_result[1]){
            IR_threshold[1] = IR_result[1] - 80;
        }
        // if(IR_threshold[2] > IR_result[2]){
        //     IR_threshold[2] = IR_result[2] - 80;
        // }
        // if(IR_threshold[3] > IR_result[3]){
        //     IR_threshold[3] = IR_result[3] - 80;
        // }

        // print IR threshold
        Serial.print("IR Calibrated: ");
        Serial.print(IR_threshold[0]);
        Serial.print(IR_result[0]);
        Serial.print(", ");
        Serial.println(IR_threshold[1]);
        // Serial.print(", ");
        // Serial.print(IR_threshold[2]);
        // Serial.print(", ");
        // Serial.println(IR_threshold[3]);

        delay(50);
    }
    turn(50);
}

void loop(){
    // look for target with sonar
    result1 = Ping(ECHO_FF, TRIG_FF);
    result2 = Ping(ECHO_2, TRIG_2);
    Serial.print(result1);
    // Serial.print(result2);

    if(result1 != 0){
        if(result1 < Sonar_threshold){
            Serial.print(" accel: ");
            speed = 70;// constrain((threshold - result1), 50, 100);
            Serial.println(speed);
            accelerate(speed);
        }
        if(result1 < 150){
          accelerate(254);
        }
        else if(result1 > Sonar_threshold){
          Serial.println(" turn");
          turn(140);
        }
    }
    
    if(result1 == 0){
      turn(70);
    }

    // after course set, look for obstacle
    IR_result[0] = analogRead(IR_FL);
    IR_result[1] = analogRead(IR_FR);
    // IR_result[2] = analogRead(IR_BL);
    // IR_result[3] = analogRead(IR_BR);

    Serial.print(", IR_result ");
    Serial.println(IR_result[0]);
    
    // if both front IR is triggered
    if((IR_result[0] < IR_threshold[0]) && (IR_result[1] < IR_threshold[1])){
        Serial.println(" go back a bit and turn 180 degrees");
        accelerate(-70);
        delay(500);
        turn(-100);
        delay(700);
    }

    // if FL IR is triggered
    else if(IR_result[0] < IR_threshold[0]){
        Serial.println(" go back a bit and turn ccw -80 ~ -180");
        accelerate(-70);
        delay(500);
        turn(-100);
        delay(500);
    }

    // if FR IR is triggered
    else if(IR_result[1] < IR_threshold[1]){
        Serial.println(" go back a bit and turn cw 80 ~ 180");
        accelerate(-70);
        delay(500);
        turn(100);
        delay(500);
    }

    // else{
    //   accelerate(0);
    // }

    
    // if(IR_result[1]>IR_threshold_FR){
    //     Serial.println("FR: yes");
    // }
    // else{
    //     Serial.println("FR: no");
    // }
    // if(IR_result[2]>IR_threshold_BL){
    //     Serial.println("BL: yes");
    // }
    // else{
    //     Serial.println("BL: no");
    // }
    // if(IR_result[3]>IR_threshold_BR){
    //     Serial.println("BR: yes");
    // }
    // else{
    //     Serial.println("BR: no");
    // }
}

void accelerate(int speed){
    /* 
    accelerate to int speed
    -255 < speed < 255
    */

    speed = constrain(speed, -254, 254);

    if(speed<0){ // set dir pin low when speed is smaller than 0
        digitalWrite(motor_FL_dir_Pin, HIGH);
        digitalWrite(motor_FR_dir_Pin, HIGH);
    }
    else if(speed>0){
        digitalWrite(motor_FL_dir_Pin, LOW);
        digitalWrite(motor_FR_dir_Pin, LOW);
    }
    else{   // set motor speed to 0 when speed is 0
        analogWrite(motor_FL_pwm_Pin, 0);
        analogWrite(motor_FR_pwm_Pin, 0);
    }
    analogWrite(motor_FL_pwm_Pin, abs(speed));
    analogWrite(motor_FR_pwm_Pin, abs(speed));
}

void turn(int speed){
    /* 
    rotate in int speed
    ccw -255 < speed < 255 cw
    */

    speed = constrain(speed, -254, 254);

    if(speed<0){ // turn left when speed is smaller than 0
        digitalWrite(motor_FL_dir_Pin, HIGH);  // left side go backward
        digitalWrite(motor_FR_dir_Pin, LOW); // right side go forward
    }
    else if(speed>0){
        digitalWrite(motor_FL_dir_Pin, LOW);
        digitalWrite(motor_FR_dir_Pin, HIGH);
    }
    else{   // set motor speed to 0 when speed is 0
        analogWrite(motor_FL_pwm_Pin, 0);
        analogWrite(motor_FR_pwm_Pin, 0);
    }
    analogWrite(motor_FL_pwm_Pin, abs(speed));
    analogWrite(motor_FR_pwm_Pin, abs(speed));
}

void move(int speed, int turn_radius){
    /*
    turn relative to point
    turn_radius = distance to turn point from center of robot
    if turn_radius > 0 turn point is in right side
    else turn point is in left side

    when turning point is in center of robot
    robot need to turn in place
    ->
    check if turning_radius is smaller than robot's width

    if turn_radius -> infinity then go forward
    */

    speed = constrain(speed, -254, 254);

    int wheel_width = 392;
    int rightside_speed = 0;
    int leftside_speed = 0;
    if(abs(turn_radius)>wheel_width){
        // turning point is outside of robot -> all wheels go forward
        if(speed<0){ // set dir pin low when speed is smaller than 0
            digitalWrite(motor_FL_dir_Pin, HIGH);
            digitalWrite(motor_FR_dir_Pin, HIGH);
        }
        else if(speed>0){
            digitalWrite(motor_FL_dir_Pin, LOW);
            digitalWrite(motor_FR_dir_Pin, LOW);
        }
    }
    else if(abs(turn_radius)<wheel_width){
        // turning point is inside of robot -> 
        if(speed<0){ // turn left when speed is smaller than 0
            digitalWrite(motor_FL_dir_Pin, LOW);  // left side go backward
            digitalWrite(motor_FR_dir_Pin, HIGH); // right side go forward
        }
        else if(speed>0){
            digitalWrite(motor_FL_dir_Pin, HIGH);
            digitalWrite(motor_FR_dir_Pin, LOW);
        }
    }
    else{   // set motor speed to 0 when speed is 0
        analogWrite(motor_FL_pwm_Pin, 0);
        analogWrite(motor_FR_pwm_Pin, 0);
    }
    // Serial.println(turn_radius);
    if(turn_radius > 0){
        // Serial.println(">0");
        // turning point to the right -> left side should turn faster
        // rightside + leftside = speed 
        // rightside/leftside = (2R+w)/(2R-w)
        rightside_speed = 2*(abs(turn_radius)-wheel_width/2)*3.14*abs(speed)/(4*3.14*abs(turn_radius));
        leftside_speed = 2*(abs(turn_radius)+wheel_width/2)*3.14*abs(speed)/(4*3.14*abs(turn_radius));
    }
    else if(turn_radius < 0){
        // Serial.println("<0");
        // turning point to the left -> right side should turn faster
        rightside_speed = 2*(abs(turn_radius)+wheel_width/2)*3.14*abs(speed)/(4*3.14*abs(turn_radius));
        leftside_speed = 2*(abs(turn_radius)-wheel_width/2)*3.14*abs(speed)/(4*3.14*abs(turn_radius));
    }
    else if(turn_radius == 0){
        // turning point to the left -> right side should turn faster
        rightside_speed = abs(speed);
        leftside_speed = abs(speed);
    }
    // Serial.println(rightside_speed);
    // Serial.println(leftside_speed);
    analogWrite(motor_FL_pwm_Pin, leftside_speed);
    analogWrite(motor_FR_pwm_Pin, rightside_speed);
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
    duration = pulseIn(echo, HIGH, 50000); // timeout of 50000 microseconds approx 8.5m
    distance = duration * 0.34 / 2;  // Speed of sound wave divided by 2 (go and back)
    return (distance);               //mm
}