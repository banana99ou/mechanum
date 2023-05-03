#define ch_forward 12 //3
#define ch_stripe 11  //4
#define ch_yaw 13 //1
#define motor_FL_speed 10
#define motor_FR_speed 9
#define motor_BL_speed 8
#define motor_BR_speed 7
#define motor_FL_dir 6
#define motor_FR_dir 5
#define motor_BL_dir 4
#define motor_BR_dir 3

int i, forward, stripe, yaw, vector;

void setup()
{
    Serial.begin(9600);
    for (i = 13; i > 10; i--)
    {
        pinMode(i, INPUT);
    }
    for (i = 3; i < 11; i++)
    {
        pinMode(i, OUTPUT);
    }
}

void loop()
{
    int ch_forward_pwm = map(constrain(pulseIn(ch_forward, HIGH), 1040, 1870), 1040, 1870, -100, 100);
    int ch_stripe_pwm  = map(constrain(pulseIn(ch_stripe,  HIGH), 1115, 1780), 1115, 1780, -100, 100);
    int ch_yaw_pwm     = map(constrain(pulseIn(ch_yaw,     HIGH),  998, 1905),  998, 1905, -100, 100);
    
    /*
    unsigned long ch_forward_pwm = pulseIn(ch_forward, HIGH);
    unsigned long ch_stripe_pwm = pulseIn(ch_stripe, HIGH);
    unsigned long ch_yaw_pwm = pulseIn(ch_yaw, HIGH);
     
    Serial.print("ch_forward_pwm: ");
    Serial.print(ch_forward_pwm);
    Serial.print(", ");
    Serial.print("ch_stripe_pwm: ");
    Serial.print(ch_stripe_pwm);
    Serial.print(", ");
    Serial.print("ch_yaw_pwm: ");
    Serial.println(ch_yaw_pwm);
    */

    //map deadzone
    if(ch_forward > 4){
        forward = ch_forward;
    }
    else if(ch_forward < -4){
        forward = ch_forward;
    }
    else{
        forward = 0;
    }

    if(ch_stripe > 4){
        stripe = ch_stripe;
    }
    else if(ch_stripe < -4){
        stripe = ch_stripe;
    }
    else{
        stripe = 0;
    }

    if(ch_yaw > 4){
        yaw = ch_yaw;
    }
    else if(ch_yaw < -4){
        yaw = ch_yaw;
    }
    else{
        yaw = 0;
    }

    int temp[] = {forward - stripe + yaw, forward + stripe - yaw, forward + stripe + yaw, forward - stripe - yaw};
    int vector[4] = {map(temp, -300, 300, 0, 255)};
    if(vector[0] > 0){
        motor_FL_dir = HIGH;
    }
    else if(vector[0] < 0){
        motor_FL_dir = LOW;
    }
    if(vector[1] > 0){
        motor_FR_dir = HIGH;
    }
    else if(vector[1] < 0){
        motor_FR_dir = LOW;
    }
    if(vector[2] > 0){
        motor_BL_dir = HIGH;
    }
    else if(vector[2] < 0){
        motor_BL_dir = LOW;
    }
    if(vector[3] > 0){
        motor_BR_dir = HIGH;
    }
    else if(vector[3] < 0){
        motor_BR_dir = LOW;
    }
    /*
    figure out which channel is throtle and wich channel is yaw
    and set dir pin acordingly

    transform pwm value to movement vector and yaw vector
    then figure out pwm for each motor
    */
}