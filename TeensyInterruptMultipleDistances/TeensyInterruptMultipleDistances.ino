#include <TimerOne.h>                                 // Header file for TimerOne library

#define trigTranTop 9                                 // trigger for transmitterTop
#define trigTranHTop 10                                // trigger for transmitterHalfTop
#define trigTranHBot 11                               // trigger for transmitterHalfBottom
#define trigTranBot 12                                // trigger for transmitterBottom
#define trigRecTop 0                                  // trigger for recieverTop
#define trigRecHTop 2                                 // trigger for recieverHalfTop  
#define trigRecHBot 4                                // trigger for recieverHalfBottom
#define trigRecBot 6                                 // trigger for recieverBottom
#define echoRecTop 1                                  // echoPin for recieverTop
#define echoRecHTop 3                                // echoPin for recieverHalfTop
#define echoRecHBot 5                                // echoPin for recieverHalfBottom
#define echoRecBot 7                                 // echoPin for recieverBottom

#define TIMER_US 20                                   // 10 uS timer duration 
#define TICK_COUNTS 6000                              

volatile long echoTopBot_start = 0;                        // Records start of echo pulse
volatile long echoTopHBot_start = 0;
volatile long echoTopHTop_start = 0;
volatile long echoTopTop_start = 0; 
volatile long echoHTopBot_start = 0;
volatile long echoHTopHBot_start = 0;
volatile long echoHTopHTop_start = 0;
volatile long echoHTopTop_start = 0;
volatile long echoHBotBot_start = 0;
volatile long echoHBotHBot_start = 0;
volatile long echoHBotHTop_start = 0;
volatile long echoHBotTop_start = 0;
volatile long echoBotBot_start = 0;
volatile long echoBotHBot_start = 0;
volatile long echoBotHTop_start = 0;
volatile long echoBotTop_start = 0;
volatile long echoTopBot_end = 0;   
volatile long echoTopHBot_end = 0;
volatile long echoTopHTop_end = 0;
volatile long echoTopTop_end = 0; 
volatile long echoHTopBot_end = 0;
volatile long echoHTopHBot_end = 0;
volatile long echoHTopHTop_end = 0;
volatile long echoHTopTop_end = 0;
volatile long echoHBotBot_end = 0;
volatile long echoHBotHBot_end = 0;
volatile long echoHBotHTop_end = 0;
volatile long echoHBotTop_end = 0;
volatile long echoBotBot_end = 0;
volatile long echoBotHBot_end = 0;
volatile long echoBotHTop_end = 0;
volatile long echoBotTop_end = 0;
volatile long echoTopBot_duration = 0;                        // Records start of echo pulse
volatile long echoTopHBot_duration = 0;
volatile long echoTopHTop_duration = 0;
volatile long echoTopTop_duration = 0; 
volatile long echoHTopBot_duration = 0;
volatile long echoHTopHBot_duration = 0;
volatile long echoHTopHTop_duration = 0;
volatile long echoHTopTop_duration = 0;
volatile long echoHBotBot_duration = 0;
volatile long echoHBotHBot_duration = 0;
volatile long echoHBotHTop_duration = 0;
volatile long echoHBotTop_duration = 0;
volatile long echoBotBot_duration = 0;
volatile long echoBotHBot_duration = 0;
volatile long echoBotHTop_duration = 0;
volatile long echoBotTop_duration = 0;
volatile int trigger_time_count = 0;                     // Count down counter to trigger pulse time
static volatile int state = 0;                           // State machine variable
float Lateralangle;
float rotAngle;
float CalculateAngles(float,float,float,float,float,float);
float cosC, cosB;
float distance1best, distance2best, distance3best;

void setup() 
{
  pinMode(trigTranTop, OUTPUT);                           // Trigger pin set to output
  pinMode(trigTranHTop, OUTPUT);
  pinMode(trigTranHBot, OUTPUT);
  pinMode(trigTranBot, OUTPUT);
  pinMode(trigRecTop, OUTPUT);
  pinMode(trigRecHTop, OUTPUT);
  pinMode(trigRecHBot, OUTPUT);
  pinMode(trigRecBot, OUTPUT);
  pinMode(echoRecTop, INPUT);
  pinMode(echoRecHTop, INPUT);
  pinMode(echoRecHBot, INPUT);
  pinMode(echoRecBot, INPUT);
  Timer1.initialize(TIMER_US);                        // Initialise timer 1
  Timer1.attachInterrupt( timerIsr );                 // Attach interrupt to the timer service routine 
  attachInterrupt(echoRecTop, echo_interruptRecTop, CHANGE);
  attachInterrupt(echoRecHTop, echo_interruptRecHTop, CHANGE);
  attachInterrupt(echoRecHBot, echo_interruptRecHBot, CHANGE);
  attachInterrupt(echoRecBot, echo_interruptRecBot, CHANGE);
  Serial.begin (9600);                                // Initialise the serial monitor output
}

