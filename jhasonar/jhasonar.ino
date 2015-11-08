void setup() {
  pinMode(3, OUTPUT); //trig of sonar 1
  pinMode(5, OUTPUT); //trig of sonar 2
  pinMode(7, OUTPUT); //trig of sonar 3
  pinMode (2, INPUT); //echo of sonar 1
  pinMode (4, INPUT); //echo of sonar 2
  pinMode (6, INPUT); //echo of sonar 3
  pinMode(8,OUTPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);
  void reading();
  void gotoangle(int);
  void finalangle();
  long microsecondsToCentimeters(long);
}
long duration, cm[3];
int oldangle=0;

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void reading()
{
  int i=1;
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  // attach pin 3 to Trig
  for (i=1;i<4;i++)
  {
  digitalWrite(2*i+1, LOW);
  delayMicroseconds(2);
  digitalWrite(2*i+1, HIGH);
  delayMicroseconds(5);
  digitalWrite(2*i+1, LOW);
  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(2*i, HIGH);
  // convert the time into a distance
  cm[i-1] = microsecondsToCentimeters(duration);
  Serial.print(i);
  Serial.print(" :");
  Serial.println(cm[i-1]);
  delay(100);
  }
  finalangle();
}

void gotoangle(int currentangle)
{
     //Serial.print("oldangle:");
    // Serial.println(oldangle);
  int change=currentangle-oldangle;
  digitalWrite(8,LOW);
  if(change<0)
  {
    digitalWrite(8,HIGH);
    change*=-1;
  }
  Serial.print("change:");
  Serial.println(change);
     
  for(int j=0;j<change*5;j++)
  {
  digitalWrite(11,HIGH);
  delay(3);
  digitalWrite(11,LOW);
  delayMicroseconds(5);
  }
  
  oldangle=currentangle;
Serial.print("currentangle: ");
     Serial.println(currentangle);
     delay(300);
  reading();
}
void finalangle()
{
   int angle;
   //Serial.println("cond1");
   if(cm[0]<=10 && cm[1]<=10 && cm[2]>10)
  {
    //Serial.println("jnjads");
    angle=15;
    gotoangle(angle); 
    
    //Function to turn angle would simply be called inside while loop
   // delay(250); //wait to complete turn
    //function will again take reading and then check for conditions of while loop, if not true loop breaks.
  }
     //Serial.println("cond2");
  if(cm[2]<=10 && cm[1]<=10 && cm[0]>10)
   {
    angle=-15;
    gotoangle(angle);
   // delay(250);
  }
     //Serial.println("cond3");
  if(cm[0]<=10 && cm[1]>10 && cm[2]>10)
   {
    angle=30;
    gotoangle(angle);
   // delay(250);
  }
     //Serial.println("cond4");
  if(cm[2]<=10 && cm[1]>10 && cm[0]>10)
   {
    angle=-30;
    gotoangle(angle);
  //  delay(250);
  }
     //Serial.println("cond5");
  if(cm[2]<=10 && cm[1]<=10 && cm[0]<=10)
   {
    angle=90; 
    //if all three are detecting obstacles, the cycle has no option but turn abruptly
    gotoangle(angle);
   // delay(250);
   }
      //Serial.println("cond6");
   if(cm[2]>10 && cm[1]>10 && cm[0]>10)
   {
     angle=0;
     gotoangle(angle);
    delay(250);
   }
     // Serial.println("cond7");
   reading();
   
}

void loop()
{
  reading();
}


