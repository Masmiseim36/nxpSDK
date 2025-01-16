/*
 * Copyright 2020, 2022, 2024 NXP
 *
 * License: NXP LA_OPT_Online Code Hosting NXP_Software_License
 *
 * NXP Proprietary. This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * FreeMASTER Communication Driver - Serial Communication Interface
 */
#include "freemaster.h"
#include "freemaster_private.h"

/* Compile this code only if the SERIAL driver is selected in freemaster_cfg.h. */
#if defined(FMSTR_SERIAL_DRV) && (FMSTR_MK_IDSTR(FMSTR_SERIAL_DRV) == FMSTR_SERIAL_S32K3XX_LPUART_ID)
#if !(FMSTR_DISABLE)

#include "freemaster_serial.h"
#include "freemaster_s32k3xx_lpuart.h"

/***********************************
*  local variables
***********************************/

/* SCI base address */
#ifdef FMSTR_LPUART_BASE
    static FMSTR_ADDR fmstr_LPUARTBaseAddr = (FMSTR_ADDR)FMSTR_LPUART_BASE;
#else
    static FMSTR_ADDR fmstr_LPUARTBaseAddr = (FMSTR_ADDR)0;
#endif

/***********************************
*  local function prototypes
***********************************/

/* Interface function - Initialization of SCI driver adapter */
static FMSTR_BOOL _FMSTR_S32K3XX_Init(void);
static void _FMSTR_S32K3XX_EnableTransmit(FMSTR_BOOL enable);
static void _FMSTR_S32K3XX_EnableReceive(FMSTR_BOOL enable);
static void _FMSTR_S32K3XX_EnableTransmitInterrupt(FMSTR_BOOL enable);
static void _FMSTR_S32K3XX_EnableTransmitCompleteInterrupt(FMSTR_BOOL enable);
static void _FMSTR_S32K3XX_EnableReceiveInterrupt(FMSTR_BOOL enable);
static FMSTR_BOOL _FMSTR_S32K3XX_IsTransmitRegEmpty(void);
static FMSTR_BOOL _FMSTR_S32K3XX_IsReceiveRegFull(void);
static FMSTR_BOOL _FMSTR_S32K3XX_IsTransmitterActive(void);
static void _FMSTR_S32K3XX_PutChar(FMSTR_BCHR ch);
static FMSTR_BCHR _FMSTR_S32K3XX_GetChar(void);
static void _FMSTR_S32K3XX_Flush(void);

/***********************************
*  global variables
***********************************/
/* Interface of this SCI driver */

const FMSTR_SERIAL_DRV_INTF FMSTR_SERIAL_S32K3XX_LPUART =
{
    .Init                       = _FMSTR_S32K3XX_Init,
    .EnableTransmit             = _FMSTR_S32K3XX_EnableTransmit,
    .EnableReceive              = _FMSTR_S32K3XX_EnableReceive,
    .EnableTransmitInterrupt    = _FMSTR_S32K3XX_EnableTransmitInterrupt,
    .EnableTransmitCompleteInterrupt= _FMSTR_S32K3XX_EnableTransmitCompleteInterrupt,
    .EnableReceiveInterrupt     = _FMSTR_S32K3XX_EnableReceiveInterrupt,
    .IsTransmitRegEmpty         = _FMSTR_S32K3XX_IsTransmitRegEmpty,
    .IsReceiveRegFull           = _FMSTR_S32K3XX_IsReceiveRegFull,
    .IsTransmitterActive        = _FMSTR_S32K3XX_IsTransmitterActive,
    .PutChar                    = _FMSTR_S32K3XX_PutChar,
    .GetChar                    = _FMSTR_S32K3XX_GetChar,
    .Flush                      = _FMSTR_S32K3XX_Flush,
};

/****************************************************************************************
* General peripheral space access macros
*****************************************************************************************/

