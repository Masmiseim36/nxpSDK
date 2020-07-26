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
* Command functions for Rc663 specific HAL-Component of Reader Library Framework.
* $Author: Purnank G (ing05193) $
* $Revision: 5076 $ (v06.01.00)
* $Date: 2016-06-13 17:29:09 +0530 (Mon, 13 Jun 2016) $
*
* History:
*  CHu: Generated 19. March 2010
*
*/
#include <phhalHw.h>
#include <phDriver.h>
#include <ph_RefDefs.h>
#include <phNxpNfcRdLib_Config.h>

#ifdef NXPBUILD__PHHAL_HW_RC663
#include <BoardSelection.h>

#include "phhalHw_Rc663.h"
#include "phhalHw_Rc663_Int.h"
#include "phhalHw_Rc663_Config.h"
#include <phhalHw_Rc663_Reg.h>
#include <phhalHw_Rc663_Cmd.h>

/*******************************************************************************
**   Function Declarations
*******************************************************************************/
static phStatus_t phhalHw_Rc663_Int_LoadTimer(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint32_t dwReload,
    uint8_t  bTimerAdd
    );


static phStatus_t phhalHw_Rc663_Int_LoadTimer(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint32_t dwReload,
    uint8_t  bTimerAdd
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;

    if (dwReload > PHHAL_HW_RC663_TIMER_RELOAD_MAX)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Write Timer reload value to High and Low registers. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, bTimerAdd, (uint8_t)(dwReload >> 8U)));
    return phhalHw_Rc663_WriteRegister(pDataParams, (bTimerAdd + 1U), (uint8_t)(dwReload));
}

