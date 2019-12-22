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

#ifndef _CoreCursorHit_H
#define _CoreCursorHit_H

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

/* Forward declaration of the class Core::CursorHit */
#ifndef _CoreCursorHit_
  EW_DECLARE_CLASS( CoreCursorHit )
#define _CoreCursorHit_
#endif

/* Forward declaration of the class Core::View */
#ifndef _CoreView_
  EW_DECLARE_CLASS( CoreView )
#define _CoreView_
#endif


/* The class Core::CursorHit provides a helper object used to identify the view 
   tapped on the touch screen by the user. When the user touches the screen, the 
   framework searches for a view lying at the touched position and willing to start 
   the touch interaction. This search process is performed by the method Core::View.CursorHitTest(). */
EW_DEFINE_FIELDS( CoreCursorHit, XObject )
  EW_VARIABLE( View,            CoreView )
  EW_VARIABLE( Deviation,       XInt32 )
  EW_VARIABLE( Offset,          XPoint )
EW_END_OF_FIELDS( CoreCursorHit )

/* Virtual Method Table (VMT) for the class : 'Core::CursorHit' */
EW_DEFINE_METHODS( CoreCursorHit, XObject )
EW_END_OF_METHODS( CoreCursorHit )

/* The method Initialize() initializes this Core::CursorHit object with the given 
   parameter. The value offset stores an optional displacement if the user has touched 
   the view outside its boundary area. This can occur when the user tries to hit 
   a small GUI component with a thick finger. This offset value determines the number 
   of pixel to correct all cursor coordinates delivered in the following Core::CursorEvent 
   and Core::DragEvent events. In this manner the target view will receive coordinates 
   lying correctly within its boundary area. */
CoreCursorHit CoreCursorHit_Initialize( CoreCursorHit _this, CoreView aView, XPoint 
  aOffset );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreCursorHit_H */

/* Embedded Wizard */
