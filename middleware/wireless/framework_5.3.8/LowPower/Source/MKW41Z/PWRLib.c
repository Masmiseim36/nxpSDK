/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/*****************************************************************************
 *                               INCLUDED HEADERS                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the headers that this module needs to include.    *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#include "EmbeddedTypes.h"
#include "fsl_device_registers.h"
#include "board.h"
#include "PWRLib.h"
#include "PWR_Configuration.h"
#include "TimersManager.h"
#include "fsl_os_abstraction.h"
#include "Keyboard.h"   
#include "GPIO_Adapter.h"
#include "gpio_pins.h"

/*****************************************************************************
 *                             PRIVATE MACROS                                *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...                                              *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#define mLptmrTimoutMaxMs_c    (65535000)

#if (cPWR_BLE_LL_Enable)   
/* LLS3 + BLE DSM wakeup on the first next instant by BLE timer */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_1  (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/* LLS3 + BLE DSM wakeup on deepSleepTimeInMs by BLE timer*/
#define BLE_LL_CLOCK_CONFIG_LP_MOD_2  (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_DSM_INT_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/* LLS3 + BLE idle wakeup on deepSleepTimeInMs by LPTMR */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_3  (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_DSM_INT_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/* VLLS0 + BLE idle wakeup by keyboard */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_4  (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_DSM_INT_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/* VLLS2 + BLE idle wakeup by keyboard */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_5  (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_DSM_INT_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/* STOP wakeup on deepSleepTimeInMs by LPTMR */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_6  (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)
#endif /* (cPWR_BLE_LL_Enable) */

/*****************************************************************************
 *                               PRIVATE VARIABLES                           *
 *---------------------------------------------------------------------------*
 * Add to this section all the variables and constants that have local       *
 * (file) scope.                                                             *
 * Each of this declarations shall be preceded by the 'static' keyword.      *
 * These variables / constants cannot be accessed outside this module.       *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#if (cPWR_BLE_LL_Enable)
const uint16_t maBLE_LL_CLOCK_CONFIG_LP[] = {BLE_LL_CLOCK_CONFIG_LP_MOD_1,
                                             BLE_LL_CLOCK_CONFIG_LP_MOD_2,
                                             BLE_LL_CLOCK_CONFIG_LP_MOD_3,
                                             BLE_LL_CLOCK_CONFIG_LP_MOD_4,
                                             BLE_LL_CLOCK_CONFIG_LP_MOD_5,
                                             BLE_LL_CLOCK_CONFIG_LP_MOD_6,
                                             BLE_LL_CLOCK_CONFIG_LP_MOD_6};
#endif /* (cPWR_BLE_LL_Enable) */

#if (cPWR_UsePowerDownMode == 1)
static uint8_t mPWRLib_DeepSleepMode = cPWR_DeepSleepMode;
/* LPTMR variables */
static uint32_t mPWRLib_RTIElapsedTicks;
const uint8_t maLPModeUseLPTMR[] = {0, 1, 1, 1, 0, 0, 1, 1};
#endif /* (cPWR_UsePowerDownMode == 1) */

/* For LVD function */ 
#if (cPWR_LVD_Enable == 2)
tmrTimerID_t PWRLib_LVD_PollIntervalTmrID;
PWRLib_LVD_VoltageLevel_t PWRLib_LVD_SavedLevel;
#endif /* (cPWR_LVD_Enable == 2) */

/*****************************************************************************
 *                               PUBLIC VARIABLES                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the variables and constants that have global      *
 * (project) scope.                                                          *
 * These variables / constants can be accessed outside this module.          *
 * These variables / constants shall be preceded by the 'extern' keyword in  *
 * the interface header.                                                     *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
/* Zigbee STACK status */ 
PWRLib_StackPS_t PWRLib_StackPS;
volatile PWRLib_WakeupReason_t PWRLib_MCU_WakeupReason;

#if (cPWR_UsePowerDownMode == 1)
/*****************************************************************************
 *                           PRIVATE FUNCTIONS PROTOTYPES                    *
 *---------------------------------------------------------------------------*
 * Add to this section all the functions prototypes that have local (file)   *
 * scope.                                                                    *
 * These functions cannot be accessed outside this module.                   *
 * These declarations shall be preceded by the 'static' keyword.             *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *                                PRIVATE FUNCTIONS                          *
 *---------------------------------------------------------------------------*
 * Add to this section all the functions that have local (file) scope.       *
 * These functions cannot be accessed outside this module.                   *
 * These definitions shall be preceded by the 'static' keyword.              *
 *---------------------------------------------------------------------------*
*****************************************************************************/

/*****************************************************************************
 *                             PUBLIC FUNCTIONS                              *
 *---------------------------------------------------------------------------*
 * Add to this section all the functions that have global (project) scope.   *
 * These functions can be accessed outside this module.                      *
 * These functions shall have their declarations (prototypes) within the     *
 * interface header file and shall be preceded by the 'extern' keyword.      *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
/*---------------------------------------------------------------------------
 * Name: PWRLib_ConfigRSIM
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_ConfigRSIM( uint8_t lpMode )
{
    RSIM->CONTROL &= ~(RSIM_CONTROL_RSIM_STOP_ACK_OVRD_EN_MASK);
    RSIM->RF_OSC_CTRL  = (RSIM->RF_OSC_CTRL & ~RSIM_RF_OSC_CTRL_RADIO_EXT_OSC_OVRD_MASK) | RSIM_RF_OSC_CTRL_RADIO_EXT_OSC_OVRD_EN_MASK;
    
    switch(lpMode)
    {
    case 1:
    case 7:  
#if cPWR_BLE_LL_Enable
        RSIM->CONTROL |= RSIM_CONTROL_BLE_RF_OSC_REQ_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_MASK;
#endif /* cPWR_BLE_LL_Enable */
        {
            uint32_t dsmControl = 0;
            
#if cPWR_GENFSK_LL_Enable
            dsmControl |= RSIM_DSM_CONTROL_DSM_TIMER_EN_MASK | 
                          RSIM_DSM_CONTROL_GEN_SYSCLK_REQ_INT_MASK |
                          RSIM_DSM_CONTROL_GEN_SYSCLK_INTERRUPT_EN_MASK |
                          RSIM_DSM_CONTROL_GEN_SYSCLK_REQUEST_EN_MASK;
#endif /* cPWR_GENFSK_LL_Enable */
            RSIM->DSM_CONTROL = dsmControl;
        }
        break;
    case 2:
    case 3:
    case 4:
    case 5:
#if cPWR_BLE_LL_Enable
        RSIM->CONTROL &= ~(RSIM_CONTROL_BLE_RF_OSC_REQ_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_EN_MASK);
#endif /* cPWR_BLE_LL_Enable */
        
#if cPWR_Z_LL_Enable
        RSIM->DSM_CONTROL &= ~(RSIM_DSM_CONTROL_ZIG_SYSCLK_REQUEST_EN_MASK | RSIM_DSM_CONTROL_ZIG_SYSCLK_INTERRUPT_EN_MASK);
#endif /* cPWR_Z_LL_Enable */
        
#if cPWR_ANT_LL_Enable
        RSIM->DSM_CONTROL &= ~(RSIM_DSM_CONTROL_ANT_SYSCLK_REQUEST_EN_MASK | RSIM_DSM_CONTROL_ANT_SYSCLK_INTERRUPT_EN_MASK);
#endif /* cPWR_ANT_LL_Enable */
        
#if cPWR_GENFSK_LL_Enable
        RSIM->DSM_CONTROL &= ~(RSIM_DSM_CONTROL_GEN_SYSCLK_REQUEST_EN_MASK | RSIM_DSM_CONTROL_GEN_SYSCLK_INTERRUPT_EN_MASK);
#endif /* cPWR_GENFSK_LL_Enable */
        break;
    case 6:
#if cPWR_BLE_LL_Enable
        RSIM->CONTROL &= ~(RSIM_CONTROL_BLE_RF_OSC_REQ_INT_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_EN_MASK) ;
#endif /* cPWR_BLE_LL_Enable */
        break;
    }
}

