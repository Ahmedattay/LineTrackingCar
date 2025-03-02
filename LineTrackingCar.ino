// Defining the Pins

// Motor Shield Pins (A,1,2 for Right , B,3,4 for Left)
#define ENA_Pin PB8   // PWM pin for Left Motor
#define ENB_Pin PB9   // PWM pin for Right Motor
#define IN1_Pin PA0
#define IN2_Pin PA1
#define IN3_Pin PA2
#define IN4_Pin PA3

// IR sensors Signals
#define RIGHTMOSTSENSOR PA4
#define RIGHTSENSOR PA5
#define LEFTSENSOR PA6
#define LEFTMOSTSENSOR PA7

// Color Sensors Signals and Output
#define S0 PB0
#define S1 PB1
#define S2 PB6
#define S3 PB5
#define SENSOR_OUT PB7

//////////////////////////////////////////////////



// Setup Function that Called only once
void setup() {

    // Choosing the Pinned pins as Output or Input

    // Motors
    pinMode(IN1_Pin, OUTPUT);
    pinMode(IN2_Pin, OUTPUT);
    pinMode(IN3_Pin, OUTPUT);
    pinMode(IN4_Pin, OUTPUT);
    pinMode(ENA_Pin, OUTPUT);
    pinMode(ENB_Pin, OUTPUT);
    
    // IR Sensors
    pinMode(RIGHTMOSTSENSOR, INPUT_PULLUP);
    pinMode(RIGHTSENSOR, INPUT_PULLUP);
    pinMode(LEFTSENSOR, INPUT_PULLUP);
    pinMode(LEFTMOSTSENSOR, INPUT_PULLUP);

    // Color Sensor
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(SENSOR_OUT, INPUT);

    // For choosin the Frequence of the Sensor
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    analogWriteFrequency(1000); // Set PWM frequency to 1 kHz
}
//////////////////////////////////////////////////



// Motor Function ( for controlling each motor respectively)
// motor 0 = left , motor 1 = right
// Work 0 = Stop , Work 1 = Move
// Direction 0 = Backward , Direction 1 = Forward
// Speed ( PWM Frequency on the Enable to control the speed out of 4095 Max)
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

//////////////////////////////////////////////////


// For Reading the color
// Read the value of the red when R , and value of Greens when G , and value of Blue when B

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



// the Function that will be made when called when the sensors reads a green color
// Moves and rotates and moves and rotates again to the same point
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


// After Moving , it goes to an infinite loop stoping the car , and it only exit that loop when all sensors are black ( That means the car is lift up in the air and put somewhere else)
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


// The main loop Function
void loop() {

    // Reading the sensors Value
    int RightMostSensor = digitalRead(RIGHTMOSTSENSOR);
    int RightLeastSensor = digitalRead(RIGHTSENSOR);
    int LeftMostSensor = digitalRead(LEFTSENSOR);
    int LeftLeastSensor = digitalRead(LEFTMOSTSENSOR);
    
    int redValue = readColor('R');
    int greenValue = readColor('G');
    int blueValue = readColor('B');
    
    int speed = 2500; // Adjust speed (Max 4095 for STM32 PWM)


    // the Green Count ( soooon )

    // int GreenCount = 0;
    // if(greenValue < redValue && greenValue < blueValue && greenValue < 200){
    //   GreenCount++ ;
    // }


    // When green is seen it calles the function of the rotationg U-Tern
    if (   (greenValue < redValue && greenValue < blueValue && greenValue < 200)      ) { // Confirm green detection          &&     ( GreenCount % 2 == 0 )    
      TheRotatingFunction(greenValue , redValue , blueValue , RightMostSensor , RightLeastSensor , LeftMostSensor , LeftLeastSensor , speed);
    }
    
    // When Red is seen it Stop the car for 5 Seconds and Moves again in the track
    if (redValue < greenValue && redValue < blueValue && redValue < 200) { // Confirm red detection
        motor_control(1, 0, 1, speed);
        motor_control(0, 0, 1, speed);
        delay(5000);

        motor_control(1, 1, 1, speed);
        motor_control(0, 1, 1, speed);
        delay(300);
        return;
    }
    


    // The main logic movements of the car
    
    // All Black ( in the Air ) = Stop the motors
    // All White = Move Forward
    // Right Most Black = Move Left slow
    // Right Least Black = Move Left Fast ( run one motor and the other is run in the other direction )
    // Left Most Black = Move Right slow
    // Left Least Black = Move Right Fast ( run one motor and the other is run in the other direction )
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
