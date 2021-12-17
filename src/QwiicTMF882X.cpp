/*
  This is a library for...
  By: Elias Santistevan
  Date: 
  License: This code is public domain but you buy me a beer if you use this and 
  we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
 */

#include "QwiicTMF882X.h"

QwiicTMF882X::QwiicTMF882X(){} 

bool QwiicTMF882X::begin(uint8_t address,  TwoWire &wirePort )
{
  return  ( commsBegin(address, wirePort) ? true : false );
}

bool QwiicTMF882X::beginSpi(uint8_t userPin, uint32_t spiPortSpeed, SPIClass &spiPort)
{

 return ( commsBeginSpi(userPin, spiPortSpeed, spiPort)  ? true : false ); //spi from COMMS lib - change name to remove confusion and conlict

}

uint16_t QwiicTMF882X::getAppID(){

  uint8_t tempReg[2]; 
  uint16_t id; 

  COMMS_STATUS_t result = readMultiRegisters(TMF882X_APPID_MAJOR, tempReg, 2);
  if( result != COMMS_SUCCESS ) 
    return static_cast<uint16_t>(TMF882X_GEN_ERROR); 

  id = tempReg[0] << 8; 
  id |= tempReg[1]; 

  return id; 
  
}

//Address 0x04, Bits[7:0]
//Returns zero if there are no errors. 
uint8_t QwiicTMF882X::getAppStatus(){

  uint8_t tempData; 
  COMMS_STATUS_t result = readRegister(TMF882X_APP_STAT, &tempData);
  if( result != COMMS_SUCCESS )
    return static_cast<uint8_t>(TMF882X_GEN_ERROR); 

  return tempData;
}

//Address 0xE3, Bits[5:0]
//Should return 0x08
uint8_t QwiicTMF882X::getDeviceID(){

  uint8_t tempData; 
  COMMS_STATUS_t result = readRegister(TMF882X_DEVICE_ID, &tempData);
  if( result != COMMS_SUCCESS )

    return static_cast<uint8_t>(TMF882X_GEN_ERROR); 

  return (tempData & 0x3F);
}

// Address 0xE0: Bit [6]
uint8_t QwiicTMF882X::getDeviceStatus(){

  uint8_t tempData; 

  COMMS_STATUS_t result = readRegister(TMF882X_ENABLE, &tempData);
  if( result != COMMS_SUCCESS )
    return static_cast<uint8_t>(TMF882X_GEN_ERROR); 

  return (tempData >> 6);
  
}


//0x04 Bits[7:0]
uint8_t QwiicTMF882X::getTemp(){

  uint8_t tempData; 

  COMMS_STATUS_t result = readRegister(TMF882X_TEMP, &tempData);
  if( result != COMMS_SUCCESS )
    return static_cast<uint8_t>(TMF882X_GEN_ERROR); 

  return tempData;
}

//0x05 Bits[7:0]
uint8_t QwiicTMF882X::getMeasStat(){
	uint8_t tempData;

	COMMS_STATUS_t result = readRegister(TMF882X_MEAS_STAT, &tempData);
	if( result != COMMS_SUCCESS)
		return static_cast<uint8_t>(TMF882X_GEN_ERROR);

	return tempData;
}

COMMS_STATUS_t QwiicTMF882X::setCommand(uint8_t command){

	COMMS_STATUS_t result = writeRegister(TMF882X_CMD_STAT, command);
	return result;

}
