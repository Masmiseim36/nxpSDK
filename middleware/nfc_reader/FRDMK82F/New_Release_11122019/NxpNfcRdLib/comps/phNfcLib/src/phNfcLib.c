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
* NFC Library Top Level API of Reader Library Framework.
* $Author: jenkins_ cm (nxp92197) $
* $Revision: 4184 $ (v06.01.00)
* $Date: 2016-05-20 18:04:59 +0530 (Fri, 20 May 2016) $
*
* History:
*  KK: Generated 20. May 2016
*/

#include <ph_Status.h>

#ifdef NXPBUILD__PHNFCLIB_PROFILES

#include "phNfcLib_Initialization.h"
#include "phNfcLib_Int.h"

static phStatus_t phNfcLib_Config_NFC_Profile(void);
static phStatus_t phNfcLib_Config_EMVCo_Profile(void);
static phStatus_t phNfcLib_Config_ISO_Profile(void);

static phStatus_t phNfcLib_Activate_NFC_Profile(uint16_t wTechnologyMask);
static phStatus_t phNfcLib_Activate_EMVCo_Profile(uint16_t wTechnologyMask);
static phStatus_t phNfcLib_Activate_ISO_Profile(uint16_t wTechnologyMask);

static phStatus_t phNfcLib_Deactivate_NFC_Profile(void);
static phStatus_t phNfcLib_Deactivate_EMVCo_Profile(void);
static phStatus_t phNfcLib_Deactivate_ISO_Profile(void);

static phStatus_t phNfcLib_NFC_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
static phStatus_t phNfcLib_EMVCo_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);
static phStatus_t phNfcLib_ISO_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength);

#ifdef NXPBUILD__PH_NFCLIB_EMVCO
static phStatus_t phNfcLib_EmvcoRfReset(void);
static phStatus_t phNfcLib_Configure_DiscLoop_EMVCo(uint16_t wTechnologyMask);
static phStatus_t phNfcLib_EMVCo_ConfigureRemoval_Cycle(void);
#endif /* NXPBUILD__PH_NFCLIB_EMVCO */
static phStatus_t phNfcLib_EMVCo_Removal_Cycle(void);

#ifdef NXPBUILD__PH_NFCLIB_ISO
static phStatus_t phNfcLib_Configure_DiscLoop_ISO(uint16_t wTechnologyMask);
#endif /* NXPBUILD__PH_NFCLIB_ISO*/

static const pphNfcLib_Config_Profile pConfigProfile[] = {
    &phNfcLib_Config_NFC_Profile,
    &phNfcLib_Config_EMVCo_Profile,
    &phNfcLib_Config_ISO_Profile
};

static const pphNfcLib_Activate_Profile pActivate[] = {
    &phNfcLib_Activate_NFC_Profile,
    &phNfcLib_Activate_EMVCo_Profile,
    &phNfcLib_Activate_ISO_Profile
};

static const pphNfcLib_Deactivate_Profile pDeactivate[] = {
    &phNfcLib_Deactivate_NFC_Profile,
    &phNfcLib_Deactivate_EMVCo_Profile,
    &phNfcLib_Deactivate_ISO_Profile
};

static const pphNfcLib_Transmit pTransmit[] = {
    &phNfcLib_NFC_Transmit,
    &phNfcLib_EMVCo_Transmit,
    &phNfcLib_ISO_Transmit
};

static phStatus_t phNfcLib_Config_NFC_Profile(void)
{
    /* TODO : NFC Profile coding is yet to be done. */
    phStatus_t wStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;

    return wStatus;
}

static phStatus_t phNfcLib_Config_EMVCo_Profile(void)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;
#ifdef NXPBUILD__PH_NFCLIB_EMVCO

    /* Set the LPCD state to Zero in EMVCo mode. */
    gphNfcLib_State.bLPCDState = 0;

    /* Disable Bail-out config in EMVCo mode. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_BAIL_OUT,
        0x00
        ));

    /* Disable LPCD feature. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_ENABLE_LPCD,
        PH_OFF
        ));

    /* reset collision Pending */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_COLLISION_PENDING,
        PH_OFF
        ));

    /* whether anti-collision is supported or not. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_ANTI_COLL,
        PH_ON
        ));

    /* Passive CON_DEVICE limit. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_DEVICE_LIMIT,
        1
        ));

    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_DEVICE_LIMIT,
        1
        ));

    /* Passive polling Tx Guard times in micro seconds. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_GTA_VALUE_US,
        PH_NXPNFCRDLIB_CONFIG_TYPEA_GT
        ));

    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_GTB_VALUE_US,
        PH_NXPNFCRDLIB_CONFIG_TYPEB_GT
        ));

    /* Configure FSDI for the 14443P4A tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_FSDI,
        PH_NXPNFCRDLIB_CONFIG_FSDI_VALUE
        ));

    /* Configure CID for the 14443P4A tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_CID,
        0x00
        ));

    /* Configure DRI for the 14443P4A tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DRI,
        0x00
        ));

    /* Configure DSI for the 14443P4A tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DSI,
        0x00
        ));

    /* Configure AFI for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_AFI_REQ,
        0x00
        ));

    /* Configure FSDI for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_FSDI,
        PH_NXPNFCRDLIB_CONFIG_FSDI_VALUE
        ));

    /* Configure CID for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_CID,
        0x00
        ));

    /* Configure DRI for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_DRI,
        0x00
        ));

    /* Configure DSI for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_DSI,
        0x00
        ));

    /* Configure Extended ATQB support for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_EXTATQB,
        0x00
        ));

    /* Configure reader library mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_OPE_MODE,
        RD_LIB_MODE_EMVCO
        ));

#endif /* NXPBUILD__PH_NFCLIB_EMVCO */
    return wStatus;
}

static phStatus_t phNfcLib_Config_ISO_Profile(void)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;
#ifdef  NXPBUILD__PH_NFCLIB_ISO

    /* TODO: Set the LPCD state to Zero in ISO mode. */
    gphNfcLib_State.bLPCDState = 1;


    /* Enable Bail-out config in ISO mode. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_BAIL_OUT,
        0x13
        ));

    /* Enable LPCD feature. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_ENABLE_LPCD,
        PH_OFF
        ));

    /* reset collision Pending */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_COLLISION_PENDING,
        PH_OFF
        ));

    /* whether anti-collision is supported or not. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_ANTI_COLL,
        PH_ON
        ));

#ifdef NXPBUILD__PHPAL_I14443P3A_SW
    /* Passive CON_DEVICE limit. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_DEVICE_LIMIT,
        1
        ));
#endif /* NXPBUILD__PHPAL_I14443P3A_SW */

#ifdef NXPBUILD__PHPAL_I14443P3B_SW
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_DEVICE_LIMIT,
        1
        ));
#endif /* NXPBUILD__PHPAL_I14443P3B_SW */

#ifdef NXPBUILD__PHPAL_SLI15693_SW
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEV_DEVICE_LIMIT,
        PH_NFCLIB_MAX_ISO15693_TAGS_SUPPORTED
        ));
#endif /* NXPBUILD__PHPAL_SLI15693_SW */

#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_18000P3M3_DEVICE_LIMIT,
        PH_NFCLIB_MAX_ISO18000_TAGS_SUPPORTED
        ));
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */

#ifdef NXPBUILD__PHPAL_I14443P3A_SW
    /* Passive polling Tx Guard times in micro seconds. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_GTA_VALUE_US,
        PH_NXPNFCRDLIB_CONFIG_TYPEA_GT
        ));
#endif /* NXPBUILD__PHPAL_I14443P3A_SW */

#ifdef NXPBUILD__PHPAL_I14443P3B_SW
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_GTB_VALUE_US,
        PH_NXPNFCRDLIB_CONFIG_TYPEB_GT
        ));
#endif /* NXPBUILD__PHPAL_I14443P3B_SW*/

#ifdef NXPBUILD__PHPAL_SLI15693_SW
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_GTV_VALUE_US,
        5200
        ));
#endif /* NXPBUILD__PHPAL_SLI15693_SW*/

#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_GT18000P3M3_VALUE_US,
        10000
        ));
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW*/

#ifdef NXPBUILD__PHPAL_I14443P4A_SW
    /* Configure FSDI for the 14443P4A tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_FSDI,
        0x08
        ));

    /* Configure CID for the 14443P4A tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_CID,
        0x00
        ));

    /* Configure DRI for the 14443P4A tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DRI,
        0x00
        ));

    /* Configure DSI for the 14443P4A tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DSI,
        0x00
        ));
#endif /* NXPBUILD__PHPAL_I14443P4A_SW */

