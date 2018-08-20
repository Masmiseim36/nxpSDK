/*!
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

#include "genfsk_interface.h"
#include "genfsk_ll.h"
#include "genfsk_utils.h"

#include "fsl_os_abstraction.h"
#include "fsl_xcvr.h"

#include "MemManager.h"
#include "FunctionLib.h"
#include "Flash_Adapter.h"

#if gMWS_Enabled_d
#include "MWS.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief GENFSK Maximum packet length. */
#define gGENFSK_MaxPacketLength_c     (2047)

/*! @brief GENFSK Maximum RAW packet length. */
#define gGENFSK_MaxRawPacketLength_c     (35)

/*! @brief GENFSK Maximum sync address size. */
#define gGENFSK_MaxSyncAddressSize_c     (4)

/*! @brief GENFSK PB_PARTITION settings for maximum TX packet length. */
#define gGENFSK_PbPartitionMaxTx_c    (1088)

/*! @brief GENFSK PB_PARTITION settings for maximum RX packet length. */
#define gGENFSK_PbPartitionMaxRx_c    (0)

/*! @brief GENFSK default enabled events. */
#define mDefaultEnabledEvents (gGenfskTxEvent | gGenfskRxEvent | gGenfskWakeEvent)

/*! @brief GENFSK Protocol Engine commands. */
typedef enum _GENFSK_sequence_commands
{
    NO_ACTION = 0U,  /*!< No action. */
    TX_START_NOW = 1U,  /*!< TX start now. */
    TX_START_T1 = 2U, /*!< TX start @T1 Timer Compare Match (EVENT_TMR = T1_CMP). */
    TX_START_T2 = 3U,  /*!< TX start @T2 Timer Compare Match (EVENT_TMR = T2_CMP). */
    TX_CANCEL = 4U,  /*!< TX cancel -- Cancels pending TX events but do not abort a TX-in-progress. */
    RX_START_NOW = 5U,  /*!< RX start now. */
    RX_START_T1 = 6U,  /*!< RX start @T1 Timer Compare Match (EVENT_TMR = T1_CMP). */
    RX_START_T2 = 7U,  /*!< RX start @T2 Timer Compare Match (EVENT_TMR = T2_CMP). */
    RX_STOP_T1 = 8U,  /*!< RX stop @T1 Timer Compare Match (EVENT_TMR = T1_CMP). */
    RX_STOP_T2 = 9U,  /*!< RX stop @T2 Timer Compare Match (EVENT_TMR = T2_CMP). */
    RX_CANCEL = 10U,  /*!< RX cancel -- Cancels pending RX events but do not abort a RX-in-progress. */
    ABORT_ALL = 11U,  /*!< Abort all -- Cancels all pending events and abort any sequence-in-progress. */
} GENFSK_sequence_commands;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void GENFSK_InstallInterrupt(void);
extern void GENFSK_EnableInterrupts(uint32_t mask);
extern void GENFSK_DisableInterrupts(uint32_t mask);
extern void GENFSK_ProtectFromXcvrInterrupt(void);
extern void GENFSK_UnprotectFromXcvrInterrupt(void);

extern void GENFSK_TimeInit(void);
extern GENFSK_timestamp_t GENFSK_TimeGetTimestamp(void);
extern genfskTimerId_t GENFSK_TimeScheduleEvent(GENFSK_TimeEvent_t *pEvent);
extern void GENFSK_TimeCancelEvent(genfskTimerId_t timerId);

extern void GENFSK_SwitchToInstance(uint8_t instanceId);
extern void GENFSK_MskPreProcessing(uint8_t * pByteIn, uint8_t * pByteOut, uint8_t length, uint8_t initBit);
extern void GENFSK_MskPostProcessing(uint8_t * pByteIn, uint8_t * pByteOut, uint8_t length, uint8_t initBit, uint8_t lsbToMsb);

/*! @brief Send commands to GENFSK LL. */
static inline void GENFSK_Command(GENFSK_sequence_commands command);

/*! @brief GENFSK RX timeout callback function. */
static void GENFSK_RxTimeoutCallback(void);

/*! @brief GENFSK LL Task. */
static void GENFSK_Task(osaTaskParam_t argument);

#if gMWS_Enabled_d
static uint32_t MWS_GENFSK_Callback(mwsEvents_t event);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

OSA_TASK_DEFINE(GENFSK_Task, gGENFSK_TaskPriority_c, 1, gGENFSK_TaskStackSize_c, FALSE);

static uint8_t genfskAlreadyInit = FALSE; 

/*! @brief GENFSK default radio configuration. */
static const GENFSK_radio_config_t mDefaultRadioConfig = 
{
    .radioMode = gGenfskGfskBt0p5h0p5,
    .dataRate = gGenfskDR1Mbps
};

/*! @brief GENFSK default packet configuration. */
static const GENFSK_packet_config_t mDefaultPacketConfig = 
{
    .preambleSizeBytes = 0,
    .packetType = gGenfskFormattedPacket,
    .lengthSizeBits = 0,
    .lengthBitOrder = gGenfskLengthBitLsbFirst,
    .syncAddrSizeBytes = 1,
    .lengthAdjBytes = 0,
    .h0SizeBits = 0,
    .h1SizeBits = 0,
    .h0Match = 0,
    .h0Mask = 0,
    .h1Match = 0,
    .h1Mask = 0
};

/*! @brief GENFSK default CRC configuration. */
static const GENFSK_crc_config_t mDefaultCrcConfig =
{
    .crcEnable = gGenfskCrcEnable,
    .crcRecvInvalid = gGenfskCrcRecvInvalid,
    .crcSize = 2,
    .crcStartByte = 0,
    .crcRefIn = gGenfskCrcInputNoRef,
    .crcRefOut = gGenfskCrcOutputNoRef,
    .crcByteOrder = gGenfskCrcLSByteFirst,
    .crcSeed = 0x0000,
    .crcPoly = 0x1021,
    .crcXorOut = 0
};

/*! @brief GENFSK default Whitener configuration. */
static const GENFSK_whitener_config_t mDefaultWhitenerConfig = 
{
    .whitenEnable = gGenfskWhitenEnable,
    .whitenStart = gWhitenStartNoWhitening,
    .whitenEnd = gWhitenEndAtEndOfPayload,
    .whitenB4Crc = gWhitenB4Crc,
    .whitenPolyType = gFibonnaciPolyType,
    .whitenRefIn = gGenfskWhitenInputNoRef,
    .whitenPayloadReinit = gGenfskWhitenNoPayloadReinit,
    .whitenSize = 9,
    .whitenInit = 0x1FF,
    .whitenPoly = 0x21,
    .whitenSizeThr = 0x800,
    .manchesterEn = gGenfskManchesterDisable,
    .manchesterStart = gGenfskManchesterStartAtPayload,
    .manchesterInv = gGenfskManchesterNoInv,
};

/*! @brief GENFSK default network address configuration. */
static const GENFSK_nwk_addr_match_t mDefaultNwkAddrConfig = 
{
    .nwkAddrSizeBytes = 1,
    .nwkAddrThrBits = 0,
    .nwkAddr = 0x55555555U,
};    

/*! @brief GENFSK local structure initial configuration. */
GENFSK_LocalStruct_t genfskLocal[gGENFSK_InstancesCnt_c];
/*! @brief GENFSK active instance. */
uint8_t mGenfskActiveInstance = gGENFSK_InvalidIdx_c;
/*! @breif GENFSK total number of allocated instances. */
uint8_t mNumberOfAllocatedInstances = 0;
/*! @brief GENFSK RX timeout timer ID. */
genfskTimerId_t rxTimeoutTimer = gGENFSK_InvalidTimerId_c;

osaTaskId_t gGenfskTaskId = 0;
osaEventId_t mGenfskTaskEvent;

extern const uint8_t gUseRtos_c;
extern volatile uint64_t gGenfskTimerOverflow;

/*******************************************************************************
 * Code
 ******************************************************************************/

static inline void GENFSK_Command(GENFSK_sequence_commands command)
{
    GENFSK->XCVR_CTRL = command; 
}

static void GENFSK_RxTimeoutCallback(void)
{
    GENFSK_AbortAll();
    
    if ((genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL) && (genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskRxEvent))
    {
        genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskRxEvent, gGenfskTimeout);
    }
}

genfskStatus_t GENFSK_Init(void)
{     
    genfskStatus_t status = gGenfskSuccess_c;    
    
    if (TRUE == genfskAlreadyInit)
    {
        status = gGenfskAlreadyInit_c;
    }
    else
    {
        FLib_MemSet(genfskLocal, 0, sizeof(genfskLocal));
        
        mNumberOfAllocatedInstances = 0;
        rxTimeoutTimer = gGENFSK_InvalidTimerId_c;
        
        for (uint8_t i = 0; i < gGENFSK_InstancesCnt_c; i++)
        {
            genfskLocal[i].enabledEvents = mDefaultEnabledEvents;
        }
        
        mGenfskTaskEvent = OSA_EventCreate(TRUE);
        
        if (NULL == mGenfskTaskEvent)
        {
            status = gGenfskFail_c;
        }
        else
        {
            gGenfskTaskId = OSA_TaskCreate(OSA_TASK(GENFSK_Task), NULL);
            
            if (NULL == gGenfskTaskId)
            {
                status = gGenfskFail_c;
            }
            else
            {               
                genfskAlreadyInit = TRUE;
            }            
        }
    }
    
    return status;
}

