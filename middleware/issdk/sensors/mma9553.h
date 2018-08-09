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
#ifndef MMA9553_H_
#define MMA9553_H_

#include <stdint.h>

/*! @brief Size of fixed header bytes in sensor commands. */
#define MMA9553_HDR_SIZE  (4)

/*! @brief Size of Version Info. */
#define VERSION_INFO_SIZE (12)

/*! @brief Size of Standby Registers. */
#define STANDBY_REG_SIZE (4)

/*! @brief XYZ Data Register Offset. */
#define MMA9553_XYZ_DATA_OFFSET (26)

/*! @brief The MMA9553 I2C Slave Address */
#define MMA9553_I2C_ADDRESS (0x4C)

/*! @brief The Read Version Info Command. */
extern const uint8_t ReadVersionInfo[4];

/*! @brief Configure MBOX for QR Data and XYZ Axes Command. */
extern const uint8_t MBoxQRDataXYZ[16] ;

/*! @brief Set Sensor to Legacy (free running) Mode Command. */
extern const uint8_t SetLegacyMode[5];

/*! @brief Set Sensor to Legacy Mode with INT_O interrupt on completion of the AFE sampling. */
extern const uint8_t SetLegacyIntMode[5];




/*! @brief Set sensor to Wake Mode Command. */
extern const uint8_t WakeApplication[5] ;

/*! @brief Command to reset all applications of the sensor. */
extern const uint8_t ResetApplication[5];

/*! @brief Full-Scale Range Selections. */
extern const uint8_t SetFSRange_2g[5];
extern const uint8_t SetFSRange_4g[5];
extern const uint8_t SetFSRange_8g[5];

/*! @brief Sample Rate Commands. */
extern const uint8_t SetSampleRate_488Hz[5] ;
extern const uint8_t SetSampleRate_244Hz[5] ;
extern const uint8_t SetSampleRate_122Hz[5] ;
extern const uint8_t SetSampleRate_61Hz[5]  ;
extern const uint8_t SetSampleRate_30Hz[5]  ;
extern const uint8_t SetSampleRate_15Hz[5]  ;
extern const uint8_t SetSampleRate_7Hz[5]   ;
extern const uint8_t SetSampleRate_3Hz[5]   ;

/*! @brief AFE Priority Commands. */
extern const uint8_t SetAFEPriority_for488Hz[5];
extern const uint8_t SetAFEPriority_for224Hz[5];
extern const uint8_t SetAFEPriority_for122Hz[5];
extern const uint8_t SetAFEPriority_for61Hz[5];
extern const uint8_t SetAFEPriority_for30Hz[5];
extern const uint8_t SetAFEPriority_for15Hz[5];
extern const uint8_t SetAFEPriority_for7Hz[5];
extern const uint8_t SetAFEPriority_for3Hz[5];

/*! @brief MBox Priority Commands. */
extern const uint8_t SetMBoxPriority_for488Hz[5];
extern const uint8_t SetMBoxPriority_for244Hz[5];
extern const uint8_t SetMBoxPriority_for122Hz[5];
extern const uint8_t SetMBoxPriority_for61Hz[5];
extern const uint8_t SetMBoxPriority_for30Hz[5];
extern const uint8_t SetMBoxPriority_for15Hz[5];
extern const uint8_t SetMBoxPriority_for7Hz[5];
extern const uint8_t SetMBoxPriority_for3Hz[5];

/*! @brief Command to Read Pedometer Data. */
extern const uint8_t ReadPedometerData[4] ;

#endif  /* MMA9553_H_ */ 
