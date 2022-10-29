/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "freemaster.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_port.h"
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

/* Macro for correct Cortex CM0 / CM4 end of interrupt */
#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1)   \
    uint32_t val  = SysTick->VAL;  \
    uint32_t load = SysTick->LOAD; \
    par1          = load - val

static void DemoSpeedStimulator(void);

/* Init HW */
static void BOARD_Init(void);
static void BOARD_InitGPIO(void);
static void BOARD_InitSysTick(void);

static void Application_Control_BL(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoModeSpeed = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt    = 0U;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "hvpkv31f120m", /* board id */
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
 * @brief   Application main function processing peripheral function calling and
 *          infinite loop
 *
 * @param   void
 *
 * @return  none
 */
int main(void)
{
    /*Accessing ID structure to prevent optimization*/
    g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    /* Disable demo mode after reset */
    bDemoModeSpeed         = FALSE;
    ui32SpeedStimulatorCnt = 0U;

    /* Init board hardware. */
    BOARD_Init();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

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

    /* Enable interrupts  */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Infinite loop */
    while (1)
    {
        Application_Control_BL();

        /* FreeMASTER Polling function */
        FMSTR_Poll();
    }
}

/* ADC1_IRQn interrupt handler */
void FAST_LOOP_IRQHANDLER(void) {
    /* ADC conversion complete (fast loop period) interrupt.
    * Processes motor M1 fast application machine function.
    * Demo mode if enabled. */

    /* Disable trigger for PDB */
    PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0xF); /* PDB Trigger off (software trigger) */
    PDB0->SC |= PDB_SC_LDOK_MASK;  /* LDOK */

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

    /* stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles =
        g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;

}

/* FTM2_IRQn interrupt handler */
void SLOW_LOOP_IRQHANDLER(void) {
    /* Slow Loop periodic interrupt period.
    * Processes motor M1 slow application machine function. */

    /* Demo speed stimulator */
    DemoSpeedStimulator();

    /* Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);

    /* Clear the TOF flag */
    FTM2->SC &= ~(FTM_SC_TOF_MASK);

    /* Add instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;

}

/* PDB0_IRQn interrupt handler */
void PDB_ERROR_IRQHANDLER(void) {
    /* Handling the PDB error interrupt.
    * Reinitializes the PDB module */

    /* PDB Error interrupt */
    if ((PDB0->CH[0].S & PDB_S_ERR_MASK) || (PDB0->CH[1].S & PDB_S_ERR_MASK)) {
        PDB0->SC &= (~PDB_SC_PDBEN_MASK);   /* Disable PDB */
        PDB0->CH[0].S &= (~PDB_S_CF_MASK);  /* Clear flag on channel 0 */
        PDB0->CH[0].S &= (~PDB_S_ERR_MASK); /* Clear error on channel 0 */
        PDB0->CH[1].S &= (~PDB_S_CF_MASK);  /* Clear flag on channel 1 */
        PDB0->CH[1].S &= (~PDB_S_ERR_MASK); /* Clear error on channel 1 */
        PDB0->SC |= PDB_SC_PDBEN_MASK;      /* Enable PDB */
    }
    /* PDB delay interrupt */
    else{
        PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0x8); /* PDB Trigger on (FTM2 init trigger) */
        PDB0->SC |= PDB_SC_LDOK_MASK;  /* LDOK */
        PDB0->SC &= (~PDB_SC_PDBIF_MASK);  /* Clear PDB interrupt flag */
    }

    /* Add instructions for correct interrupt flag clearing */
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
static void DemoSpeedStimulator(void)
{
    /* Increment push button pressing counter  */
    if (ui32ButtonFilter < 1000)
        ui32ButtonFilter++;

    if (bDemoModeSpeed)
    {
        ui32SpeedStimulatorCnt++;
        switch (ui32SpeedStimulatorCnt)
        {
            case 100:
                M1_SetSpeed(1000.0F);
                break;
            case 3000:
                M1_SetSpeed(2000.0F);
                break;
            case 6000:
                M1_SetSpeed(4000.0F);
                break;
            case 9000:
                M1_SetSpeed(-1000.0F);
                break;
            case 12000:
                M1_SetSpeed(-2000.0F);
                break;
            case 15000:
                M1_SetSpeed(-4000.0F);
                break;
            case 18000:
                ui32SpeedStimulatorCnt = 0;
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
    /* Initialize clock configuration */
    BOARD_BootClockHSRUN();
    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();

    /* Init GPIO pins */
    BOARD_InitGPIO();

    /* Init peripherals set in peripherals file */
    BOARD_InitBootPeripherals();
}

/*!
 *@brief      Initialization of the GPIO pins
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_InitGPIO(void)
{
    /* LED pin configuration */
    const gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput, /* Set current pin as digital output */
        (uint8_t)0U          /* Set default logic high */
    };

    /* enable port for HVP GREEN LED */
    GPIO_PinInit(GPIOB, 19U, &led_config);

    /* enable port for Braking resistor */
    GPIO_PinInit(GPIOC, 0U, &led_config);

    /* enable port for Inrush relay */
    GPIO_PinInit(GPIOC, 13U, &led_config);
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

