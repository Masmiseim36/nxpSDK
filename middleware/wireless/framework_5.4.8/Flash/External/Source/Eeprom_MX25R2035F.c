/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is a source file which implements the driver for the MX25R2035F memory.
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

#if gEepromType_d == gEepromDevice_MX25R2035F_c

#include "SPI_Adapter.h"
#include "GPIO_Adapter.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#ifndef gEepromWriteEnable_d
#define gEepromWriteEnable_d   1
#endif

/* Control Commands */
#define  EEPROM_RDSR            0x05
#define  EEPROM_RDSFDR          0x5A
#define  EEPROM_WREN            0x06
#define  EEPROM_WRDI            0x04
#define  EEPROM_RDCR            0x15
#define  EEPROM_WRSR            0x01
#define  EEPROM_RDID            0x9F
#define  EEPROM_RES             0xAB
#define  EEPROM_REMS            0x90

/* Read Commands */
#define  EEPROM_READ            0x03
#define  EEPROM_FAST_READ       0x0B
#define  EEPROM_2READ           0xBB
#define  EEPROM_DREAD           0x3B
#define  EEPROM_4READ           0xEB
#define  EEPROM_QREAD           0x6B

/* Erase Commands */
#define  EEPROM_ERASE_4K        0x20
#define  EEPROM_ERASE_32K       0x52
#define  EEPROM_ERASE_64K       0xD8
#define  EEPROM_ERASE_ALL       0x60

/* Write Commands */
#define  EEPROM_WRITE_PAGE      0x02
#define  EEPROM_WRITE_QPAGE     0x38

/* Status Flags */
#define  EEPROM_BUSY_FLAG_MASK  0x01
#define  EEPROM_WEL_FLAG_MASK   0x02

#define  EEPROM_PAGE_SIZE       (256)
#define  EEPROM_SECTOR_SIZE     (4*1024)
#define  EEPROM_BLOCK_SIZE      (32*1024)
#define  EEPROM_PAGE_MASK       (EEPROM_PAGE_SIZE - 1)

/* adress mask */
#define  ADDRESS_MASK 0x000000FF

/* SPI config */
#ifndef gEepromSpiInstance_c
#define gEepromSpiInstance_c  0
#endif

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
static void     EEPROM_WriteEnable(void);
#endif
static ee_err_t EEPROM_WaitForReady(void);
static uint32_t EEPROM_ReadStatusReq(void);
static void     EEPROM_Command(uint8_t opCode, uint32_t Addr);


/*! *********************************************************************************
*************************************************************************************
* Private Memory Declarations
*************************************************************************************
********************************************************************************** */
static spiState_t mEepromSpiState;
const gpioOutputPinConfig_t mEepromSpiCsCfg = {
    .gpioPort = gpioPort_A_c,
    .gpioPin = 3,
    .outputLogic = 1,
    .slewRate = pinSlewRate_Fast_c,
    .driveStrength = pinDriveStrength_Low_c
};

#if gEepromWriteEnable_d
static uint8_t mEepromEraseBitmap[gEepromParams_TotalSize_c/gEepromParams_SectorSize_c];
#endif


/*! *********************************************************************************
*************************************************************************************
* Public Functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief   This function initializes external memory.
*
* \return MEM_SUCCESS_c if initialization is successful. (It's always successful).
*
********************************************************************************** */
ee_err_t EEPROM_Init(void)
{
    static uint8_t initialized = 0;
    ee_err_t status = ee_ok;

    spiBusConfig_t cfg = {
        .bitsPerSec = 8000000,
        .master = TRUE,
        .clkActiveHigh = TRUE,
        .clkPhaseFirstEdge = TRUE,
        .MsbFirst = TRUE
    };

#if gEepromWriteEnable_d
    uint32_t i;

    /* Mark Flash as Unerased */
    for (i = 0; i < NumberOfElements(mEepromEraseBitmap); i++)
    {
        mEepromEraseBitmap[i] = 0;
    }
#endif

    if (!initialized)
    {
        Spi_Init(gEepromSpiInstance_c, &mEepromSpiState, NULL, NULL);
        Spi_Configure(gEepromSpiInstance_c, &cfg);
        GpioOutputPinInit(&mEepromSpiCsCfg, 1);
        
        status = EEPROM_WaitForReady();

        if (status == ee_ok)
        {
            initialized = 1;
        }
    }

    return status;
}

