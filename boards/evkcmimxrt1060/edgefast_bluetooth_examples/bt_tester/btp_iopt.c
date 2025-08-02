/* btp_iopt.c - Bluetooth IOPT Tester */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if defined(CONFIG_BT_IOPT) && (CONFIG_BT_IOPT > 0)

#include "fsl_debug_console.h"

#include <porting.h>
#include <errno.h>
#include <sys/util.h>
#include "BT_hci_api.h"
#include "db_gen.h"
#include "btp.h"

static uint32_t iopt_class_of_device;

static void iopt_determine_class_of_device(void)
{
    iopt_class_of_device = 0;
#if defined(CONFIG_BT_A2DP_SINK) && (CONFIG_BT_A2DP_SINK > 0)
    WRITE_BIT(iopt_class_of_device, 18, 1);
#endif

#if defined(CONFIG_BT_A2DP_SOURCE) && (CONFIG_BT_A2DP_SOURCE > 0)
    WRITE_BIT(iopt_class_of_device, 19, 1);
#endif

#if defined(CONFIG_BT_AVRCP_CT) && (CONFIG_BT_AVRCP_CT > 0)
    WRITE_BIT(iopt_class_of_device, 8, 1);
    WRITE_BIT(iopt_class_of_device, 10, 1);
    WRITE_BIT(iopt_class_of_device, 2, 1);
    WRITE_BIT(iopt_class_of_device, 3, 1);
#endif

#if defined(CONFIG_BT_HFP_HF) && (CONFIG_BT_HFP_HF > 0)
    WRITE_BIT(iopt_class_of_device, 21, 1);
#endif

    iopt_class_of_device &= 0x0FFFFFF; // only bit0 ~ bit23 is valid
}

uint8_t tester_init_iopt(void)
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;
    int err = 0;

    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        iopt_determine_class_of_device();
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(iopt_class_of_device, &cp->class_of_device[0]);
        err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        return BTP_STATUS_FAILED;
    }
    if (err < 0)
    {
        return BTP_STATUS_FAILED;
    }

    return BTP_STATUS_SUCCESS;
}

uint8_t tester_unregister_iopt(void)
{
    return BTP_STATUS_SUCCESS;
}

#endif /* CONFIG_BT_IOPT */