#include <math.h>
#include <Wire.h>
#include <avr/io.h>

#define ROT_ANGLE 180
#define PULSE_COUNT 5*ROT_ANGLE
#define PERIOD 60000
#define OPT 10

#define LEDPIN 13
#define PULPIN 11
#define DIRPIN 8


volatile int currentAngle = 0;
volatile int count = 0;
volatile int flag = 0;
volatile int Pulse_Count = 0; 

//BASIC FUNCTIONS
void _delay()
{
   delay(40);
}

void setPin(int pin)
{
  digitalWrite(pin, HIGH); 
}

void resetPin(int pin)
{
  digitalWrite(pin, LOW); 
}

double getMax(double a, double b)
{
  if(abs(a) > abs(b))
   return a;
  else
   return b; 
}

void setOCR(int value)
{
   OCR1A = value; 
}
void setICR(int value)
{
   ICR1 = value; 
}

void timInitPWM()
{ 
  pinMode(PULPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  pinMode(DIRPIN, OUTPUT);
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
   
  setOCR(0);     // compare match register 16MHz/256/2Hz           /// Motor Pulse given @ 10kHz
  setICR(PERIOD);
  
  TCCR1A |= (1<<COM1A1)|(1<<WGM11); //Fast PWM Mode with ICR1 as TOP, Clear on Compare Match
  TCCR1B |= (1<<WGM12)|(1<<WGM13);  
  TCCR1B |= (1 << CS10);            // 8 prescaler 
  //TIMSK1 |= (1 << OCIE1A);          // enable timer compare interrupt
  TIMSK1 |= (1<<TOIE1);
}

ISR(TIMER1_OVF_vect)          // timer compare interrupt service routine
{
  //digitalWrite(LEDPIN, digitalRead(LEDPIN) ^ 1);   // toggle LED pin
  
  if(count < Pulse_Count && flag == 1)
  {  
    count++;
  }
  else
  {
    flag = 0;
    setOCR(0);
    count = 0;
    Pulse_Count = 0;
  }  
}

void gotoAngle(int g_Angle)  //Loaded Function should not be called at a high rate 
{
   int deltaAngle = g_Angle - currentAngle;
   
   if(deltaAngle != 0)
   {
     if(deltaAngle < 0)
     {
       setPin(DIRPIN);
       deltaAngle = -deltaAngle; 
     }
     else
     {
        resetPin(DIRPIN); 
     }
     flag = 1;
     Pulse_Count = 5*deltaAngle;
     setOCR(PERIOD/2);
     while(flag); 
     currentAngle = g_Angle; 
     delay(g_Angle*2);            // delay for ensuring the motor reaches given angle.    
   }
}

void changeAngle(int dir)
{
     if(dir == -1)
     setPin(DIRPIN);
     else if(dir == 1)
     resetPin(DIRPIN);
     else
     return;
     
     flag = 1;
     Pulse_Count = 5*OPT;
     setOCR(PERIOD/2);
     while(flag);
     //delay(1);  
}


void setup()
{
  setPin(DIRPIN);
  noInterrupts();           // disable all interrupts  
  timInitPWM();
  interrupts();             // enable all interrupts
 // Serial.begin(9600);
  
 // gotoAngle(90);
 
  
  int i = 360/OPT;  // Rotate 180 using Change Angle
  
  gotoAngle(360);
  
 // delay(2000);      //Dont Use Delays in the codes - Can create huge error as Delays have already been accounted for.
  
  while(i--)
  {
    changeAngle(-1);
  }  
}


void loop()
{
  
  
}

