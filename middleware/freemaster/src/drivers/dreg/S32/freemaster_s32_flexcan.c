/*
 * Copyright 2024 NXP
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
 * FreeMASTER Communication Driver - CAN Communication Interface
 */
#include "freemaster.h"
#include "freemaster_private.h"

/* Compile this code only if the CAN driver is selected in freemaster_cfg.h. */
#if defined(FMSTR_CAN_DRV) && (FMSTR_MK_IDSTR(FMSTR_CAN_DRV) == FMSTR_CAN_S32_FLEXCAN_ID)
#if !(FMSTR_DISABLE)

#include "freemaster_can.h"
#include "freemaster_s32_flexcan.h"

/***********************************
*  local variables
***********************************/

/* CAN base address */
#ifdef FMSTR_CAN_BASE
    static FMSTR_ADDR fmstr_CANBaseAddr = (FMSTR_ADDR)FMSTR_CAN_BASE;
#else
    static FMSTR_ADDR fmstr_CANBaseAddr = (FMSTR_ADDR)0;
#endif

/***********************************
*  local function prototypes
***********************************/

/* Interface function - Initialization of SCI driver adapter */
static FMSTR_BOOL _FMSTR_S32_Init(FMSTR_U32 idRx, FMSTR_U32 idTx);
static void _FMSTR_S32_EnableTxInterrupt(FMSTR_BOOL enable);
static void _FMSTR_S32_EnableRxInterrupt(FMSTR_BOOL enable);
static void _FMSTR_S32_EnableRx(void);
static FMSTR_SIZE8 _FMSTR_S32_GetRxFrameLen(void);
static FMSTR_BCHR _FMSTR_S32_GetRxFrameByte(FMSTR_SIZE8 index);
static void _FMSTR_S32_AckRxFrame(void);
static FMSTR_BOOL _FMSTR_S32_PrepareTxFrame(void);
static void _FMSTR_S32_PutTxFrameByte(FMSTR_SIZE8 index, FMSTR_BCHR data);
static void _FMSTR_S32_SendTxFrame(FMSTR_SIZE8 len);

/***********************************
*  global variables
***********************************/
static FMSTR_U32 fmstr_idRx;
static FMSTR_U32 fmstr_idTx;

/* Interface of this CAN driver */
const FMSTR_CAN_DRV_INTF FMSTR_CAN_S32_FLEXCAN =
{
    .Init              = _FMSTR_S32_Init,
    .EnableTxInterrupt = _FMSTR_S32_EnableTxInterrupt,
    .EnableRxInterrupt = _FMSTR_S32_EnableRxInterrupt,
    .EnableRx          = _FMSTR_S32_EnableRx,
    .GetRxFrameLen     = _FMSTR_S32_GetRxFrameLen,
    .GetRxFrameByte    = _FMSTR_S32_GetRxFrameByte,
    .AckRxFrame        = _FMSTR_S32_AckRxFrame,
    .PrepareTxFrame    = _FMSTR_S32_PrepareTxFrame,
    .PutTxFrameByte    = _FMSTR_S32_PutTxFrameByte,
    .SendTxFrame       = _FMSTR_S32_SendTxFrame,
};

/****************************************************************************************
* General peripheral space access macros
*****************************************************************************************/

#define FMSTR_SETBIT32(base, offset, bit)                           (*(volatile FMSTR_U32*)(((volatile FMSTR_U8*)(base))+(offset)) |= (bit))
#define FMSTR_CLRBIT32(base, offset, bit)                           (*(volatile FMSTR_U32*)(((volatile FMSTR_U8*)(base))+(offset)) &= ~(bit))
#define FMSTR_TSTBIT32(base, offset, bit)                           (*(volatile FMSTR_U32*)(((volatile FMSTR_U8*)(base))+(offset)) & (bit))
#define FMSTR_SETFIELD32(base, offset, shiftpos, mask, value)       (*(volatile FMSTR_U32*)(((volatile FMSTR_U8*)(base)) + (offset)) = (*(volatile FMSTR_U32*)(((volatile FMSTR_U8*)(base)) + (offset)) & (~(mask << (shiftpos)))) | (((FMSTR_U32)(value) & mask) << (shiftpos)))
#define FMSTR_GETFIELD32(base, offset, shiftpos, mask)              (((*(volatile FMSTR_U32*)(((volatile FMSTR_U8*)(base)) + (offset))) >> shiftpos) & mask)
#define FMSTR_SETREG8(base, offset, value)                          (*(volatile FMSTR_U8*)(((FMSTR_U32)(base))+(offset))) = (FMSTR_U32)(value)
#define FMSTR_GETREG8(base, offset)                                 (*(volatile FMSTR_U8*)(((FMSTR_U32)(base))+(offset)))

