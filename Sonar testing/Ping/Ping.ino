#define TRIG_FF 9 //10
#define ECHO_FF 8 //11

#define TRIG_2 14
#define ECHO_2 15

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_FF, OUTPUT);
  pinMode(ECHO_FF, INPUT);
  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_2, INPUT);
}

void loop() {
  int result1 = Ping(ECHO_FF, TRIG_FF);
  int result2 = Ping(ECHO_2, TRIG_2);
  Serial.print(result1);
  Serial.print(", ");
  Serial.println(result2);
  result1 = 0.9248*result1 - 10.04;
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