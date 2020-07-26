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
* RC663 specific HAL-Component of Reader Library Framework.
* $Author: Ankur Srivastava (nxp79569) $
* $Revision: 6279 $ (v06.01.00)
* $Date: 2016-10-04 18:30:45 +0530 (Tue, 04 Oct 2016) $
*
* History:
*  CHu: Generated 12. January 2010
*
*/

#include <ph_Status.h>
#include <phDriver.h>
#include <phhalHw.h>
#include <ph_RefDefs.h>
#include <phNxpNfcRdLib_Config.h>

#ifdef NXPBUILD__PH_KEYSTORE
#include <phKeyStore.h>
#endif

#ifdef NXPBUILD__PHHAL_HW_RC663

#include <BoardSelection.h>

#include "phhalHw_Rc663.h"
#include "phhalHw_Rc663_Int.h"
#include "phhalHw_Rc663_Config.h"
#include <phhalHw_Rc663_Reg.h>
#include <phhalHw_Rc663_Cmd.h>
#include <phTools.h>

static const uint8_t bHalEventName[] = "Hal";

#define SET_RC663_SHADOW(USED_SHADOW)   \
    pShadowDefault = &(USED_SHADOW)[0][0]; \
    wShadowCount = (uint8_t)(sizeof(USED_SHADOW) / (sizeof(USED_SHADOW)[0]))

/** Default shadow for ISO14443-3A Mode */
static const uint16_t PH_MEMLOC_CONST_ROM wRc663_DefaultShadow_I14443a[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_ON},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_OFF},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_OFF},
    {PHHAL_HW_CONFIG_RXWAIT_US,             76},
    {PHHAL_HW_CONFIG_TXWAIT_US,             87},
    {PHHAL_HW_CONFIG_TXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_RXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,      PHHAL_HW_RC663_DEFAULT_TIMEOUT},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,      0x0000},
    {PHHAL_HW_CONFIG_ASK100,                PH_ON}
};

/** Default shadow for ISO14443-3A Mode during Emvco Mode*/
static const uint16_t PH_MEMLOC_CONST_ROM wRc663_Emvco_I14443a_DefaultShadow[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_ON},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_OFF},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_OFF},
    {PHHAL_HW_CONFIG_RXWAIT_US,             76},
    {PHHAL_HW_CONFIG_TXWAIT_US,             500},
    {PHHAL_HW_CONFIG_TXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_RXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,      PHHAL_HW_RC663_DEFAULT_TIMEOUT},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,      0x0000},
    {PHHAL_HW_CONFIG_ASK100,                PH_ON}
};

/** Default shadow for ISO14443-3B Mode */
static const uint16_t PH_MEMLOC_CONST_ROM wRc663_DefaultShadow_I14443b[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_OFF},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXWAIT_US,             76},
    {PHHAL_HW_CONFIG_TXWAIT_US,             85},
    {PHHAL_HW_CONFIG_TXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_RXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,      PHHAL_HW_RC663_DEFAULT_TIMEOUT},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,      0x0000},
    {PHHAL_HW_CONFIG_MODINDEX,              PHHAL_HW_RC663_MODINDEX_I14443B},
    {PHHAL_HW_CONFIG_ASK100,                PH_OFF}
};

/** Default shadow for ISO14443-3B Mode during Emvco Mode */
static const uint16_t PH_MEMLOC_CONST_ROM wRc663_Emvco_I14443b_DefaultShadow[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_OFF},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXWAIT_US,             76},
    {PHHAL_HW_CONFIG_TXWAIT_US,             500},
    {PHHAL_HW_CONFIG_TXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_RXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,      PHHAL_HW_RC663_DEFAULT_TIMEOUT},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,      0x0000},
    {PHHAL_HW_CONFIG_MODINDEX,              PHHAL_HW_RC663_MODINDEX_I14443B},
    {PHHAL_HW_CONFIG_ASK100,                PH_OFF}
};

/** Default shadow for ISO14443-3B Mode during NFC Mode to apply higher TxWait of 699micro-seconds by default
 *  as TxWait configuration doesn't apply after 1st Exchange but takes effect after 2nd Exchange. */
static const uint16_t PH_MEMLOC_CONST_ROM wRc663_NFC_I14443b_DefaultShadow[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_OFF},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXWAIT_US,             76},
    {PHHAL_HW_CONFIG_TXWAIT_US,             699},
    {PHHAL_HW_CONFIG_TXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_RXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_106},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,      PHHAL_HW_RC663_DEFAULT_TIMEOUT},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,      0x0000},
    {PHHAL_HW_CONFIG_MODINDEX,              PHHAL_HW_RC663_MODINDEX_I14443B},
    {PHHAL_HW_CONFIG_ASK100,                PH_OFF}
};

/** Default shadow for FeliCa Mode */
static const uint16_t PH_MEMLOC_CONST_ROM wRc663_DefaultShadow_Felica[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_OFF},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXWAIT_US,             15},
    {PHHAL_HW_CONFIG_TXWAIT_US,             0x0000},
    {PHHAL_HW_CONFIG_TXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_212},
    {PHHAL_HW_CONFIG_RXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_212},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,      PHHAL_HW_RC663_DEFAULT_TIMEOUT},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,      0x0000},
    {PHHAL_HW_CONFIG_MODINDEX,              PHHAL_HW_RC663_MODINDEX_FELICA},
    {PHHAL_HW_CONFIG_ASK100,                PH_OFF}
};

/** Shadow for 424 FeliCa Mode */
static const uint16_t PH_MEMLOC_CONST_ROM wRc663_DefaultShadow_Felica_424[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_OFF},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXWAIT_US,             8},
    {PHHAL_HW_CONFIG_TXWAIT_US,             0x0000},
    {PHHAL_HW_CONFIG_TXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_424},
    {PHHAL_HW_CONFIG_RXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_424},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,      PHHAL_HW_RC663_DEFAULT_TIMEOUT},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,      0x0000},
    {PHHAL_HW_CONFIG_MODINDEX,              PHHAL_HW_RC663_MODINDEX_FELICA},
    {PHHAL_HW_CONFIG_ASK100,                PH_OFF}
};

/** Default shadow for ISO15693 Mode */
static const uint16_t PH_MEMLOC_CONST_ROM wRc663_DefaultShadow_I15693[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_OFF},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXWAIT_US,             300},
    {PHHAL_HW_CONFIG_TXWAIT_US,             500},
    {PHHAL_HW_CONFIG_TXDATARATE_FRAMING,    PHHAL_HW_RF_TX_DATARATE_1_OUT_OF_4},
    {PHHAL_HW_CONFIG_RXDATARATE_FRAMING,    PHHAL_HW_RF_RX_DATARATE_HIGH},
    {PHHAL_HW_CONFIG_SUBCARRIER,            PHHAL_HW_SUBCARRIER_SINGLE},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,      PHHAL_HW_RC663_DEFAULT_TIMEOUT},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,      0x0000},
    {PHHAL_HW_CONFIG_MODINDEX,              PHHAL_HW_RC663_MODINDEX_I15693},
    {PHHAL_HW_CONFIG_ASK100,                PH_ON}
};

/** Default shadow for ICode EPC/UID Mode */
static const uint16_t PH_MEMLOC_CONST_ROM wRc663_DefaultShadow_EpcUid[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_OFF},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_OFF},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_OFF},
    {PHHAL_HW_CONFIG_RXWAIT_US,             227},
    {PHHAL_HW_CONFIG_TXWAIT_US,             302},
    {PHHAL_HW_CONFIG_TXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_EPCUID},
    {PHHAL_HW_CONFIG_RXDATARATE_FRAMING,    PHHAL_HW_RF_DATARATE_EPCUID},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,      PHHAL_HW_RC663_DEFAULT_TIMEOUT},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,      0x0000},
    {PHHAL_HW_CONFIG_MODINDEX,              PHHAL_HW_RC663_MODINDEX_EPCUID},
    {PHHAL_HW_CONFIG_ASK100,                PH_OFF}
};

/** Default shadow for ISO18000-3 Mode3 */
static const uint16_t PH_MEMLOC_CONST_ROM wRc663_DefaultShadow_I18000p3m3[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_OFF},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXWAIT_US,             70},
    {PHHAL_HW_CONFIG_TXWAIT_US,             150},
    {PHHAL_HW_CONFIG_TXDATARATE_FRAMING,    PHHAL_HW_RF_TX_DATARATE_I18000P3M3},
    {PHHAL_HW_CONFIG_RXDATARATE_FRAMING,    PHHAL_HW_RX_I18000P3M3_FL_423_MAN2},
    {PHHAL_HW_CONFIG_SUBCARRIER,            PHHAL_HW_SUBCARRIER_DUAL},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,      PHHAL_HW_RC663_DEFAULT_TIMEOUT},
    {PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,      0x0000},
    {PHHAL_HW_CONFIG_MODINDEX,              PHHAL_HW_RC663_MODINDEX_I18000P3M3},
    {PHHAL_HW_CONFIG_ASK100,                PH_OFF}
};

/** Default shadow for Type-A NFC DEP Communication */
static const uint16_t PH_MEMLOC_CONST_ROM wRc663_DefaultShadow_NfcDep[][2] =
{
    {PHHAL_HW_CONFIG_PARITY,                PH_ON},
    {PHHAL_HW_CONFIG_TXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXCRC,                 PH_ON},
    {PHHAL_HW_CONFIG_RXWAIT_US,             76},
    {PHHAL_HW_CONFIG_TXWAIT_US,             81},
    {PHHAL_HW_CONFIG_ASK100,                PH_ON}
};

static void phhalHw_Rc663_Reset(void);
static void phhalHw_Rc663_EventCallback(void * pDataParams);
static phStatus_t phhalHw_Rc663_ConfigTimeout(phhalHw_Rc663_DataParams_t * pDataParams, uint16_t wConfig, uint16_t wValue);
static phStatus_t phhalHw_Rc663_TransmitJewelCmd(
                                                 phhalHw_Rc663_DataParams_t * pDataParams,
                                                 uint16_t wJewelBufferLen,
                                                 uint8_t * pTmpBuffer
                                                 );

static void phhalHw_Rc663_EventCallback(void * pDataParams)
{
#ifndef _WIN32
    phhalHw_Rc663_DataParams_t * pRc663DataParams = NULL;

    pRc663DataParams = (phhalHw_Rc663_DataParams_t*) pDataParams;
    /* Post Abort Event. */
    (void)phOsal_EventPost(&pRc663DataParams->HwEventObj.EventHandle, E_OS_EVENT_OPT_POST_ISR, E_PH_OSAL_EVT_RF, NULL);
#else /* _WIN32 */
    /* satisfy compiler */
    if (0U != (pDataParams));
#endif /* _WIN32 */
}

