/*
*                     Copyright 2010-2019, NXP
*
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
* Internal functions of Software implementation of MIFARE DESFire EV1 contactless IC
* Application layer.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHALMFDF_SW_INT_H
#define PHALMFDF_SW_INT_H

phStatus_t phalMfdf_Sw_Int_SendDataToPICC(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t * pCmd,
    uint16_t wCmdLen,
    uint8_t * pData,
    uint16_t wDataLen,
    uint8_t * pResp,
    uint16_t * pRespLen
    );


phStatus_t phalMfdf_Sw_Int_ReadData_Plain(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t ** ppRxdata,
    uint16_t * pRxdataLen
    );


phStatus_t phalMfdf_Sw_Int_Write_Plain(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint8_t * bCmdBuff,
                                       uint16_t wCmdLen,
                                       uint8_t bCommOption,
                                       uint8_t * pData,
                                       uint16_t  wDataLen
                                       );

phStatus_t phalMfdf_Sw_Int_IsoRead(
                                   phalMfdf_Sw_DataParams_t * pDataParams,
                                   uint16_t wOption,
                                   uint8_t * bCmdBuff,
                                   uint16_t wCmdLen,
                                   uint8_t ** ppRxBuffer,
                                   uint16_t * pBytesRead
                                   );
/* QAC_Fix: Rule  QL1:MISRA.8.1
 * Function Decl added
 */
phStatus_t phalMfdf_Sw_Int_GetData(
                                   phalMfdf_Sw_DataParams_t * pDataParams,
                                   uint8_t * pSendBuff,
                                   uint16_t wCmdLen,
                                   uint8_t ** pResponse,
                                   uint16_t * pRxlen
                                   );
#endif /* PHALMFDF_SW_INT_H */
