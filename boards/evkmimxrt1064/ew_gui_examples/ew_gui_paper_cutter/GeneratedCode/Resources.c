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

#include "ewlocale.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "Resources.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x00000058, /* ratio 95.45 % */
  0xB8005500, 0x00000452, 0x00D0002A, 0x04000328, 0xE4003800, 0x70F37800, 0x74004461,
  0x0869E400, 0x230004E0, 0x6800C317, 0x800A7163, 0x58F40034, 0xE510E8F4, 0x00528E1C,
  0x80020C14, 0x0027C708, 0x0260009C, 0x17000B20, 0x0C1A8306, 0x00000008, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XPoint _Const0000 = { 0, 0 };
static const XStringRes _Const0001 = { _StringsDefault0, 0x0003 };

/* Initializer for the class 'Resources::Bitmap' */
void ResourcesBitmap__Init( ResourcesBitmap _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreResource__Init( &_this->_Super, aLink, aArg );

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
  /* Finalize this class */
  _this->_VMT = EW_CLASS( ResourcesBitmap );

  /* Call the user defined destructor of the class */
  ResourcesBitmap_Done( _this );

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
      noOfFrames = bmp->NoOfFrames;
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
                 "Resources::Bitmap" )
  ResourcesBitmap_OnSetFrameSize,
  ResourcesBitmap_Update,
EW_END_OF_CLASS( ResourcesBitmap )

/* Initializer for the class 'Resources::Font' */
void ResourcesFont__Init( ResourcesFont _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreResource__Init( &_this->_Super, aLink, aArg );

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
  /* Finalize this class */
  _this->_VMT = EW_CLASS( ResourcesFont );

  /* Call the user defined destructor of the class */
  ResourcesFont_Done( _this );

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
                 "Resources::Font" )
EW_END_OF_CLASS( ResourcesFont )

/* Embedded Wizard */
