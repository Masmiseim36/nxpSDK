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
#include "_ApplicationApplication.h"
#include "_CoreCursorEvent.h"
#include "_CoreCursorGrabEvent.h"
#include "_CoreCursorHit.h"
#include "_CoreDialogContext.h"
#include "_CoreDragEvent.h"
#include "_CoreEvent.h"
#include "_CoreGroup.h"
#include "_CoreKeyEvent.h"
#include "_CoreKeyPressHandler.h"
#include "_CoreLayoutContext.h"
#include "_CoreLayoutQuadContext.h"
#include "_CoreModalContext.h"
#include "_CoreOutline.h"
#include "_CoreQuadView.h"
#include "_CoreRectView.h"
#include "_CoreResource.h"
#include "_CoreRoot.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreTask.h"
#include "_CoreTaskQueue.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_GraphicsCanvas.h"
#include "_ResourcesBitmap.h"
#include "Core.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x00000230, /* ratio 52.86 % */
  0xB8005500, 0x00000452, 0x00D0002A, 0x04000328, 0xD2003B00, 0xA770042C, 0x00320021,
  0x639C4CDE, 0x00464DD1, 0x998A2C74, 0xC7A3E6C8, 0x6A431633, 0xA1521874, 0x510E8E44,
  0x0758F9CA, 0x00831C00, 0x082C0C2C, 0x8747C9C0, 0xAC322510, 0x8027F2C8, 0x86390C99,
  0x0C60030C, 0x3B383580, 0x256834EC, 0x56A244E2, 0xA8F00261, 0x8943A431, 0x04BA8B1E,
  0x4D2655A0, 0x4C25389B, 0xE83469ED, 0x994694D5, 0x7068C462, 0x42AADAC3, 0x5A645A29,
  0xB9002912, 0x0458F200, 0x59A2D7BB, 0x1A6F358D, 0xCA56B1A4, 0x6735A744, 0x91089472,
  0x0FADD7E1, 0x292802FF, 0x90D32311, 0x88022F96, 0x6164F56C, 0x4F2921A7, 0xF4589874,
  0x034DD5E7, 0xD0C92EC7, 0x6B2C4E05, 0xBD0C2A11, 0x2860A264, 0x05923747, 0x47B34480,
  0x5F83B48C, 0x0DB0CAD6, 0xE70C9180, 0xB0E8C44E, 0xA596C4A2, 0x38FD870B, 0x9063CD52,
  0x7D7A3F1D, 0x4A30913E, 0xCE191887, 0xA2DD1D54, 0x9D1AE099, 0xD1F21C3B, 0xF7000EAF,
  0x8D1FC08F, 0x9C4E36D6, 0x00000101, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XPoint _Const0000 = { 0, 0 };
static const XRect _Const0001 = {{ 0, 0 }, { 0, 0 }};
static const XStringRes _Const0002 = { _StringsDefault0, 0x0003 };
static const XStringRes _Const0003 = { _StringsDefault0, 0x002D };
static const XStringRes _Const0004 = { _StringsDefault0, 0x0043 };
static const XStringRes _Const0005 = { _StringsDefault0, 0x0060 };
static const XStringRes _Const0006 = { _StringsDefault0, 0x0072 };
static const XColor _Const0007 = { 0x00, 0x00, 0x00, 0x00 };
static const XStringRes _Const0008 = { _StringsDefault0, 0x008D };
static const XStringRes _Const0009 = { _StringsDefault0, 0x00C1 };

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

/* Global constant containing the preferred size of the screen in pixel. */
const XPoint EwScreenSize = { 480, 272 };

/* Global constant containing the main application class. */
const XClass EwApplicationClass = EW_CLASS( ApplicationApplication );

/* Global constant containing the user defined application title. */
const char* EwApplicationTitle = "";

/* Initializer for the class 'Core::View' */
void CoreView__Init( CoreView _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreView );

  /* ... and initialize objects, variables, properties, etc. */
  _this->viewState = CoreViewStateAlphaBlended | CoreViewStateFastReshape | CoreViewStateVisible;
  _this->Layout = CoreLayoutAlignToLeft | CoreLayoutAlignToTop;
}

/* Re-Initializer for the class 'Core::View' */
void CoreView__ReInit( CoreView _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::View' */
void CoreView__Done( CoreView _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreView );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Core::View.initLayoutContext()' */
void CoreView_initLayoutContext( CoreView _this, XRect aBounds, CoreOutline aOutline )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aOutline );
  EW_UNUSED_ARG( aBounds );
}

/* Wrapper function for the virtual method : 'Core::View.initLayoutContext()' */
void CoreView__initLayoutContext( void* _this, XRect aBounds, CoreOutline aOutline )
{
  ((CoreView)_this)->_VMT->initLayoutContext((CoreView)_this, aBounds, aOutline );
}

/* The method GetRoot() delivers the application object, this view belongs to. The 
   application object represents the entire screen of the GUI application. Thus 
   in the views hierarchy, the application object serves as the root view.
   This method can fail and return null if the view still doesn't belong to any 
   owner group. */
CoreRoot CoreView_GetRoot( CoreView _this )
{
  CoreGroup grp = _this->Owner;

  while ( grp != 0 )
  {
    CoreRoot root = EwCastObject( grp, CoreRoot );

    if ( root != 0 )
      return root;

    grp = grp->Super2.Owner;
  }

  return 0;
}

/* Wrapper function for the virtual method : 'Core::View.GetRoot()' */
CoreRoot CoreView__GetRoot( void* _this )
{
  return ((CoreView)_this)->_VMT->GetRoot((CoreView)_this );
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
void CoreView_Draw( CoreView _this, GraphicsCanvas aCanvas, XRect aClip, XPoint 
  aOffset, XInt32 aOpacity, XBool aBlend )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aBlend );
  EW_UNUSED_ARG( aOpacity );
  EW_UNUSED_ARG( aOffset );
  EW_UNUSED_ARG( aClip );
  EW_UNUSED_ARG( aCanvas );
}

/* Wrapper function for the virtual method : 'Core::View.Draw()' */
void CoreView__Draw( void* _this, GraphicsCanvas aCanvas, XRect aClip, XPoint aOffset, 
  XInt32 aOpacity, XBool aBlend )
{
  ((CoreView)_this)->_VMT->Draw((CoreView)_this, aCanvas, aClip, aOffset, aOpacity
  , aBlend );
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
XObject CoreView_HandleEvent( CoreView _this, CoreEvent aEvent )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aEvent );

  return 0;
}

/* Wrapper function for the virtual method : 'Core::View.HandleEvent()' */
XObject CoreView__HandleEvent( void* _this, CoreEvent aEvent )
{
  return ((CoreView)_this)->_VMT->HandleEvent((CoreView)_this, aEvent );
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
CoreCursorHit CoreView_CursorHitTest( CoreView _this, XRect aArea, XInt32 aFinger, 
  XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aRetargetReason );
  EW_UNUSED_ARG( aDedicatedView );
  EW_UNUSED_ARG( aStrikeCount );
  EW_UNUSED_ARG( aFinger );
  EW_UNUSED_ARG( aArea );

  return 0;
}

/* Wrapper function for the virtual method : 'Core::View.CursorHitTest()' */
CoreCursorHit CoreView__CursorHitTest( void* _this, XRect aArea, XInt32 aFinger, 
  XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
{
  return ((CoreView)_this)->_VMT->CursorHitTest((CoreView)_this, aArea, aFinger, 
  aStrikeCount, aDedicatedView, aRetargetReason );
}

/* The method ArrangeView() is invoked automatically if the superior @Owner group 
   needs to re-arrange its views. For example, the changing of the owners size can 
   cause the enclosed views to adapt their position and size, so all views still 
   fit within the new owners area. This method provides the core functionality for 
   the automatic GUI layout mechanism.
   The arrangement is controlled primarily by the @Layout property of the view. 
   It specifies a set of alignment constraints and determines whether the view can 
   change its size. The bounds area where the view should be arranged, is passed 
   in the parameter aBounds. This is usually the current area of the views owner.
   The parameter aFormation defines the desired arrangement mode. Depending on the 
   value of this parameter, the views can be arranged in rows or columns. If aFormation 
   == Core::Formation.None, no automatic row/column arrangement is performed and 
   the view is moved and scaled only to fit inside the aBounds area.
   ArrangeView() is invoked automatically by the framework, so you never should 
   need to invoke it directly.
   The method returns the size of the view after it has been arranged. */
XPoint CoreView_ArrangeView( CoreView _this, XRect aBounds, XEnum aFormation )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aFormation );
  EW_UNUSED_ARG( aBounds );

  return _Const0000;
}

/* Wrapper function for the virtual method : 'Core::View.ArrangeView()' */
XPoint CoreView__ArrangeView( void* _this, XRect aBounds, XEnum aFormation )
{
  return ((CoreView)_this)->_VMT->ArrangeView((CoreView)_this, aBounds, aFormation );
}

/* The method MoveView() changes the position of the view by adding the value aOffset 
   to all corners of the view. For example, in case of a line view the value is 
   added to the both line end points.
   The parameter aFastMove serves for the optimization purpose. If it is 'true', 
   the corners are modified without performing any updates of the view and without 
   redrawing the screen. This is useful, when you wish to move or arrange a lot 
   of views at once. In this case it's up to you to request the finally screen update. 
   To do this you can use the method InvalidateArea() of the views @Owner.
   In the case aFastMove == false, the operation automatically requests the screen 
   redraw of the view areas before and after the movement. You don't need to take 
   care about it. */
void CoreView_MoveView( CoreView _this, XPoint aOffset, XBool aFastMove )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aFastMove );
  EW_UNUSED_ARG( aOffset );
}

/* Wrapper function for the virtual method : 'Core::View.MoveView()' */
void CoreView__MoveView( void* _this, XPoint aOffset, XBool aFastMove )
{
  ((CoreView)_this)->_VMT->MoveView((CoreView)_this, aOffset, aFastMove );
}

/* The method GetExtent() returns the position and the size of the view relative 
   to the origin of its @Owner. In case of views with a non rectangular shape the 
   method returns the rectangular boundary area enclosing the entire shape. */
XRect CoreView_GetExtent( CoreView _this )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );

  return _Const0001;
}

/* Wrapper function for the virtual method : 'Core::View.GetExtent()' */
XRect CoreView__GetExtent( void* _this )
{
  return ((CoreView)_this)->_VMT->GetExtent((CoreView)_this );
}

/* The method ChangeViewState() modifies the current state of the view. The state 
   is a set of switches determining whether a view is visible, whether it can react 
   to user inputs or whether it is just performing some update operations, etc.
   The modification is controlled by the the both parameters. The first aSetState 
   contains the switches to activate within the view state. The second aClearState 
   determines all switches to disable.
   Depending on the state alteration the method will perform different operations, 
   e.g. in response to the clearing of the visible state, the method will request 
   a screen redraw to make disappear the view from the screen.
   ChangeViewState() is invoked automatically by the framework, so you never should 
   need to invoke it directly. All relevant states are available as properties e.g. 
   the property Visible in derived classes reflects the visible state of the view. */
void CoreView_ChangeViewState( CoreView _this, XSet aSetState, XSet aClearState )
{
  XSet newState;
  XSet deltaState;

  if ((( _this->viewState & CoreViewStateIsOutline ) == CoreViewStateIsOutline ))
    aSetState = aSetState & ~CoreViewStateEmbedded;

  newState = ( _this->viewState & ~aClearState ) | aSetState;
  deltaState = newState ^ _this->viewState;
  _this->viewState = newState;

  if (( _this->Owner != 0 ) && !!( deltaState & ( CoreViewStateEnabled | CoreViewStateFocusable )))
  {
    XBool focusable = (( _this->viewState & ( CoreViewStateEnabled | CoreViewStateFocusable )) 
      == ( CoreViewStateEnabled | CoreViewStateFocusable ));

    if ( focusable && ( _this->Owner->Focus == 0 ))
      CoreGroup__OnSetFocus( _this->Owner, _this );

    if ( !focusable && ( _this->Owner->Focus == _this ))
      CoreGroup__OnSetFocus( _this->Owner, CoreGroup_FindSiblingView( _this->Owner, 
      _this, CoreViewStateEnabled | CoreViewStateFocusable ));
  }

  if (( _this->Owner != 0 ) && !!( deltaState & ( CoreViewStateAlphaBlended | CoreViewStateEmbedded 
      | CoreViewStateVisible )))
    CoreGroup__InvalidateArea( _this->Owner, CoreView__GetExtent( _this ));

  if ((( _this->layoutContext != 0 ) && (( newState & CoreViewStateEmbedded ) == 
      CoreViewStateEmbedded )) && (( deltaState & CoreViewStateVisible ) == CoreViewStateVisible ))
  {
    _this->viewState = _this->viewState | CoreViewStateRequestLayout;
    _this->Owner->Super2.viewState = _this->Owner->Super2.viewState | CoreViewStatePendingLayout;
    EwPostSignal( EwNewSlot( _this->Owner, CoreGroup_updateComponent ), ((XObject)_this ));
  }

  if (( _this->Owner != 0 ) && (( deltaState & CoreViewStateEmbedded ) == CoreViewStateEmbedded ))
  {
    _this->layoutContext = 0;
    _this->viewState = _this->viewState | CoreViewStateRequestLayout;
    _this->Owner->Super2.viewState = _this->Owner->Super2.viewState | CoreViewStatePendingLayout;
    EwPostSignal( EwNewSlot( _this->Owner, CoreGroup_updateComponent ), ((XObject)_this ));
  }
}

/* Wrapper function for the virtual method : 'Core::View.ChangeViewState()' */
void CoreView__ChangeViewState( void* _this, XSet aSetState, XSet aClearState )
{
  ((CoreView)_this)->_VMT->ChangeViewState((CoreView)_this, aSetState, aClearState );
}

/* Variants derived from the class : 'Core::View' */
EW_DEFINE_CLASS_VARIANTS( CoreView )
EW_END_OF_CLASS_VARIANTS( CoreView )

/* Virtual Method Table (VMT) for the class : 'Core::View' */
EW_DEFINE_CLASS( CoreView, XObject, next, viewState, viewState, viewState, viewState, 
                 "Core::View" )
  CoreView_initLayoutContext,
  CoreView_GetRoot,
  CoreView_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreView_ArrangeView,
  CoreView_MoveView,
  CoreView_GetExtent,
  CoreView_ChangeViewState,
EW_END_OF_CLASS( CoreView )

/* Initializer for the class 'Core::QuadView' */
void CoreQuadView__Init( CoreQuadView _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreView__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreQuadView );

  /* ... and initialize objects, variables, properties, etc. */
}

/* Re-Initializer for the class 'Core::QuadView' */
void CoreQuadView__ReInit( CoreQuadView _this )
{
  /* At first re-initialize the super class ... */
  CoreView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::QuadView' */
void CoreQuadView__Done( CoreQuadView _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreQuadView );

  /* Don't forget to deinitialize the super class ... */
  CoreView__Done( &_this->_Super );
}

/* 'C' function for method : 'Core::QuadView.initLayoutContext()' */
void CoreQuadView_initLayoutContext( CoreQuadView _this, XRect aBounds, CoreOutline 
  aOutline )
{
  CoreLayoutQuadContext context = EwNewObject( CoreLayoutQuadContext, 0 );

  _this->Super1.layoutContext = 0;
  context->Super1.extent = CoreView__GetExtent( _this );
  context->Super1.bounds = aBounds;
  context->Super1.outline = aOutline;
  context->point1 = _this->Point1;
  context->point2 = _this->Point2;
  context->point3 = _this->Point3;
  context->point4 = _this->Point4;
  _this->Super1.layoutContext = ((CoreLayoutContext)context );
}

/* The method ArrangeView() is invoked automatically if the superior @Owner group 
   needs to re-arrange its views. For example, the changing of the owners size can 
   cause the enclosed views to adapt their position and size, so all views still 
   fit within the new owners area. This method provides the core functionality for 
   the automatic GUI layout mechanism.
   The arrangement is controlled primarily by the @Layout property of the view. 
   It specifies a set of alignment constraints and determines whether the view can 
   change its size. The bounds area where the view should be arranged, is passed 
   in the parameter aBounds. This is usually the current area of the views owner.
   The parameter aFormation defines the desired arrangement mode. Depending on the 
   value of this parameter, the views can be arranged in rows or columns. If aFormation 
   == Core::Formation.None, no automatic row/column arrangement is performed and 
   the view is moved and scaled only to fit inside the aBounds area.
   ArrangeView() is invoked automatically by the framework, so you never should 
   need to invoke it directly.
   The method returns the size of the view after it has been arranged. */
XPoint CoreQuadView_ArrangeView( CoreQuadView _this, XRect aBounds, XEnum aFormation )
{
  XSet layout = _this->Super1.Layout;
  CoreLayoutQuadContext context = EwCastObject( _this->Super1.layoutContext, CoreLayoutQuadContext );
  XInt32 x1 = context->Super1.extent.Point1.X;
  XInt32 y1 = context->Super1.extent.Point1.Y;
  XInt32 x2 = context->Super1.extent.Point2.X;
  XInt32 y2 = context->Super1.extent.Point2.Y;
  XPoint dstSize = EwGetRectSize( aBounds );
  XInt32 w = x2 - x1;
  XInt32 h = y2 - y1;
  XInt32 ex;
  XInt32 ey;
  XInt32 ew;
  XInt32 eh;

  if ( aFormation == CoreFormationNone )
  {
    XPoint srcSize = EwGetRectSize( context->Super1.bounds );
    x1 = x1 - context->Super1.bounds.Point1.X;
    y1 = y1 - context->Super1.bounds.Point1.Y;

    if ( srcSize.X != dstSize.X )
    {
      XBool alignToLeft = (( layout & CoreLayoutAlignToLeft ) == CoreLayoutAlignToLeft );
      XBool alignToRight = (( layout & CoreLayoutAlignToRight ) == CoreLayoutAlignToRight );
      XBool resizeHorz = (( layout & CoreLayoutResizeHorz ) == CoreLayoutResizeHorz );

      if ( !alignToLeft && ( resizeHorz || !alignToRight ))
        x1 = ( x1 * dstSize.X ) / srcSize.X;

      if ( !alignToRight && ( resizeHorz || !alignToLeft ))
      {
        x2 = x2 - context->Super1.bounds.Point1.X;
        x2 = ( x2 * dstSize.X ) / srcSize.X;
        x2 = x2 - dstSize.X;
      }
      else
        x2 = x2 - context->Super1.bounds.Point2.X;

      x1 = x1 + aBounds.Point1.X;
      x2 = x2 + aBounds.Point2.X;

      if ( !resizeHorz )
      {
        if ( alignToLeft && !alignToRight )
          x2 = x1 + w;
        else
          if ( !alignToLeft && alignToRight )
            x1 = x2 - w;
          else
          {
            x1 = x1 + ((( x2 - x1 ) - w ) / 2 );
            x2 = x1 + w;
          }
      }
    }
    else
    {
      x2 = x2 - context->Super1.bounds.Point2.X;
      x1 = x1 + aBounds.Point1.X;
      x2 = x2 + aBounds.Point2.X;
    }

    if ( srcSize.Y != dstSize.Y )
    {
      XBool alignToTop = (( layout & CoreLayoutAlignToTop ) == CoreLayoutAlignToTop );
      XBool alignToBottom = (( layout & CoreLayoutAlignToBottom ) == CoreLayoutAlignToBottom );
      XBool resizeVert = (( layout & CoreLayoutResizeVert ) == CoreLayoutResizeVert );

      if ( !alignToTop && ( resizeVert || !alignToBottom ))
        y1 = ( y1 * dstSize.Y ) / srcSize.Y;

      if ( !alignToBottom && ( resizeVert || !alignToTop ))
      {
        y2 = y2 - context->Super1.bounds.Point1.Y;
        y2 = ( y2 * dstSize.Y ) / srcSize.Y;
        y2 = y2 - dstSize.Y;
      }
      else
        y2 = y2 - context->Super1.bounds.Point2.Y;

      y1 = y1 + aBounds.Point1.Y;
      y2 = y2 + aBounds.Point2.Y;

      if ( !resizeVert )
      {
        if ( alignToTop && !alignToBottom )
          y2 = y1 + h;
        else
          if ( !alignToTop && alignToBottom )
            y1 = y2 - h;
          else
          {
            y1 = y1 + ((( y2 - y1 ) - h ) / 2 );
            y2 = y1 + h;
          }
      }
    }
    else
    {
      y2 = y2 - context->Super1.bounds.Point2.Y;
      y1 = y1 + aBounds.Point1.Y;
      y2 = y2 + aBounds.Point2.Y;
    }
  }
  else
  {
    switch ( aFormation )
    {
      case CoreFormationLeftToRight :
      {
        x1 = aBounds.Point1.X;
        x2 = x1 + w;
      }
      break;

      case CoreFormationRightToLeft :
      {
        x2 = aBounds.Point2.X;
        x1 = x2 - w;
      }
      break;

      case CoreFormationTopToBottom :
      {
        y1 = aBounds.Point1.Y;
        y2 = y1 + h;
      }
      break;

      case CoreFormationBottomToTop :
      {
        y2 = aBounds.Point2.Y;
        y1 = y2 - h;
      }
      break;

      default : 
        ;
    }

    if (( aFormation == CoreFormationLeftToRight ) || ( aFormation == CoreFormationRightToLeft ))
    {
      XBool alignToTop = (( layout & CoreLayoutAlignToTop ) == CoreLayoutAlignToTop );
      XBool alignToBottom = (( layout & CoreLayoutAlignToBottom ) == CoreLayoutAlignToBottom );
      XBool resizeVert = (( layout & CoreLayoutResizeVert ) == CoreLayoutResizeVert );

      if ( resizeVert )
      {
        y1 = aBounds.Point1.Y;
        y2 = aBounds.Point2.Y;
      }
      else
        if ( alignToTop && !alignToBottom )
        {
          y1 = aBounds.Point1.Y;
          y2 = y1 + h;
        }
        else
          if ( alignToBottom && !alignToTop )
          {
            y2 = aBounds.Point2.Y;
            y1 = y2 - h;
          }
          else
          {
            y1 = aBounds.Point1.Y + (( EwGetRectH( aBounds ) - h ) / 2 );
            y2 = y1 + h;
          }
    }

    if (( aFormation == CoreFormationTopToBottom ) || ( aFormation == CoreFormationBottomToTop ))
    {
      XBool alignToLeft = (( layout & CoreLayoutAlignToLeft ) == CoreLayoutAlignToLeft );
      XBool alignToRight = (( layout & CoreLayoutAlignToRight ) == CoreLayoutAlignToRight );
      XBool resizeHorz = (( layout & CoreLayoutResizeHorz ) == CoreLayoutResizeHorz );

      if ( resizeHorz )
      {
        x1 = aBounds.Point1.X;
        x2 = aBounds.Point2.X;
      }
      else
        if ( alignToLeft && !alignToRight )
        {
          x1 = aBounds.Point1.X;
          x2 = x1 + w;
        }
        else
          if ( alignToRight && !alignToLeft )
          {
            x2 = aBounds.Point2.X;
            x1 = x2 - w;
          }
          else
          {
            x1 = aBounds.Point1.X + (( EwGetRectW( aBounds ) - w ) / 2 );
            x2 = x1 + w;
          }
    }
  }

  context->Super1.isEmpty = (XBool)(( x1 >= x2 ) || ( y1 >= y2 ));
  w = ( x2 - x1 ) - 1;
  h = ( y2 - y1 ) - 1;
  ex = context->Super1.extent.Point1.X;
  ey = context->Super1.extent.Point1.Y;
  ew = ( context->Super1.extent.Point2.X - ex ) - 1;
  eh = ( context->Super1.extent.Point2.Y - ey ) - 1;

  if ( ew == 0 )
    ew = 1;

  if ( eh == 0 )
    eh = 1;

  if ((( _this->Super1.viewState & CoreViewStateFastReshape ) == CoreViewStateFastReshape ))
  {
    _this->Point1 = EwNewPoint( x1 + ((( context->point1.X - ex ) * w ) / ew ), 
    y1 + ((( context->point1.Y - ey ) * h ) / eh ));
    _this->Point2 = EwNewPoint( x1 + ((( context->point2.X - ex ) * w ) / ew ), 
    y1 + ((( context->point2.Y - ey ) * h ) / eh ));
    _this->Point3 = EwNewPoint( x1 + ((( context->point3.X - ex ) * w ) / ew ), 
    y1 + ((( context->point3.Y - ey ) * h ) / eh ));
    _this->Point4 = EwNewPoint( x1 + ((( context->point4.X - ex ) * w ) / ew ), 
    y1 + ((( context->point4.Y - ey ) * h ) / eh ));
  }
  else
  {
    CoreQuadView_OnSetPoint1( _this, EwNewPoint( x1 + ((( context->point1.X - ex ) 
    * w ) / ew ), y1 + ((( context->point1.Y - ey ) * h ) / eh )));
    CoreQuadView_OnSetPoint2( _this, EwNewPoint( x1 + ((( context->point2.X - ex ) 
    * w ) / ew ), y1 + ((( context->point2.Y - ey ) * h ) / eh )));
    CoreQuadView_OnSetPoint3( _this, EwNewPoint( x1 + ((( context->point3.X - ex ) 
    * w ) / ew ), y1 + ((( context->point3.Y - ey ) * h ) / eh )));
    CoreQuadView_OnSetPoint4( _this, EwNewPoint( x1 + ((( context->point4.X - ex ) 
    * w ) / ew ), y1 + ((( context->point4.Y - ey ) * h ) / eh )));
    _this->Super1.layoutContext = ((CoreLayoutContext)context );
  }

  return EwNewPoint( w + 1, h + 1 );
}

