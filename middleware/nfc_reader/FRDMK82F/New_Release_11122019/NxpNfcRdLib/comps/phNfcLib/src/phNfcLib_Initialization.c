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

#ifdef NXPBUILD__PHNFCLIB

#include "phNfcLib_Initialization.h"


/*******************************************************************************
**   Macro Declaration
*******************************************************************************/
#if defined (NXPBUILD__PH_KEYSTORE_SW) || defined(NXPBUILD__PH_KEYSTORE_RC663)
    #define PH_NFCLIB_KEYSTORE_DATAPARAMS    (&gphNfcLib_Params.sKeyStore)
#else
    #define PH_NFCLIB_KEYSTORE_DATAPARAMS (NULL)
#endif

#ifdef NXPBUILD__PH_KEYSTORE_SW
/**
 * Parameter for Keystore
 * Defines the number of key entries and key version pairs in sw keystore
 */
#define NUMBER_OF_KEYENTRIES        2u
#define NUMBER_OF_KEYVERSIONPAIRS   2u
#define NUMBER_OF_KUCENTRIES        1u
#endif /* NXPBUILD__PH_KEYSTORE_SW */

#define PH_CHECK_NFCLIB_INIT_FCT(status,fct)  {(status) = (fct); PH_BREAK_ON_FAILURE(status);}

/*******************************************************************************
**   Global Variable Declaration
*******************************************************************************/

phNfcLib_DataParams_t    gphNfcLib_Params;
phNfcLib_InternalState_t gphNfcLib_State;

#ifdef NXPBUILD__PH_KEYSTORE_SW
/* Set the key for the MIFARE (R) Classic contactless IC cards. */
static uint8_t gphNfcLib_Key[6] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

/**
 * SW Key Structure Pointers
 */

static phKeyStore_Sw_KeyEntry_t        gpKeyEntries[NUMBER_OF_KEYENTRIES];
static phKeyStore_Sw_KeyVersionPair_t  gpKeyVersionPairs[NUMBER_OF_KEYVERSIONPAIRS * NUMBER_OF_KEYENTRIES];
static phKeyStore_Sw_KUCEntry_t        gpKUCEntries[NUMBER_OF_KUCENTRIES];
#endif /* NXPBUILD__PH_KEYSTORE_SW */

#ifdef NXPBUILD__PHCE_T4T_SW
/**
 * Application buffer. Used in phceT4T_Init. Its needed for data exchange
 * between application thread and reader library thread. Refer phceT4T_Init in
 * phceT4T.h for more info.
 * */
uint8_t aAppHCEBuf[PH_NXPNFCRDLIB_CONFIG_HCE_BUFF_LENGTH];
#endif /* NXPBUILD__PHCE_T4T_SW */


#ifdef NXPBUILD__PHPAL_I14443P4_SW
#   define PTR_spalI14443p4 (&gphNfcLib_Params.spalI14443p4)
#else
#   define PTR_spalI14443p4 NULL
#endif

#ifdef NXPBUILD__PH_KEYSTORE_SW
#   define PTR_sKeyStore (&gphNfcLib_Params.sKeyStore)
#else
#   define PTR_sKeyStore NULL
#endif

#ifdef NXPBUILD__PH_CRYPTOSYM_SW
#   define PTR_sCryptoSym (&gphNfcLib_Params.sCryptoSym)
#else
#   define PTR_sCryptoSym NULL
#endif

#ifdef NXPBUILD__PH_CRYPTORNG_SW
#   define PTR_sCryptoRng (&gphNfcLib_Params.sCryptoRng)
#else
#   define PTR_sCryptoRng NULL
#endif

#ifdef NXPBUILD__PHAL_T1T_SW
#   define PTR_salT1T (&gphNfcLib_Params.salT1T)
#else
#   define PTR_salT1T NULL
#endif

#ifdef NXPBUILD__PHAL_MFUL_SW
#   define PTR_salMFUL (&gphNfcLib_Params.salMFUL)
#else
#   define PTR_salMFUL NULL
#endif

#ifdef NXPBUILD__PHAL_FELICA_SW
#   define PTR_salFelica (&gphNfcLib_Params.salFelica)
#else
#   define PTR_salFelica NULL
#endif