static phStatus_t phhalHw_Rc663_ConfigTimeout(phhalHw_Rc663_DataParams_t * pDataParams, uint16_t wConfig, uint16_t wValue)
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint8_t    PH_MEMLOC_REM bRegister;

    /* If a Setconfig(PHHAL_HW_CONFIG_POLL_GUARD_TIME_US) is done then configuring of this timeout
     * will be done in phhalHw_Exchange() API. */
    if (pDataParams->bPollGuardTimeFlag == PH_OFF)
    {
        /*
         * Timeout is inaccurate if applyprotocolsettings is not done previously.
         * T0CONTROL should be set to 0x08 - meaning after running down to zero, it will restart.
         * T1CONTROL should be set to 0x02 - meaning The timer input clock is an underflow of Timer0
         */
        if (pDataParams->bCardType == PHHAL_HW_CARDTYPE_UNKNOWN)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_T0CONTROL, &bRegister));
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T0CONTROL, bRegister | PHHAL_HW_RC663_BIT_TAUTORESTARTED | PHHAL_HW_RC663_VALUE_TCLK_1356_MHZ));

            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_T1CONTROL, &bRegister));
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T1CONTROL, bRegister | PHHAL_HW_RC663_VALUE_TCLK_T0));
        }

        /* Calculate values for Microsecond values */
        if (wConfig == PHHAL_HW_CONFIG_TIMEOUT_VALUE_US)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetFdt(pDataParams, PHHAL_HW_TIME_MICROSECONDS, wValue));
            pDataParams->bTimeoutUnit = PHHAL_HW_TIME_MICROSECONDS;
        }
        /* Calculate values for Millisecond values */
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetFdt(pDataParams, PHHAL_HW_TIME_MILLISECONDS, wValue));
            pDataParams->bTimeoutUnit = PHHAL_HW_TIME_MILLISECONDS;
        }
    }
    else
    {
        /* Update dataparams with input units and time, which will be set in phhalHw_Exchange() API. */
        if (wConfig == PHHAL_HW_CONFIG_TIMEOUT_VALUE_US)
        {
            pDataParams->bTimeoutUnit = PHHAL_HW_TIME_MICROSECONDS;
        }
        else
        {
            pDataParams->bTimeoutUnit = PHHAL_HW_TIME_MILLISECONDS;
        }
    }
    /* Write configuration data into shadow */
    pDataParams->wCfgShadow[wConfig] = wValue;

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_Init(
                              phhalHw_Rc663_DataParams_t * pDataParams,
                              uint16_t wSizeOfDataParams,
                              void * pBalDataParams,
                              uint8_t * pLoadRegConfig,
                              uint8_t * pTxBuffer,
                              uint16_t wTxBufSize,
                              uint8_t * pRxBuffer,
                              uint16_t wRxBufSize
                              )
{
    phStatus_t status;
    uint8_t bIrqType;

    if (sizeof(phhalHw_Rc663_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
    }
    if ((wTxBufSize == 0U) || (wRxBufSize == 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pBalDataParams);
    PH_ASSERT_NULL (pTxBuffer);
    PH_ASSERT_NULL (pRxBuffer);


#ifndef _WIN32
    phbalReg_Type_t * pBalParams = (phbalReg_Type_t *) pBalDataParams;
    phDriver_Pin_Config_t pinCfg;
#endif

    status = PH_ERR_SUCCESS;

    /* We need to reserve 1 byte at the beginning of the buffers for SPI operation */
    ++pTxBuffer;
    --wTxBufSize;
    ++pRxBuffer;
    --wRxBufSize;

    /* Init. private data */
    pDataParams->wId                    = PH_COMP_HAL | PHHAL_HW_RC663_ID;
    pDataParams->pBalDataParams         = pBalDataParams;
    pDataParams->bLoadRegMode           = PH_ON;
    pDataParams->pLoadRegConfig         = pLoadRegConfig;
    pDataParams->pTxBuffer              = pTxBuffer;
    pDataParams->wTxBufSize             = wTxBufSize;
    pDataParams->wTxBufLen              = 0;
    pDataParams->pRxBuffer              = pRxBuffer;
    pDataParams->wRxBufSize             = wRxBufSize;
    pDataParams->wRxBufLen              = 0;
    pDataParams->wRxBufStartPos         = 0;
    pDataParams->bCardType              = PHHAL_HW_CARDTYPE_UNKNOWN;
    pDataParams->wTimingMode            = PHHAL_HW_TIMING_MODE_OFF;
    pDataParams->bTimeoutUnit           = PHHAL_HW_TIME_MICROSECONDS;
    pDataParams->dwTimingUs             = 0;
    pDataParams->wFieldOffTime          = PHHAL_HW_FIELD_OFF_DEFAULT;
    pDataParams->wFieldRecoveryTime     = PHHAL_HW_FIELD_RECOVERY_DEFAULT;
    pDataParams->bSymbolStart           = PH_OFF;
    pDataParams->bSymbolEnd             = PH_OFF;
    pDataParams->bFifoSize              = PH_NXPNFCRDLIB_CONFIG_RC663_DEFAULT_FIFOSIZE;
    pDataParams->wMaxPrecachedBytes     = PH_NXPNFCRDLIB_CONFIG_RC663_DEFAULT_PRECACHED_BYTES;
    pDataParams->wAdditionalInfo        = 0;
    pDataParams->bRfResetAfterTo        = PH_OFF;
    pDataParams->bMode                  = PHHAL_HW_RC663_CMD_LPCD_MODE_DEFAULT;
    pDataParams->bCRC5Enabled           = PH_OFF;
    pDataParams->bOpeMode               = RD_LIB_MODE_NFC;
    pDataParams->bRxMultiple            = PH_OFF;
    pDataParams->bNfcipMode             = PH_OFF;
    pDataParams->bCheckEmdErr           = PH_OFF;
    pDataParams->bPollGuardTimeFlag     = PH_OFF;
    pDataParams->wCurrentSlotCount      = 0;
    pDataParams->bSession               = 0;
    pDataParams->b18000p3m3Q            = 0;
    pDataParams->pRFISRCallback         = &phhalHw_Rc663_EventCallback;
    pDataParams->wDerivative            = 0;
    /* Disable Charge Pump and Filter By Default. */
    pDataParams->bLpcdOption            = 0;

    /* Reset sub-carrier setting, needed for phhalHw_Rc663_SetCardMode */
    pDataParams->wCfgShadow[PHHAL_HW_CONFIG_SUBCARRIER] = PHHAL_HW_SUBCARRIER_SINGLE;


#ifndef _WIN32

    /* Config Reset pin as output and set the default voltage level. */
    pinCfg.bPullSelect = PHDRIVER_PIN_RESET_PULL_CFG;
    pinCfg.bOutputLogic = RESET_POWERUP_LEVEL;
    PH_CHECK_SUCCESS_FCT(status, phDriver_PinConfig(PHDRIVER_PIN_RESET, PH_DRIVER_PINFUNC_OUTPUT, &pinCfg));

    if ((pBalParams->bBalType == PHBAL_REG_TYPE_SPI) ||
        (pBalParams->bBalType == PHBAL_REG_TYPE_KERNEL_SPI) ||
        (pBalParams->bBalType == PHBAL_REG_TYPE_USER_SPI))
    {
        pDataParams->bBalConnectionType = PHHAL_HW_BAL_CONNECTION_SPI;

        if (pBalParams->bBalType == PHBAL_REG_TYPE_SPI)
        {
            /* Config NSS (Chip Select) pin as GPIO output and set to high. */
            pinCfg.bPullSelect = PHDRIVER_PIN_NSS_PULL_CFG;
            pinCfg.bOutputLogic = PH_DRIVER_SET_HIGH;
            PH_CHECK_SUCCESS_FCT(status, phDriver_PinConfig(PHDRIVER_PIN_SSEL, PH_DRIVER_PINFUNC_OUTPUT, &pinCfg));
        }
    }
    else if (pBalParams->bBalType == PHBAL_REG_TYPE_I2C)
    {
        pDataParams->bBalConnectionType = PHHAL_HW_BAL_CONNECTION_I2C;
    }
    else if (pBalParams->bBalType == PHBAL_REG_TYPE_SERIAL_WIN)
    {
        pDataParams->bBalConnectionType = PHHAL_HW_BAL_CONNECTION_RS232;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Reset the front end after configuring the interface select pins. */
    phhalHw_Rc663_Reset();
#endif /* _WIN32 */

#ifndef NXPBUILD__PH_PLATFORM_DELEGATE
    if(PIN_IRQ_TRIGGER_TYPE == PH_DRIVER_INTERRUPT_RISINGEDGE)
    {
        bIrqType = PH_HALHW_RC663_RAISING_EDGE_TRIGGER;
    }
    else if(PIN_IRQ_TRIGGER_TYPE == PH_DRIVER_INTERRUPT_FALLINGEDGE)
    {
        bIrqType = PH_HALHW_RC663_FALLING_EDGE_TRIGGER;
    }
#endif /* NXPBUILD__PH_PLATFORM_DELEGATE */
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Set irq0en (08h) register bit 8 to either 0U or 1 to configure interrupt as falling edge (active low)
     * or raising edge (active high). Interrupt configuration type must be changed in phNxpNfcRdLib_Config.h file.
     * */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_WriteRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_IRQ0EN,
        bIrqType
        ));

#ifndef _WIN32
    /* Create the event. */
    pDataParams->HwEventObj.pEvtName = (uint8_t *)&bHalEventName[0];
    pDataParams->HwEventObj.intialValue = 0;
    status = phOsal_EventCreate(&pDataParams->HwEventObj.EventHandle, &pDataParams->HwEventObj);
#endif

    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}

phStatus_t phhalHw_Rc663_Exchange(
                                  phhalHw_Rc663_DataParams_t * pDataParams,
                                  uint16_t wOption,
                                  uint8_t * pTxBuffer,
                                  uint16_t wTxLength,
                                  uint8_t ** ppRxBuffer,
                                  uint16_t * pRxLength
                                  )
{
    uint8_t *   PH_MEMLOC_REM pTmpBuffer = NULL;
    uint32_t    PH_MEMLOC_REM dwTimingSingle = 0;
    phStatus_t  PH_MEMLOC_REM status = PH_ERR_SUCCESS;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wNumPrecachedBytes;
    uint16_t    PH_MEMLOC_REM wFifoBytes;
    uint16_t    PH_MEMLOC_REM wTmpBufferLen = 0;
    uint16_t    PH_MEMLOC_REM wTmpBufferSize;
    uint16_t    PH_MEMLOC_REM wIndex = 0;
    uint16_t    PH_MEMLOC_REM wSBIndex = 0;
    uint16_t    PH_MEMLOC_REM wVerifyRxCrc5 = 0;
    uint8_t     PH_MEMLOC_REM aCrc[2];
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor_tmp;
    uint8_t     PH_MEMLOC_REM bIrq1WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq0Reg = 0x00;
    uint8_t     PH_MEMLOC_REM bIrq1Reg = 0x00;
    uint8_t     PH_MEMLOC_REM bRegister;
    uint8_t     PH_MEMLOC_REM bNoData = PH_OFF;
    uint8_t     PH_MEMLOC_REM bBitlen;
    uint8_t     PH_MEMLOC_REM bSbBackup = 0;
    uint32_t    PH_MEMLOC_REM dwMultiReg;
    uint32_t    PH_MEMLOC_REM dwSaveReg;

    /* Initializing Variable used in EMVCo Mode */
    dwSaveReg      = 0x00;
    dwMultiReg     = 0x00;
    wTmpBufferLen  = 0x00;
    bIrq0WaitFor   = 0x00;

    do
    {
        /* Check options */
        if (0U != (wOption & (uint16_t)~(uint16_t)(PH_EXCHANGE_BUFFERED_BIT | PH_EXCHANGE_LEAVE_BUFFER_BIT)))
        {
            statusTmp = (PH_ERR_INVALID_PARAMETER | PH_COMP_HAL);
            break;
        }

        /* clear internal buffer if requested */
        if (0U == (wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
        {
            pDataParams->wTxBufLen = 0;
        }

        /* set the receive length */
        if (pRxLength != NULL)
        {
            *pRxLength = 0;
        }

        /* Prepend Start Byte (0xF0) in Passive 106kbps during NFCIP exchange. */
        if ((pDataParams->bNfcipMode == PH_ON) &&
            (0U == (wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT)))
        {
            /* Read the Shadow and Card Type if it configured at 106kbps Type-A */
            if ((pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING] == PHHAL_HW_RF_DATARATE_106) &&
                (pDataParams->bCardType == PHHAL_HW_CARDTYPE_ISO14443A))
            {
                /* retrieve transmit buffer */
                PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_GetTxBuffer(pDataParams, PH_ON, &pTmpBuffer, &wTmpBufferLen, &wTmpBufferSize));

                /* Prepend Start Byte 0xF0 in case of Passive Communication at 106kbps BaudRate */
                (void)memset(&pTmpBuffer[wTmpBufferLen], 0xF0, 0x01);
                pDataParams->wTxBufLen++;
            }
        }

        /* Fill the global TxBuffer */
        /* Note: We always need to buffer for SPI, else the input buffer would get overwritten! */
        if ((0U != (wOption & PH_EXCHANGE_BUFFERED_BIT)) ||
            (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_SPI) ||
            (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_I2C))
        {
            /* retrieve transmit buffer */
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_GetTxBuffer(pDataParams, PH_ON, &pTmpBuffer, &wTmpBufferLen, &wTmpBufferSize));

            if (wTxLength != 0U)
            {
                /* TxBuffer overflow check */
                if (wTxLength > (wTmpBufferSize - wTmpBufferLen))
                {
                    pDataParams->wTxBufLen = 0;
                    statusTmp = (PH_ERR_BUFFER_OVERFLOW | PH_COMP_HAL);
                    break;
                }

                /* copy data */
                (void)memcpy(&pTmpBuffer[wTmpBufferLen], pTxBuffer, wTxLength);
                pDataParams->wTxBufLen = pDataParams->wTxBufLen + wTxLength;
            }

            /* Buffer operation -> finished */
            if (0U != (wOption & PH_EXCHANGE_BUFFERED_BIT))
            {
                return PH_ERR_SUCCESS;
            }
            /* Else reset the input length for SPI and continue */
            else
            {
                wTxLength = 0;
            }
        }

        /* retrieve transmit buffer */
        PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_GetTxBuffer(pDataParams, PH_ON, &pTmpBuffer, &wTmpBufferLen, &wTmpBufferSize));

        if(pDataParams->bJewelActivated == PH_ON)
        {
            /* if data is sent through serial port(UART), copy it to pTmpBuffer */
            if((0U == (wOption & PH_EXCHANGE_BUFFERED_BIT)) &&
                (pDataParams->bBalConnectionType != PHHAL_HW_BAL_CONNECTION_SPI) &&
                (pDataParams->bBalConnectionType != PHHAL_HW_BAL_CONNECTION_I2C))
            {
                /* Additional 2 bytes are required for CRC. */
                if((wTxLength + 2U) <= wTmpBufferSize)
                {
                    (void)memcpy(pTmpBuffer, pTxBuffer, wTxLength);
                    wTmpBufferLen = wTxLength;
                    wTxLength = 0;
                }
                else
                {
                    statusTmp = (PH_ERR_BUFFER_OVERFLOW | PH_COMP_HAL);
                    break;
                }
            }

            /* Compute Crc_B for the data to be transmitted */
            PH_CHECK_FAILURE_FCT(statusTmp, phTools_ComputeCrc_B(pTmpBuffer, wTmpBufferLen, aCrc));

            /* Copy CRC bytes to Tx Buffer. */
            (void)memcpy(&pTmpBuffer[wTmpBufferLen], aCrc, 2);

            /* Update the Jewel Tx length by 2U to incluse CRC. */
            wTmpBufferLen = wTmpBufferLen + 2U;

            /* Turn OFF the parity, by default shadow it is ON */
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_PARITY, PH_OFF));

            /* Make sure CRC is OFF */
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

            /* First Jewel byte to be transmitted is with 7 bits. */
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXLASTBITS, 7));

            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXWAIT_US, 500));
        }

        /* check for TX CRC enable or not ?? */
        if(0U != (pDataParams->bCRC5Enabled))
        {
            /* if data is sent through serial port(UART), copy it to pTmpBuffer */
            if((0U == (wOption & PH_EXCHANGE_BUFFERED_BIT)) &&
                (pDataParams->bBalConnectionType != PHHAL_HW_BAL_CONNECTION_SPI) &&
                (pDataParams->bBalConnectionType != PHHAL_HW_BAL_CONNECTION_I2C))
            {
                /* additional 2 bytes are required for CRC*/
                if((wTxLength + 2U) <= wTmpBufferSize)
                {
                    (void)memcpy(pTmpBuffer, pTxBuffer, wTxLength);
                    wTmpBufferLen = wTxLength;
                    wTxLength = 0;
                }
                else
                {
                    statusTmp = (PH_ERR_BUFFER_OVERFLOW | PH_COMP_HAL);
                    break;
                }
            }

            /* Store CRC flag values
             * Since we need to perform software CRC calculation by disabling hardware CRC calculation
             */
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_GetConfig(pDataParams, PHHAL_HW_CONFIG_TXCRC, &wIndex));
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_GetConfig(pDataParams, PHHAL_HW_CONFIG_RXCRC, &wVerifyRxCrc5));

            /* Make sure CRC is OFF */
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

            /* If Tx CRC5 is enable */
            if(0U != (wIndex))
            {
                /* Read Tx Last bit value*/
                PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_GetConfig(pDataParams, PHHAL_HW_CONFIG_TXLASTBITS, &wIndex));

                /* check condition is added to fix only QAC. Always wTmpBufferLen will be greater the 1 */
                if(wTmpBufferLen > 1U)
                {
                    /* Get bit length */
                    wIndex = ((wIndex > 0U) ? (((uint16_t)(wTmpBufferLen - 1U) << 3U) + wIndex) : ((uint16_t)(wTmpBufferLen << 3U)));
                }

                /* Perform CRC-5 calculation */
                PH_CHECK_FAILURE_FCT(statusTmp, phTools_CalculateCrc5(
                    PH_TOOLS_CRC_OPTION_MSB_FIRST | PH_TOOLS_CRC_OPTION_BITWISE,
                    PH_TOOLS_CRC5_PRESET_I18000P3,
                    PH_TOOLS_CRC5_POLY_I18000P3,
                    pTmpBuffer,
                    wIndex,
                    &aCrc[0]));

                /* bBitLength becomes TxLastBits */
                bBitlen = (uint8_t)(wIndex & 0x07U);

                /* Complete last byte -> map to 8 bits for the formula below to work */
                if (bBitlen == 0U)
                {
                    bBitlen = 8;
                }

                /* Merge CRC-5 into last byte */
                pTmpBuffer[wTmpBufferLen - 1U] = (uint8_t)(pTmpBuffer[wTmpBufferLen - 1U] & (uint8_t)(0xFFU << (uint8_t)(8U - bBitlen)));
                pTmpBuffer[wTmpBufferLen - 1U] |= (aCrc[0] << 3U) >> bBitlen;

                /* Add the CRC length to TxLastBits */
                bBitlen = bBitlen + 5U;

                /* Additional byte needed to store the rest of the CRC */
                if (bBitlen > 8U)
                {
                    bBitlen &= 0x07U;
                    pTmpBuffer[wTmpBufferLen] = (uint8_t)(aCrc[0] << (uint8_t)(5U - bBitlen));
                    wTmpBufferLen++;
                }
                else
                {
                    bBitlen &= 0x07U;
                }

                /*  last Tx-Byte. */
                PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXLASTBITS, bBitlen));
            }
        }
    }while(FALSE);

    /* If any error has been reported previously, then stop poll guard timer if it is ON. */
    if (statusTmp == PH_ERR_SUCCESS)
    {
        /* Break out for any error in this loop. */
        do
        {
            /* Disable DataEn if no data is to be sent */
            if ((wTmpBufferLen == 0U) &&
                (wTxLength == 0U))
            {
                /* Store that we sent no data */
                bNoData = PH_ON;

                /* Read out TxDataNum register */
                PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TXDATANUM, &bRegister));

                /* Disable DataEn */
                bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_DATAEN;

                /* Write changed register */
                PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXDATANUM, bRegister));
            }
            else
            {
                /* Reset NoData flag */
                bNoData = PH_OFF;
            }

            if(pDataParams->bJewelActivated == PH_ON)
            {
                /* Transmit all the Jewel Tx byte except last byte. */
                PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_TransmitJewelCmd(pDataParams, wTmpBufferLen, pTmpBuffer));
                /* Update the Tmp Tx Buffer Length to 1, to transmit last byte of Jewel Command. */
                pTmpBuffer += wTmpBufferLen - 1U;
                wTmpBufferLen = 1;

                /* Retrieve content of Timer Control register */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, &bRegister));

                /* Manually stop timers T0 and T1 used for FDT/FWT. */
                bRegister |= PHHAL_HW_RC663_BIT_T0STARTSTOPNOW | PHHAL_HW_RC663_BIT_T1STARTSTOPNOW;
                bRegister &= (uint8_t)~(uint8_t)(PHHAL_HW_RC663_BIT_T0RUNNING | PHHAL_HW_RC663_BIT_T1RUNNING);
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, bRegister));
            }

            /* Terminate a possibly running command */
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_IDLE));

            /* Flush FiFo */
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));

            /* clear all IRQ0 flags */
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                pDataParams,
                PHHAL_HW_RC663_REG_IRQ0,
                (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_SET));

            /* clear all IRQ1 flags */
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                pDataParams,
                PHHAL_HW_RC663_REG_IRQ1,
                (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_SET));

            /* Enable the LOW ALERT IRQ now. */
            /* If the Number of Bytes to send is greater than wMaxPrecachedBytes, then enable the LOW_ALERT irq. */
            if (wTmpBufferLen > pDataParams->wMaxPrecachedBytes)
            {
                bIrq0WaitFor = PHHAL_HW_RC663_BIT_LOALERTIRQ;
            }

            /* Precache buffered contents into FIFO */
            wNumPrecachedBytes = (wTmpBufferLen > pDataParams->wMaxPrecachedBytes) ? pDataParams->wMaxPrecachedBytes : wTmpBufferLen;
            if (wNumPrecachedBytes > 0U)
            {
                /* Perform actual write to FIFO */
                status = phhalHw_Rc663_WriteFifo(pDataParams, pTmpBuffer, wNumPrecachedBytes, &wFifoBytes);

                /* Ignore FiFo overflow warning */
                if ((status & PH_ERR_MASK) != PH_ERR_BUFFER_OVERFLOW)
                {
                    statusTmp = status;
                    PH_BREAK_ON_FAILURE(status);
                }

                /* Update buffer pointer and length */
                pTmpBuffer += wFifoBytes;
                wTmpBufferLen = wTmpBufferLen - wFifoBytes;
            }
            else
            {
                status = PH_ERR_SUCCESS;
            }

            /* Precache given buffer contents into FIFO */
            if ((status & PH_ERR_MASK) != PH_ERR_BUFFER_OVERFLOW)
            {
                wNumPrecachedBytes = (wTxLength > (pDataParams->wMaxPrecachedBytes - wNumPrecachedBytes)) ? (pDataParams->wMaxPrecachedBytes - wNumPrecachedBytes) : wTxLength;

                if (wNumPrecachedBytes > 0U)
                {
                    /* Perform actual write to FIFO */
                    status = phhalHw_Rc663_WriteFifo(pDataParams, pTxBuffer, wNumPrecachedBytes, &wFifoBytes);

                    /* Ignore FiFo overflow warning */
                    if ((status & PH_ERR_MASK) != PH_ERR_BUFFER_OVERFLOW)
                    {
                        statusTmp = status;
                        PH_BREAK_ON_FAILURE(status);
                    }

                    /* Update buffer pointer and length */
                    pTxBuffer += wFifoBytes;
                    wTxLength = wTxLength - wFifoBytes;
                }
            }
        }while(FALSE);
    }

    /* If Guard time is configured previously using SetConfig(PHHAL_HW_CONFIG_POLL_GUARD_TIME_US). */
    if (pDataParams->bPollGuardTimeFlag == PH_ON)
    {
        pDataParams->bPollGuardTimeFlag = PH_OFF;

        /* retrieve content of Timer Control register */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, &bRegister));

        /* Check if timer is running for guard time to expire, else don't wait for timer IRQ. */
        if ((0U != ((bRegister & PHHAL_HW_RC663_BIT_T1RUNNING))) &&
            ((statusTmp == PH_ERR_SUCCESS) ||
            (statusTmp == PH_ERR_BUFFER_OVERFLOW)))
        {
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_WaitIrq(pDataParams, PH_ON, PH_OFF, 0x00, PHHAL_HW_RC663_BIT_TIMER1IRQ, NULL, &bIrq1Reg));
        }

        /* Stop Timer T0 and T1 used for Poll guard time. */
        bRegister &= (uint8_t)~(uint8_t)(PHHAL_HW_RC663_BIT_T0RUNNING | PHHAL_HW_RC663_BIT_T1RUNNING);
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, bRegister));
        /* Clear Timer Interrupt */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1, (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_SET));

        /* Apply FWT/FDT delay from dataparams. */
        if (pDataParams->bTimeoutUnit == PHHAL_HW_TIME_MICROSECONDS)
        {
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_SetFdt(
                pDataParams,
                PHHAL_HW_TIME_MICROSECONDS,
                pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_US]));
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_SetFdt(
                pDataParams,
                PHHAL_HW_TIME_MILLISECONDS,
                pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS]));
        }
    }

    /* If there is any error reported before, then return error. */
    if ((statusTmp != PH_ERR_SUCCESS) &&
        (statusTmp != PH_ERR_BUFFER_OVERFLOW))
    {
        return statusTmp;
    }

#ifndef NXPBUILD__PH_PLATFORM_DELEGATE

    /* start the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_TRANSCEIVE));

    /* Set wait IRQs */
    bIrq0WaitFor |= PHHAL_HW_RC663_BIT_TXIRQ | PHHAL_HW_RC663_BIT_IDLEIRQ;
    bIrq1WaitFor = PHHAL_HW_RC663_BIT_TIMER1IRQ;

#endif /* NXPBUILD__PH_PLATFORM_DELEGATE */

    do{
#ifndef NXPBUILD__PH_PLATFORM_DELEGATE
        /* wait until the transmission is finished */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WaitIrq(
            pDataParams,
            PH_ON,
            PH_OFF,
            bIrq0WaitFor,
            bIrq1WaitFor,
            &bIrq0Reg,
            &bIrq1Reg));
#endif /* NXPBUILD__PH_PLATFORM_DELEGATE*/

        /* Write the remaining data into the Fifo */
        /* (Only possible if interface to RC is fast enough!) */
        if (0U != (bIrq0WaitFor & PHHAL_HW_RC663_BIT_LOALERTIRQ))
        {
            /* Switch from prechache buffer to given buffer if it's empty */
            if (wTmpBufferLen == 0U)
            {
                pTmpBuffer = pTxBuffer;
                wTmpBufferLen = wTxLength;
                wTxLength = 0;
            }
            /* */
            /* Check for FIFO underflow */
            /* */
#ifdef PHHAL_HW_RC663_FEATURE_FIFO_UNDERFLOW_CHECK
            /* read interrupt status */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0, &bIrq0Reg));

            /* If we got data to transmit but the Tx-command aborted, we were too slow! */
            if (((0U != (bIrq0Reg & PHHAL_HW_RC663_BIT_IDLEIRQ)) ||
                (0U != (bIrq0Reg & PHHAL_HW_RC663_BIT_TXIRQ)))
                && (wTmpBufferLen != 0U) &&
                (wTxLength != 0U))
            {
                /* stop the command */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                    pDataParams,
                    PHHAL_HW_RC663_REG_COMMAND,
                    PHHAL_HW_RC663_CMD_IDLE));

                /* Flush FiFo */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));

                return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERFACE_ERROR, PH_COMP_HAL);
            }
