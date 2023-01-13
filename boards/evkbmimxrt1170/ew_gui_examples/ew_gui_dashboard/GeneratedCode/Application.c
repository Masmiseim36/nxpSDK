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
#include "_ApplicationApplication.h"
#include "_CoreCursorGrabEvent.h"
#include "_CoreGroup.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_DashboardColorsClass.h"
#include "_DashboardDeviceClass.h"
#include "_DashboardStartScreen.h"
#include "_EffectsEffectTimerClass.h"
#include "_EffectsFadeInOutTransition.h"
#include "_EffectsTransition.h"
#include "_ResourcesFont.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"
#include "Application.h"
#include "Dashboard.h"
#include "Resources.h"
#include "Views.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x00000026, /* ratio 105.26 % */
  0xB8001300, 0x0008C452, 0x00A60028, 0x040001D0, 0x22001800, 0xF0581822, 0x0D85C260,
  0x188C6A23, 0x00000010, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XRect _Const0000 = {{ 0, 0 }, { 1280, 720 }};
static const XRect _Const0001 = {{ 1170, 690 }, { 1270, 720 }};
static const XStringRes _Const0002 = { _StringsDefault0, 0x0002 };
static const XPoint _Const0003 = { 0, 0 };
static const XPoint _Const0004 = { 1280, 0 };
static const XPoint _Const0005 = { 1280, 720 };
static const XPoint _Const0006 = { 0, 720 };
static const XStringRes _Const0007 = { _StringsDefault0, 0x000B };

/* Initializer for the class 'Application::Application' */
void ApplicationApplication__Init( ApplicationApplication _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreRoot__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ApplicationApplication );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Background, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->fpsTimer, &_this->_XObject, 0 );
  DashboardDemoMode__Init( &_this->DemoMode, &_this->_XObject, 0 );
  EffectsEffectTimerClass__Init( &_this->inactiveTimer, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->AutoDemoTouchHandler, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ApplicationApplication );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( &_this->Background, _Const0000 );
  ViewsRectangle_OnSetColor( &_this->Background, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Background );
  CoreTimer_OnSetEnabled( &_this->fpsTimer, 1 );
  CoreTimer_OnSetPeriod((CoreTimer)&_this->inactiveTimer, 0 );
  CoreTimer_OnSetBegin((CoreTimer)&_this->inactiveTimer, 10000 );
  CoreTimer_OnSetEnabled((CoreTimer)&_this->inactiveTimer, 1 );
  CoreView_OnSetStackingPriority((CoreView)&_this->Text, 100 );
  CoreRectView__OnSetBounds( &_this->Text, _Const0001 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzRight | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0002 ));
  CoreQuadView__OnSetPoint4( &_this->AutoDemoTouchHandler, _Const0003 );
  CoreQuadView__OnSetPoint3( &_this->AutoDemoTouchHandler, _Const0004 );
  CoreQuadView__OnSetPoint2( &_this->AutoDemoTouchHandler, _Const0005 );
  CoreQuadView__OnSetPoint1( &_this->AutoDemoTouchHandler, _Const0006 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Background ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->AutoDemoTouchHandler ), 0 );
  _this->device = EwGetAutoObject( &DashboardDevice, DashboardDeviceClass );
  _this->fpsTimer.OnTrigger = EwNewSlot( _this, ApplicationApplication_onFpsSlot );
  _this->inactiveTimer.Super1.OnTrigger = EwNewSlot( _this, ApplicationApplication_onInactive );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &ResourcesDefaultFont, ResourcesFont ));

  /* Call the user defined constructor */
  ApplicationApplication_Init( _this, aArg );
}

/* Re-Initializer for the class 'Application::Application' */
void ApplicationApplication__ReInit( ApplicationApplication _this )
{
  /* At first re-initialize the super class ... */
  CoreRoot__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Background );
  CoreTimer__ReInit( &_this->fpsTimer );
  DashboardDemoMode__ReInit( &_this->DemoMode );
  EffectsEffectTimerClass__ReInit( &_this->inactiveTimer );
  ViewsText__ReInit( &_this->Text );
  CoreSimpleTouchHandler__ReInit( &_this->AutoDemoTouchHandler );
}

/* Finalizer method for the class 'Application::Application' */
void ApplicationApplication__Done( ApplicationApplication _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreRoot );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Background );
  CoreTimer__Done( &_this->fpsTimer );
  DashboardDemoMode__Done( &_this->DemoMode );
  EffectsEffectTimerClass__Done( &_this->inactiveTimer );
  ViewsText__Done( &_this->Text );
  CoreSimpleTouchHandler__Done( &_this->AutoDemoTouchHandler );

  /* Don't forget to deinitialize the super class ... */
  CoreRoot__Done( &_this->_Super );
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
XObject ApplicationApplication_HandleEvent( ApplicationApplication _this, CoreEvent 
  aEvent )
{
  CoreCursorGrabEvent event = EwCastObject( aEvent, CoreCursorGrabEvent );

  if (( event != 0 ) && event->Down )
  {
    CoreTimer_OnSetEnabled((CoreTimer)&_this->inactiveTimer, 0 );
    CoreTimer_OnSetEnabled((CoreTimer)&_this->inactiveTimer, 1 );
    DashboardDeviceClass_OnSetAutoDemoMode( _this->device, 0 );
  }

  return CoreView_HandleEvent((CoreView)_this, aEvent );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void ApplicationApplication_Init( ApplicationApplication _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  CoreRectView__OnSetBounds( _this, EwSetRectSize( _this->Super3.Bounds, _Const0005 ));
  CoreGroup_PresentDialog((CoreGroup)_this, ((CoreGroup)EwNewObject( DashboardStartScreen, 
  0 )), ((EffectsTransition)EwGetAutoObject( &DashboardFadeInOutCentered, EffectsFadeInOutTransition )), 
  0, 0, 0, 0, 0, EwNullSlot, EwNullSlot, 0 );
}

/* 'C' function for method : 'Application::Application.onInactive()' */
void ApplicationApplication_onInactive( ApplicationApplication _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  DashboardDeviceClass_OnSetAutoDemoMode( _this->device, 1 );
}

/* 'C' function for method : 'Application::Application.onFpsSlot()' */
void ApplicationApplication_onFpsSlot( ApplicationApplication _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  ViewsText_OnSetString( &_this->Text, EwConcatString( EwLoadString( &_Const0007 ), 
  EwNewStringInt( CoreRoot_GetFPS((CoreRoot)_this ), 0, 10 )));
}

/* Variants derived from the class : 'Application::Application' */
EW_DEFINE_CLASS_VARIANTS( ApplicationApplication )
EW_END_OF_CLASS_VARIANTS( ApplicationApplication )

/* Virtual Method Table (VMT) for the class : 'Application::Application' */
EW_DEFINE_CLASS( ApplicationApplication, CoreRoot, device, Background, Background, 
                 Background, _None, _None, "Application::Application" )
  CoreRectView_initLayoutContext,
  CoreRoot_GetRoot,
  CoreRoot_Draw,
  ApplicationApplication_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreRoot_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreRoot_OnSetFocus,
  CoreRoot_OnSetBuffered,
  CoreRoot_OnSetOpacity,
  CoreRoot_IsDialog,
  CoreRoot_DispatchEvent,
  CoreRoot_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreRoot_InvalidateArea,
EW_END_OF_CLASS( ApplicationApplication )

/* Embedded Wizard */
