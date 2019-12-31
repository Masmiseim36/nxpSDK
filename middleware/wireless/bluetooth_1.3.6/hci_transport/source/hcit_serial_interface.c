/*! *********************************************************************************
* \addtogroup HCI
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"
#include "MemManager.h"

#include "ble_general.h"
#include "hci_transport.h"
#include "fsl_device_registers.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef PACKED_STRUCT hciCommandPacketHeader_tag
{
    uint16_t    opCode;
    uint8_t     parameterTotalLength;
}hciCommandPacketHeader_t;

typedef PACKED_STRUCT hciAclDataPacketHeader_tag
{
    uint16_t    handle      :12;
    uint16_t    pbFlag      :2;
    uint16_t    bcFlag      :2;
    uint16_t    dataTotalLength;
}hciAclDataPacketHeader_t;

typedef PACKED_STRUCT hciEventPacketHeader_tag
{
    hciEventCode_t  eventCode;
    uint8_t     dataTotalLength;
}hciEventPacketHeader_t;

typedef PACKED_STRUCT hcitPacketHdr_tag
{
    hciPacketType_t packetTypeMarker;
    PACKED_UNION
    {
        hciAclDataPacketHeader_t    aclDataPacket;
        hciEventPacketHeader_t      eventPacket;
        hciCommandPacketHeader_t    commandPacket;
    };
}hcitPacketHdr_t;

typedef PACKED_STRUCT hcitPacketStructured_tag
{
    hcitPacketHdr_t header;
    uint8_t         payload[gHcitMaxPayloadLen_c];
} hcitPacketStructured_t;

typedef PACKED_UNION hcitPacket_tag
{
    /* The entire packet as unformatted data. */
    uint8_t raw[sizeof(hcitPacketStructured_t)];
}hcitPacket_t;

typedef struct hcitComm_tag
{
    hcitPacket_t        *pPacket;
    hcitPacketHdr_t     pktHeader;
    uint16_t            bytesReceived;
    uint16_t            expectedLength;
}hcitComm_t;

typedef enum{
    mDetectMarker_c       = 0,
    mDetectHeader_c,
    mPacketInProgress_c
}detectState_t;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static bool_t   mHcitInit = FALSE;
static uint8_t  mHcitSerMgrIf;

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c)
static hcitComm_t               mHcitData;
static hciTransportInterface_t  mTransportInterface;
static detectState_t            mPacketDetectStep;
static hcitPacket_t             mHcitPacketRaw;
#endif  /* gSerialManagerMaxInterfaces_c */

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c)
static void Hcit_RxCallBack(void *pData);
#endif
static void Hcit_FreePacket(void *pPacket);

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
#if gUseHciTransportDownward_d
    osaSemaphoreId_t gHciDataBufferingSem;
#endif /* gUseHciTransportDownward_d */

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief
*
* \param[in]
*
* \param[out]
*
* \return
*
* \pre
*
* \remarks
*
********************************************************************************** */
bleResult_t Hcit_Init( hcitConfigStruct_t* hcitConfigStruct )
{
    bleResult_t result = gHciSuccess_c;

    if( mHcitInit == FALSE )
    {
#if gUseHciTransportDownward_d
        gHciDataBufferingSem = OSA_SemaphoreCreate(0);

        if (gHciDataBufferingSem == NULL)
        {
            return gHciTransportError_c;
        }
#endif /* gUseHciTransportDownward_d */

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c)
        /* Initialize HCI Transport interface */
        mTransportInterface = hcitConfigStruct->transportInterface;

#if (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
        Serial_InitManager();
#else
        SerialManager_Init();
#endif
        /* Initialize HCI Transport */
        if (Serial_InitInterface(&mHcitSerMgrIf,
                                 hcitConfigStruct->interfaceType,
                                 hcitConfigStruct->interfaceChannel) != gSerial_Success_c)
        {
            return gHciTransportError_c;
        }

        (void)Serial_SetBaudRate(mHcitSerMgrIf, hcitConfigStruct->interfaceBaudrate);

        /* Install Controller Events Callback handler */
        (void)Serial_SetRxCallBack(mHcitSerMgrIf, Hcit_RxCallBack, NULL);
#endif  /* gSerialManagerMaxInterfaces_c */

        /* Flag initialization on module */
        mHcitInit = TRUE;
    }
    else
    {
        /* Module has already been initialized */
        result = gHciAlreadyInit_c;
    }

    return result;
}

/*! *********************************************************************************
* \brief
*
* \param[in]
*
* \param[out]
*
* \return
*
* \pre
*
* \remarks
*
********************************************************************************** */
bleResult_t Hcit_SendPacket
    (
        hciPacketType_t packetType,
        void*           pPacket,
        uint16_t        packetSize
    )
{
    uint8_t*        pSerialPacket = NULL;
    bleResult_t     result = gBleSuccess_c;
    serialStatus_t  status = gSerial_Success_c;

    pSerialPacket = MEM_BufferAlloc(1UL + (uint32_t)packetSize);

    if( NULL != pSerialPacket )
    {
        pSerialPacket[0] = (uint8_t)packetType;
        FLib_MemCpy(pSerialPacket+1, (uint8_t*)pPacket, packetSize);
        status = Serial_AsyncWrite(mHcitSerMgrIf, pSerialPacket, 1U + packetSize, Hcit_FreePacket, pSerialPacket);

        if( gSerial_Success_c != status )
        {
            Hcit_FreePacket(pSerialPacket);
            result = gHciTransportError_c;
        }
    }
    else
    {
        result = gBleOutOfMemory_c;
    }

    return result;
}