#endif
            /* write remaining data to transmit into FiFo buffer */
            status = phhalHw_Rc663_WriteFifo(pDataParams, pTmpBuffer, wTmpBufferLen, &wFifoBytes);

            /* Ignore FiFo overflow warning */
            if ((status & PH_ERR_MASK) != PH_ERR_BUFFER_OVERFLOW)
            {
                PH_CHECK_SUCCESS(status);
            }

            /* Update buffer pointer and length */
            pTmpBuffer += wFifoBytes;
            wTmpBufferLen = wTmpBufferLen - wFifoBytes;

            /* Mask the LOW ALERT IRQ. */
            bIrq0Reg &= (uint8_t)~PHHAL_HW_RC663_BIT_LOALERTIRQ;

            /* Clear the LOW ALERT IRQ. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                pDataParams,
                PHHAL_HW_RC663_REG_IRQ0,
                (uint8_t)PHHAL_HW_RC663_BIT_LOALERTIRQ));
        }

    }while((0U == (bIrq0Reg & bIrq0WaitFor)) &&
        (0U == (bIrq1Reg & bIrq1WaitFor)));

    /* Let's now wait for the RX Irq.*/
    if ((0U == (bIrq0Reg & PHHAL_HW_RC663_BIT_IDLEIRQ))
        && (0U == (bIrq0Reg & PHHAL_HW_RC663_BIT_ERRIRQ)))
    {
        /* Clear high-alert irq flag */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0, PHHAL_HW_RC663_BIT_HIALERTIRQ));

        /* Change wait IRQs */
        if(0U != (pDataParams->bRxMultiple))
        {
            bIrq0WaitFor = PHHAL_HW_RC663_BIT_RXIRQ |PHHAL_HW_RC663_BIT_IDLEIRQ | PHHAL_HW_RC663_BIT_EMDIRQ;
            bIrq1WaitFor = PHHAL_HW_RC663_BIT_TIMER1IRQ;
        }
        else
        {
            bIrq0WaitFor = PHHAL_HW_RC663_BIT_RXIRQ | PHHAL_HW_RC663_BIT_HIALERTIRQ | PHHAL_HW_RC663_BIT_IDLEIRQ | PHHAL_HW_RC663_BIT_EMDIRQ;
        }

        /* EMD Handling logic will be enabled only when EMD is enabled and SPI interface is selected. */
        if ((pDataParams->bCheckEmdErr != PH_ON) || (pDataParams->bBalConnectionType != PHHAL_HW_BAL_CONNECTION_SPI))
        {
            /* wait until reception */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WaitIrq(
                pDataParams,
                PH_ON,
                PH_OFF,
                bIrq0WaitFor,
                bIrq1WaitFor,
                &bIrq0Reg,
                &bIrq1Reg));
        }
        else
        {
            /* EMD Suppression is requested, Special Handling required */
            dwSaveReg = 0x00;

            /* Set wait IRQs  this required below */
            bIrq0WaitFor = PHHAL_HW_RC663_BIT_RXIRQ | PHHAL_HW_RC663_BIT_HIALERTIRQ | PHHAL_HW_RC663_BIT_IDLEIRQ | PHHAL_HW_RC663_BIT_EMDIRQ;
            bIrq1WaitFor = PHHAL_HW_RC663_BIT_TIMER1IRQ;

            status = phhalHw_Rc663_CheckForEmdError(pDataParams, bIrq0WaitFor, bIrq1WaitFor, &dwMultiReg, &dwSaveReg);

            /* Parse Irq0 and Irq1 data */
            bIrq0Reg = (uint8_t)dwMultiReg;
            bIrq1Reg = (uint8_t)(dwMultiReg >> 8U);
        }

    }

    /* Reset TxLastBits */
    if (pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS] != 0U)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXLASTBITS, 0x00U));
    }

    /* Reset buffered bytes */
    pDataParams->wTxBufLen = 0;

    /* Enable DataEn again if no data has been sent */
    if (bNoData != PH_OFF)
    {
        /* Read out TxDataNum register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TXDATANUM, &bRegister));

        /* Enable DataEn */
        bRegister |= PHHAL_HW_RC663_BIT_DATAEN;

        /* Write changed register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXDATANUM, bRegister));
    }

    /* Timeout handling */
    if ((0U != (bIrq1Reg & PHHAL_HW_RC663_BIT_TIMER1IRQ)) &&
        (0U == (bIrq0Reg & PHHAL_HW_RC663_BIT_EMDIRQ)))
    {
        /* retrieve content of Timer Control register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, &bRegister));

        /* manually stop timers T0 and T1 used for FDT/FWT. */
        bRegister |= PHHAL_HW_RC663_BIT_T0STARTSTOPNOW | PHHAL_HW_RC663_BIT_T1STARTSTOPNOW;
        bRegister &= (uint8_t)~(uint8_t)(PHHAL_HW_RC663_BIT_T0RUNNING | PHHAL_HW_RC663_BIT_T1RUNNING);
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, bRegister));

        /* Reset after timeout behavior */
        if (pDataParams->bRfResetAfterTo != PH_OFF)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FieldReset(pDataParams));
        }

        status = PH_ERR_IO_TIMEOUT;

        /* WorkAround for 18000p3m3 */
        if(0U != (wVerifyRxCrc5))
        {
            /* Check Whether Error IRQ is set along with Timeout */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0, &bRegister));
            if(0U != (bRegister & PHHAL_HW_RC663_BIT_ERRIRQ))
            {
                 PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_ERROR, &bRegister));
                 /* If Either of MINFRAME\COLLDET\PROT\INTEG error is set treat that as collision */
                if(
                    (0U != (bRegister & PHHAL_HW_RC663_BIT_MINFRAMEERR)) ||
                    (0U != (bRegister & PHHAL_HW_RC663_BIT_COLLDET)) ||
                    (0U != (bRegister & PHHAL_HW_RC663_BIT_PROTERR)) ||
                    (0U != (bRegister & PHHAL_HW_RC663_BIT_INTEGERR)))
                 {
                    status = PH_ERR_COLLISION_ERROR;
                }
            }
        }
    }
    else
    {
        status = PH_ERR_SUCCESS;
    }

    /* Reset receive buffer length */
    pDataParams->wRxBufLen = 0;

    /* Retrieve receive buffer properties */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetRxBuffer(pDataParams, PH_ON, &pTmpBuffer, &wTmpBufferLen, &wTmpBufferSize));

    /* Needed for 18092 PAL RX chaining, backup the byte which will be over
     * written by received data. */
    if(pDataParams->bNfcipMode == PH_ON)
    {
        if ((pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING] == PHHAL_HW_RF_DATARATE_106) &&
            (pDataParams->bCardType == PHHAL_HW_CARDTYPE_ISO14443A))
        {
            bSbBackup = pTmpBuffer[0];
        }
    }

    /* Do the following if no timeout occurred */
    if (status == PH_ERR_SUCCESS)
    {
        bIrq0WaitFor_tmp = bIrq0WaitFor;
        do
        {
            /* Either HIALERTIRQ Or RxIRQ is there, retrieve fifo bytes.
             * If RxIRQ break */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadFifo(
                    pDataParams,
                    wTmpBufferSize,
                    &pTmpBuffer[wTmpBufferLen],
                    &wFifoBytes));

            /* advance receive buffer */
            wTmpBufferLen = wTmpBufferLen + wFifoBytes;
            wTmpBufferSize = wTmpBufferSize - wFifoBytes;

            if (0U != (bIrq0Reg & PHHAL_HW_RC663_BIT_RXIRQ))
            {
                break;
            }
            else
            {
                /* High Alert. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                    pDataParams,
                    PHHAL_HW_RC663_REG_IRQ0,
                    (uint8_t)PHHAL_HW_RC663_BIT_HIALERTIRQ));
            }

            bIrq0WaitFor_tmp &= (uint8_t)~(uint8_t)(PHHAL_HW_RC663_BIT_HIALERTIRQ
                                    |  PHHAL_HW_RC663_BIT_RXIRQ | PHHAL_HW_RC663_BIT_IDLEIRQ);

            /* Enabling IRQs, so that already occurred events are not missed. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0EN, &bRegister));

            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0EN,
                bIrq0WaitFor | (bRegister & PHHAL_HW_RC663_BIT_IRQINV)));

            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1EN,
                bIrq1WaitFor | PHHAL_HW_RC663_BIT_IRQPUSHPULL | PHHAL_HW_RC663_BIT_GLOBALIRQ));

            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WaitIrq(
                pDataParams,PH_OFF,PH_OFF,
                bIrq0WaitFor,bIrq1WaitFor,
                &bIrq0Reg,&bIrq1Reg));
        }
        while (
            (0U == (bIrq0Reg & bIrq0WaitFor_tmp)) &&
            (0U == (bIrq1Reg & bIrq1WaitFor)));
        /*
        * Check if an error occurred
        * read the error register
        */
        status =  phhalHw_Rc663_GetErrorStatus(pDataParams, &wTmpBufferLen, &wTmpBufferSize);
        if (status == PH_ERR_FRAMING_ERROR)
        {
            if((dwMultiReg >> 24U) == 0x05U)
            {
                status = PH_ERR_SUCCESS;
            }
        }
    }

    /* Exchange was successful */
    if (status == PH_ERR_SUCCESS)
    {
        /* Retrieve valid bits of last byte */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RXBITCTRL, &bRegister));

        /* Mask out valid bits of last byte */
        bRegister &= PHHAL_HW_RC663_MASK_RXLASTBITS;

        /* Set RxLastBits */
        pDataParams->wAdditionalInfo = bRegister;

        /* Set incomplete byte status if applicable */
        if (bRegister != 0x00U)
        {
            status = PH_ERR_SUCCESS_INCOMPLETE_BYTE;
        }
    }

    /* Stop the command / flush the FIFO on error */
    if ((status != PH_ERR_SUCCESS) &&
        (status != PH_ERR_SUCCESS_INCOMPLETE_BYTE))
    {
        /* stop the command */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_IDLE));

        /* Flush FiFo */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));
    }

    /* Store received data length in dataparams */
    pDataParams->wRxBufLen = pDataParams->wRxBufStartPos + wTmpBufferLen;

    if((0U != (pDataParams->bRxMultiple)) &&
        (status == PH_ERR_SUCCESS))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FrameRxMultiplePkt(pDataParams));
    }

    /* Remove Start Byte (0xF0) during communication at Passive 106kbps during NFCIP exchange. */
    if ((pDataParams->bNfcipMode == PH_ON) &&
        (status == PH_ERR_SUCCESS))
    {
        /* Read the Shadow and Card Type if it configured at 106kbps Type-A */
        if ((pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING] == PHHAL_HW_RF_DATARATE_106) &&
            (pDataParams->bCardType == PHHAL_HW_CARDTYPE_ISO14443A))
        {
            if(pDataParams->pRxBuffer[0] == PHHAL_HW_RF_TYPE_A_START_BYTE)
            {
                pTmpBuffer[0] = bSbBackup;
                wSBIndex = 1;
                pDataParams->wRxBufLen--;
            }
            else
            {
                status = PH_ERR_IO_TIMEOUT;
            }
        }
    }

    /* Return RxBuffer pointer */
    if (ppRxBuffer != NULL)
    {
        *ppRxBuffer = pDataParams->pRxBuffer + wSBIndex;
    }

    /* Return RxBuffer length */
    if (pRxLength != NULL)
    {
        /* Emvco: case_id TB306_12 */
        if((pDataParams->bOpeMode != RD_LIB_MODE_ISO) &&
            ((status & PH_ERR_MASK) == PH_ERR_NOISE_ERROR))
        {
            *pRxLength = (uint16_t)((dwSaveReg >> 24U) & 0xFFU);
        }
        else
        {
            *pRxLength = pDataParams->wRxBufLen;
        }
    }

    /* Timing */
    if ((pDataParams->wTimingMode & (uint16_t)~(uint16_t)PHHAL_HW_TIMING_MODE_OPTION_MASK) == PHHAL_HW_TIMING_MODE_FDT)
    {
        /* Calculate FDT Value */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetFdt(pDataParams, status, &dwTimingSingle));

        /* FDT single -> replace the previous value */
        if (0U != (pDataParams->wTimingMode & PHHAL_HW_TIMING_MODE_OPTION_AUTOCLEAR))
        {
            pDataParams->dwTimingUs = dwTimingSingle;
        }
        /* FDT -> add current value to current count */
        else
        {
            /* Check for overflow */
            if ((0xFFFFFFFFU - pDataParams->dwTimingUs) < dwTimingSingle)
            {
                pDataParams->dwTimingUs = 0xFFFFFFFFU;
            }
            /* Update global timing value */
            else
            {
                pDataParams->dwTimingUs = pDataParams->dwTimingUs + dwTimingSingle;
            }
        }
    }

    do
    {
        /* QAC Fix */
        if((NULL == ppRxBuffer) ||
            (NULL == pRxLength ))
        {
            status = PH_ERR_INVALID_PARAMETER;
            break;
        }

        if(((pDataParams->bJewelActivated) == PH_ON) && ((status == PH_ERR_SUCCESS) ||
            (( status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)))
        {
            /* Remove parity of the received data-as harware's parity is off */
            statusTmp = phTools_DecodeParity(
                    PH_TOOLS_PARITY_OPTION_ODD,
                    *ppRxBuffer,
                    *pRxLength,
                    (uint8_t)pDataParams->wAdditionalInfo,
                    *pRxLength,
                    *ppRxBuffer,
                    &wTmpBufferLen,
                    &bBitlen);

            /* Return error if there is a Parity error */
            if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)
            {
                status = statusTmp & PH_ERR_MASK;
                break;
            }

            /* Check for response length
             * For Type 1 tag, valid minimum response length is 4 */
            if(wTmpBufferLen >= 4U)
            {
                /*verify Crc_B for the received data*/
                PH_CHECK_SUCCESS_FCT(statusTmp, phTools_ComputeCrc_B( *ppRxBuffer, (uint32_t)wTmpBufferLen - 2U, aCrc));

                if((aCrc[0] != ((*ppRxBuffer)[wTmpBufferLen - 2U])) || (aCrc[1] != ((*ppRxBuffer)[wTmpBufferLen - 1U])))
                {
                    status = PH_ERR_INTEGRITY_ERROR;
                    break;
                }
                else
                {
                    /* subtract CRC from Rx buffer */
                    *pRxLength = wTmpBufferLen - 2U;
                    status = PH_ERR_SUCCESS;
                }
            }
            else
            {
                status = PH_ERR_PROTOCOL_ERROR;
                break;
            }
        }

        if((0U != (wVerifyRxCrc5)) &&  ((*pRxLength - pDataParams->wRxBufStartPos) != 0U))
        {
            /* Get bit length */
            bBitlen = (uint8_t)((pDataParams->wAdditionalInfo > 0U) ?
                (((uint16_t)((*pRxLength - pDataParams->wRxBufStartPos) - 1U) << 3U) + pDataParams->wAdditionalInfo) :
                    ((uint16_t)(*pRxLength - pDataParams->wRxBufStartPos) << 3U));

            /* CRC error if CRC-5 is not present */
            if (bBitlen < 5U)
            {
                status = PH_ERR_INTEGRITY_ERROR;
                break;
            }

            PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc5(
                PH_TOOLS_CRC_OPTION_MSB_FIRST | PH_TOOLS_CRC_OPTION_BITWISE,
                PH_TOOLS_CRC5_PRESET_I18000P3,
                PH_TOOLS_CRC5_POLY_I18000P3,
                &(*ppRxBuffer)[pDataParams->wRxBufStartPos],
                (uint16_t)bBitlen,
                &aCrc[0]));

            /* Return error if CRC does not match */
            if (aCrc[0] != 0x00U)
            {
                status = PH_ERR_INTEGRITY_ERROR;
                break;
            }

            /* Remove CRC5 from output data */
            bBitlen = bBitlen - 5U;
            pDataParams->wAdditionalInfo = (uint8_t)(bBitlen & 0x07U);

            /* Update total RX length */
            *pRxLength = (pDataParams->wAdditionalInfo > 0U) ? (((uint16_t)bBitlen >> 3U) + 1U) : ((uint16_t)bBitlen >> 3U);
            *pRxLength += pDataParams->wRxBufStartPos;

            /*  Mask out CRC bits */
            if (pDataParams->wAdditionalInfo > 0U)
            {
                (*ppRxBuffer)[*pRxLength - 1U] &= (uint8_t)(0xFFU << (8U - pDataParams->wAdditionalInfo));
            }
        }
    }while (FALSE);

    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}

