// Lecture temp et humi sur DHT22
// lecture date heure avec RTC
// Arduino UNO
// Emetteur RF95 433 MHz

//partie RTC
#include <Wire.h>
#include "PCF85063TP.h"
PCD85063TP clock;

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
    clock.begin();
    Serial.println("Test lecture DHT");
    if (!rf95.init()) {
        Serial.println("init RF95 failed");
        while (1);
    }
    Serial.println("initia ok");
    rf95.setFrequency(434.0);
    dht.begin();
    clock.getTime();
    Serial.println(clock.minute);
}

///////////////LOOP BEGIN///////////////////////////
void loop() {
clock.getTime();
//mesure toutes les 5 minutes
if ((clock.minute) % 5 == 0){

/////////////////////envoi dht 22/////////////
String DHTdata = lireDHT();
Serial.print(DHTdata);
Serial.print(" : ");
Serial.println(DHTdata.length());
// Convertissez la chaîne en tableau uint8_t
uint8_t buffer[DHTdata.length()];
DHTdata.getBytes(buffer, DHTdata.length());
// Transmettez les données via RFM95
rf95.send(buffer, sizeof(buffer));
rf95.waitPacketSent();
delay(2000);
//////////////////////envoi date//////////////
String dateH = formaterDate();
Serial.print(dateH);
Serial.print(" : ");
Serial.println(dateH.length());
// Convertissez la chaîne en tableau uint8_t
uint8_t bufferD[dateH.length()];
dateH.getBytes(bufferD, dateH.length());
// Transmettez les données via RFM95
rf95.send(bufferD, sizeof(bufferD));
rf95.waitPacketSent();
delay(60000);
}
Serial.print("attendre : ");
Serial.println(formaterDate());
delay(10000);
}

///////////////////LOOP END////////////////////////////
///////////////////LES FONCTIONS////////////////////////////
String lireDHT(){
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  return "Humi:" + String(humi) + "-Temp:" + String(temp);
}

String formaterDate(){
  clock.getTime();
  //on ajoute un "0" devant les mois inférieurs à 10
  String mois = ajouteZero(clock.month);
  String jour = ajouteZero(clock.dayOfMonth);
  String hour = ajouteZero(clock.hour);
  String minute = ajouteZero(clock.minute);
  //une chaine de caractere au format "YYYY:MM:DD"
  return String(clock.year+2000, DEC) + ":" + mois + ":" + jour+ " " + hour + ":" + minute +" ";
  }

String ajouteZero(uint8_t val){
  String newVal;
  if (val <10){
    newVal = "0" + String(val, DEC);
  }
  else {
    newVal = String(val, DEC);
    };
    return newVal;
}
