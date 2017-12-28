#define NODEID         2    //0 = broadcast, 1= central node
#include <SimpleMesh.h>

#include <avr/power.h>

// Sensor libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h> // Must include Wire library for I2C
#include <SparkFun_MMA8452Q.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


// temperature
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

// humidity
#define DHTPIN            5   
#define DHTTYPE           DHT11  
#define DHTTYPE           DHT11  
DHT_Unified dht(DHTPIN, DHTTYPE);

// accelerometer
MMA8452Q accel;

// mesh
SimpleMesh mesh;

// counters
int countclose=0; // count for door close algo
bool doorIsOpen = false;
int doorOpenings = 0;

unsigned long lastTime = 0;

void setup() {
  // SAVE ALL THE POWER
  power_adc_disable();

  Serial.begin(9600);

  mesh.initialize(NODEID,1);

  // Start all engines!
  accel.init();
  sensors.begin();
  // GOOOOOOOOOO
}

void checkDoorOpen() {
  if (accel.available()){        //wait for new data from the accelerometer
      accel.read();              //read new values
      //Serial.println(accel.cz);
      //Door open
      if(accel.cz > 0.7){
        Serial.println("Door open");
        doorIsOpen = true;
      }

      //To make sure the signal we are reacting to is not just noise
      if(accel.cz < -0.7){
        countclose++;
      }
      
      //Door closed
      if (countclose < 20){
        //Serial.println("Door closed");
        countclose=0;
        if (doorIsOpen) {
          doorOpenings++;
          doorIsOpen = false;
        }
      }
  }
}

float getTemperature() {
  float temperature;
  
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  temperature = sensors.getTempCByIndex(0);

  while (temperature == -127.00) {// don't read wrong readings
    sensors.requestTemperatures(); // Send the command to get temperature readings 
    temperature = sensors.getTempCByIndex(0);
  }
  Serial.println(temperature); 

  return temperature;
}

float getHumidity() {
  sensors_event_t event;  
  dht.humidity().getEvent(&event);
  while (isnan(event.relative_humidity)) { // retry till it works
    dht.humidity().getEvent(&event);
  }
  
  Serial.println(event.relative_humidity);
  return event.relative_humidity;
}

void doTimers() {
  unsigned long now = millis();
  if (lastTime > now) {
    //reset happened
    lastTime = 0;
  }

  if ((lastTime + 60000) < now) { // send measurements every 60 seconds
      sendEnvoirement();
      lastTime = millis();
  }  
}


void sendEnvoirement() {
  if (!doorIsOpen) {
    float temp = getTemperature();
    float hum = getHumidity();     
    
    String out = "";
    out += "TMP ";
    out += String(temp);
    out += " HUM ";
    out += String(hum);
    out += " DOR ";
    out += doorOpenings;
    mesh.send(1, out.c_str());

    doorOpenings = 0;
  }
}

void sendDoorOpen() {
    doorOpenings = 0;
}

void loop() {
  //clock_prescale_set(clock_div_2);
  
  mesh.receiveDone(); // trigger mesh network, this node doesn't need to receive messages
  
  checkDoorOpen();  
  doTimers();

  //clock_prescale_set(clock_div_256);
  //delay(1000/256);
  //clock_prescale_set(clock_div_1);
  delay(100);
}