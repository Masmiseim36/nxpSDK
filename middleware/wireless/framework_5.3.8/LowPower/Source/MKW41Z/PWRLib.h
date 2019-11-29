/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

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
   
/*****************************************************************************
 *                             PUBLIC MACROS                                *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#define REGS_BLE_BASE           0x4005B000
#define COMMAND_REGISTER        (0x00)  /* 32-bit:0x00 */
#define EVENT_CLEAR             (0x04)  /* 32-bit:0x08 */
#define EVENT_STATUS            (0x04)  /* 32-bit:0x08 */
#define EVENT_ENABLE            (0x08)  /* 32-bit:0x10 */
#define WAKEUP_CONFIG           (0x5C)  /* 32-bit:0xB8 */
#define WAKEUP_CONTROL          (0x60)  /* 32-bit:0xC0 */
#define CLOCK_CONFIG            (0x62)  /* 32-bit:0xC4 */
#define TIM_COUNTER_L           (0x64)  /* 32-bit:0xC8 */
#define ENTER_DSM               (0x50)

#define BLE_LL_REG(offset)  (*((volatile uint16_t*)(REGS_BLE_BASE + offset))) 
#define BLE_LL_EVENT_CLEAR_DSM_INT_MASK                ((uint16_t)(1<<5))
#define BLE_LL_EVENT_CLEAR_SM_INT_MASK                 ((uint16_t)(1<<4))   
#define BLE_LL_EVENT_STATUS_DSM_INT_MASK               ((uint16_t)(1<<5))
#define BLE_LL_EVENT_STATUS_SM_INT_MASK                ((uint16_t)(1<<4))   
#define BLE_LL_EVENT_ENABLE_DSM_INT_MASK               ((uint16_t)(1<<5))
#define BLE_LL_EVENT_ENABLE_SM_INT_MASK                ((uint16_t)(1<<4))   
#define BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_SHIFT          (10)
#define BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_MASK           (0xFC00)   
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SHIFT           (0)   
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_MASK            (0x00FF)   
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SLOT_SHIFT      (5)   
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SLOT_MASK       (0x00E0)   
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_CLK_SHIFT       (0)   
#define BLE_LL_WAKEUP_CONFIG_OSC_DELAY_CLK_MASK        (0x001F)   
#define BLE_LL_WAKEUP_CONFIG_CON_RAM_DSM_RET           (0x0100)   
   
#define BLE_LL_CLOCK_CONFIG_DSM_EN_MASK                ((uint16_t)(1<<15))
#define BLE_LL_CLOCK_CONFIG_SM_EN_MASK                 ((uint16_t)(1<<14))   
#define BLE_LL_CLOCK_CONFIG_DSM_INT_EN_MASK            ((uint16_t)(1<<13))      
#define BLE_LL_CLOCK_CONFIG_SM_INT_EN_MASK             ((uint16_t)(1<<12))         
#define BLE_LL_CLOCK_CONFIG_DSM_AUTO_SLEEP_MASK        ((uint16_t)(1<<11))   
#define BLE_LL_CLOCK_CONFIG_SM_AUTO_WKUP_MASK          ((uint16_t)(1<<10))   
#define BLE_LL_CLOCK_CONFIG_LPO_EXT_MASK               ((uint16_t)(1<<9))   
#define BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK          ((uint16_t)(1<<8))      
#define BLE_LL_CLOCK_CONFIG_LLH_IDLE_MASK              ((uint16_t)(1<<7))   
#define BLE_LL_CLOCK_CONFIG_PHY_CLK_GATE_EN_MASK       ((uint16_t)(1<<6))      
#define BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK       ((uint16_t)(1<<5))         
#define BLE_LL_CLOCK_CONFIG_CORE_CLK_GATE_EN_MASK      ((uint16_t)(1<<4))   
#define BLE_LL_CLOCK_CONFIG_CONN_CLK_GATE_EN_MASK      ((uint16_t)(1<<3))      
#define BLE_LL_CLOCK_CONFIG_INIT_CLK_GATE_EN_MASK      ((uint16_t)(1<<2))   
#define BLE_LL_CLOCK_CONFIG_SCAN_CLK_GATE_EN_MASK      ((uint16_t)(1<<1))      
#define BLE_LL_CLOCK_CONFIG_ADV_CLK_GATE_EN_MASK       ((uint16_t)(1<<0))      

