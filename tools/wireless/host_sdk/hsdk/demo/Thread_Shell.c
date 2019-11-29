/*
 * \file Thread_Shell.c
 * Source file that demonstrates THCI over UART/SPI for the Thread IP stack
 * from a simulated user-friendly shell interface.
 *
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define _GNU_SOURCE

#include <arpa/inet.h>
#include <assert.h>
#include <linux/if_tun.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>   // flock
#include <unistd.h>

// netlink includes
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>

#include "Framer.h"
#include "FSCIFrame.h"
#include "PhysicalDevice.h"
#include "SPIConfiguration.h"
#include "UARTConfiguration.h"

#include "Thread_Shell.h"

#define DEBUG                       FALSE
#define FEATURE_NETLINK_DETECT      TRUE
#define TAP_IFNAME                  "threadtap0"
#define UPLINK_IFNAME               "br-lan"      // change with the appropriate interface name here


// TAP name and file descriptor
static char TAP_NAME[IFNAMSIZ] = TAP_IFNAME;
static char UPLINK_NAME[IFNAMSIZ] = UPLINK_IFNAME;
static int tap_fd = -1;

// Variables used to print ping statistics
static char destination[45];
static uint16_t packetsize = 56;    // bytes
static float interval = 1;          // seconds
static volatile bool_t stopPing = FALSE;
static bool_t coapInstAlreadyCreated = FALSE;

// FSCI standard payloads
static uint8_t tuntap_link_buf[]        = {0x01};
static uint8_t inst_id_buf[]            = {THR_INST_ID};
static uint8_t reset_buf[]              = {0xFF, 0x00, 0x00, 0x00};  // timeout in ms
static uint8_t create_buf[]             = {THR_INST_ID};
static uint8_t set_pskd_buf[]           = {THR_INST_ID, 0x19, 0x00, 0x06, 'T', 'H', 'R', 'E', 'A', 'D'};
static uint8_t set_ch_buf[]             = {THR_INST_ID, 0x04, 0x00, 0x01, 0x00};
static uint8_t join_buf[]               = {THR_INST_ID, 0x01};
static uint8_t get_steering_buf[]       = {THR_INST_ID, 0x31, 0x00};
static uint8_t get_randomaddr_buf[]     = {THR_INST_ID, 0x00, 0x00};
static uint8_t sync_nwkdata_buf[]       = {THR_INST_ID};
static uint8_t sync_steering_buf[]      = {THR_INST_ID, 0x01};
static uint8_t add_joiner_default_buf[] = {THR_INST_ID, 0x01, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x06,
                                           'T', 'H', 'R', 'E', 'A', 'D'
                                          };
static uint8_t set_activets_buf[]       = {THR_INST_ID, 0x60, 0x00, 0x08,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // active seconds
                                           0x00, 0x00                           // active ticks
                                          };
static uint8_t ping_buf[37]             = { 0 };    // placeholder, to be completed later
static uint8_t getnodesip_buf[20]       = {THR_INST_ID, IN6ADDR_REALMLOCAL_ALLNODES_INIT, 0x02,
                                           gDiagnosticTlv_ShortAddr_c, gDiagnosticTlv_IPv6AddrList_c
                                          };


#if DEBUG
/* In DEBUG mode, this function is linked by libsys and prints all serial communication. */
void printBuffer(const char *tag, uint8_t *buff, int size)
{

    int i;
    printf("%s: ", tag);

    for (i = 0; i < size; i++) {
        printf("%02X ", buff[i]);
    }

    printf("\n");
}
#endif

/* Wrapper around strtol that checks for NULL input. */
inline long int my_strtol(char *str, int base)
{
    return str ? strtol(str, NULL, base) : 0;
}

/* Prints a buffer as space delimited hex numbers */
static void print_array(const char *tag, uint8_t *buf, int size)
{
    int i;

    printf("%s: ", tag);

    for (i = 0; i < size; i++)
        printf("%02X ", buf[i]);

    printf("\n");
}


/* "0x11223344" -> 0x44, 0x33, 0x22, 0x11 */
static void hex_string_to_byte_array(char *str, uint8_t *buf)
{
    int i;
    char temp[2];

    if (!str) {
        return;
    }

    str = str + 2;  // jump over 0x

    for (i = 0; i < strlen(str); i += 2) {
        memcpy(temp, str + i, 2);
        buf[i / 2] = (uint8_t)strtol(temp, NULL, 16);
    }
}

/* [0x11, 0x22, 0x33, 0x44] -> [0x44, 0x33, 0x22, 0x11]*/
static void reverse_byte_array(uint8_t *buf, int size)
{
    int i;
    uint8_t *temp = malloc(size);

    memcpy(temp, buf, size);

    for (i = 0; i < size; i++) {
        buf[i] = temp[size - i - 1];
    }

    free(temp);

}

/* Convert baudrate value to enum. */
static int get_baudrate(int value)
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

/* Ctrl-C signal handler, only used to stop ping. */
static void sig_handler(int signo)
{
    if (signo == SIGINT) {
        stopPing = TRUE;
    }
}

/* Open a TAP tunnel */
static int tun_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

    if (*dev) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ - 1);
        ifr.ifr_name[IFNAMSIZ - 1] = '\0';
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

/* TAP thread routine */
static void *tap_start(void *arg)
{
    Framer *framer = (Framer *)arg;
    tap_fd = tun_alloc(TAP_NAME);
    int nread;
    uint8_t buffer[MTU + 2], request[MTU];

    while (1) {
        nread = read(tap_fd, request, MTU);

        if (nread < 0) {
            perror("Reading from interface");
            fprintf(stderr, "Please create a TAP device named threadtap0. You can use make_tap.sh.\n");
            close(tap_fd);
            exit(1);
        }

        if (nread <= ENET_HDR_SIZE || request[ENET_HDR_SIZE] != IPv6_VERSION) {
            continue;
        }

        /* Add Size field, with respect to endianness */
        Store16((uint16_t)nread, buffer, _LITTLE_ENDIAN);
        /* Add Data field */
        memcpy(buffer + 2, request, MTU);

        FSCIFrame *fsci_request = CreateFSCIFrame(framer, TX_OG, VTUN_TX_OC,
                                  buffer, (uint16_t)nread + 2, 0);

        SendFrame(framer, fsci_request);
        /* Free frame memory. */
        DestroyFSCIFrame(fsci_request);
    }
}

static void *netlink_start(void *arg)
{
    Framer *framer = (Framer *)arg;
    FSCIFrame *tuntap_link = CreateFSCIFrame(framer, TX_OG, VTUN_LINK_IND_OC, tuntap_link_buf, sizeof(tuntap_link_buf), VIF);

    /* Set-up netlink socket */
    struct sockaddr_nl sa;
    int fd, rc;

    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = RTMGRP_LINK;

    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

    if (fd == -1) {
        perror("socket");
        return NULL;
    }

    rc = bind(fd, (struct sockaddr *) &sa, sizeof(sa));

    if (rc == -1) {
        perror("bind");
        return NULL;
    }

    /* Read from netlink socket */
    int len;
    char buf[8192];     /* 8192 to avoid message truncation on platforms with page size > 4096 */
    struct iovec iov = {buf, sizeof(buf)};
    struct msghdr msg = {&sa, sizeof(sa), &iov, 1, NULL, 0, 0};
    struct nlmsghdr *nh;
    struct ifinfomsg *ifi;
    char nl_ifname[IFNAMSIZ];

    int prevState = -1; /* unknown: -1, down: 0, up: 1*/
    int newState;

    while (1) {
        len = recvmsg(fd, &msg, 0);

        for (nh = (struct nlmsghdr *)buf; NLMSG_OK(nh, len); nh = NLMSG_NEXT(nh, len)) {

            /* The end of multipart message */
            if (nh->nlmsg_type == NLMSG_DONE)
                break;

            if (nh->nlmsg_type == NLMSG_ERROR) {
                /* No error handling */
                break;
            }

            /* Continue with parsing payload */
            ifi = NLMSG_DATA(nh);
            if_indextoname(ifi->ifi_index, nl_ifname);

            if (strcmp(UPLINK_NAME, nl_ifname) != 0) {
                continue;
            }

            switch (nh->nlmsg_type) {

                case RTM_NEWLINK:
                case RTM_DELLINK:
                    newState = ifi->ifi_flags & IFF_RUNNING;

                    if (newState != prevState) {
                        if (newState) {
                            printf("%s: link up\n", nl_ifname);
                            /* Even without STP, the bridge forwarding delay typically adds 15
                               seconds to the recovery time for even the briefest of outages.*/
                            sleep(15);
                            SendFrame(framer, tuntap_link);
                            sleep(1);
                        } else {
                            printf("%s: link down\n", nl_ifname);
                        }
                    }

                    prevState = newState;
                    break;

                default:
                    break;
            }
        }
    }
}

/* Sends a MESHCOP_MgmtGetRequest, OC may be Commissioner|Active|Pending */
static void MESHCOP_MgmtGetRequest(MESHCOP_MgmtGetRequest_t *req, Framer *framer, uint8_t OC)
{
    FSCIFrame *mgmt_get;
    uint8_t *pMsg = NULL;
    uint16_t msgLen = 0, idx = 0;

    /* Sanity check */
    if (!req) {
        return;
    }

    /* Compute the size of the request */
    msgLen += sizeof(uint8_t);  // InstanceId
    msgLen += sizeof(req->IPAddress);  // IPAddress
    msgLen += sizeof(uint8_t);  // NumberOfTlvIds
    msgLen += req->NumberOfTlvIds * sizeof(uint8_t);  // TlvIds

    /* Allocate memory for the marshalled payload */
    pMsg = calloc(msgLen, sizeof(uint8_t));

    if (!pMsg) {
        return;
    }

    /* Serialize */
    pMsg[idx] = req->InstanceId; idx++;
    memcpy(pMsg + idx, req->IPAddress, sizeof(req->IPAddress)); idx += sizeof(req->IPAddress);
    pMsg[idx] = req->NumberOfTlvIds; idx++;
    memcpy(pMsg + idx, req->TlvIds, req->NumberOfTlvIds * sizeof(uint8_t)); idx += req->NumberOfTlvIds * sizeof(uint8_t);

    /* Send the request */
    mgmt_get = CreateFSCIFrame(framer, TX_OG, OC, pMsg, msgLen, VIF);
    SendFrame(framer, mgmt_get); sleep(1);
    DestroyFSCIFrame(mgmt_get);
}

