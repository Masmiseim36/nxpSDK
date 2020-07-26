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
* Software ISO 14443p4mC mode Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 25. March 2014
*/

#ifndef PHPALI14443P4MC_SW_H
#define PHPALI14443P4MC_SW_H

#include <phpalI14443p4mC.h>

/**
* \brief Reset protocol parameters.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p4mC_Sw_ResetProtocol(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams           /**< [In] Pointer to this layer's parameter structure */
    );

/**
* \brief Set Configuration.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
*/
phStatus_t phpalI14443p4mC_Sw_SetConfig(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams,          /**< [In] Pointer to this layer's parameter structure */
    uint16_t wConfig,                                       /**< [In] Configuration Identifier. */
    uint16_t wValue                                         /**< [In] Configuration Value. */
    );

/**
* \brief Get Configuration.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
*/
phStatus_t phpalI14443p4mC_Sw_GetConfig(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams,          /**< [In] Pointer to this layer's parameter structure */
    uint16_t wConfig,                                       /**< [In] Configuration Identifier. */
    uint16_t *pValue                                        /**< [Out]Configuration Value. */
    );

/**
* \brief Validates received RATS request and send ATS response.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Protocol Error.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalI14443p4mC_Sw_Activate(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams,          /**< [In] Pointer to this layer's parameter structure */
    uint8_t * pRats,                                        /**< [In] Pointer to RATS receive Buffer */
    uint8_t  bRatsLength,                                   /**< [In] RATS receive Buffer Length*/
    uint8_t * pAts,                                         /**< [In] ATS to be sent */
    uint16_t  wAtsLength                                    /**< [In] ATS length */
    );

/**
* \brief Perform receive operation and validate received block.
*
* Response to S-block and R-Block are send internally. Response to I-Block can
* be send using \ref phpalI14443p4mC_Transmit.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4mC_Sw_Receive(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams,          /**< [In] Pointer to this layer's parameter structure */
    uint16_t wOption,                                       /**< [In] Receive option */
    uint8_t ** ppRxBuffer,                                  /**< [Out] Pointer to receive Buffer */
    uint16_t * pRxLength                                    /**< [Out] Pointer to receive Buffer Length */
    );

/**
* \brief Transmit I-Block block
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4mC_Sw_Transmit(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams,          /**< [In] Pointer to this layer's parameter structure */
    uint16_t wOption,                                       /**< [In] Transmit option */
    uint8_t * pTxBuffer,                                    /**< [In] Pointer to Transmit Buffer */
    uint16_t  wTxLength                                     /**< [In] Transmit Buffer Length */
    );

/**
* \brief Send WTX request and wait for WTX response.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Protocol Error.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4mC_Sw_Wtx(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams           /**< [In] Pointer to this layer's parameter structure */
    );
#endif /* PHPALI14443P4MC_SW_H */
