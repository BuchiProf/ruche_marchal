/****************************************************************************/

/****************************************************************************/
#include <Wire.h>
#include "PCF85063TP.h"
#include "rgb_lcd.h"

rgb_lcd lcd;
PCD85063TP clock;//define a object of PCD85063TP class
void setup()
{
  Serial.begin(9600);
  clock.begin();
lcd.begin(16, 2);
  lcd.print("hello, world!");

    delay(1000);
 
}
void loop()
{
  printTime();
  delay(1000);
}
/*Function: Display time on the serial monitor*/
void printTime()
{
  clock.getTime();
  //on ajoute un "0" devant les mois inférieurs à 10
  String mois;
  if (clock.month <10){
    mois = "0" + String(clock.month, DEC);
  }
  else {
    mois = String(clock.month, DEC);
    };
  //une chaine de caractere au format "YYYY:MM:DD"
  String date = String(clock.year+2000, DEC) + ":" + mois + ":" + String(clock.dayOfMonth, DEC);
  Serial.println(date);
  lcd.setCursor(0, 1);
   
    lcd.print(date);

  delay(1000);
 
}
