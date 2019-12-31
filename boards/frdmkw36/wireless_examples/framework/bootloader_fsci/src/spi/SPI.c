/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "EmbeddedTypes.h"
#include "SPI.h"

#if gBoot_UseSpiSlave_d
uint8_t mSpiRxBuffer[gSpiSlave_RxBufferSize_d];
volatile uint32_t mSpiIn, mSpiOut;

uint8_t * pSpiTxData;
volatile uint32_t mSpiTxLen, mSpiTRxLen;
static uint32_t mSpiCallbackParam;
void (*mSpiCallback)(uint32_t);

static void spi_slave_isr(void);
#endif

#if defined(CPU_QN9080C)
//SPI not implemented for QN9080C

void spi_master_init(uint32_t instance)
{
}

void spi_master_configure_bus(uint32_t instance, const spi_user_config_t * device)
{
}

void spi_slave_init(void (*calback)(uint32_t), uint32_t param)
{
}

uint16_t spi_slave_read(uint8_t* pData, uint16_t count)
{
    while(1);
}

void spi_slave_sync_write(uint8_t* pData, uint16_t count)
{
    while(1);
}

void mSpiCallback(uint32_t param)
{
  while(1);
}

#else  /* defined(CPU_QN9080C) */
/*! *********************************************************************************
*************************************************************************************
* Public Functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief   This function initialized the SPI module
*
* \param[in]  interfaceId        SPI module number
*
********************************************************************************** */
void spi_master_init(uint32_t instance)
{
    (void)instance;

    /* Enable SPI PORT clock */
    BOOT_SPI_PORT_SIM_SCG |= cSPI_PORT_SIM_SCG_Config_c;
    /* Enable SPI clock */
    BOOT_SPI_SIM_SCG |= cSPI_SIM_SCG_Config_c;
    /* Initialize SPI Pin Control registers */
    BOOT_SPI_SCLK_PCR &= ~PORT_PCR_MUX_MASK;
    BOOT_SPI_SCLK_PCR |= PORT_PCR_MUX(BOOT_SPI_ALTERNATIVE);
    BOOT_SPI_MOSI_PCR &= ~PORT_PCR_MUX_MASK;
    BOOT_SPI_MOSI_PCR |= PORT_PCR_MUX(BOOT_SPI_ALTERNATIVE);
    BOOT_SPI_MISO_PCR &= ~PORT_PCR_MUX_MASK;
    BOOT_SPI_MISO_PCR |= PORT_PCR_MUX(BOOT_SPI_ALTERNATIVE);
//    BOOT_SPI_SSEL_PCR &= ~PORT_PCR_MUX_MASK;
//    BOOT_SPI_SSEL_PCR |= PORT_PCR_MUX(BOOT_SPI_ALTERNATIVE);
    
    BOOT_SPI_SSEL_PCR &= ~PORT_PCR_MUX_MASK;
    BOOT_SPI_SSEL_PCR |= PORT_PCR_MUX(BOOT_PIN_GPIO_ALTERNATIVE);
    BOOT_SPI_SSEL_PDDR |= 1 << BOOT_SPI_SSEL_PIN_Number_c;
    deassert_cs();

    /* Initialize SPI module */
#if defined(USE_DSPI)
    BOOT_SPI_BasePtr->MCR = SPI_MCR_MSTR_MASK    |
                            SPI_MCR_ROOE_MASK    |
                            SPI_MCR_CLR_RXF_MASK |
                            SPI_MCR_CLR_TXF_MASK |
                            SPI_MCR_PCSIS(1)     |
                            SPI_MCR_SMPL_PT(2)   |
                            SPI_MCR_HALT_MASK;
#else
    BOOT_SPI_BasePtr->C1 = SPI_C1_MSTR_MASK;
    BOOT_SPI_BasePtr->C2 = 0x00;
    BOOT_SPI_BasePtr->C1 |= SPI_C1_SPE_MASK;
#endif
}


/*! *********************************************************************************
* \brief   This function configures the SPI interface
*
* \param[in]  interfaceId        SPI module number
* \param[in]  device             device config strucure
*
********************************************************************************** */
void spi_master_configure_bus(uint32_t instance, const spi_user_config_t * device)
{
#if defined(USE_DSPI)
    BOOT_SPI_BasePtr->CTAR[0] = SPI_CTAR_PBR(0)    | SPI_CTAR_BR(0x01)    |
                                SPI_CTAR_PCSSCK(1) | SPI_CTAR_CSSCK(0x04) |
                                SPI_CTAR_PASC(3)   | SPI_CTAR_ASC(0x04)   |
                                SPI_CTAR_PDT(3)    | SPI_CTAR_DT(0x05)    |
                                SPI_CTAR_FMSZ(7);
#else
    BOOT_SPI_BasePtr->BR = SPI_BR_SPPR(1) | SPI_BR_SPR(0);
#endif
}


