#include <avr/io.h>
#include <avr/interrupt.h>
void speed_drive_motor(void)
{
  int a=47;
  OCR1AH=a>>8;
  OCR1AL=a;
  
}


void setup()
{
  
  TCCR1A|=(0<<COM1A1)|(1<<COM1A0)|(0<<COM1B1)|(1<<COM1B0)|(0<<WGM10)|(0<<WGM11);
  TCCR1B|=0<<WGM13|1<<WGM12|0<<CS12|0<<CS11|1<<CS10;
  pinMode(8,OUTPUT);
digitalWrite(8,LOW);  //pin 8 for direction pin
  pinMode(9,OUTPUT);
  pinMode(11,OUTPUT); //pin 11 for pulse pin
  Serial.begin(9600);
}

void loop()
{
 speed_drive_motor();
delay(10000);
digitalWrite(11,LOW);

}
