/*! *********************************************************************************
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * \file
 *
 * This source file contains the code that enables the OTA Programming protocol
 * specific to the OTA Server
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#include "FsciInterface.h"
#include "FsciCommands.h"
#include "FunctionLib.h"
#include "fsl_component_mem_manager.h"

#include "OtaSupport.h"

#if 0

typedef struct {
    /*! The FSCI interface used to download an image */
    uint8_t   OtaFsciInterface;

    /*! The size of the image to be downloaded */
    uint32_t  TotalUpdateSize;

    /*! Contains Application Callbacks for packets received over the serial interface */
    otaServer_AppCB_t *pOTA_AppCB;

    bool AllowUpgradeOnCurrentDevice;
    uint8_t NextPushChunkSeq;        /* Used only if AllowUpgradeOnCurrentDevice is set */

    otaMode_t UpgradeMode;

} OtaServerStateCtx_t;


static bool_t OtaSupportCallback( clientPacket_t* pData);


static OtaServerStateCtx_t mServerHdl = {
    .OtaFsciInterface = 0,
    .TotalUpdateSize = 0,
    /*! Contains Application Callbacks for packets received over the serial interface */
    .pOTA_AppCB = NULL,
    .AllowUpgradeOnCurrentDevice = FALSE,
    .UpgradeMode = gUpgradeImageOnCurrentDevice_c,
    .pfFSCI_OtaSupportCallback = NULL,
    

};



/*! *********************************************************************************
* \brief  Retrieve Upgrade mode 
*
* \return  UpgradeMode set in previous FSCI command.
*
********************************************************************************** */
otaMode_t OTA_GetUpgradeMode(void)
{
    return mServerHdl.UpgradeMode;
}

/*! *********************************************************************************
* \brief  Cancel Image at Server's side
*
********************************************************************************** */
void OTA_ServerCancelImage(void)
{
    mServerHdl.TotalUpdateSize = 0;
}

/*! *********************************************************************************
* \brief  Set parameter allowing updagrade on current device
*
* \param[in]  allow value to set
*
********************************************************************************** */
void OTA_AllowUpgradeOnCurrentDevice(bool allow)
{
    mServerHdl.AllowUpgradeOnCurrentDevice  = allow;
}


/*! *********************************************************************************
* \brief  Registers the OTA component to FSCI
*
* \param[in] fsciInterface  The Id of the FSCI interface used by the Otap Server
* \param[in] pCB            Pointer to a table of callback function.
*
* \return  error code.
*
********************************************************************************** */
otaResult_t OTA_RegisterToFsci( uint32_t fsciInterface, otaServer_AppCB_t *pCB)
{
#if 0
    pfFSCI_OtaSupportCallback = OtaSupportCallback;
    // pfFSCI_OtaSupportCallback declared nowhere ??
    // When was it last used
#endif
    mServerHdl.OtaFsciInterface = (uint8_t)fsciInterface;
    mServerHdl.pOTA_AppCB = pCB;
    return gOtaSuccess_c;
}

