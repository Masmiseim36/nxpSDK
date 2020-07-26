/*
*         Copyright (c), NXP Semiconductors Bangalore / India
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
* Generic Tag Operation Application Layer Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHALTOP_H
#define PHALTOP_H

#include <ph_Status.h>
#include <phacDiscLoop.h>
#include <phalT1T.h>
#include <phalMful.h>
#include <phalMfdf.h>
/* #define __DEBUG */

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */


#ifdef NXPBUILD__PHAL_TOP_SW

/** \defgroup phalTop_Sw Component : Software
* @{
*/
#define PHAL_TOP_SW_ID                           0x01U       /**< ID for Software Tag Operation layer */

#define PHAL_TOP_MAX_TAGTYPE_SUPPORTED           0x06U       /**< Maximum number of Tag Types Supported */

#define PHAL_TOP_T1T_MAX_MEM_CTRL_TLV            0x01U        /**< Max Memory control TLV count for T1T */
#define PHAL_TOP_T1T_MAX_LOCK_CTRL_TLV           0x01U        /**< Max Lock control TLV count for T1T */

#define PHAL_TOP_T2T_MAX_MEM_CTRL_TLV            0x01U        /**< Max Memory control TLV count for T2T */
#define PHAL_TOP_T2T_MAX_LOCK_CTRL_TLV           0x01U        /**< Max Lock control TLV count for T2T */

#define PHAL_TOP_T3T_READ_MAX_BLOCKS             0x0FU        /**< Max number of blocks supported in one read command. */
#define PHAL_TOP_T3T_WRITE_MAX_BLOCKS            0x0FU        /**< Max number of blocks supported in one write command. */

#define PHAL_TOP_T1T_NDEF_SUPPORTED_VNO          0x10U        /**< Version Number for T1T */
#define PHAL_TOP_T2T_NDEF_SUPPORTED_VNO          0x10U        /**< Version Number for T2T */
#define PHAL_TOP_T3T_NDEF_SUPPORTED_VNO          0x11U        /**< Version Number for T3T */
#define PHAL_TOP_T4T_NDEF_SUPPORTED_VNO          0x20U        /**< Version Number for T4T */
#define PHAL_TOP_T5T_NDEF_SUPPORTED_VNO          0x04U        /**< Version Number for T5T */


/** \name phalTop Custom Error Codes
*/
/** @{ */
#define PHAL_TOP_ERR_READONLY_TAG                  ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x01U)     /**< Tag is Read Only */
#define PHAL_TOP_ERR_INVALID_STATE                 ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x02U)     /**< Tag state is invalid */
#define PHAL_TOP_ERR_FORMATTED_TAG                 ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x03U)     /**< Tag already in NDEF formatted state. */
#define PHAL_TOP_ERR_UNSUPPORTED_VERSION           ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x04U)     /**< Unsupported NDEF version. */
#define PHAL_TOP_ERR_MISCONFIGURED_TAG             ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x05U)     /**< Tag not configured as per NDEF specification. */
#define PHAL_TOP_ERR_UNSUPPORTED_TAG               ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x06U)     /**< Tag with unsupported structure/format. */
#define PHAL_TOP_ERR_EMPTY_NDEF                    ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x07U)     /**< NDEF message length is zero (i.e. Tag in initialized state). */
#define PHAL_TOP_ERR_NON_NDEF_TAG                  ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x08U)     /**< Tag is not NDEF formatted. */
/** @} */

#ifdef NXPBUILD__PHAL_T1T_SW
/**
* \brief T1T Lock Control TLV parameter structure
*/
typedef struct /*phalTop_T1T_LockCtrlTlv*/
{
    uint16_t wOffset;                                             /**< TLV offset */
    uint16_t wByteAddr;                                           /**< Lock bytes start address/offset */
    uint8_t bSizeInBits;                                          /**< Lock bytes size in bits */
    uint8_t bBytesPerPage;                                        /**< Number of Bytes per page */
    uint8_t bBytesLockedPerBit;                                   /**< Number of bytes locked per lock bit */
}phalTop_T1T_LockCtrlTlv_t;

