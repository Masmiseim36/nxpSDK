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

#ifndef _CoreLayoutContext_H
#define _CoreLayoutContext_H

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

/* Forward declaration of the class Core::LayoutContext */
#ifndef _CoreLayoutContext_
  EW_DECLARE_CLASS( CoreLayoutContext )
#define _CoreLayoutContext_
#endif

/* Forward declaration of the class Core::Outline */
#ifndef _CoreOutline_
  EW_DECLARE_CLASS( CoreOutline )
#define _CoreOutline_
#endif


/* Deklaration of class : 'Core::LayoutContext' */
EW_DEFINE_FIELDS( CoreLayoutContext, XObject )
  EW_VARIABLE( outline,         CoreOutline )
  EW_VARIABLE( extent,          XRect )
  EW_VARIABLE( bounds,          XRect )
  EW_VARIABLE( isEmpty,         XBool )
EW_END_OF_FIELDS( CoreLayoutContext )

/* Virtual Method Table (VMT) for the class : 'Core::LayoutContext' */
EW_DEFINE_METHODS( CoreLayoutContext, XObject )
EW_END_OF_METHODS( CoreLayoutContext )

#ifdef __cplusplus
  }
#endif

#endif /* _CoreLayoutContext_H */

/* Embedded Wizard */
