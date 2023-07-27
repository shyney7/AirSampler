// ePaper Display is 2.13", 250x122 pixel, 2 grey levels

// https://github.com/Xinyuan-LilyGO/T5-Ink-Screen-Series
// http://www.lilygo.cn/prod_view.aspx?TypeId=50031&Id=1149&FId=t3:50031:3
// LilyGo T5 V2.3  // SKU: H239

#pragma once

#include "gps.h"
#include "sdcard.h"
#include "bootscreen.h"
#include "EPD_screen_full.h"
#include "EPD_screen_purge.h"
#include "MCP_PinAssignments.h"

void EPD_clear(){
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.display(false); // true >> partitial Mode
}

void EPD_DisplayPurge(){
  dprintfln(F("EPD display PURGE"));
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.drawBitmap(0,0,Screen_EPD_purge,epd_WIDTH,epd_HEIGHT,GxEPD_WHITE);
  display.display(false); // true >> partitial Mode
}

void EPD_closed(){
  dprintfln(F("Status changed to CLOSED"));
  display.fillRect(0,121,122,30,GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE);
  display.setCursor(18,144);
  // display.print("CLOSED");
  display.print("READY");
  display.display(true); // true >> partitial Mode
}

void EPD_open(){
  dprintfln(F("Status changed to OPEN"));
  display.fillRect(0,121,122,30,GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE);
  display.setCursor(28,144);
  display.print("OPEN");
  display.display(true); // true >> partitial Mode
}

void EPD_DisplayCurrentValvePURGE(uint8_t thisValve){
  dprintfln("Current Valve: "+String(thisValve));
  display.fillRoundRect(4,90,114,76,5,GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&uniBlackEx28);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(String(thisValve),0,0,&tbx,&tby,&tbw,&tbh);
  uint16_t x = ((122-tbw)/2)-tbx;
  
  display.setCursor(x,146);
  display.print(thisValve);
  display.setFont(&FreeSans12pt7b);
  display.display(true); // true >> partitial Mode
}

void EPD_DisplayCurrentValve(){
  dprintfln("Current Valve: "+String(G_valveCounter));
  display.fillRoundRect(2,28,118,92,5,GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&uniBlackEx28);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(String(G_valveCounter),0,0,&tbx,&tby,&tbw,&tbh);
  uint16_t x = ((122-tbw)/2)-tbx;
  
  display.setCursor(x,96);
  display.print(G_valveCounter);
  display.setFont(&FreeSans12pt7b);
  display.display(true); // true >> partitial Mode
}

void EPD_DisplayEndState(){
  dprintfln(F("EPD display END"));
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.drawBitmap(0,0,Screen_EPD_full,epd_WIDTH,epd_HEIGHT,GxEPD_WHITE);
  display.display(false); // true >> partitial Mode
}

void EPD_DrawCountdownBar(int16_t timeLeftInSecs, int16_t timeMaxInSecs){
  // if(timeLeftInSecs>VALVEOPENTIMEINSECS){
  if(timeLeftInSecs>timeMaxInSecs){
    return;
  }

  if(timeLeftInSecs<1){ // redraw can take a looong time - don't redraw in the last second
    return;
  }

  dprintfln("Time Left: "+String(timeLeftInSecs)+" / "+String(timeMaxInSecs));
  
  const static int16_t countdownBarX=10;
  const static int16_t countdownBarY=108;
  const static int16_t countdownBarW=102;
  const static int16_t countdownBarH=6;

  // uint8_t countdownBarL=(VALVEOPENTIMEINSECS-timeLeftInSecs)*(countdownBarW/VALVEOPENTIMEINSECS);
  int16_t countdownBarL=(timeLeftInSecs+1)*((float)countdownBarW/(timeMaxInSecs+1));
  // dprintfln("countdownBarL: "+String(countdownBarL));
  
  display.fillRect(countdownBarX,countdownBarY,countdownBarW,countdownBarH,GxEPD_WHITE);
  display.drawRect(countdownBarX,countdownBarY,countdownBarW,countdownBarH,GxEPD_BLACK);
  display.fillRect(countdownBarX,countdownBarY,countdownBarL,countdownBarH,GxEPD_BLACK);
  display.display(true); // true >> partitial Mode
}