void loop()
{
    
    /*Lateralangle = giveLateralAngle(echo11_duration/58, echo21_duration/58, 5);
    rotAngle = CalculateAngles(10, 10, echo11_duration/58, echo22_duration/58, echo12_duration/58, echo21_duration/58);
    cosC = getcosC(echo11_duration/58, echo21_duration/58, 5);
    cosB = getcosB(echo11_duration/58, echo21_duration/58, 5);*/
    Serial.print((float)echoTopTop_duration * 0.0343);
    Serial.print(", ");
    Serial.print((float)echoTopHTop_duration * 0.0343);
    Serial.print(", ");
    Serial.print((float)echoTopHBot_duration * 0.0343);
    Serial.print(", ");
    Serial.print((float)echoTopBot_duration * 0.0343);
    Serial.print(", ;");
    Serial.print((float)echoHTopTop_duration * 0.0343);
    Serial.print(",");
    Serial.print((float)echoHTopHTop_duration * 0.0343);
    Serial.print(",");
    Serial.print((float)echoHTopHBot_duration * 0.0343);
    Serial.print(",");
    Serial.print((float)echoHTopBot_duration * 0.0343);
    Serial.print(", ;");
    Serial.print((float)echoHBotTop_duration * 0.0343);
    Serial.print(",");
    Serial.print((float)echoHBotHTop_duration * 0.0343);
    Serial.print(",");
    Serial.print((float)echoHBotHBot_duration * 0.0343);
    Serial.print(",");
    Serial.print((float)echoHBotBot_duration * 0.0343);
    Serial.print(", ;");
    Serial.print((float)echoBotTop_duration * 0.0343);
    Serial.print(",");
    Serial.print((float)echoBotHTop_duration * 0.0343);
    Serial.print(",");
    Serial.print((float)echoBotHBot_duration * 0.0343);
    Serial.print(",");
    Serial.print((float)echoBotBot_duration * 0.0343);
    Serial.print(", ;");
    delay(100);
}



float giveLateralAngle(float distance1, float distance2, float distance3)
{
  float latangle = 0;
  float s = (distance1 + distance2 + distance3)/2;
  float h = 2*sqrt(s*(s-distance1)*(s-distance2)*(s-distance3))/distance3;
  latangle = map(h, 2, 35, 0, 45); 
  return latangle;
}

float getcosC(float distance1, float distance2, float distance3)
{
  return (pow(distance1, 2) + pow(distance2, 2) - pow(distance3,2)/(2*distance1*distance2));
}

float getcosB(float distance1, float distance2, float distance3)
{
  return (pow(distance2, 2) + pow(distance3, 2) - pow(distance1,2)/(2*distance3*distance2));
}

void timerIsr()
{
    trigger_pulse();                                 // Schedule the trigger pulses
}

void trigger_pulse()
{
  if(!(trigger_time_count))
      {
        state = 1;
        trigger_time_count = TICK_COUNTS;
      }
      if(trigger_time_count == TICK_COUNTS*3/4)                   // Count to 200mS
      {                                              // Time out - Initiate trigger pulse
         state = 2;                                  // Changing to state 1 initiates a pulse
      }
      if(trigger_time_count == TICK_COUNTS/2)
      {
        state = 3;
      }
      if(trigger_time_count == TICK_COUNTS/4)
      {
        state = 4;
      }
      trigger_time_count--;
      switch(state)                                  // State machine handles delivery of trigger pulse
      {
        case 0:                                      // Normal state does nothing
            break;
        
        case 1:                                      // Initiate pulse
           digitalWrite(trigTranTop, HIGH);          // Set the trigger output high
           digitalWrite(trigRecTop, HIGH);
           digitalWrite(trigRecHTop, HIGH);
           digitalWrite(trigRecHBot, HIGH);
           digitalWrite(trigRecBot, HIGH);
           state = 5;                                // and set state to 2
           break;
        case 2:
           digitalWrite(trigTranHTop, HIGH);             // Set the trigger output high
           digitalWrite(trigRecTop, HIGH);
           digitalWrite(trigRecHTop, HIGH);
           digitalWrite(trigRecHBot, HIGH);
           digitalWrite(trigRecBot, HIGH);
           state = 5;
           break;
        case 3:
           digitalWrite(trigTranHBot, HIGH);             // Set the trigger output high
           digitalWrite(trigRecTop, HIGH);
           digitalWrite(trigRecHTop, HIGH);
           digitalWrite(trigRecHBot, HIGH);
           digitalWrite(trigRecBot, HIGH);            
           state = 5;
           break;
        case 4:
           digitalWrite(trigTranBot, HIGH);             // Set the trigger output high
           digitalWrite(trigRecTop, HIGH);
           digitalWrite(trigRecHTop, HIGH);
           digitalWrite(trigRecHBot, HIGH);
           digitalWrite(trigRecBot, HIGH);
           state = 5;
           break;
        case 5:
          
        default:      
           digitalWrite(trigTranTop, LOW);
           digitalWrite(trigTranHTop, LOW);
           digitalWrite(trigTranHBot, LOW);
           digitalWrite(trigTranBot, LOW);    
           digitalWrite(trigRecTop, LOW);
           digitalWrite(trigRecHTop, LOW);
           digitalWrite(trigRecHBot, LOW);
           digitalWrite(trigRecBot, LOW);
           state = 0;                                // and return state to normal 0
           break;
     }
}

