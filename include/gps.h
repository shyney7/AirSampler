#pragma once

#include "epaperDisplay.h"

// GPS

void GPSLifeSignDebug(){

  static unsigned long GPSLifeSignLastTime=millis();
  
  if((unsigned long)(millis()-GPSLifeSignLastTime)>2000){
    GPSLifeSignLastTime=millis();
    dprint("\nGPS Chars Processed: ");
    dprintln(String(gps.charsProcessed()));
    dprint("GPS LONG: ");
    dprintln(String(gps.location.lng()));
    dprint("GPS LAT: ");
    dprintln(String(gps.location.lat()));

    dprint("GPS Time: ");
    dprint(withLeadingZero(gps.date.day()));
    dprint(".");
    dprint(withLeadingZero(gps.date.month()));
    dprint(".");
    dprint(String(gps.date.year()));
    dprint(" ");
    dprint(withLeadingZero(gps.time.hour()));
    dprint(":");
    dprint(withLeadingZero(gps.time.minute()));
    dprint(":");
    dprint(withLeadingZero(gps.time.second()));
    dprintln("");
  }
}

void GPSInit(){

  if(dontWaitForGPS){
    dprintfln(F("Ignoring GPS startup"));
    return;
  }
  
  dprintfln(F("START"));

  Serial2.begin(9600,SERIAL_8N1,GPSSERIAL_RX,GPSSERIAL_TX);

  dprintfln(F("waiting for valid data..."));
  
  while(!gps.date.isValid() || gps.date.year()<2021 || gps.date.year()>2079){ // BLOCKING!!!
    if(Serial2.available()){
      while(Serial2.available()){
        digitalWrite(BOARDLED,HIGH);
        gps.encode(Serial2.read());
        digitalWrite(BOARDLED,LOW);
      }
    }
    GPSLifeSignDebug();
    delay(1);
  }

  dprint("[InitGPSData] Current Date: ");

  dprint(withLeadingZero(gps.date.day()));
  dprint(".");
  dprint(withLeadingZero(gps.date.month()));
  dprint(".");
  dprint(String(gps.date.year()));
  dprint(" - ");
  dprint(withLeadingZero(gps.time.hour()));
  dprint(":");
  dprint(withLeadingZero(gps.time.minute()));
  dprint(":");
  dprintln(withLeadingZero(gps.time.second()));

  // display.fillScreen(GxEPD_WHITE);
  // display.display(false);
  
  dprintln("[InitGPSData] DONE");
}

void GPSFeedTheEncoder(){
  if(Serial2.available()){
    while(Serial2.available()){
      gps.encode(Serial2.read());
    }
  }
}
