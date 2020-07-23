/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This is a source file which implements the driver for the AT45DB021E memory.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "Eeprom.h"

#if gEepromType_d == gEepromDevice_AT45DB041E_c

#include "SPI_Serial_Adapter.h"
#include "GPIO_Adapter.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define  gEepromWriteEnable_d   1

#define  EEPROM_RDSR            0xD7U
#define  EEPROM_READ            0x03U
#define  EEPROM_READ_HF         0x0BU

#define  EEPROM_ERASE_256B      0x81U
#define  EEPROM_ERASE_2K        0x50U

#define  EEPROM_WRITE_BYTES     0x02U

#define  EEPROM_BUSY_FLAG_MASK  0x80U
#define  EEPROM_PAGE_SIZE       (256U)
#define  EEPROM_BLOCK_SIZE      (8U*EEPROM_PAGE_SIZE)
#define  EEPROM_PAGE_MASK       (EEPROM_PAGE_SIZE - 1U)

/* adress mask */
#define  ADDRESS_MASK 0x000000FFU


/* SPI config */
#define gEepromSpiInstance_c  0U

#define gEepromAssertCS_d()   GpioClearPinOutput(&mEepromSpiCsCfg)
#define gEepromDeassertCS_d() GpioSetPinOutput(&mEepromSpiCsCfg)


/******************************************************************************
*******************************************************************************
* Private Prototypes
*******************************************************************************
******************************************************************************/
#if gEepromWriteEnable_d
static ee_err_t EEPROM_PrepareForWrite(uint32_t NoOfBytes, uint32_t Addr);
static void  EEPROM_WritePage(uint32_t NoOfBytes, uint32_t Addr, uint8_t *Outbuf);
#endif
static ee_err_t EEPROM_WaitForReady(bool_t IsInfinite);
static uint16_t EEPROM_ReadStatusReq(void);
static void     EEPROM_Command(uint8_t opCode, uint32_t Addr);


/*! *********************************************************************************
*************************************************************************************
* Private Memory Declarations
*************************************************************************************
********************************************************************************** */
static spiState_t mEepromSpiState;
static const gpioOutputPinConfig_t mEepromSpiCsCfg = {
    .gpioPort = gpioPort_C_c,
    .gpioPin = 19,
    .outputLogic = TRUE,
    .slewRate = pinSlewRate_Fast_c,
    .driveStrength = pinDriveStrength_Low_c
};

#if gEepromWriteEnable_d
static uint8_t mEepromEraseBitmap[32];
#endif


/*! *********************************************************************************
*************************************************************************************
* Public Functions
*************************************************************************************
********************************************************************************** */

/*****************************************************************************
*  EEPROM_Init
*
*  Initializes the EEPROM peripheral
*
*****************************************************************************/
ee_err_t EEPROM_Init(void)
{
    static bool_t initialized = FALSE;
    ee_err_t retval;
    uint8_t cmd[] = {0x3D, 0x2A, 0x80, 0xA6};
    spiBusConfig_t spiConfig = {
        .bitsPerSec = 8000000,
        .master = TRUE,
        .clkActiveHigh = TRUE,
        .clkPhaseFirstEdge = TRUE,
        .MsbFirst = TRUE
    };

#if gEepromWriteEnable_d
    uint32_t i;

    /* Mark Flash as Unerased */
    for(i = 0; i < NumberOfElements(mEepromEraseBitmap); i++)
    {
        mEepromEraseBitmap[i] = 0U;
    }
#endif

    if( !initialized )
    {
        (void)Spi_Init(gEepromSpiInstance_c, &mEepromSpiState, NULL, NULL);
        (void)Spi_Configure(gEepromSpiInstance_c, &spiConfig);
        (void)GpioOutputPinInit(&mEepromSpiCsCfg, 1);

        retval = EEPROM_WaitForReady(FALSE);
        if (retval != ee_ok)
        {
            return retval;
        }

        /* Set page size to 256bits: */
        gEepromAssertCS_d();
        (void)Spi_SyncTransfer(gEepromSpiInstance_c, (uint8_t *)cmd, NULL, (uint16_t)sizeof(cmd));
        gEepromDeassertCS_d();
        initialized = TRUE;
    }

    return ee_ok;
}

