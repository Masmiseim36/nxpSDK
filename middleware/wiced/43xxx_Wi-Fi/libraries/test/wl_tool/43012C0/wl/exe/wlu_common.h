/*
 * Common code for wl routines
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
 * $Id: wlu_common.h 649191 2016-07-15 06:30:56Z ykodiche $
 */
#include <wlioctl.h>
#include <bcmutils.h>

#if	defined(_CFE_) /* router boot loader */
#include <lib_types.h>
#include <lib_string.h>
#include <lib_printf.h>
#include <lib_malloc.h>
#include <cfe_error.h>
#elif	defined(__IOPOS__)  /* XXX Nintendo Revolution */
#include <typedefs.h>
#include <bcmstdlib.h>
#include <bcmutils.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#endif /* defined(_CFE_) */

#if defined(EDK_RELEASE_VERSION) || (EDK_RELEASE_VERSION >= 0x00020000)
#define strtoul(nptr, endptr, base) bcm_strtoul((nptr), (endptr), (base))
#define strtol(nptr, endptr, base)  (long)bcm_strtoul((nptr), (endptr), (base))
#endif /* defined(EDK_RELEASE_VERSION) || (EDK_RELEASE_VERSION >= 0x00020000) */

#if defined (__NetBSD__) || defined(linux) || defined(MACOSX) || defined(EFI)

#define stricmp strcasecmp
#define strnicmp strncasecmp

#elif	defined(_CRT_SECURE_NO_DEPRECATE) /* MSVC common runtime */

#define stricmp _stricmp
#define strnicmp _strnicmp

#elif	defined(DONGLEBUILD)

#define stricmp strcmp
#define strnicmp strncmp

#elif	defined(_CFE_) /* router boot loader */

#include <bcmutils.h>
#include <osl.h>
#define isalnum(c) bcm_isalnum(c)
#define isalpha(c) bcm_isalpha(c)
#define iscntrl(c) bcm_iscntrl(c)
#define isdigit(c) bcm_isdigit(c)
#define isgraph(c) bcm_isgraph(c)
#define islower(c) bcm_islower(c)
#define isprint(c) bcm_isprint(c)
#define ispunct(c) bcm_ispunct(c)
#define isspace(c) bcm_isspace(c)
#define isupper(c) bcm_isupper(c)
#define isxdigit(c) bcm_isxdigit(c)
#define stricmp(s1, s2) lib_strcmpi((s1), (s2))
#define strtoul(nptr, endptr, base) bcm_strtoul((nptr), (endptr), (base))
#define tolower(c) (bcm_isupper((c)) ? ((c) + 'a' - 'A') : (c))
#define fprintf(stream, fmt, args...) xprintf(fmt, ##args)
#define fputs(s, stream) puts(s)
#define malloc(size) KMALLOC((size), 0)
#define free(ptr) KFREE(ptr)
/* XXX Buggy implementations */
#define strnicmp(s1, s2, len) strncmp((s1), (s2), (len))
#define strspn(s, accept) (0)
#define strtol(nptr, endptr, base) bcm_strtoul((nptr), (endptr), (base))

#elif	defined(__IOPOS__) /* XXX Nintendo Revolution */

#define fprintf(file, fmt, arg...) jtag_printf(fmt, ##arg)
#define fputs(s, stream) jtag_puts(s)
#define printf jtag_printf
#define stricmp strcmp
#define strnicmp strncmp
#define strtol(nptr, endptr, base) ((long)strtoul((nptr), (endptr), (base)))
#define tolower(c) bcm_tolower(c)
#define toupper(c) bcm_toupper(c)
#define islower(c) bcm_islower(c)
#define isprint(c) bcm_isprint(c)
#define isspace(c) bcm_isspace(c)
#define isupper(c) bcm_isupper(c)
#define isxdigit(c) bcm_isxdigit(c)
#define isdigit(c) bcm_isdigit(c)

#elif	defined(BWL_STRICMP)

#define stricmp bcmstricmp
#define strnicmp bcmstrnicmp

#endif /* BWL_STRICMP */

/* IOCTL swapping mode for Big Endian host with Little Endian dongle.  Default to off */
/* The below macros handle endian mis-matches between wl utility and wl driver. */
extern bool g_swap;
#define htod64(i) (g_swap?bcmswap64(i):(uint64)(i))
#define htod32(i) (g_swap?bcmswap32(i):(uint32)(i))
#define htod16(i) (g_swap?bcmswap16(i):(uint16)(i))
#define dtoh64(i) (g_swap?bcmswap64(i):(uint64)(i))
#define dtoh32(i) (g_swap?bcmswap32(i):(uint32)(i))
#define dtoh16(i) (g_swap?bcmswap16(i):(uint16)(i))
#define htodchanspec(i) (g_swap?htod16(i):i)
#define dtohchanspec(i) (g_swap?dtoh16(i):i)
#define htodenum(i) (g_swap?((sizeof(i) == 4) ? htod32(i) : ((sizeof(i) == 2) ? htod16(i) : i)):i)
#define dtohenum(i) (g_swap?((sizeof(i) == 4) ? dtoh32(i) : ((sizeof(i) == 2) ? htod16(i) : i)):i)

/* command batching data structure */
typedef struct wl_seq_cmd_pkt {
	struct wl_seq_cmd_pkt	*next;
	wl_seq_cmd_ioctl_t cmd_header;
	char * data;			/* user buffer */
} wl_seq_cmd_pkt_t;

typedef struct wl_cmd_list {
	wl_seq_cmd_pkt_t	*head;
	wl_seq_cmd_pkt_t	*tail;
} wl_cmd_list_t;

/*
 * Name table to associate strings with numeric IDs
 */
typedef struct wlu_name_entry {
	uint id;
	const char *name;
} wlu_name_entry_t;

extern wl_cmd_list_t cmd_list;
extern int cmd_pkt_list_num;
extern bool cmd_batching_mode;

extern int wlu_iovar_getbuf(void* wl, const char *iovar,
	void *param, int paramlen, void *bufptr, int buflen);
extern int wlu_iovar_setbuf(void* wl, const char *iovar,
	void *param, int paramlen, void *bufptr, int buflen);
extern int wlu_var_setbuf(void *wl, const char *iovar, void *param, int param_len);
extern int wlu_iovar_getint(void *wl, const char *iovar, int *pval);
extern void init_cmd_batchingmode(void);
extern void clean_up_cmd_list(void);
extern int wl_check(void *wl);

extern int add_one_batched_cmd(int cmd, void *cmdbuf, int len);
extern int wlu_get_req_buflen(int cmd, void *cmdbuf, int len);
extern int wlu_get(void *wl, int cmd, void *cmdbuf, int len);
extern int wlu_set(void *wl, int cmd, void *cmdbuf, int len);
extern int wlu_iovar_get(void *wl, const char *iovar, void *outbuf, int len);
extern int wlu_iovar_set(void *wl, const char *iovar, void *param, int paramlen);
extern int wlu_iovar_getint(void *wl, const char *iovar, int *pval);
extern int wlu_iovar_setint(void *wl, const char *iovar, int val);
extern const char* wlu_lookup_name(const wlu_name_entry_t* tbl, uint id);
