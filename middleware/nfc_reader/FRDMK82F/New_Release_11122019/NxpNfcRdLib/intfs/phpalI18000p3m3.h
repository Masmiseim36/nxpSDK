/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
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
* Generic ISO18000-3 Mode3 Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  MWt: Generated 27. January 2010
*
*/

#ifndef PHPALI18000P3M3_H
#define PHPALI18000P3M3_H

#include <ph_Status.h>
#include <phhalHw.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHPAL_I18000P3M3_SW

/** \defgroup phpalI18000p3m3_Sw Component : Software
* @{
*/

#define PHPAL_I18000P3M3_SW_ID  0x01U    /**< ID for Software I18000P3M3 layer. */

/**
* \brief Software PAL-I18000P3M3 parameter structure
*/
typedef struct
{
    uint16_t wId;               /**< Layer ID for this component, NEVER MODIFY! */
    void  * pHalDataParams;     /**< Pointer to the parameter structure of the underlying HAL layer. */
    uint8_t bSession;           /**< Active session of inventory round. */
    uint8_t abStoredCRC[2];     /**< Stored CRC returned by the tag. */
    uint8_t bStoredCRCValid;    /**< Whether StoredCRC is valid or not. */
} phpalI18000p3m3_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI18000p3m3_Sw_Init(
                                   phpalI18000p3m3_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                   uint16_t wSizeOfDataParams,                      /**< [In] Specifies the size of the data parameter structure. */
                                   void * pHalDataParams                            /**< [In] Pointer to the parameter structure of the underlying HAL layer. */
                                   );

/** @} */
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


#ifdef NXPBUILD__PHPAL_I18000P3M3

/** \defgroup phpalI18000p3m3 ISO/IEC 18000-3 Mode3
* \brief These Components implement the ISO/IEC 18000-3 Mode3 support.
* @{
*/

/**
* \name Targets
*/
/*@{*/
#define PHPAL_I18000P3M3_TARGET_INVENTORIED_S0    0x00U /**< INVENTORIED_S0 Target. */
#define PHPAL_I18000P3M3_TARGET_INVENTORIED_S2    0x02U /**< INVENTORIED_S2 Target. */
#define PHPAL_I18000P3M3_TARGET_SL                0x04U /**< SL Target. */
/*@}*/

/**
* \name Memory Banks
*/
/*@{*/
#define PHPAL_I18000P3M3_MEMBANK_RESERVED   0x00U   /** < Reserved Memory Bank. */
#define PHPAL_I18000P3M3_MEMBANK_UII        0x01U   /** < UII Memory Bank. */
#define PHPAL_I18000P3M3_MEMBANK_TID        0x02U   /** < TID Memory Bank. */
#define PHPAL_I18000P3M3_MEMBANK_USER       0x03U   /** < User Memory Bank. */
/*@}*/

/**
* \name Link Frequencies (DR)
*/
/*@{*/
#define PHPAL_I18000P3M3_LF_423KHZ  0x00U   /**< 423kHz (fc/32). Equivalent to #PHHAL_HW_SUBCARRIER_DUAL DR = 0 */
#define PHPAL_I18000P3M3_LF_847KHZ  0x01U   /**< 847kHz (fc/16). Equivalent to #PHHAL_HW_SUBCARRIER_QUAD DR = 1*/
/*@}*/

/**
* \name 'M' Values
*/
/*@{*/
#define PHPAL_I18000P3M3_M_MANCHESTER_2     0x02U   /**< 2 sub-carrier pulse Manchester. Equivalent to #PHHAL_HW_RX_I18000P3M3_FL_423_MAN2 or #PHHAL_HW_RX_I18000P3M3_FL_847_MAN2. */
#define PHPAL_I18000P3M3_M_MANCHESTER_4     0x03U   /**< 4 sub-carrier pulse Manchester. Equivalent to #PHHAL_HW_RX_I18000P3M3_FL_423_MAN4 or #PHHAL_HW_RX_I18000P3M3_FL_847_MAN4. */
/*@}*/

/**
* \name 'Sel' Values
*/
/*@{*/
#define PHPAL_I18000P3M3_SEL_ALL_00     0x00U   /**< All. */
#define PHPAL_I18000P3M3_SEL_ALL_01     0x01U   /**< All. */
#define PHPAL_I18000P3M3_SEL_NOT_SL     0x02U   /**< Select only tags not matching SL. */
#define PHPAL_I18000P3M3_SEL_SL         0x03U   /**< Select only tags matching SL. */
/*@}*/

