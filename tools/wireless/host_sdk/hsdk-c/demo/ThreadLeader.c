/*
 * \file ThreadLeader.c
 * Source file that demonstrates Thread stack functionality, using C
 * bindings generated from the Thread FSCI XML.
 *
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*==================================================================================================
Include Files
==================================================================================================*/
#define _BSD_SOURCE

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "FSCIFrame.h"
#include "Framer.h"
#include "PhysicalDevice.h"
#include "UARTConfiguration.h"

#include "cmd_threadip_1.2.8.h"

/*==================================================================================================
Private macros
==================================================================================================*/
#define FSCI_THR_IF                         0

/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/

/*==================================================================================================
Private variables declarations
==================================================================================================*/
/* Framework variables */
static PhysicalDevice *mpDevice = NULL;
static Framer *mpFramer = NULL;
static UARTConfigurationData *mpUartConfig = NULL;

/*==================================================================================================
Public prototypes
==================================================================================================*/
extern void SHELL_ThrEventNotify(void *param);

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Private functions
==================================================================================================*/
/* Free resources when Ctrl-C is pressed. */
static void sig_handler(int signo)
{
    if (signo == SIGINT) {
        DestroyFramer(mpFramer);
        DestroyPhysicalDevice(mpDevice);
        freeConfigurationData(mpUartConfig);

        exit(0);
    }
}

/* Called on every received FSCI packet from the board. */
static void FSCI_RX_Callback(void *callee, void *response)
{
    static thrEvtContainer_t container;
    KHC_THREAD_RX_MsgHandler(response, &container, FSCI_THR_IF);
    DestroyFSCIFrame(response);
    SHELL_ThrEventNotify(&container);
}

/*==================================================================================================
Private debug functions
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/
int main(int argc, char **argv)
{
    /* Check number of arguments. */
    if (argc < 2) {
        shell_printf("Usage: # %s </dev/ttyACMx | /dev/ttymxcx>\n", argv[0]);
        exit(1);
    }

    /* Add signal handler for SIGINT. */
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        shell_printf("Cannot catch SIGINT\n");
    }

    /* Open device and create FSCI framer. */
    mpUartConfig = defaultConfigurationData();
    mpDevice = InitPhysicalDevice(UART, mpUartConfig, argv[1], GLOBAL);
    mpFramer = InitializeFramer(mpDevice, FSCI, FSCI_LENGTH_FIELD_SIZE, 1, _LITTLE_ENDIAN);
    OpenPhysicalDevice(mpDevice);
    AttachToFramer(mpFramer, NULL, FSCI_RX_Callback);

    /* Factory reset and then create a new Thread network */
    THR_FactoryResetRequest(mpFramer, FSCI_THR_IF);
    sleep(5);

    THR_CreateNwkRequest_t req = {
        .InstanceID = 0
    };
    THR_CreateNwkRequest(&req, mpFramer, FSCI_THR_IF);
    /* wait for asynchronous events to be displayed */
    sleep(5);

    return 0;
}
