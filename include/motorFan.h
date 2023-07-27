#pragma once

// I2C / Motor/Fan Control / PCF8574
// #define MotoFan_ADRESS 0x38 /* PCF8574 */

// #include <PCF8574.h>
// PCF8574 motoFan(MotoFan_ADRESS);

void updateMotoController(){
  dprintln("[updateMotoController] G_MotoStatus is now "+String(G_MotoStatus));
  delay(500);
  motoFan.digitalWrite(0,G_MotoStatus);
  delay(800);
  if(G_MotoStatus){ // off is handeled by autoFanOff()
    motoFan.digitalWrite(1,HIGH);
  }
}

void autoFanOff(){
  static bool hasFired=LOW;
  static uint32_t lastTime=millis();

  if(G_MotoStatus){
    lastTime=millis();
    hasFired=HIGH;
  }

  if(hasFired && (unsigned long)(millis()-lastTime)>FANNACHLAUF){
    dprintln("[autoFanOff] triggered");
  
    hasFired=LOW;
    motoFan.digitalWrite(1,LOW);
  }
}

void MotoFanInit(){
  dprintln("[MotoFanInit] Controller init");

  motoFan.begin(MotoFan_ADRESS-0x20,&Wire);
  
  motoFan.pinMode(0,OUTPUT);
  motoFan.pinMode(1,OUTPUT);
  motoFan.pinMode(2,OUTPUT);

  motoFan.digitalWrite(0,LOW);
  motoFan.digitalWrite(1,LOW);
  motoFan.digitalWrite(2,LOW);
}
