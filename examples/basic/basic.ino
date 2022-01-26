/*
*****************************************************************************
* Copyright by ams AG                                                       *
* All rights are reserved.                                                  *
*                                                                           *
* IMPORTANT - PLEASE READ CAREFULLY BEFORE COPYING, INSTALLING OR USING     *
* THE SOFTWARE.                                                             *
*                                                                           *
* THIS SOFTWARE IS PROVIDED FOR USE ONLY IN CONJUNCTION WITH AMS PRODUCTS.  *
* USE OF THE SOFTWARE IN CONJUNCTION WITH NON-AMS-PRODUCTS IS EXPLICITLY    *
* EXCLUDED.                                                                 *
*                                                                           *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         *
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS         *
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT          *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
*****************************************************************************
*/

//#include "QwiicTMF882X.h"
#include "tmf882x_interface.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "mcu_tmf882x_config.h"
//#include "inc\platform_wrapper.h"
#include <Wire.h>

#define TMF882X_I2C_ADDR 0x41
#define PWR_EN_PIN 10

static struct tmf882x_tof tof;
static struct platform_ctx ctx = {
		ctx.i2cdev = NULL, 
		ctx.debug = 0,
    ctx.i2c_addr = TMF882X_I2C_ADDR,
    ctx.debug = 0,
		ctx.mode_8x8 = 0,
    ctx.tof = &tof
};


void setup(){

	Wire.begin();
	Serial.begin(115200);

}

void loop()
{
    int32_t rc = 0;
    uint32_t exit_num_results = 0;
    struct tmf882x_mode_app_calib tof_calib = {0};

    platform_wrapper_power_off();
    platform_wrapper_power_on();

    // Init application mode on device
    if (platform_wrapper_init_device(&ctx, 0, 0)) 
        Serial.print("Error loading application mode\n");

    rc = platform_wrapper_cfg_device(&ctx);

    if (rc) {
        Serial.print("Error configuring device: ");
				Serial.println(rc);
    }

		else {
        // Function only returns if a specified number of measurements are given
        platform_wrapper_start_measurements(&ctx, exit_num_results, &tof_calib);
    }

    platform_wrapper_power_off();
}
