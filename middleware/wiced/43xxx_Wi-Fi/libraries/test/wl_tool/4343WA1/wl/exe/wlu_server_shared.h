/*
 * wl server declarations
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
 * $Id: wlu_server_shared.h 287128 2011-09-30 04:27:53Z estucki $
 */

#ifndef _wlu_server_shared_h
#define _wlu_server_shared_h

#if defined(LINUX) || defined(SYMBIAN) || defined(TARGET_wiced)
	extern int wl_ioctl(void *wl, int cmd, void *buf, int len, bool set);
#endif

#if defined (LINUX)
	extern int dhd_ioctl(void *dhd, int cmd, void *buf, int len, bool set);
#endif

#ifdef RWLASD
/* streams' buffers */
BYTE *xcCmdBuf = NULL, *parmsVal = NULL;
BYTE *trafficBuf = NULL, *respBuf = NULL;
struct timeval *toutvalp = NULL;
#endif

#define POLLING_TIME      			200
#define DONGLE_TX_FRAME_SIZE   		1024
#define MESSAGE_LENGTH				1024
#define MAX_SHELL_FILE_LENGTH       50
#define MAX_IOVAR				10000
int remote_type = NO_REMOTE;
rem_ioctl_t *g_rem_ptr;

#if defined(LINUX) || defined(SYMBIAN) || defined(TARGETOS_nucleus)
extern int wl_ioctl(void *wl, int cmd, void *buf, int len, bool set);
#endif

/* Function prototypes from shellpoc_linux.c/shell_ce.c */
extern int rwl_create_dir(void);
extern int remote_shell_execute(char *buf_ptr, void *wl);
extern int remote_shell_get_resp(char* shell_fname, void *wl);
extern void rwl_wifi_find_server_response(void *wl, dot11_action_wifi_vendor_specific_t *rec_frame);
extern dot11_action_wifi_vendor_specific_t *rwl_wifi_allocate_actionframe(void);

/* Common code for serial and wifi */
#if defined (RWL_DONGLE) || defined (RWL_WIFI) || defined(RWL_SERIAL)
typedef struct rem_packet {
	rem_ioctl_t rem_cdc;
	uchar message[MESSAGE_LENGTH];
} rem_packet_t;
#define REMOTE_PACKET_SIZE sizeof(rem_packet_t)

rem_packet_t *g_rem_pkt_ptr;
rem_packet_t g_rem_pkt;
#endif

static struct ether_addr rwlea;

static union {
	uchar bufdata[WLC_IOCTL_MAXLEN];
	uint32 alignme;
} bufstruct_wlu;
static uchar* rwl_buf = (uchar*) &bufstruct_wlu.bufdata;
extern int need_speedy_response;

#endif /* _wlu_server_shared_h_ */
