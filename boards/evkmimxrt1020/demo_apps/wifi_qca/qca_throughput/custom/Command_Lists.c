//------------------------------------------------------------------------------
// Copyright (c) 2011 Qualcomm Atheros, Inc.
// All Rights Reserved.
// Qualcomm Atheros Confidential and Proprietary.
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is
// hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
// INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
// USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
/*
* $FileName: Command_Lists.c$
* $Version : 3.5.16.0$
* $Date    : Dec-2-2009$
*
* Comments:
*
*
*
*END************************************************************************/

#include "main.h"
#include "throughput.h"
#include "atheros_stack_offload.h"
#include "fsl_shell.h"

#if ENABLE_STACK_OFFLOAD
#if ENABLE_HTTPS_SERVER || ENABLE_HTTPS_CLIENT
#include <https.h>
#endif
#endif
#if !ENABLE_STACK_OFFLOAD
#endif

// Internal shell functions
extern int cmd_exit(p_shell_context_t context, int argc, char **argv);
extern int cmd_help(p_shell_context_t context, int argc, char **argv);

// QCA shell functions
extern int worker_cmd_handler(p_shell_context_t context, int argc, char **argv);
extern int worker_cmd_quit(p_shell_context_t context, int argc, char **argv);
extern int wmi_ping(p_shell_context_t context, int argc, char **argv);
extern int wmi_ping6(p_shell_context_t context, int argc, char **argv);
extern int print_perf(p_shell_context_t context, int argc, char **argv);
extern int ipconfig_query(p_shell_context_t context, int argc, char **argv);
extern int sensor_handle(p_shell_context_t context, int argc, char **argv);

// Additional shell functions
extern int cmd_osinfo(p_shell_context_t context, int argc, char **argv);

const shell_command_context_t shell_commands[] = {
    //    { "osinfo",               "\r\n\"osinfo\"\n\r print information of OS\r\n", cmd_osinfo, 0 },
    {"benchtx",
     "\r\n\"benchtx\": Run the transmit traffic test\r\n"
     "Format: benchtx <remoteIP> <remotePort> <protocol> <packetSize> <testMode> <time|packets> <delay>\r\n"
     "Params:\r\n"
     "   <remoteIP> fbench IP address\r\n"
     "   <remotePort> fbench listening port\r\n"
     "   <protocol> tcp|udp\r\n"
     "   <packetSize> packet size in Bytes\r\n"
     "   <testMode> '0' means time test, '1' means number of packets\r\n"
     "   <time|packets> time in seconds or number of packets\r\n"
     "   <delay> delay in ms between packets\r\n",
     (cmd_function_t)worker_cmd_handler, SHELL_OPTIONAL_PARAMS},
    {"benchrx",
     "\r\n\"benchrx\": Run the receive traffic test\r\n"
     "Format: benchrx <protocol> <localPort> <multicastIP> <localIP>\r\n"
     "Params:\r\n"
     "   <protocol> tcp|udp\r\n"
     "   <localPort> listening port\r\n"
     "   <multicastIP> optional, IP address of multicast group\r\n"
     "   <localIP> local IP address of interface\r\n",
     (cmd_function_t)worker_cmd_handler, SHELL_OPTIONAL_PARAMS},
#if READ_HOST_MEMORY
//    {"hostmemmap", "\r\n\"hostmemmap\"\r\n", worker_cmd_handler, SHELL_OPTIONAL_PARAMS},
#endif
#if (ENABLE_STACK_OFFLOAD && MULTI_SOCKET_SUPPORT)
//    {"benchrx_multi_socket", "\r\n\"benchrx_multi_socket\"\r\n", worker_cmd_handler, SHELL_OPTIONAL_PARAMS},
#endif
//    { "benchquit",            "\r\n\"benchquit\"\r\n", worker_cmd_quit, 0 },
//    {"benchmode", "\r\n\"benchmode\"\r\n", worker_cmd_handler, SHELL_OPTIONAL_PARAMS},
//    { "perf",                 "\r\n\"perf\"\r\n", print_perf, 0 },
#if (ENABLE_STACK_OFFLOAD)
//    { "ipconfig",             "\r\n\"ipconfig\"\r\n", ipconfig_query, 0 },
#else
//    { "ipconfig",             "\r\n\"ipconfig\"\r\n", Shell_ipconfig, 0 },
#endif
#if DEMOCFG_USE_WIFI
    {"iwconfig", "\r\n\"iwconfig\"\r\n", (cmd_function_t)worker_cmd_handler, SHELL_OPTIONAL_PARAMS}, // wmi_iwconfig },
    {"wmiconfig",
     "\r\n\"wmiconfig\": Run the configuration tool\r\n"
     "Full command description is available by \"wmiconfig --help\"\r\n",
     (cmd_function_t)worker_cmd_handler, SHELL_OPTIONAL_PARAMS},
#endif
#if DEMOCFG_USE_SENSOR
//   { "sensor",                "\r\n\"sensor\"\r\n", sensor_handle, 0 },
#endif
//   { "ping",                  "\r\n\"ping\"\r\n", wmi_ping, 0 },
//   { "ping6",                 "\r\n\"ping6\"\r\n", wmi_ping6, 0 },
#if ENABLE_STACK_OFFLOAD
#if ENABLE_SSL
//   { "getcert",               "\r\n\"getcert\"\r\n", worker_cmd_handler, 0 },
#if ENABLE_HTTPS_SERVER
//   { "httpss",                "\r\n\"httpss\"\r\n", worker_cmd_handler, 0 },
#endif
#if ENABLE_HTTPS_CLIENT
//   { "httpsc",                "\r\n\"httpsc\"\r\n", worker_cmd_handler, 0 },
#endif
#endif
#endif
    {NULL, NULL, NULL, 0}};

/* EOF */