phStatus_t phhalHw_Rc663_Transmit(
                                  phhalHw_Rc663_DataParams_t * pDataParams,
                                  uint16_t wOption,
                                  uint8_t * pTxBuffer,
                                  uint16_t wTxLength
                                  )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wNumPrecachedBytes;
    uint16_t    PH_MEMLOC_REM wFifoBytes = 0;
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq1WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq0Reg = 0x00;
    uint8_t     PH_MEMLOC_REM bIrq1Reg = 0x00;
    uint8_t     PH_MEMLOC_REM bRegister;
    uint8_t     PH_MEMLOC_REM bError;
    uint8_t     PH_MEMLOC_REM bNoData;
    uint8_t *   PH_MEMLOC_REM pTmpBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wTmpBufferLen = 0;
    uint16_t    PH_MEMLOC_REM wTmpBufferSize;

    /* Check options */
    if (0U != (wOption & (uint16_t)~(uint16_t)(PH_EXCHANGE_BUFFERED_BIT | PH_EXCHANGE_LEAVE_BUFFER_BIT)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }
    wTmpBufferLen = 0x00;

    /* clear internal buffer if requested */
    if (0U == (wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        pDataParams->wTxBufLen = 0;
    }

    /* Prepend Start Byte (0xF0) in Passive 106kbps during NFCIP exchange. */
    if ((pDataParams->bNfcipMode == PH_ON) &&
        (0U == (wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT)))
    {
        /* Read the Shadow and Card Type if it configured at 106kbps Type-A */
        if ((pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING] == PHHAL_HW_RF_DATARATE_106) &&
            (pDataParams->bCardType == PHHAL_HW_CARDTYPE_ISO14443A))
        {
            /* retrieve transmit buffer */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetTxBuffer(pDataParams, PH_ON, &pTmpBuffer, &wTmpBufferLen, &wTmpBufferSize));

            /* Prepend Start Byte 0xF0 in case of Passive Communication at 106kbps BaudRate */
            (void)memset(&pTmpBuffer[wTmpBufferLen], 0xF0, 0x01);
            pDataParams->wTxBufLen++;
        }
    }

    /* Fill the global TxBuffer */
    /* Note: We always need to buffer for SPI, else the input buffer would get overwritten! */
    if ((0U != (wOption & PH_EXCHANGE_BUFFERED_BIT)) ||
        (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_SPI) ||
        (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_I2C))
    {
        /* retrieve transmit buffer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetTxBuffer(pDataParams, PH_ON, &pTmpBuffer, &wTmpBufferLen, &wTmpBufferSize));

        if (wTxLength != 0U)
        {
            /* TxBuffer overflow check */
            if (wTxLength > (wTmpBufferSize - wTmpBufferLen))
            {
                pDataParams->wTxBufLen = 0;
                return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
            }

            /* copy data */
            (void)memcpy(&pTmpBuffer[wTmpBufferLen], pTxBuffer, wTxLength);
            pDataParams->wTxBufLen = pDataParams->wTxBufLen + wTxLength;
        }

        /* Buffer operation -> finished */
        if (0U != (wOption & PH_EXCHANGE_BUFFERED_BIT))
        {
            return PH_ERR_SUCCESS;
        }
        /* Else reset the input length for SPI and continue */
        else
        {
            wTxLength = 0;
        }
    }

    /* retrieve transmit buffer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetTxBuffer(pDataParams, PH_ON, &pTmpBuffer, &wTmpBufferLen, &wTmpBufferSize));

    /* Disable DataEn if no data is to be sent */
    if ((wTmpBufferLen == 0U) &&
        (wTxLength == 0U))
    {
        /* Store that we sent no data */
        bNoData = PH_ON;

        /* Read out TxDataNum register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TXDATANUM, &bRegister));

        /* Disable DataEn */
        bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_DATAEN;

        /* Write changed register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXDATANUM, bRegister));
    }
    else
    {
        /* Reset NoData flag */
        bNoData = PH_OFF;
    }

    /* Terminate a possibly running command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_IDLE));

    /* Flush FiFo */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));

    /* clear all IRQ0 flags */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_IRQ0,
        (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_SET));

    /* clear all IRQ1 flags */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_IRQ1,
        (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_SET));

    /* Precache buffered contents into FIFO */
    wNumPrecachedBytes = (wTmpBufferLen > pDataParams->wMaxPrecachedBytes) ? pDataParams->wMaxPrecachedBytes : wTmpBufferLen;
    if (wNumPrecachedBytes > 0U)
    {
        /* Perform actual write to FIFO */
        status = phhalHw_Rc663_WriteFifo(pDataParams, pTmpBuffer, wNumPrecachedBytes, &wFifoBytes);

        /* Ignore FiFo overflow warning */
        if ((status & PH_ERR_MASK) != PH_ERR_BUFFER_OVERFLOW)
        {
            PH_CHECK_SUCCESS(status);
        }

        /* Update buffer pointer and length */
        pTmpBuffer += wFifoBytes;
        wTmpBufferLen = wTmpBufferLen - wFifoBytes;
    }
    else
    {
        status = PH_ERR_SUCCESS;
    }

    /* Precache given buffer contents into FIFO */
    if ((status & PH_ERR_MASK) != PH_ERR_BUFFER_OVERFLOW)
    {
        wNumPrecachedBytes = (wTxLength > (pDataParams->wMaxPrecachedBytes - wNumPrecachedBytes)) ? (pDataParams->wMaxPrecachedBytes - wNumPrecachedBytes) : wTxLength;

        if (wNumPrecachedBytes > 0U)
        {
            /* Perform actual write to FIFO */
            status = phhalHw_Rc663_WriteFifo(pDataParams, pTxBuffer, wNumPrecachedBytes, &wFifoBytes);

            /* Ignore FiFo overflow warning */
            if ((status & PH_ERR_MASK) != PH_ERR_BUFFER_OVERFLOW)
            {
                PH_CHECK_SUCCESS(status);
            }

            /* Update buffer pointer and length */
            pTxBuffer += wFifoBytes;
            wTxLength = wTxLength - wFifoBytes;
        }
    }

    /* start the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_TRANSMIT));

    /* Write the remaining data into the Fifo */
    /* (Only possible if interface to RC is fast enough!) */
    while ((wTmpBufferLen != 0U) ||
        (wTxLength != 0U))
    {
        /* Switch from prechache buffer to given buffer if it's empty */
        if (wTmpBufferLen == 0U)
        {
            pTmpBuffer = pTxBuffer;
            wTmpBufferLen = wTxLength;
            wTxLength = 0;
        }
        /* */
        /* Check for FIFO underflow */
        /* */
#ifdef PHHAL_HW_RC663_FEATURE_FIFO_UNDERFLOW_CHECK
        /* read interrupt status */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0, &bIrq0Reg));

        if ((0U != (bIrq0Reg & PHHAL_HW_RC663_BIT_IDLEIRQ)) ||
            (0U != (bIrq0Reg & PHHAL_HW_RC663_BIT_TXIRQ)))
        {
            /* stop the command */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                pDataParams,
                PHHAL_HW_RC663_REG_COMMAND,
                PHHAL_HW_RC663_CMD_IDLE));

            /* Flush FiFo */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));

            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERFACE_ERROR, PH_COMP_HAL);
        }
#endif
        /* write remaining data to transmit into FiFo buffer */
        status = phhalHw_Rc663_WriteFifo(pDataParams, pTmpBuffer, wTmpBufferLen, &wFifoBytes);

        /* Ignore FiFo overflow warning */
        if ((status & PH_ERR_MASK) != PH_ERR_BUFFER_OVERFLOW)
        {
            PH_CHECK_SUCCESS(status);
        }

        /* Update buffer pointer and length */
        pTmpBuffer += wFifoBytes;
        wTmpBufferLen = wTmpBufferLen - wFifoBytes;
    }

    /* Set wait IRQs */
    bIrq0WaitFor = PHHAL_HW_RC663_BIT_TXIRQ | PHHAL_HW_RC663_BIT_IDLEIRQ;
    bIrq1WaitFor = 0x00;

    /* wait until the command is finished */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WaitIrq(
        pDataParams,
        PH_ON,
        PH_OFF,
        bIrq0WaitFor,
        bIrq1WaitFor,
        &bIrq0Reg,
        &bIrq1Reg));

    /* Check if an error occured */
    if (0U != (bIrq0Reg & PHHAL_HW_RC663_BIT_ERRIRQ))
    {
        /* read the error register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_ERROR, &bError));

        /* no data error */
        if (0U != (bError & PHHAL_HW_RC663_BIT_NODATAERR))
        {
            status = PH_ERR_INTERNAL_ERROR;
        }
        /* buffer overflow */
        else if (0U != (bError & PHHAL_HW_RC663_BIT_FIFOOVL))
        {
            status = PH_ERR_BUFFER_OVERFLOW;
        }
        /* Fifo write error */
        else if (0U != (bError & PHHAL_HW_RC663_BIT_FIFOWRERR))
        {
            status = PH_ERR_READ_WRITE_ERROR;
        }
        /* No error */
        else
        {
            status = PH_ERR_SUCCESS;
        }
    }
    /* No error */
    else
    {
        status = PH_ERR_SUCCESS;
    }

    /* Reset TxLastBits */
    if (pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS] != 0U)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXLASTBITS, 0x00U));
    }

    /* Reset buffered bytes */
    pDataParams->wTxBufLen = 0;

    /* Enable DataEn again if no data has been sent */
    if (bNoData != PH_OFF)
    {
        /* Read out TxDataNum register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TXDATANUM, &bRegister));

        /* Enable DataEn */
        bRegister |= PHHAL_HW_RC663_BIT_DATAEN;

        /* Write changed register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXDATANUM, bRegister));
    }

    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}


phStatus_t phhalHw_Rc663_Receive(
                                 phhalHw_Rc663_DataParams_t * pDataParams,
                                 uint16_t wOption,
                                 uint8_t ** ppRxBuffer,
                                 uint16_t * pRxLength
                                 )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wFifoBytes;
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor_tmp;
    uint8_t     PH_MEMLOC_REM bIrq1WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq0Reg = 0x00;
    uint8_t     PH_MEMLOC_REM bIrq1Reg = 0x00;
    uint8_t     PH_MEMLOC_REM bRegister;
    uint8_t     PH_MEMLOC_REM bRegRxWait;
    uint8_t     PH_MEMLOC_REM bError;
    uint8_t     PH_MEMLOC_REM bSbBackup = 0;
    uint32_t    PH_MEMLOC_REM dwTimingSingle;
    uint8_t *   PH_MEMLOC_REM pTmpBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wTmpBufferLen = 0;
    uint16_t    PH_MEMLOC_REM wTmpBufferSize;
    uint16_t    PH_MEMLOC_REM wSBIndex = 0;

    /* Check options */
    if (wOption != PH_RECEIVE_DEFAULT)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Reduce RxWait to minimum */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RXWAIT, &bRegRxWait));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXWAIT, 0U));

    /* set the receive length */
    if (pRxLength != NULL)
    {
        *pRxLength = 0;
    }

    /* Terminate a probably running command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_IDLE));

    /* Flush FiFo */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));

    /* clear all IRQ0 flags */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_IRQ0,
        (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_SET));

    /* clear all IRQ1 flags */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_IRQ1,
        (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_SET));

    /* timer should be running, otherwise we'd wait forever */
    /* retrieve content of Timer Control register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, &bRegister));
    /* Check T1Running flag */
    if (0U == (bRegister & PHHAL_HW_RC663_BIT_T1RUNNING))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_IO_TIMEOUT, PH_COMP_HAL);
    }

    /* start the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_RECEIVE));

    /* Set wait IRQs */
    bIrq0WaitFor = PHHAL_HW_RC663_BIT_RXIRQ | PHHAL_HW_RC663_BIT_HIALERTIRQ | PHHAL_HW_RC663_BIT_IDLEIRQ | PHHAL_HW_RC663_BIT_EMDIRQ;
    bIrq1WaitFor = PHHAL_HW_RC663_BIT_TIMER1IRQ;

    /* wait until the command is finished */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WaitIrq(
        pDataParams,
        PH_ON,
        PH_OFF,
        bIrq0WaitFor,
        bIrq1WaitFor,
        &bIrq0Reg,
        &bIrq1Reg));

    /* Timeout handling */
    if ((0U != (bIrq1Reg & PHHAL_HW_RC663_BIT_TIMER1IRQ)) &&
        (0U == (bIrq0Reg & PHHAL_HW_RC663_BIT_EMDIRQ)))
    {
        /* stop the command */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_COMMAND,
            PHHAL_HW_RC663_CMD_IDLE));

        /* Flush FiFo */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));

        /* retrieve content of Timer Control register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, &bRegister));

        /* manually stop timers T0 and T1 used for FDT/FWT. */
        bRegister |= PHHAL_HW_RC663_BIT_T0STARTSTOPNOW | PHHAL_HW_RC663_BIT_T1STARTSTOPNOW;
        bRegister &= (uint8_t)~(uint8_t)(PHHAL_HW_RC663_BIT_T0RUNNING | PHHAL_HW_RC663_BIT_T1RUNNING);
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, bRegister));

        /* Reset after timeout behavior */
        if (pDataParams->bRfResetAfterTo != PH_OFF)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FieldReset(pDataParams));
        }

        status = PH_ERR_IO_TIMEOUT;
    }
    else
    {
        status = PH_ERR_SUCCESS;
    }

    /* Reset receive buffer length */
    pDataParams->wRxBufLen = 0;

    /* Retrieve receive buffer properties */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetRxBuffer(pDataParams, PH_ON, &pTmpBuffer, &wTmpBufferLen, &wTmpBufferSize));

    /* Needed for 18092 PAL RX chaining, backup the byte which will be over
     * written by received data. */
    if(pDataParams->bNfcipMode == PH_ON)
    {
        if ((pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING] == PHHAL_HW_RF_DATARATE_106) &&
            (pDataParams->bCardType == PHHAL_HW_CARDTYPE_ISO14443A))
        {
            bSbBackup = pTmpBuffer[0];
        }
    }

    /* Do the following if no timeout occurred */
    if (status == PH_ERR_SUCCESS)
    {
        /* mask out high-alert */
        bIrq0WaitFor &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_HIALERTIRQ;

        /* retrieve fifo bytes */
        do
        {
            /* read interrupt status */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0, &bIrq0Reg));
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1, &bIrq1Reg));

            bIrq0WaitFor_tmp = bIrq0WaitFor;
            bIrq0WaitFor_tmp &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_HIALERTIRQ;


            /* retrieve bytes from FiFo */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadFifo(
                pDataParams,
                wTmpBufferSize,
                &pTmpBuffer[wTmpBufferLen],
                &wFifoBytes));

            /* advance receive buffer */
            wTmpBufferLen = wTmpBufferLen + wFifoBytes;
            wTmpBufferSize = wTmpBufferSize - wFifoBytes;

            if (0U != (bIrq0Reg & PHHAL_HW_RC663_BIT_HIALERTIRQ))
            {
                              PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                                            pDataParams,
                                            PHHAL_HW_RC663_REG_IRQ0,
                                            (uint8_t)PHHAL_HW_RC663_BIT_HIALERTIRQ));
            }
        }
        while (((0U == ((bIrq0Reg & bIrq0WaitFor_tmp))) && (0U == ((bIrq1Reg & bIrq1WaitFor)))));

        /* */
        /* Check if an error occured */
        /* read the error register */
        /* */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_ERROR, &bError));

        /* buffer overflow */
        if (0U != (bError & PHHAL_HW_RC663_BIT_FIFOOVL))
        {
            status = PH_ERR_BUFFER_OVERFLOW;
        }
        /* Fifo write error */
        else if (0U != (bError & PHHAL_HW_RC663_BIT_FIFOWRERR))
        {
            status = PH_ERR_READ_WRITE_ERROR;
        }
        /* framing error */
        else if (0U != (bError & PHHAL_HW_RC663_BIT_MINFRAMEERR))
        {
            status = PH_ERR_FRAMING_ERROR;
        }
        /* collision */
        else if (0U != (bError & PHHAL_HW_RC663_BIT_COLLDET))
        {
            /* No bytes received -> timeout */
            if (wTmpBufferLen == 0U)
            {
                status = PH_ERR_IO_TIMEOUT;
            }
            /* Else perform collision handling */
            else
            {
                status = PH_ERR_COLLISION_ERROR;

                /* Retrieve collision position */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RXCOLL, &bRegister));

                /* check if collpos is valid */
                if (0U != (bRegister & PHHAL_HW_RC663_BIT_COLLPOSVALID))
                {
                    /* retrieve collpos */
                    bRegister &= PHHAL_HW_RC663_MASK_COLLPOS;
                }
                else
                {
                    /* collision position out of range --> max. */
                    bRegister = 0x3F;
                }

                /* Restore wRxBufSize */
                wTmpBufferSize = wTmpBufferSize + wTmpBufferLen;

                /* Check for buffer overflow */
                if (((uint16_t)bRegister >> 3U) > wTmpBufferSize)
                {
                    /* Return the maximum possible size of data */
                    wTmpBufferLen = wTmpBufferSize;
                    status = PH_ERR_BUFFER_OVERFLOW;
                }
                else
                {
                    /* Adjust valid byte count */
                    wTmpBufferLen = ((uint16_t)bRegister >> 3U);
                }

                /* Mask out valid bits of last byte */
                bRegister &= PHHAL_HW_RC663_MASK_RXLASTBITS;

                /* Incomplete byte also counts as received byte */
                if (0U != (bRegister))
                {
                    /* Check for buffer overflow */
                    if (wTmpBufferLen == wTmpBufferSize)
                    {
                        status = PH_ERR_BUFFER_OVERFLOW;
                    }
                    else
                    {
                        ++wTmpBufferLen;
                    }
                }

                /* Store valid bits of last byte */
                pDataParams->wAdditionalInfo = bRegister;
            }
        }
        /* CRC / parity error */
        else if (0U != (bError & PHHAL_HW_RC663_BIT_INTEGERR))
        {
            status = PH_ERR_INTEGRITY_ERROR;
        }
        /* protocol error */
        else if (0U != (bError & PHHAL_HW_RC663_BIT_PROTERR))
        {
            status = PH_ERR_PROTOCOL_ERROR;
        }
        /* No error */
        else
        {
            status = PH_ERR_SUCCESS;
        }
    }

    /* Exchange was successfull */
    if (status == PH_ERR_SUCCESS)
    {
        /* Retrieve valid bits of last byte */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RXBITCTRL, &bRegister));

        /* Mask out valid bits of last byte */
        bRegister &= PHHAL_HW_RC663_MASK_RXLASTBITS;

        /* Set RxLastBits */
        pDataParams->wAdditionalInfo = bRegister;

        /* Set incomplete byte status if applicable */
        if (bRegister != 0x00U)
        {
            status = PH_ERR_SUCCESS_INCOMPLETE_BYTE;
        }
    }

    /* Stop the command / flush the FIFO on error */
    if ((status != PH_ERR_SUCCESS) &&
        (status != PH_ERR_SUCCESS_INCOMPLETE_BYTE))
    {
        /* stop the command */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_IDLE));

        /* Flush FiFo */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));
    }

    /* Store received data length in dataparams */
    pDataParams->wRxBufLen = pDataParams->wRxBufStartPos + wTmpBufferLen;

    /* Remove Start Byte (0xF0) during communication at Passive 106kbps during NFCIP exchange. */
    if ((pDataParams->bNfcipMode == PH_ON) &&
        (status == PH_ERR_SUCCESS))
    {
        /* Read the Shadow and Card Type if it configured at 106kbps Type-A */
        if ((pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING] == PHHAL_HW_RF_DATARATE_106) &&
            (pDataParams->bCardType == PHHAL_HW_CARDTYPE_ISO14443A))
        {
            if(pDataParams->pRxBuffer[0] == PHHAL_HW_RF_TYPE_A_START_BYTE)
            {
                pTmpBuffer[0] = bSbBackup;
                wSBIndex = 1;
                pDataParams->wRxBufLen--;
            }
            else
            {
                status = PH_ERR_IO_TIMEOUT;
            }
        }
    }

    /* Return RxBuffer pointer */
    if (ppRxBuffer != NULL)
    {
        *ppRxBuffer = pDataParams->pRxBuffer + wSBIndex;
    }

    /* Return RxBuffer length */
    if (pRxLength != NULL)
    {
        *pRxLength = pDataParams->wRxBufLen;
    }

    /* Timing */
    if ((pDataParams->wTimingMode & (uint16_t)~(uint16_t)PHHAL_HW_TIMING_MODE_OPTION_MASK) == PHHAL_HW_TIMING_MODE_FDT)
    {
        /* Calculate FDT Value */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetFdt(pDataParams, status, &dwTimingSingle));

        /* FDT single -> replace the previous value */
        if (0U != (pDataParams->wTimingMode & PHHAL_HW_TIMING_MODE_OPTION_AUTOCLEAR))
        {
            pDataParams->dwTimingUs = dwTimingSingle;
        }
        /* FDT -> add current value to current count */
        else
        {
            /* Check for overflow */
            if ((0xFFFFFFFFU - pDataParams->dwTimingUs) < dwTimingSingle)
            {
                pDataParams->dwTimingUs = 0xFFFFFFFFU;
            }
            /* Update global timing value */
            else
            {
                pDataParams->dwTimingUs = pDataParams->dwTimingUs + dwTimingSingle;
            }
        }
    }

    /* Restore RxWait setting */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXWAIT, bRegRxWait));

    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}

