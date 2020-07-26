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
* Generic Sli15693 Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  RSn: Generated 21. October 2009
*
*/

#ifndef PHPALSLI15693_H
#define PHPALSLI15693_H

#include <ph_Status.h>
#include <phhalHw.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHPAL_SLI15693_SW

/** \defgroup phpalSli15693_Sw Component : Software
* @{
*/
/**
 * ID for this layer (Software Sli15693 layer).
 *
 * \ref phpalSli15693_Sw_DataParams::wId is initialized with this ID in the
 * initialization function \ref phpalSli15693_Sw_Init.
 * */
#define PHPAL_SLI15693_SW_ID   0x01U

/**
* \brief Software Sli15693 PAL parameter structure
*/
typedef struct
{
    /**
     * ID for this component, not to be modified by user.
     *
     * This is initialized with #PHPAL_SLI15693_SW_ID in the initialization
     * function \ref phpalSli15693_Sw_Init. This is used for validating the
     * data parameter structure and to make sure that APIs are called after
     * initialization.
     * */
    uint16_t wId;

    /**
     * Pointer to the HAL data parameter.
     *
     * This pointer is initialized with the address of HAL data parameter
     * structure by \ref phpalSli15693_Sw_Init.
     * */
    void  * pHalDataParams;

    /**
     * Holds the last error code sent by the VICC. But for Authentication
     * Scenario contains the Flag Byte[non-error] as well.
     *
     * When a response is received from the VICC with the error flag
     * #PHPAL_SLI15693_FLAG_RESP_ERROR set, the error code returned by VICC is
     * stored here.
     * */
    uint16_t wAdditionalInfo;

    /**
     * Flags.
     *
     * This holds the value of \ref req_flags "request flag" field used in the
     * commands sent by the library.
     * */
    uint8_t bFlags;

    /**
     * VICC's UID.
     *
     * This holds the value of VICC's Unique Identifier received during the
     * inventory process, which always is of 8 bytes long.
     * */
    uint8_t pUid[8];

    /**
     * VICC's UID length.
     *
     * This holds the value of VICC's UID length in bits. After successful
     * inventory process, the complete UID length will be 64 bits.
     * */
    uint8_t bUidBitLength;

    /**
     * Indicator of API to be in explicit Addressed mode
     *
     * This indicates that the api is explicitly asked to be used in addressed mode
     * this parameter differentiate the understanding of explicitly addressed mode from the apis like select
     * and reset to ready which are implicitly addressed
     * */
    uint8_t bExplicitlyAddressed;

    /**
     * Operation Mode. One of NFC, EMVCo, ISO.
     */
    uint8_t bOpeMode;

    /**
     * Enable or disable the data buffering.
     */
    uint8_t bBuffering;

} phpalSli15693_Sw_DataParams_t;

/**
 * \brief Initialize PAL Sli15693 layer.
 *
 * This should be called before using any of the PAL Sli15693 APIs to do
 * initialization of the layer. This configures the Sli15693 PAL with default
 * configurations. Refer \ref config_palsli "configuration options" for
 * supported configurations and their default values.
 *
 * @param[in] pDataParams    Pointer to PAL Sli15693 data parameter
 * structure \ref phpalSli15693_Sw_DataParams_t.
 *
 * @param[in] wSizeOfDataParams    Specifies the size of data parameter
 * structure \ref phpalSli15693_Sw_DataParams_t.
 *
 * @param[in] pHalDataParams    Pointer to the HAL data parameter structure.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS Invalid data parameter. Size of data
 *         parameter structure \ref phpalSli15693_Sw_DataParams_t specified in
 *         wSizeOfDataParams is wrong.
 */
phStatus_t phpalSli15693_Sw_Init(
                                 phpalSli15693_Sw_DataParams_t * pDataParams,
                                 uint16_t wSizeOfDataParams,
                                 void * pHalDataParams
                                 );

/** @} */
#endif /* NXPBUILD__PHPAL_SLI15693_SW */



#ifdef NXPBUILD__PHPAL_SLI15693

/** \addtogroup ph_Error
* phpalSli15693 Custom Errors
* @{
*/

/** \name phpalSli15693 Error Codes
*/
/*@{*/
/**
 * ISO15693 specific error code from card.
 *
 * When a response is received from the VICC with the error flag
 * #PHPAL_SLI15693_FLAG_RESP_ERROR set, this error code returned by library.
 * The actual error code returned by VICC can be accessed via
 * \ref phpalSli15693_GetConfig using #PHPAL_SLI15693_CONFIG_ADD_INFO option.
 * */
#define PHPAL_SLI15693_ERR_ISO15693     ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0U)
/*@}*/

/** @} */

/** \defgroup phpalSli15693 ISO/IEC 15693 / ISO/IEC 18000-3M1
* \brief These component implements the protocol layer (activation) commands of
* ISO/IEC 15693 (ISO/IEC 18000-3M1) vicinity protocol.
*
* This implementation is based on ISO/IEC 15693-3:2009(E) specification. This
* layer also implements all activation related custom commands of NXP's ICODE
* SLI series of tags.
*
* Different states of a VICC (Vicinity Integrated Circuit Card) and
* corresponding commands are shown below.
*
* \image html palSli15693_states.png
*
* \b Note: PERSISTENT QUIET state is supported only by few ICODE SLI tags (like
* ICODE SLIX2). Refer \ref phpalSli15693_Inventory "function documentation" for
* more information on commands.
* @{
*/

/**
* \name Configuration Types
* \anchor config_palsli
* \brief These options shall be used along with \ref phpalSli15693_SetConfig and
* \ref phpalSli15693_GetConfig to configure PAL Sli15693 layer.
*/
/*@{*/
/**
 * Set or get the flags field.
 *
 * This shall be used to set or get the value of \ref req_flags "request flag"
 * field used in the commands sent by the library.
 * */
#define PHPAL_SLI15693_CONFIG_FLAGS           0x0000U

/**
 * Get the additional error info.
 *
 * When a response is received from the VICC with the error flag
 * #PHPAL_SLI15693_FLAG_RESP_ERROR set, library returns the custom error code
 * #PHPAL_SLI15693_ERR_ISO15693. This configuration option can be used to get
 * the actual error code returned by VICC.
 * */
