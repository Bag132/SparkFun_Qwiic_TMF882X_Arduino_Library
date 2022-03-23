
//////////////////////////////////////////////////////////////////////////////
//
//  This is a library written for the SparkFun AMS TMF882X ToF breakout/qwiic board
//
//  SparkFun sells these at its website: www.sparkfun.com
//  Do you like this library? Help support SparkFun. Buy a board!
//
//  https://www.sparkfun.com/products/<TODO>
//
//  Written by <TODO>
//
//
//  https://github.com/sparkfun/<TODO>
//
//  License
//     <TODO>
//
//////////////////////////////////////////////////////////////////////////////

#pragma once 

// The AMS supplied library/sdk interface
#include "inc/tmf882x.h"
#include "tmf882x_interface.h"

#include "qwiic_i2c.h"


// Default I2C address for the device
#define kDefaultTMF882XAddress 0x41

// define a type for the results -- just alias the underlying measurment struct - easier to type
typedef struct tmf882x_msg_meas_results TMF882XMeasurement_t;

typedef void (*TMF882XMeasurementHandler_t)(TMF882XMeasurement_t *measurment);

class QwDevTMF882X
{

public:

    // Default noop constructor
    QwDevTMF882X(): _isInit{false}, _msgHandlerCB{nullptr}{};

    bool begin(TwoWire &wirePort = Wire, uint8_t deviceAddress = kDefaultTMF882XAddress);
    bool isConnected(); //Checks if sensor ack's the I2C request


    void setMeasurementHandler(TMF882XMeasurementHandler_t handler);
    bool startMeasuring(uint32_t reqMeasurements);    
    bool startMeasuring(TMF882XMeasurement_t &results);
    void stopMeasuring(void);

    void printMeasurement(TMF882XMeasurement_t * results);
    void printMeasurement(TMF882XMeasurement_t &results){
        printMeasurement(&results);
    }    

    // get firmware version
    bool getAppVersion(char *pVersion, uint8_t len);
    bool setFactoryCalibration(struct tmf882x_mode_app_calib *tof_calib);

    bool setCalibration(struct tmf882x_mode_app_calib *tof_calib);

    bool init(void);

    // Methods that are called from our "shim relay". They are public, but not really
    bool _sdk_msg_handler(struct tmf882x_msg *msg);

    // method to set the communication bus this object should use
    void set_comm_bus(QwI2C &theBus, uint8_t id_bus);

    // access to the underlying i2c calls - used by the underlying SDK

    int32_t writeRegisterRegion(uint8_t offset, uint8_t *data, uint16_t length);
    int32_t readRegisterRegion(uint8_t reg, uint8_t* data, uint16_t numBytes);    

private:
    bool init_tmf882x(void);
    bool start_measuring(uint8_t nMeasurements);

    // I2C  things
    QwI2C             * _i2cBus;       // pointer to our i2c bus object
    uint8_t             _i2c_address;  // address of the device


    TMF882XMeasurementHandler_t _msgHandlerCB;

    // The AMS SDK TOF struct - used when communicating with the device
    tmf882x_tof     _tof;

    // for processing messages from SDK
    uint16_t _nMeasurements;
    struct tmf882x_msg_meas_results * _lastMeasurment;

    bool _stopMeasuring;

    bool _isInit;


};