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
#include "_GraphicsArcPath.h"
#include "_GraphicsCanvas.h"
#include "_GraphicsPath.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "Graphics.h"
#include "Resources.h"
#include "Views.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x00000400, /* ratio 46.88 % */
  0xB8007D00, 0x80000452, 0x00C20021, 0xC4000370, 0x006F0042, 0x400869D0, 0x0032800E,
  0x034800E6, 0x8A45CF40, 0x8000F118, 0x46CD8003, 0x4E4E6300, 0xA2879934, 0x151389C2,
  0x91F344A6, 0x1533C50C, 0x40048E13, 0x8D4A6393, 0xC432310E, 0x462191C9, 0x02546287,
  0xA493A368, 0x06358346, 0x04E00416, 0x0A9CCDE2, 0x9944F649, 0xA243E411, 0x48028F0F,
  0xD32271CA, 0xAA252EAC, 0x26B3C995, 0x1D1189C5, 0x940ADC4E, 0x768642AB, 0x283D3E74,
  0xB69520D5, 0x05DE5362, 0x5D615790, 0x9A15758E, 0xA15188B4, 0x8B0E96CD, 0xF8A8E52A,
  0x368C47A4, 0x4A51DE1D, 0x8742A1D3, 0xDDA22D38, 0xD0EBD3FA, 0x92294693, 0xA7767B55,
  0x0D049341, 0x9169D15E, 0xADA510A4, 0x9000A249, 0x92446282, 0xE5AB4518, 0x43A19118,
  0x7F55869A, 0xA97BEA14, 0x299889D2, 0x51F3ACDA, 0x34EB55AE, 0x4AEE32C5, 0xC944E4DD,
  0x6CE73D38, 0x78231CAC, 0xDA399B8A, 0xE2DCDE0C, 0xB33893F1, 0xD3AEC4E7, 0x001A0038,
  0x01B00060, 0x92450720, 0x9184891C, 0xE4182E49, 0x4C006C5C, 0x1F495761, 0x739656DD,
  0x9BD6ED68, 0x247DBF43, 0xD95677DC, 0x48575C87, 0x8510F72D, 0x1D083DCE, 0x08919900,
  0xC1627500, 0x146016F5, 0x7751A76D, 0xD6957DC3, 0xF58D9F76, 0x9D9A79D1, 0x567CDEB4,
  0x4716D7B9, 0xE2B859F1, 0xA7E1F686, 0x39FE7F1F, 0x02002004, 0x41A05832, 0x04482111,
  0xF66C0056, 0xAC006917, 0xA8130001, 0x7361247D, 0x618951C0, 0x586D1471, 0x31185257,
  0x22532207, 0x74D0E5BA, 0x9715A9D5, 0xE439F25A, 0x620C89E2, 0x34789A78, 0x7865D796,
  0x2428EA55, 0x156E3E88, 0x0DF47B24, 0xFB9190E6, 0x49DFF7F5, 0xE068124D, 0x000101A4,
  0x00000000
};

/* Constant values used in this 'C' module only. */
static const XPoint _Const0000 = { 0, 0 };
static const XStringRes _Const0001 = { _StringsDefault0, 0x0003 };
static const XRect _Const0002 = {{ 0, 0 }, { 0, 0 }};
static const XStringRes _Const0003 = { _StringsDefault0, 0x0041 };
static const XStringRes _Const0004 = { _StringsDefault0, 0x0076 };
static const XStringRes _Const0005 = { _StringsDefault0, 0x00B9 };
static const XStringRes _Const0006 = { _StringsDefault0, 0x0125 };
static const XStringRes _Const0007 = { _StringsDefault0, 0x0192 };
static const XPoint _Const0008 = { 1, 1 };
static const XPoint _Const0009 = { 2, 2 };

/* Initializer for the class 'Graphics::Canvas' */
void GraphicsCanvas__Init( GraphicsCanvas _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  ResourcesBitmap__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( GraphicsCanvas );

  /* Call the user defined constructor */
  GraphicsCanvas_Init( _this, aArg );
}

