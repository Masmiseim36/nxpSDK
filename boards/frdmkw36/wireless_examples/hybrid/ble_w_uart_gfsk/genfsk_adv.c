/*! *********************************************************************************
* \addtogroup Beacon
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
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
#include "EmbeddedTypes.h"

/* Framework / Drivers */
#include "EmbeddedTypes.h"
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "MemManager.h"
#include "Panic.h"
#include "SerialManager.h"
#include "MWS.h"

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
#include "PWR_Interface.h"
#include "PWR_Configuration.h"
#endif

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
    (void)GENFSK_Init();

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
    (void)TMR_StartIntervalTimer(mAppGenfskTmr, gGenFskApp_TxInterval_c, GfskApp_Tx, NULL);

    /* Check if GenFSK is the active protocol */
    if (MWS_GetActiveProtocol() != gMWS_GENFSK_c)
    {
        if (gMWS_Success_c == MWS_Acquire(gMWS_GENFSK_c, FALSE))
        {
            /* The GenFSK acquired access to the resources. Program the release timer. */
            duration = MWS_GetInactivityDuration(gMWS_GENFSK_c) / 1000;
            (void)TMR_StartSingleShotTimer(mAppGenfskCoexistenceTmr, duration, GfskApp_CoexistenceTimeout, NULL);
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
                    /* The GenFSK acquired access to the resources. Program the release timer. */
                    (void)TMR_StartSingleShotTimer(mAppGenfskCoexistenceTmr, duration, GfskApp_CoexistenceTimeout, NULL);
                }
            }

            (void)GENFSK_StartRx(mAppGenfskId, buffer, (uint16_t)sizeof(buffer), 0U, 0U);
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
        (void)GENFSK_AbortAll();
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
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* BD_ADDR */
#ifdef TEST_GENFSK_ADV
    0x02, 0x01, 0x06, 0x1B, 0xFF, 0x25, 0x00, 0xBC, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E
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

    if (MWS_GetActiveProtocol() == gMWS_GENFSK_c)
    {
        /* Copy Device's address into the GenFSK payload */
        FLib_MemCpy(&message[gcBleDeviceAddressSize_c], gBD_ADDR, gcBleDeviceAddressSize_c);

        (void)GENFSK_PacketToByteArray(mAppGenfskId, &pkt, buffer);
        /* Compute GenFSK Tx duration and Radio Idle duration. */
        txTime = GENFSK_GetTxDuration(mAppGenfskId, len) + (gGenFskApp_GuardTime_c * 1000);
        idleTime = MWS_GetInactivityDuration(gMWS_GENFSK_c);

        /* Check if there is enough idle time to perform the TX */
        if (txTime < idleTime)
        {
            mAppGenfskTxPending = FALSE;
            (void)GENFSK_AbortAll();
            (void)GENFSK_StartTx(mAppGenfskId, buffer, len, 0U);
        }
        else
        {
            mAppGenfskTxPending = TRUE;
        }
    }
    else
    {
        mAppGenfskTxPending = TRUE;
    }
}

/*! *********************************************************************************
* \brief  This function is the timer callback for the GenFSK
********************************************************************************** */
static void GfskApp_CoexistenceTimeout(void * p)
{
    (void)p;
    (void)GENFSK_AbortAll();
    (void)MWS_Release(gMWS_GENFSK_c);
}

/*! *********************************************************************************
* \brief  This function is called when a MWS event is generated.
* \param[in]  event The mws event.
*
* \remarks  This function should call the previously registered MWS callback
*
********************************************************************************** */
static uint32_t App_MwsCallback ( mwsEvents_t event )
{
    uint32_t status = 0;
    uint32_t duration;
    /* Call previously registered callback. */
    if (mGFSK_LL_CB)
    {
        status = mGFSK_LL_CB(event);
    }

    switch(event)
    {
    case gMWS_Idle_c:
        /* All other protocols are Idle. GenFSK can become active */
        duration = MWS_GetInactivityDuration(gMWS_GENFSK_c) / 1000;

        if ((duration > gGenFskApp_GuardTime_c) && (gMWS_Success_c == MWS_Acquire(gMWS_GENFSK_c, FALSE)))
        {
            duration -= gGenFskApp_GuardTime_c;

            if (mAppGenfskTxPending)
            {
                GfskApp_Tx(NULL);
            }
            else
            {
                if (mAppGenfskRxOn)
                {
                    (void)GENFSK_StartRx(mAppGenfskId, buffer, (uint16_t)sizeof(buffer), 0U, 0U);
                }
            }

            /* The GenFSK acquired access to the resources. Program the release timer. */
            (void)TMR_StartSingleShotTimer(mAppGenfskCoexistenceTmr, duration, GfskApp_CoexistenceTimeout, NULL);
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
        /* Another protocol has released the XCVR. GenFSK can become active */
        break;

    default:
        break;
    }

    return status;
}

/*! *********************************************************************************
* @}
********************************************************************************** */
