/*
* The Clear BSD License
* Copyright 2012-2017 NXP
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

/*!
\file       ota_private.h
\brief      Header for Over the air upgrade cluster
*/

#ifndef OTA_H_PRIVATE
#define OTA_H_PRIVATE

#if defined __cplusplus
extern "C" {
#endif

#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"
#include "OTA.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/


#ifdef OTA_PC_BUILD
#define OTA_FLS_VALIDITY_OFFSET 105

#ifdef OTA_UNIT_TEST_FRAMEWORK
extern tsOTA_ImageHeader _FlsOtaHeader;
extern uint8 FlsPrivateKey[];
extern uint8 FlsZcCert[];
extern uint8 _FlsLinkKey[];
extern uint16 u16ImageStartSector;
extern  uint32   _flash_start;
#else
#define FlsStart 0
#define _FlsOtaHeader 0
#define FlsPrivateKey 0
#define FlsZcCert 0
#define _FlsLinkKey 0
#define u16ImageStartSector 0
#define _flash_start 0
#endif

#else
#define OTA_FLS_VALIDITY_OFFSET 0
#endif

#ifdef OTA_UNIT_TEST_FRAMEWORk
extern uint32 u32Registers;
#define OTA_EFUSE_BANK1_31_0        (&u32Registers)
#define OTA_EFUSE_BANK1_63_32       (&u32Registers)
#define OTA_EFUSE_BANK1_95_64       (&u32Registers)
#define OTA_EFUSE_BANK1_127_96      (&u32Registers)

#else
#define OTA_EFUSE_BANK1_31_0        (0x02000000 + 0x0070)
#define OTA_EFUSE_BANK1_63_32       (0x02000000 + 0x0074)
#define OTA_EFUSE_BANK1_95_64       (0x02000000 + 0x0078)
#define OTA_EFUSE_BANK1_127_96      (0x02000000 + 0x007C)
#endif
#define OTA_FLS_MAGIC_NUMBER_LENGTH  (uint8)0 
#define OTA_IS_ZCL_UNICAST(ZCL_ADDRESS)   ( (((ZCL_ADDRESS).eAddressMode == ZPS_E_ADDR_MODE_SHORT)&&((ADDRESS).u16DestinationAddress < 0xFFF8)\
    && ((ZCL_ADDRESS).eAddressMode != ZPS_E_ADDR_MODE_GROUP)) ? TRUE : FALSE)

#define OTA_IS_UNICAST(ADDRESS_MODE,ADDRESS)   ( (((ADDRESS_MODE) == E_ZCL_AM_SHORT)&&((ADDRESS) < 0xFFF8)\
    && ((ADDRESS_MODE) != E_ZCL_AM_GROUP)) ? TRUE : FALSE)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef void(*pFuncptr)(void *, tsZCL_EndPointDefinition    *);



/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC teZCL_Status eOtaCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);
PUBLIC teZCL_Status eOtaRegisterTimeServer(void);
PUBLIC void vOtaTimerClickCallback(
                    tsZCL_CallBackEvent         *psCallBackEvent);
PUBLIC teZCL_Status eOtaTimeUpdate(
                    uint8                        u8SourceEndPointId,
                    bool_t                       bIsServer,
                    tsZCL_CallBackEvent         *psCallBackEvent);
PUBLIC void vOtaFlashInitHw (
                    uint8                        u8FlashType,
                    void                        *pvFlashTable);
PUBLIC void vOtaFlashErase(
                    uint8                        u8Sector);
PUBLIC void vOtaFlashWrite(
                    uint32                       u32FlashByteLocation,
                    uint16                       u16Len,
                    uint8                       *pu8Data);
PUBLIC void vOtaFlashRead(
                    uint32                       u32FlashByteLocation,
                    uint16                       u16Len,
                    uint8                       *pu8Data);
PUBLIC teZCL_Status eOtaFindCluster(
                    uint8                        u8SourceEndPointId,
                    tsZCL_EndPointDefinition   **ppsEndPointDefinition,
                    tsZCL_ClusterInstance      **ppsClusterInstance,
                    tsOTA_Common               **ppsOTACustomDataStructure,
                    bool_t                       bIsServer);