#ifdef NXPBUILD__PHPAL_I14443P3B_SW
    /* Configure AFI for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_AFI_REQ,
        0x00
        ));

    /* Configure FSDI for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_FSDI,
        0x08
        ));

    /* Configure CID for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_CID,
        0x00
        ));

    /* Configure DRI for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_DRI,
        0x00
        ));

    /* Configure DSI for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_DSI,
        0x00
        ));

    /* Configure Extended ATQB support for the type B tags */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEB_EXTATQB,
        0x00
        ));
#endif /* NXPBUILD__PHPAL_I14443P3B_SW*/

#ifdef NXPBUILD__PHPAL_I18092MPI_SW
    /* Set the Device Identifier for type A P2P device in passive mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_P2P_DID,
        0x00
        ));

    /* Set the Initiator Length Reduction field (LRI) for Type A P2P device in passive mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_P2P_LRI,
        0x00
        ));

    /* Enable/Disable Node Addressing (NAD) for Type A P2P device in passive mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_P2P_NAD_ENABLE,
        PH_OFF
        ));

    /* Set the Node Address for Type A P2P device in passive mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_P2P_NAD,
        0x00
        ));

    /* Set the general bytes for Type A P2P device in passive mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEA_P2P_GI_LEN,
        0x00
        ));

    /* Set the Device Identifier (DID) for Type F P2P device in passive mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEF_P2P_DID,
        0x00
        ));

    /* Set the Initiator Length Reduction field (LRI) for Type F P2P device in passive mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEF_P2P_LRI,
        0x00
        ));

    /* Enable/Disable Node Addressing (NAD) for Type F P2P device in passive mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEF_P2P_NAD_ENABLE,
        PH_OFF
        ));

    /* Set the Node Address for Type F P2P device in passive mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEF_P2P_NAD,
        0x00
        ));

    /* Set the general bytes for Type F P2P device in passive mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEF_P2P_GI_LEN,
        0x00
        ));
#endif /* NXPBUILD__PHPAL_I18092MPI_SW */

#ifdef NXPBUILD__PHPAL_SLI15693_SW
    /* Configure ISO 15693 for high/low RX data rate */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_TYPEV_RX_DATA_RATE,
        0x000D
        ));
#endif /* NXPBUILD__PHPAL_SLI15693_SW */

#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    /* Configure ISO 18000p3m3 modulation/sub-carrier */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_I18000P3M3_MODULATION,
        PHPAL_I18000P3M3_M_MANCHESTER_4
        ));

    /* Configure ISO 18000p3m3 link frequency */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_I18000P3M3_FREQUENCY,
        PHPAL_I18000P3M3_LF_847KHZ
        ));
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */


    /* Configure reader library mode */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_OPE_MODE,
        RD_LIB_MODE_ISO
        ));

#endif /* NXPBUILD__PH_NFCLIB_ISO*/
    return wStatus;
}

#ifdef NXPBUILD__PH_NFCLIB_EMVCO
static phStatus_t phNfcLib_EmvcoRfReset(void)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;

    /*RF Field OFF*/
    PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_FieldOff(gphNfcLib_Params.sDiscLoop.pHalDataParams));

    PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_Wait(
        gphNfcLib_Params.sDiscLoop.pHalDataParams,
        PHHAL_HW_TIME_MICROSECONDS,
        PH_NXPNFCRDLIB_CONFIG_EMVCO_FIELD_OFF_DELAY_US));

    /*RF Field ON*/
    return phhalHw_FieldOn(gphNfcLib_Params.sDiscLoop.pHalDataParams);
}
#endif /* NXPBUILD__PH_NFCLIB_EMVCO */

#ifdef NXPBUILD__PH_NFCLIB_EMVCO
static phStatus_t phNfcLib_Configure_DiscLoop_EMVCo(uint16_t wTechnologyMask)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;

    if (wTechnologyMask != (PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_A | PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_B))
    {
        return PH_ERR_INVALID_PARAMETER;
    }

    /* Passive poll bitmap config in EMVCo mode. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG,
        (uint16_t)(((uint16_t)PHAC_DISCLOOP_POS_BIT_MASK_A) | PHAC_DISCLOOP_POS_BIT_MASK_B)
        ));

    /* Disable Listen config in EMVCo mode. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG,
        0x00
        ));
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG,
        0x00
        ));

    /* Disable active poll config. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG,
        0x00
        ));

    /* Poll Mode default state*/
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE,
        (uint16_t)PHAC_DISCLOOP_POLL_STATE_DETECTION
        ));

    return wStatus;
}
#endif /* NXPBUILD__PH_NFCLIB_EMVCO*/

#ifdef NXPBUILD__PH_NFCLIB_ISO
static phStatus_t phNfcLib_Configure_DiscLoop_ISO(uint16_t wTechnologyMask)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;

    if((0U != (wTechnologyMask & PH_NFCLIB_TECHNOLOGY_INITIATOR_FELICA)) || (0U != ((wTechnologyMask & PH_NFCLIB_TECHNOLOGY_TARGET_FELICA))))
    {
        return PH_ERR_INVALID_PARAMETER;
    }

    /*  Passive Listen bitmap config in ISO mode. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG,
        0x00
        ));

    /*  Active Listen bitmap config in ISO mode. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG,
        0x00
        ));

    /* TODO: to be changed if technology mask bit value for active changes */
    /* Active poll bit map config. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG,
        (((wTechnologyMask & 0x0020U) != 0U) ? 0x0007U : 0x0U)
        ));

    /* TODO: to be changed if technology mask bit value for FeliCa changes */
    wTechnologyMask = (wTechnologyMask & 0x0007U) | ((wTechnologyMask << 1U) & 0x0030U) ;

    /* Passive poll bitmap config in ISO mode. */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG,
        (wTechnologyMask & 0x0033U)
        ));

    /* Poll Mode default state*/
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
        &gphNfcLib_Params.sDiscLoop,
        PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE,
        (uint16_t)PHAC_DISCLOOP_POLL_STATE_DETECTION
        ));

    return wStatus;
}
#endif /* NXPBUILD__PH_NFCLIB_ISO*/

static phStatus_t phNfcLib_Activate_NFC_Profile(uint16_t wTechnologyMask)
{
    /* TODO : NFC Profile coding is yet to be done. */
    phStatus_t wStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
    PH_UNUSED_VARIABLE(wTechnologyMask);
    return wStatus;
}

