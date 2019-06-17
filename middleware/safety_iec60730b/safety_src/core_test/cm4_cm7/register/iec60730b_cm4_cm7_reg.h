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



/***********************************************************************************************
*
* file      IEC60730B_B_CM4_CM7_reg.h
*
* author    B48632
* 
* version   3.0
*
* date      Nov-01-2018
* 
* brief     IEC60730 Class B Safety Routines core registers tests for Cortex CM4, CM7 cores
*
************************************************************************************************

***********************************************************************************************/
#ifndef _IEC60730_B_CM4_CM7_REG_H_
#define _IEC60730_B_CM4_CM7_REG_H_


/******************************************************************************
* Macros
******************************************************************************/
#define CPACR_CP10_CP11_MASK   (0xF << 20)

/*******************************************************************************
* Return values
*******************************************************************************/
/* Test passed */
#define IEC60730B_ST_CPU_PASS    (0x0000)
/* Test failed */
#define IEC60730B_ST_CPU_REGISTER_FAIL             (IEC60730B_ST_CPU_FAIL_CODE + 0x01)  // | -> +
#define IEC60730B_ST_CPU_NONSTACKED_REGISTER_FAIL  (IEC60730B_ST_CPU_FAIL_CODE + 0x02)  // | -> +
#define IEC60730B_ST_CPU_PRIMASK_FAIL              (IEC60730B_ST_CPU_FAIL_CODE + 0x03)  // | -> +
#define IEC60730B_ST_CPU_SP_MAIN_FAIL              (IEC60730B_ST_CPU_FAIL_CODE + 0x04)  // | -> +
#define IEC60730B_ST_CPU_SP_PROCESS_FAIL           (IEC60730B_ST_CPU_FAIL_CODE + 0x05)  // | -> +
#define IEC60730B_ST_CPU_CONTROL_FAIL              (IEC60730B_ST_CPU_FAIL_CODE + 0x06)  // | -> +
#define IEC60730B_ST_CPU_SPECIAL_FAIL              (IEC60730B_ST_CPU_FAIL_CODE + 0x07)  // | -> +
#define IEC60730B_ST_CPU_FLOAT_1_FAIL              (IEC60730B_ST_CPU_FAIL_CODE + 0x08)  // | -> +
#define IEC60730B_ST_CPU_FLOAT_2_FAIL              (IEC60730B_ST_CPU_FAIL_CODE + 0x09)  // | -> +


/******************************************************************************
* Global functions
******************************************************************************/
#ifndef __ASM__

/********************************************************************************
*
* brief  IEC60730B_CM4_CM7_CPU_RegisterTest
*
* param  none
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                        IEC60730B_ST_CPU_REGISTER_FAIL - Test failed
*                     or function will not return, it will stay in endless loop
*		
* remarks This function tests the  register (R0-R7, R12, LR, APSR) by rules IEC60730 Class B
*
********************************************************************************/ 
     IEC60730B_RESULT IEC60730B_CM4_CM7_CPU_RegisterTest(void);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_CPU_NonStackedRegisterTest
*
* param  none
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                         IEC60730B_ST_CPU_NONSTACKED_REGISTER_FAIL - Test failed
*                     
*		
* remarks This function tests the   registers (R8, R9, R10, R11) by rules IEC60730 Class B
*
*******************************************************************************/
     IEC60730B_RESULT IEC60730B_CM4_CM7_CPU_NonStackedRegisterTest(void);  
     
/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_CPU_PrimaskTest
*
* param  none
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                         IEC60730B_ST_CPU_PRIMASK_FAIL - Test failed
*                     
*		
* remarks This function tests the  PRIMASK register by rules IEC60730 Class B
*
*******************************************************************************/     
     IEC60730B_RESULT IEC60730B_CM4_CM7_CPU_PrimaskTest(void);
     
/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_CPU_SPmainTest
*
* param  none
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                     or function will not return, it will stay in endless loop
*                     		
* remarks This function tests the Cortex CM4 and CM7 SP_main and SP_process registers by rules IEC60730 Class B
*
*******************************************************************************/      
     IEC60730B_RESULT IEC60730B_CM4_CM7_CPU_SPmainTest(void); 
     
/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_CPU_SPprocessTest
*
* param  none
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                    or function will not return, it will stay in endless loop
*                     		
* remarks This function tests  SP_process registers by rules IEC60730 Class B
*
*******************************************************************************/      
     IEC60730B_RESULT IEC60730B_CM4_CM7_CPU_SPprocessTest(void);
     
/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_CPU_ControlTest
*
* param  none
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                         IEC60730B_ST_CPU_CONTROL_FAIL - Test failed
*                     		
* remarks This function tests the Cortex CM4 and CM7 CONTROL register by rules IEC60730 Class B
*
*******************************************************************************/     
     IEC60730B_RESULT IEC60730B_CM4_CM7_CPU_ControlTest(void);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_CPU_ControlTest_fpu
*
* param  none
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                         IEC60730B_ST_CPU_CONTROL_FAIL - Test failed
*                     		
* remarks This function tests the Cortex CM4 CONTROL register by rules IEC60730 Class B
*         Only for devices with Floating Point Unit
*******************************************************************************/     
     IEC60730B_RESULT IEC60730B_CM4_CM7_CPU_ControlTest_fpu(void);
     
/********************************************************************************
*
* brief  IEC60730B_CM4_CM7_CPU_SpecialTest
*
* param  none
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                        IEC60730B_ST_CPU_SPECIAL_FAIL - Test failed
*		
* remarks This function tests others of the M4 and M7 CPU register, which are not included in M0+
*         by rules IEC60730 Class B
*         
************************************************************************************/        
     IEC60730B_RESULT IEC60730B_CM4_CM7_CPU_SpecialTest(void);
     
/********************************************************************************
*
* brief  IEC60730B_CM4_CM7_CPU_SpecialTest_8PriorityLevels
*
* param  none
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                        IEC60730B_ST_CPU_SPECIAL_FAIL - Test failed
*		
* remarks This function tests BASEPRI and FAULTMASK registers 
* for devices with 8 priority levels for interrupts
* by rules IEC60730 Class B
*
************************************************************************************/  
     IEC60730B_RESULT IEC60730B_CM4_CM7_CPU_SpecialTest_8PriorityLevels(void);     
       
/********************************************************************************
*
* brief  IEC60730B_CM4_CM7_CPU_FloatTest1
*
* param  none
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                        IEC60730B_ST_CPU_FLOAT_1_FAIL - Test failed
*		
* remarks This function tests the M4 and M7 FPU register (FPSCR,S0-S15) by rules IEC60730 Class B
*         Only for devices with Floating Point Unit
************************************************************************************/      
     IEC60730B_RESULT IEC60730B_CM4_CM7_CPU_FloatTest1(void);
     
/********************************************************************************
*
* brief  IEC60730B_CM4_CM7_CPU_FloatTest2
*
* param  none
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                        IEC60730B_ST_CPU_FLOAT_2_FAIL - Test failed
*                     		
* remarks This function tests the M4 and M7 FPU register (S16-S31) by rules IEC60730 Class B
*         Only for devices with Floating Point Unit
************************************************************************************/     
     IEC60730B_RESULT IEC60730B_CM4_CM7_CPU_FloatTest2(void);
     
#endif /* __ASM__ */

     
#endif /* IEC60730_B_CM4_CM7_REG_H_ */
