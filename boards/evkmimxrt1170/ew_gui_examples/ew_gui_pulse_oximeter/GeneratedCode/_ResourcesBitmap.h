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

#ifndef _ResourcesBitmap_H
#define _ResourcesBitmap_H

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

#include "_CoreResource.h"

/* Forward declaration of the class Resources::Bitmap */
#ifndef _ResourcesBitmap_
  EW_DECLARE_CLASS( ResourcesBitmap )
#define _ResourcesBitmap_
#endif


/* The class Resources::Bitmap provides the base functionality to handle bitmaps. 
   A bitmap consists of at least one frame (an image). If there are more than one 
   frame stored within the bitmap, the bitmap is called 'multi-frame'. Such multi-frame 
   bitmaps are very useful e.g. to run short video sequences.
   This class implements a set of properties to query or change bitmap attributes. 
   For example, the property @FrameSize stores the size of a single bitmap frame 
   in pixel. The property @NoOfFrames determines how many frames are available in 
   the bitmap.
   Usually, bitmaps are created at the design time by drag-and-drop them from the 
   Embedded Wizard Gallery into the Composer window where you are editing your GUI 
   project. The bitmap attributes will be determined in the Embedded Wizard Inspector. */
EW_DEFINE_FIELDS( ResourcesBitmap, CoreResource )
  EW_VARIABLE( bitmap,          XHandle )
  EW_PROPERTY( FrameDelay,      XInt32 )
  EW_PROPERTY( NoOfFrames,      XInt32 )
  EW_PROPERTY( FrameSize,       XPoint )
  EW_VARIABLE( Mutable,         XBool )
  EW_VARIABLE( Animated,        XBool )
EW_END_OF_FIELDS( ResourcesBitmap )

/* Virtual Method Table (VMT) for the class : 'Resources::Bitmap' */
EW_DEFINE_METHODS( ResourcesBitmap, CoreResource )
  EW_METHOD( OnSetFrameSize,    void )( ResourcesBitmap _this, XPoint value )
  EW_METHOD( Update,            void )( ResourcesBitmap _this )
EW_END_OF_METHODS( ResourcesBitmap )

/* 'C' function for method : 'Resources::Bitmap.Done()' */
void ResourcesBitmap_Done( ResourcesBitmap _this );

/* 'C' function for method : 'Resources::Bitmap.Init()' */
void ResourcesBitmap_Init( ResourcesBitmap _this, XHandle aArg );

/* 'C' function for method : 'Resources::Bitmap.OnSetFrameSize()' */
void ResourcesBitmap_OnSetFrameSize( ResourcesBitmap _this, XPoint value );

/* Wrapper function for the virtual method : 'Resources::Bitmap.OnSetFrameSize()' */
void ResourcesBitmap__OnSetFrameSize( void* _this, XPoint value );

/* 'C' function for method : 'Resources::Bitmap.Update()' */
void ResourcesBitmap_Update( ResourcesBitmap _this );

/* Wrapper function for the virtual method : 'Resources::Bitmap.Update()' */
void ResourcesBitmap__Update( void* _this );

#ifdef __cplusplus
  }
#endif

#endif /* _ResourcesBitmap_H */

/* Embedded Wizard */
