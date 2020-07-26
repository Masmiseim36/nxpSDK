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
* Software MIFARE DESFire EV1 contactless IC Application Component of Reader
* Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHALMFDF_SW_H
#define PHALMFDF_SW_H


/*
*  CreateApplication
*/
phStatus_t phalMfdf_Sw_CreateApplication(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * pAid,
    uint8_t bKeySettings1,
    uint8_t bKeySettings2,
    uint8_t * pISOFileId,
    uint8_t * pISODFName,
    uint8_t bISODFNameLen
    );


/*
*  SelectApplication
*/
phStatus_t phalMfdf_Sw_SelectApplication(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t * pAppId
    );


/*
*  GetVersion
*/
phStatus_t phalMfdf_Sw_GetVersion(
                                  phalMfdf_Sw_DataParams_t * pDataParams,
                                  uint8_t * pResponse
                                  );


/*
*
*  CreateStdDataFile
*/
phStatus_t phalMfdf_Sw_CreateStdDataFile(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t  * pISOFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pFileSize
    );


/*
*  WriteData
*/
phStatus_t phalMfdf_Sw_WriteData(
                                 phalMfdf_Sw_DataParams_t * pDataParams,
                                 uint8_t bCommOption,
                                 uint8_t bFileNo,
                                 uint8_t * pOffset,
                                 uint8_t * pData,
                                 uint8_t * pDataLen
                                 );


/*
*  ISO Select
*/
phStatus_t phalMfdf_Sw_IsoSelectFile(
                                     phalMfdf_Sw_DataParams_t * pDataParams,
                                     uint8_t bOption,
                                     uint8_t bSelector,
                                     uint8_t * pFid,
                                     uint8_t * pDFname,
                                     uint8_t bDFnameLen,
                                     uint8_t ** ppRecv,
                                     uint16_t * pwRxlen
                                     );

/*
*  ISO Read Binary
*/
phStatus_t phalMfdf_Sw_IsoReadBinary(
                                     phalMfdf_Sw_DataParams_t * pDataParams,
                                     uint16_t wOption,
                                     uint8_t bOffset,
                                     uint8_t bSfid,
                                     uint8_t bBytesToRead,
                                     uint8_t ** ppRxBuffer,
                                     uint16_t * pBytesRead
                                     );

/*
*  ISO Update Binary
*/
phStatus_t phalMfdf_Sw_IsoUpdateBinary(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint8_t bOffset,
                                       uint8_t bSfid,
                                       uint8_t * pData,
                                       uint8_t bDataLen
                                       );


phStatus_t phalMfdf_Sw_GetConfig(
                                 phalMfdf_Sw_DataParams_t * pDataParams,
                                 uint16_t wConfig,
                                 uint16_t * pValue
                                 );

phStatus_t phalMfdf_Sw_SetConfig(
                                 phalMfdf_Sw_DataParams_t *pDataParams,
                                 uint16_t wConfig,
                                 uint16_t wValue
                                 );


#endif /* PHALMFDF_SW_H */