/* The method MoveView() changes the position of the view by adding the value aOffset 
   to all corners of the view. For example, in case of a line view the value is 
   added to the both line end points.
   The parameter aFastMove serves for the optimization purpose. If it is 'true', 
   the corners are modified without performing any updates of the view and without 
   redrawing the screen. This is useful, when you wish to move or arrange a lot 
   of views at once. In this case it's up to you to request the finally screen update. 
   To do this you can use the method InvalidateArea() of the views @Owner.
   In the case aFastMove == false, the operation automatically requests the screen 
   redraw of the view areas before and after the movement. You don't need to take 
   care about it. */
void CoreQuadView_MoveView( CoreQuadView _this, XPoint aOffset, XBool aFastMove )
{
  if ( aFastMove )
  {
    _this->Point1 = EwMovePointPos( _this->Point1, aOffset );
    _this->Point2 = EwMovePointPos( _this->Point2, aOffset );
    _this->Point3 = EwMovePointPos( _this->Point3, aOffset );
    _this->Point4 = EwMovePointPos( _this->Point4, aOffset );
  }
  else
  {
    CoreQuadView_OnSetPoint1( _this, EwMovePointPos( _this->Point1, aOffset ));
    CoreQuadView_OnSetPoint2( _this, EwMovePointPos( _this->Point2, aOffset ));
    CoreQuadView_OnSetPoint3( _this, EwMovePointPos( _this->Point3, aOffset ));
    CoreQuadView_OnSetPoint4( _this, EwMovePointPos( _this->Point4, aOffset ));
  }
}

/* The method GetExtent() returns the position and the size of the view relative 
   to the origin of its @Owner. In case of views with a non rectangular shape the 
   method returns the rectangular boundary area enclosing the entire shape. */
XRect CoreQuadView_GetExtent( CoreQuadView _this )
{
  XInt32 x1;
  XInt32 y1;
  XInt32 x2;
  XInt32 y2;

  if (( _this->Super1.layoutContext != 0 ) && _this->Super1.layoutContext->isEmpty )
    return _Const0001;

  x1 = _this->Point1.X;
  y1 = _this->Point1.Y;
  x2 = _this->Point3.X;
  y2 = _this->Point3.Y;

  if (((( _this->Point4.X != x1 ) || ( _this->Point2.Y != y1 )) || ( _this->Point2.X 
      != x2 )) || ( _this->Point4.Y != y2 ))
  {
    if ( _this->Point2.X < x1 )
      x1 = _this->Point2.X;

    if ( _this->Point3.X < x1 )
      x1 = _this->Point3.X;

    if ( _this->Point4.X < x1 )
      x1 = _this->Point4.X;

    if ( _this->Point2.Y < y1 )
      y1 = _this->Point2.Y;

    if ( _this->Point3.Y < y1 )
      y1 = _this->Point3.Y;

    if ( _this->Point4.Y < y1 )
      y1 = _this->Point4.Y;

    if ( _this->Point1.X > x2 )
      x2 = _this->Point1.X;

    if ( _this->Point2.X > x2 )
      x2 = _this->Point2.X;

    if ( _this->Point4.X > x2 )
      x2 = _this->Point4.X;

    if ( _this->Point1.Y > y2 )
      y2 = _this->Point1.Y;

    if ( _this->Point2.Y > y2 )
      y2 = _this->Point2.Y;

    if ( _this->Point4.Y > y2 )
      y2 = _this->Point4.Y;
  }
  else
  {
    XInt32 tmp;

    if ( x2 < x1 )
    {
      tmp = x1;
      x1 = x2;
      x2 = tmp;
    }

    if ( y2 < y1 )
    {
      tmp = y1;
      y1 = y2;
      y2 = tmp;
    }
  }

  return EwNewRect( x1, y1, x2 + 1, y2 + 1 );
}

