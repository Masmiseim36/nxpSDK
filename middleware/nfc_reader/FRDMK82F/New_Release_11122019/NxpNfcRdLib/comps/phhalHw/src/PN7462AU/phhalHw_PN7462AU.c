/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2013
 *
 *         All rights are reserved. Reproduction in whole or in part is
 *        prohibited without the written consent of the copyright owner.
 *    NXP reserves the right to make changes without notice at any time.
 *   NXP makes no warranty, expressed, implied or statutory, including but
 *   not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 *   copyright or trademark. NXP must not be liable for any loss or damage
 *                            arising from its use.
 */

/**
 *
 * \file phhalHw_PN7462AU.c : This file contains wrapper Api's which connects
 *                         the Reader library interfaces with the phhalRf interfaces.
 *
 * Project:  PN7462AU
 *
 * $Date$
 * $Author$
 * $Revision$ (v06.01.00)
 *
 */

/*********************************************************************************************************************/
/*   INCLUDES                                                                                                        */
/*********************************************************************************************************************/

#include "phhalHw.h"
#ifdef NXPBUILD__PH_KEYSTORE
#include <phKeyStore.h>
#endif /* NXPBUILD__PH_KEYSTORE*/

#ifdef NXPBUILD__PHHAL_HW_PN7462AU

#include "phhalHw_PN7462AU.h"
#include "phhalRf.h"
#include "phUser.h"
#include "phhalRf_LP_Int.h"
#include "phhalHw_PN7462AU_Int.h"

/*********************************************************************************************************************/
/*   DEFINITIONS                                                                                                     */
/*********************************************************************************************************************/

#define PHPAL_MIFARE_KEYA           0x0AU
#define PHPAL_MIFARE_KEYB           0x0BU
#define PHHAL_HW_TXSTARTSYMBOL0     0x01U
#define PHHAL_HW_TXSTARTSYMBOL1     0x02U
#define PHHAL_HW_15693_HIGH         0x00U
#define PHHAL_HW_15693_FASTHIGH     0x01U
#define PHHAL_HW_15693_RX106        0x02U
#define PHHAL_HW_EPC_423_MAN2       0x00U
#define PHHAL_HW_EPC_423_MAN4       0x01U
#define PHHAL_HW_EPC_847_MAN2       0x02U
#define PHHAL_HW_EPC_847_MAN4       0x03U
#define PHHAL_HW_SINGLESIZEUID      0x04U
#define PHHAL_HW_CRC5_PRESET        0x04U
#define PHHAL_HW_15693_TX53         0x04U
#define PHHAL_HW_15693_TX106        0x05U
#define PHHAL_HW_15693_TX212        0x06U


/*********************************************************************************************************************/
/*   LOCALIZED GLOBAL VARIABLES                                                                                      */
/*********************************************************************************************************************/

/**
 * HalRf Rx parameters structure, we need this variable as global because of getconfig api.
 * Some fields of this structure will be returned via get config api
 */
PH_NOINIT static phhalRf_RxParams_t gphhalRf_RxParams;

/*********************************************************************************************************************/
/*   PRIVATE FUNCTIONS PROTOTYPES                                                                                    */
/*********************************************************************************************************************/


/*********************************************************************************************************************/
/*   PUBLIC FUNCTIONS                                                                                                */
/*********************************************************************************************************************/

/**
 * \brief This api is used to initialize the phhalRf.
 * \description The initialization api of the bare metal Rf hal. The default configuration of the device is
 *              made as Type A reader. Also the PN7462AU hardware  context structure is also initialized with the
 *              Rx and Tx buffer.
 *
 * \param phhalHwClifRdLib_DataParams : This is the context structure for the PN7462AU hal component of Reader library
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */

phStatus_t phhalHw_PN7462AU_Init(phhalHw_PN7462AU_DataParams_t * pDataParams,
    uint16_t wSizeOfDataParams,
    void * pBalDataParams,
    void * pKeyStoreDataParams,
    uint8_t* pRdLibTxBuf,
    uint16_t wRdlibTxBufSize,
    uint8_t* pRdLibRxBuf,
    uint16_t wRdlibRxBufSize)
{
    phStatus_t status = PH_ERR_INTERNAL_ERROR;

    if (sizeof(phhalHw_PN7462AU_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
    }
    if (wRdlibTxBufSize == 0U || wRdlibRxBufSize == 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* initialising the hal context structure */
    pDataParams->wId                                = PHHAL_HW_PN7462AU_ID | PH_COMP_HAL;
    pDataParams->bCardType                          = PHHAL_HW_CARDTYPE_UNKNOWN;
    pDataParams->bFrameStep                         = PH_OFF;
    pDataParams->bSession                           = 0;
    pDataParams->bRxMultiple                        = PH_OFF;
    pDataParams->bActiveMode                        = PH_OFF;
    pDataParams->bPiccAState                        = PHHAL_RF_CMA_ENTRY_STATE_IDLE;
    pDataParams->bNfcipMode                         = 0;
    pDataParams->bMfcCryptoDisabled                 = 1;
    pDataParams->bAsk100                            = 1;
    pDataParams->bTxLastBits                        = 0;
    pDataParams->pTxBuffer                          = (uint8_t *)(pRdLibTxBuf);
    pDataParams->pRxBuffer                          = (uint8_t *)(pRdLibRxBuf);
    pDataParams->wRxSubCarrier                      = PHHAL_HW_SUBCARRIER_SINGLE;
    pDataParams->wTxDataRate                        = PHHAL_HW_RF_DATARATE_106;
    pDataParams->wRxDataRate                        = PHHAL_HW_RF_DATARATE_106;
    pDataParams->wTxBufLen                          = 0;
    pDataParams->wParity                            = PH_OFF;
    pDataParams->wOpeMode                           = RD_LIB_MODE_NFC;
    pDataParams->wFieldOffTime                      = 5000;
    pDataParams->wFieldRecoveryTime                 = 5000;
    pDataParams->wTxBufSize                         = wRdlibTxBufSize;
    pDataParams->wRxBufSize                         = wRdlibRxBufSize;
    pDataParams->wAdditionalInfo                    = 0;
    pDataParams->wRxBufStartPos                     = 0;
    pDataParams->wCurrentSlotCount                  = 0;
    pDataParams->wTargetMode                        = PH_OFF;
    pDataParams->dwFdt                              = 0;
    pDataParams->dwLpcdThreshold                    = PHHAL_HW_PN7462AU_LPCD_THRE_DEFAULT;
    pDataParams->dwLpcdReference                    = PHHAL_HW_PN7462AU_LPCD_REF_DEFAULT;
    pDataParams->dwLpcd_NewValue                    = 0;
    pDataParams->dwI15693Sof                        = 1;
    pDataParams->wRxBufLen                          = 0;
    pDataParams->pKeyStoreDataParams                = pKeyStoreDataParams;

    phUser_MemSet(&pDataParams-> abStoredCRC[0], 0x00, 0x02);
    phUser_MemSet(&pDataParams-> pAutocoll, 0x00, sizeof(pDataParams-> pAutocoll));

    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_RX_MAX_LENGTH, wRdlibRxBufSize);

    status = status & PH_ERR_MASK;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}


/**
 * \brief This api is used to set the configuration parameter.
 * \description This api sets several configurations of the PN7462AU Clif hardware.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR None of the configuration matches.
 */
phStatus_t phhalHw_PN7462AU_SetConfig(phhalHw_PN7462AU_DataParams_t * pDataParams,
    uint16_t wConfig, uint16_t wValue)
{
    phStatus_t status = PH_ERR_SUCCESS;
    uint32_t dwValueTmp;

    switch(wConfig)
    {
    case PHHAL_HW_CONFIG_PARITY:
    {
        if(wValue > 1U)
        {
            status = PH_ERR_INVALID_PARAMETER;
        }
        else
        {
            /* configuring the parity */
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_PARITY, (uint32_t)(wValue));
            pDataParams->wParity = wValue;
        }
    }
    break;

    case PHHAL_HW_CONFIG_TXCRC:
    {
        if(wValue > 1U)
        {
            status = PH_ERR_INVALID_PARAMETER;
        }
        else
        {
            /* configuring the tx crc */
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TXCRC, (uint32_t)(wValue));
        }
    }
    break;

    case PHHAL_HW_CONFIG_RXCRC:
    {
        if(wValue > 1U)
        {
            status = PH_ERR_INVALID_PARAMETER;
        }
        else
        {
            /* configuring the rx crc */
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_RXCRC, (uint32_t)(wValue));
        }
    }
    break;

    case PHHAL_HW_CONFIG_TXLASTBITS:
    {
        if(wValue > 7U)
        {
            status = PH_ERR_INVALID_PARAMETER;
        }
        else
        {
            pDataParams->bTxLastBits = (uint8_t) wValue;
            /* Configuring the last bits */
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, (uint32_t)(wValue));
        }
    }
    break;

    case PHHAL_HW_CONFIG_RXALIGN:
    {
        if (wValue > 7U)
        {
            status = PH_ERR_INVALID_PARAMETER;
        }
        else
        {
            /* Configuring the rx aligned bits */
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_RXALIGN, (uint32_t)(wValue));
        }
    }
    break;

    case PHHAL_HW_CONFIG_RXWAIT_US:
    {
        /* configuring the RX Wait value */
        status =  phhalRf_SetConfig(PHHAL_RF_CONFIG_RXWAIT_US, wValue);
    }
    break;

    case PHHAL_HW_CONFIG_TXWAIT_US:
    {
        /* configuring the TX Wait value */
        status =  phhalRf_SetConfig(PHHAL_RF_CONFIG_TXWAIT_US, wValue);
    }
    break;

    /* We support asymmetric Baud Rate. */
    case PHHAL_HW_CONFIG_TXDATARATE_FRAMING:
    {
        if (((wValue & PHHAL_HW_RF_FRAMING_OPTION_MASK) == PHHAL_HW_RF_TYPE_ACTIVE_FRAMING)
                && ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) < 4U ))
        {
            if(0U != (pDataParams->wTargetMode))
            {
                switch(wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK)
                {
                case PHHAL_HW_RF_DATARATE_106:
                {
                    status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_A_106_ACT, E_PHHAL_RF_LP_T_RX_KEEP_ASIS);
                }
                break;
                case PHHAL_HW_RF_DATARATE_212:
                {
                    status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_F_212_ACT, E_PHHAL_RF_LP_T_RX_KEEP_ASIS);
                }
                break;
                case PHHAL_HW_RF_DATARATE_424:
                {
                    status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_F_424_ACT, E_PHHAL_RF_LP_T_RX_KEEP_ASIS);
                }
                break;
                default:
                    status = PH_ERR_INVALID_PARAMETER;
                    break;
                }
            }
            else
            {
                switch(wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK)
                {
                case PHHAL_HW_RF_DATARATE_106:
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_A_106, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                }
                break;
                case PHHAL_HW_RF_DATARATE_212:
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_F_212, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                }
                break;
                case PHHAL_HW_RF_DATARATE_424:
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_F_424, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                }
                break;
                default:
                    status = PH_ERR_INVALID_PARAMETER;
                    break;
                }
            }
        }
        else if (((wValue & PHHAL_HW_RF_FRAMING_OPTION_MASK) == PHHAL_HW_RF_TYPE_A_FRAMING)
                    && ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) < 4U ))
        {
            if(0U != (pDataParams->wTargetMode))
            {
                status = phhalRf_LoadProtocol_Target((phhalRf_LP_T_Tx_t)(PHHAL_HW_CARDTYPE_ISO14443A +
                            (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 1U) , E_PHHAL_RF_LP_T_RX_KEEP_ASIS);
            }
            else
            {

                status = phhalRf_LoadProtocol_Initiator((phhalRf_LP_I_Tx_t)(PHHAL_HW_CARDTYPE_ISO14443A +
                            (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 1U) ,E_PHHAL_RF_LP_I_RX_KEEP_ASIS);

            }
        }
        else if (((wValue & PHHAL_HW_RF_FRAMING_OPTION_MASK) == PHHAL_HW_RF_TYPE_B_FRAMING)
                    && ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) < 4U ))
        {
            if(0U != (pDataParams->wTargetMode))
            {
                status = phhalRf_LoadProtocol_Target((phhalRf_LP_T_Tx_t)(PHHAL_HW_CARDTYPE_ISO14443B +
                            (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 8U) ,E_PHHAL_RF_LP_T_RX_KEEP_ASIS);
            }
            else
            {
                status = phhalRf_LoadProtocol_Initiator((phhalRf_LP_I_Tx_t)(PHHAL_HW_CARDTYPE_ISO14443B +
                            (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 5U) ,E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
            }
        }
        else if (((wValue & PHHAL_HW_RF_FRAMING_OPTION_MASK) == PHHAL_HW_RF_TYPE_F_FRAMING)
                    && ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) < 4U ))
        {
            if(0U != (pDataParams->wTargetMode))
            {
                switch(wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK)
                {
                case PHHAL_HW_RF_DATARATE_212:
                {
                    status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_F_212_P, E_PHHAL_RF_LP_T_RX_KEEP_ASIS);
                }
                break;
                case PHHAL_HW_RF_DATARATE_424:
                {
                    status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_F_424_P, E_PHHAL_RF_LP_T_RX_KEEP_ASIS);
                }
                break;
                default:
                    status = PH_ERR_INVALID_PARAMETER;
                    break;
                }
            }

            else
            {
                switch(wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK)
                {
                case PHHAL_HW_RF_DATARATE_212:
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_F_212, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                }
                break;
                case PHHAL_HW_RF_DATARATE_424:
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_F_424, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                }
                break;
                default:
                    status = PH_ERR_INVALID_PARAMETER;
                    break;
                }
            }
        }

        else
        {
            switch(pDataParams->bCardType)
            {
            case PHHAL_HW_CARDTYPE_ISO14443A:
            {
                if((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) > 3U)
                {
                    status = PH_ERR_INVALID_PARAMETER;
                }
                else
                {
                    status = phhalRf_LoadProtocol_Initiator((phhalRf_LP_I_Tx_t)(PHHAL_HW_CARDTYPE_ISO14443A +
                                (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 1U) ,E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                }
            }
            break;

            case PHHAL_HW_CARDTYPE_ISO14443B:
            {
                if((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) > 3U)
                {
                    status = PH_ERR_INVALID_PARAMETER;
                }
                else
                {
                    status = phhalRf_LoadProtocol_Initiator((phhalRf_LP_I_Tx_t)(PHHAL_HW_CARDTYPE_ISO14443B +
                                (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 5U) ,E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                }
            }
            break;

            case PHHAL_HW_CARDTYPE_ISO15693:
            {
                /* If the requested baud rate is 26 kbps, load the initial setting according to ask configuration */
                if((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) == PHHAL_HW_RF_TX_DATARATE_1_OUT_OF_4)
                {
                    if(0U != (pDataParams->bAsk100))
                    {
                        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_15693_100, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                    }
                    else
                    {
                        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_15693_10, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                    }
                }

                /* If the requested baud rate is 53 kbps, load the initial setting according to ask configuration */
                else if((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) == PHHAL_HW_RF_I15693_53KBPS_DATARATE)
                {
                    if(0U != (pDataParams->bAsk100))
                    {
                        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_15693_100, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                    }
                    else
                    {
                        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_15693_10, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                    }
                    /* apply the 53kbps specific setting */
                    phhalRf_SetConfig(PHHAL_RF_CONFIG_15693_TX_BAUDRATE, PHHAL_HW_15693_TX53);
                }

                /* If the requested baud rate is 106 kbps, load the initial setting according to ask configuration */
                else if((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) == PHHAL_HW_RF_DATARATE_106)
                {
                    if(0U != (pDataParams->bAsk100))
                    {
                        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_15693_100, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                    }
                    else
                    {
                        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_15693_10, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                    }
                    /* apply the 106kbps specific setting */
                    phhalRf_SetConfig(PHHAL_RF_CONFIG_15693_TX_BAUDRATE, PHHAL_HW_15693_TX106);
                }

                /* If the requested baud rate is 212 kbps, load the initial setting according to ask configuration */
                else if((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) == PHHAL_HW_RF_DATARATE_212)
                {
                    if(0U != (pDataParams->bAsk100))
                    {
                        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_15693_100, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                    }
                    else
                    {
                        status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_15693_10, E_PHHAL_RF_LP_I_RX_KEEP_ASIS);
                    }
                    /* apply the 212kbps specific setting */
                    phhalRf_SetConfig(PHHAL_RF_CONFIG_15693_TX_BAUDRATE, PHHAL_HW_15693_TX212);
                }
                else
                {
                    status = PH_ERR_INVALID_PARAMETER;
                }
            }
            break;

            case PHHAL_HW_CARDTYPE_I18000P3M3:
            {
                status = PH_ERR_SUCCESS;
            }
            break;
            default:
                status = PH_ERR_INVALID_PARAMETER;
                break;
            }
        }
        pDataParams->wTxDataRate = wValue;
    }
    break;

    /* We support asymmetric Baud Rate. */
    case PHHAL_HW_CONFIG_RXDATARATE_FRAMING:
    {
        if (((wValue & PHHAL_HW_RF_FRAMING_OPTION_MASK) == PHHAL_HW_RF_TYPE_ACTIVE_FRAMING)
                && ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) < 4U ))
        {
            if(0U != (pDataParams->wTargetMode))
            {
                switch(wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK)
                {
                case PHHAL_HW_RF_DATARATE_106:
                {
                    status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_KEEP_ASIS, E_PHHAL_RF_LP_T_RX_A_106_ACT);
                }
                break;
                case PHHAL_HW_RF_DATARATE_212:
                {
                    status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_KEEP_ASIS, E_PHHAL_RF_LP_T_RX_F_212_ACT);
                }
                break;
                case PHHAL_HW_RF_DATARATE_424:
                {
                    status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_KEEP_ASIS, E_PHHAL_RF_LP_T_RX_F_424_ACT);
                }
                break;
                default:
                    status = PH_ERR_INVALID_PARAMETER;
                    break;
                }
            }
            else
            {
                switch(wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK)
                {
                case PHHAL_HW_RF_DATARATE_106:
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_KEEP_ASIS, E_PHHAL_RF_LP_I_RX_ACT_106);
                }
                break;
                case PHHAL_HW_RF_DATARATE_212:
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_KEEP_ASIS, E_PHHAL_RF_LP_I_RX_ACT_212);
                }
                break;
                case PHHAL_HW_RF_DATARATE_424:
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_KEEP_ASIS, E_PHHAL_RF_LP_I_RX_ACT_424);
                }
                break;
                default:
                    status = PH_ERR_INVALID_PARAMETER;
                    break;
                }
            }
        }
        else if (((wValue & PHHAL_HW_RF_FRAMING_OPTION_MASK) == PHHAL_HW_RF_TYPE_A_FRAMING)
                    && ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) < 4U ))
        {
            if(0U != (pDataParams->wTargetMode))
            {
                status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_KEEP_ASIS ,
                (phhalRf_LP_T_Rx_t)(PHHAL_HW_CARDTYPE_ISO14443A + (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 1U) );
            }
            else
            {
                status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_KEEP_ASIS,
                (phhalRf_LP_I_Rx_t)(PHHAL_HW_CARDTYPE_ISO14443A + (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 1U));
            }
        }
        else if (((wValue & PHHAL_HW_RF_FRAMING_OPTION_MASK) == PHHAL_HW_RF_TYPE_B_FRAMING)
                    && ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) < 4U ))
        {
            if(0U != (pDataParams->wTargetMode))
            {
                status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_KEEP_ASIS ,
                (phhalRf_LP_T_Rx_t)(PHHAL_HW_CARDTYPE_ISO14443B + (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 8U));
            }
            else
            {
                status = phhalRf_LoadProtocol_Initiator((phhalRf_LP_I_Tx_t)E_PHHAL_RF_LP_I_TX_KEEP_ASIS ,
                (phhalRf_LP_I_Rx_t)(PHHAL_HW_CARDTYPE_ISO14443B + (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 8U));
            }
        }
        else if (((wValue & PHHAL_HW_RF_FRAMING_OPTION_MASK) == PHHAL_HW_RF_TYPE_F_FRAMING)
                    && ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) < 4U ))
        {
            if(0U != (pDataParams->wTargetMode))
            {
                switch(wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK)
                {
                case PHHAL_HW_RF_DATARATE_212:
                {
                    status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_KEEP_ASIS, E_PHHAL_RF_LP_T_RX_F_212);
                }
                break;
                case PHHAL_HW_RF_DATARATE_424:
                {
                    status = phhalRf_LoadProtocol_Target(E_PHHAL_RF_LP_T_TX_KEEP_ASIS, E_PHHAL_RF_LP_T_RX_F_424);
                }
                break;
                default:
                    status = PH_ERR_INVALID_PARAMETER;
                    break;
                }
            }
            else
            {
                switch(wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK)
                {
                case PHHAL_HW_RF_DATARATE_212:
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_KEEP_ASIS, E_PHHAL_RF_LP_I_RX_F_212_P);
                }
                break;
                case PHHAL_HW_RF_DATARATE_424:
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_KEEP_ASIS, E_PHHAL_RF_LP_I_RX_F_424_P);
                }
                break;
                default:
                    status = PH_ERR_INVALID_PARAMETER;
                    break;
                }
            }
        }
        else
        {
            switch(pDataParams->bCardType)
            {
            case PHHAL_HW_CARDTYPE_ISO14443A:
            {
                if ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) < 4U )
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_KEEP_ASIS,
                    (phhalRf_LP_I_Rx_t)(PHHAL_HW_CARDTYPE_ISO14443A + (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 1U));
                }
                else
                {
                    status = PH_ERR_INVALID_PARAMETER;
                }
            }
            break;

            case PHHAL_HW_CARDTYPE_ISO14443B:
            {
                if ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) < 4U )
                {
                    status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_KEEP_ASIS,
                    (phhalRf_LP_I_Rx_t)(PHHAL_HW_CARDTYPE_ISO14443B + (wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) + 5U));
                }
                else
                {
                    status = PH_ERR_INVALID_PARAMETER;
                }
            }
            break;
            case PHHAL_HW_CARDTYPE_ISO15693:
            {
                switch(wValue)
                {
                case PHHAL_HW_RF_RX_DATARATE_LOW:
                case PHHAL_HW_RF_RX_DATARATE_FAST_LOW:
                case PHHAL_HW_RF_DATARATE_212:
                {
                    status = PH_ERR_UNSUPPORTED_PARAMETER;
                }
                break;

                case PHHAL_HW_RF_RX_DATARATE_HIGH:
                {
                    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_15693_RX_BAUDRATE, (uint32_t)(PHHAL_HW_15693_HIGH));
                }
                break;

                case PHHAL_HW_RF_RX_DATARATE_FAST_HIGH:
                {
                    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_15693_RX_BAUDRATE, (uint32_t)(PHHAL_HW_15693_FASTHIGH));
                }
                break;
                case PHHAL_HW_RF_DATARATE_106:
                {
                    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_15693_RX_BAUDRATE, (uint32_t)(PHHAL_HW_15693_RX106));
                }
                break;
                default:
                    status = PH_ERR_INVALID_PARAMETER;
                    break;
                }
            }
            break;
            case PHHAL_HW_CARDTYPE_I18000P3M3:
            {
                switch(wValue)
                {
                case PHHAL_HW_RX_I18000P3M3_FL_423_MAN2:
                {
                    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_EPC_RX_BAUDRATE, (uint32_t)(PHHAL_HW_EPC_423_MAN2));
                }
                break;
                case PHHAL_HW_RX_I18000P3M3_FL_847_MAN2:
                {
                    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_EPC_RX_BAUDRATE, (uint32_t)(PHHAL_HW_EPC_847_MAN2));
                }
                break;
                case PHHAL_HW_RX_I18000P3M3_FL_423_MAN4:
                {
                    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_EPC_RX_BAUDRATE, (uint32_t)(PHHAL_HW_EPC_423_MAN4));
                }
                break;
                case PHHAL_HW_RX_I18000P3M3_FL_847_MAN4:
                {
                    status = phhalRf_SetConfig(PHHAL_RF_CONFIG_EPC_RX_BAUDRATE, (uint32_t)(PHHAL_HW_EPC_847_MAN4));
                }
                break;
                default:
                    status = PH_ERR_INVALID_PARAMETER;
                    break;
                }

            }
            break;
            default:
                status = PH_ERR_INVALID_PARAMETER;
                break;
            }
        }
        pDataParams->wRxDataRate = wValue;
    }
    break;

    case PHHAL_HW_CONFIG_ASK100:
    {
        /* enable or disable 100% ask modulation */
        pDataParams->bAsk100 = (uint8_t)wValue;
        status = phhalRf_SetConfig(PHHAL_RF_CONFIG_ASK100, (uint32_t)wValue);
    }
    break;

    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_US:
    {
        /* fdt in microsecond */
        pDataParams->dwFdt = (uint32_t)(wValue);
        status = phhalRf_PCD_SetExchgFdt((uint32_t)pDataParams->dwFdt);
    }
    break;

    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS:
    {
        /* fdt in millisecond */
        pDataParams->dwFdt = (((uint32_t)(wValue)) * 1000U);
        status = phhalRf_PCD_SetExchgFdt((uint32_t)pDataParams->dwFdt);
    }
    break;

    case PHHAL_HW_CONFIG_SUBCARRIER:
    {
        /* subcarrier to be used while reception by ISO15693 */
        if ((pDataParams->bCardType == PHHAL_HW_CARDTYPE_ISO15693) && (wValue == PHHAL_HW_SUBCARRIER_SINGLE))
        {
            pDataParams->wRxSubCarrier = PHHAL_HW_SUBCARRIER_SINGLE;
        }
        else if(pDataParams->bCardType == PHHAL_HW_CARDTYPE_I18000P3M3)
        {
            status = PH_ERR_SUCCESS;
        }
        else
        {
            status = PH_ERR_UNSUPPORTED_PARAMETER;
        }
    }
    break;

    case PHHAL_HW_CONFIG_FIELD_OFF_TIME:
    {
        /* Field off duration during rf reset */
        if(wValue != 0U)
        {
            pDataParams->wFieldOffTime = ((uint16_t)wValue);
            status = PH_ERR_SUCCESS;
        }
        else
        {
            status = PH_ERR_INVALID_PARAMETER;
        }
    }
    break;

    case PHHAL_HW_CONFIG_FIELD_RECOVERY_TIME:
    {
        /* Field reset duration during rf reset */
        pDataParams->wFieldRecoveryTime = ((uint16_t)wValue);
        status = PH_ERR_SUCCESS;
    }
    break;

    case PHHAL_HW_CONFIG_SYMBOL_START:
    {
        /* start of symbol configuration for ISO15693 and ISO18000p3m3 */
        switch(pDataParams->bCardType)
        {
        case PHHAL_HW_CARDTYPE_I18000P3M3:
        {
            if(wValue == PHHAL_HW_SYMBOL_I18000P3M3_FSYNC)
            {
                status = phhalRf_SetConfig(PHHAL_RF_CONFIG_EPC_TX_SYMBOL, (uint32_t) (PHHAL_HW_TXSTARTSYMBOL1));
            }
            else
            {
                status = phhalRf_SetConfig(PHHAL_RF_CONFIG_EPC_TX_SYMBOL, (uint32_t) (PHHAL_HW_TXSTARTSYMBOL0));
            }
        }
        break;
        case PHHAL_HW_CARDTYPE_ISO15693:
        {
            if (wValue == PHHAL_HW_SYMBOL_I15693_SOF)
            {
                pDataParams->dwI15693Sof = PH_ON;
            }
            else
            {
                pDataParams->dwI15693Sof = PH_OFF;
            }
            status = PH_ERR_SUCCESS;
        }
        break;

        default:
            status = PH_ERR_INVALID_PARAMETER;
            break;
        }
    }
    break;

    case PHHAL_HW_CONFIG_CRCTYPE:
    {
        if((wValue & PH_ERR_MASK) == PHHAL_HW_CRCTYPE_CRC16)
        {
            dwValueTmp = 0x00;
            /* configuring the crc type */
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TXCRCTYPE, dwValueTmp);
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_RXCRCTYPE, dwValueTmp);
            if(status != PH_ERR_SUCCESS)
            {
                break;
            }
        }
        else if((wValue & PH_ERR_MASK) == PHHAL_HW_CRCTYPE_CRC5)
        {
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_TXCRCTYPE, PHHAL_HW_CRCTYPE_CRC5);
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_RXCRCTYPE, PHHAL_HW_CRCTYPE_CRC5);
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_CRCPRESET, PHHAL_HW_CRC5_PRESET);
            if(status != PH_ERR_SUCCESS)
            {
                break;
            }
        }
        else
        {
            /* To avoid the warning */
        }
        status = phhalRf_SetConfig(PHHAL_RF_CONFIG_CRCINVERTED, (uint32_t)((wValue & PH_COMP_MASK)>>8U));

    }
    break;

    case PHHAL_HW_CONFIG_TXWAIT_MS:
    {
        /* configuring tx wait in ms*/
        dwValueTmp = (uint32_t)(wValue*1000U);
        status =  phhalRf_SetConfig(wConfig, wValue);
    }
    break;

    case PHHAL_HW_CONFIG_JEWEL_MODE:
    {
        /* configuring the Jewel mode */
        if (wValue > 1U)
        {
            status = PH_ERR_INVALID_PARAMETER;
        }
        else
        {
            pDataParams->bFrameStep = (uint8_t)(wValue);
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_JEWEL_MODE, wValue);
        }

    }
    break;

    case PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1:
    {
        /* disabling the MF crypto */
       pDataParams->bMfcCryptoDisabled = (uint8_t) wValue;
       if(0U != (wValue))
       {
           status = phhalRf_MFC_Disable();
       }
    }
    break;

    case PHHAL_HW_CONFIG_TXBUFFER_OFFSET:
    {
        /* Setting the value of additional info */
        if(wValue > pDataParams->wTxBufSize)
        {
            status = PH_ERR_BUFFER_OVERFLOW;
        }
        else
        {
            pDataParams->wAdditionalInfo = wValue;
            status = PH_ERR_SUCCESS;
        }
    }
    break;

    case PHHAL_HW_CONFIG_RXBUFFER_STARTPOS:
    {
        /* configuring the value of rxbuffer Startpos */
        if(wValue >= pDataParams->wRxBufSize)
        {
            status = PH_ERR_BUFFER_OVERFLOW;
        }
        else
        {
            pDataParams->wRxBufStartPos = wValue;
            status = PH_ERR_SUCCESS;
        }

    }
    break;

    case  PHHAL_HW_CONFIG_TXBUFFER_LENGTH:
    {
        /* configuring the number of valid bytes in Tx buffer */
        if(wValue > pDataParams->wTxBufSize)
        {
            status = PH_ERR_BUFFER_OVERFLOW;
        }
        else
        {
            pDataParams->wTxBufLen = (uint16_t) wValue;
        }
    }
    break;

    case PHHAL_HW_CONFIG_TXBUFFER:
    {
        /* To set a desired value at particular TxBuffer position */
        pDataParams->pTxBuffer[pDataParams->wAdditionalInfo] = (uint8_t)(wValue);
        status = PH_ERR_SUCCESS;
    }
    break;

    case PHHAL_HW_CONFIG_SETMINFDT:
    {
        status = phhalHw_PN7462AU_SetMinFDT(pDataParams, wValue);
    }
    break;

    case PHHAL_HW_CONFIG_RXMULTIPLE:
    {
        /* setting the rxmultiple for FeliCa */
        pDataParams->bRxMultiple = (uint8_t)(wValue);
        status = PH_ERR_SUCCESS;
    }
    break;

    case PHHAL_HW_CONFIG_MFHALTED:
    {
        /* configuring the IC state in card mode */
        pDataParams->bPiccAState = (uint8_t)(wValue);
        status = PH_ERR_SUCCESS;
    }
    break;

    case PHHAL_HW_CONFIG_RFCA:
    {
        status = PH_ERR_SUCCESS;
    }
    break;

    case PHHAL_HW_CONFIG_RFON_INTERRUPT:
    {
        /* configuring the interrupt for EXT RF ON */
        status = phhalRf_SetConfig(PHHAL_RF_CONFIG_EXT_FIELD_ON_IRQ, E_PH_HALRF_ENABLE);
    }
    break;

    case PHHAL_HW_CONFIG_OPE_MODE:
    {
        /* Configure operation mode for this Layer */
        pDataParams->wOpeMode = wValue;
        status = phhalRf_SetConfig(wConfig, wValue);
    }
    break;

    case PHHAL_HW_CONFIG_NFCIP_STARTBYTE:
    {
        /* Configuring the NFCIP StartByte */
        pDataParams->bNfcipMode = (uint8_t) wValue;
        status = phhalRf_SetConfig(PHHAL_RF_CONFIG_NFCIP1, (uint32_t)wValue);
    }
    break;

    case PHHAL_HW_CONFIG_SET_SYMBOL_SEND:
    {
        /* configuring the symbol for 18000p3m3 */
        status = PH_ERR_SUCCESS;
    }
    break;

    case PHHAL_HW_CONFIG_SET_EMD:
    {
        /* configuring the EMD check, should be enabled only after ISO 14443A/B layer 3 activation */
        status = phhalRf_SetConfig(PHHAL_RF_CONFIG_SET_EMD, (uint32_t)(wValue));
    }
    break;

    case PHHAL_HW_CONFIG_MODINDEX:
    {
        if(wValue > 0x3FU)
        {
            status = PH_ERR_INVALID_PARAMETER;
        }
        else
        {
            /* Configure the modulation index */
            status = phhalRf_SetConfig(PHHAL_RF_CONFIG_MODINDEX, (uint32_t)(wValue));
        }
    }
    break;

    case PHHAL_HW_CONFIG_PN7462AU_CRCPRESET_SEL:
    {
        /* configuring the CRC PRESET SEL value */
        status = phhalRf_SetConfig(PHHAL_RF_CONFIG_CRCPRESET, (uint32_t)wValue);
    }
    break;

    case PHHAL_HW_CONFIG_PN7462AU_LPCD_THRESHOLD:
    {
        /* AGC_CardThreshold Initial Detection Threshold of the AGC to de-tune*/
        pDataParams->dwLpcdThreshold = (uint32_t) wValue;
        status = PH_ERR_SUCCESS;
    }
    break;

    case PHHAL_HW_CONFIG_PN7462AU_LPCD_REF_VALUE:
    {
        /* AGC_RefValue reference value for agc*/
        pDataParams->dwLpcdReference = (uint32_t) wValue;
        status = PH_ERR_SUCCESS;
    }
    break;

    case PHHAL_HW_CONFIG_POLL_GUARD_TIME_US:
        /* Disable Poll guard time by not configuring the timer. */
        if (wValue == 0x00U)
        {
            return PH_ERR_SUCCESS;
        }
        status = phhalRf_SetConfig((uint32_t)wConfig, (uint32_t)wValue);
        break;

    default:
        status = PH_ERR_UNSUPPORTED_PARAMETER;
        break;
    }
    status = status & PH_ERR_MASK;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}



