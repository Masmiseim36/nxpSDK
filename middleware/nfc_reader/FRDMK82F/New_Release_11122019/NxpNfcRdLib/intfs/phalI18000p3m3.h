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
* Generic ISO18000-3 Mode3 Application Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  MWt: Generated 27. January 2010
*
*/

#ifndef PHALI18000P3M3_H
#define PHALI18000P3M3_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHAL_I18000P3M3_SW

/** \defgroup phalI18000p3m3_Sw Component : Software
* @{
*/

#define PHAL_I18000P3M3_SW_ID   0x01U    /**< ID for Software I18000P3M3 layer. */

/**
* \brief Software PAL-I18000P3M3 parameter structure
*/
typedef struct
{
    uint16_t wId;                       /**< Layer ID for this component, NEVER MODIFY! */
    void  * pPalI18000p3m3DataParams;   /**< Pointer to the parameter structure of the underlying PAL layer. */
    uint8_t abHandle[2];                /**< Handle retrieved by ReqRn. */
    uint8_t bHandleValid;               /**< Whether Handle is valid or not. */
} phalI18000p3m3_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalI18000p3m3_Sw_Init(
                                  phalI18000p3m3_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                  uint16_t wSizeOfDataParams,                   /**< [In] Specifies the size of the data parameter structure. */
                                  void * pPalI18000p3m3DataParams               /**< [In] Pointer to the parameter structure of the underlying PAL layer. */
                                  );

/** @} */
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

#ifdef NXPBUILD__PHAL_I18000P3M3

/** \addtogroup ph_Error
* phalI18000p3m3 Custom Errors
* @{
*/

/** \name phalI18000p3m3 Error Codes
*/
/*@{*/
#define PHAL_I18000P3M3_ERR_OTHER               ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0U) /**< Other Error. */
#define PHAL_I18000P3M3_ERR_MEMORY_OVERRUN      ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 1U) /**< Memory overrun. */
#define PHAL_I18000P3M3_ERR_MEMORY_LOCKED       ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 2U) /**< Memory locked. */
#define PHAL_I18000P3M3_ERR_INSUFFICIENT_POWER  ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 3U) /**< Insufficient power. */
#define PHAL_I18000P3M3_ERR_NON_SPECIFIC        ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 4U) /**< Non-specific error. */
/*@}*/

/** @} */

/** \defgroup phalI18000p3m3 ISO/IEC 18000-3 Mode3
* \brief These Components implement the ICODE ILT support.
* @{
*/

/**
* \name ReqRn Options
*/
/*@{*/
#define PHAL_I18000P3M3_REQRN_USE_CRC       0x00U   /**<  Use StoredCrc for ReqRn command. Deprecated option. Not to be used anymore. */
#define PHAL_I18000P3M3_REQRN_USE_HANDLE    0x01U   /**< Use given Handle for ReqRn command. */
/** @} */

/**
* \name Access Command Options
*/
/*@{*/
#define PHAL_I18000P3M3_AC_NO_COVER_CODING  0x00U   /**< Use cover coding to diversify passwords. */
#define PHAL_I18000P3M3_AC_USE_COVER_CODING 0x01U   /**< Do not use cover coding, send plain passwords. */
/** @} */

/**
* \name Memory Banks
*/
/*@{*/
#define PHAL_I18000P3M3_MEMBANK_RESERVED    0x00U   /** < Reserved Memory Bank. */
#define PHAL_I18000P3M3_MEMBANK_UII         0x01U   /** < UII Memory Bank. */
#define PHAL_I18000P3M3_MEMBANK_TID         0x02U   /** < TID Memory Bank. */
#define PHAL_I18000P3M3_MEMBANK_USER        0x03U   /** < User Memory Bank. */
/** @} */


#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phalI18000p3m3/src/Sw/phalI18000p3m3_Sw.h"

#define phalI18000p3m3_Ack(pDataParams,ppRxBuffer,pRxBitLength) \
        phalI18000p3m3_Sw_Ack((phalI18000p3m3_Sw_DataParams_t*)pDataParams, ppRxBuffer, pRxBitLength)

#define phalI18000p3m3_ReqRn(pDataParams,bOption,ppRxBuffer) \
        phalI18000p3m3_Sw_ReqRn((phalI18000p3m3_Sw_DataParams_t*)pDataParams, bOption, ppRxBuffer)

#define phalI18000p3m3_Read(pDataParams,bMemBank,pWordPtr,bWordPtrLength,bWordCount,ppRxBuffer,pRxBitLength) \
        phalI18000p3m3_Sw_Read((phalI18000p3m3_Sw_DataParams_t*)pDataParams, bMemBank,pWordPtr,bWordPtrLength,bWordCount, ppRxBuffer, pRxBitLength)

