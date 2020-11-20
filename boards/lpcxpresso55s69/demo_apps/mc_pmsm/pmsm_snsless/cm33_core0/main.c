/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "main.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* CPU load measurement using Systick*/
uint32_t g_ui32NumberOfCycles    = 0;
uint32_t g_ui32MaxNumberOfCycles = 0;

/* Demo mode enabled/disabled */
bool_t bDemoModeSpeed = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt = 0;

/* Application and board ID  */
app_ver_t g_sAppId = {
    "evk-lpc55s69", /* board id */
    "pmsm",         /* motor type */
    MCRSP_VER,      /* sw version */
};
/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void init_freemaster_usart(void);

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

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    /* Disable demo mode after reset */
    bDemoModeSpeed         = TRUE;
    ui32SpeedStimulatorCnt = 0;

    /* Pass actual demo id and board info to FM */
    g_sAppIdFM = g_sAppId;

    /* Init board hardware. */
    BOARD_Init();

    /* Initialize peripheral motor control driver for motor M1*/
    MCDRV_Init_M1();

    /* Attach main clock divide to FLEXCOMM0 (debug console) 12MHz */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* FreeMASTER communication layer initialization */
    init_freemaster_usart();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* FreeMASTER initialization */
    FMSTR_Init();

    /* Turn off application */
    M1_SetAppSwitch(0);

    /* Enable interrupts  */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Infinite loop */
    while (1)
    {
        /* FreeMASTER Polling function */
        FMSTR_Poll();
    }
}

/*!
 *@brief      Initialization of the Clocks and Pins
 *
 *@param      none
 *
 *@return     none
 */
void BOARD_Init(void)
{
    /* Initialize clock configuration */
    BOARD_BootClockPLL150M();
    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();
    /* Init GPIO pins */
    BOARD_InitGPIOPins();
}

/*!
 * @brief USART Module initialization (USART is a the standard block included e.g. in K22F)
 */
static void init_freemaster_usart(void)
{
    usart_config_t config;
    /*
     *   usartConfig->baudRate_Bps = 115200U;
     *   usartConfig->parityMode = kUSART_ParityDisabled;
     *   usartConfig->stopBitCount = kUSART_OneStopBit;
     *   usartConfig->bitCountPerChar = kUSART_8BitsPerChar;
     *   usartConfig->loopback = false;
     *   usartConfig->enableTx = false;
     *   usartConfig->enableRx = false;
     */
    USART_GetDefaultConfig(&config);
    /* Override the Default configuration to satisfy FreeMASTER needs */
    config.baudRate_Bps = BOARD_FMSTR_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    USART_Init(BOARD_FMSTR_UART_PORT, &config, BOARD_DEBUG_UART_CLK_FREQ);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(BOARD_FMSTR_UART_PORT);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(BOARD_UART_IRQ);
    EnableGlobalIRQ(0);
#endif
}

/*!
 * @brief   CTIMER2 ISR called with 1ms period and processes following functions:
 *           - motor M1 slow application machine function
 *
 * @param   void
 *
 * @return  none
 */
void CTIMER2_IRQHandler(void)
{
    static int16_t ui16i = 0;

    /* M1 Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);

    /* If in STOP state turn on red */
    if (M1_GetAppState() == 2)
    {
        LED_GREEN_OFF();
    }

    /* If in FAULT state red blinking*/
    else if (M1_GetAppState() == 0)
    {
        if (ui16i-- < 0)
        {
            LED_GREEN_TOGGLE();
            ui16i = 125;
        }
    }

    /* If in RUN or INIT state turn on green */
    else
    {
        LED_GREEN_ON();
    }

    /* Demo speed stimulator */
    DemoSpeedStimulator();

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    CTIMER2->IR = CTIMER_IR_MR1INT(1U);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
 * @brief   ADC ISR called with 100us period processes
 *           - motor M1 fast application machine function
 *           - demo mode if enabled
 *
 * @param   void
 *
 * @return  none
 */
void ADC0_IRQHandler(void)
{
    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();

    /* M1 Fast StateMachine call */
    SM_StateMachineFast(&g_sM1Ctrl);

    /* Stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles =
        g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Enable interrupts  */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
 * @brief   SCT0 ISR initiated from CPM (overcurrent protection)
 *
 * @param   void
 *
 * @return  none
 */
void SCT0_IRQHandler(void)
{
    MCDRV_scTmrPwm3PhOutDis(&g_sM1Pwm3ph); // DISABLE PWM OUTPUT
    FAULT_SET(g_sM1Drive.sFaultIdPending, FAULT_I_DCBUS_OVER);

    /* Write to the flag registers */
    SCT0->EVFLAG  = (0x4000 & 0xFFFFU);
    SCT0->CONFLAG = (0x4000 & (SCT_CONFLAG_BUSERRL_MASK | SCT_CONFLAG_BUSERRH_MASK));

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
void DemoSpeedStimulator(void)
{
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
                break;
        }
    }
    else
    {
        ui32SpeedStimulatorCnt = 0U;
    }
}

/*!
 *@brief      Initialization of the GPIO pins
 *
 *@param      none
 *
 *@return     none
 */
void BOARD_InitGPIOPins(void)
{
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    /* Init output LED GPIO. */
    GPIO_PortInit(GPIO, BOARD_LED_GREEN_GPIO_PORT);
    GPIO_PinInit(GPIO, BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN, &led_config);
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
