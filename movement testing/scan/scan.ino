#define motor_FL_dir_Pin 4
#define motor_FL_pwm_Pin 3
#define motor_FR_dir_Pin 2
#define motor_FR_pwm_Pin 5
#define motor_BL_dir_Pin 6
#define motor_BL_pwm_Pin 7
#define motor_BR_dir_Pin 8 
#define motor_BR_pwm_Pin 44

#define TRIG_FF 10
#define ECHO_FF 11

float result1;
int speed;
int threshold = 80;
int i = 0;

void setup(){
    Serial.begin(9600);
    for(i=0; i<9; i++){
        pinMode(i, OUTPUT);
    }
    pinMode(motor_BR_pwm_Pin, OUTPUT);
    pinMode(TRIG_FF, OUTPUT);
    pinMode(ECHO_FF, INPUT);
}

void loop(){
    result1 = Ping(ECHO_FF, TRIG_FF);
    // Serial.print("corrected: ");
    // Serial.print(result1);
    if(result1 < threshold){
        Serial.println("accel");
        accelerate(70);
    }
    else if(result1 > threshold){
        Serial.println("turn");
        turn(70);
    }
}

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
    Serial.println("abs(speed): " + abs(speed));
}

void turn(int speed){
    /* 
    rotate in int speed
    ccw -255 < speed < 255 cw
    */

    Serial.print("speed: ");
    Serial.print(speed);

    if(speed<0){ // turn left when speed is smaller than 0
        Serial.print(" speed<0 ");
        digitalWrite(motor_FL_dir_Pin, HIGH);  // left side go backward
        digitalWrite(motor_FR_dir_Pin, LOW); // right side go forward
        digitalWrite(motor_BL_dir_Pin, LOW);
        digitalWrite(motor_BR_dir_Pin, HIGH);
    }
    else if(speed>0){
        Serial.print(" speed>0 ");
        digitalWrite(motor_FL_dir_Pin, LOW);
        digitalWrite(motor_FR_dir_Pin, HIGH);
        digitalWrite(motor_BL_dir_Pin, HIGH);
        digitalWrite(motor_BR_dir_Pin, LOW);
    }
    else{   // set motor speed to 0 when speed is 0
        Serial.print(" speed==0 ");
        analogWrite(motor_FL_pwm_Pin, 0);
        analogWrite(motor_FR_pwm_Pin, 0);
        analogWrite(motor_BL_pwm_Pin, 0);
        analogWrite(motor_BR_pwm_Pin, 0);
    }
    Serial.print("abs(speed): ");
    Serial.println(abs(speed));
    analogWrite(motor_FL_pwm_Pin, abs(speed));
    analogWrite(motor_FR_pwm_Pin, abs(speed));
    analogWrite(motor_BL_pwm_Pin, abs(speed));
    analogWrite(motor_BR_pwm_Pin, abs(speed));
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
    int wheel_width = 392;
    int rightside_speed = 0;
    int leftside_speed = 0;
    if(abs(turn_radius)>wheel_width){
        // turning point is outside of robot -> all wheels go forward
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
    }
    else if(abs(turn_radius)<wheel_width){
        // turning point is inside of robot -> 
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
    }
    else{   // set motor speed to 0 when speed is 0
        analogWrite(motor_FL_pwm_Pin, 0);
        analogWrite(motor_FR_pwm_Pin, 0);
        analogWrite(motor_BL_pwm_Pin, 0);
        analogWrite(motor_BR_pwm_Pin, 0);
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
    analogWrite(motor_BL_pwm_Pin, leftside_speed);
    analogWrite(motor_BR_pwm_Pin, rightside_speed);
}

int Ping(int echo, int trig) {
    long duration;
    int distance;
    digitalWrite(trig, LOW);
    delayMicroseconds(1);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echo, HIGH, 2000); // timeout of 2000 microseconds approx 0.68m
    distance = duration * 0.34 / 2;  // Speed of sound wave divided by 2 (go and back)
    return (distance);               //mm
}