#define FMSTR_SETBIT(base, offset, bit)         (*(volatile FMSTR_U32*)(((volatile FMSTR_U8*)(base))+(offset)) |= (bit))
#define FMSTR_CLRBIT(base, offset, bit)         (*(volatile FMSTR_U32*)(((volatile FMSTR_U8*)(base))+(offset)) &= ~(bit))
#define FMSTR_TSTBIT(base, offset, bit)         (*(volatile FMSTR_U32*)(((volatile FMSTR_U8*)(base))+(offset)) & (bit))
#define FMSTR_SETREG(base, offset, value)       (*(volatile FMSTR_U32*)(((volatile FMSTR_U8*)(base))+(offset)) = (value))
#define FMSTR_GETREG(base, offset)              (*(volatile FMSTR_U32*)(((volatile FMSTR_U8*)(base))+(offset)))

/****************************************************************************************
* LPUART module constants
*****************************************************************************************/
/* LPUART module registers */
#define FMSTR_LPUART_STAT_OFFSET         0x14       /* 32bit register */
#define FMSTR_LPUART_CTRL_OFFSET         0x18       /* 32bit register */
#define FMSTR_LPUART_DATA_OFFSET         0x1C       /* 32bit register */

/* LPUART CTRL Control register bits */
#define FMSTR_LPUART_CTRL_TIE            0x00800000
#define FMSTR_LPUART_CTRL_TCIE           0x00400000
#define FMSTR_LPUART_CTRL_RIE            0x00200000
#define FMSTR_LPUART_CTRL_TE             0x00080000
#define FMSTR_LPUART_CTRL_RE             0x00040000

/* LPUART STAT Status registers bits */
#define FMSTR_LPUART_STAT_TDRE           0x00800000
#define FMSTR_LPUART_STAT_TC             0x00400000
#define FMSTR_LPUART_STAT_RDRF           0x00200000
#define FMSTR_LPUART_STAT_OR             0x00080000

/**************************************************************************//*!
*
* @brief    SCI communication initialization
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_S32K3XX_Init(void)
{
#if FMSTR_SERIAL_SINGLEWIRE
    #error Internal single wire mode is not supported.
    return FMSTR_FALSE;
#endif

    /* valid runtime module address must be assigned */
    if(fmstr_LPUARTBaseAddr != 0)
        return FMSTR_TRUE;
    else
        return FMSTR_FALSE;
}


/**************************************************************************//*!
*
* @brief    Enable/Disable LPUART transmitter
*
******************************************************************************/

static void _FMSTR_S32K3XX_EnableTransmit(FMSTR_BOOL enable)
{
    if(enable)
    {
        /* Enable transmitter */
        FMSTR_SETBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_CTRL_OFFSET, FMSTR_LPUART_CTRL_TE);
    }
    else
    {
        /* Disable transmitter */
        FMSTR_CLRBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_CTRL_OFFSET, FMSTR_LPUART_CTRL_TE);
    }
}

/**************************************************************************//*!
*
* @brief    Enable/Disable LPUART receiver
*
******************************************************************************/

static void _FMSTR_S32K3XX_EnableReceive(FMSTR_BOOL enable)
{
    if(enable)
    {
        /* Enable receiver (enables single-wire connection) */
        FMSTR_SETBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_CTRL_OFFSET, FMSTR_LPUART_CTRL_RE);
    }
    else
    {
        /* Disable receiver */
        FMSTR_CLRBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_CTRL_OFFSET, FMSTR_LPUART_CTRL_RE);
    }
}

/**************************************************************************//*!
*
* @brief    Enable/Disable interrupt from transmit register empty event
*
******************************************************************************/

static void _FMSTR_S32K3XX_EnableTransmitInterrupt(FMSTR_BOOL enable)
{
    if(enable)
    {
        /* Enable interrupt */
        FMSTR_SETBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_CTRL_OFFSET, FMSTR_LPUART_CTRL_TIE);
    }
    else
    {
        /* Disable interrupt */
        FMSTR_CLRBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_CTRL_OFFSET, FMSTR_LPUART_CTRL_TIE);
    }
}

/**************************************************************************//*!
*
* @brief    Enable/Disable interrupt when transmission is complete
*
******************************************************************************/

static void _FMSTR_S32K3XX_EnableTransmitCompleteInterrupt(FMSTR_BOOL enable)
{
    if(enable)
    {
        /* Enable interrupt */
        FMSTR_SETBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_CTRL_OFFSET, FMSTR_LPUART_CTRL_TCIE);
    }
    else
    {
        /* Disable interrupt */
        FMSTR_CLRBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_CTRL_OFFSET, FMSTR_LPUART_CTRL_TCIE);
    }
}