phStatus_t phhalHw_Rc663_Cmd_Lpcd(
                                  phhalHw_Rc663_DataParams_t * pDataParams
                                  )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd;
    uint16_t    PH_MEMLOC_REM wOption;
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq1WaitFor;
    uint32_t    PH_MEMLOC_REM dwReload;
    float32_t   PH_MEMLOC_REM fReload;
    uint8_t     PH_MEMLOC_REM bRxAnaBackup;
    uint8_t     PH_MEMLOC_REM bQMin;
    uint8_t     PH_MEMLOC_REM bQMax;
    uint8_t     PH_MEMLOC_REM bIMin;
    uint8_t     PH_MEMLOC_REM bIMax;
    uint8_t     PH_MEMLOC_REM bDerivative = 0;
    uint8_t     PH_MEMLOC_REM bVersion = 0;
    uint8_t     PH_MEMLOC_REM bRegister;

    /* Configure T4 for AutoLPCD and Autowakeup and to use 16KHz LFO. */
    bRegister = PHHAL_HW_RC663_BIT_T4AUTOLPCD | PHHAL_HW_RC663_BIT_T4AUTOWAKEUP |
        PHHAL_HW_RC663_VALUE_TCLK_LFO_16_KHZ;

    /* Prepare command and option */
    aCmd = PHHAL_HW_RC663_CMD_LPCD;
    wOption = PH_EXCHANGE_DEFAULT;

    /* Prepare wait IRQs */
    bIrq0WaitFor = 0x00;
    bIrq1WaitFor = PHHAL_HW_RC663_BIT_LPCDIRQ;

    /* Parameter check */
    switch (pDataParams->bMode & (uint8_t)~(uint8_t)PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_MASK)
    {
    case PHHAL_HW_RC663_CMD_LPCD_MODE_DEFAULT:

        bIrq1WaitFor |= PHHAL_HW_RC663_BIT_TIMER3IRQ;
        break;

    case PHHAL_HW_RC663_CMD_LPCD_MODE_POWERDOWN_GUARDED:

        /* start and wait for timer */
        /* This option is used as fall through mechanism to exit LPCD loop after certain LPCD cycles. */
        wOption |= PHHAL_HW_RC663_RXTX_TIMER_START;
        bIrq1WaitFor |= PHHAL_HW_RC663_BIT_TIMER1IRQ;

    case PHHAL_HW_RC663_CMD_LPCD_MODE_POWERDOWN:

        /* Configure T4 additionally with AutoRestart to start T4 after every RFON during LPCD
         * to continue standby + rfon cycle continuously to detect antenna de-tuning. */
        bRegister |= PHHAL_HW_RC663_BIT_T4AUTORESTARTED;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Set standby bit */
    aCmd |= PHHAL_HW_RC663_BIT_STANDBY;

    /* Calculate timer 3 reload value */
    fReload = (float32_t)pDataParams->wDetectionTimeUs * PHHAL_HW_RC663_TIMER0_FREQ;
    dwReload = (uint32_t)fReload;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Int_LoadTimer(
        pDataParams,
        dwReload,
        PHHAL_HW_RC663_REG_T3RELOADHI));

    /* Calculate timer 4 reload value with LFO (16khz) clock used as last two bits of T4Control = 00b */
    dwReload = (uint32_t)((uint32_t)pDataParams->wPowerDownTimeMs * 16U);

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Int_LoadTimer(
        pDataParams,
        dwReload,
        PHHAL_HW_RC663_REG_T4RELOADHI));

    /* Set AutoTrimm bit if requested */
    if (0U != (pDataParams->bMode & PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_TRIMM_LPO))
    {
        bRegister |= PHHAL_HW_RC663_BIT_T4AUTOTRIMM;
    }

    /* Write T4Control register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T4CONTROL, bRegister));

    /* Set LPCD_IRQ_Clr to zero to get correct LPCD measurements period and to avoid not detecting card in first LPCD cycle
     * when Filter is ON. This is also a workaround for HW Artifacts : artf207360, artf207359 and artf207361 of Rc66303 Si. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_RESULT_Q, 0));

    /* Set I & Q values */
    if ((0U == ((pDataParams->bMode & PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_IGNORE_IQ))))
    {
        /* Set the Min I and Q values to the read LPCD Result values. */
        bQMin = pDataParams->bQ;
        bIMin = pDataParams->bI;
        /* Set the Max I and Q values to the read LPCD Result values. */
        bQMax = pDataParams->bQ;
        bIMax = pDataParams->bI;

        /* Read the RC663 derivative and version. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Int_ReadDerivative(pDataParams, &bDerivative, &bVersion));

        /* Based on the derivative version, the logic used to load I and Q values are different. */
        if (bVersion == PHHAL_HW_CLRC663PLUS_VERSION)
        {
            /* If Filter is enabled with option 2, load I and Q min values with read I and Q Result
             * values and set I and Q max values based on the I and Q Unstable bit read during calibration.
             * Option2 is enabled for improved detection range at the cost of frequent wake-ups. */
            if ((0U != (pDataParams->bLpcdOption & PHHAL_HW_RC663_BIT_LPCD_FILTER)) &&
                (0U != (pDataParams->bLpcdOption & PHHAL_HW_RC663_FILTER_BIT_OPTION2)))
            {
                /* Based on Q Unstable bit, Qmax value is set. */
                if ((0U != (pDataParams->bLpcdOption & PHHAL_HW_RC663_BIT_LPCD_Q_UNSTABLE)) &&
                    (bQMax < 0x3FU))
                {
                    /* Q Unstable Bit is Set. Increment QMax which is LPCD Result of Q + 1. */
                    bQMax++;
                }

                /* Based on I Unstable bit, Imax value is set. */
                if ((0U != (pDataParams->bLpcdOption & PHHAL_HW_RC663_BIT_LPCD_I_UNSTABLE)) &&
                    (bIMax < 0x3FU))
                {
                    /* I Unstable Bit is Set. Increment IMax which is LPCD Result of I + 1. */
                    bIMax++;
                }
            }
            /* Either if Filter is set to zero or Filter is set with option 1,
             * load I and Q values with +1 and -1 for stable operation with less wake-ups. */
            else
            {
                /* Set the Max I and Q value. */
                if (bQMax < 0x3FU)
                {
                    bQMax++;
                }
                if (bIMax < 0x3FU)
                {
                    bIMax++;
                }
                /* Set the Min I and Q value. */
                if (bQMin > 0U)
                {
                    bQMin--;
                }
                if (bIMin > 0U)
                {
                    bIMin--;
                }
            }
        }
        else
        {
            /* Set the Max I and Q value for other Derivatives. */
            if (bQMax < 0x3FU)
            {
                bQMax++;
            }
            if (bIMax < 0x3FU)
            {
                bIMax++;
            }
            /* Set the Min I and Q value for other Derivatives. */
            if (bQMin > 0U)
            {
                bQMin--;
            }
            if (bIMin > 0U)
            {
                bIMin--;
            }
        }

        /* Set Qmin register */
        bRegister = bQMin | (uint8_t)((bIMax & 0x30U) << 2U);
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_QMIN, bRegister));

        /* Set Qmax register */
        bRegister = bQMax | (uint8_t)((bIMax & 0x0CU) << 4U);
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_QMAX, bRegister));

        /* Set Imin register */
        bRegister = bIMin | (uint8_t)((bIMax & 0x03U) << 6U);
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_IMIN, bRegister));
    }

    /* Set Mix2Adc bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RCV, &bRegister));
    bRegister |= PHHAL_HW_RC663_BIT_RX_SHORT_MIX2ADC;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RCV, bRegister));

    /* Backup current RxAna setting */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RXANA, &bRxAnaBackup));
    /* Raise receiver gain to maximum */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXANA, PHHAL_HW_RC663_LPCD_RECEIVER_GAIN));

    /* Perform command */
    status = phhalHw_Rc663_Command_Int(
        pDataParams,
        aCmd,
        wOption,
        bIrq0WaitFor,
        bIrq1WaitFor,
        NULL,
        0,
        0,
        NULL,
        NULL);

    /* Restore RxAna register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXANA, bRxAnaBackup));

    /* Clear Mix2Adc bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_RCV, &bRegister));
    bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_RX_SHORT_MIX2ADC;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RCV, bRegister));

    /* Stop Timer4 if started */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_T4CONTROL, &bRegister));
    bRegister |= PHHAL_HW_RC663_BIT_T4STARTSTOPNOW;
    bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_T4RUNNING;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_T4CONTROL, bRegister));

    /* Stop the LPCD command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_COMMAND,
        PHHAL_HW_RC663_CMD_IDLE));

    /* Command error check (ignore timeout since interface could have been too slow) */
    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        PH_CHECK_SUCCESS(status);
    }

    /* Check if LPCD-IRQ is set */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1, &bRegister));

    /* Clear LPCD interrupt source to avoid any spurious LPCD_IRQ to be triggered. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_RESULT_Q, PHHAL_HW_RC663_BIT_LPCDIRQ_CLR));

    if (0U == (bRegister & PHHAL_HW_RC663_BIT_LPCDIRQ))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_IO_TIMEOUT, PH_COMP_HAL);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_Cmd_Lpcd_GetConfig(
                                            phhalHw_Rc663_DataParams_t * pDataParams,
                                            uint8_t * pI,
                                            uint8_t * pQ
                                            )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bDerivative = 0;
    uint8_t     PH_MEMLOC_REM bVersion = 0;
    uint8_t     PH_MEMLOC_REM bRegister;

    /* Set Qmin register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_QMIN, PHHAL_HW_RC663_LPCD_CALIBRATE_QMIN_REG));

    /* Set Qmax register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_QMAX, PHHAL_HW_RC663_LPCD_CALIBRATE_QMAX_REG));

    /* Set Imin register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_IMIN, PHHAL_HW_RC663_LPCD_CALIBRATE_IMIN_REG));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Int_ReadDerivative(pDataParams, &bDerivative, &bVersion));

    if (bVersion == PHHAL_HW_CLRC663PLUS_VERSION)
    {
        /* Configure Charge Pump and Filter set by the Application. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_OPTIONS,
            (pDataParams->bLpcdOption & (PHHAL_HW_RC663_BIT_LPCD_CHARGEPUMP | PHHAL_HW_RC663_BIT_LPCD_FILTER))));
    }

    /* Use default mode, trimm lpo and do not permit the function to set I and Q values */
    pDataParams->bMode |= PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_TRIMM_LPO | PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_IGNORE_IQ;

    /* Execute CMD LPCD to get I and Q values to perform LPCD procedure */
    status = phhalHw_Rc663_Cmd_Lpcd(pDataParams);

    /* If Derivative is RC663 Plus read I/Q unstable bit. */
    if (bVersion == PHHAL_HW_CLRC663PLUS_VERSION)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_OPTIONS, &bRegister));
        pDataParams->bLpcdOption |= bRegister & (PHHAL_HW_RC663_BIT_LPCD_Q_UNSTABLE | PHHAL_HW_RC663_BIT_LPCD_I_UNSTABLE);
    }

    /* Ignore timeout error */
    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        PH_CHECK_SUCCESS(status);
    }

    /* Retrieve I and Q results */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_RESULT_I, pI));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_LPCD_RESULT_Q, pQ));

    /* Mask out irrelevant bits */
    *pI &= 0x3FU;
    *pQ &= 0x3FU;

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_Cmd_LoadKey(
                                     phhalHw_Rc663_DataParams_t * pDataParams,
                                     uint8_t * pKey
                                     )
{
    uint8_t PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t PH_MEMLOC_REM bIrq1WaitFor;

    /* Set wait IRQs */
    bIrq0WaitFor = 0x00;
    bIrq1WaitFor = 0x00;

    /* Perform command */
    return phhalHw_Rc663_Command_Int(
        pDataParams,
        PHHAL_HW_RC663_CMD_LOADKEY,
        PH_EXCHANGE_DEFAULT,
        bIrq0WaitFor,
        bIrq1WaitFor,
        pKey,
        PHHAL_HW_RC663_MFC_KEY_LENGTH,
        0,
        NULL,
        NULL);
}