/**
 * \brief This api is used to get the configuration parameter.
 * \description This api gets several configurations of the PN7462AU Clif hardware.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR None of the configuration matches.
 */
phStatus_t phhalHw_PN7462AU_GetConfig(phhalHw_PN7462AU_DataParams_t * pDataParams,
    uint16_t wConfig, uint16_t* pValue)
{
    phStatus_t status = PH_ERR_SUCCESS;
    uint32_t dwValue = 0;

    switch(wConfig)
    {
    case PHHAL_HW_CONFIG_PARITY:
    {
        /* To get the parity enable or disable status */
        status = phhalRf_GetConfig(PHHAL_RF_CONFIG_PARITY, &dwValue);
        *pValue = (uint16_t) dwValue;
        pDataParams->wParity = (uint16_t) dwValue;
    }
    break;

    case PHHAL_HW_CONFIG_TXCRC:
    {
        /* To get the configured TX crc status */
        status = phhalRf_GetConfig(PHHAL_RF_CONFIG_TXCRC, &dwValue);
        *pValue = (uint16_t) dwValue;
    }
    break;

    case PHHAL_HW_CONFIG_RXCRC:
    {
        /* To get the configured RX crc status */
        status = phhalRf_GetConfig(PHHAL_RF_CONFIG_RXCRC, &dwValue);
        *pValue = (uint16_t) dwValue;
    }
    break;

    case PHHAL_HW_CONFIG_TXLASTBITS:
    {
        /* Getting the txlastbits value */
        *pValue = (uint16_t) pDataParams->bTxLastBits;
    }
    break;

    case PHHAL_HW_CONFIG_RXLASTBITS:
    {
        /* to get the valid number of bits received in last byte */
        *pValue = (uint16_t)(gphhalRf_RxParams.bRxBits);
    }
    break;

    case PHHAL_HW_CONFIG_RXWAIT_US:
    {
        /* getting the value of rx wait */
        status  = phhalRf_GetConfig(PHHAL_RF_CONFIG_RXWAIT_US, &dwValue);
        *pValue = (uint16_t)(dwValue);
    }
    break;

    case PHHAL_HW_CONFIG_TXDATARATE_FRAMING:
    {
        /* to get the tx data rate */
        *pValue = (uint16_t) pDataParams->wTxDataRate;
    }
    break;

    case PHHAL_HW_CONFIG_RXDATARATE_FRAMING:
    {
        /* to get the rx data rate */
        *pValue = (uint16_t) pDataParams->wRxDataRate;
    }
    break;

    case PHHAL_HW_CONFIG_ASK100:
    {
        /* to get the status of ask100% modulation ON/OFF status */
        *pValue = (uint16_t) pDataParams->bAsk100;
    }
    break;

    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_US:
    {
        /* to get the configured frame delay time value in us */
        if ((pDataParams->dwFdt) > 0xFFFFU)
        {
            status = PH_ERR_PARAMETER_OVERFLOW;
        }
        else
        {
            *pValue = (uint16_t) pDataParams->dwFdt;
        }
    }
    break;

    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS:
    {
        /* to get the configured frame delay time value in ms */
        *pValue = (uint16_t) (pDataParams->dwFdt/1000);
    }
    break;

    case PHHAL_HW_CONFIG_SUBCARRIER:
    {
        /* to get the configured rxsubcarrier value */
        *pValue = (uint16_t) pDataParams->wRxSubCarrier;
    }
    break;

    case PHHAL_HW_CONFIG_FIELD_OFF_TIME:
    {
        /* To get the value of configured field off time in us */
        *pValue = (uint16_t) pDataParams->wFieldOffTime;
    }
    break;

    case PHHAL_HW_CONFIG_FIELD_RECOVERY_TIME:
    {
        /* To get the value of configured field recovery time in us */
        *pValue = (uint16_t) pDataParams->wFieldRecoveryTime;
    }
    break;

    case PHHAL_HW_CONFIG_JEWEL_MODE:
    {
        /* To get the jewel mode enable or disable status */
        *pValue = (uint16_t) pDataParams->bFrameStep;

    }
    break;

    case PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1:
    {
        /*To get the MFC Disable Status */
        *pValue = (uint16_t) pDataParams->bMfcCryptoDisabled;
    }

    break;

    case PHHAL_HW_CONFIG_TXBUFFER_OFFSET:
    {
        /* to get the TX buffer offset value */
        *pValue = (uint16_t)(pDataParams->wAdditionalInfo);
    }
    break;

    case PHHAL_HW_CONFIG_RXBUFFER_STARTPOS:
    {
        /* to get the rx buffer startpos value */
        *pValue = (uint16_t) pDataParams->wRxBufStartPos;
    }
    break;

    case PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE:
    {
        /* to get the hal rx buffer size*/
        *pValue = (uint16_t) ((pDataParams->wRxBufSize) - (pDataParams->wRxBufStartPos)) ;
    }
    break;

    case PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE:
    {
        /* to get the hal tx buffer size*/
        *pValue = (uint16_t) pDataParams->wTxBufSize;
    }
    break;

    case  PHHAL_HW_CONFIG_TXBUFFER_LENGTH:
    {
        /* to get the length of transmitted buffer */
        *pValue = (uint16_t) pDataParams->wTxBufLen;
    }
    break;

    case PHHAL_HW_CONFIG_TXBUFFER:
    {
        /* to get the value at Tx buffer at offset position */
        *pValue = (uint16_t) (pDataParams->pTxBuffer[pDataParams->wAdditionalInfo]);
    }
    break;

    case PHHAL_HW_CONFIG_EXT_RF_ON:
    {
        /* To get the presence of external field */
        if (PH_ERR_SUCCESS == phhalRf_GetExtFieldStatus())
        {
            *pValue = (uint16_t) PH_ON;
        }
        else
        {
            *pValue = (uint16_t) PH_OFF;
        }
    }
    break;

    case PHHAL_HW_CONFIG_RXMULTIPLE:
    {
        /* to get whether rx multiple is on or off */
        *pValue = (uint16_t) pDataParams->bRxMultiple;
    }
    break;

    case PHHAL_HW_CONFIG_ACTIVEMODE:
    {
        /* to get whether active configuration is enabled or disabled */
        *pValue = (uint16_t) pDataParams->bActiveMode;
    }
    break;

    case PHHAL_HW_CONFIG_MFHALTED:
    {
        /* To get the state of card mode */
        status = phhalRf_GetConfig(PHHAL_RF_CONFIG_CMA_STATE, &dwValue);
        pDataParams->bPiccAState = (uint8_t)(dwValue);
        *pValue = pDataParams->bPiccAState;
    }
    break;

    case PHHAL_HW_CONFIG_TARGET_DATARATE_FRAMING:
    {
        /* to get the tx data rate value */
        *pValue = pDataParams->wTxDataRate;
    }
    break;

    case PHHAL_HW_CONFIG_OPE_MODE:
    {
        /* to get the ope mode */
        *pValue = pDataParams->wOpeMode ;
    }
    break;

    case PHHAL_HW_CONFIG_NFCIP_STARTBYTE:
    {
        /* gettingthe NFCIP StartByte */
        *pValue = (uint16_t) pDataParams->bNfcipMode ;
    }
    break;

    case PHHAL_HW_CONFIG_SET_EMD:
    {
        /* to get the status of EMd check */
        status = phhalRf_GetConfig(wConfig, &dwValue);
        *pValue = (uint16_t)dwValue;
    }
    break;

    case PHHAL_HW_CONFIG_MODINDEX:
    {
        /* Configure the modulation index */
        status = phhalRf_GetConfig(PHHAL_RF_CONFIG_MODINDEX, &dwValue);
        *pValue = (uint16_t)dwValue;
    }
    break;

    case PHHAL_HW_CONFIG_PN7462AU_LPCD_THRESHOLD:
    {
        /* to get AGC_CardThreshold Initial Detection Threshold of the AGC to de-tune*/
        *pValue = (uint16_t) pDataParams->dwLpcdThreshold;
    }
    break;

    case PHHAL_HW_CONFIG_PN7462AU_LPCD_REF_VALUE :
    {
        /* to get AGC_RefValue reference value for agc*/
        *pValue = (uint16_t) pDataParams->dwLpcdReference;
    }
    break;

    case PHHAL_HW_CONFIG_PN7462AU_LPCD_NEW_VALUE :
    {
        /* to get the new agc stored value */
        *pValue = (uint16_t)(pDataParams->dwLpcd_NewValue);
    }
    break;

    case PHHAL_HW_CONFIG_CARD_TYPE:
        *pValue = pDataParams->bCardType;
        break;

    case PHHAL_HW_CONFIG_INT_RF_ON:
        status = phhalRf_GetConfig((uint32_t)wConfig, &dwValue);
        *pValue = (uint16_t)dwValue;
        break;

    default:
        status = PH_ERR_UNSUPPORTED_PARAMETER;
        break;
    }
    status = status & PH_ERR_MASK;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}