phStatus_t phhalHw_Rc663_ApplyProtocolSettings(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint8_t bMode
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_COUNT wIndex;
    const uint16_t *  PH_MEMLOC_REM pShadowDefault;
    uint16_t    PH_MEMLOC_REM wShadowCount;
    uint8_t     PH_MEMLOC_REM bUseDefaultShadow;
    uint16_t    PH_MEMLOC_REM wConfig;

    /* Disable the EMD. */
    pDataParams->bCheckEmdErr = PH_OFF;

    /* Store new card type */
    if (bMode != PHHAL_HW_CARDTYPE_CURRENT)
    {
        pDataParams->bCardType = bMode;
        pDataParams->bTimeoutUnit = PHHAL_HW_TIME_MICROSECONDS;
        bUseDefaultShadow = 1;
        pDataParams->bNfcipMode = PH_OFF;
    }
    else
    {
        bUseDefaultShadow = 0;
    }

    /* apply default setting */
    pDataParams->bJewelActivated = PH_OFF;
    pDataParams->bCRC5Enabled    = PH_OFF;

    /* Check the RC663 derivative support of the protocol that is been requested to be configured. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Int_ProtocolSupport(pDataParams, pDataParams->bCardType));

    /* configure reader IC for current card */
    switch (pDataParams->bCardType)
    {
        /* configure hardware for ISO 14443A */
    case PHHAL_HW_CARDTYPE_ISO14443A:
        if(pDataParams->bOpeMode != RD_LIB_MODE_ISO)
        {
            /* Use 14443a default shadow */
            SET_RC663_SHADOW(wRc663_Emvco_I14443a_DefaultShadow);

            /* Max EMD noise length is 2 bytes (excluding 2 bytes of CRC) */
            pDataParams->bEmdNoiseMaxDataLen = PHHAL_HW_RC663_EMD_NOISE_MAXLEN;
        }
        else
        {
            /* Use 14443a default shadow */
            SET_RC663_SHADOW(wRc663_DefaultShadow_I14443a);

            /* Max EMD noise length is 1 byte (excluding 2 bytes of CRC) */
            pDataParams->bEmdNoiseMaxDataLen = PHHAL_HW_RC663_EMD_NOISE_MAXLEN_ISO_MODE;
        }
        break;

        /* configure hardware for ISO 14443B */
    case PHHAL_HW_CARDTYPE_ISO14443B:

        if(pDataParams->bOpeMode != RD_LIB_MODE_ISO)
        {
            if (pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
            {
                /* Use 14443b EMVCo shadow */
                SET_RC663_SHADOW(wRc663_Emvco_I14443b_DefaultShadow);
            }
            else
            {
                /* Use 14443b NFC Forum shadow */
                SET_RC663_SHADOW(wRc663_NFC_I14443b_DefaultShadow);
            }

            /* Max EMD noise length is 2 bytes (excluding 2 bytes of CRC).
            * For Type B, RX last bits will not be updated in register. So
            * to manage noise error due to residual bits, max noise length
            * is set to 3 bytes (2u + 1 bytes). */
            pDataParams->bEmdNoiseMaxDataLen = PHHAL_HW_RC663_EMD_NOISE_MAXLEN + 1U;
        }
        else
        {
            /* Use 14443b shadow */
            SET_RC663_SHADOW(wRc663_DefaultShadow_I14443b);

            /* Max EMD noise length is 1 byte (excluding 2 bytes of CRC).
            * For Type B, RX last bits will not be updated in register. So
            * to manage noise error due to residual bits, max noise length
            * is set to 2 bytes (1u + 1 bytes). */
            pDataParams->bEmdNoiseMaxDataLen = PHHAL_HW_RC663_EMD_NOISE_MAXLEN_ISO_MODE + 1U;
        }
        break;

        /* configure hardware for FeliCa */
    case PHHAL_HW_CARDTYPE_FELICA_212:
        /* Use FeliCa shadow */
        SET_RC663_SHADOW(wRc663_DefaultShadow_Felica);
        break;

    case PHHAL_HW_CARDTYPE_FELICA_424:
        /* Use FeliCa shadow */
        SET_RC663_SHADOW(wRc663_DefaultShadow_Felica_424);
        break;

        /* configure hardware for ISO15693 */
    case PHHAL_HW_CARDTYPE_ISO15693:
        /* Use ISO15693 shadow */
        SET_RC663_SHADOW(wRc663_DefaultShadow_I15693);
        break;

        /* configure hardware for ICode EPC/UID */
    case PHHAL_HW_CARDTYPE_ICODEEPCUID:
        /* Use EPC/UID shadow */
        SET_RC663_SHADOW(wRc663_DefaultShadow_EpcUid);
        break;

        /* configure hardware for ISO18000 Part 3 Mode 3 */
    case PHHAL_HW_CARDTYPE_I18000P3M3:
        /* Use I18000P3M3 shadow */
        SET_RC663_SHADOW(wRc663_DefaultShadow_I18000p3m3);

        /* Set default start symbol */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_SYMBOL_START, PHHAL_HW_SYMBOL_I18000P3M3_FSYNC));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_EPCV2_Init(pDataParams));
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Configure generic IC settings */
    if (bMode != PHHAL_HW_CARDTYPE_CURRENT)
    {
        if (pDataParams->bPollGuardTimeFlag == PH_OFF)
        {
            /* Configure T0 */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                pDataParams,
                PHHAL_HW_RC663_REG_T0CONTROL,
                PHHAL_HW_RC663_BIT_TAUTORESTARTED | PHHAL_HW_RC663_BIT_TSTOP_RX | PHHAL_HW_RC663_BIT_TSTART_TX | PHHAL_HW_RC663_VALUE_TCLK_1356_MHZ));

            /* Configure T1 and cascade it with T0 */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                pDataParams,
                PHHAL_HW_RC663_REG_T1CONTROL,
                PHHAL_HW_RC663_BIT_TSTOP_RX | PHHAL_HW_RC663_BIT_TSTART_TX | PHHAL_HW_RC663_VALUE_TCLK_T0));
        }
        /* Configure T2 for LFO AutoTrimm */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_T2CONTROL,
            PHHAL_HW_RC663_BIT_TSTART_LFO | PHHAL_HW_RC663_VALUE_TCLK_1356_MHZ));

        /* T2 reload value for LFO AutoTrimm*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T2RELOADHI, 0x03U));
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T2RELOADLO, 0xFFU));

        /* Configure T3 (for LPCD/ AutoTrimm) */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_T3CONTROL,
            PHHAL_HW_RC663_VALUE_TCLK_1356_MHZ));

        /* Set FiFo-Size and Waterlevel */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig(
            pDataParams,
            PHHAL_HW_RC663_CONFIG_FIFOSIZE,
            (uint16_t)pDataParams->bFifoSize));

        /* Init. RxBitCtrl register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXBITCTRL, PHHAL_HW_RC663_BIT_VALUESAFTERCOLL));

        /* Init RxSOFD register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXSOFD, 0x00U));

    }

    /* Copy over default shadow contents into current shadow. */
    if (0U != (bUseDefaultShadow))
    {
        /* Initialize configure shadow */
        (void)memset(pDataParams->wCfgShadow, 0x00, PHHAL_HW_RC663_SHADOW_COUNT);

        for (wIndex = 0; wIndex < wShadowCount; ++wIndex)
        {
            wConfig = pShadowDefault[wIndex << 1U];
            pDataParams->wCfgShadow[wConfig] = pShadowDefault[(wIndex << 1U) + 1U];
        }
    }

    /* Evaluate hardware settings */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetCardMode(
        pDataParams,
        pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING],
        pDataParams->wCfgShadow[PHHAL_HW_CONFIG_RXDATARATE_FRAMING],
        pDataParams->wCfgShadow[PHHAL_HW_CONFIG_SUBCARRIER]));

    /* Apply shadowed registers */
    for (wIndex = 0; wIndex < wShadowCount; ++wIndex)
    {
        /* Get wConfig */
        wConfig = pShadowDefault[wIndex << 1U];

        switch (wConfig)
        {
            /* The following have been already set by phhalHw_Rc663_SetCardMode */
        case PHHAL_HW_CONFIG_TXDATARATE_FRAMING:
        case PHHAL_HW_CONFIG_RXDATARATE_FRAMING:
        case PHHAL_HW_CONFIG_SUBCARRIER:
        case PHHAL_HW_CONFIG_PARITY:
        case PHHAL_HW_CONFIG_TXCRC:
        case PHHAL_HW_CONFIG_RXCRC:
            break;
        case PHHAL_HW_CONFIG_TIMEOUT_VALUE_US:
            if (pDataParams->bTimeoutUnit == PHHAL_HW_TIME_MICROSECONDS)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, wConfig, pDataParams->wCfgShadow[wConfig]));
            }
            break;
        case PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS:
            if (pDataParams->bTimeoutUnit == PHHAL_HW_TIME_MILLISECONDS)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, wConfig, pDataParams->wCfgShadow[wConfig]));
            }
            break;
        default:
            /* LoadReg Mode */
            if (pDataParams->bLoadRegMode != PH_OFF)
            {
                switch (wConfig)
                {
                    /* Do not overwrite the following configurations */
                case PHHAL_HW_CONFIG_RXWAIT_US:
                case PHHAL_HW_CONFIG_TXWAIT_US:
                case PHHAL_HW_CONFIG_MODINDEX:
                case PHHAL_HW_CONFIG_ASK100:
                    break;
                    /* Apply all other configurations */
                default:
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, wConfig, pDataParams->wCfgShadow[wConfig]));
                    break;
                }
            }
            /* Default Mode, apply all configurations */
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, wConfig, pDataParams->wCfgShadow[wConfig]));
            }
            break;
        }
    }

    /* MIFARE Classic Crypto 1 state is disabled by default */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams, PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1, PH_ON));

    return PH_ERR_SUCCESS;
}


