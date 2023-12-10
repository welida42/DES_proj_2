// Libraries for LCD display (currently commented out)
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd (0x27, 16, 2);

// Constants and settings for RPM calculations
const byte PulsesPerRevolution = 2;
const unsigned long ZeroTimeout = 100000;  // Timeout to reset RPM
const byte numReadings = 2;  // Number of readings for averaging

// Volatile variables for interrupt handler
volatile unsigned long LastTimeWeMeasured;
volatile unsigned long PeriodBetweenPulses = ZeroTimeout + 1000;
volatile unsigned long PeriodAverage = ZeroTimeout + 1000;

// Variables to store calculated RPM and intermediate values
unsigned long FrequencyRaw;
unsigned long FrequencyReal;
unsigned long RPM;
unsigned int PulseCounter = 1;
unsigned long PeriodSum;

// Variables for averaging multiple RPM readings
unsigned long LastTimeCycleMeasure = LastTimeWeMeasured;
unsigned long CurrentMicros = micros();
unsigned int AmountOfReadings = 1;
unsigned int ZeroDebouncingExtra;
unsigned long readings[numReadings];
unsigned long readIndex;
unsigned long total;
unsigned long average;

// Ultrasonic sensor settings
const unsigned int TRIG_PIN = 13;
const unsigned int ECHO_PIN = 12;
const unsigned int BAUD_RATE = 9600;

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // Initialize LCD display (currently commented out)
  //lcd.init();
  //lcd.backlight();
  // Attach interrupt to detect RPM pulses
  attachInterrupt(digitalPinToInterrupt(2), Pulse_Event, RISING);
  delay(1000);
}

void loop() {
  // Ultrasonic Sensor Code
  // Send trigger pulse to ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Receive echo pulse from ultrasonic sensor
  const unsigned long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration / 29 / 2;
  if (duration == 0) {
    Serial.println("Warning: no pulse from sensor");
  } else {
    Serial.print("Distance to nearest object: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  // RPM Calculation Code
  LastTimeCycleMeasure = LastTimeWeMeasured;
  CurrentMicros = micros();
  if (CurrentMicros < LastTimeCycleMeasure) {
    LastTimeCycleMeasure = CurrentMicros;
  }
  FrequencyRaw = 10000000000 / PeriodAverage;
  // Reset RPM if no pulse is detected within the timeout
  if (PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra) {
    FrequencyRaw = 0;  // Set frequency as 0.
    ZeroDebouncingExtra = 2000;
  } else {
    ZeroDebouncingExtra = 0;
  }
  FrequencyReal = FrequencyRaw / 10000;

  // Convert frequency to RPM
  RPM = FrequencyRaw / PulsesPerRevolution * 60;
  RPM = RPM / 10000;

  // Calculate average RPM over multiple readings
  total = total - readings[readIndex];
  readings[readIndex] = RPM;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings;

  // Print the calculated values
  Serial.print("Period: ");
  Serial.print(PeriodBetweenPulses);
  Serial.print("\tReadings: ");
  Serial.print(AmountOfReadings);
  Serial.print("\tFrequency: ");
  Serial.print(FrequencyReal);
  Serial.print("\tRPM: ");
  Serial.print(RPM);
  Serial.print("\tTachometer: ");
  Serial.println(average);
  // Print RPM to LCD display (currently commented out)
  //lcd.setCursor(0, 0);
  //lcd.print("RPM : ");
  //lcd.print(RPM);
  //lcd.print("   ");
  delay(5000);
}

// Interrupt service routine to handle detected pulses for RPM calculations
void Pulse_Event() {
  PeriodBetweenPulses = micros() - LastTimeWeMeasured;
  LastTimeWeMeasured = micros();
  if (PulseCounter >= AmountOfReadings) {
    PeriodAverage = PeriodSum / AmountOfReadings;
    PulseCounter = 1;
    PeriodSum = PeriodBetweenPulses;
    int RemapedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);
    RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);
    AmountOfReadings = RemapedAmountOfReadings;
  } else {
    PulseCounter++;
    PeriodSum = PeriodSum + PeriodBetweenPulses;
  }
}

