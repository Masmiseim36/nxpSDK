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
* Generic ISO14443-4A Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 29. May 2009
*
*/

#ifndef PHPALI14443P4A_H
#define PHPALI14443P4A_H

#include <ph_Status.h>
#include <phhalHw.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHPAL_I14443P4A_SW

/** \defgroup phpalI14443p4a ISO/IEC 14443-4A
* \brief These Components configure PHPAL_I14443P4A
* @{
*/

/**
* \name Configuration
*/
/*@{*/

/**
 * Configure Operation Mode for this Layer. \n
 * Possible values are\n
 *      #RD_LIB_MODE_EMVCO, \n
 *      #RD_LIB_MODE_NFC, \n
 *      #RD_LIB_MODE_ISO,\n
 * Default value is #RD_LIB_MODE_NFC.
 * */

#define PHPAL_I14443P4A_CONFIG_OPE_MODE         0x01U

/**
 * Configure retry count for RATS command.\n
 * Possible values are\n
 *      #PHPAL_I14443P4A_RATS_RETRY_MIN, \n
 *      #PHPAL_I14443P4A_RATS_RETRY_MAX, \n
 *
 * Default value is #PHPAL_I14443P4A_RATS_RETRY_MAX.\n
 *
 * For mode #RD_LIB_MODE_EMVCO retry count should be 1.
 * */
#define PHPAL_I14443P4A_CONFIG_RATS_RETRY_COUNT 0x02U

/*@}*/

/** @}
 *
 */
/** \defgroup phpalI14443p4a_Sw Component : Software
* @{
*/

#define PHPAL_I14443P4A_SW_ID   0x01U    /**< ID for Software ISO14443-4A layer     */

/**
* \brief Software PAL-ISO14443P4A parameter structure
*/
typedef struct
{
    uint16_t wId;           /**< Layer ID for this component, NEVER MODIFY! */
    void  * pHalDataParams; /**< Pointer to the parameter structure of the underlying layer. */
    uint8_t bCidSupported;  /**< Cid Support indicator; Unequal '0' if supported. */
    uint8_t bNadSupported;  /**< Nad Support indicator; Unequal '0' if supported. */
    uint8_t bCid;           /**< Card Identifier; Ignored if bCidSupported is equal '0'. */
    uint8_t bBitRateCaps;   /**< TA(1) byte of ATS; ISO/IEC 14443-4:2008, Section 5.2.4. */
    uint8_t bFwi;           /**< Frame Waiting Integer. */
    uint8_t bFsci;          /**< PICC Frame Size Integer; 0-12; */
    uint8_t bFsdi;          /**< (Current) PCD Frame Size Integer; 0-12; */
    uint8_t bDri;           /**< (Current) Divisor Receive (PCD to PICC) Integer; 0-3; */
    uint8_t bDsi;           /**< (Current) Divisor Send (PICC to PCD) Integer; 0-3; */
    uint8_t bOpeMode;       /**< Operation mode. One of NFC, EMVCo, ISO.*/
    uint8_t bRetryCount;    /**< Retry count for RATS command as per NFC Digital Protocol Version 1.1\n
                                 For mode #RD_LIB_MODE_EMVCO retry count should be 1. */
} phpalI14443p4a_Sw_DataParams_t;

/**
* \brief Initialize this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p4a_Sw_Init(
                                  phpalI14443p4a_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                  uint16_t wSizeOfDataParams,                   /**< [In] Specifies the size of the data parameter structure. */
                                  void * pHalDataParams                         /**< [In] Pointer to the parameter structure of the underlying layer.*/
                                  );

/** @} */
#endif /* NXPBUILD__PHPAL_I14443P4A_SW */




#ifdef NXPBUILD__PHPAL_I14443P4A

/** \defgroup phpalI14443p4a ISO/IEC 14443-4A
* \brief These Components implement the additional ISO/IEC 14443-4 activation steps
* needed for ISO14443 Type-A cards.
* @{
*/

/**
 * \name FSDI/FSCI Values
 */
/*@{*/
#define PHPAL_I14443P4A_NFC_FRAMESIZE_MAX 8U   /**< Maximum allowed FSDI/FSCI value as per NFC Forum Spec. */
/*@}*/

/**
* \name DRI/DSI Values
*/
/*@{*/
#define PHPAL_I14443P4A_DATARATE_106    0x00U   /**< DRI/DSI value for 106 kBit/s. */
#define PHPAL_I14443P4A_DATARATE_212    0x01U   /**< DRI/DSI value for 212 kBit/s. */
#define PHPAL_I14443P4A_DATARATE_424    0x02U   /**< DRI/DSI value for 424 kBit/s. */
#define PHPAL_I14443P4A_DATARATE_848    0x03U   /**< DRI/DSI value for 848 kBit/s. */
/*@}*/

