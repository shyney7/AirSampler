#pragma once

void SFC_UpdateDAC(){ // loop
	
	// not needed anymore: SFC is always fully open

  static float oldDACval=sfcMaxVolInLiters+1.0;

  if(oldDACval==G_SFCCurrentFlow){
    return;
  }

  oldDACval=G_SFCCurrentFlow;

  uint8_t SFC_dac= 255 * G_SFCCurrentFlow / sfcMaxVolInLiters;

  dprintln("[SFC_UpdateDAC] DAC value: "+String(SFC_dac));

  daad.analogWrite(SFC_dac);
}

void SFC_UpdateEPD(){
	
  String currentFlow=String(G_SFCCurrentFlow,0)+" l";
  
  display.fillRoundRect(3,190,116,20,5,GxEPD_WHITE);
  display.setFont(&univ9);
  display.setTextColor(GxEPD_BLACK);

  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(currentFlow,0,0,&tbx,&tby,&tbw,&tbh);
  uint16_t x = ((122-tbw)/2)-tbx;
  display.setCursor(x,206);

  display.print(currentFlow);
  display.setFont(&FreeSans12pt7b);
  display.display(true); // true >> partitial Mode

}

void SFC_UpdateEPD_(){

  String currentFlow=String(G_SFCCurrentFlow,1);
  
  display.fillRect(0,200,122,16,GxEPD_WHITE);
  display.setFont(&univ9);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0,216);
  display.print("Spl. Vol: "+currentFlow+" l");
  display.setFont(&FreeSans12pt7b);
  display.display(true); // true >> partitial Mode

}

void SFC_DebugSense(uint16_t interval=500){ // loop
  static unsigned long AsyncTaskLastTime=0;

  if(!G_MotoStatus){
    return;
  }

  if((unsigned long)(millis()-AsyncTaskLastTime)<interval){
    return;
  }

  AsyncTaskLastTime=millis();

  float SFC_senseInV=int_to_volts(daad.analogRead(0),8,PCF8591_REF_VOLT);

  float liter=SFC_senseInV+SFC_senseInV;

  // dprintln("[SFC_DebugSense] SFC Sense: "+String(SFC_senseInV,1)+"V");
  dprintfln("SFC Sense: "+String(liter,1)+"l");
}

void SFC_init(){
  // not much to do here....
  daad.begin(DAAD_ADRESS);
  daad.enableDAC(true);
	delay(50);
  // daad.analogWrite(0xff); // always completly open
  daad.analogWrite(SFC_MaxFlow);
}