/**
* \brief T1T Memory Control TLV parameter structure
*/
typedef struct /*phalTop_T1T_MemCtrlTlv*/
{
    uint16_t wOffset;                                             /**< TLV offset */
    uint16_t wByteAddr;                                           /**< Reserved memory start address/offset */
    uint8_t bSizeInBytes;                                         /**< Reserved memory size in bytes */
    uint8_t bBytesPerPage;                                        /**< Number of Bytes per page */
}phalTop_T1T_MemCtrlTlv_t;

/**
* \brief T1T Proprietary TLV parameter structure
*/
typedef struct /*phalTop_T1T_ProprietaryTlv*/
{
    uint16_t wOffset;                                             /**< TLV address/offset */
    uint16_t wLength;                                             /**< TLV Length */
}phalTop_T1T_ProprietaryTlv_t;

/**
* \brief T1T Segment parameters
*/
typedef struct /*phalTop_TIT_Segment*/
{
    uint8_t bAddress;                                             /**< Current active segment address */
    uint8_t * pData;                                              /**< Segment data (points to HAL RX buffer) */
    uint8_t bLockReservedOtp[16];                                 /**< Lock/reserved/OTP bytes in current segment */
}phalTop_TIT_Segment_t;
#endif /* NXPBUILD__PHAL_T1T_SW */

#ifdef NXPBUILD__PHAL_MFUL_SW
/**
* \brief T2T Lock Control TLV parameter structure
*/
typedef struct /* phalTop_T2T_LockCtrlTlv */
{
    uint16_t wOffset;                                             /**< TLV offset */
    uint16_t wByteAddr;                                           /**< Lock bytes start address/offset */
    uint8_t bSizeInBits;                                          /**< Lock bytes size in bits */
    uint8_t bBytesPerPage;                                        /**< Number of Bytes per page */
    uint8_t bBytesLockedPerBit;                                   /**< Number of bytes locked per lock bit */
}phalTop_T2T_LockCtrlTlv_t;

/**
* \brief T2T Memory Control TLV parameter structure
*/
typedef struct /*phalTop_T2T_MemCtrlTlv*/
{
    uint16_t wOffset;                                             /**< TLV offset */
    uint16_t wByteAddr;                                           /**< Reserved memory start address/offset */
    uint8_t bSizeInBytes;                                         /**< Reserved memory size in bytes */
    uint8_t bBytesPerPage;                                        /**< Number of Bytes per page */
}phalTop_T2T_MemCtrlTlv_t;

/**
* \brief T2T Sector parameters
*/
typedef struct /*phalTop_T2T_Sector*/
{
    uint8_t bAddress;                                            /**< Current active Sector address */
    uint8_t bBlockAddress;                                       /**< Current active block address */
    uint8_t bLockReservedOtp[32];                                /**< Lock/reserved/OTP bytes in current Sector */
    uint8_t bValidity;                                           /**< Read data validity */
}phalTop_T2T_Sector_t;
#endif /* NXPBUILD__PHAL_MFUL_SW */

#ifdef NXPBUILD__PHAL_T1T_SW
/**
* \brief T1T parameter structure
*/
typedef struct /*phalTop_T1T*/
{
    void * pAlT1TDataParams;                                                   /**< Pointer to T1T data parameters */
    uint8_t bRwa;                                                               /**< Tag read/write size */
    uint8_t bTms;                                                               /**< Tag Memory size */
    uint8_t bTagMemoryType;                                                     /**< Tag memory type (static or dynamic) */
    uint8_t bTerminatorTlvPresence;                                             /**< Terminator TLV presence; 1 -present, 0 - not present */
    uint8_t bLockTlvCount;                                                      /**< Number of lock TLV present in tag */
    uint8_t bMemoryTlvCount;                                                    /**< Number of memory TLV present in tag */
    uint16_t wNdefHeaderAddr;                                                   /**< Header offset of first NDEF message */
    uint16_t wNdefMsgAddr;                                                      /**< NDEF message start address */
    uint8_t bUid[4];                                                            /**< T1T UID */
    phalTop_T1T_MemCtrlTlv_t asMemCtrlTlv[PHAL_TOP_T1T_MAX_MEM_CTRL_TLV];       /**< Memory TLV details for each TLV present */
    phalTop_T1T_LockCtrlTlv_t asLockCtrlTlv[PHAL_TOP_T1T_MAX_LOCK_CTRL_TLV];    /**< Lock TLV details for each TLV present */
    phalTop_TIT_Segment_t sSegment;                                             /**< Current segment details */
}phalTop_T1T_t;
#endif /* NXPBUILD__PHAL_T1T_SW */