/*! *********************************************************************************
* \brief  This function handles the FSCI messages for the OTA Server
*
* \param[in] pData    pointer to the received FSCI packet
*
* \return  TRUE if the received message should be recycled.
*
********************************************************************************** */
static bool_t OtaSupportCallback( clientPacket_t* pData )
{
    otaResult_t status = gOtaInvalidOperation_c;

    switch( pData->structured.header.opCode )
    {
    case mFsciOtaSupportStartImageReq_c:
        if( mServerHdl.TotalUpdateSize )
        {
            break;
        }

        FLib_MemCpy(&mServerHdl.TotalUpdateSize, pData->structured.payload, sizeof(mServerHdl.TotalUpdateSize));
        pData->structured.header.len = 3*sizeof(uint8_t);
        pData->structured.payload[0] = gOtaSuccess_c;
        pData->structured.payload[1] = gOtaVersion_c;
        /* Check if image storage is available */
        pData->structured.payload[2] = ( mServerHdl.TotalUpdateSize <= OTA_GetSelectedFlashAvailableSpace() ) ? TRUE : FALSE;

#if gOtaUpgradeImageOnCurrentDevice_d
        mServerHdl.NextPushChunkSeq = 0;
#endif
        return TRUE;

    case mFsciOtaSupportSetModeReq_c:
        mServerHdl.UpgradeMode = (otaMode_t)pData->structured.payload[0];

        if( mServerHdl.UpgradeMode > gDoNotUseExternalMemoryForOtaUpdate_c )
        {
            status = gOtaInvalidParam_c;
        }
        else if( mServerHdl.UpgradeMode == gUseExternalMemoryForOtaUpdate_c )
        {
            otaResult_t st = OTA_SelectExternalStoragePartition();
            if (st != gOtaSuccess_c)
            {
                status = gOtaInvalidOperation_c;
            }
        }
        else if( mServerHdl.AllowUpgradeOnCurrentDevice && 
                 mServerHdl.UpgradeMode == gUpgradeImageOnCurrentDevice_c )
        {
            status = OTA_StartImage(mServerHdl.TotalUpdateSize);
        }
        else if( (mServerHdl.pOTA_AppCB != NULL)&& (mServerHdl.pOTA_AppCB->otaServerSetModeCnf != NULL))
        {
            status = mServerHdl.pOTA_AppCB->otaServerSetModeCnf(pData->structured.payload, pData->structured.header.len);
        }
        break;

    case mFsciOtaSupportPushImageChunkReq_c:
        if( !mServerHdl.TotalUpdateSize )
        {
            status = gOtaInvalidOperation_c;
        }
        else if( mServerHdl.AllowUpgradeOnCurrentDevice && 
                 mServerHdl.UpgradeMode == gUpgradeImageOnCurrentDevice_c )
        {
            if( mServerHdl.NextPushChunkSeq == pData->structured.payload[0] )
            {
                status = OTA_PushImageChunk(&pData->structured.payload[1], pData->structured.header.len-1, NULL, NULL);
                if( gOtaSuccess_c == status )
                {
                    mServerHdl.NextPushChunkSeq++;
                }
            }
        }
        else if( (mServerHdl.pOTA_AppCB != NULL) && (mServerHdl.pOTA_AppCB->otaServerPushChunkCnf != NULL))
        {
            status = mServerHdl.pOTA_AppCB->otaServerPushChunkCnf(pData->structured.payload, pData->structured.header.len);
        }
        break;

    case mFsciOtaSupportCommitImageReq_c:
        if (mServerHdl.AllowUpgradeOnCurrentDevice)
        {
            mServerHdl.TotalUpdateSize = 0;
            if( mServerHdl.UpgradeMode == gUpgradeImageOnCurrentDevice_c )
            {
                status = OTA_CommitImage(pData->structured.payload);
                OTA_SetNewImageFlag();
            }
        }
        break;

    case mFsciOtaSupportCancelImageReq_c:
        if( (mServerHdl.pOTA_AppCB != NULL) && (mServerHdl.pOTA_AppCB->otaServerCancelImgCnf != NULL))
        {
            status = mServerHdl.pOTA_AppCB->otaServerCancelImgCnf(pData->structured.payload, pData->structured.header.len);
        }
        OTA_CancelImage();
        break;

    case mFsciOtaSupportQueryImageRsp_c:
        if( (mServerHdl.pOTA_AppCB != NULL) && (mServerHdl.pOTA_AppCB->otaServerQueryImageCnf != NULL))
        {
            status = mServerHdl.pOTA_AppCB->otaServerQueryImageCnf(pData->structured.payload, pData->structured.header.len);
        }
        break;

    case mFsciOtaSupportImageNotifyReq_c:
        if( (mServerHdl.pOTA_AppCB != NULL) && (mServerHdl.pOTA_AppCB->otaServerImgNotifyCnf != NULL))
        {
            status = mServerHdl.pOTA_AppCB->otaServerImgNotifyCnf(pData->structured.payload, pData->structured.header.len);
        }
        break;
    case mFsciOtaSupportSetFileVerPoliciesReq_c:
        if( (mServerHdl.pOTA_AppCB != NULL) && (mServerHdl.pOTA_AppCB->otaServerSetFileVersPoliciesCnf != NULL))
        {
            status = mServerHdl.pOTA_AppCB->otaServerSetFileVersPoliciesCnf(pData->structured.payload, pData->structured.header.len);
        }
        break;
    case mFsciOtaSupportAbortOTAUpgradeReq_c:
        if( (mServerHdl.pOTA_AppCB != NULL) && (mServerHdl.pOTA_AppCB->otaServerAbortProcessCnf != NULL))
        {
            status = mServerHdl.pOTA_AppCB->otaServerAbortProcessCnf(pData->structured.payload, pData->structured.header.len);
        }
        break;
    case mFsciOtaSupportGetClientInfo_c:
         if( (mServerHdl.pOTA_AppCB != NULL) && (mServerHdl.pOTA_AppCB->otaServerClientInfoCnf != NULL))
         {
            status = mServerHdl.pOTA_AppCB->otaServerClientInfoCnf(pData->structured.payload, pData->structured.header.len);
         }
        break;
    }

    /* Default response */
    pData->structured.header.len = sizeof(uint8_t);
    pData->structured.payload[0] = status;
    return TRUE;
}



