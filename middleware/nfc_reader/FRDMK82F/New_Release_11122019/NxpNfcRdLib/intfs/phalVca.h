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
* Generic Virtual Card Architecture (R) Application Component of Reader Library Framework.
* $Author: nxp79920 $
* $Revision: 2141 $ (v06.01.00)
* $Date: 2016-09-19 17:59:46 +0530 (Mon, 19 Sep 2016) $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#ifndef PHALVCA_H
#define PHALVCA_H

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalMifare.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHAL_VCA_SW

/** \defgroup phalVca_Sw Component : Software
* @{
*/

#define PHAL_VCA_SW_ID  0x01U    /**< ID for Software Virtual Card Architecture layer. */

#define PHAL_VCA_ADDITIONAL_INFO    0x0001U    /**< Option for GetConfig/SetConfig to get/set additional info of a generic error. */
#define PHAL_VCA_WRAPPED_MODE       0x0002U    /**< Option for GetConfig/SetConfig to get/set Wrapped mode. */
#define PHAL_VCA_TIMING_MODE        0x0003U    /**< Option for GetConfig/SetConfig to get/set Timing measurements ON/OFF. */
#define PHAL_VCA_PC_LOWER_THRESHOLD 0x0004U    /**< Option for GetConfig/SetConfig to Enable/Diable lower boundary(-20%) on threshold time. */
#define PHAL_VCA_PC_EXTENDED_APDU   0x0005U    /**< Option for GetConfig/SetConfig to get/set current status of extended wrapping in ISO 7816-4 APDUs. */

/**
* \brief Structure for Card table
*/
typedef struct
{
    uint16_t wIidIndex;     /**< Index of the associated IID. */
    uint8_t bValid;         /**< Indicates if an entry is valid or not */
    uint8_t pCardData[16];  /**< Card Data received from the Card. */
} phalVca_Sw_CardTableEntry_t;

/**
* \brief Structure for IID table
*/
typedef struct
{
    uint16_t wIidIndex;         /**< Index of the associated IID. */
    uint16_t wKeyEncNumber;     /**< Key number for Encryption key. */
    uint16_t wKeyEncVersion;    /**< Key version for Encryption key. */
    uint16_t wKeyMacNumber;     /**< Key number for MAC key. */
    uint16_t wKeyMacVersion;    /**< Key version for MAC key. */
} phalVca_Sw_IidTableEntry_t;

/**
* \brief Enum defining VC states
*/
typedef enum
{
    VC_NOT_SELECTED = 0x00,
    VC_PROXIMITYFAILED,
    VC_PROXIMITYCHECK,
    VC_DF_NOT_AUTH,
    VC_DF_AUTH_D40,
    VC_DF_AUTH_ISO,
    VC_DF_AUTH_EV2,
    VC_DF_AUTH_AES,
    VC_MFP_AUTH_AES_SL1,
    VC_MFP_AUTH_AES_SL3
}phalVca_VirtualCardState;

/**
* \brief Enum defining PC states
*/
typedef enum
{
    PC_NO_PCHK_IN_PROGRESS = 0x00,
    PC_PPC_IN_PROGRESS,
    PC_PCHK_PREPARED,
    PC_PCHK_IN_PROGRESS,
    PC_WAITING_PC_VERIFICATION,
    PC_VPC_IN_PROGRESS
}phalVca_ProximityCheckState;

