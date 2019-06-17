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
* @file      IEC60730_B_CM4_CM7_dio.h
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

#ifndef _IEC60730_B_CM4_CM7_DIO_H_
#define _IEC60730_B_CM4_CM7_DIO_H_


/* DIO test results constants */
#define IEC60730B_ST_DIO_PASS        (0x0)
#define IEC60730B_ST_DIO_FAIL        (IEC60730B_ST_DIO_FAIL_CODE|0x1)

/******************************************************************************
* Macros for GPIO pins - module address + pcr address
******************************************************************************/

/* Kinetis */
#define IEC60730B_DIO_PORTA 0x400FF000
#define IEC60730B_DIO_PORTB 0x400FF040
#define IEC60730B_DIO_PORTC 0x400FF080
#define IEC60730B_DIO_PORTD 0x400FF0C0
#define IEC60730B_DIO_PORTE 0x400FF100
#define IEC60730B_DIO_PORTF 0x400FF140
#define IEC60730B_DIO_PORTG 0x400FF180

#define IEC60730B_DIO_PORTA_PCR 0x40049000
#define IEC60730B_DIO_PORTB_PCR 0x4004A000
#define IEC60730B_DIO_PORTC_PCR 0x4004B000
#define IEC60730B_DIO_PORTD_PCR 0x4004C000
#define IEC60730B_DIO_PORTE_PCR 0x4004D000
#define IEC60730B_DIO_PORTF_PCR 0x4004E000
#define IEC60730B_DIO_PORTG_PCR 0x4004F000

/* K3S */
#define IEC60730B_DIO_PORTA_K3S 0x48020000 
#define IEC60730B_DIO_PORTB_K3S 0x48020040 
#define IEC60730B_DIO_PORTC_K3S 0x48020080 
#define IEC60730B_DIO_PORTD_K3S 0x480200C0 
#define IEC60730B_DIO_PORTE_K3S 0x4100F000 

#define IEC60730B_DIO_PORTA_K3S_PCR 0x40046000
#define IEC60730B_DIO_PORTB_K3S_PCR 0x40047000
#define IEC60730B_DIO_PORTC_K3S_PCR 0x40048000
#define IEC60730B_DIO_PORTD_K3S_PCR 0x40049000
#define IEC60730B_DIO_PORTE_K3S_PCR 0x41037000

/* IMX_RT */
#define IEC60730B_DIO_GPIO1_IMX 0x401B8000
#define IEC60730B_DIO_GPIO2_IMX 0x401BC000
#define IEC60730B_DIO_GPIO3_IMX 0x401C0000
#define IEC60730B_DIO_GPIO4_IMX 0x401C4000
#define IEC60730B_DIO_GPIO5_IMX 0x400C0000

#ifndef __ASM__

/*******************************************************************************
* Definitions
******************************************************************************/

/* Used to backup original DIO port/pin configuration */
typedef struct
{
    unsigned long pcr;  /* Pin control register */
    unsigned long pddr; /* Port data direction register */
    unsigned long pdor; /* Port data output register */
} IEC60730_DIO_Backup;

typedef struct
{
    unsigned long mux;  /* SW_MUX control register */
    unsigned long pad;  /* SW_PAD control register  */
    unsigned long gdir; /* GPIO direction register */
    unsigned long dr;   /* GPIO data register */
} IEC60730_DIO_Backup_RT;

/* Safety DIO test item */
typedef struct _dio_test
{
      unsigned long  gpio;
      unsigned long  pcr;
      unsigned char  pinNum;
      unsigned char  pinDir;
      unsigned char  pinMux;
      IEC60730_DIO_Backup sTestedPinBackup;
} dio_test_t;

/* Safety DIO test item RT */
typedef struct _dio_test_rt
{
      unsigned long gpio;
      unsigned long muxAddr;
      unsigned long padAddr;
      unsigned char pinNum;
      unsigned char pinDir;
      unsigned char pinMux;
      IEC60730_DIO_Backup_RT sTestedPinBackup;
} dio_test_rt_t;

/******************************************************************************
* Global functions
******************************************************************************/

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_DIO_OutputTest
*
* param  *pTestedPin - pointer to tested pin struct (dio_test_t)
* param  delay       - the delay needed to recognize the value change on tested pin
*
* return - IEC60730B_ST_DIO_FAIL        
*        - IEC60730B_ST_DIO_PASS
*                     		
* remarks This function executes the digital output test. The test is focused only to test one digital pin. 
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_DIO_OutputTest(dio_test_t *pTestedPin, unsigned int delay);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_DIO_OutputTest_RT
*
* param  *pTestedPin - pointer to tested pin struct (dio_test_rt_t)
* param  delay       - the delay needed to recognize the value change on tested pin
*
* return - IEC60730B_ST_DIO_FAIL       
*        - IEC60730B_ST_DIO_PASS
*                     		
* remarks This function executes the digital output test. The test is focused only to test one digital pin. 
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_DIO_OutputTest_RT(dio_test_rt_t *pTestedPin, unsigned int delay);

#endif /* __ASM__ */

#endif /* _IEC60730_B_CM4_CM7_DIO_H_ */
