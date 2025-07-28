/** @file
 * @brief Bluetooth A2DP shell module
 *
 * Provide some Bluetooth shell commands that can be useful to applications.
 */

/*
 * Copyright (c) 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno/errno.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/byteorder.h>
#include <porting.h>
#include <errno.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "fsl_shell.h"
#include "shell_bt.h"
#include <bluetooth/hfp_hf.h>
#include <bluetooth/sdp.h>

#if (defined(CONFIG_BT_HFP_HF) && (CONFIG_BT_HFP_HF > 0U))

static struct bt_sdp_attribute hfp_hf_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_HANDSFREE_SVCLASS) //11 1E
        },
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_GENERIC_AUDIO_SVCLASS) //12 03
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 12),//35 10
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),// 35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) // 01 00
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),// 35 05
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM) // 00 19
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
                BT_SDP_ARRAY_16(BT_RFCOMM_CHAN_HFP_HF) //channel number
            },
            )
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_HANDSFREE_SVCLASS) //11 1E
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0108U) //01 08
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("Handsfree"),
    BT_SDP_SUPPORTED_FEATURES(0x3400),
};
static struct bt_sdp_record hfp_hf_rec = BT_SDP_RECORD(hfp_hf_attrs);

static void connected(struct bt_conn *conn, int err)
{
    printf("HFP HF Connected:%d!\n", err);
    default_br_conn = conn;
}

static void disconnected(struct bt_conn *conn)
{
    PRINTF("HFP BT Disconnected !\n");

    if (default_br_conn)
    {
        default_br_conn = NULL;
    }
}

static struct bt_hfp_hf_cb hf_cb = {
    .connected    = connected,
    .disconnected = disconnected,
};
static int cmd_init(const struct shell *sh, size_t argc, char *argv[])
{
    int err;
    bt_sdp_register_service(&hfp_hf_rec);

    err = bt_hfp_hf_register(&hf_cb);
    if (err < 0)
    {
        printf("HFP HF Registration failed (err %d)\n", err);
        return -EINVAL;
    }
    return 0;
}

#define HELP_NONE "[none]"
SHELL_STATIC_SUBCMD_SET_CREATE(hfp_cmds, SHELL_CMD_ARG(init, NULL, HELP_NONE, cmd_init, 1, 0), SHELL_SUBCMD_SET_END);

static int cmd_hfp(const struct shell *sh, size_t argc, char **argv)
{
    if (argc == 1)
    {
        shell_help(sh);
        /* shell returns 1 when help is printed */
        return SHELL_CMD_HELP_PRINTED;
    }

    shell_error(sh, "%s unknown parameter: %s", argv[0], argv[1]);

    return -EINVAL;
}

SHELL_CMD_ARG_REGISTER(hfp, hfp_cmds, "Bluetooth pbap shell commands", cmd_hfp, 1, 1);

void bt_ShellHfpInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandhfp))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandhfp.pcCommand);
    }
}

#endif
