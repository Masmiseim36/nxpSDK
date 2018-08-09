/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
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

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"

#include "FunctionLib.h"
#include "PhyInterface.h"

#include "string.h"
#include <dbg.h>
#include "EH_IEEE_802154_Switch.h"
#include "EH_Switch_Configurations.h"
#include "EH_IEEE_Features.h"
#include "EH_IEEE_Commands.h"
#include "EH_Button.h"

#ifdef DK4
#include "app_main.h"
#include "ZTimer.h"
#endif

#include "PhyInterface.h"
#include "MacInterface.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/* DIO Used for Cap Switching */
#define DIO_1 (0x00000002)
#define DIO_2 (0x00000004)
#define DIO_3 (0x00000008)
/* Mask for all Cap switching buttons */
#define DIO_CAP_MASK (DIO_1|DIO_2|DIO_3)

#define BUTTON_PRESSED       0x00               /*No DIO set */
#define BUTTON_RELEASED      DIO_8              /*DIO 8 set when button released*/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
volatile bool_t bEnableSleep = TRUE;

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
/* bTxComplete is used to flag TX completion from ISR to main code */
volatile bool_t bTxComplete;

/*bRxComplete is used to flag RX completion from ISR to main code */
volatile bool_t bRxComplete;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: vSendMACFrame
 *
 * DESCRIPTION:
 * Transmits MAC frame and updates persistent data
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vSendMACFrame( bool bUpdatePersistentData)
{
    uint8 u8TxRepeats = 0;

    if(bUpdatePersistentData)
    {
        /* update persistence data and button press count*/
#ifdef GPD_SUPPORT_PERSISTENT_DATA
        bAHI_WriteEEPROMBlock(EEPROM_START_SECTOR,
                sizeof(tGPD_PersistantData),
                (uint8 *)&sGPDPersistentData);
#endif
    }
    
    /* Set Channel */
    macToPlmeMessage_t plmeMsg;
    plmeMsg.msgType = gPlmeSetReq_c;
    plmeMsg.msgData.setReq.PibAttribute = gPhyPibCurrentChannel_c;
    plmeMsg.msgData.setReq.PibAttributeValue = sGPDPersistentData.u8Channel;
    plmeMsg.macInstance = 0;
    MAC_PLME_SapHandler(&plmeMsg, 0);
    
    do
    {
        /* Set transmit flag to false */
        bTxComplete = FALSE;
        /* Start Transmit with no CCA or ACK */
        static macToPdDataMessage_t *pMsg = NULL;
        
        if (pMsg == NULL)
        {
            pMsg = MEM_BufferAlloc(sizeof(macToPdDataMessage_t) + gMaxPHYPacketSize_c);
        }
        
        if (pMsg)
        {
            uint8_t dstAddrMode = (sGPD_MACFrame.u16FCF & 0x0C00) >> 10;
            uint8_t srcAddrMode = (sGPD_MACFrame.u16FCF & 0xC000) >> 14;
            uint8_t panIdCompression = sGPD_MACFrame.u16FCF & (1<<6);
            uint8_t *p;
            
            pMsg->msgType = gPdDataReq_c;
            pMsg->macInstance = 0;
            
            //Fill all data request fields:
            pMsg->msgData.dataReq.startTime   = gPhySeqStartAsap_c;
            pMsg->msgData.dataReq.txDuration  = 0xFFFFFFFF;
            pMsg->msgData.dataReq.slottedTx   = gPhyUnslottedMode_c;
            pMsg->msgData.dataReq.CCABeforeTx = gPhyNoCCABeforeTx_c;
            pMsg->msgData.dataReq.ackRequired = gPhyNoAckRqd_c;
            pMsg->msgData.dataReq.pPsdu       = (uint8_t*)&pMsg->msgData.dataReq.pPsdu + 
                                                sizeof(pMsg->msgData.dataReq.pPsdu);
            
            /* Create a valid 802.15.4 frame */
            FLib_MemCpy(pMsg->msgData.dataReq.pPsdu, &sGPD_MACFrame.u16FCF, sizeof(uint16_t));
            
            p = &pMsg->msgData.dataReq.pPsdu[2];
            *p++ = sGPD_MACFrame.u8SequenceNum;
            
            switch (dstAddrMode)
            {
            case 2:
                FLib_MemCpy(p, &sGPD_MACFrame.u16DestPAN, sizeof(uint16_t));
                p += sizeof(uint16_t);
                FLib_MemCpy(p, &sGPD_MACFrame.uDestAddr.u16Short, sizeof(uint16_t));
                p += sizeof(uint16_t);
                break;
            case 3:
                FLib_MemCpy(p, &sGPD_MACFrame.u16DestPAN, sizeof(uint16_t));
                p += sizeof(uint16_t);
                FLib_MemCpy(p, &sGPD_MACFrame.uDestAddr.sExt.u32L, sizeof(uint32_t));
                p += sizeof(uint32_t);
                FLib_MemCpy(p, &sGPD_MACFrame.uDestAddr.sExt.u32H, sizeof(uint32_t));
                p += sizeof(uint32_t);
                break;
            }
            
            switch (srcAddrMode)
            {
            case 2:
                if (!panIdCompression)
                {
                    FLib_MemCpy(p, &sGPD_MACFrame.u16SrcPAN, sizeof(uint16_t));
                    p += sizeof(uint16_t);
                }
                FLib_MemCpy(p, &sGPD_MACFrame.uSrcAddr.u16Short, sizeof(uint16_t));
                p += sizeof(uint16_t);
                break;
            case 3:
                if (!panIdCompression)
                {
                    FLib_MemCpy(p, &sGPD_MACFrame.u16SrcPAN, sizeof(uint16_t));
                    p += sizeof(uint16_t);
                }
                FLib_MemCpy(p, &sGPD_MACFrame.uSrcAddr.sExt.u32L, sizeof(uint32_t));
                p += sizeof(uint32_t);
                FLib_MemCpy(p, &sGPD_MACFrame.uSrcAddr.sExt.u32H, sizeof(uint32_t));
                p += sizeof(uint32_t);
                break;
            }
            
            /* Unsecured frame! No Auxiliary Security Header! */
            
            /* Copy MAC payload */
            FLib_MemCpy(p, sGPD_MACFrame.uPayload.au8Byte, sGPD_MACFrame.u8PayloadLength);
            p += sGPD_MACFrame.u8PayloadLength;
            
            /* FCS is added by hardware */
            
            /* Compute PSDU length */
            pMsg->msgData.dataReq.psduLength = (p - pMsg->msgData.dataReq.pPsdu);
            
            MAC_PD_SapHandler(pMsg, 0);
        }
        
        /* While call-back not reset flag */
        while (bTxComplete != TRUE)
        {
            ZTIMER_vTask();
            /* Interrupt will be generated when finished, so can doze now */
            //vAHI_CpuDoze();
        }

        /* add one to repeat value */
        u8TxRepeats++;
        DBG_vPrintf(DBG_DEVICE_SWITCH, "\n vSendMACFrame channel = %d  \n", sGPDPersistentData.u8Channel );
        /* Keep transmitting until counter reached or power runs out
        * or if powered by mains for a max of 20 packets */
    } while (u8TxRepeats < sGPDPersistentData.u8NoOfTransPerChannel);
}
/****************************************************************************
 *
 * NAME: vEnableRxAndReceive
 *
 * DESCRIPTION:
 * Turns on Rx and receives data
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
#if defined( GPD_RX_AFTER_TX) || defined(GPD_RX_ENABLE)
void vEnableRxAndReceive(void)
{
    /* MAC clock gives time in 16us resolution */
    /* keep time as 10 milli seconds  */
    bRxComplete = FALSE;

    /* Set RX state On/Off*/
    macToPlmeMessage_t plmeMsg;
    plmeMsg.msgType = gPlmeSetReq_c;
    plmeMsg.msgData.setReq.PibAttribute = gPhyPibRxOnWhenIdle;
    plmeMsg.msgData.setReq.PibAttributeValue = 1; /* Receiver state */
    plmeMsg.macInstance = 0;
    MAC_PLME_SapHandler ( &plmeMsg, 0 );

    ZTIMER_eStop(u8TimerReceive);
    ZTIMER_eStart(u8TimerReceive, ZTIMER_TIME_MSEC(250));

    while (FALSE == bRxComplete)
    {
         if (ZTIMER_eGetState(u8TimerReceive) == E_ZTIMER_STATE_EXPIRED)
         {
            ZTIMER_eStop(u8TimerReceive);
            DBG_vPrintf(DBG_DEVICE_SWITCH, "\n Stop wait to receive \n" );
            break;
        }
        ZTIMER_vTask();
    }

    if(bRxComplete)
    {
        /* Process Received frame */
        DBG_vPrintf(DBG_DEVICE_SWITCH, "\n bRxComplete  vHandleRxMacFrame \n" );
        vHandleRxMacFrame();
    }
    else
    {
        DBG_vPrintf(DBG_DEVICE_SWITCH, "\n MAC receive Timer Expired \n" );
    }
}
#endif
/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: SOC_ZPS_vSetMacAddrLocation
 *
 * DESCRIPTION:
 * Set IEEE 802.15.4 MAC extended address.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
void SOC_ZPS_vSetMacAddrLocation(void* pu64MacAddress)
{
    macToPlmeMessage_t plmeMsg;

    plmeMsg.msgType = gPlmeSetReq_c;
    plmeMsg.macInstance = 0;
    plmeMsg.msgData.setReq.PibAttribute = gPhyPibLongAddress_c;
    FLib_MemCpy(&plmeMsg.msgData.setReq.PibAttributeValue,
                pu64MacAddress,
                sizeof(uint64_t));
    MAC_PLME_SapHandler (&plmeMsg, 0);
}