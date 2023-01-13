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

#ifndef Graphics_H
#define Graphics_H

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

#include "_GraphicsArcPath.h"
#include "_GraphicsCanvas.h"
#include "_GraphicsPath.h"
#include "_GraphicsWarpMatrix.h"

/* The definition Graphics::Edges enumerates the parts of a free scalable frame. 
   It is used to determine, which parts should be drawn. */
typedef enum
{
  GraphicsEdgesLeft                     = 0x00000001,
  GraphicsEdgesTop                      = 0x00000002,
  GraphicsEdgesRight                    = 0x00000004,
  GraphicsEdgesBottom                   = 0x00000008,
  GraphicsEdgesInterior                 = 0x00000010
} GraphicsEdges;

/* The definition Graphics::FillRule enumerates the possible modes how complex polygons 
   consisting of nested paths are filled. It is used to determine how Views::FillPath 
   should fill the path. */
typedef enum
{
  GraphicsFillRuleEvenOdd               = 0,
  GraphicsFillRuleNonZero               = 1
} GraphicsFillRule;

/* The definition Graphics::PathJoin enumerates the possible modes how the edges 
   of a path are connected when stroking the path with path thickness greater than 
   1 pixel. It is used to determine how Views::StrokePath should raster the path. */
typedef enum
{
  GraphicsPathJoinBevel                 = 0,
  GraphicsPathJoinMiter                 = 1,
  GraphicsPathJoinRound                 = 2
} GraphicsPathJoin;

/* The definition Graphics::PathCap enumerates the possible modes how the path caps 
   are displayed when stroking the path with path thickness greater than 1 pixel. 
   It is used to determine how Views::StrokePath should raster the path. */
typedef enum
{
  GraphicsPathCapFlat                   = 0,
  GraphicsPathCapSquare                 = 1,
  GraphicsPathCapRound                  = 2,
  GraphicsPathCapTriangle               = 3
} GraphicsPathCap;

/* The definition Graphics::SVGParserStatus enumerates the possible error states 
   of the SVG string parser implemented in the method Graphics::Path.CreateFromSVGString(). */
typedef enum
{
  GraphicsSVGParserStatusSuccess        = 0,
  GraphicsSVGParserStatusEmptyString    = 1,
  GraphicsSVGParserStatusSyntaxError    = 2,
  GraphicsSVGParserStatusInvalidArgument = 3,
  GraphicsSVGParserStatusOutOfMemory    = 4
} GraphicsSVGParserStatus;

/* The definition Graphics::ArcStyle enumerates the possible shapes of an elliptical 
   arc. It is used to determine the mode how Graphics::ArcPath should calculate 
   the path data. */
typedef enum
{
  GraphicsArcStyleArc                   = 0,
  GraphicsArcStyleSegment               = 1,
  GraphicsArcStylePie                   = 2,
  GraphicsArcStylePieRounded            = 3,
  GraphicsArcStylePieRoundedStart       = 4,
  GraphicsArcStylePieRoundedEnd         = 5
} GraphicsArcStyle;

#ifdef __cplusplus
  }
#endif

#endif /* Graphics_H */

/* Embedded Wizard */
