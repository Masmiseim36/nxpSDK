/** @file
 *  @brief Bluetooth shell functions
 *
 *  This is not to be included by the application.
 */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BT_H
#define __BT_H

#include "stdarg.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

#define SHELL_CMD_ARG(command, cmdList, descriptor, func, minarg, additonarg)                    \
        SHELL_COMMAND_EXDEFINE_ANONYMOUS(command,                                                \
                                         cmdList,                                                \
                                         "\"" STRINGIFY(command) "\"" ": " STRINGIFY(command) " " descriptor "\r\n", \
                                         func,                                                   \
                                         minarg - 1,                                             \
                                         additonarg + minarg - 1                                 \
                                        )
#define SHELL_CMD_REGISTER(command, cmdList, descriptor, func, minarg, additonarg)               \
        SHELL_COMMAND_EXDEFINE(command,                                                          \
                                         cmdList,                                                \
                                         "\"" STRINGIFY(command) "\"" ": " STRINGIFY(command) " " descriptor "\r\n", \
                                         func,                                                   \
                                         minarg - 1,                                             \
                                         additonarg + minarg - 1                                 \
                                        )
#define SHELL_SUBCMD_SET_END NULL
#define SHELL_STATIC_SUBCMD_SET_CREATE(cmds, ...) \
        static shell_command_t *cmds[] = {    \
            __VA_ARGS__                      \
            };
extern shell_handle_t ctx_shell;
extern struct bt_conn *default_conn;

#define shell_dump(shell, fmt, ...)  do { \
      (void)shell;PRINTF( fmt, ##__VA_ARGS__);}while(0)
#define shell_print(shell, fmt, ...) do { \
      (void)shell;PRINTF( fmt "\r\n", ##__VA_ARGS__);}while(0)
#define shell_error(shell, fmt, ...) do { \
      (void)shell;PRINTF( fmt "\r\n", ##__VA_ARGS__);}while(0)
#define shell_help(x)

void conn_addr_str(struct bt_conn *conn, char *addr, size_t len);
void shell_hexdump(shell_handle_t shell, const uint8_t *data, size_t len);
unsigned long shell_strtoul(const char *str, int base, int *err);
long shell_strtol(const char *str, int base, int *err);

void bt_CommandInit(shell_handle_t shell);

void bt_ShellGattInit(shell_handle_t shell);
void bt_ShellL2capInit(shell_handle_t shell);
void bt_ShellBrEdrInit(shell_handle_t shell);
void bt_ShellRfcommInit(shell_handle_t shell);
#if (defined(CONFIG_BT_A2DP) && (CONFIG_BT_A2DP > 0))
void bt_ShellA2dpInit(shell_handle_t shell);
#endif
#if (defined(CONFIG_BT_RF_TEST_MODE) && (CONFIG_BT_RF_TEST_MODE > 0))
void bt_ShellTestModeInit(shell_handle_t shell);
#endif
#if (defined(CONFIG_BT_AVRCP) && (CONFIG_BT_AVRCP > 0))
void bt_ShellAvrcpInit(shell_handle_t shell);
#endif

#endif /* __BT_H */