/*****************************************************************************
 *                             PRIVATE MACROS                                *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#define RSIM_BASE_ADDRESS             (0x40059000)
/* Definitions for LPTMR timer setup */

/* LPTMR prescaler clocking options */
#define cLPTMR_Source_Int_MCGIRCLK    0
#define cLPTMR_Source_Int_LPO_1KHz    1
#define cLPTMR_Source_Ext_ERCLK32K    2
#define cLPTMR_Source_Ext_OSCERCLK    3

#define RSIM_CONTROL_READ_FIELD(fieldName)  (((RSIM->CONTROL) & (fieldName##_MASK)) >> (fieldName##_SHIFT))  
#define READ_REGISTER_FIELD(reg,fieldName)  (((reg) & (fieldName##_MASK)) >> (fieldName##_SHIFT))     
#define RSIM_CONTROL_MODIFY_FIELD(fieldName, value)  (((RSIM->CONTROL) & (~(fieldName##_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_MASK))) | fieldName(value))
     
/*****************************************************************************
 *                        PRIVATE TYPE DEFINITIONS                           *
 *---------------------------------------------------------------------------*
 * Add to this section all the data types definitions: stuctures, unions,    *
 * enumerations, typedefs ...                                                *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
/* Constants defined for use with GetLLSWakeUpFlags method */
#define LLWU_EXT_PIN0                  0x1U  /* WakeUp caused by - External pin 0    */            
#define LLWU_EXT_PIN1                  0x2U  /* WakeUp caused by - External pin 1    */         
#define LLWU_EXT_PIN2                  0x4U  /* WakeUp caused by - External pin 2    */      
#define LLWU_EXT_PIN3                  0x8U  /* WakeUp caused by - External pin 3    */      
#define LLWU_EXT_PIN4                  0x10U  /* WakeUp caused by - External pin 4    */   
#define LLWU_EXT_PIN5                  0x20U  /* WakeUp caused by - External pin 5    */
#define LLWU_EXT_PIN6                  0x40U  /* WakeUp caused by - External pin 6    */
#define LLWU_EXT_PIN7                  0x80U  /* WakeUp caused by - External pin 7    */
#define LLWU_EXT_PIN8                  0x0100U  /* WakeUp caused by - External pin 8  */
#define LLWU_EXT_PIN9                  0x0200U  /* WakeUp caused by - External pin 9  */
#define LLWU_EXT_PIN10                 0x0400U  /* WakeUp caused by - External pin 10 */
#define LLWU_EXT_PIN14                 0x4000U  /* WakeUp caused by - External pin 14 */
#define LLWU_EXT_PIN15                 0x8000U  /* WakeUp caused by - External pin 15 */
#define LLWU_INT_MODULE0               0x00010000U  /* WakeUp caused by - Internal module 0 (LPTMR0) */
#define LLWU_INT_MODULE1               0x00020000U  /* WakeUp caused by - Internal module 1 (CMP0) */
#define LLWU_INT_MODULE2               0x00040000U  /* WakeUp caused by - Internal module 2 (BTLL) */
#define LLWU_INT_MODULE3               0x00080000U  /* WakeUp caused by - Internal module 3 (DCDC) */
#define LLWU_INT_MODULE4               0x00100000U  /* WakeUp caused by - Internal module 4 (TSI0) */
#define LLWU_INT_MODULE5               0x00200000U  /* WakeUp caused by - Internal module 5 (RTC) */
#define LLWU_INT_MODULE7               0x00800000U  /* WakeUp caused by - Internal module 7 (RTC_Seconds) */
#define LLWU_FILTER1                   0x01000000U  /* WakeUp caused by digital filter 1 */
#define LLWU_FILTER2                   0x02000000U  /* WakeUp caused by digital filter 2 */

