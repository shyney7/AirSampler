#pragma once

// TESTMODE

// this is a sub-loop construct used while testing

// hold PumpOnOff while booting to get here

void TestMode_DisplayStatus(){
  display.fillRect(0,210,122,99,GxEPD_WHITE);
  // display.setFont(&univ9);
  display.setFont(&FreeSans12pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0,228);
  display.print("FAN: "+String(TestMode_fanStat));
  display.setCursor(0,249);
  display.print("PUMP: "+String(TestMode_pumpStat));
  // display.setFont(&FreeSans12pt7b);
  display.display(true); // true >> partitial Mode
}

void TestMode_ButGO(){
  TestMode_fanStat=!TestMode_fanStat;
  dprintln("[TestMode_ButGO]  TestMode_fanStat: "+String(TestMode_fanStat));
  motoFan.digitalWrite(1,TestMode_fanStat);
  TestMode_DisplayStatus();
}

void TestMode_ButPUMP(){
  TestMode_pumpStat=!TestMode_pumpStat;
  dprintln("[TestMode_ButGO]  TestMode_pumpStat: "+String(TestMode_pumpStat));
  motoFan.digitalWrite(0,TestMode_pumpStat);
  TestMode_DisplayStatus();
}

void TestMode_ButUp(){
  if(G_valveCounter<32){
    G_valveCounter++;
    dprintln("[TestMode_ButUp] G_valveCounter: "+String(G_valveCounter));
    EPD_DisplayCurrentValve();
    MCPSwitchOneCHOn();
  }
}

void TestMode_ButDown(){
  if(G_valveCounter>1){
    G_valveCounter--;
    dprintln("[TestMode_ButDown] G_valveCounter: "+String(G_valveCounter));
    EPD_DisplayCurrentValve();
    MCPSwitchOneCHOn();
  }
}

void TestMode(){
  dprintfln("-- TESTMODE --");

  ticker.detach();

  LED.show();
  LED.setPixelColor(0,LED.Color(0,0,0));
  LED.show();

  display.init();
  display.setRotation(0);
  display.setFont(&FreeSans12pt7b);
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);

  display.display(false); // true >> partitial Mode

  display.setCursor(0,22);
  display.print("TESTING");

  display.display(true); // true >> partitial Mode


  MCPInit();

  MotoFanInit();

  but.attachClick(TestMode_ButGO);
  butPump.attachClick(TestMode_ButPUMP);
  butSFCup.attachClick(TestMode_ButUp);
  butSFCdown.attachClick(TestMode_ButDown);

  EPD_DisplayCurrentValve();
  MCPSwitchOneCHOn();
  TestMode_DisplayStatus();

  for(;;){
    but.tick();
    butPump.tick();
    butSFCup.tick();
    butSFCdown.tick();
    yield();
  }    
}
