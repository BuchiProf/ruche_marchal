// rf95_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_client
// Tested with Anarduino MiniWirelessLoRa

#include "rgb_lcd.h"
#include <Wire.h>

#include <RH_RF95.h>

    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(2, 3); // RX, TX
    #define COMSerial SSerial
    #define ShowSerial Serial

    RH_RF95<SoftwareSerial> rf95(COMSerial);



int led = 13;
rgb_lcd lcd;

void setup() {
    ShowSerial.begin(115200);
    ShowSerial.println("RF95 server test.");

    pinMode(led, OUTPUT);
    lcd.begin(16, 2);
    lcd.print("demarrage");

    delay(1000);

    if (!rf95.init()) {
        ShowSerial.println("init failed");
        while (1);
    }
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
    // you can set transmitter powers from 5 to 23 dBm:
    //rf95.setTxPower(13, false);

    rf95.setFrequency(434.0);
}

void loop() {
    if (rf95.available()) {
        // Should be a message for us now
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (rf95.recv(buf, &len)) {
            digitalWrite(led, HIGH);

            ShowSerial.print("reçu : ");
            ShowSerial.println((char*)buf);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print((char*)buf);
            delay(1000);

            // Send a reply
            uint8_t data[] = "Bien reçu";
            rf95.send(data, sizeof(data));
            rf95.waitPacketSent();
            ShowSerial.println("réponse envoyée");

            digitalWrite(led, LOW);
        } else {
            ShowSerial.println("erreur réception");
        }
    }
}


