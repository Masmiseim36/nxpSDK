/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpit.h"
#include "main.h"
#include "freemaster.h"
#include "freemaster_serial_lpuart.h"
#include "fsl_lpuart.h"
#include "pin_mux.h"
#include "board.h"

extern void TSI_DRV_IRQHandler(uint32_t instance);

static void pit_init(void);
static void port_led_init(void);
static void init_freemaster_lpuart(void);
static void aslider_callback(const struct nt_control *control, enum nt_control_aslider_event event, uint32_t position);

/* Call when the TSI counter overflows 65535 */
static void system_callback(uint32_t event, union nt_system_event_context *context);
#if defined(__ICCARM__)
uint8_t nt_memory_pool[1024]; /* IAR EWARM compiler */
#else
uint8_t nt_memory_pool[1024] __attribute__((aligned(4))); /* Keil, GCC compiler */
#endif

lpit_config_t lpitConfig;
lpit_chnl_params_t lpitChannelConfig;

/*
 * This list describes all TSA tables that should be exported to the
 * FreeMASTER application.
 */
#ifndef FMSTR_PE_USED
FMSTR_TSA_TABLE_LIST_BEGIN()
FMSTR_TSA_TABLE(nt_frmstr_tsa_table)
FMSTR_TSA_TABLE_LIST_END()
#endif

#define nt_printf(...) /* do nothing - the debug lines are used by FreeMASTER */

int main(void)
{
    int32_t result;

    /* Init board hardware */
    BOARD_InitPins();

    BOARD_BootClockRUN();
    CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcSircAsync);

    port_led_init();

    /* Initialize the OS abstraction layer */
    NT_OSA_Init();

    /* FreeMASTER communication layer initialization */
    init_freemaster_lpuart();

    /* FreeMASTER initialization */
    (void)FMSTR_Init();

    if ((result = nt_init(&System_0, nt_memory_pool, sizeof(nt_memory_pool))) != NT_SUCCESS)
    {
        switch (result)
        {
            case NT_FAILURE:
                nt_printf("\nCannot initialize NXP Touch due to a non-specific error.\n");
                break;
            case NT_OUT_OF_MEMORY:
                nt_printf("\nCannot initialize NXP Touch due to a lack of free memory.\n");
                printf("\nCannot initialize NXP Touch due to a non-specific error.\n");
                break;
        }
        while (1)
            ; /* add code to handle this error */
    }

    nt_printf("\nNXP Touch is successfully initialized.\n");

    nt_printf("Unused memory: %d bytes, you can make the memory pool smaller without affecting the functionality.\n",
              (int)nt_mem_get_free_size());

    /* Enable electrodes and controls */
    nt_enable();

    /* Slider electrodes */
    nt_control_aslider_register_callback(&ASlider_1, &aslider_callback);

    /* System TSI overflow warning callback */
    nt_system_register_callback(&system_callback);

    pit_init();

    while (1)
    {
        nt_task();

        /* The FreeMASTER poll call must be called in the main application loop
        to handle the communication interface and protocol.
           In the LONG_INTR FreeMASTER interrupt mode, all the processing is done
        during the communication interrupt routine and the FMSTR_Poll() is
        compiled empty. */
        FMSTR_Poll();
    }
}

void LPIT0_IRQHandler(void)
{
    /* Clear the interrupt flag.*/
    LPIT_ClearStatusFlags(LPIT0, kLPIT_Channel0TimerFlag);
    nt_trigger();
}

void TSI0_IRQHandler(void)
{
    TSI_DRV_IRQHandler(0);
}

