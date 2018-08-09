/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __SPI_H__
#define __SPI_H__


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "main.h"


/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#define  assert_cs()   {BOOT_SPI_SSEL_PCOR |= 1 << BOOT_SPI_SSEL_PIN_Number_c;}
#define  deassert_cs() {BOOT_SPI_SSEL_PSOR |= 1 << BOOT_SPI_SSEL_PIN_Number_c;}

#ifndef gBoot_UseSpiSlave_d
#define gBoot_UseSpiSlave_d 1
#endif

#ifndef gSpi_DummyChar_d
#define gSpi_DummyChar_d            (0xFF)
#endif

#ifndef gSPISlaveDapTxLogicOne_c
#define gSPISlaveDapTxLogicOne_c      (0)
#endif

#ifndef gSpiSlave_RxBufferSize_d
#define gSpiSlave_RxBufferSize_d   (2100)
#endif


/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */

/*! @brief SPI clock polarity configuration.*/
typedef enum _spi_clock_polarity {
    kSpiClockPolarity_ActiveHigh = 0,   /*!< Active-high SPI clock (idles low).*/
    kSpiClockPolarity_ActiveLow = 1     /*!< Active-low SPI clock (idles high).*/
} spi_clock_polarity_t;

/*! @brief SPI clock phase configuration.*/
typedef enum _spi_clock_phase {
    kSpiClockPhase_FirstEdge = 0,       /*!< First edge on SPSCK occurs at the middle of the first cycle of a data transfer.*/
    kSpiClockPhase_SecondEdge = 1       /*!< First edge on SPSCK occurs at the start of the first cycle of a data transfer.*/
} spi_clock_phase_t;

/*! @brief SPI data shifter direction options.*/
typedef enum _spi_shift_direction {
    kSpiMsbFirst = 0,    /*!< Data transfers start with most significant bit.*/
    kSpiLsbFirst = 1    /*!< Data transfers start with least significant bit.*/
} spi_shift_direction_t;

typedef uint8_t  spi_status_t;
typedef struct spi_user_config_tag{
    uint32_t busFrequencyKHz;
    spi_clock_polarity_t polarity;
    spi_clock_phase_t phase;
    spi_shift_direction_t direction;
}spi_user_config_t;


/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
void spi_master_init(uint32_t instance);
void spi_master_configure_bus(uint32_t instance, const spi_user_config_t * device);
spi_status_t spi_master_transfer(uint32_t instance,
                                 const uint8_t * sendBuffer,
                                 uint8_t * receiveBuffer,
                                 uint32_t transferByteCount);

void spi_slave_init(void (*calback)(uint32_t), uint32_t param);
uint16_t spi_slave_read(uint8_t* pData, uint16_t count);
void spi_slave_sync_write(uint8_t* pData, uint16_t count);

#endif