/* 'C' function for method : 'Core::QuadView.OnSetPoint4()' */
void CoreQuadView_OnSetPoint4( CoreQuadView _this, XPoint value )
{
  if ( !EwCompPoint( value, _this->Point4 ))
    return;

  if (( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super1.Owner, CoreView__GetExtent( _this ));

  _this->Super1.layoutContext = 0;
  _this->Point4 = value;

  if (( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super1.Owner, CoreView__GetExtent( _this ));

  if ((( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateEmbedded ) 
      == CoreViewStateEmbedded )) && !(( _this->Super1.Owner->Super2.viewState & 
      CoreViewStateUpdatingLayout ) == CoreViewStateUpdatingLayout ))
  {
    _this->Super1.viewState = _this->Super1.viewState | CoreViewStateRequestLayout;
    _this->Super1.Owner->Super2.viewState = _this->Super1.Owner->Super2.viewState 
    | CoreViewStatePendingLayout;
    EwPostSignal( EwNewSlot( _this->Super1.Owner, CoreGroup_updateComponent ), ((XObject)_this ));
  }
}

/* 'C' function for method : 'Core::QuadView.OnSetPoint3()' */
void CoreQuadView_OnSetPoint3( CoreQuadView _this, XPoint value )
{
  if ( !EwCompPoint( value, _this->Point3 ))
    return;

  if (( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super1.Owner, CoreView__GetExtent( _this ));

  _this->Super1.layoutContext = 0;
  _this->Point3 = value;

  if (( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super1.Owner, CoreView__GetExtent( _this ));

  if ((( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateEmbedded ) 
      == CoreViewStateEmbedded )) && !(( _this->Super1.Owner->Super2.viewState & 
      CoreViewStateUpdatingLayout ) == CoreViewStateUpdatingLayout ))
  {
    _this->Super1.viewState = _this->Super1.viewState | CoreViewStateRequestLayout;
    _this->Super1.Owner->Super2.viewState = _this->Super1.Owner->Super2.viewState 
    | CoreViewStatePendingLayout;
    EwPostSignal( EwNewSlot( _this->Super1.Owner, CoreGroup_updateComponent ), ((XObject)_this ));
  }
}

/* 'C' function for method : 'Core::QuadView.OnSetPoint2()' */
void CoreQuadView_OnSetPoint2( CoreQuadView _this, XPoint value )
{
  if ( !EwCompPoint( value, _this->Point2 ))
    return;

  if (( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super1.Owner, CoreView__GetExtent( _this ));

  _this->Super1.layoutContext = 0;
  _this->Point2 = value;

  if (( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super1.Owner, CoreView__GetExtent( _this ));

  if ((( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateEmbedded ) 
      == CoreViewStateEmbedded )) && !(( _this->Super1.Owner->Super2.viewState & 
      CoreViewStateUpdatingLayout ) == CoreViewStateUpdatingLayout ))
  {
    _this->Super1.viewState = _this->Super1.viewState | CoreViewStateRequestLayout;
    _this->Super1.Owner->Super2.viewState = _this->Super1.Owner->Super2.viewState 
    | CoreViewStatePendingLayout;
    EwPostSignal( EwNewSlot( _this->Super1.Owner, CoreGroup_updateComponent ), ((XObject)_this ));
  }
}

/* 'C' function for method : 'Core::QuadView.OnSetPoint1()' */
void CoreQuadView_OnSetPoint1( CoreQuadView _this, XPoint value )
{
  if ( !EwCompPoint( value, _this->Point1 ))
    return;

  if (( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super1.Owner, CoreView__GetExtent( _this ));

  _this->Super1.layoutContext = 0;
  _this->Point1 = value;

  if (( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super1.Owner, CoreView__GetExtent( _this ));

  if ((( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateEmbedded ) 
      == CoreViewStateEmbedded )) && !(( _this->Super1.Owner->Super2.viewState & 
      CoreViewStateUpdatingLayout ) == CoreViewStateUpdatingLayout ))
  {
    _this->Super1.viewState = _this->Super1.viewState | CoreViewStateRequestLayout;
    _this->Super1.Owner->Super2.viewState = _this->Super1.Owner->Super2.viewState 
    | CoreViewStatePendingLayout;
    EwPostSignal( EwNewSlot( _this->Super1.Owner, CoreGroup_updateComponent ), ((XObject)_this ));
  }
}

/* The method IsPointInside() verifies whether the specified position aPoint lies 
   within the quad and returns 'true' if this is the case. If aPoint lies outside 
   the quad, the method returns 'false'. */
XBool CoreQuadView_IsPointInside( CoreQuadView _this, XPoint aPoint )
{
  XPoint points[ 4 ];
  XInt32 i = 0;
  XInt32 j = 3;
  XBool inside1 = 0;
  XBool inside2 = 0;

  points[ 0 ] = _this->Point1;
  points[ 1 ] = _this->Point2;
  points[ 2 ] = _this->Point3;
  points[ 3 ] = _this->Point4;

  while ( i < 4 )
  {
    XInt32 vert1X = points[ EwCheckIndex( i, 4 )].X;
    XInt32 vert1Y = points[ EwCheckIndex( i, 4 )].Y;
    XInt32 vert2X = points[ EwCheckIndex( j, 4 )].X;
    XInt32 vert2Y = points[ EwCheckIndex( j, 4 )].Y;

    if ((( vert1Y > aPoint.Y ) != ( vert2Y > aPoint.Y )) || (( vert1Y < aPoint.Y ) 
        != ( vert2Y < aPoint.Y )))
    {
      XInt32 x = ((( vert2X - vert1X ) * ( aPoint.Y - vert1Y )) / ( vert2Y - vert1Y )) 
        + vert1X;

      if ( aPoint.X > x )
        inside1 = (XBool)!inside1;

      if ( aPoint.X < x )
        inside2 = (XBool)!inside2;
    }

    j = i;
    i = i + 1;
  }

  return (XBool)( inside1 || inside2 );
}

/* The method HasRectShape() evaluates the shape of the quad and returns 'true' 
   if the quad has the shape of a rectangle. Otherwise 'false' is returned. */
XBool CoreQuadView_HasRectShape( CoreQuadView _this )
{
  return (XBool)((((( _this->Point1.X == _this->Point4.X ) && ( _this->Point2.X 
    == _this->Point3.X )) && ( _this->Point1.Y == _this->Point2.Y )) && ( _this->Point3.Y 
    == _this->Point4.Y )) || (((( _this->Point1.X == _this->Point2.X ) && ( _this->Point3.X 
    == _this->Point4.X )) && ( _this->Point1.Y == _this->Point4.Y )) && ( _this->Point2.Y 
    == _this->Point3.Y )));
}

/* Variants derived from the class : 'Core::QuadView' */
EW_DEFINE_CLASS_VARIANTS( CoreQuadView )
EW_END_OF_CLASS_VARIANTS( CoreQuadView )

/* Virtual Method Table (VMT) for the class : 'Core::QuadView' */
EW_DEFINE_CLASS( CoreQuadView, CoreView, _None, _None, _None, _None, _None, "Core::QuadView" )
  CoreQuadView_initLayoutContext,
  CoreView_GetRoot,
  CoreView_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreQuadView_ArrangeView,
  CoreQuadView_MoveView,
  CoreQuadView_GetExtent,
  CoreView_ChangeViewState,
EW_END_OF_CLASS( CoreQuadView )

/* Initializer for the class 'Core::RectView' */
void CoreRectView__Init( CoreRectView _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreView__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreRectView );

  /* ... and initialize objects, variables, properties, etc. */
}

/* Re-Initializer for the class 'Core::RectView' */
void CoreRectView__ReInit( CoreRectView _this )
{
  /* At first re-initialize the super class ... */
  CoreView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::RectView' */
void CoreRectView__Done( CoreRectView _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreRectView );

  /* Don't forget to deinitialize the super class ... */
  CoreView__Done( &_this->_Super );
}

/* 'C' function for method : 'Core::RectView.initLayoutContext()' */
void CoreRectView_initLayoutContext( CoreRectView _this, XRect aBounds, CoreOutline 
  aOutline )
{
  CoreLayoutContext context = EwNewObject( CoreLayoutContext, 0 );

  context->extent = _this->Bounds;
  context->bounds = aBounds;
  context->outline = aOutline;
  _this->Super1.layoutContext = context;
}

/* The method ArrangeView() is invoked automatically if the superior @Owner group 
   needs to re-arrange its views. For example, the changing of the owners size can 
   cause the enclosed views to adapt their position and size, so all views still 
   fit within the new owners area. This method provides the core functionality for 
   the automatic GUI layout mechanism.
   The arrangement is controlled primarily by the @Layout property of the view. 
   It specifies a set of alignment constraints and determines whether the view can 
   change its size. The bounds area where the view should be arranged, is passed 
   in the parameter aBounds. This is usually the current area of the views owner.
   The parameter aFormation defines the desired arrangement mode. Depending on the 
   value of this parameter, the views can be arranged in rows or columns. If aFormation 
   == Core::Formation.None, no automatic row/column arrangement is performed and 
   the view is moved and scaled only to fit inside the aBounds area.
   ArrangeView() is invoked automatically by the framework, so you never should 
   need to invoke it directly.
   The method returns the size of the view after it has been arranged. */
XPoint CoreRectView_ArrangeView( CoreRectView _this, XRect aBounds, XEnum aFormation )
{
  XSet layout = _this->Super1.Layout;
  CoreLayoutContext context = _this->Super1.layoutContext;
  XInt32 x1 = context->extent.Point1.X;
  XInt32 y1 = context->extent.Point1.Y;
  XInt32 x2 = context->extent.Point2.X;
  XInt32 y2 = context->extent.Point2.Y;
  XPoint dstSize = EwGetRectSize( aBounds );
  XInt32 w = x2 - x1;
  XInt32 h = y2 - y1;

  if ( aFormation == CoreFormationNone )
  {
    XPoint srcSize = EwGetRectSize( context->bounds );
    x1 = x1 - context->bounds.Point1.X;
    y1 = y1 - context->bounds.Point1.Y;

    if ( srcSize.X != dstSize.X )
    {
      XBool alignToLeft = (( layout & CoreLayoutAlignToLeft ) == CoreLayoutAlignToLeft );
      XBool alignToRight = (( layout & CoreLayoutAlignToRight ) == CoreLayoutAlignToRight );
      XBool resizeHorz = (( layout & CoreLayoutResizeHorz ) == CoreLayoutResizeHorz );

      if ( !alignToLeft && ( resizeHorz || !alignToRight ))
        x1 = ( x1 * dstSize.X ) / srcSize.X;

      if ( !alignToRight && ( resizeHorz || !alignToLeft ))
      {
        x2 = x2 - context->bounds.Point1.X;
        x2 = ( x2 * dstSize.X ) / srcSize.X;
        x2 = x2 - dstSize.X;
      }
      else
        x2 = x2 - context->bounds.Point2.X;

      x1 = x1 + aBounds.Point1.X;
      x2 = x2 + aBounds.Point2.X;

      if ( !resizeHorz )
      {
        if ( alignToLeft && !alignToRight )
          x2 = x1 + w;
        else
          if ( !alignToLeft && alignToRight )
            x1 = x2 - w;
          else
          {
            x1 = x1 + ((( x2 - x1 ) - w ) / 2 );
            x2 = x1 + w;
          }
      }
    }
    else
    {
      x2 = x2 - context->bounds.Point2.X;
      x1 = x1 + aBounds.Point1.X;
      x2 = x2 + aBounds.Point2.X;
    }

    if ( srcSize.Y != dstSize.Y )
    {
      XBool alignToTop = (( layout & CoreLayoutAlignToTop ) == CoreLayoutAlignToTop );
      XBool alignToBottom = (( layout & CoreLayoutAlignToBottom ) == CoreLayoutAlignToBottom );
      XBool resizeVert = (( layout & CoreLayoutResizeVert ) == CoreLayoutResizeVert );

      if ( !alignToTop && ( resizeVert || !alignToBottom ))
        y1 = ( y1 * dstSize.Y ) / srcSize.Y;

      if ( !alignToBottom && ( resizeVert || !alignToTop ))
      {
        y2 = y2 - context->bounds.Point1.Y;
        y2 = ( y2 * dstSize.Y ) / srcSize.Y;
        y2 = y2 - dstSize.Y;
      }
      else
        y2 = y2 - context->bounds.Point2.Y;

      y1 = y1 + aBounds.Point1.Y;
      y2 = y2 + aBounds.Point2.Y;

      if ( !resizeVert )
      {
        if ( alignToTop && !alignToBottom )
          y2 = y1 + h;
        else
          if ( !alignToTop && alignToBottom )
            y1 = y2 - h;
          else
          {
            y1 = y1 + ((( y2 - y1 ) - h ) / 2 );
            y2 = y1 + h;
          }
      }
    }
    else
    {
      y2 = y2 - context->bounds.Point2.Y;
      y1 = y1 + aBounds.Point1.Y;
      y2 = y2 + aBounds.Point2.Y;
    }
  }
  else
  {
    switch ( aFormation )
    {
      case CoreFormationLeftToRight :
      {
        x1 = aBounds.Point1.X;
        x2 = x1 + w;
      }
      break;

      case CoreFormationRightToLeft :
      {
        x2 = aBounds.Point2.X;
        x1 = x2 - w;
      }
      break;

      case CoreFormationTopToBottom :
      {
        y1 = aBounds.Point1.Y;
        y2 = y1 + h;
      }
      break;

      case CoreFormationBottomToTop :
      {
        y2 = aBounds.Point2.Y;
        y1 = y2 - h;
      }
      break;

      default : 
        ;
    }

    if (( aFormation == CoreFormationLeftToRight ) || ( aFormation == CoreFormationRightToLeft ))
    {
      XBool alignToTop = (( layout & CoreLayoutAlignToTop ) == CoreLayoutAlignToTop );
      XBool alignToBottom = (( layout & CoreLayoutAlignToBottom ) == CoreLayoutAlignToBottom );
      XBool resizeVert = (( layout & CoreLayoutResizeVert ) == CoreLayoutResizeVert );

      if ( resizeVert )
      {
        y1 = aBounds.Point1.Y;
        y2 = aBounds.Point2.Y;
      }
      else
        if ( alignToTop && !alignToBottom )
        {
          y1 = aBounds.Point1.Y;
          y2 = y1 + h;
        }
        else
          if ( alignToBottom && !alignToTop )
          {
            y2 = aBounds.Point2.Y;
            y1 = y2 - h;
          }
          else
          {
            y1 = aBounds.Point1.Y + (( EwGetRectH( aBounds ) - h ) / 2 );
            y2 = y1 + h;
          }
    }

    if (( aFormation == CoreFormationTopToBottom ) || ( aFormation == CoreFormationBottomToTop ))
    {
      XBool alignToLeft = (( layout & CoreLayoutAlignToLeft ) == CoreLayoutAlignToLeft );
      XBool alignToRight = (( layout & CoreLayoutAlignToRight ) == CoreLayoutAlignToRight );
      XBool resizeHorz = (( layout & CoreLayoutResizeHorz ) == CoreLayoutResizeHorz );

      if ( resizeHorz )
      {
        x1 = aBounds.Point1.X;
        x2 = aBounds.Point2.X;
      }
      else
        if ( alignToLeft && !alignToRight )
        {
          x1 = aBounds.Point1.X;
          x2 = x1 + w;
        }
        else
          if ( alignToRight && !alignToLeft )
          {
            x2 = aBounds.Point2.X;
            x1 = x2 - w;
          }
          else
          {
            x1 = aBounds.Point1.X + (( EwGetRectW( aBounds ) - w ) / 2 );
            x2 = x1 + w;
          }
    }
  }

  context->isEmpty = (XBool)(( x1 >= x2 ) || ( y1 >= y2 ));

  if ((( _this->Super1.viewState & CoreViewStateFastReshape ) == CoreViewStateFastReshape ))
  {
    _this->Bounds = EwNewRect( x1, y1, x2, y2 );
  }
  else
  {
    CoreRectView__OnSetBounds( _this, EwNewRect( x1, y1, x2, y2 ));
    _this->Super1.layoutContext = context;
  }

  return EwNewPoint( x2 - x1, y2 - y1 );
}

/* The method MoveView() changes the position of the view by adding the value aOffset 
   to all corners of the view. For example, in case of a line view the value is 
   added to the both line end points.
   The parameter aFastMove serves for the optimization purpose. If it is 'true', 
   the corners are modified without performing any updates of the view and without 
   redrawing the screen. This is useful, when you wish to move or arrange a lot 
   of views at once. In this case it's up to you to request the finally screen update. 
   To do this you can use the method InvalidateArea() of the views @Owner.
   In the case aFastMove == false, the operation automatically requests the screen 
   redraw of the view areas before and after the movement. You don't need to take 
   care about it. */
void CoreRectView_MoveView( CoreRectView _this, XPoint aOffset, XBool aFastMove )
{
  if ( aFastMove )
    _this->Bounds = EwMoveRectPos( _this->Bounds, aOffset );
  else
    CoreRectView__OnSetBounds( _this, EwMoveRectPos( _this->Bounds, aOffset ));
}

/* The method GetExtent() returns the position and the size of the view relative 
   to the origin of its @Owner. In case of views with a non rectangular shape the 
   method returns the rectangular boundary area enclosing the entire shape. */
XRect CoreRectView_GetExtent( CoreRectView _this )
{
  return _this->Bounds;
}

/* 'C' function for method : 'Core::RectView.OnSetBounds()' */
void CoreRectView_OnSetBounds( CoreRectView _this, XRect value )
{
  if ( !EwCompRect( value, _this->Bounds ))
    return;

  if (( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super1.Owner, _this->Bounds );

  _this->Super1.layoutContext = 0;
  _this->Bounds = value;

  if (( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this->Super1.Owner, _this->Bounds );

  if ((( _this->Super1.Owner != 0 ) && (( _this->Super1.viewState & CoreViewStateEmbedded ) 
      == CoreViewStateEmbedded )) && !(( _this->Super1.Owner->Super2.viewState & 
      CoreViewStateUpdatingLayout ) == CoreViewStateUpdatingLayout ))
  {
    _this->Super1.viewState = _this->Super1.viewState | CoreViewStateRequestLayout;
    _this->Super1.Owner->Super2.viewState = _this->Super1.Owner->Super2.viewState 
    | CoreViewStatePendingLayout;
    EwPostSignal( EwNewSlot( _this->Super1.Owner, CoreGroup_updateComponent ), ((XObject)_this ));
  }
}

/* Wrapper function for the virtual method : 'Core::RectView.OnSetBounds()' */
void CoreRectView__OnSetBounds( void* _this, XRect value )
{
  ((CoreRectView)_this)->_VMT->OnSetBounds((CoreRectView)_this, value );
}

/* Variants derived from the class : 'Core::RectView' */
EW_DEFINE_CLASS_VARIANTS( CoreRectView )
EW_END_OF_CLASS_VARIANTS( CoreRectView )

/* Virtual Method Table (VMT) for the class : 'Core::RectView' */
EW_DEFINE_CLASS( CoreRectView, CoreView, _None, _None, _None, _None, _None, "Core::RectView" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreView_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreView_ChangeViewState,
  CoreRectView_OnSetBounds,
EW_END_OF_CLASS( CoreRectView )

/* Initializer for the class 'Core::Group' */
void CoreGroup__Init( CoreGroup _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreRectView__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreGroup );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Super2.viewState = CoreViewStateAlphaBlended | CoreViewStateEnabled | CoreViewStateFocusable 
  | CoreViewStateTouchable | CoreViewStateVisible;
  _this->Opacity = 255;

  /* Call the user defined constructor */
  CoreGroup_Init( _this, aArg );
}

/* Re-Initializer for the class 'Core::Group' */
void CoreGroup__ReInit( CoreGroup _this )
{
  /* At first re-initialize the super class ... */
  CoreRectView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::Group' */
void CoreGroup__Done( CoreGroup _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreGroup );

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
void CoreGroup_Draw( CoreGroup _this, GraphicsCanvas aCanvas, XRect aClip, XPoint 
  aOffset, XInt32 aOpacity, XBool aBlend )
{
  aOpacity = (( aOpacity + 1 ) * _this->Opacity ) >> 8;
  aBlend = (XBool)( aBlend && (( _this->Super2.viewState & CoreViewStateAlphaBlended ) 
  == CoreViewStateAlphaBlended ));

  if ( _this->buffer == 0 )
    CoreGroup_drawContent( _this, aCanvas, aClip, EwMovePointPos( aOffset, _this->Super1.Bounds.Point1 ), 
    aOpacity, aBlend );
  else
  {
    XColor c = EwNewColor( 255, 255, 255, (XUInt8)aOpacity );
    ResourcesBitmap__Update( _this->buffer );
    GraphicsCanvas_CopyBitmap( aCanvas, aClip, ((ResourcesBitmap)_this->buffer ), 
    0, EwMoveRectPos( _this->Super1.Bounds, aOffset ), _Const0000, c, c, c, c, aBlend );
  }
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
CoreCursorHit CoreGroup_CursorHitTest( CoreGroup _this, XRect aArea, XInt32 aFinger, 
  XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
{
  CoreView view = _this->last;
  CoreCursorHit found = 0;
  XRect area = _Const0001;
  CoreView form = 0;
  XBool lock = (XBool)(( _this->fadersQueue != 0 ) && (( _this->fadersQueue->current 
    != 0 ) || ( _this->fadersQueue->first != 0 )));

  if ( EwIsRectEmpty( EwIntersectRect( aArea, _this->Super1.Bounds )))
    return 0;

  aArea = EwMoveRectNeg( aArea, _this->Super1.Bounds.Point1 );

  while ( view != 0 )
  {
    if ((( view->viewState & CoreViewStateEmbedded ) == CoreViewStateEmbedded ) 
        && ( form == 0 ))
    {
      form = view->prev;

      while (( form != 0 ) && !(( form->viewState & CoreViewStateIsOutline ) == 
             CoreViewStateIsOutline ))
        form = form->prev;

      if ( form != 0 )
        area = EwIntersectRect( aArea, CoreView__GetExtent( form ));
      else
        area = _Const0001;
    }

    if ( form == view )
    {
      form = 0;
      area = _Const0001;
    }

    if (((((( view->viewState & CoreViewStateTouchable ) == CoreViewStateTouchable ) 
        && (( view->viewState & CoreViewStateEnabled ) == CoreViewStateEnabled )) 
        && !(( view->viewState & CoreViewStateRunningFader ) == CoreViewStateRunningFader )) 
        && !(( view->viewState & CoreViewStatePendingFader ) == CoreViewStatePendingFader )) 
        && ( !(( view->viewState & CoreViewStateDialog ) == CoreViewStateDialog ) 
        || (((CoreView)_this->dialogStack->group == view ) && !lock )))
    {
      XRect extent = CoreView__GetExtent( view );
      CoreView dedicatedView = aDedicatedView;
      CoreCursorHit cursorHit = 0;

      if ( dedicatedView == view )
        dedicatedView = 0;

      if ((( view->viewState & CoreViewStateEmbedded ) == CoreViewStateEmbedded ))
      {
        if ( !EwIsRectEmpty( EwIntersectRect( extent, area )))
          cursorHit = CoreView__CursorHitTest( view, area, aFinger, aStrikeCount, 
          dedicatedView, aRetargetReason );
      }
      else
      {
        if ( !EwIsRectEmpty( EwIntersectRect( extent, aArea )) || ( aDedicatedView 
            == view ))
          cursorHit = CoreView__CursorHitTest( view, aArea, aFinger, aStrikeCount, 
          dedicatedView, aRetargetReason );
      }

      view = view->prev;

      if ( cursorHit != 0 )
      {
        if (( found == 0 ) || (( cursorHit->Deviation < found->Deviation ) && ( 
            cursorHit->Deviation >= 0 )))
          found = cursorHit;

        if ( cursorHit->Deviation == 0 )
          view = 0;
      }
    }
    else
      view = view->prev;
  }

  return found;
}

/* The method ChangeViewState() modifies the current state of the view. The state 
   is a set of switches determining whether a view is visible, whether it can react 
   to user inputs or whether it is just performing some update operations, etc.
   The modification is controlled by the the both parameters. The first aSetState 
   contains the switches to activate within the view state. The second aClearState 
   determines all switches to disable.
   Depending on the state alteration the method will perform different operations, 
   e.g. in response to the clearing of the visible state, the method will request 
   a screen redraw to make disappear the view from the screen.
   ChangeViewState() is invoked automatically by the framework, so you never should 
   need to invoke it directly. All relevant states are available as properties e.g. 
   the property Visible in derived classes reflects the visible state of the view. */
void CoreGroup_ChangeViewState( CoreGroup _this, XSet aSetState, XSet aClearState )
{
  XSet oldState = _this->Super2.viewState;
  XSet deltaState;

  CoreView_ChangeViewState((CoreView)_this, aSetState, aClearState );
  deltaState = _this->Super2.viewState ^ oldState;

  if (( _this->Focus != 0 ) && (( deltaState & CoreViewStateFocused ) == CoreViewStateFocused ))
  {
    if ((( _this->Super2.viewState & CoreViewStateFocused ) == CoreViewStateFocused ))
      CoreView__ChangeViewState( _this->Focus, CoreViewStateFocused, 0 );
    else
      CoreView__ChangeViewState( _this->Focus, 0, CoreViewStateFocused );
  }

  if (( _this->dialogStack != 0 ) && (( deltaState & CoreViewStateFocused ) == CoreViewStateFocused ))
  {
    if ((( _this->Super2.viewState & CoreViewStateFocused ) == CoreViewStateFocused ) 
        && (( _this->dialogStack->group->Super2.viewState & ( CoreViewStateEnabled 
        | CoreViewStateFocusable )) == ( CoreViewStateEnabled | CoreViewStateFocusable )))
      CoreView__ChangeViewState( _this->dialogStack->group, CoreViewStateFocused, 
      0 );
    else
      CoreView__ChangeViewState( _this->dialogStack->group, 0, CoreViewStateFocused );
  }

  if ( !!deltaState )
  {
    _this->Super2.viewState = _this->Super2.viewState | CoreViewStatePendingViewState;
    EwPostSignal( EwNewSlot( _this, CoreGroup_updateComponent ), ((XObject)_this ));
  }
}

/* 'C' function for method : 'Core::Group.OnSetBounds()' */
void CoreGroup_OnSetBounds( CoreGroup _this, XRect value )
{
  XPoint oldSize;
  XPoint newSize;
  XBool resize;

  if ( !EwCompRect( value, _this->Super1.Bounds ))
    return;

  oldSize = EwGetRectSize( _this->Super1.Bounds );
  newSize = EwGetRectSize( value );
  resize = (XBool)EwCompPoint( oldSize, newSize );

  if ( resize && ( _this->buffer != 0 ))
  {
    ResourcesBitmap__OnSetFrameSize( _this->buffer, newSize );
    EwNotifyObjObservers((XObject)_this, 0 );
    EwNotifyObjObservers((XObject)_this->buffer, 0 );
  }

  CoreRectView_OnSetBounds((CoreRectView)_this, value );

  if (( resize && ( oldSize.X > 0 )) && ( oldSize.Y > 0 ))
  {
    XRect bounds = EwNewRect2Point( _Const0000, oldSize );
    CoreView view = _this->first;
    XSet fixed = CoreLayoutAlignToLeft | CoreLayoutAlignToTop;

    while ( view != 0 )
    {
      if ((( view->layoutContext == 0 ) && ( view->Layout != fixed )) && !(( view->viewState 
          & CoreViewStateEmbedded ) == CoreViewStateEmbedded ))
        CoreView__initLayoutContext( view, bounds, 0 );

      view = view->next;
    }
  }

  if ( resize )
  {
    _this->Super2.viewState = _this->Super2.viewState | ( CoreViewStatePendingLayout 
    | CoreViewStateUpdateLayout );
    EwPostSignal( EwNewSlot( _this, CoreGroup_updateComponent ), ((XObject)_this ));
  }
}

/* 'C' function for method : 'Core::Group.processKeyHandlers()' */
XObject CoreGroup_processKeyHandlers( CoreGroup _this, CoreEvent aEvent )
{
  CoreKeyEvent keyEvent = EwCastObject( aEvent, CoreKeyEvent );
  CoreKeyPressHandler handler = _this->keyHandlers;

  if ( keyEvent == 0 )
    return 0;

  while (( handler != 0 ) && ( !handler->Enabled || !CoreKeyPressHandler_HandleEvent( 
         handler, keyEvent )))
    handler = handler->next;

  return ((XObject)handler );
}

/* 'C' function for method : 'Core::Group.drawContent()' */
void CoreGroup_drawContent( CoreGroup _this, GraphicsCanvas aCanvas, XRect aClip, 
  XPoint aOffset, XInt32 aOpacity, XBool aBlend )
{
  CoreView view = _this->first;
  XRect clip = _Const0001;
  XBool outlineVisible = 1;

  while ( view != 0 )
  {
    if ((( view->viewState & CoreViewStateIsOutline ) == CoreViewStateIsOutline ))
    {
      CoreOutline outlineView = EwCastObject( view, CoreOutline );
      clip = EwIntersectRect( aClip, EwMoveRectPos( outlineView->Super1.Bounds, 
      aOffset ));
      outlineVisible = (( outlineView->Super2.viewState & CoreViewStateVisible ) 
      == CoreViewStateVisible );
    }

    if ((( view->viewState & CoreViewStateVisible ) == CoreViewStateVisible ))
    {
      if ((( view->viewState & CoreViewStateEmbedded ) == CoreViewStateEmbedded ))
      {
        if ( outlineVisible )
        {
          XRect area = EwIntersectRect( EwMoveRectPos( CoreView__GetExtent( view ), 
            aOffset ), clip );

          if ( !EwIsRectEmpty( area ))
            CoreView__Draw( view, aCanvas, area, aOffset, aOpacity, aBlend );
        }
      }
      else
      {
        XRect area = EwIntersectRect( EwMoveRectPos( CoreView__GetExtent( view ), 
          aOffset ), aClip );

        if ( !EwIsRectEmpty( area ))
          CoreView__Draw( view, aCanvas, area, aOffset, aOpacity, aBlend );
      }
    }

    view = view->next;
  }
}

/* 'C' function for method : 'Core::Group.recalculateLayout()' */
void CoreGroup_recalculateLayout( CoreGroup _this )
{
  XBool groupLayout = (( _this->Super2.viewState & CoreViewStateUpdateLayout ) == 
    CoreViewStateUpdateLayout );
  XRect groupBounds = EwGetRectORect( _this->Super1.Bounds );
  XBool formLayout = 0;
  XRect formBounds = _Const0001;
  XPoint formOffset = _Const0000;
  XInt32 formSpace = 0;
  XEnum formMode = CoreFormationNone;
  CoreView view = _this->last;
  CoreOutline form = 0;
  XSet fixed = CoreLayoutAlignToLeft | CoreLayoutAlignToTop;

  while ( view != 0 )
  {
    if ((( view->viewState & CoreViewStateRequestLayout ) == CoreViewStateRequestLayout ))
    {
      formLayout = 1;
      view->viewState = view->viewState & ~CoreViewStateRequestLayout;
    }

    if ( formLayout && (( view->viewState & CoreViewStateIsOutline ) == CoreViewStateIsOutline ))
    {
      formLayout = 0;

      if ( EwCastObject( view, CoreOutline )->Formation != CoreFormationNone )
        view->viewState = view->viewState | CoreViewStateUpdateLayout;
    }

    view = view->prev;
  }

  formLayout = 0;
  view = _this->first;

  if ( groupLayout )
  {
    _this->Super2.viewState = _this->Super2.viewState & ~CoreViewStateUpdateLayout;
    groupLayout = (XBool)!EwIsRectEmpty( groupBounds );
  }

  _this->Super2.viewState = _this->Super2.viewState | CoreViewStateUpdatingLayout;

  while ( view != 0 )
  {
    if ((( view->viewState & CoreViewStateEmbedded ) == CoreViewStateEmbedded ))
    {
      if (( view->layoutContext != 0 ) && ( view->layoutContext->outline != form ))
        view->layoutContext = 0;

      if ((( view->layoutContext == 0 ) && formLayout ) && (( view->Layout != fixed ) 
          || ( formMode != CoreFormationNone )))
        CoreView__initLayoutContext( view, formBounds, form );
    }

    if ( view->layoutContext != 0 )
    {
      if ( groupLayout && !(( view->viewState & CoreViewStateEmbedded ) == CoreViewStateEmbedded ))
        CoreView__ArrangeView( view, groupBounds, CoreFormationNone );

      if ( formLayout && (( view->viewState & CoreViewStateEmbedded ) == CoreViewStateEmbedded ))
      {
        XPoint size = CoreView__ArrangeView( view, EwMoveRectPos( formBounds, formOffset ), 
          formMode );

        if ((( view->viewState & CoreViewStateVisible ) == CoreViewStateVisible ))
        {
          XPoint ofs = _Const0000;

          switch ( formMode )
          {
            case CoreFormationLeftToRight :
              ofs.X = ( size.X + formSpace );
            break;

            case CoreFormationRightToLeft :
              ofs.X = ( -size.X - formSpace );
            break;

            case CoreFormationTopToBottom :
              ofs.Y = ( size.Y + formSpace );
            break;

            case CoreFormationBottomToTop :
              ofs.Y = ( -size.Y - formSpace );
            break;

            default : 
              ;
          }

          formOffset = EwMovePointPos( formOffset, ofs );
        }
      }
    }

    if ((( view->viewState & CoreViewStateIsOutline ) == CoreViewStateIsOutline ))
    {
      formLayout = (( view->viewState & CoreViewStateUpdateLayout ) == CoreViewStateUpdateLayout );
      form = EwCastObject( view, CoreOutline );

      if ( formLayout )
      {
        view->viewState = view->viewState & ~CoreViewStateUpdateLayout;
        formBounds = EwMoveRectPos( form->Super1.Bounds, form->ScrollOffset );
        formOffset = _Const0000;
        formMode = form->Formation;
        formSpace = form->Space;
        formLayout = (XBool)!EwIsRectEmpty( formBounds );
      }

      if ( formLayout )
      {
        CoreGroup__InvalidateArea( _this, form->Super1.Bounds );
      }
    }

    view = view->next;
  }

  _this->Super2.viewState = _this->Super2.viewState & ~CoreViewStateUpdatingLayout;
  CoreGroup_UpdateLayout( _this, EwGetRectSize( groupBounds ));
}

/* 'C' function for method : 'Core::Group.updateComponent()' */
void CoreGroup_updateComponent( CoreGroup _this, XObject sender )
{
  XBool updateLayout;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  updateLayout = (( _this->Super2.viewState & CoreViewStateUpdateLayout ) == CoreViewStateUpdateLayout );

  if ((( _this->Super2.viewState & CoreViewStatePendingLayout ) == CoreViewStatePendingLayout ))
  {
    _this->Super2.viewState = _this->Super2.viewState & ~CoreViewStatePendingLayout;
    CoreGroup_recalculateLayout( _this );
  }

  if ((( _this->Super2.viewState & CoreViewStatePendingViewState ) == CoreViewStatePendingViewState ) 
      || updateLayout )
  {
    _this->Super2.viewState = _this->Super2.viewState & ~CoreViewStatePendingViewState;
    CoreGroup__UpdateViewState( _this, _this->Super2.viewState );
  }
}

/* 'C' function for method : 'Core::Group.OnSetFocus()' */
void CoreGroup_OnSetFocus( CoreGroup _this, CoreView value )
{
  if (( value != 0 ) && ( value->Owner != _this ))
  {
    EwThrow( EwLoadString( &_Const0002 ));
    return;
  }

  if (( value != 0 ) && !(( value->viewState & ( CoreViewStateEnabled | CoreViewStateFocusable )) 
      == ( CoreViewStateEnabled | CoreViewStateFocusable )))
    value = 0;

  if (( value != 0 ) && (( value->viewState & CoreViewStateDialog ) == CoreViewStateDialog ))
    value = 0;

  if ( value == _this->Focus )
    return;

  if ( _this->Focus != 0 )
    CoreView__ChangeViewState( _this->Focus, 0, CoreViewStateFocused | CoreViewStateSelected );

  _this->Focus = value;

  if ( value != 0 )
  {
    if ((( _this->Super2.viewState & CoreViewStateFocused ) == CoreViewStateFocused ))
      CoreView__ChangeViewState( value, CoreViewStateFocused | CoreViewStateSelected, 
      0 );
    else
      CoreView__ChangeViewState( value, CoreViewStateSelected, 0 );
  }
}

/* Wrapper function for the virtual method : 'Core::Group.OnSetFocus()' */
void CoreGroup__OnSetFocus( void* _this, CoreView value )
{
  ((CoreGroup)_this)->_VMT->OnSetFocus((CoreGroup)_this, value );
}

/* The method LocalPosition() converts the given position aPoint from the screen 
   coordinate space to the coordinate space of this component and returns the calculated 
   position. In the case the component isn't visible within the application, the 
   method can fail and return a wrong position. */
XPoint CoreGroup_LocalPosition( CoreGroup _this, XPoint aPoint )
{
  CoreGroup tmp = _this;

  while ( tmp != 0 )
  {
    aPoint = EwMovePointNeg( aPoint, tmp->Super1.Bounds.Point1 );
    tmp = tmp->Super2.Owner;
  }

  return aPoint;
}

/* The method DispatchEvent() feeds the component with the event passed in the parameter 
   aEvent and propagates it along the so-called focus path. This focus path leads 
   to the currently selected keyboard event receiver view. If the event is rejected 
   by the view, the same operation is repeated for the next superior view. This 
   permits the hierarchical event dispatching until a willing view has handled the 
   event or all views in the focus path have been evaluated. If the event remains 
   still unhandled, it will be passed to the component itself.
   The focus path is established by the property @Focus. 
   DispatchEvent() returns the value returned by the @HandleEvent() method of the 
   view which has handled the event. In the case, the event was not handled, the 
   method returns 'null'. */
XObject CoreGroup_DispatchEvent( CoreGroup _this, CoreEvent aEvent )
{
  CoreView view = _this->Focus;
  CoreGroup grp = EwCastObject( view, CoreGroup );
  XObject handled = 0;
  XBool lock = (XBool)(( _this->fadersQueue != 0 ) && (( _this->fadersQueue->current 
    != 0 ) || ( _this->fadersQueue->first != 0 )));

  if (( view != 0 ) && (((( view->viewState & CoreViewStateDialog ) == CoreViewStateDialog ) 
      || (( view->viewState & CoreViewStateRunningFader ) == CoreViewStateRunningFader )) 
      || (( view->viewState & CoreViewStatePendingFader ) == CoreViewStatePendingFader )))
  {
    view = 0;
    grp = 0;
  }

  if (( _this->dialogStack != 0 ) && !lock )
    handled = CoreGroup__DispatchEvent( _this->dialogStack->group, aEvent );

  if (( handled == 0 ) && ( grp != 0 ))
    handled = CoreGroup__DispatchEvent( grp, aEvent );
  else
    if (( handled == 0 ) && ( view != 0 ))
      handled = CoreView__HandleEvent( view, aEvent );

  if ( handled == 0 )
    handled = CoreView__HandleEvent( _this, aEvent );

  if ( handled == 0 )
    handled = CoreGroup_processKeyHandlers( _this, aEvent );

  return handled;
}

/* Wrapper function for the virtual method : 'Core::Group.DispatchEvent()' */
XObject CoreGroup__DispatchEvent( void* _this, CoreEvent aEvent )
{
  return ((CoreGroup)_this)->_VMT->DispatchEvent((CoreGroup)_this, aEvent );
}

/* The method BroadcastEventAtPosition() feeds the component with the event passed 
   in the parameter aEvent and propagates it to all views, which do exist within 
   the component and do enclose the given position aPosition until the event has 
   been handled or all affected views are evaluated. If the event remains still 
   unhandled, it will be passed to the component itself.
   Beside the position, the additional parameter aFilter can be used to limit the 
   operation to special views only, e.g. to visible and touchable views.
   BroadcastEventAtPosition() is very useful to provide views with one and the same 
   event in order e.g. to inform the views about an important global state alteration. 
   The parameters aPosition and aFilter limit the operation to particular views. 
   To send events to all views regardless of their position use the method @BroadcastEvent().
   BroadcastEventAtPosition() returns the value returned by the @HandleEvent() method 
   of the view which has handled the event. In the case, the event was not handled, 
   the method returns 'null'. */
XObject CoreGroup_BroadcastEventAtPosition( CoreGroup _this, CoreEvent aEvent, XPoint 
  aPosition, XSet aFilter )
{
  CoreView view = _this->last;
  XObject handled = 0;

  while (( view != 0 ) && ( handled == 0 ))
  {
    if (( !aFilter || EwSetContains( view->viewState, aFilter )) && EwIsPointInRect( 
        CoreView__GetExtent( view ), aPosition ))
    {
      CoreGroup grp = EwCastObject( view, CoreGroup );

      if ( grp != 0 )
        handled = CoreGroup_BroadcastEventAtPosition( grp, aEvent, EwMovePointNeg( 
        aPosition, grp->Super1.Bounds.Point1 ), aFilter );
      else
        handled = CoreView__HandleEvent( view, aEvent );
    }

    view = view->prev;
  }

  if ( handled == 0 )
    handled = CoreView__HandleEvent( _this, aEvent );

  return handled;
}

/* Wrapper function for the non virtual method : 'Core::Group.BroadcastEventAtPosition()' */
XObject CoreGroup__BroadcastEventAtPosition( void* _this, CoreEvent aEvent, XPoint 
  aPosition, XSet aFilter )
{
  return CoreGroup_BroadcastEventAtPosition((CoreGroup)_this, aEvent, aPosition, 
  aFilter );
}

/* The method BroadcastEvent() feeds the component with the event passed in the 
   parameter aEvent and propagates it to all views enclosed within the component 
   until the event has been handled or all views are evaluated. If the event remains 
   still unhandled, it will be passed to the component itself.
   The additional parameter aFilter can be used to limit the operation to special 
   views only, e.g. to visible and touchable views. To broadcast the event to all 
   views pass in the parameter aFilter the value 'Core::ViewState[]'.
   BroadcastEvent() is very useful to provide all views with one and the same event 
   in order e.g. to inform all views about an important global state alteration. 
   To send events to views enclosing a given position use the method @BroadcastEventAtPosition().
   BroadcastEvent() returns the value returned by the @HandleEvent() method of the 
   view which has handled the event. In the case, the event was not handled, the 
   method returns 'null'. */
XObject CoreGroup_BroadcastEvent( CoreGroup _this, CoreEvent aEvent, XSet aFilter )
{
  CoreView view = _this->last;
  XObject handled = 0;

  while (( view != 0 ) && ( handled == 0 ))
  {
    if ( !aFilter || EwSetContains( view->viewState, aFilter ))
    {
      CoreGroup grp = EwCastObject( view, CoreGroup );

      if ( grp != 0 )
        handled = CoreGroup__BroadcastEvent( grp, aEvent, aFilter );
      else
        handled = CoreView__HandleEvent( view, aEvent );
    }

    view = view->prev;
  }

  if ( handled == 0 )
    handled = CoreView__HandleEvent( _this, aEvent );

  if ( handled == 0 )
    handled = CoreGroup_processKeyHandlers( _this, aEvent );

  return handled;
}

/* Wrapper function for the virtual method : 'Core::Group.BroadcastEvent()' */
XObject CoreGroup__BroadcastEvent( void* _this, CoreEvent aEvent, XSet aFilter )
{
  return ((CoreGroup)_this)->_VMT->BroadcastEvent((CoreGroup)_this, aEvent, aFilter );
}

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void CoreGroup_UpdateLayout( CoreGroup _this, XPoint aSize )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aSize );
}

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visibility or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void CoreGroup_UpdateViewState( CoreGroup _this, XSet aState )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aState );
}

/* Wrapper function for the virtual method : 'Core::Group.UpdateViewState()' */
void CoreGroup__UpdateViewState( void* _this, XSet aState )
{
  ((CoreGroup)_this)->_VMT->UpdateViewState((CoreGroup)_this, aState );
}

/* The method InvalidateViewState() declares the state of this component as changed, 
   so its visual aspect possibly doesn't reflect its current state anymore. To update 
   the visual aspect, the framework will invoke the @UpdateViewState() method. */
void CoreGroup_InvalidateViewState( CoreGroup _this )
{
  _this->Super2.viewState = _this->Super2.viewState | CoreViewStatePendingViewState;
  EwPostSignal( EwNewSlot( _this, CoreGroup_updateComponent ), ((XObject)_this ));
}

/* The method InvalidateArea() declares the given area of the component as invalid, 
   this means this area should be redrawn at the next screen update. */
void CoreGroup_InvalidateArea( CoreGroup _this, XRect aArea )
{
  CoreGroup grp = _this;

  while (( grp != 0 ) && !EwIsRectEmpty( aArea ))
  {
    GraphicsCanvas buf = grp->buffer;

    if (( grp->Super2.Owner == 0 ) && ( grp != _this ))
    {
      CoreGroup__InvalidateArea( grp, aArea );
      return;
    }

    if ( buf != 0 )
    {
      XBool fullOffScreenBufferUpdate;

      if ( EwIsRectEmpty( buf->InvalidArea ))
      {
        EwNotifyObjObservers((XObject)grp, 0 );
        EwNotifyObjObservers((XObject)buf, 0 );
      }

      fullOffScreenBufferUpdate = 0;
      fullOffScreenBufferUpdate = EwFullOffScreenBufferUpdate;

      if ( fullOffScreenBufferUpdate )
        buf->InvalidArea = EwGetRectORect( grp->Super1.Bounds );
      else
        buf->InvalidArea = EwUnionRect( buf->InvalidArea, aArea );
    }

    if ( !(( grp->Super2.viewState & CoreViewStateVisible ) == CoreViewStateVisible ))
      return;

    aArea = EwIntersectRect( EwMoveRectPos( aArea, grp->Super1.Bounds.Point1 ), 
    grp->Super1.Bounds );
    grp = grp->Super2.Owner;
  }
}

/* Wrapper function for the virtual method : 'Core::Group.InvalidateArea()' */
void CoreGroup__InvalidateArea( void* _this, XRect aArea )
{
  ((CoreGroup)_this)->_VMT->InvalidateArea((CoreGroup)_this, aArea );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void CoreGroup_Init( CoreGroup _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  CoreGroup_InvalidateViewState( _this );
}

/* The method FindSiblingView() searches for a sibling view of the view specified 
   in the parameter aView - aView itself will be excluded from the search operation.
   The method combines the functionality of @FindNextView() and @FindPrevView() 
   and tries to find any neighbor view (regarding the Z-order not the position).
   The additional parameter aFilter can be used to limit the search operation to 
   special views only, e.g. to visible and touchable views.
   If there are no other views complying the filter condition, the method returns 
   'null'. In contrast to other find methods, FindSiblingView() will fail, if it 
   has been invoked with aView == 'null'. */
CoreView CoreGroup_FindSiblingView( CoreGroup _this, CoreView aView, XSet aFilter )
{
  CoreView nextView;
  CoreView prevView;
  XSet notFilter;

  if (( aView == 0 ) || ( aView->Owner != _this ))
    return 0;

  nextView = aView->next;
  prevView = aView->prev;
  notFilter = CoreViewStateDialog;

  if ((( aFilter & CoreViewStateDialog ) == CoreViewStateDialog ))
    notFilter = 0;

  while (( nextView != 0 ) || ( prevView != 0 ))
  {
    if ((( nextView != 0 ) && ( !aFilter || EwSetContains( nextView->viewState, 
        aFilter ))) && ( !notFilter || !EwSetContains( nextView->viewState, notFilter )))
      return nextView;

    if ((( prevView != 0 ) && ( !aFilter || EwSetContains( prevView->viewState, 
        aFilter ))) && ( !notFilter || !EwSetContains( prevView->viewState, notFilter )))
      return prevView;

    if ( nextView != 0 )
      nextView = nextView->next;

    if ( prevView != 0 )
      prevView = prevView->prev;
  }

  return 0;
}

/* The method Restack() changes the Z-order of views in the component. Depending 
   on the parameter aOrder the method will elevate or lower the given view aView. 
   If aOrder is negative, the view will be lowered to the background. If aOrder 
   is positive, the view will be elevated to the foreground. If aOrder == 0, no 
   modification will take place.
   The absolute value of aOrder determines the number of sibling views the view 
   has to skip over in order to reach its new Z-order.
   Please note, changing the Z-order of views within a component containing a Core::Outline 
   view can cause this outline to update its automatic row or column formation. */
void CoreGroup_Restack( CoreGroup _this, CoreView aView, XInt32 aOrder )
{
  CoreView after;
  CoreView before;

  if ( aView == 0 )
  {
    EwThrow( EwLoadString( &_Const0003 ));
    return;
  }

  if ( aView->Owner != _this )
  {
    EwThrow( EwLoadString( &_Const0004 ));
    return;
  }

  after = aView;
  before = aView;

  while (( aOrder > 0 ) && ( after->next != 0 ))
  {
    after = after->next;
    aOrder = aOrder - 1;
  }

  while (( aOrder < 0 ) && ( before->prev != 0 ))
  {
    before = before->prev;
    aOrder = aOrder + 1;
  }

  if (( after == aView ) && ( before == aView ))
    return;

  if ((( aView->viewState & ( CoreViewStateEmbedded | CoreViewStateVisible )) == 
      ( CoreViewStateEmbedded | CoreViewStateVisible )))
  {
    if (( aView->prev != 0 ) && ( aView->layoutContext != 0 ))
      aView->prev->viewState = aView->prev->viewState | CoreViewStateRequestLayout;

    aView->viewState = aView->viewState | CoreViewStateRequestLayout;
    _this->Super2.viewState = _this->Super2.viewState | CoreViewStatePendingLayout;
    EwPostSignal( EwNewSlot( _this, CoreGroup_updateComponent ), ((XObject)_this ));
  }

  if ((( aView->viewState & CoreViewStateIsOutline ) == CoreViewStateIsOutline ))
  {
    if ( aView->prev != 0 )
      aView->prev->viewState = aView->prev->viewState | CoreViewStateRequestLayout;

    aView->viewState = aView->viewState | CoreViewStateRequestLayout;
    _this->Super2.viewState = _this->Super2.viewState | CoreViewStatePendingLayout;
    EwPostSignal( EwNewSlot( _this, CoreGroup_updateComponent ), ((XObject)_this ));
  }

  if ( aView->prev != 0 )
    aView->prev->next = aView->next;

  if ( aView->next != 0 )
    aView->next->prev = aView->prev;

  if ( _this->first == aView )
    _this->first = aView->next;

  if ( _this->last == aView )
    _this->last = aView->prev;

  if ( after != aView )
  {
    aView->next = after->next;
    aView->prev = after;
    after->next = aView;

    if ( aView->next != 0 )
      aView->next->prev = aView;
  }

  if ( before != aView )
  {
    aView->next = before;
    aView->prev = before->prev;
    before->prev = aView;

    if ( aView->prev != 0 )
      aView->prev->next = aView;
  }

  if ( aView->next == 0 )
    _this->last = aView;

  if ( aView->prev == 0 )
    _this->first = aView;

  if ((( aView->viewState & CoreViewStateVisible ) == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this, CoreView__GetExtent( aView ));
}

/* Wrapper function for the virtual method : 'Core::Group.Restack()' */
void CoreGroup__Restack( void* _this, CoreView aView, XInt32 aOrder )
{
  ((CoreGroup)_this)->_VMT->Restack((CoreGroup)_this, aView, aOrder );
}

/* The method Add() inserts the given view aView into this component and places 
   it at a Z-order position resulting from the parameter aOrder. The parameter determines 
   the number of sibling views the view has to skip over starting with the top most 
   view. If aOrder == 0, the newly added view will obtain the top most position. 
   If the value is negative, the view will be lowered to the background accordingly 
   to the absolute value of aOrder. After this operation the view belongs to the 
   component - the view can appear on the screen and it can receive events.
   Please note, adding of views to a component containing a Core::Outline view can 
   cause this outline to update its automatic row or column formation. */
void CoreGroup_Add( CoreGroup _this, CoreView aView, XInt32 aOrder )
{
  if ( aView == 0 )
  {
    EwThrow( EwLoadString( &_Const0005 ));
    return;
  }

  if ( aView->Owner != 0 )
  {
    EwThrow( EwLoadString( &_Const0006 ));
    return;
  }

  aView->Owner = _this;
  aView->prev = _this->last;

  if ( _this->last != 0 )
    _this->last->next = aView;

  _this->last = aView;

  if ( _this->first == 0 )
    _this->first = aView;

  if ( aOrder != 0 )
    CoreGroup__Restack( _this, aView, aOrder );

  if ((( aView->viewState & CoreViewStateVisible ) == CoreViewStateVisible ))
    CoreGroup__InvalidateArea( _this, CoreView__GetExtent( aView ));

  if (((( _this->Focus == 0 ) && (( aView->viewState & CoreViewStateFocusable ) 
      == CoreViewStateFocusable )) && (( aView->viewState & CoreViewStateEnabled ) 
      == CoreViewStateEnabled )) && !(( aView->viewState & CoreViewStateDialog ) 
      == CoreViewStateDialog ))
    CoreGroup__OnSetFocus( _this, aView );

  if ((( aView->viewState & ( CoreViewStateEmbedded | CoreViewStateVisible )) == 
      ( CoreViewStateEmbedded | CoreViewStateVisible )))
  {
    aView->viewState = aView->viewState | CoreViewStateRequestLayout;
    _this->Super2.viewState = _this->Super2.viewState | CoreViewStatePendingLayout;
    EwPostSignal( EwNewSlot( _this, CoreGroup_updateComponent ), ((XObject)_this ));
  }

  if ((( aView->viewState & CoreViewStateIsOutline ) == CoreViewStateIsOutline ))
  {
    aView->viewState = aView->viewState | CoreViewStateRequestLayout;
    _this->Super2.viewState = _this->Super2.viewState | CoreViewStatePendingLayout;
    EwPostSignal( EwNewSlot( _this, CoreGroup_updateComponent ), ((XObject)_this ));
  }
}

/* Wrapper function for the virtual method : 'Core::Group.Add()' */
void CoreGroup__Add( void* _this, CoreView aView, XInt32 aOrder )
{
  ((CoreGroup)_this)->_VMT->Add((CoreGroup)_this, aView, aOrder );
}

/* Variants derived from the class : 'Core::Group' */
EW_DEFINE_CLASS_VARIANTS( CoreGroup )
EW_END_OF_CLASS_VARIANTS( CoreGroup )

/* Virtual Method Table (VMT) for the class : 'Core::Group' */
EW_DEFINE_CLASS( CoreGroup, CoreRectView, first, Opacity, Opacity, Opacity, Opacity, 
                 "Core::Group" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CoreGroup )

/* Initializer for the class 'Core::Root' */
void CoreRoot__Init( CoreRoot _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->cursorHoldTimer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreRoot );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Super3.viewState = CoreViewStateAlphaBlended | CoreViewStateEnabled | CoreViewStateFocusable 
  | CoreViewStateFocused | CoreViewStateSelected | CoreViewStateTouchable | CoreViewStateVisible;
  _this->hasRootFocus = 1;
  CoreTimer_OnSetPeriod( &_this->cursorHoldTimer, 50 );
  _this->CursorDragLimit = 8;
  _this->CursorSequelDelay = 250;
  _this->cursorHoldTimer.OnTrigger = EwNewSlot( _this, CoreRoot_cursorHoldTimerProc );
}

/* Re-Initializer for the class 'Core::Root' */
void CoreRoot__ReInit( CoreRoot _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->cursorHoldTimer );
}

/* Finalizer method for the class 'Core::Root' */
void CoreRoot__Done( CoreRoot _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreRoot );

  /* Finalize all embedded objects */
  CoreTimer__Done( &_this->cursorHoldTimer );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method GetRoot() delivers the application object, this view belongs to. The 
   application object represents the entire screen of the GUI application. Thus 
   in the views hierarchy, the application object serves as the root view.
   This method can fail and return null if the view still doesn't belong to any 
   owner group. */
CoreRoot CoreRoot_GetRoot( CoreRoot _this )
{
  return _this;
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
void CoreRoot_Draw( CoreRoot _this, GraphicsCanvas aCanvas, XRect aClip, XPoint 
  aOffset, XInt32 aOpacity, XBool aBlend )
{
  XBool fullScreenUpdate = 0;

  fullScreenUpdate = (XBool)EwFullScreenUpdate;

  if ( !fullScreenUpdate )
    GraphicsCanvas_FillRectangle( aCanvas, aClip, EwMoveRectPos( EwMoveRectPos( 
    aClip, aOffset ), _this->Super2.Bounds.Point1 ), _Const0007, _Const0007, _Const0007, 
    _Const0007, 0 );

  CoreGroup_Draw((CoreGroup)_this, aCanvas, aClip, aOffset, aOpacity, aBlend );
}

/* The method ChangeViewState() modifies the current state of the view. The state 
   is a set of switches determining whether a view is visible, whether it can react 
   to user inputs or whether it is just performing some update operations, etc.
   The modification is controlled by the the both parameters. The first aSetState 
   contains the switches to activate within the view state. The second aClearState 
   determines all switches to disable.
   Depending on the state alteration the method will perform different operations, 
   e.g. in response to the clearing of the visible state, the method will request 
   a screen redraw to make disappear the view from the screen.
   ChangeViewState() is invoked automatically by the framework, so you never should 
   need to invoke it directly. All relevant states are available as properties e.g. 
   the property Visible in derived classes reflects the visible state of the view. */
void CoreRoot_ChangeViewState( CoreRoot _this, XSet aSetState, XSet aClearState )
{
  CoreGroup_ChangeViewState((CoreGroup)_this, aSetState, aClearState );

  if (( _this->Super3.Owner == 0 ) && ((( aSetState & CoreViewStateVisible ) == 
      CoreViewStateVisible ) || (( aClearState & CoreViewStateVisible ) == CoreViewStateVisible )))
    CoreGroup__InvalidateArea( _this, EwGetRectORect( _this->Super2.Bounds ));

  if (( _this->Super3.Owner == 0 ) && ((( aSetState & CoreViewStateAlphaBlended ) 
      == CoreViewStateAlphaBlended ) || (( aClearState & CoreViewStateAlphaBlended ) 
      == CoreViewStateAlphaBlended )))
    CoreGroup__InvalidateArea( _this, EwGetRectORect( _this->Super2.Bounds ));
}

/* 'C' function for method : 'Core::Root.OnSetFocus()' */
void CoreRoot_OnSetFocus( CoreRoot _this, CoreView value )
{
  if (( value != (CoreView)_this->VirtualKeyboard ) || ( value == 0 ))
    CoreGroup_OnSetFocus((CoreGroup)_this, value );
}

/* The method DispatchEvent() feeds the component with the event passed in the parameter 
   aEvent and propagates it along the so-called focus path. This focus path leads 
   to the currently selected keyboard event receiver view. If the event is rejected 
   by the view, the same operation is repeated for the next superior view. This 
   permits the hierarchical event dispatching until a willing view has handled the 
   event or all views in the focus path have been evaluated. If the event remains 
   still unhandled, it will be passed to the component itself.
   The focus path is established by the property @Focus. 
   DispatchEvent() returns the value returned by the @HandleEvent() method of the 
   view which has handled the event. In the case, the event was not handled, the 
   method returns 'null'. */
XObject CoreRoot_DispatchEvent( CoreRoot _this, CoreEvent aEvent )
{
  XObject handled;

  if (( _this->animationLock > 0 ) && ( EwCastObject( aEvent, CoreKeyEvent ) != 
      0 ))
    return 0;

  if ( aEvent != 0 )
  {
    aEvent->IsTimeExact = (XBool)( _this->currentEventTimestamp != 0 );

    if ( _this->currentEventTimestamp != 0 )
      aEvent->Time = _this->currentEventTimestamp;
  }

  handled = 0;

  if ( _this->VirtualKeyboard != 0 )
  {
    handled = CoreGroup__DispatchEvent( _this->VirtualKeyboard, aEvent );

    if ( handled != 0 )
    {
      _this->currentEventTimestamp = 0;
      return handled;
    }
  }

  if ( _this->modalGroups != 0 )
  {
    handled = CoreGroup__DispatchEvent( _this->modalGroups->group, aEvent );

    if ( handled == 0 )
      handled = CoreView__HandleEvent( _this, aEvent );

    if ( handled == 0 )
      handled = CoreGroup_processKeyHandlers((CoreGroup)_this, aEvent );

    _this->currentEventTimestamp = 0;
    return handled;
  }

  handled = CoreGroup_DispatchEvent((CoreGroup)_this, aEvent );
  _this->currentEventTimestamp = 0;
  return handled;
}

/* The method BroadcastEvent() feeds the component with the event passed in the 
   parameter aEvent and propagates it to all views enclosed within the component 
   until the event has been handled or all views are evaluated. If the event remains 
   still unhandled, it will be passed to the component itself.
   The additional parameter aFilter can be used to limit the operation to special 
   views only, e.g. to visible and touchable views. To broadcast the event to all 
   views pass in the parameter aFilter the value 'Core::ViewState[]'.
   BroadcastEvent() is very useful to provide all views with one and the same event 
   in order e.g. to inform all views about an important global state alteration. 
   To send events to views enclosing a given position use the method @BroadcastEventAtPosition().
   BroadcastEvent() returns the value returned by the @HandleEvent() method of the 
   view which has handled the event. In the case, the event was not handled, the 
   method returns 'null'. */
XObject CoreRoot_BroadcastEvent( CoreRoot _this, CoreEvent aEvent, XSet aFilter )
{
  XObject handled;

  if ( aEvent != 0 )
  {
    aEvent->IsTimeExact = (XBool)( _this->currentEventTimestamp != 0 );

    if ( _this->currentEventTimestamp != 0 )
      aEvent->Time = _this->currentEventTimestamp;
  }

  handled = CoreGroup_BroadcastEvent((CoreGroup)_this, aEvent, aFilter );
  _this->currentEventTimestamp = 0;
  return handled;
}

/* The method InvalidateArea() declares the given area of the component as invalid, 
   this means this area should be redrawn at the next screen update. */
void CoreRoot_InvalidateArea( CoreRoot _this, XRect aArea )
{
  XBool fullScreenUpdate;
  XInt32 i;
  XInt32 j;
  XInt32 k;
  XInt32 jj;
  XInt32 kk;
  XInt32 best;

  if ( _this->updateLock > 0 )
  {
    EwThrow( EwLoadString( &_Const0008 ));
    return;
  }

  if (( _this->Super1.buffer != 0 ) && ( _this->Super3.Owner == 0 ))
  {
    XBool fullOffScreenBufferUpdate;

    if ( EwIsRectEmpty( _this->Super1.buffer->InvalidArea ))
    {
      EwNotifyObjObservers((XObject)_this, 0 );
      EwNotifyObjObservers((XObject)_this->Super1.buffer, 0 );
    }

    fullOffScreenBufferUpdate = 0;
    fullOffScreenBufferUpdate = EwFullOffScreenBufferUpdate;

    if ( fullOffScreenBufferUpdate )
      _this->Super1.buffer->InvalidArea = EwGetRectORect( _this->Super2.Bounds );
    else
      _this->Super1.buffer->InvalidArea = EwUnionRect( _this->Super1.buffer->InvalidArea, 
      aArea );
  }

  fullScreenUpdate = 0;
  fullScreenUpdate = (XBool)EwFullScreenUpdate;

  if ( fullScreenUpdate )
    aArea = EwGetRectORect( _this->Super2.Bounds );

  if ( _this->Super3.Owner != 0 )
  {
    CoreGroup_InvalidateArea((CoreGroup)_this, aArea );
    return;
  }

  aArea = EwIntersectRect( EwMoveRectPos( aArea, _this->Super2.Bounds.Point1 ), 
  _this->Super2.Bounds );

  if ( EwIsRectEmpty( aArea ))
    return;

  for ( i = 0; i < _this->noOfRegions; i = i + 1 )
    if ( !EwIsRectEmpty( EwIntersectRect( _this->regions[ EwCheckIndex( i, 4 )], 
        aArea )))
    {
      _this->regions[ EwCheckIndex( i, 4 )] = EwUnionRect( _this->regions[ EwCheckIndex( 
      i, 4 )], aArea );
      _this->regionsArea[ EwCheckIndex( i, 4 )] = EwGetRectArea( _this->regions[ 
      EwCheckIndex( i, 4 )]);
      return;
    }

  if ( _this->noOfRegions < 3 )
  {
    _this->regions[ EwCheckIndex( _this->noOfRegions, 4 )] = aArea;
    _this->regionsArea[ EwCheckIndex( _this->noOfRegions, 4 )] = EwGetRectArea( 
    aArea );
    _this->noOfRegions = _this->noOfRegions + 1;
    return;
  }

  jj = 0;
  kk = 0;
  best = 2147483647;
  _this->regions[ EwCheckIndex( _this->noOfRegions, 4 )] = aArea;
  _this->regionsArea[ EwCheckIndex( _this->noOfRegions, 4 )] = EwGetRectArea( aArea );

  for ( j = 0; j <= _this->noOfRegions; j = j + 1 )
    for ( k = j + 1; k <= _this->noOfRegions; k = k + 1 )
    {
      XInt32 unionArea = EwGetRectArea( EwUnionRect( _this->regions[ EwCheckIndex( 
        j, 4 )], _this->regions[ EwCheckIndex( k, 4 )]));
      XInt32 overhead = ( unionArea << 8 ) / ( _this->regionsArea[ EwCheckIndex( 
        j, 4 )] + _this->regionsArea[ EwCheckIndex( k, 4 )]);

      if ( overhead < best )
      {
        best = overhead;
        jj = j;
        kk = k;
      }
    }

  _this->regions[ EwCheckIndex( jj, 4 )] = EwUnionRect( _this->regions[ EwCheckIndex( 
  jj, 4 )], _this->regions[ EwCheckIndex( kk, 4 )]);
  _this->regionsArea[ EwCheckIndex( jj, 4 )] = EwGetRectArea( _this->regions[ EwCheckIndex( 
  jj, 4 )]);

  if ( kk != _this->noOfRegions )
  {
    _this->regions[ EwCheckIndex( kk, 4 )] = _this->regions[ EwCheckIndex( _this->noOfRegions, 
    4 )];
    _this->regionsArea[ EwCheckIndex( kk, 4 )] = _this->regionsArea[ EwCheckIndex( 
    _this->noOfRegions, 4 )];
  }
}

/* The method Restack() changes the Z-order of views in the component. Depending 
   on the parameter aOrder the method will elevate or lower the given view aView. 
   If aOrder is negative, the view will be lowered to the background. If aOrder 
   is positive, the view will be elevated to the foreground. If aOrder == 0, no 
   modification will take place.
   The absolute value of aOrder determines the number of sibling views the view 
   has to skip over in order to reach its new Z-order.
   Please note, changing the Z-order of views within a component containing a Core::Outline 
   view can cause this outline to update its automatic row or column formation. */
void CoreRoot_Restack( CoreRoot _this, CoreView aView, XInt32 aOrder )
{
  if ( aView == 0 )
  {
    EwThrow( EwLoadString( &_Const0003 ));
    return;
  }

  if ((( aOrder > 0 ) && ( _this->VirtualKeyboard != 0 )) && ((CoreRoot)_this->VirtualKeyboard->Super2.Owner 
      == _this ))
  {
    CoreView view = aView;
    XInt32 order = aOrder;

    while (( view->next != (CoreView)_this->VirtualKeyboard ) && ( order > 0 ))
    {
      view = view->next;
      order = order - 1;
    }

    aOrder = aOrder - order;
  }

  CoreGroup_Restack((CoreGroup)_this, aView, aOrder );
}

/* The method Add() inserts the given view aView into this component and places 
   it at a Z-order position resulting from the parameter aOrder. The parameter determines 
   the number of sibling views the view has to skip over starting with the top most 
   view. If aOrder == 0, the newly added view will obtain the top most position. 
   If the value is negative, the view will be lowered to the background accordingly 
   to the absolute value of aOrder. After this operation the view belongs to the 
   component - the view can appear on the screen and it can receive events.
   Please note, adding of views to a component containing a Core::Outline view can 
   cause this outline to update its automatic row or column formation. */
void CoreRoot_Add( CoreRoot _this, CoreView aView, XInt32 aOrder )
{
  if ((( aOrder >= 0 ) && ( _this->VirtualKeyboard != 0 )) && ((CoreRoot)_this->VirtualKeyboard->Super2.Owner 
      == _this ))
    aOrder = -1;

  CoreGroup_Add((CoreGroup)_this, aView, aOrder );
}

/* 'C' function for method : 'Core::Root.createDragEvent()' */
CoreDragEvent CoreRoot_createDragEvent( CoreRoot _this )
{
  CoreDragEvent event = EwNewObject( CoreDragEvent, 0 );

  event->Super1.IsTimeExact = (XBool)( _this->currentEventTimestamp != 0 );

  if ( _this->currentEventTimestamp != 0 )
    event->Super1.Time = _this->currentEventTimestamp;

  return event;
}

/* 'C' function for method : 'Core::Root.createCursorEvent()' */
CoreCursorEvent CoreRoot_createCursorEvent( CoreRoot _this )
{
  CoreCursorEvent event = EwNewObject( CoreCursorEvent, 0 );

  event->Super1.IsTimeExact = (XBool)( _this->currentEventTimestamp != 0 );

  if ( _this->currentEventTimestamp != 0 )
    event->Super1.Time = _this->currentEventTimestamp;

  return event;
}

/* 'C' function for method : 'Core::Root.createCursorGrabEvent()' */
CoreCursorGrabEvent CoreRoot_createCursorGrabEvent( CoreRoot _this )
{
  CoreCursorGrabEvent event = EwNewObject( CoreCursorGrabEvent, 0 );

  event->Super1.IsTimeExact = (XBool)( _this->currentEventTimestamp != 0 );

  if ( _this->currentEventTimestamp != 0 )
    event->Super1.Time = _this->currentEventTimestamp;

  return event;
}

/* 'C' function for method : 'Core::Root.cursorHoldTimerProc()' */
void CoreRoot_cursorHoldTimerProc( CoreRoot _this, XObject sender )
{
  XInt32 i;
  XBool found;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  found = 0;

  for ( i = 0; i < 10; i = i + 1 )
    if ( _this->cursorTargetView[ EwCheckIndex( i, 10 )] != 0 )
    {
      XPoint pos = _this->cursorLastPos[ EwCheckIndex( i, 10 )];
      CoreGroup grp = _this->cursorTargetView[ EwCheckIndex( i, 10 )]->Owner;

      while (( grp != 0 ) && ((CoreRoot)grp != _this ))
      {
        pos = EwMovePointNeg( pos, grp->Super1.Bounds.Point1 );
        grp = grp->Super2.Owner;
      }

      if (( grp == 0 ) && ((CoreRoot)_this->cursorTargetView[ EwCheckIndex( i, 10 )] 
          != _this ))
      {
        CoreView tmp = _this->cursorTargetView[ EwCheckIndex( i, 10 )];
        _this->cursorFinger = i;
        _this->cursorTargetView[ EwCheckIndex( i, 10 )] = 0;
        CoreView__HandleEvent( tmp, ((CoreEvent)CoreCursorEvent_InitializeUp( CoreRoot_createCursorEvent( 
        _this ), i, _this->cursorCurrentPos[ EwCheckIndex( i, 10 )], _this->cursorHittingPos[ 
        EwCheckIndex( i, 10 )], _this->cursorHoldPeriod[ EwCheckIndex( i, 10 )], 
        _this->cursorSequelCounter[ EwCheckIndex( i, 10 )] + 1, _this->cursorHitOffset[ 
        EwCheckIndex( i, 10 )], 0, _this->cursorLastPos[ EwCheckIndex( i, 10 )], 
        _this->cursorFirstPos[ EwCheckIndex( i, 10 )])));

        if ( tmp == _this->cursorCaptureView )
          _this->cursorCaptureView = 0;

        CoreGroup__BroadcastEvent( _this, ((CoreEvent)CoreCursorGrabEvent_InitializeUp( 
        CoreRoot_createCursorGrabEvent( _this ), i, _this->cursorSequelCounter[ 
        EwCheckIndex( i, 10 )] + 1, 0, tmp, _this->cursorLastPos[ EwCheckIndex( 
        i, 10 )])), CoreViewStateEnabled | CoreViewStateTouchable );
      }
      else
      {
        _this->cursorHoldPeriod[ EwCheckIndex( i, 10 )] = (XInt32)( _this->cursorHoldTimer.Time 
        - _this->cursorHittingTime[ EwCheckIndex( i, 10 )]);

        if ( _this->cursorHoldPeriod[ EwCheckIndex( i, 10 )] < 10 )
          _this->cursorHoldPeriod[ EwCheckIndex( i, 10 )] = 10;

        _this->cursorFinger = i;
        CoreView__HandleEvent( _this->cursorTargetView[ EwCheckIndex( i, 10 )], 
        ((CoreEvent)CoreCursorEvent_InitializeHold( CoreRoot_createCursorEvent( 
        _this ), i, pos, _this->cursorHittingPos[ EwCheckIndex( i, 10 )], _this->cursorHoldPeriod[ 
        EwCheckIndex( i, 10 )], _this->cursorSequelCounter[ EwCheckIndex( i, 10 )] 
        + 1, _this->cursorHitOffset[ EwCheckIndex( i, 10 )], _this->cursorLastPos[ 
        EwCheckIndex( i, 10 )], _this->cursorFirstPos[ EwCheckIndex( i, 10 )])));
        found = 1;
      }
    }

  if ( !found )
    CoreTimer_OnSetEnabled( &_this->cursorHoldTimer, 0 );
}

/* The method GetFPS() returns the screen update performance expressed in frames 
   per second. The performance is estimated for the period between the current and 
   the preceding invocation of the GetFPS() method. */
XInt32 CoreRoot_GetFPS( CoreRoot _this )
{
  XUInt32 ticksCount = 0;
  XInt32 fps = 0;

  ticksCount = (XUInt32)EwGetTicks();

  if (( _this->fpsTime != 0 ) && ( ticksCount > _this->fpsTime ))
    fps = ( _this->fpsCounter * 1000 ) / (XInt32)( ticksCount - _this->fpsTime );

  _this->fpsCounter = 0;
  _this->fpsTime = ticksCount;
  return fps;
}

/* Wrapper function for the non virtual method : 'Core::Root.GetFPS()' */
XInt32 CoreRoot__GetFPS( void* _this )
{
  return CoreRoot_GetFPS((CoreRoot)_this );
}

/* The method Update() exists for the integration purpose with the underlying target 
   system. You will never need to invoke this method directly from your GUI application. 
   The method is responsible for the redrawing of invalid screen areas.
   This method forces the redraw of all affected views and returns the position 
   and the size of the redrawn area. If there is no invalid area to redraw, */
XRect CoreRoot_Update( CoreRoot _this )
{
  if ( _this->canvas == 0 )
  {
    _this->canvas = EwNewObject( GraphicsCanvas, 0 );
    ResourcesBitmap__OnSetFrameSize( _this->canvas, EwGetRectSize( _this->Super2.Bounds ));
  }

  ResourcesBitmap__Update( _this->canvas );
  return CoreRoot_UpdateGE20( _this, _this->canvas );
}

/* Wrapper function for the non virtual method : 'Core::Root.Update()' */
XRect CoreRoot__Update( void* _this )
{
  return CoreRoot_Update((CoreRoot)_this );
}

/* The method UpdateGE20() exists for the integration purpose with the underlying 
   target system. You will never need to invoke this method directly from your GUI 
   application. The method is responsible for the redrawing of invalid screen areas.
   This method forces the redraw of all affected views into the given drawing destination 
   aCanvas and returns the position and the size of the redrawn area. If there is 
   no invalid area to redraw, UpdateGE20() returns an empty rectangle. This method 
   is intended to work with the new Graphics Engine 2.0 only. */
XRect CoreRoot_UpdateGE20( CoreRoot _this, GraphicsCanvas aCanvas )
{
  XRect wholeArea;

  if ( CoreRoot_BeginUpdate( _this ) == 0 )
    return _Const0001;

  wholeArea = CoreRoot_UpdateCanvas( _this, aCanvas, _Const0000 );
  CoreRoot_EndUpdate( _this );
  return wholeArea;
}

/* Wrapper function for the non virtual method : 'Core::Root.UpdateGE20()' */
XRect CoreRoot__UpdateGE20( void* _this, GraphicsCanvas aCanvas )
{
  return CoreRoot_UpdateGE20((CoreRoot)_this, aCanvas );
}

/* The method EndUpdate() belongs to an interface required for integration purpose 
   with the underlying target system. You will never need to invoke this method 
   directly from your GUI application.
   Calling this method finalizes the screen update cycle initiated by the preceding 
   @BeginUpdate() invocation. */
void CoreRoot_EndUpdate( CoreRoot _this )
{
  if ( _this->noOfRegions > 0 )
  {
    _this->fpsCounter = _this->fpsCounter + 1;
    _this->noOfRegions = 0;
  }
}

/* Wrapper function for the non virtual method : 'Core::Root.EndUpdate()' */
void CoreRoot__EndUpdate( void* _this )
{
  CoreRoot_EndUpdate((CoreRoot)_this );
}

/* The method UpdateCanvas() belongs to an interface required for integration purpose 
   with the underlying target system. You will never need to invoke this method 
   directly from your GUI application.
   This method redraws all invalid screen regions into the canvas passed in the 
   parameter aCanvas. The parameter aOffset determines the origin where to map the 
   top-left corner of the canvas on the screen. All update areas lying outside the 
   canvas are clipped.
   The method returns the position and the size of the resulting area within the 
   canvas affected by the update. If there is no intersection between the canvas 
   and any of the invalid areas, UpdateCanvas() returns an empty rectangle. */
XRect CoreRoot_UpdateCanvas( CoreRoot _this, GraphicsCanvas aCanvas, XPoint aOffset )
{
  XRect wholeArea = _Const0001;
  XRect canvasArea = EwNewRect2Point( aOffset, EwMovePointPos( aCanvas->Super1.FrameSize, 
    aOffset ));
  XInt32 i;
  XInt32 j = _this->noOfRegions;

  _this->updateLock = _this->updateLock + 1;

  for ( i = 0; ( i < j ) && ( i < 4 ); i = i + 1 )
  {
    if ( _this->regionsArea[ EwCheckIndex( i, 4 )] > 0 )
    {
      CoreView__Draw( _this, aCanvas, EwMoveRectNeg( _this->regions[ EwCheckIndex( 
      i, 4 )], aOffset ), EwNegPoint( aOffset ), 255, 1 );
      wholeArea = EwUnionRect( wholeArea, EwIntersectRect( canvasArea, _this->regions[ 
      EwCheckIndex( i, 4 )]));
    }
    else
      j = j + 1;
  }

  _this->updateLock = _this->updateLock - 1;

  if ( !EwIsRectEmpty( wholeArea ))
    return EwMoveRectNeg( wholeArea, aOffset );
  else
    return wholeArea;
}

/* Wrapper function for the non virtual method : 'Core::Root.UpdateCanvas()' */
XRect CoreRoot__UpdateCanvas( void* _this, GraphicsCanvas aCanvas, XPoint aOffset )
{
  return CoreRoot_UpdateCanvas((CoreRoot)_this, aCanvas, aOffset );
}

/* The method GetUpdateRegion() belongs to an interface required for integration 
   purpose with the underlying target system. You will never need to invoke this 
   method directly from your GUI application.
   This method returns the position and the size of the invalid region with the 
   number specified in the parameter aRegionNo. If the desired region does not exist, 
   the method returns an empty area. */
XRect CoreRoot_GetUpdateRegion( CoreRoot _this, XInt32 aRegionNo )
{
  XInt32 i;
  XInt32 j = _this->noOfRegions;

  if ( aRegionNo < 0 )
    return _Const0001;

  for ( i = 0; ( i < j ) && ( i < 4 ); i = i + 1 )
  {
    if ( _this->regionsArea[ EwCheckIndex( i, 4 )] == 0 )
    {
      j = j + 1;
      aRegionNo = aRegionNo + 1;
    }
    else
      if ( i == aRegionNo )
        return _this->regions[ EwCheckIndex( i, 4 )];
  }

  return _Const0001;
}

/* Wrapper function for the non virtual method : 'Core::Root.GetUpdateRegion()' */
XRect CoreRoot__GetUpdateRegion( void* _this, XInt32 aRegionNo )
{
  return CoreRoot_GetUpdateRegion((CoreRoot)_this, aRegionNo );
}

/* The method BeginUpdate() belongs to an interface required for integration purpose 
   with the underlying target system. You will never need to invoke this method 
   directly from your GUI application.
   Calling this method initiates a new screen update cycle. Subsequently the method 
   @UpdateCanvas() can be called in order to perform all waiting drawing operations. 
   If necessary UpdateCanvas() can be called multiple times in order to draw multiple 
   screen areas individually. Finally the update cycle is terminated by calling 
   the method @EndUpdate().
   The method BeginUpdate() returns the number of invalid regions waiting currently 
   for the update. If there is nothing to update, the method returns 0 (zero). The 
   position and the size of the invalid regions can be queried by using the method 
   @GetUpdateRegion(). */
XInt32 CoreRoot_BeginUpdate( CoreRoot _this )
{
  XBool preserveFramebufferContent = 1;
  XBool fullScreenUpdate = 0;
  XInt32 i;
  XRect overlayArea;
  XBool par1;
  XInt32 j;
  XInt32 k;

  {
    preserveFramebufferContent = (XBool)EwPreserveFramebufferContent;
    fullScreenUpdate           = (XBool)EwFullScreenUpdate;
  }
  overlayArea = _Const0001;
  par1 = (XBool)( _this->noOfRegions > 0 );
  overlayArea = EwGetOverlayArea( par1, 1 );

  if ( !EwIsRectEmpty( overlayArea ))
    CoreGroup__InvalidateArea( _this, overlayArea );

  if (( !preserveFramebufferContent && !fullScreenUpdate ) && ( _this->noOfRegions 
      > 0 ))
  {
    XRect tmpRegions[ 3 ];
    XInt32 tmpNoOfRegions = _this->noOfRegions;

    for ( i = 0; i < tmpNoOfRegions; i = i + 1 )
      tmpRegions[ EwCheckIndex( i, 3 )] = _this->regions[ EwCheckIndex( i, 4 )];

    for ( i = 0; i < _this->noOfLastRegions; i = i + 1 )
      CoreGroup__InvalidateArea( _this, _this->lastRegions[ EwCheckIndex( i, 3 )]);

    for ( i = 0; i < tmpNoOfRegions; i = i + 1 )
      _this->lastRegions[ EwCheckIndex( i, 3 )] = tmpRegions[ EwCheckIndex( i, 3 )];

    _this->noOfLastRegions = tmpNoOfRegions;
  }

  for ( j = 0; j < ( _this->noOfRegions - 1 ); j = j + 1 )
    if ( _this->regionsArea[ EwCheckIndex( j, 4 )] > 0 )
      for ( k = j + 1; k < _this->noOfRegions; k = k + 1 )
        if ( _this->regionsArea[ EwCheckIndex( k, 4 )] > 0 )
        {
          XInt32 unionArea = EwGetRectArea( EwUnionRect( _this->regions[ EwCheckIndex( 
            j, 4 )], _this->regions[ EwCheckIndex( k, 4 )]));

          if ((( unionArea - _this->regionsArea[ EwCheckIndex( j, 4 )]) - _this->regionsArea[ 
              EwCheckIndex( k, 4 )]) < 0 )
          {
            _this->regions[ EwCheckIndex( j, 4 )] = EwUnionRect( _this->regions[ 
            EwCheckIndex( j, 4 )], _this->regions[ EwCheckIndex( k, 4 )]);
            _this->regionsArea[ EwCheckIndex( j, 4 )] = unionArea;
            _this->regionsArea[ EwCheckIndex( k, 4 )] = 0;
          }
        }

  for ( i = _this->noOfRegions - 1; i >= 0; i = i - 1 )
    if ( _this->regionsArea[ EwCheckIndex( i, 4 )] == 0 )
      _this->noOfRegions = _this->noOfRegions - 1;

  return _this->noOfRegions;
}

/* Wrapper function for the non virtual method : 'Core::Root.BeginUpdate()' */
XInt32 CoreRoot__BeginUpdate( void* _this )
{
  return CoreRoot_BeginUpdate((CoreRoot)_this );
}

/* The method DoesNeedUpdate() returns a value indicating whether an area of the 
   application needs an update. If there is no invalid area to redraw, DoesNeedUpdate() 
   returns 'false'. */
XBool CoreRoot_DoesNeedUpdate( CoreRoot _this )
{
  XRect overlayArea;

  if ( _this->noOfRegions > 0 )
    return 1;

  overlayArea = _Const0001;
  overlayArea = EwGetOverlayArea( 0, 0 );

  if ( !EwIsRectEmpty( overlayArea ))
    return 1;

  return 0;
}

/* Wrapper function for the non virtual method : 'Core::Root.DoesNeedUpdate()' */
XBool CoreRoot__DoesNeedUpdate( void* _this )
{
  return CoreRoot_DoesNeedUpdate((CoreRoot)_this );
}

/* The method Initialize() exists for the integration purpose with the underlying 
   target system. You will never need to invoke this method directly from your GUI 
   application. The method is responsible for the preparation of the application 
   object to work with a screen with the given size aSize. */
CoreRoot CoreRoot_Initialize( CoreRoot _this, XPoint aSize )
{
  CoreRectView__OnSetBounds( _this, EwNewRect2Point( _Const0000, aSize ));

  if ( _this->hasRootFocus )
    _this->Super3.viewState = _this->Super3.viewState | ( CoreViewStateFocused | 
    CoreViewStateSelected );
  else
    _this->Super3.viewState = _this->Super3.viewState | CoreViewStateSelected;

  CoreGroup__InvalidateArea( _this, _this->Super2.Bounds );
  return _this;
}

/* Wrapper function for the non virtual method : 'Core::Root.Initialize()' */
CoreRoot CoreRoot__Initialize( void* _this, XPoint aSize )
{
  return CoreRoot_Initialize((CoreRoot)_this, aSize );
}

/* 'C' function for method : 'Core::Root.SetRootFocus()' */
XBool CoreRoot_SetRootFocus( CoreRoot _this, XBool aHasRootFocus )
{
  if ( aHasRootFocus == _this->hasRootFocus )
    return 0;

  _this->hasRootFocus = aHasRootFocus;

  if ( !aHasRootFocus )
  {
    if ( _this->VirtualKeyboard != 0 )
      CoreView__ChangeViewState( _this->VirtualKeyboard, 0, CoreViewStateFocused );

    if ( _this->modalGroups != 0 )
      CoreView__ChangeViewState( _this->modalGroups->group, 0, CoreViewStateFocused );
    else
      CoreView__ChangeViewState( _this, 0, CoreViewStateFocused );
  }
  else
  {
    if ( _this->modalGroups != 0 )
      CoreView__ChangeViewState( _this->modalGroups->group, CoreViewStateFocused, 
      0 );
    else
      CoreView__ChangeViewState( _this, CoreViewStateFocused, 0 );

    if ( _this->VirtualKeyboard != 0 )
      CoreView__ChangeViewState( _this->VirtualKeyboard, CoreViewStateFocused, 0 );
  }

  return 1;
}

/* Wrapper function for the non virtual method : 'Core::Root.SetRootFocus()' */
XBool CoreRoot__SetRootFocus( void* _this, XBool aHasRootFocus )
{
  return CoreRoot_SetRootFocus((CoreRoot)_this, aHasRootFocus );
}

/* The method 'SetUserInputTimestamp()' exists for optional purpose to provide the 
   exact timestamp when the last user interaction took place. With this exact information 
   the GUI application can calculate better interactions like touch screen gestures, 
   etc. The time is expressed in milliseconds.
   Usually the method will be invoked in response to user input events received 
   in the main() message loop from the target specific keyboard or touch screen 
   driver. The method should be called before the event is fed to the GUI system. */
void CoreRoot_SetUserInputTimestamp( CoreRoot _this, XUInt32 aTimestamp )
{
  _this->currentEventTimestamp = aTimestamp;
}

/* Wrapper function for the non virtual method : 'Core::Root.SetUserInputTimestamp()' */
void CoreRoot__SetUserInputTimestamp( void* _this, XUInt32 aTimestamp )
{
  CoreRoot_SetUserInputTimestamp((CoreRoot)_this, aTimestamp );
}

/* The method DriveKeyboardHitting() exists for the integration purpose with the 
   underlying target system. Usually the method will be invoked in response to events 
   received in the main() message loop from the target specific hardware keyboard 
   driver.
   The parameters passed to the method determine the event. aDown determines whether 
   the user has pressed (== 'true') or released (== 'false') a key. The parameters 
   aCode and aCharCode are used exclusively. They determine the affected key. If 
   the method is called with a valid aCode then aCharCode should be '\0'. If the 
   method is called with a valid aCharCode then aCode should be Core::KeyCode.NoKey. */
XBool CoreRoot_DriveKeyboardHitting( CoreRoot _this, XEnum aCode, XChar aCharCode, 
  XBool aDown )
{
  XBool processed = (XBool)( _this->keyLastTarget != 0 );

  if (( _this->keyLastTarget != 0 ) && (( !aDown || ( _this->keyLastCode != aCode )) 
      || ( _this->keyLastCharCode != aCharCode )))
  {
    CoreKeyEvent event = 0;
    CoreView view = EwCastObject( _this->keyLastTarget, CoreView );
    CoreKeyPressHandler handler = EwCastObject( _this->keyLastTarget, CoreKeyPressHandler );

    if ( _this->keyLastCode != CoreKeyCodeNoKey )
      event = CoreKeyEvent_Initialize( EwNewObject( CoreKeyEvent, 0 ), _this->keyLastCode, 
      0 );

    if ( _this->keyLastCharCode != 0x0000 )
      event = CoreKeyEvent_Initialize2( EwNewObject( CoreKeyEvent, 0 ), _this->keyLastCharCode, 
      0 );

    if ( handler != 0 )
      CoreKeyPressHandler_HandleEvent( handler, event );
    else
      if ( view != 0 )
        CoreView__HandleEvent( view, ((CoreEvent)event ));

    _this->keyLastCode = CoreKeyCodeNoKey;
    _this->keyLastCharCode = 0x0000;
    _this->keyLastTarget = 0;
  }

  if ( _this->keyLastTarget != 0 )
  {
    CoreKeyEvent event = 0;
    CoreView view = EwCastObject( _this->keyLastTarget, CoreView );
    CoreKeyPressHandler handler = EwCastObject( _this->keyLastTarget, CoreKeyPressHandler );

    if ( aCode != CoreKeyCodeNoKey )
      event = CoreKeyEvent_Initialize( EwNewObject( CoreKeyEvent, 0 ), aCode, 1 );

    if ( _this->keyLastCharCode != 0x0000 )
      event = CoreKeyEvent_Initialize2( EwNewObject( CoreKeyEvent, 0 ), aCharCode, 
      1 );

    if ( handler != 0 )
      CoreKeyPressHandler_HandleEvent( handler, event );
    else
      if ( view != 0 )
        CoreView__HandleEvent( view, ((CoreEvent)event ));
  }

  if ( _this->keyLastLocked && (( !aDown || ( _this->keyLastCode != aCode )) || 
      ( _this->keyLastCharCode != aCharCode )))
  {
    _this->keyLastCode = CoreKeyCodeNoKey;
    _this->keyLastCharCode = 0x0000;
    _this->keyLastLocked = 0;
  }

  if ((( _this->keyLastTarget == 0 ) && aDown ) && ( _this->animationLock > 0 ))
  {
    _this->keyLastCode = aCode;
    _this->keyLastCharCode = aCharCode;
    _this->keyLastLocked = 1;
  }

  if ((( _this->keyLastTarget == 0 ) && aDown ) && !_this->keyLastLocked )
  {
    if ( aCode != CoreKeyCodeNoKey )
      _this->keyLastTarget = CoreGroup__DispatchEvent( _this, ((CoreEvent)CoreKeyEvent_Initialize( 
      EwNewObject( CoreKeyEvent, 0 ), aCode, 1 )));

    if ( aCharCode != 0x0000 )
      _this->keyLastTarget = CoreGroup__DispatchEvent( _this, ((CoreEvent)CoreKeyEvent_Initialize2( 
      EwNewObject( CoreKeyEvent, 0 ), aCharCode, 1 )));

    if (( EwCastObject( _this->keyLastTarget, CoreKeyPressHandler ) == 0 ) && ( 
        EwCastObject( _this->keyLastTarget, CoreView ) == 0 ))
      _this->keyLastTarget = 0;

    _this->keyLastCode = aCode;
    _this->keyLastCharCode = aCharCode;
    processed = (XBool)( processed || ( _this->keyLastTarget != 0 ));
  }

  _this->currentEventTimestamp = 0;
  return processed;
}

/* Wrapper function for the non virtual method : 'Core::Root.DriveKeyboardHitting()' */
XBool CoreRoot__DriveKeyboardHitting( void* _this, XEnum aCode, XChar aCharCode, 
  XBool aDown )
{
  return CoreRoot_DriveKeyboardHitting((CoreRoot)_this, aCode, aCharCode, aDown );
}

/* The method DriveCursorMovement() exists for the integration purpose with the 
   underlying target system. Usually the method will be invoked in response to to 
   drag events received in the main() message loop from the target specific touch 
   screen driver.
   The method expects only one parameter aPos. This parameter determines the current 
   cursor (or finger in the touch screen analogy) position. aPos lies relative to 
   the top-left corner of the application coordinate system.
   Please note, this method is limited to process a single touch (or mouse) event 
   at the same time. When integrating with a target system supporting multi-touch 
   please use the method @DriveMultiTouchMovement(). */
XBool CoreRoot_DriveCursorMovement( CoreRoot _this, XPoint aPos )
{
  return CoreRoot_DriveMultiTouchMovement( _this, _this->cursorFinger, aPos );
}

/* Wrapper function for the non virtual method : 'Core::Root.DriveCursorMovement()' */
XBool CoreRoot__DriveCursorMovement( void* _this, XPoint aPos )
{
  return CoreRoot_DriveCursorMovement((CoreRoot)_this, aPos );
}

/* The method DriveMultiTouchMovement() exists for the integration purpose with 
   the underlying target system. Usually the method will be invoked in response 
   to to drag events received in the main() message loop from the target specific 
   touch screen driver.
   The parameter aPos determines the current cursor (or finger in the touch screen 
   analogy) position. aPos lies relative to the top-left corner of the application 
   coordinate system. The number of the finger is specified in the parameter aFinger. 
   The first finger has the number 0, the second 1, and so far till 9.
   Please note, unlike the method @DriveCursorMovement() this method is able to 
   process and dispatch several multi-touch events simultaneously. */
XBool CoreRoot_DriveMultiTouchMovement( CoreRoot _this, XInt32 aFinger, XPoint aPos )
{
  XPoint offset;
  XPoint pos;
  CoreGroup grp;

  if (( aFinger < 0 ) || ( aFinger > 9 ))
  {
    _this->currentEventTimestamp = 0;
    return 0;
  }

  offset = EwMovePointNeg( aPos, _this->cursorLastPos[ EwCheckIndex( aFinger, 10 )]);
  _this->cursorLastPos[ EwCheckIndex( aFinger, 10 )] = aPos;

  if (( _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )] == 0 ) || !EwCompPoint( 
      offset, _Const0000 ))
  {
    _this->currentEventTimestamp = 0;
    return 0;
  }

  pos = aPos;
  grp = _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )]->Owner;

  while (( grp != 0 ) && ((CoreRoot)grp != _this ))
  {
    pos = EwMovePointNeg( pos, grp->Super1.Bounds.Point1 );
    grp = grp->Super2.Owner;
  }

  if (( grp == 0 ) && ((CoreRoot)_this->cursorTargetView[ EwCheckIndex( aFinger, 
      10 )] != _this ))
  {
    CoreView tmp = _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )];
    _this->cursorFinger = aFinger;
    _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )] = 0;
    CoreView__HandleEvent( tmp, ((CoreEvent)CoreCursorEvent_InitializeUp( CoreRoot_createCursorEvent( 
    _this ), aFinger, _this->cursorCurrentPos[ EwCheckIndex( aFinger, 10 )], _this->cursorHittingPos[ 
    EwCheckIndex( aFinger, 10 )], _this->cursorHoldPeriod[ EwCheckIndex( aFinger, 
    10 )], _this->cursorSequelCounter[ EwCheckIndex( aFinger, 10 )] + 1, _this->cursorHitOffset[ 
    EwCheckIndex( aFinger, 10 )], 0, _this->cursorLastPos[ EwCheckIndex( aFinger, 
    10 )], _this->cursorFirstPos[ EwCheckIndex( aFinger, 10 )])));

    if ( tmp == _this->cursorCaptureView )
      _this->cursorCaptureView = 0;

    CoreGroup__BroadcastEvent( _this, ((CoreEvent)CoreCursorGrabEvent_InitializeUp( 
    CoreRoot_createCursorGrabEvent( _this ), aFinger, _this->cursorSequelCounter[ 
    EwCheckIndex( aFinger, 10 )] + 1, 0, tmp, aPos )), CoreViewStateEnabled | CoreViewStateTouchable );
  }
  else
  {
    _this->cursorCurrentPos[ EwCheckIndex( aFinger, 10 )] = pos;
    _this->cursorFinger = aFinger;
    CoreView__HandleEvent( _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )], 
    ((CoreEvent)CoreDragEvent_Initialize( CoreRoot_createDragEvent( _this ), aFinger, 
    pos, _this->cursorHittingPos[ EwCheckIndex( aFinger, 10 )], offset, _this->cursorHoldPeriod[ 
    EwCheckIndex( aFinger, 10 )], _this->cursorSequelCounter[ EwCheckIndex( aFinger, 
    10 )] + 1, _this->cursorHitOffset[ EwCheckIndex( aFinger, 10 )], aPos, _this->cursorFirstPos[ 
    EwCheckIndex( aFinger, 10 )])));
  }

  _this->currentEventTimestamp = 0;
  return 1;
}

