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

#ifndef _IEC60730B_DIO_
#define _IEC60730B_DIO_

/******************************************************************************
* Global variables
******************************************************************************/
/* DIO test result constants */
#define FS_DIO_PASS  (0x0)
#define FS_DIO_FAIL  (FS_DIO_FAIL_CODE|0x1)

/******************************************************************************
* Macros for GPIO pins - module address + pcr address
******************************************************************************/
/* Kinetis */
#define FS_DIO_PORTA 0x400FF000U
#define FS_DIO_PORTB 0x400FF040U
#define FS_DIO_PORTC 0x400FF080U
#define FS_DIO_PORTD 0x400FF0C0U
#define FS_DIO_PORTE 0x400FF100U
#define FS_DIO_PORTF 0x400FF140U
#define FS_DIO_PORTG 0x400FF180U

#define FS_DIO_PORTA_PCR 0x40049000U
#define FS_DIO_PORTB_PCR 0x4004A000U
#define FS_DIO_PORTC_PCR 0x4004B000U
#define FS_DIO_PORTD_PCR 0x4004C000U
#define FS_DIO_PORTE_PCR 0x4004D000U
#define FS_DIO_PORTF_PCR 0x4004E000U
#define FS_DIO_PORTG_PCR 0x4004F000U

/* K3S */
#define FS_DIO_PORTA_K3S 0x48020000U 
#define FS_DIO_PORTB_K3S 0x48020040U 
#define FS_DIO_PORTC_K3S 0x48020080U 
#define FS_DIO_PORTD_K3S 0x480200C0U 
#define FS_DIO_PORTE_K3S 0x4100F000U 

#define FS_DIO_PORTA_K3S_PCR 0x40046000U
#define FS_DIO_PORTB_K3S_PCR 0x40047000U
#define FS_DIO_PORTC_K3S_PCR 0x40048000U
#define FS_DIO_PORTD_K3S_PCR 0x40049000U
#define FS_DIO_PORTE_K3S_PCR 0x41037000U

/* IMXRT */
#define FS_DIO_GPIO1_IMX 0x401B8000U
#define FS_DIO_GPIO2_IMX 0x401BC000U
#define FS_DIO_GPIO3_IMX 0x401C0000U
#define FS_DIO_GPIO4_IMX 0x401C4000U
#define FS_DIO_GPIO5_IMX 0x400C0000U

/* LPC */
#define FS_IOCON_PIO_2BIT_MASK 0x3U
#define FS_IOCON_PIO_PULL_DOWN 0x1U
#define FS_IOCON_PIO_PULL_UP   0x2U

#ifndef __ASM__

/*******************************************************************************
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

/* IMXRT */
typedef struct
{
    uint32_t mux;  /* SW_MUX control register */
    uint32_t pad;  /* SW_PAD control register  */
    uint32_t gdir; /* GPIO direction register */
    uint32_t dr;   /* GPIO data register */
} fs_dio_backup_rt_t;

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
      uint8_t  pinNum;
      uint8_t  pinDir;
      uint8_t  pinMux;
      fs_dio_backup_t sTestedPinBackup;
} fs_dio_test_t;

/* Safety DIO test item IMXRT */
typedef struct
{
      uint32_t gpio;
      uint32_t muxAddr;
      uint32_t padAddr;
      uint8_t pinNum;
      uint8_t pinDir;
      uint8_t pinMux;
      fs_dio_backup_rt_t sTestedPinBackup;
} fs_dio_test_rt_t;

/* Safety DIO test item LPC */
typedef struct
{
    uint8_t  *pPort_byte;  /*adress of byte register in GPIO*/
    uint32_t *pPort_dir;   /* asress of dir register in GPIO*/
    uint32_t *pPort_Iocon; /* Adress of concrete IOCON register*/
    uint32_t iocon_mode_shift;
    uint8_t  pinNum;
    uint8_t  pinDir;
    uint32_t gpio_clkc_shift;
    fs_dio_backup_lpc_t sTestedPinBackup;
} fs_dio_test_lpc_t;

/******************************************************************************
* Global functions
******************************************************************************/

/*******************************************************************************
*
* brief  FS_DIO_Input
*
* param  *pTestedPin   - pointer to tested pin struct (fs_dio_test_t)
* param  expectedValue - expected value of tested pin
*
* return - FS_DIO_FAIL
*        - FS_DIO_PASS
*                     		
* remarks This function executes the digital input test. The test is focused on the testing of one digital pin. 
*
*******************************************************************************/
FS_RESULT FS_DIO_Input(fs_dio_test_t *pTestedPin, bool_t expectedValue);

/*******************************************************************************
*
* brief  FS_DIO_Output
*
* param  *pTestedPin - pointer to tested pin struct (fs_dio_test_t)
* param  delay       - the delay needed to recognize the value change on tested pin
*
* return - FS_DIO_FAIL        
*        - FS_DIO_PASS
*                     		
* remarks This function executes the digital output test. The test is focused only to test one digital pin. 
*
*******************************************************************************/
FS_RESULT FS_DIO_Output(fs_dio_test_t *pTestedPin, uint32_t delay);

/*******************************************************************************
*
* brief  FS_DIO_Output_RT
*
* param  *pTestedPin - pointer to tested pin struct (fs_dio_test_rt_t)
* param  delay       - the delay needed to recognize the value change on tested pin
*
* return - FS_DIO_FAIL       
*        - FS_DIO_PASS
*                     		
* remarks This function executes the digital output test. The test is focused only to test one digital pin. 
*
*******************************************************************************/
FS_RESULT FS_DIO_Output_RT(fs_dio_test_rt_t *pTestedPin, uint32_t delay);

/*******************************************************************************
*
* brief  FS_DIO_Output_LPC
*
* param  *pTestedPin - pointer to tested pin struct (fs_dio_test_lpc_t)
* param  delay       - the delay needed to recognize the value change on tested pin
*
* return - FS_DIO_FAIL       
*        - FS_DIO_PASS
*                           
* remarks This function executes the digital output test. The test is focused only to test one digital pin. 
*
*******************************************************************************/
FS_RESULT FS_DIO_Output_LPC(fs_dio_test_lpc_t *pTestedPin, uint32_t delay);

#endif /* __ASM__ */

#endif /* _IEC60730B_DIO_ */