#ifdef NXPBUILD__PHAL_MFDF_SW
#   define PTR_salMFDF (&gphNfcLib_Params.salMFDF)
#else
#   define PTR_salMFDF NULL
#endif

#ifdef NXPBUILD__PHAL_ICODE_SW
#   define PTR_salICode (&gphNfcLib_Params.salICode)
#else
#   define PTR_salICode NULL
#endif

#ifdef NXPBUILD__PHPAL_I14443P3A_SW
#   define PTR_spalI14443p3a (&gphNfcLib_Params.spalI14443p3a)
#else
#   define PTR_spalI14443p3a NULL
#endif

/*******************************************************************************
**   Function Declarations
*******************************************************************************/
/**
* This function will initialize Reader Library PAL Components
*/
static phStatus_t phNfcLib_PAL_Init(void);

/**
* This function will initialize Reader Library AL Components
*/
static phStatus_t phNfcLib_AL_Init(void);


/*******************************************************************************
**   Function Definitions
*******************************************************************************/
phNfcLib_Status_t phNfcLib_SetContext(phNfcLib_AppContext_t * pAppContext)
{
    if (pAppContext == NULL)
    {
        return PH_NFCLIB_STATUS_INVALID_PARAMETER;
    }

    gphNfcLib_Params.pBal = pAppContext->pBalDataparams;
#ifdef NXPBUILD__PHPAL_I14443P4MC_SW
    gphNfcLib_Params.pWtxCallback = (pWtxTimerCallback)pAppContext->pWtxCallback;
#endif /* NXPBUILD__PHPAL_I14443P4MC_SW */

#ifdef NXPBUILD__PHPAL_I18092MT_SW
    gphNfcLib_Params.pRtoxCallback = (pRtoxTimerCallback)pAppContext->pRtoxCallback;
#endif /* NXPBUILD__PHPAL_I18092MT_SW */

    return PH_NFCLIB_STATUS_SUCCESS;
}

/**
* This function will initialize Reader LIbrary PAL Components
*/
static phStatus_t phNfcLib_PAL_Init(void)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;

    do
    {
        /* Initialize the I14443-3A PAL layer */
#ifdef NXPBUILD__PHPAL_I14443P3A_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phpalI14443p3a_Sw_Init(
            &gphNfcLib_Params.spalI14443p3a,
            (uint16_t)(sizeof(phpalI14443p3a_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal));
#endif /* NXPBUILD__PHPAL_I14443P3A_SW */

        /* Initialize the I14443-3B PAL  component */
#ifdef NXPBUILD__PHPAL_I14443P3B_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phpalI14443p3b_Sw_Init(
            &gphNfcLib_Params.spalI14443p3b,
            (uint16_t)(sizeof(phpalI14443p3b_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal));
#endif /* NXPBUILD__PHPAL_I14443P3B_SW */

        /* Initialize the I14443-4A PAL component */
#ifdef NXPBUILD__PHPAL_I14443P4A_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phpalI14443p4a_Sw_Init(
            &gphNfcLib_Params.spalI14443p4a,
            (uint16_t)(sizeof(phpalI14443p4a_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal));
#endif /* NXPBUILD__PHPAL_I14443P4A_SW */

        /* Initialize the I14443-4 PAL component */
#ifdef NXPBUILD__PHPAL_I14443P4_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phpalI14443p4_Sw_Init(
            &gphNfcLib_Params.spalI14443p4,
            (uint16_t)(sizeof(phpalI14443p4_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal));
#endif /* NXPBUILD__PHPAL_I14443P4_SW */

        /* Initialize the MIFARE product PAL component */
#ifdef NXPBUILD__PHPAL_MIFARE_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phpalMifare_Sw_Init(
            &gphNfcLib_Params.spalMifare,
            (uint16_t)(sizeof(phpalMifare_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal,
            PTR_spalI14443p4
        ));
#endif /* NXPBUILD__PHPAL_MIFARE_SW */

        /* Initialize PAL FeliCa PAL component */
#ifdef NXPBUILD__PHPAL_FELICA_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phpalFelica_Sw_Init(
            &gphNfcLib_Params.spalFelica,
            (uint16_t)(sizeof(phpalFelica_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal));
#endif /* NXPBUILD__PHPAL_FELICA_SW */

        /* Initialize the 15693 PAL component */
#ifdef NXPBUILD__PHPAL_SLI15693_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phpalSli15693_Sw_Init(
            &gphNfcLib_Params.spalSli15693,
            (uint16_t)(sizeof(phpalSli15693_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal));
#endif /* NXPBUILD__PHPAL_SLI15693_SW */

        /* Initialize the 1800p3m3 PAL component */
#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phpalI18000p3m3_Sw_Init(
            &gphNfcLib_Params.spalI18000p3m3,
            (uint16_t)(sizeof(phpalI18000p3m3_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal));
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */

        /* Initialize EPC UID component */
#ifdef NXPBUILD__PHPAL_EPCUID_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phpalEpcUid_Sw_Init(
            &gphNfcLib_Params.spalEpcUid,
            (uint16_t)(sizeof(phpalEpcUid_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal));
#endif /* NXPBUILD__PHPAL_EPCUID_SW */

        /* Initialize 18092 Initiator PAL component */
#ifdef NXPBUILD__PHPAL_I18092MPI_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus,phpalI18092mPI_Sw_Init(
            &gphNfcLib_Params.spalI18092mPI,
            (uint16_t)(sizeof(phpalI18092mPI_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal));
#endif /* NXPBUILD__PHPAL_I18092MPI_SW */

        /* Initialize 14443-4mC Target PAL component */
#ifdef NXPBUILD__PHPAL_I14443P4MC_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus,phpalI14443p4mC_Sw_Init(
            &gphNfcLib_Params.spalI14443p4mC,
            (uint16_t)(sizeof(phpalI14443p4mC_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal,
            gphNfcLib_Params.pWtxCallback
            ));
#endif /* NXPBUILD__PHPAL_I14443P4MC_SW */

        /* Initialize 18092 Target PAL component */
#ifdef NXPBUILD__PHPAL_I18092MT_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus,phpalI18092mT_Sw_Init(
            &gphNfcLib_Params.spalI18092mT,
            (uint16_t)(sizeof(phpalI18092mT_Sw_DataParams_t)),
            &gphNfcLib_Params.sHal,
            gphNfcLib_Params.pRtoxCallback
            ));