/* Wrapper function for the non virtual method : 'Core::Root.DriveMultiTouchMovement()' */
XBool CoreRoot__DriveMultiTouchMovement( void* _this, XInt32 aFinger, XPoint aPos )
{
  return CoreRoot_DriveMultiTouchMovement((CoreRoot)_this, aFinger, aPos );
}

/* The method DriveCursorHitting() exists for the integration purpose with the underlying 
   target system. Usually the method will be invoked in response to to touch events 
   received in the main() message loop from the target specific touch screen driver.
   The parameters passed to the method determine the event. aDown determines whether 
   the user has pressed (== 'true') or released (== 'false') his finger at the position 
   aPos relative to the top-left origin of the GUI application area. The parameter 
   aFinger specifies the finger (or mouse button) the user pressed for this operation. 
   The first finger (mouse button) has the number 0, the second 1, and so far.
   Please note, this method is limited to process a single touch (or mouse) event 
   at the same time. When integrating with a target system supporting multi-touch 
   please use the method @DriveMultiTouchHitting(). */
XBool CoreRoot_DriveCursorHitting( CoreRoot _this, XBool aDown, XInt32 aFinger, 
  XPoint aPos )
{
  return CoreRoot_DriveMultiTouchHitting( _this, aDown, aFinger, aPos );
}

