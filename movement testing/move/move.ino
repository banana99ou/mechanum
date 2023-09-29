#define motor_FL_dir_Pin 4
#define motor_FL_pwm_Pin 5
#define motor_FR_dir_Pin 2
#define motor_FR_pwm_Pin 3
#define motor_BL_dir_Pin 6
#define motor_BL_pwm_Pin 7
#define motor_BR_dir_Pin 8 
#define motor_BR_pwm_Pin 44

bool stringComplete;
String inputString;
int i = 0;

void accelerate(int speed){
    /* 
    accelerate to int speed
    -255 < speed < 255
    */
    if(speed<0){ // set dir pin low when speed is smaller than 0
        digitalWrite(motor_FL_dir_Pin, LOW);
        digitalWrite(motor_FR_dir_Pin, LOW);
        digitalWrite(motor_BL_dir_Pin, LOW);
        digitalWrite(motor_BR_dir_Pin, LOW);
    }
    else if(speed>0){
        digitalWrite(motor_FL_dir_Pin, HIGH);
        digitalWrite(motor_FR_dir_Pin, HIGH);
        digitalWrite(motor_BL_dir_Pin, HIGH);
        digitalWrite(motor_BR_dir_Pin, HIGH);
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
        digitalWrite(motor_FL_dir_Pin, LOW);  // left side go backward
        digitalWrite(motor_FR_dir_Pin, HIGH); // right side go forward
        digitalWrite(motor_BL_dir_Pin, LOW);
        digitalWrite(motor_BR_dir_Pin, HIGH);
    }
    else if(speed>0){
        digitalWrite(motor_FL_dir_Pin, HIGH);
        digitalWrite(motor_FR_dir_Pin, LOW);
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

void serialEvent() {
    while(Serial.available()){
        char inChar = (char)Serial.read();
        inputString += inChar;
        if(inChar == '\n') {
        stringComplete = true;
        }
    }
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