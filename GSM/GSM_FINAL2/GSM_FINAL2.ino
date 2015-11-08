
int i=0;
float DISTX,DISTY;       //DISTX and DISTY are integer distances in X direction and Y direction respectivey
char msg[75];             
char distx[4],disty[4],directx,directy;   //chalking of need values but in characters
int maxi,m,n,multi;
int front_pulse_pin , rear_pulse_pin=11 , front_direction_pin ,rear_direction_pin=9;

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
 
 delayMicroseconds(500);
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
/*************************************************************************************************************************************************************************/
void setup() 
 {
 
   modem_initialization();
   new_rec_sms();
  
   
  }

void loop() 
{

  
  
}



