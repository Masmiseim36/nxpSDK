/*
 * OS independent declarations
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
 * $Id: wlu_pipe.h 287128 2011-09-30 04:27:53Z estucki $
 */
#ifndef _wlu_pipe_h
#define _wlu_pipe_h

/* Function prototypes defined in wlu_pipe.c */

/* Macros to access globals */
extern char *g_rwl_device_name_serial;
#define rwl_get_serial_port_name()		(g_rwl_device_name_serial)
#define rwl_set_serial_port_name(name) 	(g_rwl_device_name_serial = name)
extern char *g_rwl_buf_mac;
#define rwl_get_wifi_mac()				(g_rwl_buf_mac)

extern int remote_CDC_tx(void *wl, uint cmd, uchar *buf, uint buf_len,
uint data_len, uint flags, int debug);
extern rem_ioctl_t *remote_CDC_rx_hdr(void *remote, int debug);
extern int remote_CDC_rx(void *wl, rem_ioctl_t *rem_ptr, uchar *readbuf, uint buflen, int debug);

extern void* rwl_open_pipe(int remote_type, char *port, int ReadTotalTimeout, int debug);
extern int rwl_close_pipe(int remote_type, void* hndle);

#ifdef RWL_SOCKET
extern int rwl_sockconnect(int SockDes, struct sockaddr *servAddr, int size);
#endif /* RWL_SOCKET */

extern int remote_CDC_DATA_wifi_rx_frag(void *wl, rem_ioctl_t *rem_ptr, uint input_len,
void *input, bool shell);
extern int remote_CDC_DATA_wifi_rx(void *wl, struct dot11_action_wifi_vendor_specific *rec_frame);
extern int
remote_CDC_wifi_tx(void *wl, uint cmd, uchar *buf, uint buf_len, uint data_len, uint flags);
extern void rwl_wifi_server_response(void *wl, struct dot11_action_wifi_vendor_specific *rec_frame);
extern int rwl_find_remote_wifi_server(void *wl, char *id);
extern void rwl_wifi_purge_actionframes(void *wl);
extern void rwl_wifi_swap_remote_type(int flag);
extern void remote_wifi_ser_init_cmds(void *wl);
#endif /* _wlu_pipe_h */
