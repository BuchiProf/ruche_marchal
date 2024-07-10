/****************************************************************************/
//Fichier exemple pour mise à l'heure de la RTC
/****************************************************************************/
#include <Wire.h>
#include "DS1307.h"

DS1307 horloge;//define a object of DS1307 class
void setup() {
    Serial.begin(9600);
    horloge.begin();
    
    horloge.fillByYMD(2024, 7, 10); 
    horloge.fillByHMS(9, 10, 30);
    horloge.fillDayOfWeek(WED);
    horloge.setTime();//write time to the RTC chip
    
}
void loop() {
    printTime();
}
/*Function: Display time on the serial monitor*/
void printTime() {
    horloge.getTime();
    Serial.print(horloge.hour, DEC);
    Serial.print(":");
    Serial.print(horloge.minute, DEC);
    Serial.print(":");
    Serial.print(horloge.second, DEC);
    Serial.print("	");
    Serial.print(horloge.month, DEC);
    Serial.print("/");
    Serial.print(horloge.dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(horloge.year + 2000, DEC);
    Serial.print(" ");
    Serial.print(horloge.dayOfMonth);
    Serial.print("*");
    switch (horloge.dayOfWeek) { // Friendly printout the weekday
        case MON:
            Serial.print("MON");
            break;
        case TUE:
            Serial.print("TUE");
            break;
        case WED:
            Serial.print("WED");
            break;
        case THU:
            Serial.print("THU");
            break;
        case FRI:
            Serial.print("FRI");
            break;
        case SAT:
            Serial.print("SAT");
            break;
        case SUN:
            Serial.print("SUN");
            break;
    }
    Serial.println(" ");
    delay(1000);
}
