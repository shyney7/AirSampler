#pragma once

#define VALVEOPENTIMEINSECS 5
// #define FANNACHLAUF 5*60*1000
#define FANNACHLAUF 60e3

const uint32_t SFC_timeout=180e3;

bool G_valveState=LOW;
uint8_t G_valveCounter=1;

bool G_MotoStatus=LOW;

bool G_endStateReached=LOW;

uint8_t buzz=12;

// Struct for all data that will be written to SD

struct{
  bool writeThisData=LOW; // Flag - set to HIGH to start the writing process
  uint8_t valve;
  
  float tempAtStart;
  float humAtStart;
  float presAtStart;

  uint32_t millisAtStart;
  
  uint16_t yearAtStart;
  uint8_t monthAtStart;
  uint8_t dayAtStart;

  uint8_t hourAtStart;
  uint8_t minuteAtStart;
  uint8_t secondAtStart;
  
  double latAtStart;
  double lonAtStart;
  
  double speedAtStart;
  double altAtStart;

  float SFCCurrentFlowAtStart;
  
  // Middle Values

  uint8_t   SFCFlow_RAW;
  float     SFCFlow_volts;
  float     SFCFlow_corrFactor;
  float     SFCFlow_liters;
  float     SFCFlow_liters_corrected;
  uint32_t  SFCFlow_calculatedTime;
  
  // End Values
  
  float tempAtEnd;
  float humAtEnd;
  float presAtEnd;

  uint32_t millisAtEnd;

  uint16_t yearAtEnd;
  uint8_t monthAtEnd;
  uint8_t dayAtEnd;

  uint8_t hourAtEnd;
  uint8_t minuteAtEnd;
  uint8_t secondAtEnd;

  double latAtEnd;
  double lonAtEnd;

  double speedAtEnd;
  double altAtEnd;

  float SFCCurrentFlowAtEnd;

} DataForSD;

/*
Not in this Struct:
  G_SFCCurrentFlow
  G_valveCounter
*/

// Board LED
#define BOARDLED 19 /* not a pin - only onboard */


// I2C Pins
#define I2C_SDA   21
#define I2C_SCL   22

#define sfcMaxVolInLiters 10.0
// float G_SFCCurrentFlow=sfcMaxVolInLiters / 2;
float G_SFCCurrentFlow=sfcMaxVolInLiters;


// I2C / BME280
#define BME280_ADRESS 0x76

#include <BMx280I2C.h>
BMx280I2C bme280(BME280_ADRESS);


// Buttons
#define BUTTON        39
#define BUTSFC_UP     36
#define BUTSFC_DOWN   34
#define BUTPUMPONOFF  33

#include <OneButton.h>
OneButton but(BUTTON,true,true);
OneButton butPump(BUTPUMPONOFF,true,true);
OneButton butSFCup(BUTSFC_UP,true,true);
OneButton butSFCdown(BUTSFC_DOWN,true,true);

// I2C / DAAD // PCF8591
#define DAAD_ADRESS 0x48
#define PCF8591_REF_VOLT 5.0

#include <Adafruit_PCF8591.h>
Adafruit_PCF8591 daad=Adafruit_PCF8591();

// EEPROM
#define EEPROM_SIZE 64
#include <EEPROM.h>


// ePaper Display
#define EPD_CS    5
#define EPD_DC    17
#define EPD_RST   16
#define EPD_BUSY  4
#define EPD_MOSI  23
#define EPD_SCLK  18

#define ENABLE_GxEPD2_GFX 1
#include <GxEPD2_BW.h>

#define MAX_DISPLAY_BUFFER_SIZE 6000UL

#define GxEPD2_TYPE GxEPD2_290_T94_V2 /* works for ALL Versions */

// display.height() and display.width() won't work - use these consts instead
constexpr uint16_t epd_WIDTH=122;
constexpr uint16_t epd_HEIGHT=250;

GxEPD2_BW <GxEPD2_TYPE,GxEPD2_TYPE::HEIGHT> display(GxEPD2_TYPE(EPD_CS,EPD_DC,EPD_RST,EPD_BUSY));

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include "uniBlackEx28.h" // biiiig Nubas
#include "univ9.h"


// GPS
#define GPSSERIAL_TX  26
#define GPSSERIAL_RX  25

#include <TinyGPS++.h>
TinyGPSPlus gps;

// Motor / Fan
#include <Adafruit_MCP23017.h>

#define MotoFan_ADRESS 0x22 /* MCP23017 - REV1*/
Adafruit_MCP23017 motoFan;


// NeoPixel

#define LEDPIN 27
#define NUMPIXELS 2

#include <Adafruit_NeoPixel.h>

// Adafruit_NeoPixel LED=Adafruit_NeoPixel(NUMPIXELS,LEDPIN,NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel LED=Adafruit_NeoPixel(NUMPIXELS,LEDPIN,NEO_RGB + NEO_KHZ800);

#define LEDSTATUSNORMAL LED.Color(0,64,0)
uint32_t G_LEDcol=LEDSTATUSNORMAL; // Color used by the blinking LED


// SD Card
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#define SD_SPEED 25000000

#define SD_CS     13
#define SD_MOSI   15
#define SD_MISO   2
#define SD_SCLK   14

SPIClass spiSD(HSPI);

// TestMode

bool TestMode_fanStat=LOW;
bool TestMode_pumpStat=LOW;

// Ticker
#include <Ticker.h>
Ticker ticker;

// Valve Controller

// I2C / ValveController

#define MCPA_ADRESS 0x20
#define MCPB_ADRESS 0x21 /*may change in production*/

#include <Adafruit_MCP23017.h>
Adafruit_MCP23017 mcpA;
Adafruit_MCP23017 mcpB; // not yet done, but will always be there

// Wifi - just to switch it OFF
#include <WiFi.h>
