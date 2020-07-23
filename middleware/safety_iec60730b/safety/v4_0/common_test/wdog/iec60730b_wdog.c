/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2020 NXP.
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
 * @version 4.0
 *
 * @brief Watchdog test common routines - IEC60730 Class B.
 */

#include "iec60730b.h"

/*******************************************************************************
 * Definitions KINETIS
 ******************************************************************************/
/* LPTMR - Register Layout Typedef */
volatile typedef struct
{
    uint32_t CSR;  /**< Low Power Timer Control Status Register, offset: 0x0 */
    uint32_t PSR;  /**< Low Power Timer Prescale Register, offset: 0x4 */
    uint32_t CMR;  /**< Low Power Timer Compare Register, offset: 0x8 */
    uint32_t CNR;  /**< Low Power Timer Counter Register, offset: 0xC */
} fs_lptmr_t;

/* LPTMR - Masks */
#define FS_LPTMR_CSR_TEN_MASK  (0x1U) //TODO ADD TO structure


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
} fs_wdog_t;

/* WDOG - Register Layout Typedef */
typedef struct
{
    uint32_t CS;     /**< Watchdog Control and Status Register, offset: 0x0 */
    uint32_t CNT;    /**< Watchdog Counter Register, offset: 0x4 */
    uint32_t TOVAL;  /**< Watchdog Timeout Value Register, offset: 0x8 */
    uint32_t WIN;    /**< Watchdog Window Register, offset: 0xC */
} fs_wdog32_t;

/**********************
 * KE0xZ
 *********************/
/* WDOG - Register Layout Typedef */
typedef struct
{
    uint8_t CS1;            /* Watchdog Control and Status Register 1, offset: 0x0 */
    uint8_t CS2;            /* Watchdog Control and Status Register 2, offset: 0x1 */
    union
    {                       /* offset: 0x2 */
        uint16_t CNT;       /* WDOG_CNT register., offset: 0x2 */
        struct
        {                   /* offset: 0x2 */
            uint8_t CNTH;   /* Watchdog Counter Register: High, offset: 0x2 */
            uint8_t CNTL;   /* Watchdog Counter Register: Low, offset: 0x3 */
        } CNT8B;
    };
    union
    {                       /* offset: 0x4 */
        uint16_t TOVAL;     /* WDOG_TOVAL register., offset: 0x4 */
        struct
        {                   /* offset: 0x4 */
            uint8_t TOVALH; /* Watchdog Timeout Value Register: High, offset: 0x4 */
            uint8_t TOVALL; /* Watchdog Timeout Value Register: Low, offset: 0x5 */
        } TOVAL8B;
    };
    union
    {                       /* offset: 0x6 */
        uint16_t WIN;       /* WDOG_WIN register., offset: 0x6 */
        struct
        {                   /* offset: 0x6 */
            uint8_t WINH;   /* Watchdog Window Register: High, offset: 0x6 */
            uint8_t WINL;   /* Watchdog Window Register: Low, offset: 0x7 */
        } WIN8B;
    };
} fs_wdog_ke0xz_t;

/* RTC - Register Layout Typedef */
volatile typedef struct
{
    uint32_t SC;   /* RTC Status and Control Register, offset: 0x0 */
    uint32_t MOD;  /* RTC Modulo Register, offset: 0x4 */
    uint32_t CNT;  /* RTC Counter Register, offset: 0x8 */
} fs_rtc_ke0xz_t;

/**********************
 * KLxx with COP 
 *********************/
/* SIM COP - Register Layout Typedef */
volatile typedef struct
{
    uint8_t reservd[4351]; //TODO VALIDATE IT! 
    uint32_t COPC;    /**< COP Control Register, offset: 0x1100 */
    uint32_t SRVCOP;  /**< Service COP, offset: 0x1104 */
} fs_sim_kl2x_t;

/* SIM COP - Masks */
#define FS_SIM_KL2X_SRVCOP_SRVCOP(x)    (((uint32_t)(x)) & 0xFFU)

/*******************************************************************************
 * Definitions IMXRT
 ******************************************************************************/
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
} fs_gpt_t;

/*******************************************************************************
 * Definitions IMX8M
 ******************************************************************************/
/* WDOG - Register Layout Typedef */
typedef struct
{
  uint16_t WCR;   /**< Watchdog Control Register, offset: 0x0 */
  uint16_t WSR;   /**< Watchdog Service Register, offset: 0x2 */
  uint16_t WRSR;  /**< Watchdog Reset Status Register, offset: 0x4 */
  uint16_t WICR;  /**< Watchdog Interrupt Control Register, offset: 0x6 */
  uint16_t WMCR;  /**< Watchdog Miscellaneous Control Register, offset: 0x8 */
} fs_wdog_imx8m_t;

