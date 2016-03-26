/**
 * Author: Trevor Yu
 * Email: trevor.tyu@gmail.com
 * Date: March 25, 2016
 * Arduino code to compute the RPMs of 2 Hall Effect gear tooth sensors
 * Built for the Arduino Mega 2560
 * Code primarily based off of: http://playground.arduino.cc/Main/ReadingRPM
 */
// General constants and variables
const int CHECK_INTERVAL = 500; // 500 milliseconds
const double CHECKS_PER_SECOND = 1000 / CHECK_INTERVAL; // The number of checks we perform per second
const int SECONDS_PER_MINUTE = 60;
const double ACCEPTABLE_RPM_PERCENTAGE_DIFF = 0.05;
unsigned long lastmillis = 0;

// Variables and constants for the engine sensor
const int ENGINE_GEAR_TEETH = 1;  // Number of teeth on the engine's gear
const double ENGINE_FREQ = CHECKS_PER_SECOND * SECONDS_PER_MINUTE / ENGINE_GEAR_TEETH;
const int engineSensorPin = 2;
volatile int engineGearCount = 0;
double engineRpm = 0;

// Variables and constants for the wheel sensor
const int WHEEL_GEAR_TEETH = 24; // Number of teeth on the wheel's gear
const double WHEEL_FREQ = CHECKS_PER_SECOND * SECONDS_PER_MINUTE / WHEEL_GEAR_TEETH;
const int wheelSensorPin = 3;
volatile int wheelGearCount = 0;
double wheelRpm = 0;

// Variables and constants for the clutch pin
const int clutchPin = 18;
volatile int clutchState = LOW;

// Output pin for indicating when the engine and wheel RPM values are within a
// certain percentage of each other
const int outputPin = 22;

/**
 * The logic to setup all the input and output pins
 */
void setup() {
  Serial.begin(9600);
  
  // Set the pin modes of the pins
  pinMode(engineSensorPin, INPUT);
  pinMode(wheelSensorPin, INPUT);
  pinMode(clutchPin, INPUT);
  pinMode(outputPin, OUTPUT);

  // Attach the interrupts
  attachInterrupt(digitalPinToInterrupt(clutchPin), clutchRead, CHANGE);
  attachSensorInterrupts();
}

/**
 * Main loop that repeatedly runs the logic to check RPMs
 */
void loop() {
  // Run the following logic only if the clutch is HIGH
  if (clutchState == HIGH) {
    Serial.println("Clutch is high");
    // Check every 0.5 seconds if the measured RPMs are within 5% of each other
    if (millis() - lastmillis >= CHECK_INTERVAL) {
      Serial.println("500 ms has passed");
      // Disable interrupts while performing check
      detachInterrupt(engineSensorPin);
      detachInterrupt(wheelSensorPin);

      // Convert frequencies to RPM
      engineRpm = engineGearCount * ENGINE_FREQ;
      wheelRpm = wheelGearCount * WHEEL_FREQ;

      // If engine RPM is within 5% of wheel RPM, set the output pin to HIGH
      if( abs(engineRpm - wheelRpm) < ACCEPTABLE_RPM_PERCENTAGE_DIFF ) {
        Serial.println("RPMs are within acceptable percentage");
        //digitalWrite(outputPin, HIGH);
        PORTA |= _BV(PA0);

        while (clutchState == HIGH){
          // Block until clutch is set to LOW
        }

        Serial.println("Out of while loop");

        // Once clutch is set back to LOW, we set the output pin to LOW
        //digitalWrite(outputPin, LOW);
        PORTA &= ~_BV(PA0);
      }

      // Reset the variables
      lastmillis = millis();
      engineRpm = 0;
      wheelRpm = 0;

      // Reenable sensor interrupts
      attachSensorInterrupts();
    }
  }
}

/**
 * Function to attach the interrupts
 */
void attachSensorInterrupts() {
  attachInterrupt(digitalPinToInterrupt(engineSensorPin), engineSensorRead, FALLING);
  attachInterrupt(digitalPinToInterrupt(wheelSensorPin), wheelSensorRead, FALLING);
}

/**
 * Interrupt that increments a count of the number of teeth that have passed the sensor
 */
void engineSensorRead() {
  engineGearCount++;
}

/**
 * Interrupt that increments a count of the number of teeth that have passed the sensor
 */
void wheelSensorRead() {
  wheelGearCount++;
}

/**
 * Interrupt to read the current state of the clutch pin
 */
void clutchRead() {
  //clutchState = digitalRead(clutchPin);
  clutchState ^= _BV(PD3) >> 3;
  Serial.print("Clutch state changed: ");
  Serial.print(clutchState);
  Serial.print("\r\n");
}
