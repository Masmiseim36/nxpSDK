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
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#include "ewlocale.h"
#include "_CoreOutline.h"
#include "_CoreSlideTouchHandler.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_EffectsInt32Effect.h"
#include "_GraphicsPath.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "_ViewsFrame.h"
#include "_ViewsLine.h"
#include "_ViewsRectangle.h"
#include "_ViewsStrokePath.h"
#include "_ViewsText.h"
#include "_WaveformChannelPanel.h"
#include "_WaveformWaveform.h"
#include "_WidgetSetHorizontalSlider.h"
#include "_WidgetSetHorizontalSliderConfig.h"
#include "_WidgetSetRadioButton.h"
#include "_WidgetSetRadioButtonConfig.h"
#include "Core.h"
#include "Resources.h"
#include "Views.h"
#include "Waveform.h"
#include "WidgetSet.h"

/* Strings for the language 'Default'. */
static const unsigned short _StringsDefault0[] =
{
  0xFFFF, 0xFFFF, 0xC557, 0x0043, 0x0068, 0x0061, 0x006E, 0x006E, 0x0065, 0x006C,
  0x0020, 0x0031, 0x0000, 0xC557, 0x0043, 0x0068, 0x0061, 0x006E, 0x006E, 0x0065,
  0x006C, 0x0020, 0x0032, 0x0000, 0xC557, 0x0043, 0x0068, 0x0031, 0x003A, 0x0000,
  0xC557, 0x0057, 0x0061, 0x0076, 0x0065, 0x0066, 0x006F, 0x0072, 0x006D, 0x0020,
  0x0047, 0x0065, 0x006E, 0x0065, 0x0072, 0x0061, 0x0074, 0x006F, 0x0072, 0x0020,
  0x0044, 0x0065, 0x006D, 0x006F, 0x0000, 0xC557, 0x0043, 0x0068, 0x0031, 0x003A,
  0x000A, 0x0000, 0xC557, 0x0020, 0x0048, 0x007A, 0x000A, 0x0000, 0xC557, 0x0020,
  0x006D, 0x0056, 0x000A, 0x0000, 0xC557, 0x0073, 0x0069, 0x006E, 0x0065, 0x0000,
  0xC557, 0x0073, 0x0071, 0x0075, 0x0061, 0x0072, 0x0065, 0x0000, 0xC557, 0x0072,
  0x0061, 0x006D, 0x0070, 0x0000, 0xC557, 0x0043, 0x0068, 0x0032, 0x003A, 0x000A,
  0x0000, 0xC557, 0x0054, 0x0065, 0x0078, 0x0074, 0x0000, 0xC557, 0x0046, 0x0072,
  0x0065, 0x0071, 0x0075, 0x0065, 0x006E, 0x0063, 0x0079, 0x0020, 0x005B, 0x0048,
  0x007A, 0x005D, 0x0000, 0xC557, 0x0041, 0x006D, 0x0070, 0x006C, 0x0069, 0x0074,
  0x0075, 0x0064, 0x0065, 0x0020, 0x005B, 0x006D, 0x0056, 0x005D, 0x0000, 0xC557,
  0x0043, 0x0068, 0x0061, 0x006E, 0x006E, 0x0065, 0x006C, 0x0000
};

/* Constant values used in this 'C' module only. */
static const XRect _Const0000 = {{ 0, 0 }, { 320, 240 }};
static const XColor _Const0001 = { 0x00, 0x00, 0x00, 0xFF };
static const XRect _Const0002 = {{ 5, 30 }, { 175, 235 }};
static const XRect _Const0003 = {{ 160, 0 }, { 320, 240 }};
static const XPoint _Const0004 = { 0, 210 };
static const XRect _Const0005 = {{ 180, 30 }, { 320, 240 }};
static const XStringRes _Const0006 = { _StringsDefault0, 0x0003 };
static const XRect _Const0007 = {{ 320, 30 }, { 460, 240 }};
static const XStringRes _Const0008 = { _StringsDefault0, 0x000E };
static const XPoint _Const0009 = { 170, 135 };
static const XPoint _Const000A = { 10, 135 };
static const XColor _Const000B = { 0x0C, 0x0E, 0x6E, 0xFF };
static const XPoint _Const000C = { 170, 66 };
static const XPoint _Const000D = { 10, 66 };
static const XPoint _Const000E = { 170, 204 };
static const XPoint _Const000F = { 10, 204 };
static const XRect _Const0010 = {{ 10, 35 }, { 170, 230 }};
static const XRect _Const0011 = {{ 10, 140 }, { 90, 230 }};
static const XStringRes _Const0012 = { _StringsDefault0, 0x0019 };
static const XRect _Const0013 = {{ 85, 140 }, { 170, 230 }};
static const XRect _Const0014 = {{ 5, 0 }, { 300, 30 }};
static const XStringRes _Const0015 = { _StringsDefault0, 0x001F };
static const XStringRes _Const0016 = { _StringsDefault0, 0x0038 };
static const XStringRes _Const0017 = { _StringsDefault0, 0x003F };
static const XStringRes _Const0018 = { _StringsDefault0, 0x0045 };
static const XStringRes _Const0019 = { _StringsDefault0, 0x004B };
static const XStringRes _Const001A = { _StringsDefault0, 0x0051 };
static const XStringRes _Const001B = { _StringsDefault0, 0x0059 };
static const XStringRes _Const001C = { _StringsDefault0, 0x005F };
static const XRect _Const001D = {{ 0, 0 }, { 140, 210 }};
static const XRect _Const001E = {{ 5, 20 }, { 135, 205 }};
static const XRect _Const001F = {{ 5, 0 }, { 120, 30 }};
static const XStringRes _Const0020 = { _StringsDefault0, 0x0066 };
static const XRect _Const0021 = {{ 17, 20 }, { 122, 70 }};
static const XRect _Const0022 = {{ 17, 75 }, { 122, 125 }};
static const XRect _Const0023 = {{ 17, 58 }, { 133, 78 }};
static const XStringRes _Const0024 = { _StringsDefault0, 0x006C };
static const XRect _Const0025 = {{ 17, 113 }, { 143, 133 }};
static const XStringRes _Const0026 = { _StringsDefault0, 0x007C };
static const XRect _Const0027 = {{ 15, 140 }, { 45, 200 }};
static const XRect _Const0028 = {{ 55, 140 }, { 85, 200 }};
static const XRect _Const0029 = {{ 95, 140 }, { 125, 200 }};
static const XStringRes _Const002A = { _StringsDefault0, 0x008C };
static const XColor _Const002B = { 0x6C, 0x6E, 0x70, 0xFF };
static const XColor _Const002C = { 0xA8, 0xAB, 0xAB, 0xFF };
static const XPoint _Const002D = { 26, 26 };