#define PHPAL_SLI15693_CONFIG_ADD_INFO        0x0001U

/**
 * Set or get the current timeout in microseconds.
 * */
#define PHPAL_SLI15693_CONFIG_TIMEOUT_US      0x0002U

/**
 * Set or get the current timeout in milliseconds.
 * */
#define PHPAL_SLI15693_CONFIG_TIMEOUT_MS      0x0003U


/**
 * Set or get the current RX Data rate
 */
#define PHPAL_SLI15693_CONFIG_RXDATARATE       0x0005U


/*
 * Option to Get / Set the bufferring of response data to be enabled or disabled.
 * By default this flag will be disabled.
 */
#define PHPAL_SLI15693_CONFIG_ENABLE_BUFFERING    0x0007U
/*@}*/

/**
* \name EOF command options
* \anchor option_sendeof
* \brief Options for \ref phpalSli15693_SendEof "EOF" command.
*/
/*@{*/
/**
 * Send an EOF with timing configuration for an \ref phpalSli15693_Inventory
 * "inventory" round.
 * */
#define PHPAL_SLI15693_EOF_NEXT_SLOT                0x00U

/**
 * Send an EOF with timing configuration for \ref phpalSli15693_InventoryRead
 * "INVENTORY READ" or \ref phpalSli15693_FastInventoryRead "fast inventory read"
 * or \ref phpalSli15693_InventoryPageRead "inventory page read" or \ref
 * phpalSli15693_FastInventoryPageRead "fast inventory page read".
 * */
#define PHPAL_SLI15693_EOF_NEXT_SLOT_INV_READ       0x01U

/**
 * Send an EOF with timing configuration for write alike commands without
 * long waiting time #PHPAL_SLI15693_TIMEOUT_LONG_US.
 * */
#define PHPAL_SLI15693_EOF_WRITE_ALIKE              0x02U

/**
 * Send an EOF with timing configuration for write alike commands with
 * long waiting time #PHPAL_SLI15693_TIMEOUT_LONG_US.
 * */
#define PHPAL_SLI15693_EOF_WRITE_ALIKE_WITH_WAIT    0x03U
/*@}*/


/**
* \name Activate Card command options
*
* Options for \ref phpalSli15693_ActivateCard "activate card".
*/
/*@{*/
/**
 * Card will be in addressed state (i.e. SELECT command is not send) afteR
 * \ref phpalSli15693_ActivateCard "activate card" command.
 *
 * After activate card with this option, to bring VICC to SELECTED state,
 * \ref phpalSli15693_Select "select" command can be used.
 * */
#define PHPAL_SLI15693_ACTIVATE_ADDRESSED   0x0000U

/**
 * Card will be in selected state (i.e. SELECT command is send) after
 * \ref phpalSli15693_ActivateCard "activate card" command.
 * */
#define PHPAL_SLI15693_ACTIVATE_SELECTED    0x0001U

/**
 * Default option.
 *
 * Same as #PHPAL_SLI15693_ACTIVATE_ADDRESSED.
 * */
#define PHPAL_SLI15693_ACTIVATE_DEFAULT     0x0000U
/*@}*/

/**
* \name Timeouts.
*/
/*@{*/
/**
 * Timeout T1 in microseconds as defined in ISO/IEC 15693, 9.1.4.
 * */
#define PHPAL_SLI15693_TIMEOUT_SHORT_US        324U


/**
 * Extended timeout value in microseconds for long timeouts.
 * */
#define PHPAL_SLI15693_TIMEOUT_LONG_US      20000U
/*@}*/

/**
* \name Request Flags 1 to 4 according to ISO 15693.
* \anchor req_flags
*
* In a request, the field "flags" specifies the actions to be performed by
* the VICC. It consists of eight bits.
*/
/*@{*/
/**
 * Sub-carrier_flag.
 *
 * If set two sub-carriers shall be used (VICC to VCD). Else a single
 * sub-carrier frequency shall be used by the VICC.
 * */
#define PHPAL_SLI15693_FLAG_TWO_SUB_CARRIERS    0x01U

/**
 * Data_rate_flag.
 *
 * If set high data rate shall be used (VICC to VCD). Else low data rate shall
 * be used.
 * */
#define PHPAL_SLI15693_FLAG_DATA_RATE           0x02U

/**
 * Inventory_flag.
 *
 * Shall be set for inventory request. If set flags 5 to 8 meaning is as per
 * this \ref inv_set "list". If not set flags 5 to 8 meaning is defined by this
 * \ref inv_not_set "list".
 * */
#define PHPAL_SLI15693_FLAG_INVENTORY           0x04U

/**
 * Protocol_Extension_flag.
 *
 * If set protocol format is extended. Reserved for future use. No supported
 * in current version.
 * */
#define PHPAL_SLI15693_FLAG_PROTOCOL_EXTENSION  0x08U

/**
 * Inventory Read Extension Flag
 *
 * If set the additional features of inventory mentioned along with the command
 * will be performed by the VICC
 *
 * */
#define PHPAL_SLI15693_FLAG_INVENTORY_READ_EXTENSION  0x80U

/**
 * Fast Data Rate Flag
 *
 * This flag is used to set the hardware RX Datarate to Fast_High (~53 kbps)
 * for Fast_Low (~13 kbps)
 *
 * */
#define PHPAL_SLI15693_FLAG_FAST_DATA_RATE                  0x80U


/**
 * Extended Inventory Read Options Flag
 *
 * If set no user memory data are requested from the tag
 * If not set the tag will add the user memory blocks in the response as requested
 * in command
 * */
#define PHPAL_SLI15693_FLAG_SKIP_DATA   0x10U


/**
 * Extended Inventory Read Options Flag
 * If set the complete UID is transmitted by the VICC to the reader irrespective of the mask
 * length
 */
#define PHPAL_SLI15693_FLAG_UID_MODE    0x02U

/**
 * Extended Inventory Read Options Flag
 * If set the 16-bit CID will be transmitted in the command and only tags with the same CID
 * will respond
 */
