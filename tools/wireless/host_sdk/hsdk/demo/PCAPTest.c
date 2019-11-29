/*
 * \file PCAPTest.c
 * Source file that demonstrates THCI over RNDIS for the Thread IP stack.
 *
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "PhysicalDevice.h"
#include "Framer.h"
#include "FSCIFrame.h"

#define LENGTH_FIELD_SIZE 2
#define CRC_FIELD_SIZE 1
#define CREATE_NWK_OC 0x1B

#define gThrStatus_Success_c 0x00
#define gThrStatus_AlreadyConnected_c 0x0A
#define gThrStatus_AlreadyCreated_c 0x0B

/*
 * Executes on every RX packet.
 */
void callback(void *callee, void *response)
{
    FSCIFrame *frame = (FSCIFrame *)response;
    if (frame->opCode == CREATE_NWK_OC) {
        if (frame->data[0] == gThrStatus_Success_c) {
            printf("[THCI] Status                OK!\n");
        } else if (frame->data[0] == gThrStatus_Success_c ||
                   frame->data[0] == gThrStatus_AlreadyCreated_c) {
            printf("[THCI] Status                ALREADY CONNECTED!\n");
        } else {
            printf("[THCI] Status                FAIL!\n");
        }
    }
    DestroyFSCIFrame(frame);
}

int main(int argc, char **argv)
{
    /* Check number of arguments. */
    if (argc < 2) {
        printf("Usage: # ./PCAPTest ethX\n");
        printf("\tethX is the interface created by the \x1b[32mrndis_host\x1b[0m driver.\n");
        exit(1);
    }

    PhysicalDevice *device = InitPhysicalDevice(PCAP, NULL, argv[1], GLOBAL);
    Framer *framer = InitializeFramer(device, FSCI, LENGTH_FIELD_SIZE, CRC_FIELD_SIZE, _LITTLE_ENDIAN);
    OpenPhysicalDevice(device);
    AttachToFramer(framer, NULL, callback);

    uint8_t buf[1] = {0};
    FSCIFrame *frame = CreateFSCIFrame(framer, 0xCE, CREATE_NWK_OC, buf, 1, 0);

    while (1) {
        printf("[THCI] THR_CreateNwk.Request\n");
        SendFrame(framer, frame);
        sleep(2);
    }

    return 0;
}
