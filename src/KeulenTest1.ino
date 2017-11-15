/********************************************************************/
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h> // Must include Wire library for I2C
#include <SFE_MMA8452Q.h> // Includes the SFE_MMA8452Q library
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 5 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/
MMA8452Q accel;
/********************************************************************/
void setup(void) 
{ 
 // start serial port 
 Serial.begin(9600); 
 Serial.println("Temperature IC Control + Moooove"); 
 // Start up the library
 accel.init();//start accelerometer 
 sensors.begin();//start temp sensor 
 
} 
void loop(void) 
{ 
 // call sensors.requestTemperatures() to issue a global temperature 
 // request to all devices on the bus 
/********************************************************************/
 sensors.requestTemperatures(); // Send the command to get temperature readings 
/********************************************************************/
 Serial.print("Temperature: "); 
 Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?
 Serial.print(" / ");  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire
 if (accel.available())
  {
    accel.read();

    printCalculatedAccels();
    
    printOrientation();
    
    Serial.println();
  }
}
void printAccels()
{
  Serial.print(accel.x, 3);
  Serial.print(" ");
  Serial.print(accel.y, 3);
  Serial.print(" ");
  Serial.print(accel.z, 3);
  Serial.print(" ");
}
void printCalculatedAccels()
{ 
  Serial.print(accel.cx, 3);
  Serial.print(" ");
  Serial.print(accel.cy, 3);
  Serial.print(" ");
  Serial.print(accel.cz, 3);
  Serial.print(" ");
}
void printOrientation()
{
  // accel.readPL() will return a byte containing information
  // about the orientation of the sensor. It will be either
  // PORTRAIT_U, PORTRAIT_D, LANDSCAPE_R, LANDSCAPE_L, or
  // LOCKOUT.
  byte pl = accel.readPL();
  switch (pl)
  {
  case PORTRAIT_U:
    Serial.print("Portrait Up");
    break;
  case PORTRAIT_D:
    Serial.print("Portrait Down");
    break;
  case LANDSCAPE_R:
    Serial.print("Landscape Right");
    break;
  case LANDSCAPE_L:
    Serial.print("Landscape Left");
    break;
  case LOCKOUT:
    Serial.print("Flat");
    break;
  }
}