#define PHPAL_SLI15693_FLAG_CID_COMPARE           0x04U

/**
 * Extended Inventory Read Options Flag
 * If set the Custom ID(CID) is transmitted by the VICC to the reader
 */
#define PHPAL_SLI15693_FLAG_CID_RESPONSE          0x08U

/**
 * Extended Inventory Read Options Flag
 * If set the VICC will go to quiet state after receiving the command
 *
 */
#define PHPAL_SLI15693_FLAG_QUIET   0x20U

/**
 * Extended Inventory Read Options Flag
 * If set the VICC will go to quiet state after receiving the command
 *
 */
#define PHPAL_SLI15693_FLAG_PERSISTENT   0x40U
/*@}*/

/**
* \name Request Flags 5 to 7 when inventory flag is not set.
* \anchor inv_not_set
*
* Flag 8 is RFU and shall be set to zero.
*/
/*@{*/
/**
 * Select_flag.
 *
 * If set only VICC in selected state shall respond. The Address_flag
 * #PHPAL_SLI15693_FLAG_ADDRESSED shall be set to 0 and the UID field will not
 * be included in the request. If not set Request shall be executed by any VICC
 * according to the setting of Address_flag #PHPAL_SLI15693_FLAG_ADDRESSED.
 * */
#define PHPAL_SLI15693_FLAG_SELECTED    0x10U

/**
 * Address_flag.
 *
 * If set request is addressed. UID field is included in request. It shall be
 * executed only by the VICC whose UID matches the UID specified in the request.
 * If not set request is not addressed. UID field is not included in request. It
 * shall be executed by any VICC.
 * */
#define PHPAL_SLI15693_FLAG_ADDRESSED   0x20U

/**
 * Option_flag.
 *
 * Meaning is defined by the command description. It shall be set to 0 if not
 * otherwise defined by the command.
 * */
#define PHPAL_SLI15693_FLAG_OPTION      0x40U
/*@}*/

/**
* \name Request Flags 5 to 7 when inventory flag is set.
* \anchor inv_set
*
* Flag 8 is RFU and shall be set to zero.
*/
/*@{*/
/**
 * Inventory flags mask.
 * */
#define PHPAL_SLI15693_MASK_INVENTORY_FLAGS 0xF0U

/**
 * AFI_flag.
 *
 * If set AFI field is present in request. If not set AFI field is not present
 * in request.
 * */
#define PHPAL_SLI15693_FLAG_AFI             0x10U

/**
 * Nb_slots_flag.
 *
 * If set Use 1 slot instead of 16 slots for inventory. If not set use 16 slots.
 * */
#define PHPAL_SLI15693_FLAG_NBSLOTS         0x20U
/*@}*/

/**
* \name Response Flags 1 and 4 according to ISO15693.
*
* All others flags are RFU and shall not be interpreted.
*/
/*@{*/
/**
 * Error_flag.
 *
 * Error detected. Error code is in the "Error" field of response. When a
 * response is received from the VICC with the error flag set, library returns
 * the custom error code #PHPAL_SLI15693_ERR_ISO15693. The configuration option
 * #PHPAL_SLI15693_CONFIG_ADD_INFO can be used to get the actual \ref
 * error_codes "error code" returned by the VICC.
 * */
#define PHPAL_SLI15693_FLAG_RESP_ERROR          0x01U

/**
 * Extension_flag.
 *
 * Protocol format is extended. Reserved for future use.
 * */
#define PHPAL_SLI15693_FLAG_RESP_EXTERNSION     0x08U
/*@}*/

/**
* \name Response error codes according to ISO15693.
* \anchor error_codes
*
* Codes ranging from A0 - DF are custom command error codes.
*/
/*@{*/
/**
 * The command is not supported, i.e. the request code is not recognized.
 * */
#define PHPAL_SLI15693_FLAG_ERROR_NOT_SUPPORTED 0x01U

/**
 * The command is not recognized, for example: a format error occurred.
 * */
#define PHPAL_SLI15693_FLAG_ERROR_FORMAT        0x02U

/**
 * The command option is not supported.
 * */
#define PHPAL_SLI15693_FLAG_ERROR_OPTION        0x03U

/**
 * Error with no information given or a specific error code is not supported.
 * */
#define PHPAL_SLI15693_FLAG_ERROR_GENERIC       0x0FU

/**
 * The specified block is not available (doesn't exist).
 * */
#define PHPAL_SLI15693_FLAG_ERROR_BLOCK_NA      0x10U

/**
 * The specified block is already locked and thus cannot be locked again.
 * */
#define PHPAL_SLI15693_FLAG_ERROR_BLOCK_LOCKED1 0x11U

/**
 * The specified block is locked and its content cannot be changed.
 * */
#define PHPAL_SLI15693_FLAG_ERROR_BLOCK_LOCKED2 0x12U

/**
 * The specified block was not successfully programmed.
 * */
#define PHPAL_SLI15693_FLAG_ERROR_BLOCK_WRITE1  0x13U

/**
 * The specified block was not successfully locked.
 * */
#define PHPAL_SLI15693_FLAG_ERROR_BLOCK_WRITE2  0x14U

#ifndef NXPBUILD__PH_NDA_NTAG5_I2C
/**
 * \brief Rx baud rates supported by ISO15693 Tags
 */
#endif /* NXPBUILD__PH_NDA_NTAG5_I2C */

#define PHPAL_SLI15693_26KBPS_DATARATE          0x1AU

#define PHPAL_SLI15693_53KBPS_DATARATE          0x1BU

#define PHPAL_SLI15693_106KBPS_DATARATE         0x1CU

#define PHPAL_SLI15693_212KBPS_DATARATE         0x1DU
/*@}*/

/** \name Lengths
*/
/*@{*/
/**
 * UID length in bytes.
 * */
#define PHPAL_SLI15693_UID_LENGTH       0x08U

/**
 * Block length in bytes.
 * */
#define PHPAL_SLI15693_BLOCK_LENGTH     0x04U

/**
 * Number of bytes per page.
 * */
#define PHPAL_SLI15693_PAGE_LENGTH      0x10U
/*@}*/

