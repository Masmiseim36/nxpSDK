/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
 
/*!=============================================================================
\file       SerialLink.c
\brief      Serial Link to Host
==============================================================================*/
#ifdef DEBUG_SERIAL_LINK
#define DEBUG_SL            TRUE
#else
#define DEBUG_SL            FALSE
#endif

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include <string.h>
#include <dbg.h>

#include "SerialLink.h"
#include "MemManager.h"
#include "SerialManager.h"
#if gFsciIncluded_c
#include "FsciInterface.h"
#include "FsciCommunication.h"
#endif
#include "FunctionLib.h"
#include "app_zb_utils.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/** Enumerated list of states for receive state machine */
typedef enum
{
    E_STATE_RX_WAIT_START,
    E_STATE_RX_WAIT_TYPEMSB,
    E_STATE_RX_WAIT_TYPELSB,
    E_STATE_RX_WAIT_LENMSB,
    E_STATE_RX_WAIT_LENLSB,
    E_STATE_RX_WAIT_CRC,
    E_STATE_RX_WAIT_DATA,
}teSL_RxState;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

PUBLIC uint8 u8SL_CalculateCRC(uint16 u16Type, uint16 u16Length, uint8 *pu8Data);
#if JENNIC_DEBUG_ENABLE   
PRIVATE void vLogInit(void);
PRIVATE void vLogFlush(void);
PRIVATE void vLogAssert(void);
#endif
PRIVATE void vLogPutch(char c);

#ifdef CCITT_CRC
PRIVATE uint8 u8CCITT_CRC(uint8 u8CRCIn, uint8 u8Val);
#endif

#if !gFsciIncluded_c
PRIVATE uint16 SL_CountSpecialChars(uint8 * pData, uint16 len);
PRIVATE uint8* SL_AddByte(uint8 * p, uint8 u8Data);
#endif

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

uint8     au8LogBuffer[256];
uint8     u8LogStart = 0;
uint8     u8LogEnd   = 0;
bool_t    bLogging = FALSE;
uint8     u8SerialId;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: bSL_ReadMessage
 *
 * DESCRIPTION:
 * Attempt to read a complete message from the serial link
 *
 * PARAMETERS  Name                    RW  Usage
 *             pu16Type                W   Location to store incoming message type
 *             pu16Length              W   Location to store incoming message length
 *             u16MaxLength            R   Length of allocated message buffer
 *             pu8Message              W   Location to store message payload
 *
 * RETURNS:
 * TRUE if a complete valid message has been received
 *
 ****************************************************************************/
PUBLIC bool bSL_ReadMessage(uint16 *pu16Type, uint16 *pu16Length, uint16 u16MaxLength, uint8 *pu8Message,uint8 u8Data)
{

    static teSL_RxState eRxState = E_STATE_RX_WAIT_START;
    static uint8 u8CRC;
    static uint16 u16Bytes;
    static bool bInEsc = FALSE;

    switch(u8Data)
    {

        case SL_START_CHAR:
            // Reset state machine
            u16Bytes = 0;
            bInEsc = FALSE;
            DBG_vPrintf(DEBUG_SL, "\nRX Start ");
            eRxState = E_STATE_RX_WAIT_TYPEMSB;
            break;

        case SL_ESC_CHAR:
            // Escape next character
            bInEsc = TRUE;
            break;

        case SL_END_CHAR:
            // End message
            DBG_vPrintf(DEBUG_SL, "\nGot END");
            eRxState = E_STATE_RX_WAIT_START;
            if(*pu16Length < u16MaxLength)
            {
                if(u8CRC == u8SL_CalculateCRC(*pu16Type, *pu16Length, pu8Message))
                {
                    /* CRC matches - valid packet */
                    DBG_vPrintf(DEBUG_SL, "\nbSL_ReadMessage(%d, %d, %02x)", *pu16Type, *pu16Length, u8CRC);
                    return(TRUE);
                }
            }
            DBG_vPrintf(DEBUG_SL, "\nCRC BAD");
            break;

        default:
            if(bInEsc)
            {
                /* Unescape the character */
                u8Data ^= 0x10;
                bInEsc = FALSE;
            }
            DBG_vPrintf(DEBUG_SL, "\nData 0x%x", u8Data & 0xFF);

            switch(eRxState)
            {

            case E_STATE_RX_WAIT_START:
                break;

            case E_STATE_RX_WAIT_TYPEMSB:
                *pu16Type = (uint16)u8Data << 8;
                eRxState++;
                break;

            case E_STATE_RX_WAIT_TYPELSB:
                *pu16Type += (uint16)u8Data;
                DBG_vPrintf(DEBUG_SL, "\nType 0x%x", *pu16Type & 0xFFFF);
                eRxState++;
                break;

            case E_STATE_RX_WAIT_LENMSB:
                *pu16Length = (uint16)u8Data << 8;
                eRxState++;
                break;

            case E_STATE_RX_WAIT_LENLSB:
                *pu16Length += (uint16)u8Data;
                DBG_vPrintf(DEBUG_SL, "\nLength %d", *pu16Length);
                if(*pu16Length > u16MaxLength)
                {
                    DBG_vPrintf(DEBUG_SL, "\nLength > MaxLength");
                    eRxState = E_STATE_RX_WAIT_START;
                }
                else
                {
                    eRxState++;
                }
                break;

            case E_STATE_RX_WAIT_CRC:
                DBG_vPrintf(DEBUG_SL, "\nCRC %02x\n", u8Data);
                u8CRC = u8Data;
                eRxState++;
                break;

            case E_STATE_RX_WAIT_DATA:
                if(u16Bytes < *pu16Length)
                {
                    DBG_vPrintf(DEBUG_SL, "%02x ", u8Data);
                    pu8Message[u16Bytes++] = u8Data;
                }
                break;
            }
            break;
    }
    return(FALSE);
}


