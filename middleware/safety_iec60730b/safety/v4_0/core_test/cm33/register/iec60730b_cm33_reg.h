/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2020 NXP.
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
 * @version 4.0
 *
 * @brief CPU registers test routines for Cortex-M33 core - IEC60730 Class B.
 */

#ifndef _IEC60730B_CM33_REG_H_
#define _IEC60730B_CM33_REG_H_

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
#define FS_FAIL_CPU_SPECIAL              (FS_CPU_CODE + 0x05)
#define FS_FAIL_CPU_FLOAT_1              (FS_CPU_CODE + 0x06)
#define FS_FAIL_CPU_FLOAT_2              (FS_CPU_CODE + 0x07)

#define CPACR_CP10_CP11_MASK   (0xF << 20)

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
FS_RESULT FS_CM33_CPU_Register(void);

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
FS_RESULT FS_CM33_CPU_NonStackedRegister(void);  
     
/*******************************************************************************
 *
 * @brief    This function tests the PRIMASK_S (secure) register.
 *           Must be executed in SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_PRIMASK
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_Primask_S(void);
     
/*******************************************************************************
 *
 * @brief    This function tests the PRIMASK_NS (non-secure) register.
 *           Must be executed in NON-SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_PRIMASK
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_Primask_NS(void);

/*******************************************************************************
 *
 * @brief    This function tests the MSP (secure) register.
 *           Must be executed in SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           Function will not return in case that MSP is corrupted. 
 *           Then it will stay in endless loop.
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_SPmain_S(void); 

/*******************************************************************************
 *
 * @brief    This function tests the MSP_NS (non-secure) register.
 *           Must be executed in NON-SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           Function will not return in case that MSP_NS is corrupted. 
 *           Then it will stay in endless loop.
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_SPmain_NS(void);

/*******************************************************************************
 *
 * @brief    This function tests the MSPLIM (secure) register.
 *           Must be executed in SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           Function will not return in case that MSPLIM is corrupted. 
 *           Then it will stay in endless loop.
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_SPmain_Limit_S(void);

/*******************************************************************************
 *
 * @brief    This function tests the MSPLIM_NS (non-secure) register.
 *           Must be executed in NON-SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           Function will not return in case that MSPLIM_NS is corrupted. 
 *           Then it will stay in endless loop.
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_SPmain_Limit_NS(void);

/*******************************************************************************
 *
 * @brief    This function tests the PSP (secure) register.
 *           Must be executed in SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           Function will not return in case that PSP is corrupted. 
 *           Then it will stay in endless loop.
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_SPprocess_S(void);

/*******************************************************************************
 *
 * @brief    This function tests the PSP_NS (non-secure) register.
 *           Must be executed in NON-SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           Function will not return in case that PSP_NS is corrupted. 
 *           Then it will stay in endless loop.
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_SPprocess_NS(void);

/*******************************************************************************
 *
 * @brief    This function tests the PSPLIM (secure) register.
 *           Must be executed in SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           Function will not return in case that PSPLIM is corrupted. 
 *           Then it will stay in endless loop.
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_SPprocess_Limit_S(void);

/*******************************************************************************
 *
 * @brief    This function tests the PSPLIM_NS (non-secure) register.
 *           Must be executed in NON-SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           Function will not return in case that PSPLIM_NS is corrupted. 
 *           Then it will stay in endless loop.
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_SPprocess_Limit_NS(void);

/*******************************************************************************
 *
 * @brief    This function tests the CONTROL register.
 *           This test must be executed only if TrustZone is not enabled.
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_CONTROL
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_Control(void);

/*******************************************************************************
 *
 * @brief    This function tests the CONTROL (secure) register.
 *           Must be executed in SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_CONTROL
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_Control_S(void);

/*******************************************************************************
 *
 * @brief    This function tests the CONTROL_NS (non-secure) register.
 *           Must be executed in NON-SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_CONTROL
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_Control_NS(void);

/*******************************************************************************
 *
 * @brief    This function tests the BASEPRI and FAULTMASK (secure) registers.
 *           For devices with 8 priority levels for interrupts.
 *           Must be executed in SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_SPECIAL
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_Special8PriorityLevels_S(void);

/*******************************************************************************
 *
 * @brief    This function tests the BASEPRI_NS and FAULTMASK_NS
 *           (non-secure) registers.
 *           For devices with 8 priority levels for interrupts.
 *           Must be executed in NON-SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_SPECIAL
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_Special8PriorityLevels_NS(void);

/*******************************************************************************
 *
 * @brief    This function tests the FPU register (FPSCR, S0-S15).
 *           Only for devices with Floating Point Unit.
 *           Must be executed in SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_FLOAT_1
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_Float1(void);

/*******************************************************************************
 *
 * @brief    This function tests the FPU register (S16-S31).
 *           Only for devices with Floating Point Unit.
 *           Must be executed in SECURE mode!
 *
 * @param    void
 *
 * @return   FS_PASS
 *           FS_FAIL_CPU_FLOAT_1
 *
 ******************************************************************************/
FS_RESULT FS_CM33_CPU_Float2(void);

#endif /* __ASM__ */
#endif /* _IEC60730B_CM33_REG_H_ */