#endif /* NXPBUILD__PHPAL_I18092MT_SW */

    }while(FALSE);

    return wStatus;
}

/**
* This function will initialize the Reader Library AL Components
*/
static phStatus_t phNfcLib_AL_Init(void)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;

    do
    {

        /* Initialize AL FeliCa component */
#ifdef NXPBUILD__PHAL_FELICA_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phalFelica_Sw_Init(
            &gphNfcLib_Params.salFelica,
            (uint16_t)(sizeof(phalFelica_Sw_DataParams_t)),
            &gphNfcLib_Params.spalFelica));
#endif /* NXPBUILD__PHAL_FELICA_SW */

        /* Initialize AL MIFARE Classic contactless IC component */
#ifdef NXPBUILD__PHAL_MFC_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phalMfc_Sw_Init(
            &gphNfcLib_Params.salMFC,
            (uint16_t)(sizeof(phalMfc_Sw_DataParams_t)),
            &gphNfcLib_Params.spalMifare,
            PTR_sKeyStore
        ));
#endif /* NXPBUILD__PHAL_MFC_SW */

        /* Initialize AL MIFARE Ultralight contactless IC component */
#ifdef NXPBUILD__PHAL_MFUL_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phalMful_Sw_Init(
            &gphNfcLib_Params.salMFUL,
            (uint16_t)(sizeof(phalMful_Sw_DataParams_t)),
            &gphNfcLib_Params.spalMifare,
            PTR_sKeyStore,
            PTR_sCryptoSym,
            PTR_sCryptoRng
        ));
#endif /* NXPBUILD__PHAL_MFUL_SW */

        /* Initialize AL MIFARE DESFire contactless IC component */
#ifdef NXPBUILD__PHAL_MFDF_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phalMfdf_Sw_Init(
            &gphNfcLib_Params.salMFDF,
            (uint16_t)(sizeof(phalMfdf_Sw_DataParams_t)),
            &gphNfcLib_Params.spalMifare,
            PTR_sKeyStore,
            PTR_sCryptoSym,
            PTR_sCryptoRng,
            &gphNfcLib_Params.sHal
        ));
