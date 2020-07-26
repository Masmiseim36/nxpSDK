/*
* =============================================================================
*
*                    Copyright (c), NXP Semiconductors
*
*                       (C)NXP Electronics N.V.2008
*         All rights are reserved. Reproduction in whole or in part is
*        prohibited without the written consent of the copyright owner.
*    NXP reserves the right to make changes without notice at any time.
*   NXP makes no warranty, expressed, implied or statutory, including but
*   not limited to any implied warranty of merchantability or fitness for any
*  particular purpose, or that the use will not infringe any third party patent,
*   copyright or trademark. NXP must not be liable for any loss or damage
*                            arising from its use.
*
* =============================================================================
*/

/**
* =========================================================================== *
*                                                                             *
*                                                                             *
* \file  phnpSnep_Sw.h                                                       *
* \brief SNEP protocol management public interfaces listed in this module.    *
*                                                                             *
*                                                                             *
* Project:                                                                    *
*                                                                             *
* $Date$                       *
* $Author$                                                         *
* $Revision$ (v06.01.00)
* $Aliases:   $                                                               *
*                                                                             *
* =========================================================================== *
*/

#ifndef PHNPSNEP_SW_H
#define PHNPSNEP_SW_H

#include <ph_Status.h>
#ifndef NXPRDLIB_REM_GEN_INTFS
#   include <phnpSnep.h>
#endif

#define PHNP_SNEP_DEFAULT_SERVER_SAP     0x04U            /**< Defined as per Standard. */
#define PHNP_SNEP_NONDEFAULT_SERVER_SAP  0x15U            /**< Non Default Server SAP Address. */

#define PHNPSNEP_VERSION_MAJOR           0x01U
#define PHNPSNEP_VERSION_MINOR           0x00U

/**
 *  NFC Forum SNEP Version 1.0 supported by this implementation.
 */
#define PHNP_SNEP_VER                   ((PHNPSNEP_VERSION_MAJOR * 16U) + PHNPSNEP_VERSION_MINOR)

#define PHNP_SNEP_VERSION_MAJOR_MASK     0xF0U            /**< Mask to apply to get major version number. */
#define PHNP_SNEP_VERSION_MINOR_MASK     0x0FU            /**< Mask to apply to get minor version number. */
#define PHNP_SNEP_GET_HEADER_SIZE        0x0AU

#define PHNP_SNEP_ACCEPTABLE_LEN_FIELD   0x04U            /**< Length of Acceptable length field in GET Request. */

/**
*  \brief SNEP Response Field Values.
* These values are passed as input to \ref phnpSnep_SendCmds API that Server needs to send to Client.
*/
# if 0
typedef enum /*phnpSnep_Response_Values*/{
    PHNP_SNEP_RES_CONT        = 0x80,                                      /**< Continue sending remaining fragments. */
    PHNP_SNEP_RES_SUCCESS     = 0x81,                                      /**< Success Operation succeeded. */
    PHNP_SNEP_RES_NOT_FOUND   = 0xC0,                                      /**< Not Found Resource not found. */
    PHNP_SNEP_RES_EXCESS_DATA = 0xC1,                                      /**< Excess Data. Resource exceeds data size limit. */
    PHNP_SNEP_RES_BAD_REQ     = 0xC2,                                      /**< Bad Request not understood. */
    PHNP_SNEP_RES_NOT_IMP     = 0xE0,                                      /**< Not Implemented Unsupported functionality requested. */
    PHNP_SNEP_RES_VER_ERR     = 0xE1,                                      /**< Unsupported Version Unsupported protocol version. */
    PHNP_SNEP_RES_REJECT      = 0xFF                                       /**< Reject Do not send remaining fragments. */
} phnpSnep_Response_Values_t;

#endif

#define PHNP_SNEP_RES_CONT        0x80                                     /**< Continue sending remaining fragments. */
#define PHNP_SNEP_RES_SUCCESS     0x81                                     /**< Success Operation succeeded. */
#define PHNP_SNEP_RES_NOT_FOUND   0xC0                                      /**< Not Found Resource not found. */
#define PHNP_SNEP_RES_EXCESS_DATA 0xC1                                      /**< Excess Data. Resource exceeds data size limit. */
#define PHNP_SNEP_RES_BAD_REQ     0xC2                                      /**< Bad Request not understood. */
#define PHNP_SNEP_RES_NOT_IMP     0xE0                                      /**< Not Implemented Unsupported functionality requested. */
#define PHNP_SNEP_RES_VER_ERR     0xE1                                      /**< Unsupported Version Unsupported protocol version. */
#define PHNP_SNEP_RES_REJECT      0xFF

void phnpSnep_GetHeader(uint8_t eReqResField, uint32_t dwLength, uint8_t *pbHeader);

phStatus_t phnpSnep_Sw_ClientInit(
                                  phnpSnep_Sw_DataParams_t * pDataParams,
                                  phnpSnep_Server_type_t eServerType,
                                  uint8_t *pNonDefaultUri,
                                  uint8_t *pbSnepRxBuffer,
                                  uint32_t dwSnepRxBufferSize
                                  );

phStatus_t phnpSnep_Sw_ServerInit(
                                  phnpSnep_Sw_DataParams_t * pDataParams,
                                  phnpSnep_Server_type_t eServerType,
                                  uint8_t *pNonDefaultUri,
                                  uint8_t *pbSnepRxBuffer,
                                  uint32_t dwSnepRxBufferSize
                                  );

phStatus_t phnpSnep_Sw_ServerListen(
                                    phnpSnep_Sw_DataParams_t * pDataParams,
                                    uint32_t   dwGetReqBuffSize,
                                    uint8_t  * pGetReqData,
                                    uint32_t * dwGetReqLength,
                                    uint8_t  * pClientReq
                                    );

phStatus_t phnpSnep_Sw_ServerSendResponse(
                                          phnpSnep_Sw_DataParams_t * pDataParams,
                                          uint8_t bClientReq,
                                          uint8_t *pRespDataForGet,
                                          uint32_t dwRespDataLen,
                                          uint32_t dwPutBuffSize,
                                          uint8_t *pPutData,
                                          uint32_t *pdwPutDataLen
                                          );

phStatus_t phnpSnep_Sw_Get(
                           phnpSnep_Sw_DataParams_t * pDataParams,
                           uint8_t *pNdefMsg,
                           uint32_t dwNdefMsgLen,
                           uint8_t *pRxBuffer,
                           uint32_t *pdwRxLen,
                           uint32_t dwAppBufSize
                           );

phStatus_t phnpSnep_Sw_Put(
                           phnpSnep_Sw_DataParams_t * pDataParams,
                           uint8_t * pTxBuffer,
                           uint32_t dwTxBufferSize
                           );

phStatus_t phnpSnep_Sw_SendCmds(
                                phnpSnep_Sw_DataParams_t * pDataParams,
                                uint8_t eReqResField
                                );


phStatus_t phnpSnep_Sw_ClientDeInit(
                                    phnpSnep_Sw_DataParams_t * pDataParams
                                    );

phStatus_t phnpSnep_Sw_ServerDeInit(
                                    phnpSnep_Sw_DataParams_t * pDataParams
                                    );

#endif /* PHNPSNEP_SW_H */
