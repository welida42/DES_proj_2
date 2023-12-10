#include <Arduino.h>
#include <SPI.h>

// Define the pin for the speed sensor and its associated interrupt
#define SPEED_SENSOR_PIN 3
#define SPEED_SENSOR_INTERRUPT 1 // Uno, Nano, Mini, other 328-based boards: 0=pin2, 1=pin3

// Volatile variables used in interrupt context
volatile unsigned long count = 0;  // Count of pulses detected
volatile unsigned long lastPulseTime = 0;  // Timestamp of the last detected pulse

// Constants for calculating RPM and speed
#define PPR 20  // Pulses per revolution for the sensor wheel
#define WheelDiameter 0.065 // Diameter of the wheel in meters
#define SensorDiameter 0.025 // Diameter of the sensor wheel in meters

// Calculate the wheel's circumference
#define WheelRadius 0.0325 // Wheel's radius in meters
#define WheelCircumference (2 * PI * WheelRadius) // Circumference of the wheel in meters

unsigned long prevTime = 0;  // Previous time when RPM and speed were calculated
unsigned long RPM_w = 0;  // RPM of the wheel

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  pinMode(SPEED_SENSOR_PIN, INPUT_PULLUP);  // Set the speed sensor pin as input with internal pull-up resistor
  attachInterrupt(SPEED_SENSOR_INTERRUPT, pulseHandler, RISING);  // Attach an interrupt to detect RISING pulses
}

void loop() {
  // If no pulse has been detected for 2 seconds, set the wheel RPM to 0
  if (millis() - lastPulseTime >= 2000) {
    RPM_w = 0;
  }

  // Calculate and print RPM and speed every 0.5 second
  if (millis() - prevTime >= 500) {
    if (count >= PPR) {
      unsigned long deltaTime = millis() - lastPulseTime;  // Time difference between current time and last detected pulse
      float rotationRate = (float)count / PPR;  // Number of full rotations of the sensor wheel
      unsigned long RPM_sensor = (60000.0 / deltaTime) * rotationRate;  // Calculate RPM of the sensor wheel

      RPM_w = RPM_sensor * (SensorDiameter / WheelDiameter);  // Convert sensor RPM to wheel RPM
      count = 0;  // Reset pulse count
      lastPulseTime = millis();  // Update the last pulse time
    }

    // Calculate speed in meters per second
    double speed_mps = (RPM_w * WheelCircumference) / 60;

    // Print the calculated RPM and speed
    Serial.print("RPM_w: ");
    Serial.println(RPM_w);
    Serial.print("Speed (m/s): ");
    Serial.println(speed_mps, 1);

    prevTime = millis();  // Update the previous time
  }
}

// Interrupt service routine to handle detected pulses
void pulseHandler() {
  count++;  // Increment the pulse count
  lastPulseTime = millis();  // Update the timestamp of the last detected pulse
}

