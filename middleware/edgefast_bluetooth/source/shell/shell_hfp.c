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
#include "db_gen.h"
#include "BT_common.h"
#include "fsl_shell.h"
#include "shell_bt.h"
#include <bluetooth/hfp_hf.h>

#if (defined(CONFIG_BT_HFP_HF) && (CONFIG_BT_HFP_HF > 0U))

static void connected(struct bt_conn *conn)
{
    printf("HFP HF Connected!\n");
    default_conn = conn;
}

static void disconnected(struct bt_conn *conn)
{
    PRINTF("HFP BT Disconnected !\n");

    if (default_conn)
    {
        bt_conn_unref(default_conn);
        default_conn = NULL;
    }
}

static struct bt_hfp_hf_cb hf_cb = {
    .connected    = connected,
    .disconnected = disconnected,
};
static int cmd_init(const struct shell *sh, size_t argc, char *argv[])
{
    int err;

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
