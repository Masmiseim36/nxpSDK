/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include <string.h>
#include <porting.h>
#include "fsl_shell.h"

#include <sys/byteorder.h>

#include <bluetooth/hci.h>
#include <bluetooth/hci_vs.h>
#include <bluetooth/conn.h>
#include "shell_bt.h"
#include "shell_test_mode.h"

#if (defined(CONFIG_BT_RF_TEST_MODE) && (CONFIG_BT_RF_TEST_MODE > 0))
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void shell_print_response(const struct shell *sh, const uint8_t *data, size_t len)
{
    shell_dump(sh, " HCI Command Response : ");

    while (len--) {
        shell_dump(sh, "%02X ", *data++);
    }
	shell_dump(sh, "\r\n");
}

#if (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0))

static int bt_enter_test_mode(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    struct net_buf *rsp=NULL;

    shell_print(sh, "Enable device under test mode \r\n");

    err = bt_hci_cmd_send_sync(BT_HCI_OP_ENABLE_TEST_MODE, NULL, &rsp);
    if (err)
    {
        shell_print(sh, "Enable device under test mode command failed (err %d)\n", err);
        net_buf_unref(rsp);
        return -EINVAL;
    }
    else
    {
    	shell_print_response(sh,rsp->data,rsp->len);
    	net_buf_unref(rsp);
        return 0;
    }
}

static int bt_tx_test(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    struct net_buf *buf = NULL;
    struct net_buf *rsp = NULL;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
    }

    if (argc < 15)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: bt_test.tx_test test_scenario[1] hopping_mode[1] tx_channel[1] rx_channel[1] tx_test_interval[1] pkt_type[1] data_length[2] whitening[1] num_pkt[4] tx_pwr[1]\n");
        return -EINVAL;
    }
    if((strtol(argv[5],NULL,16)) > 0x0E)
    {
        shell_print(sh,"tx_test_interval= %x\n", strtol(argv[6],NULL,16));
        shell_print(sh,"invalid argument parameter for tx_test_interval. The value should be less than or equal to 0x0E\n");
        return -EINVAL;
    }

    struct bt_hci_cp_tx_test *cp;
    buf = bt_hci_cmd_create(BT_HCI_OP_TX_TEST, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        /*Set to default*/
        cp->rx_on_start = 0x80;
        shell_print(sh, "rx_on_start default set to=%x\n",cp->rx_on_start);

        cp->synt_on_start = 0x80;
        shell_print(sh, "synt_on_start default set to=%x\n",cp->synt_on_start);

        cp->tx_on_start = 0x80;
        shell_print(sh, "tx_on_start default set to=%x\n",cp->tx_on_start);

        cp->phd_off_start = 0x80;
        shell_print(sh, "phd_off_start default set to=%x\n",cp->phd_off_start);

        cp->test_scenario = strtol(argv[1],NULL,16);
        shell_print(sh, "test_scenario= %x\n", cp->test_scenario);

        cp->hopping_mode = strtol(argv[2],NULL,16);
        shell_print(sh, "hopping_mode= %x\n", cp->hopping_mode);

        cp->tx_channel = strtol(argv[3],NULL,16);
        shell_print(sh, "tx_channel= %x\n", cp->tx_channel);

        cp->rx_channel = strtol(argv[4],NULL,16);
        shell_print(sh, "rx_channel= %x\n", cp->rx_channel);

        cp->tx_test_interval = strtol(argv[5],NULL,16);
        shell_print(sh, "tx_test_interval= %x\n", cp->tx_test_interval);

        cp->pkt_type = strtol(argv[6],NULL,16);
        shell_print(sh, "pkt_type= %x\n", cp->pkt_type);

        cp->data_length[0] = strtol(argv[7],NULL,16);
        cp->data_length[1] = strtol(argv[8],NULL,16);
        shell_print(sh, "data_length= %x %x\n", cp->data_length[0],cp->data_length[1]);

        cp->whitening = strtol(argv[9],NULL,16);
        shell_print(sh, "whitening= %x\n", cp->whitening);

        cp->num_pkt[0] = strtol(argv[10],NULL,16);
        cp->num_pkt[1] = strtol(argv[11],NULL,16);
        cp->num_pkt[2] = strtol(argv[12],NULL,16);
        cp->num_pkt[3] = strtol(argv[13],NULL,16);
        shell_print(sh, "num_pkt= %x %x %x %x\n", cp->num_pkt[0],cp->num_pkt[1],cp->num_pkt[2],cp->num_pkt[3]);

        cp->tx_pwr = strtol(argv[14],NULL,16);
        shell_print(sh, "tx_pwr= %x\n", cp->tx_pwr);

        err = bt_hci_cmd_send_sync(BT_HCI_OP_TX_TEST, buf, &rsp);
    }
    else
    {
        err = -ENOBUFS;
        shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "BR/EDR transmitter test command failed (err %d)\n", err);
        net_buf_unref(rsp);
        return -EINVAL;
    }
    else
    {
        shell_print_response(sh,rsp->data,rsp->len);
        net_buf_unref(rsp);
        return 0;
    }
}


