/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 *
 * License: NXP LA_OPT_NXP_Software_License
 *
 * NXP Confidential. This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.  This code may only be used in a microprocessor,
 * microcontroller, sensor or digital signal processor ("NXP Product")
 * supplied directly or indirectly from NXP.  See the full NXP Software
 * License Agreement in license/LA_OPT_NXP_Software_License.pdf
 *
 * FreeMASTER Communication Driver - CAN communication
 */

#include "freemaster.h"
#include "freemaster_private.h"

/* Numeric identifier to help pre-processor to identify whether our transport is used or not. */
#define FMSTR_CAN_ID 1

#if (FMSTR_MK_IDSTR(FMSTR_TRANSPORT) == FMSTR_CAN_ID) && (!(FMSTR_DISABLE))

#include "freemaster_protocol.h"
#include "freemaster_can.h"
#include "freemaster_utils.h"

/* CAN driver interface structure, defined in freemaster_defcfg.h */
extern const FMSTR_CAN_DRV_INTF FMSTR_CAN_DRV;

/******************************************************************************
* Local function declarations
******************************************************************************/

static void _FMSTR_Listen(void);
static void _FMSTR_RxDone(void);
static void _FMSTR_SendError(FMSTR_BCHR nErrCode);
static FMSTR_BOOL _FMSTR_TxCan(void);
static FMSTR_BOOL _FMSTR_RxCan(FMSTR_SIZE8 rxLen);

/******************************************************************************
* Local variables
******************************************************************************/

/* FreeMASTER communication buffer (in/out) plus the STS, LEN and CRC bytes */
static FMSTR_BCHR fmstr_pCommBuffer[FMSTR_COMM_BUFFER_SIZE+3+1];

/* FreeMASTER runtime flags */
typedef volatile union
{
    FMSTR_FLAGS all;

    struct
    {
        unsigned bTxActive : 1;        /* response is just being transmitted */
        unsigned bTxFirst : 1;         /* first frame to be send out */
        unsigned bRxActive : 1;        /* just in the middle of receiving (fragmented) frame */
        unsigned bRxFirst : 1;         /* expecting the first frame (FST) */
        unsigned bRxTgl1 : 1;          /* expecting TGL=1 in next frame */
        unsigned bRxFrameReady : 1;    /* frame received (waiting to be handled in poll) */
        unsigned bRxSpecial : 1;       /* special command received (not passed to ProtocolDecode) */

    } flg;

} FMSTR_CAN_FLAGS;

static FMSTR_CAN_FLAGS fmstr_wFlags;

/* receive and transmit buffers and counters */
static FMSTR_SIZE8 fmstr_nTxTodo;        /* transmission to-do counter (0 when tx is idle) */
static FMSTR_BPTR  fmstr_pTxBuff;        /* pointer to next byte to transmit */
static FMSTR_SIZE8 fmstr_nRxCtr;         /* how many bytes received (total across all fragments) */
static FMSTR_BPTR  fmstr_pRxBuff;        /* pointer to next free place in RX buffer */
static FMSTR_BCHR  fmstr_nRxErr;         /* error raised during receive process */
static FMSTR_BCHR  fmstr_nRxCrc8;        /* checksum of data being received for short messages */
static FMSTR_U8    fmstr_uTxCtlByte;
static FMSTR_U8    fmstr_uTxFrmCtr;      /* TX CAN frame counter (within one FreeMASTER frame) */

/* Command ID */
#ifdef FMSTR_CAN_CMDID
    static FMSTR_U32 fmstr_canCmdID = FMSTR_CAN_CMDID;
#else
    static FMSTR_U32 fmstr_canCmdID = 0;
#endif

/* Response ID */
#ifdef FMSTR_CAN_RESPID
    static FMSTR_U32 fmstr_canRespID = FMSTR_CAN_RESPID;
#else
    static FMSTR_U32 fmstr_canRespID = 0;