static phStatus_t phNfcLib_Activate_EMVCo_Profile(uint16_t wTechnologyMask)
{
    phStatus_t wtmpStatus = PH_ERR_INTERNAL_ERROR;
#ifdef NXPBUILD__PH_NFCLIB_EMVCO
    phStatus_t wStatus    = PH_ERR_SUCCESS;
    uint16_t   wValue;

    /* Set the LPCD state to Zero in EMVCo mode. */
    gphNfcLib_State.bLPCDState = 0;

    PH_CHECK_SUCCESS_FCT(wStatus, phNfcLib_Configure_DiscLoop_EMVCo(wTechnologyMask));

    /* Assign ATS buffer for Type A */
    gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_I3P4.pAts = gphNfcLib_State.pPeerInfo->uRi.uInitiator.tIso14443_4.aAts;

    do
    {
        /* Start Polling, Function will return once card is activated or any other error has occurred */
        wtmpStatus = phacDiscLoop_Run(&gphNfcLib_Params.sDiscLoop, (uint8_t)PHAC_DISCLOOP_ENTRY_POINT_POLL);
        if((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED)
        {
            break;
        }
        else
        {
            if((wtmpStatus & PH_ERR_MASK) != PHAC_DISCLOOP_NO_TECH_DETECTED)
            {
                /* In case of collision detection, report collision to the terminal. */
                if ((((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_COLLISION_PENDING) ||
                    ((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_TECH_DETECTED) ||
                    ((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED)) &&
                    (gphNfcLib_Params.pNfcLib_ErrCallbck != NULL))
                {
                    gphNfcLib_Params.pNfcLib_ErrCallbck(PH_NFCLIB_STATUS_MULTIPLE_PEERS_DETECTED);
                }
                else if ((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
                {
                    wStatus = phacDiscLoop_GetConfig(&gphNfcLib_Params.sDiscLoop, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
                    if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS)
                    {
                        if ((wValue & PH_ERR_MASK) == PH_ERR_ABORTED)
                        {
                            break;
                        }
                        else
                        {
                            if (gphNfcLib_Params.pNfcLib_ErrCallbck != NULL)
                            {
                                if ((wValue & PH_ERR_MASK) == PH_ERR_PROTOCOL_ERROR)
                                {
                                    gphNfcLib_Params.pNfcLib_ErrCallbck(PH_NFCLIB_STATUS_RF_PROTOCOL_ERROR);
                                }
                                else if ((wValue & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR)
                                {
                                    gphNfcLib_Params.pNfcLib_ErrCallbck(PH_NFCLIB_STATUS_RF_TRANSMISSION_ERROR);
                                }
                                else if ((wValue & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR)
                                {
                                    gphNfcLib_Params.pNfcLib_ErrCallbck(PH_NFCLIB_STATUS_MULTIPLE_PEERS_DETECTED);
                                }
                                else
                                {
                                   /* Do nothing */
                                }
                            }
                        }
                    }
                }
                else
                {
                    /* Do nothing */
                }

                /* Perform RF Reset */
                PH_CHECK_SUCCESS_FCT(wStatus, phNfcLib_EmvcoRfReset());
            }

            if (0U != (gphNfcLib_State.bActivateBlocking))
            {
                PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
                    &gphNfcLib_Params.sDiscLoop,
                    PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE,
                    (uint16_t)PHAC_DISCLOOP_POLL_STATE_DETECTION
                    ));
            }
        }
    } while(0u != gphNfcLib_State.bActivateBlocking);
#endif /* NXPBUILD__PH_NFCLIB_EMVCO*/
    return wtmpStatus;
}

static phStatus_t phNfcLib_Activate_ISO_Profile(uint16_t wTechnologyMask)
{
    phStatus_t wtmpStatus = PH_ERR_INTERNAL_ERROR;
#ifdef NXPBUILD__PH_NFCLIB_ISO
    phStatus_t wStatus    = PH_ERR_SUCCESS;
    uint16_t   wValue;
    uint8_t bEntryPoint;
    uint16_t wTagsDetected = 0;
    uint16_t wGtLength = 0;
#if defined(NXPBUILD__PHPAL_I14443P4A_SW) && defined(NXPBUILD__PHPAL_I14443P4_SW)
    uint8_t bCidEnabled = 0;
    uint8_t bCid = 0;
    uint8_t bNadSupported = 0;
    uint8_t bFwi = 0;
    uint8_t bFsdi = 0;
    uint8_t bFsci = 0;
#endif
#if defined(NXPBUILD__PH_NFCLIB_ISO_15693) || defined (NXPBUILD__PH_NFCLIB_ISO_18000)
    uint8_t bIndex = 0;
#endif
    uint32_t sak_atqa = 0;

    /* Set the LPCD state to one in ISO  mode. */
    gphNfcLib_State.bLPCDState = 1;

    PH_CHECK_SUCCESS_FCT(wStatus, phNfcLib_Configure_DiscLoop_ISO(wTechnologyMask));

    /* For ISO mode the starting mode is poll */
    /* TODO if technology mask says no poll then entry point has to be only listen and vice versa */
    bEntryPoint = (uint8_t)PHAC_DISCLOOP_ENTRY_POINT_POLL;

#ifdef NXPBUILD__PHPAL_I14443P4A_SW
    /* Assign ATS buffer for Type A */
    gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_I3P4.pAts = gphNfcLib_State.pPeerInfo->uRi.uInitiator.tIso14443_4.aAts;
#endif /* NXPBUILD__PHPAL_I14443P4A_SW*/

    do
    {
        /* Start Polling, Function will return once card is activated or any other error has occurred */
        wtmpStatus = phacDiscLoop_Run(&gphNfcLib_Params.sDiscLoop, bEntryPoint);

        /* Reset Activated tag/peer type. */
        gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_UNKNOWN;

        if (((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_NO_TECH_DETECTED) ||
            ((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_NO_DEVICE_RESOLVED) ||
            ((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_EXTERNAL_RFON))
        {
            /* In ISO mode, Target mode is not supported. */
        }
        else if(((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED)
            ||((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED)
            ||((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_MERGED_SEL_RES_FOUND))
        {
            /* Get detected technology type */
            PH_CHECK_SUCCESS_FCT(wStatus,  phacDiscLoop_GetConfig(&gphNfcLib_Params.sDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected));

            if ((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_MERGED_SEL_RES_FOUND)
            {
                if(gphNfcLib_State.bMergedSakPrio == PH_NFCLIB_ACTIVATION_MERGED_SAK_PRIO_14443)
                {
#if defined(NXPBUILD__PHPAL_I14443P4A_SW) && defined(NXPBUILD__PHPAL_I14443P4_SW)
                    PH_CHECK_SUCCESS_FCT(wtmpStatus, phpalI14443p4a_ActivateCard(
                        gphNfcLib_Params.sDiscLoop.pPal1443p4aDataParams,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_I3P4.bFsdi,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_I3P4.bCid,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_I3P4.bDri,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_I3P4.bDsi,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_I3P4.pAts));

                    /* Retrieve 14443-4A protocol parameter */
                    PH_CHECK_SUCCESS_FCT(wtmpStatus, phpalI14443p4a_GetProtocolParams(
                        gphNfcLib_Params.sDiscLoop.pPal1443p4aDataParams,
                        &bCidEnabled,
                        &bCid,
                        &bNadSupported,
                        &bFwi,
                        &bFsdi,
                        &bFsci));

                    /* Set 14443-4 protocol parameter */
                    PH_CHECK_SUCCESS_FCT(wtmpStatus, phpalI14443p4_SetProtocol(
                        gphNfcLib_Params.sDiscLoop.pPal14443p4DataParams,
                        bCidEnabled,
                        bCid,
                        bNadSupported,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_I3P4.bNad,
                        bFwi,
                        bFsdi,
                        bFsci));

                    wtmpStatus = PHAC_DISCLOOP_DEVICE_ACTIVATED;
                    break;
#endif
                }
                else
                {
#ifdef NXPBUILD__PHPAL_I18092MPI_SW
                    /* Send ATR_REQ to activate device in P2P mode. */
                    wtmpStatus = phpalI18092mPI_Atr(gphNfcLib_Params.sDiscLoop.pPal18092mPIDataParams,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_P2P.bDid,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_P2P.bLri,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_P2P.bNadEnable,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_P2P.bNad,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_P2P.pGi,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_P2P.bGiLength,
                        gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_P2P.pAtrRes,
                        &gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.sTypeA_P2P.bAtrResLength
                        );
                    wtmpStatus = PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED;
                    break;
#endif /* NXPBUILD__PHPAL_I18092MPI_SW*/
                }
            }
            else
            {
                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
                {
                    /* Get the ATR_RES Length. */
                    wtmpStatus = phacDiscLoop_GetConfig(&gphNfcLib_Params.sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_P2P_ATR_RES_LEN, &wGtLength);
                }
                else if((PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F212)) ||
                    (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F424)))
                {
                    /* Get the ATR_RES Length. */
                    wtmpStatus = phacDiscLoop_GetConfig(&gphNfcLib_Params.sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEF_P2P_ATR_RES_LEN, &wGtLength);
                }
                else
                {

                }
            }
        }
        else if((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED)
        {
            /* Get detected technology type */
            PH_CHECK_SUCCESS_FCT(wStatus,  phacDiscLoop_GetConfig(&gphNfcLib_Params.sDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected));
#ifdef NXPBUILD__PHPAL_I14443P3A_SW
            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
            {
                gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso14443_3a.bUidLength = gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;
                (void)memcpy(&gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso14443_3a.aUid[0], &gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aUid[0] ,gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);

                sak_atqa = (uint32_t)((((uint32_t)(gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aSak)) << 16U) |
                    (gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aAtqa[0] << 8U) |
                    (gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aAtqa[1]));

                switch (sak_atqa & 0x00FF0FFFU)
                {
                case PH_NFCLIB_SAK_MFC_1K << 16U | PH_NFCLIB_ATQA_MFP_S:
                    gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_MIFARE_CLASSIC_1K;
                    break;
                case PH_NFCLIB_SAK_MFC_4K << 16U | PH_NFCLIB_ATQA_MFC:
                    gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_MIFARE_CLASSIC_4K;
                    break;
                default:
                    break;
                }

                if ((gphNfcLib_State.pPeerInfo->dwActivatedType) == E_PH_NFCLIB_UNKNOWN)
                {
                    switch (sak_atqa)
                    {
                    case PH_NFCLIB_SAK_UL << 16U | PH_NFCLIB_ATQA_UL:
                        gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_MIFARE_ULTRALIGHT;
                        break;
                    case PH_NFCLIB_SAK_LAYER4 << 16U | PH_NFCLIB_ATQA_DESFIRE:
                        gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_MIFARE_DESFIRE;
                        break;
                    case PH_NFCLIB_SAK_LAYER4 << 16U | PH_NFCLIB_ATQA_DESFIRE1:
                        gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_MIFARE_DESFIRE;
                        break;
                    default:
                        break;
                    }
                }
                if (gphNfcLib_State.pPeerInfo->dwActivatedType == E_PH_NFCLIB_UNKNOWN)
                {
                    if(gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aSak == PH_NFCLIB_SAK_LAYER4)
                    {
                        gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_TYPEA_LAYER4;
                    }
                    else
                    {
                        gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_TYPEA_LAYER3;
                    }
                }
                break;

            }
#endif /* NXPBUILD__PHPAL_I14443P3A_SW */
#ifdef NXPBUILD__PHPAL_I14443P3B_SW
            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_B))
            {
                PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_GetConfig(&gphNfcLib_Params.sDiscLoop,
                    PHAC_DISCLOOP_CONFIG_TYPEB_SUPPORT_TYPE4B,
                    &wValue));

                if (wValue == PH_OFF)
                {
                    gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_TYPEB_LAYER3;
                }
                else
                {
                    gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_TYPEB_LAYER4;
                }
                break;
            }
#endif /* NXPBUILD__PHPAL_I14443P3B_SW */
#ifdef NXPBUILD__PH_NFCLIB_ISO_15693
            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_V))
            {
                gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_ISO15693;
                gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso15693.TagIndex[0].pUid = &gphNfcLib_Params.sDiscLoop.sTypeVTargetInfo.aTypeV[0].aUid[0];
                gphNfcLib_State.pActivatedUid = &gphNfcLib_Params.sDiscLoop.sTypeVTargetInfo.aTypeV[0].aUid[0];
                gphNfcLib_State.wActivatedUIDLength = 8;
                break;
            }
#endif /* NXPBUILD__PH_NFCLIB_ISO_15693 */
#ifdef NXPBUILD__PH_NFCLIB_ISO_18000
            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3))
            {
                /* TBD */
                gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso18000_3_3.TagIndex[0].pUii = &gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.aI18000p3m3[0].aUii[0];
                gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso18000_3_3.TagIndex[0].wUiiLength = gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.aI18000p3m3[0].wUiiLength;
                gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_ISO18000p3m3;
                gphNfcLib_State.wActivatedUIDLength = gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.aI18000p3m3[0].wUiiLength;
                gphNfcLib_State.pActivatedUid = &gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.aI18000p3m3[0].aUii[0];
                break;
            }
#endif /* NXPBUILD__PH_NFCLIB_ISO_18000*/

        }

        else if((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED)
        {
            /* Get detected technology type */
            PH_CHECK_SUCCESS_FCT(wStatus,  phacDiscLoop_GetConfig(&gphNfcLib_Params.sDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected));

#ifdef NXPBUILD__PH_NFCLIB_ISO_15693
            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_V))
            {
                gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso15693.bTotalTagsFound = gphNfcLib_Params.sDiscLoop.sTypeVTargetInfo.bTotalTagsFound;
                for(bIndex =0; bIndex < gphNfcLib_Params.sDiscLoop.sTypeVTargetInfo.bTotalTagsFound; bIndex++)
                {
                    gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso15693.TagIndex[bIndex].pUid = &gphNfcLib_Params.sDiscLoop.sTypeVTargetInfo.aTypeV[bIndex].aUid[0];
                }
                gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_ISO15693;
                break;
            }
#endif /* NXPBUILD__PH_NFCLIB_ISO_15693 */

#ifdef NXPBUILD__PH_NFCLIB_ISO_18000
            if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3))
            {
                gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso18000_3_3.bTotalTagsFound = gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.bTotalTagsFound;
                for(bIndex =0; bIndex < gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.bTotalTagsFound; bIndex++)
                {
                    gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso18000_3_3.TagIndex[bIndex].wUiiLength = gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.aI18000p3m3[bIndex].wUiiLength;
                    gphNfcLib_State.pPeerInfo->uTi.uInitiator.tIso18000_3_3.TagIndex[bIndex].pUii = &gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.aI18000p3m3[bIndex].aUii[0];
                }
                gphNfcLib_State.pPeerInfo->dwActivatedType = E_PH_NFCLIB_ISO18000p3m3;
                break;
            }
#endif /* NXPBUILD__PH_NFCLIB_ISO_18000*/
        }
        else
        {
            if((wtmpStatus & PH_ERR_MASK) != PHAC_DISCLOOP_NO_TECH_DETECTED)
            {
                if ((wtmpStatus & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
                {
                    wStatus = phacDiscLoop_GetConfig(&gphNfcLib_Params.sDiscLoop, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
                    if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS)
                    {
                        if ((wValue & PH_ERR_MASK) == PH_ERR_ABORTED)
                        {
                            break;
                        }
                    }
                }
            }

            if (0U != (gphNfcLib_State.bActivateBlocking))
            {
                PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
                    &gphNfcLib_Params.sDiscLoop,
                    (uint16_t)PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE,
                    (uint16_t)PHAC_DISCLOOP_POLL_STATE_DETECTION
                    ));
            }
        }

    } while(0u != gphNfcLib_State.bActivateBlocking);

#endif /* NXPBUILD__PH_NFCLIB_ISO*/
    return wtmpStatus;

}

static phStatus_t phNfcLib_Deactivate_NFC_Profile(void)
{
    /* TODO : NFC Profile coding is yet to be done. */
    phStatus_t wStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;

    return wStatus;
}

#ifdef NXPBUILD__PH_NFCLIB_EMVCO
static phStatus_t phNfcLib_EMVCo_ConfigureRemoval_Cycle(void)
{
    phStatus_t wStatus  = PH_ERR_INTERNAL_ERROR;

    /* Check which technology was activated. */
    if(PHAC_DISCLOOP_CHECK_ANDMASK(gphNfcLib_Params.sDiscLoop.bDetectedTechs, PHAC_DISCLOOP_POS_BIT_MASK_A))
    {
        /* Perform Field ON after Apply protocol settings. */
        PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_ApplyProtocolSettings(
            &gphNfcLib_Params.sHal,
            PHHAL_HW_CARDTYPE_ISO14443A));

        /* Disable RFCA (if supported) */
        PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_SetConfig(&gphNfcLib_Params.sHal, PHHAL_HW_CONFIG_RFCA, PH_OFF));

        /* Switch on RF without performing I-RFCA. */
        PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_FieldOn(&gphNfcLib_Params.sHal));
    }
    else
    {
        /* Perform Field ON after Apply protocol settings. */
        PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_ApplyProtocolSettings(
            &gphNfcLib_Params.sHal,
            PHHAL_HW_CARDTYPE_ISO14443B)
            );

        /* Disable RFCA (if supported) */
        PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_SetConfig(&gphNfcLib_Params.sHal, PHHAL_HW_CONFIG_RFCA, PH_OFF));

        /* Switch on RF without performing I-RFCA. */
        PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_FieldOn(&gphNfcLib_Params.sHal));
    }
    return wStatus;
}
#endif /* NXPBUILD__PH_NFCLIB_EMVCO */

