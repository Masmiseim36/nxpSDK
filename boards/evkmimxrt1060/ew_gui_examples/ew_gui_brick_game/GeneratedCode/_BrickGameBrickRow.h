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

#ifndef _BrickGameBrickRow_H
#define _BrickGameBrickRow_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

#include "_CoreGroup.h"
#include "_ViewsImage.h"

/* Forward declaration of the class BrickGame::BallPosition */
#ifndef _BrickGameBallPosition_
  EW_DECLARE_CLASS( BrickGameBallPosition )
#define _BrickGameBallPosition_
#endif

/* Forward declaration of the class BrickGame::BrickRow */
#ifndef _BrickGameBrickRow_
  EW_DECLARE_CLASS( BrickGameBrickRow )
#define _BrickGameBrickRow_
#endif

/* Forward declaration of the class Core::DialogContext */
#ifndef _CoreDialogContext_
  EW_DECLARE_CLASS( CoreDialogContext )
#define _CoreDialogContext_
#endif

/* Forward declaration of the class Core::KeyPressHandler */
#ifndef _CoreKeyPressHandler_
  EW_DECLARE_CLASS( CoreKeyPressHandler )
#define _CoreKeyPressHandler_
#endif

/* Forward declaration of the class Core::LayoutContext */
#ifndef _CoreLayoutContext_
  EW_DECLARE_CLASS( CoreLayoutContext )
#define _CoreLayoutContext_
#endif

/* Forward declaration of the class Core::TaskQueue */
#ifndef _CoreTaskQueue_
  EW_DECLARE_CLASS( CoreTaskQueue )
#define _CoreTaskQueue_
#endif

/* Forward declaration of the class Core::View */
#ifndef _CoreView_
  EW_DECLARE_CLASS( CoreView )
#define _CoreView_
#endif

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif


/* This is a class. */
EW_DEFINE_FIELDS( BrickGameBrickRow, CoreGroup )
  EW_OBJECT  ( Image1,          ViewsImage )
  EW_OBJECT  ( Image2,          ViewsImage )
  EW_OBJECT  ( Image3,          ViewsImage )
  EW_OBJECT  ( Image4,          ViewsImage )
  EW_OBJECT  ( Image5,          ViewsImage )
  EW_OBJECT  ( Image6,          ViewsImage )
  EW_OBJECT  ( Image7,          ViewsImage )
  EW_OBJECT  ( Image8,          ViewsImage )
  EW_VARIABLE( noOfBricks,      XInt32 )
EW_END_OF_FIELDS( BrickGameBrickRow )

/* Virtual Method Table (VMT) for the class : 'BrickGame::BrickRow' */
EW_DEFINE_METHODS( BrickGameBrickRow, CoreGroup )
  EW_METHOD( initLayoutContext, void )( CoreRectView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( CoreGroup _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( HandleEvent,       XObject )( CoreView _this, CoreEvent aEvent )
  EW_METHOD( CursorHitTest,     CoreCursorHit )( CoreGroup _this, XRect aArea, XInt32 
    aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
  EW_METHOD( ArrangeView,       XPoint )( CoreRectView _this, XRect aBounds, XEnum 
    aFormation )
  EW_METHOD( MoveView,          void )( CoreRectView _this, XPoint aOffset, XBool 
    aFastMove )
  EW_METHOD( GetExtent,         XRect )( CoreRectView _this )
  EW_METHOD( ChangeViewState,   void )( CoreGroup _this, XSet aSetState, XSet aClearState )
  EW_METHOD( OnSetBounds,       void )( CoreGroup _this, XRect value )
  EW_METHOD( OnSetFocus,        void )( CoreGroup _this, CoreView value )
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateViewState,   void )( CoreGroup _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
  EW_METHOD( Restack,           void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
  EW_METHOD( Add,               void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
EW_END_OF_METHODS( BrickGameBrickRow )

/* The method 'CheckCollision' checks for collisions of the given ball with a brick 
   of the row. In case of a collision, the direction of the ball will change, the 
   brick becomes invisble and the score is counted. The method returns the number 
   of remaining active bricks. */
XInt32 BrickGameBrickRow_CheckCollision( BrickGameBrickRow _this, BrickGameBallPosition 
  aBall );

/* The method 'PrepareBricks' initializes all bricks of the row. The given parameter 
   'aType' determines the style of the bricks. */
void BrickGameBrickRow_PrepareBricks( BrickGameBrickRow _this, XEnum aType );

#ifdef __cplusplus
  }
#endif

#endif /* _BrickGameBrickRow_H */

/* Embedded Wizard */