genfskStatus_t GENFSK_AllocInstance(uint8_t *pInstanceId, GENFSK_radio_config_t *radioConfig, GENFSK_packet_config_t *packetConfig, GENFSK_bitproc_t *bitProcConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;
    uint8_t instanceId = gGENFSK_InvalidIdx_c;
     
    if (pInstanceId == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        for (uint8_t i = 0; i < gGENFSK_InstancesCnt_c; i++)
        {
            if (genfskLocal[i].genfskState == gGENFSK_LL_NoInit)
            {
                genfskLocal[i].genfskState = gGENFSK_LL_Idle;
                instanceId = i;
                break;
            }
        }
        
        if (instanceId != gGENFSK_InvalidIdx_c)
        {
            /* Enter critical section. */
            OSA_InterruptDisable();
            
            /* Set current istance as active if it is the first instance initiated. */
            if (mGenfskActiveInstance == gGENFSK_InvalidIdx_c)
            {
                mGenfskActiveInstance = instanceId;
            }
            
            if (radioConfig == NULL)
            {
                GENFSK_RadioConfig(instanceId, (GENFSK_radio_config_t *)&mDefaultRadioConfig);
            }
            else
            {
                status = GENFSK_RadioConfig(instanceId, radioConfig);
            }
            
            if (status == gGenfskSuccess_c)
            {                
                if (packetConfig == NULL)
                {
                    GENFSK_SetPacketConfig(instanceId, (GENFSK_packet_config_t *)&mDefaultPacketConfig);
                }
                else
                {
                    status = GENFSK_SetPacketConfig(instanceId, packetConfig);
                }
                
                if (status == gGenfskSuccess_c)
                {
                    if (bitProcConfig == NULL)
                    {
                        GENFSK_SetCrcConfig(instanceId, (GENFSK_crc_config_t *)&mDefaultCrcConfig);
                        GENFSK_SetWhitenerConfig(instanceId, (GENFSK_whitener_config_t *)&mDefaultWhitenerConfig);
                    }
                    else
                    {
                        if (bitProcConfig->crcConfig == NULL)
                        {
                            GENFSK_SetCrcConfig(instanceId, (GENFSK_crc_config_t *)&mDefaultCrcConfig);                
                        }
                        else
                        {
                            status = GENFSK_SetCrcConfig(instanceId, bitProcConfig->crcConfig);
                        }
                        
                        if (status == gGenfskSuccess_c)
                        {
                            if (bitProcConfig->whitenerConfig == NULL)
                            {
                                GENFSK_SetWhitenerConfig(instanceId, (GENFSK_whitener_config_t *)&mDefaultWhitenerConfig);
                            }
                            else
                            {
                                status = GENFSK_SetWhitenerConfig(instanceId, bitProcConfig->whitenerConfig);
                            }
                        }
                    }
                }
            }
            
            /* GENFSK instance initialization failed. */
            if (status != gGenfskSuccess_c)
            {
                if (mGenfskActiveInstance == instanceId)
                {
                    mGenfskActiveInstance = gGENFSK_InvalidIdx_c;
                }
                
                genfskLocal[instanceId].genfskState = gGENFSK_LL_NoInit;
                instanceId = gGENFSK_InvalidIdx_c;
            }
            else
            {                
                mNumberOfAllocatedInstances++;
                GENFSK_UnprotectFromXcvrInterrupt();
            }
            
            /* Exit critical section. */
            OSA_InterruptEnable();
        }
        else
        {
            status = gGenfskAllocInstanceFailed;
        }
        
        *pInstanceId = instanceId;        
    }
    
    return status;
}

genfskStatus_t GENFSK_RadioConfig(uint8_t instanceId, GENFSK_radio_config_t *radioConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;
    radio_mode_t radioMode = NUM_RADIO_MODES;
    
    if(radioConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyRx)
    {
        status = gGenfskBusyRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyTx)
    {
        status = gGenfskBusyTx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingRx)
    {
        status = gGenfskBusyPendingRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingTx)
    {
        status = gGenfskBusyPendingTx_c;
    }
    else
    {
        switch (radioConfig->radioMode)
        {
        case gGenfskGfskBt0p5h0p5:
            radioMode = GFSK_BT_0p5_h_0p5;
            break;
        case gGenfskGfskBt0p5h0p32:
            radioMode = GFSK_BT_0p5_h_0p32;
            break;
        case gGenfskGfskBt0p5h0p7:
            radioMode = GFSK_BT_0p5_h_0p7;
            break;
        case gGenfskGfskBt0p5h1p0:
            radioMode = GFSK_BT_0p5_h_1p0;
            break;
        case gGenfskGfskBt0p3h0p5:
            radioMode = GFSK_BT_0p3_h_0p5;
            break;
        case gGenfskGfskBt0p7h0p5:
            radioMode = GFSK_BT_0p7_h_0p5;
            break;
        case gGenfskFsk:
            radioMode = GFSK_BT_0p5_h_0p5;
            break;
        case gGenfskMsk:
            radioMode = GFSK_BT_0p5_h_0p5;
            break;
        default:
            status = gGenfskInvalidParameters_c;
            break;
        }

        if(status == gGenfskSuccess_c)
        {
            FLib_MemCpy(&genfskLocal[instanceId].radioConfig, radioConfig, sizeof(genfskLocal[instanceId].radioConfig));
            
            if (mGenfskActiveInstance == instanceId)
            {
                /* This is the first initialization */
                if (mNumberOfAllocatedInstances == 0)
                {
                    XCVR_Init(radioMode, (data_rate_t)genfskLocal[instanceId].radioConfig.dataRate);
                    XCVR_SetXtalTrim( (uint8_t)gHardwareParameters.xtalTrim );

                    /* Install interrupt. */
                    GENFSK_InstallInterrupt();
                    GENFSK_TimeInit();  
#if gMWS_Enabled_d
                    MWS_Register(gMWS_GENFSK_c, MWS_GENFSK_Callback);
#endif
                    GENFSK->BITRATE = genfskLocal[instanceId].radioConfig.dataRate;
                    genfskLocal[instanceId].genfskRegs.bitRate = genfskLocal[instanceId].radioConfig.dataRate;
                }
                else
                {
                    XCVR_ChangeMode(radioMode, (data_rate_t)genfskLocal[instanceId].radioConfig.dataRate);
                    GENFSK->BITRATE = genfskLocal[instanceId].radioConfig.dataRate;
                    genfskLocal[instanceId].genfskRegs.bitRate = genfskLocal[instanceId].radioConfig.dataRate;
                }
                
                if ((radioConfig->radioMode == gGenfskFsk) || (radioConfig->radioMode == gGenfskMsk))
                {
                    XCVR_TX_DIG->GFSK_COEFF1 = ((0U)<<0 | (511U)<<7 | (0U)<<16 | (511U)<<23);
                    XCVR_TX_DIG->GFSK_COEFF2 = 0x00000000U;
                    XCVR_TX_DIG->GFSK_CTRL |= XCVR_TX_DIG_GFSK_CTRL_GFSK_FLD_MASK;
                }
            }
        }
    }
    
    return status;
}

genfskStatus_t GENFSK_SetPacketConfig(uint8_t instanceId, GENFSK_packet_config_t *packetConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;  
     
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyRx)
    {
        status = gGenfskBusyRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyTx)
    {
        status = gGenfskBusyTx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingRx)
    {
        status = gGenfskBusyPendingRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingTx)
    {
        status = gGenfskBusyPendingTx_c;
    }    
    else if (packetConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (packetConfig->preambleSizeBytes > 7)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((packetConfig->lengthSizeBits > 16) || (packetConfig->h0SizeBits > 16) || (packetConfig->h1SizeBits > 16))
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((packetConfig->lengthSizeBits + packetConfig->h0SizeBits + packetConfig->h1SizeBits) & 0x07)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((packetConfig->lengthAdjBytes < -31) || (packetConfig->lengthAdjBytes > 31))
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (packetConfig->syncAddrSizeBytes > 3)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {      
        genfskLocal[instanceId].packetType = packetConfig->packetType;                
        genfskLocal[instanceId].syncAddrSizeBytes = packetConfig->syncAddrSizeBytes + 1;  /* Save the sync address bytes = size + 1 */
        
        /* Save the configuration in local structure. */
        genfskLocal[instanceId].genfskRegs.xcvrCfg &= ~(GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK);                        
        genfskLocal[instanceId].genfskRegs.xcvrCfg |= GENFSK_XCVR_CFG_PREAMBLE_SZ(packetConfig->preambleSizeBytes);
            
        genfskLocal[instanceId].genfskRegs.packetCfg &= (uint32_t)~(uint32_t)(GENFSK_PACKET_CFG_LENGTH_SZ_MASK |
                                                                                  GENFSK_PACKET_CFG_LENGTH_BIT_ORD_MASK |
                                                                                      GENFSK_PACKET_CFG_SYNC_ADDR_SZ_MASK |
                                                                                          GENFSK_PACKET_CFG_LENGTH_ADJ_MASK |
                                                                                              GENFSK_PACKET_CFG_H0_SZ_MASK |
                                                                                                  GENFSK_PACKET_CFG_H1_SZ_MASK);
        
                    
        genfskLocal[instanceId].genfskRegs.h0Cfg &= (uint32_t)~(uint32_t)(GENFSK_H0_CFG_H0_MATCH_MASK |
                                                                              GENFSK_H0_CFG_H0_MASK_MASK);
        
        genfskLocal[instanceId].genfskRegs.h1Cfg &= (uint32_t)~(uint32_t)(GENFSK_H1_CFG_H1_MATCH_MASK |
                                                                             GENFSK_H1_CFG_H1_MASK_MASK);
            
        if ((genfskLocal[instanceId].packetType == gGenfskFormattedPacket) && (genfskLocal[instanceId].radioConfig.radioMode != gGenfskMsk))
        {
            genfskLocal[instanceId].genfskRegs.packetCfg |= (uint32_t)(GENFSK_PACKET_CFG_LENGTH_SZ(packetConfig->lengthSizeBits) |
                                                                           GENFSK_PACKET_CFG_LENGTH_BIT_ORD(packetConfig->lengthBitOrder) |
                                                                               GENFSK_PACKET_CFG_SYNC_ADDR_SZ(packetConfig->syncAddrSizeBytes) |
                                                                                   GENFSK_PACKET_CFG_LENGTH_ADJ(packetConfig->lengthAdjBytes) |
                                                                                       GENFSK_PACKET_CFG_H0_SZ(packetConfig->h0SizeBits) | 
                                                                                           GENFSK_PACKET_CFG_H1_SZ(packetConfig->h1SizeBits));
            
            
            genfskLocal[instanceId].genfskRegs.h0Cfg |= (uint32_t)(GENFSK_H0_CFG_H0_MATCH(packetConfig->h0Match) |
                                                                       GENFSK_H0_CFG_H0_MASK(packetConfig->h0Mask));
            
            
            
            genfskLocal[instanceId].genfskRegs.h1Cfg |= (uint32_t)(GENFSK_H1_CFG_H1_MATCH(packetConfig->h1Match) |
                                                                       GENFSK_H1_CFG_H1_MASK(packetConfig->h1Mask));
        }
        else
        {            
            genfskLocal[instanceId].genfskRegs.packetCfg |= GENFSK_PACKET_CFG_SYNC_ADDR_SZ(packetConfig->syncAddrSizeBytes);
            genfskLocal[instanceId].genfskRegs.xcvrCfg |= (GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK | 
                                                               GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK | 
                                                                   GENFSK_XCVR_CFG_SW_CRC_EN_MASK);
            genfskLocal[instanceId].crcEnable = gGenfskCrcDisable;
            genfskLocal[instanceId].whitenEnable = gGenfskWhitenDisable;
            
        }
        
        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->XCVR_CFG &= ~GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK;
            GENFSK->XCVR_CFG |= (genfskLocal[instanceId].genfskRegs.xcvrCfg & 
                                        (GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK | 
                                             GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK | 
                                                 GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK | 
                                                     GENFSK_XCVR_CFG_SW_CRC_EN_MASK));
            GENFSK->PACKET_CFG = genfskLocal[instanceId].genfskRegs.packetCfg;
            GENFSK->H0_CFG = genfskLocal[instanceId].genfskRegs.h0Cfg;
            GENFSK->H1_CFG = genfskLocal[instanceId].genfskRegs.h1Cfg;
        }
    }
    
    return status;
}