/*******************************************************************************
 * Definitions LPC
 ******************************************************************************/
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
} fs_wwdt_t;

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
} fs_ctimer_t;

/* CTIMER - Masks */
#define FS_CTIMER_TCR_CEN_MASK   0x1U
#define FS_CTIMER_TCR_CRST_MASK  0x2U

/** MRT - Register Layout Typedef */
typedef struct {
    struct {                    /* offset: 0x0, array step: 0x10 */
        uint32_t INTVAL;        /**< MRT Time interval value register. This value is loaded into the TIMER register., array offset: 0x0, array step: 0x10 */
        uint32_t TIMER;         /**< MRT Timer register. This register reads the value of the down-counter., array offset: 0x4, array step: 0x10 */
        uint32_t CTRL;          /**< MRT Control register. This register controls the MRT modes., array offset: 0x8, array step: 0x10 */
        uint32_t STAT;          /**< MRT Status register., array offset: 0xC, array step: 0x10 */
    } CHANNEL[4];
    uint8_t RESERVED_0[176];
    uint32_t MODCFG;            /**< Module Configuration register. This register provides information about this particular MRT instance., offset: 0xF0 */
    uint32_t IDLE_CH;           /**< Idle channel register. This register returns the number of the first idle channel., offset: 0xF4 */
    uint32_t IRQ_FLAG;          /**< Global interrupt flag register, offset: 0xF8 */
} fs_mrt_t;

/*******************************************************************************
 * Implementation
 ******************************************************************************/
void FS_WDOG_Setup_LPTMR(fs_wdog_test_t *pWatchdogBackup, uint8_t refresh_index)
{
    volatile fs_lptmr_t *pTmr = (fs_lptmr_t *)pWatchdogBackup->RefTimerBase;
    
    pWatchdogBackup->resets = 0U;                  /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0U;    /* clear the wd_test_uncomplete flag */
 
    switch(refresh_index) /* Choose correct refresh sequence */
    {
      case FS_KINETIS_WDOG :
        ((fs_wdog_t *)pWatchdogBackup->WdogBase)->REFRESH = 0xA602U; /* refresh sequence */
        ((fs_wdog_t *)pWatchdogBackup->WdogBase)->REFRESH = 0xB480U;
        break;
      
      case FS_WDOG32 :
        ((fs_wdog32_t *)pWatchdogBackup->WdogBase)->CNT = 0xB480A602U; /* refresh sequence */
        break;
        
      case FS_COP_WDOG:
        ((fs_sim_kl2x_t *)pWatchdogBackup->WdogBase)->SRVCOP = FS_SIM_KL2X_SRVCOP_SRVCOP(0x55U);
        ((fs_sim_kl2x_t *)pWatchdogBackup->WdogBase)->SRVCOP = FS_SIM_KL2X_SRVCOP_SRVCOP(0xAAU);
      break;
      default:
        break;
    }

    pTmr->CSR |= FS_LPTMR_CSR_TEN_MASK;       /* start of LPTMR */

    while(1)                                       /* wait for WD reset */
    {
        pTmr->CNR = 0U;/* write any value to synchronize the counter value */
        pWatchdogBackup->counter = pTmr->CNR; /* store the actual value from LPTMR */
    }
}

/*******************************************************************************
 ******************************************************************************/
void FS_WDOG_Setup_KE0XZ(fs_wdog_test_t *pWatchdogBackup)
{
    volatile fs_wdog_ke0xz_t *pWdg = (fs_wdog_ke0xz_t *)pWatchdogBackup->WdogBase;
    volatile fs_rtc_ke0xz_t *pTmr = (fs_rtc_ke0xz_t *)pWatchdogBackup->RefTimerBase;
    
    pWatchdogBackup->resets = 0U;                     /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0U;       /* clear the wd_test_uncomplete flag */

    pWdg->CNT = 0x02A6U;  /* refresh sequence */
    pWdg->CNT = 0x80B4U;

    pTmr->MOD = 0xFFFFU;  /* start of RTC */

    while(1)   /* wait for WD reset */
    {
        pWatchdogBackup->counter = pTmr->CNT; /* store the actual value from RTC */
    }
}

/*******************************************************************************
 ******************************************************************************/