/**
* \name 'Session' Values
*/
/*@{*/
#define PHPAL_I18000P3M3_SESSION_S0 0x00U   /**< Session S0. */
#define PHPAL_I18000P3M3_SESSION_S1 0x01U   /**< Session S1 (Optional). */
#define PHPAL_I18000P3M3_SESSION_S2 0x02U   /**< Session S2. */
#define PHPAL_I18000P3M3_SESSION_S3 0x03U   /**< Session S3 (Optional). */
/*@}*/

/**
* \name 'UpDn' Values
*/
/*@{*/
#define PHPAL_I18000P3M3_UPDN_INCREMENT     0x06U   /**< Increment Q. */
#define PHPAL_I18000P3M3_UPDN_NOCHANGE      0x00U   /**< Do not change Q. */
#define PHPAL_I18000P3M3_UPDN_DECREMENT     0x03U   /**< Decrement Q. */
/*@}*/

/**
* \name ACK Options
*/
/*@{*/
#define PHPAL_I18000P3M3_ACK_USE_CRC    0x00U   /**< Use StoredCrc for ACK command. */
#define PHPAL_I18000P3M3_ACK_USE_HANDLE 0x01U   /**< Use given Handle for ACK command. */
/*@}*/

/**
* \name Communication Configs
*/
/*@{*/
#define PHPAL_I18000P3M3_CONFIG_TXLASTBITS          0x0003U     /**< Set number of valid bits of last Tx-Byte. */
#define PHPAL_I18000P3M3_CONFIG_TIMEOUT_VALUE_MS    0x000EU     /**< Set RC Timeout (in [ms]). */
/*@}*/

/**
* \name Values used for Communication Configs
*/
/*@{*/
#define PHPAL_I18000P3M3_TXLASTBITS_MAX             0x000FU     /**< Maximum value used for configuring TXLASTBITS. */
#define PHPAL_I18000P3M3_VALID_TXLASTBITS           0x0007U     /**< Valid value of TXLASTBITS. */
/*@}*/

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phpalI18000p3m3/src/Sw/phpalI18000p3m3_Sw.h"

#define phpalI18000p3m3_Exchange( pDataParams, wOption, pTxBuffer, wTxLength, bTxLastBits, ppRxBuffer, pRxLength, pRxLastBits) \
        phpalI18000p3m3_Sw_Exchange((phpalI18000p3m3_Sw_DataParams_t *)pDataParams, wOption, pTxBuffer, \
                    wTxLength, bTxLastBits, ppRxBuffer,  pRxLength,pRxLastBits)

#define phpalI18000p3m3_CreateBeginRoundCmd(pDataParams,bDr,bM,bTRext,bSel,bSession,bRfu,bQ,pBeginRnd) \
        phpalI18000p3m3_Sw_CreateBeginRoundCmd((phpalI18000p3m3_Sw_DataParams_t *)pDataParams,bDr,bM,bTRext,bSel,bSession,bRfu,bQ,pBeginRnd)

#define phpalI18000p3m3_CreateSelectCmd(pDataParams,bTarget,bAction,bMemBank,pPointer,bPointerLength, \
                                        pMask,bMaskBitLength,bTruncate,pSelectCmd,wLen,bValidBits) \
        phpalI18000p3m3_Sw_CreateSelectCmd((phpalI18000p3m3_Sw_DataParams_t *)pDataParams,bTarget,bAction,bMemBank,pPointer,bPointerLength, \
                                        pMask,bMaskBitLength,bTruncate,pSelectCmd,wLen,bValidBits)

#define phpalI18000p3m3_Ack(  pDataParams, bOption,  pHandle, ppRxBuffer,  pRxBitLength) \
        phpalI18000p3m3_Sw_Ack( (phpalI18000p3m3_Sw_DataParams_t *)pDataParams, bOption, pHandle,ppRxBuffer,  pRxBitLength)

#define phpalI18000p3m3_Nak( pDataParams) \
        phpalI18000p3m3_Sw_Nak((phpalI18000p3m3_Sw_DataParams_t *)pDataParams)

