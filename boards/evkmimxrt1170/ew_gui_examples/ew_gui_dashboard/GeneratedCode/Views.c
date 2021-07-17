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
#include "_CoreCursorEvent.h"
#include "_CoreCursorHit.h"
#include "_CoreDragEvent.h"
#include "_CoreEvent.h"
#include "_CoreGroup.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_EffectsEffectTimerClass.h"
#include "_GraphicsCanvas.h"
#include "_GraphicsPath.h"
#include "_GraphicsWarpMatrix.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "_ViewsFillPath.h"
#include "_ViewsFrame.h"
#include "_ViewsImage.h"
#include "_ViewsLine.h"
#include "_ViewsRectangle.h"
#include "_ViewsStrokePath.h"
#include "_ViewsText.h"
#include "_ViewsWallpaper.h"
#include "_ViewsWarpGroup.h"
#include "_ViewsWarpImage.h"
#include "_ViewsWarpView.h"
#include "Core.h"
#include "Effects.h"
#include "Graphics.h"
#include "Resources.h"
#include "Views.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x000000BA, /* ratio 68.82 % */
  0xB8000900, 0x3F9FE452, 0x16070300, 0x0C200219, 0x40003700, 0x06F0043C, 0x00889D00,
  0x034800C8, 0x38000E60, 0x0434D800, 0x45E30790, 0x60CA0034, 0xCE5233BC, 0x3D0E8C47,
  0xC6395482, 0x668AC622, 0xA940024C, 0x3AC7A252, 0xE4BA552F, 0xC4A5D158, 0x565C0033,
  0x05D20A04, 0x351322C6, 0xD1E9548A, 0x349744A3, 0xE9750001, 0x949DA391, 0x9CAA3518,
  0xC42C1519, 0x229D259A, 0x20200B5C, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XColor _Const0000 = { 0xFF, 0xFF, 0xFF, 0xFF };
static const XPoint _Const0001 = { 1, 1 };
static const XPoint _Const0002 = { 0, 0 };
static const XRect _Const0003 = {{ 0, 0 }, { 0, 0 }};
static const XStringRes _Const0004 = { _StringsDefault0, 0x0002 };
static const XStringRes _Const0005 = { _StringsDefault0, 0x0006 };

#ifndef EW_DONT_CHECK_INDEX
  /* This function is used to check the indices when accessing an array.
     If you don't want this verification add the define EW_DONT_CHECK_INDEX
     to your Makefile or project settings. */
  static int EwCheckIndex( int aIndex, int aRange, const char* aFile, int aLine )
  {
    if (( aIndex < 0 ) || ( aIndex >= aRange ))
    {
      EwPrint( "[FATAL ERROR in %s:%d] Array index %d out of bounds %d",
                aFile, aLine, aIndex, aRange );
      EwPanic();
    }
    return aIndex;
  }

  #define EwCheckIndex( aIndex, aRange ) \
    EwCheckIndex( aIndex, aRange, __FILE__, __LINE__ )
#else
  #define EwCheckIndex( aIndex, aRange ) aIndex
#endif

/* Initializer for the class 'Views::Line' */
void ViewsLine__Init( ViewsLine _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreLineView__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ViewsLine );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ViewsLine );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Width2 = 1;
  _this->Width1 = 1;
  _this->Color2 = _Const0000;
  _this->Color1 = _Const0000;
}

/* Re-Initializer for the class 'Views::Line' */
void ViewsLine__ReInit( ViewsLine _this )
{
  /* At first re-initialize the super class ... */
  CoreLineView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Views::Line' */
void ViewsLine__Done( ViewsLine _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreLineView );

  /* Don't forget to deinitialize the super class ... */
  CoreLineView__Done( &_this->_Super );
}

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void ViewsLine_Draw( ViewsLine _this, GraphicsCanvas aCanvas, XRect aClip, XPoint 
  aOffset, XInt32 aOpacity, XBool aBlend )
{
  XInt32 w1 = _this->Width1;
  XInt32 w2 = _this->Width2;
  XPoint p1 = EwMovePointPos( _this->Super1.Point1, aOffset );
  XPoint p2 = EwMovePointPos( _this->Super1.Point2, aOffset );
  XColor c1 = _this->Color1;
  XColor c2 = _this->Color2;

  if ( !EwCompPoint( p1, p2 ))
    return;

  aBlend = (XBool)( aBlend && (( _this->Super2.viewState & CoreViewStateAlphaBlended ) 
  == CoreViewStateAlphaBlended ));
  aOpacity = aOpacity + 1;

  if ( aOpacity < 256 )
  {
    c1.Alpha = (XUInt8)(( aOpacity * c1.Alpha ) >> 8 );
    c2.Alpha = (XUInt8)(( aOpacity * c2.Alpha ) >> 8 );
  }

  if (( w1 == 1 ) && ( w2 == 1 ))
    GraphicsCanvas_DrawLine( aCanvas, aClip, p1, p2, c1, c2, aBlend );
  else
    GraphicsCanvas_DrawThickLine( aCanvas, aClip, p1, p2, w1, w2, c1, c2, aBlend );
}

/* The method GetExtent() returns the position and the size of the view relative 
   to the origin of its @Owner. In case of views with a non rectangular shape the 
   method returns the rectangular boundary area enclosing the entire shape. */
XRect ViewsLine_GetExtent( ViewsLine _this )
{
  XInt32 w1 = _this->Width1;
  XInt32 w2 = _this->Width2;
  XPoint p1 = _this->Super1.Point1;
  XPoint p2 = _this->Super1.Point2;
  XFloat fw1;
  XFloat fw2;
  XFloat fp1X;
  XFloat fp1Y;
  XFloat fp2X;
  XFloat fp2Y;
  XFloat dirX;
  XFloat dirY;
  XFloat len;
  XFloat c1x;
  XFloat c1y;
  XFloat c2x;
  XFloat c2y;
  XFloat c3x;
  XFloat c3y;
  XFloat c4x;
  XFloat c4y;
  XFloat left;
  XFloat right;
  XFloat top;
  XFloat bottom;
  XRect r;

  if ((( w1 == 1 ) && ( w2 == 1 )) || !EwCompPoint( p1, p2 ))
    return CoreLineView_GetExtent((CoreLineView)_this );

  fw1 = (XFloat)w1 / 2.000000f;
  fw2 = (XFloat)w2 / 2.000000f;
  fp1X = (XFloat)p1.X;
  fp1Y = (XFloat)p1.Y;
  fp2X = (XFloat)p2.X;
  fp2Y = (XFloat)p2.Y;
  dirX = fp2X - fp1X;
  dirY = fp2Y - fp1Y;
  len = EwMathSqrt(( dirX * dirX ) + ( dirY * dirY ));
  dirX = dirX / len;
  dirY = dirY / len;
  c1x = fp1X + ( dirY * fw1 );
  c1y = fp1Y - ( dirX * fw1 );
  c2x = fp2X + ( dirY * fw2 );
  c2y = fp2Y - ( dirX * fw2 );
  c3x = fp2X - ( dirY * fw2 );
  c3y = fp2Y + ( dirX * fw2 );
  c4x = fp1X - ( dirY * fw1 );
  c4y = fp1Y + ( dirX * fw1 );
  left = c1x;
  right = c1x;
  top = c1y;
  bottom = c1y;

  if ( c2x < left )
    left = c2x;

  if ( c3x < left )
    left = c3x;

  if ( c4x < left )
    left = c4x;

  if ( c2x > right )
    right = c2x;

  if ( c3x > right )
    right = c3x;

  if ( c4x > right )
    right = c4x;

  if ( c2y < top )
    top = c2y;

  if ( c3y < top )
    top = c3y;

  if ( c4y < top )
    top = c4y;

  if ( c2y > bottom )
    bottom = c2y;

  if ( c3y > bottom )
    bottom = c3y;

  if ( c4y > bottom )
    bottom = c4y;

  r = EwNewRect((XInt32)left, (XInt32)top, (XInt32)right, (XInt32)bottom );
  r.Point2 = EwMovePointPos( r.Point2, _Const0001 );
  return r;
}

/* 'C' function for method : 'Views::Line.observerSlot()' */
void ViewsLine_observerSlot( ViewsLine _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, CoreView__GetExtent( _this ));
}

/* 'C' function for method : 'Views::Line.OnSetWidth()' */
void ViewsLine_OnSetWidth( ViewsLine _this, XInt32 value )
{
  if ( value < 1 )
    value = 1;

  if (( value == _this->Width1 ) && ( value == _this->Width2 ))
    return;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, CoreView__GetExtent( _this ));

  _this->Width1 = value;
  _this->Width2 = value;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, CoreView__GetExtent( _this ));

  if (( value != 1 ) && ( _this->whiteBitmap == 0 ))
  {
    _this->whiteBitmap = EwLoadResource( &ResourcesWhiteBitmapStripe, ResourcesBitmap );

    if ( _this->whiteBitmap->Mutable )
      EwAttachObjObserver( EwNewSlot( _this, ViewsLine_observerSlot ), (XObject)_this->whiteBitmap, 
        0 );
  }

  if (( value == 1 ) && ( _this->whiteBitmap != 0 ))
  {
    if ( _this->whiteBitmap->Mutable )
      EwDetachObjObserver( EwNewSlot( _this, ViewsLine_observerSlot ), (XObject)_this->whiteBitmap, 
        0 );

    _this->whiteBitmap = 0;
  }
}

/* 'C' function for method : 'Views::Line.OnSetColor()' */
void ViewsLine_OnSetColor( ViewsLine _this, XColor value )
{
  if ( !EwCompColor( value, _this->Color1 ) && !EwCompColor( value, _this->Color2 ))
    return;

  _this->Color1 = value;
  _this->Color2 = value;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, CoreView__GetExtent( _this ));
}

/* Variants derived from the class : 'Views::Line' */
EW_DEFINE_CLASS_VARIANTS( ViewsLine )
EW_END_OF_CLASS_VARIANTS( ViewsLine )

/* Virtual Method Table (VMT) for the class : 'Views::Line' */
EW_DEFINE_CLASS( ViewsLine, CoreLineView, whiteBitmap, Width2, Width2, Width2, Width2, 
                 Width2, "Views::Line" )
  CoreLineView_initLayoutContext,
  CoreView_GetRoot,
  ViewsLine_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreLineView_ArrangeView,
  CoreLineView_MoveView,
  ViewsLine_GetExtent,
  CoreView_ChangeViewState,
EW_END_OF_CLASS( ViewsLine )

/* Initializer for the class 'Views::Rectangle' */
void ViewsRectangle__Init( ViewsRectangle _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreRectView__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ViewsRectangle );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ViewsRectangle );

  /* ... and initialize objects, variables, properties, etc. */
  _this->ColorBL = _Const0000;
  _this->ColorBR = _Const0000;
  _this->ColorTR = _Const0000;
  _this->ColorTL = _Const0000;
}

/* Re-Initializer for the class 'Views::Rectangle' */
void ViewsRectangle__ReInit( ViewsRectangle _this )
{
  /* At first re-initialize the super class ... */
  CoreRectView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Views::Rectangle' */
void ViewsRectangle__Done( ViewsRectangle _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreRectView );

  /* Don't forget to deinitialize the super class ... */
  CoreRectView__Done( &_this->_Super );
}

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void ViewsRectangle_Draw( ViewsRectangle _this, GraphicsCanvas aCanvas, XRect aClip, 
  XPoint aOffset, XInt32 aOpacity, XBool aBlend )
{
  aBlend = (XBool)( aBlend && (( _this->Super2.viewState & CoreViewStateAlphaBlended ) 
  == CoreViewStateAlphaBlended ));
  aOpacity = aOpacity + 1;

  if ( aOpacity < 256 )
  {
    XColor ctl = _this->ColorTL;
    XColor ctr = _this->ColorTR;
    XColor cbl = _this->ColorBL;
    XColor cbr = _this->ColorBR;
    ctl.Alpha = (XUInt8)(( aOpacity * ctl.Alpha ) >> 8 );
    ctr.Alpha = (XUInt8)(( aOpacity * ctr.Alpha ) >> 8 );
    cbl.Alpha = (XUInt8)(( aOpacity * cbl.Alpha ) >> 8 );
    cbr.Alpha = (XUInt8)(( aOpacity * cbr.Alpha ) >> 8 );
    GraphicsCanvas_FillRectangle( aCanvas, aClip, EwMoveRectPos( _this->Super1.Bounds, 
    aOffset ), ctl, ctr, cbr, cbl, aBlend );
  }
  else
    GraphicsCanvas_FillRectangle( aCanvas, aClip, EwMoveRectPos( _this->Super1.Bounds, 
    aOffset ), _this->ColorTL, _this->ColorTR, _this->ColorBR, _this->ColorBL, aBlend );
}

/* 'C' function for method : 'Views::Rectangle.OnSetColor()' */
void ViewsRectangle_OnSetColor( ViewsRectangle _this, XColor value )
{
  if ((( !EwCompColor( value, _this->ColorTL ) && !EwCompColor( value, _this->ColorTR )) 
      && !EwCompColor( value, _this->ColorBL )) && !EwCompColor( value, _this->ColorBR ))
    return;

  _this->ColorTL = value;
  _this->ColorTR = value;
  _this->ColorBL = value;
  _this->ColorBR = value;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* Variants derived from the class : 'Views::Rectangle' */
EW_DEFINE_CLASS_VARIANTS( ViewsRectangle )
EW_END_OF_CLASS_VARIANTS( ViewsRectangle )

/* Virtual Method Table (VMT) for the class : 'Views::Rectangle' */
EW_DEFINE_CLASS( ViewsRectangle, CoreRectView, _None, _None, _None, _None, _None, 
                 _None, "Views::Rectangle" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  ViewsRectangle_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreView_ChangeViewState,
  CoreRectView_OnSetBounds,
EW_END_OF_CLASS( ViewsRectangle )

/* Initializer for the class 'Views::Frame' */
void ViewsFrame__Init( ViewsFrame _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreRectView__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ViewsFrame );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ViewsFrame );

  /* ... and initialize objects, variables, properties, etc. */
  _this->animFrameNumber = -1;
  _this->ColorBL = _Const0000;
  _this->ColorBR = _Const0000;
  _this->ColorTR = _Const0000;
  _this->ColorTL = _Const0000;
  _this->Endless = 1;
  _this->Edges = GraphicsEdgesBottom | GraphicsEdgesInterior | GraphicsEdgesLeft 
  | GraphicsEdgesRight | GraphicsEdgesTop;
  _this->Opacity = 255;
}

/* Re-Initializer for the class 'Views::Frame' */
void ViewsFrame__ReInit( ViewsFrame _this )
{
  /* At first re-initialize the super class ... */
  CoreRectView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Views::Frame' */
void ViewsFrame__Done( ViewsFrame _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreRectView );

  /* Don't forget to deinitialize the super class ... */
  CoreRectView__Done( &_this->_Super );
}

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void ViewsFrame_Draw( ViewsFrame _this, GraphicsCanvas aCanvas, XRect aClip, XPoint 
  aOffset, XInt32 aOpacity, XBool aBlend )
{
  XInt32 frameNr = _this->FrameNumber;
  XColor ctl;
  XColor ctr;
  XColor cbr;
  XColor cbl;
  XInt32 opacity;
  XSet edges;
  XRect r;
  XPoint sd;

  if ( _this->animFrameNumber >= 0 )
    frameNr = _this->animFrameNumber;

  if (( _this->Bitmap == 0 ) || ( frameNr >= _this->Bitmap->NoOfFrames ))
    return;

  ResourcesBitmap__Update( _this->Bitmap );
  ctl = _this->ColorTL;
  ctr = _this->ColorTR;
  cbr = _this->ColorBR;
  cbl = _this->ColorBL;
  opacity = ((( aOpacity + 1 ) * _this->Opacity ) >> 8 ) + 1;
  edges = _this->Edges;
  r = EwMoveRectPos( _this->Super1.Bounds, aOffset );
  sd = EwMovePointNeg( EwGetRectSize( r ), _this->NoEdgesLimit );
  aBlend = (XBool)( aBlend && (( _this->Super2.viewState & CoreViewStateAlphaBlended ) 
  == CoreViewStateAlphaBlended ));

  if ( opacity < 256 )
  {
    ctl.Alpha = (XUInt8)(( ctl.Alpha * opacity ) >> 8 );
    ctr.Alpha = (XUInt8)(( ctr.Alpha * opacity ) >> 8 );
    cbr.Alpha = (XUInt8)(( cbr.Alpha * opacity ) >> 8 );
    cbl.Alpha = (XUInt8)(( cbl.Alpha * opacity ) >> 8 );
  }

  if ((( _this->NoEdgesLimit.X > 0 ) && ( sd.X > 0 )) && !(( edges & ( GraphicsEdgesLeft 
      | GraphicsEdgesRight )) == ( GraphicsEdgesLeft | GraphicsEdgesRight )))
  {
    XInt32 d = ( _this->Bitmap->FrameSize.X / 3 ) - sd.X;

    if ((( edges & GraphicsEdgesLeft ) == GraphicsEdgesLeft ))
    {
      if ( aClip.Point2.X > r.Point2.X )
        aClip.Point2.X = r.Point2.X;

      if ( d > 0 )
        r.Point2.X = ( r.Point2.X + d );

      edges = edges | GraphicsEdgesRight;
    }
    else
      if ((( edges & GraphicsEdgesRight ) == GraphicsEdgesRight ))
      {
        if ( aClip.Point1.X < r.Point1.X )
          aClip.Point1.X = r.Point1.X;

        if ( d > 0 )
          r.Point1.X = ( r.Point1.X - d );

        edges = edges | GraphicsEdgesLeft;
      }
      else
      {
        if ( aClip.Point1.X < r.Point1.X )
          aClip.Point1.X = r.Point1.X;

        if ( aClip.Point2.X > r.Point2.X )
          aClip.Point2.X = r.Point2.X;

        if ( d > 0 )
        {
          r.Point1.X = ( r.Point1.X - ( d / 2 ));
          r.Point2.X = (( r.Point2.X + d ) - ( d / 2 ));
        }

        edges = edges | ( GraphicsEdgesLeft | GraphicsEdgesRight );
      }
  }

  if ((( _this->NoEdgesLimit.Y > 0 ) && ( sd.Y > 0 )) && !(( edges & ( GraphicsEdgesBottom 
      | GraphicsEdgesTop )) == ( GraphicsEdgesBottom | GraphicsEdgesTop )))
  {
    XInt32 d = ( _this->Bitmap->FrameSize.Y / 3 ) - sd.Y;

    if ((( edges & GraphicsEdgesTop ) == GraphicsEdgesTop ))
    {
      if ( aClip.Point2.Y > r.Point2.Y )
        aClip.Point2.Y = r.Point2.Y;

      if ( d > 0 )
        r.Point2.Y = ( r.Point2.Y + d );

      edges = edges | GraphicsEdgesBottom;
    }
    else
      if ((( edges & GraphicsEdgesBottom ) == GraphicsEdgesBottom ))
      {
        if ( aClip.Point1.Y < r.Point1.Y )
          aClip.Point1.Y = r.Point1.Y;

        if ( d > 0 )
          r.Point1.Y = ( r.Point1.Y - d );

        edges = edges | GraphicsEdgesTop;
      }
      else
      {
        if ( aClip.Point1.Y < r.Point1.Y )
          aClip.Point1.Y = r.Point1.Y;

        if ( aClip.Point2.Y > r.Point2.Y )
          aClip.Point2.Y = r.Point2.Y;

        if ( d > 0 )
        {
          r.Point1.Y = ( r.Point1.Y - ( d / 2 ));
          r.Point2.Y = (( r.Point2.Y + d ) - ( d / 2 ));
        }

        edges = edges | ( GraphicsEdgesBottom | GraphicsEdgesTop );
      }
  }

  GraphicsCanvas_DrawBitmapFrame( aCanvas, aClip, _this->Bitmap, frameNr, r, edges, 
  ctl, ctr, cbr, cbl, aBlend );
}

