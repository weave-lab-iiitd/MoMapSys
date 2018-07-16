#include <TimerOne.h>
#define trigPin1 3
#define trigPin2 5
#define echoPin1 4
#define echoPin2 6
#define echo_int1 7
#define echo_int2 8

#define TIMER_US 50
#define TICK_COUNTS 4000

volatile long echo1_start = 0;
volatile long echo2_start = 0;
volatile long echo1_end = 0;
volatile long echo2_end = 0;
volatile long echo1_duration = 0;
volatile long echo2_duration = 0;
volatile int trigger_time_count = 0;
volatile long range_flasher_counter = 0;


void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  Timer1.initialize(TIMER_US);
  Timer1.attachInterrupt(timerISR);
  attachInterrupt(echo_int1, echo1_interrupt, CHANGE);
  attachInterrupt(echo_int2, echo2_interrupt, CHANGE);
  Serial.begin(9600);
}

void loop() {
  Serial.println(echo1_duration);
  delay(20);
}

void timerISR(void)
{
  trigger_pulse();
}

void trigger_pulse()
{
  static volatile int state = 0;
  if(!(-trigger_time_count))
  {
    trigger_time_count = TICK_COUNTS;
    state = 1;
  }

  switch(state)
  {
    case 0: break;
    case 1: 
      digitalWrite(trigPin1, HIGH);
      digitalWrite(trigPin2, HIGH);
      state = 2;
    case 2:
    default:
      digitalWrite(trigPin1, LOW);
      digitalWrite(trigPin2, LOW);
      state = 0;
      break;
  }
}

void echo2_interrupt()
{
  switch(digitalRead(echoPin2))
  {
    case HIGH: echo2_end = 0;
               echo2_start = micros();
               break;
    case LOW: echo2_end = micros();
              echo2_duration = echo2_end - echo2_start;
              break;
  }
}

void echo1_interrupt()
{
  switch(digitalRead(echoPin1))
  {
    case HIGH: echo1_end = 0;
               echo1_start = micros();
               break;
    case LOW: echo1_end = micros();
              echo1_duration = echo1_end - echo1_start;
              break;
  }
}