/****************************************************************************
 *
 * NAME: vSL_WriteMessage
 *
 * DESCRIPTION:
 * Write message to the serial link
 *
 * PARAMETERS: Name                   RW  Usage
 *             u16Type                R   Message type
 *             u16Length              R   Message length
 *             pu8Data                R   Message payload
 * RETURNS:
 * void
 ****************************************************************************/
PUBLIC void vSL_WriteMessage(uint16 u16Type, uint16 u16Length, uint8 *pu8Data)
{
    uint16 len;
    uint8 *pTxData;
    uint8 *p;

#if !gFsciIncluded_c
    int n;
    uint8 u8CRC;
#endif

#if gFsciIncluded_c
    len = u16Length + sizeof(clientPacketHdr_t) + gFsci_TailBytes_c;
#else
    u8CRC = u8SL_CalculateCRC(u16Type, u16Length, pu8Data);

    DBG_vPrintf(DEBUG_SL, "\nvSL_WriteMessage(%d, %d, %02x)", u16Type, u16Length, u8CRC);

    len = 1 + sizeof(u16Type) + sizeof(u16Length) + sizeof(u8CRC) + u16Length + 1;
    len += SL_CountSpecialChars((uint8*)&u16Type, sizeof(u16Type));
    len += SL_CountSpecialChars((uint8*)&u16Length, sizeof(u16Length));
    len += SL_CountSpecialChars(&u8CRC, sizeof(u8CRC));
    len += SL_CountSpecialChars(pu8Data, u16Length);
#endif
    
    pTxData = MEM_BufferAlloc(len);
    
    if (pTxData)
    {
        p = pTxData;
#if gFsciIncluded_c
        /* NOTE: When using the FSCI protocol, there is no need to include additional start/end chars, CRC, msg length fields and apply the escape sequence */
        *p++ = gFSCI_StartMarker_c;
        *p++ = ((u16Type >> 8) & 0xFF) + ZB_FSCI_CMD_OFFSET;
        *p++ = (u16Type >> 0) & 0xFF;
        /* Copy length of the FSCI packet */
        FLib_MemCpy(p, &u16Length, sizeof(fsciLen_t));
        p += sizeof(fsciLen_t);
        /* Copy payload */
        FLib_MemCpy(p, pu8Data, u16Length);

        FSCI_transmitFormatedPacket((clientPacket_t*)pTxData, u8SerialId);
#else
        /* Send start character */
        *p++ = SL_START_CHAR;

        /* Send message type */
        p = SL_AddByte(p, (u16Type >> 8) & 0xff);
        p = SL_AddByte(p, (u16Type >> 0) & 0xff);

        /* Send message length */
        p = SL_AddByte(p, (u16Length >> 8) & 0xff);
        p = SL_AddByte(p, (u16Length >> 0) & 0xff);

        /* Send message checksum */
        p = SL_AddByte(p, u8CRC);

        /* Send message payload */
        for(n = 0; n < u16Length; n++)
        {
            p = SL_AddByte(p, pu8Data[n]);
        }

        /* Send end character */
        *p++ = SL_END_CHAR;
        
        Serial_AsyncWrite (u8SerialId, pTxData, len, (pSerialCallBack_t)MEM_BufferFree, pTxData);
#endif
    }
}


