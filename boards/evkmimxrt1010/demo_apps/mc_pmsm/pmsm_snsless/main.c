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

#include "freemaster_serial_lpuart.h"
#include "board.h"
#include "mid_sm_states.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

/* Example's feature set in form of bits inside ui16featureSet.
   This feature set is expected to be growing over time.
   ... | FEATURE_S_RAMP | FEATURE_FIELD_WEAKENING | FEATURE_ENC
*/
#define FEATURE_ENC (0)               /* Encoder feature flag */
#define FEATURE_FIELD_WEAKENING (0)   /* Field weakening feature flag */
#define FEATURE_S_RAMP (0)            /* S-ramp feature flag */

#define FEATURE_SET (FEATURE_ENC << (0) | \
                     FEATURE_FIELD_WEAKENING << (1) | \
                     FEATURE_S_RAMP << (2))


#define BOARD_USER_BUTTON_PRIORITY 4

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1)   \
    uint32_t val  = SysTick->VAL;  \
    uint32_t load = SysTick->LOAD; \
    par1          = load - val

/* Three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }


/* Init SDK HW */
static void BOARD_Init(void);
/* ADC_ETC interrupt */
RAM_FUNC_LIB 
void ADC_ETC_IRQ0_IRQHandler(void);
/* PIT reload ISR called with 1ms period */
RAM_FUNC_LIB 
void PIT_IRQHandler(void);
/* User button interrupt handler */
RAM_FUNC_LIB 
void GPIO2_Combined_0_15_IRQHandler(void);
/* Demo Speed Stimulator */
RAM_FUNC_LIB 
static void DemoSpeedStimulator(void);

static void BOARD_InitSysTick(void);
static void BOARD_InitGPIO(void);
RAM_FUNC_LIB
static void Application_Control_BL(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoModeSpeed    = FALSE;

/* Counters used for demo mode */
static uint32_t ui32SpeedStimulatorCnt    = 0U;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "evkmimxrt1010", /* board id */
    "pmsm_snsless", /* example id */
    MCRSP_VER,      /* sw version */
    FEATURE_SET,    /* example's feature-set */
};

