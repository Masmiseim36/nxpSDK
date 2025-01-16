/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "mc_periph_init.h"
#include "freemaster.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_gpio.h"
#include "fsl_lpuart.h"
#include "m1_sm_snsless.h"
#include "mc_periph_init.h"
#include "board.h"


#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }

/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

/* Example's feature set in form of bits inside ui16featureSet.
   This feature set is expected to be growing over time.
   ... | FEATURE_S_RAMP | FEATURE_FIELD_WEAKENING | FEATURE_ENC
*/
#define FEATURE_ENC (0)               /* Encoder feature flag */
#define FEATURE_FIELD_WEAKENING (0)   /* Field weakening feature flag */
#define FEATURE_S_RAMP (0)            /* S-ramp feature flag */

#define FEATURE_SET (FEATURE_FIELD_WEAKENING << (1) | \
                     FEATURE_S_RAMP << (2))

#define BOARD_USER_BUTTON_PRIORITY 4

   /* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1)   \
    uint32_t val  = SysTick->VAL;  \
    uint32_t load = SysTick->LOAD; \
    par1          = load - val

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* CPU load measurement using Systick*/
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoMode = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt = 0U;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../examples/evkbmimxrt1170/demo_apps/mc_bldc/cm7",     /* User Path 1- the highest priority */
    "",       /* User Path 2 */
    "evkbmimxrt1170",  /* board id */
    "bldc",             /* example id */
    MCRSP_VER,          /* sw version */
    FEATURE_SET,        /* example's feature-set */
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Init SDK HW */
static void BOARD_Init(void);
void BOARD_InitSysTick(void);

/* ADC COCO interrupt */
RAM_FUNC_LIB
void ADC_ETC_IRQ0_IRQHandler(void);

/* TMR1 reload ISR called with 1ms period */
RAM_FUNC_LIB
void TMR1_IRQHandler(void);

/* Commutation Timer ISR */
RAM_FUNC_LIB
void PWM1_3_IRQHandler(void);

/* Button interupt handler */
RAM_FUNC_LIB
void GPIO13_Combined_0_31_IRQHandler(void);

/* Demo Speed Stimulator */
RAM_FUNC_LIB
void DemoSpeedStimulator(void);
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   Application main function processing peripheral function calling and
 *          infinite loop
 *
 * @param   void
 *
 * @return  none
 */
int main(void)
{
    uint32_t ui32PrimaskReg;
    
    /*Accessing ID structure to prevent optimization*/
    g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    /* Disable demo mode after reset */
    bDemoMode              = FALSE;

    /* Init board hardware. */
    BOARD_Init();

    /* Initialize peripheral motor control driver for motor M1*/
    MCDRV_Init_M1();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* Turn off application */
    M1_SetAppSwitch(0);

    /* Enable interrupts */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Infinite loop */
    while (1)
    {
        FMSTR_Poll();
    }
}

/*!
 * @brief   ADC conversion complete ISR called with 100us period processes
 *           - motor M1 fast application machine function
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
void ADC_ETC_IRQ0_IRQHandler(void)
{
     /* Start CPU tick number couting */
     SYSTICK_START_COUNT();

     /* State machine */
     SM_StateMachineFast(&g_sM1Ctrl);

     /* stop CPU tick number couting and store actual and maximum ticks */
     SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
     g_ui32MaxNumberOfCycles =
         g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    /* ADC_ETC flags clearing */
    ADC_ETC_ClearInterruptStatusFlags(ADC_ETC, kADC_ETC_Trg0TriggerSource, kADC_ETC_Done0StatusFlagMask);
    ADC_ETC_ClearInterruptStatusFlags(ADC_ETC, kADC_ETC_Trg4TriggerSource, kADC_ETC_Done0StatusFlagMask);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}


