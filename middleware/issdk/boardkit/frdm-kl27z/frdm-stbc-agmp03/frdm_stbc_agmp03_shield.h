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

/*! @file: frdm_stbc_agmp03_shield.h
* @brief The frdm_stbc_agmp03_shield.h file declares mapping of the Kinetis
         Device peripherals to the frmd-kl27z and frmd-stbc-agmp03 shield.
*/

#ifndef _FRDM_STBC_AGMP03_SHIELD_H_
#define _FRDM_STBC_AGMP03_SHIELD_H_

/* The shield name */
#define SHIELD_NAME "FRDM-STBC-AGMP03"

// FXLS8962 Sensor Information
#define FXLS8962_I2C_ADDR 0x18
#define FXLS8962_CS D10
#define FXLS8962_MOSI D11
#define FXLS8962_MISO D12
#define FXLS8962_SCLK D13
#define FXLS8962_INT1 D2
#define FXLS8962_INT2 A0

// FXAS21002 Sensor Information
#define FXAS21002_I2C_ADDR 0x20
#define FXAS21002_INT1 D8
#define FXAS21002_INT2 A2
#define FXAS21002_CS A1

// MPL3115 Sensor Information
#define MPL3115_I2C_ADDR 0x60
#define MPL3115_INT1 D7

// MAG3110 Sensor Information
#define MAG3110_I2C_ADDR 0x0E
#define MAG3110_INT1 D5

// FRDM-STBC-AGMP03 Shield Reset
#define RESET_GPIO A3

#endif /* _FRDM_STBC_AGMP03_SHIELD_H_ */