mid_app_cmd_t g_eMidCmd;                  /* Start/Stop MID command */
ctrl_m1_mid_t g_sSpinMidSwitch;           /* Control Spin/MID switching */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    /*Accessing ID structure to prevent optimization*/
    g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    /* Disable demo mode after reset */
    bDemoModeSpeed    = FALSE;

    /* Init board hardware. */
    BOARD_Init();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* Init peripheral motor control driver for motor M1 */
    MCDRV_Init_M1();

    /* Turn off application */
    M1_SetAppSwitch(FALSE);

    /* Init MID state machine - call before the spin state machine */
    g_sSpinMidSwitch.eAppState = kAppStateMID;         
    
    if(g_sSpinMidSwitch.eAppState == kAppStateMID)
    {
      MID_Init_AR();
    }
    
    /* Spin state machine is default */
    g_sSpinMidSwitch.eAppState = kAppStateSpin;

    /* Enable interrupts */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Infinite loop */
    while (1)
    {
		Application_Control_BL();

        /* FreeMASTER Polling function */
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

    switch(g_sSpinMidSwitch.eAppState)
    {
    case kAppStateSpin:
        /* M1 state machine */
        SM_StateMachineFast(&g_sM1Ctrl);
      break;
    default:
        /* MID state machine */
        MID_ProcessFast_FL();
      break;
    }
          
    /* Stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles =
        g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);
    
    /* Clear ADC_ETC interrupt flag - ADC_ETC_ClearInterruptStatusFlags(ADC_ETC, kADC_ETC_Trg0TriggerSource, kADC_ETC_Done0StatusFlagMask); */ 
    ADC_ETC->DONE0_1_IRQ = ((uint32_t)ADC_ETC_DONE0_1_IRQ_TRIG0_DONE0_MASK << (uint32_t)kADC_ETC_Trg0TriggerSource);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
 * @brief   PIT reload ISR called with 1ms period and processes following functions:
 *           - motor M1 slow application machine function
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
void PIT_IRQHandler(void)
{
    static int16_t ui16i = 0;

    /* M1 Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);
    
    /* If in STOP state turn on LED */
    if (M1_GetAppState() == 2)
    {
        /* User LED off - USER_LED_OFF(); */
        BOARD_USER_LED_GPIO->DR_CLEAR = 1U << BOARD_USER_LED_GPIO_PIN;
    }

    /* If in FAULT state LED blinking*/
    else if (M1_GetAppState() == 0)
    {
        bDemoModeSpeed = FALSE;

        if (ui16i-- < 0)
        {
            /* User LED toggle - USER_LED_TOGGLE(); */ 
            if ((0x1 ^ (((BOARD_USER_LED_GPIO->DR) >> BOARD_USER_LED_GPIO_PIN) & 0x1U)) == 0U)
            {
                BOARD_USER_LED_GPIO->DR_CLEAR = (1UL << BOARD_USER_LED_GPIO_PIN);
            }
            else
            {
                BOARD_USER_LED_GPIO->DR_SET = (1UL << BOARD_USER_LED_GPIO_PIN);
            }

            ui16i = 125;
        }
    }

    /* If in RUN or INIT state turn on LED */
    else
    {
        /* User LED on - USER_LED_ON(); */
        BOARD_USER_LED_GPIO -> DR_SET = 1U << BOARD_USER_LED_GPIO_PIN;        
    }

    /* Demo speed stimulator */
    DemoSpeedStimulator();

    /* Clear interrupt flag - PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag); */   
    PIT->CHANNEL[kPIT_Chnl_0].TFLG = kPIT_TimerFlag;

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
 * @brief   SW4 Button interrupt handler
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
void GPIO2_Combined_0_15_IRQHandler(void)
{
    if (GPIO2->ISR & GPIO_ICR1_ICR0_MASK)
    {
        /* Clear the flag */
        GPIO2->ISR |= GPIO_ISR_ISR(1);

        /* Proceed only if pressing longer than timeout */
        if (ui32ButtonFilter > 300)
        {
            ui32ButtonFilter = 0;

            /* Speed demo */
            if (bDemoModeSpeed)
            {
                /* Stop application */
                M1_SetSpeed(0);
                M1_SetAppSwitch(0);
                bDemoModeSpeed = FALSE;
            }
            else
            {
                /* Start application */
                M1_SetAppSwitch(1);
                bDemoModeSpeed         = TRUE;
                ui32SpeedStimulatorCnt = 0;
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
static void DemoSpeedStimulator(void)
{
    /* Increase push button pressing counter  */
    if (ui32ButtonFilter < 1000)
        ui32ButtonFilter++;

    if (bDemoModeSpeed)
    {
        ui32SpeedStimulatorCnt++;
        switch (ui32SpeedStimulatorCnt)
        {
            case 10:
                M1_SetAppSwitch(0);
                break;
            case 20:
                g_sM1Drive.eControl                  = kControlMode_SpeedFOC;
                g_sM1Drive.sMCATctrl.ui16PospeSensor = MCAT_SENSORLESS_CTRL;
                M1_SetAppSwitch(1);
                break;
            case 1000:
                M1_SetSpeed(1000.0F);
                break;
            case 5000:
                M1_SetSpeed(2000.0F);
                break;
            case 10000:
                M1_SetSpeed(-1000.0F);
                break;
            case 15000:
                M1_SetSpeed(-2000.0F);
                break;
            case 19800:
                M1_SetSpeed(0.0F);
                break;
            case 20000:
                ui32SpeedStimulatorCnt = 0;
                M1_SetAppSwitch(0);
                break;
            default:
            	;
                break;
        }
    }
}

/*!
 * @brief   Application_Control_BL
 *           - Control switching between Spin and MID
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
static void Application_Control_BL(void)
{
  switch(g_sSpinMidSwitch.eAppState)
  {
    case kAppStateSpin:
        /* M1 state machine */
        if(g_sSpinMidSwitch.bCmdRunMid == TRUE)
        {
          if((kSM_AppStop == M1_GetAppState()) && (FALSE == M1_GetAppSwitch()) )
          {
            MID_Init_AR();
            g_sSpinMidSwitch.sFaultCtrlM1_Mid &= ~(FAULT_APP_SPIN);
            g_eMidCmd = kMID_Cmd_Stop;                          /* Reset MID control command */
            g_sSpinMidSwitch.eAppState = kAppStateMID;          /* MID routines will be processed */ 
          }
          else
            g_sSpinMidSwitch.sFaultCtrlM1_Mid |= FAULT_APP_SPIN;
                           
          g_sSpinMidSwitch.bCmdRunMid = FALSE;                  /* Always clear request */
        }
        
        g_sSpinMidSwitch.bCmdRunM1 = FALSE;
        break;
    default:
        /* MID state machine */
        if(g_sSpinMidSwitch.bCmdRunM1 == TRUE)
        {
          if((g_eMidCmd == kMID_Cmd_Stop) && (kMID_Stop == MID_GetActualState()))
          {
            g_sSpinMidSwitch.sFaultCtrlM1_Mid &= ~(FAULT_APP_MID);
            g_sM1Ctrl.eState = kSM_AppInit;                      /* Set Init state for M1 state machine */
            g_sSpinMidSwitch.eAppState = kAppStateSpin;          /* Switch application state to Spin */
          }
          else
            g_sSpinMidSwitch.sFaultCtrlM1_Mid |= FAULT_APP_MID;
          
           /* Always clear request */ 
          g_sSpinMidSwitch.bCmdRunM1 = FALSE;   
          g_sSpinMidSwitch.bCmdRunMid = FALSE;
          break;
        }
        
        g_sSpinMidSwitch.bCmdRunMid = FALSE;
        MID_Process_BL(&g_eMidCmd);
        break;
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
    /* Init board hardware. */
    BOARD_InitBootPins();
    /* Initialize clock configuration */
    BOARD_InitBootClocks();
    /* Init peripherals set in peripherals file */
    BOARD_InitBootPeripherals();
    /* Init GPIO pins */
    BOARD_InitGPIO();
}

/*!
 * @brief   static void BOARD_InitGPIO(void)
 *           - Initialization of the GPIO peripherals
 *
 * @param   void
 *
 * @return  none
 */
static void BOARD_InitGPIO(void)
{
    /* LED pin configuration */
    const gpio_pin_config_t output_pin_config = {
        kGPIO_DigitalOutput, /* Set current pin as digital output */
        (uint8_t)0U          /* Set default logic low */
    };

    /* SW4 Button configuration */
    const gpio_pin_config_t user_button_config = {
        kGPIO_DigitalInput,  /* Set pin as digital input */
        0,                   /* Set default output logic, which has no use in input  */
        kGPIO_IntFallingEdge /* Set pin interrupt mode: falling-edge sensitive */
    };

    /* Enable port for USER LED */
    GPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &output_pin_config);
    GPIO_PortClear(BOARD_USER_LED_GPIO, 1U << BOARD_USER_LED_GPIO_PIN);

    /* Enable GPIO pin interrupt for SW4 button */
    EnableIRQ(BOARD_USER_BUTTON_IRQ);
    GPIO_PinInit(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &user_button_config);
    GPIO_PortEnableInterrupts(BOARD_USER_BUTTON_GPIO, 1U << BOARD_USER_BUTTON_GPIO_PIN);
    NVIC_SetPriority(BOARD_USER_BUTTON_IRQ, BOARD_USER_BUTTON_PRIORITY);
}


/*!
 *@brief      SysTick initialization for CPU cycle measurement
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_InitSysTick(void)
{
    /* Initialize SysTick core timer to run free */
    /* Set period to maximum value 2^24*/
    SysTick->LOAD = 0xFFFFFF;

    /*Clock source - System Clock*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    /*Start Sys Timer*/
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}