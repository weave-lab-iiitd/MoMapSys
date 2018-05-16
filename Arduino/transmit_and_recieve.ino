//  Initial Set up and Experimentation for the setup
//  Using HCSR04 and Arduino UNO
//  Trigger pins go in PWM, echo can go anywhere
//  Make sure to fit in the coordinates of the recievers according to your set up
//  Documented on 12/5/2018


// Declarations can be changed according to the user
// All trig pins go to PWM
const int trigPinTrans = 11;
const int echoPinTrans = 12;
const int trigPinReciv1 = 5;
const int echoPinReciv1 = 4;
const int trigPinReciv2 = 3;
const int echoPinReciv2 = 2;
const int trigPinReciv3 = 9;
const int echoPinReciv3 = 8;

// 3 durations and distances
float duration1, duration2, duration3;
float distance1, distance2, distance3;
// estimated coordinates of the Transmitter
float x, y, z;

void setup() {
  Serial.begin(9600);
  pinMode(trigPinTrans, OUTPUT);
  pinMode(echoPinTrans, INPUT);
  pinMode(trigPinReciv1, OUTPUT);
  pinMode(echoPinReciv1, INPUT);
  pinMode(trigPinReciv2, OUTPUT);
  pinMode(echoPinReciv2, INPUT);
  pinMode(trigPinReciv3, OUTPUT);
  pinMode(echoPinReciv3, INPUT);
}

void loop() {
  
  
  digitalWrite(trigPinTrans, LOW);   //Set all pins to LOW
  digitalWrite(trigPinReciv1, LOW);
  digitalWrite(trigPinReciv2, LOW);
  digitalWrite(trigPinReciv3, LOW);
  delayMicroseconds(2);
  //Trigger 1
  digitalWrite(trigPinTrans, HIGH); //First Calculate distance between Transmitter
  digitalWrite(trigPinReciv1, HIGH);  // and Reciever 1
  delayMicroseconds(10);
  digitalWrite(trigPinTrans, LOW);
  digitalWrite(trigPinReciv1, LOW);
  duration1 = pulseIn(echoPinReciv1, HIGH);
  distance1 = duration1*0.034; // Distance between transmitter and reciever 1
  delay(10);
  //Trigger 2
  digitalWrite(trigPinTrans, HIGH); //Calculate distance between Transmitter
  digitalWrite(trigPinReciv2, HIGH);  // and reciever 2
  delayMicroseconds(10);
  digitalWrite(trigPinTrans, LOW);
  digitalWrite(trigPinReciv2, LOW);
  duration2 = pulseIn(echoPinReciv2, HIGH);
  distance2 = duration2*0.034;  //Distance between Transmitter and Reciever 2
  delay(10);
  //Trigger 3
  digitalWrite(trigPinTrans, HIGH); //Calculate Distance between Transmitter
  digitalWrite(trigPinReciv3, HIGH);  // and Reciever 3
  delayMicroseconds(10);
  digitalWrite(trigPinTrans, LOW);
  digitalWrite(trigPinReciv3, LOW);
  duration3 = pulseIn(echoPinReciv3, HIGH);
  distance3 = duration3*0.034;  //Distance between transmitter and reciever 3
  //Printing statements
  Serial.print("Measured Distance1:");
  Serial.println(distance1);
  Serial.print("Measured Distance2:");
  Serial.println(distance2);
  Serial.print("Measured Distance3:");
  Serial.println(distance3);
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
  a = distance2;
  b = distance3;
  c = distance1;
  l = 12;
  r = 9.5;
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