#define phalI18000p3m3_Write(pDataParams,bOption,bMemBank,pWordPtr,bWordPtrLength,pData) \
        phalI18000p3m3_Sw_Write((phalI18000p3m3_Sw_DataParams_t*)pDataParams, bOption,bMemBank,pWordPtr, bWordPtrLength,pData)

#define phalI18000p3m3_Kill(pDataParams,bOption,pPassword,bRecom) \
        phalI18000p3m3_Sw_Kill((phalI18000p3m3_Sw_DataParams_t*)pDataParams, bOption, pPassword, bRecom)

#define phalI18000p3m3_Lock(pDataParams,pMask,pAction) \
        phalI18000p3m3_Sw_Lock((phalI18000p3m3_Sw_DataParams_t*)pDataParams,pMask,pAction)

#define phalI18000p3m3_Access(pDataParams,bOption,pPassword) \
        phalI18000p3m3_Sw_Access((phalI18000p3m3_Sw_DataParams_t*)pDataParams,bOption, pPassword)

#define phalI18000p3m3_BlockWrite(pDataParams,bMemBank,pWordPtr,bWordPtrLength,bWordCount,pData) \
        phalI18000p3m3_Sw_BlockWrite((phalI18000p3m3_Sw_DataParams_t*)pDataParams,bMemBank, pWordPtr,bWordPtrLength,bWordCount, pData)

#define phalI18000p3m3_BlockErase(pDataParams,bMemBank,pWordPtr,bWordPtrLength,bWordCount) \
        phalI18000p3m3_Sw_BlockErase((phalI18000p3m3_Sw_DataParams_t*)pDataParams,bMemBank, pWordPtr, bWordPtrLength,bWordCount)

#define phalI18000p3m3_BlockPermaLock(pDataParams,bRFU,bReadLock,bMemBank,pBlockPtr,bBlockPtrLength,bBlockRange,pMask,ppRxBuffer,pRxBitLength) \
        phalI18000p3m3_Sw_BlockPermaLock((phalI18000p3m3_Sw_DataParams_t *)pDataParams,bRFU,bReadLock,bMemBank,pBlockPtr,bBlockPtrLength,bBlockRange,pMask,ppRxBuffer,pRxBitLength)

#define phalI18000p3m3_SetHandle( pDataParams, pHandle) \
        phalI18000p3m3_Sw_SetHandle((phalI18000p3m3_Sw_DataParams_t *) pDataParams, pHandle)

#else


/**
* \brief Acknowledge a single tag.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI18000p3m3_Ack(
                              void * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
                              uint8_t ** ppRxBuffer,    /**< [Out] Pointer to Tag data and, if required, PacketCRC. */
                              uint16_t * pRxBitLength   /**< [Out] Tag response length in bits. */
                              );

/**
* \brief Instruct a tag to loadmodulate a new RN16 or Handle.
*
* \c bOption is :\n
* \li #PHAL_I18000P3M3_REQRN_USE_HANDLE
* Other option is deprecated.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI18000p3m3_ReqRn(
                                void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                uint8_t bOption,        /**< [In] Option parameter. */
                                uint8_t ** ppRxBuffer   /**< [Out] New RN16 or handle. */
                                );

/**
* \brief Read part or all of a tag Reserved, UII, TID, or User memory.
* bWordPtrLength depends on the TAG memory size. For TAGs with 8 bits
* memory, bWordPtrLength should be always '0'. If we make 'bWordPtrLength' =1 (16bits)
* or higher for 8 bits memory TAGs then this function returns MEMORY_OVERRUN error.
* This is an expected behaviour.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI18000p3m3_Read(
                               void * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bMemBank,        /**< [In] Memory bank where the read shall be performed. */
                               uint8_t * pWordPtr,      /**< [In] Starting read address. */
                               uint8_t bWordPtrLength,  /**< [In] Length of the pointer in bytes; 0 -> 1byte,1->2bytes,2->3bytes or 3->4bytes. */
                               uint8_t bWordCount,      /**< [In] Number of bytes to read. */
                               uint8_t ** ppRxBuffer,   /**< [Out] Header and requested memory words. */
                               uint16_t * pRxBitLength  /**< [Out] Number of received bits. */
                               );