static phStatus_t phNfcLib_EMVCo_Removal_Cycle(void)
{
    phStatus_t wStatus  = PH_ERR_INTERNAL_ERROR;
#ifdef NXPBUILD__PH_NFCLIB_EMVCO
    uint8_t    bPollingCount = 0;

    /* Check which technology was activated. */
    if(PHAC_DISCLOOP_CHECK_ANDMASK(gphNfcLib_Params.sDiscLoop.bDetectedTechs, PHAC_DISCLOOP_POS_BIT_MASK_A))
    {
        while(bPollingCount < PH_NXPNFCRDLIB_CONFIG_EMVCO_REMOVAL_RETRY_COUNT)
        {
            PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_SetConfig(
                &gphNfcLib_Params.sHal,
                PHHAL_HW_CONFIG_POLL_GUARD_TIME_US,
                gphNfcLib_Params.sDiscLoop.waPasPollGTimeUs[PHAC_DISCLOOP_TECH_TYPE_A])
                );

            wStatus = phpalI14443p3a_WakeUpA(
                &gphNfcLib_Params.spalI14443p3a,
                gphNfcLib_Params.sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aAtqa);
            if ((wStatus & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
            {
                /* Return if status is aborted. */
                PH_CHECK_ABORT(wStatus);

                PH_CHECK_ABORT_FCT(wStatus, phpalI14443p3a_HaltA(&gphNfcLib_Params.spalI14443p3a));
                break;
            }
            bPollingCount++;
            wStatus = PH_ERR_SUCCESS;
        }
    }
    else
    {
        while(bPollingCount < PH_NXPNFCRDLIB_CONFIG_EMVCO_REMOVAL_RETRY_COUNT)
        {
            PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_SetConfig(
                &gphNfcLib_Params.sHal,
                PHHAL_HW_CONFIG_POLL_GUARD_TIME_US,
                gphNfcLib_Params.sDiscLoop.waPasPollGTimeUs[PHAC_DISCLOOP_TECH_TYPE_B])
                );

            wStatus = phpalI14443p3b_WakeUpB(
                &gphNfcLib_Params.spalI14443p3b,
                0,
                0,
                0,
                gphNfcLib_Params.sDiscLoop.sTypeBTargetInfo.aTypeB_I3P3[0].aAtqB,
                &gphNfcLib_Params.sDiscLoop.sTypeBTargetInfo.aTypeB_I3P3[0].bAtqBLength);
            if ((wStatus & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
            {
                /* Return if status is aborted. */
                PH_CHECK_ABORT(wStatus);

                break;
            }
            bPollingCount++;
            wStatus = PH_ERR_SUCCESS;
        }
    }

    if (PH_NXPNFCRDLIB_CONFIG_EMVCO_REMOVAL_RETRY_COUNT != bPollingCount)
    {
        gphNfcLib_State.bNfcLibState = eNfcLib_DeactOngoingState;
        wStatus = PH_ERR_INTERNAL_ERROR;
    }
#endif /* NXPBUILD__PH_NFCLIB_EMVCO */
    return wStatus;
}

static phStatus_t phNfcLib_Deactivate_EMVCo_Profile(void)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;
#ifdef NXPBUILD__PH_NFCLIB_EMVCO
    if (gphNfcLib_State.bDeactBlocking == PH_ON)
    {
        /* Set poll state to perform tag removal procedure. */
        PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(
            &gphNfcLib_Params.sDiscLoop,
            PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE,
            (uint16_t)PHAC_DISCLOOP_POLL_STATE_REMOVAL
            ));

        /* Perform EMVCo tag removal procedure. */
        wStatus = phacDiscLoop_Run(&gphNfcLib_Params.sDiscLoop, (uint8_t)PHAC_DISCLOOP_ENTRY_POINT_POLL);
    }
    else
    {
        /* Perform Field Off and wait for time tP. */
        PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_FieldOff(&gphNfcLib_Params.sHal));

        PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_Wait(
            &gphNfcLib_Params.sHal,
            PHHAL_HW_TIME_MICROSECONDS,
            PH_NXPNFCRDLIB_CONFIG_EMVCO_FIELD_OFF_DELAY_US));

        PH_CHECK_SUCCESS_FCT(wStatus, phNfcLib_EMVCo_ConfigureRemoval_Cycle());

        wStatus = phNfcLib_EMVCo_Removal_Cycle();
    }