static int bt_rx_test(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    struct net_buf *buf = NULL;
    struct net_buf *rsp = NULL;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
    }


    if (argc < 18)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: bt_test.rx_test test_scenario[1] tx_channel[1] rx_channel[1] pkt_type[1] num_pkt[4] data_length[2] tx_addr[6] report_err_pkt[1]\r\n");
        return -EINVAL;
    }

    uint8_t index;
    struct bt_hci_cp_rx_test *cp;
    buf = bt_hci_cmd_create(BT_HCI_OP_RX_TEST, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));

        cp->test_scenario = strtol(argv[1],NULL,16);
        shell_print(sh, "test_scenario= %x\n", cp->test_scenario);

        cp->tx_channel = strtol(argv[2],NULL,16);
        shell_print(sh, "tx_channel= %x\n", cp->tx_channel);

        cp->rx_channel = strtol(argv[3],NULL,16);
        shell_print(sh, "rx_channel= %x\n", cp->rx_channel);

        cp->pkt_type = strtol(argv[4],NULL,16);
        shell_print(sh, "pkt_type= %x\n", cp->pkt_type);

        cp->num_pkt[0] = strtol(argv[5],NULL,16);
        cp->num_pkt[1] = strtol(argv[6],NULL,16);
        cp->num_pkt[2] = strtol(argv[7],NULL,16);
        cp->num_pkt[3] = strtol(argv[8],NULL,16);
        shell_print(sh, "num_pkt= %x %x %x %x\n", cp->num_pkt[0],cp->num_pkt[1],cp->num_pkt[2],cp->num_pkt[3]);

        cp->data_length[0] = strtol(argv[9],NULL,16);
        cp->data_length[1] = strtol(argv[10],NULL,16);
        shell_print(sh, "data_length= %x %x\n", cp->data_length[0],cp->data_length[1]);

        /*Set to default*/
        cp->tx_am_addr = 0x01;
        shell_print(sh, "tx_am_addr default set to= %x\n", cp->tx_am_addr);

        for (index = 0; index < BT_BD_ADDR_SIZE; index++)
        {
            cp->tx_addr[index]=strtol(argv[11+index], NULL,16);
        }

        shell_print(sh, "tx_addr: ");

        for (index = 0;index < BT_BD_ADDR_SIZE; index++)
        {
            shell_print(sh, "%x ",cp->tx_addr[index]);
        }

        shell_print(sh, "\n");

        cp->report_err_pkt = strtol(argv[17], NULL, 16);
        shell_print(sh, "report_err_pkt= %x\n", cp->report_err_pkt);

        err = bt_hci_cmd_send_sync(BT_HCI_OP_RX_TEST, buf, &rsp);
    }
    else
    {
        err = -ENOBUFS;
        shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "BR/EDR receiver test command failed (err %d)\n", err);
        net_buf_unref(rsp);
        return -EINVAL;
    }
    else
    {
        shell_print_response(sh,rsp->data,rsp->len);
        net_buf_unref(rsp);
        return 0;
    }
}

static int bt_reset(const struct shell *sh, size_t argc, char **argv)
{
    int err;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
    }

    err = bt_hci_cmd_send_sync(BT_HCI_OP_RESET,NULL,NULL);
    if (err)
    {
        shell_print(sh, "HCI reset command failed (err %d)\n", err);
        return -EINVAL;
    }
    else
    {
        shell_print(sh, "API returned success...\r\n");
    	return 0;
    }
}

#endif /* CONFIG_BT_CLASSIC */

