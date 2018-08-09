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

/**
 * @defgroup g_mac_vs_data MAC layer Service Access Point (SAP)
 */

#ifndef _mac_vs_data_h_
#define _mac_vs_data_h_

/*!
\file       mac_vs_sap.h
\brief      MAC shim layer to provide PDU manager i/f to NWK layer
*/

     
#ifdef __cplusplus
extern "C" {
#endif

/***********************/
/**** INCLUDE FILES ****/
/***********************/

#include "jendefs.h"
#include <pdum_nwk.h>
#include "mac_sap.h"
#ifndef CPU_MKW41Z512VHT4  
#include "mac_pib.h"
#include "AppApi.h"
#include "tsv_pub.h"
#endif
/************************/
/**** MACROS/DEFINES ****/
/************************/

#define MAC_MCPS_VS_TYPE_OFFSET 0x80

/**************************/
/**** TYPE DEFINITIONS ****/
/**************************/

/**********************/
/**** MCPS Request ****/
/**********************/

/**
 * @defgroup g_mac_sap_mcps_req MCPS Request objects
 * @ingroup g_mac_sap_mcps
 *
 * These are passed in a call to MAC_vHandleMcpsReqRsp.
 * Confirms to a Request will either be passed back synchronously on the function return,
 * or a special 'deferred' confirm will come back asynchronously via the
 * Deferred Confirm/Indication callback.
 * Responses have no effective confirmation of sending as they are in response to
 * an Indication; this will be indicated in the synchronous Confirm passed back.
 */

/**
 * @defgroup g_mac_sap_mcps_req_15_4 MCPS Request 802.15.4 specification parameters
 * @ingroup g_mac_sap_mcps_req
 *
 * @{
 */

/**
 * @brief Structure for MCPS-DATA.request
 *
 * Data transmit request. Use type MAC_MCPS_REQ_DATA
 */
typedef struct
{
    MAC_Addr_s  sSrcAddr;    /**< Source address */
    MAC_Addr_s  sDstAddr;    /**< Destination address */
    uint8       u8TxOptions; /**< Transmit options */
    uint8       u8Handle;    /**< MAC handle */
    uint8       u8Free;      /**< Whether NPDU is freed or not */
    PDUM_thNPdu hNPdu;       /**< NPDU handle */
} MAC_tsMcpsVsReqData;

/**
 * @brief Structure for MCPS-PURGE.request
 *
 * Purge request. Use type MAC_MCPS_REQ_PURGE
 */
typedef struct
{
    uint8 u8Handle;    /**< Handle of request to purge from queue */
} MAC_tsMcpsVsReqPurge;

/* @} */

/**********************/
/**** MCPS Confirm ****/
/**********************/

/**
 * @defgroup g_mac_sap_mcps_cfm MCPS Confirm objects
 * @ingroup g_mac_sap_mcps
 *
 * These come back synchronously as a returned parameter in the Request/Response call.
 * They can also be deferred and asynchronously posted via the Deferred Confirm/Indication callback.
 */

/**
 * @defgroup g_mac_sap_mcps_cfm_15_4 MCPS Confirm 802.15.4 specification parameters
 * @ingroup g_mac_sap_mcps_cfm
 *
 * @{
 */

/**
 * @brief Structure for MCPS-DATA.confirm
 *
 * Data transmit confirm. Use type MAC_MCPS_CFM_DATA
 */
typedef struct
{
    uint8 u8Status; /**< Status of request @sa MAC_Enum_e */
    uint8 u8Handle; /**< Handle matching associated request */
} MAC_tsMcpsVsCfmData;

/**
 * @brief Structure for MCPS-PURGE.confirm
 *
 * Data transmit confirm. Use type MAC_MCPS_CFM_PURGE
 */
typedef struct
{
    uint8 u8Status; /**< Status of request @sa MAC_Enum_e */
    uint8 u8Handle; /**< Handle matching associated request */
} MAC_tsMcpsVsCfmPurge;

/* @} */
typedef struct
{
    tsTxFrameFormat sTxFrame;
    TSV_Timer_s sTimer;
    uint8 u8HigherLayerRetryCount;
} tsHigherLayerTxFrame;

/*************************/
/**** MCPS Indication ****/
/*************************/

/**
 * @defgroup g_mac_sap_mcps_ind MCPS Indication Object
 * @ingroup g_mac_sap_mcps
 *
 * These are sent asynchronously via the registered Deferred Confirm/Indication callback
 */

/**
 * @defgroup g_mac_sap_mcps_ind_15_4 MCPS Indication 802.15.4 specification parameters
 * @ingroup g_mac_sap_mcps_ind
 *
 * @{
 */

/**
 * @brief Structure for MCPS-DATA.indication
 *
 * Data received indication. Uses type MAC_MCPS_IND_DATA
 */
typedef struct
{
    MAC_Addr_s  sSrcAddr;       /**< Source address */
    MAC_Addr_s  sDstAddr;       /**< Destination address */
    uint8       u8LinkQuality;  /**< Link quality of received frame */
    uint8       u8SecurityUse;  /**< True if security was used */
    uint8       u8AclEntry;     /**< Security suite used */
    uint8       u8SeqNum;       /**< sequence number */
    uint32      u32ArrivalTime; /**< Arrival time    */
    PDUM_thNPdu hNPdu;          /**< NPDU handle */
    bool_t      bFramePending;  /*Frame pending set*/
} MAC_tsMcpsVsIndData;

/* @} */

/*****************************************/
/**** MCPS Request/Response Interface ****/
/*****************************************/

/**
 * @defgroup g_mac_sap_mcps_req_rsp_if MCPS Request/Response interface
 * @ingroup g_mac_sap_mcps g_mac_VS
 *
 * The interface for the client to issue an MCPS Request or Response
 * is via a function call to MAC_vHandleMcpsReqRsp.
 * @li Request/Response parameters are passed in via psMcpsReqRsp
 * @li Synchronous Confirm parameters are passed out via psMcpsSyncCfm
 * @li Deferred Confirms are posted back asynchronously via the
 *     Deferred Confirm/Indication callback.
 * @note There are currently no MCPS Responses specified
 *
 * @{
 */

/**
 * @brief MAC MCPS Request/Response enumeration.
 *
 * Enumeration of MAC MCPS Request/Response
 * @note Must not exceed 256 entries
 */
typedef enum
{
    MAC_MCPS_VS_REQ_DATA = MAC_MCPS_VS_TYPE_OFFSET,  /**< Use with MAC_tsMcpsVsReqRsp */
    MAC_MCPS_VS_REQ_PURGE,        /**< Use with MAC_tsMcpsVsReqPurge */
} MAC_teMcpsVsReqRspType;

#define NUM_MAC_MCPS_VS_REQ 1

/**
 * @brief MCPS Request/Response Parameter union
 *
 * Union of all the possible MCPS Requests and Responses
 * @note There are no Responses currently specified
 */
typedef union
{
    MAC_tsMcpsVsReqData  sVsReqData;   /**< Data request */
    MAC_tsMcpsVsReqPurge sVsReqPurge;  /**< Purge request */
} MAC_tuMcpsVsReqRspParam;

/**
 * @brief MCPS Request/Response object
 *
 * The object passed to MAC_vHandleMcpsReqRsp containing the request/response
 */
typedef struct
{
    uint8                   u8Type;          /**< Request type (@sa MAC_teMcpsReqRspVsType) */
    uint8                   u8ParamLength;   /**< Parameter length in following union */
    uint16                  u16Pad;          /**< Padding to force alignment */
    MAC_tuMcpsVsReqRspParam uParam;          /**< Union of all possible Requests */
} MAC_tsMcpsVsReqRsp;

/**
 * @brief MCPS Synchronouse Confirm Parameter union
 *
 * Union of all the possible MCPS Synchronous Confirms
 */
typedef union
{
    MAC_tsMcpsVsCfmData  sVsCfmData;
    MAC_tsMcpsVsCfmData  sVsCfmPurge;
} MAC_tuMcpsVsSyncCfmParam;

/**
 * @brief MCPS Synchronous Confirm
 *
 * The object returned by MAC_vHandleMcpsReqRsp containing the synchronous confirm.
 * The confirm type is implied as corresponding to the request
 * @note All Confirms may also be sent asynchronously via the registered Deferred Confirm/Indication callback;
 * this is notified by returning MAC_MCPS_CFM_DEFERRED.
 */
typedef struct
{
    uint8                    u8Status;        /**< Confirm status (@sa MAC_teMcpsSyncCfmVsStatus) */
    uint8                    u8ParamLength;   /**< Parameter length in following union */
    uint16                   u16Pad;          /**< Padding to force alignment */
    MAC_tuMcpsVsSyncCfmParam uParam;          /**< Union of all possible Confirms */
} MAC_tsMcpsVsSyncCfm;

/* @} */

/****************************************************/
/**** MCPS Deferred Confirm/Indication Interface ****/
/****************************************************/

/**
 * @defgroup g_mac_sap_mcps_dcfm_ind_if MCPS Deferred Confirm/Indication Interface
 * @ingroup g_mac_sap_mcps g_mac_VS
 *
 * The interface for the client to receive an MCPS Deferred Confirm or Indication
 * is via a function callback to the function registered using MAC_vMcpsDcfmIndRegister
 *
 * @{
 */

/**
 * @brief Deferred Confirm/Indication type
 *
 * Indicates the type of deferred confirm or indication
 * @note NB Must not exceed 256 entries
 */
typedef enum
{
    MAC_MCPS_VS_DCFM_DATA = MAC_MCPS_VS_TYPE_OFFSET,
    MAC_MCPS_VS_DCFM_PURGE,
    MAC_MCPS_VS_IND_DATA
} MAC_teMcpsVsDcfmIndType;

#define NUM_MAC_MCPS_VS_DCFM_IND 3

/**
 * @brief MCPS Deferred Confirm/Indication Parameter union
 *
 * Union of all the possible MCPS Deferred Confirms or Indications
 */
typedef union
{
    MAC_tsMcpsVsCfmData  sVsDcfmData;   /**< Deferred transmit data confirm */
    MAC_tsMcpsVsCfmPurge sVsDcfmPurge;  /**< Deferred purge confirm */
    MAC_tsMcpsVsIndData  sVsIndData;    /**< Received data indication */
} MAC_tuMcpsVsDcfmIndParam;

/**
 * @brief MCPS Deferred Confirm/Indication
 *
 * The object passed in the MCPS Deferred Confirm/Indication callback
 */
typedef struct
{
    uint8                    u8Type;          /**< Indication type (@sa MAC_teMcpsVsDcfmIndType) */
    uint8                    u8ParamLength;   /**< Parameter length in following union */
    uint16                   u16Pad;          /**< Padding to force alignment */
    MAC_tuMcpsVsDcfmIndParam uParam;          /**< Union of all possible Indications */
} MAC_tsMcpsVsDcfmInd;

/* @} */

/**
 * @defgroup g_mac_sap_gen Generic headers
 * @ingroup g_mac_sap
 *
 * Generic headers which abstract the parameter interfaces to the function calls.
 * The headers reflect the common structure at the head of the derived structures
 * for MLME/MCPS
 *
 * @{
 */

#define MAC_MAX_ZBP_BEACON_PAYLOAD_LEN 15

typedef struct
{
    uint8          u8Status;                /**< Status of scan request @sa MAC_Enum_e */
    uint8          u8ScanType;              /**< Scan type */
    uint8          u8ResultListSize;        /**< Size of scan results list */
    uint8          u8Pad;                   /**< Padding to show alignment */
    uint32         u32UnscannedChannels;    /**< Bitmap of unscanned channels */
    uint8          au8EnergyDetect[MAC_MAX_SCAN_CHANNELS];
} MAC_tsMlmeVsCfmScan;


typedef struct
{
    MAC_PanDescr_s   sPANdescriptor;                                      /**< PAN descriptor */
    uint8            u8BSN;                                               /**< Beacon sequence number */
    uint8            au8BeaconPayload[MAC_MAX_ZBP_BEACON_PAYLOAD_LEN];    /**< Beacon payload */
} MAC_tsMlmeVsIndBeacon;

typedef union
{
    MAC_tsMlmeVsCfmScan         sVsDcfmScan;
    MAC_MlmeCfmAssociate_s      sDcfmAssociate;
    MAC_MlmeCfmDisassociate_s   sDcfmDisassociate;
    MAC_MlmeCfmPoll_s           sDcfmPoll;
    MAC_MlmeCfmRxEnable_s       sDcfmRxEnable;
    MAC_MlmeIndAssociate_s      sIndAssociate;
    MAC_MlmeIndDisassociate_s   sIndDisassociate;
    MAC_MlmeIndGts_s            sIndGts;
    MAC_tsMlmeVsIndBeacon       sVsIndBeacon;
    MAC_MlmeIndSyncLoss_s       sIndSyncLoss;
    MAC_MlmeIndCommStatus_s     sIndCommStatus;
    MAC_MlmeIndOrphan_s         sIndOrphan;
#if defined(DEBUG) && defined(EMBEDDED)
    MAC_MlmeIndVsDebug_s        sIndVsDebug;
#endif /* defined(DEBUG) && defined(EMBEDDED) */
} MAC_tuMlmeVsDcfmIndParam;

typedef struct
{
    uint8                    u8Type;          /**< Deferred Confirm/Indication type @sa MAC_MlmeDcfmIndType_e */
    uint8                    u8ParamLength;   /**< Parameter length in following union */
    uint16                   u16Pad;          /**< Padding to force alignment */
    MAC_tuMlmeVsDcfmIndParam uParam;          /**< Union of all possible Deferred Confirms/Indications */
} MAC_tsMlmeVsDcfmInd;

typedef void (*pfnMacVsDataRequest) (uint16 u16Pan, uint16 u16Short);
/****************************/
/**** EXPORTED VARIABLES ****/
/****************************/

/****************************/
/**** EXPORTED FUNCTIONS ****/
/****************************/
PUBLIC void MAC_vRegisterDataRequest(pfnMacVsDataRequest pfn);
PUBLIC void MAC_vSetMutex(void* hMacMutex);
PUBLIC void MAC_vHandleMcpsVsReqRsp(
	void* pvMac,
    MAC_tsMcpsVsReqRsp *psMcpsVsReqRsp,
    MAC_tsMcpsVsSyncCfm *psMcpsVsSyncCfm);

PUBLIC void MAC_vHandleMlmeVsReqRsp(
    void* pvMac,
    MAC_MlmeReqRsp_s *psMlmeReqRsp,
    MAC_MlmeSyncCfm_s *psMlmeSyncCfm);

/****************************************************************************/
/***        ZPS MAC SHIM EXPORTS (Called by Zigbee Pro Stack directly     ***/
/****************************************************************************/
#define MAX_TIMERS 7
#define TIMER_INTERVAL 64
#define u32SymbolsToTicks(A)   (((A) + TIMER_INTERVAL - 1) / TIMER_INTERVAL)



PUBLIC uint32 ZPS_eMiniMacSetTxBuffers (uint8 u8MaxTxBuffers);
PUBLIC void
ZPS_MAC_vShimInit(PR_GET_BUFFER prMlmeGetBuffer,
              PR_POST_CALLBACK prMlmeCallback,
              PR_GET_BUFFER prMcpsGetBuffer,
              PR_POST_CALLBACK prMcpsCallback,
              void *pfGetVsBuffer,
              void *pfVsCallback,
              void *pvApl,
              void *pvNwk,
              void *pvMac);


PUBLIC void         ZPS_vMacSetFilterStorage (uint16* pu16FilterTable,uint8* pu8LinkQualityTable, uint16 u16MaxTableNumAddrs);
PUBLIC uint32       ZPS_eMacFilterAddAccept(uint16 u16Addr, uint8 u8LinkQuality);


PUBLIC void 		ZPS_vMacModifyForZgp(bool_t bStopCca, uint8 u8SeqNo);
PUBLIC void 		ZPS_vNwkHandleMcpsDcfmInd (void *pvNwk,
										MAC_DcfmIndHdr_s *psDcfmIndHdr);

PUBLIC uint32 		ZPS_u32MacSetTxBuffers(uint8 u8MaxTxBuffers);


PUBLIC void 		ZPS_vPhyInterruptHandler(void);
PUBLIC void 		ZPS_vMacInterruptHandler(void);



/* ZPS MAC PIB Get Functions */
PUBLIC	uint16		ZPS_u16MacPibGetCoordShortAddress(void);
PUBLIC	uint16		ZPS_u16MacPibGetMaxFrameTotalWaitTime(void);
PUBLIC	uint16		ZPS_u16MacPibGetTransactionPersistenceTime(void);
PUBLIC	uint8		ZPS_u8MacPibGetMaxFrameRetries(void);
PUBLIC	uint8		ZPS_u8MacPibGetResponseWaitTime(void);
PUBLIC	uint16		ZPS_u16MacPibGetPanId(void);
PUBLIC	uint16		ZPS_u16MacPibGetShortAddr(void);
PUBLIC	uint8		ZPS_u8MacPibGetMinBe(void);
PUBLIC	uint8		ZPS_u8MacPibGetMaxBe(void);
PUBLIC	uint8		ZPS_u8MacPibGetMaxCsmaBackoffs(void);
PUBLIC	bool_t		ZPS_bMacPibGetRxOnWhenIdle(void);
PUBLIC	tsExtAddr	ZPS_sMacPibGetCoordExtAddr(void);
PUBLIC	uint32		ZPS_u32MacPibGetMacFrameCounter(void);
PUBLIC	bool_t		ZPS_bMacPibGetAssociationPermit(void);
PUBLIC	uint8		ZPS_u8MacPibGetBeaconPayloadLength(void);
PUBLIC	uint8		ZPS_u8MacPibGetBeaconPayload(uint8 *pu8Payload);
PUBLIC	uint8		ZPS_u8MacPibGetBsn(void);
PUBLIC	uint8		ZPS_u8MacPibGetDsn(void);
PUBLIC	bool_t		ZPS_bMacPibGetPanCoordinator(void);
PUBLIC	uint8		ZPS_u8MacPibGetBeaconRequestLqiThreshold(void);
PUBLIC	uint8		ZPS_u8MacPibGetMaxMaxConcurrentAssocResponses(void);
PUBLIC	tsExtAddr 	ZPS_sMacPibGetExtAddr(void);
PUBLIC	uint8		ZPS_u8MacPibGetEBsn(void);
PUBLIC	bool_t		ZPS_bMacPibGetEBFilteringEnable(void);
PUBLIC	uint8		ZPS_u8MacPibGetEBR_PayloadLength(void);
PUBLIC	uint8		ZPS_u8MacPibGetEBR_Payload(uint8 *pu8EBRPayload);


/* ZPS MAC PIB Set Functions */
PUBLIC	void		ZPS_vMacPibSetCoordShortAddress(uint16 u16CoordShortAddr);
PUBLIC	void		ZPS_vMacPibSetMaxFrameTotalWaitTime(uint16 u16MaxFrameTotalWaitTime);
PUBLIC	void		ZPS_vMacPibSetTransactionPersistenceTime(uint16 u16TransactionPersistenceTime);
PUBLIC	void		ZPS_vMacPibSetMaxFrameRetries(uint8 u8MaxFrameRetries);
PUBLIC	void		ZPS_vMacPibSetResponseWaitTime(uint8 u8ResponseWaitTime);
PUBLIC	void		ZPS_vMacPibSetPanId(uint16 u16PanID);
PUBLIC	void		ZPS_vMacPibSetShortAddr(uint16 u16ShortAddr);
PUBLIC	void		ZPS_vMacPibSetMinBe(uint8 u8MinBe);
PUBLIC	void		ZPS_vMacPibSetMaxBe(uint8 u8MaxBe);
PUBLIC	void		ZPS_vMacPibSetMaxCsmaBackoffs(uint8 u8MaxCsmaBackoffs);
PUBLIC	void 		ZPS_vMacPibSetRxOnWhenIdle(bool_t bNewState, bool_t bInReset);
PUBLIC	void		ZPS_vMacPibSetCoordExtAddr(tsExtAddr sCoordExtAddr);
PUBLIC	void		ZPS_vMacPibSetMacFrameCounter(uint32 u32MacFrameCounter);
PUBLIC	void		ZPS_vMacPibSetAssociationPermit(bool_t bAssociatePermit);
PUBLIC	void		ZPS_vMacPibSetBeaconPayloadLength(uint8 u8BeaconPayloadLen);
PUBLIC	void		ZPS_vMacPibSetBeaconPayload(const uint8* pu8Payload, uint8 u8Len);
PUBLIC	void		ZPS_vMacPibSetBsn(uint8 u8Bsn);
PUBLIC	void		ZPS_vMacPibSetDsn(uint8 u8Dsn);
PUBLIC	void		ZPS_vMacPibSetPanCoordinator(bool_t bPanCoordinator);
PUBLIC	void		ZPS_vMacPibSetBeaconRequestLqiThreshold(uint8 u8LqiThreshold);
PUBLIC	void		ZPS_vMacPibSetMaxMaxConcurrentAssocResponses(uint8 u8Max);
PUBLIC	void 		ZPS_vMacPibSetExtAddr(tsExtAddr *psExtAddr);
PUBLIC  void 		ZPS_vMacPibSetRxInCca(bool_t bEnable);
PUBLIC	void		ZPS_vMacPibSetEBsn(uint8 u8EBsn);
PUBLIC	void		ZPS_vMacPibSetEBFilteringEnable(bool_t bEBFilteringEnabled);
PUBLIC	void		ZPS_vMacPibSetEBR_PayloadLength(uint8 u8EBRPayloadLen);
PUBLIC	void		ZPS_vMacPibSetEBR_Payload(const uint8* pu8EBRPayload, uint8 u8EBRPayloadLen);


/****************************************************************************/
/***        ZPS SOC MAC EXPORTS (Called by ZPS MAC SHIM directly          ***/
/****************************************************************************/
#ifdef JENNIC_MAC_MiniMacShim
PUBLIC void SOC_MAC_vShimInit(PR_GET_BUFFER prMlmeGetBuffer,
              	  	  	  PR_POST_CALLBACK prMlmeCallback,
              	  	  	  PR_GET_BUFFER prMcpsGetBuffer,
              	  	  	  PR_POST_CALLBACK prMcpsCallback,
              	  	  	  void *pfGetVsBuffer,
              	  	  	  void *pfVsCallback,
              	  	  	  void *pvApl,
              	  	  	  void *pvNwk,
              	  	  	  void *pvMac);
#endif

PUBLIC void SOC_MAC_vHandleMcpsVsReqRsp(void* pvMac,
									MAC_tsMcpsVsReqRsp *psMcpsVsReqRsp,
									MAC_tsMcpsVsSyncCfm *psMcpsVsSyncCfm);
PUBLIC void SOC_MAC_vHandleMlmeVsReqRsp(void* pvMac,
										MAC_MlmeReqRsp_s *psMlmeReqRsp,
										MAC_MlmeSyncCfm_s *psMlmeSyncCfm);


PUBLIC void vMiniMac_InterruptHandler(void);



PUBLIC void         SOC_MAC_vSetFilterStorage (uint16* pu16FilterTable,uint8* pu8LinkQualityTable, uint16 u16MaxTableNumAddrs);
PUBLIC uint32       SOC_MAC_eMacFilterAddAccept(uint16 u16Addr, uint8 u8LinkQuality);


PUBLIC void SOC_ZPS_vMacModifyForZgp(bool_t bStopCca, uint8 u8SeqNo);
PUBLIC void SOC_ZPS_vNwkHandleMcpsDcfmInd (void *pvNwk,
										MAC_DcfmIndHdr_s *psDcfmIndHdr);

PUBLIC void 		SOC_ZPS_vSetMacAddrLocation(void* pu64MacAddress);
PUBLIC void* 	    SOC_ZPS_pvGetMacAddrLocation(void);

/* ZPS MAC PIB Get Functions */
PUBLIC	uint16		SOC_ZPS_u16MacPibGetCoordShortAddress(void);
PUBLIC	uint16		SOC_ZPS_u16MacPibGetMaxFrameTotalWaitTime(void);
PUBLIC	uint16		SOC_ZPS_u16MacPibGetTransactionPersistenceTime(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetMaxFrameRetries(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetResponseWaitTime(void);
PUBLIC	uint16		SOC_ZPS_u16MacPibGetPanId(void);
PUBLIC	uint16		SOC_ZPS_u16MacPibGetShortAddr(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetMinBe(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetMaxBe(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetMaxCsmaBackoffs(void);
PUBLIC	bool_t		SOC_ZPS_bMacPibGetRxOnWhenIdle(void);
PUBLIC	tsExtAddr	SOC_ZPS_sMacPibGetCoordExtAddr(void);
PUBLIC	uint32		SOC_ZPS_u32MacPibGetMacFrameCounter(void);
PUBLIC	bool_t		SOC_ZPS_bMacPibGetAssociationPermit(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetBeaconPayloadLength(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetBeaconPayload(uint8 *pu8Payload);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetBsn(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetDsn(void);
PUBLIC	bool_t		SOC_ZPS_bMacPibGetPanCoordinator(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetBeaconRequestLqiThreshold(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetMaxMaxConcurrentAssocResponses(void);
PUBLIC	tsExtAddr 	SOC_ZPS_sMacPibGetExtAddr(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetEBsn(void);
PUBLIC	bool_t		SOC_ZPS_bMacPibGetEBFilteringEnable(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetEBR_PayloadLength(void);
PUBLIC	uint8		SOC_ZPS_u8MacPibGetEBR_Payload(uint8 *pu8EBRPayload);

/* ZPS MAC PIB Set Functions */
PUBLIC	void		SOC_ZPS_vMacPibSetCoordShortAddress(uint16 u16CoordShortAddr);
PUBLIC	void		SOC_ZPS_vMacPibSetMaxFrameTotalWaitTime(uint16 u16MaxFrameTotalWaitTime);
PUBLIC	void		SOC_ZPS_vMacPibSetTransactionPersistenceTime(uint16 u16TransactionPersistenceTime);
PUBLIC	void		SOC_ZPS_vMacPibSetMaxFrameRetries(uint8 u8MaxFrameRetries);
PUBLIC	void		SOC_ZPS_vMacPibSetResponseWaitTime(uint8 u8ResponseWaitTime);
PUBLIC	void		SOC_ZPS_vMacPibSetPanId(uint16 u16PanID);
PUBLIC	void		SOC_ZPS_vMacPibSetShortAddr(uint16 u16ShortAddr);
PUBLIC	void		SOC_ZPS_vMacPibSetMinBe(uint8 u8MinBe);
PUBLIC	void		SOC_ZPS_vMacPibSetMaxBe(uint8 u8MaxBe);
PUBLIC	void		SOC_ZPS_vMacPibSetMaxCsmaBackoffs(uint8 u8MaxCsmaBackoffs);
PUBLIC	void 		SOC_ZPS_vMacPibSetRxOnWhenIdle(bool_t bNewState, bool_t bInReset);
PUBLIC	void		SOC_ZPS_vMacPibSetCoordExtAddr(tsExtAddr sCoordExtAddr);
PUBLIC	void		SOC_ZPS_vMacPibSetMacFrameCounter(uint32 u32MacFrameCounter);
PUBLIC	void		SOC_ZPS_vMacPibSetAssociationPermit(bool_t bAssociatePermit);
PUBLIC	void		SOC_ZPS_vMacPibSetBeaconPayloadLength(uint8 u8BeaconPayloadLen);
PUBLIC	void		SOC_ZPS_vMacPibSetBeaconPayload(const uint8* pu8Payload, uint8 u8Len);
PUBLIC	void		SOC_ZPS_vMacPibSetBsn(uint8 u8Bsn);
PUBLIC	void		SOC_ZPS_vMacPibSetDsn(uint8 u8Dsn);
PUBLIC	void		SOC_ZPS_vMacPibSetPanCoordinator(bool_t bPanCoordinator);
PUBLIC	void		SOC_ZPS_vMacPibSetBeaconRequestLqiThreshold(uint8 u8LqiThreshold);
PUBLIC	void		SOC_ZPS_vMacPibSetMaxMaxConcurrentAssocResponses(uint8 u8Max);
PUBLIC	void 		SOC_ZPS_vMacPibSetExtAddr(tsExtAddr *psExtAddr);
PUBLIC  void 		SOC_ZPS_vMacPibSetRxInCca(bool_t bEnable);
PUBLIC	void		SOC_ZPS_vMacPibSetEBsn(uint8 u8EBsn);
PUBLIC	void		SOC_ZPS_vMacPibSetEBFilteringEnable(bool_t bEBFilteringEnabled);
PUBLIC	void		SOC_ZPS_vMacPibSetEBR_PayloadLength(uint8 u8EBRPayloadLen);
PUBLIC	void		SOC_ZPS_vMacPibSetEBR_Payload(const uint8* pu8EBRPayload, uint8 u8EBRPayloadLen);


/****************************************************************************/
/***        ZPS SERIAL MAC EXPORTS (Called by ZPS MAC SHIM directly          ***/
/****************************************************************************/
#ifdef JENNIC_MAC_MiniMacShim
PUBLIC void SERIAL_MAC_vShimInit(PR_GET_BUFFER prMlmeGetBuffer,
              	  	  	  PR_POST_CALLBACK prMlmeCallback,
              	  	  	  PR_GET_BUFFER prMcpsGetBuffer,
              	  	  	  PR_POST_CALLBACK prMcpsCallback,
              	  	  	  void *pfGetVsBuffer,
              	  	  	  void *pfVsCallback,
              	  	  	  void *pvApl,
              	  	  	  void *pvNwk,
              	  	  	  void *pvMac);
#endif

PUBLIC void SERIAL_MAC_vHandleMcpsVsReqRsp(void* pvMac,
									MAC_tsMcpsVsReqRsp *psMcpsVsReqRsp,
									MAC_tsMcpsVsSyncCfm *psMcpsVsSyncCfm);
PUBLIC void SERIAL_MAC_vHandleMlmeVsReqRsp(void* pvMac,
										MAC_MlmeReqRsp_s *psMlmeReqRsp,
										MAC_MlmeSyncCfm_s *psMlmeSyncCfm);
PUBLIC void vSerialMac_InterruptHandler(void);

PUBLIC void SERIAL_ZPS_vMacModifyForZgp(bool_t bStopCca, uint8 u8SeqNo);
PUBLIC void SERIAL_ZPS_vNwkHandleMcpsDcfmInd (void *pvNwk,
										MAC_DcfmIndHdr_s *psDcfmIndHdr);

PUBLIC void 		SERIAL_ZPS_vSetMacAddrLocation(void* pu64MacAddress);
PUBLIC void* 	    SERIAL_ZPS_pvGetMacAddrLocation(void);


/* ZPS MAC PIB Get Functions */
PUBLIC	uint16		SERIAL_ZPS_u16MacPibGetCoordShortAddress(void);
PUBLIC	uint16		SERIAL_ZPS_u16MacPibGetMaxFrameTotalWaitTime(void);
PUBLIC	uint16		SERIAL_ZPS_u16MacPibGetTransactionPersistenceTime(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetMaxFrameRetries(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetResponseWaitTime(void);
PUBLIC	uint16		SERIAL_ZPS_u16MacPibGetPanId(void);
PUBLIC	uint16		SERIAL_ZPS_u16MacPibGetShortAddr(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetMinBe(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetMaxBe(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetMaxCsmaBackoffs(void);
PUBLIC	bool_t		SERIAL_ZPS_bMacPibGetRxOnWhenIdle(void);
PUBLIC	tsExtAddr	SERIAL_ZPS_sMacPibGetCoordExtAddr(void);
PUBLIC	uint32		SERIAL_ZPS_u32MacPibGetMacFrameCounter(void);
PUBLIC	bool_t		SERIAL_ZPS_bMacPibGetAssociationPermit(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetBeaconPayloadLength(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetBeaconPayload(uint8 *pu8Payload);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetBsn(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetDsn(void);
PUBLIC	bool_t		SERIAL_ZPS_bMacPibGetPanCoordinator(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetBeaconRequestLqiThreshold(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetMaxMaxConcurrentAssocResponses(void);
PUBLIC	tsExtAddr 	SERIAL_ZPS_sMacPibGetExtAddr(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetEBsn(void);
PUBLIC	bool_t		SERIAL_ZPS_bMacPibGetEBFilteringEnable(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetEBR_PayloadLength(void);
PUBLIC	uint8		SERIAL_ZPS_u8MacPibGetEBR_Payload(uint8 *pu8EBRPayload);

/* ZPS MAC PIB Set Functions */
PUBLIC	void		SERIAL_ZPS_vMacPibSetCoordShortAddress(uint16 u16CoordShortAddr);
PUBLIC	void		SERIAL_ZPS_vMacPibSetMaxFrameTotalWaitTime(uint16 u16MaxFrameTotalWaitTime);
PUBLIC	void		SERIAL_ZPS_vMacPibSetTransactionPersistenceTime(uint16 u16TransactionPersistenceTime);
PUBLIC	void		SERIAL_ZPS_vMacPibSetMaxFrameRetries(uint8 u8MaxFrameRetries);
PUBLIC	void		SERIAL_ZPS_vMacPibSetResponseWaitTime(uint8 u8ResponseWaitTime);
PUBLIC	void		SERIAL_ZPS_vMacPibSetPanId(uint16 u16PanID);
PUBLIC	void		SERIAL_ZPS_vMacPibSetShortAddr(uint16 u16ShortAddr);
PUBLIC	void		SERIAL_ZPS_vMacPibSetMinBe(uint8 u8MinBe);
PUBLIC	void		SERIAL_ZPS_vMacPibSetMaxBe(uint8 u8MaxBe);
PUBLIC	void		SERIAL_ZPS_vMacPibSetMaxCsmaBackoffs(uint8 u8MaxCsmaBackoffs);
PUBLIC	void 		SERIAL_ZPS_vMacPibSetRxOnWhenIdle(bool_t bNewState, bool_t bInReset);
PUBLIC	void		SERIAL_ZPS_vMacPibSetCoordExtAddr(tsExtAddr sCoordExtAddr);
PUBLIC	void		SERIAL_ZPS_vMacPibSetMacFrameCounter(uint32 u32MacFrameCounter);
PUBLIC	void		SERIAL_ZPS_vMacPibSetAssociationPermit(bool_t bAssociatePermit);
PUBLIC	void		SERIAL_ZPS_vMacPibSetBeaconPayloadLength(uint8 u8BeaconPayloadLen);
PUBLIC	void		SERIAL_ZPS_vMacPibSetBeaconPayload(const uint8* pu8Payload, uint8 u8Len);
PUBLIC	void		SERIAL_ZPS_vMacPibSetBsn(uint8 u8Bsn);
PUBLIC	void		SERIAL_ZPS_vMacPibSetDsn(uint8 u8Dsn);
PUBLIC	void		SERIAL_ZPS_vMacPibSetPanCoordinator(bool_t bPanCoordinator);
PUBLIC	void		SERIAL_ZPS_vMacPibSetBeaconRequestLqiThreshold(uint8 u8LqiThreshold);
PUBLIC	void		SERIAL_ZPS_vMacPibSetMaxMaxConcurrentAssocResponses(uint8 u8Max);
PUBLIC	void 		SERIAL_ZPS_vMacPibSetExtAddr(tsExtAddr *psExtAddr);
PUBLIC  void 		SERIAL_ZPS_vMacPibSetRxInCca(bool_t bEnable);
PUBLIC	void		SERIAL_ZPS_vMacPibSetEBsn(uint8 u8EBsn);
PUBLIC	void		SERIAL_ZPS_vMacPibSetEBFilteringEnable(bool_t bEBFilteringEnabled);
PUBLIC	void		SERIAL_ZPS_vMacPibSetEBR_PayloadLength(uint8 u8EBRPayloadLen);
PUBLIC	void		SERIAL_ZPS_vMacPibSetEBR_Payload(const uint8* pu8EBRPayload, uint8 u8EBRPayloadLen);






#ifdef __cplusplus
};
#endif

#endif /* _mac_vs_data_h_ */