#endif /* NXPBUILD__PH_NFCLIB_EMVCO */
    return wStatus;
}

static phStatus_t phNfcLib_Deactivate_ISO_Profile(void)
{
    phStatus_t wStatus = PH_NFCLIB_STATUS_INTERNAL_ERROR;
#ifdef NXPBUILD__PH_NFCLIB_ISO

#ifdef NXPBUILD__PH_NFCLIB_ISO_18000
    uint8_t  aBeginRoundCmd[3] = {0};
    uint8_t  aSelectCmd[4] = {0};
    uint8_t  wSelectCmdLen = 0;
    uint8_t  bSelectCmdValidBits = 0;
    uint8_t   bEmptyPointer = 0;
#endif /* NXPBUILD__PH_NFCLIB_ISO_18000 */

    /* build command frame */
    switch(gphNfcLib_State.pPeerInfo->dwActivatedType)
    {
#ifdef NXPBUILD__PH_NFCLIB_ISO_MFC
    case E_PH_NFCLIB_MIFARE_CLASSIC_1K:
    case E_PH_NFCLIB_MIFARE_CLASSIC_4K:
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFC */
#ifdef NXPBUILD__PH_NFCLIB_ISO_MFUL
    case E_PH_NFCLIB_MIFARE_ULTRALIGHT:
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFUL */
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P3_TAGS
    case E_PH_NFCLIB_TYPEA_LAYER3:
        wStatus = phpalI14443p3a_HaltA(&gphNfcLib_Params.spalI14443p3a);
        break;
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P3_TAGS */
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P3B_TAGS
    case E_PH_NFCLIB_TYPEB_LAYER3:
        wStatus = phpalI14443p3b_HaltB(&gphNfcLib_Params.spalI14443p3b);
        break;
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P3B_TAGS*/
#ifdef NXPBUILD__PH_NFCLIB_ISO_MFDF
    case E_PH_NFCLIB_MIFARE_DESFIRE:
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFDF*/
#if (defined NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS || defined NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P4B_TAGS)
    case E_PH_NFCLIB_TYPEA_LAYER4:
    case E_PH_NFCLIB_TYPEB_LAYER4:
        wStatus = phpalI14443p4_Deselect(gphNfcLib_Params.sDiscLoop.pPal14443p4DataParams);
        break;
#endif
#ifdef NXPBUILD__PH_NFCLIB_ISO_15693
    case E_PH_NFCLIB_ISO15693:
        wStatus = phpalSli15693_StayQuiet(gphNfcLib_Params.sDiscLoop.pPalSli15693DataParams);
        break;
#endif /* NXPBUILD__PH_NFCLIB_ISO_15693*/
#ifdef NXPBUILD__PH_NFCLIB_ISO_18000
    case E_PH_NFCLIB_ISO18000p3m3:
        /* Create BeginRound command frame */
        PH_CHECK_SUCCESS_FCT(wStatus, phpalI18000p3m3_CreateBeginRoundCmd(
            gphNfcLib_Params.sDiscLoop.pPal18000p3m3DataParams,
            gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.bDr,
            gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.bM,
            PH_OFF,
            PHPAL_I18000P3M3_SEL_ALL_00,
            PHPAL_I18000P3M3_SESSION_S0,
            0,
            0,
            aBeginRoundCmd));

        /* Create select command frame */
        PH_CHECK_SUCCESS_FCT(wStatus, phpalI18000p3m3_CreateSelectCmd(
            gphNfcLib_Params.sDiscLoop.pPal18000p3m3DataParams,
            PHPAL_I18000P3M3_TARGET_INVENTORIED_S0,
            0,
            PHPAL_I18000P3M3_MEMBANK_UII,
            &bEmptyPointer,
            0,
            NULL,
            0,
            0,
            aSelectCmd,
            &wSelectCmdLen,
            &bSelectCmdValidBits));


        /* Inventory with 1 slot */
        wStatus = phhalHw_I18000p3m3Inventory(
            &gphNfcLib_Params.sHal,
            aSelectCmd,
            (uint8_t)wSelectCmdLen,
            bSelectCmdValidBits,
            aBeginRoundCmd,
            PHHAL_HW_I18000P3M3_ONE_TS_ONLY,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);
        break;
#endif /* NXPBUILD__PH_NFCLIB_ISO_18000*/
    default:
        break;
    }

#endif /* NXPBUILD__PH_NFCLIB_ISO */
    return wStatus;
}

static phStatus_t  phNfcLib_NFC_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength)
{
    /* TODO : NFC Profile coding is yet to be done. */
    phStatus_t wStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
    PH_UNUSED_VARIABLE(pTxBuffer);
    PH_UNUSED_VARIABLE(wTxBufferLength);
    return wStatus;
}

static phStatus_t phNfcLib_EMVCo_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength)
{
    phStatus_t wStatus = PH_NFCLIB_STATUS_INTERNAL_ERROR;
#ifdef NXPBUILD__PH_NFCLIB_EMVCO
    uint16_t wOption = PH_EXCHANGE_DEFAULT;

    if ((pTxBuffer == NULL) && (gphNfcLib_State.bTxState == PH_NFCLIB_INT_TRANSMIT_SUCCESS_CHAINING))
    {
        wOption = PH_EXCHANGE_RXCHAINING;
    }

    /* Send SELECT_PPSE command and start exchange I-PDU */
    wStatus = phpalI14443p4_Exchange(
        &gphNfcLib_Params.spalI14443p4,
        wOption,
        (uint8_t *)pTxBuffer,
        wTxBufferLength,
        &gphNfcLib_State.pRxBuffer,
        &gphNfcLib_State.wRxLength
        );
#endif /* NXPBUILD__PH_NFCLIB_EMVCO*/
    return wStatus;
}