/**
* \brief Write a word in a tag Reserved, UII, TID, or User memory.
* bWordPtrLength depends on the TAG memory size. For TAGs with 8 bits
* memory, bWordPtrLength should be always '0'. If we make 'bWordPtrLength' =1 (16bits)
* or higher for 8 bits memory TAGs then this function returns MEMORY_OVERRUN error.
* This is an expected behaviour.
*
* \c bOption can be one of:\n
* \li #PHAL_I18000P3M3_AC_NO_COVER_CODING
* \li #PHAL_I18000P3M3_AC_USE_COVER_CODING
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI18000p3m3_Write(
                                void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                uint8_t bOption,        /**< [In] Option parameter. */
                                uint8_t bMemBank,       /**< [In] Memory bank where the write shall be performed. */
                                uint8_t * pWordPtr,     /**< [In] Starting write address. */
                                uint8_t bWordPtrLength, /**< [In] [In] Length of the pointer in bytes; 0 -> 1byte,1->2bytes,2->3bytes or 3->4bytes. */
                                uint8_t * pData         /**< [In] Word to write; uint8_t[2]. */
                                );

/**
* \brief Render a tag killed or recommissioned as appropriate.
*
* \c bOption can be one of:\n
* \li #PHAL_I18000P3M3_AC_NO_COVER_CODING
* \li #PHAL_I18000P3M3_AC_USE_COVER_CODING
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI18000p3m3_Kill(
                               void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bOption,     /**< [In] Option parameter. */
                               uint8_t * pPassword, /**< [In] Full kill password; uint8_t[4] */
                               uint8_t bRecom       /**< [In] Recommissioning bits. */
                               );

/**
* \brief Lock or Permalock individual passwords and memory banks.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI18000p3m3_Lock(
                               void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                               uint8_t * pMask,     /**< [In] 10bit Action Field Mask; uint8_t[2]. */
                               uint8_t * pAction    /**< [In] 10bit Action Field; uint8_t[2]. */
                               );

/**
* \brief Cause a tag with a non-zero-valued access password to transition from the open to the secured state.
*
* \c bOption can be one of:\n
* \li #PHAL_I18000P3M3_AC_NO_COVER_CODING
* \li #PHAL_I18000P3M3_AC_USE_COVER_CODING
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI18000p3m3_Access(
                                 void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                 uint8_t bOption,       /**< [In] Option parameter. */
                                 uint8_t * pPassword    /**< [In] Full access password; uint8_t[4] */
                                 );

/**
* \brief Write multiple words in a tag Reserved, UII, TID, or User memory.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI18000p3m3_BlockWrite(
                                     void * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bMemBank,          /**< [In] Memory bank where the write shall be performed. */
                                     uint8_t * pWordPtr,        /**< [In] Starting write adress. */
                                     uint8_t bWordPtrLength,    /**< [In] Length of the pointer in bytes; 1,2,3 or 4. */
                                     uint8_t bWordCount,        /**< [In] Number of blocks to write. */
                                     uint8_t * pData            /**< [In] Words to write; uint8_t[2U * \c bWordCount]. */
                                     );

/**
* \brief Erase multiple words in a tag Reserved, UII, TID, or User memory.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI18000p3m3_BlockErase(
                                     void * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bMemBank,          /**< [In] Memory bank where the erase shall be performed. */
                                     uint8_t * pWordPtr,        /**< [In] Starting erase adress. */
                                     uint8_t bWordPtrLength,    /**< [In] Length of the pointer in bytes; 1,2,3 or 4. */
                                     uint8_t bWordCount         /**< [In] Number of blocks to erase. */
                                     );

/**
* \brief Erase multiple words in a tag Reserved, UII, TID, or User memory.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI18000p3m3_BlockPermaLock(
    void * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bRFU,               /**< [In] RFU, shall be set to \c 0. */
    uint8_t bReadLock,          /**< [In] Whether the permalock states shall be retrieved (\c 0) or the blocks shall be permalocked (\c 1). */
    uint8_t bMemBank,           /**< [In] Memory bank where the erase shall be performed. */
    uint8_t * pBlockPtr,        /**< [In] Starting erase adress. */
    uint8_t bBlockPtrLength,    /**< [In] Length of the pointer in bytes; 1,2,3 or 4. */
    uint8_t bBlockRange,        /**< [In] Mask range, specified in units of 16 blocks. */
    uint8_t * pMask,            /**< [In] Specifies which memory blocks a tag permalocks; uint8_t[2U * \c bBlockRange] Ignored if \c bReadLock is \c 0 */
    uint8_t ** ppRxBuffer,      /**< [Out] Header and Permalock bits if \c bReadLock is \c 0 or NULL otherwise. */
    uint16_t * pRxBitLength     /**< [Out] Number of received bits if \c bReadLock is \c 0. */
    );

/**
* \brief Set the Handle into the internal data structure.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI18000p3m3_SetHandle(
    void * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
    uint8_t* pHandle            /**< [In] Handle to the Card. */
    );

/** @} */

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHAL_I18000P3M3 */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALI18000P3M3_H */
