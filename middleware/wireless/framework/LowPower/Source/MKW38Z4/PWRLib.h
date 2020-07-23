/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
* All rights reserved.
 *
 * \file
 *
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef __PWR_LIB_H__
#define __PWR_LIB_H__

/*****************************************************************************
 *                               INCLUDED HEADERS                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the headers that this module needs to include.    *
 * Note that it is not a good practice to include header files into header   *
 * files, so use this section only if there is no other better solution.     *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#include "PWR_Interface.h"
#include "board.h"

/*****************************************************************************
 *                             PUBLIC MACROS                                *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#if defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1)
extern uint32_t* p_gcov_RCM;
#define RCM_REG ((RCM_Type *)p_gcov_RCM)
extern uint32_t gcov_RSIM_CONTROL;
#define RSIM_CONTROL_REG (gcov_RSIM_CONTROL)
extern uint16_t gcov_BLE_REG_EVENT_STATUS;
#define BLE_LL_REG_EVENT_STATUS (gcov_BLE_REG_EVENT_STATUS)
extern uint16_t gcov_BLE_REG_CLOCK_CONFIG;
#define BLE_LL_REG_CLOCK_CONFIG (gcov_BLE_REG_CLOCK_CONFIG)
extern uint32_t gcov_VECTOR_PENDING;
#define VECTOR_PENDING gcov_VECTOR_PENDING
extern uint32_t gcov_PMC_REGSC;
#define PMC_REGSC (gcov_PMC_REGSC)
extern uint32_t gcov_SIM_SCGC5;
#define SIM_REG_SCGC5 gcov_SIM_SCGC5
extern uint32_t gcov_LLWU_REG_FILT1;
#define LLWU_REG_FILT1 gcov_LLWU_REG_FILT1
extern uint32_t gcov_LLWU_REG_FILT2;
#define LLWU_REG_FILT2 gcov_LLWU_REG_FILT2
#endif

#ifndef PMC_REGSC
#define PMC_REGSC PMC->REGSC
#endif

#ifndef LLWU_REG_FILT1
#define LLWU_REG_FILT1  LLWU->FILT1
#endif

#ifndef LLWU_REG_FILT2
#define LLWU_REG_FILT2  LLWU->FILT2
#endif

#ifndef SIM_REG_SCGC5
#define SIM_REG_SCGC5  SIM->SCGC5
#endif


/* BLE LL registers */
#define COMMAND_REGISTER        (0x00U)  /* 32-bit:0x000 */
#define EVENT_CLEAR             (0x04U)  /* 32-bit:0x008 */
#define EVENT_STATUS            (0x04U)  /* 32-bit:0x008 */
#define EVENT_ENABLE            (0x08U)  /* 32-bit:0x010 */
#define ENGINE_INDEX            (0x58U)  /* 32-bit:0x0B0 */
#define WAKEUP_CONFIG           (0x5CU)  /* 32-bit:0x0B8 */
#define WAKEUP_CONTROL          (0x60U)  /* 32-bit:0x0C0 */
#define CLOCK_CONFIG            (0x62U)  /* 32-bit:0x0C4 */
#define TIM_COUNTER_L           (0x64U)  /* 32-bit:0x0C8 */
#define DATA_LIST_SENT_STATUS   (0x94U)  /* 32-bit:0x128 */
#define DATA_LIST_ACK_STATUS    (0x96U)  /* 32-bit:0x12C */

/* BLE LL Instructions */
#define ENTER_DSM               (0x50U)
#define ENTER_DSM3              (0x77U)

#define BLE_LL_REG(offset)  (*((volatile uint16_t*)(BTLE_RF_BASE + (offset))))
#define BLE_LL_EVENT_CLEAR_DSM_INT_MASK                (1U<<5)
#define BLE_LL_EVENT_CLEAR_SM_INT_MASK                 (1U<<4)
#define BLE_LL_EVENT_STATUS_LLH_IDLE_INT_MASK          (1U<<7)
#define BLE_LL_EVENT_STATUS_DSM_INT_MASK               (1U<<5)
#define BLE_LL_EVENT_STATUS_SM_INT_MASK                (1U<<4)
#define BLE_LL_EVENT_ENABLE_LLH_IDLE_INT_MASK          (1U<<7)
#define BLE_LL_EVENT_ENABLE_DSM_INT_MASK               (1U<<5)
#define BLE_LL_EVENT_ENABLE_SM_INT_MASK                (1U<<4)
#define BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_SHIFT          (10)
#define BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_MASK           (0xFC00U)
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SHIFT           (0)
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_MASK            (0x00FFU)
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SLOT_SHIFT      (5)
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SLOT_MASK       (0x00E0U)
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_CLK_SHIFT       (0)
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_CLK_MASK        (0x001FU)
#define BLE_LL_WAKEUP_CONFIG_CON_RAM_DSM_RET           (0x0100U)

