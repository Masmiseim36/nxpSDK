/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_enet.h"

#include "lwip/opt.h"

#include "freemaster.h"
#include "network.h"
#include "freemaster_example.h"
#include "freemaster_net.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/* Stack size of the temporary lwIP initialization thread. */
#define EXAMPLE_THREAD_STACKSIZE 1024

/* Priority of the temporary lwIP initialization thread. */
#define EXAMPLE_FMSTR_THREAD_PRIO 3

/* Priority of the temporary lwIP initialization thread. */
#define EXAMPLE_APP_THREAD_PRIO 2

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Note: All global variables accessed by FreeMASTER are defined in a shared
//! freemaster_example.c file

static FMSTR_BOOL fmstr_initialized = FMSTR_FALSE;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void fmstr_task(void *arg);
static void example_task(void *arg);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void IOMUXC_SelectENETClock(void)
{
#if BOARD_NETWORK_USE_100M_ENET_PORT
    IOMUXC_GPR->GPR4 |= IOMUXC_GPR_GPR4_ENET_REF_CLK_DIR_MASK; /* 50M ENET_REF_CLOCK output to PHY and ENET module. */
#else
    IOMUXC_GPR->GPR5 |= IOMUXC_GPR_GPR5_ENET1G_RGMII_EN_MASK; /* bit1:iomuxc_gpr_enet_clk_dir
                                                                 bit0:GPR_ENET_TX_CLK_SEL(internal or OSC) */
#endif
}

int main(void)
{
    FMSTR_NET_IF_CAPS caps;
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    const clock_sys_pll1_config_t sysPll1Config = {
        .pllDiv2En = true,
    };

    memset(&caps, 0, sizeof(caps));

    /* Board initialization */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    IOMUXC_SelectENETClock();
    CLOCK_InitSysPll1(&sysPll1Config);

#if BOARD_NETWORK_USE_100M_ENET_PORT
    clock_root_config_t rootCfg = {.mux = 4, .div = 10}; /* Generate 50M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet1, &rootCfg);
#else
    clock_root_config_t rootCfg = {.mux = 4, .div = 4}; /* Generate 125M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet2, &rootCfg);
#endif

    /* 50M ENET_REF_CLOCK output to PHY and ENET module. */
    IOMUXC_GPR->GPR4 |= IOMUXC_GPR_GPR4_ENET_REF_CLK_DIR_MASK;

#if __CORTEX_M == 7
    /* Errata ERR050396: For some bus masters, writing access to CM7 TCM is handled by a NIC301 block which does
     * not support sparse write conversion. It results in a data corruption when sparse writing to CM7 TCM happens.
     * For ENET: If CM7 TCM is the destination for writing, IOMUXC_GPR_GPR28[CACHE_ENET] should be cleared. If
     * IOMUXC_GPR_GPR28[CACHE_ENET] is set, write buffers must be placed in OCRAM or external RAM. */
    IOMUXC_GPR->GPR28 &= (~IOMUXC_GPR_GPR28_CACHE_ENET_MASK);
#endif

#if BOARD_NETWORK_USE_100M_ENET_PORT
    BOARD_InitEnetPins();
    GPIO_PinInit(GPIO9, 11, &gpio_config);
    GPIO_PinInit(GPIO12, 12, &gpio_config);
    /* Pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO9, 11, 1);
    GPIO_WritePinOutput(GPIO12, 12, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO12, 12, 1);
    SDK_DelayAtLeastUs(6, CLOCK_GetFreq(kCLOCK_CpuClk));
#else
    BOARD_InitEnet1GPins();
    GPIO_PinInit(GPIO11, 14, &gpio_config);

    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO11, 14, 1);
    SDK_DelayAtLeastUs(30000, CLOCK_GetFreq(kCLOCK_CpuClk));

    EnableIRQ(ENET_1G_MAC0_Tx_Rx_1_IRQn);
    EnableIRQ(ENET_1G_MAC0_Tx_Rx_2_IRQn);
#endif

    /* FreeMaster task */
    if(xTaskCreate(fmstr_task, "fmstr_task", EXAMPLE_THREAD_STACKSIZE, NULL, EXAMPLE_FMSTR_THREAD_PRIO, NULL) == pdFAIL)
        LWIP_ASSERT("fmstr_task: Task creation failed.", 0);

    /* Example application task */
    if(xTaskCreate(example_task, "example_task", EXAMPLE_THREAD_STACKSIZE, NULL, EXAMPLE_APP_THREAD_PRIO, NULL) == pdFAIL)
        LWIP_ASSERT("example_task: Task creation failed.", 0);

    FMSTR_ASSERT_RETURN(FMSTR_NET_DRV.GetCaps != NULL, 0);
    FMSTR_NET_DRV.GetCaps(&caps);

    PRINTF("\n\nFreeMaster %s %s Example\n\n",
           ((caps.flags & FMSTR_NET_IF_CAPS_FLAG_UDP) != 0U ? "UDP" : "TCP"),
           (FMSTR_NET_BLOCKING_TIMEOUT == 0 ? "Non-Blocking" : "Blocking"));

    vTaskStartScheduler();

    /* Will not get here unless a task calls vTaskEndScheduler ()*/
    return 0;
}

void BOARD_ENETFlexibleConfigure(enet_config_t *config)
{
#if BOARD_NETWORK_USE_100M_ENET_PORT
    config->miiMode = kENET_RmiiMode;
#else
    config->miiMode = kENET_RgmiiMode;
#endif
}

/*
 * FreeMASTER Example application task.
 *
 * This task runs a generic FreeMASTER example code - increments several
 * variables so they can be monitored using the FreeMASTER PC Host tool.
 * Note that the FALSE is passed to generic calls, so that the functions
 * do not call FMSTR_Init() and FMSTR_Poll() API - this is called in a
 * task dedicated to FreeMASTER communication.
 */
static void example_task(void *arg)
{
    while (!fmstr_initialized)
    {
        vTaskDelay(10);
    };

    /* Generic example initialization code */
    FMSTR_Example_Init_Ex(FMSTR_FALSE);

    while (1)
    {
        /* Increment test variables periodically, use the
           FreeMASTER PC Host tool to visualize the variables */
        FMSTR_Example_Poll_Ex(FMSTR_FALSE);

        /* Check the network connection and DHCP status periodically */
        Network_Poll();
    }
}

/*
 * FreeMASTER task.
 *
 * Network communication takes place here. This task sleeps when waiting
 * for a communication and lets the other example tasks to run.
 */
static void fmstr_task(void *arg)
{
    /* Network interface initialization */
    Network_Init(CLOCK_GetRootClockFreq(kCLOCK_Root_Bus));

    /* FreeMASTER driver initialization */
    FMSTR_Init();

    fmstr_initialized = FMSTR_TRUE;

    while (1)
    {
        /* The FreeMASTER poll handles the communication interface and protocol
           processing. This call will block the task execution when no communication
           takes place (also see FMSTR_NET_BLOCKING_TIMEOUT option) */
        FMSTR_Poll();

        /* When no blocking timeout is specified, the FMSTR_Poll() returns
           immediately without any blocking. We need to sleep to let other
           tasks to run. */
#if FMSTR_NET_BLOCKING_TIMEOUT == 0
        vTaskDelay(1);
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////
