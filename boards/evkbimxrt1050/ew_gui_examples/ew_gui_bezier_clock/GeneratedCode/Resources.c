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
  0x000002F2, /* ratio 48.81 % */
  0xB8001500, 0x80000452, 0x00DE0026, 0x0C800370, 0xF2003080, 0x60B00418, 0x54004160,
  0x5001D400, 0x869CC006, 0x6C4E230F, 0x15E0D160, 0x7619208E, 0xA210E8FC, 0x4A0C8344,
  0x46800462, 0x1E390023, 0x289CBA41, 0x2341A090, 0xD1A40137, 0xCCA61229, 0xD8E92129,
  0x28B2D9C4, 0x91179E80, 0xF4D8646E, 0x4A994525, 0x08DC3242, 0x98D5A711, 0xED00013E,
  0xD7282628, 0xC81356A0, 0xC015D844, 0xD5139A18, 0x00020D86, 0x6CA1D04E, 0x1B581E26,
  0xAC022AD2, 0xFC4E5536, 0xC6EB361B, 0x1AA156BC, 0x8CF1BBA4, 0x0D4A8FCD, 0xB48B0C92,
  0x1DA55DA3, 0xDB2800DB, 0x8AC1A711, 0x05B89F61, 0xE72154A0, 0x3AC346A0, 0x0EC00849,
  0x3DA7CD4A, 0x917B0003, 0x99B7CA08, 0xBD94A337, 0x85422C37, 0xBEF60D0C, 0x47637198,
  0x764F32E1, 0x6C3C8874, 0x9B1B9AC6, 0x738A045F, 0x488374A8, 0x30064621, 0xBD607B1C,
  0xE7062180, 0xEEAC732E, 0xF24307EB, 0xEED74996, 0xC17DDF7A, 0xDE8EFE17, 0xE8BC8030,
  0x8C700319, 0xF95D72BE, 0xD6F68917, 0xB001DD76, 0xA1AE429A, 0x6F475B67, 0x4C82A419,
  0xC204A1CF, 0x6429387F, 0x89A46757, 0x6209E134, 0x66910505, 0x9435211D, 0x3871281E,
  0x4D1F50E1, 0x05000528, 0x84DB65D1, 0x04C6ADA2, 0xE0005900, 0x063844C2, 0x00000004,
  0x00000000
};

/* Constant values used in this 'C' module only. */
static const XPoint _Const0000 = { 0, 0 };
static const XStringRes _Const0001 = { _StringsDefault0, 0x0150 };

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

/* Bitmap resource : 'Resources::WhiteBitmap' */
EW_DEFINE_BITMAP_RES( ResourcesWhiteBitmap )
  EW_BITMAP_FRAMES( ResourcesWhiteBitmap, Default, EW_DRIVER_VARIANT_ALPHA8, 254, 254, 0 )
    EW_BITMAP_FRAME( 0, 0, 254, 254, 0x00000000, 0x00000000 )

  EW_BITMAP_PIXEL( ResourcesWhiteBitmap, Default )              /* ratio 0.66 % */
    0x1C09FF00, 0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x231087C3, 0xB1489C4A, 0x188BC5A2,
    0xC8DC6A33, 0x8FC7A3B1, 0x1C8A4320, 0xC9A4B249, 0xAA532893, 0xA5B2C95C, 0x633097CB,
    0xB3499CCA, 0x389BCDA6, 0xC9DCEA73, 0x9FCFA7B3, 0x1D0A8340, 0xD1A8B44A, 0x2A9348A3,
    0xA9B4CA5D, 0xA350A7D3, 0xB54A9D4A, 0x58ABD5AA, 0xCADD6AB3, 0xAFD7ABB5, 0x1D8AC360,
    0xD9ACB64B, 0xAAD368B3, 0xADB6CB5D, 0xE370B7DB, 0xB74B9DCA, 0x78BBDDAE, 0xCBDDEAF3,
    0xBFDFAFB7, 0x1E0B0380, 0xE1B0B84C, 0x2B1388C3, 0xB1B8CC5E, 0x2390C7E3, 0xB94C9E4B,
    0x98CBE5B2, 0xCCDE6B33, 0xCFE7B3B9, 0x1E8B43A0, 0xE9B4BA4D, 0xAB53A8D3, 0xB5BACD5E,
    0x63B0D7EB, 0xBB4D9ECB, 0xB8DBEDB6, 0xCDDEEB73, 0xDFEFB7BB, 0x1F0B83C0, 0xF1B8BC4E,
    0x2B93C8E3, 0xB9BCCE5F, 0xA3D0E7F3, 0xBD4E9F4B, 0xD8EBF5BA, 0xCEDF6BB3, 0xEFF7BBBD,
    0x1F8BC3E0, 0xF9BCBE4F, 0xABD3E8F3, 0xBDBECF5F, 0xE3F0F7FB, 0xBF4F9FCB, 0xF8FBFDBE,
    0xCFDFEBF3, 0xFFFFBFBF, 0xE0280600, 0x68160480, 0x260881E0, 0x0C82E0A8, 0x83E0E836,
    0xE1284610, 0x68561484, 0x661885E1, 0x1C86E1A8, 0x87E1E876, 0xE2288620, 0x68962488,
    0xA62889E2, 0x2C8AE2A8, 0x8BE2E8B6, 0xE328C630, 0x68D6348C, 0xE6388DE3, 0x3C8EE3A8,
    0x8FE3E8F6, 0xE4290640, 0x69164490, 0x264891E4, 0x4C92E4A9, 0x93E4E936, 0xE5294650,
    0x69565494, 0x665895E5, 0x5C96E5A9, 0x97E5E976, 0xE6298660, 0x69966498, 0x01017FE6,
    0x00000000

  EW_BITMAP_CLUT_EMPTY( ResourcesWhiteBitmap, Default )

  EW_BITMAPS_TABLE( ResourcesWhiteBitmap )
    EW_BITMAP( ResourcesWhiteBitmap, Default )
