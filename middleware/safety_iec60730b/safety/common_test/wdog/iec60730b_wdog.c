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
 * @brief     IEC60730 Class B Safety WATCHDOG test
 *
 ***********************************************************************************************/

#include "iec60730b.h"
#include "asm_mac_common.h"

/*******************************/
/*          UNIVERSAL          */
/*******************************/
/* LPTMR - Register Layout Typedef */
volatile typedef struct
{
  uint32_t CSR;  /**< Low Power Timer Control Status Register, offset: 0x0 */
  uint32_t PSR;  /**< Low Power Timer Prescale Register, offset: 0x4 */
  uint32_t CMR;  /**< Low Power Timer Compare Register, offset: 0x8 */
  uint32_t CNR;  /**< Low Power Timer Counter Register, offset: 0xC */
} FS_LPTMR_Type;

/* LPTMR - Base Adresses */
#define FS_LPTMR0_BASE  (0x40040000u)
#define FS_LPTMR0       ((FS_LPTMR_Type *)FS_LPTMR0_BASE)

/* LPTMR - Masks */
#define FS_LPTMR_CSR_TEN_MASK  (0x1U)

/* WDOG - Register Layout Typedef */
typedef struct
{
   uint16_t STCTRLH;  /* Watchdog Status and Control Register High, offset: 0x0 */
   uint16_t STCTRLL;  /* Watchdog Status and Control Register Low, offset: 0x2 */
   uint16_t TOVALH;   /* Watchdog Time-out Value Register High, offset: 0x4 */
   uint16_t TOVALL;   /* Watchdog Time-out Value Register Low, offset: 0x6 */
   uint16_t WINH;     /* Watchdog Window Register High, offset: 0x8 */
   uint16_t WINL;     /* Watchdog Window Register Low, offset: 0xA */
   uint16_t REFRESH;  /* Watchdog Refresh register, offset: 0xC */
   uint16_t UNLOCK;   /* Watchdog Unlock register, offset: 0xE */
   uint16_t TMROUTH;  /* Watchdog Timer Output Register High, offset: 0x10 */
   uint16_t TMROUTL;  /* Watchdog Timer Output Register Low, offset: 0x12 */
   uint16_t RSTCNT;   /* Watchdog Reset Count register, offset: 0x14 */
   uint16_t PRESC;    /* Watchdog Prescaler register, offset: 0x16 */
} FS_WDOG_Type;

/* WDOG - Base Adresses */
#define FS_WDOG_BASE  (0x40052000u)
#define FS_WDOG       ((FS_WDOG_Type *)FS_WDOG_BASE)

/* RCM - Register Layout Typedef */
typedef struct
{
  uint8_t SRS0;           /**< System Reset Status Register 0, offset: 0x0 */
  uint8_t SRS1;           /**< System Reset Status Register 1, offset: 0x1 */
  uint8_t RESERVED_0[2];
  uint8_t RPFC;           /**< Reset Pin Filter Control register, offset: 0x4 */
  uint8_t RPFW;           /**< Reset Pin Filter Width register, offset: 0x5 */
} FS_RCM_Type;

/* RCM - Base Adresses */
#define FS_RCM_BASE  (0x4007F000u)
#define FS_RCM       ((FS_RCM_Type *)FS_RCM_BASE)

/* RCM - Masks */
#define RCM_SRS0_WDOG_MASK  (0x20U)

/***************************/
/*          KE0xZ          */
/***************************/
/* WDOG - Register Layout Typedef */
typedef struct
{
   uint8_t CS1;         /* Watchdog Control and Status Register 1, offset: 0x0 */
   uint8_t CS2;         /* Watchdog Control and Status Register 2, offset: 0x1 */
   union
   {                    /* offset: 0x2 */
     uint16_t CNT;      /* WDOG_CNT register., offset: 0x2 */
     struct
     {                  /* offset: 0x2 */
      uint8_t CNTH;     /* Watchdog Counter Register: High, offset: 0x2 */
      uint8_t CNTL;     /* Watchdog Counter Register: Low, offset: 0x3 */
     }
     CNT8B;
   };
  union
  {                     /* offset: 0x4 */
    uint16_t TOVAL;             /* WDOG_TOVAL register., offset: 0x4 */
    struct
    {                    /* offset: 0x4 */
      uint8_t TOVALH;    /* Watchdog Timeout Value Register: High, offset: 0x4 */
      uint8_t TOVALL;    /* Watchdog Timeout Value Register: Low, offset: 0x5 */
    } TOVAL8B;
  };
  union
  {                     /* offset: 0x6 */
    uint16_t WIN;               /* WDOG_WIN register., offset: 0x6 */
    struct
    {                    /* offset: 0x6 */
     uint8_t WINH;     /* Watchdog Window Register: High, offset: 0x6 */
     uint8_t WINL;     /* Watchdog Window Register: Low, offset: 0x7 */
    } WIN8B;
  };
} FS_WDOG_KE0XZ_Type;

/* WDOG - Base Adresses */
#define FS_WDOG_KE0XZ_BASE  (0x40052000u)
#define FS_WDOG_KE0XZ       ((FS_WDOG_KE0XZ_Type *)FS_WDOG_KE0XZ_BASE)

/* SIM - Register Layout Typedef */
typedef struct
{
  uint32_t SRSID;   /**< System Reset Status and ID Register, offset: 0x0 */
  uint32_t SOPT;    /**< System Options Register, offset: 0x4 */
  uint32_t PINSEL;  /**< Pin Selection Register, offset: 0x8 */
  uint32_t SCGC;    /**< System Clock Gating Control Register, offset: 0xC */
  uint32_t UUIDL;   /**< Universally Unique Identifier Low Register, offset: 0x10 */
  uint32_t UUIDH;   /**< Universally Unique Identifier High Register, offset: 0x14 */
  uint32_t BUSDIV;  /**< BUS Clock Divider Register, offset: 0x18 */
} FS_SIM_KE0XZ_Type;

