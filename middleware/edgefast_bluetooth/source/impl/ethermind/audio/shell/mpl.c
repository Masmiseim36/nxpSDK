/** @file
 *  @brief Media player shell
 *
 */

/*
 * Copyright (c) 2020 - 2021 Nordic Semiconductor ASA
 * Copyright (C) 2022 - 2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if (defined(CONFIG_BT_MPL) && (CONFIG_BT_MPL > 0))

#include <stdlib.h>
#include "fsl_shell.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>

#include "shell_bt.h"

#include <bluetooth/audio/media_proxy.h>
#include "mpl_internal.h"

#define BT_DBG_ENABLED IS_ENABLED(CONFIG_BT_DEBUG_MPL)
#define LOG_MODULE_NAME bt_mpl_shell
#include "fsl_component_log.h"

#if (defined(CONFIG_BT_DEBUG_MPL) && (CONFIG_BT_DEBUG_MPL > 0)) && \
	(defined(CONFIG_BT_TESTING) && (CONFIG_BT_TESTING > 0))
static shell_status_t cmd_mpl_test_set_media_state(shell_handle_t sh, int32_t argc, char *argv[])
{
	unsigned long state;
	int err = 0;

	state = shell_strtoul(argv[1], 0, &err);
	if (err != 0) {
		shell_error(sh, "Could not parse state: %d", err);

		return kStatus_SHELL_Error;
	}

	if (state > UINT8_MAX) {
		shell_error(sh, "Invalid state %lu", state);

		return kStatus_SHELL_Error;
	}

	mpl_test_media_state_set(state);

	return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_MPL_OBJECTS) && (CONFIG_BT_MPL_OBJECTS > 0))
static shell_status_t cmd_mpl_test_unset_parent_group(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_unset_parent_group();

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_MPL_OBJECTS */
#endif /* CONFIG_BT_DEBUG_MPL && CONFIG_BT_TESTING */

#if (defined(CONFIG_BT_DEBUG_MPL) && (CONFIG_BT_DEBUG_MPL > 0))
static shell_status_t cmd_mpl_debug_dump_state(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_debug_dump_state();

	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_DEBUG_MPL */

static shell_status_t cmd_media_proxy_pl_init(shell_handle_t sh, int32_t argc, char *argv[])
{
	if (!ctx_shell) {
		ctx_shell = sh;
	}

	int err = media_proxy_pl_init();

	if (err) {
		shell_error(sh, "Could not init mpl");
	}

	return (shell_status_t)err;
}

static shell_status_t cmd_mpl_test_player_name_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_player_name_changed_cb();

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_player_icon_url_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_player_icon_url_changed_cb();

	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_track_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_track_changed_cb();
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_title_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_title_changed_cb();
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_duration_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_duration_changed_cb();
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_position_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_position_changed_cb();
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_playback_speed_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_playback_speed_changed_cb();
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_seeking_speed_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_seeking_speed_changed_cb();
	return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_MPL_OBJECTS) && (CONFIG_BT_MPL_OBJECTS > 0))
static shell_status_t cmd_mpl_test_current_track_id_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_current_track_id_changed_cb();
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_next_track_id_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_next_track_id_changed_cb();
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_current_group_id_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_current_group_id_changed_cb();
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_parent_group_id_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_parent_group_id_changed_cb();
	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_MPL_OBJECTS */

static shell_status_t cmd_mpl_test_playing_order_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_playing_order_changed_cb();
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_state_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_media_state_changed_cb();
	return kStatus_SHELL_Success;
}

static shell_status_t cmd_mpl_test_media_opcodes_supported_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_opcodes_supported_changed_cb();
	return kStatus_SHELL_Success;
}

#if (defined(CONFIG_BT_MPL_OBJECTS) && (CONFIG_BT_MPL_OBJECTS > 0))
static shell_status_t cmd_mpl_test_search_results_changed_cb(shell_handle_t sh, int32_t argc, char *argv[])
{
	mpl_test_search_results_changed_cb();
	return kStatus_SHELL_Success;
}
#endif /* CONFIG_BT_MPL_OBJECTS */

static shell_status_t cmd_mpl(shell_handle_t sh, int32_t argc, char *argv[])
{
	shell_error(sh, "%s unknown parameter: %s", argv[0], argv[1]);

	return kStatus_SHELL_Error;
}

