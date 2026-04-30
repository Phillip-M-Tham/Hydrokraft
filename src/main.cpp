#include <Arduino.h>
#include <PID_v1.h>
#include <ESP32Servo.h> // Add to platformio.ini: lib_deps = madhephaestus/ESP32Servo

// --- PIN DEFINITIONS ---
const int flexPins[5] = {34, 35, 32, 33, 25}; 
const int pin35kg = 18;  
const int pin20kg = 19;  

// --- SERVO OBJECTS ---
Servo motor35kg;
Servo motor20kg;

// --- PID VARIABLES ---
double setpoint35, input35, output35;
// Kp is lower here because Servo output (0-180) is different from PWM (0-255)
double Kp35 = 2.0, Ki35 = 0.1, Kd35 = 0.1; 
PID pid35(&input35, &output35, &setpoint35, Kp35, Ki35, Kd35, DIRECT);

const int FINGER_NEUTRAL = 820; 

void setup() {
    Serial.begin(115200);

    // Attach Servos with standard pulse ranges (500us to 2400us)
    motor35kg.attach(pin35kg, 500, 2400);
    motor20kg.attach(pin20kg, 500, 2400);

    setpoint35 = FINGER_NEUTRAL;
    pid35.SetMode(AUTOMATIC);
    
    // SERVO LIMITS: 0 to 180 degrees
    pid35.SetOutputLimits(0, 180); 

    Serial.println("Servo PID Control Initialized.");
}

void loop() {
    input35 = (analogRead(flexPins[1]) + analogRead(flexPins[2]) + 
               analogRead(flexPins[3]) + analogRead(flexPins[4])) / 4.0;

    pid35.Compute();

    // The PID 'output35' is now a Degree (0-180)
    motor35kg.write(output35); 

    static long lastTime = 0;
    if (millis() - lastTime > 100) {
        Serial.print("Flex:"); Serial.print(input35);
        Serial.print(" | Target:"); Serial.print(setpoint35);
        Serial.print(" | Angle:"); Serial.println(output35);
        lastTime = millis();
    }
}