genfskStatus_t GENFSK_GetPacketConfig(uint8_t instanceId, GENFSK_packet_config_t *packetConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;  
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }    
    else if (packetConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        packetConfig->preambleSizeBytes = ((genfskLocal[instanceId].genfskRegs.xcvrCfg & GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK) >> GENFSK_XCVR_CFG_PREAMBLE_SZ_SHIFT);
        packetConfig->lengthSizeBits = ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_LENGTH_SZ_MASK) >> GENFSK_PACKET_CFG_LENGTH_SZ_SHIFT);
        packetConfig->lengthBitOrder = (genfskPacketCfgLengthBitOrd_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_LENGTH_BIT_ORD_MASK) >> GENFSK_PACKET_CFG_LENGTH_BIT_ORD_SHIFT);
        packetConfig->syncAddrSizeBytes = ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_SYNC_ADDR_SZ_MASK) >> GENFSK_PACKET_CFG_SYNC_ADDR_SZ_SHIFT);
        packetConfig->lengthAdjBytes = ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_LENGTH_ADJ_MASK) >> GENFSK_PACKET_CFG_LENGTH_ADJ_SHIFT);
        packetConfig->h0SizeBits = ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H0_SZ_MASK) >> GENFSK_PACKET_CFG_H0_SZ_SHIFT);
        packetConfig->h1SizeBits = ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H1_SZ_MASK) >> GENFSK_PACKET_CFG_H1_SZ_SHIFT);
        packetConfig->h0Match = ((genfskLocal[instanceId].genfskRegs.h0Cfg & GENFSK_H0_CFG_H0_MATCH_MASK) >> GENFSK_H0_CFG_H0_MATCH_SHIFT);
        packetConfig->h0Mask = ((genfskLocal[instanceId].genfskRegs.h0Cfg & GENFSK_H0_CFG_H0_MASK_MASK) >> GENFSK_H0_CFG_H0_MASK_SHIFT);
        packetConfig->h1Match = ((genfskLocal[instanceId].genfskRegs.h1Cfg & GENFSK_H1_CFG_H1_MATCH_MASK) >> GENFSK_H1_CFG_H1_MATCH_SHIFT);
        packetConfig->h1Mask = ((genfskLocal[instanceId].genfskRegs.h1Cfg & GENFSK_H1_CFG_H1_MASK_MASK) >> GENFSK_H1_CFG_H1_MASK_SHIFT);
    }
   
    return status;
}

genfskStatus_t GENFSK_SetCrcConfig(uint8_t instanceId, GENFSK_crc_config_t *crcConfig)
{   
    genfskStatus_t status = gGenfskSuccess_c;  
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyRx)
    {
        status = gGenfskBusyRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyTx)
    {
        status = gGenfskBusyTx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingRx)
    {
        status = gGenfskBusyPendingRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingTx)
    {
        status = gGenfskBusyPendingTx_c;
    }    
    else if (crcConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (crcConfig->crcStartByte > 0xF)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (crcConfig->crcSize > 4)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        genfskLocal[instanceId].crcEnable = crcConfig->crcEnable; 
        genfskLocal[instanceId].crcRecvInvalid = crcConfig->crcRecvInvalid;
        
        genfskLocal[instanceId].genfskRegs.crcCfg &= (uint32_t)~(uint32_t)(GENFSK_CRC_CFG_CRC_SZ_MASK |
                                                                               GENFSK_CRC_CFG_CRC_START_BYTE_MASK |
                                                                                   GENFSK_CRC_CFG_CRC_REF_IN_MASK |
                                                                                       GENFSK_CRC_CFG_CRC_REF_OUT_MASK |
                                                                                           GENFSK_CRC_CFG_CRC_BYTE_ORD_MASK);
        
        genfskLocal[instanceId].genfskRegs.crcCfg |= (uint32_t)(GENFSK_CRC_CFG_CRC_SZ(crcConfig->crcSize) |
                                                                    GENFSK_CRC_CFG_CRC_START_BYTE(crcConfig->crcStartByte) |
                                                                        GENFSK_CRC_CFG_CRC_REF_IN(crcConfig->crcRefIn) |
                                                                            GENFSK_CRC_CFG_CRC_REF_OUT(crcConfig->crcRefOut) |
                                                                                GENFSK_CRC_CFG_CRC_BYTE_ORD(crcConfig->crcByteOrder));
        
        /* Left align CRC seed and Poly. */
        genfskLocal[instanceId].genfskRegs.crcInit = crcConfig->crcSeed << ((4 - crcConfig->crcSize) << 3);
        genfskLocal[instanceId].genfskRegs.crcPoly = crcConfig->crcPoly << ((4 - crcConfig->crcSize) << 3);
        genfskLocal[instanceId].genfskRegs.crcXorOut = crcConfig->crcXorOut  << ((4 - crcConfig->crcSize) << 3);
        
        if ((genfskLocal[instanceId].crcEnable == gGenfskCrcEnable) && (genfskLocal[instanceId].packetType == gGenfskFormattedPacket) &&
            (genfskLocal[instanceId].radioConfig.radioMode != gGenfskMsk))
        {
            genfskLocal[instanceId].genfskRegs.xcvrCfg &= ~GENFSK_XCVR_CFG_SW_CRC_EN_MASK;
        }
        else
        {
            genfskLocal[instanceId].genfskRegs.xcvrCfg |= GENFSK_XCVR_CFG_SW_CRC_EN_MASK;
            genfskLocal[instanceId].crcEnable = gGenfskCrcDisable;
        }
        
        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->CRC_CFG = genfskLocal[instanceId].genfskRegs.crcCfg;
            GENFSK->CRC_INIT = genfskLocal[instanceId].genfskRegs.crcInit;
            GENFSK->CRC_POLY = genfskLocal[instanceId].genfskRegs.crcPoly;
            GENFSK->CRC_XOR_OUT = genfskLocal[instanceId].genfskRegs.crcXorOut;
            GENFSK->XCVR_CFG &= ~GENFSK_XCVR_CFG_SW_CRC_EN_MASK;
            GENFSK->XCVR_CFG |= genfskLocal[instanceId].genfskRegs.xcvrCfg & GENFSK_XCVR_CFG_SW_CRC_EN_MASK;            
        }
    }
    
    return status;
}

genfskStatus_t GENFSK_GetCrcConfig(uint8_t instanceId, GENFSK_crc_config_t *crcConfig)
{   
    genfskStatus_t status = gGenfskSuccess_c;  
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }    
    else if (crcConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        crcConfig->crcEnable = genfskLocal[instanceId].crcEnable;
        crcConfig->crcRecvInvalid = genfskLocal[instanceId].crcRecvInvalid;
        crcConfig->crcSize = ((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_SZ_MASK) >> GENFSK_CRC_CFG_CRC_SZ_SHIFT);
        crcConfig->crcStartByte = ((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_START_BYTE_MASK) >> GENFSK_CRC_CFG_CRC_START_BYTE_SHIFT);
        crcConfig->crcRefIn = (genfskCrcCfgCrcRefIn_t)((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_REF_IN_MASK) >> GENFSK_CRC_CFG_CRC_REF_IN_SHIFT);
        crcConfig->crcRefOut = (genfskCrcCfgCrcRefOut_t)((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_REF_OUT_MASK) >> GENFSK_CRC_CFG_CRC_REF_OUT_SHIFT);
        crcConfig->crcByteOrder = (genfskCrcCfgCrcByteOrd_t)((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_BYTE_ORD_MASK) >> GENFSK_CRC_CFG_CRC_BYTE_ORD_SHIFT);
        crcConfig->crcSeed = genfskLocal[instanceId].genfskRegs.crcInit >> ((4 - crcConfig->crcSize) << 3);
        crcConfig->crcPoly = genfskLocal[instanceId].genfskRegs.crcPoly >> ((4 - crcConfig->crcSize) << 3);
        crcConfig->crcXorOut = genfskLocal[instanceId].genfskRegs.crcXorOut >> ((4 - crcConfig->crcSize) << 3);
    }
    
    return status;
}

