/*
  DigitalReadSerial
  Calculate current speed  
  Read reed switch and calculate speed based on wheel diameter.
*/

#define MAXSPEED 60
#define WHEEL_DIAMETER 0.61
#define BREAK_PIN 2
#define REEDSWITCH_PIN 3
#define EFFECT_PIN 4

#define WAIT_TIME_MS 2000
#define MAX_REVOLUTION_SEC 20
bool state = LOW;

unsigned long prevTime;
volatile int revolutions = 0;
int maxRevolutions;

 
// the setup routine runs once when you press reset:
void SpeedSetup() {
  pinMode(REEDSWITCH_PIN, INPUT);
  pinMode(BREAK_PIN, INPUT);
  pinMode(EFFECT_PIN, INPUT);
  
 // maxRevolutions = (int)((MAXSPEED *  WAIT_TIME_MS)/ (WHEEL_DIAMETER * 3.1415 * 1000.0));
 // if (DEBUG) Serial.println(maxRevolutions);
  attachInterrupt(digitalPinToInterrupt(REEDSWITCH_PIN),addRevolutions,CHANGE);
  attachInterrupt(digitalPinToInterrupt(BREAK_PIN),breakApplied,CHANGE);
  attachInterrupt(digitalPinToInterrupt(EFFECT_PIN),changeEffects,CHANGE);

  prevTime = millis();
}

 void SpeedLoop() {

   // Restrict range of revolution to value between 0 - 1024
   if ((millis() - prevTime) > WAIT_TIME_MS){
     gSPEED_VAL = map(revolutions, 0, MAX_REVOLUTION_SEC, 0, 1024);
     gSPEED_KPH =  map(revolutions, 0, MAX_REVOLUTION_SEC, 0, WHEEL_DIAMETER * 3.1415 * 3.600 * MAX_REVOLUTION_SEC );
     if (gSPEED_VAL > 0)
        gDISTANCE += gSPEED_KPH / 3600;
    
    if (DEBUG) {
      Serial.println("CT");
      Serial.print(revolutions);
      Serial.print(" S");
      Serial.println(gSPEED_VAL);
    }
    
    prevTime=millis();
    revolutions =0;
  }
}

void changeEffects() {
 
 if(ColorEffect++ > 9)
   ColorEffect = 0;
 turnOnGreen(digitalRead(EFFECT_PIN) == HIGH);

}

void breakApplied() {
 gBreakON= (digitalRead(BREAK_PIN) == LOW);
 
 if (DEBUG) Serial.print(gBreakON?"B":"F"); 
 turnOnRed(gBreakON);
}

void addRevolutions(){
  if (digitalRead(REEDSWITCH_PIN) == HIGH)
    return;
  if (DEBUG) Serial.print("-");
   
  revolutions++;
  
}