/*! *********************************************************************************
* \brief   This function transferes bytes over the SPI interface
*
* \param[in]  interfaceId        SPI module number
* \param[in]  device             device config strucure
* \param[in]  sendBuffer         location of data to be Tx. NULL if no Tx needed
* \param[out] receiveBuffer      location where Rx data is stored. NULL if no Rx needed
* \param[in]  transferByteCount  number of bytes to be transfered
*
* \return spi_status_t
*
********************************************************************************** */
spi_status_t spi_master_transfer(uint32_t instance,
                                 const uint8_t * sendBuffer,
                                 uint8_t * receiveBuffer,
                                 uint32_t transferByteCount)
{
    volatile uint8_t dummy = 0;

    if( !transferByteCount )
        return 0;

    if( !sendBuffer && !receiveBuffer )
        return 1;

#if defined(USE_DSPI)
    /* Flush SPI FIFOs */
    BOOT_SPI_BasePtr->MCR |= SPI_MCR_CLR_RXF_MASK | SPI_MCR_CLR_TXF_MASK;
    /* Clear the EOQF, TCF and RFDF flags by writting a 1 to them */
    BOOT_SPI_BasePtr->SR  |= SPI_SR_EOQF_MASK | SPI_SR_TCF_MASK | SPI_SR_RFDF_MASK;
#else
    dummy = BOOT_SPI_BasePtr->S;
#endif

    while( transferByteCount-- )
    {
        if( sendBuffer )
        {
            dummy = *sendBuffer;
            sendBuffer++;
        }
        else
        {
            dummy = gSpi_DummyChar_d;
        }

#if defined(USE_DSPI)
        /* Write data */
        BOOT_SPI_BasePtr->PUSHR = dummy;
        /* Start SPI Transfer */
        BOOT_SPI_BasePtr->MCR &= ~SPI_MCR_HALT_MASK;
        /* Wait Transfer complete */
        while( !(BOOT_SPI_BasePtr->SR & SPI_SR_TCF_MASK) );
        /* Clear Transfer complete flag */
        BOOT_SPI_BasePtr->SR |= SPI_SR_TCF_MASK;
        /* Stop SPI transfer */
        BOOT_SPI_BasePtr->MCR |= SPI_MCR_HALT_MASK;
        /* Get received datad */
        dummy = BOOT_SPI_BasePtr->POPR;
#else
        /* Write data */
        BOOT_SPI_BasePtr->DL = dummy;
        /* Wait for transfer complete */
        //        while(!(SPI1_S & SPI_S_SPTEF_MASK));
        while(!((BOOT_SPI_BasePtr->S) & SPI_S_SPRF_MASK));
        /* Get received datad */
        dummy = BOOT_SPI_BasePtr->DL;
#endif

        if( receiveBuffer )
        {
            *receiveBuffer = dummy;
            receiveBuffer++;
        }
    }

    (void)dummy;
    return 0;
}

