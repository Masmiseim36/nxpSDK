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

#include "app_main.h"
#include "app_buttons.h"
#include "ZTimer.h"

#include "fsl_cop.h"
#include "fsl_rcm.h"
#include "board.h"
#include "PWR_Interface.h"
#include "NVM_Interface.h"
#include "TimersManager.h"
#include "Keyboard.h"
#include "LED.h"
#include "MemManager.h"
#include "SerialManager.h"
#include "FunctionLib.h"
/* 802.15.4 */
#include "PhyInterface.h"
#include "MacInterface.h"
#include "RNG_Interface.h"

#include "EH_IEEE_Commands.h"
#include "pwrm.h"

#include <dbg.h>
#include "app_cfg.h"
#include "Panic.h"
#include "EH_IEEE_802154_Switch.h"
#include "EH_Button.h"
#include "app_events.h"
#include "EH_IEEE_Features.h"
#include "fsl_os_abstraction.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef DEBUG_APP
#define TRACE_APP   FALSE
#else
#define TRACE_APP   TRUE
#endif

#define mPhyStackSize_c 1024
#define mPhyTaskPrio_c  4

#ifndef APP_MIN_WAKE_TIME
#define APP_MIN_WAKE_TIME            3000
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PUBLIC void APP_cbTimerReceive(void *pvParam);
PUBLIC void APP_cbTimerShortPress(void *pvParam);
PRIVATE void KBD_Callback( uint8_t events);
resultType_t PD2MAC_Handler(pdDataToMacMessage_t * pMsg, instanceId_t instanceId);
resultType_t PLME2MAC_Handler(plmeToMacMessage_t * pMsg, instanceId_t instanceId);
void PhyThread(osaTaskParam_t argument);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/* dummy variable for compilation */
uint16 s_u16ActivityCounter;
PUBLIC uint8 u8TimerMinWake;
PUBLIC uint8 u8TimerReceive;
PUBLIC uint8 u8TimerTick;
PUBLIC uint8 u8TimerChangeMode;
PUBLIC uint8 u8TimerShortPress;

extern volatile bool_t bTxComplete;
extern volatile bool_t bRxComplete;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE ZTIMER_tsTimer  asTimers[3];

OSA_TASK_DEFINE( PhyThread, mPhyTaskPrio_c, 1, mPhyStackSize_c, FALSE );

osaEventId_t            mPhyEvent;
osaTaskId_t             mButtonTaskHandler;
osaTaskId_t             mPhyTaskHandler;
te_TransitionCode       mButton;

static anchor_t         mPdInputQueue;
static anchor_t         mPlmeInputQueue;

static osaMsgQId_t      mButtonInputQueue;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
extern void vTimerServerInit(void);
void App_SedWakeUpFromKeyBoard(void);

/****************************************************************************
 *
 * NAME: APP_ZLO_Switch_Task
 *
 * DESCRIPTION:
 * Task that handles the application related functionality
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_vInitResources
 *
 * DESCRIPTION:
 * Initialise resources (timers, queue's etc)
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitResources(void)
{

    /* Initialise the Z timer module */
    ZTIMER_eInit(asTimers, (sizeof(asTimers) / sizeof(ZTIMER_tsTimer)));

    ZTIMER_eOpen(&u8TimerMinWake,       NULL,                   NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerReceive,       APP_cbTimerReceive,     NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    //ZTIMER_eOpen(&u8TimerShortPress,    APP_cbTimerShortPress,  NULL, ZTIMER_FLAG_PREVENT_SLEEP);

}
PUBLIC void APP_cbTimerShortPress(void *pvParam)
{
    bEnableSleep = TRUE;
}

PUBLIC void APP_cbTimerReceive(void *pvParam)
{
//	bRxTimerExpired = TRUE;
}

PUBLIC void PWRM_vWakeInterruptCallback(void)
{

}

