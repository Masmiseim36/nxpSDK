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
* Wait Routines for RC663 specific HAL-Component of Reader Library Framework.
* $Author: Ankur Srivastava (nxp79569) $
* $Revision: 6279 $ (v06.01.00)
* $Date: 2016-10-04 18:30:45 +0530 (Tue, 04 Oct 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHHAL_HW_RC663

#include "phhalHw.h"
#include "phhalHw_Rc663.h"
#include "phhalHw_Rc663_Int.h"
#include <phhalHw_Rc663_Reg.h>
#include "phOsal.h"


phStatus_t phhalHw_Rc663_WaitIrq(
                                 phhalHw_Rc663_DataParams_t * pDataParams,
                                 uint8_t bEnableIrqs,
                                 uint8_t bWaitUntilPowerUp,
                                 uint8_t bIrq0WaitFor,
                                 uint8_t bIrq1WaitFor,
                                 uint8_t * pIrq0Reg,
                                 uint8_t * pIrq1Reg
                                 )
{
#ifdef _WIN32
    phStatus_t  PH_MEMLOC_REM status;
#endif /* _WIN32 */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bIrq0EnReg;
    uint8_t     PH_MEMLOC_REM bIrq1EnReg;
    uint8_t     PH_MEMLOC_REM bRegister;
#ifndef _WIN32
    phOsal_EventBits_t   PH_MEMLOC_REM dwEventReceived = 0;
#endif /* _WIN32 */

    /* Parameter check */
    if ((0U == bIrq0WaitFor) && (0U == bIrq1WaitFor))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }

    /* Read the IRQ0EN register once here. Used below */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0EN, &bRegister));

    /* Enable Irqs if requested */
    if (bEnableIrqs != PH_OFF)
    {
#ifndef _WIN32
        /* Clear all events that might have occurred. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phOsal_EventClear(&pDataParams->HwEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_RF, NULL));
#endif /* _WIN32 */
        /* Enable IRQ0 interrupt sources */

        /* Clear all interrupts */
        bIrq0EnReg = bRegister & PHHAL_HW_RC663_BIT_IRQINV;

        /* Set all bits except the 8th bit */
        bIrq0EnReg |= (0x7FU & bIrq0WaitFor);

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0EN, bIrq0EnReg));

        /* Enable IRQ1 interrupt sources */
        bIrq1EnReg = PHHAL_HW_RC663_BIT_IRQPUSHPULL;
        bIrq1EnReg |= PHHAL_HW_RC663_BIT_GLOBALIRQ | bIrq1WaitFor;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1EN, bIrq1EnReg));
    }
    /* Else just init. the Irq values */
    else
    {
        /* Enable IRQ1 interrupt sources */
        bIrq1EnReg = PHHAL_HW_RC663_BIT_IRQPUSHPULL;
        bIrq1EnReg |= PHHAL_HW_RC663_BIT_GLOBALIRQ | bIrq1WaitFor;
    }


#ifndef NXPBUILD__PH_PLATFORM_DELEGATE
#ifdef _WIN32
    /* wait until an IRQ occurs */
    do
    {
        /* INSERT CODE HERE */
        /* IMPORTANT: Do not clear GlobalIRQ bit in IRQ1 register since it is needed below! */

        /* Wait until power up mode */
        if (bWaitUntilPowerUp != PH_OFF)
        {
            /* Read command register */
            status = phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, &bRegister);

            /* Ignore timeout error */
            if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
            {
                /* Status check */
                PH_CHECK_SUCCESS(status);

                /* Only read IRQ register if not in standby mode */
                if (0U == (bRegister & PHHAL_HW_RC663_BIT_STANDBY))
                {
                    status = phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1, &bRegister);

                    /* Ignore timeout error */
                    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
                    {
                        PH_CHECK_SUCCESS(status);
                    }
                }
                else
                {
                    bRegister = 0x00;
                }
            }
            else
            {
                bRegister = 0x00;
            }
        }
        /* Always read IRQ register */
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1, &bRegister));
        }
    }
    while (0U == (bRegister & PHHAL_HW_RC663_BIT_GLOBALIRQ));

    /* Leave standby mode by terminating the command */
    if (bWaitUntilPowerUp != PH_OFF)
    {
        do
        {
            status = phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_IDLE);
        }
        while ((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT);
    }
#endif /* _WIN32 */
#ifndef _WIN32
    PH_CHECK_SUCCESS_FCT(statusTmp, phOsal_EventPend((volatile phOsal_Event_t * )(&pDataParams->HwEventObj.EventHandle), E_OS_EVENT_OPT_PEND_SET_ANY,
        PHOSAL_MAX_DELAY, (E_PH_OSAL_EVT_RF | E_PH_OSAL_EVT_ABORT), &dwEventReceived));

    if (0U != (dwEventReceived & E_PH_OSAL_EVT_ABORT))
    {
        /* Clear IRQ0 interrupt sources */
        bIrq0EnReg = bRegister & PHHAL_HW_RC663_BIT_IRQINV;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0EN, bIrq0EnReg));

        /* Clear IRQ1 interrupt sources */
        bIrq1EnReg &= (PHHAL_HW_RC663_BIT_IRQPUSHPULL | PHHAL_HW_RC663_BIT_GLOBALIRQ);
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1EN, bIrq1EnReg));

        /* Consume abort event */
        PH_CHECK_SUCCESS_FCT(statusTmp, phOsal_EventClear(&pDataParams->HwEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_ABORT, NULL));

        /* Load IDLE Command and exit with HAL Aborted status. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_COMMAND, PHHAL_HW_RC663_CMD_IDLE));
        return PH_ADD_COMPCODE_FIXED(PH_ERR_ABORTED, PH_COMP_HAL);
    }
#endif /* _WIN32 */
#endif /* NXPBUILD__PH_PLATFORM_DELEGATE */

    /* Clear IRQ0 interrupt sources */
    bIrq0EnReg = bRegister & PHHAL_HW_RC663_BIT_IRQINV;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0EN, bIrq0EnReg));

    /* Clear IRQ1 interrupt sources */
    bIrq1EnReg &= (PHHAL_HW_RC663_BIT_IRQPUSHPULL | PHHAL_HW_RC663_BIT_GLOBALIRQ);
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_WriteRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1EN, bIrq1EnReg));

    /* return IRQ1 status */
    if (NULL != pIrq1Reg)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ1, pIrq1Reg));
    }
    /* return Irq0 status */
    if (NULL != pIrq0Reg)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Rc663_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0, pIrq0Reg));
    }

#ifndef _WIN32
    /* Consume Unwanted Events */
   PH_CHECK_SUCCESS_FCT(statusTmp, phOsal_EventClear(&pDataParams->HwEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_RF, NULL));
#endif

    PH_UNUSED_VARIABLE(bWaitUntilPowerUp);
    return PH_ERR_SUCCESS;
}


#endif /* NXPBUILD__PHHAL_HW_RC663 */
