#include <string.h>
char lati[10],longi[10];
double LATI,LONGI;
char gpstr[6],b;
int j;
void getlatilongi(void)
{
  while(Serial.available()==0);
  for(b=Serial.read(),j=0;b!=',';j++)
  {
   gpstr[j]=b; 
   while(Serial.available()==0);
   b=Serial.read();
  }
  if (strcmp(gpstr,"$GPGGA")==0)
  {
   
   while(Serial.available()==0);
    for(b=Serial.read();b!=',';)
    {
      while(Serial.available()==0);
      b=Serial.read();
    }
     while(Serial.available()==0);
    
  for(b=Serial.read(),j=0;b!=',';j++)
  {
   lati[j]=b; 
   while(Serial.available()==0);
   b=Serial.read();
  }
    while(Serial.available()==0);
    b=Serial.read();
    while(Serial.available()==0);
    b=Serial.read();
    while(Serial.available()==0);
  for(b=Serial.read(),j=0;b!=',';j++)
  {
   longi[j]=b; 
   while(Serial.available()==0);
   b=Serial.read();
  }
  }
  else
 { 
   int count=0;
    while(count<5)
    {
      while(Serial.available()==0);
       b=Serial.read();
       if(b=='/r')
       count++;
    }
    
  }
}

void setup()
{
  
Serial.begin(9600);
for(int i=0;i<10;i++)
{
  lati[i]='0';
  longi[i]='0';
  
}
  
}
void loop()
{
  getlatilongi();
for(int i=0;i<10;i++)
{
  Serial.print(lati[i]);
  
}
 Serial.print("              ");
 for(int i=0;i<10;i++)
{
  Serial.print(longi[i]);
  
}
Serial.println(';');
delay(100);
  
  
  
  
/*  float m=atof(lati);
  float n=atof(longi);
  LATI=(int)(m/100)+(m-((int)(m/100))*100)/60*100;
  LONGI=(int)(n/100)+(n-((int)(n/100))*100)/60*100;
  Serial.print(LATI);
  Serial.print("    ");
  Serial.println(LONGI);
  delay(100);*/
}