/****************************************************************************************
* FLEXCAN module constants
*****************************************************************************************/

/* FlexCAN Control register bit masks */
#define FMSTR_FCAN_MCR_MDIS_BIT          0x80000000
#define FMSTR_FCAN_MCR_FRZ_BIT           0x40000000
#define FMSTR_FCAN_MCR_RFEN_BIT          0x20000000
#define FMSTR_FCAN_MCR_HALT_BIT          0x10000000
#define FMSTR_FCAN_MCR_NOTRDY_BIT        0x08000000
#define FMSTR_FCAN_MCR_SOFTRST_BIT       0x02000000
#define FMSTR_FCAN_MCR_FRZACK_BIT        0x01000000
#define FMSTR_FCAN_MCR_SUPV_BIT          0x00800000
#define FMSTR_FCAN_MCR_WRNEN_BIT         0x00200000
#define FMSTR_FCAN_MCR_LPMACK_BIT        0x00100000
#define FMSTR_FCAN_MCR_SRXDIS_BIT        0x00020000
#define FMSTR_FCAN_MCR_IRMQ_BIT          0x00010000
#define FMSTR_FCAN_MCR_DMA_BIT           0x00008000
#define FMSTR_FCAN_MCR_LPRIOEN_BIT       0x00002000
#define FMSTR_FCAN_MCR_AEN_BIT           0x00001000
#define FMSTR_FCAN_MCR_FDEN_BIT          0x00000800
#define FMSTR_FCAN_MCR_IDAM9_BIT         0x00000200
#define FMSTR_FCAN_MCR_IDAM8_BIT         0x00000100
#define FMSTR_FCAN_ERFCR_ERFEN_BIT       0x80000000
#define FMSTR_FCAN_MB_SRR_BIT            0x00400000
#define FMSTR_FCAN_MB_IDE_BIT            0x00200000
#define FMSTR_FCAN_MB_RTR_BIT            0x00100000
#define FMSTR_FLEXCAN_RXMB_BIT           0X00000001 << FMSTR_FLEXCAN_RXMB
#define FMSTR_FLEXCAN_TXMB_BIT           0X00000001 << FMSTR_FLEXCAN_TXMB

/* FLEXCAN module registers offsets */
#define FMSTR_FCAN_MBTX_OFFSET           (0x80 + ((FMSTR_FLEXCAN_TXMB) * 0x10))    /* Tx Message Buffer */
#define FMSTR_FCAN_MCR_OFFSET            0x00                                      /* Module Configuration */
#define FMSTR_FCAN_TIMER_OFFSET          0x08                                      /* 16 bits Free-running Timer */
#define FMSTR_FCAN_MBRX_OFFSET           (0x80 + ((FMSTR_FLEXCAN_RXMB) * 0x10))    /* Rx Message Buffer */
#define FMSTR_FCAN_RXMGMASK_OFFSET       0x10                                      /* Message buffer global Id mask */
#define FMSTR_FCAN_IMASK1_OFFSET         0x28                                      /* Interrupt enable mask */
#define FMSTR_FCAN_IFLAG1_OFFSET         0x30                                      /* Interrupt active flag */
#define FMSTR_FCAN_RXIMR0_OFFSET         0x880                                     /* Message buffer 0 individual Id mask */
#define FMSTR_FCAN_RXIMR1_OFFSET         0x884                                     /* Message buffer 1 individual Id mask */
#define FMSTR_FCAN_ERFCR_OFFSET          0xC0C                                     /* Enhanced RX FIFO Control */

/* Masks for fields / fields (starting bit position and size in bits) */

#define FMSTR_FCAN_MB_CODE_SHIFT         24u
#define FMSTR_FCAN_MB_STDID_SHIFT        18u
#define FMSTR_FCAN_MB_EXTID_SHIFT        0u
#define FMSTR_FCAN_MB_DLC_SHIFT          16u
#define FMSTR_FCAN_MCR_MAXMB_SHIFT       0u
#define FMSTR_FCAN_TIMER_SHIFT           0u

