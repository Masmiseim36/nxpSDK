/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Software LLCP Link layer Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 18. Feb 2015
*
*/

#ifndef PHLNLLCP_SW_H
#define PHLNLLCP_SW_H

#include <ph_Status.h>

#ifndef NXPRDLIB_REM_GEN_INTFS
#   include <phlnLlcp.h>
#endif

#define PHLN_LLCP_HEADER_SIZE                 0x03U            /**< Header size of the LLCP in Information PDU (I -PDU). */
#define PHLN_LLCP_UI_HEADER_SIZE              0x02U            /**< Header size of the LLCP in Connection-less Information PDU (UI -PDU). */
#define PHLN_LLCP_AGF_HEADER_SIZE             0x02U            /**< Header size of the LLCP in Aggregation PDU (AGF -PDU). */
#define PHLN_LLCP_SNL_HEADER_SIZE             0x02U            /**< Header size of the LLCP in SNL PDU. */

#define PHLN_LLCP_AGF_LEN_FIELD_SIZE          0x02U            /**< Length field size of the Aggregation PDU (AGF -PDU). */

#define PHLN_LLCP_INVALID_VERSION             0x00U            /**< Invalid VERSION value. */
#define PHLN_LLCP_VERSION_MAJOR_MASK          0xF0U            /**< Mask to apply to get major version number. */
#define PHLN_LLCP_VERSION_MINOR_MASK          0x0FU            /**< Mask to apply to get minor version number. */

#define PHLN_LLCP_DEFAULT_SPEC_MIU            128U             /**< Default or minimum LLCP MIU value as per LLCP specification v1.1. */

#define PHLN_LLCP_PDU_MERGE_DSAP(dsap, ptype)           \
  ( (((ptype) & /* */ 0x0CU) >> 2U) | ((dsap) << 2U) )

#define PHLN_LLCP_PDU_MERGE_SSAP(ptype, ssap)           \
  ( (((ptype) & /* */ 0x03U) << 6U) | (ssap) )

#define PHLN_LLCP_PDU_MERGE_SEQ(Ns, Nr)                 \
  ( (Nr) | ((Ns) << 4U) )

#define PHLN_LLCP_PDU_GET_PTYPE(Rxbyte0, Rxbyte1)    \
  ((((Rxbyte0) & 0x03U) << 2U)  | \
  (((Rxbyte1) & 0xC0U) >> 6U))
#define PHLN_LLCP_PDU_GET_DSAP(Rxbyte0)              \
  ( ((Rxbyte0) & 0xFCU) >> 2U )
#define PHLN_LLCP_PDU_GET_SSAP(Rxbyte1)              \
  ( (Rxbyte1) & 0x3FU )

#if 0
/* Disconnected Mode Reasons */
#define PHLN_LLCP_PDU_DM_DISC         0x00U                    /**< Indicates the LLC has received a DISC PDU and is now logically disconnected from the data link connection. */
#define PHLN_LLCP_PDU_DM_NO_CONN      0x01U                    /**< Indicates the LLC has received a connection-oriented PDU but the target service access point has no active connection. */
#define PHLN_LLCP_PDU_DM_NO_SAP       0x02U                    /**< Indicates the remote LLC has received a CONNECT PDU and there is no service bound to the specified target service access point. */
#define PHLN_LLCP_PDU_DM_CONN_REJ     0x03U                    /**< Indicates the remote LLC has processed a CONNECT PDU and the request to connect was rejected by the service layer. */
#define PHLN_LLCP_PDU_DM_SAP_REJ      0x10U                    /**< Indicates the LLC will permanently not accept any CONNECT PDUs with the same target service access point address. */
#define PHLN_LLCP_PDU_DM_ANY_REJ      0x11U                    /**< Indicates the LLC will permanently not accept any CONNECT PDUs with any target service access point address. */
#define PHLN_LLCP_PDU_DM_TEMP_SAP_REJ 0x20U                    /**< Indicates the LLC will temporarily not accept any CONNECT PDUs with the specified target service access point address. */
#define PHLN_LLCP_PDU_DM_TEMP_ANY_REJ 0x21U                    /**< Indicates the LLC will temporarily not accept any CONNECT PDUs with any target service access point address. */
#endif

/**
*  \name NFC Internal status codes
*  Internal Status codes for the NFC components.
*/
/*@{*/

/**
* The system is busy with the previous read operation.
*/
#define PH_ERR_LLCP_BUSY                                     0x91U

/**
* Received SNL PDU with SDRES TLV. Need to unblock DiscoverServices Call.
*/
#define PH_ERR_LLCP_RECEIVED_SDRES                           0x92U