#endif

#if FMSTR_DEBUG_TX
/* The poll counter is used to roughly measure duration of test frame transmission.
 * The test frame will be sent once per N.times this measured period. */
static FMSTR_S32 fmstr_nDebugTxPollCount;
/* the N factor for multiplying the transmission time to get the wait time */
#define FMSTR_DEBUG_TX_POLLCNT_XFACTOR 256
#define FMSTR_DEBUG_TX_POLLCNT_MIN     (-1*0x100000L)
#endif

/******************************************************************************
* Transport interface
******************************************************************************/

static FMSTR_BOOL _FMSTR_InitCan(void);
static void _FMSTR_Poll(void);
static void _FMSTR_SendResponse(FMSTR_BPTR pResponse, FMSTR_SIZE nLength, FMSTR_U8 statusCode);

const FMSTR_TRANSPORT_INTF FMSTR_CAN =
{
    .Init = _FMSTR_InitCan,
    .Poll = _FMSTR_Poll,
    .SendResponse = _FMSTR_SendResponse,
};

/******************************************************************************
* Implementation
******************************************************************************/

/* CAN initialization */

static FMSTR_BOOL _FMSTR_InitCan(void)
{
    /* Check if mandatory driver functions are present (optional functions are checked before each call) */
#define VALIDATE(x) if(!FMSTR_CAN_DRV.x) return FMSTR_FALSE
    VALIDATE(Init);
    VALIDATE(EnableRx);
    VALIDATE(GetRxFrameLen);
    VALIDATE(GetRxFrameByte);
    VALIDATE(AckRxFrame);
    VALIDATE(PrepareTxFrame);
    VALIDATE(PutTxFrameByte);
    VALIDATE(SendTxFrame);

#if FMSTR_LONG_INTR || FMSTR_SHORT_INTR
    VALIDATE(EnableRxInterrupt);
    VALIDATE(EnableTxInterrupt);  // TODO: remove TX interrupts?
#endif

    /* initialize low-level driver and assign command and response IDs */
    if(!FMSTR_CAN_DRV.Init(fmstr_canCmdID, fmstr_canRespID))
        return FMSTR_FALSE;

    /* initialize all state variables */
    fmstr_wFlags.all = 0U;
    fmstr_nTxTodo = 0U;

#if FMSTR_DEBUG_TX
    /* this zero will initiate the test frame transmission
     * as soon as possible during Listen */
    fmstr_nDebugTxPollCount = 0;
#endif

    /* start listening for commands */
    _FMSTR_Listen();

    return FMSTR_TRUE;
}

/* CAN poll routine */

static void _FMSTR_Poll(void)
{
    /* handle the physical CAN module */
#if FMSTR_POLL_DRIVEN
    FMSTR_ProcessCanRx();
    FMSTR_ProcessCanTx();
#endif

#if FMSTR_POLL_DRIVEN || FMSTR_SHORT_INTR
    /* except in the LONG_INTR mode, the frame gets handled here */
    if(fmstr_wFlags.flg.bRxFrameReady)
        _FMSTR_RxDone();
#endif

#if FMSTR_DEBUG_TX
    /* down-counting the polls for heuristic time measurement */
    if(fmstr_nDebugTxPollCount != 0 && fmstr_nDebugTxPollCount > FMSTR_DEBUG_TX_POLLCNT_MIN)
        fmstr_nDebugTxPollCount--;
#endif
}

/* Assign CAN frame ID for command frame */

void FMSTR_SetCanCmdID(FMSTR_U32 canID)
{
    fmstr_canCmdID = canID;
}

/* Assign CAN frame ID for response frame */

void FMSTR_SetCanRespID(FMSTR_U32 canID)
{
    fmstr_canRespID = canID;
}

/* Start listening on a CAN bus */

