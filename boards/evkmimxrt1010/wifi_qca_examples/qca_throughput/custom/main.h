// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// - Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
/*
 * $FileName: main.h$
 * $Version : 1.0$
 * $Date    :
 *
 * Comments:
 *   The header file for Atheros wmiconfig tool.
 *
 *END************************************************************************/

#ifndef _main_h_
#define _main_h_

#include <string.h>
#include <stdint.h>

#include <stdlib.h>
//#include "rtcs.h"
//#include "ipcfg.h"
#include "fsl_shell.h"
#include <stdlib.h>
#include "atheros_wifi_api.h"
#include "atheros_stack_offload.h"
#include "qcom_api.h"

#include "qca_structs.h"

#include "throughput.h"

// IMPORTANT: This defines what PRINTF() function to use
//#define PRINTF debug_printf
// IMPORTANT: This is the line terminator to use in PRINTF()
#define NL "\r\n"

#define A_CHANNEL_MISMATCH 11
/* define MACROS required by common code. */
#define ATH_STRCMP(_a, _b) strcmp((char const *)(_a), (const char *)(_b))

#define DEMOCFG_ENABLE_SERIAL_SHELL 1   /* enable shell task for serial console */
#define DEMOCFG_ENABLE_SWITCH_TASK 0    /* enable button sensing task (otherwise keys are polled) */
#define DEMOCFG_ENABLE_AUTO_LOGGING 0   /* enable logging to serial console (or USB drive) */
#define DEMOCFG_ENABLE_USB_FILESYSTEM 0 /* enable USB mass storage */
// E.Y. hide #define DEMOCFG_ENABLE_RTCS            1   /* enable RTCS operation */
#define DEMOCFG_ENABLE_RTCS 0          /* enable RTCS operation */
#define DEMOCFG_ENABLE_FTP_SERVER 0    /* enable ftp server */
#define DEMOCFG_ENABLE_TELNET_SERVER 0 /* enable telnet server */
#define DEMOCFG_ENABLE_KLOG 0          /* enable kernel logging */
#define DEMOCFG_USE_POOLS 0            /* enable external memory pools for USB and RTCS */
#define DEMOCFG_USE_WIFI 1             /* USE WIFI Interface */
#define DEMOCFG_USE_SENSOR 0           /*enable sensor functions*/
/* default addresses for external memory pools and klog */
#if BSP_M52259EVB
#define DEMOCFG_RTCS_POOL_ADDR (uint32_t)(BSP_EXTERNAL_MRAM_RAM_BASE)
#define DEMOCFG_RTCS_POOL_SIZE 0x0000A000
#define DEMOCFG_MFS_POOL_ADDR (uint32_t)(DEMOCFG_RTCS_POOL_ADDR + DEMOCFG_RTCS_POOL_SIZE)
#define DEMOCFG_MFS_POOL_SIZE 0x00002000
#define DEMOCFG_KLOG_ADDR (uint32_t)(DEMOCFG_MFS_POOL_ADDR + DEMOCFG_MFS_POOL_SIZE)
#define DEMOCFG_KLOG_SIZE 4000
#elif DEMOCFG_USE_POOLS
#warning External pools will not be used on this board.
#endif

#if DEMOCFG_ENABLE_RTCS

extern void HVAC_initialize_networking(void);

#ifndef ENET_IPADDR
//   #define ENET_IPADDR  IPADDR(15,15,103,25)
#define ENET_IPADDR IPADDR(192, 168, 1, 90)
//#define ENET_IPADDR  IPADDR(172,16,83,25)
#endif

#ifndef ENET_IPMASK
#define ENET_IPMASK IPADDR(255, 255, 255, 0)
#endif

#ifndef ENET_IPGATEWAY
//  #define ENET_IPGATEWAY  IPADDR(15,15,103,10)
#define ENET_IPGATEWAY IPADDR(192, 168, 1, 1)
#endif
#endif
#if DEMOCFG_USE_WIFI
//   #include "iwcfg.h"

#define DEMOCFG_SSID "iot"
// Possible Values managed or adhoc
#define DEMOCFG_NW_MODE "managed"
#define DEMOCFG_AP_MODE "softap"
// Possible vales
// 1. "wep"
// 2. "wpa"
// 3. "wpa2"
// 4. "none"
#define DEMOCFG_SECURITY "none"
#define DEMOCFG_PASSPHRASE NULL
#define DEMOCFG_WEP_KEY "abcde"
// Possible values 1,2,3,4
#define DEMOCFG_WEP_KEY_INDEX 1
#define DEMOCFG_DEFAULT_DEVICE 0 /*ATHEROS_WIFI_DEFAULT_ENET_DEVICE vivi*/
#endif

#ifndef DEMOCFG_DEFAULT_DEVICE
#define DEMOCFG_DEFAULT_DEVICE BSP_DEFAULT_ENET_DEVICE
#endif