#endif /* NXPBUILD__PHAL_MFDF_SW */

        /* Initialize the T1T AL component */
#ifdef NXPBUILD__PHAL_T1T_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phalT1T_Sw_Init(
            &gphNfcLib_Params.salT1T,
            (uint16_t)(sizeof(phalT1T_Sw_DataParams_t)),
            &gphNfcLib_Params.spalI14443p3a));
#endif /* NXPBUILD__PHAL_T1T_SW */

        /* Initialize the ISO ICODE AL component */
#ifdef NXPBUILD__PHAL_ICODE_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phalICode_Sw_Init(
            &gphNfcLib_Params.salICode,
            (uint16_t)(sizeof(phalICode_Sw_DataParams_t)),
            &gphNfcLib_Params.spalSli15693,
            NULL,
            NULL,
            NULL));
#endif /* NXPBUILD__PHAL_ICODE_SW */

        /* Initialize the Tag operations component */
#ifdef NXPBUILD__PHAL_TOP_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phalTop_Sw_Init(
            &gphNfcLib_Params.salTop,
            (uint16_t)(sizeof(phalTop_Sw_DataParams_t)),
              PTR_salT1T,
              PTR_salMFUL,
              PTR_salFelica,
              PTR_salMFDF,
              PTR_salICode,
              PTR_spalI14443p3a
        ));
#endif /* NXPBUILD__PHAL_TOP_SW */

        /* Initialize the 18000p3m3 AL component */
#ifdef NXPBUILD__PHAL_I18000P3M3_SW
        PH_CHECK_NFCLIB_INIT_FCT(wStatus, phalI18000p3m3_Sw_Init(
            &gphNfcLib_Params.salI18000p3m3,
            (uint16_t)(sizeof(phalI18000p3m3_Sw_DataParams_t)),
            &gphNfcLib_Params.spalI18000p3m3));
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    }while(FALSE);

    return wStatus;
}

phNfcLib_Status_t phNfcLib_Init(void)
{
    phStatus_t        wStatus  = PH_ERR_SUCCESS;
    phNfcLib_Status_t dwStatus = PH_NFCLIB_STATUS_INVALID_STATE;

    if (((phNfcLib_StateMachine_t)gphNfcLib_State.bNfcLibState) == eNfcLib_ResetState)
    {
        do
        {
#ifdef NXPBUILD__PHHAL_HW_RC663
            /* Initialize the RC663 HAL component */
            PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_Rc663_Init(
                &gphNfcLib_Params.sHal,
                (uint16_t)(sizeof(phhalHw_Rc663_DataParams_t)),
                gphNfcLib_Params.pBal,
                (uint8_t *)gkphhalHw_Rc663_LoadConfig,
                gphNfcLib_State.bHalBufferTx,
                PH_NXPNFCRDLIB_CONFIG_HAL_TX_BUFFSIZE,
                gphNfcLib_State.bHalBufferRx,
                PH_NXPNFCRDLIB_CONFIG_HAL_RX_BUFFSIZE
                ));
#endif /* NXPBUILD__PHHAL_HW_RC663 */

#ifdef NXPBUILD__PHHAL_HW_PN5180
            /* Initialize the Pn5180 HAL component */
            PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_Pn5180_Init(
                &gphNfcLib_Params.sHal,
                (uint16_t)(sizeof(phhalHw_Pn5180_DataParams_t)),
                gphNfcLib_Params.pBal,
                PH_NFCLIB_KEYSTORE_DATAPARAMS,
                gphNfcLib_State.bHalBufferTx,
                PH_NXPNFCRDLIB_CONFIG_HAL_TX_BUFFSIZE,
                gphNfcLib_State.bHalBufferRx,
                PH_NXPNFCRDLIB_CONFIG_HAL_RX_BUFFSIZE
                ));
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN5190
            /* Initialize the Pn5190 HAL component */
            PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_Pn5190_Init(
                &gphNfcLib_Params.sHal,
                (uint16_t)(sizeof(phhalHw_Pn5190_DataParams_t)),
                gphNfcLib_Params.pBal,
                PH_NFCLIB_KEYSTORE_DATAPARAMS,
                gphNfcLib_State.bHalBufferTx,
                PH_NXPNFCRDLIB_CONFIG_HAL_TX_BUFFSIZE,
                gphNfcLib_State.bHalBufferRx,
                PH_NXPNFCRDLIB_CONFIG_HAL_RX_BUFFSIZE
                ));