phStatus_t phhalHw_Rc663_SetConfigMultiple(
                                    phhalHw_Rc663_DataParams_t * pDataParams,
                                    uint16_t wNumOfConfigs,
                                    uint16_t (*arr2dConfig) [PHHAL_HW_ARRY_COLUMNS_SIZE]
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t PH_MEMLOC_REM  loopCount = 0;

    if (wNumOfConfigs == 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    for (loopCount = 0; loopCount < wNumOfConfigs; loopCount++ )
    {
        PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_Rc663_SetConfig(
            pDataParams,
            arr2dConfig[loopCount][0],
            arr2dConfig[loopCount][1]
            ));
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_SetConfig(
                                   phhalHw_Rc663_DataParams_t * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t wValue
                                   )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wDataRate;
    uint8_t     PH_MEMLOC_REM bValue;
    uint8_t     PH_MEMLOC_REM bRegister;
    uint8_t *   PH_MEMLOC_REM pBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wBufferLen = 0;
    uint16_t    PH_MEMLOC_REM wBufferSize;
    uint16_t    PH_MEMLOC_REM wFraming;
    uint8_t     PH_MEMLOC_REM wShadowCount;
    uint8_t     PH_MEMLOC_REM wIndex;
    uint16_t    PH_MEMLOC_REM wConfigShadow;
    const uint16_t *  PH_MEMLOC_REM pShadowDefault;
    uint8_t     PH_MEMLOC_REM bDerivative = 0;
    uint8_t     PH_MEMLOC_REM bVersion = 0;

    wShadowCount   = 0x00;
    wIndex         = 0x00;
    wConfigShadow  = 0x00;
    pShadowDefault = NULL;

    switch (wConfig)
    {
    /* Emvco: To Define Running Mode for RdLib: Either NFC, EMVCO, ISO */
    case PHHAL_HW_CONFIG_OPE_MODE:
        {
            pDataParams->bOpeMode = (uint8_t)wValue;
            break;
        }

    case PHHAL_HW_CONFIG_PARITY:
    case PHHAL_HW_CONFIG_TXCRC:
    case PHHAL_HW_CONFIG_RXCRC:
    case PHHAL_HW_CONFIG_TXLASTBITS:
    case PHHAL_HW_CONFIG_ASK100:

        if ((wConfig == PHHAL_HW_CONFIG_TXLASTBITS) || (wValue != pDataParams->wCfgShadow[wConfig]))
        {
            /* Use internal SetConfig */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig_Int(pDataParams, wConfig, wValue));

            /* Write configuration data into shadow */
            pDataParams->wCfgShadow[wConfig] = wValue;
        }
        break;

    case PHHAL_HW_CONFIG_SET_SYMBOL_SEND:
    case PHHAL_HW_CONFIG_TXWAIT_US:
    case PHHAL_HW_CONFIG_RXWAIT_US:
    case PHHAL_HW_CONFIG_MODINDEX:
    case PHHAL_HW_CONFIG_SUBCARRIER:

        /* Use internal SetConfig */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig_Int(pDataParams, wConfig, wValue));
        break;

    case PHHAL_HW_CONFIG_RXALIGN:

        /* check parameter */
        if (wValue > (PHHAL_HW_RC663_MASK_RXALIGN >> 4U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RXBITCTRL, &bRegister));
        bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_RC663_MASK_RXALIGN;
        bValue |= (uint8_t)(wValue << 4U);

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXBITCTRL, bValue));
        }

        /* Write configuration data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_TXWAIT_MS:

        /* Set TxWait */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Wait(pDataParams, PHHAL_HW_TIME_MILLISECONDS, wValue));
        break;

    case PHHAL_HW_CONFIG_TXDATARATE_FRAMING:
        wFraming = wValue & PHHAL_HW_RF_FRAMING_OPTION_MASK;
        wValue = wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK;

        switch (pDataParams->bCardType)
        {
            case PHHAL_HW_CARDTYPE_ISO14443A:
                if(wFraming == PHHAL_HW_RF_TYPE_F_FRAMING)
                {
                    if(wValue == PHHAL_HW_RF_DATARATE_212)
                    {
                        /* Use FeliCa shadow */
                        SET_RC663_SHADOW(wRc663_DefaultShadow_Felica);

                        /* Update Card Type */
                        pDataParams->bCardType = PHHAL_HW_CARDTYPE_FELICA_212;
                    }
                    else if(wValue == PHHAL_HW_RF_DATARATE_424)
                    {
                        /* Use FeliCa shadow */
                        SET_RC663_SHADOW(wRc663_DefaultShadow_Felica_424);

                        /* Update Card Type */
                        pDataParams->bCardType = PHHAL_HW_CARDTYPE_FELICA_424;
                    }
                    else
                    {
                        /* Do Nothing */
                    }
                }
                break;

            case PHHAL_HW_CARDTYPE_FELICA_212:
                if((wFraming == PHHAL_HW_RF_TYPE_F_FRAMING) &&
                    (wValue == PHHAL_HW_RF_DATARATE_424))
                {
                    /* Use FeliCa shadow */
                    SET_RC663_SHADOW(wRc663_DefaultShadow_Felica_424);

                    /* Update Card Type */
                    pDataParams->bCardType = PHHAL_HW_CARDTYPE_FELICA_424;
                }
                else if(wValue == PHHAL_HW_RF_DATARATE_106)
                {
                    /* Use ISO14443 shadow */
                    SET_RC663_SHADOW(wRc663_DefaultShadow_NfcDep);

                    /* Update Card Type */
                    pDataParams->bCardType = PHHAL_HW_CARDTYPE_ISO14443A;
                }
                else
                {
                    /* Do Nothing */
                }
                break;

            case PHHAL_HW_CARDTYPE_FELICA_424:
                if((wFraming == PHHAL_HW_RF_TYPE_F_FRAMING) &&
                    (wValue == PHHAL_HW_RF_DATARATE_212))
                {
                    /* Use FeliCa shadow */
                    SET_RC663_SHADOW(wRc663_DefaultShadow_Felica);

                    /* Update Card Type */
                    pDataParams->bCardType = PHHAL_HW_CARDTYPE_FELICA_212;
                }
                else if(wValue == PHHAL_HW_RF_DATARATE_106)
                {
                    /* Use ISO14443 shadow */
                    SET_RC663_SHADOW(wRc663_DefaultShadow_NfcDep);

                    /* Update Card Type */
                    pDataParams->bCardType = PHHAL_HW_CARDTYPE_ISO14443A;
                }
                else
                {
                    /* Do Nothing */
                }
                break;
            case PHHAL_HW_CARDTYPE_ISO15693:
                if(!(((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) == PHHAL_HW_RF_TX_DATARATE_1_OUT_OF_4) ||
                     ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) == PHHAL_HW_RF_TX_DATARATE_1_OUT_OF_256)))
                {
                    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_HAL);
                }
                break;
            /* Others */
            default:
                break;
        }

        if(wShadowCount > 0U)
        {
            for (wIndex = 0; wIndex < wShadowCount; ++wIndex)
            {
                wConfigShadow = pShadowDefault[wIndex << 1U];
                pDataParams->wCfgShadow[wConfigShadow] = pShadowDefault[(wIndex << 1U) + 1U];

                /* Apply configuration */
                if ((wConfigShadow != PHHAL_HW_CONFIG_TXDATARATE_FRAMING) &&
                    (wConfigShadow != PHHAL_HW_CONFIG_RXDATARATE_FRAMING) &&
                    (wConfigShadow != PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS) &&
                    (wConfigShadow != PHHAL_HW_CONFIG_TIMEOUT_VALUE_US))
                {
                    /* Use internal SetConfig */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig_Int(pDataParams, wConfigShadow, pShadowDefault[(wIndex << 1U) + 1U]));
                }
            }
        }

        if(wFraming != PHHAL_HW_RF_TYPE_F_FRAMING)
        {
            wDataRate = pDataParams->wCfgShadow[PHHAL_HW_CONFIG_RXDATARATE_FRAMING];
        }
        else
        {
            /* FeliCa card -> RxDataRate equals the new TxDataRate */
            wDataRate = wValue;
        }

        if ((wShadowCount > 0U) || (wValue != pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING]) ||
            (wDataRate != pDataParams->wCfgShadow[PHHAL_HW_CONFIG_RXDATARATE_FRAMING]))
        {
            /* Evaluate hardware settings */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetCardMode(
                pDataParams,
                wValue,
                wDataRate,
                pDataParams->wCfgShadow[PHHAL_HW_CONFIG_SUBCARRIER]));
        }

        /* FeliCa card -> Update RxDataRate in shadow */
        if ((pDataParams->bCardType == PHHAL_HW_CARDTYPE_FELICA_212) ||
            (pDataParams->bCardType == PHHAL_HW_CARDTYPE_FELICA_424))
        {
            pDataParams->wCfgShadow[PHHAL_HW_CONFIG_RXDATARATE_FRAMING] = wDataRate;
        }

        /* Write configuration data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_RXDATARATE_FRAMING:
        wFraming = wValue & PHHAL_HW_RF_FRAMING_OPTION_MASK;
        wValue = wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK;

        switch (pDataParams->bCardType)
        {
        case PHHAL_HW_CARDTYPE_ISO14443A:
            if(wFraming == PHHAL_HW_RF_TYPE_F_FRAMING)
            {
                if(wValue == PHHAL_HW_RF_DATARATE_212)
                {
                    /* Use FeliCa shadow */
                    SET_RC663_SHADOW(wRc663_DefaultShadow_Felica);

                    /* Update Card Type */
                    pDataParams->bCardType = PHHAL_HW_CARDTYPE_FELICA_212;
                }
                else if(wValue == PHHAL_HW_RF_DATARATE_424)
                {
                    /* Use FeliCa shadow */
                    SET_RC663_SHADOW(wRc663_DefaultShadow_Felica_424);

                    /* Update Card Type */
                    pDataParams->bCardType = PHHAL_HW_CARDTYPE_FELICA_424;
                }
                else
                {
                    /* Do Nothing */
                }
            }
            break;

        case PHHAL_HW_CARDTYPE_FELICA_212:
            if((wFraming == PHHAL_HW_RF_TYPE_F_FRAMING) &&
                (wValue == PHHAL_HW_RF_DATARATE_424))
            {
                /* Use FeliCa shadow */
                SET_RC663_SHADOW(wRc663_DefaultShadow_Felica_424);

                /* Update Card Type */
                pDataParams->bCardType = PHHAL_HW_CARDTYPE_FELICA_424;
            }
            else if(wValue == PHHAL_HW_RF_DATARATE_106)
            {
                /* Use ISO14443 shadow */
                SET_RC663_SHADOW(wRc663_DefaultShadow_NfcDep);

                /* Update Card Type */
                pDataParams->bCardType = PHHAL_HW_CARDTYPE_ISO14443A;
            }
            else
            {
                /* Do Nothing */
            }
            break;

        case PHHAL_HW_CARDTYPE_FELICA_424:
            if((wFraming == PHHAL_HW_RF_TYPE_F_FRAMING) &&
                (wValue == PHHAL_HW_RF_DATARATE_212))
            {
                /* Use FeliCa shadow */
                SET_RC663_SHADOW(wRc663_DefaultShadow_Felica);

                /* Update Card Type */
                pDataParams->bCardType = PHHAL_HW_CARDTYPE_FELICA_212;
            }
            else if(wValue == PHHAL_HW_RF_DATARATE_106)
            {
                /* Use ISO14443 shadow */
                SET_RC663_SHADOW(wRc663_DefaultShadow_NfcDep);

                /* Update Card Type */
                pDataParams->bCardType = PHHAL_HW_CARDTYPE_ISO14443A;
            }
            else
            {
                /* Do Nothing */
            }
            break;
        case PHHAL_HW_CARDTYPE_ISO15693:
            if(((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) !=  PHHAL_HW_RF_RX_DATARATE_HIGH) &&
                    ((wValue & PHHAL_HW_RF_DATARATE_OPTION_MASK) !=  PHHAL_HW_RF_RX_DATARATE_FAST_HIGH))
            {
                return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_HAL);
            }
            break;
        /* Others */
        default:
            break;
        }

        if(wShadowCount > 0U)
        {
            for (wIndex = 0; wIndex < wShadowCount; ++wIndex)
            {
                wConfigShadow = pShadowDefault[wIndex << 1U];
                pDataParams->wCfgShadow[wConfigShadow] = pShadowDefault[(wIndex << 1U) + 1U];

                /* Apply configuration */
                if ((wConfigShadow != PHHAL_HW_CONFIG_TXDATARATE_FRAMING) &&
                    (wConfigShadow != PHHAL_HW_CONFIG_RXDATARATE_FRAMING) &&
                    (wConfigShadow != PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS) &&
                    (wConfigShadow != PHHAL_HW_CONFIG_TIMEOUT_VALUE_US))
                {
                    /* Use internal SetConfig */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig_Int(pDataParams, wConfigShadow, pShadowDefault[(wIndex << 1U) + 1U]));
                }
            }
        }

        if(wFraming != PHHAL_HW_RF_TYPE_F_FRAMING)
        {
            wDataRate = pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING];
        }
        else
        {
            /* FeliCa card -> RxDataRate equals the new TxDataRate */
            wDataRate = wValue;
        }

        if ((wShadowCount > 0U) || (wValue != pDataParams->wCfgShadow[PHHAL_HW_CONFIG_RXDATARATE_FRAMING]) ||
            (wDataRate != pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING]))
        {
            /* Evaluate hardware settings */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetCardMode(
                pDataParams,
                wDataRate,
                wValue,
                pDataParams->wCfgShadow[PHHAL_HW_CONFIG_SUBCARRIER]));
        }

        /* FeliCa card -> Update TxDataRate in shadow*/
        if ((pDataParams->bCardType == PHHAL_HW_CARDTYPE_FELICA_212) ||
            (pDataParams->bCardType == PHHAL_HW_CARDTYPE_FELICA_424))
        {
            pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING] = wDataRate;
        }

        /* Write configuration data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_US:
    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS:

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ConfigTimeout(pDataParams, wConfig, wValue));
        break;

    case PHHAL_HW_CONFIG_TIMING_MODE:

        /* Check supported option bits */
        switch (wValue & PHHAL_HW_TIMING_MODE_OPTION_MASK)
        {
        case PHHAL_HW_TIMING_MODE_OPTION_DEFAULT:
        case PHHAL_HW_TIMING_MODE_OPTION_AUTOCLEAR:
            break;
        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Check supported timing modes */
        switch (wValue & (uint16_t)~(uint16_t)PHHAL_HW_TIMING_MODE_OPTION_MASK)
        {
        case PHHAL_HW_TIMING_MODE_OFF:
        case PHHAL_HW_TIMING_MODE_FDT:
            pDataParams->dwTimingUs = 0;
            pDataParams->wTimingMode = wValue;
            break;
        case PHHAL_HW_TIMING_MODE_COMM:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_HAL);
        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        break;

    case PHHAL_HW_CONFIG_FIELD_OFF_TIME:

        /* Parameter Check */
        if (wValue == 0U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Store configuration data */
        pDataParams->wFieldOffTime = wValue;
        break;

    case PHHAL_HW_CONFIG_FIELD_RECOVERY_TIME:

        /* Store configuration data */
        pDataParams->wFieldRecoveryTime = wValue;
        break;

    case PHHAL_HW_CONFIG_SYMBOL_START:

        /* Parameter check */
        if ((pDataParams->bCardType != PHHAL_HW_CARDTYPE_ICODEEPCUID) &&
            (pDataParams->bCardType != PHHAL_HW_CARDTYPE_ISO15693) &&
            (pDataParams->bCardType != PHHAL_HW_CARDTYPE_I18000P3M3))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_HAL);
        }

        /* Parameter check #2 */
        switch (wValue)
        {
        case PHHAL_HW_SYMBOL_ICODEEPCUID_SSOF:
        case PHHAL_HW_SYMBOL_ICODEEPCUID_LSOF:
        case PHHAL_HW_SYMBOL_ICODEEPCUID_CS:
            if (pDataParams->bCardType != PHHAL_HW_CARDTYPE_ICODEEPCUID)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
            }
            break;
        case PHHAL_HW_SYMBOL_I15693_SOF:
            if (pDataParams->bCardType != PHHAL_HW_CARDTYPE_ISO15693)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
            }
            break;
        case PHHAL_HW_SYMBOL_I18000P3M3_PREAMBLE:
        case PHHAL_HW_SYMBOL_I18000P3M3_FSYNC:
            if (pDataParams->bCardType != PHHAL_HW_CARDTYPE_I18000P3M3)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
            }
            break;
        case PH_OFF:
            break;
        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_HAL);
        }

        /* Store Start-Symbol */
        pDataParams->bSymbolStart = (uint8_t)wValue;

        /* Read out TxFrameCon register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_FRAMECON, &bRegister));
        bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_RC663_MASK_STARTSYM;

        /* Set new symbol */
        switch (pDataParams->bSymbolStart)
        {
        case PHHAL_HW_SYMBOL_ICODEEPCUID_SSOF:
            bValue |= PHHAL_HW_RC663_VALUE_START_SYM2;
            break;
        case PHHAL_HW_SYMBOL_ICODEEPCUID_LSOF:
            bValue |= PHHAL_HW_RC663_VALUE_START_SYM0;
            break;
        case PHHAL_HW_SYMBOL_ICODEEPCUID_CS:
            bValue |= PHHAL_HW_RC663_VALUE_START_SYM1;
            break;
        case PHHAL_HW_SYMBOL_I15693_SOF:
            bValue |= PHHAL_HW_RC663_VALUE_START_SYM2;
            break;
        case PHHAL_HW_SYMBOL_I18000P3M3_PREAMBLE:
            bValue |= PHHAL_HW_RC663_VALUE_START_SYM0;
            break;
        case PHHAL_HW_SYMBOL_I18000P3M3_FSYNC:
            bValue |= PHHAL_HW_RC663_VALUE_START_SYM1;
            break;
        default:
            break;
        }

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_FRAMECON, bValue));
        }
        break;

    case PHHAL_HW_CONFIG_SYMBOL_END:

        /* Parameter check */
        if (pDataParams->bCardType != PHHAL_HW_CARDTYPE_ICODEEPCUID)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_HAL);
        }

        /* Parameter check #2 */
        switch (wValue)
        {
        case PHHAL_HW_SYMBOL_ICODEEPCUID_CEOF:
        case PH_OFF:
            break;
        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Store End-Symbol */
        pDataParams->bSymbolEnd = (uint8_t)wValue;

        /* Read out TxFrameCon register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_FRAMECON, &bRegister));
        bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_RC663_MASK_STOPSYM;

        /* Set new symbol */
        switch (pDataParams->bSymbolEnd)
        {
        case PHHAL_HW_SYMBOL_ICODEEPCUID_CEOF:
            bValue |= PHHAL_HW_RC663_VALUE_STOP_SYM3;
            break;
        default:
            break;
        }

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_FRAMECON, bValue));
        }
        break;

    case PHHAL_HW_CONFIG_CRCTYPE:

        bValue = 0;
        if(0U == (wValue & (PHHAL_HW_CRCTYPE_CRC5 | PHHAL_HW_CRCTYPE_CRC16 | PHHAL_HW_CRCOPTION_INVERTED)))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
        /* Enable CRC16 */
        if(0U != (wValue & PHHAL_HW_CRCTYPE_CRC16))
        {
            bValue = PHHAL_HW_RC663_MASK_CRCTYPE16;
        }

        /* in case of 18000P3M3 for CRC5 calculation we need preset value of 9 which is not supported by RC663
         * So flag is enable to perform software calculation of CRC5 in RC663 HAL
         */
        if((0U != (wValue & PHHAL_HW_CRCTYPE_CRC5)) && (pDataParams->bCardType == PHHAL_HW_CARDTYPE_I18000P3M3))
        {
            pDataParams->bCRC5Enabled = PH_ON;
            break;
        }

        if(0U != (wValue & PHHAL_HW_CRCOPTION_INVERTED))
        {
            bValue |= PHHAL_HW_RC663_BIT_CRCINVERT;
        }
        /* Set TXPresetVal I18000p3m3  for CRC16 preset 0xFFFFU is used */
        if ((pDataParams->bCardType == PHHAL_HW_CARDTYPE_I18000P3M3) &&
            (0U != (wValue & PHHAL_HW_CRCTYPE_CRC16)))
        {
            bValue |= PHHAL_HW_RC663_MASK_CRCPRESETVAL;
        }
        if(pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXCRC] == PH_ON)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TXCRCCON, &bRegister));
            /* update CRC value */
            bValue |= bRegister;
            /* Only perform the operation, if the new value is different */
            if (bValue != bRegister)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXCRCCON, bValue));
            }
        }
        if(pDataParams->wCfgShadow[PHHAL_HW_CONFIG_RXCRC] == PH_ON)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RXCRCCON, &bRegister));
            /* update CRC value */
            bValue |= bRegister;

            /* For CRC-16 disable Force CRC Write */
            if(0U != (wValue & PHHAL_HW_CRCTYPE_CRC16))
            {
                bValue &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_RXFORCECRCWRITE;
            }

            /* Only perform the operation, if the new value is different */
            if (bValue != bRegister)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXCRCCON, bValue));
            }
        }
        break;

    case PHHAL_HW_CONFIG_JEWEL_MODE:

        if((wValue != PH_ON) &&
            (wValue != PH_OFF))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Write configuration data into shadow */
        pDataParams->bJewelActivated = (uint8_t)wValue;
        break;

    case PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1:

        /* Disable crypto, enabling is not supported */
        if (wValue != PH_OFF)
        {
            /* Retrieve Status register */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_STATUS, &bRegister));

            /* Clear Crypto1On bit */
            bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_CRYPTO1ON;

            /* Only perform the operation, if the new value is different */
            if (bValue != bRegister)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_STATUS, bValue));
            }
        }
        break;

    case PHHAL_HW_CONFIG_TXBUFFER_OFFSET:

        /* Modify additional info parameter */
        pDataParams->wAdditionalInfo = wValue;

        if(pDataParams->bNfcipMode == PH_ON)
        {
            if ((pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING] == PHHAL_HW_RF_DATARATE_106) &&
                (pDataParams->bCardType == PHHAL_HW_CARDTYPE_ISO14443A))
            {
                pDataParams->wAdditionalInfo += 1U;
            }
        }
        break;

    case PHHAL_HW_CONFIG_RXBUFFER_STARTPOS:

        /* Retrieve RxBuffer parameters */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetRxBuffer(pDataParams, PH_ON, &pBuffer, &wBufferLen, &wBufferSize));

        /* Boundary check */
        if (wValue >= wBufferSize)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
        }

        /* Set start position */
        pDataParams->wRxBufStartPos = wValue;
        break;

    case PHHAL_HW_CONFIG_TXBUFFER_LENGTH:

        /* Retrieve TxBuffer parameters */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetTxBuffer(pDataParams, PH_ON, &pBuffer, &wBufferLen, &wBufferSize));

        if ((pDataParams->bNfcipMode == PH_ON) &&
            (wValue > 0U))
        {
            if ((pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING] == PHHAL_HW_RF_DATARATE_106) &&
                (pDataParams->bCardType == PHHAL_HW_CARDTYPE_ISO14443A))
            {
                wValue += 1U;
            }
        }

        /* Check parameter, must not exceed TxBufferSize */
        if (wValue > wBufferSize)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
        }

        /* set buffer length */
        pDataParams->wTxBufLen = wValue;
        break;

    case PHHAL_HW_CONFIG_TXBUFFER:

        /* Retrieve TxBuffer parameters */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetTxBuffer(pDataParams, PH_ON, &pBuffer, &wBufferLen, &wBufferSize));

        /* Check parameter, must not exceed TxBufferSize */
        if (pDataParams->wAdditionalInfo >= wBufferSize)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
        }

        /* Modify TxBuffer byte */
        pBuffer[pDataParams->wAdditionalInfo] = (uint8_t)wValue;
        break;

    case PHHAL_HW_CONFIG_MAX_PRECACHED_BYTES:

        /* Check parameter */
        if (((pDataParams->bFifoSize == PH_NXPNFCRDLIB_CONFIG_RC663_VALUE_FIFOSIZE_255) &&
            (wValue > 0xFFU)) ||
            ((pDataParams->bFifoSize == PH_NXPNFCRDLIB_CONFIG_RC663_VALUE_FIFOSIZE_512) &&
                (wValue > 0x200U)))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Store parameter*/
        pDataParams->wMaxPrecachedBytes = wValue;
        break;

    case PHHAL_HW_CONFIG_BAL_CONNECTION:

        /* parameter check */
        if (wValue > PHHAL_HW_BAL_CONNECTION_I2C)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        pDataParams->bBalConnectionType = (uint8_t)wValue;
        break;

    case PHHAL_HW_CONFIG_SERIAL_BITRATE:

        switch (wValue)
        {
        case PHHAL_HW_RS232_BITRATE_7200:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_7200;
            break;
        case PHHAL_HW_RS232_BITRATE_9600:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_9600;
            break;
        case PHHAL_HW_RS232_BITRATE_14400:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_14400;
            break;
        case PHHAL_HW_RS232_BITRATE_19200:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_19200;
            break;
        case PHHAL_HW_RS232_BITRATE_38400:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_38400;
            break;
        case PHHAL_HW_RS232_BITRATE_57600:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_57600;
            break;
        case PHHAL_HW_RS232_BITRATE_115200:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_115200;
            break;
        case PHHAL_HW_RS232_BITRATE_128000:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_128000;
            break;
        case PHHAL_HW_RS232_BITRATE_230400:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_230400;
            break;
        case PHHAL_HW_RS232_BITRATE_460800:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_460800;
            break;
        case PHHAL_HW_RS232_BITRATE_921600:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_921600;
            break;
        case PHHAL_HW_RS232_BITRATE_1228800:
            bRegister = PHHAL_HW_RC663_SERIALSPEED_1228800;
            break;
        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Set the register value */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_SERIALSPEED, bRegister));
        break;

    case PHHAL_HW_CONFIG_RFRESET_ON_TIMEOUT:

        if (wValue == PH_OFF)
        {
            pDataParams->bRfResetAfterTo = PH_OFF;
        }
        else
        {
            pDataParams->bRfResetAfterTo = PH_ON;
        }
        break;

    case PHHAL_HW_RC663_CONFIG_FIFOSIZE:

        /* configure the water level */
        if (wValue == PH_NXPNFCRDLIB_CONFIG_RC663_VALUE_FIFOSIZE_255)
        {
            /* Set 255 bytes and clear Water level High-Bit */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                pDataParams,
                PHHAL_HW_RC663_REG_FIFOCONTROL,
                PHHAL_HW_RC663_BIT_FIFOSIZE | PHHAL_HW_RC663_BIT_FLUSHFIFO));

            /* Also reset max. precached bytes. */
            pDataParams->wMaxPrecachedBytes = PH_NXPNFCRDLIB_CONFIG_RC663_PRECACHED_255BYTES;

        }
        else if (wValue == PH_NXPNFCRDLIB_CONFIG_RC663_VALUE_FIFOSIZE_512)
        {
            /* Set 512 bytes and Water level High-Bit */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                pDataParams,
                PHHAL_HW_RC663_REG_FIFOCONTROL,
                PHHAL_HW_RC663_BIT_FLUSHFIFO | PHHAL_HW_RC663_BIT_WATERLEVEL_HI));

            /* Also reset max. precached bytes. */
            pDataParams->wMaxPrecachedBytes = PH_NXPNFCRDLIB_CONFIG_RC663_PRECACHED_512BYTES;

        }
        else
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Set Waterlevel to FifoSize - 0x0A
         * HighAlertIrq = (FifoSize - FifoLength) <= WaterLevel
         *                (0xFFU - 0x0A)                  <= 0xF5U
         */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_WATERLEVEL,
             0xF5));

        /* Save Fifo-Size */
        pDataParams->bFifoSize = (uint8_t)wValue;
        break;

    case PHHAL_HW_RC663_CONFIG_LOADREG_MODE:

        if (wValue != PH_OFF)
        {
            /* Check use condition */
            if (pDataParams->pLoadRegConfig == NULL)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_HAL);
            }
            pDataParams->bLoadRegMode = PH_ON;
        }
        else
        {
            pDataParams->bLoadRegMode = PH_OFF;
        }
        break;

    case PHHAL_HW_CONFIG_SETMINFDT:

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetMinFDT(pDataParams, wValue));
        break;

    case PHHAL_HW_CONFIG_RXMULTIPLE:

            if (wValue == PH_ON)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp,
                    phhalHw_Rc663_ReadRegister(
                    pDataParams,
                    PHHAL_HW_RC663_REG_RXCTRL,
                    &bRegister));
                bRegister |= PHHAL_HW_RC663_BIT_RXMULTIPLE;

                PH_CHECK_SUCCESS_FCT(statusTmp,
                    phhalHw_Rc663_WriteRegister(
                    pDataParams,
                    PHHAL_HW_RC663_REG_RXCTRL,
                    bRegister
                    ));

                pDataParams->bRxMultiple = PH_ON;
            }
            else
            {
                pDataParams->bRxMultiple = PH_OFF;

                PH_CHECK_SUCCESS_FCT(statusTmp,
                    phhalHw_Rc663_ReadRegister(
                    pDataParams,
                    PHHAL_HW_RC663_REG_RXCTRL,
                    &bRegister));

                bRegister = bRegister & (uint8_t)(~(uint8_t)PHHAL_HW_RC663_BIT_RXMULTIPLE);
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                    pDataParams,
                    PHHAL_HW_RC663_REG_RXCTRL,
                    bRegister
                    ));
            }
        break;

    case PHHAL_HW_CONFIG_NFCIP_STARTBYTE:
        if((wValue != PH_ON) &&
            (wValue != PH_OFF))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
        pDataParams->bNfcipMode = (uint8_t)wValue;
        break;

    case PHHAL_HW_CONFIG_SET_EMD:
        if((wValue != PH_ON) &&
            (wValue != PH_OFF))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
        pDataParams->bCheckEmdErr = (uint8_t)wValue;
        break;

    case PHHAL_HW_CONFIG_POLL_GUARD_TIME_US:
        /* Disable Poll guard time. */
        if (wValue == 0x00U)
        {
            return PH_ERR_SUCCESS;
        }

        /* Set the Poll guard time flag. */
        pDataParams->bPollGuardTimeFlag = PH_ON;

        /*
         * Time to wait is inaccurate if applyprotocolsettings is not done previously. Also configure T0 and T1 to handle FWT/FDT by default.
         * T0CONTROL should be set to 0x98 - after running down to zero, it will restart and starts after Tx and stops after receiving 4bits.
         * T1CONTROL should be set to 0x92 - starts after Tx and stops after receiving 4bits and timer input clock is an underflow of Timer0.
         */
        if (pDataParams->bCardType == PHHAL_HW_CARDTYPE_UNKNOWN)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_T0CONTROL, &bRegister));
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T0CONTROL,
                bRegister | PHHAL_HW_RC663_BIT_TAUTORESTARTED | PHHAL_HW_RC663_BIT_TSTOP_RX | PHHAL_HW_RC663_BIT_TSTART_TX | PHHAL_HW_RC663_VALUE_TCLK_1356_MHZ));

            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_T1CONTROL, &bRegister));
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T1CONTROL,
                bRegister | PHHAL_HW_RC663_BIT_TSTOP_RX | PHHAL_HW_RC663_BIT_TSTART_TX | PHHAL_HW_RC663_VALUE_TCLK_T0));
        }

        /* Configure and start timer. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Wait(pDataParams, PHHAL_HW_TIME_MICROSECONDS, wValue));
        break;

    case PHHAL_HW_CONFIG_RFCA:
        break;

    case PHHAL_HW_RC663_CONFIG_WATERLEVEL:
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_WATERLEVEL,
            (uint8_t)wValue));
        break;

    case PHHAL_HW_RC663_CONFIG_LPCD_CHARGEPUMP:
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Int_ReadDerivative(pDataParams, &bDerivative, &bVersion));

        if (bVersion == PHHAL_HW_CLRC663PLUS_VERSION)
        {
            if (wValue == PH_ON)
            {
                pDataParams->bLpcdOption |= PHHAL_HW_RC663_BIT_LPCD_CHARGEPUMP;
            }
            else if (wValue == PH_OFF)
            {
                pDataParams->bLpcdOption &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_LPCD_CHARGEPUMP;
            }
            else
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
            }
        }
        else
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_HAL);
        }
        break;

    case PHHAL_HW_RC663_CONFIG_LPCD_FILTER:
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Int_ReadDerivative(pDataParams, &bDerivative, &bVersion));

        if (bVersion == PHHAL_HW_CLRC663PLUS_VERSION)
        {
            if ((wValue & PHHAL_HW_RC663_FILTER_MASK) == PH_ON)
            {
                pDataParams->bLpcdOption |= PHHAL_HW_RC663_BIT_LPCD_FILTER;

                /* Higher nibble of bLpcdOption is used to store Option which is used to determine the configuration of I and Q values. */
                if ((wValue & PHHAL_HW_RC663_FILTER_OPTION_MASK) == PHHAL_HW_RC663_FILTER_OPTION1)
                {
                    pDataParams->bLpcdOption |= PHHAL_HW_RC663_FILTER_BIT_OPTION1;
                    pDataParams->bLpcdOption &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_FILTER_BIT_OPTION2;
                }
                else if ((wValue & PHHAL_HW_RC663_FILTER_OPTION_MASK) == PHHAL_HW_RC663_FILTER_OPTION2)
                {
                    pDataParams->bLpcdOption |= PHHAL_HW_RC663_FILTER_BIT_OPTION2;
                    pDataParams->bLpcdOption &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_FILTER_BIT_OPTION1;
                }
                else
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
                }
            }
            else if ((wValue & PHHAL_HW_RC663_FILTER_MASK) == PH_OFF)
            {
                pDataParams->bLpcdOption &= (uint8_t)~(uint8_t)(PHHAL_HW_RC663_BIT_LPCD_FILTER |
                    PHHAL_HW_RC663_FILTER_BIT_OPTION1 | PHHAL_HW_RC663_FILTER_BIT_OPTION2);
            }
            else
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
            }
        }
        else
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_HAL);
        }
        break;

