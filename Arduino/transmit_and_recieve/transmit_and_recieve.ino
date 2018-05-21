//  Initial Set up and Experimentation for the setup
//  Using HCSR04 and Arduino UNO
//  Trigger pins go in PWM, echo can go anywhere
//  Make sure to fit in the coordinates of the recievers according to your set up
//  Documented on 12/5/2018


// Declarations can be changed according to the user
// All trig pins go to PWM
const int trigPinTrans[4] = {11,8,7,6};
const int trigPinReciv[3] = {5,3,9};
const int echoPinTrans[4] = {12,25,26,27};
const int echoPinReciv[3] = {24,22,23};

// 3 durations and distances
float duration[3];
float distance[3];
// estimated coordinates of the Transmitter
float x, y, z;

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 4; i++)
  {
    pinMode(trigPinTrans[i], OUTPUT);
    pinMode(echoPinTrans[i], INPUT);
  }
  for(int i = 0; i < 3; i++)
  {
    pinMode(trigPinReciv[i], OUTPUT);
    pinMode(echoPinReciv[i], INPUT);
  }
}

void loop() {
  
  for(int j = 0; j < 4; j++)
    digitalWrite(trigPinTrans[j], LOW);
  for(int j = 0; j < 3; j++)
    digitalWrite(trigPinReciv[j], LOW);
  delayMicroseconds(2);
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 4; j++)
      digitalWrite(trigPinTrans[j], HIGH);
    digitalWrite(trigPinReciv[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinReciv[i], LOW);
    for(int j = 0; j < 4; j++)
      digitalWrite(trigPinTrans[j], LOW);
    duration[i] = pulseIn(echoPinReciv[i], HIGH);
    distance[i] = duration[i]*0.034;
    delay(10);
  }
  //Printing statements
  Serial.print("Measured Distance1:");
  Serial.println(distance[0]);
  Serial.print("Measured Distance2:");
  Serial.println(distance[1]);
  Serial.print("Measured Distance3:");
  Serial.println(distance[2]);
  Calc();   //Used to find the coordinates of the transmitter when the coordinates of the reciever are assumed to be constant.
  //Coordinates have been found through Calc(), Now displaying,  
  Serial.print("Coordinates:  X:");
  Serial.print(x);
  Serial.print("      Y:");
  Serial.print(y);
  Serial.print("      Z:");
  Serial.println(z);
  delay(1000);
}

void Calc()
{
  /*Find the coordinates of the transmitter given the distances from the three recievers. This assumes
  that the first reciever is at (0,0) , the other is at (l,0) and the third is at (0,r) all at the same
  plane, it kind of works out as the z coordinates are 0 for all the above. Since the distances and the
  theoretical calculations had to be mapped hence used a,b,c for the three distances.*/
  int counter;
  float a,b,c,l,r;
  a = distance[1];
  b = distance[2];
  c = distance[0];
  l = 14;
  r = 12.5;
  // 3 x coordinates and 3 y coordinates which resulted due to the intersection of planes on which the 
  // circles of intersection of the three spheres lied. one circle lies in the yz plane, one on the 
  // xz plane. the first 2 points are
  float xp = (pow(a,2) - pow(b,2) + pow(l,2))/(2*l); 
  float yp = (pow(a,2) - pow(c,2) + pow(r,2))/(2*r);
  // Let m be the slope of the third plane in the xy plane
  float m = l/r;
  // Using the radical axes equation S1 - S2 = 0 which gives the plane of intersection between two spheres
  // assuming S1 = 0 and S2 = 0 is the equation of the 2 spheres
  float cp = (pow(b,2) - pow(c,2) + pow(l,2) - pow(r,2))/(2*l);
  //  averaging out the x value for the three given values
  x = (2*xp + ((yp-cp)/m))/3;
  //  averaging out the y value for the three given values
  y = (2*yp + (m*xp + cp))/3;
  z = 0;  //for convenience sake, otherwise it becomes imaginary if the spheres don't intersect at all
  counter = 0;
  if(pow(a,2)>(pow(x,2) + pow(y,2)))
  {
    z += sqrt(pow(a,2)-pow(x,2)-pow(y,2));
    counter+=1;
  }
  if(pow(b,2)>(pow(x-l,2) + pow(y,2)))
  {
    z += sqrt(pow(b,2)-pow(x-l,2)-pow(y,2));
    counter+=1;
  }
  if(pow(c,2)>(pow(x,2) + pow(y-r,2)))
  {
    z += sqrt(pow(c,2)-pow(x,2)-pow(y-r,2));
    counter+=1;
  }
  z = z/counter; //to make sure the most average value possible.
}

