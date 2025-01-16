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
#include "fsl_rgpio.h"
#include "fsl_lpuart.h"
#include "m1_sm_snsless_enc.h"

#include "freemaster_serial_lpuart.h"
#include "board.h"
#include "mid_sm_states.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

#define DAPENG_TEST /* Dapeng test */

/* Example's feature set in form of bits inside ui16featureSet.
   This feature set is expected to be growing over time.
   ... | FEATURE_S_RAMP | FEATURE_FIELD_WEAKENING | FEATURE_ENC
*/
#define FEATURE_ENC (1)               /* Encoder feature flag */
#define FEATURE_FIELD_WEAKENING (0)   /* Field weakening feature flag */
#define FEATURE_S_RAMP (0)            /* S-ramp feature flag */

#define FEATURE_SET (FEATURE_ENC << (0) | \
                     FEATURE_FIELD_WEAKENING << (1) | \
                     FEATURE_S_RAMP << (2))

/*! @brief The UART to use for FreeMASTER communication */
#define BOARD_FMSTR_UART_PORT LPUART1
#define BOARD_FMSTR_UART_BAUDRATE 115200U

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
/* ADC COCO interrupt */
RAM_FUNC_LIB
void ADC1_IRQHandler(void);
/* TMR1 reload ISR called with 1ms period */
RAM_FUNC_LIB
void TMR1_IRQHandler(void);
/* SW8 Button interrupt handler */
RAM_FUNC_LIB
void GPIO1_0_IRQHandler(void);
/* Demo Speed Stimulator */
RAM_FUNC_LIB
static void DemoSpeedStimulator(void);
/* Demo Position Stimulator */
RAM_FUNC_LIB
static void DemoPositionStimulator(void);

static void BOARD_InitUART(uint32_t u32BaudRate);
static void BOARD_InitSysTick(void);
static void BOARD_InitGPIO(void);
static void Application_Control_BL(void);

lpadc_conv_result_t g_LpadcResultConfigStruct;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

#ifdef DAPENG_TEST
/* ISR counters */
uint32_t ui32FastIsrCount = 0U;
uint32_t ui32SlowIsrCount = 0U;
#endif

/* Demo mode enabled/disabled */
bool_t bDemoModeSpeed    = FALSE;
bool_t bDemoModePosition = FALSE;

