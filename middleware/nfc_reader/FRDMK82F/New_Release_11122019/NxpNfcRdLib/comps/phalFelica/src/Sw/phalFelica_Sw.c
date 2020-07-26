/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
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
* Software FeliCa application layer of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  RSn: Generated 26. January 2010
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalFelica.h>
#include <phalFelica.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_FELICA_SW

#include "phalFelica_Sw.h"
#include "../phalFelica_Int.h"

phStatus_t phalFelica_Sw_Init(
                              phalFelica_Sw_DataParams_t * pDataParams,
                              uint16_t wSizeOfDataParams,
                              void * pPalFelica_DataParams
                              )
{
    /* parameter structure length check */
    if (sizeof(phalFelica_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_FELICA);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPalFelica_DataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_FELICA | PHAL_FELICA_SW_ID;
    pDataParams->pPalFelicaDataParams   = pPalFelica_DataParams;
    pDataParams->wAdditionalInfo        = 0x0000;

    return PH_ERR_SUCCESS;
}


phStatus_t phalFelica_Sw_RequestResponse(
    phalFelica_Sw_DataParams_t * pDataParams,
    uint8_t * pMode
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1];
    uint8_t     PH_MEMLOC_REM *pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* Build the command frame */
    aCmd[0] = PHAL_FELICA_CMD_REQUEST_RESPONSE;

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_DEFAULT,
        0,
        aCmd,
        1,
        &pRxBuffer,
        &wRespLength));

    /* Check the length */
    if (wRespLength != 1U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_FELICA);
    }

    /* Note: The IDm has already been checked by the phpalFelica_Exchange function. */
    /* Success */
    *pMode = pRxBuffer[0];
    return PH_ERR_SUCCESS;
}

phStatus_t phalFelica_Sw_RequestService(
                                        phalFelica_Sw_DataParams_t * pDataParams,
                                        uint8_t  bTxNumServices,
                                        uint8_t * pTxServiceList,
                                        uint8_t * pRxNumServices,
                                        uint8_t * pRxServiceList
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[2];
    uint8_t     PH_MEMLOC_REM *pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* Parameter check */
    if (bTxNumServices < 1U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_FELICA);
    }

    /* Build the command frame */
    /* send the command frame fragmented as following:
    First send [cmd][num_of_srvcs]
    Then send the service code list in 2-byte chunks since a single service code element
    consists of 2 bytes. */
    aCmd[0] = PHAL_FELICA_CMD_REQUEST_SERVICE;
    aCmd[1] = bTxNumServices;

    /* Exchange command ... */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bTxNumServices,
        aCmd,
        2,
        &pRxBuffer,
        &wRespLength));

    /* ... and the service code (or area code) list. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bTxNumServices,
        pTxServiceList,
        (uint16_t)(((uint16_t)bTxNumServices) * 2U),
        &pRxBuffer,
        &wRespLength));

    /* Check the length (number of services and service list)*/
    if ((wRespLength < 1U) || (wRespLength != ((((uint16_t)(pRxBuffer[0])) * 2U) + 1U)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_FELICA);
    }

    /* Success */
    *pRxNumServices = pRxBuffer[0];
    (void)memcpy(pRxServiceList, &pRxBuffer[1], (((uint32_t)*pRxNumServices) * 2U));

    return PH_ERR_SUCCESS;
}

phStatus_t phalFelica_Sw_Read(
                              phalFelica_Sw_DataParams_t * pDataParams,
                              uint8_t  bNumServices,
                              uint8_t * pServiceList,
                              uint8_t  bTxNumBlocks,
                              uint8_t * pBlockList,
                              uint8_t  bBlockListLength,
                              uint8_t * pRxNumBlocks,
                              uint8_t * pBlockData
                              )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[2];
    uint8_t     PH_MEMLOC_REM *pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* check correct number of services / blocks */
    if ((bNumServices < 1U) || (bTxNumBlocks < 1U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_FELICA);
    }

    /* check blocklistlength against numblocks */
    if ((bBlockListLength < (bTxNumBlocks * 2U))  || (bBlockListLength > (bTxNumBlocks * 3U)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_FELICA);
    }

    /* Exchange command and the number of services ... */
    aCmd[0] = PHAL_FELICA_CMD_READ;
    aCmd[1] = bNumServices;
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bTxNumBlocks,
        aCmd,
        2,
        &pRxBuffer,
        &wRespLength));

    /* ... the service code list ... */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        bTxNumBlocks,
        pServiceList,
        (uint16_t)(((uint16_t)bNumServices) * 2U),
        &pRxBuffer,
        &wRespLength));

    /* ... the number of blocks ... */
    aCmd[0] = bTxNumBlocks;
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        bTxNumBlocks,
        aCmd,
        1,
        &pRxBuffer,
        &wRespLength));

    /* ... and the block list. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bTxNumBlocks,
        pBlockList,
        bBlockListLength,
        &pRxBuffer,
        &wRespLength));

    /* Check response. */
    /* Length: We expect:
    Status flags:       2 bytes
    Number of blocks:   1 byte
    Block data:         16 x number of blocks bytes */

    /* at least 2 status bytes have to be in the rx buffer */
    if (wRespLength < 2U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_FELICA);
    }

    /* on a FeliCa error save the status flags */
    if (pRxBuffer[0] != 0U)
    {
        if (wRespLength != 2U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_FELICA);
        }
        pDataParams->wAdditionalInfo = ((uint16_t)pRxBuffer[0] << 8U) | (uint16_t)pRxBuffer[1];
        return PH_ADD_COMPCODE_FIXED(PHAL_FELICA_ERR_FELICA, PH_COMP_AL_FELICA);
    }

    if (wRespLength != (3u + (16u * ((uint16_t)(pRxBuffer[2])))))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_FELICA);
    }

    *pRxNumBlocks = pRxBuffer[2];
    (void)memcpy(pBlockData, &pRxBuffer[3], 16U * ((uint32_t)bTxNumBlocks));

    return PH_ERR_SUCCESS;
}

