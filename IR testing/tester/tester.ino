int result = 0;
int i = 0;

void setup(){
    pinMode(A0, INPUT);
    Serial.begin(9600);
    for(i=0; i<201; i++){
        result += analogRead(A0);
        Serial.println(result);
        delay(10);
    }
    Serial.println(result/200);
}

void loop(){
//  Serial.println(analogRead(A0));
}