/* Sends a MESHCOP_MgmtSetRequest, OC may be Commissioner|Active|Pending */
static void MESHCOP_MgmtSetRequest(MESHCOP_MgmtSetRequest_t *req, Framer *framer, uint8_t OC)
{
    FSCIFrame *mgmt_set;
    uint8_t *pMsg = NULL;
    uint16_t msgLen = 0, idx = 0;

    /* Sanity check */
    if (!req) {
        return;
    }

    /* Compute the size of the request */
    msgLen += sizeof(uint8_t);  // InstanceId
    msgLen += sizeof(req->IPAddress);  // IPAddress
    msgLen += sizeof(bool_t);  // SessionIdEnable

    if (req->SessionIdEnable) {
        msgLen += 2;
    };  // SessionId

    msgLen += sizeof(bool_t);  // BorderRouterLocatorEnable

    if (req->BorderRouterLocatorEnable) {
        msgLen += 2;
    };  // BorderRouterLocator

    msgLen += sizeof(bool_t);  // NewSessionIdEnable

    if (req->NewSessionIdEnable) {
        msgLen += 2;
    };  // NewSessionId

    msgLen += sizeof(bool_t);  // SteeringDataEnable

    if (req->SteeringDataEnable) {
        msgLen += 1;
    };  // SteeringDataSize

    msgLen += req->SteeringDataSize;  // SteeringData

    msgLen += sizeof(bool_t);  // ChannelEnable

    if (req->ChannelEnable) {
        msgLen += 1;
    };  // Channel

    msgLen += sizeof(bool_t);  // ChannelMaskEnable

    if (req->ChannelMaskEnable) {
        msgLen += 1;
    };  // ChannelPage


    if (req->ChannelMaskEnable) {
        msgLen += 1;
    };  // ChannelMaskLength

    msgLen += req->ChannelMaskLength;  // ChannelMask

    msgLen += sizeof(bool_t);  // XpanIdEnable

    if (req->XpanIdEnable) {
        msgLen += 8;
    }  // XpanId

    msgLen += sizeof(bool_t);  // MLPrefixEnable

    if (req->MLPrefixEnable) {
        msgLen += 16;  // PrefixData
        msgLen += sizeof(uint8_t);  // PrefixLength
    }  // MLPrefix

    msgLen += sizeof(bool_t);  // MasterKeyEnable

    if (req->MasterKeyEnable) {
        msgLen += 16;
    };  // MasterKey

    msgLen += sizeof(bool_t);  // NwkNameEnable

    if (req->NwkNameEnable) {
        msgLen += 1;
    };  // NwkNameSize

    msgLen += req->NwkNameSize;  // NwkName

    msgLen += sizeof(bool_t);  // PanIdEnable

    if (req->PanIdEnable) {
        msgLen += 2;
    };  // PanId

    msgLen += sizeof(bool_t);  // PSKcEnable

    if (req->PSKcEnable) {
        msgLen += 1;
    };  // PskcSize

    msgLen += req->PskcSize;  // PSKc

    msgLen += sizeof(bool_t);  // PolicyEnable

    if (req->PolicyEnable) {
        msgLen += 2;
    };  // RotationInterval

    msgLen += 1;  // O|N|R|C|B  // Policy

    msgLen += sizeof(bool_t);  // ActiveTimestampEnable

    if (req->ActiveTimestampEnable) {
        msgLen += 6;
    };  // ActiveSeconds


    if (req->ActiveTimestampEnable) {
        msgLen += 2;
    };  // ActiveTicks

    msgLen += sizeof(bool_t);  // PendingTimestampEnable

    if (req->PendingTimestampEnable) {
        msgLen += 6;
    };  // PendingSeconds


    if (req->PendingTimestampEnable) {
        msgLen += 2;
    };  // PendingTicks

    msgLen += sizeof(bool_t);  // DelayTimerEnable

    if (req->DelayTimerEnable) {
        msgLen += 4;
    };  // Timeout

    msgLen += sizeof(bool_t);  // FutureTlvEnable

    if (req->FutureTlvEnable) {
        msgLen += sizeof(uint8_t);  // FutureTlvSize
        msgLen += req->FutureTlv.FutureTlvSize;  // FutureTlvValue
    }  // FutureTlv


    /* Allocate memory for the marshalled payload */
    pMsg = calloc(msgLen, sizeof(uint8_t));

    if (!pMsg) {
        return;
    }

    /* Serialize */
    pMsg[idx] = req->InstanceId; idx++;
    memcpy(pMsg + idx, req->IPAddress, sizeof(req->IPAddress)); idx += sizeof(req->IPAddress);
    pMsg[idx] = req->SessionIdEnable; idx++;

    if (req->SessionIdEnable) {
        memcpy(pMsg + idx, &(req->SessionId), sizeof(req->SessionId)); idx += sizeof(req->SessionId);
    }

    pMsg[idx] = req->BorderRouterLocatorEnable; idx++;

    if (req->BorderRouterLocatorEnable) {
        memcpy(pMsg + idx, &(req->BorderRouterLocator), sizeof(req->BorderRouterLocator)); idx += sizeof(req->BorderRouterLocator);
    }

    pMsg[idx] = req->NewSessionIdEnable; idx++;

    if (req->NewSessionIdEnable) {
        memcpy(pMsg + idx, &(req->NewSessionId), sizeof(req->NewSessionId)); idx += sizeof(req->NewSessionId);
    }

    pMsg[idx] = req->SteeringDataEnable; idx++;

    if (req->SteeringDataEnable) {
        pMsg[idx] = req->SteeringDataSize; idx++;
    }

    memcpy(pMsg + idx, req->SteeringData, req->SteeringDataSize); idx += req->SteeringDataSize;

    pMsg[idx] = req->ChannelEnable; idx++;

    if (req->ChannelEnable) {
        pMsg[idx] = req->Channel; idx++;
    }

    pMsg[idx] = req->ChannelMaskEnable; idx++;

    if (req->ChannelMaskEnable) {
        pMsg[idx] = req->ChannelPage; idx++;
    }


    if (req->ChannelMaskEnable) {
        pMsg[idx] = req->ChannelMaskLength; idx++;
    }

    memcpy(pMsg + idx, req->ChannelMask, req->ChannelMaskLength); idx += req->ChannelMaskLength;

    pMsg[idx] = req->XpanIdEnable; idx++;

    if (req->XpanIdEnable) {
        memcpy(pMsg + idx, req->XpanId, 8); idx += 8;
    }

    pMsg[idx] = req->MLPrefixEnable; idx++;

    if (req->MLPrefixEnable) {
        memcpy(pMsg + idx, req->MLPrefix.PrefixData, 16); idx += 16;
        pMsg[idx] = req->MLPrefix.PrefixLength; idx++;
    }

    pMsg[idx] = req->MasterKeyEnable; idx++;

    if (req->MasterKeyEnable) {
        memcpy(pMsg + idx, req->MasterKey, sizeof(req->MasterKey)); idx += sizeof(req->MasterKey);
    }

    pMsg[idx] = req->NwkNameEnable; idx++;

    if (req->NwkNameEnable) {
        pMsg[idx] = req->NwkNameSize; idx++;
    }

    memcpy(pMsg + idx, req->NwkName, req->NwkNameSize); idx += req->NwkNameSize;
    pMsg[idx] = req->PanIdEnable; idx++;

    if (req->PanIdEnable) {
        memcpy(pMsg + idx, &(req->PanId), sizeof(req->PanId)); idx += sizeof(req->PanId);
    }

    pMsg[idx] = req->PSKcEnable; idx++;

    if (req->PSKcEnable) {
        pMsg[idx] = req->PskcSize; idx++;
    }

    memcpy(pMsg + idx, req->PSKc, req->PskcSize); idx += req->PskcSize;
    pMsg[idx] = req->PolicyEnable; idx++;

    if (req->PolicyEnable) {
        memcpy(pMsg + idx, &(req->RotationInterval), sizeof(req->RotationInterval)); idx += sizeof(req->RotationInterval);
    }

    pMsg[idx] = req->Policy; idx++;

    pMsg[idx] = req->ActiveTimestampEnable; idx++;

    if (req->ActiveTimestampEnable) {
        memcpy(pMsg + idx, req->ActiveSeconds, sizeof(req->ActiveSeconds)); idx += sizeof(req->ActiveSeconds);
    }


    if (req->ActiveTimestampEnable) {
        memcpy(pMsg + idx, &(req->ActiveTicks), sizeof(req->ActiveTicks)); idx += sizeof(req->ActiveTicks);
    }

    pMsg[idx] = req->PendingTimestampEnable; idx++;

    if (req->PendingTimestampEnable) {
        memcpy(pMsg + idx, req->PendingSeconds, sizeof(req->PendingSeconds)); idx += sizeof(req->PendingSeconds);
    }


    if (req->PendingTimestampEnable) {
        memcpy(pMsg + idx, &(req->PendingTicks), sizeof(req->PendingTicks)); idx += sizeof(req->PendingTicks);
    }

    pMsg[idx] = req->DelayTimerEnable; idx++;

    if (req->DelayTimerEnable) {
        memcpy(pMsg + idx, &(req->Timeout), sizeof(req->Timeout)); idx += sizeof(req->Timeout);
    }

    pMsg[idx] = req->FutureTlvEnable; idx++;

    if (req->FutureTlvEnable) {
        pMsg[idx] = req->FutureTlv.FutureTlvSize; idx++;
        memcpy(pMsg + idx, req->FutureTlv.FutureTlvValue, req->FutureTlv.FutureTlvSize); idx += req->FutureTlv.FutureTlvSize;
    }

    mgmt_set = CreateFSCIFrame(framer, TX_OG, OC, pMsg, msgLen, VIF);
    SendFrame(framer, mgmt_set); sleep(1);
    DestroyFSCIFrame(mgmt_set);
}


/*
 * Executes on every RX packet.
 */
