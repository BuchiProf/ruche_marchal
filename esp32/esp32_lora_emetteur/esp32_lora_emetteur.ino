/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Modified from the examples of the Arduino LoRa library
  More resources: https://RandomNerdTutorials.com/esp32-lora-rfm95-transceiver-arduino-ide/
  Adaptation pour mesure BME280 et deepsleep
*********/
//Préparation deepsleep
#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60          /* Time ESP32 will go to sleep (in seconds) */

#include <SPI.h>
#include <LoRa.h>
// BME280 I2C
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 bme;
// Variables to hold sensor readings
float temp;
float hum;
float pres;

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

int counter = 0;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");
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




    // New BME280 sensor readings
    temp = bme.readTemperature();
    hum = bme.readHumidity();
    pres = bme.readPressure()/100.0F;
    //concaténation des valeurs et envoi sour forme de texte
    String data = "BME280: T=" + String(temp) + ",H=" + String(hum) + ",P=" + String(pres);
    LoRa.beginPacket();
    LoRa.print(data);
    LoRa.endPacket();
    Serial.println(data);
    delay(1000);

    
    /*on démarre le deepsleep*/
    Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop() {
  
}
