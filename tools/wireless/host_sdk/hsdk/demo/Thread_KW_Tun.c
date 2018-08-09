/*
 * \file Thread_KW_Tun.c
 * Source file that demonstrates the Serial TUN/TAP capability of a Thread
 * border router.
 *
 * The Clear BSD License
 * Copyright 2013-2015 Freescale Semiconductor, Inc.
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

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>

#ifdef __linux__
#include <linux/if_tun.h>
#include <linux/if.h>
#endif

#include "hsdkError.h"
#ifdef __linux__spi__
#include "SPIConfiguration.h"
#include "SPIDevice.h"
#endif
#include "UARTConfiguration.h"
#include "UARTDevice.h"
#include "FSCIFrame.h"


#define DEBUG 1
#define PROVISION 1
#define SERIAL_TAP 0
#define HOST_ADDS_ULA_PREFIX 0
#define CLOSE_OPEN_PORT 0

#define ENET_HDR_SIZE 14
#define IPv6_VERSION 0x60
#define MTU 1500
#define LENGTH_FIELD_SIZE 2
#define CRC_FIELD_SIZE 1
#define VIF 0
#define SYNC_BYTE 0x02

#define TX_OG 0xCE
#define RX_OG 0xCF

#define VTUN_TX_OC 0xF2
#define VTUN_RX_OC 0xF3
#define FACTRY_RST 0x1F
#define SET_ATTRIB 0x18
#define CREATE_NWK 0x1B
#if HOST_ADDS_ULA_PREFIX
    #define ADD_PREFIX 0x29
    #define SYNC_PREFX 0x2C
#endif
#define START_COMM 0x40
#define ADD_JOINER 0x42
#define SYNC_STEER 0x46

static int tun_fd;
static PhysicalDevice *device = NULL;
static Framer *framer = NULL;
static DeviceType dev_type;
static void *config = NULL;

static uint8_t THR_SetAttr_Channel[5] = {0x00, 0x04, 0x00, 0x01, 0x19};
static uint8_t THR_CreateNwk[1] = {0x00};
#if HOST_ADDS_ULA_PREFIX
    static uint8_t THR_BrPrefixAddEntry[31] = {0x00, 0x40, 0xFD, 0x01, 0x00, 0x00,
                                               0x00, 0x00, 0x3E, 0xAD, 0x00, 0x00,
                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                               0x00, 0x33, 0xFF, 0xFF, 0xFF, 0xFF,
                                               0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                                               0x00
                                              };
    static uint8_t THR_BrPrefixSync[1] = {0x00};
#endif
static uint8_t MESHCOP_StartCommissioner[1] = {0x00};
/* PSKd = 'THREAD' (54 48 52 45 41 44) */
static uint8_t MESHCOP_AddExpectedJoiner[18] = {0x00, 0x01, 0x01, 0xFF, 0xFF,
                                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                0xFF, 0x06, 0x54, 0x48, 0x52,
                                                0x45, 0x41, 0x44
                                               };
static uint8_t MESHCOP_SyncSteeringData[2] = {0x00, 0x01};