static void callback(void *callee, void *response)
{
    int i, j, k;
    FSCIFrame *frame = (FSCIFrame *)response;

    // used in ping and ifconfig
    char ip6str[INET6_ADDRSTRLEN];
    uint8_t temp_ip6[16] = { 0 };

    if (frame->opGroup != THR_RX_OG && frame->opGroup != MWS_RX_OG) {
        DestroyFSCIFrame(frame);
        return;
    }

    switch (frame->opCode) {

        case VTUN_LINK_IND_OC:
            break;

        case FACTORY_RESET_OC:
            printf("RX: THR_FactoryReset.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> Success\n");
            }

            break;

        case CPU_RESET_OC:
            printf("RX: THR_McuReset.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> Success\n");
            }

            break;

        case CPU_RESET_IND_OC:
            printf("RX: THR_McuReset.Indication");

            if (frame->data[0] == 0x01) {
                printf(" -> ResetMcuPending\n");
            } else if (frame->data[0] == 0x00) {
                printf(" -> ResetMcuSuccess\n");
            }

            break;

        case THR_EVT_GENERAL_CNF_OC:
            printf("RX: THR_EventGeneral.Confirm -> ");

            if (frame->data[1] == 0x01 && frame->data[2] == 0x00) {
                printf("Disconnected\n");
            } else if (frame->data[1] == 0x02 && frame->data[2] == 0x00) {
                printf("Connected\n");
            } else if (frame->data[1] == 0x03 && frame->data[2] == 0x00) {
                printf("Reset to factory default\n");
                coapInstAlreadyCreated = FALSE;
            } else if (frame->data[1] == 0x04 && frame->data[2] == 0x00) {
                printf("Instance restore started\n");
            } else if (frame->data[1] == 0x05 && frame->data[2] == 0x00) {
                printf("Router Synced\n");
            } else if (frame->data[1] == 0x06 && frame->data[2] == 0x00) {
                printf("End Device Synced\n");
            } else if (frame->data[1] == 0x07 && frame->data[2] == 0x00) {
                printf("Connecting started\n");
            } else if (frame->data[1] == 0x08 && frame->data[2] == 0x00) {
                printf("Connecting failed\n");
            } else if (frame->data[1] == 0x09 && frame->data[2] == 0x00) {
                printf("Connecting deffered\n");
            } else if (frame->data[1] == 0x0A && frame->data[2] == 0x00) {
                printf("Device is Leader\n");
            } else if (frame->data[1] == 0x0B && frame->data[2] == 0x00) {
                printf("Device is Router\n");
            } else if (frame->data[1] == 0x0C && frame->data[2] == 0x00) {
                printf("Device is REED\n");
            } else if (frame->data[1] == 0x0D && frame->data[2] == 0x00) {
                printf("Device is End Device\n");
            } else if (frame->data[1] == 0x0E && frame->data[2] == 0x00) {
                printf("Device is Sleepy End Device\n");
            } else if (frame->data[1] == 0x0F && frame->data[2] == 0x00) {
                printf("Requesting Global Address\n");
            } else if (frame->data[1] == 0x10 && frame->data[2] == 0x00) {
                printf("Global Address assigned\n");
            } else if (frame->data[1] == 0x11 && frame->data[2] == 0x00) {
                printf("Requesting Router Id\n");
            } else if (frame->data[1] == 0x12 && frame->data[2] == 0x00) {
                printf("Router Id assigned\n");
            } else if (frame->data[1] == 0x13 && frame->data[2] == 0x00) {
                printf("Router Id assigned failed\n");
            } else if (frame->data[1] == 0x14 && frame->data[2] == 0x00) {
                printf("Allow device to sleep\n");
            } else if (frame->data[1] == 0x15 && frame->data[2] == 0x00) {
                printf("Disallow device to sleep\n");
            } else if (frame->data[1] == 0x16 && frame->data[2] == 0x00) {
                printf("Child Id assigned\n");
            } else if (frame->data[1] == 0x17 && frame->data[2] == 0x00) {
                printf("Visualization: Child Removed\n");
            } else if (frame->data[1] == 0x18 && frame->data[2] == 0x00) {
                printf("Visualization: Router Removed\n");
            } else if (frame->data[1] == 0x19 && frame->data[2] == 0x00) {
                printf("Visualization: Routing Updates\n");
            } else {
                printf("Status %02x\n", frame->data[1]);
            }

            break;

        case THR_SET_ATTR_OC:
            printf("RX: THR_SetAttr.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> Success\n");
            }

            break;

        case THR_CREATE_NWK_OC:
            printf("RX: THR_CreateNwk.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK\n");
            } else if (frame->data[0] == 0x0B) {
                printf(" -> Already Created\n");
            }

            break;

        case THR_JOIN_NWK_OC:
            printf("RX: THR_Join.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK\n");
            } else if (frame->data[0] == 0x0A) {
                printf(" -> Already Connected\n");
            }

            break;

        case THR_EVT_NWK_CREATE_CNF_OC:
            printf("RX: THR_EventNwkCreate.Confirm");

            if (frame->data[1] == 0x03 && frame->data[2] == 0x00) {
                printf(" -> Select best channel\n");
            } else if (frame->data[1] == 0x04 && frame->data[2] == 0x00) {
                printf(" -> Generate PSKc\n");
            } else if (frame->data[1] == 0x01 && frame->data[2] == 0x00) {
                printf(" -> Success\n");
            }

            break;

        case THR_EVT_NWK_JOIN_IND_OC:
            printf("RX: THR_EventNwkJoin.Indication");

            if (frame->data[1] == 0x01 && frame->data[2] == 0x00) {
                printf(" -> Attaching\n");
            } else if (frame->data[1] == 0x02 && frame->data[2] == 0x00) {
                printf(" -> Join Success\n");
            }

            break;

        case THR_EVT_NWK_COMM_IND_OC:
            if (frame->data[1] == 0x0D && frame->data[2] == 0x00) {
                // avoid flooding the console with keep-alive
                break;
            }

            printf("RX: THR_EventNwkCommissioning.Indication");

            if (frame->data[1] == 0x01 && frame->data[2] == 0x00) {
                printf(" -> Joiner discovery started\n");
            } else if (frame->data[1] == 0x02 && frame->data[2] == 0x00) {
                printf(" -> Joiner discovery failed - no beacon\n");
            } else if (frame->data[1] == 0x03 && frame->data[2] == 0x00) {
                printf(" -> Joiner discovery failed - device is filtered\n");
            } else if (frame->data[1] == 0x04 && frame->data[2] == 0x00) {
                printf(" -> Joiner discovery success\n");
            } else if (frame->data[1] == 0x05 && frame->data[2] == 0x00) {
                printf(" -> Joiner DTLS session started\n");
            } else if (frame->data[1] == 0x06 && frame->data[2] == 0x00) {
                printf(" -> Joiner DTLS error\n");
            } else if (frame->data[1] == 0x07 && frame->data[2] == 0x00) {
                printf(" -> Joiner error\n");
            } else if (frame->data[1] == 0x08 && frame->data[2] == 0x00) {
                printf(" -> Joiner accepted\n");
            } else if (frame->data[1] == 0x09 && frame->data[2] == 0x00) {
                printf(" -> Commissioner petition started\n");
            } else if (frame->data[1] == 0x0A && frame->data[2] == 0x00) {
                printf(" -> Commissioner petition accepted\n");
            } else if (frame->data[1] == 0x0B && frame->data[2] == 0x00) {
                printf(" -> Commissioner petition rejected\n");
            } else if (frame->data[1] == 0x0C && frame->data[2] == 0x00) {
                printf(" -> Commissioner petition alert\n");
            } else if (frame->data[1] == 0x0E && frame->data[2] == 0x00) {
                printf(" -> Commissioner error\n");
            } else if (frame->data[1] == 0x0F && frame->data[2] == 0x00) {
                printf(" -> Commissioner Joiner DTLS session started\n");
            } else if (frame->data[1] == 0x10 && frame->data[2] == 0x00) {
                printf(" -> Joiner DTLS alert\n");
            } else if (frame->data[1] == 0x11 && frame->data[2] == 0x00) {
                printf(" -> Commissioner Joiner accepted\n");
            } else if (frame->data[1] == 0x13 && frame->data[2] == 0x00) {
                printf(" -> Commissioner<-BR DTLS Session Started\n");
            } else if (frame->data[1] == 0x14 && frame->data[2] == 0x00) {
                printf(" -> Commissioner<-BR DTLS Error\n");
            } else if (frame->data[1] == 0x15 && frame->data[2] == 0x00) {
                printf(" -> Commissioner<-BR Error\n");
            } else if (frame->data[1] == 0x16 && frame->data[2] == 0x00) {
                printf(" -> External Commissioner accepted\n");
            } else if (frame->data[1] == 0x17 && frame->data[2] == 0x00) {
                printf(" -> Commissioner->BR DTLS Session Started\n");
            } else if (frame->data[1] == 0x18 && frame->data[2] == 0x00) {
                printf(" -> Commissioner->BR DTLS Error\n");
            } else if (frame->data[1] == 0x19 && frame->data[2] == 0x00) {
                printf(" -> Commissioner->BR Accepted\n");
            } else if (frame->data[1] == 0x1A && frame->data[2] == 0x00) {
                printf(" -> Commissioner->BR Inbound Data Relayed\n");
            } else if (frame->data[1] == 0x1B && frame->data[2] == 0x00) {
                printf(" -> Commissioner<-BR Outbound Data Relayed\n");
            } else if (frame->data[1] == 0x1C && frame->data[2] == 0x00) {
                printf(" -> Commissioner<-Joiner Outbound Data Relayed\n");
            } else if (frame->data[1] == 0x1D && frame->data[2] == 0x00) {
                printf(" -> Commissioner->Joiner Inbound Data Relayed\n");
            } else if (frame->data[1] == 0x1E && frame->data[2] == 0x00) {
                printf(" -> Providing the security material to the Joiner\n");
            } else {
                printf(" -> %d\n", frame->data[1]);
            }

            break;

        case THR_EVT_COMM_DIAG_IND_OC:
            printf("RX: THR_CommissioningDiagnostic.Indication");

            if (frame->data[0] == 0x01) {
                printf(" -> IN");
            } else if (frame->data[0] == 0x00) {
                printf(" -> OUT");
            }

            if (frame->data[1] == 0x00) {
                printf(" JOIN_FIN_REQ\n");
            } else if (frame->data[1] == 0x01) {
                printf(" JOIN_FIN_RSP\n");
            } else if (frame->data[1] == 0x02) {
                printf(" JOIN_ENT_REQ\n");
            } else if (frame->data[1] == 0x03) {
                printf(" JOIN_ENT_RSP\n");
            } else if (frame->data[1] == 0x04) {
                printf(" DTLS_CLOSE_NOTIFY\n");
            } else if (frame->data[1] == 0x03) {
                printf(" DTLS_CLOSE_NOTIFY\n");
            }

            break;

        case THR_GET_IP_ADDR_OC:
            printf("RX: THR_GetThreadIpAddr.Confirm");

            if (frame->data[1] == 0x00) {
                for (i = 0; i < 16; i++) {
                    temp_ip6[i] = frame->data[4 + 15 - i];
                }

                inet_ntop(AF_INET6, temp_ip6, ip6str, INET6_ADDRSTRLEN);
                printf(" -> %s\n", ip6str);
            } else {
                printf(" -> no address available\n");
            }

            break;

        case THR_MGMT_DIAG_GET_OC:
            printf("RX: THR_MgmtDiagnosticGet.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK!\n");
            } else {
                printf(" -> FAIL!\n");
            }

            break;

        case THR_MGMT_DIAG_GET_IND_OC:
            printf("RX: THR_MgmtDiagnosticGetRsp.Indication");

            if (frame->data[0] == 0x00) {
                printf(" -> OK!\n");
            } else {
                printf(" -> FAIL!\n");
                break;
            }

            uint16_t dataLen = frame->data[3] + (frame->data[4] << 8);
            uint16_t offset = 0;

            while (dataLen > 0) {
                uint8_t tlvType, tlvLength;

                tlvType = frame->data[6 + offset];
                tlvLength = frame->data[7 + offset];

                if (tlvType != gDiagnosticTlv_ShortAddr_c && tlvType != gDiagnosticTlv_IPv6AddrList_c) {
                    printf("TLV type %d not currently processed!\n", tlvType);
                }

                if (tlvType == gDiagnosticTlv_ShortAddr_c) {
                    uint16_t shortAddr = (frame->data[8 + offset] << 8) + frame->data[9 + offset];
                    printf("\tNode Short Address: %04X\n", shortAddr);
                }

                if (tlvType == gDiagnosticTlv_IPv6AddrList_c) {

                    if (tlvLength % 16 != 0) {
                        printf("Invalid payload!\n");

                    } else {
                        for (i = 0; i < tlvLength / 16; i++) {
                            inet_ntop(AF_INET6, &frame->data[8 + offset + 16 * i], ip6str, INET6_ADDRSTRLEN);
                            printf("\t\t%s\n", ip6str);
                        }
                    }
                }

                offset += (2 + tlvLength);
                dataLen -= (2 + tlvLength);
            }

            break;

        case MESHCOP_ADD_JOINER_OC:
            printf("RX: MESHCOP_AddExpectedJoiner.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK!\n");
            } else {
                printf(" -> FAIL!\n");
            }

            break;

        case MESHCOP_SYNC_STEERING_OC:
            printf("RX: MESHCOP_SyncSteeringData.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK!\n");
            } else {
                printf(" -> FAIL!\n");
            }

            break;

        case MESHCOP_START_COMM:
        case MESHCOP_STOP_COMM:
            printf("RX: MESHCOP_(Start|Stop)Commissioner.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK!\n");
            } else {
                printf(" -> FAIL!\n");
            }

            break;

        case MESHCOP_MGMT_SET_OC:
        case MESHCOP_COMM_SET_OC:
        case MESHCOP_ACTIVE_SET_OC:
        case MESHCOP_PENDING_SET_OC:
            printf("RX: MESHCOP_Mgmt*Set.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK!\n");
            } else {
                printf(" -> FAIL!\n");
            }

            break;

        case MESHCOP_COMM_GET_OC:
        case MESHCOP_ACTIVE_GET_OC:
        case MESHCOP_PENDING_GET_OC:
            printf("RX: MESHCOP_Mgmt*Get.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK!\n");
            } else {
                printf(" -> FAIL!\n");
                break;
            }

            uint32_t total_length = (frame->data[1]) +
                                    (frame->data[2] << 8) +
                                    (frame->data[3] << 16) +
                                    (frame->data[4] << 24);

            printf("TOTAL_LENGTH of TVLs %d\n", total_length);
            uint32_t idx = 5;

            while (total_length) {
                uint8_t type = frame->data[idx], length = frame->data[idx + 1];
                printf("TYPE: 0x%02x ", type);
                printf("LEN: 0x%02x ", length);
                idx += 2;

                print_array("VALUE", frame->data + idx, length);
                idx += length;
                total_length -= (length + 2);
                printf("REMAINING_LENGTH of TVLs %d\n", total_length);
            }

            break;

        case MESHCOP_PANID_QUERY_OC:
            printf("RX: MESHCOP_MgmtSendPanIdQuery.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK!\n");
            } else {
                printf(" -> FAIL!\n");
            }

            break;

        case MESHCOP_PANID_CONFLICT_OC:
            printf("RX: MESHCOP_MgmtPanIdConflict.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK!\n");
            } else {
                printf(" -> FAIL!\n");
            }

            break;

        case THR_BR_SYNC_OC:
            printf("RX: THR_BrPrefixSync.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK!\n");
            } else {
                printf(" -> FAIL!\n");
            }

            break;

        case THR_BR_ADD_ENTRY_OC:
            printf("RX: THR_BrPrefixAddEntry.Confirm");

            if (frame->data[0] == 0x00) {
                printf(" -> OK!\n");
            } else {
                printf(" -> FAIL!\n");
            }

            break;

        case IFCONFIG_OC:
            printf("RX: NWKU_IfconfigAll.Response -> ");
            int curr = 0;
            uint8_t CountInterfaces = frame->data[curr]; curr++;
            printf("CountInterfaces %d", CountInterfaces);

            for (i = 0; i < CountInterfaces; i++) {

                if (curr == frame->length) {
                    break;
                }

                uint8_t InterfaceID = frame->data[curr]; curr++;
                printf("\nInterface ID %d ", InterfaceID);
                uint8_t CountIpAddresses = frame->data[curr]; curr++;
                printf("CountIpAddresses %d\n", CountIpAddresses);

                for (j = 0; j < CountIpAddresses; j++) {

                    for (k = 0; k < 16; k++) {
                        temp_ip6[k] = frame->data[curr + 15 - k];
                    }

                    curr += 16;

                    inet_ntop(AF_INET6, temp_ip6, ip6str, INET6_ADDRSTRLEN);
                    printf("\t%s\n", ip6str);
                }
            }

            break;

        case NWKU_PING_OC:
            if (frame->data[0] == 0x00) {
                uint16_t time = frame->data[1] + (frame->data[2] << 8);
                printf("Reply from %s: bytes=%d time=%dms\n", destination, packetsize, time);
            } else {
                printf("Request timed out\n");
            }

            break;

        case COAP_CREATE_INST_OC:
            if (frame->data[0] == 0x00) {
                printf("RX: NWKU_CoapCreateInstance.Confirm -> OK!\n");
            } else {
                printf("RX: NWKU_CoapCreateInstance.Confirm -> FAIL!\n");
            }

            break;

        case COAP_SEND_OC:
            if (frame->data[0] != 0x00) {
                printf("RX: NWKU_CoapSend.Confirm -> FAIL!\n");
            }

            break;

        case COAP_MSG_RCVD_OC:
            printf("RX: NWKU_CoapMsgReceived.Indication\n");
            printf("\tStatus -> ");

            if (frame->data[0] == 0x00) {
                printf("Success\n");
            } else if (frame->data[0] == 0x01) {
                printf("Failed\n");
                break;
            } else if (frame->data[0] == 0x02) {
                printf("Duplicate\n");
                break;
            }

            uint8_t payloadLength = frame->data[51];

            if (payloadLength) {
                char payload[0xFF] = { 0 };
                strncpy(payload, (const char *)&frame->data[52], payloadLength);
                printf("\tPayload -> %s\n", payload);
            }

            break;

        case MWS_REQ_ACCESS_OC:
        case MWS_REL_ACCESS_OC:
            if (frame->data[0] == 0x00) {
                printf("Line asserted!\n");
            } else {
                printf("Request denied!\n");
            }

            break;

        case THR_GET_ATTR_OC:
            if (frame->data[1] == 0x31) {
                if (frame->data[4] == 0x00) {
                    printf("RX: THR_GetAttr.Confirm SteeringData is NULL. Please issue `thr create` again!\n");
                } else {
                    printf("RX: THR_GetAttr.Confirm SteeringData -> OK!\n");
                }
            }

            if (frame->data[1] == 0x00) {
                if (frame->data[4] == 0x00) {
                    printf("RX: THR_GetAttr.Confirm RandomExtendedAddr an error occurred\n");
                } else {
                    printf("RX: THR_GetAttr.Confirm RandomExtendedAddr -> 0x");

                    for (i = 7; i >= 0; i--) {
                        printf("%02x", frame->data[i + 5]);
                    }
                }
            }

            break;

        case VTUN_RX_OC:
            // add an advisory lock on tunnel
            flock(tap_fd, LOCK_EX);
            int rc = write(tap_fd, frame->data, frame->length);

            if (rc == -1) {
                perror("TUN write");
            }

            flock(tap_fd, LOCK_UN);
            break;

        default:
            printf("RX: unknown frame (OG, OC) = (%02x, %02x)\n", frame->opGroup, frame->opCode);
            break;
    }

    DestroyFSCIFrame(frame);
}


