#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <Arduino.h>
#include <SPI.h>
#include <mcp_can.h>

#define CAN0_INT 2
MCP_CAN CAN0(9);

// Define the constants for wheel and sensor diameters, wheel radius, and wheel circumference
#define WheelDiameter 0.065 
#define SensorDiameter 0.025 
#define WheelRadius 0.0325
#define WheelCircumference (2 * PI * WheelRadius) 

float RPM_w = 0;
const byte PulsesPerRevolution = 20;

const unsigned long ZeroTimeout = 100000; //Microseconds
const byte numReadings = 2;

// Declare the global variables
volatile unsigned long LastTimeWeMeasured;
volatile unsigned long PeriodBetweenPulses = ZeroTimeout + 1000;
volatile unsigned long PeriodAverage = ZeroTimeout + 1000;
unsigned long FrequencyRaw;
unsigned long FrequencyReal;
unsigned long RPM;
unsigned int PulseCounter = 1;
unsigned long PeriodSum;

unsigned long LastTimeCycleMeasure = LastTimeWeMeasured;
unsigned long CurrentMicros = micros();
unsigned int AmountOfReadings = 1;
unsigned int ZeroDebouncingExtra;
unsigned long readings[numReadings];
unsigned long readIndex;  
unsigned long total; 
unsigned long average;

void setup() {
  // Initialize serial communication and attach interrupt
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(3), Pulse_Event, RISING);
  delay(1000);

  // Initialize CAN bus shield
  while (CAN_OK != CAN0.begin(CAN_500KBPS)) {
    Serial.println("CAN BUS Shield init fail");
    Serial.println("Init CAN BUS Shield again");
    delay(100);
  }
  Serial.println("CAN BUS Shield init ok!");
}

void loop() {
  // Update the time variables
  LastTimeCycleMeasure = LastTimeWeMeasured;
  CurrentMicros = micros();
  if (CurrentMicros < LastTimeCycleMeasure) {
    LastTimeCycleMeasure = CurrentMicros;
  }
  
  // Change PeriodAverage from seconds to microseconds
  FrequencyRaw = 10000000000 / PeriodAverage;

  // If the period between pulses is greater than the timeout, set the frequency to 0
  if (PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra) {
    FrequencyRaw = 0;
    ZeroDebouncingExtra = 2000;
  } else {
    ZeroDebouncingExtra = 0;
  }

  // Calculate the real frequency, RPM, and speed
  FrequencyReal = FrequencyRaw / 10000;
  RPM = FrequencyRaw / PulsesPerRevolution * 60;
  RPM = RPM / 10000;
  total = total - readings[readIndex];
  readings[readIndex] = RPM;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  // Reset the readIndex if it is greater than or equal to the number of readings
  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  // Calculate the average RPM and the speed in meters per second
  average = total / numReadings;
  RPM_w = RPM * (SensorDiameter / WheelDiameter);
  double speed_mps = (RPM_w * WheelCircumference) / 60;

  // Print the RPM and speed to the serial monitor
  Serial.print("\tRPM: ");
  Serial.print(RPM_w);
  Serial.print("\tSpeed: ");
  Serial.println(speed_mps);

  // Send the RPM and speed over the CAN bus
  byte rpmData[8];
  float tempRPM = RPM_w;
  float tempSpeed = speed_mps;
  memcpy(rpmData, &tempRPM, 4);
  memcpy(rpmData + 4, &tempSpeed, 4);
  CAN0.sendMsgBuf(0x100, 0, 8, rpmData);
}

void Pulse_Event() {
  // Calculate the period between pulses and update the LastTimeWeMeasured variable
  PeriodBetweenPulses = micros() - LastTimeWeMeasured;
  LastTimeWeMeasured = micros();

  // If the pulse counter is greater than or equal to the amount of readings, calculate the average period
  if (PulseCounter >= AmountOfReadings)  {
    PeriodAverage = PeriodSum / AmountOfReadings;
    PulseCounter = 1;
    PeriodSum = PeriodBetweenPulses;

    // Remap the amount of readings based on the period between pulses
    int RemapedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);
    RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);
    AmountOfReadings = RemapedAmountOfReadings;
  } else {
    PulseCounter++;
    PeriodSum = PeriodSum + PeriodBetweenPulses;
  }
}

