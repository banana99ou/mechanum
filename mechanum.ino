int motor_FL_dir_Pin = 4;
int FL_speed = 5;
int motor_FR_dir_Pin = 2;
int FR_speed = 3;
int motor_BL_dir_Pin = 7;
int BL_speed = 8;
int motor_BR_dir_Pin = 9;
int BR_speed = 10;

int i = 0;

void setup() {
  Serial.begin(9600);

  for(i=0; i<12; i++){
    pinMode(i, OUTPUT);
  }
  for(i=11; i<14; i++){
    pinMode(i, INPUT);
  }
}

void loop() {
  int forward = map(pulseIn(11, HIGH, 25000), 1040, 1870, -254, 254);
  int stripe = map(pulseIn(12, HIGH, 25000), 1115, 1780, -254, 254);
  int yaw = map(pulseIn(13, HIGH, 25000), 998, 1905, -254, 254);

  //Serial.print(forward);
  if(abs(forward) < 10){
    forward = 0;
  }
  //Serial.println(forward);
  if(abs(stripe) < 10){
    stripe = 0;
  }
  if(abs(yaw) < 10){
    yaw = 0;
  }

  int motor_FL_speed = map(forward - stripe - yaw, -254*3, 254*3, -254, 254);
  int motor_FR_speed = map(forward + stripe + yaw, -254*3, 254*3, -254, 254);
  int motor_BL_speed = map(forward + stripe - yaw, -254*3, 254*3, -254, 254);
  int motor_BR_speed = map(forward - stripe + yaw, -254*3, 254*3, -254, 254);

  /*
  Serial.print("motor FL ");
  Serial.print(motor_FL_speed);
  Serial.print(", motor FR ");
  Serial.print(motor_FR_speed);
  Serial.print(", motor BL ");
  Serial.print(motor_BL_speed);
  Serial.print(", motor BR ");
  Serial.println(motor_BR_speed);
  */

  analogWrite(FL_speed, abs(motor_FL_speed));
  analogWrite(FR_speed, abs(motor_FR_speed));
  analogWrite(BL_speed, abs(motor_BL_speed));
  analogWrite(BR_speed, abs(motor_BR_speed));

  int motor_FL_dir = (forward - stripe - yaw >= 0) ? HIGH : LOW;
  int motor_FR_dir = (forward + stripe + yaw >= 0) ? HIGH : LOW;
  int motor_BL_dir = (forward + stripe - yaw >= 0) ? HIGH : LOW;
  int motor_BR_dir = (forward - stripe + yaw >= 0) ? HIGH : LOW;

  digitalWrite(motor_FL_dir_Pin, motor_FL_dir);
  digitalWrite(motor_FR_dir_Pin, motor_FR_dir);
  digitalWrite(motor_BL_dir_Pin, motor_BL_dir);
  digitalWrite(motor_BR_dir_Pin, motor_BR_dir);
}
