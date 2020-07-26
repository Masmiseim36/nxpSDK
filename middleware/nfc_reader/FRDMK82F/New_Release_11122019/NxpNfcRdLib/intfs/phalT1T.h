/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Internal functions for Tag Operation Application Layer component of
* Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/
#ifndef PHALT1T_H
#define PHALT1T_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

/* #define __DEBUG */
#ifdef NXPBUILD__PHAL_T1T_SW

/** \defgroup phalT1T_Sw Component : Software
* @{
*/

#define PHAL_T1T_SW_ID      0x01U                                    /**< ID for Software T1T layer */

/**
* \brief Private parameter structure
*/
typedef struct
{
    uint16_t wId;                                                    /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalI14443p3aDataParams;                                  /**< Pointer to pal parameter structure. */
    uint8_t abHR[2];                                                 /**< Header Rom bytes. */
    uint8_t abUid[4];                                                /**< UID of selected tag. */
} phalT1T_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalT1T_Sw_Init(
    phalT1T_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wSizeOfDataParams,              /**< [In] Specifies the size of the data parameter structure. */
    void * pPalI14443p3aDataParams           /**< [In] Pointer to the parameter structure of the underlying ISO14443-3 layer. */
    );

    /** @} */

#endif /* NXPBUILD__PHAL_T1T_SW */

#ifdef NXPBUILD__PHAL_T1T
/** \defgroup phalT1T Type 1 tag
* \brief These Components implement the Type 1 Tag commands.
* @{
*/

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phalT1T/src/Sw/phalT1T_Sw.h"

#define phalT1T_ReadUID(pDataParams, pUid, pLength) \
        phalT1T_Sw_ReadUID((phalT1T_Sw_DataParams_t *)pDataParams , pUid , pLength)

#define phalT1T_ReadAll(pDataParams, pUid, pData , pLength) \
        phalT1T_Sw_ReadAll((phalT1T_Sw_DataParams_t *)pDataParams, pUid, pData , pLength)

#define phalT1T_ReadByte(pDataParams, pUid , bAddress ,  pData , pLength)\
        phalT1T_Sw_ReadByte((phalT1T_Sw_DataParams_t *)pDataParams, pUid , bAddress ,  pData , pLength)

#define phalT1T_WriteEraseByte(pDataParams, pUid , bAddress , bTxData ,  pRxData , pLength) \
        phalT1T_Sw_WriteEraseByte((phalT1T_Sw_DataParams_t *)pDataParams, pUid , bAddress , bTxData ,  pRxData , pLength)

#define phalT1T_WriteNoEraseByte(pDataParams, pUid , bAddress , bTxData ,  pRxData , pLength) \
        phalT1T_Sw_WriteNoEraseByte((phalT1T_Sw_DataParams_t *)pDataParams, pUid , bAddress , bTxData ,  pRxData , pLength)

#define phalT1T_ReadSegment(pDataParams, pUid , bAddress ,  pData , pLength) \
        phalT1T_Sw_ReadSegment((phalT1T_Sw_DataParams_t *)pDataParams, pUid , bAddress ,  pData , pLength)

#define phalT1T_ReadBlock(pDataParams, pUid , bAddress ,  pData , pLength) \
        phalT1T_Sw_ReadBlock((phalT1T_Sw_DataParams_t *)pDataParams, pUid , bAddress ,  pData , pLength)

#define phalT1T_WriteEraseBlock(pDataParams, pUid , bAddress , pTxData ,  pRxData , pLength) \
        phalT1T_Sw_WriteEraseBlock((phalT1T_Sw_DataParams_t *)pDataParams, pUid , bAddress , pTxData ,  pRxData , pLength)

#define phalT1T_WriteNoEraseBlock(pDataParams, pUid , bAddress , pTxData ,  pRxData , pLength) \
        phalT1T_Sw_WriteNoEraseBlock((phalT1T_Sw_DataParams_t *)pDataParams, pUid , bAddress , pTxData ,  pRxData , pLength)

#else

/**
* \brief Perform Jewel/Topaz Read UID command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalT1T_ReadUID(
    void    * pDataParams,                     /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pUid,                            /**< [Out] Uid of Card, 4 bytes. */
    uint16_t * pLength                         /**< [Out] Number of received data bytes. */
    ) ;

/**
* \brief Perform Jewel/Topaz Read All command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalT1T_ReadAll(
    void * pDataParams,                        /**< [In]  Pointer to this layer's parameter structure. */
    uint8_t * pUid,                            /**< [In]  Known Uid , 4 bytes. */
    uint8_t ** pData,                          /**< [Out] pData containing 122 data bytes returned from the PICC. [HR0][HR1][Data from blocks 0 to 0xE] */
    uint16_t * pLength                         /**< [Out] Number of received data bytes. */
    ) ;

