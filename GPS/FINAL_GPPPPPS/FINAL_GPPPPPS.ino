
char temp,longi[11],lati[11];
int i;
float LATI,LONGI;
 
void getlatilongi(void)
{
  
                                                                //==================== searching for "GG" ===================//
    
    
        
        
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
}
 
void loop ()
{
   getlatilongi();  
}
 
