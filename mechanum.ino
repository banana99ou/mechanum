int motor_FL_dir;
int motor_FR_dir;
int motor_BL_dir;
int motor_BR_dir;

void setup() {
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  motor_FL_dir = 0;
  motor_FR_dir = 0;
  motor_BL_dir = 0;
  motor_BR_dir = 0;
}

void loop() {
  int forward = map(pulseIn(11, HIGH, 25000), 1040, 1870, -254, 254);
  int stripe = map(pulseIn(12, HIGH, 25000), 1115, 1780, -254, 254);
  int yaw = map(pulseIn(13, HIGH, 25000), 998, 1905, -254, 254);

  int motor_FL_speed = map(forward - stripe - yaw, -254, 254, 0, 255);
  int motor_FR_speed = map(forward + stripe + yaw, -254, 254, 0, 255);
  int motor_BL_speed = map(forward + stripe - yaw, -254, 254, 0, 255);
  int motor_BR_speed = map(forward - stripe + yaw, -254, 254, 0, 255);

  analogWrite(5, motor_FL_speed);
  analogWrite(6, motor_FR_speed);
  analogWrite(9, motor_BL_speed);
  analogWrite(10, motor_BR_speed);

  motor_FL_dir = (forward - stripe - yaw >= 0) ? HIGH : LOW;
  motor_FR_dir = (forward + stripe + yaw >= 0) ? HIGH : LOW;
  motor_BL_dir = (forward + stripe - yaw >= 0) ? HIGH : LOW;
  motor_BR_dir = (forward - stripe + yaw >= 0) ? HIGH : LOW;

  digitalWrite(7, motor_FL_dir);
  digitalWrite(8, motor_FR_dir);
  digitalWrite(11, motor_BL_dir);
  digitalWrite(12, motor_BR_dir);
}
