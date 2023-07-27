#pragma once

void die(String DeathMsg="[ERROR] Fatal Error - ending program. Please reset me.");

void BME_init(){
  if(!bme280.begin()){
    die("[BME_init] Cannot initialize BME280 - program halted");
  }

  bme280.resetToDefaults();

  bme280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
  bme280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);
  bme280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
  bme280.measure();

  // this is pretty fugly
  do{
    delay(100);
  }while(!bme280.hasValue());

  dprint("[BME_init] Pressure: "); dprintln(String(bme280.getPressure()));
  dprint("[BME_init] Temperature: "); dprintln(String(bme280.getTemperature()));
  dprint("[BME_init] Humidity: "); dprintln(String(bme280.getHumidity()));

  bme280.measure();
}