void main_task(uint32_t param)
{
    static uint8_t initialized = FALSE;
    te_TransitionCode button;
    
    if(!initialized)
    {
        vTimerServerInit();
        {
            cop_config_t configCop;
            COP_GetDefaultConfig(&configCop);
            /* timeout cycle - kCOP_2Power5CyclesOr2Power13Cycles gives approx 8 sec timeout and
             * kCOP_2Power8CyclesOr2Power16Cycles gives approx 64 sec timeout */
            /* Set watch dog feed time constant to approximately 8s */
            configCop.timeoutCycles = kCOP_2Power5CyclesOr2Power13Cycles;
            COP_Init(SIM, &configCop);
        }

        /*myEventId = OSA_EventCreate(TRUE);*/
        initialized = TRUE;
        MEM_Init();
        SecLib_Init();
        TMR_Init();
        LED_Init();
        Phy_Init();
        RNG_Init();
        KBD_Init(KBD_Callback);
        Phy_RegisterSapHandlers((PD_MAC_SapHandler_t)PD2MAC_Handler, (PLME_MAC_SapHandler_t)PLME2MAC_Handler, 0);
#if defined(DBG_ENABLE)
        SerialManager_Init();
        Serial_InitInterface(&APP_SerialId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
        Serial_SetBaudRate  (APP_SerialId,  APP_SERIAL_INTERFACE_SPEED);
#endif

#if cPWR_UsePowerDownMode
        /* Initialise Power Manager even on non-sleeping nodes as it allows the
        * device to doze when in the idle task */
        PWRM_vInit(4);
#endif
        
        APP_vInitResources();
        APP_vSetMacAddr();

        /* Create button event queue */
        mButtonInputQueue = OSA_MsgQCreate(1);

        /* Create PHY task */
        mPhyEvent = OSA_EventCreate(TRUE);
        MSG_InitQueue(&mPdInputQueue);
        MSG_InitQueue(&mPlmeInputQueue);
        mPhyTaskHandler = OSA_TaskCreate(OSA_TASK(PhyThread), NULL);
        if( NULL == mPhyTaskHandler )
        {
            panic(0,0,0,0);
            return;
        }
    }

    while(1)
    {
        if (OSA_MsgQGet(mButtonInputQueue, &button, 0) == osaStatus_Success)
        {
            vHandleButtonPress(button);
        }
        
        ZTIMER_vTask();
        
#ifdef CPU_MKW41Z512VHT4
        COP_Refresh(SIM);
#endif

#if cPWR_UsePowerDownMode
        PWRM_vManagePower();
#endif

        if(!gUseRtos_c)
        {
            break;
        }
    }
}

void PhyThread(osaTaskParam_t argument)
{
    int i = 0;
    osaEventFlags_t ev;
    void *pMsgIn = NULL;

    while(1)
    {
        OSA_EventWait(mPhyEvent, osaEventFlagsAll_c, FALSE, osaWaitForever_c, &ev);

        if(!gUseRtos_c && !ev)
        {
            break;
        }

        pMsgIn = NULL;

        if(ev & gPhyEvtMessageFromPD_c)
        {
            /* Get message from the PD queue */
            pMsgIn = MSG_DeQueue(&mPdInputQueue);

            DBG_vPrintf(TRUE, "\n Get PD message type: %d \n", ((pdDataToMacMessage_t *)pMsgIn)->msgType);
            
            /* Check the message */
            if(pMsgIn)
            {
                if(((pdDataToMacMessage_t *)pMsgIn)->msgType == gPdDataCnf_c)
                {
                    bTxComplete = TRUE;
                }
                
                if(((pdDataToMacMessage_t *)pMsgIn)->msgType == gPdDataInd_c)
                {
                    bRxComplete = TRUE;

                    pdDataInd_t *pData = &((pdDataToMacMessage_t *)pMsgIn)->msgData.dataInd;
                    uint8_t *p = pData->pPsdu;
                    uint8_t dstAddrMode, srcAddrMode, panIdCompression;

                    //FCF
                    FLib_MemCpy(&sGPD_MACReceivedFrame.u16FCF, p, sizeof(uint16_t));
                    p += sizeof(uint16_t);
                    //Num
                    sGPD_MACReceivedFrame.u8SequenceNum = *p++;
                    //Dst
                    dstAddrMode = (sGPD_MACReceivedFrame.u16FCF & 0x0C00) >> 10;
                    switch (dstAddrMode)
                    {
                    case 2:
                        FLib_MemCpy(&sGPD_MACReceivedFrame.u16DestPAN, p, sizeof(uint16_t));
                        p += sizeof(uint16_t);
                        FLib_MemCpy(&sGPD_MACReceivedFrame.uDestAddr.u16Short, p, sizeof(uint16_t));
                        p += sizeof(uint16_t);
                        break;
                    case 3:
                        FLib_MemCpy(&sGPD_MACReceivedFrame.u16DestPAN, p, sizeof(uint16_t));
                        p += sizeof(uint16_t);
                        FLib_MemCpy(&sGPD_MACReceivedFrame.uDestAddr.sExt.u32L, p, sizeof(uint32_t));
                        p += sizeof(uint32_t);
                        FLib_MemCpy(&sGPD_MACReceivedFrame.uDestAddr.sExt.u32H, p, sizeof(uint32_t));
                        p += sizeof(uint32_t);
                        break;
                    }
                    //Src
                    srcAddrMode = (sGPD_MACReceivedFrame.u16FCF & 0xC000) >> 14;
                    panIdCompression = sGPD_MACReceivedFrame.u16FCF & (1<<6);
                    switch (srcAddrMode)
                    {
                    case 2:
                        if (!panIdCompression)
                        {
                        FLib_MemCpy(&sGPD_MACReceivedFrame.u16SrcPAN, p, sizeof(uint16_t));
                        p += sizeof(uint16_t);
                        }
                        FLib_MemCpy(&sGPD_MACReceivedFrame.uSrcAddr.u16Short, p, sizeof(uint16_t));
                        p += sizeof(uint16_t);
                        break;
                    case 3:
                        if (!panIdCompression)
                        {
                            FLib_MemCpy(&sGPD_MACReceivedFrame.u16SrcPAN, p, sizeof(uint16_t));
                            p += sizeof(uint16_t);
                        }
                        FLib_MemCpy(&sGPD_MACReceivedFrame.uSrcAddr.sExt.u32L, p, sizeof(uint32_t));
                        p += sizeof(uint32_t);
                        FLib_MemCpy(&sGPD_MACReceivedFrame.uSrcAddr.sExt.u32H, p, sizeof(uint32_t));
                        p += sizeof(uint32_t);
                        break;
                    }
                    //Payload
                    sGPD_MACReceivedFrame.u8PayloadLength = pData->psduLength - (p - pData->pPsdu);
                    FLib_MemCpy(sGPD_MACReceivedFrame.uPayload.au8Byte, p, sGPD_MACReceivedFrame.u8PayloadLength);
                    
                    //autumn show all data ind
                    for (i = 0; i < pData->psduLength; i++)
                    {
                        DBG_vPrintf(TRUE, "0x%02x ", pData->pPsdu[i]);
                    }
                    DBG_vPrintf(TRUE, "\n\n");
                }
                /* Free the message */
                MSG_Free(pMsgIn);
                pMsgIn = NULL;
            }
        }

        if(ev & gPhyEvtMessageFromPLME_c)
        {
            /* Get message from the PLME queue */
            pMsgIn = MSG_DeQueue(&mPlmeInputQueue);
            
            DBG_vPrintf(TRUE, "\n Get PLME message type: %d \n", ((plmeToMacMessage_t *)pMsgIn)->msgType);
            
            /* Check the message */
            if(pMsgIn)
            {
                /* Free the message */
                MSG_Free(pMsgIn);
                pMsgIn = NULL;
            }
        }
        /* Check for pending messages in the Queue */ 
        if( MSG_Pending(&mPdInputQueue) )
            OSA_EventSet(mPhyEvent, gPhyEvtMessageFromPD_c);
        if( MSG_Pending(&mPlmeInputQueue) )
            OSA_EventSet(mPhyEvent, gPhyEvtMessageFromPLME_c);
        
        if( !gUseRtos_c )
        {
            break;
        } 
    }
}

/****************************************************************************
 *
 * NAME: KBD_Callback
 *
 * DESCRIPTION:
 * Entry point into the application.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void KBD_Callback( uint8_t events)
{
    static uint8_t mark_up_status = 0;
    static uint8_t mark_down_status = 0;
    switch(events)
    {
        case gKBD_EventPB1_c:
            mButton = ON_PRESSED;
            break;
        case gKBD_EventPB2_c:
            mButton = OFF_PRESSED;
            break;
        case gKBD_EventPB3_c:
            mButton = (mark_up_status == 0) ? UP_PRESSED : UP_RELEASED;
            mark_up_status ^= 1;
            break;
        case gKBD_EventPB4_c:
            mButton = (mark_down_status == 0) ? DOWN_PRESSED : DOWN_RELEASED;
            mark_down_status ^= 1;
            break;
        case gKBD_EventLongPB1_c:
        case gKBD_EventLongPB2_c:
        case gKBD_EventLongPB3_c:
        case gKBD_EventLongPB4_c:
        	return;
        case gKBD_EventVeryLongPB1_c:
        case gKBD_EventVeryLongPB2_c:
        case gKBD_EventVeryLongPB3_c:
        case gKBD_EventVeryLongPB4_c:
            mButton = COMM_BUTTON_PRESSED;
        	break;
        default:
            return;
    }

    DBG_vPrintf(TRUE, "\n ==================== KBD_Callback \n");
    
    /* Send the button event to the queue */
    OSA_MsgQPut(mButtonInputQueue, &mButton);
}

void App_SedWakeUpFromKeyBoard(void)
{
    ZTIMER_eStart(u8TimerMinWake, APP_MIN_WAKE_TIME);    
}

/******************************************************************************
* The following functions are called by the PHY to put messages into the
* Application's queue. They need to be defined even if they are not used
* in order to avoid linker errors.
******************************************************************************/
resultType_t PD2MAC_Handler(pdDataToMacMessage_t * pMsg, instanceId_t instanceId)
{
    /* Put the incoming PD message in the application input queue */
    MSG_Queue(&mPdInputQueue, pMsg);
    OSA_EventSet(mPhyEvent, gPhyEvtMessageFromPD_c);
    return gSuccess_c;
}

resultType_t PLME2MAC_Handler(plmeToMacMessage_t * pMsg,     instanceId_t instanceId)
{
    /* Put the incoming PLME message in the application input queue */
    MSG_Queue(&mPlmeInputQueue, pMsg);
    OSA_EventSet(mPhyEvent, gPhyEvtMessageFromPLME_c);
    return gSuccess_c;
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