genfskStatus_t GENFSK_SetWhitenerConfig(uint8_t instanceId, GENFSK_whitener_config_t *whitenerConfig)
{    
    uint16_t tempValInit;
    uint16_t tempValPoly;
    genfskStatus_t status = gGenfskSuccess_c;
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyRx)
    {
        status = gGenfskBusyRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyTx)
    {
        status = gGenfskBusyTx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingRx)
    {
        status = gGenfskBusyPendingRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingTx)
    {
        status = gGenfskBusyPendingTx_c;
    }    
    else if (whitenerConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (whitenerConfig->whitenSize > 9)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((whitenerConfig->whitenInit > 0x1FF) || (whitenerConfig->whitenPoly > 0x1FF))
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (whitenerConfig->whitenSizeThr > 0xFFF)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((whitenerConfig->whitenEnable > gGenfskWhitenDisable) && (whitenerConfig->manchesterEn == gGenfskManchesterEnable))
    {
        /* Whitening and Manchester are mutual exclusive. */
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        genfskLocal[instanceId].whitenEnable = whitenerConfig->whitenEnable;
        
        /* Reorder the bits in POLY and SEED for whitening. */           
        tempValInit = GENFSK_Reverse9Bit(whitenerConfig->whitenInit & 0x1FF);            
        tempValInit >>= 9 - whitenerConfig->whitenSize;            
        tempValPoly = whitenerConfig->whitenPoly;
        tempValPoly &= ~(0x01);
        tempValPoly = GENFSK_Reverse9Bit(tempValPoly & 0x1FF);
        tempValPoly >>= 9 - whitenerConfig->whitenSize;
               
        genfskLocal[instanceId].genfskRegs.whitenCfg &= ~(GENFSK_WHITEN_CFG_WHITEN_START_MASK |
                                                              GENFSK_WHITEN_CFG_WHITEN_END_MASK |
                                                                  GENFSK_WHITEN_CFG_WHITEN_B4_CRC_MASK |
                                                                      GENFSK_WHITEN_CFG_WHITEN_POLY_TYPE_MASK |
                                                                          GENFSK_WHITEN_CFG_WHITEN_REF_IN_MASK |
                                                                              GENFSK_WHITEN_CFG_WHITEN_PAYLOAD_REINIT_MASK |
                                                                                  GENFSK_WHITEN_CFG_WHITEN_SIZE_MASK |
                                                                                      GENFSK_WHITEN_CFG_WHITEN_INIT_MASK);
        
        genfskLocal[instanceId].genfskRegs.whitenCfg |= (GENFSK_WHITEN_CFG_WHITEN_START(whitenerConfig->whitenStart) |
                                                             GENFSK_WHITEN_CFG_WHITEN_END(whitenerConfig->whitenEnd) |
                                                                 GENFSK_WHITEN_CFG_WHITEN_B4_CRC(whitenerConfig->whitenB4Crc) |
                                                                     GENFSK_WHITEN_CFG_WHITEN_POLY_TYPE(whitenerConfig->whitenPolyType) |
                                                                         GENFSK_WHITEN_CFG_WHITEN_REF_IN(whitenerConfig->whitenRefIn) |
                                                                             GENFSK_WHITEN_CFG_WHITEN_PAYLOAD_REINIT(whitenerConfig->whitenPayloadReinit) |
                                                                                 GENFSK_WHITEN_CFG_WHITEN_SIZE(whitenerConfig->whitenSize) |
                                                                                     GENFSK_WHITEN_CFG_WHITEN_INIT(tempValInit));
        
        genfskLocal[instanceId].genfskRegs.whitenPoly = GENFSK_WHITEN_POLY_WHITEN_POLY(tempValPoly);        
        genfskLocal[instanceId].genfskRegs.whitenSzThr &= ~GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK;
        genfskLocal[instanceId].genfskRegs.whitenSzThr |= GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR(whitenerConfig->whitenSizeThr);
         
        genfskLocal[instanceId].genfskRegs.whitenCfg &= ~(GENFSK_WHITEN_CFG_MANCHESTER_EN_MASK |
                                                              GENFSK_WHITEN_CFG_MANCHESTER_INV_MASK |
                                                                  GENFSK_WHITEN_CFG_MANCHESTER_START_MASK);
                
        if ((genfskLocal[instanceId].whitenEnable == gGenfskWhitenEnable) && (genfskLocal[instanceId].packetType == gGenfskFormattedPacket) &&
            (genfskLocal[instanceId].radioConfig.radioMode != gGenfskMsk))
        {
            genfskLocal[instanceId].genfskRegs.xcvrCfg &= ~(GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK | GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK);
            
            genfskLocal[instanceId].genfskRegs.whitenCfg |= (GENFSK_WHITEN_CFG_MANCHESTER_EN(whitenerConfig->manchesterEn) |
                                                                 GENFSK_WHITEN_CFG_MANCHESTER_INV(whitenerConfig->manchesterInv) |
                                                                     GENFSK_WHITEN_CFG_MANCHESTER_START(whitenerConfig->manchesterStart));
        }
        else
        {
            genfskLocal[instanceId].genfskRegs.xcvrCfg |= GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK | GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK;
            
            genfskLocal[instanceId].genfskRegs.whitenCfg |= (GENFSK_WHITEN_CFG_MANCHESTER_EN(0) |
                                                                 GENFSK_WHITEN_CFG_MANCHESTER_INV(0) |
                                                                     GENFSK_WHITEN_CFG_MANCHESTER_START(0));
            
            genfskLocal[instanceId].whitenEnable = gGenfskWhitenDisable;
        }
        
        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->WHITEN_CFG = genfskLocal[instanceId].genfskRegs.whitenCfg;
            GENFSK->WHITEN_POLY = genfskLocal[instanceId].genfskRegs.whitenPoly;
            GENFSK->WHITEN_SZ_THR &= ~GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK;
            GENFSK->WHITEN_SZ_THR |= genfskLocal[instanceId].genfskRegs.whitenSzThr & GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK;
            GENFSK->XCVR_CFG &= ~(GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK | GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK);
            GENFSK->XCVR_CFG |= genfskLocal[instanceId].genfskRegs.xcvrCfg & (GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK | GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK);
        }
    }
    
    return status;
}

genfskStatus_t GENFSK_GetWhitenerConfig(uint8_t instanceId, GENFSK_whitener_config_t *whitenerConfig)
{      
    genfskStatus_t status = gGenfskSuccess_c;  
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }    
    else if (whitenerConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        whitenerConfig->whitenEnable = genfskLocal[instanceId].whitenEnable;        
        whitenerConfig->whitenStart = (genfskWhitenStart_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_START_MASK) >> GENFSK_WHITEN_CFG_WHITEN_START_SHIFT);
        whitenerConfig->whitenEnd = (genfskWhitenEnd_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_END_MASK) >> GENFSK_WHITEN_CFG_WHITEN_END_SHIFT);
        whitenerConfig->whitenB4Crc = (genfskWhitenB4Crc_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_B4_CRC_MASK) >> GENFSK_WHITEN_CFG_WHITEN_B4_CRC_SHIFT);
        whitenerConfig->whitenPolyType = (genfskWhitenPolyType_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_POLY_TYPE_MASK) >> GENFSK_WHITEN_CFG_WHITEN_POLY_TYPE_SHIFT);
        whitenerConfig->whitenRefIn = (genfskWhitenCfgRefIn_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_REF_IN_MASK) >> GENFSK_WHITEN_CFG_WHITEN_REF_IN_SHIFT);
        whitenerConfig->whitenPayloadReinit = (genfskWhitenCfgPayloadReinit_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_PAYLOAD_REINIT_MASK) >> GENFSK_WHITEN_CFG_WHITEN_PAYLOAD_REINIT_SHIFT);
        whitenerConfig->whitenSize = ((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_SIZE_MASK) >> GENFSK_WHITEN_CFG_WHITEN_SIZE_SHIFT);
        whitenerConfig->whitenInit = ((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_INIT_MASK) >> GENFSK_WHITEN_CFG_WHITEN_INIT_SHIFT);
        whitenerConfig->manchesterEn = (genfskManchesterEn_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_MANCHESTER_EN_MASK) >> GENFSK_WHITEN_CFG_MANCHESTER_EN_SHIFT);
        whitenerConfig->manchesterInv = (genfskManchesterInv_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_MANCHESTER_INV_MASK) >> GENFSK_WHITEN_CFG_MANCHESTER_INV_SHIFT);
        whitenerConfig->manchesterStart = (genfskManchesterStart_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_MANCHESTER_START_MASK) >> GENFSK_WHITEN_CFG_MANCHESTER_START_SHIFT);        
        whitenerConfig->whitenPoly = ((genfskLocal[instanceId].genfskRegs.whitenPoly & GENFSK_WHITEN_POLY_WHITEN_POLY_MASK) >> GENFSK_WHITEN_POLY_WHITEN_POLY_SHIFT);
        whitenerConfig->whitenSizeThr = ((genfskLocal[instanceId].genfskRegs.whitenSzThr & GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK) >> GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR_SHIFT);
        
        /* Reorder the bits in POLY and SEED for whitening. */            
        whitenerConfig->whitenInit = GENFSK_Reverse9Bit(whitenerConfig->whitenInit & 0x1FF);
        whitenerConfig->whitenInit >>= 9 - whitenerConfig->whitenSize;
        whitenerConfig->whitenPoly = GENFSK_Reverse9Bit(whitenerConfig->whitenPoly & 0x1FF);
        whitenerConfig->whitenPoly >>= 9 - whitenerConfig->whitenSize;
        whitenerConfig->whitenPoly |= (0x01);
    }
    
    return status;
}

genfskStatus_t GENFSK_FreeInstance(uint8_t instanceId)
{
    genfskStatus_t status = gGenfskSuccess_c;
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else
    {
        /* Enter critical section. */
        OSA_InterruptDisable();
        
        GENFSK_ResetToDefaults(instanceId);            
        genfskLocal[instanceId].genfskState = gGENFSK_LL_NoInit;
        mNumberOfAllocatedInstances--;
        
        if (mNumberOfAllocatedInstances == 0)
        {
            GENFSK_ProtectFromXcvrInterrupt();
        }
        
        /* Exit critical section. */
        OSA_InterruptEnable();
    }
    
    return status;
}

void GENFSK_ResetToDefaults(uint8_t instanceId)
{
    if (instanceId < gGENFSK_InstancesCnt_c)
    {
        if (genfskLocal[instanceId].genfskState != gGENFSK_LL_NoInit)
        {
            /* Enter critical section. */
            OSA_InterruptDisable();
            
            if (mGenfskActiveInstance == instanceId)
            {
                GENFSK_AbortAll();
            }
            
            GENFSK_SetPacketConfig(instanceId, (GENFSK_packet_config_t *)&mDefaultPacketConfig);
            GENFSK_SetCrcConfig(instanceId, (GENFSK_crc_config_t *)&mDefaultCrcConfig);
            GENFSK_SetWhitenerConfig(instanceId, (GENFSK_whitener_config_t *)&mDefaultWhitenerConfig);
            
            for (uint8_t i = 0; i < 4; i++)
            {
                GENFSK_SetNetworkAddress(instanceId, i, (GENFSK_nwk_addr_match_t *)&mDefaultNwkAddrConfig);
                GENFSK_DisableNetworkAddress(instanceId, i);
            }
            
            genfskLocal[instanceId].genfskState = gGENFSK_LL_Idle;
            
            /* Exit critical section. */
            OSA_InterruptEnable();
        }
    }
}

genfskStatus_t GENFSK_SetNetworkAddress(uint8_t instanceId, uint8_t location, GENFSK_nwk_addr_match_t *nwkAddressSettings)
{
    GENFSK_nwk_addr_t tempNwkAddress;
    uint8_t *pTempNwkAddress = NULL;
    genfskStatus_t status = gGenfskSuccess_c;  
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyRx)
    {
        status = gGenfskBusyRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyTx)
    {
        status = gGenfskBusyTx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingRx)
    {
        status = gGenfskBusyPendingRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingTx)
    {
        status = gGenfskBusyPendingTx_c;
    }    
    else if ((location > 3) || (nwkAddressSettings == NULL))
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((nwkAddressSettings->nwkAddrSizeBytes > 3) || (nwkAddressSettings->nwkAddrThrBits > 7 ))
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        tempNwkAddress = nwkAddressSettings->nwkAddr;
        
        if (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)
        {
            /* Twiddle network address bytes for MSK. */            
            pTempNwkAddress = (uint8_t *)&tempNwkAddress;
            
            GENFSK_MskPreProcessing(pTempNwkAddress,
                                    pTempNwkAddress,
                                    nwkAddressSettings->nwkAddrSizeBytes + 1,
                                    ((~pTempNwkAddress[0]) & 0x01));
        }
                
        if (location == 0)
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr0 = tempNwkAddress;
            
            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl &= ~(GENFSK_NTW_ADR_CTRL_NTW_ADR0_SZ_MASK |
                                                                    GENFSK_NTW_ADR_CTRL_NTW_ADR_THR0_MASK);
                                      
            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl |= (GENFSK_NTW_ADR_CTRL_NTW_ADR0_SZ(nwkAddressSettings->nwkAddrSizeBytes) |
                                                                   GENFSK_NTW_ADR_CTRL_NTW_ADR_THR0(nwkAddressSettings->nwkAddrThrBits));
        }
        else if (location == 1)
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr1 = tempNwkAddress;
            
            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl &= ~(GENFSK_NTW_ADR_CTRL_NTW_ADR1_SZ_MASK |
                                                                    GENFSK_NTW_ADR_CTRL_NTW_ADR_THR1_MASK);
            
            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl |= (GENFSK_NTW_ADR_CTRL_NTW_ADR1_SZ(nwkAddressSettings->nwkAddrSizeBytes) |
                                                                   GENFSK_NTW_ADR_CTRL_NTW_ADR_THR1(nwkAddressSettings->nwkAddrThrBits));
        }
        else if (location == 2)
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr2 = tempNwkAddress;
            
            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl &= ~(GENFSK_NTW_ADR_CTRL_NTW_ADR2_SZ_MASK |
                                                                    GENFSK_NTW_ADR_CTRL_NTW_ADR_THR2_MASK);
            
            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl |= (GENFSK_NTW_ADR_CTRL_NTW_ADR2_SZ(nwkAddressSettings->nwkAddrSizeBytes) |
                                                                   GENFSK_NTW_ADR_CTRL_NTW_ADR_THR2(nwkAddressSettings->nwkAddrThrBits));
        }
        else 
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr3 = tempNwkAddress;
            
            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl &= ~(GENFSK_NTW_ADR_CTRL_NTW_ADR3_SZ_MASK |
                                                                    GENFSK_NTW_ADR_CTRL_NTW_ADR_THR3_MASK);
            
            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl |= (GENFSK_NTW_ADR_CTRL_NTW_ADR3_SZ(nwkAddressSettings->nwkAddrSizeBytes) |
                                                                   GENFSK_NTW_ADR_CTRL_NTW_ADR_THR3(nwkAddressSettings->nwkAddrThrBits));
        }
        
        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->NTW_ADR_CTRL = genfskLocal[instanceId].genfskRegs.ntwAdrCtrl;
            GENFSK->NTW_ADR_0 = genfskLocal[instanceId].genfskRegs.ntwAdr0;
            GENFSK->NTW_ADR_1 = genfskLocal[instanceId].genfskRegs.ntwAdr1;
            GENFSK->NTW_ADR_2 = genfskLocal[instanceId].genfskRegs.ntwAdr2;
            GENFSK->NTW_ADR_3 = genfskLocal[instanceId].genfskRegs.ntwAdr3;         
        }
    }
    
    return status;
}

genfskStatus_t GENFSK_GetNetworkAddress(uint8_t instanceId, uint8_t location, GENFSK_nwk_addr_match_t *nwkAddressSettings)
{
    GENFSK_nwk_addr_t tempNwkAddress;
    uint8_t *pTempNwkAddress = NULL;
    genfskStatus_t status = gGenfskSuccess_c;  
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }    
    else if ((location > 3) || (nwkAddressSettings == NULL))
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {        
        if (location == 0)
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr0;
            nwkAddressSettings->nwkAddrSizeBytes = ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR0_SZ_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR0_SZ_SHIFT);
            nwkAddressSettings->nwkAddrThrBits = ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR_THR0_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_THR0_SHIFT);
        }
        else if (location == 1)
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr1;
            nwkAddressSettings->nwkAddrSizeBytes = ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR1_SZ_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR1_SZ_SHIFT);
            nwkAddressSettings->nwkAddrThrBits = ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR_THR1_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_THR1_SHIFT);
        }
        else if (location == 2)
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr2;
            nwkAddressSettings->nwkAddrSizeBytes = ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR2_SZ_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR2_SZ_SHIFT);
            nwkAddressSettings->nwkAddrThrBits = ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR_THR2_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_THR2_SHIFT);
        }
        else 
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr3;
            nwkAddressSettings->nwkAddrSizeBytes = ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR3_SZ_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR3_SZ_SHIFT);
            nwkAddressSettings->nwkAddrThrBits = ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR_THR3_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_THR3_SHIFT);
        }
        
        if (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)
        {
            tempNwkAddress = nwkAddressSettings->nwkAddr;
            
            /* Twiddle network address bytes for MSK. */            
            pTempNwkAddress = (uint8_t *)&tempNwkAddress;
            
            GENFSK_MskPostProcessing(pTempNwkAddress,
                                     pTempNwkAddress,
                                     nwkAddressSettings->nwkAddrSizeBytes + 1,
                                     ((~pTempNwkAddress[0]) & 0x01),
                                     1);
            
            nwkAddressSettings->nwkAddr = tempNwkAddress;
        }
    }
    
    return status;
}
 
genfskStatus_t GENFSK_EnableNetworkAddress(uint8_t instanceId, uint8_t location)
{
    genfskStatus_t status = gGenfskSuccess_c;  
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyRx)
    {
        status = gGenfskBusyRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyTx)
    {
        status = gGenfskBusyTx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingRx)
    {
        status = gGenfskBusyPendingRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingTx)
    {
        status = gGenfskBusyPendingTx_c;
    }   
    else if (location > 3)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        genfskLocal[instanceId].genfskRegs.ntwAdrCtrl |= (uint32_t)((1 << location) << GENFSK_NTW_ADR_CTRL_NTW_ADR_EN_SHIFT);
        
        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->NTW_ADR_CTRL = genfskLocal[instanceId].genfskRegs.ntwAdrCtrl;
        }
    }
    
    return status;
}

genfskStatus_t GENFSK_DisableNetworkAddress(uint8_t instanceId, uint8_t location)
{
    genfskStatus_t status = gGenfskSuccess_c;  
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyRx)
    {
        status = gGenfskBusyRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyTx)
    {
        status = gGenfskBusyTx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingRx)
    {
        status = gGenfskBusyPendingRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingTx)
    {
        status = gGenfskBusyPendingTx_c;
    }    
    else if (location > 3)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {    
        genfskLocal[instanceId].genfskRegs.ntwAdrCtrl &= (uint32_t)~(uint32_t)((1 << location) << GENFSK_NTW_ADR_CTRL_NTW_ADR_EN_SHIFT);
        
        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->NTW_ADR_CTRL = genfskLocal[instanceId].genfskRegs.ntwAdrCtrl;
        }
    }
    
    return status;
}

genfskStatus_t GENFSK_SetChannelNumber(uint8_t instanceId, uint8_t channelNum)
{
    genfskStatus_t status = gGenfskSuccess_c;  
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }    
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyRx)
    {
        status = gGenfskBusyRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyTx)
    {
        status = gGenfskBusyTx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingRx)
    {
        status = gGenfskBusyPendingRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingTx)
    {
        status = gGenfskBusyPendingTx_c;
    }    
    else if (channelNum > 127)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {    
        genfskLocal[instanceId].genfskRegs.channelNum = channelNum;
        
        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->CHANNEL_NUM = genfskLocal[instanceId].genfskRegs.channelNum;
        }
    }
    
    return status;
}

uint8_t GENFSK_GetChannelNumber(uint8_t instanceId)
{       
    uint8_t chanNum = 0xFF;
    
    if (instanceId < gGENFSK_InstancesCnt_c)
    {
        if (genfskLocal[instanceId].genfskState != gGENFSK_LL_NoInit)
        {
            chanNum = (uint8_t)genfskLocal[instanceId].genfskRegs.channelNum;
        }
    }

    return chanNum;
}

genfskStatus_t GENFSK_SetTxPowerLevel(uint8_t instanceId, uint8_t txPowerLevel)
{
    uint8_t tempTxPower = 0;
    genfskStatus_t status = gGenfskSuccess_c;
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }    
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyRx)
    {
        status = gGenfskBusyRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyTx)
    {
        status = gGenfskBusyTx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingRx)
    {
        status = gGenfskBusyPendingRx_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_BusyPendingTx)
    {
        status = gGenfskBusyPendingTx_c;
    }
    else if (txPowerLevel > 32)
    {
        status = gGenfskInvalidParameters_c;
    }    
    else
    {    
        if (txPowerLevel < 2)
        {
            tempTxPower = txPowerLevel;
        }
        else
        {
            /* TX power is converted to match the hardware. 
             * Valid values are 0,1, 2,4,6,8...0x3e (that is, only even values above 1). 
             */
            tempTxPower = ((txPowerLevel << 1) - 2);
        }
        
        genfskLocal[instanceId].genfskRegs.txPower = tempTxPower;
        
        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->TX_POWER = genfskLocal[instanceId].genfskRegs.txPower;
        }
    }
    
    return status;
}

uint8_t GENFSK_GetTxPowerLevel(uint8_t instanceId)
{    
    uint8_t tempTxPower = 0xFF;

    if (instanceId < gGENFSK_InstancesCnt_c)
    {
        if (genfskLocal[instanceId].genfskState != gGENFSK_LL_NoInit)
        {
            tempTxPower = (uint8_t)genfskLocal[instanceId].genfskRegs.txPower; 
            
            if (tempTxPower > 2)
            {
                tempTxPower = ((tempTxPower + 2) >> 1);
            }
        }
    }
    
    return tempTxPower;
}

