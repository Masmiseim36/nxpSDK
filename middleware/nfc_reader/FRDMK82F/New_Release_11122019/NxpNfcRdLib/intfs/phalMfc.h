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
* Generic MIFARE Classic Application Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 31. July 2009
*
*/

#ifndef PHALMFC_H
#define PHALMFC_H

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalMifare.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHAL_MFC_SW

/** \defgroup phalMfc_Sw Component : Software
* @{
*/

#define PHAL_MFC_SW_ID          0x01U    /**< ID for Software layer of the MIFARE Classic AL */

/**
* \brief Private parameter structure
*/
typedef struct
{
    uint16_t wId;                   /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalMifareDataParams;    /**< Pointer to the parameter structure of the palMifare component. */
    void * pKeyStoreDataParams;     /**< Pointer to the parameter structure of the KeyStore component. */
} phalMfc_Sw_DataParams_t;

/**
* \brief Initialize this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_Sw_Init(
                           phalMfc_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                           uint16_t wSizeOfDataParams,              /**< [In] Specifies the size of the data parameter structure. */
                           void * pPalMifareDataParams,             /**< [In] Pointer to the parameter structure of the palMifare component. */
                           void * pKeyStoreDataParams               /**< [In] Pointer to the parameter structure of the KeyStore component. */
                           );

/** @} */
#endif /* NXPBUILD__PHAL_MFC_SW */


#ifdef NXPBUILD__PHAL_MFC

/** \defgroup phalMfc MIFARE Classic contactless IC
* \brief These Functions implement the MIFARE Classic commands.
* @{
*/

/**
* \name Authentication Parameters
*/
/*@{*/
#define PHAL_MFC_KEYA           0x0AU   /**< MIFARE product Key A. */
#define PHAL_MFC_KEYB           0x0BU   /**< MIFARE product Key B. */
/*@}*/

/**
* \name Block definitions
*/
/*@{*/
#define PHAL_MFC_DATA_BLOCK_LENGTH      16U     /**< Length of a MIFARE Classic data block. */
#define PHAL_MFC_VALUE_BLOCK_LENGTH      4U     /**< Length of a MIFARE Classic value block. */
/*@}*/

/**
* \name UID Type definitions
*/
/*@{*/
#define PHAL_MFC_UID_TYPE_UIDF0      0x00U     /**< MIFARE Classic UID type UIDF0. */
#define PHAL_MFC_UID_TYPE_UIDF1      0x40U     /**< MIFARE Classic UID type UIDF1. */
#define PHAL_MFC_UID_TYPE_UIDF2      0x20U     /**< MIFARE Classic UID type UIDF2. */
#define PHAL_MFC_UID_TYPE_UIDF3      0x60U     /**< MIFARE Classic UID type UIDF3. */
/*@}*/


#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phalMfc/src/Sw/phalMfc_Sw.h"

#define phalMfc_Authenticate(pDataParams, bBlockNo, bKeyType, wKeyNumber, wKeyVersion, pUid, bUidLength) \
        phalMfc_Sw_Authenticate((phalMfc_Sw_DataParams_t *)pDataParams, bBlockNo, bKeyType, wKeyNumber, wKeyVersion, pUid, bUidLength)

#define phalMfc_Read(pDataParams, bBlockNo, pBlockData) \
        phalMfc_Sw_Read((phalMfc_Sw_DataParams_t *)pDataParams, bBlockNo, pBlockData)

#define phalMfc_ReadValue(pDataParams, bBlockNo, pValue, pAddrData) \
        phalMfc_Sw_ReadValue((phalMfc_Sw_DataParams_t *)pDataParams, bBlockNo, pValue, pAddrData)

#define phalMfc_Write(pDataParams, bBlockNo, pBlockData) \
        phalMfc_Sw_Write((phalMfc_Sw_DataParams_t *)pDataParams, bBlockNo, pBlockData)

#define phalMfc_WriteValue(pDataParams, bBlockNo, pValue, bAddrData) \
        phalMfc_Sw_WriteValue((phalMfc_Sw_DataParams_t *)pDataParams, bBlockNo, pValue, bAddrData)

#define phalMfc_Increment(pDataParams, bBlockNo, pValue) \
        phalMfc_Sw_Increment((phalMfc_Sw_DataParams_t *)pDataParams, bBlockNo, pValue)

#define phalMfc_Decrement(pDataParams, bBlockNo, pValue) \
        phalMfc_Sw_Decrement((phalMfc_Sw_DataParams_t *)pDataParams, bBlockNo, pValue)

#define  phalMfc_Transfer(pDataParams, bBlockNo) \
         phalMfc_Sw_Transfer((phalMfc_Sw_DataParams_t *)pDataParams, bBlockNo)

#define phalMfc_Restore(pDataParams, bBlockNo) \
        phalMfc_Sw_Restore((phalMfc_Sw_DataParams_t *)pDataParams, bBlockNo)

#define phalMfc_IncrementTransfer(pDataParams, bSrcBlockNo, bDstBlockNo, pValue) \
        phalMfc_Sw_IncrementTransfer((phalMfc_Sw_DataParams_t *)pDataParams, bSrcBlockNo, bDstBlockNo, pValue)

#define phalMfc_DecrementTransfer(pDataParams, bSrcBlockNo, bDstBlockNo, pValue) \
        phalMfc_Sw_DecrementTransfer((phalMfc_Sw_DataParams_t *)pDataParams, bSrcBlockNo, bDstBlockNo, pValue)