phStatus_t phhalHw_Rc663_Cmd_WriteE2(
                                     phhalHw_Rc663_DataParams_t * pDataParams,
                                     uint16_t wAddress,
                                     uint8_t bData
                                     )
{
    uint8_t PH_MEMLOC_REM aCmd[3];
    uint8_t PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t PH_MEMLOC_REM bIrq1WaitFor;

    /* Check Address range */
    if (wAddress >= PHHAL_HW_RC663_EEPROM_SIZE)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Set wait IRQs */
    bIrq0WaitFor = 0x00;
    bIrq1WaitFor = 0x00;

    /* Prepare command */
    aCmd[0] = (uint8_t)(wAddress >> 8U);
    aCmd[1] = (uint8_t)(wAddress);
    aCmd[2] = bData;

    /* Perform command */
    return phhalHw_Rc663_Command_Int(
        pDataParams,
        PHHAL_HW_RC663_CMD_WRITEE2,
        PH_EXCHANGE_DEFAULT,
        bIrq0WaitFor,
        bIrq1WaitFor,
        aCmd,
        3,
        0,
        NULL,
        NULL);
}

phStatus_t phhalHw_Rc663_Cmd_WriteE2Page(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint16_t wAddress,
    uint8_t * pData,
    uint8_t bDataLen
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[2];
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq1WaitFor;
    uint16_t    PH_MEMLOC_REM wFifoBytes;

    /* Check Address range and data length */
    if ((wAddress >= PHHAL_HW_RC663_EEPROM_NUM_PAGES) || (bDataLen == 0U) || (bDataLen > PHHAL_HW_RC663_EEPROM_PAGE_SIZE))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Set wait IRQs */
    bIrq0WaitFor = 0x00;
    bIrq1WaitFor = 0x00;

    /* Flush FiFo */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));

    /* Write Address into FIFO */
    aCmd[1] = (uint8_t)(wAddress);
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteFifo(pDataParams, &aCmd[1], 1, &wFifoBytes));

    /* Perform command */
    return phhalHw_Rc663_Command_Int(
        pDataParams,
        PHHAL_HW_RC663_CMD_WRITEE2PAGE,
        PHHAL_HW_RC663_OPTION_FIFO_NOCLEAR,
        bIrq0WaitFor,
        bIrq1WaitFor,
        pData,
        (uint16_t)bDataLen,
        0,
        NULL,
        NULL);
}

