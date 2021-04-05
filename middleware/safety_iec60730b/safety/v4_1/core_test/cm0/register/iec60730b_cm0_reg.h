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
 * @version 4.1
 *
 * @brief CPU registers test routines for Cortex-M0+ core - IEC60730 Class B.
 */

#ifndef IEC60730B_CM0_REG_H_
#define IEC60730B_CM0_REG_H_

/*
 * List of devices and supported functions can be found in library user guide, 
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* Return values */
#define FS_FAIL_CPU_REGISTER             (FS_CPU_CODE + 0x01)
#define FS_FAIL_CPU_NONSTACKED_REGISTER  (FS_CPU_CODE + 0x02)
#define FS_FAIL_CPU_PRIMASK              (FS_CPU_CODE + 0x03)
#define FS_FAIL_CPU_CONTROL              (FS_CPU_CODE + 0x04)

/* Assembler skips following section */
#ifndef __ASM__

/******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 *
 * @brief    This function tests the following CPU registers
 *           in a sequence: R0-R7, R12, LR, APSR.
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_REGISTER
 *           Function will not return in case that at least one of 
 *           R0, R1, APSR, LR is corrupted. Then it will stay in endless loop.
 *
 ******************************************************************************/
FS_RESULT FS_CM0_CPU_Register(void);

/*******************************************************************************
 *
 * @brief    This function tests the following CPU register: R8, R9, R10, R11.
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_NONSTACKED_REGISTER
 *
 ******************************************************************************/
FS_RESULT FS_CM0_CPU_NonStackedRegister(void);     

/*******************************************************************************
 *
 * @brief    This function tests the PRIMASK register.
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_PRIMASK
 *
 ******************************************************************************/
FS_RESULT FS_CM0_CPU_Primask(void);

/*******************************************************************************
 *
 * @brief    This function tests the SP_main register.
 *
 * @param    void
 *
 * @return   FS_PASS
 *           Function will not return in case that SP_main is corrupted. 
 *           Then it will stay in endless loop.
 *
 ******************************************************************************/
FS_RESULT FS_CM0_CPU_SPmain(void);

/*******************************************************************************
 *
 * @brief    This function tests the SP_process register.
 *
 * @param    void
 *
 * @return   FS_PASS
 *           Function will not return in case that SP_process is corrupted. 
 *           Then it will stay in endless loop.
 *
 ******************************************************************************/
FS_RESULT FS_CM0_CPU_SPprocess(void);

/*******************************************************************************
 *
 * @brief    This function tests the CONTROL register.
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_CONTROL
 *
 ******************************************************************************/
FS_RESULT FS_CM0_CPU_Control(void);

#endif /* __ASM__ */
#endif /* IEC60730B_CM0_REG_H_ */
