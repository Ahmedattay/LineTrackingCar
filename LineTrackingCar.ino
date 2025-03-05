// ======================== DEFINING PINS ======================== //

// Motor Shield Pins (A1, A2 for Right Motor, B3, B4 for Left Motor)
#define ENA_Pin PB8   // PWM pin for Left Motor
#define ENB_Pin PB9   // PWM pin for Right Motor
#define IN1_Pin PA0   // Left Motor Direction Control
#define IN2_Pin PA1
#define IN3_Pin PA2   // Right Motor Direction Control
#define IN4_Pin PA3

// IR Sensor Pins
#define RIGHTMOSTSENSOR PA4
#define RIGHTSENSOR PA5
#define LEFTSENSOR PA6
#define LEFTMOSTSENSOR PA7

// Color Sensor Pins
#define S0 PB0
#define S1 PB1
#define S2 PB6
#define S3 PB5
#define SENSOR_OUT PB7 // Output signal from color sensor

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

    // For choosin the Frequence of the Color Sensor
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    analogWriteFrequency(1000); // Set PWM frequency to 1 kHz
}
//////////////////////////////////////////////////



// ======================== MOTOR CONTROL FUNCTION ======================== //
// motorNumber: 0 = Left, 1 = Right
// work: 0 = Stop, 1 = Move
// direction: 0 = Backward, 1 = Forward
// speed: PWM signal (Max 4095 for STM32 PWM)
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


// ======================== COLOR SENSOR FUNCTION ======================== //
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



// ======================== GREEN COLOR DETECTED FUNCTION ======================== //
void TheRotatingFunction(int greenValue,int redValue , int blueValue , int RightMostSensor , int RightLeastSensor , int LeftMostSensor , int LeftLeastSensor , int speed) {
        // Move Forward
        motor_control(1, 1, 1, 4000);
        motor_control(0, 1, 1, 4000);
        delay(600);

        // Rotate Right
        motor_control(1, 1, 1, 3000);
        motor_control(0, 0, 1, 3000);
        delay(1300);

        // Move Forward Again
        motor_control(1, 1, 1, 4000);
        motor_control(0, 1, 1, 4000);
        delay(1100);

        // Rotate Again
        motor_control(1, 1, 1, 3000);
        motor_control(0, 0, 1, 3000);
        delay(1300);

        // Move Forward Again
        motor_control(1, 1, 1, 4000);
        motor_control(0, 1, 1, 4000);
        delay(600);
////////////////////////

// After Moving , it goes to an infinite loop stoping the car , and it only exit that loop when all sensors are black ( That means the car is lift up in the air and put somewhere else)
          while(1){
            motor_control(1, 0, 1, speed);
            motor_control(0, 0, 1, speed);

            RightMostSensor = digitalRead(RIGHTMOSTSENSOR);
            RightLeastSensor = digitalRead(RIGHTSENSOR);
            LeftMostSensor = digitalRead(LEFTMOSTSENSOR);
            LeftLeastSensor = digitalRead(LEFTSENSOR);
    
            if(RightMostSensor && RightLeastSensor && LeftMostSensor && LeftLeastSensor){
              break;}
          }
}




// ======================== MAIN LOOP ======================== //
void loop() {

    // Reading the sensors Value
    int RightMostSensor = digitalRead(RIGHTMOSTSENSOR);
    int RightLeastSensor = digitalRead(RIGHTSENSOR);
    int LeftMostSensor = digitalRead(LEFTMOSTSENSOR);
    int LeftLeastSensor = digitalRead(LEFTSENSOR);
    
    int redValue = readColor('R');
    int greenValue = readColor('G');
    int blueValue = readColor('B');
    



    int speed = 2000; // Adjust speed (Max 4095 for STM32 PWM)

    
    // If Red is detected, stop for 5 seconds then resume
    if (redValue < greenValue && redValue < blueValue && redValue < 200) { // Confirm red detection
        motor_control(1, 0, 1, speed);
        motor_control(0, 0, 1, speed);
        delay(5000);

        motor_control(1, 1, 1, 3000);
        motor_control(0, 1, 1, 3000);
        delay(300);
        return;
    }
    
    // To Declate A count only once Begins from Zero
    static int GreenCount = 0;

    if(greenValue < redValue && greenValue < blueValue && greenValue < 200){
      GreenCount++ ;  // Increments the Count when counts a green 


      if(GreenCount == 1){  // It moves forward far from the green to avoid readin it again so quickly then returns to the track
        motor_control(1, 1, 1, 3000); 
        motor_control(0, 1, 1, 3000);
        delay(600);
      } 
      if(GreenCount == 2){  // When the Count reaches 2 it calles the function of the rotating then resets the count again to 0
        TheRotatingFunction(greenValue , redValue , blueValue , RightMostSensor , RightLeastSensor , LeftMostSensor , LeftLeastSensor , speed);
        GreenCount = 0; // Reset after calling the function

      // So It makes the rotation function every two loops on the track
      }

    }



//////////////////////////////////////////////////////////////////////



    // Movement Logic Based on IR Sensors
    if (RightMostSensor && RightLeastSensor && LeftMostSensor && LeftLeastSensor) {
          // All Black ( in the Air ) = Stop the motors
        motor_control(1, 0, 1, speed);
        motor_control(0, 0, 1, speed);
        delay(5000);
    } else if (!RightMostSensor && !RightLeastSensor && !LeftMostSensor && !LeftLeastSensor) {
          // All White = Move Forward
        motor_control(1, 1, 1, speed);
        motor_control(0, 1, 1, speed);
    } else if (RightLeastSensor) {
          // Right Least Black = Move Left Fast ( run one motor and the other is run in the other direction )
        motor_control(1, 1, 1, 2500);
        motor_control(0, 1, 0, 2500);
        delay(25);
    } else if (RightMostSensor) {
          // Right Most Black = Move Left slow
        motor_control(1, 1, 1, 2500);
        motor_control(0, 0, 1, 2500);
    } else if (LeftLeastSensor) {
           // Left Least Black = Move Right Fast ( run one motor and the other is run in the other direction )
        motor_control(1, 1, 0, 2500);
        motor_control(0, 1, 1, 2500);
        delay(25);
    } else if (LeftMostSensor) {
          // Left Most Black = Move Right slow
        motor_control(1, 0, 1, 2500);
        motor_control(0, 1, 1, 2500);
    }

}