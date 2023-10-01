/*
program to verify trct5000 sensor function
test reading from trct5000 sensor and test threshold works
*/

#define IR_FL A0
#define IR_FR A5
#define IR_BL A6
#define IR_BR A7

// Threshold of IR sensors
int IR_threshold_FL = 0;
int IR_threshold_FR = 0;
int IR_threshold_BL = 0;
int IR_threshold_BR = 0;

int i = 0;

void setup(){
    Serial.begin(9600);
  
    pinMode(IR_FL, INPUT);
    // pinMode(IR_FR, INPUT);
    // pinMode(IR_BL, INPUT);
    // pinMode(IR_BR, INPUT);

    for(i=0; i<20; i++){
        // use lowest value in current situation
        if(IR_threshold_FL < analogRead(IR_FL)){
            IR_threshold_FL = analogRead(IR_FL) + 100;
        }
        // if(IR_threshold_FR < analogRead(IR_FR)){
        //     IR_threshold_FR = analogRead(IR_FR) + 100;
        // }
        // if(IR_threshold_BL < analogRead(IR_BL)){
        //     IR_threshold_BL = analogRead(IR_BL) + 100;
        // }
        // if(IR_threshold_BR < analogRead(IR_BR)){
        //     IR_threshold_BR = analogRead(IR_BR) + 100;
        // }

        // print IR threshold
        Serial.print("IR Calibrated: ");
        Serial.println(IR_threshold_FL);
        // Serial.print(" ");
        // Serial.print(IR_threshold_FR);
        // Serial.print(" ");
        // Serial.print(IR_threshold_BL);
        // Serial.print(" ");
        // Serial.println(IR_threshold_BR);
    }
}

void loop(){
    int IR_result[4];
    IR_result[0] = analogRead(IR_FL);
    // IR_result[1] = analogRead(IR_FR);
    // IR_result[2] = analogRead(IR_BL);
    // IR_result[3] = analogRead(IR_BR);

    Serial.println(IR_result[0]);
//    Serial.println("Is result over threshold?");
//    if(IR_result[0]>IR_threshold_FL){
//        Serial.println("FL: yes");
//    }
//    else{
//        Serial.println("FL: no");
//    }
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