#if (cPWR_BLE_LL_Enable)
/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_ConfigDSM
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_ConfigDSM(uint8_t lpMode)
{
    BLE_LL_REG(WAKEUP_CONFIG) = (((cPWR_BLE_LL_OffsetToWakeupInstant << BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_SHIFT) & BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_MASK) |
                                 ((cPWR_BLE_LL_OscStartupDelay << BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SHIFT)& BLE_LL_WAKEUP_CONFIG_OSC_DELAY_MASK) |
                                 BLE_LL_WAKEUP_CONFIG_CON_RAM_DSM_RET);
    
    if(lpMode)
    {
        BLE_LL_REG(CLOCK_CONFIG) = maBLE_LL_CLOCK_CONFIG_LP[lpMode - 1];
    }
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_ClearRSIMInt
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
bool_t PWRLib_ClearRSIMInt(void)
{
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_INT))
    {
        RSIM->CONTROL = RSIM->CONTROL ;
        return TRUE;
    }
    if( (RSIM->CONTROL & RSIM_CONTROL_RSIM_DSM_EXIT_MASK) && (BLE_LL_REG(EVENT_STATUS) & BLE_LL_EVENT_STATUS_DSM_INT_MASK))
    {
        RSIM->CONTROL &= ~RSIM_CONTROL_RSIM_DSM_EXIT_MASK;
        BLE_LL_REG(EVENT_CLEAR) = BLE_LL_EVENT_CLEAR_DSM_INT_MASK;
    }
    return FALSE;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_GetOffsetSlotsBeforeDsmExit
 * Description: Returns dsm_offset_to_wakeup_instant + osc_startup_delay
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint16_t PWRLib_BLL_GetOffsetSlotsBeforeDsmExit(void)
{
    uint16_t wakeupConfig, offsetSlots;
    
    wakeupConfig =  BLE_LL_REG(WAKEUP_CONFIG);
    offsetSlots = (wakeupConfig & BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_MASK) >> BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_SHIFT;
    offsetSlots += (wakeupConfig & BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SLOT_MASK) >> BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SLOT_SHIFT;
    
    if(wakeupConfig & BLE_LL_WAKEUP_CONFIG_OSC_DELAY_CLK_MASK)
    {
        offsetSlots++;
    }
    
    return offsetSlots;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_EnterDSM
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_EnterDSM(void)
{
    //while((BLE_LL_REG(CLOCK_CONFIG) & BLE_LL_CLOCK_CONFIG_LLH_IDLE_MASK ) == 0);
    BLE_LL_REG(EVENT_CLEAR) = BLE_LL_EVENT_CLEAR_DSM_INT_MASK;
    BLE_LL_REG(COMMAND_REGISTER) = ENTER_DSM;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_GetInstantTimer
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint16_t PWRLib_BLL_GetInstantTimer(void)
{
    return BLE_LL_REG(TIM_COUNTER_L);
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_SetWakeupInstant
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_SetWakeupInstant(uint16_t wakeupInstant)
{
    BLE_LL_REG(WAKEUP_CONTROL) = wakeupInstant;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_WaitForDSMExit
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_WaitForDSMExit(void)
{
    while((BLE_LL_REG(EVENT_STATUS) & BLE_LL_EVENT_STATUS_DSM_INT_MASK) == 0);
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_GetIntEn
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint16_t PWRLib_BLL_GetIntEn(void)
{
    return BLE_LL_REG(EVENT_ENABLE);
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_DisableInterrupts
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_DisableInterrupts(uint16_t bmInterrupts)
{
    BLE_LL_REG(EVENT_ENABLE) &= ~ bmInterrupts;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_EnableInterrupts
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_EnableInterrupts(uint16_t bmInterrupts)
{
    BLE_LL_REG(EVENT_ENABLE) |= bmInterrupts;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_ClearInterrupts
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_ClearInterrupts(uint16_t bmInterrupts)
{
    BLE_LL_REG(EVENT_CLEAR) |= bmInterrupts;
}
#endif /* (cPWR_BLE_LL_Enable) */

/*---------------------------------------------------------------------------
 * Name: PWRLib_ClearRSIMDsmInt
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/  
bool_t PWRLib_ClearRSIMDsmInt(void)
{
#if cPWR_GENFSK_LL_Enable
    if (RSIM->DSM_CONTROL & RSIM_DSM_CONTROL_GEN_SYSCLK_REQ_INT_MASK)
    {
        RSIM->DSM_CONTROL = RSIM->DSM_CONTROL;
        
        return TRUE;
    }
#endif /* cPWR_GENFSK_LL_Enable */
    
    return FALSE;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_ConfigLLWU
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_ConfigLLWU(uint8_t lpMode)
{
    switch(lpMode)
    {
    case 1:
        LLWU->ME = gPWRLib_LLWU_WakeupModuleEnable_Radio_c | gPWRLib_LLWU_WakeupModuleEnable_LPTMR_c;
        break;
    case 2:
    case 3:
        LLWU->ME = gPWRLib_LLWU_WakeupModuleEnable_LPTMR_c | gPWRLib_LLWU_WakeupModuleEnable_DCDC_c;
        break;
    case 4:
    case 5:
        LLWU->ME = gPWRLib_LLWU_WakeupModuleEnable_DCDC_c;
        break;  
    case 6:
        LLWU->ME = 0;
        break;
    case 7:
        LLWU->ME = gPWRLib_LLWU_WakeupModuleEnable_Radio_c | gPWRLib_LLWU_WakeupModuleEnable_LPTMR_c;
        break;
    }
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_SetDeepSleepMode
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_SetDeepSleepMode(uint8_t lpMode)
{
    if(maLPModeUseLPTMR[lpMode])
    {
        PWRLib_LPTMR_Init();
    }
    else
    {
        if(maLPModeUseLPTMR[mPWRLib_DeepSleepMode])
        {
            PWRLib_LPTMR_DeInit();
        }
    }
    
    mPWRLib_DeepSleepMode = lpMode;  
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_GetDeepSleepMode
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint8_t PWRLib_GetDeepSleepMode(void)
{
    return mPWRLib_DeepSleepMode;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_Sleep
 * Description: Puts the processor into Sleep .

                Mode of operation details:
                 - ARM core enters Sleep Mode
                 - ARM core is clock gated (HCLK = OFF)
                 - peripherals are functional
                
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_Sleep(void)
{
    /* SCB_SCR: SLEEPDEEP=0 */
    SCB->SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPDEEP_Msk);
    
    /* SCB_SCR: SLEEPONEXIT=0 */
    SCB->SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPONEXIT_Msk);
    
    /* WFI instruction will start entry into deep sleep mode */
    __asm("WFI");
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_VLLS0
 * Description: Puts the processor into VLLS0 (Very Low Leakage Stop1).

                Mode of operation details:
                 - ARM core enters SleepDeep Mode
                 - ARM core is clock gated (HCLK = OFF)
                 - NVIC is disable (FCLK = OFF)
                 - LLWU should configure by user to enable the desire wake up source
                 - Platform and peripheral clock are stopped
                 - All SRAM powered off
                 - VLLS0 mode is exited into RUN mode using LLWU module or RESET.
                   All wakeup goes through Reset sequence.

                The AVLLS must be set to 0b1 in SMC_PMPROT register in order to allow VLLS mode.

 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_VLLS0(void)
{
    LLWU->F1 = LLWU->F1;
    LLWU->F2 = LLWU->F2;
    LLWU->FILT1 |= LLWU_FILT1_FILTF_MASK;
    LLWU->FILT2 |= LLWU_FILT2_FILTF_MASK;
    SCB->SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPONEXIT_Msk);
    /* Set the SLEEPDEEP bit to enable CORTEX M0 deep sleep mode */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    SMC->PMCTRL = (uint8_t)((SMC->PMCTRL & (uint8_t)~(uint8_t)(SMC_PMCTRL_STOPM(0x03))) | (uint8_t)(SMC_PMCTRL_STOPM(0x04)));
    
#if cPWR_POR_DisabledInVLLS0
    SMC->STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_PORPO_MASK | SMC_STOPCTRL_LLSM(0x00));
#else
    SMC->STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(0x00));
#endif /* cPWR_POR_DisabledInVLLS0 */
    
    (void)(SMC->PMCTRL == 0U);  /* Dummy read of SMC_PMCTRL to ensure the register is written before enterring low power mode */
    
    /* WFI instruction will start entry into deep sleep mode */
    __asm("WFI");
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_VLLS1
 * Description: Puts the processor into VLLS1 (Very Low Leakage Stop1).

                Mode of operation details:
                 - ARM core enters SleepDeep Mode
                 - ARM core is clock gated (HCLK = OFF)
                 - NVIC is disable (FCLK = OFF)
                 - LLWU should configure by user to enable the desire wake up source
                 - Platform and peripheral clock are stopped
                 - All SRAM powered off
                 - VLLS1 mode is exited into RUN mode using LLWU module or RESET.
                All wakeup goes through Reset sequence.

                The AVLLS must be set to 0b1 in MC_PMPROT register in order to allow VLLS mode.

 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_VLLS1(void)
{
    LLWU->F1 = LLWU->F1;
    LLWU->F2 = LLWU->F2;
    LLWU->FILT1 |= LLWU_FILT1_FILTF_MASK;
    LLWU->FILT2 |= LLWU_FILT2_FILTF_MASK;
    SCB->SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPONEXIT_Msk);
    /* Set the SLEEPDEEP bit to enable CORTEX M0 deep sleep mode */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    SMC->PMCTRL = (uint8_t)((SMC->PMCTRL & (uint8_t)~(uint8_t)(SMC_PMCTRL_STOPM(0x03))) | (uint8_t)(SMC_PMCTRL_STOPM(0x04)));
    SMC->STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(0x01));
    (void)(SMC->PMCTRL == 0U);  /* Dummy read of SMC_PMCTRL to ensure the register is written before enterring low power mode */
    
    /* WFI instruction will start entry into deep sleep mode */
    __asm("WFI");
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_VLLS2
 * Description: Puts the processor into VLLS1 (Very Low Leakage Stop1).

                Mode of operation details:
                 - ARM core enters SleepDeep Mode
                 - ARM core is clock gated (HCLK = OFF)
                 - NVIC is disable (FCLK = OFF)
                 - LLWU should configure by user to enable the desire wake up source
                 - Platform and peripheral clock are stopped
                 - All SRAM powered off
                 - VLLS1 mode is exited into RUN mode using LLWU module or RESET.
                All wakeup goes through Reset sequence.

                The AVLLS must be set to 0b1 in MC_PMPROT register in order to allow VLLS mode.

 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_VLLS2(void)
{
    LLWU->F1 = LLWU->F1;
    LLWU->F2 = LLWU->F2;
    LLWU->FILT1 |= LLWU_FILT1_FILTF_MASK;
    LLWU->FILT2 |= LLWU_FILT2_FILTF_MASK;
    SCB->SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPONEXIT_Msk);
    /* Set the SLEEPDEEP bit to enable CORTEX M0 deep sleep mode */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    SMC->PMCTRL = (uint8_t)((SMC->PMCTRL & (uint8_t)~(uint8_t)(SMC_PMCTRL_STOPM(0x03))) | (uint8_t)(SMC_PMCTRL_STOPM(0x04)));
    SMC->STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(0x02));
    (void)(SMC->PMCTRL == 0U);  /* Dummy read of SMC_PMCTRL to ensure the register is written before enterring low power mode */
    
    /* WFI instruction will start entry into deep sleep mode */
    __asm("WFI");
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_LLS2
 * Description: Puts the processor into LLS3 (Low Leakage Stop1).

                Mode of operation details:
                 - ARM core enters SleepDeep Mode
                 - ARM core is clock gated (HCLK = OFF)
                 - NVIC is disable (FCLK = OFF)
                 - LLWU should configure by user to enable the desire wake up source
                 - Platform and peripheral clock are stopped
                 - Full SRAM retention.               

                The ALLS must be set to 0b1 in SMC_PMPROT register in order to allow LLS mode.

 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_LLS2(void)
{
    LLWU->F1 = LLWU->F1;
    LLWU->F2 = LLWU->F2;
    LLWU->FILT1 |= LLWU_FILT1_FILTF_MASK;
    LLWU->FILT2 |= LLWU_FILT2_FILTF_MASK;
    SCB->SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPONEXIT_Msk);
    /* Set the SLEEPDEEP bit to enable CORTEX M0 deep sleep mode */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    SMC->PMCTRL = (uint8_t)((SMC->PMCTRL & (uint8_t)~(uint8_t)(SMC_PMCTRL_STOPM(0x04))) | (uint8_t)(SMC_PMCTRL_STOPM(0x03)));
    SMC->STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(0x02)) | SMC_STOPCTRL_RAM2PO(cPWR_RAM2_EnabledInLLS2);
    (void)(SMC->PMCTRL == 0U);  /* Dummy read of SMC_PMCTRL to ensure the register is written before enterring low power mode */
    
    /* WFI instruction will start entry into deep sleep mode */
    __asm("WFI");
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_LLS3
 * Description: Puts the processor into LLS3 (Low Leakage Stop1).

                Mode of operation details:
                 - ARM core enters SleepDeep Mode
                 - ARM core is clock gated (HCLK = OFF)
                 - NVIC is disable (FCLK = OFF)
                 - LLWU should configure by user to enable the desire wake up source
                 - Platform and peripheral clock are stopped
                 - Full SRAM retention.               

                The ALLS must be set to 0b1 in SMC_PMPROT register in order to allow LLS mode.

 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_LLS3(void)
{
    LLWU->F1 = LLWU->F1;
    LLWU->F2 = LLWU->F2;
    LLWU->FILT1 |= LLWU_FILT1_FILTF_MASK;
    LLWU->FILT2 |= LLWU_FILT2_FILTF_MASK;
    SCB->SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPONEXIT_Msk);
    /* Set the SLEEPDEEP bit to enable CORTEX M0 deep sleep mode */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    SMC->PMCTRL = (uint8_t)((SMC->PMCTRL & (uint8_t)~(uint8_t)(SMC_PMCTRL_STOPM(0x04))) | (uint8_t)(SMC_PMCTRL_STOPM(0x03)));
    SMC->STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(0x03));
    (void)(SMC->PMCTRL == 0U);  /* Dummy read of SMC_PMCTRL to ensure the register is written before enterring low power mode */
    
    /* WFI instruction will start entry into deep sleep mode */
    __asm("WFI");
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_Stop
 * Description: Puts the processor into Stop 

                Mode of operation details:
                 - ARM core enters SleepDeep Mode
                 - ARM core is clock gated (HCLK = OFF)
                 - NVIC is disable (FCLK = OFF)
                 - Platform and peripheral clock are stopped
                 - Full SRAM retention.               
                The ALLS must be set to 0b1 in SMC_PMPROT register in order to allow LLS mode.

 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_Stop(void)
{
    SCB->SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPONEXIT_Msk);
    /* Set the SLEEPDEEP bit to enable CORTEX M0 deep sleep mode */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    SMC->PMCTRL = (uint8_t)(SMC->PMCTRL & (uint8_t)~(uint8_t)(SMC_PMCTRL_STOPM(0x07)));
    SMC->STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(0x00));
    (void)(SMC->PMCTRL == 0U);  /* Dummy read of SMC_PMCTRL to ensure the register is written before enterring low power mode */
    
    /* WFI instruction will start entry into deep sleep mode */
    __asm("WFI");
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_GetWakeUpFlags
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint32_t PWRLib_LLWU_GetWakeUpFlags(void)
{
    uint32_t Flags;
    
    Flags = LLWU->F1;
    Flags |= (uint32_t)((uint32_t)LLWU->F2 << 8U);
    Flags |= (uint32_t)((uint32_t)LLWU->F3 << 16U);
    
    if ((LLWU->FILT1 & 0x80U) != 0x00U ) 
    {
        Flags |= LLWU_FILTER1;
    }
    
    if ((LLWU->FILT2 & 0x80U) != 0x00U ) 
    {
        Flags |= LLWU_FILTER2;
    }
    
    return Flags;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_UpdateWakeupReason
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LLWU_UpdateWakeupReason(void)
{
    uint32_t  llwuFlags;
    
    llwuFlags =  PWRLib_LLWU_GetWakeUpFlags();
    
#if BOARD_LLWU_PIN_ENABLE_BITMAP
    if(llwuFlags & BOARD_LLWU_PIN_ENABLE_BITMAP)
    {
        PWRLib_MCU_WakeupReason.Bits.FromKeyBoard = 1; 
    }
#endif /* BOARD_LLWU_PIN_ENABLE_BITMAP */
    
    if(llwuFlags & gPWRLib_LLWU_WakeupModuleFlag_LPTMR_c)
    {
        PWRLib_MCU_WakeupReason.Bits.FromLPTMR = 1;
        PWRLib_MCU_WakeupReason.Bits.DeepSleepTimeout = 1;
    }
    
    if(llwuFlags & gPWRLib_LLWU_WakeupModuleFlag_Radio_c)
    {
        PWRLib_MCU_WakeupReason.Bits.FromBLE_LLTimer = 1;
        
        if(RSIM->DSM_CONTROL & (RSIM_DSM_CONTROL_GEN_SYSCLK_REQ_INT_MASK))
        {
            PWRLib_MCU_WakeupReason.Bits.FromRadio = 1;
        }
        
        PWRLib_MCU_WakeupReason.Bits.DeepSleepTimeout = 1;
    }
    
    if(llwuFlags & gPWRLib_LLWU_WakeupModuleFlag_DCDC_c)
    {
        PWRLib_MCU_WakeupReason.Bits.FromPSwitch_UART = 1;
    }
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_DisableAllWakeupSources
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LLWU_DisableAllWakeupSources(void)
{
    LLWU->PE1 = 0;
    LLWU->PE2 = 0;
    LLWU->PE3 = 0;
    LLWU->PE4 = 0;
    LLWU->ME = 0;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_GetTimeSettings
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_GetTimeSettings(uint32_t timeInMs, uint8_t* pClkMode,uint32_t* pTicks, uint32_t* pFreq)
{
    uint64_t ticks64;
    uint32_t ticks;
    uint8_t prescaler = 0;
    uint8_t bypass = LPTMR_PSR_PBYP_MASK;
    uint32_t freq = 32768;
    
    if(timeInMs > mLptmrTimoutMaxMs_c)
    {
        timeInMs = mLptmrTimoutMaxMs_c;
    }
    
    ticks64 = timeInMs;
    ticks64 *= 32768;
    ticks64 /= 1000;
    ticks = ticks64;
    
    while(ticks & 0xFFFF0000)
    {
        if(bypass)
        {
            bypass = 0;
        }
        else
        {
            prescaler++;
        }
        
        ticks >>= 1;
        freq >>= 1;
    }
    
    *pClkMode = (prescaler<<LPTMR_PSR_PRESCALE_SHIFT)| bypass | cLPTMR_Source_Ext_ERCLK32K;
    *pTicks = ticks;
    *pFreq = freq;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockStart
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_ClockStart(uint8_t ClkMode, uint32_t Ticks)
{
    OSA_InterruptDisable();
    
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
    
    /* Set compare value */
    if(Ticks)
    {
        Ticks--;
    }
    
    LPTMR0->CMR = Ticks;
    /* Use specified tick count */
    mPWRLib_RTIElapsedTicks = 0;
    /* Configure prescaler, bypass prescaler and clck source */
    LPTMR0->PSR = ClkMode;
    LPTMR0->CSR |= LPTMR_CSR_TFC_MASK;
    /* Start counting */
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
    
    OSA_InterruptEnable();
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockCheck
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint32_t PWRLib_LPTMR_ClockCheck(void)
{
    OSA_InterruptDisable();
    
    /* LPTMR is still running */
    if(LPTMR0->CSR & LPTMR_CSR_TEN_MASK)
    {
        LPTMR0->CNR = 0;  /* CNR must be written first in order to be read */
        mPWRLib_RTIElapsedTicks = LPTMR0->CNR;
        
        /* timer compare flag is set */
        if(LPTMR0->CSR & LPTMR_CSR_TCF_MASK)
        {
            uint32_t compareReg;
            
            compareReg = LPTMR0->CMR;
            
            if(mPWRLib_RTIElapsedTicks < compareReg)
            {
                mPWRLib_RTIElapsedTicks += 0x10000;
            }
        }
    }
    
    OSA_InterruptEnable();
    
    return mPWRLib_RTIElapsedTicks;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockStop
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_ClockStop(void)
{
    OSA_InterruptDisable();
    
    /* LPTMR is still running */
    if(LPTMR0->CSR & LPTMR_CSR_TEN_MASK)
    {
        LPTMR0->CNR = 0;  /* CNR must be written first in order to be read */
        mPWRLib_RTIElapsedTicks = LPTMR0->CNR;
        
        /* timer compare flag is set */
        if(LPTMR0->CSR & LPTMR_CSR_TCF_MASK)
        {
            uint32_t compareReg;
            
            compareReg = LPTMR0->CMR;
            
            if(mPWRLib_RTIElapsedTicks < compareReg)
            {
                mPWRLib_RTIElapsedTicks += 0x10000;
            }
        }
    }
    
    /* Stop LPTMR */
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
    
    OSA_InterruptEnable();
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_StopUpdateWakeupReason
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_StopUpdateWakeupReason(void)
{
    uint32_t iser;
    uint32_t ispr;
    
    iser = NVIC->ISER[0];
    ispr = NVIC->ISPR[0];
    
    if(( iser & ispr) & (1 << LPUART0_IRQn))
    {
        PWRLib_MCU_WakeupReason.Bits.FromPSwitch_UART = 1;
    }
    
    if(( iser & ispr) & (1 << LVD_LVW_DCDC_IRQn))
    {
        PWRLib_MCU_WakeupReason.Bits.FromPSwitch_UART = 1;
    }
    
    if((iser & ispr) & (1 << Radio_0_IRQn | 1 << Radio_1_IRQn))
    {
        PWRLib_MCU_WakeupReason.Bits.FromRadio = 1;
#if cPWR_GENFSK_LL_Enable
        if((GENFSK->IRQ_CTRL & (GENFSK_IRQ_CTRL_WAKE_IRQ_EN_MASK | GENFSK_IRQ_CTRL_GENERIC_FSK_IRQ_EN_MASK |GENFSK_IRQ_CTRL_WAKE_IRQ_MASK)) == 
           (GENFSK_IRQ_CTRL_WAKE_IRQ_EN_MASK | GENFSK_IRQ_CTRL_GENERIC_FSK_IRQ_EN_MASK |GENFSK_IRQ_CTRL_WAKE_IRQ_MASK))
        {
            PWRLib_MCU_WakeupReason.Bits.DeepSleepTimeout = 1;
        }        
#endif /* cPWR_GENFSK_LL_Enable */
    }
    
    if((iser & ispr) & (1 << LPTMR0_IRQn))
    {
        PWRLib_MCU_WakeupReason.Bits.FromLPTMR = 1;
        PWRLib_MCU_WakeupReason.Bits.DeepSleepTimeout = 1;   
    }
    
    if(KBD_IsWakeUpSource())
    {
        PWRLib_MCU_WakeupReason.Bits.FromKeyBoard = 1;
    }
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_Isr
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_Isr(void)
{
    LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_Init
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_Init(void)
{
    OSA_InstallIntHandler(LPTMR0_IRQn,PWRLib_LPTMR_Isr);
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;
    
    LPTMR0->CSR = (LPTMR_CSR_TCF_MASK | LPTMR_CSR_TPS(0x00));  /* Clear control register */
    LPTMR0->CMR = LPTMR_CMR_COMPARE(0x02);  /* Set up compare register */
    NVIC->IP[7] &= ~0x000000FFu;
    NVIC->IP[7] |=  0x80;
    NVIC->ISER[0] |= 0x10000000;
    LPTMR0->CSR = (LPTMR_CSR_TIE_MASK | LPTMR_CSR_TPS(0x00));  /* Set up control register */    
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_DeInit
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_DeInit(void)
{
    LPTMR0->CSR = LPTMR_CSR_TCF_MASK;  /* Clear control register */
    SIM->SCGC5 &= ~SIM_SCGC5_LPTMR_MASK;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_Isr
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LLWU_Isr(void)
{
    uint32_t llwuFlags;
    
    llwuFlags = PWRLib_LLWU_GetWakeUpFlags();
    /* Clear external pins wakeup interrupts */
    LLWU->F1 = LLWU->F1; 
    LLWU->F2 = LLWU->F2; 
    
    /* LPTMR is wakeup source */
    if(llwuFlags & gPWRLib_LLWU_WakeupModuleFlag_LPTMR_c)  
    {
        /* Clear LPTMR interrupt */
        LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;
    }
    
    if(llwuFlags & gPWRLib_LLWU_WakeupModuleFlag_Radio_c)  
    {
        /* Clear RSIM interrupt */    
        if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_INT))
        {
            RSIM->CONTROL = RSIM->CONTROL;
        }
        
        RSIM->DSM_CONTROL = RSIM->DSM_CONTROL;
    }
    
    if(llwuFlags & gPWRLib_LLWU_WakeupModuleFlag_DCDC_c)  
    {
        /* Clear pSwitch interrupt */
        DCDC->REG6 |= DCDC_REG6_PSWITCH_INT_CLEAR_MASK;
        DCDC->REG6 &= ~DCDC_REG6_PSWITCH_INT_CLEAR_MASK;
    }
}
#endif /* #if (cPWR_UsePowerDownMode==1) */

/*---------------------------------------------------------------------------
* Name: PWRLib_LVD_CollectLevel
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
PWRLib_LVD_VoltageLevel_t PWRLib_LVD_CollectLevel(void)
{
#if ((cPWR_LVD_Enable == 1) || (cPWR_LVD_Enable == 2))
    
    /* Check low detect voltage 1.6V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(0);
    PMC->LVDSC1 = PMC_LVDSC1_LVDACK_MASK;
    
    if(PMC->LVDSC1 & PMC_LVDSC1_LVDF_MASK)
    {
        /* Low detect voltage reached */
        PMC->LVDSC1 = PMC_LVDSC1_LVDACK_MASK;
        
        return(PWR_LEVEL_CRITICAL);
    }
    
    /* Check low trip voltage 1.8V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(0);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;
    
    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */
        
        return(PWR_BELOW_LEVEL_1_8V);
    }
    
    /* Check low trip voltage 1.9V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(1);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;
    
    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */
        
        return(PWR_BELOW_LEVEL_1_9V);
    }
    
    /* Check low trip voltage 2.0V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(2);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;
    
    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */
        
        return(PWR_BELOW_LEVEL_2_0V);
    }
    
    /* Check low trip voltage 2.1V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(3);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;
    
    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */
        
        return(PWR_BELOW_LEVEL_2_1V);
    }
    
    /* Check low detect voltage (high range) 2.56V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(1);  /* Set high trip voltage and clear warning flag */
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(0);
    PMC->LVDSC1 |= PMC_LVDSC1_LVDACK_MASK;
    
    if(PMC->LVDSC1 & PMC_LVDSC1_LVDF_MASK)
    {
        /* Low detect voltage reached */
        PMC->LVDSC1 = PMC_LVDSC1_LVDACK_MASK;  /* Set low trip voltage and clear warning flag */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */
        
        return(PWR_BELOW_LEVEL_2_56V);
    }
    
    /* Check high trip voltage 2.7V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(1);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(0);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;
    
    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */
        
        return(PWR_BELOW_LEVEL_2_7V);
    }
    
    /* Check high trip voltage 2.8V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(1);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(1);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;
    
    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */
        
        return(PWR_BELOW_LEVEL_2_8V);
    }
    
    /* Check high trip voltage 2.9V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(1);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(2);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;
    
    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */
        
        return(PWR_BELOW_LEVEL_2_9V);
    }
    
    /* Check high trip voltage 3.0V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(1);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(3);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;
    
    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */
        
        return(PWR_BELOW_LEVEL_3_0V);
    }
    
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(0);
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */
#endif /* ((cPWR_LVD_Enable == 1) || (cPWR_LVD_Enable == 2)) */
    
    /*--- Voltage level is okay > 3.0V */
    return(PWR_ABOVE_LEVEL_3_0V);
}

/******************************************************************************
 * Name: PWRLib_LVD_PollIntervalCallback
 * Description:
 *
 * Parameter(s): -
 * Return: -
 ******************************************************************************/
#if (cPWR_LVD_Enable == 2)
static void PWRLib_LVD_PollIntervalCallback(void* param)
{
    (void)param;
    PWRLib_LVD_SavedLevel = PWRLib_LVD_CollectLevel();
}
#endif /* (cPWR_LVD_Enable == 2) */

/*---------------------------------------------------------------------------
 * Name: PWRLib_Init
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_Init(void)
{
#if (cPWR_UsePowerDownMode == 1)
    
#if cPWR_GENFSK_LL_Enable
    SIM->SCGC5 |= SIM_SCGC5_GEN_FSK_MASK;
    GENFSK->IRQ_CTRL |= GENFSK_IRQ_CTRL_WAKE_IRQ_EN_MASK | GENFSK_IRQ_CTRL_GENERIC_FSK_IRQ_EN_MASK;
#endif /* cPWR_GENFSK_LL_Enable */
    
    SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK ;
    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
    
    if ((RTC->CR & RTC_CR_OSCE_MASK) == 0u) 
    {
        /* RTC_CR: SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
        RTC->CR &= (uint32_t)~(uint32_t)( RTC_CR_SC2P_MASK | RTC_CR_SC4P_MASK | RTC_CR_SC8P_MASK | RTC_CR_SC16P_MASK );
        /* RTC_CR: OSCE=1 */
        RTC->CR |= RTC_CR_OSCE_MASK;
        /* RTC_CR: CLKO=0 */
        RTC->CR &= (uint32_t)~(uint32_t)(RTC_CR_CLKO_MASK);
    }
    
    if(RCM->SRS0 & RCM_SRS0_WAKEUP_MASK)
    {
        PWRLib_LLWU_UpdateWakeupReason();
        
        if(RCM->SRS0 & RCM_SRS0_PIN_MASK)
        {
            PWRLib_MCU_WakeupReason.Bits.FromReset = 1;
        }
    }
#if BOARD_LLWU_PIN_ENABLE_BITMAP
    {
        uint16_t pinEn16 = BOARD_LLWU_PIN_ENABLE_BITMAP;
        uint32_t PinEn32 = 0;
        uint32_t i;
        for(i=0; pinEn16 ; i++)
        {
            if(pinEn16 & 0x1)
            {
                PinEn32 |= 0x3<<(i<<1);
            }
            pinEn16 >>= 1;
        }
        LLWU->PE1 = PinEn32&0xff;
        LLWU->PE2 = (PinEn32>>8)&0xff;
        LLWU->PE3 = (PinEn32>>16)&0xff;
        LLWU->PE4 = (PinEn32>>24)&0xff;
    }
#endif /* BOARD_LLWU_PIN_ENABLE_BITMAP */
    
    /* LLWU_FILT1: FILTF=1,FILTE=0,??=0,FILTSEL=0 */
    LLWU->FILT1 = LLWU_FILT1_FILTF_MASK |
                  LLWU_FILT1_FILTE(0x00) |
                  LLWU_FILT1_FILTSEL(0x00);
    
    /* LLWU_FILT2: FILTF=1,FILTE=0,??=0,FILTSEL=0 */
    LLWU->FILT2 = LLWU_FILT2_FILTF_MASK |
                  LLWU_FILT2_FILTE(0x00) |
                  LLWU_FILT2_FILTSEL(0x00);
    
    /* SMC_PMPROT: ??=0,??=0,AVLP=1,??=0,ALLS=1,??=0,AVLLS=1,??=0 */
    SMC->PMPROT = SMC_PMPROT_AVLP_MASK |
                  SMC_PMPROT_ALLS_MASK |
                  SMC_PMPROT_AVLLS_MASK;  /* Setup Power mode protection register */
    
    /* Install LLWU ISR and validate it in NVIC */
    OSA_InstallIntHandler(LLWU_IRQn, PWRLib_LLWU_Isr);
    NVIC_EnableIRQ(LLWU_IRQn);
    NVIC_SetPriority(LLWU_IRQn, 2);
    
#if (gKeyBoardSupported_d == 0) 
#if gLowPower_switchPinsToInitBitmap_d
    {
        uint32_t pinIndex = gLowPower_switchPinsToInitBitmap_d;
        uint32_t i;
        
        for(i = 0; pinIndex; i++)
        {
            if(pinIndex & 0x1)
            {
                (void)GpioInputPinInit(&switchPins[i], 1);
            }
            
            pinIndex >>= 1;
        }
    }
#endif /* gLowPower_switchPinsToInitBitmap_d */
#endif /* (gKeyBoardSupported_d == 0) */
    
#if cPWR_BLE_LL_Enable 
    if((SIM->SCGC5 & SIM_SCGC5_BTLL_MASK) == 0)
    {
        SIM->SCGC5 |= SIM_SCGC5_BTLL_MASK;
    }
    
    PWRLib_BLL_ConfigDSM(mPWRLib_DeepSleepMode);
#endif  /* cPWR_BLE_LL_Enable */
    
    PWRLib_ConfigRSIM(mPWRLib_DeepSleepMode);
    PWRLib_ConfigLLWU(mPWRLib_DeepSleepMode);
    
    if(maLPModeUseLPTMR[mPWRLib_DeepSleepMode])
    {
        PWRLib_LPTMR_Init();
    }
    
#endif /* (cPWR_UsePowerDownMode == 1) */
    
    /* LVD_Init TBD */
#if (cPWR_LVD_Enable == 0)
    PMC->LVDSC1 &= (uint8_t) ~( PMC_LVDSC1_LVDIE_MASK  | PMC_LVDSC1_LVDRE_MASK);
    PMC->LVDSC2 &= (uint8_t) ~( PMC_LVDSC2_LVWIE_MASK );
#elif ((cPWR_LVD_Enable == 1) || (cPWR_LVD_Enable == 2))
    PMC->LVDSC1 &= (uint8_t) ~( PMC_LVDSC1_LVDIE_MASK | PMC_LVDSC1_LVDRE_MASK);
    PMC->LVDSC2 &= (uint8_t) ~( PMC_LVDSC2_LVWIE_MASK );
#elif (cPWR_LVD_Enable==3)
    PMC->LVDSC1 = (PMC->LVDSC1 | (uint8_t)PMC_LVDSC1_LVDRE_MASK) & (uint8_t)(~PMC_LVDSC1_LVDIE_MASK );
    PMC->LVDSC2 &= (uint8_t) ~( PMC_LVDSC2_LVWIE_MASK );
#endif /* (cPWR_LVD_Enable) */

#if (cPWR_LVD_Enable == 2)
#if ((cPWR_LVD_Ticks == 0) || (cPWR_LVD_Ticks > 71582))
#error  "*** ERROR: cPWR_LVD_Ticks invalid value"
#endif /* ((cPWR_LVD_Ticks == 0) || (cPWR_LVD_Ticks > 71582)) */
    
    PWRLib_LVD_SavedLevel = PWRLib_LVD_CollectLevel(); 
    
    /* Allocate a platform timer */
    PWRLib_LVD_PollIntervalTmrID = TMR_AllocateTimer();
    
    if(gTmrInvalidTimerID_c != PWRLib_LVD_PollIntervalTmrID)
    {
        /* start the timer */
        TMR_StartLowPowerTimer(PWRLib_LVD_PollIntervalTmrID, gTmrIntervalTimer_c,TmrMinutes(cPWR_LVD_Ticks) , PWRLib_LVD_PollIntervalCallback, NULL);
    }
#endif /* (cPWR_LVD_Enable == 2) */
}

