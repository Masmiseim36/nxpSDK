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
 * @brief     IEC60730 Class B Safety Routines for Digital I/O
 *
 ***********************************************************************************************/

#ifndef _IEC60730B_DIO_EXT_
#define _IEC60730B_DIO_EXT_

/******************************************************************************
* Global variables
******************************************************************************/
/* PORT register masks */
#define PORT_PCR_PS_MSK     (0x1)
#define PORT_PCR_PE_MSK     (0x2)
#define PORT_PCR_MUX_7_MSK  (0x700)
#define PORT_PCR_MUX_1_MSK  (0x100)

/* IMXRT */
/* PORT register masks */
#define IOMUXC_SW_PAD_CTL_PAD_PUS_1_MSK      (0x4000)
#define IOMUXC_SW_PAD_CTL_PAD_PUS_2_MSK      (0x8000)
#define IOMUXC_SW_PAD_CTL_PAD_PUS_3_MSK      (0xC000)
#define IOMUXC_SW_PAD_CTL_PAD_PUS_4_MSK      (0xC000)
#define IOMUXC_SW_PAD_CTL_PAD_PUE_1_MSK      (0x2000)
#define IOMUXC_SW_PAD_CTL_PAD_PKE_1_MSK      (0x1000)
#define IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_5_MSK (0x5)
#define IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_7_MSK (0x7)

/******************************************************************************
* Global functions
******************************************************************************/
#ifndef __ASM__

