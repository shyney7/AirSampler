#pragma once

uint32_t SD_MillisAtFistHalf=0;
uint32_t SD_MillisAtSecondHalf=0;

double GPS_Start_lat=0;
double GPS_Start_lng=0;

void die_NoSD();

String SD_buildHeader(){

  String head="HS DUESSELDORF - Umweltmesstechnik -- "+SysID+"\n";

  head+="Valve";head+="\t";

  head+="Time [UTC]";head+="\t";
  head+="Date [UTC]";head+="\t";

  head+="Start Temp";head+="\t";
  head+="Start Hum";head+="\t";
  head+="Start Pres";head+="\t";

  head+="Start Lat";head+="\t";
  head+="Start Long";head+="\t";
  head+="Start Speed";head+="\t";
  head+="Start Alt";head+="\t";

  head+="MFC RAW";head+="\t";
  head+="MFC Volt";head+="\t";
  head+="MFC Flow";head+="\t";
  head+="MFC CorFactor";head+="\t";
  head+="MFC Flow Corrected";head+="\t";
  head+="MFC calculated Time";head+="\t";

  head+="End Temp";head+="\t";
  head+="End Hum";head+="\t";
  head+="End Pres";head+="\t";

  head+="End Lat";head+="\t";
  head+="End Long";head+="\t";
  head+="End Speed";head+="\t";
  head+="End Alt";head+="\t";

  head+="Distance";head+="\t";

  head+="Duration";head+="\t";

  head+="MFC Volume";head+="\n";

  // ---------------------------------------------

  head+="";head+="\t";

  head+="";head+="\t";
  head+="";head+="\t";

  head+="[C]";head+="\t";
  head+="[%relH]";head+="\t";
  head+="[hPa]";head+="\t";

  head+="";head+="\t";
  head+="";head+="\t";
  head+="[km/h]";head+="\t";
  head+="[m]";head+="\t";
	
  head+="[digits 8 bit]";head+="\t";
  head+="[0...5V]";head+="\t";
  head+="[l/min]";head+="\t";
  head+="";head+="\t";
  head+="[l/min]";head+="\t";
  head+="[ms]";head+="\t";	

  head+="[C]";head+="\t";
  head+="[%relH]";head+="\t";
  head+="[hPa]";head+="\t";

  head+="";head+="\t";
  head+="";head+="\t";
  head+="[km/h]";head+="\t";
  head+="[m]";head+="\t";

  head+="[m]";head+="\t";

  head+="[ms]";head+="\t";

  head+="[l]"; // head+="\n";
	
	return head;

}

String SD_BuildDataFirstHalf(){

	String s="\n";
	
	s+=String(DataForSD.valve);s+="\t";

	s+=withLeadingZero(DataForSD.hourAtStart);s+=":";	
	s+=withLeadingZero(DataForSD.minuteAtStart);s+=":";	
	s+=withLeadingZero(DataForSD.secondAtStart);s+="\t";	

	s+=withLeadingZero(DataForSD.dayAtStart);s+=".";	
	s+=withLeadingZero(DataForSD.monthAtStart);s+=".";	
	s+=String(DataForSD.yearAtStart);s+="\t";	
	
	s+=String(DataForSD.tempAtStart,1);s+="\t";
	s+=String(DataForSD.humAtStart,1);s+="\t";
	s+=String((DataForSD.presAtStart/100.0),1);s+="\t";
	
	s+=String(DataForSD.latAtStart,6);s+="\t";
	s+=String(DataForSD.lonAtStart,6);s+="\t";
	s+=String(DataForSD.speedAtStart,1);s+="\t";
	s+=String(DataForSD.altAtStart,1);s+="\t";
	
	return s;

}

String SD_BuildDataSecondHalf(){

  String s="";
	
	s+=String(DataForSD.tempAtEnd,1);s+="\t";
	s+=String(DataForSD.humAtEnd,1);s+="\t";
	s+=String((DataForSD.presAtEnd/100.0),1);s+="\t";
	
	s+=String(DataForSD.latAtEnd,6);s+="\t";
	s+=String(DataForSD.lonAtEnd,6);s+="\t";
	s+=String(DataForSD.speedAtEnd,1);s+="\t";
	s+=String(DataForSD.altAtEnd,1);s+="\t";
	
	double distance=gps.distanceBetween(DataForSD.latAtStart,DataForSD.lonAtStart,DataForSD.latAtEnd,DataForSD.lonAtEnd);

  s+=String(distance,3);s+="\t";
  s+=String(DataForSD.millisAtEnd-DataForSD.millisAtStart);s+="\t";
	
	s+=String(DataForSD.SFCCurrentFlowAtEnd,1);
	
	return s;

}

String SD_BuildmiddlePart(){
  String s="";
  
  s+=String(DataForSD.SFCFlow_RAW);s+="\t";
  s+=String(DataForSD.SFCFlow_volts,2);s+="\t";
  s+=String(DataForSD.SFCFlow_liters,2);s+="\t";
  s+=String(DataForSD.SFCFlow_corrFactor,2);s+="\t";
  s+=String(DataForSD.SFCFlow_liters_corrected,2);s+="\t";
  s+=String(DataForSD.SFCFlow_calculatedTime);s+="\t";

  return s;


}

