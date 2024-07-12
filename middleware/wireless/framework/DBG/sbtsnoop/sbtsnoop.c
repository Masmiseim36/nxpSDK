/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2022 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

#include "sbtsnoop.h"
#include "sbtsnoop_port.h"
#include "fsl_os_abstraction.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

#define PACK_BE_4_BYTE_VAL(dst, src) \
    *((uint8_t *)(dst) + 3U) = (uint8_t)(src);\
    *((uint8_t *)(dst) + 2U) = (uint8_t)((src) >> 8U);\
    *((uint8_t *)(dst) + 1U) = (uint8_t)((src) >> 16U);\
    *((uint8_t *)(dst) + 0U) = (uint8_t)((src) >> 24U);

#define HCI_COMMAND_PACKET      0x01U
#define HCI_ACL_DATA_PACKET     0x02U
#define HCI_SCO_DATA_PACKET     0x03U
#define HCI_EVENT_PACKET        0x04U

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static void sbtsnoop_freset(void);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */
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

static uint8_t  btsnoop_log[SBTSNOOP_LOG_SIZE];
static uint32_t btsnoop_log_index = 0U;
static bool     btsnoop_enable = false;

static const uint8_t btsnoop_hdr[16] = {
    0x62U,0x74U,0x73,0x6eU,0x6fU,0x6fU,0x70U, 0x00,   // identification pattern "btsnoop null terminated string"
    0x00U,0x00U,0x00U,0x01U,            // version number = 1
    0x00U,0x00U,0x03U,0xEAU             // datalink type 1002
};


/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void sbtsnoop_fopen(void)
{
    if(btsnoop_enable)
    {
        sbtsnoop_fclose();
    }
    btsnoop_enable = true;
    sbtsnoop_fwrite((const void*)btsnoop_hdr, 16U);
    sbtsnoop_log_printf("[SBTSNOOP] Serial BTSNOOP file opened\r\n");
}

void sbtsnoop_fclose(void)
{
    btsnoop_enable = false;
    sbtsnoop_fread();
    sbtsnoop_log_printf("[SBTSNOOP] Serial BTSNOOP file closed\r\n");
}

void sbtsnoop_fwrite(const void* pkt, uint16_t size)
{
    if(btsnoop_enable)
    {
        if( (btsnoop_log_index + size) > SBTSNOOP_LOG_SIZE )
        {
            /* log array is full, we need to dump current content and continue logging
             * to avoid information loss */
            sbtsnoop_fread();
        }
        (void)memcpy(&btsnoop_log[btsnoop_log_index], (const uint8_t*)pkt, size);
        btsnoop_log_index += size;
    }
}

void sbtsnoop_fread(void)
{
    if(btsnoop_log_index != 0U)
    {
        sbtsnoop_log_printf("********** SERIAL BTSNOOP DUMP **********\r\n");

        uint32_t lines = (btsnoop_log_index >> SBTSNOOP_BYTES_PER_LINE_SHIFT) + 1u;

        for(uint32_t i = 0u; i < lines; i++)
        {
            for(uint32_t j = 0u; j < SBTSNOOP_BYTES_PER_LINE; j++)
            {
                uint32_t index = (i << SBTSNOOP_BYTES_PER_LINE_SHIFT) + j;
                if(index >= btsnoop_log_index)
                {
                    break;
                }
                else
                {
                    sbtsnoop_log_printf("%02x ", btsnoop_log[index]);
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

void sbtsnoop_write_hci_pkt(uint8_t pkt_type, uint8_t is_rxed, uint8_t* pkt, uint16_t pkt_len)
{
    uint64_t timestamp;
    uint32_t four_octet_value;
    uint8_t buffer[4U];

    if(btsnoop_enable)
    {
        uint32_t irqMask = DisableGlobalIRQ();

        /* Packet Record Format */
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
            case HCI_SCO_DATA_PACKET: /* SCO Data */
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

        /* Packet Data: n Octets */
        /* Type: 1 Octet */
        sbtsnoop_fwrite(&pkt_type, 1U);

        /* Packet */
        sbtsnoop_fwrite(pkt, pkt_len);

        EnableGlobalIRQ(irqMask);
    }
}

void main_serial_btsnoop_operations(void)
{
    unsigned int  choice;

    while(true)
    {
        sbtsnoop_log_printf("%s \r\n", serial_btsnoop_menu);
        sbtsnoop_log_printf("Enter your choice : ");
        sbtsnoop_log_scanf("%d", &choice);

        switch(choice)
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