#endif /* NXPBUILD__PHHAL_HW_PN5190 */

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
            /* Initialize the Pn7462AU HAL component */
            PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_PN7462AU_Init(&gphNfcLib_Params.sHal,
                (uint16_t)(sizeof(phhalHw_PN7462AU_DataParams_t)),
                NULL,
                PH_NFCLIB_KEYSTORE_DATAPARAMS,
                gphNfcLib_State.bHalBufferTx,
                PH_NXPNFCRDLIB_CONFIG_HAL_TX_BUFFSIZE,
                gphNfcLib_State.bHalBufferRx,
                PH_NXPNFCRDLIB_CONFIG_HAL_RX_BUFFSIZE
                ));
#endif /* NXPBUILD__PHHAL_HW_PN7462AU*/

            /* Perform Reader Library PAL Initialization */
            PH_CHECK_NFCLIB_INIT_FCT(wStatus, phNfcLib_PAL_Init());

            /* Perform Reader Library AL Initialization */
            PH_CHECK_NFCLIB_INIT_FCT(wStatus, phNfcLib_AL_Init());

            /* Initialize the HCE component */
#ifdef NXPBUILD__PHCE_T4T_SW
            PH_CHECK_NFCLIB_INIT_FCT(wStatus, phceT4T_Sw_Init(
                    &gphNfcLib_Params.sceT4T,
                    (uint16_t)(sizeof(phceT4T_Sw_DataParams_t)),
                    &gphNfcLib_Params.spalI14443p4mC,
                    aAppHCEBuf,
                    PH_NXPNFCRDLIB_CONFIG_HCE_BUFF_LENGTH));
#endif /* NXPBUILD__PHCE_T4T_SW */

#ifdef NXPBUILD__PH_KEYSTORE_SW
            PH_CHECK_NFCLIB_INIT_FCT(wStatus, phKeyStore_Sw_Init(
                &gphNfcLib_Params.sKeyStore,
                (uint16_t)(sizeof(phKeyStore_Sw_DataParams_t)),
                &gpKeyEntries[0],
                NUMBER_OF_KEYENTRIES,
                &gpKeyVersionPairs[0],
                NUMBER_OF_KEYVERSIONPAIRS,
                &gpKUCEntries[0],
                NUMBER_OF_KUCENTRIES
                ));

            /* load a Key to the Store */
            /* Note: If You use Key number 0x00, be aware that in SAM
            this Key is the 'Host authentication key' !!! */
            PH_CHECK_NFCLIB_INIT_FCT(wStatus, phKeyStore_FormatKeyEntry(&gphNfcLib_Params.sKeyStore, 1, 0x6));

            /* Set Key Store */
            PH_CHECK_NFCLIB_INIT_FCT(wStatus,  phKeyStore_SetKey(&gphNfcLib_Params.sKeyStore, 1, 0, 0x6, &gphNfcLib_Key[0], 0));
#endif /* NXPBUILD__PH_KEYSTORE_SW */

            /* Initialize the discover component */
#ifdef NXPBUILD__PHAC_DISCLOOP_SW
            PH_CHECK_NFCLIB_INIT_FCT(wStatus, phacDiscLoop_Sw_Init(
                &gphNfcLib_Params.sDiscLoop,
                (uint16_t)(sizeof(phacDiscLoop_Sw_DataParams_t)),
                &gphNfcLib_Params.sHal));

            /* Assign other layer parameters in discovery loop */
            gphNfcLib_Params.sDiscLoop.pHalDataParams = &gphNfcLib_Params.sHal;

#ifdef NXPBUILD__PHPAL_I14443P3A_SW
            gphNfcLib_Params.sDiscLoop.pPal1443p3aDataParams = &gphNfcLib_Params.spalI14443p3a;
#endif /* NXPBUILD__PHPAL_I14443P3A_SW */

#ifdef NXPBUILD__PHPAL_I14443P3B_SW
            gphNfcLib_Params.sDiscLoop.pPal1443p3bDataParams = &gphNfcLib_Params.spalI14443p3b;
#endif /* NXPBUILD__PHPAL_I14443P3B_SW */

