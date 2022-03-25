/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is a source file which implements the driver for the AT45DB161E memory.
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

#if gEepromType_d == gEepromDevice_AT45DB161E_c

#include "SPI_Adapter.h"
#include "GPIO_Adapter.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define  gEepromWriteEnable_d   1

#define  EEPROM_RDSR            0xD7
#define  EEPROM_READ            0x03
#define  EEPROM_READ_HF         0x0B

#define  EEPROM_ERASE_512B      0x81
#define  EEPROM_ERASE_4K        0x50

#define  EEPROM_WRITE_BYTES     0x02

#define  EEPROM_BUSY_FLAG_MASK  0x80
#define  EEPROM_PAGE_SIZE       (512)
#define  EEPROM_BLOCK_SIZE      (8*EEPROM_PAGE_SIZE)
#define  EEPROM_PAGE_MASK       (EEPROM_PAGE_SIZE - 1)

/* adress mask */
#define  ADDRESS_MASK 0x000000FF


/* SPI config */
#define gEepromSpiInstance_c  0

#define gEepromAssertCS_d()   GpioClearPinOutput(&mEepromSpiCsCfg)
#define gEepromDeassertCS_d() GpioSetPinOutput(&mEepromSpiCsCfg)


/******************************************************************************
*******************************************************************************
* Private Prototypes
*******************************************************************************
******************************************************************************/
#if gEepromWriteEnable_d
static ee_err_t EEPROM_PrepareForWrite(uint32_t NoOfBytes, uint32_t Addr);
static ee_err_t EEPROM_WritePage(uint32_t NoOfBytes, uint32_t Addr, uint8_t *Outbuf);
#endif
static ee_err_t EEPROM_WaitForReady(void);
static uint16_t EEPROM_ReadStatusReq(void);
static void     EEPROM_Command(uint8_t opCode, uint32_t Addr);


/*! *********************************************************************************
*************************************************************************************
* Private Memory Declarations
*************************************************************************************
********************************************************************************** */
static spiState_t mEepromSpiState;
const gpioOutputPinConfig_t mEepromSpiCsCfg = {
    .gpioPort = gpioPort_C_c,
    .gpioPin = 4,
    .outputLogic = 1,
    .slewRate = pinSlewRate_Fast_c,
    .driveStrength = pinDriveStrength_Low_c
};

#if gEepromWriteEnable_d
static uint8_t mEepromEraseBitmap[64];
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
    static uint8_t initialized = 0;
    ee_err_t retval;
    const uint8_t cmd[] = {0x3D, 0x2A, 0x80, 0xA6};
    spiBusConfig_t cfg = {
        .bitsPerSec = 24000000,
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
        mEepromEraseBitmap[i] = 0;
    }