/* SIM - Base Adresses */
#define FS_SIM_KE0XZ_BASE  (0x40048000u)
#define FS_SIM_KE0XZ       ((FS_SIM_KE0XZ_Type *) FS_SIM_KE0XZ_BASE)

/* SIM - Masks */
#define FS_SIM_KE0XZ_SRSID_WDOG_MASK  0x20u

/* RTC - Register Layout Typedef */
volatile typedef struct
{
  uint32_t SC;   /* RTC Status and Control Register, offset: 0x0 */
  uint32_t MOD;  /* RTC Modulo Register, offset: 0x4 */
  uint32_t CNT;  /* RTC Counter Register, offset: 0x8 */
} FS_RTC_KE0XZ_Type;

/* RTC - Base Adresses */
#define FS_RTC_KE0XZ_BASE  (0x4003D000u)
#define FS_RTC_KE0XZ       ((FS_RTC_KE0XZ_Type *)FS_RTC_KE0XZ_BASE)

/**************************/
/*          KL2x          */
/**************************/
/* SIM COP - Register Layout Typedef */
volatile typedef struct
{
  uint32_t COPC;    /**< COP Control Register, offset: 0x1100 */
  uint32_t SRVCOP;  /**< Service COP, offset: 0x1104 */
} FS_SIM_KL2X_Type;

/* SIM COP - Base Adresses */
#define FS_SIM_KL2X_BASE  (0x40048100u) /* Offset 0x1100 */
#define FS_SIM_KL2X       ((FS_SIM_KL2X_Type *)FS_SIM_KL2X_BASE)

/* SIM COP - Masks */
#define FS_SIM_KL2X_SRVCOP_SRVCOP_MASK  (0xFFU)
#define FS_SIM_KL2X_SRVCOP_SRVCOP_SHIFT (0U)
#define FS_SIM_KL2X_SRVCOP_SRVCOP(x)    (((uint32_t)(((uint32_t)(x)) << FS_SIM_KL2X_SRVCOP_SRVCOP_SHIFT)) & FS_SIM_KL2X_SRVCOP_SRVCOP_MASK)

/***************************/
/*          KE1xZ          */
/***************************/
/* WDOG - Register Layout Typedef */
typedef struct
{
  uint32_t CS;     /**< Watchdog Control and Status Register, offset: 0x0 */
  uint32_t CNT;    /**< Watchdog Counter Register, offset: 0x4 */
  uint32_t TOVAL;  /**< Watchdog Timeout Value Register, offset: 0x8 */
  uint32_t WIN;    /**< Watchdog Window Register, offset: 0xC */
} FS_WDOG_KE1XZ_Type;

/* WDOG - Base Adresses */
#define FS_WDOG_KE1XZ_BASE  (0x40052000u)
#define FS_WDOG_KE1XZ       ((FS_WDOG_KE1XZ_Type *)FS_WDOG_KE1XZ_BASE)

/* RCM - Register Layout Typedef */
volatile typedef struct
{
  uint32_t VERID;  /**< Version ID Register, offset: 0x0 */
  uint32_t PARAM;  /**< Parameter Register, offset: 0x4 */
  uint32_t SRS;    /**< System Reset Status Register, offset: 0x8 */
  uint32_t RPC;    /**< Reset Pin Control register, offset: 0xC */
  uint32_t MR;     /**< Mode Register, offset: 0x10 */
  uint32_t FM;     /**< Force Mode Register, offset: 0x14 */
  uint32_t SSRS;   /**< Sticky System Reset Status Register, offset: 0x18 */
  uint32_t SRIE;   /**< System Reset Interrupt Enable Register, offset: 0x1C */
} FS_RCM_KE1XZ_Type;

/* RCM - Base Adresses */
#define FS_RCM_KE1XZ_BASE  (0x4007F000u)
#define FS_KE1XZ_RCM       ((FS_RCM_KE1XZ_Type *)FS_RCM_KE1XZ_BASE)

/* RCM - Masks */
#define FS_RCM_KE1XZ_SRS_WDOG_MASK  (0x20U)

/***************************/
/*          KE1xF          */
/***************************/
/* WDOG - Register Layout Typedef */
typedef struct
{
  uint32_t CS;     /**< Watchdog Control and Status Register, offset: 0x0 */
  uint32_t CNT;    /**< Watchdog Counter Register, offset: 0x4 */
  uint32_t TOVAL;  /**< Watchdog Timeout Value Register, offset: 0x8 */
  uint32_t WIN;    /**< Watchdog Window Register, offset: 0xC */
} FS_WDOG_KE1XF_Type;

/* WDOG - Base Adresses */
#define FS_WDOG_KE1XF_BASE  (0x40052000u)
#define FS_WDOG_KE1XF       ((FS_WDOG_KE1XF_Type *)FS_WDOG_KE1XF_BASE)

/* RCM - Register Layout Typedef */
volatile typedef struct
{
  uint32_t VERID;  /**< Version ID Register, offset: 0x0 */
  uint32_t PARAM;  /**< Parameter Register, offset: 0x4 */
  uint32_t SRS;    /**< System Reset Status Register, offset: 0x8 */
  uint32_t RPC;    /**< Reset Pin Control register, offset: 0xC */
  uint32_t MR;     /**< Mode Register, offset: 0x10 */
  uint32_t FM;     /**< Force Mode Register, offset: 0x14 */
  uint32_t SSRS;   /**< Sticky System Reset Status Register, offset: 0x18 */
  uint32_t SRIE;   /**< System Reset Interrupt Enable Register, offset: 0x1C */
} FS_RCM_KE1XF_Type;

