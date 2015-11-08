#include <math.h>
#include <Wire.h>
#include <avr/io.h>
/**********************************/
#define ROT_ANGLE 180
#define PULSE_COUNT 5*ROT_ANGLE
#define PERIOD 60000
#define OPT 10
/********************************/
#define LEDPIN 13
#define PULPIN 11
#define DIRPIN 8


volatile int currentAngle = 0;
volatile int count = 0;
volatile int flag = 0;
volatile int Pulse_Count = 0; 

/******************************************************************************************************************/

char temp,longi[11],lati[11];
int i;
float LATI,LONGI;
/******************************************************************************************************************/

int j=0;
float DISTX,DISTY;       //DISTX and DISTY are integer distances in X direction and Y direction respectivey
char msg[75];             
char distx[4],disty[4],directx,directy;   //chalking of need values but in characters
int maxi,m,n,multi;
int front_pulse_pin , rear_pulse_pin=11 , front_direction_pin ,rear_direction_pin=9;

/******************************************************************************************************************/
String stringOne, stringTwo, stringThree,stringFour,stringFive,stringSix,stringSeven,stringEight,stringNine,stringTen;
/*******************************************************************************************************************/
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
    while (Serial1.available()>0)     // this function is used to clear rx_buffer
    {Serial1.read();}   
}

void modem_initialization(void)
{
  Serial1.begin(38400);
  delay(100);
  clear_rx_buffer();                                
  Serial1.println("AT\r");// Attention command to wake up GSM modem
  delay(200);
  clear_rx_buffer();
  Serial1.println("ATE0\r");
  clear_rx_buffer();
  Serial1.println("AT+CMGF=1\r");      // setting up GSM in text mode
  delay(500);
  clear_rx_buffer();
  Serial1.println("AT+CNMI=1,2,0,0,0\r");      // setting up GSM in text mode
  delay(500);
  clear_rx_buffer();
  Serial1.println("AT+CIPSHUT\r");      // setting up GSM in text mode
  delay(200);
  clear_rx_buffer();
  Serial1.println("AT+CIPMUX=0\r");      // setting up GSM in text mode
  delay(200);
  clear_rx_buffer();
  Serial1.println("AT+CSTT=\"AIRTELGPRS.COM\",\",\"\r");      // setting up GSM in text mode
  delay(200);
  clear_rx_buffer();
  Serial1.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r");      // setting up GSM in text mode
  delay(200);
  clear_rx_buffer();
 Serial1.println("AT+SAPBR=3,1,\"APN\",\"AIRTELGPRS.COM\"\r");      // setting up GSM in text mode
  delay(200);
  clear_rx_buffer();
 Serial1.println("AT+SAPBR=1,1\r");      // setting up GSM in text mode
  delay(200);
  clear_rx_buffer(); 
Serial1.println("AT+HTTPINIT\r");      // setting up GSM in text mode
  delay(200);
  clear_rx_buffer();  



}

void getlatilongi(void)
{
  
                                                                //==================== searching for "GG" =================//        
        
            while ( !Serial.available() );    
        while ( 'G' != Serial.read() )
        {
           while(!Serial.available());
        }        
      
        while(!Serial.available());
     while ( 'G' != Serial.read() )
     { 
       while ( !Serial.available() );    
        while ( 'G' != Serial.read() )
        {
           while(!Serial.available());
        }        
      
        while(!Serial.available());
     }
    

 /***************************************************************************************************************************/
                                                                          //============== seeking for north cordi
    
    
        while ( !Serial.available() );                           
     while ( ',' != Serial.read() )                        //this flushes all values between two commas
     {
         while ( !Serial.available() );
     }
 
    while ( !Serial.available() );                           
     while ( ',' != Serial.read() )
     {
         while ( !Serial.available() );
     }
 
/****************************************************************************************************************************/
    Serial.print(" N: ");
    
   
        while ( !Serial.available() ); 
       
        for( i=0,temp=Serial.read();',' !=temp;i++)                //storing all north latitude values
        {
          lati[i]=temp;
          while ( !Serial.available() );
          temp=Serial.read();
          
        }
        lati[i]='\0';
        float m=atof(lati);                                        //conversion of array to floating value that is RAW data of GPS and then converting it in degrees
        Serial.print(m,8);                          
        Serial.print("   ");
      LATI=(int)(m/100)+(m-((int)(m/100))*100)/60.0;
      Serial.print(LATI,8);
/********************************************************************************************************************************/
    
    
        while ( !Serial.available() );                       // reading a character from the GPS
     while ( ',' != Serial.read() )
     {
    while ( !Serial.available() );   
     }

/***************************************************************************************************/

     Serial.print(" E: ");
    while ( !Serial.available() ); 
       
        for( i=0,temp=Serial.read();',' !=temp;i++)
        {
          longi[i]=temp;
          while ( !Serial.available() );
          temp=Serial.read();
          
        }
        longi[i]='\0';
        float n=atof(longi);                                              //conversion of array to floating value that is RAW data of GPS and then converting it in degrees
         Serial.print(m,8);
        Serial.print("   ");
      LONGI=(int)n/100+(n-((int)n/100)*100)/60.0;
      Serial.print(LONGI,8);
        
    Serial.println();
    
 
  
  
  
  
}
 

/***************************************************************************************************************************************/

void send_gps_data(void)               //Using grps
{
  stringOne = String("AT+HTTPARA=\"URL\",\"http://tracker.systron.in/trace.php?latitude=");
  stringTwo = String("&longitude=");
  stringThree=String("&time=");
  stringFour=String("&deviceid=");
  stringFive=String("\"\r");

  Serial1.println(stringOne + LATI + stringTwo + LONGI + stringThree+20156565+stringFour+"D1009"+stringFive);
  
}

void new_rec_sms(void)
{
while(1){
  while(Serial1.available()>0)
  {
 msg[j]=Serial1.read();
 //Serial.print(msg[i]);
 j=j+1;
 while(Serial1.available()==0);
 //delayMicroseconds(500);
 }
  maxi=j;
 /****************************************************************************************/ 
  if(maxi>0){   
  for(j=51,m=0;msg[j]!=' ';j++,m++)   //m also gives me the number of digits
 {
   distx[m]=msg[j];
   
 } 

 directx=msg[j+1];
  for(j=j+3,n=0;msg[j]!=' ';j++,n++)   //n also gives me the number of digits
 {
   disty[n]=msg[j];
   
 } 
 directy=msg[j+1];
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
  
  maxi=0;j=0;
  
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
   Serial.begin(9600);
 for(int i=0;i<10;i++)                  //initializing total array zero string to avoid producing garbage values
{
  lati[i]='0';
  longi[i]='0';
  
}
 lati[10]='\0';
 longi[10]='\0';
  delay(100);      

   modem_initialization();
   //new_rec_sms();
   
 /*setPin(DIRPIN);
  noInterrupts();           // disable all interrupts  
  timInitPWM();
  interrupts();             // enable all interrupts
  

  int i = 360/OPT;  // Rotate 180 using Change Angle
  
  gotoDistance(DISTX);*/
 
   
  }

void loop() 
{
  getlatilongi();
  //send_gps_data();
  delay(1000);
}