float CalculateAngles(float fixedlength1, float fixedlength2, float length1, float length2, float diagonal1, float diagonal2)
{
  float angle1, angle2;
  if(diagonal1 < 50 && diagonal2 < 50)
  {
    angle1 = (pow(fixedlength1,2) + pow(length1,2) - pow(diagonal2,2))/(2*fixedlength1*length1);
    angle2 = (pow(fixedlength2,2) + pow(length2,2) - pow(diagonal1,2))/(2*fixedlength2*length2);   
  }
  return angle1;
  // Angle 2 wil come after getting sin inverse code
}

// --------------------------
// echo_interrupt() External interrupt from HC-SR04 echo signal. 
// Called every time the echo signal changes state.
//
// Note: this routine does not handle the case where the timer
//       counter overflows which will result in the occassional error.

//void echo_interruptLeftCalf()
//{
//  switch (digitalRead(echoLeftCalf))                     // Test to see if the signal is high or low
//  {
//    case HIGH:                                      // High so must be the start of the echo pulse
//      echoLeftCalf_end = 0;
//      echoLeftCalf_duration = echoLeftCalf_end - echoLeftCalf_start;
//    case LOW:                                       // Low so must be the end of hte echo pulse
//      echoLeftCalf_end = micros();
//      echoLeftCalf_duration = echoLeftCalf_end - echoLeftCalf_start;
//  }
//}
//
//void echo_interruptRightCalf()
//{
//  switch (digitalRead(echoRightCalf))                     // Test to see if the signal is high or low
//  {
//    case HIGH:                                      // High so must be the start of the echo pulse
//        echoRightCalf_end = 0;                                 
//        echoRightCalf_start = micros();                        
//      
//    case LOW:                                       // Low so must be the end of hte echo pul
//        echoRightCalf_end = micros();
//        echoRightCalf_duration = echoRightCalf_end - echoRightCalf_start;
//  }
//}

void echo_interruptRecTop()
{
  switch (digitalRead(echoRecTop))                     // Test to see if the signal is high or low
  {
    case HIGH:                                      // High so must be the start of the echo pulse
      if(trigger_time_count > TICK_COUNTS*3/4)
      {
        echoTopTop_end = 0;
        echoTopTop_start = micros();
      }
      else if(trigger_time_count > TICK_COUNTS/2)
      {  
        echoHTopTop_end = 0;                                 // Clear the end time
        echoHTopTop_start = micros();                        // Save the start time
      }
      else if(trigger_time_count > TICK_COUNTS/4)
      {
        echoHBotTop_end = 0;
        echoHBotTop_start = micros();
      }
      else
      {
        echoBotTop_end = 0;
        echoBotTop_start = micros();
      }
      break;
      
    case LOW:                                       // Low so must be the end of hte echo pulse
      if(trigger_time_count > TICK_COUNTS*3/4)
      {
        echoTopTop_end = micros();
        echoTopTop_duration = echoTopTop_end - echoTopTop_start;
      }
      else if(trigger_time_count > TICK_COUNTS/2)
      {  
        echoHTopTop_end = micros();                                 
        echoHTopTop_duration = echoHTopTop_end - echoHTopTop_start; 
      }
      else if(trigger_time_count > TICK_COUNTS/4)
      {
        echoHBotTop_end = micros();
        echoHBotTop_duration = echoHBotTop_end - echoHBotTop_start;
      }
      else
      {
        echoBotTop_end = micros();
        echoBotTop_duration = echoBotTop_end - echoBotTop_start;
      }      
      break;
  }
}

