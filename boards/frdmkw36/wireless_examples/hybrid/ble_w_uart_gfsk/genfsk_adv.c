/*! *********************************************************************************
* \addtogroup Beacon
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the Beacon application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/* Framework / Drivers */
#include "EmbeddedTypes.h"
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "MemManager.h"
#include "SerialManager.h"
#include "MWS.h"

#include "PWR_Interface.h"


/* Application */
#include "board.h"
#include "genfsk_adv.h"
#include "genfsk_interface.h"
#include "genfsk_utils.h"

#include "ble_constants.h"

#define mHeaderSize_c (6U)

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

#ifndef APP_DBG_LOG
#define APP_DBG_LOG(...)
#endif

#if !((defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES)))
#define BOARD_DBGAPPIOSET(x,y)
#endif /* !((defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))) */

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
extern uint8_t gBD_ADDR[gcBleDeviceAddressSize_c];

static uint8_t gAppSerMgrIf;

uint8_t mAppGenfskId;
tmrTimerID_t mAppGenfskTmr;
tmrTimerID_t mAppGenfskCoexistenceTmr;
static uint8_t buffer[250];

/* packet configuration */
static GENFSK_packet_config_t pktConfig =
{
    .preambleSizeBytes = 0, /*1 byte of preamble*/
    .packetType = /*gGenfskRawPacket,*/gGenfskFormattedPacket,
    .lengthSizeBits = gGenFskDefaultLengthFieldSize_c,
    .lengthBitOrder = gGenfskLengthBitLsbFirst,
    /*sync address bytes = size + 1*/
    .syncAddrSizeBytes = gGenFskDefaultSyncAddrSize_c,
    .lengthAdjBytes = gGenFskDefaultCrcSize_c, /*length field not including CRC so adjust by crc len*/
    .h0SizeBits = gGenFskDefaultH0FieldSize_c,
    .h1SizeBits = gGenFskDefaultH1FieldSize_c,
    .h0Match = gGenFskDefaultH0Value_c, /*match field containing zeros*/
    .h0Mask = gGenFskDefaultH0Mask_c,
    .h1Match = gGenFskDefaultH1Value_c,
    .h1Mask = gGenFskDefaultH1Mask_c
};

/* CRC configuration */
static GENFSK_crc_config_t crcConfig =
{
    .crcEnable = gGenfskCrcEnable,
    .crcSize = gGenFskDefaultCrcSize_c,
    .crcStartByte = gGenFskDefaultSyncAddrSize_c+1U,
    .crcRefIn = gGenfskCrcInputNoRef,
    .crcRefOut = gGenfskCrcOutputNoRef,
    .crcByteOrder = gGenfskCrcLSByteFirst,
    .crcSeed = gGenFskDefaultCrcSeed_c,
    .crcPoly = gGenFskDefaultCrccPoly_c,
    .crcXorOut = 0
};

/* whitener configuration */
static GENFSK_whitener_config_t whitenConfig =
{
    .whitenEnable = gGenfskWhitenEnable,
    .whitenStart = gWhitenStartWhiteningAtH0,
    .whitenEnd = gWhitenEndAtEndOfCrc,
    .whitenB4Crc = gCrcB4Whiten,
    .whitenPolyType = gGaloisPolyType,
    .whitenRefIn = gGenfskWhitenInputNoRef,
    .whitenPayloadReinit = gGenfskWhitenNoPayloadReinit,
    .whitenSize = 7,
    .whitenInit = 0x53,
    .whitenPoly = 0x11, /*x^7 + x^4 + 1! x^7 is never set*/
    .whitenSizeThr = 0,
    .manchesterEn = gGenfskManchesterDisable,
    .manchesterStart = gGenfskManchesterStartAtPayload,
    .manchesterInv = gGenfskManchesterNoInv,
};

/* radio configuration */
static GENFSK_radio_config_t radioConfig =
{
    .radioMode = gGenfskGfskBt0p5h0p5,
    .dataRate = gGenfskDR1Mbps
};