void systemEndState(){ // not used
  G_MotoStatus=LOW;
  motoFan.digitalWrite(0,G_MotoStatus); // not the nice way to kill the pump
  delay(100);

  uint16_t bitmaskReadback=0;

  do{
    mcpA.writeGPIOAB(0);
    delay(2);
    bitmaskReadback=mcpA.readGPIOAB();
  }while(bitmaskReadback!=0);

  do{
    mcpB.writeGPIOAB(0);
    delay(2);
    bitmaskReadback=mcpB.readGPIOAB();
  }while(bitmaskReadback!=0);
  
  EPD_DisplayEndState();
  G_endStateReached=HIGH;
}

void valveClosingLogic(){ // function runs while valve is OPEN
  if(G_valveState==LOW){ // Valve CLOSED
    return;
  }

  static bool exitOnError=LOW;

  static bool setNuOfftimer=LOW;
  static bool doFirstDataWrite=LOW;
  static uint32_t firstDataWriteTimer=0;
  static uint32_t OffTimer=0;
  static bool measureCycle=LOW;
  static int16_t EPD_BarMaxTImeInSecs=0;

  if(millis()>firstDataWriteTimer && doFirstDataWrite==HIGH){
    doFirstDataWrite=LOW;
    SD_write(1);
    dprintfln(F("First Data Part written"));
  }

  if(setNuOfftimer==LOW){
    OffTimer=millis()+SFC_MeasureDelay;
    firstDataWriteTimer=millis()+600;
    setNuOfftimer=HIGH;
    doFirstDataWrite=HIGH;
    measureCycle=HIGH;
    EPD_BarMaxTImeInSecs=(int16_t)SFC_MeasureDelay/1000;
  }

  if(millis()>OffTimer && measureCycle==HIGH){
    measureCycle=LOW;
		
		DataForSD.SFCFlow_RAW=daad.analogRead(0);

    float SFC_senseInV=int_to_volts(DataForSD.SFCFlow_RAW,8,PCF8591_REF_VOLT);
		DataForSD.SFCFlow_volts=SFC_senseInV;
		
    dprintf("Reading ");
    dprint(SFC_senseInV);
    dprintln("V on the SFC ADC.");

    float VolStr=SFC_senseInV*2; // 5V for 10l - so it's simply *2
		
		DataForSD.SFCFlow_liters=VolStr;

    dprintf("VoLStr: ");
    dprint(VolStr);
    dprintln("l/m");
		
		DataForSD.SFCFlow_corrFactor=SFC_CorrFactor; // set in globals
		DataForSD.SFCFlow_liters_corrected=VolStr*SFC_CorrFactor;

		dprintf("VoLStr corrected: ");
    dprint(DataForSD.SFCFlow_liters_corrected);
    dprintln("l/m");

    uint32_t SFC_time=G_SFCCurrentFlow/DataForSD.SFCFlow_liters_corrected*60000UL;
    EPD_BarMaxTImeInSecs=(int16_t)(G_SFCCurrentFlow/DataForSD.SFCFlow_liters_corrected*60)-(SFC_MeasureDelay/1000);

    dprintf(F("SFC_time: "));
    dprint(SFC_time);
    dprintln(F("ms"));
		
		DataForSD.SFCFlow_calculatedTime=SFC_time;

    OffTimer=millis()-SFC_MeasureDelay+SFC_time;
    
    dprintf(F("OffTimer: "));
    dprint(OffTimer);
    dprintln(F("ms"));
		
		SD_write(2);

    if(SFC_time > maxSampleRuntime){
      SD_write(3); // throws an error message in log file
      dprintfln(F("runtime is longer than maxSapleRuntime -- aborting!"));
      OffTimer=millis()+100; // just to be sure :)
      exitOnError=HIGH;
      for(uint8_t t=0;t<10;t++){
        digitalWrite(buzz,LOW);
        delay(200);
        digitalWrite(buzz,HIGH);
        delay(100);
      }
    }else{
      exitOnError=LOW;
    }
  }

  if(millis()>OffTimer && measureCycle==LOW){
    dprintfln(F("Closing Valve"));
    SD_updateEndTimeValues();
    ticker.detach();
    G_valveState=LOW;
    setNuOfftimer=LOW;
    G_valveCounter++;
    MCPAllPortsOff();
    BYPASSVALVE; // Should always be open while no measurements.

    if(exitOnError==LOW){
      SD_write(0); // sdcard
    }
		
    if(G_valveCounter>=32){ // all probes filled

      motoFan.digitalWrite(0,LOW); // pump OFF
      motoFan.digitalWrite(1,LOW); // fan OFF
    
      delay(1000);
    
      MCPAllPortsOff();
    
      ESP.restart();
      
      // systemEndState();
    }else{
      EPD_closed();
      EPD_DisplayCurrentValve();
    }

    // PowerLED to green
    LED.setPixelColor(0,LEDSTATUSNORMAL);
    LED.show();
    LED.setPixelColor(0,LEDSTATUSNORMAL);
    LED.show();

    return;
  }

  static uint16_t timerLastTimeUpdate=0;

  if(timerLastTimeUpdate!=(OffTimer-millis())/1000){
    timerLastTimeUpdate=(OffTimer-millis())/1000;
    EPD_DrawCountdownBar(timerLastTimeUpdate,EPD_BarMaxTImeInSecs);
    // dprintfln("Time Left: "+String(timerLastTimeUpdate)+" / "+String(EPD_BarMaxTImeInSecs));
  }
}