/* Initializer for the class 'Waveform::Waveform' */
void WaveformWaveform__Init( WaveformWaveform _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WaveformWaveform );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  ViewsFrame__Init( &_this->Frame, &_this->_XObject, 0 );
  CoreSlideTouchHandler__Init( &_this->SlideTouchHandler, &_this->_XObject, 0 );
  CoreOutline__Init( &_this->Outline, &_this->_XObject, 0 );
  WaveformChannelPanel__Init( &_this->ChannelPanel1, &_this->_XObject, 0 );
  WaveformChannelPanel__Init( &_this->ChannelPanel2, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->RecalcTimer, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->AutoDemoTimer, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->LineCenter, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->LineTop, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->LineBotom, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->Path, &_this->_XObject, 0 );
  ViewsStrokePath__Init( &_this->StrokePath, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextCh1, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextCh2, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Caption, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WaveformWaveform );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0000 );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const0000 );
  ViewsRectangle_OnSetColor( &_this->Rectangle, _Const0001 );
  CoreRectView__OnSetBounds( &_this->Frame, _Const0002 );
  ViewsFrame_OnSetColor( &_this->Frame, WaveformThemeColor );
  CoreRectView__OnSetBounds( &_this->SlideTouchHandler, _Const0003 );
  CoreSlideTouchHandler_OnSetSnapNext( &_this->SlideTouchHandler, _Const0004 );
  _this->SlideTouchHandler.SlideHorz = 0;
  CoreSlideTouchHandler_OnSetResetSpace( &_this->SlideTouchHandler, 50 );
  CoreRectView__OnSetBounds( &_this->Outline, _Const0005 );
  CoreOutline_OnSetFormation( &_this->Outline, CoreFormationTopToBottom );
  CoreRectView__OnSetBounds( &_this->ChannelPanel1, _Const0005 );
  CoreGroup_OnSetEmbedded((CoreGroup)&_this->ChannelPanel1, 1 );
  _this->ChannelPanel1.Frequency = 100;
  _this->ChannelPanel1.Amplitude = 14;
  WaveformChannelPanel_OnSetCaption( &_this->ChannelPanel1, EwLoadString( &_Const0006 ));
  CoreRectView__OnSetBounds( &_this->ChannelPanel2, _Const0007 );
  CoreGroup_OnSetEmbedded((CoreGroup)&_this->ChannelPanel2, 1 );
  _this->ChannelPanel2.Frequency = 71;
  _this->ChannelPanel2.Amplitude = 15;
  WaveformChannelPanel_OnSetWaveform( &_this->ChannelPanel2, WaveformWaveformTypeSquare );
  WaveformChannelPanel_OnSetCaption( &_this->ChannelPanel2, EwLoadString( &_Const0008 ));
  CoreTimer_OnSetPeriod( &_this->RecalcTimer, 250 );
  _this->AutoDemoTimer.Super1.Symmetric = 1;
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->AutoDemoTimer, 12000 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->AutoDemoTimer, 0 );
  _this->AutoDemoTimer.Value2 = 140;
  _this->AutoDemoTimer.Value1 = 60;
  CoreLineView_OnSetPoint2((CoreLineView)&_this->LineCenter, _Const0009 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->LineCenter, _Const000A );
  ViewsLine_OnSetColor( &_this->LineCenter, _Const000B );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->LineTop, _Const000C );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->LineTop, _Const000D );
  ViewsLine_OnSetColor( &_this->LineTop, _Const000B );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->LineBotom, _Const000E );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->LineBotom, _Const000F );
  ViewsLine_OnSetColor( &_this->LineBotom, _Const000B );
  CoreView_OnSetLayout((CoreView)&_this->StrokePath, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->StrokePath, _Const0010 );
  ViewsStrokePath_OnSetWidth( &_this->StrokePath, 2.500000f );
  CoreRectView__OnSetBounds( &_this->TextCh1, _Const0011 );
  ViewsText_OnSetAlignment( &_this->TextCh1, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertBottom );
  ViewsText_OnSetString( &_this->TextCh1, EwLoadString( &_Const0012 ));
  ViewsText_OnSetColor( &_this->TextCh1, WaveformThemeColor );
  CoreRectView__OnSetBounds( &_this->TextCh2, _Const0013 );
  ViewsText_OnSetAlignment( &_this->TextCh2, ViewsTextAlignmentAlignHorzRight | 
  ViewsTextAlignmentAlignVertBottom );
  ViewsText_OnSetString( &_this->TextCh2, EwLoadString( &_Const0012 ));
  ViewsText_OnSetColor( &_this->TextCh2, WaveformThemeColor );
  CoreRectView__OnSetBounds( &_this->Caption, _Const0014 );
  ViewsText_OnSetAlignment( &_this->Caption, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Caption, EwLoadString( &_Const0015 ));
  ViewsText_OnSetColor( &_this->Caption, WaveformThemeColor );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Frame ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SlideTouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Outline ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ChannelPanel1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ChannelPanel2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LineCenter ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LineTop ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LineBotom ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->StrokePath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextCh1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextCh2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Caption ), 0 );
  CoreGroup__OnSetFocus( _this, 0 );
  ViewsFrame_OnSetBitmap( &_this->Frame, EwLoadResource( &WaveformPanelBorder, ResourcesBitmap ));
  CoreOutline_OnSetSlideHandler( &_this->Outline, &_this->SlideTouchHandler );
  _this->ChannelPanel1.OnChanged = EwNewSlot( _this, WaveformWaveform_SettingsChanged );
  _this->ChannelPanel2.OnChanged = EwNewSlot( _this, WaveformWaveform_SettingsChanged );
  _this->RecalcTimer.OnTrigger = EwNewSlot( _this, WaveformWaveform_TimerSlot );
  _this->AutoDemoTimer.Super1.OnAnimate = EwNewSlot( _this, WaveformWaveform_AnimateFrequency );
  _this->AutoDemoTimer.Outlet = EwNewRef( &_this->ChannelPanel1, WaveformChannelPanel_OnGetFrequency, 
  WaveformChannelPanel_OnSetFrequency );
  ViewsStrokePath_OnSetPath( &_this->StrokePath, &_this->Path );
  ViewsText_OnSetFont( &_this->TextCh1, EwLoadResource( &ResourcesFontSmall, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextCh2, EwLoadResource( &ResourcesFontSmall, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Caption, EwLoadResource( &ResourcesFontMedium, ResourcesFont ));

  /* Call the user defined constructor */
  WaveformWaveform_Init( _this, aArg );
}

/* Re-Initializer for the class 'Waveform::Waveform' */
void WaveformWaveform__ReInit( WaveformWaveform _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Rectangle );
  ViewsFrame__ReInit( &_this->Frame );
  CoreSlideTouchHandler__ReInit( &_this->SlideTouchHandler );
  CoreOutline__ReInit( &_this->Outline );
  WaveformChannelPanel__ReInit( &_this->ChannelPanel1 );
  WaveformChannelPanel__ReInit( &_this->ChannelPanel2 );
  CoreTimer__ReInit( &_this->RecalcTimer );
  EffectsInt32Effect__ReInit( &_this->AutoDemoTimer );
  ViewsLine__ReInit( &_this->LineCenter );
  ViewsLine__ReInit( &_this->LineTop );
  ViewsLine__ReInit( &_this->LineBotom );
  GraphicsPath__ReInit( &_this->Path );
  ViewsStrokePath__ReInit( &_this->StrokePath );
  ViewsText__ReInit( &_this->TextCh1 );
  ViewsText__ReInit( &_this->TextCh2 );
  ViewsText__ReInit( &_this->Caption );
}

