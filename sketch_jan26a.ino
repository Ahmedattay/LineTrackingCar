#define ENA_Pin PB8   // PWM pin for Left Motor
#define ENB_Pin PB9   // PWM pin for Right Motor
#define IN1_Pin PA0
#define IN2_Pin PA1
#define IN3_Pin PA2
#define IN4_Pin PA3

#define RIGHTMOSTSENSOR PA4
#define RIGHTSENSOR PA5
#define LEFTSENSOR PA6
#define LEFTMOSTSENSOR PA7

#define S0 PB0
#define S1 PB1
#define S2 PB6
#define S3 PB5
#define SENSOR_OUT PB7

void setup() {
    pinMode(IN1_Pin, OUTPUT);
    pinMode(IN2_Pin, OUTPUT);
    pinMode(IN3_Pin, OUTPUT);
    pinMode(IN4_Pin, OUTPUT);
    pinMode(ENA_Pin, OUTPUT);
    pinMode(ENB_Pin, OUTPUT);
    
    pinMode(RIGHTMOSTSENSOR, INPUT_PULLUP);
    pinMode(RIGHTSENSOR, INPUT_PULLUP);
    pinMode(LEFTSENSOR, INPUT_PULLUP);
    pinMode(LEFTMOSTSENSOR, INPUT_PULLUP);

    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(SENSOR_OUT, INPUT);

    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    analogWriteFrequency(1000); // Set PWM frequency to 1 kHz
}

void motor_control(int motorNumber, int work, int direction, int speed) {
    if (motorNumber == 0) { // Left Motor
        analogWrite(ENA_Pin, work ? speed : 0);
        digitalWrite(IN1_Pin, direction ? LOW : HIGH);
        digitalWrite(IN2_Pin, direction ? HIGH : LOW);
    } else if (motorNumber == 1) { // Right Motor
        analogWrite(ENB_Pin, work ? speed : 0);
        digitalWrite(IN3_Pin, direction ? LOW : HIGH);
        digitalWrite(IN4_Pin, direction ? HIGH : LOW);
    }
}

int readColor(char color) {
    switch (color) {
        case 'R': // Red
            digitalWrite(S2, LOW);
            digitalWrite(S3, LOW);
            break;
        case 'G': // Green
            digitalWrite(S2, HIGH);
            digitalWrite(S3, HIGH);
            break;
        case 'B': // Blue
            digitalWrite(S2, LOW);
            digitalWrite(S3, HIGH);
            break;
        default:
            return 0;
    }
    return pulseIn(SENSOR_OUT, LOW);
}

void TheRotatingFunction(int greenValue,int redValue , int blueValue , int RightMostSensor , int RightLeastSensor , int LeftMostSensor , int LeftLeastSensor , int speed) {
        motor_control(1, 1, 1, speed);
        motor_control(0, 1, 1, speed);
        delay(600);
        motor_control(1, 1, 1, speed);
        motor_control(0, 0, 1, speed);
        delay(1150);
////////////////////////
        motor_control(1, 1, 1, speed);
        motor_control(0, 1, 1, speed);
        delay(800);
        motor_control(1, 1, 1, speed);
        motor_control(0, 0, 1, speed);
        delay(1150);
        motor_control(1, 1, 1, speed);
        motor_control(0, 1, 1, speed);
        delay(200);
////////////////////////
          while(1){
            motor_control(1, 0, 1, speed);
            motor_control(0, 0, 1, speed);

            RightMostSensor = digitalRead(RIGHTMOSTSENSOR);
            RightLeastSensor = digitalRead(RIGHTSENSOR);
            LeftMostSensor = digitalRead(LEFTSENSOR);
            LeftLeastSensor = digitalRead(LEFTMOSTSENSOR);
    
            if(RightMostSensor && RightLeastSensor && LeftMostSensor && LeftLeastSensor){
              break;}
          }
}

void loop() {
    int RightMostSensor = digitalRead(RIGHTMOSTSENSOR);
    int RightLeastSensor = digitalRead(RIGHTSENSOR);
    int LeftMostSensor = digitalRead(LEFTSENSOR);
    int LeftLeastSensor = digitalRead(LEFTMOSTSENSOR);
    
    int redValue = readColor('R');
    int greenValue = readColor('G');
    int blueValue = readColor('B');
    
    int speed = 2500; // Adjust speed (Max 4095 for STM32 PWM)

    // int GreenCount = 0;
    // if(greenValue < redValue && greenValue < blueValue && greenValue < 200){
    //   GreenCount++ ;
    // }

    if (   (greenValue < redValue && greenValue < blueValue && greenValue < 200)      ) { // Confirm green detection          &&     ( GreenCount % 2 == 0 )    
      TheRotatingFunction(greenValue , redValue , blueValue , RightMostSensor , RightLeastSensor , LeftMostSensor , LeftLeastSensor , speed);
    }
    
    if (redValue < greenValue && redValue < blueValue && redValue < 200) { // Confirm red detection
        motor_control(1, 0, 1, speed);
        motor_control(0, 0, 1, speed);
        delay(5100);

        motor_control(1, 1, 1, speed);
        motor_control(0, 1, 1, speed);
        delay(300);
        return;
    }
    
    if (RightMostSensor && RightLeastSensor && LeftMostSensor && LeftLeastSensor) {
        motor_control(1, 0, 1, speed);
        motor_control(0, 0, 1, speed);
        delay(5000);
    } else if (!RightMostSensor && !RightLeastSensor && !LeftMostSensor && !LeftLeastSensor) {
        motor_control(1, 1, 1, speed);
        motor_control(0, 1, 1, speed);
    } else if (RightMostSensor) {
        motor_control(1, 1, 1, speed);
        motor_control(0, 0, 1, speed);
    } else if (RightLeastSensor) {
        motor_control(1, 1, 1, speed);
        motor_control(0, 1, 0, speed);
    } else if (LeftLeastSensor) {
        motor_control(1, 0, 1, speed);
        motor_control(0, 1, 1, speed);
    } else if (LeftMostSensor) {
        motor_control(1, 1, 0, speed);
        motor_control(0, 1, 1, speed);
    }

}