#ifdef NXPBUILD__PHAL_MFUL_SW
/**
* \brief T2T parameter structure
*/
typedef struct /*phalTop_T2T*/
{
    void * pAlT2TDataParams;                                                     /**< Pointer to T2T data parameters */
    uint8_t bRwa;                                                                /**< Tag read/write size */
    uint8_t bTms;                                                                /**< Tag Memory size */
    uint8_t bTagMemoryType;                                                      /**< Tag memory type (static or dynamic) */
    uint8_t bLockTlvCount;                                                       /**< Number of lock TLV present in tag */
    uint8_t bMemoryTlvCount;                                                     /**< Number of memory TLV present in tag */
    uint16_t wNdefHeaderAddr;                                                    /**< Header offset of first NDEF message */
    uint16_t wNdefMsgAddr;                                                       /**< NDEF message start address */
    phalTop_T2T_MemCtrlTlv_t asMemCtrlTlv[PHAL_TOP_T2T_MAX_MEM_CTRL_TLV];        /**< Memory TLV details for each TLV present */
    phalTop_T2T_LockCtrlTlv_t asLockCtrlTlv[PHAL_TOP_T2T_MAX_LOCK_CTRL_TLV];     /**< Lock TLV details for each TLV present */
    phalTop_T2T_Sector_t sSector;                                                /**< Current segment details */
}phalTop_T2T_t;
#endif /* NXPBUILD__PHAL_MFUL_SW */

#ifdef NXPBUILD__PHAL_FELICA_SW
/**
* \brief T3T parameter structure
*/
typedef struct /*phalTop_T3T*/
{
    void * pAlT3TDataParams;                                                    /**< Pointer to FeliCa data parameters */
    uint8_t bRwa;                                                                /**< Tag read/write access; 1 - Read/Write, 0 - Read Only */
    uint8_t bNbr;                                                                /**< Number of Blocks that can be read at one time*/
    uint8_t bNbw;                                                                /**< Number of Blocks that can be written at one time*/
    uint16_t bNmaxb;                                                             /**< Maximum number of 'Blocks * 16' available for NDEF data */
    uint8_t bUid[16];                                                            /**< UID to address T3T */
    uint8_t bAttributeBlock[16];                                                 /**< Contains the attribute block of the Ndef Type 3 tag */
}phalTop_T3T_t;
#endif /* NXPBUILD__PHAL_FELICA_SW */

