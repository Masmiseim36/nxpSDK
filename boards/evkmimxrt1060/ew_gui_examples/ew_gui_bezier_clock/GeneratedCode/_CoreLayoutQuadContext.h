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

#ifndef _CoreLayoutQuadContext_H
#define _CoreLayoutQuadContext_H

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

#include "_CoreLayoutContext.h"

/* Forward declaration of the class Core::LayoutQuadContext */
#ifndef _CoreLayoutQuadContext_
  EW_DECLARE_CLASS( CoreLayoutQuadContext )
#define _CoreLayoutQuadContext_
#endif

/* Forward declaration of the class Core::Outline */
#ifndef _CoreOutline_
  EW_DECLARE_CLASS( CoreOutline )
#define _CoreOutline_
#endif


/* Deklaration of class : 'Core::LayoutQuadContext' */
EW_DEFINE_FIELDS( CoreLayoutQuadContext, CoreLayoutContext )
  EW_VARIABLE( point4,          XPoint )
  EW_VARIABLE( point3,          XPoint )
  EW_VARIABLE( point2,          XPoint )
  EW_VARIABLE( point1,          XPoint )
EW_END_OF_FIELDS( CoreLayoutQuadContext )

/* Virtual Method Table (VMT) for the class : 'Core::LayoutQuadContext' */
EW_DEFINE_METHODS( CoreLayoutQuadContext, CoreLayoutContext )
EW_END_OF_METHODS( CoreLayoutQuadContext )

#ifdef __cplusplus
  }
#endif

#endif /* _CoreLayoutQuadContext_H */

/* Embedded Wizard */