/* network / sync address configuration */
static GENFSK_nwk_addr_match_t ntwkAddr =
{
    .nwkAddrSizeBytes = gGenFskDefaultSyncAddrSize_c,
    .nwkAddrThrBits = 0,
    .nwkAddr = gGenFskDefaultSyncAddress_c,
};

/* Pointer to the MWS callback register previously */
static pfMwsCallback mGFSK_LL_CB = NULL;

static bool_t mAppGenfskRxOn = FALSE;
static bool_t mAppGenfskTxPending = FALSE;
static bool_t mAppGenfskTxOngoing = FALSE;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void GfskApp_Tx(void * p);
static void GFSK_PacketReceived (uint8_t *pBuffer, uint16_t bufferLength, uint64_t timestamp, int8_t rssi, uint8_t crcValid);
static void GFSK_EventNotify (genfskEvent_t event, genfskEventStatus_t eventStatus);
static uint32_t App_MwsCallback ( mwsEvents_t event );
static void GfskApp_CoexistenceTimeout(void * p);
static void GfskApp_RestartRx(void);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief        Initializat GFSK layer
*
********************************************************************************** */
void GfskApp_Init(uint8_t appSerMgrIf)
{
    genfskStatus_t status;

    status = GENFSK_Init();
    if (status != gGenfskSuccess_c)
    {
        Serial_Print(gAppSerMgrIf, "\n\rGFSK initialization error!", gNoBlock_d);
        assert(0);
        return;
    }

    /* GENFSK LL Init with default register config */
    (void)GENFSK_AllocInstance(&mAppGenfskId, NULL, NULL, NULL);

    /*set bitrate*/
    (void)GENFSK_RadioConfig(mAppGenfskId, &radioConfig);
    /*set packet config*/
    (void)GENFSK_SetPacketConfig(mAppGenfskId, &pktConfig);
    /*set whitener config*/
    (void)GENFSK_SetWhitenerConfig(mAppGenfskId, &whitenConfig);
    /*set crc config*/
    (void)GENFSK_SetCrcConfig(mAppGenfskId, &crcConfig);

    /*set network address at location 0 and enable it*/
    (void)GENFSK_SetNetworkAddress(mAppGenfskId, 0, &ntwkAddr);
    (void)GENFSK_EnableNetworkAddress(mAppGenfskId, 0);

    /*set tx power level*/
    (void)GENFSK_SetTxPowerLevel(mAppGenfskId, gGenFskDefaultTxPowerLevel_c);
    /*set channel: Freq = 2360MHz + ChannNumber*1MHz*/
    (void)GENFSK_SetChannelNumber(mAppGenfskId, gGenFskDefaultChannel_c);

    (void)GENFSK_RegisterCallbacks(mAppGenfskId, GFSK_PacketReceived, GFSK_EventNotify);

    /* Allocate Tx timer */
    mAppGenfskTmr = TMR_AllocateTimer();
    /* Allocate protocol coexistence timer */
    mAppGenfskCoexistenceTmr = TMR_AllocateTimer();
    gAppSerMgrIf = appSerMgrIf;

    /* Get previously registered MWS callback, and install application callback */
    mGFSK_LL_CB = MWS_GetRegisteredCallback(gMWS_GENFSK_c);
    (void)MWS_Register(gMWS_GENFSK_c, App_MwsCallback);
}


/*! *********************************************************************************
* \brief        Start GFSK App timer for periodic TX
*
********************************************************************************** */
void GfskApp_StartTx(void)
{
    uint32_t duration;

    (void)Serial_Print(gAppSerMgrIf, "\n\rGFSK: Start TX... ", gNoBlock_d);
    (void)TMR_StartLowPowerTimer(mAppGenfskTmr, gTmrIntervalTimer_c, gGenFskApp_TxInterval_c, GfskApp_Tx, NULL);
    mAppGenfskTxPending = TRUE;
    mAppGenfskTxOngoing = FALSE;
    PWR_AllowDeviceToSleep();
    BOARD_DBGAPPIOSET(5, 1);

    /* Check if GenFSK is the active protocol */
    if (MWS_GetActiveProtocol() != gMWS_GENFSK_c)
    {
        if (gMWS_Success_c == MWS_Acquire(gMWS_GENFSK_c, FALSE))
        {
            /* The GenFSK acquired access to the resources. Program the release timer. */
            duration = MWS_GetInactivityDuration(gMWS_GENFSK_c) / 1000;
            APP_DBG_LOG("acq_genfsk_dur=%d" , duration);
            (void)TMR_StartLowPowerTimer(mAppGenfskCoexistenceTmr, gTmrSingleShotTimer_c, duration, GfskApp_CoexistenceTimeout, NULL);
        }
    }
}

