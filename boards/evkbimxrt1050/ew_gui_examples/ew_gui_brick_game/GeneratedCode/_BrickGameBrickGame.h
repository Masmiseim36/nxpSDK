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

#ifndef _BrickGameBrickGame_H
#define _BrickGameBrickGame_H

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

#include "_BrickGameBallPosition.h"
#include "_BrickGameBrickRow.h"
#include "_BrickGameLcdDisplay.h"
#include "_CoreGroup.h"
#include "_CoreKeyPressHandler.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreTimer.h"
#include "_ViewsImage.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"

/* Forward declaration of the class BrickGame::BrickGame */
#ifndef _BrickGameBrickGame_
  EW_DECLARE_CLASS( BrickGameBrickGame )
#define _BrickGameBrickGame_
#endif

/* Forward declaration of the class BrickGame::DataClass */
#ifndef _BrickGameDataClass_
  EW_DECLARE_CLASS( BrickGameDataClass )
#define _BrickGameDataClass_
#endif

/* Forward declaration of the class Core::DialogContext */
#ifndef _CoreDialogContext_
  EW_DECLARE_CLASS( CoreDialogContext )
#define _CoreDialogContext_
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


/* Deklaration of class : 'BrickGame::BrickGame' */
EW_DEFINE_FIELDS( BrickGameBrickGame, CoreGroup )
  EW_VARIABLE( Data,            BrickGameDataClass )
  EW_OBJECT  ( AnimationTimer,  CoreTimer )
  EW_OBJECT  ( BallPosition,    BrickGameBallPosition )
  EW_OBJECT  ( Rectangle,       ViewsRectangle )
  EW_OBJECT  ( Background,      ViewsImage )
  EW_OBJECT  ( Landscape,       ViewsImage )
  EW_OBJECT  ( BrickRow1,       BrickGameBrickRow )
  EW_OBJECT  ( BrickRow2,       BrickGameBrickRow )
  EW_OBJECT  ( BrickRow3,       BrickGameBrickRow )
  EW_OBJECT  ( BrickRow4,       BrickGameBrickRow )
  EW_OBJECT  ( BrickRow5,       BrickGameBrickRow )
  EW_OBJECT  ( SimpleTouchHandler, CoreSimpleTouchHandler )
  EW_OBJECT  ( Ball,            ViewsImage )
  EW_OBJECT  ( Paddle,          ViewsImage )
  EW_OBJECT  ( KeyLeftHandler,  CoreKeyPressHandler )
  EW_OBJECT  ( KeyRightHandler, CoreKeyPressHandler )
  EW_OBJECT  ( KeySpaceHandler, CoreKeyPressHandler )
  EW_OBJECT  ( PaddleTimer,     CoreTimer )
  EW_OBJECT  ( ScoreDisplay,    BrickGameLcdDisplay )
  EW_OBJECT  ( ScoreDescr,      ViewsText )
  EW_OBJECT  ( LevelDisplay,    BrickGameLcdDisplay )
  EW_OBJECT  ( LevelDescr,      ViewsText )
  EW_OBJECT  ( LostDisplay,     BrickGameLcdDisplay )
  EW_OBJECT  ( LostDescr,       ViewsText )
  EW_OBJECT  ( AutoDemoTimer,   CoreTimer )
  EW_VARIABLE( lastTime,        XUInt32 )
  EW_VARIABLE( speedFactor,     XFloat )
  EW_VARIABLE( paddleDirection, XInt32 )
  EW_VARIABLE( paddlePos,       XInt32 )
  EW_VARIABLE( ballMove,        XBool )
EW_END_OF_FIELDS( BrickGameBrickGame )

/* Virtual Method Table (VMT) for the class : 'BrickGame::BrickGame' */
EW_DEFINE_METHODS( BrickGameBrickGame, CoreGroup )
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
  EW_METHOD( UpdateViewState,   void )( BrickGameBrickGame _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
  EW_METHOD( Restack,           void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
  EW_METHOD( Add,               void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
EW_END_OF_METHODS( BrickGameBrickGame )

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
void BrickGameBrickGame_UpdateViewState( BrickGameBrickGame _this, XSet aState );

/* This is a slot method. */
void BrickGameBrickGame_onDragPaddle( BrickGameBrickGame _this, XObject sender );

/* The slot method 'Animate' is timer driven and calculates the next position of 
   ball, paddle and background. */
void BrickGameBrickGame_Animate( BrickGameBrickGame _this, XObject sender );

/* The slot method 'PrepareLevel' is used to initialize all brick rows according 
   the reached game level. */
void BrickGameBrickGame_PrepareLevel( BrickGameBrickGame _this, XObject sender );

/* The method 'CheckCollision' is responsible to detect any ball collision with 
   borders, paddle or other elements. */
void BrickGameBrickGame_CheckCollision( BrickGameBrickGame _this, BrickGameBallPosition 
  aBall );

/* 'C' function for method : 'BrickGame::BrickGame.leftKeyDown()' */
void BrickGameBrickGame_leftKeyDown( BrickGameBrickGame _this, XObject sender );

/* 'C' function for method : 'BrickGame::BrickGame.rightKeyDown()' */
void BrickGameBrickGame_rightKeyDown( BrickGameBrickGame _this, XObject sender );

/* 'C' function for method : 'BrickGame::BrickGame.keyUpHandler()' */
void BrickGameBrickGame_keyUpHandler( BrickGameBrickGame _this, XObject sender );

/* 'C' function for method : 'BrickGame::BrickGame.updatePaddle()' */
void BrickGameBrickGame_updatePaddle( BrickGameBrickGame _this, XObject sender );

/* Slot method to start non-moving ball. */
void BrickGameBrickGame_onStartBall( BrickGameBrickGame _this, XObject sender );

/* Slot method to trigger the next phase of the auto demo. */
void BrickGameBrickGame_AutoDemoSlot( BrickGameBrickGame _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _BrickGameBrickGame_H */

/* Embedded Wizard */
