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
* Version  : 9.30
* Date     : 14.02.2020  8:00:50
* Profile  : iMX_RT
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#include "ewlocale.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "Resources.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x000002A4, /* ratio 51.48 % */
  0xB8001300, 0x8009A452, 0x00DC0037, 0x0C200320, 0x22003C80, 0x8458182A, 0x003A800A,
  0xB39800CA, 0x88C3E1B0, 0xBC501800, 0x42A3B192, 0x3A191C8D, 0x01711884, 0x2E688B45,
  0xB1B39002, 0x02904AE3, 0x09991A09, 0x8FCD8D20, 0xF14E292D, 0x4D264748, 0xE4014095,
  0xA314322C, 0x012CA442, 0xC4A8244C, 0x0E8C42A3, 0xA9754669, 0x9E18A4B3, 0x45467958,
  0x5660D140, 0xC4006300, 0x6096488C, 0x78700010, 0xD20364FA, 0x518F5940, 0xEAAC500F,
  0xBD4464B4, 0x8DD26554, 0xC62DF4BA, 0x16393233, 0xE092288D, 0x8B718D50, 0xE0036C6A,
  0x129A46AB, 0x0B413E29, 0xCB4228A0, 0x7412833C, 0x9D801072, 0x63419492, 0x08B5C006,
  0x27699291, 0xDAB92E57, 0x0883412E, 0x5DB44615, 0xDDC6A651, 0x04324B23, 0x4CA6104E,
  0xD27645E2, 0xA87C89F4, 0x7800C5C3, 0x77584D37, 0x5DEAD5AF, 0x6E457E35, 0x8A747436,
  0xBED6E36F, 0x5F39137B, 0x6305E6EA, 0x0018622F, 0xE325E6E2, 0x169CDF22, 0x08D26F3A,
  0xAD7F234F, 0x14C46D31, 0x1255A92F, 0x84F17008, 0x75975C50, 0x278111F6, 0x479CA000,
  0x94346994, 0xD4947A4F, 0x713E8120, 0x0014A044, 0x5BC00114, 0x19B811AF, 0x90004C69,
  0x802E0005, 0x004062A4, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XPoint _Const0000 = { 0, 0 };
static const XStringRes _Const0001 = { _StringsDefault0, 0x0129 };

/* Initializer for the class 'Resources::Bitmap' */
void ResourcesBitmap__Init( ResourcesBitmap _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreResource__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ResourcesBitmap );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ResourcesBitmap );

  /* ... and initialize objects, variables, properties, etc. */
  _this->NoOfFrames = 1;

  /* Call the user defined constructor */
  ResourcesBitmap_Init( _this, aArg );
}

/* Re-Initializer for the class 'Resources::Bitmap' */
void ResourcesBitmap__ReInit( ResourcesBitmap _this )
{
  /* At first re-initialize the super class ... */
  CoreResource__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Resources::Bitmap' */
void ResourcesBitmap__Done( ResourcesBitmap _this )
{
  /* Call the user defined destructor of the class */
  ResourcesBitmap_Done( _this );

  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreResource );

  /* Don't forget to deinitialize the super class ... */
  CoreResource__Done( &_this->_Super );
}

/* 'C' function for method : 'Resources::Bitmap.Done()' */
void ResourcesBitmap_Done( ResourcesBitmap _this )
{
  XHandle handle;

  if ( _this->bitmap == 0 )
    return;

  handle = _this->bitmap;
  EwFreeBitmap((XBitmap*)handle );
  _this->bitmap = 0;
  _this->FrameSize = _Const0000;
  _this->FrameDelay = 0;
  _this->NoOfFrames = 1;
  _this->Animated = 0;
}

