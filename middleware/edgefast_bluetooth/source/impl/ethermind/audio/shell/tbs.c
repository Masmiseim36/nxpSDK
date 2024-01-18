/** @file
 *  @brief Bluetooth Telephone Bearer Service shell
 */

/*
 * Copyright (c) 2020-2021 Nordic Semiconductor ASA
 * Copyright (C) 2022-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if (defined(CONFIG_BT_TBS) && (CONFIG_BT_TBS > 0))

#include <stdlib.h>

#include "fsl_shell.h"

#include <bluetooth/audio/tbs.h>

#include "shell_bt.h"

static struct bt_conn *tbs_authorized_conn;
static bool cbs_registered;

static bool tbs_authorize_cb(struct bt_conn *conn)
{
	return conn == tbs_authorized_conn;
}

static bool tbs_originate_call_cb(struct bt_conn *conn, uint8_t call_index,
				  const char *uri)
{
	/* Always accept calls */
	return true;
}

static struct bt_tbs_cb tbs_cbs = {
	.originate_call = tbs_originate_call_cb,
	.authorize = tbs_authorize_cb
};

static shell_status_t cmd_tbs_authorize(shell_handle_t sh, int32_t argc, char *argv[])
{
	char addr[BT_ADDR_LE_STR_LEN];

	tbs_authorized_conn = default_conn;

	(void)bt_addr_le_to_str(bt_conn_get_dst(tbs_authorized_conn),
				addr, sizeof(addr));

	shell_print(sh, "Connection with addr %s authorized", addr);

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_tbs_init(shell_handle_t sh, int32_t argc, char *argv[])
{
	if (!cbs_registered) {
		bt_tbs_register_cb(&tbs_cbs);
		cbs_registered = true;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_tbs_accept(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long call_index;
	int result = 0;

	call_index = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse call_index: %d", result);

		return kStatus_SHELL_Error;
	}

	if (call_index > UINT8_MAX) {
		shell_error(sh, "Invalid call_index: %lu", call_index);

		return kStatus_SHELL_Error;
	}

	result = bt_tbs_accept((uint8_t)call_index);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "TBS failed: %d", result);
	} else {
		shell_print(sh, "TBS succeeded for call_index: %ld",
			    call_index);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_terminate(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long call_index;
	int result = 0;

	call_index = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse call_index: %d", result);

		return kStatus_SHELL_Error;
	}

	if (call_index > UINT8_MAX) {
		shell_error(sh, "Invalid call_index: %lu", call_index);

		return kStatus_SHELL_Error;
	}

	result = bt_tbs_terminate((uint8_t)call_index);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "TBS failed: %d", result);
	} else {
		shell_print(sh, "TBS succeeded for call_index: %ld",
			    call_index);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_hold(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long call_index;
	int result = 0;

	call_index = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse call_index: %d", result);

		return kStatus_SHELL_Error;
	}

	if (call_index > UINT8_MAX) {
		shell_error(sh, "Invalid call_index: %lu", call_index);

		return kStatus_SHELL_Error;
	}

	result = bt_tbs_hold((uint8_t)call_index);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "TBS failed: %d", result);
	} else {
		shell_print(sh, "TBS succeeded for call_index: %ld",
			    call_index);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_retrieve(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long call_index;
	int result = 0;

	call_index = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse call_index: %d", result);

		return kStatus_SHELL_Error;
	}

	if (call_index > UINT8_MAX) {
		shell_error(sh, "Invalid call_index: %lu", call_index);

		return kStatus_SHELL_Error;
	}

	result = bt_tbs_retrieve((uint8_t)call_index);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "TBS failed: %d", result);
	} else {
		shell_print(sh, "TBS succeeded for call_index: %ld",
			    call_index);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_originate(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long service_index;
	uint8_t call_index;
	int result = 0;

	if (argc > 2) {
		service_index = shell_strtoul(argv[1], 0, &result);
		if (result != 0) {
			shell_error(sh, "Could not parse service_index: %d",
				    result);

			return kStatus_SHELL_Error;
		}

		if (service_index > CONFIG_BT_TBS_BEARER_COUNT) {
			shell_error(sh, "Invalid service_index: %lu",
				    service_index);

			return kStatus_SHELL_Error;
		}
	} else {
		service_index = 0U;
	}

	result = bt_tbs_originate((uint8_t)service_index, argv[argc - 1],
				  &call_index);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "TBS failed: %d", result);
	} else {
		shell_print(sh, "TBS call_index %u originated", call_index);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_join(shell_handle_t sh, int32_t argc, char *argv[])
{
	uint8_t call_indexes[CONFIG_BT_TBS_MAX_CALLS];
	unsigned long call_index;
	int result = 0;

	for (size_t i = 1; i < argc; i++) {
		call_index = shell_strtoul(argv[i], 0, &result);
		if (result != 0) {
			shell_error(sh, "Could not parse call_index: %d",
				    result);

			return kStatus_SHELL_Error;
		}

		if (call_index > UINT8_MAX) {
			shell_error(sh, "Invalid call_index: %lu", call_index);

			return kStatus_SHELL_Error;
		}

		call_indexes[i - 1] = (uint8_t)call_index;
	}

	result = bt_tbs_join(argc - 1, call_indexes);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "TBS failed: %d", result);
	} else {
		shell_print(sh, "TBS join succeeded");
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_answer(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long call_index;
	int result = 0;

	call_index = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse call_index: %d", result);

		return kStatus_SHELL_Error;
	}

	if (call_index > UINT8_MAX) {
		shell_error(sh, "Invalid call_index: %lu", call_index);

		return kStatus_SHELL_Error;
	}

	result = bt_tbs_remote_answer((uint8_t)call_index);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "TBS failed: %d", result);
	} else {
		shell_print(sh, "TBS succeeded for call_index: %ld",
			    call_index);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_remote_hold(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long call_index;
	int result = 0;

	call_index = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse call_index: %d", result);

		return kStatus_SHELL_Error;
	}

	if (call_index > UINT8_MAX) {
		shell_error(sh, "Invalid call_index: %lu", call_index);

		return kStatus_SHELL_Error;
	}

	result = bt_tbs_remote_hold((uint8_t)call_index);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "TBS failed: %d", result);
	} else {
		shell_print(sh, "TBS succeeded for call_index: %ld",
			    call_index);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_remote_retrieve(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long call_index;
	int result = 0;

	call_index = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse call_index: %d", result);

		return kStatus_SHELL_Error;
	}

	if (call_index > UINT8_MAX) {
		shell_error(sh, "Invalid call_index: %lu", call_index);

		return kStatus_SHELL_Error;
	}

	result = bt_tbs_remote_retrieve((uint8_t)call_index);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "TBS failed: %d", result);
	} else {
		shell_print(sh, "TBS succeeded for call_index: %ld",
			    call_index);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_remote_terminate(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long call_index;
	int result = 0;

	call_index = shell_strtoul(argv[1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse call_index: %d", result);

		return kStatus_SHELL_Error;
	}

	if (call_index > UINT8_MAX) {
		shell_error(sh, "Invalid call_index: %lu", call_index);

		return kStatus_SHELL_Error;
	}

	result = bt_tbs_remote_terminate((uint8_t)call_index);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "TBS failed: %d", result);
	} else {
		shell_print(sh, "TBS succeeded for call_index: %ld",
			    call_index);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_incoming(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long service_index;
	int result = 0;

	if (argc > 4) {
		if (strcmp(argv[1], "gtbs") == 0) {
			service_index = BT_TBS_GTBS_INDEX;
		} else {
			service_index = shell_strtoul(argv[1], 0, &result);
			if (result != 0) {
				shell_error(sh,
					    "Could not parse service_index: %d",
					    result);

				return kStatus_SHELL_Error;
			}

			if (service_index > CONFIG_BT_TBS_BEARER_COUNT) {
				shell_error(sh, "Invalid service_index: %lu",
					    service_index);

				return kStatus_SHELL_Error;
			}
		}
	} else {
		service_index = 0U;
	}

	result = bt_tbs_remote_incoming((uint8_t)service_index,
					argv[argc - 3],
					argv[argc - 2],
					argv[argc - 1]);
	if (result < 0) {
		shell_print(sh, "TBS failed: %d", result);
	} else {
		shell_print(sh, "TBS succeeded");
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_set_bearer_provider_name(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long service_index;
	int result = 0;

	if (argc > 2) {
		if (strcmp(argv[1], "gtbs") == 0) {
			service_index = BT_TBS_GTBS_INDEX;
		} else {
			service_index = shell_strtoul(argv[1], 0, &result);
			if (result != 0) {
				shell_error(sh,
					    "Could not parse service_index: %d",
					    result);

				return kStatus_SHELL_Error;
			}

			if (service_index > CONFIG_BT_TBS_BEARER_COUNT) {
				shell_error(sh, "Invalid service_index: %lu",
					    service_index);

				return kStatus_SHELL_Error;
			}
		}
	} else {
		service_index = 0U;
	}

	result = bt_tbs_set_bearer_provider_name((uint8_t)service_index,
						 argv[argc - 1]);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "Could not set provider name: %d", result);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_set_bearer_technology(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long service_index;
	unsigned long technology;
	int result = 0;

	if (argc > 2) {
		if (strcmp(argv[1], "gtbs") == 0) {
			service_index = BT_TBS_GTBS_INDEX;
		} else {
			service_index = shell_strtoul(argv[1], 0, &result);
			if (result != 0) {
				shell_error(sh,
					    "Could not parse service_index: %d",
					    result);

				return kStatus_SHELL_Error;
			}

			if (service_index > CONFIG_BT_TBS_BEARER_COUNT) {
				shell_error(sh, "Invalid service_index: %lu",
					    service_index);

				return kStatus_SHELL_Error;
			}
		}
	} else {
		service_index = 0U;
	}

	technology = shell_strtoul(argv[argc - 1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse technology: %d", result);

		return kStatus_SHELL_Error;
	}

	if (technology > UINT8_MAX) {
		shell_error(sh, "Invalid technology: %lu", technology);

		return kStatus_SHELL_Error;
	}

	result = bt_tbs_set_bearer_technology((uint8_t)service_index,
					      (uint8_t)technology);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "Could not set technology: %d", result);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_set_bearer_signal_strength(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long signal_strength;
	unsigned long service_index;
	int result = 0;

	if (argc > 2) {
		if (strcmp(argv[1], "gtbs") == 0) {
			service_index = BT_TBS_GTBS_INDEX;
		} else {
			service_index = shell_strtoul(argv[1], 0, &result);
			if (result != 0) {
				shell_error(sh,
					    "Could not parse service_index: %d",
					    result);

				return kStatus_SHELL_Error;
			}

			if (service_index > CONFIG_BT_TBS_BEARER_COUNT) {
				shell_error(sh, "Invalid service_index: %lu",
					    service_index);

				return kStatus_SHELL_Error;
			}
		}
	} else {
		service_index = 0U;
	}

	signal_strength = shell_strtoul(argv[argc - 1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse signal_strength: %d", result);

		return kStatus_SHELL_Error;
	}

	if (signal_strength > UINT8_MAX) {
		shell_error(sh, "Invalid signal_strength: %lu",
			    signal_strength);

		return kStatus_SHELL_Error;
	}

	result = bt_tbs_set_signal_strength((uint8_t)service_index,
					    (uint8_t)signal_strength);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "Could not set signal strength: %d", result);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_set_status_flags(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long service_index;
	unsigned long status_flags;
	int result = 0;

	if (argc > 2) {
		if (strcmp(argv[1], "gtbs") == 0) {
			service_index = BT_TBS_GTBS_INDEX;
		} else {
			service_index = shell_strtoul(argv[1], 0, &result);
			if (result != 0) {
				shell_error(sh,
					    "Could not parse service_index: %d",
					    result);

				return kStatus_SHELL_Error;
			}

			if (service_index > CONFIG_BT_TBS_BEARER_COUNT) {
				shell_error(sh, "Invalid service_index: %lu",
					    service_index);

				return kStatus_SHELL_Error;
			}
		}
	} else {
		service_index = 0U;
	}

	status_flags = shell_strtoul(argv[argc - 1], 0, &result);
	if (result != 0) {
		shell_error(sh, "Could not parse status_flags: %d", result);

		return kStatus_SHELL_Error;
	}

	if (status_flags > UINT8_MAX) {
		shell_error(sh, "Invalid status_flags: %lu", status_flags);

		return kStatus_SHELL_Error;
	}

	result = bt_tbs_set_status_flags((uint8_t)service_index,
					 (uint16_t)status_flags);
	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "Could not set status flags: %d", result);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_set_uri_scheme_list(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long service_index;
	int result = 0;

	if (argc > 2) {
		if (strcmp(argv[1], "gtbs") == 0) {
			service_index = BT_TBS_GTBS_INDEX;
		} else {
			service_index = shell_strtoul(argv[1], 0, &result);
			if (result != 0) {
				shell_error(sh,
					    "Could not parse service_index: %d",
					    result);

				return kStatus_SHELL_Error;
			}

			if (service_index > CONFIG_BT_TBS_BEARER_COUNT) {
				shell_error(sh, "Invalid service_index: %lu",
					    service_index);

				return kStatus_SHELL_Error;
			}
		}
	} else {
		service_index = 0U;
	}

	result = bt_tbs_set_uri_scheme_list((uint8_t)service_index,
					    (const char **)&argv[2],
					    argc - 2);

	if (result != BT_TBS_RESULT_CODE_SUCCESS) {
		shell_print(sh, "Could not set URI prefix list: %d", result);
	}

	return (shell_status_t)result;
}

static shell_status_t cmd_tbs_print_calls(shell_handle_t sh, int32_t argc, char *argv[])
{
#if (defined(CONFIG_BT_DEBUG_TBS) && (CONFIG_BT_DEBUG_TBS > 0))
	if (IS_ENABLED(CONFIG_BT_DEBUG_TBS)) {
		bt_tbs_dbg_print_calls();
		return 0;
	}
#endif /* CONFIG_BT_DEBUG_TBS */

	return kStatus_SHELL_Error;
}

static shell_status_t cmd_tbs(shell_handle_t sh, int32_t argc, char *argv[])
{
	if (argc > 1) {
		shell_error(sh, "%s unknown parameter: %s", argv[0],
			    argv[1]);
	} else {
		shell_error(sh, "%s Missing subcommand", argv[0]);
	}

	return kStatus_SHELL_Error;
}

SHELL_STATIC_SUBCMD_SET_CREATE(tbs_cmds,
	SHELL_CMD_ARG(init, NULL,
		      "Initialize TBS",
		      cmd_tbs_init, 1, 0),
	SHELL_CMD_ARG(authorize, NULL,
		      "Authorize the current connection",
		      cmd_tbs_authorize, 1, 0),
	SHELL_CMD_ARG(accept, NULL,
		      "Accept call <call_index>",
		      cmd_tbs_accept, 2, 0),
	SHELL_CMD_ARG(terminate, NULL,
		      "Terminate call <call_index>",
		      cmd_tbs_terminate, 2, 0),
	SHELL_CMD_ARG(hold, NULL,
		      "Hold call <call_index>",
		      cmd_tbs_hold, 2, 0),
	SHELL_CMD_ARG(retrieve, NULL,
		      "Retrieve call <call_index>",
		      cmd_tbs_retrieve, 2, 0),
	SHELL_CMD_ARG(originate, NULL,
		      "Originate call [<instance_index>] <uri>",
		      cmd_tbs_originate, 2, 1),
#if CONFIG_BT_TBS_MAX_CALLS > 1
	SHELL_CMD_ARG(join, NULL,
		      "Join calls <id> <id> [<id> [<id> [...]]]",
		      cmd_tbs_join, 3, CONFIG_BT_TBS_MAX_CALLS - 2),
#endif /* CONFIG_BT_TBS_MAX_CALLS > 1 */
	SHELL_CMD_ARG(incoming, NULL,
		      "Simulate incoming remote call "
		      "[<{instance_index, gtbs}>] <local_uri> <remote_uri> "
		      "<remote_friendly_name>",
		      cmd_tbs_incoming, 4, 1),
	SHELL_CMD_ARG(remote_answer, NULL,
		      "Simulate remote answer outgoing call <call_index>",
		      cmd_tbs_answer, 2, 0),
	SHELL_CMD_ARG(remote_retrieve, NULL,
		      "Simulate remote retrieve <call_index>",
		      cmd_tbs_remote_retrieve, 2, 0),
	SHELL_CMD_ARG(remote_terminate, NULL,
		      "Simulate remote terminate <call_index>",
		      cmd_tbs_remote_terminate, 2, 0),
	SHELL_CMD_ARG(remote_hold, NULL,
		      "Simulate remote hold <call_index>",
		      cmd_tbs_remote_hold, 2, 0),
	SHELL_CMD_ARG(set_bearer_provider_name, NULL,
		      "Set the bearer provider name [<{instance_index, gtbs}>] "
		      "<name>",
		      cmd_tbs_set_bearer_provider_name, 2, 1),
	SHELL_CMD_ARG(set_bearer_technology, NULL,
		      "Set the bearer technology [<{instance_index, gtbs}>] "
		      "<technology>",
		      cmd_tbs_set_bearer_technology, 2, 1),
	SHELL_CMD_ARG(set_bearer_signal_strength, NULL,
		      "Set the bearer signal strength "
		      "[<{instance_index, gtbs}>] <strength>",
		      cmd_tbs_set_bearer_signal_strength, 2, 1),
	SHELL_CMD_ARG(set_status_flags, NULL,
		      "Set the bearer feature and status value "
		      "[<{instance_index, gtbs}>] <feature_and_status>",
		      cmd_tbs_set_status_flags, 2, 1),
	SHELL_CMD_ARG(set_uri_scheme, NULL,
		      "Set the URI prefix list <bearer_idx> "
		      "<uri1 [uri2 [uri3 [...]]]>",
		      cmd_tbs_set_uri_scheme_list, 3, 30),
	SHELL_CMD_ARG(print_calls, NULL,
		      "Output all calls in the debug log",
		      cmd_tbs_print_calls, 1, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(tbs, tbs_cmds, "Bluetooth TBS shell commands",
		       cmd_tbs, 1, 1);

void bt_ShellTbsInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandtbs))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandtbs.pcCommand);
    }

	bt_tbs_init();
}

#endif /* CONFIG_BT_TBS */