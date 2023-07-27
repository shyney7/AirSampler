#pragma once

void EEPROM_readStuff(){

  byte checkByteA=byte(EEPROM.read(0));
  byte checkByteB=byte(EEPROM.read(1));

  if(checkByteA!=19 || checkByteB!=68){
    dprintfln(F("No valid data - using defaults"));
    return; // no valid data in EEPROM - so get outta here
  }

  dprintfln(F("Data found - using EEPROM values"));
  G_valveCounter=byte(EEPROM.read(2));
  float2byte.b[0]=byte(EEPROM.read(3));
  float2byte.b[1]=byte(EEPROM.read(4));
  float2byte.b[2]=byte(EEPROM.read(5));
  float2byte.b[3]=byte(EEPROM.read(6));

  G_SFCCurrentFlow=float2byte.f;

  dprintf(F("G_valveCounter="));
  dprintln(G_valveCounter);

}

void saveCurrentStuffToEEPROM(byte thisValve){

  float2byte.f=G_SFCCurrentFlow;
  
  EEPROM.write(0,19);
  EEPROM.write(1,68);
  EEPROM.write(2,thisValve);
  EEPROM.write(3,float2byte.b[0]);
  EEPROM.write(4,float2byte.b[1]);
  EEPROM.write(5,float2byte.b[2]);
  EEPROM.write(6,float2byte.b[3]);
  
  EEPROM.commit();
  dprintln("[saveCurrentStuffToEEPROM] Writing EEPROM data");
}

void EEPROM_resetValveCounter(){ // run in SETUP via pressing BUTTON
  // this is not a real "clear" - is simply resets the current valveCounter

  EEPROM.write(2,1);
  // EEPROM.write(2,29);
  EEPROM.commit();

  dprintfln(F("Valve Counter RESET to 1"));
}

void EEPROM_init(){
  EEPROM.begin(EEPROM_SIZE);
  dprintf(F("Init eeprom with "));
  dprint(EEPROM_SIZE);
  dprintln(F(" Bytes"));
}