/* RCM - Base Adresses */
#define FS_RCM_KE1XF_BASE  (0x4007F000u)
#define FS_RCM_KE1XF       ((FS_RCM_KE1XF_Type *) FS_RCM_KE1XF_BASE)

/* RCM - Masks */
#define FS_RCM_KE1X_SRS_WDOG_MASK  (0x20U)

/**************************/
/*          K32W          */
/**************************/
/* WDOG - Register Layout Typedef */
typedef struct
{
  uint32_t CS;     /**< Watchdog Control and Status Register, offset: 0x0 */
  uint32_t CNT;    /**< Watchdog Counter Register, offset: 0x4 */
  uint32_t TOVAL;  /**< Watchdog Timeout Value Register, offset: 0x8 */
  uint32_t WIN;    /**< Watchdog Window Register, offset: 0xC */
} FS_WDOG_K32W_Type;

/* SMC - Register Layout Typedef */
volatile typedef struct
{
  uint32_t VERID;           /**< Version ID Register, offset: 0x0 */
  uint32_t PARAM;           /**< Parameter Register, offset: 0x4 */
  uint32_t PMPROT;          /**< Power Mode Protection register, offset: 0x8 */
  uint8_t RESERVED_0[4];
  uint32_t PMCTRL;          /**< Power Mode Control register, offset: 0x10 */
  uint8_t RESERVED_1[4];
  uint32_t PMSTAT;          /**< Power Mode Status register, offset: 0x18 */
  uint8_t RESERVED_2[4];
  uint32_t SRS;             /**< System Reset Status, offset: 0x20 */
  uint32_t RPC;             /**< Reset Pin Control, offset: 0x24 */
  uint32_t SSRS;            /**< Sticky System Reset Status, offset: 0x28 */
  uint32_t SRIE;            /**< System Reset Interrupt Enable, offset: 0x2C */
  uint32_t SRIF;            /**< System Reset Interrupt Flag, offset: 0x30 */
  uint8_t RESERVED_3[12];
  uint32_t MR;              /**< Mode Register, offset: 0x40 */
  uint8_t RESERVED_4[12];
  uint32_t FM;              /**< Force Mode Register, offset: 0x50 */
  uint8_t RESERVED_5[12];
  uint32_t SRAMLPR;         /**< SRAM Low Power Register, offset: 0x60 */
  uint32_t SRAMDSR;         /**< SRAM Deep Sleep Register, offset: 0x64 */
} FS_SMC_Type; 

/* SMC - Base Adresses */
#define FS_SMC0_BASE  (0x40020000)
#define FS_SMC0       ((FS_SMC_Type *)FS_SMC0_BASE) 
#define FS_SMC1_BASE  (0x41020000)
#define FS_SMC1       ((FS_SMC_Type *)FS_SMC1_BASE)

/* SMC - Masks */
#define FS_SMC_SRS_WDOG_MASK  0x2000U

/***************************/
/*          IMXRT          */
/***************************/
/* GPT - Register Layout Typedef */
typedef struct
{
  uint32_t CR;        /* GPT Control Register, offset: 0x0 */
  uint32_t PR;        /* GPT Prescaler Register, offset: 0x4 */
  uint32_t SR;        /* GPT Status Register, offset: 0x8 */
  uint32_t IR;        /* GPT Interrupt Register, offset: 0xC */
  uint32_t OCR[3];    /* GPT Output Compare Register 1..GPT Output Compare Register 3, array offset: 0x10, array step: 0x4 */
  uint32_t ICR[2];    /* GPT Input Capture Register 1..GPT Input Capture Register 2, array offset: 0x1C, array step: 0x4 */
  uint32_t CNT;       /* GPT Counter Register, offset: 0x24 */
} FS_GPT_Type;

/* RTWDOG - Register Layout Typedef */
typedef struct  
{
  uint32_t CS;    /* Watchdog Control and Status Register, offset: 0x0 */
  uint32_t CNT;   /* Watchdog Counter Register, offset: 0x4 */
  uint32_t TOVAL; /* Watchdog Timeout Value Register, offset: 0x8 */
  uint32_t WIN;   /* Watchdog Window Register, offset: 0xC */
} FS_RTWDOG_Type;

/* RTWDOG - Base Adresses */
#define FS_RTWDOG_BASE  (0x400BC000u)
#define FS_RTWDOG       ((FS_RTWDOG_Type *)FS_RTWDOG_BASE)

/* SRC - Register Layout Typedef */
volatile typedef struct
{
  uint32_t SCR;              /**< SRC Control Register, offset: 0x0 */
  uint32_t SBMR1;            /**< SRC Boot Mode Register 1, offset: 0x4 */
  uint32_t SRSR;             /**< SRC Reset Status Register, offset: 0x8 */
  uint8_t  RESERVED_0[16];
  uint32_t SBMR2;            /**< SRC Boot Mode Register 2, offset: 0x1C */
  uint32_t GPR[10];          /**< SRC General Purpose Register 1..SRC General Purpose Register 10, array offset: 0x20, array step: 0x4 */
} FS_SRC_Type;

/* SRC - Base Adresses */
#define FS_SRC_BASE  (0x400F8000u)
#define FS_SRC       ((FS_SRC_Type *)FS_SRC_BASE)

/* SRC - Masks */
#define FS_SRC_SRSR_WDOG3_RST_B_MSK  (0x80U)

/* WWDT - Register Layout Typedef */
typedef struct
{
  uint32_t MOD;           /**< Watchdog mode register. This register contains the basic mode and status of the Watchdog Timer., offset: 0x0 */
  uint32_t TC;            /**< Watchdog timer constant register. This 24-bit register determines the time-out value., offset: 0x4 */
  uint32_t FEED;          /**< Watchdog feed sequence register. Writing 0xAA followed by 0x55 to this register reloads the Watchdog timer with the value contained in TC., offset: 0x8 */
  uint32_t TV;            /**< Watchdog timer value register. This 24-bit register reads out the current value of the Watchdog timer., offset: 0xC */
  uint8_t RESERVED_0[4];
  uint32_t WARNINT;       /**< Watchdog Warning Interrupt compare value., offset: 0x14 */
  uint32_t WINDOW;        /**< Watchdog Window compare value., offset: 0x18 */
} FS_WWDT_Type;

