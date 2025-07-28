/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2022 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#if defined(SERIAL_BTSNOOP)

#include <stdint.h>
#include <stdbool.h>

#include "sbtsnoop.h"
#include "sbtsnoop_port.h"
#include "fsl_os_abstraction.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

#define PACK_BE_4_BYTE_VAL(dst, src)                    \
    *((uint8_t *)(dst) + 3U) = (uint8_t)(src);          \
    *((uint8_t *)(dst) + 2U) = (uint8_t)((src) >> 8U);  \
    *((uint8_t *)(dst) + 1U) = (uint8_t)((src) >> 16U); \
    *((uint8_t *)(dst) + 0U) = (uint8_t)((src) >> 24U);

#define HCI_COMMAND_PACKET  0x01U
#define HCI_ACL_DATA_PACKET 0x02U
#define HCI_SCO_DATA_PACKET 0x03U
#define HCI_EVENT_PACKET    0x04U

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static void sbtsnoop_freset(void);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */
#if !defined(SBTSNOOP_MODE_RAW)
static char const serial_btsnoop_menu[] =
    "\r\n"
    "--------------------------------------------\r\n"
    "         SERIAL BTSNOOP MENU\r\n"
    "--------------------------------------------\r\n"
    "  0 - Exit \r\n"
    "  1 - Refresh \r\n"
    "  2 - Open Serial BTSNOOP File\r\n"
    "  3 - Close Serial BTSNOOP File\r\n"
    "  4 - Dump current BTSNOOP File\r\n";

static const uint8_t btsnoop_hdr[16] = {
    0x62U, 0x74U, 0x73,  0x6eU, 0x6fU, 0x6fU, 0x70U, 0x00, // identification pattern "btsnoop null terminated string"
    0x00U, 0x00U, 0x00U, 0x01U,                            // version number = 1
    0x00U, 0x00U, 0x03U, 0xEAU                             // datalink type 1002
};

#endif

#ifdef BTSNOOP_FILTER
static struct
{
    uint8_t pkt_type;
    uint8_t byte0;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t truncated_lenght;
} const serial_btsnoop_filter[] = BTSNOOP_FILTER;
#endif

static uint8_t  btsnoop_log[SBTSNOOP_LOG_SIZE];
static uint32_t btsnoop_log_index = 0U;
static bool     btsnoop_enable    = false;

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void sbtsnoop_fopen(void)
{
    if (btsnoop_enable)
    {
        sbtsnoop_fclose();
    }
    btsnoop_enable = true;
#if !defined(SBTSNOOP_MODE_RAW)
    sbtsnoop_fwrite((const void *)btsnoop_hdr, 16U);
#endif
    sbtsnoop_log_printf("[SBTSNOOP] Serial BTSNOOP file opened\r\n");
}

void sbtsnoop_fclose(void)
{
    btsnoop_enable = false;
    sbtsnoop_fread();
    sbtsnoop_log_printf("[SBTSNOOP] Serial BTSNOOP file closed\r\n");
}

void sbtsnoop_fwrite(const void *pkt, uint16_t size)
{
    if (btsnoop_enable)
    {
#if !defined(SBTSNOOP_MODE_RAW)
        if ((btsnoop_log_index + size) > SBTSNOOP_LOG_SIZE)
        {
            /* log array is full, we need to dump current content and continue logging
             * to avoid information loss */
            sbtsnoop_fread();
        }
#endif
        (void)memcpy(&btsnoop_log[btsnoop_log_index], (const uint8_t *)pkt, size);
        btsnoop_log_index += size;
#if defined(SBTSNOOP_MODE_RAW)
        sbtsnoop_fread();
#endif
    }
}

void sbtsnoop_fread(void)
{
    if (btsnoop_log_index != 0U)
    {
        sbtsnoop_log_printf("********** SERIAL BTSNOOP DUMP **********\r\n");

        uint32_t lines = (btsnoop_log_index >> SBTSNOOP_BYTES_PER_LINE_SHIFT) + 1u;

        for (uint32_t i = 0u; i < lines; i++)
        {
            for (uint32_t j = 0u; j < SBTSNOOP_BYTES_PER_LINE; j++)
            {
                uint32_t index = (i << SBTSNOOP_BYTES_PER_LINE_SHIFT) + j;
                if (index >= btsnoop_log_index)
                {
                    break;
                }
                else
                {
#if !defined(SBTSNOOP_MODE_RAW)
                    sbtsnoop_log_printf("%02x ", btsnoop_log[index]);
#else
                    sbtsnoop_log_data("%c", btsnoop_log[index]);
#endif
                }
            }
            sbtsnoop_log_printf("\r\n");
        }
        sbtsnoop_log_printf("*****************************************\r\n");
        /* each dump resets current write index in the file
         * so each dump can be assembled to get complete btsnoop file */
        sbtsnoop_freset();
    }
}

