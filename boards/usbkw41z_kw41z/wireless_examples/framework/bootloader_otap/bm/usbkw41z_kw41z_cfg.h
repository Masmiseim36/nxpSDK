/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file 
*
* This file contains configuration options for the usbkw41z board.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/**************************************************/
/** USER SETTINGS OF KINETIS MCU */
/**  Kinetis ARM Cortex-M0 model */
#ifndef _USBKW41Z_CFG_H
#define _USBKW41Z_CFG_H


#define KINETIS_MODEL KW41_32MHz

/*  in the case of using USB VIRTUAL SERIAL LINK you must activate No break TRIM CHECKBOX in the master AN2295 PC Application  */
/*  the break impulse is replaced by using only 9 bits zero impulse  */
//  BREAK IMPULSE       |START| |0| |0| |0| |0| |0| |0| |0| |0| |0| |0| STOP|
//  ZERO IMPULSE        |START| |0| |0| |0| |0| |0| |0| |0| |0| |0| |STOP|
#define BOOTLOADER_SHORT_TRIM  1

/** Kinetis RAM */
#define KINETIS_RAM_START (0x1FFF8000)
#define KINETIS_RAM_END   (0x20017FFF)

/* Disable NMI pin function */
#define gFlashConfigOptions_d  0xFFFFFBFE

/**************************************************/
/* Actual used UART module */
/**************************************************/
#define USE_LPUART 1
//#define BOOT_UART_CLOCK        (32768 * 640)
#define BOOT_UART_CLOCK        (32000000)
/* Actual used UART module */
/* A range of UART baudrates is (9600 - 115200) */
//#define BOOT_UART_BAUD_RATE  9600 
//#define BOOT_UART_BAUD_RATE  14400
//#define BOOT_UART_BAUD_RATE  19200 
//#define BOOT_UART_BAUD_RATE  38400 
//#define BOOT_UART_BAUD_RATE  57600 
#define BOOT_UART_BAUD_RATE  115200
//#define BOOT_UART_BAUD_RATE  76800 
//#define BOOT_UART_BAUD_RATE  115200 
//#define BOOT_UART_BAUD_RATE  128000 


#define BOOT_UART_IRQ          LPUART0_IRQn
#define BOOT_UART_BASE         LPUART0
#define BOOT_UART_SIM_SCG_REG  SIM->SCGC5
#define BOOT_UART_SIM_SCG_MASK SIM_SCGC5_LPUART0_MASK
/** GPIO & UART pins initialization */
#define BOOT_UART_GPIO_PORT_RX   PORTC
#define BOOT_UART_GPIO_PORT_TX   PORTC
#define BOOT_UART_GPIO_PORT_SIM_SCG_REG  SIM->SCGC5
#define BOOT_UART_GPIO_PORT_SIM_SCG_MASK SIM_SCGC5_PORTC_MASK
/*  setting of multiplexer for UART alternative of pin */
#define BOOT_PIN_UART_ALTERNATIVE 4
#define BOOT_UART_GPIO_PIN_RX   6  
#define BOOT_UART_GPIO_PIN_TX   7

/*  setting of multiplexer for GPIO alternative of pin */
#define BOOT_PIN_GPIO_ALTERNATIVE 1

/**************************************************/
/* SPI */
/**************************************************/
#define USE_DSPI 1

/* SPI Master */
#define BOOT_SPI_BasePtr           SPI0
#define BOOT_SPI_PORT_SIM_SCG      SIM->SCGC5
#define cSPI_PORT_SIM_SCG_Config_c SIM_SCGC5_PORTC_MASK
#define BOOT_SPI_SIM_SCG           SIM->SCGC6
#define cSPI_SIM_SCG_Config_c      SIM_SCGC6_SPI0_MASK

#define BOOT_SPI_ALTERNATIVE       2
#define BOOT_SPI_SCLK_PCR          PORTC->PCR[16]
#define BOOT_SPI_MOSI_PCR          PORTC->PCR[17]
#define BOOT_SPI_MISO_PCR          PORTC->PCR[18]
#define BOOT_SPI_SSEL_PCR          PORTC->PCR[19]

#define BOOT_SPI_SSEL_PCOR         GPIOC->PCOR
#define BOOT_SPI_SSEL_PSOR         GPIOC->PSOR
#define BOOT_SPI_SSEL_PDDR         GPIOC->PDDR
#define BOOT_SPI_SSEL_PIN_Number_c 19

/* SPI Slave */
#define BOOT_SPI_Slave_IRQ               SPI1_IRQn
#define BOOT_SPI_Slave_BasePtr           SPI1
#define BOOT_SPI_Slave_PORT_SIM_SCG      SIM->SCGC5
#define cSPI_Slave_PORT_SIM_SCG_Config_c SIM_SCGC5_PORTA_MASK
#define BOOT_SPI_Slave_SIM_SCG           SIM->SCGC6
#define cSPI_Slave_SIM_SCG_Config_c      SIM_SCGC6_SPI1_MASK

