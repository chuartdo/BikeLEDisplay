/*  
 *  Interactive LED lights by Leon Hong Chu @chuartdo
 *  Animate strip light by rendering strips of moving bars based on 
 *  speed of bike
  */

#define LightBarWidth 1
int     ledPos = 0; 
 
unsigned long t0;
unsigned long t1;
volatile boolean gBreakON = false;

uint32_t Wheel(byte WheelPos);
void drawFractionalBar( int pos16, int width, uint8_t hue);

void LedSetup() {
  leds.begin();
  leds.show();
  
  t0 = millis();
}


void LedLoop(){
  t1 = millis();
  
  if (t1 - t0 < 20 )   // Updates every 20 miliseconds
     return;

  int dx = 1;
  t0 = t1;
  
  ledPos +=   dx;

  if (ledPos > NUM_LEDS)
    ledPos = 0;
 
  uint16_t i;

// Draw Rainbow
    for(i=0; i< leds.numPixels(); i++) {

      if (gBreakON) {  
        // Turn on RED for strip color
          leds.setPixelColor(i,255,0,0);
          continue;
      }

      // Set fixed color every 2 consecutive led
      if (i % 3 != 0) {
        leds.setPixelColor(i,BaseLedColor);
        continue;
      }
 /*
      if (i % (gSPEED_VAL / 10) == 0) {
         leds.setPixelColor(i,gSPEED_VAL/100, 0, 0);
         continue;
      }
*/
      // rainbow background color light every 3 pixel
      leds.setPixelColor(i, Wheel(((i * 256 / leds.numPixels()) + i) & 255));
    }

  
  if (gSPEED_VAL > 0) {
    // Slower the speed, larger the gap
    uint16_t widthGap = LightBarWidth+3 + (12 -  gSPEED_VAL/100);
    uint32_t speedColor = leds.Color(0,40,  gSPEED_VAL /10);
    for (int bars = 0; bars < NUM_LEDS / widthGap; bars++) {
      drawSpeedBar( ledPos + bars*widthGap, LightBarWidth, speedColor);
    }
  }
  leds.show();
}

void drawSpeedBar(int startPos, int width, uint32_t color) {
  int i = startPos;
  
   for( int n = 0; n <= width; n++) {
      if( i >= NUM_LEDS) 
        i = 0; // wrap around
      leds.setPixelColor(i, color);
      i++; 
     
  }
}

 