phStatus_t phhalHw_Rc663_Cmd_ReadE2(
                                    phhalHw_Rc663_DataParams_t * pDataParams,
                                    uint16_t wAddress,
                                    uint16_t wNumBytes,
                                    uint8_t * pData
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[3];
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq1WaitFor;
    uint16_t    PH_MEMLOC_REM wDataLenTmp;

    /* Parameter check */
    if (wNumBytes > 0x100U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* 0 means 256, do this for the check below */
    if (wNumBytes == 0U)
    {
        wNumBytes = 0x100;
    }

    /* Limit number of bytes to read to FiFo-Size */
    if (pDataParams->bFifoSize == PH_NXPNFCRDLIB_CONFIG_RC663_VALUE_FIFOSIZE_255)
    {
        if (wNumBytes > 0xFFU)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
    }

    /* Check Address range */
    if ((wAddress + wNumBytes) > PHHAL_HW_RC663_EEPROM_SIZE)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Set wait IRQs */
    bIrq0WaitFor = PHHAL_HW_RC663_BIT_HIALERTIRQ;
    bIrq1WaitFor = 0x00;

    /* Prepare command */
    aCmd[0] = (uint8_t)(wAddress >> 8U);
    aCmd[1] = (uint8_t)(wAddress);
    aCmd[2] = (uint8_t)wNumBytes;

    /* Perform command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Command_Int(
        pDataParams,
        PHHAL_HW_RC663_CMD_READE2,
        PH_EXCHANGE_DEFAULT,
        bIrq0WaitFor,
        bIrq1WaitFor,
        aCmd,
        3,
        wNumBytes,
        pData,
        &wDataLenTmp));

    /* Check received data */
    if (wDataLenTmp != wNumBytes)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_Cmd_LoadReg(
                                     phhalHw_Rc663_DataParams_t * pDataParams,
                                     uint16_t wEEAddress,
                                     uint8_t bRegAddress,
                                     uint8_t bNumBytes
                                     )
{
    uint8_t PH_MEMLOC_REM aCmd[4];
    uint8_t PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t PH_MEMLOC_REM bIrq1WaitFor;

    /* Parameter check */
    if (bNumBytes == 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* EEPROM address check */
    if ((wEEAddress < PHHAL_HW_RC663_EEPROM_SECTION2_BEGIN) || ((wEEAddress + bNumBytes) > (PHHAL_HW_RC663_EEPROM_SECTION2_END + 1U)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Register address check */
    if ((bRegAddress + bNumBytes) > PHHAL_HW_RC663_MAX_NUM_REGS)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Set wait IRQs */
    bIrq0WaitFor = 0x00;
    bIrq1WaitFor = 0x00;

    /* Prepare command */
    aCmd[0] = (uint8_t)(wEEAddress >> 8U);
    aCmd[1] = (uint8_t)(wEEAddress);
    aCmd[2] = bRegAddress;
    aCmd[3] = bNumBytes;

    /* Perform command */
    return phhalHw_Rc663_Command_Int(
        pDataParams,
        PHHAL_HW_RC663_CMD_LOADREG,
        PH_EXCHANGE_DEFAULT,
        bIrq0WaitFor,
        bIrq1WaitFor,
        aCmd,
        4,
        0,
        NULL,
        NULL);
}

phStatus_t phhalHw_Rc663_Cmd_LoadProtocol(
    phhalHw_Rc663_DataParams_t * pDataParams,
    uint8_t bTxProtocol,
    uint8_t bRxProtocol
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t PH_MEMLOC_REM aCmd[2];
    uint8_t PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t PH_MEMLOC_REM bIrq1WaitFor;
    uint8_t PH_MEMLOC_REM bDerivative = 0;
    uint8_t PH_MEMLOC_REM bVersion;

    /* Parameter check */
    if ((bTxProtocol > PHHAL_HW_RC663_MAX_TX_SETTINGS) || (bRxProtocol > PHHAL_HW_RC663_MAX_RX_SETTINGS))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Set wait IRQs this bit needs to be set for interrupt mode  */
    bIrq0WaitFor = 0x00;

    bIrq1WaitFor = 0x00;

    /* Check the RC663 derivative type by reading the manufacturer info page in the EEPROM*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_Int_ReadDerivative(pDataParams, &bDerivative, &bVersion));

    if(bDerivative == PHHAL_HW_SLRC610_PRODUCT_ID)
    {
        if (bTxProtocol >= 0x0AU)
        {
            bTxProtocol -= 0xAU;
        }
        if(bRxProtocol >= 0x0AU)
        {
            bRxProtocol -= 0xAU;
        }
    }

	/* CLRC661 supports TypeA, ISO15693 and ISO18000p3m3.
       Protocol Rx/Tx numbers for ISO15693 and ISO18000p3m3 are different
       for RC663 and RC661. Below code is to perform Protocol Rx/Tx number
	   synchronization. */
    if(bDerivative == PHHAL_HW_CLRC661_PRODUCT_ID)
    {
        if (bTxProtocol >= 0x04U)
        {
            bTxProtocol -= 0x6U;
        }
        if(bRxProtocol >= 0x04U)
        {
            bRxProtocol -= 0x6U;
        }
    }

    /* Prepare command */
    aCmd[0] = bRxProtocol;
    aCmd[1] = bTxProtocol;

    /* Perform command */
    return phhalHw_Rc663_Command_Int(
        pDataParams,
        PHHAL_HW_RC663_CMD_LOADPROTOCOL,
        PH_EXCHANGE_DEFAULT,
        bIrq0WaitFor,
        bIrq1WaitFor,
        aCmd,
        2,
        0,
        NULL,
        NULL);
}

phStatus_t phhalHw_Rc663_Cmd_LoadKeyE2(
                                       phhalHw_Rc663_DataParams_t * pDataParams,
                                       uint8_t  bKeyNo
                                       )
{
    uint8_t PH_MEMLOC_REM aCmd[1];
    uint8_t PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t PH_MEMLOC_REM bIrq1WaitFor;

    /* Set wait IRQs */
    bIrq0WaitFor = 0x00;
    bIrq1WaitFor = 0x00;

    /* Prepare command */
    aCmd[0] = bKeyNo;

    /* Perform command */
    return phhalHw_Rc663_Command_Int(
        pDataParams,
        PHHAL_HW_RC663_CMD_LOADKEYE2,
        PH_EXCHANGE_DEFAULT,
        bIrq0WaitFor,
        bIrq1WaitFor,
        aCmd,
        1,
        0,
        NULL,
        NULL);
}

phStatus_t phhalHw_Rc663_Cmd_StoreKeyE2(
                                        phhalHw_Rc663_DataParams_t * pDataParams,
                                        uint8_t  bKeyNo,
                                        uint8_t * pKeys,
                                        uint8_t bNumberOfKeys
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[2];
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t     PH_MEMLOC_REM bIrq1WaitFor;
    uint16_t    PH_MEMLOC_REM wDataLen = 0;
    uint16_t    PH_MEMLOC_REM wFifoBytes;

    /* Parameter check */
    if (bNumberOfKeys == 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Parameter check */
    if ((uint16_t)((uint16_t)bKeyNo + (uint16_t)bNumberOfKeys) > PHHAL_HW_RC663_MAX_NUM_KEYS)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Calculate overal data length */
    wDataLen = (uint16_t)((uint16_t)bNumberOfKeys * PHHAL_HW_RC663_MFC_KEY_LENGTH);

    /* Limit number of bytes to write to FiFo-Size */
    if (pDataParams->bFifoSize == PH_NXPNFCRDLIB_CONFIG_RC663_VALUE_FIFOSIZE_255)
    {
        if (wDataLen > 0xFFU)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
    }
    else
    {
        if (wDataLen > 0x200U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
    }

    /* Set wait IRQs */
    bIrq0WaitFor = 0x00;
    bIrq1WaitFor = 0x00;

    /* Flush FiFo */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));

    /* Write Keynumber into FIFO */
    aCmd[1] = bKeyNo;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteFifo(pDataParams, &aCmd[1], 1, &wFifoBytes));

    /* Perform command */
    return phhalHw_Rc663_Command_Int(
        pDataParams,
        PHHAL_HW_RC663_CMD_STOREKEYE2,
        PHHAL_HW_RC663_OPTION_FIFO_NOCLEAR,
        bIrq0WaitFor,
        bIrq1WaitFor,
        pKeys,
        wDataLen,
        0,
        NULL,
        NULL);
}

phStatus_t phhalHw_Rc663_Cmd_SoftReset(
                                       phhalHw_Rc663_DataParams_t * pDataParams
                                       )
{

    return phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_SOFTRESET);
}

phStatus_t phhalHw_Rc663_Cmd_AckReq(
                                    phhalHw_Rc663_DataParams_t * pDataParams,
                                    uint8_t * pCmdBuffer,
                                    uint16_t wLen,
                                    uint8_t * pRxBuffer,
                                    uint16_t * pwRxBufferLen
                                    )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint16_t   PH_MEMLOC_REM wBytesWritten;
    uint8_t    PH_MEMLOC_REM bError;
    uint8_t    PH_MEMLOC_REM bRegister;
    uint8_t    PH_MEMLOC_REM bIrq0WaitFor;
    uint8_t    PH_MEMLOC_REM bIrq1WaitFor;
    uint8_t    PH_MEMLOC_REM bIrq0Reg = 0x00;
    uint8_t    PH_MEMLOC_REM bIrq1Reg = 0x00;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXCORR, 0x88));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXCTRL, 0x07));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_RXWAIT, 0x3D));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXWAITLO, 0x80));

    /* Set Preamble */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_FRAMECON, 0x01));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXDATAMOD, 0x00U));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXSYM0H, 0x68));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXSYM0L, 0x41));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXSYM10LEN, 0x8E));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXSYM10BURSTLEN, 0x00U));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXSYM10BURSTCTRL, 0x00U));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXSYM10MOD, 0x00U));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_TXSYMFREQ, 0x05));
    /* Set preamble done */

    /* Load IDLE command (to terminate a possibly running command) */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_COMMAND,
        PHHAL_HW_RC663_CMD_IDLE));

    /* Flush FiFo */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));

    /* Clear all IRQ0 flags */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_IRQ0,
        (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_SET));

    /* Clear all IRQ1 flags */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_IRQ1,
        (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_SET));

    /* Write exchange command to FiFo */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteFifo(
        pDataParams,
        &pCmdBuffer[0],
        wLen,
        &wBytesWritten));

    /* Start the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_COMMAND,
        PHHAL_HW_RC663_CMD_ACKREQ));

    /* Set wait IRQs */
    bIrq0WaitFor = PHHAL_HW_RC663_BIT_IDLEIRQ;
    bIrq1WaitFor = PHHAL_HW_RC663_BIT_TIMER1IRQ;

    /* Wait for enabled IRQs */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WaitIrq(
        pDataParams,
        PH_ON,
        PH_OFF,
        bIrq0WaitFor,
        bIrq1WaitFor,
        &bIrq0Reg,
        &bIrq1Reg));

    /* Timeout handling */
    if (0U != (bIrq1Reg & PHHAL_HW_RC663_BIT_TIMER1IRQ))
    {
        /* Stop the command */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_COMMAND,
            PHHAL_HW_RC663_CMD_IDLE));

        /* Flush FiFo */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_FlushFifo(pDataParams));

        /* Retrieve content of Timer Control Register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_TCONTROL,
            &bRegister));

        /* Manually stop timers */
        bRegister |= PHHAL_HW_RC663_BIT_T0STARTSTOPNOW | PHHAL_HW_RC663_BIT_T1STARTSTOPNOW;
        bRegister &= (uint8_t)~(uint8_t)(PHHAL_HW_RC663_BIT_T0RUNNING | PHHAL_HW_RC663_BIT_T1RUNNING);
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
            pDataParams,
            PHHAL_HW_RC663_REG_TCONTROL,
            bRegister));

        /* Return timeout */
        return PH_ADD_COMPCODE_FIXED(PH_ERR_IO_TIMEOUT, PH_COMP_HAL);
    }

    /* Read received data from FiFo (Max possible response length is 70) */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadFifo(
        pDataParams,
        70,
        pRxBuffer,
        pwRxBufferLen));

    /* No bytes received -> timeout */
    if(pwRxBufferLen == NULL)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_IO_TIMEOUT, PH_COMP_HAL);
    }

    /* Check if any error occurred */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(
        pDataParams,
        PHHAL_HW_RC663_REG_ERROR,
        &bError));

    /* Buffer overflow */
    if (0U != (bError & PHHAL_HW_RC663_BIT_FIFOOVL))
    {
        statusTmp = PH_ERR_BUFFER_OVERFLOW;
    }
    /* FiFo write error */
    else if (0U != (bError & PHHAL_HW_RC663_BIT_FIFOWRERR))
    {
        statusTmp = PH_ERR_READ_WRITE_ERROR;
    }
    /* EEPROM error */
    else if (0U != (bError & PHHAL_HW_RC663_BIT_EE_ERR))
    {
        statusTmp = PH_ERR_READ_WRITE_ERROR;
    }
    /* Framing error */
    else if (0U != (bError & PHHAL_HW_RC663_BIT_MINFRAMEERR))
    {
        statusTmp = PH_ERR_FRAMING_ERROR;
    }
    /* Collision */
    else if (0U != (bError & PHHAL_HW_RC663_BIT_COLLDET))
    {
        statusTmp = PH_ERR_COLLISION_ERROR;
    }
    /* Protocol error */
    else if (0U != (bError & PHHAL_HW_RC663_BIT_PROTERR))
    {
        statusTmp = PH_ERR_PROTOCOL_ERROR;
    }
    /* CRC / parity error */
    else if (0U != (bError & PHHAL_HW_RC663_BIT_INTEGERR))
    {
        statusTmp = PH_ERR_INTEGRITY_ERROR;
    }
    /* No error */
    else
    {
        statusTmp = PH_ERR_SUCCESS;
    }

    return PH_ADD_COMPCODE(statusTmp, PH_COMP_HAL);
}

