/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

 /*! *********************************************************************************
 * \defgroup Version Interface
 * @{
 ***********************************************************************************/

#ifndef _VERSION_H
#define _VERSION_H

/*!
\file       version.h
\brief      This is a header file for keeping the version of the stack. 
*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"
#include "stack_config.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*!
  INFORMATIVE MACROS THAT SHOULD NOT BE CHANGED BY THE USER
  (THESE ARE USED BY THREAD LIBRARY AT COMPILE TIME)
*/

#define gIpVerMajor_c   1
#define gIpVerMinor_c   1
#define gIpVerPatch_c   2
#define gIpBuildNo_c    36

#define gIpModuleId_c   (0xCE)
#define QU(x) #x
#define QUH(x) QU(x)

/*! NXP Thread Software Version - this is mapped to Vendor SW Version TLV - maximum length 16 bytes */
#define gNXPVendorSwVersion_c "NXP THR" \
                              QUH(gIpVerMajor_c) "." \
                              QUH(gIpVerMinor_c) "." \
                              QUH(gIpVerPatch_c) "." \
                              QUH(gIpBuildNo_c)

/*! NXP Thread SW Version, formated for module information */
#define gNXPIpVerString_c "NXP Thread v" \
                          QUH(gIpVerMajor_c) "." \
                          QUH(gIpVerMinor_c) "." \
                          QUH(gIpVerPatch_c) "." \
                          QUH(gIpBuildNo_c)

/*! NXP IEEE OUI */
#define gNXPThreadStackVendorOUI_c    0x00, 0x60, 0x37

/*! Thread stack Version: VendorOUI(3Bytes),
                          BuildNo(12 bits), Revision(4 bits),
                          Min(4 bits), Maj(4 bits)
    This is mapped to Vendor Stack Version TLV - size 6 bytes */
#define gNXPThreadVendorStackVersion_c  {gNXPThreadStackVendorOUI_c, \
                                       ((gIpBuildNo_c & 0xF0)>>4), \
                                      (((gIpBuildNo_c & 0x0F)<<4)|(gIpVerPatch_c & 0x0F)), \
                                      (((gIpVerMinor_c & 0x0F)<<4)|(gIpVerMajor_c & 0x0F))}

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _VERSION_H */