/* WWDT - Base Adresses */
#define FS_WWDT_BASE  (0x40000000u)
#define FS_WWDT       ((FS_WWDT_Type *)FS_WWDT_BASE)

/* SYSCON - Masks */
#define SYSCON_SYSRSTSTAT_WDT_MASK  0x4U
#define SYSCON_SYSRSTSTAT_ADDRESS   (uint32_t *)0x40048038U
/* PMC - Register Layout Typedef */
typedef struct
{
  uint8_t RESERVED_0[8];
  uint32_t RESETCTRL;          /**< Reset Control [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0x8 */
  uint32_t RESETCAUSE;         /**< Reset Cause register [Reset by: PoR], offset: 0xC */
  uint8_t RESERVED_1[32];
  uint32_t BODVBAT;            /**< VBAT Brown Out Dectector (BoD) control register [Reset by: PoR, Pin Reset, Software Reset], offset: 0x30 */
  uint8_t RESERVED_2[4];
  uint32_t BODCORE;            /**< Digital Core logic Brown Out Dectector control register [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0x38 */
  uint8_t RESERVED_3[8];
  uint32_t FRO1M;              /**< 1 MHz Free Running Oscillator control register [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0x44 */
  uint32_t FRO32K;             /**< 32 KHz Free Running Oscillator (FRO) control register [Reset by: PoR, Brown Out Detectors Reset], offset: 0x48 */
  uint32_t XTAL32K;            /**< 32 KHz Crystal oscillator (XTAL) control register [Reset by: PoR, Brown Out Detectors Reset], offset: 0x4C */
  uint32_t COMP;               /**< Analog Comparator control register [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0x50 */
  uint8_t RESERVED_4[20];
  uint32_t WAKEIOCAUSE;        /**< Allows to identify the Wake-up I/O source from Deep Power Down mode, offset: 0x68 */
  uint8_t RESERVED_5[8];
  uint32_t STATUSCLK;          /**< FRO and XTAL status register [Reset by: PoR, Brown Out Detectors Reset], offset: 0x74 */
  uint8_t RESERVED_6[12];
  uint32_t AOREG1;             /**< General purpose always on domain data storage [Reset by: PoR, Brown Out Detectors Reset], offset: 0x84 */
  uint8_t RESERVED_7[16];
  uint32_t RTCOSC32K;          /**< RTC 1 KHZ and 1 Hz clocks source control register [Reset by: PoR, Brown Out Detectors Reset], offset: 0x98 */
  uint32_t OSTIMERr;           /**< OS Timer control register [Reset by: PoR, Brown Out Detectors Reset], offset: 0x9C */
  uint8_t RESERVED_8[16];
  uint32_t PDSLEEPCFG0;        /**< Controls the power to various modules during Low Power modes - DEEP SLEEP, POWER DOWN and DEEP POWER DOWN [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Software Reset], offset: 0xB0 */
  uint8_t RESERVED_9[4];
  uint32_t PDRUNCFG0;          /**< Controls the power to various analog blocks [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0xB8 */
  uint8_t RESERVED_10[4];
  uint32_t PDRUNCFGSET0;       /**< Controls the power to various analog blocks [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0xC0 */
  uint8_t RESERVED_11[4];
  uint32_t PDRUNCFGCLR0;       /**< Controls the power to various analog blocks [Reset by: PoR, Pin Reset, Brown Out Detectors Reset, Deep Power Down Reset, Software Reset], offset: 0xC8 */
} FS_PMC_Type;

/* PMC - Base Adresses */
#define FS_PMC_BASE  (0x50020000u)
#define FS_PMC       ((FS_PMC_Type *)FS_PMC_BASE)

/* PMC - Masks */
#define FS_PMC_RESETCAUSE_WDTRESET_MASK  (0x10U)

/* CTIMER - Register Layout Typedef */
typedef struct
{
  uint32_t IR;             /**< Interrupt Register. The IR can be written to clear interrupts. The IR can be read to identify which of eight possible interrupt sources are pending., offset: 0x0 */
  uint32_t TCR;            /**< Timer Control Register. The TCR is used to control the Timer Counter functions. The Timer Counter can be disabled or reset through the TCR., offset: 0x4 */
  uint32_t TC;             /**< Timer Counter. The 32 bit TC is incremented every PR+1 cycles of the APB bus clock. The TC is controlled through the TCR., offset: 0x8 */
  uint32_t PR;             /**< Prescale Register. When the Prescale Counter (PC) is equal to this value, the next clock increments the TC and clears the PC., offset: 0xC */
  uint32_t PC;             /**< Prescale Counter. The 32 bit PC is a counter which is incremented to the value stored in PR. When the value in PR is reached, the TC is incremented and the PC is cleared. The PC is observable and controllable through the bus interface., offset: 0x10 */
  uint32_t MCR;            /**< Match Control Register. The MCR is used to control if an interrupt is generated and if the TC is reset when a Match occurs., offset: 0x14 */
  uint32_t MR[4];          /**< Match Register . MR can be enabled through the MCR to reset the TC, stop both the TC and PC, and/or generate an interrupt every time MR matches the TC., array offset: 0x18, array step: 0x4 */
  uint32_t CCR;            /**< Capture Control Register. The CCR controls which edges of the capture inputs are used to load the Capture Registers and whether or not an interrupt is generated when a capture takes place., offset: 0x28 */
  uint32_t CR[4];          /**< Capture Register . CR is loaded with the value of TC when there is an event on the CAPn. input., array offset: 0x2C, array step: 0x4 */
  uint32_t EMR;            /**< External Match Register. The EMR controls the match function and the external match pins., offset: 0x3C */
  uint8_t RESERVED_0[48];
  uint32_t CTCR;           /**< Count Control Register. The CTCR selects between Timer and Counter mode, and in Counter mode selects the signal and edge(s) for counting., offset: 0x70 */
  uint32_t PWMC;           /**< PWM Control Register. The PWMCON enables PWM mode for the external match pins., offset: 0x74 */
  uint32_t MSR[4];         /**< Match Shadow Register . If enabled, the Match Register will be automatically reloaded with the contents of this register whenever the TC is reset to zero., array offset: 0x78, array step: 0x4 */
} FS_CTIMER_Type;