#endif

    if( !initialized )
    {
        Spi_Init(gEepromSpiInstance_c, &mEepromSpiState, NULL, NULL);
        Spi_Configure(gEepromSpiInstance_c, &cfg);
        GpioOutputPinInit(&mEepromSpiCsCfg, 1);
        
        retval = EEPROM_WaitForReady();
        if (retval != ee_ok)
            return retval;

        /* Set page size to 512bits: */
        gEepromAssertCS_d();
        Spi_SyncTransfer(gEepromSpiInstance_c, (uint8_t*)cmd, NULL, sizeof(cmd));
        gEepromDeassertCS_d();

        initialized = 1;
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
    const uint8_t cmd[] = {0xC7, 0x94, 0x80, 0x9A};
    while (EEPROM_isBusy());

    gEepromAssertCS_d();
    Spi_SyncTransfer(gEepromSpiInstance_c, (uint8_t*)cmd, NULL, sizeof(cmd));
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
    
    while (EEPROM_isBusy());
    
    switch (size)
    {
    case EEPROM_BLOCK_SIZE:
        cmd[0] = EEPROM_ERASE_4K;
        break;
    case EEPROM_PAGE_SIZE:
        cmd[0] = EEPROM_ERASE_512B;
        break;
    default:
        return ee_error;
    }

    cmd[1] = (Addr >> 16) & ADDRESS_MASK;
    cmd[2] = (Addr >>  8) & ADDRESS_MASK;
    cmd[3] = (Addr >>  0) & ADDRESS_MASK;

    gEepromAssertCS_d();
    Spi_SyncTransfer(gEepromSpiInstance_c, cmd, NULL, sizeof(cmd));
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
    
    if (NoOfBytes == 0)
        return ee_ok;
    
    while (EEPROM_isBusy());
    
    retval = EEPROM_PrepareForWrite(NoOfBytes, Addr);
    if (retval != ee_ok)
        return retval;
    
    while ((Addr & EEPROM_PAGE_MASK) + NoOfBytes > EEPROM_PAGE_MASK)
    {
        uint32_t bytes = EEPROM_PAGE_SIZE - (Addr & EEPROM_PAGE_MASK);
        
        retval = EEPROM_WritePage(bytes, Addr, Outbuf);
        NoOfBytes -= bytes;
        Addr += bytes;
        Outbuf += bytes;
        
        if (retval != ee_ok)
            return retval;
    }
    
    retval = EEPROM_WritePage(NoOfBytes, Addr, Outbuf);
    
    return retval;
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
    
    while (EEPROM_isBusy());
    
    EEPROM_Command(EEPROM_READ,Addr);
    
    Spi_SyncTransfer(gEepromSpiInstance_c, NULL, inbuf, NoOfBytes);
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
    return !(EEPROM_ReadStatusReq() & EEPROM_BUSY_FLAG_MASK);
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
    const uint8_t cmd = EEPROM_RDSR;
    uint16_t data = 0;
    
    gEepromAssertCS_d();
    Spi_SyncTransfer(gEepromSpiInstance_c, (uint8_t*)&cmd, NULL, sizeof(cmd));
    Spi_SyncTransfer(gEepromSpiInstance_c, NULL, (uint8_t*)&data, 2);
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
    
    /* Obtain the block number */
    startBlk = Addr / EEPROM_BLOCK_SIZE;
    endBlk   = (Addr + NoOfBytes) / EEPROM_BLOCK_SIZE;
    
    if( (Addr + NoOfBytes) % EEPROM_BLOCK_SIZE )
    {
        endBlk++;
    }
    
    for(i = startBlk; i <= endBlk; i++)
    {
        if ( (mEepromEraseBitmap[i/8] & (1 << (i%8) ) ) == 0)
        {
            ret = EEPROM_EraseBlock(i * EEPROM_BLOCK_SIZE, EEPROM_BLOCK_SIZE);
            if (ret != ee_ok)
                return ret;
            
            mEepromEraseBitmap[i/8] |= 1 << (i%8);
            while (EEPROM_isBusy());
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
static ee_err_t EEPROM_WritePage(uint32_t NoOfBytes, uint32_t Addr, uint8_t *Outbuf)
{
    
    if (NoOfBytes == 0)
        return ee_ok;
    
    while (EEPROM_isBusy());
    
    EEPROM_Command(EEPROM_WRITE_BYTES, Addr); /*CS will remain ASSERTED */
    
    Spi_SyncTransfer(gEepromSpiInstance_c, Outbuf, NULL, NoOfBytes);
    gEepromDeassertCS_d();

    return ee_ok;
}
#endif

/*****************************************************************************
*  EEPROM_WaitForReady
*
*
*****************************************************************************/
static ee_err_t EEPROM_WaitForReady(void)
{
    volatile uint16_t wait = 0x400; /* near 50 ms @ 50 us/cycle */
    
    /* Byte1 - Bit:   7       6      5:2       1         0
    *             RDY/BUSY  COMP  DENSITY  PROTECT  PAGE_SIZE
    *
    * Byte2 - Bit:   7       6    5    4    3    2    1    0
    *             RDY/BUSY  RES  EPE  RES  SLE  PS2  PS1  ES
    */
    
    while( EEPROM_isBusy() && (wait !=0) )
    {
        wait--;
    }
    
    if(wait != 0)
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
    cmd[1] = (Addr >> 16) & ADDRESS_MASK;
    cmd[2] = (Addr >>  8) & ADDRESS_MASK;
    cmd[3] = (Addr >>  0) & ADDRESS_MASK;

    gEepromAssertCS_d();
    Spi_SyncTransfer(gEepromSpiInstance_c, cmd, NULL, sizeof(cmd));
    /* CS will remain asserted to be able to send the data */
}

#endif
/* EOF: Eeprom_AT45DB161E */ 

