/*
*                     Copyright 2016-2019, NXP
*
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
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHNFCLIB_INITIALIZATION_H
#define PHNFCLIB_INITIALIZATION_H

#include <ph_Status.h>

#ifdef NXPBUILD__PHNFCLIB

#include <phNfcLib.h>

typedef phStatus_t (*pphNfcLib_Config_Profile)(void);

#define PH_CHECK_NFCLIB_SUCCESS(status)         {if ((status) != PH_ERR_SUCCESS) {return (PH_NFCLIB_STATUS_INVALID_PARAMETER);}}
#define PH_CHECK_NFCLIB_SUCCESS_FCT(status,fct) {(status) = (fct); PH_CHECK_NFCLIB_SUCCESS(status);}

#define PH_NFCLIB_INT_TRANSMIT_OFF                  0x00U
#define PH_NFCLIB_INT_TRANSMIT_ON                   0x01U
#define PH_NFCLIB_INT_TRANSMIT_SUCCESS_CHAINING     0x02U

/* By default invalid authenticatation status */
#define PH_NFCLIB_MFDF_NOT_AUTHENTICATED             0xFFU   /**< No authentication. */

/**
* \brief NFCLIB parameter structure
*/
typedef struct
{
    uint16_t                            wId;               /**< ID of this component, do not modify. */

    void                              *pBal;               /**< Holds BAL context */

    #ifdef NXPBUILD__PHHAL_HW_RC663
    phhalHw_Rc663_DataParams_t         sHal;               /**< HAL component holder */
    #endif /* NXPBUILD__PHHAL_HW_RC663 */

    #ifdef NXPBUILD__PHHAL_HW_PN5180
    phhalHw_Pn5180_DataParams_t        sHal;               /**< HAL component holder */
    #endif /* NXPBUILD__PHHAL_HW_PN5180 */

    #ifdef NXPBUILD__PHHAL_HW_PN5190
    phhalHw_Pn5190_DataParams_t        sHal;               /**< HAL component holder */
    #endif /* NXPBUILD__PHHAL_HW_PN5190 */

    #ifdef NXPBUILD__PHHAL_HW_PN7462AU
    phhalHw_PN7462AU_DataParams_t      sHal;               /**< HAL component holder */
    #endif /* NXPBUILD__PHHAL_HW_PN7462AU */

    #ifdef NXPBUILD__PH_KEYSTORE_SW
    phKeyStore_Sw_DataParams_t         sKeyStore;          /**< SW Keystore component holder */
    #endif /* NXPBUILD__PH_KEYSTORE_SW */

    #ifdef NXPBUILD__PH_KEYSTORE_RC663
    phKeyStore_Rc663_DataParams_t      sKeyStore;          /**< Rc663 HW Keystore component holder */
    #endif /* NXPBUILD__PH_KEYSTORE_RC663 */

    pphNfcLib_ErrCallbck               pNfcLib_ErrCallbck; /**< Call back in case of error in activation(applicable for activation profile #PH_NFCLIB_ACTIVATION_PROFILE_EMVCO) */

    /* PAL Data Parameter Structures */

    #ifdef NXPBUILD__PHPAL_I14443P3A_SW
    phpalI14443p3a_Sw_DataParams_t     spalI14443p3a;      /* PAL ISO I14443-A component */
    #endif /* NXPBUILD__PHPAL_I14443P3A_SW */

    #ifdef NXPBUILD__PHPAL_I14443P3B_SW
    phpalI14443p3b_Sw_DataParams_t     spalI14443p3b;      /* PAL ISO I14443-B component */
    #endif /* NXPBUILD__PHPAL_I14443P3B_SW */

    #ifdef NXPBUILD__PHPAL_I14443P4A_SW
    phpalI14443p4a_Sw_DataParams_t     spalI14443p4a;      /* PAL ISO I14443-4A component */
    #endif /* NXPBUILD__PHPAL_I14443P4A_SW */

    #ifdef NXPBUILD__PHPAL_I14443P4_SW
    phpalI14443p4_Sw_DataParams_t      spalI14443p4;       /* PAL ISO I14443-4 component */
    #endif /* NXPBUILD__PHPAL_I14443P4_SW */

    #ifdef NXPBUILD__PHPAL_SLI15693_SW
    phpalSli15693_Sw_DataParams_t      spalSli15693;       /* PAL ISO 15693 component */
    #endif /* NXPBUILD__PHPAL_SLI15693_SW */

    #ifdef NXPBUILD__PHPAL_I18000P3M3_SW
    phpalI18000p3m3_Sw_DataParams_t    spalI18000p3m3;     /* PAL ISO 18000p3m3 component */
    #endif /* NXPBUILD__PHPAL_I18000P3M3_SW */

    #ifdef NXPBUILD__PHPAL_FELICA_SW
    phpalFelica_Sw_DataParams_t        spalFelica;         /* PAL FeliCa component */
    #endif /* NXPBUILD__PHPAL_FELICA_SW */

    #ifdef NXPBUILD__PHPAL_I18092MPI_SW
    phpalI18092mPI_Sw_DataParams_t     spalI18092mPI;      /* PAL ISO 18092 Initiator component */
    #endif /* NXPBUILD__PHPAL_I18092MPI_SW */

    #ifdef NXPBUILD__PHPAL_I18092MT_SW
    phpalI18092mT_Sw_DataParams_t      spalI18092mT;       /* PAL ISO 18092 Target component */

    pRtoxTimerCallback                 pRtoxCallback;      /**< Holds the function pointer of RTOX callback */
    #endif /* NXPBUILD__PHPAL_I18092MT_SW */

    #ifdef NXPBUILD__PHPAL_I14443P4MC_SW
    phpalI14443p4mC_Sw_DataParams_t    spalI14443p4mC;     /* PAL 14443-4mC Target component */

    pWtxTimerCallback                  pWtxCallback;       /**< Holds the function pointer of WTX callback */
    #endif /* NXPBUILD__PHPAL_I14443P4MC_SW */

    #ifdef NXPBUILD__PHPAL_MIFARE_SW
    phpalMifare_Sw_DataParams_t        spalMifare;         /* PAL MIFARE product component */
    #endif /* NXPBUILD__PHPAL_MIFARE_SW */

    #ifdef NXPBUILD__PHPAL_EPCUID_SW
    phpalEpcUid_Sw_DataParams_t        spalEpcUid;         /* Generic ICode EPC/UID Component */
    #endif /* NXPBUILD__PHPAL_EPCUID_SW */

    /* AL Data Parameter Structures */
    #ifdef NXPBUILD__PHAL_I18000P3M3_SW
    phalI18000p3m3_Sw_DataParams_t     salI18000p3m3;      /* AL ISO 18000p3m3 component */
    #endif /* NXPBUILD__PHAL_I18000P3M3_SW */

    #ifdef NXPBUILD__PHAL_FELICA_SW
    phalFelica_Sw_DataParams_t         salFelica;          /* AL FeliCa component */
    #endif /* NXPBUILD__PHAL_FELICA_SW */

    #ifdef NXPBUILD__PHAL_ICODE_SW
    phalICode_Sw_DataParams_t         salICode;          /* ICode AL component */
    #endif /* NXPBUILD__PHAL_ICODE_SW */

    #ifdef NXPBUILD__PHAL_MFC_SW
    phalMfc_Sw_DataParams_t            salMFC;             /* AL MIFARE Classic contactless IC component */
    #endif /* NXPBUILD__PHAL_MFC_SW */

    #ifdef NXPBUILD__PHAL_MFDF_SW
    phalMfdf_Sw_DataParams_t           salMFDF;            /* AL MIFARE DESFire contactless IC component */
    #endif /* NXPBUILD__PHAL_MFDF_SW */

    #ifdef NXPBUILD__PHAL_MFUL_SW
    phalMful_Sw_DataParams_t           salMFUL;            /* AL MIFARE Ultralight contactless IC component */
    #endif /* NXPBUILD__PHAL_MFUL_SW */

    #ifdef NXPBUILD__PHAL_T1T_SW
    phalT1T_Sw_DataParams_t            salT1T;            /* AL T1T component */
    #endif /* NXPBUILD__PHAL_T1T_SW */

    #ifdef NXPBUILD__PHAL_TOP_SW
    phalTop_Sw_DataParams_t            salTop;            /* Tag operations component */
    #endif /* NXPBUILD__PHAL_TOP_SW */

    #ifdef NXPBUILD__PHAC_DISCLOOP_SW
    phacDiscLoop_Sw_DataParams_t       sDiscLoop;          /* Discovery loop component */
    #endif /* NXPBUILD__PHAC_DISCLOOP_SW */

    #ifdef NXPBUILD__PHCE_T4T_SW
    phceT4T_Sw_DataParams_t            sceT4T;             /* HCE component */
    #endif /* NXPBUILD__PHCE_T4T_SW */

    /* Crypto Data Parameter Structures */
    #ifdef NXPBUILD__PH_CRYPTOSYM_SW
    phCryptoSym_Sw_DataParams_t       sCryptoSym;          /* Crypto Sym component */
    #endif /* NXPBUILD__PH_CRYPTOSYM_SW */

    #ifdef NXPBUILD__PH_CRYPTORNG_SW
    phCryptoRng_Sw_DataParams_t       sCryptoRng;          /* Crypto Rng component */
    #endif /* NXPBUILD__PH_CRYPTORNG_SW */

} phNfcLib_DataParams_t;