/* CTIMER - Masks */
#define FS_CTIMER_TCR_CEN_MASK   0x1U
#define FS_CTIMER_TCR_CRST_MASK  0x2U

/******************************************************************************
* Code
******************************************************************************/
/*****************************************************************************/
void FS_WDOG_Setup(fs_wdog_test *pWatchdogBackup)
{
    pWatchdogBackup->resets = 0U;                  /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0U;    /* clear the wd_test_uncomplete flag */

    FS_WDOG->REFRESH = 0xA602U;                    /* refresh sequence */
    FS_WDOG->REFRESH = 0xB480U;

    FS_LPTMR0->CSR |= FS_LPTMR_CSR_TEN_MASK;       /* start of LPTMR */

    while(1)                                      /* wait for WD reset */
    {
        FS_LPTMR0->CNR = 0U;                       /* write any value to synchronize the counter value */
        pWatchdogBackup->counter = FS_LPTMR0->CNR; /* store the actual value from LPTMR */
    }
}

/*****************************************************************************/
void FS_WDOG_Check(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup)
{
  uint32_t errorFlag = 0U;

  if ((FS_RCM->SRS0 & RCM_SRS0_WDOG_MASK) == RCM_SRS0_WDOG_MASK) /* if WD reset has occured */
  {
     pWatchdogBackup->resets += 1U; /* increment the number of WD resets */
    /* check if the watchdog resets the system too many times */
    if (pWatchdogBackup->resets >= limitResets)
    {
      pWatchdogBackup->resets = limitResets; /* to avoid overflow of reset counter */
      errorFlag = 1U;   /* set the error flag */
    }
    /* check if the timeout from watchdog test corresponds to value of independent timer */
    if ((pWatchdogBackup->counter > limitHigh)||(pWatchdogBackup->counter < limitLow))
    {
      errorFlag = 1U;   /* set the error flag */
    }
  }
  else 
  {
    pWatchdogBackup->wdTestUncompleteFlag = 1U;  /* set the flag to 1 */
    errorFlag = 1U;   /* set the error flag */
  }
  /* if endless loop is enabled and some error has occured, wait in loop */
  if (errorFlag && endlessLoopEnable)
  {
    while(1)
    {
      ASM_INLINE("nop");
    }
  }
}

/*****************************************************************************/
void FS_WDOG_Setup_KE0XZ(fs_wdog_test *pWatchdogBackup)
{
    pWatchdogBackup->resets = 0U;                   /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0U;     /* clear the wd_test_uncomplete flag */

    FS_WDOG_KE0XZ->CNT = 0x02A6U;                   /* refresh sequence */
    FS_WDOG_KE0XZ->CNT = 0x80B4U;

    FS_RTC_KE0XZ->MOD = 0xFFFFU;                    /* start of RTC */

    while(1)                                       /* wait for WD reset */
    {
      pWatchdogBackup->counter = FS_RTC_KE0XZ->CNT; /* store the actual value from RTC */
    }
}

/*****************************************************************************/
void FS_WDOG_Check_KE0XZ(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup)
{
  uint32_t errorFlag = 0U;

  if ((FS_SIM_KE0XZ->SRSID & FS_SIM_KE0XZ_SRSID_WDOG_MASK) == FS_SIM_KE0XZ_SRSID_WDOG_MASK) /* if WD reset has occured */
  {
     pWatchdogBackup->resets += 1U; /* increment the number of WD resets */
    /* check if the watchdog resets the system too many times */
    if (pWatchdogBackup->resets >= limitResets)
    {
      pWatchdogBackup->resets = limitResets; /* to avoid overflow of reset counter */
      errorFlag = 1U;   /* set the error flag */
    }
    /* check if the timeout from watchdog test corresponds to value of independent timer */
    if ((pWatchdogBackup->counter > limitHigh) || (pWatchdogBackup->counter < limitLow))
    {
      errorFlag = 1U;   /* set the error flag */
    }
  }
  else 
  {
    pWatchdogBackup->wdTestUncompleteFlag = 1U;  /* set the flag to 1 */
    errorFlag = 1U;   /* set the error flag */
  }
  /* if endless loop is enabled and some error has occured, wait in loop */
  if (errorFlag && endlessLoopEnable)
  {
    while(1)
    {
      ASM_INLINE("nop");
    }
  }
}

/*****************************************************************************/
void FS_WDOG_Setup_COP(fs_wdog_test *pWatchdogBackup)
{
    pWatchdogBackup->resets = 0U;                 /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0U;   /* clear the wd_test_uncomplete flag */

    FS_SIM_KL2X->SRVCOP = FS_SIM_KL2X_SRVCOP_SRVCOP(0x55U);
    FS_SIM_KL2X->SRVCOP = FS_SIM_KL2X_SRVCOP_SRVCOP(0xAAU);

    FS_LPTMR0->CSR |= FS_LPTMR_CSR_TEN_MASK;      /* start of LPTMR */

    while(1)                                     /* wait for WD reset */
    {
      FS_LPTMR0->CNR = 0U;                        /* write any value to synchronize the counter value */
      pWatchdogBackup->counter = FS_LPTMR0->CNR;  /* store the actual value from LPTMR */
    }
}