static void configure_for_harness(Framer *framer)
{
    /* Network Key */
    uint8_t buf1[] = {0x00, 0x16, 0x00, 0x10,
                      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
                     };
    FSCIFrame *set_nwk_key = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC, buf1, sizeof(buf1), VIF);

    /* Network Name */
    uint8_t buf2[] = {0x00, 0x10, 0x00, 0x03,
                      'G', 'R', 'L'
                     };
    FSCIFrame *set_nwk_name = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC, buf2, sizeof(buf2), VIF);

    /* PAN ID */
    uint8_t buf3[] = {0x00, 0x05, 0x00, 0x02,
                      0xCE, 0xFA
                     };
    FSCIFrame *set_panid = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC, buf3, sizeof(buf3), VIF);

    /* Extended PAN ID */
    uint8_t buf4[] = {0x00, 0x06, 0x00, 0x08,
                      0x00, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00
                     };
    FSCIFrame *set_epanid = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC, buf4, sizeof(buf4), VIF);

    /* ML Prefix */
    uint8_t buf5[] = {0x00, 0x1C, 0x00, 0x11,
                      0xFD, 0x00, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x40
                     };
    FSCIFrame *set_ml_prefix = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC, buf5, sizeof(buf5), VIF);

    /* PSKc */
    uint8_t buf6[] = {0x00, 0x18, 0x00, 0x0F,
                      't', 'h', 'r', 'e', 'a', 'd', 'j', 'p', 'a', 'k', 'e', 't', 'e', 's', 't'
                     };
    FSCIFrame *set_pskc = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC, buf6, sizeof(buf6), VIF);

    /* Security Policy Rotation Interval */
    uint8_t buf7[] = {0x00, 0x20, 0x00, 0x04,
                      0x10, 0x0E, 0x00, 0x00
                     };
    FSCIFrame *set_policy = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC, buf7, sizeof(buf7), VIF);

    printf("TX: THR_SetAttr.Request Network Key\n");
    SendFrame(framer, set_nwk_key); sleep(1);
    DestroyFSCIFrame(set_nwk_key);

    printf("TX: THR_SetAttr.Request Network Name\n");
    SendFrame(framer, set_nwk_name); sleep(1);
    DestroyFSCIFrame(set_nwk_name);

    printf("TX: THR_SetAttr.Request PAN ID\n");
    SendFrame(framer, set_panid); sleep(1);
    DestroyFSCIFrame(set_panid);

    printf("TX: THR_SetAttr.Request Extended PAN ID\n");
    SendFrame(framer, set_epanid); sleep(1);
    DestroyFSCIFrame(set_epanid);

    printf("TX: THR_SetAttr.Request ML Prefix\n");
    SendFrame(framer, set_ml_prefix); sleep(1);
    DestroyFSCIFrame(set_ml_prefix);

    printf("TX: THR_SetAttr.Request PSKc\n");
    SendFrame(framer, set_pskc); sleep(6);
    DestroyFSCIFrame(set_pskc);

    printf("TX: THR_SetAttr.Request Security Policy Rotation Interval\n");
    SendFrame(framer, set_policy); sleep(1);
    DestroyFSCIFrame(set_policy);
}


static bool_t add_joiners(Framer *framer, char *command)
{
    char *token;
    FSCIFrame *add_joiner;

    token = strtok(command, " ");   // thr
    token = strtok(NULL, " ");      // addjoiners
    token = strtok(NULL, " ");      // psk

    if (token) {
        uint8_t psk_size, *add_joiner_buf;

        if (strlen(token) > 255) {
            printf("PSK length exceeded");
            return FALSE;
        }

        psk_size = strlen(token) - 1;  // -1 for \n
        add_joiner_buf = calloc(12 + psk_size, sizeof(uint8_t));

        if (!add_joiner_buf) {
            return FALSE;
        }

        add_joiner_buf[0] = THR_INST_ID;
        add_joiner_buf[1] = TRUE; // Selected
        add_joiner_buf[2] = 0x01; // LongEUI
        memset(add_joiner_buf + 3, 0xFF, 8);
        add_joiner_buf[11] = psk_size;
        memcpy(add_joiner_buf + 12, token, psk_size);

        add_joiner = CreateFSCIFrame(framer, TX_OG, MESHCOP_ADD_JOINER_OC, add_joiner_buf, 12 + psk_size, VIF);
        printf("TX: MESHCOP_AddExpectedJoiner.Request\n");
        SendFrame(framer, add_joiner); sleep(1);
        free(add_joiner_buf);

    } else {
        printf("No PSKd provided, using `THREAD`\n");
        printf("TX: MESHCOP_AddExpectedJoiner.Request\n");
        add_joiner = CreateFSCIFrame(framer, TX_OG, MESHCOP_ADD_JOINER_OC, add_joiner_default_buf, sizeof(add_joiner_default_buf), VIF);
        SendFrame(framer, add_joiner); sleep(1);
    }

    return TRUE;
}

