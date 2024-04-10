// Reception des donnée via Rf95
// Arduino WIFI Rev2


#include <RH_RF95.h>

#include <SoftwareSerial.h>
#define COMSerial Serial1    //Rx (D0) - Tx (D1) coté arduino
#define ShowSerial Serial
RH_RF95<HardwareSerial> rf95(COMSerial);

void setup() {

    ShowSerial.begin(115200);
    ShowSerial.println("RF95 recepteur test.");
    
 
    if (!rf95.init()) {
        ShowSerial.println("init échoué");
        while (1);
    }
   
ShowSerial.println("initialisation ok");

    rf95.setFrequency(434.0);
}

void loop() {
    if (rf95.available()) {
        // s'il y a un message reçu
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (rf95.recv(buf, &len)) {
         
            ShowSerial.print("Avons reçu : ");
            ShowSerial.println((char*)buf);
            // Envoi de réponse
            uint8_t data[] = "Bien reçu";
            rf95.send(data, sizeof(data));
            rf95.waitPacketSent();
            ShowSerial.println("Envoie de réponse");

            
        } else {
            ShowSerial.println("Echec réception");
        }
    }
}


