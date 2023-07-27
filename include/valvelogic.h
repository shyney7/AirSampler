#pragma once

void MCPAllPortsOff(){

  Wire.setClock(50000);
	
	uint16_t bitmaskReadback=0;

	do{
		mcpA.writeGPIOAB(0);
		delay(2);
		bitmaskReadback=mcpA.readGPIOAB();
	}while(bitmaskReadback!=0);

	do{
		mcpB.writeGPIOAB(0);
		delay(2);
		bitmaskReadback=mcpB.readGPIOAB();
	}while(bitmaskReadback!=0);

	// mcpA.writeGPIOAB(0); // all OFF
  // mcpB.writeGPIOAB(0); // all OFF
}

void MCPwriteOutput(bool board, uint16_t bitmask){
	uint16_t bitmaskReadback=0;
	
	if(board==0){ // mcpA
		do{
			mcpA.writeGPIOAB(bitmask);
			delay(2);
			bitmaskReadback=mcpA.readGPIOAB();
		}while(bitmaskReadback!=bitmask);
		return;
	}else{ // mcpB
		do{
			mcpB.writeGPIOAB(bitmask);
			delay(2);
			bitmaskReadback=mcpB.readGPIOAB();
		}while(bitmaskReadback!=bitmask);
		return;
	}
}

void MCPSwitchOneCHOn(){

  Wire.setClock(50000);

  MCPAllPortsOff(); // just to be on the safe side

    switch(G_valveCounter){ 
    case 1:
      CH01;
      break;
    case 2:
      CH02;
      break;
    case 3:
      CH03;
      break;
    case 4:
      CH04;
      break;
    case 5:
      CH05;
      break;
    case 6:
      CH06;
      break;
    case 7:
      CH07;
      break;
    case 8:
      CH08;
      break;
    case 9:
      CH09;
      break;
    case 10:
      CH10;
      break;
    case 11:
      CH11;
      break;
    case 12:
      CH12;
      break;
    case 13:
      CH13;
      break;
    case 14:
      CH14;
      break;
    case 15:
      CH15;
      break;
    case 16:
      CH16;
      break;
    case 17:
      CH17;
      break;
    case 18:
      CH18;
      break;
    case 19:
      CH19;
      break;
    case 20:
      CH20;
      break;
    case 21:
      CH21;
      break;
    case 22:
      CH22;
      break;
    case 23:
      CH23;
      break;
    case 24:
      CH24;
      break;
    case 25:
      CH25;
      break;
    case 26:
      CH26;
      break;
    case 27:
		  CH27;
      break;
    case 28:
      CH28;
      break;
    case 29:
      CH29;
      break;
    case 30:
      CH30;
      break;
    case 31:
      CH31;
      break;
    case 32:
      CH32;
      break;
  }
}

void MCPInit(){

  // no need to check if the chips exist - we did that before

  // adafruit wont take real I2C adresses - they start with a 0
  mcpA.begin(MCPA_ADRESS-0x20,&Wire);
  mcpB.begin(MCPB_ADRESS-0x20,&Wire);
  
  MCPAllPortsOff();

	for(uint8_t t=0;t<16;t++){
    mcpA.pinMode(t,OUTPUT);
    mcpB.pinMode(t,OUTPUT);
  }

  // BYPASSVALVE; // Should always be open while no measurements.
}
