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
* Software ISO18092 target mode Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 27. October 2014
*/

#ifndef PHPALI18092MT_SW_H
#define PHPALI18092MT_SW_H


#include <phpalI18092mT.h>


phStatus_t phpalI18092mT_Sw_ResetProtocol(
    phpalI18092mT_Sw_DataParams_t * pDataParams
    );

phStatus_t phpalI18092mT_Sw_DepRecv(
        phpalI18092mT_Sw_DataParams_t * pDataParams,
        uint16_t wOption,
        uint8_t ** ppRxBuffer,
        uint16_t * pRxLength);

phStatus_t phpalI18092mT_Sw_DslRes(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pDslReq,
    uint8_t wDslReqLength
    );

phStatus_t phpalI18092mT_Sw_PslRes(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pPslReq,
    uint16_t  wPslReqLength,
    uint16_t  wActiveMode
    );

phStatus_t phpalI18092mT_Sw_RlsRes(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pRlsReq,
    uint16_t wRlsReqLength
    );

phStatus_t phpalI18092mT_Sw_WupRes(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pRlsReq,
    uint16_t wRlsReqLength,
    uint16_t  wActiveMode
    );

phStatus_t phpalI18092mT_Sw_Activate(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pAtrReq,
    uint16_t wAtrReqLength,
    uint8_t * pAtrRes,
    uint16_t wAtrResLength,
    uint8_t **pGeneralDataBytes,
    uint16_t *wGeneralDataBytesLength
    );

phStatus_t phpalI18092mT_Sw_Transmit(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint16_t   wOption,
    uint8_t * pTransportDataBytes,
    uint16_t   wTransportDataBytesLength
    );

phStatus_t phpalI18092mT_Sw_Receive(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint16_t   wOption,
    uint8_t ** ppTransportDataBytes,
    uint16_t * pTransportDataBytesLength
    );

phStatus_t phpalI18092mT_Sw_Rtox(
    phpalI18092mT_Sw_DataParams_t * pDataParams
    );

phStatus_t phpalI18092mT_Sw_SetConfig(
   phpalI18092mT_Sw_DataParams_t * pDataParams,
   uint16_t wConfig,
   uint16_t wValue
   );

phStatus_t phpalI18092mT_Sw_GetConfig(
   phpalI18092mT_Sw_DataParams_t * pDataParams,
   uint16_t wConfig,
   uint16_t * pValue
   );

#endif  /* PHPALI18092MT_SW_H */
