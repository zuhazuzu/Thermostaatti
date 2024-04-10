#include <Arduino_MKRIoTCarrier.h>
#include "visuals.h"
#include "pitches.h"

MKRIoTCarrier carrier;

String state = "start";

//Alustetaan arvot
int count = 3;
int minlampotila = 21;
int maxlampotila = 30;
bool rele1 = false;                           // Lämmitys rele 1
bool automaattinen = true;                   // automaatti moodi
bool home = true;

uint32_t colorRed = carrier.leds.Color(200, 0, 0);
uint32_t colorGreen = carrier.leds.Color(0, 200, 0);
uint32_t colorBlue = carrier.leds.Color(0, 0, 200);

//Declare pages osa näistä on demon pohjalta
void configure();
void startPage();
void touchpage();
void sensorsPage();
void connectorsPage();
void actuatorsPage();
void temperatureControl();

void temperatureControl2();

//Declare their flags. Nämä ovat jäännöksiä demosta
bool touchCheckCompleted = false;
bool shakeCheckCompleted = false;
bool colorCheckCompleted = false;
bool gestureCheckCompleted = false;
bool relay1CheckCompleted = false;
bool relay2CheckCompleted = false;

//Käynnistykseen l
void setup()
{
  CARRIER_CASE = false;
  Serial.begin(9600);
  delay(1500);


  if (!carrier.begin())
  {
    Serial.println("Carrier not connected, check connections");
    while (1);
  }
}

void loop()
{
  temperatureControl();
  while (1);
}

void temperatureControl(){

  float temperature = carrier.Env.readTemperature();

while(true){

    int r,g,b;
    
    carrier.Light.readColor(r, g, b); //read rgb colour values

    carrier.Buttons.update();



    if (automaattinen == false){
      carrier.leds.fill(carrier.leds.Color(100, 0, 0), 0, 5);
    }
    if (automaattinen == true){
    carrier.leds.fill(carrier.leds.Color(0, 10, 0), 0, 5);
    }


    if (carrier.Buttons.onTouchDown(TOUCH0))  //Automaatti moodi päälle ja pois
    {
      if(automaattinen == true){
        carrier.leds.setPixelColor(2, colorRed);
        automaattinen = false;
        
      }
      
    }


    if (carrier.Buttons.onTouchDown(TOUCH1))  // Rele1 (lämmitys) manuaalisesti päälle ja kiinni (automaatti menee aina pois kun tähän kosketaan)
    {

      if(rele1 == false && automaattinen == false){
          rele1 = true;//turn relay 1 on if button 0 is pressed
          automaattinen = false;
      }
      if(automaattinen == true){
        maxlampotila++;
      }

    }

    if (carrier.Buttons.onTouchDown(TOUCH3))  //Automaatti moodi päälle ja pois
    {
      if(rele1 == true && automaattinen == false){
      rele1 = false;//turn relay 1 on if button 0 is pressed
      automaattinen = false;
      }
      if(automaattinen == true){
        maxlampotila--;
      }
    }
    if (carrier.Buttons.onTouchDown(TOUCH4))  //Automaatti moodi päälle ja pois
    {
        if(automaattinen == false){
        carrier.leds.setPixelColor(2, colorGreen);
        automaattinen = true; 

        }
    }



      if(temperature < (maxlampotila) && automaattinen == true) // jos lämpötila on alle sallitun ja automaatti tila on päällä niin lämmitys päälle
      {
      carrier.Relay1.open(); 
      carrier.display.fillScreen(0x0000);
      carrier.display.setTextSize(3);
      carrier.display.setCursor(54, 120);
      carrier.display.print("Lammitys1 ");

      carrier.display.setTextSize(3);
      carrier.display.setCursor(84, 150);
      carrier.display.println(maxlampotila);



    while (!carrier.Light.colorAvailable())
    {
      delay(5);
    }  
    if (r >= 135 && g >= 135 && b >= 135) // check if color/light is bright enough
    {
      carrier.display.setTextSize(2);
      carrier.display.setCursor(84, 180);
      carrier.display.println("Valosaa");
    } else {
      carrier.display.setTextSize(2);
      carrier.display.setCursor(84, 180);
      carrier.display.println("Dark");
    }
      
      carrier.display.setTextSize(2);
      carrier.display.setCursor(64, 60);
      carrier.display.println(temperature);


  }
  if(temperature > (maxlampotila +1) && automaattinen == true) // jos lämpötila on yli sallitun ja automaatti tila on päällä niin lämmitys pois
    {
      carrier.Relay1.close(); 
      carrier.display.fillScreen(0x0000);
      carrier.display.setTextSize(3);
      carrier.display.setCursor(54, 120);
      carrier.display.print("Lammitys2 ");

      carrier.display.setTextSize(3);
      carrier.display.setCursor(84, 150);
      carrier.display.println(maxlampotila);


    while (!carrier.Light.colorAvailable())
    {
      delay(5);
    }  
    if (r >= 135 && g >= 135 && b >= 135) // check if color/light is bright enough
    {
      carrier.display.setTextSize(2);
      carrier.display.setCursor(84, 180);
      carrier.display.println("Valosaa");
    } else {
      carrier.display.setTextSize(2);
      carrier.display.setCursor(84, 180);
      carrier.display.println("Dark");
    }
      carrier.display.setTextSize(2);
      carrier.display.setCursor(64, 60);
      carrier.display.println(temperature);
 
    }


      if (rele1 == true && automaattinen == false)  // Lämmitys (rele1) on päällä (viesti)
    {
      carrier.Relay1.open(); //turn relay 1 on if button 0 is pressed
      carrier.display.fillScreen(0x0000);
      carrier.display.setTextSize(2);
      carrier.display.setCursor(44, 120);
      carrier.display.print("rele auki");

    }
    if (rele1 == false && automaattinen == false) // Lämmitys (rele1) on kiinni (viesti)
    {
      carrier.Relay1.close(); //turn relay 1 off if button 4 is pressed
      carrier.display.fillScreen(0x0000);
      carrier.display.setTextSize(2);
      carrier.display.setCursor(44, 120);
      carrier.display.print("Rele kiinni");
      
    }
    carrier.leds.show();
    delay(150);
}
}