/*! *********************************************************************************
* \brief   Erase all memory to 0xFF
*
* \return ee_err_t.
*
********************************************************************************** */
#if gEepromWriteEnable_d
ee_err_t EEPROM_ChipErase(void)
{
    uint8_t cmd[] = {EEPROM_ERASE_ALL};
    uint32_t i;
    
    /* Wait for idle state */
    while (EEPROM_isBusy());
    
    /* Enable write */
    EEPROM_WriteEnable();

    gEepromAssertCS_d();
    Spi_SyncTransfer(gEepromSpiInstance_c, cmd, NULL, sizeof(cmd));
    gEepromDeassertCS_d();

    /* Mark Flash as erased */
    for (i = 0; i < NumberOfElements(mEepromEraseBitmap); i++)
    {
        mEepromEraseBitmap[i] = 0xFF;
    }

    return ee_ok;
}
#endif

/*! *********************************************************************************
* \brief   Erase a block of memory to 0xFF
*
* \param[in] Addr      Start memory address
* \param[in] size      The size of the block
*
* \return ee_err_t.
*
********************************************************************************** */
#if gEepromWriteEnable_d
ee_err_t EEPROM_EraseBlock(uint32_t Addr, uint32_t size)
{
    uint8_t cmd[4];
    uint32_t i = Addr/size;;
    
    while (EEPROM_isBusy());
    
    EEPROM_WriteEnable();
    
    switch (size)
    {
    case EEPROM_SECTOR_SIZE:
        cmd[0] = EEPROM_ERASE_4K;
        mEepromEraseBitmap[i/8] |= 1 << (i%8);
        break;
    case EEPROM_BLOCK_SIZE:
        cmd[0] = EEPROM_ERASE_32K;
        mEepromEraseBitmap[i] = 0xFF;
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
#endif

/*! *********************************************************************************
* \brief   Write a data buffer into the external memory, at a given address
*
* \param[in] NoOfBytes Number of bytes to write
* \param[in] Addr      Start memory address
* \param[in] inbuf     Pointer to the data
*
* \return ee_err_t.
*
********************************************************************************** */
#if gEepromWriteEnable_d
ee_err_t EEPROM_WriteData(uint32_t NoOfBytes, uint32_t Addr, uint8_t *Outbuf)
{
    ee_err_t retval = ee_ok;
    uint32_t bytes;
    
    if (NoOfBytes > 0)
    {
        retval = EEPROM_PrepareForWrite(NoOfBytes, Addr);

        if (retval == ee_ok)
        {
            while ((Addr & EEPROM_PAGE_MASK) + NoOfBytes > EEPROM_PAGE_MASK)
            {
                bytes = EEPROM_PAGE_SIZE - (Addr & EEPROM_PAGE_MASK);
                
                retval = EEPROM_WritePage(bytes, Addr, Outbuf);
                NoOfBytes -= bytes;
                Addr += bytes;
                Outbuf += bytes;
                
                if (retval != ee_ok)
                {
                    break;
                }
            }
            
            retval = EEPROM_WritePage(NoOfBytes, Addr, Outbuf);
        }
    }
    
    return retval;
}
#endif

/*! *********************************************************************************
* \brief   Reads a data buffer from the external memory, from a given address
*
* \param[in] NoOfBytes Number of bytes to read
* \param[in] Addr      Start memory address
* \param[in] inbuf     Pointer to a location where to store the read data
*
* \return ee_err_t.
*
********************************************************************************** */
ee_err_t EEPROM_ReadData(uint16_t NoOfBytes, uint32_t Addr, uint8_t *inbuf)
{
    
    while (EEPROM_isBusy());
    
    EEPROM_Command(EEPROM_READ,Addr);
    
    Spi_SyncTransfer(gEepromSpiInstance_c, NULL, inbuf, NoOfBytes);
    gEepromDeassertCS_d();

    return ee_ok;
}

/*! *********************************************************************************
* \brief   Check if the memory controller is busy
*
* \return TRUE/FALSE.
*
********************************************************************************** */
uint8_t EEPROM_isBusy(void)
{
    return (EEPROM_ReadStatusReq() & EEPROM_BUSY_FLAG_MASK) == EEPROM_BUSY_FLAG_MASK;
}

/*! *********************************************************************************
*************************************************************************************
* Private Functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief   Read the memory controller status register
*
* \return status register.
*
********************************************************************************** */
static uint32_t EEPROM_ReadStatusReq(void)
{
    uint8_t data = EEPROM_RDSR;
    
    gEepromAssertCS_d();
    Spi_SyncTransfer(gEepromSpiInstance_c, &data, NULL, sizeof(data));
    Spi_SyncTransfer(gEepromSpiInstance_c, NULL, &data, sizeof(data));
    gEepromDeassertCS_d();

    return data;
}

/*! *********************************************************************************
* \brief   Enabled Write/Erase access for the next operation
*
********************************************************************************** */
#if gEepromWriteEnable_d
static void EEPROM_WriteEnable(void)
{
    uint8_t data = EEPROM_WREN;
    
    gEepromAssertCS_d();
    Spi_SyncTransfer(gEepromSpiInstance_c, &data, NULL, sizeof(data));
    gEepromDeassertCS_d();
}
#endif

/*! *********************************************************************************
* \brief   Prepare the memory for write operations (bits must be in erased state)
*
* \param[in] NoOfBytes Number of bytes to write
* \param[in] Addr      Start memory address
*
* \return ee_err_t.
*
********************************************************************************** */
#if gEepromWriteEnable_d
static ee_err_t EEPROM_PrepareForWrite(uint32_t NoOfBytes, uint32_t Addr)
{
    uint32_t i;
    ee_err_t ret = ee_ok;
    uint32_t startBlk, endBlk;
    
    /* Obtain the block number */
    startBlk = Addr / EEPROM_SECTOR_SIZE;
    endBlk   = (Addr + NoOfBytes) / EEPROM_SECTOR_SIZE;
    
    if( (Addr + NoOfBytes) % EEPROM_SECTOR_SIZE )
    {
        endBlk++;
    }
    
    for (i = startBlk; i <= endBlk; i++)
    {
        if ((mEepromEraseBitmap[i/8] & (1 << (i%8) ) ) == 0)
        {
            ret = EEPROM_EraseBlock(i * EEPROM_SECTOR_SIZE, EEPROM_SECTOR_SIZE);

            if (ret != ee_ok)
            {
                break;
            }
        }
    }

    return ret;
}
#endif

/*! *********************************************************************************
* \brief   Writes maximum 256 bytes into a memory page
*
* \param[in] NoOfBytes Number of bytes to write into thr page
* \param[in] Addr      Start memory address
* \param[in] Outbuf    Pointer to the data to be written
*
* \return ee_err_t.
*
********************************************************************************** */
#if gEepromWriteEnable_d
static ee_err_t EEPROM_WritePage(uint32_t NoOfBytes, uint32_t Addr, uint8_t *Outbuf)
{
    if (NoOfBytes > 0)
    {
        while (EEPROM_isBusy());
        
        EEPROM_WriteEnable();
    
        EEPROM_Command(EEPROM_WRITE_PAGE, Addr); /*CS will remain ASSERTED */
    
        Spi_SyncTransfer(gEepromSpiInstance_c, Outbuf, NULL, NoOfBytes);
        gEepromDeassertCS_d();
    }

    return ee_ok;
}
#endif

/*! *********************************************************************************
* \brief   Wait for the external memory to become ready.
*
* \return ee_err_t status.
*
********************************************************************************** */
static ee_err_t EEPROM_WaitForReady(void)
{
    ee_err_t status = ee_ok;
    volatile uint16_t wait = 0x400;
    
    while (EEPROM_isBusy() && (wait !=0))
    {
        wait--;
    }
    
    if (wait == 0)
    {
        status = ee_error;
    }
    
    return status;
}


/*! *********************************************************************************
* \brief   This function will assert the CS signal and write over SPI the command 
*          header: command Id and memory address
*
* \param[in] opCode  Command Id
* \param[in] Addr    Start address
*
* \remarks The CS signal remains asserted to transfer data over SPI.
*
********************************************************************************** */
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
