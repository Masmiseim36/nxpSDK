/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021, 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "mc_periph_init.h"
#include "freemaster.h"
#include "freemaster_serial_lpuart.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_rgpio.h"
#include "fsl_lpuart.h"
#include "m1_sm_servo.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

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
      
/* TMR1 reload ISR called with 1ms period */
RAM_FUNC_LIB
void TMR1_IRQHandler(void);

/* SINC conversation interrupt handler */
RAM_FUNC_LIB
void SINC3_CH0_CH1_CH2_CH3_IRQHandler(void);

/* EnDat2.2 interrupt */
RAM_FUNC_LIB
void ENDAT2P2_IRQHandler(void);

static void BOARD_InitUART(uint32_t u32BaudRate);
static void BOARD_InitSysTick(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoMode    = FALSE;
bool_t bDemoModeSpeed    = FALSE;
bool_t bDemoModePosition = FALSE;

GFLIB_RAMP_T_FLT sPositionDemoRampParams;       /* Position demo ramp parameters */
float_t fltPositionDemoReqValue = 10.0F;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../examples/_boards/frdmimxrt1186/demo_apps/mc_pmsm/pmsm_servo/cm7",         /* User Path 1- the highest priority */
    "../../../boards/frdmimxrt1186/demo_apps/mc_pmsm/pmsm_servo/cm7",       /* User Path 2 */
    "frdmimxrt1186", /* board id */
    "pmsm_servo",     /* example id */
    MCRSP_VER,      /* sw version */
    FEATURE_SET,    /* example's feature-set */
};

ctrl_m1_mid_t g_sSpinMidSwitch;           /* Control Spin/MID switching */

volatile uint32_t ui32CntFast = 0U;

uint8_t ui8M1EndatISRCheck = 0U;
uint64_t ui64EndatData;

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
    bDemoMode = FALSE;

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks(); 
    
    /* Enable target USER_LED */
    RGPIO_PinWrite(BOARD_INITPINS_TP_J3_16_GPIO, BOARD_INITPINS_TP_J3_16_GPIO_PIN, 1); 
    BOARD_INITPINS_TP_J3_16_GPIO->PDDR |= (1U << BOARD_INITPINS_TP_J3_16_GPIO_PIN);     

    /* Init UART for FreeMaster communication */
    BOARD_InitUART(BOARD_FMSTR_UART_BAUDRATE);
    
    /* FreeMaster Initialization */
    FMSTR_Init();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();
    
    /* Init peripheral motor control driver for motors M1 and M2 */
    MCDRV_Init();

    /* Turn off application */
    M1_SetAppSwitch(FALSE);
    
    /* Position demo ramp */
    sPositionDemoRampParams.fltRampUp   = 0.00375; // 1 [rev/s] / SlowLoopSampleTime = 1/4000 = 0.00025
    sPositionDemoRampParams.fltRampDown = sPositionDemoRampParams.fltRampUp;

    /* Spin state machine is default */
    g_sSpinMidSwitch.eAppState = kAppStateSpin;

    /* Enable interrupts */
    EnableGlobalIRQ(ui32PrimaskReg);
    
    /* Enable PWM clock */
    g_sM1Pwm3ph.pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_RUN(0xF);	/* Enable PWM for Motor 1 */
        
    /* Infinite loop */
    while (1)
    {    
        /* FreeMASTER Polling function */
        FMSTR_Poll();
    }
}


/* Endat2.2 IRQ handler for M1 */
RAM_FUNC_LIB
void M1_FLEXIO_IRQHandler()
{
  
    USER_J3_16_ON();
    
    /* Clear EnDat2.2 fault check flag */
    ui8M1EndatISRCheck = 0U;
  
    M1_MCDRV_FLEXIO_ENDAT2P2_GET(&g_sM1Enc);    
    
    FLEXIO_ENDAT2_ClearRxStatusFlags(&g_sM1Enc.endat2Master);
    
    /* M1 state machine */
    SM_StateMachineFast(&g_sM1Ctrl); 
    
    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);
    
    USER_J3_16_OFF();
    
    M1_END_OF_ISR;

}


/* SINC3 conversation interrupt handler */
RAM_FUNC_LIB
void SINC3_CH0_CH1_CH2_CH3_IRQHandler(void)
{
    USER_J3_16_ON();
  
    if(ui8M1EndatISRCheck > 3U)
    {
        /* Set EnDat2.2 fault */
        FAULT_SET(g_sM1Drive.sFaultIdPending, FAULT_ENC_TIMEOUT);
        
        /* Run M1 state machine */
        SM_StateMachineFast(&g_sM1Ctrl); 
        
        ui8M1EndatISRCheck--;
        
    }
      
    /* Read SINC results and process data */
    M1_MCDRV_SINC_GET(&g_sM1Curr3phDcBus);  
    
    ui8M1EndatISRCheck++;
           
    USER_J3_16_OFF();
    
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

    /* Clear the CSCTRL0[TCF1] flag */
    TMR1->CHANNEL[0].CSCTRL |= TMR_CSCTRL_TCF1(0x00);
    TMR1->CHANNEL[0].CSCTRL &= ~(TMR_CSCTRL_TCF1_MASK);

    /* Clear the CSCTRL0[TCF] flag */
    TMR1->CHANNEL[0].SCTRL &= ~(TMR_SCTRL_TCF_MASK);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;

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
