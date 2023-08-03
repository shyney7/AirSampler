#include <Arduino.h>
// !!!! IMPORTANT !!!
// You CAN'T write to the SD card while USB is connected - yeah, that sucks.

String SysID="AirSampler";

#define _SMARTDEBUG
#include <smartdebug.h>
#include <WiFi.h>

// USE THESE FOR SFC CALIBRATION //
// extra für den Christian :) //

const uint8_t SFC_MaxFlow=180; // 217 ~ 7l - directly written to the DAC [0...255] //AS4:180; I:180;

const uint32_t SFC_MeasureDelay=5e3; // at this time, we calculate the actual flow [ms]
const float SFC_CorrFactor=1.02; // komischer Fucktor der experimentell ermittelt werden will (auslitern) :-( AS4:1.02; 

// END OF SFC CALIBRATION STUFF //

const uint32_t maxSampleRuntime=50e3;  //Abbruchbedingung bei z.B. verstopften Röhrchen

constexpr bool ignoreMissingHardware=false; // debugging -- yay!
constexpr bool dontWaitForGPS=false;
constexpr bool dontCheckSD=false;

const uint32_t purgeTimePerValve=2e3;
const bool purgeResetsValveCounter=HIGH;

#include "globals.h"
#include "helpers.h"
#include "gps.h"
#include "eeeprom.h"

#include "MCP_PinAssignments.h" // refers to stuff in valveLogic
#include "valvelogic.h"

#include "EPD_screen_full.h"
#include "EPD_screen_purge.h"
#include "bootscreen.h"
#include "ERROR_NOSD.h"
#include "HardwareError.h"
#include "epaperDisplay.h"

#include "sdcard.h"

#include "bme280.h"
#include "SFC.h"
#include "motorFan.h"

#include "neopixl.h"

#include "tickker.h"

#include "butts.h"

#include "lateHelpers.h"

#include "testmode.h"
#include "SFC_testmode.h"

void purgeMode(){
  ticker.detach();

  LED.show();
  LED.setPixelColor(0,LED.Color(0,0,0));
  LED.show();

  MotoFanInit();
  MCPInit();

  motoFan.digitalWrite(1,HIGH); // fan on

  EPD_clear();
  EPD_DisplayPurge();

  G_LEDcol=LED.Color(0,0xaa,0x33);
  ticker.attach(0.5,blinkLED);

  G_valveCounter=1;
  MCPSwitchOneCHOn();

  delay(500);

  motoFan.digitalWrite(0,HIGH); // pump ON

  for(uint8_t t=1;t<=32;t++){
    G_valveCounter=t;
    MCPSwitchOneCHOn();
    EPD_DisplayCurrentValvePURGE(t);
    delay(purgeTimePerValve);
  }

  motoFan.digitalWrite(0,LOW); // pump OFF
  motoFan.digitalWrite(1,LOW); // fan OFF

  delay(1000);

  MCPAllPortsOff();

  if(purgeResetsValveCounter){
    EEPROM_resetValveCounter();
  }

  ESP.restart();
}

void purgeButLongPressService(){
  purgeMode();
}

void systemInEndstate(){
  if(G_valveCounter<32){
    return;
  }

  ticker.detach();

  LED.show();
  LED.setPixelColor(0,LED.Color(0,0,0));
  LED.show();

  EPD_clear();
  EPD_DisplayEndState();

  G_LEDcol=LED.Color(0xaa,0,0xaa);

  ticker.attach(0.7,blinkLED);

  but.attachDuringLongPress(purgeButLongPressService);

  for(;;){
    yield();
    but.tick();
  }
  
}

void turnWifiOff(){
  WiFi.mode(WIFI_OFF);  // WIFI off
  btStop();             // BT OFF
}

void SetSysID(){
  String mac;
  mac=WiFi.macAddress();
  mac.replace(":","");
  SysID=SysID+mac.substring(6);
  dprintln("[SetSysID] This System's ID: "+SysID);
}

void setup(){

  // PowerLED at first, so we can do some blinking :)
  LED.begin();
  LED.setBrightness(255);
  G_LEDcol=LED.Color(0,0,255); // default 
  ticker.attach(0.7,blinkLED);

  pinMode(BUTTON,INPUT_PULLUP);
  pinMode(BUTSFC_UP,INPUT);
  pinMode(BUTSFC_DOWN,INPUT);
  pinMode(BUTPUMPONOFF,INPUT);

  dinit(115200);

	ddelay(444);

	dprint("\n\n[SETUP] --STARTING--\n");

  SetSysID();

  turnWifiOff();

 	pinMode(BOARDLED,OUTPUT);
  digitalWrite(BOARDLED,LOW);

  pinMode(buzz,OUTPUT);
  digitalWrite(buzz,HIGH);

	// Wire.begin(I2C_SDA,I2C_SCL,100000UL);
  Wire.begin(I2C_SDA,I2C_SCL);

  Wire.setClock(50000); // go slow for long cables

  EEPROM_init();

  if(digitalRead(BUTTON)==LOW){
    dprintfln(F("Resetting Valve Counter to 1"));
    EEPROM_resetValveCounter();
  }

  if(digitalRead(BUTPUMPONOFF)==LOW){
    TestMode();
  }

  if(digitalRead(BUTSFC_UP)==LOW){
    SFC_testmode();
  }

	EEPROM_readStuff();

  EPD_init();

  systemInEndstate(); // blocking!!

  checkI2CHardwareAttatched(); // in case of error - this blocks!

  MCPInit();

  MotoFanInit();

  SFC_init();

  BME_init();

  // GPS on Serial2
  GPSInit();

  initSDCard();

  ticker.detach();

  LED.show();
  LED.setPixelColor(0,LED.Color(0,0,0));
  LED.show();

  /*
  dprintln("[SETUP] Millis right now: "+String(millis()));

  while(millis()<15000){ // give the boot screen time to shine
    delay(1);
  }
  */

  EPD_clear();
  EPD_DrawStartupScreen();

  ticker.attach(0.7,blinkRedGreen);

  // Enable all Buttons
  but.attachClick(butClick);
  butPump.attachClick(butPumpClick);
  butSFCup.attachClick(butSFCupClick);
  butSFCdown.attachClick(butSFCdownClick);

  SFC_UpdateEPD();

  dprintln("[SETUP] DONE.");
}

void loop(){
  but.tick();
  butPump.tick();
  butSFCup.tick();
  butSFCdown.tick();
  autoFanOff();
  valveClosingLogic(); // epaperDisplay - don't ask
  GPSFeedTheEncoder();
  EPD_DrawCurrentTimeAndDate();
  // SFC_UpdateDAC();
  SFC_DebugSense();
  // SD_WriteSD();
}
