#include <TimerOne.h>                                 // Header file for TimerOne library

#define trigPin1 3                                    // Pin 3 trigger output
#define trigPin2 5                                    // Pin 5 trigger output
#define echoPin1 4                                    // Pin 4 Echo input
#define echoPin2 6                                    // Pin 6 Echo input

#define TIMER_US 50                                   // 50 uS timer duration 
#define TICK_COUNTS 4000                              // 200 mS worth of timer ticks

volatile long echo1_start = 0;                        // Records start of echo pulse
volatile long echo2_start = 0; 
volatile long echo1_end = 0;                          // Records end of echo pulse
volatile long echo2_end = 0;
volatile long echo1_duration = 0;                     // Duration - difference between end and start
volatile long echo2_duration = 0;
volatile int trigger_time_count = 0;                  // Count down counter to trigger pulse time

void setup() 
{
  pinMode(trigPin1, OUTPUT);                           // Trigger pin set to output
  pinMode(trigPin2, OUTPUT);
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
    Serial.println(echo1_duration / 58);
    Serial.println(echo2_duration / 58);
    delay(10);
}


void timerIsr()
{
    trigger_pulse();                                 // Schedule the trigger pulses
}

void trigger_pulse()
{
      static volatile int state = 0;                 // State machine variable

      if (!(trigger_time_count--))                   // Count to 200mS
      {                                              // Time out - Initiate trigger pulse
         trigger_time_count = TICK_COUNTS;           // Reload
         state = 1;                                  // Changing to state 1 initiates a pulse
      }
    
      switch(state)                                  // State machine handles delivery of trigger pulse
      {
        case 0:                                      // Normal state does nothing
            break;
        
        case 1:                                      // Initiate pulse
           digitalWrite(trigPin1, HIGH);              // Set the trigger output high
           digitalWrite(trigPin2, HIGH);
           state = 2;                                // and set state to 2
           break;
        
        case 2:                                      // Complete the pulse
        default:      
           digitalWrite(trigPin1, LOW);               // Set the trigger output low
           digitalWrite(trigPin2, LOW);
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
      echo2_end = 0;                                 // Clear the end time
      echo2_start = micros();                        // Save the start time
      break;
      
    case LOW:                                       // Low so must be the end of hte echo pulse
      echo2_end = micros();                          // Save the end time
      echo2_duration = echo2_end - echo2_start;        // Calculate the pulse duration
      break;
  }
}

void echo_interrupt1()
{
  switch (digitalRead(echoPin1))                     // Test to see if the signal is high or low
  {
    case HIGH:                                      // High so must be the start of the echo pulse
      echo1_end = 0;                                 // Clear the end time
      echo1_start = micros();                        // Save the start time
      break;
      
    case LOW:                                       // Low so must be the end of hte echo pulse
      echo1_end = micros();                          // Save the end time
      echo1_duration = echo1_end - echo1_start;        // Calculate the pulse duration
      break;
  }
}

