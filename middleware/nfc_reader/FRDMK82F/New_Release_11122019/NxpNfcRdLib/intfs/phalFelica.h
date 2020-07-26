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
* Generic FeliCa Application Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  RSn: Generated 25. January 2010
*
*/

#ifndef PHALFELICA_H
#define PHALFELICA_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHAL_FELICA_SW

/** \defgroup phalFelica_Sw Component : Software
* @{
*/

#define PHAL_FELICA_SW_ID   0x01U    /**< ID for Software alFelica layer */

/**
* \brief Private parameter structure
*/
typedef struct
{
    uint16_t wId;                   /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalFelicaDataParams;    /**< Pointer to palFelica parameter structure. */
    uint16_t wAdditionalInfo;       /**< Holds the last error code sent by the VICC. */
} phalFelica_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalFelica_Sw_Init(
                              phalFelica_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                              uint16_t wSizeOfDataParams,               /**< [In] Specifies the size of the data parameter structure. */
                              void * pPalFelica_DataParams              /**< [In] Pointer to palFelica parameter structure. */
                              );

/** @} */
#endif /* NXPBUILD__PHAL_FELICA_SW */

#ifdef NXPBUILD__PHAL_FELICA

/** \defgroup phalFelica FeliCa
* \brief These Components implement the FeliCa commands.
* @{
*/

/**
* \name FeliCa constant definitions
*/
/*@{*/
#define PHAL_FELICA_CONFIG_ADD_INFO     0x0000U     /**< Set or get the additional info word. */

/**
* \name FeliCa error codes
*/
/*@{*/
#define PHAL_FELICA_ERR_FELICA  ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0U) /**< General FeliCa error. */
/*@}*/

/**
* \name Misc. Defines
*/
/*@{*/
#define PHAL_FELICA_MAX_SERVICES    121U    /**< Maximum number of services. */
/*@}*/


#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phalFelica/src/Sw/phalFelica_Sw.h"

#define phalFelica_RequestResponse( pDataParams, pMode)\
        phalFelica_Sw_RequestResponse((phalFelica_Sw_DataParams_t *)pDataParams, pMode)

#define phalFelica_RequestService(pDataParams, bTxNumServices, pTxServiceList, pRxNumServices, pRxServiceList) \
        phalFelica_Sw_RequestService((phalFelica_Sw_DataParams_t *)pDataParams, bTxNumServices, pTxServiceList, pRxNumServices, pRxServiceList)

#define phalFelica_Read(pDataParams, bNumServices, pServiceList, bTxNumBlocks, pBlockList, bBlockListLength, pRxNumBlocks, pBlockData) \
        phalFelica_Sw_Read((phalFelica_Sw_DataParams_t *)pDataParams, bNumServices, pServiceList, bTxNumBlocks, pBlockList, bBlockListLength, pRxNumBlocks, pBlockData)

#define phalFelica_Write(pDataParams,  bNumServices, pServiceList, bNumBlocks, pBlockList, bBlockListLength, pBlockData) \
        phalFelica_Sw_Write((phalFelica_Sw_DataParams_t *)pDataParams,  bNumServices, pServiceList, bNumBlocks, pBlockList, bBlockListLength, pBlockData)

#define phalFelica_GetConfig( pDataParams, wConfig, pValue) \
        phalFelica_Sw_GetConfig((phalFelica_Sw_DataParams_t *)pDataParams, wConfig, pValue )

#define phalFelica_ActivateCard( pDataParams, pSystemCode, bTimeSlot, pRxBuffer, pRxLength, pMoreCardsAvailable) \
        phalFelica_Sw_ActivateCard( (phalFelica_Sw_DataParams_t *)pDataParams, pSystemCode, bTimeSlot, pRxBuffer, pRxLength, pMoreCardsAvailable)

#else

/**
* \brief When receiving the RequestResponse command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalFelica_RequestResponse(
                                      void * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                                      uint8_t * pMode       /**< [Out] Current Card Mode. (0, 1, 2). */
                                      );

/**
* \brief When receiving the RequestService command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalFelica_RequestService(
                                     void * pDataParams,        /**< [In] Pointer to this layers parameter structure. */
                                     uint8_t  bTxNumServices,   /**< [In] Number of services or areas within the command message. */
                                     uint8_t * pTxServiceList,  /**< [In] Service code or area code list within the command message. */
                                     uint8_t * pRxNumServices,  /**< [Out] Number of received services or areas. */
                                     uint8_t * pRxServiceList   /**< [Out] Received Service Key version or area version list, max 64 bytes. */
                                     );

/**
* \brief When receiving the Read command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalFelica_Read(
                           void * pDataParams,          /**< [In] Pointer to this layers parameter structure. */
                           uint8_t  bNumServices,       /**< [In] Number of Services. */
                           uint8_t * pServiceList,      /**< [In] List of Services. */
                           uint8_t  bTxNumBlocks,       /**< [In] Number of Blocks to send. */
                           uint8_t * pBlockList,        /**< [In] List of Blocks to read. */
                           uint8_t  bBlockListLength,   /**< [In] Number of Blocks to read. */
                           uint8_t * pRxNumBlocks,      /**< [Out] Number of received blocks. */
                           uint8_t * pBlockData         /**< [Out] Received Block data. */
                           );

/**
* \brief When receiving the Write command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalFelica_Write(
                            void * pDataParams,         /**< [In] Pointer to this layers parameter structure. */
                            uint8_t  bNumServices,      /**< [In] Number of Services. */
                            uint8_t * pServiceList,     /**< [In] List of Services. */
                            uint8_t  bNumBlocks,        /**< [In] Number of Blocks to send. */
                            uint8_t * pBlockList,       /**< [In] List of Blocks to write. */
                            uint8_t  bBlockListLength,  /**< [In] Number of Blocks to write. */
                            uint8_t * pBlockData        /**< [In] Block data to write. */
                            );

/**
* \brief Perform a GetConfig command.
*
* \c wConfig can be one of:\n
* \li #PHAL_FELICA_CONFIG_ADD_INFO
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalFelica_GetConfig(
                                void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                                uint16_t wConfig,   /**< [In] Configuration Identifier. */
                                uint16_t * pValue   /**< [Out] Configuration Value. */
                                );

/**
* \brief This command activates a FeliCa card.
*
* \b Note: SystemCode 0xFFFF is passed to poll for all FeliCa Tags irrespective of the SystemCode.
*          If SystemCode is specified then only corresponding Tag will be Polled and Activated.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalFelica_ActivateCard(
                                    void* pDataParams,              /**< [In] Pointer to this layer's parameter structure. */
                                    uint8_t * pSystemCode,          /**< [In] System-Code; uint8_t[2]. */
                                    uint8_t bNumTimeSlots,          /**< [In] Number of timeslots to use. Refer to e.g. #PHPAL_FELICA_NUMSLOTS_1 for valid values. */
                                    uint8_t * pRxBuffer,            /**< [Out] 8 bytes NFCID2 + 8 bytes PAD; uint8_t[16]. */
                                    uint8_t * pRxLength,            /**< [Out] Length of received data. 0 or 16. */
                                    uint8_t * pMoreCardsAvailable   /**< [Out] Whether there are more cards in the field or not; uint8_t. */
                                    );


/** @} */
#endif

#endif /* NXPBUILD__PHAL_FELICA */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALFELICA_H */