#define  gPWRLib_LLWU_WakeupPin_PTC16_c    LLWU_EXT_PIN0   
#define  gPWRLib_LLWU_WakeupPin_PTC17_c    LLWU_EXT_PIN1   
#define  gPWRLib_LLWU_WakeupPin_PTC18_c    LLWU_EXT_PIN2   
#define  gPWRLib_LLWU_WakeupPin_PTC19_c    LLWU_EXT_PIN3
#define  gPWRLib_LLWU_WakeupPin_PTA16_c    LLWU_EXT_PIN4
#define  gPWRLib_LLWU_WakeupPin_PTA17_c    LLWU_EXT_PIN5
#define  gPWRLib_LLWU_WakeupPin_PTA18_c    LLWU_EXT_PIN6   
#define  gPWRLib_LLWU_WakeupPin_PTA19_c    LLWU_EXT_PIN7
#define  gPWRLib_LLWU_WakeupPin_PTB0_c     LLWU_EXT_PIN8
#define  gPWRLib_LLWU_WakeupPin_PTC0_c     LLWU_EXT_PIN9
#define  gPWRLib_LLWU_WakeupPin_PTC2_c     LLWU_EXT_PIN10
#define  gPWRLib_LLWU_WakeupPin_PTC3_c     LLWU_EXT_PIN11
#define  gPWRLib_LLWU_WakeupPin_PTC4_c     LLWU_EXT_PIN12
#define  gPWRLib_LLWU_WakeupPin_PTC5_c     LLWU_EXT_PIN13
#define  gPWRLib_LLWU_WakeupPin_PTC6_c     LLWU_EXT_PIN14
#define  gPWRLib_LLWU_WakeupPin_PTC7_c     LLWU_EXT_PIN15 

//#define  gPWRLib_LLWU_KeyboardFlagMask_c (gPWRLib_LLWU_WakeupPin_PTA18_c | gPWRLib_LLWU_WakeupPin_PTA19_c )

#define  gPWRLib_LLWU_WakeupModuleFlag_LPTMR_c      LLWU_INT_MODULE0
#define  gPWRLib_LLWU_WakeupModuleFlag_CMP0_c       LLWU_INT_MODULE1
#define  gPWRLib_LLWU_WakeupModuleFlag_Radio_c      LLWU_INT_MODULE2
#define  gPWRLib_LLWU_WakeupModuleFlag_DCDC_c       LLWU_INT_MODULE3
#define  gPWRLib_LLWU_WakeupModuleFlag_TSI_c        LLWU_INT_MODULE5

#define  gPWRLib_LLWU_WakeupModuleEnable_LPTMR_c      (1)
#define  gPWRLib_LLWU_WakeupModuleEnable_CMP0_c       (1<<1)
#define  gPWRLib_LLWU_WakeupModuleEnable_Radio_c      (1<<2)
#define  gPWRLib_LLWU_WakeupModuleEnable_DCDC_c       (1<<3)
#define  gPWRLib_LLWU_WakeupModuleEnable_RTC_Alarm_c  (1<<4)   

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
/* Zigbee stack status */
extern PWRLib_StackPS_t PWRLib_StackPS;

/* For LVD function */
#if (cPWR_LVD_Enable == 2)
extern PWRLib_LVD_VoltageLevel_t PWRLib_LVD_SavedLevel;
#endif /* (cPWR_LVD_Enable == 2) */

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
void PWRLib_MCU_Enter_VLLS0(void);

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
void PWRLib_MCU_Enter_VLLS1(void);

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
void PWRLib_MCU_Enter_VLLS2(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCU_Enter_LLS2
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
void PWRLib_MCU_Enter_LLS2(void);

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
void PWRLib_MCU_Enter_LLS3(void);

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
 * Name: PWRLib_BLL_SetWakeupInstant
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
 void PWRLib_BLL_SetWakeupInstant(uint16_t wakeupInstant);
 
/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_GetOffsetSlotsBeforeDsmExit
 * Description: Returns dsm_offset_to_wakeup_instant + osc_startup_delay
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint16_t PWRLib_BLL_GetOffsetSlotsBeforeDsmExit(void);

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
 * Name: PWRLib_SetLowPowerMode
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_SetDeepSleepMode(uint8_t lpMode);

/*---------------------------------------------------------------------------
 * Name: PWRLib_GetDeepSleepMode
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint8_t PWRLib_GetDeepSleepMode(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_ConfigDSM
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_ConfigDSM(uint8_t lpMode);

/*---------------------------------------------------------------------------
 * Name: PWRLib_ConfigLLWU
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_ConfigLLWU(uint8_t lpMode);

/*---------------------------------------------------------------------------
 * Name: PWRLib_ConfigRSIM
 * Description: 
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_ConfigRSIM(uint8_t lpMode);

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
 * Name: PWRLib_LPTMR_DeInit
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_DeInit(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_StopUpdateWakeupReason
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWRLib_StopUpdateWakeupReason(void);

/*---------------------------------------------------------------------------
 * Name: PWRLib_LVD_CollectLevel
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
PWRLib_LVD_VoltageLevel_t PWRLib_LVD_CollectLevel(void);

#endif /* __PWR_LIB_H__ */

