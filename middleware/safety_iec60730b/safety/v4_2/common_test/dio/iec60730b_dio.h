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
 * @brief Digital I/O test common routines - IEC60730 Class B.
 */

#ifndef IEC60730B_DIO_H_
#define IEC60730B_DIO_H_

/*
 * List of devices and supported functions can be found in library user guide,
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* Return values */
#define FS_FAIL_DIO             (FS_DIO_CODE + 0x1U)
#define FS_FAIL_DIO_OUTPUT      (FS_DIO_CODE + 0x2U)
#define FS_FAIL_DIO_INPUT       (FS_DIO_CODE + 0x3U)
#define FS_FAIL_DIO_NOT_SET     (FS_DIO_CODE + 0x4U)
#define FS_FAIL_DIO_NOT_CLEAR   (FS_DIO_CODE + 0x5U)
#define FS_FAIL_DIO_MODE        (FS_DIO_CODE + 0x6U)
#define FS_FAIL_DIO_WRONG_VALUE (FS_DIO_CODE + 0x7U)

/******************************************************************************
 * Macros for GPIO pins
 ******************************************************************************/
/* LPC */
#define FS_IOCON_PIO_2BIT_MASK 0x3UL
#define FS_IOCON_PIO_PULL_DOWN 0x1UL
#define FS_IOCON_PIO_PULL_UP   0x2UL
   
/* LPC54S018M */
#define FS_IOCON_PIO_DIGIMODE_MASK (0x100U)
   
/* Assembler skips following section */
#ifndef __ASM__

/******************************************************************************
 * Definitions
 ******************************************************************************/
/* Used to backup original DIO port/pin configuration */
/* Kinetis */
typedef struct
{
    uint32_t pcr;  /* Pin control register */
    uint32_t pddr; /* Port data direction register */
    uint32_t pdor; /* Port data output register */
} fs_dio_backup_t;

/* IMX */
typedef struct
{
    uint32_t mux;  /* SW_MUX control register */
    uint32_t pad;  /* SW_PAD control register  */
    uint32_t gdir; /* GPIO direction register */
    uint32_t dr;   /* GPIO data register */
} fs_dio_backup_imx_t;

/* LPC */
typedef struct
{
    uint8_t byte_backup;   /* Port state register */
    uint32_t dir_backup;   /* Port direction register */
    uint32_t iocon_backup; /* IOCON settings register */
} fs_dio_backup_lpc_t;

/* Safety DIO test item Kinetis */
typedef struct
{
    uint32_t gpio;
    uint32_t pcr;
    uint8_t pinNum;
    uint8_t pinDir;
    uint8_t pinMux;
    fs_dio_backup_t sTestedPinBackup;
} fs_dio_test_t;

/* Safety DIO test item IMX */
typedef struct
{
    uint32_t gpio;
    uint32_t muxAddr;
    uint32_t padAddr;
    uint8_t pinNum;
    uint8_t pinDir;
    uint8_t pinMux;
    fs_dio_backup_imx_t sTestedPinBackup;
    uint32_t pullUpMask;
    uint32_t pullDownMask;
    uint32_t pullResetMask;
} fs_dio_test_imx_t;

/* Safety DIO test item LPC */
typedef struct
{
    uint8_t *pPort_byte;   /* Adress of byte register in GPIO */
    uint32_t *pPort_dir;   /* Adress of dir register in GPIO */
    uint32_t *pPort_Iocon; /* Adress of concrete IOCON register */
    uint32_t iocon_mode_shift;
    uint8_t pinNum;
    uint8_t pinDir;
    uint32_t gpio_clkc_shift;
    uint8_t iocon_digimode;
    fs_dio_backup_lpc_t sTestedPinBackup;
} fs_dio_test_lpc_t;

/******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 *
 * @brief    Checks whether the tested pin has the expected input value.
 *
 * @param    *pTestedPin   - pointer to tested pin struct.
 * @param    expectedValue - expected input value.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_Input(fs_dio_test_t *pTestedPin, bool_t expectedValue);

/*******************************************************************************
 *
 * @brief    This function executes the digital output test.
 *           The test is focused only to test one digital pin.
 *
 * @param    *pTestedPin - pointer to tested pin struct.
 * @param    delay       - the delay needed to recognize the value change on tested pin.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_Output(fs_dio_test_t *pTestedPin, uint32_t delay);

/*******************************************************************************
 *
 * @brief    This function executes the digital output test.
 *           The test is focused only to test one digital pin.
 *
 * @param    *pTestedPin - pointer to tested pin struct.
 * @param    delay       - the delay needed to recognize the value change on tested pin.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_Output_IMXRT(fs_dio_test_imx_t *pTestedPin, uint32_t delay);

/*******************************************************************************
 *
 * @brief    This function executes the digital output test.
 *           The test is focused only to test one digital pin.
 *
 * @param    *pTestedPin - pointer to tested pin struct.
 * @param    delay       - the delay needed to recognize the value change on tested pin.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_Output_IMX8M(fs_dio_test_imx_t *pTestedPin, uint32_t delay);

/*******************************************************************************
 *
 * @brief    This function executes the digital output test.
 *           The test is focused only to test one digital pin.
 *
 * @param    *pTestedPin - pointer to tested pin struct.
 * @param    delay       - the delay needed to recognize the value change on tested pin.
 *
 * @return   FS_PASS
 *           FS_FAIL_DIO
 *
 ******************************************************************************/
FS_RESULT FS_DIO_Output_LPC(fs_dio_test_lpc_t *pTestedPin, uint32_t delay);

#endif /* __ASM__ */
#endif /* IEC60730B_DIO_H_ */