static int hci_cmd_interface(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    struct net_buf *buf = NULL;
    struct net_buf *rsp = NULL;
    struct bt_hci_command command_buffer;


    if (argc < 3)
    {
    	shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: le_test.set_tx_power tx_power[1]\n");
        return -EINVAL;
    }

    command_buffer.ogf = strtol(argv[1],NULL,16);
    command_buffer.ocf = strtol(argv[2],NULL,16);

    command_buffer.opcode = ((command_buffer.ocf & 0x3ff) | (command_buffer.ogf << 10));
    command_buffer.param_len = argc - 3 ;

    uint8_t *bt_hci_cmd_params = NULL;

    buf = bt_hci_cmd_create(command_buffer.opcode, command_buffer.param_len);
    if (buf != NULL)
    {
    	bt_hci_cmd_params = net_buf_add(buf, command_buffer.param_len);

        for(int i= 0 ; i < command_buffer.param_len ; i++)
        {
        	bt_hci_cmd_params[i] = strtol(argv[i+3],NULL, 16);
        }

        err = bt_hci_cmd_send_sync(command_buffer.opcode, buf, &rsp);
        if (command_buffer.opcode == BT_HCI_OP_LE_TEST_TX_COUNT)
        {
            shell_dump(sh, "LE TX pkt number: %d\r\n", sys_get_le16(&rsp->data[1]));
        }
    }
    else
    {
    	err = -ENOBUFS;
    	shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "HCI command failed (err %d)\n", err);
        if(rsp)
        {
          net_buf_unref(rsp);
        }
        return -EINVAL;
    }
    else
    {
    	shell_print_response(sh,rsp->data,rsp->len);
    	net_buf_unref(rsp);
    	return 0;
    }
}

static int le_set_tx_power(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    struct net_buf *buf = NULL, *rsp=NULL;

    if (argc < 1)
    {
    	shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: le_test.set_tx_power tx_power[1]\n");
        return -EINVAL;
    }

    struct bt_hci_le_config *cp;
    buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_TX_POWER, sizeof(*cp));
    if (buf != NULL)
    {
    	cp = net_buf_add(buf, sizeof(*cp));

    	cp->tx_power = strtol(argv[1],NULL,16);
        cp->feloss= strtol(argv[2],NULL,16);
        if(cp->feloss >= 1)
        {
            cp->feloss = 1;
        }

    	shell_print(sh, "tx_power= %x\n", cp->tx_power);
        shell_print(sh, "feloss= %x\n", cp->feloss);

    	err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_TX_POWER, buf, &rsp);
    }
    else
    {
    	err = -ENOBUFS;
    	shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "LE Set TX Power command failed (err %d)\n", err);
        net_buf_unref(rsp);
        return -EINVAL;
    }
    else
    {
        shell_print_response(sh,rsp->data,rsp->len);
        net_buf_unref(rsp);
        return 0;
    }
}

static int le_tx_test(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    struct net_buf *buf = NULL;
    struct net_buf *rsp = NULL;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        return -EINVAL;
    }

    if (argc < 5)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: le_test.tx_test tx_channel[1] test_data_len[1] pkt_payload[1] phy[1]\r\n");
        return -EINVAL;
    }

    struct bt_hci_cp_le_enh_tx_test *cp;
    buf = bt_hci_cmd_create(BT_HCI_OP_LE_ENH_TX_TEST, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        cp->tx_ch = strtol(argv[1],NULL,16);
        shell_print(sh, "tx_channel= %x\n", cp->tx_ch);

        cp->test_data_len = strtol(argv[2],NULL,16);
        shell_print(sh, "test_data_len= %x\n", cp->test_data_len);

        cp->pkt_payload = strtol(argv[3],NULL,16);
        shell_print(sh, "pkt_payload= %x\n", cp->pkt_payload);

        cp->phy = strtol(argv[4],NULL,16);
        shell_print(sh, "phy= %x\n", cp->phy);
        err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_ENH_TX_TEST, buf, &rsp);
    }
    else
    {
        err = -ENOBUFS;
        shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "LE enhanced transmitter test command failed (err %d)\n", err);
        net_buf_unref(rsp);
        return -EINVAL;
    }
    else
    {
        shell_print_response(sh,rsp->data,rsp->len);
        net_buf_unref(rsp);
        return 0;
    }
}


