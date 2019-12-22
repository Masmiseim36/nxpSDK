/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This file was generated automatically by Embedded Wizard Studio.
*
* Please do not make any modifications of this file! The modifications are lost
* when the file is generated again by Embedded Wizard Studio!
*
* The template of this heading text can be found in the file 'head.ewt' in the
* directory 'Platforms' of your Embedded Wizard installation directory. If you
* wish to adapt this text, please copy the template file 'head.ewt' into your
* project directory and edit the copy only. Please avoid any modifications of
* the original template file!
*
* Version  : 9.20
* Profile  : iMX_RT
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#ifndef _CoreResource_H
#define _CoreResource_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

/* Forward declaration of the class Core::Resource */
#ifndef _CoreResource_
  EW_DECLARE_CLASS( CoreResource )
#define _CoreResource_
#endif


/* Deklaration of class : 'Core::Resource' */
EW_DEFINE_FIELDS( CoreResource, XObject )
  EW_VARIABLE( resource,        XHandle )
EW_END_OF_FIELDS( CoreResource )

/* Virtual Method Table (VMT) for the class : 'Core::Resource' */
EW_DEFINE_METHODS( CoreResource, XObject )
EW_END_OF_METHODS( CoreResource )

/* 'C' function for method : 'Core::Resource.Done()' */
void CoreResource_Done( CoreResource _this );

/* 'C' function for method : 'Core::Resource.Init()' */
void CoreResource_Init( CoreResource _this, XHandle aArg );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreResource_H */

/* Embedded Wizard */
