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
*  KK: Generated 19. Feb 2015
*
*/

#ifndef PHLNLLCP_SW_INT_H
#define PHLNLLCP_SW_INT_H

#include <phlnLlcp.h>
#include "phlnLlcp_Sw.h"

/* *****************************************************************************************************************
* Types/Structure Declarations
* ***************************************************************************************************************** */
/**
* \brief Used to differentiate between the socket register type.
*/
typedef enum /*phlnLlcp_SocketType*/{
    PHLN_LLCP_DEFAULTLINK_SOCKET = 0x00,                                            /**< LLCP Default Link Socket. */
    PHLN_LLCP_LOGICALLINK_SOCKET,                                                   /**< LLCP Logical Link Socket. */
    PHLN_LLCP_CONNECTIONSERVER_SOCKET                                               /**< LLCP Connection Server Socket. */
} phlnLlcp_SocketType_t;

#define PHLN_LLCP_PDU_FRAME_HEADER(Rsap, PTYPE, Lsap, NS, NR, baHeader) {           \
    baHeader[0] = (PHLN_LLCP_PDU_MERGE_DSAP(Rsap, PTYPE));                          \
    baHeader[1] = (PHLN_LLCP_PDU_MERGE_SSAP(PTYPE, Lsap));                          \
    baHeader[2] = (PHLN_LLCP_PDU_MERGE_SEQ(NS, NR));                                \
    }

#define PHLN_LLCP_PDU_FRAME_MANDATORY_HEADER(Rsap, PTYPE, Lsap, baHeader) {         \
    baHeader[0] = (PHLN_LLCP_PDU_MERGE_DSAP(Rsap, PTYPE));                          \
    baHeader[1] = (PHLN_LLCP_PDU_MERGE_SSAP(PTYPE, Lsap));                          \
    }

#define PHLN_LLCP_SW_MOD16_INC(x) if ((x) > 14U) { (x) = 0; }                      \
                                     else { (x)++; }

#define PHLN_LLCP_SW_MOD16_DEC(x) if ((x) == 0U) { (x) = 14; }                      \
                                     else { (x)--; }

#define PHLN_LLCP_SW_FIRST_TID    0x80U
/**
* As per mod16 if x > y then true
*/
#define PHLN_LLCP_SW_MOD16_HIGH(x, y)  ( ((((x) == 0U) && ((y) == 15U)) || ((x) > (y))) ? true : false )

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
*   Function Prototypes
* ***************************************************************************************************************** */

phStatus_t phlnLlcp_Sw_Int_Transport_Socket_Register(
                                                     phlnLlcp_Transport_Socket_t* pSocket,
                                                     phlnLlcp_Transport_Socket_Type_t eSocketType,
                                                     uint8_t bLsap,
                                                     uint8_t * pUri,
                                                     phlnLlcp_SocketType_t eType
                                                     );

phStatus_t phlnLlcp_Sw_Int_CheckGenBytes(uint8_t *pGenBytes,
                                         uint16_t wLength,
                                         phlnLlcp_LMDataParams_t *pLMBytes
                                         );

uint8_t * phlnLlcp_Sw_Int_GetUri(uint8_t *pGenBytes,
                                 uint16_t wLength,
                                 uint8_t *pbUriLen,
                                 uint8_t *pbTid,
                                 uint8_t *pbSDREQPresent);

void phlnLlcp_Sw_Int_ParseSAP(uint8_t *pGenBytes,
                              uint16_t wLength,
                              uint8_t bSAPCount,
                              uint8_t *pSAPVal);

phStatus_t phlnLlcp_Sw_Int_ParseGenBytes(uint8_t *pGenBytes,
                                         uint16_t wLength,
                                         phlnLlcp_LMDataParams_t *pLMBytes
                                         );

phStatus_t phlnLlcp_Sw_Int_VersionAgreement(uint8_t    bLocalVersion,
                                            uint8_t    bRemoteVersion,
                                            uint8_t   *pAgreedVersion);

/**
* Address should be from 32 to 63.
* Constraint as of now only 31 address are available.
*/
phStatus_t phlnLlcp_Sw_Int_Transport_Socket_GetSap(phlnLlcp_Sw_DataParams_t* pDataParams,
                                                   uint8_t *pbSap);

phStatus_t phlnLlcp_Sw_Int_Socket_SendInt(phlnLlcp_Transport_Socket_t* psSocket,
                                          uint8_t* pTxBuffer,
                                          uint32_t dwLength,
                                          uint16_t wFrameOpt,
                                          uint8_t bState);

