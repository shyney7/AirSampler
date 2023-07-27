uint8_t SFC_testmode_ADCval=0;
uint8_t SFC_testmode_DACval=0;
bool SFC_testmode_pumpState=LOW;
bool SFC_testmode_valve1State=LOW;

void SFC_testmode_ButGO(){
  SFC_testmode_valve1State=!SFC_testmode_valve1State;
  dprintf("Valve 1 state: ");
  dprintln(SFC_testmode_valve1State);

  if(SFC_testmode_valve1State){
    CH01;
  }else{
    MCPAllPortsOff();
  }

}

void SFC_testmode_ButPUMP(){
  SFC_testmode_pumpState=!SFC_testmode_pumpState;
  motoFan.digitalWrite(0,SFC_testmode_pumpState);
  dprintf("pump state: ");
  dprintln(SFC_testmode_pumpState);

  uint32_t ledC=LED.Color(0,0,0);
  
  if(SFC_testmode_pumpState){
    ledC=LED.Color(0,64,0);
  }

  LED.setPixelColor(0,ledC);
  LED.show();
}

void SFC_testmode_updateAout(){
  display.fillRect(0,58,122,24,GxEPD_WHITE);
  // display.fillRect(12,58,122,24,GxEPD_BLACK);
  display.setFont(&FreeSans12pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0,80);
  display.print("DAC: "+String(SFC_testmode_DACval));
  display.display(true); // true >> partitial Mode
}

void SFC_testmode_updateAin(){

  static uint32_t lastTime=0;
  static uint8_t oldDACval=SFC_testmode_DACval+1;

  if(oldDACval==SFC_testmode_DACval){
    // return;  
  }

  if((uint32_t)(millis()-lastTime)<2000){
    return;
  }

  oldDACval=SFC_testmode_DACval;
  lastTime=millis();
  
  display.fillRect(0,36,122,24,GxEPD_WHITE);
  // display.fillRect(0,36,122,24,GxEPD_BLACK);
  display.setFont(&FreeSans12pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0,55);
  display.print("ADC: "+String(SFC_testmode_ADCval));
  display.display(true); // true >> partitial Mode
}

void SFC_Testmode_debug(){
  dprint("ADC: ");
  dprint(SFC_testmode_ADCval);
  dprint("\t");

  dprint("Vin: ");
  dprint(int_to_volts(SFC_testmode_ADCval,8,PCF8591_REF_VOLT));
  dprint("\t");

  float lperm=int_to_volts(SFC_testmode_ADCval,8,PCF8591_REF_VOLT)*2.0;

  dprint("l/m: ");
  dprint(String(lperm,2));
  dprint("\t");

  float fac16=lperm*1.6;

  dprint("Fac1.6: ");
  dprint(String(fac16,2));
  dprint("\n");

  // int_to_volts(daad.analogRead(0),8,PCF8591_REF_VOLT);

  // int_to_volts(uint16_t dac_value, uint8_t bits, float logic_level);
}

void SFC_testmode_readADC(){
  static uint32_t lastTime=0;

  if((uint32_t)(millis()-lastTime)<100){
    return;
  }

  lastTime=millis();

  do{
    SFC_testmode_ADCval=daad.analogRead(0);
    delay(10);
  }while(SFC_testmode_ADCval==255);

  SFC_Testmode_debug();
}

void SFC_Testmode_updateDAC(){
  dprintfln("DAC: "+String(SFC_testmode_DACval));
  daad.analogWrite(SFC_testmode_DACval);
}

void SFC_Testmode_ButMFCplus(){
  SFC_testmode_DACval+=10;
  SFC_Testmode_updateDAC();
  SFC_testmode_updateAout();
}

void SFC_Testmode_ButMFCminus(){
  SFC_testmode_DACval-=10;
  SFC_Testmode_updateDAC();
  SFC_testmode_updateAout();
}

void SFC_testmode(){
  dprintln("\n-- SFC TESTMODE --\nPress RESET to exit\n");

  ticker.detach();

  display.init();
  display.setRotation(0);
  display.setFont(&FreeSans12pt7b);
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);

  display.setCursor(0,22);
  display.print("SFC TEST");

  display.drawRoundRect(105,130,99,24,4,GxEPD_BLACK);
  display.setCursor(110,146);
  display.print("+");

  display.drawRoundRect(105,162,99,24,4,GxEPD_BLACK);
  display.setCursor(112,180);
  display.print("-");

  display.display(false); // true >> partitial Mode

  MotoFanInit();

  MCPInit();

  LED.show();
  LED.setPixelColor(0,LED.Color(0,0,0));
  LED.show();

  daad.begin(DAAD_ADRESS);
  daad.enableDAC(true);

  butSFCup.attachClick(SFC_Testmode_ButMFCplus);
  butSFCdown.attachClick(SFC_Testmode_ButMFCminus);
  butPump.attachClick(SFC_testmode_ButPUMP);
  but.attachClick(SFC_testmode_ButGO);

  SFC_testmode_DACval=SFC_MaxFlow; 
  SFC_Testmode_updateDAC();
  SFC_testmode_updateAout();

  for(;;){ // never leave
    SFC_testmode_updateAin();
    SFC_testmode_readADC();
    but.tick();
    butPump.tick();
    butSFCup.tick();
    butSFCdown.tick();
    yield();
  }
}

void SFC_testmode_(){
  dprintln("\n-- SFC TESTMODE --\nPress RESET to exit\n");

  ticker.detach();

  display.init();
  display.setRotation(0);
  display.setFont(&FreeSans12pt7b);
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);

  display.setCursor(0,22);
  display.print("SFC TEST");

  display.drawRoundRect(105,130,99,24,4,GxEPD_BLACK);
  display.setCursor(110,146);
  display.print("+");

  display.drawRoundRect(105,162,99,24,4,GxEPD_BLACK);
  display.setCursor(112,180);
  display.print("-");

  display.display(false); // true >> partitial Mode

  SFC_testmode_updateAout();

  MotoFanInit();

  LED.show();
  LED.setPixelColor(0,LED.Color(0,0,0));
  LED.show();

  daad.begin(DAAD_ADRESS);
  daad.enableDAC(true);

  butSFCup.attachClick(SFC_Testmode_ButMFCplus);
  butSFCdown.attachClick(SFC_Testmode_ButMFCminus);
  butPump.attachClick(SFC_testmode_ButPUMP);

  for(;;){ // never leave
    SFC_testmode_updateAin();
    SFC_testmode_readADC();
    butSFCup.tick();
    butSFCdown.tick();
    butPump.tick();
    yield();
  }

}