EW_END_OF_BITMAP_RES( ResourcesWhiteBitmap )

/* Table with links to derived variants of the bitmap resource : 'Resources::WhiteBitmap' */
EW_RES_WITHOUT_VARIANTS( ResourcesWhiteBitmap )

/* Bitmap resource : 'Resources::WhiteBitmapStripe' */
EW_DEFINE_BITMAP_RES( ResourcesWhiteBitmapStripe )
  EW_BITMAP_FRAMES( ResourcesWhiteBitmapStripe, Default, EW_DRIVER_VARIANT_ALPHA8, 256, 4, 0 )
    EW_BITMAP_FRAME( 0, 0, 256, 4, 0x00000000, 0x00000000 )

  EW_BITMAP_PIXEL( ResourcesWhiteBitmapStripe, Default )        /* ratio 5.86 % */
    0x1C09FF00, 0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x231087C3, 0xB1489C4A, 0x188BC5A2,
    0xC8DC6A33, 0x8FC7A3B1, 0x1C8A4320, 0xC9A4B249, 0xAA532893, 0x6452C95C, 0x00000040,
    0x00000000

  EW_BITMAP_CLUT_EMPTY( ResourcesWhiteBitmapStripe, Default )

  EW_BITMAPS_TABLE( ResourcesWhiteBitmapStripe )
    EW_BITMAP( ResourcesWhiteBitmapStripe, Default )
EW_END_OF_BITMAP_RES( ResourcesWhiteBitmapStripe )

/* Table with links to derived variants of the bitmap resource : 'Resources::WhiteBitmapStripe' */
EW_RES_WITHOUT_VARIANTS( ResourcesWhiteBitmapStripe )

/* Bitmap resource containing diverse icons you can use in your project. Usually 
   the icons are used to represent navigation buttons, etc.. The icons have the 
   size 36x36 pixel. */
