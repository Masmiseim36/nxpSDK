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
 * @version   4.2
 *
 * @brief     IEC60730 Class B Safety Routines common header file
 *
 ***********************************************************************************************/

#ifndef IEC60730B_TYPES_H_
#define IEC60730B_TYPES_H_

#ifndef __ASM__

#include <stdint.h>

#ifndef __STDINT_DECLS      /* Test stdint.h in  MDK  uVision */
#ifndef __STDINT_H_INCLUDED /* Test stdint.h in  MCUXpresso  */

/* Signed and Unsigned integer data types */
/* 8 bits */
        /*IAR                               */
#if !defined(__INT8_T_TYPE__) && !defined(_INT8_T_DECLARED) && !defined(__int8_t_defined)
  typedef signed char int8_t;
  typedef unsigned char uint8_t;
  #define __int8_t_defined 1
#endif /* __int8_t_defined */

/* 16 bits */
#if !defined(__INT16_T_TYPE__) && !defined(_INT16_T_DECLARED) && !defined(__int16_t_defined) 
  typedef signed short int16_t;
  typedef unsigned short uint16_t;
  #define __int16_t_defined 1
#endif /* __int16_t_defined */

/* 32 bits */
#if !defined(__INT32_T_TYPE__) && !defined(_INT32_T_DECLARED) && !defined(__int32_t_defined) 
  typedef signed long int32_t;
  typedef unsigned long uint32_t;
  #define __int32_t_defined 1
#endif /* __int32_t_defined */

/* 64 bits */
#if !defined(__INT64_T_TYPE__) && !defined(_INT64_T_DECLARED) && !defined(__int64_t_defined) 
  typedef signed long long int64_t;
  typedef unsigned long long uint64_t;
  #define __int64_t_defined 1
#endif /* __int64_t_defined */
  
#endif /* MDK stdint.h included */
#endif /* MCUX stdint.h included */

/* Float data types */
#if !defined(_MATH) && !defined(__math_h) /* IAR and MDK uVision detect */
typedef float float_t;
#endif 
/* Boolean data types */
typedef unsigned short bool_t;


/* Result type */
typedef uint32_t FS_RESULT;

#ifndef FALSE
#define FALSE ((bool_t)0)
#endif
#ifndef TRUE
#define TRUE ((bool_t)1)
#endif



/******************************************************************************
 * Structure for library
 ******************************************************************************/

/**********************
 * Kinetis
 *********************/
/* GPIO - Register Layout Typedef */
typedef struct
{
    volatile uint32_t PDOR;       /* Port Data Output Register, offset: 0x0 */
    volatile uint32_t PSOR;       /* Port Set Output Register, offset: 0x4 */
    volatile uint32_t PCOR;       /* Port Clear Output Register, offset: 0x8 */
    volatile uint32_t PTOR;       /* Port Toggle Output Register, offset: 0xC */
    volatile const uint32_t PDIR; /* Port Data Input Register, offset: 0x10 */
    volatile uint32_t PDDR;       /* Port Data Direction Register, offset: 0x14 */
} fs_gpio_t;

/* LPTMR - Register Layout Typedef */
typedef struct // TODO volatile removed*/
{
    volatile uint32_t CSR; /**< Low Power Timer Control Status Register, offset: 0x0 */
    volatile uint32_t PSR; /**< Low Power Timer Prescale Register, offset: 0x4 */
    volatile uint32_t CMR; /**< Low Power Timer Compare Register, offset: 0x8 */
    volatile uint32_t CNR; /**< Low Power Timer Counter Register, offset: 0xC */
} fs_lptmr_t;

/* LPTMR - Masks */
#define FS_LPTMR_CSR_TEN_MASK (0x1U)

/* PORT - Register Layout Typedef */
typedef struct
{
    volatile uint32_t PCR[32]; /* Pin Control Register (PCR), offset: 0x0 */
} fs_pcr_t;

/* RTC - Register Layout Typedef */
typedef struct
{
    volatile uint32_t SC;        /* RTC Status and Control Register, offset: 0x0 */
    volatile uint32_t MOD;       /* RTC Modulo Register, offset: 0x4 */
    volatile const uint32_t CNT; /* RTC Counter Register, offset: 0x8 */
} fs_rtc_t;

/*******************************************************************************
 * Definitions LPC
 ******************************************************************************/
