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

#include "ewlocale.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "Resources.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x000002B8, /* ratio 51.72 % */
  0xB8001300, 0x8009A452, 0x00DC0037, 0x0C200320, 0x22003C80, 0x8458182A, 0x003A800A,
  0xB39800CA, 0x88C3E1B0, 0xBC501800, 0x42A3B192, 0x3A191C8D, 0x01711884, 0x2E688B45,
  0xB1B39002, 0x02904AE3, 0x09991A09, 0x8FCD8D20, 0xF14E292D, 0x4D264748, 0xE4014095,
  0xA314322C, 0x012CA442, 0x0418A014, 0x5388C180, 0x0AB14209, 0x25412272, 0x7462151E,
  0x4BAA3348, 0xF0C5259D, 0x2A33CB1C, 0xC9068A02, 0x20031802, 0x04B7C466, 0xE9E1C2AB,
  0x03480D93, 0x3D463D70, 0xD3AC1140, 0x7F091192, 0xEA37E995, 0xCF18AAD2, 0x3458E4C8,
  0x438248A2, 0xAA2DEE35, 0xB5000DB1, 0xA44A691A, 0x802E64F8, 0xF34108A2, 0xC9D04A0C,
  0x4A760041, 0x1A2D567A, 0x4422D9C0, 0xFCEDD24A, 0xC06D64B9, 0x54220D04, 0x45781118,
  0x8F8B1A99, 0x3950C92C, 0x9D329841, 0xD349D917, 0x0EA1F4A7, 0xDE30032B, 0xC2DFE134,
  0xD5A3BF61, 0xDC7A56A8, 0x02A9DBD5, 0xF03BFBD8, 0xBD86E94D, 0xBE2C67C3, 0x92006408,
  0x2BDC97AF, 0xED5D34BD, 0x5086B9C6, 0xA6E5D26C, 0xA5115D11, 0x255D52F0, 0x4F1EB081,
  0xDA15ED08, 0x78391F69, 0x79D40002, 0x434699E4, 0x4947A4F9, 0x13E83A17, 0x014A0E47,
  0x54001140, 0xA5839B95, 0x0004C6B9, 0x02E00059, 0x04063448, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XPoint _Const0000 = { 0, 0 };
static const XStringRes _Const0001 = { _StringsDefault0, 0x0133 };

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

/* Include a file containing the font resource : 'Resources::DefaultFont' */
#include "_ResourcesDefaultFont.h"

/* Table with links to derived variants of the font resource : 'Resources::DefaultFont' */
EW_RES_WITHOUT_VARIANTS( ResourcesDefaultFont )

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

/* Include a file containing the bitmap resource : 'Resources::WhiteBitmapStripe' */
#include "_ResourcesWhiteBitmapStripe.h"

/* Table with links to derived variants of the bitmap resource : 'Resources::WhiteBitmapStripe' */
EW_RES_WITHOUT_VARIANTS( ResourcesWhiteBitmapStripe )

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
const XStringRes ResourcesAM = { _StringsDefault0, 0x0049 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesPM = { _StringsDefault0, 0x004E };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJanuary = { _StringsDefault0, 0x0053 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesFebruary = { _StringsDefault0, 0x005D };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMarch = { _StringsDefault0, 0x0068 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesApril = { _StringsDefault0, 0x0070 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMay = { _StringsDefault0, 0x0078 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJune = { _StringsDefault0, 0x007E };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJuly = { _StringsDefault0, 0x0085 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesAugust = { _StringsDefault0, 0x008C };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSeptember = { _StringsDefault0, 0x0095 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesOctober = { _StringsDefault0, 0x00A1 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesNovember = { _StringsDefault0, 0x00AB };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesDecember = { _StringsDefault0, 0x00B6 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMondayAbbr = { _StringsDefault0, 0x00C1 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesTuesdayAbbr = { _StringsDefault0, 0x00C7 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesWednesdayAbbr = { _StringsDefault0, 0x00CD };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesThursdayAbbr = { _StringsDefault0, 0x00D3 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesFridayAbbr = { _StringsDefault0, 0x00D9 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSaturdayAbbr = { _StringsDefault0, 0x00DF };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSundayAbbr = { _StringsDefault0, 0x00E5 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJanuaryAbbr = { _StringsDefault0, 0x00EB };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesFebruaryAbbr = { _StringsDefault0, 0x00F1 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMarchAbbr = { _StringsDefault0, 0x00F7 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesAprilAbbr = { _StringsDefault0, 0x00FD };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMayAbbr = { _StringsDefault0, 0x0103 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJuneAbbr = { _StringsDefault0, 0x0109 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJulyAbbr = { _StringsDefault0, 0x010F };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesAugustAbbr = { _StringsDefault0, 0x0115 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSeptemberAbbr = { _StringsDefault0, 0x011B };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesOctoberAbbr = { _StringsDefault0, 0x0121 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesNovemberAbbr = { _StringsDefault0, 0x0127 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesDecemberAbbr = { _StringsDefault0, 0x012D };

/* Embedded Wizard */
