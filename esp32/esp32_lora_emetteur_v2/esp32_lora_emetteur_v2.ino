/*********
  Sources : https://RandomNerdTutorials.com/esp32-lora-rfm95-transceiver-arduino-ide/
  Adaptation pour :
  - mesure de température, humidité et pression atmo avec BME280,
  - envoi des données par LoRa via rfm95 en 886.6MHz,
  - mesure de masse avec HX711,
  - mesure temperature et humidité interne avec DHT22
  - lecture date et heure RTC
  - gestion de l'énergie avec un passage en deepsleep.
  Buchi S 07-2024
*********/
//Préparation deepsleep
#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30          /* Time ESP32 will go to sleep (in seconds) */

#include <SPI.h>
#include <LoRa.h>
#include "HX711.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include "PCF85063TP.h"
#include "DHT.h"

//la rtc
PCD85063TP horloge;
// BME280 I2C
Adafruit_BME280 bme;
float temp;
float hum;
float pres;
// hx711
#define LOADCELL_DOUT_PIN  12
#define LOADCELL_SCK_PIN  4
HX711 scale;
float calibration_factor = -450; //-450 pour cellule 5kg
//#define calibration_factor 22983 //valeur obtenue expérimentalement pese personne lidl
//le DHT22
#define DHTPIN 13     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);
//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

void setup() {
 
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Affichage des données envoyées par LoRa");
  
  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(868E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

   // Initialize BME280 sensor 
  if (!bme.begin(0x76)) {
    Serial.println("Capteur BME280 non trouvé, vérifier cablage!");
    while (1);
  }
  /*configuration de la source de réveil :
  avec un timer pour se réveiller toutes les minutes (60 secondes)
  */
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

// configuration et envoi de la masse
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  String masse = String(scale.get_units(10)+252.0) + "g";
  envoiLoRa(masse);

//config et envoi RTC
  horloge.begin();
  String dateH = formaterDate();
  envoiLoRa(dateH);

//config et envoi DHT22
  String temp = lireTemp();
  envoiLoRa(temp);
  String humi = lireHumi();
  envoiLoRa(humi);

// config et envoi BME280
  temp = bme.readTemperature();
  hum = bme.readHumidity();
  pres = bme.readPressure()/100.0F;
  //concaténation des valeurs et envoi sour forme de texte
  String data = "BME280: T=" + String(temp) + ",H=" + String(hum) + ",P=" + String(pres);
  envoiLoRa(data);
    
/*on démarre le deepsleep*/
  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop() {
  // on ne rentre jamais dans la bouble loop
}

///////////////////LES FONCTIONS////////////////////////////
String formaterDate(){
  horloge.getTime();
  //on ajoute un "0" devant les mois inférieurs à 10
  String mois = ajouteZero(horloge.month);
  String jour = ajouteZero(horloge.dayOfMonth);
  String hour = ajouteZero(horloge.hour);
  String minute = ajouteZero(horloge.minute);
  //une chaine de caractere au format "YYYY-MM-DD HH:mm"
  return String(horloge.year+2000, DEC) + "-" + mois + "-" + jour+ " " + hour + ":" + minute +" ";
  }

String ajouteZero(uint8_t val){
  //une fonction qui rajoute un "0" pour toujours avoir 2 chiffres dans la date : 1-1-2024 -> 01-01-2024
  String newVal;
  if (val <10){
    newVal = "0" + String(val, DEC);
  }
  else {
    newVal = String(val, DEC);
    };
    return newVal;
}


String lireTemp(){
  float temp = dht.readTemperature();
  return "Temp:" + String(temp)+" ";
}

String lireHumi(){
  float humi = dht.readHumidity();
  return "Humi:" + String(humi)+" ";
}

void envoiLoRa(String valeur){
  //une fonction qui envoi une chaine de caractère par LoRa
    LoRa.beginPacket();
    LoRa.print(valeur);
    LoRa.endPacket();
    Serial.println(valeur);
    delay(1000);
}
