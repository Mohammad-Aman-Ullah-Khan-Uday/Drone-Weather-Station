#include <avr/io.h>
#include "DHT1.h"
#include "DHT2.h"
#include "DS3231.h"
// #include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

/*parameters*/
#define standard 27
#define min_temp 40
#define max_temp 60
#define min_hum 30
#define max_hum 90

/*work state*/
#define paused 1
#define busy 2
#define enter 3
#define complete 4

/* job state*/
#define hold 5
#define start 6
#define check 7
#define stop 8

/*Arduino to ESP32*/
// #define RX 5
// #define TX 4
// SoftwareSerial esp32(RX, TX);

/*sensor pin*/
#define DHTPIN 2
#define DHTTYPE DHT22  // DHT 22  (AM2302)

/* pins for components*/
#define fan PB0    //8
#define heater PB1 //9

DHT dht(DHTPIN, DHTTYPE);

uint16_t  prev_time = 0, current_time = 0, target_min = 0, target_hr = 0, status = stop;
unsigned char desired_temp = 0, desired_hum = 0, state = 0, store[5];
float hum = 0, temp = 0, f = 0;
uint8_t work_State = 0, job_state = 0;
bool query = 0;
String esp_data;

void setup() {
   i2c_init();
  DDRB = (1 << fan) | (1 << heater);
  PORTB &= ~(1 << fan) | (1 << heater);
  Serial.begin(115200);
  //  esp32.begin(115200);
  StaticJsonDocument<200> doc;
  dht.begin();
  // esp32.println("HI esp32");
  Serial.println("AM2302!");
}

void loop() {
  dht22_func();
  StaticJsonDocument<200> doc;
  if(Serial.available()) {
    esp_data = Serial.readString();
    // Serial.println(esp_data);
    DeserializationError error = deserializeJson(doc, esp_data);
    desired_hum = doc["hum"].as<int>();
    desired_temp = doc["temp"].as<int>();
    target_min = doc["min"].as<int>();
    target_hr = doc["hr"].as<int>();
    query = doc["update"].as<bool>();  
    job_state = doc["job"].as<int>();   
    store[0] = desired_hum;
    store[1] = desired_temp;
    store[2] = job_state;
    store[3] = target_min;
    store[4] = target_hr;  
    if(target_min&target_hr){
      RTC();
      if((target_hr == hours) &&(target_min == minutes)){
        regulation(standard, standard, stop);
      }
    }
    if (query) {
      data_send(status);
    }
   Serial.print("Desired Humidity: ");
    Serial.print(desired_hum);
    Serial.print("   ");
    Serial.print("Desired Temperature: ");
    Serial.println(desired_temp);    
     for (int i = 0; i < 5; i++) {
    // this performs as EEPROM.write(i, i)
    EEPROM.update(i,store[i] );  //address, value
    }    

  }
  if ((!(desired_temp&0xff)) && (!(desired_hum&0xff)) && (!(job_state&0xff))) {
  regulation(EEPROM.read(0), EEPROM.read(1), EEPROM.read(2));
  }
  else{
    regulation(desired_temp, desired_hum, job_state);
  }
  // delay(500);
}

void data_send(int ws) {  //to send actual temp readings
  dht22_func();
  StaticJsonDocument<200> doc;
  doc["hum"] = hum;
  doc["temp"] = temp;
  doc["WS"] = ws;
  serializeJson(doc, Serial);  
  Serial.println();
  // Serial.print("Humidity: ");
  //   Serial.print(hum);
  //   Serial.print("   ");
  //   Serial.print("Temperature: ");
  //   Serial.println(temp);
 
}

void dht22_func() {

  hum = dht.readHumidity();
  temp = dht.readTemperature();
  f = dht.readTemperature(true);

  if (isnan(hum) || isnan(temp) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Serial.print("Humidity: ");
  // Serial.print(hum);
  // Serial.print("   ");
  // Serial.print("Temperature: ");
  // Serial.println(temp);

}


void regulation(int t, int h, int js) {  

  if (js == stop) {  //stop
    PORTB &= ~((1 << heater) | (1 << fan));
     while(temp > standard) {
      PORTB &= (1 << fan);
    } 
      status = complete;
  } 
  
  else if (js == check) {
    if (temp > standard) {
      PORTB &= (1 << fan);
    } else {
      status = check;
    }
  } 
  else if (js == hold) {
    PORTB &= ~((1 << heater) | (1 << fan));
    status = paused;
  } 
  else{

    if ((t < max_temp) && (t > min_temp)) {
      if (temp < t) {
        PORTB &= (1 << heater);
      } else if (temp > t) {
        PORTB &= (1 << fan);
      } else {
        PORTB &= ~((1 << fan) | (1 << heater));    
      }
    }

    if (hum < h) {
      PORTB &= ~(1 << fan);
    }
    else if (hum > h) {
      PORTB |= (1 << fan);
    }
    else {
      PORTB &= ~((1 << fan) | (1 << heater));
    }
  }
}