#define BOOT_SPI_Slave_SCLK_PCR          PORTA->PCR[16]
#define BOOT_SPI_Slave_MOSI_PCR          PORTA->PCR[17]
#define BOOT_SPI_Slave_MISO_PCR          PORTA->PCR[18]
#define BOOT_SPI_Slave_SSEL_PCR          PORTA->PCR[19]
#define BOOT_SPI_Slave_ALTERNATIVE       2

#define BOOT_SPI_Slave_DAP_SCG_REG       SIM->SCGC5
#define BOOT_SPI_Slave_DAP_SCG_MASK      SIM_SCGC5_PORTB_MASK
#define BOOT_SPI_Slave_DAP_PCR           PORTB->PCR[1]
#define BOOT_SPI_Slave_DAP_PDDR          GPIOB->PDDR
#define BOOT_SPI_Slave_DAP_PSOR          GPIOB->PSOR
#define BOOT_SPI_Slave_DAP_PCOR          GPIOB->PCOR
#define BOOT_SPI_Slave_DAP_pin           1

/**************************************************/
/* Actual PIN used to trigger the serial Boot */
/**************************************************/
#ifndef BOOT_PIN_ENABLE_SIM_SCG_REG
#define BOOT_PIN_ENABLE_SIM_SCG_REG  SIM->SCGC5
#endif

#ifndef BOOT_PIN_ENABLE_SIM_SCG_MASK
#define BOOT_PIN_ENABLE_SIM_SCG_MASK SIM_SCGC5_PORTB_MASK
#endif

#ifndef BOOT_PIN_ENABLE_PORT_BASE
#define BOOT_PIN_ENABLE_PORT_BASE    PORTB
#endif

#ifndef BOOT_PIN_ENABLE_GPIO_BASE
#define BOOT_PIN_ENABLE_GPIO_BASE    GPIOB
#endif

#ifndef BOOT_PIN_ENABLE_NUM
#define BOOT_PIN_ENABLE_NUM          2
#endif

/**************************************************/
/* NMI Pin settings                               */
/**************************************************/
#define NMI_PIN_SIM_SCG_REG   SIM->SCGC5
#define NMI_PIN_SIM_SCG_MASK  SIM_SCGC5_PORTB_MASK
#define NMI_PIN_PORT_BASE     PORTB
#define NMI_PIN_NUM           18
/* Unused Pin ALT set for disabling the NMI PIN */
#define NMI_PIN_ALT           2

/**************************************************/
/* CRC module clock gating info */
/**************************************************/
#define BOOT_CRC_SIM_SCG_REG  SIM->SCGC6
#define BOOT_CRC_SIM_SCG_MASK SIM_SCGC6_CRC_MASK 

/**************************************************/
/** BOOTLOADER FEATURES SETTINGS */
/**************************************************/
#define BOOTLOADER_ENABLE_READ_CMD  1

#define BOOTLOADER_INT_WATCHDOG     0

#define BOOTLOADER_ENABLE_VERIFY    1

#define BOOTLOADER_CRC_ENABLE       1

#define BOOTLOADER_AUTO_TRIMMING    1 

#define BOOTLOADER_PIN_ENABLE       0
/**************************************************/
/** CALIBRATION OF BOOTLOADER TRIM SETTINGS */
#define BOOT_CALIBRATION_GPIO_BASE  PTC_BASE_PTR
/******************************************************************************
*
*
*     Advanced detail settings for non standard settings
*
*
******************************************************************************/

#define VECTOR_TABLE_SIZE (48)

/* Flash block count of this MCU */
#define FLASH_BLOCK_CNT 2

/* Start address of interrupt vector table */ 
#define INTERRUPT_VECTORS 0x0000

/* Start address of relocated interrutp vector table */
#define RELOCATED_VECTORS 0x4000 

/* Flash start address */
#define USER_FLASH_START RELOCATED_VECTORS

/* Flash end address */
//#define USER_FLASH_END 0x0003FFFF

/* Flash2 start address */
//#define USER_FLASH_START_2 0x00040000

/* Flash2 end address */
//#define USER_FLASH_END_2 0x0005FFFF

/* Size of write block */
#define FLASH_WRITE_PAGE 128

/* Size of erase block */
#define FLASH_ERASE_PAGE 2048

/* Maximal length of ID_STRING */
//#define ID_STRING_MAX 5

/* Description string */
#define KINETIS_MODEL_STR "KW41"
#define FLASH_INIT_FLASH_CLOCK { SIM->SCGC6 |= SIM_SCGC6_FTF_MASK; }
#define FLASH_BASE_PTR                FTFA
#define FLASH_FSTAT                   FTFA->FSTAT                                  
#define FLASH_FSTAT_CCIF_MASK         FTFA_FSTAT_CCIF_MASK
#define FLASH_FSTAT_ACCERR_MASK       FTFA_FSTAT_ACCERR_MASK
#define FLASH_FSTAT_FPVIOL_MASK       FTFA_FSTAT_FPVIOL_MASK
#define FLASH_FSTAT_RDCOLERR_MASK     FTFA_FSTAT_RDCOLERR_MASK
#define FLASH_FSTAT_MGSTAT0_MASK      FTFA_FSTAT_MGSTAT0_MASK 

#endif//_USBKW41Z_CFG_H