/**
* \brief VCA Software parameter structure
*/
typedef struct
{
    uint16_t wId;                               /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalMifareDataParams;                /**< Pointer to the parameter structure of the palMifare component. */
    void * pKeyStoreDataParams;                 /**< Pointer to the parameter structure of the KeyStore layer. */
    void * pCryptoDataParams;                   /**< Pointer to the parameter structure of the Crypto layer. */
    void * pCryptoRngDataParams;                /**< Pointer to the parameter structure of the CryptoRng layer. */
    phalVca_Sw_CardTableEntry_t * pCardTable;   /**< Pointer to the Card Table of the layer. */
    phalVca_Sw_IidTableEntry_t * pIidTable;     /**< Pointer to the Iid Table of the layer. */
    uint16_t wCurrentCardTablePos;              /**< Position of current entry in the CardTable. */
    uint16_t wNumCardTableEntries;              /**< Number of Card table entries in the table. */
    uint16_t wNumIidTableEntries;               /**< Number of Iid table entries in the table. */
    uint16_t wCurrentIidIndex;                  /**< Current index of the Iid sequence. */
    uint16_t wCurrentIidTablePos;               /**< Position of current entry in the Iid Table. */
    uint16_t wAdditionalInfo;                   /**< Specific error codes for VC generic errors. */
    uint8_t  bSessionAuthMACKey[16];            /**< Session Keys for Authentication of MAC. */
    phalVca_VirtualCardState eVCState;          /**< Enum variable holding Virtual Card state. */
    phalVca_ProximityCheckState ePCState;       /**< Enum Variable holding Proximity Check state. */
    void * pAlDataParams;                       /**< Pointer to the parameter structure of the MIFARE DESFire EV2 contactless IC component. */
    uint8_t bWrappedMode;                       /**< Wrapped APDU mode. All native commands need to be sent wrapped in ISO 7816 APDUs. */
    uint8_t bExtendedLenApdu;                   /**< Extended length APDU. If set the native commands should be wrapped in extended format */
    uint8_t bOption;                            /**< bOption parameter for Timing measurements ON/OFF. */
    uint8_t bLowerBoundThreshold;               /**< parameter for enable/disable the LowerBound threshold time calculation(-20%). */
} phalVca_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalVca_Sw_Init(
                           phalVca_Sw_DataParams_t * pDataParams,           /**< [In] Pointer to this layer's parameter structure. */
                           uint16_t wSizeOfDataParams,                      /**< [In] Specifies the size of the data parameter structure. */
                           void * pPalMifareDataParams,                     /**< [In] Pointer to the parameter structure of the palMifare layer. */
                           void * pKeyStoreDataParams,                      /**< [In] Pointer to the parameter structure of the keyStore layer. */
                           void * pCryptoDataParams,                        /**< [In] Pointer to the parameter structure of the Crypto layer. */
                           void * pCryptoRngDataParams,                     /**< [In] Pointer to the parameter structure of the CryptoRng layer. */
                           phalVca_Sw_IidTableEntry_t * pIidTableStorage,   /**< [In] Pointer to the Iid Table storage for the layer. */
                           uint16_t wNumIidTableStorageEntries,             /**< [In] Number of possible Iid table entries in the storage. */
                           phalVca_Sw_CardTableEntry_t * pCardTableStorage, /**< [In] Pointer to the Card Table storage for the layer. */
                           uint16_t wNumCardTableStorageEntries             /**< [In] Number of possible Card table entries in the storage. */
                           );
/** @} */
#endif /* NXPBUILD__PHAL_VCA_SW */



#ifdef NXPBUILD__PHAL_VCA

/** \defgroup phalVca Virtual Card Architecture (R)
* \brief These Components implement the Virtual Card Architecture (R) commands.
* @{
*/

/** \name Custom Error Codes
*/
/*@{*/
#define PHAL_VCA_ERR_CMD_INVALID    ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0U) /**< VCA Invalid Command Error. */
#define PHAL_VCA_ERR_FORMAT         ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 1U) /**< VCA Format Error. */
#define PHAL_VCA_ERR_AUTH           ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 2U) /**< VCA MAC Error. */
#define PHAL_VCA_ERR_GEN            ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 3U) /**< VCA GEN Error. */
#define PHAL_VCA_ERR_CMD_OVERFLOW   ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 4U) /**< VCA CMD Overflow Error. */
#define PHAL_VCA_ERR_COMMAND_ABORTED ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 5U)/**< VCA Command Abort Error. */
/*@}*/

/** \addtogroup phalVca
* @{
*/
/**
* \name Virtual Card
*/
/*@{*/

/**
* \brief Performs a Virtual Card Support command (MIFARE Plus X contactless IC only).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_VcSupport(
        void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pIid,                 /**< [In] Installation Identifier (16 bytes). */
        uint16_t wKeyEncNumber,         /**< [In] Key number of the ENC key associated to the Iid. */
        uint16_t wKeyEncVersion,        /**< [In] Key version of the ENC key associated to the Iid. */
        uint16_t wKeyMacNumber,         /**< [In] Key number of the MAC key associated to the Iid. */
        uint16_t wKeyMacVersion         /**< [In] Key version of the MAC key associated to the Iid. */
    );