EW_DEFINE_BITMAP_RES( ResourcesNavigationIconsMedium )
  EW_BITMAP_FRAMES( ResourcesNavigationIconsMedium, Default, EW_DRIVER_VARIANT_ALPHA8, 36, 36, 0 )
    EW_BITMAP_FRAME( 16, 13, 4, 3, 0x00000000, 0x00000000 ),
    EW_BITMAP_FRAME( 15, 20, 6, 2, 0x000000BC, 0x00000000 ),
    EW_BITMAP_FRAME( 14, 15, 2, 6, 0x00000178, 0x00000000 ),
    EW_BITMAP_FRAME( 20, 15, 2, 6, 0x00000230, 0x00000000 ),
    EW_BITMAP_FRAME( 0, 0, 0, 0, 0x000002E4, 0x00000000 ),
    EW_BITMAP_FRAME( 12, 5, 2, 13, 0x000003E8, 0x00000000 ),
    EW_BITMAP_FRAME( 18, 12, 13, 2, 0x000004EC, 0x00000000 ),
    EW_BITMAP_FRAME( 5, 12, 13, 2, 0x000005D8, 0x00000000 ),
    EW_BITMAP_FRAME( 11, 24, 4, 2, 0x000006C8, 0x00000000 ),
    EW_BITMAP_FRAME( 16, 16, 4, 4, 0x0000076C, 0x00000000 ),
    EW_BITMAP_FRAME( 16, 16, 4, 4, 0x0000081C, 0x00000000 ),
    EW_BITMAP_FRAME( 5, 10, 26, 3, 0x000008B0, 0x00000000 ),
    EW_BITMAP_FRAME( 15, 6, 2, 7, 0x0000095C, 0x00000000 ),
    EW_BITMAP_FRAME( 2, 11, 31, 2, 0x00000AE8, 0x00000000 ),
    EW_BITMAP_FRAME( 3, 7, 6, 6, 0x00000BBC, 0x00000000 ),
    EW_BITMAP_FRAME( 27, 7, 6, 6, 0x00000D0C, 0x00000000 ),
    EW_BITMAP_FRAME( 2, 18, 21, 2, 0x00000E60, 0x00000000 ),
    EW_BITMAP_FRAME( 13, 18, 20, 2, 0x00000F94, 0x00000000 ),
    EW_BITMAP_FRAME( 17, 3, 2, 30, 0x000010D0, 0x00000000 ),
    EW_BITMAP_FRAME( 3, 17, 30, 2, 0x00001168, 0x00000000 )

  EW_BITMAP_PIXEL( ResourcesNavigationIconsMedium, Default )    /* ratio 17.56 % */
    0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x231087C3, 0xB1489C4A, 0x188BC5A2,
    0xC8DC6A33, 0x300060D1, 0x14740260, 0xBFD20018, 0x72448F7F, 0xE4E9200C, 0x332A48C9,
    0xD2E4A006, 0x1698A4E5, 0x59AA5805, 0x89CE12B3, 0xE4D52E02, 0x25A6A9D4, 0x64051501,
    0x8049A9AA, 0x31354D80, 0xB4F89521, 0x02A9A5E6, 0xFA9D3130, 0x97514050, 0x1CEA093E,
    0x80E195CA, 0xFD25354F, 0x52601004, 0x1A034FCD, 0x13550806, 0x91925AD2, 0x5BBA826A,
    0x46CD4030, 0xAA3A497C, 0x80607819, 0x1516FFBC, 0xE5E8C926, 0xBC619262, 0x25E9263C,
    0x9891E4CC, 0xCCDE6B33, 0xCFE7B3B9, 0x0C1743A0, 0x00000008, 0x1C080100, 0xC1A0B048,
    0x2A130883, 0xA1B0C85C, 0x231087C3, 0xB1489C4A, 0x188BC5A2, 0x98DC6A33, 0x2F184031,
    0xF8061C01, 0xC3242612, 0xFDFE9100, 0x6152645B, 0x232A4600, 0x40184CB5, 0x33474AA5,
    0x6478060D, 0x83CE5FF3, 0x84F54001, 0xCA2053F4, 0x1A8AA09E, 0xE7A9F01C, 0x28019FA4,
    0xE034F4F5, 0x3D4E80A1, 0x2820092B, 0x4E4F52C0, 0xD80A215B, 0x585313D4, 0x6A7A9901,
    0xAD111B0A, 0xCB63015A, 0xE02895B2, 0x36AC57D2, 0x189DCD2D, 0x9ECF52A0, 0x0C469494,
    0x3FEFE930, 0x1E258249, 0x239449B5, 0x13379D80, 0x06000C57, 0x98C7648C, 0xCCDE6B33,
    0xCFE7B3B9, 0x0C6F43A0, 0x00000008, 0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C,
    0x231087C3, 0xB1489C4A, 0x1800C1A2, 0x101C2A2D, 0x00A45FEA, 0xF80A0F1A, 0x9864AFF4,
    0x74050590, 0x0BD2592A, 0x360281CA, 0x08B7FCB1, 0x9A014BE4, 0x89B23A66, 0x99A60052,
    0x0C867890, 0x24A66960, 0x50031AA1, 0xA909599A, 0x669200C5, 0x12F0125E, 0x33480040,
    0x0AAD5135, 0xADA72666, 0x0276675C, 0xA3C032FB, 0x065164AC, 0x53133488, 0x594BA1DA,
    0xB49A725A, 0x7449CB28, 0x04B80A2D, 0x01456F34, 0xA5FA7499, 0xA3267318, 0xCE5513C1,
    0xE0555974, 0x1B279648, 0xCA001C0B, 0x188AE223, 0xCCDE4B2F, 0xCFE7B3B9, 0x00020355,
    0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x231087C3, 0x51489C4A, 0x158B8062,
    0xF4880617, 0x8C80D40F, 0x1BFCC309, 0xC80049E9, 0x8E465F83, 0x24F264E4, 0x548C00C1,
    0xD2D4DBFE, 0x8F00C0A5, 0x69AA6664, 0x4BCCA373, 0x0C0E6C01, 0xA5664930, 0x040142A0,
    0x127324B0, 0x3002A152, 0xD4F48CC9, 0x324D80A6, 0x2975551F, 0x304C93A0, 0xFD740149,
    0x4CAAF49A, 0x0A335B47, 0x521324C8, 0x9B4B8ADA, 0x8018A5BE, 0x13B9D192, 0x84B4AA7D,
    0x954E62D7, 0x0C827898, 0x26A648D0, 0xAA5320C1, 0x24561964, 0x7D27C449, 0xF2EC827E,
    0x98B0DF62, 0xCCDE6B33, 0x80C0F3B9, 0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C,
    0x1D0487C3, 0x00D351A8, 0xF03443E2, 0x7783F1F8, 0x35C322C6, 0x53F2431A, 0x482131E3,
    0x85BADD8D, 0xE0B25924, 0x42DD7E49, 0x91A95084, 0x2F004B65, 0xB99C0A63, 0x10CE66F2,
    0x2CD67904, 0xD0A0D3F9, 0x1E8B1691, 0x63F36A3C, 0x99099D48, 0x50AA5436, 0x0BAA42AA,
    0xF3441EAD, 0xA3C3E814, 0xA8760D1D, 0x442BD287, 0x1838F4A1, 0xA5B6540C, 0xEB064050,
    0xCFCB9C88, 0x743B418E, 0xA831D245, 0x8D5E5E42, 0xFDE20B76, 0x41C54F2B, 0x06FECF27,
    0xEE983826, 0x2F0A26B3, 0x822C9888, 0x1ABBE016, 0x71165169, 0xCBAA62C1, 0xBAA63582,
    0x5D73E009, 0xDF46FCCC, 0xE078BD34, 0x592DD560, 0x525D86A3, 0xA4D4E876, 0x9D16336D,
    0xDC6EF41B, 0xD4693737, 0x71B8A805, 0x91371A5D, 0xE457F206, 0x7A00D20F, 0x0BD7E735,
    0x921C40BA, 0x2EDF693C, 0xEFF58709, 0x0C97C3E0, 0x00000008, 0x1C080100, 0xC1A0B048,
    0x2A130883, 0xA1B0C85C, 0x760087C3, 0xB1450250, 0x101D2522, 0xC7FB0213, 0x6047A3B1,
    0xF10A0D19, 0x9063B248, 0x713D0490, 0xA5A825E4, 0x3D1C5ECB, 0x8AC81CA1, 0x78831DA3,
    0x299AB238, 0x9BBFE6B0, 0xDBFE7305, 0x5027D3C9, 0xCE894282, 0x27538A34, 0x834C004F,
    0xB36A2D3A, 0x51A7D0E9, 0x787D4E91, 0x11578441, 0xFD4AAAB7, 0x88ABEB77, 0x489D0A0B,
    0x2B56292B, 0x0E4F695B, 0xB6C7A8AA, 0x6CA140ED, 0x5830ADEB, 0x815DAEA7, 0x31D6D978,
    0xB8238818, 0x80433310, 0xE0C828E3, 0x11867B86, 0x11CC5467, 0x6883871F, 0x48861E3A,
    0x849AA399, 0x664B346B, 0x9B1FF9D8, 0x9323A185, 0x34F1CC5E, 0x36FFD4C3, 0xBAED2C27,
    0x06D96C21, 0x3DBC6769, 0x90EEA07A, 0x46FE09BC, 0xF0C01C19, 0xF38FBC99, 0xF4DACDBC,
    0x3BB082D3, 0x103E7ED6, 0xF28E3C8D, 0xD83F50D2, 0x069A8D40, 0x1F8BC3DB, 0x0020321F,
    0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x390487C3, 0x02A3B208, 0xF834C3E2,
    0xE7ABFDFE, 0x364322C4, 0xF346431A, 0x46A410A8, 0x902F2170, 0x1FF2683C, 0x452110B8,
    0xF2D82CAC, 0x33988025, 0x210CD607, 0x9DCE6710, 0x04AE7D03, 0xADA3541A, 0xAE934855,
    0x63E404EA, 0x43049B48, 0xEFFA9D4A, 0x3CA7C957, 0xFADD520B, 0xA242072F, 0x1D88EF05,
    0x2F0EF64B, 0xA60E948D, 0x90524AC1, 0x1B065046, 0xD5993BCC, 0x707FDBA3, 0x3BA1AE0B,
    0x825E6EF3, 0xB5D65D7B, 0x5CEE36FB, 0xAAFD2BC1, 0xEF184B65, 0xCB61C657, 0x2FFB3D96,
    0x5ABE63ED, 0x9AF54EB9, 0xCD4F6C10, 0xF42EAD21, 0x52A8F21C, 0x387E9DA4, 0x26742B04,
    0xD9AED6D1, 0x774D8EC2, 0x29976D36, 0xA96C1A73, 0x836EA471, 0xE469EE69, 0xC4A0FBF8,
    0x2B8B153A, 0x62DCCE5F, 0x00000040, 0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C,
    0x550487C3, 0x01D419D8, 0xF527C3E2, 0x97C3FDFE, 0x694362C6, 0xF1AE431A, 0x484310A8,
    0x22492270, 0x04321114, 0x55238D4A, 0x11A96416, 0x3294CB80, 0x27FCD207, 0x9CCC26F0,
    0xF40E7702, 0xC3262FF9, 0x29569D20, 0x56CABA5D, 0x8D448248, 0xB215FC26, 0x509E55AA,
    0xA855165F, 0xAC57AAE3, 0xA41E7159, 0x9DD791A4, 0xDED368B3, 0x21151839, 0x48828D28,
    0xF068ED12, 0x72B8BFED, 0x506BCA5F, 0x9F4CAE17, 0xFDDEFA6A, 0x417032FB, 0xFA3D78BB,
    0x31382BDC, 0xC668A244, 0x994B5596, 0x04A956AD, 0x6AB57EBB, 0xCDD4EDB0, 0x567A0B55,
    0x53292408, 0x0E6B4EA5, 0x68539CBD, 0x9FADD64F, 0xBB1D84DA, 0xFFDAE8E6, 0x70BCE76E,
    0x81502C52, 0xFE62AB20, 0xBFE324ED, 0x1B7F917B, 0x10475154, 0xA3D0E507, 0x1018F74B,
    0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x231087C3, 0xB1489C4A, 0x048BC5A2,
    0x98C0401D, 0x77BA0530, 0xF41A3834, 0x3BDFEFF8, 0x9207220D, 0x43EF692C, 0xFF2B004A,
    0x32F223D9, 0xE89ACCE6, 0x99A4CA4A, 0x914F26D2, 0xBCEA713E, 0x0E8830F9, 0x9A1F4185,
    0x0ACFB7F1, 0x4F0EA4C2, 0x74D92BF8, 0x3F964E64, 0xA6E84E1D, 0xABCAAAB3, 0xE07A273B,
    0x4C29D54A, 0x92C544B0, 0x2A14D7FD, 0xD95DB1C5, 0x115ACD0D, 0x29AC4EAE, 0x697DE2ED,
    0x6EDF2D37, 0x58025F7E, 0xE1B04120, 0x2B1388C3, 0x61D8CC5E, 0x00000040, 0x1C080100,
    0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x231087C3, 0x91489C4A, 0xEC5B8261, 0xC274409C,
    0x860256EE, 0xD3FDFEB7, 0x13A1B1A7, 0x316F227D, 0x64722764, 0x5B278549, 0x531863B2,
    0x28984924, 0xD9C4BE6D, 0x9AC1E6F2, 0xF5021F2E, 0x4EE87049, 0x0E833993, 0x29913A34,
    0x934DA700, 0x714A8492, 0x55A3D4A8, 0x5AC4F693, 0x8AD2A811, 0x0512BD05, 0x4EABD02B,
    0x5EA565B2, 0x6A946B25, 0x1D6EA744, 0x30BB8D52, 0x77845D2E, 0x9B5D86EF, 0x9640EF50,
    0x9D26EB67, 0x57E430B9, 0x622D29C1, 0xE3B7DAE4, 0x2390B447, 0xB94C9E4B, 0x010065B2,
    0x00000001, 0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x231087C3, 0xB1489C4A,
    0x188BC5A2, 0xC8DC6A33, 0x8FC7A3B1, 0x588A2520, 0x41884A94, 0xC5369B19, 0xD5242830,
    0x250B1122, 0x1347FBD8, 0x3419410B, 0x881CAE69, 0x08BDA725, 0xFCE4E70A, 0xA6E08C27,
    0x0E089BA1, 0xE8272A1B, 0x8F03A2D0, 0x703A4CE6, 0x09A6CE69, 0x39B4E665, 0x9DD0C630,
    0xFCD27B02, 0x228450B9, 0xC60C4154, 0x60760948, 0x608A5410, 0xB74B9C86, 0x78BBDDAE,
    0xCBDDEAF3, 0xBBDFAFB7, 0x00000101, 0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C,
    0x231087C3, 0xB1489C4A, 0x188BC5A2, 0x50A40E33, 0x8FC7A3A2, 0xA4824320, 0xBB841070,
    0xAA53287F, 0xA572C95C, 0x8106103B, 0x53499CB4, 0x32831025, 0xB9DCD5FC, 0x838C9FD2,
    0x1D08D64A, 0xD1A8B44A, 0x2A344AA2, 0x12C058D5, 0xA350A7AE, 0xD52A9D4A, 0x3F830152,
    0xF94D6A78, 0xADD6E623, 0xB0360B37, 0xD9ACAD85, 0xA2D368B3, 0xA960D5B5, 0x8F418B5B,
    0x26BB9DCA, 0x5F9D4E29, 0x9AF4FA6B, 0x82586777, 0x7CD2FB26, 0xC8A39079, 0x904B88C3,
    0xB1B8CB7C, 0x2390C7E3, 0x315C9E4B, 0x00000020, 0x1C080100, 0xC1A0B048, 0x2A130883,
    0xA1B0C85C, 0x720087C3, 0xF101517C, 0xFF5301A1, 0x52DBE5FE, 0x5901A191, 0xEB723319,
    0xC663F0A8, 0x5632C876, 0xE4D07920, 0x3C3176BF, 0xB2C0C465, 0x00984BA0, 0x59A3FE64,
    0x18CDE053, 0xACF27502, 0x4EA073F9, 0xB002131E, 0xE4346A2C, 0xA0E67D3F, 0x84045AE5,
    0x4DA64340, 0x877D6A9D, 0x84848B8A, 0x29360758, 0x291348F4, 0x69D8BE57, 0x8F2F67FD,
    0x4E918355, 0x61F87502, 0xF75CA18A, 0x50D34642, 0x97BCC1B0, 0x50623B73, 0xB4DFF7F4,
    0x644B0408, 0x27A075FC, 0x6F19708B, 0xB195FC15, 0x199AE356, 0x41A465CC, 0xCABAC5B0,
    0x182A0997, 0xC64C90E0, 0x7C5D0554, 0xA0B967FE, 0xB6EE7F27, 0x49E5A31D, 0x0383450D,
    0x677BFDC0, 0x82683550, 0x236AD73A, 0xAC20A063, 0xAF7A108C, 0x63341D6E, 0x994ED5B3,
    0x28982051, 0xBE826063, 0x0A165F59, 0x2E0D8220, 0x2B5642B9, 0xC64F7030, 0xB7B3081E,
    0x49504EE6, 0x065F8CB6, 0x21D8CA82, 0xEC03FAA1, 0xD8C91A09, 0x8D31E06B, 0x5EA42802,
    0x44193329, 0x08D267C0, 0xFF3F82A4, 0x7650C304, 0x4F33FCFD, 0x07BD0671, 0x64A2408E,
    0xD9434584, 0x41CA13FD, 0x9FD8300C, 0x4399191C, 0x681B435E, 0x27210C02, 0x4643B416,
    0x9191D887, 0x54F44A3F, 0x19191410, 0x3621D213, 0x56842416, 0x23D69096, 0xB464A941,
    0x1A431A8F, 0x5D0F4014, 0x93E4950C, 0x50294650, 0x00000040, 0x1C080100, 0xC1A0B048,
    0x2A130883, 0xA1B0C85C, 0x231087C3, 0xD1089C4A, 0x1420B605, 0x67F9721B, 0x8546630F,
    0xFC7A391B, 0x48A43078, 0xA9FF2592, 0x4090393C, 0x63305121, 0x13499CCA, 0x2A95B126,
    0x823C0E3B, 0x9FCFA7B3, 0x102E3940, 0xD1A823B8, 0x21C0FFA3, 0x8D64F818, 0xA350A7D3,
    0xAA1A754A, 0x28103827, 0x8A4C9A09, 0x9340EB35, 0x556ABD57, 0x07E17619, 0x6E201950,
    0xC3F0AB2C, 0x205C7FB7, 0xB5D5086A, 0x62B85BED, 0x195DBA07, 0xBE5EEF37, 0x04AEEF7E,
    0x6170B535, 0x080F0327, 0xA578CA46, 0xEA0A810E, 0xD2AC9E35, 0x922040C8, 0xC9AB0E71,
    0x01A13399, 0xECE33104, 0x4AB4579B, 0xA333A5D2, 0x6F378D4E, 0x5C2D6BE8, 0xBB4D9ECB,
    0x0188EDB6, 0x00000001, 0x1C080100, 0xC1A0B048, 0x2A130883, 0x60F0C85C, 0x42872D84,
    0xCA05D2ED, 0x0D089690, 0x63214E05, 0x5E95C050, 0x1C89FEFC, 0xAEDF7229, 0x1E18012A,
    0xA08807FB, 0x63246610, 0x63F654C6, 0x00964AE1, 0xF9921C04, 0x4AA555CC, 0x14C99EAE,
    0xCDE12024, 0x9162867F, 0x8326871C, 0xC8680CC1, 0x00B57491, 0x9F98D1E1, 0xE0842820,
    0x83516A69, 0x639E4557, 0xB9899192, 0x860E8191, 0xA328B7FD, 0x45670AC0, 0x132B00FA,
    0x23265BA7, 0xB824A591, 0xB74B6456, 0x2952469F, 0x1CA457D8, 0xFEA410B7, 0x8DD055D3,
    0x2190C7BB, 0x000B7233, 0x94AD494D, 0xA9186A33, 0x685BB325, 0xCE790D22, 0xA8244AF8,
    0x892B0DD1, 0xA19A85CC, 0x5ABB7FEA, 0xEFFD2C2F, 0xA084ECB4, 0xB846DE91, 0xDDE76459,
    0xE3FF2D06, 0xC1381187, 0x197922C4, 0x2458182F, 0xF103E596, 0x75F6648A, 0x4291DC39,
    0x6541CA18, 0x91B9ECA3, 0x11B217A2, 0x7152EFE9, 0x8B9A1084, 0xA13D5584, 0xDAEA9221,
    0x505030C0, 0x22C51A43, 0x48683163, 0x7D4C90A0, 0xC95280D0, 0x5491B92B, 0x150C7B48,
    0x755D4D11, 0x679D9331, 0x4C314802, 0xC3F20C3F, 0x7A0815CB, 0x8B160126, 0x45513288,
    0xDD079C5B, 0x0405C088, 0x1C080100, 0xC1A0B048, 0x2A130883, 0x20F0C85C, 0x40862C84,
    0xEA15D2E9, 0x0D081690, 0xA5D95217, 0x8FC79FEF, 0xB2F1F71E, 0x18E0C012, 0x247D0B29,
    0xE592C903, 0x010420B2, 0x309633FD, 0xFE9620C0, 0x44A2B99C, 0x7F3395C9, 0x94950B2C,
    0x4218EFF9, 0x8A40AD05, 0x20C1833B, 0x40E33386, 0xD1F42AC2, 0xAE00C1A8, 0xA0727640,
    0x96A781F0, 0x1416AD1E, 0x929CB1F4, 0xB5CC4C8C, 0x07D03B14, 0x28E57FD0, 0x68A81464,
    0x1F6441AD, 0xCA25CECF, 0x29802D92, 0x05CD3D1F, 0xC16FB7C0, 0x71C51E5D, 0x6C0718CD,
    0x3A108258, 0x98333BFE, 0x98CBBB91, 0x9821B731, 0xA6BFA3C5, 0xF21E3383, 0x5CF42CE8,
    0x995D1E50, 0x23CBA8CE, 0x5B0DD5A5, 0x7B0D7BFF, 0xB5D4A861, 0x9D221A17, 0xCF16A15B,
    0x8C1B7BA0, 0x6523CFB0, 0x257B07E1, 0xE0DC807E, 0x0BCDC095, 0x37965806, 0x6520C13A,
    0xB8175AE5, 0x8CD9A3DB, 0x69C54E35, 0x48162018, 0x4213CC4F, 0xE3CFD4B8, 0xCC0B869A,
    0x5C1A5C7F, 0x6C180160, 0x942C81D2, 0x90550031, 0x4AF33CFE, 0x9340A025, 0x0C834E0F,
    0xD4105830, 0x3412C639, 0x8016AC44, 0x0714BC15, 0x48C23F16, 0x106E5251, 0x44D11168,
    0x457228A2, 0xD895DB04, 0x0000405C, 0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C,
    0x231087C3, 0xB1489C4A, 0x5B1041E2, 0x82546632, 0x07856241, 0x1BFDB308, 0x361FF249,
    0xE4990F20, 0xA0E2672B, 0xF33092BB, 0x92372438, 0xB22E41A5, 0x46B4E672, 0x868E9FD2,
    0x3B9A4893, 0x0F8030B1, 0x39992367, 0xE894248C, 0xFB232542, 0xA8A87031, 0xE101C2E9,
    0x2878EE46, 0x1439E488, 0x4C8D8B0B, 0x578F7102, 0xFEC509B0, 0x8E910B23, 0xC76914D7,
    0x6FE3644A, 0x07B83A24, 0xE065B2D5, 0xBFDF998A, 0x23D70380, 0x5CAF3779, 0x2A604900,
    0x9AB24C5E, 0x8102B5E1, 0x525C9E30, 0x00B663D6, 0xCCDA0CCA, 0x43E773B9, 0x4AEE48F4,
    0x01A23970, 0x91752458, 0x8110FD2E, 0xE208D4AD, 0x80A87642, 0x7A856C17, 0x4610361D,
    0x856C5732, 0x842F0EC6, 0xEFF7AC80, 0x8DE40D84, 0x4250C16C, 0x8EB89233, 0xD2334427,
    0x6A86011E, 0x185F45FF, 0x91BBD102, 0x1B3E1AAC, 0xA7E0BDB9, 0x97D1E87D, 0x61A19774,
    0xA4A86187, 0x98B97C0F, 0x028B4A1C, 0xD7DCC163, 0xFC0411CC, 0xC1201103, 0x001E0380,
    0x260881E0, 0x014560A8, 0x00000001, 0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C,
    0x231087C3, 0x91489C4A, 0x5B250621, 0x51046632, 0x6C1020B1, 0xFC8A42FF, 0x04210D97,
    0x8CF50F0A, 0x60EEE95C, 0x86F6750B, 0x4FE1DC34, 0x385935A4, 0x68397271, 0x223DA42B,
    0x74859B0C, 0x434023E6, 0xD9FF3533, 0x4190A22B, 0xCB1024D2, 0x709A5D24, 0x0270C847,
    0x17E97E22, 0x83D31EF2, 0x9B164249, 0x575F6598, 0x52C56023, 0xACEFA294, 0xC6FFB0C1,
    0xAF826452, 0x708291EB, 0x623C52E5, 0x82122BAF, 0xA00A0D79, 0xE1B0ACC4, 0x89D988C3,
    0x6057F7C4, 0x561B82E0, 0x78BC9E4A, 0x05B56381, 0xCCA5702B, 0x03366439, 0x741EE397,
    0xE9B4A832, 0x89C5A8D3, 0xE06196CC, 0x44888A63, 0x1AED6B62, 0xB24844F3, 0x2D16D769,
    0xD925C031, 0x3486DE18, 0x27805126, 0x1B82D791, 0x1E521174, 0x4C048803, 0x906E5721,
    0x6790BDC0, 0x6560161E, 0xEE41FA93, 0xE221DEFF, 0x4877F0C8, 0x660FDE6B, 0x9F723770,
    0x0E9FF77B, 0xA17F3A86, 0x94729021, 0xF05C1A5B, 0x877D57EA, 0x8CD23811, 0xD2C52359,
    0x480741D1, 0x0482A0A0, 0x83E0E81C, 0x61284610, 0x00010143, 0x1C080100, 0xC1A0B048,
    0x2A130883, 0xA1B0C85C, 0x1B0487C3, 0x08D7FBFD, 0x13704422, 0x887C59C1, 0x8DC69FF1,
    0x047E3B0D, 0x492450B9, 0x2A4F2686, 0x2112A94C, 0x5F0696CB, 0x732824C2, 0x369AC0A6,
    0xB9D4CE71, 0x9FB811B0, 0xE4FE8340, 0xD09F9137, 0x162AE0A3, 0xA9B4C89C, 0x9950A7D3,
    0xB54A8C1A, 0x3E82D4AA, 0xFA250691, 0xA056A8CF, 0x6CF22D4A, 0xD9655639, 0xA6496793,
    0x6376B905, 0x6D19B15B, 0x972B8CC6, 0x37BADD26, 0x0B7DE2EF, 0xBC452251, 0x160B0380,
    0x00001018, 0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x231087C3, 0xB1489C4A,
    0x188BC5A2, 0xC8DC6A33, 0x8FC7A3B1, 0x6C4A4320, 0xC9A4AE04, 0xAA532893, 0x9FF0646C,
    0x633097CB, 0xB2E99CCA, 0x389BCD25, 0xA924166B, 0x934FA7B2, 0x1D0A452C, 0xD1A8B44A,
    0x2A9348A3, 0xA9B4CA5D, 0x010185D3, 0x00000000

  EW_BITMAP_CLUT_EMPTY( ResourcesNavigationIconsMedium, Default )

  EW_BITMAPS_TABLE( ResourcesNavigationIconsMedium )
    EW_BITMAP( ResourcesNavigationIconsMedium, Default )
