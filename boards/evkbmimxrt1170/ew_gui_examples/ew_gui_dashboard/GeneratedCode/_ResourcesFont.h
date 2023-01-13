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

#ifndef _ResourcesFont_H
#define _ResourcesFont_H

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

/* Forward declaration of the class Resources::Font */
#ifndef _ResourcesFont_
  EW_DECLARE_CLASS( ResourcesFont )
#define _ResourcesFont_
#endif


/* The class Resources::Font provides the base functionality to handle fonts. A 
   font serves as a kind of container to store glyphs together with their metrics 
   and attributes. Fonts are used to print text on the screen. Due to their complexity 
   fonts can be loaded from font resources only. The dynamic creation or modification 
   of fonts at the runtime is not possible.
   This class implements a set of methods and properties to query font and glyph 
   metrics and attributes. For example, the method @GetGlyphSize() returns the size 
   in pixel of the specified glyph. The method @GetTextExtent() in contrast determines 
   the area needed to print the given text.
   Font are created at the design time by drag-and-drop them from the Embedded Wizard 
   Gallery into the Composer window where you are editing your GUI project. The 
   font attributes will be determined in the Embedded Wizard Inspector. */
EW_DEFINE_FIELDS( ResourcesFont, CoreResource )
  EW_VARIABLE( font,            XHandle )
  EW_PROPERTY( Leading,         XInt32 )
  EW_PROPERTY( Descent,         XInt32 )
  EW_PROPERTY( Ascent,          XInt32 )
EW_END_OF_FIELDS( ResourcesFont )

/* Virtual Method Table (VMT) for the class : 'Resources::Font' */
EW_DEFINE_METHODS( ResourcesFont, CoreResource )
EW_END_OF_METHODS( ResourcesFont )

/* 'C' function for method : 'Resources::Font.Done()' */
void ResourcesFont_Done( ResourcesFont _this );

/* 'C' function for method : 'Resources::Font.Init()' */
void ResourcesFont_Init( ResourcesFont _this, XHandle aArg );

/* 'C' function for method : 'Resources::Font.GetFlowTextAdvance()' */
XInt32 ResourcesFont_GetFlowTextAdvance( ResourcesFont _this, XString aFlowString );

/* 'C' function for method : 'Resources::Font.ParseFlowString()' */
XString ResourcesFont_ParseFlowString( ResourcesFont _this, XString aString, XInt32 
  aOffset, XInt32 aWidth, XInt32 aMaxNoOfRows, XHandle aBidi );

/* The method GetTextAdvance() calculates the horizontal advance in pixel of a text 
   row to print with this font. This value is calculated by the sum of advance values 
   of all affected glyphs. The text is passed in the parameter aString. The parameter 
   aOffset determines within aString the sign to start the calculation. If aOffset 
   is zero, the calculation starts with the first sign. The parameter aCount determines 
   the max. number of following sigs to calculate the advance value. If aCount is 
   -1, all signs until the end of the string will be evaluated. */
XInt32 ResourcesFont_GetTextAdvance( ResourcesFont _this, XString aString, XInt32 
  aOffset, XInt32 aCount );

/* Default onget method for the property 'Leading' */
XInt32 ResourcesFont_OnGetLeading( ResourcesFont _this );

/* Wrapper function for the non virtual method : 'Resources::Font.OnGetLeading()' */
XInt32 ResourcesFont__OnGetLeading( void* _this );

/* The following define announces the presence of the method Resources::Font.OnGetLeading(). */
#define _ResourcesFont__OnGetLeading_

/* Default onget method for the property 'Descent' */
XInt32 ResourcesFont_OnGetDescent( ResourcesFont _this );

/* Wrapper function for the non virtual method : 'Resources::Font.OnGetDescent()' */
XInt32 ResourcesFont__OnGetDescent( void* _this );

/* The following define announces the presence of the method Resources::Font.OnGetDescent(). */
#define _ResourcesFont__OnGetDescent_

/* Default onget method for the property 'Ascent' */
XInt32 ResourcesFont_OnGetAscent( ResourcesFont _this );

/* Wrapper function for the non virtual method : 'Resources::Font.OnGetAscent()' */
XInt32 ResourcesFont__OnGetAscent( void* _this );

/* The following define announces the presence of the method Resources::Font.OnGetAscent(). */
#define _ResourcesFont__OnGetAscent_

#ifdef __cplusplus
  }
#endif

#endif /* _ResourcesFont_H */

/* Embedded Wizard */