/**
 * \brief This api is used to switch on the RF field
 * \description This api will turn on the RF field. Guard time after turning on the RF field is not taken care here.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR Operation failure.
 */
phStatus_t phhalHw_PN7462AU_FieldOn(phhalHw_PN7462AU_DataParams_t * pDataParams PH_UNUSED)
{
    phStatus_t status = PH_ERR_INTERNAL_ERROR;

    status = phhalRf_FieldOn();

    if (status != PH_ERR_SUCCESS)
    {
        status = PH_ERR_RF_ERROR;
    }

    return PH_ADD_COMPCODE(status, PH_COMP_HAL);

}

/**
 * \brief This api is used to switch off the RF field
 * \description This api will turn off the RF field.
 * \warning Rf off guard time has to be provided and it is not checked for the value here.
 *          Unambiguous values will result in unambiguous results
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR Operation failure.
 */
phStatus_t phhalHw_PN7462AU_FieldOff(phhalHw_PN7462AU_DataParams_t * pDataParams PH_UNUSED)
{
    phStatus_t status = (phStatus_t)PH_ERR_INTERNAL_ERROR;

    /* Disable the EMD */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_PN7462AU_SetConfig(pDataParams, PHHAL_HW_CONFIG_SET_EMD, PH_OFF));

    (void)phhalRf_FieldOff();

    return PH_ERR_SUCCESS;
}
/**
 * \brief This api is used to apply the load protocol settings depending on the technology A, B , FeliCa or GTM etc
 * \description This api is used to set the appropriate protocol requested. Please note that these protocols will be
 *              applied with the default bit rates.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR Operation failure.
 */