void sbtsnoop_write_hci_pkt(uint8_t pkt_type, uint8_t is_rxed, uint8_t *pkt, uint16_t pkt_len)
{
    bool drop_pkt = false;

    if (btsnoop_enable)
    {
        uint32_t irqMask;

#ifdef BTSNOOP_FILTER
        /* This feature allows to truncate packet payload to save some UART bandwidth */
        uint32_t filter_sz = sizeof(serial_btsnoop_filter) / sizeof serial_btsnoop_filter[0];
        for (uint32_t i = 0; i < filter_sz; i++)
        {
            if (pkt_type != serial_btsnoop_filter[i].pkt_type)
                continue;

            /* Event, 0=Event Code, 1=param total lenght, 2=subevent code */
            if ((pkt_type == HCI_EVENT_PACKET) && (serial_btsnoop_filter[i].byte0 == pkt[0]) && /* Event code */
                (serial_btsnoop_filter[i].byte2 == pkt[2]))                                     /* subevent code */
            {
                if (serial_btsnoop_filter[i].truncated_lenght == 0)
                {
                    drop_pkt = true;
                }
                else
                {
                    pkt[1]  = serial_btsnoop_filter[i].truncated_lenght;
                    pkt_len = pkt[1] + 2;
                }
                break;
            }
            else
                /* Command, 0=OCF, 1=OGF, 2=param total lenght */
                if ((pkt_type == HCI_COMMAND_PACKET) &&
                    (serial_btsnoop_filter[i].byte0 == pkt[0]) && /* command code OCF */
                    (serial_btsnoop_filter[i].byte1 == pkt[1]))   /* command code OGF */
                {
                    if (serial_btsnoop_filter[i].truncated_lenght == 0)
                    {
                        drop_pkt = true;
                    }
                    else
                    {
                        pkt[2]  = serial_btsnoop_filter[i].truncated_lenght;
                        pkt_len = pkt[2] + 3;
                    }
                }
                else
                    /* Command, Event Code (), command code OCF, OGF */
                    if ((pkt_type == HCI_ACL_DATA_PACKET) || (pkt_type == HCI_SCO_DATA_PACKET))
                    {
                        drop_pkt = true;
                    }
        }
#endif

        irqMask = DisableGlobalIRQ();

#if !defined(SBTSNOOP_MODE_RAW)
        {
            uint8_t  buffer[4U];
            uint32_t four_octet_value;
            uint64_t timestamp;

            /* BTSNOOP Packet Record Format */
            /* Original Length: 4 Octets */
            /* Include the length of the packet type */
            four_octet_value = (uint32_t)(pkt_len) + 1U;
            PACK_BE_4_BYTE_VAL(buffer, four_octet_value);
            sbtsnoop_fwrite(buffer, 4U);

            /* Included Length: 4 Octets */
            sbtsnoop_fwrite(buffer, 4U);

            /* Packet Flags: 4 Octets */
            switch (pkt_type)
            {
                case HCI_COMMAND_PACKET: /* Command */
                    four_octet_value = 2U;
                    break;

                case HCI_EVENT_PACKET: /* Event */
                    four_octet_value = 3U;
                    break;

                case HCI_ACL_DATA_PACKET: /* ACL Data */ /* Fallthrough */
                case HCI_SCO_DATA_PACKET:                /* SCO Data */
                    four_octet_value = is_rxed;
                    break;

                default:
                    sbtsnoop_log_printf("[SBTSNOOP] Invalid Packet Type\r\n");
                    break;
            }

            PACK_BE_4_BYTE_VAL(buffer, four_octet_value);
            sbtsnoop_fwrite(buffer, 4U);

            /* Cumulative Drops: 4 Octets */
            four_octet_value = 0U;
            PACK_BE_4_BYTE_VAL(buffer, four_octet_value);
            sbtsnoop_fwrite(buffer, 4U);

            /* Timestamp Microseconds: 8 Octets */
            timestamp = (uint64_t)sbtsnoop_get_us_timestamp();

            /* Time HI */
            four_octet_value = (UINT32)(timestamp >> 32U);
            PACK_BE_4_BYTE_VAL(buffer, four_octet_value);
            sbtsnoop_fwrite(buffer, 4U);

            /* Time LO */
            four_octet_value = (UINT32)(timestamp & 0xFFFFFFFFU);
            PACK_BE_4_BYTE_VAL(buffer, four_octet_value);
            sbtsnoop_fwrite(buffer, 4U);
        }
#endif

        if (!drop_pkt)
        {
            /* Packet Data: n Octets */
            /* Type: 1 Octet */
            sbtsnoop_fwrite(&pkt_type, 1U);

            /* Packet */
            sbtsnoop_fwrite(pkt, pkt_len);
        }

        EnableGlobalIRQ(irqMask);
    }
}

#if !defined(SBTSNOOP_MODE_RAW)
void main_serial_btsnoop_operations(void)
{
    unsigned int choice;

    while (true)
    {
        sbtsnoop_log_printf("%s \r\n", serial_btsnoop_menu);
        sbtsnoop_log_printf("Enter your choice : ");
        sbtsnoop_log_scanf("%d", &choice);

        switch (choice)
        {
            case 0:
                break;

            case 1:
                break;

            case 2:
                sbtsnoop_fopen();
                break;

            case 3:
                sbtsnoop_fclose();
                break;

            case 4:
                sbtsnoop_fread();
                break;

            default:
                sbtsnoop_log_printf("Invalid Choice\r\n");
                break;
        }
    }
}
#endif

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Resets write index, will discard any file content
 *
 */
static void sbtsnoop_freset(void)
{
    btsnoop_log_index = 0;
}

#endif /* SERIAL_BTSNOOP */