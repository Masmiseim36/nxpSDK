/*
 * \file ZigBeeCoordinator.c
 * Source file that demonstrates ZigBee stack functionality, using C
 * bindings generated from the ZigBee 3.0 FSCI XML.
 *
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the
 * disclaimer below) provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 * GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#include "cmd_zigbee3_0_6.0.7.h"

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