#ifdef NXPBUILD__PHAL_MFDF_SW
/**
* \brief T4T parameter structure
*/
typedef struct /*phalTop_T4T*/
{
    void * pAlT4TDataParams;                                                     /**< Pointer to T4T data parameters */
    uint8_t aNdefFileID[2];                                                      /**< NDEF File ID to Create */
    uint8_t bRa;                                                                 /**< NDEF file read access condition size */
    uint8_t bWa;                                                                 /**< NDEF file write access condition size */
    uint8_t bCurrentSelectedFile;                                                /**< Current selected file */
    uint16_t wMLe;                                                               /**< MLe supported by card (obtained from CC) */
    uint16_t wMLc;                                                               /**< MLc supported by card (obtained from CC) */
    uint16_t wCCLEN;                                                             /**< Size of CCr,Valid CCLEN range 000Fh-FFFEh.*/
    uint16_t wMaxFileSize;                                                       /**< Maximum NDEF file size in bytes */
}phalTop_T4T_t;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_ICODE_SW
/**
* \brief T5T parameter structure
*/
typedef struct /*phalTop_T5T*/
{
    void *  pAlI15693DataParams;                                                  /**< Pointer to T5T data parameters */
    uint8_t bRwa;                                                                /**< Tag read/write size */
    uint8_t bTerminatorTlvPresence;                                              /**< Terminator TLV presence; 1 -present, 0 - not present */
    uint8_t bMbRead;                                                             /**< Flag Bit for Multiple Read support */
    uint8_t bLockBlock;                                                          /**< Flag Bit for Lock Command support */
    uint8_t bSplFrm;                                                             /**< Flag Bit for special frame support */
    uint8_t bExtendedCommandSupport;                                             /**< Flag Bit for Extended Command Support support */
    uint8_t bOptionFlag;                                                         /**< Option Flag PH_ON = Enable, PH_OFF = Disable */
    uint16_t wMlen;                                                              /**< Tag Memory size */
    uint16_t wNdefHeaderAddr;                                                    /**< Header offset of first NDEF message */
    uint16_t wNdefMsgAddr;                                                       /**< NDEF message start address */
    uint16_t bBlockSize;                                                          /**< T5T Block Size supported by card */
    uint16_t bMaxBlockNum;                                                       /**< Maximum Block number supported by Card */
}phalTop_T5T_t;
#endif /* NXPBUILD__PHAL_ICODE_SW */

#ifdef  NXPBUILD__PHPAL_I14443P3A_SW

/**
* \name MIFARE Classic contactless IC Card Types
*/
/*@{*/
#define PHAL_TOP_NO_MFC                         0x00U       /**< No Card is selected */
#define PHAL_TOP_MFC_1K                         0x01U       /**< MFC 1K is selected*/
#define PHAL_TOP_MFC_4K                         0x02U       /**< MFC 4K is selected*/
#define PHAL_TOP_MFP_2K                         0x03U       /**< MFP 2K is selected*/
#define PHAL_TOP_MFP_4K                         0x04U       /**< MFP 4K is selected*/


/**
* \brief Tag Operations parameter structure
*/
typedef struct  /*phalTop_MfcTop*/
{
    void * pPalI14443paDataParams;                /**< Pointer to the parameter structure of the type A layer 3. */
    uint8_t bCardType;                            /**< Selected Card Type can be MFC1K, MFC4K, MFP2K, MFP4K */
    uint8_t bFirstNdefSector;                     /**< First Sector with NDEF TLV */
    uint8_t bPreFormatted;                        /**< Card is non default and is formatted incorrect */
    uint8_t bNdefSectorCount;                     /**< Count of sectors with Ndef AID present */
    uint8_t bOffset;                              /**< Offset Used while writing the Ndef */
    uint8_t bNdefMessageStart;                    /**< In a Ndef Sector the Ndef Message Starts from this byte */
}phalTop_MfcTop_t;

#endif /* NXPBUILD__PHPAL_I14443P3A_SW */