#ifdef NXPBUILD__PHPAL_I14443P4A_SW
            gphNfcLib_Params.sDiscLoop.pPal1443p4aDataParams = &gphNfcLib_Params.spalI14443p4a;
#endif /* NXPBUILD__PHPAL_I14443P4A_SW */

#ifdef NXPBUILD__PHPAL_I14443P4_SW
            gphNfcLib_Params.sDiscLoop.pPal14443p4DataParams = &gphNfcLib_Params.spalI14443p4;
#endif /* NXPBUILD__PHPAL_I14443P4_SW */

#ifdef NXPBUILD__PHPAL_FELICA_SW
            gphNfcLib_Params.sDiscLoop.pPalFelicaDataParams = &gphNfcLib_Params.spalFelica;
#endif /* NXPBUILD__PHPAL_FELICA_SW */

#ifdef NXPBUILD__PHPAL_SLI15693_SW
            gphNfcLib_Params.sDiscLoop.pPalSli15693DataParams = &gphNfcLib_Params.spalSli15693;
#endif /* NXPBUILD__PHPAL_SLI15693_SW */

#ifdef NXPBUILD__PHPAL_I18092MPI_SW
            gphNfcLib_Params.sDiscLoop.pPal18092mPIDataParams = &gphNfcLib_Params.spalI18092mPI;
#endif /* NXPBUILD__PHPAL_I18092MPI_SW */

#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
            gphNfcLib_Params.sDiscLoop.pPal18000p3m3DataParams = &gphNfcLib_Params.spalI18000p3m3;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */

#ifdef NXPBUILD__PHAL_I18000P3M3_SW
            gphNfcLib_Params.sDiscLoop.pAl18000p3m3DataParams = &gphNfcLib_Params.salI18000p3m3;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

#ifdef NXPBUILD__PHAL_T1T_SW
            gphNfcLib_Params.sDiscLoop.pAlT1TDataParams = &gphNfcLib_Params.salT1T;
#endif /* NXPBUILD__PHAL_T1T_SW */
#endif /* NXPBUILD__PHAC_DISCLOOP_SW */

        }while(FALSE);

        if(wStatus != PH_ERR_SUCCESS)
        {
            dwStatus = PH_NFCLIB_STATUS_INTERNAL_ERROR;
        }
        else
        {
            gphNfcLib_State.bNfcLibState      = eNfcLib_InitializedState;
            gphNfcLib_State.bProfileSelected  = PH_NFCLIB_ACTIVATION_PROFILE_NFC;
            gphNfcLib_State.wConfiguredRFTech = PH_NFCLIB_TECHNOLOGY_DEFAULT;
            gphNfcLib_State.bActivateBlocking = PH_NFCLIB_ACTIVATION_BLOCKINGMODE_DEFAULT;
            gphNfcLib_State.bDeactBlocking    = PH_NFCLIB_DEACTIVATION_BLOCKINGMODE_DEFAULT;
            gphNfcLib_State.bLPCDState        = PH_OFF;
            gphNfcLib_State.bTxState          = PH_NFCLIB_INT_TRANSMIT_OFF;
            gphNfcLib_State.bMergedSakPrio    = PH_NFCLIB_ACTIVATION_MERGED_SAK_PRIO_14443;
            gphNfcLib_State.bAuthMode         = PH_NFCLIB_MFDF_NOT_AUTHENTICATED;
            gphNfcLib_Params.pNfcLib_ErrCallbck = NULL;

            dwStatus = PH_NFCLIB_STATUS_SUCCESS;
        }
    }

    return dwStatus;
}

