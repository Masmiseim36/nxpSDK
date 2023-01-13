/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2021 NXP.
 *
 * License: LA_OPT_NXP_Software_License
 *
 * This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms. If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * @version 4.2
 *
 * @brief Watchdog test common routines - IEC60730 Class B.
 */

#ifndef IEC60730B_WDOG_H_
#define IEC60730B_WDOG_H_

/*
 * List of devices and supported functions can be found in library user guide,
 * section Core self test library - source code version.
 */

/* Assembler skips following section */
#ifndef __ASM__

/******************************************************************************
 * Definitions
 ******************************************************************************/
#define FS_FAIL_WDOG_OVER_RESET    (FS_WDOG_CODE + 0x1U)
#define FS_FAIL_WDOG_VALUE         (FS_WDOG_CODE + 0x2U)
#define FS_FAIL_WDOG_WRONG_RESET   (FS_WDOG_CODE + 0x3U)
#define FS_FAIL_WDOG_WRONG_REFRESH (FS_WDOG_CODE + 0x4U)

#define FS_WDOG_SRS_WIDE_8b  1
#define FS_WDOG_SRS_WIDE_32b 0

#define FS_KINETIS_WDOG 0
#define FS_WDOG32       1
#define FS_COP_WDOG     2
#define FS_IMXRT        3
#define FS_IMX8M        4
#define FS_IMXRT1170    5

/* Watchdog test structure - must be placed in memory section which is not touched
 * by startup code (only POR reset can touch selected memory section). */
typedef struct
{
    uint32_t counter;
    uint32_t resets;
    uint32_t wdTestUncompleteFlag;
    uint32_t RefTimerBase;
    uint32_t WdogBase;
    uint32_t pResetDetectRegister;
    uint32_t ResetDetectMask;
} volatile fs_wdog_test_t;

/******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 *
 * @brief    This function should be called only once after POR reset.
 *           Each calling of this function causes a wd reset.
 *           Function clears the WD resets counter to 0.
 *           No WD settings are changed here. It starts the LPTMR which must
 *           be configured before the function call occurs.
 *           Within the waiting endless loop, value from LPTMR is periodically
 *           stored to reserved area in RAM.
 *
 * @param    *pWatchdogBackup - pointer to structure with fs_wdog_test_t variables.
 * @param    refresh_index - Index for select WDOG refresh sequence : FS_KINETIS_WDOG, FS_WDOG32 or FS_COP_WDOG - see
 *LIbrary UG, chapter "Dedicated function" for your device to chooce correct type.
 *
 * @return   FS_RESULT - FS_FAIL_WDOG_WRONG_REFRESH - if refresh sequence index out of range, otherwise function wait on
 *WDOG refresh
 *
 ******************************************************************************/
FS_RESULT FS_WDOG_Setup_LPTMR(fs_wdog_test_t *pWatchdogBackup, uint8_t refresh_index);

/*******************************************************************************
 *
 * @brief    This function should be called only once after POR reset.
 *           Each calling of this function causes a wd reset.
 *           Function clears the WD resets counter to 0.
 *           No WD settings are changed here. It starts the RTC which must
 *           be configured before the function call occurs.
 *           Within the waiting endless loop, value from RTC is periodically
 *           stored to reserved area in RAM.
 *
 * @param    *pWatchdogBackup - pointer to structure with fs_wdog_test_t variables.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_WDOG_Setup_KE0XZ(fs_wdog_test_t *pWatchdogBackup);

/*******************************************************************************
 *
 * @brief    This function should be called only once after POR reset.
 *           Each calling of this function causes a wd reset.
 *           Function clears the WD resets counter to 0.
 *           No WD settings are changed here. It starts the GPT which must
 *           be configured before the function call occurs.
 *           Within the waiting endless loop, value from GPT is periodically
 *           stored to reserved area in RAM.
 *
 * @param    *pWatchdogBackup - pointer to structure with fs_wdog_test_t variables.
 * @param    refresh_index - Index for select WDOG refresh sequence : FS_IMXRT, FS_IMX8M, FS_RTWDOG  - see LIbrary UG,
 *chapter "Dedicated function" for your device to chooce correct type.
 *
 * @return   FS_RESULT - FS_FAIL_WDOG_WRONG_REFRESH - if refresh sequence index out of range, otherwise function wait on
 *WDOG refresh
 *
 ******************************************************************************/
FS_RESULT FS_WDOG_Setup_IMX_GPT(fs_wdog_test_t *pWatchdogBackup, uint8_t refresh_index);