static phStatus_t phNfcLib_ISO_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength)
{
    phStatus_t wStatus = PH_NFCLIB_STATUS_INTERNAL_ERROR;
#ifdef NXPBUILD__PH_NFCLIB_ISO
    /* build command frame */
    switch(gphNfcLib_State.pPeerInfo->dwActivatedType)
    {
#ifdef NXPBUILD__PH_NFCLIB_ISO_MFC
    /* MIFARE Classic contactless IC 1k and MIFARE Classic 4k supports the same commands */
    case E_PH_NFCLIB_MIFARE_CLASSIC_1K:
    case E_PH_NFCLIB_MIFARE_CLASSIC_4K:
        wStatus = phNfcLib_MFC_Transmit(pTxBuffer, wTxBufferLength);
        /* Enable RxCrc */
        (void)phhalHw_SetConfig(gphNfcLib_Params.sDiscLoop.pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON);
        break;
#endif /* #ifdef NXPBUILD__PH_NFCLIB_ISO_MFC*/

#ifdef NXPBUILD__PH_NFCLIB_ISO_MFUL
        /* Identified card is either MIFARE Ultralight contactless IC or MIFARE Ultralight C contactless IC */
    case E_PH_NFCLIB_MIFARE_ULTRALIGHT:
        wStatus = phNfcLib_MFUL_Transmit(pTxBuffer, wTxBufferLength);
        /* Enable RxCrc */
        (void)phhalHw_SetConfig(gphNfcLib_Params.sDiscLoop.pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON);
        break;
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFUL*/

#ifdef NXPBUILD__PH_NFCLIB_ISO_MFDF
        /* Identified card is MIFARE DESFire contactless IC */
    case E_PH_NFCLIB_MIFARE_DESFIRE:
        wStatus = phNfcLib_MFDF_Transmit(pTxBuffer, wTxBufferLength);
        break;
#endif /* NXPBUILD__PH_NFCLIB_ISO_MFDF */

#if (defined NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P3_TAGS || defined NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P3B_TAGS)
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P3_TAGS
    case E_PH_NFCLIB_TYPEA_LAYER3:
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P3_TAGS */
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P3B_TAGS
    case E_PH_NFCLIB_TYPEB_LAYER3:
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P3B_TAGS */
        /* Perform raw 14443-3 exchange. Application should provide the frame supported by the card/peer. */
        wStatus = phhalHw_Exchange(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PH_EXCHANGE_DEFAULT,
            (uint8_t *)pTxBuffer,
            wTxBufferLength,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);
        break;
#endif

#if (defined NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS || defined NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P4B_TAGS)
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS
    case E_PH_NFCLIB_TYPEA_LAYER4:
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS */
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P4B_TAGS
    case E_PH_NFCLIB_TYPEB_LAYER4:
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P4B_TAGS*/
        /* Perform raw 14443-4 exchange. Application should frame 7816-4 frames when using Simplified API. */
        wStatus = phpalI14443p4_Exchange(
            &gphNfcLib_Params.spalI14443p4,
            PH_EXCHANGE_DEFAULT,
            (uint8_t *)pTxBuffer,
            wTxBufferLength,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength
            );
        break;
#endif

#ifdef NXPBUILD__PH_NFCLIB_ISO_15693
        /* Identified cards support ISO 15693 Technology */
    case E_PH_NFCLIB_ISO15693:
        wStatus =  phNfcLib_ISO15693_Transmit(pTxBuffer, wTxBufferLength);
        break;
#endif /* NXPBUILD__PH_NFCLIB_ISO_15693 */

#ifdef NXPBUILD__PH_NFCLIB_ISO_18000
        /* Identified cards support ISO 18000p3m3 Technology */
    case E_PH_NFCLIB_ISO18000p3m3:
        wStatus =  phNfcLib_ISO18000_Transmit(pTxBuffer, wTxBufferLength);
        break;
#endif /* NXPBUILD__PH_NFCLIB_ISO_18000 */

    default:
        break;
    }
#endif /* NXPBUILD__PH_NFCLIB_ISO*/
    return wStatus;
}

phNfcLib_Status_t phNfcLib_SetConfig_Value(
                                           uint8_t bConfigTag,
                                           uint32_t dwValue
                                           )
{
    phStatus_t        wStatus;
    phNfcLib_Status_t dwStatus = PH_NFCLIB_STATUS_INVALID_STATE;

    switch(bConfigTag)
    {
        case PH_NFCLIB_CONFIG_ACTIVATION_BLOCKING:
            if (gphNfcLib_State.bNfcLibState == eNfcLib_InitializedState)
            {
                if ((dwValue == PH_ON) || (dwValue == PH_OFF))
                {
                    gphNfcLib_State.bActivateBlocking = (uint8_t)dwValue;
                    dwStatus = PH_NFCLIB_STATUS_SUCCESS;
                }
                else
                {
                    dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
                }
            }
            break;

        case PH_NFCLIB_CONFIG_ACTIVATION_PROFILE:
            if (gphNfcLib_State.bNfcLibState == eNfcLib_InitializedState)
            {
                if (dwValue <= PH_NFCLIB_ACTIVATION_PROFILE_ISO)
                {
                    PH_CHECK_NFCLIB_SUCCESS_FCT(wStatus, pConfigProfile[dwValue]());
                    gphNfcLib_State.bProfileSelected = (uint8_t)dwValue;
                    dwStatus = PH_NFCLIB_STATUS_SUCCESS;
                }
                else
                {
                    dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
                }
            }
            break;

        case PH_NFCLIB_CONFIG_ACTIVATION_MERGED_SAK_PRIO:
            if (gphNfcLib_State.bNfcLibState == eNfcLib_InitializedState)
            {
                /* The decision to activate needs to be taken in setconfig also or only at the run time */
                if ((dwValue == PH_NFCLIB_ACTIVATION_MERGED_SAK_PRIO_14443) || (dwValue == PH_NFCLIB_ACTIVATION_MERGED_SAK_PRIO_18092))
                {
                if((gphNfcLib_State.bProfileSelected == PH_NFCLIB_ACTIVATION_PROFILE_EMVCO) && (dwValue == PH_NFCLIB_ACTIVATION_MERGED_SAK_PRIO_18092))
                    {
                        dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
                    }
                    else
                    {
                    gphNfcLib_State.bMergedSakPrio = (uint8_t)dwValue;
                        dwStatus = PH_NFCLIB_STATUS_SUCCESS;
                    }
                }
                else
                {
                    dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
                }
            }
            break;

        case PH_NFCLIB_CONFIG_INITIATOR_ENABLE_LPCD:
            if (gphNfcLib_State.bNfcLibState == eNfcLib_InitializedState)
            {
                if ((dwValue == PH_ON) || (dwValue == PH_OFF))
                {
                    if((gphNfcLib_State.bProfileSelected == PH_NFCLIB_ACTIVATION_PROFILE_EMVCO) && (dwValue == PH_ON))
                    {
                        dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
                    }
                    else
                    {
                        gphNfcLib_State.bLPCDState = (uint8_t)dwValue;
                        dwStatus = PH_NFCLIB_STATUS_SUCCESS;
                    }
                }
                else
                {
                    dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
                }
            }
            break;

        case PH_NFCLIB_CONFIG_DEACTIVATION_BLOCKING:
            if (gphNfcLib_State.bNfcLibState != eNfcLib_ResetState)
            {
                if ((dwValue == PH_ON) || (dwValue == PH_OFF))
                {
                    gphNfcLib_State.bDeactBlocking = (uint8_t)dwValue;
                    dwStatus = PH_NFCLIB_STATUS_SUCCESS;
                }
                else
                {
                    dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
                }
            }
            break;

        default:
            dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
            break;
        }

    return dwStatus;
}

phNfcLib_Status_t phNfcLib_SetConfig(
                                     uint8_t bConfigTag,
                                     const uint8_t* pConfigParam,
                                     uint16_t wConfigParamLength
                                     )
{
    phNfcLib_Status_t dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
    PH_UNUSED_VARIABLE(bConfigTag);
    PH_UNUSED_VARIABLE(pConfigParam);
    PH_UNUSED_VARIABLE(wConfigParamLength);
    return dwStatus;
}