#ifdef NXPBUILD__PHHAL_HW_RC663_CUSTOM_DEBUG
    case PHHAL_HW_RC663_CONFIG_TX_AMPLITUDE:

        /* Parameter masking */
        wValue &= PHHAL_HW_RC663_MASK_CW_AMPLITUDE;

        /* Read out TxAmp register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TXAMP, &bRegister));

        /* Set Tx Amplitude value */
        bValue = (bRegister & (uint8_t)~(uint8_t)PHHAL_HW_RC663_MASK_CW_AMPLITUDE) | (uint8_t)wValue;

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXAMP, bValue));
        }
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663_CUSTOM_DEBUG */

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_HAL);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_GetConfig(
                                   phhalHw_Rc663_DataParams_t * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t * pValue
                                   )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRegister;
    uint8_t *   PH_MEMLOC_REM pBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wBufferLen = 0;
    uint16_t    PH_MEMLOC_REM wBufferSize;

    switch (wConfig)
    {

    case PHHAL_HW_CONFIG_OPE_MODE:
        *pValue = pDataParams->bOpeMode;
        break;

    case PHHAL_HW_CONFIG_PARITY:
    case PHHAL_HW_CONFIG_TXCRC:
    case PHHAL_HW_CONFIG_RXCRC:
    case PHHAL_HW_CONFIG_TXLASTBITS:

        /* Read configuration from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_TXBUFFER_OFFSET:
    case PHHAL_HW_CONFIG_RXLASTBITS:

        *pValue = pDataParams->wAdditionalInfo;
        break;

    case PHHAL_HW_CONFIG_RXWAIT_US:
    case PHHAL_HW_CONFIG_TXWAIT_US:
    case PHHAL_HW_CONFIG_TXDATARATE_FRAMING:
    case PHHAL_HW_CONFIG_RXDATARATE_FRAMING:
    case PHHAL_HW_CONFIG_MODINDEX:
    case PHHAL_HW_CONFIG_ASK100:
        /* Read configuration from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_US:

        if (pDataParams->bTimeoutUnit == PHHAL_HW_TIME_MICROSECONDS)
        {
            *pValue = pDataParams->wCfgShadow[wConfig];
        }
        else
        {
            if (pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS] > (0xFFFFU / 1000U))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PARAMETER_OVERFLOW, PH_COMP_HAL);
            }
            *pValue = pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS] * 1000U;
        }
        break;

    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS:

        if (pDataParams->bTimeoutUnit == PHHAL_HW_TIME_MILLISECONDS)
        {
            *pValue = pDataParams->wCfgShadow[wConfig];
        }
        else
        {
            *pValue = pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_US] / 1000U;
        }
        break;

    case PHHAL_HW_CONFIG_TIMING_MODE:

        *pValue = pDataParams->wTimingMode;
        break;

    case PHHAL_HW_CONFIG_TIMING_US:

        if (pDataParams->dwTimingUs > 0xFFFFU)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PARAMETER_OVERFLOW, PH_COMP_HAL);
        }

        *pValue = (uint16_t)pDataParams->dwTimingUs;
        pDataParams->dwTimingUs = 0;
        break;

    case PHHAL_HW_CONFIG_TIMING_MS:

        if (pDataParams->dwTimingUs > (0xFFFFU * 1000U))
        {
            pDataParams->dwTimingUs = 0;
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PARAMETER_OVERFLOW, PH_COMP_HAL);
        }

        *pValue = (uint16_t)(pDataParams->dwTimingUs / 1000U);
        pDataParams->dwTimingUs = 0;
        break;

    case PHHAL_HW_CONFIG_SUBCARRIER:

        /* Read configuration from shadow */
        *pValue = pDataParams->wCfgShadow[wConfig];
        break;

    case PHHAL_HW_CONFIG_FIELD_OFF_TIME:

        *pValue = pDataParams->wFieldOffTime;
        break;

    case PHHAL_HW_CONFIG_FIELD_RECOVERY_TIME:

        *pValue = pDataParams->wFieldRecoveryTime;
        break;

    case PHHAL_HW_CONFIG_SYMBOL_START:

        /* Return parameter */
        *pValue = pDataParams->bSymbolStart;
        break;

    case PHHAL_HW_CONFIG_SYMBOL_END:

        /* Return parameter */
        *pValue = pDataParams->bSymbolEnd;
        break;
    case PHHAL_HW_CONFIG_JEWEL_MODE:
        /* Read configuration from shadow */
        *pValue = pDataParams->bJewelActivated;
        break;

    case PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1:

        /* Retrieve Status register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_STATUS, &bRegister));

        /* Check Crypto1On bit */
        if (0U != (bRegister & PHHAL_HW_RC663_BIT_CRYPTO1ON))
        {
            *pValue = PH_OFF; /* OFF in this case means "Crypto1 not disabled --> enabled" */
        }
        else
        {
            *pValue = PH_ON; /* ON in this case means "Crypto1 is disabled" */
        }
        break;

    case PHHAL_HW_CONFIG_RXBUFFER_STARTPOS:

        /* Return parameter */
        *pValue = pDataParams->wRxBufStartPos;
        break;

    case PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE:

        /* Retrieve RxBuffer parameters */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetRxBuffer(pDataParams, PH_ON, &pBuffer, &wBufferLen, &wBufferSize));

        /* Return parameter */
        *pValue = wBufferSize;
        break;

    case PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE:

        /* Retrieve TxBuffer parameters */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetTxBuffer(pDataParams, PH_ON, &pBuffer, &wBufferLen, &wBufferSize));

        /* In case of 18092 communication in Passive Type-A at 106kbps then Start Byte '0xF0' is handled in HAL and buffer length should be decremented by 1. */
        if (pDataParams->bNfcipMode == PH_ON)
        {
            /* Read the TxMode Register to read the BaudRate */
            if ((pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING] == PHHAL_HW_RF_DATARATE_106) &&
                (pDataParams->bCardType == PHHAL_HW_CARDTYPE_ISO14443A) &&
                (wBufferSize > 0U))
            {
                wBufferSize--;
            }
        }

        /* Return parameter */
        *pValue = wBufferSize;
        break;

    case PHHAL_HW_CONFIG_TXBUFFER_LENGTH:

        /* Retrieve TxBuffer parameters */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetTxBuffer(pDataParams, PH_ON, &pBuffer, &wBufferLen, &wBufferSize));

        /* Return parameter */
        *pValue = wBufferLen;

        if ((pDataParams->bNfcipMode == PH_ON) &&
            (*pValue != 0U))
        {
            if ((pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE_FRAMING] == PHHAL_HW_RF_DATARATE_106) &&
                (pDataParams->bCardType == PHHAL_HW_CARDTYPE_ISO14443A))
            {
                *pValue -= 1U;
            }
        }
        break;

    case PHHAL_HW_CONFIG_TXBUFFER:

        /* Retrieve TxBuffer parameters */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_GetTxBuffer(pDataParams, PH_ON, &pBuffer, &wBufferLen, &wBufferSize));

        /* Check additional info parameter */
        if (pDataParams->wAdditionalInfo >= wBufferSize)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
        }

        /* Retrieve TxBuffer byte */
        *pValue = (uint16_t)pBuffer[pDataParams->wAdditionalInfo];
        break;

    case PHHAL_HW_CONFIG_MAX_PRECACHED_BYTES:

        /* Return parameter */
        *pValue = pDataParams->wMaxPrecachedBytes;
        break;

    case PHHAL_HW_CONFIG_BAL_CONNECTION:

        /* Return parameter */
        *pValue = (uint16_t)pDataParams->bBalConnectionType;
        break;

    case PHHAL_HW_CONFIG_SERIAL_BITRATE:

        /* Read the register value */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_SERIALSPEED, &bRegister));

        switch (bRegister)
        {
        case PHHAL_HW_RC663_SERIALSPEED_7200:
            *pValue = PHHAL_HW_RS232_BITRATE_7200;
            break;
        case PHHAL_HW_RC663_SERIALSPEED_9600:
            *pValue = PHHAL_HW_RS232_BITRATE_9600;
            break;
        case PHHAL_HW_RC663_SERIALSPEED_14400:
            *pValue = PHHAL_HW_RS232_BITRATE_14400;
            break;
        case PHHAL_HW_RC663_SERIALSPEED_19200:
            *pValue = PHHAL_HW_RS232_BITRATE_19200;
            break;
        case PHHAL_HW_RC663_SERIALSPEED_38400:
            *pValue = PHHAL_HW_RS232_BITRATE_38400;
            break;
        case PHHAL_HW_RC663_SERIALSPEED_57600:
            *pValue = PHHAL_HW_RS232_BITRATE_57600;
            break;
        case PHHAL_HW_RC663_SERIALSPEED_115200:
            *pValue = PHHAL_HW_RS232_BITRATE_115200;
            break;
        case PHHAL_HW_RC663_SERIALSPEED_128000:
            *pValue = PHHAL_HW_RS232_BITRATE_128000;
            break;
        case PHHAL_HW_RC663_SERIALSPEED_230400:
            *pValue = PHHAL_HW_RS232_BITRATE_230400;
            break;
        case PHHAL_HW_RC663_SERIALSPEED_460800:
            *pValue = PHHAL_HW_RS232_BITRATE_460800;
            break;
        case PHHAL_HW_RC663_SERIALSPEED_921600:
            *pValue = PHHAL_HW_RS232_BITRATE_921600;
            break;
        case PHHAL_HW_RC663_SERIALSPEED_1228800:
            *pValue = PHHAL_HW_RS232_BITRATE_1228800;
            break;
        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
        }
        break;

    case PHHAL_HW_CONFIG_RFRESET_ON_TIMEOUT:

        /* Return parameter */
        *pValue = (uint16_t)pDataParams->bRfResetAfterTo;
        break;

    case PHHAL_HW_RC663_CONFIG_FIFOSIZE:

        /* Return parameter */
        *pValue = (uint16_t)pDataParams->bFifoSize;
        break;

    case PHHAL_HW_RC663_CONFIG_LOADREG_MODE:

        /* Return parameter */
        *pValue = (uint16_t)pDataParams->bLoadRegMode;
        break;

    case PHHAL_HW_CONFIG_NFCIP_STARTBYTE:

        /* Return parameter */
        *pValue = pDataParams->bNfcipMode;
        break;

    case PHHAL_HW_CONFIG_ACTIVEMODE:

        /* Return PH_OFF, as this is not applicable in-case of Rc663. */
        *pValue = PH_OFF;
        break;

    case PHHAL_HW_CONFIG_RXMULTIPLE:
        *pValue = (uint16_t)pDataParams->bRxMultiple;
        break;

    case PHHAL_HW_CONFIG_SET_EMD:
        *pValue = (uint16_t)pDataParams->bCheckEmdErr;
        break;

    case PHHAL_HW_CONFIG_CARD_TYPE:
        *pValue = pDataParams->bCardType;
        break;

    case PHHAL_HW_CONFIG_INT_RF_ON:
        /* Read out DrvMode register to check if internal RF is ON. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_DRVMODE,
            &bRegister));
        if(0U != (bRegister & PHHAL_HW_RC663_BIT_TXEN))
        {
            *pValue = PH_ON;
        }
        else
        {
            *pValue = PH_OFF;
        }
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_HAL);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_FieldOn(
                                 phhalHw_Rc663_DataParams_t * pDataParams
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRegister;

    /* Read out DrvMode register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_DRVMODE, &bRegister));

    /* Enable Drivers */
    bRegister |= PHHAL_HW_RC663_BIT_TXEN;

    /* Write DrvMode register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_DRVMODE, bRegister));

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_FieldOff(
                                  phhalHw_Rc663_DataParams_t * pDataParams
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRegister;

    /* Disable the EMD. */
    pDataParams->bCheckEmdErr = PH_OFF;

    /* Read out DrvMode register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_DRVMODE, &bRegister));

    /* Disable Drivers */
    bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_TXEN;

    /* Write DrvMode register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_DRVMODE, bRegister));

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_FieldReset(
                                    phhalHw_Rc663_DataParams_t * pDataParams
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;

    /* Switch off the field */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FieldOff(pDataParams));

    /* wait for field-off timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Wait(
        pDataParams,
        PHHAL_HW_TIME_MILLISECONDS,
        pDataParams->wFieldOffTime));

    /* switch on the field again */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FieldOn(pDataParams));

    /* wait for field-recovery timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Wait(
        pDataParams,
        PHHAL_HW_TIME_MILLISECONDS,
        pDataParams->wFieldRecoveryTime));

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_Wait(
                              phhalHw_Rc663_DataParams_t * pDataParams,
                              uint8_t bUnit,
                              uint16_t wTimeout
                              )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bControlReg;
    uint8_t     PH_MEMLOC_REM bT0Ctrl;
    uint8_t     PH_MEMLOC_REM bT1Ctrl;
    uint8_t     PH_MEMLOC_REM bIrqRq;

    /* Parameter check */
    if ((bUnit != PHHAL_HW_TIME_MICROSECONDS) &&
        (bUnit != PHHAL_HW_TIME_MILLISECONDS))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /*
     * Time to wait is inaccurate if applyprotocolsettings is not done previously.
     * T0CONTROL should be set to 0x08 - meaning after running down to zero, it will restart.
     * T1CONTROL should be set to 0x02 - meaning The timer input clock is an underflow of Timer0
     */
    if ((pDataParams->bCardType == PHHAL_HW_CARDTYPE_UNKNOWN) &&
        (pDataParams->bPollGuardTimeFlag == PH_OFF))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_T0CONTROL, &bT0Ctrl));
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T0CONTROL, bT0Ctrl | PHHAL_HW_RC663_BIT_TAUTORESTARTED | PHHAL_HW_RC663_VALUE_TCLK_1356_MHZ));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_T1CONTROL, &bT1Ctrl));
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T1CONTROL, bT1Ctrl | PHHAL_HW_RC663_VALUE_TCLK_T0));
    }

    /* Set temporary timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetFdt(pDataParams, bUnit, wTimeout));

    /* retrieve content of Timer Control register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, &bControlReg));

    /* clear all irq flags */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1, (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_SET));

    /* manually start T0 */
    bControlReg |= PHHAL_HW_RC663_BIT_T0STARTSTOPNOW | PHHAL_HW_RC663_BIT_T0RUNNING;
    /* manually start T1 */
    bControlReg |= PHHAL_HW_RC663_BIT_T1STARTSTOPNOW | PHHAL_HW_RC663_BIT_T1RUNNING;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, bControlReg));

    /* If a Setconfig(PHHAL_HW_CONFIG_POLL_GUARD_TIME_US) is done then waiting for timer interrupt
     * should be done in phhalHw_Exchange() API. */
    if (pDataParams->bPollGuardTimeFlag == PH_OFF)
    {
        /* wait for timer interrupt */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WaitIrq(pDataParams, PH_ON, PH_OFF, 0x00, PHHAL_HW_RC663_BIT_TIMER1IRQ, NULL, &bIrqRq));

        /* manually stop T0 and T1 */
        bControlReg &= (uint8_t)~(uint8_t)(PHHAL_HW_RC663_BIT_T0RUNNING | PHHAL_HW_RC663_BIT_T1RUNNING);
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TCONTROL, bControlReg));

        /* Restore previous timeout */
        if (pDataParams->bCardType != PHHAL_HW_CARDTYPE_UNKNOWN)
        {
            if (pDataParams->bTimeoutUnit == PHHAL_HW_TIME_MICROSECONDS)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetFdt(
                    pDataParams,
                    PHHAL_HW_TIME_MICROSECONDS,
                    pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_US]));
            }
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetFdt(
                    pDataParams,
                    PHHAL_HW_TIME_MILLISECONDS,
                    pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS]));
            }
        }
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_MfcAuthenticateKeyNo(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNo,
    uint16_t wKeyVersion,
    uint8_t * pUid
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bKeyNoInt;

    /* check key version */
    if (wKeyVersion != 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* set keytype and keynumber */
    if (bKeyType == PHHAL_HW_MFC_KEYA)
    {
        /* check key number */
        if (wKeyNo >= (PHHAL_HW_RC663_MAX_NUM_KEYS >> 1U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
        bKeyNoInt = (uint8_t)(wKeyNo << 1U);
    }
    else if (bKeyType == PHHAL_HW_MFC_KEYB)
    {
        /* check key number */
        if (wKeyNo >= ((PHHAL_HW_RC663_MAX_NUM_KEYS >> 1U) - 1U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
        bKeyNoInt = (uint8_t)(wKeyNo << 1U) + 1U;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* load key from EEPROM */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Cmd_LoadKeyE2(
        pDataParams,
        bKeyNoInt));

    /* execute Authenticate command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_MfcAuthenticate_Int(
        pDataParams,
        bBlockNo,
        bKeyType,
        pUid));

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_MfcAuthenticate(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint8_t * pKey,
    uint8_t * pUid
    )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    /* load key directly */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Cmd_LoadKey(
        pDataParams,
        pKey));

    /* execute Authenticate command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_MfcAuthenticate_Int(
        pDataParams,
        bBlockNo,
        bKeyType,
        pUid));

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_SetMinFDT(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint16_t wValue
    )
{
    phStatus_t PH_MEMLOC_REM status = 0;
    uint16_t   PH_MEMLOC_REM wTimer = 0;
    uint16_t   PH_MEMLOC_REM wTxRate = 0;

    if (wValue == PH_ON)
    {
        /*Backup the old Timer values and set min FDT*/
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_GetConfig(pDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, &wTimer));
        pDataParams->dwFdtPc = wTimer;
        /* Calculate values for Microsecond values */
        /* Get the data rate */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_GetConfig(pDataParams,
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
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_ConfigTimeout(pDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US, wTimer));
    }
    else if (wValue == PH_OFF)
    {
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_ConfigTimeout(pDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, pDataParams->dwFdtPc));
    }
    else
    {
        status = (PH_ERR_INVALID_PARAMETER | PH_COMP_HAL);
    }
    return status;
}

phStatus_t phhalHw_Rc663_AsyncAbort(
                                    phhalHw_Rc663_DataParams_t * pDataParams
                                    )
{
#ifndef _WIN32
    phStatus_t  PH_MEMLOC_REM status = PH_ERR_SUCCESS;
    /* Post Abort Event. */
    PH_CHECK_SUCCESS_FCT(status, phOsal_EventPost(&pDataParams->HwEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_ABORT, NULL));
#else
    /* satisfy compiler */
    if (0U != (pDataParams));
#endif
    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_I18000p3m3Inventory(
                                             phhalHw_Rc663_DataParams_t  * pDataParams,
                                             uint8_t * pSelCmd,
                                             uint8_t bSelCmdLen,
                                             uint8_t bNumValidBitsinLastByte,
                                             uint8_t * pBeginRndCmd,
                                             uint8_t bTSprocessing,
                                             uint8_t ** ppRxBuffer,
                                             uint16_t * pRxBufferLen
                                             )
{
    phStatus_t PH_MEMLOC_REM status;
    uint16_t   PH_MEMLOC_REM wNumSlotsDec;
    uint8_t    PH_MEMLOC_REM *pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wRxBufferLen = 0;
    uint8_t    PH_MEMLOC_REM bRxLastBits = 0;
    uint8_t    PH_MEMLOC_REM bQ, bBeginSession, bSelTarget;

    *ppRxBuffer = pDataParams->pRxBuffer;
    *pRxBufferLen = 0;

    /* Reset RX Buffer start position */
    pDataParams->wRxBufStartPos = 0;

    /* Reset Current slot count (used for resume inventory) */
    pDataParams->wCurrentSlotCount = 0;

    if(pBeginRndCmd == NULL)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    bBeginSession = ((uint8_t)pBeginRndCmd[1] & 0x30U) >> 4U;

    if(pSelCmd != NULL)
    {
    bSelTarget = ((uint8_t)pSelCmd[0] & 0x0EU) >>1U;


    /* If pSelCmd -> Target is SL then BeginRound for both S0 and S2 session is valid.
     * If pSelCmd -> Target = S0 then BeginRound -> Session should be S0.
     * If pSelCmd -> Target = S2 then BeginRound -> Session should be S2.
     * For other cases return invalid parameter.
     * */
    if(bSelTarget != PHHAL_HW_I18000P3M3_TARGET_SL){

        if(((bBeginSession == PHHAL_HW_I18000P3M3_SESSION_S0) &&
            (bSelTarget == PHHAL_HW_I18000P3M3_TARGET_INVENTORIED_S2))||
                ((bBeginSession == PHHAL_HW_I18000P3M3_SESSION_S2) &&
                    (bSelTarget == PHHAL_HW_I18000P3M3_TARGET_INVENTORIED_S0)))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
    }
    }

    /* Extract bQ value from pBeginRndCmd */
    bQ = (uint8_t)((uint8_t)((uint8_t)pBeginRndCmd[1] & 0x7U) << 1U) | ((uint8_t)((uint8_t)pBeginRndCmd[2] & 0x80U) >> 7U);

    if(bTSprocessing != PHHAL_HW_I18000P3M3_GET_MAX_RESPS)
    {
        /* Perform SELECT */
        if((bSelCmdLen > 0U) &&
            (pSelCmd != NULL))
        {
            status = phhalHw_Rc663_18000p3m3_Sw_Select(
                    pDataParams,
                    pSelCmd,
                    bSelCmdLen,
                    bNumValidBitsinLastByte);

        }
    }

    /* Perform BEGIN ROUND */
    status = phhalHw_Rc663_18000p3m3_Sw_BeginRound(pDataParams, pBeginRndCmd);

    if(bTSprocessing == PHHAL_HW_I18000P3M3_ONE_TS_ONLY)
    {
        pDataParams->wRxBufStartPos = 3;

        /* If get response in TS return success */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Perform ACK */
            status = phhalHw_Rc663_18000p3m3_Sw_Ack(
                pDataParams,
                &pRxBuffer,
                &wRxBufferLen,
                &bRxLastBits);

            if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                /* prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = (uint8_t)wRxBufferLen;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = bRxLastBits;

                pDataParams->wRxBufStartPos += pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U];
            }
            else
            {
                /* prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = (uint8_t)status;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
            }
        }
        else if((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR)
        {
            /* prefix 3 control bytes for each response */
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 3;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
        }
        else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
        {
            /* prefix 3 control bytes for each response */
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 2;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
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
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 3;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
                }
                else
                {
                    /* prefix 3 control bytes for each response */
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 2;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
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
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 3;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
            }
            else if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                /* Perform ACK */
                status = phhalHw_Rc663_18000p3m3_Sw_Ack(
                    pDataParams,
                    &pRxBuffer,
                    &wRxBufferLen,
                    &bRxLastBits);

                if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                {
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 0;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = (uint8_t)wRxBufferLen;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = bRxLastBits;

                    pDataParams->wRxBufStartPos += pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U];

                    /* Last next slot command to set the inventoried flag of last card successfully acked */
                    if (wNumSlotsDec == 0U)
                    {
                        /* Send CloseSlot command */
                        (void)phhalHw_Rc663_18000p3m3_Sw_NextSlot(pDataParams);
                    }
                }
                else if (((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR) ||
                         ((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR))
                {
                    /* Prefix 3 control bytes for each response */
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 3;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;

                    /* Wait for 1210us to avoid the colliding cards going out of the resolution loop */
                    (void)phhalHw_Rc663_Wait(pDataParams, PHHAL_HW_TIME_MICROSECONDS, 1210);
                }
                else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
                {
                    /* prefix 3 control bytes for each response */
                    /* Timeout as reply to ACK to be treated as collision as the card was there but not successfully counted */
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 3;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
                    pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
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
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 2;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
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
                status = phhalHw_Rc663_18000p3m3_Sw_NextSlot(pDataParams);
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
            status = phhalHw_Rc663_18000p3m3_Sw_Ack(
                pDataParams,
                &pRxBuffer,
                &wRxBufferLen,
                &bRxLastBits);

            if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                /* prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = (uint8_t)wRxBufferLen;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = bRxLastBits;

                pDataParams->wRxBufStartPos += pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U];

                /* Send ReqRN for getting handle */
                status = phhalHw_Rc663_18000p3m3_Sw_ReqRn(
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
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 3;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
            }
            else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
            {
                /* prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 2;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
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
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 3;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
        }
        else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
        {
            /* prefix 3 control bytes for each response */
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 2;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
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

phStatus_t phhalHw_Rc663_I18000p3m3ResumeInventory(
                                                   phhalHw_Rc663_DataParams_t * pDataParams,
                                                   uint8_t ** ppRxBuffer,
                                                   uint16_t * pRxBufferLen
                                                   )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_SUCCESS;
    uint8_t    PH_MEMLOC_REM *pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wRxBufferLen = 0;
    uint8_t    PH_MEMLOC_REM bRxLastBits = 0;
    uint16_t   PH_MEMLOC_REM wNumSlotsDec;

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
        if((pDataParams->wRxBufStartPos + 75U) > pDataParams->wRxBufSize)
        {
            pDataParams->wCurrentSlotCount = wNumSlotsDec;
            *pRxBufferLen = pDataParams->wRxBufStartPos;
            pDataParams->wRxBufStartPos = 0;

            return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
        }

        /* Send NextSlot command */
        status = phhalHw_Rc663_18000p3m3_Sw_NextSlot(pDataParams);

        /* Increment buffer position to store control bytes for current slot */
        pDataParams->wRxBufStartPos += 3U;

        if (((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR) ||
            ((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR))
        {
            /* Prefix 3 control bytes for each response */
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 3;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
        }
        else if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Perform ACK */
            status = phhalHw_Rc663_18000p3m3_Sw_Ack(
                pDataParams,
                &pRxBuffer,
                &wRxBufferLen,
                &bRxLastBits);

            if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = (uint8_t)wRxBufferLen;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = bRxLastBits;

                pDataParams->wRxBufStartPos += pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U];

                /* Last next slot command to set the inventoried flag of last card successfully acked */
                if ((wNumSlotsDec == 0U) ||
                    ((pDataParams->wRxBufStartPos + 75U) > pDataParams->wRxBufSize))
                {
                    /* Send CloseSlot command */
                    (void)phhalHw_Rc663_18000p3m3_Sw_NextSlot(pDataParams);
                }

            }
            else if (((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR) ||
                     ((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR))
            {
                /* Prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 3;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;

                /* Wait for 1210us to avoid the colliding cards going out of the resolution loop */
                (void)phhalHw_Rc663_Wait(pDataParams, PHHAL_HW_TIME_MICROSECONDS, 1210);
            }
            else if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
            {
                /* ACK timeout to be treated as collision as a card was present in vicinity */
                /* prefix 3 control bytes for each response */
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 3;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
                pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
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
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 3U] = 2;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 2U] = 0;
            pDataParams->pRxBuffer[pDataParams->wRxBufStartPos - 1U] = 0;
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

phStatus_t phhalHw_Rc663_EventWait(phhalHw_Rc663_DataParams_t * pDataParams, uint32_t dwEventTimeout)
{
    return phOsal_EventPend((volatile phOsal_Event_t * )(&pDataParams->HwEventObj.EventHandle), E_OS_EVENT_OPT_PEND_SET_ANY,
        dwEventTimeout, (E_PH_OSAL_EVT_RF | E_PH_OSAL_EVT_ABORT), NULL);
}

phStatus_t phhalHw_Rc663_EventConsume(phhalHw_Rc663_DataParams_t * pDataParams)
{
    return phOsal_EventClear(&pDataParams->HwEventObj.EventHandle, E_OS_EVENT_OPT_NONE, (E_PH_OSAL_EVT_RF | E_PH_OSAL_EVT_ABORT), NULL);
}

phStatus_t phhalHw_Rc663_DeInit(
                                phhalHw_Rc663_DataParams_t * pDataParams
                                )
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_REM bRegister;

    PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0EN, &bRegister));
    /* Clear IRQ0 interrupt sources */
    bRegister = 0x00;
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0EN, bRegister));

    PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1EN, &bRegister));
    /* Clear IRQ1 interrupt sources */
    bRegister = 0x00;
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1EN, bRegister));

    /* De-Init. private data */
    pDataParams->wId                    = 0;
    pDataParams->pBalDataParams         = NULL;
    pDataParams->bLoadRegMode           = PH_ON;
    pDataParams->pLoadRegConfig         = NULL;
    pDataParams->pTxBuffer              = NULL;
    pDataParams->wTxBufSize             = 0;
    pDataParams->wTxBufLen              = 0;
    pDataParams->pRxBuffer              = NULL;
    pDataParams->wRxBufSize             = 0;
    pDataParams->wRxBufLen              = 0;
    pDataParams->wRxBufStartPos         = 0;
    pDataParams->bCardType              = PHHAL_HW_CARDTYPE_UNKNOWN;
    pDataParams->wTimingMode            = PHHAL_HW_TIMING_MODE_OFF;
    pDataParams->bTimeoutUnit           = PHHAL_HW_TIME_MICROSECONDS;
    pDataParams->dwTimingUs             = 0;
    pDataParams->wFieldOffTime          = PHHAL_HW_FIELD_OFF_DEFAULT;
    pDataParams->wFieldRecoveryTime     = PHHAL_HW_FIELD_RECOVERY_DEFAULT;
    pDataParams->bSymbolStart           = PH_OFF;
    pDataParams->bSymbolEnd             = PH_OFF;
    pDataParams->bFifoSize              = PH_NXPNFCRDLIB_CONFIG_RC663_DEFAULT_FIFOSIZE;
    pDataParams->wMaxPrecachedBytes     = PH_NXPNFCRDLIB_CONFIG_RC663_DEFAULT_PRECACHED_BYTES;
    pDataParams->wAdditionalInfo        = 0;
    pDataParams->bRfResetAfterTo        = PH_OFF;
    pDataParams->bMode                  = PHHAL_HW_RC663_CMD_LPCD_MODE_DEFAULT;
    pDataParams->bCRC5Enabled           = PH_OFF;
    pDataParams->bOpeMode               = RD_LIB_MODE_NFC;
    pDataParams->bRxMultiple            = PH_OFF;
    pDataParams->bNfcipMode             = PH_OFF;
    pDataParams->bCheckEmdErr           = PH_OFF;
    pDataParams->wCurrentSlotCount      = 0;
    pDataParams->bSession               = 0;
    pDataParams->bPollGuardTimeFlag     = PH_OFF;
    pDataParams->pRFISRCallback         = NULL;
    pDataParams->wDerivative            = 0;
    /* Disable Charge Pump and Filter By Default. */
    pDataParams->bLpcdOption            = 0;

    /* Reset sub-carrier setting, needed for phhalHw_Rc663_SetCardMode */
    pDataParams->wCfgShadow[PHHAL_HW_CONFIG_SUBCARRIER] = PHHAL_HW_SUBCARRIER_SINGLE;

    /* Delete event */
    return phOsal_EventDelete(&pDataParams->HwEventObj.EventHandle);
}