/*! *********************************************************************************
* \brief        Stop GFSK App timer
*
********************************************************************************** */
void GfskApp_StopTx(void)
{
    (void)Serial_Print(gAppSerMgrIf, "\n\rGFSK: Stop TX", gNoBlock_d);
    (void)TMR_StopTimer(mAppGenfskTmr);
    mAppGenfskTxPending = FALSE;
}

/*! *********************************************************************************
* \brief        Start GFSK App timer for periodic RX
*
********************************************************************************** */
void GfskApp_StartRx(void)
{
    (void)Serial_Print(gAppSerMgrIf, "\n\rGFSK: Start RX...", gNoBlock_d);

    if (!mAppGenfskRxOn)
    {
        uint32_t duration = MWS_GetInactivityDuration(gMWS_GENFSK_c) / 1000;

        mAppGenfskRxOn = TRUE;

        /* Check if there is enogh idle time */
        if (duration > gGenFskApp_GuardTime_c)
        {
            duration -= gGenFskApp_GuardTime_c;

            /* Check if GenFSK is the active protocol */
            if (MWS_GetActiveProtocol() != gMWS_GENFSK_c)
            {
                if (gMWS_Success_c == MWS_Acquire(gMWS_GENFSK_c, FALSE))
                {
                    (void)GENFSK_StartRx(mAppGenfskId, buffer, (uint16_t)sizeof(buffer), 0U, 0U);
                    /* The GenFSK acquired access to the resources. Program the release timer. */
                    (void)TMR_StartLowPowerTimer(mAppGenfskCoexistenceTmr, gTmrSingleShotTimer_c, duration, GfskApp_CoexistenceTimeout, NULL);
                }
            }
        }
    }
}

/*! *********************************************************************************
* \brief        Stop GFSK App timer
*
********************************************************************************** */
void GfskApp_StopRx(void)
{
    (void)Serial_Print(gAppSerMgrIf, "\n\rGFSK: Stop RX", gNoBlock_d);

    if (mAppGenfskRxOn)
    {
        mAppGenfskRxOn = FALSE;
    }
}


/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief        Restart the GFSK RX sequence if there is enough idle time
*
********************************************************************************** */
static void GfskApp_RestartRx(void)
{
    uint32_t duration = MWS_GetInactivityDuration(gMWS_GENFSK_c) / 1000;

    /* Check if there is enogh idle time to restart RX. */
    if (duration > gGenFskApp_GuardTime_c)
    {
        (void)GENFSK_StartRx(mAppGenfskId, buffer, (uint16_t)sizeof(buffer), 0U, 0U);
    }
}

/*! *********************************************************************************
* \brief        Handles packets received over GFSK
*
********************************************************************************** */
static void GFSK_PacketReceived (uint8_t *pBuffer, uint16_t bufferLength, uint64_t timestamp, int8_t rssi, uint8_t crcValid)
{
    uint8_t addrOfInterest[6] = {gGenFSK_AdvAddress_c};
    (void)timestamp;
    (void)rssi;
    (void)crcValid;

    /* Restart RX asap */
    GfskApp_RestartRx();

    if ((bufferLength >= (mHeaderSize_c + sizeof(addrOfInterest) + sizeof(gBD_ADDR))) &&
       FLib_MemCmp(addrOfInterest, &pBuffer[mHeaderSize_c], sizeof(addrOfInterest)))
    {
        (void)Serial_Print(gAppSerMgrIf, "\n\rGFSK: ", gNoBlock_d);
        (void)Serial_PrintHex(gAppSerMgrIf,
                        &pBuffer[mHeaderSize_c], /* Skip header */
                        sizeof(addrOfInterest) + sizeof(gBD_ADDR),
                        gPrtHexNewLine_c);
    }
}