/* Wrapper function for the non virtual method : 'Core::Root.DriveCursorHitting()' */
XBool CoreRoot__DriveCursorHitting( void* _this, XBool aDown, XInt32 aFinger, XPoint 
  aPos )
{
  return CoreRoot_DriveCursorHitting((CoreRoot)_this, aDown, aFinger, aPos );
}

/* The method DriveMultiTouchHitting() exists for the integration purpose with the 
   underlying target system. Usually the method will be invoked in response to touch 
   events received in the main() message loop from the target specific touch screen 
   driver.
   The parameters passed to the method determine the event. aDown determines whether 
   the user has pressed (== 'true') or released (== 'false') his finger at the position 
   aPos relative to the top-left origin of the GUI application area. The parameter 
   aFinger identifies the associated finger. The first finger has the number 0, 
   the second 1 and so far till 9. 
   Please note, unlike the method @DriveCursorHitting() this method is able to process 
   and dispatch several multi-touch events simultaneously. */
XBool CoreRoot_DriveMultiTouchHitting( CoreRoot _this, XBool aDown, XInt32 aFinger, 
  XPoint aPos )
{
  XUInt32 ticksCount;
  XRect dragLimit;
  XUInt32 savedCurrentEventTimestamp;

  if (( aFinger < 0 ) || ( aFinger > 9 ))
  {
    _this->currentEventTimestamp = 0;
    return 0;
  }

  ticksCount = _this->currentEventTimestamp;
  dragLimit = EwNewRect2Point( EwNewPoint( -_this->CursorDragLimit, -_this->CursorDragLimit ), 
  EwNewPoint( _this->CursorDragLimit + 1, _this->CursorDragLimit + 1 ));

  if ( ticksCount == 0 )
  {
    ticksCount = (XUInt32)EwGetTicks();
  }

  savedCurrentEventTimestamp = _this->currentEventTimestamp;
  CoreRoot_DriveMultiTouchMovement( _this, aFinger, aPos );
  aPos = _this->cursorLastPos[ EwCheckIndex( aFinger, 10 )];
  _this->currentEventTimestamp = savedCurrentEventTimestamp;

  if ( aDown )
    _this->cursorFirstPos[ EwCheckIndex( aFinger, 10 )] = aPos;

  if (( aDown && ( _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )] == 0 )) 
      && ( _this->animationLock == 0 ))
  {
    CoreCursorHit hit = 0;
    XPoint pos = aPos;
    CoreGroup grp;

    if ( EwIsPointInRect( _this->cursorSequelArea[ EwCheckIndex( aFinger, 10 )], 
        aPos ) && (( ticksCount - _this->cursorHittingTime[ EwCheckIndex( aFinger, 
        10 )]) <= (XUInt32)_this->CursorSequelDelay ))
      _this->cursorSequelCounter[ EwCheckIndex( aFinger, 10 )] = _this->cursorSequelCounter[ 
      EwCheckIndex( aFinger, 10 )] + 1;
    else
      _this->cursorSequelCounter[ EwCheckIndex( aFinger, 10 )] = 0;

    _this->cursorSequelArea[ EwCheckIndex( aFinger, 10 )] = EwMoveRectPos( dragLimit, 
    aPos );
    _this->cursorHittingTime[ EwCheckIndex( aFinger, 10 )] = ticksCount;

    if ((( _this->VirtualKeyboard != 0 ) && ( _this->VirtualKeyboard->Super2.Owner 
        != 0 )) && (( _this->VirtualKeyboard->Super2.viewState & ( CoreViewStateEnabled 
        | CoreViewStateTouchable )) == ( CoreViewStateEnabled | CoreViewStateTouchable )))
    {
      XRect area = EwMoveRectPos( dragLimit, CoreGroup_LocalPosition( _this->VirtualKeyboard->Super2.Owner, 
        aPos ));
      hit = CoreView__CursorHitTest( _this->VirtualKeyboard, area, aFinger, _this->cursorSequelCounter[ 
      EwCheckIndex( aFinger, 10 )] + 1, 0, 0 );
    }

    if ( hit == 0 )
    {
      if (( _this->cursorCaptureView != 0 ) && ( _this->cursorCaptureView->Owner 
          != 0 ))
      {
        if ((( _this->cursorCaptureView->viewState & CoreViewStateTouchable ) == 
            CoreViewStateTouchable ) && (( _this->cursorCaptureView->viewState & 
            CoreViewStateEnabled ) == CoreViewStateEnabled ))
        {
          XRect area = EwMoveRectPos( dragLimit, CoreGroup_LocalPosition( _this->cursorCaptureView->Owner, 
            aPos ));
          hit = CoreView__CursorHitTest( _this->cursorCaptureView, area, aFinger, 
          _this->cursorSequelCounter[ EwCheckIndex( aFinger, 10 )] + 1, 0, 0 );
        }
      }
      else
        if ( _this->modalGroups != 0 )
          hit = CoreView__CursorHitTest( _this, EwMoveRectPos( dragLimit, aPos ), 
          aFinger, _this->cursorSequelCounter[ EwCheckIndex( aFinger, 10 )] + 1, 
          ((CoreView)_this->modalGroups->group ), 0 );
        else
          hit = CoreView__CursorHitTest( _this, EwMoveRectPos( dragLimit, aPos ), 
          aFinger, _this->cursorSequelCounter[ EwCheckIndex( aFinger, 10 )] + 1, 
          0, 0 );
    }

    if ( hit != 0 )
    {
      CoreGroup__BroadcastEvent( _this, ((CoreEvent)CoreCursorGrabEvent_InitializeDown( 
      CoreRoot_createCursorGrabEvent( _this ), aFinger, _this->cursorSequelCounter[ 
      EwCheckIndex( aFinger, 10 )] + 1, 0, hit->View, aPos )), CoreViewStateEnabled 
      | CoreViewStateTouchable );
      _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )] = hit->View;
      _this->cursorHitOffset[ EwCheckIndex( aFinger, 10 )] = hit->Offset;
    }
    else
    {
      _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )] = 0;
      _this->cursorHitOffset[ EwCheckIndex( aFinger, 10 )] = _Const0000;
      _this->currentEventTimestamp = 0;
      return 0;
    }

    grp = hit->View->Owner;

    while (( grp != 0 ) && ((CoreRoot)grp != _this ))
    {
      pos = EwMovePointNeg( pos, grp->Super1.Bounds.Point1 );
      grp = grp->Super2.Owner;
    }

    _this->cursorHittingPos[ EwCheckIndex( aFinger, 10 )] = pos;
    _this->cursorCurrentPos[ EwCheckIndex( aFinger, 10 )] = pos;
    _this->cursorHoldPeriod[ EwCheckIndex( aFinger, 10 )] = 0;
    CoreTimer_OnSetEnabled( &_this->cursorHoldTimer, 1 );
    _this->cursorFinger = aFinger;
    CoreView__HandleEvent( _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )], 
    ((CoreEvent)CoreCursorEvent_InitializeDown( CoreRoot_createCursorEvent( _this ), 
    aFinger, pos, _this->cursorSequelCounter[ EwCheckIndex( aFinger, 10 )] + 1, 
    _this->cursorHitOffset[ EwCheckIndex( aFinger, 10 )], 0, aPos )));
    _this->currentEventTimestamp = 0;
    return 1;
  }

  if ( !aDown && ( _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )] != 0 ))
  {
    XPoint pos = aPos;
    CoreGroup grp = _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )]->Owner;
    CoreView tmp;

    while (( grp != 0 ) && ((CoreRoot)grp != _this ))
    {
      pos = EwMovePointNeg( pos, grp->Super1.Bounds.Point1 );
      grp = grp->Super2.Owner;
    }

    if ( grp == 0 )
      pos = _this->cursorCurrentPos[ EwCheckIndex( aFinger, 10 )];

    _this->cursorFinger = aFinger;
    tmp = _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )];
    _this->cursorTargetView[ EwCheckIndex( aFinger, 10 )] = 0;
    CoreView__HandleEvent( tmp, ((CoreEvent)CoreCursorEvent_InitializeUp( CoreRoot_createCursorEvent( 
    _this ), aFinger, pos, _this->cursorHittingPos[ EwCheckIndex( aFinger, 10 )], 
    _this->cursorHoldPeriod[ EwCheckIndex( aFinger, 10 )], _this->cursorSequelCounter[ 
    EwCheckIndex( aFinger, 10 )] + 1, _this->cursorHitOffset[ EwCheckIndex( aFinger, 
    10 )], 0, aPos, _this->cursorFirstPos[ EwCheckIndex( aFinger, 10 )])));
    CoreGroup__BroadcastEvent( _this, ((CoreEvent)CoreCursorGrabEvent_InitializeUp( 
    CoreRoot_createCursorGrabEvent( _this ), aFinger, _this->cursorSequelCounter[ 
    EwCheckIndex( aFinger, 10 )] + 1, 0, tmp, aPos )), CoreViewStateEnabled | CoreViewStateTouchable );
    _this->currentEventTimestamp = 0;
    return 1;
  }

  _this->currentEventTimestamp = 0;
  return 0;
}