/* Finalizer method for the class 'Waveform::Waveform' */
void WaveformWaveform__Done( WaveformWaveform _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Rectangle );
  ViewsFrame__Done( &_this->Frame );
  CoreSlideTouchHandler__Done( &_this->SlideTouchHandler );
  CoreOutline__Done( &_this->Outline );
  WaveformChannelPanel__Done( &_this->ChannelPanel1 );
  WaveformChannelPanel__Done( &_this->ChannelPanel2 );
  CoreTimer__Done( &_this->RecalcTimer );
  EffectsInt32Effect__Done( &_this->AutoDemoTimer );
  ViewsLine__Done( &_this->LineCenter );
  ViewsLine__Done( &_this->LineTop );
  ViewsLine__Done( &_this->LineBotom );
  GraphicsPath__Done( &_this->Path );
  ViewsStrokePath__Done( &_this->StrokePath );
  ViewsText__Done( &_this->TextCh1 );
  ViewsText__Done( &_this->TextCh2 );
  ViewsText__Done( &_this->Caption );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void WaveformWaveform_Init( WaveformWaveform _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  EwSignal( EwNewSlot( _this, WaveformWaveform_SettingsChanged ), ((XObject)_this ));
}

/* The slot method 'SettingsChanged' is called when the user has changed the current 
   waveform gernator channel settings. */
