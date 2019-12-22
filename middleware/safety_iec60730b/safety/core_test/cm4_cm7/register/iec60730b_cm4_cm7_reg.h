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
 * @brief     IEC60730 Class B Safety Routines core registers tests for Cortex CM4, CM7 cores
 *
 ***********************************************************************************************/

#ifndef _IEC60730B_CM4_CM7_REG_
#define _IEC60730B_CM4_CM7_REG_

/******************************************************************************
* Macros
******************************************************************************/
#define CPACR_CP10_CP11_MASK   (0xF << 20)

/*******************************************************************************
* Return values
*******************************************************************************/
/* Test passed */
#define FS_CPU_PASS    (0x0000)
/* Test failed */
#define FS_CPU_REGISTER_FAIL             (FS_CPU_FAIL_CODE + 0x01)
#define FS_CPU_NONSTACKED_REGISTER_FAIL  (FS_CPU_FAIL_CODE + 0x02)
#define FS_CPU_PRIMASK_FAIL              (FS_CPU_FAIL_CODE + 0x03)
#define FS_CPU_SP_MAIN_FAIL              (FS_CPU_FAIL_CODE + 0x04)
#define FS_CPU_SP_PROCESS_FAIL           (FS_CPU_FAIL_CODE + 0x05)
#define FS_CPU_CONTROL_FAIL              (FS_CPU_FAIL_CODE + 0x06)
#define FS_CPU_SPECIAL_FAIL              (FS_CPU_FAIL_CODE + 0x07)
#define FS_CPU_FLOAT_1_FAIL              (FS_CPU_FAIL_CODE + 0x08)
#define FS_CPU_FLOAT_2_FAIL              (FS_CPU_FAIL_CODE + 0x09)

/******************************************************************************
* Global functions
******************************************************************************/
#ifndef __ASM__

/********************************************************************************
*
* brief  FS_CM4_CM7_CPU_Register
*
* param  none
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                        FS_CPU_REGISTER_FAIL - Test failed
*                     or function will not return, it will stay in endless loop
*		
* remarks This function tests the  register (R0-R7, R12, LR, APSR) by rules IEC60730 Class B
*
********************************************************************************/ 
FS_RESULT FS_CM4_CM7_CPU_Register(void);

/*******************************************************************************
*
* brief  FS_CM4_CM7_CPU_NonStackedRegister
*
* param  none
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                         FS_CPU_NONSTACKED_REGISTER_FAIL - Test failed
*                     
*		
* remarks This function tests the   registers (R8, R9, R10, R11) by rules IEC60730 Class B
*
*******************************************************************************/
FS_RESULT FS_CM4_CM7_CPU_NonStackedRegister(void);  
     
/*******************************************************************************
*
* brief  FS_CM4_CM7_CPU_Primask
*
* param  none
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                         FS_CPU_PRIMASK_FAIL - Test failed
*                     
*		
* remarks This function tests the  PRIMASK register by rules IEC60730 Class B
*
*******************************************************************************/     
FS_RESULT FS_CM4_CM7_CPU_Primask(void);
     
/*******************************************************************************
*
* brief  FS_CM4_CM7_CPU_SPmain
*
* param  none
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                     or function will not return, it will stay in endless loop
*                     		
* remarks This function tests the Cortex CM4 and CM7 SP_main and SP_process registers by rules IEC60730 Class B
*
*******************************************************************************/      
FS_RESULT FS_CM4_CM7_CPU_SPmain(void); 
     
/*******************************************************************************
*
* brief  FS_CM4_CM7_CPU_SPprocess
*
* param  none
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                    or function will not return, it will stay in endless loop
*                     		
* remarks This function tests  SP_process registers by rules IEC60730 Class B
*
*******************************************************************************/      
FS_RESULT FS_CM4_CM7_CPU_SPprocess(void);
     
/*******************************************************************************
*
* brief  FS_CM4_CM7_CPU_Control
*
* param  none
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                         FS_CPU_CONTROL_FAIL - Test failed
*                     		
* remarks This function tests the Cortex CM4 and CM7 CONTROL register by rules IEC60730 Class B
*
*******************************************************************************/     
FS_RESULT FS_CM4_CM7_CPU_Control(void);

/*******************************************************************************
*
* brief  FS_CM4_CM7_CPU_ControlFpu
*
* param  none
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                         FS_CPU_CONTROL_FAIL - Test failed
*                     		
* remarks This function tests the Cortex CM4 CONTROL register by rules IEC60730 Class B
*         Only for devices with Floating Point Unit
*******************************************************************************/     
FS_RESULT FS_CM4_CM7_CPU_ControlFpu(void);
     
/********************************************************************************
*
* brief  FS_CM4_CM7_CPU_Special
*
* param  none
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                        FS_CPU_SPECIAL_FAIL - Test failed
*		
* remarks This function tests others of the M4 and M7 CPU register, which are not included in M0+
*         by rules IEC60730 Class B
*         
************************************************************************************/        
FS_RESULT FS_CM4_CM7_CPU_Special(void);
     
/********************************************************************************
*
* brief  FS_CM4_CM7_CPU_Special8PriorityLevels
*
* param  none
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                        FS_CPU_SPECIAL_FAIL - Test failed
*		
* remarks This function tests BASEPRI and FAULTMASK registers 
* for devices with 8 priority levels for interrupts
* by rules IEC60730 Class B
*
************************************************************************************/  
FS_RESULT FS_CM4_CM7_CPU_Special8PriorityLevels(void);     
       
/********************************************************************************
*
* brief  FS_CM4_CM7_CPU_Float1
*
* param  none
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                        FS_CPU_FLOAT_1_FAIL - Test failed
*		
* remarks This function tests the M4 and M7 FPU register (FPSCR,S0-S15) by rules IEC60730 Class B
*         Only for devices with Floating Point Unit
************************************************************************************/      
FS_RESULT FS_CM4_CM7_CPU_Float1(void);
     
/********************************************************************************
*
* brief  FS_CM4_CM7_CPU_Float2
*
* param  none
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                        FS_CPU_FLOAT_2_FAIL - Test failed
*                     		
* remarks This function tests the M4 and M7 FPU register (S16-S31) by rules IEC60730 Class B
*         Only for devices with Floating Point Unit
************************************************************************************/     
FS_RESULT FS_CM4_CM7_CPU_Float2(void);

#endif /* __ASM__ */

#endif /* _FS_CM4_CM7_REG_ */