/**
* \brief Tag Operations parameter structure
*/
typedef struct  /*phalTop_Sw_DataParams*/
{
    uint16_t wId;                                 /**< Layer ID for this component, NEVER MODIFY! */
    uint8_t bTagType;                             /**< Tag type */
    uint8_t bVno;                                 /**< NDEF version number */
    uint8_t bTagState;                            /**< Tag state */
    uint16_t wNdefLength;                         /**< NDEF message length */
    uint16_t wMaxNdefLength;                      /**< Maximum supported NDEF length */
    void *  pTopTagsDataParams[PHAL_TOP_MAX_TAGTYPE_SUPPORTED];  /**< Array of void pointer for Tag Types structure */
    union
    {
#ifdef NXPBUILD__PHAL_T1T_SW
        phalTop_T1T_t   salTop_T1T;               /**< T1T Parameter Structure */
#endif /* NXPBUILD__PHAL_T1T_SW*/
#ifdef NXPBUILD__PHAL_MFUL_SW
        phalTop_T2T_t   salTop_T2T;               /**< T2T Parameter Structure */
#endif /* NXPBUILD__PHAL_MFUL_SW*/
#ifdef NXPBUILD__PHAL_FELICA_SW
        phalTop_T3T_t   salTop_T3T;               /**< T3T Parameter Structure */
#endif /* NXPBUILD__PHAL_FELICA_SW */
#ifdef NXPBUILD__PHAL_MFDF_SW
        phalTop_T4T_t   salTop_T4T;               /**< T4T Parameter Structure */
#endif /* NXPBUILD__PHAL_MFDF_SW */
#ifdef NXPBUILD__PHAL_ICODE_SW
        phalTop_T5T_t   salTop_T5T;               /**< T5T Parameter Structure */
#endif /* NXPBUILD__PHAL_ICODE_SW*/
#ifdef NXPBUILD__PHPAL_I14443P3A_SW
        phalTop_MfcTop_t salTop_MfcTop;           /**< MFC as Tag Type Parameter structure */
#endif /* NXPBUILD__PHPAL_I14443P3A_SW*/
    } ualTop;

}phalTop_Sw_DataParams_t;

/**
* \brief Initialize this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on error type
*/
phStatus_t phalTop_Sw_Init(
    phalTop_Sw_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wSizeOfDataParams,                   /**< [In] Specifies the size of the data parameter structure. */
    void * pAlT1T,                                /**< Pointer to T1T data parameters */
    void * pAlMful,                               /**< Pointer to T2T data parameters */
    void * pAlFelica,                             /**< Pointer to FeliCa data parameters */
    void * pAlMfdf,                               /**< Pointer to T4T data parameters */
    void * pAl15693,                                /**< Pointer to T5T data parameters */
    void * pPalI14443paDataParams                  /**< Pointer to Layer 3a data paramters */
    );
/** @} */
#endif /* NXPBUILD__PHAL_TOP_SW */

#ifdef NXPBUILD__PHAL_TOP

/** \defgroup phalTop Tag Operation Layer
* \brief This Component implements tag operations for different NFC Tag types T1T, T2T, T3T,T4T and T5T.
* To perform any operation like Read, Write, Lock, format and erase operation on the tag. The Tag should be
* activated first as shown in the below block diagram. Any Read/Write operation is possible on the Tag only
* if the tag is NDEF format. Refer NFC Forum Tag operation Specifications.
*
* \image html TOP_BlockDiagram.png
*
* @{
*/
/**
* \name Tag types
* \brief Tag types are used to set Tag used using \ref phalTop_SetConfig with \ref PHAL_TOP_CONFIG_TAG_TYPE Type
*/
/*@{*/
#define PHAL_TOP_TAG_TYPE_T1T_TAG                   0x01U                /**< Type 1 Tag. */
#define PHAL_TOP_TAG_TYPE_T2T_TAG                   0x02U                /**< Type 2 Tag. */
#define PHAL_TOP_TAG_TYPE_T3T_TAG                   0x03U                /**< Type 3 Tag. */
#define PHAL_TOP_TAG_TYPE_T4T_TAG                   0x04U                /**< Type 4 Tag. */
#define PHAL_TOP_TAG_TYPE_T5T_TAG                   0x05U                /**< Type 5 Tag. */
#define PHAL_TOP_TAG_TYPE_MFC_TOP                   0x06U                /**< MFC as Tag Type */
/*@}*/

/**
* \name Configuration types
* \brief Use these Macro in \ref phalTop_SetConfig to configure Tags.
*/
/*@{*/
#define PHAL_TOP_CONFIG_TAG_TYPE                    0x51U                /**< Set/Get Tag type. Should be configured before calling CheckNdef. */
#define PHAL_TOP_CONFIG_TAG_STATE                   0x52U                /**< Get tag state. This shall be also used to set tag to read-only state. */
#define PHAL_TOP_CONFIG_NDEF_LENGTH                 0x53U                /**< Get current NDEF message Length. */
#define PHAL_TOP_CONFIG_MAX_NDEF_LENGTH             0x54U                /**< Get Max support NDEF Length by tag. */
#define PHAL_TOP_CONFIG_NDEF_VERSION                0x55U                /**< Get NDEF Version Number. */

