/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_ERROR_HANDLER_H_
#define _SAFETY_ERROR_HANDLER_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Safety test error flags. */
#define FS_ERROR_FLASH_TEST        (1UL <<  0U) /* Flash test error flag. */
#define FS_ERROR_RAM_SAFETY_TEST   (1UL <<  1U) /* Safety RAM test error flag. */
#define FS_ERROR_RAM_STACK_TEST    (1UL <<  2U) /* Safety RAM test error flag. */
#define FS_ERROR_STACK_TEST        (1UL <<  3U) /* Stack test error flag. */
#define FS_ERROR_CLOCK_TEST        (1UL <<  4U) /* Clock test error flag. */
#define FS_ERROR_PC_TEST           (1UL <<  5U) /* Program counter test error flag. */
#define FS_ERROR_CPU_PRIMASK       (1UL <<  6U) /* PRIMASK test error flag.*/
#define FS_ERROR_CPU_MSP           (1UL <<  7U) /* Main SP test error flag. */
#define FS_ERROR_CPU_STACKED       (1UL <<  8U) /* Stacked CPU registers error flag. */
#define FS_ERROR_CPU_NONSTACKED    (1UL <<  9U) /* Non-stacked CPU registers error flag. */
#define FS_ERROR_CPU_CONTROL       (1UL << 10U) /* CONTROL register test error flag. */
#define FS_ERROR_CPU_PSP           (1UL << 11U) /* Process SP test error flag. */
#define FS_ERROR_CPU_FLOAT1        (1UL << 12U) /* Process FP1 test error flag. */
#define FS_ERROR_CPU_FLOAT2        (1UL << 13U) /* Process FP1 test error flag. */
#define FS_ERROR_UNEXPECTED_IRQ    (1UL << 14U) /* Unexpected IRQ is executed. */
#define FS_ERROR_MCDRV_ADC_FAIL    (1UL << 15U) /* Unrecoverable ADC measurement failure. */
#define FS_ERROR_MCSM_FAIL         (1UL << 16U) /* Unrecoverable MC.SM failure. */
#define FS_ERROR_ISR_TEST          (1UL << 17U) /* The interrupt handling test. */
#define FS_ERROR_SW_FLOW_AR_TEST   (1UL << 18U) /* The after-reset software flow test. */
#define FS_ERROR_SW_FLOW_BL_TEST   (1UL << 19U) /* The background loop software flow test. */
#define FS_ERROR_SW_FLOW_FL_TEST   (1UL << 20U) /* The fast control loop software flow test. */
#define FS_ERROR_SW_FLOW_SL_TEST   (1UL << 21U) /* The slow control loop software flow test. */

/* FS_ERROR_MCDRV_ADC_FAIL: The ADC periphery safety error codes. */
#define SAFETY_ERROR_ADC_CALIB_FAIL            (1UL << 0U) /* ADC periphery calibration failed. */
#define SAFETY_ERROR_ADC_OFFST_CAL_MEAS_FAIL   (1UL << 1U) /* Current offset measurement failed. */
#define SAFETY_ERROR_ADC_OFFST_CAL_PROC_FAIL   (1UL << 2U) /* Current offset processing failed. */

/* FS_ERROR_MCSM_FAIL: The MC.SM safety error codes. */
#define SAFETY_ERROR_NO_INIT                   (1UL << 0U) /* MC.SM BL, FL, or SL routines called without initialization. */
#define SAFETY_ERROR_BAD_RET_VAL               (1UL << 1U) /* Bad return value from FOC found. */

/* FS_ERROR_ISR_TEST: The interrupt handling test error codes. */
#define SAFETY_ERROR_TOO_MANY_FL_EXECS         (1UL << 0U) /* FL executed too many times compared to SL. */
#define SAFETY_ERROR_BAD_FL_TO_SL_RATE         (1UL << 1U) /* FL was not executed correct number of times per SL. */
#define SAFETY_ERROR_BAD_SL_TO_BL_RATE         (1UL << 2U) /* SL executed too many times compared to BL. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @name Safety IEC60730 Class B tests handling functions
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief   Safety error handler.
 *
 * @details This function stores the code of recognized safety error into the
 *          dedicated RAM memory that is deleted only after POR. If the macro
 *          FS_CFG_SAFETY_ERROR_ACTION is defined, all the interrupts are
 *          disabled and function waits in the endless loop. If the DEBUG
 *          macro is defined, then the watchdog is regularly fed and the LED is
 *          blinked to signalize the safety error code FS_ERROR_n, otherwise the
 *          wdog reset is awaited.
 *
 * @param   ui32ErrCode - The safety violation error code.
 * @param   ui32TestResult - Return code of the executed test. Zero value is
 *                           used if no return code is available.
 *
 * @return  none
 */
void FS_fsErrorHandling(uint32_t ui32ErrCode, uint32_t ui32TestResult);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SAFETY_ERROR_HANDLER_H_ */
