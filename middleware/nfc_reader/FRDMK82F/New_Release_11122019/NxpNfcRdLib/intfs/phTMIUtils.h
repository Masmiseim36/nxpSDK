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
* Log Component of Reader Library Framework.
* $Author: NXP99556 $
* $Revision: 1209 $ (v06.01.00)
* $Date: 2015-08-18 14:16:49 +0530 (Tue, 18 Aug 2015) $
*
* History:
*  CHu: Generated 9. October 2009
*
*/

#ifndef PHTMIUTILS_H
#define PHTMIUTILS_H

#include <ph_Status.h>
#include <ph_TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PH_TMIUTILS

/** \defgroup phTMIUtils TMI Utilities
*
* \brief This layer provides Utilities for operating on transaction MAC.
* @{
*/

/**
* \name Activate TMI options
*/
/** @{ */
#define PH_TMIUTILS_DEACTIVATE_TMI     0x00U   /**< Deactivate TMI collection. */
#define PH_TMIUTILS_ACTIVATE_TMI       0x01U   /**< Activate TMI collection. */
#define PH_TMIUTILS_PAUSE_TMI          0x02U   /**< Pause TMI collection. */
#define PH_TMIUTILS_RESUME_TMI         0x03U   /**< Resume Paused TMI collection. */
#define PH_TMIUTILS_RESET_TMI          0x04U   /**< Reset TMI collection Buffer Index to 0. */
/** @} */

/**
* \name Configuration Options
*/
/** @{ */
#define PH_TMIUTILS_TMI_STATUS          0x01U   /**< Config Option for TMI Status.*/
#define PH_TMIUTILS_TMI_OFFSET_LENGTH   0x02U   /**< Config option for Length offset in TMI Length */
#define PH_TMIUTILS_TMI_BUFFER_INDEX    0x04U   /**< Config option for buffer index */
#define PH_TMIUTILS_TMI_OFFSET_VALUE    0x08U   /**< Config option for buffer index */
/** @} */

/**
* \name Flags for CollectTMI
*/
/** @{ */
#define PH_TMIUTILS_NO_PADDING          0x00U   /**< Flag indicating that padding is not required.*/
#define PH_TMIUTILS_READ_INS            0x01U   /**< Flag indicating that instruction is Read Operation.*/
#define PH_TMIUTILS_ZEROPAD_CMDBUFF     0x02U   /**< Flag to indicate zero padding after Cmd buffer */
#define PH_TMIUTILS_ZEROPAD_DATABUFF    0x04U   /**< Flag to indicate zero padding after Data buffer */
/** @} */

/**
* \brief Definition of a singe Log entry
*/
typedef struct
{
    uint8_t * pTMIBuffer;                                       /**< Pointer to TMI Buffer. */
    uint32_t dwTMIBufLen;                                        /**< Length of TMI Buffer. */
    uint32_t dwTMIbufIndex;                                      /**< Pointer indicating the TMI Buffer fill index. */
    uint8_t bTMIStatus;                                         /**< Indicates whether TMI collection is PH_ON or PH_OFF. */
    uint32_t dwOffsetInTMI;                                      /**< Indicates the offset in TMI buffer where the Length field is stored in case of unspecified length read. */
} phTMIUtils_t;

/**
* \brief Initialize TMI Utils component
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/

phStatus_t phTMIUtils_Init(
                           phTMIUtils_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                           uint8_t * pTMIBuffer,        /**< [In] TMI Buffer. */
                           uint32_t dwBufLen            /**< [In] length of Buffer. */
                           );
/**
* \brief Activate TMI Collection
*
* \c bOption can be one of:\n
* \li #PH_TMIUTILS_DEACTIVATE_TMI
* \li #PH_TMIUTILS_ACTIVATE_TMI
* \li #PH_TMIUTILS_PAUSE_TMI
* \li #PH_TMIUTILS_RESUME_TMI
* \li #PH_TMIUTILS_RESET_TMI
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Invalid Parameter.
*/
phStatus_t phTMIUtils_ActivateTMICollection(
                                             phTMIUtils_t * pDataParams,    /**< [In] Pointer to this layers parameter structure. */
                                             uint8_t bOption                /**< [In] Option field. */
                                             );

/**
* \brief Get TMI buffer
*
* \c bOption can be combination of:\n
* \li #PH_TMIUTILS_READ_INS
* \li #PH_TMIUTILS_ZEROPAD_CMDBUFF
* \li #PH_TMIUTILS_ZEROPAD_DATABUFF
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Invalid Parameter.
*/
phStatus_t phTMIUtils_GetTMI(
                              phTMIUtils_t * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                              uint8_t ** ppTMIBuffer,       /**< [Out] Pointer to TMI Buffer. */
                              uint32_t * dwTMILen            /**< [Out] pointer to length of TMI Buffer collected during previous transaction. */
                              );

/**
* \brief Collect TMI
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Invalid Parameter.
*/
phStatus_t phTMIUtils_CollectTMI(
                                 phTMIUtils_t * pDataParams,    /**< [In] Pointer to this layers parameter structure. */
                                 uint8_t bOption,               /**< [In] Option field. */
                                 uint8_t * pCmdBuff,          /**< [In] Cmd Buffer. */
                                 uint16_t wCmdLen,           /**< [In] length of Cmd Buffer */
                                 uint8_t * pData,               /**< [In] Data Buffer. */
                                 uint32_t dwDataLen,            /**< [In] length of Data Buffer */
                                 uint16_t wBlockSize            /**< [In] Block Size */
                                 );

/**
* \brief SetConfig
*
* Sets the #PH_TMIUTILS_TMI_OFFSET_LENGTH
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Invalid Parameter.
*/
phStatus_t phTMIUtils_SetConfig(
                                phTMIUtils_t * pDataParams,     /**< [In] Pointer to this layers parameter structure. */
                                uint16_t wConfig,               /**< [In] Config Option. */
                                uint32_t dwValue                 /**< [In] Value field. */
                                );

/**
* \brief GetConfig
*
* Gets the TMI status #PH_TMIUTILS_TMI_STATUS
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Invalid Parameter.
*/
phStatus_t phTMIUtils_GetConfig(
                                phTMIUtils_t * pDataParams,     /**< [In] Pointer to this layers parameter structure. */
                                uint16_t wConfig,               /**< [In] Config Option. */
                                uint32_t *pValue                /**< [Out] Read value. */
                                );

/** @} */
#endif /* NXPBUILD__PH_TMIUTILS */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif  /* PHTMIUTILS_H */
