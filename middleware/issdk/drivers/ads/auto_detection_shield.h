/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

/**
 * @file auto_detection_shield.h
 * @brief The auto_detection_shield.h file describes the definitions for Sensor
 * I2C and SPI parameters required to detects the Sensor Shield.
 */

#ifndef _ADS_DETECTION_SHIELD_H_
#define _ADS_DETECTION_SHIELD_H_

// FXOS8700 Sensor Information
/*
   In order to route INT1 signal from FXOS8700 to the K64F,
   Pins 1-2 of Jumper J3 on the FRDM-FXS-MULT2-B should be connected.
   In order to route INT2 signal from FXOS8700 to the K64F,
   Pins 1-2 of Jumper J4 on the FRDM-FXS-MULT2-B should be connected.
*/
#define FXOS8700_I2C_ADDR 0x1E

// FXAS21002 Sensor Information
/*
   In order to route INT1 signal from FXAS21002 to the K64F,
   Pins 1-2 of Jumper J6 on the FRDM-FXS-MULT2-B should be connected.
*/
#define FXAS21002_I2C_ADDR 0x20

// MAG3110 Sensor Information
/*
   In order to route INT1 signal from MAG3110 to the K64F,
   Pins 2-3 of Jumper J3 on the FRDM-FXS-MULT2-B should be connected.
*/
#define MAG3110_I2C_ADDR 0x0E

// MPL3115 Sensor Information
/*
   In order to route INT1 signal from MPL3115 to the K64F,
   Pins 2-3 of Jumper J5 on the FRDM-FXS-MULT2-B should be connected.
*/
#define MPL3115_I2C_ADDR 0x60

// FXPQ3115 Sensor Information
#define FXPQ3115_I2C_ADDR 0x60

// MMA8652 Sensor Information
/*
   In order to route INT1 signal from MMA8652 to the K64F,
   Pins 2-3 of Jumper J4 on the FRDM-FXS-MULT2-B should be connected.
*/
#define MMA8652_I2C_ADDR 0x1D

// FXLS8471Q Sensor Information
/*
   In order to route INT1 signal from FXLS8471Q to the FRDM-K64F,
   Pins 2-3 of Jumper J6 on the FRDM-FXS-MULT2-B should be connected.
*/
#define FXLS8471_SPI_CS D10

// FXLS8962 Sensor Information
#define FXLS8962_I2C_ADDR 0x18
#define FXLS8962_CS D10

// FXLC95000 Sensor Information
#define FXLC95000_I2C_ADDR 0x4C

// MMA9553 Sensor Information
#define MMA9553_I2C_ADDR 0x4C

// MMA8491 Sensor Information
#define MMA8491_I2C_ADDR 0x55
#define MMA8491_EN A1

// MMA8451 Sensor Information
#define MMA8451_I2C_ADDR 0x1D

// DIFF-P Sensor Information
#define DIFF_P_I2C_ADDR 0x60
#define DIFF_P_SPI_CS D10

#endif /* _ADS_DETECTION_SHIELD_H_ */
