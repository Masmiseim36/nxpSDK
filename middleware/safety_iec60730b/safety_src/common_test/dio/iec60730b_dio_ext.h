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
* @file      IEC60730_B_CM4_CM7_dio_ext.h
*
* @author    B48632
*
* @version   3.0
*
* @date      Nov-01-2018
*
* @brief     IEC60730 Class B Safety Routines for Digital IO
*
************************************************************************************************/

#ifndef _IEC60730_B_CM4_CM7_DIO_EXT_H_
#define _IEC60730_B_CM4_CM7_DIO_EXT_H_

/******************************************************************************
* Global variables
******************************************************************************/

/* PORT register masks */
#define PORT_PCR_PS_MSK                         (0x1U)
#define PORT_PCR_PE_MSK                         (0x2U)
#define PORT_PCR_MUX_7_MSK                      (0x700U)
#define PORT_PCR_MUX_1_MSK                      (0x100U)

/* IMXRT */
/* PORT register masks */
#define IOMUXC_SW_PAD_CTL_PAD_PUS_1_MSK      (0x4000U)
#define IOMUXC_SW_PAD_CTL_PAD_PUS_2_MSK      (0x8000U)
#define IOMUXC_SW_PAD_CTL_PAD_PUS_3_MSK      (0xC000U)
#define IOMUXC_SW_PAD_CTL_PAD_PUS_4_MSK      (0xC000U)
#define IOMUXC_SW_PAD_CTL_PAD_PUE_1_MSK      (0x2000U)
#define IOMUXC_SW_PAD_CTL_PAD_PKE_1_MSK      (0x1000U)
#define IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_5_MSK (0x5U)
#define IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_7_MSK (0x7U)