#define FMSTR_FCAN_MB_CODE_MASK          0xF
#define FMSTR_FCAN_MB_STDID_MASK         0x7FF
#define FMSTR_FCAN_MB_EXTID_MASK         0x1FFFFFFF
#define FMSTR_FCAN_MB_DLC_MASK           0xF
#define FMSTR_FCAN_MCR_MAXMB_MASK        0x7F
#define FMSTR_FCAN_TIMER_MASK            0xFFFF

/* Bits groups / fields specific values */
#define FMSTR_FCAN_MB_CODE_RXINACTIVE    0x00        /* buffer void after received data read-out */
#define FMSTR_FCAN_MB_CODE_RXFULL        0x02        /* active and full */
#define FMSTR_FCAN_MB_CODE_RXEMPTY       0x04        /* active and empty */
#define FMSTR_FCAN_MB_CODE_TXINACTIVE    0x08        /* Message buffer not ready for transmit */
#define FMSTR_FCAN_MB_CODE_TXABORT       0x09        /* Message buffer aborted */
#define FMSTR_FCAN_MB_CODE_TXDATA        0x0C        /* Transmit data frame unconditionally once */

/**************************************************************************//*!
*
* @brief    Initialize CAN communication module
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_S32_Init(FMSTR_U32 idRx, FMSTR_U32 idTx)
{
    fmstr_idRx = idRx;
    fmstr_idTx = idTx;
    /* Valid runtime module address must be assigned */
    if (fmstr_CANBaseAddr != 0)
    {
        FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_MDIS_BIT);                 /* FlexCAN module enable */
        FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_FRZ_BIT);                  /* freeze enable on halt */
        while (!(FMSTR_TSTBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_FRZACK_BIT)))     /* wait for FRZACK */
        {
            FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_HALT_BIT);             /* enter freeze with HALT */
        }

        if (FMSTR_GETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_MAXMB_SHIFT, FMSTR_FCAN_MCR_MAXMB_MASK) < 1u)
        {
            /* allow the existence of at least 2 message buffers (0 and 1) for FreeMASTER's Tx and Rx */
            FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_MAXMB_SHIFT, FMSTR_FCAN_MCR_MAXMB_MASK, 1u);
        }
        FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_FDEN_BIT );                /* disable FDEN (CAN FD Operation) */
        FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_RFEN_BIT );                /* disable RDEN (legacy FIFO) */
        FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_FRZ_BIT);                  /* freeze disable on halt */
        do
        {
            FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_HALT_BIT);             /* exit freeze with HALT) */
        }
        while (FMSTR_TSTBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_FRZACK_BIT));       /* wait for FRZACK */
        FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MCR_OFFSET, FMSTR_FCAN_MCR_HALT_BIT);                    /* clear HALT requests */

        /* Configuring the FCAN Rx and Tx buffers, only once at the initialization phase */
        if ((FMSTR_CAN_EXTID & fmstr_idRx) == 0)
        {
            /* Init CAN Rx and Tx message buffers with standard Id*/
            FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBRX_OFFSET, FMSTR_FCAN_MB_CODE_SHIFT, FMSTR_FCAN_MB_CODE_MASK, FMSTR_FCAN_MB_CODE_RXINACTIVE);
            FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MBRX_OFFSET, FMSTR_FCAN_MB_IDE_BIT);
            FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBRX_OFFSET + 0x4, FMSTR_FCAN_MB_STDID_SHIFT, FMSTR_FCAN_MB_STDID_MASK, fmstr_idRx);
            FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET, FMSTR_FCAN_MB_CODE_SHIFT, FMSTR_FCAN_MB_CODE_MASK, FMSTR_FCAN_MB_CODE_TXINACTIVE);
            FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET, FMSTR_FCAN_MB_IDE_BIT);
            FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET + 0x4, FMSTR_FCAN_MB_STDID_SHIFT, FMSTR_FCAN_MB_STDID_MASK, fmstr_idTx);
        }
        else
        {
            /* Init CAN Rx and Tx message buffers with extended Id */
            FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBRX_OFFSET, FMSTR_FCAN_MB_CODE_SHIFT, FMSTR_FCAN_MB_CODE_MASK, FMSTR_FCAN_MB_CODE_RXINACTIVE);
            FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MBRX_OFFSET, FMSTR_FCAN_MB_IDE_BIT);
            FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBRX_OFFSET + 0x4, FMSTR_FCAN_MB_EXTID_SHIFT, FMSTR_FCAN_MB_EXTID_MASK, (fmstr_idRx & ~FMSTR_CAN_EXTID));
            FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET, FMSTR_FCAN_MB_CODE_SHIFT, FMSTR_FCAN_MB_CODE_MASK, FMSTR_FCAN_MB_CODE_TXINACTIVE);
            FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET, FMSTR_FCAN_MB_IDE_BIT);
            FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET + 0x4, FMSTR_FCAN_MB_EXTID_SHIFT, FMSTR_FCAN_MB_EXTID_MASK, (fmstr_idTx & ~FMSTR_CAN_EXTID));
        }

        return FMSTR_TRUE;
    }
    else
    {
        return FMSTR_FALSE;
    }
}

