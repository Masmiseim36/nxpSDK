/*
 * Common code for wl command line utility
 *
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of 
 * Cypress Semiconductor Corporation. All Rights Reserved.
 * 
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 *
 *
 * $Id: wlu.h 517211 2014-11-24 12:48:18Z pnair $
 */

#ifndef _wlu_h_
#define _wlu_h_

#include "wlu_cmd.h"

extern const char *wlu_av0;

/* parse common option */
extern int wl_option(char ***pargv, char **pifname, int *phelp);
extern void wl_cmd_init(void);
extern void wlu_init(void);

/* print usage */
//extern void wl_cmd_usage(FILE *fid, cmd_t *cmd);
//extern void wl_usage(FILE *fid, cmd_t *port_cmds);
//extern void wl_cmds_usage(FILE *fid, cmd_t *port_cmds);

/* print helpers */
extern void wl_printlasterror(void *wl);
extern void wl_printint(int val);

/* pretty print an SSID */
extern int wl_format_ssid(char* buf, uint8* ssid, int ssid_len);

/* pretty hex print a contiguous buffer */
extern void wl_hexdump(uchar *buf, uint nbytes);

/* check driver version */
extern int wl_check(void *wl);

/* return global ioctl_version */
extern int wl_get_ioctl_version(void);
/* return the address of bufstruct_wlu, global buf */
extern char *wl_get_buf(void);


extern int wl_parse_chanspec_list(char* list_str, chanspec_t *chanspec_list, int chanspec_num);
extern int wl_parse_channel_list(char* list_str, uint16* channel_list, int channel_num);
extern int get_ie_data(uchar *data_str, uchar *ie_data, int len);
extern int get_oui_bytes(uchar *oui_str, uchar *oui);

/* register commands for a module */
extern void wl_module_cmds_register(cmd_t *cmds);

extern cmd_t *wlu_find_cmd(char *name);

/* wluc_module init functions */
extern void wluc_nan_module_init(void);

/* wl functions used by the ndis wl. */
extern void dump_rateset(uint8 *rates, uint count);
extern uint freq2channel(uint freq);
extern int wl_ether_atoe(const char *a, struct ether_addr *n);
extern char *wl_ether_etoa(const struct ether_addr *n);
struct ipv4_addr;   /* forward declaration */
extern int wl_atoip(const char *a, struct ipv4_addr *n);
extern char *wl_iptoa(const struct ipv4_addr *n);
extern cmd_func_t wl_int;
extern cmd_func_t wl_varint;
extern void wl_dump_raw_ie(bcm_tlv_t *ie, uint len);
extern int wl_mk_ie_setbuf(const char *command, uint32 pktflag_ok, char **argv,
    vndr_ie_setbuf_t **buf, int *buf_len);
extern cmd_func_t wl_list_ie;

extern void wl_printlasterror(void *wl);
extern bool wc_cmd_check(const char *cmd);

#if defined (WL_NAN)
extern int wl_nan_do_get_ioctl(void *wl, wl_nan_ioc_t *nanioc, uint16 iocsz);
#endif

#ifdef BCMINTERNAL
extern int wl_iov_names(void *wl, cmd_t *cmd, char **argv);
extern int wl_cmd_help(void *wl, cmd_t *cmd_in, char **argv);
#endif

/* functions for downloading firmware to a device via serial or other transport */
#ifdef SERDOWNLOAD
extern int dhd_init(void *dhd, cmd_t *cmd, char **argv);
extern int dhd_download(void *dhd, cmd_t *cmd, char **argv);
extern int hsic_download(void *dhd, cmd_t *cmd, char **argv);
extern int rwl_download(void *dhd, cmd_t *cmd, char **argv);
#endif /* SERDOWNLOAD */

#ifdef BCMDLL
#ifdef LOCAL
extern FILE *dll_fd;
#else
extern void * dll_fd_out;
extern void * dll_fd_in;
#endif
#undef printf
#undef fprintf
#define printf printf_to_fprintf    /* printf to stdout */
#define fprintf fprintf_to_fprintf  /* fprintf to stderr */
extern void fprintf_to_fprintf(FILE * stderror, const char *fmt, ...);
extern void printf_to_fprintf(const char *fmt, ...);
extern void raw_puts(const char *buf, void *dll_fd_out);
#define fputs(buf, stdout) raw_puts(buf, dll_fd_out)
#endif /* BCMDLL */

#if defined (WL_NAN)
extern int wl_nan_do_get_ioctl(void *wl, wl_nan_ioc_t *nanioc, uint16 iocsz);
#endif

extern int wlu_var_getbuf(void *wl, const char *iovar, void *param, int param_len, void **bufptr);
extern chanspec_t wl_chspec_from_driver(chanspec_t chanspec);
extern chanspec_t wl_chspec_to_driver(chanspec_t chanspec);
extern int wlu_var_getbuf_sm(void *wl, const char *iovar, void *param, int param_len,
    void **bufptr);
extern char* wl_rate_print(char *rate_buf, uint32 rspec);
extern int ARGCNT(char **argv);

#define RAM_SIZE_4325  0x60000
#define RAM_SIZE_4329  0x48000
#define RAM_SIZE_43291 0x60000
#define RAM_SIZE_4330_a1  0x3c000
#define RAM_SIZE_4330_b0  0x48000

/* useful macros */
#ifndef ARRAYSIZE
#define ARRAYSIZE(a)  (sizeof(a)/sizeof(a[0]))
#endif /* ARRAYSIZE */

/* buffer length needed for wl_format_ssid
 * 32 SSID chars, max of 4 chars for each SSID char "\xFF", plus NULL
 */
#ifndef ATE_BUILD
#define SSID_FMT_BUF_LEN (4*32+1)   /* Length for SSID format string */
#endif
#define USAGE_ERROR  -1     /* Error code for Usage */
#define IOCTL_ERROR  -2     /* Error code for Ioctl failure */
#define BAD_PARAM -3 /* Error code for bad params, but don't dump cmd_help */
#define CMD_DEPRECATED -4 /* Commands that are functionally deprecated or don't provide
               * a useful value to a specific OS port of wl
               */

/* integer output format */
#define INT_FMT_DEC 0   /* signed integer */
#define INT_FMT_UINT    1   /* unsigned integer */
#define INT_FMT_HEX 2   /* hexdecimal */

/* command line argument usage */
#define CMD_ERR -1  /* Error for command */
#define CMD_OPT 0   /* a command line option */
#define CMD_WL  1   /* the start of a wl command */

#endif /* _wlu_h_ */