#define BLE_LL_CLOCK_CONFIG_DSM_EN_MASK                ((uint16_t)1U<<15)
#define BLE_LL_CLOCK_CONFIG_SM_EN_MASK                 ((uint16_t)1U<<14)
#define BLE_LL_CLOCK_CONFIG_DSM_INT_EN_MASK            ((uint16_t)1U<<13)
#define BLE_LL_CLOCK_CONFIG_SM_INT_EN_MASK             ((uint16_t)1U<<12)
#define BLE_LL_CLOCK_CONFIG_DSM_AUTO_WKUP_MASK         ((uint16_t)1U<<11)
#define BLE_LL_CLOCK_CONFIG_SM_AUTO_WKUP_MASK          ((uint16_t)1U<<10)
#define BLE_LL_CLOCK_CONFIG_LPO_EXT_MASK               ((uint16_t)1U<<9)
#define BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK          ((uint16_t)1U<<8)
#define BLE_LL_CLOCK_CONFIG_LLH_IDLE_MASK              ((uint16_t)1U<<7)
#define BLE_LL_CLOCK_CONFIG_PHY_CLK_GATE_EN_MASK       ((uint16_t)1U<<6)
#define BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK       ((uint16_t)1U<<5)
#define BLE_LL_CLOCK_CONFIG_CORE_CLK_GATE_EN_MASK      ((uint16_t)1U<<4)
#define BLE_LL_CLOCK_CONFIG_CONN_CLK_GATE_EN_MASK      ((uint16_t)1U<<3)
#define BLE_LL_CLOCK_CONFIG_INIT_CLK_GATE_EN_MASK      ((uint16_t)1U<<2)
#define BLE_LL_CLOCK_CONFIG_SCAN_CLK_GATE_EN_MASK      ((uint16_t)1U<<1)
#define BLE_LL_CLOCK_CONFIG_ADV_CLK_GATE_EN_MASK       ((uint16_t)1U<<0)

#ifndef BLE_LL_REG_EVENT_STATUS
#define BLE_LL_REG_EVENT_STATUS BLE_LL_REG(EVENT_STATUS)
#endif

#ifndef BLE_LL_REG_CLOCK_CONFIG
#define BLE_LL_REG_CLOCK_CONFIG BLE_LL_REG(CLOCK_CONFIG)
#endif

/*****************************************************************************
 *                             PRIVATE MACROS                                *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...
 *---------------------------------------------------------------------------*
 *****************************************************************************/
/* Definitions for LPTMR timer setup */

/* LPTMR prescaler clocking options */
#define cLPTMR_Source_Int_MCGIRCLK    0
#define cLPTMR_Source_Int_LPO_1KHz    1
#define cLPTMR_Source_Ext_ERCLK32K    2
#define cLPTMR_Source_Ext_OSCERCLK    3

#ifndef RSIM_CONTROL_REG
#define RSIM_CONTROL_REG (RSIM->CONTROL)
#endif

#define RSIM_CONTROL_RF_OSC_EN_ASSIGN(x)                (((uint32_t)(((uint32_t)(x)) << RSIM_CONTROL_RF_OSC_EN_SHIFT)) & RSIM_CONTROL_RF_OSC_EN_MASK)
#define RSIM_CONTROL_RSIM_DSM_EXIT_ASSIGN(x)            (((uint32_t)(((uint32_t)(x)) << RSIM_CONTROL_RSIM_DSM_EXIT_SHIFT)) & RSIM_CONTROL_RSIM_DSM_EXIT_MASK)

#define RSIM_CONTROL_READ_FIELD(fieldName)  (((RSIM_CONTROL_REG) & (fieldName##_MASK)) >> (fieldName##_SHIFT))

#define READ_REGISTER_FIELD(reg,fieldName)  (((reg) & (fieldName##_MASK)) >> (fieldName##_SHIFT))
#define RSIM_CONTROL_MODIFY_FIELD(fieldName, value)  (((RSIM->CONTROL) & (~(fieldName##_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_MASK))) | fieldName##_ASSIGN(value))