/*****************************************************************************
*  EEPROM_ChipErase
*
*  Erase all memory to 0xFF
*
*****************************************************************************/
ee_err_t EEPROM_ChipErase(void)
{
    uint8_t cmd[] = {0xC7, 0x94, 0x80, 0x9A};
    (void)EEPROM_WaitForReady(TRUE);

    gEepromAssertCS_d();
    (void)Spi_SyncTransfer(gEepromSpiInstance_c, (uint8_t *)cmd, NULL, (uint16_t)sizeof(cmd));
    gEepromDeassertCS_d();

    return ee_ok;
}

/*****************************************************************************
*  EEPROM_EraseBlock
*
*  Erase a block of memory to 0xFF
*
*****************************************************************************/
ee_err_t EEPROM_EraseBlock(uint32_t Addr, uint32_t size)
{
    uint8_t cmd[4];
    
    (void)EEPROM_WaitForReady(TRUE);
    
    if (size == EEPROM_BLOCK_SIZE)
    {
        cmd[0] = EEPROM_ERASE_2K;
    }
    else if(size == EEPROM_PAGE_SIZE)
    {
        cmd[0] = EEPROM_ERASE_256B;
    }
    else
    {
        return ee_error;
    }

    cmd[1] = (uint8_t)(Addr >> 16U) & ADDRESS_MASK;
    cmd[2] = (uint8_t)(Addr >>  8U) & ADDRESS_MASK;
    cmd[3] = (uint8_t)(Addr >>  0U) & ADDRESS_MASK;

    gEepromAssertCS_d();
    (void)Spi_SyncTransfer(gEepromSpiInstance_c, cmd, NULL, (uint16_t)sizeof(cmd));
    gEepromDeassertCS_d();
    
    return ee_ok;
}

/*****************************************************************************
*  EEPROM_WriteData
*
*  Writes a data buffer into EEPROM, at a given address
*
*****************************************************************************/
#if gEepromWriteEnable_d
ee_err_t EEPROM_WriteData(uint32_t NoOfBytes, uint32_t Addr, uint8_t *Outbuf)
{
    ee_err_t retval;
    
    if (NoOfBytes == 0U)
    {
        return ee_ok;
    }
    
    (void)EEPROM_WaitForReady(TRUE);
    
    retval = EEPROM_PrepareForWrite(NoOfBytes, Addr);
    if (retval != ee_ok)
    {
        return retval;
    }

    while ((Addr & EEPROM_PAGE_MASK) + NoOfBytes > EEPROM_PAGE_MASK)
    {
        uint32_t bytes = EEPROM_PAGE_SIZE - (Addr & EEPROM_PAGE_MASK);
        
        EEPROM_WritePage(bytes, Addr, Outbuf);
        NoOfBytes -= bytes;
        Addr += bytes;
        Outbuf += bytes;
    }
    
    EEPROM_WritePage(NoOfBytes, Addr, Outbuf);
    
    return ee_ok;
}
#endif

/*****************************************************************************
*  EEPROM_ReadData
*
*  Reads a data buffer from EEPROM, from a given address
*
*****************************************************************************/
ee_err_t EEPROM_ReadData(uint16_t NoOfBytes, uint32_t Addr, uint8_t *inbuf)
{
    
    (void)EEPROM_WaitForReady(TRUE);
    
    EEPROM_Command(EEPROM_READ,Addr);
    
    (void)Spi_SyncTransfer(gEepromSpiInstance_c, NULL, inbuf, NoOfBytes);
    gEepromDeassertCS_d();

    return ee_ok;
}

/*****************************************************************************
*  EEPROM_ReadStatusReq
*
*
*****************************************************************************/
uint8_t EEPROM_isBusy(void)
{
    return (EEPROM_ReadStatusReq() & EEPROM_BUSY_FLAG_MASK) == 0U?  0x01U:0x00U;
}

/*! *********************************************************************************
*************************************************************************************
* Private Functions
*************************************************************************************
********************************************************************************** */

/*****************************************************************************
*  EEPROM_ReadStatusReq
*
*
*****************************************************************************/
static uint16_t EEPROM_ReadStatusReq(void)
{
    uint8_t cmd = EEPROM_RDSR;
    uint16_t data = 0;
    
    gEepromAssertCS_d();
    (void)Spi_SyncTransfer(gEepromSpiInstance_c, (uint8_t *)&cmd, NULL, (uint16_t)sizeof(cmd));
    (void)Spi_SyncTransfer(gEepromSpiInstance_c, NULL, (uint8_t*)&data, 2);
    gEepromDeassertCS_d();

    return data;
}

