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
 * @brief Digital I/O extended test common routines - IEC60730 Class B.
 */

#ifndef IEC60730B_DIO_EXT_H_
#define IEC60730B_DIO_EXT_H_

/*
 * List of devices and supported functions can be found in library user guide,
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* PORT register masks */
#define PORT_PCR_PS_MSK    (0x1U)
#define PORT_PCR_PE_MSK    (0x2U)
#define PORT_PCR_MUX_7_MSK (0x700U)
#define PORT_PCR_MUX_1_MSK (0x100U)

/* IMX8M */
/* PORT register masks */
#define FS_IOMUXC_SW_PAD_CTL_PAD_PUE_MASK (0x40U)
#define FS_IOMUXC_SW_PAD_CTL_PAD_PE_MASK  (0x100U)

/* Assembler skips following section */
#ifndef __ASM__

/******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 *
 * @brief    Function tests whether the tested pin, that is configured as the input one, has the expected value.
 *           It is adapted to be used as a "get" function for the FS_DIO_ShortToSupplySet*() and the
 *FS_DIO_ShortToAdjSet*() function. If the Adjacent pin is not involved in test, then tested pin parameters can be
 *entered instead the adjacent pin parameters. Both tested pin and adjacent pin must be configured as GPIO before the
 *call of the function!!!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    *pAdjPin       - pointer to adjacent pin struct.
 * @param    testedPinValue - expected value of the tested pin (logical 0 or logical 1).
 * @param    backupEnable   - flag. if non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_InputExt(fs_dio_test_t *pTestedPin,
                          fs_dio_test_t *pAdjPin,
                          bool_t testedPinValue,
                          bool_t backupEnable);

/*******************************************************************************
 *
 * @brief    Function sets the tested pin as GPIO input with pull-up, or pull-down enable, to test the pin against the
 *short to Vdd or GND. For getting the result of the test, call the "FS_DIO_InputExt*()" function with the expected
 *"testedPinValue" sets to 0 if for VDD and sets to 1 if for GND. Tested pin must be configured as GPIO before the call
 *of the function!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    shortToVoltage - specify whether pin is tested for short against Gnd, or Vdd.
 *                            For GND - enter 1, for VDD - enter 0 or non-zero.
 * @param    backupEnable   - flag. If non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_ShortToSupplySet(fs_dio_test_t *pTestedPin, bool_t shortToVoltage, bool_t backupEnable);

/*******************************************************************************
 *
 * @brief    Function sets the tested pin as GPIO input with "testedPinValue".
 *           And the adjacent pin as GPIO output with opposite output value as for the tested pin.
 *           For getting the result of the test, call the "FS_DIO_InputExt*()" function with the expected
 *"testedPinValue". Both tested pin and adjacent pin must be configured as GPIO before the call of the function!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    *pAdjPin       - pointer to adjacent pin struct.
 * @param    testedPinValue - value that will be set on the tested pin.
 * @param    backupEnable   - flag. If non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_ShortToAdjSet(fs_dio_test_t *pTestedPin,
                               fs_dio_test_t *pAdjPin,
                               bool_t testedPinValue,
                               bool_t backupEnable);

/*******************************************************************************
 *
 * @brief    Function tests whether the tested pin, that is configured as the input one, has the expected value.
 *           It is adapted to be used as a "get" function for the FS_DIO_ShortToSupplySet*() and the
 *FS_DIO_ShortToAdjSet*() function. If the Adjacent pin is not involved in test, then tested pin parameters can be
 *entered instead the adjacent pin parameters. Both tested pin and adjacent pin must be configured as GPIO before the
 *call of the function!!!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    *pAdjPin       - pointer to adjacent pin struct.
 * @param    testedPinValue - expected value of the tested pin (logical 0 or logical 1).
 * @param    backupEnable   - flag. if non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_InputExt_IMXRT(fs_dio_test_imx_t *pTestedPin,
                                fs_dio_test_imx_t *pAdjPin,
                                bool_t testedPinValue,
                                bool_t backupEnable);

/*******************************************************************************
 *
 * @brief    Function sets the tested pin as GPIO input with pull-up, or pull-down enable, to test the pin against the
 *short to Vdd or GND. For getting the result of the test, call the "FS_DIO_InputExt*()" function with the expected
 *"testedPinValue" sets to 0 if for VDD and sets to 1 if for GND. Tested pin must be configured as GPIO before the call
 *of the function!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    shortToVoltage - specify whether pin is tested for short against Gnd, or Vdd.
 *                            For GND - enter 1, for VDD - enter 0 or non-zero.
 * @param    backupEnable   - flag. If non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_ShortToSupplySet_IMXRT(fs_dio_test_imx_t *pTestedPin, bool_t shortToVoltage, bool_t backupEnable);

/*******************************************************************************
 *
 * @brief    Function sets the tested pin as GPIO input with "testedPinValue".
 *           And the adjacent pin as GPIO output with opposite output value as for the tested pin.
 *           For getting the result of the test, call the "FS_DIO_InputExt*()" function with the expected
 *"testedPinValue". Both tested pin and adjacent pin must be configured as GPIO before the call of the function!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    *pAdjPin       - pointer to adjacent pin struct.
 * @param    testedPinValue - value that will be set on the tested pin.
 * @param    backupEnable   - flag. If non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_ShortToAdjSet_IMXRT(fs_dio_test_imx_t *pTestedPin,
                                     fs_dio_test_imx_t *pAdjPin,
                                     bool_t testedPinValue,
                                     bool_t backupEnable);

/*******************************************************************************
 *
 * @brief    Function tests whether the tested pin, that is configured as the input one, has the expected value.
 *           It is adapted to be used as a "get" function for the FS_DIO_ShortToSupplySet*() and the
 *FS_DIO_ShortToAdjSet*() function. If the Adjacent pin is not involved in test, then tested pin parameters can be
 *entered instead the adjacent pin parameters. Both tested pin and adjacent pin must be configured as GPIO before the
 *call of the function!!!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    *pAdjPin       - pointer to adjacent pin struct.
 * @param    testedPinValue - expected value of the tested pin (logical 0 or logical 1).
 * @param    backupEnable   - flag. if non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/

FS_RESULT FS_DIO_InputExt_IMX8M(fs_dio_test_imx_t *pTestedPin,
                                fs_dio_test_imx_t *pAdjPin,
                                bool_t testedPinValue,
                                bool_t backupEnable);

/*******************************************************************************
 *
 * @brief    Function sets the tested pin as GPIO input with pull-up, or pull-down enable, to test the pin against the
 *short to Vdd or GND. For getting the result of the test, call the "FS_DIO_InputExt*()" function with the expected
 *"testedPinValue" sets to 0 if for VDD and sets to 1 if for GND. Tested pin must be configured as GPIO before the call
 *of the function!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    shortToVoltage - specify whether pin is tested for short against Gnd, or Vdd.
 *                            For GND - enter 1, for VDD - enter 0 or non-zero.
 * @param    backupEnable   - flag. If non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/

FS_RESULT FS_DIO_ShortToSupplySet_IMX8M(fs_dio_test_imx_t *pTestedPin, bool_t shortToVoltage, bool_t backupEnable);

/*******************************************************************************
 *
 * @brief    Function sets the tested pin as GPIO input with "testedPinValue".
 *           And the adjacent pin as GPIO output with opposite output value as for the tested pin.
 *           For getting the result of the test, call the "FS_DIO_InputExt*()" function with the expected
 *"testedPinValue". Both tested pin and adjacent pin must be configured as GPIO before the call of the function!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    *pAdjPin       - pointer to adjacent pin struct.
 * @param    testedPinValue - value that will be set on the tested pin.
 * @param    backupEnable   - flag. If non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/

FS_RESULT FS_DIO_ShortToAdjSet_IMX8M(fs_dio_test_imx_t *pTestedPin,
                                     fs_dio_test_imx_t *pAdjPin,
                                     bool_t testedPinValue,
                                     bool_t backupEnable);

/*******************************************************************************
 *
 * @brief    Function tests whether the tested pin, that is configured as the input one, has the expected value.
 *           It is adapted to be used as a "get" function for the FS_DIO_ShortToSupplySet*() and the
 *FS_DIO_ShortToAdjSet*() function. If the Adjacent pin is not involved in test, then tested pin parameters can be
 *entered instead the adjacent pin parameters. Both tested pin and adjacent pin must be configured as GPIO before the
 *call of the function!!!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    *pAdjPin       - pointer to adjacent pin struct.
 * @param    testedPinValue - expected value of the tested pin (logical 0 or logical 1).
 * @param    backupEnable   - flag. if non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_InputExt_LPC(fs_dio_test_lpc_t *pTestedPin,
                              fs_dio_test_lpc_t *pAdjPin,
                              bool_t testedPinValue,
                              bool_t backupEnable);

/*******************************************************************************
 *
 * @brief    Function sets the tested pin as GPIO input with pull-up, or pull-down enable, to test the pin against the
 *short to Vdd or GND. For getting the result of the test, call the "FS_DIO_InputExt*()" function with the expected
 *"testedPinValue" sets to 0 if for VDD and sets to 1 if for GND. Tested pin must be configured as GPIO before the call
 *of the function!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    shortToVoltage - specify whether pin is tested for short against Gnd, or Vdd.
 *                            For GND - enter 1, for VDD - enter 0 or non-zero.
 * @param    backupEnable   - flag. If non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_ShortToSupplySet_LPC(fs_dio_test_lpc_t *pTestedPin, bool_t shortToVoltage, bool_t backupEnable);

/*******************************************************************************
 *
 * @brief    Function sets the tested pin as GPIO input with "testedPinValue".
 *           And the adjacent pin as GPIO output with opposite output value as for the tested pin.
 *           For getting the result of the test, call the "FS_DIO_InputExt*()" function with the expected
 *"testedPinValue". Both tested pin and adjacent pin must be configured as GPIO before the call of the function!
 *
 * @param    *pTestedPin    - pointer to tested pin struct.
 * @param    *pAdjPin       - pointer to adjacent pin struct.
 * @param    testedPinValue - value that will be set on the tested pin.
 * @param    backupEnable   - flag. If non-zero, backup functionality is enable/active.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_ShortToAdjSet_LPC(fs_dio_test_lpc_t *pTestedPin,
                                   fs_dio_test_lpc_t *pAdjPin,
                                   bool_t testedPinValue,
                                   bool_t backupEnable);

#endif /* __ASM__ */
#endif /* IEC60730B_DIO_EXT_H_ */
