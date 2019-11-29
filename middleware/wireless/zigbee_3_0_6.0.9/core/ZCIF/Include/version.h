/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _VERSION_H_
#define _VERSION_H_

/*!
\file       version.h
\brief      module version
*/

#if defined __cplusplus
extern "C" {
#endif
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

//#define SVN_VERSION         (0x40000000UL|0x80000000UL|9999UL)  // These values are overwritten by the release script
//#define SVN_VERSIONSTR      "UNCONTROLLED"                      // But not when building from a checkout.
//#define SVN_REVDATESTR      "UNCONTROLLED"                      // UNCONTROLLED should be replaced by the svn information when using version.h from a release.zip
//#define SVN_BUILDDATESTR    "UNCONTROLLED"

/* ZCL build version  */
/*! \cond DOXY_SKIP_TAG */
#define gZclVerMajor_c   6
#define gZclVerMinor_c   0
#define gZclVerPatch_c   9
#define gZclBuildNo_c    2
#define gZclModuleId_c   0x70
/*! \endcond */

#define gZclVerString_c "ZCL_Version_v" \
                         QUH(gZclVerMajor_c) "." \
                         QUH(gZclVerMinor_c) "." \
                         QUH(gZclVerPatch_c)  
    
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /*_VESRION_H_*/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