/*! *********************************************************************************
* \brief   This function initialized the SPI module in Slave mode
*
* \param[in]  calback  Pointer to a function to be called when data is received
* \param[in]  param    Parameter to be passed to the callback function
*
********************************************************************************** */
void spi_slave_init(void (*calback)(uint32_t), uint32_t param)
{
#if gBoot_UseSpiSlave_d
    uint32_t config = 0;
    
    /* Enable SPI PORT clock */
    BOOT_SPI_Slave_PORT_SIM_SCG |= cSPI_Slave_PORT_SIM_SCG_Config_c;
    /* Enable SPI clock */
    BOOT_SPI_Slave_SIM_SCG |= cSPI_Slave_SIM_SCG_Config_c;
    /* Initialize SPI Pin Control registers */
    BOOT_SPI_Slave_SCLK_PCR &= ~PORT_PCR_MUX_MASK;
    BOOT_SPI_Slave_SCLK_PCR |= PORT_PCR_MUX(BOOT_SPI_Slave_ALTERNATIVE);
    BOOT_SPI_Slave_MOSI_PCR &= ~PORT_PCR_MUX_MASK;
    BOOT_SPI_Slave_MOSI_PCR |= PORT_PCR_MUX(BOOT_SPI_Slave_ALTERNATIVE);
    BOOT_SPI_Slave_MISO_PCR &= ~PORT_PCR_MUX_MASK;
    BOOT_SPI_Slave_MISO_PCR |= PORT_PCR_MUX(BOOT_SPI_Slave_ALTERNATIVE);
    BOOT_SPI_Slave_SSEL_PCR &= ~PORT_PCR_MUX_MASK;
    BOOT_SPI_Slave_SSEL_PCR |= PORT_PCR_MUX(BOOT_SPI_Slave_ALTERNATIVE);

    /* Data Available Pin */
    BOOT_SPI_Slave_DAP_SCG_REG |= BOOT_SPI_Slave_DAP_SCG_MASK;
    /* Configure pin as GPIO */
    BOOT_SPI_Slave_DAP_PCR &= ~PORT_PCR_MUX_MASK;
    BOOT_SPI_Slave_DAP_PCR |= PORT_PCR_MUX(1);
    /* Set pin as output */
    BOOT_SPI_Slave_DAP_PDDR |= 1<< BOOT_SPI_Slave_DAP_pin;
#if gSPISlaveDapTxLogicOne_c
    /* Clear DAP */
    BOOT_SPI_Slave_DAP_PCOR |= 1<< BOOT_SPI_Slave_DAP_pin;
#else
    /* Set DAP */
    BOOT_SPI_Slave_DAP_PSOR |= 1<< BOOT_SPI_Slave_DAP_pin;
#endif
 
    pSpiTxData = NULL;
    mSpiOut = mSpiIn = 0;
    mSpiTRxLen = mSpiTxLen = 0;
    mSpiCallbackParam = param;
    mSpiCallback = calback;
    InstallIrqHandler(BOOT_SPI_Slave_IRQ, spi_slave_isr);
    BOOT_SPI_SIM_SCG |= cSPI_SIM_SCG_Config_c;

    /* Initialize SPI module */
#if defined(USE_DSPI)
    config = SPI_MCR_ROOE_MASK    |
             SPI_MCR_CLR_RXF_MASK | 
             SPI_MCR_CLR_TXF_MASK |
             SPI_MCR_PCSIS(1);

    BOOT_SPI_Slave_BasePtr->MCR = config | SPI_MCR_HALT_MASK;
    /* Configure frame size to 8 bytes */
    BOOT_SPI_Slave_BasePtr->CTAR[0] = SPI_CTAR_SLAVE_FMSZ(7);
    /* Enable SPI module */
    BOOT_SPI_Slave_BasePtr->MCR = config;
    /* write dummy data */
    BOOT_SPI_Slave_BasePtr->PUSHR = gSpi_DummyChar_d;
    /* Enable interrupt */
    BOOT_SPI_Slave_BasePtr->RSER = SPI_RSER_RFDF_RE_MASK;

#else
    BOOT_SPI_Slave_BasePtr->C1 = config;
    BOOT_SPI_Slave_BasePtr->C2 = config;
    /* Enable FIFO */
    BOOT_SPI_Slave_BasePtr->C3 = SPI_C3_RNFULLF_MARK_MASK | SPI_C3_RNFULLIEN_MASK | SPI_C3_FIFOMODE_MASK;
    BOOT_SPI_Slave_BasePtr->C1 |= SPI_C1_SPE_MASK | SPI_C1_SPIE_MASK;
    BOOT_SPI_Slave_BasePtr->DL = gSpi_DummyChar_d;
#endif

#else
    (void)calback;
    (void)param;
#endif /* #if gBoot_UseSpiSlave_d */
}

/*! *********************************************************************************
* \brief   Get data received over SPI
*
* \param[in]  pData  Pointer where data will be stored
* \param[in]  count  Number of bytes to read
*
* \return The number of bytes read
*
********************************************************************************** */
uint16_t spi_slave_read(uint8_t* pData, uint16_t count)
{
    uint16_t n = 0;
#if gBoot_UseSpiSlave_d
    uint32_t inIdx = mSpiIn;

    while( (mSpiOut != inIdx) && count )
    {
        *pData = mSpiRxBuffer[mSpiOut++];
        if( mSpiOut >= gSpiSlave_RxBufferSize_d )
        {
            mSpiOut=0;
        }
        inIdx = mSpiIn;
        count--;
        n++;
    }
#endif
    return n;
}