static void add_br_entry(Framer *framer, char *command)
{
    char *token, *type, *prefix;
    long int plen, lifetime;

    token = strtok(command, " ");   // thr
    token = strtok(NULL, " ");      // nwkdata
    token = strtok(NULL, " ");      // add
    token = strtok(NULL, " ");      // slaac | dhcp | extroute
    type = strdup(token);
    token = strtok(NULL, " ");      // -p

    token = strtok(NULL, " ");      // prefix

    if (token) {
        prefix = strdup(token);
    } else {
        printf("Invalid parameters\n");
        return;
    }

    token = strtok(NULL, " ");      // -l

    token = strtok(NULL, " ");      // length

    if (token) {
        plen = strtol(token, NULL, 10);
    } else {
        printf("Invalid parameters\n");
        return;
    }

    if (plen <= 0 || plen > 128) {
        printf("prefix length in wrong format! should be 1 - 128\n");
        return;
    }

    token = strtok(NULL, " ");      // -t

    token = strtok(NULL, " ");      // lifetime

    if (token) {
        lifetime = strtol(token, NULL, 16);
    } else {
        printf("Invalid parameters\n");
        return;
    }

    if (lifetime == 0) {
        printf("lifetime in wrong format! should be 0x11223344\n");
        return;
    }

    printf("%s::/%ld 0x%08lx\n", prefix, plen, lifetime);
    uint8_t flags = 0, prefixAdv = 0, extRouteAdv = 0;

    if (strcmp(type, "slaac") == 0) {
        flags = 0x11;
        prefixAdv = 1;
    } else if (strcmp(type, "dhcp") == 0) {
        flags = 0x09;
        prefixAdv = 1;
    } else if (strcmp(type, "extroute") == 0) {
        extRouteAdv = 1;
    } else {
        printf("invalid type selected! please choose between slaac | dhcp | extroute\n");
        return;
    }

    free(type);

    uint8_t buf_nwk_prefix[31] = { 0 };
    buf_nwk_prefix[1] = (uint8_t)plen;

    if (inet_pton(AF_INET6, prefix, &buf_nwk_prefix[2]) != 1) {
        printf("invalid IPv6 string!\n");
        return;
    }

    free(prefix);

    buf_nwk_prefix[19] = flags;
    buf_nwk_prefix[20] = (lifetime & 0xFF000000) >> 24;
    buf_nwk_prefix[21] = (lifetime & 0x00FF0000) >> 16;
    buf_nwk_prefix[22] = (lifetime & 0x0000FF00) >> 8;
    buf_nwk_prefix[23] = (lifetime & 0x000000FF);
    buf_nwk_prefix[24] = prefixAdv;
    buf_nwk_prefix[26] = (lifetime & 0xFF000000) >> 24;
    buf_nwk_prefix[27] = (lifetime & 0x00FF0000) >> 16;
    buf_nwk_prefix[28] = (lifetime & 0x0000FF00) >> 8;
    buf_nwk_prefix[29] = (lifetime & 0x000000FF);
    buf_nwk_prefix[30] = extRouteAdv;

    FSCIFrame *add_prefix = CreateFSCIFrame(framer, TX_OG, THR_BR_ADD_ENTRY_OC, buf_nwk_prefix, sizeof(buf_nwk_prefix), VIF);
    SendFrame(framer, add_prefix); sleep(1);
}

static void mgmt_get_tlvs(Framer *framer)
{
    printf("NAME         SIZE DESC              PARAMS                                EXAMPLE\n");
    printf("----------   --------------------------------------------------------------------\n");
    printf("sid          2    Session ID        value (hex)                           0x0001\n");
    printf("brloc        2    BR Locator        value (hex)                           0x0400\n");
    printf("newsid       2    New Session ID    value (hex)                           0x0002\n");
    printf("steering     var  Steering Data     size (dec) + value (hex)              3 0x112233\n");
    printf("ch           1    Channel           value (dec)                           11\n");
    printf("chmask       4    Channel Mask      value (hex)                           0x07FFF800\n");
    printf("xpanid       8    Extended PAN ID   value (hex)                           0x1122334455667788\n");
    printf("mlprefix     17   ML Prefix         IPv6 prefix (str) + plen (dec)        fd01::3ead 64\n");
    printf("masterkey    16   Master Key        value (hex)                           0x0011..FF\n");
    printf("nwkname      var  Network Name      size (dec) + value (str)              6 THREAD\n");
    printf("panid        2    PAN ID            value (hex)                           0xFACE\n");
    printf("pskc         var  PSKc              size (dec) + value (str)              7 kinetis\n");
    printf("policy       3    Security Policy   rot interval (hex) + policy (hex)     0x1122 0x01 (B|C|R|N|O)\n");
    printf("activets     6    Active Timestamp  seconds (hex)                         0x112233445566\n");
    printf("pendingts    6    Pending Timestamp seconds (hex)                         0x112233445566\n");
    printf("delaytmr     4    Delay Timer       timeout (hex)                         0x11223344\n");
    printf("futuretlv    var  Future TLV        size (dec) + value (hex)              3 0x112233\n");
    printf("scanduration var  Scan Duration     size (dec) + value (hex)              only for active get\n");
    printf("energylist   var  Energy List       size (dec) + value (hex)              only for active get\n");
}

/* Mgmt set command parser */
static void mgmt_set(Framer *framer, char *command, uint8_t OC)
{
    int i;
    MESHCOP_MgmtSetRequest_t req = { 0 };
    char *token, *addr, *name = NULL;

    token = strtok(command, " ");   // thr
    token = strtok(NULL, " ");      // mgmt
    token = strtok(NULL, " ");      // activeset | pendingset | commset

    token = strtok(NULL, " ");      // addr

    if (token) {
        addr = strdup(token);
    } else {
        printf("Invalid parameters\n");
        return;
    }

    token = strtok(NULL, " ");      // tlvname

    if (token) {
        name = strdup(token);
    } else {
        printf("Invalid parameters\n");
        return;
    }

    uint8_t temp[16];

    if (inet_pton(AF_INET6, addr, temp) != 1) {
        printf("invalid IPv6 string!\n");
        return;
    }

    free(addr);

    // reverse IPv6 bytes
    for (i = 0; i < 16; i++) {
        req.IPAddress[i] = temp[15 - i];
    }

    while (name) {

        if (strncmp(name, "sid", strlen("sid")) == 0) {
            req.SessionIdEnable = TRUE;
            req.SessionId = my_strtol(strtok(NULL, " "), 16);

        } else if (strncmp(name, "brloc", strlen("brloc")) == 0) {
            req.BorderRouterLocatorEnable = TRUE;
            req.BorderRouterLocator = my_strtol(strtok(NULL, " "), 16);

        } else if (strncmp(name, "newsid", strlen("newsid")) == 0) {
            req.NewSessionIdEnable = TRUE;
            req.NewSessionId = my_strtol(strtok(NULL, " "), 16);

        } else if (strncmp(name, "steering", strlen("steering")) == 0) {
            req.SteeringDataEnable = TRUE;
            req.SteeringDataSize = my_strtol(strtok(NULL, " "), 10);
            req.SteeringData = calloc(req.SteeringDataSize, sizeof(uint8_t));
            hex_string_to_byte_array(strtok(NULL, " "), req.SteeringData);
            reverse_byte_array(req.SteeringData, req.SteeringDataSize);

        } else if (strncmp(name, "chmask", strlen("chmask")) == 0) {
            req.ChannelMaskEnable = TRUE;
            req.ChannelPage = 0;
            req.ChannelMaskLength = 4;
            req.ChannelMask = calloc(req.ChannelMaskLength, sizeof(uint8_t));
            hex_string_to_byte_array(strtok(NULL, " "), req.ChannelMask);
            reverse_byte_array(req.ChannelMask, req.ChannelMaskLength);

        } else if (strncmp(name, "ch", strlen("ch")) == 0) {
            req.ChannelEnable = TRUE;
            req.Channel = my_strtol(strtok(NULL, " "), 10);

        } else if (strncmp(name, "xpanid", strlen("xpanid")) == 0) {
            req.XpanIdEnable = TRUE;
            hex_string_to_byte_array(strtok(NULL, " "), req.XpanId);
            reverse_byte_array(req.XpanId, sizeof(req.XpanId));

        } else if (strncmp(name, "mlprefix", strlen("mlprefix")) == 0) {
            req.MLPrefixEnable = TRUE;

            if (inet_pton(AF_INET6, strtok(NULL, " "), req.MLPrefix.PrefixData) != 1) {
                printf("Wrong IPv6 string for mlprefix\n");
                return;
            }

            req.MLPrefix.PrefixLength = my_strtol(strtok(NULL, " "), 10);

        } else if (strncmp(name, "masterkey", strlen("masterkey")) == 0) {
            req.MasterKeyEnable = TRUE;
            hex_string_to_byte_array(strtok(NULL, " "), req.MasterKey);
            reverse_byte_array(req.MasterKey, sizeof(req.MasterKey));

        } else if (strncmp(name, "nwkname", strlen("nwkname")) == 0) {
            req.NwkNameEnable = TRUE;
            req.NwkNameSize = my_strtol(strtok(NULL, " "), 10);
            req.NwkName = strdup(strtok(NULL, " "));

        } else if (strncmp(name, "panid", strlen("panid")) == 0) {
            req.PanIdEnable = TRUE;
            req.PanId = my_strtol(strtok(NULL, " "), 16);

        } else if (strncmp(name, "pskc", strlen("pskc")) == 0) {
            req.PSKcEnable = TRUE;
            req.PskcSize = my_strtol(strtok(NULL, " "), 10);
            req.PSKc = strdup(strtok(NULL, " "));

        } else if (strncmp(name, "policy", strlen("policy")) == 0) {
            req.PolicyEnable = TRUE;
            req.RotationInterval = my_strtol(strtok(NULL, " "), 16);
            req.Policy = my_strtol(strtok(NULL, " "), 16);

        } else if (strncmp(name, "activets", strlen("activets")) == 0) {
            req.ActiveTimestampEnable = TRUE;
            hex_string_to_byte_array(strtok(NULL, " "), req.ActiveSeconds);
            reverse_byte_array(req.ActiveSeconds, sizeof(req.ActiveSeconds));

        } else if (strncmp(name, "pendingts", strlen("pendingts")) == 0) {
            req.PendingTimestampEnable = TRUE;
            hex_string_to_byte_array(strtok(NULL, " "), req.PendingSeconds);
            reverse_byte_array(req.PendingSeconds, sizeof(req.PendingSeconds));

        } else if (strncmp(name, "delaytmr", strlen("delaytmr")) == 0) {
            req.DelayTimerEnable = TRUE;
            req.Timeout = my_strtol(strtok(NULL, " "), 16);

        } else if (strncmp(name, "futuretlv", strlen("futuretlv")) == 0) {
            req.FutureTlvEnable = TRUE;
            req.FutureTlv.FutureTlvSize = my_strtol(strtok(NULL, " "), 10);
            req.FutureTlv.FutureTlvValue = calloc(req.FutureTlv.FutureTlvSize, sizeof(uint8_t));
            hex_string_to_byte_array(strtok(NULL, " "), req.FutureTlv.FutureTlvValue);
            reverse_byte_array(req.FutureTlv.FutureTlvValue, sizeof(req.FutureTlv.FutureTlvValue));
        } else {
            printf("Parameter not recognized! Please see > thr mgmt gettlvs\n");
            return;
        }

        // get another tlv
        free(name);
        token = strtok(NULL, " ");      // tlvname

        if (token) {
            name = strdup(token);
        } else {
            name = NULL;
        }
    }

    MESHCOP_MgmtSetRequest(&req, framer, OC);
}

