/** @file
 * @brief Bluetooth shell module
 *
 * Provides ble scan filter functions.
 */

/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <porting.h>
#include "fsl_shell.h"
#include "shell_ble_scan_filter.h"

#if (defined(CONFIG_BLE_ADV_REPORT_BUFFER_FILTER) && (CONFIG_BLE_ADV_REPORT_BUFFER_FILTER > 0U))
/* Global to configure the Peer ADV List size */
#define SHELL_BLE_SCAN_PEER_ADV_MAX_REPORT_COUNT 100U
/* BLE Adv Report */
struct shell_ble_scan_peer_adv_report{
        /* BLE ADV Report Event Type */
        uint8_t       event_type;
        /* Device Address and Type */
        bt_addr_le_t  dev_addr;
};
/* BLE Adv list pool*/
static struct shell_ble_scan_peer_adv_report shell_le_peer_adv_report[SHELL_BLE_SCAN_PEER_ADV_MAX_REPORT_COUNT];
/* BLE Adv list full flag*/
static uint8_t shell_le_peer_adv_report_list_full;
/* ADV List DS access macros */
#define SHELL_BLE_GET_PEER_ADV_ADDR(i) shell_le_peer_adv_report[(i)].dev_addr
#define SHELL_BLE_GET_PEER_ADV_TYPE(i) shell_le_peer_adv_report[(i)].event_type
/* ADV List DS compare macros */
#define SHELL_BLE_COMPARE_BD_ADDR_AND_TYPE(addr_a,addr_b)\
        ((SHELL_BLE_COMPARE_TYPE((addr_a)->type,(addr_b)->type)) &&\
         (SHELL_BLE_COMPARE_ADDR((addr_a)->a.val,(addr_b)->a.val)))
#define SHELL_BLE_COMPARE_TYPE(type_a,type_b)\
        (((type_a) == (type_b))?true:false)
#define SHELL_BLE_COMPARE_ADDR(addr_a,addr_b)\
        ((0 == memcmp((addr_a), (addr_b), 6))?true:false)
#define SHELL_BLE_BD_ADDR_IS_NON_ZERO(addr)\
        ((0x00U == ((addr)[0U] | (addr)[1U] | (addr)[2U] | (addr)[3U] | (addr)[4U] | (addr)[5U]))?\
        false:true)


static int shell_le_is_dev_in_adv_list(bt_addr_le_t *dev_addr, uint8_t event_type)
{
    uint32_t  index = 0U;
    int       retval;

    retval = -EINVAL;

    /* Validate the device if already present in the Adv list */
    for (index = 0U; index < SHELL_BLE_SCAN_PEER_ADV_MAX_REPORT_COUNT; index++)
    {
        if((true ==
           SHELL_BLE_COMPARE_BD_ADDR_AND_TYPE
           (
                dev_addr,
                &SHELL_BLE_GET_PEER_ADV_ADDR(index)
           )) &&
           (true ==
           SHELL_BLE_COMPARE_TYPE
           (
                event_type,
                SHELL_BLE_GET_PEER_ADV_TYPE(index)
           )))
        {
             retval = 0;
             break;
        }
    }
    return retval;
}

static int shell_le_find_free_adv_list_inst(uint32_t *free_index)
{
    uint32_t  index = 0U;
    int       retval;
    bt_addr_le_t * t_addr;

    retval = -EINVAL;

    if (true != shell_le_peer_adv_report_list_full)
    {
        for (index = 0U; index < SHELL_BLE_SCAN_PEER_ADV_MAX_REPORT_COUNT; index++)
        {
            /* Fetch the Address */
            t_addr = &SHELL_BLE_GET_PEER_ADV_ADDR(index);

            /* Check if the BD Address is Non Zero */
            if (true != SHELL_BLE_BD_ADDR_IS_NON_ZERO(t_addr->a.val))
            {
                retval = 0;
                break;
            }
        }

        /* Store the index */
        *(free_index) = index;

        if (0 != retval)
        {
            if (SHELL_BLE_SCAN_PEER_ADV_MAX_REPORT_COUNT == *(free_index))
            {
                /* Device list is fully occupied, needs a reset for the list */
                PRINTF("ADV Report Device list is full!\n");
                PRINTF("Consider Disabling Scanning!\n");

                shell_le_peer_adv_report_list_full = true;
            }
        }
    }
    return  retval;
}

int shell_le_init_adv_list(void)
{
    /* Initialize the fields with default values */
    memset
    (
        shell_le_peer_adv_report,
        0x0,
        sizeof(shell_le_peer_adv_report)
    );
    shell_le_peer_adv_report_list_full = false;
    return 0;
}

int shell_le_scan_filter_handler(const struct bt_le_scan_recv_info *info)
{
    uint32_t index = 0;
     /* Validate if this device already exists in the filtering list */
    if (0 != shell_le_is_dev_in_adv_list((bt_addr_le_t *)info->addr, info->adv_type))
    {
        /* Device dosen't present in the filtering list, add the device to the filtering list */
        if (0 == shell_le_find_free_adv_list_inst(&index))
        {
            /* Save the Event type */
            SHELL_BLE_GET_PEER_ADV_TYPE(index) = info->adv_type;
            /* Save the Device address and Type */
            SHELL_BLE_GET_PEER_ADV_ADDR(index).type = (info->addr)->type;
            memcpy(&(SHELL_BLE_GET_PEER_ADV_ADDR(index).a.val), (info->addr)->a.val, 6);
        }
        else
        {
            /* Adv device list full */
            return -EINVAL;
        }
    }
    else
    {
        /* Duplicate adv device */
        return -EINVAL;
    }
    return 0;
}
#endif /* CONFIG_BLE_ADV_REPORT_BUFFER_FILTER */