/**************************************************************************//*!
*
* @brief    Enable/Disable interrupt for the CAN transmit Message Buffer
*
******************************************************************************/

static void _FMSTR_S32_EnableTxInterrupt(FMSTR_BOOL enable)
{
    /* Apply enable value to the interrupt mask */
    if (enable)
    {
        FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IMASK1_OFFSET, FMSTR_FLEXCAN_TXMB_BIT);
    }
    else
    {
        FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IMASK1_OFFSET, FMSTR_FLEXCAN_TXMB_BIT);
    }
    /* Check interrupt flag */
    while (FMSTR_TSTBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IFLAG1_OFFSET, FMSTR_FLEXCAN_TXMB_BIT))
    {
        /* Clear interrupt flag by writing the value 1 again (w1c) */
        FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IFLAG1_OFFSET, FMSTR_FLEXCAN_TXMB_BIT);
    }
}

/**************************************************************************//*!
*
* @brief    Enable/Disable interrupt for the CAN receive Message Buffer
*
******************************************************************************/

static void _FMSTR_S32_EnableRxInterrupt(FMSTR_BOOL enable)
{
    /* Apply enable value to the interrupt mask  */
    if (enable)
    {
        FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IMASK1_OFFSET, FMSTR_FLEXCAN_RXMB_BIT);
    }
    else
    {
        FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IMASK1_OFFSET, FMSTR_FLEXCAN_RXMB_BIT);
    }

    /* Check interrupt flag */
    while (FMSTR_TSTBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IFLAG1_OFFSET, FMSTR_FLEXCAN_RXMB_BIT))
    {
        /* Clear interrupt flag by writing the value 1 again (w1c) */
        FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IFLAG1_OFFSET, FMSTR_FLEXCAN_RXMB_BIT);
    }
}

/**************************************************************************//*!
*
* @brief    Enable/re-initialize Receiver buffer
*
******************************************************************************/

static void _FMSTR_S32_EnableRx(void)
{
    /* Rx Message Buffer code set to active and empty */
    FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBRX_OFFSET, FMSTR_FCAN_MB_CODE_SHIFT, FMSTR_FCAN_MB_CODE_MASK, FMSTR_FCAN_MB_CODE_RXEMPTY);
}

/**************************************************************************//*!
*
* @brief    Return size of received CAN frame, or 0 if no Rx frame is available
*
******************************************************************************/

static FMSTR_SIZE8 _FMSTR_S32_GetRxFrameLen(void)
{
    FMSTR_SIZE8 rxfl;

    if (FMSTR_TSTBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IFLAG1_OFFSET, FMSTR_FLEXCAN_RXMB_BIT))
    {
        rxfl = FMSTR_GETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBRX_OFFSET, FMSTR_FCAN_MB_DLC_SHIFT, FMSTR_FCAN_MB_DLC_MASK);

        return rxfl;
    }
    else
    {
        return 0u;
    }
}

/**************************************************************************//*!
*
* @brief    Get data byte at index (0..8)
*
******************************************************************************/

static FMSTR_BCHR _FMSTR_S32_GetRxFrameByte(FMSTR_SIZE8 index)
{
    FMSTR_BCHR rxch;
    rxch = (FMSTR_SIZE8)FMSTR_GETREG8(fmstr_CANBaseAddr, FMSTR_FCAN_MBRX_OFFSET + 0x0B - (0x3 & index) + (0x4 & index));

    return rxch;
}

/**************************************************************************//*!
*
* @brief    Discard received frame and enable receiving a next one.
*
******************************************************************************/

static void _FMSTR_S32_AckRxFrame(void)
{

    /* Check interrupt flag */
    while (FMSTR_TSTBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IFLAG1_OFFSET, FMSTR_FLEXCAN_RXMB_BIT))
    {
        /* Clear interrupt flag by writing the value 1 again (w1c) */
        FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IFLAG1_OFFSET, FMSTR_FLEXCAN_RXMB_BIT);
    }

    /* FCAN reception, reset timer (unlock the buffer) */
    FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_TIMER_OFFSET, FMSTR_FCAN_TIMER_SHIFT, FMSTR_FCAN_TIMER_MASK, 0x0);
}