/* 'C' function for method : 'Views::Frame.observerSlot()' */
void ViewsFrame_observerSlot( ViewsFrame _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Frame.timerSlot()' */
void ViewsFrame_timerSlot( ViewsFrame _this, XObject sender )
{
  XInt32 frameNr;
  XInt32 period;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  frameNr = _this->animFrameNumber;
  period = 0;

  if ( _this->Bitmap != 0 )
    period = _this->Bitmap->NoOfFrames * _this->Bitmap->FrameDelay;

  if ((( _this->timer != 0 ) && ( _this->animFrameNumber < 0 )) && ( period > 0 ))
    _this->startTime = _this->timer->Time - ( _this->FrameNumber * _this->Bitmap->FrameDelay );

  if (( _this->timer != 0 ) && ( period > 0 ))
  {
    XInt32 time = (XInt32)( _this->timer->Time - _this->startTime );
    frameNr = time / _this->Bitmap->FrameDelay;

    if ( time >= period )
    {
      frameNr = frameNr % _this->Bitmap->NoOfFrames;
      _this->startTime = _this->timer->Time - ( time % period );

      if ( !_this->Endless )
      {
        frameNr = _this->Bitmap->NoOfFrames - 1;
        period = 0;
      }
    }
  }

  if ((( frameNr != _this->animFrameNumber ) && ( _this->Super2.Owner != 0 )) && 
      (( _this->Super2.viewState & CoreViewStateVisible ) == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  _this->animFrameNumber = frameNr;

  if (( period == 0 ) && ( _this->timer != 0 ))
  {
    EwDetachObjObserver( EwNewSlot( _this, ViewsFrame_timerSlot ), (XObject)_this->timer, 
      0 );
    _this->timer = 0;
    EwSignal( _this->OnFinished, ((XObject)_this ));
  }
}

/* 'C' function for method : 'Views::Frame.OnSetColor()' */
void ViewsFrame_OnSetColor( ViewsFrame _this, XColor value )
{
  if ((( !EwCompColor( value, _this->ColorTL ) && !EwCompColor( value, _this->ColorTR )) 
      && !EwCompColor( value, _this->ColorBL )) && !EwCompColor( value, _this->ColorBR ))
    return;

  _this->ColorTL = value;
  _this->ColorTR = value;
  _this->ColorBL = value;
  _this->ColorBR = value;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Frame.OnSetAnimated()' */
void ViewsFrame_OnSetAnimated( ViewsFrame _this, XBool value )
{
  if ( _this->Animated == value )
    return;

  _this->Animated = value;
  _this->animFrameNumber = -1;

  if ( !value && ( _this->timer != 0 ))
  {
    EwDetachObjObserver( EwNewSlot( _this, ViewsFrame_timerSlot ), (XObject)_this->timer, 
      0 );
    _this->timer = 0;
  }

  if ( value )
  {
    _this->timer = ((CoreTimer)EwGetAutoObject( &EffectsEffectTimer, EffectsEffectTimerClass ));
    EwAttachObjObserver( EwNewSlot( _this, ViewsFrame_timerSlot ), (XObject)_this->timer, 
      0 );
    EwPostSignal( EwNewSlot( _this, ViewsFrame_timerSlot ), ((XObject)_this ));
  }

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Frame.OnSetFrameNumber()' */
void ViewsFrame_OnSetFrameNumber( ViewsFrame _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if (( value == _this->FrameNumber ) && ( _this->animFrameNumber == -1 ))
    return;

  _this->FrameNumber = value;

  if ( _this->timer == 0 )
    _this->animFrameNumber = -1;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Frame.OnSetBitmap()' */
void ViewsFrame_OnSetBitmap( ViewsFrame _this, ResourcesBitmap value )
{
  if ( value == _this->Bitmap )
    return;

  if (( _this->Bitmap != 0 ) && _this->Bitmap->Mutable )
    EwDetachObjObserver( EwNewSlot( _this, ViewsFrame_observerSlot ), (XObject)_this->Bitmap, 
      0 );

  _this->Bitmap = value;
  _this->animFrameNumber = -1;

  if (( value != 0 ) && value->Mutable )
    EwAttachObjObserver( EwNewSlot( _this, ViewsFrame_observerSlot ), (XObject)value, 
      0 );

  if ( _this->Animated )
  {
    ViewsFrame_OnSetAnimated( _this, 0 );
    ViewsFrame_OnSetAnimated( _this, 1 );
  }

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* Variants derived from the class : 'Views::Frame' */
EW_DEFINE_CLASS_VARIANTS( ViewsFrame )
EW_END_OF_CLASS_VARIANTS( ViewsFrame )

/* Virtual Method Table (VMT) for the class : 'Views::Frame' */
EW_DEFINE_CLASS( ViewsFrame, CoreRectView, timer, OnFinished, animFrameNumber, animFrameNumber, 
                 animFrameNumber, animFrameNumber, "Views::Frame" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  ViewsFrame_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreView_ChangeViewState,
  CoreRectView_OnSetBounds,
EW_END_OF_CLASS( ViewsFrame )

/* Initializer for the class 'Views::Image' */
void ViewsImage__Init( ViewsImage _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreRectView__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ViewsImage );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ViewsImage );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Orientation = ViewsOrientationNormal;
  _this->Quality = 1;
  _this->ColorBL = _Const0000;
  _this->ColorBR = _Const0000;
  _this->ColorTR = _Const0000;
  _this->ColorTL = _Const0000;
  _this->Endless = 1;
  _this->Alignment = ViewsImageAlignmentAlignHorzCenter | ViewsImageAlignmentAlignVertCenter;
  _this->Opacity = 255;
}

/* Re-Initializer for the class 'Views::Image' */
void ViewsImage__ReInit( ViewsImage _this )
{
  /* At first re-initialize the super class ... */
  CoreRectView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Views::Image' */
void ViewsImage__Done( ViewsImage _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreRectView );

  /* Don't forget to deinitialize the super class ... */
  CoreRectView__Done( &_this->_Super );
}

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void ViewsImage_Draw( ViewsImage _this, GraphicsCanvas aCanvas, XRect aClip, XPoint 
  aOffset, XInt32 aOpacity, XBool aBlend )
{
  XInt32 frameNr = _this->FrameNumber;
  XRect area;
  XPoint size;
  XEnum orient;
  XColor ctl;
  XColor ctr;
  XColor cbr;
  XColor cbl;
  XInt32 opacity;

  if ( _this->animFrameNumber >= 0 )
    frameNr = _this->animFrameNumber;

  if (( _this->Bitmap == 0 ) || ( frameNr >= _this->Bitmap->NoOfFrames ))
    return;

  ResourcesBitmap__Update( _this->Bitmap );
  area = ViewsImage_GetContentArea( _this );
  size = _this->Bitmap->FrameSize;
  orient = _this->Orientation;

  if ( EwIsRectEmpty( area ))
    return;

  ctl = _this->ColorTL;
  ctr = _this->ColorTR;
  cbr = _this->ColorBR;
  cbl = _this->ColorBL;
  opacity = ((( aOpacity + 1 ) * _this->Opacity ) >> 8 ) + 1;
  aBlend = (XBool)( aBlend && (( _this->Super2.viewState & CoreViewStateAlphaBlended ) 
  == CoreViewStateAlphaBlended ));

  if ( opacity < 256 )
  {
    ctl.Alpha = (XUInt8)(( ctl.Alpha * opacity ) >> 8 );
    ctr.Alpha = (XUInt8)(( ctr.Alpha * opacity ) >> 8 );
    cbr.Alpha = (XUInt8)(( cbr.Alpha * opacity ) >> 8 );
    cbl.Alpha = (XUInt8)(( cbl.Alpha * opacity ) >> 8 );
  }

  if ( !EwCompPoint( EwGetRectSize( area ), size ) && ( orient == ViewsOrientationNormal ))
    GraphicsCanvas_CopyBitmap( aCanvas, aClip, _this->Bitmap, frameNr, EwMoveRectPos( 
    _this->Super1.Bounds, aOffset ), EwMovePointNeg( _this->Super1.Bounds.Point1, 
    area.Point1 ), ctl, ctr, cbr, cbl, aBlend );
  else
    if ( orient == ViewsOrientationNormal )
      GraphicsCanvas_ScaleBitmap( aCanvas, aClip, _this->Bitmap, frameNr, EwMoveRectPos( 
      area, aOffset ), EwNewRect2Point( _Const0002, size ), ctl, ctr, cbr, cbl, 
      aBlend, _this->Quality );
    else
    {
      XFloat left = (XFloat)( area.Point1.X + aOffset.X );
      XFloat top = (XFloat)( area.Point1.Y + aOffset.Y );
      XFloat right = (XFloat)( area.Point2.X + aOffset.X );
      XFloat bottom = (XFloat)( area.Point2.Y + aOffset.Y );

      if ( orient == ViewsOrientationRotated_90 )
        GraphicsCanvas_WarpBitmap( aCanvas, aClip, _this->Bitmap, frameNr, left, 
        bottom, 1.000000f, left, top, 1.000000f, right, top, 1.000000f, right, bottom, 
        1.000000f, EwNewRect2Point( _Const0002, size ), cbl, ctl, ctr, cbr, aBlend, 
        _this->Quality );
      else
        if ( orient == ViewsOrientationRotated_180 )
          GraphicsCanvas_WarpBitmap( aCanvas, aClip, _this->Bitmap, frameNr, right, 
          bottom, 1.000000f, left, bottom, 1.000000f, left, top, 1.000000f, right, 
          top, 1.000000f, EwNewRect2Point( _Const0002, size ), cbr, cbl, ctl, ctr, 
          aBlend, _this->Quality );
        else
          if ( orient == ViewsOrientationRotated_270 )
            GraphicsCanvas_WarpBitmap( aCanvas, aClip, _this->Bitmap, frameNr, right, 
            top, 1.000000f, right, bottom, 1.000000f, left, bottom, 1.000000f, left, 
            top, 1.000000f, EwNewRect2Point( _Const0002, size ), ctr, cbr, cbl, 
            ctl, aBlend, _this->Quality );
    }
}

/* 'C' function for method : 'Views::Image.observerSlot()' */
void ViewsImage_observerSlot( ViewsImage _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ((( _this->AutoSize && ( _this->Bitmap != 0 )) && ( _this->Bitmap->FrameSize.X 
      > 0 )) && ( _this->Bitmap->FrameSize.Y > 0 ))
    CoreRectView__OnSetBounds( _this, EwMoveRectNeg( ViewsImage_GetContentArea( 
    _this ), _this->ScrollOffset ));

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Image.timerSlot()' */
void ViewsImage_timerSlot( ViewsImage _this, XObject sender )
{
  XInt32 frameNr;
  XInt32 period;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  frameNr = _this->animFrameNumber;
  period = 0;

  if ( _this->Bitmap != 0 )
    period = _this->Bitmap->NoOfFrames * _this->Bitmap->FrameDelay;

  if ((( _this->timer != 0 ) && ( _this->animFrameNumber < 0 )) && ( period > 0 ))
    _this->startTime = _this->timer->Time - ( _this->FrameNumber * _this->Bitmap->FrameDelay );

  if (( _this->timer != 0 ) && ( period > 0 ))
  {
    XInt32 time = (XInt32)( _this->timer->Time - _this->startTime );
    frameNr = time / _this->Bitmap->FrameDelay;

    if ( time >= period )
    {
      frameNr = frameNr % _this->Bitmap->NoOfFrames;
      _this->startTime = _this->timer->Time - ( time % period );

      if ( !_this->Endless )
      {
        frameNr = _this->Bitmap->NoOfFrames - 1;
        period = 0;
      }
    }
  }

  if ((( frameNr != _this->animFrameNumber ) && ( _this->Super2.Owner != 0 )) && 
      (( _this->Super2.viewState & CoreViewStateVisible ) == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  _this->animFrameNumber = frameNr;

  if (( period == 0 ) && ( _this->timer != 0 ))
  {
    EwDetachObjObserver( EwNewSlot( _this, ViewsImage_timerSlot ), (XObject)_this->timer, 
      0 );
    _this->timer = 0;
    EwSignal( _this->OnFinished, ((XObject)_this ));
  }
}

/* 'C' function for method : 'Views::Image.OnSetColor()' */
void ViewsImage_OnSetColor( ViewsImage _this, XColor value )
{
  if ((( !EwCompColor( value, _this->ColorTL ) && !EwCompColor( value, _this->ColorTR )) 
      && !EwCompColor( value, _this->ColorBL )) && !EwCompColor( value, _this->ColorBR ))
    return;

  _this->ColorTL = value;
  _this->ColorTR = value;
  _this->ColorBL = value;
  _this->ColorBR = value;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Image.OnSetAnimated()' */
void ViewsImage_OnSetAnimated( ViewsImage _this, XBool value )
{
  if ( _this->Animated == value )
    return;

  _this->Animated = value;
  _this->animFrameNumber = -1;

  if ( !value && ( _this->timer != 0 ))
  {
    EwDetachObjObserver( EwNewSlot( _this, ViewsImage_timerSlot ), (XObject)_this->timer, 
      0 );
    _this->timer = 0;
  }

  if ( value )
  {
    _this->timer = ((CoreTimer)EwGetAutoObject( &EffectsEffectTimer, EffectsEffectTimerClass ));
    EwAttachObjObserver( EwNewSlot( _this, ViewsImage_timerSlot ), (XObject)_this->timer, 
      0 );
    EwPostSignal( EwNewSlot( _this, ViewsImage_timerSlot ), ((XObject)_this ));
  }

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Image.OnSetAlignment()' */
void ViewsImage_OnSetAlignment( ViewsImage _this, XSet value )
{
  if ( value == _this->Alignment )
    return;

  _this->Alignment = value;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Image.OnSetFrameNumber()' */
void ViewsImage_OnSetFrameNumber( ViewsImage _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if (( value == _this->FrameNumber ) && ( _this->animFrameNumber == -1 ))
    return;

  _this->FrameNumber = value;

  if ( _this->timer == 0 )
    _this->animFrameNumber = -1;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Image.OnSetBitmap()' */
void ViewsImage_OnSetBitmap( ViewsImage _this, ResourcesBitmap value )
{
  if ( value == _this->Bitmap )
    return;

  if (( _this->Bitmap != 0 ) && _this->Bitmap->Mutable )
    EwDetachObjObserver( EwNewSlot( _this, ViewsImage_observerSlot ), (XObject)_this->Bitmap, 
      0 );

  _this->Bitmap = value;
  _this->animFrameNumber = -1;

  if (( value != 0 ) && value->Mutable )
    EwAttachObjObserver( EwNewSlot( _this, ViewsImage_observerSlot ), (XObject)value, 
      0 );

  if ( _this->Animated )
  {
    ViewsImage_OnSetAnimated( _this, 0 );
    ViewsImage_OnSetAnimated( _this, 1 );
  }

  if ((( _this->AutoSize && ( value != 0 )) && ( value->FrameSize.X > 0 )) && ( 
      value->FrameSize.Y > 0 ))
    CoreRectView__OnSetBounds( _this, EwMoveRectNeg( ViewsImage_GetContentArea( 
    _this ), _this->ScrollOffset ));

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Image.OnSetOpacity()' */
void ViewsImage_OnSetOpacity( ViewsImage _this, XInt32 value )
{
  if ( value > 255 )
    value = 255;

  if ( value < 0 )
    value = 0;

  if ( value == _this->Opacity )
    return;

  _this->Opacity = value;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* The method GetContentArea() returns the position and the size of an area where 
   the view will show the bitmap. This area is expressed in coordinates relative 
   to the top-left corner of the view's @Owner. The method takes in account all 
   properties which do affect the position and the alignment of the bitmap, e.g. 
   @Alignment or @ScrollOffset. */
XRect ViewsImage_GetContentArea( ViewsImage _this )
{
  XEnum orient;
  XSet align;
  XPoint size;
  XRect bounds;
  XInt32 width;
  XInt32 height;
  XRect rd;
  XRect rs;

  if ( _this->Bitmap == 0 )
    return _Const0003;

  orient = _this->Orientation;
  align = _this->Alignment;
  size = _this->Bitmap->FrameSize;
  bounds = _this->Super1.Bounds;
  width = EwGetRectW( bounds );
  height = EwGetRectH( bounds );

  if (( size.X == 0 ) || ( size.Y == 0 ))
    return _Const0003;

  if (( orient == ViewsOrientationRotated_90 ) || ( orient == ViewsOrientationRotated_270 ))
  {
    width = height;
    height = EwGetRectW( bounds );
  }

  rd = EwNewRect( 0, 0, width, height );
  rs = rd;

  if ((( align & ViewsImageAlignmentScaleToFill ) == ViewsImageAlignmentScaleToFill ))
  {
    XInt32 scaleX = (( EwGetRectW( rd ) << 16 ) + ( size.X / 2 )) / size.X;
    XInt32 scaleY = (( EwGetRectH( rd ) << 16 ) + ( size.Y / 2 )) / size.Y;
    XInt32 scale = scaleX;

    if ( scaleY > scale )
      scale = scaleY;

    rs = EwSetRectW( rs, (( size.X * scale ) + 32768 ) >> 16 );
    rs = EwSetRectH( rs, (( size.Y * scale ) + 32768 ) >> 16 );
  }
  else
    if ((( align & ViewsImageAlignmentScaleToFit ) == ViewsImageAlignmentScaleToFit ))
    {
      XInt32 scaleX = (( EwGetRectW( rd ) << 16 ) + ( size.X / 2 )) / size.X;
      XInt32 scaleY = (( EwGetRectH( rd ) << 16 ) + ( size.Y / 2 )) / size.Y;
      XInt32 scale = scaleX;

      if ( scaleY < scale )
        scale = scaleY;

      rs = EwSetRectW( rs, (( size.X * scale ) + 32768 ) >> 16 );
      rs = EwSetRectH( rs, (( size.Y * scale ) + 32768 ) >> 16 );
    }
    else
      if ( !(( align & ViewsImageAlignmentStretchToFill ) == ViewsImageAlignmentStretchToFill ))
        rs = EwSetRectSize( rs, size );

  if ( EwGetRectW( rs ) != EwGetRectW( rd ))
  {
    if ((( align & ViewsImageAlignmentAlignHorzRight ) == ViewsImageAlignmentAlignHorzRight ))
      rs = EwSetRectX( rs, rd.Point2.X - EwGetRectW( rs ));
    else
      if ((( align & ViewsImageAlignmentAlignHorzCenter ) == ViewsImageAlignmentAlignHorzCenter ))
        rs = EwSetRectX( rs, ( rd.Point1.X + ( EwGetRectW( rd ) / 2 )) - ( EwGetRectW( 
        rs ) / 2 ));
  }

  if ( EwGetRectH( rs ) != EwGetRectH( rd ))
  {
    if ((( align & ViewsImageAlignmentAlignVertBottom ) == ViewsImageAlignmentAlignVertBottom ))
      rs = EwSetRectY( rs, rd.Point2.Y - EwGetRectH( rs ));
    else
      if ((( align & ViewsImageAlignmentAlignVertCenter ) == ViewsImageAlignmentAlignVertCenter ))
        rs = EwSetRectY( rs, ( rd.Point1.Y + ( EwGetRectH( rd ) / 2 )) - ( EwGetRectH( 
        rs ) / 2 ));
  }

  if ( orient == ViewsOrientationNormal )
    rs = EwMoveRectPos( rs, bounds.Point1 );
  else
    if ( orient == ViewsOrientationRotated_90 )
    {
      XPoint org = EwNewPoint( bounds.Point1.X + rs.Point1.Y, bounds.Point2.Y - 
        rs.Point2.X );
      rs = EwNewRect2Point( org, EwMovePointPos( org, EwNewPoint( EwGetRectH( rs ), 
      EwGetRectW( rs ))));
    }
    else
      if ( orient == ViewsOrientationRotated_180 )
      {
        XPoint org = EwNewPoint( bounds.Point2.X - rs.Point2.X, bounds.Point2.Y 
          - rs.Point2.Y );
        rs = EwNewRect2Point( org, EwMovePointPos( org, EwGetRectSize( rs )));
      }
      else
        if ( orient == ViewsOrientationRotated_270 )
        {
          XPoint org = EwNewPoint( bounds.Point2.X - rs.Point2.Y, bounds.Point1.Y 
            + rs.Point1.X );
          rs = EwNewRect2Point( org, EwMovePointPos( org, EwNewPoint( EwGetRectH( 
          rs ), EwGetRectW( rs ))));
        }

  return EwMoveRectPos( rs, _this->ScrollOffset );
}

/* Default onget method for the property 'Opacity' */
XInt32 ViewsImage_OnGetOpacity( ViewsImage _this )
{
  return _this->Opacity;
}

/* Variants derived from the class : 'Views::Image' */
EW_DEFINE_CLASS_VARIANTS( ViewsImage )
EW_END_OF_CLASS_VARIANTS( ViewsImage )

/* Virtual Method Table (VMT) for the class : 'Views::Image' */
EW_DEFINE_CLASS( ViewsImage, CoreRectView, timer, OnFinished, startTime, startTime, 
                 startTime, startTime, "Views::Image" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  ViewsImage_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreView_ChangeViewState,
  CoreRectView_OnSetBounds,
EW_END_OF_CLASS( ViewsImage )

/* Initializer for the class 'Views::Wallpaper' */
void ViewsWallpaper__Init( ViewsWallpaper _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreRectView__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ViewsWallpaper );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ViewsWallpaper );

  /* ... and initialize objects, variables, properties, etc. */
  _this->ColorBL = _Const0000;
  _this->ColorBR = _Const0000;
  _this->ColorTR = _Const0000;
  _this->ColorTL = _Const0000;
  _this->Endless = 1;
  _this->Opacity = 255;
}

/* Re-Initializer for the class 'Views::Wallpaper' */
void ViewsWallpaper__ReInit( ViewsWallpaper _this )
{
  /* At first re-initialize the super class ... */
  CoreRectView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Views::Wallpaper' */
void ViewsWallpaper__Done( ViewsWallpaper _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreRectView );

  /* Don't forget to deinitialize the super class ... */
  CoreRectView__Done( &_this->_Super );
}

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void ViewsWallpaper_Draw( ViewsWallpaper _this, GraphicsCanvas aCanvas, XRect aClip, 
  XPoint aOffset, XInt32 aOpacity, XBool aBlend )
{
  XInt32 frameNr = _this->FrameNumber;
  XColor ctl;
  XColor ctr;
  XColor cbr;
  XColor cbl;
  XInt32 opacity;

  if ( _this->animFrameNumber >= 0 )
    frameNr = _this->animFrameNumber;

  if (( _this->Bitmap == 0 ) || ( frameNr >= _this->Bitmap->NoOfFrames ))
    return;

  ResourcesBitmap__Update( _this->Bitmap );
  ctl = _this->ColorTL;
  ctr = _this->ColorTR;
  cbr = _this->ColorBR;
  cbl = _this->ColorBL;
  opacity = ((( aOpacity + 1 ) * _this->Opacity ) >> 8 ) + 1;
  aBlend = (XBool)( aBlend && (( _this->Super2.viewState & CoreViewStateAlphaBlended ) 
  == CoreViewStateAlphaBlended ));

  if ( opacity < 256 )
  {
    ctl.Alpha = (XUInt8)(( ctl.Alpha * opacity ) >> 8 );
    ctr.Alpha = (XUInt8)(( ctr.Alpha * opacity ) >> 8 );
    cbr.Alpha = (XUInt8)(( cbr.Alpha * opacity ) >> 8 );
    cbl.Alpha = (XUInt8)(( cbl.Alpha * opacity ) >> 8 );
  }

  GraphicsCanvas_TileBitmap( aCanvas, aClip, _this->Bitmap, frameNr, EwMoveRectPos( 
  _this->Super1.Bounds, aOffset ), EwNewRect2Point( _Const0002, _this->Bitmap->FrameSize ), 
  EwNegPoint( _this->ScrollOffset ), ctl, ctr, cbr, cbl, aBlend );
}

/* 'C' function for method : 'Views::Wallpaper.observerSlot()' */
void ViewsWallpaper_observerSlot( ViewsWallpaper _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Wallpaper.timerSlot()' */
void ViewsWallpaper_timerSlot( ViewsWallpaper _this, XObject sender )
{
  XInt32 frameNr;
  XInt32 period;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  frameNr = _this->animFrameNumber;
  period = 0;

  if ( _this->Bitmap != 0 )
    period = _this->Bitmap->NoOfFrames * _this->Bitmap->FrameDelay;

  if ((( _this->timer != 0 ) && ( _this->animFrameNumber < 0 )) && ( period > 0 ))
    _this->startTime = _this->timer->Time - ( _this->FrameNumber * _this->Bitmap->FrameDelay );

  if (( _this->timer != 0 ) && ( period > 0 ))
  {
    XInt32 time = (XInt32)( _this->timer->Time - _this->startTime );
    frameNr = time / _this->Bitmap->FrameDelay;

    if ( time >= period )
    {
      frameNr = frameNr % _this->Bitmap->NoOfFrames;
      _this->startTime = _this->timer->Time - ( time % period );

      if ( !_this->Endless )
      {
        frameNr = _this->Bitmap->NoOfFrames - 1;
        period = 0;
      }
    }
  }

  if ((( frameNr != _this->animFrameNumber ) && ( _this->Super2.Owner != 0 )) && 
      (( _this->Super2.viewState & CoreViewStateVisible ) == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  _this->animFrameNumber = frameNr;

  if (( period == 0 ) && ( _this->timer != 0 ))
  {
    EwDetachObjObserver( EwNewSlot( _this, ViewsWallpaper_timerSlot ), (XObject)_this->timer, 
      0 );
    _this->timer = 0;
    EwSignal( _this->OnFinished, ((XObject)_this ));
  }
}

/* 'C' function for method : 'Views::Wallpaper.OnSetAnimated()' */
void ViewsWallpaper_OnSetAnimated( ViewsWallpaper _this, XBool value )
{
  if ( _this->Animated == value )
    return;

  _this->Animated = value;
  _this->animFrameNumber = -1;

  if ( !value && ( _this->timer != 0 ))
  {
    EwDetachObjObserver( EwNewSlot( _this, ViewsWallpaper_timerSlot ), (XObject)_this->timer, 
      0 );
    _this->timer = 0;
  }

  if ( value )
  {
    _this->timer = ((CoreTimer)EwGetAutoObject( &EffectsEffectTimer, EffectsEffectTimerClass ));
    EwAttachObjObserver( EwNewSlot( _this, ViewsWallpaper_timerSlot ), (XObject)_this->timer, 
      0 );
    EwPostSignal( EwNewSlot( _this, ViewsWallpaper_timerSlot ), ((XObject)_this ));
  }

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Wallpaper.OnSetScrollOffset()' */
void ViewsWallpaper_OnSetScrollOffset( ViewsWallpaper _this, XPoint value )
{
  if ( !EwCompPoint( value, _this->ScrollOffset ))
    return;

  _this->ScrollOffset = value;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Wallpaper.OnSetBitmap()' */
void ViewsWallpaper_OnSetBitmap( ViewsWallpaper _this, ResourcesBitmap value )
{
  if ( value == _this->Bitmap )
    return;

  if (( _this->Bitmap != 0 ) && _this->Bitmap->Mutable )
    EwDetachObjObserver( EwNewSlot( _this, ViewsWallpaper_observerSlot ), (XObject)_this->Bitmap, 
      0 );

  _this->Bitmap = value;
  _this->animFrameNumber = -1;

  if (( value != 0 ) && value->Mutable )
    EwAttachObjObserver( EwNewSlot( _this, ViewsWallpaper_observerSlot ), (XObject)value, 
      0 );

  if ( _this->Animated )
  {
    ViewsWallpaper_OnSetAnimated( _this, 0 );
    ViewsWallpaper_OnSetAnimated( _this, 1 );
  }

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Wallpaper.OnSetAlphaBlended()' */
void ViewsWallpaper_OnSetAlphaBlended( ViewsWallpaper _this, XBool value )
{
  if ( value )
    CoreView__ChangeViewState( _this, CoreViewStateAlphaBlended, 0 );
  else
    CoreView__ChangeViewState( _this, 0, CoreViewStateAlphaBlended );
}

/* Variants derived from the class : 'Views::Wallpaper' */
EW_DEFINE_CLASS_VARIANTS( ViewsWallpaper )
EW_END_OF_CLASS_VARIANTS( ViewsWallpaper )

/* Virtual Method Table (VMT) for the class : 'Views::Wallpaper' */
EW_DEFINE_CLASS( ViewsWallpaper, CoreRectView, timer, OnFinished, animFrameNumber, 
                 animFrameNumber, animFrameNumber, animFrameNumber, "Views::Wallpaper" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  ViewsWallpaper_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreView_ChangeViewState,
  CoreRectView_OnSetBounds,
EW_END_OF_CLASS( ViewsWallpaper )

/* Initializer for the class 'Views::Text' */
void ViewsText__Init( ViewsText _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreRectView__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ViewsText );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ViewsText );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Orientation = ViewsOrientationNormal;
  _this->ColorBL = _Const0000;
  _this->ColorBR = _Const0000;
  _this->ColorTR = _Const0000;
  _this->ColorTL = _Const0000;
  _this->Alignment = ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter;
  _this->Opacity = 255;
}

/* Re-Initializer for the class 'Views::Text' */
void ViewsText__ReInit( ViewsText _this )
{
  /* At first re-initialize the super class ... */
  CoreRectView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Views::Text' */
void ViewsText__Done( ViewsText _this )
{
  /* Call the user defined destructor of the class */
  ViewsText_Done( _this );

  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreRectView );

  /* Don't forget to deinitialize the super class ... */
  CoreRectView__Done( &_this->_Super );
}

/* 'C' function for method : 'Views::Text.Done()' */
void ViewsText_Done( ViewsText _this )
{
  if ( _this->bidiContext != 0 )
  {
    ViewsText_freeBidi( _this, _this->bidiContext );
    _this->bidiContext = 0;
  }
}

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void ViewsText_Draw( ViewsText _this, GraphicsCanvas aCanvas, XRect aClip, XPoint 
  aOffset, XInt32 aOpacity, XBool aBlend )
{
  XSet align;
  XEnum orient;
  ResourcesFont font;
  XRect rd;
  XColor ctl;
  XColor ctr;
  XColor cbr;
  XColor cbl;
  XInt32 opacity;
  XInt32 noOfRows;
  XRect area;
  XPoint ofs;
  XInt32 leading;
  XInt32 rowHeight;
  XInt32 clipY1;
  XInt32 clipY2;
  XInt32 areaW;
  XInt32 y;
  XInt32 i;
  XInt32 c;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aBlend );

  if ( !EwCompString( _this->flowString, 0 ) || ( _this->Font == 0 ))
    return;

  align = _this->Alignment;
  orient = _this->Orientation;
  font = _this->Font;
  rd = EwMoveRectPos( _this->Super1.Bounds, aOffset );
  ctl = _this->ColorTL;
  ctr = _this->ColorTR;
  cbr = _this->ColorBR;
  cbl = _this->ColorBL;
  opacity = ((( aOpacity + 1 ) * _this->Opacity ) >> 8 ) + 1;
  noOfRows = EwGetStringChar( _this->flowString, 0 );
  area = EwMoveRectPos( ViewsText_GetContentArea( _this ), aOffset );
  ofs = EwNewPoint( rd.Point1.X - area.Point1.X, ( rd.Point1.Y - area.Point1.Y ) 
  - font->Ascent );

  if ( noOfRows < 1 )
    return;

  if ( opacity < 256 )
  {
    ctl.Alpha = (XUInt8)(( ctl.Alpha * opacity ) >> 8 );
    ctr.Alpha = (XUInt8)(( ctr.Alpha * opacity ) >> 8 );
    cbr.Alpha = (XUInt8)(( cbr.Alpha * opacity ) >> 8 );
    cbl.Alpha = (XUInt8)(( cbl.Alpha * opacity ) >> 8 );
  }

  if ((( align & ViewsTextAlignmentAlignHorzAuto ) == ViewsTextAlignmentAlignHorzAuto ))
  {
    if ( ViewsText_IsBaseDirectionRTL( _this ))
      align = ( align & ~ViewsTextAlignmentAlignHorzAuto ) | ViewsTextAlignmentAlignHorzRight;
    else
      align = ( align & ~ViewsTextAlignmentAlignHorzAuto ) | ViewsTextAlignmentAlignHorzLeft;
  }

  if ((( noOfRows == 1 ) && !(( align & ViewsTextAlignmentAlignHorzJustified ) == 
      ViewsTextAlignmentAlignHorzJustified )) && ( orient == ViewsOrientationNormal ))
  {
    GraphicsCanvas_DrawText( aCanvas, aClip, font, _this->flowString, 2, EwGetStringChar( 
    _this->flowString, 1 ) - 1, rd, ofs, 0, orient, ctl, ctr, cbr, cbl, 1 );
    return;
  }

  leading = font->Leading;

  if ( _this->RowDistance > 0 )
    leading = ( _this->RowDistance - font->Ascent ) - font->Descent;

  rowHeight = ( font->Ascent + font->Descent ) + leading;
  clipY1 = aClip.Point1.Y - area.Point1.Y;
  clipY2 = aClip.Point2.Y - area.Point1.Y;
  areaW = EwGetRectW( area );
  y = 0;
  i = 1;
  c = EwGetStringChar( _this->flowString, i );

  if ( orient == ViewsOrientationRotated_90 )
  {
    ofs = EwNewPoint( area.Point2.Y - rd.Point2.Y, ( rd.Point1.X - area.Point1.X ) 
    - font->Ascent );
    clipY1 = aClip.Point1.X - area.Point1.X;
    clipY2 = aClip.Point2.X - area.Point1.X;
    areaW = EwGetRectH( area );
  }
  else
    if ( orient == ViewsOrientationRotated_180 )
    {
      ofs = EwNewPoint( area.Point2.X - rd.Point2.X, ( area.Point2.Y - rd.Point2.Y ) 
      - font->Ascent );
      clipY1 = area.Point2.Y - aClip.Point2.Y;
      clipY2 = area.Point2.Y - aClip.Point1.Y;
    }
    else
      if ( orient == ViewsOrientationRotated_270 )
      {
        ofs = EwNewPoint( rd.Point1.Y - area.Point1.Y, ( area.Point2.X - rd.Point2.X ) 
        - font->Ascent );
        clipY1 = area.Point2.X - aClip.Point2.X;
        clipY2 = area.Point2.X - aClip.Point1.X;
        areaW = EwGetRectH( area );
      }

  while ((( y + rowHeight ) < clipY1 ) && ( c > 0 ))
  {
    i = i + c;
    y = y + rowHeight;
    c = EwGetStringChar( _this->flowString, i );
  }

  while (( y < clipY2 ) && ( c > 0 ))
  {
    XPoint ofs2 = EwMovePointNeg( ofs, EwNewPoint( 0, y ));
    XInt32 rw = 0;
    XBool justified = 0;

    if ((((( align & ViewsTextAlignmentAlignHorzJustified ) == ViewsTextAlignmentAlignHorzJustified ) 
        && ( EwGetStringChar( _this->flowString, ( i + c ) - 1 ) != 0x000A )) && 
        ( EwGetStringChar( _this->flowString, i + 1 ) != 0x000A )) && ( EwGetStringChar( 
        _this->flowString, i + c ) != 0x0000 ))
      justified = 1;

    if ( justified && !!( align & ( ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignHorzRight )))
    {
      XInt32 rowEnd = i + c;
      XInt32 blank1 = EwStringFindChar( _this->flowString, 0x0020, i + 1 );
      XInt32 blank2 = EwStringFindChar( _this->flowString, 0x00A0, i + 1 );

      if ((( blank1 < 0 ) || ( blank1 >= rowEnd )) && (( blank2 < 0 ) || ( blank2 
          >= rowEnd )))
        justified = 0;
    }

    if ( justified )
      rw = areaW;
    else
      if ((( align & ViewsTextAlignmentAlignHorzRight ) == ViewsTextAlignmentAlignHorzRight ))
        ofs2.X = (( ofs2.X - areaW ) + ResourcesFont_GetTextAdvance( font, _this->flowString, 
        i + 1, c - 1 ));
      else
        if ((( align & ViewsTextAlignmentAlignHorzCenter ) == ViewsTextAlignmentAlignHorzCenter ))
          ofs2.X = (( ofs2.X - ( areaW / 2 )) + ( ResourcesFont_GetTextAdvance( 
          font, _this->flowString, i + 1, c - 1 ) / 2 ));

    GraphicsCanvas_DrawText( aCanvas, aClip, font, _this->flowString, i + 1, c - 
    1, rd, ofs2, rw, orient, ctl, ctr, cbr, cbl, 1 );
    i = i + c;
    y = y + rowHeight;
    c = EwGetStringChar( _this->flowString, i );
  }
}

/* 'C' function for method : 'Views::Text.OnSetBounds()' */
void ViewsText_OnSetBounds( ViewsText _this, XRect value )
{
  XBool resized;

  if ( !EwCompRect( value, _this->Super1.Bounds ))
    return;

  if (( _this->Orientation == ViewsOrientationNormal ) || ( _this->Orientation == 
      ViewsOrientationRotated_180 ))
    resized = (XBool)( EwGetRectW( _this->Super1.Bounds ) != EwGetRectW( value ));
  else
    resized = (XBool)( EwGetRectH( _this->Super1.Bounds ) != EwGetRectH( value ));

  if (((( resized && ( _this->WrapWidth == 0 )) && _this->WrapText ) && _this->reparsed ) 
      && !(( _this->Super2.viewState & CoreViewStateUpdatingLayout ) == CoreViewStateUpdatingLayout ))
  {
    _this->flowString = 0;
    _this->reparsed = 0;
    EwPostSignal( EwNewSlot( _this, ViewsText_preReparseSlot ), ((XObject)_this ));
  }

  if ((( _this->Ellipsis && _this->reparsed ) && EwCompPoint( EwGetRectSize( _this->Super1.Bounds ), 
      EwGetRectSize( value ))) && !(( _this->Super2.viewState & CoreViewStateUpdatingLayout ) 
      == CoreViewStateUpdatingLayout ))
  {
    _this->flowString = 0;
    _this->reparsed = 0;
    EwPostSignal( EwNewSlot( _this, ViewsText_preReparseSlot ), ((XObject)_this ));
  }

  CoreRectView_OnSetBounds((CoreRectView)_this, value );
  EwPostSignal( EwNewSlot( _this, ViewsText_preOnUpdateSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::Text.freeBidi()' */
void ViewsText_freeBidi( ViewsText _this, XHandle aBidi )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );

  if ( aBidi == 0 )
    return;

  EwFreeBidi( aBidi );
}

/* 'C' function for method : 'Views::Text.createBidi()' */
XHandle ViewsText_createBidi( ViewsText _this, XInt32 aSize )
{
  XHandle bidi;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );

  bidi = 0;
  bidi = EwCreateBidi( aSize );
  return bidi;
}

/* 'C' function for method : 'Views::Text.preOnUpdateSlot()' */
void ViewsText_preOnUpdateSlot( ViewsText _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( _this->OnUpdate, ((XObject)_this ));
}

/* 'C' function for method : 'Views::Text.preReparseSlot()' */
void ViewsText_preReparseSlot( ViewsText _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( EwNewSlot( _this, ViewsText_reparseSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::Text.reparseSlot()' */
void ViewsText_reparseSlot( ViewsText _this, XObject sender )
{
  XEnum orient;
  XInt32 width;
  XInt32 height;
  XInt32 maxWidth;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->reparsed )
    return;

  orient = _this->Orientation;
  width = EwGetRectW( _this->Super1.Bounds );
  height = EwGetRectH( _this->Super1.Bounds );
  maxWidth = -1;

  if (( orient == ViewsOrientationRotated_90 ) || ( orient == ViewsOrientationRotated_270 ))
  {
    width = height;
    height = EwGetRectW( _this->Super1.Bounds );
  }

  if ( _this->WrapText )
  {
    if ( _this->WrapWidth > 0 )
      maxWidth = _this->WrapWidth;
    else
      if ( !_this->AutoSize )
        maxWidth = width - ( _this->Padding * 2 );
      else
        maxWidth = width;

    if ( maxWidth < 0 )
      maxWidth = 1;
  }

  if ( _this->bidiContext != 0 )
  {
    ViewsText_freeBidi( _this, _this->bidiContext );
    _this->bidiContext = 0;
  }

  _this->reparsed = 1;

  if (( EwCompString( _this->String, 0 ) != 0 ) && ( _this->Font != 0 ))
  {
    XInt32 length = EwGetStringLength( _this->String );

    if ( _this->EnableBidiText )
      _this->bidiContext = ViewsText_createBidi( _this, length );

    _this->flowString = EwShareString( ResourcesFont_ParseFlowString( _this->Font, 
    _this->String, 0, maxWidth, length, _this->bidiContext ));

    if (( _this->bidiContext != 0 ) && !ViewsText_IsBidiText( _this ))
    {
      ViewsText_freeBidi( _this, _this->bidiContext );
      _this->bidiContext = 0;
    }
  }
  else
    _this->flowString = 0;

  _this->textSize = _Const0002;

  if ((( _this->Ellipsis && ( EwCompString( _this->flowString, 0 ) != 0 )) && !_this->AutoSize ) 
      && ( _this->Font != 0 ))
  {
    XSet align = _this->Alignment;
    ResourcesFont font = _this->Font;
    XInt32 leading = font->Leading;
    XString res = _this->flowString;
    XBool rtl = ViewsText_IsBaseDirectionRTL( _this );
    XInt32 rh;
    XInt32 noOfRows;
    XInt32 maxNoOfRows;
    XBool clipF;
    XBool clipL;
    XInt32 row;
    XInt32 inx;
    XInt32 maxW;

    if ((( align & ViewsTextAlignmentAlignHorzAuto ) == ViewsTextAlignmentAlignHorzAuto ))
    {
      if ( rtl )
        align = ( align & ~ViewsTextAlignmentAlignHorzAuto ) | ViewsTextAlignmentAlignHorzRight;
      else
        align = ( align & ~ViewsTextAlignmentAlignHorzAuto ) | ViewsTextAlignmentAlignHorzLeft;
    }

    if ( _this->RowDistance > 0 )
      leading = ( _this->RowDistance - font->Ascent ) - font->Descent;

    rh = ( font->Ascent + font->Descent ) + leading;
    noOfRows = EwGetStringChar( res, 0 );
    maxNoOfRows = ( height + leading ) / rh;
    clipF = 0;
    clipL = 0;

    if ( maxNoOfRows <= 0 )
      maxNoOfRows = 1;

    if ( noOfRows > maxNoOfRows )
    {
      XInt32 row = 0;
      XInt32 rowF = 0;
      XInt32 rowL = noOfRows - 1;
      XInt32 inxF = 0;
      XInt32 inxL = EwGetStringLength( res );
      XString tmp = 0;

      if ((( align & ViewsTextAlignmentAlignVertBottom ) == ViewsTextAlignmentAlignVertBottom ))
        rowF = noOfRows - maxNoOfRows;
      else
        if ((( align & ViewsTextAlignmentAlignVertCenter ) == ViewsTextAlignmentAlignVertCenter ))
        {
          rowF = ( noOfRows - maxNoOfRows ) / 2;
          rowL = ( rowF + maxNoOfRows ) - 1;
        }
        else
          rowL = maxNoOfRows - 1;

      clipF = (XBool)( rowF > 0 );
      clipL = (XBool)( rowL < ( noOfRows - 1 ));

      for ( inxF = 1; row < rowF; row = row + 1 )
        inxF = inxF + EwGetStringChar( res, inxF );

      if ( clipL )
        for ( inxL = inxF; row < rowL; row = row + 1 )
          inxL = inxL + EwGetStringChar( res, inxL );

      if ( clipF )
      {
        XInt32 len = EwGetStringChar( res, inxF );
        tmp = EwShareString( EwConcatString( EwConcatString( EwLoadString( &_Const0004 ), 
        EwStringMiddle( res, inxF, len )), EwLoadString( &_Const0004 )));
        tmp = EwSetStringChar( tmp, 0, (XChar)( len + 2 ));
        inxF = inxF + len;

        if ( EwGetStringChar( tmp, len ) == 0x000A )
        {
          tmp = EwSetStringChar( tmp, len, 0xFEFF );
          tmp = EwSetStringChar( tmp, len + 1, 0x000A );
        }

        if ( EwGetStringChar( tmp, 2 ) == 0x000A )
        {
          tmp = EwSetStringChar( tmp, 2, 0xFEFF );
          tmp = EwSetStringChar( tmp, 1, 0x000A );
        }
        else
          tmp = EwSetStringChar( tmp, 1, 0xFEFF );
      }

      tmp = EwShareString( EwConcatString( tmp, EwStringMiddle( res, inxF, inxL 
      - inxF )));

      if ( clipL && ( inxL >= inxF ))
      {
        XInt32 len = EwGetStringChar( res, inxL );
        XString tmp2 = EwShareString( EwConcatString( EwConcatString( EwLoadString( 
          &_Const0004 ), EwStringMiddle( res, inxL, len )), EwLoadString( &_Const0004 )));
        tmp2 = EwSetStringChar( tmp2, 0, (XChar)( len + 2 ));
        tmp2 = EwSetStringChar( tmp2, 1, 0xFEFF );

        if ( EwGetStringChar( tmp2, len ) == 0x000A )
        {
          tmp2 = EwSetStringChar( tmp2, len, 0xFEFF );
          tmp2 = EwSetStringChar( tmp2, len + 1, 0x000A );
        }

        if ( EwGetStringChar( tmp2, 2 ) == 0x000A )
        {
          tmp2 = EwSetStringChar( tmp2, 2, 0xFEFF );
          tmp2 = EwSetStringChar( tmp2, 1, 0x000A );
        }
        else
          tmp2 = EwSetStringChar( tmp2, 1, 0xFEFF );

        tmp = EwShareString( EwConcatString( tmp, tmp2 ));
      }

      res = EwShareString( EwConcatCharString((XChar)maxNoOfRows, tmp ));
    }

    row = 0;
    inx = 1;
    maxW = width - ( _this->Padding * 2 );

    if ( _this->WrapText && ( _this->WrapWidth > 0 ))
      maxW = _this->WrapWidth;

    noOfRows = EwGetStringChar( res, 0 );

    for ( ; row < noOfRows; row = row + 1 )
    {
      XBool rowEllipF = (XBool)( clipF && ( row == 0 ));
      XBool rowEllipL = (XBool)( clipL && ( row == ( noOfRows - 1 )));
      XBool colEllipF = 0;
      XBool colEllipL = 0;
      XBool rtl2 = rtl;
      XInt32 start;
      XInt32 len;
      XInt32 inxF;
      XInt32 inxL;
      XInt32 inxF2;
      XInt32 inxL2;

      if (( rtl && rowEllipF ) && !rowEllipL )
      {
        rowEllipF = 0;
        rowEllipL = 1;
      }
      else
        if (( rtl && rowEllipL ) && !rowEllipF )
        {
          rowEllipL = 0;
          rowEllipF = 1;
        }

      start = inx + 1;
      len = EwGetStringChar( res, inx );
      inxF = start;
      inxL = ( start + len ) - 2;
      inxF2 = -1;
      inxL2 = -1;

      if ( !_this->WrapText && ( ResourcesFont_GetTextAdvance( font, res, start, 
          len - 1 ) > maxW ))
      {
        if ((( align & ViewsTextAlignmentAlignHorzRight ) == ViewsTextAlignmentAlignHorzRight ))
          colEllipF = 1;
        else
          if ((( align & ViewsTextAlignmentAlignHorzCenter ) == ViewsTextAlignmentAlignHorzCenter ))
          {
            colEllipF = 1;
            colEllipL = 1;
          }
          else
            colEllipL = 1;
      }

      if ( EwGetStringChar( res, inxF ) == 0x000A )
        inxF = inxF + 1;

      if ( EwGetStringChar( res, inxL ) == 0x000A )
        inxL = inxL - 1;

      while ( colEllipF && ( EwGetStringChar( res, inxF ) == 0xFEFF ))
        inxF = inxF + 1;

      while ( colEllipL && ( EwGetStringChar( res, inxL ) == 0xFEFF ))
        inxL = inxL - 1;

      colEllipF = (XBool)( colEllipF && !rowEllipL );
      colEllipL = (XBool)( colEllipL && !rowEllipF );

      while (((( colEllipF || colEllipL ) || rowEllipF ) || rowEllipL ) && ( inxF 
             < inxL ))
      {
        if (( colEllipF && ( rtl2 || !colEllipL )) || rowEllipF )
        {
          if ( inxF2 > 0 )
            res = EwSetStringChar( res, inxF2, 0xFEFF );

          res = EwSetStringChar( res, inxF, 0x2026 );
          inxF2 = inxF;
          inxF = inxF + 1;
          rtl2 = (XBool)!rtl2;
          rowEllipF = 0;

          if ( ResourcesFont_GetTextAdvance( font, res, start, len - 1 ) <= maxW )
          {
            colEllipF = 0;
            colEllipL = 0;
          }
          else
            colEllipF = (XBool)( colEllipF || !colEllipL );
        }

        if (( colEllipL && ( !rtl2 || !colEllipF )) || rowEllipL )
        {
          if ( inxL2 > 0 )
            res = EwSetStringChar( res, inxL2, 0xFEFF );

          res = EwSetStringChar( res, inxL, 0x2026 );
          inxL2 = inxL;
          inxL = inxL - 1;
          rtl2 = (XBool)!rtl2;
          rowEllipL = 0;

          if ( ResourcesFont_GetTextAdvance( font, res, start, len - 1 ) <= maxW )
          {
            colEllipF = 0;
            colEllipL = 0;
          }
          else
            colEllipL = (XBool)( colEllipL || !colEllipF );
        }
      }

      inx = inx + len;
    }

    _this->textSize = EwNewPoint( ResourcesFont_GetFlowTextAdvance( font, res ), 
    ( EwGetStringChar( res, 0 ) * rh ) - leading );
    _this->flowString = EwShareString( res );
  }

  if ( _this->AutoSize && ( EwCompString( _this->flowString, 0 ) != 0 ))
  {
    XPoint padding = EwNewPoint( _this->Padding, 0 );

    if (( orient == ViewsOrientationRotated_90 ) || ( orient == ViewsOrientationRotated_270 ))
    {
      padding.Y = padding.X;
      padding.X = 0;
    }

    _this->Super2.viewState = _this->Super2.viewState | CoreViewStateUpdatingLayout;
    CoreRectView__OnSetBounds( _this, EwMoveRectNeg( EwInflateRect( ViewsText_GetContentArea( 
    _this ), padding ), _this->ScrollOffset ));
    _this->Super2.viewState = _this->Super2.viewState & ~CoreViewStateUpdatingLayout;
  }

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  EwPostSignal( EwNewSlot( _this, ViewsText_preOnUpdateSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::Text.OnSetEnableBidiText()' */
void ViewsText_OnSetEnableBidiText( ViewsText _this, XBool value )
{
  if ( value == _this->EnableBidiText )
    return;

  _this->EnableBidiText = value;
  _this->flowString = 0;
  _this->reparsed = 0;
  EwPostSignal( EwNewSlot( _this, ViewsText_preReparseSlot ), ((XObject)_this ));
}

/* The onset method for the property 'Ellipsis' changes the ellipsis mode and forces 
   an update. */
void ViewsText_OnSetEllipsis( ViewsText _this, XBool value )
{
  if ( value == _this->Ellipsis )
    return;

  _this->Ellipsis = value;
  _this->flowString = 0;
  _this->reparsed = 0;
  EwPostSignal( EwNewSlot( _this, ViewsText_preReparseSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::Text.OnSetOnUpdate()' */
void ViewsText_OnSetOnUpdate( ViewsText _this, XSlot value )
{
  if ( !EwCompSlot( value, _this->OnUpdate ))
    return;

  _this->OnUpdate = value;

  if (( _this->WrapWidth == 0 ) || ( value.Object != 0 ))
    _this->Super2.viewState = _this->Super2.viewState & ~CoreViewStateFastReshape;
  else
    _this->Super2.viewState = _this->Super2.viewState | CoreViewStateFastReshape;
}

/* 'C' function for method : 'Views::Text.OnSetAutoSize()' */
void ViewsText_OnSetAutoSize( ViewsText _this, XBool value )
{
  if ( value == _this->AutoSize )
    return;

  _this->AutoSize = value;

  if ( value && _this->Ellipsis )
  {
    _this->flowString = 0;
    _this->reparsed = 0;
    EwPostSignal( EwNewSlot( _this, ViewsText_preReparseSlot ), ((XObject)_this ));
  }

  if ( value && _this->reparsed )
  {
    XPoint padding = EwNewPoint( _this->Padding, 0 );

    if (( _this->Orientation == ViewsOrientationRotated_90 ) || ( _this->Orientation 
        == ViewsOrientationRotated_270 ))
    {
      padding.Y = padding.X;
      padding.X = 0;
    }

    _this->Super2.viewState = _this->Super2.viewState | CoreViewStateUpdatingLayout;
    CoreRectView__OnSetBounds( _this, EwMoveRectNeg( EwInflateRect( ViewsText_GetContentArea( 
    _this ), padding ), _this->ScrollOffset ));
    _this->Super2.viewState = _this->Super2.viewState & ~CoreViewStateUpdatingLayout;
  }
}

/* 'C' function for method : 'Views::Text.OnSetWrapText()' */
void ViewsText_OnSetWrapText( ViewsText _this, XBool value )
{
  if ( value == _this->WrapText )
    return;

  _this->WrapText = value;

  if ( _this->reparsed )
  {
    _this->flowString = 0;
    _this->reparsed = 0;
    EwPostSignal( EwNewSlot( _this, ViewsText_preReparseSlot ), ((XObject)_this ));
  }

  if ( value && ( _this->WrapWidth == 0 ))
    _this->Super2.viewState = _this->Super2.viewState & ~CoreViewStateFastReshape;
  else
    _this->Super2.viewState = _this->Super2.viewState | CoreViewStateFastReshape;
}

/* 'C' function for method : 'Views::Text.OnSetAlignment()' */
void ViewsText_OnSetAlignment( ViewsText _this, XSet value )
{
  if ( value == _this->Alignment )
    return;

  _this->Alignment = value;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  if ( _this->Ellipsis )
  {
    _this->flowString = 0;
    _this->reparsed = 0;
    EwPostSignal( EwNewSlot( _this, ViewsText_preReparseSlot ), ((XObject)_this ));
  }

  if ( _this->reparsed )
    EwPostSignal( EwNewSlot( _this, ViewsText_preOnUpdateSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::Text.OnSetString()' */
void ViewsText_OnSetString( ViewsText _this, XString value )
{
  if ( !EwCompString( value, _this->String ))
    return;

  _this->String = EwShareString( value );
  _this->flowString = 0;
  _this->reparsed = 0;
  EwPostSignal( EwNewSlot( _this, ViewsText_preReparseSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::Text.OnSetFont()' */
void ViewsText_OnSetFont( ViewsText _this, ResourcesFont value )
{
  if ( value == _this->Font )
    return;

  _this->Font = value;
  _this->flowString = 0;
  _this->reparsed = 0;
  EwPostSignal( EwNewSlot( _this, ViewsText_preReparseSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::Text.OnSetColor()' */
void ViewsText_OnSetColor( ViewsText _this, XColor value )
{
  if ((( !EwCompColor( value, _this->ColorTL ) && !EwCompColor( value, _this->ColorTR )) 
      && !EwCompColor( value, _this->ColorBL )) && !EwCompColor( value, _this->ColorBR ))
    return;

  _this->ColorTL = value;
  _this->ColorTR = value;
  _this->ColorBL = value;
  _this->ColorBR = value;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Text.OnSetOpacity()' */
void ViewsText_OnSetOpacity( ViewsText _this, XInt32 value )
{
  if ( value > 255 )
    value = 255;

  if ( value < 0 )
    value = 0;

  if ( value == _this->Opacity )
    return;

  _this->Opacity = value;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::Text.OnGetVisible()' */
XBool ViewsText_OnGetVisible( ViewsText _this )
{
  return (( _this->Super2.viewState & CoreViewStateVisible ) == CoreViewStateVisible );
}

/* 'C' function for method : 'Views::Text.OnSetVisible()' */
void ViewsText_OnSetVisible( ViewsText _this, XBool value )
{
  if ( value )
    CoreView__ChangeViewState( _this, CoreViewStateVisible, 0 );
  else
    CoreView__ChangeViewState( _this, 0, CoreViewStateVisible );
}

/* The method IsBaseDirectionRTL() returns 'true' if the text specified in @String 
   starts with an RTL (right-to-left) character. This implies the base direction 
   of the entire text paragraph. If the text starts with an LTR (left-to-right) 
   sign or the property @EnableBidiText is 'false', this method returns 'false'. */
XBool ViewsText_IsBaseDirectionRTL( ViewsText _this )
{
  XBool result;
  XHandle bidi;

  if ( !_this->reparsed )
    EwSignal( EwNewSlot( _this, ViewsText_reparseSlot ), ((XObject)_this ));

  if ( _this->bidiContext == 0 )
    return 0;

  result = 0;
  bidi = _this->bidiContext;
  result = EwBidiIsRTL( bidi );
  return result;
}

/* The method IsBidiText() returns 'true' if the text specified in the property 
   @String contains any right-to-left contents or any other Bidi algorithm specific 
   control codes requiring the Bidi processing of this text. Please note, if the 
   property @EnableBidiText is false, the text is not processed by the Bidi algorithm 
   and this method returns 'false'. */
XBool ViewsText_IsBidiText( ViewsText _this )
{
  XBool result;
  XHandle bidi;

  if ( !_this->reparsed )
    EwSignal( EwNewSlot( _this, ViewsText_reparseSlot ), ((XObject)_this ));

  if ( _this->bidiContext == 0 )
    return 0;

  result = 0;
  bidi = _this->bidiContext;
  result = EwBidiIsNeeded( bidi );
  return result;
}

/* The method GetContentArea() returns the position and the size of an area where 
   the view will show the text. This area is expressed in coordinates relative to 
   the top-left corner of the view's @Owner. The method takes in account all properties 
   which do affect the position and the alignment of the text, e.g. @Alignment, 
   @Orientation, @ScrollOffset, @WrapText, etc. */
XRect ViewsText_GetContentArea( ViewsText _this )
{
  XInt32 leading;
  XInt32 rh;
  XSet align;
  XEnum orient;
  XRect bounds;
  XInt32 padding;
  XInt32 width;
  XInt32 height;
  XRect rd;
  XRect rs;

  if ( !EwCompString( _this->String, 0 ) || ( _this->Font == 0 ))
    return _Const0003;

  if ( !_this->reparsed )
    EwSignal( EwNewSlot( _this, ViewsText_reparseSlot ), ((XObject)_this ));

  if ( !EwCompString( _this->flowString, 0 ))
    return _Const0003;

  leading = _this->Font->Leading;
  rh = ( _this->Font->Ascent + _this->Font->Descent ) + _this->Font->Leading;

  if ( _this->RowDistance > 0 )
  {
    leading = ( _this->RowDistance - _this->Font->Ascent ) - _this->Font->Descent;
    rh = _this->RowDistance;
  }

  if ( !EwCompPoint( _this->textSize, _Const0002 ))
    _this->textSize.X = ResourcesFont_GetFlowTextAdvance( _this->Font, _this->flowString );

  _this->textSize.Y = (( EwGetStringChar( _this->flowString, 0 ) * rh ) - leading );
  align = _this->Alignment;
  orient = _this->Orientation;
  bounds = _this->Super1.Bounds;
  padding = _this->Padding;
  width = EwGetRectW( bounds );
  height = EwGetRectH( bounds );

  if (( orient == ViewsOrientationRotated_90 ) || ( orient == ViewsOrientationRotated_270 ))
  {
    width = height;
    height = EwGetRectW( bounds );
  }

  rd = EwNewRect( padding, 0, width - padding, height );
  rs = EwNewRect2Point( rd.Point1, EwMovePointPos( rd.Point1, _this->textSize ));

  if ((( align & ViewsTextAlignmentAlignHorzAuto ) == ViewsTextAlignmentAlignHorzAuto ))
  {
    if ( ViewsText_IsBaseDirectionRTL( _this ))
      align = ( align & ~ViewsTextAlignmentAlignHorzAuto ) | ViewsTextAlignmentAlignHorzRight;
    else
      align = ( align & ~ViewsTextAlignmentAlignHorzAuto ) | ViewsTextAlignmentAlignHorzLeft;
  }

  if ((( align & ViewsTextAlignmentAlignHorzJustified ) == ViewsTextAlignmentAlignHorzJustified ))
  {
    XInt32 maxWidth = _this->WrapWidth;

    if ( maxWidth <= 0 )
      maxWidth = width - ( _this->Padding * 2 );

    if ( maxWidth < 0 )
      maxWidth = 0;

    if ( maxWidth > EwGetRectW( rs ))
      rs = EwSetRectW( rs, maxWidth );
  }

  if ( EwGetRectW( rs ) != EwGetRectW( rd ))
  {
    if ((( align & ViewsTextAlignmentAlignHorzRight ) == ViewsTextAlignmentAlignHorzRight ))
      rs = EwSetRectX( rs, rd.Point2.X - EwGetRectW( rs ));
    else
      if ((( align & ViewsTextAlignmentAlignHorzCenter ) == ViewsTextAlignmentAlignHorzCenter ))
        rs = EwSetRectX( rs, ( rd.Point1.X + ( EwGetRectW( rd ) / 2 )) - ( EwGetRectW( 
        rs ) / 2 ));
  }

  if ( EwGetRectH( rs ) != EwGetRectH( rd ))
  {
    if ((( align & ViewsTextAlignmentAlignVertBottom ) == ViewsTextAlignmentAlignVertBottom ))
      rs = EwSetRectY( rs, rd.Point2.Y - EwGetRectH( rs ));
    else
      if ((( align & ViewsTextAlignmentAlignVertCenter ) == ViewsTextAlignmentAlignVertCenter ))
        rs = EwSetRectY( rs, ( rd.Point1.Y + ( EwGetRectH( rd ) / 2 )) - ( EwGetRectH( 
        rs ) / 2 ));
  }

  if ( orient == ViewsOrientationNormal )
    rs = EwMoveRectPos( rs, bounds.Point1 );
  else
    if ( orient == ViewsOrientationRotated_90 )
    {
      XPoint org = EwNewPoint( bounds.Point1.X + rs.Point1.Y, bounds.Point2.Y - 
        rs.Point2.X );
      rs = EwNewRect2Point( org, EwMovePointPos( org, EwNewPoint( _this->textSize.Y, 
      _this->textSize.X )));
    }
    else
      if ( orient == ViewsOrientationRotated_180 )
      {
        XPoint org = EwNewPoint( bounds.Point2.X - rs.Point2.X, bounds.Point2.Y 
          - rs.Point2.Y );
        rs = EwNewRect2Point( org, EwMovePointPos( org, _this->textSize ));
      }
      else
        if ( orient == ViewsOrientationRotated_270 )
        {
          XPoint org = EwNewPoint( bounds.Point2.X - rs.Point2.Y, bounds.Point1.Y 
            + rs.Point1.X );
          rs = EwNewRect2Point( org, EwMovePointPos( org, EwNewPoint( _this->textSize.Y, 
          _this->textSize.X )));
        }

  return EwMoveRectPos( rs, _this->ScrollOffset );
}

/* Variants derived from the class : 'Views::Text' */
EW_DEFINE_CLASS_VARIANTS( ViewsText )
EW_END_OF_CLASS_VARIANTS( ViewsText )

/* Virtual Method Table (VMT) for the class : 'Views::Text' */
EW_DEFINE_CLASS( ViewsText, CoreRectView, Font, OnUpdate, flowString, flowString, 
                 flowString, bidiContext, "Views::Text" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  ViewsText_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreView_ChangeViewState,
  ViewsText_OnSetBounds,
EW_END_OF_CLASS( ViewsText )

/* Initializer for the class 'Views::WarpView' */
void ViewsWarpView__Init( ViewsWarpView _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreQuadView__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ViewsWarpView );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ViewsWarpView );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Super2.viewState = CoreViewStateAlphaBlended | CoreViewStateVisible;
  _this->Shininess = 5.000000f;
  _this->Quality = 1;
}

/* Re-Initializer for the class 'Views::WarpView' */
void ViewsWarpView__ReInit( ViewsWarpView _this )
{
  /* At first re-initialize the super class ... */
  CoreQuadView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Views::WarpView' */
void ViewsWarpView__Done( ViewsWarpView _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreQuadView );

  /* Don't forget to deinitialize the super class ... */
  CoreQuadView__Done( &_this->_Super );
}

/* 'C' function for method : 'Views::WarpView.OnSetPoint4()' */
void ViewsWarpView_OnSetPoint4( ViewsWarpView _this, XPoint value )
{
  if ( !EwCompPoint( value, _this->Super1.Point4 ))
    return;

  CoreQuadView_OnSetPoint4((CoreQuadView)_this, value );
  _this->vertices[ 3 ][ 0 ] = (XFloat)value.X;
  _this->vertices[ 3 ][ 1 ] = (XFloat)value.Y;
  _this->vertices[ 3 ][ 2 ] = 1.000000f;
  _this->newUpdateCase = 'E';
  _this->matrix = 0;
  EwPostSignal( EwNewSlot( _this, ViewsWarpView__update ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::WarpView.OnSetPoint3()' */
void ViewsWarpView_OnSetPoint3( ViewsWarpView _this, XPoint value )
{
  if ( !EwCompPoint( value, _this->Super1.Point3 ))
    return;

  CoreQuadView_OnSetPoint3((CoreQuadView)_this, value );
  _this->vertices[ 2 ][ 0 ] = (XFloat)value.X;
  _this->vertices[ 2 ][ 1 ] = (XFloat)value.Y;
  _this->vertices[ 2 ][ 2 ] = 1.000000f;
  _this->newUpdateCase = 'E';
  _this->matrix = 0;
  EwPostSignal( EwNewSlot( _this, ViewsWarpView__update ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::WarpView.OnSetPoint2()' */
void ViewsWarpView_OnSetPoint2( ViewsWarpView _this, XPoint value )
{
  if ( !EwCompPoint( value, _this->Super1.Point2 ))
    return;

  CoreQuadView_OnSetPoint2((CoreQuadView)_this, value );
  _this->vertices[ 1 ][ 0 ] = (XFloat)value.X;
  _this->vertices[ 1 ][ 1 ] = (XFloat)value.Y;
  _this->vertices[ 1 ][ 2 ] = 1.000000f;
  _this->newUpdateCase = 'E';
  _this->matrix = 0;
  EwPostSignal( EwNewSlot( _this, ViewsWarpView__update ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::WarpView.OnSetPoint1()' */
void ViewsWarpView_OnSetPoint1( ViewsWarpView _this, XPoint value )
{
  if ( !EwCompPoint( value, _this->Super1.Point1 ))
    return;

  CoreQuadView_OnSetPoint1((CoreQuadView)_this, value );
  _this->vertices[ 0 ][ 0 ] = (XFloat)value.X;
  _this->vertices[ 0 ][ 1 ] = (XFloat)value.Y;
  _this->vertices[ 0 ][ 2 ] = 1.000000f;
  _this->newUpdateCase = 'E';
  _this->matrix = 0;
  EwPostSignal( EwNewSlot( _this, ViewsWarpView__update ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::WarpView.calculateLight()' */
void ViewsWarpView_calculateLight( ViewsWarpView _this )
{
  XFloat lx;
  XFloat ly;
  XFloat lz;
  XFloat ax;
  XFloat ay;
  XFloat az;
  XFloat bx;
  XFloat by;
  XFloat bz;
  XFloat nx;
  XFloat ny;
  XFloat nz;
  XFloat nlen;
  XInt32 i;

  if ( _this->LightingEffect == 0.000000f )
    return;

  lx = 640.000000f;
  ly = 640.000000f;
  lz = -10000000.000000f;
  ax = _this->vertices[ 1 ][ 0 ] - _this->vertices[ 0 ][ 0 ];
  ay = _this->vertices[ 1 ][ 1 ] - _this->vertices[ 0 ][ 1 ];
  az = _this->vertices[ 1 ][ 2 ] - _this->vertices[ 0 ][ 2 ];
  bx = _this->vertices[ 3 ][ 0 ] - _this->vertices[ 0 ][ 0 ];
  by = _this->vertices[ 3 ][ 1 ] - _this->vertices[ 0 ][ 1 ];
  bz = _this->vertices[ 3 ][ 2 ] - _this->vertices[ 0 ][ 2 ];
  nx = ( ay * bz ) - ( az * by );
  ny = ( az * bx ) - ( ax * bz );
  nz = ( ax * by ) - ( ay * bx );
  nlen = EwMathSqrt((( nx * nx ) + ( ny * ny )) + ( nz * nz ));

  if ( nlen > 0.000000f )
  {
    nlen = 1.000000f / nlen;
    nx = nx * nlen;
    ny = ny * nlen;
    nz = nz * nlen;
  }

  for ( i = 0; i < 4; i = i + 1 )
  {
    XFloat vlx = _this->vertices[ EwCheckIndex( i, 4 )][ 0 ] - lx;
    XFloat vly = _this->vertices[ EwCheckIndex( i, 4 )][ 1 ] - ly;
    XFloat vlz = _this->vertices[ EwCheckIndex( i, 4 )][ 2 ] - lz;
    XFloat vllen = EwMathSqrt((( vlx * vlx ) + ( vly * vly )) + ( vlz * vlz ));
    XFloat light;

    if ( vllen > 0.000000f )
    {
      vllen = 1.000000f / vllen;
      vlx = vlx * vllen;
      vly = vly * vllen;
      vlz = vlz * vllen;
    }

    light = (( vlx * nx ) + ( vly * ny )) + ( vlz * nz );

    if ( light < 0.000000f )
      light = -light;

    light = EwMathPow( light, _this->Shininess );
    light = (( _this->LightingEffect * light ) + 1.000000f ) - _this->LightingEffect;
    _this->lighting[ EwCheckIndex( i, 4 )] = (XInt32)( light * 257.000000f );

    if ( _this->lighting[ EwCheckIndex( i, 4 )] > 256 )
      _this->lighting[ EwCheckIndex( i, 4 )] = 256;
  }
}

/* 'C' function for method : 'Views::WarpView.update()' */
void ViewsWarpView_update( ViewsWarpView _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->newUpdateCase == 0x0000 )
    return;

  if ( _this->newUpdateCase == 'E' )
  {
    GraphicsWarpMatrix m = EwNewObject( GraphicsWarpMatrix, 0 );
    m = GraphicsWarpMatrix_DeriveFromQuad( m, _this->vertices[ 0 ][ 0 ], _this->vertices[ 
    0 ][ 1 ], _this->vertices[ 1 ][ 0 ], _this->vertices[ 1 ][ 1 ], _this->vertices[ 
    2 ][ 0 ], _this->vertices[ 2 ][ 1 ], _this->vertices[ 3 ][ 0 ], _this->vertices[ 
    3 ][ 1 ]);

    if ( m != 0 )
    {
      GraphicsWarpMatrix_CalculateZ( m, 0.000000f, 0.000000f );
      _this->vertices[ 0 ][ 2 ] = m->Z * 240.000000f;
      GraphicsWarpMatrix_CalculateZ( m, 1.000000f, 0.000000f );
      _this->vertices[ 1 ][ 2 ] = m->Z * 240.000000f;
      GraphicsWarpMatrix_CalculateZ( m, 1.000000f, 1.000000f );
      _this->vertices[ 2 ][ 2 ] = m->Z * 240.000000f;
      GraphicsWarpMatrix_CalculateZ( m, 0.000000f, 1.000000f );
      _this->vertices[ 3 ][ 2 ] = m->Z * 240.000000f;
    }

    ViewsWarpView_calculateLight( _this );
  }

  if (((( _this->newUpdateCase == 'M' ) && ( _this->matrix != 0 )) && ( _this->bitmapSize.X 
      != 0 )) && ( _this->bitmapSize.Y != 0 ))
  {
    XRect sourceArea = EwIntersectRect2( _this->SourceArea, EwNewRect2Point( _Const0002, 
      _this->bitmapSize ));
    XPoint sourceAnchor = _this->SourceAnchor;
    XFloat l = (XFloat)( sourceArea.Point1.X - sourceAnchor.X );
    XFloat r = (XFloat)( sourceArea.Point2.X - sourceAnchor.X );
    XFloat t = (XFloat)( sourceArea.Point1.Y - sourceAnchor.Y );
    XFloat b = (XFloat)( sourceArea.Point2.Y - sourceAnchor.Y );
    GraphicsWarpMatrix_Project( _this->matrix, l, t );
    _this->vertices[ 0 ][ 0 ] = _this->matrix->X + _this->dstPosX;
    _this->vertices[ 0 ][ 1 ] = _this->matrix->Y + _this->dstPosY;
    _this->vertices[ 0 ][ 2 ] = _this->matrix->Z;
    GraphicsWarpMatrix_Project( _this->matrix, r, t );
    _this->vertices[ 1 ][ 0 ] = _this->matrix->X + _this->dstPosX;
    _this->vertices[ 1 ][ 1 ] = _this->matrix->Y + _this->dstPosY;
    _this->vertices[ 1 ][ 2 ] = _this->matrix->Z;
    GraphicsWarpMatrix_Project( _this->matrix, r, b );
    _this->vertices[ 2 ][ 0 ] = _this->matrix->X + _this->dstPosX;
    _this->vertices[ 2 ][ 1 ] = _this->matrix->Y + _this->dstPosY;
    _this->vertices[ 2 ][ 2 ] = _this->matrix->Z;
    GraphicsWarpMatrix_Project( _this->matrix, l, b );
    _this->vertices[ 3 ][ 0 ] = _this->matrix->X + _this->dstPosX;
    _this->vertices[ 3 ][ 1 ] = _this->matrix->Y + _this->dstPosY;
    _this->vertices[ 3 ][ 2 ] = _this->matrix->Z;

    if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
        == CoreViewStateVisible ))
      CoreGroup__InvalidateArea( _this->Super2.Owner, CoreView__GetExtent( _this ));

    _this->Super1.Point1 = EwNewPoint((XInt32)( _this->vertices[ 0 ][ 0 ] + 0.500000f ), 
    (XInt32)( _this->vertices[ 0 ][ 1 ] + 0.500000f ));
    _this->Super1.Point2 = EwNewPoint((XInt32)( _this->vertices[ 1 ][ 0 ] + 0.500000f ), 
    (XInt32)( _this->vertices[ 1 ][ 1 ] + 0.500000f ));
    _this->Super1.Point3 = EwNewPoint((XInt32)( _this->vertices[ 2 ][ 0 ] + 0.500000f ), 
    (XInt32)( _this->vertices[ 2 ][ 1 ] + 0.500000f ));
    _this->Super1.Point4 = EwNewPoint((XInt32)( _this->vertices[ 3 ][ 0 ] + 0.500000f ), 
    (XInt32)( _this->vertices[ 3 ][ 1 ] + 0.500000f ));
    ViewsWarpView_calculateLight( _this );
  }

  _this->oldUpdateCase = _this->newUpdateCase;
  _this->newUpdateCase = 0x0000;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, CoreView__GetExtent( _this ));
}

/* Wrapper function for the virtual method : 'Views::WarpView.update()' */
void ViewsWarpView__update( void* _this, XObject sender )
{
  ((ViewsWarpView)_this)->_VMT->update((ViewsWarpView)_this, sender );
}

/* 'C' function for method : 'Views::WarpView.OnSetSourceAnchor()' */
void ViewsWarpView_OnSetSourceAnchor( ViewsWarpView _this, XPoint value )
{
  if ( !EwCompPoint( value, _this->SourceAnchor ))
    return;

  _this->SourceAnchor = value;

  if (( _this->oldUpdateCase != 'E' ) && ( _this->newUpdateCase == 0x0000 ))
  {
    _this->newUpdateCase = _this->oldUpdateCase;
    EwPostSignal( EwNewSlot( _this, ViewsWarpView__update ), ((XObject)_this ));
  }
}

/* The method RotateAndScale() performs a 2D rotation and scaling of the source 
   image. The transformation is performed around a reference position specified 
   in the property @SourceAnchor. This allows e.g. a rotation around the center 
   of the image, etc. The rotation angle is passed in the parameter aAngle as a 
   value in the range 0 .. 360°. The values passed in the parameters aScaleX and 
   aScaleY determine additional scaling factors for the image in the horizontal 
   and vertical direction. If no scaling should be performed, the value 1.0 can 
   be passed to these parameters. In contrast, if no rotation is desired, 0.0 can 
   be passed to the parameter aAngle.
   The transformed image appears on the screen at the position aDstPos relative 
   to the top-left corner of this view's @Owner. The view itself takes the shape 
   resulting from this transformation and adapts its @Point1 .. @Point4 coordinates 
   accordingly. More sophisticated 2D and 3D transformations are performed by the 
   method @Warp3D(). */
void ViewsWarpView_RotateAndScale( ViewsWarpView _this, XPoint aDstPos, XFloat aAngle, 
  XFloat aScaleX, XFloat aScaleY )
{
  XRect sourceArea;
  XPoint sourceAnchor;
  XFloat l;
  XFloat r;
  XFloat t;
  XFloat b;
  XFloat cosA;
  XFloat sinA;
  XFloat l_cosA;
  XFloat l_sinA;
  XFloat r_cosA;
  XFloat r_sinA;
  XFloat t_cosA;
  XFloat t_sinA;
  XFloat b_cosA;
  XFloat b_sinA;
  XFloat dstX;
  XFloat dstY;

  if (( _this->bitmapSize.X == 0 ) || ( _this->bitmapSize.Y == 0 ))
    return;

  sourceArea = EwIntersectRect2( _this->SourceArea, EwNewRect2Point( _Const0002, 
  _this->bitmapSize ));
  sourceAnchor = _this->SourceAnchor;
  l = (XFloat)( sourceArea.Point1.X - sourceAnchor.X ) * aScaleX;
  r = (XFloat)( sourceArea.Point2.X - sourceAnchor.X ) * aScaleX;
  t = (XFloat)( sourceArea.Point1.Y - sourceAnchor.Y ) * aScaleY;
  b = (XFloat)( sourceArea.Point2.Y - sourceAnchor.Y ) * aScaleY;
  cosA = EwMathCos( aAngle );
  sinA = EwMathSin( aAngle );
  l_cosA = l * cosA;
  l_sinA = l * sinA;
  r_cosA = r * cosA;
  r_sinA = r * sinA;
  t_cosA = t * cosA;
  t_sinA = t * sinA;
  b_cosA = b * cosA;
  b_sinA = b * sinA;
  dstX = (XFloat)aDstPos.X;
  dstY = (XFloat)aDstPos.Y;
  _this->vertices[ 0 ][ 0 ] = ( dstX + l_cosA ) + t_sinA;
  _this->vertices[ 0 ][ 1 ] = ( dstY - l_sinA ) + t_cosA;
  _this->vertices[ 1 ][ 0 ] = ( dstX + r_cosA ) + t_sinA;
  _this->vertices[ 1 ][ 1 ] = ( dstY - r_sinA ) + t_cosA;
  _this->vertices[ 2 ][ 0 ] = ( dstX + r_cosA ) + b_sinA;
  _this->vertices[ 2 ][ 1 ] = ( dstY - r_sinA ) + b_cosA;
  _this->vertices[ 3 ][ 0 ] = ( dstX + l_cosA ) + b_sinA;
  _this->vertices[ 3 ][ 1 ] = ( dstY - l_sinA ) + b_cosA;
  _this->vertices[ 0 ][ 2 ] = 1.000000f;
  _this->vertices[ 1 ][ 2 ] = 1.000000f;
  _this->vertices[ 2 ][ 2 ] = 1.000000f;
  _this->vertices[ 3 ][ 2 ] = 1.000000f;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, CoreView__GetExtent( _this ));

  _this->Super1.Point1 = EwNewPoint((XInt32)( _this->vertices[ 0 ][ 0 ] + 0.500000f ), 
  (XInt32)( _this->vertices[ 0 ][ 1 ] + 0.500000f ));
  _this->Super1.Point2 = EwNewPoint((XInt32)( _this->vertices[ 1 ][ 0 ] + 0.500000f ), 
  (XInt32)( _this->vertices[ 1 ][ 1 ] + 0.500000f ));
  _this->Super1.Point3 = EwNewPoint((XInt32)( _this->vertices[ 2 ][ 0 ] + 0.500000f ), 
  (XInt32)( _this->vertices[ 2 ][ 1 ] + 0.500000f ));
  _this->Super1.Point4 = EwNewPoint((XInt32)( _this->vertices[ 3 ][ 0 ] + 0.500000f ), 
  (XInt32)( _this->vertices[ 3 ][ 1 ] + 0.500000f ));
  _this->oldUpdateCase = 'E';
  _this->newUpdateCase = 0x0000;
  _this->matrix = 0;

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, CoreView__GetExtent( _this ));

  ViewsWarpView_calculateLight( _this );
}

/* Wrapper function for the virtual method : 'Views::WarpView.RotateAndScale()' */
void ViewsWarpView__RotateAndScale( void* _this, XPoint aDstPos, XFloat aAngle, 
  XFloat aScaleX, XFloat aScaleY )
{
  ((ViewsWarpView)_this)->_VMT->RotateAndScale((ViewsWarpView)_this, aDstPos, aAngle
  , aScaleX, aScaleY );
}

/* Variants derived from the class : 'Views::WarpView' */
EW_DEFINE_CLASS_VARIANTS( ViewsWarpView )
EW_END_OF_CLASS_VARIANTS( ViewsWarpView )

/* Virtual Method Table (VMT) for the class : 'Views::WarpView' */
EW_DEFINE_CLASS( ViewsWarpView, CoreQuadView, matrix, dstPosY, dstPosY, dstPosY, 
                 dstPosY, dstPosY, "Views::WarpView" )
  CoreQuadView_initLayoutContext,
  CoreView_GetRoot,
  CoreView_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreQuadView_ArrangeView,
  CoreQuadView_MoveView,
  CoreQuadView_GetExtent,
  CoreView_ChangeViewState,
  ViewsWarpView_OnSetPoint4,
  ViewsWarpView_OnSetPoint3,
  ViewsWarpView_OnSetPoint2,
  ViewsWarpView_OnSetPoint1,
  ViewsWarpView_update,
  ViewsWarpView_RotateAndScale,
EW_END_OF_CLASS( ViewsWarpView )

/* Initializer for the class 'Views::WarpImage' */
void ViewsWarpImage__Init( ViewsWarpImage _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  ViewsWarpView__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ViewsWarpImage );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ViewsWarpImage );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Color4 = _Const0000;
  _this->Color3 = _Const0000;
  _this->Color2 = _Const0000;
  _this->Color1 = _Const0000;
  _this->Endless = 1;
  _this->Opacity = 255;
}

/* Re-Initializer for the class 'Views::WarpImage' */
void ViewsWarpImage__ReInit( ViewsWarpImage _this )
{
  /* At first re-initialize the super class ... */
  ViewsWarpView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Views::WarpImage' */
void ViewsWarpImage__Done( ViewsWarpImage _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( ViewsWarpView );

  /* Don't forget to deinitialize the super class ... */
  ViewsWarpView__Done( &_this->_Super );
}

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void ViewsWarpImage_Draw( ViewsWarpImage _this, GraphicsCanvas aCanvas, XRect aClip, 
  XPoint aOffset, XInt32 aOpacity, XBool aBlend )
{
  XInt32 frameNr = _this->FrameNumber;
  XColor c1;
  XColor c2;
  XColor c3;
  XColor c4;
  XInt32 opacity;
  XFloat ofsX;
  XFloat ofsY;
  XRect area;

  if ( _this->animFrameNumber >= 0 )
    frameNr = _this->animFrameNumber;

  if (( _this->Bitmap == 0 ) || ( frameNr >= _this->Bitmap->NoOfFrames ))
    return;

  ResourcesBitmap__Update( _this->Bitmap );
  c1 = _this->Color1;
  c2 = _this->Color2;
  c3 = _this->Color3;
  c4 = _this->Color4;
  opacity = ((( aOpacity + 1 ) * _this->Opacity ) >> 8 ) + 1;
  ofsX = (XFloat)aOffset.X;
  ofsY = (XFloat)aOffset.Y;
  area = EwIntersectRect2( _this->Super1.SourceArea, EwNewRect2Point( _Const0002, 
  _this->Super1.bitmapSize ));
  aBlend = (XBool)( aBlend && (( _this->Super3.viewState & CoreViewStateAlphaBlended ) 
  == CoreViewStateAlphaBlended ));

  if ( opacity < 256 )
  {
    c1.Alpha = (XUInt8)(( c1.Alpha * opacity ) >> 8 );
    c2.Alpha = (XUInt8)(( c2.Alpha * opacity ) >> 8 );
    c3.Alpha = (XUInt8)(( c3.Alpha * opacity ) >> 8 );
    c4.Alpha = (XUInt8)(( c4.Alpha * opacity ) >> 8 );
  }

  if ( _this->Super1.LightingEffect > 0.000000f )
  {
    if ( _this->Super1.lighting[ 0 ] < 256 )
      c1.Alpha = (XUInt8)(( c1.Alpha * _this->Super1.lighting[ 0 ]) >> 8 );

    if ( _this->Super1.lighting[ 1 ] < 256 )
      c2.Alpha = (XUInt8)(( c2.Alpha * _this->Super1.lighting[ 1 ]) >> 8 );

    if ( _this->Super1.lighting[ 2 ] < 256 )
      c3.Alpha = (XUInt8)(( c3.Alpha * _this->Super1.lighting[ 2 ]) >> 8 );

    if ( _this->Super1.lighting[ 3 ] < 256 )
      c4.Alpha = (XUInt8)(( c4.Alpha * _this->Super1.lighting[ 3 ]) >> 8 );
  }

  aClip = EwIntersectRect2( aClip, EwMoveRectPos( _this->Super1.Clipping, aOffset ));
  GraphicsCanvas_WarpBitmap( aCanvas, aClip, _this->Bitmap, frameNr, _this->Super1.vertices[ 
  0 ][ 0 ] + ofsX, _this->Super1.vertices[ 0 ][ 1 ] + ofsY, _this->Super1.vertices[ 
  0 ][ 2 ], _this->Super1.vertices[ 1 ][ 0 ] + ofsX, _this->Super1.vertices[ 1 ][ 
  1 ] + ofsY, _this->Super1.vertices[ 1 ][ 2 ], _this->Super1.vertices[ 2 ][ 0 ] 
  + ofsX, _this->Super1.vertices[ 2 ][ 1 ] + ofsY, _this->Super1.vertices[ 2 ][ 
  2 ], _this->Super1.vertices[ 3 ][ 0 ] + ofsX, _this->Super1.vertices[ 3 ][ 1 ] 
  + ofsY, _this->Super1.vertices[ 3 ][ 2 ], area, c1, c2, c3, c4, aBlend, _this->Super1.Quality );
}

/* 'C' function for method : 'Views::WarpImage.observerSlot()' */
void ViewsWarpImage_observerSlot( ViewsWarpImage _this, XObject sender )
{
  XPoint oldBitmapSize;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldBitmapSize = _this->Super1.bitmapSize;

  if ( _this->Bitmap != 0 )
    _this->Super1.bitmapSize = _this->Bitmap->FrameSize;
  else
    _this->Super1.bitmapSize = _Const0002;

  if ((( !EwCompPoint( oldBitmapSize, _this->Super1.bitmapSize ) || (( _this->Super1.oldUpdateCase 
      == 'E' ) && ( _this->Super1.newUpdateCase == 0x0000 ))) && ( _this->Super3.Owner 
      != 0 )) && (( _this->Super3.viewState & CoreViewStateVisible ) == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super3.Owner, CoreView__GetExtent( _this ));

  if ((( _this->Super1.oldUpdateCase != 'E' ) && ( _this->Super1.newUpdateCase == 
      0x0000 )) && EwCompPoint( oldBitmapSize, _this->Super1.bitmapSize ))
  {
    _this->Super1.newUpdateCase = _this->Super1.oldUpdateCase;
    EwPostSignal( EwNewSlot( _this, ViewsWarpView__update ), ((XObject)_this ));
  }
}

/* 'C' function for method : 'Views::WarpImage.timerSlot()' */
void ViewsWarpImage_timerSlot( ViewsWarpImage _this, XObject sender )
{
  XInt32 frameNr;
  XInt32 period;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  frameNr = _this->animFrameNumber;
  period = 0;

  if ( _this->Bitmap != 0 )
    period = _this->Bitmap->NoOfFrames * _this->Bitmap->FrameDelay;

  if ((( _this->timer != 0 ) && ( _this->animFrameNumber < 0 )) && ( period > 0 ))
    _this->startTime = _this->timer->Time - ( _this->FrameNumber * _this->Bitmap->FrameDelay );

  if (( _this->timer != 0 ) && ( period > 0 ))
  {
    XInt32 time = (XInt32)( _this->timer->Time - _this->startTime );
    frameNr = time / _this->Bitmap->FrameDelay;

    if ( time >= period )
    {
      frameNr = frameNr % _this->Bitmap->NoOfFrames;
      _this->startTime = _this->timer->Time - ( time % period );

      if ( !_this->Endless )
      {
        frameNr = _this->Bitmap->NoOfFrames - 1;
        period = 0;
      }
    }
  }

  if ((( frameNr != _this->animFrameNumber ) && ( _this->Super3.Owner != 0 )) && 
      (( _this->Super3.viewState & CoreViewStateVisible ) == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super3.Owner, CoreView__GetExtent( _this ));

  _this->animFrameNumber = frameNr;

  if (( period == 0 ) && ( _this->timer != 0 ))
  {
    EwDetachObjObserver( EwNewSlot( _this, ViewsWarpImage_timerSlot ), (XObject)_this->timer, 
      0 );
    _this->timer = 0;
    EwSignal( _this->OnFinished, ((XObject)_this ));
  }
}

/* 'C' function for method : 'Views::WarpImage.OnSetAnimated()' */
void ViewsWarpImage_OnSetAnimated( ViewsWarpImage _this, XBool value )
{
  if ( _this->Animated == value )
    return;

  _this->Animated = value;
  _this->animFrameNumber = -1;

  if ( !value && ( _this->timer != 0 ))
  {
    EwDetachObjObserver( EwNewSlot( _this, ViewsWarpImage_timerSlot ), (XObject)_this->timer, 
      0 );
    _this->timer = 0;
  }

  if ( value )
  {
    _this->timer = ((CoreTimer)EwGetAutoObject( &EffectsEffectTimer, EffectsEffectTimerClass ));
    EwAttachObjObserver( EwNewSlot( _this, ViewsWarpImage_timerSlot ), (XObject)_this->timer, 
      0 );
    EwPostSignal( EwNewSlot( _this, ViewsWarpImage_timerSlot ), ((XObject)_this ));
  }

  if (( _this->Super3.Owner != 0 ) && (( _this->Super3.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super3.Owner, CoreView__GetExtent( _this ));
}

/* 'C' function for method : 'Views::WarpImage.OnSetColor()' */
void ViewsWarpImage_OnSetColor( ViewsWarpImage _this, XColor value )
{
  if ((( !EwCompColor( value, _this->Color1 ) && !EwCompColor( value, _this->Color2 )) 
      && !EwCompColor( value, _this->Color3 )) && !EwCompColor( value, _this->Color4 ))
    return;

  _this->Color1 = value;
  _this->Color2 = value;
  _this->Color3 = value;
  _this->Color4 = value;

  if (( _this->Super3.Owner != 0 ) && (( _this->Super3.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super3.Owner, CoreView__GetExtent( _this ));
}

/* 'C' function for method : 'Views::WarpImage.OnSetBitmap()' */
void ViewsWarpImage_OnSetBitmap( ViewsWarpImage _this, ResourcesBitmap value )
{
  if ( value == _this->Bitmap )
    return;

  if (( _this->Bitmap != 0 ) && _this->Bitmap->Mutable )
    EwDetachObjObserver( EwNewSlot( _this, ViewsWarpImage_observerSlot ), (XObject)_this->Bitmap, 
      0 );

  _this->Bitmap = value;
  _this->animFrameNumber = -1;

  if (( value != 0 ) && value->Mutable )
    EwAttachObjObserver( EwNewSlot( _this, ViewsWarpImage_observerSlot ), (XObject)value, 
      0 );

  if ( _this->Animated )
  {
    ViewsWarpImage_OnSetAnimated( _this, 0 );
    ViewsWarpImage_OnSetAnimated( _this, 1 );
  }

  EwSignal( EwNewSlot( _this, ViewsWarpImage_observerSlot ), ((XObject)_this ));
}

/* Variants derived from the class : 'Views::WarpImage' */
EW_DEFINE_CLASS_VARIANTS( ViewsWarpImage )
EW_END_OF_CLASS_VARIANTS( ViewsWarpImage )

/* Virtual Method Table (VMT) for the class : 'Views::WarpImage' */
EW_DEFINE_CLASS( ViewsWarpImage, ViewsWarpView, timer, OnFinished, startTime, startTime, 
                 startTime, startTime, "Views::WarpImage" )
  CoreQuadView_initLayoutContext,
  CoreView_GetRoot,
  ViewsWarpImage_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreQuadView_ArrangeView,
  CoreQuadView_MoveView,
  CoreQuadView_GetExtent,
  CoreView_ChangeViewState,
  ViewsWarpView_OnSetPoint4,
  ViewsWarpView_OnSetPoint3,
  ViewsWarpView_OnSetPoint2,
  ViewsWarpView_OnSetPoint1,
  ViewsWarpView_update,
  ViewsWarpView_RotateAndScale,
EW_END_OF_CLASS( ViewsWarpImage )

/* Initializer for the class 'Views::WarpGroup' */
void ViewsWarpGroup__Init( ViewsWarpGroup _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  ViewsWarpView__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ViewsWarpGroup );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ViewsWarpGroup );

  /* ... and initialize objects, variables, properties, etc. */
  _this->i11 = 1.000000f;
  _this->i22 = 1.000000f;
  _this->i33 = 1.000000f;
  _this->Opacity4 = 255;
  _this->Opacity3 = 255;
  _this->Opacity2 = 255;
  _this->Opacity1 = 255;
}

/* Re-Initializer for the class 'Views::WarpGroup' */
void ViewsWarpGroup__ReInit( ViewsWarpGroup _this )
{
  /* At first re-initialize the super class ... */
  ViewsWarpView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Views::WarpGroup' */
void ViewsWarpGroup__Done( ViewsWarpGroup _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( ViewsWarpView );

  /* Don't forget to deinitialize the super class ... */
  ViewsWarpView__Done( &_this->_Super );
}

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void ViewsWarpGroup_Draw( ViewsWarpGroup _this, GraphicsCanvas aCanvas, XRect aClip, 
  XPoint aOffset, XInt32 aOpacity, XBool aBlend )
{
  XFloat ofsX;
  XFloat ofsY;
  XRect area;
  XColor c1;
  XColor c2;
  XColor c3;
  XColor c4;

  if ( _this->bitmap == 0 )
    return;

  ResourcesBitmap__Update( _this->bitmap );
  ofsX = (XFloat)aOffset.X;
  ofsY = (XFloat)aOffset.Y;
  area = EwIntersectRect2( _this->Super1.SourceArea, EwNewRect2Point( _Const0002, 
  _this->Super1.bitmapSize ));
  aBlend = (XBool)( aBlend && (( _this->Super3.viewState & CoreViewStateAlphaBlended ) 
  == CoreViewStateAlphaBlended ));
  aOpacity = aOpacity + 1;
  c1 = EwNewColor( 255, 255, 255, (XUInt8)(( aOpacity * _this->Opacity1 ) >> 8 ));
  c2 = EwNewColor( 255, 255, 255, (XUInt8)(( aOpacity * _this->Opacity2 ) >> 8 ));
  c3 = EwNewColor( 255, 255, 255, (XUInt8)(( aOpacity * _this->Opacity3 ) >> 8 ));
  c4 = EwNewColor( 255, 255, 255, (XUInt8)(( aOpacity * _this->Opacity4 ) >> 8 ));

  if ( _this->Super1.LightingEffect > 0.000000f )
  {
    if ( _this->Super1.lighting[ 0 ] < 256 )
      c1.Alpha = (XUInt8)(( c1.Alpha * _this->Super1.lighting[ 0 ]) >> 8 );

    if ( _this->Super1.lighting[ 1 ] < 256 )
      c2.Alpha = (XUInt8)(( c2.Alpha * _this->Super1.lighting[ 1 ]) >> 8 );

    if ( _this->Super1.lighting[ 2 ] < 256 )
      c3.Alpha = (XUInt8)(( c3.Alpha * _this->Super1.lighting[ 2 ]) >> 8 );

    if ( _this->Super1.lighting[ 3 ] < 256 )
      c4.Alpha = (XUInt8)(( c4.Alpha * _this->Super1.lighting[ 3 ]) >> 8 );
  }

  aClip = EwIntersectRect2( aClip, EwMoveRectPos( _this->Super1.Clipping, aOffset ));
  GraphicsCanvas_WarpBitmap( aCanvas, aClip, _this->bitmap, 0, _this->Super1.vertices[ 
  0 ][ 0 ] + ofsX, _this->Super1.vertices[ 0 ][ 1 ] + ofsY, _this->Super1.vertices[ 
  0 ][ 2 ], _this->Super1.vertices[ 1 ][ 0 ] + ofsX, _this->Super1.vertices[ 1 ][ 
  1 ] + ofsY, _this->Super1.vertices[ 1 ][ 2 ], _this->Super1.vertices[ 2 ][ 0 ] 
  + ofsX, _this->Super1.vertices[ 2 ][ 1 ] + ofsY, _this->Super1.vertices[ 2 ][ 
  2 ], _this->Super1.vertices[ 3 ][ 0 ] + ofsX, _this->Super1.vertices[ 3 ][ 1 ] 
  + ofsY, _this->Super1.vertices[ 3 ][ 2 ], area, c1, c2, c3, c4, aBlend, _this->Super1.Quality );
}

/* The method HandleEvent() is invoked automatically if the view has received an 
   event. For example, touching the view on the touch screen can cause the view 
   to receive a Core::CursorEvent event. Within this method the view can evaluate 
   the event and react to it.
   Whether the event has been handled by the view or not is determined by the return 
   value. To sign an event as handled HandleEvent() should return a valid object 
   (e.g. 'this'). If the event has not been handled, 'null' should be returned.
   Depending on the kind of the event, the framework can continue dispatching of 
   still unhandled events. For example, keyboard events (Core::KeyEvent class) are 
   automatically delivered to the superior @Owner of the receiver view if this view 
   has ignored the event.
   HandleEvent() is invoked automatically by the framework, so you never should 
   need to invoke it directly. However you can prepare and post new events by using 
   the methods DispatchEvent() and BroadcastEvent() of the application class Core::Root. */
XObject ViewsWarpGroup_HandleEvent( ViewsWarpGroup _this, CoreEvent aEvent )
{
  CoreCursorEvent event1;
  CoreDragEvent event2;
  XPoint offset;
  CoreGroup grp;
  CoreView view;

  if (( _this->Group == 0 ) || ( _this->cursorTargetView == 0 ))
    return 0;

  event1 = EwCastObject( aEvent, CoreCursorEvent );
  event2 = EwCastObject( aEvent, CoreDragEvent );

  if (( event1 == 0 ) && ( event2 == 0 ))
    return 0;

  offset = _Const0002;
  grp = _this->cursorTargetView->Owner;

  while (( grp != 0 ) && ( grp != _this->Group ))
  {
    offset = EwMovePointNeg( offset, grp->Super1.Bounds.Point1 );
    grp = grp->Super2.Owner;
  }

  view = _this->cursorTargetView;

  if (( event1 != 0 ) && !event1->Down )
    _this->cursorTargetView = 0;

  if ( event1 != 0 )
  {
    event1->CurrentPos = EwMovePointPos( ViewsWarpGroup_mapPosition( _this, event1->CurrentPos ), 
    offset );
    event1->HittingPos = EwMovePointPos( ViewsWarpGroup_mapPosition( _this, event1->HittingPos ), 
    offset );
    return CoreView__HandleEvent( view, ((CoreEvent)event1 ));
  }

  if ( event2 != 0 )
  {
    XPoint oldPos = EwMovePointPos( ViewsWarpGroup_mapPosition( _this, EwMovePointNeg( 
      event2->CurrentPos, event2->Offset )), offset );
    event2->CurrentPos = EwMovePointPos( ViewsWarpGroup_mapPosition( _this, event2->CurrentPos ), 
    offset );
    event2->HittingPos = EwMovePointPos( ViewsWarpGroup_mapPosition( _this, event2->HittingPos ), 
    offset );
    event2->Offset = EwMovePointNeg( event2->CurrentPos, oldPos );
    return CoreView__HandleEvent( view, ((CoreEvent)event2 ));
  }

  return CoreView_HandleEvent((CoreView)_this, aEvent );
}

/* The method CursorHitTest() is invoked automatically in order to determine whether 
   the view is interested in the receipt of cursor events or not. This method will 
   be invoked immediately after the user has tapped the visible area of the view. 
   If the view is not interested in the cursor event, the framework repeats this 
   procedure for the next behind view until a willing view has been found or all 
   views are evaluated.
   In the implementation of the method the view can evaluate the passed aArea parameter. 
   It determines the place where the user has tapped the view with his fingertip 
   expressed in the coordinates of the views @Owner. The method can test e.g. whether 
   the tapped area does intersect any touchable areas within the view, etc. The 
   affected finger is identified in the parameter aFinger. The first finger (or 
   the first mouse device button) has the number 0.
   The parameter aStrikeCount determines how many times the same area has been tapped 
   in series. This is useful to detect series of multiple touches, e.g. in case 
   of the double click, aStrikeCount == 2.
   The parameter aDedicatedView, if it is not 'null', restricts the event to be 
   handled by this view only. If aDedicatedView == null, no special restriction 
   exists.
   This method is also invoked if during an existing grab cycle the current target 
   view has decided to resign and deflect the cursor events to another view. This 
   is usually the case after the user has performed a gesture the current target 
   view is not interested to process. Thereupon, the system looks for another view 
   willing to take over the cursor event processing after the performed gesture. 
   Which gesture has caused the operation, is specified in the parameter aRetargetReason.
   If the view is willing to process the event, the method should create, initialize 
   and return a new Core::CursorHit object. This object identify the willing view. 
   Otherwise the method should return 'null'.
   CursorHitTest() is invoked automatically by the framework, so you never should 
   need to invoke it directly. This method is predetermined for the hit-test only. 
   The proper processing of events should take place in the @HandleEvent() method 
   by reacting to Core::CursorEvent and Core::DragEvent events. */
CoreCursorHit ViewsWarpGroup_CursorHitTest( ViewsWarpGroup _this, XRect aArea, XInt32 
  aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
{
  XPoint pos;
  XRect srcArea;
  CoreCursorHit hit;

  if (( _this->Group == 0 ) || !(( _this->Group->Super2.viewState & ( CoreViewStateEnabled 
      | CoreViewStateTouchable )) == ( CoreViewStateEnabled | CoreViewStateTouchable )))
    return 0;

  pos = EwGetRectCenter( aArea );

  if ( !EwIsRectEmpty( _this->Super1.Clipping ) && !EwIsPointInRect( _this->Super1.Clipping, 
      pos ))
    return 0;

  pos = ViewsWarpGroup_mapPosition( _this, pos );
  srcArea = EwIntersectRect2( _this->Super1.SourceArea, EwNewRect2Point( _Const0002, 
  _this->Super1.bitmapSize ));

  if ( !EwIsPointInRect( srcArea, pos ))
    return 0;

  hit = CoreView__CursorHitTest( _this->Group, EwMoveRectPos( EwMoveRectPos( EwGetRectORect( 
  aArea ), pos ), _this->Group->Super1.Bounds.Point1 ), aFinger, aStrikeCount, aDedicatedView, 
  aRetargetReason );

  if ( hit != 0 )
    _this->cursorTargetView = hit->View;
  else
    _this->cursorTargetView = 0;

  if ( _this->cursorTargetView != 0 )
    return CoreCursorHit_Initialize( EwNewObject( CoreCursorHit, 0 ), ((CoreView)_this ), 
      _Const0002 );

  return 0;
}

/* 'C' function for method : 'Views::WarpGroup.update()' */
void ViewsWarpGroup_update( ViewsWarpGroup _this, XObject sender )
{
  ViewsWarpView_update((ViewsWarpView)_this, sender );
  _this->invMatrixValid = 0;
}

/* The method RotateAndScale() performs a 2D rotation and scaling of the source 
   image. The transformation is performed around a reference position specified 
   in the property @SourceAnchor. This allows e.g. a rotation around the center 
   of the image, etc. The rotation angle is passed in the parameter aAngle as a 
   value in the range 0 .. 360°. The values passed in the parameters aScaleX and 
   aScaleY determine additional scaling factors for the image in the horizontal 
   and vertical direction. If no scaling should be performed, the value 1.0 can 
   be passed to these parameters. In contrast, if no rotation is desired, 0.0 can 
   be passed to the parameter aAngle.
   The transformed image appears on the screen at the position aDstPos relative 
   to the top-left corner of this view's @Owner. The view itself takes the shape 
   resulting from this transformation and adapts its @Point1 .. @Point4 coordinates 
   accordingly. More sophisticated 2D and 3D transformations are performed by the 
   method @Warp3D(). */
void ViewsWarpGroup_RotateAndScale( ViewsWarpGroup _this, XPoint aDstPos, XFloat 
  aAngle, XFloat aScaleX, XFloat aScaleY )
{
  ViewsWarpView_RotateAndScale((ViewsWarpView)_this, aDstPos, aAngle, aScaleX, aScaleY );
  _this->invMatrixValid = 0;
}

/* 'C' function for method : 'Views::WarpGroup.updateInvMatrix()' */
void ViewsWarpGroup_updateInvMatrix( ViewsWarpGroup _this )
{
  XFloat x1 = _this->Super1.vertices[ 0 ][ 0 ];
  XFloat y1 = _this->Super1.vertices[ 0 ][ 1 ];
  XFloat x2 = _this->Super1.vertices[ 1 ][ 0 ];
  XFloat y2 = _this->Super1.vertices[ 1 ][ 1 ];
  XFloat x3 = _this->Super1.vertices[ 2 ][ 0 ];
  XFloat y3 = _this->Super1.vertices[ 2 ][ 1 ];
  XFloat x4 = _this->Super1.vertices[ 3 ][ 0 ];
  XFloat y4 = _this->Super1.vertices[ 3 ][ 1 ];
  XFloat deltaX1 = x2 - x3;
  XFloat deltaY1 = y2 - y3;
  XFloat deltaX2 = x4 - x3;
  XFloat deltaY2 = y4 - y3;
  XFloat sumX = (( x1 - x2 ) + x3 ) - x4;
  XFloat sumY = (( y1 - y2 ) + y3 ) - y4;
  XFloat det = ( deltaX1 * deltaY2 ) - ( deltaY1 * deltaX2 );
  XFloat g;
  XFloat h;
  XFloat i;
  XFloat a;
  XFloat b;
  XFloat c;
  XFloat d;
  XFloat e;
  XFloat f;

  if ( det == 0.000000f )
    return;

  g = (( sumX * deltaY2 ) - ( sumY * deltaX2 )) / det;
  h = (( deltaX1 * sumY ) - ( deltaY1 * sumX )) / det;
  i = 1.000000f;
  a = ( x2 - x1 ) + ( g * x2 );
  b = ( x4 - x1 ) + ( h * x4 );
  c = x1;
  d = ( y2 - y1 ) + ( g * y2 );
  e = ( y4 - y1 ) + ( h * y4 );
  f = y1;
  _this->i11 = ( e * i ) - ( f * h );
  _this->i12 = ( c * h ) - ( b * i );
  _this->i13 = ( b * f ) - ( c * e );
  _this->i21 = ( f * g ) - ( d * i );
  _this->i22 = ( a * i ) - ( c * g );
  _this->i23 = ( c * d ) - ( a * f );
  _this->i31 = ( d * h ) - ( e * g );
  _this->i32 = ( b * g ) - ( a * h );
  _this->i33 = ( a * e ) - ( b * d );
  _this->invMatrixValid = 1;
}

/* 'C' function for method : 'Views::WarpGroup.mapPosition()' */
XPoint ViewsWarpGroup_mapPosition( ViewsWarpGroup _this, XPoint aPos )
{
  XFloat xd;
  XFloat yd;
  XFloat xs;
  XFloat ys;
  XFloat qs;
  XRect srcArea;

  if ( !_this->invMatrixValid )
    ViewsWarpGroup_updateInvMatrix( _this );

  xd = (XFloat)aPos.X;
  yd = (XFloat)aPos.Y;
  xs = (( xd * _this->i11 ) + ( yd * _this->i12 )) + _this->i13;
  ys = (( xd * _this->i21 ) + ( yd * _this->i22 )) + _this->i23;
  qs = (( xd * _this->i31 ) + ( yd * _this->i32 )) + _this->i33;

  if ( qs != 0.000000f )
  {
    xs = xs / qs;
    ys = ys / qs;
  }

  srcArea = EwIntersectRect2( _this->Super1.SourceArea, EwNewRect2Point( _Const0002, 
  _this->Super1.bitmapSize ));
  xs = xs * (XFloat)EwGetRectW( srcArea );
  ys = ys * (XFloat)EwGetRectH( srcArea );
  return EwMovePointPos( EwNewPoint((XInt32)xs, (XInt32)ys ), srcArea.Point1 );
}

/* 'C' function for method : 'Views::WarpGroup.observerSlot()' */
void ViewsWarpGroup_observerSlot( ViewsWarpGroup _this, XObject sender )
{
  XPoint oldBitmapSize;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldBitmapSize = _this->Super1.bitmapSize;

  if ( _this->Group != 0 )
    _this->bitmap = ((ResourcesBitmap)_this->Group->buffer );
  else
    _this->bitmap = 0;

  if ( _this->bitmap != 0 )
    _this->Super1.bitmapSize = _this->bitmap->FrameSize;
  else
    _this->Super1.bitmapSize = _Const0002;

  if ((( !EwCompPoint( oldBitmapSize, _this->Super1.bitmapSize ) || (( _this->Super1.oldUpdateCase 
      == 'E' ) && ( _this->Super1.newUpdateCase == 0x0000 ))) && ( _this->Super3.Owner 
      != 0 )) && (( _this->Super3.viewState & CoreViewStateVisible ) == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super3.Owner, CoreView__GetExtent( _this ));

  if ((( _this->Super1.oldUpdateCase != 'E' ) && ( _this->Super1.newUpdateCase == 
      0x0000 )) && EwCompPoint( oldBitmapSize, _this->Super1.bitmapSize ))
  {
    _this->Super1.newUpdateCase = _this->Super1.oldUpdateCase;
    EwPostSignal( EwNewSlot( _this, ViewsWarpView__update ), ((XObject)_this ));
  }
}

/* 'C' function for method : 'Views::WarpGroup.OnSetOpacity()' */
void ViewsWarpGroup_OnSetOpacity( ViewsWarpGroup _this, XInt32 value )
{
  if (((( value == _this->Opacity1 ) && ( value == _this->Opacity2 )) && ( value 
      == _this->Opacity3 )) && ( value == _this->Opacity4 ))
    return;

  _this->Opacity1 = value;
  _this->Opacity2 = value;
  _this->Opacity3 = value;
  _this->Opacity4 = value;

  if (( _this->Super3.Owner != 0 ) && (( _this->Super3.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super3.Owner, CoreView__GetExtent( _this ));
}

/* 'C' function for method : 'Views::WarpGroup.OnSetGroup()' */
void ViewsWarpGroup_OnSetGroup( ViewsWarpGroup _this, CoreGroup value )
{
  if ( value == _this->Group )
    return;

  if (( value != 0 ) && ( value == _this->Super3.Owner ))
  {
    EwThrow( EwLoadString( &_Const0005 ));
    return;
  }

  if ( _this->Group != 0 )
  {
    EwDetachObjObserver( EwNewSlot( _this, ViewsWarpGroup_observerSlot ), (XObject)_this->Group, 
      0 );
    _this->bitmap = 0;
  }

  _this->Group = value;

  if ( value != 0 )
  {
    EwAttachObjObserver( EwNewSlot( _this, ViewsWarpGroup_observerSlot ), (XObject)value, 
      0 );
    _this->bitmap = ((ResourcesBitmap)value->buffer );
  }

  EwSignal( EwNewSlot( _this, ViewsWarpGroup_observerSlot ), ((XObject)_this ));
}

/* Variants derived from the class : 'Views::WarpGroup' */
EW_DEFINE_CLASS_VARIANTS( ViewsWarpGroup )
EW_END_OF_CLASS_VARIANTS( ViewsWarpGroup )

/* Virtual Method Table (VMT) for the class : 'Views::WarpGroup' */
EW_DEFINE_CLASS( ViewsWarpGroup, ViewsWarpView, cursorTargetView, i11, i11, i11, 
                 i11, i11, "Views::WarpGroup" )
  CoreQuadView_initLayoutContext,
  CoreView_GetRoot,
  ViewsWarpGroup_Draw,
  ViewsWarpGroup_HandleEvent,
  ViewsWarpGroup_CursorHitTest,
  CoreQuadView_ArrangeView,
  CoreQuadView_MoveView,
  CoreQuadView_GetExtent,
  CoreView_ChangeViewState,
  ViewsWarpView_OnSetPoint4,
  ViewsWarpView_OnSetPoint3,
  ViewsWarpView_OnSetPoint2,
  ViewsWarpView_OnSetPoint1,
  ViewsWarpGroup_update,
  ViewsWarpGroup_RotateAndScale,
EW_END_OF_CLASS( ViewsWarpGroup )

/* Initializer for the class 'Views::FillPath' */
void ViewsFillPath__Init( ViewsFillPath _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreRectView__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ViewsFillPath );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ViewsFillPath );

  /* ... and initialize objects, variables, properties, etc. */
  _this->ColorBL = _Const0000;
  _this->ColorBR = _Const0000;
  _this->ColorTR = _Const0000;
  _this->ColorTL = _Const0000;
  _this->Quality = 1;
  _this->FillRule = GraphicsFillRuleEvenOdd;
}

/* Re-Initializer for the class 'Views::FillPath' */
void ViewsFillPath__ReInit( ViewsFillPath _this )
{
  /* At first re-initialize the super class ... */
  CoreRectView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Views::FillPath' */
void ViewsFillPath__Done( ViewsFillPath _this )
{
  /* Call the user defined destructor of the class */
  ViewsFillPath_Done( _this );

  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreRectView );

  /* Don't forget to deinitialize the super class ... */
  CoreRectView__Done( &_this->_Super );
}

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void ViewsFillPath_Draw( ViewsFillPath _this, GraphicsCanvas aCanvas, XRect aClip, 
  XPoint aOffset, XInt32 aOpacity, XBool aBlend )
{
  GraphicsCanvas bufferCanvas;
  XColor ctl;
  XColor ctr;
  XColor cbl;
  XColor cbr;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aBlend );

  if (( _this->Path == 0 ) || ( _this->Buffered && ( _this->buffer == 0 )))
    return;

  bufferCanvas = 0;
  ctl = _this->ColorTL;
  ctr = _this->ColorTR;
  cbl = _this->ColorBL;
  cbr = _this->ColorBR;

  if ( _this->Buffered )
  {
    bufferCanvas = EwNewObject( GraphicsCanvas, 0 );
    GraphicsCanvas_AttachBitmap( bufferCanvas, _this->buffer );
  }

  aOpacity = aOpacity + 1;

  if ( aOpacity < 256 )
  {
    ctl.Alpha = (XUInt8)(( aOpacity * ctl.Alpha ) >> 8 );
    ctr.Alpha = (XUInt8)(( aOpacity * ctr.Alpha ) >> 8 );
    cbl.Alpha = (XUInt8)(( aOpacity * cbl.Alpha ) >> 8 );
    cbr.Alpha = (XUInt8)(( aOpacity * cbr.Alpha ) >> 8 );
  }

  if ( bufferCanvas == 0 )
    GraphicsCanvas_FillPath( aCanvas, aClip, _this->Path, EwMoveRectPos( _this->Super1.Bounds, 
    aOffset ), _this->FlipY, _this->Offset, ctl, ctr, cbr, cbl, 1, _this->Quality, 
    _this->FillRule );
  else
  {
    GraphicsCanvas_CopyBitmap( aCanvas, aClip, ((ResourcesBitmap)bufferCanvas ), 
    0, EwMoveRectPos( _this->Super1.Bounds, aOffset ), _Const0002, ctl, ctr, cbr, 
    cbl, 1 );
    GraphicsCanvas_DetachBitmap( bufferCanvas );
  }
}

/* 'C' function for method : 'Views::FillPath.OnSetBounds()' */
void ViewsFillPath_OnSetBounds( ViewsFillPath _this, XRect value )
{
  if ( _this->Buffered && EwCompPoint( EwGetRectSize( _this->Super1.Bounds ), EwGetRectSize( 
      value )))
    EwPostSignal( EwNewSlot( _this, ViewsFillPath_updateBuffer ), ((XObject)_this ));

  CoreRectView_OnSetBounds((CoreRectView)_this, value );
}

/* 'C' function for method : 'Views::FillPath.Done()' */
void ViewsFillPath_Done( ViewsFillPath _this )
{
  ViewsFillPath_destroyBuffer( _this );
}

/* 'C' function for method : 'Views::FillPath.destroyBuffer()' */
void ViewsFillPath_destroyBuffer( ViewsFillPath _this )
{
  XHandle handle;

  if ( _this->buffer == 0 )
    return;

  handle = _this->buffer;
  EwFreeBitmap((XBitmap*)handle );
  _this->buffer = 0;
}

/* 'C' function for method : 'Views::FillPath.updateBuffer()' */
void ViewsFillPath_updateBuffer( ViewsFillPath _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  ViewsFillPath_destroyBuffer( _this );

  if ( _this->Buffered && ( _this->Path != 0 ))
  {
    XHandle handle = 0;
    XHandle path = _this->Path->path;
    XPoint size = EwGetRectSize( _this->Super1.Bounds );
    XBool quality = _this->Quality;
    XBool flipY = _this->FlipY;
    XPoint offset = _this->Offset;
    XBool nzwind = (XBool)( _this->FillRule == GraphicsFillRuleNonZero );
    handle = (XHandle)EwGetBitmapFromFillPath( size, (XPath*)path, flipY, offset, quality, nzwind );
    _this->buffer = handle;
  }
}

/* 'C' function for method : 'Views::FillPath.updatePath()' */
void ViewsFillPath_updatePath( ViewsFillPath _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  if ( _this->Buffered )
    EwPostSignal( EwNewSlot( _this, ViewsFillPath_updateBuffer ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::FillPath.OnSetColor()' */
void ViewsFillPath_OnSetColor( ViewsFillPath _this, XColor value )
{
  if ((( !EwCompColor( value, _this->ColorTL ) && !EwCompColor( value, _this->ColorTR )) 
      && !EwCompColor( value, _this->ColorBL )) && !EwCompColor( value, _this->ColorBR ))
    return;

  _this->ColorTL = value;
  _this->ColorTR = value;
  _this->ColorBL = value;
  _this->ColorBR = value;

  if ((( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible )) && ( _this->Path != 0 ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::FillPath.OnSetOffset()' */
void ViewsFillPath_OnSetOffset( ViewsFillPath _this, XPoint value )
{
  if ( !EwCompPoint( value, _this->Offset ))
    return;

  _this->Offset = value;

  if ((( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible )) && ( _this->Path != 0 ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  if ( _this->Buffered )
    EwPostSignal( EwNewSlot( _this, ViewsFillPath_updateBuffer ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::FillPath.OnSetPath()' */
void ViewsFillPath_OnSetPath( ViewsFillPath _this, GraphicsPath value )
{
  if ( value == _this->Path )
    return;

  if ( _this->Path != 0 )
    EwDetachObjObserver( EwNewSlot( _this, ViewsFillPath_updatePath ), (XObject)_this->Path, 
      0 );

  _this->Path = value;

  if ( _this->Path != 0 )
    EwAttachObjObserver( EwNewSlot( _this, ViewsFillPath_updatePath ), (XObject)_this->Path, 
      0 );

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  if ( _this->Buffered )
    EwPostSignal( EwNewSlot( _this, ViewsFillPath_updateBuffer ), ((XObject)_this ));
}

/* Variants derived from the class : 'Views::FillPath' */
EW_DEFINE_CLASS_VARIANTS( ViewsFillPath )
EW_END_OF_CLASS_VARIANTS( ViewsFillPath )

/* Virtual Method Table (VMT) for the class : 'Views::FillPath' */
EW_DEFINE_CLASS( ViewsFillPath, CoreRectView, Path, buffer, buffer, buffer, buffer, 
                 buffer, "Views::FillPath" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  ViewsFillPath_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreView_ChangeViewState,
  ViewsFillPath_OnSetBounds,
EW_END_OF_CLASS( ViewsFillPath )

/* Initializer for the class 'Views::StrokePath' */
void ViewsStrokePath__Init( ViewsStrokePath _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreRectView__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ViewsStrokePath );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ViewsStrokePath );

  /* ... and initialize objects, variables, properties, etc. */
  _this->ColorBL = _Const0000;
  _this->ColorBR = _Const0000;
  _this->ColorTR = _Const0000;
  _this->ColorTL = _Const0000;
  _this->Quality = 1;
  _this->MiterLimit = 3.000000f;
  _this->JoinPoints = GraphicsPathJoinBevel;
  _this->EndCap = GraphicsPathCapFlat;
  _this->StartCap = GraphicsPathCapFlat;
  _this->Width = 1.000000f;
}

/* Re-Initializer for the class 'Views::StrokePath' */
void ViewsStrokePath__ReInit( ViewsStrokePath _this )
{
  /* At first re-initialize the super class ... */
  CoreRectView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Views::StrokePath' */
void ViewsStrokePath__Done( ViewsStrokePath _this )
{
  /* Call the user defined destructor of the class */
  ViewsStrokePath_Done( _this );

  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreRectView );

  /* Don't forget to deinitialize the super class ... */
  CoreRectView__Done( &_this->_Super );
}

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void ViewsStrokePath_Draw( ViewsStrokePath _this, GraphicsCanvas aCanvas, XRect 
  aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
{
  GraphicsCanvas bufferCanvas;
  XColor ctl;
  XColor ctr;
  XColor cbl;
  XColor cbr;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aBlend );

  if (( _this->Path == 0 ) || ( _this->Buffered && ( _this->buffer == 0 )))
    return;

  bufferCanvas = 0;
  ctl = _this->ColorTL;
  ctr = _this->ColorTR;
  cbl = _this->ColorBL;
  cbr = _this->ColorBR;

  if ( _this->Buffered )
  {
    bufferCanvas = EwNewObject( GraphicsCanvas, 0 );
    GraphicsCanvas_AttachBitmap( bufferCanvas, _this->buffer );
  }

  aOpacity = aOpacity + 1;

  if ( aOpacity < 256 )
  {
    ctl.Alpha = (XUInt8)(( aOpacity * ctl.Alpha ) >> 8 );
    ctr.Alpha = (XUInt8)(( aOpacity * ctr.Alpha ) >> 8 );
    cbl.Alpha = (XUInt8)(( aOpacity * cbl.Alpha ) >> 8 );
    cbr.Alpha = (XUInt8)(( aOpacity * cbr.Alpha ) >> 8 );
  }

  if ( bufferCanvas == 0 )
    GraphicsCanvas_StrokePath( aCanvas, aClip, _this->Path, EwMoveRectPos( _this->Super1.Bounds, 
    aOffset ), _this->FlipY, _this->Offset, _this->Width, _this->StartCap, _this->EndCap, 
    _this->JoinPoints, _this->MiterLimit, ctl, ctr, cbr, cbl, 1, _this->Quality );
  else
  {
    GraphicsCanvas_CopyBitmap( aCanvas, aClip, ((ResourcesBitmap)bufferCanvas ), 
    0, EwMoveRectPos( _this->Super1.Bounds, aOffset ), _Const0002, ctl, ctr, cbr, 
    cbl, 1 );
    GraphicsCanvas_DetachBitmap( bufferCanvas );
  }
}

/* 'C' function for method : 'Views::StrokePath.OnSetBounds()' */
void ViewsStrokePath_OnSetBounds( ViewsStrokePath _this, XRect value )
{
  if ( _this->Buffered && EwCompPoint( EwGetRectSize( _this->Super1.Bounds ), EwGetRectSize( 
      value )))
    EwPostSignal( EwNewSlot( _this, ViewsStrokePath_updateBuffer ), ((XObject)_this ));

  CoreRectView_OnSetBounds((CoreRectView)_this, value );
}

/* 'C' function for method : 'Views::StrokePath.Done()' */
void ViewsStrokePath_Done( ViewsStrokePath _this )
{
  ViewsStrokePath_destroyBuffer( _this );
}

/* 'C' function for method : 'Views::StrokePath.destroyBuffer()' */
void ViewsStrokePath_destroyBuffer( ViewsStrokePath _this )
{
  XHandle handle;

  if ( _this->buffer == 0 )
    return;

  handle = _this->buffer;
  EwFreeBitmap((XBitmap*)handle );
  _this->buffer = 0;
}

/* 'C' function for method : 'Views::StrokePath.updateBuffer()' */
void ViewsStrokePath_updateBuffer( ViewsStrokePath _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  ViewsStrokePath_destroyBuffer( _this );

  if ( _this->Buffered && ( _this->Path != 0 ))
  {
    XHandle handle = 0;
    XHandle path = _this->Path->path;
    XPoint size = EwGetRectSize( _this->Super1.Bounds );
    XBool quality = _this->Quality;
    XBool flipY = _this->FlipY;
    XPoint offset = _this->Offset;
    XFloat width = _this->Width;
    XFloat miterLimit = _this->MiterLimit;
    XUInt32 style = 0;

    switch ( _this->StartCap )
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

    switch ( _this->EndCap )
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

    switch ( _this->JoinPoints )
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

    handle = (XHandle)EwGetBitmapFromStrokePath( size, (XPath*)path, flipY, offset, width, style, miterLimit, quality );
    _this->buffer = handle;
  }
}

/* 'C' function for method : 'Views::StrokePath.updatePath()' */
void ViewsStrokePath_updatePath( ViewsStrokePath _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  if ( _this->Buffered )
    EwPostSignal( EwNewSlot( _this, ViewsStrokePath_updateBuffer ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::StrokePath.OnSetColor()' */
void ViewsStrokePath_OnSetColor( ViewsStrokePath _this, XColor value )
{
  if ((( !EwCompColor( value, _this->ColorTL ) && !EwCompColor( value, _this->ColorTR )) 
      && !EwCompColor( value, _this->ColorBL )) && !EwCompColor( value, _this->ColorBR ))
    return;

  _this->ColorTL = value;
  _this->ColorTR = value;
  _this->ColorBL = value;
  _this->ColorBR = value;

  if ((( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible )) && ( _this->Path != 0 ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );
}

/* 'C' function for method : 'Views::StrokePath.OnSetMiterLimit()' */
void ViewsStrokePath_OnSetMiterLimit( ViewsStrokePath _this, XFloat value )
{
  if ( value < 0.000000f )
    value = 0.000000f;

  if ( value == _this->MiterLimit )
    return;

  _this->MiterLimit = value;

  if ((( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible )) && ( _this->Path != 0 ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  if ( _this->Buffered )
    EwPostSignal( EwNewSlot( _this, ViewsStrokePath_updateBuffer ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::StrokePath.OnSetJoinPoints()' */
void ViewsStrokePath_OnSetJoinPoints( ViewsStrokePath _this, XEnum value )
{
  if ( value == _this->JoinPoints )
    return;

  _this->JoinPoints = value;

  if ((( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible )) && ( _this->Path != 0 ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  if ( _this->Buffered )
    EwPostSignal( EwNewSlot( _this, ViewsStrokePath_updateBuffer ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::StrokePath.OnSetStartCap()' */
void ViewsStrokePath_OnSetStartCap( ViewsStrokePath _this, XEnum value )
{
  if ( value == _this->StartCap )
    return;

  _this->StartCap = value;

  if ((( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible )) && ( _this->Path != 0 ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  if ( _this->Buffered )
    EwPostSignal( EwNewSlot( _this, ViewsStrokePath_updateBuffer ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::StrokePath.OnSetWidth()' */
void ViewsStrokePath_OnSetWidth( ViewsStrokePath _this, XFloat value )
{
  if ( value < 0.000000f )
    value = 0.000000f;

  if ( value == _this->Width )
    return;

  _this->Width = value;

  if ((( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible )) && ( _this->Path != 0 ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  if ( _this->Buffered )
    EwPostSignal( EwNewSlot( _this, ViewsStrokePath_updateBuffer ), ((XObject)_this ));
}

/* 'C' function for method : 'Views::StrokePath.OnSetPath()' */
void ViewsStrokePath_OnSetPath( ViewsStrokePath _this, GraphicsPath value )
{
  if ( value == _this->Path )
    return;

  if ( _this->Path != 0 )
    EwDetachObjObserver( EwNewSlot( _this, ViewsStrokePath_updatePath ), (XObject)_this->Path, 
      0 );

  _this->Path = value;

  if ( _this->Path != 0 )
    EwAttachObjObserver( EwNewSlot( _this, ViewsStrokePath_updatePath ), (XObject)_this->Path, 
      0 );

  if (( _this->Super2.Owner != 0 ) && (( _this->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super2.Owner, _this->Super1.Bounds );

  if ( _this->Buffered )
    EwPostSignal( EwNewSlot( _this, ViewsStrokePath_updateBuffer ), ((XObject)_this ));
}

/* Variants derived from the class : 'Views::StrokePath' */
EW_DEFINE_CLASS_VARIANTS( ViewsStrokePath )
EW_END_OF_CLASS_VARIANTS( ViewsStrokePath )

/* Virtual Method Table (VMT) for the class : 'Views::StrokePath' */
EW_DEFINE_CLASS( ViewsStrokePath, CoreRectView, Path, buffer, buffer, buffer, buffer, 
                 buffer, "Views::StrokePath" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  ViewsStrokePath_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreView_ChangeViewState,
  ViewsStrokePath_OnSetBounds,
EW_END_OF_CLASS( ViewsStrokePath )

/* Embedded Wizard */