/* Wrapper function for the non virtual method : 'Core::Root.DriveMultiTouchHitting()' */
XBool CoreRoot__DriveMultiTouchHitting( void* _this, XBool aDown, XInt32 aFinger, 
  XPoint aPos )
{
  return CoreRoot_DriveMultiTouchHitting((CoreRoot)_this, aDown, aFinger, aPos );
}

/* The method RetargetCursorWithReason() changes the currently active cursor event 
   target view. Usually, the target view is determined when the user presses the 
   finger on the touch screen. Once determined, the target view remains active until 
   the user has released the finger. In the meantime the framework will provide 
   this target view with all cursor events. This entire cycle is called 'grab cycle'. 
   The method RetargetCursorWithReason() allows you to select a new target view 
   without the necessity to wait until the user has released the touch screen and 
   thus has finalized the grab cycle.
   Except the additional parameter aRetargetReason, this method works similarly 
   to @RetargetCursor(). At first the method asks the new potential target view 
   aNewTarget whether it or one of its sub-views is willing to continue processing 
   the cursor events for the gesture specified in aRetargetReason. If successful, 
   the method hands over the cursor event flow to this determined view. If there 
   is no view willing to handle these events, the method hands over the event flow 
   directly to the view specified in the parameter aFallbackTarget. If no willing 
   view could be found and no fall-back view was given, the current grab cycle is 
   finalized as if the user had released the touch screen.
   Unlike the method @DeflectCursor() this RetargetCursorWithReason() method performs 
   the cursor hit test for all views of the new potential target. This is as if 
   the user had tapped the screen and the framework tries to determine the view 
   affected by this interaction. This search operation is limited to views at the 
   current cursor position. Unlike @RetargetCursor(), this method limits additionally 
   to candidates willing to handle the gesture specified in the parameter aRetargetReason.
   When switching the target view, the framework provides the old and the new target 
   views with cursor events. The old view will receive a Core::CursorEvent with 
   variables Down == 'false' and AutoDeflected == 'true' - this simulates the release 
   operations. The new target view will receive a Core::CursorEvent with the variable 
   Down == 'true' as if it had been just touched by the user.
   If the application is running within a multi-touch environment, the invocation 
   of the RetargetCursor() method does affect the event flow corresponding only 
   to the finger which has lastly generated touch events. */
void CoreRoot_RetargetCursorWithReason( CoreRoot _this, CoreView aNewTarget, CoreView 
  aFallbackTarget, XSet aRetargetReason )
{
  XRect dragLimit = EwNewRect2Point( EwNewPoint( -_this->CursorDragLimit, -_this->CursorDragLimit ), 
    EwNewPoint( _this->CursorDragLimit + 1, _this->CursorDragLimit + 1 ));
  CoreCursorHit hit;

  if ((CoreRoot)aNewTarget == _this )
    aNewTarget = 0;

  if ( _this->cursorTargetView[ EwCheckIndex( _this->cursorFinger, 10 )] == 0 )
    return;

  hit = CoreView__CursorHitTest( _this, EwMoveRectPos( dragLimit, _this->cursorLastPos[ 
  EwCheckIndex( _this->cursorFinger, 10 )]), _this->cursorFinger, 1, aNewTarget, 
  aRetargetReason );

  if (( hit != 0 ) && ( _this->cursorTargetView[ EwCheckIndex( _this->cursorFinger, 
      10 )] != hit->View ))
    CoreRoot_DeflectCursor( _this, hit->View, hit->Offset );

  if (( hit == 0 ) && ( _this->cursorTargetView[ EwCheckIndex( _this->cursorFinger, 
      10 )] != aFallbackTarget ))
    CoreRoot_DeflectCursor( _this, aFallbackTarget, _Const0000 );
}

/* The method DeflectCursor() changes the currently active cursor event target view. 
   Usually, the target view is determined when the user presses the finger on the 
   touch screen. Once determined, the target view remains active until the user 
   has released the finger. In the meantime the framework will provide this target 
   view with all cursor events. This entire cycle is called 'grab cycle'. The method 
   DeflectCursor() allows you to select a new target view without the necessity 
   to wait until the user has released the touch screen and thus has finalized the 
   grab cycle.
   Unlike the method @RetargetCursor() this @DeflectCursor() method hands over the 
   cursor event flow to the new target regardless its position or state. If no new 
   target view has been specified, the current grab cycle is finalized as if the 
   user had released the touch screen.
   When switching the target view, the framework provides the old and the new target 
   views with cursor events. The old view will receive a Core::CursorEvent with 
   variables Down == 'false' and AutoDeflected == 'true' - this simulates the release 
   operations. The new target view will receive a Core::CursorEvent with the variable 
   Down == 'true' as if it had been just touched by the user.
   If the application is running within a multi-touch environment, the invocation 
   of the DeflectCursor() method does affect the event flow corresponding only to 
   the finger which has lastly generated touch events. */
void CoreRoot_DeflectCursor( CoreRoot _this, CoreView aNewTarget, XPoint aHitOffset )
{
  CoreView tmp;
  XPoint pos;
  CoreGroup grp;
  XUInt32 ticksCount;

  if (( _this->cursorTargetView[ EwCheckIndex( _this->cursorFinger, 10 )] == 0 ) 
      || ( _this->cursorTargetView[ EwCheckIndex( _this->cursorFinger, 10 )] == 
      aNewTarget ))
    return;

  tmp = _this->cursorTargetView[ EwCheckIndex( _this->cursorFinger, 10 )];
  _this->cursorTargetView[ EwCheckIndex( _this->cursorFinger, 10 )] = 0;
  CoreView__HandleEvent( tmp, ((CoreEvent)CoreCursorEvent_InitializeUp( CoreRoot_createCursorEvent( 
  _this ), _this->cursorFinger, _this->cursorCurrentPos[ EwCheckIndex( _this->cursorFinger, 
  10 )], _this->cursorHittingPos[ EwCheckIndex( _this->cursorFinger, 10 )], _this->cursorHoldPeriod[ 
  EwCheckIndex( _this->cursorFinger, 10 )], _this->cursorSequelCounter[ EwCheckIndex( 
  _this->cursorFinger, 10 )] + 1, _this->cursorHitOffset[ EwCheckIndex( _this->cursorFinger, 
  10 )], 1, _this->cursorLastPos[ EwCheckIndex( _this->cursorFinger, 10 )], _this->cursorFirstPos[ 
  EwCheckIndex( _this->cursorFinger, 10 )])));
  CoreGroup__BroadcastEvent( _this, ((CoreEvent)CoreCursorGrabEvent_InitializeUp( 
  CoreRoot_createCursorGrabEvent( _this ), _this->cursorFinger, _this->cursorSequelCounter[ 
  EwCheckIndex( _this->cursorFinger, 10 )] + 1, 1, tmp, _this->cursorLastPos[ EwCheckIndex( 
  _this->cursorFinger, 10 )])), CoreViewStateEnabled | CoreViewStateTouchable );
  pos = _this->cursorLastPos[ EwCheckIndex( _this->cursorFinger, 10 )];
  grp = 0;

  if ( aNewTarget != 0 )
    grp = aNewTarget->Owner;

  while (( grp != 0 ) && ((CoreRoot)grp != _this ))
  {
    pos = EwMovePointNeg( pos, grp->Super1.Bounds.Point1 );
    grp = grp->Super2.Owner;
  }

  if (( grp == 0 ) && ((CoreRoot)aNewTarget != _this ))
  {
    _this->cursorTargetView[ EwCheckIndex( _this->cursorFinger, 10 )] = 0;
    CoreTimer_OnSetEnabled( &_this->cursorHoldTimer, 0 );
    return;
  }

  CoreGroup__BroadcastEvent( _this, ((CoreEvent)CoreCursorGrabEvent_InitializeDown( 
  CoreRoot_createCursorGrabEvent( _this ), _this->cursorFinger, _this->cursorSequelCounter[ 
  EwCheckIndex( _this->cursorFinger, 10 )] + 1, 1, aNewTarget, _this->cursorLastPos[ 
  EwCheckIndex( _this->cursorFinger, 10 )])), CoreViewStateEnabled | CoreViewStateTouchable );
  ticksCount = 0;
  ticksCount = (XUInt32)EwGetTicks();
  _this->cursorTargetView[ EwCheckIndex( _this->cursorFinger, 10 )] = aNewTarget;
  _this->cursorHitOffset[ EwCheckIndex( _this->cursorFinger, 10 )] = aHitOffset;
  _this->cursorHittingPos[ EwCheckIndex( _this->cursorFinger, 10 )] = pos;
  _this->cursorCurrentPos[ EwCheckIndex( _this->cursorFinger, 10 )] = pos;
  _this->cursorSequelCounter[ EwCheckIndex( _this->cursorFinger, 10 )] = 0;
  _this->cursorHoldPeriod[ EwCheckIndex( _this->cursorFinger, 10 )] = 0;
  _this->cursorHittingTime[ EwCheckIndex( _this->cursorFinger, 10 )] = ticksCount;
  _this->cursorFirstPos[ EwCheckIndex( _this->cursorFinger, 10 )] = _this->cursorLastPos[ 
  EwCheckIndex( _this->cursorFinger, 10 )];
  CoreView__HandleEvent( _this->cursorTargetView[ EwCheckIndex( _this->cursorFinger, 
  10 )], ((CoreEvent)CoreCursorEvent_InitializeDown( CoreRoot_createCursorEvent( 
  _this ), _this->cursorFinger, pos, _this->cursorSequelCounter[ EwCheckIndex( _this->cursorFinger, 
  10 )] + 1, _this->cursorHitOffset[ EwCheckIndex( _this->cursorFinger, 10 )], 1, 
  _this->cursorFirstPos[ EwCheckIndex( _this->cursorFinger, 10 )])));
}

/* Variants derived from the class : 'Core::Root' */
EW_DEFINE_CLASS_VARIANTS( CoreRoot )
EW_END_OF_CLASS_VARIANTS( CoreRoot )

/* Virtual Method Table (VMT) for the class : 'Core::Root' */
EW_DEFINE_CLASS( CoreRoot, CoreGroup, keyLastTarget, cursorHoldTimer, cursorHoldTimer, 
                 cursorHoldTimer, updateLock, "Core::Root" )
  CoreRectView_initLayoutContext,
  CoreRoot_GetRoot,
  CoreRoot_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreRoot_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreRoot_OnSetFocus,
  CoreRoot_DispatchEvent,
  CoreRoot_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreRoot_InvalidateArea,
  CoreRoot_Restack,
  CoreRoot_Add,
EW_END_OF_CLASS( CoreRoot )

/* Initializer for the class 'Core::Event' */
void CoreEvent__Init( CoreEvent _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreEvent );

  /* Call the user defined constructor */
  CoreEvent_Init( _this, aArg );
}

/* Re-Initializer for the class 'Core::Event' */
void CoreEvent__ReInit( CoreEvent _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::Event' */
void CoreEvent__Done( CoreEvent _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreEvent );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* The method 'GetCurrentTime()' returns the current time expressed in milliseconds. 
   The value can be used e.g. to calculate the time span elapsed since the event 
   was generated (see @Time). */
XUInt32 CoreEvent_GetCurrentTime( CoreEvent _this )
{
  XUInt32 ticksCount;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );

  ticksCount = 0;
  ticksCount = (XUInt32)EwGetTicks();
  return ticksCount;
}

/* 'C' function for method : 'Core::Event.Init()' */
void CoreEvent_Init( CoreEvent _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  _this->Time = CoreEvent_GetCurrentTime( _this );
}

/* Variants derived from the class : 'Core::Event' */
EW_DEFINE_CLASS_VARIANTS( CoreEvent )
EW_END_OF_CLASS_VARIANTS( CoreEvent )

/* Virtual Method Table (VMT) for the class : 'Core::Event' */
EW_DEFINE_CLASS( CoreEvent, XObject, _None, _None, _None, _None, _None, "Core::Event" )
EW_END_OF_CLASS( CoreEvent )

/* Initializer for the class 'Core::KeyEvent' */
void CoreKeyEvent__Init( CoreKeyEvent _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreEvent__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreKeyEvent );
}

/* Re-Initializer for the class 'Core::KeyEvent' */
void CoreKeyEvent__ReInit( CoreKeyEvent _this )
{
  /* At first re-initialize the super class ... */
  CoreEvent__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::KeyEvent' */
void CoreKeyEvent__Done( CoreKeyEvent _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreKeyEvent );

  /* Don't forget to deinitialize the super class ... */
  CoreEvent__Done( &_this->_Super );
}

/* The method Initialize2() initializes this Core::KeyEvent object with the given 
   parameters. This method stores the UNICODE character code aCode in the variable 
   @CharCode and returns this event object to the caller. The @Code variable will 
   be initialized with the Core::KeyCode.NoKey value unless a '0' .. '9', 'A' .. 
   'Z' character code is passed to this method. In this case the variable @Code 
   is initialized with the corresponding key code Key0 .. Key9 or KeyA .. KeyZ. */
