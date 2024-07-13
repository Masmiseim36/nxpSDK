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

/* Example's feature set in form of bits inside ui16featureSet.
   This feature set is expected to be growing over time.
   ... | FEATURE_S_RAMP | FEATURE_FIELD_WEAKENING | FEATURE_ENC
*/
#define FEATURE_ENC             (1) /* Encoder feature flag */
#define FEATURE_FIELD_WEAKENING (0) /* Field weakening feature flag */
#define FEATURE_S_RAMP          (0) /* S-ramp feature flag */

#define FEATURE_SET (FEATURE_ENC << (0) | FEATURE_FIELD_WEAKENING << (1) | FEATURE_S_RAMP << (2))

/*! @brief The UART to use for FreeMASTER communication */
#define BOARD_FMSTR_UART_PORT     LPUART1
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

/* ADC COCO interrupt */
RAM_FUNC_LIB
void ADC1_IRQHandler(void);

/* Init UART for FreeMASTER communication */
static void BOARD_InitUART(uint32_t u32BaudRate);

static void BOARD_InitSysTick(void);

lpadc_conv_result_t g_LpadcResultConfigStruct;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../boards/src/ecat_examples/servo_motor",   /* User Path 1- the highest priority */
    "../../../boards/evkmimxrt1180/ecat_examples/servo_motor/cm7",   /* User Path 2 */
    "evkmimxrt1180", /* board id */
    "pmsm_enc",      /* example id */
    MCRSP_VER,       /* sw version */
    FEATURE_SET,     /* example's feature-set */
};

mid_app_cmd_t g_eMidCmd;        /* Start/Stop MID command */
ctrl_m1_mid_t g_sSpinMidSwitch; /* Control Spin/MID switching */

/* Demo mode enabled/disabled */
volatile bool_t bDemoModeSpeed;
volatile bool_t bDemoModePosition;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief servo_motor_init function
 */
int servo_motor_init(void)
{
    /* Accessing variables to prevent optimization */
    bDemoModeSpeed = FALSE;
    bDemoModePosition = FALSE;
    g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* Init peripheral motor control driver for motor M1 */
    MCDRV_Init_M1();
    

   /* Init UART for FreeMaster communication */
    BOARD_InitUART(BOARD_FMSTR_UART_BAUDRATE);

    /* FreeMaster init */
    FMSTR_Init();

    /* Turn off application */
    M1_SetAppSwitch(FALSE);

    /* Init MID state machine - call before the spin state machine */
    g_sSpinMidSwitch.eAppState = kAppStateMID;

    if (g_sSpinMidSwitch.eAppState == kAppStateMID)
    {
        MID_Init_AR();
    }

    /* Spin state machine is default */
    g_sSpinMidSwitch.eAppState = kAppStateSpin;

    /* Enable interrupts */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Enable PWM clock */
    PWM1->MCTRL |= PWM_MCTRL_RUN(0xF);
    
    return 0;
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

    switch (g_sSpinMidSwitch.eAppState)
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
void SM_StateMachineSlowTask(void)
{
    TP2_ON();

    /* M1 Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);

    TP2_OFF();
}

/*!
 * @brief   Application_Control_BL
 *           - Control switching between Spin and MID
 *
 * @param   void
 *
 * @return  none
 */
void Application_Control_BL(void)
{
    switch (g_sSpinMidSwitch.eAppState)
    {
        case kAppStateSpin:
            /* M1 state machine */
            if (g_sSpinMidSwitch.bCmdRunMid == TRUE)
            {
                if ((kSM_AppStop == M1_GetAppState()) && (FALSE == M1_GetAppSwitch()))
                {
                    MID_Init_AR();
                    g_sSpinMidSwitch.sFaultCtrlM1_Mid &= ~(FAULT_APP_SPIN);
                    g_eMidCmd                  = kMID_Cmd_Stop; /* Reset MID control command */
                    g_sSpinMidSwitch.eAppState = kAppStateMID;  /* MID routines will be processed */
                }
                else
                    g_sSpinMidSwitch.sFaultCtrlM1_Mid |= FAULT_APP_SPIN;

                g_sSpinMidSwitch.bCmdRunMid = FALSE; /* Always clear request */
            }

            g_sSpinMidSwitch.bCmdRunM1 = FALSE;
            break;
        default:
            /* MID state machine */
            if (g_sSpinMidSwitch.bCmdRunM1 == TRUE)
            {
                if ((g_eMidCmd == kMID_Cmd_Stop) && (kMID_Stop == MID_GetActualState()))
                {
                    g_sSpinMidSwitch.sFaultCtrlM1_Mid &= ~(FAULT_APP_MID);
                    g_sM1Ctrl.eState           = kSM_AppInit;   /* Set Init state for M1 state machine */
                    g_sSpinMidSwitch.eAppState = kAppStateSpin; /* Switch application state to Spin */
                }
                else
                    g_sSpinMidSwitch.sFaultCtrlM1_Mid |= FAULT_APP_MID;

                /* Always clear request */
                g_sSpinMidSwitch.bCmdRunM1  = FALSE;
                g_sSpinMidSwitch.bCmdRunMid = FALSE;
                break;
            }

            g_sSpinMidSwitch.bCmdRunMid = FALSE;
            MID_Process_BL(&g_eMidCmd);
            break;
    }
    FMSTR_Poll();
}

int32_t GetPositionActualValue(void)
{
    return g_sM1Enc.pui32QdBase->LPOS + (int16_t)(g_sM1Enc.pui32QdBase->REV) * (M1_POSPE_ENC_PULSES * 4);
}

acc32_t GetPositionCmdValue(int32_t targetPos)
{
    return ACC32(targetPos / (M1_POSPE_ENC_PULSES * 4.0));
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

/* Initialization of the UART module */
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