void WaveformWaveform_SettingsChanged( WaveformWaveform _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  WaveformWaveform_CalculatePath( _this );
  ViewsText_OnSetString( &_this->TextCh1, EwConcatString( EwConcatString( EwConcatString( 
  EwConcatString( EwLoadString( &_Const0016 ), EwNewStringInt( _this->ChannelPanel1.Frequency, 
  0, 10 )), EwLoadString( &_Const0017 )), EwNewStringInt( _this->ChannelPanel1.Amplitude, 
  0, 10 )), EwLoadString( &_Const0018 )));

  if ( _this->ChannelPanel1.Waveform == WaveformWaveformTypeSinus )
    ViewsText_OnSetString( &_this->TextCh1, EwConcatString( _this->TextCh1.String, 
    EwLoadString( &_Const0019 )));
  else
    if ( _this->ChannelPanel1.Waveform == WaveformWaveformTypeSquare )
      ViewsText_OnSetString( &_this->TextCh1, EwConcatString( _this->TextCh1.String, 
      EwLoadString( &_Const001A )));
    else
      if ( _this->ChannelPanel1.Waveform == WaveformWaveformTypeRamp )
        ViewsText_OnSetString( &_this->TextCh1, EwConcatString( _this->TextCh1.String, 
        EwLoadString( &_Const001B )));

  ViewsText_OnSetString( &_this->TextCh2, EwConcatString( EwConcatString( EwConcatString( 
  EwConcatString( EwLoadString( &_Const001C ), EwNewStringInt( _this->ChannelPanel2.Frequency, 
  0, 10 )), EwLoadString( &_Const0017 )), EwNewStringInt( _this->ChannelPanel2.Amplitude, 
  0, 10 )), EwLoadString( &_Const0018 )));

  if ( _this->ChannelPanel2.Waveform == WaveformWaveformTypeSinus )
    ViewsText_OnSetString( &_this->TextCh2, EwConcatString( _this->TextCh2.String, 
    EwLoadString( &_Const0019 )));
  else
    if ( _this->ChannelPanel2.Waveform == WaveformWaveformTypeSquare )
      ViewsText_OnSetString( &_this->TextCh2, EwConcatString( _this->TextCh2.String, 
      EwLoadString( &_Const001A )));
    else
      if ( _this->ChannelPanel2.Waveform == WaveformWaveformTypeRamp )
        ViewsText_OnSetString( &_this->TextCh2, EwConcatString( _this->TextCh2.String, 
        EwLoadString( &_Const001B )));

  CoreTimer_OnSetEnabled( &_this->RecalcTimer, (XBool)(( _this->ChannelPanel1.Waveform 
  == WaveformWaveformTypeNoise ) || ( _this->ChannelPanel2.Waveform == WaveformWaveformTypeNoise )));
}

/* Slot method to force the recalculation of coordinates in case of a noise waveform. */
void WaveformWaveform_TimerSlot( WaveformWaveform _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  WaveformWaveform_CalculatePath( _this );
}

/* This is a slot method. */
void WaveformWaveform_AnimateFrequency( WaveformWaveform _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwNotifyRefObservers( EwNewRef( &_this->ChannelPanel1, WaveformChannelPanel_OnGetFrequency, 
    WaveformChannelPanel_OnSetFrequency ), 0 );
  EwPostSignal( EwNewSlot( _this, WaveformWaveform_SettingsChanged ), ((XObject)_this ));
}

/* The method 'CalculatePath' contains all the mathematics to calculate the waveform 
   path (= all graph coordinates) according the current waveform generator settings. */
void WaveformWaveform_CalculatePath( WaveformWaveform _this )
{
  XFloat f1 = (XFloat)_this->ChannelPanel1.Frequency;
  XFloat a1 = (XFloat)_this->ChannelPanel1.Amplitude / 20.000000f;
  XFloat f2 = (XFloat)_this->ChannelPanel2.Frequency;
  XFloat a2 = (XFloat)_this->ChannelPanel2.Amplitude / 20.000000f;
  XFloat r1 = 0.000000f;
  XFloat r2 = 0.000000f;
  XFloat x = 0.000000f;
  XFloat y = 0.000000f;
  XFloat yOffset = (XFloat)EwGetRectH( _this->StrokePath.Super1.Bounds ) / 2.000000f;
  XFloat xFactor = (XFloat)EwGetRectW( _this->StrokePath.Super1.Bounds ) / 4.000000f;
  XFloat yFactor = (XFloat)EwGetRectH( _this->StrokePath.Super1.Bounds ) / 4.000000f;

  GraphicsPath_InitSubPath( &_this->Path, 0, 82 );

  while ( x < 4.000000f )
  {
    if ( _this->ChannelPanel1.Waveform == WaveformWaveformTypeSinus )
      r1 = EwMathSin((( f1 * x ) * 360.000000f ) / 100.000000f ) * a1;
    else
      if ( _this->ChannelPanel1.Waveform == WaveformWaveformTypeSquare )
        r1 = (((XFloat)((XInt32)((( f1 * x ) * 2.000000f ) / 100.000000f ) & 1 ) 
        * 2.000000f ) - 1.000000f ) * a1;
      else
        if ( _this->ChannelPanel1.Waveform == WaveformWaveformTypeRamp )
          r1 = ((((XFloat)((XInt32)(( f1 * x ) * 1.000000f ) % 100 ) / 100.000000f ) 
          * -2.000000f ) + 1.000000f ) * a1;
        else
          r1 = ((XFloat)EwMathRandInt32( -100, 100 ) * a1 ) / 100.000000f;

    if ( _this->ChannelPanel2.Waveform == WaveformWaveformTypeSinus )
      r2 = EwMathSin((( f2 * x ) * 360.000000f ) / 100.000000f ) * a2;
    else
      if ( _this->ChannelPanel2.Waveform == WaveformWaveformTypeSquare )
        r2 = (((XFloat)((XInt32)((( f2 * x ) * 2.000000f ) / 100.000000f ) & 1 ) 
        * 2.000000f ) - 1.000000f ) * a2;
      else
        if ( _this->ChannelPanel2.Waveform == WaveformWaveformTypeRamp )
          r2 = ((((XFloat)((XInt32)(( f2 * x ) * 1.000000f ) % 100 ) / 100.000000f ) 
          * -2.000000f ) + 1.000000f ) * a2;
        else
          r2 = ((XFloat)EwMathRandInt32( -100, 100 ) * a2 ) / 100.000000f;

    y = (( r1 + r2 ) * yFactor ) + yOffset;

    if ( x == 0.000000f )
      GraphicsPath_Begin( &_this->Path, 0, x * xFactor, y );

    GraphicsPath_AddLine( &_this->Path, 0, x * xFactor, y );
    x = x + 0.050000f;
  }
}

