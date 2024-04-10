// rf95_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_client
// Tested with Anarduino MiniWirelessLoRa



#include <RH_RF95.h>

    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(8, 9); // RX, TX
    
    #define ShowSerial Serial

    RH_RF95<SoftwareSerial> rf95(SSerial);






void setup() {
    ShowSerial.begin(115200);
    ShowSerial.println("RF95 server test.");

    if (!rf95.init()) {
        ShowSerial.println("init failed");
        while (1);
    }
    ShowSerial.println("initia ok");
    rf95.setFrequency(434.0);
}

void loop() {
       ShowSerial.println("Sending to rf95_server");
      // Send a message to rf95_server
      uint8_t data[] = "Hello World!";
     rf95.send(data, sizeof(data));
  
      rf95.waitPacketSent();
      // Now wait for a reply
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);

      if (rf95.waitAvailableTimeout(3000))
          { 
             // Should be a reply message for us now   
              if (rf95.recv(buf, &len))
               {
                 ShowSerial.print("got reply: ");
                 ShowSerial.println((char*)buf); 
               }
              else
               {
                  ShowSerial.println("recv failed");
               }
          }
       else
          {
            ShowSerial.println("No reply, is rf95_server running?");
           }
      delay(4000);
    
}