void provision(Framer *framer, int use_factory_reset, int channel)
{
    FSCIFrame *create_nwk         = CreateFSCIFrameAdHoc(TX_OG, CREATE_NWK, THR_CreateNwk, sizeof(THR_CreateNwk), VIF, LENGTH_FIELD_SIZE, _LITTLE_ENDIAN);
#if HOST_ADDS_ULA_PREFIX
    FSCIFrame *add_prefix         = CreateFSCIFrameAdHoc(TX_OG, ADD_PREFIX, THR_BrPrefixAddEntry, sizeof(THR_BrPrefixAddEntry), VIF, LENGTH_FIELD_SIZE, _LITTLE_ENDIAN);
    FSCIFrame *sync_prefix        = CreateFSCIFrameAdHoc(TX_OG, SYNC_PREFX, THR_BrPrefixSync, sizeof(THR_BrPrefixSync), VIF, LENGTH_FIELD_SIZE, _LITTLE_ENDIAN);
#endif
    FSCIFrame *start_comm         = CreateFSCIFrameAdHoc(TX_OG, START_COMM, MESHCOP_StartCommissioner, sizeof(MESHCOP_StartCommissioner), VIF, LENGTH_FIELD_SIZE, _LITTLE_ENDIAN);
    FSCIFrame *add_joiner         = CreateFSCIFrameAdHoc(TX_OG, ADD_JOINER, MESHCOP_AddExpectedJoiner, sizeof(MESHCOP_AddExpectedJoiner), VIF, LENGTH_FIELD_SIZE, _LITTLE_ENDIAN);
    FSCIFrame *sync_steering_data = CreateFSCIFrameAdHoc(TX_OG, SYNC_STEER, MESHCOP_SyncSteeringData, sizeof(MESHCOP_SyncSteeringData), VIF, LENGTH_FIELD_SIZE, _LITTLE_ENDIAN);

    if (use_factory_reset) {
        int rc = 0, fd;
        uint8_t rst[6] = {0x02, TX_OG, FACTRY_RST, 0x00, 0x00, 0xD1};

        if (dev_type == UART) {
            fd = ((UARTHandle *)(device->deviceHandle))->portHandle;
        }
#ifdef __linux__spi__
        else if (dev_type == SPI) {
            fd = ((SPIHandle *)(device->deviceHandle))->portHandle;
            sleep(1);
        }
#endif
        else {
            printf("Device type not supported. Exiting..\n");
            exit(1);
        }

        rc = write(fd, rst, 6);
        if (rc == -1) {
            perror("Send Factory Reset");
            exit(1);
        }

#if CLOSE_OPEN_PORT
        printf("[THR] Factory Reset"); fflush(stdout);
        sleep(3);
        printf(" ...."); fflush(stdout);

        /* Close UART device. */
        if (dev_type == UART) {
            rc = close(fd);
            if (rc == -1) {
                perror("Close USB/UART Connection");
                exit(1);
            }
            sleep(3);
            printf(" ...."); fflush(stdout);
        } else {
            ClosePhysicalDevice(device);
            sleep(2);
        }

        /* Reopen UART device. */
        if (dev_type == UART) {
            rc = device->open(device->deviceHandle, config);
            if (rc != HSDK_ERROR_SUCCESS) {
                perror("Open USB/UART Connection");
                exit(1);
            }
            sleep(3);
            printf(" ....  OK!\n");
        } else {
            OpenPhysicalDevice(device);
            sleep(2);
        }
#else
        sleep(1);
#endif
    }

    if (channel != -1) {
        THR_SetAttr_Channel[4] = (uint8_t)channel;
    }

    FSCIFrame *set_channel = CreateFSCIFrameAdHoc(TX_OG, SET_ATTRIB, THR_SetAttr_Channel, sizeof(THR_SetAttr_Channel), VIF, LENGTH_FIELD_SIZE, _LITTLE_ENDIAN);

    SendFrame(framer, set_channel); sleep(1);
    SendFrame(framer, create_nwk); sleep(12);
#if HOST_ADDS_ULA_PREFIX
    SendFrame(framer, add_prefix); sleep(1);
    SendFrame(framer, sync_prefix); sleep(1);
#endif
    SendFrame(framer, start_comm); sleep(1);
    SendFrame(framer, add_joiner); sleep(1);
    SendFrame(framer, sync_steering_data); sleep(1);

    DestroyFSCIFrame(set_channel);
    DestroyFSCIFrame(create_nwk);
#if HOST_ADDS_ULA_PREFIX
    DestroyFSCIFrame(add_prefix);
    DestroyFSCIFrame(sync_prefix);
#endif
    DestroyFSCIFrame(start_comm);
    DestroyFSCIFrame(add_joiner);
    DestroyFSCIFrame(sync_steering_data);
}

/*
 * Free resources when Ctrl-C is pressed.
 */
void sig_handler(int signo)
{
    if (signo == SIGINT) {
        printf("Received SIGINT, stopping threads and freeing resources...\n");

        close(tun_fd);
        DestroyFramer(framer);
        DestroyPhysicalDevice(device);
        freeConfigurationData(config);

        exit(0);
    }
}

/*
 * Pretty print a byte array.
 */
void hex_dump(char *desc, uint8_t *data, int size)
{
#if DEBUG
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
#endif
}

/*
 * On Linux, opens /dev/net/tun and creates a new interface named `dev`.
 * On OS X, expects `dev` = /dev/tun[0-15].
 *
 * Returns file descriptor.
 */
int tun_alloc(char *dev)
{
#ifdef __linux__
    struct ifreq ifr;
    int fd, err;

    if ( (fd = open("/dev/net/tun", O_RDWR)) < 0 ) {
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));

#if SERIAL_TAP
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
#else
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
#endif

    if ( *dev ) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ - 1);
        ifr.ifr_name[IFNAMSIZ - 1] = '\0';
    }

    if ( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
        close(fd);
        return err;
    }
    strcpy(dev, ifr.ifr_name);
    return fd;

#elif __APPLE__
    return open(dev, O_RDWR | O_NOCTTY);
#endif
}

int get_baudrate(int value)
{
    if (value == 110)
        return BR110;
    if (value == 300)
        return BR300;
    if (value == 600)
        return BR600;
    if (value == 1200)
        return BR1200;
    if (value == 2400)
        return BR2400;
    if (value == 4800)
        return BR4800;
    if (value == 9600)
        return BR9600;
    if (value == 19200)
        return BR19200;
    if (value == 38400)
        return BR38400;
    if (value == 57600)
        return BR57600;
    if (value == 115200)
        return BR115200;

    return -1;
}