/* Re-Initializer for the class 'Graphics::Canvas' */
void GraphicsCanvas__ReInit( GraphicsCanvas _this )
{
  /* At first re-initialize the super class ... */
  ResourcesBitmap__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Graphics::Canvas' */
void GraphicsCanvas__Done( GraphicsCanvas _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( GraphicsCanvas );

  /* Call the user defined destructor of the class */
  GraphicsCanvas_Done( _this );

  /* Don't forget to deinitialize the super class ... */
  ResourcesBitmap__Done( &_this->_Super );
}

/* 'C' function for method : 'Graphics::Canvas.Done()' */
void GraphicsCanvas_Done( GraphicsCanvas _this )
{
  if ( _this->attached )
    GraphicsCanvas_DetachBitmap( _this );
}

/* 'C' function for method : 'Graphics::Canvas.Init()' */
void GraphicsCanvas_Init( GraphicsCanvas _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  _this->Super1.Mutable = 1;
}

/* 'C' function for method : 'Graphics::Canvas.OnSetFrameSize()' */
void GraphicsCanvas_OnSetFrameSize( GraphicsCanvas _this, XPoint value )
{
  XHandle handle;

  if (( value.X <= 0 ) || ( value.Y <= 0 ))
    value = _Const0000;

  if ( !EwCompPoint( value, _this->Super1.FrameSize ))
    return;

  if ( _this->attached )
  {
    EwThrow( EwLoadString( &_Const0001 ));
    return;
  }

  _this->Super1.FrameSize = value;
  _this->Super1.Animated = (XBool)(((( _this->Super1.FrameSize.X > 0 ) && ( _this->Super1.FrameSize.Y 
  > 0 )) && ( _this->Super1.FrameDelay > 0 )) && ( _this->Super1.NoOfFrames > 1 ));

  if ( _this->Super1.bitmap == 0 )
    return;

  handle = _this->Super1.bitmap;
  EwFreeBitmap((XBitmap*)handle );
  _this->Super1.bitmap = 0;
}

/* 'C' function for method : 'Graphics::Canvas.Update()' */
void GraphicsCanvas_Update( GraphicsCanvas _this )
{
  if ((( _this->Super1.bitmap == 0 ) && ( _this->Super1.FrameSize.X > 0 )) && ( 
      _this->Super1.FrameSize.Y > 0 ))
  {
    XPoint frameSize = _this->Super1.FrameSize;
    XInt32 noOfFrames = _this->Super1.NoOfFrames;
    XInt32 frameDelay = _this->Super1.FrameDelay;
    XHandle handle = 0;
    {
      handle = (XHandle)EwCreateBitmap( EW_PIXEL_FORMAT_NATIVE, frameSize, 
                                        frameDelay, noOfFrames );
    }
    _this->Super1.bitmap = handle;

    if ( _this->Super1.bitmap == 0 )
    {
      _this->Super1.FrameSize = _Const0000;
      _this->Super1.FrameDelay = 0;
      _this->Super1.NoOfFrames = 1;
    }

    _this->InvalidArea = EwNewRect2Point( _Const0000, _this->Super1.FrameSize );
  }

  if ( !EwIsRectEmpty( _this->InvalidArea ))
  {
    if (( _this->Super1.FrameSize.X > 0 ) && ( _this->Super1.FrameSize.Y > 0 ))
      EwSignal( _this->OnDraw, ((XObject)_this ));

    _this->InvalidArea = _Const0002;
  }
}

/* The method DetachBitmap() exists for the integration purpose with the underlying 
   target system. You will never need to invoke this method directly from your GUI 
   application. The method is invoked after the screen update is finished and the 
   canvas object should be detached from the framebuffer. */
GraphicsCanvas GraphicsCanvas_DetachBitmap( GraphicsCanvas _this )
{
  if ( !_this->attached )
  {
    EwThrow( EwLoadString( &_Const0003 ));
    return 0;
  }

  _this->Super1.bitmap = 0;
  _this->attached = 0;
  _this->Super1.FrameSize = _Const0000;
  _this->Super1.FrameDelay = 0;
  _this->Super1.NoOfFrames = 1;
  _this->Super1.Animated = 0;
  return _this;
}

/* Wrapper function for the non virtual method : 'Graphics::Canvas.DetachBitmap()' */
GraphicsCanvas GraphicsCanvas__DetachBitmap( void* _this )
{
  return GraphicsCanvas_DetachBitmap((GraphicsCanvas)_this );
}

/* The method AttachBitmap() exists for the integration purpose with the underlying 
   target system. You will never need to invoke this method directly from your GUI 
   application. The method is invoked at the beginning of the screen update.
   The method connects the canvas object with the framebuffer. */
GraphicsCanvas GraphicsCanvas_AttachBitmap( GraphicsCanvas _this, XHandle aBitmap )
{
  XInt32 noOfFrames;
  XPoint frameSize;
  XInt32 frameDelay;

  if ( _this->Super1.bitmap != 0 )
  {
    EwThrow( EwLoadString( &_Const0004 ));
    return 0;
  }

  if ( aBitmap == 0 )
    return _this;

  _this->Super1.bitmap = aBitmap;
  _this->attached = 1;
  noOfFrames = 1;
  frameSize = _Const0000;
  frameDelay = 0;
  {
    XBitmap* bmp = (XBitmap*)aBitmap;

    noOfFrames = bmp->NoOfFrames;
    frameSize  = bmp->FrameSize;
    frameDelay = bmp->FrameDelay;
  }
  _this->Super1.NoOfFrames = noOfFrames;
  _this->Super1.FrameSize = frameSize;
  _this->Super1.FrameDelay = frameDelay;
  _this->Super1.Animated = (XBool)(( _this->Super1.FrameDelay > 0 ) && ( _this->Super1.NoOfFrames 
  > 1 ));
  return _this;
}

/* Wrapper function for the non virtual method : 'Graphics::Canvas.AttachBitmap()' */
GraphicsCanvas GraphicsCanvas__AttachBitmap( void* _this, XHandle aBitmap )
{
  return GraphicsCanvas_AttachBitmap((GraphicsCanvas)_this, aBitmap );
}

/* The method DrawText() draws the text row passed in the parameter aString into 
   the canvas. The font to draw the text is passed in the parameter aFont. The parameter 
   aOffset determines within aString the sign to start the drawing operation. If 
   aOffset is zero, the operation starts with the first sign. The parameter aCount 
   determines the max. number of following sigs to draw. If aCount is -1, all signs 
   until the end of the string are drawn. 
   The area to draw the text is determined by the parameter aDstRect. The parameter 
   aOrientation controls the rotation of the text. The parameter aSrcPos determines 
   the base line position of the text relative to a corner of aDstRect, which by 
   taking in account the specified text orientation serves as the origin for the 
   draw operation. For example, if the parameter aOrientation is Views::Orientation.Rotated_270, 
   the text is drawn aSrcPos pixel relative to the bottom-right corner of aDstRect. 
   The parameter aMinWidth determines the min. width in pixel of the drawn text 
   row regardless of the specified rotation. If necessary the space signs within 
   the text will be stretched to fill this area. The parameters aColorTL, aColorTR, 
   aColorBL, aColorBR determine the colors at the corresponding corners of the aDstRect 
   area.
   The parameter aClip limits the drawing operation. Pixel lying outside this area 
   remain unchanged. The last aBlend parameter controls the mode how drawn pixel 
   are combined with the pixel already existing in the destination bitmap. If aBlend 
   is 'true', the drawn pixel are alpha-blended with the background, otherwise the 
   drawn pixel will overwrite the old content. */
void GraphicsCanvas_DrawText( GraphicsCanvas _this, XRect aClip, ResourcesFont aFont, 
  XString aString, XInt32 aOffset, XInt32 aCount, XRect aDstRect, XPoint aSrcPos, 
  XInt32 aMinWidth, XEnum aOrientation, XColor aColorTL, XColor aColorTR, XColor 
  aColorBR, XColor aColorBL, XBool aBlend )
{
  XInt32 orient;
  XInt32 dstFrameNo;
  XHandle dstBitmap;
  XHandle srcFont;
  XRect tempRect;

  if ( _this->Super1.bitmap == 0 )
    ResourcesBitmap__Update( _this );

  if ( _this->Super1.bitmap == 0 )
    return;

  if ( aOffset < 0 )
    aOffset = 0;

  if ((( aFont == 0 ) || ( aFont->font == 0 )) || (( aOffset > 0 ) && ( aOffset 
      >= EwGetStringLength( aString ))))
    return;

  orient = 0;

  if ( aOrientation == ViewsOrientationRotated_90 )
    orient = 90;
  else
    if ( aOrientation == ViewsOrientationRotated_180 )
      orient = 180;
    else
      if ( aOrientation == ViewsOrientationRotated_270 )
        orient = 270;

  dstFrameNo = _this->DstFrameNr;
  dstBitmap = _this->Super1.bitmap;
  srcFont = aFont->font;
  tempRect = aClip;
  {
    EwDrawText((XBitmap*)dstBitmap, (XFont*)srcFont, aString + aOffset, aCount,
                dstFrameNo, tempRect, aDstRect, aSrcPos, aMinWidth, orient, aColorTL, aColorTR,
                aColorBR, aColorBL, aBlend );
  }
}

/* The method DrawBitmapFrame() draws a free scalable frame by composing it of bitmap 
   segments. These segments are used to draw the frame's corners, to fill its edges 
   and to fill its interior area. The bitmap has thus to contain nine equal segments 
   arranged in three rows and three columns. The top-left segment e.g. is used to 
   draw the top-left corner of the frame. In contrast, the top-middle segment corresponds 
   to the frame's top edge. If the edge is wider than the segment, multiple copies 
   of the segment are used to fill the entire edge. In this manner the entire frame 
   is composed by simply copying bitmap segments.
   The bitmap is specified in the parameter aBitmap. In case of a multi-frame bitmap 
   the desired frame can be selected in the parameter aFrameNr. The resulting size 
   of the drawn frame is specified by aDstRect parameter. The parameter aEdges control 
   which edges are drawn and which are omitted. Optionally the copied pixel can 
   be modulated by a color gradient specified by the four parameters aColorTL .. 
   aColorBL.
   An additional clipping area aClip limits the operation. All pixel lying outside 
   this area will not be drawn. The last aBlend parameter controls the mode how 
   drawn pixel are combined with the pixel already existing in the destination bitmap. 
   If aBlend is 'true', the drawn pixel are alpha-blended with the background, otherwise 
   the drawn pixel will overwrite the old content. */
void GraphicsCanvas_DrawBitmapFrame( GraphicsCanvas _this, XRect aClip, ResourcesBitmap 
  aBitmap, XInt32 aFrameNr, XRect aDstRect, XSet aEdges, XColor aColorTL, XColor 
  aColorTR, XColor aColorBR, XColor aColorBL, XBool aBlend )
{
  XHandle dstBitmap;
  XHandle srcBitmap;
  XInt32 dstFrameNo;
  XRect srcRect;
  XBool left;
  XBool top;
  XBool right;
  XBool bottom;
  XBool interior;

  if ( _this->Super1.bitmap == 0 )
    ResourcesBitmap__Update( _this );

  if ( _this->Super1.bitmap == 0 )
    return;

  if ((((( aBitmap == 0 ) || ( aBitmap->bitmap == 0 )) || !aEdges ) || ( aFrameNr 
      < 0 )) || ( aFrameNr >= aBitmap->NoOfFrames ))
    return;

  dstBitmap = _this->Super1.bitmap;
  srcBitmap = aBitmap->bitmap;
  dstFrameNo = _this->DstFrameNr;
  srcRect = EwNewRect2Point( _Const0000, aBitmap->FrameSize );
  left = (( aEdges & GraphicsEdgesLeft ) == GraphicsEdgesLeft );
  top = (( aEdges & GraphicsEdgesTop ) == GraphicsEdgesTop );
  right = (( aEdges & GraphicsEdgesRight ) == GraphicsEdgesRight );
  bottom = (( aEdges & GraphicsEdgesBottom ) == GraphicsEdgesBottom );
  interior = (( aEdges & GraphicsEdgesInterior ) == GraphicsEdgesInterior );
  {
    EwDrawBitmapFrame((XBitmap*)dstBitmap, (XBitmap*)srcBitmap, dstFrameNo, aFrameNr,
                       aClip, aDstRect, srcRect, left, top, right, bottom,
                       interior, aColorTL, aColorTR, aColorBR, aColorBL, aBlend );
  }
}

/* The method StrokePath() strokes within the rectangular area aDstRect of canvas 
   a path determined by data stored in the Graphics::Path object aPath. All path 
   coordinates are assumed as being relative to the top-left corner of the aDstRect 
   area, or if the parameter aFlipY is 'true', relative to the bottom-left corner. 
   With the parameter aFlipY equal 'true' the path coordinates are mirrored vertically 
   causing the positive Y-axis to point upwards. With the parameter aOffset the 
   origin of the path coordinate system can be moved within aDstRect. Accordingly 
   modifying this value scrolls the displayed path content.
   The thickness of the path is determined by the parameter aWidth and is expressed 
   in pixel. The parameters aStartCapStyle and aEndCapStyle determine how the start/end 
   position of the path are displayed. The possible values are specified in Graphics::PathCap. 
   Furthermore the parameter aJoinStyle controls how the line segments of the path 
   are connected together. Here the possible values are specified in Graphics::PathJoin. 
   Please note, if aJoinStyle is Graphics::PathJoin.Miter, the additional parameter 
   aMiterLimit determines the max. ratio between the length of the miter and the 
   half of the path thickness (aWidth / 2). If this limit is exceeded, the affected 
   corner is joined with an ordinary bevel (Graphics::PathJoin.Bevel) instead of 
   miter.
   The parameters aColorTL, aColorTR, aColorBL, aColorBR determine the colors at 
   the corresponding corners of the aDstRect area. If the parameter aAntialiased 
   is 'true', the method applies antialiasing while rasterizing the path pixel.
   The parameter aClip limits the drawing operation. Pixel lying outside this area 
   remain unchanged. The aBlend parameter controls the mode how drawn pixel are 
   combined with the pixel already existing in the destination bitmap. If aBlend 
   is 'true', the drawn pixel are alpha-blended with the background, otherwise the 
   drawn pixel will overwrite the old content. */
void GraphicsCanvas_StrokePath( GraphicsCanvas _this, XRect aClip, GraphicsPath 
  aPath, XRect aDstRect, XBool aFlipY, XPoint aOffset, XFloat aWidth, XEnum aStartCapStyle, 
  XEnum aEndCapStyle, XEnum aJoinStyle, XFloat aMiterLimit, XColor aColorTL, XColor 
  aColorTR, XColor aColorBR, XColor aColorBL, XBool aBlend, XBool aAntialiased )
{
  XUInt32 style;
  XHandle dstBitmap;
  XInt32 dstFrameNo;
  XHandle path;

  if ( _this->Super1.bitmap == 0 )
    ResourcesBitmap__Update( _this );

  if ( _this->Super1.bitmap == 0 )
    return;

  if (( aPath == 0 ) || ( aPath->path == 0 ))
    return;

  style = 0;

  switch ( aStartCapStyle )
  {
    case GraphicsPathCapSquare :
      style = style | 1;
    break;

    case GraphicsPathCapTriangle :
      style = style | 2;
    break;

    case GraphicsPathCapRound :
      style = style | 3;
    break;

    default : 
      ;
  }

  switch ( aEndCapStyle )
  {
    case GraphicsPathCapSquare :
      style = style | 256;
    break;

    case GraphicsPathCapTriangle :
      style = style | 512;
    break;

    case GraphicsPathCapRound :
      style = style | 768;
    break;

    default : 
      ;
  }

  switch ( aJoinStyle )
  {
    case GraphicsPathJoinMiter :
      style = style | 65536;
    break;

    case GraphicsPathJoinRound :
      style = style | 131072;
    break;

    default : 
      ;
  }

  dstBitmap = _this->Super1.bitmap;
  dstFrameNo = _this->DstFrameNr;
  path = aPath->path;
  {
    EwStrokePath((XBitmap*)dstBitmap, (XPath*)path, dstFrameNo, aClip, aDstRect, aFlipY,
                  aOffset, aWidth, style, aMiterLimit, aColorTL, aColorTR, aColorBR, 
                  aColorBL, aBlend, aAntialiased );
  }
}

/* The method WarpBitmap() performs the projection of a rectangular source bitmap 
   area onto a four corner polygon within the destination canvas. The bitmap is 
   specified in the parameter aBitmap and the desired area to copy in aSrcRect. 
   In case of a multi-frame bitmap the desired frame can be selected in the parameter 
   aFrameNr.
   The destination polygon is determined by the coordinates aX1,aY1 .. aX4,aY4. 
   The coefficients aW1 .. aW4 are responsible for the perspective distortion. The 
   parameters aColor1, aColor2, aColor3, aColor4 determine the colors or opacities 
   at the corresponding corners of the polygon area. The parameter aClip limits 
   the drawing operation. Pixel lying outside this area remain unchanged. The last 
   aBlend parameter controls the mode how drawn pixel are combined with the pixel 
   already existing in the destination bitmap. If aBlend is 'true', the drawn pixel 
   are alpha-blended with the background, otherwise the drawn pixel will overwrite 
   the old content. */
void GraphicsCanvas_WarpBitmap( GraphicsCanvas _this, XRect aClip, ResourcesBitmap 
  aBitmap, XInt32 aFrameNr, XFloat aDstX1, XFloat aDstY1, XFloat aDstW1, XFloat 
  aDstX2, XFloat aDstY2, XFloat aDstW2, XFloat aDstX3, XFloat aDstY3, XFloat aDstW3, 
  XFloat aDstX4, XFloat aDstY4, XFloat aDstW4, XRect aSrcRect, XColor aColor1, XColor 
  aColor2, XColor aColor3, XColor aColor4, XBool aBlend, XBool aFilter )
{
  XFloat x1;
  XFloat x2;
  XFloat y1;
  XFloat y2;
  XHandle dstBitmap;
  XHandle srcBitmap;
  XInt32 dstFrameNr;

  if ( _this->Super1.bitmap == 0 )
    ResourcesBitmap__Update( _this );

  if ( _this->Super1.bitmap == 0 )
    return;

  if (((( aBitmap == 0 ) || ( aBitmap->bitmap == 0 )) || ( aFrameNr < 0 )) || ( 
      aFrameNr >= aBitmap->NoOfFrames ))
    return;

  x1 = aDstX1;
  x2 = aDstX2;
  y1 = aDstY1;
  y2 = aDstY2;

  if ( aDstX2 < x1 )
    x1 = aDstX2;

  if ( aDstX3 < x1 )
    x1 = aDstX3;

  if ( aDstX4 < x1 )
    x1 = aDstX4;

  if ( aDstX2 > x2 )
    x2 = aDstX2;

  if ( aDstX3 > x2 )
    x2 = aDstX3;

  if ( aDstX4 > x2 )
    x2 = aDstX4;

  if ( aDstY2 < y1 )
    y1 = aDstY2;

  if ( aDstY3 < y1 )
    y1 = aDstY3;

  if ( aDstY4 < y1 )
    y1 = aDstY4;

  if ( aDstY2 > y2 )
    y2 = aDstY2;

  if ( aDstY3 > y2 )
    y2 = aDstY3;

  if ( aDstY4 > y2 )
    y2 = aDstY4;

  if ((((( x2 - x1 ) > 4096.000000f ) || (( x2 - x1 ) < -4096.000000f )) || (( y2 
      - y1 ) > 4096.000000f )) || (( y2 - y1 ) < -4096.000000f ))
  {
    EwTrace( "%s", EwLoadString( &_Const0005 ));
    return;
  }

  dstBitmap = _this->Super1.bitmap;
  srcBitmap = aBitmap->bitmap;
  dstFrameNr = _this->DstFrameNr;
  {
    EwWarpBitmap((XBitmap*)dstBitmap, (XBitmap*)srcBitmap, dstFrameNr, aFrameNr,
                  aClip, aDstX1, aDstY1, aDstW1, aDstX2, aDstY2, aDstW2, 
                  aDstX3, aDstY3, aDstW3, aDstX4, aDstY4, aDstW4, aSrcRect, aColor1,
                  aColor2, aColor3, aColor4, aBlend, aFilter );
  }
}

/* The method ScaleBitmap() copies and scales an area of a aBitmap into the canvas. 
   The bitmap is specified in the parameter aBitmap and the desired area to copy 
   in aSrcRect. In case of a multi-frame bitmap the desired frame can be selected 
   in the parameter aFrameNr.
   The destination area in canvas is determined by the parameter aDstRect. The parameters 
   aColorTL, aColorTR, aColorBL, aColorBR determine the colors or opacities at the 
   corresponding corners of the aDstRect area.
   The parameter aClip limits the drawing operation. Pixel lying outside this area 
   remain unchanged. The last aBlend parameter controls the mode how drawn pixel 
   are combined with the pixel already existing in the destination bitmap. If aBlend 
   is 'true', the drawn pixel are alpha-blended with the background, otherwise the 
   drawn pixel will overwrite the old content. */
void GraphicsCanvas_ScaleBitmap( GraphicsCanvas _this, XRect aClip, ResourcesBitmap 
  aBitmap, XInt32 aFrameNr, XRect aDstRect, XRect aSrcRect, XColor aColorTL, XColor 
  aColorTR, XColor aColorBR, XColor aColorBL, XBool aBlend, XBool aFilter )
{
  XFloat top;
  XFloat left;
  XFloat right;
  XFloat bottom;
  XHandle dstBitmap;
  XHandle srcBitmap;
  XInt32 dstFrameNo;

  if ( _this->Super1.bitmap == 0 )
    ResourcesBitmap__Update( _this );

  if ( _this->Super1.bitmap == 0 )
    return;

  if (((( aBitmap == 0 ) || ( aBitmap->bitmap == 0 )) || ( aFrameNr < 0 )) || ( 
      aFrameNr >= aBitmap->NoOfFrames ))
    return;

  top = (XFloat)aDstRect.Point1.Y;
  left = (XFloat)aDstRect.Point1.X;
  right = (XFloat)aDstRect.Point2.X;
  bottom = (XFloat)aDstRect.Point2.Y;

  if ((((( right - left ) > 4096.000000f ) || (( right - left ) < -4096.000000f )) 
      || (( bottom - top ) > 4096.000000f )) || (( bottom - top ) < -4096.000000f ))
  {
    EwTrace( "%s", EwLoadString( &_Const0006 ));
    return;
  }

  dstBitmap = _this->Super1.bitmap;
  srcBitmap = aBitmap->bitmap;
  dstFrameNo = _this->DstFrameNr;
  {
    EwWarpBitmap((XBitmap*)dstBitmap, (XBitmap*)srcBitmap, dstFrameNo, aFrameNr, aClip,
                  left,  top,    1.0f, right, top,    1.0f,
                  right, bottom, 1.0f, left,  bottom, 1.0f,
                  aSrcRect, aColorTL, aColorTR, aColorBR, aColorBL, aBlend, aFilter );
  }
}

/* The method CopyBitmap() copies an area of a aBitmap into the canvas. The bitmap 
   is specified in the parameter aBitmap. In case of a multi-frame bitmap the desired 
   frame can be selected in the parameter aFrameNr.
   The area to copy the bitmap is determined by the parameter aDstRect. The optional 
   aSrcPos parameter determines a displacement of the bitmap within this aDstRect 
   area. The parameters aColorTL, aColorTR, aColorBL, aColorBR determine the colors 
   or opacities at the corresponding corners of the aDstRect area.
   The parameter aClip limits the drawing operation. Pixel lying outside this area 
   remain unchanged. The last aBlend parameter controls the mode how drawn pixel 
   are combined with the pixel already existing in the destination bitmap. If aBlend 
   is 'true', the drawn pixel are alpha-blended with the background, otherwise the 
   drawn pixel will overwrite the old content. */
void GraphicsCanvas_CopyBitmap( GraphicsCanvas _this, XRect aClip, ResourcesBitmap 
  aBitmap, XInt32 aFrameNr, XRect aDstRect, XPoint aSrcPos, XColor aColorTL, XColor 
  aColorTR, XColor aColorBR, XColor aColorBL, XBool aBlend )
{
  XHandle dstBitmap;
  XHandle srcBitmap;
  XInt32 dstFrameNr;

  if ( _this->Super1.bitmap == 0 )
    ResourcesBitmap__Update( _this );

  if ( _this->Super1.bitmap == 0 )
    return;

  if (((( aBitmap == 0 ) || ( aBitmap->bitmap == 0 )) || ( aFrameNr < 0 )) || ( 
      aFrameNr >= aBitmap->NoOfFrames ))
    return;

  dstBitmap = _this->Super1.bitmap;
  srcBitmap = aBitmap->bitmap;
  dstFrameNr = _this->DstFrameNr;
  {
    EwCopyBitmap((XBitmap*)dstBitmap, (XBitmap*)srcBitmap, dstFrameNr, aFrameNr,
                  aClip, aDstRect, aSrcPos, aColorTL, aColorTR, aColorBR, aColorBL,
                  aBlend );
  }
}

/* The method FillPath() fills within the rectangular area aDstRect of canvas a 
   polygon determined by data stored in the Graphics::Path object aPath. All path 
   coordinates are assumed as being relative to the top-left corner of the aDstRect 
   area, or if the parameter aFlipY is 'true', relative to the bottom-left corner. 
   With the parameter aFlipY equal 'true' the path coordinates are additionally 
   mirrored vertically causing the positive Y-axis to point upwards. With the parameter 
   aOffset the origin of the path coordinate system can be moved within aDstRect. 
   Accordingly modifying this value scrolls the displayed path content.
   The parameters aColorTL, aColorTR, aColorBL, aColorBR determine the colors at 
   the corresponding corners of the aDstRect area. If the parameter aAntialiased 
   is 'true', the method applies antialiasing while rasterizing the path pixel. 
   The parameter aFillRule controls the fill algorithm how it treats nested polygon 
   areas. The possible values are specified in Graphics::FillRule.
   The parameter aClip limits the drawing operation. Pixel lying outside this area 
   remain unchanged. The aBlend parameter controls the mode how drawn pixel are 
   combined with the pixel already existing in the destination bitmap. If aBlend 
   is 'true', the drawn pixel are alpha-blended with the background, otherwise the 
   drawn pixel will overwrite the old content. */
void GraphicsCanvas_FillPath( GraphicsCanvas _this, XRect aClip, GraphicsPath aPath, 
  XRect aDstRect, XBool aFlipY, XPoint aOffset, XColor aColorTL, XColor aColorTR, 
  XColor aColorBR, XColor aColorBL, XBool aBlend, XBool aAntialiased, XEnum aFillRule )
{
  XBool nonZeroWinding;
  XHandle dstBitmap;
  XInt32 dstFrameNo;
  XHandle path;

  if ( _this->Super1.bitmap == 0 )
    ResourcesBitmap__Update( _this );

  if ( _this->Super1.bitmap == 0 )
    return;

  if (( aPath == 0 ) || ( aPath->path == 0 ))
    return;

  nonZeroWinding = (XBool)( aFillRule == GraphicsFillRuleNonZero );
  dstBitmap = _this->Super1.bitmap;
  dstFrameNo = _this->DstFrameNr;
  path = aPath->path;
  {
    EwFillPath((XBitmap*)dstBitmap, (XPath*)path, dstFrameNo, aClip, aDstRect, aFlipY,
                aOffset, aColorTL, aColorTR, aColorBR, aColorBL, aBlend, aAntialiased,
                nonZeroWinding );
  }
}

/* The method FillRectangle() fills an area of canvas. The area is determined by 
   the parameter aDstRect. The parameters aColorTL, aColorTR, aColorBL, aColorBR 
   determine the colors at the corresponding corners of the aDstRect area.
   The parameter aClip limits the drawing operation. Pixel lying outside this area 
   remain unchanged. The last aBlend parameter controls the mode how drawn pixel 
   are combined with the pixel already existing in the destination bitmap. If aBlend 
   is 'true', the drawn pixel are alpha-blended with the background, otherwise the 
   drawn pixel will overwrite the old content. */
void GraphicsCanvas_FillRectangle( GraphicsCanvas _this, XRect aClip, XRect aDstRect, 
  XColor aColorTL, XColor aColorTR, XColor aColorBR, XColor aColorBL, XBool aBlend )
{
  XHandle dstBitmap;
  XInt32 dstFrameNo;

  if ( _this->Super1.bitmap == 0 )
    ResourcesBitmap__Update( _this );

  if ( _this->Super1.bitmap == 0 )
    return;

  dstBitmap = _this->Super1.bitmap;
  dstFrameNo = _this->DstFrameNr;
  {
    EwFillRectangle((XBitmap*)dstBitmap, dstFrameNo, aClip, aDstRect, aColorTL, aColorTR,
                     aColorBR, aColorBL, aBlend );
  }
}

/* 'C' function for method : 'Graphics::Canvas.DrawThickLine()' */
void GraphicsCanvas_DrawThickLine( GraphicsCanvas _this, XRect aClip, XPoint aDstPos1, 
  XPoint aDstPos2, XInt32 aWidth1, XInt32 aWidth2, XColor aColor1, XColor aColor2, 
  XBool aBlend )
{
  XInt32 w1;
  XInt32 w2;
  XPoint p1;
  XPoint p2;
  XFloat fw1;
  XFloat fw2;
  XFloat fp1X;
  XFloat fp1Y;
  XFloat fp2X;
  XFloat fp2Y;
  XFloat dirX;
  XFloat dirY;
  XFloat len;
  XFloat x1;
  XFloat y1;
  XFloat x2;
  XFloat y2;
  XFloat x3;
  XFloat y3;
  XFloat x4;
  XFloat y4;
  ResourcesBitmap opaqueBitmap;
  XRect srcRect;
  XHandle dstBitmap;
  XInt32 dstFrameNo;
  XHandle srcBitmap;

  if ( _this->Super1.bitmap == 0 )
    ResourcesBitmap__Update( _this );

  if ( _this->Super1.bitmap == 0 )
    return;

  w1 = aWidth1;
  w2 = aWidth2;
  p1 = aDstPos1;
  p2 = aDstPos2;

  if ( !EwCompPoint( p1, p2 ))
    return;

  if ( w1 <= 1 )
    w1 = 1;

  if ( w2 <= 1 )
    w2 = 1;

  fw1 = (XFloat)w1 / 2.000000f;
  fw2 = (XFloat)w2 / 2.000000f;
  fp1X = (XFloat)p1.X;
  fp1Y = (XFloat)p1.Y;
  fp2X = (XFloat)p2.X;
  fp2Y = (XFloat)p2.Y;
  dirX = fp2X - fp1X;
  dirY = fp2Y - fp1Y;
  len = EwMathSqrt(( dirX * dirX ) + ( dirY * dirY ));

  if ((( w1 > 4096 ) || ( w2 > 4096 )) || ( len > 4096.000000f ))
  {
    EwTrace( "%s", EwLoadString( &_Const0007 ));
    return;
  }

  dirX = dirX / len;
  dirY = dirY / len;
  x1 = fp1X + ( dirY * fw1 );
  y1 = fp1Y - ( dirX * fw1 );
  x2 = fp2X + ( dirY * fw2 );
  y2 = fp2Y - ( dirX * fw2 );
  x3 = fp2X - ( dirY * fw2 );
  y3 = fp2Y + ( dirX * fw2 );
  x4 = fp1X - ( dirY * fw1 );
  y4 = fp1Y + ( dirX * fw1 );
  opaqueBitmap = EwLoadResource( &ResourcesWhiteBitmapStripe, ResourcesBitmap );
  srcRect = EwNewRect2Point( _Const0008, EwMovePointNeg( opaqueBitmap->FrameSize, 
  _Const0009 ));
  dstBitmap = _this->Super1.bitmap;
  dstFrameNo = _this->DstFrameNr;
  srcBitmap = opaqueBitmap->bitmap;
  {
    EwWarpBitmap((XBitmap*)dstBitmap, (XBitmap*)srcBitmap, dstFrameNo, 0, aClip,
                  x1, y1, 1.0, x2, y2, 1.0, x3, y3, 1.0, x4, y4, 1.0, 
                  srcRect, aColor1, aColor2, aColor2, aColor1, aBlend, 0 );
  }
}

/* The method DrawLine() draws a line into the canvas. The line will be drawn from 
   the aDstPos1 point to the aDstPos2 point with the given colors aColor1 and aColor2.
   The parameter aClip limits the drawing operation. Pixel lying outside this area 
   remain unchanged. The last aBlend parameter controls the mode how drawn pixel 
   are combined with the pixel already existing in the destination bitmap. If aBlend 
   is 'true', the drawn pixel are alpha-blended with the background, otherwise the 
   drawn pixel will overwrite the old content. */
void GraphicsCanvas_DrawLine( GraphicsCanvas _this, XRect aClip, XPoint aDstPos1, 
  XPoint aDstPos2, XColor aColor1, XColor aColor2, XBool aBlend )
{
  XHandle dstBitmap;
  XInt32 dstFrameNo;

  if ( _this->Super1.bitmap == 0 )
    ResourcesBitmap__Update( _this );

  if ( _this->Super1.bitmap == 0 )
    return;

  dstBitmap = _this->Super1.bitmap;
  dstFrameNo = _this->DstFrameNr;
  {
    EwDrawLine((XBitmap*)dstBitmap, dstFrameNo, aClip, aDstPos1, aDstPos2, aColor1,
                aColor2, aBlend );
  }
}

/* Variants derived from the class : 'Graphics::Canvas' */
EW_DEFINE_CLASS_VARIANTS( GraphicsCanvas )
EW_END_OF_CLASS_VARIANTS( GraphicsCanvas )

/* Virtual Method Table (VMT) for the class : 'Graphics::Canvas' */
EW_DEFINE_CLASS( GraphicsCanvas, ResourcesBitmap, OnDraw, OnDraw, InvalidArea, InvalidArea, 
                 InvalidArea, "Graphics::Canvas" )
  GraphicsCanvas_OnSetFrameSize,
  GraphicsCanvas_Update,
EW_END_OF_CLASS( GraphicsCanvas )

/* Initializer for the class 'Graphics::Path' */
void GraphicsPath__Init( GraphicsPath _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( GraphicsPath );
}

/* Re-Initializer for the class 'Graphics::Path' */
void GraphicsPath__ReInit( GraphicsPath _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Graphics::Path' */
void GraphicsPath__Done( GraphicsPath _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( GraphicsPath );

  /* Call the user defined destructor of the class */
  GraphicsPath_Done( _this );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Graphics::Path.onUpdate()' */
void GraphicsPath_onUpdate( GraphicsPath _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwNotifyObjObservers((XObject)_this, 0 );
}

/* 'C' function for method : 'Graphics::Path.Done()' */
void GraphicsPath_Done( GraphicsPath _this )
{
  XHandle handle;

  if ( _this->path == 0 )
    return;

  handle = _this->path;
  EwFreePath((XPath*)handle );
  _this->path = 0;
}

/* The method AddArc() adds to the sub-path a new elliptical arc composed of exact 
   aNoOfEdges straight line segments. The more edges the more smooth the resulting 
   curve. The center of the ellipse is determined in the parameters aCenterX, aCenterY. 
   The parameters aRadiusX and aRadiusY determine the horizontal and vertical radius 
   of the ellipse. The start and the end angle of the arc is determined in the parameters 
   aStartAngle and aEndAngle, both expressed in degree and measured clockwise relative 
   to the positive X-axis of the path coordinate system.
   If the absolute difference between aStartAngle and aEndAngle is equal to or greater 
   than 360.0, the method calculates with a closed ellipse instead of an opened 
   arc. If the parameters aRadiusX and aRadiusY are equal, the method calculates 
   with a circle instead of an ellipse.
   If the affected sub-path contains already curve information, the end of the last 
   existing segment and the start of the new arc (resulting from the parameter aStartAngle) 
   are connected with an additional straight line segment. Similarly, if start position 
   for the sub-path has been specified by using the @Begin() method, an additional 
   line segment is added to connect the arc with the given sub-path start position. 
   After the elliptical arc curve is added, the sub-path actual position is adjusted 
   to refer to the end of the last line segment of the curve (resulting from the 
   parameter aEndAngle).
   The specified position aX, aY is relative to the origin of the path coordinate 
   system. If the path own matrix is not an identity matrix, the corresponding transformations 
   are applied on the resulting arc segments before storing them in the sub-path. 
   See methods @Translate(), @Scale() and @Rotate().
   The method returns the index of the node within the sub-path where the start 
   position of the first arc line segment has been stored (parameter aStartAngle). 
   Knowing this index and the number of segments the arc curve is composed of (aNoOfEdges), 
   the position of every arc line segment can be modified later by using the method 
   @SetNode().
   If the sub-path has not been initialized previously by using the method @InitSubPath(), 
   the sub-path has been closed by @Close() method or the memory reserved for the 
   sub-path (see parameter aMaxNoOfEdges in the method @InitSubPath()) is occupied 
   by other edges so no sufficient space is available to accommodate aNoOfEdges 
   new segments, the method fails and returns 0.
   The affected sub-path is determined in the parameter aSubPathNo. Within the path 
   all existing sub-paths are numbered starting with the value 0 for the first sub-path, 
   1 for the second and so far. The maximum number of sub-paths the path can manage 
   is determined by the method @SetMaxNoOfSubPaths(). */
XInt32 GraphicsPath_AddArc( GraphicsPath _this, XInt32 aSubPathNo, XFloat aCenterX, 
  XFloat aCenterY, XFloat aRadiusX, XFloat aRadiusY, XFloat aStartAngle, XFloat 
  aEndAngle, XInt32 aNoOfEdges )
{
  XHandle handle;
  XInt32 result;

  EwPostSignal( EwNewSlot( _this, GraphicsPath_onUpdate ), ((XObject)_this ));
  handle = _this->path;
  result = 0;
  {
    result = EwAddSubPathArc((XPath*)handle, aSubPathNo, aCenterX, aCenterY,
                              aRadiusX, aRadiusY, aStartAngle, aEndAngle,
                              aNoOfEdges );
  }
  return result;
}

/* The method AddBezier3() appends between the actual position of the sub-path and 
   the position aX, aY a new cubic Bézier curve composed of exact aNoOfEdges straight 
   line segments. The more edges the more smooth the resulting curve. The both Bézier 
   curve control points are determined in the parameters aCP1X, aCP1Y and aCP2X, 
   aCP2Y. After the Bézier curve is added, the sub-path actual position is adjusted 
   to refer to the end of the last line segment of the curve (position aX, aY).
   Within a sub-path the actual position is either the end position of the preceding 
   sub-path segment or it is the value specified in the invocation of the @Begin() 
   method. If the sub-path is empty and no start position has been specified in 
   the preceding @Begin() invocation, the method assumes the origin of the path 
   coordinate system (X=0, Y=0) as the start position for the new curve.
   The specified positions aCP1X, aCP1Y, aCP2X, aCP2, aX, aY are relative to the 
   origin of the path coordinate system. If the path own matrix is not an identity 
   matrix, the corresponding transformations are applied on the positions before 
   calculating the Bézier curve and storing the resulting line segments it in the 
   sub-path. See methods @Translate(), @Scale() and @Rotate().
   The method returns the index of the node within the sub-path where the end position 
   of the first Bézier line segment has been stored. Knowing this index and the 
   number of segments the Bézier curve is composed of (parameter aNoOfEdges), the 
   position of every Bézier line segment can be modified later by using the method 
   @SetNode().
   If the sub-path has not been initialized previously by using the method @InitSubPath(), 
   the sub-path has been closed by @Close() method or the memory reserved for the 
   sub-path (see parameter aMaxNoOfEdges in the method @InitSubPath()) is occupied 
   by other edges so no sufficient space is available to accommodate aNoOfEdges 
   new segments, the method fails and returns 0.
   The affected sub-path is determined in the parameter aSubPathNo. Within the path 
   all existing sub-paths are numbered starting with the value 0 for the first sub-path, 
   1 for the second and so far. The maximum number of sub-paths the path can manage 
   is determined by the method @SetMaxNoOfSubPaths(). */
XInt32 GraphicsPath_AddBezier3( GraphicsPath _this, XInt32 aSubPathNo, XFloat aCP1X, 
  XFloat aCP1Y, XFloat aCP2X, XFloat aCP2Y, XFloat aX, XFloat aY, XInt32 aNoOfEdges )
{
  XHandle handle;
  XInt32 result;

  EwPostSignal( EwNewSlot( _this, GraphicsPath_onUpdate ), ((XObject)_this ));
  handle = _this->path;
  result = 0;
  {
    result = EwAddSubPathBezier3((XPath*)handle, aSubPathNo, aCP1X, aCP1Y, aCP2X, aCP2Y,
                                  aX, aY, aNoOfEdges );
  }
  return result;
}

/* The method AddLine() appends between the actual position of the sub-path and 
   the position aX, aY a new straight line segment consisting of one edge. Then 
   the sub-path actual position is adjusted to refer to the end of the just added 
   line segment (aX, aY).
   Within a sub-path the actual position is either the end position of the preceding 
   sub-path segment or it is the value specified in the invocation of the @Begin() 
   method. If the sub-path is empty and no start position has been specified in 
   the preceding @Begin() invocation, the method assumes the origin of the path 
   coordinate system (X=0, Y=0) as the start position for the new line segment.
   The specified position aX, aY is relative to the origin of the path coordinate 
   system. If the path own matrix is not an identity matrix, the corresponding transformations 
   are applied on the position before storing it in the sub-path. See methods @Translate(), 
   @Scale() and @Rotate().
   The method returns the index of the node within the sub-path where the end position 
   of the new line segment has been stored. Knowing this index, the position can 
   be modified later by using the method @SetNode().
   If the sub-path has not been initialized previously by using the method @InitSubPath(), 
   the sub-path has been closed by @Close() method or the entire memory reserved 
   for the sub-path (see parameter aMaxNoOfEdges in the method @InitSubPath()) is 
   already occupied by other edges, the method fails and returns 0.
   The affected sub-path is determined in the parameter aSubPathNo. Within the path 
   all existing sub-paths are numbered starting with the value 0 for the first sub-path, 
   1 for the second and so far. The maximum number of sub-paths the path can manage 
   is determined by the method @SetMaxNoOfSubPaths(). */
XInt32 GraphicsPath_AddLine( GraphicsPath _this, XInt32 aSubPathNo, XFloat aX, XFloat 
  aY )
{
  XHandle handle;
  XInt32 result;

  EwPostSignal( EwNewSlot( _this, GraphicsPath_onUpdate ), ((XObject)_this ));
  handle = _this->path;
  result = 0;
  result = EwAddSubPathLine((XPath*)handle, aSubPathNo, aX, aY );
  return result;
}

/* The method Close() marks the affected sub-path as closed. The method verifies 
   whether the first and the last position of the sub-path are equal and if this 
   is not the case, adds an additional straight line segment to the sub-path in 
   order to connect them together.
   Once the method is invoked, no additional path information can be added to the 
   affected sub-path unless it is initialized or cleared again by using the method 
   @InitSubPath() or @Begin(). If the affected sub-path is empty, the method returns 
   without any effect.
   Within the path all existing sub-paths are numbered starting with the value 0 
   for the first sub-path, 1 for the second and so far. The maximum number of sub-paths 
   the path can manage is determined by the method @SetMaxNoOfSubPaths(). */
void GraphicsPath_Close( GraphicsPath _this, XInt32 aSubPathNo )
{
  XHandle handle;

  EwPostSignal( EwNewSlot( _this, GraphicsPath_onUpdate ), ((XObject)_this ));
  handle = _this->path;
  EwCloseSubPath((XPath*)handle, aSubPathNo );
}

/* The method Begin() sets the parameters aX, aY as the start position for the sub-path 
   with the specified number aSubPathNo. The affected sub-path has to be initialized 
   previously by invoking the method @InitSubPath(). Beginning with the specified 
   position, the path can be filled with curve data by using methods like @AddLine(), 
   @AddBezier2(), @AddArc(), etc.
   The specified position aX, aY is relative to the origin of the path coordinate 
   system. If the path own matrix is not an identity matrix, the corresponding transformations 
   are applied on the position before storing it in the sub-path. See methods @Translate(), 
   @Scale() and @Rotate().
   Every sub-path contains exact one start position. Calling this method for a sub-path 
   being already filled with edge coordinates will clear the actual sub-path as 
   if the @InitSubPath() method has been called before.
   Within the path all existing sub-paths are numbered starting with the value 0 
   for the first sub-path, 1 for the second and so far. The maximum number of sub-paths 
   the path can manage is determined by the method @SetMaxNoOfSubPaths(). */
void GraphicsPath_Begin( GraphicsPath _this, XInt32 aSubPathNo, XFloat aX, XFloat 
  aY )
{
  XHandle handle;

  EwPostSignal( EwNewSlot( _this, GraphicsPath_onUpdate ), ((XObject)_this ));
  handle = _this->path;
  EwBeginSubPath((XPath*)handle, aSubPathNo, aX, aY );
}

/* The method InitSubPath() prepares the sub-path with the number aSubPathNo to 
   be able to store up to aMaxNoOfEdges path edges (straight line segments). With 
   this operation memory for the sub-path data is reserved. Initially the just initialized 
   sub-path is considered as being empty. To fill the sub-path with data use the 
   methods like @AddLine(), @AddBezier2(), @AddArc(), etc.
   If the affected sub-path has been already initialized in the past, the old information 
   is discarded before initializing the new sub-path. Passing 0 (zero) in the parameter 
   aMaxNoOfEdges results in the old sub-path data being simply released without 
   allocating the memory for the new sub-path.
   If successful, the method returns 'true'. If there is no memory available for 
   the specified number of edges, the method fails and 'false' is returned leaving 
   the sub-path not initialized. Similarly, trying to initialize a sub-path not 
   existing in the path causes the method to return 'false'.
   Within the path all existing sub-paths are numbered starting with the value 0 
   for the first sub-path, 1 for the second and so far. The maximum number of sub-paths 
   the path can manage is determined by the method @SetMaxNoOfSubPaths(). */
XBool GraphicsPath_InitSubPath( GraphicsPath _this, XInt32 aSubPathNo, XInt32 aMaxNoOfEdges )
{
  XHandle handle;
  XBool result;

  if (( _this->path == 0 ) && ( aSubPathNo == 0 ))
    GraphicsPath_SetMaxNoOfSubPaths( _this, 1 );

  EwPostSignal( EwNewSlot( _this, GraphicsPath_onUpdate ), ((XObject)_this ));
  handle = _this->path;
  result = 0;
  result = EwInitSubPath((XPath*)handle, aSubPathNo, aMaxNoOfEdges );
  return result;
}

/* The method GetMaxNoOfSubPaths() returns how many sub-paths the affected path 
   can maximally store. The returned value corresponds to the parameter passed in 
   the invocation of the @SetMaxNoOfSubPaths() method. */
XInt32 GraphicsPath_GetMaxNoOfSubPaths( GraphicsPath _this )
{
  XHandle handle;
  XInt32 result;

  if ( _this->path == 0 )
    return 1;

  handle = _this->path;
  result = 0;
  result = EwGetMaxNoOfSubPaths((XPath*)handle );
  return result;
}

/* The method SetMaxNoOfSubPaths() changes the number of sub-paths the affected 
   path can maximally manage to the specified value aMaxNoOfSubPaths. After modifying 
   its size the affected path is empty. This means all contents of previously existing 
   sub-paths are discarded. Before new path data can be stored in a sub-path the 
   method @InitSubPath() should be called. Please note, if the size of the path 
   does already correspond to the parameter aMaxNoOfSubPaths, the path remains unaffected 
   and retains all existing contents.
   The method returns 'true' if the path could be initialized with the new size. 
   In case, there is no sufficient memory to store the desired number of sub-paths, 
   the method fails and returns 'false' leaving the path empty. */
XBool GraphicsPath_SetMaxNoOfSubPaths( GraphicsPath _this, XInt32 aMaxNoOfSubPaths )
{
  XHandle handle;

  if ( aMaxNoOfSubPaths < 1 )
    aMaxNoOfSubPaths = 1;

  if (( _this->path != 0 ) && ( GraphicsPath_GetMaxNoOfSubPaths( _this ) == aMaxNoOfSubPaths ))
    return 1;

  handle = _this->path;
  {
    EwFreePath((XPath*)handle );
    handle = (XHandle)EwCreatePath( aMaxNoOfSubPaths );
  }
  _this->path = handle;
  EwPostSignal( EwNewSlot( _this, GraphicsPath_onUpdate ), ((XObject)_this ));
  return (XBool)( _this->path != 0 );
}

/* Variants derived from the class : 'Graphics::Path' */
EW_DEFINE_CLASS_VARIANTS( GraphicsPath )
EW_END_OF_CLASS_VARIANTS( GraphicsPath )

/* Virtual Method Table (VMT) for the class : 'Graphics::Path' */
EW_DEFINE_CLASS( GraphicsPath, XObject, _None, _None, _None, _None, _None, "Graphics::Path" )
EW_END_OF_CLASS( GraphicsPath )

/* Initializer for the class 'Graphics::ArcPath' */
void GraphicsArcPath__Init( GraphicsArcPath _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  GraphicsPath__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( GraphicsArcPath );

  /* ... and initialize objects, variables, properties, etc. */
  _this->EndAngle = 360.000000f;
  _this->Style = GraphicsArcStyleArc;
}

/* Re-Initializer for the class 'Graphics::ArcPath' */
void GraphicsArcPath__ReInit( GraphicsArcPath _this )
{
  /* At first re-initialize the super class ... */
  GraphicsPath__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Graphics::ArcPath' */
void GraphicsArcPath__Done( GraphicsArcPath _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( GraphicsArcPath );

  /* Don't forget to deinitialize the super class ... */
  GraphicsPath__Done( &_this->_Super );
}

/* 'C' function for method : 'Graphics::ArcPath.updatePath()' */
void GraphicsArcPath_updatePath( GraphicsArcPath _this, XObject sender )
{
  XFloat angle;
  XInt32 noOfEdges;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  angle = _this->EndAngle - _this->StartAngle;
  noOfEdges = _this->NoOfEdges;

  if (( angle >= 360.000000f ) || ( angle <= -360.000000f ))
    angle = 360.000000f;

  GraphicsPath_SetMaxNoOfSubPaths((GraphicsPath)_this, 2 );

  if ( noOfEdges == 0 )
  {
    XFloat r = _this->RadiusX;

    if ( _this->RadiusY > r )
      r = _this->RadiusY;

    if ( angle == 360.000000f )
      noOfEdges = (XInt32)r;
    else
      noOfEdges = (XInt32)(( angle * r ) / 360.000000f );

    if ( noOfEdges < 0 )
      noOfEdges = -noOfEdges;

    noOfEdges = noOfEdges + 1;
  }

  if (((( angle == 0.000000f ) || ( _this->RadiusX == 0.000000f )) || ( _this->RadiusY 
      == 0.000000f )) || ((((( _this->Style == GraphicsArcStylePie ) || ( _this->Style 
      == GraphicsArcStylePieRounded )) || ( _this->Style == GraphicsArcStylePieRoundedStart )) 
      || ( _this->Style == GraphicsArcStylePieRoundedEnd )) && (( _this->InnerRadiusX 
      >= _this->RadiusX ) || ( _this->InnerRadiusY >= _this->RadiusY ))))
  {
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, 0 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );
    return;
  }

  if (( angle == 360.000000f ) && (((((( _this->Style != GraphicsArcStylePie ) && 
      ( _this->Style != GraphicsArcStylePieRounded )) && ( _this->Style != GraphicsArcStylePieRoundedStart )) 
      && ( _this->Style != GraphicsArcStylePieRoundedEnd )) || ( _this->InnerRadiusX 
      == 0.000000f )) || ( _this->InnerRadiusY == 0.000000f )))
  {
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, noOfEdges + 1 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );
    GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
    _this->RadiusY, 0.000000f, 360.000000f, noOfEdges );
    GraphicsPath_Close((GraphicsPath)_this, 0 );
    return;
  }

  if (((( angle == 360.000000f ) && (((( _this->Style == GraphicsArcStylePie ) || 
      ( _this->Style == GraphicsArcStylePieRounded )) || ( _this->Style == GraphicsArcStylePieRoundedStart )) 
      || ( _this->Style == GraphicsArcStylePieRoundedEnd ))) && ( _this->InnerRadiusX 
      > 0.000000f )) && ( _this->InnerRadiusY > 0.000000f ))
  {
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, noOfEdges + 1 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, noOfEdges + 1 );
    GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
    _this->RadiusY, 0.000000f, 360.000000f, noOfEdges );
    GraphicsPath_AddArc((GraphicsPath)_this, 1, 0.000000f, 0.000000f, _this->InnerRadiusX, 
    _this->InnerRadiusY, 360.000000f, 0.000000f, noOfEdges );
    GraphicsPath_Close((GraphicsPath)_this, 0 );
    GraphicsPath_Close((GraphicsPath)_this, 1 );
    return;
  }

  if ( _this->Style == GraphicsArcStyleArc )
  {
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, noOfEdges + 1 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );
    GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
    _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
    return;
  }

  if ( _this->Style == GraphicsArcStyleSegment )
  {
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, noOfEdges + 1 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );
    GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
    _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
    GraphicsPath_Close((GraphicsPath)_this, 0 );
    return;
  }

  if (( _this->Style == GraphicsArcStylePie ) && (( _this->InnerRadiusX == 0.000000f ) 
      || ( _this->InnerRadiusY == 0.000000f )))
  {
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, noOfEdges + 1 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );
    GraphicsPath_Begin((GraphicsPath)_this, 0, 0.000000f, 0.000000f );
    GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
    _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
    GraphicsPath_Close((GraphicsPath)_this, 0 );
    return;
  }

  if (( _this->Style == GraphicsArcStylePieRoundedStart ) && (( _this->InnerRadiusX 
      == 0.000000f ) || ( _this->InnerRadiusY == 0.000000f )))
  {
    XFloat ox = _this->RadiusX * EwMathCos( _this->StartAngle );
    XFloat oy = _this->RadiusY * EwMathSin( _this->StartAngle );
    XFloat dia = EwMathSqrt(( ox * ox ) + ( oy * oy ));
    XInt32 noOfEdgesDia = (XInt32)( dia * 0.250000f ) + 3;
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, ( noOfEdges + noOfEdgesDia ) 
    + 2 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );
    GraphicsPath_Begin((GraphicsPath)_this, 0, 0.000000f, 0.000000f );

    if ( angle >= 0.000000f )
      GraphicsPath_AddArc((GraphicsPath)_this, 0, ox / 2.000000f, oy / 2.000000f, 
      dia / 2.000000f, dia / 2.000000f, _this->StartAngle + 180.000000f, _this->StartAngle 
      + 360.000000f, noOfEdgesDia );
    else
      GraphicsPath_AddArc((GraphicsPath)_this, 0, ox / 2.000000f, oy / 2.000000f, 
      dia / 2.000000f, dia / 2.000000f, _this->StartAngle + 180.000000f, _this->StartAngle, 
      noOfEdgesDia );

    GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
    _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
    GraphicsPath_Close((GraphicsPath)_this, 0 );
    return;
  }

  if (( _this->Style == GraphicsArcStylePieRoundedEnd ) && (( _this->InnerRadiusX 
      == 0.000000f ) || ( _this->InnerRadiusY == 0.000000f )))
  {
    XFloat ea = _this->StartAngle + angle;
    XFloat ox = _this->RadiusX * EwMathCos( ea );
    XFloat oy = _this->RadiusY * EwMathSin( ea );
    XFloat dia = EwMathSqrt(( ox * ox ) + ( oy * oy ));
    XInt32 noOfEdgesDia = (XInt32)( dia * 0.250000f ) + 3;
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, ( noOfEdges + noOfEdgesDia ) 
    + 2 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );
    GraphicsPath_Begin((GraphicsPath)_this, 0, 0.000000f, 0.000000f );
    GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
    _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );

    if ( angle >= 0.000000f )
      GraphicsPath_AddArc((GraphicsPath)_this, 0, ox / 2.000000f, oy / 2.000000f, 
      dia / 2.000000f, dia / 2.000000f, ea, ea + 180.000000f, noOfEdgesDia );
    else
      GraphicsPath_AddArc((GraphicsPath)_this, 0, ox / 2.000000f, oy / 2.000000f, 
      dia / 2.000000f, dia / 2.000000f, ea, ea - 180.000000f, noOfEdgesDia );

    GraphicsPath_Close((GraphicsPath)_this, 0 );
    return;
  }

  if (( _this->Style == GraphicsArcStylePieRounded ) && (( _this->InnerRadiusX == 
      0.000000f ) || ( _this->InnerRadiusY == 0.000000f )))
  {
    XFloat sox = _this->RadiusX * EwMathCos( _this->StartAngle );
    XFloat soy = _this->RadiusY * EwMathSin( _this->StartAngle );
    XFloat sdia = EwMathSqrt(( sox * sox ) + ( soy * soy ));
    XFloat ea = _this->StartAngle + angle;
    XFloat eox = _this->RadiusX * EwMathCos( ea );
    XFloat eoy = _this->RadiusY * EwMathSin( ea );
    XFloat edia = EwMathSqrt(( eox * eox ) + ( eoy * eoy ));
    XInt32 noOfEdgesSDia = (XInt32)( sdia * 0.250000f ) + 3;
    XInt32 noOfEdgesEDia = (XInt32)( edia * 0.250000f ) + 3;
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, (( noOfEdges + noOfEdgesSDia ) 
    + noOfEdgesEDia ) + 10 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );
    GraphicsPath_Begin((GraphicsPath)_this, 0, 0.000000f, 0.000000f );

    if ( angle >= 0.000000f )
    {
      GraphicsPath_AddArc((GraphicsPath)_this, 0, sox / 2.000000f, soy / 2.000000f, 
      sdia / 2.000000f, sdia / 2.000000f, _this->StartAngle + 180.000000f, _this->StartAngle 
      + 360.000000f, noOfEdgesSDia );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
      _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, eox / 2.000000f, eoy / 2.000000f, 
      edia / 2.000000f, edia / 2.000000f, ea, ea + 180.000000f, noOfEdgesEDia );
    }
    else
    {
      GraphicsPath_AddArc((GraphicsPath)_this, 0, sox / 2.000000f, soy / 2.000000f, 
      sdia / 2.000000f, sdia / 2.000000f, _this->StartAngle + 180.000000f, _this->StartAngle, 
      noOfEdgesSDia );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
      _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, eox / 2.000000f, eoy / 2.000000f, 
      edia / 2.000000f, edia / 2.000000f, ea, ea - 180.000000f, noOfEdgesEDia );
    }

    GraphicsPath_Close((GraphicsPath)_this, 0 );
    return;
  }

  if ( _this->Style == GraphicsArcStylePie )
  {
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, ( noOfEdges * 2 ) + 1 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );
    GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
    _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
    GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->InnerRadiusX, 
    _this->InnerRadiusY, _this->EndAngle, _this->EndAngle - angle, noOfEdges );
    GraphicsPath_Close((GraphicsPath)_this, 0 );
    return;
  }

  if ( _this->Style == GraphicsArcStylePieRoundedStart )
  {
    XFloat sin = EwMathSin( _this->StartAngle );
    XFloat cos = EwMathCos( _this->StartAngle );
    XFloat ix = _this->InnerRadiusX * cos;
    XFloat iy = _this->InnerRadiusY * sin;
    XFloat ox = _this->RadiusX * cos;
    XFloat oy = _this->RadiusY * sin;
    XFloat dia = EwMathSqrt((( ox - ix ) * ( ox - ix )) + (( oy - iy ) * ( oy - 
      iy )));
    XInt32 noOfEdgesDia = (XInt32)( dia * 0.250000f ) + 3;
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, (( noOfEdges * 2 ) + noOfEdgesDia ) 
    + 2 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );

    if ( angle >= 0.000000f )
    {
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
      _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->InnerRadiusX, 
      _this->InnerRadiusY, _this->EndAngle, _this->EndAngle - angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, ix + (( ox - ix ) / 2.000000f ), 
      iy + (( oy - iy ) / 2.000000f ), dia / 2.000000f, dia / 2.000000f, _this->StartAngle 
      + 180.000000f, _this->StartAngle + 360.000000f, noOfEdgesDia );
    }
    else
    {
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
      _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->InnerRadiusX, 
      _this->InnerRadiusY, _this->EndAngle, _this->EndAngle - angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, ix + (( ox - ix ) / 2.000000f ), 
      iy + (( oy - iy ) / 2.000000f ), dia / 2.000000f, dia / 2.000000f, _this->StartAngle 
      + 180.000000f, _this->StartAngle, noOfEdgesDia );
    }

    GraphicsPath_Close((GraphicsPath)_this, 0 );
    return;
  }

  if ( _this->Style == GraphicsArcStylePieRoundedEnd )
  {
    XFloat ea = _this->StartAngle + angle;
    XFloat sin = EwMathSin( ea );
    XFloat cos = EwMathCos( ea );
    XFloat ix = _this->InnerRadiusX * cos;
    XFloat iy = _this->InnerRadiusY * sin;
    XFloat ox = _this->RadiusX * cos;
    XFloat oy = _this->RadiusY * sin;
    XFloat dia = EwMathSqrt((( ox - ix ) * ( ox - ix )) + (( oy - iy ) * ( oy - 
      iy )));
    XInt32 noOfEdgesDia = (XInt32)( dia * 0.250000f ) + 3;
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, (( noOfEdges * 2 ) + noOfEdgesDia ) 
    + 2 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );

    if ( angle >= 0.000000f )
    {
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
      _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, ix + (( ox - ix ) / 2.000000f ), 
      iy + (( oy - iy ) / 2.000000f ), dia / 2.000000f, dia / 2.000000f, ea, ea 
      + 180.000000f, noOfEdgesDia );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->InnerRadiusX, 
      _this->InnerRadiusY, _this->EndAngle, _this->EndAngle - angle, noOfEdges );
    }
    else
    {
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
      _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, ix + (( ox - ix ) / 2.000000f ), 
      iy + (( oy - iy ) / 2.000000f ), dia / 2.000000f, dia / 2.000000f, ea, ea 
      - 180.000000f, noOfEdgesDia );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->InnerRadiusX, 
      _this->InnerRadiusY, _this->EndAngle, _this->EndAngle - angle, noOfEdges );
    }

    GraphicsPath_Close((GraphicsPath)_this, 0 );
    return;
  }

  if ( _this->Style == GraphicsArcStylePieRounded )
  {
    XFloat ea = _this->StartAngle + angle;
    XFloat ssin = EwMathSin( _this->StartAngle );
    XFloat scos = EwMathCos( _this->StartAngle );
    XFloat esin = EwMathSin( ea );
    XFloat ecos = EwMathCos( ea );
    XFloat six = _this->InnerRadiusX * scos;
    XFloat siy = _this->InnerRadiusY * ssin;
    XFloat sox = _this->RadiusX * scos;
    XFloat soy = _this->RadiusY * ssin;
    XFloat eix = _this->InnerRadiusX * ecos;
    XFloat eiy = _this->InnerRadiusY * esin;
    XFloat eox = _this->RadiusX * ecos;
    XFloat eoy = _this->RadiusY * esin;
    XFloat sdia = EwMathSqrt((( sox - six ) * ( sox - six )) + (( soy - siy ) * 
      ( soy - siy )));
    XFloat edia = EwMathSqrt((( eox - eix ) * ( eox - eix )) + (( eoy - eiy ) * 
      ( eoy - eiy )));
    XInt32 noOfEdgesSDia = (XInt32)( sdia * 0.250000f ) + 3;
    XInt32 noOfEdgesEDia = (XInt32)( edia * 0.250000f ) + 3;
    GraphicsPath_InitSubPath((GraphicsPath)_this, 0, ((( noOfEdges * 2 ) + noOfEdgesSDia ) 
    + noOfEdgesEDia ) + 4 );
    GraphicsPath_InitSubPath((GraphicsPath)_this, 1, 0 );

    if ( angle >= 0.000000f )
    {
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
      _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, eix + (( eox - eix ) / 2.000000f ), 
      eiy + (( eoy - eiy ) / 2.000000f ), edia / 2.000000f, edia / 2.000000f, ea, 
      ea + 180.000000f, noOfEdgesEDia );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->InnerRadiusX, 
      _this->InnerRadiusY, _this->EndAngle, _this->EndAngle - angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, six + (( sox - six ) / 2.000000f ), 
      siy + (( soy - siy ) / 2.000000f ), sdia / 2.000000f, sdia / 2.000000f, _this->StartAngle 
      + 180.000000f, _this->StartAngle + 360.000000f, noOfEdgesSDia );
    }
    else
    {
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->RadiusX, 
      _this->RadiusY, _this->StartAngle, _this->StartAngle + angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, eix + (( eox - eix ) / 2.000000f ), 
      eiy + (( eoy - eiy ) / 2.000000f ), edia / 2.000000f, edia / 2.000000f, ea, 
      ea - 180.000000f, noOfEdgesEDia );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, 0.000000f, 0.000000f, _this->InnerRadiusX, 
      _this->InnerRadiusY, _this->EndAngle, _this->EndAngle - angle, noOfEdges );
      GraphicsPath_AddArc((GraphicsPath)_this, 0, six + (( sox - six ) / 2.000000f ), 
      siy + (( soy - siy ) / 2.000000f ), sdia / 2.000000f, sdia / 2.000000f, _this->StartAngle 
      + 180.000000f, _this->StartAngle, noOfEdgesSDia );
    }

    GraphicsPath_Close((GraphicsPath)_this, 0 );
    return;
  }
}

