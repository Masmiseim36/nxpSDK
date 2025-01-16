/*
 * Copyright 2020, 2024 NXP
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
#if defined(FMSTR_SERIAL_DRV) && (FMSTR_MK_IDSTR(FMSTR_SERIAL_DRV) == FMSTR_SERIAL_S32S247TV_LINFLEXD_ID)
#if !(FMSTR_DISABLE) 

#include "freemaster_serial.h"
#include "freemaster_s32s247tv_linflexd.h"

/***********************************
*  local variables
***********************************/

/* SCI base address */
#ifdef FMSTR_LINFLEXD_BASE
    static FMSTR_ADDR fmstr_LINFLEXDBaseAddr = (FMSTR_ADDR)FMSTR_LINFLEXD_BASE;
#else
    static FMSTR_ADDR fmstr_LINFLEXDBaseAddr = (FMSTR_ADDR)0;
#endif

/***********************************
*  local function prototypes
***********************************/

/* Interface function - Initialization of SCI driver adapter */
static FMSTR_BOOL _FMSTR_S32S247TV_Init(void);
static void _FMSTR_S32S247TV_EnableTransmit(FMSTR_BOOL enable);
static void _FMSTR_S32S247TV_EnableReceive(FMSTR_BOOL enable);
static void _FMSTR_S32S247TV_EnableTransmitInterrupt(FMSTR_BOOL enable);
static void _FMSTR_S32S247TV_EnableTransmitCompleteInterrupt(FMSTR_BOOL enable);
static void _FMSTR_S32S247TV_EnableReceiveInterrupt(FMSTR_BOOL enable);
static FMSTR_BOOL _FMSTR_S32S247TV_IsTransmitRegEmpty(void);
static FMSTR_BOOL _FMSTR_S32S247TV_IsReceiveRegFull(void);
static FMSTR_BOOL _FMSTR_S32S247TV_IsTransmitterActive(void);
static void _FMSTR_S32S247TV_PutChar(FMSTR_BCHR ch);
static FMSTR_BCHR _FMSTR_S32S247TV_GetChar(void);
static void _FMSTR_S32S247TV_Flush(void);

/***********************************
*  global variables
***********************************/
/* Interface of this SCI driver */ 

const FMSTR_SERIAL_DRV_INTF FMSTR_SERIAL_S32S247TV_LINFLEXD =
{
    .Init                       = _FMSTR_S32S247TV_Init,
    .EnableTransmit             = _FMSTR_S32S247TV_EnableTransmit,
    .EnableReceive              = _FMSTR_S32S247TV_EnableReceive,
    .EnableTransmitInterrupt    = _FMSTR_S32S247TV_EnableTransmitInterrupt,
    .EnableTransmitCompleteInterrupt= _FMSTR_S32S247TV_EnableTransmitCompleteInterrupt,
    .EnableReceiveInterrupt     = _FMSTR_S32S247TV_EnableReceiveInterrupt,
    .IsTransmitRegEmpty         = _FMSTR_S32S247TV_IsTransmitRegEmpty,
    .IsReceiveRegFull           = _FMSTR_S32S247TV_IsReceiveRegFull,
    .IsTransmitterActive        = _FMSTR_S32S247TV_IsTransmitterActive,
    .PutChar                    = _FMSTR_S32S247TV_PutChar,
    .GetChar                    = _FMSTR_S32S247TV_GetChar,
    .Flush                      = _FMSTR_S32S247TV_Flush,
};

/****************************************************************************************
* General peripheral space access macros
*****************************************************************************************/

#define FMSTR_SETBIT(base, offset, bit)         (*(((volatile FMSTR_U8*)(base))+(offset)) |= (bit))
#define FMSTR_CLRBIT(base, offset, bit)         (*(((volatile FMSTR_U8*)(base))+(offset)) &= ~(bit))
#define FMSTR_TSTBIT(base, offset, bit)         (*(((volatile FMSTR_U8*)(base))+(offset)) & (bit))
#define FMSTR_SETREG(base, offset, value)       (*(((volatile FMSTR_U8*)(base))+(offset)) = (value))
#define FMSTR_GETREG(base, offset)              (*(((volatile FMSTR_U8*)(base))+(offset)))