/*****************************************************************************/
void FS_WDOG_Setup_KE1XZ(fs_wdog_test *pWatchdogBackup)
{
    pWatchdogBackup->resets = 0U;                 /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0U;   /* clear the wd_test_uncomplete flag */

    FS_WDOG_KE1XZ->CNT = 0xB480A602U;             /* refresh sequence */

    FS_LPTMR0->CSR |= FS_LPTMR_CSR_TEN_MASK;      /* start of LPTMR */

    while(1)                                     /* wait for WD reset */
    {
      FS_LPTMR0->CNR = 0U;                        /* write any value to synchronize the counter value */
      pWatchdogBackup->counter = FS_LPTMR0->CNR;  /* store the actual value from LPTMR */
    }
}

/*****************************************************************************/
void FS_WDOG_Check_KE1XZ(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup)
{
  uint32_t errorFlag = 0U;

  if ((FS_KE1XZ_RCM->SRS & FS_RCM_KE1XZ_SRS_WDOG_MASK) == FS_RCM_KE1XZ_SRS_WDOG_MASK) /* if WD reset has occured */
  {
     pWatchdogBackup->resets +=1U; /* increment the number of WD resets */
    /* check if the watchdog resets the system too many times */
    if (pWatchdogBackup->resets >= limitResets)
    {
      pWatchdogBackup->resets = limitResets; /* to avoid overflow of reset counter */
      errorFlag = 1U;   /* set the error flag */
    }
    /* check if the timeout from watchdog test corresponds to value of independent timer */
    if ((pWatchdogBackup->counter > limitHigh)||(pWatchdogBackup->counter < limitLow))
    {
      errorFlag = 1U;   /* set the error flag */
    }
  }
  else 
  {
    pWatchdogBackup->wdTestUncompleteFlag = 1U;  /* set the flag to 1 */
    errorFlag = 1U;   /* set the error flag */
  }
  /* if endless loop is enabled and some error has occured, wait in loop */
  if (errorFlag && endlessLoopEnable)
  {
    while(1)
    {
      ASM_INLINE("nop");
    }
  }
}

/*****************************************************************************/
void FS_WDOG_Setup_KE1XF(fs_wdog_test *pWatchdogBackup)
{
    pWatchdogBackup->resets = 0U;                 /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0U;   /* clear the wd_test_uncomplete flag */
  
    FS_WDOG_KE1XF->CNT = 0xB480A602U;             /* refresh sequence */
      
    FS_LPTMR0->CSR |= FS_LPTMR_CSR_TEN_MASK;      /* start of LPTMR */
    
    while(1)                                     /* wait for WD reset */
    {    
      FS_LPTMR0->CNR = 0U;                        /* write any value to synchronize the counter value */
      pWatchdogBackup->counter = FS_LPTMR0->CNR;  /* store the actual value from LPTMR */      
    }
}

/*****************************************************************************/
void FS_WDOG_Check_KE1XF(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup)
{   
  uint32_t errorFlag = 0U;
    
  if ((FS_RCM_KE1XF->SRS & FS_RCM_KE1X_SRS_WDOG_MASK) == FS_RCM_KE1X_SRS_WDOG_MASK) /* if WD reset has occured */
  {
     pWatchdogBackup->resets +=1U; /* increment the number of WD resets */ 
    /* check if the watchdog resets the system too many times */
    if (pWatchdogBackup->resets >= limitResets) 
    {
      pWatchdogBackup->resets = limitResets; /* to avoid overflow of reset counter */
      errorFlag = 1U;   /* set the error flag */   
    }
    /* check if the timeout from watchdog test corresponds to value of independent timer */
    if ((pWatchdogBackup->counter > limitHigh)||(pWatchdogBackup->counter < limitLow))
    {
      errorFlag = 1U;   /* set the error flag */
    }    
  }
  else 
  {
    pWatchdogBackup->wdTestUncompleteFlag = 1U;  /* set the flag to 1 */
    errorFlag = 1U;   /* set the error flag */
  }
  /* if endless loop is enabled and some error has occured, wait in loop */  
  if (errorFlag && endlessLoopEnable) 
  {
    while(1)
    {
      ASM_INLINE("nop"); 
    }
  }
}

/*****************************************************************************/
void FS_CM0_WDOG_Setup_K32W(fs_wdog_test *pWatchdogBackup, uint32_t *pWDOG, uint32_t *pLPTMR)
{
    volatile FS_WDOG_K32W_Type *pWdg = (FS_WDOG_K32W_Type *)pWDOG;
    volatile FS_LPTMR_Type *pTmr = (FS_LPTMR_Type *)pLPTMR;

    pWatchdogBackup->resets = 0U;                /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0U;  /* clear the wd_test_uncomplete flag */
  
    pWdg->CNT = 0xB480A602U;                     /* refresh sequence */
      
    pTmr->CSR |= FS_LPTMR_CSR_TEN_MASK;          /* start of LPTMR */
    
    while(1)                                    /* wait for WD reset */
    {    
      pTmr->CNR = 0U;                            /* write any value to synchronize the counter value */
      pWatchdogBackup->counter = pTmr->CNR;      /* store the actual value from LPTMR */      
    }
}