/****************************************************************************
 *
 * NAME: vSL_LogSend
 *
 * DESCRIPTION:
 * Send log messages from the log buffer to the host
 *
 * PARAMETERS:  Name                RW  Usage
 *
 * RETURNS:
 * void
 ****************************************************************************/
PUBLIC void vSL_LogSend(void)
{
    int n;
#if !gFsciIncluded_c
    uint8  u8CRC;
#endif
    uint8  u8Length = 0;
    uint16 len = 0;
    uint8 *pTxData;
    uint8 *p;

    while (u8LogEnd - u8LogStart != 0)
    {
#if gFsciIncluded_c
        for (u8Length = 0; au8LogBuffer[(u8LogStart + u8Length) & 0xFF] != '\0'; u8Length++)
        {
        }
        len = u8Length + sizeof(clientPacketHdr_t) + gFsci_TailBytes_c;
#else
        u8CRC = ((E_SL_MSG_LOG >> 8) & 0xff) ^ ((E_SL_MSG_LOG >> 0) & 0xff);

        for (u8Length = 0; au8LogBuffer[(u8LogStart + u8Length) & 0xFF] != '\0'; u8Length++)
        {
            u8CRC ^= au8LogBuffer[(u8LogStart + u8Length) & 0xFF];

            if (au8LogBuffer[(u8LogStart + u8Length) & 0xFF] < 0x10)
            {
                len++;
            }
        }
        //u8CRC ^= 0;
        u8CRC ^= u8Length;
        /* Take into account the extra SL_ESC_CHARs */
        len += 1 + sizeof(uint16) + sizeof(uint16) + sizeof(uint8) + u8Length + 1 + 4;
#endif
        
        pTxData = MEM_BufferAlloc(len);
        
        if (pTxData)
        {
            p = pTxData;
#if gFsciIncluded_c
            *p++ = gFSCI_StartMarker_c;
            *p++ = ((E_SL_MSG_LOG >> 8) & 0xFF) + ZB_FSCI_CMD_OFFSET;
            *p++ = (E_SL_MSG_LOG >> 0) & 0xFF;
            /* Copy length of the FSCI packet */
            FLib_MemCpy(p, &u8Length, sizeof(fsciLen_t));
            p += sizeof(fsciLen_t);
            /* Copy payload */
            for(n = 0; n < u8Length; n++)
            {
                *p++ = au8LogBuffer[u8LogStart];
                u8LogStart++;
            }
            u8LogStart++;
            
            FSCI_transmitFormatedPacket((clientPacket_t*)pTxData, u8SerialId);
#else
            /* Send start character */
            *p++ = SL_START_CHAR;
            
            /* Send message type */
            p = SL_AddByte(p, (E_SL_MSG_LOG >> 8) & 0xff);
            p = SL_AddByte(p, (E_SL_MSG_LOG >> 0) & 0xff);
            
            /* Send message length */
            p = SL_AddByte(p, 0);
            p = SL_AddByte(p, u8Length);
            
            /* Send message checksum */
            p = SL_AddByte(p, u8CRC);
            
            /* Send message payload */
            for(n = 0; n < u8Length; n++)
            {
                p = SL_AddByte(p, au8LogBuffer[u8LogStart]);
                u8LogStart++;
            }
            u8LogStart++;
            
            /* Send end character */
            *p++ = SL_END_CHAR;
            len = p - pTxData;
            
            Serial_AsyncWrite (u8SerialId, pTxData, len, (pSerialCallBack_t)MEM_BufferFree, pTxData);
#endif
        }
    }
}

/****************************************************************************
 *
 * NAME: vSL_WriteLog
 *
 * DESCRIPTION:
 * Writes log message to the buffer
 *
 * PARAMETERS:  Name                RW  Usage
 *
 * RETURNS:
 * void
 ****************************************************************************/