/*! *********************************************************************************
* \brief   Send data over SPI interface
*
* \param[in]  pData  Pointer to the data to be transfered
* \param[in]  count  Number of bytes to transfer
*
********************************************************************************** */
void spi_slave_sync_write(uint8_t* pData, uint16_t count)
{
#if gBoot_UseSpiSlave_d
    /* wait for previous Tx to finish */
    while( mSpiTRxLen );

    mSpiTRxLen = count;

    /* Disable SPI interrupt */
#if defined(USE_DSPI)
    BOOT_SPI_Slave_BasePtr->RSER = 0;
#else
    BOOT_SPI_Slave_BasePtr->C1 &= ~(SPI_C1_SPIE_MASK);
#endif

#if defined(USE_DSPI)
    /* Load the Tx FIFO */
    while( (BOOT_SPI_Slave_BasePtr->SR & SPI_SR_TFFF_MASK) && count )
    {
        BOOT_SPI_Slave_BasePtr->PUSHR = *pData++;
        BOOT_SPI_Slave_BasePtr->SR = SPI_SR_TFFF_MASK;
        count--;
    }

    pSpiTxData = pData;
    mSpiTxLen = count;
    
    /* Enable SPI interrupt */
    BOOT_SPI_Slave_BasePtr->RSER = SPI_RSER_RFDF_RE_MASK;

#else
    /* Load the Tx FIFO */
    while( !((BOOT_SPI_Slave_BasePtr->S) & SPI_S_TXFULLF_MASK) && count )
    {
        BOOT_SPI_Slave_BasePtr->DL = *pData++;
        count--;
    }

    pSpiTxData = pData;
    mSpiTxLen = count;
    
    /* Enable SPI interrupt */
    BOOT_SPI_Slave_BasePtr->C1 |= SPI_C1_SPIE_MASK;
#endif

#if gSPISlaveDapTxLogicOne_c
    /* Set Data Available Pin */
    BOOT_SPI_Slave_DAP_PSOR |= 1<< BOOT_SPI_Slave_DAP_pin;
#else
    /* Clear Data Available Pin */
    BOOT_SPI_Slave_DAP_PCOR |= 1<< BOOT_SPI_Slave_DAP_pin;
#endif

    /* Wait for transfer to finish */
    while( mSpiTRxLen );
#if gSPISlaveDapTxLogicOne_c
    /* Clear Data Available Pin */
    BOOT_SPI_Slave_DAP_PCOR |= 1 << BOOT_SPI_Slave_DAP_pin;
#else
    /* Set Data Available Pin */
    BOOT_SPI_Slave_DAP_PSOR |= 1 << BOOT_SPI_Slave_DAP_pin;
#endif

#endif /* #if gBoot_UseSpiSlave_d */
}

/*! *********************************************************************************
* \brief   Interrupt Service Routine for the SPI Slave
*
********************************************************************************** */
#if gBoot_UseSpiSlave_d
static void spi_slave_isr(void)
{
    uint32_t temp;
#if defined(USE_DSPI)
    /* Check Rx FIFO drain request flag */
    while( BOOT_SPI_Slave_BasePtr->SR & SPI_SR_RFDF_MASK )
    {
        /* Store Rx data */
        temp = BOOT_SPI_Slave_BasePtr->POPR;
        mSpiRxBuffer[mSpiIn++] = temp;
        if( mSpiIn >= gSpiSlave_RxBufferSize_d )
        {
            mSpiIn=0;
        }
        /* Clear Rx FIFO drain request flag */
        BOOT_SPI_Slave_BasePtr->SR = SPI_SR_RFDF_MASK;
        /* Check for buffer overflow */
        temp = mSpiIn;
        if( temp == mSpiOut )
        {
            mSpiOut++;
            if( mSpiOut >= gSpiSlave_RxBufferSize_d )
            {
                mSpiOut=0;
            }
        }
        
        if( mSpiTRxLen )
        {
            mSpiTRxLen--;
            /* Tx remaining data */
            if( mSpiTxLen )
            {
                BOOT_SPI_Slave_BasePtr->PUSHR = *pSpiTxData++;
                mSpiTxLen--;
            }
            else if( mSpiTRxLen == 0 )
            {
                BOOT_SPI_Slave_BasePtr->PUSHR = gSpi_DummyChar_d;
            }
        }

        /* Call installed callback function */
        if( mSpiCallback )
        {
            mSpiCallback(mSpiCallbackParam);
        }
    }
#else
    /* Check Rx FIFO drain request flag */
    while( BOOT_SPI_Slave_BasePtr->S & SPI_S_SPRF_MASK )
    {
        /* Store Rx data */
        temp = BOOT_SPI_Slave_BasePtr->DL
        mSpiRxBuffer[mSpiIn++] = temp;
        if( mSpiIn >= gSpiSlave_RxBufferSize_d )
        {
            mSpiIn=0;
        }
        /* Check for buffer overflow */
        temp = mSpiIn;
        if( temp == mSpiOut )
        {
            mSpiOut++;
            if( mSpiOut >= gSpiSlave_RxBufferSize_d )
            {
                mSpiOut=0;
            }
        }
        
        if( mSpiTRxLen )
        {
            mSpiTRxLen--;
            /* Tx remaining data */
            if( mSpiTxLen )
            {
                BOOT_SPI_Slave_BasePtr->DL = *pSpiTxData++;
                mSpiTxLen--;
            }
            else if( mSpiTRxLen == 0 )
            {
                BOOT_SPI_Slave_BasePtr->DL = gSpi_DummyChar_d;
            }
        }

        /* Call installed callback function */
        if( mSpiCallback )
        {
            mSpiCallback(mSpiCallbackParam);
        }
    }
#endif
}
#endif /* #if gBoot_UseSpiSlave_d */

#endif /* defined(CPU_QN9080C) */