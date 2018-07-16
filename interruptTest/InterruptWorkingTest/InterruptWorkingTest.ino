#include <TimerOne.h>                                 // Header file for TimerOne library

#define trigPin1 3                                    // Pin 3 trigger output
#define trigPin2 5                                    // Pin 5 trigger output
#define trigPin3 7                                    // Pin 7 trigger output
#define trigPin4 9                                    // Pin 9 trigger output
#define echoPin1 8                                    // Pin 4 Echo input
#define echoPin2 10                                    // Pin 6 Echo input

#define TIMER_US 50                                   // 50 uS timer duration 
#define TICK_COUNTS 8000                              

volatile long echo11_start = 0;                        // Records start of echo pulse
volatile long echo21_start = 0;
volatile long echo12_start = 0;
volatile long echo22_start = 0; 
volatile long echo11_end = 0;                          // Records end of echo pulse
volatile long echo21_end = 0;
volatile long echo12_end = 0;
volatile long echo22_end = 0;
volatile long echo11_duration = 0;                     // Duration - difference between end and start
volatile long echo21_duration = 0;
volatile long echo12_duration = 0;
volatile long echo22_duration = 0;
volatile int trigger_time_count = 0;                  // Count down counter to trigger pulse time

void setup() 
{
  pinMode(trigPin1, OUTPUT);                           // Trigger pin set to output
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin1, INPUT);                            // Echo pin set to input
  pinMode(echoPin2, INPUT);
  Timer1.initialize(TIMER_US);                        // Initialise timer 1
  Timer1.attachInterrupt( timerIsr );                 // Attach interrupt to the timer service routine 
  attachInterrupt(echoPin1, echo_interrupt1, CHANGE);  // Attach interrupt to the sensor echo input
  attachInterrupt(echoPin2, echo_interrupt2, CHANGE);
  Serial.begin (9600);                                // Initialise the serial monitor output
}

void loop()
{
    Serial.print("Distance 1:");
    Serial.println(echo11_duration / 58);
    Serial.print("Distance 2:");
    Serial.println(echo21_duration / 58);
    Serial.print("Distance 3:");
    Serial.println(echo12_duration / 58);
    Serial.print("Distance 4:");
    Serial.println(echo22_duration / 58);
    delay(100);
}


void timerIsr()
{
    trigger_pulse();                                 // Schedule the trigger pulses
}

void trigger_pulse()
{
      static volatile int state = 0;                 // State machine variable
      if(!(trigger_time_count))
      {
        state = 1;
        trigger_time_count = TICK_COUNTS;
      }
      if(trigger_time_count == TICK_COUNTS/2)                   // Count to 200mS
      {                                              // Time out - Initiate trigger pulse
         state = 2;                                  // Changing to state 1 initiates a pulse
      }
      trigger_time_count--;
      switch(state)                                  // State machine handles delivery of trigger pulse
      {
        case 0:                                      // Normal state does nothing
            break;
        
        case 1:                                      // Initiate pulse
           digitalWrite(trigPin1, HIGH);              // Set the trigger output high
           digitalWrite(trigPin3, HIGH);
           digitalWrite(trigPin4, HIGH);
           state = 3;                                // and set state to 2
           break;
        case 2:
           digitalWrite(trigPin2, HIGH);
           digitalWrite(trigPin3, HIGH);
           digitalWrite(trigPin4, HIGH);
           state = 3;
           break;
        case 3:
        default:      
           digitalWrite(trigPin1, LOW);
           digitalWrite(trigPin2, LOW);
           digitalWrite(trigPin3, LOW);
           digitalWrite(trigPin4, LOW);
           state = 0;                                // and return state to normal 0
           break;
     }
}

// --------------------------
// echo_interrupt() External interrupt from HC-SR04 echo signal. 
// Called every time the echo signal changes state.
//
// Note: this routine does not handle the case where the timer
//       counter overflows which will result in the occassional error.

void echo_interrupt2()
{
  switch (digitalRead(echoPin2))                     // Test to see if the signal is high or low
  {
    case HIGH:                                      // High so must be the start of the echo pulse
      if(trigger_time_count > TICK_COUNTS/2)
      {
        echo21_end = 0;
        echo21_start = micros();
      }
      else
      {  
        echo22_end = 0;                                 // Clear the end time
        echo22_start = micros();                        // Save the start time
      }
      break;
      
    case LOW:                                       // Low so must be the end of hte echo pulse
      if(trigger_time_count > TICK_COUNTS/2)
      { 
        echo21_end = micros();                          // Save the end time
        echo21_duration = echo21_end - echo21_start;        // Calculate the pulse duration
      }
      else
      {
        echo22_end = micros();
        echo22_duration = echo22_end - echo22_start;
      }
      break;
  }
}

void echo_interrupt1()
{
  switch (digitalRead(echoPin1))                     // Test to see if the signal is high or low
  {
    case HIGH:                                      // High so must be the start of the echo pulse
      if(trigger_time_count > TICK_COUNTS/2)
      {
        echo11_end = 0;
        echo11_start = micros();
      }
      else
      {  
        echo12_end = 0;                                 // Clear the end time
        echo12_start = micros();                        // Save the start time
      }
      break;
      
    case LOW:                                       // Low so must be the end of hte echo pulse
      if(trigger_time_count > TICK_COUNTS/2)
      { 
        echo11_end = micros();                          // Save the end time
        echo11_duration = echo11_end - echo11_start;        // Calculate the pulse duration
      }
      else
      {
        echo12_end = micros();
        echo12_duration = echo12_end - echo12_start;
      }
      break;
  }
}

