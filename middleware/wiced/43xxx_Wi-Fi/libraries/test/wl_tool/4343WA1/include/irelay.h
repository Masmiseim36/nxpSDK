/*
 * Windows Broadcom relay device driver interface.
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
 * $Id: irelay.h 514055 2014-11-08 18:42:49Z sudhirbs $
 */

/* Win9x used FILE_READ_ACCESS instead of FILE_READ_DATA */
#ifndef FILE_READ_DATA
#define FILE_READ_DATA  FILE_READ_ACCESS
#define FILE_WRITE_DATA  FILE_WRITE_ACCESS
# endif

#ifndef IOCTL_NDIS_QUERY_GLOBAL_STATS
#    define _NDIS_CONTROL_CODE(request, method) \
		CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, request, method, FILE_ANY_ACCESS)

#    define IOCTL_NDIS_QUERY_GLOBAL_STATS   _NDIS_CONTROL_CODE(0, METHOD_OUT_DIRECT)
#endif

#define IOCTL_OID_RELAY		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, \
					 FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_PKT_TX_RELAY	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, \
					 FILE_WRITE_DATA)
#define IOCTL_PKT_RX_RELAY	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, \
					 FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_LIST		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805, METHOD_BUFFERED, \
					 FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_XLIST		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x806, METHOD_BUFFERED, \
					 FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_VERSION		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x807, METHOD_BUFFERED, \
					 FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_BIND		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x808, METHOD_BUFFERED, \
					 FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_UNBIND		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x809, METHOD_BUFFERED, \
					 FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_IR_DUMP		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80A, METHOD_BUFFERED, \
					 FILE_WRITE_DATA | FILE_READ_DATA)

#pragma pack(push, 1)

/* structure definitions for sending packets via IOCTL_PKT_TX_RELAY */

typedef struct {
	ULONG	InstanceID;
	CHAR	Buffer[];
} RxRequest, *PRxRequest;

/* structure definitions for sending packets via IOCTL_PKT_TX_RELAY */

typedef struct {
	ULONG	InstanceID;
	ULONG	Copies;
	CHAR	Buffer[];
} TxRequest, *PTxRequest;

/* structure definitions for relaying OIDs through via IOCTL_OID_RELAY */

typedef struct {
	ULONG	OID;
	ULONG	IsQuery;
	ULONG	BufferLength;
	ULONG	Status;
} RelayHeader, *PRelayHeader;

typedef struct {
	RelayHeader rh;
	UCHAR Buffer[];
} RelayQuery, *PRelayQuery;

typedef struct {
	RelayHeader rh;
	ULONG Cookie;
	UCHAR Buffer[];
} RelaySet, *PRelaySet;

/* structure definitions for sending packets via IOCTL_BIND */

typedef struct {
    CHAR  name[80];
} BindRequest, *PBindRequest;


/* Structure for passing generic OIDs through irelay.  This is the
 * same as a normal set but it does not have a cookie.
 */
typedef RelayQuery RelayGenSet, *PRelayGenSet;

/* structure definitions for relaying OIDs through via IOCTL_VERSION */

typedef struct {
    ULONG   VersionMS;        /* e.g. 0x00030075 = "3.75" */
    ULONG   VersionLS;        /* e.g. 0x00000031 = "0.31" */
} VersionResponse, *PVersionResponse;

/* Combined generic/custom query/set relay oid structure. */

typedef struct _IRelay {
	RelayHeader rh;
	union {
		struct {
			ULONG Cookie;
			UCHAR *Buffer;
		} EpiOid;
		struct {
			UCHAR *Buffer;
		} GenOid;
		UCHAR *Buffer;
	};
} IRELAY, *PIRELAY;

#pragma pack(pop)