/**
* \brief Perform Jewel/Topaz Read Byte command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalT1T_ReadByte(
    void * pDataParams,                       /**< [In]  Pointer to this layer's parameter structure. */
    uint8_t * pUid,                           /**< [In]  Known Uid , 4 bytes. */
    uint8_t   bAddress,                       /**< [In]  Address of a byte on Picc to read from. */
    uint8_t * pData,                          /**< [Out] pData containing one data byte returned from the PICC. */
    uint16_t * pLength                        /**< [Out] Number of received data bytes. */
    ) ;

/**
* \brief Perform Jewel/Topaz Write Erase Byte command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalT1T_WriteEraseByte(
    void * pDataParams,                 /**< [In]  Pointer to this layer's parameter structure. */
    uint8_t * pUid,                     /**< [In]  Known Uid, 4 bytes. */
    uint8_t   bAddress,                 /**< [In]  Address of a byte on Picc to write to. */
    uint8_t   bTxData,                  /**< [In]  pData containing data to be written to the Picc.*/
    uint8_t * pRxData,                  /**< [Out] pRxData containing 2 data bytes returned from the Picc. [ADD][Byte] */
    uint16_t * pLength                  /**< [Out] Number of received data bytes. */
    );

/**
* \brief Perform Jewel/Topaz Write No Erase Byte command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalT1T_WriteNoEraseByte(
    void * pDataParams,               /**< [In]  Pointer to this layer's parameter structure. */
    uint8_t * pUid,                   /**< [In]  Known Uid , 4 bytes. */
    uint8_t   bAddress,               /**< [In]  Address of a byte on PICC to write to. */
    uint8_t   bTxData,                /**< [In]  pData containing data to be written to the PICC. */
    uint8_t * pRxData,                /**< [Out] pRxData containing 2 data bytes returned from the PICC.*/
    uint16_t * pLength                /**< [Out] Number of received data bytes. */
    );

/**
* \brief Perform Jewel/Topaz Read Segment command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalT1T_ReadSegment(
    void * pDataParams,                    /**< [In]  Pointer to this layer's parameter structure. */
    uint8_t * pUid,                        /**< [In]  Known Uid , 4 bytes. */
    uint8_t   bAddress,                    /**< [In]  Address of a segment on Picc to read from. */
    uint8_t ** pData,                      /**< [Out] pData containing 128 bytes of data returned from the PICC. */
    uint16_t * pLength                     /**< [Out] Number of received data bytes. */
    );

/**
* \brief Perform Jewel/Topaz Read Block command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalT1T_ReadBlock(
    void * pDataParams,                      /**< [In]  Pointer to this layer's parameter structure. */
    uint8_t * pUid,                          /**< [In]  Known Uid , 4 bytes. */
    uint8_t   bAddress,                      /**< [In]  Address of a block on Picc to read from. */
    uint8_t * pData,                         /**< [Out] pData containing 8 data bytes returned from the Picc. At least 8 bytes must be provided by the user */
    uint16_t * pLength                       /**< [Out] Number of received data bytes. */
    );

/**
* \brief Perform Jewel/Topaz Write Erase Block command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalT1T_WriteEraseBlock(
    void * pDataParams,                /**< [In]  Pointer to this layer's parameter structure. */
    uint8_t * pUid,                    /**< [In]  Known Uid , 4 bytes. */
    uint8_t   bAddress,                /**< [In]  Address of a block on Picc to write to. */
    uint8_t * pTxData,                 /**< [In]  pTxData containing data to be written to the Picc. */
    uint8_t * pRxData,                 /**< [Out] pRxData containing 8 data bytes returned from the Picc. At least 8 bytes buffer must be provided by the user. */
    uint16_t * pLength                 /**< [Out] Number of received data bytes. */
    );

/**
* \brief Perform Jewel/Topaz Write No Erase Block command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalT1T_WriteNoEraseBlock(
    void * pDataParams,              /**< [In]  Pointer to this layer's parameter structure. */
    uint8_t * pUid,                  /**< [In]  Known Uid , 4 bytes. */
    uint8_t   bAddress,              /**< [In]  Address of a block on Picc to write to. */
    uint8_t * pTxData,               /**< [In]  pTxData containing data to be written to the Picc. */
    uint8_t * pRxData,               /**< [Out] pRxData containing 8 data bytes returned from the Picc. At least 8 bytes buffer must be provided by the user. */
    uint16_t * pLength               /**< [Out] Number of received data bytes. */
    ) ;
/** @} */

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHAL_T1T */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALT1T_H */