static void _FMSTR_Listen(void)
{
    /* disable transmitter state machine */
#if FMSTR_LONG_INTR || FMSTR_SHORT_INTR
    FMSTR_CAN_DRV.EnableTxInterrupt(FMSTR_FALSE);
#endif
    fmstr_wFlags.flg.bTxActive = 0U;

    /* wait for first frame */
    fmstr_wFlags.flg.bRxFrameReady = 0U;
    fmstr_wFlags.flg.bRxFirst = 1U;
    fmstr_wFlags.flg.bRxActive = 1U;

    /* enable CAN receiving */
    FMSTR_CAN_DRV.EnableRx();

#if FMSTR_DEBUG_TX
    /* we have just finished the transmission of the test frame, now wait the 32x times the sendtime
       to receive any command from PC (count<0 is measurement, count>0 is waiting, count=0 is send trigger) */
    if(fmstr_nDebugTxPollCount < 0)
        fmstr_nDebugTxPollCount *= -32;
#endif

#if FMSTR_LONG_INTR || FMSTR_SHORT_INTR
    FMSTR_CAN_DRV.EnableRxInterrupt(FMSTR_TRUE);
#endif
}

/* Send response of given error code (no data) */

static void _FMSTR_SendError(FMSTR_BCHR nErrCode)
{
    /* fill & send single-byte response */
    _FMSTR_SendResponse(&fmstr_pCommBuffer[2], 0U, nErrCode);
}

/*
* Finalize transmit buffer before transmitting
*
* @param nLength - response length (1 for status + data length)
*
* This Function takes the data already prepared in the transmit buffer
* (including the status byte). It computes the check sum and kicks on tx.
*
*/

static void _FMSTR_SendResponse(FMSTR_BPTR pResponse, FMSTR_SIZE nLength, FMSTR_U8 statusCode)
{
    FMSTR_BCHR chSum = 0U;
    FMSTR_U8 i, c;

    if(nLength > 254 || pResponse != &fmstr_pCommBuffer[2])
    {
        /* The Serial driver doesn't support bigger responses than 254 bytes, change the response to status error */
        statusCode = FMSTR_STC_RSPBUFFOVF;
        nLength = 0;
    }

    /* remember the buffer to be sent */
    fmstr_pTxBuff = fmstr_pCommBuffer;

    /* send the message with status, length and the checksum */
    fmstr_nTxTodo = (FMSTR_SIZE8) (nLength + 3);

    if(statusCode & FMSTR_STSF_VARLEN)
    {
        /* variable-length frame starts at [0] and contains the length byte */
        fmstr_pCommBuffer[0] = statusCode;
        fmstr_pCommBuffer[1] = nLength;
    }
    else
    {
        /* fixed-length frame starts at [1] and omits the length byte */
        fmstr_pCommBuffer[1] = statusCode;
        fmstr_pTxBuff++;
        fmstr_nTxTodo--;
    }

    /* Initialize CRC algorithms */
    FMSTR_Crc8Init(&chSum);

    /* status byte and data are already there, compute checksum only */
    pResponse = fmstr_pTxBuff;
    for (i=1U; i<fmstr_nTxTodo; i++)
    {
        pResponse = FMSTR_ValueFromBuffer8(&c, pResponse);
        /* add character to checksum */
        FMSTR_Crc8AddByte(&chSum, c);
    }

    /* store checksum after the message */
    pResponse = FMSTR_ValueToBuffer8(pResponse, chSum);

    /* now transmitting the response */
    fmstr_wFlags.flg.bTxActive = 1U;
    fmstr_wFlags.flg.bTxFirst = 1U;

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Frame will be sent in interrupt asap */
    FMSTR_CAN_DRV.EnableTxInterrupt(FMSTR_TRUE);
#endif

    /* Send now if possible (will keep trying in poll if not possible) */
    FMSTR_ProcessCanTx();
}