#define PHAL_TOP_CONFIG_T1T_TMS                     0x03U                /**< Set tag memory size. Set before format operation. */
#define PHAL_TOP_CONFIG_T1T_TERMINATOR_TLV          0x05U                /**< Set Terminator TLV presence. Set before format/write operation to enable writing terminator TLV at end of NDEF TLV. */

#define PHAL_TOP_CONFIG_T2T_TMS                     0x09U                /**< Set tag memory size. Set before format operation. */

#define PHAL_TOP_CONFIG_T4T_NDEF_FILE_ID            0x15U                /**< Set NDEF file ID. Set before format operation. */
#define PHAL_TOP_CONFIG_T4T_NDEF_FILE_SIZE          0x18U                /**< Set Max NDEF length. Set before format operation. */
#define PHAL_TOP_CONFIG_T4T_MLE                     0x19U                /**< Set MLe. Set before format operation. */
#define PHAL_TOP_CONFIG_T4T_MLC                     0x1AU                /**< Set MLc. Set before format operation. */

#define PHAL_TOP_CONFIG_T5T_MLEN                    0x1DU                /**< Set T5T NDEF data area. Set before format operation. */
#define PHAL_TOP_CONFIG_T5T_MBREAD                  0x20U                /**< Set T5T multiple block read support. Set before format operation. */
#define PHAL_TOP_CONFIG_T5T_LOCKBLOCK               0x21U                /**< Set T5T Lock block command support. Set before format operation. */
#define PHAL_TOP_CONFIG_T5T_SPL_FRM                 0x22U                /**< Set T5T special frame support. Set before format operation. */
#define PHAL_TOP_CONFIG_T5T_OPTION_FLAG             0x26U                /**< Set T5T option Flag. Set before format operation. */
#define PHAL_TOP_CONFIG_T5T_TERMINATOR_TLV          0x1FU                /**< Set Terminator TLV presence. Set before format/write operation to enable writing terminator TLV at end of NDEF TLV. */

#define PHAL_TOP_CONFIG_MFCTOP_CARD_TYPE            0x60U                /**< Set the Card Type */
/*@}*/

/**
* \name Tag States
*/
/*@{*/
#define PHAL_TOP_STATE_NONE                     0x00U                /**< Default initial state. */
#define PHAL_TOP_STATE_INITIALIZED              0x01U                /**< Initialized state. */
#define PHAL_TOP_STATE_READONLY                 0x02U                /**< Read Only state. */
#define PHAL_TOP_STATE_READWRITE                0x04U                /**< Read/Write state. */
/*@}*/

#define PHAL_TOP_T1T_TAG_MEMORY_TYPE_STATIC      0x00U        /**< Static memory type */
#define PHAL_TOP_T1T_TAG_MEMORY_TYPE_DYNAMIC     0x01U        /**< Dynamic memory type */

#define PHAL_TOP_T2T_TAG_MEMORY_TYPE_STATIC      0x00U        /**< Static memory type */
#define PHAL_TOP_T2T_TAG_MEMORY_TYPE_DYNAMIC     0x01U        /**< Dynamic memory type */

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phalTop/src/Sw/phalTop_Sw.h"

#define phalTop_ReadNdef(pDataParams, pData, pLength) \
        phalTop_Sw_ReadNdef( (phalTop_Sw_DataParams_t *)pDataParams, pData, pLength)

#define phalTop_WriteNdef(pDataParams, pData, pLength) \
        phalTop_Sw_WriteNdef( (phalTop_Sw_DataParams_t *)pDataParams, pData, pLength)

#define phalTop_CheckNdef(pDataParams, pNdefPresence) \
        phalTop_Sw_CheckNdef( (phalTop_Sw_DataParams_t *)pDataParams, pNdefPresence)

