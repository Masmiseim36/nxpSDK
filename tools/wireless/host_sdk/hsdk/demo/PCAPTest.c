/*
 * \file PCAPTest.c
 * Source file that demonstrates THCI over RNDIS for the Thread IP stack.
 *
 * The Clear BSD License
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