void SD_write(uint8_t whatHalf){

  String fname="/"+SysID+"_";
  fname+=String(DataForSD.yearAtStart-2000);
  fname+="-";
  fname+=withLeadingZero(DataForSD.monthAtStart);
  fname+="-";
  fname+=withLeadingZero(DataForSD.dayAtStart);
  fname+=".csv";

  // actually write something...

  boolean doNuFile=false;

  dprintfln("Filename: "+fname);

  if(!SD.exists(fname.c_str())){
    doNuFile=true;
  }

  if(doNuFile){
		String head=SD_buildHeader();
    File nuFile=SD.open(fname,FILE_WRITE);
    dprintfln(F("That's a new file"));
    dprintfln(F("Header: "));
    dprint(head);
    nuFile.print(head);
    nuFile.close();
  }

	String s="";
	
	switch (whatHalf){
		case 1:
			s=SD_BuildDataFirstHalf();
			dprintfln(F("calling SD_BuildDataFirstHalf"));
			break;
		case 0:
			dprintfln(F("calling SD_BuildDataSecondHalf"));
			s=SD_BuildDataSecondHalf();
			break;
    case 2:
      dprintfln(F("calling SD_BuildmiddlePart"));
      s=SD_BuildmiddlePart();
      break;
    case 3:
      dprintfln(F("Error message maxSampleTime"));
      s="calculated Sample Time runs longer than maxSampleTime - aborting!";
      break;
	}

  dprint(s);

  File dataFile=SD.open(fname,FILE_APPEND);
  dataFile.print(s);
  dataFile.close();

  dprintfln(F(" --- END ---"));
}

void SD_updateStartTimeValues(){ // gets called in butClick

  DataForSD.millisAtStart=millis();

  dprintf(F("Getting Start Time Values..."));

  DataForSD.valve=G_valveCounter;
  
  bme280.measure();

  do{
    delay(50);
  }while(!bme280.hasValue());

  DataForSD.tempAtStart=bme280.getTemperature();
  DataForSD.humAtStart=bme280.getHumidity();
  DataForSD.presAtStart=bme280.getPressure();

  DataForSD.SFCCurrentFlowAtStart=G_SFCCurrentFlow;

  DataForSD.yearAtStart=gps.date.year();
  DataForSD.monthAtStart=gps.date.month();
  DataForSD.dayAtStart=gps.date.day();

  DataForSD.hourAtStart=gps.time.hour();
  DataForSD.minuteAtStart=gps.time.minute();
  DataForSD.secondAtStart=gps.time.second();

  DataForSD.latAtStart=gps.location.lat();
  DataForSD.lonAtStart=gps.location.lng();
  DataForSD.altAtStart=gps.altitude.meters();
  DataForSD.speedAtStart=gps.speed.kmph();

  dprintln("done.");
}

void SD_updateEndTimeValues(){ // called in valveClosingLogic

  dprintf(F("Getting Values..."));

  DataForSD.millisAtEnd=millis();
  
  bme280.measure();

  do{
    delay(50);
  }while(!bme280.hasValue());

  DataForSD.tempAtEnd=bme280.getTemperature();
  DataForSD.humAtEnd=bme280.getHumidity();
  DataForSD.presAtEnd=bme280.getPressure();

  DataForSD.SFCCurrentFlowAtEnd=G_SFCCurrentFlow;

  DataForSD.yearAtEnd=gps.date.year();
  DataForSD.monthAtEnd=gps.date.month();
  DataForSD.dayAtEnd=gps.date.day();

  DataForSD.hourAtEnd=gps.time.hour();
  DataForSD.minuteAtEnd=gps.time.minute();
  DataForSD.secondAtEnd=gps.time.second();

  DataForSD.latAtEnd=gps.location.lat();
  DataForSD.lonAtEnd=gps.location.lng();
  DataForSD.altAtEnd=gps.altitude.meters();
  DataForSD.speedAtEnd=gps.speed.kmph();

  DataForSD.writeThisData=HIGH; // triggers the actual Write

  dprintln(F("OK"));
}

void initSDCard(){

  if(dontCheckSD){
    dprintfln(F("ignoring SD start check"));
    return;
  }

  dprintfln(F(" -- START --"));

  spiSD.begin(SD_SCLK,SD_MISO,SD_MOSI,SD_CS);
  
  if(!SD.begin(SD_CS,spiSD,SD_SPEED)){
    dprintfln(F("Card Mount Failed"));
    // return;
    die_NoSD();
  }
  
  uint8_t cardType=SD.cardType();

  if(cardType==CARD_NONE){
    dprintfln(F("No SD card attached -- Returning"));
    // return;
    die_NoSD();
  }

  dprintf("SD Card Type: ");
  
  if(cardType == CARD_MMC){
      dprintln("MMC");
  } else if(cardType == CARD_SD){
      dprintln("SDSC");
  } else if(cardType == CARD_SDHC){
      dprintln("SDHC");
  } else {
      dprintln("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("[initSDCard] SD Card Size: %lluMB\n", cardSize);

  dprintfln(F("-- END --"));
}
