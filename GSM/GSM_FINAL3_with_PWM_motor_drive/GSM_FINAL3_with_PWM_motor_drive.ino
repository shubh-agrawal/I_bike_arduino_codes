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


int i=0;
float DISTX,DISTY;       //DISTX and DISTY are integer distances in X direction and Y direction respectivey
char msg[75];             
char distx[4],disty[4],directx,directy;   //chalking of need values but in characters
int maxi,m,n,multi;
int front_pulse_pin , rear_pulse_pin=11 , front_direction_pin ,rear_direction_pin=9;

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


void clear_rx_buffer(void)                                                  
{ 
  //function to clear recieving buffer 
    while (Serial.available()>0)     // this function is used to clear rx_buffer
    {Serial.read();}   
}

void modem_initialization(void)
{
  Serial.begin(38400);
  delay(100);
  clear_rx_buffer();                                
  Serial.println("AT\r");// Attention command to wake up GSM modem
  delay(200);
  clear_rx_buffer();
  Serial.println("ATE0\r");
  clear_rx_buffer();
    Serial.println("AT+CMGF=1\r");      // setting up GSM in text mode
  delay(500);
  clear_rx_buffer();
      Serial.println("AT+CNMI=1,2,0,0,0\r");      // setting up GSM in text mode
  delay(500);
  clear_rx_buffer();



}


/***************************************************************************************************************************************/

void new_rec_sms(void)
{
while(1){
  while(Serial.available()>0)
  {
 msg[i]=Serial.read();
 //Serial.print(msg[i]);
 i=i+1;
 while(Serial.available()==0);
 //delayMicroseconds(500);
 }
  maxi=i;
 /****************************************************************************************/ 
  if(maxi>0){   
  for(i=51,m=0;msg[i]!=' ';i++,m++)   //m also gives me the number of digits
 {
   distx[m]=msg[i];
   
 } 

 directx=msg[i+1];
  for(i=i+3,n=0;msg[i]!=' ';i++,n++)   //n also gives me the number of digits
 {
   disty[n]=msg[i];
   
 } 
 directy=msg[i+1];
 //code for converting necessary distance value
 /******************************************************************************************/
 DISTX=atof(distx);
 DISTY=atof(disty);
 
 /***********************************************************/
 
  Serial.println(DISTX);
  Serial.println(directx);
  Serial.println(DISTY);
  Serial.println(directy);
  /***************************************************************/
  // Serial.print(msg[i]);
    
  
break;  
  }
  
  maxi=0;i=0;
  
}
}
 
 
/*************************************************************************************************************************************************************************/

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

void gotoDistance(float Dist)
{
  
   setPin(DIRPIN);
     flag = 1;
       Pulse_Count = 5*(360/1.8858)*Dist;
     setOCR(PERIOD/2);
     while(flag);     //Look in interrupt function 
      
     delay((360/1.8858)*Dist*2);           
  
  
}


/*************************************************************************************************************************************************************************/
void setup() 
 {
 
   modem_initialization();
   new_rec_sms();
   
 setPin(DIRPIN);
  noInterrupts();           // disable all interrupts  
  timInitPWM();
  interrupts();             // enable all interrupts
  

  int i = 360/OPT;  // Rotate 180 using Change Angle
  
  gotoDistance(DISTX);
 
   
  }

void loop() 
{
}