/**
* \brief Perform a Virtual Card Support (Last) command. This cimmand performs the Virtual card support command in
* ISO14443 Layer 3 activated state. This comand is supported by MIFARE Plus EV1 contactless IC product.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_VcSupportLast(
        void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pIid,                 /**< [In] Installation Identifier (16 bytes). */
        uint8_t bLenCap,                /**< [In] Length of the PCDcaps (0-6). */
        uint8_t * pPcdCapabilities,     /**< [In] PCD Capabilities (bLenCap bytes), ignored if bLenCap == 0. */
        uint16_t wKeyEncNumber,         /**< [In] Key number of the ENC key used in VCSL command. */
        uint16_t wKeyEncVersion,        /**< [In] Key version of the ENC key used in VCSL command. */
        uint16_t wKeyMacNumber,         /**< [In] Key number of the MAC key used in VCSL command. */
        uint16_t wKeyMacVersion         /**< [In] Key version of the MAC key used in VCSL command. */
    );

/**
* \brief Start Card Selection.
*
* This command is used to start a VCS/VCSL sequence.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_StartCardSelection(
        void * pDataParams              /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief finalize Card Selection.
*
* This command is used to finalize a VCS/VCSL sequence. The number of sucessfully detected IIDs is returned
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_FinalizeCardSelection(
        void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
        uint16_t * pNumValidIids        /**< [Out] Number of valid IIDs detected during VCS/VCSL sequence execution. */
    );

/**
* \brief Retrieve card information.
*
* This command retrieves card information like UID and Info byte and the associated IID
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_GetIidInfo(
        void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wValidIidIndex,        /**< [In] index of valid IID, needs to be < pNumValidIids in FinalizeCardSelection call. */
        uint16_t * pIidIndex,           /**< [Out] corresponding IID to the key pair where the MAC was matching in VCSL command call. */
        uint8_t * pVcUidSize,           /**< [Out] Size of the VC UID (4 or 7 bytes). */
        uint8_t * pVcUid,               /**< [Out] VC UID (pVcUidSize bytes). */
        uint8_t * pInfo,                /**< [Out] Info byte. */
        uint8_t * pPdCapabilities       /**< [Out] PD Capabilities (2 bytes). */
    );

/**
* \brief Performs a Select Virtual Card command (MIFARE Plus X only).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_SelectVc(
        void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wValidIidIndex,        /**< [In] index of valid IID, needs to be < pNumValidIids in FinalizeCardSelection call. */
        uint16_t wKeyNumber,            /**< [In] Key number of the MAC key used in SVC command. */
        uint16_t wKeyVersion            /**< [In] Key version of the MAC key used in SVC command. */
    );

/**
* \brief PCD explicitly indicates which Virtual Card it wants to target by issuing this command.
*        Both PCD and the PD agree on which VC to use (if any) and they negotiate the capabilities that they will use.
* \return Status code
* \retval #PH_ERR_SUCCESS on Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_IsoSelect(
        void *    pDataParams,          /**< [In] Pointer to this layer's parameter structure which is of type phalVca_Sw_DataParams_t. */
        uint8_t   bSelectionControl,    /**< [In] bSelectionControl equals 0x04 which indicates VC Selection by DF name. Any other value results in PH_ERR_INVALID_PARAMETER.*/
        uint8_t   bOption,              /**< [In] bOption equals 0x00 which indicates FCI template is returned. Any other value results in PH_ERR_INVALID_PARAMETER.*/
        uint8_t   bDFnameLen,           /**< [In] Length of the IID which is nothing but DFName string provided by the user. */
        uint8_t*  pDFname,              /**< [In] This is the IID which is DFName string upto 16 Bytes. Valid only when bSelectionControl = 0x04. */
        uint8_t*  pDivInput,            /**< [IN] Key diversification input. This is used for SAM Key diversification */
        uint8_t   bDivInputLen,         /**< [IN] Key diversification input Length. */
        uint8_t*  pKeys,                /**< [IN] Keys to perform VCSelect operation through SAM */
        uint8_t*  pFCI,                 /**< [Out] File control information of 36 bytes and response code of 2 bytes. */
        uint16_t* pwFCILen,             /**< [Out] Length of response data returned. */
        uint8_t*  pResponse             /**< [Out] Length of response data returned. */
    );

