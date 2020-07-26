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
* Internal functions for Tag Operation Application Layer Component of
* Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/
#include <ph_Status.h>
#include <phalT1T.h>
#include <phpalI14443p3a.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_T1T_SW
#include "phalT1T_Sw.h"

phStatus_t phalT1T_Sw_Init(
                           phalT1T_Sw_DataParams_t * pDataParams,
                           uint16_t wSizeOfDataParams,
                           void * pPalI14443p3aDataParams
                           )
{
    if (sizeof(phalT1T_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_T1T);
    }

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPalI14443p3aDataParams);

    /* Initialize private data */
    pDataParams->wId = PH_COMP_AL_T1T | PHAL_T1T_SW_ID;
    pDataParams->pPalI14443p3aDataParams = pPalI14443p3aDataParams;

    return PH_ERR_SUCCESS;
}

phStatus_t phalT1T_Sw_ReadUID(
                              phalT1T_Sw_DataParams_t * pDataParams,
                              uint8_t * pUid,
                              uint16_t * pLength
                              )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bCountLoop;
    uint8_t *  PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wTxLength = 7;
    uint16_t   PH_MEMLOC_REM wUIDLength = 4;
    uint8_t    PH_MEMLOC_REM abTxBuffer[] = {PHAL_T1T_CMD_READUID, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00};

    /* Set command timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_SetConfig(
        pDataParams->pPalI14443p3aDataParams,
        PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US,
        PHAL_T1T_SW_READ_TIME_US + PHAL_T1T_SW_EXT_TIME_US));

    /* Send and receive the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_Exchange(
        pDataParams->pPalI14443p3aDataParams,
        PH_EXCHANGE_DEFAULT,
        &abTxBuffer[0],
        wTxLength,
        &pRxBuffer,
        pLength));

    if(((phpalI14443p3a_Sw_DataParams_t*)(pDataParams->pPalI14443p3aDataParams))->bOpeMode == RD_LIB_MODE_NFC)
    {
        /* Check received length. NFC Digital Protocol 1.1-10.6.2.1 */
        if ((*pLength != PHAL_T1T_READUID_RESP_LEN) ||
            ((pRxBuffer[0] & PHAL_T1T_HEADER_ROM0) != PHAL_T1T_HEADER_ROM0))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
        }
    }
    else
    {
        if (*pLength != PHAL_T1T_READUID_RESP_LEN)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
        }
    }

    /* Update the Header ROM bytes */
    pDataParams->abHR[0] = pRxBuffer[0];
    pDataParams->abHR[1] = pRxBuffer[1];

    /* Update the UID */
    for(bCountLoop = 0; bCountLoop < 4U; bCountLoop++)
    {
        pDataParams->abUid[bCountLoop] = pRxBuffer[bCountLoop+2U];
        pUid[bCountLoop] = pDataParams->abUid[bCountLoop];
    }

    (*pLength) = wUIDLength;   /* UID length */

    return PH_ERR_SUCCESS;
}

phStatus_t phalT1T_Sw_ReadAll(
                              phalT1T_Sw_DataParams_t * pDataParams,
                              uint8_t * pUid,
                              uint8_t ** pData,
                              uint16_t * pLength
                              )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bCountLoop;
    uint8_t    PH_MEMLOC_REM abTxBuffer[10];
    uint16_t   PH_MEMLOC_REM wTxLength = 7;

    abTxBuffer[0] = PHAL_T1T_CMD_READALL;   /* Command */
    abTxBuffer[1] = 0x00;                   /* Address */
    abTxBuffer[2] = 0x00;                   /* Data */

    for(bCountLoop = 3; bCountLoop < 7U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = (*pUid);
        pUid++;
    }

    /* Set command timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_SetConfig(
        pDataParams->pPalI14443p3aDataParams,
        PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US,
        PHAL_T1T_SW_READ_TIME_US + PHAL_T1T_SW_EXT_TIME_US));

    /* Send and receive the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_Exchange(
        pDataParams->pPalI14443p3aDataParams,
        PH_EXCHANGE_DEFAULT,
        &abTxBuffer[0],
        wTxLength,
        pData,
        pLength));

    /* Check received length */
    if (*pLength != PHAL_T1T_READALL_RESP_LEN)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalT1T_Sw_ReadByte(
                               phalT1T_Sw_DataParams_t * pDataParams,
                               uint8_t * pUid,
                               uint8_t bAddress,
                               uint8_t * pData,
                               uint16_t * pLength
                               )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bCountLoop;
    uint8_t    PH_MEMLOC_REM abTxBuffer[10];
    uint8_t *  PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wTxLength = 7;

    abTxBuffer[0] = PHAL_T1T_CMD_READBYTE;         /* Command */
    abTxBuffer[1] = bAddress;                      /* Address */
    abTxBuffer[2] = 0x00;                          /* Data */

    for(bCountLoop = 3; bCountLoop < 7U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = (*pUid);
        pUid++;
    }

    /* Set command timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_SetConfig(
        pDataParams->pPalI14443p3aDataParams,
        PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US,
        PHAL_T1T_SW_READ_TIME_US + PHAL_T1T_SW_EXT_TIME_US));

    /* Send and receive the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_Exchange(
        pDataParams->pPalI14443p3aDataParams,
        PH_EXCHANGE_DEFAULT,
        &abTxBuffer[0],
        wTxLength,
        &pRxBuffer,
        pLength));

    /* Check received length */
    if (*pLength != PHAL_T1T_READBYTE_RESP_LEN)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    (void)memcpy(&pData[0], pRxBuffer, *pLength);

    return PH_ERR_SUCCESS;
}

