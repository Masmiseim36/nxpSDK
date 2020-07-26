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
* Generic ISO14443-4 Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHPALI14443P4_H
#define PHPALI14443P4_H

#include <ph_Status.h>
#include <phhalHw.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHPAL_I14443P4_SW

/** \defgroup phpalI14443p4_Sw Component : Software
* @{
*/

#define PHPAL_I14443P4_SW_ID        0x01U    /**< ID for Software ISO14443-4 layer */

/**
* \brief ISO14443-4 parameter structure
*/
typedef struct
{
    uint16_t  wId;              /**< Layer ID for this component, NEVER MODIFY! */
    void    * pHalDataParams;   /**< Pointer to the parameter structure of the underlying layer. */
    uint8_t   bStateNow;        /**< Current Exchange-State. */
    uint8_t   bCidEnabled;      /**< Card Identifier Enabler; Unequal '0' if enabled. */
    uint8_t   bCid;             /**< Card Identifier; Ignored if *pCidSupported is equal '0'. */
    uint8_t   bNadEnabled;      /**< Node Address Enabler; Unequal '0' if enabled. */
    uint8_t   bNad;             /**< Node Address; Ignored if bNadEnabled is equal '0'. */
    uint8_t   bFwi;             /**< Frame Waiting Integer. */
    uint8_t   bFsdi;            /**< PCD Frame Size Integer; 0-12; */
    uint8_t   bFsci;            /**< PICC Frame Size Integer; 0-12; */
    uint8_t   bPcbBlockNum;     /**< Current Block-Number; 0/1; */
    uint8_t   bMaxRetryCount;   /**< Maximum Retry count for ISO/IEC 14443-4:2008(E) Rule 4 and 5. */
    uint8_t   bOpeMode;         /**< Operation mode. One of NFC, EMVCo, ISO. */
} phpalI14443p4_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p4_Sw_Init(
                                 phpalI14443p4_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                 uint16_t wSizeOfDataParams,                    /**< [In] Specifies the size of the data parameter structure. */
                                 void * pHalDataParams                          /**< [In] Pointer to the parameter structure of the underlying layer. */
                                 );

/** @} */
#endif /* NXPBUILD__PHPAL_I14443P4_SW */





#ifdef NXPBUILD__PHPAL_I14443P4

/** \addtogroup ph_Error
* phpalI14443p4 Custom Errors
* @{
*/

/** \name phpalI14443p4 Error Codes
*/
/*@{*/
#define PHPAL_I14443P4_ERR_RECOVERY_FAILED  ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0U)     /**< ISO14443-4 error recovery failed. */
/*@}*/

/** @} */

/** \defgroup phpalI14443p4 ISO/IEC 14443-4
* \brief These Components implement the ISO/IEC 14443-4:2008(E) contactless protocol.
* @{
*/

#define PHPAL_I14443P4_CID_MAX          14U  /**< The last valid CID. */
#define PHPAL_I14443P4_FWI_MAX          14U  /**< The maximum allowed FWI value. */
#define PHPAL_I14443P4_FRAMESIZE_MAX    12U   /**< The maximum allowed FSDI/FSCI value. */

/**
* \name ISO14443-4 Parameters
*/
/*@{*/
/**
* \brief Set / Get Blocknumber.
*/
#define PHPAL_I14443P4_CONFIG_BLOCKNO           0x0000U
/**
* \brief Set / Get Card Identifier.
*
* \c wValue is parsed as follows:
* \verbatim
* CidEnabled = (wValue & 0xFF00U) ? 1 : 0;
* Cid = (wValue & 0x00FFU);
* \endverbatim
*/
#define PHPAL_I14443P4_CONFIG_CID               0x0001U
/**
* \brief Set / Get Node Address.
*
* \c wValue is parsed as follows:
* \verbatim
* NadEnabled = (wValue & 0xFF00U) ? 1 : 0;
* Nad = (wValue & 0x00FFU);
* \endverbatim
*/
#define PHPAL_I14443P4_CONFIG_NAD               0x0002U
/**
* \brief Set / Get Frame Waiting Integer.
*/
#define PHPAL_I14443P4_CONFIG_FWI               0x0003U
/**
* \brief Set / Get PCD & PICC Frame Size Integer.
*
* \c wValue is parsed as follows:
* \verbatim
* Fsdi = ((wValue & 0xFF00U) >> 8U);
* Fsci = (wValue & 0x00FFU);
* \endverbatim
*/
#define PHPAL_I14443P4_CONFIG_FSI               0x0004U
/**
* \brief Set / Get Maximum Retry Count.
*/
#define PHPAL_I14443P4_CONFIG_MAXRETRYCOUNT     0x0005U

/**
* \brief Configure Operation mode.
*/

/**
 * Configure Operation Mode for this Layer. \n
 * Possible values are\n
 *      #RD_LIB_MODE_EMVCO, \n
 *      #RD_LIB_MODE_NFC, \n
 *      #RD_LIB_MODE_ISO,\n
 * Default value is #RD_LIB_MODE_NFC.
 * */
#define PHPAL_I14443P4_CONFIG_OPE_MODE          0x0006U
/*@}*/

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phpalI14443p4/src/Sw/phpalI14443p4_Sw.h"

