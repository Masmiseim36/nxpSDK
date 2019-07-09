/*
 * Monitor Mode routines.
 * This header file housing the define and function use by DHD
 *
 * Copyright 2019, Cypress Semiconductor Corporation or a subsidiary of 
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
 * <<Broadcom-WL-IPTag/Proprietary:>>
 *
 * $Id: bcmwifi_monitor.h 512698 2016-02-11 13:12:15Z danielh $
 */
#ifndef _BCMWIFI_MONITOR_H_
#define _BCMWIFI_MONITOR_H_

#include <proto/monitor.h>
#include <bcmwifi_radiotap.h>

#define MAX_RADIOTAP_SIZE	sizeof(wl_radiotap_vht_t)
#define MAX_MON_PKT_SIZE	(4096 + MAX_RADIOTAP_SIZE)

typedef struct monitor_info monitor_info_t;

typedef struct monitor_pkt_ts {
	union {
		uint32	ts_low; /* time stamp low 32 bits */
		uint32	reserved; /* If timestamp not used */
	};
	union {
		uint32  ts_high; /* time stamp high 28 bits */
		union {
			uint32  ts_high_ext :28; /* time stamp high 28 bits */
			uint32  clk_id_ext :3; /* clock ID source  */
			uint32  phase :1; /* Phase bit */
			uint32	marker_ext;
		};
	};
} monitor_pkt_ts_t;

typedef struct monitor_pkt_info {
	uint32	marker;
	/* timestamp */
	monitor_pkt_ts_t ts;
} monitor_pkt_info_t;


extern uint16 bcmwifi_monitor_create(monitor_info_t**);
extern void bcmwifi_monitor_delete(monitor_info_t* info);
extern uint16 bcmwifi_monitor(monitor_info_t* info,
	monitor_pkt_info_t* pkt_info, void *pdata, uint16 len, void* pout, uint16* offset);
extern uint16 wl_rxsts_to_rtap(struct wl_rxsts* rxsts, void *pdata, uint16 len, void* pout);

#endif /* _BCMWIFI_MONITOR_H_ */