#ifdef  NXPRDLIB_REM_GEN_INTFS
#include "../comps/phpalSli15693/src/Sw/phpalSli15693_Sw.h"

#define phpalSli15693_Inventory( pDataParams, bFlags, bAfi, pMask, bMaskBitLength, pDsfid, pUid) \
        phpalSli15693_Sw_Inventory((phpalSli15693_Sw_DataParams_t*)pDataParams, bFlags, bAfi, pMask,bMaskBitLength, pDsfid, pUid)

#define phpalSli15693_SendEof( pDataParams, bOption, pDsfid, pUid, pUidLength, pData, pDataLength) \
        phpalSli15693_Sw_SendEof((phpalSli15693_Sw_DataParams_t*)pDataParams, bOption, pDsfid, pUid,pUidLength, pData, pDataLength)

#define phpalSli15693_StayQuiet( pDataParams) \
        phpalSli15693_Sw_StayQuiet((phpalSli15693_Sw_DataParams_t*)pDataParams)

#define phpalSli15693_Select(pDataParams) \
        phpalSli15693_Sw_Select((phpalSli15693_Sw_DataParams_t*)pDataParams )

#define phpalSli15693_ResetToReady( pDataParams) \
        phpalSli15693_Sw_ResetToReady((phpalSli15693_Sw_DataParams_t*)pDataParams )

#define phpalSli15693_Exchange( pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength) \
        phpalSli15693_Sw_Exchange((phpalSli15693_Sw_DataParams_t*)pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength )

#define phpalSli15693_ActivateCard( pDataParams,  bOption, bFlags,  bAfi, pMask,  bMaskBitLength, pDsfid, pUid, pMoreCardsAvailable) \
        phpalSli15693_Sw_ActivateCard((phpalSli15693_Sw_DataParams_t*)pDataParams,  bOption, bFlags, bAfi, pMask, bMaskBitLength, pDsfid, pUid, pMoreCardsAvailable)

#define phpalSli15693_GetSerialNo( pDataParams, pUid,pUidLength) \
        phpalSli15693_Sw_GetSerialNo((phpalSli15693_Sw_DataParams_t*)pDataParams, pUid, pUidLength )

#define phpalSli15693_SetConfig( pDataParams, wConfig, wValue) \
        phpalSli15693_Sw_SetConfig((phpalSli15693_Sw_DataParams_t*)pDataParams, wConfig, wValue )

#define phpalSli15693_GetConfig( pDataParams, wConfig, pValue) \
        phpalSli15693_Sw_GetConfig((phpalSli15693_Sw_DataParams_t*)pDataParams, wConfig, pValue )

#define phpalSli15693_InventoryRead( pDataParams, bFlags, bAfi, pMask, bMaskBitLength, bBlockNo, wNoOfBlocks, pUid, pUidLength, pData, pDataLength) \
        phpalSli15693_Sw_InventoryRead((phpalSli15693_Sw_DataParams_t*)pDataParams, bFlags, bAfi, pMask, bMaskBitLength, bBlockNo, wNoOfBlocks, pUid, pUidLength, pData, pDataLength )

#define phpalSli15693_InventoryReadExtended( pDataParams, bFlags, bAfi, pMask, bMaskBitLength, bExtendedOptions, pCID, bBlockNo, wNoOfBlocks, pCidOut, pUid, pUidLength, pData, pDataLength) \
		phpalSli15693_Sw_InventoryReadExtended((phpalSli15693_Sw_DataParams_t*) pDataParams, bFlags, bAfi, pMask, bMaskBitLength, bExtendedOptions, pCID, bBlockNo, wNoOfBlocks, pCidOut, pUid, pUidLength, pData, pDataLength)

#define phpalSli15693_FastInventoryRead(pDataParams, bFlags, bAfi, pMask, bMaskBitLength, bBlockNo, wNoOfBlocks, pUid, pUidLength, pData, pDataLength ) \
        phpalSli15693_Sw_FastInventoryRead((phpalSli15693_Sw_DataParams_t*)pDataParams, bFlags, bAfi, pMask, bMaskBitLength, bBlockNo, wNoOfBlocks, pUid, pUidLength, pData, pDataLength )

#define phpalSli15693_FastInventoryReadExtended( pDataParams, bFlags, bAfi, bExtendedOptions, pCID, pMask, bMaskBitLength, bBlockNo, wNoOfBlocks, pCidOut, pUid, pUidLength, pData, pDataLength) \
        phpalSli15693_Sw_FastInventoryReadExtended((phpalSli15693_Sw_DataParams_t*) pDataParams, bFlags, bAfi, bExtendedOptions, pCID, pMask, bMaskBitLength, bBlockNo, wNoOfBlocks, pCidOut, pUid, pUidLength, pData, pDataLength)

#define phpalSli15693_InventoryPageRead(pDataParams,bFlags,bAfi,pMask,bMaskBitLength,bPageNo,wNoOfPages,pUid,pUidLength,pData,pDataLength) \
        phpalSli15693_Sw_InventoryPageRead((phpalSli15693_Sw_DataParams_t*)pDataParams, bFlags,bAfi, pMask, bMaskBitLength,bPageNo, wNoOfPages, pUid, pUidLength, pData, pDataLength)

#define phpalSli15693_FastInventoryPageRead(pDataParams, bFlags, bAfi, pMask, bMaskBitLength, bPageNo, wNoOfPages, pUid, pUidLength, pData, pDataLength ) \
        phpalSli15693_Sw_FastInventoryPageRead((phpalSli15693_Sw_DataParams_t*)pDataParams, bFlags, bAfi, pMask, bMaskBitLength, bPageNo, wNoOfPages, pUid, pUidLength, pData, pDataLength )

#define phpalSli15693_StayQuietPersistent( pDataParams) \
        phpalSli15693_Sw_StayQuietPersistent((phpalSli15693_Sw_DataParams_t*)pDataParams)

#define phpalSli15693_SetSerialNo( pDataParams, pUid,bUidLength) \
        phpalSli15693_Sw_SetSerialNo((phpalSli15693_Sw_DataParams_t *) pDataParams, pUid,bUidLength)