/*!
 * @brief   PWM1_3 ISR called asynchronously
 *           - forced motor commutation
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
void PWM1_3_IRQHandler(void)
{
  /* Asynchronous time event processing */
  M1_TimeEvent();

  /* Clear flag */
  PWM1->SM[3].STS = PWM_STS_CMPF(0x4);

  /* Update register values */
  PWM1->MCTRL = (PWM1->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0x8);

  /* Add empty instructions for correct interrupt flag clearing */
  M1_END_OF_ISR;
}


/*!
 * @brief   TMR1 reload ISR called with 1ms period and processes following functions:
 *           - motor M1 slow application machine function
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
void TMR1_IRQHandler(void)
{
    /* M1 Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);

    /* If in FAULT state turn off DemoMode*/
    if (M1_GetAppState() == 0)
    {
        bDemoMode    = FALSE;
    }

    /* Demo speed stimulator */
    DemoSpeedStimulator();

    /* Clear the CSCTRL0[TCF1] flag */
    TMR1->CHANNEL[0].CSCTRL |= TMR_CSCTRL_TCF1(0x00);
    TMR1->CHANNEL[0].CSCTRL &= ~(TMR_CSCTRL_TCF1_MASK);

    /* Clear the CSCTRL0[TCF] flag */
    TMR1->CHANNEL[0].SCTRL &= ~(TMR_SCTRL_TCF_MASK);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
 * @brief   SW7 Button interrupt handler
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
void GPIO13_Combined_0_31_IRQHandler(void)
{
    if (GPIO13->ISR & GPIO_ICR1_ICR0_MASK)
    {
        /* Clear the flag */
        GPIO13->ISR |= GPIO_ISR_ISR(1);

        /* Proceed only if pressing longer than timeout */
        if (ui32ButtonFilter > 300U)
        {
            ui32ButtonFilter = 0U;

            /* Speed demo */
            if (bDemoMode)
            {
                /* Stop application */
                M1_SetSpeed(0);
                M1_SetAppSwitch(0);
                bDemoMode = FALSE;
            }
            else
            {
                /* Start application */
                M1_SetAppSwitch(1);
                bDemoMode = TRUE;
                ui32SpeedStimulatorCnt = 0U;
            }
        }
    }

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
 * @brief   DemoSpeedStimulator
 *           - When demo mode is enabled it changes the required speed according
 *             to predefined profile
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
void DemoSpeedStimulator(void)
{
   /* Increase push button pressing counter  */
   if (ui32ButtonFilter < 1000U)
   {
      ui32ButtonFilter++;
   }

   if (bDemoMode)
   {
       ui32SpeedStimulatorCnt++;
       switch (ui32SpeedStimulatorCnt)
       {
            case 10U:
               M1_SetAppSwitch(FALSE);
               break;
            case 20U:
               M1_SetAppSwitch(TRUE);
               break;
            case 1000U:
                M1_SetSpeed(3300);
                break;
            case 5000U:
                M1_SetSpeed(6500);
                break;
            case 10000U:
                M1_SetSpeed(-3300);
                break;
            case 15000U:
                M1_SetSpeed(-6500);
                break;
            case 19800U:
                M1_SetSpeed(0);
                break;
            case 20000U:
                ui32SpeedStimulatorCnt = 0U;
                M1_SetAppSwitch(FALSE);
                break;
            default:
            	;
            	break;
       }
   }
}

/*!
 *@brief      Initialization of the Clocks and Pins
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_Init(void)
{
    /* MPU configuration. */
    BOARD_ConfigMPU();
    /* Init board hardware. */
    BOARD_InitBootPins();
    /* Initialize clock configuration */
    BOARD_InitBootClocks();
    /* Init peripherals set in peripherals file */
    BOARD_InitBootPeripherals();
}

/*!
 *@brief      SysTick initialization for CPU cycle measurement
 *
 *@param      none
 *
 *@return     none
 */
void BOARD_InitSysTick(void)
{
    /* Initialize SysTick core timer to run free */
    /* Set period to maximum value 2^24*/
    SysTick->LOAD = 0xFFFFFF;

    /*Clock source - System Clock*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    /*Start Sys Timer*/
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