phStatus_t phalT1T_Sw_WriteEraseByte(
                                     phalT1T_Sw_DataParams_t * pDataParams,
                                     uint8_t * pUid,
                                     uint8_t bAddress,
                                     uint8_t bTxData,
                                     uint8_t * pRxData,
                                     uint16_t * pLength
                                     )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bCountLoop;
    uint8_t    PH_MEMLOC_REM abTxBuffer[10];
    uint8_t *  PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wTxLength = 7;

    abTxBuffer[0] = PHAL_T1T_CMD_WRITEERASEBYTE;      /* Command */
    abTxBuffer[1] = bAddress;                         /* Address */
    abTxBuffer[2] = bTxData;                          /* Data */

    for(bCountLoop = 3; bCountLoop < 7U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = (*pUid);
        pUid++;
    }

    /* Set command timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_SetConfig(
        pDataParams->pPalI14443p3aDataParams,
        PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US,
        PHAL_T1T_SW_WRITE_ERASE_TIME_US + PHAL_T1T_SW_EXT_TIME_US));

    /* Send and receive the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_Exchange(
        pDataParams->pPalI14443p3aDataParams,
        PH_EXCHANGE_DEFAULT,
        &abTxBuffer[0],
        wTxLength,
        &pRxBuffer,
        pLength));

    /* Check received length */
    if (*pLength != PHAL_T1T_WRITEERASEBYTE_RESP_LEN)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    (void)memcpy(&pRxData[0], pRxBuffer, *pLength);

    if ((bAddress != pRxData[0]) || (bTxData != pRxData[1]))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalT1T_Sw_WriteNoEraseByte(
                                       phalT1T_Sw_DataParams_t * pDataParams,
                                       uint8_t * pUid,
                                       uint8_t bAddress,
                                       uint8_t bTxData,
                                       uint8_t * pRxData,
                                       uint16_t * pLength
                                       )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bCountLoop;
    uint8_t    PH_MEMLOC_REM abTxBuffer[10];
    uint8_t *  PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wTxLength = 7;

    abTxBuffer[0] = PHAL_T1T_CMD_WRITENOERASEBYTE;   /* Command */
    abTxBuffer[1] = bAddress;                        /* Address */
    abTxBuffer[2] = bTxData;                         /* Data */

    for(bCountLoop = 3; bCountLoop < 7U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = (*pUid);
        pUid++;
    }

    /* Set command timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_SetConfig(
        pDataParams->pPalI14443p3aDataParams,
        PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US,
        PHAL_T1T_SW_WRITE_NO_ERASE_TIME_US + PHAL_T1T_SW_EXT_TIME_US));

    /* Send and receive the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_Exchange(
        pDataParams->pPalI14443p3aDataParams,
        PH_EXCHANGE_DEFAULT,
        &abTxBuffer[0],
        wTxLength,
        &pRxBuffer,
        pLength));

    /* Check received length */
    if (*pLength != PHAL_T1T_WRITENOERASEBYTE_RESP_LEN)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    (void)memcpy(&pRxData[0], pRxBuffer, *pLength);

    if (bAddress != pRxData[0])
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalT1T_Sw_ReadSegment(
                                  phalT1T_Sw_DataParams_t * pDataParams,
                                  uint8_t * pUid,
                                  uint8_t bAddress,
                                  uint8_t ** pData,
                                  uint16_t * pLength
                                  )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bCountLoop;
    uint8_t    PH_MEMLOC_REM abTxBuffer[14];
    uint16_t   PH_MEMLOC_REM wTxLength = 14;

    abTxBuffer[0] = PHAL_T1T_CMD_READSEG;        /* Command */
    abTxBuffer[1] = bAddress;                    /* Address */

    for(bCountLoop = 2; bCountLoop < 10U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = 0x00;           /* Data */
    }

    for(; bCountLoop < 14U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = (*pUid);
        pUid++;
    }

    /* Set command timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_SetConfig(
        pDataParams->pPalI14443p3aDataParams,
        PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US,
        PHAL_T1T_SW_READ_TIME_US + PHAL_T1T_SW_EXT_TIME_US));

    /* Send and receive the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_Exchange(
        pDataParams->pPalI14443p3aDataParams,
        PH_EXCHANGE_DEFAULT,
        &abTxBuffer[0],
        wTxLength,
        pData,
        pLength));

    /* Check received length */
    if (*pLength != PHAL_T1T_READSEG_RESP_LEN)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalT1T_Sw_ReadBlock(
                                phalT1T_Sw_DataParams_t * pDataParams,
                                uint8_t * pUid,
                                uint8_t bAddress,
                                uint8_t * pData,
                                uint16_t * pLength
                                )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bCountLoop;
    uint8_t    PH_MEMLOC_REM abTxBuffer[14];
    uint8_t *  PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wTxLength = 14;

    abTxBuffer[0] = PHAL_T1T_CMD_READBLOCK;       /* Command */
    abTxBuffer[1] = bAddress;                     /* Address */

    for(bCountLoop = 2; bCountLoop < 10U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = 0x00;            /* Data */
    }

    for(; bCountLoop < 14U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = (*pUid);
        pUid++;
    }

    /* Set command timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_SetConfig(
        pDataParams->pPalI14443p3aDataParams,
        PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US,
        PHAL_T1T_SW_READ_TIME_US + PHAL_T1T_SW_EXT_TIME_US));

    /*Send and receive the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_Exchange(
        pDataParams->pPalI14443p3aDataParams,
        PH_EXCHANGE_DEFAULT,
        &abTxBuffer[0],
        wTxLength,
        &pRxBuffer,
        pLength));

    /* Check received length */
    if (*pLength != PHAL_T1T_READBLOCK_RESP_LEN)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    (void)memcpy(&pData[0], pRxBuffer, *pLength);

    return PH_ERR_SUCCESS;
}