phNfcLib_Status_t phNfcLib_GetConfig(
                                     uint8_t bConfigTag,
                                     uint8_t* pConfigParam,
                                     uint16_t * pConfigParamLength
                                     )
{
    phNfcLib_Status_t dwStatus = PH_NFCLIB_STATUS_SUCCESS;

    if (gphNfcLib_State.bNfcLibState != eNfcLib_ResetState)
    {
        if((pConfigParam == NULL) || (pConfigParamLength == NULL))
        {
            return PH_NFCLIB_STATUS_INVALID_PARAMETER;
        }

        switch(bConfigTag)
        {
        case PH_NFCLIB_CONFIG_ACTIVATION_BLOCKING:
            *pConfigParam = gphNfcLib_State.bActivateBlocking;
            *pConfigParamLength = 1;
            break;

        case PH_NFCLIB_CONFIG_ACTIVATION_PROFILE:
            *pConfigParam = gphNfcLib_State.bProfileSelected;
            *pConfigParamLength = 1;
            break;

        case PH_NFCLIB_CONFIG_ACTIVATION_MERGED_SAK_PRIO:
            *pConfigParam = gphNfcLib_State.bMergedSakPrio;
            *pConfigParamLength = 1;
            break;

        case PH_NFCLIB_CONFIG_INITIATOR_ENABLE_LPCD:
            *pConfigParam = gphNfcLib_State.bLPCDState;
            *pConfigParamLength = 1;
            break;

        case PH_NFCLIB_CONFIG_DEACTIVATION_BLOCKING:
            *pConfigParam = gphNfcLib_State.bDeactBlocking;
            *pConfigParamLength = 1;
            break;

        default:
            dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
            break;
        }
    }
    else
    {
        dwStatus = PH_NFCLIB_STATUS_INVALID_STATE;
    }

    return dwStatus;
}