#else

/**
* \brief Perform a ISO15693 Inventory command.
*
* When receiving the Inventory request, the VICC shall perform the
* anti-collision sequence. This is the fist command in the activation/detection
* sequence of VICC.
*
* When issuing the inventory command, the reader shall set the Nb_slots_flag
* #PHPAL_SLI15693_FLAG_NBSLOTS to the desired setting to select 1 slot or 16
* slots for the inventory process.
*
* The mask length indicates the number of significant bits of the mask value.
* It can have any value between 0 and 60 when 16 slots are used and any
* value between 0 and 64 when 1 slot is used. The mask value is contained in
* an integer number of bytes. If the mask length is not a multiple of 8 (bits),
* the mask value MSB shall be padded with the required number of null (set to 0)
* bits so that the mask value is contained in an integer number of bytes.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_INVALID_PARAMETER mask length or flag invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_Inventory(
    void * pDataParams,      /**< [In] Pointer to this layers parameter structure. */
    uint8_t bFlags,          /**< [In] \ref req_flags "Request flags" byte. */
    uint8_t bAfi,            /**< [In] Application Family Identifier. */
    uint8_t * pMask,         /**< [In] UID mask, holding known UID bits. */
    uint8_t bMaskBitLength,  /**< [In] Number of UID bits within pMask. */
    uint8_t * pDsfid,        /**< [Out] Data Storage Format Identifier. */
    uint8_t * pUid           /**< [Out] Received UID. */
    );

/**
* \brief Send a ISO15693 EOF (End of Frame).
*
* When 16 slots are used in the inventory process, to switch to the next slot,
* the reader shall send an EOF with option #PHPAL_SLI15693_EOF_NEXT_SLOT.
*
* To send EOF for write alike commands, #PHPAL_SLI15693_EOF_WRITE_ALIKE or
* #PHPAL_SLI15693_EOF_WRITE_ALIKE_WITH_WAIT shall be used. For ICODE SLI custom
* commands like Inventory Read, #PHPAL_SLI15693_EOF_NEXT_SLOT_INV_READ option
* shall be used.
*
* \c bOption can be one of:\n
* \li #PHPAL_SLI15693_EOF_NEXT_SLOT
* \li #PHPAL_SLI15693_EOF_WRITE_ALIKE
* \li #PHPAL_SLI15693_EOF_NEXT_SLOT_INV_READ
* \li #PHPAL_SLI15693_EOF_WRITE_ALIKE_WITH_WAIT
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_INVALID_PARAMETER Invalid option
* \retval #PHPAL_SLI15693_ERR_ISO15693 error response from VICC.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_SendEof(
    void * pDataParams,        /**< [In] Pointer to this layers parameter structure. */
    uint8_t bOption,           /**< [In] Option parameter. */
    uint8_t * pDsfid,          /**< [Out] Data Storage Format Identifier. */
    uint8_t * pUid,            /**< [Out] Received Uid */
    uint8_t * pUidLength,      /**< [Out] Number of received UID bytes. */
    uint8_t * pData,           /**< [Out] Received data. */
    uint16_t * pDataLength     /**< [Out] Number of received data bytes. */
    );

/**
* \brief Perform a ISO15693 StayQuiet command.
*
* When receiving the Stay quiet command, the VICC shall enter the quiet state
* and shall not send back a response. There is no response to the stay quiet
* command.
*
* In quiet state, the VICC shall not process any request where Inventory_flag is
* set and the VICC shall process any addressed request. The VICC shall exit the
* quiet state when reset (power off), receiving a \ref phpalSli15693_Select
* "select" request (It shall then go to the selected state if supported or
* return an error if not supported), receiving a \ref phpalSli15693_ResetToReady
* "Reset to ready request" (It shall then go to the Ready state).
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_USE_CONDITION No UID available.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_StayQuiet(
    void * pDataParams    /**< [In] Pointer to this layers parameter structure. */
    );

/**
* \brief Perform a ISO15693 Select command.
*
* When receiving the Select command, if the UID is equal to its own UID, the
* VICC shall enter the selected state and shall send a response. If the UID is
* different to its own and in selected state, the VICC shall return to the Ready
* state and shall not send a response. If the UID is different to its own and
* not in selected state, the VICC shall remain in its state and shall not send
* a response.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_USE_CONDITION No UID available.
* \retval #PHPAL_SLI15693_ERR_ISO15693 error response from VICC.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_Select(
    void * pDataParams    /**< [In] Pointer to this layers parameter structure. */
    );

/**
* \brief Perform a ISO15693 ResetToReady command.
*
* When receiving a Reset to ready command, the VICC shall return to the Ready
* state.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PHPAL_SLI15693_ERR_ISO15693 error response from VICC.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_ResetToReady(
    void * pDataParams    /**< [In] Pointer to this layers parameter structure. */
    );

/**
* \brief Perform ISO15693 Data Exchange with VICC.
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
* \retval #PHPAL_SLI15693_ERR_ISO15693 error response from VICC.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_USE_CONDITION No UID available.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_Exchange(
    void * pDataParams,       /**< [In] Pointer to this layers parameter structure. */
    uint16_t wOption,         /**< [In] Option parameter. */
    uint8_t * pTxBuffer,      /**< [In] Data to transmit. Flags, IC MFC code and UID will be added automatically. */
    uint16_t wTxLength,       /**< [In] Length of data to transmit. */
    uint8_t ** ppRxBuffer,    /**< [Out] Pointer to received data. */
    uint16_t * pRxLength      /**< [Out] number of received data bytes. */
    );

