/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
* 
*
*******************************************************************************/


/*******************************************************************************
*
* file      IEC60730B_B_CM4_CM7_wdg.h
*
* author    B48632
* 
* version   3.0
*
* date      Nov-01-2018
* 
* brief     IEC60730 Class B Safety Routines Watchdog test for Cortex CM4, CM7 cores
*
*******************************************************************************/

#ifndef _IEC60730_B_WDG_H_
#define _IEC60730_B_WDG_H_


#ifndef __ASM__

/* Structure of WD_test variables */
/* must be placed in memory which is not touched by startup code,  which is only chagned by POR reset */
typedef struct {
  unsigned long counter;
  unsigned long resets;
  unsigned long wd_test_uncomplete_flag;
}volatile WD_Test_Str;


/*******************************************************************************
* Watchdog_test_configuration
*  
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_setup
* param[in]    pWatchdogBackup  pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_setup(WD_Test_Str* pWatchdogBackup);

/*******************************************************************************
* Watchdog_test_check
* 
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wd_test_uncomplete_flag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_check
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_check(unsigned long limit_high, unsigned long limit_low, unsigned long resets_limit, unsigned long endless_loop_enable, WD_Test_Str* pWatchdogBackup);

/*******************************************************************************
* Watchdog_test_configuration for MKE1xF
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_setup_ke
* param[in]    pWatchdogBackup  pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_setup_ke(WD_Test_Str* pWatchdogBackup);

/*******************************************************************************
* Watchdog_test_check for MKE1xF
*
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wd_test_uncomplete_flag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_check_ke
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_check_ke(unsigned long limit_high, unsigned long limit_low, unsigned long resets_limit, unsigned long endless_loop_enable, WD_Test_Str* pWatchdogBackup);

/*******************************************************************************
* Watchdog_test_configuration for MK32W
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_setup_k32w
* param[in]    pWatchdogBackup  pointer to structure with WD_test variables
* param[in]    pWDOG  pointer to WDOG base address
* param[in]    pLPTMR  pointer to LPTMR base address
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_setup_k32w(WD_Test_Str* pWatchdogBackup, unsigned long* pWDOG, unsigned long* pLPTMR);

/*******************************************************************************
* Watchdog_test_check for MK32W
*
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wd_test_uncomplete_flag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_check_k32w
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_check_k32w(unsigned long limit_high, unsigned long limit_low, unsigned long resets_limit, unsigned long endless_loop_enable, WD_Test_Str* pWatchdogBackup);

/*******************************************************************************
* Watchdog_test_configuration for I.MXrt
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_setup_RT
* param[in]    pWatchdogBackup  pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_setup_RT(WD_Test_Str* pWatchdogBackup);

/*******************************************************************************
* Watchdog_test_check for I.MXrt
*
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wd_test_uncomplete_flag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_check_RT
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_check_RT(unsigned long limit_high, unsigned long limit_low, unsigned long resets_limit, unsigned long endless_loop_enable, WD_Test_Str* pWatchdogBackup);


#endif /* __ASM__ */
	
#endif /* _IEC60730_B_CM4_CM7_WDG_H_ */
