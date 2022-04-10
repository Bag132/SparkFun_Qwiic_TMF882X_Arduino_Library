// SparkFun_TMF882X_Library.h
//
// This is a library written for SparkFun Qwiic TMF882X boards
//
// SparkFun sells these bpards at its website: www.sparkfun.com
//
// Do you like this library? Help support SparkFun. Buy a board!
//
// <<TODO>> Insert board links
//
//
// Written by Kirk Benell @ SparkFun Electronics, April 2022
//
// This library provides an abstract interface to the underlying TMF882X
// SDK that is provided by AMS.
//
// Repository:
//     >>TODO  Link
//
// Documentation:
//     >>TODO  Link
//
// SparkFun code, firmware, and software is released under the MIT
// License(http://opensource.org/licenses/MIT).
//
// SPDX-License-Identifier: MIT
//
//    The MIT License (MIT)
//
//    Copyright (c) 2022 SparkFun Electronics
//    Permission is hereby granted, free of charge, to any person obtaining a
//    copy of this software and associated documentation files (the "Software"),
//    to deal in the Software without restriction, including without limitation
//    the rights to use, copy, modify, merge, publish, distribute, sublicense,
//    and/or sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following conditions: The
//    above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
//    "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
//    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// This is really just a thin wrapper class that provides an Arduino interface to
// the underlying C++/C implementation

// Include our implementation class
#include "qwiic_tmf882x.h"
#include "sfe_arduino_c.h"

// Arduino things
#include <Wire.h>

class SparkFun_TMF882X : public QwDevTMF882X {

public:
    // Default noop constructor
    SparkFun_TMF882X() {};

    ///////////////////////////////////////////////////////////////////////
    // begin()
    //
    // This method is called to initialize the TMF882X library and connection to
    // the TMF882X device. This method must be called before calling any other method.
    //
    // This method follows the standard startup pattern in SparkFun Arduino
    // libraries.
    //
    //  Parameter   Description
    //  ---------   ----------------------------
    //  wirePort    optional. The Wire port. If not provided, the default port is used
    //  address     optional. I2C Address. If not provided, the default address is used.
    //  retval      true on success, false on startup failure

    bool begin(TwoWire& wirePort = Wire, uint8_t deviceAddress = kDefaultTMF882XAddress)
    {
        // Setup  I2C object and pass into the superclass
        _i2cBus.init(wirePort);
        setCommBus(_i2cBus, deviceAddress);

        // Initialize the system - return results
        return this->QwDevTMF882X::init();
    };

    ///////////////////////////////////////////////////////////////////////
    // setOutputDevice()
    //
    // This method is called to provide an output Serial device that the 
    // is used to output messages from the underlying AMS SDK.
    //
    // This is helpful when debug or info messages are enabled in the library
    //
    //  Parameter   Description
    //  ---------   ----------------------------
    //  theStream   The output stream device - normally a Serial port.

    void setOutputDevice(Stream& theStream)
    {
        sfe_set_output_device((void*)&theStream);
    }

private:
    QwI2C _i2cBus;
};