/**************************************************************************//*!
*
* @brief    Initialize transmit buffer; return false when Tx buffer is not available.
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_S32_PrepareTxFrame(void)
{
    /* FCAN: read TX status register */
    while (FMSTR_TSTBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IFLAG1_OFFSET, FMSTR_FLEXCAN_TXMB_BIT))
    {
        /* Clear interrupt flag by writing the value 1 again (w1c) */
        FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_IFLAG1_OFFSET, FMSTR_FLEXCAN_TXMB_BIT);
    }

    /* Check TX MB status register */
    if (FMSTR_FCAN_MB_CODE_TXINACTIVE == FMSTR_GETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET, FMSTR_FCAN_MB_CODE_SHIFT, FMSTR_FCAN_MB_CODE_MASK))
    {
        if ((fmstr_idTx & FMSTR_CAN_EXTID) == 0)
        {
            /* CAN message buffer with standard Id */
            FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET + 0x4, FMSTR_FCAN_MB_STDID_SHIFT, FMSTR_FCAN_MB_STDID_MASK, fmstr_idTx);
        }
        else
        {
            /* CAN message buffer with extended Id */
            FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET + 0x4, FMSTR_FCAN_MB_EXTID_SHIFT, FMSTR_FCAN_MB_EXTID_MASK, (fmstr_idTx & ~FMSTR_CAN_EXTID));
        }

        return FMSTR_TRUE;
    }
    else
    {
        return FMSTR_FALSE;
    }
}

/**************************************************************************//*!
*
* @brief    Fill one byte of transmit data into buffer.
*
******************************************************************************/

static void _FMSTR_S32_PutTxFrameByte(FMSTR_SIZE8 index, FMSTR_BCHR data)
{
    FMSTR_SETREG8(fmstr_CANBaseAddr, (FMSTR_FCAN_MBTX_OFFSET + 0x0B - (0x3 & index) + (0x4 & index)), data);
}

/**************************************************************************//*!
*
* @brief    Send the Tx buffer.
*
******************************************************************************/

static void _FMSTR_S32_SendTxFrame(FMSTR_SIZE8 len)
{
    FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET, FMSTR_FCAN_MB_DLC_SHIFT, FMSTR_FCAN_MB_DLC_MASK, len);

    FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET, FMSTR_FCAN_MB_RTR_BIT);
    if ((fmstr_idTx & FMSTR_CAN_EXTID) == 0)
    {
        /* Standard Id */
        FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET, FMSTR_FCAN_MB_IDE_BIT);
    }
    else
    {
        /* Extended Id */
        FMSTR_SETBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET, FMSTR_FCAN_MB_IDE_BIT);
    }

    FMSTR_CLRBIT32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET, FMSTR_FCAN_MB_SRR_BIT);

    FMSTR_SETFIELD32(fmstr_CANBaseAddr, FMSTR_FCAN_MBTX_OFFSET, FMSTR_FCAN_MB_CODE_SHIFT, FMSTR_FCAN_MB_CODE_MASK, FMSTR_FCAN_MB_CODE_TXDATA);
}

/**************************************************************************//*!
*
* @brief    Assign FreeMASTER communication module base address
*
******************************************************************************/

void FMSTR_CanSetBaseAddress(FMSTR_ADDR base)
{
    fmstr_CANBaseAddr = base;
}

/**************************************************************************//*!
*
* @brief    Process FreeMASTER CAN interrupt (call this function from CAN ISR handler)
*
******************************************************************************/

void FMSTR_CanIsr()
{
    /* Process incoming or just transmitted byte */
    #if (FMSTR_LONG_INTR) || (FMSTR_SHORT_INTR)
    FMSTR_ProcessCanRx();
    FMSTR_ProcessCanTx();
    #endif
}

#else /* !(FMSTR_DISABLE) */

/* Empty API functions when FMSTR_DISABLE is set */
void FMSTR_SerialSetBaseAddress(FMSTR_ADDR base)
{
    FMSTR_UNUSED(base);
}

void FMSTR_CanIsr()
{
}

#endif /* !(FMSTR_DISABLE) */
#endif /* defined(FMSTR_CAN_DRV) && (FMSTR_MK_IDSTR(FMSTR_CAN_DRV) == FMSTR_CAN_S32_FLEXCAN_ID) */
