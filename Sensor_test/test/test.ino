#define pwm1 6
#define dir1 7
#define pwm2 4
#define dir2 5

int speed = 0; // 0 ~ 255
int threshold = 1000;

void setup() {
  Serial.begin(9600);
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  //pinMode(out3, INPUT);
}

void loop() {
    analogWrite(pwm1, 255);
    analogWrite(pwm2, 255);
    delay(500);
}