#define phpalI14443p4_SetProtocol( pDataParams, bCidEnable,  bCid,  bNadEnable,   bNad, bFwi,  bFsdi, bFsci) \
         phpalI14443p4_Sw_SetProtocol((phpalI14443p4_Sw_DataParams_t *)pDataParams, bCidEnable, bCid, bNadEnable,bNad, bFwi, bFsdi,bFsci)

#define phpalI14443p4_ResetProtocol(pDataParams) \
        phpalI14443p4_Sw_ResetProtocol((phpalI14443p4_Sw_DataParams_t *)pDataParams)

#define phpalI14443p4_Deselect( pDataParams) \
        phpalI14443p4_Sw_Deselect((phpalI14443p4_Sw_DataParams_t *)pDataParams)

#define  phpalI14443p4_PresCheck( pDataParams) \
        phpalI14443p4_Sw_PresCheck((phpalI14443p4_Sw_DataParams_t *)pDataParams)

#define phpalI14443p4_Exchange( pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength) \
        phpalI14443p4_Sw_Exchange((phpalI14443p4_Sw_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength)

#define phpalI14443p4_SetConfig( pDataParams, wConfig, wValue) \
        phpalI14443p4_Sw_SetConfig((phpalI14443p4_Sw_DataParams_t *)pDataParams, wConfig, wValue)

#define phpalI14443p4_GetConfig( pDataParams, wConfig, pValue) \
        phpalI14443p4_Sw_GetConfig((phpalI14443p4_Sw_DataParams_t *)pDataParams, wConfig, pValue)

#else

/**
* \brief Set the ISO14443-4 protocol parameters.
*
* This sets the protocol parameters for this layer.
* It is recommended to input this function with the parameters retrieved from either
* \ref phpalI14443p4a_GetProtocolParams or \ref phpalI14443p3b_GetProtocolParams respectively after card activation.
* \b Note: Refer to the ISO/IEC 14443-4:2008(E) specification for a detailed description of the parameters.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p4_SetProtocol(
                                     void * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t   bCidEnable,      /**< [In] Enable usage of Card Identifier; Unequal '0' if enabled. */
                                     uint8_t   bCid,            /**< [In] Card Identifier; Ignored if \c bCidEnable is equal '0'. */
                                     uint8_t   bNadEnable,      /**< [In] Enabler usage of Node Address; Unequal '0' if enabled. */
                                     uint8_t   bNad,            /**< [In] Node Address; Ignored if bNadEnabled is equal '0'. */
                                     uint8_t   bFwi,            /**< [In] Frame Waiting Integer. */
                                     uint8_t   bFsdi,           /**< [In] PCD Frame Size Integer; 0-12 */
                                     uint8_t   bFsci            /**< [In] PICC Frame Size Integer; 0-12 */
                                     );

/**
* \brief Reset the ISO14443-4 protocol parameters.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p4_ResetProtocol(
                                       void * pDataParams   /**< [In] Pointer to this layer's parameter structure. */
                                       );

/**
* \brief Deselect ISO14443-4 card.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p4_Deselect(
                                  void * pDataParams    /**< [In] Pointer to this layer's parameter structure. */
                                  );

/**
* \brief Perform presence check for current card. (Perform R(NAK) polling as defined in ISO 14443-4:2008(E)).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p4_PresCheck(
                                   void * pDataParams   /**< [In] Pointer to this layer's parameter structure. */
                                   );

/**
* \brief Perform ISO14443-4 Data Exchange with Picc.
*
* \c wOption can be one of:\n
* \li #PH_EXCHANGE_DEFAULT
* \li #PH_EXCHANGE_TXCHAINING
* \li #PH_EXCHANGE_RXCHAINING
* \li #PH_EXCHANGE_RXCHAINING_BUFSIZE
*
* Additionally, the following options are also available:\n
* \li #PH_EXCHANGE_BUFFER_FIRST
* \li #PH_EXCHANGE_BUFFER_CONT
* \li #PH_EXCHANGE_BUFFER_LAST
*
* Alternatively to the FIRST/CONT/LAST options, the following bits can be combined:\n
* \li #PH_EXCHANGE_BUFFERED_BIT
* \li #PH_EXCHANGE_LEAVE_BUFFER_BIT
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4_Exchange(
                                  void * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
                                  uint16_t wOption,         /**< [In] Option parameter. */
                                  uint8_t * pTxBuffer,      /**< [In] Data to transmit. */
                                  uint16_t wTxLength,       /**< [In] Length of data to transmit. */
                                  uint8_t ** ppRxBuffer,    /**< [Out] Pointer to received data. */
                                  uint16_t * pRxLength      /**< [Out] number of received data bytes. */
                                  );

/**
* \brief Set configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4_SetConfig(
                                   void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                   uint16_t wConfig,    /**< [In] Configuration Identifier */
                                   uint16_t wValue      /**< [In] Configuration Value */
                                   );

/**
* \brief Get configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4_GetConfig(
                                   void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                   uint16_t wConfig,    /**< [In] Configuration Identifier */
                                   uint16_t * pValue    /**< [Out] Configuration Value */
                                   );
#endif /* NXPRDLIB_REM_GEN_INTFS */
/** @} */
#endif /* NXPBUILD__PHPAL_I14443P4 */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHPALI14443P4_H */