/**
* \brief Perform ISO15693 ActivateCard command.
*
* This function activates the first VICC detected during the collision
* resolution and optionally move it to selected state based on the wOption
* specified.
*
* \c wOption can be one of:\n
* \li #PHPAL_SLI15693_ACTIVATE_DEFAULT
* \li #PHPAL_SLI15693_ACTIVATE_SELECTED
* \li #PHPAL_SLI15693_ACTIVATE_ADDRESSED
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_INVALID_PARAMETER mask length or flag invalid.
* \retval #PHPAL_SLI15693_ERR_ISO15693 error response from VICC.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_ActivateCard(
    void *   pDataParams,             /**< [In] Pointer to this layers parameter structure. */
    uint8_t  bOption,                 /**< [In] Selected or addressed */
    uint8_t  bFlags,                  /**< [In] Request flags byte. */
    uint8_t  bAfi,                    /**< [In] Application Family Identifier. */
    uint8_t * pMask,                  /**< [In] UID mask, holding known UID bits. */
    uint8_t  bMaskBitLength,          /**< [In] Number of UID bits within pMask. */
    uint8_t * pDsfid,                 /**< [Out] Data Storage Format Identifier. */
    uint8_t * pUid,                   /**< [Out]Received UID of first found card */
    uint8_t * pMoreCardsAvailable     /**< [Out] If there are more cards available this value is different from zero.    */
    );

/**
* \brief Retrieve the serial number (UID).
*
* This API is used to get the UID of current VICC with which this layer is
* communicating.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_USE_CONDITION No UID available.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_GetSerialNo(
    void * pDataParams,    /**< [In] Pointer to this layers parameter structure. */
    uint8_t * pUid,        /**< [Out] Buffer, holding the UID (8 bytes) */
    uint8_t * bUidLength   /**< [Out] Length of received UID bytes. */
    );

/**
* \brief Set serial number (UID).
*
* When multiple VICCs are detected, this API shall be used to set a specific UID
* to this layer from the list of detected UIDs.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER UID length invalid.
*/
phStatus_t phpalSli15693_SetSerialNo(
    void * pDataParams,    /**< [In] Pointer to this layers parameter structure. */
    uint8_t * pUid,        /**< [In] Buffer, holding the UID (8 bytes) */
    uint8_t bUidLength     /**< [In] Length of UID in bytes. */
    );

/**
* \brief Set value of the specified \ref config_palsli
* "configuration identifier".
*
* The SetConfig command is used to set values for the different configuration
* parameters of this layer.
*
* \c wConfig can be one of:\n
* \li #PHPAL_SLI15693_CONFIG_FLAGS
* \li #PHPAL_SLI15693_CONFIG_ADD_INFO
* \li #PHPAL_SLI15693_CONFIG_TIMEOUT_US
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER Invalid wConfig.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_SetConfig(
    void * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
    uint16_t wConfig,    /**< [In] Item to configure. */
    uint16_t wValue      /**< [In] Value to set. */
    );

/**
* \brief Get value of the specified \ref config_palsli
* "configuration identifier".
*
* The GetConfig command is used to  retrieve values of different configuration
* parameters of this layer.
*
* \c wConfig can be one of:\n
* \li #PHPAL_SLI15693_CONFIG_FLAGS
* \li #PHPAL_SLI15693_CONFIG_ADD_INFO
* \li #PHPAL_SLI15693_CONFIG_TIMEOUT_US
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER Invalid wConfig.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_GetConfig(
    void * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
    uint16_t wConfig,    /**< [In] Item to read. */
    uint16_t * pValue    /**< [In] Read value. */
    );

/**
* \brief Perform ICODE SLI InventoryRead command.
*
* When receiving the INVENTORY READ request, the ICODE SLI tag performs the same
* as the anti-collision sequence, with the difference that instead of the UID
* and the DSFID, the requested memory content is re-transmitted from the ICODE
* SLI. If an error is detected, the ICODE SLI tag remains silent.
*
* If the Option_flag #PHPAL_SLI15693_FLAG_OPTION is not set, n blocks of data
* are re-transmitted. If the Option_flag is set, n blocks of data and the part
* of the UID which is not part of the mask are re-transmitted. Additionally the
* slot number in case of 16 slots, are returned. Instead of padding with zeros
* up to the next byte boundary, the corresponding bits of the UID are returned.
*
* The number of bits of the re-transmitted UID can be calculated as follows:
*  - 16 slots: 60 bits (bit 64 to bit 4) - mask length rounded up to the
*    next byte boundary.
*  - 1 slot: 64 bits - mask length rounded up to the next byte boundary.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PHPAL_SLI15693_ERR_ISO15693 error response from VICC.
* \retval #PH_ERR_INVALID_PARAMETER mask length or flag or number of blocks
*         invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_InventoryRead(
    void * pDataParams,      /**< [In] Pointer to this layers parameter structure. */
    uint8_t bFlags,          /**< [In] \ref req_flags "Request flags" byte. */
    uint8_t bAfi,            /**< [In] Application Family Identifier. */
    uint8_t * pMask,         /**< [In] UID mask, holding known UID bits. */
    uint8_t bMaskBitLength,  /**< [In] Number of UID bits within pMask. */
    uint8_t bBlockNo,        /**< [In] Block number of first block to read. */
    uint16_t wNoOfBlocks,    /**< [In] Number of blocks to read */
    uint8_t * pUid,          /**< [Out] Received UID */
    uint8_t * pUidLength,    /**< [Out] Number of received UID bytes. */
    uint8_t * pData,         /**< [Out] Received data. */
    uint16_t * pDataLength   /**< [Out] Number of received data bytes. */
    );

