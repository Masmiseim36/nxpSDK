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
* @file      IEC60730_B_CM4_CM7_wdg.c
*
* @author    B48632
* 
* @version   3.0
*
* @date      Nov-01-2018
* 
* @brief     IEC60730 Class B Safety Watchdog test for Cortex CM4, CM7 cores
*`
************************************************************************************************

***********************************************************************************************/

#include "iec60730b_wdog.h"


/* WATCHDOG */
/* WATCHDOG_Register_Accessor_Macros */
typedef struct  
{
   unsigned short STCTRLH;                           /* Watchdog Status and Control Register High, offset: 0x0 */
   unsigned short STCTRLL;                           /* Watchdog Status and Control Register Low, offset: 0x2 */
   unsigned short TOVALH;                            /* Watchdog Time-out Value Register High, offset: 0x4 */
   unsigned short TOVALL;                            /* Watchdog Time-out Value Register Low, offset: 0x6 */
   unsigned short WINH;                              /* Watchdog Window Register High, offset: 0x8 */
   unsigned short WINL;                              /* Watchdog Window Register Low, offset: 0xA */
   unsigned short REFRESH;                           /* Watchdog Refresh register, offset: 0xC */
   unsigned short UNLOCK;                            /* Watchdog Unlock register, offset: 0xE */
   unsigned short TMROUTH;                           /* Watchdog Timer Output Register High, offset: 0x10 */
   unsigned short TMROUTL;                           /* Watchdog Timer Output Register Low, offset: 0x12 */
   unsigned short RSTCNT;                            /* Watchdog Reset Count register, offset: 0x14 */
   unsigned short PRESC;                             /* Watchdog Prescaler register, offset: 0x16 */
} Watchdog_Struct;


#define Watchdog_BASE     (0x40052000)
#define WATCHDOG_REG      ((Watchdog_Struct   *)  Watchdog_BASE)

/* STCTRLH Bit Fields */
#define WATCHDOG_STCTRLH_WDOGEN_MSK                 0x1
#define WATCHDOG_STCTRLH_CLKSRC_MSK                 0x2
#define WATCHDOG_STCTRLH_IRQRSTEN_MSK               0x4
#define WATCHDOG_STCTRLH_WINEN_MSK                  0x8
#define WATCHDOG_STCTRLH_ALLOWUPDATE_MSK            0x10
#define WATCHDOG_STCTRLH_DBGEN_MSK                  0x20
#define WATCHDOG_STCTRLH_STOPEN_MSK                 0x40
#define WATCHDOG_STCTRLH_WAITEN_MSK                 0x80
#define WATCHDOG_STCTRLH_TESTWDOG_MSK               0x400
#define WATCHDOG_STCTRLH_TESTSEL_MSK                0x800
#define WATCHDOG_STCTRLH_BYTESEL_MSK                0x3000
#define WATCHDOG_STCTRLH_BYTESEL_SHIFT               12
#define WATCHDOG_STCTRLH_BYTESEL(x)                  (((unsigned short)(((unsigned short)(x))<<WATCHDOG_STCTRLH_BYTESEL_SHIFT))&WATCHDOG_STCTRLH_BYTESEL_MSK)
#define WATCHDOG_STCTRLH_DISTESTWDOG_MSK            0x4000
/* STCTRLL Bit Fields */
#define WATCHDOG_STCTRLL_INTFLG_MSK                 0x8000
/* PRESC Bit Fields */
#define WATCHDOG_PRESC_PRESCVAL_MSK                 0x700
#define WATCHDOG_PRESC_PRESCVAL_SHIFT                8
#define WATCHDOG_PRESC_PRESCVAL(x)                   (((unsigned short)(((unsigned short)(x))<<WATCHDOG_PRESC_PRESCVAL_SHIFT))&WATCHDOG_PRESC_PRESCVAL_MSK)


/* LPTMR */
/* LPTMR_Register_Accessor_Macros */

#define LPTMR_BASE     (0x40040000)
#define LPTMR_REG      ((LPTMR_Str   *)  LPTMR_BASE)

volatile typedef struct {
  unsigned long CSR;         /* Low Power Timer Control Status Register, offset: 0x0 */
  unsigned long PSR;         /* Low Power Timer Prescale Register, offset: 0x4 */
  unsigned long CMR;         /* Low Power Timer Compare Register, offset: 0x8 */
  unsigned long CNR;         /* Low Power Timer Counter Register, offset: 0xC */
} LPTMR_Str;

#define LPTMR_CSR_TIE_MSK                     0x40
#define LPTMR_CSR_TCF_MSK                     0x80
#define LPTMR_CSR_TEN_MSK                     0x1


/* RCM */
/* RCM_Register_Accessor_Macros */
#define RCM_BASE     (0x4007F000)
#define RCM_REG      ((RCM_Str   *)  RCM_BASE)

volatile typedef struct {
  unsigned char SRS0;
} RCM_Str;

#define RCM_SRS0_WDOG_MSK                     0x20
#define RCM_SRS0_POR_MSK                      0x80


/* WATCHDOG KE */
/* WATCHDOG_Register_Accessor_Macros */
typedef struct  
{
  unsigned long CS;          /* Watchdog Control and Status Register, offset: 0x0 */
  unsigned long CNT;         /* Watchdog Counter Register, offset: 0x4 */
  unsigned long TOVAL;       /* Watchdog Timeout Value Register, offset: 0x8 */
  unsigned long WIN;         /* Watchdog Window Register, offset: 0xC */   
} Watchdog_Ke_Struct;


#define WATCHDOG_KE_REG      ((Watchdog_Ke_Struct   *)  Watchdog_BASE)


#define Watchdog_K32w_BASE   (0x4002A000u)
#define WATCHDOG_K32W_REG    ((Watchdog_Ke_Struct   *)  Watchdog_K32w_BASE)

#define WATCHDOG_KE_CS_STOP_MSK              0x1
#define WATCHDOG_KE_CS_WAIT_MSK              0x2
#define WATCHDOG_KE_CS_DBG_MSK               0x4
#define WATCHDOG_KE_CS_TST_MSK               0x18
#define WATCHDOG_KE_CS_TST_SHIFT             3
#define WATCHDOG_KE_CS_TST(x)                (((unsigned long)(((unsigned long)(x)) << WATCHDOG_KE_CS_TST_SHIFT)) & WATCHDOG_KE_CS_TST_MSK)
#define WATCHDOG_KE_CS_UPDATE_MSK            0x20
#define WATCHDOG_KE_CS_INT_MSK               0x40
#define WATCHDOG_KE_CS_EN_MSK                0x80
#define WATCHDOG_KE_CS_CLK_MSK               0x18
#define WATCHDOG_KE_CS_CLK_SHIFT             8
#define WATCHDOG_KE_CS_CLK(x)                (((unsigned long)(((unsigned long)(x)) << WATCHDOG_KE_CS_CLK_SHIFT)) & WATCHDOG_KE_CS_CLK_MSK)
#define WATCHDOG_KE_CS_RCS_MSK               0x400
#define WATCHDOG_KE_CS_ULK_MSK               0x800
#define WATCHDOG_KE_CS_PRES_MSK              0x1000
#define WATCHDOG_KE_CS_CMD32EN_MSK           0x2000
#define WATCHDOG_KE_CS_FLG_MSK               0x4000
#define WATCHDOG_KE_CS_WIN_MSK               0x8000

/* RCM KE*/
/* RCM_Register_Accessor_Macros */
#define RCM_KE_REG      ((RCM_KE_Str   *)  RCM_BASE)

volatile typedef struct {
  unsigned long empty0;
  unsigned long empty1;
  unsigned long SRS;
} RCM_KE_Str;

#define RCM_KE_SRS_WDOG_MSK                     0x20
#define RCM_KE_SRS_POR_MSK                      0x80


/* MSMC0 */
/* MSMC0_Register_Accessor_Macros */
#define MSMC0_BASE     (0x40020000)
#define MSMC0_REG  ((MSMC0_Str  *) MSMC0_BASE) 

volatile typedef struct {
  unsigned long empty0[8];
  unsigned long SRS;  
} MSMC0_Str;  

#define MSMC_SRS_WAKEUP_MSK                      0x1U
#define MSMC_SRS_POR_MSK                         0x2U
#define MSMC_SRS_LVD_MSK                         0x4U
#define MSMC_SRS_HVD_MSK                         0x8U
#define MSMC_SRS_WARM_MSK                        0x10U
#define MSMC_SRS_FATAL_MSK                       0x20U
#define MSMC_SRS_CORE_MSK                        0x80U
#define MSMC_SRS_PIN_MSK                         0x100U
#define MSMC_SRS_MDM_MSK                         0x200U
#define MSMC_SRS_RSTACK_MSK                      0x400U
#define MSMC_SRS_STOPACK_MSK                     0x800U
#define MSMC_SRS_SCG_MSK                         0x1000U
#define MSMC_SRS_WDOG_MSK                        0x2000U
#define MSMC_SRS_SW_MSK                          0x4000U
#define MSMC_SRS_LOCKUP_MSK                      0x8000U
#define MSMC_SRS_CORE0_MSK                       0x10000U
#define MSMC_SRS_CORE1_MSK                       0x20000U
#define MSMC_SRS_JTAG_MSK                        0x10000000U

/* iMXRT */
/* Structure type to access the low power timer (GPT).  */
typedef struct {
  unsigned long CR;                              /* GPT Control Register, offset: 0x0 */
  unsigned long PR;                              /* GPT Prescaler Register, offset: 0x4 */
  unsigned long SR;                              /* GPT Status Register, offset: 0x8 */
  unsigned long IR;                              /* GPT Interrupt Register, offset: 0xC */
  unsigned long OCR[3];                          /* GPT Output Compare Register 1..GPT Output Compare Register 3, array offset: 0x10, array step: 0x4 */
  unsigned long ICR[2];                          /* GPT Input Capture Register 1..GPT Input Capture Register 2, array offset: 0x1C, array step: 0x4 */
  unsigned long CNT;                             /* GPT Counter Register, offset: 0x24 */
} GPT_Str;
#define GPT1_BASE     (0x401EC000U)
#define GPT1_REG      ((GPT_Str   *)  GPT1_BASE)

/* RTWDOG_Register_Accessor_Macros */
typedef struct  
{
  unsigned long CS;    /* Watchdog Control and Status Register, offset: 0x0 */
  unsigned long CNT;   /* Watchdog Counter Register, offset: 0x4 */
  unsigned long TOVAL; /* Watchdog Timeout Value Register, offset: 0x8 */
  unsigned long WIN;   /* Watchdog Window Register, offset: 0xC */
} Watchdog_Struct_RT;

#define Watchdog_BASE_RT (0x400BC000U)
#define WATCHDOG_REG_RT  ((Watchdog_Struct_RT *) Watchdog_BASE_RT)

/* SRC */
/* SRC_Register_Accessor_Macros */
#define SRC_BASE (0x400F8000u)
#define SRC_REG  ((SRC_Str *)SRC_BASE)

/** SRC - Register Layout Typedef */
volatile typedef struct {
  unsigned long SCR;             /**< SRC Control Register, offset: 0x0 */
  unsigned long SBMR1;           /**< SRC Boot Mode Register 1, offset: 0x4 */
  long SRSR;                     /**< SRC Reset Status Register, offset: 0x8 */
  unsigned char RESERVED_0[16];
  unsigned long SBMR2;          /**< SRC Boot Mode Register 2, offset: 0x1C */
  unsigned long GPR[10];        /**< SRC General Purpose Register 1..SRC General Purpose Register 10, array offset: 0x20, array step: 0x4 */
} SRC_Str;

#define SRC_SRSR_IPP_RESET_B_MSK  0x1U
#define SRC_SRSR_WDOG3_RST_B_MSK  0x80U

/*******************************************************************************
* Watchdog_test_configuration
*  
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_setup
* param[in]    pWatchdogBackup  pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_setup(WD_Test_Str* pWatchdogBackup)
{
    pWatchdogBackup->resets=0;                    /* clear the number of WD resets */
    pWatchdogBackup->wd_test_uncomplete_flag =0;  /* clear the wd_test_uncomplete flag */
  
    WATCHDOG_REG->REFRESH = 0xA602;               /* refresh sequence */
    WATCHDOG_REG->REFRESH = 0xB480;  
      
    LPTMR_REG->CSR |= LPTMR_CSR_TEN_MSK;          /* start of LPTMR */
    
    while(1)                                      /* wait for WD reset */
    {    
      LPTMR_REG->CNR = 0;                         /* write any value to synchronize the counter value */
      pWatchdogBackup->counter = LPTMR_REG->CNR;  /* store the actual value from LPTMR */      
    }
}

/*******************************************************************************
* Watchdog_test_check
* 
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wd_test_uncomplete_flag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_check
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_check(unsigned long limit_high, unsigned long limit_low, unsigned long resets_limit, unsigned long endless_loop_enable, WD_Test_Str* pWatchdogBackup)
{   
  unsigned long error_flag = 0;
    
  if ((RCM_REG->SRS0 & RCM_SRS0_WDOG_MSK) == RCM_SRS0_WDOG_MSK) /* if WD reset has occured */
  {
     pWatchdogBackup->resets +=1; /* increment the number of WD resets */ 
    /* check if the watchdog resets the system too many times */
    if (pWatchdogBackup->resets >= resets_limit) 
    {
      pWatchdogBackup->resets = resets_limit; /* to avoid overflow of reset counter */
      error_flag = 1;   /* set the error flag */   
    }
    /* check if the timeout from watchdog test corresponds to value of independent timer */
    if ((pWatchdogBackup->counter > limit_high)||(pWatchdogBackup->counter < limit_low))
    {
      error_flag = 1;   /* set the error flag */
    }    
  }
  else 
  {
    pWatchdogBackup->wd_test_uncomplete_flag = 1;              /* set the flag to 1 */
    error_flag = 1;   /* set the error flag */
  }
  /* if endless loop is enabled and some error has occured, wait in loop */  
  if (error_flag && endless_loop_enable) 
  {
    while(1)
    {
      __asm("nop"); 
    }
  }
}

/*******************************************************************************
* Watchdog_test_configuration for MKE1xF
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_setup_ke
* param[in]    pWatchdogBackup  pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_setup_ke(WD_Test_Str* pWatchdogBackup)
{
    pWatchdogBackup->resets=0;                    /* clear the number of WD resets */
    pWatchdogBackup->wd_test_uncomplete_flag =0;  /* clear the wd_test_uncomplete flag */
  
    WATCHDOG_KE_REG->CNT = 0xB480A602;            /* refresh sequence */
      
    LPTMR_REG->CSR |= LPTMR_CSR_TEN_MSK;          /* start of LPTMR */
    
    while(1)                                      /* wait for WD reset */
    {    
      LPTMR_REG->CNR = 0;                         /* write any value to synchronize the counter value */
      pWatchdogBackup->counter = LPTMR_REG->CNR;  /* store the actual value from LPTMR */      
    }
}

/*******************************************************************************
* Watchdog_test_check for MKE1xF
*
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wd_test_uncomplete_flag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_check_ke
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_check_ke(unsigned long limit_high, unsigned long limit_low, unsigned long resets_limit, unsigned long endless_loop_enable, WD_Test_Str* pWatchdogBackup)
{   
  unsigned long error_flag = 0;
    
  if ((RCM_KE_REG->SRS & RCM_KE_SRS_WDOG_MSK) == RCM_KE_SRS_WDOG_MSK) /* if WD reset has occured */
  {
     pWatchdogBackup->resets +=1; /* increment the number of WD resets */ 
    /* check if the watchdog resets the system too many times */
    if (pWatchdogBackup->resets >= resets_limit) 
    {
      pWatchdogBackup->resets = resets_limit; /* to avoid overflow of reset counter */
      error_flag = 1;   /* set the error flag */   
    }
    /* check if the timeout from watchdog test corresponds to value of independent timer */
    if ((pWatchdogBackup->counter > limit_high)||(pWatchdogBackup->counter < limit_low))
    {
      error_flag = 1;   /* set the error flag */
    }    
  }
  else 
  {
    pWatchdogBackup->wd_test_uncomplete_flag = 1;              /* set the flag to 1 */
    error_flag = 1;   /* set the error flag */
  }
  /* if endless loop is enabled and some error has occured, wait in loop */  
  if (error_flag && endless_loop_enable) 
  {
    while(1)
    {
      __asm("nop"); 
    }
  }
}

/*******************************************************************************
* Watchdog_test_configuration for MK32W
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the LPTMR which must be configured before the function call occurs. 
* Within the waiting endless loop, value from LPTMR is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_setup_k32w
* param[in]    pWatchdogBackup  pointer to structure with WD_test variables
* param[in]    pWDOG  pointer to WDOG base address
* param[in]    pLPTMR  pointer to LPTMR base address
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_setup_k32w(WD_Test_Str* pWatchdogBackup, unsigned long* pWDOG, unsigned long* pLPTMR)
{
    volatile Watchdog_Ke_Struct* pWdg = (Watchdog_Ke_Struct*)pWDOG;
    volatile LPTMR_Str* pTmr = (LPTMR_Str*)pLPTMR;
    pWatchdogBackup->resets=0;                    /* clear the number of WD resets */
    pWatchdogBackup->wd_test_uncomplete_flag =0;  /* clear the wd_test_uncomplete flag */
  
    pWdg->CNT = 0xB480A602;            /* refresh sequence */
      
    pTmr->CSR |= LPTMR_CSR_TEN_MSK;          /* start of LPTMR */
    
    while(1)                                      /* wait for WD reset */
    {    
      pTmr->CNR = 0;                         /* write any value to synchronize the counter value */
      pWatchdogBackup->counter = pTmr->CNR;  /* store the actual value from LPTMR */      
    }
}

/*******************************************************************************
* Watchdog_test_check for MK32W
*
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wd_test_uncomplete_flag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_check_k32w
* param[in]    limit_high          precalculated limit value for LPTMR counter
* param[in]    limit_low           precalculated limit value for LPTMR counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_check_k32w(unsigned long limit_high, unsigned long limit_low, unsigned long resets_limit, unsigned long endless_loop_enable, WD_Test_Str* pWatchdogBackup)
{   
  unsigned long error_flag = 0;
    
  if ((MSMC0_REG->SRS & MSMC_SRS_WDOG_MSK) == MSMC_SRS_WDOG_MSK) /* if WD reset has occured */
  {
     pWatchdogBackup->resets +=1; /* increment the number of WD resets */ 
    /* check if the watchdog resets the system too many times */
    if (pWatchdogBackup->resets >= resets_limit) 
    {
      pWatchdogBackup->resets = resets_limit; /* to avoid overflow of reset counter */
      error_flag = 1;   /* set the error flag */   
    }
    /* check if the timeout from watchdog test corresponds to value of independent timer */
    if ((pWatchdogBackup->counter > limit_high)||(pWatchdogBackup->counter < limit_low))
    {
      error_flag = 1;   /* set the error flag */
    }    
  }
  else 
  {
    pWatchdogBackup->wd_test_uncomplete_flag = 1;              /* set the flag to 1 */
    error_flag = 1;   /* set the error flag */
  }
  
  /* if endless loop is enabled and some error has occured, wait in loop */  
  if (error_flag && endless_loop_enable) 
  {
    while(1)
    {
      __asm("nop"); 
    }
  }
}

/*******************************************************************************
* Watchdog_test_configuration for RT 
*  
* This function should be called only once after POR reset. - each calling of this function causes a wd reset
* Function clears the WD resets counter to 0. No WD settings are changed here.  
* It starts the GPT which must be configured before the function call occurs. 
* Within the waiting endless loop, value from GPT is periodically stored to reserved area in RAM
* 
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_setup_RT
* param[in]    pWatchdogBackup  pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_setup_RT(WD_Test_Str* pWatchdogBackup)
{
    pWatchdogBackup->resets = 0;                  /* Clear the number of WD resets */
    pWatchdogBackup->wd_test_uncomplete_flag = 0; /* Clear the wd_test_uncomplete flag */
  
    WATCHDOG_REG_RT->CNT = 0xB480A602; /* Refresh sequence */   
    
    GPT1_REG->OCR[0] = 0xFFFFFFFF; /* Change "compare" value, this cause RESET of CNT to zero */
    
    while(1)                                       /* Wait for WD reset */
    {    
        pWatchdogBackup->counter = GPT1_REG->CNT;  /* Store value from GPT */
    }
}
/*******************************************************************************
* Watchdog_test_check_RT
* 
* function should be called after every WD reset, its calling should be handled by conditional execution in application
* if the source of reset is WD, function increments the wd_reset counter
* if the reset counter has overflowed, function will wait in endless loop if it is enabled
* if the source of reset is WD, function next check the timeout value, if it is not in range, function will wait in endless loop if it is enabled
*
* if the source of reset isn't WD or POR, function will sets the wd_test_uncomplete_flag and will wait in endless loop if it is enabled
*******************************************************************************/
/********************************************************************************************************* 
* brief        IEC60730B_CM4_CM7_watchdog_test_check_RT
* param[in]    limit_high          precalculated limit value for GPT counter
* param[in]    limit_low           precalculated limit value for GPT counter
* param[in]    resets_limit        limit value for WD resets
* param[in]    endless_loop_enable enable, or disable endless loop within the function
* param[in]    pWatchdogBackup     pointer to structure with WD_test variables
* return       void
**********************************************************************************************************/ 
void IEC60730B_CM4_CM7_watchdog_test_check_RT(unsigned long limit_high, unsigned long limit_low, unsigned long resets_limit, unsigned long endless_loop_enable, WD_Test_Str* pWatchdogBackup)
{   
    unsigned long error_flag = 0;
    if (SRC_SRSR_WDOG3_RST_B_MSK == (SRC_REG->SRSR & SRC_SRSR_WDOG3_RST_B_MSK)) /* If WD reset has occured */
    {
        /* Clear wdog3_rst_b bit */
        SRC_REG->SRSR = SRC_SRSR_WDOG3_RST_B_MSK;
      
        pWatchdogBackup->resets +=1; /* Increment the number of WD resets */ 
        /* Check if the watchdog resets the system too many times */
        if (pWatchdogBackup->resets >= resets_limit) 
        {
            pWatchdogBackup->resets = resets_limit; /* To avoid overflow of reset counter */
            error_flag = 1;   /* Set the error flag */   
        }
        
        /* Check if the timeout from watchdog test corresponds to value of independent timer */
        if ((pWatchdogBackup->counter > limit_high) || (pWatchdogBackup->counter < limit_low))
        {
            error_flag = 1;   /* Set the error flag */
        }    
    }
    else 
    {     
        pWatchdogBackup->wd_test_uncomplete_flag = 1; /* Set the flag to 1 */
        error_flag = 1;   /* Set the error flag */
    }
    
    /* If endless loop is enabled and some error has occured, wait in loop */  
    if (error_flag && endless_loop_enable) 
    {
        while(1)
        {
            __asm("nop"); 
        }
    }
}
