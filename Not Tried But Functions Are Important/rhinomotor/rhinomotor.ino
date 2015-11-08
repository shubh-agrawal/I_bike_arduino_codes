void setup()
{
  pinMode(8,OUTPUT);  //pin 8 for direction pin
  pinMode(9,OUTPUT); //pin 11 for pulse pin
  digitalWrite(8,LOW); 
  unsigned long anglespeed(unsigned long ,int );
  void stopmotor();
  void godistance(unsigned long,unsigned long);
}
void anglespeed(unsigned long noofrev, int rpm) //function to cover specified number of revolutions at specified rpm, both in int. rpm above 10 would not be accepted as max rpm is 10
{
  if(rpm>10)
  {
  rpm=10;
  } //to avoid any chance of error
  for(unsigned long i=0;i<(noofrev*1800);i++) 
  {
    digitalWrite(9,HIGH);
    delay(33-3*rpm); //0.2 degrees per increment,  3.33 seconds delay for 10rpm
    digitalWrite(9,LOW);
    delayMicroseconds(5);
  }
}
void stalltest() //enclose in while(1) to stall
{
  digitalWrite(9,LOW);
  delayMicroseconds(5);
  digitalWrite(11,LOW);
  delayMicroseconds(5);
}
void stopmotor() //to really stop motor, enclose this function in while(1) 
{
    digitalWrite(9,LOW);
}
void godistance(int a, unsigned long distance) //function to make the motor move a distance, specified in centimeters forward or backward. Enter 1 as argument for backward and vice versa
{
  if(a==0)
  {
    digitalWrite(11,HIGH);
  for(double i=0;i<(distance*9.55);i++) //9.55 is the ratio between 1800 and 188.5 which is the value of 2pi*r(30cm)
  {
    
    digitalWrite(9,HIGH);
    delay(3); 
    digitalWrite(9,LOW);
    delayMicroseconds(5);
  }
  }
  
  else if(a==1)
  {
    digitalWrite(11,LOW);
    for(double i=0;i<(distance*9.55);i++)
  {    
    
    digitalWrite(9,HIGH);
    delay(3);
    digitalWrite(9,LOW);
    delayMicroseconds(5);
 }
 }
}
 
  void loop()
{
  godistance(1,376);
  delay(3000); //3 seconds delay is necessary for ompensating for lag between any two commands to the motor
  godistance(0,378);
  while(1)
  {
  stalltest();
  }  
}