/* WKT - Peripheral register structure */
typedef struct
{
    volatile uint32_t CTRL; /**< Self wake-up timer control register., offset: 0x0 */
    uint8_t RESERVED_0[8];
    volatile uint32_t COUNT; /**< Counter register., offset: 0xC */
} fs_wkt_t;

/*******************************************************************************
 * Definitions IMX
 ******************************************************************************/
/* GPT - Register Layout Typedef */
typedef struct
{
    volatile uint32_t CR; /* GPT Control Register, offset: 0x0 */
    volatile uint32_t PR; /* GPT Prescaler Register, offset: 0x4 */
    volatile uint32_t SR; /* GPT Status Register, offset: 0x8 */
    volatile uint32_t IR; /* GPT Interrupt Register, offset: 0xC */
    volatile uint32_t
        OCR[3]; /* GPT Output Compare Register 1..GPT Output Compare Register 3, array offset: 0x10, array step: 0x4 */
    volatile const uint32_t
        ICR[2]; /* GPT Input Capture Register 1..GPT Input Capture Register 2, array offset: 0x1C, array step: 0x4 */
    volatile const uint32_t CNT; /* GPT Counter Register, offset: 0x24 */
} fs_gpt_t;

/* GPIO - Register Layout Typedef */
typedef struct
{
    volatile uint32_t DR;        /* GPIO data register, offset: 0x0 */
    volatile uint32_t GDIR;      /* GPIO direction register, offset: 0x4 */
    volatile const uint32_t PSR; /* GPIO pad status register, offset: 0x8 */
    volatile uint32_t ICR1;      /* GPIO interrupt configuration register1, offset: 0xC */
    volatile uint32_t ICR2;      /* GPIO interrupt configuration register2, offset: 0x10 */
    volatile uint32_t IMR;       /* GPIO interrupt mask register, offset: 0x14 */
    volatile uint32_t ISR;       /* GPIO interrupt status register, offset: 0x18 */
    volatile uint32_t EDGE_SEL;  /* GPIO edge select register, offset: 0x1C */
    uint8_t RESERVED_0[100];
    volatile uint32_t DR_SET;    /* GPIO data register SET, offset: 0x84 */
    volatile uint32_t DR_CLEAR;  /* GPIO data register CLEAR, offset: 0x88 */
    volatile uint32_t DR_TOGGLE; /* GPIO data register TOGGLE, offset: 0x8C */
} fs_gpio_imxrt_t;

/**********************
 * IMX8Mx
 *********************/
/* GPIO - Register Layout Typedef */
typedef struct
{
    volatile uint32_t DR;        /* GPIO data register, offset: 0x0 */
    volatile uint32_t GDIR;      /* GPIO direction register, offset: 0x4 */
    volatile const uint32_t PSR; /* GPIO pad status register, offset: 0x8 */
    volatile uint32_t ICR1;      /* GPIO interrupt configuration register1, offset: 0xC */
    volatile uint32_t ICR2;      /* GPIO interrupt configuration register2, offset: 0x10 */
    volatile uint32_t IMR;       /* GPIO interrupt mask register, offset: 0x14 */
    volatile uint32_t ISR;       /* GPIO interrupt status register, offset: 0x18 */
    volatile uint32_t EDGE_SEL;  /* GPIO edge select register, offset: 0x1C */
} fs_gpio_imx8m_t;