CoreKeyEvent CoreKeyEvent_Initialize2( CoreKeyEvent _this, XChar aCode, XBool aDown )
{
  _this->Code = CoreKeyCodeNoKey;
  _this->CharCode = aCode;
  _this->Down = aDown;

  if (( aCode >= '0' ) && ( aCode <= '9' ))
    _this->Code = (XEnum)(( 10 + aCode ) - 48 );

  if (( aCode >= 'A' ) && ( aCode <= 'Z' ))
    _this->Code = (XEnum)(( 105 + aCode ) - 65 );

  if (( aCode >= 'a' ) && ( aCode <= 'z' ))
    _this->Code = (XEnum)(( 105 + aCode ) - 97 );

  if ( aCode == 0x0020 )
    _this->Code = CoreKeyCodeSpace;

  if ( _this->Code == CoreKeyCodeNoKey )
    switch ( aCode )
    {
      case '+' :
        _this->Code = CoreKeyCodePlus;
      break;

      case '-' :
        _this->Code = CoreKeyCodeMinus;
      break;

      case '*' :
        _this->Code = CoreKeyCodeMultiply;
      break;

      case '/' :
        _this->Code = CoreKeyCodeDivide;
      break;

      case '=' :
        _this->Code = CoreKeyCodeEquals;
      break;

      case '.' :
        _this->Code = CoreKeyCodePeriod;
      break;

      case ',' :
        _this->Code = CoreKeyCodeComma;
      break;

      case ':' :
        _this->Code = CoreKeyCodeColon;
      break;

      case ';' :
        _this->Code = CoreKeyCodeSemicolon;
      break;

      default : 
        ;
    }

  return _this;
}

/* Wrapper function for the non virtual method : 'Core::KeyEvent.Initialize2()' */
CoreKeyEvent CoreKeyEvent__Initialize2( void* _this, XChar aCode, XBool aDown )
{
  return CoreKeyEvent_Initialize2((CoreKeyEvent)_this, aCode, aDown );
}

/* The method Initialize() initializes this Core::KeyEvent object with the given 
   parameters. This method stores the key code aCode in the variable @Code and returns 
   this event object to the caller. The @CharCode variable will be initialized with 
   the zero character '\x0000' unless a Key0 .. Key9; KeyA .. KeyZ code is passed 
   to this method. In this case the variable @CharCode is initialized with the corresponding 
   UNICODE character code '0' .. '9' or 'A' .. 'Z'. */
CoreKeyEvent CoreKeyEvent_Initialize( CoreKeyEvent _this, XEnum aCode, XBool aDown )
{
  XInt32 cd;
  XInt32 ca;

  _this->Code = aCode;
  _this->Down = aDown;
  _this->CharCode = 0x0000;
  cd = aCode - 10;
  ca = aCode - 105;

  if (( cd >= 0 ) && ( cd <= 9 ))
    _this->CharCode = (XChar)( 48 + cd );

  if (( ca >= 0 ) && ( ca <= 25 ))
    _this->CharCode = (XChar)( 65 + ca );

  if ( aCode == CoreKeyCodeSpace )
    _this->CharCode = 0x0020;

  if ( _this->CharCode == 0x0000 )
    switch ( aCode )
    {
      case CoreKeyCodePlus :
        _this->CharCode = '+';
      break;

      case CoreKeyCodeMinus :
        _this->CharCode = '-';
      break;

      case CoreKeyCodeMultiply :
        _this->CharCode = '*';
      break;

      case CoreKeyCodeDivide :
        _this->CharCode = '/';
      break;

      case CoreKeyCodeEquals :
        _this->CharCode = '=';
      break;

      case CoreKeyCodePeriod :
        _this->CharCode = '.';
      break;

      case CoreKeyCodeComma :
        _this->CharCode = ',';
      break;

      case CoreKeyCodeColon :
        _this->CharCode = ':';
      break;

      case CoreKeyCodeSemicolon :
        _this->CharCode = ';';
      break;

      default : 
        ;
    }

  return _this;
}

/* Wrapper function for the non virtual method : 'Core::KeyEvent.Initialize()' */
CoreKeyEvent CoreKeyEvent__Initialize( void* _this, XEnum aCode, XBool aDown )
{
  return CoreKeyEvent_Initialize((CoreKeyEvent)_this, aCode, aDown );
}

/* The method IsCode() compares the key code stored within the event with the code 
   passed in the parameter aCodeOrCategory and returns 'true' or 'false' depending 
   on the result of this operation.
   The method is able to test whether a code does belong to a code category. Categories 
   are special codes defined in the Core::KeyCode enumeration, like AlphaKeys, DigitKeys 
   or CursorKeys. They stand for an entire range of key codes. */
XBool CoreKeyEvent_IsCode( CoreKeyEvent _this, XEnum aCodeOrCategory )
{
  switch ( aCodeOrCategory )
  {
    case CoreKeyCodeAlphaKeys :
      return (XBool)((( _this->CharCode >= 'A' ) && ( _this->CharCode <= 'Z' )) 
        || (( _this->CharCode >= 'a' ) && ( _this->CharCode <= 'z' )));

    case CoreKeyCodeAlphaOrDigitKeys :
      return (XBool)(((( _this->CharCode >= 'A' ) && ( _this->CharCode <= 'Z' )) 
        || (( _this->CharCode >= 'a' ) && ( _this->CharCode <= 'z' ))) || (( _this->CharCode 
        >= '0' ) && ( _this->CharCode <= '9' )));

    case CoreKeyCodeDigitKeys :
      return (XBool)(( _this->CharCode >= '0' ) && ( _this->CharCode <= '9' ));

    case CoreKeyCodeHexDigitKeys :
      return (XBool)(((( _this->CharCode >= 'A' ) && ( _this->CharCode <= 'F' )) 
        || (( _this->CharCode >= 'a' ) && ( _this->CharCode <= 'f' ))) || (( _this->CharCode 
        >= '0' ) && ( _this->CharCode <= '9' )));

    case CoreKeyCodeCharacterKeys :
      return (XBool)( _this->CharCode != 0x0000 );

    case CoreKeyCodeControlKeys :
      return (XBool)(( _this->CharCode == 0x0000 ) && ( _this->Code != CoreKeyCodeNoKey ));

    case CoreKeyCodeCursorKeys :
      return (XBool)(((( _this->Code == CoreKeyCodeLeft ) || ( _this->Code == CoreKeyCodeRight )) 
        || ( _this->Code == CoreKeyCodeUp )) || ( _this->Code == CoreKeyCodeDown ));

    case CoreKeyCodeAnyKey :
      return (XBool)(( _this->CharCode != 0x0000 ) || ( _this->Code != CoreKeyCodeNoKey ));

    default : 
      ;
  }

  return (XBool)( aCodeOrCategory == _this->Code );
}

/* Variants derived from the class : 'Core::KeyEvent' */
EW_DEFINE_CLASS_VARIANTS( CoreKeyEvent )
EW_END_OF_CLASS_VARIANTS( CoreKeyEvent )

/* Virtual Method Table (VMT) for the class : 'Core::KeyEvent' */
EW_DEFINE_CLASS( CoreKeyEvent, CoreEvent, _None, _None, _None, _None, _None, "Core::KeyEvent" )
EW_END_OF_CLASS( CoreKeyEvent )

/* Initializer for the class 'Core::CursorGrabEvent' */
void CoreCursorGrabEvent__Init( CoreCursorGrabEvent _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreEvent__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreCursorGrabEvent );
}

/* Re-Initializer for the class 'Core::CursorGrabEvent' */
void CoreCursorGrabEvent__ReInit( CoreCursorGrabEvent _this )
{
  /* At first re-initialize the super class ... */
  CoreEvent__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::CursorGrabEvent' */
void CoreCursorGrabEvent__Done( CoreCursorGrabEvent _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreCursorGrabEvent );

  /* Don't forget to deinitialize the super class ... */
  CoreEvent__Done( &_this->_Super );
}

/* The method InitializeUp() simplifies the initialization of this Core::CursorGrabEvent 
   object. This method stores the given parameters in the variables of this event 
   object, sets its variable @Down 'false' and returns this event object to the 
   caller. */
CoreCursorGrabEvent CoreCursorGrabEvent_InitializeUp( CoreCursorGrabEvent _this, 
  XInt32 aFinger, XInt32 aStrikeCount, XBool aAutoDeflected, CoreView aTarget, XPoint 
  aGlobalCurrentPos )
{
  _this->Down = 0;
  _this->Finger = aFinger;
  _this->StrikeCount = aStrikeCount;
  _this->GlobalCurrentPos = aGlobalCurrentPos;
  _this->Target = aTarget;
  _this->AutoDeflected = aAutoDeflected;
  return _this;
}

/* Wrapper function for the non virtual method : 'Core::CursorGrabEvent.InitializeUp()' */
CoreCursorGrabEvent CoreCursorGrabEvent__InitializeUp( void* _this, XInt32 aFinger, 
  XInt32 aStrikeCount, XBool aAutoDeflected, CoreView aTarget, XPoint aGlobalCurrentPos )
{
  return CoreCursorGrabEvent_InitializeUp((CoreCursorGrabEvent)_this, aFinger, aStrikeCount
  , aAutoDeflected, aTarget, aGlobalCurrentPos );
}

/* The method InitializeDown() simplifies the initialization of this Core::CursorGrabEvent 
   object. This method stores the given parameters in the variables of this event 
   object, sets its variable @Down 'true' and returns this event object to the caller. */
CoreCursorGrabEvent CoreCursorGrabEvent_InitializeDown( CoreCursorGrabEvent _this, 
  XInt32 aFinger, XInt32 aStrikeCount, XBool aAutoDeflected, CoreView aTarget, XPoint 
  aGlobalCurrentPos )
{
  _this->Down = 1;
  _this->Finger = aFinger;
  _this->StrikeCount = aStrikeCount;
  _this->GlobalCurrentPos = aGlobalCurrentPos;
  _this->Target = aTarget;
  _this->AutoDeflected = aAutoDeflected;
  return _this;
}

/* Wrapper function for the non virtual method : 'Core::CursorGrabEvent.InitializeDown()' */
CoreCursorGrabEvent CoreCursorGrabEvent__InitializeDown( void* _this, XInt32 aFinger, 
  XInt32 aStrikeCount, XBool aAutoDeflected, CoreView aTarget, XPoint aGlobalCurrentPos )
{
  return CoreCursorGrabEvent_InitializeDown((CoreCursorGrabEvent)_this, aFinger, 
  aStrikeCount, aAutoDeflected, aTarget, aGlobalCurrentPos );
}

/* Variants derived from the class : 'Core::CursorGrabEvent' */
EW_DEFINE_CLASS_VARIANTS( CoreCursorGrabEvent )
EW_END_OF_CLASS_VARIANTS( CoreCursorGrabEvent )

/* Virtual Method Table (VMT) for the class : 'Core::CursorGrabEvent' */
EW_DEFINE_CLASS( CoreCursorGrabEvent, CoreEvent, Target, GlobalCurrentPos, GlobalCurrentPos, 
                 GlobalCurrentPos, GlobalCurrentPos, "Core::CursorGrabEvent" )
EW_END_OF_CLASS( CoreCursorGrabEvent )

/* Initializer for the class 'Core::CursorEvent' */
void CoreCursorEvent__Init( CoreCursorEvent _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreEvent__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreCursorEvent );
}

/* Re-Initializer for the class 'Core::CursorEvent' */
void CoreCursorEvent__ReInit( CoreCursorEvent _this )
{
  /* At first re-initialize the super class ... */
  CoreEvent__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::CursorEvent' */
void CoreCursorEvent__Done( CoreCursorEvent _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreCursorEvent );

  /* Don't forget to deinitialize the super class ... */
  CoreEvent__Done( &_this->_Super );
}

/* The method InitializeHold() simplifies the initialization of this Core::CursorEvent 
   object. This method stores the given parameters in the variables of this event 
   object, sets its variable @Down 'true' and returns this event object to the caller. */
CoreCursorEvent CoreCursorEvent_InitializeHold( CoreCursorEvent _this, XInt32 aFinger, 
  XPoint aCurrentPos, XPoint aHittingPos, XInt32 aHoldPeriod, XInt32 aStrikeCount, 
  XPoint aHitOffset, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos )
{
  _this->Down = 1;
  _this->Finger = aFinger;
  _this->CurrentPos = EwMovePointPos( aCurrentPos, aHitOffset );
  _this->HittingPos = EwMovePointPos( aHittingPos, aHitOffset );
  _this->HoldPeriod = aHoldPeriod;
  _this->StrikeCount = aStrikeCount;
  _this->GlobalCurrentPos = aGlobalCurrentPos;
  _this->GlobalHittingPos = aGlobalHittingPos;
  return _this;
}

/* Wrapper function for the non virtual method : 'Core::CursorEvent.InitializeHold()' */
CoreCursorEvent CoreCursorEvent__InitializeHold( void* _this, XInt32 aFinger, XPoint 
  aCurrentPos, XPoint aHittingPos, XInt32 aHoldPeriod, XInt32 aStrikeCount, XPoint 
  aHitOffset, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos )
{
  return CoreCursorEvent_InitializeHold((CoreCursorEvent)_this, aFinger, aCurrentPos
  , aHittingPos, aHoldPeriod, aStrikeCount, aHitOffset, aGlobalCurrentPos, aGlobalHittingPos );
}

/* The method InitializeUp() simplifies the initialization of this Core::CursorEvent 
   object. This method stores the given parameters in the variables of this event 
   object, sets its variable @Down 'false' and returns this event object to the 
   caller. */
CoreCursorEvent CoreCursorEvent_InitializeUp( CoreCursorEvent _this, XInt32 aFinger, 
  XPoint aCurrentPos, XPoint aHittingPos, XInt32 aHoldPeriod, XInt32 aStrikeCount, 
  XPoint aHitOffset, XBool aAutoDeflected, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos )
{
  _this->Down = 0;
  _this->Finger = aFinger;
  _this->CurrentPos = EwMovePointPos( aCurrentPos, aHitOffset );
  _this->HittingPos = EwMovePointPos( aHittingPos, aHitOffset );
  _this->HoldPeriod = aHoldPeriod;
  _this->StrikeCount = aStrikeCount;
  _this->AutoDeflected = aAutoDeflected;
  _this->GlobalCurrentPos = aGlobalCurrentPos;
  _this->GlobalHittingPos = aGlobalHittingPos;
  return _this;
}

/* Wrapper function for the non virtual method : 'Core::CursorEvent.InitializeUp()' */
CoreCursorEvent CoreCursorEvent__InitializeUp( void* _this, XInt32 aFinger, XPoint 
  aCurrentPos, XPoint aHittingPos, XInt32 aHoldPeriod, XInt32 aStrikeCount, XPoint 
  aHitOffset, XBool aAutoDeflected, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos )
{
  return CoreCursorEvent_InitializeUp((CoreCursorEvent)_this, aFinger, aCurrentPos
  , aHittingPos, aHoldPeriod, aStrikeCount, aHitOffset, aAutoDeflected, aGlobalCurrentPos
  , aGlobalHittingPos );
}

/* The method InitializeDown() simplifies the initialization of this Core::CursorEvent 
   object. This method stores the given parameters in the variables of this event 
   object, sets its variable @Down 'true' and returns this event object to the caller. */
CoreCursorEvent CoreCursorEvent_InitializeDown( CoreCursorEvent _this, XInt32 aFinger, 
  XPoint aCurrentPos, XInt32 aStrikeCount, XPoint aHitOffset, XBool aAutoDeflected, 
  XPoint aGlobalCurrentPos )
{
  _this->Down = 1;
  _this->Finger = aFinger;
  _this->CurrentPos = EwMovePointPos( aCurrentPos, aHitOffset );
  _this->HittingPos = EwMovePointPos( aCurrentPos, aHitOffset );
  _this->HoldPeriod = 0;
  _this->StrikeCount = aStrikeCount;
  _this->AutoDeflected = aAutoDeflected;
  _this->GlobalCurrentPos = aGlobalCurrentPos;
  _this->GlobalHittingPos = aGlobalCurrentPos;
  return _this;
}

/* Wrapper function for the non virtual method : 'Core::CursorEvent.InitializeDown()' */
CoreCursorEvent CoreCursorEvent__InitializeDown( void* _this, XInt32 aFinger, XPoint 
  aCurrentPos, XInt32 aStrikeCount, XPoint aHitOffset, XBool aAutoDeflected, XPoint 
  aGlobalCurrentPos )
{
  return CoreCursorEvent_InitializeDown((CoreCursorEvent)_this, aFinger, aCurrentPos
  , aStrikeCount, aHitOffset, aAutoDeflected, aGlobalCurrentPos );
}

/* Variants derived from the class : 'Core::CursorEvent' */
EW_DEFINE_CLASS_VARIANTS( CoreCursorEvent )
EW_END_OF_CLASS_VARIANTS( CoreCursorEvent )

/* Virtual Method Table (VMT) for the class : 'Core::CursorEvent' */
EW_DEFINE_CLASS( CoreCursorEvent, CoreEvent, _None, _None, _None, _None, _None, 
                 "Core::CursorEvent" )
EW_END_OF_CLASS( CoreCursorEvent )

/* Initializer for the class 'Core::DragEvent' */
void CoreDragEvent__Init( CoreDragEvent _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreEvent__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreDragEvent );
}

/* Re-Initializer for the class 'Core::DragEvent' */
void CoreDragEvent__ReInit( CoreDragEvent _this )
{
  /* At first re-initialize the super class ... */
  CoreEvent__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::DragEvent' */
void CoreDragEvent__Done( CoreDragEvent _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreDragEvent );

  /* Don't forget to deinitialize the super class ... */
  CoreEvent__Done( &_this->_Super );
}

/* The method Initialize() simplifies the initialization of this Core::DragEvent 
   object. This method stores the given parameters in the variables of this event 
   object and returns this event object to the caller. */
CoreDragEvent CoreDragEvent_Initialize( CoreDragEvent _this, XInt32 aFinger, XPoint 
  aCurrentPos, XPoint aHittingPos, XPoint aOffset, XInt32 aHoldPeriod, XInt32 aSequelCount, 
  XPoint aHitOffset, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos )
{
  _this->Finger = aFinger;
  _this->CurrentPos = EwMovePointPos( aCurrentPos, aHitOffset );
  _this->HittingPos = EwMovePointPos( aHittingPos, aHitOffset );
  _this->Offset = aOffset;
  _this->HoldPeriod = aHoldPeriod;
  _this->StrikeCount = aSequelCount;
  _this->GlobalCurrentPos = aGlobalCurrentPos;
  _this->GlobalHittingPos = aGlobalHittingPos;
  return _this;
}

/* Wrapper function for the non virtual method : 'Core::DragEvent.Initialize()' */
CoreDragEvent CoreDragEvent__Initialize( void* _this, XInt32 aFinger, XPoint aCurrentPos, 
  XPoint aHittingPos, XPoint aOffset, XInt32 aHoldPeriod, XInt32 aSequelCount, XPoint 
  aHitOffset, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos )
{
  return CoreDragEvent_Initialize((CoreDragEvent)_this, aFinger, aCurrentPos, aHittingPos
  , aOffset, aHoldPeriod, aSequelCount, aHitOffset, aGlobalCurrentPos, aGlobalHittingPos );
}

/* Variants derived from the class : 'Core::DragEvent' */
EW_DEFINE_CLASS_VARIANTS( CoreDragEvent )
EW_END_OF_CLASS_VARIANTS( CoreDragEvent )

/* Virtual Method Table (VMT) for the class : 'Core::DragEvent' */
EW_DEFINE_CLASS( CoreDragEvent, CoreEvent, _None, _None, _None, _None, _None, "Core::DragEvent" )
EW_END_OF_CLASS( CoreDragEvent )

/* Initializer for the class 'Core::Outline' */
void CoreOutline__Init( CoreOutline _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreRectView__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreOutline );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Super2.viewState = CoreViewStateAlphaBlended | CoreViewStateIsOutline | 
  CoreViewStateVisible;
  _this->Formation = CoreFormationNone;
}

/* Re-Initializer for the class 'Core::Outline' */
void CoreOutline__ReInit( CoreOutline _this )
{
  /* At first re-initialize the super class ... */
  CoreRectView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::Outline' */
void CoreOutline__Done( CoreOutline _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreOutline );

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
void CoreOutline_Draw( CoreOutline _this, GraphicsCanvas aCanvas, XRect aClip, XPoint 
  aOffset, XInt32 aOpacity, XBool aBlend )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aBlend );
  EW_UNUSED_ARG( aOpacity );
  EW_UNUSED_ARG( aOffset );
  EW_UNUSED_ARG( aClip );
  EW_UNUSED_ARG( aCanvas );
}

/* 'C' function for method : 'Core::Outline.OnSetBounds()' */
void CoreOutline_OnSetBounds( CoreOutline _this, XRect value )
{
  XPoint oldSize;
  XPoint newSize;
  XBool resize;
  XPoint offset;

  if ( !EwCompRect( value, _this->Super1.Bounds ))
    return;

  oldSize = EwGetRectSize( _this->Super1.Bounds );
  newSize = EwGetRectSize( value );
  resize = (XBool)EwCompPoint( oldSize, newSize );
  offset = EwMovePointNeg( value.Point1, _this->Super1.Bounds.Point1 );

  if ( EwCompPoint( offset, _Const0000 ) && !resize )
  {
    CoreView view = _this->Super2.next;

    while (( view != 0 ) && !(( view->viewState & CoreViewStateIsOutline ) == CoreViewStateIsOutline ))
    {
      if ((( view->viewState & CoreViewStateEmbedded ) == CoreViewStateEmbedded ))
      {
        XBool tmp = (( view->viewState & CoreViewStateFastReshape ) == CoreViewStateFastReshape );
        CoreView__MoveView( view, offset, tmp );
      }

      view = view->next;
    }
  }

  if (( resize && ( oldSize.X > 0 )) && ( oldSize.Y > 0 ))
  {
    XRect bounds = EwMoveRectPos( _this->Super1.Bounds, _this->ScrollOffset );
    CoreView view = _this->Super2.next;
    XSet fixed = CoreLayoutAlignToLeft | CoreLayoutAlignToTop;

    while (( view != 0 ) && !(( view->viewState & CoreViewStateIsOutline ) == CoreViewStateIsOutline ))
    {
      if ((( view->viewState & CoreViewStateEmbedded ) == CoreViewStateEmbedded ))
      {
        if (( view->layoutContext != 0 ) && ( view->layoutContext->outline != _this ))
          view->layoutContext = 0;

        if (( view->layoutContext == 0 ) && (( view->Layout != fixed ) || ( _this->Formation 
            != CoreFormationNone )))
          CoreView__initLayoutContext( view, bounds, _this );
      }

      view = view->next;
    }
  }

  CoreRectView_OnSetBounds((CoreRectView)_this, value );

  if (( _this->Super2.Owner != 0 ) && resize )
  {
    _this->Super2.viewState = _this->Super2.viewState | CoreViewStateUpdateLayout;

    if ( !(( _this->Super2.Owner->Super2.viewState & CoreViewStateUpdatingLayout ) 
        == CoreViewStateUpdatingLayout ))
    {
      _this->Super2.Owner->Super2.viewState = _this->Super2.Owner->Super2.viewState 
      | CoreViewStatePendingLayout;
      EwPostSignal( EwNewSlot( _this->Super2.Owner, CoreGroup_updateComponent ), 
        ((XObject)_this ));
    }
  }
}

/* Variants derived from the class : 'Core::Outline' */
EW_DEFINE_CLASS_VARIANTS( CoreOutline )
EW_END_OF_CLASS_VARIANTS( CoreOutline )

/* Virtual Method Table (VMT) for the class : 'Core::Outline' */
EW_DEFINE_CLASS( CoreOutline, CoreRectView, _None, _None, _None, _None, _None, "Core::Outline" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreOutline_Draw,
  CoreView_HandleEvent,
  CoreView_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreView_ChangeViewState,
  CoreOutline_OnSetBounds,
EW_END_OF_CLASS( CoreOutline )

/* Initializer for the class 'Core::SimpleTouchHandler' */
void CoreSimpleTouchHandler__Init( CoreSimpleTouchHandler _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreQuadView__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreSimpleTouchHandler );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Super2.viewState = CoreViewStateAlphaBlended | CoreViewStateEnabled | CoreViewStateFastReshape 
  | CoreViewStateTouchable | CoreViewStateVisible;
  _this->RetargetDelay = 1000;
  _this->RetargetOffset = 8;
  _this->LimitToFinger = -1;
  _this->MaxStrikeCount = 1;
  _this->MinStrikeCount = 1;
}