PUBLIC void vSL_WriteLog(char * fmt, ...)
{
    va_list ap;
    uint16_t n;
    uint8_t placeholders = 0;
    char *pStr;
    bLogging = FALSE;
    
    /* Get size of string and check for placeholders */
    for (n=0; fmt[n] != '\0'; n++)
    {
        if (fmt[n] == '%')
        {
            placeholders++;
        }
    }
    
    if (placeholders == 0)
    {
        /* If there are no placeholders, just copy the string to the buffer */
        for (n=0; fmt[n] != '\0'; n++)
        {
            vLogPutch(fmt[n]);
        }
    }
    else
    {
        /* Allocate a memory buffer to populate placeholders. Account for placeholders length */
        pStr = MEM_BufferAlloc(n + MAX_PARAM_LEN);
        
        if (pStr)
        {
            uint32_t i = 0;
            int len;
            
            n = MEM_BufferGetSize(pStr);
            
            va_start(ap, fmt);
            
            for ( ;*fmt != '\0'; fmt++)
            {
                if (*fmt == '%')
                {
                    len = n-i; /* remaining space */
                    fmt = pccHandlePlaceHolder (fmt + 1, &ap, &pStr[i], &len);
                    i += len;
                }
                else if ((*fmt == '\n') && (i < (n - 1)))
                {
                    pStr[i++] = '\r';
                    pStr[i++] = '\n';
                }
                else if (i < n)
                {
                    pStr[i++] = *fmt;
                }
            }
            
            n = i;
            
            va_end(ap);
            
            /* If there are no placeholders, just copy the string to the buffer */
            for (n=0; n < i; n++)
            {
                vLogPutch(pStr[n]);
            }
            
            MEM_BufferFree(pStr);
        }
    }
    
    au8LogBuffer[u8LogEnd] = '\0';
    u8LogEnd++;
}
/****************************************************************************
 *
 * NAME: vSL_LogFlush
 *
 * DESCRIPTION:
 * Flush any log messages from the outoing queue
 *
 * PARAMETERS:  Name                RW  Usage
 *
 * RETURNS:
 * void
 ****************************************************************************/
PUBLIC void vSL_LogFlush(void)
{
    /* Copy log buffer to the UART buffer for transmission */
    /* flush hardware buffer */
    vSL_LogSend();
}

/****************************************************************************
 *
 * NAME: vSL_LogInit
 *
 * DESCRIPTION:
 * Initialise Serial Link logging
 * Set up DBG module to use serial link functions for its output
 *
 * PARAMETERS:  Name                RW  Usage
 *
 * RETURNS:
 * void
 ****************************************************************************/