/* CTIMER - Register Layout Typedef */
typedef struct
{
    volatile uint32_t IR;  /**< Interrupt Register. The IR can be written to clear interrupts. The IR can be read to
                              identify which of eight possible interrupt sources are pending., offset: 0x0 */
    volatile uint32_t TCR; /**< Timer Control Register. The TCR is used to control the Timer Counter functions. The
                              Timer Counter can be disabled or reset through the TCR., offset: 0x4 */
    volatile uint32_t TC;  /**< Timer Counter. The 32 bit TC is incremented every PR+1 cycles of the APB bus clock. The
                              TC is controlled through the TCR., offset: 0x8 */
    volatile uint32_t PR;  /**< Prescale Register. When the Prescale Counter (PC) is equal to this value, the next clock
                              increments the TC and clears the PC., offset: 0xC */
    volatile uint32_t PC;  /**< Prescale Counter. The 32 bit PC is a counter which is incremented to the value stored in
                              PR. When the value in PR is reached, the TC is incremented and the PC is cleared. The PC is
                              observable and controllable through the bus interface., offset: 0x10 */
    volatile uint32_t MCR; /**< Match Control Register. The MCR is used to control if an interrupt is generated and if
                              the TC is reset when a Match occurs., offset: 0x14 */
    volatile uint32_t
        MR[4]; /**< Match Register . MR can be enabled through the MCR to reset the TC, stop both the TC and PC, and/or
                  generate an interrupt every time MR matches the TC., array offset: 0x18, array step: 0x4 */
    volatile uint32_t CCR; /**< Capture Control Register. The CCR controls which edges of the capture inputs are used to
                              load the Capture Registers and whether or not an interrupt is generated when a capture
                              takes place., offset: 0x28 */
    volatile const uint32_t CR[4]; /**< Capture Register . CR is loaded with the value of TC when there is an event on
                                      the CAPn. input., array offset: 0x2C, array step: 0x4 */
    volatile uint32_t EMR; /**< External Match Register. The EMR controls the match function and the external match
                              pins., offset: 0x3C */
    uint8_t RESERVED_0[48];
    volatile uint32_t CTCR; /**< Count Control Register. The CTCR selects between Timer and Counter mode, and in Counter
                               mode selects the signal and edge(s) for counting., offset: 0x70 */
    volatile uint32_t
        PWMC; /**< PWM Control Register. The PWMCON enables PWM mode for the external match pins., offset: 0x74 */
    volatile uint32_t
        MSR[4]; /**< Match Shadow Register . If enabled, the Match Register will be automatically reloaded with the
                   contents of this register whenever the TC is reset to zero., array offset: 0x78, array step: 0x4 */
} fs_ctimer_t;

/*******************************************************************************
 * Definitions ADC
 ******************************************************************************/
/* ADC */
/* ADC_Register_Accessor_Macros */
typedef struct
{
    volatile uint32_t sci[2]; /* ADC Status and Control Registers 1, array offset: 0x0, array step: 0x4 */
    uint32_t tmp;
    uint32_t tmp2;
    volatile const uint32_t res[2]; /* ADC Data Result Register, array offset: 0x10, array step: 0x4 */
} fs_aio_a23_t;

/* ADC KExx */
/* ADC_Register_Accessor_Macros */
typedef struct
{
    volatile uint32_t sci[2]; /* ADC Status and Control Registers 1, array offset: 0x0, array step: 0x4 */
    uint32_t empty[16];
    volatile const uint32_t res[2]; /* ADC Data Result Register, array offset: 0x48, array step: 0x4 */
} fs_aio_a4_t;

/* ADC K3S */
/* ADC_Register_Accessor_Macros */
typedef struct
{
    uint32_t verid;
    uint32_t param;
    uint8_t reserved_0[8];
    uint32_t ctrl;
    uint32_t stat;
    uint32_t ie;
    uint32_t de;
    uint32_t cfg;
    uint32_t pause;
    uint8_t reserved_1[8];
    uint32_t fctrl;
    uint32_t swtrig;
    uint8_t reserved_2[8];
    uint32_t ofstrim;
    uint8_t reserved_3[124];
    uint32_t tctrl[4];
    uint8_t reserved_4[48];
    struct
    {
        uint32_t cmdl;
        uint32_t cmdh;
    } cmd[15];
    uint8_t reserved_5[136];
    uint32_t cv[4];
    uint8_t reserved_6[240];
    uint32_t resfifo;
} fs_aio_a1_t;

/* ADC Cyclic */
/* ADC_Register_Accessor_Macros */
typedef struct
{
    volatile uint16_t ctrl[2];    /* ADC Control Registers 1 and 2, array offset: 0x0, array step: 0x02 */
    volatile uint16_t zxctrl[2];  /* ADC Zero Crossing Control 1 and 2, array offset 0x4, array step 0x02 */
    volatile uint16_t clist[4];   /* ADC Channel List Register 1 - 4, array offset 0x8, array step 0x02 */
    volatile uint16_t sdis;       /* ADC Sample Disable Register, offset 0x10 */
    volatile uint16_t stat;       /* ADC Status Register, offset 0x12 */
    volatile const uint16_t rdy;  /* ADC Ready Register, offset 0x14 */
    volatile uint16_t limstat[2]; /* ADC Low Limit and High Status Register, array offset 0x16, array step 0x02 */
    volatile uint16_t zxstat;     /* ADC Zero Crossing Status Register, offset 0x1A */
    volatile uint16_t
        rslt[16]; /* ADC Result Registers with sign extension 0 - 15, array offset 0x1C, array step 0x02 */
    /*GAP to offset 0xAA*/
} fs_aio_a7_t;