void EPD_DrawCurrentTimeAndDate(){
  static uint8_t oldMinute=99;

   if(G_valveCounter>31){
    return;
  }

  if(G_valveState){ // do not update Date/Time while valve is open - this takes about 1 sec.
    return;  
  }

  if(oldMinute==(uint8_t)gps.time.minute()){
    return;
  }

  oldMinute=gps.time.minute();

  // stuff from here on runs every minute

  dprintln("[EPD_DrawCurrentTimeAndDate] updating current time displayed");

  String GPS_Time=withLeadingZero(gps.time.hour())+":"+withLeadingZero(gps.time.minute());
  String GPS_Date=withLeadingZero(gps.date.day())+"."+withLeadingZero(gps.date.month())+"."+String(gps.date.year());

  String temperature=String(bme280.getTemperature(),1);

  dprintln("[EPD_DrawCurrentTimeAndDate] GPS_Time: "+GPS_Time);
  dprintln("[EPD_DrawCurrentTimeAndDate] GPS_Date: "+GPS_Date);
  dprintln("[EPD_DrawCurrentTimeAndDate] Temperature: "+temperature);

  display.fillRect(0,217,122,32,GxEPD_WHITE);
  display.setFont(&univ9);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0,233);
  display.print("Temp: "+temperature);
  display.setCursor(0,249);
  display.print(GPS_Time+" | "+GPS_Date);
  display.setFont(&FreeSans12pt7b);
  display.display(true); // true >> partitial Mode
}

void EPD_DrawStartupScreen(){
  dprintfln(F("drawing startup screen..."));

  display.fillRect(0,0,122,122,GxEPD_BLACK);

  display.setTextColor(GxEPD_WHITE);

  display.setCursor(12,22);
  display.print("SAMPLE");
	
	display.fillRect(0,160,122,56,GxEPD_BLACK);

  display.setCursor(3,181);
  display.print("Sample Vol");
  
  display.display(false); // true >> partitial Mode

  EPD_DisplayCurrentValve();
  EPD_closed();

  dprintfln(F("drawing startup screen DONE"));
}

void EPD_init(){
  dprintf(F("Initializing Screen..."));
  display.init();
  display.setRotation(0);
  display.setFont(&FreeSans12pt7b);
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.drawInvertedBitmap(0,0,BootScreen,epd_WIDTH,epd_HEIGHT,GxEPD_BLACK);
  display.display(false); // true >> partitial Mode
  dprintln(F("done."));
}