static void phhalHw_Rc663_Reset(void)
{
#ifndef _WIN32
    /* Send the reset pulse to FE to reset. */
    phDriver_PinWrite(PHDRIVER_PIN_RESET, RESET_POWERUP_LEVEL);

    /* delay of ~2 ms */
    (void)phDriver_TimerStart(PH_DRIVER_TIMER_MILLI_SECS, PHHAL_HW_RC663_RESET_DELAY_MILLI_SECS, NULL);

    phDriver_PinWrite(PHDRIVER_PIN_RESET, RESET_POWERDOWN_LEVEL);

    /* delay of ~2 ms */
    (void)phDriver_TimerStart(PH_DRIVER_TIMER_MILLI_SECS, PHHAL_HW_RC663_RESET_DELAY_MILLI_SECS, NULL);

    phDriver_PinWrite(PHDRIVER_PIN_RESET, RESET_POWERUP_LEVEL);

    /* delay of ~2 ms */
    (void)phDriver_TimerStart(PH_DRIVER_TIMER_MILLI_SECS, PHHAL_HW_RC663_RESET_DELAY_MILLI_SECS, NULL);
#endif /* _WIN32 */
}

static phStatus_t phhalHw_Rc663_TransmitJewelCmd(
                                                 phhalHw_Rc663_DataParams_t * pDataParams,
                                                 uint16_t wJewelBufferLen,
                                                 uint8_t * pTmpBuffer
                                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp = PH_ERR_SUCCESS;
    uint16_t    PH_MEMLOC_REM wIndex = 0;
    uint16_t    PH_MEMLOC_REM wFifoBytes;
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor = 0U;
    uint8_t     PH_MEMLOC_REM bIrq1WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq0Reg = 0x00U;

    for(wIndex = 0U; wIndex < (wJewelBufferLen - 1U); wIndex++)
    {
        /* Terminate a possibly running command */
        PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_COMMAND,
            PHHAL_HW_RC663_CMD_IDLE));

        /* Flush FiFo */
        PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));

        /* clear all IRQ0 flags */
        PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_IRQ0,
            (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_SET));

        /* Perform actual write to FIFO */
        PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_WriteFifo(
            pDataParams,
            pTmpBuffer++,
            1U,
            &wFifoBytes));

        /* Set wait IRQs */
        bIrq0WaitFor |=  PHHAL_HW_RC663_BIT_TXIRQ | PHHAL_HW_RC663_BIT_IDLEIRQ;
        bIrq1WaitFor = 0U;


        /* Start the Transmit command */
        PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_COMMAND,
            PHHAL_HW_RC663_CMD_TRANSMIT));

        do
        {
#ifndef NXPBUILD__PH_PLATFORM_DELEGATE
            /* wait until the transmission is finished */
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_WaitIrq(
                pDataParams,
                PH_ON,
                PH_OFF,
                bIrq0WaitFor,
                bIrq1WaitFor,
                &bIrq0Reg,
                NULL));
#endif /* NXPBUILD__PH_PLATFORM_DELEGATE*/
        }while(0U == (bIrq0Reg & bIrq0WaitFor));

        /* Reset TxLastBits */
        if (pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS] != 0U)
        {
            PH_CHECK_FAILURE_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXLASTBITS, 0x00U));
        }
    }

    return(statusTmp);
}

#endif /* NXPBUILD__PHHAL_HW_RC663 */