/*****************************************************************************
 *                        PRIVATE TYPE DEFINITIONS                           *
 *---------------------------------------------------------------------------*
 * Add to this section all the data types definitions: stuctures, unions,    *
 * enumerations, typedefs ...                                                *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
/* Constants defined for use with GetLLSWakeUpFlags method */
#define LLWU_INT_MODULE0               0x00010000U  /* WakeUp caused by - Internal module 0 (LPTMR0) */
#define LLWU_INT_MODULE1               0x00020000U  /* WakeUp caused by - Internal module 1 (CMP0) */
#define LLWU_INT_MODULE2               0x00040000U  /* WakeUp caused by - Internal module 2 (BTLL) */
#define LLWU_INT_MODULE3               0x00080000U  /* WakeUp caused by - Internal module 3 (DCDC) */
#define LLWU_INT_MODULE4               0x00100000U  /* WakeUp caused by - Internal module 4 (TSI0) */
#define LLWU_INT_MODULE5               0x00200000U  /* WakeUp caused by - Internal module 5 (RTC) */
#define LLWU_INT_MODULE7               0x00800000U  /* WakeUp caused by - Internal module 7 (RTC_Seconds) */
#define LLWU_FILTER1                   0x01000000U  /* WakeUp caused by digital filter 1 */
#define LLWU_FILTER2                   0x02000000U  /* WakeUp caused by digital filter 2 */

#define  gPWRLib_LLWU_WakeupModuleFlag_LPTMR_c      LLWU_INT_MODULE0
#define  gPWRLib_LLWU_WakeupModuleFlag_CMP0_c       LLWU_INT_MODULE1
#define  gPWRLib_LLWU_WakeupModuleFlag_Radio_c      LLWU_INT_MODULE2
#define  gPWRLib_LLWU_WakeupModuleFlag_DCDC_c       LLWU_INT_MODULE3
#define  gPWRLib_LLWU_WakeupModuleFlag_TSI_c        LLWU_INT_MODULE5

#define  gPWRLib_LLWU_WakeupModuleEnable_LPTMR_c      (1U)
#define  gPWRLib_LLWU_WakeupModuleEnable_CMP0_c       (1U<<1)
#define  gPWRLib_LLWU_WakeupModuleEnable_Radio_c      (1U<<2)
#define  gPWRLib_LLWU_WakeupModuleEnable_DCDC_c       (1U<<3)
#define  gPWRLib_LLWU_WakeupModuleEnable_RTC_Alarm_c  (1U<<4)

#define PWRLIB_VLLS_0                           0u
#define PWRLIB_VLLS_1                           1u
#define PWRLIB_VLLS_2                           2u
#define PWRLIB_VLLS_3                           3u

#define PWRLIB_LLS_2                            2u
#define PWRLIB_LLS_3                            3u

#define PWRLIB_FLASH_TO_ERASE                   PWRLIB_RAM_SAVE_COUNT_DOWN
#define PWRLIB_RAM_TO_BE_SAVED                  1u
#define PWRLIB_RAM_SAVED                        0u

#ifndef RCM_REG
#define RCM_REG 								RCM
#endif

/*****************************************************************************
 *                               PUBLIC VARIABLES(External)                  *
 *---------------------------------------------------------------------------*
 * Add to this section all the variables and constants that have global      *
 * (project) scope.                                                          *
 * These variables / constants can be accessed outside this module.          *
 * These variables / constants shall be preceded by the 'extern' keyword in  *
 * the interface header.                                                     *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

extern  PWR_AppState    PWRLib_AppState;
extern  uint8_t         mPWRLib_DeepSleepMode ;
extern  uint32_t        PWRLib_wkup_pin_setup;
extern  uint32_t        mPWR_DeepSleepTimeMs;

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
extern uint8_t gPWRLib_RamSavedStatus;
#endif

#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
extern uint32_t pwr_llwu_serial_bitmap;
#endif

/* For LVD function */
#if defined(cPWR_LVD_Enable) && (cPWR_LVD_Enable == 2)
extern PWRLib_LVD_VoltageLevel_t PWRLib_LVD_SavedLevel;
#endif /*  defined(cPWR_LVD_Enable) && (cPWR_LVD_Enable == 2) */

