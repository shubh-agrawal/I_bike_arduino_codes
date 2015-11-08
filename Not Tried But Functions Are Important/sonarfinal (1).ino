/*
    Sonar Placement
    Sonar 1 : Facing towards the left of the bicycle
    Sonar 0 : Facing towards the center of the bicycle
    Sonar 2 : Facing towrds the right of the bicycle
    
    Turning algorithm :
    Detection by Sonar 0 ONLY : turn right by 28 degrees to get the obstacle out of range of vision
    Detection by Sonar 1 ONLY : don't do anything
    Detection by Sonar 2 ONLY : don't do anything
    Detection by Sonar 0 and Sonar 1 (object in front and left) : turn right by 35-37 degrees to get obstacle out of range of vision
    Detection by Sonar 0 and Sonar 2 (object in front and right) : turn left by 35-37 degrees to get obstacle out of range of vision
    Detection by Sonar 1 and Sonar 2 (object in right and left) : don't do anything
    Detection by all three Sonars : Abrupt turn of about 50 - 60 degrees on either side
*/
void setup() {
  pinMode(3, OUTPUT); //trig of sonar 0
  pinMode(5, OUTPUT); //trig of sonar 1
  pinMode(7, OUTPUT); //trig of sonar 2
  pinMode (2, INPUT); //echo of sonar 0
  pinMode (4, INPUT); //echo of sonar 1
  pinMode (6, INPUT); //echo of sonar 2
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
int change;
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  if (microseconds > 34000)
  {
      microseconds = 0;
  }  
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
  /*if (oldangle !=0)
  {
    if (currentangle == oldangle)
    {
        if (currentangle >0)
        {
            currentangle += 20;
        }
        else if (currentangle < 0)
        {
              currentangle-=20;
        }
         else
         {
             currentangle += 0;
         }
    }
    else
    {
        currentangle+=0;
    }   
  }
  else
  {
      currentangle+=0;
  }*/
  change=currentangle-oldangle;
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
      //Serial.println("Hag diya mc!!!!!!!!!");
      digitalWrite(11,HIGH);
      delay(3);
      digitalWrite(11,LOW);
      delayMicroseconds(5);
      //Serial.println("Hag diya mc!!!!!!!!!");
  }
  
  oldangle=currentangle;
  Serial.print("currentangle: ");
  Serial.println(currentangle);
  delay(300);
  //reading();
}
void finalangle()
{
     int angle;
     //Serial.println("cond1");
     if(cm[0]<=100 && cm[1]<=100 && cm[2]>100 && cm[0] !=0 && cm[1]!=0 && cm[2]!=0)  //left and centre object
    {
        //Serial.println("jnjads");
        angle = 20;
        gotoangle(angle); 
    
        //Function to turn angle would simply be called inside while loop
        // delay(250); //wait to complete turn
        //function will again take reading and then check for conditions of while loop, if not true loop breaks.
    }
    else if(cm[0]<=100 && cm[2]<=100 && cm[1]>100 && cm[0] !=0 && cm[1]!=0 && cm[2]!=0)  //right and centre object
    {
        //Serial.println("jnjads");
        angle = -20;
        gotoangle(angle); 
    
        //Function to turn angle would simply be called inside while loop
        // delay(250); //wait to complete turn
        //function will again take reading and then check for conditions of while loop, if not true loop breaks.
    }
    else if(cm[2]<=100 && cm[1]<=100 && cm[0]>100 && cm[0] !=0 && cm[1]!=0 && cm[2]!=0)     // left and right object
    {
        angle = 0;
        gotoangle(angle);
        // delay(250);
    }
    else if(cm[0]<=100 && cm[1]>100 && cm[2]>100 && cm[0] !=0 && cm[1]!=0 && cm[2]!=0)    //centre object
    {
        angle = 20;
        gotoangle(angle);
        // delay(250);
     }
    else if(cm[1]<=100 && cm[0]>100 && cm[2]>100 && cm[0] !=0 && cm[1]!=0 && cm[2]!=0)     //left object
    {
        angle = 10;
    }
    else if(cm[2]<=100 && cm[1]>100 && cm[0]>100 && cm[0] !=0 && cm[1]!=0 && cm[2]!=0)     //right object
    {
        angle = -10;
        gotoangle(angle);
        //  delay(250);
    }
    else if(cm[2]<=100 && cm[1]<=100 && cm[0]<=100 && cm[0] !=0 && cm[1]!=0 && cm[2]!=0)    //all direction object
    {
        //stall motor and perform reverse operation  
        angle = -20; 
        //if all three are detecting obstacles, the cycle has no option but turn abruptly
        gotoangle(angle);
        // delay(250);
    }
    else if(cm[2]>100 && cm[1]>100 && cm[0]>100 && cm[0] !=0 && cm[1]!=0 && cm[2]!=0)         //no obstacle found
    {
         angle = 0;
         gotoangle(angle);
         //delay(250);
    }
    else if (cm[0] == 0 && cm[1] == 0 && cm[2] == 0)
    {
        angle = 0;
        gotoangle(angle);
    }
    else
    {
        angle = 0;
        gotoangle(angle);
    }
    reading();
   
}

void loop()
{
  reading();
}