phStatus_t phhalHw_PN7462AU_ApplyProtocolSettings(phhalHw_PN7462AU_DataParams_t * pDataParams, uint8_t bCardType)
{
    phStatus_t status = (phStatus_t)(PH_ERR_INTERNAL_ERROR);

    /* Disable the EMD */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_PN7462AU_SetConfig(pDataParams, PHHAL_HW_CONFIG_SET_EMD, PH_OFF));

    /* Store new card type */
    if (bCardType == PHHAL_HW_CARDTYPE_CURRENT)
    {
        status = PH_ERR_SUCCESS;
    }
    else
    {
        pDataParams->bCardType = bCardType;
        pDataParams->bActiveMode = PH_OFF;
        pDataParams->wTargetMode = PH_OFF;

        switch(pDataParams->bCardType)
        {
        case PHHAL_HW_CARDTYPE_ISO14443A:
        {
            /* Call the load protocol for Type A with 106K baud rate */
            status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_A_106, E_PHHAL_RF_LP_I_RX_A_106_P);
            pDataParams->wTxDataRate = PHHAL_HW_RF_DATARATE_106 | PHHAL_HW_RF_TYPE_A_FRAMING ;
            pDataParams->wRxDataRate = PHHAL_HW_RF_DATARATE_106 | PHHAL_HW_RF_TYPE_A_FRAMING ;
        }
        break;

        case PHHAL_HW_CARDTYPE_ISO14443B:
        {
            /*Call the load protocol for Type B with 106K baud rate*/
            status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_B_106, E_PHHAL_RF_LP_I_RX_B_106);
            pDataParams->wTxDataRate = PHHAL_HW_RF_DATARATE_106 | PHHAL_HW_RF_TYPE_B_FRAMING ;
            pDataParams->wRxDataRate = PHHAL_HW_RF_DATARATE_106 | PHHAL_HW_RF_TYPE_B_FRAMING ;
        }
        break;
        case PHHAL_HW_CARDTYPE_FELICA_212:
        {
            /* Call the load protocol for Type FeliCa with 212K baud rate */
            status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_F_212, E_PHHAL_RF_LP_I_RX_F_212_P);
            pDataParams->wTxDataRate = PHHAL_HW_RF_DATARATE_212 | PHHAL_HW_RF_TYPE_F_FRAMING ;
            pDataParams->wRxDataRate = PHHAL_HW_RF_DATARATE_212 | PHHAL_HW_RF_TYPE_F_FRAMING ;

        }
        break;

        case PHHAL_HW_CARDTYPE_FELICA_424:
        {
            /* Call the load protocol for Type FeliCa with 424K baud rate */
            status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_F_424, E_PHHAL_RF_LP_I_RX_F_424_P);
            pDataParams->wTxDataRate = PHHAL_HW_RF_DATARATE_424 | PHHAL_HW_RF_TYPE_F_FRAMING ;
            pDataParams->wRxDataRate = PHHAL_HW_RF_DATARATE_424 | PHHAL_HW_RF_TYPE_F_FRAMING ;
        }
        break;

        case PHHAL_HW_CARDTYPE_ISO15693:
        {

            status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_15693_100, E_PHHAL_RF_LP_I_RX_15693_26);
            pDataParams->wTxDataRate = PHHAL_HW_RF_TX_DATARATE_1_OUT_OF_4;
            pDataParams->wRxDataRate = PHHAL_HW_RF_RX_DATARATE_HIGH;
            pDataParams->wRxSubCarrier = PHHAL_HW_SUBCARRIER_SINGLE;
        }
        break;
        case PHHAL_HW_CARDTYPE_I18000P3M3:
        {
            status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_EPCV2_TARI_9_44,
                                                    E_PHHAL_RF_LP_I_RX_EPCV2_SC424_2MP);
            if(status == PH_ERR_SUCCESS)
            {
                pDataParams->wRxSubCarrier = PHHAL_HW_SUBCARRIER_DUAL;
            }
        }
        break;

        case PHHAL_HW_CARDTYPE_I18092M_ACTIVE_106:
        {
            /* Call the load protocol for Active Initiator  with 106K baud rate */
            status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_ACT_106, E_PHHAL_RF_LP_I_RX_ACT_106);
            pDataParams->bActiveMode = PH_ON;
            pDataParams->wTxDataRate = PHHAL_HW_RF_DATARATE_106 | PHHAL_HW_RF_TYPE_ACTIVE_FRAMING ;
            pDataParams->wRxDataRate = PHHAL_HW_RF_DATARATE_106 | PHHAL_HW_RF_TYPE_ACTIVE_FRAMING ;
        }
        break;
        case PHHAL_HW_CARDTYPE_I18092M_ACTIVE_212:
        {
            /* Call the load protocol for Active Initiator with 212K baud rate */
            status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_ACT_212, E_PHHAL_RF_LP_I_RX_ACT_212);

            pDataParams->bActiveMode = PH_ON;

            pDataParams->wTxDataRate = PHHAL_HW_RF_DATARATE_212 | PHHAL_HW_RF_TYPE_ACTIVE_FRAMING ;
            pDataParams->wRxDataRate = PHHAL_HW_RF_DATARATE_212 | PHHAL_HW_RF_TYPE_ACTIVE_FRAMING ;
        }
        break;

        case PHHAL_HW_CARDTYPE_I18092M_ACTIVE_424:
        {
            /* Call the load protocol for Active Initiator with 424K baud rate */
            status = phhalRf_LoadProtocol_Initiator(E_PHHAL_RF_LP_I_TX_ACT_424, E_PHHAL_RF_LP_I_RX_ACT_424);
            pDataParams->bActiveMode = PH_ON;
            pDataParams->wTxDataRate = PHHAL_HW_RF_DATARATE_424 | PHHAL_HW_RF_TYPE_ACTIVE_FRAMING ;
            pDataParams->wRxDataRate = PHHAL_HW_RF_DATARATE_424 | PHHAL_HW_RF_TYPE_ACTIVE_FRAMING ;
        }
        break;

        default:
            break;
        }
    }

    if(status == PH_ERR_SUCCESS)
    {
        /* MIFARE Classic Crypto 1 state is disabled by default */
        status = phhalHw_PN7462AU_SetConfig(pDataParams, PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1, PH_ON);
    }

    status = status & PH_ERR_MASK;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}

/**
 * \brief This api is used to authenticate the MIFARE card
 * \description This api is very thin wrapper and directly calls the phhalRf MIFARE authenticate functionality.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR Operation failure.
 */
phStatus_t phhalHw_PN7462AU_MfcAuthenticate(
    phhalHw_PN7462AU_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint8_t * pKey,
    uint8_t * pUid
)
{
    phStatus_t status = PH_ERR_INTERNAL_ERROR;
    phhalRf_MFCParams_t phhalRf_MFCParams;

    if(PHPAL_MIFARE_KEYA == bKeyType)
    {
        phhalRf_MFCParams.bKeyCmd = (uint8_t) PHHAL_RF_MFC_KEYA_CMD;
    }
    else if(PHPAL_MIFARE_KEYB == bKeyType)
    {
        phhalRf_MFCParams.bKeyCmd = (uint8_t) PHHAL_RF_MFC_KEYB_CMD;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }
    phhalRf_MFCParams.bBlock = bBlockNo;
    phhalRf_MFCParams.pKey = pKey;
    phhalRf_MFCParams.pUID = pUid;
    phhalRf_MFCParams.bsizeUID = PHHAL_HW_SINGLESIZEUID;

    status = (phStatus_t)(phhalRf_PCD_ExchgMFC_Auth(pDataParams->pTxBuffer,
        &pDataParams->pRxBuffer[pDataParams->wRxBufStartPos],
        &phhalRf_MFCParams,
        &gphhalRf_RxParams));
    if(status == PH_ERR_SUCCESS)
    {
        pDataParams->bMfcCryptoDisabled = PH_OFF;
    }

    status = status & PH_ERR_MASK;

    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}

/**
 * \brief This api is used to authenticate the MIFARE product card
 * \description This api is very thin wrapper and directly calls the phhalRf MIFARE product authenticate functionality.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR Operation failure.
 */
phStatus_t phhalHw_PN7462AU_MfcAuthenticateKeyNo(
    phhalHw_PN7462AU_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNo,
    uint16_t wKeyVersion,
    uint8_t * pUid
    )
{
#ifdef NXPBUILD__PH_KEYSTORE
    phStatus_t   status;
    uint8_t      aKey[PHHAL_HW_MFC_KEY_LENGTH * 2U];
    uint8_t *    pKey = NULL;
    uint16_t     bKeystoreKeyType;

    /* Bail out if we haven't got a keystore */
    if (pDataParams->pKeyStoreDataParams == NULL)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_HAL);
    }

    /* retrieve KeyA & KeyB from keystore */
    status = phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVersion,
        sizeof(aKey),
        aKey,
        &bKeystoreKeyType);

    /* Change component code for invalid parameter */
    if ((status & PH_ERR_MASK) == PH_ERR_INVALID_PARAMETER)
    {
        return PH_ADD_COMPCODE((status & PH_ERR_MASK), PH_COMP_HAL);
    }
    else
    {
        PH_CHECK_SUCCESS(status);
    }

    /* check key type */
    if (bKeystoreKeyType != PH_KEYSTORE_KEY_TYPE_MIFARE)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Evaluate which key to use */
    if (bKeyType == PHHAL_HW_MFC_KEYA)
    {
        /* Use KeyA */
        pKey = aKey;
    }
    else if (bKeyType == PHHAL_HW_MFC_KEYB)
    {
        /* Use KeyB */
        pKey = &aKey[PHHAL_HW_MFC_KEY_LENGTH];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Call the authentication function using the key */
    /*Warning Fix : aKey to pKey*/
    return phhalHw_PN7462AU_MfcAuthenticate(pDataParams, bBlockNo, bKeyType, pKey, pUid);
#endif /* NXPBUILD__PH_KEYSTORE*/
#if !defined(NXPBUILD__PH_KEYSTORE)
    /* satisfy compiler */
    if (pDataParams || bBlockNo || bKeyType || wKeyNo || wKeyVersion || pUid);
    return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_HAL);
#endif /* NXPBUILD__PH_KEYSTORE*/
}

/**
 * \brief This api is used to set the Hw wait using the TXWAIT and RXWAIT
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_Wait(phhalHw_PN7462AU_DataParams_t *pDataParams PH_UNUSED, uint8_t bUnit, uint16_t wTimeout)
{
    uint32_t dwRdLibTimeout = 0x00;

    if(PHHAL_HW_TIME_MILLISECONDS == bUnit)
    {
        /* Convert the milisecond value to microsecond */
        dwRdLibTimeout = (1000u * (uint32_t)wTimeout);
        phUser_Wait(dwRdLibTimeout);
    }
    else if (PHHAL_HW_TIME_MICROSECONDS == bUnit)
    {
        dwRdLibTimeout = (uint32_t)(wTimeout);
        phUser_HwWait(dwRdLibTimeout);
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }


    return PH_ERR_SUCCESS;
}

/**
 * \brief This api is used to exchange a command with the counter part (Card or Reader)
 * \description This api is used to transmit the command and receives the response and returns the response
 *              to the reader library along with the status. Please note that each exchange requests from the
 *              reader library is sent to the phhalRf by calling the appropriate exchange api's.
 *              e.g for short frame, Anti Collision frame and standard frame etc.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR Operation failure.
 */