PUBLIC  teZCL_Status eOtaSetEventTypeAndGiveCallBack(
                       tsOTA_Common                     *psOTA_Common,
                       teOTA_UpgradeClusterEvents        eEventType,
                       tsZCL_EndPointDefinition         *psEndPointDefinition);
PUBLIC tsOTA_ImageHeader sOtaGetHeader(
                    uint8                       *pu8HeaderBytes);
PUBLIC void vOtaUpgManServerCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    void                        *pvStruct,
                    tsZCL_EndPointDefinition    *psEndPointDefinition);
PUBLIC void vOtaFlashLockRead(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsOTA_Common                *psCustomData,
                    uint32                       u32FlashByteLocation,
                    uint16                       u16Len,
                    uint8                       *pu8Data);
PUBLIC void vOtaFlashLockWrite(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsOTA_Common                *psCustomData,
                    uint32                       u32FlashByteLocation,
                    uint16                       u16Len,
                    uint8                       *pu8Data);
PUBLIC  void vOtaFlashLockErase(
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsOTA_Common                *psCustomData,
                    uint8                        u8Sector);

PUBLIC bool_t bOtaIsImageValid(uint8_t * pu8Data);
PUBLIC void vOtaSwitchLoads(void);
PUBLIC uint32 eOTA_OtaOffset(void);
PUBLIC tsReg128 eOTA_retOtaUseKey(void);

#ifdef OTA_CLIENT
PUBLIC void vOtaInitStateMachine(
                    tsOTA_Common                *psCustomData);
PUBLIC void vOtaClientUpgMgrMapStates(
                    teOTA_ImageUpgradeStatus     eStatus,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsOTA_Common                *psCustomData);
PUBLIC teZCL_Status eOtaReceivedImageNotify(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset);
PUBLIC teZCL_Status eOtaReceivedQueryNextImageResponse(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset);
PUBLIC teZCL_Status eOtaReceivedBlockResponse(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16 u16Offset);
PUBLIC teZCL_Status eOtaReceivedUpgradeEndResponse(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset);
PUBLIC teZCL_Status eOtaQuerySpecificFileResponse(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset);
PUBLIC teZCL_Status eOtaClientReqSeqNoUpdate(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8RequestTransSeqNo);

PUBLIC void vOtaReceivedDefaultResponse(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);


#endif
#ifdef OTA_SERVER
PUBLIC void vOtaServerUpgManagerInit(pFunc *pFuncPtr);
PUBLIC teZCL_Status eOtaReceivedQueryNextImage(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset);
PUBLIC teZCL_Status eOtaReceivedBlockRequest(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset);

PUBLIC teZCL_Status eOtaReceivedUpgradeEndRequest(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset);

PUBLIC teZCL_Status eOtaReceivedUpgradeEndRequest(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset);
PUBLIC teZCL_Status eOtaReceivedQuerySpecificFileRequest(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset);
PUBLIC teZCL_Status eOtaReceivedPageRequest(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset);
#ifdef OTA_PAGE_REQUEST_SUPPORT
PUBLIC void vOtaHandleTimedPageRequest(uint8 u8SourceEndPointId);
#endif
PUBLIC teZCL_Status eOTA_GetOtaHeader(
                    uint8                        u8Endpoint,
                    uint8                        u8ImageIndex,
                    tsOTA_ImageHeader           *psOTAHeader);
#endif
PUBLIC void vOTA_GetTagIdandLengh(uint16 *pu16TagId,uint32 *pu32TagLength,uint8 * pu8Tag);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/




#ifdef OTA_CLIENT
extern const tsZCL_AttributeDefinition asOTAClusterAttributeDefinitions[];
extern uint8 au8OtaHeader[OTA_MAX_HEADER_SIZE] ;
#if (OTA_MAX_CO_PROCESSOR_IMAGES != 0)
extern uint8 asCommonCoProcessorOTAHeader[OTA_MAX_CO_PROCESSOR_IMAGES][OTA_MAX_HEADER_SIZE] ;
extern bool_t bIsCoProcessorImgUpgdDependent;
#endif
#endif


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* OTA_H_PRIVATE */