/*
* Send one CAN frame if needed. Transmit buffer is already prepared
*
* @return TRUE if frame was actually filled to a buffer and submitted for send
*
* This function is called internally by our interrupt handler or poll routine.
* User may also decide to use FMSTR_CAN_PASSIVE_SW so it is then his responsibility
* to call us when a frame can be send.
*
*/

static FMSTR_BOOL _FMSTR_TxCan(void)
{
    FMSTR_U8 ch;
    FMSTR_SIZE8 i, len = fmstr_nTxTodo;

    if(!fmstr_wFlags.flg.bTxActive || !fmstr_nTxTodo)
        return FMSTR_FALSE;

    if(len > 7)
        len = 7;

    /* first byte is control */
    if(fmstr_wFlags.flg.bTxFirst)
    {
        /* the first frame and the length*/
        fmstr_uTxCtlByte = (FMSTR_U8) (FMSTR_CANCTL_FST | len);
        fmstr_uTxFrmCtr = 0U;
        fmstr_wFlags.flg.bTxFirst = 0U;
    }
    else
    {
        /* the next frame */
        fmstr_uTxCtlByte &= ~(FMSTR_CANCTL_FST | FMSTR_CANCTL_LEN_MASK);
        fmstr_uTxCtlByte ^= FMSTR_CANCTL_TGL;
        fmstr_uTxCtlByte |= len;
        fmstr_uTxFrmCtr++;
    }

    /* is it the last frame? */
    fmstr_nTxTodo -= len;
    if(!fmstr_nTxTodo)
        fmstr_uTxCtlByte |= FMSTR_CANCTL_LST;

    /* put control byte */
    FMSTR_CAN_DRV.PutTxFrameByte(0, fmstr_uTxCtlByte);

    /* put data part */
    for(i=1; i<=len; i++)
    {
        fmstr_pTxBuff = FMSTR_ValueFromBuffer8(&ch, fmstr_pTxBuff);
        FMSTR_CAN_DRV.PutTxFrameByte(i, ch);
    }

    /* submit frame for transmission */
    FMSTR_CAN_DRV.SendTxFrame(len+1);

    /* if the full frame is safe in tx buffer(s), release the received command */
    if(!fmstr_nTxTodo)
    {
        /* no more transmitting */
        fmstr_wFlags.flg.bTxActive = 0U;

        /* start listening immediately (also frees the last received frame) */
        _FMSTR_Listen();
    }

    /* returning TRUE, frame was sent */
    return FMSTR_TRUE;
}

/* Handle received CAN frame
*
* @return TRUE if this frame was processed
*
*/

