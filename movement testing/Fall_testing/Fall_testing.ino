/*
program to verify 2Y0A21 sensor function
test reading from 2Y0A21 sensor and test threshold works
*/

#define motor_FL_dir_Pin 2
#define motor_FL_pwm_Pin 3
#define motor_FR_dir_Pin 4
#define motor_FR_pwm_Pin 5

#define IR_FL A0
#define IR_FR A5
#define IR_BL A6
#define IR_BR A7

// Threshold of IR sensors
int IR_threshold_FL = 1023;
int IR_threshold_FR = 1023;
int IR_threshold_BL = 1023;
int IR_threshold_BR = 1023;

int IR_result[4];
int IR_threshold[4];

int i = 0;

void setup(){
    Serial.begin(9600);
  
    pinMode(IR_FL, INPUT);
    // pinMode(IR_FR, INPUT);
    // pinMode(IR_BL, INPUT);
    // pinMode(IR_BR, INPUT);

    for(i=0; i<20; i++){
        // use lowest value in current situation
        IR_result[0] = analogRead(IR_FL);
        // IR_result[1] = analogRead(IR_FR);
        // IR_result[2] = analogRead(IR_BL);
        // IR_result[3] = analogRead(IR_BR);
      
        if(IR_threshold[0] > IR_result[0]){
            IR_threshold[0] = IR_result[0] - 80;
        }
        // if(IR_threshold[1] > IR_result[1]){
        //     IR_threshold[1] = IR_result[1] - 80;
        // }
        // if(IR_threshold[2] > IR_result[2]){
        //     IR_threshold[2] = IR_result[2] - 80;
        // }
        // if(IR_threshold[3] > IR_result[3]){
        //     IR_threshold[3] = IR_result[3] - 80;
        // }

        // print IR threshold
        Serial.print("IR Calibrated: ");
        Serial.println(IR_threshold[0]);
        // Serial.print(", ");
        // Serial.print(IR_threshold[1]);
        // Serial.print(", ");
        // Serial.print(IR_threshold[2]);
        // Serial.print(", ");
        // Serial.println(IR_threshold[3]);

        delay(50);
    }
}

void loop(){
    IR_result[0] = analogRead(IR_FL);
    // IR_result[1] = analogRead(IR_FR);
    // IR_result[2] = analogRead(IR_BL);
    // IR_result[3] = analogRead(IR_BR);

    Serial.print(IR_result[0]);
    
    // if both front IR is triggered
    if((IR_result[0] < IR_threshold[0]) && (IR_result[1] < IR_threshold[1])){
        Serial.println(" go back a bit and turn 180 degrees");
        accelerate(-70);
        delay(500);
        turn(-100);
        delay(500);
    }

    // if FL IR is triggered
    if(IR_result[0] < IR_threshold[0]){
        Serial.println(" go back a bit and turn ccw -80 ~ -180");
        accelerate(-70);
        delay(500);
        turn(-100);
        delay(500);
    }

    // if FR IR is triggered
    if(IR_result[1] < IR_threshold[1]){
        Serial.println(" go back a bit and turn cw 80 ~ 180");
        accelerate(-70);
        delay(500);
        turn(100);
        delay(500);
    }
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
        digitalWrite(motor_FL_dir_Pin, LOW);
        digitalWrite(motor_FR_dir_Pin, LOW);
    }
    else if(speed>0){
        digitalWrite(motor_FL_dir_Pin, HIGH);
        digitalWrite(motor_FR_dir_Pin, HIGH);
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
        digitalWrite(motor_FL_dir_Pin, LOW);  // left side go backward
        digitalWrite(motor_FR_dir_Pin, HIGH); // right side go forward
    }
    else if(speed>0){
        digitalWrite(motor_FL_dir_Pin, HIGH);
        digitalWrite(motor_FR_dir_Pin, LOW);
    }
    else{   // set motor speed to 0 when speed is 0
        analogWrite(motor_FL_pwm_Pin, 0);
        analogWrite(motor_FR_pwm_Pin, 0);
    }
    analogWrite(motor_FL_pwm_Pin, abs(speed));
    analogWrite(motor_FR_pwm_Pin, abs(speed));
}