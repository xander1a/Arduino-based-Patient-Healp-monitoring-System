#include <SoftwareSerial.h>
#include <TinyGPS++.h>
SoftwareSerial sim(4,5);
#include <Wire.h>
#include "DHT.h"
#define DHTPIN 13 
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000
#define REPORTING_PERIOD_MS1     500
#define DHTTYPE DHT11 
static const int RXPin = 10, TXPin = 11;
static const uint32_t GPSBaud = 9600;

DHT dht(DHTPIN, DHTTYPE);
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
//////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////


int _timeout;
String _buffer;
String number = "0783159293"; //-> change with your number
PulseOximeter pox;

uint32_t tsLastReport = 0;
uint32_t tsLastReport1 = 0;

void onBeatDetected()
{
    Serial.println("Beat!");
}


void setup() {
  //delay(7000); //delay for 7 seconds to make sure the modules get the signal
  Serial.begin(112500);
    dht.begin();
  _buffer.reserve(50);
  Serial.println("Sistem Started...");
  sim.begin(9600);
  ss.begin(GPSBaud);
  delay(1000);


       if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
//SendMessage();
// callNumber();  
}
void loop() {
  
  //amerekezo();
  heartbeat();
  
    }




  
//dht11();
    

void SendMessage()
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Hello ";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();

  
}
void RecieveMessage()
{
  Serial.println ("SIM800L Read an SMS");
  sim.println("AT+CMGF=1");
  delay (200);
  sim.println("AT+CNMI=1,2,0,0,0"); // AT Command to receive a live SMS
  delay(200);
  Serial.write ("Unread Message done");

  
}
String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}
void callNumber() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}

void heartbeat(){
  
  
   pox.update();

    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.println("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        dht11();
       
        tsLastReport = millis();
       
    }

  amerekezo();
  }
    void dht11(){
    
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
 
      }


      void amerekezo(){
         if (millis() - tsLastReport1 > REPORTING_PERIOD_MS1) {
          // This sketch displays information every time a new sentence is correctly encoded.
     while (ss.available() > 0){
     gps.encode(ss.read());
     if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
    }
   // heartbeat();
  }
        
         
           tsLastReport1 = millis();} 
        }