phNfcLib_Status_t phNfcLib_DeInit(void)
{
    phNfcLib_Status_t dwStatus = PH_NFCLIB_STATUS_INVALID_STATE;
    phStatus_t  wStatus;

    if (((phNfcLib_StateMachine_t)gphNfcLib_State.bNfcLibState) == eNfcLib_InitializedState)
    {
        /* Perform HAL De-Init */
        PH_CHECK_NFCLIB_SUCCESS_FCT(wStatus, phhalHw_DeInit(&gphNfcLib_Params.sHal));

        gphNfcLib_State.bNfcLibState      = eNfcLib_ResetState;
        gphNfcLib_State.bProfileSelected  = PH_NFCLIB_ACTIVATION_PROFILE_NFC;
        gphNfcLib_State.wConfiguredRFTech = PH_NFCLIB_TECHNOLOGY_DEFAULT;
        gphNfcLib_State.bActivateBlocking = PH_NFCLIB_ACTIVATION_BLOCKINGMODE_DEFAULT;
        gphNfcLib_State.bDeactBlocking    = PH_NFCLIB_DEACTIVATION_BLOCKINGMODE_DEFAULT;
        gphNfcLib_State.bLPCDState        = PH_OFF;
        gphNfcLib_State.bTxState          = PH_NFCLIB_INT_TRANSMIT_OFF;
        gphNfcLib_State.bMergedSakPrio    = PH_NFCLIB_ACTIVATION_MERGED_SAK_PRIO_14443;
        gphNfcLib_State.bAuthMode = PH_NFCLIB_MFDF_NOT_AUTHENTICATED;
        gphNfcLib_Params.pNfcLib_ErrCallbck = NULL;

        dwStatus = PH_NFCLIB_STATUS_SUCCESS;
    }

    return dwStatus;
}

