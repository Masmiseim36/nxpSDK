/*
 * \file SPITest.c
 * Source file that demonstrates THCI over SPI for the Thread IP stack.
 *
 * The Clear BSD License
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
#include "SPIConfiguration.h"


// mutually exclusive
#define MAC802154 0
#define THREAD 1


void hex_dump(char *desc, uint8_t *data, int size)
{
    int i;
    uint8_t ascii_repr[17];

    if (desc != NULL)
        printf("%s:\n", desc);

    for (i = 0; i < size; i++) {
        if ((i % 16) == 0) {
            if (i != 0)
                printf("  %s\n", ascii_repr);

            printf("  %04x ", i);
        }

        printf(" %02x", data[i]);

        if ((data[i] < 0x20) || (data[i] > 0x7e))
            ascii_repr[i % 16] = '.';
        else
            ascii_repr[i % 16] = data[i];
        ascii_repr[(i % 16) + 1] = '\0';
    }

    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }

    printf("  %s\n", ascii_repr);
}


int counter = 0;
/*
 * Executes on every RX packet.
 */
void callback(void *callee, void *response)
{
    counter++;
    FSCIFrame *frame = (FSCIFrame *)response;
    printf("%d ", counter);
    hex_dump("RX", frame->data, frame->length);
    DestroyFSCIFrame(frame);
}


#if MAC802154
void start_mac(Framer *framer)
{
    uint8_t set_channel_data[0x12] = {0x21, 0, 0x0B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t set_panid_data[0x12]   = {0x50, 0, 0xCC, 0xCC, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t mac_reset_data[1]      = {0x01};

    FSCIFrame *set_channel = CreateFSCIFrame(framer, 0x85, 0x09, set_channel_data, 0x12, 0);
    FSCIFrame *set_panid   = CreateFSCIFrame(framer, 0x85, 0x09, set_panid_data,   0x12, 0);
    FSCIFrame *mac_reset   = CreateFSCIFrame(framer, 0x85, 0x06, mac_reset_data,   0x01, 0);

    SendFrame(framer, mac_reset);   sleep(1);
    SendFrame(framer, set_channel); sleep(1);
    SendFrame(framer, set_panid);   sleep(1);
}

void mac_data_req(Framer *framer)
{
    uint8_t mac_data_req_data[0x24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCC, 0xCC,
                                       0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC,
                                       0xCC, 0x02, 0x00, 0x55, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x05
                                      };
    FSCIFrame *mac_data_req = CreateFSCIFrame(framer, 0x87, 0x00, mac_data_req_data, 0x24, 0);
    SendFrame(framer, mac_data_req);
}
#endif


int main(int argc, char **argv)
{
    /* Check number of arguments. */
    if (argc < 2) {
        printf("Usage: # %s /dev/spidev-bus.device [speed Hz]\n", argv[0]);
        printf("\t* /dev/spidev-bus.device is the SPI node created by \x1b[32mspidev\x1b[0m.\n");
        printf("\t* speed defaults to \x1b[32m1MHz\x1b[0m.\n");
        exit(1);
    }

    uint8_t LENGTH_FIELD_SIZE = (THREAD) ? 2 : 1;
    uint8_t CRC_FIELD_SIZE = 1;

    SPIConfigurationData *config = defaultSettingsSPI();
    setLengthFieldSize(config, LENGTH_FIELD_SIZE);
    if (argc == 3) {
        setSpeedHzSPI(config, atoi(argv[2]));
    }

    PhysicalDevice *device = InitPhysicalDevice(SPI, config, argv[1], GLOBAL);
    Framer *framer = InitializeFramer(device, FSCI, LENGTH_FIELD_SIZE, CRC_FIELD_SIZE, _LITTLE_ENDIAN);
    OpenPhysicalDevice(device);
    AttachToFramer(framer, NULL, callback);

#if THREAD
    uint8_t buf1[1] = {0x00};                                                   // THR Create Network payload
    uint8_t buf2[9] = {0x00, 0x00, 0x08, 0x20, 0x00, 0x01, 0x02, 0x0A, 0x00};   // THR Scan Networks payload
    // FSCIFrame *factory_rst = CreateFSCIFrame(framer, 0xCE, 0x1F, NULL, 0, 0);     // THR Factory Reset frame
    FSCIFrame *create_nwk = CreateFSCIFrame(framer, 0xCE, 0x1B, buf1, 1, 0);      // THR Create Network frame
    FSCIFrame *scan_nwks = CreateFSCIFrame(framer, 0xCE, 0x1A, buf2, 9, 0);       // THR Scan Networks frame
#endif

#if MAC802154
    start_mac(framer);
#endif

    printf("Press ENTER to send frames..\n");
    while (getchar()) {
#if THREAD
        SendFrame(framer, scan_nwks);
        nanosleep((const struct timespec[]){{0, 100000000L}}, NULL);
        SendFrame(framer, create_nwk);
        nanosleep((const struct timespec[]){{0, 100000000L}}, NULL);
#endif

#if MAC802154
        mac_data_req(framer);
        nanosleep((const struct timespec[]){{0, 100000000L}}, NULL);
#endif
    }

    return 0;
}
