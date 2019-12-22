/************************************************************************************************
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2019 NXP.
 *
 * License: LA_OPT_NXP_Software_License
 *
 * This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * 
 * @version   4.0
 * 
 * @brief     IEC60730 Class B Safety Routines Watchdog test
 *
 ***********************************************************************************************/

#ifndef _IEC60730B_WDOG_
#define _IEC60730B_WDOG_

#ifndef __ASM__

/* Watchdog test structure - must be placed in memory section which is not touched
 * by startup code (only POR reset can touch selected memory section). */
typedef struct
{
  uint32_t counter;
  uint32_t resets;
  uint32_t wdTestUncompleteFlag;
} volatile fs_wdog_test;

/*******************************************************************************
* Watchdog test configuration
*  
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Setup
* param[in]    pWatchdogBackup pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Setup(fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* Watchdog test check
* 
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wdTestUncompleteFlag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Check
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Check(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* Watchdog test configuration for KE0xZ
*  
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from RTC is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Setup_KE0XZ - for Watchdog implemented in KE devices
* param[in]    pWatchdogBackup  pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Setup_KE0XZ(fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* Watchdog test check for KE0xZ
* 
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wdTestUncompleteFlag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Check_KE0XZ - for Watchdog implemented in KE devices
* param[in]    limit_high          precalculated limit value for RTC counter
* param[in]    limit_low           precalculated limit value for RTC counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Check_KE0XZ(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* COP test configuration
*  
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Setup_COP
* param[in]    pWatchdogBackup  pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Setup_COP(fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* Watchdog test configuration for KE1xZ
*
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Setup_KE1XZ
* param[in]    pWatchdogBackup  pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Setup_KE1XZ(fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* Watchdog test check for KE1xZ
*
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wdTestUncompleteFlag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Check_KE1XZ
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Check_KE1XZ(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* Watchdog test configuration for KE1xF
*
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Setup_KE1XF
* param[in]    pWatchdogBackup  pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Setup_KE1XF(fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* Watchdog test check for KE1xF
*
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wdTestUncompleteFlag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Check_KE1XF
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Check_KE1XF(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* Watchdog test configuration for K32W device (core CM0)
*
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.
* It starts the LPTMR which must be configured before the function call occurs.
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
*
*******************************************************************************/
/*********************************************************************************************************
* brief        FS_CM0_WDOG_Setup_K32W
* param[in]    pWatchdogBackup  pointer to structure with fs_wdog_test variables
* param[in]    pWDOG  - pointer to WDOG base address
* param[in]    pLPTMR - pointer to LPTMR base address
* return       void
**********************************************************************************************************/
void FS_CM0_WDOG_Setup_K32W(fs_wdog_test *pWatchdogBackup, uint32_t *pWDOG, uint32_t *pLPTMR);

/*******************************************************************************
* Watchdog test check for K32W device (core CM0)
*
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wdTestUncompleteFlag and will wait in endless loop if it is enabled
*******************************************************************************/
/*********************************************************************************************************
* brief        FS_CM0_WDOG_Check_K32W
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/
void FS_CM0_WDOG_Check_K32W(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* Watchdog test configuration for K32W device (core CM4)
*
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_CM4_WDOG_Setup_K32W
* param[in]    pWatchdogBackup  pointer to structure with fs_wdog_test variables
* param[in]    pWDOG  pointer to WDOG base address
* param[in]    pLPTMR  pointer to LPTMR base address
* return       void
**********************************************************************************************************/ 
void FS_CM4_WDOG_Setup_K32W(fs_wdog_test *pWatchdogBackup, uint32_t *pWDOG, uint32_t *pLPTMR);

/*******************************************************************************
* Watchdog test check for K32W device (core CM4)
*
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wdTestUncompleteFlag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_CM4_WDOG_Check_K32W
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_CM4_WDOG_Check_K32W(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* Watchdog test configuration for IMXRT
*
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Setup_RT
* param[in]    pWatchdogBackup  pointer to structure with fs_wdog_test variables
* param[in]    pGPT             pointer to GPT base address
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Setup_RT(fs_wdog_test *pWatchdogBackup, uint32_t *pGPT);

/*******************************************************************************
* Watchdog test check for IMXRT
*
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wdTestUncompleteFlag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Check_RT
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Check_RT(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup);

/*******************************************************************************
* Watchdog test configuration for LPC
*
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Setup_WWDT_LPC
* param[in]    pWatchdogBackup  pointer to structure with fs_wdog_test variables
* param[in]    pCtimer_base - pointer to used Ctimer
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Setup_WWDT_LPC(fs_wdog_test *pWatchdogBackup, uint32_t *pCtimer_base);

/********************************************************************************************************* 
* brief        FS_WDOG_Setup_WWDT_LPC_mrt
* param[in]    pWatchdogBackup  pointer to structure with fs_wdog_test variables
* param[in]    pMRTimer_base - pointer to used Ctimer
* param[in]    channel - chanel index of MRT timer
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Setup_WWDT_LPC_mrt(fs_wdog_test *pWatchdogBackup, uint32_t *pMRTimer_base, uint8_t channel);

/*******************************************************************************
* Watchdog test check for LPC
*
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wdTestUncompleteFlag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        FS_WDOG_Check_WWDT_LPC
* param[in]    limitHigh         precalculated limit value for LPTMR counter
* param[in]    limitLow          precalculated limit value for LPTMR counter
* param[in]    limitResets       limit value for WD resets
* param[in]    endlessLoopEnable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup   pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Check_WWDT_LPC(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup);

/********************************************************************************************************* 
* brief        FS_WDOG_Check_WWDT_LPC55SXX
* param[in]    limitHigh         precalculated limit value for LPTMR counter
* param[in]    limitLow          precalculated limit value for LPTMR counter
* param[in]    limitResets       limit value for WD resets
* param[in]    endlessLoopEnable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup   pointer to structure with fs_wdog_test variables
* return       void
**********************************************************************************************************/ 
void FS_WDOG_Check_WWDT_LPC55SXX(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup);

#endif /* __ASM__ */
	
#endif /* _IEC60730B_WDOG_ */