/*
 * Executes on every RX packet.
 */
void callback(void *callee, void *response)
{
    FSCIFrame *frame = (FSCIFrame *) response;
    /* Checks for frame validity */
    if (    frame->sync == SYNC_BYTE &&
            frame->opGroup == RX_OG &&
            frame->opCode == VTUN_RX_OC &&
            frame->data != NULL
       ) {
        hex_dump("RX", frame->data, frame->length);

        /* Add an advisory lock on the interface. */
        flock(tun_fd, LOCK_EX);
        int rc = write(tun_fd, frame->data, frame->length);
        if (rc == -1) {
            perror("TUN write");
        }
        flock(tun_fd, LOCK_UN);

    } else if (frame->opGroup == RX_OG) {
        if (frame->opCode == FACTRY_RST) {
            if (frame->data[0] == 0x00) {
                printf("[THR] Factory Reset                 OK!\n");
            } else {
                printf("[THR] Factory Reset                 FAIL!\n");
            }
        }
        if (frame->opCode == SET_ATTRIB) {
            if (frame->data[0] == 0x00) {
                printf("[THR] Set 802.15.4 channel          OK!\n");
            } else {
                printf("[THR] Set 802.15.4 channel          FAIL!\n");
            }
        }
        if (frame->opCode == CREATE_NWK) {
            if (frame->data[0] == 0x00) {
                printf("[THR] Create Network                OK!\n");
            } else if (frame->data[0] == 0x0A || frame->data[0] == 0x0B) {
                printf("[THR] Create Network                ALREADY CONNECTED!\n");
            } else {
                printf("[THR] Create Network                FAIL!\n");
            }
        }
#if HOST_ADDS_ULA_PREFIX
        if (frame->opCode == ADD_PREFIX) {
            if (frame->data[0] == 0x00) {
                printf("[THR] Border Router Add Prefix      OK!\n");
            } else {
                printf("[THR] Border Router Add Prefix      FAIL!\n");
            }
        }
        if (frame->opCode == SYNC_PREFX) {
            if (frame->data[0] == 0x00) {
                printf("[THR] Border Router Sync Prefix     OK!\n");
            } else {
                printf("[THR] Border Router Sync Prefix     FAIL!\n");
            }
        }
#endif
        if (frame->opCode == START_COMM) {
            if (frame->data[0] == 0x00) {
                printf("[MESHCOP] Start Commissioner        OK!\n");
            } else {
                printf("[MESHCOP] Start Commissioner        FAIL!\n");
            }
        }
        if (frame->opCode == ADD_JOINER) {
            if (frame->data[0] == 0x00) {
                printf("[MESHCOP] Add Expected Joiner       OK!\n");
            } else {
                printf("[MESHCOP] Add Expected Joiner       FAIL!\n");
            }
        }
        if (frame->opCode == SYNC_STEER) {
            if (frame->data[0] == 0x00) {
                printf("[MESHCOP] Sync Steering Data        OK!\n");
            } else {
                printf("[MESHCOP] Sync Steering Data        FAIL!\n");
            }
        }
    }

    DestroyFSCIFrame(frame);
}