SHELL_STATIC_SUBCMD_SET_CREATE(mpl_cmds,
#if (defined(CONFIG_BT_DEBUG_MPL) && (CONFIG_BT_DEBUG_MPL > 0)) && \
	(defined(CONFIG_BT_TESTING) && (CONFIG_BT_TESTING > 0))
	SHELL_CMD_ARG(test_set_media_state, NULL,
		      "Set the media player state (test) <state>",
		      cmd_mpl_test_set_media_state, 2, 0),
#if CONFIG_BT_MPL_OBJECTS
	SHELL_CMD_ARG(test_unset_parent_group, NULL,
		      "Set current group to be its own parent (test)",
		      cmd_mpl_test_unset_parent_group, 1, 0),
#endif /* CONFIG_BT_MPL_OBJECTS */
#endif /* CONFIG_BT_DEBUG_MPL && CONFIG_BT_TESTING */
#if (defined(CONFIG_BT_DEBUG_MPL) && (CONFIG_BT_DEBUG_MPL > 0))
	SHELL_CMD_ARG(debug_dump_state, NULL,
		      "Dump media player's state as debug output (debug)",
		      cmd_mpl_debug_dump_state, 1, 0),
#endif /* CONFIG_BT_DEBUG_MPL */
	SHELL_CMD_ARG(init, NULL,
		      "Initialize media player",
		      cmd_media_proxy_pl_init, 1, 0),
	SHELL_CMD_ARG(player_name_changed_cb, NULL,
		      "Trigger Player Name changed callback (test)",
		      cmd_mpl_test_player_name_cb, 1, 0),
	SHELL_CMD_ARG(player_icon_url_changed_cb, NULL,
		      "Trigger Player icon URL changed callback (test)",
		      cmd_mpl_test_player_icon_url_cb, 1, 0),
	SHELL_CMD_ARG(track_changed_cb, NULL,
		      "Trigger Track Changed callback (test)",
		      cmd_mpl_test_track_changed_cb, 1, 0),
	SHELL_CMD_ARG(title_changed_cb, NULL,
		      "Trigger Track Title callback (test)",
		      cmd_mpl_test_title_changed_cb, 1, 0),
	SHELL_CMD_ARG(duration_changed_cb, NULL,
		      "Trigger Track Duration callback (test)",
		      cmd_mpl_test_duration_changed_cb, 1, 0),
	SHELL_CMD_ARG(position_changed_cb, NULL,
		      "Trigger Track Position callback (test)",
		      cmd_mpl_test_position_changed_cb, 1, 0),
	SHELL_CMD_ARG(playback_speed_changed_cb, NULL,
		      "Trigger Playback Speed callback (test)",
		      cmd_mpl_test_playback_speed_changed_cb, 1, 0),
	SHELL_CMD_ARG(seeking_speed_changed_cb, NULL,
		      "Trigger Seeking Speed callback (test)",
		      cmd_mpl_test_seeking_speed_changed_cb, 1, 0),
#if (defined(CONFIG_BT_MPL_OBJECTS) && (CONFIG_BT_MPL_OBJECTS > 0))
	SHELL_CMD_ARG(current_track_id_changed_cb, NULL,
		      "Trigger Current Track callback (test)",
		      cmd_mpl_test_current_track_id_changed_cb, 1, 0),
	SHELL_CMD_ARG(next_track_id_changed_cb, NULL,
		      "Trigger Next Track callback (test)",
		      cmd_mpl_test_next_track_id_changed_cb, 1, 0),
	SHELL_CMD_ARG(current_group_id_changed_cb, NULL,
		      "Trigger Current Group callback (test)",
		      cmd_mpl_test_current_group_id_changed_cb, 1, 0),
	SHELL_CMD_ARG(parent_group_id_changed_cb, NULL,
		      "Trigger Parent Group callback (test)",
		      cmd_mpl_test_parent_group_id_changed_cb, 1, 0),
#endif /* CONFIG_BT_MPL_OBJECTS */
	SHELL_CMD_ARG(playing_order_changed_cb, NULL,
		      "Trigger Playing Order callback (test)",
		      cmd_mpl_test_playing_order_changed_cb, 1, 0),
	SHELL_CMD_ARG(state_changed_cb, NULL,
		      "Trigger Media State callback (test)",
		      cmd_mpl_test_state_changed_cb, 1, 0),
	SHELL_CMD_ARG(media_opcodes_changed_cb, NULL,
		      "Trigger Opcodes Supported callback (test)",
		      cmd_mpl_test_media_opcodes_supported_changed_cb, 1, 0),
#if (defined(CONFIG_BT_MPL_OBJECTS) && (CONFIG_BT_MPL_OBJECTS > 0))
	SHELL_CMD_ARG(search_results_changed_cb, NULL,
		      "Trigger Search Results Object ID callback (test)",
		      cmd_mpl_test_search_results_changed_cb, 1, 0),
#endif /* CONFIG_BT_MPL_OBJECTS */
	SHELL_SUBCMD_SET_END
);

/* TODO Remove this workaround macro once Scancode has been updated to
 * not report this as a false positive MPL license
 *
 * See https://github.com/nexB/scancode-toolkit/issues/2304
 * and https://github.com/nexB/scancode-toolkit/commit/6abbc4a22973f40ab74f6f8d948dd06416c97bd4
 */
#define CMD_NQM cmd_mpl
SHELL_CMD_REGISTER(mpl, mpl_cmds, "Media player (MPL) related commands",
		       CMD_NQM, 1, 1);

void bt_ShellMplInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandmpl))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommandmpl.pcCommand);
    }
}

#endif /* CONFIG_BT_MPL */