genfskStatus_t GENFSK_StartTx(uint8_t instanceId, uint8_t *pBuffer, uint16_t bufLengthBytes, GENFSK_timestamp_t txStartTime)
{
    uint8_t codedBytes[gGENFSK_MaxSyncAddressSize_c + gGENFSK_MaxRawPacketLength_c];
    GENFSK_timestamp_t currentTime = 0;
    GENFSK_timestamp_t tempTime = 0;
    
    genfskStatus_t status = gGenfskSuccess_c;
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }    
    else if (pBuffer == NULL) 
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (((genfskLocal[instanceId].packetType == gGenfskRawPacket) || (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)) && 
              (bufLengthBytes > gGENFSK_MaxRawPacketLength_c + genfskLocal[instanceId].syncAddrSizeBytes))
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (bufLengthBytes > gGENFSK_MaxPacketLength_c + genfskLocal[instanceId].syncAddrSizeBytes)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        /* Enter critical section. */
        OSA_InterruptDisable();
        
        /* If GENFSK LL active sequence is Idle start TX. */
        if (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_Idle)
        {
#if gMWS_Enabled_d
            if (gMWS_Success_c == MWS_Acquire(gMWS_GENFSK_c, FALSE))
            {
#endif
                /* Change the active instance */
                if (instanceId != mGenfskActiveInstance)
                {
                    GENFSK_SwitchToInstance(instanceId);
                    mGenfskActiveInstance = instanceId;
                }
                
                if ((genfskLocal[instanceId].packetType == gGenfskRawPacket) ||
                    (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk))
                {
                    GENFSK->PACKET_CFG &= ~(GENFSK_PACKET_CFG_H0_SZ_MASK |
                                            GENFSK_PACKET_CFG_H1_SZ_MASK |
                                            GENFSK_PACKET_CFG_LENGTH_ADJ_MASK);
                    
                    /* Compute the length without sync address. */
                    uint16_t tempLength = bufLengthBytes - genfskLocal[instanceId].syncAddrSizeBytes;
                    
                    /* If the length exceedes the LENGTH_ADJ size, store the remaining 4 bytes in H0 and H1. */
                    if (tempLength  > (gGENFSK_MaxRawPacketLength_c - 4))
                    {
                        GENFSK->PACKET_CFG |= (GENFSK_PACKET_CFG_H0_SZ(16) |
                                               GENFSK_PACKET_CFG_H1_SZ(16) |
                                               GENFSK_PACKET_CFG_LENGTH_ADJ(tempLength - 4));  /* Keep 4 bytes in H0 and H1 */
                    }
                    else
                    {
                        GENFSK->PACKET_CFG |= GENFSK_PACKET_CFG_LENGTH_ADJ(tempLength);
                    }
                }
                
                /* Set the packet buffer partition to maximum TX packet. */
                GENFSK->PB_PARTITION = gGENFSK_PbPartitionMaxTx_c;
                
                if (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)
                {
                    FLib_MemSet(codedBytes, 0, sizeof(codedBytes));
                    FLib_MemCpy(codedBytes, pBuffer, bufLengthBytes);
                    
                    GENFSK_MskPreProcessing(codedBytes,
                                            codedBytes,
                                            sizeof(codedBytes),
                                            ((~codedBytes[0]) & 0x01));
                    
                    /* Write packet to packet buffer. */
                    GENFSK_WritePacketBuffer(0, codedBytes, bufLengthBytes);
                }
                else
                {
                    /* Write packet to packet buffer. */
                    GENFSK_WritePacketBuffer(0, pBuffer, bufLengthBytes);
                }
                
                /* Start TX now. */
                if (!txStartTime)
                {
                    genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyTx;
                    
                    /* Enable TX and PLL unlock interrupts. */
                    GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK);
                    
                    GENFSK_Command(TX_START_NOW);
                }
                else
                {
                    /* Subtract the TX warmup time from the start time. */
                    tempTime = txStartTime - ((GENFSK->XCVR_CFG & GENFSK_XCVR_CFG_TX_WARMUP_MASK) >> GENFSK_XCVR_CFG_TX_WARMUP_SHIFT);
                    
                    currentTime = GENFSK_GetTimestamp();
                    
                    if (tempTime <= currentTime + gGENFSK_MinSetupTime_c)
                    {
                        status = gGenfskInstantPassed_c;
                    }
                    else if ((tempTime - currentTime) >= gGENFSK_OverflowTimerUnit_c)
                    {
                        /* Timer value overrun the 24bit value. */
                        status = gGenfskInvalidParameters_c;
                    }
                    else
                    {
                        genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyPendingTx;
                        
                        GENFSK->T1_CMP = (tempTime & GENFSK_T1_CMP_T1_CMP_MASK) | GENFSK_T1_CMP_T1_CMP_EN_MASK;
                        
                        /* Enable TX and PLL unlock interrupts. */
                        GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK);
                        
                        GENFSK_Command(TX_START_T1);
                    }
                }
#if gMWS_Enabled_d
            }
            else
            {
                status = gGenfskFail_c;
            }
#endif
        }
        else
        {
            switch(genfskLocal[mGenfskActiveInstance].genfskState)
            {
            case gGENFSK_LL_NoInit:
                status = gGenfskFail_c;
                break;
            case gGENFSK_LL_BusyRx:
                status = gGenfskBusyRx_c;
                break;
            case gGENFSK_LL_BusyTx:
                status = gGenfskBusyTx_c;
                break;
            case gGENFSK_LL_BusyPendingRx:
                status = gGenfskBusyPendingRx_c;
                break;
            case gGENFSK_LL_BusyPendingTx:
                status = gGenfskBusyPendingTx_c;
                break;
            default:
                status = gGenfskFail_c;
                break;
            }
        }
        
        /* Exit critical section. */
        OSA_InterruptEnable();
    }
    
    return status;
}

genfskStatus_t GENFSK_CancelPendingTx(void)
{
    genfskStatus_t status = gGenfskSuccess_c;        

    if (mGenfskActiveInstance != gGENFSK_InvalidIdx_c)
    {
        /* Enter critical section. */
        OSA_InterruptDisable();
        
        GENFSK_Command(TX_CANCEL);
        
        /* Wait for XCVR to become idle. */
        while (GENFSK->XCVR_CTRL & GENFSK_XCVR_CTRL_XCVR_BUSY_MASK){};
        
        /* Enable TX and PLL unlock interrupts. */                
        GENFSK_DisableInterrupts(GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK);
        
        genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;    
        
        /* Exit critical section. */
        OSA_InterruptEnable();
    }
        
    return status;
}

genfskStatus_t GENFSK_StartRx(uint8_t instanceId, uint8_t *pBuffer, uint16_t maxBufLengthBytes, GENFSK_timestamp_t rxStartTime, GENFSK_timestamp_t rxDuration)
{
    GENFSK_TimeEvent_t event;
    GENFSK_timestamp_t currentTime = 0;
    GENFSK_timestamp_t tempTime = 0;
    
    genfskStatus_t status = gGenfskSuccess_c;
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }    
    else if (pBuffer == NULL) 
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (((genfskLocal[instanceId].packetType == gGenfskRawPacket) || (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)) && 
              (maxBufLengthBytes > gGENFSK_MaxRawPacketLength_c  + genfskLocal[instanceId].syncAddrSizeBytes))
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (maxBufLengthBytes > gGENFSK_MaxPacketLength_c + genfskLocal[instanceId].syncAddrSizeBytes)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        /* Enter critical section. */
        OSA_InterruptDisable();

        /* If GENFSK LL is idle start RX. */
        if (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_Idle)
        {
#if gMWS_Enabled_d
            if (gMWS_Success_c == MWS_Acquire(gMWS_GENFSK_c, FALSE))
            {
#endif
                /* Change the active instance */
                if (instanceId != mGenfskActiveInstance)
                {
                    GENFSK_SwitchToInstance(instanceId);
                    mGenfskActiveInstance = instanceId;
                }
                
                if ((genfskLocal[instanceId].packetType == gGenfskRawPacket) ||
                    (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk))
                {
                    GENFSK->PACKET_CFG &= ~(GENFSK_PACKET_CFG_H0_SZ_MASK |
                                            GENFSK_PACKET_CFG_H1_SZ_MASK |
                                            GENFSK_PACKET_CFG_LENGTH_ADJ_MASK);
                    
                    /* Compute the length without sync address. */
                    uint16_t tempLength = maxBufLengthBytes - genfskLocal[instanceId].syncAddrSizeBytes;
                    
                    /* If the length exceedes the LENGTH_ADJ size, store the remaining 4 bytes in H0 and H1. */
                    if (tempLength  > (gGENFSK_MaxRawPacketLength_c - 4))
                    {
                        GENFSK->PACKET_CFG |= (GENFSK_PACKET_CFG_H0_SZ(16) |
                                               GENFSK_PACKET_CFG_H1_SZ(16) |
                                               GENFSK_PACKET_CFG_LENGTH_ADJ(tempLength - 4));  /* Keep 4 bytes in H0 and H1 */
                    }
                    else
                    {
                        GENFSK->PACKET_CFG |= GENFSK_PACKET_CFG_LENGTH_ADJ(tempLength);
                    }
                }
                
                /* Set the packet buffer partition to maximum RX packet. */
                GENFSK->PB_PARTITION = gGENFSK_PbPartitionMaxRx_c;
                
                /* Enable RX interrupts. */
                GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_RX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK);
                
                genfskLocal[instanceId].genfskRxLocal.rxPacketBuffer = pBuffer;
                genfskLocal[instanceId].genfskRxLocal.rxMaxPacketLength = maxBufLengthBytes;
                
                /* Start RX now. */
                if (!rxStartTime)
                {
                    genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyRx;
                    
                    if (rxDuration)
                    {
                        event.timestamp = GENFSK_GetTimestamp() + rxDuration;
                        event.callback = GENFSK_RxTimeoutCallback;
                        
                        rxTimeoutTimer = GENFSK_TimeScheduleEvent(&event);
                    }
                    
                    /* Start RX immediately. */
                    GENFSK_Command(RX_START_NOW);
                }
                else
                {
                    /* Subtract the RX warmup time from the start time. */
                    tempTime = rxStartTime - ((GENFSK->XCVR_CFG & GENFSK_XCVR_CFG_RX_WARMUP_MASK) >> GENFSK_XCVR_CFG_RX_WARMUP_SHIFT);
                    
                    currentTime = GENFSK_GetTimestamp();
                    
                    if (tempTime <= currentTime + gGENFSK_MinSetupTime_c)
                    {
                        status = gGenfskInstantPassed_c;
                    }
                    else if ((tempTime - currentTime) >= gGENFSK_OverflowTimerUnit_c)
                    {
                        /* The start time absolute value should be less than 24bits. */
                        status = gGenfskInvalidParameters_c;
                    }
                    else
                    {
                        genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyPendingRx;
                        
                        GENFSK->T1_CMP = (tempTime & GENFSK_T1_CMP_T1_CMP_MASK) | GENFSK_T1_CMP_T1_CMP_EN_MASK;
                        
                        if (rxDuration)
                        {
                            event.timestamp = rxStartTime + rxDuration;
                            event.callback = GENFSK_RxTimeoutCallback;
                            
                            rxTimeoutTimer = GENFSK_TimeScheduleEvent(&event);
                        }
                        
                        GENFSK_Command(RX_START_T1);
                    }
                }
#if gMWS_Enabled_d
            }
            else
            {
                status = gGenfskFail_c;
            }