/*****************************************************************************
*  EEPROM_WriteData
*
*  Writes a data buffer into the External Memory, at a given address
*
*****************************************************************************/
#if gEepromWriteEnable_d
static ee_err_t EEPROM_PrepareForWrite(uint32_t NoOfBytes, uint32_t Addr)
{
    uint32_t i;
    ee_err_t ret = ee_ok;
    uint32_t startBlk, endBlk;
    
    if ( Addr >= gEepromParams_TotalSize_c )
    {
        return ee_error;
    }

    /* Obtain the block number */
    startBlk = Addr / EEPROM_BLOCK_SIZE;
    endBlk   = (Addr + NoOfBytes) / EEPROM_BLOCK_SIZE;
    
    if( ((Addr + NoOfBytes) % EEPROM_BLOCK_SIZE) != 0U)
    {
        endBlk++;
    }
    
    for(i = startBlk; i <= endBlk; i++)
    {
        if ( (mEepromEraseBitmap[i/8U] & (1U << (i%8U) ) ) == 0U)
        {
            (void)EEPROM_EraseBlock(i * EEPROM_BLOCK_SIZE, EEPROM_BLOCK_SIZE);
            mEepromEraseBitmap[i/8U] |= 1U << (i%8U);
            (void)EEPROM_WaitForReady(TRUE);
        }
    }

    return ret;
}
#endif

/*****************************************************************************
*  EEPROM_WritePage
*
*  Writes maximum 256 bytes into a EEPROM page
*
*****************************************************************************/
#if gEepromWriteEnable_d
static void EEPROM_WritePage(uint32_t NoOfBytes, uint32_t Addr, uint8_t *Outbuf)
{ 
    (void)EEPROM_WaitForReady(TRUE);
    /*SPI_Send_byte(EEPROM_WRITE_ENABLE,DEASSERT_CS) */
    
    EEPROM_Command(EEPROM_WRITE_BYTES, Addr); /*CS will remain ASSERTED */
    
    (void)Spi_SyncTransfer(gEepromSpiInstance_c, Outbuf, NULL, (uint16_t)NoOfBytes);
    gEepromDeassertCS_d();
}
#endif

/*****************************************************************************
*  EEPROM_WaitForReady
*
*  WaitFor EEPROM Ready, IsInfinite TRUE wait for Infinite, FALSE time out 50ms
*****************************************************************************/
static ee_err_t EEPROM_WaitForReady(bool_t IsInfinite)
{
    volatile uint16_t wait = 0x400U; /* near 50 ms @ 50 us/cycle */
    
    /* Byte1 - Bit:   7       6      5:2       1         0
    *             RDY/BUSY  COMP  DENSITY  PROTECT  PAGE_SIZE
    *
    * Byte2 - Bit:   7       6    5    4    3    2    1    0
    *             RDY/BUSY  RES  EPE  RES  SLE  PS2  PS1  ES
    */
    
    while( wait != 0U )
    {
        if(EEPROM_isBusy() == 0U)
        {
            break;
        }

        if (!IsInfinite)
        {
            wait--;
        }
    }
    
    if(wait != 0U)
    {
        return ee_ok;
    }
    return ee_error;
}


/*****************************************************************************
*  EEPROM_Command
*
*
*****************************************************************************/
static void EEPROM_Command(uint8_t opCode, uint32_t Addr)
{
    uint8_t cmd[4];
    
    cmd[0] = opCode;
    cmd[1] = (uint8_t)(Addr >> 16U) & ADDRESS_MASK;
    cmd[2] = (uint8_t)(Addr >>  8U) & ADDRESS_MASK;
    cmd[3] = (uint8_t)(Addr >>  0U) & ADDRESS_MASK;

    gEepromAssertCS_d();
    (void)Spi_SyncTransfer(gEepromSpiInstance_c, cmd, NULL, (uint16_t)sizeof(cmd));
    /* CS will remain asserted to be able to send the data */
}

#endif
/* EOF: Eeprom_AT45DB041E */ 