#define phalTop_Reset(pDataParams) \
        phalTop_Sw_Reset( (phalTop_Sw_DataParams_t *)pDataParams)

#define  phalTop_SetConfig(pDataParams, wConfig, wValue) \
         phalTop_Sw_SetConfig( (phalTop_Sw_DataParams_t *)pDataParams, wConfig, wValue)

#define phalTop_GetConfig(pDataParams, wConfig, pValue) \
        phalTop_Sw_GetConfig( (phalTop_Sw_DataParams_t *)pDataParams, wConfig, pValue)

#define phalTop_EraseNdef(pDataParams) \
        phalTop_Sw_EraseNdef( (phalTop_Sw_DataParams_t *)pDataParams)

#define phalTop_FormatNdef(pDataParams) \
        phalTop_Sw_FormatNdef( (phalTop_Sw_DataParams_t *)pDataParams)

#else



/**
* \brief Format a new non-NDEF tag as NDEF tag.
*
* FormatNdef shall be used for formatting a non-NDEF tag as NDEF tag if needed
* when CheckNdef returns error. Before formatting, tag parameters like max. NDEF
* size etc., shall be specified by the application. If not specified tag will be
* formatted with default values. FormatNdef shall be called
* only once for a tag. Once formatted, tag will become a NDEF tag. To remove NDEF
* format if needed, application needs to over write / delete NDEF format using
* tag specific read/write commands.
*
* This is only a utility function not specified by NFC Forum.
* Please note that for T4T, this will attempt to use MIFARE DESFire contactless IC proprietary
* commands to format the tag. If T4T is not MIFARE DESFire tag then format
* will not succeed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PHAL_TOP_ERR_UNSUPPORTED_TAG Tag does not support NDEF formatting.
* \retval #PHAL_TOP_ERR_FORMATTED_TAG Tag is already NDEF formatted.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalTop_FormatNdef(
    void * pDataParams            /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief Read NDEF message from Tag.
*
* ReadNdef shall only be called after CheckNdef returned success.
* If it is an empty NDEF message(i.e. initialized state) then this will return
* empty NDEF error.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PHAL_TOP_ERR_INVALID_STATE Tag is not is any valid state (i.e. when check NDEF failed or has not been called before.)
* \retval #PHAL_TOP_ERR_EMPTY_NDEF Tag is in initialized state (i.e. no NDEF / empty NDEF)
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalTop_ReadNdef(
    void * pDataParams,           /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pData,              /**< [Out] NDEF data from the Tag. User has to allocate memory accordingly */
    uint16_t * pLength            /**< [Out] NDEF data length. */
    );

/**
* \brief Write NDEF message into Tag.
*
* WriteNdef shall be used to write a new NDEF message to tag if check NDEF
* returned success. If tag is in read only state, write NDEF will return error.
* The tag is expected to be a formatted NDEF tag for this to succeed.
*
* WriteNDEF will update the NDEF message TLV or the capability container with
* the length of the data written to the tag.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER wLength is 0 or wLength is more than supported max length.
* \retval #PHAL_TOP_ERR_INVALID_STATE Tag is not is any valid state (i.e. when check NDEF failed).
* \retval #PHAL_TOP_ERR_READONLY_TAG Tag is in read only state.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalTop_WriteNdef(
    void * pDataParams,           /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pData,              /**< [In] NDEF data to be written to tag. User has to allocate memory accordingly */
    uint16_t wLength              /**< [In] Length of NDEF data to be written. */
    );