/**
* \brief Perform ICODE SLI InventoryRead command in extended mode.
*
* When receiving the INVENTORY READ EXTENDED request, the ICODE SLI tag performs the same
* as the anti-collision sequence, with the difference that instead of the UID
* and the DSFID, the requested memory content is re-transmitted from the ICODE
* SLI. If an error is detected, the ICODE SLI tag remains silent.
*
* If the Option_flag #PHPAL_SLI15693_FLAG_OPTION is not set, n blocks of data
* are re-transmitted. If the Option_flag is set, n blocks of data and the part
* of the UID which is not part of the mask are re-transmitted. Additionally the
* slot number in case of 16 slots, are returned. Instead of padding with zeros
* up to the next byte boundary, the corresponding bits of the UID are returned.
*
* The number of bits of the re-transmitted UID can be calculated as follows:
*  - 16 slots: 60 bits (bit 64 to bit 4) - mask length rounded up to the
*    next byte boundary.
*  - 1 slot: 64 bits - mask length rounded up to the next byte boundary.
*
*
* Depending on different values of Extended Options the api behaves differently
* b1 - 0 --> Label responds independent from the EAS status
* b1 - 1 --> Only labels will respond which have the EAS enabled
* b2 - 0 --> UID will be transmitted as in regular mode
* b2 - 1 --> Complete UID will be transmitted
* b3 - 0 --> No CID is transmitted in the command
* b3 - 1 --> 16 bits CID will be transmitted in the command and only matching tags should reply
* b4 - 0 --> CID will not be transmitted in the response from the tag
* b4 - 1 --> CID will be transmitted in the response from the tag
* b5 - 0 --> tag will add the user memory block in the response as requested
* b5 - 1 --> no user memory data is requested from the tag
* b6 & b7 - 00 --> Remain in current state
* b6 & b7 - 10 --> Go to quiet state after response
* b6 & b7 - 01 --> Go to persistent quiet state after response
* b6 & b7 - 11 --> Only tags in the PERSISTENT QUIET state will respond to the command
* b8 - 0 --> RFU
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PHPAL_SLI15693_ERR_ISO15693 error response from VICC.
* \retval #PH_ERR_INVALID_PARAMETER mask length or flag or number of blocks
*         invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_InventoryReadExtended(
    void * pDataParams,      /**< [In] Pointer to this layers parameter structure. */
    uint8_t bFlags,          /**< [In] \ref req_flags "Request flags" byte. */
    uint8_t bAfi,            /**< [In] Application Family Identifier. */
    uint8_t * pMask,         /**< [In] UID mask, holding known UID bits. */
    uint8_t bMaskBitLength,  /**< [In] Number of UID bits within pMask. */
    uint8_t bExtendedOptions,   /**< [In] Features of the extended mode */
    uint8_t * pCID,          /**< [In] Two byte CID -> if marked in extended options */
    uint8_t bBlockNo,        /**< [In] Block number of first block to read. */
    uint16_t wNoOfBlocks,    /**< [In] Number of blocks to read */
    uint8_t * pCidOut,       /**< [Out] Received CustomId */
    uint8_t * pUid,          /**< [Out] Received UID */
    uint8_t * pUidLength,    /**< [Out] Number of received UID bytes. */
    uint8_t * pData,         /**< [Out] Received data. */
    uint16_t * pDataLength   /**< [Out] Number of received data bytes. */
    );

/**
* \brief Perform ICODE SLI FastInventoryRead command.
*
* When receiving the FAST INVENTORY READ command the ICODE SLI tag behaves the
* same as the \ref phpalSli15693_InventoryRead "INVENTORY READ" command with the
* exceptions that the data rate in the direction ICODE SLI tag to the reader is
* twice than defined in ISO/IEC 15693-3 depending on the Datarate_flag 53 kbit
* (high data rate) or 13 kbit (low data rate).
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PHPAL_SLI15693_ERR_ISO15693 error response from VICC.
* \retval #PH_ERR_INVALID_PARAMETER mask length or flag or number of blocks
*         invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_FastInventoryRead(
    void * pDataParams,         /**< [In] Pointer to this layers parameter structure. */
    uint8_t bFlags,             /**< [In] \ref req_flags "Request flags" byte. */
    uint8_t bAfi,               /**< [In] Application Family Identifier. */
    uint8_t * pMask,            /**< [In] UID mask, holding known UID bits. */
    uint8_t bMaskBitLength,     /**< [In] Number of UID bits within pMask. */
    uint8_t bBlockNo,           /**< [In] Block number of first block to read. */
    uint16_t wNoOfBlocks,       /**< [In] Number of blocks to read */
    uint8_t * pUid,             /**< [Out] Received UID */
    uint8_t * pUidLength,       /**< [Out]  Number of received UID bytes. */
    uint8_t * pData,            /**< [Out]  Received data. */
    uint16_t * pDataLength      /**< [Out]  Number of received data bytes. */
    );


/**
* \brief Perform ICODE SLI FastInventoryRead command in extended mode.
*
* When receiving the FAST INVENTORY READ command the ICODE SLIX2 \ DNA tag behaves the
* same as the \ref phpalSli15693_InventoryReadExtended "INVENTORY READ EXTENDED" command
* with the exceptions that the data rate in the direction ICODE SLIX2 \ DNA tag to the reader is
* twice than defined in ISO/IEC 15693-3 depending on the Datarate_flag 53 kbit
* (high data rate) or 13 kbit (low data rate).
*
* If the Option_flag #PHPAL_SLI15693_FLAG_OPTION is not set, n blocks of data
* are re-transmitted. If the Option_flag is set, n blocks of data and the part
* of the UID which is not part of the mask are re-transmitted. Additionally the
* slot number in case of 16 slots, are returned. Instead of padding with zeros
* up to the next byte boundary, the corresponding bits of the UID are returned.
*
* The number of bits of the re-transmitted UID can be calculated as follows:
*  - 16 slots: 60 bits (bit 64 to bit 4) - mask length rounded up to the
*    next byte boundary.
*  - 1 slot: 64 bits - mask length rounded up to the next byte boundary.
*
*
* Depending on different values of Extended Options the api behaves differently
* b1 - 0 --> Label responds independent from the EAS status
* b1 - 1 --> Only labels will respond which have the EAS enabled
* b2 - 0 --> UID will be transmitted as in regular mode
* b2 - 1 --> Complete UID will be transmitted
* b3 - 0 --> No CID is transmitted in the command
* b3 - 1 --> 16 bits CID will be transmitted in the command and only matching tags should reply
* b4 - 0 --> CID will not be transmitted in the response from the tag
* b4 - 1 --> CID will be transmitted in the response from the tag
* b5 - 0 --> tag will add the user memory block in the response as requested
* b5 - 1 --> no user memory data is requested from the tag
* b6 & b7 - 00 --> Remain in current state
* b6 & b7 - 10 --> Go to quiet state after response
* b6 & b7 - 01 --> Go to persistent quiet state after response
* b6 & b7 - 11 --> Only tags in the PERSISTENT QUIET state will respond to the command
* b8 - 0 --> RFU
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PHPAL_SLI15693_ERR_ISO15693 error response from VICC.
* \retval #PH_ERR_INVALID_PARAMETER mask length or flag or number of blocks
*         invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_FastInventoryReadExtended(
                                           void * pDataParams,        /**< [In] Pointer to this layers parameter structure. */
                                           uint8_t bFlags,            /**< [In] Request flags byte. */
                                           uint8_t bAfi,              /**< [In] Application Family Identifier. */
                                           uint8_t * pMask,           /**< [In] UID mask, holding known UID bits. */
                                           uint8_t bMaskBitLength,    /**< [In] Number of UID bits within pMask. */
                                           uint8_t bExtendedOptions,  /**< [In] Extended options */
                                           uint8_t * pCID,            /**< [In] Two byte CID -> if marked in extended options */
                                           uint8_t bBlockNo,          /**< [In] Block Number from where start reading */
                                           uint16_t wNumOfBlocks,     /**< [In] Number of blocks to read */
                                           uint8_t * pCIDOut,         /**< [Out] Received CID. */
                                           uint8_t * pUid,            /**< [Out] Received UID. */
                                           uint8_t * pUidLength,      /**< [Out] Received UID Length. */
                                           uint8_t * pData,           /**< [Out] Received Data. */
                                           uint16_t * pDataLength     /**< [Out] Received Data Length. */
                                           );