/* 'C' function for method : 'Graphics::ArcPath.OnSetNoOfEdges()' */
void GraphicsArcPath_OnSetNoOfEdges( GraphicsArcPath _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value == _this->NoOfEdges )
    return;

  _this->NoOfEdges = value;

  if ((( _this->StartAngle != _this->EndAngle ) && ( _this->RadiusX > 0.000000f )) 
      && ( _this->RadiusY > 0.000000f ))
    EwPostSignal( EwNewSlot( _this, GraphicsArcPath_updatePath ), ((XObject)_this ));
}

/* 'C' function for method : 'Graphics::ArcPath.OnSetEndAngle()' */
void GraphicsArcPath_OnSetEndAngle( GraphicsArcPath _this, XFloat value )
{
  if ( value == _this->EndAngle )
    return;

  _this->EndAngle = value;

  if (( _this->RadiusX > 0.000000f ) && ( _this->RadiusY > 0.000000f ))
    EwPostSignal( EwNewSlot( _this, GraphicsArcPath_updatePath ), ((XObject)_this ));
}

/* 'C' function for method : 'Graphics::ArcPath.OnSetStartAngle()' */
void GraphicsArcPath_OnSetStartAngle( GraphicsArcPath _this, XFloat value )
{
  if ( value == _this->StartAngle )
    return;

  _this->StartAngle = value;

  if (( _this->RadiusX > 0.000000f ) && ( _this->RadiusY > 0.000000f ))
    EwPostSignal( EwNewSlot( _this, GraphicsArcPath_updatePath ), ((XObject)_this ));
}