static FMSTR_BOOL _FMSTR_RxCan(FMSTR_SIZE8 rxLen)
{
    FMSTR_SIZE8 i, len;
    FMSTR_U8 ctl, ch;
    FMSTR_BOOL last;

    if(!fmstr_wFlags.flg.bRxActive)
        return FMSTR_FALSE;

    /* last FreeMASTER frame not yet handled */
    if(fmstr_wFlags.flg.bRxFrameReady)
        return FMSTR_FALSE;

    /* get the first (control) byte */
    ctl = FMSTR_CAN_DRV.GetRxFrameByte(0);

    /* should be master-to-slave (otherwise perhaps a self-received frame) */
    if(!(ctl & FMSTR_CANCTL_M2S))
        goto frame_contd;

    /* first frame resets the state machine */
    if((ctl & FMSTR_CANCTL_FST) || fmstr_wFlags.flg.bRxFirst)
    {
        if(!(ctl & FMSTR_CANCTL_FST) || /* must be the first frame! */
            (ctl & FMSTR_CANCTL_TGL))   /* TGL must be zero! */
        {
            /* frame is ours, but we ignore it */
            goto frame_contd;
        }

        /* now receive the rest of bigger FreeMASTER frame */
        fmstr_wFlags.flg.bRxFirst = 0U;
        fmstr_wFlags.flg.bRxTgl1 = 1U;

        /* special command? */
        if(ctl & FMSTR_CANCTL_SPC)
            fmstr_wFlags.flg.bRxSpecial = 1U;
        else
            fmstr_wFlags.flg.bRxSpecial = 0U;

        /* start receiving the frame */
        fmstr_pRxBuff = fmstr_pCommBuffer;
        fmstr_nRxCtr = 0;
        fmstr_nRxErr = 0;
        FMSTR_Crc8Init(&fmstr_nRxCrc8);
    }
    else
    {
        /* toggle bit should match */
        if((fmstr_wFlags.flg.bRxTgl1 && !(ctl & FMSTR_CANCTL_TGL)) ||
           (!fmstr_wFlags.flg.bRxTgl1 && (ctl & FMSTR_CANCTL_TGL)))
        {
            /* invalid sequence detected */
            fmstr_nRxErr = FMSTR_STC_CANTGLERR;
            goto frame_done;
        }

        /* expect next frame toggled again */
        fmstr_wFlags.flg.bRxTgl1 ^= 1U;
    }

    /* frame is valid, get the data */
    len = (FMSTR_SIZE8) (ctl & FMSTR_CANCTL_LEN_MASK);

    /* sanity check of the real received frame length */
    if(len >= rxLen)
    {
        /* invalid frame length, re-start receiving */
        fmstr_nRxErr = FMSTR_STC_CANMSGERR;
        goto frame_done;
    }

    /* will data fit? (+1 is for checksum byte to fit) */
    if((fmstr_nRxCtr+len) > (FMSTR_COMM_BUFFER_SIZE+1))
    {
        /* this frame must be ignored, start receiving again */
        fmstr_nRxErr = FMSTR_STC_CANMSGERR;
        goto frame_done;
    }

    /* is this the last frame? */
    last = ctl & FMSTR_CANCTL_LST;

    /* okay, receive all data bytes */
    fmstr_nRxCtr += len;
    for(i=1; i<=len; i++)
    {
        ch = FMSTR_CAN_DRV.GetRxFrameByte(i);
        fmstr_pRxBuff = FMSTR_ValueToBuffer8(fmstr_pRxBuff, ch);

        /* the very last byte is a checksum and must match the CRC computed so far*/
        if(last && i==len)
            fmstr_nRxCrc8 = fmstr_nRxCrc8 == ch ? 0 : 1; /* 0 signals a correct CRC */
        else
            FMSTR_Crc8AddByte(&fmstr_nRxCrc8, ch);  /* all other bytes are added to CRC */
    }

    /* was it the last frame? */
    if(!last)
        goto frame_contd;

    /* done with the FreeMASTER frame, it will be handled later in RxDone */
frame_done:
    /* disable receiver so it does not corrupt the frame until it is handled */
    fmstr_wFlags.flg.bRxActive = 0;
    /* FreeMASTER frame received. Checksum and frame will be checked later in RxDone */
    fmstr_wFlags.flg.bRxFrameReady = 1U;

    /* will continue by next chunk next time, but this CAN frame is finished */
frame_contd:
    FMSTR_CAN_DRV.AckRxFrame();
    return FMSTR_TRUE;
}

/* Handle received FreeMASTER frame */