phStatus_t phhalHw_PN7462AU_Exchange(
    phhalHw_PN7462AU_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
)
{
    phStatus_t status = PH_ERR_INTERNAL_ERROR;
    gphhalRf_RxParams.wRxBytes = 0;

    if (wOption != PH_EXCHANGE_DEFAULT)
    {
        if (wOption == PH_EXCHANGE_BUFFER_FIRST)
        {
            phUser_MemCpy(pDataParams->pTxBuffer, pTxBuffer, wTxLength);
            pDataParams->wTxBufLen = wTxLength;
            return PH_ERR_SUCCESS;
        }
        else if ((wOption == PH_EXCHANGE_BUFFER_CONT) || (wOption == PH_EXCHANGE_BUFFER_LAST))
        {
            if (pDataParams->wTxBufLen == 0U)
                return PH_ADD_COMPCODE_FIXED(PH_ERR_FRAMING_ERROR, PH_COMP_HAL);

            else if ((pDataParams->wTxBufLen + wTxLength) > pDataParams->wTxBufSize)
                return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);

            else
            {
                if( (pTxBuffer != NULL) && (wTxLength != 0U))
                {
                    phUser_MemCpy(&pDataParams->pTxBuffer[pDataParams->wTxBufLen], pTxBuffer, wTxLength);
                    pDataParams->wTxBufLen += wTxLength;
                }

                if (wOption == PH_EXCHANGE_BUFFER_CONT)
                {
                    return PH_ERR_SUCCESS;
                }

            }

        }
        else
        {
            /* QL warning. */
        }

    }
    else
    {
        if ((pDataParams->wTxBufLen + wTxLength) > pDataParams->wTxBufSize)
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);

        if( (pTxBuffer != NULL) && (wTxLength != 0U))
        {
            pDataParams->wTxBufLen = wTxLength;
            phUser_MemCpy(pDataParams->pTxBuffer, pTxBuffer, wTxLength);
        }
    }




    if (0U != (pDataParams->bRxMultiple))
    {
        /* Exchange for Rx Multiple case in FeliCa */
        phUser_MemSet(&pDataParams->pRxBuffer[pDataParams->wRxBufStartPos], 0x00, pDataParams->wRxBufSize);

        status = phhalRf_PCD_ExchgRxMultiple(pDataParams->pTxBuffer,
            &pDataParams->pRxBuffer[pDataParams->wRxBufStartPos],
            pDataParams->wTxBufLen,
            &gphhalRf_RxParams);

    }

    else if(0U == (pDataParams->dwI15693Sof))
    {
        /* Exchange for Transmitting only EOF in ISO15693 case */
        status = phhalRf_PCD_Exchg_TxOnlyEOF(&pDataParams->pRxBuffer[pDataParams->wRxBufStartPos],
            &gphhalRf_RxParams
        );
    }
    else
    {
        /* Call normal PCD frame exchange */
        status = phhalRf_PCD_Exchg(pDataParams->pTxBuffer,
            &pDataParams->pRxBuffer[pDataParams->wRxBufStartPos],
            pDataParams->wTxBufLen,
            &gphhalRf_RxParams);
    }

    /* Resetting the TxLast Bits. */
    if(!(pDataParams->bFrameStep))
    {
        pDataParams->bTxLastBits = 0x00;
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 0x00);
    }

    status = status & PH_ERR_MASK;

    if(status == PH_ERR_SUCCESS)
    {
        if( gphhalRf_RxParams.wRxBytes > pDataParams->wRxBufSize)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
        }
    }

    if((status == PH_ERR_SUCCESS) || (status == PH_ERR_SUCCESS_INCOMPLETE_BYTE))
    {
        *ppRxBuffer = (uint8_t *)(&pDataParams->pRxBuffer[0]);

        *pRxLength = gphhalRf_RxParams.wRxBytes;

        if(0U != (pDataParams->wRxBufStartPos))
        {
            *pRxLength = *pRxLength + (pDataParams->wRxBufStartPos);
        }
    }
    /* considering the case for collision */
    else if (status == PH_ERR_COLLISION_ERROR)
    {
        *ppRxBuffer = (uint8_t *)(&pDataParams->pRxBuffer[0]);
        *pRxLength = gphhalRf_RxParams.wRxBytes;

        gphhalRf_RxParams.bRxBits = (uint8_t)(gphhalRf_RxParams.bRxCollPos + 8 - (*pRxLength * 8U));
        if(0U != (pDataParams->wRxBufStartPos))
        {
            *pRxLength = *pRxLength + (pDataParams->wRxBufStartPos);
        }
        if (gphhalRf_RxParams.bRxBits == 0U)
        {
            *pRxLength = *pRxLength - 1;
        }
    }
    else
    {
        *pRxLength = 0;
    }
    pDataParams->wRxBufLen = *pRxLength;
    pDataParams->wTxBufLen = 0x00;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}

#if PH_EXCHANGE_BUFFER_LAST != PH_EXCHANGE_LEAVE_BUFFER_BIT
#   error "The code below needs to be guarded if PH_EXCHANGE_LEAVE_BUFFER_BIT is not same as PH_EXCHANGE_BUFFER_LAST"
#endif

/**
 * \brief This api is used to tranmsit a command to the counter part (Card or Reader)
 * \description This api is used to transmit the command. Please note that each transmit requests from the
 *              reader library is sent to the phhalRf by calling the phhalRf_Transmit api.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR Operation failure.
 */
phStatus_t phhalHw_PN7462AU_Transmit(phhalHw_PN7462AU_DataParams_t * pDataParams, uint16_t wOption, uint8_t * pTxBuffer,
                                        uint16_t wTxLength)
{
    phStatus_t status = PH_ERR_INTERNAL_ERROR;
    /* Check options */
    if (wOption & (~(PH_TRANSMIT_BUFFERED_BIT | PH_TRANSMIT_LEAVE_BUFFER_BIT | PH_TRANSMIT_PREV_FRAME)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Re-send option should not be combined with buffer options. */
#if 0
    if ((0U != ((wOption & PH_TRANSMIT_PREV_FRAME))) && (wOption & (uint16_t)~(uint16_t)(PH_TRANSMIT_PREV_FRAME)))
#endif
        if ((0U != ((wOption & PH_TRANSMIT_PREV_FRAME))) && (wOption != PH_TRANSMIT_PREV_FRAME))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

    if (wOption == PH_TRANSMIT_PREV_FRAME)
    {
        pDataParams->wTxBufLen = wTxLength;
    }

    else if (wOption != PH_EXCHANGE_DEFAULT)
    {
        if (wOption == PH_EXCHANGE_BUFFER_FIRST)
        {
            phUser_MemCpy(pDataParams->pTxBuffer, pTxBuffer, wTxLength);
            pDataParams->wTxBufLen = wTxLength;
            return PH_ERR_SUCCESS;
        }
        else if ((wOption == PH_EXCHANGE_BUFFER_CONT) || (wOption == PH_EXCHANGE_BUFFER_LAST)
            /* || (wOption == PH_EXCHANGE_LEAVE_BUFFER_BIT) :
             * No need PH_EXCHANGE_BUFFER_LAST  == PH_EXCHANGE_LEAVE_BUFFER_BIT */
        )
        {
            if ((pDataParams->wTxBufLen + wTxLength) > pDataParams->wTxBufSize)
                return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);

            else
            {
                phUser_MemCpy(&pDataParams->pTxBuffer[pDataParams->wTxBufLen], pTxBuffer, wTxLength);
                pDataParams->wTxBufLen += wTxLength;

                if (wOption == PH_EXCHANGE_BUFFER_CONT)
                {
                    return PH_ERR_SUCCESS;
                }
            }

        }
        else
        {
            /* QL warning. */
        }
    }
    else
    {
        if ((pDataParams->wTxBufLen + wTxLength) > pDataParams->wTxBufSize)
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
        pDataParams->wTxBufLen = wTxLength;
        phUser_MemCpy(pDataParams->pTxBuffer, pTxBuffer, wTxLength);
    }

    status = phhalRf_Transmit(pDataParams->pTxBuffer,
        pDataParams->wTxBufLen
    );

    /* Resetting the TxLast Bits. */
    if(!(pDataParams->bFrameStep))
    {
        pDataParams->bTxLastBits = 0x00;
        (void)phhalRf_SetConfig(PHHAL_RF_CONFIG_TXLASTBITS, 0x00);
    }

    pDataParams->wTxBufLen = 0x00;
    status = status & PH_ERR_MASK;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}


/**
 * \brief This api is used to receive a command from the counter part (Card or Reader)
 * \description This api is used to transmit the command. Please note that each transmit requests from the
 *              reader library is sent to the phhalRf by calling the phhalRf_Receive api.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR Operation failure.
 */
phStatus_t phhalHw_PN7462AU_Receive(phhalHw_PN7462AU_DataParams_t * pDataParams, uint16_t wOption ,
                                    uint8_t ** ppRxBuffer, uint16_t *pRxLength)
{

    phStatus_t status = PH_ERR_INTERNAL_ERROR;
    uint8_t* pTmpBuffer = NULL;
    *pRxLength = 0;

    /* Check options */
    if (wOption != PH_RECEIVE_DEFAULT)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    status = phhalRf_Receive(&pTmpBuffer, &gphhalRf_RxParams);
    status = status & PH_ERR_MASK;

    if((status == PH_ERR_SUCCESS) || (status == PH_ERR_SUCCESS_INCOMPLETE_BYTE))
    {
        if( gphhalRf_RxParams.wRxBytes > pDataParams->wRxBufSize)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
        }
        *pRxLength = gphhalRf_RxParams.wRxBytes;
        phUser_MemCpy(&pDataParams->pRxBuffer[pDataParams->wRxBufStartPos], pTmpBuffer, *pRxLength );
        *ppRxBuffer = &pDataParams->pRxBuffer[0];

        if(0U != (pDataParams->wRxBufStartPos))
        {
            *pRxLength = *pRxLength + (pDataParams->wRxBufStartPos);
        }

    }

    /* considering the case for collision */
    else if (status == PH_ERR_COLLISION_ERROR)
    {
        if( gphhalRf_RxParams.wRxBytes > pDataParams->wRxBufSize)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
        }
        *pRxLength = gphhalRf_RxParams.wRxBytes;
        phUser_MemCpy(&pDataParams->pRxBuffer[pDataParams->wRxBufStartPos], pTmpBuffer, *pRxLength );
        *ppRxBuffer = &pDataParams->pRxBuffer[0];

        gphhalRf_RxParams.bRxBits = (uint8_t)(gphhalRf_RxParams.bRxCollPos + 8 - (*pRxLength * 8U));
        if(0U != (pDataParams->wRxBufStartPos))
        {
            *pRxLength = *pRxLength + (pDataParams->wRxBufStartPos);
        }
        if (gphhalRf_RxParams.bRxBits == 0U)
        {
            *pRxLength = *pRxLength - 1;
        }
    }
    else
    {
        /* To avoid the warning */
    }
    pDataParams->wRxBufLen = *pRxLength;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}

/**
 * \brief This api is used to perform the Autocoll
 * \description This function will perform the AutoColl(Target mode) and return the Technology Listened.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR Operation failure.
 */
