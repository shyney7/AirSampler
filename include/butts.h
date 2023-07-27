#pragma once

void saveCurrentStuffToEEPROM(byte thisValve);

void butClick(){

  if(G_valveCounter>31){ // no more switch on when all probes filled up
    return;
  }
  
  if(G_valveState==HIGH){ // Valve OPEN
    return;
  }

  if(!G_MotoStatus){ // can only be clicked if MOTOR is on
    return;
  }

  // this part runs only while the valve is CLOSED
  EPD_open();
  G_valveState=HIGH;
  MCPSwitchOneCHOn();
  SD_updateStartTimeValues();
  saveCurrentStuffToEEPROM(G_valveCounter+1);
  ticker.detach(); // just to be sure
  G_LEDcol=LEDSTATUSNORMAL; 
  ticker.attach(0.5,blinkLED);

}

void butPumpClick(){

if(G_valveCounter>31){ // no more switch on when all probes filled up
    return;
  }
	
  dprintln("[butPumpClick]");
  G_MotoStatus=!G_MotoStatus;
	
	updateMotoController();

  if(G_MotoStatus){
    ticker.detach();
    LED.show();
    LED.setPixelColor(0,LEDSTATUSNORMAL);
    LED.show();
    BYPASSVALVE;
  }else{
    ticker.detach();
    ticker.attach(0.4,blinkRedGreen);
    MCPAllPortsOff();
  }
  
}

void butSFCupClick(){

  if(G_MotoStatus){ // can only be clicked if MOTOR is on
    return;
  }
  
  dprint("[butSFCupClick] G_SFCCurrentFlow if now at ");
  if(G_SFCCurrentFlow<sfcMaxVolInLiters){
    G_SFCCurrentFlow+=1;
  }
  dprintln(String(G_SFCCurrentFlow));
  saveCurrentStuffToEEPROM(G_valveCounter);
  SFC_UpdateEPD();
}

void butSFCdownClick(){
  if(G_MotoStatus){
    return;
  }
  
  dprint("[butSFCdownClick] G_SFCCurrentFlow if now at ");
  if(G_SFCCurrentFlow>1){
    G_SFCCurrentFlow-=1;
  }
  dprintln(String(G_SFCCurrentFlow));
  saveCurrentStuffToEEPROM(G_valveCounter);
  SFC_UpdateEPD();
}
