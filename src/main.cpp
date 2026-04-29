#include <Arduino.h>
#include <ESP32Servo.h>

Servo servo20; //thumb
Servo servo35; //4 fingers

const int servo20Pin = 18;
const int servo35Pin = 19;
// Your specific 10 pins in order
const int sensorPins[] = {36, 39, 34, 35, 32, 33, 25, 26, 27, 14};
const int numSensors = 10;



//Angles
float homeAngle =90.0;
float leftEnd = 20.0; //this should be adjusted to 0
float rightEnd = 160.0;

//PD gains
float kProp = 2.0;
float kDer = 0.3;

//State
float currentLeft = homeAngle;
float currentRight= homeAngle;

float targetLeft= homeAngle;
float targetRight = homeAngle;

float lastErrorLeft =0.0;
float lastErrorRight = 0.0;

unsigned long lastTime=0;

//StateMachine (replaces delay)
int state=0;
unsigned long lastSwitch=0;


void setup() {
  Serial.begin(115200);
  Serial.println("S1,S2,S3,S4,S5,S6,S7,S8,S9,S10");
  delay(1000);

  servo20.setPeriodHertz(50);
  servo35.setPeriodHertz(50);

  servo20.attach(servo20Pin, 500, 2500);
  servo35.attach(servo35Pin, 500, 2500);

  servo20.write(90);
  servo35.write(90);
  //Serial.println("Both servos centered");

  //Init sensors   
  // Initialize all pins as inputs
  for (int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  
  //Serial.println("Reading all 10 sensors... (36, 39, 34, 35, 32, 33, 25, 26, 27, 14)");
  delay(1000);
}

void loop() {

  for (int i = 0; i < numSensors; i++) {
    int val = analogRead(sensorPins[i]);
    
    // Print the value
    Serial.print(val);
    
    // Add a tab or comma between numbers for readability
    if (i < numSensors - 1) {
      Serial.print(","); // Using tabs makes columns line up in Serial Monitor
    }
  }
  
  Serial.println(); // Start a new line for the next set of readings
  delay(50);        // Fast enough for real-time feel, slow enough to read

}