/* Mgmt get command parser */
static void mgmt_get(Framer *framer, char *command, uint8_t OC)
{
    int i;
    MESHCOP_MgmtGetRequest_t req = { 0 };
    char *token, *addr, *name = NULL;
    uint8_t temp[20];

    token = strtok(command, " ");   // thr
    token = strtok(NULL, " ");      // mgmt
    token = strtok(NULL, " ");      // activeget | pendingget | commget

    token = strtok(NULL, " ");      // addr

    if (token) {
        addr = strdup(token);
    } else {
        printf("Invalid parameters\n");
        return;
    }

    token = strtok(NULL, " ");      // tlvname

    if (token) {
        name = strdup(token);
    } else {
        printf("Sending an empty request to get back all available TLVs..\n");
        // remove ENTER from addr
        addr[strlen(addr) - 1] = 0;
    }

    uint8_t temp_addr[16];

    if (inet_pton(AF_INET6, addr, temp_addr) != 1) {
        printf("invalid IPv6 string!\n");
        return;
    }

    // reverse IPv6 bytes
    for (i = 0; i < 16; i++) {
        req.IPAddress[i] = temp_addr[15 - i];
    }

    while (name) {

        if (strncmp(name, "brloc", strlen("brloc")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_BorderRouterLocator;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "sid", strlen("sid")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_CommissionerSessionId;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "steering", strlen("steering")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_SteeringData;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "chmask", strlen("chmask")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_ChannelMask;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "ch", strlen("ch")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_Channel;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "xpanid", strlen("xpanid")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_XpanId;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "mlprefix", strlen("mlprefix")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_MeshLocalUla;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "masterkey", strlen("masterkey")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_MasterKey;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "nwkname", strlen("nwkname")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_NetworkName;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "panid", strlen("panid")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_PanId;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "pskc", strlen("pskc")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_PSKc;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "policy", strlen("policy")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_SecurityPolicy;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "activets", strlen("activets")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_ActiveTimestamp;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "pendingts", strlen("pendingts")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_PendingTimestamp;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "delaytmr", strlen("delaytmr")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_DelayTimer;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "scanduration", strlen("scanduration")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_ScanDuration;
            req.NumberOfTlvIds++;

        } else if (strncmp(name, "energylist", strlen("energylist")) == 0) {
            temp[req.NumberOfTlvIds] = (uint8_t)TlvId_EnergyList;
            req.NumberOfTlvIds++;

        } else {
            printf("Parameter %s not recognized! Please see > thr mgmt gettlvs\n", name);
            return;
        }

        // get another tlv
        free(name);
        token = strtok(NULL, " ");      // tlvname

        if (token) {
            name = strdup(token);
        } else {
            name = NULL;
        }
    }

    req.TlvIds = malloc(req.NumberOfTlvIds);
    memcpy(req.TlvIds, temp, req.NumberOfTlvIds);

    MESHCOP_MgmtGetRequest(&req, framer, OC);

    free(req.TlvIds);
}

/* Mgmt query command parser */
static void mgmt_query(Framer *framer, char *command)
{
    int i;
    uint8_t buf[0x17] = { 0 };
    char *token, *addr, *name = NULL;

    token = strtok(command, " ");   // thr
    token = strtok(NULL, " ");      // mgmt
    token = strtok(NULL, " ");      // query

    token = strtok(NULL, " ");      // addr

    if (token) {
        addr = strdup(token);
    } else {
        printf("Invalid parameters\n");
        return;
    }

    token = strtok(NULL, " ");      // tlvname

    if (token) {
        name = strdup(token);
    } else {
        printf("Invalid parameters\n");
        return;
    }

    uint8_t temp_addr[16];

    if (inet_pton(AF_INET6, addr, temp_addr) != 1) {
        printf("invalid IPv6 string!\n");
        return;
    }

    free(addr);

    // reverse IPv6 bytes
    for (i = 0; i < 16; i++) {
        buf[i + 7] = temp_addr[15 - i];
    }

    while (name) {

        if (strncmp(name, "chmask", sizeof("chmask")) == 0) {
            hex_string_to_byte_array(strtok(NULL, " "), &buf[1]);
            reverse_byte_array(&buf[1], 4);

        } else if (strncmp(name, "panid", sizeof("panid")) == 0) {
            hex_string_to_byte_array(strtok(NULL, " "), &buf[5]);
            reverse_byte_array(&buf[5], 2);

        } else {
            printf("Forbidden parameter %s\n", name);
            return;
        }

        // get another tlv
        free(name);
        token = strtok(NULL, " ");      // tlvname

        if (token) {
            name = strdup(token);
        } else {
            name = NULL;
        }
    }

    /* Send the request */
    FSCIFrame *mgmt_query = CreateFSCIFrame(framer, TX_OG, 0xA8, buf, sizeof(buf), VIF);
    SendFrame(framer, mgmt_query); sleep(1);
    DestroyFSCIFrame(mgmt_query);
}

static void NWKU_CoapSendRequest(NWKU_CoapSendRequest_t *req, Framer *framer)
{
    FSCIFrame *coap_req;
    uint8_t *pMsg = NULL;
    uint16_t msgLen = 0, idx = 0;

    /* Sanity check */
    if (!req) {
        return;
    }

    /* Compute the size of the request */
    msgLen += sizeof(uint8_t);  // InstanceID
    msgLen += sizeof(req->DestinationIpAddress);  // DestinationIpAddress
    msgLen += sizeof(uint16_t);  // UDPPort
    msgLen += sizeof(uint8_t);  // RequestType
    msgLen += sizeof(uint8_t);  // MessageType
    msgLen += 30;  // URIpath
    msgLen += sizeof(uint8_t);  // PayloadLength
    msgLen += req->PayloadLength;  // Payload

    /* Allocate memory for the marshalled payload */
    pMsg = malloc(msgLen);

    if (!pMsg) {
        return;
    }

    /* Serialize */
    pMsg[idx] = req->InstanceID; idx++;
    memcpy(pMsg + idx, req->DestinationIpAddress, sizeof(req->DestinationIpAddress)); idx += sizeof(req->DestinationIpAddress);
    memcpy(pMsg + idx, &(req->UDPPort), sizeof(req->UDPPort)); idx += sizeof(req->UDPPort);
    pMsg[idx] = req->RequestType; idx++;
    pMsg[idx] = req->MessageType; idx++;
    memcpy(pMsg + idx, req->URIpath, 30); idx += 30;
    pMsg[idx] = req->PayloadLength; idx++;
    memcpy(pMsg + idx, req->Payload, req->PayloadLength); idx += req->PayloadLength;

    /* Send the request */

    coap_req = CreateFSCIFrame(framer, TX_OG, COAP_SEND_OC, pMsg, msgLen, VIF);
    SendFrame(framer, coap_req);
    free(req->Payload);
    DestroyFSCIFrame(coap_req);
}

static void coap_create_instance(Framer *framer)
{
    if (!coapInstAlreadyCreated) {
        uint8_t NWKU_CoapCreateInstanceReq[3] = {COAP_DEFAULT_PORT & 0x00FF, COAP_DEFAULT_PORT >> 8, AF_INET6};
        FSCIFrame *coap_create_instance_req = CreateFSCIFrame(framer, TX_OG, COAP_CREATE_INST_OC, NWKU_CoapCreateInstanceReq,
                                              sizeof(NWKU_CoapCreateInstanceReq), VIF);
        SendFrame(framer, coap_create_instance_req); sleep(1);
        DestroyFSCIFrame(coap_create_instance_req);
        coapInstAlreadyCreated = TRUE;
    }
}

static void coap_handler(Framer *framer, char *command)
{
    int i;
    NWKU_CoapSendRequest_t req = { 0 };
    char *token, *payload;

    // req.UDPPort = COAP_DEFAULT_PORT;

    token = strtok(command, " ");   // coap
    token = strtok(NULL, " ");      // request type

    if (token) {
        if (strcmp(token, "CON") == 0) {
            req.RequestType = NWKU_CoapSendRequest_RequestType_CON;
        } else if (strcmp(token, "NON") == 0) {
            req.RequestType = NWKU_CoapSendRequest_RequestType_NON;
        } else {
            printf("Bad request type, please choose between CON|NON.\n");
            return;
        }
    } else {
        printf("Missing request type, please choose between CON|NON.\n");
        return;
    }

    token = strtok(NULL, " ");      // message type

    if (token) {
        if (strcmp(token, "GET") == 0) {
            req.MessageType = NWKU_CoapSendRequest_MessageType_GET;
        } else if (strcmp(token, "POST") == 0) {
            req.MessageType = NWKU_CoapSendRequest_MessageType_POST;
        } else if (strcmp(token, "PUT") == 0) {
            req.MessageType = NWKU_CoapSendRequest_MessageType_PUT;
        } else if (strcmp(token, "DELETE") == 0) {
            req.MessageType = NWKU_CoapSendRequest_MessageType_DELETE;
        } else {
            printf("Bad message type, please choose between GET|POST|PUT|DELETE.\n");
            return;
        }
    } else {
        printf("Missing message type, please choose between GET|POST|PUT|DELETE.\n");
        return;
    }

    token = strtok(NULL, " ");      // address
    uint8_t temp_addr[16];

    if (!token || (inet_pton(AF_INET6, token, temp_addr) != 1)) {
        printf("invalid IPv6 string!\n");
        return;
    }

    // reverse IPv6 bytes
    for (i = 0; i < 16; i++) {
        req.DestinationIpAddress[i] = temp_addr[15 - i];
    }

    token = strtok(NULL, " ");      // URI path

    if (!token || (strlen(token) > sizeof(req.URIpath))) {
        printf("URI path too long, max 30 characters allowed!\n");
        return;
    }

    memcpy(req.URIpath, token, strlen(token));

    token = strtok(NULL, " ");      // payload

    if (token) {
        payload = token;
        req.PayloadLength = strlen(token) - 1;  // -1 to remove ENTER

        if (strncmp(token, "rgb", strlen("rgb")) == 0) {
            token = strtok(NULL, " ");      // red

            if (token) {
                req.PayloadLength += strlen(token) + 1;
            } else {
                printf("Please provide a RED value in decimal in interval [0, 255]\n");
                return;
            }

            token = strtok(NULL, " ");      // green

            if (token) {
                req.PayloadLength += strlen(token) + 1;
            } else {
                printf("Please provide a GREEN value in decimal in interval [0, 255]\n");
                return;
            }

            token = strtok(NULL, " ");      // blue

            if (token) {
                req.PayloadLength += strlen(token) + 1;
            } else {
                printf("Please provide a BLUE value in decimal in interval [0, 255]\n");
                return;
            }
        }

        req.Payload = calloc(req.PayloadLength, sizeof(uint8_t));

        if (!req.Payload) {
            printf("No memory for creating the CoAP request.\n");
            return;
        }

        memcpy(req.Payload, payload, req.PayloadLength);
    }

    coap_create_instance(framer);
    NWKU_CoapSendRequest(&req, framer);
}

static void mws_handler(Framer *framer, char *command, bool_t permit)
{
    if (permit) {
        int seconds, duty_cycle;
        char *token;
        token = strtok(command, " ");   // ptaon
        token = strtok(NULL, " ");      // seconds

        if (token) {
            seconds = atoi(token);
        } else {
            printf("Please provide the number of seconds!");
            return;
        }

        if (seconds == 0) {
            printf("Asserting PTA indefinitely. Use > ptaoff to disable.\n");
        }

        if (seconds > 255) {
            printf("The number of seconds has been reduced to 255, the maximum value allowed..\n");
            seconds = 255;
        }

        token = strtok(NULL, " ");      // duty cycle

        if (token) {
            duty_cycle = atoi(token);

            if (duty_cycle < 0 || duty_cycle > 99) {
                printf("Invalid duty cycle value, must be in [0, 99]");
                return;
            }

        } else {
            printf("Please provide the duty cycle percentage!");
            return;
        }

        uint8_t mws_req_access_buf[2] = {seconds, duty_cycle};
        FSCIFrame *mws_req_access = CreateFSCIFrame(framer, MWS_TX_OG, MWS_REQ_ACCESS_OC,
                                    mws_req_access_buf, sizeof(mws_req_access_buf), VIF);
        SendFrame(framer, mws_req_access); sleep(1);
        DestroyFSCIFrame(mws_req_access);

    } else {
        FSCIFrame *mws_rel_access = CreateFSCIFrame(framer, MWS_TX_OG, MWS_REL_ACCESS_OC, NULL, 0, VIF);
        SendFrame(framer, mws_rel_access); sleep(1);
        DestroyFSCIFrame(mws_rel_access);
    }
}

static void display_help()
{
    printf("Supported commands:\n");

    printf("%s - reset device to factory defaults\n", SHELL_FACTORY_RESET);
    printf("%s - reset device (MCU reset)\n", SHELL_RESET);
    printf("%s - configure the Thread network for harness\n\t\tmust be called before create/join\n", SHELL_HARNESS_CONFIG);
    printf("\n");

    printf("%s - set 802.15.4 channel (11-26)\n", SHELL_THR_SET_CHANNEL);
    printf("%s 0/1- enable/disable stack autostart from NVM\n", SHELL_THR_SET_AUTOSTART);
    printf("%s 0/1 - whether the device is already commissioned\n", SHELL_THR_SET_ISCOMM);
    printf("%s 0x112233445566 (seconds) - set active timestamp\n", SHELL_THR_SET_ACTIVETS);
    printf("%s - get 802.15.4 extended address\n", SHELL_THR_GET_EXT_ADDR);
    printf("%s - create a Thread network\n", SHELL_THR_CREATE);
    printf("%s - join a Thread network\n", SHELL_THR_JOIN);
    printf("\n");

    printf("%s slaac %s\n", SHELL_NWKDATA_ADD, HELP_NWKDATA_ADD);
    printf("%s dhcpserver %s\n", SHELL_NWKDATA_ADD, HELP_NWKDATA_ADD);
    printf("%s extroute %s\n", SHELL_NWKDATA_ADD, HELP_NWKDATA_ADD);
    printf("%s - needed to advertise new data after each nwkdata add\n", SHELL_SYNC_NWKDATA);

    printf("%s - start the commissioner feature\n", SHELL_START_COMM);
    printf("%s - stop the commissioner feature\n", SHELL_STOP_COMM);
    printf("%s <psk> - allow all Joiners with this PSKd\n", SHELL_ADD_JOINERS);
    printf("%s - displays a list of all TLV names available for get/set and their format\n", SHELL_MGMT_GET_TLVS);
    printf("%s %s\n", SHELL_MGMT_COMMSET, HELP_ATV);
    printf("%s %s\n", SHELL_MGMT_COMMGET, HELP_AV);
    printf("%s %s\n", SHELL_MGMT_ACTIVESET, HELP_ATV);
    printf("%s %s\n", SHELL_MGMT_ACTIVEGET, HELP_AV);
    printf("%s %s\n", SHELL_MGMT_PENDINGSET, HELP_ATV);
    printf("%s %s\n", SHELL_MGMT_PENDINGGET, HELP_AV);
    printf("%s <addr> <chmask> <value> <panid> <value>\n", SHELL_MGMT_QUERY);

    printf("\n");

    printf("%s - ifconfig: get all addresses available\n", SHELL_IFCONFIG);
    printf("%s - get addresses available on every remote Thread node\n", SHELL_GET_NODES_IP);
    printf("%s - ping\n", SHELL_PING);
    printf("%s - send CoAP message\n", SHELL_COAP);
    printf("\tcoap <reqtype: CON|NON> <reqcode (GET|POST|PUT|DELETE)> <IP addr dest> <URI path> <payload ASCII>\n");
    printf("\t\tcoap CON GET <IP addr dest> /temp\n");
    printf("\t\tcoap CON POST <IP addr dest> /led on|off|flash|toggle\n");
    printf("\t\tcoap CON POST <IP addr dest> /led rgb r50 g100 b200\n");
    printf("%s <number of seconds - {0|[1-255]}> <percentage duty cycle Wi-Fi>- assert PTA line to stop Wi-Fi traffic intermittently and commission Thread joiners\n", SHELL_PERMIT_JOIN);
    printf("\t\tIf the number of seconds is 0, the PTA line is asserted indefinitely.\n");
    printf("%s - resume the default CoEx algorithm driven by the embedded application\n", SHELL_DENY_JOIN);
    printf("%s - show again the current message\n", SHELL_HELP);
    printf("%s - exit\n", SHELL_EXIT);
    printf("> ");
}


int main(int argc, char **argv)
{
    DeviceType dev_type;
    void *serial_config;

    /* Check number of arguments. */
    if (argc < 3) {
        printf("Usage UART: # %s {/dev/ttyACMx | /dev/ttymxcx} channel [baudrate bps]\n", argv[0]);
        printf("Usage  SPI: # %s /dev/spidev-bus.device channel [speed Hz]\n", argv[0]);
        printf("\t* /dev/spidev-bus.device is the SPI node created by \x1b[32mspidev\x1b[0m.\n");
        printf("\t* channel must be in range [11-26].\n");
        printf("\t* baudrate defaults to \x1b[32m115200 bps\x1b[0m.\n");
        printf("\t* speed in Hz, defaults to \x1b[32m0.96MHz\x1b[0m.\n");
        exit(1);
    }

    uint8_t dest_ip6[16] = { 0 };
    uint8_t channel = atoi(argv[2]);
    assert(channel >= 11 && channel <= 26);
    set_ch_buf[4] = channel;

    /* Identify device as UART/SPI. */
    if (strstr(argv[1], "spi")) {
        dev_type = SPI;
        serial_config = defaultSettingsSPI();
        setSpeedHzSPI(serial_config, 960000);  // match DK07 driver cap
    } else {
        dev_type = UART;
        serial_config = defaultConfigurationData();
    }

    /* If a third argument exists, it is the baudrate/speed. */
    if (argc == 4) {
        if (dev_type == UART) {
            int baudrate = get_baudrate(atoi(argv[3]));

            if (baudrate == -1) {
                printf("Wrong baudrate value.\n");
                exit(1);
            } else {
                setBaudrate(serial_config, baudrate);
            }
        } else {
            setSpeedHzSPI(serial_config, atoi(argv[3]));
        }
    }

    /* Add signal handler for SIGINT, a.k.a Ctrl-C. */
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        printf("Cannot catch SIGINT\n");
    }

    PhysicalDevice *device = InitPhysicalDevice(dev_type, serial_config, argv[1], GLOBAL);
    Framer *framer = InitializeFramer(device, FSCI, LENGTH_FIELD_SIZE, CRC_FIELD_SIZE, _LITTLE_ENDIAN);
    OpenPhysicalDevice(device);
    AttachToFramer(framer, NULL, callback);

    FSCIFrame *tuntap_link          = CreateFSCIFrame(framer, TX_OG, VTUN_LINK_IND_OC,          tuntap_link_buf, sizeof(tuntap_link_buf), VIF);
    FSCIFrame *reset                = CreateFSCIFrame(framer, TX_OG, CPU_RESET_OC,              reset_buf, sizeof(reset_buf), VIF);
    FSCIFrame *factory_rst          = CreateFSCIFrame(framer, TX_OG, FACTORY_RESET_OC,          NULL, 0, VIF);
    FSCIFrame *set_channel          = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC,           set_ch_buf, sizeof(set_ch_buf), VIF);
    FSCIFrame *set_pskd             = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC,           set_pskd_buf, sizeof(set_pskd_buf), VIF);
    FSCIFrame *create_nwk           = CreateFSCIFrame(framer, TX_OG, THR_CREATE_NWK_OC,         create_buf, sizeof(create_buf), VIF);
    FSCIFrame *join_nwk             = CreateFSCIFrame(framer, TX_OG, THR_JOIN_NWK_OC,           join_buf, sizeof(join_buf), VIF);
    FSCIFrame *ifconfig             = CreateFSCIFrame(framer, TX_OG, IFCONFIG_OC,               NULL, 0, VIF);
    FSCIFrame *add_joiner           = CreateFSCIFrame(framer, TX_OG, MESHCOP_ADD_JOINER_OC,     add_joiner_default_buf, sizeof(add_joiner_default_buf), VIF);
    FSCIFrame *sync_steering_data   = CreateFSCIFrame(framer, TX_OG, MESHCOP_SYNC_STEERING_OC,  sync_steering_buf, sizeof(sync_steering_buf), VIF);
    FSCIFrame *get_steering_data    = CreateFSCIFrame(framer, TX_OG, THR_GET_ATTR_OC,           get_steering_buf, sizeof(get_steering_buf), VIF);
    FSCIFrame *get_randomaddr       = CreateFSCIFrame(framer, TX_OG, THR_GET_ATTR_OC,           get_randomaddr_buf, sizeof(get_randomaddr_buf), VIF);
    FSCIFrame *sync_nwkdata         = CreateFSCIFrame(framer, TX_OG, THR_BR_SYNC_OC,            sync_nwkdata_buf, sizeof(sync_nwkdata_buf), VIF);
    FSCIFrame *start_comm           = CreateFSCIFrame(framer, TX_OG, MESHCOP_START_COMM,        inst_id_buf, sizeof(inst_id_buf), VIF);
    FSCIFrame *stop_comm            = CreateFSCIFrame(framer, TX_OG, MESHCOP_STOP_COMM,         inst_id_buf, sizeof(inst_id_buf), VIF);
    FSCIFrame *getnodesip           = CreateFSCIFrame(framer, TX_OG, THR_MGMT_DIAG_GET_OC,      getnodesip_buf, sizeof(getnodesip_buf), VIF);
    FSCIFrame *ping;

    /* start TAP thread */
    pthread_t tap_thread;
    int rc = pthread_create(&tap_thread, NULL, &tap_start, framer);

    if (rc != 0) {
        printf("TAP thread could not be created");
        return 1;
    }

    /* announce that TUN/TAP link is ready */
    printf("%s: link up\n", TAP_NAME);
    sleep(1);
    SendFrame(framer, tuntap_link);