#endif
        }
        else
        {
            switch(genfskLocal[mGenfskActiveInstance].genfskState)
            {
            case gGENFSK_LL_NoInit:
                status = gGenfskFail_c;
                break;
            case gGENFSK_LL_BusyRx:
                status = gGenfskBusyRx_c;
                break;
            case gGENFSK_LL_BusyTx:
                status = gGenfskBusyTx_c;
                break;
            case gGENFSK_LL_BusyPendingRx:
                status = gGenfskBusyPendingRx_c;
                break;
            case gGENFSK_LL_BusyPendingTx:
                status = gGenfskBusyPendingTx_c;
                break;
            default:
                status = gGenfskFail_c;
                break;
            }
        }
        
        /* Exit critical section. */
        OSA_InterruptEnable();
    }
    
    return status;
}

genfskStatus_t GENFSK_CancelPendingRx(void)
{
    genfskStatus_t status = gGenfskSuccess_c;
    
    if (mGenfskActiveInstance != gGENFSK_InvalidIdx_c)
    {
        /* Enter critical section. */
        OSA_InterruptDisable();
        
        GENFSK_TimeCancelEvent(rxTimeoutTimer);
        
        GENFSK_Command(RX_CANCEL);
        
        /* Wait for XCVR to become idle. */
        while (GENFSK->XCVR_CTRL & GENFSK_XCVR_CTRL_XCVR_BUSY_MASK){};
        
        /* Disable RX interrupts. */
        GENFSK_DisableInterrupts(GENFSK_IRQ_CTRL_RX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK);
        
        genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength = 0;
        genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer = NULL;
        genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;
        
        /* Exit critical section. */
        OSA_InterruptEnable();
    }
    
    return status;
}

genfskStatus_t GENFSK_AbortAll(void)
{      
    genfskStatus_t status = gGenfskSuccess_c;       
    
    if (mGenfskActiveInstance != gGENFSK_InvalidIdx_c)
    {
        /* Enter critical section. */
        OSA_InterruptDisable();       
                
        GENFSK_TimeCancelEvent(rxTimeoutTimer);
        
        GENFSK_Command(ABORT_ALL);
        
        /* Wait for XCVR to become idle. */
        while (GENFSK->XCVR_CTRL & GENFSK_XCVR_CTRL_XCVR_BUSY_MASK){};
        
        /* Disable interrupts. */
        GENFSK_DisableInterrupts(GENFSK_IRQ_CTRL_RX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK | 
                                    GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK | GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK);

        genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength = 0;
        genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer = NULL;
        genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;
        
        /* Exit critical section. */
        OSA_InterruptEnable();
    }
    
    return status;
}

GENFSK_timestamp_t GENFSK_GetTimestamp(void)
{
    volatile GENFSK_timestamp_t tempTime = 0;

    if (TRUE == genfskAlreadyInit)
    {
        tempTime = GENFSK_TimeGetTimestamp();
    }

    return tempTime;
}

genfskStatus_t GENFSK_SetEventMask(uint8_t instanceId, uint32_t eventMask)
{
    genfskStatus_t status = gGenfskSuccess_c;
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (eventMask > gGenfskAllEvents)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        genfskLocal[instanceId].enabledEvents = eventMask;
    }
    return status;
}

uint32_t GENFSK_GetEventMask(uint8_t instanceId)
{    
    uint32_t eventMask = 0xFF;
    
    if (instanceId < gGENFSK_InstancesCnt_c)
    {
        eventMask = genfskLocal[instanceId].enabledEvents & ~gGENFSK_PromiscuousEventMask_c;
    }
    
    return eventMask;
}

genfskStatus_t GENFSK_RegisterCallbacks(uint8_t instanceId, genfskPacketReceivedCallBack_t packetReceivedCallback, genfskEventNotifyCallBack_t eventCallback)
{
    genfskStatus_t status = gGenfskSuccess_c;
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        /* Enter critical section. */
        OSA_InterruptDisable();
        
        genfskLocal[instanceId].packetReceivedcallback = packetReceivedCallback;
        genfskLocal[instanceId].eventNotifyCallback = eventCallback;
        
        /* Exit critical section. */
        OSA_InterruptEnable();
    }

    return status;
}

genfskStatus_t GENFSK_PacketToByteArray(uint8_t instanceId, GENFSK_packet_t *pPacket, uint8_t *pBuffer)
{   
    uint8_t lengthFieldSize;
    uint8_t h0FieldSize;
    uint8_t h1FieldSize;
    uint8_t syncAddrSize;
    uint64_t tempVar;
    uint16_t tempField;
    uint8_t fieldSizeAdjust;
    
    genfskStatus_t status = gGenfskSuccess_c;
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((pPacket == NULL) || (pBuffer == NULL) || (pPacket->payload == NULL))
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        lengthFieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_LENGTH_SZ_MASK) >> GENFSK_PACKET_CFG_LENGTH_SZ_SHIFT);
        h0FieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H0_SZ_MASK) >> GENFSK_PACKET_CFG_H0_SZ_SHIFT);
        h1FieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H1_SZ_MASK) >> GENFSK_PACKET_CFG_H1_SZ_SHIFT);
        syncAddrSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_SYNC_ADDR_SZ_MASK) >> GENFSK_PACKET_CFG_SYNC_ADDR_SZ_SHIFT);
        
        tempVar = (uint64_t)pPacket->addr << (64 - 8 * (syncAddrSize + 1));
        
        for (uint8_t count = syncAddrSize + 1; count > 0; count --)
        {
            *pBuffer++ = (tempVar >> (64 - 8 * count)) & 0xFF;
        }
        
        fieldSizeAdjust = h1FieldSize;
        tempField = (pPacket->header.h1Field & ((1 << h1FieldSize) - 1));
        tempVar = ((uint64_t)tempField << (64 - h1FieldSize));
        fieldSizeAdjust = h1FieldSize + lengthFieldSize;
        tempField = (pPacket->header.lengthField & ((1 << lengthFieldSize) - 1));
        tempVar |= ((uint64_t)tempField << (64 - fieldSizeAdjust));
        fieldSizeAdjust = h1FieldSize + lengthFieldSize + h0FieldSize;
        tempField = (pPacket->header.h0Field & ((1 << h0FieldSize) - 1));
        tempVar |= ((uint64_t)tempField << (64 - fieldSizeAdjust));
        fieldSizeAdjust = fieldSizeAdjust / 8;
        
        for (uint8_t count = fieldSizeAdjust; count > 0; count --)
        {
            *pBuffer++ = (tempVar >> (64 - 8 * count)) & 0xFF;
        }
        
        FLib_MemCpy(pBuffer, pPacket->payload, pPacket->header.lengthField);
    }
    
    return status;
}

genfskStatus_t GENFSK_ByteArrayToPacket(uint8_t instanceId, uint8_t *pBuffer, GENFSK_packet_t *pPacket)
{
    uint8_t lengthFieldSize;
    uint8_t h0FieldSize;
    uint8_t h1FieldSize;
    uint8_t syncAddrSize;
    uint8_t crcSize;
    uint16_t count = 0;    
    uint8_t fieldSizeAdjust = 0;
    uint64_t tempVar = 0;
    
    genfskStatus_t status = gGenfskSuccess_c;
    
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((pPacket == NULL) || (pBuffer == NULL) || (pPacket->payload == NULL))
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        lengthFieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_LENGTH_SZ_MASK) >> GENFSK_PACKET_CFG_LENGTH_SZ_SHIFT);
        h0FieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H0_SZ_MASK) >> GENFSK_PACKET_CFG_H0_SZ_SHIFT);
        h1FieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H1_SZ_MASK) >> GENFSK_PACKET_CFG_H1_SZ_SHIFT);
        syncAddrSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_SYNC_ADDR_SZ_MASK) >> GENFSK_PACKET_CFG_SYNC_ADDR_SZ_SHIFT);
        crcSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_SZ_MASK) >> GENFSK_CRC_CFG_CRC_SZ_SHIFT);
        
        pPacket->addr = 0x00U;
            
        for (count = 0; count < syncAddrSize + 1; count++)
        {
            pPacket->addr |= (uint32_t)(*pBuffer++) << (8 * count);
        }
        
        fieldSizeAdjust += lengthFieldSize + h0FieldSize + h1FieldSize;
        
        for (count = 0; count < fieldSizeAdjust / 8; count++)
        {
            tempVar |= (uint64_t)(*pBuffer++) << (8 * count);
        }
        
        pPacket->header.h1Field = (tempVar >> (lengthFieldSize + h0FieldSize)) & 0xFF;
        pPacket->header.lengthField = ((uint64_t)tempVar << (64 - (h0FieldSize + lengthFieldSize))) >> (64 - lengthFieldSize);
        pPacket->header.h0Field = ((uint64_t)tempVar << (64 - (h0FieldSize))) >> (64 - h0FieldSize);
        
        FLib_MemCpy(pPacket->payload, pBuffer, pPacket->header.lengthField + crcSize);
    }
    
    return status;
}

