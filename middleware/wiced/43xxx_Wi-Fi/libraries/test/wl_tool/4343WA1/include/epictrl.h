/*
 * Windows/NDIS Broadcom HNBU control driver API.
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
 * $Id: epictrl.h 514055 2014-11-08 18:42:49Z sudhirbs $
 */

#ifndef _EPICTRL_H_
#define _EPICTRL_H_

#include "typedefs.h"
#include "proto/ethernet.h"

/* The following ifdef block is the standard way of creating macros
 * which make exporting from a DLL simpler. All files within this DLL
 * are compiled with the EPICTRL_EXPORTS symbol defined on the
 * command line. this symbol should not be defined on any project that
 * uses this DLL. This way any other project whose source files
 * include this file see EPICTRL_API functions as being imported from
 * a DLL, where as this DLL sees symbols defined with this macro as
 * being exported.
 */

#if defined(BUILD_EPICTRL_DLL)
#       define EPICTRL_API __declspec(dllexport)
#elif defined(USE_EPICTRL_DLL)
#       define EPICTRL_API __declspec(dllimport)
#else
#       define EPICTRL_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_EPICTRL_OVERHEAD	512

/* definitions of adapter types used in the type field below */
typedef enum {
	IR_UNKNOWN = -1,
	IR_802_3 = 0x0,
	IR_ILINE,
	IR_ENET,
	IR_CODEC,
	IR_WIRELESS,
	IR_WWAN,
	IR_USB,
	IR_LOOPBACK,
	IR_VIRTUAL_WIRELESS,
	IR_WIMAX,

	/* Add new types above this line and keep this one at the end!! */
	IR_MAXTYPE
} IRTYPE, *PIRTYPE;

#ifdef NEED_IR_TYPES
	static PCHAR ir_typestrings[] = {
	    "802.3",
	    "iline",
	    "enet",
	    "codec",
	    "wireless",
	    "wwan",
	    "usb",
	    "loopback",
	    "virtualwireless",
	    "wimax"
	};
	static PCHAR ir_prefixstrings[] = {
	    "??",
	    "il",
	    "et",
	    "co",
	    "wl",
	    "wn",
	    "ub",
	    "lp",
	    "vw",
	    "wx"
	};
#endif /* NEED_IR_TYPES */

#define IR_BRAND_MAX 10
#define MAX_ADAPTERS 10

typedef struct _ADAPTER {
	union {
		TCHAR			regkey[10];
		TCHAR			shortname[10];
	};
	union {
		TCHAR			adaptername[80];
		TCHAR			name[80];
	};
	TCHAR			wminame[256];
	TCHAR			enumerator[20];
	BYTE			macaddr[6];
	TCHAR			description[80];
	TCHAR			componentId[64];
	TCHAR			brand[IR_BRAND_MAX];
	DWORD			instance;
	IRTYPE			type;
	BOOL			valid;
	BOOL			bvirtual;
} ADAPTER, *PADAPTER;

typedef DWORD WINERR;

EPICTRL_API WINERR ir_init(HANDLE *);
EPICTRL_API WINERR ir_adapter_list(HANDLE, PADAPTER, PDWORD);
EPICTRL_API WINERR ir_exit(HANDLE);
EPICTRL_API WINERR ir_bind(HANDLE m_dh, LPCTSTR DeviceName);
EPICTRL_API WINERR ir_unbind(HANDLE m_dh);
EPICTRL_API WINERR ir_queryinformation(HANDLE m_dh, ULONG oid, PBYTE inbuf, PDWORD inlen);
EPICTRL_API WINERR ir_setinformation(HANDLE m_dh, ULONG oid, PBYTE inbuf, PDWORD inlen);
EPICTRL_API WINERR ir_adapter_reinitialize(LPCTSTR DeviceName);
EPICTRL_API void ir_usewmiset(BOOL useWMI);
EPICTRL_API WINERR ir_adapter_disable(LPCTSTR DeviceName);
EPICTRL_API WINERR ir_adapter_enable(LPCTSTR componentId);

typedef struct _dll_private {
    HANDLE handle;
    PVOID   param;
	PVOID   reserved; /* epictrl internal use only. DO NOT USE! */
} dll_private_t;


#ifdef __cplusplus
}
#endif

#endif /* _EPICTRL_H_ */
