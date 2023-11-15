#define TRIG_FF 11
#define ECHO_FF 10

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_FF, OUTPUT);
  pinMode(ECHO_FF, INPUT);
}

void loop() {
  int result1 = Ping(ECHO_FF, TRIG_FF);
  Serial.println(result1);
  // long duration;  //variable for the duration of sound wave travel
  // int distance;
  // digitalWrite(trig, LOW);
  // delayMicroseconds(1);
  // digitalWrite(trig, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trig, LOW);
  // // Reads the echoPin, returns the sound wave travel time in microseconds
  // duration = pulseIn(echo, HIGH);
  // // Calculating the distance
  // distance = duration * 0.034 / 2;  // Speed of sound wave divided by 2 (go and back)
  // Serial.println(distance);
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
    duration = pulseIn(echo, HIGH, 2000); // timeout of 2000 microseconds approx 0.68m
    distance = duration * 0.34 / 2;  // Speed of sound wave divided by 2 (go and back)
    return (distance);               //mm
}