static void GENFSK_Task(osaTaskParam_t argument)
{    
    uint64_t tempTime = 0;
    uint16_t byteCount = 0;
    uint8_t rssi = 0;
    osaEventFlags_t ev;
    
    while(1)
    {
        (void)OSA_EventWait(mGenfskTaskEvent, osaEventFlagsAll_c, FALSE, osaWaitForever_c, &ev);             
        
        /* RX event. */
        if (ev & gGenfskRxEventFlag_c)
        {
            byteCount = ((GENFSK->RX_WATERMARK & GENFSK_RX_WATERMARK_BYTE_COUNTER_MASK) >> GENFSK_RX_WATERMARK_BYTE_COUNTER_SHIFT);
            
            /* CRC disabled */
            if (genfskLocal[mGenfskActiveInstance].crcEnable == gGenfskCrcDisable)
            {
                if (byteCount > genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength)
                {
                    GENFSK_ReadPacketBuffer(0, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength);
                    
                    genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;
#if gMWS_Enabled_d
                    MWS_Release(gMWS_GENFSK_c);
#endif
                    
                    /* Packet received OK but allocated length is smaller than the received length. */
                    if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
                    {
                        genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskRxEvent, gGenfskRxAllocLengthFail);
                    }
                }
                else
                {
                    GENFSK_ReadPacketBuffer(0, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, byteCount);
                    
                    rssi = (uint8_t)((GENFSK->XCVR_STS & GENFSK_XCVR_STS_RSSI_MASK) >> GENFSK_XCVR_STS_RSSI_SHIFT);
                    tempTime = gGenfskTimerOverflow | GENFSK->TIMESTAMP;
                    
                    genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;                    
#if gMWS_Enabled_d
                    MWS_Release(gMWS_GENFSK_c);
#endif
                    
                    if (genfskLocal[mGenfskActiveInstance].radioConfig.radioMode == gGenfskMsk)
                    {
                        GENFSK_MskPostProcessing(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                 genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                 byteCount, !(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer[0] & 0x01), 1);
                            
                    }
                    
                    /* Packet received OK. */
                    if (genfskLocal[mGenfskActiveInstance].packetReceivedcallback != NULL)
                    {
                        genfskLocal[mGenfskActiveInstance].packetReceivedcallback(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, byteCount, tempTime, rssi, FALSE);
                    }
                }
            }
            else
            {
                if (GENFSK->IRQ_CTRL & GENFSK_IRQ_CTRL_CRC_VALID_MASK)
                {
                    if (byteCount > genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength)
                    {
                        GENFSK_ReadPacketBuffer(0, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength);
                        
                        genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;
#if gMWS_Enabled_d
                        MWS_Release(gMWS_GENFSK_c);
#endif
                        
                        if (genfskLocal[mGenfskActiveInstance].radioConfig.radioMode == gGenfskMsk)
                        {
                            GENFSK_MskPostProcessing(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                     genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                     byteCount, !(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer[0] & 0x01), 1);
                            
                        }
                        
                        /* Packet received OK but allocated length is smaller than the received length. */
                        if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
                        {
                            genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskRxEvent, gGenfskRxAllocLengthFail);
                        }
                    }
                    else
                    {
                        GENFSK_ReadPacketBuffer(0, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, byteCount);
                        
                        rssi = (uint8_t)((GENFSK->XCVR_STS & GENFSK_XCVR_STS_RSSI_MASK) >> GENFSK_XCVR_STS_RSSI_SHIFT);
                        tempTime = gGenfskTimerOverflow | GENFSK->TIMESTAMP;
                        
                        genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;
#if gMWS_Enabled_d
                        MWS_Release(gMWS_GENFSK_c);
#endif

                        if (genfskLocal[mGenfskActiveInstance].radioConfig.radioMode == gGenfskMsk)
                        {
                            GENFSK_MskPostProcessing(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                     genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                     byteCount, !(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer[0] & 0x01), 1);
                            
                        }
                        
                        /* Packet received OK. */
                        if (genfskLocal[mGenfskActiveInstance].packetReceivedcallback != NULL)
                        {
                            genfskLocal[mGenfskActiveInstance].packetReceivedcallback(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, byteCount, tempTime, rssi, TRUE);
                        }
                    }
                }
                else
                {
                    if (byteCount > genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength)
                    {
                        GENFSK_ReadPacketBuffer(0, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength);
                        
                        genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;
#if gMWS_Enabled_d
                        MWS_Release(gMWS_GENFSK_c);
#endif
                        
                        if (genfskLocal[mGenfskActiveInstance].radioConfig.radioMode == gGenfskMsk)
                        {
                            GENFSK_MskPostProcessing(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                     genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                     byteCount, !(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer[0] & 0x01), 1);
                            
                        }
                        
                        if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
                        {
                            genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskRxEvent, gGenfskCRCInvalid);
                        }
                    }
                    else
                    {
                        if (genfskLocal[mGenfskActiveInstance].crcRecvInvalid == gGenfskCrcRecvInvalid)
                        {
                            GENFSK_ReadPacketBuffer(0, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, byteCount);
                            
                            rssi = (uint8_t)((GENFSK->XCVR_STS & GENFSK_XCVR_STS_RSSI_MASK) >> GENFSK_XCVR_STS_RSSI_SHIFT);
                            tempTime = gGenfskTimerOverflow | GENFSK->TIMESTAMP;
      
                            genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;
#if gMWS_Enabled_d
                            MWS_Release(gMWS_GENFSK_c);
#endif

                            if (genfskLocal[mGenfskActiveInstance].radioConfig.radioMode == gGenfskMsk)
                            {
                                GENFSK_MskPostProcessing(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                         genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                         byteCount, !(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer[0] & 0x01), 1);
                            
                            }
                            
                            /* Packet received with CRC invalid. */
                            if (genfskLocal[mGenfskActiveInstance].packetReceivedcallback != NULL)
                            {
                                genfskLocal[mGenfskActiveInstance].packetReceivedcallback(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, byteCount, tempTime, rssi, FALSE);
                            }
                        }
                        else
                        {
                            GENFSK_ReadPacketBuffer(0, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength);

                            genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;                            
#if gMWS_Enabled_d
                            MWS_Release(gMWS_GENFSK_c);
#endif
                            
                            if (genfskLocal[mGenfskActiveInstance].radioConfig.radioMode == gGenfskMsk)
                            {
                                GENFSK_MskPostProcessing(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                         genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                                         byteCount, !(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer[0] & 0x01), 1);
                            
                            }
                            
                            if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
                            {
                                genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskRxEvent, gGenfskCRCInvalid);
                            }
                        }
                    }
                }
            }
        }
        
        /* TX event. */
        if (ev & gGenfskTxEventFlag_c)
        {
            genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;
#if gMWS_Enabled_d
            MWS_Release(gMWS_GENFSK_c);
#endif            
            if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
            {
                genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskTxEvent, gGenfskSuccess);
            }
        }
        
        /* H1 Fail event. */
        if (ev & gGenfskH0FailEventFlag_c)
        {
#if gMWS_Enabled_d
            MWS_Release(gMWS_GENFSK_c);
#endif
            if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
            {
                    genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskRxEvent, gGenfskH0Fail);
            }
        }
        
        /* Length Fail event. */
        if (ev & gGenfskLengthFailEventFlag_c)
        {
#if gMWS_Enabled_d
            MWS_Release(gMWS_GENFSK_c);
#endif
            if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
            {
                genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskRxEvent, gGenfskLengthFail);
            }
        }
        
        /* H1 Fail event. */
        if (ev & gGenfskH1FailEventFlag_c)
        {
#if gMWS_Enabled_d
            MWS_Release(gMWS_GENFSK_c);
#endif
            if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
            {                
                genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskRxEvent, gGenfskH1Fail);
            }
        }
        
        /* Network Address Match event. */
        if (ev & gGenfskNwkAddressMatchEventFlag_c)
        {
            if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
            {
                genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskNwkAddressMatch, gGenfskSuccess);
            }
        }
        
        /* RX PLL Unlock event. */
        if (ev & gGenfskRxPllUnlockEventFlag_c)
        {
#if gMWS_Enabled_d
            MWS_Release(gMWS_GENFSK_c);
#endif
            if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
            {
                genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskRxEvent, gGenfskSyncLost);
            }
        }
         
        /* TX PLL Unlock event. */
        if (ev & gGenfskTxPllUnlockEventFlag_c)
        {
#if gMWS_Enabled_d
            MWS_Release(gMWS_GENFSK_c);
#endif
            if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
            {
                genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskTxEvent, gGenfskSyncLost);
            }
        }
        
        /* Wake event. */
        if (ev & gGenfskWakeEventFlag_c)
        {           
            if (genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL)
            {
                genfskLocal[mGenfskActiveInstance].eventNotifyCallback(gGenfskWakeEvent, gGenfskSuccess);
            }
        }
        
        /* For BM break the while() loop. */
        if (gUseRtos_c == 0)
        {
            break;
        }
    }    
}

#if gMWS_Enabled_d
static uint32_t MWS_GENFSK_Callback(mwsEvents_t event)
{
    uint32_t status = gMWS_Success_c;
    
    switch(event)
    {
    case gMWS_Init_c:
        /* Do nothing */
        break;
    case gMWS_Active_c:
        GENFSK_UnprotectFromXcvrInterrupt();
        break;
    case gMWS_Idle_c:
        /* Do nothing */
        break;
    case gMWS_Abort_c:
        /* Enter critical section */
        OSA_InterruptDisable();
        
        if (genfskLocal[mGenfskActiveInstance].genfskState != gGENFSK_LL_NoInit)
        {
            if (genfskLocal[mGenfskActiveInstance].genfskState != gGENFSK_LL_Idle)
            {
                GENFSK_AbortAll();
            }
            
            GENFSK_ProtectFromXcvrInterrupt();
        }
        
        /* Exit critical section */
        OSA_InterruptEnable();        
        break;
    case gMWS_Release_c:
        /* Enter critical section */
        OSA_InterruptDisable();    
        
        if (genfskLocal[mGenfskActiveInstance].genfskState != gGENFSK_LL_NoInit)
        {           
            if (genfskLocal[mGenfskActiveInstance].genfskState != gGENFSK_LL_Idle)
            {
                GENFSK_AbortAll();
            }
            
            GENFSK_ProtectFromXcvrInterrupt();
        }
        
        /* Exit critical section */
        OSA_InterruptEnable();        
        break;
    default:
        status = gMWS_InvalidParameter_c;
        break;
    }
    
    return status;
}
#endif /* gMWS_Enabled_d */

