/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software 
* components. It is provided as an example software which is intended to be 
* modified and extended according to particular requirements.
* 
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability 
* and non-infringement of any third party IPR or other rights which may result 
* from the use or the inability to use the software.
*
* This file was generated automatically by Embedded Wizard Studio.
*
* Please do not make any modifications of this file! The modifications are lost
* when the file is generated again by Embedded Wizard Studio!
*
* Version  : 10.0
* Date     : 17.02.2021  8:00:50
* Profile  : iMX_RT
* Platform : NXP.iMX_RT_VGLite.RGB565
*
*******************************************************************************/

#ifndef _GraphicsArcPath_H
#define _GraphicsArcPath_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x000A0000
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x000A0000
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

#include "_GraphicsPath.h"

/* Forward declaration of the class Graphics::ArcPath */
#ifndef _GraphicsArcPath_
  EW_DECLARE_CLASS( GraphicsArcPath )
#define _GraphicsArcPath_
#endif


/* The class Graphics::ArcPath provides a convenient version of Graphics::Path class 
   intended to calculate paths for elliptical arcs, segments, pies or rings. The 
   arc is configured with the properties @StartAngle, @EndAngle, @RadiusX and @RadiusY. 
   With the property @Style the particular variant (Arc, Segment, Pie, etc.) of 
   the resulting path is determined. If the style 'Pie', 'PieRounded', 'PieRoundedStart' 
   or 'PieRoundedEnd' is selected, the the additional properties @InnerRadiusX and 
   @InnerRadiusY can be used to construct a ring or segment of a ring.
   With the path information the curves can be displayed on the screen. The view 
   Views::FillPath displays the path as filled polygon. The view Views::StrokePath 
   displays the path as a line with specified width. */
EW_DEFINE_FIELDS( GraphicsArcPath, GraphicsPath )
  EW_PROPERTY( NoOfEdges,       XInt32 )
  EW_PROPERTY( EndAngle,        XFloat )
  EW_PROPERTY( StartAngle,      XFloat )
  EW_PROPERTY( InnerRadiusY,    XFloat )
  EW_PROPERTY( InnerRadiusX,    XFloat )
  EW_PROPERTY( RadiusY,         XFloat )
  EW_PROPERTY( RadiusX,         XFloat )
  EW_PROPERTY( Style,           XEnum )
EW_END_OF_FIELDS( GraphicsArcPath )

/* Virtual Method Table (VMT) for the class : 'Graphics::ArcPath' */
EW_DEFINE_METHODS( GraphicsArcPath, GraphicsPath )
EW_END_OF_METHODS( GraphicsArcPath )

/* 'C' function for method : 'Graphics::ArcPath.updatePath()' */
void GraphicsArcPath_updatePath( GraphicsArcPath _this, XObject sender );

/* 'C' function for method : 'Graphics::ArcPath.OnSetRadius()' */
void GraphicsArcPath_OnSetRadius( GraphicsArcPath _this, XFloat value );

/* 'C' function for method : 'Graphics::ArcPath.OnSetStyle()' */
void GraphicsArcPath_OnSetStyle( GraphicsArcPath _this, XEnum value );

#ifdef __cplusplus
  }
#endif

#endif /* _GraphicsArcPath_H */

/* Embedded Wizard */