/*! *********************************************************************************
* \brief        Handles GFSK events
*
********************************************************************************** */
static void GFSK_EventNotify (genfskEvent_t event, genfskEventStatus_t eventStatus)
{
    (void)event;
    (void)eventStatus;

    if ((event == gGenfskRxEvent) ||
        ((event == gGenfskTxEvent) && (mAppGenfskRxOn == TRUE)))
    {
        /* Rx sequence complete. Restart it. */
        GfskApp_RestartRx();
    }
    if (event == gGenfskTxEvent)
    {
        mAppGenfskTxOngoing = FALSE;

        PWR_AllowDeviceToSleep();
        BOARD_DBGAPPIOSET(5, 1);
        BOARD_DBGAPPIOSET(5, 0);
        BOARD_DBGAPPIOSET(5, 1);
    }
}

/*! *********************************************************************************
* \brief        Transmit a packet over GFSK
*
********************************************************************************** */
static void GfskApp_Tx(void * p)
{
    (void)p;
    uint16_t len;
    /* Dummy packet */
    uint8_t message[] = {gGenFSK_AdvAddress_c, /* AdvA */

#ifdef TEST_GENFSK_ADV
    /* raw advertising data: flags and device name (GFSK_AD) */
    0x02, 0x01, 0x06, 0x08, 0x08, 0x47, 0x46, 0x53, 0x4B, 0x5F, 0x41, 0x44
#endif
    };
    uint32_t txTime;
    uint32_t idleTime;

    GENFSK_packet_t pkt = {
        .addr = gGenFskDefaultSyncAddress_c,
#ifdef TEST_GENFSK_ADV
        .header.h0Field = 2,
#else
        .header.h0Field = 0,
#endif
        .header.h1Field = 0,
        .header.lengthField = (uint16_t)sizeof(message),
        .payload = message
    };

    len = pkt.header.lengthField +
          (gGenFskDefaultHeaderSizeBytes_c)+
          (gGenFskDefaultSyncAddrSize_c + 1U);

    if ((MWS_GetActiveProtocol() == gMWS_GENFSK_c) && (!mAppGenfskTxOngoing))
    {
        (void)GENFSK_PacketToByteArray(mAppGenfskId, &pkt, buffer);
        /* Compute GenFSK Tx duration and Radio Idle duration. */
        txTime = GENFSK_GetTxDuration(mAppGenfskId, len) + (gGenFskApp_GuardTime_c * 1000);
        idleTime = MWS_GetInactivityDuration(gMWS_GENFSK_c);

        /* Check if there is enough idle time to perform the TX */
        if (txTime < idleTime)
        {
            genfskStatus_t status = GENFSK_StartTx(mAppGenfskId, buffer, len, 0U);

#if (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
            /* Different possibilities:
                - if only GENFSK TX is enabled the GENFSK_StartTx() function returns gGenfskSuccess_c 
                - if both GENFSK RX and TX are enabled, the GENFSK_StartTx() function returns gGenfskBusyRx_c (the RX activity is not stopped until a valid RX packet is received).
                  So to allow for TX activity, the RX activity needs first to be stopped (aborted) to map a GENFSK TX activity. 
            */  
            if (status != gGenfskSuccess_c)
            {
                /* This is a workaround to cope with abort issue during RX warmup, see CONNRF-767 */
                if ((GENFSK->SEQ_STS & GENFSK_SEQ_STS_RX_IN_WARMUP_MASK) != 0x0U)
                {
                    /* If RX is in warmup then wait for warmup to complete before issuing abort to ensure ABORT does not crash RX */
                    uint32_t end_of_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT;
                    while ((( XCVR_MISC->XCVR_STATUS & XCVR_MISC_XCVR_STATUS_TSM_COUNT_MASK) >> XCVR_MISC_XCVR_STATUS_TSM_COUNT_SHIFT ) < end_of_wu) {}; /* Wait for TSM RX WU complete */
                }
                GENFSK_AbortAll();
                status = GENFSK_StartTx(mAppGenfskId, buffer, len, 0U);
            }
#endif /* (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES)) */
            if (status == gGenfskSuccess_c)
            {
                mAppGenfskTxOngoing = TRUE;
                PWR_DisallowDeviceToSleep();
                BOARD_DBGAPPIOSET(5, 0);
            }
        }
    }
}