/******************************************************************************
* Global functions
******************************************************************************/
#ifndef __ASM__

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_DIO_InputTest_Ext
*
* param  *pTestedPin      - pointer to tested pin struct (dio_test_t)
* param  *pAdjPin         - pointer to adjacent pin struct (dio_test_t)  
* param  TestedPinValue   - expected value of the tested pin (logical 0 or logical 1)
* param  BackupEnable     - flag. if non-zero, backup functionality is enable/active
*
* return - IEC60730B_ST_DIO_FAIL
*        - IEC60730B_ST_DIO_PASS
*
* remarks -Function tests whether the tested pin, that is configured as the input one, has the expected value.
*          It is adapted to be used as the a "get" function for the IEC60730B_CM4_CM7_DIO_ShortToAdjTest_Set() and 
*          IEC60730B_CM4_CM7_DIO_ShortToSupplyTest_Set() functions.
*          If the Adjacent pin is not involved in test, then the Tested pin parameters can be entered instead the Adjacent pin parameters.
*        
*          Both "TestedPin" and "AdjacentPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_DIO_InputTest_Ext(dio_test_t *pTestedPin, dio_test_t *pAdjPin, unsigned long TestedPinValue, unsigned short BackupEnable);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_DIO_ShortToSupplyTest_Set
*
* param  *pTestedPin    - pointer to tested pin struct (dio_test_t)
* param  ShortToVoltage - specify whether pin is tested for short against Gnd, or Vdd. for GND - enter 1, for VDD - enter 0 or non-zero  
* param  BackupEnable   - flag. if non-zero, backup functionality is enable/active
*
* return - IEC60730B_ST_DIO_FAIL
*        - IEC60730B_ST_DIO_PASS
*
* remarks -Function sets the tested pin as GPIO input with pull-down, or pull-down enable, to test the pin against the short to Vdd or GND. 
*          For getting the result of the test, call the "IEC60730B_CM4_CM7_DIO_InputTest_Ext" function with the expected "TestedPinValue" sets to 0 if for VDD and sets to 1 if for GND.
*        
*          "TestedPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_DIO_ShortToSupplyTest_Set(dio_test_t *pTestedPin, unsigned short ShortToVoltage, unsigned short BackupEnable);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_DIO_ShortToAdjTest_Set
*
* param  *pTestedPin      - pointer to tested pin struct (dio_test_t)
* param  *pAdjPin         - pointer to adjacent pin struct (dio_test_t)
* param  TestedPinValue   - value that will be set on the tested pin (logical 0 or logical 1)
* param  BackupEnable     - flag. if non-zero, backup functionality is enable/active
*
* return - IEC60730B_ST_DIO_FAIL
*        - IEC60730B_ST_DIO_PASS
*
* remarks -Function sets the tested pin as GPIO input with "TestedPinValue". 
*          And the adjacent pin as GPIO output with opposite output value as for the tested pin.
*          For getting the result of the test, call the "IEC60730B_CM4_CM7_DIO_InputTest" function with the expected "TestedPinValue".
*        
*          Both "TestedPin" and "AdjacentPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_DIO_ShortToAdjTest_Set(dio_test_t *pTestedPin, dio_test_t *pAdjPin, unsigned long TestedPinValue, unsigned short BackupEnable);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_DIO_InputTest_Ext_RT
*
* param  *pTestedPin      - pointer to tested pin struct (dio_test_rt_t)
* param  *pAdjPin         - pointer to adjacent pin struct (dio_test_rt_t)  
* param  TestedPinValue   - expected value of the tested pin (logical 0 or logical 1)
* param  BackupEnable     - flag. if non-zero, backup functionality is enable/active
*
* return - IEC60730B_ST_DIO_FAIL
*        - IEC60730B_ST_DIO_PASS
*
* remarks -Function tests whether the tested pin, that is configured as the input one, has the expected value.
*          It is adapted to be used as the a "get" function for the IEC60730B_CM4_CM7_DIO_ShortToAdjTest_Set() and 
*          IEC60730B_CM4_CM7_DIO_ShortToSupplyTest_Set() functions.
*          If the Adjacent pin is not involved in test, then the Tested pin parameters can be entered instead the Adjacent pin parameters.
*        
*          Both "TestedPin" and "AdjacentPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_DIO_InputTest_Ext_RT(dio_test_rt_t *pTestedPin, dio_test_rt_t *pAdjPin, unsigned long TestedPinValue, unsigned short BackupEnable);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_DIO_ShortToSupplyTest_Set_RT
*
* param  *pTestedPin    - pointer to tested pin struct (dio_test_rt_t)
* param  ShortToVoltage - specify whether pin is tested for short against Gnd, or Vdd. for GND - enter 1, for VDD - enter 0 or non-zero  
* param  BackupEnable   - flag. if non-zero, backup functionality is enable/active
*
* return - IEC60730B_ST_DIO_FAIL
*        - IEC60730B_ST_DIO_PASS
*
* remarks -Function sets the tested pin as GPIO input with pull-down, or pull-down enable, to test the pin against the short to Vdd or GND. 
*          For getting the result of the test, call the "IEC60730B_CM4_CM7_DIO_InputTest_Ext" function with the expected "TestedPinValue" sets to 0 if for VDD and sets to 1 if for GND.
*        
*          "TestedPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_DIO_ShortToSupplyTest_Set_RT(dio_test_rt_t *pTestedPin, unsigned short ShortToVoltage, unsigned short BackupEnable);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_DIO_ShortToAdjTest_Set_RT
*
* param  *pTestedPin      - pointer to tested pin struct (dio_test_rt_t)
* param  *pAdjPin         - pointer to adjacent pin struct (dio_test_rt_t)
* param  TestedPinValue   - value that will be set on the tested pin (logical 0 or logical 1)
* param  BackupEnable     - flag. if non-zero, backup functionality is enable/active
*
* return - IEC60730B_ST_DIO_FAIL
*        - IEC60730B_ST_DIO_PASS
*
* remarks -Function sets the tested pin as GPIO input with "TestedPinValue". 
*          And the adjacent pin as GPIO output with opposite output value as for the tested pin.
*          For getting the result of the test, call the "IEC60730B_CM4_CM7_DIO_InputTest" function with the expected "TestedPinValue".
*        
*          Both "TestedPin" and "AdjacentPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_DIO_ShortToAdjTest_Set_RT(dio_test_rt_t *pTestedPin, dio_test_rt_t *pAdjPin, unsigned long TestedPinValue, unsigned short BackupEnable);

#endif /* __ASM__ */

#endif /* _IEC60730_B_CM4_CM7_DIO_EXT_H_ */