/**
* Received SNL PDU with no SDREQ TLV. No need to send SNL PDU back.
*/
#define PH_ERR_LLCP_NO_SDREQ                                 0x93U
/*@}*/


phStatus_t phlnLlcp_Sw_Activate(
                                phlnLlcp_Sw_DataParams_t * pDataParams,
                                uint8_t*                   pRemoteGnrlBytes,
                                uint8_t                    bRemoteGnrBytesLen,
                                uint8_t                    bDevType
                                );


phStatus_t phlnLlcp_Sw_Deactivate(
                                  phlnLlcp_Sw_DataParams_t * pDataParams
                                  );

phStatus_t phlnLlcp_Sw_WaitForActivation(
                                         phlnLlcp_Sw_DataParams_t * pDataParams
                                         );

phStatus_t phlnLlcp_Sw_Transport_Socket_Init(
                                             phlnLlcp_Sw_DataParams_t * pDataParams,
                                             phlnLlcp_Transport_Socket_t * psSocket,
                                             phlnLlcp_Transport_Socket_Type_t eSocketType,
                                             uint8_t* pSocketRxBuffer,
                                             uint32_t dwSocketRxBufferSize);

phStatus_t phlnLlcp_Sw_Transport_Socket_Register(
                                                 phlnLlcp_Sw_DataParams_t * pDataParams,
                                                 phlnLlcp_Transport_Socket_t * pSocket,
                                                 phlnLlcp_Transport_Socket_Type_t eSocketType,
                                                 uint8_t bLsap,
                                                 uint8_t * pUri);

phStatus_t phlnLlcp_Sw_Transport_Socket_WaitForConnection(
                                                          void * pDataParams,
                                                          phlnLlcp_Transport_Socket_t * pServerSocket,
                                                          phlnLlcp_Transport_Socket_t * pConnServerSocket
                                                          );

phStatus_t phlnLlcp_Sw_Transport_Socket_Connect(
                                                phlnLlcp_Sw_DataParams_t * pDataParams,
                                                phlnLlcp_Transport_Socket_t * pClientSocket,
                                                uint8_t bDsap
                                                );

phStatus_t phlnLlcp_Sw_Transport_Socket_ConnectViaUri(
                                                      phlnLlcp_Sw_DataParams_t  *pDataParams,
                                                      phlnLlcp_Transport_Socket_t* pClientSocket,
                                                      uint8_t *pUri
                                                      );

phStatus_t phlnLlcp_Sw_Transport_Socket_Receive(
                                                phlnLlcp_Sw_DataParams_t * pDataParams,
                                                phlnLlcp_Transport_Socket_t* psSocket
                                                );

phStatus_t phlnLlcp_Sw_Transport_Socket_Send(
                                             phlnLlcp_Sw_DataParams_t * pDataParams,
                                             phlnLlcp_Transport_Socket_t * pClientSocket,
                                             uint8_t * pTxBuffer,
                                             uint32_t dwTxBufferSize,
                                             uint16_t wFrameOpt
                                             );

phStatus_t phlnLlcp_Sw_Transport_Socket_DiscoverServices(
                                                         phlnLlcp_Sw_DataParams_t * pDataParams,
                                                         phlnLlcp_Transport_Socket_t * pSocket,
                                                         uint8_t  bListSize,
                                                         uint8_t  bSNBuffSize,
                                                         uint8_t *  psServiceNameList,
                                                         uint8_t *  pSapList
                                                         );

phStatus_t phlnLlcp_Sw_Transport_Socket_SendTo(
                                               phlnLlcp_Sw_DataParams_t * pDataParams,
                                               phlnLlcp_Transport_Socket_t * pClientSocket,
                                               uint8_t   bDsap,
                                               uint8_t * pTxBuffer,
                                               uint32_t dwTxBufferSize,
                                               uint16_t wFrameOpt
                                               );

phStatus_t phlnLlcp_Sw_Transport_Socket_Disconnect(
                                                   phlnLlcp_Sw_DataParams_t* pDataParams,
                                                   phlnLlcp_Transport_Socket_t * pClientSocket
                                                   );

phStatus_t phlnLlcp_Sw_Transport_Socket_Unregister(
                                                   phlnLlcp_Sw_DataParams_t * pDataParams,
                                                   phlnLlcp_Transport_Socket_t * pSocket
                                                   );

phStatus_t phlnLlcp_Sw_Transport_Socket_UnblockAll(
                                                   phlnLlcp_Sw_DataParams_t * pDataParams
                                                   );

phStatus_t phlnLlcp_Sw_DeInit(
                              phlnLlcp_Sw_DataParams_t * pDataParams
                              );

#endif /* PHLNLLCP_SW_H */