void FS_WDOG_Setup_IMX_GPT(fs_wdog_test_t *pWatchdogBackup, uint8_t refresh_index)
{
    volatile fs_gpt_t *pTmr = (fs_gpt_t *)pWatchdogBackup->RefTimerBase;  

    pWatchdogBackup->resets = 0U;               /* Clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0U; /* Clear the wd_test_uncomplete flag */

    switch(refresh_index) /* Choose correct refresh sequence */
    {
      case FS_IMXRT:
           ((fs_wdog32_t *)pWatchdogBackup->WdogBase)->CNT = 0xB480A602U;     /* Refresh sequence */   
        break;
      
      case FS_IMX8M:
           ((fs_wdog_imx8m_t *)pWatchdogBackup->WdogBase)->WSR = 0x5555;       /* Refresh sequence */ 
           ((fs_wdog_imx8m_t *)pWatchdogBackup->WdogBase)->WSR = 0xAAAA;        
        break;
      default:
        break;
    }
    
    pTmr->OCR[0] = 0xFFFFFFFFU;                 /* Change "compare" value, this cause RESET of CNT to zero */

    while(1)                                    /* Wait for WD reset */
    {    
        pWatchdogBackup->counter = pTmr->CNT;   /* Store value from GPT */
    }
}

/*******************************************************************************
 ******************************************************************************/
void FS_WDOG_Setup_WWDT_LPC(fs_wdog_test_t *pWatchdogBackup)
{
    volatile fs_wwdt_t *pWdg = (fs_wwdt_t *)pWatchdogBackup->WdogBase;
    volatile fs_ctimer_t *pTmr = (fs_ctimer_t *)pWatchdogBackup->RefTimerBase;
    
    pWatchdogBackup->resets = 0;               /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0; /* clear the wdTestUncompleteFlag */

    pTmr->TCR |= FS_CTIMER_TCR_CEN_MASK; /* Enable and reset counter */
    pTmr->TCR |= FS_CTIMER_TCR_CRST_MASK;
    pTmr->TCR &= ~(FS_CTIMER_TCR_CRST_MASK);

    ASM_INLINE("CPSID i");
    pWdg->FEED = 0xAA;
    pWdg->FEED = 0x55;
    ASM_INLINE("CPSIE i");

    while(1) /* wait for WD reset */
    {
        pWatchdogBackup->counter = pTmr->TC; /* store the actual value from LPTMR */
    }       
}

/*******************************************************************************
 ******************************************************************************/
void FS_WDOG_Setup_WWDT_LPC_mrt(fs_wdog_test_t *pWatchdogBackup, uint8_t channel)
{
    volatile fs_wwdt_t *pWdg = (fs_wwdt_t *)pWatchdogBackup->WdogBase;
    volatile fs_mrt_t *pTmr = (fs_mrt_t *)pWatchdogBackup->RefTimerBase;
    pWatchdogBackup->resets = 0;               /* clear the number of WD resets */
    pWatchdogBackup->wdTestUncompleteFlag = 0; /* clear the wdTestUncompleteFlag */

    /* Enable and reset counter */
    pTmr->CHANNEL[channel].INTVAL = 0x7FFFFFFF; /* Load maximum value of 31 bit (counter decrement)   */

    ASM_INLINE("CPSID i");
    pWdg->FEED = 0xAA;
    pWdg->FEED = 0x55;
    ASM_INLINE("CPSIE i");

    while(1) /* wait for WD reset */
    {
        /* recalculate for using "clasic" check function (with increment counter ) */
        pWatchdogBackup->counter = 0x7FFFFFFF - pTmr->CHANNEL[channel].TIMER; /* store the actual value from LPTMR */
    }       
}

/*******************************************************************************
 ******************************************************************************/