/*******************************************************************************
 *
 * @brief    This function should be called only once after POR reset.
 *           Each calling of this function causes a wd reset.
 *           Function clears the WD resets counter to 0.
 *           No WD settings are changed here. It starts the CTIMER which must
 *           be configured before the function call occurs.
 *           Within the waiting endless loop, value from CTIMER is periodically
 *           stored to reserved area in RAM.
 *
 * @param    *pWatchdogBackup - pointer to structure with fs_wdog_test_t variables.
 * @param    *pCtimerBase     - pointer to CTIMER base address.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_WDOG_Setup_WWDT_LPC(fs_wdog_test_t *pWatchdogBackup);

/*******************************************************************************
 *
 * @brief    This function should be called only once after POR reset.
 *           Each calling of this function causes a wd reset.
 *           Function clears the WD resets counter to 0.
 *           No WD settings are changed here. It starts the LPTMR which must
 *           be configured before the function call occurs.
 *           Within the waiting endless loop, value from LPTMR is periodically
 *           stored to reserved area in RAM.
 *
 * @param    *pWatchdogBackup - pointer to structure with fs_wdog_test_t variables.
 * @param    *pMRTimerBase    - pointer to used Ctimer.
 * @param    channel          - chanel index of MRT timer.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_WDOG_Setup_WWDT_LPC_mrt(fs_wdog_test_t *pWatchdogBackup, uint8_t channel);

/*******************************************************************************
 *
 * @brief    Function should be called after every WD reset, its calling should
 *           be handled by conditional execution in application.
 *
 *           If the source of reset is WD, function increments the wd_reset counter.
 *           If the reset counter has overflowed, function will wait in endless
 *           loop if it is enabled.
 *
 *           If the source of reset is WD, function check the timeout
 *           value. If it is not in range, function will wait in the endless
 *           loop if enabled.
 *
 *           If the source of reset isn't WD or POR, function will sets the
 *           wdTestUncompleteFlag and will wait in endless loop if enabled.
 *
 * @param    limitHigh         - precalculated limit value for the reference counter.
 * @param    limitLow          - precalculated limit value for the reference counter.
 * @param    limitResets       - limit value for watchdog resets.
 * @param    endlessLoopEnable - enable or disable endless loop within the function.
 * @param    *pWatchdogBackup  - pointer to structure with fs_wdog_test_t variables.
 * @param    clear_flag        - flag for clear WDOG reset flag, "1" cause clear WDOG reset flag (by write 1 to WDOG
 *flag bit)
 * @param    reg_wide          - define wide of SRS register in correspond device, put FS_WDOG_SRS_WIDE_8b for 8Bit and
 *FS_WDOG_SRS_WIDE_32b for 32bit wide
 *
 * @return   uint32_t FS_PASS or FS_FAIL_WDOG_OVER_RESET or FS_FAIL_WDOG_VALUE or FS_FAIL_WDOG_WRONG_RESET
 *
 ******************************************************************************/
uint32_t FS_WDOG_Check(uint32_t limitHigh,
                       uint32_t limitLow,
                       uint32_t limitResets,
                       bool_t endlessLoopEnable,
                       fs_wdog_test_t *pWatchdogBackup,
                       bool_t clear_flag,
                       bool_t RegWide8b);

/*******************************************************************************
 *
 * @brief    Function should be called after every WD reset, its calling should
 *           be handled by conditional execution in application.
 *
 *           If the source of reset is WD, function increments the wd_reset counter.
 *           If the reset counter has overflowed, function will wait in endless
 *           loop if it is enabled.
 *
 *           If the source of reset is WD, function check the timeout
 *           value. If it is not in range, function will wait in the endless
 *           loop if enabled.
 *
 *           If the source of reset isn't WD or POR, function will sets the
 *           wdTestUncompleteFlag and will wait in endless loop if enabled.
 *
 * @param    limitHigh         - precalculated limit value for the reference counter.
 * @param    limitLow          - precalculated limit value for the reference counter.
 * @param    limitResets       - limit value for watchdog resets.
 * @param    endlessLoopEnable - enable or disable endless loop within the function.
 * @param    *pWatchdogBackup  - pointer to structure with fs_wdog_test_t variables.
 *
 * @return   uint32_t FS_PASS or FS_FAIL_WDOG_OVER_RESET or FS_FAIL_WDOG_VALUE or FS_FAIL_WDOG_WRONG_RESET
 *
 ******************************************************************************/
uint32_t FS_WDOG_Check_WWDT_LPC(uint32_t limitHigh,
                                uint32_t limitLow,
                                uint32_t limitResets,
                                bool_t endlessLoopEnable,
                                fs_wdog_test_t *pWatchdogBackup);

/*******************************************************************************
 *
 * @brief    Function should be called after every WD reset, its calling should
 *           be handled by conditional execution in application.
 *
 *           If the source of reset is WD, function increments the wd_reset counter.
 *           If the reset counter has overflowed, function will wait in endless
 *           loop if it is enabled.
 *
 *           If the source of reset is WD, function check the timeout
 *           value. If it is not in range, function will wait in the endless
 *           loop if enabled.
 *
 *           If the source of reset isn't WD or POR, function will sets the
 *           wdTestUncompleteFlag and will wait in endless loop if enabled.
 *
 * @param    limitHigh         - precalculated limit value for the reference counter.
 * @param    limitLow          - precalculated limit value for the reference counter.
 * @param    limitResets       - limit value for watchdog resets.
 * @param    endlessLoopEnable - enable or disable endless loop within the function.
 * @param    *pWatchdogBackup  - pointer to structure with fs_wdog_test_t variables.
 *
 * @return   uint32_t FS_PASS or FS_FAIL_WDOG_OVER_RESET or FS_FAIL_WDOG_VALUE or FS_FAIL_WDOG_WRONG_RESET
 *
 ******************************************************************************/
uint32_t FS_WDOG_Check_WWDT_LPC55SXX(uint32_t limitHigh,
                                     uint32_t limitLow,
                                     uint32_t limitResets,
                                     bool_t endlessLoopEnable,
                                     fs_wdog_test_t *pWatchdogBackup);

#endif /* __ASM__ */
#endif /* IEC60730B_WDOG_H_ */
