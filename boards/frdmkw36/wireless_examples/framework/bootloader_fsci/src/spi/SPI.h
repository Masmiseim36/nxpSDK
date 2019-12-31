/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

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