/* Variants derived from the class : 'Waveform::Waveform' */
EW_DEFINE_CLASS_VARIANTS( WaveformWaveform )
EW_END_OF_CLASS_VARIANTS( WaveformWaveform )

/* Virtual Method Table (VMT) for the class : 'Waveform::Waveform' */
EW_DEFINE_CLASS( WaveformWaveform, CoreGroup, Rectangle, Rectangle, Rectangle, Rectangle, 
                 _None, _None, "Waveform::Waveform" )
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
EW_END_OF_CLASS( WaveformWaveform )

/* Initializer for the class 'Waveform::ChannelPanel' */
void WaveformChannelPanel__Init( WaveformChannelPanel _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WaveformChannelPanel );

  /* ... then construct all embedded objects */
  ViewsFrame__Init( &_this->Frame, &_this->_XObject, 0 );
  ViewsText__Init( &_this->CaptionText, &_this->_XObject, 0 );
  WidgetSetHorizontalSlider__Init( &_this->SliderFrequency, &_this->_XObject, 0 );
  WidgetSetHorizontalSlider__Init( &_this->SliderAmplitude, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text1, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text2, &_this->_XObject, 0 );
  WidgetSetRadioButton__Init( &_this->ButtonSinus, &_this->_XObject, 0 );
  WidgetSetRadioButton__Init( &_this->ButtonSquare, &_this->_XObject, 0 );
  WidgetSetRadioButton__Init( &_this->ButtonRamp, &_this->_XObject, 0 );
  WidgetSetRadioButtonConfig__Init( &_this->CheckBox, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WaveformChannelPanel );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const001D );
  CoreRectView__OnSetBounds( &_this->Frame, _Const001E );
  ViewsFrame_OnSetColor( &_this->Frame, WaveformThemeColor );
  CoreRectView__OnSetBounds( &_this->CaptionText, _Const001F );
  ViewsText_OnSetAlignment( &_this->CaptionText, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->CaptionText, EwLoadString( &_Const0020 ));
  ViewsText_OnSetColor( &_this->CaptionText, WaveformThemeColor );
  CoreRectView__OnSetBounds( &_this->SliderFrequency, _Const0021 );
  WidgetSetHorizontalSlider_OnSetMaxValue( &_this->SliderFrequency, 200 );
  WidgetSetHorizontalSlider_OnSetMinValue( &_this->SliderFrequency, 10 );
  CoreRectView__OnSetBounds( &_this->SliderAmplitude, _Const0022 );
  WidgetSetHorizontalSlider_OnSetMaxValue( &_this->SliderAmplitude, 50 );
  CoreRectView__OnSetBounds( &_this->Text1, _Const0023 );
  ViewsText_OnSetAlignment( &_this->Text1, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text1, EwLoadString( &_Const0024 ));
  ViewsText_OnSetColor( &_this->Text1, WaveformThemeColor );
  CoreRectView__OnSetBounds( &_this->Text2, _Const0025 );
  ViewsText_OnSetAlignment( &_this->Text2, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text2, EwLoadString( &_Const0026 ));
  ViewsText_OnSetColor( &_this->Text2, WaveformThemeColor );
  _this->Frequency = 100;
  _this->Amplitude = 10;
  _this->Waveform = WaveformWaveformTypeSinus;
  CoreRectView__OnSetBounds( &_this->ButtonSinus, _Const0027 );
  WidgetSetRadioButton_OnSetSelected( &_this->ButtonSinus, 1 );
  WidgetSetRadioButton_OnSetIconOnFrameActive( &_this->ButtonSinus, 0 );
  WidgetSetRadioButton_OnSetIconOnFrameFocused( &_this->ButtonSinus, 0 );
  WidgetSetRadioButton_OnSetIconOnFrameDisabled( &_this->ButtonSinus, 0 );
  WidgetSetRadioButton_OnSetIconOnFrameDefault( &_this->ButtonSinus, 0 );
  WidgetSetRadioButton_OnSetIconOffFrameActive( &_this->ButtonSinus, 0 );
  WidgetSetRadioButton_OnSetIconOffFrameFocused( &_this->ButtonSinus, 0 );
  WidgetSetRadioButton_OnSetIconOffFrameDisabled( &_this->ButtonSinus, 0 );
  WidgetSetRadioButton_OnSetIconOffFrameDefault( &_this->ButtonSinus, 0 );
  CoreRectView__OnSetBounds( &_this->ButtonSquare, _Const0028 );
  WidgetSetRadioButton_OnSetIconOnFrameActive( &_this->ButtonSquare, 1 );
  WidgetSetRadioButton_OnSetIconOnFrameFocused( &_this->ButtonSquare, 1 );
  WidgetSetRadioButton_OnSetIconOnFrameDisabled( &_this->ButtonSquare, 1 );
  WidgetSetRadioButton_OnSetIconOnFrameDefault( &_this->ButtonSquare, 1 );
  WidgetSetRadioButton_OnSetIconOffFrameActive( &_this->ButtonSquare, 1 );
  WidgetSetRadioButton_OnSetIconOffFrameFocused( &_this->ButtonSquare, 1 );
  WidgetSetRadioButton_OnSetIconOffFrameDisabled( &_this->ButtonSquare, 1 );
  WidgetSetRadioButton_OnSetIconOffFrameDefault( &_this->ButtonSquare, 1 );
  CoreRectView__OnSetBounds( &_this->ButtonRamp, _Const0029 );
  WidgetSetRadioButton_OnSetIconOnFrameActive( &_this->ButtonRamp, 2 );
  WidgetSetRadioButton_OnSetIconOnFrameFocused( &_this->ButtonRamp, 2 );
  WidgetSetRadioButton_OnSetIconOnFrameDisabled( &_this->ButtonRamp, 2 );
  WidgetSetRadioButton_OnSetIconOnFrameDefault( &_this->ButtonRamp, 2 );
  WidgetSetRadioButton_OnSetIconOffFrameActive( &_this->ButtonRamp, 2 );
  WidgetSetRadioButton_OnSetIconOffFrameFocused( &_this->ButtonRamp, 2 );
  WidgetSetRadioButton_OnSetIconOffFrameDisabled( &_this->ButtonRamp, 2 );
  WidgetSetRadioButton_OnSetIconOffFrameDefault( &_this->ButtonRamp, 2 );
  _this->Caption = EwShareString( EwLoadString( &_Const002A ));
  WidgetSetRadioButtonConfig_OnSetLabelOnColorActive( &_this->CheckBox, _Const002B );
  WidgetSetRadioButtonConfig_OnSetLabelOnColorFocused( &_this->CheckBox, _Const002B );
  WidgetSetRadioButtonConfig_OnSetLabelOnColorDisabled( &_this->CheckBox, _Const002C );
  WidgetSetRadioButtonConfig_OnSetLabelOnColorDefault( &_this->CheckBox, _Const002B );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorActive( &_this->CheckBox, _Const002B );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorFocused( &_this->CheckBox, _Const002B );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorDisabled( &_this->CheckBox, _Const002C );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorDefault( &_this->CheckBox, _Const002B );
  WidgetSetRadioButtonConfig_OnSetLabelMarginLeft( &_this->CheckBox, 34 );
  WidgetSetRadioButtonConfig_OnSetIconOnTintActive( &_this->CheckBox, WaveformThemeColor );
  WidgetSetRadioButtonConfig_OnSetIconOnTintFocused( &_this->CheckBox, WaveformThemeColor );
  WidgetSetRadioButtonConfig_OnSetIconOnTintDisabled( &_this->CheckBox, WaveformThemeColor );
  WidgetSetRadioButtonConfig_OnSetIconOnTintDefault( &_this->CheckBox, WaveformThemeColor );
  WidgetSetRadioButtonConfig_OnSetIconOffTintActive( &_this->CheckBox, WaveformThemeColor );
  WidgetSetRadioButtonConfig_OnSetIconOffTintFocused( &_this->CheckBox, WaveformThemeColor );
  WidgetSetRadioButtonConfig_OnSetIconOffTintDisabled( &_this->CheckBox, WaveformThemeColor );
  WidgetSetRadioButtonConfig_OnSetIconOffTintDefault( &_this->CheckBox, WaveformThemeColor );
  WidgetSetRadioButtonConfig_OnSetIconMarginTop( &_this->CheckBox, 30 );
  WidgetSetRadioButtonConfig_OnSetLabelAlignment( &_this->CheckBox, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  WidgetSetRadioButtonConfig_OnSetIconAlignment( &_this->CheckBox, ViewsImageAlignmentAlignHorzCenter 
  | ViewsImageAlignmentAlignVertTop );
  WidgetSetRadioButtonConfig_OnSetFaceLayout( &_this->CheckBox, CoreLayoutAlignToTop );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameActive( &_this->CheckBox, 7 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameFocused( &_this->CheckBox, 5 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameDisabled( &_this->CheckBox, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameDefault( &_this->CheckBox, 3 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameActive( &_this->CheckBox, 6 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameFocused( &_this->CheckBox, 4 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameDisabled( &_this->CheckBox, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameDefault( &_this->CheckBox, 2 );
  WidgetSetRadioButtonConfig_OnSetWidgetMinSize( &_this->CheckBox, _Const002D );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Frame ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CaptionText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SliderFrequency ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SliderAmplitude ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ButtonSinus ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ButtonSquare ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ButtonRamp ), 0 );
  ViewsFrame_OnSetBitmap( &_this->Frame, EwLoadResource( &WaveformPanelBorder, ResourcesBitmap ));
  ViewsText_OnSetFont( &_this->CaptionText, EwLoadResource( &ResourcesFontMedium, 
  ResourcesFont ));
  _this->SliderFrequency.OnChange = EwNewSlot( _this, WaveformChannelPanel_OnSlider );
  WidgetSetHorizontalSlider_OnSetOutlet( &_this->SliderFrequency, EwNewRef( _this, 
  WaveformChannelPanel_OnGetFrequency, WaveformChannelPanel_OnSetFrequency ));
  WidgetSetHorizontalSlider_OnSetAppearance( &_this->SliderFrequency, EwGetAutoObject( 
  &WidgetSetHorizontalSlider_Small, WidgetSetHorizontalSliderConfig ));
  _this->SliderAmplitude.OnChange = EwNewSlot( _this, WaveformChannelPanel_OnSlider );
  WidgetSetHorizontalSlider_OnSetOutlet( &_this->SliderAmplitude, EwNewRef( _this, 
  WaveformChannelPanel_OnGetAmplitude, WaveformChannelPanel_OnSetAmplitude ));
  WidgetSetHorizontalSlider_OnSetAppearance( &_this->SliderAmplitude, EwGetAutoObject( 
  &WidgetSetHorizontalSlider_Small, WidgetSetHorizontalSliderConfig ));
  ViewsText_OnSetFont( &_this->Text1, EwLoadResource( &ResourcesFontSmall, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text2, EwLoadResource( &ResourcesFontSmall, ResourcesFont ));
  _this->ButtonSinus.OnSelect = EwNewSlot( _this, WaveformChannelPanel_OnSinus );
  WidgetSetRadioButton_OnSetIcon( &_this->ButtonSinus, EwLoadResource( &WaveformWaveformIcons, 
  ResourcesBitmap ));
  WidgetSetRadioButton_OnSetAppearance( &_this->ButtonSinus, &_this->CheckBox );
  _this->ButtonSquare.OnSelect = EwNewSlot( _this, WaveformChannelPanel_OnSquare );
  WidgetSetRadioButton_OnSetIcon( &_this->ButtonSquare, EwLoadResource( &WaveformWaveformIcons, 
  ResourcesBitmap ));
  WidgetSetRadioButton_OnSetAppearance( &_this->ButtonSquare, &_this->CheckBox );
  _this->ButtonRamp.OnSelect = EwNewSlot( _this, WaveformChannelPanel_OnRamp );
  WidgetSetRadioButton_OnSetIcon( &_this->ButtonRamp, EwLoadResource( &WaveformWaveformIcons, 
  ResourcesBitmap ));
  WidgetSetRadioButton_OnSetAppearance( &_this->ButtonRamp, &_this->CheckBox );
  WidgetSetRadioButtonConfig_OnSetLabelOnFont( &_this->CheckBox, EwLoadResource( 
  &ResourcesFontSmall, ResourcesFont ));
  WidgetSetRadioButtonConfig_OnSetLabelOffFont( &_this->CheckBox, EwLoadResource( 
  &ResourcesFontSmall, ResourcesFont ));
  WidgetSetRadioButtonConfig_OnSetFaceOnActive( &_this->CheckBox, EwLoadResource( 
  &WidgetSetCheckBoxSmall, ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnFocused( &_this->CheckBox, EwLoadResource( 
  &WidgetSetCheckBoxSmall, ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnDisabled( &_this->CheckBox, EwLoadResource( 
  &WidgetSetCheckBoxSmall, ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnDefault( &_this->CheckBox, EwLoadResource( 
  &WidgetSetCheckBoxSmall, ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffActive( &_this->CheckBox, EwLoadResource( 
  &WidgetSetCheckBoxSmall, ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffFocused( &_this->CheckBox, EwLoadResource( 
  &WidgetSetCheckBoxSmall, ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffDisabled( &_this->CheckBox, EwLoadResource( 
  &WidgetSetCheckBoxSmall, ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffDefault( &_this->CheckBox, EwLoadResource( 
  &WidgetSetCheckBoxSmall, ResourcesBitmap ));
}

/* Re-Initializer for the class 'Waveform::ChannelPanel' */
void WaveformChannelPanel__ReInit( WaveformChannelPanel _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsFrame__ReInit( &_this->Frame );
  ViewsText__ReInit( &_this->CaptionText );
  WidgetSetHorizontalSlider__ReInit( &_this->SliderFrequency );
  WidgetSetHorizontalSlider__ReInit( &_this->SliderAmplitude );
  ViewsText__ReInit( &_this->Text1 );
  ViewsText__ReInit( &_this->Text2 );
  WidgetSetRadioButton__ReInit( &_this->ButtonSinus );
  WidgetSetRadioButton__ReInit( &_this->ButtonSquare );
  WidgetSetRadioButton__ReInit( &_this->ButtonRamp );
  WidgetSetRadioButtonConfig__ReInit( &_this->CheckBox );
}

/* Finalizer method for the class 'Waveform::ChannelPanel' */
void WaveformChannelPanel__Done( WaveformChannelPanel _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsFrame__Done( &_this->Frame );
  ViewsText__Done( &_this->CaptionText );
  WidgetSetHorizontalSlider__Done( &_this->SliderFrequency );
  WidgetSetHorizontalSlider__Done( &_this->SliderAmplitude );
  ViewsText__Done( &_this->Text1 );
  ViewsText__Done( &_this->Text2 );
  WidgetSetRadioButton__Done( &_this->ButtonSinus );
  WidgetSetRadioButton__Done( &_this->ButtonSquare );
  WidgetSetRadioButton__Done( &_this->ButtonRamp );
  WidgetSetRadioButtonConfig__Done( &_this->CheckBox );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Slot method, called when the user changed a slider value. */
void WaveformChannelPanel_OnSlider( WaveformChannelPanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( _this->OnChanged, ((XObject)_this ));
}

/* Slot method, called when the user pressed the corresponding waveform selection 
   button. */
void WaveformChannelPanel_OnSinus( WaveformChannelPanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  WaveformChannelPanel_OnSetWaveform( _this, WaveformWaveformTypeSinus );
  EwPostSignal( _this->OnChanged, ((XObject)_this ));
}

/* 'C' function for method : 'Waveform::ChannelPanel.OnSetCaption()' */
void WaveformChannelPanel_OnSetCaption( WaveformChannelPanel _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->CaptionText, value );
}

/* Slot method, called when the user pressed the corresponding waveform selection 
   button. */
void WaveformChannelPanel_OnSquare( WaveformChannelPanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  WaveformChannelPanel_OnSetWaveform( _this, WaveformWaveformTypeSquare );
  EwPostSignal( _this->OnChanged, ((XObject)_this ));
}

/* Slot method, called when the user pressed the corresponding waveform selection 
   button. */
void WaveformChannelPanel_OnRamp( WaveformChannelPanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  WaveformChannelPanel_OnSetWaveform( _this, WaveformWaveformTypeRamp );
  EwPostSignal( _this->OnChanged, ((XObject)_this ));
}

/* 'C' function for method : 'Waveform::ChannelPanel.OnSetWaveform()' */
void WaveformChannelPanel_OnSetWaveform( WaveformChannelPanel _this, XEnum value )
{
  _this->Waveform = value;
  WidgetSetRadioButton_OnSetSelected( &_this->ButtonSinus, (XBool)( value == WaveformWaveformTypeSinus ));
  WidgetSetRadioButton_OnSetSelected( &_this->ButtonSquare, (XBool)( value == WaveformWaveformTypeSquare ));
  WidgetSetRadioButton_OnSetSelected( &_this->ButtonRamp, (XBool)( value == WaveformWaveformTypeRamp ));
}

/* Default onget method for the property 'Frequency' */
XInt32 WaveformChannelPanel_OnGetFrequency( WaveformChannelPanel _this )
{
  return _this->Frequency;
}

/* Default onset method for the property 'Frequency' */
void WaveformChannelPanel_OnSetFrequency( WaveformChannelPanel _this, XInt32 value )
{
  _this->Frequency = value;
}

/* Default onget method for the property 'Amplitude' */
XInt32 WaveformChannelPanel_OnGetAmplitude( WaveformChannelPanel _this )
{
  return _this->Amplitude;
}

/* Default onset method for the property 'Amplitude' */
void WaveformChannelPanel_OnSetAmplitude( WaveformChannelPanel _this, XInt32 value )
{
  _this->Amplitude = value;
}

/* Variants derived from the class : 'Waveform::ChannelPanel' */
EW_DEFINE_CLASS_VARIANTS( WaveformChannelPanel )
EW_END_OF_CLASS_VARIANTS( WaveformChannelPanel )

/* Virtual Method Table (VMT) for the class : 'Waveform::ChannelPanel' */
EW_DEFINE_CLASS( WaveformChannelPanel, CoreGroup, OnChanged, OnChanged, Frame, Frame, 
                 Caption, Frequency, "Waveform::ChannelPanel" )
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
EW_END_OF_CLASS( WaveformChannelPanel )

/* Include a file containing the bitmap resource : 'Waveform::PanelBorder' */
#include "_WaveformPanelBorder.h"

/* Table with links to derived variants of the bitmap resource : 'Waveform::PanelBorder' */
EW_RES_WITHOUT_VARIANTS( WaveformPanelBorder )

/* Include a file containing the bitmap resource : 'Waveform::WaveformIcons' */
#include "_WaveformWaveformIcons.h"

/* Table with links to derived variants of the bitmap resource : 'Waveform::WaveformIcons' */
EW_RES_WITHOUT_VARIANTS( WaveformWaveformIcons )

/* User defined constant: 'Waveform::ThemeColor' */
const XColor WaveformThemeColor = { 0xB2, 0xD3, 0x39, 0xFF };

/* Embedded Wizard */