PUBLIC void vSL_LogInit(void)
{
#if JENNIC_DEBUG_ENABLE    
    tsDBG_FunctionTbl sFunctionTbl;

    sFunctionTbl.prInitHardwareCb   = vLogInit;
    sFunctionTbl.prPutchCb          = vLogPutch;
    sFunctionTbl.prFlushCb          = vLogFlush;
    sFunctionTbl.prFailedAssertCb   = vLogAssert;

    DBG_vInit(&sFunctionTbl);
#endif   
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

#ifndef CCITT_CRC
/****************************************************************************
 *
 * NAME: u8SL_CalculateCRC
 *
 * DESCRIPTION:
 * Calculate CRC of packet
 *
 * PARAMETERS: Name                   RW  Usage
 *             u8Type                 R   Message type
 *             u16Length              R   Message length
 *             pu8Data                R   Message payload
 * RETURNS:
 * CRC of packet
 ****************************************************************************/
PUBLIC uint8 u8SL_CalculateCRC(uint16 u16Type, uint16 u16Length, uint8 *pu8Data)
{

    int n;
    uint8 u8CRC;

    u8CRC  = (u16Type   >> 0) & 0xff;
    u8CRC ^= (u16Type   >> 8) & 0xff;
    u8CRC ^= (u16Length >> 0) & 0xff;
    u8CRC ^= (u16Length >> 8) & 0xff;

    for(n = 0; n < u16Length; n++)
    {
        u8CRC ^= pu8Data[n];
    }

    return(u8CRC);
}

#endif

/****************************************************************************
 *
 * NAME: vLogPutch
 *
 * DESCRIPTION:
 * Callback function for DBG module to write out characters
 *
 * PARAMETERS:  Name                RW  Usage
 *
 * RETURNS:
 * void
 ****************************************************************************/
PRIVATE void vLogPutch(char c)
{
    if (!bLogging)
    {
        /* Handle first character being the log level */
        if (c < 7)
        {
            /* Ensure log level is LOG_INFO or higher */
            au8LogBuffer[u8LogEnd] = c;
        }
        else
        {
            au8LogBuffer[u8LogEnd] = 6;
        }
        u8LogEnd++;
    }

    if (c >= 0x20 && c < 0x7F)
    {
        /* Add ASCII characters to the output buffer */
        au8LogBuffer[u8LogEnd] = c;
        u8LogEnd++;
    }

    bLogging = TRUE;
}

#if JENNIC_DEBUG_ENABLE 
/****************************************************************************
 *
 * NAME: vLogInit
 *
 * DESCRIPTION:
 * Callback function for DBG module to initialise output
 *
 * PARAMETERS:  Name                RW  Usage
 *
 * RETURNS:
 * void
 ****************************************************************************/
PRIVATE void vLogInit(void)
{

}

/****************************************************************************
 *
 * NAME: vLogFlush
 *
 * DESCRIPTION:
 * Callback function for DBG module to flush output buffer - used to terminate
 * an entry in the logbuffer
 *
 * PARAMETERS:  Name                RW  Usage
 *
 * RETURNS:
 * void
 ****************************************************************************/
PRIVATE void vLogFlush(void)
{
    au8LogBuffer[u8LogEnd] = '\0';
    u8LogEnd++;
    bLogging = FALSE;
    vSL_LogSend();
}

/****************************************************************************
 *
 * NAME: vLogAssert
 *
 * DESCRIPTION:
 * Callback function for DBG module to assert - not used
 *
 * PARAMETERS:  Name                RW  Usage
 *
 * RETURNS:
 * void
 ****************************************************************************/
PRIVATE void vLogAssert(void)
{

}
#endif
#ifdef CCITT_CRC
/****************************************************************************
 *
 * NAME: u8CCITT_CRC
 *
 * DESCRIPTION:
 * Calculate CCITT CRC8 of the supplied data.
 * Polynomial: x^8 + x^2 + x + 1 (0x07).
 *
 * CRC Calculation. Polynomial =  x^8 + x^2 + x + 1. Initial Value = 00, Terms = 8
 *
 * Example
 * -------
 * Val = 0x11, CRC = 0x77
 * Val = 0x22, CRC = 0xac
 * Val = 0x33, CRC = 0xd4
 * Val = 0x44, CRC = 0xf9
 * Val = 0x55, CRC = 0x4d
 * Val = 0x66, CRC = 0xd1
 * Val = 0x77, CRC = 0x7b
 * Val = 0x88, CRC = 0xd7 (Calculated result)
 *
 * PARAMETERS: Name                   RW  Usage
 *             u8CRCIn                R   Seed
 *             u8Val                  R   Value to apply CRC polynomial to
 * RETURNS:
 * CCITT CRC8
 *
 * NOTES:
 *
 ****************************************************************************/
PRIVATE uint8 u8CCITT_CRC(uint8 u8CRCIn, uint8 u8Val)
{
    uint8 i=8;                          /* test all 8 bits */
    uint8 u8CRCOut = u8CRCIn ^ u8Val;   /* x^8 is set */

    while(i--)
    {
        if(u8CRCOut & 0x80)             /* test next bit */
        {
            u8CRCOut <<= 1;
            u8CRCOut ^= 0x07;           /* add polynomial x^2 x^1 x^0 */
        }
        else
        {                               /* don't add polynomial */
            u8CRCOut <<=1 ;
        }
    }
    return u8CRCOut;
}

/****************************************************************************
 *
 * NAME: u8SL_CalculateCRC
 *
 * DESCRIPTION:
 * Calculate CRC of packet. Includes Length, Type then Data.
 *
 * PARAMETERS: Name                  RW  Usage
 *             u8Length              R   Message length
 *             u8Type                R   Message type
 *             pu8Data               R   Message payload
 * RETURNS:
 * CRC of packet
 ****************************************************************************/
PUBLIC uint8 u8SL_CalculateCRC(uint16 u16Type, uint16 u16Length,  uint8 *pu8Data)
{
    uint16 n;
    uint8 u8CRC=0;

    u8CRC = 0;      /*  seed with zero */
    u8CRC  = u8CCITT_CRC(u8CRC, ((u16Type   >> 0) & 0xff));
    u8CRC  = u8CCITT_CRC(u8CRC, ((u16Type   >> 8) & 0xff));
    u8CRC  = u8CCITT_CRC(u8CRC, ((u16Length >> 0) & 0xff));
    u8CRC  = u8CCITT_CRC(u8CRC, ((u16Length >> 8) & 0xff));

    for(n = 0; n < u16Length; n++)
    {
        u8CRC = u8CCITT_CRC(u8CRC, pu8Data[n]);
    }
    return u8CRC;
}
#endif

#if !gFsciIncluded_c
PRIVATE uint16 SL_CountSpecialChars(uint8 * pData, uint16 len)
{
    uint16 count = 0;

    while (len--)
    {
        if (*pData < 0x10)
        {
            count++;
        }
        pData++;
    }

    return count;
}

PRIVATE uint8* SL_AddByte(uint8 * p, uint8 u8Data)
{
    if(u8Data < 0x10)
    {
        /* Send escape character and escape byte */
        u8Data ^= 0x10;
        *p++ = SL_ESC_CHAR;
    }
    *p++ = (u8Data);
    
    return p;
}
#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