EW_END_OF_BITMAP_RES( ResourcesNavigationIconsMedium )

/* Table with links to derived variants of the bitmap resource : 'Resources::NavigationIconsMedium' */
EW_RES_WITHOUT_VARIANTS( ResourcesNavigationIconsMedium )

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMonday = { _StringsDefault0, 0x0003 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesTuesday = { _StringsDefault0, 0x000D };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesWednesday = { _StringsDefault0, 0x0018 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesThursday = { _StringsDefault0, 0x0025 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesFriday = { _StringsDefault0, 0x0031 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSaturday = { _StringsDefault0, 0x003B };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSunday = { _StringsDefault0, 0x0047 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJanuary = { _StringsDefault0, 0x0051 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesFebruary = { _StringsDefault0, 0x005C };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMarch = { _StringsDefault0, 0x0068 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesApril = { _StringsDefault0, 0x0071 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMay = { _StringsDefault0, 0x007A };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJune = { _StringsDefault0, 0x0081 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJuly = { _StringsDefault0, 0x0089 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesAugust = { _StringsDefault0, 0x0091 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSeptember = { _StringsDefault0, 0x009B };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesOctober = { _StringsDefault0, 0x00A8 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesNovember = { _StringsDefault0, 0x00B3 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesDecember = { _StringsDefault0, 0x00BF };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMondayAbbr = { _StringsDefault0, 0x00CB };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesTuesdayAbbr = { _StringsDefault0, 0x00D2 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesWednesdayAbbr = { _StringsDefault0, 0x00D9 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesThursdayAbbr = { _StringsDefault0, 0x00E0 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesFridayAbbr = { _StringsDefault0, 0x00E7 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSaturdayAbbr = { _StringsDefault0, 0x00EE };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSundayAbbr = { _StringsDefault0, 0x00F5 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJanuaryAbbr = { _StringsDefault0, 0x00FC };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesFebruaryAbbr = { _StringsDefault0, 0x0103 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMarchAbbr = { _StringsDefault0, 0x010A };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesAprilAbbr = { _StringsDefault0, 0x0111 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesMayAbbr = { _StringsDefault0, 0x0118 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJuneAbbr = { _StringsDefault0, 0x011F };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesJulyAbbr = { _StringsDefault0, 0x0126 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesAugustAbbr = { _StringsDefault0, 0x012D };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesSeptemberAbbr = { _StringsDefault0, 0x0134 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesOctoberAbbr = { _StringsDefault0, 0x013B };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesNovemberAbbr = { _StringsDefault0, 0x0142 };

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
const XStringRes ResourcesDecemberAbbr = { _StringsDefault0, 0x0149 };

/* Embedded Wizard */