/* ADC LPC */
/* ADC_Register_Accessor_Macros */
typedef struct
{
    volatile uint32_t CTRL; /**< ADC Control register. Contains the clock divide value, resolution selection, sampling
                               time selection, and mode controls., offset: 0x0 */
    uint8_t RESERVED_0[4];
    volatile uint32_t SEQ_CTRL[2]; /**< ADC Conversion Sequence-n control register: Controls triggering and channel
                                      selection for conversion sequence-n. Also specifies interrupt mode for
                                      sequence-n., array offset: 0x8, array step: 0x4 */
    volatile const uint32_t
        SEQ_GDAT[2]; /**< ADC Sequence-n Global Data register. This register contains the result of the most recent ADC
                        conversion performed under sequence-n., array offset: 0x10, array step: 0x4 */
    uint8_t RESERVED_1[8];
    volatile const uint32_t
        DAT[12]; /**< ADC Channel N Data register. This register contains the result of the most recent conversion
                    completed on channel N., array offset: 0x20, array step: 0x4 */
    volatile uint32_t
        THR0_LOW; /**< ADC Low Compare Threshold register 0: Contains the lower threshold level for automatic threshold
                     comparison for any channels linked to threshold pair 0., offset: 0x50 */
    volatile uint32_t
        THR1_LOW; /**< ADC Low Compare Threshold register 1: Contains the lower threshold level for automatic threshold
                     comparison for any channels linked to threshold pair 1., offset: 0x54 */
    volatile uint32_t
        THR0_HIGH; /**< ADC High Compare Threshold register 0: Contains the upper threshold level for automatic
                      threshold comparison for any channels linked to threshold pair 0., offset: 0x58 */
    volatile uint32_t
        THR1_HIGH; /**< ADC High Compare Threshold register 1: Contains the upper threshold level for automatic
                      threshold comparison for any channels linked to threshold pair 1., offset: 0x5C */
    volatile uint32_t CHAN_THRSEL; /**< ADC Channel-Threshold Select register. Specifies which set of threshold compare
                                      registers are to be used for each channel, offset: 0x60 */
    volatile uint32_t
        INTEN; /**< ADC Interrupt Enable register. This register contains enable bits that enable the sequence-A,
                  sequence-B, threshold compare and data overrun interrupts to be generated., offset: 0x64 */
    volatile uint32_t FLAGS; /**< ADC Flags register. Contains the four interrupt/DMA trigger flags and the individual
                                component overrun and threshold-compare flags. (The overrun bits replicate information
                                stored in the result registers)., offset: 0x68 */
    volatile uint32_t TRM;   /**< ADC Startup register., offset: 0x6C */
} fs_aio_a5_t;


/* ADC IMXRT10xx */
/* ADC_Register_Accessor_Macros */
typedef struct
{
    volatile uint32_t HC[8];      /**< Control register for hardware triggers, array offset: 0x0, array step: 0x4 */
    volatile const uint32_t HS;   /**< Status register for HW triggers, offset: 0x20 */
    volatile const uint32_t R[8]; /**< Data result register for HW triggers, array offset: 0x24, array step: 0x4 */
    volatile uint32_t CFG;        /**< Configuration register, offset: 0x44 */
    volatile uint32_t GC;         /**< General control register, offset: 0x48 */
    volatile uint32_t GS;         /**< General status register, offset: 0x4C */
    volatile uint32_t CV;         /**< Compare value register, offset: 0x50 */
    volatile uint32_t OFS;        /**< Offset correction value register, offset: 0x54 */
    volatile uint32_t CAL;        /**< Calibration value register, offset: 0x58 */
} fs_aio_a6_t;


/*******************************************************************************
 * Definitions Invariable memory
 ******************************************************************************/

/* WAS (Write As Seed) bit mask used in HW CRC module */
#define FS_FLASH_CRC_CTRL_WAS_MASK 0x2000000

