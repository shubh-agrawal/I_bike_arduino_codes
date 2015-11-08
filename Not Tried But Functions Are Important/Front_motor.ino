 /*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int central_adc = 798;              // need to be determined
int f_dir = 8;
int f_pulse = 11 ;


int x=0 , y=0 ;
int d , angle = 25;
int alpha = 10;
int final_adc ;
int l;
int check = 0;
void go_to_ang_step(int angle);
void go_to_ang_step(int angle);

#define F_CPU 16000000UL
#include <math.h>
#include <Wire.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>


#define ROT_ANGLE 180
#define PULSE_COUNT 5*ROT_ANGLE
#define PERIOD 60000
#define OPT 1

#define LEDPIN 13
#define PULPIN 11
#define DIRPIN 8 


volatile int currentAngle = 0;
volatile int count = 0;
volatile int flag = 0;
volatile int Pulse_Count = 0;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
void setOCR1(int value)
{
     OCR1B = value;
}
void setICR(int value)
{
     ICR1 = value;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    TCCR1B |= (1 << CS10);            // 1 prescaler
    //TIMSK1 |= (1 << OCIE1A);          // enable timer compare interrupt
    TIMSK1 |= (1<<TOIE1);
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void stop_motor(void)
{
    while(1)
    {
  //      digitalWrite(PULPIN,LOW);
        setOCR(0);
        
        Serial.print("current_adc .. wtf: ");
        Serial.println(analogRead(A0));
    }
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int go_to_angle(int angle)
{
    int ret = 0;
  
    int req_adc = (angle*1023.0)/270.0 + central_adc;
    Serial.print("req_adc:");
    Serial.print(req_adc);
    
    int current_adc = analogRead(A0);
    Serial.print("     current angle:");
    /*if (current_adc < 150 || current_adc > 930)
           {
             Serial.println("");
             Serial.print("motor stopped due to adc_cross");
             stop_motor();
           
           }*/
     
    
      if (req_adc < 100 || req_adc > 930 )          // if extends these adc_values stop the motor
          
           {
             Serial.println("");
             Serial.print("motor stopped due to adc_cross");
             stop_motor();
           
           }
     
    else 
     {
           
            Serial.print((current_adc - central_adc)*270.0/1023.0);
            
            if(current_adc - req_adc > 10)            // current_adc > req_adc means motor is on  is on left side of req. so rotate motor cw (1)
            { 
               case_1(current_adc,req_adc);
            }
            else if(current_adc - req_adc < -10)      // current_adc < req_adc means motor is on right side of req. angle., so rotate motor acw (-1)
            {
               case_2(current_adc,req_adc);  
            }
            else 
            {
                delay(300);
                Serial.print("    current adc :");
                Serial.println(analogRead(A0));
                ret = 1;
                delay(300);
                Serial.println(current_adc-req_adc);
                Serial.println("stopped");
                stop_motor();
           
            }
            return ret;
            
     }
}
void go_to_ang_step(int angle)
{
 int req_adc = angle/0.267 + central_adc;
 
      if (req_adc < 100 || req_adc > 930 )          // if extends these adc_values stop the motor
          
           {
             Serial.println("");
             Serial.print("motor stopped due to adc_cross");
             stop_motor();
           
           }
   Serial.print("req. adc :");
   Serial.println(req_adc);
  int current_adc = analogRead(A0);
  Serial.print("current angle:");
  Serial.println((current_adc - central_adc)*0.267);
 
  if(current_adc > req_adc + 2 )        // means motor is on left side of the req. angle
      {
         if(current_adc>req_adc)
         {
           digitalWrite(f_dir,LOW);  // SO rotate in clockwise .... if LOW of f_dir pin rotates cw.
           digitalWrite(f_pulse,HIGH);
           delay(3);
           digitalWrite(f_pulse,LOW);
           delayMicroseconds(5);
           current_adc = analogRead(A0);
           Serial.print("the_angle_difference==---------------------");
           Serial.println((current_adc - central_adc)*0.267);
         
           }

      }

   else if(current_adc < req_adc - 2 )        // means motor is on right side of the req. angle
      {
         if(current_adc<req_adc)
         {
           digitalWrite(f_dir,HIGH);  // SO rotate in clockwise .... if LOW of f_dir pin rotates cw.
           digitalWrite(f_pulse,HIGH);
           delay(3);
           digitalWrite(f_pulse,LOW);
           delayMicroseconds(5);
           current_adc = analogRead(A0);
           Serial.print("the_angle:==____________________");
           Serial.println((current_adc - central_adc)*0.267);
         
           }
      }
      else
      {
        digitalWrite(f_pulse,LOW);
        check = 1;
      }
     
     // Serial.println("stopped");
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void case_1( int current_adc, int req_adc)
{  
    changeAngle(1);
    current_adc = analogRead(A0);
    Serial.print("     Angle rotated:");
    Serial.println((current_adc - central_adc)*270.0/1023.0);
    Serial.print("    current adc :");
    Serial.println(analogRead(A0));
    delay(5);

}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void case_2( int current_adc, int req_adc)
{
    changeAngle(-1);
    current_adc = analogRead(A0);
    Serial.print("     Angle rotated:");
    Serial.print(( -current_adc + central_adc)*270.0/1023.0);
    Serial.print("    current adc :");
    Serial.println(analogRead(A0));
    delay(5);
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void case_3(int current_adc, int req_adc)
{
    
    Serial.println(current_adc-req_adc);
    Serial.println("stopped");
    final_adc = current_adc;
    Serial.print("    current adc :");
    Serial.println(analogRead(A0));
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


void setup()
{
    Serial.begin(115200);       // for recieving from rear arduino on rx0
    Serial1.begin(115200);      // for transmitting with rear arduino on tx1
    setPin(DIRPIN);
    noInterrupts();           // disable all interrupts 
    timInitPWM();
    interrupts();             // enable all interrupts
  
   /* central_adc = analogRead(A0);
    Serial.print("central_adc:");
    Serial.println(central_adc);*/
    
    
    /*int a = 0;
    while(!a)
    {
      a = go_to_ang_step(10);
    }
    delay(1000);
    Serial.print("current_adc :");
    Serial.print(analogRead(A0));
   /* a = 0;
    while(!a)
    a = go_to_angle(0);
    stop_motor();*/
 


}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void loop()
{
    if(check == 0)
    {
        go_to_ang_step(0);
         delay(3);
         Serial.print("current adc:");
         Serial.println(analogRead(A0)); 
      }

   Serial.print("final adc:");
   Serial.println(analogRead(A0)); 
      
 
      
}

void serialEvent() 
{
  while (Serial.available()) 
  {
   
    alpha = Serial.read();
    check = 0;
    //Serial.print("alpha :");
    //Serial.println(alpha);
 
  }
}


