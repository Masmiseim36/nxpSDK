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
 * @brief Program counter test routines for Cortex-M4/M7 core - IEC60730 Class B.
 */

#ifndef IEC60730B_CM4_CM7_PC_H_
#define IEC60730B_CM4_CM7_PC_H_

/*
 * List of devices and supported functions can be found in library user guide,
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* Return values */
#define FS_FAIL_PC (FS_PC_CODE + 0x01)

/* Assembler skips following section */
#ifndef __ASM__

/******************************************************************************
 * Definitions
 ******************************************************************************/
/* For using the address of function FS_PC_Object as a parameter of FS_CM7_PC_Test */
typedef void (*tFcn_pc)(void);

/******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 *
 * @brief    This function executes the PC test. It is universal and can be
 *           implemented with various memory addresses.
 *           Flag is set to 1 before some pattern is written into PC.
 *           After successful execution, flag is cleared. It is usefull for
 *           validating the test, or when finding the error after reset.
 *
 * @param    pattern1        - address from RAM memory, adequate as a pattern for PC.
 * @param    pObjectFunction - address of FS_PC_Object() function.
 * @param    *pFlag          - address of variable/place in memory used as a flag.
 *                             If flag is 0 test was successful, if 1 test failed.
 *
 * @return   FS_PASS
 *           FS_FAIL_PC - in case of incorrect test execution, PC_flag has a value of 1.
 *
 ******************************************************************************/
FS_RESULT FS_CM4_CM7_PC_Test(uint32_t pattern1, tFcn_pc pObjectFunction, uint32_t *flag);

/*******************************************************************************
 *
 * @brief    This function is internally used by the FS_CM4_CM7_PC_Test() function.
 *           Function is used for performing PC test, it should be called only
 *           by FS_CM4_CM7_PC_Test() function.
 *           It should be placed in reliable address - by editing the linker file.
 *
 * @param    void
 *
 * @return   void
 *
 ******************************************************************************/
void FS_PC_Object(void);

#endif /* __ASM__ */
#endif /* IEC60730B_CM4_CM7_PC_H_ */