phStatus_t phalFelica_Sw_Write(
                               phalFelica_Sw_DataParams_t * pDataParams,
                               uint8_t  bNumServices,
                               uint8_t * pServiceList,
                               uint8_t  bNumBlocks,
                               uint8_t * pBlockList,
                               uint8_t  bBlockListLength,
                               uint8_t * pBlockData
                               )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[2];
    uint8_t     PH_MEMLOC_REM *pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* check correct number of services / blocks */
    if ((bNumServices < 1U) || (bNumBlocks < 1U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_FELICA);
    }

    /* check blocklistlength against numblocks */
    if ((bBlockListLength < (bNumBlocks * 2U))  || (bBlockListLength > (bNumBlocks * 3U)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_FELICA);
    }

    /* Exchange command and the number of services ... */
    aCmd[0] = PHAL_FELICA_CMD_WRITE;
    aCmd[1] = bNumServices;
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bNumBlocks,
        aCmd,
        2,
        &pRxBuffer,
        &wRespLength));

    /* ... the service code list ... */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        bNumBlocks,
        pServiceList,
        (uint16_t)(((uint16_t)bNumServices)*2U),
        &pRxBuffer,
        &wRespLength));

    aCmd[0] = bNumBlocks;

    /* ... the number of blocks ... */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        bNumBlocks,
        aCmd,
        1,
        &pRxBuffer,
        &wRespLength));

    /* ... the block list ... */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        bNumBlocks,
        pBlockList,
        bBlockListLength,
        &pRxBuffer,
        &wRespLength));

    /* ... and the block data. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalFelica_Exchange(
        pDataParams->pPalFelicaDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bNumBlocks,
        pBlockData,
        (uint16_t)(((uint16_t)bNumBlocks) * 16U),
        &pRxBuffer,
        &wRespLength));

    /* We should have received 2 status bytes */
    if (wRespLength != 2U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_FELICA);
    }

    /* save the status on an error */
    if (pRxBuffer[0] != 0U)
    {
        pDataParams->wAdditionalInfo = ((uint16_t)pRxBuffer[0] << 8U) | (uint16_t)pRxBuffer[1];
        return PH_ADD_COMPCODE_FIXED(PHAL_FELICA_ERR_FELICA, PH_COMP_AL_FELICA);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalFelica_Sw_GetConfig(
                                   phalFelica_Sw_DataParams_t * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t * pValue
                                   )
{
    switch (wConfig)
    {
    case PHAL_FELICA_CONFIG_ADD_INFO:
        *pValue = pDataParams->wAdditionalInfo;
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_FELICA);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalFelica_Sw_ActivateCard(
                                       phalFelica_Sw_DataParams_t* pDataParams,
                                       uint8_t * pSystemCode,
                                       uint8_t bNumTimeSlots,
                                       uint8_t * pRxBuffer,
                                       uint8_t * pRxLength,
                                       uint8_t * pMoreCardsAvailable
                                       )
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_REM *pIDmPMm = NULL;

    *pRxLength = 0;
    *pMoreCardsAvailable = 0;

    /* RequestC == ReqC command (not requesting the system code) */
    /* In the case of a collision repeat the ReqC up to 16 slots. */

    PH_CHECK_SUCCESS_FCT(status, phpalFelica_ActivateCard(
            pDataParams->pPalFelicaDataParams,
            pIDmPMm,
            0x00,
            pSystemCode,
            bNumTimeSlots,
            pRxBuffer,
            pRxLength,
            pMoreCardsAvailable
            ));

    return status;
}

#endif /* NXPBUILD__PHAL_FELICA_SW */