#if DEMOCFG_ENABLE_WEBSERVER
#include "httpd.h"
#endif

//#include <shell.h>

typedef struct
{
    uint8_t ssid[32];
    uint8_t macaddress[6];
    uint16_t channel;
} wps_scan_params;

#define PRINT_HOSTVERSION(ver)                                                                             \
    (ver & 0xF0000000) >> 28, (ver & 0x0F000000) >> 24, (ver & 0x00FC0000) >> 18, (ver & 0x0003FF00) >> 8, \
        (ver & 0x000000FF)

#define PRINT_TARGETVERSION(ver) ver

#define PRINT_FWVERSION(ver)                                                                               \
    (ver & 0xF0000000) >> 28, (ver & 0x0F000000) >> 24, (ver & 0x00FC0000) >> 18, (ver & 0x0003FF00) >> 8, \
        (ver & 0x000000FF)

#define PRINT_ABIVERSION(ver) ver

typedef struct version
{
    uint32_t host_ver;
    uint32_t target_ver;
    uint32_t wlan_ver;
    uint32_t abi_ver;
} version_t;

typedef struct versionstr
{
    uint8_t host_ver[20];
    uint8_t target_ver[20];
    uint8_t wlan_ver[20];
    uint8_t abi_ver[20];
} versionstr_t;

typedef struct wepkey
{
    char key[30];
    uint8_t key_index;
} wepkey_t;

enum wps_mode_e
{
    PUSH_BUTTON = 1,
    PIN
};

#if ENABLE_P2P_MODE
enum p2p_mode_e
{
    CONNECT = 1,
    P2P_FIND,
    JOIN,
    LISTEN,
    CANCEL,
    STOP,
    CONNECT_CLIENT,
    NODE_LIST,
    SET_CONFIG,
    WPS_CONFIG,
    AUTH,
    DISC_REQ,
    SET,
    P2P_INVITE_AUTH,
    LIST_NETWORK,
    P2P_INVITE,
    INV_CONNECT,
    JOIN_PROFILE,
    AP_MODE,
    AP_MODE_PP,
    P2P_ON_OFF,
    P2P_SET_NOA,
    P2P_SET_OPPPS
};
#endif

typedef enum wifimode
{

    MODE_STATION = 0,
    MODE_AP,
    MODE_ADHOC,
    MODE_MAXIMUM

} wifimode_t;

typedef struct
{
    union
    {
        struct
        {
            uint8_t hidden_ssid;
        } wmi_ap_hidden_ssid_cmd;

        struct
        {
            uint32_t period;
        } wmi_ap_conn_inact_cmd;

        struct
        {
            uint16_t beaconInterval;
        } wmi_beacon_int_cmd;

        struct
        {
            uint8_t dtimval;
        } wmi_ap_dtim_cmd;

        struct
        {
            uint8_t ps_val[2];
        } wmi_ap_ps_buf_cmd;
    } u;
} AP_CFG_CMD;

#include "atheros_wifi_api.h"

enum ath_private_ioctl_cmd
{
    ATH_REG_QUERY = ATH_CMD_LAST,
    ATH_MEM_QUERY
};

typedef struct
{
    uint32_t address;
    uint32_t value;
    uint32_t mask;
    uint32_t size;
    uint32_t operation;
} ATH_REGQUERY;

typedef struct
{
    uint32_t literals;
    uint32_t rodata;
    uint32_t data;
    uint32_t bss;
    uint32_t text;
    uint32_t heap;
} ATH_MEMQUERY;

typedef struct
{
    uint8_t wps_in_progress;
    uint8_t connect_flag;
    uint8_t wps_pbc_interrupt;
    ATH_NETPARAMS netparams;
} wps_context_t;

#define ATH_REG_OP_READ (1)
#define ATH_REG_OP_WRITE (2)
#define ATH_REG_OP_RMW (3)
#define MAX_ACK_RETRY (20) // Max number of times ACK issent to Peer with stats

#define MIN_WEP_KEY_SIZE 10
#define MAX_WEP_KEY_SIZE 26
#define MAX_PASSPHRASE_SIZE 64
#define MAX_NUM_WEP_KEYS 4
#define MAX_WPS_PIN_SIZE 32

#define DRIVER_UP (0)
#define DRIVER_DOWN (1)
#define READ_STATUS (2)

/* Security modes */
#define SEC_MODE_OPEN (0)
#define SEC_MODE_WEP (1)
#define SEC_MODE_WPA (2)

#define getByte(n, data) ((int)((data >> (8 * n)) & 0xFF))

