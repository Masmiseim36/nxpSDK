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

int main(void)
{
    FMSTR_NET_IF_CAPS caps;
    memset(&caps, 0, sizeof(caps));
    
    /* Board initialization */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);
    
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    //delay();
    GPIO_WritePinOutput(GPIO1, 9, 1);
    
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
    while(!fmstr_initialized)
    {
        vTaskDelay(10);
    };
    
    /* Generic example initialization code */
    FMSTR_Example_Init_Ex(FMSTR_FALSE);
    
    while(1)
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
    Network_Init(CLOCK_GetFreq(kCLOCK_IpgClk));
    
    /* FreeMASTER driver initialization */
    FMSTR_Init();

    fmstr_initialized = FMSTR_TRUE;
    
    while(1)
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