#define phpalI18000p3m3_ActivateCard(pDataParams, pSelCmd, bSelCmdLen, bNumValidBitsinLastByte, pBeginRndCmd, \
                                     bTSprocessing, ppRxBuffer, pRxBufferLen, pMoreCardsAvailable) \
        phpalI18000p3m3_Sw_ActivateCard((phpalI18000p3m3_Sw_DataParams_t *)pDataParams, pSelCmd, bSelCmdLen, bNumValidBitsinLastByte, pBeginRndCmd, \
                                          bTSprocessing, ppRxBuffer, pRxBufferLen, pMoreCardsAvailable)

#define phpalI18000p3m3_SetConfig( pDataParams, wConfig, wValue) \
        phpalI18000p3m3_Sw_SetConfig((phpalI18000p3m3_Sw_DataParams_t *)pDataParams, wConfig, wValue)

#define phpalI18000p3m3_Select(pDataParams, bTarget, bAction, bMemBank, pPointer, bPointerLength, pMask, bMaskBitLength, bTruncate) \
        phpalI18000p3m3_Sw_Select((phpalI18000p3m3_Sw_DataParams_t *)pDataParams, bTarget, bAction, bMemBank, pPointer, bPointerLength, pMask, bMaskBitLength, bTruncate)

#else

/**
* \brief Perform Data Exchange with Tag.
*
* \c wOption can be one of:\n
* \li #PH_EXCHANGE_DEFAULT
* \li #PH_EXCHANGE_BUFFER_FIRST
* \li #PH_EXCHANGE_BUFFER_CONT
* \li #PH_EXCHANGE_BUFFER_LAST
*
* Alternatively, the following bits can be combined:\n
* \li #PH_EXCHANGE_BUFFERED_BIT
* \li #PH_EXCHANGE_LEAVE_BUFFER_BIT
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI18000p3m3_Exchange(
                                    void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wOption,       /**< [In] Option parameter. */
                                    uint8_t * pTxBuffer,    /**< [In] Data to transmit. */
                                    uint16_t wTxLength,     /**< [In] Length of input data. */
                                    uint8_t bTxLastBits,    /**< [In] Number of valid bits of last byte (Tx). */
                                    uint8_t ** ppRxBuffer,  /**< [Out] Pointer to received data. */
                                    uint16_t * pRxLength,   /**< [Out] Number of received data bytes including incomplete byte. */
                                    uint8_t * pRxLastBits   /**< [Out] Number of valid bits of last byte (Rx). */
                                    );


/**
* \brief Create the Select command frame.
* This is a utility function to create the Select frame in the required format of ISO 18000p3m3.
* Parameters are validated to ensure that wrong values or wrong combinations are checked.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER if wrong options are provided.
*/
phStatus_t phpalI18000p3m3_CreateSelectCmd(
                                  void * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
                                  uint8_t bTarget,          /**< [In] Indicates whether the SL flag or the inventoried flags shall be modified. */
                                  uint8_t bAction,          /**< [In] Indicates the way of changing the SL flag and the inventoried flags. */
                                  uint8_t bMemBank,         /**< [In] Memory bank where the mask shall be applied. */
                                  uint8_t * pPointer,       /**< [In] Memory address where the mask shall be applied. */
                                  uint8_t bPointerLength,   /**< [In] Length of the pointer, 0->8bits,1->16bits,2->24bits or 3->32bits. */
                                  uint8_t * pMask,          /**< [In] Contains a bit string that a tag compares against the memory location that begins at pPointer. */
                                  uint8_t bMaskBitLength,   /**< [In] Length of the mask in bits. */
                                  uint8_t bTruncate,        /**< [In] Specifies whether a tag load modulates its entire UII (#PH_OFF), or only that portion of the UII immediately following Mask (#PH_ON). */
                                  uint8_t * pSelectCmd,     /**< [Out] Select Command frame. (Max 39 bytes) */
                                  uint8_t * bLen,           /**< [Out] Length of Select cmd bytes */
                                  uint8_t * bValidBits      /**< [Out] Number of valid bits in the last byte of pSelectCmd. */
                                  );

/**
* \brief Create the BeginRound command frame.
* This is a utility function to create the BeginRound frame in the required format of ISO 18000p3m3.
* Parameters are validated to ensure that wrong values or wrong combinations are checked.
*
* bDr can be one of \n
* \li #PHPAL_I18000P3M3_LF_423KHZ
* \li #PHPAL_I18000P3M3_LF_847KHZ
*
* bM (Number of subcarrier cycles per symbol) cane be one of \n
* \li #PHPAL_I18000P3M3_M_MANCHESTER_2 (M = 10b)
* \li #PHPAL_I18000P3M3_M_MANCHESTER_4 (M = 11b)
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER if wrong options are provided.
*/
phStatus_t phpalI18000p3m3_CreateBeginRoundCmd(
                                      void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                      uint8_t bDr,          /**< [In] Divide ratio. */
                                      uint8_t bM,           /**< [In] Cycles per symbol. */
                                      uint8_t bTRext,       /**< [In] Indicates whether a pilot tone shall be used in the T=>R pre-amble (#PH_ON) or not (#PH_OFF). */
                                      uint8_t bSel,         /**< [In] Indicates which tags shall take part of the inventory round. */
                                      uint8_t bSession,     /**< [In] Indicates the session of the inventory round. */
                                      uint8_t bRfu,         /**< [In] RFU (1 bit). */
                                      uint8_t bQ,           /**< [In] Indicates the number of slots in a round (2^Q slots). */
                                      uint8_t * pBeginRnd   /**< [Out] Begin round command frame (3 bytes) */
                                      );

/**
* \brief Perform a Ack for ISO18000-3 Mode3 tags.
*
*
* \c bOption can be one of:\n
* \li #PHPAL_I18000P3M3_ACK_USE_CRC - This option is deprecated and no longer used.
* \li #PHPAL_I18000P3M3_ACK_USE_HANDLE
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI18000p3m3_Ack(
                               void * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bOption,         /**< [In] Option parameter. */
                               uint8_t * pHandle,       /**< [In] Handle value, mandatory if \c bOption is #PHPAL_I18000P3M3_ACK_USE_HANDLE; uint8_t[2]. */
                               uint8_t ** ppRxBuffer,   /**< [Out] Pointer to Tag data and, if required, PacketCRC. */
                               uint16_t * pRxBitLength  /**< [Out] Tag response length in bits. */
                               );

/**
* \brief Perform a Nak for ISO18000-3 Mode3 tags.
* All tags will move to arbitrate state after receive this command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI18000p3m3_Nak(
                               void * pDataParams   /**< [In] Pointer to this layer's parameter structure. */
                               );

/**
* \brief Bring the first matching tag to OPEN or SECURED state.
*
* The command performs a selection followed by an inventory round.
* The first responding tag is set to the OPEN or SECURED state and
* the tag handle is returned.
* If more tags are in the field, it shall resolve collision and
* activate one tag. The pMoreCardsAvailable value will be set to 1
* if more cards are available in the field.
*
* Issuing the Select command is optional based on the value of bSelCmdLen parameter.
* If bSelCmdLen parameter is zero, then ISO 18000p3m3 Select command is
* not issued. Only the BeginRound command is issued.
*
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI18000p3m3_ActivateCard(
                                        void * pDataParams,                 /**< [In] Pointer to this layer's parameter structure. */
                                        uint8_t * pSelCmd,                  /**< [In] ISO 18000p3m3 Select command frame. */
                                        uint8_t bSelCmdLen,                 /**< [In] Select command length in bytes- 1 to 39 bytes. If set to zero, Select command is not sent. */
                                        uint8_t bNumValidBitsinLastByte,    /**< [In] Number of valid bits in last byte of pSelCmd. */
                                        uint8_t * pBeginRndCmd,             /**< [In] ISO 18000p3m3 BeginRound command frame. This is 17bits i.e., 3 bytes are expected. CRC5 should not be provided. */
                                        uint8_t bTSprocessing,              /**< [In] TimeSlot processing behavior. */
                                        uint8_t ** ppRxBuffer,              /**< [Out] Pointer to HAL Rx Buffer containing handle of the activated tag. */
                                        uint16_t * pRxBufferLen,            /**< [Out] Length of response in hal Rx Buffer. */
                                        uint8_t * pMoreCardsAvailable       /**< [Out] Indicates if more tags are present in the field. */
                                        );

/**
* \brief Sets the configuration parameter.
*
* This function sets the values for the provided configuration parameter
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI18000p3m3_SetConfig(
                                     void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                     uint16_t wConfig,      /**< [In] Configuration Identifier. */
                                     uint16_t wValue        /**< [In] Configuration Value. */
                                     );

/**
* \brief Select a particular tag population.
* This API is deprecated. This operation is now done by the phhalHw_I18000p3m3Inventory() function
* of the HAL.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI18000p3m3_Select(
                                  void * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
                                  uint8_t bTarget,          /**< [In] Indicates whether the SL flag or the inventoried flags shall be modified. */
                                  uint8_t bAction,          /**< [In] Indicates the way of changing the SL flag and the inventoried flags. */
                                  uint8_t bMemBank,         /**< [In] Memory bank where the mask shall be applied. */
                                  uint8_t * pPointer,       /**< [In] Memory address where the mask shall be applied. */
                                  uint8_t bPointerLength,   /**< [In] Length of the pointer in bytes; 0,1,2 or 3. */
                                  uint8_t * pMask,          /**< [In] Contains a bit string that a tag compares against the memory location that begins at pPointer. */
                                  uint8_t bMaskBitLength,   /**< [In] Length of the mask in bits. */
                                  uint8_t bTruncate         /**< [In] Specifies whether a tag load modulates its entire UII (#PH_OFF), or only that portion of the UII immediately following Mask (#PH_ON). */
                                  );

#ifdef NXPBUILD__PHPAL_I18000P3M3_DEPRECATED
/**
* \brief Perform a BeginRound for ISO18000-3 Mode3 tags.
* This API is deprecated. This operation is now done by the phhalHw_I18000p3m3Inventory() function
* of the HAL.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI18000p3m3_BeginRound(
                                      void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                      uint8_t bDr,          /**< [In] Divide ratio. */
                                      uint8_t bM,           /**< [In] Cycles per symbol. */
                                      uint8_t bTRext,       /**< [In] Indicates whether a pilot tone shall be used in the T=>R pre-amble (#PH_ON) or not (#PH_OFF). */
                                      uint8_t bSel,         /**< [In] Indicates which tags shall take part of the inventory round. */
                                      uint8_t bSession,     /**< [In] Indicates the session of the inventory round. */
                                      uint8_t bRfu,         /**< [In] RFU (1 bit). */
                                      uint8_t bQ,           /**< [In] Indicates the number of slots in a round (2^Q slots). */
                                      uint8_t * pStoredCRC  /**< [Out] Stored CRC of a responding tag. This buffer has to be 2 bytes long. */
                                      );

/**
* \brief Perform a ResizeRound for ISO18000-3 Mode3 tags.
* This API is deprecated. The equivalent operation is now done within the phhalHw_I18000p3m3Inventory() function
* of the HAL.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI18000p3m3_ResizeRound(
                                       void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                       uint8_t bUpDn,       /**< [In] Indicates whether and how Q shall be adjusted. */
                                       uint8_t * pStoredCRC /**< [Out] Stored CRC of a responding tag. This buffer has to be 2 bytes long. */
                                       );

/**
* \brief Perform a NextSlot for ISO18000-3 Mode3 tags.
* This API is deprecated. The equivalent operation is now done within the phhalHw_I18000p3m3Inventory() function
* of the HAL.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI18000p3m3_NextSlot(
                                    void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                    uint8_t * pStoredCRC    /**< [Out] Stored CRC of a responding tag. This buffer has to be 2 bytes long. */
                                    );

/**
* \brief Get the tag data.
*
* The command returns the tag response to the last ActivateCard or ACK.
* This API is deprecated. This functionality is available only when the other deprecated functions
* are used.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI18000p3m3_GetSerialNo(
                                       void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                       uint8_t * pRxBuffer, /**< [Out] StoredCRC; uint8_t[2]. */
                                       uint8_t * pRxLength  /**< [Out] Length of StoredCRC; 2 bytes. */
                                       );

#endif /* NXPBUILD__PHPAL_I18000P3M3_DEPRECATED */


/** @} */
#endif /* NXPRDLIB_REM_GEN_INTFS */
#endif /* NXPBUILD__PHPAL_I18000P3M3 */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHPALI18000P3M3_H */