phStatus_t phhalHw_PN7462AU_Autocoll(phhalHw_PN7462AU_DataParams_t * pDataParams, uint16_t wMode, uint8_t ** ppRxBuffer,
                                        uint16_t * pRxLength, uint16_t * pProtParams)
{
    phStatus_t status= PH_ERR_INTERNAL_ERROR;
    phhalRf_TargetActivatedParam_t sProtParams;
    uint8_t bLoop;

    /* Disable the EMD */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_PN7462AU_SetConfig(pDataParams, PHHAL_HW_CONFIG_SET_EMD, PH_OFF));

    do
    {
        bLoop = PH_OFF;
        pDataParams->wRxDataRate = 0x00;
        pDataParams->wTxDataRate = 0x00;

        status = phhalRf_AutoColl(&pDataParams->pAutocoll, pDataParams->bPiccAState, (phhalRf_Listen_Tech_t)(wMode),
                                     ppRxBuffer, pRxLength,  &sProtParams);
        status = status & PH_ERR_MASK;

        if (status == PH_ERR_SUCCESS)
        {
            /** SC3625 **/

            *(((uint8_t *)(pProtParams)) + 0U) = sProtParams.eBaudRateDetected;
            *(((uint8_t *)(pProtParams)) + 1U) = sProtParams.eProtocolDetected;

            /** SC3625 **/

            *pProtParams = *pProtParams & 0x00FFU;

            switch(sProtParams.eProtocolDetected)
            {
            case E_ISO14443_4A:
            {
                *pProtParams = *pProtParams | PHHAL_HW_RF_TYPE_A_FRAMING;
                pDataParams->bActiveMode = PH_OFF;
            }
            break;
            case E_ISO18092:
            {
                if(phhalRf_GetExtFieldStatus())
                {
                    *pProtParams = *pProtParams | PHHAL_HW_RF_TYPE_ACTIVE_FRAMING;
                    pDataParams->bActiveMode = PH_ON;
                }
                else
                {
                    if ((sProtParams.bCardModeStatus == PH_OFF) &&
                        (sProtParams.eBaudRateDetected == E_PH_HALRF_BR_106K))
                    {
                        bLoop = PH_ON;
                    }
                    else
                    {
                        if(sProtParams.eBaudRateDetected == E_PH_HALRF_BR_106K)
                        {
                            *pProtParams = *pProtParams | PHHAL_HW_RF_TYPE_A_FRAMING;
                        }
                        else
                        {
                            *pProtParams = *pProtParams | PHHAL_HW_RF_TYPE_F_FRAMING;
                        }
                        pDataParams->bActiveMode = PH_OFF;
                    }
                }
            }
            break;
            case E_TYPE_F:
            {
                *pProtParams = *pProtParams | PHHAL_HW_RF_TYPE_F_FRAMING;
            }
            break;
            case E_ISO14443B:
            {
                *pProtParams = *pProtParams | PHHAL_HW_RF_TYPE_B_FRAMING;
            }
            break;
            default:
                break;
            }

            (*pProtParams) = (*pProtParams) & (uint16_t)0xFF00U;
            switch(sProtParams.eBaudRateDetected)
            {
            case E_PH_HALRF_BR_106K:
            {
                *pProtParams = *pProtParams | PHHAL_HW_RF_DATARATE_106;
            }
            break;
            case E_PH_HALRF_BR_212K:
            {
                *pProtParams = *pProtParams | PHHAL_HW_RF_DATARATE_212;
            }
            break;
            case E_PH_HALRF_BR_424K:
            {
                *pProtParams = *pProtParams | PHHAL_HW_RF_DATARATE_424;
            }
            break;
            default:
                break;
            }

            if (*ppRxBuffer[0] == 0xF0U)
            {
                *ppRxBuffer = (uint8_t *)(*ppRxBuffer + 1U);
                if(((*pProtParams) & PHHAL_HW_RF_FRAMING_OPTION_MASK) == PHHAL_HW_RF_TYPE_ACTIVE_FRAMING)
                {
                    /* Active Communication. */
                    *pRxLength = *pRxLength - 3;
                }
                else
                {
                    *pRxLength = *pRxLength - 1;
                }
            }

            pDataParams->wTargetMode = PH_ON;
        }

        else
        {
            ;
        }
    }while(bLoop == PH_ON);

    pDataParams->wTxDataRate = *pProtParams;
    pDataParams->wRxDataRate = *pProtParams;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}


/**
 * \brief This api is used to set the Parameters required by Autocoll.
 * \description This function will set the parameter required to behave as target for Type A and Type F.
 *
 * @param[in] pSensRes Pointer containing the address of 2 byte sens response for ISO14443- Type A.
 * @param[in] pNfcid1 Pointer containing the address of  3 byte UID for ISO14443- Type A, first byte being 0x08.
 * @param[in] bSelRes, response to select i.e. sak for ISO14443- Type A.
 * @param[in] bSupportT3T,if set to 0 supports Nfc dep else Type 3 Tag for FeliCa.
 * @param[in] pNfcid2 Pointer containing the address of 6 byte NFCID2 for FeliCa.
 * @param[in] pPad Pointer containing the address of 8 pad Bytes for FeliCa.
 * @param[in] pSystemCode Pointer containing the address of 2 byte system code for FeliCa.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INTERNAL_ERROR Operation failure.
 */
phStatus_t phhalHw_PN7462AU_SetListenParam(phhalHw_PN7462AU_DataParams_t * pDataParams, uint8_t* pSensRes,
                                            uint8_t* pNfcid1, uint8_t bSelRes, uint8_t bSupportT3T, uint8_t* pNfcid2,
                                            uint8_t* pPad, uint8_t* pSystemCode)

{

    /* Either A OR F should have proper values. */
    if( !( ((uint32_t)pSensRes && (uint32_t)pNfcid1)
        || (((uint32_t)pNfcid2 && (uint32_t)pPad && (uint32_t)pSystemCode))) )
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    phUser_MemSet(&pDataParams->pAutocoll, 0x00, sizeof(pDataParams->pAutocoll));
    if(0U != (pSensRes))
    {
        phUser_MemCpy(&pDataParams->pAutocoll.pSenseRes, pSensRes, sizeof(pDataParams->pAutocoll.pSenseRes));
    }
    if(0U != (pNfcid1))
    {
        phUser_MemCpy(&pDataParams->pAutocoll.pNfcId1, pNfcid1, sizeof(pDataParams->pAutocoll.pNfcId1));
    }
    pDataParams->pAutocoll.bSelRes = bSelRes; /* sak*/
    if(0U != (bSupportT3T))
    {
        pDataParams->pAutocoll.bFProtSupp[0] = 0x02; /* Type 3 tag supported */
    }
    else
    {
        pDataParams->pAutocoll.bFProtSupp[0] = 0x01; /* NFC Dep supported */
    }
    pDataParams->pAutocoll.bFProtSupp[1] = 0xFE;

    if(0U != (pNfcid2))
    {
        phUser_MemCpy(&pDataParams->pAutocoll.pNfcId2, pNfcid2, sizeof(pDataParams->pAutocoll.pNfcId2));
    }
    if(0U != (pPad))
    {
        phUser_MemCpy(&pDataParams->pAutocoll.pPad, pPad, sizeof(pDataParams->pAutocoll.pPad));
    }
    if(0U != (pSystemCode))
    {
        phUser_MemCpy(&pDataParams->pAutocoll.pSystemCode, pSystemCode, sizeof(pDataParams->pAutocoll.pSystemCode));
    }

    return PH_ERR_SUCCESS;
}

/**
 *\brief Perform Low Power Card Detection.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_Lpcd(phhalHw_PN7462AU_DataParams_t * pDataParams)
{

    phStatus_t status= PH_ERR_USE_CONDITION;

    status = phhalRf_LPCD(pDataParams->dwLpcdThreshold, pDataParams->dwLpcdReference, &pDataParams->dwLpcd_NewValue);

    status = status & PH_ERR_MASK;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}


/**
 * \brief Performs RF-Reset.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_FieldReset (phhalHw_PN7462AU_DataParams_t * pDataParams)
{
    phStatus_t status = PH_ERR_INTERNAL_ERROR;

    /* Disable the EMD */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_PN7462AU_SetConfig(pDataParams, PHHAL_HW_CONFIG_SET_EMD, PH_OFF));

    status =  phhalRf_FieldReset(pDataParams->wFieldOffTime, pDataParams->wFieldRecoveryTime);

    if (status != PH_ERR_SUCCESS)
    {
        status = PH_ERR_RF_ERROR;
    }
    status = status & PH_ERR_MASK;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}

