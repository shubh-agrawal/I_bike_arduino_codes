void setup()
{
Serial.begin(38400); 
  Serial.println("Initiallizing....");
  delay(100);
Serial.println("AT\r");
delay(1000);
Serial.println("AT\r");
delay(1000);
Serial.println("ATD+919933988118;\r");
delay(30000);
Serial.println("ATH\r");

}
void loop()
{
  
    
}

