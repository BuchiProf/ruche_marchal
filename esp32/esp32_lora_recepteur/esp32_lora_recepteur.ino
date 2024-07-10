/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Modified from the examples of the Arduino LoRa library
  More resources: https://RandomNerdTutorials.com/esp32-lora-rfm95-transceiver-arduino-ide/
*********/

#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //868E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(868E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.println("Paquet reçu !");

    // lire le paquet au format "BME280: T=22.5,H=68,P=989"
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.println(LoRaData);
      /*
      //on divise la chaine de caractère pour en extraire des valeurs numérique et préparer le stockage en BDD
      if (LoRaData.substring(0,6)=="BME280"){
                  // Divisez les données reçues par ": "
                  int colonIndex = LoRaData.indexOf(": ");
                  String sensorId = LoRaData.substring(0, colonIndex); //contient "BME280"
                  String valuesStr = LoRaData.substring(colonIndex + 2); // +2 pour ignorer ": " et contient "T=22.5,H=68,P=989"

                  // Divisez les valeurs par le premier ","
                  int commaIndex1 = valuesStr.indexOf(",");
                  String temperatureStr = valuesStr.substring(2, commaIndex1); //contient "22.5"
                  String resteDroite = valuesStr.substring(commaIndex1 + 1); // +1 pour ignorer "," et contient "H=68,P=989"
                  // Divisez les valeurs par le second ","
                  int commaIndex2 = resteDroite.indexOf(",");
                  String humiditeStr = resteDroite.substring(2, commaIndex2);  //contient "68"
                  String pressionStr = resteDroite.substring(commaIndex1 + 3); // +1 pour ignorer ",P=" et contient "989"

                  // Convertissez la température et l'humidité en nombres décimaux
                  float temperature = temperatureStr.toFloat();
                  float humidite = humiditeStr.toFloat();
                  float pression = pressionStr.toFloat();
                  Serial.print("la temperature : ");
                  Serial.println(temperature);
                  Serial.print("l'humidité : ");
                  Serial.println(humidite);
                  Serial.print("la pression : ");
                  Serial.println(pression);
                }
                */
    }

    // print RSSI of packet
    Serial.print("avec un RSSI de ");
    Serial.print(LoRa.packetRssi());
    Serial.println(" dB");
  }
}