static void pit_init(void)
{
    /*
     * lpitConfig.enableRunInDebug = false;
     * lpitConfig.enableRunInDoze = false;
     */
    LPIT_GetDefaultConfig(&lpitConfig);

    /* Init lpit module */
    LPIT_Init(LPIT0, &lpitConfig);

    lpitChannelConfig.chainChannel          = false;
    lpitChannelConfig.enableReloadOnTrigger = false;
    lpitChannelConfig.enableStartOnTrigger  = false;
    lpitChannelConfig.enableStopOnTimeout   = false;
    lpitChannelConfig.timerMode             = kLPIT_PeriodicCounter;
    /* Set default values for the trigger source */
    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn0;
    lpitChannelConfig.triggerSource = kLPIT_TriggerSource_External;

    /* Init lpit channel 0 */
    LPIT_SetupChannel(LPIT0, kLPIT_Chnl_0, &lpitChannelConfig);

    /* Set PIT0 period 5ms */
    LPIT_SetTimerPeriod(LPIT0, kLPIT_Chnl_0, USEC_TO_COUNT(5000U, CLOCK_GetIpFreq(kCLOCK_Lpit0)));

    /* Enable timer interrupts for channel 0 */
    LPIT_EnableInterrupts(LPIT0, kLPIT_Channel0TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(LPIT0_IRQn);

    /* Start channel 0 */
    PRINTF("\r\nStarting channel No.0 ...");
    LPIT_StartTimer(LPIT0, kLPIT_Chnl_0);
}

static void port_led_init(void)
{
    /* LED Init */
    LED_RED_INIT(1);
    LED_GREEN_INIT(1);
    LED_BLUE_INIT(1);
}

static void aslider_callback(const struct nt_control *control, enum nt_control_aslider_event event, uint32_t position)
{
    switch (event)
    {
        case NT_ASLIDER_INITIAL_TOUCH:
            nt_printf("\n Touch: %d", position);
            if (position < 20)
            {
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 20) && (position < 40))
            {
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 40) && (position < 60))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 60) && (position < 80))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_ON();
            }
            else if (position >= 80)
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_ON();
            }
            break;
        case NT_ASLIDER_MOVEMENT:
            nt_printf("\n Movement: %d", position);
            if (position < 20)
            {
                LED_RED_TOGGLE();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 20) && (position < 40))
            {
                LED_RED_TOGGLE();
                LED_GREEN_TOGGLE();
                LED_BLUE_OFF();
            }
            else if ((position >= 40) && (position < 60))
            {
                LED_RED_OFF();
                LED_GREEN_TOGGLE();
                LED_BLUE_OFF();
            }
            else if ((position >= 60) && (position < 80))
            {
                LED_RED_OFF();
                LED_GREEN_TOGGLE();
                LED_BLUE_TOGGLE();
            }
            else if (position >= 80)
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_TOGGLE();
            }
            break;
        case NT_ASLIDER_ALL_RELEASE:
            nt_printf("\n Release: %d", position);
            LED_RED_OFF();
            LED_GREEN_OFF();
            LED_BLUE_OFF();
            break;
    }
}
/* Call on the TSI CNTR overflow 16-bit range (65535) */
void system_callback(uint32_t event, union nt_system_event_context *context)
{
    switch (event)
    {
        case NT_SYSTEM_EVENT_DATA_OVERFLOW:
            /* TSI counter overflow warning */
            nt_printf("TSI CNTR overflow warning on electrode %d.\n", (int)index);
            break;
        default:
            break;
    }
}
/*!
 * @brief UART Module initialization
 */
static void init_freemaster_lpuart(void)
{
    lpuart_config_t config;

    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSircAsync);

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200U;
    config.enableTx     = false;
    config.enableRx     = false;

    LPUART_Init(BOARD_DEBUG_UART_BASEADDR, &config, BOARD_DEBUG_UART_CLK_FREQ);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(BOARD_DEBUG_UART_BASEADDR);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(BOARD_UART_IRQ);
    EnableGlobalIRQ(0);
#endif
}

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
/*
 *   Application interrupt handler of communication peripheral used in interrupt modes
 *   of FreeMASTER communication.
 *
 *   NXP MCUXpresso SDK framework defines interrupt vector table as a part of "startup_XXXXXX.x"
 *   assembler/C file. The table points to weakly defined symbols, which may be overwritten by the
 *   application specific implementation. FreeMASTER overrides the original weak definition and
 *   redirects the call to its own handler.
 *
 */

void BOARD_UART_IRQ_HANDLER(void)
{
    /* Call FreeMASTER Interrupt routine handler */
    FMSTR_SerialIsr();
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
        exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif
