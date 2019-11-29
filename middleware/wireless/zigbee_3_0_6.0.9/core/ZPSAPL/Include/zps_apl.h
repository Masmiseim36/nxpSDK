/*
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef ZPS_APL_H_
#define ZPS_APL_H_

/*!
\file       zps_apl.h
\brief      Application Layer global definitions
*/


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/*
 * general status type
 * The value returned can be from any layer of the ZigBee stack
 * Consult the relevant header file depending on the value
 * 0x80 - 0x8F : Zigbee device profile (zps_apl_zdp.h)
 * 0xA0 - 0xBF : Application support sub-layer (zps_apl_aps.h)
 * 0xC0 - 0xCF : Network layer (zps_nwk_sap.h)
 * 0xE0 - 0xFF : MAC layer (mac_sap.h)
 */
typedef uint8 ZPS_teStatus;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/


/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /* ZPS_APL_H_ */