/*****************************************************************************/
void FS_CM0_WDOG_Check_K32W(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup)
{   
  uint32_t errorFlag = 0U;
    
  if ((FS_SMC1->SRS & FS_SMC_SRS_WDOG_MASK) == FS_SMC_SRS_WDOG_MASK) /* if WD reset has occured */
  {
     pWatchdogBackup->resets += 1U; /* increment the number of WD resets */ 
    /* check if the watchdog resets the system too many times */
    if (pWatchdogBackup->resets >= limitResets) 
    {
      pWatchdogBackup->resets = limitResets; /* to avoid overflow of reset counter */
      errorFlag = 1U;   /* set the error flag */   
    }
    /* check if the timeout from watchdog test corresponds to value of independent timer */
    if ((pWatchdogBackup->counter > limitHigh)||(pWatchdogBackup->counter < limitLow))
    {
      errorFlag = 1U;   /* set the error flag */
    }    
  }
  else 
  {
    pWatchdogBackup->wdTestUncompleteFlag = 1U;  /* set the flag to 1 */
    errorFlag = 1U;   /* set the error flag */
  }
  /* if endless loop is enabled and some error has occured, wait in loop */  
  if (errorFlag && endlessLoopEnable) 
  {
    while(1)
    {
      ASM_INLINE("nop"); 
    }
  }
}

/*****************************************************************************/
void FS_CM4_WDOG_Setup_K32W(fs_wdog_test *pWatchdogBackup, uint32_t *pWDOG, uint32_t *pLPTMR)
{
    volatile FS_WDOG_K32W_Type *pWdg = (FS_WDOG_K32W_Type *)pWDOG;
    volatile FS_LPTMR_Type *pTmr = (FS_LPTMR_Type *)pLPTMR;

    pWatchdogBackup->resets = 0U;               /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0U; /* clear the wd_test_uncomplete flag */
  
    pWdg->CNT = 0xB480A602U;                    /* refresh sequence */
      
    pTmr->CSR |= FS_LPTMR_CSR_TEN_MASK;         /* start of LPTMR */
    
    while(1)                                   /* wait for WD reset */
    {    
      pTmr->CNR = 0U;                           /* write any value to synchronize the counter value */
      pWatchdogBackup->counter = pTmr->CNR;     /* store the actual value from LPTMR */      
    }
}

/*****************************************************************************/
void FS_CM4_WDOG_Check_K32W(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup)
{   
  uint32_t errorFlag = 0U;
    
  if ((FS_SMC0->SRS & FS_SMC_SRS_WDOG_MASK) == FS_SMC_SRS_WDOG_MASK) /* if WD reset has occured */
  {
     pWatchdogBackup->resets +=1U; /* increment the number of WD resets */ 
    /* check if the watchdog resets the system too many times */
    if (pWatchdogBackup->resets >= limitResets) 
    {
      pWatchdogBackup->resets = limitResets; /* to avoid overflow of reset counter */
      errorFlag = 1U;   /* set the error flag */   
    }
    /* check if the timeout from watchdog test corresponds to value of independent timer */
    if ((pWatchdogBackup->counter > limitHigh)||(pWatchdogBackup->counter < limitLow))
    {
      errorFlag = 1U;   /* set the error flag */
    }    
  }
  else 
  {
    pWatchdogBackup->wdTestUncompleteFlag = 1U;  /* set the flag to 1 */
    errorFlag = 1U;   /* set the error flag */
  }
  
  /* if endless loop is enabled and some error has occured, wait in loop */  
  if (errorFlag && endlessLoopEnable) 
  {
    while(1)
    {
      ASM_INLINE("nop"); 
    }
  }
}

/*****************************************************************************/
void FS_WDOG_Setup_RT(fs_wdog_test *pWatchdogBackup, uint32_t *pGPT)
{
    volatile FS_GPT_Type* pTmr = (FS_GPT_Type *)pGPT;
    
    pWatchdogBackup->resets = 0U;                /* Clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0U;  /* Clear the wd_test_uncomplete flag */
  
    FS_RTWDOG->CNT = 0xB480A602U;                /* Refresh sequence */   
    
    pTmr->OCR[0] = 0xFFFFFFFFU;                  /* Change "compare" value, this cause RESET of CNT to zero */
    
    while(1)                                    /* Wait for WD reset */
    {    
        pWatchdogBackup->counter = pTmr->CNT;    /* Store value from GPT */
    }
}

/*****************************************************************************/
void FS_WDOG_Check_RT(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup)
{   
    uint32_t errorFlag = 0U;
    if (FS_SRC_SRSR_WDOG3_RST_B_MSK == (FS_SRC->SRSR & FS_SRC_SRSR_WDOG3_RST_B_MSK)) /* If WD reset has occured */
    {
        /* Clear wdog3_rst_b bit */
        FS_SRC->SRSR = FS_SRC_SRSR_WDOG3_RST_B_MSK;
      
        pWatchdogBackup->resets +=1U; /* Increment the number of WD resets */ 
        /* Check if the watchdog resets the system too many times */
        if (pWatchdogBackup->resets >= limitResets) 
        {
            pWatchdogBackup->resets = limitResets; /* To avoid overflow of reset counter */
            errorFlag = 1U;   /* Set the error flag */   
        }
        
        /* Check if the timeout from watchdog test corresponds to value of independent timer */
        if ((pWatchdogBackup->counter > limitHigh) || (pWatchdogBackup->counter < limitLow))
        {
            errorFlag = 1U;   /* Set the error flag */
        }    
    }
    else 
    {     
        pWatchdogBackup->wdTestUncompleteFlag = 1U; /* Set the flag to 1 */
        errorFlag = 1U;   /* Set the error flag */
    }
    
    /* If endless loop is enabled and some error has occured, wait in loop */  
    if (errorFlag && endlessLoopEnable) 
    {
        while(1)
        {
            ASM_INLINE("nop"); 
        }
    }
}

