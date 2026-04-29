#include <Arduino.h>
#include <PID_v1.h>

// --- PIN DEFINITIONS ---
const int flexPins[5] = {34, 35, 32, 33, 25}; 
const int motorPWM    = 18;  
const int motorDir    = 19;  

// --- PID VARIABLES ---
double Setpoint, Input, Output;
// For position control, we need a higher Kp to overcome the glove's resistance
double Kp = 3.5, Ki = 1.2, Kd = 0.5; 
// Change DIRECT to REVERSE if the motor pulls when it should release
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE); 

// --- TARGETS (Based on your HandSensors1.csv) ---
// We will use the average of the 4 fingers on the Whiffletree
const int NEUTRAL_TARGET = 820; 

void setup() {
    Serial.begin(115200);
    
    pinMode(motorPWM, OUTPUT);
    pinMode(motorDir, OUTPUT);
    digitalWrite(motorDir, HIGH); 

    // Initialize PID
    Setpoint = NEUTRAL_TARGET; 
    myPID.SetMode(AUTOMATIC);
    myPID.SetOutputLimits(0, 255); 
}

void loop() {
    // 1. Read Flex Sensors (S2 to S5 are the fingers on the whiffletree)
    float avgFlex = (analogRead(flexPins[1]) + analogRead(flexPins[2]) + 
                     analogRead(flexPins[3]) + analogRead(flexPins[4])) / 4.0;
    
    Input = avgFlex;

    // 2. Compute PID
    myPID.Compute();

    // 3. Motor Execution
    // If Input < Setpoint (Hand is flexing), Output increases to pull it back.
    analogWrite(motorPWM, Output);

    // 4. Monitoring
    static long lastTime = 0;
    if (millis() - lastTime > 100) {
        Serial.print("CurrentPos:"); Serial.print(Input);
        Serial.print(",");
        Serial.print("Target:"); Serial.print(Setpoint);
        Serial.print(",");
        Serial.print("MotorPower:"); Serial.println(Output);
        lastTime = millis();
    }
}