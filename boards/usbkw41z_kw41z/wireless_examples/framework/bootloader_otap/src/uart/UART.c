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


/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "EmbeddedTypes.h"
#include "main.h"
#include "UART.h"


/************************************************************************************
*************************************************************************************
* Private Macros
*************************************************************************************
************************************************************************************/
#define GPIO_RX_MASK   (0x01 << BOOT_UART_GPIO_PIN_RX)    
#define UART_SBR  (unsigned long)(BOOT_UART_CLOCK / (16*BOOT_UART_BAUD_RATE))
#define UART_BRFA (unsigned long)((((BOOT_UART_CLOCK/(16*BOOT_UART_BAUD_RATE)) - UART_SBR)*32.0)+0.5)

/************************************************************************************
*************************************************************************************
* Private Memory
*************************************************************************************
************************************************************************************/
#if gBoot_UseUart_d
static uint8_t mUartIn, mUartOut;
static uint8_t mUartRxBuffer[256];
static uint32_t mUartCallbackParam;
static void (*mUartCallback)(uint32_t);
static void uart_isr(void);

#if gBoot_UseUartCalibration_d
static bool_t uart_FrequencyCalibration(void);
#endif

#endif

/************************************************************************************
*************************************************************************************
* Public Functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief   Trimming of Internal Oscilator
*          This function measures break impulse from PC and sets correct trim value
*
* \return TRUE is callibration was successfull
*
********************************************************************************** */
#if gBoot_UseUartCalibration_d
static bool_t uart_FrequencyCalibration(void)
{
    uint8_t uartCallibrationChar = 0xFC;
    uint32_t fSbr;
    uint32_t iSbr;
    uint32_t sysTiks;
    uint8_t brfa;
    
    /* Configure UART Rx Pin as GPIO */
    PORT_PCR_REG(BOOT_UART_GPIO_PORT_RX, BOOT_UART_GPIO_PIN_RX) = PORT_PCR_MUX(BOOT_PIN_GPIO_ALTERNATIVE);
    
    uart_sync_write(&uartCallibrationChar, 1);
    
    /*  Systick initialization */
    SYST_CSR = (SysTick_CSR_CLKSOURCE_MASK );
    SYST_RVR = 0xFFFFFF; /* full range 0.8s for standard bus clock  = 32768Hz * 640 FLL / 2^24 */
    SYST_CVR = 0;
    SYST_CSR = (SysTick_CSR_CLKSOURCE_MASK | SysTick_CSR_ENABLE_MASK);
    
    while( (GPIO_PDIR_REG(BOOT_CALIBRATION_GPIO_BASE) & GPIO_RX_MASK) == 0 )
    {
        if(SYST_CSR & SysTick_CSR_COUNTFLAG_MASK)
        {
            /* Calibration timeout */
            return FALSE;
        }
    }
    
    while( (GPIO_PDIR_REG(BOOT_CALIBRATION_GPIO_BASE) & GPIO_RX_MASK) )
    {
        if(SYST_CSR & SysTick_CSR_COUNTFLAG_MASK)
        {
            /* Calibration timeout */
            return FALSE;
        }
    }
    
    //(void)SYST_CSR;
    SYST_CSR = (SysTick_CSR_CLKSOURCE_MASK );
    SYST_CVR = 0;
    SYST_CSR = (SysTick_CSR_CLKSOURCE_MASK | SysTick_CSR_ENABLE_MASK);
    
    while( (GPIO_PDIR_REG(BOOT_CALIBRATION_GPIO_BASE) & GPIO_RX_MASK) == 0 )
    {
        if(SYST_CSR & SysTick_CSR_COUNTFLAG_MASK)
        {
            /* Calibration timeout */
            return FALSE;
        }
    }
    
    sysTiks = SYST_CVR;   
    sysTiks = SYST_RVR - sysTiks + 10;
    
    
#if BOOTLOADER_SHORT_TRIM
    #define BR_DIV (9*16)
#else
    #define BR_DIV (10*16)
#endif
    iSbr = sysTiks/BR_DIV;
    fSbr = sysTiks%BR_DIV;
    brfa = (unsigned char)((fSbr*32 + (BR_DIV>>1))/BR_DIV);

#if defined(USE_LPUART)
    LPUART_BAUD_REG(BOOT_UART_BASE) &= ~LPUART_BAUD_SBR_MASK;
    LPUART_BAUD_REG(BOOT_UART_BASE) |= LPUART_BAUD_SBR(iSbr);
    (void)brfa; // No fine trim on LPUART
#else
    UART_BDH_REG(BOOT_UART_BASE)     = (unsigned char)((iSbr>>8)&0x1f);
    UART_BDL_REG(BOOT_UART_BASE)     = (unsigned char)(iSbr&0xff);
    UART_C4_REG(BOOT_UART_BASE)      = (brfa&0x1f);  
#endif
    /* Configure UART Rx Pin */
    PORT_PCR_REG(BOOT_UART_GPIO_PORT_RX, BOOT_UART_GPIO_PIN_RX) = PORT_PCR_MUX(BOOT_PIN_UART_ALTERNATIVE);

    uart_sync_write(&uartCallibrationChar, 1);

    return TRUE;
}
#endif


