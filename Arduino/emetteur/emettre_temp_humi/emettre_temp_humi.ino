// Lecture temp et humi sur DHT22
// Arduino UNO
// Emetteur RF95 433 MHz


//partie rf95
#include <RH_RF95.h>
#include <SoftwareSerial.h>
SoftwareSerial SSerial(8, 9); // RX, TX
RH_RF95<SoftwareSerial> rf95(SSerial);

//partie dht22
#include "DHT.h"
#define DHTPIN 3     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);


///////////////////////////////////////////////////
void setup() {
    Serial.begin(115200);
    Serial.println("Test lecture DHT");
    if (!rf95.init()) {
        Serial.println("init RF95 failed");
        while (1);
    }
    Serial.println("initia ok");
    rf95.setFrequency(434.0);
    dht.begin();
}

///////////////////////////////////////////////////
void loop() {
/////////////////////envoi dht 22/////////////
String DHTdata = lireDHT();
// Convertissez la chaîne en tableau uint8_t
uint8_t buffer[DHTdata.length()];
data.getBytes(buffer, DHTdata.length());
// Transmettez les données via RFM95
rf95.send(buffer, sizeof(buffer));
rf95.waitPacketSent();
delay(1000);
//////////////////////envoi date//////////////
}

//////////////////////////////////////////////////////
String lireDHT(){
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  return "Humidite:" + String(humi) + "-Temperature:" + String(temp);
  delay(500);
}