/*******************************************************************************
*
* brief  FS_DIO_InputExt
*
* param  *pTestedPin      - pointer to tested pin struct (fs_dio_test_t)
* param  *pAdjPin         - pointer to adjacent pin struct (fs_dio_test_t)  
* param  TestedPinValue   - expected value of the tested pin (logical 0 or logical 1)
* param  BackupEnable     - flag. if non-zero, backup functionality is enable/active
*
* return - FS_DIO_FAIL
*        - FS_DIO_PASS
*
* remarks -Function tests whether the tested pin, that is configured as the input one, has the expected value.
*          It is adapted to be used as a "get" function for the FS_DIO_ShortToAdjSet() function.
*          If the Adjacent pin is not involved in test, then tested pin parameters can be entered instead the adjacent pin parameters.
*        
*          Both "TestedPin" and "AdjacentPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
FS_RESULT FS_DIO_InputExt(fs_dio_test_t *pTestedPin, fs_dio_test_t *pAdjPin, bool_t testedPinValue, bool_t backupEnable);

/*******************************************************************************
*
* brief  FS_DIO_ShortToSupplySet
*
* param  *pTestedPin    - pointer to tested pin struct (fs_dio_test_t)
* param  ShortToVoltage - specify whether pin is tested for short against Gnd, or Vdd. for GND - enter 1, for VDD - enter 0 or non-zero  
* param  BackupEnable   - flag. if non-zero, backup functionality is enable/active
*
* return - FS_DIO_FAIL
*        - FS_DIO_PASS
*
* remarks -Function sets the tested pin as GPIO input with pull-down, or pull-down enable, to test the pin against the short to Vdd or GND. 
*          For getting the result of the test, call the "FS_DIO_InputTest" function with the expected "TestedPinValue" sets to 0 if for VDD and sets to 1 if for GND.
*        
*          "TestedPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
FS_RESULT FS_DIO_ShortToSupplySet(fs_dio_test_t *pTestedPin, bool_t shortToVoltage, bool_t backupEnable);

/*******************************************************************************
*
* brief  FS_DIO_ShortToAdjSet
*
* param  *pTestedPin      - pointer to tested pin struct (fs_dio_test_t)
* param  *pAdjPin         - pointer to adjacent pin struct (fs_dio_test_t)
* param  TestedPinValue   - value that will be set on the tested pin (logical 0 or logical 1)
* param  BackupEnable     - flag. if non-zero, backup functionality is enable/active
*
* return - FS_DIO_FAIL
*        - FS_DIO_PASS
*
* remarks -Function sets the tested pin as GPIO input with "TestedPinValue". 
*          And the adjacent pin as GPIO output with opposite output value as for the tested pin.
*          For getting the result of the test, call the "FS_DIO_InputTest" function with the expected "TestedPinValue".
*        
*          Both "TestedPin" and "AdjacentPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
FS_RESULT FS_DIO_ShortToAdjSet(fs_dio_test_t *pTestedPin, fs_dio_test_t *pAdjPin, bool_t testedPinValue, bool_t backupEnable);

/*******************************************************************************
*
* brief  FS_DIO_InputExt_RT
*
* param  *pTestedPin      - pointer to tested pin struct (fs_dio_test_rt_t)
* param  *pAdjPin         - pointer to adjacent pin struct (fs_dio_test_rt_t)  
* param  TestedPinValue   - expected value of the tested pin (logical 0 or logical 1)
* param  BackupEnable     - flag. if non-zero, backup functionality is enable/active
*
* return - FS_DIO_FAIL
*        - FS_DIO_PASS
*
* remarks -Function tests whether the tested pin, that is configured as the input one, has the expected value.
*          It is adapted to be used as the a "get" function for the FS_DIO_ShortToAdjSet_RT() and 
*          FS_DIO_ShortToSupplySet_RT() functions.
*          If the Adjacent pin is not involved in test, then the Tested pin parameters can be entered instead the Adjacent pin parameters.
*        
*          Both "TestedPin" and "AdjacentPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
FS_RESULT FS_DIO_InputExt_RT(fs_dio_test_rt_t *pTestedPin, fs_dio_test_rt_t *pAdjPin, bool_t testedPinValue, bool_t backupEnable);

/*******************************************************************************
*
* brief  FS_DIO_ShortToSupplySet_RT
*
* param  *pTestedPin    - pointer to tested pin struct (fs_dio_test_rt_t)
* param  ShortToVoltage - specify whether pin is tested for short against Gnd, or Vdd. for GND - enter 1, for VDD - enter 0 or non-zero  
* param  BackupEnable   - flag. if non-zero, backup functionality is enable/active
*
* return - FS_DIO_FAIL
*        - FS_DIO_PASS
*
* remarks -Function sets the tested pin as GPIO input with pull-down, or pull-down enable, to test the pin against the short to Vdd or GND. 
*          For getting the result of the test, call the "FS_DIO_InputTestExt" function with the expected "TestedPinValue" sets to 0 if for VDD and sets to 1 if for GND.
*        
*          "TestedPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
FS_RESULT FS_DIO_ShortToSupplySet_RT(fs_dio_test_rt_t *pTestedPin, bool_t shortToVoltage, bool_t backupEnable);

/*******************************************************************************
*
* brief  FS_DIO_ShortToAdjSet_RT
*
* param  *pTestedPin      - pointer to tested pin struct (fs_dio_test_rt_t)
* param  *pAdjPin         - pointer to adjacent pin struct (fs_dio_test_rt_t)
* param  TestedPinValue   - value that will be set on the tested pin (logical 0 or logical 1)
* param  BackupEnable     - flag. if non-zero, backup functionality is enable/active
*
* return - FS_DIO_FAIL
*        - FS_DIO_PASS
*
* remarks -Function sets the tested pin as GPIO input with "TestedPinValue". 
*          And the adjacent pin as GPIO output with opposite output value as for the tested pin.
*          For getting the result of the test, call the "FS_DIO_InputTest" function with the expected "TestedPinValue".
*        
*          Both "TestedPin" and "AdjacentPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
FS_RESULT FS_DIO_ShortToAdjSet_RT(fs_dio_test_rt_t *pTestedPin, fs_dio_test_rt_t *pAdjPin, bool_t testedPinValue, bool_t backupEnable);

/*******************************************************************************
*
* brief  FS_DIO_InputExt_LPC
*
* param  *pTestedPin      - pointer to tested pin struct (fs_dio_test_rt_t)
* param  *pAdjPin         - pointer to adjacent pin struct (fs_dio_test_rt_t)  
* param  testedPinValue   - expected value of the tested pin (logical 0 or logical 1)
* param  backupEnable     - flag. if non-zero, backup functionality is enable/active
*
* return - FS_DIO_FAIL
*        - FS_DIO_PASS
*
* remarks -Function tests whether the tested pin, that is configured as the input one, has the expected value.
*          It is adapted to be used as the a "get" function for the FS_DIO_ShortToAdjSet_LPC() and 
*          FS_DIO_ShortToSupplySet_LPC() functions.
*          If the Adjacent pin is not involved in test, then the Tested pin parameters can be entered instead the Adjacent pin parameters.
*        
*          Both "TestedPin" and "AdjacentPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
FS_RESULT FS_DIO_InputExt_LPC(fs_dio_test_lpc_t *pTestedPin, fs_dio_test_lpc_t *pAdjPin, bool_t testedPinValue, bool_t backupEnable);

/*******************************************************************************
*
* brief  FS_DIO_ShortToSupplySet_LPC
*
* param  *pTestedPin    - pointer to tested pin struct (fs_dio_test_rt_t)
* param  shortToVoltage - specify whether pin is tested for short against Gnd, or Vdd. for GND - enter 1, for VDD - enter 0 or non-zero  
* param  backupEnable   - flag. if non-zero, backup functionality is enable/active
*
* return - FS_DIO_FAIL
*        - FS_DIO_PASS
*
* remarks -Function sets the tested pin as GPIO input with pull-down, or pull-down enable, to test the pin against the short to Vdd or GND. 
*          For getting the result of the test, call the "FS_DIO_InputTestExt" function with the expected "TestedPinValue" sets to 0 if for VDD and sets to 1 if for GND.
*        
*          "TestedPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
FS_RESULT FS_DIO_ShortToSupplySet_LPC(fs_dio_test_lpc_t *pTestedPin, bool_t shortToVoltage, bool_t backupEnable);

/*******************************************************************************
*
* brief  FS_DIO_ShortToAdjSet_LPC
*
* param  *pTestedPin      - pointer to tested pin struct (fs_dio_test_rt_t)
* param  *pAdjPin         - pointer to adjacent pin struct (fs_dio_test_rt_t)
* param  testedPinValue   - value that will be set on the tested pin (logical 0 or logical 1)
* param  backupEnable     - flag. if non-zero, backup functionality is enable/active
*
* return - FS_DIO_FAIL
*        - FS_DIO_PASS
*
* remarks -Function sets the tested pin as GPIO input with "TestedPinValue". 
*          And the adjacent pin as GPIO output with opposite output value as for the tested pin.
*          For getting the result of the test, call the "FS_DIO_InputTest" function with the expected "TestedPinValue".
*        
*          Both "TestedPin" and "AdjacentPin" must be configured as GPIO before the call of the function !!!!
*      
*******************************************************************************/
FS_RESULT FS_DIO_ShortToAdjSet_LPC(fs_dio_test_lpc_t *pTestedPin, fs_dio_test_lpc_t *pAdjPin, bool_t testedPinValue, bool_t backupEnable);

#endif /* __ASM__ */

#endif /* _IEC60730B_DIO_EXT_ */