uint32_t FS_WDOG_Check(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test_t *pWatchdogBackup, uint8_t clear_flag, uint8_t RegWide8b)
{
    uint32_t state = 0U;
    uint32_t srsVal =0;
    
   if(RegWide8b)
     srsVal = *(uint8_t*)(pWatchdogBackup->pResetDetectRegister); 
   else 
     srsVal = *(uint32_t*)(pWatchdogBackup->pResetDetectRegister);
          /*Mask for value added - due to KE02z */
    if ( (srsVal & pWatchdogBackup->ResetDetectMask ) == pWatchdogBackup->ResetDetectMask) /* if WD reset has occured */
    {
      if(clear_flag)
      {
         if(RegWide8b)
           *((uint8_t*)(pWatchdogBackup->pResetDetectRegister)) = pWatchdogBackup->ResetDetectMask;
         else
           *((uint32_t*)(pWatchdogBackup->pResetDetectRegister)) = pWatchdogBackup->ResetDetectMask;
      }
      
        pWatchdogBackup->resets += 1U; /* increment the number of WD resets */
        /* check if the watchdog resets the system too many times */
        if (pWatchdogBackup->resets >= limitResets)
        {
            pWatchdogBackup->resets = limitResets; /* to avoid overflow of reset counter */
            state = FS_FAIL_WDOG_OVER_RESET; //state overresets
        } 
        /* check if the timeout from watchdog test corresponds to value of independent timer */
        if ((pWatchdogBackup->counter > limitHigh)||(pWatchdogBackup->counter < limitLow))
        {
          state = FS_FAIL_WDOG_VALUE;//state = wrong timeout
        }
    }
    else 
    {
        pWatchdogBackup->wdTestUncompleteFlag = 1U;  /* set the flag to 1 */
        state = FS_FAIL_WDOG_WRONG_RESET; //state wrong reset 
    }

    /* if endless loop is enabled and some error has occured, wait in loop */
    if (state && endlessLoopEnable)
    {
        while(1)
        {
          ASM_INLINE("nop");
        }
    }
      
    return state;
}

/*******************************************************************************
 ******************************************************************************/
uint32_t FS_WDOG_Check_WWDT_LPC(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test_t *pWatchdogBackup)
{
    uint32_t state = 0U;

    if (( *(uint32_t*)(pWatchdogBackup->pResetDetectRegister) & pWatchdogBackup->ResetDetectMask ) == pWatchdogBackup->ResetDetectMask) /* if WD reset has occured */
    {
      *(uint32_t*)(pWatchdogBackup->pResetDetectRegister) |= (1 & pWatchdogBackup->ResetDetectMask);
      
       // *SYSCON_SYSRSTSTAT_ADDRESS |= (1 & SYSCON_SYSRSTSTAT_WDT_MASK); //TODO ??? 


        pWatchdogBackup->resets += 1U; /* increment the number of WD resets */
        /* check if the watchdog resets the system too many times */
        if (pWatchdogBackup->resets >= limitResets)
        {
            pWatchdogBackup->resets = limitResets; /* to avoid overflow of reset counter */
            state = FS_FAIL_WDOG_OVER_RESET; //state overresets
        } 
        /* check if the timeout from watchdog test corresponds to value of independent timer */
        if ((pWatchdogBackup->counter > limitHigh)||(pWatchdogBackup->counter < limitLow))
        {
          state = FS_FAIL_WDOG_VALUE;//state = wrong timeout
        }
    }
    else 
    {
        pWatchdogBackup->wdTestUncompleteFlag = 1U;  /* set the flag to 1 */
        state = FS_FAIL_WDOG_WRONG_RESET; //state wrong reset 
    }

    /* if endless loop is enabled and some error has occured, wait in loop */
    if (state & endlessLoopEnable)
    {
        while(1)
        {
            ASM_INLINE("nop");
        }
    }
      
    return state;
}
/*******************************************************************************
 ******************************************************************************/
uint32_t FS_WDOG_Check_WWDT_LPC55SXX(uint32_t limitHigh, uint32_t limitLow, uint32_t limitResets, bool_t endlessLoopEnable, fs_wdog_test_t *pWatchdogBackup)
{
    uint32_t state = 0U;

    if (((*(uint32_t*)(pWatchdogBackup->pResetDetectRegister) >> 4) &  pWatchdogBackup->ResetDetectMask) ==  pWatchdogBackup->ResetDetectMask) /* if WD reset has occured */
    {
        pWatchdogBackup->resets += 1U; /* increment the number of WD resets */
        /* check if the watchdog resets the system too many times */
        if (pWatchdogBackup->resets >= limitResets)
        {
            pWatchdogBackup->resets = limitResets; /* to avoid overflow of reset counter */
            state = FS_FAIL_WDOG_OVER_RESET; //state overresets
        } 
        /* check if the timeout from watchdog test corresponds to value of independent timer */
        if ((pWatchdogBackup->counter > limitHigh)||(pWatchdogBackup->counter < limitLow))
        {
          state = FS_FAIL_WDOG_VALUE;//state = wrong timeout
        }
    }
    else 
    {
        pWatchdogBackup->wdTestUncompleteFlag = 1U;  /* set the flag to 1 */
        state = FS_FAIL_WDOG_WRONG_RESET; //state wrong reset 
    }

    /* if endless loop is enabled and some error has occured, wait in loop */
    if (state & endlessLoopEnable)
    {
        while(1)
        {
            ASM_INLINE("nop");
        }
    }
      
    return state;
}