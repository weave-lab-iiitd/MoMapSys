const int trigPinTrans = 3;
const int trigPinRec = 5;
const int echoPinTrans = 4;
const int echoPinRec = 6;
float distance1, distance1mapped, distance2, distance2mapped, duration1calc, duration2calc;
const int winwidth = 5;
float distance1filtered[winwidth];
float distance2filtered[winwidth];
float alpha = 0.6;
int counter = 0;
float avg(float[]);

void setup() {
  Serial.begin(9600);
  pinMode(trigPinTrans, OUTPUT);
  pinMode(trigPinRec, OUTPUT);
  pinMode(echoPinTrans, INPUT);
  pinMode(echoPinRec, INPUT);
  digitalWrite(trigPinTrans, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinTrans, LOW);
  duration1calc = pulseIn(echoPinTrans, HIGH);
  digitalWrite(trigPinRec, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinRec, LOW);
  duration2calc = pulseIn(echoPinRec, HIGH);
  for(int i = 0; i < winwidth; i++)
  {
    distance1filtered[i] = duration1calc;
    distance2filtered[i] = duration2calc;
  }
}

void loop() 
{
  digitalWrite(trigPinTrans, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinTrans, LOW);
  duration1calc = pulseIn(echoPinTrans, HIGH);
  digitalWrite(trigPinRec, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinRec, LOW);
  duration2calc = pulseIn(echoPinRec, HIGH); 
  distance1 = (1-alpha)*avg(distance1filtered)*0.0343 + alpha*duration1calc*0.0343;
  distance2 = (1-alpha)*avg(distance2filtered)*0.0343 + alpha*duration2calc*0.0343;
  distance1filtered[counter] = distance1;
  distance2filtered[counter] = distance2;
  counter = counter+1%winwidth;
  distance1mapped = map(distance1, 4, 27, 0.8, -0.12);
  distance2mapped = map(distance2, 4, 27, 0.8, -0.12);
  Serial.print(distance1mapped);
  Serial.print(',');
  Serial.println(distance2mapped);
  delay(10);
}

float avg(float arrays[])
{
  float sum = 0;
  for(int i = 0; i < winwidth; i++)
  {
    sum+=arrays[i];
  }
  return (sum/winwidth);
}

