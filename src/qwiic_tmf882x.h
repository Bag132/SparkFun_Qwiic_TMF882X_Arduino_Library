
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

// Arduino things - this needs to change once moved to framework
#include <Wire.h>

// Default I2C address for the device
#define kDefaultTMF882XAddress 0x41

#define kDefaultSampleDelayMS 500

// Flags for enable/disable output messages from the underlying SDK

#define TMF882X_MSG_INFO 0x01
#define TMF882X_MSG_DEBUG 0x02
#define TMF882X_MSG_ERROR 0x04
#define TMF882X_MSG_ALL 0x07
#define TMF882X_MSG_NONE 0x00

// define a type for the results -- just alias the underlying measurment struct - easier to type
typedef struct tmf882x_msg_meas_results TMF882XMeasurement_t;

typedef void (*TMF882XMeasurementHandler_t)(TMF882XMeasurement_t* measurment);

class QwDevTMF882X {

public:
    // Default noop constructor
    QwDevTMF882X()
        : m_isInitialized { false }
        , m_sampleDelayMS { kDefaultSampleDelayMS }
        , m_outputSettings { TMF882X_MSG_NONE }
        , m_debug { false }
        , m_messageHandlerCB{ nullptr } {};

    bool init();
    bool isConnected(); // Checks if sensor ack's the I2C request

    bool applicationVersion(char* pVersion, uint8_t vlen);
    void setMeasurementHandler(TMF882XMeasurementHandler_t handler);

    bool loadFirmware(const unsigned char *firmwareBinImage, unsigned long length);
    
    int startMeasuring(uint32_t reqMeasurements = 0, uint32_t timeout = 0);
    int startMeasuring(TMF882XMeasurement_t& results, uint32_t timeout = 0);
    void stopMeasuring(void);

    bool setFactoryCalibration(struct tmf882x_mode_app_calib* tof_calib);

    bool setCalibration(struct tmf882x_mode_app_calib* tof_calib);

    void setSampleDelay(uint16_t delay)
    {
        if (delay)
            m_sampleDelayMS = delay;
    };
    uint16_t sampleDelay(void)
    {
        return m_sampleDelayMS;
    }

    // Methods that are called from our "shim relay". They are public, but not really
    int32_t sdkMessageHandler(struct tmf882x_msg* msg);

    // access to the underlying i2c calls - used by the underlying SDK
    int32_t writeRegisterRegion(uint8_t offset, uint8_t* data, uint16_t length);
    int32_t readRegisterRegion(uint8_t reg, uint8_t* data, uint16_t numBytes);

    // method to set the communication bus this object should use
    void setCommBus(QwI2C& theBus, uint8_t id_bus);

    void setDebug(bool bEnable)
    {
        m_debug = true;
        if (m_debug)
            m_outputSettings |= TMF882X_MSG_DEBUG;
        else
            m_outputSettings &= ~TMF882X_MSG_DEBUG;
    }

    bool debug(void)
    {
        return m_debug;
    }

    void setMessageLevel(uint8_t msg)
    {
        m_outputSettings = msg;
    }
    uint8_t messageLevel(void)
    {
        return m_outputSettings;
    }

private:

    bool initializeTMF882x(void);
    int measurementLoop(uint16_t nMeasurements, uint32_t timeout);

    // Library initialized flag
    bool m_isInitialized;

    // I2C  things
    QwI2C* m_i2cBus; // pointer to our i2c bus object
    uint8_t m_i2cAddress; // address of the device

    // Delay for the read sample loop
    uint16_t m_sampleDelayMS;

    // Structure/state for the underlying TOF SDK
    tmf882x_tof m_TOF;

    // for processing messages from SDK
    uint16_t m_nMeasurements;

    // Cache of our last measurement taking
    struct tmf882x_msg_meas_results* m_lastMeasurement;

    // Flag to indicate to the system to stop measurements
    bool m_stopMeasuring;

    // Callback function pointer
    TMF882XMeasurementHandler_t m_messageHandlerCB;

    // for managing message output levels
    uint8_t m_outputSettings;
    bool m_debug;
};