phNfcLib_Status_t phNfcLib_Activate(
                                    uint16_t wTechnologyMask,
                                    phNfcLib_PeerInfo_t * peerInfo,
                                    pphNfcLib_ErrCallbck  ErrCallbck
                                    )
{
    phNfcLib_Status_t dwStatus = PH_NFCLIB_STATUS_INVALID_STATE;
    phStatus_t        wStatus  = PH_ERR_SUCCESS;
    uint16_t          wValue;

    if (gphNfcLib_State.bNfcLibState == eNfcLib_InitializedState)
    {
        if (peerInfo == NULL)
        {
            return PH_NFCLIB_STATUS_INVALID_PARAMETER;
        }

        /* Assign application callback. */
        gphNfcLib_Params.pNfcLib_ErrCallbck = ErrCallbck;
        gphNfcLib_State.pPeerInfo           = peerInfo;
        gphNfcLib_State.bTxState            = PH_NFCLIB_INT_TRANSMIT_OFF;

        wStatus = pActivate[gphNfcLib_State.bProfileSelected](wTechnologyMask);
        if ((wStatus & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED)
        {
            dwStatus = PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE;

            gphNfcLib_State.bNfcLibState = eNfcLib_ActivatedState;
            gphNfcLib_State.wConfiguredRFTech = wTechnologyMask;
        }
        else if (((wStatus & PH_ERR_MASK) == PHAC_DISCLOOP_NO_TECH_DETECTED) ||
            ((wStatus & PH_ERR_MASK) == PHAC_DISCLOOP_NO_DEVICE_RESOLVED))
        {
            dwStatus = PH_NFCLIB_STATUS_NO_PEER_DISCOVERED;
        }
        else if (((wStatus & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_TECH_DETECTED) ||
            ((wStatus & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED) ||
            ((wStatus & PH_ERR_MASK) == PHAC_DISCLOOP_COLLISION_PENDING))
        {
            dwStatus = PH_NFCLIB_STATUS_MULTIPLE_PEERS_DETECTED;

            if (gphNfcLib_State.bProfileSelected == PH_NFCLIB_ACTIVATION_PROFILE_ISO)
            {
                gphNfcLib_State.bNfcLibState = eNfcLib_ActivatedState;
                gphNfcLib_State.wConfiguredRFTech = wTechnologyMask;
            }
        }
        else if ((wStatus & PH_ERR_MASK) == PH_ERR_ABORTED)
        {
            dwStatus = PH_NFCLIB_STATUS_ABORTED;
        }
        else if ((wStatus & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
        {
            wStatus = phacDiscLoop_GetConfig(&gphNfcLib_Params.sDiscLoop, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
            if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                if ((wValue & PH_ERR_MASK) == PH_ERR_ABORTED)
                {
                    dwStatus = PH_NFCLIB_STATUS_ABORTED;
                }
                else if ((wValue & PH_ERR_MASK) == PH_ERR_PROTOCOL_ERROR)
                {
                    dwStatus = PH_NFCLIB_STATUS_RF_PROTOCOL_ERROR;
                }
                else if ((wValue & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR)
                {
                    dwStatus = PH_NFCLIB_STATUS_RF_TRANSMISSION_ERROR;
                }
                else if ((wValue & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR)
                {
                    dwStatus = PH_NFCLIB_STATUS_MULTIPLE_PEERS_DETECTED;
                }
                else if ((wStatus & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
                {
                    dwStatus = PH_NFCLIB_STATUS_NO_PEER_DISCOVERED;
                }
                else
                {
                    dwStatus = PH_NFCLIB_STATUS_PEER_ACTIVATION_FAILED;
                }
            }
        }
        else if (wStatus == PH_NFCLIB_STATUS_INVALID_PARAMETER)
        {
            /* TODO: Remove after NFC and ISO profile is implemented. */
            dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
        }
        else if (wStatus == PH_ERR_INVALID_PARAMETER)
        {
            dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
        }
        else
        {
            dwStatus = PH_NFCLIB_STATUS_PEER_ACTIVATION_FAILED;
        }
    }

    return dwStatus;
}

phNfcLib_Status_t phNfcLib_Transmit(
                                    void * const pTxBuffer,
                                    uint16_t wTxBufferLength
                                    )
{
    phNfcLib_Status_t dwStatus = PH_NFCLIB_STATUS_INVALID_STATE;
    phStatus_t        wStatus  = PH_ERR_SUCCESS;

    gphNfcLib_State.pRxBuffer   = NULL;
    gphNfcLib_State.wRxLength   = 0x00;
    gphNfcLib_State.wRxDataPoin = 0x00;

    if ((pTxBuffer == NULL) && (gphNfcLib_State.bTxState != PH_NFCLIB_INT_TRANSMIT_SUCCESS_CHAINING))
    {
        return PH_NFCLIB_STATUS_INVALID_PARAMETER;
    }

    if (gphNfcLib_State.bNfcLibState == eNfcLib_ActivatedState)
    {
        wStatus = pTransmit[gphNfcLib_State.bProfileSelected](pTxBuffer, wTxBufferLength);
        if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Updated the state of Transmit. */
            gphNfcLib_State.bTxState = PH_NFCLIB_INT_TRANSMIT_ON;

            dwStatus = PH_NFCLIB_STATUS_SUCCESS;
        }
        else if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
            /* Updated the state of Transmit. */
            gphNfcLib_State.bTxState = PH_NFCLIB_INT_TRANSMIT_SUCCESS_CHAINING;

            /* Return Buffer overflow to user so that Receive has to be called to clear NFCLIB
             * Rx buffer and the perform Transmit with pTxBuffer parameter as NULL. */
            dwStatus = PH_NFCLIB_STATUS_BUFFER_OVERFLOW;
        }
        else if ((wStatus & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
        {
            dwStatus = PH_NFCLIB_STATUS_RF_TIMEOUT_ERROR;
            gphNfcLib_State.bTxState    = PH_NFCLIB_INT_TRANSMIT_OFF;
        }
        else if (((wStatus & PH_ERR_MASK) == PH_ERR_PROTOCOL_ERROR) ||
            ((wStatus & PH_ERR_MASK) == PH_ERR_AUTH_ERROR))
        {
            dwStatus = PH_NFCLIB_STATUS_RF_PROTOCOL_ERROR;
            gphNfcLib_State.bTxState    = PH_NFCLIB_INT_TRANSMIT_OFF;
        }
        else if ((wStatus & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR)
        {
            dwStatus = PH_NFCLIB_STATUS_RF_TRANSMISSION_ERROR;
            gphNfcLib_State.bTxState    = PH_NFCLIB_INT_TRANSMIT_OFF;
        }
        else if (wStatus == PH_ERR_INVALID_PARAMETER)
        {
            /* TODO: Remove after NFC profile is implemented. */
            dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
            gphNfcLib_State.bTxState    = PH_NFCLIB_INT_TRANSMIT_OFF;
        }
        else if ((wStatus & PH_ERR_MASK) >= PH_ERR_INTERNAL_ERROR)
        {
            dwStatus = PH_NFCLIB_STATUS_INTERNAL_ERROR;
            gphNfcLib_State.bTxState    = PH_NFCLIB_INT_TRANSMIT_OFF;
        }
        else
        {
            dwStatus = PH_NFCLIB_STATUS_RF_PEER_LOST;
            gphNfcLib_State.bTxState    = PH_NFCLIB_INT_TRANSMIT_OFF;
        }
    }

    return dwStatus;
}

phNfcLib_Status_t phNfcLib_Receive(
                                   uint8_t * const pRxBuffer,
                                   uint16_t *pNumberOfBytes,
                                   uint8_t *pMoreDataAvailable
                                   )
{
    phNfcLib_Status_t dwStatus = PH_NFCLIB_STATUS_INVALID_STATE;

    if ((pRxBuffer == NULL) || (pNumberOfBytes == NULL) || (*pNumberOfBytes == 0U))
    {
        return PH_NFCLIB_STATUS_INVALID_PARAMETER;
    }

    if (gphNfcLib_State.bNfcLibState == eNfcLib_ActivatedState)
    {
        if ((gphNfcLib_State.bProfileSelected == PH_NFCLIB_ACTIVATION_PROFILE_EMVCO) ||
            (gphNfcLib_State.bProfileSelected == PH_NFCLIB_ACTIVATION_PROFILE_ISO))
        {
            if (gphNfcLib_State.bTxState == PH_NFCLIB_INT_TRANSMIT_OFF)
            {
                return dwStatus;
            }

            if ((gphNfcLib_State.pRxBuffer != NULL) && (gphNfcLib_State.wRxLength > 0U))
            {
                if (gphNfcLib_State.wRxLength <= *pNumberOfBytes)
                {
                    if (pMoreDataAvailable != NULL)
                    {
                        *pMoreDataAvailable = 0;
                    }
                    *pNumberOfBytes = gphNfcLib_State.wRxLength;
                    (void)memcpy((uint8_t*)pRxBuffer, &gphNfcLib_State.pRxBuffer[gphNfcLib_State.wRxDataPoin], *pNumberOfBytes);
                    gphNfcLib_State.wRxDataPoin = 0;
                    gphNfcLib_State.wRxLength = 0;

                    /* Do not reset the TxState if previous Tx is chaining as this status is checked in Transmit API. */
                    if (gphNfcLib_State.bTxState != PH_NFCLIB_INT_TRANSMIT_SUCCESS_CHAINING)
                    {
                        gphNfcLib_State.bTxState = PH_NFCLIB_INT_TRANSMIT_OFF;
                    }
                    dwStatus = PH_NFCLIB_STATUS_SUCCESS;
                }
                else
                {
                    if (pMoreDataAvailable != NULL)
                    {
                        *pMoreDataAvailable = 1;
                        (void)memcpy((uint8_t*)pRxBuffer, &gphNfcLib_State.pRxBuffer[gphNfcLib_State.wRxDataPoin], *pNumberOfBytes);
                        gphNfcLib_State.wRxDataPoin = *pNumberOfBytes;
                        gphNfcLib_State.wRxLength -= *pNumberOfBytes;
                        dwStatus = PH_NFCLIB_STATUS_SUCCESS;
                    }
                    else
                    {
                        dwStatus = PH_NFCLIB_STATUS_BUFFER_OVERFLOW;
                    }
                }
            }
            else
            {
                dwStatus = PH_NFCLIB_STATUS_RF_TIMEOUT_ERROR;
                if (gphNfcLib_State.bProfileSelected == PH_NFCLIB_ACTIVATION_PROFILE_ISO)
                {
                    *pNumberOfBytes = gphNfcLib_State.wRxLength;
                    dwStatus = PH_NFCLIB_STATUS_SUCCESS;
                }
            }
        }
        else
        {
            /* TODO: Remove after NFC profile is implemented. */
        }
    }
    return dwStatus;
}

phNfcLib_Status_t phNfcLib_Deactivate(
                                      uint8_t bDeactivateMode,
                                      phNfcLib_PeerInfo_t * peerInfo
                                      )
{
    phStatus_t        wStatus;
    phNfcLib_Status_t dwStatus = PH_NFCLIB_STATUS_INVALID_STATE;
    uint16_t          wValue;

    if (bDeactivateMode == PH_NFCLIB_DEACTIVATION_MODE_RF_OFF)
    {
        if (gphNfcLib_State.bNfcLibState != eNfcLib_ResetState)
        {
            /* wStatus = */ (void)phhalHw_FieldOff(&gphNfcLib_Params.sHal);
            gphNfcLib_State.bNfcLibState = eNfcLib_InitializedState;

            /* wStatus = */ (void)phhalHw_Wait(&gphNfcLib_Params.sHal, PHHAL_HW_TIME_MICROSECONDS, 5100);

            dwStatus = PH_NFCLIB_STATUS_SUCCESS;
        }
    }
    else if (bDeactivateMode == PH_NFCLIB_DEACTIVATION_MODE_RELEASE)
    {
        if (gphNfcLib_State.bNfcLibState == eNfcLib_ActivatedState)
        {
            wStatus = pDeactivate[gphNfcLib_State.bProfileSelected]();
        }
        else if ((gphNfcLib_State.bNfcLibState == eNfcLib_DeactOngoingState) &&
            (gphNfcLib_State.bProfileSelected == PH_NFCLIB_ACTIVATION_PROFILE_EMVCO))
        {
            wStatus = phNfcLib_EMVCo_Removal_Cycle();
        }
        else
        {
            return PH_NFCLIB_STATUS_INVALID_STATE;
        }

        if (wStatus == PH_ERR_SUCCESS)
        {
            dwStatus = PH_NFCLIB_STATUS_SUCCESS;

            gphNfcLib_State.bNfcLibState = eNfcLib_InitializedState;
            gphNfcLib_State.wConfiguredRFTech = 0x00;
            gphNfcLib_Params.pNfcLib_ErrCallbck = NULL;
        }
        else if ((wStatus & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
        {
            wStatus = phacDiscLoop_GetConfig(&gphNfcLib_Params.sDiscLoop, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
            if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                if ((wValue & PH_ERR_MASK) == PH_ERR_ABORTED)
                {
                    dwStatus = PH_NFCLIB_STATUS_ABORTED;
                }
                else if ((wValue & PH_ERR_MASK) == PH_ERR_SUCCESS)
                {
                    dwStatus = PH_NFCLIB_STATUS_SUCCESS;

                    gphNfcLib_State.bNfcLibState = eNfcLib_InitializedState;
                    gphNfcLib_State.wConfiguredRFTech = 0x00;
                    gphNfcLib_Params.pNfcLib_ErrCallbck = NULL;
                }
                else
                {
                    /* Do Nothing */
                }
            }
        }
        else if ((wStatus & PH_ERR_MASK) == PH_ERR_ABORTED)
        {
            dwStatus = PH_NFCLIB_STATUS_ABORTED;
        }
        else
        {
            dwStatus = PH_NFCLIB_STATUS_PEER_DEACTIVATION_FAILED;
        }
#ifdef NXPBUILD__PHPAL_I14443P4_SW
        /* Irrespective of Profile selected, PAL 14443-4 layer needs to be reset. */
        (void)phpalI14443p4_ResetProtocol(&gphNfcLib_Params.spalI14443p4);
#endif /* NXPBUILD__PHPAL_I14443P4_SW*/
    }
    else
    {
        dwStatus = PH_NFCLIB_STATUS_INVALID_PARAMETER;
    }
    PH_UNUSED_VARIABLE(peerInfo);
    return dwStatus;
}

phNfcLib_Status_t phNfcLib_AsyncAbort(void)
{
    phStatus_t        wStatus  = PH_ERR_SUCCESS;
    phNfcLib_Status_t dwStatus = PH_NFCLIB_STATUS_INVALID_STATE;
    if (gphNfcLib_State.bNfcLibState != eNfcLib_ResetState)
    {
        wStatus =  phhalHw_AsyncAbort(&gphNfcLib_Params.sHal);
        if(wStatus == PH_ERR_SUCCESS)
        {
            dwStatus = PH_NFCLIB_STATUS_SUCCESS;
        }
    }
    return dwStatus;
}

#endif /* NXPBUILD__PHNFCLIB_PROFILES */