/**
 * \brief NFC LIB StateMachine
 * \anchor NfcLib_StateMachine
 * Refer state machine for each mode defined above.
 */
typedef enum
{
    eNfcLib_ResetState = 0,
    eNfcLib_InitializedState = 1,
    eNfcLib_ActivatedState = 2,
    eNfcLib_DeactOngoingState = 3,
    eNfcLib_LAST = 0x7F /* Where facilitated by compiler, promote to 8 bit number */
} phNfcLib_StateMachine_t;

/**
 * \brief NFC LIB State
 * \anchor NfcLib_State
 */
typedef struct
{
    phNfcLib_StateMachine_t  bNfcLibState;                         /* State of the NfcLib takes value from enum phNfcLib_StateMachine_t */
    uint8_t  bProfileSelected;                                     /* Tells about the selected profile takes value ISO, NFC, EMVCO */
    uint8_t  bActivateBlocking;                                    /* Tells the blocking #PH_ON or #PH_OFF feature of Activation */
    uint8_t  bDeactBlocking;                                       /* Tells the blocking #PH_ON or #PH_OFF feature of DeActivation */
    uint8_t  bMergedSakPrio;                                       /* Tells the priority to perform activation of the card in case Merged Sak is received */
    uint8_t  bLPCDState;                                           /* Tells the LPCD #PH_ON or #PH_OFF performance */
    uint8_t  bTxState;                                             /* Tells the TX state of nfclib, if 1 then only receive can be performed */
    uint16_t wConfiguredRFTech;                                    /* Tells the configured RF Tech that was used for current activation cycle */
#ifdef NXPBUILD__PHNFCLIB_PROFILES
    phNfcLib_PeerInfo_t * pPeerInfo;
#endif /* NXPBUILD__PHNFCLIB_PROFILES */
    uint8_t  *pRxBuffer;                                           /* Pointer to the RX */
    uint16_t wRxLength;                                            /* Stores the length of the received Data */
    uint16_t  wRxDataPoin;                                         /* Points to the starting address of the rx buffer to perform Chaining */
    uint8_t  bHalBufferTx[PH_NXPNFCRDLIB_CONFIG_HAL_TX_BUFFSIZE];  /* HAL TX buffer. */
    uint8_t  bHalBufferRx[PH_NXPNFCRDLIB_CONFIG_HAL_RX_BUFFSIZE];  /* HAL RX buffer. */
    uint8_t bWrappedMode;                                          /* Tells about the MIFARE DESFire Wrapped Mode Status */
    uint8_t bAuthMode;                                             /* Tells about the MIFARE DESFire Auth Mode */
    uint16_t wActivatedUIDLength;                                  /* Length of the activated UID */
    uint8_t* pActivatedUid;                                        /* Contains the pointer of the activated Uid */

} phNfcLib_InternalState_t;

extern phNfcLib_DataParams_t    gphNfcLib_Params;
extern phNfcLib_InternalState_t gphNfcLib_State;

#endif /* NXPBUILD__PHNFCLIB */

#endif /* PHNFCLIB_INITIALIZATION_H */
