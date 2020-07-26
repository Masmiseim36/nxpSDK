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
* Software FeliCa Application Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  RSn: Generated 26. January 2010
*
*/

#ifndef PHALFELICA_SW_H
#define PHALFELICA_SW_H

#include <ph_Status.h>

phStatus_t phalFelica_Sw_RequestResponse(
                    phalFelica_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                    uint8_t * pMode                             /**< [Out] number of received data bytes. */
                                        );

phStatus_t phalFelica_Sw_RequestService(
                    phalFelica_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                    uint8_t  bTxNumServices,    /**< [IN] */
                    uint8_t* pTxServiceList,    /**< [IN] */
                    uint8_t* pRxNumServices,    /**< [OUT] */
                    uint8_t* pRxServiceList     /**< [OUT] */
                          );

phStatus_t phalFelica_Sw_Read(
                    phalFelica_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                    uint8_t  bNumServices,      /**< [IN] */
                    uint8_t* pServiceList,      /**< [IN] */
                    uint8_t  bTxNumBlocks,      /**< [IN] */
                    uint8_t* pBlockList,        /**< [IN] */
                    uint8_t  bBlockListLength,  /**< [IN] */
                    uint8_t* pRxNumBlocks,      /**< [OUT] */
                    uint8_t* pBlockData         /**< [OUT] */
                );

phStatus_t phalFelica_Sw_Write(
                    phalFelica_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                    uint8_t  bNumServices,      /**< [IN] */
                    uint8_t* pServiceList,      /**< [IN] */
                    uint8_t  bNumBlocks,        /**< [IN] */
                    uint8_t* pBlockList,        /**< [IN] */
                    uint8_t  bBlockListLength,  /**< [IN] */
                    uint8_t* pBlockData         /**< [IN] */
                 );

phStatus_t phalFelica_Sw_GetConfig(
                                      phalFelica_Sw_DataParams_t* pDataParams,
                                      uint16_t wConfig,
                                      uint16_t* pValue
                                      );

phStatus_t phalFelica_Sw_ActivateCard(
                                       phalFelica_Sw_DataParams_t* pDataParams,
                                       uint8_t * pSystemCode,
                                       uint8_t bNumTimeSlots,
                                       uint8_t * pRxBuffer,
                                       uint8_t * pRxLength,
                                       uint8_t * pMoreCardsAvailable
                                       );

#endif /* PHALFELICA_SW_H */