/**************************************************************************//*!
*
* @brief    Enable/Disable interrupt from receive register full event
*
******************************************************************************/

static void _FMSTR_S32K3XX_EnableReceiveInterrupt(FMSTR_BOOL enable)
{
    if(enable)
    {
        /* Enable interrupt */
        FMSTR_SETBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_CTRL_OFFSET, FMSTR_LPUART_CTRL_RIE);
    }
    else
    {
        /* Disable interrupt */
        FMSTR_CLRBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_CTRL_OFFSET, FMSTR_LPUART_CTRL_RIE);
    }
}

/**************************************************************************//*!
*
* @brief    Returns TRUE if the transmit register is empty, and it's possible to put next char
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_S32K3XX_IsTransmitRegEmpty(void)
{
    return (FMSTR_BOOL) FMSTR_TSTBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_STAT_OFFSET, FMSTR_LPUART_STAT_TDRE);
}

/**************************************************************************//*!
*
* @brief    Returns TRUE if the receive register is full, and it's possible to get received char
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_S32K3XX_IsReceiveRegFull(void)
{
    /* Clear overrun bit if set for the receiver to continue normal operation. */
    if(FMSTR_TSTBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_STAT_OFFSET, FMSTR_LPUART_STAT_OR))
    {
        FMSTR_SETBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_STAT_OFFSET, FMSTR_LPUART_STAT_OR);
    }

    return (FMSTR_BOOL) FMSTR_TSTBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_STAT_OFFSET, FMSTR_LPUART_STAT_RDRF);
}

/**************************************************************************//*!
*
* @brief    Returns TRUE if the transmitter is still active
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_S32K3XX_IsTransmitterActive(void)
{
    /* 0 - Transmission in progress, 1 - No transmission in progress */
    return (!(FMSTR_TSTBIT(fmstr_LPUARTBaseAddr, FMSTR_LPUART_STAT_OFFSET, FMSTR_LPUART_STAT_TC)));
}

/**************************************************************************//*!
*
* @brief    The function puts the char for transmit
*
******************************************************************************/

static void _FMSTR_S32K3XX_PutChar(FMSTR_BCHR  ch)
{
    FMSTR_SETREG(fmstr_LPUARTBaseAddr, FMSTR_LPUART_DATA_OFFSET , ch);
}

/**************************************************************************//*!
*
* @brief    The function gets the received char
*
******************************************************************************/
static FMSTR_BCHR _FMSTR_S32K3XX_GetChar(void)
{
    FMSTR_BCHR c=0;
    c = FMSTR_GETREG(fmstr_LPUARTBaseAddr, FMSTR_LPUART_DATA_OFFSET);
    return c;
}

/**************************************************************************//*!
*
* @brief    The function sends buffered data
*
******************************************************************************/

static void _FMSTR_S32K3XX_Flush(void)
{
}

/**************************************************************************//*!
*
* @brief    Assign FreeMASTER communication module base address
*
******************************************************************************/

void FMSTR_SerialSetBaseAddress(FMSTR_ADDR base)
{
    fmstr_LPUARTBaseAddr = base;
}

/**************************************************************************//*!
*
* @brief    Process FreeMASTER serial interrupt (call this function from SCI ISR)
*
******************************************************************************/

void FMSTR_SerialIsr()
{
    /* process incoming or just transmitted byte */
    #if (FMSTR_LONG_INTR) || (FMSTR_SHORT_INTR)
        FMSTR_ProcessSerial();
    #endif
}

#else /* !(FMSTR_DISABLE) */

/* Empty API functions when FMSTR_DISABLE is set */
void FMSTR_SerialSetBaseAddress(FMSTR_ADDR base)
{
    FMSTR_UNUSED(base);
}

void FMSTR_SerialIsr()
{
}

#endif /* !(FMSTR_DISABLE) */
#endif /* defined(FMSTR_SERIAL_DRV) && (FMSTR_MK_IDSTR(FMSTR_SERIAL_DRV) == FMSTR_SERIAL_S32K3XX_LPUART_ID) */
