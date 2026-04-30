#include <Arduino.h>
#include <PID_v1.h>


// --- PIN DEFINITIONS ---
const int flexPins[5] = {34, 35, 32, 33, 25}; // S1 (Thumb), S2-S5 (Fingers)
const int motor35kg = 18;  // Signal for 35kg motor
const int motor20kg = 19;  // Signal for 20kg motor

// --- PID 1: 35kg Motor (Main Fingers) ---
double setpoint35, input35, output35;
double Kp35 = 60.0, Ki35 = 0.5, Kd35 = 30.0; 
PID pid35(&input35, &output35, &setpoint35, Kp35, Ki35, Kd35, REVERSE);

// --- PID 2: 20kg Motor (Thumb) ---
double setpoint20, input20, output20;
double Kp20 = 3.0, Ki20 = 0.5, Kd20 = 0.1; 
PID pid20(&input20, &output20, &setpoint20, Kp20, Ki20, Kd20, DIRECT);

// --- NEUTRAL TARGETS (From your HandSensors1.csv data) ---
const int FINGER_NEUTRAL = 820; // Average for S2-S5
const int THUMB_NEUTRAL  = 770; // Value for S1

void setup() {
    Serial.begin(115200);

    // Standard PWM setup for ESP32
    pinMode(motor35kg, OUTPUT);
    pinMode(motor20kg, OUTPUT);

    // Initialize 35kg PID
    setpoint35 = FINGER_NEUTRAL;
    pid35.SetMode(AUTOMATIC);
    pid35.SetOutputLimits(0, 255); 

    // Initialize 20kg PID
    setpoint20 = THUMB_NEUTRAL;
    pid20.SetMode(AUTOMATIC);
    pid20.SetOutputLimits(0, 255);

    Serial.println("Dual Motor Control: Maintain Neutral Position Active");
}

void loop() {
    // 1. DATA COLLECTION
    // Input for 35kg: Average of Index, Middle, Ring, Pinky (S2-S5)
    input35 = (analogRead(flexPins[1]) + analogRead(flexPins[2]) + 
               analogRead(flexPins[3]) + analogRead(flexPins[4])) / 4.0;

    // Input for 20kg: Thumb Flex (S1)
    input20 = analogRead(flexPins[0]);

    // 2. COMPUTE PIDs
    pid35.Compute();
    pid20.Compute();

    // 3. MOTOR EXECUTION (Individual PWM signals)
    analogWrite(motor35kg, output35);
    analogWrite(motor20kg, output20);

    // 4. MONITORING
    static long lastTime = 0;
    if (millis() - lastTime > 100) {
        Serial.print("FingersFlex:"); Serial.print(input35);
        Serial.print(" | 35kg_PWM:"); Serial.print(output35);
        Serial.print(" || ThumbFlex:"); Serial.print(input20);
        Serial.print(" | 20kg_PWM:"); Serial.println(output20);
        lastTime = millis();
    }
}