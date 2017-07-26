/*  
 *  Interactive LED lights by Leon Hong Chu @chuartdo
 *  Allow LCD display and remote control and display via Ble with Blynk App
 *  See library license and credits below
 */
#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h>
#include <Adafruit_NeoPixel.h>


#define DEBUG 1
 
#define NUM_LEDS 75      // Set to number of addressable RGB leds present

#define LED_DATAPIN 6     
#define HEADLIGHT_PIN 8


//Comment out if not using BLYNK BLE display control app
#define USE_BLYNK_GUI_APP 1

// comment out following line if lcd display is not used
#define lcd_display 1 

#ifdef lcd_display 
   #include <Wire.h>
   #include "rgb_lcd.h"
   rgb_lcd lcd;
#endif 

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, LED_DATAPIN);
int InterframeDelay = 1; //ms

BLEPeripheral blePeripheral; // create peripheral instance

uint32_t BaseLedColor;
uint8_t ColorEffect = 0;  // 0-9 shows different patterns. Default to 0 using Interactive display


//Blync Authentication Token
char auth[] = "8aca3456abe24c69ae883cf299b32128";
unsigned long gSPEED_VAL = 0;
float gSPEED_KPH = 0;
float gDISTANCE = 0;

#ifdef lcd_display
  void printLCD(  float velocity, float distance ) {
      String outputMsg;
      lcd.setCursor(0,0);
      outputMsg = "SPEED: ";
      outputMsg += String(velocity);
      outputMsg += (" km  ");
      outputMsg += "\0";
      lcd.print(outputMsg);
      
      outputMsg = "DIST : ";
      outputMsg += String(distance);
      outputMsg += (" km  ");
      outputMsg += "\0";
      lcd.setCursor(0,1);
      lcd.print(outputMsg);
  }
  
  void turnOnRed(bool on) {
    if (on)
      lcd.setRGB(100,0,0);
    else
      lcd.setRGB(30,30,30);   
  }

  void turnOnGreen(bool on) {
    if (on)
      lcd.setRGB(0,100,0);
    else
      lcd.setRGB(30,30,30);   
  }
#endif


void setup()
{
  // Clear LEDs
  LedSetup();
  SpeedSetup();
  OrientationSetup();

  #ifdef lcd_display
    lcd.begin(16, 2);
    lcd.print("BIke LED");

    lcd.setCursor(0,1);
    lcd.print("Display ");
    lcd.setRGB(100, 100, 100);
  #endif

  // Open serial port and tell the controller we're ready.
  Serial.begin(9600);

  pinMode(HEADLIGHT_PIN, OUTPUT); // use the LED on pin 13 as an output
  
  #ifdef USE_BLYNK_GUI_APP
    // set the local name peripheral advertises
    blePeripheral.setLocalName("BikeDisplay");
    blePeripheral.setDeviceName("BikeDisplay");
    blePeripheral.setAppearance(384);
  
    Blynk.begin(blePeripheral, auth);

    // advertise the service
    blePeripheral.begin();
    Serial.println(("Bluetooth device active, waiting for connections..."));
    
  #endif 
}

void loop()
{
  blePeripheral.poll();
  Blynk.run();

  // Read a command
  SpeedLoop();
  
#ifdef lcd_display
  printLCD(gSPEED_KPH, gDISTANCE);
#endif

  if (ColorEffect > 0) {
    ChangeColorEffect(ColorEffect);
  } else {
    MotionSensingLoop();
    LedLoop();
  }
}

#ifdef USE_BLYNK_GUI_APP
// Blynk library GUI communication code

BLYNK_CONNECTED() {
    Blynk.syncAll();
    #ifdef lcd_display
    lcd.begin(0, 2);
    lcd.print("GUI connected");
    lcd.setRGB(0, 0, 100);
    #endif
}

// Update speed to speed Widget
BLYNK_READ(V0) {
  float speed_kmph = gSPEED_VAL;
  Blynk.virtualWrite(V0, speed_kmph);
}

BLYNK_READ(V1) {
   Blynk.virtualWrite(V1, gSPEED_VAL);
   //    Serial.println("=1=");
}

// turn Headlight Relay On / Off
BLYNK_READ(V2) {
  bool lightOn =  (digitalRead(HEADLIGHT_PIN) == HIGH);
  // Toggle light
  digitalWrite(HEADLIGHT_PIN,lightOn?HIGH:LOW);
}

//Read syle slider value
BLYNK_WRITE(V3) {
  gSPEED_VAL = param.asInt();
       Serial.print("= ");
       Serial.println(gSPEED_VAL);

}

//Read syle slider value
BLYNK_WRITE(V4) {
  int effectNum = param.asInt();
  Serial.print("Effect  ");
  Serial.println(effectNum);
  ChangeColorEffect(effectNum);
}


BLYNK_WRITE(V5)   {
    int r = param[0].asInt(); // get a RED channel value
    int g = param[1].asInt(); // get a GREEN channel value
    int b = param[2].asInt(); // get a BLUE channel value
    if (DEBUG) {
        Serial.println("RGB ");
        Serial.print(r);
        Serial.print(g);
        Serial.print(b);
    }    
    #ifdef lcd_display
    lcd.setRGB(r,g,b);
    #endif
    BaseLedColor =  leds.Color(r,g,b);
    ColorEffect = 4;
}

#endif

  
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  Note: This requires CurieBLE library
    from http://librarymanager/all#CurieBLE

  Warning: Bluetooth support is in beta!
 *************************************************************/