/* Counters used for demo mode */
static uint32_t ui32SpeedStimulatorCnt    = 0U;
static uint32_t ui32PositionStimulatorCnt = 0U;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../examples/evkmimxrt1180/demo_apps/mc_pmsm/pmsm_enc",         /* User Path 1- the highest priority */
    "",       /* User Path 2 */
    "evkmimxrt1180", /* board id */
    "pmsm_enc", /* example id */
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
    bDemoModePosition = FALSE;

    /* Init board hardware. */
    BOARD_Init();

    /* Init UART for FreeMaster communication */
    BOARD_InitUART(BOARD_FMSTR_UART_BAUDRATE);

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* Init peripheral motor control driver for motor M1 */
    MCDRV_Init_M1();

    /* FreeMaster init */
    FMSTR_Init();

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

    /* Enable PWM clock */
    g_sM1Pwm3ph.pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_RUN(0xF);

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
void ADC1_IRQHandler(void)
{
    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();

    TP0_ON();

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

#ifdef DAPENG_TEST
    /* Increment ISR counter */
    ui32FastIsrCount++;

    if(ui32FastIsrCount > 16000U)
    {
       ui32FastIsrCount = 0;
    }

#endif

    TP0_OFF();

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
    static int16_t ui16i = 0;

    TP2_ON();

    /* M1 Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);

    /* If in STOP state turn on LED */
    if (M1_GetAppState() == 2)
    {
        USER_LED_OFF();
    }

    /* If in FAULT state LED blinking*/
    else if (M1_GetAppState() == 0)
    {
        bDemoModeSpeed    = FALSE;
        bDemoModePosition = FALSE;

        if (ui16i-- < 0)
        {
            USER_LED_TOGGLE();
            ui16i = 125;
        }
    }

    /* If in RUN or INIT state turn on LED */
    else
    {
        USER_LED_ON();
    }

    /* Demo speed stimulator */
    DemoSpeedStimulator();

    /* Demo position stimulator */
    DemoPositionStimulator();

#ifdef DAPENG_TEST
    /* Increment ISR counter */
    ui32SlowIsrCount++;

    if(ui32SlowIsrCount > 1000U)
    {
       ui32SlowIsrCount = 0;
    }

#endif

    TP2_OFF();

    /* Clear the CSCTRL0[TCF1] flag */
    TMR1->CHANNEL[0].CSCTRL |= TMR_CSCTRL_TCF1(0x00);
    TMR1->CHANNEL[0].CSCTRL &= ~(TMR_CSCTRL_TCF1_MASK);

    /* Clear the CSCTRL0[TCF] flag */
    TMR1->CHANNEL[0].SCTRL &= ~(TMR_SCTRL_TCF_MASK);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
 * @brief   SW8 Button interrupt handler
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
void GPIO1_0_IRQHandler(void)
{
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

    /* Clear external interrupt flag. */
    RGPIO_ClearPinsInterruptFlags(BOARD_USER_BUTTON_GPIO, kRGPIO_InterruptOutput0, 1U << BOARD_USER_BUTTON_GPIO_PIN);

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
    {
    	ui32ButtonFilter++;
    }

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
 * @brief   DemoPositionStimulator
 *           - When demo mode is enabled it changes the required position according
 *             to predefined profile
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
static void DemoPositionStimulator(void)
{
    if (bDemoModePosition)
    {
        ui32PositionStimulatorCnt++;
        switch (ui32PositionStimulatorCnt)
        {
            case 1:
                M1_SetAppSwitch(0);
                break;
            case 2:
                g_sM1Drive.eControl                  = kControlMode_PositionFOC;
                g_sM1Drive.sMCATctrl.ui16PospeSensor = MCAT_ENC_CTRL;
                M1_SetAppSwitch(1);
                break;
            case 10:
                M1_SetPosition(ACC32(20));
                break;
            case 3000:
                M1_SetPosition(ACC32(10));
                break;
            case 6000:
                M1_SetPosition(ACC32(20));
                break;
            case 9000:
                M1_SetPosition(ACC32(0));
                break;
            case 12000:
                M1_SetPosition(ACC32(20));
                break;
            case 15000:
                M1_SetPosition(ACC32(0));
                break;
            case 18000:
                ui32PositionStimulatorCnt = 0;
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
    /* MPU configuration. */
    BOARD_ConfigMPU();
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

    /* Define the init structure for the input switch pin */
    rgpio_pin_config_t sw_config = {
        kRGPIO_DigitalInput,
        0,
    };

    /* Workaround: Disable interrupt which might be enabled by ROM. */
    RGPIO_SetPinInterruptConfig(RGPIO1, 9U, kRGPIO_InterruptOutput0, kRGPIO_InterruptOrDMADisabled);
    NVIC_ClearPendingIRQ(GPIO1_0_IRQn);

    /* Init input switch GPIO. */
    RGPIO_SetPinInterruptConfig(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, kRGPIO_InterruptOutput0, kRGPIO_InterruptFallingEdge);
    RGPIO_PinInit(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &sw_config);

    /* Enable GPIO pin interrupt for SW8 button */
    EnableIRQ(BOARD_USER_BUTTON_IRQ);
    NVIC_SetPriority(BOARD_USER_BUTTON_IRQ, BOARD_USER_BUTTON_PRIORITY);
}

/*!
 *@brief      Initialization of the UART module
 *
 *@param      u32BaudRate         Baud rate
 *
 *@return     none
 */
static void BOARD_InitUART(uint32_t u32BaudRate)
{
    lpuart_config_t config;

    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_FMSTR_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    LPUART_Init(BOARD_FMSTR_UART_PORT, &config, BOARD_DebugConsoleSrcFreq());

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(BOARD_FMSTR_UART_PORT);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(BOARD_UART_IRQ);
    EnableGlobalIRQ(0);
#endif
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