phStatus_t phhalHw_Rc663_Cmd_Lpcd_SetConfig(
                                         phhalHw_Rc663_DataParams_t * pDataParams,
                                         uint8_t bMode,
                                         uint8_t bI,
                                         uint8_t bQ,
                                         uint16_t wPowerDownTimeMs,
                                         uint16_t wDetectionTimeUs
                                         )
{
    /* Check for valid I/Q values only in Default Mode and with Option to not ignore I and Q. */
    if (((bMode & PHHAL_HW_RC663_CMD_LPCD_MODE_MASK) != PHHAL_HW_RC663_CMD_LPCD_MODE_DEFAULT) &&
        (0U == (bMode & PHHAL_HW_RC663_CMD_LPCD_MODE_OPTION_IGNORE_IQ)))
    {
        if ((bI > 0x3FU) || (bQ > 0x3FU))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
    }

    pDataParams->bMode = bMode;
    pDataParams->bI = bI;
    pDataParams->bQ = bQ;
    pDataParams->wPowerDownTimeMs = wPowerDownTimeMs;
    pDataParams->wDetectionTimeUs = wDetectionTimeUs;

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_WriteRegister(
                                       phhalHw_Rc663_DataParams_t * pDataParams,
                                       uint8_t bAddress,
                                       uint8_t bValue
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bDataBuffer[2];
    uint16_t    PH_MEMLOC_REM wBytesRead;
    uint8_t     PH_MEMLOC_REM bNumExpBytes;

    /* RS232 protocol */
    if (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_RS232)
    {
        /* clear RD/NWR bit to indicate write operation */
        bAddress &= 0x7FU;
        bNumExpBytes = 1;
    }
    /* SPI protocol */
    else if (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_SPI)
    {
        /* shift address and clear RD/NWR bit to indicate write operation */
        bAddress = (uint8_t)(bAddress << 1U);
        bNumExpBytes = 2;

        /* Enable chip select connected to reader IC by pulling NSS low. */
        phhalHw_Rc663_WriteSSEL(pDataParams->pBalDataParams, PH_DRIVER_SET_LOW);
    }
    /* I2C protocol */
    else if (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_I2C)
    {
        bNumExpBytes = 0;
    }
    else
    {
        /* Insert Code for other protocols here */
        bNumExpBytes = 0;
    }

    /* Write the address and data */
    bDataBuffer[0] = bAddress;
    bDataBuffer[1] = bValue;
    PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Exchange(
        pDataParams->pBalDataParams,
        PH_EXCHANGE_DEFAULT,
        bDataBuffer,
        2,
        bNumExpBytes,
        bDataBuffer,
        &wBytesRead));

    /* Check number of received bytes */
    if (wBytesRead != bNumExpBytes)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERFACE_ERROR, PH_COMP_HAL);
    }

    /* RS232 protocol */
    if (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_RS232)
    {
        /* Returned address should match */
        if (bDataBuffer[0] != bAddress)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERFACE_ERROR, PH_COMP_HAL);
        }
    }
    /* SPI protocol */
    else if (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_SPI)
    {
        /* Disable chip select connected to reader IC by pulling NSS high. */
        phhalHw_Rc663_WriteSSEL(pDataParams->pBalDataParams, PH_DRIVER_SET_HIGH);
    }
    /* I2C protocol */
    else if (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_I2C)
    {
        /* nothing to be done */
    }
    else
    {
        /* Insert Code for other protocols here */
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_ReadRegister(
                                      phhalHw_Rc663_DataParams_t * pDataParams,
                                      uint8_t bAddress,
                                      uint8_t * pValue
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bTxBuffer[2];
    uint16_t    PH_MEMLOC_REM wTxLength = 0;
    uint16_t    PH_MEMLOC_REM wBytesRead;
    uint8_t     PH_MEMLOC_REM bNumExpBytes;

    /* RS232 protocol */
    if (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_RS232)
    {
        /* set RD/NWR bit to indicate read operation */
        bTxBuffer[0] = bAddress | 0x80U;
        wTxLength = 1;
        bNumExpBytes = 1;
    }
    /* SPI protocol */
    else if (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_SPI)
    {
        /* set RD/NWR bit to indicate read operation */
        bTxBuffer[0] = (uint8_t)(bAddress << 1U) | 0x01U;
        bTxBuffer[1] = 0x00;
        wTxLength = 2;
        bNumExpBytes = 2;

        /* Enable chip select connected to reader IC by pulling NSS low. */
        phhalHw_Rc663_WriteSSEL(pDataParams->pBalDataParams, PH_DRIVER_SET_LOW);
    }
    /* I2C protocol */
    else if (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_I2C)
    {
        /* nothing to be modified */
        bTxBuffer[0] = bAddress;
        wTxLength = 1;
        bNumExpBytes = 1;
    }
    else
    {
        /* Insert Code for other protocols here */
        wTxLength = 0;
        bNumExpBytes = 0;
    }

    /* Write the address and retrieve the register content */
    PH_CHECK_SUCCESS_FCT(statusTmp, phbalReg_Exchange(
        pDataParams->pBalDataParams,
        PH_EXCHANGE_DEFAULT,
        bTxBuffer,
        wTxLength,
        bNumExpBytes,
        bTxBuffer,
        &wBytesRead));

    /* Check number of received bytes */
    if (wBytesRead != bNumExpBytes)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERFACE_ERROR, PH_COMP_HAL);
    }

    /* in case of SPI 2 bytes are received from a read */
    if (pDataParams->bBalConnectionType == PHHAL_HW_BAL_CONNECTION_SPI)
    {
        /* Disable chip select connected to reader IC by pulling NSS high. */
        phhalHw_Rc663_WriteSSEL(pDataParams->pBalDataParams, PH_DRIVER_SET_HIGH);

        *pValue = bTxBuffer[1];
    }
    else
    {
        *pValue = bTxBuffer[0];
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phhalHw_Rc663_Cmd_PRBS(
                                  phhalHw_Rc663_DataParams_t * pDataParams,
                                  uint8_t * pBufferStartPos,
                                  uint16_t wBuffLength
                                  )
{
    phStatus_t  PH_MEMLOC_REM status = PH_ERR_SUCCESS;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wFifoBytes;
    uint16_t    PH_MEMLOC_REM wTxLength;
    uint16_t    PH_MEMLOC_REM wTmpBufferLen = 0x00;
    uint16_t    PH_MEMLOC_REM wNumPrecachedBytes = 0x00;
    uint8_t *   PH_MEMLOC_REM pTxBuffer;
    uint8_t     PH_MEMLOC_REM bIrq0WaitFor = 0x00;
    uint8_t     PH_MEMLOC_REM bIrq1WaitFor = 0x00;
    uint8_t     PH_MEMLOC_REM bIrq0Reg = 0x00;
    uint8_t     PH_MEMLOC_REM bIrq1Reg = 0x00;

    if ((wBuffLength == 0x00U) || (pBufferStartPos == NULL))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Initializing Variable used. */
    pTxBuffer = pBufferStartPos;
    wTxLength = wBuffLength;

    /* Clear internal Tx buffer. */
    pDataParams->wTxBufLen = 0;

    /* Turn off Parity irrespective of protocol configured */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_PARITY, PH_OFF));

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

    /* Set Water level count based on number of bytes that are been provided as input. */
    do
    {
        /* Precache given buffer contents into FIFO */
        wNumPrecachedBytes = (wTxLength > pDataParams->wMaxPrecachedBytes) ? pDataParams->wMaxPrecachedBytes : wTxLength;

        /* Perform actual write to FIFO */
        status = phhalHw_Rc663_WriteFifo(pDataParams, pTxBuffer, wNumPrecachedBytes, &wFifoBytes);

        /* Ignore FiFo overflow warning */
        if ((status & PH_ERR_MASK) != PH_ERR_BUFFER_OVERFLOW)
        {
            statusTmp = status;
            PH_BREAK_ON_FAILURE(status);
        }

        /* Update buffer pointer and length */
        wTxLength = wTxLength - wFifoBytes;
        if (wTxLength == 0x00U)
        {
            pTxBuffer = pBufferStartPos;
            wTxLength = wBuffLength;
        }
        else
        {
            pTxBuffer += wFifoBytes;
        }
        wTmpBufferLen += wFifoBytes;
    }
    while (wTmpBufferLen != pDataParams->wMaxPrecachedBytes);

    /* start the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_TRANSMIT));

    /* Set wait IRQs */
    bIrq0WaitFor = PHHAL_HW_RC663_BIT_TXIRQ | PHHAL_HW_RC663_BIT_IDLEIRQ | PHHAL_HW_RC663_BIT_LOALERTIRQ;
    bIrq1WaitFor = 0U;

    do{
#ifndef NXPBUILD__PH_PLATFORM_DELEGATE
        /* wait until the transmission is finished */
        statusTmp = phhalHw_Rc663_WaitIrq(
            pDataParams,
            PH_ON,
            PH_OFF,
            bIrq0WaitFor,
            bIrq1WaitFor,
            &bIrq0Reg,
            &bIrq1Reg);
        if ((statusTmp & PH_ERR_MASK) == PH_ERR_ABORTED)
        {
            status = PH_ERR_SUCCESS;
            break;
        }
        else
        {
            PH_CHECK_SUCCESS(statusTmp);
        }

#endif /* NXPBUILD__PH_PLATFORM_DELEGATE*/

        /* Write the remaining data into the Fifo */
        /* (Only possible if interface to RC is fast enough!) */
        if (0U != (bIrq0WaitFor & PHHAL_HW_RC663_BIT_LOALERTIRQ))
        {
            /* Check for FIFO underflow */
#ifdef PHHAL_HW_RC663_FEATURE_FIFO_UNDERFLOW_CHECK
            /* read interrupt status */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0, &bIrq0Reg));

            /* If we got data to transmit but the Tx-command aborted, we were too slow! */
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
            status = phhalHw_Rc663_WriteFifo(pDataParams, pTxBuffer, wTxLength, &wFifoBytes);
            /* Ignore FiFo overflow warning */
            if ((status & PH_ERR_MASK) != PH_ERR_BUFFER_OVERFLOW)
            {
                PH_CHECK_SUCCESS(status);
            }

            /* Update buffer pointer and length */
            wTxLength = wTxLength - wFifoBytes;
            if (wTxLength == 0x00U)
            {
                pTxBuffer = pBufferStartPos;
                wTxLength = wBuffLength;
            }
            else
            {
                pTxBuffer += wFifoBytes;
            }

            /* Mask the LOW ALERT IRQ. */
            bIrq0Reg &= (uint8_t)~PHHAL_HW_RC663_BIT_LOALERTIRQ;

            /* Clear the LOW ALERT IRQ. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(
                pDataParams,
                PHHAL_HW_RC663_REG_IRQ0,
                (uint8_t)PHHAL_HW_RC663_BIT_LOALERTIRQ));
        }
    } while(1);

    return PH_ADD_COMPCODE(status, PH_COMP_HAL);
}

#endif /* NXPBUILD__PHHAL_HW_RC663 */