/**
 * \brief This api is used to abort the operation asynchronously
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_AsyncAbort(phhalHw_PN7462AU_DataParams_t *pDataParams)
{
    phStatus_t status = PH_ERR_INTERNAL_ERROR;
    status = phhalRf_SetIdleState(E_PH_HALRF_ENABLE);
    status = status & PH_ERR_MASK;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}

/**
 * \brief This api is used to deinitialise the hal rf component
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_DeInit(phhalHw_PN7462AU_DataParams_t *pDataParams PH_UNUSED)
{
    phStatus_t status = PH_ERR_SUCCESS;
    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}

/**
 * \brief This api is used to perform 18000p3m3 inventory
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_I18000p3m3Inventory(
    phhalHw_PN7462AU_DataParams_t  * pDataParams,
    uint8_t * pSelCmd,
    uint8_t bSelCmdLen,
    uint8_t bNumValidBitsinLastByte,
    uint8_t * pBeginRndCmd,
    uint8_t bTSprocessing,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxBufferLen
)
{
    phStatus_t  status;
    uint16_t    wNumSlotsDec;
    uint8_t     *pRxBuffer = NULL;
    uint16_t    wRxBufferLen = 0;
    uint8_t     bRxLastBits;
    uint8_t     bQ;
    uint8_t     bBeginSession;
    uint8_t     bSelTarget;

    /* Reset RX Buffer start position */
    pDataParams->wRxBufStartPos = 0;

    *ppRxBuffer = pDataParams->pRxBuffer;
    *pRxBufferLen = 0;

    /* Reset Current slot count (used for resume inventory) */
    pDataParams->wCurrentSlotCount = 0;

    if(pBeginRndCmd == NULL)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    if(pSelCmd == NULL)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    bBeginSession = ((uint8_t)pBeginRndCmd[1] & 0x30U) >> 4U;
    bSelTarget = ((uint8_t)pSelCmd[0] & 0x0EU) >>1U;

    /* If pSelCmd -> Target is SL then BeginRound for both S0 and S2 session is valid.
     * If pSelCmd -> Target = S0 then BeginRound -> Session should be S0.
     * If pSelCmd -> Target = S2 then BeginRound -> Session should be S2.
     * For other cases return invalid parameter.
     * */
    if(bSelTarget != PHHAL_HW_I18000P3M3_TARGET_SL)
    {
        if(((bBeginSession == PHHAL_HW_I18000P3M3_SESSION_S0)
            && (bSelTarget == PHHAL_HW_I18000P3M3_TARGET_INVENTORIED_S2))
            ||((bBeginSession == PHHAL_HW_I18000P3M3_SESSION_S2)
            && (bSelTarget == PHHAL_HW_I18000P3M3_TARGET_INVENTORIED_S0)))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
    }

    /* Extract bQ value from pBeginRndCmd */
    bQ = (uint8_t)((uint8_t)((uint8_t)pBeginRndCmd[1] & 0x7U) << 1U) | ((uint8_t)((uint8_t)pBeginRndCmd[2] & 0x80U) >> 7U);

    if(bTSprocessing != PHHAL_HW_I18000P3M3_GET_MAX_RESPS)
    {
        /* Perform SELECT */
        if((bSelCmdLen > 0U) && (pSelCmd != NULL))
        {
            status = phhalHw_PN7462AU_18000p3m3_Sw_Select(
                    pDataParams,
                    pSelCmd,
                    bSelCmdLen,
                    bNumValidBitsinLastByte);
        }
    }

    /* Perform BEGIN ROUND */
    status = phhalHw_PN7462AU_18000p3m3_Sw_BeginRound(pDataParams, pBeginRndCmd);

    if(bTSprocessing == PHHAL_HW_I18000P3M3_ONE_TS_ONLY)
    {
        pDataParams->wRxBufStartPos = 3;

        /* If get response in TS return success */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {

            /* Perform ACK */
            status = phhalHw_PN7462AU_18000p3m3_Sw_Ack(
                pDataParams,
                &pRxBuffer,
                &wRxBufferLen,
                &bRxLastBits);

            if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                /* prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = (uint8_t)wRxBufferLen;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = bRxLastBits;

                pDataParams->wRxBufStartPos += pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u];
            }
            else
            {
                /* prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = (uint8_t)status;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
            }
        }
        else if((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR)
        {
            /* prefix 3 control bytes for each response */
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 3;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
        }
        else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
        {
            /* prefix 3 control bytes for each response */
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 2;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
        }
        else
        {
            pDataParams->wRxBufStartPos = 0;
            return status;
        }

        *pRxBufferLen = pDataParams->wRxBufStartPos;
        pDataParams->wRxBufStartPos = 0;

        return status;
    }
    else if(bTSprocessing == PHHAL_HW_I18000P3M3_GET_MAX_RESPS)
    {
        wNumSlotsDec = (uint16_t)1 << bQ;

        while(0U != (wNumSlotsDec--))
        {
            /* Do not proceed if available buffer is not enough to accomodate further exchanges */
            if((pDataParams->wRxBufStartPos + 75U) > pDataParams->wRxBufSize)
            {
                pDataParams->wRxBufStartPos += 3U;

                /* Even if the slot was success, count it as collision as this slot is
                 not going to be entertained further */
                if (((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR) ||
                    ((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR) ||
                    ((status & PH_ERR_MASK) == PH_ERR_SUCCESS))
                {
                    /* Prefix 3 control bytes for each response */
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 3;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
                }
                else
                {
                    /* prefix 3 control bytes for each response */
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 2;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
                }

                /* Backup the relevent data for Resume Inventory */
                pDataParams->wCurrentSlotCount = wNumSlotsDec;
                *pRxBufferLen = pDataParams->wRxBufStartPos;
                pDataParams->wRxBufStartPos = 0;
                return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
            }

            pDataParams->wRxBufStartPos += 3U;

            /* Multiple labels found in current slot */
            if (((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR) ||
                ((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR))
            {
                /* Prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 3;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
            }
            else if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                /* Perform ACK */
                status = phhalHw_PN7462AU_18000p3m3_Sw_Ack(
                    pDataParams,
                    &pRxBuffer,
                    &wRxBufferLen,
                    &bRxLastBits);

                if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                {
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 0;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = (uint8_t)wRxBufferLen;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = bRxLastBits;

                    pDataParams->wRxBufStartPos += pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u];

                    /* Last next slot command to set the inventoried flag of last card successfully acked */
                    if (wNumSlotsDec == 0U)
                    {
                        /* Send CloseSlot command */
                        (void)phhalHw_PN7462AU_18000p3m3_Sw_NextSlot(pDataParams);
                    }

                }
                else if (((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR) ||
                         ((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR))
                {
                    /* Prefix 3 control bytes for each response */
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 3;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;

                    /* Wait for 1210us to avoid the colliding cards going out of the resolution loop */
                    (void)phhalHw_PN7462AU_Wait(pDataParams, PHHAL_HW_TIME_MICROSECONDS, 1210);
                }
                else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
                {
                    /* prefix 3 control bytes for each response */
                    /* Timeout as reply to ACK to be treated as collision as the card was there but not successfully counted */
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 3;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
                }
                else
                {
                    /* For all other error types, return error */
                    pDataParams->wRxBufStartPos = 0;
                    return status;
                }
            }
            else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
            {
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 2;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
            }
            else
            {
                /* For all other error types, return error */
                pDataParams->wRxBufStartPos = 0;
                return status;
            }

            /* Not the last slot -> Send NextSlot */
            if (wNumSlotsDec > 0U)
            {
                /* Send CloseSlot command */
                status = phhalHw_PN7462AU_18000p3m3_Sw_NextSlot(pDataParams);
            }
        }

        *pRxBufferLen = pDataParams->wRxBufStartPos;
        pDataParams->wRxBufStartPos = 0;

        return PH_ERR_SUCCESS;
    }
    else if(bTSprocessing == PHHAL_HW_I18000P3M3_GET_TAG_HANDLE)
    {
        pDataParams->wRxBufStartPos = 3;

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {

            /* Perform ACK */
            status = phhalHw_PN7462AU_18000p3m3_Sw_Ack(
                pDataParams,
                &pRxBuffer,
                &wRxBufferLen,
                &bRxLastBits);

            if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                /* prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = (uint8_t)wRxBufferLen;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = bRxLastBits;

                pDataParams->wRxBufStartPos += pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u];

                /* Send ReqRN for getting handle */
                status = phhalHw_PN7462AU_lI18000p3m3ReqRn(
                    pDataParams,
                    &pRxBuffer);

                if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                {
                    pDataParams->wRxBufStartPos += 2U;
                }
                else
                {
                    pDataParams->wRxBufStartPos = 0;
                    return status;
                }
            }
            else if (((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR) ||
                ((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR))
            {
                /* Prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 3;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
            }
            else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
            {
                /* prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 2;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
            }
            else
            {
                /* For all other error types, return error */
                pDataParams->wRxBufStartPos = 0;
                return status;
            }
        }
        else if((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR)
        {
            /* prefix 3 control bytes for each response */
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 3;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
        }
        else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
        {
            /* prefix 3 control bytes for each response */
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 2;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
        }
        else
        {
            pDataParams->wRxBufStartPos = 0;
            return status;
        }

        *pRxBufferLen = pDataParams->wRxBufStartPos;
        pDataParams->wRxBufStartPos = 0;

        return status;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }
}

/**
 * \brief This api is used to perform 18000p3m3 resume inventory
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_I18000p3m3ResumeInventory(
    phhalHw_PN7462AU_DataParams_t * pDataParams,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxBufferLen
)
{
    phStatus_t  status;
    uint8_t     *pRxBuffer = NULL;
    uint16_t    wRxBufferLen = 0;
    uint8_t     bRxLastBits;
    uint16_t    wNumSlotsDec;


    *ppRxBuffer = pDataParams->pRxBuffer;
    *pRxBufferLen = 0;

    /* Reset RX Buffer start position */
    pDataParams->wRxBufStartPos = 0;

    /* Check if current slot count is valid */
    if(0U != (pDataParams->wCurrentSlotCount))
    {
        wNumSlotsDec = pDataParams->wCurrentSlotCount;
        pDataParams->wCurrentSlotCount = 0;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_HAL);
    }

    /* Start with slot count, where Inventory stopped because of overflow */
    while(0U != (wNumSlotsDec--))
    {
        /* Check available RX buffer */
        if(pDataParams->wRxBufStartPos + 75 > pDataParams->wRxBufSize)
        {
            pDataParams->wCurrentSlotCount = wNumSlotsDec;
            *pRxBufferLen = pDataParams->wRxBufStartPos;
            pDataParams->wRxBufStartPos = 0;

            return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
        }

        /* Send NextSlot command */
        status = phhalHw_PN7462AU_18000p3m3_Sw_NextSlot(pDataParams);

        /* Increment buffer position to store control bytes for current slot */
        pDataParams->wRxBufStartPos += 3U;

        if (((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR) ||
            ((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR))
        {
            /* Prefix 3 control bytes for each response */
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 3;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
        }
        else if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Perform ACK */
            status = phhalHw_PN7462AU_18000p3m3_Sw_Ack(
                pDataParams,
                &pRxBuffer,
                &wRxBufferLen,
                &bRxLastBits);

            if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = (uint8_t)wRxBufferLen;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = bRxLastBits;

                pDataParams->wRxBufStartPos += pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u];

                /* Last next slot command to set the inventoried flag of last card successfully acked */
                if ((wNumSlotsDec == 0U) || ((pDataParams->wRxBufStartPos + 75U) > pDataParams->wRxBufSize))
                {
                    /* Send CloseSlot command */
                    (void)phhalHw_PN7462AU_18000p3m3_Sw_NextSlot(pDataParams);
                }

            }
            else if (((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR) ||
                     ((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR))
            {
                /* Prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 3;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;

                /* Wait for 1210us to avoid the colliding cards going out of the resolution loop */
                (void)phhalHw_PN7462AU_Wait(pDataParams, PHHAL_HW_TIME_MICROSECONDS, 1210);
            }
            else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
            {
                /* ACK timeout to be treated as collision as a card was present in vicinity */
                /* prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 3;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
            }
            else
            {
                /* For all other error types, return error */
                pDataParams->wRxBufStartPos = 0;
                return status;
            }
        }
        else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
        {
            /* prefix 3 control bytes for each response */
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3u] = 2;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2u] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1u] = 0;
        }
        else
        {
            /* For all other error types, return error */
            pDataParams->wRxBufStartPos = 0;
            return status;
        }
    }

    *pRxBufferLen = pDataParams->wRxBufStartPos;
    pDataParams->wRxBufStartPos = 0;

    return PH_ERR_SUCCESS;
}

/**
 * \brief This api is used to set Min Fdt value for MIFARE product
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phhalHw_PN7462AU_SetMinFDT(
    phhalHw_PN7462AU_DataParams_t * pDataParams,
    uint16_t wValue
    )
{
    phStatus_t  statusTmp = 0;
    uint16_t    wTimer = 0;
    uint16_t    wTxRate = 0;

    if (wValue == PH_ON)
    {
        /*Backup the old Timer values and set min FDT*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_PN7462AU_GetConfig(pDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, &wTimer));
        pDataParams->dwFdt = wTimer;
        /* Calculate values for Microsecond values */
        /* Get the data rate */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_PN7462AU_GetConfig(pDataParams,
            PHHAL_HW_CONFIG_TXDATARATE_FRAMING, &wTxRate));
        switch(wTxRate)
        {
        case PHHAL_HW_RF_DATARATE_106:
            wTimer = PHHAL_HW_MINFDT_106_US;
            break;
        case PHHAL_HW_RF_DATARATE_212:
            wTimer = PHHAL_HW_MINFDT_212_US;
            break;
        case PHHAL_HW_RF_DATARATE_424:
            wTimer = PHHAL_HW_MINFDT_424_US;
            break;
        case PHHAL_HW_RF_DATARATE_848:
            wTimer = PHHAL_HW_MINFDT_848_US;
            break;
        default:
            break;
        }
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_PN7462AU_SetConfig(pDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US, wTimer));
    }
    else if (wValue == PH_OFF)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_PN7462AU_SetConfig(pDataParams,
            (uint16_t)PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,(uint16_t)pDataParams->dwFdt));
    }
    else
    {
        /* Do nothing*/
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_PN7462AU_EventWait(phhalHw_PN7462AU_DataParams_t * pDataParams, uint32_t dwEventTimeout)
{
    return phhalRf_HalEventWaitAny(dwEventTimeout);
}

phStatus_t phhalHw_PN7462AU_EventConsume(phhalHw_PN7462AU_DataParams_t * pDataParams)
{
    return phhalRf_HalEventConsumeAll();
}

#endif /* NXPBUILD__PHHAL_HW_PN7462AU */
