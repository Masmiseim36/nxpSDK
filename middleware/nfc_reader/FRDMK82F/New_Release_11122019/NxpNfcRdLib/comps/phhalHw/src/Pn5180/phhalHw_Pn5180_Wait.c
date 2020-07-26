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
* Wait Routines for Pn5180 specific HAL-Component of Reader Library Framework.
*
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHHAL_HW_PN5180

#include <phOsal.h>
#include <phDriver.h>
#include <phNxpNfcRdLib_Config.h>
#include "phhalHw_Pn5180.h"
#include "phhalHw_Pn5180_Instr.h"
#include "phhalHw_Pn5180_Int.h"
#include "phhalHw_Pn5180_Wait.h"
#include "phhalHw_Pn5180_Reg.h"



#if defined(_WIN32)
#include <phOsal_WinOs.h>
#endif

phStatus_t phhalHw_Pn5180_WaitIrq(
    phhalHw_Pn5180_DataParams_t * pDataParams,
    uint8_t bEnableIrq,
    uint8_t bWaitUntilPowerUp,
    uint32_t dwIrqWaitFor,
    uint32_t * dwIrqReg
    )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    uint32_t   PH_MEMLOC_REM dwRegister;
    phOsal_EventBits_t PH_MEMLOC_REM tReceivedEvents;

    /* Parameter check */
    if (0U == (dwIrqWaitFor))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }
    if (bWaitUntilPowerUp == PH_ON)
    {
        /* Need to add power up behaviour */
    }

    /* If Test Bus is enabled, we should not relay on IRQ Pin
     * instead poll on IRQ Status register. */
    if ( pDataParams->bIsTestBusEnabled == PH_ON)
    {
        /* Test Bus is enabled */
        bEnableIrq &= (uint8_t)~(uint8_t)PHHAL_HW_CHECK_IRQ_PIN_MASK;
    }

    /* If requested by the user, enable the corresponding IRQs */
    if ((bEnableIrq & PHHAL_HW_CHECK_IRQ_PIN_MASK) != PH_OFF)
    {
        /*wait for IRQ pin event or Abort event*/

        {
            statusTmp = phOsal_EventPend((volatile phOsal_Event_t * )(&pDataParams->HwEventObj.EventHandle), E_OS_EVENT_OPT_PEND_SET_ANY, PHOSAL_MAX_DELAY,
                (E_PH_OSAL_EVT_RF | E_PH_OSAL_EVT_ABORT), &tReceivedEvents);
        }

        /*Handle abort event*/
        if ((E_PH_OSAL_EVT_ABORT & tReceivedEvents) || (statusTmp != PH_ERR_SUCCESS))
        {

            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Pn5180_Int_IdleCommand(pDataParams));

            /* Disable IRQ sources */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Pn5180_Instr_WriteRegisterAndMask(pDataParams, IRQ_ENABLE, (uint32_t)~dwIrqWaitFor));

            (void)phOsal_EventClear(&pDataParams->HwEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_ABORT, NULL);
            return PH_ADD_COMPCODE_FIXED(PH_ERR_ABORTED, PH_COMP_HAL);
        }
        else
        {
            /* Read the IRQ register and check if the interrupt has occured */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Pn5180_Instr_ReadRegister(pDataParams, IRQ_STATUS, &dwRegister));

            /* Store the state of status register */
            *dwIrqReg = dwRegister;

            if ((bEnableIrq & PHHAL_HW_DISABLE_IRQ_CLEAR_MASK) == PH_OFF)
            {
                /* Clear all Interrupts for e.g Tx interrupt during receive */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Pn5180_Instr_WriteRegister(pDataParams, IRQ_SET_CLEAR, dwRegister));
            }
            /* Disable IRQ sources */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Pn5180_Instr_WriteRegisterAndMask(pDataParams, IRQ_ENABLE, (uint32_t)~dwIrqWaitFor));

            (void)phOsal_EventClear(&pDataParams->HwEventObj.EventHandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_RF, NULL);

            return PH_ERR_SUCCESS;

        }
    }
    else
    {
        /* Wait until any of the IRQ bits that we are subscribing for occurs */
        do
        {
            /* If Test Bus is enabled, we should avoid polling on registers
             * continuously as we put SPI noise on RF. */
            if ( pDataParams->bIsTestBusEnabled == PH_ON)
            {
                if (pDataParams->wWaitIRQDelayWithTestBus == 0x00U)
                {
                    /* Wait for atleast 1milli sec. */
                    phDriver_TimerStart(PH_DRIVER_TIMER_MILLI_SECS, 0x01U, NULL);
                }
                else
                {
                    phDriver_TimerStart(PH_DRIVER_TIMER_MILLI_SECS, pDataParams->wWaitIRQDelayWithTestBus, NULL);
                }
            }


            /* Read the IRQ register and check if the interrupt has occured */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Pn5180_Instr_ReadRegister(pDataParams, IRQ_STATUS, &dwRegister));
        }while((0U == ((dwRegister & dwIrqWaitFor))));

        /* Store the state of status register */
        *dwIrqReg = dwRegister;

        if ((bEnableIrq & PHHAL_HW_DISABLE_IRQ_CLEAR_MASK) == PH_OFF)
        {
            /* Clear all Interrupts for e.g Tx interrupt during receive */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Pn5180_Instr_WriteRegister(pDataParams, IRQ_SET_CLEAR, dwRegister));
        }
    }

    return PH_ERR_SUCCESS;

}

#endif  /* NXPBUILD__PHHAL_HW_PN5180 */