/** CRC - Register Layout Typedef */
typedef struct {
  union {                                          /* offset: 0x0 */
    struct {                                         /* offset: 0x0 */
      volatile uint8_t DATALL;                             /**< CRC_DATALL register, offset: 0x0 */
      volatile uint8_t DATALU;                             /**< CRC_DATALU register, offset: 0x1 */
      volatile uint8_t DATAHL;                             /**< CRC_DATAHL register, offset: 0x2 */
      volatile uint8_t DATAHU;                             /**< CRC_DATAHU register, offset: 0x3 */
    } ACCESS8BIT;
    struct {                                         /* offset: 0x0 */
      volatile uint16_t DATAL;                             /**< CRC_DATAL register, offset: 0x0 */
      volatile uint16_t DATAH;                             /**< CRC_DATAH register, offset: 0x2 */
    } ACCESS16BIT;
    volatile uint32_t DATA;                              /**< CRC DATA register, offset: 0x0 */
  };
  union {                                          /* offset: 0x4 */
    struct {                                         /* offset: 0x4 */
      volatile uint8_t GPOLYLL;                            /**< CRC_GPOLYLL register, offset: 0x4 */
      volatile uint8_t GPOLYLU;                            /**< CRC_GPOLYLU register, offset: 0x5 */
      volatile uint8_t GPOLYHL;                            /**< CRC_GPOLYHL register, offset: 0x6 */
      volatile uint8_t GPOLYHU;                            /**< CRC_GPOLYHU register, offset: 0x7 */
    } GPOLY_ACCESS8BIT;
    struct {                                         /* offset: 0x4 */
      volatile uint16_t GPOLYL;                            /**< CRC_GPOLYL register, offset: 0x4 */
      volatile uint16_t GPOLYH;                            /**< CRC_GPOLYH register, offset: 0x6 */
    } GPOLY_ACCESS16BIT;
    volatile uint32_t GPOLY;                             /**< CRC Polynomial register, offset: 0x4 */
  };
  union {                                          /* offset: 0x8 */
    struct {                                         /* offset: 0x8 */
           uint8_t RESERVED_0[3];
      volatile uint8_t CTRLHU;                             /**< CRC_CTRLHU register, offset: 0xB */
    } CTRL_ACCESS8BIT;
    volatile uint32_t CTRL;                              /**< CRC Control register, offset: 0x8 */
  };
} FS_CRC_Type;

/** CRC - Register Layout Typedef */
typedef struct {
  volatile uint32_t MODE;                              /**< CRC mode register, offset: 0x0 */
  volatile uint32_t SEED;                              /**< CRC seed register, offset: 0x4 */
   union {                                          /* offset: 0x8 */
    struct {                                         
      volatile uint8_t DATALL;                             /**< CRC_DATALL register, offset: 0x0 */
      volatile uint8_t DATALU;                             /**< CRC_DATALU register, offset: 0x1 */
      volatile uint8_t DATAHL;                             /**< CRC_DATAHL register, offset: 0x2 */
      volatile uint8_t DATAHU;                             /**< CRC_DATAHU register, offset: 0x3 */
    } ACCESS8BIT;
    struct {                                         
      volatile uint16_t DATAL;                             /**< CRC_DATAL register, offset: 0x0 */
      volatile uint16_t DATAH;                             /**< CRC_DATAH register, offset: 0x2 */
    } ACCESS16BIT;
    volatile uint32_t DATA;                              /**< CRC DATA register, offset: 0x8 */ 
    volatile const  uint32_t SUM;                        /**< CRC checksum register, offset: 0x8 */ 
  };
     
} FS_CRC_L_Type;

#endif /* __ASM__ */

/******************************************************************************
 * Error codes for tests
 ******************************************************************************/
#define FS_PASS         (0x0000U)
#define FS_CPU_CODE     (0x0100U)
#define FS_PC_CODE      (0x0200U)
#define FS_FLASH_CODE   (0x0300U)
#define FS_FLASH_C_CODE (0x0310U)
#define FS_RAM_CODE     (0x0400U)
#define FS_STACK_CODE   (0x0500U)
#define FS_CLK_CODE     (0x0600U)
#define FS_AIO_CODE     (0x0700U)
#define FS_DIO_CODE     (0x0800U)
#define FS_TSI_CODE     (0x0900U)
#define FS_WDOG_CODE    (0x0A00U)

#endif /* IEC60730B_TYPES_H_ */