/****************************************************************************************
* LINFLEXD module constants
*****************************************************************************************/
/* LINFlexD module registers */
#define FMSTR_LINFLEXD_UARTCR_OFFSET        0x10       /* 32bit register */
#define FMSTR_LINFLEXD_LINIER_OFFSET        0x04        /* 32bit register */
#define FMSTR_LINFLEXD_LINCR1_OFFSET        0x00        /* 32bit register */
#define FMSTR_LINFLEXD_UARTSR_OFFSET        0x14       /* 32bit register */
#define FMSTR_LINFLEXD_BDRL_OFFSET          0x38       /* 32bit register output*/
#define FMSTR_LINFLEXD_BDRM_OFFSET          0x3C       /* 32bit register input*/
#define FMSTR_LINFLEXD_UARTPTO_OFFSET       0x50        /* 32bit register */
#define FMSTR_LINFLEXD_UARTCTO_OFFSET       0x54        /* 32bit register */

/* LINFlexD UARTCR Control Register 1 bits */
#define FMSTR_LINFLEXD_UARTCR_TXEN         0x00000010
#define FMSTR_LINFLEXD_UARTCR_RXEN         0x00000020

/* LINFlexD LINIER Control Register 1 bits */
#define FMSTR_LINFLEXD_LINIER_DRIE         0x00000004
#define FMSTR_LINFLEXD_LINIER_DTIE         0x00000002

/* LINFlexD UART Status registers bits */
#define FMSTR_LINFLEXD_UARTSR_DRFRFE          0x00000004      /*DRF*/
#define FMSTR_LINFLEXD_UARTSR_DTFTFF          0x00000002      /*DTF*/

/******************************************************************************
*
* @brief    SCI communication initialization
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_S32S247TV_Init(void)
{
#if FMSTR_SERIAL_SINGLEWIRE
    #error Internal single wire mode is not supported.
    return FMSTR_FALSE;
#endif
    
    /* valid runtime module address must be assigned */
    if(fmstr_LINFLEXDBaseAddr != 0)
        return FMSTR_TRUE;
    else 
        return FMSTR_FALSE;
}


/******************************************************************************
*
* @brief    Enable/Disable LINFLEXD transmitter
*
******************************************************************************/

static void _FMSTR_S32S247TV_EnableTransmit(FMSTR_BOOL enable)
{
    if(enable)
    {
        /* Enable transmitter */
        FMSTR_SETBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_UARTCR_OFFSET, FMSTR_LINFLEXD_UARTCR_TXEN);
    }
    else
    {
        /* Disable transmitter */
        FMSTR_CLRBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_UARTCR_OFFSET, FMSTR_LINFLEXD_UARTCR_TXEN);
    }
}

/******************************************************************************
*
* @brief    Enable/Disable LINFLEXD receiver
*
******************************************************************************/

static void _FMSTR_S32S247TV_EnableReceive(FMSTR_BOOL enable)
{
    if(enable)
    {
        /* Enable receiver (enables single-wire connection) */
        FMSTR_SETBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_UARTCR_OFFSET, FMSTR_LINFLEXD_UARTCR_RXEN);
    } 
    else
    {
        /* Disable receiver */
        FMSTR_CLRBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_UARTCR_OFFSET, FMSTR_LINFLEXD_UARTCR_RXEN);
    }
}

/******************************************************************************
*
* @brief    Enable/Disable interrupt from transmit register empty event
*
******************************************************************************/

static void _FMSTR_S32S247TV_EnableTransmitInterrupt(FMSTR_BOOL enable)
{
    if(enable)
    {
        /* Enable interrupt */
        FMSTR_SETBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_LINIER_OFFSET, FMSTR_LINFLEXD_LINIER_DTIE);
    }
    else
    {
        /* Disable interrupt */
        FMSTR_CLRBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_LINIER_OFFSET, FMSTR_LINFLEXD_LINIER_DTIE);
    }
}