void OTA_QueryImageReq(uint16_t devId, uint16_t manufacturer, uint16_t imgType, uint32_t fileVersion)
{
    uint8_t idx;
    clientPacket_t *pPkt;

    /* compute payload len */
    idx = sizeof(devId) + sizeof(manufacturer) + sizeof(imgType) + sizeof(fileVersion);
    pPkt = MEM_BufferAlloc(sizeof(clientPacketHdr_t) + idx + 2);

    if( NULL == pPkt )
    {
        return;
    }

    pPkt->structured.header.opGroup = gFSCI_ReqOpcodeGroup_c;
    pPkt->structured.header.opCode = mFsciOtaSupportQueryImageReq_c;
    pPkt->structured.header.len = idx;

    /* Copy data into the payload buffer */
    idx = 0;
    FLib_MemCpy(&pPkt->structured.payload[idx], &devId, sizeof(devId));
    idx +=sizeof(devId);
    FLib_MemCpy(&pPkt->structured.payload[idx], &manufacturer, sizeof(manufacturer));
    idx +=sizeof(manufacturer);
    FLib_MemCpy(&pPkt->structured.payload[idx], &imgType, sizeof(imgType));
    idx +=sizeof(imgType);
    FLib_MemCpy(&pPkt->structured.payload[idx], &fileVersion, sizeof(fileVersion));

    FSCI_transmitFormatedPacket(pPkt, mServerHdl.OtaFsciInterface);
}


void OTA_ImageChunkReq(uint32_t offset, uint16_t len, uint16_t devId)
{
    uint8_t idx;
    clientPacket_t *pPkt;

    idx = sizeof(offset) + sizeof(len) + sizeof(devId);
    pPkt = MEM_BufferAlloc(sizeof(clientPacketHdr_t) + idx + 2);

    if( NULL == pPkt )
    {
        return;
    }

    pPkt->structured.header.opGroup = gFSCI_ReqOpcodeGroup_c;
    pPkt->structured.header.opCode = mFsciOtaSupportImageChunkReq_c;
    pPkt->structured.header.len = idx;

    /* Copy data into the payload buffer */
    idx = 0;
    FLib_MemCpy(&pPkt->structured.payload[idx], &devId, sizeof(devId));
    idx +=sizeof(devId);
    FLib_MemCpy(&pPkt->structured.payload[idx], &offset, sizeof(offset));
    idx +=sizeof(offset);
    FLib_MemCpy(&pPkt->structured.payload[idx], &len, sizeof(len));

    FSCI_transmitFormatedPacket(pPkt, mServerHdl.OtaFsciInterface);
}


void OTA_ClientInfoCnf(uint8_t* pClientAddr, uint32_t offset, uint16_t devId)
{
    clientPacket_t *pPkt;
    uint8_t idx;

    if (pClientAddr)
    {
      pPkt = MEM_BufferAlloc(sizeof(clientPacketHdr_t) + 16 + sizeof(uint16_t) + sizeof(uint32_t) + 2);

      if( NULL == pPkt )
      {
          return;
      }

      pPkt->structured.header.opGroup = gFSCI_CnfOpcodeGroup_c;
      pPkt->structured.header.opCode = mFsciOtaSupportGetClientInfo_c;
      pPkt->structured.header.len = 16 + sizeof(uint32_t) + sizeof(uint16_t);

      /* Copy data into the payload buffer */
      idx = 0;
      FLib_MemCpy(&pPkt->structured.payload[idx], pClientAddr, 16);
      idx += 16;
      FLib_MemCpy(&pPkt->structured.payload[idx], &offset, sizeof(uint32_t));
      idx += sizeof(uint32_t);
      FLib_MemCpy(&pPkt->structured.payload[idx], &devId, sizeof(devId));

      FSCI_transmitFormatedPacket(pPkt, mServerHdl.OtaFsciInterface);
    }
}

#endif
