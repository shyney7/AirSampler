#pragma once

void blinkLED(){

  static bool tickState=LOW;
  
	LED.show();
  if(tickState){
    LED.setPixelColor(0,G_LEDcol);
  }else{
    LED.setPixelColor(0,LED.Color(0,0,0));
  }
  tickState=!tickState;
  LED.show();
}

void blinkRedGreen(){
  static bool redGreen=HIGH;

  LED.show();
  if(redGreen){
    LED.setPixelColor(0,LEDSTATUSNORMAL);
  }else{
    LED.setPixelColor(0,LED.Color(64,0,0));
  }
  redGreen=!redGreen;
  LED.show();
}
