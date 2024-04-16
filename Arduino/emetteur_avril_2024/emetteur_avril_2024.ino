// Lecture masse avec HX711 
// Lecture temp et humi sur DHT22
// lecture date heure avec RTC
// lecture tension-courant aux bornes de la batterie 12 V
// Arduino UNO
// Emetteur RF95 433 MHz
// Emission des données toutes les 5 minutes
// version 1 du 16 avril 2024

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

//Partie banlance HX711
#include "HX711.h" /
#define calibration_factor 22983 //valeur obtenue expérimentalement
#define LOADCELL_DOUT_PIN  7
#define LOADCELL_SCK_PIN  6
HX711 scale;

//Partie ina219 (mesure U et I)
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

///////////////////////////////////////////////////
void setup() {
    Serial.begin(115200);
    clock.begin();
    
    if (!rf95.init()) {
        Serial.println("init RF95 failed");
        while (1);
    }
    if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
     }
    //mesure du courant jusqu'à 400 mA et de la tension sous 16V 
    ina219.setCalibration_16V_400mA();

    Serial.println("initia ok");
    rf95.setFrequency(434.0);
    dht.begin();
    clock.getTime();
    Serial.println(clock.minute);
    //démarrage balance
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(calibration_factor); 
    scale.tare();	//on remet la balance à zero au démarrage


}

///////////////LOOP BEGIN///////////////////////////
void loop() {
clock.getTime();
//mesure toutes les 5 minutes
if ((clock.minute) % 5 == 0){
    /////////////////////envoi dht 22/////////////
    String temp = lireTemp();
    envoiRF95(temp);
    String humi = lireHumi();
    envoiRF95(humi);

    ///////////////////envoi masse///////
    String masse = lireMasse();
    envoiRF95(masse);

    //////////////////////envoi date//////////////
    String dateH = formaterDate();
    envoiRF95(dateH);

    ////////////////////////envoi U et I/////////////
    String tension = lireTension();
    String courant = lireCourant();
    envoiRF95(courant);
    envoiRF95(tension);
    

    // attendre une minute pour éviter répétition d'envois durant ce délais
    delay(60000);
}
Serial.print("attendre 10s : ");
Serial.println(formaterDate());
delay(10000);
}

///////////////////LOOP END////////////////////////////
///////////////////LES FONCTIONS////////////////////////////
String lireTemp(){
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  return "Temp:" + String(temp);
}

String lireHumi(){
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  return "Humi:" + String(humi);
}

String lireMasse(){
  float masse = scale.get_units(10);
  return "Masse:" + String(masse);
}

String lireTension(){
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float loadvoltage = busvoltage + (shuntvoltage / 1000);
  return "U=" + String(loadvoltage)+"V";
}

String lireCourant(){
  float current_mA = ina219.getCurrent_mA();
  return "I=" + String(current_mA) +"mA";
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

void envoiRF95(String data){
  uint8_t buffer[data.length()];
  data.getBytes(buffer, data.length());
  // Transmettez les données via RFM95
  rf95.send(buffer, sizeof(buffer));
  rf95.waitPacketSent();
  delay(2000);
}