static void _FMSTR_RxDone(void)
{
    FMSTR_BPTR pFrame;
    FMSTR_U8 cmd;
    FMSTR_U8 len;

    /* frame really here? */
    if(!fmstr_wFlags.flg.bRxFrameReady)
        return ;

    /* get the frame information */
    pFrame = fmstr_pCommBuffer;
    cmd = 0;
    len = 0;

    /* acknowledge the flag, we're going to process the frame now */
    fmstr_wFlags.flg.bRxFrameReady = 0U;

    /* any fragmentation error detected during the receive process? */
    if(!fmstr_nRxErr)
    {
        /* checksum should be zeroed (see above when last byte was received) */
        if(fmstr_nRxCrc8)
        {
            fmstr_nRxErr = FMSTR_STC_CMDCSERR;
        }
        /* checksum okay, check frame length */
        else
        {
            /* get command byte */
            pFrame = FMSTR_ValueFromBuffer8(&cmd, pFrame);
            /* get length */
            pFrame = FMSTR_ValueFromBuffer8(&len, pFrame);

            /* now the len received should match the data bytes received
               note that command-byte, length and checksum are included in the nRxCtr */
            if(fmstr_nRxCtr != (len+3))
                fmstr_nRxErr = FMSTR_STC_CMDCSERR;
        }
    }

    /* any error? */
    if(fmstr_nRxErr)
    {
        _FMSTR_SendError(fmstr_nRxErr);
    }
    /* no error */
    else
    {
        /* special CAN command? for our layer only */
        if(fmstr_wFlags.flg.bRxSpecial)
        {
            switch(cmd)
            {
            case FMSTR_CANSPC_PING:
                _FMSTR_SendError(FMSTR_STS_OK);
                break;

            default:
                /* unknown command */
                _FMSTR_SendError(FMSTR_STC_INVCMD);
            }
        }
        /* standard FreeMASTER command to be passed above */
        else
        {
            /* decode and handle frame by SCI classic driver */
            if(!FMSTR_ProtocolDecoder(pFrame, len,  cmd))
            {
                /* if no response was generated, start listening again, otherwise,
                   the receive will be initiated after transmission  is complete in
                   _FMSTR_TxCan (this prevents our TX buffer to be corrupted by RX) */
                _FMSTR_Listen();
            }
        }
    }
}

/**************************************************************************//*!
*
* @brief    Handle CAN communication
*
* @note This function can be called either from CAN ISRs or from the polling routine
*
******************************************************************************/

void FMSTR_ProcessCanRx(void)
{
    /* If there is any frame received */
    FMSTR_SIZE8 len = FMSTR_CAN_DRV.GetRxFrameLen();
    if(len > 0)
    {
#if FMSTR_LONG_INTR || FMSTR_SHORT_INTR
        /* In interrupt mode, we need to disable interrupt when frame was not handled. 
         * This prevents servicing the interrupt again endlessly. */
        FMSTR_BOOL processed = _FMSTR_RxCan(len);
        FMSTR_CAN_DRV.EnableRxInterrupt(processed);
#else
        /* In polled mode, we just keep trying to process the frame. */
        _FMSTR_RxCan(len);
#endif
            
#if FMSTR_LONG_INTR
        /* Handle completed frame now? (may be we're in the interrupt now). Otherwise wait to Poll */
        if(fmstr_wFlags.flg.bRxFrameReady)
            _FMSTR_RxDone();
#endif
    }
#if FMSTR_DEBUG_TX
    /* Time to send another test frame? */
    else if(fmstr_doDebugTx && fmstr_nDebugTxPollCount == 0)
    {
        /* yes, start sending it now */
        if(FMSTR_SendTestFrame(fmstr_pCommBuffer))
        {
            /* measure how long it takes to transmit it */
            fmstr_nDebugTxPollCount = -1;
        }
    }
#endif
}

/**************************************************************************//*!
*
* @brief    Handle CAN communication
*
* @note This function can be called either from CAN ISRs or from the polling routine
*
******************************************************************************/

void FMSTR_ProcessCanTx(void)
{
    /* Any TX buffer available? */
    if(fmstr_wFlags.flg.bTxActive && FMSTR_CAN_DRV.PrepareTxFrame())
    {
#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
        /* send one CAN frame */
        if(!_FMSTR_TxCan())
        {
            /* no more frames, disable TX Interrupt */
            FMSTR_CAN_DRV.EnableTxInterrupt(FMSTR_FALSE);
        }
#else
        /* send if you have anything to be sent */
        _FMSTR_TxCan();
#endif
    }
}

#endif /* defined(FMSTR_CAN_DRV)) && (!(FMSTR_DISABLE)) */