phStatus_t phalT1T_Sw_WriteEraseBlock(
                                      phalT1T_Sw_DataParams_t * pDataParams,
                                      uint8_t * pUid,
                                      uint8_t bAddress,
                                      uint8_t * pTxData,
                                      uint8_t * pRxData,
                                      uint16_t * pLength
                                      )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bCountLoop;
    uint8_t    PH_MEMLOC_REM abTxBuffer[14];
    uint8_t *  PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wTxLength = 14;

    abTxBuffer[0] = PHAL_T1T_CMD_WRITEERASEBLOCK;         /* Command */
    abTxBuffer[1] = bAddress;                             /* Address */

    for(bCountLoop = 2U; bCountLoop < 10U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = pTxData[bCountLoop-2U];   /* Data */
    }

    for(; bCountLoop < 14U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = (*pUid);
        pUid++;
    }

    /* Set command timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_SetConfig(
        pDataParams->pPalI14443p3aDataParams,
        PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US,
        PHAL_T1T_SW_WRITE_ERASE_TIME_US + PHAL_T1T_SW_EXT_TIME_US));

    /* Send and receive the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_Exchange(
        pDataParams->pPalI14443p3aDataParams,
        PH_EXCHANGE_DEFAULT,
        &abTxBuffer[0],
        wTxLength,
        &pRxBuffer,
        pLength));

    /* Check received length */
    if (*pLength != PHAL_T1T_WRITEERASEBLOCK_RESP_LEN)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    (void)memcpy(&pRxData[0], pRxBuffer, *pLength);

    /* Verify address of the received data */
    if (bAddress != pRxData[0])
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    /* Verify the data written */
    for(bCountLoop = 1U; bCountLoop < 9U; bCountLoop++)
    {
        if (pTxData[bCountLoop-1U] != pRxData[bCountLoop])
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalT1T_Sw_WriteNoEraseBlock(
                                        phalT1T_Sw_DataParams_t * pDataParams,
                                        uint8_t * pUid,
                                        uint8_t bAddress,
                                        uint8_t * pTxData,
                                        uint8_t * pRxData,
                                        uint16_t * pLength
                                        )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bCountLoop;
    uint8_t    PH_MEMLOC_REM abTxBuffer[14];
    uint8_t *  PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wTxLength = 14;

    abTxBuffer[0] = PHAL_T1T_CMD_WRITENOERASEBLOCK;        /* Command */
    abTxBuffer[1] = bAddress;                              /* Address */

    for(bCountLoop = 2U; bCountLoop < 10U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = pTxData[bCountLoop-2U];    /* Data */
    }

    for(; bCountLoop < 14U; bCountLoop++)
    {
        abTxBuffer[bCountLoop] = (*pUid);
        pUid++;
    }

    /* Set command timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_SetConfig(
        pDataParams->pPalI14443p3aDataParams,
        PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US,
        PHAL_T1T_SW_WRITE_NO_ERASE_TIME_US + PHAL_T1T_SW_EXT_TIME_US));

    /* Send and receive the data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3a_Exchange(
        pDataParams->pPalI14443p3aDataParams,
        PH_EXCHANGE_DEFAULT,
        &abTxBuffer[0],
        wTxLength,
        &pRxBuffer,
        pLength));

    /* Check received length */
    if (*pLength != PHAL_T1T_WRITENOERASEBLOCK_RESP_LEN)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    (void)memcpy(&pRxData[0], pRxBuffer, *pLength);

    /* Verify address of the received data */
    if (bAddress != pRxData[0])
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_T1T);
    }

    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHAL_T1T_SW */