#define IS_DRIVER_READY is_driver_initialized()
#define HANDLE_IOCTL(inout_param) handle_ioctl(inout_param)
#define ATH_COMMIT(device) ath_commit(device)
#define ATH_SET_MODE(device, mode) ath_set_mode(device, mode)
#define ATH_SET_ESSID(device, essid) ath_set_essid(device, essid)
#define ATH_SET_SEC_TYPE(device, sectype) ath_set_sec_type(device, sectype)
#define ATH_SET_PASSPHRASE(device, passphrase) ath_set_passphrase(device, passphrase)
#define ATH_GET_ESSID(device, essid) ath_get_essid(device, essid)
#define ATH_NET_CONNECT(pNetparams) NetConnect(pNetparams)
//#define SET_POWER_MODE(pwr_mode)      set_power_mode(pwr_mode)
#define TURN_LED_ON
#define TURN_LED_OFF
#define TEST_FLASH test_flash()
#define ATH_MEM_QUERY(argc, argv) ath_mem_query(argc, argv)
#define ATH_REG_QUERY(argc, argv) ath_reg_query(argc, argv)

/********************** Function Prototypes **************************************/

A_STATUS is_driver_initialized(void);
void SystemReset(void);
int32_t ext_scan(void);
A_STATUS handle_ioctl(ATH_IOCTL_PARAM_STRUCT *inout_param);
// int32_t set_power_mode(char* pwr_mode);
int32_t test_flash(void);
int32_t ath_mem_query(int32_t argc, char *argv[]);
int32_t ath_reg_query(int32_t argc, char *argv[]);
int32_t wmi_set_scan(uint32_t dev_num, void *start_scan);
int32_t ath_driver_state(int32_t argc, char *argv[]);
A_STATUS ath_set_essid(uint32_t device, char *essid);
A_STATUS ath_set_sec_type(uint32_t device, char *sec_type);
A_STATUS ath_set_passphrase(uint32_t device, char *passphrase);
A_STATUS ath_get_essid(uint32_t device, char *essid);
int32_t get_power_mode(char *pwr_mode);
A_STATUS set_power_mode(bool pwr_mode, uint8_t pwr_module);
A_STATUS ath_commit(uint32_t device);
int32_t set_callback(void);
A_STATUS ath_set_mode(uint32_t device, char *mode);
uint8_t driver_status(uint8_t val);
int32_t CompleteWPS(ATH_NETPARAMS *pNetparams, uint8_t block, wifimode_t mode_flag);
int32_t NetConnect(ATH_NETPARAMS *pNetparams);
int handle_pmk(char *pmk);
A_STATUS is_driver_initialized(void);
A_STATUS handle_ioctl(ATH_IOCTL_PARAM_STRUCT *inout_param);
void atheros_driver_setup(void);
extern int32_t wps_query(uint8_t block);
void resetTarget(void);
int32_t set_callback(void);
int32_t set_wep_keys(uint32_t dev_num,
                     char *wep_key1,
                     char *wep_key2,
                     char *wep_key3,
                     char *wep_key4,
                     uint32_t key_len,
                     uint32_t key_index);

#if ENABLE_STACK_OFFLOAD
char *inet_ntoa(uint32_t addr, char *res_str);
char *inet6_ntoa(char *addr, char *str);
void ath_udp_echo(int32_t argc, char_ptr argv[]);
char *print_ip6(IP6_ADDR_T *addr, char *str);
int32_t ipconfig_static(int32_t argc, char *argv[]);
int32_t ipconfig_dhcp(int32_t argc, char *argv[]);
#endif
uint32_t mystrtoul(const char *arg, const char *endptr, int base);
void application_frame_cb(A_VOID *ptr);
int32_t get_tx_status(void);
int32_t StartWPS(ATH_NETPARAMS *pNetparams, uint8_t use_pinmode, wps_scan_params *wpsScan_p);
_qca_handle get_active_device_handle(void);
extern int32_t get_active_device(void);
int32_t get_dev_mode(void);
// void app_time_delay(int msec);
int32_t ip_get_addr_for_host_name(char *hostname, uint32_t *v4addr, IP6_ADDR_T *ip6addr, uint32_t domain);
void clear_kf_profile(void);
void output_kf_pfm(void);

// Shell functions
int ipconfig_query(shell_handle_t context, int argc, char **argv);

int32_t wmiconfig_handler(int32_t argc, char *argv[]);
int32_t wmi_iwconfig(int32_t argc, char *argv[]);

/*********************** Extern Variables ******************************************/
extern uint8_t access_category;
extern uint32_t wifi_connected_flag; // Flag to indicate WIFI connectivity status
extern uint8_t bench_quit;
extern wps_context_t wps_context;
extern int active_device;

// Shell command helper function
int RunInWmiTaskThread(shell_handle_t context, int argc, char **argv);

// These two pointers are of type ENET_CONTEXT_STRUCT_PTR
extern _qca_handle handle;
extern _qca_handle p_Driver_Cxt[2];

#endif
