/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*! \file main_baremetal.c
    \brief Bare metal implementation of sensor fusion on FRDM-K64F.

    This file shows the recommended way to incorporate sensor fusion capabilities
    into a bare metal (no RTOS) project.
*/

// KSDK  and ISSDK Headers
#include "fsl_debug_console.h"  // KSDK header file for the debug interface
#include "board.h"              // KSDK header file to define board configuration
#include "pin_mux.h"            // KSDK header file for pin mux initialization functions
#include "clock_config.h"       // KSDK header file for clock configuration
#include "fsl_port.h"           // KSDK header file for Port I/O control
#include "fsl_i2c.h"            // KSDK header file for I2C interfaces
#include "fsl_smc.h"
#include "register_io_i2c.h"
#include "fsl_i2c_cmsis.h"

#include "fsl_pit.h"            // KSDK header feile for Periodic Interval Timer
#include "fxas21002.h"          // Register and bit-field definitions
#include "mpl3115.h"            // Register and bit-field definitions
#include "fxos8700.h"           // Register and bit-field definitions

// Sensor Fusion Headers
#include "sensor_fusion.h"      // top level magCal and sensor fusion interfaces
#include "control.h"  	        // Command/Streaming interface - application specific
#include "status.h"   	        // Status indicator interface - application specific
#include "drivers.h"  	        // NXP sensor drivers OR customer-supplied drivers
#include "driver_pit.h"         // Project-specific - PIT is used to control main() timing loop

registerDeviceInfo_t i2cBusInfo = {
    .deviceInstance     = I2C_S_DEVICE_INDEX,
    .functionParam      = SMC,
    .idleFunction       = (registeridlefunction_t) SMC_SetPowerModeWait
};

// Global data structures
SensorFusionGlobals sfg;                ///< This is the primary sensor fusion data structure
struct ControlSubsystem controlSubsystem;      ///< used for serial communications
struct StatusSubsystem statusSubsystem;        ///< provides visual (usually LED) status indicator
struct PhysicalSensor sensors[3];              ///< This implementation uses up to 3 sensors

/// This is a bare-metal implementation of the NXP sensor fusion demo build.
int main(void)
{
    ARM_DRIVER_I2C* I2Cdrv = &I2C_S_DRIVER;   // defined in the <shield>.h file
    uint16_t i=0;                       // general counter variable
    BOARD_InitPins();                   // defined in pin_mux.c, initializes pkg pins
    BOARD_BootClockRUN();               // defined in clock_config.c, initializes clocks
    BOARD_InitDebugConsole();           // defined in board.c, initializes the OpenSDA port

    I2Cdrv->Initialize(I2C_S_SIGNAL_EVENT);           // Initialize the KSDK driver for the I2C port
    I2Cdrv->PowerControl(ARM_POWER_FULL);      // Set the I2C Power mode.
    I2Cdrv->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);      // Configure the I2C bus speed

    initializeControlPort(&controlSubsystem);                           // configure pins and ports for the control sub-system
    initializeStatusSubsystem(&statusSubsystem);                        // configure pins and ports for the status sub-system
    initSensorFusionGlobals(&sfg, &statusSubsystem, &controlSubsystem); // Initialize sensor fusion structures
    // "install" the sensors we will be using
#if F_USING_ACCEL || F_USING_MAG
    sfg.installSensor(&sfg, &sensors[0], FXOS8700_I2C_ADDR,  1, (void*) I2Cdrv, &i2cBusInfo, FXOS8700_Init,  FXOS8700_Read);
#endif
#if F_USING_GYRO
    sfg.installSensor(&sfg, &sensors[1], FXAS21002_I2C_ADDR, 1, (void*) I2Cdrv, &i2cBusInfo, FXAS21002_Init, FXAS21002_Read);
#endif
#if F_USING_PRESSURE
    sfg.installSensor(&sfg, &sensors[2], MPL3115_I2C_ADDR,   1, (void*) I2Cdrv, &i2cBusInfo, MPL3115_Init,   MPL3115_Read);
#endif
    sfg.initializeFusionEngine(&sfg);	        // This will initialize sensors and magnetic calibration

    pit_init(1000000/FUSION_HZ);                // pitIsrFlag will be set true at FUSION_HZ periodic intervals

    sfg.setStatus(&sfg, NORMAL);                // If we got this far, let's set status state to NORMAL
    while (true)
    {
        if (true == pitIsrFlag) {               // Check whether occur interupt and toggle LED
            sfg.readSensors(&sfg, 1);           // Reads sensors, applies HAL and does averaging (if applicable)
            sfg.conditionSensorReadings(&sfg);  // magCal is run as part of this
            sfg.runFusion(&sfg);                // Run the actual fusion algorithms
            sfg.applyPerturbation(&sfg);        // apply debug perturbation (testing only)
            sfg.loopcounter++;                  // The loop counter is used to "serialize" mag cal operations
            i=i+1;
            if (i>=4) {                         // Some status codes include a "blink" feature.  This loop
                    i=0;                        // should cycle at least four times for that to operate correctly.
                    sfg.updateStatus(&sfg);     // This is where pending status updates are made visible
            }

            sfg.queueStatus(&sfg, NORMAL);      // assume NORMAL status for next pass through the loop
            sfg.pControlSubsystem->stream(&sfg, sUARTOutputBuffer);      // Send stream data to the Sensor Fusion Toolbox
            pitIsrFlag = false;                 // Reset the flag for the next cycle
        }
    }
}
/// \endcode