/*****************************************************************************
 *                            PUBLIC FUNCTIONS                               *
 *---------------------------------------------------------------------------*
 * Add to this section all the global functions prototype preceded (as a     *
 * good practice) by the keyword 'extern'                                    *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/*---------------------------------------------------------------------------
 * Name: PWRLib_Init
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_Init(void);

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
void PWRLib_MCU_Enter_Sleep(void);

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
/*---------------------------------------------------------------------------
 * Name: PWRLib_MCU_Enter_vlls2_3
 * Description: Puts the processor into VLLS2 or VLLS3 (Very Low Leakage Stop 0-3).
                And save the processor context

 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5 || cPWR_EnableDeepSleepMode_9)
void PWRLib_MCU_Enter_vlls2_3(void);
#endif

/*---------------------------------------------------------------------------
 * Name: warmmain
 * Description: called on warmboot

 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5 || cPWR_EnableDeepSleepMode_9)
void warmmain(void);
#endif

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_VLLSx
 * Description: Puts the processor into VLLS0 - VLLS3 (Very Low Leakage Stop 0-3).

                Mode of operation details:
                 - ARM core enters SleepDeep Mode
                 - ARM core is clock gated (HCLK = OFF)
                 - NVIC is disable (FCLK = OFF)
                 - LLWU should configure by user to enable the desire wake up source
                 - Platform and peripheral clock are stopped
                 - VLLS0-1 All SRAM powered off
                 - VLLS3 SRAM is partially preserved
                 - VLLS3 All SRAM is preserved
                 - VLLSx mode is exited into RUN mode using LLWU module or RESET.
                All wakeup goes through Reset sequence.

                The AVLLS must be set to 0b1 in MC_PMPROT register in order to allow VLLS mode.

 * Parameters: mode VLLS mode 0-3
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_VLLSx(uint8_t mode);

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_LLSx
 * Description: Puts the processor into LLSx (Low Leakage Stop low power mode).

                Mode of operation details:
                 - ARM core enters SleepDeep Mode
                 - ARM core is clock gated (HCLK = OFF)
                 - NVIC is disable (FCLK = OFF)
                 - LLWU should configure by user to enable the desire wake up source
                 - Platform and peripheral clock are stopped
                 - FULL or partial SRAM retention.

                The ALLS must be set to 0b1 in SMC_PMPROT register in order to allow LLS mode.

 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_LLSx(void);

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
void PWRLib_MCU_Enter_Stop(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_SetDeepSleepMode
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_SetDeepSleepMode(uint8_t lpMode);

/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_GetWakeUpFlags
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint8_t PWRLib_GetDeepSleepMode(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_GetWakeUpFlags
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint32_t PWRLib_LLWU_GetWakeUpFlags(void);


/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_UpdateWakeupReason
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LLWU_UpdateWakeupReason(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_DisableAllWakeupSources
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LLWU_DisableAllWakeupSources(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_GetTimeSettings
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_GetTimeSettings(uint32_t timeInMs, uint8_t* pClkMode,uint32_t* pTicks, uint32_t* pFreq);

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockStart
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_ClockStart(uint8_t ClkMode, uint32_t Ticks);

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockCheck
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint32_t PWRLib_LPTMR_ClockCheck(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockStop
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_ClockStop(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_Init
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_Init(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ReInit
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_ReInit(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_StopUpdateWakeupReason
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if defined(PWR_FUNCTION_NOT_USED)
void PWRLib_StopUpdateWakeupReason(void);
#endif


#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_WaitForDSMExit
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
 void PWRLib_BLL_WaitForDSMExit(void);

 /*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_GetIntEn
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
 uint16_t PWRLib_BLL_GetIntEn(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_DisableInterrupts
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
 void PWRLib_BLL_DisableInterrupts(uint16_t bmInterrupts);

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_EnableInterrupts
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_EnableInterrupts(uint16_t bmInterrupts);

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_ClearInterrupts
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_ClearInterrupts(uint16_t bmInterrupts);

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_SetWakeupInstant
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
 void PWRLib_BLL_SetWakeupInstant(uint16_t wakeupInstant);

 /*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_EnterDSM
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_EnterDSM(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_GetInstantTimer
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint16_t PWRLib_BLL_GetInstantTimer(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_ClearRSIMInt
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_ClearRSIMInt(void);
#endif /* defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1) */
#endif /* defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)*/

/*---------------------------------------------------------------------------
 * Name: PWRLib_LVD_CollectLevel
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
PWRLib_LVD_VoltageLevel_t PWRLib_LVD_CollectLevel(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_ClearRSIMDsmInt
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
bool_t PWRLib_ClearRSIMDsmInt(void);

/*---------------------------------------------------------------------------
 * Name: PWR_HandleDeepSleepMode
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_HandleDeepSleepMode(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_ClearWakeUpSources
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LLWU_ClearWakeUpSources(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_SetWakeupPins
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_SetWakeupPins(uint32_t wkup_pin_set);

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLPEtoBLPI(), PWRLib_BLPItoBPE(), PWRLib_BPEtoBLPE()
 * Description: Set of functions to change from MCG internal to external and viceversa
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLPEtoBLPI(void);
void PWRLib_BLPItoBPE(void);
void PWRLib_BPEtoBLPE(void);

#endif /* __PWR_LIB_H__ */