/**
* \brief Does the NDEF detection procedure as per NFC Tag Operation
* specifications for each tag type.
*
* The caller has to ensure that Tag activation is done before calling this API.
* Also \ref phalTop_SetConfig should be called before to configure the Tag type
* #PHAL_TOP_CONFIG_TAG_TYPE. For MIFARE Classic as tag type \ref phalTop_SetConfig needs to be called
* to configure the MIFARE Classic IC-based contactless card type #PHAL_TOP_CONFIG_MFCTOP_CARD_TYPE e.g. #PHAL_TOP_MFC_1K.
* Only after phalTop_CheckNdef is called any other
* NDEF operation on Tag can be performed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PHAL_TOP_ERR_NON_NDEF_TAG Tag don't indicate NDEF presence
* \retval #PHAL_TOP_ERR_UNSUPPORTED_VERSION Tag indicates NDEF presence but NDEF version mentioned in tag is not supported by reader library.
* \retval #PHAL_TOP_ERR_MISCONFIGURED_TAG Tag indicates NDEF presence but the NDEF CC or NDEF attribute information is wrongly configured
* \retval #PHAL_TOP_ERR_UNSUPPORTED_TAG Tag uses some proprietary or RFU or unsupported configuration
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalTop_CheckNdef(
    void * pDataParams,           /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pTagState           /**< [Out] State of tag */
    );

/**
* \brief Reset Tag Operation parameters
*
* User has to call phalTop_Reset to reset all the Software parameters. This shall
* be called after performing all NDEF operations if needed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalTop_Reset(
    void * pDataParams            /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief Erase a valid NDEF message with an empty NDEF.
*
* EraseNdef shall be used to erase a valid NDEF message by writing an empty NDEF
* (i.e. NDEF length as 0) to tag. This will change the tag from read/write state to
* initialized state. If tag is already in initialized state this API will return error.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PHAL_TOP_ERR_INVALID_STATE Tag is not is any valid state (i.e. when check NDEF failed)
* \retval #PHAL_TOP_ERR_READONLY_TAG Tag is in read only state.
* \retval #PHAL_TOP_ERR_EMPTY_NDEF Tag is in initialized state (i.e. no NDEF / empty NDEF)
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalTop_EraseNdef(
    void * pDataParams            /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief Set configuration parameter.
* Refer to #PHAL_TOP_CONFIG_TAG_TYPE from where the configurable parameters are listed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
*/
phStatus_t phalTop_SetConfig(
    void * pDataParams,           /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wConfig,             /**< [In] Configuration Identifier. */
    uint16_t wValue               /**< [In] Configuration Value. */
    );

/**
* \brief Get configuration parameter.
*
* After calling \ref phalTop_CheckNdef, \ref phalTop_WriteNdef, \ref phalTop_ReadNdef
* or \ref phalTop_EraseNdef, user can call phalTop_GetConfig to get different
* parameters.
*
* Refer to #PHAL_TOP_CONFIG_TAG_TYPE from where the configurable parameters are listed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
*/
phStatus_t phalTop_GetConfig(
    void * pDataParams,           /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wConfig,             /**< [In] Configuration Identifier. */
    uint16_t * pValue             /**< [Out] Configuration Value. */
    );

/** @} */

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHAL_TOP */

#ifdef __cplusplus
} /* Extern C */
#endif

#ifdef __DEBUG
/* This API would be available only for debugging purpose and test bench */
/**
* \brief   Assigns the component data parameter to one of the filed in
*          T1T parameter based on the option
*
* \return  Error code
* \retval  #PH_ERR_INVALID_PARAMTER  If invalid option was passed
* \retval  #PH_ERR_SUCCESS           on success
*
*/
phStatus_t phalTop_SetPtr(
    void * pDataParams,           /**< [In] Pointer to this layer's parameter structure */
    void * pT1T,                  /**< [In] Pointer to Tag Type 1 parameter structure */
    void * pT2T,                  /**< [In] Pointer to Tag Type 2 parameter structure */
    void * pT3T,                  /**< [In] Pointer to Tag Type 3 parameter structure */
    void * pT4T,                   /**< [In] Pointer to Tag Type 4 parameter structure */
    void * pT5T,                   /**< [In] Pointer to Tag Type 5 parameter structure */
    void * pMfcTop                  /**< [In] Pointer to Tag Type MFC parameter structure */
    );
#endif /* __DEBUG */
#endif  /* PHALTOP_H */