/**
* \brief Perform ICODE SLI InventoryPageRead command.
*
* When receiving the Inventory Page Read request, the ICODE SLI tag performs the
* same as in the anti-collision sequence, with the difference that instead of
* the UID and the DSFID the requested memory content is re-transmitted from the
* tag.
*
* If an error is detected the ICODE SLI tag remains silent.
*
* If the Option_flag #PHPAL_SLI15693_FLAG_OPTION is not set, n pages of data
* including page protection status (password protection condition) are
* re-transmitted. If the Option_flag is set, n pages (4 blocks = 16 byte) of
* data including page protection status (password protection condition) and the
* part of the UID which is not part of the mask are re-transmitted. Additionally
* the slot number in case of 16 slots, are returned.
*
* The number of bits of the re-transmitted UID can be calculated as follows:
*  - 16 slots: 60 bits (bit 64 to bit 4) - mask length rounded up to the
*    next byte boundary.
*  - 1 slot: 64 bits - mask length rounded up to the next byte boundary.
*
*  If a requested page is protected with the Read password and the valid Read
*  password has not been transmitted before, the page is not read and only
*  protection status (0x0F) is returned.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PHPAL_SLI15693_ERR_ISO15693 error response from VICC.
* \retval #PH_ERR_INVALID_PARAMETER mask length or flag or number of pages
*         invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_InventoryPageRead(
    void * pDataParams,         /**< [In] Pointer to this layers parameter structure. */
    uint8_t bFlags,             /**< [In] \ref req_flags "Request flags" byte. */
    uint8_t bAfi,               /**< [In] Application Family Identifier. */
    uint8_t * pMask,            /**< [In] UID mask, holding known UID bits. */
    uint8_t bMaskBitLength,     /**< [In] Number of UID bits within pMask. */
    uint8_t bPageNo,            /**< [In] Block number of first page to read. */
    uint16_t wNoOfPages,        /**< [In] Number of pages to read */
    uint8_t * pUid,             /**< [Out] Received UID */
    uint8_t * pUidLength,       /**< [Out] Number of received UID bytes. */
    uint8_t * pData,            /**< [Out] Received data. */
    uint16_t * pDataLength      /**< [Out] Number of received data bytes. */
    );

/**
* \brief Perform ICODE SLI FastInventoryPageRead command.
*
* When receiving the Fast Inventory Page Read command the ICODE SLI tag behaves
* the same as in the \ref phpalSli15693_InventoryPageRead "Inventory Page Read"
* command with the exceptions that the data rate in the direction of tag to the
* reader is twice as defined in ISO/IEC 15693-3, depending on the datarate_flag
* 53 kbit (high data rate) or 13 kbit (low data rate).

* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PHPAL_SLI15693_ERR_ISO15693 error response from VICC.
* \retval #PH_ERR_INVALID_PARAMETER mask length or flag or number of pages
*         invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_FastInventoryPageRead(
    void * pDataParams,         /**< [In] Pointer to this layers parameter structure. */
    uint8_t bFlags,             /**< [In] \ref req_flags "Request flags" byte. */
    uint8_t bAfi,               /**< [In] Application Family Identifier. */
    uint8_t * pMask,            /**< [In] UID mask, holding known UID bits. */
    uint8_t bMaskBitLength,     /**< [In] Number of UID bits within pMask. */
    uint8_t bPageNo,            /**< [In] Block number of first page to read. */
    uint16_t wNoOfPages,        /**< [In] Number of pages to read */
    uint8_t * pUid,             /**< [Out] Received UID */
    uint8_t * pUidLength,       /**< [Out] Number of received UID bytes. */
    uint8_t * pData,            /**< [Out] Received data. */
    uint16_t * pDataLength      /**< [Out] Number of received data bytes. */
    );

/**
* \brief Perform ICODE SLI Stay quite persistent command.
*
* When receiving the STAY QUIET PERSISTENT command, the tag enters the
* persistent quiet state and will not send back a response.
*
* The STAY QUIET PERSISTENT command provides the same behavior as the mandatory
* \ref phpalSli15693_StayQuiet "STAY QUIET" command with the only difference at
* a reset (power off), the tag will turn to the ready state, if the power off
* time is exceeding the persistent time. Refer the corresponding ICODE SLI tag
* specification for the value of persistent time.
*
* The tag will exit the persistent quiet state when:
* - reset (power off) exceeding the persistent time.
* - receiving a SELECT request. It shall then go to the Selected state.
* - receiving a RESET TO READY request. It shall then go to the Ready state.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_USE_CONDITION No UID available.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalSli15693_StayQuietPersistent(
    void * pDataParams           /**< [In] Pointer to this layers parameter structure. */
    );


/** @} */

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHPAL_SLI15693 */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHPALSLI15693_H */
