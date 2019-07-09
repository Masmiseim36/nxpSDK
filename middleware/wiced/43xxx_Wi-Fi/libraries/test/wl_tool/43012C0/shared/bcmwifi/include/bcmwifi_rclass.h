/*
 * Common interface to channel definitions.
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
 * $Id: bcmwifi_rclass.h 660004 2016-09-16 23:08:05Z pavank $
 */

#ifndef _BCMWIFI_RCLASS_H_
#define _BCMWIFI_RCLASS_H_

#include <typedefs.h>
#include <bcmutils.h>
#include <bcmwifi_channels.h>

#define BCMWIFI_MAX_VEC_SIZE	(255/8)

/* Actual bandwidth in Annex E - x tables */
#define BCMWIFI_BW_20	20
#define BCMWIFI_BW_40	40
#define BCMWIFI_BW_80	80
#define BCMWIFI_BW_160	160

/*
 * Band constants representing op class channel starting
 * frequency in 500KHz units. i.e. 2 = 1 MHz, 10,000 = 5 GHz
 */
enum {
	BCMWIFI_BAND_2G = (2407 * 2),
	BCMWIFI_BAND_5G = (5000 * 2)
};

enum {
	BCMWIFI_RCLASS_CHAN_TYPE_CHAN_PRIMARY = 0,
	BCMWIFI_RCLASS_CHAN_TYPE_CNTR_FREQ = 1
};
typedef uint8 bcmwifi_rclass_chan_type_t;

/*
 * Constants for the domain of op classess defined in
 * IEEE 802.11REVmc_D8.0, Annex E.
 */
enum {
	BCMWIFI_RCLASS_TYPE_NONE = 0,
	BCMWIFI_RCLASS_TYPE_US = 1,	/* United States, Table E-1 */
	BCMWIFI_RCLASS_TYPE_EU = 2,	/* Europe, Table E-2 */
	BCMWIFI_RCLASS_TYPE_JP = 3,	/* Japan, Table E-3 */
	BCMWIFI_RCLASS_TYPE_GBL = 4,	/* Global, Table E-4 */
	BCMWIFI_RCLASS_TYPE_CH = 5	/* China, Table E-5 */
};
typedef uint8 bcmwifi_rclass_type_t;

enum {
	BCMWIFI_RCLASS_FLAGS_NONE = 0x0000,
	BCMWIFI_RCLASS_FLAGS_PRIMARY_LOWER = 0x0001,
	BCMWIFI_RCLASS_FLAGS_PRIMARY_UPPER = 0x0002,
	BCMWIFI_RCLASS_FLAGS_EIRP = 0x0004,
	BCMWIFI_RCLASS_FLAGS_DFS = 0x0008,
	BCMWIFI_RCLASS_FLAGS_NOMADIC = 0x0010,
	BCMWIFI_RCLASS_FLAGS_LIC_EXMPT = 0x0020,
	BCMWIFI_RCLASS_FLAGS_80PLUS = 0x0040
};
typedef uint16 bcmwifi_rclass_flags_t;

/* an operating class number 0 - 255 */
typedef uint8 bcmwifi_rclass_opclass_t;
/* a channel number from 0 - 200 */
typedef uint8 bcmwifi_rclass_channel_t;
/* band represented as a Starting Channel frequency in 500KHz units */
typedef uint32 bcmwifi_rclass_band_t;
/* bandwidth in MHz */
typedef uint16 bcmwifi_rclass_bw_t;
/* an array of bcmwifi_rclass_channel_t */
typedef uint8 bcmwifi_rclass_chan_set_t;

/*
 * Op class table info
 */
typedef struct bcmwifi_rclass_info {
	bcmwifi_rclass_opclass_t rclass;	/* op class */
	bcmwifi_rclass_chan_type_t chan_type;	/* channel type */
	bcmwifi_rclass_bw_t bw;			/* bandwidth */
	bcmwifi_rclass_band_t band;		/* starting frequency */
	bcmwifi_rclass_flags_t flags;		/* behavior info */
	uint8 chan_set_len;			/* len of the list chan_set */
	const bcmwifi_rclass_chan_set_t *chan_set;	/* channel set or list of channels */
} bcmwifi_rclass_info_t;

/*
 * Operating class table bit vector of supported
 * op classes
 */
typedef struct bcmwifi_rclass_bvec {
	uint8 len;	/* length of bit vector array */
	uint8 bvec[0];	/* array of bits */
} bcmwifi_rclass_bvec_t;

/*
 * Return the operating class for a given chanspec
 * Input: chanspec
 *        type (op class type is US, EU, JP, GBL, or CH). Currently only GBL
 *        is supported.
 * Output: rclass (op class)
 * On success return status BCME_OK.
 * On error return status BCME_UNSUPPORTED, BCME_BADARG, BCME_ERROR.
 */
int bcmwifi_rclass_get_opclass(bcmwifi_rclass_type_t type, chanspec_t chanspec,
	bcmwifi_rclass_opclass_t *rclass);

/*
 * Return op class info (starting freq, bandwidth, sideband, channel set and behavior)
 * for given op class and type.
 * Input(s): type (op class type - US, EU, JP, GBL, or CH)
 *           rclass (op class)
 * Output: rcinfo (op class info entry)
 * Return status BCME_OK when no error. On error, BCME_ERROR.
 */
int bcmwifi_rclass_get_rclass_info(bcmwifi_rclass_type_t type,
	bcmwifi_rclass_opclass_t rclass, const bcmwifi_rclass_info_t **rcinfo);

/*
 * Return supported op class encoded as a bitvector.
 * Input: type (op class type US, EU, JP, GBL, or CH)
 *
 * Output: rclass_bvec
 * Return status BCME_OK when no error.
 * On error, return status BCME_ERROR, BCME_BADARG, BCME_UNSUPPORTED
 */
int bcmwifi_rclass_get_supported_opclasses(bcmwifi_rclass_type_t type,
	const bcmwifi_rclass_bvec_t **rclass_bvec);

/*
 * Return chanspec given op class type, op class and channel index
 * Input: type (op class type US, EU, JP, GBL, CH)
 *        rclass (op class)
 *        chn_idx (index in channel list)
 * Output: chanspec
 * Return BCME_OK on success and BCME_ERROR on error
 */
int bcmwifi_rclass_get_chanspec(bcmwifi_rclass_type_t type, bcmwifi_rclass_opclass_t rclass,
	uint8 chan_idx, chanspec_t *cspec);
#endif /* _BCMWIFI_RCLASS_H */
