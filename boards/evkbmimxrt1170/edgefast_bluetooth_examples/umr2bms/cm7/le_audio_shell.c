/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/types.h>
#include <stdio.h>
#include <stddef.h>
#include <errno/errno.h>
#include <toolchain.h>
#include <porting.h>
#include "fsl_debug_console.h"
#include <bluetooth/audio/audio.h>

#include "fsl_shell.h"

#include "broadcast_media_sender.h"
#include "le_audio_shell.h"

shell_handle_t s_shellHandle;
SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
extern serial_handle_t g_serialHandle;

extern int select_lc3_preset(char *preset_name);
extern void print_all_preset(int sample_rate);

static shell_status_t lc3_preset_list(shell_handle_t shellHandle, int32_t argc, char **argv)
{
	print_all_preset(0);

	return kStatus_SHELL_Success;
}

static shell_status_t lc3_preset(shell_handle_t shellHandle, int32_t argc, char **argv)
{
	int err;
	char *name = argv[1];

	err = select_lc3_preset(name);
	if(err)
	{
		PRINTF("set lc3 preset fail %d\r\n", err);
		return kStatus_SHELL_Error;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t bms_enable(shell_handle_t shellHandle, int32_t argc, char **argv)
{
	int err;

	err = bms_init();
	if (err < 0) {
		PRINTF("Fail to initialize BMS\r\n");
		return kStatus_SHELL_Error;
	}

	err = bms_start();
	if (err < 0) {
		PRINTF("Fail to enable BMS\r\n");
		bms_deinit();
		return kStatus_SHELL_Error;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t bms_disable(shell_handle_t shellHandle, int32_t argc, char **argv)
{
	int err;

	err = bms_stop();
	if (err < 0) {
		PRINTF("Fail to disable BMS\r\n");
		return kStatus_SHELL_Error;
	}

	err = bms_deinit();
	if (err < 0) {
		PRINTF("Fail to de-initialize BMS\r\n");
		return kStatus_SHELL_Error;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t bms_streaming(shell_handle_t shellHandle, int32_t argc, char **argv)
{
	int err = 0;

	if (!strcmp(argv[1], "on")) {
		err = bms_streaming_simulate_start();
	} else if (!strcmp(argv[1], "off")) {
		err = bms_streaming_simulate_stop();
	} else {
		PRINTF("Invalid argument\r\n");
		return kStatus_SHELL_Error;
	}

	if (err < 0) {
		return kStatus_SHELL_Error;
	}

	return kStatus_SHELL_Success;
}

static shell_status_t set_broadcast_code(shell_handle_t shellHandle, int32_t argc, char **argv)
{
	char *format;
	char *input;
	uint8_t broadcast_code[BT_AUDIO_BROADCAST_CODE_SIZE];

	format = argv[1];
	input = argv[2];

	int len = strlen(input);

	if(0 == strcmp(format, "str"))
	{
		if(len <= BT_AUDIO_BROADCAST_CODE_SIZE)
		{
			memcpy(broadcast_code, input, len);
		}
		else
		{
			return kStatus_SHELL_Error;
		}

		if(config_broadcast_code(broadcast_code, len))
		{
			return kStatus_SHELL_Error;
		}
	}
	else if(0 == strcmp(format, "hex"))
	{
		if((len % 2 == 0) && (len <= 2 * BT_AUDIO_BROADCAST_CODE_SIZE))
		{
			int ret = hex2bin(input, len, broadcast_code, BT_AUDIO_BROADCAST_CODE_SIZE);
			if(ret != len / 2)
			{
				return kStatus_SHELL_Error;
			}
		}
		else
		{
			return kStatus_SHELL_Error;
		}

		if(config_broadcast_code(broadcast_code, len / 2))
		{
			return kStatus_SHELL_Error;
		}
	}
	else
	{
		return kStatus_SHELL_Error;
	}

	return kStatus_SHELL_Success;
}

SHELL_COMMAND_DEFINE(lc3_preset_list, "lc3_preset_list\r\n",                  lc3_preset_list, 0);
SHELL_COMMAND_DEFINE(lc3_preset,      "lc3_preset <name>\r\n",                lc3_preset,      1);
SHELL_COMMAND_DEFINE(bms_enable,      "bms_enable\r\n",                       bms_enable,      0);
SHELL_COMMAND_DEFINE(bms_disable,     "bms_disable\r\n",                      bms_disable,     0);
SHELL_COMMAND_DEFINE(bms_streaming,   "Simulate BMS streaming: bms_streaming <on/off>\r\n", bms_streaming, 1);
SHELL_COMMAND_DEFINE(set_broadcast_code, "set_broadcast_code [str,hex] [data]\r\r\n", set_broadcast_code, 2);

void le_audio_shell_init(void)
{
	/* Init SHELL */
	if (s_shellHandle) {
		return;
	}

	s_shellHandle = &s_shellHandleBuffer[0];
	SHELL_Init(s_shellHandle, g_serialHandle, "UMR2BMS>> ");
	SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(lc3_preset_list));
	SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(lc3_preset));
	SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(bms_enable));
	SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(bms_disable));
	SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(bms_streaming));
	SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(set_broadcast_code));

	SHELL_Printf(s_shellHandle, "\r\nUMR To BMS.\r\r\n");
}