/**
* \brief Depending on the VC configuration, the PCD might be required to subsequently (after IsoSelect execution)
*        authenticate itself via this command before the targeted VC becomes selected.
* \return Status code
* \retval #PH_ERR_SUCCESS on Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_IsoExternalAuthenticate(
        void *    pDataParams,          /**< [In] Pointer to this layer's parameter structure which is of type phalVca_Sw_DataParams_t. */
        uint8_t*  pInData,              /**< [In] Input buffer to be passed for authentication. Response data (RndChl||VCData) of IsoSelect to be passed here. */
        uint16_t  wKeyNumber,           /**< [In] Key number of the VCSelect MAC key. */
        uint16_t  wKeyVersion           /**< [In] Key version of the VCSelect MAC key. */
    );

/**
* \brief Performs a Deselect Virtual Card command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_DeselectVc(
        void * pDataParams              /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief Perform a SetConfig command where a particular configuration item vis. Error code can be set in VC structure.
*
* \c wConfig can be #PHAL_VCA_ADDITIONAL_INFO
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalVca_SetConfig(
        void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wConfig,               /**< [In] Item to set. */
        uint16_t wValue                 /**< [In] Value to set. */
    );

/**
* \brief Perform a GetConfig command where a particular configuration item vis. Error code can be retreived from VC structure.
*
* \c wConfig can be #PHAL_VCA_ADDITIONAL_INFO
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalVca_GetConfig(
        void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wConfig,               /**< [In] Item to be retreived. */
        uint16_t * pValue               /**< [Out] Value of the item to be retreived. */
    );
/** @} */

/**
* \name Proximity Check
*/
/*@{*/

/**
* \brief Performs the whole Proximity Check command chain which calls all the three commands(PreparePC, ExecutePC and VerifyPC) together..
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_ProximityCheck(
        void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
        uint8_t bGenerateRndC,          /**< [In] 0: RndC provided; 1: generate RndC; */
        uint8_t * pRndC,                /**< [In] Provided RndC (7 bytes), ignored if bGenerateRndC == 1. */
        uint8_t bPps1,                  /**< [In] Speed(PPS1). This indicates communication data rate between PD and PCD. */
        uint8_t bNumSteps,              /**< [In] Number of ProximityCheck cycles; RFU, must be 1. */
        uint16_t wKeyNumber,            /**< [In] Proximity Check MAC Key number. */
        uint16_t wKeyVersion,           /**< [In] Proximity Check MAC Key version. */
        uint8_t * pUsedRndC             /**< [Out] Used RndC (7 bytes), can be NULL. */
    );

/**
* \brief Performs the whole Proximity Check New command chain which calls all the three commands(PreparePC, ExecutePC and VerifyPC) together.
*        This command is supported by MIFARE DESFire EV2 and MIFARE Plus EV1 product only.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_ProximityCheckNew(
        void    * pDataParams,          /**< [In] Pointer to this layer's parameter structure. */
        uint8_t   bGenerateRndC,        /**< [In] 0: RndC provided; 1: generate RndC; */
        uint8_t * pPrndC,               /**< [In] Provided RndC (7 bytes), ignored if bGenerateRndC == 1. */
        uint8_t   bNumSteps,            /**< [In] Number of ProximityCheck cycles; RFU, must be 1. */
        uint16_t  wKeyNumber,           /**< [In] Proximity Check MAC Key number. */
        uint16_t  wKeyVersion,          /**< [In] Proximity Check MAC Key version. */
        uint8_t *pDivInput,             /**< [In] Diversification input in case of SAM . */
        uint8_t bDivInputLen,           /**< [In] Diversification input length in case of SAM . */
        uint8_t * pOption,              /**< [Out] Option field defining subsequent response content e.g. bit[0] of bOption denotes presence of pPPS */
        uint8_t * pPubRespTime,         /**< [Out] Published response time: time in microseconds. The PD will transmit the Cmd.ProximityCheck response as close as possible to this time. */
        uint8_t * pPps1,                /**< [Out] Speed(PPS1). This indicates communication data rate between PD and PCD. */
        uint8_t * pCumRndRC             /**< [Out] Combination of both pRndCmd(cumulative of 8 bytes of Random data sent from PCD) and pRndResp(cumulative of 8 bytes response data received from PD). */
    );

/**
* \brief "PrepareProximityCheckNew" command shall prepare the card by generating the Option, publish response time and optional PPS.
*        This command is supported by MIFARE DESFire EV2 and MIFARE Plus EV1 product only.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_PrepareProximityCheckNew(
        void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * bOption,              /**< [Out] Option field defining subsequent response content e.g. bit[0] of bOption denotes presence of pPPS */
        uint8_t * pPubRespTime,         /**< [Out] Published response time: time in microseconds. The PD will transmit the Cmd.ProximityCheck response as close as possible to this time. */
        uint8_t * pPPS                  /**< [Out] Speed(PPS1). This indicates communication data rate between PD and PCD. */
    );

