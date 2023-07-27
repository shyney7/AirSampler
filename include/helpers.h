#pragma once

union{
  float f;
  uint8_t b[4];
}float2byte;

/* String SetSysID(){
  String mac;
  mac=WiFi.macAddress();
  mac.replace(":","");
  SysID=SysID+mac.substring(6);
  dprintln("[SetSysID] This System's ID: "+SysID);
} */

float int_to_volts(uint16_t dac_value, uint8_t bits, float logic_level) {
  return(((float)dac_value/((1<<bits)-1))*logic_level);
}

String withLeadingZero(byte x){
  if (x<10){
  return "0"+String(x);
  }
  return String(x);
}