void* phNfcLib_GetDataParams(
                             uint16_t wComponent
                             )
{
    void * pDataparam = NULL;

    if (((phNfcLib_StateMachine_t)gphNfcLib_State.bNfcLibState) != eNfcLib_ResetState)
    {
        switch(wComponent)
        {
#ifdef NXPBUILD__PHHAL_HW
        case PH_COMP_HAL:
            pDataparam = (void *) &gphNfcLib_Params.sHal;
            break;
#endif /* NXPBUILD__PHHAL_HW */

#ifdef NXPBUILD__PHPAL_I14443P3A_SW
        case PH_COMP_PAL_ISO14443P3A:
            pDataparam = (void *) &gphNfcLib_Params.spalI14443p3a;
            break;
#endif /* NXPBUILD__PHPAL_I14443P3A_SW */

#ifdef NXPBUILD__PHPAL_I14443P3B_SW
        case PH_COMP_PAL_ISO14443P3B:
            pDataparam = (void *) &gphNfcLib_Params.spalI14443p3b;
            break;
#endif /* NXPBUILD__PHPAL_I14443P3B_SW */

#ifdef NXPBUILD__PHPAL_I14443P4A_SW
        case PH_COMP_PAL_ISO14443P4A:
            pDataparam = (void *) &gphNfcLib_Params.spalI14443p4a;
            break;
#endif /* NXPBUILD__PHPAL_I14443P4A_SW */

#ifdef NXPBUILD__PHPAL_I14443P4_SW
        case PH_COMP_PAL_ISO14443P4:
            pDataparam = (void *) &gphNfcLib_Params.spalI14443p4;
            break;
#endif /* NXPBUILD__PHPAL_I14443P4_SW */

#ifdef NXPBUILD__PHPAL_MIFARE_SW
        case PH_COMP_PAL_MIFARE:
            pDataparam = (void *) &gphNfcLib_Params.spalMifare;
            break;
#endif /* NXPBUILD__PHPAL_MIFARE_SW */

#ifdef NXPBUILD__PHPAL_SLI15693_SW
        case PH_COMP_PAL_SLI15693:
            pDataparam = (void *) &gphNfcLib_Params.spalSli15693;
            break;
#endif /* NXPBUILD__PHPAL_SLI15693_SW*/

#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
        case PH_COMP_PAL_I18000P3M3:
            pDataparam = (void *) &gphNfcLib_Params.spalI18000p3m3;
            break;
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW*/

#ifdef NXPBUILD__PHPAL_I18092MPI_SW
        case PH_COMP_PAL_I18092MPI:
            pDataparam = (void *) &gphNfcLib_Params.spalI18092mPI;
            break;
#endif /* NXPBUILD__PHPAL_I18092MPI_SW*/

#ifdef NXPBUILD__PHPAL_FELICA_SW
        case PH_COMP_PAL_FELICA:
            pDataparam = (void *) &gphNfcLib_Params.spalFelica;
            break;
#endif /* NXPBUILD__PHPAL_FELICA_SW */

#ifdef NXPBUILD__PHPAL_I18092MT_SW
        case PH_COMP_PAL_I18092MT:
            pDataparam = (void *) &gphNfcLib_Params.spalI18092mT;
            break;
#endif /* NXPBUILD__PHPAL_I18092MT_SW */

#ifdef NXPBUILD__PHPAL_I14443P4MC_SW
        case PH_COMP_PAL_I14443P4MC:
            pDataparam = (void *) &gphNfcLib_Params.spalI14443p4mC;
            break;
#endif /* NXPBUILD__PHPAL_I14443P4MC_SW */

#ifdef NXPBUILD__PHPAL_EPCUID_SW
        case PH_COMP_PAL_EPCUID:
            pDataparam = (void *) &gphNfcLib_Params.spalEpcUid;
            break;
#endif /* NXPBUILD__PHPAL_EPCUID_SW */

#ifdef NXPBUILD__PHAL_MFC_SW
        case PH_COMP_AL_MFC:
            pDataparam = (void *) &gphNfcLib_Params.salMFC;
            break;
#endif /* NXPBUILD__PHAL_MFC_SW */

#ifdef NXPBUILD__PHAL_MFDF_SW
        case PH_COMP_AL_MFDF:
            pDataparam = (void *) &gphNfcLib_Params.salMFDF;
            break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFUL_SW
        case PH_COMP_AL_MFUL:
            pDataparam = (void *) &gphNfcLib_Params.salMFUL;
            break;
#endif /* NXPBUILD__PHAL_MFUL_SW */

#ifdef NXPBUILD__PHAL_FELICA_SW
        case PH_COMP_AL_FELICA:
            pDataparam = (void *) &gphNfcLib_Params.salFelica;
            break;
#endif /* NXPBUILD__PHAL_FELICA_SW */

#ifdef NXPBUILD__PHAL_ICODE_SW
        case PH_COMP_AL_ICODE:
            pDataparam = (void *) &gphNfcLib_Params.salICode;
            break;
#endif /* NXPBUILD__PHAL_ICODE_SW */

#ifdef NXPBUILD__PHAL_T1T_SW
        case PH_COMP_AL_T1T:
            pDataparam = (void *) &gphNfcLib_Params.salT1T;
            break;
#endif /* NXPBUILD__PHAL_T1T_SW */

#ifdef NXPBUILD__PHAL_TOP_SW
        case PH_COMP_AL_TOP:
            pDataparam = (void *) &gphNfcLib_Params.salTop;
            break;
#endif /* NXPBUILD__PHAL_TOP_SW */

#ifdef NXPBUILD__PHAL_I18000P3M3_SW
        case PH_COMP_AL_I18000P3M3:
            pDataparam = (void *) &gphNfcLib_Params.salI18000p3m3;
            break;
#endif /* NXPBUILD__PHAL_I18000P3M3_SW*/

#ifdef NXPBUILD__PHAC_DISCLOOP_SW
        case PH_COMP_AC_DISCLOOP:
            pDataparam = (void *) &gphNfcLib_Params.sDiscLoop;
            break;
#endif /* NXPBUILD__PHAC_DISCLOOP_SW */

#ifdef NXPBUILD__PHCE_T4T_SW
        case PH_COMP_CE_T4T:
            pDataparam = (void *) &gphNfcLib_Params.sceT4T;
            break;
#endif /* NXPBUILD__PHCE_T4T_SW */

#if defined(NXPBUILD__PH_KEYSTORE_SW) || defined(NXPBUILD__PH_KEYSTORE_RC663)
        case PH_COMP_KEYSTORE:
            pDataparam = (void *) &gphNfcLib_Params.sKeyStore;
            break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */

#ifdef NXPBUILD__PH_CRYPTOSYM_SW
        case PH_COMP_CRYPTOSYM:
            pDataparam = (void *) &gphNfcLib_Params.sCryptoSym;
            break;
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */

#ifdef NXPBUILD__PH_CRYPTORNG_SW
        case PH_COMP_CRYPTORNG:
            pDataparam = (void *) &gphNfcLib_Params.sCryptoRng;
            break;
#endif /* NXPBUILD__PH_CRYPTORNG_SW */

        default:
            /* Do nothing. pDataparam is already null. */
            break;
        }
    }
    return pDataparam;
}

#endif /* NXPBUILD__PHNFCLIB */