/* 'C' function for method : 'Resources::Bitmap.Init()' */
void ResourcesBitmap_Init( ResourcesBitmap _this, XHandle aArg )
{
  XHandle handle;
  XInt32 noOfFrames;
  XPoint frameSize;
  XInt32 frameDelay;

  if ( aArg == 0 )
    return;

  handle = 0;
  noOfFrames = 1;
  frameSize = _Const0000;
  frameDelay = 0;
  {
    /* aArg is a pointer to the memory where the bitmap resource is stored. */
    XBitmap* bmp = EwLoadBitmap((const XBmpRes*)aArg );

    /* After the bitmap has been loaded get the size of the bitmap and store it
       in the following variables. */
    if ( bmp )
    {
      noOfFrames = bmp->NoOfVirtFrames;
      frameSize  = bmp->FrameSize;
      frameDelay = bmp->FrameDelay;
    }

    handle = (XHandle)(void*)bmp;
  }
  _this->bitmap = handle;
  _this->NoOfFrames = noOfFrames;
  _this->FrameSize = frameSize;
  _this->FrameDelay = frameDelay;
  _this->Animated = (XBool)((( _this->bitmap != 0 ) && ( _this->FrameDelay > 0 )) 
  && ( _this->NoOfFrames > 1 ));
}

/* 'C' function for method : 'Resources::Bitmap.OnSetFrameSize()' */
void ResourcesBitmap_OnSetFrameSize( ResourcesBitmap _this, XPoint value )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( value );

  {
    EwThrow( EwLoadString( &_Const0001 ));
    return;
  }
}

/* Wrapper function for the virtual method : 'Resources::Bitmap.OnSetFrameSize()' */
void ResourcesBitmap__OnSetFrameSize( void* _this, XPoint value )
{
  ((ResourcesBitmap)_this)->_VMT->OnSetFrameSize((ResourcesBitmap)_this, value );
}

/* 'C' function for method : 'Resources::Bitmap.Update()' */
void ResourcesBitmap_Update( ResourcesBitmap _this )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
}

/* Wrapper function for the virtual method : 'Resources::Bitmap.Update()' */
void ResourcesBitmap__Update( void* _this )
{
  ((ResourcesBitmap)_this)->_VMT->Update((ResourcesBitmap)_this );
}

/* Variants derived from the class : 'Resources::Bitmap' */
EW_DEFINE_CLASS_VARIANTS( ResourcesBitmap )
EW_END_OF_CLASS_VARIANTS( ResourcesBitmap )

/* Virtual Method Table (VMT) for the class : 'Resources::Bitmap' */
EW_DEFINE_CLASS( ResourcesBitmap, CoreResource, _None, _None, _None, _None, _None, 
                 _None, "Resources::Bitmap" )
  ResourcesBitmap_OnSetFrameSize,
  ResourcesBitmap_Update,
EW_END_OF_CLASS( ResourcesBitmap )

/* Initializer for the class 'Resources::Font' */
void ResourcesFont__Init( ResourcesFont _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreResource__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ResourcesFont );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ResourcesFont );

  /* Call the user defined constructor */
  ResourcesFont_Init( _this, aArg );
}

/* Re-Initializer for the class 'Resources::Font' */
void ResourcesFont__ReInit( ResourcesFont _this )
{
  /* At first re-initialize the super class ... */
  CoreResource__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Resources::Font' */
void ResourcesFont__Done( ResourcesFont _this )
{
  /* Call the user defined destructor of the class */
  ResourcesFont_Done( _this );

  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreResource );

  /* Don't forget to deinitialize the super class ... */
  CoreResource__Done( &_this->_Super );
}

/* 'C' function for method : 'Resources::Font.Done()' */
void ResourcesFont_Done( ResourcesFont _this )
{
  XHandle handle;

  if ( _this->font == 0 )
    return;

  handle = _this->font;
  EwFreeFont((XFont*)handle );
  _this->font = 0;
  _this->Ascent = 0;
  _this->Descent = 0;
  _this->Leading = 0;
}