/**
* \name RATS Command Retry Values
*/
/*@{*/
#define PHPAL_I14443P4A_RATS_RETRY_MAX    1U   /**< Maximum retry value of RATS during transmission and timeout error.*/
#define PHPAL_I14443P4A_RATS_RETRY_MIN    0U   /**< Minimum retry value of RATS during transmission and timeout error.*/
/*@}*/

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phpalI14443p4a/src/Sw/phpalI14443p4a_Sw.h"

#define phpalI14443p4a_Rats( pDataParams,  bFsdi, bCid, pAts) \
        phpalI14443p4a_Sw_Rats((phpalI14443p4a_Sw_DataParams_t *)pDataParams, bFsdi, bCid, pAts)

#define phpalI14443p4a_Pps( pDataParams,  bDri, bDsi ) \
        phpalI14443p4a_Sw_Pps((phpalI14443p4a_Sw_DataParams_t *)pDataParams, bDri, bDsi)

#define phpalI14443p4a_ActivateCard( pDataParams, bFsdi, bCid,bDri, bDsi, pAts) \
        phpalI14443p4a_Sw_ActivateCard((phpalI14443p4a_Sw_DataParams_t *)pDataParams, bFsdi, bCid, bDri, bDsi, pAts)

#define phpalI14443p4a_GetProtocolParams( pDataParams,  pCidEnabled, pCid, pNadSupported,  pFwi,  pFsdi, pFsci ) \
        phpalI14443p4a_Sw_GetProtocolParams((phpalI14443p4a_Sw_DataParams_t *)pDataParams, pCidEnabled, pCid, pNadSupported, pFwi, pFsdi, pFsci)

#define phpalI14443p4a_SetConfig(pDataParams, wConfig,wValue) phpalI14443p4a_Sw_SetConfig((phpalI14443p4a_Sw_DataParams_t *)pDataParams, wConfig,wValue)

#else
/**
* \brief Perform a ISO14443-4A "Request Answer to Select" command.
*
\verbatim
[FSDI to FSD conversion]
---------------------------------------------------------------
FSDI         | 00 | 01 | 02 | 03 | 04 | 05 | 06 | 07  | 08  |
---------------------------------------------------------------
FSD (bytes)  | 16 | 24 | 32 | 40 | 48 | 64 | 96 | 128 | 256 |
---------------------------------------------------------------
\endverbatim
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4a_Rats(
                               void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bFsdi,       /**< [In] Frame Size Integer; 0-12. */
                               uint8_t bCid,        /**< [In] Card Identifier; 0-14. */
                               uint8_t * pAts       /**< [Out] Answer to Select; uint8_t[255]. */
                               );

/**
* \brief Perform a ISO14443-4A "Protocol and Parameter Selection" command.
*
* The following values for \c bDri and \c bDsi are possible:\n
* \li #PHPAL_I14443P4A_DATARATE_106
* \li #PHPAL_I14443P4A_DATARATE_212
* \li #PHPAL_I14443P4A_DATARATE_424
* \li #PHPAL_I14443P4A_DATARATE_848
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4a_Pps(
                              void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                              uint8_t bDri,         /**< [In] Divisor Receive (PCD to PICC) Integer; 0-3. */
                              uint8_t bDsi          /**< [In] Divisor Send (PICC to PCD) Integer; 0-3. */
                              );

/**
* \brief Perform ISO14443-4A Rats and Pps commands.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4a_ActivateCard(
                                       void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                       uint8_t bFsdi,       /**< [In] Frame Size Integer; 0-12. */
                                       uint8_t bCid,        /**< [In] Card Identifier; 0-14. */
                                       uint8_t bDri,        /**< [In] Divisor Receive (PCD to PICC) Integer; 0-3. */
                                       uint8_t bDsi,        /**< [In] Divisor Send (PICC to PCD) Integer; 0-3. */
                                       uint8_t * pAts       /**< [Out] Answer to Select; uint8_t[255]. */
                                       );

/**
* \brief Retrieve the ISO14443-4A protocol parameters.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p4a_GetProtocolParams(
    void * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pCidEnabled,      /**< [Out] Unequal '0' if Card Identifier is enabled. */
    uint8_t * pCid,             /**< [Out] Card Identifier. */
    uint8_t * pNadSupported,    /**< [Out] Node Address Support; Unequal '0' if supported. */
    uint8_t * pFwi,             /**< [Out] Frame Waiting Integer. */
    uint8_t * pFsdi,            /**< [Out] PCD Frame Size Integer; 0-12. */
    uint8_t * pFsci             /**< [Out] PICC Frame Size Integer; 0-12. */
    );

/**
* \brief Set configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4a_SetConfig(
                                    void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                                    uint16_t wConfig,   /**< [In] Configuration Identifier. */
                                    uint16_t wValue     /**< [In] Configuration Value. */
                                   );
/** @} */
#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHPAL_I14443P4A */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHPALI14443P4A_H */