void echo_interruptRecHTop()
{
  switch (digitalRead(echoRecHTop))                     // Test to see if the signal is high or low
  {
    case HIGH:                                      // High so must be the start of the echo pulse
      if(trigger_time_count > TICK_COUNTS*3/4)
      {
        echoTopHTop_end = 0;
        echoTopHTop_start = micros();
      }
      else if(trigger_time_count > TICK_COUNTS/2)
      {  
        echoHTopHTop_end = 0;                                 // Clear the end time
        echoHTopHTop_start = micros();                        // Save the start time
      }
      else if(trigger_time_count > TICK_COUNTS/4)
      {
        echoHBotHTop_end = 0;
        echoHBotHTop_start = micros();
      }
      else
      {
        echoBotHTop_end = 0;
        echoBotHTop_start = micros();
      }
      break;
      
    case LOW:                                       // Low so must be the end of hte echo pulse
      if(trigger_time_count > TICK_COUNTS*3/4)
      {
        echoTopHTop_end = micros();
        echoTopHTop_duration = echoTopHTop_end - echoTopTop_start;
      }
      else if(trigger_time_count > TICK_COUNTS/2)
      {  
        echoHTopHTop_end = micros();                                 
        echoHTopHTop_duration = echoHTopHTop_end - echoHTopHTop_start; 
      }
      else if(trigger_time_count > TICK_COUNTS/4)
      {
        echoHBotHTop_end = micros();
        echoHBotHTop_duration = echoHBotHTop_end - echoHBotHTop_start;
      }
      else
      {
        echoBotHTop_end = micros();
        echoBotHTop_duration = echoBotHTop_end - echoBotHTop_start;
      }      
      break;
  }
}

void echo_interruptRecHBot()
{
  switch (digitalRead(echoRecHBot))                     // Test to see if the signal is high or low
  {
    case HIGH:                                      // High so must be the start of the echo pulse
      if(trigger_time_count > TICK_COUNTS*3/4)
      {
        echoTopHBot_end = 0;
        echoTopHBot_start = micros();
      }
      else if(trigger_time_count > TICK_COUNTS/2)
      {  
        echoHTopHBot_end = 0;                                 // Clear the end time
        echoHTopHBot_start = micros();                        // Save the start time
      }
      else if(trigger_time_count > TICK_COUNTS/4)
      {
        echoHBotHBot_end = 0;
        echoHBotHBot_start = micros();
      }
      else
      {
        echoBotHBot_end = 0;
        echoBotHBot_start = micros();
      }
      break;
      
    case LOW:                                       // Low so must be the end of hte echo pulse
      if(trigger_time_count > TICK_COUNTS*3/4)
      {
        echoTopHBot_end = micros();
        echoTopHBot_duration = echoTopHBot_end - echoTopHBot_start;
      }
      else if(trigger_time_count > TICK_COUNTS/2)
      {  
        echoHTopHBot_end = micros();                                 
        echoHTopHBot_duration = echoHTopHBot_end - echoHTopHBot_start; 
      }
      else if(trigger_time_count > TICK_COUNTS/4)
      {
        echoHBotHBot_end = micros();
        echoHBotHBot_duration = echoHBotHBot_end - echoHBotHBot_start;
      }
      else
      {
        echoBotHBot_end = micros();
        echoBotHBot_duration = echoBotHBot_end - echoBotHBot_start;
      }      
      break;
  }
}

void echo_interruptRecBot()
{
  switch (digitalRead(echoRecBot))                     // Test to see if the signal is high or low
  {
    case HIGH:                                      // High so must be the start of the echo pulse
      if(trigger_time_count > TICK_COUNTS*3/4)
      {
        echoTopBot_end = 0;
        echoTopBot_start = micros();
      }
      else if(trigger_time_count > TICK_COUNTS/2)
      {  
        echoHTopBot_end = 0;                                 // Clear the end time
        echoHTopBot_start = micros();                        // Save the start time
      }
      else if(trigger_time_count > TICK_COUNTS/4)
      {
        echoHBotBot_end = 0;
        echoHBotBot_start = micros();
      }
      else
      {
        echoBotBot_end = 0;
        echoBotBot_start = micros();
      }
      break;
      
    case LOW:                                       // Low so must be the end of hte echo pulse
      if(trigger_time_count > TICK_COUNTS*3/4)
      {
        echoTopBot_end = micros();
        echoTopBot_duration = echoTopBot_end - echoTopBot_start;
      }
      else if(trigger_time_count > TICK_COUNTS/2)
      {  
        echoHTopBot_end = micros();                                 
        echoHTopBot_duration = echoHTopBot_end - echoHTopBot_start; 
      }
      else if(trigger_time_count > TICK_COUNTS/4)
      {
        echoHBotBot_end = micros();
        echoHBotBot_duration = echoHBotBot_end - echoHBotBot_start;
      }
      else
      {
        echoBotBot_end = micros();
        echoBotBot_duration = echoBotBot_end - echoBotBot_start;
      }      
      break;
  }
}
