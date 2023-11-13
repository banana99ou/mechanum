#define motor_FL_dir_Pin 2
#define motor_FL_pwm_Pin 3
#define motor_FR_dir_Pin 4
#define motor_FR_pwm_Pin 5

bool stringComplete;
String inputString;
int i = 0;

void setup(){
    Serial.begin(9600);
    for(i=2; i<5; i++){
        pinMode(i, OUTPUT);
    }
}

void loop(){

    move(10, 0);
    delay(500);
    move(-10,0);
    delay(500);
    move(10, 10);
    delay(500);
    move(-10,10);
    delay(500);

    // Serial.println("running");
    serialEvent();
    if(stringComplete) {

        if(inputString.startsWith("accel")) {
            Serial.print("Ack: accel ");
            int amount = inputString.substring(6).toInt();
            Serial.println(amount);
            accelerate(amount);
            delay(500);
            accelerate(0);
        }
        if(inputString.startsWith("turn")) {
            Serial.println("Ack: turn");
            int amount = inputString.substring(5).toInt();
            Serial.println(amount);
            turn(amount);
            delay(500);
            turn(0);
        }
        // if(inputString.startsWith("move")) {
        //     Serial.println("Ack: move");
        //     int amount = inputString.substring(5).toInt();
        //     Serial.println(amount);
        //     move(amount);
        // }
        
        inputString = "";
        stringComplete = false;
    }
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
            digitalWrite(motor_FL_dir_Pin, LOW);
            digitalWrite(motor_FR_dir_Pin, LOW);
        }
        else if(speed>0){
            digitalWrite(motor_FL_dir_Pin, HIGH);
            digitalWrite(motor_FR_dir_Pin, HIGH);
        }
    }
    else if(abs(turn_radius)<wheel_width){
        // turning point is inside of robot -> 
        if(speed<0){ // turn left when speed is smaller than 0
            digitalWrite(motor_FL_dir_Pin, HIGH);  // left side go backward
            digitalWrite(motor_FR_dir_Pin, LOW); // right side go forward
        }
        else if(speed>0){
            digitalWrite(motor_FL_dir_Pin, LOW);
            digitalWrite(motor_FR_dir_Pin, HIGH);
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

void serialEvent() {
    while(Serial.available()){
        char inChar = (char)Serial.read();
        inputString += inChar;
        if(inChar == '\n') {
        stringComplete = true;
        }
    }
}