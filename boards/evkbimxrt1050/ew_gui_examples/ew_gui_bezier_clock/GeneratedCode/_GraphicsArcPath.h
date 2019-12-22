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

#ifndef _GraphicsArcPath_H
#define _GraphicsArcPath_H

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

/* 'C' function for method : 'Graphics::ArcPath.OnSetNoOfEdges()' */
void GraphicsArcPath_OnSetNoOfEdges( GraphicsArcPath _this, XInt32 value );

/* 'C' function for method : 'Graphics::ArcPath.OnSetEndAngle()' */
void GraphicsArcPath_OnSetEndAngle( GraphicsArcPath _this, XFloat value );

/* 'C' function for method : 'Graphics::ArcPath.OnSetStartAngle()' */
void GraphicsArcPath_OnSetStartAngle( GraphicsArcPath _this, XFloat value );

/* 'C' function for method : 'Graphics::ArcPath.OnSetRadiusY()' */
void GraphicsArcPath_OnSetRadiusY( GraphicsArcPath _this, XFloat value );

/* 'C' function for method : 'Graphics::ArcPath.OnSetRadiusX()' */
void GraphicsArcPath_OnSetRadiusX( GraphicsArcPath _this, XFloat value );

/* 'C' function for method : 'Graphics::ArcPath.OnSetRadius()' */
void GraphicsArcPath_OnSetRadius( GraphicsArcPath _this, XFloat value );

/* 'C' function for method : 'Graphics::ArcPath.OnSetStyle()' */
void GraphicsArcPath_OnSetStyle( GraphicsArcPath _this, XEnum value );

#ifdef __cplusplus
  }
#endif

#endif /* _GraphicsArcPath_H */

/* Embedded Wizard */