int main(int argc, char **argv)
{
    int use_factory_reset = 0, channel = -1, rc;
    ssize_t nread;
    uint8_t buffer[MTU + 2], request[MTU];
#ifdef __APPLE__
    /* Variables for select(). */
    fd_set set;
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
#endif

    /* Check number of arguments. */
    if (argc < 3) {
        printf("Usage: # %s dev_name tun_name [factory_reset] [channel] [baudrate/speed]\n", argv[0]);
        printf("Required arguments\n");
        printf("\tdev_name - /dev/ttyACMx | /dev/spidevX-Y, can be retrieved with dmesg\n");
#ifndef __linux__spi__
        printf("\t\tWARNING: SPI device node support is disabled.\n");
#endif
        printf("\ttun_name - threadtun0 when using make_tun.sh\n");
        printf("Optional arguments\n");
        printf("\tfactory_reset - boolean, whether or not the device will suffer a factory reset at the beginning of the program; defaults to 0\n");
        printf("\tchannel - the 802.15.4 channel, possible values [11-25]; defaults to 25\n");
        printf("\tbaudrate/speed - not used when interfacing over USB; defaults to 115200 (UART) / 1000000 (SPI)\n");
        printf("Example 1 Linux: # %s /dev/ttyACM0 threadtun0 0 15 115200\n", argv[0]);
        printf("Example 2 Linux: # %s /dev/ttyACM0 threadtun0 1\n", argv[0]);
        printf("Example OS X:  # ./VTun /dev/cu.usbmodemX /dev/tun12\n");
        printf("\tFor OS X, /dev/tun12 is automatically configured by this program.\n");
        printf("\tIf you choose a different tun device, make sure to configure it yourself.\n");
        exit(1);
    }

#ifdef __linux__spi__
    /* Identify device as UART/SPI. */
    if (strstr(argv[1], "spi")) {
        dev_type = SPI;
        config = defaultSettingsSPI();
    } else
#endif
    {
        dev_type = UART;
        config = defaultConfigurationData();
    }

    /* If a third argument exists, it is the factory_reset bool. */
    if (argc >= 4) {
        use_factory_reset = atoi(argv[3]);
        if (use_factory_reset != 0 && use_factory_reset != 1) {
            printf("Please use 0 or 1 when specifying the factory_reset parameter.\n");
            exit(1);
        }
    }

    /* If a fourth argument exists, it is the 802.15.4 channel. */
    if (argc >= 5) {
        channel = atoi(argv[4]);
        if (channel < 11 || channel > 25) {
            printf("Wrong 802.15.4 channel value, the allowed interval is 11-25.\n");
            exit(1);
        }
    }

    /* If a fifth argument exists, it is the baudrate/speed. */
    if (argc >= 6) {
        if (dev_type == UART) {
            int baudrate = get_baudrate(atoi(argv[5]));
            if (baudrate == -1) {
                printf("Wrong baudrate value.\n");
                exit(1);
            } else {
                setBaudrate(config, baudrate);
            }
        }
#ifdef __linux__spi__
        else {
            setSpeedHzSPI(config, atoi(argv[5]));
        }
#endif
    }

    /* Attach to or create a new tun */
#ifdef __linux__
    tun_fd = tun_alloc(argv[2]);
#elif __APPLE__
    tun_fd = tun_alloc(argv[2]);
    rc = system("ifconfig tun12 inet6 2001:0000:0000:D0B9:0204:9FFF:FE00:FA42");
    if (rc == -1 || rc == 127) {
        printf("Assigning an IPv6 address failed. Exiting..\n");
        exit(1);
    }
    rc = system("route add -inet6 fd00:db8::/64 -interface tun12");
    if (rc == -1 || rc == 127) {
        printf("Adding an IPv6 route failed. Exiting..\n");
        exit(1);
    }
#endif
    if (tun_fd < 0) {
        perror("Allocating interface");
        exit(1);
    }

    /* Initialize framer and device */
    device = InitPhysicalDevice(dev_type, config, argv[1], GLOBAL);
    if (!device) {
        perror("InitPhysicalDevice");
        exit(1);
    }

    rc = OpenPhysicalDevice(device);
    if (rc != HSDK_ERROR_SUCCESS) {
        perror("OpenPhysicalDevice");
        exit(1);
    }

    framer = InitializeFramer(device, FSCI, LENGTH_FIELD_SIZE, CRC_FIELD_SIZE, _LITTLE_ENDIAN);
    if (!framer) {
        perror("InitializeFramer");
        exit(1);
    }

    /* Subscribe to incoming events with the callback */
    AttachToFramer(framer, NULL, callback);

    /* Add signal handler for SIGINT, a.k.a Ctrl-C. */
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        printf("Cannot catch SIGINT\n");
    }

    /* Configure and start the device as a Thread Leader. */
    if (PROVISION) {
        provision(framer, use_factory_reset, channel);
    }

    /* Now read data coming from the kernel */
    while (1) {
#ifdef __APPLE__
        FD_ZERO(&set);
        FD_SET(tun_fd, &set);
        rc = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
        if (rc < 0) {
            perror("Select on TUN fd");
            close(tun_fd);
            exit(1);
        } else if (rc == 0) {
            continue;
        }
#endif
        /* Note that "buffer" should be at least the MTU size of the interface, e.g. 1500 bytes */
        nread = read(tun_fd, request, MTU);
        if (nread < 0) {
            perror("Reading from interface");
            close(tun_fd);
            exit(1);
        }

#if SERIAL_TAP
        if (nread <= ENET_HDR_SIZE || request[ENET_HDR_SIZE] != IPv6_VERSION) {
            continue;
        }
#endif

        /* Add Size field, with respect to endianness */
        Store16((uint16_t)nread, buffer, _LITTLE_ENDIAN);
        /* Add Data field */
        memcpy(buffer + 2, request, MTU);

        FSCIFrame *fsci_request = CreateFSCIFrame(framer, TX_OG, VTUN_TX_OC,
                                  buffer, (uint16_t)nread + 2, 0);
        hex_dump("TX", request, (uint16_t)nread);

        SendFrame(framer, fsci_request);
        /* Free frame memory. */
        DestroyFSCIFrame(fsci_request);
    }

    return 0;
}