#define phalMfc_RestoreTransfer(pDataParams, bSrcBlockNo, bDstBlockNo) \
        phalMfc_Sw_RestoreTransfer((phalMfc_Sw_DataParams_t *)pDataParams, bSrcBlockNo, bDstBlockNo)

#define phalMfc_PersonalizeUid(pDataParams, bUidType) \
        phalMfc_Sw_PersonalizeUid((phalMfc_Sw_DataParams_t *)pDataParams, bUidType)

#define phalMfc_GetVersion(pDataParams, pVersion) \
        phalMfc_Sw_GetVersion((phalMfc_Sw_DataParams_t *)pDataParams, pVersion)
#else

/**
* \brief Perform Authenticate command with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_Authenticate(
                                void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                uint8_t bBlockNo,       /**< [In] Block number on card to authenticate to. */
                                uint8_t bKeyType,       /**< [In] Either #PHAL_MFC_KEYA or #PHAL_MFC_KEYB. */
                                uint16_t wKeyNumber,    /**< [In] Key number to be used in authentication. */
                                uint16_t wKeyVersion,   /**< [In] Key version to be used in authentication. */
                                uint8_t * pUid,         /**< [In] Complete serial number; uint8_t[4/7/10]. */
                                uint8_t bUidLength      /**< [In] Length of provided serial number (4/7/10). */
                                );

/**
* \brief Perform Read command with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_Read(
                        void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                        uint8_t bBlockNo,       /**< [In] block number to be read. */
                        uint8_t * pBlockData    /**< [Out] pBlockData[16] containing block read from the MIFARE Classic */
                        );

/**
* \brief Perform Read Value command with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful
*/
phStatus_t phalMfc_ReadValue(
                             void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                             uint8_t bBlockNo,      /**< [In] block number to be read. */
                             uint8_t * pValue,      /**< [Out] pValue[4] containing value (LSB first) read from the MIFARE Classic */
                             uint8_t * pAddrData    /**< [Out] pAddrData containing address read from the MIFARE Classic value block */
                             );

/**
* \brief Perform Write command with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_Write(
                         void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                         uint8_t bBlockNo,      /**< [In] block number to be written. */
                         uint8_t * pBlockData   /**< [In] pBlockData[16] containing block to be written to the MIFARE Classic */
                         );
/**
* \brief Perform Write Value command with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_WriteValue(
                              void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                              uint8_t bBlockNo,     /**< [In] block number to be written. */
                              uint8_t * pValue,     /**< [In] pValue[4] containing value (LSB first) to be written to the MIFARE Classic card */
                              uint8_t bAddrData     /**< [In] bAddrData containing address written to the MIFARE Classic value block*/
                              );

/**
* \brief Perform Increment command with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_Increment(
                             void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                             uint8_t bBlockNo,      /**< [In] block number to be incremented. */
                             uint8_t * pValue       /**< [In] pValue[4] containing value (LSB first) to be incremented on the MIFARE Classic card */
                             );

/**
* \brief Perform Decrement command with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_Decrement(
                             void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                             uint8_t bBlockNo,      /**< [In] block number to be decremented. */
                             uint8_t * pValue       /**< [In] pValue[4] containing value (LSB first) to be decremented on the MIFARE Classic card */
                             );

/**
* \brief Perform Transfer command with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_Transfer(
                            void * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                            uint8_t bBlockNo    /**< [In] block number the transfer buffer shall be transferred to. */
                            );

/**
* \brief Perform Restore command with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_Restore(
                           void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                           uint8_t bBlockNo     /**< [In] block number the transfer buffer shall be restored from. */
                           );

/**
* \brief Perform Increment Transfer command sequence with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_IncrementTransfer(
                                     void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bSrcBlockNo,   /**< [In] block number to be incremented. */
                                     uint8_t bDstBlockNo,   /**< [In] block number to be transferred to. */
                                     uint8_t * pValue       /**< [In] pValue[4] containing value (LSB first) to be incremented on the MIFARE Classic */
                                     );

/**
* \brief Perform Decrement Transfer command sequence with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_DecrementTransfer(
                                     void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bSrcBlockNo,   /**< [In] block number to be decremented. */
                                     uint8_t bDstBlockNo,   /**< [In] block number to be transferred to. */
                                     uint8_t * pValue       /**< [In] pValue[4] containing value (LSB first) to be decremented on the MIFARE Classic */
                                     );
/**
* \brief Perform Restore Transfer command sequence with MIFARE Classic PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_RestoreTransfer(
                                   void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t bSrcBlockNo, /**< [In] block number to be decremented. */
                                   uint8_t bDstBlockNo  /**< [In] block number to be transferred to. */
                                   );

/**
* \brief Perform Personalize UID usage command sequence with MIFARE Classic PICC.
* UID type can be one of
* \li #PHAL_MFC_UID_TYPE_UIDF0
* \li #PHAL_MFC_UID_TYPE_UIDF1
* \li #PHAL_MFC_UID_TYPE_UIDF2
* \li #PHAL_MFC_UID_TYPE_UIDF3
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfc_PersonalizeUid(
                                  void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                  uint8_t bUidType      /**< [In] UID type. */
                                  );

/**
* \brief Perform GET VERSION usage command sequence with MIFARE Classic PICC.
* GET_VERSION command is used to retrieve information about the NTAG family, the
* product version, storage size and other product data required to identify the specific NTAG-I2C
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR wrong Length byte received.
*/
phStatus_t phalMfc_GetVersion(
                              void * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
                              uint8_t * pVersion        /**< [Out] Version no  It should be #PHAL_MFC_VERSION_LENGTH */
                              );
/** @} */

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHAL_MFC */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALMFC_H */
