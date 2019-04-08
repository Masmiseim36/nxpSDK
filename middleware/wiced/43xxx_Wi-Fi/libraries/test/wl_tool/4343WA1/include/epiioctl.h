/*
 * iLine10(tm) Windows device driver custom OID definitions.
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
 * $Id: epiioctl.h 514055 2014-11-08 18:42:49Z sudhirbs $
 */

#ifndef _epiioctl_h_
#define	_epiioctl_h_

/*
 * custom OID support
 *
 * 0xFF - implementation specific OID
 * 0xDA - first byte of Epigram PCI vendor ID
 * 0xFE - second byte of Epigram PCI vendor ID
 * 0xXX - the custom OID number
 *
 * Order is monotonically increasing but non-contiguous
 * to preserve backwards compatibility with older OIDS.
 *
 * NOTE: Add any query oids to IS_QUERY_OID() .
 */

#define OID_EPI_BASE				0xFFFEDA00

#define	OID_EPI_LIST				(OID_EPI_BASE + 0x01)
#define	OID_EPI_UP				(OID_EPI_BASE + 0x02)
#define	OID_EPI_DOWN				(OID_EPI_BASE + 0x03)
#define	OID_EPI_LOOP				(OID_EPI_BASE + 0x04)
#define	OID_EPI_DUMP				(OID_EPI_BASE + 0x05)
#define	OID_EPI_TXPE				(OID_EPI_BASE + 0x06)
#define	OID_EPI_TXBPB				(OID_EPI_BASE + 0x06) /* usb driver compatibility */
#define	OID_EPI_TXPRI				(OID_EPI_BASE + 0x07)
#define	OID_EPI_SETMSGLEVEL			(OID_EPI_BASE + 0x08)
#define	OID_EPI_PROMISC				(OID_EPI_BASE + 0x0A)
#define	OID_EPI_LINKINT				(OID_EPI_BASE + 0x0B)

#define	OID_EPI_PESSET				(OID_EPI_BASE + 0x0C)
#define	OID_EPI_DUMPPES				(OID_EPI_BASE + 0x0D)

#define	OID_EPI_SET_DEBUG			(OID_EPI_BASE + 0x10)

#define	OID_LARQ_DUMP				(OID_EPI_BASE + 0x11)
#define	OID_LARQ_ONOFF				(OID_EPI_BASE + 0x12)
#define	OID_LARQ_SETMSGLEVEL			(OID_EPI_BASE + 0x13)

#define OID_USB_DUMP				(OID_EPI_BASE + 0x14)
#define	OID_EPI_MAXRXBPB			(OID_EPI_BASE + 0x15)

#define	OID_EPISTAT_BLOCK			(OID_EPI_BASE + 0x16)
#define	OID_EPISTAT_BLOCKSIZE			(OID_EPI_BASE + 0x17)
#define	OID_EPISTAT_GET_CHANNEL_ESTIMATE	(OID_EPI_BASE + 0x18)
#define	OID_EPISTAT_BLOCK_LARQ			(OID_EPI_BASE + 0x19)
#define	OID_EPISTAT_BLOCKSIZE_LARQ		(OID_EPI_BASE + 0x1a)

#define	OID_EPI_TXDOWN				(OID_EPI_BASE + 0x1b)

#define OID_USB_VERSION				(OID_EPI_BASE + 0x1c)

#define	OID_EPI_PROMISCTYPE			(OID_EPI_BASE + 0x1d)
#define	OID_EPI_TXGEN				(OID_EPI_BASE + 0x1e)

#define	OID_EL_SETMSGLEVEL			(OID_EPI_BASE + 0x21)
#define	OID_EL_LARQ_ONOFF			(OID_EPI_BASE + 0x24)
#define	OID_EPI_GET_INSTANCE			(OID_EPI_BASE + 0x25)
#define	OID_EPI_IS_EPILAYER			(OID_EPI_BASE + 0x26)
#define	OID_CSA					(OID_EPI_BASE + 0x28)

#define	OID_SET_HPNA_MODE			(OID_EPI_BASE + 0x2c)
#define OID_SET_CSA_HPNA_MODE			(OID_EPI_BASE + 0x2d)

#define	OID_EPI_RXSEL_HYST			(OID_EPI_BASE + 0x2e)
#define	OID_EPI_RXSEL_MAXERR			(OID_EPI_BASE + 0x2f)
#define	OID_EPI_RXSEL_MAXDELTA			(OID_EPI_BASE + 0x30)
#define	OID_EPI_RXSEL_KMAX			(OID_EPI_BASE + 0x31)

#define	OID_EPI_DUMPSCB				(OID_EPI_BASE + 0x33)

#define OID_EPI_XLIST				(OID_EPI_BASE + 0x34)

#define OID_EPI_COS_MODE			(OID_EPI_BASE + 0x36)
#define OID_EPI_COS_LIST			(OID_EPI_BASE + 0x37)
#define OID_EPI_DUMPPM				(OID_EPI_BASE + 0x38)
#define OID_EPI_FORCEPM				(OID_EPI_BASE + 0x39)

/* Srom access for (at least) USB */
#define	OID_EPI_READ_SROM			(OID_EPI_BASE + 0x3a)
#define	OID_EPI_WRITE_SROM			(OID_EPI_BASE + 0x3b)

#define OID_EPI_CONSUMECERT			(OID_EPI_BASE + 0x3c)
#define OID_EPI_SENDUPLINK			(OID_EPI_BASE + 0x3d)
/* (OID_EPI_BASE + 0x3e-0x3f) are defined in oidencap.h */

#ifdef	INTEL
#    define RMA_MASK_POWER_ON_COUNT			0x000000FF
#    define RMA_MASK_POWER_ON_DURATION			0x0000FF00
#    define RMA_MASK_FIRMWARE_DOWNLOAD_COUNT		0x00FF0000
#    define RMA_MASK_RESERVED				0xFF000000
#    define SERIAL_NO_LENGTH				17

#    define OID_CUSTOM_BASE				0xFFA0C900
#    define OID_CUSTOM_RMA_COUNTERS			(OID_CUSTOM_BASE + 0x01)
#    define OID_CUSTOM_SERIAL_NUMBER			(OID_CUSTOM_BASE + 0x02)
#endif	/* INTEL */

/* Classify oids */

#define IS_EPI_OID(oid) (((oid) & 0xFFFFFF00) == 0xFFFEDA00)

#define	IS_QUERY_OID(oid) \
	((oid == OID_EPI_LIST) || \
	 (oid == OID_EPI_XLIST) || \
	 (oid == OID_EPI_DUMP) || \
	 (oid == OID_EPI_DUMPSCB) || \
	 (oid == OID_EPISTAT_BLOCK) || \
	 (oid == OID_EPISTAT_BLOCKSIZE) || \
	 (oid == OID_EPISTAT_BLOCK_LARQ) || \
	 (oid == OID_EPISTAT_BLOCKSIZE_LARQ) || \
	 (oid == OID_EPI_GET_INSTANCE) || \
	 (oid == OID_USB_DUMP) || \
	 (oid == OID_USB_VERSION) || \
	 (oid == OID_LARQ_DUMP) || \
	 (oid == OID_EPI_READ_SROM) || \
	 (oid == OID_EPI_DUMPPES) || \
	 (oid == OID_EPI_DUMPPM) || \
	 (oid == OID_EPI_FORCEPM))
#endif /* _epiioctl_h_ */