#if FEATURE_NETLINK_DETECT
    /* start netlink thread to detect network changes on the uplink interface */
    pthread_t netlink_thread;
    rc = pthread_create(&netlink_thread, NULL, &netlink_start, framer);

    if (rc != 0) {
        printf("NETLINK thread could not be created");
        return 1;
    }
#endif

    display_help();

    /* cmd interface */
    char *command = malloc(SHELL_MAX_CMD_LEN * sizeof(char));
    size_t len = SHELL_MAX_CMD_LEN;
    ssize_t nread;
    int i;

    /* disable stdout buffering to be able to print live prompt (>) */
    setbuf(stdout, NULL);

    while ((nread = getline(&command, &len, stdin)) != -1) {

        if (strncmp(command, SHELL_FACTORY_RESET, strlen(SHELL_FACTORY_RESET)) == 0) {
            printf("TX: THR_FactoryReset.Request\n");
            SendFrame(framer, factory_rst); sleep(4);
        }

        else if (strncmp(command, SHELL_SYNC_NWKDATA, strlen(SHELL_SYNC_NWKDATA)) == 0) {
            printf("TX: THR_BrPrefixSync.Request\n");
            SendFrame(framer, sync_nwkdata); sleep(1);
        }

        else if (strncmp(command, SHELL_NWKDATA_ADD, strlen(SHELL_NWKDATA_ADD)) == 0) {
            printf("TX: THR_BrPrefixAddEntry.Request -> ");
            add_br_entry(framer, command);
        }

        else if (strncmp(command, SHELL_START_COMM, strlen(SHELL_START_COMM)) == 0) {
            printf("TX: MESHCOP_StartCommissioner.Request -> ");
            SendFrame(framer, start_comm); sleep(1);
        }

        else if (strncmp(command, SHELL_STOP_COMM, strlen(SHELL_STOP_COMM)) == 0) {
            printf("TX: MESHCOP_StopCommissioner.Request -> ");
            SendFrame(framer, stop_comm); sleep(1);
        }

        else if (strncmp(command, SHELL_ADD_JOINERS, strlen(SHELL_ADD_JOINERS)) == 0) {
            if (add_joiners(framer, command)) {
                printf("TX: MESHCOP_SyncSteeringData.Request\n");
                SendFrame(framer, sync_steering_data); sleep(1);
            }
        }

        else if (strncmp(command, SHELL_MGMT_GET_TLVS, strlen(SHELL_MGMT_GET_TLVS)) == 0) {
            mgmt_get_tlvs(framer);
        }

        else if (strncmp(command, SHELL_MGMT_ACTIVESET, strlen(SHELL_MGMT_ACTIVESET)) == 0) {
            printf("TX: MESHCOP_MgmtSet.Request -> ");
            mgmt_set(framer, command, MESHCOP_ACTIVE_SET_OC);
        }

        else if (strncmp(command, SHELL_MGMT_ACTIVEGET, strlen(SHELL_MGMT_ACTIVEGET)) == 0) {
            printf("TX: MESHCOP_MgmtActiveGet.Request -> ");
            mgmt_get(framer, command, MESHCOP_ACTIVE_GET_OC);
        }

        else if (strncmp(command, SHELL_MGMT_COMMSET, strlen(SHELL_MGMT_COMMSET)) == 0) {
            printf("TX: MESHCOP_MgmtCommissionerSet.Request -> ");
            mgmt_set(framer, command, MESHCOP_COMM_SET_OC);
        }

        else if (strncmp(command, SHELL_MGMT_COMMGET, strlen(SHELL_MGMT_COMMGET)) == 0) {
            printf("TX: MESHCOP_MgmtCommissionerGet.Request -> ");
            mgmt_get(framer, command, MESHCOP_COMM_GET_OC);
        }

        else if (strncmp(command, SHELL_MGMT_PENDINGSET, strlen(SHELL_MGMT_PENDINGSET)) == 0) {
            printf("TX: MESHCOP_MgmtPendingSet.Request -> ");
            mgmt_set(framer, command, MESHCOP_PENDING_SET_OC);
        }

        else if (strncmp(command, SHELL_MGMT_PENDINGGET, strlen(SHELL_MGMT_PENDINGGET)) == 0) {
            printf("TX: MESHCOP_MgmtPendingGet.Request -> ");
            mgmt_get(framer, command, MESHCOP_PENDING_GET_OC);
        }

        else if (strncmp(command, SHELL_MGMT_QUERY, strlen(SHELL_MGMT_QUERY)) == 0) {
            printf("TX: MESHCOP_MgmtSendPanIdQuery.Request -> ");
            mgmt_query(framer, command);
        }

        else if (strncmp(command, SHELL_RESET, strlen(SHELL_RESET)) == 0) {
            printf("TX: THR_McuReset.Request\n");
            SendFrame(framer, reset); sleep(1);
        }

        else if (strncmp(command, SHELL_THR_SET_CHANNEL, strlen(SHELL_THR_SET_CHANNEL)) == 0) {
            printf("TX: THR_SetAttr.Request Channel %d\n", channel);
            SendFrame(framer, set_channel); sleep(1);
        }

        else if (strncmp(command, SHELL_THR_SET_ISCOMM, strlen(SHELL_THR_SET_ISCOMM)) == 0) {
            printf("TX: THR_SetAttr.Request IsDevCommissioned\n");
            uint8_t set_iscomm_buf[] = {THR_INST_ID, 0x13, 0x00, 0x01, FALSE};

            if (command[strlen(command) - 2] == '1') {
                set_iscomm_buf[4] = TRUE;
            }

            FSCIFrame *set_iscomm = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC, set_iscomm_buf, sizeof(set_iscomm_buf), VIF);
            SendFrame(framer, set_iscomm); sleep(1);
            DestroyFSCIFrame(set_iscomm);
        }

        else if (strncmp(command, SHELL_THR_SET_AUTOSTART, strlen(SHELL_THR_SET_AUTOSTART)) == 0) {
            printf("TX: THR_SetAttr.Request NVM_RestoreAutoStart\n");
            uint8_t set_autostart_buf[] = {THR_INST_ID, 0x36, 0x00, 0x01, FALSE};

            if (command[strlen(command) - 2] == '1') {
                set_autostart_buf[4] = TRUE;
            }

            FSCIFrame *set_autostart = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC, set_autostart_buf, sizeof(set_autostart_buf), VIF);
            SendFrame(framer, set_autostart); sleep(1);
            DestroyFSCIFrame(set_autostart);
        }

        else if (strncmp(command, SHELL_THR_SET_ACTIVETS, strlen(SHELL_THR_SET_ACTIVETS)) == 0) {
            printf("TX: THR_SetAttr.Request ActiveTimestamp\n");

            char *token, *seconds;
            token = strtok(command, " ");   // thr
            token = strtok(NULL, " ");      // set
            token = strtok(NULL, " ");      // activets

            token = strtok(NULL, " ");      // seconds

            if (token) {
                seconds = strdup(token);
            } else {
                printf("Invalid parameters\n");
                return 1;
            }

            hex_string_to_byte_array(seconds, set_activets_buf + 4);

            FSCIFrame *set_activets = CreateFSCIFrame(framer, TX_OG, THR_SET_ATTR_OC, set_activets_buf, sizeof(set_activets_buf), VIF);
            SendFrame(framer, set_activets); sleep(1);
            free(seconds);
            DestroyFSCIFrame(set_activets);
        }

        else if (strncmp(command, SHELL_THR_CREATE, strlen(SHELL_THR_CREATE)) == 0) {
            printf("TX: THR_SetAttr.Request Channel %d\n", channel);
            SendFrame(framer, set_channel); sleep(1);

            printf("TX: THR_SetAttr.Request PSKd `THREAD`\n");
            SendFrame(framer, set_pskd); sleep(1);

            printf("TX: THR_CreateNwk.Request\n");
            SendFrame(framer, create_nwk); sleep(12);

            printf("TX: MESHCOP_AddExpectedJoiner.Request\n");
            SendFrame(framer, add_joiner); sleep(1);

            printf("TX: MESHCOP_SyncSteeringData.Request\n");
            SendFrame(framer, sync_steering_data); sleep(1);

            printf("TX: THR_GetAttr.Request Steering Data\n");
            SendFrame(framer, get_steering_data); sleep(1);
        }

        else if (strncmp(command, SHELL_THR_JOIN, strlen(SHELL_THR_JOIN)) == 0) {
            printf("TX: THR_SetAttr.Request Channel %d\n", channel);
            SendFrame(framer, set_channel); sleep(1);

            printf("TX: THR_SetAttr.Request PSKd `THREAD`\n");
            SendFrame(framer, set_pskd); sleep(1);

            printf("TX: THR_Join.Request\n");
            SendFrame(framer, join_nwk); sleep(1);
        }

        else if (strncmp(command, SHELL_HARNESS_CONFIG, strlen(SHELL_HARNESS_CONFIG)) == 0) {
            configure_for_harness(framer);
        }

        else if (strncmp(command, SHELL_IFCONFIG, strlen(SHELL_IFCONFIG)) == 0) {
            printf("TX: NWKU_IfconfigAll.Request\n");
            SendFrame(framer, ifconfig); sleep(1);
        }

        else if (strncmp(command, SHELL_GET_NODES_IP, strlen(SHELL_GET_NODES_IP)) == 0) {
            printf("TX: THR_MgmtDiagnosticGet.Request\n");
            SendFrame(framer, getnodesip); sleep(1);
        }

        else if (strncmp(command, SHELL_PING, strlen(SHELL_PING)) == 0) {
            printf("Simple interactive ping, stop with Ctrl-C.\n");
            printf("\tPlease enter destination IPv6 address: ");

            if (fgets(destination, sizeof(destination), stdin) == NULL) {
                printf("Invalid IPv6 address\n");
                continue;
            }

            destination[strlen(destination) - 1] = 0;

            if (inet_pton(AF_INET6, destination, dest_ip6) == 1) {
                for (i = 0; i < 16; i++) {
                    ping_buf[i] = dest_ip6[15 - i];
                }

                printf("\tPlease enter payload size: ");

                if (scanf("%hu", &packetsize) <= 0) {
                    printf("Invalid packet size\n");
                    continue;
                }

                ping_buf[32] = packetsize & 0xFF; // payload length
                ping_buf[33] = packetsize >> 8;

                printf("\tPlease enter interval seconds between sending each packet: ");

                if (scanf("%f", &interval) <= 0) {
                    printf("Invalid interval\n");
                    continue;
                }

                ping_buf[34] = 0xD0; // timeout
                ping_buf[35] = 0x07;
                ping_buf[36] = 0x01; // secured

                ping = CreateFSCIFrame(framer, TX_OG, NWKU_PING_OC, ping_buf, sizeof(ping_buf), VIF);
                printf("Pinging %s with %d bytes of data:\n", destination, packetsize);

                stopPing = FALSE;

                while (!stopPing) {
                    SendFrame(framer, ping);
                    usleep(interval * 1000 * 1000);
                }

                stopPing = TRUE;

            } else {
                printf("Invalid IPv6 address\n");
            }
        }

        else if (strncmp(command, SHELL_COAP, strlen(SHELL_COAP)) == 0) {
            coap_handler(framer, command);
        }

        else if (strncmp(command, SHELL_PERMIT_JOIN, strlen(SHELL_PERMIT_JOIN)) == 0) {
            mws_handler(framer, command, TRUE);
        }

        else if (strncmp(command, SHELL_DENY_JOIN, strlen(SHELL_DENY_JOIN)) == 0) {
            mws_handler(framer, command, FALSE);
        }

        else if (strncmp(command, SHELL_THR_GET_EXT_ADDR, strlen(SHELL_THR_GET_EXT_ADDR)) == 0) {
            printf("TX: THR_GetAttr.Request RandomExtendedAddr\n");
            SendFrame(framer, get_randomaddr); sleep(1);
        }

        else if (strncmp(command, SHELL_HELP, strlen(SHELL_HELP)) == 0) {
            display_help();
        }

        else if (strncmp(command, SHELL_EXIT, strlen(SHELL_EXIT)) == 0) {
            free(command);
            return 0;
        }

        else if (nread > 1) {
            printf("Unrecognized command\n");
        }

        printf("> ");
    }

    return 0;
}
