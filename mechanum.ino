int motor_FL_dir_Pin = 4;
int FL_speed = 5;
int motor_FR_dir_Pin = 2;
int FR_speed = 3;
int motor_BL_dir_Pin = 6;
int BL_speed = 7;
int motor_BR_dir_Pin = 8;
int BR_speed = 9;


int motor_FL_speed = 0;
int motor_FR_speed = 0;
int motor_BL_speed = 0;
int motor_BR_speed = 0;

int i = 0;

void setup() {
  Serial.begin(9600);

  for(i=2; i<10; i++){
    pinMode(i, OUTPUT);
  }
  for(i=10; i<14; i++){
    pinMode(i, INPUT);
  }
}

void loop() {
  int forward_raw = constrain(pulseIn(12, HIGH), 1044, 1885);
  int stripe_raw = constrain(pulseIn(13, HIGH), 1135, 1800);
  int yaw_raw = constrain(pulseIn(11, HIGH, 25000), 1013, 1941);

  int forward = map(forward_raw, 1044, 1885, -254, 254);
  int stripe = map(stripe_raw, 1135, 1800, -254, 254);
  int yaw = map(yaw_raw, 1013, 1941, -254, 254);

  //Serial.print(forward);
  if(abs(forward) < 24){
    forward = 0;
  }
  //Serial.println(forward);
  if(abs(stripe) < 24){
    stripe = 0;
  }
  if(abs(yaw) < 30){
    yaw = 0;
  }

  /* 
  Serial.print(forward);
  Serial.print(", ");
  Serial.print(stripe);
  Serial.print(", ");
  Serial.print(yaw);
  Serial.println(", ");
  */
  
  //Serial.print(stripe);
  //Serial.print(", ");
  
  //Serial.println(forward_raw);
  //Serial.print(", ");
  

  motor_FL_speed = map(forward - stripe + yaw, -254*3, 254*3, -254, 254);
  motor_FR_speed = map(forward + stripe - yaw, -254*3, 254*3, -254, 254);
  motor_BL_speed = map(forward + stripe + yaw, -254*3, 254*3, -254, 254);
  motor_BR_speed = map(forward - stripe - yaw, -254*3, 254*3, -254, 254);

  
  Serial.print(motor_FL_speed);
  Serial.print(", ");
  Serial.print(motor_FR_speed);
  Serial.print(", ");
  Serial.print(motor_BL_speed);
  Serial.print(", ");
  Serial.println(motor_BR_speed);
  
  
  int motor_FL_dir = (forward - stripe + yaw >= 0) ? HIGH : LOW;
  int motor_FR_dir = (forward + stripe - yaw >= 0) ? HIGH : LOW;
  int motor_BL_dir = (forward + stripe + yaw >= 0) ? LOW : HIGH;
  int motor_BR_dir = (forward - stripe - yaw >= 0) ? LOW : HIGH;

  /*
  Serial.print(motor_FL_dir);
  Serial.print(", ");
  Serial.print(motor_FR_dir);
  Serial.print(", ");
  Serial.print(motor_BL_dir);
  Serial.print(", ");
  Serial.println(motor_BR_dir);
  */

  analogWrite(FL_speed, abs(motor_FL_speed));
  analogWrite(FR_speed, abs(motor_FR_speed));
  analogWrite(BL_speed, abs(motor_BL_speed));
  analogWrite(BR_speed, abs(motor_BR_speed));

  digitalWrite(motor_FL_dir_Pin, motor_FL_dir);
  digitalWrite(motor_FR_dir_Pin, motor_FR_dir);
  digitalWrite(motor_BL_dir_Pin, motor_BL_dir);
  digitalWrite(motor_BR_dir_Pin, motor_BR_dir);
}