/*! *********************************************************************************
* \brief  This function is the timer callback for the GenFSK
********************************************************************************** */
static void GfskApp_CoexistenceTimeout(void * p)
{
    (void)p;
    (void)MWS_Release(gMWS_GENFSK_c);
}

/*! *********************************************************************************
* \brief  This function is called when a MWS event is generated.
* \param[in]  event The mws event.
*
* \remarks  This function should call the previously registered MWS callback
*
********************************************************************************** */
static uint32_t count_for_switch = 0; 
static uint32_t App_MwsCallback ( mwsEvents_t event )
{
    uint32_t status = 0;
    uint32_t duration;
    /* Call previously registered callback. */
    if (mGFSK_LL_CB)
    {
        status = mGFSK_LL_CB(event);
    }

    APP_DBG_LOG("evt=%d", event);

    switch(event)
    {
    case gMWS_Idle_c:
        if (mAppGenfskTxPending || mAppGenfskRxOn)
        {
            /* All other protocols are Idle. GenFSK can become active */
            duration = MWS_GetInactivityDuration(gMWS_GENFSK_c) / 1000;
            if(duration > gGenFskApp_GuardTime_c)
            {
                if (gMWS_Success_c == MWS_Acquire(gMWS_GENFSK_c, FALSE))
                {
                    if ((mAppGenfskTxPending) && (mAppGenfskRxOn))
                    {
                        /* If both Rx and Tx enabled, alternate between Rx and Tx. */
                        if (count_for_switch & 0x1)
                        {
                            BOARD_DBGAPPIOSET(5, 1);
                            mAppGenfskTxOngoing = FALSE;
                            PWR_AllowDeviceToSleep();
                            GfskApp_Tx(NULL);
                        }
                        else
                        {
                            (void)GENFSK_StartRx(mAppGenfskId, buffer, (uint16_t)sizeof(buffer), 0U, 0U);
                        }
                        count_for_switch++;
                    }
                    else if (mAppGenfskTxPending)
                    {
                        BOARD_DBGAPPIOSET(5, 1);
                        mAppGenfskTxOngoing = FALSE;
                        PWR_AllowDeviceToSleep();
                        GfskApp_Tx(NULL);
                    }
                    else
                    {
                        if (mAppGenfskRxOn)
                        {
                            (void)GENFSK_StartRx(mAppGenfskId, buffer, (uint16_t)sizeof(buffer), 0U, 0U);
                        }
                    }
					duration = MWS_GetInactivityDuration(gMWS_GENFSK_c) / 1000;
                    duration -= gGenFskApp_GuardTime_c;
                    /* The GenFSK acquired access to the resources. Program the release timer. */
                    (void)TMR_StartLowPowerTimer(mAppGenfskCoexistenceTmr, gTmrSingleShotTimer_c, duration, GfskApp_CoexistenceTimeout, NULL);	
                }
				else
                {
                    MWS_SignalIdle(gMWS_GENFSK_c);
                }
            }
            else
            {
                MWS_SignalIdle(gMWS_GENFSK_c);
            }
        }
        else
        {
            MWS_SignalIdle(gMWS_GENFSK_c);
        }
        break;

    case gMWS_Abort_c:
        /* Abort signal received from other protocol. Stop everythig and release access. */
        (void)TMR_StopTimer(mAppGenfskCoexistenceTmr);
        break;

    case gMWS_Release_c:
        /* GenFSK has released the XCVR. Another protocol can become active */
        break;

    default:
        {
            ; /* No action required */
        }
        break;
    }

    return status;
}

/*! *********************************************************************************
* @}
********************************************************************************** */
