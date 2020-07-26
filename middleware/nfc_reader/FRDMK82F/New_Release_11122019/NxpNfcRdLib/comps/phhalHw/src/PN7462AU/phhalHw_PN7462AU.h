/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2013
 *
 *         All rights are reserved. Reproduction in whole or in part is
 *        prohibited without the written consent of the copyright owner.
 *    NXP reserves the right to make changes without notice at any time.
 *   NXP makes no warranty, expressed, implied or statutory, including but
 *   not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 *   copyright or trademark. NXP must not be liable for any loss or damage
 *                            arising from its use.
 */

/**
 *
 * \file phhalHw_PN7462AU.h :  This file contains wrapper Api's which connects
 *                              the Reader library interfaces with the phhalRf interfaces.
 *
 * Project:  PN7462AU
 *
 * $Date$
 * $Author$
 * $Revision$ (v06.01.00)
 *
 */
#ifndef PHHALHW_PN7462AU_H
#define PHHALHW_PN7462AU_H


/*********************************************************************************************************************/
/*   INCLUDES                                                                                                        */
/*********************************************************************************************************************/
#include "ph_Status.h"
#include "ph_Datatypes.h"

/*********************************************************************************************************************/
/*   DFEINES                                                                                                         */
/*********************************************************************************************************************/
/**
 * AGC Threshold for low power tag detection
 */
#define PHHAL_HW_PN7462AU_LPCD_THRE_DEFAULT               0x0000FFFFU

/**
 * Reference value for low power card detection.
 */
#define PHHAL_HW_PN7462AU_LPCD_REF_DEFAULT                0x0000FFFFU

/*********************************************************************************************************************/
/*   PUBLIC FUNCTIONS PROTOTYPES                                                                                     */
/*********************************************************************************************************************/

/**
* \brief This api is used to set the configuration parameter.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER Configuration is not supported or invalid.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
* \retval #PH_ERR_PARAMETER_OVERFLOW Setting the parameter value would lead to an overflow.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phhalHw_PN7462AU_SetConfig(phhalHw_PN7462AU_DataParams_t * pDataParams, /**< [In] Data parameters */
                                      uint16_t wConfig,      /**< [In] Configuration Identifier. */
                                      uint16_t wValue        /**< [In] Configuration Value. */
                                      );

/**
* \brief This api is used to get the configuration parameter.
* \description This api gets several configurations of the PN7462AU Clif hardware.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INTERNAL_ERROR None of the configuration matches.
*/
phStatus_t phhalHw_PN7462AU_GetConfig(phhalHw_PN7462AU_DataParams_t * pDataParams, /**< [In] Data parameters */
                                      uint16_t wConfig, /**< [In] Configuration Identifier. */
                                      uint16_t* pValue  /**< [Out] Returning configuration Value. */
                                      );
/**
* \brief This api is used to switch on the RF field
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_FieldOn(phhalHw_PN7462AU_DataParams_t * pDataParams);  /**< [In] Data parameters */

/**
* \brief This api is used to switch off the RF field
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_FieldOff(phhalHw_PN7462AU_DataParams_t * pDataParams);   /**< [In] Data parameters */

/**
* \brief This api is used to apply the load protocol settings depending on the technology A, B , FeliCa or GTM etc
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_ApplyProtocolSettings(phhalHw_PN7462AU_DataParams_t * pDataParams,  /**< [In] Data parameters */
                                                  uint8_t bCardType            /**< [In] Technology Type to be applied. */
                                                  );

/**
* \brief This api is used to exchange a command with the counter part (Card or Reader)
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_Exchange(phhalHw_PN7462AU_DataParams_t * pDataParams,   /**< [In] Data parameters */
                                     uint16_t wOption,           /**< [In] Option for Exchange. */
                                     uint8_t * pTxBuffer,        /**< [In] Pointer to the transmit buffer. */
                                     uint16_t wTxLength,         /**< [In] Length of Data to Transmit. */
                                     uint8_t ** ppRxBuffer,      /**< [In] Pointer to the transmit buffer. */
                                     uint16_t * pRxLength
                                     );
/**
* \brief This api is used to authenticate the MIFARE product card
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_MfcAuthenticate(
                                   phhalHw_PN7462AU_DataParams_t * pDataParams,
                                   uint8_t bBlockNo,
                                   uint8_t bKeyType,
                                   uint8_t * pKey,
                                   uint8_t * pUid
                                   );


/**
* \brief This api is used to authenticate the MIFARE product card using a keystore
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_MfcAuthenticateKeyNo(
                                phhalHw_PN7462AU_DataParams_t * pDataParams,
                                uint8_t bBlockNo,
                                uint8_t bKeyType,
                                uint16_t wKeyNo,
                                uint16_t wKeyVersion,
                                uint8_t * pUid
                                );

/**
 * \brief This api is used to introduce wait
 * return Status code
 *\retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_Wait(phhalHw_PN7462AU_DataParams_t *pDataParams, uint8_t bUnit, uint16_t wTimeout);


/**
 * \brief Enables the card presence check
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_Lpcd(phhalHw_PN7462AU_DataParams_t * pDataParams);

/**
 * \brief Perform RF-Reset.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_FieldReset (phhalHw_PN7462AU_DataParams_t * pDataParams);


/**
* \brief This api is used to perform 18000p3m3 inventory
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_I18000p3m3Inventory(
    phhalHw_PN7462AU_DataParams_t * pDataParams,
    uint8_t * pSelCmd,
    uint8_t bSelCmdLen,
    uint8_t bNumValidBitsinLastByte,
    uint8_t * pBeginRndCmd,
    uint8_t bTSprocessing,
    uint8_t ** ppRxBuffer,
    uint16_t * wRxBufferLen
    );


/**
* \brief This api is used to perform 18000p3m3 resume inventory
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_I18000p3m3ResumeInventory(
    phhalHw_PN7462AU_DataParams_t * pDataParams,
    uint8_t ** ppRxBuffer,
    uint16_t * wRxBufferLen
    );


/**
* \brief This api is used to receive a command from the counter part (Card or Reader)
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_Receive(phhalHw_PN7462AU_DataParams_t * pDataParams, uint16_t wOption, uint8_t ** ppRxBuffer, uint16_t * pRxLength);


/**
* \brief This api is used to transmit a command to the counter part (Card or Reader)
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_Transmit(phhalHw_PN7462AU_DataParams_t * pDataParams, uint16_t wOption, uint8_t * pTxBuffer, uint16_t wTxLength);

/**
* \brief This api is used in listen mode to detect the reader presence and technology
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_Autocoll(phhalHw_PN7462AU_DataParams_t * pDataParams, uint16_t wMode, uint8_t ** ppRxBuffer, uint16_t * pRxLength, uint16_t * pProtParams);

/**
* \brief This api is used to abort the operation asynchronously
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_AsyncAbort(phhalHw_PN7462AU_DataParams_t *pDataParams);

/**
* \brief This api is used to deinitialise the hal rf component
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_PN7462AU_DeInit(phhalHw_PN7462AU_DataParams_t *pDataParams);

/**
 * \brief This api is used to set the Min FDT according to the baud rate
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_SetMinFDT(
        phhalHw_PN7462AU_DataParams_t * pDataParams,
        uint16_t wValue
        );

phStatus_t phhalHw_PN7462AU_EventWait(
    phhalHw_PN7462AU_DataParams_t * pDataParams,
    uint32_t dwEventTimeout
    );

phStatus_t phhalHw_PN7462AU_EventConsume(
    phhalHw_PN7462AU_DataParams_t * pDataParams
    );

#endif /* PHHALHW_PN7462AU_H */