/*! *********************************************************************************
* \brief
*
* \param[in]
*
* \param[out]
*
* \return
*
* \pre
*
* \remarks
*
********************************************************************************** */
bleResult_t Hcit_RecvPacket
    (
        void*           pPacket,
        uint16_t        packetSize
    )
{
    bleResult_t result = gHciSuccess_c;

    uint8_t* aData = (uint8_t*) pPacket;
    uint8_t type = aData[0];

    if (type != 0x01U && type != 0x02U && type != 0x04U)
    {
        result = /* Something more meaningful? */ gHciTransportError_c;
    }
    else
    {
        hciPacketType_t packetType = (hciPacketType_t) type;
        result = Ble_HciRecv
        (
            packetType,
            aData + 1,
            packetSize - 1U
        );

        Hcit_FreePacket(pPacket);  ///TODO: Verify here in FSCI
    }

    return result;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c)
static inline void Hcit_SendMessage(void)
{
    /* Send the message to HCI */
    (void)mTransportInterface(mHcitData.pktHeader.packetTypeMarker,
                              mHcitData.pPacket,
                              mHcitData.bytesReceived);

    mHcitData.pPacket = NULL;
    mPacketDetectStep = mDetectMarker_c;
}

static void Hcit_RxCallBack(void *pData)
{
    uint8_t         recvChar;
    uint16_t        count = 0U;
    serialStatus_t temp = Serial_GetByteFromRxBuffer( mHcitSerMgrIf, &recvChar, &count);
    if(temp != gSerial_Success_c)
    {
        return;
    }

    while( count != 0U )
    {
        switch( mPacketDetectStep )
        {
            case mDetectMarker_c:
                if( (recvChar == (uint8_t)gHciDataPacket_c) ||
                    (recvChar == (uint8_t)gHciEventPacket_c) ||
                    (recvChar == (uint8_t)gHciCommandPacket_c) )
                {
                    union
                    {
                        hcitPacketHdr_t *pPacketHdr;
                        hcitPacket_t    *pPacket;
                    } packetTemp; /* MISRA rule 11.3 */

                    packetTemp.pPacketHdr = &mHcitData.pktHeader;
                    mHcitData.pPacket = packetTemp.pPacket;

                    mHcitData.pktHeader.packetTypeMarker = (hciPacketType_t)recvChar;
                    mHcitData.bytesReceived = 1;

                    mPacketDetectStep = mDetectHeader_c;
                }
                break;

            case mDetectHeader_c:
                mHcitData.pPacket->raw[mHcitData.bytesReceived++] = recvChar;

                switch( mHcitData.pktHeader.packetTypeMarker )
                {
                    case gHciDataPacket_c:
                        /* ACL Data Packet */
                        if( mHcitData.bytesReceived == (gHciAclDataPacketHeaderLength_c + 1U) )
                        {
                            /* Validate ACL Data packet length */
                            if( mHcitData.pktHeader.aclDataPacket.dataTotalLength > gHcLeAclDataPacketLengthDefault_c )
                            {
                                mHcitData.pPacket = NULL;
                                mPacketDetectStep = mDetectMarker_c;
                                break;
                            }
                            mHcitData.expectedLength = gHciAclDataPacketHeaderLength_c +
                                                       mHcitData.pktHeader.aclDataPacket.dataTotalLength;

                            mPacketDetectStep = mPacketInProgress_c;
                        }
                        break;

                    case gHciEventPacket_c:
                        /* HCI Event Packet */
                        if( mHcitData.bytesReceived == (gHciEventPacketHeaderLength_c + 1U) )
                        {
                            /* Validate HCI Event packet length
                            if( mHcitData.pktHeader.eventPacket.dataTotalLength > gHcEventPacketLengthDefault_c )
                            {
                                mHcitData.pPacket = NULL;
                                mPacketDetectStep = mDetectMarker_c;
                                break;
                            } */
                            mHcitData.expectedLength = gHciEventPacketHeaderLength_c +
                                                       (uint16_t)mHcitData.pktHeader.eventPacket.dataTotalLength;
                            mPacketDetectStep = mPacketInProgress_c;
                        }
                        break;

                    case gHciCommandPacket_c:
                        /* HCI Command Packet */
                        if( mHcitData.bytesReceived == (gHciCommandPacketHeaderLength_c + 1U) )
                        {

                            mHcitData.expectedLength = gHciCommandPacketHeaderLength_c +
                                                       (uint16_t)mHcitData.pktHeader.commandPacket.parameterTotalLength;
                            mPacketDetectStep = mPacketInProgress_c;
                        }
                        break;
                    case gHciSynchronousDataPacket_c:
                    default:
                        ; /* Not Supported */
                        break;
                }

                if( mPacketDetectStep == mPacketInProgress_c )
                {
                    mHcitData.pPacket = &mHcitPacketRaw;
                    FLib_MemCpy(mHcitData.pPacket, (uint8_t*)&mHcitData.pktHeader + 1, sizeof(hcitPacketHdr_t) - 1U);
                    mHcitData.bytesReceived -= 1U;

                    if( mHcitData.bytesReceived == mHcitData.expectedLength )
                    {
                        Hcit_SendMessage();
                    }
                }
                break;

            case mPacketInProgress_c:
                mHcitData.pPacket->raw[mHcitData.bytesReceived++] = recvChar;

                if( mHcitData.bytesReceived == mHcitData.expectedLength )
                {
                    Hcit_SendMessage();
                }
                break;

            default:
                ; /* No action required */
                break;
        }

        if( Serial_GetByteFromRxBuffer( mHcitSerMgrIf, &recvChar, &count) != gSerial_Success_c )
        {
            return;
        }
    }
}
#endif  /* gSerialManagerMaxInterfaces_c */

static void Hcit_FreePacket
(
    void *pPacket
)
{
    (void)MEM_BufferFree(pPacket);
}

/*! *********************************************************************************
* @}
********************************************************************************** */