/* 'C' function for method : 'Resources::Font.Init()' */
void ResourcesFont_Init( ResourcesFont _this, XHandle aArg )
{
  XHandle handle;
  XInt32 ascent;
  XInt32 descent;
  XInt32 leading;

  if ( aArg == 0 )
    return;

  handle = 0;
  ascent = 0;
  descent = 0;
  leading = 0;
  {
    /* aArg is a pointer to a memory where the font resource is stored. */
    XFont* font = EwLoadFont((const XFntRes*)aArg );

    /* After the font has been loaded query its ascent and descent. */
    if ( font )
    {
      ascent  = font->Ascent;
      descent = font->Descent;
      leading = font->Leading;
    }

    handle = (XHandle)(void*)font;
  }
  _this->font = handle;
  _this->Ascent = ascent;
  _this->Descent = descent;
  _this->Leading = leading;
}

/* 'C' function for method : 'Resources::Font.GetFlowTextAdvance()' */
XInt32 ResourcesFont_GetFlowTextAdvance( ResourcesFont _this, XString aFlowString )
{
  XHandle handle;
  XInt32 advance;

  if ( _this->font == 0 )
    return 0;

  handle = _this->font;
  advance = 0;
  advance = EwGetFlowTextAdvance((XFont*)handle, aFlowString );
  return advance;
}

/* 'C' function for method : 'Resources::Font.ParseFlowString()' */
XString ResourcesFont_ParseFlowString( ResourcesFont _this, XString aString, XInt32 
  aOffset, XInt32 aWidth, XInt32 aMaxNoOfRows, XHandle aBidi )
{
  XHandle handle;
  XString result;

  if ( aOffset < 0 )
    aOffset = 0;

  if (( _this->font == 0 ) || (( aOffset > 0 ) && ( aOffset >= EwGetStringLength( 
      aString ))))
    return 0;

  handle = _this->font;
  result = 0;
  result = EwParseFlowString((XFont*)handle, aString + aOffset, aWidth, aMaxNoOfRows, aBidi );
  return result;
}

/* The method GetTextAdvance() calculates the horizontal advance in pixel of a text 
   row to print with this font. This value is calculated by the sum of advance values 
   of all affected glyphs. The text is passed in the parameter aString. The parameter 
   aOffset determines within aString the sign to start the calculation. If aOffset 
   is zero, the calculation starts with the first sign. The parameter aCount determines 
   the max. number of following sigs to calculate the advance value. If aCount is 
   -1, all signs until the end of the string will be evaluated. */
XInt32 ResourcesFont_GetTextAdvance( ResourcesFont _this, XString aString, XInt32 
  aOffset, XInt32 aCount )
{
  XHandle handle;
  XInt32 advance;

  if ( aOffset < 0 )
    aOffset = 0;

  if (( _this->font == 0 ) || (( aOffset > 0 ) && ( aOffset >= EwGetStringLength( 
      aString ))))
    return 0;

  handle = _this->font;
  advance = 0;
  advance = EwGetTextAdvance((XFont*)handle, aString + aOffset, aCount );
  return advance;
}

/* Default onget method for the property 'Leading' */
XInt32 ResourcesFont_OnGetLeading( ResourcesFont _this )
{
  return _this->Leading;
}

/* Wrapper function for the non virtual method : 'Resources::Font.OnGetLeading()' */
XInt32 ResourcesFont__OnGetLeading( void* _this )
{
  return ResourcesFont_OnGetLeading((ResourcesFont)_this );
}

/* Default onget method for the property 'Descent' */
XInt32 ResourcesFont_OnGetDescent( ResourcesFont _this )
{
  return _this->Descent;
}

/* Wrapper function for the non virtual method : 'Resources::Font.OnGetDescent()' */
XInt32 ResourcesFont__OnGetDescent( void* _this )
{
  return ResourcesFont_OnGetDescent((ResourcesFont)_this );
}

/* Default onget method for the property 'Ascent' */
XInt32 ResourcesFont_OnGetAscent( ResourcesFont _this )
{
  return _this->Ascent;
}