/*! *********************************************************************************
* \brief   This function initialized the UART module in Interrupt mode
*
* \param[in]  calback  Pointer to a function to be called when data is received
* \param[in]  param    Parameter to be passed to the callback function
*
********************************************************************************** */
void uart_init(void (*calback)(uint32_t), uint32_t param)
{
#if gBoot_UseUart_d
    mUartOut = mUartIn = 0;
    mUartCallbackParam = param;
    mUartCallback = calback;

    /* Set Pin Mux */
    BOOT_UART_GPIO_PORT_SIM_SCG_REG |= BOOT_UART_GPIO_PORT_SIM_SCG_MASK;
    BOOT_UART_GPIO_PORT_RX->PCR[BOOT_UART_GPIO_PIN_RX] = PORT_PCR_MUX(BOOT_PIN_UART_ALTERNATIVE);
    BOOT_UART_GPIO_PORT_TX->PCR[BOOT_UART_GPIO_PIN_TX] = PORT_PCR_MUX(BOOT_PIN_UART_ALTERNATIVE);

    /* Enable UART module clock */
    BOOT_UART_SIM_SCG_REG |= BOOT_UART_SIM_SCG_MASK;

#if defined(USE_LPUART)
    BOOT_UART_BASE->BAUD &= ~LPUART_BAUD_SBR_MASK;
    BOOT_UART_BASE->BAUD |= LPUART_BAUD_SBR(UART_SBR);
    BOOT_UART_BASE->STAT = 0xC01FC000;
    BOOT_UART_BASE->MATCH = 0;
    BOOT_UART_BASE->CTRL = LPUART_CTRL_RIE_MASK | LPUART_CTRL_RE_MASK | LPUART_CTRL_TE_MASK;
    
#else
    /* Set baudrate */
    BOOT_UART_BASE->BDH = ((UART_SBR>>8)&0x1f);
    BOOT_UART_BASE->BDL = (UART_SBR&0xff);
    BOOT_UART_BASE->C4  = (UART_BRFA&0x1f); 
    /* Enable RX, TX, and RX IRQ */
    BOOT_UART_BASE->C1  = 0x00;
    BOOT_UART_BASE->C2  = UART_C2_RIE_MASK | UART_C2_RE_MASK | UART_C2_TE_MASK;
    BOOT_UART_BASE->C3  = 0x00;
#endif

#if gBoot_UseUartCalibration_d
    uart_FrequencyCalibration();
#endif

    InstallIrqHandler(BOOT_UART_IRQ, uart_isr);
    
#else
    (void)calback;
    (void)param;
#endif /* #if gBoot_UseUart_d */
}


/*! *********************************************************************************
* \brief   Get data received over UART
*
* \param[in]  pData  Pointer where data will be stored
* \param[in]  count  Number of bytes to read
*
* \return The number of bytes read
*
********************************************************************************** */
uint16_t uart_read(uint8_t* pData, uint16_t count)
{
    uint16_t n = 0;

#if gBoot_UseUart_d    
    while( (mUartIn != mUartOut) && count )
    {
        *pData = mUartRxBuffer[mUartOut++];
        count--;
        n++;
    }
#endif

    return n;
}


/*! *********************************************************************************
* \brief   Send data over UART interface
*
* \param[in]  pData  Pointer to the data to be transfered
* \param[in]  count  Number of bytes to transfer
*
********************************************************************************** */
void uart_sync_write(uint8_t* pData, uint16_t count)
{
#if gBoot_UseUart_d
    while(count)
    {
#if defined(USE_LPUART)
        while( (BOOT_UART_BASE->STAT & LPUART_STAT_TC_MASK) == 0 ) {};
        BOOT_UART_BASE->DATA = *pData++;

#else
        while( (BOOT_UART_BASE->S1 & UART_S1_TC_MASK) == 0 ){};
        BOOT_UART_BASE->D = *pData++;
#endif
        count--;
    }
#endif
}

/*! *********************************************************************************
* \brief   Interrupt Service Routine for the UART module
*
********************************************************************************** */
#if gBoot_UseUart_d
static void uart_isr(void)
{
#if defined(USE_LPUART)
    while(BOOT_UART_BASE->STAT & LPUART_STAT_RDRF_MASK)
    {
        /* Store Rx data */
        mUartRxBuffer[mUartIn++] = BOOT_UART_BASE->DATA;
        /* Check for buffer overflow */
        if( mUartIn == mUartOut )
        {
            mUartOut++;
        }
        /* Call installed callback function */
        if( mUartCallback )
        {
            mUartCallback(mUartCallbackParam);
        }
    }

#else
    while(BOOT_UART_BASE->S1 & UART_S1_RDRF_MASK)
    {
        /* Store Rx data */
        mUartRxBuffer[mUartIn++] = BOOT_UART_BASE->D;
        /* Check for buffer overflow */
        if( mUartIn == mUartOut )
        {
            mUartOut++;
        }
        /* Call installed callback function */
        if( mUartCallback )
        {
            mUartCallback(mUartCallbackParam);
        }
    }
#endif
}
#endif /* #if gBoot_UseUart_d */