/*
 * \file ZigBeeCoordinator.c
 * Source file that demonstrates ZigBee stack functionality, using C
 * bindings generated from the ZigBee 3.0 FSCI XML.
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

#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "FSCIFrame.h"
#include "Framer.h"
#include "PhysicalDevice.h"
#include "UARTConfiguration.h"

#include "cmd_zigbee3_0_6.0.9.h"

/*==================================================================================================
Private macros
==================================================================================================*/
#define FSCI_ZB_IF                          0

/*==================================================================================================
Private type definitions
==================================================================================================*/
/* Generic structure for holding uint64 values */
typedef union uuint64_tag {
    uint64_t    u64;
    uint32_t    u32[2];
    uint16_t    u16[4];
    uint8_t     u8[8];
} uuint64_t;

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
extern void SHELL_ZigEventNotify(void *param);

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

/* Reverts a 64 bit numeric value. */
static uint64_t NWKU_Revert64(uint64_t value)
{
    uuint64_t in;
    uuint64_t out;
    in.u64 = value;

    out.u8[0] = in.u8[7];
    out.u8[1] = in.u8[6];
    out.u8[2] = in.u8[5];
    out.u8[3] = in.u8[4];
    out.u8[4] = in.u8[3];
    out.u8[5] = in.u8[2];
    out.u8[6] = in.u8[1];
    out.u8[7] = in.u8[0];

    return out.u64;
}

/* Dispatcher for the events that are of interest for the demo. */
static void KHC_Dispatcher(zigEvtContainer_t *container)
{
    switch (container->id) {

        case NetworkJoinedFormed_FSCI_ID:
            shell_printf("\tShort Address --> %04X\n", container->Data.NetworkJoinedFormed.ShortAddr);
            shell_printf("\tExtended Address --> %" PRIx64 "\n", NWKU_Revert64(container->Data.NetworkJoinedFormed.ExtAddr));
            shell_printf("\tChannel --> %d\n", container->Data.NetworkJoinedFormed.Channel);
            break;

        default:
            break;
    }
}


/* Called on every received FSCI packet from the board. */
static void FSCI_RX_Callback(void *callee, void *response)
{
    static zigEvtContainer_t container;
    KHC_ZIGBEE_RX_MsgHandler(response, &container, FSCI_ZB_IF);
    DestroyFSCIFrame(response);
    SHELL_ZigEventNotify(&container);
    KHC_Dispatcher(&container);
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

    /* Factory reset and then create a new ZigBee 3.0 network */
    ErasePersistentData(mpFramer, FSCI_ZB_IF);
    sleep(3);

    StartNetworkMessage(mpFramer, FSCI_ZB_IF);
    /* wait for asynchronous events to be displayed */
    sleep(3);

    return 0;
}