/* Wrapper function for the non virtual method : 'Resources::Font.OnGetAscent()' */
XInt32 ResourcesFont__OnGetAscent( void* _this )
{
  return ResourcesFont_OnGetAscent((ResourcesFont)_this );
}

/* Variants derived from the class : 'Resources::Font' */
EW_DEFINE_CLASS_VARIANTS( ResourcesFont )
EW_END_OF_CLASS_VARIANTS( ResourcesFont )

/* Virtual Method Table (VMT) for the class : 'Resources::Font' */
EW_DEFINE_CLASS( ResourcesFont, CoreResource, _None, _None, _None, _None, _None, 
                 _None, "Resources::Font" )
EW_END_OF_CLASS( ResourcesFont )

/* Include a file containing the bitmap resource : 'Resources::WhiteBitmap' */
#include "_ResourcesWhiteBitmap.h"

/* Table with links to derived variants of the bitmap resource : 'Resources::WhiteBitmap' */
EW_RES_WITHOUT_VARIANTS( ResourcesWhiteBitmap )

/* Include a file containing the bitmap resource : 'Resources::WhiteBitmapStripe' */
#include "_ResourcesWhiteBitmapStripe.h"

/* Table with links to derived variants of the bitmap resource : 'Resources::WhiteBitmapStripe' */
EW_RES_WITHOUT_VARIANTS( ResourcesWhiteBitmapStripe )

/* Include a file containing the bitmap resource : 'Resources::NavigationIconsMedium' */
#include "_ResourcesNavigationIconsMedium.h"

/* Table with links to derived variants of the bitmap resource : 'Resources::NavigationIconsMedium' */
EW_RES_WITHOUT_VARIANTS( ResourcesNavigationIconsMedium )

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMonday = { _StringsDefault0, 0x0002 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesTuesday = { _StringsDefault0, 0x000B };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesWednesday = { _StringsDefault0, 0x0015 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesThursday = { _StringsDefault0, 0x0021 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesFriday = { _StringsDefault0, 0x002C };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSaturday = { _StringsDefault0, 0x0035 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSunday = { _StringsDefault0, 0x0040 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJanuary = { _StringsDefault0, 0x0049 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesFebruary = { _StringsDefault0, 0x0053 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMarch = { _StringsDefault0, 0x005E };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesApril = { _StringsDefault0, 0x0066 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMay = { _StringsDefault0, 0x006E };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJune = { _StringsDefault0, 0x0074 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJuly = { _StringsDefault0, 0x007B };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesAugust = { _StringsDefault0, 0x0082 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSeptember = { _StringsDefault0, 0x008B };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesOctober = { _StringsDefault0, 0x0097 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesNovember = { _StringsDefault0, 0x00A1 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesDecember = { _StringsDefault0, 0x00AC };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMondayAbbr = { _StringsDefault0, 0x00B7 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesTuesdayAbbr = { _StringsDefault0, 0x00BD };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesWednesdayAbbr = { _StringsDefault0, 0x00C3 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesThursdayAbbr = { _StringsDefault0, 0x00C9 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesFridayAbbr = { _StringsDefault0, 0x00CF };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSaturdayAbbr = { _StringsDefault0, 0x00D5 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSundayAbbr = { _StringsDefault0, 0x00DB };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJanuaryAbbr = { _StringsDefault0, 0x00E1 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesFebruaryAbbr = { _StringsDefault0, 0x00E7 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMarchAbbr = { _StringsDefault0, 0x00ED };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesAprilAbbr = { _StringsDefault0, 0x00F3 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMayAbbr = { _StringsDefault0, 0x00F9 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJuneAbbr = { _StringsDefault0, 0x00FF };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJulyAbbr = { _StringsDefault0, 0x0105 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesAugustAbbr = { _StringsDefault0, 0x010B };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSeptemberAbbr = { _StringsDefault0, 0x0111 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesOctoberAbbr = { _StringsDefault0, 0x0117 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesNovemberAbbr = { _StringsDefault0, 0x011D };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesDecemberAbbr = { _StringsDefault0, 0x0123 };

/* Embedded Wizard */