phStatus_t phlnLlcp_Sw_Int_Send(phTools_Q_t *psMsgQueue,
                                uint8_t *pbPerformRx
                                );

uint16_t phlnLlcp_Sw_Int_Pdu_FrameHeader(uint8_t ePType,
                                         uint8_t bDsap,
                                         uint8_t bSsap,
                                         uint8_t bNr,
                                         uint8_t bNs,
                                         uint8_t *pbPduFrame
                                         );

phStatus_t phlnLlcp_Sw_Int_Pdu_Frmr(phlnLlcp_Transport_Socket_t *psSocket,
									uint8_t bPtype,
                                    uint8_t bLsap,
                                    uint8_t bRsap,
                                    uint8_t bSeq,
                                    uint8_t bReason,
                                    uint8_t bSend
                                    );

phStatus_t phlnLlcp_Sw_Int_Pdu_ConnectionLess_InfoEx(phlnLlcp_Transport_Socket_t *psSocket,
                                                     uint8_t *pbTxData,
                                                     uint16_t wLength,
                                                     uint16_t wFrameOpt,
                                                     uint8_t *pbPerformRx);

phStatus_t phlnLlcp_Sw_Int_Pdu_InfoEx(phlnLlcp_Transport_Socket_t *psSocket,
                                      uint8_t *pbTxData,
                                      uint16_t wLength,
                                      uint16_t wFrameOpt,
                                      uint8_t *pbPerformRx
                                      );

phStatus_t phlnLlcp_Sw_Int_Pdu_SnlEx(phlnLlcp_Transport_Socket_t *psSocket,
                                     uint8_t *pbTxData,
                                     uint16_t wLength,
                                     uint16_t wNoURI);

phStatus_t phlnLlcp_Sw_Int_Pdu_CcOrC(phlnLlcp_Transport_Socket_t *psSocket,
                                     uint8_t bPtype,
                                     uint8_t *pServiceName,
                                     uint8_t bSnLength
                                     );

phStatus_t phlnLlcp_Sw_Int_Pdu_DiscOrDm(phlnLlcp_Transport_Socket_t *psSocket,
                                        uint8_t bPtype,
                                        uint8_t bReason
                                        );

phStatus_t phlnLlcp_Sw_Int_Pdu_Snl(uint8_t bTid, uint8_t bSap);

phStatus_t phlnLlcp_Sw_Int_Pdu_RrOrRnr(phlnLlcp_Transport_Socket_t *psSocket,
                                       uint8_t bPtype
                                       );

phStatus_t phlnLlcp_Sw_Int_Pdu_Process(phlnLlcp_Sw_DataParams_t * pDataParams,
                                       uint8_t *pRxBuffer,
                                       uint32_t dwLength,
                                       uint8_t epType,
                                       phlnLlcp_Transport_Socket_t **ppsSocket
                                       );

uint16_t phlnLlcp_Sw_Int_PostEvents(phStatus_t wProcessStatus,
                                    uint8_t epType,
                                    phlnLlcp_Transport_Socket_t *psSocket,
                                    uint8_t *pRxData,
                                    uint8_t bVersion);

phStatus_t phlnLlcp_Sw_Int_Pdu_Handle(phStatus_t wProcessStatus,
                                      phlnLlcp_Transport_Socket_t *psSocket,
                                      uint8_t *pRxData,
                                      uint16_t wLength
                                      );
#if 0
phStatus_t phlnLlcp_Sw_Int_ParseRxFrame(uint8_t *pRxBuffer,
                                        uint32_t dwLength,
                                        uint8_t *pbPtype,
                                        phlnLlcp_Transport_Socket_t **ppsSocket
                                        );
#endif
/**
* Search for the registered socket using its bLsap & bRsap.
* If it is a server based socket and not yet connected to any remote client then the bRsap will be
* ignored by setting bIgnoreRsap.
*/
phlnLlcp_Transport_Socket_t *phlnLlcp_Transport_Socket_Search(uint8_t bLsap,
                                                              uint8_t bRsap,
                                                              uint8_t bIgnoreRsap
                                                              );
/**
* Search for the registered socket using Uri
*/
phlnLlcp_Transport_Socket_t *phlnLlcp_Transport_Socket_SearchUri(uint8_t *pUri,
                                                                 uint8_t bUriLen);

phStatus_t phlnLlcp_Sw_Int_HandleMsgQueue(uint8_t *pBuffer, uint16_t wLen, uint8_t bType);

phStatus_t phlnLlcp_Sw_Int_PostRxMsgQueue(uint8_t *pRxBuffer,
                                          uint16_t wRxLen);

#endif /* PHLNLLCP_SW_INT_H */