/******************************************************************************
*
* @brief    Enable/Disable interrupt when transmission is complete
*
******************************************************************************/

static void _FMSTR_S32S247TV_EnableTransmitCompleteInterrupt(FMSTR_BOOL enable)
{
    if(enable)
    {
        /* Enable interrupt */
        FMSTR_SETBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_LINIER_OFFSET, FMSTR_LINFLEXD_LINIER_DTIE);
    }
    else
    {
        /* Disable interrupt */
        FMSTR_CLRBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_LINIER_OFFSET, FMSTR_LINFLEXD_LINIER_DTIE);
    }
}

/******************************************************************************
*
* @brief    Enable/Disable interrupt from receive register full event
*
******************************************************************************/

static void _FMSTR_S32S247TV_EnableReceiveInterrupt(FMSTR_BOOL enable)
{
    if(enable)
    {
        /* Enable interrupt */
        FMSTR_SETBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_LINIER_OFFSET, FMSTR_LINFLEXD_LINIER_DRIE);
    }
    else
    {
        /* Disable interrupt */
        FMSTR_CLRBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_LINIER_OFFSET, FMSTR_LINFLEXD_LINIER_DRIE);
    }
}

/******************************************************************************
*
* @brief    Returns TRUE if the transmit register is empty, and it's possible to put next char
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_S32S247TV_IsTransmitRegEmpty(void)
{
    return (FMSTR_BOOL) FMSTR_TSTBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_UARTSR_OFFSET, FMSTR_LINFLEXD_UARTSR_DTFTFF);
}

/******************************************************************************
*
* @brief    Returns TRUE if the receive register is full, and it's possible to get received char
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_S32S247TV_IsReceiveRegFull(void)
{
    return (FMSTR_BOOL) FMSTR_TSTBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_UARTSR_OFFSET, FMSTR_LINFLEXD_UARTSR_DRFRFE);
}

/******************************************************************************
*
* @brief    Returns TRUE if the transmitter is still active 
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_S32S247TV_IsTransmitterActive(void)
{
    /* 0 - Transmission in progress, 1 - No transmission in progress */
    return (!(FMSTR_TSTBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_UARTSR_OFFSET, FMSTR_LINFLEXD_UARTSR_DTFTFF)));
}

/******************************************************************************
*
* @brief    The function puts the char for transmit
*
******************************************************************************/

static void _FMSTR_S32S247TV_PutChar(FMSTR_BCHR  ch)
{
    FMSTR_SETREG(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_BDRL_OFFSET , ch);
    FMSTR_SETBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_UARTSR_OFFSET, FMSTR_LINFLEXD_UARTSR_DTFTFF);
}

/******************************************************************************
*
* @brief    The function gets the received char
*
******************************************************************************/
static FMSTR_BCHR _FMSTR_S32S247TV_GetChar(void)
{
    FMSTR_BCHR c=0;
    c = FMSTR_GETREG(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_BDRM_OFFSET );
    FMSTR_SETBIT(fmstr_LINFLEXDBaseAddr, FMSTR_LINFLEXD_UARTSR_OFFSET, FMSTR_LINFLEXD_UARTSR_DRFRFE);
    return c;
}

/******************************************************************************
*
* @brief    The function sends buffered data
*
******************************************************************************/

static void _FMSTR_S32S247TV_Flush(void)
{
}

/******************************************************************************
*
* @brief    Assign FreeMASTER communication module base address
*
******************************************************************************/

void FMSTR_SerialSetBaseAddress(FMSTR_ADDR base)
{
    fmstr_LINFLEXDBaseAddr = base;
}

/******************************************************************************
*
* @brief    Process FreeMASTER serial interrupt (call this function from SCI ISR)
*
******************************************************************************/

void FMSTR_SerialIsr(void)
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

void FMSTR_SerialIsr(void)
{
}

#endif /* !(FMSTR_DISABLE) */ 
#endif /* defined(FMSTR_SERIAL_DRV) && (FMSTR_MK_IDSTR(FMSTR_SERIAL_DRV) == FMSTR_SERIAL_S32S247TV_LINFLEXD_ID) */
