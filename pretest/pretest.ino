#define motor_FL_dir_Pin 4
#define motor_FL_pwm_Pin 5
#define motor_FR_dir_Pin 6
#define motor_FR_pwm_Pin 7

#define TRIG_F 23
#define ECHO_F 22
#define TRIG_B 25
#define ECHO_B 24

float result1;
float result2;
int speed;
int Sonar_threshold = 500; // about 1000 should be right

#define IR_FL A0
#define IR_FR A1
#define IR_BL A2
#define IR_BR A3

// Threshold of IR sensors
int IR_threshold_FL = 1023;
int IR_threshold_FR = 1023;
int IR_threshold_BL = 1023;
int IR_threshold_BR = 1023;

int IR_result[4];
int IR_threshold[4] = {1023, 1023, 1023, 1023};

int i = 0;

void setup() {
    Serial.begin(115200);

    for (i = 2; i < 6; i++) {
        pinMode(i, OUTPUT);
    }
    pinMode(TRIG_F, OUTPUT);
    pinMode(ECHO_F, INPUT);
    pinMode(TRIG_B, OUTPUT);
    pinMode(ECHO_B, INPUT);
    pinMode(IR_FL, INPUT);
    pinMode(IR_FR, INPUT);
    pinMode(IR_BL, INPUT);
    pinMode(IR_BR, INPUT);

    // Calibrate IR sensors
    for (i = 0; i < 20; i++) {
        IR_result[0] = analogRead(IR_FL);
        IR_result[1] = analogRead(IR_FR);
        IR_result[2] = analogRead(IR_BL);
        IR_result[3] = analogRead(IR_BR);

        if (IR_threshold[0] > IR_result[0]) {
            IR_threshold[0] = IR_result[0] - 80;
        }
        if (IR_threshold[1] > IR_result[1]) {
            IR_threshold[1] = IR_result[1] - 80;
        }
        if (IR_threshold[2] > IR_result[2]) {
            IR_threshold[2] = IR_result[2] - 80;
        }
        if (IR_threshold[3] > IR_result[3]) {
            IR_threshold[3] = IR_result[3] - 80;
        }

        // Print IR thresholds
        Serial.print("IR Calibrated: ");
        Serial.print(IR_threshold[0]);
        Serial.print(", ");
        Serial.print(IR_threshold[1]);
        Serial.print(", ");
        Serial.print(IR_threshold[2]);
        Serial.print(", ");
        Serial.println(IR_threshold[3]);

        delay(50);
    }
    turn(50);
}

void loop() {
    // Look for target with sonar (front and back)
    result1 = Ping(ECHO_F, TRIG_F);
    result2 = Ping(ECHO_B, TRIG_B);
    Serial.print("SONAR_F: ");
    Serial.print(result1);
    Serial.print(" SONAR_B: ");
    Serial.println(result2);

    if (result1 != 0 && result1 < Sonar_threshold) {
        Serial.println("Front obstacle detected");
        accelerate(170);
        if(result1 < 150){
          accelerate(255);
        }
    } 
    else if (result2 != 0 && result2 < Sonar_threshold) {
        Serial.println("Back obstacle detected");
        accelerate(-170);
        if(result2 < 150){
          accelerate(-255);
        }
    } 
    else {
        turn(70);
    }

    // Look out for clifs
    IR_result[0] = analogRead(IR_FL);
    IR_result[1] = analogRead(IR_FR);
    IR_result[2] = analogRead(IR_BL);
    IR_result[3] = analogRead(IR_BR);

    // Check front IR sensors
    if ((IR_result[0] < IR_threshold[0]) && (IR_result[1] < IR_threshold[1])) {
        Serial.println("Front IR: Both triggered, turning 180 degrees");
        accelerate(-150);
        delay(500);
        turn(-100);
        delay(700);
    } 
    else if (IR_result[0] < IR_threshold[0]) {
        Serial.println("Front IR: FL triggered, turning CCW");
        accelerate(-150);
        delay(500);
        turn(-100);
        delay(500);
    } 
    else if (IR_result[1] < IR_threshold[1]) {
        Serial.println("Front IR: FR triggered, turning CW");
        accelerate(-150);
        delay(500);
        turn(100);
        delay(500);
    }

    // Check back IR sensors
    if ((IR_result[2] < IR_threshold[2]) && (IR_result[3] < IR_threshold[3])) {
        Serial.println("Back IR: Both triggered, moving forward and turning 180 degrees");
        accelerate(150);
        delay(500);
        turn(100);
        delay(700);
    } else if (IR_result[2] < IR_threshold[2]) {
        Serial.println("Back IR: BL triggered, turning CW");
        accelerate(150);
        delay(500);
        turn(100);
        delay(500);
    } else if (IR_result[3] < IR_threshold[3]) {
        Serial.println("Back IR: BR triggered, turning CCW");
        accelerate(150);
        delay(500);
        turn(-100);
        delay(500);
    }
}

void accelerate(int speed) {
    // accelerate to int speed
    // -255 <= speed <= 255
    
    speed = constrain(speed, -255, 255);

    if (speed < 0) { // Set dir pin low when speed is smaller than 0
        digitalWrite(motor_FL_dir_Pin, HIGH);
        digitalWrite(motor_FR_dir_Pin, HIGH);
    } else if (speed > 0) {
        digitalWrite(motor_FL_dir_Pin, LOW);
        digitalWrite(motor_FR_dir_Pin, LOW);
    } else { // Set motor speed to 0 when speed is 0
        analogWrite(motor_FL_pwm_Pin, 0);
        analogWrite(motor_FR_pwm_Pin, 0);
    }
    analogWrite(motor_FL_pwm_Pin, abs(speed));
    analogWrite(motor_FR_pwm_Pin, abs(speed));
}

void turn(int speed) {
    speed = constrain(speed, -254, 254);

    if (speed < 0) { // Turn left when speed is smaller than 0
        digitalWrite(motor_FL_dir_Pin, HIGH);  // Left side go backward
        digitalWrite(motor_FR_dir_Pin, LOW);   // Right side go forward
    } else if (speed > 0) {
        digitalWrite(motor_FL_dir_Pin, LOW);
        digitalWrite(motor_FR_dir_Pin, HIGH);
    } else { // Set motor speed to 0 when speed is 0
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
    long duration; // Variable for the duration of sound wave travel
    int distance;
    digitalWrite(trig, LOW);
    delayMicroseconds(1);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echo, HIGH, 50000); // Timeout of 50000 microseconds approx 8.5m
    distance = duration * 0.34 / 2;        // Speed of sound wave divided by 2 (go and back)
    return (distance);                     // mm
}
