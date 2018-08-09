/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* FSCI Bootloader Source file
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

#ifdef __cplusplus
    extern "C" {
#endif

#include "EmbeddedTypes.h"
#include "FsciBootloader.h"

#if gFsciIncluded_c
#include "main.h"
#include "SPI.h"
#include "UART.h"
#include "flash_boot_kinetis.h"
#if gFsciRxAck_c
#include "timer.h"
#endif

/******************************************************************************
 *******************************************************************************
 * Public macros
 *******************************************************************************
 ******************************************************************************/


/******************************************************************************
 *******************************************************************************
 * Private type definitions
 *******************************************************************************
 ******************************************************************************/
typedef enum {
    mFsciNone_c,
    mFsciUart_c,
    mFsciSpi_c
}fsciInterfaceType_t;

typedef enum {
  gFsciBootSucess_c = 0,
  gFsciBootNoImage_c,
  gFsciBootUpdated_c,
  gFsciBootError_c,
  gFsciBootCrcError_c,
  gFsciBootInvalidParam_c,
  gFsciBootInvalidOperation_c,
  gFsciBootExternalFlashError_c,
  gFsciBootInternalFlashError_c,
} fsciBootResult_t;


/******************************************************************************
 *******************************************************************************
 * Private memory declarations
 *******************************************************************************
 ******************************************************************************/
static fsciInterfaceType_t mFsciInterface;
static fsciLen_t mFsciBytes;
static clientPacket_t gFsciPacket;

static uint32_t mTotalUpdateSize;
static uint32_t mFlashAddr;
static uint32_t mMaxUpgradeSize;
static uint8_t * mpFlashWriteBuffer;
static uint32_t mFlashBufferOffset;
#if gFsciUseCRC_c
static uint16_t computedCRC;
#endif
#if gFsciBootUseSeqNo_c
static uint8_t  mNextPushChunkSeq;
#endif

/* FSCI Ack message */
#if gFsciTxAck_c
static gFsciAckMsg_t mFsciAckMsg;
#endif
#if gFsciRxAck_c
    uint8_t            txRetryCnt;
    volatile bool_t    ackReceived;
    volatile bool_t    ackWaitOngoing;
#endif

#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = ".BOOT_MODULE_INFO"
__root const moduleInfo_t mBootloaderInfo =
#elif defined(__GNUC__)
const moduleInfo_t mBootloaderInfo __attribute__ ((section(".BOOT_MODULE_INFO"), used)) =
#endif
{
  {'B', 'O', 'O', 'T'},
  gBootloaderVerString_c,
  gBootloaderModuleId_c,
  {gBootloaderVerMajor_c, gBootloaderVerMinor_c, gBootloaderVerPatch_c},
  gBootloaderBuildNo_c
};

/******************************************************************************
 *******************************************************************************
 * Private function prototypes
 *******************************************************************************
 ******************************************************************************/
static fsci_packetStatus_t FSCI_checkPacket( clientPacket_t *pData, uint16_t bytes );
static uint8_t FSCI_computeChecksum( uint8_t *pBuffer, uint16_t size );
static void FSCI_BOOT_ProcessRxPkt (clientPacket_t* pPacket);
static void FSCI_SendPacketToSerial(uint8_t *pData, uint16_t size);
static uint8_t FSCI_GetDataFromSerial(uint8_t *pData);
static uint8_t FSCI_BOOT_WriteBufferToFlash(void);
static void FSCI_ReceivePacket();
#if gFsciUseCRC_c
static void CrcAddByte(uint8_t byte);
#endif
#if gFsciTxAck_c
void FSCI_Ack(uint8_t checksum);
#endif


/******************************************************************************
 *******************************************************************************
 * Public function
 *******************************************************************************
 ******************************************************************************/

/*! *********************************************************************************
* \brief  Main loop of the FSCI Bootloader.
*
********************************************************************************** */
void FSCI_BOOT_main(void)
{
#ifdef FLASH_PROGRAM_SECTION_SUPPORT
    mpFlashWriteBuffer = (uint8_t*)(0x14000000);
#else
    uint8_t flashWriteBuffer[gFlashErasePage_c];
    mpFlashWriteBuffer = flashWriteBuffer;
#endif
    mMaxUpgradeSize = gMcuFlashSize_c / 32 * 31;

    /* FSCI data initialization */
    mFsciInterface = mFsciNone_c;
    mFsciBytes = 0;
    mTotalUpdateSize = 0;

#if gFsciTxAck_c
    mFsciAckMsg.header.startMarker = gFSCI_StartMarker_c;
    mFsciAckMsg.header.opGroup = gFSCI_CnfOpcodeGroup_c;
    mFsciAckMsg.header.opCode = mFsciMsgAck_c;
    mFsciAckMsg.header.len = sizeof(uint8_t);
#endif

    EnableRamVectorTable();
    /* Init the flash module */
    FlashInitialization();
    /* Init serial interfaces */
    uart_init(FSCI_BOOT_SerialRxCallback, mFsciUart_c);
    spi_slave_init(FSCI_BOOT_SerialRxCallback, mFsciSpi_c);

#if gFsciRxAck_c
    txRetryCnt = mFsciTxRetryCnt_c;
    ackReceived = FALSE;
    ackWaitOngoing = FALSE;
#endif

    /* Main loop - Process received data */
    while(1)
    {
        FSCI_ReceivePacket();

    } /* while(1) */
}

/*! *********************************************************************************
* \brief  Polls the serial interfaces and if a packet is found, it is processed
*
********************************************************************************** */
void FSCI_ReceivePacket()
{
    uint8_t c;
    fsci_packetStatus_t status;

    if( FSCI_GetDataFromSerial(&c) )
    {
        /* Wait for start marker */
        if( 0 == mFsciBytes )
        {
            if( c == gFSCI_StartMarker_c )
            {
                gFsciPacket.raw[mFsciBytes++] = c;
            }
        }
        /* FSCI packet reception is in progress */
        else
        {
            gFsciPacket.raw[mFsciBytes++] = c;

            status = FSCI_checkPacket(&gFsciPacket, mFsciBytes);

            switch(status)
            {
                case PACKET_IS_TO_SHORT:
                /* wait for more data to arrive */
                break;
                case PACKET_IS_VALID :
                mFsciBytes = 0;
                #if gFsciRxAck_c
                /* Check for ACK packet */
                if(( gFSCI_CnfOpcodeGroup_c == gFsciPacket.structured.header.opGroup ) &&
                   ( mFsciMsgAck_c == gFsciPacket.structured.header.opCode ))
                {
                    if( ackWaitOngoing )
                    {
                        ackReceived = TRUE;
                    }
                }
                /* if waiting for ack don't process normal messages */
                else if (!ackWaitOngoing)
                #endif
                {
                    FSCI_BOOT_ProcessRxPkt(&gFsciPacket);
                }
                return;
                case FRAMING_ERROR:
                default:
                mFsciBytes = 0;
                return;
            }
        }
    }
}

/*! *********************************************************************************
* \brief  Signals that data has been received over the serialinterface.
*         The function will set the interface type used for the boot process
*
* \param[in] param interface type on thich data has been received
*
********************************************************************************** */
void FSCI_BOOT_SerialRxCallback(uint32_t param)
{
    if( mFsciInterface == mFsciNone_c )
    {
        extern void (*mSpiCallback)(uint32_t);

        mSpiCallback = NULL;
        mFsciInterface = (fsciInterfaceType_t)param;
    }
}


/*! *********************************************************************************
* \brief  Send packet over the serial interface, after computing Checksum.
*
* \param[in] pPacket pointer to the packet to be sent over the serial interface
* \param[in] fsciInterface the interface on which the packet should be sent
*
********************************************************************************** */
void FSCI_BOOT_transmitFormatedPacket( clientPacket_t *pPacket )
{
    uint32_t        size;
    uint8_t         checksum;

    pPacket->structured.header.startMarker = gFSCI_StartMarker_c;
    size = sizeof(clientPacketHdr_t) + pPacket->structured.header.len + 1 /* CRC */;

    /* Compute Checksum */
    checksum = FSCI_computeChecksum( pPacket->raw+1, size - 2);
    pPacket->structured.payload[pPacket->structured.header.len] = checksum;

#if gFsciRxAck_c

    ackReceived = FALSE;
    txRetryCnt = mFsciTxRetryCnt_c;

    while( txRetryCnt )
    {
        /* send message to the serial interface */
        FSCI_SendPacketToSerial(pPacket->raw, size);

        ackWaitOngoing = TRUE;
        StartTimer();

        while( !ackReceived )
        {
            FSCI_ReceivePacket();
            if( GetElapsedTime()  > mFsciRxAckTimeoutMs_c )
            {
                ackWaitOngoing = FALSE;
                if( txRetryCnt )
                {
                    txRetryCnt--;
                }
                break; /* Timeout expired */
            }
        }
        StopTimer();

        if( ackReceived )
        {
            ackWaitOngoing = FALSE;
            break; /* Success */
        }
    }

#else
    /* send message to the serial interface */
    FSCI_SendPacketToSerial(pPacket->raw, size);
#endif
}


/******************************************************************************
 *******************************************************************************
 * Private function
 *******************************************************************************
 ******************************************************************************/

/*! *********************************************************************************
* \brief  This function will handle the received FSCI packets
*
* \param[in]  pPacket  pointer to the received packet
*
********************************************************************************** */
static void FSCI_BOOT_ProcessRxPkt (clientPacket_t* pPacket)
{
    fsciLen_t len = 1;
#if gFsciTxAck_c
    if(( gFSCI_CnfOpcodeGroup_c != pPacket->structured.header.opGroup ) &&
       ( mFsciMsgAck_c != pPacket->structured.header.opCode ))
    {
        /* Do not cascade ACK messages */
        FSCI_Ack(0);
    }
#endif

    /* Process only FSCI requests */
    if( pPacket->structured.header.opGroup == gFSCI_ReqOpcodeGroup_c )
    {
        switch(pPacket->structured.header.opCode)
        {
        case mFsciMsgResetCPUReq_c:
            Boot_ResetMCU();
            break;

#if 0 /* debug purpose only */
        case mFsciLowLevelPing_c:
            /* Nothing to do here */
            len = pPacket->structured.header.len;
            break;
#endif
            /* Start image upgrade process */
        case mFsciBootStartImageReq_c:
            len = 3*sizeof(uint8_t);

            if( !mTotalUpdateSize )
            {
                if(mTotalUpdateSize <= mMaxUpgradeSize)
                {
#if gFsciBootUseSeqNo_c
                    mNextPushChunkSeq = 0;
#endif
                    mFlashBufferOffset = 0;
#if gFsciUseCRC_c
                    computedCRC = 0;
#endif
                    mFlashAddr = gMcuFlashSize_c / 32;
                    Boot_MemCpy(&mTotalUpdateSize, pPacket->structured.payload, sizeof(mTotalUpdateSize));
                    pPacket->structured.payload[0] = gFsciBootSucess_c;

                    if( gBootFlags.newImageFlag == gBootFlagInvalid_d )
                    {
                        /* Mark that the upgrade process is in progress */
                        uint64_t flag = 0;
                        FLASH_Boot_Program((uint32_t)&gBootFlags.newImageFlag, (uint32_t)&flag, sizeof(flag));
                    }
                }
                else
                {
                    /* Not enough Flash memory to store the new image */
                    pPacket->structured.payload[0] = gFsciBootInvalidParam_c;
                }
            }
            else
            {
                /* Upgrade allready in progress */
                pPacket->structured.payload[0] = gFsciBootInvalidOperation_c;
            }

            pPacket->structured.payload[1] = 1; /* Protocol version */
            pPacket->structured.payload[2] = TRUE; /* Use storage? -> Yes */
            break;

            /* Cancel image upgrade process */
        case mFsciBootCancelImageReq_c:
        case mFsciBootAbortUpgradeReq_c:
            mTotalUpdateSize = 0;
            pPacket->structured.payload[0] = FLASH_Boot_EraseSector((uint32_t)__BOOT_Flags_start__);
            break;

            /* Send dummy confirm */
        case mFsciBootSetModeReq_c:
        case mFsciBootImageNotifyReq_c:
        case mFsciBootSetFileVerPoliciesReq_c:
            pPacket->structured.payload[0] = gFsciBootSucess_c;
            break;

            /* Ignore this commands */
        case mFsciBootQueryImageRsp_c:
            return;

            /* Store the received Image Chunk */
        case mFsciBootPushImageChunkReq_c:
            if( !mTotalUpdateSize )
            {
                /* Use StartImage command before sending Image chuncks */
                pPacket->structured.payload[0] = gFsciBootInvalidOperation_c;
            }
            else
            {
#if gFsciBootUseSeqNo_c
                if( mNextPushChunkSeq == pPacket->structured.payload[0] )
#endif
                {
#if gFsciBootUseSeqNo_c
                    uint8_t * pPayload = &pPacket->structured.payload[1];
                    uint16_t  payloadLen = pPacket->structured.header.len-1;
#else
                    uint8_t * pPayload = &pPacket->structured.payload;
                    uint16_t  payloadLen = pPacket->structured.header.len;
#endif
                    uint16_t  len = payloadLen;
                    uint8_t   status = FLASH_OK;

                    /* Store data in a temporary buffer */
                    if( len > (gFlashErasePage_c - mFlashBufferOffset) )
                    {
                        len = gFlashErasePage_c - mFlashBufferOffset;
                    }

                    Boot_MemCpy(mpFlashWriteBuffer + mFlashBufferOffset , pPayload, len);
                    mFlashBufferOffset += len;

                    /* Buffer Full, write to Flash */
                    if( gFlashErasePage_c == mFlashBufferOffset )
                    {
                        status = FSCI_BOOT_WriteBufferToFlash();
                    }

                    /* Store remaining bytes */
                    if( len < payloadLen )
                    {
                        Boot_MemCpy(mpFlashWriteBuffer + mFlashBufferOffset, &pPayload[len], payloadLen - len);
                        mFlashBufferOffset += payloadLen - len;
                    }

                    /* Set return status */
                    if(FLASH_OK == status)
                    {
                        pPacket->structured.payload[0] = gFsciBootSucess_c;
#if gFsciBootUseSeqNo_c
                        mNextPushChunkSeq++;
#endif
                    }
                    else
                    {
                        /* Flash Erase/Program error! */
                        pPacket->structured.payload[0] = gFsciBootInternalFlashError_c;
                    }
                }
#if gFsciBootUseSeqNo_c
                else
                {
                    /* Invalid sequence number! Data has been lost! */
                    pPacket->structured.payload[0] = gFsciBootError_c;
                }
#endif
            }
            break;

            /* Upgrade process complete */
        case mFsciBootCommitImageReq_c:
            if( mTotalUpdateSize )
            {
                uint8_t status;
                uint8_t *pBitmap;
                uint8_t  bitMask;
#if gFsciUseCRC_c
                uint16_t CRCinput;
#endif
                /* Write remaining data to flash */
                status = FSCI_BOOT_WriteBufferToFlash();
#if gFsciUseCRC_c
                Boot_MemCpy(&CRCinput, pPacket->structured.payload+(pPacket->structured.header.len-sizeof(CRCinput)), sizeof(CRCinput));
                if( CRCinput == computedCRC )
#endif
                {
                    /* Erase remaining sectors if required */
                    if( pPacket->structured.header.len >= gBootData_SectorsBitmap_Size_c )
                    {
                        pBitmap = pPacket->structured.payload;
                        bitMask = gBitMaskInit_c;
                        /* Update value of mFlashAddr to match sector bitmap */
                        if (mFlashAddr & (gFlashErasePage_c - 1))
                        {
                            mFlashAddr = mFlashAddr + gFlashErasePage_c - (mFlashAddr & (gFlashErasePage_c-1));
                        }
                        pBitmap = pBitmap + ((mFlashAddr/gFlashErasePage_c) >> 3);
                        bitMask = 1 << ((mFlashAddr/gFlashErasePage_c) & 7);
                        while (mFlashAddr < (gMcuFlashSize_c - gFlashErasePage_c))
                        {
                            if (*pBitmap & bitMask)
                            {
                                status = FLASH_Boot_EraseSector(mFlashAddr);
                                if( FLASH_OK != status )
                                {
                                    break;
                                }
                            }

                            mFlashAddr += gFlashErasePage_c;
                            /* Update Bitmask */
                            bitMask <<= 1;
                            if (bitMask == 0)
                            {
                                /* This was last bit in the current bitmap byte. Move to next bitmap byte */
                                bitMask = gBitMaskInit_c;
                                pBitmap++;
                            }
                        }
                    }

                    /* Mark that the upgrade process has finished */
                    if( FLASH_OK == status )
                    {
                        status = FLASH_Boot_EraseSector((uint32_t)__BOOT_Flags_start__);
                    }

                    if( FLASH_OK == status )
                    {
                        pPacket->structured.payload[0] = gFsciBootSucess_c;
                    }
                    else
                    {
                        pPacket->structured.payload[0] = gFsciBootInternalFlashError_c;
                    }
                }
#if gFsciUseCRC_c
                else
                {
                    pPacket->structured.payload[0] = gFsciBootCrcError_c;
                }
#endif
            }
            else
            {
                pPacket->structured.payload[0] = gFsciBootInvalidOperation_c;
            }
            break;

        default:
            /* Send FSCI-ERROR */
            pPacket->structured.header.opCode = mFsciMsgError_c;
            pPacket->structured.payload[0] = gFsciUnknownOpcode_c;
            break;
        }
    }
    else
    {
        /* Send FSCI-ERROR */
        pPacket->structured.header.opCode = mFsciMsgError_c;
        pPacket->structured.payload[0] = gFsciUnknownOpcodeGroup_c;
    }

    /* send message to the serial interface */
    pPacket->structured.header.opGroup = gFSCI_CnfOpcodeGroup_c;
    pPacket->structured.header.len = len;
    FSCI_BOOT_transmitFormatedPacket(pPacket);
}

/*! *********************************************************************************
* \brief  Checks to see if we have a valid packet
*
* \param[in] pData The message containing the incoming data packet to be handled.
* \param[in] bytes the number of bytes inside the buffer
*
* \return the status of the packet
*
********************************************************************************** */
static fsci_packetStatus_t FSCI_checkPacket( clientPacket_t *pData, uint16_t bytes )
{
    uint8_t checksum = 0;
    uint16_t len;

    if ( bytes < sizeof(clientPacketHdr_t) )
    {
        return PACKET_IS_TO_SHORT;            /* Too short to be valid. */
    }

    if ( bytes >= sizeof(clientPacket_t) )
    {
        return FRAMING_ERROR;
    }

//    if ( NULL == pData ) // pData will always be &gFsciPacket
//    {
//        return INTERNAL_ERROR;
//    }

    /* The packet's len field does not count the STX, the opcode group, the */
    /* opcode, the len field, or the checksum. */
    len = pData->structured.header.len;

    /* If the length appears to be too long, it might be because the external */
    /* client is sending a packet that is too long, or it might be that we're */
    /* out of sync with the external client. Assume we're out of sync. */
    if ( len > sizeof(pData->structured.payload) )
    {
        return FRAMING_ERROR;
    }

    if ( bytes < len + sizeof(clientPacketHdr_t) + sizeof(checksum) )
    {
        return PACKET_IS_TO_SHORT;
    }

    /* If the length looks right, make sure that the checksum is correct. */
    if( bytes == len + sizeof(clientPacketHdr_t) + sizeof(checksum) )
    {
        checksum = FSCI_computeChecksum(pData->raw+1, len + sizeof(clientPacketHdr_t)-1);
        if( checksum == pData->structured.payload[len] )
        {
            return PACKET_IS_VALID;
        }
    }

    return FRAMING_ERROR;
}

/*! *********************************************************************************
* \brief  This function performs a XOR over the message to compute the CRC
*
* \param[in]  pBuffer - pointer to the messae
* \param[in]  size - the length of the message
*
* \return  the CRC of the message
*
********************************************************************************** */
static uint8_t FSCI_computeChecksum( uint8_t *pBuffer, uint16_t size )
{
    uint16_t index;
    uint8_t  checksum = 0;

    for ( index = 0; index < size; index++ )
    {
        checksum ^= pBuffer[index];
    }

    return checksum;
}

/*! *********************************************************************************
* \brief  This function will tx data over the serial inteface
*
* \param[in]  pData - pointer to the messae
* \param[in]  size - the length of the message
*
********************************************************************************** */
static void FSCI_SendPacketToSerial(uint8_t *pData, uint16_t size)
{
    switch(mFsciInterface)
    {
    case mFsciUart_c:
        uart_sync_write(pData, size);
        break;
    case mFsciSpi_c:
        spi_slave_sync_write(pData, size);
        break;
    default:
        break;
    }
}

/*! *********************************************************************************
* \brief  This function will retrieve data received over the serial inteface
*
* \param[in]  pData - pointer where to store received data
*
* \return  number of bytes read
*
********************************************************************************** */
static uint8_t FSCI_GetDataFromSerial(uint8_t *pData)
{
//    if( pData ) // pData will alwais be valid
    {
        switch(mFsciInterface)
        {
        case mFsciUart_c:
            return uart_read(pData, 1);
            break;
        case mFsciSpi_c:
            return spi_slave_read(pData, 1);
            break;
        default:
            break;
        }
    }

    return 0;
}

/*! *********************************************************************************
* \brief  Write data from the RAM buffer into FLASH
*
* \return  the Flash Erase/Program status
*
********************************************************************************** */
static uint8_t FSCI_BOOT_WriteBufferToFlash(void)
{
    uint8_t status = FLASH_OK;
    #if gFsciUseCRC_c
    uint16_t index;
    #endif

    if( mFlashBufferOffset )
    {
        status = FLASH_Boot_EraseSector(mFlashAddr);

        if(FLASH_OK == status)
        {
#ifdef FLASH_PROGRAM_SECTION_SUPPORT
            status = FLASH_Boot_ProgramPreloadedSection(mFlashAddr, mFlashBufferOffset);
#else
            status = FLASH_Boot_Program(mFlashAddr, (uint32_t)mpFlashWriteBuffer, mFlashBufferOffset);
#endif
            #if gFsciUseCRC_c
            if (FLASH_OK == status)
            {
                for (index=0; index<mFlashBufferOffset; index++)
                {
                    CrcAddByte(((uint8_t*)mFlashAddr)[index]);
                }
            }
            #endif
            mFlashAddr += mFlashBufferOffset;
            mFlashBufferOffset = 0;
        }
    }

    return status;
}

/*! *********************************************************************************
* \brief  Compute CCITT-CRC on byte
*
* \return  -
********************************************************************************** */
#if gFsciUseCRC_c
static void CrcAddByte(uint8_t byte)
{
	uint8_t crcA;
	computedCRC ^= ((uint16_t)byte << 8);
	for(crcA = 8; crcA; crcA--)
	{
		if(computedCRC & 0x8000)
        {
			computedCRC <<= 1;
			computedCRC ^= 0x1021;
		}
		else
		{
			computedCRC <<= 1;
		}
	}
}
#endif

#if gFsciTxAck_c
/*! *********************************************************************************
* \brief  Send an ack message back to the external client.
*
* \param[in] checksum of the packet received
* \param[in] fsciInterface the interface on which the packet was received
*
*
********************************************************************************** */
void FSCI_Ack(uint8_t checksum)
{
    uint8_t size = sizeof(mFsciAckMsg);

    mFsciAckMsg.checksumPacketReceived = checksum;
    mFsciAckMsg.checksum = FSCI_computeChecksum( &mFsciAckMsg.header.opGroup, size-2 );

    FSCI_SendPacketToSerial((uint8_t*)&mFsciAckMsg, size);
}
#endif

#endif /* #if gFsciIncluded_c */

#ifdef __cplusplus
}
#endif