/*****************************************************************************/
void FS_WDOG_Setup_WWDT_LPC(fs_wdog_test *pWatchdogBackup, uint32_t *pCtimer_base)
{
    pWatchdogBackup->resets = 0;               /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0; /* clear the wdTestUncompleteFlag */

    ((FS_CTIMER_Type *)pCtimer_base)->TCR |= FS_CTIMER_TCR_CEN_MASK; /* Enable and reset counter */
    ((FS_CTIMER_Type *)pCtimer_base)->TCR |= FS_CTIMER_TCR_CRST_MASK;
    ((FS_CTIMER_Type *)pCtimer_base)->TCR &= ~(FS_CTIMER_TCR_CRST_MASK);

    ASM_INLINE("CPSID i");
    FS_WWDT->FEED = 0xAA;
    FS_WWDT->FEED = 0x55;
    ASM_INLINE("CPSIE i");

    while(1) /* wait for WD reset */
    {
        pWatchdogBackup->counter = ((FS_CTIMER_Type *)pCtimer_base)->TC; /* store the actual value from LPTMR */
    }       
}

/** MRT - Register Layout Typedef */
typedef struct {
  struct {                                         /* offset: 0x0, array step: 0x10 */
   uint32_t INTVAL;                            /**< MRT Time interval value register. This value is loaded into the TIMER register., array offset: 0x0, array step: 0x10 */
   uint32_t TIMER;                             /**< MRT Timer register. This register reads the value of the down-counter., array offset: 0x4, array step: 0x10 */
   uint32_t CTRL;                              /**< MRT Control register. This register controls the MRT modes., array offset: 0x8, array step: 0x10 */
   uint32_t STAT;                              /**< MRT Status register., array offset: 0xC, array step: 0x10 */
  } CHANNEL[4];
    uint8_t RESERVED_0[176];
    uint32_t MODCFG;                            /**< Module Configuration register. This register provides information about this particular MRT instance., offset: 0xF0 */
    uint32_t IDLE_CH;                           /**< Idle channel register. This register returns the number of the first idle channel., offset: 0xF4 */
  uint32_t IRQ_FLAG;                          /**< Global interrupt flag register, offset: 0xF8 */
} FS_MRT_Type;

/*****************************************************************************/
void FS_WDOG_Setup_WWDT_LPC_mrt(fs_wdog_test *pWatchdogBackup, uint32_t *pMRTimer_base, uint8_t channel)
{
    pWatchdogBackup->resets = 0;               /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0; /* clear the wdTestUncompleteFlag */

      /* Enable and reset counter */
   ((FS_MRT_Type *)pMRTimer_base)->CHANNEL[channel].INTVAL = 0x7FFFFFFF; /* Load maximum value of 31 bit (counter decrement)   */
    
    ASM_INLINE("CPSID i");
    FS_WWDT->FEED = 0xAA;
    FS_WWDT->FEED = 0x55;
    ASM_INLINE("CPSIE i");

    while(1) /* wait for WD reset */
    {                   /* recalculate for using "clasic" check function (with increment counter ) */
        pWatchdogBackup->counter = 0x7FFFFFFF - ((FS_MRT_Type *)pMRTimer_base)->CHANNEL[channel].TIMER; /* store the actual value from LPTMR */
    }       
}

/*****************************************************************************/
void FS_WDOG_Check_WWDT_LPC(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup)
{
    uint32_t errorFlag = 0;
  
    if (( (*SYSCON_SYSRSTSTAT_ADDRESS) & SYSCON_SYSRSTSTAT_WDT_MASK) == SYSCON_SYSRSTSTAT_WDT_MASK) /* if WD reset has occured */
    {
        *SYSCON_SYSRSTSTAT_ADDRESS |= (1 & SYSCON_SYSRSTSTAT_WDT_MASK);
           
        pWatchdogBackup->resets +=1; /* increment the number of WD resets */

        /* check if the watchdog resets the system too many times */
        if (pWatchdogBackup->resets >= limitResets)
        {
            pWatchdogBackup->resets = limitResets; /* to avoid overflow of reset counter */
            errorFlag = 1; /* set the error flag */
        }
        /* check if the timeout from watchdog test corresponds to value of independent timer */
        if ((pWatchdogBackup->counter > limitHigh)||(pWatchdogBackup->counter < limitLow))
        {
            errorFlag = 1; /* set the error flag */
        }
    }
    else 
    {
        pWatchdogBackup->wdTestUncompleteFlag = 1; /* set the flag to 1 */
        errorFlag = 1; /* set the error flag */
    }

    /* If endless loop is enabled and some error has occured, wait in loop */
    if (errorFlag && endlessLoopEnable) 
    {
        while(1)
        {
            ASM_INLINE("nop");
        }
    }
}

/*****************************************************************************/
void FS_WDOG_Check_WWDT_LPC55SXX(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test *pWatchdogBackup)
{
    uint32_t errorFlag = 0;
    
    if (((FS_PMC->AOREG1 >> 4) & FS_PMC_RESETCAUSE_WDTRESET_MASK) == FS_PMC_RESETCAUSE_WDTRESET_MASK) /* if WD reset has occured */
    {
        pWatchdogBackup->resets +=1; /* increment the number of WD resets */

        /* check if the watchdog resets the system too many times */
        if (pWatchdogBackup->resets >= limitResets)
        {
            pWatchdogBackup->resets = limitResets; /* to avoid overflow of reset counter */
            errorFlag = 1; /* set the error flag */
        }
        /* check if the timeout from watchdog test corresponds to value of independent timer */
        if ((pWatchdogBackup->counter > limitHigh)||(pWatchdogBackup->counter < limitLow))
        {
            errorFlag = 1; /* set the error flag */
        }
    }
    else 
    {
        pWatchdogBackup->wdTestUncompleteFlag = 1; /* set the flag to 1 */
        errorFlag = 1; /* set the error flag */
    }

    /* If endless loop is enabled and some error has occured, wait in loop */
    if (errorFlag && endlessLoopEnable) 
    {
        while(1)
        {
            ASM_INLINE("nop");
        }
    }
}


