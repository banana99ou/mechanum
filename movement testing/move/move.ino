#define motor_FL_dir_Pin 4
#define motor_FL_pwm_Pin 5
#define motor_FR_dir_Pin 2
#define motor_FR_pwm_Pin 3
#define motor_BL_dir_Pin 6
#define motor_BL_pwm_Pin 7
#define motor_BR_dir_Pin 8 
#define motor_BR_pwm_Pin 44

void accelerate(int speed){
    /* 
    accelerate to int speed
    -255 < speed < 255
    */
    if(speed<0){ // set dir pin low when speed is smaller than 0
        digitalwrite(motor_FL_dir_Pin, LOW);
        digitalwrite(motor_FR_dir_Pin, LOW);
        digitalwrite(motor_BL_dir_Pin, LOW);
        digitalwrite(motor_BR_dir_Pin, LOW);
    }
    else if(speed>0){
        digitalwrite(motor_FL_dir_Pin, HIGH);
        digitalwrite(motor_FR_dir_Pin, HIGH);
        digitalwrite(motor_BL_dir_Pin, HIGH);
        digitalwrite(motor_BR_dir_Pin, HIGH);
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
        digitalwrite(motor_FL_dir_Pin, LOW);  // left side go backward
        digitalwrite(motor_FR_dir_Pin, HIGH); // right side go forward
        digitalwrite(motor_BL_dir_Pin, LOW);
        digitalwrite(motor_BR_dir_Pin, HIGH);
    }
    else if(speed>0){
        digitalwrite(motor_FL_dir_Pin, HIGH);
        digitalwrite(motor_FR_dir_Pin, LOW);
        digitalwrite(motor_BL_dir_Pin, HIGH);
        digitalwrite(motor_BR_dir_Pin, LOW);
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

void setup(){
    Serial.begin(9600);
    for(i=0; i<9; i++){
        pinMode(i, OUTPUT);
    }
    pinMode(motor_BR_pwm_Pin, OUTPUT);
}

void loop(){
    serialEvent();
    if(stringComplete) {

        if(inputString.startsWith("accel")) {
            Serial.println("Ack: accel");
            int amount = inputString.substring(6).toInt();
            accelerate(amount);
        }
        if(inputString.startsWith("turn")) {
            Serial.println("Ack: turn");
            int amount = inputString.substring(5).toInt();
            turn(amount);
        }
        
        inputString = "";
        stringComplete = false;
    }
}