/* 'C' function for method : 'Graphics::ArcPath.OnSetRadiusY()' */
void GraphicsArcPath_OnSetRadiusY( GraphicsArcPath _this, XFloat value )
{
  if ( value < 0.000000f )
    value = 0.000000f;

  if ( value == _this->RadiusY )
    return;

  _this->RadiusY = value;

  if (( _this->StartAngle != _this->EndAngle ) && ( _this->RadiusX > 0.000000f ))
    EwPostSignal( EwNewSlot( _this, GraphicsArcPath_updatePath ), ((XObject)_this ));
}

/* 'C' function for method : 'Graphics::ArcPath.OnSetRadiusX()' */
void GraphicsArcPath_OnSetRadiusX( GraphicsArcPath _this, XFloat value )
{
  if ( value < 0.000000f )
    value = 0.000000f;

  if ( value == _this->RadiusX )
    return;

  _this->RadiusX = value;

  if (( _this->StartAngle != _this->EndAngle ) && ( _this->RadiusY > 0.000000f ))
    EwPostSignal( EwNewSlot( _this, GraphicsArcPath_updatePath ), ((XObject)_this ));
}

/* 'C' function for method : 'Graphics::ArcPath.OnSetRadius()' */
void GraphicsArcPath_OnSetRadius( GraphicsArcPath _this, XFloat value )
{
  if (( value == _this->RadiusX ) && ( value == _this->RadiusY ))
    return;

  _this->RadiusX = value;
  _this->RadiusY = value;

  if ( _this->StartAngle != _this->EndAngle )
    EwPostSignal( EwNewSlot( _this, GraphicsArcPath_updatePath ), ((XObject)_this ));
}

/* 'C' function for method : 'Graphics::ArcPath.OnSetStyle()' */
void GraphicsArcPath_OnSetStyle( GraphicsArcPath _this, XEnum value )
{
  if ( value == _this->Style )
    return;

  _this->Style = value;

  if ((( _this->StartAngle != _this->EndAngle ) && ( _this->RadiusX > 0.000000f )) 
      && ( _this->RadiusY > 0.000000f ))
    EwPostSignal( EwNewSlot( _this, GraphicsArcPath_updatePath ), ((XObject)_this ));
}

/* Variants derived from the class : 'Graphics::ArcPath' */
EW_DEFINE_CLASS_VARIANTS( GraphicsArcPath )
EW_END_OF_CLASS_VARIANTS( GraphicsArcPath )

/* Virtual Method Table (VMT) for the class : 'Graphics::ArcPath' */
EW_DEFINE_CLASS( GraphicsArcPath, GraphicsPath, _None, _None, _None, _None, _None, 
                 "Graphics::ArcPath" )
EW_END_OF_CLASS( GraphicsArcPath )

/* Embedded Wizard */