/* Re-Initializer for the class 'Core::SimpleTouchHandler' */
void CoreSimpleTouchHandler__ReInit( CoreSimpleTouchHandler _this )
{
  /* At first re-initialize the super class ... */
  CoreQuadView__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::SimpleTouchHandler' */
void CoreSimpleTouchHandler__Done( CoreSimpleTouchHandler _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreSimpleTouchHandler );

  /* Don't forget to deinitialize the super class ... */
  CoreQuadView__Done( &_this->_Super );
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
void CoreSimpleTouchHandler_Draw( CoreSimpleTouchHandler _this, GraphicsCanvas aCanvas, 
  XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aBlend );
  EW_UNUSED_ARG( aOpacity );
  EW_UNUSED_ARG( aOffset );
  EW_UNUSED_ARG( aClip );
  EW_UNUSED_ARG( aCanvas );
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
XObject CoreSimpleTouchHandler_HandleEvent( CoreSimpleTouchHandler _this, CoreEvent 
  aEvent )
{
  CoreCursorEvent event1 = EwCastObject( aEvent, CoreCursorEvent );
  CoreDragEvent event2 = EwCastObject( aEvent, CoreDragEvent );
  XBool inside = _this->Inside;

  if (( event1 == 0 ) && ( event2 == 0 ))
    return 0;

  if ( event1 != 0 )
  {
    _this->active = event1->Down;
    _this->Down = event1->Down;
    _this->Inside = CoreQuadView_IsPointInside((CoreQuadView)_this, event1->CurrentPos );
    _this->HittingPos = event1->HittingPos;
    _this->CurrentPos = event1->CurrentPos;
    _this->Offset = _Const0000;
    _this->HoldPeriod = event1->HoldPeriod;
    _this->StrikeCount = event1->StrikeCount;
    _this->AutoDeflected = event1->AutoDeflected;
    _this->Finger = event1->Finger;
    _this->activeFinger = event1->Finger;
    _this->Time = event1->Super1.Time;

    if ( event1->Down && ( event1->HoldPeriod == 0 ))
      inside = 0;
  }

  if ( event2 != 0 )
  {
    _this->Down = 1;
    _this->Inside = CoreQuadView_IsPointInside((CoreQuadView)_this, event2->CurrentPos );
    _this->HittingPos = event2->HittingPos;
    _this->CurrentPos = event2->CurrentPos;
    _this->Offset = event2->Offset;
    _this->HoldPeriod = event2->HoldPeriod;
    _this->StrikeCount = event2->StrikeCount;
    _this->Finger = event2->Finger;
    _this->activeFinger = event2->Finger;
    _this->AutoDeflected = 0;
    _this->Time = event2->Super1.Time;
    EwSignal( _this->OnDrag, ((XObject)_this ));
  }

  if ((( event1 != 0 ) && _this->Down ) && ( _this->HoldPeriod == 0 ))
    EwSignal( _this->OnPress, ((XObject)_this ));

  if ((( event1 != 0 ) && _this->Down ) && ( _this->HoldPeriod > 0 ))
    EwSignal( _this->OnHold, ((XObject)_this ));

  if (( _this->Down && _this->Inside ) && !inside )
    EwSignal( _this->OnEnter, ((XObject)_this ));

  if ((( _this->Down && !_this->Inside ) && inside ) || (( !_this->Down && _this->Inside ) 
      && inside ))
    EwSignal( _this->OnLeave, ((XObject)_this ));

  if (( event1 != 0 ) && !_this->Down )
    EwSignal( _this->OnRelease, ((XObject)_this ));

  if ( !!_this->RetargetCondition )
  {
    if ((((( _this->RetargetCondition & CoreRetargetReasonLongPress ) == CoreRetargetReasonLongPress ) 
        && ( event1 != 0 )) && event1->Down ) && ( event1->HoldPeriod >= _this->RetargetDelay ))
      CoreRoot_RetargetCursorWithReason( CoreView__GetRoot( _this ), 0, ((CoreView)_this ), 
      CoreRetargetReasonLongPress );

    if (((( _this->RetargetCondition & CoreRetargetReasonWipeUp ) == CoreRetargetReasonWipeUp ) 
        && ( event2 != 0 )) && (( event2->GlobalCurrentPos.Y - event2->GlobalHittingPos.Y ) 
        <= -_this->RetargetOffset ))
      CoreRoot_RetargetCursorWithReason( CoreView__GetRoot( _this ), 0, ((CoreView)_this ), 
      CoreRetargetReasonWipeUp );

    if (((( _this->RetargetCondition & CoreRetargetReasonWipeDown ) == CoreRetargetReasonWipeDown ) 
        && ( event2 != 0 )) && (( event2->GlobalCurrentPos.Y - event2->GlobalHittingPos.Y ) 
        >= _this->RetargetOffset ))
      CoreRoot_RetargetCursorWithReason( CoreView__GetRoot( _this ), 0, ((CoreView)_this ), 
      CoreRetargetReasonWipeDown );

    if (((( _this->RetargetCondition & CoreRetargetReasonWipeLeft ) == CoreRetargetReasonWipeLeft ) 
        && ( event2 != 0 )) && (( event2->GlobalCurrentPos.X - event2->GlobalHittingPos.X ) 
        <= -_this->RetargetOffset ))
      CoreRoot_RetargetCursorWithReason( CoreView__GetRoot( _this ), 0, ((CoreView)_this ), 
      CoreRetargetReasonWipeLeft );

    if (((( _this->RetargetCondition & CoreRetargetReasonWipeRight ) == CoreRetargetReasonWipeRight ) 
        && ( event2 != 0 )) && (( event2->GlobalCurrentPos.X - event2->GlobalHittingPos.X ) 
        >= _this->RetargetOffset ))
      CoreRoot_RetargetCursorWithReason( CoreView__GetRoot( _this ), 0, ((CoreView)_this ), 
      CoreRetargetReasonWipeRight );
  }

  return ((XObject)_this );
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
CoreCursorHit CoreSimpleTouchHandler_CursorHitTest( CoreSimpleTouchHandler _this, 
  XRect aArea, XInt32 aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet 
  aRetargetReason )
{
  if (( aDedicatedView != 0 ) && ( aDedicatedView != (CoreView)_this ))
    return 0;

  if (( aStrikeCount < _this->MinStrikeCount ) || ( aStrikeCount > _this->MaxStrikeCount ))
    return 0;

  if (( _this->LimitToFinger >= 0 ) && ( aFinger != _this->LimitToFinger ))
    return 0;

  if (( !_this->EnableMultiTouch && _this->active ) && ( aFinger != _this->activeFinger ))
    return 0;

  if ( !!( aRetargetReason & _this->RetargetCondition ))
    return 0;

  if ( CoreQuadView_HasRectShape((CoreQuadView)_this ))
  {
    XRect r = EwIntersectRect( aArea, CoreView__GetExtent( _this ));

    if ( !EwIsRectEmpty( r ))
    {
      XPoint center = EwGetRectCenter( aArea );
      XPoint offset = _Const0000;

      if ( center.X < r.Point1.X )
        offset.X = ( r.Point1.X - center.X );

      if ( center.X >= r.Point2.X )
        offset.X = (( r.Point2.X - center.X ) - 1 );

      if ( center.Y < r.Point1.Y )
        offset.Y = ( r.Point1.Y - center.Y );

      if ( center.Y >= r.Point2.Y )
        offset.Y = (( r.Point2.Y - center.Y ) - 1 );

      return CoreCursorHit_Initialize( EwNewObject( CoreCursorHit, 0 ), ((CoreView)_this ), 
        offset );
    }
  }
  else
  {
    XPoint points[ 9 ];
    XInt32 i;
    points[ 0 ] = EwGetRectCenter( aArea );
    points[ 1 ] = points[ 0 ];
    points[ 2 ] = points[ 0 ];
    points[ 3 ] = points[ 0 ];
    points[ 4 ] = points[ 0 ];
    points[ 1 ].X = aArea.Point1.X;
    points[ 2 ].Y = aArea.Point1.Y;
    points[ 3 ].X = aArea.Point2.X;
    points[ 4 ].Y = aArea.Point2.Y;
    points[ 5 ] = aArea.Point1;
    points[ 6 ] = EwNewPoint( aArea.Point2.X, aArea.Point1.Y );
    points[ 7 ] = EwNewPoint( aArea.Point1.X, aArea.Point2.Y );
    points[ 8 ] = aArea.Point2;

    for ( i = 0; i < 9; i = i + 1 )
      if ( CoreQuadView_IsPointInside((CoreQuadView)_this, points[ EwCheckIndex( 
          i, 9 )]))
        return CoreCursorHit_Initialize( EwNewObject( CoreCursorHit, 0 ), ((CoreView)_this ), 
          EwMovePointNeg( points[ EwCheckIndex( i, 9 )], points[ 0 ]));
  }

  return 0;
}

/* Variants derived from the class : 'Core::SimpleTouchHandler' */
EW_DEFINE_CLASS_VARIANTS( CoreSimpleTouchHandler )
EW_END_OF_CLASS_VARIANTS( CoreSimpleTouchHandler )

/* Virtual Method Table (VMT) for the class : 'Core::SimpleTouchHandler' */
EW_DEFINE_CLASS( CoreSimpleTouchHandler, CoreQuadView, OnDrag, OnDrag, activeFinger, 
                 activeFinger, activeFinger, "Core::SimpleTouchHandler" )
  CoreQuadView_initLayoutContext,
  CoreView_GetRoot,
  CoreSimpleTouchHandler_Draw,
  CoreSimpleTouchHandler_HandleEvent,
  CoreSimpleTouchHandler_CursorHitTest,
  CoreQuadView_ArrangeView,
  CoreQuadView_MoveView,
  CoreQuadView_GetExtent,
  CoreView_ChangeViewState,
EW_END_OF_CLASS( CoreSimpleTouchHandler )

/* Initializer for the class 'Core::KeyPressHandler' */
void CoreKeyPressHandler__Init( CoreKeyPressHandler _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreKeyPressHandler );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Filter = CoreKeyCodeAnyKey;
  _this->Enabled = 1;

  /* Call the user defined constructor */
  CoreKeyPressHandler_Init( _this, aArg );
}

/* Re-Initializer for the class 'Core::KeyPressHandler' */
void CoreKeyPressHandler__ReInit( CoreKeyPressHandler _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::KeyPressHandler' */
void CoreKeyPressHandler__Done( CoreKeyPressHandler _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreKeyPressHandler );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Core::KeyPressHandler.HandleEvent()' */
XBool CoreKeyPressHandler_HandleEvent( CoreKeyPressHandler _this, CoreKeyEvent aEvent )
{
  if (( aEvent != 0 ) && CoreKeyEvent_IsCode( aEvent, _this->Filter ))
  {
    _this->Down = aEvent->Down;
    _this->Code = aEvent->Code;
    _this->CharCode = aEvent->CharCode;
    _this->Time = aEvent->Super1.Time;
    _this->Continue = 0;

    if ( aEvent->Down )
    {
      _this->RepetitionCount = _this->pressCounter;
      _this->Repetition = (XBool)( _this->pressCounter > 0 );

      if ( _this->Repetition )
        EwSignal( _this->OnHold, ((XObject)_this ));
      else
        EwSignal( _this->OnPress, ((XObject)_this ));

      if ( !_this->Continue )
        _this->pressCounter = _this->pressCounter + 1;

      return (XBool)!_this->Continue;
    }

    if ( !aEvent->Down )
    {
      _this->Repetition = (XBool)( _this->pressCounter > 1 );
      _this->RepetitionCount = _this->pressCounter - 1;
      _this->pressCounter = 0;
      EwSignal( _this->OnRelease, ((XObject)_this ));
      return (XBool)!_this->Continue;
    }
  }

  return 0;
}

/* 'C' function for method : 'Core::KeyPressHandler.Init()' */
void CoreKeyPressHandler_Init( CoreKeyPressHandler _this, XHandle aArg )
{
  CoreGroup group;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  group = EwCastObject( _this->_XObject._Link, CoreGroup );

  if ( group == 0 )
  {
    EwThrow( EwLoadString( &_Const0009 ));
    return;
  }

  _this->next = group->keyHandlers;
  group->keyHandlers = _this;
}

/* Variants derived from the class : 'Core::KeyPressHandler' */
EW_DEFINE_CLASS_VARIANTS( CoreKeyPressHandler )
EW_END_OF_CLASS_VARIANTS( CoreKeyPressHandler )

/* Virtual Method Table (VMT) for the class : 'Core::KeyPressHandler' */
EW_DEFINE_CLASS( CoreKeyPressHandler, XObject, next, OnRelease, pressCounter, pressCounter, 
                 pressCounter, "Core::KeyPressHandler" )
EW_END_OF_CLASS( CoreKeyPressHandler )

/* Initializer for the class 'Core::CursorHit' */
void CoreCursorHit__Init( CoreCursorHit _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreCursorHit );
}

/* Re-Initializer for the class 'Core::CursorHit' */
void CoreCursorHit__ReInit( CoreCursorHit _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::CursorHit' */
void CoreCursorHit__Done( CoreCursorHit _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreCursorHit );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* The method Initialize() initializes this Core::CursorHit object with the given 
   parameter. The value offset stores an optional displacement if the user has touched 
   the view outside its boundary area. This can occur when the user tries to hit 
   a small GUI component with a thick finger. This offset value determines the number 
   of pixel to correct all cursor coordinates delivered in the following Core::CursorEvent 
   and Core::DragEvent events. In this manner the target view will receive coordinates 
   lying correctly within its boundary area. */
CoreCursorHit CoreCursorHit_Initialize( CoreCursorHit _this, CoreView aView, XPoint 
  aOffset )
{
  _this->View = aView;
  _this->Offset = aOffset;
  _this->Deviation = ( aOffset.X * aOffset.X ) + ( aOffset.Y * aOffset.Y );
  return _this;
}

/* Variants derived from the class : 'Core::CursorHit' */
EW_DEFINE_CLASS_VARIANTS( CoreCursorHit )
EW_END_OF_CLASS_VARIANTS( CoreCursorHit )

/* Virtual Method Table (VMT) for the class : 'Core::CursorHit' */
EW_DEFINE_CLASS( CoreCursorHit, XObject, View, Deviation, Deviation, Deviation, 
                 Deviation, "Core::CursorHit" )
EW_END_OF_CLASS( CoreCursorHit )

/* Initializer for the class 'Core::ModalContext' */
void CoreModalContext__Init( CoreModalContext _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreModalContext );
}

/* Re-Initializer for the class 'Core::ModalContext' */
void CoreModalContext__ReInit( CoreModalContext _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::ModalContext' */
void CoreModalContext__Done( CoreModalContext _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreModalContext );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* Variants derived from the class : 'Core::ModalContext' */
EW_DEFINE_CLASS_VARIANTS( CoreModalContext )
EW_END_OF_CLASS_VARIANTS( CoreModalContext )

/* Virtual Method Table (VMT) for the class : 'Core::ModalContext' */
EW_DEFINE_CLASS( CoreModalContext, XObject, group, _None, _None, _None, _None, "Core::ModalContext" )
EW_END_OF_CLASS( CoreModalContext )

/* Initializer for the class 'Core::LayoutContext' */
void CoreLayoutContext__Init( CoreLayoutContext _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreLayoutContext );
}

/* Re-Initializer for the class 'Core::LayoutContext' */
void CoreLayoutContext__ReInit( CoreLayoutContext _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::LayoutContext' */
void CoreLayoutContext__Done( CoreLayoutContext _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreLayoutContext );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* Variants derived from the class : 'Core::LayoutContext' */
EW_DEFINE_CLASS_VARIANTS( CoreLayoutContext )
EW_END_OF_CLASS_VARIANTS( CoreLayoutContext )

/* Virtual Method Table (VMT) for the class : 'Core::LayoutContext' */
EW_DEFINE_CLASS( CoreLayoutContext, XObject, outline, extent, extent, extent, extent, 
                 "Core::LayoutContext" )
EW_END_OF_CLASS( CoreLayoutContext )

/* Initializer for the class 'Core::LayoutQuadContext' */
void CoreLayoutQuadContext__Init( CoreLayoutQuadContext _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreLayoutContext__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreLayoutQuadContext );
}

/* Re-Initializer for the class 'Core::LayoutQuadContext' */
void CoreLayoutQuadContext__ReInit( CoreLayoutQuadContext _this )
{
  /* At first re-initialize the super class ... */
  CoreLayoutContext__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::LayoutQuadContext' */
void CoreLayoutQuadContext__Done( CoreLayoutQuadContext _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreLayoutQuadContext );

  /* Don't forget to deinitialize the super class ... */
  CoreLayoutContext__Done( &_this->_Super );
}

/* Variants derived from the class : 'Core::LayoutQuadContext' */
EW_DEFINE_CLASS_VARIANTS( CoreLayoutQuadContext )
EW_END_OF_CLASS_VARIANTS( CoreLayoutQuadContext )

/* Virtual Method Table (VMT) for the class : 'Core::LayoutQuadContext' */
EW_DEFINE_CLASS( CoreLayoutQuadContext, CoreLayoutContext, _None, _None, _None, 
                 _None, _None, "Core::LayoutQuadContext" )
EW_END_OF_CLASS( CoreLayoutQuadContext )

/* Initializer for the class 'Core::DialogContext' */
void CoreDialogContext__Init( CoreDialogContext _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreDialogContext );
}

/* Re-Initializer for the class 'Core::DialogContext' */
void CoreDialogContext__ReInit( CoreDialogContext _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::DialogContext' */
void CoreDialogContext__Done( CoreDialogContext _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreDialogContext );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* Variants derived from the class : 'Core::DialogContext' */
EW_DEFINE_CLASS_VARIANTS( CoreDialogContext )
EW_END_OF_CLASS_VARIANTS( CoreDialogContext )

/* Virtual Method Table (VMT) for the class : 'Core::DialogContext' */
EW_DEFINE_CLASS( CoreDialogContext, XObject, group, _None, _None, _None, _None, 
                 "Core::DialogContext" )
EW_END_OF_CLASS( CoreDialogContext )

/* Initializer for the class 'Core::TaskQueue' */
void CoreTaskQueue__Init( CoreTaskQueue _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreTaskQueue );
}

/* Re-Initializer for the class 'Core::TaskQueue' */
void CoreTaskQueue__ReInit( CoreTaskQueue _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::TaskQueue' */
void CoreTaskQueue__Done( CoreTaskQueue _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreTaskQueue );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* Variants derived from the class : 'Core::TaskQueue' */
EW_DEFINE_CLASS_VARIANTS( CoreTaskQueue )
EW_END_OF_CLASS_VARIANTS( CoreTaskQueue )

/* Virtual Method Table (VMT) for the class : 'Core::TaskQueue' */
EW_DEFINE_CLASS( CoreTaskQueue, XObject, current, _None, _None, _None, _None, "Core::TaskQueue" )
EW_END_OF_CLASS( CoreTaskQueue )

/* Initializer for the class 'Core::Task' */
void CoreTask__Init( CoreTask _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreTask );
}

/* Re-Initializer for the class 'Core::Task' */
void CoreTask__ReInit( CoreTask _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::Task' */
void CoreTask__Done( CoreTask _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreTask );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* Variants derived from the class : 'Core::Task' */
EW_DEFINE_CLASS_VARIANTS( CoreTask )
EW_END_OF_CLASS_VARIANTS( CoreTask )

/* Virtual Method Table (VMT) for the class : 'Core::Task' */
EW_DEFINE_CLASS( CoreTask, XObject, _None, _None, _None, _None, _None, "Core::Task" )
EW_END_OF_CLASS( CoreTask )

/* Initializer for the class 'Core::Resource' */
void CoreResource__Init( CoreResource _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreResource );

  /* Call the user defined constructor */
  CoreResource_Init( _this, aArg );
}

/* Re-Initializer for the class 'Core::Resource' */
void CoreResource__ReInit( CoreResource _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::Resource' */
void CoreResource__Done( CoreResource _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreResource );

  /* Call the user defined destructor of the class */
  CoreResource_Done( _this );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Core::Resource.Done()' */
void CoreResource_Done( CoreResource _this )
{
  if ( _this->resource != 0 )
    {
      #define this _this

      /* Remove 'this' resource object from the Resource Manager. */
      EwDeregisterResource((XObject)this );

      #undef this
    }

  _this->resource = 0;
}

/* 'C' function for method : 'Core::Resource.Init()' */
void CoreResource_Init( CoreResource _this, XHandle aArg )
{
  {
    #define this _this

    /* aArg is a pointer to the area in the ROM where the resources code is
       stored. Register 'this' object by the Resource Manager. */
    if ( aArg )
      EwRegisterResource((XObject)this, (const void*)aArg );

    #undef this
  }
  _this->resource = aArg;
}

/* Variants derived from the class : 'Core::Resource' */
EW_DEFINE_CLASS_VARIANTS( CoreResource )
EW_END_OF_CLASS_VARIANTS( CoreResource )

/* Virtual Method Table (VMT) for the class : 'Core::Resource' */
EW_DEFINE_CLASS( CoreResource, XObject, _None, _None, _None, _None, _None, "Core::Resource" )
EW_END_OF_CLASS( CoreResource )

/* User defined inline code: 'Core::TimerProc' */
  /* This local procedure will be called when the OS timer is expired, so 
     the routine invokes the Core::Timer.Trigger() method of the timer 
     object. */
  static void TimerProc( XHandle aArg )
  {
    CoreTimer__Trigger((CoreTimer)aArg );
  }


/* Initializer for the class 'Core::Timer' */
void CoreTimer__Init( CoreTimer _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CoreTimer );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Period = 1000;
}

/* Re-Initializer for the class 'Core::Timer' */
void CoreTimer__ReInit( CoreTimer _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Core::Timer' */
void CoreTimer__Done( CoreTimer _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CoreTimer );

  /* Call the user defined destructor of the class */
  CoreTimer_Done( _this );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Core::Timer.Done()' */
void CoreTimer_Done( CoreTimer _this )
{
  XHandle tmp = _this->timer;

  if ( tmp != 0 )
    EwDestroyTimer((XTimer*)tmp );

  _this->timer = 0;
}

/* 'C' function for method : 'Core::Timer.restart()' */
void CoreTimer_restart( CoreTimer _this, XInt32 aBegin, XInt32 aPeriod )
{
  XHandle tmp;

  if ( aBegin < 0 )
    aBegin = 0;

  if ( aPeriod < 0 )
    aPeriod = 0;

  tmp = _this->timer;

  if (( tmp == 0 ) && (( aBegin > 0 ) || ( aPeriod > 0 )))
    {
      #define this _this

      tmp = (XHandle)(void*)EwCreateTimer( TimerProc, (XHandle)(void*)this );

      #undef this
    }

  if ( tmp != 0 )
    {
      /* At first stop the timer ... */
      EwResetTimer((XTimer*)tmp );

      /* and then restart it again. */
      EwStartTimer((XTimer*)tmp, aBegin, aPeriod );
    }

  _this->timer = tmp;
}

/* 'C' function for method : 'Core::Timer.OnSetPeriod()' */
void CoreTimer_OnSetPeriod( CoreTimer _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value == _this->Period )
    return;

  _this->Period = value;

  if ( _this->Enabled )
    CoreTimer_restart( _this, _this->Begin, value );
}

/* 'C' function for method : 'Core::Timer.OnSetBegin()' */
void CoreTimer_OnSetBegin( CoreTimer _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value == _this->Begin )
    return;

  _this->Begin = value;

  if ( _this->Enabled )
    CoreTimer_restart( _this, value, _this->Period );
}

/* 'C' function for method : 'Core::Timer.OnSetEnabled()' */
void CoreTimer_OnSetEnabled( CoreTimer _this, XBool value )
{
  if ( value == _this->Enabled )
    return;

  _this->Enabled = value;

  if ( value )
    CoreTimer_restart( _this, _this->Begin, _this->Period );
  else
    CoreTimer_restart( _this, 0, 0 );

  _this->Time = CoreTimer_GetCurrentTime( _this );
}

/* The method 'GetCurrentTime()' returns the current time expressed in milliseconds. 
   The value can be used e.g. to calculate the time span elapsed since the timer 
   was expired (see @Time). */
XUInt32 CoreTimer_GetCurrentTime( CoreTimer _this )
{
  XUInt32 ticksCount;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );

  ticksCount = 0;
  ticksCount = (XUInt32)EwGetTicks();
  return ticksCount;
}

/* The method Trigger() will be invoked when the timer is expired (when the interval 
   defined in @Begin or @Period is elapsed). The method can be overridden and implemented 
   in derived classes. The default implementation of this method sends a signal 
   to the slot method stored in the @OnTrigger property. */
void CoreTimer_Trigger( CoreTimer _this )
{
  _this->Time = CoreTimer_GetCurrentTime( _this );

  if ( _this->Period == 0 )
    CoreTimer_OnSetEnabled( _this, 0 );

  EwSignal( _this->OnTrigger, ((XObject)_this ));
}

/* Wrapper function for the virtual method : 'Core::Timer.Trigger()' */
void CoreTimer__Trigger( void* _this )
{
  ((CoreTimer)_this)->_VMT->Trigger((CoreTimer)_this );
}

/* Variants derived from the class : 'Core::Timer' */
EW_DEFINE_CLASS_VARIANTS( CoreTimer )
EW_END_OF_CLASS_VARIANTS( CoreTimer )

/* Virtual Method Table (VMT) for the class : 'Core::Timer' */
EW_DEFINE_CLASS( CoreTimer, XObject, OnTrigger, OnTrigger, timer, timer, timer, 
                 "Core::Timer" )
  CoreTimer_Trigger,
EW_END_OF_CLASS( CoreTimer )

/* Embedded Wizard */
