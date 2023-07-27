#pragma once

// void die(String DeathMsg="[ERROR] Fatal Error - ending program. Please reset me."){
void die(String DeathMsg){ // forwarded in bme280
  dprintln(DeathMsg);
  pinMode(BOARDLED,OUTPUT);

  G_LEDcol=LED.Color(64,0,0); 
  ticker.detach(); // just to be sure
  ticker.attach(0.3,blinkLED);

  for(;;){
    digitalWrite(BOARDLED,!digitalRead(BOARDLED));
    delay(200);
  }
}

void die_NoSD(){

  if(ignoreMissingHardware){
    dprintln("[die_NoSD] SD Card missing. Ingnoring...");
    return;
  }
  
  dprintln("[die_NoSD] No SD Card inserted. PLease Reset me...");
  pinMode(BOARDLED,OUTPUT);

  G_LEDcol=LED.Color(128,0,0); 
  ticker.detach(); // just to be sure
  ticker.attach(0.2,blinkLED);

  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.drawInvertedBitmap(0,0,ERROR_NOSD,122,250,GxEPD_BLACK);
  display.display(false); // true >> partitial Mode

  for(;;){
    digitalWrite(BOARDLED,!digitalRead(BOARDLED));
    delay(200);
    if(SD.begin(SD_CS,spiSD,SD_SPEED)){
      ESP.restart();
    }
  }
}

byte CheckI2CAdress(int adr){
  Wire.beginTransmission(adr);
  return Wire.endTransmission();
}

void checkI2CHardwareAttatched(){
  
  bool MCPA_status=LOW;
  bool MCPB_status=LOW;
  bool DAAD_status=LOW; // SFC
  bool MotoFan_status=LOW;
  bool BME_status=LOW;

  if(CheckI2CAdress(MCPA_ADRESS)==0){
    MCPA_status=HIGH;
  }

  if(CheckI2CAdress(MCPB_ADRESS)==0){
    MCPB_status=HIGH;
  }

  if(CheckI2CAdress(DAAD_ADRESS)==0){
    DAAD_status=HIGH;
  }

  if(CheckI2CAdress(MotoFan_ADRESS)==0){
    MotoFan_status=HIGH;
  }

  if(CheckI2CAdress(BME280_ADRESS)==0){
    BME_status=HIGH;
  }

  dprintln("\n[checkI2CHardwareAttatched] MCPA_status: "+String(MCPA_status));
  dprintln("[checkI2CHardwareAttatched] MCPB_status: "+String(MCPB_status));
  dprintln("[checkI2CHardwareAttatched] DAAD_status: "+String(DAAD_status));
  dprintln("[checkI2CHardwareAttatched] MotoFan_status: "+String(MotoFan_status));
  dprintln("[checkI2CHardwareAttatched] BME_status: "+String(BME_status)+"\n");

  if(MCPA_status && MCPB_status && DAAD_status && MotoFan_status && BME_status){
    return;
  }

  if(ignoreMissingHardware){
    dprintln("[checkI2CHardwareAttatched] Missing Hardware found - ignoring");
    return;
  }

  uint8_t startLineInPx=85;
  uint8_t lineHeightInPx=25;

  display.fillScreen(GxEPD_WHITE);
  display.drawInvertedBitmap(0,0,ScreenHardwareError,122,250,GxEPD_BLACK);

  display.setFont(&FreeSans9pt7b);
  display.setTextColor(GxEPD_BLACK);

  if(!MCPA_status){
    display.setCursor(2,startLineInPx);
    display.print("ValveBoard A");
    startLineInPx=startLineInPx+lineHeightInPx;
  }

  if(!MCPB_status){
    display.setCursor(2,startLineInPx);
    display.print("ValveBoard B");
    startLineInPx=startLineInPx+lineHeightInPx;
  }

  if(!DAAD_status){
    display.setCursor(2,startLineInPx);
    display.print("MFC Board");
    startLineInPx=startLineInPx+lineHeightInPx;
  }

  if(!MotoFan_status){
    display.setCursor(2,startLineInPx);
    display.print("Pump/Fan");
    startLineInPx=startLineInPx+lineHeightInPx;
  }

  if(!BME_status){
    display.setCursor(2,startLineInPx);
    display.print("BME280");
    startLineInPx=startLineInPx+lineHeightInPx;
  }

  display.display(false);
  
  die("[checkI2CHardwareAttatched] Catastrophic I2C Device Error!!\n\nCan't continue like that.");
}