static int le_rx_test(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    struct net_buf *buf = NULL;
    struct net_buf *rsp = NULL;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        return -EINVAL;
    }

    if (argc < 4)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        shell_print(sh, "Usage: le_test.rx_test rx_channel[1] phy[1] modulation_index[1]\r\n");
        return -EINVAL;
    }

    struct bt_hci_cp_le_enh_rx_test *cp;
    buf = bt_hci_cmd_create(BT_HCI_OP_LE_ENH_RX_TEST, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        cp->rx_ch =  strtol(argv[1],NULL,16);
        shell_print(sh, "rx_channel= %x\n", cp->rx_ch);

        cp->phy = strtol(argv[2],NULL,16);
        shell_print(sh, "phy= %x\n", cp->phy);

        cp->mod_index = strtol(argv[3],NULL,16);
        shell_print(sh, "modulation_index= %x\n", cp->mod_index);

        err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_ENH_RX_TEST, buf, &rsp);
    }
    else
    {
        err = -ENOBUFS;
        shell_print(sh, "No buffer space available\r\n");
    }

    if (err)
    {
        shell_print(sh, "LE enhanced receiver test command failed (err %d)\n", err);
        net_buf_unref(rsp);
        return -EINVAL;
    }
    else
    {
        shell_print_response(sh,rsp->data,rsp->len);
        net_buf_unref(rsp);
        return 0;
    }
}

static int le_end_test(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    struct net_buf *rsp;
    struct bt_hci_rp_le_test_end *rp;

    if (argc < 1)
    {
        shell_print(sh, "the parameter count is wrong\r\n");
        return -EINVAL;
    }

    err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_TEST_END, NULL, &rsp);
    if (err)
    {
        shell_print(sh, "LE test end command failed (err %d)\n", err);
        return -EINVAL;
    }
    else
    {
        rp = (void *)rsp->data;
        shell_print(sh, "Number of packets received: %d\n", (int)rp->rx_pkt_count);
        net_buf_unref(rsp);
        shell_print(sh, "API returned success...\n");
    	return 0;
    }
}


SHELL_STATIC_SUBCMD_SET_CREATE(hci_cmds,
    SHELL_CMD_ARG(generic_command, NULL, "ogf[1] ocf[1] params....", hci_cmd_interface, 2, 255),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(le_cmds,
    SHELL_CMD_ARG(set_tx_power, NULL, "tx_power[1]", le_set_tx_power, 2, 0),
    SHELL_CMD_ARG(tx_test, NULL, "tx_channel[1] data_length[1] payload[1] phy[1]", le_tx_test, 2, 6),
    SHELL_CMD_ARG(rx_test, NULL, "rc_channel[1] phy[1] modulation[1]", le_rx_test, 2, 5),
    SHELL_CMD_ARG(end_test, NULL, "end the le test", le_end_test, 1, 0),
    SHELL_SUBCMD_SET_END
);

#if (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0))
SHELL_STATIC_SUBCMD_SET_CREATE(br_cmds,
    SHELL_CMD_ARG(enter_test_mode, NULL, "Enable device under test mode", bt_enter_test_mode, 1, 0),
    SHELL_CMD_ARG(tx_test, NULL, "test_scenario[1] hopping_mode[1] tx_channel[1] rx_channel[1] tx_test_interval[1] pkt_type[1] data_length[2] whitening[1] num_pkt[4] tx_pwr[1]", bt_tx_test, 2, 19),
    SHELL_CMD_ARG(rx_test, NULL, "test_scenario[1] tx_channel[1] rx_channel[1] pkt_type[1] num_pkt[4] data_length[2] tx_addr[6] report_err_pkt[1]", bt_rx_test, 2, 19),
    SHELL_CMD_ARG(reset, NULL, " Reset the HCI interface", bt_reset, 1, 0),
    SHELL_SUBCMD_SET_END
);
#endif

static int cmd_bt_test(const struct shell *sh, size_t argc, char **argv)
{
    if (argc == 1) {
        shell_help(sh);
        return SHELL_CMD_HELP_PRINTED;
    }

    shell_error(sh, "%s unknown parameter: %s", argv[0], argv[1]);

    return -EINVAL;
}

#if (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0))
SHELL_CMD_ARG_REGISTER(bt_test, br_cmds, "Bluetooth BR/EDR test mode commands",
               cmd_bt_test, 1, 17);
#endif
SHELL_CMD_ARG_REGISTER(le_test, le_cmds, "Bluetooth BLE test mode commands",
               cmd_bt_test, 1, 6);

SHELL_CMD_ARG_REGISTER(hci, hci_cmds, "Bluetooth HCI Command interface",
               cmd_bt_test, 1, 255);


void bt_ShellTestModeInit(shell_handle_t shell)
{
    /* Add new command to commands list */
#if (defined(CONFIG_BT_CLASSIC) && (CONFIG_BT_CLASSIC > 0))
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandbt_test))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandbt_test.pcCommand);
    }
#endif /* CONFIG_BT_CLASSIC */
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandle_test))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandle_test.pcCommand);
    }

    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandhci))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandle_test.pcCommand);
    }

}
#endif /* CONFIG_BT_RF_TEST_MODE */