/**
* \brief This new command is implemented to support VCA for MIFARE DESFire EV2 and MIFARE Plus EV1.
* Proximity Device answers with a prepared random number at the published response time
* in Command "PrepareProximityCheckNew". So "ProximityCheckNew" command may be repeated up to 8 times splitting the random number for different time measurements.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_ExecuteProximityCheckNew(
        void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
        uint8_t   bGenerateRndC,        /**< [In] 0: RndC provided; 1: generate RndC; */
        uint8_t * pPrndC,               /**< [In] Provided RndC (8 bytes), ignored if bGenerateRndC == 1. */
        uint8_t   bNumSteps,            /**< [In] Number of ProximityCheck cycles; RFU, must be 1. */
        uint8_t * pPubRespTime,         /**< [In] Published response time: time in microseconds. The PD will transmit the Cmd.ProximityCheck response as close as possible to this time. */
        uint8_t * pCumRndRC             /**< [Out] Combination of both pRndCmd(cumulative of 8 bytes of Random data sent from PCD) and pRndResp(cumulative of 8 bytes response data received from PD). */
    );

/**
* \brief "VerifyProximityCheckNew" command verifies all random numbers received after execution of command "ExecuteProximityCheckNew" using cryptographic methods.
*        This command is supported by MIFARE DESFire EV2 and MIFARE Plus EV1 product only.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_VerifyProximityCheckNew(
        void *    pDataParams,          /**< [In] Pointer to this layer's parameter structure. */
        uint8_t   bOption,              /**< [In] Option field defining subsequent response content e.g. bit[0] of bOption denotes presence of pPPS */
        uint8_t * pPubRespTime,         /**< [In] Published response time: time in microseconds. The PD will transmit the Cmd.ProximityCheck response as close as possible to this time. */
        uint8_t   bPps1,                /**< [In] Speed(PPS1). This indicates communication data rate between PD and PCD. */
        uint16_t  wKeyNumber,           /**< [In] Proximity Check MAC Key number. */
        uint16_t  wKeyVersion,          /**< [In] Proximity Check MAC Key version. */
        uint8_t * pRndCmdResp           /**< [Out] Combination of both pRndCmd(cumulative of 8 bytes of Random data sent from PCD) and pRndResp(cumulative of 8 bytes response data received from PD). */
    );

/**
* \brief It does all the operations as is which the above API "phalVca_VerifyProximityCheckNew" does. It is specially designed to fulfil the requirements for REC.
* It directly sends the MAC and receive it without any further operations on the sent MAC and the received MAC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_VerifyProximityCheckUtility(
        void *    pDataParams,          /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pCmdMac,              /**< [In] 8 bytes Command MAC, to be sent directly to the PICC */
        uint8_t * pCmdResp              /**< [Out] 8 bytes Response MAC received from PICC */
    );

/**
* \brief This is a utility API which passes card type specific session key to VCA
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_SetSessionKeyUtility(
        void *    pDataParams,          /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pSessionKey,          /**< [In] 16 bytes Session Key */
        uint8_t   bAuthMode             /**< [In] Current Authentication Mode (Card type specific) */
    );

/**
* \brief This is a utility API which decrypts the response data(32 bytes) of IsoSelect
* to get the RndChal and VCData(INFO||PDCap1||VCUID||ZeroPadding)
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_DecryptResponse(
        void *    pDataParams,          /**< [In] Pointer to this layer's parameter structure. */
        uint16_t  wKeyNo,               /**< [In] Key number of the VCSelect ENC key. */
        uint16_t  wKeyVersion,          /**< [In] Key version of the VCSelect ENC key. */
        uint8_t * pInData,              /**< [In] Input data to be decrypted. */
        uint8_t * pRandChal,            /**< [Out] Random Challenge. */
        uint8_t * pVCData               /**< [Out] Decrypted VC Data. */
    );

/**
* \brief This is a utility API which sets the application type(MIFARE DESFire EV2, MFP EV1 etc) in the VCA structure
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_SetApplicationType(
        void * pDataParams,
        void * pAlDataParams
        );

/** @} */
#endif /* NXPBUILD__PHAL_VCA */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALVCA_H */
