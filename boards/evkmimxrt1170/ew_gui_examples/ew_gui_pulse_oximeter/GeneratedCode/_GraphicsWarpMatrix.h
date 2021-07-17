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

#ifndef _GraphicsWarpMatrix_H
#define _GraphicsWarpMatrix_H

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

/* Forward declaration of the class Graphics::WarpMatrix */
#ifndef _GraphicsWarpMatrix_
  EW_DECLARE_CLASS( GraphicsWarpMatrix )
#define _GraphicsWarpMatrix_
#endif


/* The class Graphics::WarpMatrix provides the core functionality for all 2D and 
   3D transformations. With its aim, Views::WarpImage and Views::WarpGroup can be 
   translated, scaled and rotated around the X, Y and Z axis. Moreover, small 3D 
   scenes consisting of several warp views can be defined. This allows you to enrich 
   GUI components with fancy 3D effects. Please note, Embedded Wizard is not a 3D 
   GUI development tool! Its lightweight 3D functionality is limited to the perspectively 
   correct projection of a source image onto a four corners polygon lying in the 
   GUI component. The matrix coefficients describe thus the transformation of the 
   rectangular area of the source image into a four corners polygon.
   To define the desired transformation the WarpMatrix class provides following 
   three methods @Translate(), @Scale() and @Rotate(). These can be combined together 
   in order to get more complex transformations. The source image can thus be rotated 
   and then e.g. translated in the 3D space. The order in which these operations 
   are executed is essential. Changing of the order may produce different results.
   The methods @Push() and @Pop() are very useful to store and restore the current 
   state of the matrix on the matrix stack. This is important when 3D scenes are 
   composed of several transformed source images. In this manner one and the same 
   matrix is used for all warp views. To perform the transformation defined in the 
   matrix, pass it to the Warp3D() method provided by the WarpImage and WarpGroup 
   views. */
EW_DEFINE_FIELDS( GraphicsWarpMatrix, XObject )
  EW_VARIABLE( M11,             XFloat )
  EW_VARIABLE( M12,             XFloat )
  EW_VARIABLE( M13,             XFloat )
  EW_VARIABLE( M14,             XFloat )
  EW_VARIABLE( M21,             XFloat )
  EW_VARIABLE( M22,             XFloat )
  EW_VARIABLE( M23,             XFloat )
  EW_VARIABLE( M24,             XFloat )
  EW_VARIABLE( M31,             XFloat )
  EW_VARIABLE( M32,             XFloat )
  EW_VARIABLE( M33,             XFloat )
  EW_VARIABLE( M34,             XFloat )
  EW_VARIABLE( M41,             XFloat )
  EW_VARIABLE( M42,             XFloat )
  EW_VARIABLE( M43,             XFloat )
  EW_VARIABLE( M44,             XFloat )
  EW_VARIABLE( Z,               XFloat )
  EW_VARIABLE( Y,               XFloat )
  EW_VARIABLE( X,               XFloat )
  EW_PROPERTY( EyeDistance,     XFloat )
  EW_VARIABLE( isIdentity,      XBool )
EW_END_OF_FIELDS( GraphicsWarpMatrix )

/* Virtual Method Table (VMT) for the class : 'Graphics::WarpMatrix' */
EW_DEFINE_METHODS( GraphicsWarpMatrix, XObject )
EW_END_OF_METHODS( GraphicsWarpMatrix )

/* 'C' function for method : 'Graphics::WarpMatrix.CalculateZ()' */
XBool GraphicsWarpMatrix_CalculateZ( GraphicsWarpMatrix _this, XFloat aX, XFloat 
  aY );

/* 'C' function for method : 'Graphics::WarpMatrix.Project()' */
XBool GraphicsWarpMatrix_Project( GraphicsWarpMatrix _this, XFloat aX, XFloat aY );

/* The method Rotate() applies the given angles to the matrix. This corresponds 
   to the rotation of the source image around the X-, Y- and Z-axis. The angles 
   are specified in degrees. The method returns 'this' object to the caller. */
GraphicsWarpMatrix GraphicsWarpMatrix_Rotate( GraphicsWarpMatrix _this, XFloat aAngleX, 
  XFloat aAngleY, XFloat aAngleZ );

/* The method Translate() applies the given displacement to the matrix. This corresponds 
   to the translation of the source image by the given values in the X-, Y- and 
   Z-direction. The method returns 'this' object to the caller. */
GraphicsWarpMatrix GraphicsWarpMatrix_Translate( GraphicsWarpMatrix _this, XFloat 
  aDeltaX, XFloat aDeltaY, XFloat aDeltaZ );

/* 'C' function for method : 'Graphics::WarpMatrix.DeriveFromQuad()' */
GraphicsWarpMatrix GraphicsWarpMatrix_DeriveFromQuad( GraphicsWarpMatrix _this, 
  XFloat aX1, XFloat aY1, XFloat aX2, XFloat aY2, XFloat aX3, XFloat aY3, XFloat 
  aX4, XFloat aY4 );

/* The method Assign() copies the coefficients of the given aMatrix into the own 
   coefficients. The method returns 'this' object to the caller. */
GraphicsWarpMatrix GraphicsWarpMatrix_Assign( GraphicsWarpMatrix _this, GraphicsWarpMatrix 
  aMatrix );

#ifdef __cplusplus
  }
#endif

#endif /* _GraphicsWarpMatrix_H */

/* Embedded Wizard */
