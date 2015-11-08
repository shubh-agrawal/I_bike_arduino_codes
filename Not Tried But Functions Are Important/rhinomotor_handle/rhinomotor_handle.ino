/*----------------------------------------------------------------*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
//  delay(500);
}
/*---------------------------------------------------------------------*/
   int front_flag = LOW;

/*---------------------------------------------------------------------*/
void setup()
{ 
/*-------------------------------------------------------------------------*/
  pinMode(9,OUTPUT);//pulse pin for front motor
  pinMode(10,OUTPUT);//direction pin for front motor
  pinMode(12,OUTPUT);  //pulse pin for rear motor
  pinMode(13,OUTPUT); //direction pin for rear motor
  digitalWrite(10,LOW);
 digitalWrite(13,LOW); 
//  unsigned long anglespeed(unsigned long ,int );
 // void stopmotor();
  //void godistance(unsigned long,unsigned long);
/*--------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
  Serial.begin(9600);
  Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();

/*----------------------------------------------------------------------------------*/
                }

/*----------------------------------------------------------------------------------------*/
float get_mag_sen_data(void){
 /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
 
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
if (headingDegrees>180) {headingDegrees=360-headingDegrees;
front_flag = LOW;   }
else {front_flag=HIGH;}
  return(headingDegrees);
  Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
  
  //delay(500);}
}
/*----------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
void front_motor_angle(float angle) {
   
  for(int i=0;i<(angle*5);i++)
  {
   digitalWrite(10,front_flag);    
   digitalWrite(9,HIGH);
   delay(3);
   digitalWrite(9,LOW);
   delayMicroseconds(5);
  }
}

void rear_motor_step(void)
{
    for(int i=0;i<400;i++)
  {    
    digitalWrite(12,HIGH);
    delay(2);
    digitalWrite(12,LOW);
    delayMicroseconds(5);
  }

}
/*-------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
  void loop()
{
 /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
 
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float ref_angle_0 = heading * 180/M_PI; 
if (ref_angle_0>180) {ref_angle_0=360-ref_angle_0;}
  
  Serial.print("Heading (degrees): "); Serial.println(ref_angle_0);
  
  //delay(500);
  
  
while(1)
{
  
rear_motor_step();

float ref_angle_1 = get_mag_sen_data();

float angle = ref_angle_1 - ref_angle_0;

Serial.print("angle (degrees): "); Serial.println(angle);

if (angle<0) 
          {angle=-1*angle;
          front_flag = HIGH;}
          
else       { front_flag = LOW;}
           
front_motor_angle(angle);

ref_angle_0=ref_angle_1;



}

}


