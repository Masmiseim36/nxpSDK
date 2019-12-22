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
#include "_CoreSimpleTouchHandler.h"
#include "_CoreTime.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_CutterBarGraph.h"
#include "_CutterBatchDialog.h"
#include "_CutterBatchWindow.h"
#include "_CutterButton.h"
#include "_CutterCancelButton.h"
#include "_CutterCutter.h"
#include "_CutterDeviceClass.h"
#include "_CutterFeedButton.h"
#include "_CutterFeedDialog.h"
#include "_CutterFeedWindow.h"
#include "_CutterFormatDialog.h"
#include "_CutterFormatWindow.h"
#include "_CutterHorzSlider.h"
#include "_CutterMainButton.h"
#include "_CutterPaperCut.h"
#include "_CutterPaperFeed.h"
#include "_CutterRemainingWindow.h"
#include "_CutterSpeedButton.h"
#include "_CutterSpeedDialog.h"
#include "_CutterSpeedWindow.h"
#include "_CutterStatusBar.h"
#include "_CutterVertSlider.h"
#include "_CutterWindow.h"
#include "_EffectsInt32Effect.h"
#include "_EffectsRectEffect.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "_ViewsBorder.h"
#include "_ViewsImage.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"
#include "Core.h"
#include "Cutter.h"
#include "Effects.h"
#include "Views.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x0000040A, /* ratio 49.13 % */
  0xB8001700, 0x80000452, 0x00C20021, 0x0E800380, 0xDE003480, 0x10637000, 0x05818200,
  0x50011801, 0x32002386, 0x34602D15, 0x12400508, 0x02344E00, 0x08905828, 0x4A000500,
  0xF2A29416, 0x419A0009, 0x09595000, 0x0D41A5C0, 0x449A6323, 0xCF6592A9, 0xA66D4497,
  0x6674216C, 0x72009BCD, 0x32196CEE, 0x2993C968, 0x029CF099, 0x994C2B80, 0xC4DA3750,
  0x4FA793A9, 0x92094000, 0x6B759A1C, 0x9529A457, 0x960A6002, 0x52A858A6, 0x108C56CD,
  0x869D66D4, 0x75FC010B, 0x44A54208, 0x24DA0039, 0x41306C04, 0x5D432BD0, 0x115244EE,
  0x94005AA6, 0x823C9E0B, 0xA0E36CB9, 0x66840039, 0xC1B4BA21, 0x7F494102, 0xB1642CBE,
  0xD00031C3, 0xB51AB654, 0x06AE5429, 0x1D2E800F, 0xA08360DA, 0x562A3B34, 0x71629108,
  0x3A2A6700, 0x6195CCBE, 0x0D208EC7, 0x325CBDF2, 0x85B7DD67, 0x4C28005F, 0xA81566D9,
  0x965E2F06, 0x00066001, 0x286A001A, 0xA1D5CA0F, 0xC30EC442, 0x912C07B3, 0x7D0341A1,
  0x7F592C01, 0xF2000639, 0x5E77943A, 0x165DA845, 0xE5417DE0, 0x0E419E64, 0x5959163C,
  0x45B61594, 0x51761462, 0x654103D1, 0x94671436, 0x852124ED, 0x5068598A, 0x84B94105,
  0x594276D5, 0x854D13C7, 0x135345DD, 0x1B84EDB5, 0x965DF800, 0xDA087A13, 0x35455E98,
  0x0D939466, 0xC008263C, 0x88EA410D, 0x65E24543, 0x784753B4, 0x50516630, 0x1E6791FE,
  0x76E8008E, 0x00306859, 0xFD7CA590, 0x97E5A7FD, 0x9FE4665C, 0xF96E597A, 0x96479764,
  0xE4506489, 0x9866A7F9, 0xDE879106, 0x4466679B, 0x2674911E, 0x6A799069, 0x80A7A99A,
  0xE759827C, 0x4900009E, 0x211965A7, 0xB04510A4, 0x4546C475, 0xA5F98A56, 0x39C69199,
  0x0541A429, 0x7350694A, 0x8EE95A0A, 0xA86A7E74, 0x29E141A7, 0x04004568, 0x00000004,
  0x00000000
};

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault1[] =
{
  0x00000022, /* ratio 94.12 % */
  0xB8000F00, 0x00000452, 0x2460001C, 0xC1000834, 0x620082C0, 0x90A8542C, 0x0080C321,
  0x00000000
};

/* Constant values used in this 'C' module only. */
static const XRect _Const0000 = {{ 0, 0 }, { 380, 200 }};
static const XPoint _Const0001 = { 0, 200 };
static const XPoint _Const0002 = { 380, 200 };
static const XPoint _Const0003 = { 380, 0 };
static const XPoint _Const0004 = { 0, 0 };
static const XRect _Const0005 = {{ 0, 0 }, { 300, 40 }};
static const XRect _Const0006 = {{ 20, 0 }, { 300, 40 }};
static const XStringRes _Const0007 = { _StringsDefault0, 0x0003 };
static const XRect _Const0008 = {{ 0, 0 }, { 230, 114 }};
static const XRect _Const0009 = {{ 0, 0 }, { 300, 140 }};
static const XStringRes _Const000A = { _StringsDefault0, 0x000E };
static const XRect _Const000B = {{ 110, 80 }, { 230, 114 }};
static const XStringRes _Const000C = { _StringsDefault0, 0x0016 };
static const XRect _Const000D = {{ 0, 20 }, { 194, 92 }};
static const XPoint _Const000E = { 0, 22 };
static const XStringRes _Const000F = { _StringsDefault0, 0x002C };
static const XStringRes _Const0010 = { _StringsDefault0, 0x0046 };
static const XStringRes _Const0011 = { _StringsDefault0, 0x005C };
static const XStringRes _Const0012 = { _StringsDefault0, 0x0076 };
static const XRect _Const0013 = {{ 0, 50 }, { 92, 99 }};
static const XRect _Const0014 = {{ 96, 46 }, { 225, 71 }};
static const XStringRes _Const0015 = { _StringsDefault0, 0x0087 };
static const XRect _Const0016 = {{ 96, 76 }, { 225, 102 }};
static const XStringRes _Const0017 = { _StringsDefault0, 0x0090 };
static const XRect _Const0018 = {{ 139, 30 }, { 230, 76 }};
static const XStringRes _Const0019 = { _StringsDefault0, 0x009A };
static const XRect _Const001A = {{ 139, 60 }, { 230, 108 }};
static const XStringRes _Const001B = { _StringsDefault0, 0x00A3 };
static const XRect _Const001C = {{ 0, 0 }, { 480, 272 }};
static const XRect _Const001D = {{ 0, 0 }, { 100, 100 }};
static const XColor _Const001E = { 0x75, 0x7C, 0x62, 0xFF };
static const XRect _Const001F = {{ 480, 20 }, { 750, 220 }};
static const XRect _Const0020 = {{ 310, 60 }, { 580, 260 }};
static const XRect _Const0021 = {{ 210, 120 }, { 470, 320 }};
static const XRect _Const0022 = {{ 260, 190 }, { 560, 390 }};
static const XRect _Const0023 = {{ 360, 260 }, { 780, 460 }};
static const XStringRes _Const0024 = { _StringsDefault0, 0x00A8 };
static const XRect _Const0025 = {{ 25, 20 }, { 85, 110 }};
static const XRect _Const0026 = {{ 103, 80 }, { 224, 98 }};
static const XStringRes _Const0027 = { _StringsDefault0, 0x00B1 };
static const XRect _Const0028 = {{ 101, 15 }, { 230, 89 }};
static const XStringRes _Const0029 = { _StringsDefault0, 0x00BB };
static const XRect _Const002A = {{ 101, 25 }, { 196, 80 }};
static const XStringRes _Const002B = { _StringsDefault0, 0x00C2 };
static const XRect _Const002C = {{ 20, 80 }, { 61, 94 }};
static const XRect _Const002D = {{ 20, 20 }, { 101, 82 }};
static const XRect _Const002E = {{ 0, 0 }, { 308, 114 }};
static const XStringRes _Const002F = { _StringsDefault0, 0x00CF };
static const XRect _Const0030 = {{ 140, 85 }, { 221, 99 }};
static const XStringRes _Const0031 = { _StringsDefault0, 0x00D8 };
static const XRect _Const0032 = {{ 67, 40 }, { 119, 92 }};
static const XRect _Const0033 = {{ 140, 25 }, { 248, 87 }};
static const XStringRes _Const0034 = { _StringsDefault0, 0x00E9 };
static const XRect _Const0035 = {{ 2, 24 }, { 42, 111 }};
static const XRect _Const0036 = {{ 266, 24 }, { 306, 111 }};
static const XRect _Const0037 = {{ 0, 0 }, { 155, 155 }};
static const XRect _Const0038 = {{ 15, 15 }, { 155, 155 }};
static const XStringRes _Const0039 = { _StringsDefault0, 0x00F1 };
static const XPoint _Const003A = { 15, 155 };
static const XPoint _Const003B = { 155, 155 };
static const XPoint _Const003C = { 155, 15 };
static const XPoint _Const003D = { 15, 15 };
static const XStringRes _Const003E = { _StringsDefault0, 0x00FA };
static const XStringRes _Const003F = { _StringsDefault0, 0x0102 };
static const XRect _Const0040 = {{ 0, 0 }, { 400, 50 }};
static const XRect _Const0041 = {{ 0, 0 }, { 133, 50 }};
static const XStringRes _Const0042 = { _StringsDefault0, 0x0108 };
static const XRect _Const0043 = {{ 133, 0 }, { 189, 50 }};
static const XRect _Const0044 = {{ 184, 0 }, { 240, 50 }};
static const XStringRes _Const0045 = { _StringsDefault0, 0x0111 };
static const XRect _Const0046 = {{ 0, 0 }, { 50, 160 }};
static const XPoint _Const0047 = { 0, 160 };
static const XPoint _Const0048 = { 50, 160 };
static const XPoint _Const0049 = { 50, 0 };
static const XRect _Const004A = {{ 0, 0 }, { 195, 270 }};
static const XRect _Const004B = {{ 0, 257 }, { 195, 260 }};
static const XRect _Const004C = {{ 0, 16 }, { 195, 19 }};
static const XRect _Const004D = {{ 12, 0 }, { 15, 270 }};
static const XRect _Const004E = {{ 180, 0 }, { 183, 270 }};
static const XRect _Const004F = {{ 87, 7 }, { 107, 227 }};
static const XRect _Const0050 = {{ 102, 121 }, { 192, 141 }};
static const XRect _Const0051 = {{ 0, 0 }, { 35, 150 }};
static const XRect _Const0052 = {{ 0, 60 }, { 35, 95 }};
static const XPoint _Const0053 = { 0, 95 };
static const XPoint _Const0054 = { 35, 95 };
static const XPoint _Const0055 = { 35, 60 };
static const XPoint _Const0056 = { 0, 60 };
static const XRect _Const0057 = {{ 0, 0 }, { 150, 35 }};
static const XRect _Const0058 = {{ 20, 0 }, { 55, 35 }};
static const XPoint _Const0059 = { 30, 50 };
static const XPoint _Const005A = { 90, 50 };
static const XPoint _Const005B = { 90, 0 };
static const XPoint _Const005C = { 30, 0 };
static const XRect _Const005D = {{ 0, 0 }, { 770, 370 }};
static const XRect _Const005E = {{ 380, 40 }, { 550, 76 }};
static const XStringRes _Const005F = { _StringsDefault0, 0x0116 };
static const XRect _Const0060 = {{ 380, 200 }, { 550, 230 }};
static const XStringRes _Const0061 = { _StringsDefault0, 0x011C };
static const XRect _Const0062 = {{ 560, 200 }, { 730, 230 }};
static const XStringRes _Const0063 = { _StringsDefault0, 0x012A };
static const XRect _Const0064 = {{ 380, 225 }, { 550, 295 }};
static const XRect _Const0065 = {{ 560, 225 }, { 730, 295 }};
static const XRect _Const0066 = {{ 463, 317 }, { 770, 362 }};
static const XRect _Const0067 = {{ 380, 10 }, { 720, 40 }};
static const XStringRes _Const0068 = { _StringsDefault0, 0x0139 };
static const XRect _Const0069 = {{ 560, 40 }, { 730, 76 }};
static const XStringRes _Const006A = { _StringsDefault0, 0x014E };
static const XRect _Const006B = {{ 380, 86 }, { 550, 126 }};
static const XStringRes _Const006C = { _StringsDefault0, 0x0154 };
static const XRect _Const006D = {{ 560, 86 }, { 730, 126 }};
static const XStringRes _Const006E = { _StringsDefault0, 0x015A };
static const XRect _Const006F = {{ 380, 136 }, { 550, 172 }};
static const XStringRes _Const0070 = { _StringsDefault0, 0x0160 };
static const XRect _Const0071 = {{ 560, 136 }, { 730, 172 }};
static const XStringRes _Const0072 = { _StringsDefault0, 0x0166 };
static const XRect _Const0073 = {{ 0, 0 }, { 170, 50 }};
static const XPoint _Const0074 = { 0, 50 };
static const XPoint _Const0075 = { 170, 50 };
static const XPoint _Const0076 = { 170, 0 };
static const XStringRes _Const0077 = { _StringsDefault0, 0x016C };
static const XRect _Const0078 = {{ 0, 0 }, { 698, 118 }};
static const XPoint _Const0079 = { 0, 118 };
static const XPoint _Const007A = { 698, 118 };
static const XPoint _Const007B = { 698, 0 };
static const XRect _Const007C = {{ 310, 90 }, { 460, 120 }};
static const XRect _Const007D = {{ 310, 20 }, { 460, 90 }};
static const XStringRes _Const007E = { _StringsDefault0, 0x0174 };
static const XRect _Const007F = {{ 37, 130 }, { 735, 251 }};
static const XRect _Const0080 = {{ 250, 29 }, { 310, 89 }};
static const XRect _Const0081 = {{ 460, 29 }, { 520, 89 }};
static const XRect _Const0082 = {{ 36, 263 }, { 106, 283 }};
static const XStringRes _Const0083 = { _StringsDefault0, 0x017C };
static const XRect _Const0084 = {{ 106, 263 }, { 176, 283 }};
static const XStringRes _Const0085 = { _StringsDefault0, 0x0183 };
static const XRect _Const0086 = {{ 176, 263 }, { 246, 283 }};
static const XStringRes _Const0087 = { _StringsDefault0, 0x018B };
static const XRect _Const0088 = {{ 246, 263 }, { 316, 283 }};
static const XStringRes _Const0089 = { _StringsDefault0, 0x0193 };
static const XRect _Const008A = {{ 316, 263 }, { 386, 283 }};
static const XStringRes _Const008B = { _StringsDefault0, 0x019B };
static const XRect _Const008C = {{ 386, 263 }, { 456, 283 }};
static const XStringRes _Const008D = { _StringsDefault0, 0x01A3 };
static const XRect _Const008E = {{ 456, 263 }, { 526, 283 }};
static const XStringRes _Const008F = { _StringsDefault0, 0x01AB };
static const XRect _Const0090 = {{ 526, 263 }, { 596, 283 }};
static const XStringRes _Const0091 = { _StringsDefault0, 0x01B3 };
static const XRect _Const0092 = {{ 596, 263 }, { 666, 283 }};
static const XStringRes _Const0093 = { _StringsDefault0, 0x01BB };
static const XRect _Const0094 = {{ 666, 263 }, { 736, 283 }};
static const XStringRes _Const0095 = { _StringsDefault0, 0x01C3 };
static const XRect _Const0096 = {{ 30, 0 }, { 170, 50 }};
static const XStringRes _Const0097 = { _StringsDefault0, 0x01CB };
static const XStringRes _Const0098 = { _StringsDefault0, 0x01D5 };
static const XStringRes _Const0099 = { _StringsDefault0, 0x01DC };
static const XStringRes _Const009A = { _StringsDefault0, 0x01E3 };
static const XStringRes _Const009B = { _StringsDefault0, 0x01EA };
static const XStringRes _Const009C = { _StringsDefault0, 0x01F1 };
static const XStringRes _Const009D = { _StringsDefault0, 0x01F8 };
static const XStringRes _Const009E = { _StringsDefault0, 0x01FF };
static const XStringRes _Const009F = { _StringsDefault1, 0x0003 };
static const XStringRes _Const00A0 = { _StringsDefault1, 0x000A };
static const XRect _Const00A1 = {{ 0, 0 }, { 344, 72 }};
static const XRect _Const00A2 = {{ 5, 0 }, { 344, 72 }};
static const XRect _Const00A3 = {{ 0, 0 }, { 5, 72 }};
static const XRect _Const00A4 = {{ 60, 10 }, { 380, 150 }};
static const XRect _Const00A5 = {{ 390, 10 }, { 710, 150 }};
static const XRect _Const00A6 = {{ 60, 160 }, { 380, 300 }};
static const XRect _Const00A7 = {{ 390, 160 }, { 710, 300 }};
static const XRect _Const00A8 = {{ 0, 0 }, { 320, 140 }};
static const XPoint _Const00A9 = { 0, 140 };
static const XPoint _Const00AA = { 320, 140 };
static const XPoint _Const00AB = { 320, 0 };
static const XRect _Const00AC = {{ 130, 80 }, { 320, 140 }};
static const XRect _Const00AD = {{ 10, 10 }, { 290, 82 }};

/* Color constant according UI design. */
const XColor CutterCaptionColor = { 0x00, 0xBC, 0xC8, 0xFF };

/* Color constant according UI design. */
const XColor CutterBackColor = { 0xFF, 0xFF, 0xFF, 0x44 };

/* Color constant according UI design. */
const XColor CutterSelectColor = { 0xB8, 0xC8, 0xC5, 0xFF };

/* Color constant according UI design. */
const XColor CutterStartColor = { 0x83, 0xC1, 0x0C, 0xFF };

/* Color constant according UI design. */
const XColor CutterStopColor = { 0xC1, 0x0C, 0x4C, 0xFF };

/* Initializer for the class 'Cutter::Window' */
void CutterWindow__Init( CutterWindow _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Background, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->CaptionRect, &_this->_XObject, 0 );
  ViewsText__Init( &_this->CaptionText, &_this->_XObject, 0 );
  EffectsRectEffect__Init( &_this->RectEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeEffect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterWindow );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0000 );
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->TouchHandler, _Const0001 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->TouchHandler, _Const0002 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->TouchHandler, _Const0003 );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->TouchHandler, _Const0004 );
  CoreView_OnSetLayout((CoreView)&_this->Background, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Background, _Const0000 );
  ViewsRectangle_OnSetColor( &_this->Background, CutterBackColor );
  CoreView_OnSetLayout((CoreView)&_this->CaptionRect, CoreLayoutAlignToLeft | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz );
  CoreRectView__OnSetBounds( &_this->CaptionRect, _Const0005 );
  ViewsRectangle_OnSetColor( &_this->CaptionRect, CutterCaptionColor );
  CoreRectView__OnSetBounds( &_this->CaptionText, _Const0006 );
  ViewsText_OnSetAlignment( &_this->CaptionText, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->CaptionText, EwLoadString( &_Const0007 ));
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->RectEffect, 1 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->RectEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->RectEffect, 300 );
  _this->Mode = CutterOperationModeProgramming;
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeEffect, 1 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeEffect, 300 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeEffect, 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Background ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->CaptionRect ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->CaptionText ), 0 );
  _this->TouchHandler.OnLeave = EwNewSlot( _this, CutterWindow_enterLeaveSlot );
  _this->TouchHandler.OnEnter = EwNewSlot( _this, CutterWindow_enterLeaveSlot );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, CutterWindow_pressReleaseSlot );
  _this->TouchHandler.OnPress = EwNewSlot( _this, CutterWindow_pressReleaseSlot );
  ViewsText_OnSetFont( &_this->CaptionText, EwLoadResource( &CutterCaptionFont, 
  ResourcesFont ));
  _this->RectEffect.Outlet = EwNewRef( _this, CoreRectView_OnGetBounds, CoreRectView__OnSetBounds );
  _this->FadeEffect.Super1.OnAnimate = EwNewSlot( _this, CutterWindow_onFade );
}

/* Re-Initializer for the class 'Cutter::Window' */
void CutterWindow__ReInit( CutterWindow _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
  ViewsRectangle__ReInit( &_this->Background );
  ViewsRectangle__ReInit( &_this->CaptionRect );
  ViewsText__ReInit( &_this->CaptionText );
  EffectsRectEffect__ReInit( &_this->RectEffect );
  EffectsInt32Effect__ReInit( &_this->FadeEffect );
}

/* Finalizer method for the class 'Cutter::Window' */
void CutterWindow__Done( CutterWindow _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterWindow );

  /* Finalize all embedded objects */
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );
  ViewsRectangle__Done( &_this->Background );
  ViewsRectangle__Done( &_this->CaptionRect );
  ViewsText__Done( &_this->CaptionText );
  EffectsRectEffect__Done( &_this->RectEffect );
  EffectsInt32Effect__Done( &_this->FadeEffect );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void CutterWindow_UpdateLayout( CutterWindow _this, XPoint aSize )
{
  XInt32 ident;
  XInt32 textW;

  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );
  CoreRectView__OnSetBounds( &_this->CaptionRect, EwNewRect( 0, 0, aSize.X, 22 ));
  ident = 11;
  textW = ( CutterScreenSize.X - ( 2 * CutterBorder.X )) / 2;

  if (( textW > aSize.X ) || ( aSize.Y < ( CutterScreenSize.Y / 2 )))
    CoreRectView__OnSetBounds( &_this->CaptionText, EwNewRect( ident, 0, aSize.X, 
    22 ));
  else
    CoreRectView__OnSetBounds( &_this->CaptionText, EwNewRect(( aSize.X - textW ) 
    - ident, 0, aSize.X, 22 ));
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
void CutterWindow_UpdateViewState( CutterWindow _this, XSet aState )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aState );

  if ( _this->TouchHandler.Down && _this->TouchHandler.Inside )
    ViewsRectangle_OnSetColor( &_this->Background, CutterSelectColor );
  else
    ViewsRectangle_OnSetColor( &_this->Background, CutterBackColor );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterWindow_enterLeaveSlot( CutterWindow _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterWindow_pressReleaseSlot( CutterWindow _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->TouchHandler.Down && _this->TouchHandler.Inside )
  {
    EwSignal( _this->OnChange, ((XObject)_this ));
  }
}

/* The function 'ChangeMode' is responsible to make the transission between the 
   different modes of the window and to start the corresponding animations. */
void CutterWindow_ChangeMode( CutterWindow _this, XEnum aMode, XRect aDestBounds )
{
  _this->Mode = aMode;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->RectEffect, 0 );
  _this->RectEffect.Value1 = _this->Super2.Bounds;
  _this->RectEffect.Value2 = aDestBounds;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->RectEffect, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeEffect, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeEffect, 1 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, (XBool)( aMode == CutterOperationModeProgramming ));
}

/* Wrapper function for the virtual method : 'Cutter::Window.ChangeMode()' */
void CutterWindow__ChangeMode( void* _this, XEnum aMode, XRect aDestBounds )
{
  ((CutterWindow)_this)->_VMT->ChangeMode((CutterWindow)_this, aMode, aDestBounds );
}

/* This is a slot method. */
void CutterWindow_onFade( CutterWindow _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CutterWindow__FadeAnimation( _this, _this->Mode, _this->FadeEffect.Value );
}

/* Method for doing a fade animation within the derived window classes. The parameter 
   aValue runs from 0 to 255 during the animation. */
void CutterWindow_FadeAnimation( CutterWindow _this, XEnum aMode, XInt32 aValue )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aValue );
  EW_UNUSED_ARG( aMode );
}

/* Wrapper function for the virtual method : 'Cutter::Window.FadeAnimation()' */
void CutterWindow__FadeAnimation( void* _this, XEnum aMode, XInt32 aValue )
{
  ((CutterWindow)_this)->_VMT->FadeAnimation((CutterWindow)_this, aMode, aValue );
}

/* Slot method to forward a signal from  an embedded Cancel button. */
void CutterWindow_onCancel( CutterWindow _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( _this->OnCancel, ((XObject)_this ));
}

/* Variants derived from the class : 'Cutter::Window' */
EW_DEFINE_CLASS_VARIANTS( CutterWindow )
EW_END_OF_CLASS_VARIANTS( CutterWindow )

/* Virtual Method Table (VMT) for the class : 'Cutter::Window' */
EW_DEFINE_CLASS( CutterWindow, CoreGroup, OnChange, OnChange, TouchHandler, TouchHandler, 
                 Mode, "Cutter::Window" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CutterWindow_UpdateLayout,
  CutterWindow_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
  CutterWindow_ChangeMode,
  CutterWindow_FadeAnimation,
EW_END_OF_CLASS( CutterWindow )

/* Constant to define the spacing between the windows. */
const XPoint CutterSpacing = { 4, 4 };

/* Constant to define the border around the screen. */
const XPoint CutterBorder = { 8, 8 };

/* This is a font resource. */
EW_DEFINE_FONT_RES( CutterCaptionFont, 15, 4, 0, 16, 0x0001, 0x00C0 )
  EW_GLYPH( 0x0001, 0, -11, 7, 11, 7, 0x00000000 ),
  EW_GLYPH( 0x0020, 0, 0, 0, 0, 4, 0x00000101 ),                /* ' ' */
  EW_GLYPH( 0x0021, 1, -11, 2, 11, 4, 0x00000101 ),             /* '!' */
  EW_GLYPH( 0x0022, 1, -12, 4, 4, 5, 0x0000013F ),              /* '"' */
  EW_GLYPH( 0x0023, 0, -11, 10, 11, 10, 0x0000016C ),           /* '#' */
  EW_GLYPH( 0x0024, 0, -13, 9, 15, 9, 0x000002E5 ),             /* '$' */
  EW_GLYPH( 0x0025, 0, -11, 11, 11, 12, 0x000004A8 ),           /* '%' */
  EW_GLYPH( 0x0026, 0, -11, 10, 11, 10, 0x00000652 ),           /* '&' */
  EW_GLYPH( 0x0027, 0, -12, 2, 4, 3, 0x000007E7 ),              /* ''' */
  EW_GLYPH( 0x0028, 1, -13, 5, 17, 5, 0x000007FE ),             /* '(' */
  EW_GLYPH( 0x0029, 0, -13, 5, 17, 6, 0x000008EB ),             /* ')' */
  EW_GLYPH( 0x002A, 0, -11, 7, 7, 7, 0x000009D0 ),              /* '*' */
  EW_GLYPH( 0x002B, 0, -10, 9, 9, 9, 0x00000A75 ),              /* '+' */
  EW_GLYPH( 0x002C, 0, -2, 3, 4, 3, 0x00000B2A ),               /* ',' */
  EW_GLYPH( 0x002D, 0, -5, 5, 1, 4, 0x00000B54 ),               /* '-' */
  EW_GLYPH( 0x002E, 1, -2, 2, 2, 4, 0x00000B6E ),               /* '.' */
  EW_GLYPH( 0x002F, 0, -11, 7, 12, 7, 0x00000B7D ),             /* '/' */
  EW_GLYPH( 0x0030, 0, -11, 9, 11, 9, 0x00000C4A ),             /* '0' */
  EW_GLYPH( 0x0031, 1, -11, 5, 11, 9, 0x00000D69 ),             /* '1' */
  EW_GLYPH( 0x0032, 0, -11, 9, 11, 9, 0x00000DD3 ),             /* '2' */
  EW_GLYPH( 0x0033, 0, -11, 8, 11, 9, 0x00000F14 ),             /* '3' */
  EW_GLYPH( 0x0034, 0, -11, 9, 11, 9, 0x00001052 ),             /* '4' */
  EW_GLYPH( 0x0035, 1, -11, 8, 11, 9, 0x0000114A ),             /* '5' */
  EW_GLYPH( 0x0036, 1, -11, 8, 11, 9, 0x0000128A ),             /* '6' */
  EW_GLYPH( 0x0037, 0, -11, 9, 11, 9, 0x000013D8 ),             /* '7' */
  EW_GLYPH( 0x0038, 0, -11, 9, 11, 9, 0x000014ED ),             /* '8' */
  EW_GLYPH( 0x0039, 0, -11, 8, 11, 9, 0x00001660 ),             /* '9' */
  EW_GLYPH( 0x003A, 1, -8, 2, 8, 4, 0x000017A3 ),               /* ':' */
  EW_GLYPH( 0x003B, 0, -8, 3, 10, 3, 0x000017D4 ),              /* ';' */
  EW_GLYPH( 0x003C, 0, -8, 7, 7, 8, 0x00001826 ),               /* '<' */
  EW_GLYPH( 0x003D, 1, -7, 7, 4, 9, 0x000018F2 ),               /* '=' */
  EW_GLYPH( 0x003E, 1, -8, 7, 7, 8, 0x0000196E ),               /* '>' */
  EW_GLYPH( 0x003F, 0, -11, 7, 11, 8, 0x00001A41 ),             /* '?' */
  EW_GLYPH( 0x0040, 0, -11, 14, 14, 14, 0x00001B3C ),           /* '@' */
  EW_GLYPH( 0x0041, 0, -11, 11, 11, 10, 0x00001E05 ),           /* 'A' */
  EW_GLYPH( 0x0042, 1, -11, 9, 11, 10, 0x00001F71 ),            /* 'B' */
  EW_GLYPH( 0x0043, 0, -11, 10, 11, 10, 0x00002094 ),           /* 'C' */
  EW_GLYPH( 0x0044, 1, -11, 9, 11, 11, 0x000021F1 ),            /* 'D' */
  EW_GLYPH( 0x0045, 1, -11, 8, 11, 9, 0x000022EB ),             /* 'E' */
  EW_GLYPH( 0x0046, 1, -11, 8, 11, 9, 0x000023CC ),             /* 'F' */
  EW_GLYPH( 0x0047, 1, -11, 9, 11, 11, 0x00002492 ),            /* 'G' */
  EW_GLYPH( 0x0048, 1, -11, 10, 11, 11, 0x000025EF ),           /* 'H' */
  EW_GLYPH( 0x0049, 1, -11, 2, 11, 4, 0x000026A7 ),             /* 'I' */
  EW_GLYPH( 0x004A, 0, -11, 8, 11, 9, 0x000026C6 ),             /* 'J' */
  EW_GLYPH( 0x004B, 1, -11, 9, 11, 10, 0x00002777 ),            /* 'K' */
  EW_GLYPH( 0x004C, 1, -11, 8, 11, 9, 0x00002893 ),             /* 'L' */
  EW_GLYPH( 0x004D, 1, -11, 12, 11, 14, 0x00002911 ),           /* 'M' */
  EW_GLYPH( 0x004E, 1, -11, 9, 11, 11, 0x00002A78 ),            /* 'N' */
  EW_GLYPH( 0x004F, 0, -11, 11, 11, 11, 0x00002B5E ),           /* 'O' */
  EW_GLYPH( 0x0050, 1, -11, 9, 11, 10, 0x00002CC8 ),            /* 'P' */
  EW_GLYPH( 0x0051, 0, -11, 10, 13, 11, 0x00002DC3 ),           /* 'Q' */
  EW_GLYPH( 0x0052, 1, -11, 9, 11, 10, 0x00002F61 ),            /* 'R' */
  EW_GLYPH( 0x0053, 0, -11, 10, 11, 10, 0x00003083 ),           /* 'S' */
  EW_GLYPH( 0x0054, 0, -11, 10, 11, 10, 0x00003215 ),           /* 'T' */
  EW_GLYPH( 0x0055, 0, -11, 10, 11, 10, 0x000032D0 ),           /* 'U' */
  EW_GLYPH( 0x0056, 0, -11, 10, 11, 10, 0x000033AA ),           /* 'V' */
  EW_GLYPH( 0x0057, 0, -11, 14, 11, 14, 0x000034F7 ),           /* 'W' */
  EW_GLYPH( 0x0058, 0, -11, 10, 11, 10, 0x000036E9 ),           /* 'X' */
  EW_GLYPH( 0x0059, 0, -11, 10, 11, 10, 0x0000384B ),           /* 'Y' */
  EW_GLYPH( 0x005A, 0, -11, 9, 11, 10, 0x0000395C ),            /* 'Z' */
  EW_GLYPH( 0x005B, 1, -13, 4, 15, 4, 0x00003A8F ),             /* '[' */
  EW_GLYPH( 0x005C, 0, -11, 7, 12, 7, 0x00003AF2 ),             /* '\' */
  EW_GLYPH( 0x005D, 0, -13, 3, 15, 4, 0x00003BC8 ),             /* ']' */
  EW_GLYPH( 0x005E, 0, -11, 7, 5, 7, 0x00003C19 ),              /* '^' */
  EW_GLYPH( 0x005F, 0, 0, 8, 1, 7, 0x00003C95 ),                /* '_' */
  EW_GLYPH( 0x0060, 0, -12, 4, 2, 5, 0x00003CBD ),              /* '`' */
  EW_GLYPH( 0x0061, 1, -8, 7, 8, 9, 0x00003CE1 ),               /* 'a' */
  EW_GLYPH( 0x0062, 1, -12, 8, 12, 9, 0x00003DD2 ),             /* 'b' */
  EW_GLYPH( 0x0063, 0, -8, 8, 8, 8, 0x00003EC2 ),               /* 'c' */
  EW_GLYPH( 0x0064, 0, -12, 8, 12, 9, 0x00003FB2 ),             /* 'd' */
  EW_GLYPH( 0x0065, 0, -8, 8, 8, 8, 0x000040A9 ),               /* 'e' */
  EW_GLYPH( 0x0066, 0, -12, 6, 12, 6, 0x000041B3 ),             /* 'f' */
  EW_GLYPH( 0x0067, 0, -8, 8, 11, 9, 0x0000425B ),              /* 'g' */
  EW_GLYPH( 0x0068, 1, -12, 7, 12, 9, 0x0000438A ),             /* 'h' */
  EW_GLYPH( 0x0069, 1, -11, 2, 11, 4, 0x00004430 ),             /* 'i' */
  EW_GLYPH( 0x006A, -1, -11, 4, 14, 4, 0x00004466 ),            /* 'j' */
  EW_GLYPH( 0x006B, 1, -12, 8, 12, 8, 0x000044D8 ),             /* 'k' */
  EW_GLYPH( 0x006C, 1, -12, 2, 12, 4, 0x000045BF ),             /* 'l' */
  EW_GLYPH( 0x006D, 1, -8, 12, 8, 14, 0x000045E2 ),             /* 'm' */
  EW_GLYPH( 0x006E, 1, -8, 7, 8, 9, 0x000046D1 ),               /* 'n' */
  EW_GLYPH( 0x006F, 0, -8, 9, 8, 9, 0x00004758 ),               /* 'o' */
  EW_GLYPH( 0x0070, 1, -8, 8, 11, 9, 0x00004857 ),              /* 'p' */
  EW_GLYPH( 0x0071, 0, -8, 8, 11, 9, 0x0000495C ),              /* 'q' */
  EW_GLYPH( 0x0072, 0, -8, 5, 8, 5, 0x00004A55 ),               /* 'r' */
  EW_GLYPH( 0x0073, 0, -8, 8, 8, 8, 0x00004AA4 ),               /* 's' */
  EW_GLYPH( 0x0074, 0, -10, 5, 10, 5, 0x00004BB9 ),             /* 't' */
  EW_GLYPH( 0x0075, 1, -8, 7, 8, 9, 0x00004C25 ),               /* 'u' */
  EW_GLYPH( 0x0076, 0, -8, 8, 8, 8, 0x00004C9E ),               /* 'v' */
  EW_GLYPH( 0x0077, 0, -8, 12, 8, 12, 0x00004D73 ),             /* 'w' */
  EW_GLYPH( 0x0078, 0, -8, 8, 8, 8, 0x00004ED4 ),               /* 'x' */
  EW_GLYPH( 0x0079, 0, -8, 8, 11, 8, 0x00004FBE ),              /* 'y' */
  EW_GLYPH( 0x007A, 0, -8, 8, 8, 8, 0x000050DE ),               /* 'z' */
  EW_GLYPH( 0x007B, 0, -12, 6, 15, 5, 0x000051C4 ),             /* '{' */
  EW_GLYPH( 0x007C, 1, -11, 2, 13, 4, 0x0000529D ),             /* '|' */
  EW_GLYPH( 0x007D, -1, -12, 6, 15, 5, 0x000052C0 ),            /* '}' */
  EW_GLYPH( 0x007E, 1, -6, 9, 3, 11, 0x0000539E ),              /* '~' */
  EW_GLYPH( 0x00A0, 0, 0, 0, 0, 4, 0x00005415 ),
  EW_GLYPH( 0x00A1, 1, -8, 2, 11, 4, 0x00005415 ),
  EW_GLYPH( 0x00A2, 1, -10, 8, 12, 9, 0x00005454 ),
  EW_GLYPH( 0x00A3, 0, -11, 9, 11, 9, 0x00005578 ),
  EW_GLYPH( 0x00A4, 0, -11, 11, 11, 11, 0x0000569B ),
  EW_GLYPH( 0x00A5, 0, -11, 10, 11, 10, 0x00005839 ),
  EW_GLYPH( 0x00A6, 1, -11, 2, 13, 4, 0x00005997 ),
  EW_GLYPH( 0x00A7, 0, -11, 9, 15, 10, 0x000059D2 ),
  EW_GLYPH( 0x00A8, 0, -12, 6, 2, 7, 0x00005BF8 ),
  EW_GLYPH( 0x00A9, 0, -11, 12, 11, 13, 0x00005C1A ),
  EW_GLYPH( 0x00AA, 1, -11, 5, 6, 7, 0x00005E28 ),
  EW_GLYPH( 0x00AB, 0, -7, 8, 6, 8, 0x00005EA5 ),
  EW_GLYPH( 0x00AC, 1, -6, 7, 3, 9, 0x00005F49 ),
  EW_GLYPH( 0x00AD, 0, -5, 5, 1, 4, 0x00005F8E ),
  EW_GLYPH( 0x00AE, 0, -11, 12, 11, 13, 0x00005FA8 ),
  EW_GLYPH( 0x00AF, 0, -11, 7, 1, 7, 0x00006184 ),
  EW_GLYPH( 0x00B0, 1, -11, 4, 4, 6, 0x000061A6 ),
  EW_GLYPH( 0x00B1, 0, -11, 8, 11, 9, 0x000061EC ),
  EW_GLYPH( 0x00B2, 0, -11, 6, 6, 6, 0x000062BE ),
  EW_GLYPH( 0x00B3, 0, -11, 6, 6, 6, 0x00006354 ),
  EW_GLYPH( 0x00B4, 1, -12, 4, 2, 5, 0x000063E6 ),
  EW_GLYPH( 0x00B5, 1, -8, 7, 11, 9, 0x0000640B ),
  EW_GLYPH( 0x00B6, 0, -11, 7, 11, 8, 0x000064B5 ),
  EW_GLYPH( 0x00B7, 1, -7, 2, 2, 4, 0x00006560 ),
  EW_GLYPH( 0x00B8, 0, 0, 4, 3, 4, 0x00006573 ),
  EW_GLYPH( 0x00B9, 1, -11, 3, 6, 6, 0x0000659D ),
  EW_GLYPH( 0x00BA, 0, -11, 7, 6, 7, 0x000065D4 ),
  EW_GLYPH( 0x00BB, 0, -7, 8, 6, 8, 0x0000666A ),
  EW_GLYPH( 0x00BC, 1, -11, 11, 11, 12, 0x0000670A ),
  EW_GLYPH( 0x00BD, 1, -11, 11, 11, 12, 0x0000686E ),
  EW_GLYPH( 0x00BE, 0, -11, 12, 11, 12, 0x000069E3 ),
  EW_GLYPH( 0x00BF, 0, -8, 8, 11, 8, 0x00006BB4 ),
  EW_GLYPH( 0x00C0, 0, -14, 11, 14, 10, 0x00006CA3 ),
  EW_GLYPH( 0x00C1, 0, -14, 11, 14, 10, 0x00006E56 ),
  EW_GLYPH( 0x00C2, 0, -14, 11, 14, 10, 0x0000700A ),
  EW_GLYPH( 0x00C3, 0, -15, 11, 15, 10, 0x000071D0 ),
  EW_GLYPH( 0x00C4, 0, -14, 11, 14, 10, 0x000073AC ),
  EW_GLYPH( 0x00C5, 0, -15, 11, 15, 10, 0x00007565 ),
  EW_GLYPH( 0x00C6, -1, -11, 16, 11, 15, 0x00007747 ),
  EW_GLYPH( 0x00C7, 0, -11, 10, 14, 10, 0x00007923 ),
  EW_GLYPH( 0x00C8, 1, -14, 8, 14, 9, 0x00007AC8 ),
  EW_GLYPH( 0x00C9, 1, -14, 8, 14, 9, 0x00007BE7 ),
  EW_GLYPH( 0x00CA, 1, -14, 8, 14, 9, 0x00007D07 ),
  EW_GLYPH( 0x00CB, 1, -14, 8, 14, 9, 0x00007E39 ),
  EW_GLYPH( 0x00CC, -1, -14, 4, 14, 4, 0x00007F5E ),
  EW_GLYPH( 0x00CD, 1, -14, 4, 14, 4, 0x00007FC5 ),
  EW_GLYPH( 0x00CE, 0, -14, 5, 14, 4, 0x0000802D ),
  EW_GLYPH( 0x00CF, -1, -14, 6, 14, 4, 0x000080B5 ),
  EW_GLYPH( 0x00D0, 0, -11, 10, 11, 11, 0x0000813E ),
  EW_GLYPH( 0x00D1, 1, -15, 9, 15, 11, 0x00008279 ),
  EW_GLYPH( 0x00D2, 0, -14, 11, 14, 11, 0x000083C7 ),
  EW_GLYPH( 0x00D3, 0, -14, 11, 14, 11, 0x00008578 ),
  EW_GLYPH( 0x00D4, 0, -14, 11, 14, 11, 0x0000872A ),
  EW_GLYPH( 0x00D5, 0, -15, 11, 15, 11, 0x000088EE ),
  EW_GLYPH( 0x00D6, 0, -14, 11, 14, 11, 0x00008AC8 ),
  EW_GLYPH( 0x00D7, 0, -9, 8, 7, 9, 0x00008C7F ),
  EW_GLYPH( 0x00D8, 0, -12, 11, 13, 11, 0x00008D5E ),
  EW_GLYPH( 0x00D9, 0, -14, 10, 14, 10, 0x00008F4E ),
  EW_GLYPH( 0x00DA, 0, -14, 10, 14, 10, 0x0000906C ),
  EW_GLYPH( 0x00DB, 0, -14, 10, 14, 10, 0x0000918B ),
  EW_GLYPH( 0x00DC, 0, -14, 10, 14, 10, 0x000092BC ),
  EW_GLYPH( 0x00DD, 0, -14, 10, 14, 10, 0x000093E0 ),
  EW_GLYPH( 0x00DE, 1, -11, 8, 11, 9, 0x00009536 ),
  EW_GLYPH( 0x00DF, 1, -12, 9, 12, 10, 0x0000960E ),
  EW_GLYPH( 0x00E0, 1, -11, 7, 11, 9, 0x00009744 ),
  EW_GLYPH( 0x00E1, 1, -11, 7, 11, 9, 0x00009870 ),
  EW_GLYPH( 0x00E2, 1, -11, 7, 11, 9, 0x0000999D ),
  EW_GLYPH( 0x00E3, 1, -12, 7, 12, 9, 0x00009ADC ),
  EW_GLYPH( 0x00E4, 1, -11, 7, 11, 9, 0x00009C2D ),
  EW_GLYPH( 0x00E5, 1, -12, 7, 12, 9, 0x00009D5F ),
  EW_GLYPH( 0x00E6, 0, -8, 14, 8, 14, 0x00009EB6 ),
  EW_GLYPH( 0x00E7, 0, -8, 8, 11, 8, 0x0000A087 ),
  EW_GLYPH( 0x00E8, 0, -11, 8, 11, 8, 0x0000A1BA ),
  EW_GLYPH( 0x00E9, 0, -11, 8, 11, 8, 0x0000A302 ),
  EW_GLYPH( 0x00EA, 0, -11, 8, 11, 8, 0x0000A44B ),
  EW_GLYPH( 0x00EB, 0, -11, 8, 11, 8, 0x0000A5A6 ),
  EW_GLYPH( 0x00EC, -1, -11, 4, 11, 4, 0x0000A6F4 ),
  EW_GLYPH( 0x00ED, 1, -11, 4, 11, 4, 0x0000A751 ),
  EW_GLYPH( 0x00EE, -1, -11, 5, 11, 4, 0x0000A7AF ),
  EW_GLYPH( 0x00EF, -1, -11, 6, 11, 4, 0x0000A82A ),
  EW_GLYPH( 0x00F0, 0, -12, 9, 12, 9, 0x0000A8A3 ),
  EW_GLYPH( 0x00F1, 1, -12, 7, 12, 9, 0x0000AA28 ),
  EW_GLYPH( 0x00F2, 0, -11, 9, 11, 9, 0x0000AB0F ),
  EW_GLYPH( 0x00F3, 0, -11, 9, 11, 9, 0x0000AC4F ),
  EW_GLYPH( 0x00F4, 0, -11, 9, 11, 9, 0x0000AD90 ),
  EW_GLYPH( 0x00F5, 0, -12, 9, 12, 9, 0x0000AEE3 ),
  EW_GLYPH( 0x00F6, 0, -11, 9, 11, 9, 0x0000B04A ),
  EW_GLYPH( 0x00F7, 0, -10, 9, 9, 9, 0x0000B190 ),
  EW_GLYPH( 0x00F8, 0, -9, 9, 10, 9, 0x0000B24A ),
  EW_GLYPH( 0x00F9, 1, -11, 7, 11, 9, 0x0000B39E ),
  EW_GLYPH( 0x00FA, 1, -11, 7, 11, 9, 0x0000B452 ),
  EW_GLYPH( 0x00FB, 1, -11, 7, 11, 9, 0x0000B507 ),
  EW_GLYPH( 0x00FC, 1, -11, 7, 11, 9, 0x0000B5CE ),
  EW_GLYPH( 0x00FD, 0, -11, 8, 14, 8, 0x0000B688 ),
  EW_GLYPH( 0x00FE, 1, -12, 8, 15, 9, 0x0000B7E7 ),
  EW_GLYPH( 0x00FF, 0, -11, 8, 14, 8, 0x0000B904 ),

  EW_FONT_PIXEL( CutterCaptionFont, 0x0000BA68 )                /* ratio 74.11 % */
    0xCF7BDCE3, 0xFEF7BBF0, 0xE498C398, 0x2D6B1334, 0x279093C8, 0x9C85AD61, 0x31873266,
    0x7F7BDDF9, 0xC009BFDE, 0x9BD3D4FF, 0x0807FC7F, 0x47BF0181, 0x78F3C437, 0xE27BAC7E,
    0x75AFDCDE, 0xDD5FBAD7, 0x07FC7917, 0xC43C8F58, 0xF3EFB9BD, 0x1BFB88DE, 0xC837B8F7,
    0xBF9C1329, 0x4E918167, 0x5404DE02, 0x7FCFF980, 0xFEFB79E3, 0x31EB7367, 0xF858C3E6,
    0x61BE5F15, 0xDE7F8CBC, 0xEDF8C06F, 0x52B17FFF, 0x2DE5F7AE, 0x1BF58464, 0x3EF0ADF2,
    0x87E7BBFF, 0xA017EE3E, 0x69FF9E03, 0x7A9CEFC0, 0xE18C47C4, 0xC625DF49, 0xE0F4E4F0,
    0xEFD54E77, 0xFA7FE783, 0x13C00BF7, 0x81FF3BF3, 0x7D3977E8, 0xFCF2FBF0, 0x9CB11255,
    0xC7E24ABF, 0xFB50FA72, 0xFBE20BFE, 0x1902FEDD, 0x8C89158F, 0xE8818F47, 0x707F1FF7,
    0xBE087462, 0x38C9D767, 0xDF1E9F23, 0xC1062B3C, 0xD592EF17, 0xEFC1A729, 0x5F8CF7B9,
    0x018571AD, 0x7306FC3E, 0x9E5EFCBD, 0x3C1FB0F1, 0x48483889, 0xF1278388, 0xC1E3507F,
    0x8D5CC5EF, 0xE7855A5F, 0x4C1F641B, 0x3D87CF13, 0x0521E982, 0x7A60A400, 0x3E7823D8,
    0xE1F6531E, 0x45C40779, 0xCFFFE880, 0x827BE67B, 0x06BA78BD, 0xF2F5AEFD, 0xF10D7E5B,
    0x025F9004, 0xDEE78000, 0x3CFBDF6D, 0xDEFB6EF7, 0xC4000007, 0x460FB80C, 0xBBF1BD54,
    0x03F7C6F7, 0x07CE0FEA, 0x27CC0F51, 0x1E503BE1, 0x7D440F38, 0xEF849F30, 0xFE078D40,
    0xF7FEB013, 0x73DF30BF, 0xCDF277DF, 0xF8C7E6A1, 0x20043FE3, 0x30080000, 0x0771B184,
    0x3EB52ADF, 0xF7DEFBE6, 0x7DFAD49D, 0x7F45627D, 0x00000173, 0xFFA00000, 0xD5F0BFF7,
    0xE4BBEFB9, 0x9FAD43D3, 0x2B100FD1, 0x5F01F588, 0x3FCDF04E, 0xD60375E8, 0xFDF309BC,
    0xDF637827, 0xFF43EF7B, 0x757C9EEF, 0xCF13F76F, 0x9C6EAA9B, 0x7ACC0807, 0x0EFBEF71,
    0x977BFBDC, 0x9C6FCD40, 0x8D3AA302, 0xFB3DF3E7, 0x7D816FDE, 0x98C01E81, 0x1FEF380B,
    0x5A8173BC, 0x13AF882F, 0xFBDEF1BC, 0x7BDF6FB9, 0x00BEE7EF, 0x7BDFF800, 0xF77C65EF,
    0x904C4BDE, 0xCE738402, 0xD9F65617, 0x25FEAFF3, 0x481118EF, 0xCD21F7F3, 0x8F4479F7,
    0x6F73BE31, 0xF3BF51FC, 0xBEFB7D62, 0xE056FF64, 0xCCF01E37, 0x64379CEF, 0x9F9FB3DD,
    0x9F7C8F98, 0xD7298BC8, 0x2CFD94C6, 0xFFC8CEF9, 0xCF5EBBBE, 0x7BDEF7BD, 0xBDEF73C7,
    0xDF0073B7, 0x80B1CC07, 0xBE4F026F, 0xD402BBC0, 0x49F701F8, 0xDE05EA30, 0xBFF58031,
    0xBEF585FF, 0xBF277DF7, 0xC03EAA55, 0xD4ABBC58, 0xBBF53C3A, 0xB5E87ABF, 0xF98B5EEF,
    0x15EFC61B, 0xF9E22201, 0xF09EF8D2, 0x3EA7BDD5, 0xE2BFB9F4, 0x97F77DAD, 0x9EB4DF31,
    0x4FD89A5F, 0x4F11B89A, 0xD5F267A7, 0xEF057B3E, 0x203E6F39, 0x773C3F9F, 0xFD39EF15,
    0x3F7C7EFA, 0xFBE1FBE0, 0xE003F7C1, 0xF551183E, 0xE61FA806, 0xFFD622FE, 0xECDFBBF7,
    0xBB30AFFA, 0xFAC42BFE, 0xC3F77EFF, 0xEFFC5FDC, 0xFEFEF7BD, 0x7F7BDEF7, 0xF7BDFF80,
    0xC07EDFDE, 0xE0AFFBF3, 0xEDDFBF3D, 0xAF9EEE64, 0xFBE7BB98, 0x76EFDFDE, 0x0AFFBF3E,
    0x6F3DDF8C, 0xFBDC7FC4, 0xFC5A33F4, 0x13D8FEAA, 0xF9AC69FA, 0x03DBC41F, 0x02B105DE,
    0x01600CF2, 0xF3DFFBE3, 0x773E807B, 0x83E7D3DF, 0x304FFE35, 0xEFC9A37E, 0xE5FF3DE3,
    0x84F7C27D, 0xA3F79C6F, 0xEA33F3CA, 0x65272627, 0x208C27AA, 0x1067084F, 0x18427183,
    0xCA6E1FF8, 0xFAD4BFC7, 0x1CF1E7CB, 0xE7739D6B, 0x55BC3DFF, 0xFDCBE77E, 0xFFF125FF,
    0x601E307A, 0x3B9EF75C, 0x0013DE00, 0x31CC04CB, 0x7CF7C03F, 0xBF18C039, 0xC6FC0FAC,
    0xF3015FF0, 0xA330B4C7, 0xE1F1BDEF, 0x77BDEF3F, 0x0FEB218F, 0x2CBE7C63, 0x7BDE5BF0,
    0x7913DDEF, 0x09DF7DEF, 0x0B01B53C, 0xF790F2B0, 0xDE455EDE, 0xC0BD777B, 0x600016F8,
    0xF7BC8B7C, 0xF445E79E, 0xF03BFEFF, 0xF1DEF71F, 0xF297E3A7, 0x81E78F1D, 0xE0BCBFAC,
    0x002001D7, 0xE5BE0BC8, 0xF49F2079, 0x1DF297C6, 0x7D8FF88F, 0x7BDED8EF, 0xE44DFFEF,
    0x02ECFBBD, 0xF80BE6B1, 0x025201F2, 0x04A40000, 0x7A207CBE, 0x3BBDE4BE, 0xBDEF7976,
    0xEF25DEF7, 0x002EF7BD, 0xEF790000, 0xDEF23FBD, 0x00007F7B, 0xF7BDE400, 0x7BDEF5DE,
    0xBC8FBDEF, 0x00FBDEF7, 0x7BC80000, 0x7BC97DEF, 0x00017DEF, 0xF7880000, 0x886FFF7F,
    0xDCDEF79D, 0xDF94BF5E, 0x74414F58, 0x01F301E6, 0xF7BDCF00, 0xBDCF0E37, 0x5802B7E5,
    0x33507FCF, 0x9EF77EFD, 0x0FF7DEFE, 0x00000DD6, 0x79000000, 0x91B7BDEF, 0x1B7BDEF7,
    0x00000000, 0x0002F780, 0x017FF000, 0x00000000, 0xCF007200, 0x689E7186, 0xC7FE4F5E,
    0xF7CFF939, 0x98067D0F, 0xBCF01BF3, 0x7BF182BF, 0x9CFFC81F, 0xF18266E0, 0xA8C16BFE,
    0x5A80BAF8, 0x079D025F, 0xFF7DF270, 0x00000000, 0x00000000, 0xF7BC8000, 0xBBDE3BDE,
    0x3E5F7580, 0x82C8C3E0, 0x03AAC91A, 0x93EE26B7, 0xFCA81D64, 0x962D46F8, 0x827E61EF,
    0x4D33C730, 0x26F9D620, 0x03E357C0, 0x77095518, 0x68CFDF83, 0x7C01CE40, 0x1B270179,
    0x98039EC8, 0xA3BC01CF, 0x8135EC81, 0x02F80959, 0x1DFF7FFA, 0x77BDC7B0, 0x97F2B1FC,
    0x37CD17F2, 0x0790F3C1, 0x1806149C, 0x84300C22, 0xDF24E0BC, 0xCAC3CF02, 0x345FCA5F,
    0x8EF7B8F6, 0xEF7BBF3F, 0xDD497FFD, 0x00DC6F7B, 0x0C0165F9, 0xEA58BF10, 0x50ED7BDE,
    0x25F3DEF7, 0x00000000, 0xFDFFDE20, 0xEE3FE237, 0xC6F363BD, 0x65CBF297, 0x8BF5103E,
    0x00FF83F8, 0x3F888000, 0x81F30FF8, 0xBE379FA8, 0xF12E5F94, 0x7D777BCF, 0xB3DFFE88,
    0xB7F406DE, 0xFDEF7BDF, 0x7BDEF237, 0x7E301F9E, 0xF8C00015, 0xBDEF213D, 0x7BC8FBFB,
    0xE7C812FF, 0x4098F405, 0xE7403F3E, 0xFEE77EA4, 0x7FEFD41E, 0x4E4DFDEE, 0x26366A4D,
    0xF8EF2F7C, 0xEE31720D, 0x4257FFF8, 0x3F6739F4, 0xF3BC64AC, 0x89C1FBC7, 0xBE3273D0,
    0x9FFBF30A, 0xBDE7EB7B, 0xEF7BDEF7, 0x57BDEF1D, 0x00077BDF, 0x00000000, 0x00000000,
    0x1F710000, 0x00006E78, 0x00000000, 0x11000000, 0xE847F87F, 0x66CCF0F9, 0xE739CEFE,
    0xE07F7D37, 0xA82C9DFF, 0xF87F953C, 0xAC16F967, 0x9E1E2787, 0xDC3AC8F8, 0xA2279C4A,
    0xCB58F0FE, 0x5FFFEB04, 0x01F557C0, 0x79818C63, 0xF7E07D03, 0x0313C295, 0x4FF33FCF,
    0xA231F8E6, 0xAFE4F154, 0x60EE18A8, 0xFF08CF3F, 0x97E794A2, 0xCC9FD4B9, 0x9D9E18F1,
    0x17BDA851, 0xEE062ABB, 0x17F94DAC, 0x55078E54, 0x607CC87F, 0xD0BCF22B, 0xC1CF6467,
    0xE3987398, 0x4671B370, 0xEF90BCAF, 0x4B00E77C, 0x068B40BC, 0x0B3DBE63, 0x2F59AE6E,
    0x78EF2F3A, 0xF907398D, 0xB093EACE, 0x10FD78CE, 0x33D11AEF, 0xB4733579, 0x9FA06F9C,
    0xAA0B1BF1, 0x1DE06CEE, 0x036900F5, 0x30000000, 0xDEF7BDEE, 0xDEF71BFB, 0x64175F7B,
    0x68EF02AF, 0xC6039EB0, 0xD9B8073B, 0xB180F3A0, 0x16A704DE, 0xBDEEB278, 0xBDFFDEF7,
    0x0000DE61, 0x00000000, 0x3FAC6F30, 0xEA20A398, 0x35015380, 0x303EF826, 0x1F3C4073,
    0xC6A02B38, 0xE607DF02, 0xFF7BFF18, 0x0000001F, 0x187FF800, 0x45EBC077, 0xFC93EF3E,
    0xB93F599E, 0xDEE2BD5E, 0x23BDEF7B, 0xB3BC9FFF, 0x97FEFFF4, 0xBCF7B9F7, 0xCF1A77B3,
    0x6F7FF78D, 0x5BDCEAF9, 0xDA80DF9E, 0xDD78D561, 0x5FFE6B73, 0xC0000000, 0x5179CEF8,
    0x63EBFCE7, 0x804FD195, 0xA6004125, 0x19E8CAB4, 0x7AFF39E6, 0x5FDCF789, 0xF77BCFF1,
    0xB0DE3597, 0xFF121BF2, 0x89288817, 0xC37A8D6F, 0xF797C7AA, 0xEF027FEE, 0x8000001F,
    0x792F9CF7, 0x29DE7FEB, 0x25186FE6, 0x2300803D, 0xA1BF980A, 0xCFFCFD16, 0x7F73E859,
    0xDDEF2FC5, 0x85FA8E5F, 0xEE72BE35, 0xDEE219BD, 0x623BEF7B, 0x18771A80, 0xFFCFD87B,
    0xBE20DFDE, 0xFDD7A3F3, 0x65C04F51, 0x8CFBECDC, 0x001F7D9B, 0xE0000000, 0xBFEBE73D,
    0x7DE7FEB7, 0x9461BF58, 0x18000F11, 0x37F300F1, 0xFF9FA28C, 0x9CFA2779, 0xA1BF422F,
    0xB7BAF3EA, 0x005FFE7B, 0x3E8C0000, 0x7FEA17E7, 0x91EB6CF6, 0x000100F5, 0xC7DF0000,
    0x007EFDF7, 0xF817BF00, 0x005EFCBD, 0x66000000, 0xFF7E773C, 0x00000005, 0xE6033AC0,
    0xFA781FFB, 0x2ABFD81B, 0xF82AFF58, 0xCC12FE8D, 0x9F17C03A, 0x00000BDF, 0x9CEFDFFC,
    0x7E73DE37, 0x5FF3DEA1, 0x7EBB9F77, 0xCF3C61BE, 0x000B7E23, 0x00000000, 0x6FFE0000,
    0xEB0BF39F, 0xAD7CF67F, 0x01015647, 0xAC000000, 0xBC2FFDFF, 0x7BE7B9DF, 0xF461ECC5,
    0x7942C78C, 0x6163C180, 0x30F66279, 0x3BF7867A, 0xFFAF7CF7, 0xA2F39DFB, 0x8FAFF3DE,
    0x27BF186F, 0xC00123C0, 0x8C37C313, 0xE7BE62DF, 0x9DF92F5F, 0xC00002F3, 0xFEF7CE7B,
    0x6F3FF57D, 0x5187A789, 0x690002D2, 0x187A7881, 0xF3FF57C5, 0x7CE7BC4E, 0x6E200009,
    0x1FF8C3EF, 0x0000004E, 0x7E73BF20, 0x5C6E75C9, 0x65745B91, 0xBC56FF74, 0xFE73DD69,
    0xBCF3F19F, 0xECC6A7FF, 0xC774C4F0, 0xAD2FFFBD, 0xAEEDB80B, 0x0002EEDB, 0x75582100,
    0xFDE2FFFA, 0x40000001, 0x5AA54F00, 0xCA73ED7A, 0xA733883C, 0xC3AD3287, 0xC44F7C59,
    0x5AC631F3, 0xFFFBE063, 0x1355983F, 0x7F582AA2, 0xAB387EFC, 0x613D4532, 0xB1F233C4,
    0x548D4B2A, 0xBBFFFFDB, 0xB27FF176, 0xFCF153D6, 0xD9915D28, 0x3F9EA063, 0x2D7055BF,
    0xEE33F89E, 0xD6393AC3, 0xD4EDF22F, 0x899F7BF9, 0xC107CF53, 0xE0EAB380, 0xA972FEF5,
    0xC77E8AF9, 0x39DC49BC, 0xB4C561E3, 0xFF17FC7E, 0x54FEB231, 0x7C2C9EFE, 0x9983EFCF,
    0x4AA881E3, 0xC660FEB0, 0xC0EF9C44, 0x7BDEF7B8, 0x4F7BDC63, 0x7C5F013E, 0x5A81CFD0,
    0xF37E44BF, 0x3C2F9381, 0xEF7BDC63, 0xFAB1B983, 0xBC899BE0, 0x28AC4800, 0xB7837EDE,
    0x028AC0DF, 0xF85E4009, 0xE3ECC4CD, 0x0000005D, 0x13D384F6, 0x03B83F39, 0x13150080,
    0xF92FBEF9, 0x13152FBE, 0x903B8001, 0x49E9C3F3, 0x8C1EEEFC, 0xFFBEE3AF, 0xCF1C3BC7,
    0xFBFABDFF, 0xFE7ABEF8, 0xFC448007, 0xB980A80A, 0x7E9E5EFD, 0xBBD7AEEF, 0xB196FE4A,
    0xB1800F31, 0x2ADF3F51, 0x7BF669D9, 0xEDCCBD77, 0xC40320D7, 0xE06FEE7B, 0x3FE6F79F,
    0x2FC3718C, 0x0FBC8B0A, 0xFBEDB580, 0xFBBE3AC7, 0x60030007, 0xFCF9E059, 0x05F7BDEF,
    0xF59FFC40, 0x6755FFBF, 0xB9F6FCC4, 0xD872FF67, 0x6FC61BF9, 0xBE07DF14, 0x13C0B229,
    0x02C88001, 0x03EF844F, 0xF97E29BF, 0x9F2FC61B, 0xB9F7B58C, 0xE639EC67, 0x989EF01D,
    0x17F318D5, 0x9FA1E7DE, 0xCF56EC87, 0xDD5CE60F, 0x1DEB7B81, 0x7BDC5DEF, 0xDC5DEFA3,
    0x8BBDF57B, 0x77BEAF7B, 0xFF800001, 0xFDFFC00B, 0x7F3000BF, 0x661EFCE7, 0xF7F6739F,
    0xAAC3D7F8, 0xF5F42623, 0xA30D7FE6, 0xFFB7C6FC, 0xAF5629FF, 0x41BA7DDF, 0x79E33BF4,
    0xFEAF8207, 0x2F7E7EFF, 0xCCFC4E7D, 0xEF78C278, 0xF10B7D78, 0x315FCC44, 0xBEBF7B6B,
    0x713DFFE7, 0x2007DF1E, 0xBFFDFFDF, 0xFBBDCF09, 0xD18B7BEE, 0xFFDFF9FB, 0xBEF4DF73,
    0xEBFBEFF1, 0x95559FEC, 0x054F6E65, 0x208588B1, 0x5560B116, 0x53DB9965, 0x7DFE37DE,
    0x33FD9D7F, 0xEFFCFDDE, 0x526FB9FF, 0xEFBBEEF7, 0x1EEFB52D, 0xF67AB7D1, 0xB73EF35D,
    0x3DFAFE39, 0x4D8DB182, 0xF2E71B9C, 0x067BE33D, 0xBE33DF00, 0xC6E71067, 0xF7BDEEB9,
    0xBDEF75DE, 0xDEEFC017, 0x7FF7C81B, 0xDD426FFF, 0xB7BEEFBB, 0x9FFEEF18, 0x9BEE7FFB,
    0xF77C37DE, 0xAB3FD9BB, 0xC054F2C0, 0xC046DF77, 0x5586DF77, 0x794F4CF0, 0x3FD900DF,
    0x92FFDDE3, 0x7526FBBF, 0xDEFBBEEF, 0x7BDEF712, 0xF5FFFD57, 0x7BC2FBDE, 0x18030BEF,
    0x1800003B, 0xFEE76EF7, 0xB9DBBDC7, 0x4400007F, 0xDC0BF901, 0xAFBDEF7B, 0x79A9673E,
    0xE2751DFF, 0x0EFE60C9, 0x58C3DFFD, 0x3BE3EE75, 0xFCF235E7, 0xEFFE8C67, 0x33FFFC19,
    0x767283EB, 0xBF3FFE67, 0x78BFF9F5, 0x000000FF, 0x8C350060, 0x67EDCF22, 0xD7F7FE7A,
    0xFE80003F, 0xADFAFBDF, 0x813028C4, 0xFC018C09, 0xCEF8809F, 0x00001049, 0x5E716FBF,
    0x2ABB956E, 0xF3FEFFD7, 0xEC6002FF, 0x77E707BB, 0x34E6477F, 0xD3990033, 0xF77E70CC,
    0xFF0FF877, 0x7BC4F781, 0xEAAFAA82, 0xABEAA003, 0xBC4F78FA, 0x00FF6427, 0xC6A898CE,
    0x007EF03E, 0xF18097EF, 0xC57B802C, 0x77F9E5FF, 0xE301BF0D, 0xB82C4F69, 0xA307F447,
    0xCE679986, 0xDEEB3465, 0x0FF65739, 0x6A898CE0, 0x07EF03EC, 0x18097EF0, 0xB7B802CF,
    0xFF3C59CF, 0xBFEF35AE, 0x9D74F183, 0x07B83278, 0x351877F3, 0x8C1EFFE8, 0xF73AAC66,
    0x0D39DF89, 0x4DF3FF18, 0xFFF39354, 0xF83F789B, 0x2FDE19EF, 0x8C227E79, 0x7389E167,
    0xFF8DEE3B, 0xABFE77CB, 0x3006FC6B, 0x01627E9E, 0xC07F44F7, 0xCCF331A8, 0xBADA30B9,
    0xEF85CE77, 0x0FBE0003, 0x93C40FE8, 0xF868FF10, 0xE9F21FC5, 0x7FA93080, 0x9DC4FC8C,
    0xF1B9E4E1, 0x401FFF21, 0xB90059DE, 0x027BC00B, 0x39809960, 0x9EF807E6, 0xE318072F,
    0xDF81F597, 0x602BFE18, 0x661698FE, 0x3E37BDF4, 0xF7BDE7FC, 0xFD6431EE, 0x97CF8C61,
    0xC80B7E05, 0x5BF007FF, 0x01FBC01F, 0x960027BC, 0x7E639809, 0x72F9EF80, 0x597E3180,
    0xE18DF81F, 0x8FE602BF, 0xDF466169, 0x7FC3E37B, 0x1EEF7BDE, 0xC61FD643, 0xE0597CF8,
    0x7FFCC0B7, 0x47B7D401, 0x7DE3B503, 0x8009EF01, 0x98E60265, 0xBE7BE01F, 0x5F8C601C,
    0x637E07D6, 0xF980AFF8, 0xD1985A63, 0xF0F8DEF7, 0xBBDEF79F, 0x87F590C7, 0x165F3E31,
    0xC0002DF8, 0x826FAF3A, 0x1313EEF7, 0x0FCE789C, 0x58009EF0, 0xF98E6026, 0xCBE7BE01,
    0x65F8C601, 0x8637E07D, 0x3F980AFF, 0x7D1985A6, 0xFF0F8DEF, 0x7BBDEF79, 0x187F590C,
    0x8165F3E3, 0xC79C42DF, 0xC40001F7, 0xE01F7C79, 0x4CB0013D, 0x03F31CC0, 0x0397CF7C,
    0xFACBF18C, 0xFF0C6FC0, 0x4C7F3015, 0xDEFA330B, 0xF3FE1F1B, 0x18F77BDE, 0xC630FEB2,
    0xBF02CBE7, 0x0BDFBE05, 0x0237E318, 0x0237E318, 0x805EFDF0, 0x32C004F7, 0x0FCC7301,
    0x0E5F3DF0, 0xEB2FC630, 0xFC31BF03, 0x31FCC057, 0x7BE8CC2D, 0xCFF87C6F, 0x63DDEF7B,
    0x18C3FAC8, 0xFC0B2F9F, 0x7BDF4016, 0x0077BDEF, 0x7BDC7339, 0xFBC01DEF, 0x3E600054,
    0xF44001EF, 0x8DF8004C, 0xF7BDEF26, 0x749A7A85, 0xBC2FBDEF, 0x003F7BBE, 0xF7BDEE3D,
    0x0D9C8027, 0x131DE00B, 0xFBDEF738, 0xFF7FFA23, 0x7B8FF81D, 0xF1D3F8EF, 0xC78EF94B,
    0x5FD640F3, 0x00EBF05E, 0x05E40010, 0x903CF2DF, 0x4BE37A4F, 0xFC478EF9, 0x6C77BEC7,
    0x1AA27BC4, 0x7700AB70, 0x81FFF215, 0xEE40B3BC, 0xBDEF7BC2, 0xEF792EF7, 0x000177BD,
    0xEF7BC800, 0xDEF791FD, 0x000003FB, 0xF7BDEF20, 0x03FFE42E, 0xF781F5BF, 0xBDEF7BC3,
    0xEF792EF7, 0x000177BD, 0xEF7BC800, 0xDEF791FD, 0x000003FB, 0xF7BDEF20, 0x0BFFE62E,
    0xA347B7D4, 0xEF2FBC76, 0xBBDEF7BD, 0xDEF7BDE4, 0x20000005, 0x47F7BDEF, 0x0FEF7BDE,
    0xBC800000, 0xC4BBDEF7, 0x001F7C79, 0xE7DF1E71, 0x7BDEF7BD, 0xDEF7BC97, 0x000000BB,
    0xFEF7BDE4, 0xFDEF7BC8, 0x90000001, 0x577BDEF7, 0x67793FFE, 0x17BC5DC9, 0x00000000,
    0xDF9FFF20, 0xEF1FBCFA, 0x00000005, 0x7FFCC000, 0x5D1EDF51, 0x7BD7DE3B, 0x00000001,
    0xCE200000, 0x3880FBE3, 0xBDE3EF8F, 0x00000000, 0x80000000, 0xBF3DEF75, 0xFF9EE702,
    0xF7F300B7, 0x1F19E026, 0x76BF1CC0, 0x3B5F80EE, 0x1CC00077, 0x103F2780, 0x7389D7F3,
    0x005BFFEF, 0x9BEBCEB0, 0x313EEF78, 0x3F39E271, 0x9FBF06EE, 0x039C80D1, 0x4E02F2F8,
    0x073D9036, 0x78039F30, 0x6BD90347, 0xF012B302, 0x03FFE405, 0x200B3BC8, 0x7FFA0177,
    0xC7B01DFF, 0xB1FC77BD, 0x17F297F2, 0xF3C137CD, 0x149C0790, 0x0C221806, 0xE0BC8430,
    0xCF02DF24, 0xCA5FCAC3, 0xB8F6345F, 0x203F8EF7, 0x6FC01FFF, 0x07EF007D, 0x77FDFFE8,
    0xDEF71EC0, 0x5FCAC7F1, 0xDF345FCA, 0x1E43CF04, 0x60185270, 0x10C03088, 0x7C9382F2,
    0x2B0F3C0B, 0xD17F297F, 0x3BDEE3D8, 0x7FFCC0FE, 0x47B7D401, 0x7DE3B503, 0x7FDFFE81,
    0xEF71EC07, 0xFCAC7F1D, 0xF345FCA5, 0xE43CF04D, 0x01852701, 0x0C030886, 0xC9382F21,
    0xB0F3C0B7, 0x17F297F2, 0xBDEE3D8D, 0xB0000FE3, 0xE09BEBCE, 0x04C4FBBD, 0x03F39E27,
    0x1DFF7FFA, 0x77BDC7B0, 0x97F2B1FC, 0x37CD17F2, 0x0790F3C1, 0x1806149C, 0x84300C22,
    0xDF24E0BC, 0xCAC3CF02, 0x345FCA5F, 0x8EF7B8F6, 0x7C79C43F, 0x9C40001F, 0xFD01F7C7,
    0xD80EFFBF, 0xFE3BDEE3, 0xF94BF958, 0xE09BE68B, 0x4E03C879, 0x110C030A, 0x5E421806,
    0x816F9270, 0x2FE561E7, 0x7B1A2FE5, 0x1FC77BDC, 0xFC5BC0DF, 0x5F2D6F87, 0x1FF57FFD,
    0xE07AA37C, 0xAF83D51B, 0x0FFABFFE, 0x16B7C3FE, 0xFFD10780, 0xF175EFFB, 0xEA9EF71F,
    0x34BF1D1F, 0x60F3C996, 0x43C8FCC6, 0x85277EFC, 0x8441D7C1, 0x9083C661, 0x6E3F7E37,
    0x17EFACF2, 0xCF9D0F3C, 0x44D17F23, 0xF8EF73CC, 0xF7FFBAA3, 0xFFF901DF, 0x01677900,
    0x7DC41772, 0x0001B9E0, 0x00000000, 0x44000000, 0xA11FE1FC, 0x9B33C3E7, 0x9CE73BF9,
    0xFFF904DF, 0x01F5BF00, 0x3EE20FDE, 0x0000DCF0, 0x00000000, 0x22000000, 0xD08FF0FE,
    0xCD99E1F3, 0xCE739DFC, 0xFFF9826F, 0x47B7D402, 0xBEF1DA83, 0xB9E07DC4, 0x00000001,
    0x00000000, 0xE1FC4400, 0xC3E7A11F, 0x3BF99B33, 0x44DF9CE7, 0x01F7C79C, 0xDF1E7100,
    0x9E07DC47, 0x0000001B, 0x00000000, 0x1FC44000, 0x3E7A11FE, 0xBF99B33C, 0x4DF9CE73,
    0xF00FFF90, 0xFDE01F5B, 0x3AC24FA0, 0xBC43F5E3, 0xE4CF446B, 0x72D1CCD5, 0xC67E81BE,
    0xBAA82C6F, 0xD47781B3, 0x000DA403, 0x5DC00000, 0xF7BE8000, 0x7BDF497C, 0x62F880ED,
    0xB17C4001, 0x0ED7BDF4, 0x12F9EF7D, 0xBFF98000, 0xFB78C1FB, 0x37E4BF7E, 0xB116331E,
    0x4FD01618, 0xC40BA882, 0x77338038, 0x203EDC80, 0xEC4782BE, 0xC6E73881, 0xE41FFF21,
    0x0BB9059D, 0xE5FFBFFD, 0xEF3DEE7D, 0x73C69DEC, 0x5BDFFDE3, 0x96F73ABE, 0x76A037E7,
    0xF75E3558, 0xFFC89ADC, 0xC1F5BF07, 0xF7FFA1FB, 0xBDCFBCBF, 0xD3BD9DE7, 0xFFBC6E78,
    0xE757CB7B, 0x06FCF2DE, 0xC6AB0ED4, 0x935B9EEB, 0xFA82FFF9, 0xC76A68F6, 0xFBFFD2FB,
    0xDEE7DE5F, 0x69DECEF3, 0xFFDE373C, 0x73ABE5BD, 0x037E796F, 0xE355876A, 0x09ADCF75,
    0x9BEBCEB0, 0x4C4FBBDE, 0xA3F39E27, 0xBCBFF7FF, 0x9DE7BDCF, 0x6E78D3BD, 0xCB7BFFBC,
    0xF2DEE757, 0x0ED406FC, 0x9EEBC6AB, 0xE3CE335B, 0x3CE200FB, 0xDFFE8FBE, 0xF73EF2FF,
    0x4EF6779E, 0xFEF1B9E3, 0x9D5F2DEF, 0x1BF3CB7B, 0x1AAC3B50, 0x4D6E7BAF, 0x318BDFBE,
    0xE318237E, 0xEFDF0237, 0x7FEFFF45, 0xCF7B9F79, 0xF1A77B3B, 0xF7FF78DC, 0xBDCEAF96,
    0xA80DF9E5, 0xD78D561D, 0xF8A6B73D, 0xE33FFDCE, 0x7C1BCF77, 0xFDFBEFB1, 0xC7E6F7FF,
    0x9ACAA5F7, 0xBF1856E2, 0xF73E6F73, 0x63749FDE, 0xDEEB9BDF, 0xBC88EF7B, 0xFE220289,
    0x187647A2, 0x37B9FFCD, 0x7BDC7FF7, 0xFB9EF12F, 0xEF79FE2B, 0x1BC6B2FE, 0xE2437E56,
    0x251102FF, 0x6F51ADF1, 0xF2F8F558, 0xF14FFDDE, 0x6E01C89E, 0x90ABB815, 0x9DE40FFF,
    0xF4177205, 0x97E2BFB9, 0x472FEEF7, 0x5F1AC2FD, 0x0CDEF739, 0xF7BDEF71, 0x8D40311D,
    0xEC3D8C3B, 0x6FEF7FE7, 0xF03FFE40, 0x3F781F5B, 0xC57F73E8, 0x5FDDEF2F, 0x3585FA8E,
    0xBDEE72BE, 0x7BDEE219, 0x80623BEF, 0x7B18771A, 0xDEFFCFD8, 0x7FFCC0DF, 0x68F6FA81,
    0x45F78ED4, 0x7E2BFB9F, 0x72FEEF79, 0xF1AC2FD4, 0xCDEF7395, 0x7BDEF710, 0xD40311DF,
    0xC3D8C3B8, 0xFEF7FE7E, 0xEF8F3886, 0xE3CE2003, 0xFEE7D0FB, 0xBBDE5F8A, 0x0BF51CBF,
    0xDCE57C6B, 0xBDC4337B, 0xC477DEF7, 0x30EE3500, 0xFF9FB0F6, 0xFF91BFBD, 0x7259DE4F,
    0x0017BF17, 0xFFF20000, 0xFBCFADF9, 0x00017BF1, 0xFFF30000, 0x747B7D45, 0x7E5F78ED,
    0x0000002F, 0x7C79C400, 0xF1E7101F, 0x005EFC7D, 0x00000000, 0x0469BF60, 0x777F67DE,
    0x12AAB9E0, 0x055BFFFA, 0x8B8CF1BF, 0x3C7FDFFE, 0x5EEE73F4, 0x886FE611, 0x160F1197,
    0x0C8C1E23, 0x6DEA57F3, 0xFF6F7BFA, 0xBCEB001E, 0xFBBDE9BE, 0x39E274C4, 0xCFB7FF3F,
    0x3FF585F9, 0x23D6BE7B, 0x000080AB, 0xFFF20000, 0x01677901, 0xFFAC0BB9, 0xDFBC2FFD,
    0xC57BE7B9, 0x8CF461EC, 0x807942C7, 0x796163C1, 0x7A30F662, 0xF73BF786, 0xFFC82F7C,
    0x07D6FC07, 0xFF581FBC, 0xBF785FFB, 0x8AF7CF73, 0x19E8C3D9, 0x00F2858F, 0xF2C2C783,
    0xF461ECC4, 0xEE77EF0C, 0xFF985EF9, 0x3DBEA02F, 0xFBC76A1A, 0xFFDFFAC2, 0x7B9DFBC2,
    0x1ECC57BE, 0x2C78CF46, 0x3C180794, 0x66279616, 0x7867A30F, 0xF7CF73BF, 0xAF3AC002,
    0xFBBDE26F, 0xE789C4C4, 0xEFFD60FC, 0xCEFDE17F, 0x662BDF3D, 0x3C67A30F, 0x0C03CA16,
    0x13CB0B1E, 0x33D187B3, 0xE7B9DFBC, 0xF8F3897B, 0xF388003E, 0xFF583EF8, 0xBF785FFB,
    0x8AF7CF73, 0x19E8C3D9, 0x00F2858F, 0xF2C2C783, 0xF461ECC4, 0xEE77EF0C, 0x33A85EF9,
    0x06750000, 0xEF7BAC00, 0x758EF7BD, 0xDEF7BDEF, 0x06750001, 0x626B0000, 0x5BFFEFFD,
    0x8D6E77F4, 0x3E47B316, 0xCB1999BF, 0x990B2DEF, 0xAFAA320F, 0xB3D98AC7, 0x8F533BC6,
    0x197BE7B8, 0x85FFBFF5, 0x77907FFC, 0xFE2EE416, 0x001FDE2F, 0xF0040000, 0xD7A5AA54,
    0xFF20A73E, 0x07D6FC1F, 0xC5FFC7EF, 0x000003FB, 0x4A9E0080, 0xE7DAF4B5, 0x0BFFE614,
    0xA9A3DBEA, 0xFFEBEF1D, 0x0001FDE2, 0x4F004000, 0xED7A5AA5, 0xC79C4A73, 0x79C401F7,
    0x8BFF9F7C, 0x000007F7, 0x953C0100, 0xCFB5E96A, 0x07FFC829, 0xEF03EB7E, 0xB8937987,
    0x8AC3C673, 0x2FF8FD69, 0xFD6463FE, 0x593DFCA9, 0x07DF9EF8, 0x5103C733, 0xC1FD6095,
    0xDF38898C, 0x0000BB81, 0x77E70000, 0xF3EA8BCE, 0xA86F97AD, 0x09150177, 0xFA446016,
    0xD5277A86, 0xF9CD5BE7, 0x0002F39D, 0xFBE3CE20, 0xF8F38800, 0x7126F33E, 0x15878CE7,
    0x5FF1FAD3, 0xFAC8C7FC, 0xB27BF953, 0x0FBF3DF0, 0xA2078E66, 0x83FAC12A, 0xBE711319,
    0x00000003

  EW_FONT_KERNING_CODES( CutterCaptionFont )
  EW_FONT_KERNING_VALUES( CutterCaptionFont )
EW_END_OF_FONT_RES( CutterCaptionFont )

/* Table with links to derived variants of the font resource : 'Cutter::CaptionFont' */
EW_RES_WITHOUT_VARIANTS( CutterCaptionFont )

/* This is a font resource. */
EW_DEFINE_FONT_RES( CutterSmallDigitFont, 25, 6, 0, 16, 0x0001, 0x000E )
  EW_GLYPH( 0x0001, 1, -18, 10, 18, 12, 0x00000000 ),
  EW_GLYPH( 0x002C, 0, -3, 5, 8, 6, 0x000001F6 ),               /* ',' */
  EW_GLYPH( 0x002E, 1, -3, 5, 3, 7, 0x00000273 ),               /* '.' */
  EW_GLYPH( 0x0030, 1, -18, 13, 18, 15, 0x000002B0 ),           /* '0' */
  EW_GLYPH( 0x0031, 2, -18, 8, 18, 15, 0x00000504 ),            /* '1' */
  EW_GLYPH( 0x0032, 1, -18, 13, 18, 15, 0x00000600 ),           /* '2' */
  EW_GLYPH( 0x0033, 1, -18, 13, 18, 15, 0x00000892 ),           /* '3' */
  EW_GLYPH( 0x0034, 0, -18, 15, 18, 15, 0x00000B58 ),           /* '4' */
  EW_GLYPH( 0x0035, 1, -18, 13, 18, 15, 0x00000D61 ),           /* '5' */
  EW_GLYPH( 0x0036, 1, -18, 13, 18, 15, 0x00000FE3 ),           /* '6' */
  EW_GLYPH( 0x0037, 0, -18, 14, 18, 15, 0x0000126D ),           /* '7' */
  EW_GLYPH( 0x0038, 1, -18, 13, 18, 15, 0x00001479 ),           /* '8' */
  EW_GLYPH( 0x0039, 1, -18, 13, 18, 15, 0x00001736 ),           /* '9' */
  EW_GLYPH( 0x003A, 1, -14, 5, 14, 7, 0x000019D9 ),             /* ':' */

  EW_FONT_PIXEL( CutterSmallDigitFont, 0x00001A8B )             /* ratio 65.31 % */
    0xEF7BDEEB, 0xEE7977BD, 0xE2DEF7BD, 0xD4FF303E, 0x1760FB86, 0xC39E1FCF, 0xDACFF02D,
    0x099DFCC0, 0x10000484, 0x677F3012, 0x836B2702, 0xD82DCFE7, 0x6D47F3C5, 0x81F70FB8,
    0xF7B9E7F9, 0x718B7BDE, 0x2C200BEF, 0xB158BF8E, 0xB54F18CC, 0x7D88FDFC, 0x593C353F,
    0x7E7893C3, 0x03DF9EE7, 0xD442AFBE, 0x803D61F7, 0xFCF1F267, 0x19F8BFEF, 0xBF94BF29,
    0x8161E390, 0x20FE09E2, 0x11700B89, 0x02000000, 0x5C172E00, 0x78920904, 0x233C5028,
    0x2FE52FC5, 0xFEA3F0E4, 0x98FC5FF7, 0xDE273013, 0xF7D442AF, 0x61FBC602, 0xFB233DEE,
    0x057703BC, 0xD9899E20, 0xADF8DBF7, 0x00000000, 0x00000000, 0x00000000, 0xE7B9DFA8,
    0xABFBC137, 0xF0FEF510, 0x72BD404E, 0x62EEE7D2, 0x8C2B98F9, 0x1FC1C88F, 0xEFBF7122,
    0xEB321881, 0xF0DE0279, 0x2E4B3C01, 0x05C4FD40, 0x4FF117D4, 0xFF917C60, 0xE47F8C01,
    0xC7F8C01D, 0x89E409D8, 0x379007FA, 0xDEF7B9D5, 0x024FFF7B, 0xF9E00000, 0x1EFFFB9D,
    0xE42AFBF0, 0x15F0FDF3, 0xE878DAA0, 0xD30FFF7B, 0x8FC8B98F, 0x07E73388, 0x3BEF9888,
    0x704C7E30, 0x69E5DDEF, 0x4003EA00, 0x7BDC13FD, 0xC00F23F7, 0xEF18D2F8, 0x326097EF,
    0x9301EAB3, 0xF24EF3F8, 0x21CCF14B, 0x0FFF7BDE, 0xE007FC33, 0x85776269, 0x001F7E7C,
    0x007FBDDF, 0x2F0001EA, 0x8004E800, 0x3A001531, 0x966005C6, 0x9B4400BB, 0x445F8003,
    0x0E8F5003, 0x01355E00, 0x60068CE8, 0xBDEFB14C, 0x5637BC9B, 0x00058000, 0xBDEF7B8C,
    0xDEF26EF7, 0x00000000, 0xEF7BDDF8, 0xC1DEF7BD, 0x80048002, 0xF7BDEE30, 0x13243BDE,
    0x26009040, 0x7FFB9DF1, 0x427CC913, 0x00383DCE, 0xBD56F871, 0xA7269EE7, 0x6219BFD1,
    0xCD80362D, 0xC8024634, 0xCCD82FBD, 0xD60DF94C, 0x7D9E6B64, 0x9E9C876F, 0xDAC5E406,
    0x7FE8C533, 0x73BFD902, 0xABFF7803, 0x0027B002, 0xCEFE60DF, 0xF623D40D, 0x3D0E015B,
    0xFBD44A00, 0x464BE7BF, 0x78A67B18, 0x80649976, 0x7BED40F8, 0xBF18989A, 0x6364D621,
    0x00333F81, 0xD80D42A0, 0xAC19C54C, 0xB90BF5C5, 0x23C66DEF, 0x3F8C01FE, 0x8C435DC6,
    0xBDEE21DE, 0xEF7BDEF7, 0x0007F7BD, 0xF71D8000, 0xBDEF7BDE, 0xA0058877, 0x8B400986,
    0x2B0D4017, 0x007E5C00, 0x40127946, 0xA51802F3, 0x02E68006, 0x80146518, 0x4A3005E6,
    0x7E5E003E, 0x1311F200, 0x3002F340, 0xDF9EE77F, 0x42B7BF04, 0x5BC3FBD4, 0x3C597A80,
    0x25879EE7, 0x47C42F82, 0x3C440004, 0x1E3E217C, 0xEE73C37E, 0x711F8C79, 0x183FE412,
    0xE4EE4137, 0x99EE79A4, 0xBD527AF2, 0x4F92DE21, 0xB126C1B2, 0xEA831FC1, 0xD461EB10,
    0x9A7B9D60, 0x8C00CE51, 0x42B7EE43, 0xE613BBD4, 0x077FDCEF, 0x7215BDF8, 0x80AF81DD,
    0xB0F187F8, 0x5A29DDCE, 0x871874F1, 0x3185CFE5, 0x58B000C4, 0xF88B05C9, 0x7E30ACF3,
    0xCFA0CF03, 0x037085DD, 0x3B8D3244, 0x443BCE53, 0xFEFFEF12, 0x003C27DF, 0xB18072F1,
    0xEE07AA6E, 0x03F21DDD, 0x310076A0, 0x6205FED6, 0x4F0D4FDF, 0xEC64F0D6, 0x000001FB,
    0xFBEC4000, 0x1AC9E1A9, 0x0000049E

  EW_FONT_KERNING_CODES( CutterSmallDigitFont )
  EW_FONT_KERNING_VALUES( CutterSmallDigitFont )
EW_END_OF_FONT_RES( CutterSmallDigitFont )

/* Table with links to derived variants of the font resource : 'Cutter::SmallDigitFont' */
EW_RES_WITHOUT_VARIANTS( CutterSmallDigitFont )

/* This is a font resource. */
EW_DEFINE_FONT_RES( CutterDescrFont, 11, 3, 0, 16, 0x0001, 0x00C0 )
  EW_GLYPH( 0x0001, 0, -9, 5, 9, 5, 0x00000000 ),
  EW_GLYPH( 0x0020, 0, 0, 0, 0, 3, 0x000000A3 ),                /* ' ' */
  EW_GLYPH( 0x0021, 0, -9, 3, 9, 3, 0x000000A3 ),               /* '!' */
  EW_GLYPH( 0x0022, 0, -9, 4, 3, 4, 0x000000F1 ),               /* '"' */
  EW_GLYPH( 0x0023, 0, -9, 7, 9, 7, 0x00000121 ),               /* '#' */
  EW_GLYPH( 0x0024, 0, -11, 7, 13, 7, 0x00000222 ),             /* '$' */
  EW_GLYPH( 0x0025, 0, -9, 9, 9, 9, 0x00000379 ),               /* '%' */
  EW_GLYPH( 0x0026, 0, -9, 8, 9, 8, 0x00000498 ),               /* '&' */
  EW_GLYPH( 0x0027, 0, -9, 2, 3, 2, 0x000005AA ),               /* ''' */
  EW_GLYPH( 0x0028, 0, -9, 4, 12, 4, 0x000005C2 ),              /* '(' */
  EW_GLYPH( 0x0029, 0, -9, 4, 12, 4, 0x00000666 ),              /* ')' */
  EW_GLYPH( 0x002A, 0, -9, 6, 5, 6, 0x0000070C ),               /* '*' */
  EW_GLYPH( 0x002B, 0, -8, 7, 7, 6, 0x00000787 ),               /* '+' */
  EW_GLYPH( 0x002C, 0, -1, 3, 4, 3, 0x00000807 ),               /* ',' */
  EW_GLYPH( 0x002D, 0, -4, 5, 1, 5, 0x0000083B ),               /* '-' */
  EW_GLYPH( 0x002E, 0, -1, 3, 1, 4, 0x00000854 ),               /* '.' */
  EW_GLYPH( 0x002F, -1, -9, 6, 10, 4, 0x00000863 ),             /* '/' */
  EW_GLYPH( 0x0030, 0, -9, 7, 9, 7, 0x000008FC ),               /* '0' */
  EW_GLYPH( 0x0031, 1, -9, 4, 9, 7, 0x000009BB ),               /* '1' */
  EW_GLYPH( 0x0032, 0, -9, 7, 9, 7, 0x00000A0C ),               /* '2' */
  EW_GLYPH( 0x0033, 0, -9, 7, 9, 7, 0x00000AFF ),               /* '3' */
  EW_GLYPH( 0x0034, 0, -9, 7, 9, 7, 0x00000BFA ),               /* '4' */
  EW_GLYPH( 0x0035, 0, -9, 7, 9, 7, 0x00000C9E ),               /* '5' */
  EW_GLYPH( 0x0036, 0, -9, 7, 9, 7, 0x00000D8F ),               /* '6' */
  EW_GLYPH( 0x0037, 0, -9, 7, 9, 7, 0x00000E7C ),               /* '7' */
  EW_GLYPH( 0x0038, 0, -9, 7, 9, 7, 0x00000F46 ),               /* '8' */
  EW_GLYPH( 0x0039, 0, -9, 7, 9, 7, 0x0000103E ),               /* '9' */
  EW_GLYPH( 0x003A, 0, -6, 3, 6, 4, 0x0000111E ),               /* ':' */
  EW_GLYPH( 0x003B, 0, -6, 3, 9, 3, 0x00001154 ),               /* ';' */
  EW_GLYPH( 0x003C, 0, -6, 6, 6, 6, 0x000011AF ),               /* '<' */
  EW_GLYPH( 0x003D, 0, -6, 7, 4, 7, 0x00001239 ),               /* '=' */
  EW_GLYPH( 0x003E, 0, -6, 6, 6, 6, 0x000012A6 ),               /* '>' */
  EW_GLYPH( 0x003F, 0, -9, 6, 9, 6, 0x00001339 ),               /* '?' */
  EW_GLYPH( 0x0040, 0, -8, 11, 11, 11, 0x000013FC ),            /* '@' */
  EW_GLYPH( 0x0041, 0, -9, 9, 9, 8, 0x000015DC ),               /* 'A' */
  EW_GLYPH( 0x0042, 0, -9, 8, 9, 8, 0x000016CE ),               /* 'B' */
  EW_GLYPH( 0x0043, 0, -9, 8, 9, 8, 0x000017B4 ),               /* 'C' */
  EW_GLYPH( 0x0044, 0, -9, 8, 9, 8, 0x000018C2 ),               /* 'D' */
  EW_GLYPH( 0x0045, 0, -9, 7, 9, 7, 0x00001983 ),               /* 'E' */
  EW_GLYPH( 0x0046, 0, -9, 7, 9, 7, 0x00001A23 ),               /* 'F' */
  EW_GLYPH( 0x0047, 0, -9, 8, 9, 8, 0x00001AB2 ),               /* 'G' */
  EW_GLYPH( 0x0048, 0, -9, 8, 9, 8, 0x00001BC8 ),               /* 'H' */
  EW_GLYPH( 0x0049, 0, -9, 3, 9, 4, 0x00001C4B ),               /* 'I' */
  EW_GLYPH( 0x004A, 0, -9, 7, 9, 7, 0x00001C74 ),               /* 'J' */
  EW_GLYPH( 0x004B, 0, -9, 8, 9, 8, 0x00001D01 ),               /* 'K' */
  EW_GLYPH( 0x004C, 0, -9, 7, 9, 7, 0x00001DCB ),               /* 'L' */
  EW_GLYPH( 0x004D, 0, -9, 10, 9, 11, 0x00001E2C ),             /* 'M' */
  EW_GLYPH( 0x004E, 0, -9, 8, 9, 8, 0x00001F2F ),               /* 'N' */
  EW_GLYPH( 0x004F, 0, -9, 8, 9, 8, 0x00001FD8 ),               /* 'O' */
  EW_GLYPH( 0x0050, 0, -9, 8, 9, 8, 0x000020D8 ),               /* 'P' */
  EW_GLYPH( 0x0051, 0, -9, 8, 11, 8, 0x00002192 ),              /* 'Q' */
  EW_GLYPH( 0x0052, 0, -9, 8, 9, 8, 0x000022CC ),               /* 'R' */
  EW_GLYPH( 0x0053, 0, -9, 8, 9, 8, 0x000023AA ),               /* 'S' */
  EW_GLYPH( 0x0054, 0, -9, 8, 9, 8, 0x000024E7 ),               /* 'T' */
  EW_GLYPH( 0x0055, 0, -9, 8, 9, 8, 0x00002570 ),               /* 'U' */
  EW_GLYPH( 0x0056, 0, -9, 8, 9, 8, 0x0000260E ),               /* 'V' */
  EW_GLYPH( 0x0057, -1, -9, 11, 9, 10, 0x000026FF ),            /* 'W' */
  EW_GLYPH( 0x0058, 0, -9, 8, 9, 8, 0x0000285A ),               /* 'X' */
  EW_GLYPH( 0x0059, 0, -9, 8, 9, 8, 0x00002965 ),               /* 'Y' */
  EW_GLYPH( 0x005A, 0, -9, 7, 9, 7, 0x00002A38 ),               /* 'Z' */
  EW_GLYPH( 0x005B, 0, -10, 4, 12, 3, 0x00002B0F ),             /* '[' */
  EW_GLYPH( 0x005C, -1, -9, 7, 10, 5, 0x00002B63 ),             /* '\' */
  EW_GLYPH( 0x005D, 0, -10, 3, 12, 3, 0x00002C1B ),             /* ']' */
  EW_GLYPH( 0x005E, 0, -9, 5, 4, 5, 0x00002C57 ),               /* '^' */
  EW_GLYPH( 0x005F, 0, 0, 6, 1, 5, 0x00002CAE ),                /* '_' */
  EW_GLYPH( 0x0060, 0, -9, 4, 2, 4, 0x00002CCC ),               /* '`' */
  EW_GLYPH( 0x0061, 0, -6, 7, 6, 6, 0x00002CEF ),               /* 'a' */
  EW_GLYPH( 0x0062, 0, -9, 7, 9, 7, 0x00002D92 ),               /* 'b' */
  EW_GLYPH( 0x0063, 0, -6, 6, 6, 6, 0x00002E2B ),               /* 'c' */
  EW_GLYPH( 0x0064, 0, -9, 7, 9, 7, 0x00002EC4 ),               /* 'd' */
  EW_GLYPH( 0x0065, 0, -6, 7, 6, 7, 0x00002F65 ),               /* 'e' */
  EW_GLYPH( 0x0066, -1, -9, 6, 9, 4, 0x0000301E ),              /* 'f' */
  EW_GLYPH( 0x0067, 0, -6, 7, 8, 7, 0x0000309B ),               /* 'g' */
  EW_GLYPH( 0x0068, 0, -9, 7, 9, 7, 0x00003159 ),               /* 'h' */
  EW_GLYPH( 0x0069, 0, -8, 3, 8, 3, 0x000031DC ),               /* 'i' */
  EW_GLYPH( 0x006A, -1, -8, 4, 10, 3, 0x0000321A ),             /* 'j' */
  EW_GLYPH( 0x006B, 0, -9, 7, 9, 7, 0x00003285 ),               /* 'k' */
  EW_GLYPH( 0x006C, 0, -9, 3, 9, 3, 0x00003330 ),               /* 'l' */
  EW_GLYPH( 0x006D, 0, -6, 10, 6, 10, 0x00003359 ),             /* 'm' */
  EW_GLYPH( 0x006E, 0, -6, 7, 6, 7, 0x000033F0 ),               /* 'n' */
  EW_GLYPH( 0x006F, 0, -6, 7, 6, 7, 0x00003458 ),               /* 'o' */
  EW_GLYPH( 0x0070, 0, -6, 7, 8, 7, 0x000034EB ),               /* 'p' */
  EW_GLYPH( 0x0071, 0, -6, 7, 8, 7, 0x0000358D ),               /* 'q' */
  EW_GLYPH( 0x0072, 0, -6, 5, 6, 5, 0x0000362D ),               /* 'r' */
  EW_GLYPH( 0x0073, 0, -6, 6, 6, 6, 0x00003675 ),               /* 's' */
  EW_GLYPH( 0x0074, 0, -8, 4, 8, 4, 0x0000372A ),               /* 't' */
  EW_GLYPH( 0x0075, 0, -6, 7, 6, 7, 0x00003787 ),               /* 'u' */
  EW_GLYPH( 0x0076, -1, -6, 8, 6, 6, 0x000037E8 ),              /* 'v' */
  EW_GLYPH( 0x0077, 0, -6, 9, 6, 9, 0x00003886 ),               /* 'w' */
  EW_GLYPH( 0x0078, 0, -6, 7, 6, 6, 0x00003959 ),               /* 'x' */
  EW_GLYPH( 0x0079, -1, -6, 8, 8, 6, 0x000039F4 ),              /* 'y' */
  EW_GLYPH( 0x007A, 0, -6, 6, 6, 6, 0x00003AC0 ),               /* 'z' */
  EW_GLYPH( 0x007B, 0, -9, 4, 11, 4, 0x00003B57 ),              /* '{' */
  EW_GLYPH( 0x007C, 1, -9, 2, 11, 3, 0x00003BE0 ),              /* '|' */
  EW_GLYPH( 0x007D, 0, -9, 4, 11, 4, 0x00003BFD ),              /* '}' */
  EW_GLYPH( 0x007E, 0, -5, 8, 3, 8, 0x00003C87 ),               /* '~' */
  EW_GLYPH( 0x00A0, 0, 0, 0, 0, 3, 0x00003CF5 ),
  EW_GLYPH( 0x00A1, 1, -6, 3, 8, 4, 0x00003CF5 ),
  EW_GLYPH( 0x00A2, 0, -8, 7, 10, 7, 0x00003D3E ),
  EW_GLYPH( 0x00A3, 0, -9, 7, 9, 7, 0x00003E29 ),
  EW_GLYPH( 0x00A4, 0, -8, 8, 8, 8, 0x00003EFF ),
  EW_GLYPH( 0x00A5, -1, -9, 9, 9, 7, 0x00003FF2 ),
  EW_GLYPH( 0x00A6, 0, -9, 3, 11, 3, 0x000040E8 ),
  EW_GLYPH( 0x00A7, 0, -9, 8, 11, 8, 0x00004130 ),
  EW_GLYPH( 0x00A8, 0, -8, 6, 1, 6, 0x000042B1 ),
  EW_GLYPH( 0x00A9, 0, -9, 9, 9, 9, 0x000042CC ),
  EW_GLYPH( 0x00AA, 0, -9, 5, 5, 5, 0x00004430 ),
  EW_GLYPH( 0x00AB, 0, -5, 6, 4, 6, 0x00004493 ),
  EW_GLYPH( 0x00AC, 1, -5, 5, 3, 7, 0x000044F3 ),
  EW_GLYPH( 0x00AD, 0, -4, 5, 1, 5, 0x00004528 ),
  EW_GLYPH( 0x00AE, 0, -9, 9, 9, 9, 0x00004541 ),
  EW_GLYPH( 0x00AF, 0, -9, 6, 1, 6, 0x0000469C ),
  EW_GLYPH( 0x00B0, 0, -9, 4, 3, 5, 0x000046B9 ),
  EW_GLYPH( 0x00B1, 0, -8, 6, 8, 6, 0x000046F2 ),
  EW_GLYPH( 0x00B2, 0, -9, 5, 5, 5, 0x00004781 ),
  EW_GLYPH( 0x00B3, 0, -9, 5, 5, 5, 0x000047FC ),
  EW_GLYPH( 0x00B4, 0, -9, 4, 2, 4, 0x00004872 ),
  EW_GLYPH( 0x00B5, 0, -6, 7, 8, 8, 0x00004897 ),
  EW_GLYPH( 0x00B6, 0, -9, 6, 9, 6, 0x00004911 ),
  EW_GLYPH( 0x00B7, 0, -5, 3, 1, 4, 0x0000498F ),
  EW_GLYPH( 0x00B8, 0, 0, 3, 3, 3, 0x000049A1 ),
  EW_GLYPH( 0x00B9, 1, -9, 3, 5, 5, 0x000049CB ),
  EW_GLYPH( 0x00BA, 0, -9, 5, 5, 6, 0x000049FA ),
  EW_GLYPH( 0x00BB, 0, -5, 6, 4, 6, 0x00004A6D ),
  EW_GLYPH( 0x00BC, 0, -9, 9, 9, 9, 0x00004ACB ),
  EW_GLYPH( 0x00BD, 0, -9, 10, 9, 9, 0x00004BDA ),
  EW_GLYPH( 0x00BE, 0, -9, 10, 9, 10, 0x00004D08 ),
  EW_GLYPH( 0x00BF, 0, -6, 6, 8, 6, 0x00004E66 ),
  EW_GLYPH( 0x00C0, 0, -11, 9, 12, 8, 0x00004F16 ),
  EW_GLYPH( 0x00C1, 0, -11, 9, 12, 8, 0x0000504A ),
  EW_GLYPH( 0x00C2, 0, -11, 9, 12, 8, 0x00005180 ),
  EW_GLYPH( 0x00C3, 0, -11, 9, 12, 8, 0x000052C6 ),
  EW_GLYPH( 0x00C4, 0, -11, 9, 11, 8, 0x0000540E ),
  EW_GLYPH( 0x00C5, 0, -11, 9, 13, 8, 0x0000553C ),
  EW_GLYPH( 0x00C6, 0, -9, 11, 9, 11, 0x0000568A ),
  EW_GLYPH( 0x00C7, 0, -9, 8, 12, 8, 0x000057AC ),
  EW_GLYPH( 0x00C8, 0, -11, 7, 12, 7, 0x00005900 ),
  EW_GLYPH( 0x00C9, 0, -11, 7, 12, 7, 0x000059DC ),
  EW_GLYPH( 0x00CA, 0, -11, 7, 12, 7, 0x00005ABA ),
  EW_GLYPH( 0x00CB, 0, -11, 7, 11, 7, 0x00005BA8 ),
  EW_GLYPH( 0x00CC, -1, -11, 4, 12, 4, 0x00005C80 ),
  EW_GLYPH( 0x00CD, 0, -11, 4, 12, 4, 0x00005CE5 ),
  EW_GLYPH( 0x00CE, -1, -11, 6, 12, 4, 0x00005D4C ),
  EW_GLYPH( 0x00CF, -1, -11, 6, 11, 4, 0x00005DDB ),
  EW_GLYPH( 0x00D0, -1, -9, 9, 9, 8, 0x00005E55 ),
  EW_GLYPH( 0x00D1, 0, -11, 8, 12, 8, 0x00005F33 ),
  EW_GLYPH( 0x00D2, 0, -11, 8, 12, 8, 0x0000602F ),
  EW_GLYPH( 0x00D3, 0, -11, 8, 12, 8, 0x0000616E ),
  EW_GLYPH( 0x00D4, 0, -11, 8, 12, 8, 0x000062AF ),
  EW_GLYPH( 0x00D5, 0, -11, 8, 12, 8, 0x00006400 ),
  EW_GLYPH( 0x00D6, 0, -11, 8, 11, 8, 0x00006553 ),
  EW_GLYPH( 0x00D7, 0, -7, 6, 6, 6, 0x0000668D ),
  EW_GLYPH( 0x00D8, 0, -10, 8, 11, 8, 0x00006724 ),
  EW_GLYPH( 0x00D9, 0, -11, 8, 12, 8, 0x0000686C ),
  EW_GLYPH( 0x00DA, 0, -11, 8, 12, 8, 0x00006949 ),
  EW_GLYPH( 0x00DB, 0, -11, 8, 12, 8, 0x00006A28 ),
  EW_GLYPH( 0x00DC, 0, -11, 8, 11, 8, 0x00006B17 ),
  EW_GLYPH( 0x00DD, 0, -11, 8, 12, 8, 0x00006BEF ),
  EW_GLYPH( 0x00DE, 0, -9, 7, 9, 7, 0x00006D03 ),
  EW_GLYPH( 0x00DF, 0, -9, 8, 9, 8, 0x00006DA2 ),
  EW_GLYPH( 0x00E0, 0, -9, 7, 9, 6, 0x00006E6E ),
  EW_GLYPH( 0x00E1, 0, -9, 7, 9, 6, 0x00006F4D ),
  EW_GLYPH( 0x00E2, 0, -9, 7, 9, 6, 0x0000702E ),
  EW_GLYPH( 0x00E3, 0, -9, 7, 9, 6, 0x0000711F ),
  EW_GLYPH( 0x00E4, 0, -8, 7, 8, 6, 0x00007212 ),
  EW_GLYPH( 0x00E5, 0, -10, 7, 10, 6, 0x000072ED ),
  EW_GLYPH( 0x00E6, 0, -6, 10, 6, 10, 0x000073E4 ),
  EW_GLYPH( 0x00E7, 0, -6, 6, 9, 6, 0x00007508 ),
  EW_GLYPH( 0x00E8, 0, -9, 7, 9, 7, 0x000075DB ),
  EW_GLYPH( 0x00E9, 0, -9, 7, 9, 7, 0x000076D0 ),
  EW_GLYPH( 0x00EA, 0, -9, 7, 9, 7, 0x000077C7 ),
  EW_GLYPH( 0x00EB, 0, -8, 7, 8, 7, 0x000078CE ),
  EW_GLYPH( 0x00EC, -1, -9, 4, 9, 3, 0x000079BF ),
  EW_GLYPH( 0x00ED, 0, -9, 4, 9, 3, 0x00007A18 ),
  EW_GLYPH( 0x00EE, -1, -9, 6, 9, 3, 0x00007A73 ),
  EW_GLYPH( 0x00EF, -1, -8, 6, 8, 3, 0x00007AF0 ),
  EW_GLYPH( 0x00F0, 0, -9, 7, 9, 7, 0x00007B58 ),
  EW_GLYPH( 0x00F1, 0, -9, 7, 9, 7, 0x00007C4D ),
  EW_GLYPH( 0x00F2, 0, -9, 7, 9, 7, 0x00007D05 ),
  EW_GLYPH( 0x00F3, 0, -9, 7, 9, 7, 0x00007DD4 ),
  EW_GLYPH( 0x00F4, 0, -9, 7, 9, 7, 0x00007EA5 ),
  EW_GLYPH( 0x00F5, 0, -9, 7, 9, 7, 0x00007F86 ),
  EW_GLYPH( 0x00F6, 0, -8, 7, 8, 7, 0x00008069 ),
  EW_GLYPH( 0x00F7, 0, -7, 7, 5, 7, 0x00008134 ),
  EW_GLYPH( 0x00F8, 0, -7, 7, 8, 7, 0x000081B2 ),
  EW_GLYPH( 0x00F9, 0, -9, 7, 9, 7, 0x0000827F ),
  EW_GLYPH( 0x00FA, 0, -9, 7, 9, 7, 0x0000831C ),
  EW_GLYPH( 0x00FB, 0, -9, 7, 9, 7, 0x000083BB ),
  EW_GLYPH( 0x00FC, 0, -8, 7, 8, 7, 0x0000846A ),
  EW_GLYPH( 0x00FD, -1, -9, 8, 11, 6, 0x00008503 ),
  EW_GLYPH( 0x00FE, 0, -9, 7, 11, 7, 0x00008610 ),
  EW_GLYPH( 0x00FF, -1, -8, 8, 10, 6, 0x000086CC ),

  EW_FONT_PIXEL( CutterDescrFont, 0x000087D2 )                  /* ratio 79.39 % */
    0xD6FDEF75, 0xC61B67B9, 0x13FC0F90, 0x0C1F24FF, 0xCF73AC63, 0x384DF75E, 0xF71C8098,
    0xEF7EBBA9, 0x291C75EF, 0xC61F5EE8, 0xDCF7FFFF, 0x3DFE9EFA, 0x27FFD77F, 0xCEFFA532,
    0x6BFE3BBF, 0xF21DCFBD, 0x3CCCC49C, 0x60778120, 0x7F933AEC, 0x888EF59E, 0xC89F3B11,
    0x9D3FF391, 0xFF6313DC, 0xCFFCDB7E, 0xC6B319E7, 0xE2FE6527, 0x3FCF2AD6, 0xE209C36F,
    0xD6B03F77, 0x407C6A7D, 0xF75AC3DC, 0x7E22CFF9, 0xF01F9FB7, 0x61FDDF7E, 0xC3E679CA,
    0xF2FC05FF, 0xF23E6799, 0x77817F77, 0x8531A79E, 0x17BFCA81, 0xE572FE47, 0x9EDFA5D3,
    0xE2B959DB, 0x9E1DFE39, 0xA79E6B58, 0x613FBD6A, 0x6DF1AF50, 0x7F513CFD, 0x1048478F,
    0xF511E3D2, 0xF8C79FA7, 0xBFB0FBF6, 0x1ECD3CF2, 0x0E91E3C3, 0xF1E3C748, 0xB6A9E31F,
    0x58B7C2BF, 0xE7ADE71D, 0xDC9FFD56, 0x75EC231D, 0x77807207, 0x2DDF8007, 0x96EFC6E7,
    0xBBC00373, 0xE21F7183, 0x88CCE789, 0xE793EF7B, 0x16E073C5, 0xC288C6D4, 0x90AA246F,
    0x818A87E3, 0xA173C17F, 0x7C43DDF6, 0xD8ABFAEB, 0x95C49E2A, 0xAC824000, 0xFC5476D8,
    0xC8FF5DD7, 0x7FFBEDF9, 0x0001FFE2, 0xDFF9E000, 0xBADF197F, 0x66B9F8BE, 0x23C7DF64,
    0x9E0CC6F1, 0xC5F8CD99, 0x1DE27109, 0x2EF7D4CC, 0xE5FFB9EF, 0x1ECEF7C9, 0x78116FCF,
    0xF783BD6F, 0x17E1378E, 0xA2037EB5, 0xB358BF3E, 0x607FAEEB, 0x05F21FDD, 0x22F1AC07,
    0x62F855DF, 0xBDF6F56D, 0x8000BD5B, 0x86F7BDFF, 0x846F7BAC, 0x7FCEB484, 0xF13BF7CA,
    0x53F2CC66, 0x6DD00D63, 0xBADF1AC7, 0xEFC60BEB, 0xEF3B3C3C, 0xC1BC6FC3, 0xA5A7FE13,
    0xC3D67FF5, 0x7813FC76, 0xF9FAE7E2, 0xF1BEBBAF, 0xBBDEF7BB, 0xE1DEF77E, 0xF89518C1,
    0x60F1581C, 0x87CF82C6, 0xE7C0C631, 0xFFBFF583, 0xECEE3B25, 0xEF911021, 0xBC66F5B8,
    0x57378EE3, 0x00A2FCFC, 0xA8BF3F15, 0xA5F8DCFC, 0xF297F7DA, 0x4F5D775B, 0x10164F6C,
    0x3F464B65, 0xF3AC25FF, 0x3F1E8517, 0x47BF19C6, 0x0BCF379E, 0xE79BCF20, 0x1802F3CD,
    0x789E21F7, 0x2F640CCE, 0xDF2FFBF5, 0x80BDAE6F, 0x5ED737EF, 0xC65FF7EA, 0x18DEF7BD,
    0x037BDEF7, 0xF7BDEE30, 0x980EF3C6, 0xFE78F7FB, 0xC10FBEBD, 0x7DF9EFF3, 0x7C3DFEE6,
    0x7E7EB4E7, 0x2B239AEF, 0xFE3FDFDD, 0xD2F99A82, 0xE06A304E, 0x81F3887E, 0x0DF3DFFE,
    0xCF7BCEC4, 0xFCDF17FE, 0xBF1FF3BE, 0xE7AF5657, 0x4633FFCE, 0x989EFC8D, 0x3CA770D7,
    0x338821A4, 0xA6718D52, 0x7D7EBD9E, 0xFCF31CAC, 0xDE21ECFA, 0xAB0190F3, 0x4127B9CF,
    0x0AC03B9C, 0x0F17F03F, 0x03958D44, 0x3E1F5D4B, 0xF0E4597B, 0xD63CA23B, 0x67E3AFBD,
    0xF7B8CA53, 0xDE40DF9E, 0x8E41FCFD, 0x06239018, 0x22F1FF79, 0xA01ECDEF, 0x7916504C,
    0xDE297C6F, 0x6F837E73, 0x39FEBBED, 0xF17E32AA, 0x133AC194, 0x86220088, 0x46B33AC1,
    0xF1E2FC65, 0x7FAEFB5F, 0x3DDEF73E, 0x5DEFEEA1, 0x3C0BC560, 0x04008051, 0xC560A278,
    0x77BFBA8B, 0xF7BDEE79, 0xBBDEEA2E, 0xDEEA0000, 0x02F7BA8B, 0xEF7BA800, 0xF7BDEE7A,
    0xF7BDD43E, 0x7BA80001, 0x0BDEEA2F, 0xEF880000, 0x5FE0DF9C, 0xF1FEBBFF, 0x8BF11A8C,
    0xDD67F7F5, 0xAEF7B819, 0x0EFE6DC0, 0xFE1AC678, 0xCEFFD7F8, 0x31FDCF5E, 0x00000FEE,
    0x4377BA80, 0x0001BBDD, 0x0FF71800, 0xFF000000, 0x0000003B, 0xF1011000, 0xE463159E,
    0x39F7BCF7, 0xBFF3FDC6, 0x31A39D0D, 0x3AB81C56, 0xA900C391, 0x85C8F621, 0xA80DED11,
    0x0FEE7E8A, 0x00000000, 0xF7500000, 0xBBDCF1DE, 0x9B1FEE70, 0xFF0A880A, 0x03D4B280,
    0x0DC8F723, 0xC145FA9E, 0x027AC5F8, 0x007FA462, 0x7EF7858F, 0x0CF177BC, 0x0AA30340,
    0xF460D17C, 0x00666058, 0xE206780D, 0xF60BE73B, 0x336D7BBA, 0xE727C89F, 0x076FE7F6,
    0xF6C40110, 0x7CCF6FE7, 0xF69C9F32, 0xE36D7BDA, 0x857F7BDE, 0x097E67BA, 0xF1E05151,
    0x06FAF750, 0x007EEF75, 0xEF880000, 0xEBD82F9C, 0x7CCDB5EE, 0xDB9C9F22, 0x401DBF9F,
    0x9FDB1004, 0xC9F33DBF, 0x6BDA727C, 0xEF88B3FF, 0xF1059A9C, 0xBDEE35DB, 0x7BA83F77,
    0x69884BE2, 0xA878CC42, 0xEA0EC67B, 0x7CF80466, 0xB202B1E0, 0xDCEFCA62, 0xFF3EE0DF,
    0x1911FF33, 0x5195BE23, 0x877AC5F1, 0x06FBAF5F, 0xB6B7FFD6, 0x93CF3FBA, 0xA693E54F,
    0xF1BED6E9, 0xF7BDDF8D, 0xEEFCFBDE, 0x003EF756, 0x00000000, 0xFDCF0000, 0x000FBBF1,
    0x40000000, 0x23395100, 0xEF3B9666, 0x93BED79D, 0x66CFF779, 0xF28A3DFB, 0x188E4664,
    0xE294F1E9, 0x18FDBFE7, 0x540E4F59, 0x0F9FC144, 0xF70E72E7, 0x91118C78, 0x0F9863C3,
    0xFDFC31EB, 0x5B63FF2F, 0x863919F2, 0x678D6329, 0x4F244F3D, 0xC1E23BDA, 0xFC0C3D60,
    0xBDB08E45, 0xFADCE3F7, 0xAA8AE545, 0x34FBF19C, 0x067CCC6F, 0xC588B8B1, 0x2ACAC741,
    0x69FDE2B9, 0x8C662B1D, 0xFB2777E7, 0xC637F5D6, 0x5E351AA8, 0xC6B7AF2F, 0xF89E33C3,
    0x1E1313E0, 0xEF000003, 0xF3DEF7BD, 0x17877BDE, 0xF685C8CF, 0x8931CAC3, 0x8AA3A357,
    0xD85F1A5C, 0xDEE7DEF7, 0x0000F987, 0xF3000000, 0x8F01BFF1, 0x183F3F09, 0x7CFC2463,
    0x472078F0, 0xB07CF80E, 0xFA8A882C, 0x0001DFDF, 0xF3070000, 0xA9F8BE4E, 0xCFBACFF1,
    0xF7BDEE78, 0xA7E751DE, 0x9DFA8D7A, 0x3BF54BFB, 0x9DF15EBB, 0x0FA7EA6D, 0xEBA0F186,
    0xF73C31BB, 0xE7800005, 0x5FEB1A7F, 0x00DB48AB, 0x7F81B690, 0xFCE7D2AD, 0x57FEAD62,
    0x3FBE356F, 0x9E55B3D4, 0xEBBD5ADF, 0x003BBF07, 0x93FE7D00, 0xF83FD567, 0xED8016BB,
    0xFFAAD45A, 0xFFDFFCC2, 0xCEE7DEA3, 0x56EB6E5F, 0xBDEF7844, 0x7E33EFE3, 0x1FCF73EA,
    0x3F06FFCF, 0xE24C6437, 0xEAD8AEAF, 0xB0000002, 0xE7DDF9CE, 0xDFC13AE9, 0xD76C0035,
    0x13FEBAA0, 0xD962FF71, 0x9F2FAEF7, 0xC00002FB, 0xE62F3FF3, 0x218399FF, 0xB000004D,
    0xB3EEB3EE, 0x780002EE, 0xE5DCF2EE, 0x00002FB9, 0x3EF7E748, 0x0005DD73, 0x8339C600,
    0x3307FC73, 0x0CC440E7, 0x770F8ECC, 0x7DCF9A8E, 0xDE000001, 0xFACFFEFB, 0x1D9C1DDD,
    0x6498F5FD, 0x0000E910, 0xFDCF0000, 0xF887EFFD, 0xD21833D7, 0xD6000004, 0xF4F2EEFF,
    0x6DB17D73, 0xB6D80515, 0xB9FA7A8A, 0xFEEE78BE, 0xD7FAC69F, 0x0036D22A, 0xFEB06DA4,
    0x3FF3C5D5, 0xFF9F400B, 0xEAB53EEF, 0x004B618F, 0xD75496C3, 0xFF9D617F, 0xBDDC6003,
    0x32FFE25F, 0xBE200003, 0x7DE37E73, 0xEF1C7DBE, 0x55FCC62F, 0xEF1CE6FD, 0x7CFB3EEB,
    0x03FB8F4F, 0xFAB7FD5B, 0x67258403, 0xDEBEE7EE, 0x800000FB, 0xD66A0C30, 0xF237FE1F,
    0x795479FE, 0x8FDBD9F8, 0xE0635591, 0xC3C1F8CB, 0x73C77BC3, 0xCF6F6EAB, 0x9CBFD3B6,
    0xC8AAE7CA, 0x846B3B27, 0x8B327CCA, 0xFAFE51ED, 0x3A7F71AE, 0xC5F9ABDF, 0x89208398,
    0xC74AAB37, 0x7FE68DEF, 0x479FEF33, 0xFF9F8795, 0x4F18C7DF, 0xFE17E255, 0x620988F0,
    0x1A7FF3C1, 0xBF7BDEEB, 0xCD11FF75, 0x1ACD331B, 0x6F2E3A2D, 0xDE3FBEC4, 0x061967A5,
    0xB2FC7CC1, 0x0F99B2FD, 0x59E86182, 0x60000037, 0xCB4F3C7D, 0x749CC01C, 0xCCFE74FE,
    0x67873209, 0xF2BCEB36, 0xCEFEF197, 0x53C727AA, 0xFF6DDFEB, 0x8375BFED, 0x01CC04BC,
    0x3C0BE01E, 0xBF91FB7F, 0x62A7E379, 0xA5903FBB, 0xE46EFB62, 0x0BF9DE6F, 0xE07EDFCF,
    0xFCE7580B, 0x7AED7552, 0x8DDCFF0D, 0xADA78C6B, 0xBCDA78FB, 0x83A4090F, 0x7FBDF5A7,
    0xCF7BF980, 0x3D737F79, 0xAB3C6F71, 0x2B3A23B2, 0x3C72781F, 0x1D93E4F0, 0x715559D1,
    0xB7D9546F, 0xE3B4DDD4, 0xD46AAA68, 0xB89C7428, 0x07B4B678, 0x077D6DCF, 0x077D6DCF,
    0x077D6DCF, 0x003EE300, 0xEE33EE30, 0xEFC60003, 0x3FF82F9C, 0x227CFFF7, 0x629A8C46,
    0x9FDF8F64, 0xCB3FF995, 0x764EF8A8, 0xCEFF958E, 0x0CDE7D4A, 0x2FADFFC6, 0x1158C62B,
    0xB9FFC323, 0xE4EA39F7, 0xDFFAC938, 0x7B9EF13D, 0x7E865ECF, 0x7F3B7F6F, 0x1CDF77BF,
    0xE94664FD, 0x4A332791, 0xDFBF3C8F, 0xFA39BBFB, 0xFEDCFA19, 0xBDCF7A76, 0xEFB12F67,
    0x239DF627, 0x7728777E, 0xFE3079A8, 0x7CE33FCF, 0xCE204BFD, 0xF7BCBFD7, 0x77BDFFDE,
    0x7DEF7103, 0x27BBFF58, 0xD9EF73DE, 0xEFFDF0CB, 0xFDFCEDFB, 0x93F46BCC, 0xE467DE69,
    0x8D6F34C9, 0xC61FBF3C, 0x7EF867EA, 0xF7A7677E, 0x12F67BDC, 0x7C3BDEF7, 0xEF388D67,
    0xFEF2F388, 0xCB759321, 0xEE5BAFFD, 0x0EF8001F, 0xFEE73DCF, 0x4F3F77E2, 0x5F519FFC,
    0x7F9BE18F, 0x13CE33CF, 0xF78CFEFB, 0x393AC8D4, 0xC4F93CC6, 0xEEF954EF, 0xB89EB193,
    0x00FF717F, 0x09118000, 0xF67185EE, 0xFFD403FD, 0x625F91DE, 0x10150005, 0xFDF2167B,
    0xFFF88000, 0x7DC756FA, 0x73EF1DC6, 0xE800FF96, 0xF39F33B3, 0x72A9CDFB, 0x9CF9954E,
    0x9FFC6FDF, 0xFBCF1A7F, 0xDE78C01C, 0x00BF50E7, 0x23F116F1, 0xA81B7E36, 0xFB78C0B9,
    0x57F712FF, 0xFEA301FF, 0xFDEF82CF, 0xB783398F, 0xA8673BDE, 0x96F1005F, 0x7E6C41F8,
    0x02E6B01B, 0xFDDF8DF4, 0x13C6B988, 0xF51867FF, 0xC397EFAA, 0x367BE2F7, 0xE33CF16F,
    0x0FEFB13C, 0x21A9EF18, 0x1C9D647E, 0x4F319B7E, 0xF117353E, 0xFEDEB53B, 0xDFDF72BF,
    0x8C03FEAF, 0x7C0B3FFA, 0x19CC7FEF, 0x73BDEB78, 0xFA067D06, 0xA837BC0C, 0x1CF8C235,
    0x9AE6F17E, 0x9CFFEA87, 0x3AAF57EF, 0x0D7AA03F, 0xC401EE60, 0xF0AC03B9, 0x40F17F03,
    0xB03958D4, 0xB3E1F5D4, 0xBF0E4597, 0xDD63CA23, 0x367E3AFB, 0x1F77C0A5, 0x901EB190,
    0xEE7106BF, 0xC0FC2B00, 0x35103C5F, 0x752C0E56, 0x65ECF87D, 0x88EFC391, 0xBEF758F2,
    0x294D9F8E, 0x6414FFD4, 0xB913B33F, 0x9C4277BB, 0x3F0AC03B, 0x440F17F0, 0x4B03958D,
    0x7B3E1F5D, 0x3BF0E459, 0xBDD63CA2, 0x5367E3AF, 0x2717FB0A, 0x815DDE50, 0x40FFF9DA,
    0x0AC03B9C, 0x0F17F03F, 0x03958D44, 0x3E1F5D4B, 0xF0E4597B, 0xD63CA23B, 0x67E3AFBD,
    0x393A8A53, 0x8E4EA24E, 0x0EE71093, 0xFC0FC2B0, 0x635103C5, 0xD752C0E5, 0x165ECF87,
    0x288EFC39, 0xEBEF758F, 0x8294D9F8, 0xF6300BEF, 0x2DF6302D, 0xC402FBE0, 0xF0AC03B9,
    0x40F17F03, 0xB03958D4, 0xB3E1F5D4, 0xBF0E4597, 0xDD63CA23, 0x367E3AFB, 0xF7BEC0A5,
    0x8350BBDE, 0x1B42EF7D, 0x1EA31816, 0xDDF35A01, 0xFA7B28CB, 0x6E75E5EE, 0x0233E44B,
    0xBEA47ED0, 0x73DE2FF7, 0xED6F837E, 0xAA39FEBB, 0x94F17E32, 0x88133AC1, 0xC1862200,
    0x6546B33A, 0x5FF1E2FC, 0x8A7FAEFB, 0xD01DC677, 0x2E33C171, 0x7AA0FCEA, 0xF07B980D,
    0x5DEF7BDC, 0x0177BDD4, 0x17BDD400, 0x0005EF75, 0x85DEF750, 0x58C83EEF, 0x3CD7F20F,
    0x177BDEF7, 0x005DEF75, 0x45EF7500, 0x00017BDD, 0x5177BDD4, 0x3F6453FF, 0xEEEE53B3,
    0xF7BDCF9D, 0x7BDD45DE, 0xDD400017, 0x5EF7517B, 0xEF750000, 0x38E4EA5D, 0x938E4EA9,
    0xDEF7BDCF, 0x177BDD45, 0x7BDD4000, 0x005EF751, 0x5DEF7500, 0xD7AA7E75, 0xEE30F730,
    0x0000001F, 0x8C9F77C0, 0x8DAFE4F5, 0x000007FB, 0xA7FEA000, 0x33B33F64, 0xE34EF777,
    0x000001FE, 0xA8000000, 0x9D64E393, 0xFB8D271C, 0x00000007, 0xDC400000, 0xE40BFF7B,
    0xA7817EED, 0xE2A90266, 0x1C441BC8, 0x2A400379, 0xC81CD4F0, 0xFF62FDDB, 0xDDE504E2,
    0xFFCED415, 0xDE3F7BC7, 0xA00678BB, 0xBE055181, 0x2C7A3068, 0x06803330, 0xFCEA033C,
    0x601AF540, 0xCEF881EE, 0xEEBD82F9, 0x27CCDB5E, 0xFDB9C9F2, 0x4401DBF9, 0xF9FDB100,
    0xCC9F33DB, 0xF6BDA727, 0xF77C1B5E, 0x40F58C81, 0x9DF11AFE, 0xDD7B05F3, 0x4F99B6BD,
    0xFB7393E4, 0x8803B7F3, 0xF3FB6200, 0x993E67B7, 0xED7B4E4F, 0x7FEA36BD, 0xECCFD90A,
    0x4EF77724, 0x0BE73BE2, 0x6D7BBAF6, 0x27C89F33, 0x6FE7F6E7, 0xC4011007, 0xCF6FE7F6,
    0x9C9F327C, 0x6D7BDAF6, 0xA09C5FEC, 0xDA82BBBC, 0xDF10FFF9, 0xD7B05F39, 0xF99B6BDD,
    0xB7393E44, 0x803B7F3F, 0x3FB62008, 0x93E67B7F, 0xD7B4E4F9, 0x27536BDE, 0xE4EA49C7,
    0xE77C4938, 0x775EC17C, 0x13E66DAF, 0xFEDCE4F9, 0x2200EDFC, 0xFCFED880, 0xE64F99ED,
    0x7B5ED393, 0xA17F2DAF, 0x5EBE7CE7, 0xF3D6BABB, 0x9EB5D501, 0xAF5F3E6F, 0xCFA08C0D,
    0xEB7E1DF9, 0x518C6BBF, 0xBF119E35, 0xE0F6BE92, 0x39C08F59, 0x4D667949, 0x4E7C6A35,
    0x8F78E4FC, 0x9CFB3A7F, 0x0FCEA0DF, 0xE601AF54, 0x8FEE781E, 0x00007DDF, 0x02000000,
    0x3119CA88, 0xEF79DCB3, 0x0FBBE0BC, 0xF207AC64, 0xC7F73CD7, 0x00003EEF, 0x01000000,
    0x988CE544, 0x77BCEE59, 0x14FFD45E, 0x49D99FB2, 0x9E9DEEEE, 0x1F77E3FB, 0x00000000,
    0x72A20080, 0x772CCC46, 0x752F3BDE, 0x4EA49C72, 0xFB9E938E, 0x001F77E3, 0x80000000,
    0x4672A200, 0xDE772CCC, 0xF77C2F3B, 0x40F58C81, 0x3BBF1AFE, 0xBFAEB7D9, 0xA8D54631,
    0xBD797AF1, 0xF19E1E35, 0x989F07C4, 0x000018F0, 0x80017758, 0xDF45E7BE, 0x052C2F9E,
    0xE7B7D14B, 0x02F3DF4B, 0x8D39DFC8, 0x1EB5DCF3, 0x60904631, 0x5E018099, 0x9CCD6037,
    0xEFA04708, 0x3F3AA73B, 0xE6035EA8, 0xDCEFD41E, 0xD9DFAA5F, 0x6CEF8AF5, 0x307D3F53,
    0xDF5D078C, 0x7DDF018D, 0xE41EB190, 0xB9DFA9AF, 0xB3BF54BF, 0xD9DF15EB, 0x60FA7EA6,
    0xBEBA0F18, 0x9FFA831B, 0x9D99FB22, 0x54EF7772, 0xA97F73BF, 0x2BD7677E, 0xFD4DB3BE,
    0x1E30C1F4, 0x06377D74, 0x942717FB, 0x9DA85777, 0x9DFA8FFF, 0x3BF54BFB, 0x9DF15EBB,
    0x0FA7EA6D, 0xEBA0F186, 0xC9D431BB, 0x1C9D5271, 0xDCEFD527, 0xD9DFAA5F, 0x6CEF8AF5,
    0x307D3F53, 0xDF5D078C, 0x0BEF818D, 0x630B7D8C, 0x0BEF82DF, 0x2FEE77EA, 0x7AECEFD5,
    0xA9B677C5, 0xC6183E9F, 0xC6EFAE83, 0xFEFFEF90, 0x997F73BF, 0xF3C6FFEF, 0xEF8DEBFD,
    0x37F7E1DC, 0xF2DE67DF, 0xB5CFF7BF, 0xD6F18CFC, 0xEEB59E7B, 0x5F9CFA9D, 0xEAFFD5AC,
    0x87F7C6AD, 0xF3CAB67A, 0xFD77AB5B, 0x741B8CF4, 0x52E33C5C, 0x6BD507E7, 0xF303DCC0,
    0x7A8FFF7F, 0xB97F3B9F, 0xE1115BAD, 0xBF8EF7BD, 0xCFA9F8CF, 0xEEF87F3D, 0x20F58C83,
    0xBFF98D7F, 0xCFBD47FF, 0xD6DCBF9D, 0xDEF088AD, 0x67DFC77B, 0x9EE7D4FC, 0x453FF53F,
    0xE53B33F6, 0xFCC9DEEE, 0xDEA3FFDF, 0x6E5FCEE7, 0x784456EB, 0xEFE3BDEF, 0x73EA7E33,
    0x1C9D5FCF, 0x71C9D527, 0xFFBFF992, 0x9DCFBD47, 0xADD6DCBF, 0x7BDEF088, 0xFC67DFC7,
    0xBF9EE7D4, 0x6BD53F3A, 0xF7187B98, 0xBE000007, 0x27AC64FB, 0x1FDC6D7F, 0xFF500000,
    0xD99FB253, 0xA77BBB99, 0x00007F71, 0x72750000, 0xE393AC9C, 0x007F71A4, 0xC6000000,
    0x5A88B3BF, 0x2FCF24FF, 0x1BF7AC7E, 0xCE7E8CF9, 0x812D9767, 0xFED89E21, 0xEEB7E5F1,
    0xC5FED2FA, 0x15DDE509, 0xE3FFE76A, 0xFDFFBFB9, 0x067AFF10, 0x00009A43, 0x541F9D40,
    0x0F7301AF, 0xE5DDFFAC, 0x62FAE7E9, 0xB00A2ADB, 0xF4F5156D, 0xEF817D73, 0x0F58C83E,
    0xFF58D7F2, 0xCFD3CBBB, 0x55B6C5F5, 0x2ADB6014, 0xFAE7E9EA, 0x914FFD42, 0xB94ECCFD,
    0xFEB277BB, 0x9FA79777, 0xAB6D8BEB, 0x55B6C028, 0xF5CFD3D4, 0x138BFD85, 0xD42BBBCA,
    0xF587FFCE, 0xFD3CBBBF, 0x5B6C5F5C, 0xADB60145, 0xAE7E9EA2, 0x38E4EA2F, 0x938E4EA9,
    0xE5DDFFAC, 0x62FAE7E9, 0xB00A2ADB, 0xF4F5156D, 0xBAC17D73, 0xEF16EB05, 0xF5DEF7BD,
    0x5DEF7BDE, 0x2D405BAC, 0xE17EFFD6, 0x6C75A7E9, 0xEC515BFB, 0x54F36C15, 0xF9F8F3D4,
    0x1777B8C2, 0xBD507E75, 0x783DCC06, 0x0FBDEBEE, 0xC3080000, 0x81FD66A0, 0x58C83EEF,
    0x3CD7F20F, 0x07DEF5F7, 0x61840000, 0x50FEB350, 0x3F6453FF, 0xEEEE53B3, 0x7AFB9E9D,
    0x000003EF, 0x59A830C2, 0x71C9D47F, 0x271C9D52, 0xDEF5F73D, 0x84000007, 0xFEB35061,
    0x903EEF80, 0x5FC81EB1, 0xEF337FE3, 0x8795479F, 0xC7DFFF9F, 0xE2554F18, 0x88F0FE17,
    0xF3C16209, 0xBEE31A7F, 0xFCF00000, 0x6BFD634F, 0x001B6915, 0xFF5836D2, 0x9FF9E2EA,
    0x8E4EA005, 0x71C9D493, 0xBCCDFF92, 0x1E551E7F, 0x1F7FFE7E, 0x89553C63, 0x23C3F85F,
    0xCF058826, 0x000069FF

  EW_FONT_KERNING_CODES( CutterDescrFont )
  EW_FONT_KERNING_VALUES( CutterDescrFont )
EW_END_OF_FONT_RES( CutterDescrFont )

/* Table with links to derived variants of the font resource : 'Cutter::DescrFont' */
EW_RES_WITHOUT_VARIANTS( CutterDescrFont )

/* Initializer for the class 'Cutter::FeedWindow' */
void CutterFeedWindow__Init( CutterFeedWindow _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CutterWindow__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  CutterPaperFeed__Init( &_this->PaperFeed, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterFeedWindow );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0008 );
  CoreRectView__OnSetBounds( &_this->Super1.CaptionRect, _Const0009 );
  ViewsText_OnSetString( &_this->Super1.CaptionText, EwLoadString( &_Const000A ));
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight );
  CoreRectView__OnSetBounds( &_this->Text, _Const000B );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const000C ));
  ViewsText_OnSetColor( &_this->Text, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->PaperFeed, CoreLayoutAlignToLeft | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz );
  CoreRectView__OnSetBounds( &_this->PaperFeed, _Const000D );
  CutterPaperFeed_OnSetType( &_this->PaperFeed, 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->PaperFeed ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &CutterDescrFont, ResourcesFont ));
}

/* Re-Initializer for the class 'Cutter::FeedWindow' */
void CutterFeedWindow__ReInit( CutterFeedWindow _this )
{
  /* At first re-initialize the super class ... */
  CutterWindow__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
  CutterPaperFeed__ReInit( &_this->PaperFeed );
}

/* Finalizer method for the class 'Cutter::FeedWindow' */
void CutterFeedWindow__Done( CutterFeedWindow _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterFeedWindow );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );
  CutterPaperFeed__Done( &_this->PaperFeed );

  /* Don't forget to deinitialize the super class ... */
  CutterWindow__Done( &_this->_Super );
}

/* Method for doing a fade animation within the derived window classes. The parameter 
   aValue runs from 0 to 255 during the animation. */
void CutterFeedWindow_FadeAnimation( CutterFeedWindow _this, XEnum aMode, XInt32 
  aValue )
{
  if ( aMode == CutterOperationModeExclusive )
  {
    if ( _this->dialog == 0 )
    {
      _this->dialog = EwNewObject( CutterFeedDialog, 0 );
      CoreRectView__OnSetBounds( _this->dialog, EwSetRectOrigin( _this->dialog->Super2.Bounds, 
      _Const000E ));
      CoreGroup__Add( _this, ((CoreView)_this->dialog ), 0 );
      _this->dialog->OnCancel = EwNewSlot( _this, CutterWindow_onCancel );
      CutterFeedDialog_OnSetPaperType( _this->dialog, EwNewRef( EwGetAutoObject( 
      &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetPaperType, CutterDeviceClass_OnSetPaperType ));
    }

    CoreGroup__OnSetOpacity( _this->dialog, aValue );
    CoreGroup__OnSetOpacity( &_this->PaperFeed, 255 - aValue );
    ViewsText_OnSetOpacity( &_this->Text, 255 - aValue );
  }
  else
  {
    if ( _this->dialog != 0 )
    {
      if ( aValue > _this->dialog->Super1.Opacity )
        CoreGroup__OnSetOpacity( _this->dialog, aValue );

      if ( aValue < 10 )
      {
        CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->dialog ));
        _this->dialog = 0;
      }
    }

    if ( aValue > _this->PaperFeed.Super1.Opacity )
      CoreGroup__OnSetOpacity( &_this->PaperFeed, aValue );

    if ( aValue > _this->Text.Opacity )
      ViewsText_OnSetOpacity( &_this->Text, aValue );
  }
}

/* 'C' function for method : 'Cutter::FeedWindow.OnSetPaperType()' */
void CutterFeedWindow_OnSetPaperType( CutterFeedWindow _this, XRef value )
{
  if ( !EwCompRef( value, _this->PaperType ))
    return;

  if ( _this->PaperType.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterFeedWindow_onUpdate ), _this->PaperType, 
      0 );

  _this->PaperType = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterFeedWindow_onUpdate ), value, 0 );

  EwPostSignal( EwNewSlot( _this, CutterFeedWindow_onUpdate ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void CutterFeedWindow_onUpdate( CutterFeedWindow _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperType.Object != 0 )
  {
    CutterPaperFeed_OnSetType( &_this->PaperFeed, EwOnGetInt32( _this->PaperType ));

    switch ( EwOnGetInt32( _this->PaperType ))
    {
      case 0 :
        ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const000C ));
      break;

      case 1 :
        ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const000F ));
      break;

      case 2 :
        ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0010 ));
      break;

      case 3 :
        ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0011 ));
      break;

      default : 
        ;
    }
  }
}

/* Variants derived from the class : 'Cutter::FeedWindow' */
EW_DEFINE_CLASS_VARIANTS( CutterFeedWindow )
EW_END_OF_CLASS_VARIANTS( CutterFeedWindow )

/* Virtual Method Table (VMT) for the class : 'Cutter::FeedWindow' */
EW_DEFINE_CLASS( CutterFeedWindow, CutterWindow, dialog, PaperType, PaperType, Text, 
                 _None, "Cutter::FeedWindow" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CutterWindow_UpdateLayout,
  CutterWindow_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
  CutterWindow_ChangeMode,
  CutterFeedWindow_FadeAnimation,
EW_END_OF_CLASS( CutterFeedWindow )

/* Color constant according UI design. */
const XColor CutterTextColor = { 0x3B, 0x5E, 0x54, 0xFF };

/* This is a bitmap resource. */
EW_DEFINE_BITMAP_RES( CutterPaperStripe )
  EW_BITMAP_FRAMES( CutterPaperStripe, Default, EW_DRIVER_VARIANT_RGBA8888, 201, 42, 0 )
    EW_BITMAP_FRAME( 3, 2, 188, 39, 0x00000000, 0x00000000 ),
    EW_BITMAP_FRAME( 3, 1, 188, 40, 0x0000149C, 0x00000000 ),
    EW_BITMAP_FRAME( 3, 1, 188, 40, 0x00002750, 0x00000000 ),
    EW_BITMAP_FRAME( 3, 1, 188, 40, 0x00003424, 0x00000000 )

  EW_BITMAP_PIXEL( CutterPaperStripe, Default )                 /* ratio 11.79 % */
    0x0409FF00, 0x411FF020, 0x4A0A4381, 0xA0AD882B, 0x05108735, 0xF118943E, 0x158B44DF,
    0xC8CC6A2F, 0x8C47A291, 0xEC86371F, 0x48A4B208, 0xAA4F2493, 0xA472C94C, 0x572F944B,
    0xB3096CC6, 0x349B4CA6, 0xC9CCEA6F, 0x9C4FA673, 0xED06773F, 0x50A8B409, 0x2A8F44A3,
    0xA874CA4D, 0x974FA453, 0xB50A6D46, 0x0C6BC9EA, 0x78298A0B, 0x82D720D0, 0x030D74B3,
    0xAB140FF6, 0xC148FD54, 0x6D76956A, 0xE16AB65B, 0xB6FB8DBA, 0x73BBDD2E, 0x6BDDCAF3,
    0xBE5F6F57, 0x0DFB0178, 0xE130B7FC, 0x0B1383C3, 0x71586C5E, 0x2188C663, 0xB8FC8E3B,
    0x93CBE532, 0x6CDE4B33, 0xCE677359, 0x0E7B4198, 0xE934B9FD, 0x8B53A3D3, 0x755A6D5E,
    0x61A8D66B, 0xB85D8EBB, 0xBB58DB94, 0xC7EBFD70, 0xDFBEDA9B, 0x1C0ABE10, 0x9F19B050,
    0x2DFE8846, 0xDFE5C311, 0x9C633494, 0xFD1E9743, 0xD6E9F579, 0xCECF6BA9, 0xEBF7BB1D,
    0x177FB7E0, 0xF97BBE1F, 0xABC9E6F1, 0xBD1ECF4F, 0xD7F0F3FB, 0xBF1F97BF, 0xF6F9FD7D,
    0xCFCFEBE9, 0xFBFFBF1F, 0x9FF7EE00, 0x57F60180, 0x120680E0, 0x0C8260A8, 0x81E0E822,
    0xA0F82E10, 0x58361184, 0x6A1684E1, 0x651A8557, 0x17C493FC, 0x45D3FC5A, 0xC3FC6D1C,
    0x94E96F0F, 0x171C06D1, 0x018FF708, 0xCED38105, 0x63FCEE3B, 0xFCE83A4E, 0xE93A8E73,
    0x8FA3C8E8, 0xE418EE3D, 0x6902428F, 0x12489164, 0x4C90E4A9, 0x93A4B91E, 0xE519264D,
    0x69425293, 0x52589565, 0x5C94E5A9, 0x97A5B95E, 0xE619665D, 0x69826297, 0x92689966,
    0x6C98E6A9, 0x9BA6B99E, 0xE719A66D, 0x69C2729B, 0xD2789D67, 0x7C9CE7A9, 0x9FA7B9DE,
    0xE819E67D, 0x93FE829F, 0xFCEA3ACE, 0xE5398E23, 0x1EC683FC, 0x4613FC76, 0xC6EC711D,
    0x70533A1B, 0x2E0685F1, 0xA350501E, 0xEA2A8505, 0x6A96A4A8, 0xA6A8A9EA, 0xACAAEAAA,
    0xABEAEAB6, 0xEB2AC6B0, 0x6AD6B4AC, 0xE6B8ADEB, 0xBCAEEBAA, 0xAFEBEAF6, 0xEC2B06C0,
    0xC38EBEB0, 0xC05B3FE8, 0x6F3FC6B1, 0x3FC7D204, 0x0E13EDBF, 0x01805E15, 0x0A308B82,
    0xFE3FCE44, 0x3DCFF3F8, 0x6DC3E0F8, 0xFB46A6B7, 0xE4FA3E0F, 0xE2B82AA3, 0xA98FD89E,
    0x4F8BBDBC, 0x3AA6EA3E, 0xF227BAAF, 0xE76F2F03, 0xBDAE23E6, 0x9BFB42F4, 0xCB7ADDA9,
    0x24FAAAAE, 0x033E8FFC, 0xB7F05A97, 0x2A8B72E1, 0x93FCFBC3, 0x1EA9C28F, 0xFCA9AF1C,
    0xC0716C74, 0x71BB8B00, 0x7BEB1DA9, 0xEF11BAEF, 0x1BA971E3, 0xBD2A9C1F, 0x30EB9AEA,
    0x8BFEE1B9, 0x2CFAC5EA, 0x0FA9318C, 0x3DCF73DB, 0xAA5B7AE3, 0x8BD717B9, 0xBE96CD6A,
    0x30C7B24B, 0xBBB48AAB, 0xAFA3F71C, 0xAC0747CB, 0xD32CBEB2, 0x0ACBF11C, 0xB933CCCB,
    0x70FD02E0, 0x5CE709A9, 0x8F11D3B6, 0x3BD12A4D, 0xBF31AA9F, 0xAFEC9B4B, 0xBC9773D3,
    0xDF5BBCAF, 0x59DDB01C, 0xA9F79C1B, 0xB6DC5B45, 0x3DAB07A9, 0xEF0BC52A, 0xC6AB7393,
    0x3FCCC334, 0xC5817446, 0xE284943F, 0xD4F53FC9, 0x4F31CF63, 0x0785B160, 0x8E4416D8,
    0xF36CDB35, 0x986B158F, 0xC52F0FF1, 0x246A6308, 0x2CC26C9E, 0x23FC7C20, 0x88F33CCF,
    0xE1388DEA, 0x1C85E3FC, 0x8BA2BE25, 0x9276A56E, 0x88FF298A, 0xFF1C8761, 0x3C0F03B8,
    0x8EF8CEA2, 0x63FFC0EF, 0xBEA5F23C, 0xFF2DCB92, 0x1B070170, 0xCE4380FF, 0x43CEA238,
    0x06CC3D0F, 0x9F247B52, 0x2EAA6270, 0x8FF304C2, 0xF1A46D16, 0x68DB354F, 0x75DDAA23,
    0x3FC8221C, 0x23048081, 0x698CC54C, 0x541FE344, 0xFE190348, 0x3C670C61, 0xA88654AE,
    0x61FE2444, 0x0C41FF07, 0x9D22A168, 0xACB7BCFE, 0x17E3085C, 0x28049AA6, 0x350FF13A,
    0xFF0F01F0, 0x44E647A0, 0xE4740E55, 0xFBF50B28, 0x11662A47, 0xC28D530B, 0x07F8AC15,
    0x06F0E818, 0x542A251F, 0xF4C780ED, 0x4887A0CE, 0xA5150271, 0x746E1530, 0xFD1205E3,
    0x0BBD7763, 0x61EE3D55, 0x81B87F8F, 0x4450FF76, 0xAA612C26, 0x19032460, 0x34A059D2,
    0x811171AE, 0x8C0F6C3C, 0x1C1F8E48, 0xAD6808A1, 0x98CE20AC, 0xE0B442C6, 0x43FC3506,
    0x55345F18, 0x89413022, 0x1081C87F, 0xE2DC1080, 0x700DC20A, 0xA974E37C, 0x7E2D47FB,
    0x5D530BC1, 0xF8A11482, 0x40F01947, 0x23BC7F87, 0xD510F11E, 0x0625128E, 0x36C3FD4A,
    0x2A1530AD, 0xE48B0164, 0x3985B279, 0xA125D50A, 0x5948C447, 0x309F1462, 0xD17E2D95,
    0x61AA040B, 0x643C8581, 0x8725CCA8, 0x10D20441, 0xA9849096, 0x650CC18A, 0xFF8EAFCC,
    0xA0348660, 0x01ABAA21, 0xD8FF8050, 0xC39222C3, 0x03608537, 0x10EA1B2B, 0x0C619030,
    0x13423153, 0x1F43645C, 0x9F0FF0F2, 0xDF20B061, 0xBE889F18, 0xD9ED0FF7, 0x509E54C2,
    0x41FE29C5, 0x63FC3AC4, 0x8768EE1D, 0xA881D0A8, 0x52A1D9E4, 0x2AC568A1, 0x5A8A954C,
    0x581FE2C8, 0xFE1A0368, 0x9C740E41, 0x2CE65443, 0x01FF09E1, 0xC2684F09, 0x8620BC54,
    0x86F0B6EB, 0x1B235165, 0xC1D5090D, 0x420880F0, 0x941083FC, 0x44D04480, 0x34678C85,
    0x48551FE3, 0xC1A232E4, 0x220552CC, 0xC7F88D12, 0x3499001C, 0x16229955, 0x1007F8AF,
    0xD58520AE, 0xFF186315, 0xAC354698, 0x201E8AB1, 0x08120961, 0x41D8B69D, 0xE59CB2A3,
    0x8710E289, 0x8CB186A9, 0x15C3FC64, 0x33A778B9, 0x7A0F2543, 0xAF146EE4, 0xB835A1C8,
    0x1552BFA1, 0x20A780F0, 0x268CBF65, 0x34FD9390, 0x95356FA8, 0xD681FF06, 0x7D15961C,
    0x2B9BB2A1, 0x7F8B5170, 0xCB19A314, 0xAD141530, 0x507F8AAA, 0xAAA57A75, 0xF1762F85,
    0x43105D8F, 0xC732A57E, 0x8AB13E8F, 0x85A3FC54, 0x2F552D0F, 0x7F8C418A, 0xBA17A2CC,
    0xDF1029E8, 0x3680BD31, 0x00E21BB6, 0xE18EA987, 0xC2AC7FB8, 0xD130B316, 0x0F42051B,
    0xAF83D47B, 0x3487FD69, 0x67AA69AE, 0xFF1A6350, 0x7F396BB8, 0xC99C2A26, 0x9C351690,
    0x85494D69, 0x03706F0D, 0xC78D03B8, 0x3FB1E43C, 0x7B8F697E, 0xA9863CDB, 0x6D8E41C2,
    0xFF151671, 0xD83B4750, 0xF85FCB21, 0x23F01FE2, 0x5AA8A94B, 0x50E05123, 0x56D8E651,
    0x0C8182A9, 0x7862C6F1, 0x2625552B, 0x7FDAE1FF, 0xDD1BC364, 0x9D4B5530, 0xD0550FF1,
    0x0DCEA0E2, 0x114B4455, 0x9708570B, 0x2E1AA68C, 0x61636C56, 0x543155CD, 0x2D455B67,
    0x070C7B72, 0xDF1B4A9A, 0x3D5586F0, 0x964CC97B, 0xF8B9370A, 0x518E2FC7, 0xE391530C,
    0xB158E61C, 0xDF154A91, 0x8410787F, 0x3B475AA5, 0xCE8A09F4, 0xA6BCAF86, 0xC11823C2,
    0x91A83DCA, 0xBD7CACD5, 0x61B835EF, 0x501AB73A, 0x2B9C3486, 0xB401F144, 0x19D506B8,
    0x34D535C7, 0xFB0FFA40, 0xF51E70E6, 0xA0416C00, 0x145B8AC6, 0x54C51896, 0xDC08EF2C,
    0x57D2C68A, 0xC5A30F54, 0x51A75764, 0x2CB2A153, 0x2A51BE46, 0xD7596549, 0xA991B649,
    0x1E1FD87A, 0x9026C031, 0xE0FF0BC1, 0x490E3846, 0xA2A8548A, 0x01C82D0F, 0x55201C0F,
    0xD3EB1286, 0xA9F50EA0, 0x86BB9ACA, 0xA2685686, 0x53AAB54E, 0x434131BD, 0x8EAB66CC,
    0xA585ACB5, 0x7ABADFC2, 0x75CEB8D6, 0xB8860FA8, 0xB7F9A152, 0x1B83985F, 0x3B8641FE,
    0xF2BB4F30, 0x262347B8, 0xC0288951, 0x783F0760, 0xABF5A740, 0x1B5B6AEB, 0xE79CF155,
    0x0F087B0E, 0xDAF8B879, 0xA9370EE0, 0x28D0FC6E, 0xA92B619C, 0xD7B8B078, 0x3150EECD,
    0xA0971362, 0x4DDAA427, 0xE4177AEF, 0xC38293CE, 0x2A6CBC3F, 0x0A2FE9FC, 0x3FC30866,
    0x4388B0F4, 0xF61ED394, 0xC015C728, 0xD8188620, 0x68440820, 0x4DEDB4C0, 0x036B5171,
    0x2D18FFC1, 0xF1564752, 0x691A878E, 0x3865ACB3, 0x8F6D41CA, 0xD1D41C9B, 0xFE3546B8,
    0x74648C01, 0x8F30E4F3, 0x21FFAD75, 0xA3FDF689, 0x952D2FA5, 0xDA91782C, 0x0B217823,
    0x55C4F8FF, 0x738E71CD, 0x460742DC, 0x7207C248, 0x28238440, 0x8F31E260, 0x4EA7D4BA,
    0xFD5BAAF5, 0xBACF58EA, 0xB9772A40, 0x0AA16821, 0x344650FF, 0x648FF19E, 0x9219832C,
    0x8C10C1C0, 0x1FA60150, 0x290353C1, 0xAD3CB648, 0xFDDBBAF5, 0xBBCF78EE, 0xF57EF7DE,
    0x10705583, 0x194278FF, 0x77CF70C0, 0x921193E8, 0x9E1AC4A0, 0xE80703ED, 0x284808A0,
    0x4EF9D400, 0xF521D675, 0x87F88938, 0xF94CD4F9, 0x1FF8B7BA, 0xE644607C, 0xCEDE28BD,
    0x6E15BDEB, 0x2E2207FE, 0x9A410931, 0x8B27F3BE, 0xBE523FCB, 0x6A1D52F2, 0x50E5301D,
    0x4E676AFA, 0x0F81D076, 0xB3C2187F, 0xC878E07F, 0xAFF2600B, 0x757FF987, 0xC567ACD3,
    0xCFC9CA03, 0xC9F57ABA, 0xA1EC3F86, 0xFE8D97B2, 0xD9E2CDDD, 0x5D0727D0, 0xDDBED574,
    0xF60EC1D5, 0xA87C088E, 0x4B294598, 0x49C85D25, 0xE88F1FEE, 0x01FE2644, 0x235E4C0F,
    0x41305F53, 0x8430800D, 0x091E3C06, 0xE27C8001, 0xE30F7157, 0x77680364, 0x5A80771C,
    0xA38630F7, 0x23777A80, 0xE745323D, 0xF57D2A34, 0x39373E0F, 0x491B90B0, 0xF0A654B5,
    0x03E03B02, 0x0A7FF016, 0xD15D7FF0, 0x72577315, 0x2A82980E, 0xD82C82B8, 0x2B682E82,
    0x8E092082, 0xF1CC1CA0, 0x0DA0D40F, 0xC07190D9, 0x10800C20, 0x04003D03, 0x077FF012,
    0xF8028240, 0x4FC6B882, 0xD00C9289, 0x64D552F0, 0x3766F684, 0x0C0CF0C9, 0xE23D59A6,
    0x0E072684, 0xCD3E40E3, 0x66014720, 0x0E575385, 0x2B398238, 0x93B72A54, 0x4284263B,
    0x6706C057, 0xF5BE4290, 0x08607F0F, 0x3E058088, 0x07FE0408, 0x84400479, 0x27102771,
    0x46B985E8, 0x7005C10A, 0x7C848702, 0xF7B76F58, 0x06504E0F, 0x7E0FF05F, 0x48834398,
    0x09545A71, 0x7606F3F3, 0xD79C6684, 0x0EC71488, 0xD92350EE, 0x72A57B21, 0x05060960,
    0x790FF701, 0xF08608A0, 0x05A0E06A, 0x36066063, 0x103E0330, 0x7FF00C04, 0x5E878002,
    0x63A287A1, 0x6A66B189, 0x8F5B40FF, 0x78B68093, 0x4C43766D, 0xBD882589, 0x24856ED8,
    0x0D20CD5B, 0x1E8C3407, 0x77B23077, 0x2A583983, 0x40635633, 0xA0530580, 0x6430EC63,
    0x5804E0C5, 0xB02205A0, 0x8230437F, 0xB2000790, 0xA8B58268, 0x8E78E68C, 0xEA8E98E8,
    0x60992AF8, 0x0FF0A30A, 0x5605C044, 0x90CE5860, 0x0C60C458, 0x300A70C8, 0x203F03C0,
    0x00876E01, 0x728E3824, 0xC8EB8E57, 0x3340FD2B, 0xE0905373, 0xA8BC3276, 0x3766B190,
    0x5A90E34D, 0x948153B7, 0x0E10FF4D, 0xAB5C95F9, 0x906D0AC0, 0x01003C69, 0x010057FF,
    0x28458E19, 0x0B92B491, 0x6E1640C1, 0x94F72CE0, 0x6AA92689, 0xCD4996DD, 0xF73C5A67,
    0x5716B292, 0x890784A5, 0x67AE0850, 0x6BF6B893, 0x863B1514, 0x22A65885, 0x05D4FA64,
    0x790664EC, 0xA2EC4950, 0x0800790F, 0x32049083, 0xF04103D0, 0x9238B000, 0x03825048,
    0x693E2DA9, 0x0C60BE2B, 0x8F07E5A5, 0x149608B0, 0x95D3D00F, 0xF20F169F, 0xD07E5DC0,
    0x79646D08, 0xAF6A9968, 0x74EF0B80, 0x7AD09608, 0xA997135E, 0xA43B6776, 0x4EF5836F,
    0x865347EC, 0x38890648, 0x52A09308, 0x480D379B, 0xC0550520, 0x04203702, 0x00875045,
    0x495A9588, 0x97A95C8B, 0x9890D2B5, 0xD2F92C49, 0x2C299B85, 0x9F99A30F, 0x95DA6A99,
    0x2A29793B, 0x415857A8, 0xF0540590, 0x0FF97455, 0xE50E40E3, 0xC0420AE0, 0x01E04F04,
    0x0904598F, 0x69029940, 0x9A377499, 0xC29C19C0, 0xF2BA9C39, 0x8F16F14D, 0xD75D58F3,
    0x50B40B05, 0x03308D0B, 0x1004203F, 0x184376E0, 0x9259948E, 0xE46479C4, 0x965A7213,
    0x3882B89D, 0xDB3F2162, 0xE62A0E00, 0x0D82BB9D, 0xBB3A60DD, 0x65AA3AD0, 0x30F2BD9E,
    0xFF8380D6, 0xE2A63D20, 0x6F328E61, 0xF42CB2C9, 0x00890FA5, 0x05C09209, 0x3D03902E,
    0xC8769560, 0x8799249B, 0xEE90422F, 0x10AF88A9, 0x3933914A, 0xB3A0B8C2, 0xE5EC7E42,
    0x55698404, 0xB7A125E8, 0x94247352, 0x79B3AA86, 0x7F2BBA1A, 0x87028A06, 0x7245E393,
    0xFF783781, 0xF5250770, 0x0700EB6A, 0x3A07A078, 0x50409530, 0x8B10487C, 0x59A399D6,
    0xE9C49D89, 0x9E83D03C, 0x219A25D7, 0x031B2AAA, 0x3CF3CDA4, 0xE92B5A43, 0xB0FF7E50,
    0xA496290D, 0xE13272BC, 0x26489179, 0x0FF5475A, 0xFF220048, 0x00A40960, 0x0E92860A,
    0x4E0C60E9, 0xE02004B5, 0x8E299276, 0x3BA68A3A, 0x087BA08A, 0xA6FA6EA5, 0xED82FA70,
    0x8A0F8EF8, 0x54A3AE7D, 0xD00990D6, 0xF0170429, 0xA699BB7F, 0x0AA6C9D7, 0x82E69C4A,
    0x2B3A716C, 0x8793D6A4, 0x03672ACA, 0x6A2A8BA2, 0x9996D30F, 0xB06004A7, 0x28D5F805,
    0xAC0AA0A6, 0xD0320750, 0x7909208A, 0x59A06006, 0x7A099BD9, 0x9DC9DE99, 0x3B3CB063,
    0x12A6A8F6, 0x0690FF93, 0x076E96F3, 0x22AAAAC5, 0x4256750A, 0x6B7F44E3, 0xB2AEAB49,
    0x08B7034A, 0xA68D18A5, 0x15EC5EA2, 0x6FA07406, 0x5C4E56F9, 0x606E0F66, 0x04C07807,
    0x9203F8FB, 0xAAA20039, 0xAA5995A6, 0xCA9D99BF, 0x77F14074, 0xABCA8951, 0x355112A4,
    0xF38D3EE7, 0x2A6AE040, 0x970FF71A, 0x3A5E0A30, 0xAE76B02F, 0x358982A7, 0x33B54572,
    0x0D52A5A5, 0x4073E9F2, 0x20950877, 0x0DA4E64C, 0x6B06805F, 0xE8AC02F0, 0x54F03C8A,
    0xA4AD5001, 0xDAF0A6DA, 0xB0FB0EB0, 0x0D31B2C1, 0x173B2A18, 0x9AD75E63, 0x070B60AA,
    0xE0DD0DB5, 0xA630B90D, 0x0A8DE019, 0x603402C0, 0x03B00003, 0xD604A047, 0x88B3A81A,
    0x5BD0E4AD, 0x777E05BF, 0x36503B63, 0x307105B1, 0xE42A7AEE, 0x56A87332, 0x5113072A,
    0x892DE333, 0xE2A599DA, 0x61F5BD5C, 0xBAB472A8, 0xA36B3585, 0x70839832, 0xE842A2AA,
    0xE36B6BD3, 0x2AA57F31, 0x2745533E, 0xF86CB413, 0x65C3195B, 0xA1AEF2FD, 0xCA8930F2,
    0x2A2B3D1B, 0xA99F6327, 0x8A427332, 0xB4845338, 0xBF3E62A7, 0x5B4D5635, 0xB49A8E31,
    0xA3A8A39A, 0x8B3031B2, 0x6115CCB5, 0x8931B2A4, 0x6102B36A, 0xB40B600F, 0x42A202A8,
    0xF38A0F9A, 0x8942AA6A, 0xA6352226, 0x99405CE2, 0xB549402A, 0x7D3502A3, 0x98122A9B,
    0x32431538, 0x9CB57B92, 0xCB8E454B, 0xB84B57B5, 0xF72EA382, 0x533C9090, 0x907B9010,
    0x272A1B67, 0x72A8B9B3, 0x4BB9A7B5, 0xB43150FC, 0xBB592A7B, 0x45A2AAB4, 0x6104B45C,
    0xB0CA1CD0, 0x0C183A4C, 0x1291E089, 0x78AD0350, 0x01F01A00, 0x3D000021, 0xB04C0490,
    0x6D3A69B2, 0x954D0080, 0x24B10A20, 0x3D49605A, 0xCD89E225, 0x67043034, 0x4C241170,
    0xC86602A5, 0x986A0565, 0x2A56574C, 0xA27AC087, 0x070608A8, 0xBE2102B8, 0xE92256C1,
    0xE48F2A5A, 0xBBD933A5, 0x398602A1, 0xF3C13BF2, 0x7B22A59A, 0x463CCAA9, 0x679F2A29,
    0x63A07742, 0x8D67A2A5, 0x83E2AC94, 0x0FFB6F82, 0x7F083071, 0x23B00FF0, 0x0C32A53B,
    0x6955E0CA, 0xD56B86B8, 0x46B2A456, 0x3208346D, 0x00FF99E8, 0x22272A0D, 0x0D3E0071,
    0xD2A3AE64, 0x09809B08, 0xF6A2DA2B, 0x10DD0FFB, 0x9F95B20E, 0x980C3595, 0x50992A45,
    0x9F70A20A, 0x05BA2C0F, 0x47B8A4B1, 0x07307606, 0xA54355CF, 0x35B96222, 0x0E749549,
    0xAE2A14FD, 0x7AB906BA, 0x2A43D844, 0xEF0C00B8, 0xF4764745, 0x55A0E30D, 0x742FABB8,
    0xB3FF0850, 0x6F109A08, 0xDD7382A5, 0x6C2DC313, 0x3FB0D40D, 0x8F98631E, 0x3BD90FF0,
    0x10280640, 0x6410D51F, 0x66700FFC, 0x08E2A54C, 0x700FF7F2, 0xA07E0810, 0xBE0A97B7,
    0x940BD5A6, 0xC0A12A55, 0x06A58D0A, 0xCE42807D, 0x41055903, 0x94A0FFAC, 0xA56FC4F6,
    0x33D49A92, 0x6122456C, 0x7E06CB60, 0xDBDB7D70, 0xAC22A4BD, 0xCA225223, 0x12E95CD9,
    0x4597054D, 0x6746647C, 0xC4CC762B, 0x0CF2CB51, 0xA55C80D5, 0x952A9862, 0x8A508907,
    0x096282A5, 0xD2CB4746, 0x2A5BDF0B, 0x704914AF, 0x7C8D0820, 0x28B102B7, 0xFF06128D,
    0x6B150FF4, 0x4926662A, 0x67A47494, 0xB0F01059, 0x0786C45D, 0xE55E7C50, 0xA7D531E2,
    0x0570FF07, 0x9DC0B489, 0x30A30A20, 0x010AD106, 0x0404370F, 0x102001B0, 0xBD554EBD,
    0xA76D3AD5, 0x80BF5A95, 0x2A56350C, 0x9E3A69E8, 0xC5E80AB0, 0x5817B84F, 0xFF3DB5DE,
    0x65EE0B70, 0x0E50E22A, 0xA609A28D, 0x4A540A40, 0x0B72A53E, 0xDF4200C0, 0x72A1BBE5,
    0x09F28928, 0xD80FF4F2, 0xE45D6540, 0x43565872, 0x03C5A367, 0x00FF6D96, 0x2A4CC2CC,
    0x0160C84A, 0x2B905035, 0x53F5CC61, 0x19C33616, 0x37DC5C46, 0xCB45AC9F, 0x060AA339,
    0x554867C5, 0x4879E32A, 0x1C0A709A, 0xA44A854D, 0x59459281, 0xE3B672DE, 0x93D66380,
    0x5B2C3DD2, 0x7F7D963C, 0x2105D249, 0x3F73C7CF, 0x796493F9, 0xF2E761C4, 0x3B9B3437,
    0xA5579605, 0x710B85B2, 0x3B85D943, 0x15D3F510, 0xE55C0FF6, 0xC812A555, 0xA809B0FF,
    0x16773C80, 0x2EF9072A, 0xF00B45B7, 0x45F20C25, 0x09D58B2A, 0xE70E2D29, 0x61052A00,
    0x0BE48AB6, 0x350B348D, 0xFC0280D9, 0x4494470F, 0x84D60692, 0x4AB267D6, 0xA3E102D6,
    0x63D340FF, 0xA2A24654, 0x0B05C98B, 0xA1B5061D, 0x3815A202, 0xBE976453, 0xE84F1661,
    0x3620A865, 0x5400A730, 0x2F0C7632, 0x4BE22A47, 0x4CDD0ED3, 0x28D2331E, 0x62A55EF9,
    0xC5E3E4C6, 0xA52C79B0, 0x348D4B32, 0x0EC7B6D6, 0xFFD9F7B8, 0x55D76530, 0x4DD4DB2A,
    0x5B507505, 0x60EE105B, 0xAB716596, 0xA1DAC729, 0xF0CE0C82, 0x398D7652, 0x40CA85CC,
    0x0503AB63, 0x3E22A68C, 0xF64743E4, 0x5CEECAA3, 0x05D0F036, 0x3E069066, 0xD9900360,
    0x02A02400, 0x1500202D, 0x001B01A0, 0xBD403E00, 0x07AA3BD6, 0x0B0CB2DA, 0xE17E16B1,
    0xE52BCE18, 0x27EF816A, 0x3B642309, 0x4C0C83B7, 0x80590560, 0x04503902, 0x1600B049,
    0x2001E0C0, 0x01601501, 0x4D04AE0E, 0xDAD6E110, 0xE1929F29, 0x3AE39E38, 0x6DBF2AFE,
    0x54554361, 0x78548CCE, 0x7A630AAA, 0x04CBD301, 0x1D018008, 0x100001E0, 0xE31E2F01,
    0x08D01E33, 0x7105004A, 0x0C70BE0D, 0xB20A65A5, 0x7D4A5410, 0x60DE5D50, 0x62E5FD4B,
    0xEE64E61E, 0xE60E65E5, 0x68E69E66, 0xAE63E6BE, 0xE6CE6DE6, 0x72E6FE67, 0xEE74E71E,
    0xE70E75E6, 0x78E79E76, 0xAE73E7BE, 0xE7CE7DE7, 0x82E7FE77, 0xEE84E81E, 0xE80E85E7,
    0x88E89E86, 0xAE83E8BE, 0xE8CE8DE8, 0x92E8FE87, 0xEE94E91E, 0xE90E95E8, 0x98E99E96,
    0xAE93E9BE, 0xE9CE9DE9, 0xB7E7FE97, 0xDE5F5B95, 0x0FF7D606, 0xB1C3CA24, 0xA0A888A5,
    0x02E07F0A, 0x36A0303A, 0x50067FC0, 0x00600600, 0x18014000, 0x4E500190, 0x1C4AA4E3,
    0x760CC102, 0x87800880, 0x04D0FF9C, 0xC705D063, 0x8690063E, 0xECCECDEC, 0xD2ECFECB,
    0xEED4ED1E, 0xED0ED5EC, 0xD8ED9ED6, 0xAED3EDBE, 0xEDCEDDED, 0xE2EDFED7, 0xEEE4EE1E,
    0xEE0EE5ED, 0xE8EE9EE6, 0xAEE3EEBE, 0xEECEEDEE, 0xF2EEFEE7, 0xEEF4EF1E, 0xEF0EF5EE,
    0xF8EF9EF6, 0xAEF3EFBE, 0xEFCEFDEF, 0x02EFFEF7, 0xEF04F01F, 0xF00F05EF, 0x08F09F06,
    0xAF03F0BF, 0xF0CF0DF0, 0x4CEF3F07, 0x7ECA0620, 0x9A56F306, 0x9108B0F7, 0xE0510930,
    0x01004183, 0x3703502B, 0x50000030, 0xE0B000F2, 0x3203EE0D, 0x7E13EBEE, 0xAF2086AD,
    0xE66370F4, 0x1F320F10, 0x0E52C60F, 0x39F370F3, 0x8F3B28CF, 0xF3FF3AF3, 0x3EF40F3C,
    0x3F44F41F, 0xF3DF46F4, 0x49F42F48, 0x7F4BF45F, 0xF4FF4AF4, 0x4EF50F4C, 0x3F54F51F,
    0xF4DF56F5, 0x59F52F58, 0x7F5BF55F, 0xF5FF5AF5, 0x5EF60F5C, 0x3F64F61F, 0xF5DF66F6,
    0x69F62F68, 0x7F6BF65F, 0xF6FF6AF6, 0x6EF70F6C, 0x3F74F71F, 0xF6DF76F7, 0x79F72F78,
    0x7F7BF75F, 0xD17F5AF7, 0x410F40E5, 0x10F60EF4, 0x0C50C50C, 0x44EB30AE, 0x002401C0,
    0x00303403, 0x7907706F, 0x923C0000, 0x09A0000E, 0x000A009E, 0x202E0220, 0xA0100003,
    0x02F9F03D, 0x1FA1FA2A, 0x00000010, 0x0409FF00, 0x411FF020, 0x260A3481, 0xE0AD482B,
    0x05108634, 0xF1189436, 0x158B44DF, 0xC8CC6A2F, 0x8C47A291, 0xEC86371F, 0x48A4B208,
    0xAA4F2493, 0xA472C94C, 0x572F944B, 0xB3096CC6, 0x349B4CA6, 0xC9CCEA6F, 0x9C4FA673,
    0xED06773F, 0x50A8B409, 0x2A8F44A3, 0xA874CA4D, 0x974FA453, 0xB35A6D46, 0xDD6DC39A,
    0xF66AA9BC, 0x68A85A2C, 0x9A79A8D3, 0xA69E8F47, 0xBDDE9A82, 0x9A09A783, 0xA49B69B4,
    0xE9B6AB51, 0x4665B298, 0x766B3192, 0xBEC160D0, 0xEAFD6CBA, 0xA9D38FF5, 0xB140FF52,
    0x7178954A, 0x218AC663, 0xB8FC8E3B, 0x93CBE532, 0x6CDE4B33, 0xCE677359, 0x0E7B4198,
    0xE934B9FD, 0x8B53A3D3, 0x755A6D5E, 0x61A8D66B, 0xBAFD8EBB, 0xB3DBED36, 0x6DDECB73,
    0xDE6F775B, 0x0EFB81B8, 0xF138BBFE, 0x0B93C3E3, 0x795C6E5F, 0xA1C8E673, 0x4A7CA73B,
    0xAB7FAC16, 0xAEC2D16C, 0x5728DFFD, 0x527DFEAC, 0xA91FEA85, 0xDFB0AD57, 0x180B6EDA,
    0xB7F07EE1, 0xB7F08C15, 0xCD8280E0, 0xF44234F8, 0x2E18896F, 0xC6914CFF, 0x580E0118,
    0x22028120, 0x07826068, 0x83E0C836, 0xE0A8460B, 0x584A0E84, 0x62108521, 0x17866168,
    0x87E1C876, 0xE1A8861B, 0x588A1E88, 0xA2208922, 0x278A6268, 0x8BE2C8B6, 0xE2A8C62B,
    0x58CA2E8C, 0xE2308D23, 0x378E6368, 0x8FE3C8F6, 0xE3A9063B, 0x590A3E90, 0x22409124,
    0x0D926469, 0x1946A156, 0x85F124FF, 0x1174FF16, 0xF0FF1B47, 0x653A5F03, 0x05C701B4,
    0x406409F5, 0xF3B4E041, 0x98FF3B8E, 0xFF3A0E93, 0x3A4EA39C, 0x2719BE6D, 0x29D26E9C,
    0xE6739D67, 0x779EE789, 0x9FE769F6, 0x2819FA7A, 0x2A127CA0, 0x2683A168, 0x87A2E88A,
    0xA3E86A36, 0x291A3A8A, 0x2A528CA4, 0x6693A569, 0x97A6E98A, 0xA7E96A76, 0x2A1A7A9A,
    0x2A929CA8, 0xA6A3A96A, 0xA7AAEA8A, 0xABEA6AB6, 0x2B1ABAAA, 0x2AD2ACAC, 0xA4FFAD6B,
    0xFF3A8EB3, 0x394E6388, 0x87B1A0FF, 0x5184FF1D, 0xC0FF1C47, 0x533A0FD7, 0x0685F170,
    0x4073FE61, 0x6416C041, 0x66D8B5ED, 0xDCB6EDAB, 0xB7EDEB76, 0xEE2B86E0, 0x6B96E4B8,
    0xA6E8B9EE, 0xECBAEEAB, 0xBBEEEBB6, 0xEF2BC6F0, 0x6BD6F4BC, 0xCEF8BDEF, 0xFFAFCE3B,
    0x1AC7016C, 0x4811BCFF, 0x63ECFF1F, 0x78543870, 0xD1080601, 0x3910298E, 0xCFE3F8FF,
    0x83E0F73F, 0x6F12C44F, 0xFF3EF06B, 0x3E4FA3E0, 0xB17C56DF, 0xAB6CFD96, 0x80F9C76C,
    0x293E2DEC, 0x3F0FD967, 0xF287C326, 0xBCB72BC5, 0x4F12B6F1, 0xFAB7F22C, 0x3E8FFCF4,
    0xF19CE306, 0x0B7313CE, 0xF8F9C734, 0x27CEEDEC, 0xD3CFCB6B, 0x72ACA4FF, 0xDC5F2CB7,
    0xC34FB674, 0xDCD4F2CC, 0xD40FBCEE, 0xB3BC9EDA, 0x1C1752B7, 0x7739B6B4, 0x49B7F16C,
    0xD2B3F3EB, 0xCF6DA6D9, 0x5C60F73D, 0x2737B674, 0x16B6F5CD, 0xD3B32D5B, 0xF2BD9750,
    0xAB6F24DA, 0x5363CC25, 0xD9D8336D, 0xDF7193F6, 0x337CFB6C, 0x53E343DA, 0xF6DADA77,
    0xDFE6B6EC, 0xDE398E42, 0xF21DB741, 0x9B6357DA, 0xE79DCCF5, 0xD8E33233, 0xD9390C9E,
    0x761E9F2D, 0xAB7734E5, 0x7770D477, 0xD8E7F6BD, 0x3EF3FD1E, 0x4C6B8B6E, 0x63FCCC33,
    0xFC581744, 0x9E284943, 0x3D4F53FC, 0x04F31CF6, 0xD0785B16, 0x58E44170, 0x567C8DB3,
    0x3FC661AC, 0xCC2314BC, 0xB27891B6, 0xF080C049, 0xF33C8FF1, 0x37AD73CC, 0x8FF384E2,
    0xF9607217, 0x6CB82E8A, 0xA62A49DB, 0x1D8623FC, 0x3B87F872, 0x6B8F01E0, 0xF31DE999,
    0xF63FDFAB, 0xBD6D3FD7, 0xF8B71722, 0xC0E01707, 0x83807F86, 0x16B8E31C, 0xB011E859,
    0x122FA840, 0xB6C4E09F, 0x608C2174, 0x6D0B43FC, 0xAA3FC348, 0x5C6D0DB1, 0x387574EB,
    0x11FE2084, 0x62304808, 0x4698CC5B, 0x8541FE34, 0x1FE19034, 0xF14670C6, 0x4A8865B4,
    0x761FE244, 0x2CC52190, 0xB9ED2D77, 0xCB008270, 0xB17E3085, 0xA28049AD, 0x0350FF13,
    0x0FF0F01F, 0x5AEF3C7A, 0x9B0740E5, 0x7FC290B2, 0xB11662A4, 0xC277D6D8, 0xBC3A0603,
    0xB2BC4E21, 0x81E03B56, 0x81E833C0, 0x05409C58, 0x8B15B462, 0x0BCFE45D, 0xFC07FA57,
    0x7AAD67E4, 0xFF1EC3DC, 0xFEFA4370, 0x584C88A1, 0x48C15B62, 0xF40A3206, 0xC35C6943,
    0xDB4AD71A, 0x1FC1181E, 0x1142383F, 0x3FDD6305, 0x690D319C, 0x6A0DD09C, 0x243087F8,
    0x6044ADB9, 0x90FF1282, 0x81002103, 0x8415E0BB, 0xC6F8E01B, 0xBDF7BA3F, 0x1782FC5A,
    0x2904BADB, 0x328FF142, 0xFF0E81E0, 0xE23C4778, 0xF1372D71, 0xFAC70C4A, 0x91602947,
    0xC2A16D1E, 0x668BFE3A, 0x2D64D918, 0x8ED01665, 0x28C4B2C4, 0xC12DB13E, 0x6A0A8610,
    0x752E21F8, 0xC99BAD69, 0x34B41061, 0xB1242584, 0x433062AD, 0x254CA719, 0x8D01A433,
    0x660D7D6B, 0x1EE1BC35, 0xD91FC116, 0xD81A4415, 0x9C3FC6B8, 0x20602504, 0x62DB18C3,
    0xC91E2684, 0xE1E43E86, 0x9BAB731F, 0xC181BEB5, 0x1FF0512A, 0xB6C5B40F, 0x538AA13C,
    0x758EC3FC, 0xDC3AC7F8, 0xA16B8ED1, 0xB3FE5763, 0xD142B203, 0x2B6C558A, 0xC590B515,
    0x06D0B03F, 0x1C83FC34, 0xB5C738E8, 0x2DC59A32, 0x9E1203FE, 0x78B68B38, 0x6DF18C41,
    0xA3000DE1, 0x71B03646, 0x01E19DAD, 0x07F88411, 0x09012821, 0x190B5DBB, 0x42C668CF,
    0x658FF7BD, 0xDA9FA344, 0x1A24440A, 0xCC398FF1, 0xEB7F5932, 0xF1622996, 0xE1007F8A,
    0x5F00520A, 0x8FF18631, 0x1AC35469, 0x1201E8AB, 0xEB812096, 0x671D8C3D, 0xC4FC0CAA,
    0x2C61ADB1, 0xC8AE2163, 0x79FF4885, 0xC3D0792D, 0x4578A37D, 0x0DC1AC78, 0x872B6A17,
    0x75CEAC3B, 0xD22D76B6, 0xA0AEB5CE, 0xBD1396DD, 0x95F5D9A3, 0x735085E5, 0xF16A2E05,
    0x6334628F, 0x641ADB19, 0x0FF15463, 0x5B5022C4, 0x2EC5F0B5, 0x620BB1FE, 0xE65B5098,
    0x5627D390, 0x0A3098C1, 0x28BD5B55, 0x31FE3106, 0x02E85E8B, 0xA39325AE, 0x5341FE5A,
    0x1C4376DE, 0x31D6D8E0, 0x599DC788, 0xF4944964, 0x583D47B0, 0x2A841E1B, 0xA0CF5B54,
    0x8B4E43C6, 0x4B5DBA51, 0x45A0A974, 0xD68ACD1A, 0x1BC3616C, 0x4DAD60DC, 0x790F31E3,
    0x0F6650BC, 0x8F43A08C, 0x9070ADB1, 0x2DCB6203, 0xC1DA3A8F, 0xC2FE728E, 0x2120FF17,
    0xABA5B630, 0x57B6EC75, 0xB69880E6, 0x060C8182, 0x5E7F02C7, 0xB932E6ED, 0x66DE0ADB,
    0xD8DD1BC3, 0xF19D5816, 0x42D0550F, 0x16B3A2A7, 0x0B114B51, 0x92C3FC57, 0x7B96D498,
    0x1F0B09A5, 0xD6910F43, 0x6A2ADCE2, 0x0863DE61, 0xF8DA5B71, 0xA32A3786, 0x71F2664C,
    0xC7F8B917, 0x8C518E2F, 0x1CE3916D, 0x9E0CA8E6, 0x618BA5B4, 0xDB9CD86C, 0x5A3B475A,
    0x5AE60A0A, 0x5ADBC1F0, 0x1B2324E5, 0xCB59C2C1, 0xCF0598B2, 0x0DC1AE3F, 0x87567CF9,
    0xCA234466, 0x260F66F6, 0xCEAEA756, 0xA6B6EF90, 0x887FD531, 0xA8F3CF42, 0x55AD0707,
    0x82DC5638, 0x67B8C4B6, 0x62C7E36E, 0x4D1622DE, 0x2D1B4AD6, 0xA6BE0B84, 0x899B6B6E,
    0xB89AB371, 0x8EE2AD99, 0xF5D7BAAF, 0xF0FEDA56, 0x81361B88, 0x07F85E0C, 0x4BA1C237,
    0x1542A452, 0x0E41687D, 0xC300E078, 0xC85F34FE, 0x97B2B64E, 0x3B66ECCD, 0x8ED0D9FB,
    0x4DA7B4B6, 0x5B5B6AED, 0xEF5E915D, 0x86E0E617, 0x0EE1907F, 0x3CB02401, 0x4988D1EE,
    0x300A2254, 0x1E0FC1D8, 0x5F116C60, 0x8EF0DDFB, 0x4DE7BCB7, 0xFBDB7AEF, 0xB7CEF8DE,
    0xB8EB28BB, 0x858409BB, 0x19828C2E, 0x3D0FF0C2, 0x1810E22C, 0xCA3D87B5, 0x88300571,
    0x08360621, 0xD01A1102, 0xFC5B7D6E, 0xB8CF18E2, 0xF1CE37C6, 0xD93C7B8E, 0x41714EFD,
    0xEB8BC164, 0x590BC11E, 0xCC638F10, 0xDC738E71, 0x48460742, 0x407207C2, 0xA0282384,
    0xB9CF1FE2, 0xF3CE77CE, 0xE7FCFB9E, 0xF2D7C740, 0x2D04379A, 0xCA1FE154, 0xFE33C688,
    0x30658C91, 0x18381243, 0x402A1182, 0x6B4C23F3, 0xD8710520, 0xF5CE83B1, 0xEBFD7BAE,
    0xD8BB0F60, 0x24B3BD9F, 0x4F1FE20E, 0x54180328, 0x32968EFA, 0x58941242, 0xE07DBA43,
    0x01141D00, 0xF9C00509, 0xF7CEF7D8, 0xEFFDFBBE, 0x8779F780, 0x0EA4FA17, 0xECA32FAA,
    0xFE1F03A0, 0xFF748430, 0x1777EEA0, 0x4F03DE80, 0xFE5BCAF9, 0xFCCF98F2, 0x3C6BF8BC,
    0x11E3FDE4, 0x3FC4C89D, 0x6BCCB1E0, 0x260BEB34, 0x861001A8, 0x23BB00D0, 0xD7930021,
    0x4F67ECBC, 0xBEDBDAFB, 0xCF0D99EF, 0x0B0793C9, 0xAFF5F63F, 0x0EC178A6, 0xD701607C,
    0x7DF5C02B, 0xDDD68BE9, 0x3BE6FB7D, 0x8FD0F9FF, 0x2105BD7E, 0xE7F44709, 0x46D1163F,
    0x0BD1FE36, 0x10030860, 0x1001E831, 0x03FAE024, 0xD2F63F20, 0xD7FABF4F, 0x9BF6FECF,
    0xD3EE7D8B, 0xC0AF1722, 0x51743386, 0x83F8FFBD, 0x85811021, 0xF5A103F0, 0xE7004777,
    0x82627587, 0x8077EE3D, 0x0A809808, 0x162F7D28, 0x70305063, 0x8A0790FF, 0x844B0860,
    0x05A0E00F, 0x36066063, 0x103E0330, 0x7AE00C04, 0x7E803002, 0xB30F8051, 0x82782680,
    0x7B829828, 0x065D65B7, 0x05305804, 0x6B0EC195, 0x804E0C56, 0x02205A05, 0xC70437AA,
    0x20007777, 0x8067CA82, 0x4284182A, 0x78448438, 0x0A609971, 0x440FF0A3, 0x505605C0,
    0x5B80CE5B, 0xC80C60C4, 0xC0300A70, 0x01203F03, 0xC8008755, 0x075983E7, 0x85F84584,
    0x62861860, 0x456E6CB8, 0x0FF50C4B, 0xFF6210E1, 0xC0AB0A60, 0x6C206D0A, 0xAE01003C,
    0xC85B0057, 0x8637E883, 0x7A879878, 0xA2DC87B8, 0x05D52D66, 0x7906651F, 0xA3224C80,
    0x0800790F, 0x32049083, 0xF04103D0, 0x87582000, 0x5D7C9048, 0x587C8258, 0x89789689,
    0x4E79C862, 0xF05F0640, 0x0930830F, 0xD341B55D, 0x50520480, 0x03702C05, 0x00045042,
    0x28907AF8, 0x75B82489, 0xB28B1898, 0xD8088B38, 0x0FF6970A, 0x54059041, 0x25B20FF0,
    0x0E40E352, 0x420AE0E5, 0xE04F04C0, 0x0458A801, 0x5C8AD009, 0x47CC8AF8, 0x8CF8CE8B,
    0x127B38D0, 0xC84A0975, 0x5FF0FF84, 0xB40B0601, 0x308D0B50, 0x04203F03, 0xE6755010,
    0x78AD83C7, 0x8E78D187, 0x5C8E98E8, 0x32C16477, 0x2FF2FD2C, 0xFA2675BD, 0x20900890,
    0x02E05C09, 0x8E03D039, 0x68CA8018, 0x0FF80487, 0x028EA7CB, 0x99049039, 0x6768EC6F,
    0xE50E1105, 0xB7693100, 0x55807776, 0x700EB815, 0xA07A0780, 0x04088B03, 0x21048790,
    0x191C8E48, 0x9058E689, 0x22921920, 0x73A56CA9, 0x57C5D12D, 0xFF253048, 0x00A40960,
    0x0E92B90A, 0x820C60E9, 0x502004B5, 0x83D8AB75, 0x1E93891D, 0x39018FF9, 0x93F93E92,
    0x476CD940, 0x93C78498, 0x7BF6AE3C, 0xDE0990D6, 0xE0170428, 0x9398C97A, 0x5E93C8E5,
    0x59549418, 0x2E995695, 0x8D07E369, 0xA0FF0890, 0x05B06004, 0xA62C0620, 0x50AC0AA0,
    0x81D03207, 0x06777872, 0xB8918FD0, 0x9539008C, 0x72971957, 0x32D89739, 0x06161561,
    0xC5070074, 0x597A0752, 0x06E48768, 0x4C078076, 0xB03F8550, 0x96C0038A, 0x6F8AE93A,
    0xD9748949, 0x95698E98, 0xFF7DA0D5, 0x77297270, 0x4F409508, 0x5F0DA519, 0xF06B0680,
    0x81E81C02, 0x0158303C, 0xD96E9880, 0x9A798F93, 0x249219A8, 0xB7456597, 0x0B60AA8B,
    0xDE15853A, 0x99330B90, 0x00A83901, 0x3603402C, 0x703B0000, 0x98904A04, 0x8B823951,
    0xB5E953F9, 0x37B31E5E, 0x783272D6, 0xB2D87226, 0x2DC35533, 0xEB45E319, 0xB2D96FE5,
    0x39454433, 0x879CB313, 0x93A22DB3, 0x2DD6485E, 0x305E79DE, 0x03062DA3, 0x2DE8EC33,
    0xBE41C45E, 0x63509E43, 0x2DD69359, 0xA4488373, 0xB7F79D85, 0x44C34B5E, 0xCB9F49CF,
    0x69D41F09, 0x9E75A42D, 0x2545F9E0, 0x63416499, 0x2DC9DF48, 0x945EB164, 0xB3419E45,
    0x45E2DF41, 0x8D639488, 0xF9C53323, 0xA125442D, 0xCB3323AD, 0xC5433BC9, 0x2DD9D744,
    0x05A03A0E, 0x53C00F91, 0x6912DF81, 0xDA9DB41C, 0x76D93A22, 0x9EB45FA2, 0xF8A14358,
    0x54862DE7, 0x3A6A3037, 0xEF5AE32F, 0x6A062DC9, 0x5AF9FAA2, 0xB72D6A1C, 0x83AA0F73,
    0xA2810533, 0xDA3192C9, 0xEA155A42, 0x3669EE2D, 0x1C9EC31C, 0xD2DDA4D3, 0x2DDA0BA3,
    0x4B48F48D, 0xA2000610, 0x7DC4FC0C, 0x890C10C0, 0x50128700, 0x00781D03, 0x2101F01A,
    0x903D0000, 0x9C004C04, 0x806ED939, 0x50FF4360, 0x4E40A209, 0xD30C65D0, 0xD2583F45,
    0x3414FE80, 0x45708706, 0x92DA4F44, 0x05610F68, 0xA90FF7F5, 0x70872DA5, 0x81109309,
    0x1870808A, 0xBA7E0FFA, 0x9272586D, 0x1D92D4C1, 0x09F13044, 0x40926B46, 0x220B83F6,
    0x40A42DA5, 0x0630FF4D, 0x6B52A400, 0x56552D76, 0x19507754, 0xC06A32DA, 0x797A2C64,
    0xA3D8CD41, 0x7F083071, 0x73E50FF0, 0x0C32DA3E, 0xF410B0CA, 0xE59C7F67, 0x49E2D959,
    0xD50834A0, 0x00FF9CC7, 0x1560D70D, 0x41415256, 0xD2D94434, 0x09809B08, 0x5990F0FF,
    0x10DDA935, 0x2FE5E10E, 0xC30BB300, 0x92D95C70, 0x0A20A509, 0x4FAAF8ED, 0xC0E92D6A,
    0x06461E0E, 0x66073076, 0xB2DB5FA4, 0x4C65E664, 0x300E74C8, 0x20F02D65, 0x06B4CC0F,
    0xA8185516, 0x82D940D0, 0x6180C00B, 0xFF4A805A, 0xC0E30DF0, 0x3504ED58, 0x33085074,
    0x409A08B4, 0x5602DA8D, 0x7105C562, 0x60D04150, 0x1021170D, 0x8F89F3A7, 0x7A730FF0,
    0x11290843, 0xA9B0968D, 0xA4F86994, 0x7A108E2D, 0x810700FF, 0x74B507E0, 0x2C02BE2D,
    0xBD5D4A7C, 0x12DA5C30, 0x5BC0AC0A, 0x5C07D06A, 0x55BF4034, 0x0FF97610, 0x87529882,
    0x48B62DB6, 0x6DD3F45B, 0x7E105663, 0xE06C0FF9, 0xA7507B07, 0x2C2DBA77, 0x84812558,
    0x31F5F78D, 0x085030FF, 0x24AF08A7, 0x105A4A10, 0x4F4FD749, 0x50CF3005, 0x2DA10F0D,
    0x7955D89F, 0xA8140890, 0x6316512D, 0xBD300B09, 0x22DBA7A0, 0x0704C44E, 0x0EB34082,
    0x5B372D6A, 0x0FF5322C, 0x8F2DB9AB, 0x2AF81E16, 0x2D740440, 0xDE6050F0, 0x1AF20786,
    0x35031A61, 0x7A07F06C, 0x40570FF0, 0x40B87E56, 0x630A30A2, 0x10109840, 0x00404371,
    0x6B02001B, 0x8A6F582A, 0x6B06ED98, 0x5D0C865A, 0xD0D82DA6, 0x09E1F20D, 0x2F1E50AB,
    0x869461E5, 0x0FF41060, 0xDB6170B7, 0x00E50E22, 0x0A609A2C, 0x1D8680A4, 0x00B72DA5,
    0x6094540C, 0xBA2D79E2, 0x61E32BC2, 0x67D0D852, 0x100C9490, 0xC4694675, 0x62BAFC39,
    0x6D526524, 0x92D9B6FB, 0x6340D00C, 0x48536534, 0x35F663AA, 0xAD363E57, 0x946432DB,
    0xEB5F4071, 0x5390AA36, 0xDA57C6A5, 0xF4B90DB2, 0x0A709A0F, 0x705C9BC9, 0x13F96572,
    0x3135C35C, 0x600E39D4, 0xF52540B6, 0x5E1ADD0F, 0xBC5C6664, 0x6BD1AA28, 0x40CB9F2D,
    0x7142D42B, 0xE4D34AC6, 0x1053AF63, 0x8A5DD9CB, 0xD2DA680A, 0x1DE4A50C, 0xED6030AE,
    0xDBED5433, 0x58E0FF63, 0x522DA10B, 0x309B5545, 0x6A08F1B0, 0x2538E105, 0xA1DB1D93,
    0x3F269B61, 0xE2C0D09D, 0x92D50E70, 0x0FF38EC3, 0xB34C05C1, 0x5ACC7250, 0x479A9F0E,
    0x106BB188, 0x96A66ADC, 0x50653A5F, 0x05403102, 0x763FBE14, 0x5E45E249, 0xCE6450A6,
    0xF41A70A3, 0x5657B95A, 0x862DB74A, 0x55240FFA, 0x2D76E21E, 0x740A73BE, 0x00C765A5,
    0xA7E2DA51, 0xFEBBBC5C, 0x9BD03504, 0x6180C10B, 0x19B0C2DA, 0xB3FD4794, 0x2D8C452F,
    0xBD4C0182, 0x2AEE61C8, 0x67C188B0, 0x0E2DA601, 0x3BF8BDB5, 0x0EE1059F, 0xA04050F1,
    0x10FF62D0, 0xB17C5E0D, 0xDF5620CE, 0xDA355361, 0x0A2375A1, 0xCC5B5535, 0x72DA5B80,
    0xB0941941, 0x9BB5542A, 0xD0F039AB, 0x06906605, 0xA903603E, 0xA02400D8, 0x00202D02,
    0x1B01A015, 0xE03E0000, 0x96DA70A6, 0xA69C28FE, 0xDCCC9A99, 0x3C38B4CC, 0x9E0473C5,
    0xB4570927, 0x0C83EC3E, 0x5905604C, 0x50390280, 0x00B04904, 0x01CC2016, 0x60150120,
    0x04ACC401, 0x89CC704D, 0xF7572D19, 0xCCEC870F, 0x7ACF1CF0, 0x763EC728, 0xB7B57957,
    0xD657E57C, 0x79330AA0, 0x04CA6D01, 0x1D018008, 0x100001E0, 0xCE7CE501, 0xFFBAECE9,
    0x71050048, 0x0C70BE0D, 0xB20A65D3, 0xABF75750, 0x635D1553, 0x1AD17BF8, 0x6D1CD19D,
    0xD18D1DD1, 0x20D21D1E, 0x2D1BD23D, 0xD24D25D2, 0x2AD27D1F, 0x6D2CD29D, 0xD28D2DD2,
    0x30D31D2E, 0x2D2BD33D, 0xD34D35D3, 0x3AD37D2F, 0x6D3CD39D, 0xD38D3DD3, 0x40D41D3E,
    0x2D3BD43D, 0xD44D45D4, 0x4AD47D3F, 0x6D4CD49D, 0xD48D4DD4, 0x50D51D4E, 0x2D4BD53D,
    0xD54D55D5, 0xD9D37D4F, 0xDD175E61, 0xB5707F06, 0xDF70480F, 0x80A45E15, 0x07F0AA0A,
    0xFA03A02E, 0x67AB0368, 0x00600500, 0x14000006, 0x80190180, 0x96ECEAD0, 0xCC1021F7,
    0x70880760, 0x84D84B76, 0x5D06304D, 0x9D810FF0, 0xD84D856B, 0x89D86063, 0xDD8AD88D,
    0xD8FD8CD8, 0x8BD91D87, 0x4D8ED92D, 0xD93D90D9, 0x99D95D98, 0xDD9AD97D, 0xD9FD9CD9,
    0x9BDA1D96, 0x4D9EDA2D, 0xDA3DA0DA, 0xA9DA5DA8, 0xDDAADA7D, 0xDAFDACDA, 0xABDB1DA6,
    0x4DAEDB2D, 0xDB3DB0DB, 0xB9DB5DB8, 0xDDBADB7D, 0xDBFDBCDB, 0xBBDC1DB6, 0x4DBEDC2D,
    0xDC3DC0DC, 0xC9DC5DC8, 0x204CDAFD, 0x067D8306, 0xF36048EE, 0x108B0F70, 0x05109309,
    0x100417E1, 0x703502B0, 0x00000303, 0xC1000DE0, 0x803ECC3C, 0xCC9D77CE, 0xEC08698A,
    0x40EE0EE0, 0x0E665F0F, 0xF1DEF0F1, 0x40F3C860, 0x2BFDF6DF, 0xF7DF8DF5, 0xCDF90E5D,
    0xDFDDFBDF, 0x02DFFE00, 0xEE04DFAE, 0xE01E05DF, 0x06E03E07, 0xCE08E0BE, 0xE0DE0AE0,
    0x12E0FE10, 0xEE14E09E, 0xE11E15E0, 0x16E13E17, 0xCE18E1BE, 0xE1DE1AE1, 0x22E1FE20,
    0xEE24E19E, 0xE21E25E1, 0x26E23E27, 0xCE28E2BE, 0xE2DE2AE2, 0x32E2FE30, 0xEE34E29E,
    0xE31E35E2, 0xFCE33E37, 0x40E5BC4D, 0x0EF4730F, 0xC50C10F6, 0xC0AE0C50, 0x01C044D6,
    0x34030024, 0x706F0030, 0x00007907, 0x000E926F, 0x009E09A0, 0x0220000A, 0x0003202E,
    0xB03D8F80, 0xE5E8F9E5, 0x00101E5D, 0x0409FF00, 0x411FF020, 0x260A3481, 0xE0AD482B,
    0x05108634, 0xF1189436, 0x158B44DF, 0xC8CC6A2F, 0x8C47A291, 0xEC86371F, 0x48A4B208,
    0xAA4F2493, 0xA472C94C, 0x572F944B, 0xB3096CC6, 0x349B4CA6, 0xC9CCEA6F, 0x9C4FA673,
    0xED06773F, 0x50A8B409, 0x2A8F44A3, 0xA874CA4D, 0x974FA453, 0xB35A6D46, 0xDD6DC39A,
    0xF66AA9BC, 0x68A85A2C, 0x9A79A8D3, 0xA69E8F47, 0xBDDE9A82, 0x9A09A783, 0xA49B69B4,
    0xE9B6AB51, 0x4665B298, 0x766B3192, 0xBEC160D0, 0xEAFD6CBA, 0xA9D38FF5, 0xB140FF52,
    0x7178954A, 0x218AC663, 0xB8FC8E3B, 0x93CBE532, 0x6CDE4B33, 0xCE677359, 0x0E7B4198,
    0xE934B9FD, 0x8B53A3D3, 0x755A6D5E, 0x61A8D66B, 0xBAFD8EBB, 0xB3DBED36, 0x6DDECB73,
    0xDE6F775B, 0x0EFB81B8, 0xF138BBFE, 0x0B93C3E3, 0x795C6E5F, 0xA1C8E673, 0x4A7CA73B,
    0xAB7FAC16, 0xAEC2D16C, 0x5728DFFD, 0x527DFEAC, 0xA91FEA85, 0xDFB0AD57, 0x180B6EDA,
    0xB7F07EE1, 0xB7F08C15, 0xCD8280E0, 0xF44234F8, 0x2E18896F, 0xC6914CFF, 0x580E0118,
    0x22028120, 0x07826068, 0x83E0C836, 0xE0A8460B, 0x584A0E84, 0x62108521, 0x17866168,
    0x87E1C876, 0xE1A8861B, 0x588A1E88, 0xA2208922, 0x278A6268, 0x8BE2C8B6, 0xE2A8C62B,
    0x58CA2E8C, 0xE2308D23, 0x378E6368, 0x8FE3C8F6, 0xE3A9063B, 0x590A3E90, 0x22409124,
    0x0D926469, 0x1946A156, 0x85F124FF, 0x1174FF16, 0xF0FF1B47, 0x653A5F03, 0x05C701B4,
    0x406409F5, 0xF3B4E041, 0x98FF3B8E, 0xFF3A0E93, 0x3A4EA39C, 0x2719BE6D, 0x29D26E9C,
    0xE6739D67, 0x779EE789, 0x9FE769F6, 0x2819FA7A, 0x2A127CA0, 0x2683A168, 0x87A2E88A,
    0xA3E86A36, 0x291A3A8A, 0x2A528CA4, 0x6693A569, 0x97A6E98A, 0xA7E96A76, 0x2A1A7A9A,
    0x2A929CA8, 0xA6A3A96A, 0xA7AAEA8A, 0xABEA6AB6, 0x2B1ABAAA, 0x2AD2ACAC, 0xA4FFAD6B,
    0xFF3A8EB3, 0x394E6388, 0x87B1A0FF, 0x5184FF1D, 0xC0FF1C47, 0x533A0FD7, 0x0685F170,
    0x4073FE61, 0x6416C041, 0x66D8B5ED, 0xDCB6EDAB, 0xB7EDEB76, 0xEE2B86E0, 0x6B96E4B8,
    0xA6E8B9EE, 0xECBAEEAB, 0xBBEEEBB6, 0xEF2BC6F0, 0x6BD6F4BC, 0xCEF8BDEF, 0xFFAFCE3B,
    0x1AC7016C, 0x4811BCFF, 0x63ECFF1F, 0x78543870, 0xD1080601, 0x3910298E, 0xF12C46F9,
    0x6C5714C4, 0x6718C5F1, 0x1CC6F1AC, 0xC7F1EC77, 0x0CD31AEE, 0xD118FF33, 0x50FF1605,
    0xFF278A12, 0x3D8F53D4, 0xC5813CC7, 0x5C341E16, 0xE720C410, 0x3CCEF3AC, 0xCFF3ECF7,
    0xF42D0740, 0x129B16D0, 0x00FF2BCB, 0xFF148571, 0x30CC52F0, 0x8D534CFF, 0x80F4AB35,
    0xD81212C4, 0x5CD130FC, 0xD7F5ED77, 0xF62D8760, 0xEC7B64D8, 0xFCFFD9CF, 0xFF2308F1,
    0x17864134, 0xCE1370FF, 0xA2A5E637, 0xE42D1F4A, 0x5A0383C0, 0xD97383FF, 0x0F2B8780,
    0x8DF8F33D, 0xF90239CE, 0xE5E2F813, 0xBEF8A7C2, 0x4F43FCE6, 0x6E30F43D, 0xC4EFB939,
    0x6B9A4F13, 0xBB39EE5E, 0x8FFE50F3, 0xB3E8FB3E, 0xFCF5E86D, 0x6E1CC5F3, 0x1DC643FC,
    0xCF13FC72, 0xD3DCF23C, 0x449524C8, 0x3F0EC301, 0xDFF7D03C, 0x89EE7B9F, 0x03FCA72A,
    0xFC6F1D06, 0x9B3C8F03, 0xCDFCEF07, 0xC51E20E7, 0x93FC6219, 0x3CB22D0A, 0x8CB5BDAF,
    0x3FC9726C, 0xFDBB0C67, 0x73B2DAF8, 0xC074B166, 0x48754FF1, 0x36CD5B6B, 0x8513FCDA,
    0xA3FC6119, 0xFC8722C7, 0xE5FECF63, 0x1886015C, 0x1041B031, 0xDF80D088, 0xDA716F91,
    0xFE3746F0, 0x25A78BD1, 0xF4073E0B, 0xF8263E1C, 0x2610C10F, 0x5E00E51C, 0x8D172F04,
    0x1805586B, 0x06C630C9, 0xD4B93823, 0x9C13E249, 0x9E3207F8, 0xC96D8CD1, 0xF8BA1782,
    0x90BC11C7, 0xC638F105, 0x85BBD11C, 0x84908C0E, 0x0880E40F, 0x01805047, 0x22C44847,
    0x4888F11A, 0x54D8A24C, 0x6821B2C6, 0x50FF0AA1, 0xF19E3446, 0x832C648F, 0xC1C09219,
    0x81508C10, 0x5A611E1F, 0xC3882903, 0x32C4A6FC, 0x688CF19A, 0xD11AA34C, 0xDA0A0F75,
    0xF88310E1, 0x00CA13C7, 0xE3AC7F86, 0x07F8ED1D, 0x89412423, 0x07CEEC35, 0x1141D00E,
    0x21805090, 0x4E6731AE, 0x127E9A83, 0x54B20FF1, 0xCB4290EE, 0xC0F8E6DC, 0x23064608,
    0x6480C719, 0x8B715AD7, 0x1103FC5A, 0x3FC49897, 0xD30FA1F2, 0xF86BF269, 0xCC3E47C8,
    0x74F48B03, 0xD2DA2AD0, 0xE1FE3AD5, 0x8C092824, 0xC7BBFA0F, 0x0741D83D, 0x0861FC3E,
    0x2041FBBB, 0x43002907, 0xB163929E, 0x022E05D0, 0x11D93036, 0x533F98EE, 0xC8F23B47,
    0xE1EC3F86, 0x4620BB1F, 0x3429A030, 0x3F9250CA, 0x2AC7D0F8, 0x753449B9, 0x551D83B0,
    0xFE1B038A, 0x5C598A81, 0x88CB5B47, 0x764A3406, 0xA23C7FB2, 0x9E509913, 0xACD1AEE6,
    0x06A0982F, 0x03421840, 0x00848904, 0x179C9318, 0xB4E29B3B, 0x0E840FD4, 0x150E594D,
    0x31FF429A, 0x4D7E88D1, 0x87AA704D, 0x8A35B72E, 0x86678457, 0x2EC58328, 0x6B352996,
    0x3B05E298, 0x080581F0, 0xF68200AA, 0xD4632FA5, 0x4A6745A0, 0xF4DA9AD3, 0x8412DEA6,
    0x9FD11C24, 0x1B4458FF, 0x302060D9, 0x18880184, 0x120800F4, 0x5C01E820, 0xC69C507A,
    0xAABB1D0B, 0xC85A3422, 0x353AAA8F, 0x47E55563, 0x8CE19F15, 0xAAE9C934, 0x38C7031B,
    0x668FF1C6, 0x141A434C, 0x75581B45, 0xB9CE8E56, 0x92DA752C, 0xA70A3CC7, 0xE1E03E06,
    0xC360571F, 0x8F672419, 0x10C1FDD4, 0x5182C088, 0x41D03081, 0x61A98012, 0xD6CDD314,
    0x84F3AF2A, 0x83F57E67, 0x4B18AF2A, 0xC3FE5931, 0x82F86509, 0x594A3C79, 0x1322298C,
    0x1687F895, 0x4686A0DE, 0x1AB3AE8A, 0xD03B8763, 0xB45220C4, 0x50D95B72, 0x40A03FE0,
    0x41E47FBF, 0x47F88611, 0x0E01F11B, 0x0CC18C2D, 0x0F819836, 0x8200C082, 0x7EB1000A,
    0x50D6EC51, 0x1C722EDB, 0x18C71FEE, 0xCA1B03D9, 0xF5657CDC, 0xC660C51F, 0xD921FE32,
    0x1B8C52EA, 0x594700E0, 0xEF0D11A5, 0xACB76C6E, 0x80350F4C, 0x1C5FADBA, 0xC34918A3,
    0xC32A14C2, 0xC316698E, 0x82D05809, 0x843A0008, 0x00120E96, 0x62E985C8, 0x02E04BCD,
    0x1AC0F81B, 0x462984CB, 0x70220FF1, 0x75D80AC1, 0x618976C6, 0x029C640C, 0x81F83C06,
    0x0088BC04, 0x8C580697, 0xB7F04603, 0x54B681FA, 0xF9C443E2, 0x212AE54A, 0x5D79E28A,
    0x6CE39557, 0x40E6D031, 0xF2D04008, 0x8708FF49, 0x8A662039, 0x0DA2B055, 0x0401E379,
    0x0F005A08, 0x32A9B7FF, 0x822E56BE, 0xB87F8C01, 0x58EB99A1, 0xB5DB926D, 0x8C09CDBC,
    0xE180B360, 0x9D55E194, 0x1121E0EB, 0x29651885, 0xD42F3CB7, 0x9E1203FD, 0x9B7604D8,
    0xAD6D5DB1, 0x82559A63, 0x90CD3AC3, 0x76E83A07, 0x03C8FA82, 0x04910620, 0x0820F419,
    0x91AE300F, 0x1052F091, 0xD7CC972F, 0x883185F2, 0x3C3F0FF1, 0xC8FF1162, 0x9D103C1C,
    0x43C878AE, 0x846E8D1E, 0x40E69A22, 0x15EBAF4A, 0x3FC5B0B8, 0x5109610E, 0x9E1F6C1A,
    0xD0F9977E, 0xB9F53C74, 0x8ADBD422, 0x409D3CE7, 0x620ECD06, 0x64689012, 0x0481A659,
    0x05815429, 0x11421037, 0x41D05B0A, 0x64B95A17, 0x58B6354C, 0x6975ECAC, 0x6D15AB49,
    0xCA26B9B6, 0x6BC3696C, 0xF53B8576, 0xC9E938B6, 0x0A816420, 0xA8B578FF, 0x728E41C7,
    0x4C0842B8, 0x82078278, 0x1804845D, 0x96C8C3FB, 0x4DE7B5A9, 0x911F7AEF, 0x9EF89A11,
    0x0D8C2183, 0x1FE3706F, 0x2D45A0B0, 0x1F83311A, 0x5E020108, 0x8D7FEA5C, 0x8319648D,
    0xB5641C83, 0x6C543FE0, 0x0E916F6F, 0xC618BAD5, 0x0E01B736, 0x5C3C546F, 0x7C86E8D8,
    0x610C5176, 0xE4E3F74C, 0xD835A89A, 0x6C77A66B, 0x08F665EB, 0x05FCF896, 0x23EB55B0,
    0x04909011, 0x83905C17, 0xB0DA041E, 0x62723EEF, 0x4F5311FF, 0x263D279F, 0x51775943,
    0xA2732DAE, 0x559AD693, 0xDE98794C, 0x027613AD, 0x83306CAF, 0xF5491FAE, 0x1D03956F,
    0xFD6DA26C, 0x0C2DBCBA, 0x758E6C42, 0x082203BC, 0xAC7C6771, 0xD0780E03, 0x0CFC0E83,
    0xE1226184, 0x3BCF0BB8, 0x62DE1BA1, 0xD263C077, 0xB577FF21, 0x88102EBB, 0x7E1FFE06,
    0x63E1349F, 0x6C7FCE79, 0x5F1E1BE3, 0x9E28B87C, 0x83866DF1, 0x4816F21C, 0x8486A103,
    0x12C3FD29, 0xAE4500A4, 0x630E91D2, 0x20096D58, 0x01B122F0, 0xEFD677AC, 0x9745E87D,
    0x4F67ECBC, 0x982F04FB, 0xA6CB289E, 0x899A4BC9, 0x02E10B82, 0xEB777504, 0x10FB0E19,
    0xB3DB58CB, 0xB1ED74C3, 0x91C2B835, 0x2FCB94D2, 0xB91CE9A5, 0xD44D787C, 0xA4608D0F,
    0x85B0C012, 0x14CBF15E, 0x1D4560AA, 0x21DC3064, 0x4100D20F, 0x89BC3427, 0x4B16FC6E,
    0x88760C7C, 0x5F669E1D, 0x41A47FE5, 0x5ECBD77C, 0x0AD145F5, 0x0D43FC55, 0xCA07907D,
    0x77D56973, 0x3F66922D, 0xEE5B908B, 0xD74D5F05, 0x0614E80E, 0x752C53E9, 0x80F63EA0,
    0x0F60F70F, 0x7807606E, 0xF61804C0, 0x0036C403, 0xC77AE7E9, 0x26877EC6, 0x5E40CC63,
    0x6709D4F6, 0x67B47CD1, 0x42751B0F, 0x24322429, 0xF79B7D53, 0x0A30970F, 0x056267A2,
    0x85AA2D68, 0x08006E26, 0xDB7954B8, 0x35EC0D52, 0x08743543, 0xB9092095, 0xF0DA0F74,
    0x06B06805, 0xC45C202F, 0x135703C5, 0x7EB82100, 0x5A83B7B1, 0xF80585B8, 0x38A5696C,
    0xB942F2DA, 0x044E0FF0, 0x0B60AA6E, 0xDE1583D9, 0x97A70B90, 0x00A5FC01, 0x3603402C,
    0x703B0000, 0x82204A04, 0x245C97C4, 0xD85C6C98, 0x87F87E87, 0xFD09659F, 0xA2000611,
    0x4F30D20C, 0xC10C00BD, 0x26410890, 0x5C303501, 0x1F01A007, 0xD0000210, 0x04C04903,
    0x7C7AD878, 0xD89C87F7, 0x0FB72889, 0x7A4D5556, 0xA06B5780, 0x47966F75, 0xA30A209D,
    0x081D0630, 0x04340201, 0x2001B004, 0x83568940, 0x89B82189, 0xBA8B989E, 0x470F6CA8,
    0x06E05A56, 0x6C0FF069, 0x28A307E0, 0x05D0F07D, 0x3E069066, 0xD6C20360, 0x02A02400,
    0x1500202D, 0x001B01A0, 0x89703E00, 0x427EA899, 0xB85987A7, 0x8DF8DE8B, 0x3283269E,
    0x00924F54, 0x6977570A, 0x4C0C80F3, 0x80590560, 0x04503902, 0x1600B049, 0x20018D40,
    0x01601501, 0x4D04A8D6, 0x18228D90, 0x0FF4602D, 0x028E02D5, 0x762D9039, 0x0AE0B10A,
    0x0C34B349, 0x53500C64, 0x7A70AA52, 0x4C896017, 0xD0180080, 0x00001E01, 0xF98F7011,
    0x38B78FB8, 0x10500474, 0xC70BE0D7, 0x20A668C0, 0x0AA3480B, 0x283D90B5, 0xF92992A9,
    0x92E92B0F, 0x3292F92D, 0xC9349319, 0x93093692, 0x39933937, 0xD9389359, 0x93E93A93,
    0x4293F93C, 0xB9449419, 0x94094693, 0x49943947, 0xD9489459, 0x94E94A94, 0x5294F94C,
    0xB9549519, 0x95095694, 0x59953957, 0xD9589559, 0x95E95A95, 0x6295F95C, 0xB9649619,
    0x96096695, 0x69963967, 0xD9689659, 0x96E96A96, 0xB31D993C, 0x606D93D0, 0x5B55B34D,
    0xE20E20DD, 0xA0A80A45, 0x02E07F0A, 0x3673E03A, 0x50065010, 0x00600600, 0x18014000,
    0xC91B0190, 0x1F77EB8F, 0x760CC102, 0xB4710880, 0x06336D6F, 0x7099705D, 0xB06304D3,
    0x99A99D99, 0x9E99C9A0, 0xF9A19A39, 0x9A59A299, 0xAA9A79A4, 0x99AC9A69, 0x9A89AD9A,
    0xAE9AB9B0, 0xF9B19B39, 0x9B59B29A, 0xBA9B79B4, 0x99BC9B69, 0x9B89BD9B, 0xBE9BB9C0,
    0xF9C19C39, 0x9C59C29B, 0xCA9C79C4, 0x99CC9C69, 0x9C89CD9C, 0xCE9CB9D0, 0xF9D19D39,
    0x9D59D29C, 0xDA9D79D4, 0x99DC9D69, 0x9D89DD9D, 0x6204C9C0, 0x43E19990, 0x0F76D82C,
    0x9309108B, 0x15460510, 0x02B01004, 0x03037035, 0x09F40000, 0x8D58D300, 0x8E8FA03E,
    0x68238DB9, 0x0F483D08, 0xE60E70E4, 0x3A010F10, 0x0E52FBA0, 0x09A070F3, 0x8A0B2BFA,
    0xA0FA0AA0, 0x0EA10A0C, 0x3A14A11A, 0xA0DA16A1, 0x19A12A18, 0x7A1BA15A, 0xA1FA1AA1,
    0x1EA20A1C, 0x3A24A21A, 0xA1DA26A2, 0x29A22A28, 0x7A2BA25A, 0xA2FA2AA2, 0x2EA30A2C,
    0x3A34A31A, 0xA2DA36A3, 0x39A32A38, 0x7A3BA35A, 0xA3FA3AA3, 0x3EA40A3C, 0x3A44A41A,
    0xA3DA46A4, 0x49A42A48, 0x7A4BA45A, 0x0E2A2AA4, 0xF40E50E5, 0xF0F00ED0, 0x0C10F60E,
    0xAE0C50C5, 0xC0449830, 0x03002401, 0x6F003034, 0x00790770, 0x0E926F00, 0x9E09A000,
    0x20000A00, 0x03202E02, 0x3D73C000, 0x473DA710, 0x101A73A7, 0x0409FF00, 0x411FF020,
    0x260A3481, 0xE0AD482B, 0x05108634, 0xF1189436, 0x158B44DF, 0xC8CC6A2F, 0x8C47A291,
    0xEC86371F, 0x48A4B208, 0xAA4F2493, 0xA472C94C, 0x572F944B, 0xB3096CC6, 0x349B4CA6,
    0xC9CCEA6F, 0x9C4FA673, 0xED06773F, 0x50A8B409, 0x2A8F44A3, 0xA874CA4D, 0x974FA453,
    0xB35A6D46, 0xDD6DC39A, 0xF66AA9BC, 0x68A85A2C, 0x9A79A8D3, 0xA69E8F47, 0xBDDE9A82,
    0x9A09A783, 0xA49B69B4, 0xE9B6AB51, 0x4665B298, 0x766B3192, 0xBEC160D0, 0xEAFD6CBA,
    0xA9D38FF5, 0xB140FF52, 0x7178954A, 0x218AC663, 0xB8FC8E3B, 0x93CBE532, 0x6CDE4B33,
    0xCE677359, 0x0E7B4198, 0xE934B9FD, 0x8B53A3D3, 0x755A6D5E, 0x61A8D66B, 0xBAFD8EBB,
    0xB3DBED36, 0x6DDECB73, 0xDE6F775B, 0x0EFB81B8, 0xF138BBFE, 0x0B93C3E3, 0x795C6E5F,
    0xA1C8E673, 0x4A7CA73B, 0xAB7FAC16, 0xAEC2D16C, 0x5728DFFD, 0x527DFEAC, 0xA91FEA85,
    0xDFB0AD57, 0x180B6EDA, 0xB7F07EE1, 0xB7F08C15, 0xCD8280E0, 0xF44234F8, 0x2E18896F,
    0xC6914CFF, 0x580E0118, 0x22028120, 0x07826068, 0x83E0C836, 0xE0A8460B, 0x584A0E84,
    0x62108521, 0x17866168, 0x87E1C876, 0xE1A8861B, 0x588A1E88, 0xA2208922, 0x278A6268,
    0x8BE2C8B6, 0xE2A8C62B, 0x58CA2E8C, 0xE2308D23, 0x378E6368, 0x8FE3C8F6, 0xE3A9063B,
    0x590A3E90, 0x22409124, 0x0D926469, 0x1946A156, 0x85F124FF, 0x1174FF16, 0xF0FF1B47,
    0x653A5F03, 0x05C701B4, 0x406409F5, 0xF3B4E041, 0x98FF3B8E, 0xFF3A0E93, 0x3A4EA39C,
    0x2719BE6D, 0x29D26E9C, 0xE6739D67, 0x779EE789, 0x9FE769F6, 0x2819FA7A, 0x2A127CA0,
    0x2683A168, 0x87A2E88A, 0xA3E86A36, 0x291A3A8A, 0x2A528CA4, 0x6693A569, 0x97A6E98A,
    0xA7E96A76, 0x2A1A7A9A, 0x2A929CA8, 0xA6A3A96A, 0xA7AAEA8A, 0xABEA6AB6, 0x2B1ABAAA,
    0x2AD2ACAC, 0xA4FFAD6B, 0xFF3A8EB3, 0x394E6388, 0x87B1A0FF, 0x5184FF1D, 0xC0FF1C47,
    0x533A0FD7, 0x0685F170, 0x4073FE61, 0x6416C041, 0x66D8B5ED, 0xDCB6EDAB, 0xB7EDEB76,
    0xEE2B86E0, 0x6B96E4B8, 0xA6E8B9EE, 0xECBAEEAB, 0xBBEEEBB6, 0xEF2BC6F0, 0x6BD6F4BC,
    0xCEF8BDEF, 0xFFAFCE3B, 0x1AC7016C, 0x4811BCFF, 0x63ECFF1F, 0x78543870, 0xD1080601,
    0x3910298E, 0xF12C46F9, 0x6C5714C4, 0x6718C5F1, 0x1CC6F1AC, 0xC7F1EC77, 0x0CD31AEE,
    0xD118FF33, 0x50FF1605, 0xFF278A12, 0x3D8F53D4, 0xC5813CC7, 0x5C341E16, 0xE720C410,
    0x3CCEF3AC, 0xCFF3ECF7, 0xF42D0740, 0x129B16D0, 0x00FF2BCB, 0xFF148571, 0x30CC52F0,
    0x8D534CFF, 0x80F4AB35, 0xD81212C4, 0x5CD130FC, 0xD7F5ED77, 0xF62D8760, 0xEC7B64D8,
    0xFCFFD9CF, 0xFF2308F1, 0x17864134, 0xCE1370FF, 0xA2A5E637, 0xE42D1F4A, 0x5A0383C0,
    0xD97383FF, 0xF82E0780, 0x6E1784E0, 0xCEF7E1F8, 0x5F3FF2E3, 0x3FC6E1CC, 0xC721DC64,
    0x23CCF13F, 0x4C8D3DCF, 0x30144952, 0x03C3F0EC, 0xF88DFF7D, 0x6E97A4E8, 0xA7A8E9FA,
    0xFC3EEF7E, 0xDA2C93F0, 0x8DB356D6, 0x6144FF36, 0xE8FF1846, 0xFF21C8B1, 0x397BE3D8,
    0x46218057, 0x4104360C, 0xABA10344, 0xCCF2FCAE, 0xF3FCEF37, 0xF683F765, 0x2B5CFED9,
    0xFCBA2F0B, 0x59178473, 0x3998E788, 0x8B739CE7, 0xF124460E, 0x1C440E41, 0xD0F240A1,
    0xFBFEEFB7, 0x7F2FC7F0, 0x3CB32ABE, 0xFC551684, 0xCF344CA3, 0x32CC93FC, 0x0E0490CC,
    0x0A846086, 0xD308EFA0, 0x1C41481A, 0x39F9B7E6, 0x8C1081F0, 0x77A104A0, 0xE3E07C0F,
    0xC438761F, 0x84F1FE20, 0x1FE18032, 0x3B4778EB, 0x4908C1FE, 0x9D0D6250, 0x740381F3,
    0x14240450, 0x8C137D80, 0x48670CA1, 0x4D021AC3, 0x87F8EB57, 0x3024A093, 0x1EEEF43E,
    0x1D0760F7, 0x2187F0F8, 0x2B07EE74, 0x6000A16C, 0x3A26C368, 0x8C5089F1, 0x1196BB22,
    0xEC94680D, 0x4478FF64, 0x0FF13227, 0x1E23C478, 0x17D668D7, 0x2003504C, 0x2C01A10C,
    0x98004244, 0xBA36C528, 0xB4708DF1, 0x115E28E1, 0x0CA1ED9E, 0x45C0BB16, 0x8A66A6D4,
    0x07C0EC17, 0x02A34816, 0xBE97D8D2, 0x71745030, 0x1722A44C, 0x09E548C9, 0xE88E1242,
    0xA22C7FCF, 0xA3FC6C8D, 0x0610C017, 0x03D06220, 0x8D204820, 0x1B242007, 0x52CA491B,
    0x4294F29A, 0xE63CC793, 0xF0350FF1, 0xB8FF0F01, 0xF0CE1B02, 0xA3D87A0F, 0x0433691E,
    0x49C0B022, 0x85B1A020, 0x02A51004, 0x543374C5, 0xC898F31A, 0xB2C5A64C, 0xFC6E8DE1,
    0xFBB70A03, 0x61141E47, 0x10547F88, 0xC2D0E01F, 0x8360CC18, 0x0820F819, 0x00A3480C,
    0x6217E984, 0xE8994D6E, 0xD73AA74C, 0x1C5FAE59, 0xC34918A3, 0xC32A14C2, 0xA3B4768E,
    0x82C04E18, 0xC6504416, 0x85B20C21, 0x21A72000, 0x82CEC98B, 0xEAA0F41A, 0x462984CC,
    0x70220FF1, 0x28FF0AC1, 0xF1963386, 0x0318620F, 0x0180A719, 0x81207E0F, 0x90802203,
    0x41205D02, 0x92D24A11, 0x7391349A, 0x74200420, 0x847FA4F8, 0x33101CC3, 0xD1582AC5,
    0x00F1BC86, 0x002C6902, 0xCA3A0148, 0xA7F4FA93, 0x3ADF9D40, 0x382559EC, 0x661FE1B0,
    0xAC1D03C8, 0xE47D4169, 0x48831001, 0x107A0C82, 0x73880784, 0xA42848A7, 0xD419CF48,
    0xABF57AAE, 0x15A2A3CB, 0x2F86409D, 0x931063FC, 0xC83FC480, 0x20698F42, 0x60550A41,
    0x50840DC1, 0x11AA5F84, 0x67355AAB, 0xD7CAC103, 0x7075FABE, 0x2D45B8AD, 0x2C8411FE,
    0xAE1FE150, 0xFE2D85C0, 0x94720E31, 0x604215C3, 0x303C13C2, 0x002422D7, 0x2BD521AF,
    0xD9CAFF22, 0x67D67ACE, 0x25C4D08C, 0x7FD0FA1A, 0xDC1BC360, 0x682C07F8, 0xCC468B51,
    0x804207E0, 0x094301C0, 0xF4F6BC50, 0xADCDB8B3, 0x32F5F36E, 0x586B047B, 0x3FD7FAFD,
    0x7D2671D8, 0x92120224, 0x720B82E0, 0x02AB03D0, 0x1D3CB313, 0xB90E3FE6, 0xF75AEADB,
    0x470AF0BA, 0xC1D62039, 0x1101DE0F, 0x3E2A6084, 0x3C0701D6, 0x530741E8, 0x91124205,
    0xBADACE30, 0xC5B7AB37, 0x4BE77CAE, 0xE2F56C5F, 0xC240FF69, 0x0961FE94, 0x57228052,
    0x318748E9, 0x1004B6AC, 0x01AED01C, 0xE1826F7D, 0xD6EA5D2B, 0xAE10C1F7, 0x6A1942AF,
    0x82ED9432, 0xA3D8C218, 0x5AF13235, 0x8D205C21, 0x6DB05597, 0x08D23C1B, 0x9CC4F89B,
    0xFC3FDEA5, 0xFC4488D0, 0xD8600943, 0x65F8AF42, 0xA2B0550A, 0x26F8320E, 0x00685B4E,
    0xB33566E8, 0x8A312DD3, 0xA0644C87, 0xEC1DA3AA, 0xE01D1628, 0x303AAC50, 0xB8F81ED9,
    0x60DC3D87, 0x41303C07, 0xEBB03FA3, 0xE0BC7C00, 0xAB9906BC, 0x98B30E45, 0x4A8D5A0F,
    0xFF7B8F6A, 0x24254438, 0x687BC3F1, 0x58680BE3, 0x09BC0BC3, 0x2D5C1E4E, 0xB2065C00,
    0xE7CCC783, 0x7AF32B3E, 0xFDC7F8B9, 0x5B0AAB0E, 0x581FE2C8, 0x082E46F1, 0x2A7E819C,
    0xB0340580, 0x70760001, 0x79742504, 0xF2FCE5C4, 0xE9CCFD5E, 0x1152934E, 0x7F44B099,
    0x652000C2, 0x988AC348, 0x6C22460C, 0x7C1A812A, 0xF81A00F3, 0xD0000840, 0xE1302483,
    0xAF8E0AD2, 0xEBCD77A7, 0x75CEB219, 0x1E83F86C, 0x3306B962, 0x9DA89080, 0xC628C510,
    0x14086580, 0xD804086F, 0x6D660800, 0x59735BB5, 0xB6CEBDD7, 0x0E07E5B6, 0x1FE38C72,
    0x1A43705A, 0x3F06C1FE, 0xE81F5B0C, 0xCC174786, 0xD81F0690, 0x200DAE80, 0x205A0A81,
    0x60680A80, 0x81F00003, 0xE3FD71AD, 0xCF7A66E8, 0xC0B80EDB, 0x923227E1, 0x928B011D,
    0xFE274500, 0x22550F21, 0xC8560983, 0x50720A02, 0xC02C2484, 0x800EF702, 0x81602A04,
    0x09A12B7C, 0xB472EEFC, 0x6A8FF812, 0x28E4FC0D, 0x6229DE67, 0x2DA48AE1, 0x80C692ED,
    0xA1ACD49A, 0xA05F020A, 0x00404CD6, 0x01E03A06, 0xE3804400, 0xB5F9071E, 0x82804BA5,
    0xC717C35C, 0x4C3FC620, 0xA9A40B21, 0x7E8716A2, 0xFEA1D47A, 0x9D57A90F, 0x3ACF56EA,
    0xF53EB7D6, 0xEBDD5FAE, 0xD73B075A, 0xB0F64EBF, 0x66EC7D97, 0xD7DA3B4F, 0x9FB6F62E,
    0x876AEDDD, 0x4EDFDB3B, 0x7DD7B8F7, 0x3BCF76EE, 0xF72EF7DE, 0xEFDDDFBE, 0xDF3C077A,
    0xC0F84EFF, 0x86F07E17, 0x17E23C4F, 0x1FC6F82F, 0x878AF1DE, 0x4F1FE33C, 0x7E57C8F9,
    0x0599D9EC, 0x87F0DBD9, 0x9A333C3D, 0x380710DD, 0x550A83CC, 0x3A05C1FC, 0xCC0DAE68,
    0x18028068, 0xA00000C0, 0x10320600, 0xF90390F4, 0x8661023E, 0x41B1101D, 0x09B44287,
    0xDB42E863, 0xE03700C6, 0xC3F89F17, 0x93F2BE3F, 0x37E37CCF, 0x8FCEF91F, 0xBFA7F43E,
    0x9F57E97C, 0xBEBFCFFA, 0x7DCFB3F6, 0xFB1F77EB, 0xF83F0FDE, 0xF17DBFC7, 0xDFFC9F97,
    0xD3FABF3F, 0xB7F37ECF, 0x8FEEFD1F, 0xBFE7FC3F, 0x9FD7F97E, 0xBFBFEFFE, 0x7FCFF3FE,
    0xFF1FF7FB, 0x008017FE, 0x27FB8038, 0x80480580, 0x4C7EB7FF, 0x77C20620, 0x3955C006,
    0x8B0F70F3, 0x10930910, 0x04149C05, 0x3502B010, 0x00030370, 0x00081E00, 0x3E6F86F6,
    0xE7B671F0, 0x08465E6F, 0xF30EF266, 0x70E80E50, 0x0E60F10E, 0xF10E80E8, 0x282D0E40,
    0x0E78340F, 0x37835836, 0xB8388398, 0x83A83E83, 0x4183D840, 0xF84483C8, 0x84784283,
    0x46843845, 0xB8488498, 0x84A84E84, 0x5184D850, 0xF85484C8, 0x85785284, 0x56853855,
    0xB8588598, 0x85A85E85, 0x6185D860, 0xF86485C8, 0x86786285, 0x66863865, 0xB8688698,
    0x86A86E86, 0x7186D870, 0xF87486C8, 0x87787286, 0x76873875, 0x68628798, 0x0EE0F30E,
    0xF50F00F1, 0x90C80C50, 0x0410B10C, 0x31028592, 0xF04003C0, 0x07F07700, 0xBB001080,
    0x00000EA2, 0x0A50A40A, 0x3A02F000, 0x982503E0, 0x00010155, 0x00000000

  EW_BITMAP_CLUT_EMPTY( CutterPaperStripe, Default )

  EW_BITMAPS_TABLE( CutterPaperStripe )
    EW_BITMAP( CutterPaperStripe, Default )
EW_END_OF_BITMAP_RES( CutterPaperStripe )

/* Table with links to derived variants of the bitmap resource : 'Cutter::PaperStripe' */
EW_RES_WITHOUT_VARIANTS( CutterPaperStripe )

/* Initializer for the class 'Cutter::FormatWindow' */
void CutterFormatWindow__Init( CutterFormatWindow _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CutterWindow__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->Icon, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextW, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextH, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextWidth, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextHeight, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterFormatWindow );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0008 );
  CoreRectView__OnSetBounds( &_this->Super1.CaptionRect, _Const0009 );
  ViewsText_OnSetString( &_this->Super1.CaptionText, EwLoadString( &_Const0012 ));
  CoreRectView__OnSetBounds( &_this->Icon, _Const0013 );
  ViewsImage_OnSetAutoSize( &_this->Icon, 1 );
  ViewsImage_OnSetAlignment( &_this->Icon, ViewsImageAlignmentAlignHorzCenter | 
  ViewsImageAlignmentAlignVertCenter );
  CoreView_OnSetLayout((CoreView)&_this->TextW, CoreLayoutAlignToRight );
  CoreRectView__OnSetBounds( &_this->TextW, _Const0014 );
  ViewsText_OnSetWrapText( &_this->TextW, 0 );
  ViewsText_OnSetAlignment( &_this->TextW, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextW, EwLoadString( &_Const0015 ));
  ViewsText_OnSetColor( &_this->TextW, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->TextH, CoreLayoutAlignToRight );
  CoreRectView__OnSetBounds( &_this->TextH, _Const0016 );
  ViewsText_OnSetWrapText( &_this->TextH, 0 );
  ViewsText_OnSetAlignment( &_this->TextH, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextH, EwLoadString( &_Const0017 ));
  ViewsText_OnSetColor( &_this->TextH, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->TextWidth, CoreLayoutAlignToRight );
  CoreRectView__OnSetBounds( &_this->TextWidth, _Const0018 );
  ViewsText_OnSetWrapText( &_this->TextWidth, 0 );
  ViewsText_OnSetAlignment( &_this->TextWidth, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextWidth, EwLoadString( &_Const0019 ));
  ViewsText_OnSetColor( &_this->TextWidth, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->TextHeight, CoreLayoutAlignToRight );
  CoreRectView__OnSetBounds( &_this->TextHeight, _Const001A );
  ViewsText_OnSetWrapText( &_this->TextHeight, 0 );
  ViewsText_OnSetAlignment( &_this->TextHeight, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextHeight, EwLoadString( &_Const0019 ));
  ViewsText_OnSetColor( &_this->TextHeight, CutterTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Icon ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextW ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextWidth ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextHeight ), 0 );
  ViewsImage_OnSetBitmap( &_this->Icon, EwLoadResource( &CutterPaperFormat, ResourcesBitmap ));
  ViewsText_OnSetFont( &_this->TextW, EwLoadResource( &CutterDescrFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextH, EwLoadResource( &CutterDescrFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextWidth, EwLoadResource( &CutterSmallDigitFont, 
  ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextHeight, EwLoadResource( &CutterSmallDigitFont, 
  ResourcesFont ));
}

/* Re-Initializer for the class 'Cutter::FormatWindow' */
void CutterFormatWindow__ReInit( CutterFormatWindow _this )
{
  /* At first re-initialize the super class ... */
  CutterWindow__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->Icon );
  ViewsText__ReInit( &_this->TextW );
  ViewsText__ReInit( &_this->TextH );
  ViewsText__ReInit( &_this->TextWidth );
  ViewsText__ReInit( &_this->TextHeight );
}

/* Finalizer method for the class 'Cutter::FormatWindow' */
void CutterFormatWindow__Done( CutterFormatWindow _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterFormatWindow );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->Icon );
  ViewsText__Done( &_this->TextW );
  ViewsText__Done( &_this->TextH );
  ViewsText__Done( &_this->TextWidth );
  ViewsText__Done( &_this->TextHeight );

  /* Don't forget to deinitialize the super class ... */
  CutterWindow__Done( &_this->_Super );
}

/* Method for doing a fade animation within the derived window classes. The parameter 
   aValue runs from 0 to 255 during the animation. */
void CutterFormatWindow_FadeAnimation( CutterFormatWindow _this, XEnum aMode, XInt32 
  aValue )
{
  if ( aMode == CutterOperationModeProgramming )
  {
    ViewsImage_OnSetOpacity( &_this->Icon, aValue );
  }
  else
    if ( aMode == CutterOperationModeRunning )
    {
      ViewsImage_OnSetOpacity( &_this->Icon, 255 - aValue );
    }

  if ( aMode == CutterOperationModeExclusive )
  {
    if ( _this->dialog == 0 )
    {
      _this->dialog = EwNewObject( CutterFormatDialog, 0 );
      CoreRectView__OnSetBounds( _this->dialog, EwSetRectOrigin( _this->dialog->Super2.Bounds, 
      _Const000E ));
      CoreGroup__Add( _this, ((CoreView)_this->dialog ), 0 );
      _this->dialog->OnCancel = EwNewSlot( _this, CutterWindow_onCancel );
      CutterFormatDialog_OnSetPaperHeight( _this->dialog, EwNewRef( EwGetAutoObject( 
      &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetPaperHeight, CutterDeviceClass_OnSetPaperHeight ));
      CutterFormatDialog_OnSetPaperWidth( _this->dialog, EwNewRef( EwGetAutoObject( 
      &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetPaperWidth, CutterDeviceClass_OnSetPaperWidth ));
    }

    CoreGroup__OnSetOpacity( _this->dialog, aValue );
    ViewsText_OnSetOpacity( &_this->TextWidth, 255 - aValue );
    ViewsText_OnSetOpacity( &_this->TextHeight, 255 - aValue );
    ViewsText_OnSetOpacity( &_this->TextW, 255 - aValue );
    ViewsText_OnSetOpacity( &_this->TextH, 255 - aValue );
    ViewsImage_OnSetOpacity( &_this->Icon, 255 - aValue );
  }
  else
  {
    if ( _this->dialog != 0 )
    {
      if ( aValue > _this->dialog->Super1.Opacity )
        CoreGroup__OnSetOpacity( _this->dialog, aValue );

      if ( aValue < 10 )
      {
        CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->dialog ));
        _this->dialog = 0;
      }
    }

    if ( aValue > _this->TextWidth.Opacity )
      ViewsText_OnSetOpacity( &_this->TextWidth, aValue );

    if ( aValue > _this->TextHeight.Opacity )
      ViewsText_OnSetOpacity( &_this->TextHeight, aValue );

    if ( aValue > _this->TextW.Opacity )
      ViewsText_OnSetOpacity( &_this->TextW, aValue );

    if ( aValue > _this->TextH.Opacity )
      ViewsText_OnSetOpacity( &_this->TextH, aValue );
  }
}

/* 'C' function for method : 'Cutter::FormatWindow.OnSetPaperWidth()' */
void CutterFormatWindow_OnSetPaperWidth( CutterFormatWindow _this, XRef value )
{
  if ( !EwCompRef( value, _this->PaperWidth ))
    return;

  if ( _this->PaperWidth.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterFormatWindow_onUpdate ), _this->PaperWidth, 
      0 );

  _this->PaperWidth = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterFormatWindow_onUpdate ), value, 
      0 );

  EwPostSignal( EwNewSlot( _this, CutterFormatWindow_onUpdate ), ((XObject)_this ));
}

/* 'C' function for method : 'Cutter::FormatWindow.OnSetPaperHeight()' */
void CutterFormatWindow_OnSetPaperHeight( CutterFormatWindow _this, XRef value )
{
  if ( !EwCompRef( value, _this->PaperHeight ))
    return;

  if ( _this->PaperHeight.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterFormatWindow_onUpdate ), _this->PaperHeight, 
      0 );

  _this->PaperHeight = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterFormatWindow_onUpdate ), value, 
      0 );

  EwPostSignal( EwNewSlot( _this, CutterFormatWindow_onUpdate ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void CutterFormatWindow_onUpdate( CutterFormatWindow _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperWidth.Object != 0 )
    ViewsText_OnSetString( &_this->TextWidth, EwConcatString( EwConcatString( EwNewStringInt( 
    EwOnGetInt32( _this->PaperWidth ) / 10, 0, 10 ), EwLoadString( &_Const001B )), 
    EwNewStringInt( EwOnGetInt32( _this->PaperWidth ) % 10, 0, 10 )));

  if ( _this->PaperHeight.Object != 0 )
    ViewsText_OnSetString( &_this->TextHeight, EwConcatString( EwConcatString( EwNewStringInt( 
    EwOnGetInt32( _this->PaperHeight ) / 10, 0, 10 ), EwLoadString( &_Const001B )), 
    EwNewStringInt( EwOnGetInt32( _this->PaperHeight ) % 10, 0, 10 )));
}

/* Variants derived from the class : 'Cutter::FormatWindow' */
EW_DEFINE_CLASS_VARIANTS( CutterFormatWindow )
EW_END_OF_CLASS_VARIANTS( CutterFormatWindow )

/* Virtual Method Table (VMT) for the class : 'Cutter::FormatWindow' */
EW_DEFINE_CLASS( CutterFormatWindow, CutterWindow, dialog, PaperWidth, PaperWidth, 
                 Icon, _None, "Cutter::FormatWindow" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CutterWindow_UpdateLayout,
  CutterWindow_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
  CutterWindow_ChangeMode,
  CutterFormatWindow_FadeAnimation,
EW_END_OF_CLASS( CutterFormatWindow )

/* This is a bitmap resource. */
EW_DEFINE_BITMAP_RES( CutterPaperFormat )
  EW_BITMAP_FRAMES( CutterPaperFormat, Default, EW_DRIVER_VARIANT_RGBA8888, 92, 49, 0 )
    EW_BITMAP_FRAME( 31, 13, 39, 19, 0x00000000, 0x00000000 )

  EW_BITMAP_PIXEL( CutterPaperFormat, Default )                 /* ratio 28.57 % */
    0x0409FF00, 0xC11FF020, 0x1A0F0581, 0xA110C84C, 0x150F85C3, 0xB1086C4A, 0x1488C4E2,
    0xC8CC5A2F, 0x8DC7A311, 0xEC8A351F, 0xC8E4B208, 0x14410590, 0xA4F240B4, 0x513096C9,
    0xD3397CCA, 0x3498CDA5, 0x89ACDE73, 0x9ECEA773, 0xFD0A7940, 0x8FA5D439, 0x6A0A9B82,
    0x60ACA82B, 0x8941959E, 0x3549F54A, 0x587F48AA, 0x39ED16AB, 0x59002BAF, 0x0586BF60,
    0x596C4B2B, 0xA2D366B2, 0xEDB63B5D, 0xE16AB759, 0x374B7DB2, 0x76B9DD6E, 0x2BDDE2F3,
    0xBFAAEE17, 0x06094A00, 0xC2B0AD30, 0x155D051D, 0x1FF057CC, 0xFEDC6F36, 0xAE97539D,
    0x4E71E371, 0xC5C0660B, 0xCFA9C019, 0xD6895A00, 0x696EF7DB, 0xA353A6D2, 0x0EC70D5E,
    0xAC4B001E, 0x65100164, 0xB6DA178B, 0x7DAEDF71, 0x6A4F6ADB, 0x1F0801C0, 0xF1B8BC4E,
    0x2B93C8E3, 0xB9BCCE5F, 0xA3D0E7F3, 0xC00E9833, 0xD61FF58F, 0xD82AC7B2, 0x6DB571AF,
    0x7A51014B, 0x97882FF4, 0xA49EFF2B, 0x934FF319, 0xD6FF532A, 0xB9676BB9, 0xF85E37E0,
    0x8FD944F7, 0xFFFFBA21, 0x81F7FA00, 0x80002809, 0x00481143, 0x43100350, 0x83A0C828,
    0xA0D82E0D, 0x635D356F, 0x66188061, 0x1C86E1A8, 0x87C13712, 0x62221400, 0xD894F410,
    0x4D027D4A, 0xFF364DA3, 0x24094204, 0x1E8175E5, 0xF12C530D, 0x28520CCF, 0x510C8FF1,
    0x132348E8, 0x46B140FF, 0xC274FF19, 0xA8FF2A4A, 0x283B0ED3, 0x700E13FE, 0xC6C1F8D4,
    0x60041794, 0x72588765, 0x310BC288, 0x1243D000, 0x03A0004C, 0x39926282, 0x6A678266,
    0xD7859535, 0x9BE6E964, 0xC2073E70, 0x61E8009C, 0xA8EB0EE7, 0x02628EA3, 0x35905631,
    0xE2D36CDC, 0x1110FF78, 0xE6371686, 0x368D0FF8, 0x14C5B0FA, 0xC881CCFF, 0xC194FF20,
    0xDCFF1D87, 0x8B134550, 0x9029D8D6, 0x49E91242, 0x93FCAA2B, 0xA24C3B0E, 0xDC374E38,
    0x16479370, 0xC04066B0, 0x6AD671AC, 0x7E5D96DC, 0xBC9A6619, 0x8526499A, 0xEB69B13A,
    0xE806C4B0, 0x3800B1C1, 0xE60FACA2, 0x409DBB34, 0xDDF415DD, 0x67922E52, 0xE29E8CDE,
    0x5C0FE368, 0x1D4FF154, 0xCFF2148A, 0x6340D332, 0xDCF83D99, 0xC02D0FF3, 0x17CFF2F4,
    0x6991C878, 0x6A744E15, 0x00530D63, 0xFF90C519, 0x3FDEF26C, 0x4EE3BCEC, 0x6334DF93,
    0x24601EF0, 0x0C04018C, 0xAE21DB17, 0x2659825E, 0x9C52BE9A, 0x0936B020, 0x1CC6F0EB,
    0x0031E04B, 0x440C8487, 0x889CF357, 0xFE2B3522, 0xD624C803, 0x8DC56176, 0x4FF8F6D9,
    0x4A405A0F, 0xFC842247, 0xE6348CA3, 0x4173E936, 0xCF83E4F7, 0xB2F8B23F, 0xC4603FCB,
    0x3CBD6F3B, 0x906F98FA, 0xCFF1906A, 0x992A48A8, 0x14E07D6A, 0x6D1F8D36, 0x00C17D74,
    0x1D0C7360, 0x6C4EBAC4, 0x6F17AF76, 0x6285E155, 0xB1E1DDB7, 0x2779D41E, 0x63ACEB3B,
    0x0A7E3FF2, 0x82374D64, 0x18285B56, 0xB4E8E288, 0x21643B8D, 0xFF38B7C5, 0xCF344C93,
    0x560FD3FC, 0xF92E473F, 0x03FEE93D, 0xBCB92F0B, 0xF31C476B, 0xA2E8CA72, 0x86CBE854,
    0x9920ADA8, 0x8E4D92CE, 0x5AABAAD3, 0xD8019EB4, 0x1C5DB803, 0x7DA312D9, 0x076AAFF6,
    0xBCECB6CB, 0xEFC1F733, 0xBC223400, 0x0F14E50F, 0x34FC414B, 0x84B52806, 0xBD9E710A,
    0x05CB6E89, 0x922ADF15, 0x44D232F3, 0xDCF6F923, 0x3D8F4F73, 0x0BFBACF5, 0xC1E2EF2F,
    0x7EF5A6B9, 0x6C14B9FE, 0xBFEFD198, 0x228C0F01, 0xBFD4CE38, 0x63F81787, 0x1CFEC0F0,
    0xC56BBED3, 0x54DB4BB7, 0xDEB1A27A, 0x1263D019, 0x16220000, 0x38790408, 0x41F54F90,
    0xC6543619, 0x8F3CF2B2, 0xD1B05206, 0x3346640C, 0x676E259B, 0x089EEB3D, 0x7FC24847,
    0x45686D04, 0xE4D25A3B, 0xC91DBE85, 0x935047EC, 0xD492D51A, 0x4FCCFA0E, 0x43EC381A,
    0x8170DAD7, 0x4E201159, 0x10A00BFE, 0x0292D631, 0x1D58B01A, 0xF2002248, 0x2C1DC4C0,
    0xCB771D89, 0x25003FDB, 0x02544128, 0x43004446, 0xB8351216, 0x70C0F07F, 0x3219CB88,
    0x7F8CE1A2, 0x251FC560, 0xD51AA34C, 0x5DA1B948, 0xB85E732E, 0xB06CE703, 0x20D5E829,
    0x17D8FF74, 0x3A86AAE9, 0x0F2EAD55, 0xEBC2D3AD, 0x20A1A6C2, 0x4476CEDA, 0x8F11890C,
    0x7DDFA588, 0x888B0000, 0x41C80FA4, 0x2409E3C9, 0xD40A7F79, 0x3CE460A0, 0x8330696C,
    0xB741F0FF, 0xEBB371FE, 0x6B7B434D, 0x572AA54C, 0x85EF9F59, 0x43E3168F, 0x45E41DDA,
    0x0A79F4C7, 0x9F623683, 0x0A75FCFB, 0x1DE3BD80, 0x33C7024D, 0x2E87A986, 0xFC7F5318,
    0x22971B1B, 0x0D8B3FF6, 0xDB197731, 0x1401231D, 0x40026A81, 0x41FCD811, 0x8AF9B63C,
    0x82904926, 0x2820BAD6, 0x0EA8A065, 0x3C83F292, 0xD73AA563, 0xB5144EC9, 0x2B466891,
    0xC0AD0B1F, 0x4D3E17B4, 0x523C3248, 0x3043E692, 0x0F93F868, 0x06036253, 0xE4D0400A,
    0x320600A4, 0x1110A660, 0x4A22CD09, 0xE1CA9A73, 0xC8974480, 0xE3B81E44, 0x8C58ACA2,
    0x456934F2, 0x8B916A2C, 0xE1DC2BD0, 0x684625C0, 0xED8CF194, 0x574AA94C, 0xB87C340A,
    0x88FF7311, 0x41FE1C23, 0xEA79CF47, 0xD7E2FA8E, 0x35774F0C, 0x55AEA8CB, 0x5082D879,
    0x0D9C33FB, 0x0004A08C, 0x921181C8, 0x2C1D8000, 0x1BB8A175, 0x04DADAE0, 0x09CCE251,
    0x66C4D704, 0x8E49280F, 0x1F640A6E, 0xDF06B98D, 0x4F416936, 0x8E940A2E, 0x261D1EA2,
    0xB274B3A5, 0x8D70ADF4, 0xF7C12B6B, 0x72C4598F, 0xDE839D85, 0xBEF7DAFA, 0x982FC65E,
    0x9D8673F5, 0x0B882100, 0x4D0940FC, 0x700B0000, 0x12A483B0, 0x94CF08E1, 0xDAA8EEAA,
    0x00466ABB, 0x01FC9192, 0xD5EB363C, 0x82636A08, 0x59A70B2B, 0x9AB33705, 0xA894CF55,
    0x576AAB94, 0xBCCB5ACB, 0x53488533, 0xE9E80E1A, 0x2A81A742, 0x5D2B536A, 0xA289F87C,
    0x55A3686F, 0x2384387D, 0x1C01C120, 0x2EB2010C, 0x0C0F01C8, 0x513558F4, 0xDAC39165,
    0x9A297480, 0xF2FC1E87, 0x8815191D, 0x8DC3BC3E, 0x12C20CA5, 0x16DBF891, 0x9139FB49,
    0xC767339E, 0xCC334680, 0xBAD6B908, 0x15F0BDF7, 0x18DF3BC8, 0xB4CC3B53, 0xD7BD67A6,
    0x53A0A4D3, 0xA7B0D152, 0x3887718D, 0x450E41C6, 0x8A110204, 0x129A4128, 0x76E81269,
    0x56100428, 0x3AB702E0, 0x1674718E, 0x6B1105AC, 0x3CE6FC64, 0xAD15E82C, 0xA49EA05B,
    0xF8C20528, 0x28C4F89A, 0xEA3D1146, 0xC459BE11, 0xE29AAF13, 0xC0A80D94, 0x49017C29,
    0x44C8FF97, 0xDFF14A2A, 0x1683A926, 0xA205C0B7, 0x87C8AA1A, 0x919AD352, 0xAE0F845C,
    0x6E08C96B, 0x11E24583, 0x6B48F42D, 0x8B89704E, 0xB14DA95C, 0xE5CCB796, 0x1F43E4C6,
    0x3145DDDB, 0x2E0FF184, 0x5E0DE1D4, 0xAE0EA304, 0xD87F8500, 0x28491FF8, 0x31FE1783,
    0xE2805189, 0x0780ED1F, 0x0C520B30, 0xC1C48B06, 0xA080D1CA, 0x42878001, 0x9B1E20FB,
    0xB641455D, 0xFEEEAD2B, 0xF892BBE3, 0x4A3E10C7, 0xB7994F5E, 0xEF48FF94, 0xE5DBDB49,
    0xC53A7B4E, 0x7747E8FC, 0x640CB184, 0xFF8E4BBC, 0xFE145C28, 0xAC2C03A1, 0x2048861A,
    0xF1FE1703, 0xE2745088, 0x4790EF1F, 0x67D31A3C, 0xC8D83722, 0x087081C3, 0x1E321640,
    0x101C7854, 0xB040B464, 0x9141AAC1, 0xF05F0847, 0xD53CFAB0, 0x8A471083, 0xB4FE25AD,
    0x5F1DB7B6, 0x172BA974, 0x3FC608C3, 0x7FA970C8, 0x4B5E80E0, 0xD4D3F373, 0x984B1EC3,
    0xA2A61442, 0xC1D0935B, 0x42C8588A, 0x08CE7E0C, 0x800DF636, 0x001BF806, 0x0FB8088C,
    0x268EEA2E, 0x5065065B, 0x3F389E6E, 0xD27323A9, 0x3AE37AC1, 0x4E27B725, 0x63EAB971,
    0x45B5601F, 0x8FF17E30, 0x0EA1E831, 0x2E2DA299, 0x3384D7A1, 0x981FE184, 0x24290540,
    0xB126A7A3, 0x50C818C3, 0xC0EA24C6, 0xC0C83F87, 0x404846B8, 0x8FC223BE, 0x9B7A1F9E,
    0x772AFE8A, 0x495E0BBD, 0xE4C3C183, 0x0F17E505, 0x1D4A3377, 0x45EB19E3, 0x4E2B1A23,
    0xF2ABAAF5, 0x370560E2, 0x87C52A30, 0xCD9AB03B, 0x129CFD20, 0x7F85F0CA, 0x77148250,
    0x7C650736, 0x18628C1C, 0xA06C13E3, 0x708E1DC3, 0xC1142103, 0x81D82F03, 0x8390340F,
    0xE76052C7, 0xDDC82C5E, 0xC818ED94, 0xD2DA1590, 0x692825BE, 0x08446226, 0x9C21ADA2,
    0x4F27D5BA, 0xFB79EE79, 0x3716E01F, 0x6E7531B9, 0x5B49025B, 0x1A0268FF, 0x51892E4E,
    0x6A1FE27C, 0xE68A0CAF, 0x38668CA1, 0x906A15A3, 0x209A1D03, 0xA0C41421, 0x41404007,
    0x60A08FDF, 0x4CE07104, 0x51D60012, 0xC6F06204, 0xCD655C20, 0xCB68C2FB, 0x46925A69,
    0x8F95CB3C, 0x8451F4BE, 0xDC687FF5, 0x03DF1D17, 0x72206FC8, 0x0979BE50, 0x3FC30066,
    0x508A512A, 0xE81C63FC, 0xEE934738, 0x24648C51, 0xB86E1523, 0x40AA1E43, 0xE0A50603,
    0x39C7B9F6, 0x8224CE0A, 0x4CE06933, 0x0C267013, 0xF7C31380, 0x4972A50F, 0x8F1E01DE,
    0x10437456, 0x42F2376D, 0x4D46074A, 0x00C60FF7, 0x8177D375, 0x1855F818, 0x20C47987,
    0x75E2966E, 0x1B760039, 0x576345D4, 0x67776776, 0xE80E90E5, 0x80F624D0, 0x0CC0CC0C,
    0x7806F0AF, 0x605607A0, 0x04404103, 0x3C03002B, 0xF7BA03F0, 0x26702958, 0x1B267023,
    0x70152670, 0x26700E26, 0x05267009, 0xA0891240, 0x18F10551, 0x9245761B, 0x263380E6,
    0x5A563463, 0x87072290, 0x262578F0, 0x85F81975, 0xBA4EF756, 0x875C81D0, 0x04F7DC29,
    0xFF3D129F, 0xF7A304C0, 0x0A20920F, 0xEB0E67A7, 0xC1F57AA0, 0x0AE7AE7A, 0x7807506D,
    0x57750540, 0x83A02A04, 0xBB02903F, 0xD2087FB7, 0x8447BE7F, 0x6700F267, 0x784A00A2,
    0x26700326, 0x1E114001, 0xA0F61050, 0x0DE0E00D, 0x8D0790FF, 0xC0FF0880, 0x5382DE2D,
    0xE6387669, 0xF85B0735, 0x8607C70F, 0xD57558A8, 0xB7D97D77, 0x5377227D, 0xE27E034F,
    0x87E67E47, 0x7EB0E67E, 0xCA0C90C6, 0x606F0A90, 0x2127F307, 0x2783B507, 0x00277380,
    0x26784284, 0x48846014, 0xE0062678, 0x50C89088, 0x05019115, 0x30DF0F41, 0x0FF0E20E,
    0xE222F082, 0xA2406EA5, 0x4606A96A, 0x7F08406F, 0x50CE0FF0, 0x8A90D30D, 0xD565D8E8,
    0xD2957997, 0x76082007, 0x4A3D034D, 0x88260660, 0x8290FF76, 0xC84EE82B, 0x28300CB0,
    0x83683483, 0x27880838, 0x583F7BB0, 0x2088C788, 0x8A208845, 0xC00A2088, 0x04B04803,
    0x4954E006, 0x120888F0, 0x40048700, 0x0507E124, 0x50E053D1, 0x08A1980E, 0xFF09809C,
    0x30630470, 0x8A08A06B, 0x8306D5E6, 0x40FF07D0, 0x0CB0CD0C, 0xD16A10FF, 0xB9348E92,
    0x0C48624E, 0xE679B664, 0x08680566, 0x29E03343, 0xFF72604C, 0xE0A20910, 0x76B7A809,
    0xCE7AD7AB, 0x70758780, 0x87D05507, 0x8283B87F, 0x888458F8, 0x04888604, 0x899087BF,
    0x703B88B8, 0x00604A04, 0x0458803E, 0xA03702F0, 0x20B00203, 0x4F54D042, 0xD4CE9175,
    0x2250F312, 0x170ED028, 0x21022305, 0x0DC18D0F, 0xFF0E10E2, 0xA09E08D0, 0x0480FF09,
    0xA0761925, 0xE6AB66B4, 0x91A08406, 0xA60CA0C0, 0xD9359326, 0x66033F98, 0x648AC5DB,
    0x693B7DA0, 0x39F2364B, 0xE206704B, 0x77E50947, 0x8B70E77E, 0xBB8B94EE, 0x60797F18,
    0x0318C005, 0xFC7FA8C2, 0x90489077, 0x84890A8C, 0xB191090E, 0xB0320049, 0x01600183,
    0x0101B01A, 0xD0261C40, 0x50A02F02, 0x2D36696C, 0xC07A0721, 0x90000007, 0x97091000,
    0xD02B0990, 0x0DF0DF0D, 0x7610509C, 0x90DD1059, 0x9200FF97, 0x63046922, 0x55F905B0,
    0x6AE6A95B, 0x8506F0FF, 0xE0C58E30, 0x98E0CC0C, 0x648EB8E8, 0xF8EE0668, 0x8688210F,
    0xF47628F1, 0x98F66748, 0x8FA7EA82, 0x700AF8FC, 0x78FF0770, 0x8C283983, 0x879AB904,
    0xC9AF95A8, 0x9609B190, 0x32003588, 0x900277A0, 0x55801F01, 0xC9A11002, 0x04004861,
    0x0CE1228D, 0x000D20D0, 0x103E0320, 0x02B00104, 0x41000965, 0x404F04C0, 0x08A60700,
    0xD80D603B, 0x29460D90, 0x10267B10, 0xE40E50E1, 0xF9219D70, 0x0620460F, 0x237236B3,
    0xE0FF74B7, 0x91A08306, 0x8E92F0C5, 0xB9E86A23, 0x0FF4276D, 0xE36630BA, 0xD5B36E56,
    0x39F34F93, 0x6F7E2941, 0x30E79468, 0x87624D87, 0xAE06E94B, 0xF7380551, 0x8838819F,
    0x48A038C6, 0x70489090, 0x95D90C84, 0x4E58795F, 0x7A0C9B40, 0x01C01B01, 0xD143E000,
    0x64CEA141, 0x375119A1, 0x013130EC, 0x60049000, 0x00777603, 0x0703E779, 0xB00B2670,
    0x08508307, 0x160D503F, 0x30FFA2D2, 0x9D518D0F, 0x7D08C97A, 0xD06397F9, 0x29E76105,
    0xE166D6AD, 0xA0C191A8, 0x0FD93198, 0xABA45498, 0x199165F5, 0x9937D80C, 0x978AF79C,
    0x399929E9, 0x8B50A48B, 0xC77EB99F, 0xA0CB0CA0, 0x7F206F0A, 0x437F5056, 0x39A97B60,
    0x95504604, 0x04843957, 0x890B048A, 0x03C00904, 0x3D910047, 0x69B490F0, 0x01B9B89B,
    0xBB256000, 0x54CB4859, 0x60B9C136, 0x00467121, 0x676E0C20, 0x04604220, 0x3A02F001,
    0x7A7D03E0, 0x73800783, 0x3B03000B, 0xB84903F0, 0x07C00F54, 0x3E086084, 0x70D60D40,
    0x8D30A30D, 0x7A0E3A8F, 0x7A3508B9, 0x7A06B3A3, 0x6E8DE8F1, 0x50FF9E30, 0xAA29E79E,
    0x597184EB, 0xC0FF6E17, 0xA4D75E9E, 0x257239DE, 0x70A47668, 0x7E90E47A, 0xCB8FB7EB,
    0x80700B00, 0x0359A507, 0x019029FE, 0x4A62906A, 0x048A66A6, 0x0890F90D, 0xCA0A04DA,
    0x01E01AA0, 0x0225648F, 0x5AD54400, 0x918A153B, 0xCD0E5145, 0x108E40A2, 0x04300109,
    0x0205004D, 0x403AA200, 0x00787D00, 0x2E00B7BB, 0xA03D03A0, 0x04BACB00, 0x8507D00F,
    0x40410860, 0x7E62160D, 0x8D0F0102, 0x8A33A311, 0xA38A369D, 0x5A74A653, 0x09863870,
    0x6A2A42A4, 0x96B08792, 0x83420F81, 0x93AA4AA4, 0x6993C039, 0x3AAD4308, 0x94794594,
    0xF4196874, 0x40AE94A0, 0xABEA5C9A, 0x47953A5E, 0xE9ACAC00, 0x95CAC49A, 0x3F00695E,
    0xBA6B04B0, 0x00203EA0, 0xB3A70A6E, 0x53FD0052, 0x5504CDB3, 0x095CE402, 0x06030E11,
    0x09909300, 0x4000009B, 0x32BAB42B, 0x00791200, 0xE200A87D, 0xB00903EA, 0x00D20803,
    0x8408207B, 0x8A8A0400, 0x0F4A8C0D, 0x929D6A8F, 0x66B197EA, 0x3D0823A9, 0x869E0039,
    0x50C18E39, 0x73D9316A, 0x8C65F8E9, 0xB07C9921, 0x66F6E87D, 0x1499A998, 0x59A0AB0B,
    0xAB30F61F, 0x79AB7AB5, 0x8ABA0570, 0x9AA90202, 0xC2048B24, 0x68CAA63A, 0xB4FB89A0,
    0x3E03CA0A, 0x19B99B70, 0x406257AD, 0x9AB98A74, 0xE6FC215B, 0x54802401, 0xBA002738,
    0xAAC80042, 0x776BA804, 0xAD83BAE9, 0x107DAEEB, 0xAF5AF3AF, 0xB8102AF7, 0xE9D69D4B,
    0x9DCA37B5, 0xAD8F1723, 0x49E26AB6, 0x7959E69E, 0x600FC0FB, 0xE6E28635, 0x9F093BB0,
    0xF32364F6, 0x50937658, 0xB170FFB1, 0xF81F59F7, 0x3831BD80, 0x21205583, 0x0303F951,
    0x8A029549, 0xBE58C895, 0x60B4FB28, 0xEB2C0499, 0xB2F002B8, 0x55AD2558, 0x1BF11C45,
    0x1127809C, 0x0850F0D8, 0x000A0090, 0x02A02300, 0x3800102C, 0x10027760, 0x00404D96,
    0xABB49BA7, 0xF90FB4EB, 0xB5307E00, 0xD7A8A041, 0xA0FFB580, 0x91D105B5, 0x60AFD91F,
    0xC7A0723B, 0xB655A603, 0x300CEB05, 0xC6275289, 0x66218CB6, 0x98296664, 0xBA4EB712,
    0x999236AA, 0x78A53765, 0xEB7B24DB, 0x9A40AF7A, 0x80A5D056, 0x5B8383D8, 0xB86BE3C3,
    0x8990DB27, 0x1A6BB8BB, 0x03D03B03, 0x1C017001, 0x4A7101D0, 0xC43B9400, 0x37C451CA,
    0x0C47B99B, 0x0091C568, 0x0000B00B, 0x62040200, 0x001ABE20, 0x4BC58C56, 0xBBA90065,
    0x049AEDBA, 0x89BB2BB0, 0x9BB76FBA, 0x09DA91BB, 0x6497F099, 0x04A0BBE0, 0xA9A66B43,
    0x8B6A4B04, 0x652ABC89, 0xAA67D6C7, 0x6607B6E3, 0x32408AF8, 0x767B7572, 0xF524DBD4,
    0xA0ABB1A0, 0x20C9A6BD, 0x2883BBDE, 0x7956BE00, 0xBE495926, 0xFDC91208, 0x5BE8006B,
    0xBEC03F9B, 0xA0C42AD0, 0x2B96A73C, 0x592AD8BF, 0xFE5944D1, 0x80080061, 0x02100000,
    0x8E029027, 0x0BFA0475, 0x95EBFC26, 0x01045BFF, 0xF912AECC, 0xAF4C06AE, 0xFFC090D6,
    0x0102C0B0, 0x097A34A9, 0xE3C10B5F, 0x58DE3324, 0xA42B05C1, 0x1CB0A469, 0x0C1E6648,
    0xAAC9F1B1, 0x16C259F3, 0xB228B18B, 0x831AB4C2, 0x0CC2F9FC, 0x7C33C312, 0x04890504,
    0x8F208841, 0x920895BC, 0x03EC3A00, 0x8FBE9C3C, 0x101E019B, 0xCF3BEFC4, 0x75CA2219,
    0xFBF3551A, 0x21BCFA6B, 0x4E009C4B, 0x4C52C50C, 0xC59CB1C5, 0x5B00B58F, 0xF00EB4FC,
    0x042087C5, 0x05C63B56, 0xF102C661, 0xA3491E0D, 0x100629D9, 0x0B622D9C, 0x9E5C7029,
    0xD518D930, 0x6865C787, 0xC7D0576E, 0x76C23869, 0xA872946B, 0x94AC840E, 0xC6834D30,
    0xAA5E0475, 0xB842087B, 0x05888D3A, 0x4CEBAC6A, 0xC96004C9, 0x9CC9AC98, 0x0D48B93C,
    0x591D4A27, 0xC067FD4D, 0xCCA826E6, 0xCADCAB00, 0xB00455C6, 0x5BA7962C, 0xC03AE3CB,
    0xB100C048, 0x7A89084B, 0x105BB60D, 0xFA9D4A8E, 0x008C0FFA, 0x673CC2CC, 0x59384695,
    0x60706AB0, 0x0FF08008, 0x7A08006A, 0x0B73C1F0, 0xAAFCD74A, 0x96BD599E, 0xD8FCCDB1,
    0x0577F2CD, 0x3C9A87B5, 0x7C8E8C48, 0x88BCE9C3, 0xCECEDCEB, 0x20015D3A, 0x2C5D95A1,
    0xBEA76D97, 0x3CA61126, 0xCFECFC2C, 0xBBD02D00, 0x704CD057, 0xBA9BFED0, 0x0803FAEA,
    0x2AF0D0D0, 0xAF6AF4AF, 0x0C3E1105, 0x892097AC, 0x3D0C0FD1, 0xFFDB98F1, 0x57E28F30,
    0xC29D2B67, 0xB7C2CB6B, 0x3D3207AA, 0xDD202B04, 0x39D37D35, 0x5DD6BE2D, 0xD3F848A6,
    0x3FBE9260, 0xFB30A0D0, 0xDE148CDD, 0x2DCF8B38, 0x3D482501, 0x9BA01901, 0x34032027,
    0xD0320020, 0xA7D04103, 0x7AC5AA7F, 0xD04BC5C7, 0x088086D5, 0xF0BB5042, 0xF18DB596,
    0xC690E40D, 0x88286922, 0xCD7BBBD2, 0x18DB7972, 0xCC0CA0F5, 0x39FA0CE0, 0xABEC8983,
    0x67C8B02C, 0xAAC1D372, 0x7BBA65D8, 0x06212009, 0x1004A810, 0x007D5402, 0x1E00D00C,
    0xCC4FD48E, 0xC5600002, 0x47D4CCF6, 0xC1AB324C, 0xE6E1AA17, 0xB00B1179, 0xF0000B00,
    0x0AE0AE0A, 0xBE0BC000, 0x70000BE0, 0x09F09C09, 0x1FA24A22, 0xB004965A, 0xBAAD097B,
    0xB8C0377A, 0xA040DABC, 0x7A70D8A2, 0x15DB1102, 0x60E29D6D, 0xD7F0E50E, 0xAB0CD0CC,
    0x5BDB9FB0, 0xDD4C8B7B, 0xD7D3BBE1, 0xF034D3DD, 0x00604203, 0x530037F8, 0x106005D0,
    0x0BE0B900, 0xB40000BF, 0x0000EB30, 0x0AF0AF0B, 0xBA0B1000, 0x01A7E6DE, 0xEBDEBEE7,
    0xC1E6FEC0, 0x70601ABE, 0x00006906, 0x69EA6033, 0x7DE3CA4E, 0x121ECEC4, 0xEA0E90E8,
    0x508F0000, 0x25E09709, 0xEA00290F, 0x7900AE4A, 0x00AABEAE, 0x51E8ADF1, 0xBC62C06B,
    0xBD510272, 0xCC8FB0E5, 0xAB1D0A80, 0x8C005307, 0xD583EB22, 0x200EAC3D, 0x7F800A91,
    0x3DB4B006, 0xE0550030, 0x00206105, 0x740AE0AA, 0xDE22ECFE, 0x153F06EC, 0x00086C05,
    0xB5CA02D0, 0xF04D99EC, 0x00F07F0F, 0xEED3ED20, 0x09609408, 0x5704D001, 0x000205A0,
    0x04891490, 0xE07BB006, 0xFA23E33E, 0x1B501104, 0x5002D05D, 0xD05D05A0, 0x04A048E2,
    0x57DD2020, 0xDEF30269, 0x849C389A, 0x7C006208, 0xD06B061A, 0x0B200106, 0x000B70B6,
    0x10F496F0, 0xF47F05F1, 0x09F49F46, 0x107F0781, 0xEFA00008, 0x00101F0E, 0x00000000

  EW_BITMAP_CLUT_EMPTY( CutterPaperFormat, Default )

  EW_BITMAPS_TABLE( CutterPaperFormat )
    EW_BITMAP( CutterPaperFormat, Default )
EW_END_OF_BITMAP_RES( CutterPaperFormat )

/* Table with links to derived variants of the bitmap resource : 'Cutter::PaperFormat' */
EW_RES_WITHOUT_VARIANTS( CutterPaperFormat )

/* This is a bitmap resource. */
EW_DEFINE_BITMAP_RES( CutterPaperBatch )
  EW_BITMAP_FRAMES( CutterPaperBatch, Default, EW_DRIVER_VARIANT_RGBA8888, 56, 47, 0 )
    EW_BITMAP_FRAME( 7, 11, 42, 24, 0x00000000, 0x00000000 )

  EW_BITMAP_PIXEL( CutterPaperBatch, Default )                  /* ratio 49.81 % */
    0x0409FF00, 0xC11FF020, 0x1A0F0581, 0xA110C84C, 0x150F85C3, 0xB1086C4A, 0x1488C4E2,
    0x782C5A2F, 0x82A9A0A2, 0x29D20AD7, 0x49A32088, 0xA637288C, 0xA4F2C8D4, 0x572D97CA,
    0x73098CBA, 0xFE95CDA6, 0xC7A80272, 0x82856353, 0xCC164077, 0x415F9057, 0x1565057B,
    0x9F70924C, 0xA2F60053, 0x137A9CD2, 0xFA7CBE66, 0xC55A9800, 0x53A8800A, 0x428C00A9,
    0x592C4AA5, 0xA6C765B3, 0xAA96AB3D, 0xE2D895CC, 0x77411002, 0x22BA5D6E, 0xABC5E6EC,
    0xBE5F2FB7, 0xB003021D, 0x89C01840, 0x28F000C3, 0xF1AFD0B6, 0x0AE28148, 0x8EC74BFE,
    0xAA4E3FDD, 0x1429C23E, 0x65305FE8, 0x1BF1FEC8, 0xC15FB92D, 0x2B52CE5D, 0x401AC2D9,
    0x3400D784, 0xA20006C8, 0xB8BBDFB6, 0xADBEE6F7, 0x12092F7B, 0xD0DC5800, 0x0E05A001,
    0xEC71C11E, 0x859CBE48, 0x9DD0E672, 0xD6EAAF2F, 0x00EBB462, 0xCEDF6BB2, 0xEFF7BBBD,
    0x07880A00, 0x04BCA570, 0x14E7E861, 0x1DD926D4, 0x3CFF783C, 0x6FF54AB1, 0xFF331ACA,
    0x92614868, 0x7F92CA5F, 0xC5115C3A, 0xD2046C3F, 0x89A06787, 0x57649A46, 0x298B93E5,
    0x97811A10, 0x6855E000, 0xE1D985E1, 0x00118430, 0x124460EC, 0x0450E800, 0x588E2112,
    0xA2228922, 0xD58A6268, 0x349098B5, 0x63286230, 0xB8D46477, 0xBE383400, 0xEF649EB1,
    0x3FCF13C4, 0xCB02CCA6, 0x91B8563F, 0x44333FC6, 0x4F0C5FF1, 0x17C43530, 0x4543FC59,
    0x83FC681B, 0xFC4F1543, 0xFF1224C3, 0x148580F0, 0x8861C4FF, 0xF318FF20, 0xA592334C,
    0x992C5059, 0x0C4409B4, 0x6728CE70, 0x60180576, 0x721A0000, 0x2C8821E8, 0x9FE2B9F6,
    0xDD48922A, 0x28C103A0, 0x05DB9CCD, 0x4F0028A0, 0x3540EA34, 0x5027E290, 0x33C10548,
    0xA0FF3C8F, 0x57528B52, 0x3FC6B1C0, 0x9FC14C35, 0xF128500C, 0x545B0FCF, 0x8B1DCFF1,
    0x338FF21C, 0x8FF350D6, 0x72A9F07B, 0x6CFF2D0B, 0xFF1B8731, 0x134530D8, 0x6A6A90FF,
    0xF1505A0F, 0x14891D4F, 0xD332CFF2, 0x40A6A348, 0xCF33D0F8, 0x628CAB3C, 0x64340F2D,
    0x7577A88B, 0x876799DE, 0xAE289E1F, 0x4B8A819F, 0x19114D68, 0xEC02618A, 0xBBC55002,
    0xAF230C0A, 0x5B390518, 0xDCF73D50, 0xB62A8FF3, 0x1C85AAE0, 0x6BFAF46D, 0x43FEA5B0,
    0x2476B1AC, 0xD5ABC862, 0xC3ECFAC8, 0xE973C4FF, 0x83FEFDBE, 0x086C1C45, 0x06B0AC0C,
    0x22B08167, 0xCD43370B, 0x33F70F34, 0x40AD3F1A, 0x230D0A68, 0xB7673CB4, 0x2E086E78,
    0x5B8E809F, 0xB6E78B33, 0xE0342388, 0x0133E171, 0x45DD04C7, 0x6648CB41, 0xBCFFBE4F,
    0xFF2E4BC2, 0x1C075174, 0x04F96B1A, 0xF97C3B97, 0x90871C8F, 0x45334C49, 0xD7F5EC43,
    0xCF23C102, 0xF2C8A48F, 0x546E948A, 0x5392DFA9, 0x3FEA9A9F, 0x08B23C7B, 0xC9AB0EC3,
    0x84FE4E4F, 0x35DEC912, 0x7837E0A4, 0xA7BB7F30, 0x6B92E5CD, 0xC2E85BEE, 0x00E2CDB4,
    0x0178E10C, 0xA60339A9, 0x740B5D40, 0xFCF83E0F, 0xBD300B43, 0x1DC603FC, 0x0373FC72,
    0x59204E15, 0xB660D570, 0xFF1F8831, 0x328CC30C, 0xF6051A0A, 0xE9A7B4EC, 0x81F03C4E,
    0x522A8AE2, 0x7F4671B6, 0x8510C9FF, 0x617040A9, 0x48803FC5, 0xD63FC8E2, 0xC3CDB370,
    0x7D23BCF3, 0xCF549A27, 0x73D80B82, 0x420FC337, 0xE1AE1884, 0x739CD7E0, 0xA4140655,
    0x48C640AA, 0xFB3F0FB6, 0x2F8B33FC, 0x4623FCBB, 0x5E8D231E, 0x0E01C73A, 0x7C2046B8,
    0xEC0CBEB4, 0x6987F0FD, 0x02C0476A, 0xED1D2D82, 0x58274CB0, 0x447F8A81, 0x00650D61,
    0x66087ED5, 0x0178212A, 0x83F8FF0B, 0x8D51FEF2, 0x59E7B06D, 0x3DD7C8F4, 0x510A1136,
    0x4F050288, 0x001702C8, 0x0C200340, 0x20390001, 0x470811C2, 0xAE219C43, 0x8812C03A,
    0xB273242F, 0x18C2F47F, 0x1987F8C4, 0xD10780FA, 0x5C6A4E91, 0xD09652BA, 0xFC3D07E0,
    0xFC401783, 0x68070047, 0xD15A2AC0, 0xC7599480, 0x8AA1363F, 0x09A3FC53, 0xB0430867,
    0x27352786, 0x884A2179, 0xFE2544C8, 0x746F0D91, 0x68F461E3, 0xC1283F4C, 0xE2504988,
    0x0E59C204, 0x03F0780C, 0x86B0CA18, 0x409F1A26, 0x5E10508C, 0xFF7D8FA8, 0x8A31C5F0,
    0xF8338FF1, 0x12000F21, 0x91FF0510, 0x89642B03, 0xC3B07B0C, 0x0C21703F, 0x1EE3FC60,
    0x15DCC8F9, 0x52CA4947, 0xEA1CE40A, 0x263FC750, 0xFC500A31, 0xA06709C3, 0x4490DAD7,
    0x61842346, 0x8440FF0B, 0x0FF12C26, 0x32238C6F, 0x348762CF, 0x468D61AA, 0x770E81B0,
    0x5C0D4120, 0xF4188008, 0x9C7E0800, 0x125F0DA1, 0x4A988280, 0xC82BF120, 0x8BF28CFA,
    0x40831C64, 0x25503882, 0xA63040A1, 0xF47E4FD8, 0x882E87FB, 0xE07F8C11, 0x8CA62AE3,
    0x59E73CA7, 0xC39C7190, 0x04A0FF1C, 0x84A95C29, 0x96817C32, 0xA9924AA6, 0x2E06208C,
    0x9D1183FC, 0xC23FC4C8, 0x3C728E51, 0x50C71D8C, 0xB11C32C6, 0xC1D43986, 0x02083503,
    0x03600011, 0x23411421, 0x2B882823, 0x670C561E, 0x6E1FE330, 0x50EB0420, 0x96234657,
    0x88770BEE, 0xB1603FC3, 0x93BDA60B, 0xA9CCF428, 0x902A774E, 0x1FF2FC6F, 0x26C4F88F,
    0x310481FE, 0xC2638D0C, 0xC6121533, 0x8848B430, 0x00FF13C2, 0xF1C83947, 0x031475B0,
    0x83B0C919, 0x04808C0E, 0xABC41402, 0xE5C51902, 0x6FCDD732, 0xAC1041B2, 0xAA95983B,
    0x6C1F3A50, 0xD06458C1, 0x01652683, 0xC3D07FBF, 0xA9E0F51E, 0xD7CAF75E, 0x1CC38882,
    0x2884950E, 0x278FF13E, 0xFF0C61A4, 0xA80DA960, 0xFE7EAC00, 0x509E11B3, 0x59814FA8,
    0xC2D05810, 0x4811FF16, 0x577A4401, 0x9792E8A4, 0x7F0FD56A, 0x8EAC507C, 0x851FE14D,
    0x6624C4B8, 0xBABEBB85, 0x857BB5F6, 0x15FEAC95, 0xE1641E30, 0xE5A3F9B4, 0x06A0A07F,
    0x19122432, 0x2E0C948C, 0x2B8BE17A, 0x0508E450, 0xB26487A1, 0xFC6F0DF1, 0xA09910E3,
    0x149E3FC4, 0x6B0150A5, 0xA303A5B1, 0x8A913518, 0x1D63A08F, 0x78AD84AA, 0x66B10BC7,
    0x76648354, 0x4AC19900, 0x1EEBD8BB, 0xFB000B31, 0xA16C2A47, 0x54E25014, 0x82A0CA4D,
    0x8120C017, 0x47C3C08C, 0x36C6C82B, 0x815501B8, 0xD83243CC, 0x26F22370, 0x42E988F1,
    0x1AA0A009, 0x10D41458, 0x5178CC41, 0xAF24091D, 0x8D0A7386, 0x1FE3586B, 0x22046078,
    0x2E8411FE, 0x35967718, 0xDE156098, 0xC451A470, 0x90F1A462, 0x18449A56, 0x22DC598E,
    0xC2C87C17, 0x8200CA18, 0x2320253D, 0x0005F5C3, 0x1C836054, 0x840FA000, 0xB40C4430,
    0x486E8DC1, 0x2D792057, 0xA79EF348, 0x83FC4BCB, 0x02D06008, 0x4E40AF25, 0x82103AEB,
    0xEF0561F1, 0x0D9F076E, 0xD162199B, 0xFA4846C0, 0xB8460900, 0xCCE7554D, 0xF630166D,
    0xA2DC558F, 0x0768FF16, 0x2691DE3C, 0x1BC3E1CC, 0x29C528A1, 0x30858108, 0xC606E190,
    0x660581E1, 0x502083E0, 0x78478620, 0xE1B64640, 0x0CCC6F0D, 0x02884700, 0x0AA00015,
    0xB8C2C057, 0x9A5BA902, 0x648CB186, 0x460AA3FC, 0x02D5AC4A, 0xF6814E44, 0x3EA83F87,
    0x31629052, 0xD99CF7AB, 0x4F641DB5, 0x09012820, 0x4B589B12, 0xAF8A6050, 0xD3DB6580,
    0x7E406D56, 0xCB9B8488, 0x3C740E91, 0xB0D41FCC, 0x016635C6, 0x41E83C07, 0x41D02E09,
    0xC7B0360F, 0xA3A0681F, 0x8A40B845, 0x03129D24, 0xE61CA38C, 0xE43C8298, 0x14D08F31,
    0x28FF31A5, 0x50DED169, 0xC9744008, 0x6B5DED6A, 0xC9A930E9, 0x082C2EF4, 0xABAE305C,
    0x37A44D87, 0xA71FE1A8, 0xC5AD0598, 0x5B5BA986, 0xF8DF1B71, 0x51063FC6, 0x395C4889,
    0x8FCDBACB, 0xA5577A92, 0x6E62383D, 0x1883304C, 0x7EF04E09, 0x901FE182, 0xFE270500,
    0x3E7C4F61, 0xB8D41E4B, 0x814E3606, 0xC2084007, 0x02483E07, 0x03401013, 0x22801DEE,
    0xE7482826, 0xAE0C559E, 0x860E03FD, 0x81B1CCE8, 0x36C64E6D, 0xDB4969ED, 0x71FE2E05,
    0xC6497D34, 0x164324BC, 0x52DB4C21, 0x602AEB82, 0x5D859DF3, 0xC970EAF1, 0xB9B9A368,
    0x892F04CC, 0x932D05F0, 0xD6AE298A, 0x71323FEE, 0x03FC520A, 0x482DE195, 0x8E11C437,
    0x1FEF5283, 0x24E82847, 0x55D8ED5D, 0xCC1FF2B6, 0xAB1C074E, 0x24431711, 0x28560A61,
    0xB11A43DF, 0xC331D6CD, 0x84206FD4, 0xE70551BD, 0xB6E0F623, 0x7A83BB9E, 0x5D0BC162,
    0x511EA3FC, 0x4F4962B9, 0xA931872A, 0x124C4C1A, 0x40B7D3B6, 0xA2C55E07, 0x7AAFA617,
    0x19197D5F, 0xA1427046, 0xC725C4B6, 0x3284BD5C, 0x50959C9C, 0xAE39F665, 0x3BBEF7B8,
    0x60FFA842, 0x39231842, 0xA3FEEC20, 0x448727F8, 0xCBD13A28, 0x325DF471, 0x2E05B8B2,
    0x52114068, 0xD05F4BE0, 0xC097AE95, 0xA8FE2A14, 0x55CFD8B5, 0xC1E63FFB, 0xF1CA7A13,
    0xDA2B5B1F, 0xD2C40F59, 0x4AEC1419, 0xA139F3A5, 0x37B80B23, 0x45565176, 0xFF50D0E2,
    0x309E08E0, 0x6AE78554, 0x491FF249, 0x13430640, 0x4F672B54, 0x6953B7CB, 0xA7B41216,
    0x7FD52C23, 0x88586464, 0x20C50BB5, 0x0C12690C, 0x990C60C5, 0xE3E71155, 0x0D71924E,
    0x840FF3CE, 0xE0920960, 0x70425160, 0x500FF44A, 0x91975870, 0x0EB0FF19, 0xF40ED0EE,
    0xE1021394, 0x06D3B03A, 0xE465B568, 0x72BD7B14, 0x4E906144, 0x9D0BF0B4, 0x23ED14E3,
    0x6CE6CC19, 0xFF583093, 0x654C0490, 0x0312A63A, 0x825446C1, 0xC5437CF7, 0x0CF53C7B,
    0x790FF2B1, 0x40FF6220, 0x8392AA5A, 0x9F29D4E7, 0xA0B40A72, 0x70D29614, 0xD41A3145,
    0x907A6D50, 0x03D12E01, 0xD330F26B, 0xF0D80DA0, 0x66E66C0F, 0x6F3C7700, 0x21FC6117,
    0x1F806C05, 0xAD0AF0A2, 0x43D26790, 0x6F00FC4F, 0x8D8250BA, 0x62564444, 0x24D84B05,
    0xFF258494, 0xD08006B0, 0x7DC7293F, 0x345BE83B, 0x831C71B0, 0x08665A65, 0xDC5D2098,
    0xC0E00E10, 0x88588353, 0x8D09007E, 0xB4186BE0, 0x0FF7ED88, 0xFF1F7052, 0xB0AE0A10,
    0x7F57F30A, 0xC20E92CF, 0x50908290, 0x02407651, 0x111256DD, 0xF0CE0CB1, 0x04B0000C,
    0x00058056, 0xE09D0970, 0x0DB04009, 0xAC0DE0DD, 0x545A1050, 0x41E0D410, 0x44715713,
    0x45B985D5, 0x8C36B170, 0x3D8E30AF, 0x74F323F2, 0x8240FF53, 0x650FF0C3, 0x914107B0,
    0x1FF8AB83, 0xAE1FC629, 0xB7F15D46, 0x0E00DB46, 0xD653B712, 0x03CF0DD0, 0x0FF60C08,
    0x04418416, 0xF2605B67, 0x62E0FF75, 0xEE840523, 0xF3D23B60, 0x0BA0B60D, 0x630960BB,
    0x106E06C0, 0x0423DB04, 0x3C031016, 0x30150400, 0x6E71196E, 0x440000E6, 0xD05104E0,
    0x00047C4C, 0xB20B10AC, 0xF10225C0, 0x0C62CF46, 0xC12680CE, 0x356A1F88, 0x5A252D4E,
    0x998720FF, 0x20FF29B2, 0x4F36320F, 0xFF14E0FA, 0xF0C10B70, 0x07839E0B, 0xC66251EA,
    0xD0930815, 0x41F3CD6B, 0xDC60953A, 0x08348320, 0x78341791, 0xC65A4464, 0xE49203B2,
    0x0FF08A71, 0xCF196618, 0xE6940FE2, 0x0610B50D, 0x3F06C069, 0xE01A1D20, 0x03C03902,
    0x1922901A, 0x300E2290, 0x0BC0B813, 0xEE0420BD, 0x20EF0EE0, 0x3B210276, 0x670C7102,
    0x07AE2612, 0x4E76F97F, 0x731233F3, 0xD0830880, 0x4E556F86, 0x19317351, 0x80B80AC3,
    0x1487A867, 0xCA94E4F3, 0x353B8665, 0x0878B58B, 0x4546B098, 0xF4934B70, 0x61440E8F,
    0x7207C0FF, 0xE06485B3, 0x4E735105, 0x924FF7C5, 0xF5840A30, 0x2CF50E57, 0xC78CB0EF,
    0x306C0960, 0x03107607, 0x3E03B030, 0x721A0040, 0x10F63F04, 0x0559501E, 0xC0FF4541,
    0x0D10D40C, 0x5B99B0FF, 0x18117F07, 0x2BC2BA35, 0xB60B90AD, 0x25CA8C70, 0x0D10C919,
    0xF69940CF, 0x64639146, 0x05E7D62A, 0xAE350076, 0x47530BA0, 0x7659ED7F, 0xE40DF0FF,
    0xA77E0E30, 0x34354B50, 0x2171B810, 0x05824627, 0x26977855, 0x9B08A866, 0x64B66560,
    0x3C054950, 0x977D345F, 0x30A30A60, 0x8E50ED19, 0xCF0E726A, 0xE0D20D10, 0x93404307,
    0xD5105003, 0x708E1024, 0x19F1A689, 0xEB9E21A1, 0xC4E72C65, 0x35034E52, 0x4D9FC0AF,
    0x69FF0F49, 0x6F374540, 0xB06F779A, 0xE70436D7, 0x3EF75005, 0xFA1583F1, 0x70E257D0,
    0x80D80B4F, 0x937C10FF, 0xD04A31C4, 0x7DB7289B, 0x57553551, 0x046F46D4, 0x78154359,
    0x92039343, 0x27727044, 0x85758458, 0x97091269, 0x50390980, 0x10273F10, 0xFF8770BA,
    0x75AB8B00, 0x61156C03, 0xA779F2C0, 0xF50796A5, 0x3133117B, 0xBF43D4F3, 0x93E90C80,
    0x44E07F06, 0x3988B946, 0x507A0628, 0x0C295307, 0x57A4A956, 0xE7869C64, 0x85EA0B4A,
    0x60045719, 0xB08B4B80, 0x55109809, 0xD253CA03, 0xF1210D90, 0x81F1EA39, 0x7284C356,
    0xE0C30B97, 0x0CA19249, 0x880CE0CD, 0x141810F5, 0x65319236, 0x1160D60B, 0x58003519,
    0x6613BE1F, 0x8B0EC1F4, 0xEA924F49, 0x0FFA840B, 0x9EA88A86, 0xC839A988, 0xAAC0FF6F,
    0x928F64ED, 0x645A5DF1, 0x0A20A609, 0x197227D1, 0x6781A177, 0x09A08950, 0xA27CD656,
    0xB9B9AA4A, 0x3A295A9B, 0x2B7D6A77, 0x30B062D6, 0x26966489, 0xCCA3C0F3, 0xD06E9230,
    0x40111901, 0x0D02F0FE, 0x51024AE2, 0x81B10285, 0xBE6CA886, 0x884C5C66, 0x41140F94,
    0x128E3891, 0x74F18783, 0x8264EB7F, 0x5B946ACE, 0xE25956B2, 0x0FF3AB06, 0xBC55054E,
    0x09E54115, 0x60F24E86, 0xCC835833, 0x753A93E5, 0x66D8868B, 0xC73C50FF, 0xE9E584C3,
    0x0A11F888, 0x720FF8F1, 0x90E42CFA, 0x07F69E0A, 0x1C063685, 0x26DA02F0, 0x0C511679,
    0x2C0CA0C9, 0x80814CC4, 0x03608A08, 0xD60D60D3, 0x298F3BC0, 0x61E0D119, 0xAE08C620,
    0xF31C56A2, 0x29E0588B, 0xFF875708, 0x0AC48F30, 0x4868514F, 0x473F584F, 0xB7C36AE4,
    0x082AE771, 0x313CC6BC, 0xD0D753B8, 0xB219630D, 0x0576E717, 0xE5869E53, 0x45E45C5B,
    0x693630EF, 0x49510FA2, 0x0B50B00E, 0x550910B6, 0xA06205F0, 0x0404CE03, 0x4168C017,
    0x027B0160, 0x8D411901, 0x2F42D8D6, 0x13331D24, 0x08007900, 0xDD044083, 0x30E00DF0,
    0x5F710224, 0xAD620374, 0xE49366F2, 0x31967365, 0x3114A29A, 0x74F36076, 0x0BF39A9A,
    0x71A37486, 0x5373AE5B, 0x0D90DA0D, 0xE20DC53C, 0xB5A98B50, 0x9E59045A, 0x738A99B6,
    0xE0A50A87, 0x143972A1, 0xAF0EA157, 0x30B40B30, 0x061B8309, 0x7F02D043, 0x34CB0259,
    0x69002704, 0x18229021, 0xA1339840, 0x07407206, 0xD202F001, 0xE0370049, 0x9D100751,
    0x7900503E, 0x908100D2, 0x04F08B08, 0x0F91EB9B, 0x00D20CA3, 0x19D9940D, 0x4E0309F4,
    0xB75E25C0, 0x9E90FF2B, 0xF37BA9EB, 0x95CAAF14, 0x539B2460, 0xDF712907, 0x6061A14A,
    0xA178A0A1, 0xA4095785, 0x80FF0A10, 0xA200EC0E, 0xB10ED269, 0xC099B800, 0x06706505,
    0x3EB3C046, 0xD0320280, 0x22902868, 0xD9229029, 0x3983223B, 0x22901122, 0x931309D7,
    0x109B0990, 0x03202600, 0x8A002035, 0x8690004B, 0x00C22900, 0x0803D6D9, 0xC04A03E0,
    0x08701504, 0x5408F08E, 0xF0DE0DC0, 0x26A7430D, 0xF40FFA3F, 0x396C6F66, 0x4E731C20,
    0xFF53105E, 0x23F18040, 0x0E0A00C0, 0xEB864AA1, 0x44645478, 0x9C206478, 0xCD7C90A1,
    0xC0ED30F6, 0x0C10C00B, 0x68C1E09F, 0x1C250490, 0x68DC2404, 0x4855B02A, 0x2223C290,
    0x01B22902, 0xE3014229, 0xB229C306, 0x0CB43759, 0x64000AB1, 0x20019240, 0xB87002B9,
    0x08229004, 0xB21B0370, 0x008C4100, 0x4E04C041, 0x8A6A0130, 0x0E905A09, 0x260EB0EA,
    0x20FE1028, 0x4E084619, 0x5735652B, 0xE95884EC, 0x7A7BB03E, 0x7B9A80F7, 0x80E80E35,
    0x0FF8A74F, 0xB64FFA55, 0x32A67FE9, 0x5089BE81, 0x4E72C09A, 0xF0F02CFA, 0x0C40C30B,
    0x6D0640A1, 0x704D06F0, 0x927028B8, 0x2769002A, 0xC229C7DC, 0x92D22901, 0x2900E223,
    0x9C8500A2, 0x22900322, 0x2C389A2B, 0x00BF0BB1, 0x0650000C, 0x0107006E, 0x203F92A0,
    0x9D3BE200, 0x3600827B, 0xF00B51A0, 0x046008CE, 0x15053051, 0xC09BC330, 0x0ECCA105,
    0xC02CF4ED, 0x7AC84DE0, 0xACB494A8, 0xE9A8BB13, 0xE49E49C4, 0xA68457A2, 0xC14AE4FA,
    0xA84C4939, 0x5084B8A9, 0x6A9AC50A, 0x20BE0F62, 0x0A40C30C, 0x74071069, 0x992704F0,
    0x690CCE02, 0x23048C7C, 0x3C81C7F0, 0x009223C8, 0xDF223CDD, 0x6001223C, 0x11648238,
    0x000FBB7C, 0x50C40C00, 0x0660000C, 0x0107106F, 0xA04703B0, 0x03C00204, 0xF1004D45,
    0x0045BE5C, 0x0086DAC4, 0x5905604D, 0x60A101A0, 0x0650A70A, 0x6898B0ED, 0xA3781052,
    0x8260C44E, 0x4507C065, 0xB259A764, 0xAC994688, 0xFF48B489, 0x2AFE9510, 0x0EA0E610,
    0x1F5DC78A, 0x806804E1, 0x6AE9E558, 0xA965A76C, 0x08B48B25, 0x0D08D447, 0x790710AF,
    0x305507B0, 0x02A045CF, 0x23C79B87, 0x5D2AD8A2, 0x016229CD, 0xDC048D2E, 0x9006223C,
    0xCE000222, 0x81337D98, 0x03394374, 0x0009080E, 0x1101000E, 0xA02F0000, 0xD480011C,
    0x3D00204B, 0x104B0480, 0xCF2223C9, 0x2200A51A, 0x804E008C, 0x01905B05, 0xA60A409F,
    0x5A3C0650, 0x1920CE0F, 0xC249BD6C, 0x6B114ED0, 0x94649349, 0x0B07FB16, 0xF88E7D08,
    0x6109013B, 0x65713D79, 0xF5CE7679, 0x0CC0F52C, 0xAC0D00CF, 0x607B6D50, 0x046C9405,
    0x2947CD88, 0x34320380, 0x223D8DCD, 0xD8223CD6, 0xB00F048C, 0x048D31CD, 0x35048D33,
    0x2385D9AD, 0xDF7D9C3E, 0xD11D7106, 0xA0161E00, 0xDA401B01, 0x4CDABDA6, 0x573ADAA0,
    0x73DC9200, 0x4FDB2042, 0x801CBCFD, 0x0AE0AD0A, 0xBF42206D, 0x5844D5CD, 0x43B0C00B,
    0x3207605F, 0x38BE4E75, 0xB2A84C41, 0xD441B3C8, 0x99600DA0, 0x0D10FC26, 0xB30D40D4,
    0x508207B0, 0x69005B08, 0x27BE802C, 0xBD292790, 0xD8FDE8D2, 0x4800AD91, 0xC00504AD,
    0xCDFDAC03, 0xF5DF3432, 0x2480E43D, 0x22DDF828, 0x2FE490CF, 0x00090083, 0x02801F00,
    0xD2BE102B, 0x5E080029, 0x73DC3D00, 0x8700C043, 0x4056008B, 0x0AE021B8, 0x700B40B2,
    0xC0D57550, 0x1440FA20, 0x4F0B79FB, 0xF04023A7, 0x0FF96E6B, 0x0F66A0D2, 0xA0D80FA3,
    0x0BB0DB0D, 0x8508307C, 0xDE3B05D0, 0x0296DD02, 0x36D8C690, 0x801D048E, 0xDED223E3,
    0xF300A223, 0x2006044C, 0x04103D03, 0x30027003, 0x00020330, 0x01801601, 0x821DB001,
    0x5DF43DF3, 0xE4722EE4, 0x000F26EB, 0x3EA40B80, 0xEA4EA5EA, 0xB70000B8, 0x5000EAB0,
    0xEA6EAE0B, 0x3D0C1EA9, 0x2059000D, 0x00106506, 0x3A041E8E, 0xFE063DCC, 0x00AE58D2,
    0x64EB58D1, 0x6C1B01F0, 0xA4A0740B, 0xFE105BB8, 0xF76846C9, 0x64C0F92C, 0x450B80DC,
    0x906108AB, 0x02E04C73, 0x34028CEF, 0xE024229E, 0x229DEAD8, 0x0B229010, 0x6047DE40,
    0x1C302B00, 0x4F044003, 0xE0010510, 0xD3BDBA09, 0xAD0C10C1, 0x80B40B4E, 0x0AD0AC93,
    0xAC03E000, 0x1E9E4D1D, 0xCE2E463E, 0xE2E9FD37, 0x50B40AFC, 0xEB6EB40B, 0x3E03ABE1,
    0xD92BC8F0, 0x009048EB, 0x3BEC0C75, 0xD00803F0, 0x06906705, 0xB80B4023, 0x407B0B90,
    0x1029E085, 0xE1B9C0EC, 0xFC480BA0, 0x73AED508, 0x27CEF02D, 0xDD296E00, 0xE85223ED,
    0x236E2223, 0xA27B0102, 0x04451900, 0x03CEF007, 0xD04A03F0, 0x09400104, 0x0009C09B,
    0x30EED590, 0x0FD3EB15, 0x3D040000, 0xE21C331F, 0xF01E9EEF, 0x19E47F50, 0xF0009A71,
    0xC8A0BB91, 0x86001CCB, 0xE223D97D, 0xB8A009F0, 0x0BD4C00B, 0xA05704E0, 0x05002405,
    0x3305D05A, 0x204F0450, 0x92A02505, 0x79021040, 0x32290262, 0x048F30E8, 0xEEF33D90,
    0x3E8BD94D, 0x05ADB500, 0x99A6A002, 0x2CFF0010, 0x98C000F0, 0x4BF490F0, 0xEF4E2DFF,
    0x000101F4, 0x00000000

  EW_BITMAP_CLUT_EMPTY( CutterPaperBatch, Default )

  EW_BITMAPS_TABLE( CutterPaperBatch )
    EW_BITMAP( CutterPaperBatch, Default )
EW_END_OF_BITMAP_RES( CutterPaperBatch )

/* Table with links to derived variants of the bitmap resource : 'Cutter::PaperBatch' */
EW_RES_WITHOUT_VARIANTS( CutterPaperBatch )

/* This is a bitmap resource. */
EW_DEFINE_BITMAP_RES( CutterPaperSpeed )
  EW_BITMAP_FRAMES( CutterPaperSpeed, Default, EW_DRIVER_VARIANT_RGBA8888, 52, 52, 0 )
    EW_BITMAP_FRAME( 9, 8, 34, 36, 0x00000000, 0x00000000 )

  EW_BITMAP_PIXEL( CutterPaperSpeed, Default )                  /* ratio 46.26 % */
    0x0409FF00, 0xC11FF020, 0x1A0F0581, 0xA110C84C, 0x150F85C3, 0xB1086C4A, 0x1488C4E2,
    0x50DC1421, 0x15C14410, 0xE4163B1F, 0x40E35050, 0x01D0E773, 0xDD8EC758, 0x5F148B45,
    0x33498CCA, 0xD06733A2, 0xA1A0AC00, 0x220E8003, 0x41167C47, 0xB71AD122, 0xB940006D,
    0xD4C0055A, 0x9F4D582C, 0x94EA8546, 0x53AAD4AA, 0x4ADD62B3, 0x5200162B, 0xE0015EB2,
    0x8014EAB4, 0x0154AD4F, 0x152AC4E8, 0xB8DE6B80, 0xA3F8282D, 0x9C829C20, 0x582B020A,
    0x82B7A0AD, 0x2DFB00F0, 0xB060AD68, 0x51A49D82, 0x1B4726AB, 0xAA3F6EB7, 0x80C6BB59,
    0x67522913, 0xD4F002D3, 0xAA2D29B6, 0x1D5EB55D, 0x68B4B56D, 0x452885D3, 0x09235002,
    0x964A1DD1, 0x509A2508, 0x4410DAA9, 0x01800076, 0xDF6F2885, 0xFF0777BD, 0x71B81C2D,
    0x27823100, 0xC8E26A27, 0x884FDD8F, 0x4703B17A, 0x9B4FA380, 0x06D51878, 0x7A3D1E49,
    0x2BE60AE0, 0xF99FAFF8, 0xB5DCEB7F, 0xDC0DB7FB, 0xB6D57FB7, 0x6CB7FB69, 0xF6FA345A,
    0xEF67CBE1, 0x773BBD1E, 0x9E479DE5, 0x0814F178, 0x60D8354E, 0x93224913, 0x13C3D24C,
    0xC0317C61, 0x00F84309, 0x04480A40, 0x49145C81, 0x745C3720, 0xDC4885C5, 0x5705C388,
    0x004C1503, 0x571803F0, 0x1443C000, 0x9404F84E, 0xE7115844, 0x005416C3, 0x3B8E6370,
    0x8FA388EA, 0xE3D8FE3C, 0x01545B0F, 0x44580EC0, 0x4312D3A1, 0x23078000, 0xCD000888,
    0x614CD635, 0xB90580D8, 0xBC604F13, 0x3FCC4318, 0xC9626C89, 0x91FC683F, 0xB8543FC7,
    0x3B3FC671, 0x14C353FE, 0x02A3FC4C, 0xF9D27293, 0xA66D9C66, 0x679966B9, 0x97663986,
    0x18196A5F, 0x63F10296, 0xECB830A8, 0x5A158BD2, 0x00C67198, 0xC421102E, 0xD8AA4991,
    0x663D9185, 0x41A6E42A, 0xA5E3AA76, 0xE311422E, 0x5425414F, 0x7A9A7063, 0x00902A9A,
    0x14C5A0F4, 0x84B511D0, 0x558C6019, 0xF8FD0C96, 0x842C8FE3, 0x358CF3FE, 0x49921D7D,
    0x3120FF25, 0xC0FF1706, 0x2D3FDD10, 0xB2445134, 0x0FF11C4F, 0xF1646011, 0xFC7174CF,
    0x81218713, 0x21C723FC, 0xAE0B7882, 0x6B5EDCB6, 0x5AD4B647, 0xCDB4ACAB, 0x122CAB3E,
    0xEC711CFF, 0x4264863F, 0xF6C0AF89, 0xF43D4FFA, 0x24CB43D4, 0x43326C98, 0x01404E10,
    0x14480C80, 0x5C0FAAC1, 0xC2E9CA60, 0x2A00FB0F, 0x58CAA20F, 0x12A740EA, 0x3FC6EAB5,
    0x0E6B0ABA, 0x40013C56, 0x21DC7D19, 0xD4D97C40, 0xE43C5051, 0x38CE4379, 0x095208FF,
    0xF0F0FF24, 0x24283FEA, 0x60B4C411, 0x1C0FF168, 0xCFF20085, 0xF2B4B128, 0x64DA34CF,
    0xEC3A0FF3, 0xBF4FF3AC, 0x2863FCF5, 0xEAE8FF3F, 0x4ABAD54F, 0xF5D3E86D, 0xD3B4CD44,
    0xB44D2B48, 0x9D0AE42B, 0xD73ACE2E, 0x34CCB37C, 0x388E337B, 0x4CB3CABD, 0xD1E4CF33,
    0x14591544, 0x44110320, 0x14857AB4, 0xB10C363A, 0x5C4E42AA, 0x4A34A931, 0x0FC46365,
    0x5121EC7B, 0x4A62A89B, 0x42A50505, 0xFE053BDE, 0x9F28C933, 0x17C423FC, 0x82B3FC58,
    0xF3FC4312, 0xFC621A44, 0xEC2A9683, 0xBE9EF385, 0x8F980FB0, 0x3ECDE33E, 0xB7B8EC5E,
    0xA6EADEFE, 0xE93A2EA3, 0x85717B9E, 0xF3FF9CE6, 0x932C3C8E, 0xEF3BCEE7, 0x218802B4,
    0x42F06889, 0x38B83D0E, 0x3B0CC885, 0x868F384C, 0xA9A33A8F, 0x78CAA38A, 0xE0BF8E8E,
    0xA4995044, 0x05078A52, 0x351E431D, 0x0FF360DA, 0xF1BC7517, 0x08490A4F, 0xC2104FF1,
    0x29F913FF, 0xF30768FF, 0x2C404772, 0x075C2B7D, 0x0502A02C, 0xF877C022, 0xE400154F,
    0xA7F0F0DF, 0x0FD9FA3F, 0x94234A7E, 0x88D10E32, 0x05808C47, 0x0F0DD037, 0x893060A6,
    0xDD616F69, 0xA0E258B3, 0x394F38B2, 0xD21D392E, 0x414C2A44, 0x833C8112, 0xC628BB3C,
    0x8451FE30, 0x056CF830, 0x3FC2204B, 0x3E0820D8, 0xDF1B43FC, 0x9702CBF8, 0xC82D2038,
    0x70FF0D81, 0x48991422, 0x712623C5, 0x5D144588, 0xC3E87910, 0x8770E47F, 0x349AB1E4,
    0x4608A108, 0x3A05E094, 0xA80F10B0, 0x65504250, 0x0388852A, 0x47C50B1F, 0x000157BE,
    0x0B1B2107, 0x10272904, 0x160415F7, 0x3FC5C8BC, 0x7FA49074, 0x4A0A20CC, 0xEEE75E20,
    0x0C7C0F81, 0xA0FD1F88, 0x6160289A, 0x42F45934, 0x55EC5517, 0x8FF7EA17, 0x932FDA4D,
    0x2F859258, 0x4A0A3930, 0x03922D4A, 0x3723A462, 0x2643B9D7, 0xF8519092, 0x49052007,
    0x43EC7D1E, 0xC3A0CC1F, 0x81D0FC1E, 0x39E0F018, 0xA7E3603D, 0xEFC70530, 0x447C2C61,
    0x78E081F9, 0xF0329413, 0x28030EE1, 0x68702AC8, 0xE0E61D8C, 0xD370500A, 0xB4309A14,
    0x1E6EE5A2, 0x1447F9DF, 0x2E0650D6, 0x4D1D2CFA, 0xAC40A503, 0xFC3486F0, 0x54EE9D83,
    0xD69D3A27, 0xB38D9F43, 0x47A4E09C, 0x4DA6C6EE, 0x9B335E6E, 0x80E6A8BB, 0x738E71CA,
    0x510902CC, 0x640242A0, 0xF50D03D0, 0xFA4A0AA0, 0x10AC1781, 0xCC5DEF2A, 0x94A31920,
    0x05F56692, 0x1B234400, 0x790398D6, 0x9E3F11A4, 0x52066CF9, 0xA93F2448, 0xEC618B88,
    0xC2F45DA5, 0x56ACBDC4, 0x43506645, 0x29B0FF1A, 0x5348A94E, 0x5994C843, 0x4B47FACE,
    0x7E4542F7, 0xD49134A2, 0xE54919F2, 0x24629E47, 0xD87E1943, 0x843A1181, 0x41182103,
    0x02C83E00, 0xB2245EAC, 0x8E731E38, 0x93620144, 0x560AF142, 0xF520A0B8, 0x38FCD410,
    0x562D82C9, 0x50A114A2, 0xB1F63FE7, 0xA20C3635, 0x01B0FF0F, 0x8DA9B815, 0x9415C8FE,
    0x784708DB, 0xBB4DCAF4, 0x54AE95E2, 0xE35BE753, 0x5A2156DA, 0x225B78D0, 0xD10C3DCF,
    0xAF4C2041, 0x42703C7A, 0x31400213, 0x0010AE8B, 0x93CE79CD, 0x59CAA4A2, 0x9B3AE790,
    0xB0FE5390, 0xE47C4468, 0xFD7F76E3, 0xC1BA35C7, 0x82C47F8D, 0x87D49817, 0xC99B636E,
    0x1FE3E9D9, 0x6C85DCA2, 0x7FDBCB69, 0x443C76B8, 0x560AA234, 0xAD01DA8B, 0x38BC2583,
    0xE0A81482, 0x81041B00, 0x81D00208, 0x83A28811, 0x2C0448A1, 0x504E80AE, 0x82CDC3B6,
    0x851B0344, 0x863FE581, 0x716B05F0, 0x8DC5B043, 0x5BE77CAF, 0x8786F7C0, 0xEF52D6BD,
    0x1E430B79, 0x14A28C4F, 0x0E416860, 0xEC9B3078, 0xFF21377A, 0xF0180003, 0x14030720,
    0x99E417E4, 0xF1062203, 0x4130160F, 0x05589D88, 0xB5B91E2A, 0x1E63D078, 0x7C710DF4,
    0x760ED1D5, 0x0A6AED60, 0xA7880EF4, 0x6F0CA036, 0x9442C2D8, 0xC3C98530, 0x63207692,
    0x02C05509, 0x04006A05, 0xBBA00C21, 0x824819B4, 0x4C09D117, 0xFC20A0EC, 0xA8354670,
    0x4A42C831, 0xF8660D81, 0x21E43C07, 0xB3F1018F, 0x05A3BE09, 0x62717A30, 0x80997B11,
    0x1B600164, 0xDD578104, 0x285F56FE, 0x203085C6, 0x903504FC, 0x23649BA1, 0xFCAB94CF,
    0xD7AB28E4, 0x7260D619, 0x0861FA41, 0x0A423846, 0x8181FCBE, 0x84802091, 0x0017B968,
    0xD03A8728, 0x08F20921, 0xC828588B, 0xCAC83FD0, 0xE2C57D0A, 0x4322C592, 0x2CFA8017,
    0x0F620435, 0x97EFD8FF, 0x7EC90CBD, 0xA01203FC, 0x40E784E8, 0x4F250FF7, 0x93485F43,
    0x61021ADF, 0x411A9844, 0xE5905B0C, 0x2FA1702E, 0x7EF59451, 0x80F47B69, 0x6C0D71AA,
    0x3F068284, 0x80030218, 0x348A005C, 0x68DF20A0, 0xA0923FE0, 0xEB56F5E9, 0x0720DF1F,
    0xCF3770DC, 0x13822C7F, 0x1E07F898, 0x570FF09D, 0xA3D782E8, 0xF03DAF8E, 0xFC7F8F71,
    0x308D1241, 0xF1FF2224, 0x2318648B, 0xC5F65A77, 0x84F6403F, 0xDC02287F, 0x3A8730FF,
    0x713B91D2, 0xFF1CA398, 0x8DAE75B8, 0xCC7869B5, 0x2CAACAA3, 0xC0D21807, 0x00D01406,
    0x00404308, 0x1E207A45, 0x5E0B4140, 0x3A972870, 0x708E21BB, 0xFB1F75CC, 0xA75FF748,
    0x731932CC, 0xB0AA1FFA, 0x88156985, 0x3C907FD8, 0xC38CEA3C, 0x0FF5DEB8, 0x1F64178F,
    0x2544988D, 0x1D02E0B6, 0x530100F4, 0xF36BC003, 0x2882D221, 0x78971362, 0xC16A2A39,
    0x3D5B952B, 0xC740E476, 0x8641FE39, 0x054CA83D, 0x29C4C3C0, 0xD8F57148, 0x30579D4D,
    0x5A80D3F5, 0x38598B21, 0x5043D902, 0x60024140, 0x32D84FB5, 0x03E866E5, 0xAD25A01E,
    0x3E63706A, 0x15BC0F65, 0x388572FF, 0x3BD801AC, 0x1C24C403, 0x0AC4B5A1, 0x9F7E4B1E,
    0x91FEBACF, 0xFA97BDF7, 0xC749291F, 0x50908E2D, 0x26E32C0D, 0x3F55C810, 0x286DD180,
    0xF10C61A8, 0xD2B20312, 0x8FC79EB3, 0x3BA6A75A, 0xF099857D, 0x420034AC, 0xB150FF0F,
    0xFAC761EB, 0xF4CB23D4, 0x6EBDEFA8, 0x260D41F7, 0x2A0B2158, 0xBA168280, 0xEF9401E0,
    0x79ED1AE3, 0x04ACCD8F, 0xB4883C86, 0xC7B0F61E, 0x0D8AE5EE, 0x1FE3746F, 0x27851092,
    0x4B8841FE, 0xFFE19A34, 0xFEB44178, 0xB83A05B1, 0xDEDD1FE1, 0xAFB0EC7E, 0x5F5830FD,
    0xEC0A566D, 0x8EA204EB, 0x0B616027, 0x6405B034, 0x10250670, 0x0E30FD49, 0x340FF418,
    0xF04B0520, 0x08306D0F, 0x660FF07E, 0x944C7F26, 0x6D37DE0D, 0x8605E065, 0x14DA5294,
    0x05503627, 0xA866D4DC, 0x47FB4487, 0x8133B181, 0xC981006E, 0x96A11BD2, 0x5920D70D,
    0x3C05C7CA, 0xB0678020, 0x0ED80602, 0x3064C678, 0xB0770FF6, 0x4A908608, 0xCE0FF48D,
    0xC0FF0774, 0x06106704, 0x6F7A60FF, 0x581A6D46, 0x7811F71F, 0xF20996DF, 0xD26D4896,
    0x0FF27726, 0x812F72C2, 0x46D384E7, 0x84A81A06, 0x4704D0FF, 0x705F0FF8, 0x0F52D907,
    0xE47DA0F7, 0x383F83D1, 0x0CD3FA3A, 0x056AF0D4, 0x18310FE1, 0x83683404, 0x79838490,
    0xC4427B36, 0x48E84086, 0x63845843, 0x884C8608, 0x76584F1F, 0x5209D08C, 0x826C4AE8,
    0x05488D23, 0x8D0FF853, 0xB85209E0, 0x8851F985, 0x48862860, 0x98678658, 0x87E20386,
    0x700FF86E, 0x41028728, 0x05C05A87, 0x26834042, 0x98088060, 0x80C80A34, 0x1382A80E,
    0xD40A8268, 0x04B17F1E, 0x1B060067, 0xD5474898, 0x0D869B85, 0x058B45C4, 0xF82A8284,
    0x82F82D1B, 0x310F7105, 0x08340408, 0x0D87CE02, 0xFF7F87D1, 0x97D77D50, 0x44C8C27D,
    0xAC1E1581, 0x20910FF4, 0x7DD0FF7E, 0x0B8D77DF, 0x10FF7F44, 0x52207806, 0x5205903C,
    0xA8251FC0, 0x7FF04E0E, 0x66063833, 0x848D0130, 0x0E90C10F, 0xB2849625, 0xE7AA0477,
    0x8E27AD1F, 0xBD78F900, 0x27BF2F27, 0x5C77BD7B, 0x597C60D6, 0x20590330, 0x00306506,
    0x350A24F9, 0x077C7957, 0x90177F1F, 0x91781918, 0x62F082B7, 0x4A74CF06, 0xE40E36A6,
    0x50BB0E50, 0x05305004, 0x1D552028, 0x40C30BF2, 0x10577B0C, 0x6753076A, 0xA9349197,
    0x76993372, 0xA9203262, 0x60B00AE0, 0x03F03309, 0x3301D042, 0x121A03E0, 0x09809709,
    0xFA1023A5, 0x20C40BA0, 0x2CB7E90C, 0xEA2DD73A, 0x71EF57D4, 0x62293577, 0x72954813,
    0xD9159515, 0x10246025, 0x9008A0FB, 0x50690920, 0x01C03D74, 0x000D1138, 0x408107B0,
    0x66601108, 0xF00EE0E2, 0x69041810, 0x0E00FF4E, 0xB90E40E5, 0x17047023, 0x03775126,
    0xAF0FF820, 0x19576BB0, 0x49869762, 0xE4054037, 0x90E56C26, 0x7050E80E, 0xFD709707,
    0x32C88FC8, 0x0F70FF67, 0x340E7998, 0x703503D8, 0x0DC01D5D, 0x2E0DF0DE, 0x321D0472,
    0x0E50E50E, 0x85105751, 0x80930980, 0x14A69233, 0xB02BD4D3, 0x10469840, 0x02E6B906,
    0x78444541, 0x54548095, 0x65B0B573, 0x8E07A0FF, 0x50FF0890, 0x08508A07, 0xC983E83C,
    0xD87E83F9, 0x87E9CA9C, 0xC29C89C6, 0xC9BF9C49, 0x45473565, 0x291F31F1, 0x69B88118,
    0x6E697604, 0xB02FA0B7, 0xB0FF7889, 0x10585A9A, 0xD10D10CE, 0x293F0AA0, 0x64002704,
    0x004F9019, 0x20800790, 0x08700008, 0xCF09008E, 0x50CD8A52, 0x5EA6AF0D, 0x546527C0,
    0x21552036, 0x66F06B4E, 0x5561066A, 0x158B57F3, 0x58485789, 0x1157B579, 0x5A104DCA,
    0xA16A12A1, 0x8C8574DC, 0xC5BCA0D8, 0x4E394E94, 0x8052B66F, 0x7A01A031, 0x87387B67,
    0x8808607F, 0xF0300660, 0x6400204B, 0xA3109013, 0x116A0A80, 0x04E04B04, 0x730EF00B,
    0x32DA95F9, 0x62E83B2E, 0xF31E748F, 0x05C52FC0, 0x97A0E40E, 0xD803A367, 0x5A4C88F4,
    0xA4E88E85, 0x4DA50890, 0x9A4A88EA, 0xA463674D, 0x435BC97A, 0x61E64FAA, 0xA401E46A,
    0xE173A83B, 0x69231E42, 0x0460CE0E, 0x33053051, 0xF04499D0, 0x6CF143A2, 0x00161318,
    0x20300210, 0x0A500003, 0x3E0AB0A9, 0x35E85E63, 0x4B105748, 0x26724367, 0x00C865E7,
    0x0404370D, 0x3505605C, 0x1A538EA3, 0xA8FA4FA5, 0x8DA91A8E, 0xB8E9A4CA, 0xA8AA888E,
    0x5E437A85, 0x933CA826, 0x6A1A7F5F, 0x941E4A7D, 0x609C09A0, 0x03C60207, 0x495DA024,
    0xA64001A0, 0x0B531871, 0x39028150, 0x904C13D0, 0x20500B63, 0x051870F1, 0x089408C1,
    0x9BA3FA6C, 0xF45BE0FF, 0xD3FBA995, 0xA953C305, 0xCAA52A93, 0x8A93A90A, 0xAC6041AC,
    0xF3AC40FF, 0x845874B5, 0x0FF41457, 0x05895ABC, 0x90E71281, 0x04D0420C, 0x37034050,
    0x30450420, 0x01064002, 0xAF4D1640, 0x902F15EA, 0x02E37D04, 0x62000603, 0x23FA0929,
    0x56A0F710, 0x5A7EE05C, 0x60964424, 0x5F40A20A, 0xCA64C2BF, 0xBA8B0554, 0xA92ACB8E,
    0x8EAC9B07, 0x8B03AC88, 0x5F267A2F, 0x40AFFAFD, 0xAAFA58E4, 0x07B1E256, 0x60084082,
    0xF040A2D0, 0x01B1439E, 0x4000A640, 0xF4720016, 0x03900038, 0x01049048, 0x500092A0,
    0x0C90C80C, 0x1F102952, 0xD04482F5, 0x0A48BD45, 0xB16710B1, 0x94FA1DE5, 0x0327EFAF,
    0xFF31E81C, 0x7B44B420, 0x256B4533, 0x46B43B47, 0x9B40337B, 0x7B9B3E48, 0xE05BC4E1,
    0x580D81B5, 0xB3333EB3, 0xBB0B6203, 0xC0930BB0, 0x02A04363, 0xA9140039, 0x0011143A,
    0x64000464, 0x1C03159F, 0xA0542AD2, 0x03C03224, 0x26001040, 0x100D0539, 0x0E30E30E,
    0x5F1059E1, 0xD0576FB4, 0x2700366F, 0x465C5670, 0x37EC0DB1, 0xAC87EF07, 0x0805A03E,
    0xEB09ACCB, 0xAC88902A, 0xFFB8605B, 0xC5C5B830, 0x271B7F4F, 0x3A45E6FC, 0xB0D92034,
    0x0C00DC0D, 0x34A37040, 0x80437170, 0x01864002, 0x4813FAAD, 0x5B680010, 0x2AC1CBB2,
    0x4A04903B, 0xB0310010, 0x00103F03, 0x7807606E, 0x51DF0270, 0x102A9A0F, 0xB164D4B3,
    0x380D5284, 0x0A00A409, 0x725AF5C6, 0xD2F84AA5, 0x0CD0C517, 0xC60FF0CB, 0x20CC0CE0,
    0xBD6BD4BD, 0xD9BD3BD5, 0xFBDABD7B, 0xBD00C40F, 0xCC87B6AD, 0xA5C74EAB, 0xB54BC739,
    0x814B2BC2, 0xD12D2034, 0x05E0D50E, 0x4706A067, 0x904050C0, 0x01E64002, 0x536CB640,
    0xABB10025, 0x59F1B304, 0x3C05C236, 0x2AAA4762, 0x03E46800, 0xF5078002, 0x85BE0329,
    0xB7B1028C, 0x5B615549, 0xD57880F4, 0x98544A57, 0xB13AE580, 0x658D8118, 0xA3CB9A52,
    0xF30F27B9, 0x306C0DB0, 0x04E07607, 0x4002A681, 0xAAE70226, 0x04704303, 0x0646B005,
    0xD05203C3, 0x5184C655, 0x8230B4C8, 0xF2A322F3, 0xAF0004C2, 0x7700403C, 0x508007E0,
    0x0F10F003, 0x478870F2, 0x758F5253, 0x3A15ECAD, 0x1661B16C, 0xA258C18B, 0x5EBC5156,
    0x8FAD76AC, 0xA2036B15, 0x0D65CF0E, 0x7507306B, 0x6C0704E0, 0x025143BF, 0x072A4A30,
    0x9001BFD0, 0x42A5153D, 0x2842DB25, 0x759D2464, 0x005C2FB2, 0x3E03B030, 0xF0670060,
    0x02E07106, 0xDF0DF0DD, 0xE1024B40, 0x88965570, 0x480FF09A, 0x783B4B12, 0x0A00FF4F,
    0x9C4D30AF, 0x4A9C8E2A, 0x4E5A0669, 0xD8C8B83B, 0x6C8909DA, 0x0D7203C8, 0xBF0DA0D9,
    0x806605D0, 0x4BE04806, 0x4002B040, 0x5143C686, 0x0082A401, 0xB304803B, 0xE2B0B89B,
    0xC7405E03, 0x59C37387, 0xB31BC721, 0xBAC2A2C3, 0x3D031C77, 0x30080400, 0x01AAE004,
    0xBD0BC0B8, 0x456808A0, 0x17D2531E, 0x8CC8AA62, 0x533B3F8A, 0x2DD2E44F, 0xDD809B53,
    0x378D3506, 0x73535B57, 0xDDCDC62C, 0xBB538096, 0xCD62DD2D, 0xB1CD2CD4, 0xF2E12DB4,
    0x0F52BDCC, 0xB80B70B3, 0xA0400940, 0x03404E04, 0x43C2ABF4, 0x4143AE61, 0xCB1C6A01,
    0x7655804A, 0xF426C6E4, 0xC3855EC6, 0x0A383560, 0xC59DD05D, 0x005140BA, 0x0A0426C8,
    0x00445D60, 0x08307B01, 0xE204E085, 0x8ABA9220, 0x53887025, 0x752F72F5, 0x12EC7EA9,
    0xCD679354, 0xE42E705F, 0x92E34082, 0x864CEC40, 0x93CD6D2A, 0x1D245307, 0xD212F32F,
    0xFA5378A2, 0xE0E09A12, 0x08107F92, 0x2CBA405E, 0x8CFABA40, 0x64002104, 0xB2CFF012,
    0x2D03C6DC, 0xCB846F38, 0x74C7159E, 0xD311D0B4, 0x047D0E3D, 0x47B61003, 0xFA2D00B0,
    0x04300F03, 0x2205104E, 0xF09E0980, 0x0E507009, 0x03BBF128, 0xB32C3C72, 0x09A088B7,
    0x7E067263, 0xD05377B0, 0x26D65106, 0x4A272BC4, 0xA7603FA0, 0x26DD793F, 0x72651D75,
    0x3D6F77BD, 0xD6CB7B26, 0xE59A62FA, 0xB0950D10, 0x07A09C09, 0x4F04C042, 0x0D5D0350,
    0xBA6140B6, 0x4001C143, 0x7C2E00F6, 0xC6C00604, 0x02CB5559, 0x74C5D0BC, 0x0AE14951,
    0x221B91B7, 0xF1C72252, 0xB6A2212E, 0x4502E21D, 0x602D2930, 0xD1100274, 0x45AE3007,
    0x1AA700E0, 0x05304901, 0x81028056, 0x008A0880, 0x0D19E906, 0x990F60A8, 0xD2BD73F9,
    0x40E0F00E, 0xE10E20DC, 0x35393281, 0x0CC5380D, 0xD40FFDD6, 0x134A0DB0, 0xDCE1E1DD,
    0xFB3B140E, 0x80F67D90, 0x0CF0CC0E, 0x800AB0D0, 0x90890870, 0x05479C06, 0x29AA7038,
    0x8C66DBA0, 0x048A6E04, 0x0AD5A015, 0xF03DDB50, 0xBB703FC3, 0x1E3E7030, 0x4DAE21D2,
    0x2292271C, 0x3C22D22B, 0xF103C70C, 0x0E30E50D, 0x0019FA77, 0x60BA0AC0, 0xE15E130B,
    0x14E16E17, 0xBE18E1AE, 0xE13E19E1, 0x130B70BB, 0x00B50B7E, 0x07406C00, 0x43BAC076,
    0xB143B671, 0x010B5E00, 0xF4014681, 0x305901EB, 0x03806506, 0x9309208B, 0x40A40670,
    0x0E1094A3, 0xB90E40E2, 0xB54C74B0, 0x62010534, 0xF0E483B0, 0x95AE0F97, 0xE400E00D,
    0xA9A330BA, 0x108A0970, 0x06C09209, 0x63061058, 0xDBF40410, 0x02768102, 0x48B1EB5E,
    0x8143C2C0, 0x00E143BA, 0xBF00692A, 0x4002056D, 0x0AC0AC0A, 0x1EE22E20, 0x8E1DE73E,
    0x098089E1, 0x491AC095, 0x21B01410, 0x0BDB681B, 0x000C20C1, 0x70C60C30, 0x0CA0000C,
    0x000CE0CC, 0xB3730C90, 0xE8CE8DE8, 0x91E8E0CD, 0x5E92E90E, 0xE88E94E9, 0xB90B4E91,
    0x996B0BA0, 0x944307B1, 0x05048E2A, 0x1640B210, 0x016A6C01, 0xA8018966, 0xF04501BC,
    0x02905204, 0xEF03790A, 0xF04005B8, 0x05C05904, 0x4A7CB926, 0xB05D7CA0, 0x05CEB604,
    0x43EB3046, 0x403C90A0, 0x031EAE04, 0x66025681, 0x6D140259, 0x640C2B02, 0x40CFDB20,
    0x764000D6, 0x003D1400, 0x0105BEB6, 0x10AD0A80, 0x0CA0C75E, 0x8FE960CB, 0xDE93EE3E,
    0x0A009CE8, 0x691BB0A2, 0xE5DC2324, 0x0001015D, 0x00000000

  EW_BITMAP_CLUT_EMPTY( CutterPaperSpeed, Default )

  EW_BITMAPS_TABLE( CutterPaperSpeed )
    EW_BITMAP( CutterPaperSpeed, Default )
EW_END_OF_BITMAP_RES( CutterPaperSpeed )

/* Table with links to derived variants of the bitmap resource : 'Cutter::PaperSpeed' */
EW_RES_WITHOUT_VARIANTS( CutterPaperSpeed )

/* Initializer for the class 'Cutter::Cutter' */
void CutterCutter__Init( CutterCutter _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Background, &_this->_XObject, 0 );
  CutterFeedWindow__Init( &_this->FeedWindow, &_this->_XObject, 0 );
  CutterFormatWindow__Init( &_this->FormatWindow, &_this->_XObject, 0 );
  CutterBatchWindow__Init( &_this->BatchWindow, &_this->_XObject, 0 );
  CutterRemainingWindow__Init( &_this->RemainingWindow, &_this->_XObject, 0 );
  CutterSpeedWindow__Init( &_this->SpeedWindow, &_this->_XObject, 0 );
  CutterMainButton__Init( &_this->MainButton, &_this->_XObject, 0 );
  CutterStatusBar__Init( &_this->StatusBar, &_this->_XObject, 0 );
  CutterPaperCut__Init( &_this->PaperCut, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->AutoDemoTimer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterCutter );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const001C );
  CoreRectView__OnSetBounds( &_this->Background, _Const001D );
  ViewsRectangle_OnSetColor( &_this->Background, _Const001E );
  CoreRectView__OnSetBounds( &_this->FeedWindow, _Const001F );
  CoreRectView__OnSetBounds( &_this->FormatWindow, _Const0020 );
  CoreRectView__OnSetBounds( &_this->BatchWindow, _Const0021 );
  CoreRectView__OnSetBounds( &_this->RemainingWindow, _Const0022 );
  CoreRectView__OnSetBounds( &_this->SpeedWindow, _Const0023 );
  CoreTimer_OnSetPeriod( &_this->AutoDemoTimer, 1000 );
  CoreTimer_OnSetBegin( &_this->AutoDemoTimer, 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Background ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->FeedWindow ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->FormatWindow ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BatchWindow ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->RemainingWindow ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SpeedWindow ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->MainButton ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->StatusBar ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->PaperCut ), 0 );
  _this->FeedWindow.Super1.OnChange = EwNewSlot( _this, CutterCutter_ChangeFeed );
  _this->FeedWindow.Super1.OnCancel = EwNewSlot( _this, CutterCutter_ProgrammingMode );
  CutterFeedWindow_OnSetPaperType( &_this->FeedWindow, EwNewRef( EwGetAutoObject( 
  &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetPaperType, CutterDeviceClass_OnSetPaperType ));
  _this->FormatWindow.Super1.OnChange = EwNewSlot( _this, CutterCutter_ChangeFormat );
  _this->FormatWindow.Super1.OnCancel = EwNewSlot( _this, CutterCutter_ProgrammingMode );
  CutterFormatWindow_OnSetPaperWidth( &_this->FormatWindow, EwNewRef( EwGetAutoObject( 
  &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetPaperWidth, CutterDeviceClass_OnSetPaperWidth ));
  CutterFormatWindow_OnSetPaperHeight( &_this->FormatWindow, EwNewRef( EwGetAutoObject( 
  &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetPaperHeight, CutterDeviceClass_OnSetPaperHeight ));
  _this->Device = EwGetAutoObject( &CutterDevice, CutterDeviceClass );
  _this->BatchWindow.Super1.OnChange = EwNewSlot( _this, CutterCutter_ChangeBatch );
  _this->BatchWindow.Super1.OnCancel = EwNewSlot( _this, CutterCutter_ProgrammingMode );
  CutterBatchWindow_OnSetPaperBatch( &_this->BatchWindow, EwNewRef( EwGetAutoObject( 
  &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetPaperBatch, CutterDeviceClass_OnSetPaperBatch ));
  _this->RemainingWindow.Super1.OnCancel = EwNewSlot( _this, CutterCutter_ProgrammingMode );
  CutterRemainingWindow_OnSetPaperRemaining( &_this->RemainingWindow, EwNewRef( 
  EwGetAutoObject( &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetRemaining, 
  CutterDeviceClass_OnSetRemaining ));
  _this->SpeedWindow.Super1.OnChange = EwNewSlot( _this, CutterCutter_ChangeSpeed );
  _this->SpeedWindow.Super1.OnCancel = EwNewSlot( _this, CutterCutter_ProgrammingMode );
  CutterSpeedWindow_OnSetPaperSpeed( &_this->SpeedWindow, EwNewRef( EwGetAutoObject( 
  &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetPaperSpeed, CutterDeviceClass_OnSetPaperSpeed ));
  _this->MainButton.OnStart = EwNewSlot( _this, CutterCutter_RunningMode );
  _this->MainButton.OnStop = EwNewSlot( _this, CutterCutter_ProgrammingMode );
  _this->MainButton.OnOk = EwNewSlot( _this, CutterCutter_ProgrammingMode );
  CutterPaperCut_OnSetPaperWidth( &_this->PaperCut, EwNewRef( EwGetAutoObject( &CutterDevice, 
  CutterDeviceClass ), CutterDeviceClass_OnGetPaperWidth, CutterDeviceClass_OnSetPaperWidth ));
  CutterPaperCut_OnSetPaperHeight( &_this->PaperCut, EwNewRef( EwGetAutoObject( 
  &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetPaperHeight, CutterDeviceClass_OnSetPaperHeight ));
  _this->AutoDemoTimer.OnTrigger = EwNewSlot( _this, CutterCutter_AutoDemoSlot );

  /* Call the user defined constructor */
  CutterCutter_Init( _this, aArg );
}

/* Re-Initializer for the class 'Cutter::Cutter' */
void CutterCutter__ReInit( CutterCutter _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Background );
  CutterFeedWindow__ReInit( &_this->FeedWindow );
  CutterFormatWindow__ReInit( &_this->FormatWindow );
  CutterBatchWindow__ReInit( &_this->BatchWindow );
  CutterRemainingWindow__ReInit( &_this->RemainingWindow );
  CutterSpeedWindow__ReInit( &_this->SpeedWindow );
  CutterMainButton__ReInit( &_this->MainButton );
  CutterStatusBar__ReInit( &_this->StatusBar );
  CutterPaperCut__ReInit( &_this->PaperCut );
  CoreTimer__ReInit( &_this->AutoDemoTimer );
}

/* Finalizer method for the class 'Cutter::Cutter' */
void CutterCutter__Done( CutterCutter _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterCutter );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Background );
  CutterFeedWindow__Done( &_this->FeedWindow );
  CutterFormatWindow__Done( &_this->FormatWindow );
  CutterBatchWindow__Done( &_this->BatchWindow );
  CutterRemainingWindow__Done( &_this->RemainingWindow );
  CutterSpeedWindow__Done( &_this->SpeedWindow );
  CutterMainButton__Done( &_this->MainButton );
  CutterStatusBar__Done( &_this->StatusBar );
  CutterPaperCut__Done( &_this->PaperCut );
  CoreTimer__Done( &_this->AutoDemoTimer );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void CutterCutter_UpdateLayout( CutterCutter _this, XPoint aSize )
{
  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );
  CoreRectView__OnSetBounds( &_this->Background, EwNewRect( 0, 0, aSize.X, aSize.Y ));
  CoreRectView__OnSetBounds( &_this->MainButton, EwNewRect( aSize.X - EwGetRectW( 
  _this->MainButton.Super2.Bounds ), aSize.Y - EwGetRectH( _this->MainButton.Super2.Bounds ), 
  aSize.X, aSize.Y ));
  CoreRectView__OnSetBounds( &_this->StatusBar, EwNewRect( 0, aSize.Y - EwGetRectH( 
  _this->StatusBar.Super2.Bounds ), EwGetRectW( _this->StatusBar.Super2.Bounds ), 
  aSize.Y ));
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void CutterCutter_Init( CutterCutter _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  CutterCutter_OnSetPaperRemaining( _this, EwNewRef( EwGetAutoObject( &CutterDevice, 
  CutterDeviceClass ), CutterDeviceClass_OnGetRemaining, CutterDeviceClass_OnSetRemaining ));
  EwSignal( EwNewSlot( _this, CutterCutter_ProgrammingMode ), ((XObject)_this ));
}

/* The slot method 'RunningMode' puts all members of the cutter into running mode. 
   The running mode is active, when the user has pressed the start button. */
void CutterCutter_RunningMode( CutterCutter _this, XObject sender )
{
  XInt32 x;
  XInt32 y;
  XInt32 w;
  XInt32 h;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  x = CutterBorder.X;
  y = CutterBorder.Y;
  w = (( CutterScreenSize.X - ( 2 * CutterBorder.X )) - ( 2 * CutterSpacing.X )) 
  / 3;
  h = ((( CutterScreenSize.Y - CutterBorder.X ) - CutterSpacing.Y ) - 32 ) / 2;
  CutterWindow__ChangeMode( &_this->FeedWindow, CutterOperationModeRunning, EwNewRect( 
  x, y, x + w, y + h ));
  x = ( x + w ) + CutterSpacing.X;
  CutterWindow__ChangeMode( &_this->SpeedWindow, CutterOperationModeRunning, EwNewRect( 
  x, y, (( x + w ) + w ) + CutterSpacing.X, y + h ));
  x = CutterBorder.X;
  y = ( y + h ) + CutterSpacing.Y;
  CutterWindow__ChangeMode( &_this->FormatWindow, CutterOperationModeRunning, EwNewRect( 
  x, y, x + w, y + h ));
  x = ( x + w ) + CutterSpacing.X;
  CutterWindow__ChangeMode( &_this->BatchWindow, CutterOperationModeRunning, EwNewRect( 
  x, y, x + w, y + h ));
  x = ( x + w ) + CutterSpacing.X;
  CutterWindow__ChangeMode( &_this->RemainingWindow, CutterOperationModeRunning, 
  EwNewRect( x, y, x + w, y + h ));
  CutterMainButton_OnSetMode( &_this->MainButton, CutterButtonModeStop );
  CutterDeviceClass_OnSetRunning( _this->Device, 1 );
}

/* The slot method 'ProgrammingMode' puts all members of the cutter into programming 
   mode. The programming mode is active, when the user has pressed the stop button. */
void CutterCutter_ProgrammingMode( CutterCutter _this, XObject sender )
{
  XInt32 x;
  XInt32 y;
  XInt32 w;
  XInt32 h;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  x = CutterBorder.X;
  y = CutterBorder.Y;
  w = (( CutterScreenSize.X - ( 2 * CutterBorder.X )) - CutterSpacing.X ) / 2;
  h = ((( CutterScreenSize.Y - CutterBorder.X ) - CutterSpacing.Y ) - 32 ) / 2;
  CutterWindow__ChangeMode( &_this->FeedWindow, CutterOperationModeProgramming, 
  EwNewRect( x, y, x + w, y + h ));
  x = ( x + w ) + CutterSpacing.X;
  CutterWindow__ChangeMode( &_this->SpeedWindow, CutterOperationModeProgramming, 
  EwNewRect( x, y, x + w, y + h ));
  x = CutterBorder.X;
  y = ( y + h ) + CutterSpacing.Y;
  CutterWindow__ChangeMode( &_this->FormatWindow, CutterOperationModeProgramming, 
  EwNewRect( x, y, x + w, y + h ));
  x = ( x + w ) + CutterSpacing.X;
  CutterWindow__ChangeMode( &_this->BatchWindow, CutterOperationModeProgramming, 
  EwNewRect( x, y, x + w, y + h ));
  CutterWindow__ChangeMode( &_this->RemainingWindow, CutterOperationModeProgramming, 
  EwNewRect( CutterScreenSize.X, y, CutterScreenSize.X + w, y + h ));
  CutterMainButton_OnSetMode( &_this->MainButton, CutterButtonModeStart );
  CutterDeviceClass_OnSetRunning( _this->Device, 0 );
  EwPostSignal( EwNewSlot( &_this->PaperCut, CutterPaperCut_FadeOut ), ((XObject)_this ));
}

/* The slot method 'ChangeFormat' puts the format window into exclusive mode. This 
   exclusive mode is active, when the user has pressed the window area of the format 
   window. */
void CutterCutter_ChangeFormat( CutterCutter _this, XObject sender )
{
  XInt32 x;
  XInt32 y;
  XInt32 w;
  XInt32 h;
  XInt32 dx;
  XInt32 dy;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  x = CutterBorder.X;
  y = CutterBorder.Y;
  w = CutterScreenSize.X - ( 2 * CutterBorder.X );
  h = ( CutterScreenSize.Y - CutterBorder.X ) - 32;
  CutterWindow__ChangeMode( &_this->FormatWindow, CutterOperationModeExclusive, 
  EwNewRect( x, y, x + w, y + h ));
  dx = CutterScreenSize.X - _this->FormatWindow.Super3.Bounds.Point2.X;
  dy = -_this->FormatWindow.Super3.Bounds.Point1.Y;
  CutterWindow__ChangeMode( &_this->FeedWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->FeedWindow.Super3.Bounds.Point1.X + dx, _this->FeedWindow.Super3.Bounds.Point1.Y 
  + dy, _this->FeedWindow.Super3.Bounds.Point2.X + dx, _this->FeedWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->SpeedWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->SpeedWindow.Super3.Bounds.Point1.X + dx, _this->SpeedWindow.Super3.Bounds.Point1.Y 
  + dy, _this->SpeedWindow.Super3.Bounds.Point2.X + dx, _this->SpeedWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->BatchWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->BatchWindow.Super3.Bounds.Point1.X + dx, _this->BatchWindow.Super3.Bounds.Point1.Y 
  + dy, _this->BatchWindow.Super3.Bounds.Point2.X + dx, _this->BatchWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->RemainingWindow, CutterOperationModeProgramming, 
  EwNewRect( CutterScreenSize.X, _this->RemainingWindow.Super3.Bounds.Point1.Y, 
  CutterScreenSize.X + EwGetRectW( _this->RemainingWindow.Super3.Bounds ), _this->RemainingWindow.Super3.Bounds.Point2.Y ));
  CutterMainButton_OnSetMode( &_this->MainButton, CutterButtonModeOk );
  CutterDeviceClass_OnSetRunning( _this->Device, 0 );
  CoreRectView__OnSetBounds( &_this->PaperCut, EwSetRectOrigin( _this->PaperCut.Super2.Bounds, 
  EwNewPoint( CutterBorder.X * 2, 0 )));
  EwPostSignal( EwNewSlot( &_this->PaperCut, CutterPaperCut_FadeIn ), ((XObject)_this ));
}

/* The slot method 'ChangeFeed' puts the feed window into exclusive mode. This exclusive 
   mode is active, when the user has pressed the window area of the feed window. */
void CutterCutter_ChangeFeed( CutterCutter _this, XObject sender )
{
  XInt32 x;
  XInt32 y;
  XInt32 w;
  XInt32 h;
  XInt32 dx;
  XInt32 dy;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  x = CutterBorder.X;
  y = CutterBorder.Y;
  w = CutterScreenSize.X - ( 2 * CutterBorder.X );
  h = ( CutterScreenSize.Y - CutterBorder.X ) - 32;
  CutterWindow__ChangeMode( &_this->FeedWindow, CutterOperationModeExclusive, EwNewRect( 
  x, y, x + w, y + h ));
  dx = CutterScreenSize.X - _this->FeedWindow.Super3.Bounds.Point2.X;
  dy = CutterScreenSize.Y - _this->FeedWindow.Super3.Bounds.Point2.Y;
  CutterWindow__ChangeMode( &_this->FormatWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->FormatWindow.Super3.Bounds.Point1.X + dx, _this->FormatWindow.Super3.Bounds.Point1.Y 
  + dy, _this->FormatWindow.Super3.Bounds.Point2.X + dx, _this->FormatWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->SpeedWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->SpeedWindow.Super3.Bounds.Point1.X + dx, _this->SpeedWindow.Super3.Bounds.Point1.Y 
  + dy, _this->SpeedWindow.Super3.Bounds.Point2.X + dx, _this->SpeedWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->BatchWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->BatchWindow.Super3.Bounds.Point1.X + dx, _this->BatchWindow.Super3.Bounds.Point1.Y 
  + dy, _this->BatchWindow.Super3.Bounds.Point2.X + dx, _this->BatchWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->RemainingWindow, CutterOperationModeProgramming, 
  EwNewRect( CutterScreenSize.X, _this->RemainingWindow.Super3.Bounds.Point1.Y, 
  CutterScreenSize.X + EwGetRectW( _this->RemainingWindow.Super3.Bounds ), _this->RemainingWindow.Super3.Bounds.Point2.Y ));
  CutterMainButton_OnSetMode( &_this->MainButton, CutterButtonModeOk );
  CutterDeviceClass_OnSetRunning( _this->Device, 0 );
}

/* The slot method 'ChangeSpeed' puts the speed window into exclusive mode. This 
   exclusive mode is active, when the user has pressed the window area of the speed 
   window. */
void CutterCutter_ChangeSpeed( CutterCutter _this, XObject sender )
{
  XInt32 x;
  XInt32 y;
  XInt32 w;
  XInt32 h;
  XInt32 dx;
  XInt32 dy;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  x = CutterBorder.X;
  y = CutterBorder.Y;
  w = CutterScreenSize.X - ( 2 * CutterBorder.X );
  h = ( CutterScreenSize.Y - CutterBorder.X ) - 32;
  CutterWindow__ChangeMode( &_this->SpeedWindow, CutterOperationModeExclusive, EwNewRect( 
  x, y, x + w, y + h ));
  dx = -_this->SpeedWindow.Super3.Bounds.Point1.X;
  dy = CutterScreenSize.Y - _this->SpeedWindow.Super3.Bounds.Point2.Y;
  CutterWindow__ChangeMode( &_this->FeedWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->FeedWindow.Super3.Bounds.Point1.X + dx, _this->FeedWindow.Super3.Bounds.Point1.Y 
  + dy, _this->FeedWindow.Super3.Bounds.Point2.X + dx, _this->FeedWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->FormatWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->FormatWindow.Super3.Bounds.Point1.X + dx, _this->FormatWindow.Super3.Bounds.Point1.Y 
  + dy, _this->FormatWindow.Super3.Bounds.Point2.X + dx, _this->FormatWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->BatchWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->BatchWindow.Super3.Bounds.Point1.X + dx, _this->BatchWindow.Super3.Bounds.Point1.Y 
  + dy, _this->BatchWindow.Super3.Bounds.Point2.X + dx, _this->BatchWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->RemainingWindow, CutterOperationModeProgramming, 
  EwNewRect( CutterScreenSize.X, _this->RemainingWindow.Super3.Bounds.Point1.Y, 
  CutterScreenSize.X + EwGetRectW( _this->RemainingWindow.Super3.Bounds ), _this->RemainingWindow.Super3.Bounds.Point2.Y ));
  CutterMainButton_OnSetMode( &_this->MainButton, CutterButtonModeOk );
  CutterDeviceClass_OnSetRunning( _this->Device, 0 );
}

/* The slot method 'ChangeBatch' puts the batch window into exclusive mode. This 
   exclusive mode is active, when the user has pressed the window area of the batch 
   window. */
void CutterCutter_ChangeBatch( CutterCutter _this, XObject sender )
{
  XInt32 x;
  XInt32 y;
  XInt32 w;
  XInt32 h;
  XInt32 dx;
  XInt32 dy;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  x = CutterBorder.X;
  y = CutterBorder.Y;
  w = CutterScreenSize.X - ( 2 * CutterBorder.X );
  h = ( CutterScreenSize.Y - CutterBorder.X ) - 32;
  CutterWindow__ChangeMode( &_this->BatchWindow, CutterOperationModeExclusive, EwNewRect( 
  x, y, x + w, y + h ));
  dx = -_this->BatchWindow.Super3.Bounds.Point1.X;
  dy = -_this->BatchWindow.Super3.Bounds.Point1.Y;
  CutterWindow__ChangeMode( &_this->FeedWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->FeedWindow.Super3.Bounds.Point1.X + dx, _this->FeedWindow.Super3.Bounds.Point1.Y 
  + dy, _this->FeedWindow.Super3.Bounds.Point2.X + dx, _this->FeedWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->FormatWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->FormatWindow.Super3.Bounds.Point1.X + dx, _this->FormatWindow.Super3.Bounds.Point1.Y 
  + dy, _this->FormatWindow.Super3.Bounds.Point2.X + dx, _this->FormatWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->SpeedWindow, CutterOperationModeProgramming, 
  EwNewRect( _this->SpeedWindow.Super3.Bounds.Point1.X + dx, _this->SpeedWindow.Super3.Bounds.Point1.Y 
  + dy, _this->SpeedWindow.Super3.Bounds.Point2.X + dx, _this->SpeedWindow.Super3.Bounds.Point2.Y 
  + dy ));
  CutterWindow__ChangeMode( &_this->RemainingWindow, CutterOperationModeProgramming, 
  EwNewRect( CutterScreenSize.X, _this->RemainingWindow.Super3.Bounds.Point1.Y, 
  CutterScreenSize.X + EwGetRectW( _this->RemainingWindow.Super3.Bounds ), _this->RemainingWindow.Super3.Bounds.Point2.Y ));
  CutterMainButton_OnSetMode( &_this->MainButton, CutterButtonModeOk );
  CutterDeviceClass_OnSetRunning( _this->Device, 0 );
}

/* 'C' function for method : 'Cutter::Cutter.OnSetPaperRemaining()' */
void CutterCutter_OnSetPaperRemaining( CutterCutter _this, XRef value )
{
  if ( !EwCompRef( value, _this->PaperRemaining ))
    return;

  if ( _this->PaperRemaining.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterCutter_onUpdateRemaining ), _this->PaperRemaining, 
      0 );

  _this->PaperRemaining = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterCutter_onUpdateRemaining ), value, 
      0 );

  EwPostSignal( EwNewSlot( _this, CutterCutter_onUpdateRemaining ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void CutterCutter_onUpdateRemaining( CutterCutter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperRemaining.Object != 0 )
    if ( EwOnGetInt32( _this->PaperRemaining ) == 0 )
      EwPostSignal( EwNewSlot( _this, CutterCutter_ProgrammingMode ), ((XObject)_this ));
}

/* Slot method to trigger the next phase of the auto demo. */
void CutterCutter_AutoDemoSlot( CutterCutter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  switch ( _this->AutoState )
  {
    case 1 :
      EwPostSignal( EwNewSlot( _this, CutterCutter_ChangeFeed ), ((XObject)_this ));
    break;

    case 2 :
      CutterDeviceClass_OnSetPaperType( _this->Device, 1 );
    break;

    case 3 :
      CutterDeviceClass_OnSetPaperType( _this->Device, 2 );
    break;

    case 4 :
      EwPostSignal( EwNewSlot( _this, CutterCutter_ProgrammingMode ), ((XObject)_this ));
    break;

    case 5 :
      EwPostSignal( EwNewSlot( _this, CutterCutter_ChangeSpeed ), ((XObject)_this ));
    break;

    case 6 :
      CutterDeviceClass_OnSetPaperSpeed( _this->Device, 2400 );
    break;

    case 7 :
      CutterDeviceClass_OnSetPaperSpeed( _this->Device, 2700 );
    break;

    case 8 :
      CutterDeviceClass_OnSetPaperSpeed( _this->Device, 3000 );
    break;

    case 9 :
      CutterDeviceClass_OnSetPaperSpeed( _this->Device, 3300 );
    break;

    case 10 :
      EwPostSignal( EwNewSlot( _this, CutterCutter_ProgrammingMode ), ((XObject)_this ));
    break;

    case 11 :
      EwPostSignal( EwNewSlot( _this, CutterCutter_ChangeFormat ), ((XObject)_this ));
    break;

    case 12 :
      CutterDeviceClass_OnSetPaperHeight( _this->Device, 2500 );
    break;

    case 13 :
      CutterDeviceClass_OnSetPaperHeight( _this->Device, 2300 );
    break;

    case 14 :
      CutterDeviceClass_OnSetPaperHeight( _this->Device, 2100 );
    break;

    case 15 :
      CutterDeviceClass_OnSetPaperWidth( _this->Device, 2000 );
    break;

    case 16 :
      CutterDeviceClass_OnSetPaperWidth( _this->Device, 1640 );
    break;

    case 17 :
      CutterDeviceClass_OnSetPaperWidth( _this->Device, 1480 );
    break;

    case 18 :
      EwPostSignal( EwNewSlot( _this, CutterCutter_ProgrammingMode ), ((XObject)_this ));
    break;

    case 19 :
      EwPostSignal( EwNewSlot( _this, CutterCutter_ChangeBatch ), ((XObject)_this ));
    break;

    case 20 :
      CutterDeviceClass_OnSetPaperBatch( _this->Device, 400 );
    break;

    case 21 :
      CutterDeviceClass_OnSetPaperBatch( _this->Device, 500 );
    break;

    case 22 :
      CutterDeviceClass_OnSetPaperBatch( _this->Device, 600 );
    break;

    case 23 :
      EwPostSignal( EwNewSlot( _this, CutterCutter_ProgrammingMode ), ((XObject)_this ));
    break;

    case 24 :
      EwPostSignal( EwNewSlot( _this, CutterCutter_RunningMode ), ((XObject)_this ));
    break;

    case 25 :
      CutterDeviceClass_OnSetPaperSpeed( _this->Device, 3200 );
    break;

    case 26 :
      CutterDeviceClass_OnSetPaperSpeed( _this->Device, 3100 );
    break;

    case 27 :
      CutterDeviceClass_OnSetPaperSpeed( _this->Device, 3000 );
    break;

    case 28 :
      CutterDeviceClass_OnSetPaperSpeed( _this->Device, 2900 );
    break;

    case 29 :
      CutterDeviceClass_OnSetPaperSpeed( _this->Device, 2800 );
    break;

    default : 
      ;
  }

  _this->AutoState = _this->AutoState + 1;

  if ( _this->AutoState > 29 )
    _this->AutoState = 0;
}

/* Variants derived from the class : 'Cutter::Cutter' */
EW_DEFINE_CLASS_VARIANTS( CutterCutter )
EW_END_OF_CLASS_VARIANTS( CutterCutter )

/* Virtual Method Table (VMT) for the class : 'Cutter::Cutter' */
EW_DEFINE_CLASS( CutterCutter, CoreGroup, Device, PaperRemaining, PaperRemaining, 
                 Background, AutoState, "Cutter::Cutter" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CutterCutter_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterCutter )

/* Constant to define the size of the entire paper cutter application. */
const XPoint CutterScreenSize = { 480, 272 };

/* Initializer for the class 'Cutter::DeviceClass' */
void CutterDeviceClass__Init( CutterDeviceClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->Timer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterDeviceClass );

  /* ... and initialize objects, variables, properties, etc. */
  _this->PaperBatch = 240;
  _this->PaperSpeed = 2100;
  _this->PaperWidth = 2100;
  _this->PaperHeight = 2970;
  _this->Remaining = 500;
  CoreTimer_OnSetPeriod( &_this->Timer, 250 );
  _this->Timer.OnTrigger = EwNewSlot( _this, CutterDeviceClass_Animate );
}

/* Re-Initializer for the class 'Cutter::DeviceClass' */
void CutterDeviceClass__ReInit( CutterDeviceClass _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->Timer );
}

/* Finalizer method for the class 'Cutter::DeviceClass' */
void CutterDeviceClass__Done( CutterDeviceClass _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterDeviceClass );

  /* Finalize all embedded objects */
  CoreTimer__Done( &_this->Timer );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Cutter::DeviceClass.OnSetPaperBatch()' */
void CutterDeviceClass_OnSetPaperBatch( CutterDeviceClass _this, XInt32 value )
{
  if ( value < 10 )
    value = 10;

  if ( value > 999 )
    value = 999;

  if ( value == _this->PaperBatch )
    return;

  _this->PaperBatch = value;
  EwNotifyRefObservers( EwNewRef( _this, CutterDeviceClass_OnGetPaperBatch, CutterDeviceClass_OnSetPaperBatch ), 
    0 );
}

/* 'C' function for method : 'Cutter::DeviceClass.OnSetPaperSpeed()' */
void CutterDeviceClass_OnSetPaperSpeed( CutterDeviceClass _this, XInt32 value )
{
  if ( value < 600 )
    value = 600;

  if ( value > 3600 )
    value = 3600;

  if ( value == _this->PaperSpeed )
    return;

  _this->PaperSpeed = value;
  EwNotifyRefObservers( EwNewRef( _this, CutterDeviceClass_OnGetPaperSpeed, CutterDeviceClass_OnSetPaperSpeed ), 
    0 );
}

/* 'C' function for method : 'Cutter::DeviceClass.OnSetPaperWidth()' */
void CutterDeviceClass_OnSetPaperWidth( CutterDeviceClass _this, XInt32 value )
{
  if ( value < 520 )
    value = 520;

  if ( value > 2970 )
    value = 2970;

  if ( value == _this->PaperWidth )
    return;

  _this->PaperWidth = value;
  EwNotifyRefObservers( EwNewRef( _this, CutterDeviceClass_OnGetPaperWidth, CutterDeviceClass_OnSetPaperWidth ), 
    0 );
}

/* 'C' function for method : 'Cutter::DeviceClass.OnSetPaperHeight()' */
void CutterDeviceClass_OnSetPaperHeight( CutterDeviceClass _this, XInt32 value )
{
  if ( value < 740 )
    value = 740;

  if ( value > 4200 )
    value = 4200;

  if ( value == _this->PaperHeight )
    return;

  _this->PaperHeight = value;
  EwNotifyRefObservers( EwNewRef( _this, CutterDeviceClass_OnGetPaperHeight, CutterDeviceClass_OnSetPaperHeight ), 
    0 );
}

/* 'C' function for method : 'Cutter::DeviceClass.OnSetRunning()' */
void CutterDeviceClass_OnSetRunning( CutterDeviceClass _this, XBool value )
{
  if ( value == _this->Running )
    return;

  _this->Running = value;
  EwNotifyRefObservers( EwNewRef( _this, CutterDeviceClass_OnGetRunning, CutterDeviceClass_OnSetRunning ), 
    0 );

  if ( value )
  {
    CutterDeviceClass_OnSetRemaining( _this, _this->PaperBatch );
    CoreTimer_OnSetEnabled( &_this->Timer, 1 );
  }
  else
    CoreTimer_OnSetEnabled( &_this->Timer, 0 );
}

/* 'C' function for method : 'Cutter::DeviceClass.OnSetRemaining()' */
void CutterDeviceClass_OnSetRemaining( CutterDeviceClass _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value > 1000 )
    value = 1000;

  if ( value == _this->Remaining )
    return;

  _this->Remaining = value;
  EwNotifyRefObservers( EwNewRef( _this, CutterDeviceClass_OnGetRemaining, CutterDeviceClass_OnSetRemaining ), 
    0 );
}

/* This is a slot method. */
void CutterDeviceClass_Animate( CutterDeviceClass _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CutterDeviceClass_OnSetRemaining( _this, _this->Remaining - 1 );

  if ( _this->Remaining == 0 )
    CutterDeviceClass_OnSetRunning( _this, 0 );
}

/* 'C' function for method : 'Cutter::DeviceClass.OnSetPaperType()' */
void CutterDeviceClass_OnSetPaperType( CutterDeviceClass _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value > 3 )
    value = 3;

  if ( value == _this->PaperType )
    return;

  _this->PaperType = value;
  EwNotifyRefObservers( EwNewRef( _this, CutterDeviceClass_OnGetPaperType, CutterDeviceClass_OnSetPaperType ), 
    0 );
}

/* Default onget method for the property 'PaperBatch' */
XInt32 CutterDeviceClass_OnGetPaperBatch( CutterDeviceClass _this )
{
  return _this->PaperBatch;
}

/* Default onget method for the property 'PaperSpeed' */
XInt32 CutterDeviceClass_OnGetPaperSpeed( CutterDeviceClass _this )
{
  return _this->PaperSpeed;
}

/* Default onget method for the property 'PaperWidth' */
XInt32 CutterDeviceClass_OnGetPaperWidth( CutterDeviceClass _this )
{
  return _this->PaperWidth;
}

/* Default onget method for the property 'PaperHeight' */
XInt32 CutterDeviceClass_OnGetPaperHeight( CutterDeviceClass _this )
{
  return _this->PaperHeight;
}

/* Default onget method for the property 'Running' */
XBool CutterDeviceClass_OnGetRunning( CutterDeviceClass _this )
{
  return _this->Running;
}

/* Default onget method for the property 'Remaining' */
XInt32 CutterDeviceClass_OnGetRemaining( CutterDeviceClass _this )
{
  return _this->Remaining;
}

/* Default onget method for the property 'PaperType' */
XInt32 CutterDeviceClass_OnGetPaperType( CutterDeviceClass _this )
{
  return _this->PaperType;
}

/* Variants derived from the class : 'Cutter::DeviceClass' */
EW_DEFINE_CLASS_VARIANTS( CutterDeviceClass )
EW_END_OF_CLASS_VARIANTS( CutterDeviceClass )

/* Virtual Method Table (VMT) for the class : 'Cutter::DeviceClass' */
EW_DEFINE_CLASS( CutterDeviceClass, XObject, Timer, Timer, Timer, Timer, PaperBatch, 
                 "Cutter::DeviceClass" )
EW_END_OF_CLASS( CutterDeviceClass )

/* User defined auto object: 'Cutter::Device' */
EW_DEFINE_AUTOOBJECT( CutterDevice, CutterDeviceClass )

/* Initializer for the auto object 'Cutter::Device' */
void CutterDevice__Init( CutterDeviceClass _this )
{
  EW_UNUSED_ARG( _this );
}

/* Table with links to derived variants of the auto object : 'Cutter::Device' */
EW_DEFINE_AUTOOBJECT_VARIANTS( CutterDevice )
EW_END_OF_AUTOOBJECT_VARIANTS( CutterDevice )

/* Initializer for the class 'Cutter::BatchWindow' */
void CutterBatchWindow__Init( CutterBatchWindow _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CutterWindow__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->Icon, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextBatchLarge, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextBatchMedium, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterBatchWindow );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0008 );
  CoreRectView__OnSetBounds( &_this->Super1.CaptionRect, _Const0009 );
  ViewsText_OnSetString( &_this->Super1.CaptionText, EwLoadString( &_Const0024 ));
  CoreView_OnSetLayout((CoreView)&_this->Icon, CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->Icon, _Const0025 );
  ViewsImage_OnSetAutoSize( &_this->Icon, 0 );
  ViewsImage_OnSetAlignment( &_this->Icon, ViewsImageAlignmentAlignHorzLeft | ViewsImageAlignmentAlignVertCenter );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToTop | CoreLayoutResizeHorz );
  CoreRectView__OnSetBounds( &_this->Text, _Const0026 );
  ViewsText_OnSetWrapText( &_this->Text, 0 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertTop );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0027 ));
  ViewsText_OnSetColor( &_this->Text, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->TextBatchLarge, CoreLayoutAlignToTop | 
  CoreLayoutResizeHorz );
  CoreRectView__OnSetBounds( &_this->TextBatchLarge, _Const0028 );
  ViewsText_OnSetWrapText( &_this->TextBatchLarge, 0 );
  ViewsText_OnSetAlignment( &_this->TextBatchLarge, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextBatchLarge, EwLoadString( &_Const0029 ));
  ViewsText_OnSetColor( &_this->TextBatchLarge, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->TextBatchMedium, CoreLayoutAlignToTop | 
  CoreLayoutResizeHorz );
  CoreRectView__OnSetBounds( &_this->TextBatchMedium, _Const002A );
  ViewsText_OnSetWrapText( &_this->TextBatchMedium, 0 );
  ViewsText_OnSetAlignment( &_this->TextBatchMedium, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextBatchMedium, EwLoadString( &_Const0029 ));
  ViewsText_OnSetColor( &_this->TextBatchMedium, CutterTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Icon ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextBatchLarge ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextBatchMedium ), 0 );
  ViewsImage_OnSetBitmap( &_this->Icon, EwLoadResource( &CutterPaperBatch, ResourcesBitmap ));
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &CutterDescrFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextBatchLarge, EwLoadResource( &CutterLargeDigitFont, 
  ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextBatchMedium, EwLoadResource( &CutterMediumDigitFont, 
  ResourcesFont ));
}

/* Re-Initializer for the class 'Cutter::BatchWindow' */
void CutterBatchWindow__ReInit( CutterBatchWindow _this )
{
  /* At first re-initialize the super class ... */
  CutterWindow__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->Icon );
  ViewsText__ReInit( &_this->Text );
  ViewsText__ReInit( &_this->TextBatchLarge );
  ViewsText__ReInit( &_this->TextBatchMedium );
}

/* Finalizer method for the class 'Cutter::BatchWindow' */
void CutterBatchWindow__Done( CutterBatchWindow _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterBatchWindow );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->Icon );
  ViewsText__Done( &_this->Text );
  ViewsText__Done( &_this->TextBatchLarge );
  ViewsText__Done( &_this->TextBatchMedium );

  /* Don't forget to deinitialize the super class ... */
  CutterWindow__Done( &_this->_Super );
}

/* Method for doing a fade animation within the derived window classes. The parameter 
   aValue runs from 0 to 255 during the animation. */
void CutterBatchWindow_FadeAnimation( CutterBatchWindow _this, XEnum aMode, XInt32 
  aValue )
{
  if ( aMode == CutterOperationModeProgramming )
  {
    ViewsText_OnSetOpacity( &_this->TextBatchLarge, aValue );
    ViewsText_OnSetOpacity( &_this->TextBatchMedium, 255 - aValue );
  }
  else
    if ( aMode == CutterOperationModeRunning )
    {
      ViewsText_OnSetOpacity( &_this->TextBatchLarge, 255 - aValue );
      ViewsText_OnSetOpacity( &_this->TextBatchMedium, aValue );
    }

  if ( aMode == CutterOperationModeExclusive )
  {
    if ( _this->dialog == 0 )
    {
      _this->dialog = EwNewObject( CutterBatchDialog, 0 );
      CoreRectView__OnSetBounds( _this->dialog, EwSetRectOrigin( _this->dialog->Super2.Bounds, 
      _Const000E ));
      CoreGroup__Add( _this, ((CoreView)_this->dialog ), 0 );
      _this->dialog->OnCancel = EwNewSlot( _this, CutterWindow_onCancel );
      CutterBatchDialog_OnSetPaperBatch( _this->dialog, EwNewRef( EwGetAutoObject( 
      &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetPaperBatch, CutterDeviceClass_OnSetPaperBatch ));
    }

    CoreGroup__OnSetOpacity( _this->dialog, aValue );
    ViewsText_OnSetOpacity( &_this->TextBatchLarge, 255 - aValue );
    ViewsText_OnSetOpacity( &_this->TextBatchMedium, 255 - aValue );
    ViewsText_OnSetOpacity( &_this->Text, 255 - aValue );
    ViewsImage_OnSetOpacity( &_this->Icon, 255 - aValue );
  }
  else
  {
    if ( _this->dialog != 0 )
    {
      if ( aValue > _this->dialog->Super1.Opacity )
        CoreGroup__OnSetOpacity( _this->dialog, aValue );

      if ( aValue < 10 )
      {
        CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->dialog ));
        _this->dialog = 0;
      }
    }

    if ( aValue > _this->Text.Opacity )
      ViewsText_OnSetOpacity( &_this->Text, aValue );

    if ( aValue > _this->Icon.Opacity )
      ViewsImage_OnSetOpacity( &_this->Icon, aValue );
  }
}

/* 'C' function for method : 'Cutter::BatchWindow.OnSetPaperBatch()' */
void CutterBatchWindow_OnSetPaperBatch( CutterBatchWindow _this, XRef value )
{
  if ( !EwCompRef( value, _this->PaperBatch ))
    return;

  if ( _this->PaperBatch.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterBatchWindow_onUpdate ), _this->PaperBatch, 
      0 );

  _this->PaperBatch = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterBatchWindow_onUpdate ), value, 
      0 );

  EwPostSignal( EwNewSlot( _this, CutterBatchWindow_onUpdate ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void CutterBatchWindow_onUpdate( CutterBatchWindow _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperBatch.Object != 0 )
    ViewsText_OnSetString( &_this->TextBatchLarge, EwNewStringInt( EwOnGetInt32( 
    _this->PaperBatch ), 0, 10 ));

  ViewsText_OnSetString( &_this->TextBatchMedium, _this->TextBatchLarge.String );
}

/* Variants derived from the class : 'Cutter::BatchWindow' */
EW_DEFINE_CLASS_VARIANTS( CutterBatchWindow )
EW_END_OF_CLASS_VARIANTS( CutterBatchWindow )

/* Virtual Method Table (VMT) for the class : 'Cutter::BatchWindow' */
EW_DEFINE_CLASS( CutterBatchWindow, CutterWindow, dialog, PaperBatch, PaperBatch, 
                 Icon, _None, "Cutter::BatchWindow" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CutterWindow_UpdateLayout,
  CutterWindow_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
  CutterWindow_ChangeMode,
  CutterBatchWindow_FadeAnimation,
EW_END_OF_CLASS( CutterBatchWindow )

/* This is a font resource. */
EW_DEFINE_FONT_RES( CutterLargeDigitFont, 49, 13, 0, 16, 0x0001, 0x000E )
  EW_GLYPH( 0x0001, 2, -37, 19, 37, 23, 0x00000000 ),
  EW_GLYPH( 0x002C, 0, -7, 9, 15, 11, 0x0000063E ),             /* ',' */
  EW_GLYPH( 0x002E, 3, -7, 8, 7, 15, 0x00000758 ),              /* '.' */
  EW_GLYPH( 0x0030, 2, -37, 26, 38, 30, 0x00000801 ),           /* '0' */
  EW_GLYPH( 0x0031, 4, -37, 16, 37, 30, 0x00000EAB ),           /* '1' */
  EW_GLYPH( 0x0032, 2, -37, 26, 37, 30, 0x000011C9 ),           /* '2' */
  EW_GLYPH( 0x0033, 2, -37, 25, 38, 30, 0x000018CD ),           /* '3' */
  EW_GLYPH( 0x0034, 1, -37, 28, 37, 30, 0x0000205F ),           /* '4' */
  EW_GLYPH( 0x0035, 3, -37, 25, 38, 30, 0x0000264D ),           /* '5' */
  EW_GLYPH( 0x0036, 3, -37, 25, 38, 30, 0x00002D8D ),           /* '6' */
  EW_GLYPH( 0x0037, 1, -37, 27, 37, 30, 0x000034DF ),           /* '7' */
  EW_GLYPH( 0x0038, 2, -37, 26, 38, 30, 0x00003B0E ),           /* '8' */
  EW_GLYPH( 0x0039, 2, -37, 25, 37, 30, 0x0000430B ),           /* '9' */
  EW_GLYPH( 0x003A, 3, -29, 8, 29, 14, 0x00004A55 ),            /* ':' */

  EW_FONT_PIXEL( CutterLargeDigitFont, 0x00004C36 )             /* ratio 46.63 % */
    0xEF7BDEEB, 0x7BDEF7BD, 0x2EF7BDEF, 0xEF7BDCE2, 0x7BDEF7BD, 0x80098437, 0x1F51932F,
    0x781EB100, 0xBD3001F3, 0xF001C630, 0x12AF8055, 0x3302F950, 0x0319C03E, 0xCF902730,
    0x04C5F007, 0x7C804C5F, 0x0027303E, 0xF8CC0C67, 0xE1F1A800, 0xBE0024FF, 0x001D4615,
    0x146FA698, 0xAC5F9800, 0xF2F8000F, 0xAC800133, 0x7F00009A, 0xBF80003F, 0xAB20001F,
    0xCBE00026, 0xE60004CF, 0x0003EB17, 0x028DF4D3, 0x51856F80, 0x3E350007, 0xE0049FFC,
    0x1F19818C, 0x303E7C80, 0x262F8027, 0xCC0262F8, 0x01F3E409, 0x33807C66, 0x0255F006,
    0xC6305F2A, 0xC055F001, 0x69800F9B, 0x4007D462, 0x02610FAC, 0x9C44CBE0, 0xF7BDEF7B,
    0x06EF7BDE, 0xFBDEF7FE, 0x00000002, 0x02040600, 0x3F14030E, 0x1F03C918, 0x34539133,
    0x2AC0E4BC, 0x69E3BFC6, 0xE207EEB0, 0xDF17773D, 0x4637E30C, 0x73027781, 0x38147302,
    0xAFC619BE, 0xE7DDEA00, 0x02DFB3DE, 0x37FFE600, 0xFE631047, 0xDF0013FE, 0xFBE4009B,
    0x001EF801, 0xF300F9E0, 0x26780001, 0xFFD10688, 0xA01DFF7B, 0xFC406603, 0x1FEB9257,
    0xA07C2F10, 0x3C1A3007, 0x01390918, 0x0A0E4066, 0x140B0014, 0xB801707F, 0x0F04CFE0,
    0x09260900, 0x92098012, 0x20180000, 0x00001004, 0x00000002, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x10044000, 0x30480000, 0x09201800,
    0x20980130, 0x90024078, 0x0D82E1E0, 0x029706C0, 0x87814005, 0x600B101C, 0x0FC0E405,
    0x178C05BC, 0xE212B07C, 0x3FD724EB, 0x10681620, 0xBFEF7FEB, 0x4CF03403, 0x01398000,
    0xE60004EF, 0x0DFDE00B, 0x017BF200, 0x2AB7FB50, 0xFF7F3189, 0xD1800007, 0xFB2005FF,
    0x720257BF, 0x404AFFED, 0x01EFFDAC, 0x3D3BF988, 0x02DEF800, 0x40000000, 0x9004DFCC,
    0x02DE77F3, 0xB7FFDCC4, 0x01DEF800, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0xBF310000, 0xEFFDEE7D, 0xF10001F7, 0x4119F7DE, 0x069DF98E,
    0x0033B9E0, 0xC04FF588, 0xC80002FC, 0x0179E09D, 0xF881C400, 0xF73BF501, 0x898C09EE,
    0xABFBC037, 0xF01FBC60, 0x01358261, 0x542302CF, 0x5E00BC40, 0x054031E0, 0x0E104880,
    0x026003F8, 0x00666664, 0xFFEF0800, 0x27FFFFFF, 0x00F01E00, 0x23015000, 0xF0680000,
    0x07300001, 0x900004C4, 0x80001A0D, 0x00263013, 0x3880DF80, 0x02BE0000, 0xE80004BF,
    0x0007AC07, 0x34F02D60, 0x506CC000, 0xF3C0001F, 0x0001FA82, 0x0FD40FD4, 0x017C6000,
    0x180003E3, 0x0FF180FF, 0x809E4000, 0x200007FC, 0x00DE426E, 0x213FC400, 0xFC00026F,
    0x004DC407, 0xF880DF80, 0x57C00027, 0xF7BDF620, 0xBDEF7BDE, 0xFF7BDEF7, 0x0000001E,
    0x00000000, 0x00000000, 0xEB900000, 0xFEF7BFF7, 0xC0006B7F, 0x12BBFDF8, 0x03F7BD59,
    0x600177A0, 0x5BE01F7C, 0x07E78000, 0x500001FA, 0xB980B18D, 0x067BBDEF, 0xE20BC958,
    0xFBC6077B, 0xF028F805, 0xE09A803E, 0x00B10151, 0x804640FC, 0x8C018007, 0xFDEF7BDA,
    0x18880002, 0x00918C63, 0xC0000200, 0x00009900, 0x0000F05E, 0x05E207E8, 0x05EE7200,
    0xBFF000BE, 0x3BFFDEF7, 0x0002DE20, 0x009FF900, 0x00CF2000, 0x7F720000, 0x78000001,
    0x7FE000F6, 0xDFFFBDEF, 0x0004F203, 0x80EDF590, 0x180001FC, 0x0004F03F, 0x05806780,
    0x0B814000, 0xE41FC000, 0x2B5AD6B3, 0xDD380001, 0x48005DDD, 0x01FC0310, 0x0FFFC1B0,
    0x0518006A, 0x049C02AA, 0x623901E4, 0x303DDC82, 0xD1F8277A, 0xF7DAC403, 0xD402BDDE,
    0x000049C4, 0x05DC83E3, 0x80767800, 0x8009BDF8, 0xCC00FBF1, 0x44955EFF, 0x0177BD3E,
    0x7B9C4000, 0x002FBDEF, 0x0002FC00, 0x003D4000, 0x00BC0000, 0x02740000, 0x13180000,
    0x1B800000, 0xD6000000, 0x90000000, 0x00000C2F, 0x0FC606F0, 0x429E0000, 0x8800005C,
    0x00003986, 0x00D913A0, 0xBC263000, 0xC0E00000, 0xD0000053, 0x0001F889, 0x03784E40,
    0xC0DE0000, 0x30000058, 0x00002E0B, 0x001AC344, 0x640BF000, 0x3D400003, 0xE00000D0,
    0x00007A85, 0x02F113A0, 0x1058C000, 0xBDEF7BFF, 0xF306DEF7, 0x07C77BDE, 0x00000000,
    0x00080000, 0x005C0000, 0x7BAC0000, 0xDEF7BDEF, 0xF7BDEF7B, 0xDEF79836, 0x0000003B,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xDEF30000, 0xF7BDEF7B,
    0xBDEF7BDE, 0x20BBDEF7, 0x0E000000, 0x02400000, 0x002C0000, 0xE7848000, 0xBDEF7BDE,
    0xEF7BDEF7, 0x0030105D, 0x00704800, 0x00900800, 0x00104C00, 0x00982400, 0x00305C00,
    0x00884800, 0xD5202000, 0xFB3DCFBB, 0x2130019B, 0x4239BFFE, 0x0BBDE78C, 0x20099090,
    0x001017FA, 0x0907D400, 0x2C6A8000, 0xDEE77EA0, 0x4CC09BFF, 0x113EFFEA, 0x9C82AFEF,
    0x0FC407FF, 0x2773BF39, 0x02C037C0, 0xC4170004, 0x12300000, 0x1400003C, 0x1C000038,
    0x00000010, 0xD4000000, 0x5BEFBEFB, 0x62203800, 0x01E739CE, 0x41338140, 0x3C123001,
    0x80162123, 0xE03C3906, 0xE05D007D, 0xEF880DE1, 0x02DFA81B, 0xE03F12F1, 0x5FDEF7D9,
    0xD3C0BF01, 0x03D60000, 0x200017D6, 0x7ECF01BA, 0x05FF5000, 0xCFD3BF20, 0xFFFAB208,
    0xC600001B, 0xDEFFF6FA, 0xFAC40001, 0x02467EEF, 0x09DBAC00, 0x1DD88000, 0x7B900000,
    0xB8800000, 0xFEFF6409, 0x5F000EF7, 0xAFFBF182, 0x02AC004A, 0x10001773, 0x002F582F,
    0x0D501BC0, 0xBC426000, 0x809E0000, 0x2E000095, 0x280000F8, 0xEFE62188, 0xB7FFEF7F,
    0x65011802, 0xAC92B7DD, 0x301E0DBE, 0xB1001D7E, 0x6603817E, 0x087D4001, 0x26A80000,
    0xEE77EA01, 0x26600DFD, 0x0ABF7C40, 0x3E203F6A, 0xF803DF80, 0xF04C7806, 0x641A003C,
    0x4A801702, 0xC0001350, 0xC0000102, 0x03009C1F, 0x07048000, 0xC1FC4000, 0xDC1FC000,
    0x16000F02, 0x26203118, 0xF1E09180, 0x06800E81, 0x7C426615, 0x83817401, 0x5067B506,
    0xC1EA05BF, 0xCF7C80F3, 0xF102BFBD, 0x0000AF82, 0x7BF025F1, 0x057C4000, 0x20097DF0,
    0x9E002DE6, 0x208E6FFF, 0x806FBE9F, 0xF7BDEF71, 0xBDEF7BDE, 0xEF7BDEF7, 0x7BDEF7BD,
    0x0000001F, 0x00000000, 0x00800000, 0x35900000, 0xDEF7BDEE, 0xF7BDEF7B, 0xFDEF7BDE,
    0x000007C0, 0x0004C42F, 0x01980D40, 0x3E0B8000, 0xC1640000, 0x68000093, 0x90000070,
    0x0000A305, 0x00BC05F0, 0xF9098800, 0x06600000, 0xF000007C, 0x00004C41, 0x001584CC,
    0x0FC0F800, 0x80CF0000, 0xBC00004A, 0x600002E0, 0x0005180C, 0x001F0680, 0x2620B200,
    0x607E0000, 0x17200002, 0x80000BC4, 0x0002A02F, 0x05E09880, 0xA80CC000, 0x0F800001,
    0xCC00001C, 0x00007C84, 0x001F81F0, 0x4C406A00, 0x98340000, 0x03180000, 0x7800007C,
    0x000093C1, 0x005E03E4, 0x18C0FC00, 0xF3C00000, 0xD9EF73EE, 0xAC0003EF, 0xC411CDF3,
    0x0059DF98, 0x2000EEC4, 0xF88017FA, 0xFEA0000F, 0x0006FC01, 0x7A81F8C0, 0xBDFFAC40,
    0x0678057B, 0x4EFF101C, 0x0BC07F46, 0x6A00E605, 0xF8064281, 0x49103E00, 0x4C804880,
    0x00030010, 0x0000C000, 0x24404608, 0xA2442440, 0x413007C0, 0x01660BC1, 0x8C2F05E3,
    0x9275F109, 0x03580FFD, 0xFEB105BC, 0xC057FEF7, 0x04DF80D3, 0xE007D600, 0x1AB0009D,
    0x0009EE40, 0xDA8017EB, 0x3DA80003, 0xF9017E40, 0x77FDEFFE, 0x13809E40, 0xE499FF50,
    0xC3F203BA, 0x403F2033, 0xB8AA03FC, 0x98800BE0, 0x07814FE0, 0x11281F80, 0x90480120,
    0x04000000, 0x20480120, 0x50014022, 0x0FC07920, 0xFE2C0F80, 0x2F803F20, 0x782665C1,
    0xB9C933FE, 0x5E0F901F, 0xBDFFDF20, 0x2F0077FF, 0xC0000BD4, 0x017E605B, 0x0137C400,
    0x80027ECF, 0x9001F798, 0x119F7DDF, 0xFBDE78C4, 0xBE780006, 0xEFD9EE7D, 0xF3AC0003,
    0x98C4233D, 0xE40099DF, 0xEF1000EE, 0x0779002F, 0xE03FA800, 0x7C600024, 0xDF1807A0,
    0xF04DE7B9, 0x6A09883C, 0x1EE6213F, 0xF20331A0, 0xE404E203, 0x02FC0FC3, 0x62382F90,
    0x82600580, 0x004603C7, 0x02310188, 0xE03C004C, 0x0E000601, 0x00006008, 0x0004C038,
    0x00048260, 0x00988060, 0x7E0501FC, 0xF0079E00, 0x3C03F907, 0xA099803F, 0x7EA213F6,
    0x0341005F, 0xFCF73BE3, 0x4CF2E00A, 0xE2402000, 0x1730009D, 0x01BEF098, 0x109BEF10,
    0x3DF3CC03, 0xA67BD649, 0xE7801E07, 0xFFFBDFFB, 0x06C0464D, 0x0F03E000, 0x505C8000,
    0x04E00009, 0xF880005C, 0x0002A80D, 0x2F105BCC, 0xEF3C6200, 0x12FC0379, 0xFB3DEE20,
    0x0FBC019B, 0x6F880000, 0xE7800002, 0x8800003E, 0x0000BFB9, 0x77FA78E4, 0xBC4001DE,
    0x9BE2EEE7, 0x28C6FC61, 0x4E604EF0, 0xC7028E60, 0x95F8C337, 0x05DDCF78, 0x00000000,
    0x00000000, 0x00000000, 0xE7BC4000, 0x619BE2EE, 0xF028C6FC, 0x604E604E, 0x37C7028E,
    0x0015F8C3

  EW_FONT_KERNING_CODES( CutterLargeDigitFont )
  EW_FONT_KERNING_VALUES( CutterLargeDigitFont )
EW_END_OF_FONT_RES( CutterLargeDigitFont )

/* Table with links to derived variants of the font resource : 'Cutter::LargeDigitFont' */
EW_RES_WITHOUT_VARIANTS( CutterLargeDigitFont )

/* This is a font resource. */
EW_DEFINE_FONT_RES( CutterMediumDigitFont, 36, 10, 0, 16, 0x0001, 0x000E )
  EW_GLYPH( 0x0001, 1, -27, 15, 27, 17, 0x00000000 ),
  EW_GLYPH( 0x002C, 0, -5, 7, 11, 8, 0x000003D9 ),              /* ',' */
  EW_GLYPH( 0x002E, 2, -5, 6, 5, 11, 0x00000498 ),              /* '.' */
  EW_GLYPH( 0x0030, 2, -27, 18, 27, 22, 0x0000050A ),           /* '0' */
  EW_GLYPH( 0x0031, 3, -27, 12, 27, 22, 0x000008FE ),           /* '1' */
  EW_GLYPH( 0x0032, 1, -27, 20, 27, 22, 0x00000AE2 ),           /* '2' */
  EW_GLYPH( 0x0033, 1, -27, 19, 27, 22, 0x00000F45 ),           /* '3' */
  EW_GLYPH( 0x0034, 1, -27, 20, 27, 22, 0x00001411 ),           /* '4' */
  EW_GLYPH( 0x0035, 2, -27, 19, 27, 22, 0x00001784 ),           /* '5' */
  EW_GLYPH( 0x0036, 2, -27, 19, 27, 22, 0x00001C0C ),           /* '6' */
  EW_GLYPH( 0x0037, 1, -27, 19, 27, 22, 0x0000208E ),           /* '7' */
  EW_GLYPH( 0x0038, 2, -27, 18, 27, 22, 0x0000242E ),           /* '8' */
  EW_GLYPH( 0x0039, 1, -27, 19, 27, 22, 0x000028D1 ),           /* '9' */
  EW_GLYPH( 0x003A, 2, -21, 6, 21, 10, 0x00002D57 ),            /* ':' */

  EW_FONT_PIXEL( CutterMediumDigitFont, 0x00002E95 )            /* ratio 53.49 % */
    0xF7BDEF71, 0xBDEF7BDE, 0xDEF71837, 0x37BDEF7B, 0x16601561, 0x3EF8039C, 0x5A825678,
    0x381F7C03, 0xC40F9807, 0x89A8807A, 0x3EF803F7, 0xAC60133C, 0x38005732, 0xF002F7C7,
    0xC000C7AC, 0xE000FBF7, 0x4F000989, 0x7EF8004C, 0xF59E001F, 0xE39C0018, 0x558C017B,
    0xEF800AE6, 0x510133C3, 0x9807EF13, 0x807AC40F, 0x80E703EF, 0x0D6A095A, 0x7DF007B8,
    0x0B300AB0, 0xDEF7BDC6, 0xF84DEF7B, 0x00077BDD, 0x18216000, 0xF0F07A64, 0xF0679F12,
    0x1678F5A6, 0x9E0FEDF9, 0xFC65FDFF, 0x9819BD52, 0x4BF19819, 0xFBF982F5, 0x4DFFFBDF,
    0x95BCE780, 0x01E7BD64, 0xF10017CF, 0x001FC83D, 0xA05F92FC, 0x10BBBDF6, 0x37EA163F,
    0x52C2BF18, 0x445C0B81, 0x1501446D, 0x200789D8, 0x58008191, 0x90018988, 0x02000058,
    0x00000000, 0x00000000, 0x4C400000, 0x0AC4800C, 0xFE42C004, 0xCE2400F1, 0x81501440,
    0x1A068151, 0x6FC61651, 0x8B037E30, 0x77BED40E, 0x2F89A217, 0x502F8800, 0x57E4025F,
    0x93EFFEA0, 0x02FFBD64, 0x807FF640, 0x4AFFBF98, 0x05BEF7A8, 0x19BFFEB9, 0x0004EDC0,
    0x7BD40062, 0xFFFD982F, 0x2B7E00CD, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0xFFBF5880, 0x0ADFFFBD, 0x2576F300, 0xE0277D59,
    0xDE20037B, 0xC000FD06, 0xD10B907A, 0x037F7BFF, 0x2B7C345E, 0x04627FA9, 0xFC40A785,
    0x05C08D20, 0x0098003C, 0xDEF7B8C0, 0x03107801, 0x00B0F800, 0x03F03980, 0x12B1A300,
    0xA304F200, 0x126E2001, 0x9FE20007, 0xE0013FC4, 0x006F883F, 0x5F106FC0, 0xC457C001,
    0x3DF00057, 0xD00057C4, 0x0015F10F, 0x02BE22F4, 0xAF88B980, 0x88BE6000, 0xDEF7BDDF,
    0xF7BDEF7B, 0x000007F1, 0x00000000, 0xE7DAE400, 0x003DFD9E, 0x3108F77A, 0x00FFEF9E,
    0xF90015BF, 0x000AF84D, 0xE20F507A, 0x42EEF73D, 0x7FC45CBC, 0x5037E306, 0xF803583C,
    0x554F1110, 0x460301FD, 0x5FBF7EFC, 0x00911201, 0x000A1F10, 0x0D127731, 0xE7BDEF78,
    0x009FE24D, 0x000F6780, 0x00005580, 0xDE00FDF2, 0x0AF9EF7B, 0x9E20034F, 0x001983DF,
    0x000B11A0, 0xF73CE064, 0x03009BDE, 0x12201426, 0x017E0477, 0xDC899434, 0x137E304F,
    0x7E44E863, 0xD05DDEE7, 0x6F0009B8, 0xC8017721, 0xDBF104DF, 0xF318846B, 0xF00017FB,
    0x00EF7BDD, 0x0003CF00, 0x80001A20, 0x3500004E, 0x01780001, 0x00A4E800, 0x0BC8B180,
    0x059C5C00, 0x06B07A00, 0x6C931800, 0x2F06E000, 0xB306B000, 0x64BE4000, 0xF06F0003,
    0xC5980006, 0x9A200053, 0xBF0001F8, 0xF7BDCE20, 0xDEFB0DDE, 0x000001F7, 0x00240000,
    0xDEF7FE00, 0xF7BDEF7B, 0xBEF7D86E, 0x00000000, 0x00000000, 0x00000000, 0xEF7BDE40,
    0x7BDEF7BD, 0x020FBDEF, 0x00098000, 0x70000240, 0xF7BDEEB1, 0x7DEF7BDE, 0x4000B890,
    0x89800120, 0x12240009, 0xF4620400, 0x4DFFF73E, 0x11AF6120, 0x03FF79F2, 0xF9002226,
    0x2000181F, 0xFA20C60E, 0x905EEF7F, 0x6F9F6793, 0x07F464AF, 0x8127887F, 0x000141E7,
    0x98000617, 0xF5000000, 0x007EFDFB, 0xAAAAAC13, 0x18C45C02, 0xC1EA0681, 0x45D8C4C3,
    0x6F0E0FAC, 0x3DEEFC62, 0x3887A81D, 0xC1F18001, 0xF9E005D8, 0x6FDFC807, 0xED63108E,
    0xEB90002F, 0x000EE7DB, 0xCFDFEFC6, 0x2EF40008, 0x00B7C000, 0xC607DE00, 0x401DEFBB,
    0xCF6FE44D, 0xB791A001, 0x27035000, 0x19E0E001, 0xAC70A000, 0xB7E7B9DF, 0xBFC408C0,
    0x7FF5442B, 0x025F81E0, 0x08083FF2, 0x60397100, 0x0E9EE77C, 0xFDA8078C, 0x383F5884,
    0x29E03E20, 0xE004603C, 0xF000088F, 0x00241E40, 0x26006030, 0xC03E0564, 0x1AC28445,
    0x1A2C1EA0, 0xD6215FF1, 0x21723F07, 0x81D3DCFA, 0x0007E899, 0x03AF82F5, 0x5807EB10,
    0xB18847BB, 0xDEEFC376, 0xF7BDEF7B, 0xBDEF7BDE, 0x00005EF7, 0x00100000, 0xDEEFD400,
    0xF7BDEF7B, 0xE06F7BDE, 0xA8330005, 0xB87E0001, 0xB249E000, 0x1F8BC000, 0x4C41A800,
    0x0098D000, 0x05E20C60, 0x000A8D00, 0x000BC2C8, 0x2000351A, 0xFC00170B, 0x10005182,
    0x78001F0B, 0x60009883, 0x15800262, 0x3E000BC4, 0x998000AC, 0x87C000F8, 0x0CF0001A,
    0x62F0005C, 0xC0D40014, 0xFAC40007, 0xFFFDEFFD, 0xEDE60056, 0x4EFAB24A, 0xC4006FD0,
    0x0005A8DB, 0xFB50B8AB, 0x45705DDE, 0xF1837EA1, 0x0709281B, 0x09806178, 0x8130904C,
    0x07031909, 0xA8D0A838, 0x0AFC60DF, 0xBED46A97, 0xC2DE1777, 0x7F2012FA, 0x1800E413,
    0x001D7E09, 0x0AF87EF1, 0x27BBDF73, 0xD078D79E, 0x8DBC60EF, 0x01721F99, 0x583DF12B,
    0x30230B00, 0x144E6000, 0xC81CC2C0, 0xF8C4CC05, 0xBB20F741, 0x581E37E1, 0x0AFFDEFB,
    0x006FC33C, 0x3D88F8C0, 0xBC1B9801, 0xAB24AEFF, 0xAC402FFF, 0x7767B9DF, 0x2BB79802,
    0x01BB58C4, 0x58801BEF, 0x0013981F, 0xAC3442CF, 0x781D3DFF, 0x4EE81993, 0xC7C43EB1,
    0x26A058C6, 0x00FC1E1E, 0x2046647F, 0x000B0501, 0x00480C80, 0x0403F120, 0xF80B1850,
    0x27743F80, 0x0AA02ECF, 0x6FDEFFD6, 0x004BE080, 0x01DC4901, 0xE44C3FC8, 0xF5149EFE,
    0xC12217FE, 0xF9EFFDF3, 0x00051B4D, 0x8002E262, 0x000F905F, 0x07E0F7E2, 0x16F79F20,
    0xEF71026A, 0xF1819BFF, 0x3FD40001, 0xFDF18000, 0xA7C88002, 0xCF007EF7, 0x7E32FEFF,
    0xCC0CDEA9, 0xA5F8CC0C, 0xFBFF3D7A, 0x0000004B, 0x00000000, 0x2FEFFCF0, 0xCDEA97E3,
    0x8CC0CCC0, 0x0017AA5F

  EW_FONT_KERNING_CODES( CutterMediumDigitFont )
  EW_FONT_KERNING_VALUES( CutterMediumDigitFont )
EW_END_OF_FONT_RES( CutterMediumDigitFont )

/* Table with links to derived variants of the font resource : 'Cutter::MediumDigitFont' */
EW_RES_WITHOUT_VARIANTS( CutterMediumDigitFont )

/* Initializer for the class 'Cutter::RemainingWindow' */
void CutterRemainingWindow__Init( CutterRemainingWindow _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CutterWindow__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextRemaining, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterRemainingWindow );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0008 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->Super1.TouchHandler, 0 );
  CoreRectView__OnSetBounds( &_this->Super1.CaptionRect, _Const0009 );
  ViewsText_OnSetString( &_this->Super1.CaptionText, EwLoadString( &_Const002B ));
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft );
  CoreRectView__OnSetBounds( &_this->Text, _Const002C );
  ViewsText_OnSetAutoSize( &_this->Text, 1 );
  ViewsText_OnSetWrapText( &_this->Text, 0 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0027 ));
  ViewsText_OnSetColor( &_this->Text, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->TextRemaining, CoreLayoutAlignToBottom 
  | CoreLayoutAlignToLeft );
  CoreRectView__OnSetBounds( &_this->TextRemaining, _Const002D );
  ViewsText_OnSetAutoSize( &_this->TextRemaining, 1 );
  ViewsText_OnSetWrapText( &_this->TextRemaining, 0 );
  ViewsText_OnSetAlignment( &_this->TextRemaining, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextRemaining, EwLoadString( &_Const0029 ));
  ViewsText_OnSetColor( &_this->TextRemaining, CutterTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextRemaining ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &CutterDescrFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextRemaining, EwLoadResource( &CutterLargeDigitFont, 
  ResourcesFont ));
}

/* Re-Initializer for the class 'Cutter::RemainingWindow' */
void CutterRemainingWindow__ReInit( CutterRemainingWindow _this )
{
  /* At first re-initialize the super class ... */
  CutterWindow__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
  ViewsText__ReInit( &_this->TextRemaining );
}

/* Finalizer method for the class 'Cutter::RemainingWindow' */
void CutterRemainingWindow__Done( CutterRemainingWindow _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterRemainingWindow );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );
  ViewsText__Done( &_this->TextRemaining );

  /* Don't forget to deinitialize the super class ... */
  CutterWindow__Done( &_this->_Super );
}

/* The function 'ChangeMode' is responsible to make the transission between the 
   different modes of the window and to start the corresponding animations. */
void CutterRemainingWindow_ChangeMode( CutterRemainingWindow _this, XEnum aMode, 
  XRect aDestBounds )
{
  CutterWindow_ChangeMode((CutterWindow)_this, aMode, aDestBounds );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->Super1.TouchHandler, 0 );
}

/* 'C' function for method : 'Cutter::RemainingWindow.OnSetPaperRemaining()' */
void CutterRemainingWindow_OnSetPaperRemaining( CutterRemainingWindow _this, XRef 
  value )
{
  if ( !EwCompRef( value, _this->PaperRemaining ))
    return;

  if ( _this->PaperRemaining.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterRemainingWindow_onUpdate ), _this->PaperRemaining, 
      0 );

  _this->PaperRemaining = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterRemainingWindow_onUpdate ), value, 
      0 );

  EwPostSignal( EwNewSlot( _this, CutterRemainingWindow_onUpdate ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void CutterRemainingWindow_onUpdate( CutterRemainingWindow _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperRemaining.Object != 0 )
    ViewsText_OnSetString( &_this->TextRemaining, EwNewStringInt( EwOnGetInt32( 
    _this->PaperRemaining ), 0, 10 ));
}

/* Variants derived from the class : 'Cutter::RemainingWindow' */
EW_DEFINE_CLASS_VARIANTS( CutterRemainingWindow )
EW_END_OF_CLASS_VARIANTS( CutterRemainingWindow )

/* Virtual Method Table (VMT) for the class : 'Cutter::RemainingWindow' */
EW_DEFINE_CLASS( CutterRemainingWindow, CutterWindow, PaperRemaining, PaperRemaining, 
                 PaperRemaining, Text, _None, "Cutter::RemainingWindow" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CutterWindow_UpdateLayout,
  CutterWindow_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
  CutterRemainingWindow_ChangeMode,
  CutterWindow_FadeAnimation,
EW_END_OF_CLASS( CutterRemainingWindow )

/* Initializer for the class 'Cutter::SpeedWindow' */
void CutterSpeedWindow__Init( CutterSpeedWindow _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CutterWindow__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Icon, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextSpeed, &_this->_XObject, 0 );
  CutterSpeedButton__Init( &_this->SpeedButtonL, &_this->_XObject, 0 );
  CutterSpeedButton__Init( &_this->SpeedButtonR, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterSpeedWindow );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const002E );
  CoreRectView__OnSetBounds( &_this->Super1.CaptionRect, _Const0009 );
  ViewsText_OnSetString( &_this->Super1.CaptionText, EwLoadString( &_Const002F ));
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->Text, _Const0030 );
  ViewsText_OnSetAutoSize( &_this->Text, 1 );
  ViewsText_OnSetWrapText( &_this->Text, 0 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertTop );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0031 ));
  ViewsText_OnSetColor( &_this->Text, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Icon, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->Icon, _Const0032 );
  ViewsImage_OnSetAutoSize( &_this->Icon, 1 );
  ViewsImage_OnSetAlignment( &_this->Icon, ViewsImageAlignmentAlignHorzCenter | 
  ViewsImageAlignmentAlignVertCenter );
  CoreView_OnSetLayout((CoreView)&_this->TextSpeed, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->TextSpeed, _Const0033 );
  ViewsText_OnSetAutoSize( &_this->TextSpeed, 1 );
  ViewsText_OnSetWrapText( &_this->TextSpeed, 0 );
  ViewsText_OnSetAlignment( &_this->TextSpeed, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertBottom );
  ViewsText_OnSetString( &_this->TextSpeed, EwLoadString( &_Const0034 ));
  ViewsText_OnSetColor( &_this->TextSpeed, CutterTextColor );
  CoreRectView__OnSetBounds( &_this->SpeedButtonL, _Const0035 );
  CutterSpeedButton_OnSetIconIndex( &_this->SpeedButtonL, 0 );
  CoreRectView__OnSetBounds( &_this->SpeedButtonR, _Const0036 );
  CutterSpeedButton_OnSetIconIndex( &_this->SpeedButtonR, 1 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Icon ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextSpeed ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SpeedButtonL ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SpeedButtonR ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &CutterDescrFont, ResourcesFont ));
  ViewsImage_OnSetBitmap( &_this->Icon, EwLoadResource( &CutterPaperSpeed, ResourcesBitmap ));
  ViewsText_OnSetFont( &_this->TextSpeed, EwLoadResource( &CutterLargeDigitFont, 
  ResourcesFont ));
  _this->SpeedButtonL.OnChange = EwNewSlot( _this, CutterSpeedWindow_onDecrement );
  _this->SpeedButtonR.OnChange = EwNewSlot( _this, CutterSpeedWindow_onIncrement );
}

/* Re-Initializer for the class 'Cutter::SpeedWindow' */
void CutterSpeedWindow__ReInit( CutterSpeedWindow _this )
{
  /* At first re-initialize the super class ... */
  CutterWindow__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
  ViewsImage__ReInit( &_this->Icon );
  ViewsText__ReInit( &_this->TextSpeed );
  CutterSpeedButton__ReInit( &_this->SpeedButtonL );
  CutterSpeedButton__ReInit( &_this->SpeedButtonR );
}

/* Finalizer method for the class 'Cutter::SpeedWindow' */
void CutterSpeedWindow__Done( CutterSpeedWindow _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterSpeedWindow );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );
  ViewsImage__Done( &_this->Icon );
  ViewsText__Done( &_this->TextSpeed );
  CutterSpeedButton__Done( &_this->SpeedButtonL );
  CutterSpeedButton__Done( &_this->SpeedButtonR );

  /* Don't forget to deinitialize the super class ... */
  CutterWindow__Done( &_this->_Super );
}

/* Method for doing a fade animation within the derived window classes. The parameter 
   aValue runs from 0 to 255 during the animation. */
void CutterSpeedWindow_FadeAnimation( CutterSpeedWindow _this, XEnum aMode, XInt32 
  aValue )
{
  if ( aMode == CutterOperationModeProgramming )
  {
    CoreGroup__OnSetOpacity( &_this->SpeedButtonL, 255 - aValue );
    CoreGroup__OnSetOpacity( &_this->SpeedButtonR, 255 - aValue );
  }
  else
    if ( aMode == CutterOperationModeRunning )
    {
      CoreGroup__OnSetOpacity( &_this->SpeedButtonL, aValue );
      CoreGroup__OnSetOpacity( &_this->SpeedButtonR, aValue );
    }

  if ( aMode == CutterOperationModeExclusive )
  {
    if ( _this->dialog == 0 )
    {
      _this->dialog = EwNewObject( CutterSpeedDialog, 0 );
      CoreRectView__OnSetBounds( _this->dialog, EwSetRectOrigin( _this->dialog->Super2.Bounds, 
      _Const000E ));
      CoreGroup__Add( _this, ((CoreView)_this->dialog ), 0 );
      _this->dialog->OnCancel = EwNewSlot( _this, CutterWindow_onCancel );
      CutterSpeedDialog_OnSetPaperSpeed( _this->dialog, EwNewRef( EwGetAutoObject( 
      &CutterDevice, CutterDeviceClass ), CutterDeviceClass_OnGetPaperSpeed, CutterDeviceClass_OnSetPaperSpeed ));
    }

    CoreGroup__OnSetOpacity( _this->dialog, aValue );
    CoreGroup__OnSetOpacity( &_this->SpeedButtonL, 255 - aValue );
    CoreGroup__OnSetOpacity( &_this->SpeedButtonR, 255 - aValue );
    ViewsText_OnSetOpacity( &_this->TextSpeed, 255 - aValue );
    ViewsText_OnSetOpacity( &_this->Text, 255 - aValue );
    ViewsImage_OnSetOpacity( &_this->Icon, 255 - aValue );
  }
  else
  {
    if ( _this->dialog != 0 )
    {
      if ( aValue > _this->dialog->Super1.Opacity )
        CoreGroup__OnSetOpacity( _this->dialog, aValue );

      if ( aValue < 10 )
      {
        CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->dialog ));
        _this->dialog = 0;
      }
    }

    if ( aValue > _this->TextSpeed.Opacity )
      ViewsText_OnSetOpacity( &_this->TextSpeed, aValue );

    if ( aValue > _this->Text.Opacity )
      ViewsText_OnSetOpacity( &_this->Text, aValue );

    if ( aValue > _this->Icon.Opacity )
      ViewsImage_OnSetOpacity( &_this->Icon, aValue );
  }
}

/* 'C' function for method : 'Cutter::SpeedWindow.OnSetPaperSpeed()' */
void CutterSpeedWindow_OnSetPaperSpeed( CutterSpeedWindow _this, XRef value )
{
  if ( !EwCompRef( value, _this->PaperSpeed ))
    return;

  if ( _this->PaperSpeed.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterSpeedWindow_onUpdate ), _this->PaperSpeed, 
      0 );

  _this->PaperSpeed = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterSpeedWindow_onUpdate ), value, 
      0 );

  EwPostSignal( EwNewSlot( _this, CutterSpeedWindow_onUpdate ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void CutterSpeedWindow_onUpdate( CutterSpeedWindow _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperSpeed.Object != 0 )
    ViewsText_OnSetString( &_this->TextSpeed, EwNewStringInt( EwOnGetInt32( _this->PaperSpeed ), 
    0, 10 ));
}

/* Slot metod to change the paper speed, when user press arrow buttons. */
void CutterSpeedWindow_onIncrement( CutterSpeedWindow _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperSpeed.Object != 0 )
    EwOnSetInt32( _this->PaperSpeed, (( EwOnGetInt32( _this->PaperSpeed ) / 10 ) 
    * 10 ) + 10 );
}

/* Slot metod to change the paper speed, when user press arrow buttons. */
void CutterSpeedWindow_onDecrement( CutterSpeedWindow _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperSpeed.Object != 0 )
    EwOnSetInt32( _this->PaperSpeed, ((( EwOnGetInt32( _this->PaperSpeed ) + 9 ) 
    / 10 ) * 10 ) - 10 );
}

/* Variants derived from the class : 'Cutter::SpeedWindow' */
EW_DEFINE_CLASS_VARIANTS( CutterSpeedWindow )
EW_END_OF_CLASS_VARIANTS( CutterSpeedWindow )

/* Virtual Method Table (VMT) for the class : 'Cutter::SpeedWindow' */
EW_DEFINE_CLASS( CutterSpeedWindow, CutterWindow, dialog, PaperSpeed, PaperSpeed, 
                 Text, _None, "Cutter::SpeedWindow" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CutterWindow_UpdateLayout,
  CutterWindow_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
  CutterWindow_ChangeMode,
  CutterSpeedWindow_FadeAnimation,
EW_END_OF_CLASS( CutterSpeedWindow )

/* Initializer for the class 'Cutter::MainButton' */
void CutterMainButton__Init( CutterMainButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->Image, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterMainButton );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0037 );
  CoreRectView__OnSetBounds( &_this->Image, _Const0037 );
  ViewsImage_OnSetAutoSize( &_this->Image, 0 );
  ViewsImage_OnSetColor( &_this->Image, CutterStartColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToLeft | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->Text, _Const0038 );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0039 ));
  _this->Mode = CutterButtonModeStart;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->TouchHandler, _Const003A );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->TouchHandler, _Const003B );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->TouchHandler, _Const003C );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->TouchHandler, _Const003D );
  CoreGroup__Add( _this, ((CoreView)&_this->Image ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TouchHandler ), 0 );
  ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &CutterButtonBig, ResourcesBitmap ));
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &CutterMainButtonFont, ResourcesFont ));
  _this->TouchHandler.OnLeave = EwNewSlot( _this, CutterMainButton_enterLeaveSlot );
  _this->TouchHandler.OnEnter = EwNewSlot( _this, CutterMainButton_enterLeaveSlot );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, CutterMainButton_pressReleaseSlot );
  _this->TouchHandler.OnPress = EwNewSlot( _this, CutterMainButton_pressReleaseSlot );

  /* Call the user defined constructor */
  CutterMainButton_Init( _this, aArg );
}

/* Re-Initializer for the class 'Cutter::MainButton' */
void CutterMainButton__ReInit( CutterMainButton _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->Image );
  ViewsText__ReInit( &_this->Text );
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
}

/* Finalizer method for the class 'Cutter::MainButton' */
void CutterMainButton__Done( CutterMainButton _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterMainButton );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->Image );
  ViewsText__Done( &_this->Text );
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void CutterMainButton_UpdateLayout( CutterMainButton _this, XPoint aSize )
{
  XInt32 dx;
  XInt32 dy;

  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );
  dx = aSize.X / 8;
  dy = aSize.Y / 8;
  CoreRectView__OnSetBounds( &_this->Image, EwNewRect( 0, 0, aSize.X, aSize.Y ));
  CoreRectView__OnSetBounds( &_this->Text, EwNewRect( dx, dy, aSize.X, aSize.Y ));
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->TouchHandler, EwNewPoint( dx, dy ));
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->TouchHandler, EwNewPoint( aSize.X, 
  dy ));
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->TouchHandler, EwNewPoint( aSize.X, 
  aSize.Y ));
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->TouchHandler, EwNewPoint( dx, aSize.Y ));
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
void CutterMainButton_UpdateViewState( CutterMainButton _this, XSet aState )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aState );

  switch ( _this->Mode )
  {
    case CutterButtonModeStart :
    {
      ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0039 ));
      ViewsImage_OnSetColor( &_this->Image, CutterStartColor );
    }
    break;

    case CutterButtonModeStop :
    {
      ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const003E ));
      ViewsImage_OnSetColor( &_this->Image, CutterStopColor );
    }
    break;

    case CutterButtonModeOk :
    {
      ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const003F ));
      ViewsImage_OnSetColor( &_this->Image, CutterStartColor );
    }
    break;

    default : 
      ;
  }

  if ( _this->TouchHandler.Down && _this->TouchHandler.Inside )
    ViewsImage_OnSetColor( &_this->Image, CutterSelectColor );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void CutterMainButton_Init( CutterMainButton _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  CoreRectView__OnSetBounds( _this, EwSetRectSize( _this->Super2.Bounds, _this->Image.Bitmap->FrameSize ));
}

/* 'C' function for method : 'Cutter::MainButton.OnSetMode()' */
void CutterMainButton_OnSetMode( CutterMainButton _this, XEnum value )
{
  _this->Mode = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterMainButton_enterLeaveSlot( CutterMainButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterMainButton_pressReleaseSlot( CutterMainButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->TouchHandler.Down && _this->TouchHandler.Inside )
  {
    switch ( _this->Mode )
    {
      case CutterButtonModeStart :
        EwSignal( _this->OnStart, ((XObject)_this ));
      break;

      case CutterButtonModeStop :
        EwSignal( _this->OnStop, ((XObject)_this ));
      break;

      case CutterButtonModeOk :
        EwSignal( _this->OnOk, ((XObject)_this ));
      break;

      default : 
        ;
    }
  }
}

/* Variants derived from the class : 'Cutter::MainButton' */
EW_DEFINE_CLASS_VARIANTS( CutterMainButton )
EW_END_OF_CLASS_VARIANTS( CutterMainButton )

/* Virtual Method Table (VMT) for the class : 'Cutter::MainButton' */
EW_DEFINE_CLASS( CutterMainButton, CoreGroup, OnStart, OnStart, Image, Image, Mode, 
                 "Cutter::MainButton" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CutterMainButton_UpdateLayout,
  CutterMainButton_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterMainButton )

/* This is a bitmap resource. */
EW_DEFINE_BITMAP_RES( CutterButtonBig )
  EW_BITMAP_FRAMES( CutterButtonBig, Default, EW_DRIVER_VARIANT_ALPHA8, 93, 93, 0 )
    EW_BITMAP_FRAME( 21, 21, 71, 71, 0x00000000, 0x00000000 )

  EW_BITMAP_PIXEL( CutterButtonBig, Default )                   /* ratio 14.15 % */
    0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x231087C3, 0xB1489C4A, 0x13054162,
    0x81207832, 0x0FC784E2, 0x183C2A1C, 0x49A4B160, 0xB82C0882, 0x0D25D230, 0x0E833E1D,
    0x13448A35, 0x7E432252, 0x0319B0F0, 0x0687C705, 0x6D0A4E01, 0x15872178, 0x35188839,
    0xD1A8F47A, 0x128A4522, 0x288A9A21, 0x8C45A291, 0xC4723D1C, 0x309F5168, 0x18144A45,
    0x86E0764A, 0x35046D20, 0xD108B482, 0x08854321, 0x2824120D, 0x78402050, 0x3415D901,
    0x43211088, 0x72291087, 0x9E10C904, 0x94122929, 0xC6E0B92D, 0x1A904299, 0xA8644229,
    0x9747A22E, 0x85F2C942, 0x6EB5D9CC, 0x6B41B9A0, 0x2A172C16, 0x1116914D, 0x3048540A,
    0x9874A6A3, 0xD12C5868, 0x8E3C90C1, 0x243A291C, 0x57A4D0B7, 0xEBADB2C7, 0xF1B8AFF7,
    0xCDFFC8E3, 0x970CE703, 0x06834427, 0xF4B44A19, 0xB41116D0, 0xB2304A1B, 0xCADB51E7,
    0xE2E53286, 0xF9B92F56, 0xA9F3C7F3, 0x918A563B, 0x12874220, 0x05F43A3D, 0x0783F67B,
    0x8EF1F88A, 0x4820D0A8, 0x0F637CB3, 0x5816047A, 0x68C63A5C, 0x8220C862, 0x1808A23D,
    0xD047DC1D, 0xC240700A, 0x08842208, 0xEF310952, 0x81A1E818, 0x4A6360FA, 0xA2188221,
    0x1C8423C8, 0x137DC0A1, 0x2348B168, 0xC98209FD, 0x687CF331, 0x68FD81A3, 0x3121CA93,
    0x0B87389D, 0x87E0C5B4, 0xF208874D, 0xDE0331C8, 0xE081A4A8, 0x6B483284, 0x0532348D,
    0xE0194302, 0x1DD05604, 0x4B3ACB02, 0x32A20976, 0x9F289492, 0x549C1C1B, 0xF20E1A01,
    0x5E3649A8, 0x3C6069B6, 0x9AD21CB1, 0x50D88123, 0xF5FB0A12, 0xF72D8837, 0x8167E9B8,
    0x082240D5, 0x41362F22, 0x183344D2, 0xD07FE302, 0x8128E9FC, 0x482298E8, 0x315C907C,
    0x2C890F11, 0x98204872, 0xA7E8F398, 0x8B63EDE7, 0x24488B98, 0xAA3C5384, 0xACA80F22,
    0x328FE726, 0x4892148A, 0x55743C44, 0xB4FC2DE4, 0xDB722BAA, 0x44084234, 0x20B55835,
    0x7B16BB2F, 0x3C8F37DC, 0x9A218274, 0xB1AC420E, 0x8DC3FED0, 0xE1F81743, 0x44B88F08,
    0x07436DAB, 0x2208F1B0, 0x8FE2A880, 0x5436D1B6, 0x0C8923C5, 0xE43C4972, 0x0D10BB96,
    0x8852248D, 0xEBB4E824, 0xF814426E, 0xD85C08C1, 0x422F7BD8, 0x63C2405D, 0x6BFEFE2E,
    0xA48C0C50, 0x053E49AB, 0x02D09B43, 0xD8123075, 0x8C3B0D36, 0x20901210, 0x152D4822,
    0x04906C5B, 0xDBE22C80, 0x8B171DAA, 0x0B021310, 0xC6C94CA2, 0xC841341E, 0x30C26C83,
    0x741EBBCB, 0x3483BA04, 0x05C9B343, 0xC887902D, 0x73B3E09A, 0xE0D905CD, 0x0CC87408,
    0x0240F44D, 0x2188E198, 0xB4A3C08F, 0x506505AE, 0xCC822288, 0x04D53532, 0x21B02105,
    0x6ADD6CDF, 0x9AF86C41, 0x8362D089, 0x8105904A, 0xC108222C, 0x503DAB69, 0xC216300E,
    0x0B72DC4D, 0x93614584, 0xA3B78924, 0x87302103, 0xFDFCB07B, 0x4C1740E9, 0x8631C825,
    0x0B903A3F, 0x4DA22312, 0x740B80E3, 0xB3502344, 0x039FF943, 0x2148F179, 0xB9B71096,
    0xA005029B, 0x8CA38A06, 0x0A40A7EE, 0x20C8D1CC, 0x3A4EA8B2, 0xE1F01300, 0xB7B22FF6,
    0x83069029, 0xD2481228, 0xD02977B9, 0xD2055507, 0x0A4BF068, 0x81BC4244, 0xF27C835C,
    0x71909800, 0x7F3F1420, 0x442740A3, 0xD6376E02, 0xF6400F5F, 0xA36F77DB, 0x92382540,
    0x8AFD2048, 0x2140A1EF, 0x34FE722C, 0xC00FAFEA, 0xB2075706, 0x0A06FC4C, 0x6E1FC164,
    0x817F830F, 0x1DC07102, 0x6C883118, 0x023D0FFD, 0x1C1B8021, 0x1C548841, 0x21028110,
    0xF134E7C3, 0x240CF3C0, 0x17988E0A, 0x9C982A3D, 0x6240E081, 0x0707088C, 0x10103391,
    0xCF1086EC, 0x0671E118, 0xF83E80D2, 0x342A174E, 0xC0C6D30E, 0x23EC4FC0, 0x8820FF86,
    0x88863211, 0x2090E074, 0x20CF8824, 0x8242A780, 0x83806014, 0x46E88528, 0x3C299058,
    0xE08C10C2, 0x92091152, 0x2E2D1802, 0x40490586, 0x3336CC1D, 0x09641A0E, 0xF54439F3,
    0xE9B558BC, 0x5315820C, 0xBA004824, 0x88442DF0, 0x31068983, 0x21122241, 0xB1C05688,
    0x9A700120, 0x0E311021, 0x2A31C632, 0xB8C53082, 0x0110881E, 0x283E6188, 0x404260AC,
    0x54429F10, 0x540746A2, 0x15BE13C8, 0x32105888, 0x03C00D21, 0x3E910EE6, 0x06042DFC,
    0x24842192, 0x8549E6A4, 0x190AC004, 0x6E2AC41B, 0x40863F18, 0x18B68ACC, 0x221AF8A4,
    0x4A684282, 0x9F53F710, 0x19206E43, 0x966DDD66, 0xC642690E, 0xB088D10A, 0x0F1EAF59,
    0xDC160021, 0x8C478847, 0x38F37731, 0x00004045, 0x00000000

  EW_BITMAP_CLUT_EMPTY( CutterButtonBig, Default )

  EW_BITMAPS_TABLE( CutterButtonBig )
    EW_BITMAP( CutterButtonBig, Default )
EW_END_OF_BITMAP_RES( CutterButtonBig )

/* Table with links to derived variants of the bitmap resource : 'Cutter::ButtonBig' */
EW_RES_WITHOUT_VARIANTS( CutterButtonBig )

/* This is a font resource. */
EW_DEFINE_FONT_RES( CutterMainButtonFont, 26, 7, 0, 16, 0x0001, 0x0035 )
  EW_GLYPH( 0x0001, 1, -20, 10, 20, 12, 0x00000000 ),
  EW_GLYPH( 0x0041, 0, -20, 18, 20, 18, 0x0000024A ),           /* 'A' */
  EW_GLYPH( 0x0042, 2, -20, 14, 20, 17, 0x00000576 ),           /* 'B' */
  EW_GLYPH( 0x0043, 1, -20, 16, 20, 18, 0x0000082C ),           /* 'C' */
  EW_GLYPH( 0x0044, 2, -20, 15, 20, 18, 0x00000B62 ),           /* 'D' */
  EW_GLYPH( 0x0045, 2, -20, 13, 20, 16, 0x00000DEB ),           /* 'E' */
  EW_GLYPH( 0x0046, 2, -20, 13, 20, 15, 0x00000FC3 ),           /* 'F' */
  EW_GLYPH( 0x0047, 1, -20, 16, 20, 19, 0x0000116F ),           /* 'G' */
  EW_GLYPH( 0x0048, 2, -20, 16, 20, 20, 0x000014B6 ),           /* 'H' */
  EW_GLYPH( 0x0049, 2, -20, 4, 20, 8, 0x00001661 ),             /* 'I' */
  EW_GLYPH( 0x004A, 0, -20, 14, 20, 15, 0x000016C0 ),           /* 'J' */
  EW_GLYPH( 0x004B, 2, -20, 16, 20, 18, 0x00001892 ),           /* 'K' */
  EW_GLYPH( 0x004C, 2, -20, 13, 20, 15, 0x00001B4C ),           /* 'L' */
  EW_GLYPH( 0x004D, 2, -20, 21, 20, 24, 0x00001C87 ),           /* 'M' */
  EW_GLYPH( 0x004E, 2, -20, 16, 20, 20, 0x00001FD9 ),           /* 'N' */
  EW_GLYPH( 0x004F, 1, -20, 17, 20, 19, 0x00002209 ),           /* 'O' */
  EW_GLYPH( 0x0050, 2, -20, 15, 20, 18, 0x00002561 ),           /* 'P' */
  EW_GLYPH( 0x0051, 1, -20, 17, 24, 19, 0x000027B3 ),           /* 'Q' */
  EW_GLYPH( 0x0052, 2, -20, 15, 20, 17, 0x00002BBC ),           /* 'R' */
  EW_GLYPH( 0x0053, 1, -20, 15, 20, 17, 0x00002E46 ),           /* 'S' */
  EW_GLYPH( 0x0054, 0, -20, 17, 20, 17, 0x000031E2 ),           /* 'T' */
  EW_GLYPH( 0x0055, 1, -20, 16, 20, 18, 0x000033B6 ),           /* 'U' */
  EW_GLYPH( 0x0056, 0, -20, 18, 20, 18, 0x000035D3 ),           /* 'V' */
  EW_GLYPH( 0x0057, 0, -20, 25, 20, 25, 0x000038F2 ),           /* 'W' */
  EW_GLYPH( 0x0058, 0, -20, 17, 20, 18, 0x00003D5A ),           /* 'X' */
  EW_GLYPH( 0x0059, 0, -20, 17, 20, 17, 0x000040AD ),           /* 'Y' */
  EW_GLYPH( 0x005A, 1, -20, 15, 20, 17, 0x0000432F ),           /* 'Z' */
  EW_GLYPH( 0x0061, 1, -15, 13, 15, 15, 0x000045EA ),           /* 'a' */
  EW_GLYPH( 0x0062, 2, -21, 13, 21, 16, 0x00004855 ),           /* 'b' */
  EW_GLYPH( 0x0063, 1, -15, 13, 15, 15, 0x00004A94 ),           /* 'c' */
  EW_GLYPH( 0x0064, 1, -21, 13, 21, 16, 0x00004CC9 ),           /* 'd' */
  EW_GLYPH( 0x0065, 1, -15, 13, 15, 15, 0x00004F0B ),           /* 'e' */
  EW_GLYPH( 0x0066, 1, -21, 9, 21, 10, 0x0000517E ),            /* 'f' */
  EW_GLYPH( 0x0067, 1, -15, 13, 21, 16, 0x000052CC ),           /* 'g' */
  EW_GLYPH( 0x0068, 1, -21, 13, 21, 15, 0x000055CA ),           /* 'h' */
  EW_GLYPH( 0x0069, 2, -20, 3, 20, 7, 0x0000577F ),             /* 'i' */
  EW_GLYPH( 0x006A, -1, -20, 6, 26, 7, 0x000057EF ),            /* 'j' */
  EW_GLYPH( 0x006B, 1, -21, 14, 21, 14, 0x00005909 ),           /* 'k' */
  EW_GLYPH( 0x006C, 2, -21, 3, 21, 7, 0x00005B50 ),             /* 'l' */
  EW_GLYPH( 0x006D, 2, -15, 21, 15, 25, 0x00005B9E ),           /* 'm' */
  EW_GLYPH( 0x006E, 1, -15, 13, 15, 15, 0x00005DFE ),           /* 'n' */
  EW_GLYPH( 0x006F, 1, -15, 14, 15, 16, 0x00005F5F ),           /* 'o' */
  EW_GLYPH( 0x0070, 2, -15, 13, 21, 16, 0x000061CE ),           /* 'p' */
  EW_GLYPH( 0x0071, 1, -15, 13, 21, 16, 0x00006428 ),           /* 'q' */
  EW_GLYPH( 0x0072, 1, -15, 8, 15, 9, 0x00006684 ),             /* 'r' */
  EW_GLYPH( 0x0073, 1, -15, 12, 15, 14, 0x00006755 ),           /* 's' */
  EW_GLYPH( 0x0074, 0, -19, 9, 19, 9, 0x000069E6 ),             /* 't' */
  EW_GLYPH( 0x0075, 1, -15, 13, 15, 15, 0x00006B02 ),           /* 'u' */
  EW_GLYPH( 0x0076, 0, -15, 14, 15, 14, 0x00006C59 ),           /* 'v' */
  EW_GLYPH( 0x0077, 0, -15, 21, 15, 21, 0x00006E68 ),           /* 'w' */
  EW_GLYPH( 0x0078, 0, -15, 14, 15, 14, 0x00007191 ),           /* 'x' */
  EW_GLYPH( 0x0079, 0, -15, 13, 21, 13, 0x000073DA ),           /* 'y' */
  EW_GLYPH( 0x007A, 1, -15, 12, 15, 14, 0x0000766E ),           /* 'z' */

  EW_FONT_PIXEL( CutterMainButtonFont, 0x00007848 )             /* ratio 55.00 % */
    0xBDEF7BFF, 0x9CFFDEF7, 0xEF7BDEF7, 0xFE404533, 0x937C26F9, 0x702223FC, 0xFA83F326,
    0xF5D807B3, 0xCE7981FC, 0x000C6204, 0x60063100, 0x7F81339E, 0x9B507F3D, 0xE64CE03D,
    0x2223FC87, 0x937C26F8, 0x39BFC808, 0xDEF7BDEF, 0xBDC60267, 0xF1F8000B, 0x63260003,
    0x3113C000, 0xD570007C, 0x1E200167, 0x003C4577, 0xE3C4F14C, 0x38B3F000, 0xF0C6012A,
    0xC00F09E7, 0x03E50A27, 0x595C1658, 0x783E13C0, 0x72381FC2, 0x2C816580, 0xF7BDEF79,
    0x01E1314E, 0xEA7C3F80, 0xBDEF7BDE, 0xE95255B7, 0xF8F9300F, 0xE510098B, 0xE002CB11,
    0x0FE15AC8, 0xEFCE13C0, 0xBDEF7BDE, 0x4001EFFF, 0xDC0FDF3E, 0x53DDEF7B, 0x5BC600BD,
    0x62FE0016, 0x00983000, 0x9F00260C, 0x1BF5001C, 0xF7BDC2F1, 0x7F8D5FDE, 0x80BF2001,
    0xE7BDEF7B, 0x6200FF35, 0xA002C67E, 0x34C006D2, 0x004D3001, 0x7201B49E, 0xBDC4E4FD,
    0x477FDEF7, 0xEA88009E, 0xEFE600FF, 0x813BB3DC, 0xC42B7FF1, 0xF8C27B58, 0xDEE7BE67,
    0xC1F788DF, 0xA833DC89, 0xA5617E5D, 0xE98B4013, 0x48C00C67, 0x801F863B, 0x04878E6F,
    0x260EDEB0, 0x00000000, 0x04000000, 0x09260013, 0xE99DBD60, 0xC39BE007, 0x48C00C67,
    0x80374ADB, 0xC6379316, 0xE5DA887E, 0xF997D417, 0x237E7BDD, 0xDFEA07BE, 0x3EFAA215,
    0xF7BDEEFC, 0x00F7FFDE, 0x1EDF3E40, 0xFEF7BDC0, 0x007FC73B, 0xFC47B5C8, 0x47F88003,
    0x5A620034, 0x289F8001, 0x00FE0C00, 0x48000290, 0x90000000, 0x00029000, 0x7E003F83,
    0x69C800A2, 0x11BC8005, 0xCED6300D, 0x7BDC07F8, 0x7B18EEEF, 0xBF98C400, 0x7BDDF89D,
    0xDEF7BDEF, 0x0000FF7B, 0xEF7BDEF7, 0x000FF7BD, 0x00000000, 0x00000000, 0xEF7BDEE0,
    0x0005F7BD, 0xF7BDEF70, 0x0002FBDE, 0x00000000, 0x00000000, 0xF7BDEF70, 0x002F7BDE,
    0xEF7BDDF8, 0x7BDEF7BD, 0x7B80005F, 0xDEF7BDEF, 0x000002FB, 0x00000000, 0xB8000000,
    0xEF7BDEF7, 0xEE00001D, 0x7BDEF7BD, 0x00000007, 0x00000000, 0x00000000, 0x7EB00000,
    0x26FD9EE7, 0xC42BA750, 0xF505FF98, 0xDEE7BE45, 0x3785E95F, 0x7EA0CF72, 0x9D2B4E8F,
    0xE7E17E00, 0x3C400C67, 0xF002862A, 0x001C9D9D, 0x0018B111, 0x80000010, 0xF7BDEF71,
    0x2C0000DE, 0xEF7B8C06, 0x01708C9D, 0xC000A3C0, 0x9E000B91, 0xC06016F2, 0x20F5F19B,
    0x7228F7EA, 0xEF7DAC57, 0x43F995F9, 0x924F7EFC, 0xEFD7FFD5, 0x7DEE005E, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0xF7BDEF70, 0x002F7BDE, 0xBDEF7000, 0x2F7BDEF7,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0001BDD6, 0x00000000, 0x00000000,
    0x0EF73C00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x7F200000,
    0xD54002BF, 0xF4C89200, 0xE919F103, 0xF506FC67, 0x6A5CCD4F, 0xF11BFBDF, 0x24CF7A16,
    0xEEFC9EE7, 0x5DEEF805, 0x5CC7E800, 0x79E5CC00, 0xF55E7801, 0xC67E3001, 0x23FE4007,
    0x1B88004F, 0xFC401271, 0xC001BE33, 0xF0017437, 0x8F001715, 0xAC001F34, 0xE0097E9D,
    0x071138E9, 0x8D78C7C0, 0x99560017, 0xD4BE0013, 0x11388009, 0x6BC60007, 0x1EB0007C,
    0x5F8004CF, 0x17BBF1EA, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0xBDEF7B80, 0x00FBDEF7, 0x7FBDDF80, 0xDEF75800, 0x3E001640,
    0x25400380, 0x1C002A10, 0x013113C0, 0xF2C03CA2, 0x0B88CC01, 0x9F00B27C, 0x00E18C02,
    0xF9605E95, 0x0C638054, 0x0988AC20, 0xB1003E51, 0x9623307C, 0xE164F804, 0x183002B3,
    0x07D2A1C3, 0x3854F810, 0x198000C6, 0x007CA331, 0x88CDF2C0, 0xF27C0009, 0xA30000B5,
    0xE0000F84, 0x80000D4F, 0xDE00FE19, 0xBDC00B7B, 0x40005E2F, 0x398000BE, 0x00384000,
    0xC00365A0, 0xC000732C, 0x2001711B, 0xC001635F, 0x8004DE1A, 0x4002F113, 0xC0026ABE,
    0xE0003A1A, 0xC6000F8A, 0x74000F52, 0x578001FA, 0x009A8000, 0x00013780, 0x9DFCC02F,
    0xC00AF67B, 0x18856FD8, 0xFF901F7D, 0xF7B9EF91, 0x709BE237, 0x6A0CEF22, 0xC3993A3F,
    0xA22FC09B, 0xC4028CF9, 0x0FC31154, 0x924F16C0, 0x64C61E00, 0x000E0802, 0x00000000,
    0x40132600, 0xF0049270, 0x01F86230, 0x19F1E2D8, 0xC2A98805, 0x7E04DE1C, 0xB9138D11,
    0xD1FB5067, 0xDE23DC89, 0xC46FEF73, 0xB7EE4137, 0x0FBE8C42, 0xDEF7BDD6, 0x0B7EEF7B,
    0x7DFCC600, 0xEF7BDCE0, 0x03E78FDD, 0x53D37C60, 0x07F0E800, 0x00090C40, 0x00218000,
    0x3005C2B0, 0x9C1E36DE, 0x7BBDEF7B, 0xF2007F1A, 0xB9C076F9, 0xFFFBDEF7, 0x0000001E,
    0x00000000, 0x00000000, 0x58800000, 0x9BF3DCFB, 0x11EDCF00, 0x00FFDEA2, 0x77E317CF,
    0xFF3379EF, 0xBB53F903, 0x0E33B988, 0xE203E23A, 0x0ACA29C8, 0x4F3E18C0, 0x19F803F8,
    0x24007831, 0x93000879, 0x00000009, 0x43800000, 0xE0C49800, 0x4F3C9001, 0x19F803F8,
    0x60056511, 0xE23A3F0C, 0x45C8E203, 0x622ED4FE, 0xCF038CEE, 0xEF77E317, 0x03FE3379,
    0x442BB73C, 0xAC409B8A, 0xBFE7B9DF, 0xD0005D8C, 0x0097C45F, 0x97C7FCF0, 0xF2EC6000,
    0x7BDEF7BF, 0x000AFEEF, 0x05FBF3C6, 0xDEF7BDE4, 0x800FA8FD, 0x00554DF1, 0x22003668,
    0x0000000D, 0x34000D22, 0x37E3003F, 0xF7BC81CD, 0x7ACBBBDE, 0xC8376001, 0xFCDDEF7B,
    0x331E3000, 0x3F11D001, 0x0732E400, 0x00343A00, 0x4002AA62, 0x36800687, 0xB9DFAC26,
    0xF1827767, 0xD6310AEF, 0xCCFF185E, 0x2BBBDCEF, 0xF711D17A, 0xF1EFC609, 0xB4095286,
    0x2400C198, 0xC00F48FF, 0x78C79FBD, 0xFE65D001, 0x75F80067, 0x2B7E7BE4, 0xA5BCEB00,
    0x9801BEFA, 0xCC55F3BF, 0xF9E2003B, 0x007F8CED, 0xBF788E30, 0x1A46007B, 0x8600317F,
    0x4F01A198, 0xFFAB47F8, 0xE4FF6413, 0xEFCCBF32, 0x315FFBDC, 0xFBFF9E2E, 0x3B7CF924,
    0x7BDEF73D, 0xDEF7BDEF, 0x000DEF7B, 0xF7BDCF00, 0xF7BCCBDE, 0x00037BDE, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0xDC400000, 0x7B9E00BB, 0x00000007, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x03691124, 0xF53E996C, 0x58794F00, 0x5C93AFC3, 0xAFF1A33F,
    0xFF7BFF58, 0xF887F51D, 0x9F249EFD, 0x7BA82FDF, 0xDEEB0017, 0x800FC2A7, 0x234C789F,
    0xFC587201, 0x3E1E0078, 0xFC01F0C6, 0x0E4FC0E4, 0x4C1F0C60, 0x078FC07E, 0xA3C03CA2,
    0x60391C12, 0x84F03FA4, 0x01627E0F, 0x05478296, 0x23072FE0, 0x1F4A801D, 0x8F002C9F,
    0xF000F2C5, 0x03F0AA89, 0xDFFF8720, 0xACF80018, 0x2B0007E5, 0x40026222, 0x80004F14,
    0xB7B801CB, 0x80DDEF01, 0x0A186F75, 0xFE008CB0, 0x46049432, 0xB6294050, 0x41B0A018,
    0x8143CD86, 0x2C502CAD, 0x9443F862, 0x995C062B, 0xA8941478, 0xB058F052, 0xDB2B6245,
    0x8A1B0C46, 0x83CAB942, 0x97488287, 0x2888CA91, 0x8A5048B6, 0x33658232, 0xD8657014,
    0x1E1ED87E, 0x3C78A07F, 0x8473250A, 0x8AEC3202, 0x1EA18818, 0xB7250031, 0xF5F67E05,
    0x6CB36C00, 0x00DA5650, 0x46050C65, 0x3200A3E4, 0xC44A00A8, 0xC06C5000, 0x4FE001E6,
    0xF81B1601, 0xBF005BDD, 0x786F1F7B, 0x1E794F01, 0x5A06CAF9, 0x3986B0FC, 0xDC0753A0,
    0xD4B18B88, 0x931A6304, 0xA7405E2B, 0x04CC358E, 0xCAF7C578, 0x0A8F5006, 0xC17E002F,
    0x240C002C, 0x3D41BC00, 0xA4F2CC00, 0xCB44005F, 0xC01F8B5F, 0xE53C742F, 0xF51EA029,
    0x781786F0, 0x657C87C5, 0x80DE9BC3, 0x98C1CC35, 0xB8AE02C6, 0x6301711C, 0xBBDD6B1A,
    0xD6F77C00, 0x3E402F4A, 0x02C4FCAA, 0x78732F34, 0x8998F181, 0xE2D02C57, 0x03794F07,
    0x2D04CCA3, 0x6068B45E, 0xD2A8663C, 0x6AF11D00, 0x99720139, 0x3A00686B, 0x80078DA2,
    0x60017178, 0x60000631, 0x00000001, 0x00000000, 0x00000000, 0xDEF78000, 0xF7BDEF7B,
    0x00EF7BDE, 0xDEF7BC00, 0x77BDEF7B, 0x23800353, 0x753A003E, 0x33CB1800, 0x07E23801,
    0x017E9BC0, 0x01E7A6A0, 0x003F95E0, 0x8006F4DE, 0x10029E9A, 0xF001FCAF, 0xA8006F4B,
    0xF1002CC7, 0x5F8017C9, 0x1E780178, 0x997800B3, 0xEF7BDEF7, 0x01BEF7BD, 0xCEF9E000,
    0xF109BF3D, 0xF5109EFB, 0xBC55F0FE, 0x96A99EE7, 0xBC433D2C, 0x16FBE3E5, 0x4F3E52D8,
    0xEFD60400, 0x186DEF7F, 0xC404AEFF, 0xDFFDF33F, 0x9C331B7B, 0x07238099, 0xFC023080,
    0xE0DF13F0, 0x568C0FF9, 0x4C6FDEFB, 0xC855FAD2, 0xDEEA3F75, 0x00000017, 0x00000000,
    0xDE880000, 0x901EFCF7, 0x5F510D7B, 0xBBDDF943, 0xBB107ACB, 0xB1A1BF18, 0x00727C02,
    0x82003C58, 0x00038000, 0x00124007, 0xE095878A, 0x1F710633, 0x31786FC6, 0xB2EEF77D,
    0x86BDEC1E, 0xE8835FA8, 0x80EEEF7D, 0xD6306BBA, 0xC62F8C1E, 0xE65FDEEF, 0x8BB91D09,
    0x966F91BA, 0x8AA5C81F, 0x075F7E02, 0x0053E409, 0x01240000, 0xBE80B160, 0xC40FCB3B,
    0x2EE474A3, 0x1AF99FEA, 0xBDDF8C5F, 0xA82791BF, 0xFD1886BB, 0xEF78001F, 0x0000001F,
    0x00000000, 0xEE77EA00, 0x5FAC03BF, 0x41FB5C85, 0xFDEFB1B5, 0x07B96819, 0xFC3193B5,
    0x050F1440, 0x020090E0, 0x0E000200, 0x0161E001, 0xC40FC318, 0xA0DFCB42, 0xB55EA076,
    0x078DFDEF, 0xEB90AEE6, 0xBEF441FE, 0xF6302F67, 0x3FF4621A, 0xBED67FC9, 0xBC3C8BF7,
    0xE7837E31, 0x2F2C9E33, 0x0F1761A8, 0xF72E78F0, 0x9DEF7BDE, 0xF7900002, 0xBDEF7BDE,
    0x00647FF7, 0x31801628, 0x85F18274, 0x721F711B, 0x7C659FFD, 0xFFBDDF91, 0xBBA9358E,
    0x1BB9C846, 0x3FB9DFCC, 0x45856FBC, 0x9BB9CF4D, 0x3480BE2D, 0x2DEE0080, 0xE005EF79,
    0x5EF792DE, 0x00000000, 0x00000000, 0x00000000, 0x9DFA8000, 0xEE78EFFB, 0xE42AFE65,
    0x1AF517BA, 0xF97F7BBF, 0xA8BFE5A0, 0x17C3927F, 0x00F14510, 0x00001208, 0x02490000,
    0x2C801458, 0xB42880BF, 0x276A0FFC, 0x7BEC6BC6, 0xDCC0637F, 0x1F75C84F, 0xFF73BE30,
    0x7801241D, 0xF00EE34C, 0xF8BF7F0C, 0x27F310AF, 0xE7D19EA7, 0xE2F2379E, 0xF54533DD,
    0x077B8D3F, 0x00000000, 0x00000000, 0xFFEF9000, 0xDF806F9E, 0x7F9E299E, 0xBDF67AC1,
    0xFD43445F, 0x246BCF06, 0x1626282E, 0x00000040, 0x00000000, 0x00000000, 0x80000000,
    0x65ACBDBE, 0xEEFC6DF5, 0x0000000F, 0x677C0000, 0x88FC4F11, 0x677E1F8A, 0x17DFF801,
    0x00000000, 0x00000000, 0x00000000, 0x77F749E0, 0x3C52B93F, 0x05DEE237, 0x00000000,
    0x00000000, 0xFF900000, 0x38804FFB, 0xF1013719, 0x804FF11F, 0x0037C46F, 0x0057E57E,
    0x801F796F, 0x2009CA6F, 0x358017C9, 0xE9E007CD, 0xE025F877, 0x003889CF, 0xC00D1B64,
    0xF001569C, 0x70005F0E, 0xFBFF3E22, 0x00000002, 0xC0000000, 0xDF6B2EFF, 0xBED627B3,
    0xFFF82F67, 0xBEF1887B, 0x4621EFF9, 0xEFC4F0FF, 0x3BCC3DDE, 0x8FFF7BDE, 0x317FC87A,
    0xF24DF90E, 0xC80A974B, 0x132C0B21, 0x218132C0, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0xC0000000, 0xFBE46F73, 0x601BE7BF, 0xF9E299ED, 0xEFB3F417,
    0x9E1A22FD, 0x8D79E0DF, 0x00228344, 0x00000260, 0x00000000, 0x00000000, 0x00000000,
    0x777BEF44, 0x06BBA807, 0xF8C2DD63, 0xFDEEFC62, 0x3706F88D, 0xCFF51772, 0x20D0C6B8,
    0xF13F18DA, 0xA5F84F00, 0x4C329001, 0x02C22580, 0xC0252298, 0xF0145B90, 0x0BC35D84,
    0xE46E51B4, 0x5C676A2E, 0x77E317C6, 0x37C46FEF, 0xC60D7750, 0x4FBDC5BA, 0x5F9EFFF4,
    0x8A577B81, 0xF93C35FA, 0xAC37F7BB, 0xFA8BB907, 0xE20517A7, 0xC5000C67, 0x00092003,
    0x24000000, 0x28A1E001, 0xE4919F10, 0x1A9FEA2E, 0xFEF77F23, 0xF6785586, 0x06BF514A,
    0x7E7BFFD9, 0x00000005, 0x00000000, 0xB9DFA800, 0xFF758EFF, 0xAE42AFF4, 0x6A5CC3FB,
    0x031BFBDF, 0xA837E33F, 0x417A549D, 0x800F064C, 0x00000123, 0x0048E000, 0x52A00506,
    0xC6D07883, 0xC0ECF06F, 0xF7BED4B3, 0xABFD091B, 0x504EEB90, 0x1DFF73BF, 0x00000000,
    0x00000000, 0x678DEE30, 0x37DF8DDF, 0xEE759E04, 0xFC03AB1F, 0x00000000, 0x00000000,
    0xE8800000, 0x4DFFEF7D, 0x3906BB8C, 0xEC627177, 0x5F197F7B, 0xBA83BA56, 0x72020BE2,
    0x1378B3CD, 0xEAD99DFA, 0x6B01FBBF, 0x77D6313F, 0xCEF8C0DE, 0x1DCC47AF, 0x64FED6A0,
    0x9E0DFEFF, 0xC03E44F2, 0x5DC8DEB2, 0xC7FC3BE4, 0xDEEFC62F, 0x67A734FF, 0xBF3E41AF,
    0x01DEE317, 0xDC000000, 0x02F7D9BB, 0xDF66EF70, 0x0000000B, 0x00000000, 0x902C0000,
    0x799441FC, 0x219BE06F, 0x300EF73D, 0x00007DEE, 0x00000000, 0x00000000, 0x00000000,
    0x43C00040, 0xBC310F81, 0xDC2778C5, 0x7BBDDF88, 0x13F6303E, 0xF21FBEA2, 0xF75807DE,
    0xFC039644, 0x09EFE13C, 0x22B0E188, 0x8878960A, 0x82BF81C7, 0x71E209E3, 0x2A2889E0,
    0xB972C0A2, 0x9457F016, 0xE78CA80F, 0xC7801C89, 0x9F801EDD, 0x31803E5A, 0xF8024622,
    0xE1E003F1, 0x00EF7501, 0xDD417DDF, 0x28142A3B, 0x1E1C8605, 0x0390A87F, 0xE0AFE2B6,
    0x363C142F, 0xFB4A0625, 0xA99142A1, 0x8C663918, 0x63C0A864, 0x1E53F3F3, 0x43DB0DB6,
    0x51E6D729, 0x23951150, 0x8A8A8A87, 0x1E31E01C, 0x286A3C45, 0x7F6F6FE0, 0x036DDDFC,
    0x4B0A2AA5, 0x9900F13D, 0x3C454414, 0x6C170F00, 0x0516C036, 0xEFFC0A1E, 0x3FB9E41F,
    0x8DE0B95E, 0x9BC7A978, 0xFC4DE53C, 0x16369A32, 0x656A7B44, 0xBC79E01C, 0xBF0137AF,
    0x200B18C8, 0x5C801F16, 0x189C00FC, 0x0D602F91, 0xF902CDB7, 0x1796639A, 0x474D11BC,
    0x83A598BE, 0xAD92CD31, 0xBD7A7426, 0x7B8806F7, 0x140B3FDF, 0xB83FA55F, 0x40547872,
    0x50B1F879, 0xFE18C7E1, 0x9F81CBF8, 0x3C1F2827, 0x40F48CA2, 0x048CA396, 0xFCFC4F7F,
    0x87311580, 0x46D28807, 0x007E2E02, 0x9401304F, 0x1FD3000A, 0xC00B0E40, 0x398009E7,
    0xDF7B80BD, 0xF2900358, 0xEF7BC06D, 0x7BDEF7BD, 0xF78002EF, 0x3DEF7BDE, 0x97E02729,
    0x8F580271, 0x55E30077, 0x3E22700B, 0x00BE9BC0, 0x9015699E, 0x2701A33F, 0xF87A00E2,
    0x7CCB5025, 0xEF799700, 0x0BDEF7BD, 0x00000000

  EW_FONT_KERNING_CODES( CutterMainButtonFont )
  EW_FONT_KERNING_VALUES( CutterMainButtonFont )
EW_END_OF_FONT_RES( CutterMainButtonFont )

/* Table with links to derived variants of the font resource : 'Cutter::MainButtonFont' */
EW_RES_WITHOUT_VARIANTS( CutterMainButtonFont )

/* Initializer for the class 'Cutter::StatusBar' */
void CutterStatusBar__Init( CutterStatusBar _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->TimeText, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Icon1, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Icon2, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->Timer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterStatusBar );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0040 );
  CoreView_OnSetLayout((CoreView)&_this->TimeText, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->TimeText, _Const0041 );
  ViewsText_OnSetAlignment( &_this->TimeText, ViewsTextAlignmentAlignHorzRight | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TimeText, EwLoadString( &_Const0042 ));
  ViewsText_OnSetColor( &_this->TimeText, CutterSelectColor );
  CoreView_OnSetLayout((CoreView)&_this->Icon1, CoreLayoutAlignToBottom | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Icon1, _Const0043 );
  ViewsImage_OnSetColor( &_this->Icon1, CutterSelectColor );
  CoreView_OnSetLayout((CoreView)&_this->Icon2, CoreLayoutAlignToBottom | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Icon2, _Const0044 );
  ViewsImage_OnSetColor( &_this->Icon2, CutterSelectColor );
  ViewsImage_OnSetFrameNumber( &_this->Icon2, 1 );
  CoreTimer_OnSetEnabled( &_this->Timer, 1 );
  CoreGroup__Add( _this, ((CoreView)&_this->TimeText ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Icon1 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Icon2 ), 0 );
  ViewsText_OnSetFont( &_this->TimeText, EwLoadResource( &CutterSmallDigitFont, 
  ResourcesFont ));
  ViewsImage_OnSetBitmap( &_this->Icon1, EwLoadResource( &CutterStatusIcons, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Icon2, EwLoadResource( &CutterStatusIcons, ResourcesBitmap ));
  _this->Timer.OnTrigger = EwNewSlot( _this, CutterStatusBar_ClockSlot );

  /* Call the user defined constructor */
  CutterStatusBar_Init( _this, aArg );
}

/* Re-Initializer for the class 'Cutter::StatusBar' */
void CutterStatusBar__ReInit( CutterStatusBar _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->TimeText );
  ViewsImage__ReInit( &_this->Icon1 );
  ViewsImage__ReInit( &_this->Icon2 );
  CoreTimer__ReInit( &_this->Timer );
}

/* Finalizer method for the class 'Cutter::StatusBar' */
void CutterStatusBar__Done( CutterStatusBar _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterStatusBar );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->TimeText );
  ViewsImage__Done( &_this->Icon1 );
  ViewsImage__Done( &_this->Icon2 );
  CoreTimer__Done( &_this->Timer );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void CutterStatusBar_UpdateLayout( CutterStatusBar _this, XPoint aSize )
{
  XInt32 dx;
  XInt32 iconW;

  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );
  dx = ( aSize.X / 2 ) + 8;
  iconW = _this->Icon1.Bitmap->FrameSize.X + 8;
  CoreRectView__OnSetBounds( &_this->TimeText, EwNewRect( 0, 0, aSize.X / 2, aSize.Y ));
  CoreRectView__OnSetBounds( &_this->Icon1, EwNewRect( dx, 0, dx + iconW, aSize.Y ));
  CoreRectView__OnSetBounds( &_this->Icon2, EwNewRect( dx + iconW, 0, ( dx + iconW ) 
  + iconW, aSize.Y ));
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void CutterStatusBar_Init( CutterStatusBar _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  CoreRectView__OnSetBounds( _this, EwSetRectSize( _this->Super2.Bounds, EwNewPoint( 
  CutterScreenSize.X / 3, 32 )));
}

/* Slot method to update clock. */
void CutterStatusBar_ClockSlot( CutterStatusBar _this, XObject sender )
{
  CoreTime t;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  t = EwNewObject( CoreTime, 0 );
  t = CoreTime_OnGetCurrentTime( t );
  ViewsText_OnSetString( &_this->TimeText, EwConcatString( EwConcatString( EwNewStringInt( 
  t->Hour, 2, 10 ), EwLoadString( &_Const0045 )), EwNewStringInt( t->Minute, 2, 
  10 )));
}

/* Variants derived from the class : 'Cutter::StatusBar' */
EW_DEFINE_CLASS_VARIANTS( CutterStatusBar )
EW_END_OF_CLASS_VARIANTS( CutterStatusBar )

/* Virtual Method Table (VMT) for the class : 'Cutter::StatusBar' */
EW_DEFINE_CLASS( CutterStatusBar, CoreGroup, TimeText, TimeText, TimeText, TimeText, 
                 _None, "Cutter::StatusBar" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CutterStatusBar_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterStatusBar )

/* This is a bitmap resource. */
EW_DEFINE_BITMAP_RES( CutterStatusIcons )
  EW_BITMAP_FRAMES( CutterStatusIcons, Default, EW_DRIVER_VARIANT_ALPHA8, 22, 22, 0 )
    EW_BITMAP_FRAME( 4, 3, 14, 7, 0x00000000, 0x00000000 ),
    EW_BITMAP_FRAME( 4, 4, 3, 14, 0x0000011C, 0x00000000 ),
    EW_BITMAP_FRAME( 0, 0, 0, 0, 0x00000200, 0x00000000 )

  EW_BITMAP_PIXEL( CutterStatusIcons, Default )                 /* ratio 38.29 % */
    0x1C080100, 0x9E04A008, 0x35DAE461, 0x0083C4AB, 0x470287C1, 0x6F67022C, 0xFC7F3F5F,
    0xE543B9F0, 0x3140E203, 0x1C89FEEA, 0x24DF3249, 0x91031006, 0xD752B95C, 0x58668204,
    0xCA691CCA, 0xEE134080, 0x233CEA66, 0x9D4E92B0, 0x40C02AE2, 0x1AA753F7, 0xCE8AFA48,
    0x9F8D758C, 0xF0C49952, 0x6E979BF9, 0xE3753ADD, 0xBAB3A9C4, 0x74385C8E, 0xFBB5D556,
    0x3FDF0D07, 0xA84E2543, 0x8526930D, 0xE84F125A, 0xC26B50BC, 0xD768BFC8, 0xA81A25FE,
    0x24BDDEA3, 0xD35C1B02, 0x8D6067D7, 0xFD28C77F, 0x6040261B, 0x04549111, 0x0FF7186A,
    0xFF3E40D1, 0x7778CD6A, 0x22C0C72F, 0x8C0B1A4A, 0xC081A581, 0xB5FED205, 0x9FE9C019,
    0xD91E8F2C, 0x01973BFC, 0xFF788A80, 0xC826C0F6, 0x373FDDE3, 0xF3FCDB03, 0x003767A7,
    0xB9FEE523, 0x9130403A, 0x203D5848, 0x6FB458A2, 0x6C8805F1, 0x3653FB70, 0x441ACE89,
    0xEC89D8B8, 0x8B760104, 0xEA64F44E, 0x605C6252, 0x38852A06, 0xE6846295, 0x01880144,
    0x00000001, 0x1C080100, 0xB000F028, 0x69DAFF7F, 0x21B0C820, 0x10ED0C80, 0x90E203FA,
    0x944900E0, 0x83B82C38, 0x0BA96110, 0xAAB8FA13, 0x2540D0C0, 0x7596E284, 0x1AADA8EC,
    0x070B6042, 0xB3495C72, 0x4D822BA6, 0x89DCD5F6, 0x56430F30, 0x5B4A6B3C, 0xCD17B0C0,
    0x9552A972, 0x818D99A3, 0x34420A40, 0xEBA95BF1, 0xBA0FC083, 0x42329A68, 0x75CD56A1,
    0x58920824, 0xB4D51357, 0xE21AB89A, 0x1B0B69AA, 0x7735B55A, 0x13478251, 0x2BB258AC,
    0xE02A1DE9, 0xAA3DCD01, 0x5D600156, 0x880EEFE9, 0x1D610302, 0xE574BA4D, 0x1103C433,
    0x534A0B14, 0x6A82071C, 0x44CCD728, 0x9A668D10, 0x41CA1898, 0x3D956BE8, 0x2164C09A,
    0xE09904EC, 0x3D883319, 0x0034E477, 0x063F9440, 0x90C40E2A, 0xDD10A3AF, 0x49F8090E,
    0xC54229CA, 0x00000080, 0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x231087C3,
    0xB1489C4A, 0x188BC5A2, 0xC8DC6A33, 0x89C7A3B1, 0x00000101, 0x00000000

  EW_BITMAP_CLUT_EMPTY( CutterStatusIcons, Default )

  EW_BITMAPS_TABLE( CutterStatusIcons )
    EW_BITMAP( CutterStatusIcons, Default )
EW_END_OF_BITMAP_RES( CutterStatusIcons )

/* Table with links to derived variants of the bitmap resource : 'Cutter::StatusIcons' */
EW_RES_WITHOUT_VARIANTS( CutterStatusIcons )

/* This is a bitmap resource. */
EW_DEFINE_BITMAP_RES( CutterArrows )
  EW_BITMAP_FRAMES( CutterArrows, Default, EW_DRIVER_VARIANT_ALPHA8, 15, 25, 0 )
    EW_BITMAP_FRAME( 3, 10, 3, 5, 0x00000000, 0x00000000 ),
    EW_BITMAP_FRAME( 9, 10, 3, 6, 0x000000A8, 0x00000000 )

  EW_BITMAP_PIXEL( CutterArrows, Default )                      /* ratio 45.33 % */
    0x1C080100, 0x9C801048, 0x02130535, 0x9D4EC208, 0x81038510, 0x0FF7FBAC, 0x3888139E,
    0xA7FC5226, 0x710BE14D, 0x498E291B, 0xC85BA2D8, 0x8E12777F, 0x507276C0, 0x4ECE0FC2,
    0x01984A52, 0x045249D9, 0x7610460A, 0x82829612, 0xEC9D720F, 0x85A0A0E5, 0x658F2754,
    0x5F2A0848, 0x14457D48, 0xD2753852, 0x0E850618, 0x55E49D6C, 0x5F842134, 0xBC267F3D,
    0xA964EC60, 0x303B099C, 0x61401993, 0x09276B08, 0xEDA15441, 0x2E881E64, 0x03849DC0,
    0x90BA86D1, 0xF80220D9, 0xBB9FF7F1, 0x08018220, 0x488F27A9, 0xA2498162, 0x00080802,
    0x94080100, 0xC1A04020, 0x08550683, 0x61B0824B, 0xDAFF6D0A, 0x90686909, 0x117FC40A,
    0xB0045075, 0x71C7A305, 0xF1CA3632, 0x8B9BD188, 0xFE487D8D, 0xA36256D3, 0x40D09523,
    0x49E01450, 0x140D32E5, 0x4952A80F, 0x0645026C, 0x8AEA54B9, 0x31C2B140, 0x5016B495,
    0xE54C90D4, 0x62068A29, 0x70D614A9, 0xD29528C0, 0x800C203C, 0x12356549, 0xECA9130D,
    0x1EA1A276, 0x3468E195, 0xDCD2A35C, 0xF45F868D, 0xA3E77491, 0x247C1361, 0x886929E3,
    0x7B491DA6, 0xA11E2850, 0x95DF11E7, 0xC090008D, 0x0C2FC7FB, 0xBE458162, 0x101C799D,
    0x00000000

  EW_BITMAP_CLUT_EMPTY( CutterArrows, Default )

  EW_BITMAPS_TABLE( CutterArrows )
    EW_BITMAP( CutterArrows, Default )
EW_END_OF_BITMAP_RES( CutterArrows )

/* Table with links to derived variants of the bitmap resource : 'Cutter::Arrows' */
EW_RES_WITHOUT_VARIANTS( CutterArrows )

/* Initializer for the class 'Cutter::SpeedButton' */
void CutterSpeedButton__Init( CutterSpeedButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Background, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->RepetitionTimer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterSpeedButton );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0046 );
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->TouchHandler, _Const0047 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->TouchHandler, _Const0048 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->TouchHandler, _Const0049 );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->TouchHandler, _Const0004 );
  CoreView_OnSetLayout((CoreView)&_this->Background, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Background, _Const0046 );
  ViewsRectangle_OnSetColor( &_this->Background, CutterBackColor );
  CoreView_OnSetLayout((CoreView)&_this->Image, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Image, _Const0046 );
  ViewsImage_OnSetColor( &_this->Image, CutterTextColor );
  CoreTimer_OnSetPeriod( &_this->RepetitionTimer, 50 );
  CoreTimer_OnSetBegin( &_this->RepetitionTimer, 300 );
  CoreGroup__Add( _this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Background ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Image ), 0 );
  _this->TouchHandler.OnLeave = EwNewSlot( _this, CutterSpeedButton_leaveSlot );
  _this->TouchHandler.OnEnter = EwNewSlot( _this, CutterSpeedButton_enterSlot );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, CutterSpeedButton_pressReleaseSlot );
  _this->TouchHandler.OnPress = EwNewSlot( _this, CutterSpeedButton_pressReleaseSlot );
  ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &CutterArrows, ResourcesBitmap ));
  _this->RepetitionTimer.OnTrigger = EwNewSlot( _this, CutterSpeedButton_RepetitionSlot );
}

/* Re-Initializer for the class 'Cutter::SpeedButton' */
void CutterSpeedButton__ReInit( CutterSpeedButton _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
  ViewsRectangle__ReInit( &_this->Background );
  ViewsImage__ReInit( &_this->Image );
  CoreTimer__ReInit( &_this->RepetitionTimer );
}

/* Finalizer method for the class 'Cutter::SpeedButton' */
void CutterSpeedButton__Done( CutterSpeedButton _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterSpeedButton );

  /* Finalize all embedded objects */
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );
  ViewsRectangle__Done( &_this->Background );
  ViewsImage__Done( &_this->Image );
  CoreTimer__Done( &_this->RepetitionTimer );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Cutter::SpeedButton.OnSetOpacity()' */
void CutterSpeedButton_OnSetOpacity( CutterSpeedButton _this, XInt32 value )
{
  CoreGroup_OnSetOpacity((CoreGroup)_this, value );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, (XBool)( _this->Super1.Opacity 
  > 128 ));
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
void CutterSpeedButton_UpdateViewState( CutterSpeedButton _this, XSet aState )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aState );

  if ( _this->TouchHandler.Down && _this->TouchHandler.Inside )
    ViewsRectangle_OnSetColor( &_this->Background, CutterSelectColor );
  else
    ViewsRectangle_OnSetColor( &_this->Background, CutterBackColor );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterSpeedButton_enterSlot( CutterSpeedButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  CoreTimer_OnSetEnabled( &_this->RepetitionTimer, 1 );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterSpeedButton_pressReleaseSlot( CutterSpeedButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->TouchHandler.Down && _this->TouchHandler.Inside )
  {
    EwSignal( _this->OnChange, ((XObject)_this ));
  }
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterSpeedButton_leaveSlot( CutterSpeedButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  CoreTimer_OnSetEnabled( &_this->RepetitionTimer, 0 );
}

/* This is a slot method. */
void CutterSpeedButton_RepetitionSlot( CutterSpeedButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwSignal( _this->OnChange, ((XObject)_this ));
}

/* 'C' function for method : 'Cutter::SpeedButton.OnSetIconIndex()' */
void CutterSpeedButton_OnSetIconIndex( CutterSpeedButton _this, XInt32 value )
{
  _this->IconIndex = value;
  ViewsImage_OnSetFrameNumber( &_this->Image, value );
}

/* Variants derived from the class : 'Cutter::SpeedButton' */
EW_DEFINE_CLASS_VARIANTS( CutterSpeedButton )
EW_END_OF_CLASS_VARIANTS( CutterSpeedButton )

/* Virtual Method Table (VMT) for the class : 'Cutter::SpeedButton' */
EW_DEFINE_CLASS( CutterSpeedButton, CoreGroup, OnChange, OnChange, TouchHandler, 
                 TouchHandler, IconIndex, "Cutter::SpeedButton" )
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
  CutterSpeedButton_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CutterSpeedButton_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterSpeedButton )

/* This is a bitmap resource. */
EW_DEFINE_BITMAP_RES( CutterPaper )
  EW_BITMAP_FRAMES( CutterPaper, Default, EW_DRIVER_VARIANT_RGBA8888, 195, 270, 0 )
    EW_BITMAP_FRAME( 0, 0, 195, 270, 0x00000000, 0x00000000 )

  EW_BITMAP_PIXEL( CutterPaper, Default )                       /* ratio 18.81 % */
    0x1C09FF00, 0xBF205048, 0xF20EFF83, 0x6050C82B, 0x190D87BE, 0x4FF89BF6, 0x0588C59F,
    0x37FBEA2D, 0x7F461FB1, 0xC45E2B20, 0xC99F7038, 0x3A490884, 0x2392A884, 0x5AFB9345,
    0xD208DC8A, 0x1A90C7DF, 0xB944C265, 0x7CC3E5B0, 0xDBFE1532, 0xCD6732D9, 0xB673369B,
    0xE651D9DC, 0x8508A0CF, 0xB2A964FA, 0x399552A9, 0x7A3C469B, 0xA94EE495, 0x6CB69F2A,
    0xC5EB144A, 0x8E715BA3, 0x6535F954, 0x8524AA58, 0x34692596, 0x6BA4DA64, 0xFAD49EBD,
    0xA14B6816, 0xAD8A4960, 0xC62BB14A, 0xCEEF14AC, 0x303279D4, 0xC924C1C6, 0x10D9949E,
    0x70C15BB0, 0x7BCD5A31, 0x865F22F8, 0x6C47395C, 0x4CAFB3DB, 0x9E8B31C9, 0xF430DBB5,
    0x2F8DA5C6, 0x511CCE3F, 0x34B364EE, 0xFB568359, 0xC4DEED9A, 0x9546FFA6, 0xE6B2111D,
    0x221BA2C2, 0xF0D8A8F6, 0x31A5D8E8, 0x71EC25FB, 0x36DCE76E, 0x4867279B, 0xCAED29DA,
    0xAC47AD08, 0x6F35BD9B, 0xCF3F31C6, 0x78771E9E, 0x91A7F5F4, 0x19EBA6F7, 0xF3BC6EF8,
    0xCA9F7631, 0xDBF578FE, 0xD9641194, 0x277DC57B, 0x2D327C91, 0x0576E097, 0x751C17ED,
    0x5D780143, 0xD7390280, 0x41D28390, 0xCA7890B5, 0x879E3831, 0xDCC6FD25, 0xE42DED83,
    0x45FD8158, 0x1F7E59B4, 0x8B5EA46E, 0x5DE4F1E8, 0xB6A10D8A, 0xC5B88B20, 0xC6436278,
    0x849E16D5, 0x21685230, 0xA8CE2277, 0xC5FC8CA1, 0x1990D1E7, 0x7D2237D5, 0xD3191238,
    0x58F50C8E, 0x0E404BE3, 0x398CA034, 0x50A5B4E5, 0xE32984F8, 0x89853996, 0xFDEE98CF,
    0x5B62E663, 0x97E6B4EE, 0x65E53D08, 0x098A609A, 0x3D3942E7, 0x629AA775, 0x4C5B6776,
    0x2719D26F, 0x989A7344, 0x2686A066, 0x829E98BA, 0x9FA6B42E, 0x676A023F, 0x79C8FFA3,
    0xA2889ED1, 0x8545E789, 0x99E7CA6A, 0xA939967B, 0xD99E75A2, 0x7A9F9FE9, 0x1BA5942A,
    0xA7AA3A61, 0xEAE44279, 0x786E9099, 0xB25BA652, 0xB8422AAA, 0x502B19A6, 0x653846B3,
    0x9ABE6E9A, 0x5297A12B, 0x5DA2ABDA, 0xAF2A4B26, 0xD17AFAC6, 0x79AE32B2, 0xEEA9A46A,
    0xA1B06BA9, 0xA2A8C45E, 0x527B853B, 0x1B891BB3, 0x9EB4B153, 0x8F62EE64, 0xA4AEEBAA,
    0x2EB45EA6, 0xBBD6C2BA, 0xDEF3AE93, 0xF5BBE7DA, 0xBF2F6AFE, 0x2FCB9EED, 0x84C6F9BD,
    0xA6FAC0EF, 0x8BC2F02B, 0xAA705442, 0xB08C130F, 0x3B96C8C1, 0x9317C431, 0x01C32FFB,
    0xBEEECC6F, 0xD39BD30C, 0x7BAF1DC3, 0x7B09C870, 0x24BF93BC, 0xCA72AC83, 0x71EC9F0F,
    0x7CB2FCC9, 0xC328CD31, 0x40C532FB, 0xCBB37C8D, 0xB2DC6AEB, 0x4A8B1EC2, 0xCAD6BAF4,
    0xFCD1F3FB, 0xD0348CCA, 0x30EB7B35, 0x9A29C2BF, 0xDDAE4B64, 0x395A0FF8, 0x812C36C2,
    0xD0581A3C, 0xC4321C51, 0x7DBE9064, 0xF5D5646D, 0x84E516AA, 0x1BAD6F5E, 0x59536C7C,
    0x2DF79590, 0xE5DCB6E6, 0x7123A941, 0x7798D361, 0x8DBE4DD9, 0x5F6A5EB6, 0x7BAA12AD,
    0x40A2090B, 0x619D713B, 0x295780DD, 0xEF594136, 0x69DC392D, 0x52224923, 0x65A8536D,
    0x86478A9A, 0x0E2FDEE0, 0x76E77994, 0x41778413, 0x373E6A36, 0xDDC5E7E6, 0x0BAF40FA,
    0x7DECAD88, 0xE77B5D8F, 0x782E5BA9, 0xEE965B41, 0x0639DE9F, 0x88F237CF, 0x84393E7B,
    0x1E441384, 0x988767E8, 0xDA3EE7F8, 0x9B8E5A9A, 0x8C5F8ED3, 0xF97EBBCA, 0x7F6356E8,
    0x2F8F92E5, 0xCDEAFCA6, 0xF0D2BEDF, 0xBA2EC504, 0x7F9BA441, 0x0F86FA20, 0xF976A0A4,
    0xDADD0E43, 0xD992CBD0, 0xEF3BD356, 0xFF3F4F31, 0x42F65E33, 0xA9EEB7E7, 0x0BCD77EE,
    0x47C6EE1F, 0xCF3DF2BF, 0xDAD3F874, 0xC08820FA, 0x78F05DEB, 0x011FD3D1, 0x0C3E97A9,
    0xBB716DD1, 0x1EECDEE3, 0x49B8BDF8, 0x72477DF0, 0xE828A55F, 0xCEA20C92, 0x216BBE3E,
    0x37772AEB, 0x784FCA1A, 0x53CA0433, 0xA7A6B4D6, 0xF7221118, 0x97106248, 0x24A04478,
    0x844C1EF1, 0x189E53C8, 0x114A2145, 0xE26C4A89, 0x26288912, 0x8B314E2D, 0x18A2B457,
    0x2C45E553, 0x638CB142, 0x531A22A4, 0x2E30C455, 0x1C6E8D51, 0x38DA98A2, 0x319E2FC6,
    0x63846E8E, 0xC742211D, 0x8C11BE2A, 0x170A047B, 0x3D46B4CF, 0x878ED1FA, 0xD3226334,
    0x0E3F4808, 0x30FF9172, 0xA8FB1AE4, 0xD1E24945, 0xE2D48D89, 0x49990B23, 0x92F26E37,
    0x2064EC91, 0x51C77943, 0x96941252, 0x4727E31C, 0xB63F4A99, 0x846B9112, 0xE9572825,
    0xB23A514A, 0x8A3C6E8B, 0xA5B2432E, 0x28E4DA5D, 0xF2E5F14F, 0x613CB456, 0xC498482A,
    0xFED92614, 0x0490CCA7, 0xC4C82433, 0xF99A4C52, 0x3344A6BD, 0x97ACD483, 0xCCF9B250,
    0x9793425F, 0x31E6CAD8, 0x649369BF, 0x4C797392, 0x213708A1, 0x9E5ECD92, 0x7CEC9813,
    0xE9CC4867, 0x4FAA7ACD, 0x67A4E79D, 0x9DD24331, 0x9B250275, 0x93A71236, 0x044E59F6,
    0xF29FB365, 0x093749B4, 0x25C8A32A, 0xB91E9EF4, 0x5A0E62E6, 0xF4327F24, 0x27B4EE28,
    0x50F9FF3C, 0xA0731A7D, 0x309E9D12, 0x6751AA11, 0x05A4B3C6, 0xCF34CFFD, 0x01B03949,
    0x4BA72C71, 0x580D0E1E, 0xF079FF3D, 0xE0641884, 0x10281C84, 0x79D075F6, 0xE308C538,
    0x16405854, 0xE939F01E, 0x62E06999, 0xC1F143C8, 0xBCC4206F, 0x787293E1, 0xD0250BD4,
    0xA693B07E, 0xBCBA78D6, 0xA9B36A9A, 0x056A7C13, 0xAE230A75, 0x35A914FA, 0x80896004,
    0x5525228A, 0x522E8075, 0x88394F6A, 0x0EE61956, 0xDF9D5AAA, 0xC3EAED58, 0x836DFA98,
    0x581F6C3A, 0x95530861, 0x7385756E, 0xF94FEC0D, 0x0A86D62A, 0xE3CB70B0, 0xDAE4F81F,
    0x0F16C6C3, 0xEBF3F1AB, 0xD6FB0084, 0x83F55D06, 0x512BCA2A, 0xAFBFD849, 0x47B2B4BA,
    0x4685610A, 0x52175928, 0x3D4CA9D6, 0xDB0B6C2D, 0xB40DE959, 0x6D943BB5, 0xB84ADD58,
    0xAB56F8A6, 0x6FC8F4E9, 0xE1A6B23D, 0x105575A6, 0xBE536E31, 0x20A6D695, 0x45CA2817,
    0x4B9B7050, 0xC452E291, 0x905142B8, 0xDD8B9971, 0xBAF74141, 0x74E0CDE0, 0xE15D6B8D,
    0x035FB786, 0x10ACAEDD, 0x3C85DDD2, 0x1419BA25, 0x35537C0A, 0xB7CD75DF, 0xC8F5D4BC,
    0x569BF373, 0xBF779EEE, 0x5259F5F3, 0xFA604BCD, 0x08B9D79E, 0xFD79EED1, 0x1F00605B,
    0x2245BA58, 0x3BEA98AF, 0x782EF4AE, 0xEF690BBD, 0x2CEC0312, 0xBF0536FE, 0x83EFB5C6,
    0x11E13B89, 0x26BDE8CB, 0xBD71F066, 0x0F0861EB, 0xFE20BF98, 0x9C2D826F, 0x51698D41,
    0x22A13963, 0x4826A58E, 0x2378ED31, 0xAA71FC7D, 0x4E641C84, 0x3A237919, 0x7AC3723E,
    0x688DAF1C, 0x564E692C, 0xF316D1B2, 0x4CF65C64, 0x724917CA, 0x45BCB794, 0xC819631F,
    0x9572E65E, 0x69E54C9B, 0x5CCCB959, 0xFD9A32D6, 0xDB356428, 0x5E618F98, 0x6CC59173,
    0x798F3266, 0xA2566D89, 0x6768E590, 0xCD794325, 0x99F22653, 0x3DC50CF3, 0x5CCFB9C3,
    0x11A0B3FE, 0x564231FD, 0x566ECD6C, 0x4CBF8DF2, 0x78F3404A, 0xB39E94C8, 0x677CE39C,
    0xD22C695D, 0x9AF3E65C, 0x25C2FB69, 0x1DAE8D62, 0x5CD68EC2, 0x9EB6D53B, 0xADAA26DA,
    0xDB27AA6D, 0x36D2A5EA, 0x6DB55ED6, 0xD45A1D69, 0xD4A84297, 0xACAC4D77, 0xC6EAAB57,
    0xBBB5EF01, 0x6759357D, 0x175DB8ED, 0x5BE78149, 0xE7455C36, 0xF5AB5E55, 0xE1A6C078,
    0x55CD6759, 0xDAFAD35D, 0x02F5898A, 0xAF56EB19, 0x8A3CCFEE, 0x4D60EBAD, 0x35EDC01D,
    0x2D5DB110, 0xBA8367E1, 0x16B35C40, 0x57AEC3DD, 0x3F5B2DB6, 0xDCDB46D0, 0xB577C5B7,
    0x76EAEDE7, 0xF1D7B61F, 0x0B4DF676, 0x177B56BE, 0x7EF2E05B, 0x7219B431, 0x2FDF7F59,
    0xDBCEE7DF, 0x6B6E17C0, 0x53D5EB83, 0xC2092713, 0x91D7A341, 0xC8731C7F, 0x4B4F793E,
    0xA5AC72AE, 0xDF26E55C, 0xE255CB4A, 0xBC7F978F, 0x79A6B5E6, 0x9ADE6280, 0xE6DC8B99,
    0xCFB96737, 0xA17402B7, 0x27E78E8B, 0x5BD0F9C7, 0x5395F453, 0x9B63E906, 0xB265BEDF,
    0xF643774C, 0xF42463CD, 0x0EE2859A, 0x1FE3BC78, 0x3686D8D5, 0xB2762EC0, 0x63EC3D8F,
    0xAE360697, 0x686A0FF1, 0x8F661B23, 0xAEE1DBBB, 0x3DA3B676, 0x469F77EE, 0x0FF1B036,
    0xEEFDF3BD, 0xDD7B2F80, 0xBBF65EF1, 0x7BEFBDCF, 0x05D9FC27, 0xBFB6F6BF, 0xA78EF15D,
    0xBF03E37B, 0x1DFBB1F8, 0xBC2F7FF1, 0xF93ED3E6, 0xF2FDC7CD, 0xDC7CA797, 0xD1787F07,
    0xA2F07E1F, 0x4BE27BCF, 0xA3BBF9EF, 0x0765F5BE, 0xCF53E27D, 0x7EC3BB78, 0x7CEFA7EC,
    0x78DF47E5, 0xF73EEFD6, 0xEABDDFB6, 0xDBFA1EF9, 0xC2F5E3FF, 0x8BE1FB2F, 0xB3CE7C9F,
    0x7F72F7DD, 0x2F21F23C, 0xFF37CCFB, 0x7EA7BAF8, 0x790F99E2, 0xF4DE3FD2, 0xE8BECFD2,
    0xD17D0F2F, 0xDBFC7F4F, 0xCFF1BD9F, 0x43E079EF, 0xFFBDF5DE, 0x1FC3FB7E, 0x036FC87C,
    0x47F8DF1C, 0x0F61EC3D, 0xCF59A6F4, 0x1A6A2694, 0x730C6143, 0x6056255F, 0xE40DF147,
    0xF0FF0E30, 0x0B80BA0A, 0xCB0C30FF, 0x40FF0CA0, 0x8140CC0C, 0xC2814816, 0x10C90CB0,
    0x81581381, 0x14820817, 0xB8210C38, 0x81981D81, 0x12829822, 0x881281D8, 0x82882181,
    0xCA0C2828, 0x582F8270, 0x81F82383, 0x3082982B, 0x181C8258, 0x82A83681, 0x2E83483C,
    0xC81E8438, 0x83B81F82, 0x42832844, 0xD82183A8, 0x84C84582, 0x2483D849, 0x38288448,
    0x8260FF85, 0x5282A850, 0x183F8478, 0x85983383, 0x5C838855, 0xF8258548, 0x85A84885,
    0x6783D81A, 0x485B8318, 0x84182386, 0x6D84D860, 0x681A8718, 0x87285D86, 0x6784B837,
    0x78568588, 0x85885686, 0x4687286B, 0xA82986E8, 0x81E87484, 0x7C887879, 0x986F8778,
    0x83F87A86, 0x8184E839, 0x488F8758, 0x89284E84, 0x7685788C, 0x084E8848, 0x87C89988,
    0x8E840863, 0x50BC8598, 0x0FF0C40C, 0xBD0BF0B4, 0x50F40FF0, 0x4C30F50F, 0x0D6315E7,
    0x55716336, 0x6E052D60, 0xE50E010D, 0x10FF0E40, 0x75485F0C, 0x4B40F750, 0x774D1A07,
    0x5738C174, 0xC53638C3, 0x76F02148, 0x7288C913, 0x4A73E739, 0x58CF8C67, 0x8D17448D,
    0x728C78D2, 0xB35B8D63, 0x8DA73B8C, 0xD372D8CD, 0xF8E38D58, 0x8E074F8D, 0xF20EF708,
    0xA0FF0F20, 0x0C20C40B, 0xB87FC7FA, 0x06E27178, 0x6F44161E, 0xFF8BB8B9, 0x80C90C00,
    0x2305690C, 0x6C71B59C, 0xE53971D5, 0x71059170, 0xBE90370A, 0xE58F70D6, 0x6A85666D,
    0x988FF252, 0x890A3E45, 0x55E5B45A, 0xEB27D0F7, 0x40FC4546, 0x70C42659, 0x2290D420,
    0x171A70F4, 0x5B391571, 0x073DA428, 0x16DF5A47, 0x8F73BA72, 0x365A1920, 0xB8D15324,
    0x0EB1026D, 0xED0EE0EE, 0xD8F18EF8, 0x57892E7F, 0x1571F586, 0xA10C8F87, 0x8FC8BC8B,
    0x193CA8FE, 0x49025547, 0x9226B76E, 0x259076C4, 0x994E9099, 0x5906D292, 0x566E990F,
    0xB9137125, 0x6C16EB57, 0xF924C569, 0x60A91170, 0x0FF0B30B, 0xCC0CE0C6, 0xE11E5590,
    0x0D40D50C, 0xB20A50FF, 0x90FF0AF0, 0x0B20B50A, 0xED0EA0FF, 0xC59E0EC0, 0x92190B71,
    0xF0565756, 0x797D9366, 0x9566C992, 0xF9959906, 0xE93F5D36, 0x5845BA95, 0xB5700210,
    0x811192C8, 0x8EE93A93, 0x3E7FB8F0, 0x456995D9, 0x47349338, 0xB6495667, 0x20DD13B4,
    0x0FF0E10E, 0xC60C80BF, 0xF2FE6950, 0x4B64794A, 0x8C4A76A2, 0x24904A54, 0x9B447F9B,
    0x9E9B849C, 0xC68E9A04, 0x6879B04A, 0xBF4A8464, 0x19B59B39, 0x9BA9C29A, 0x909BC4AA,
    0xA9C49AE4, 0x7F712727, 0x9F08E7F9, 0xE0FF09B0, 0x0A90AC09, 0xF40F48AC, 0xD0F30FF0,
    0x0AC0FF9D, 0xFF0B50B8, 0xC0DE0D80, 0x0AB0FF0D, 0xFF0B40B6, 0x00C10B80, 0x4B42F50C,
    0xB79C89A2, 0x24B59BD4, 0x9C59F19F, 0xB29B6267, 0x99CA4A64, 0x9FB4909B, 0x8A9FF9F3,
    0x69F999F4, 0x4A09CD9C, 0xB19F7A00, 0x1353A034, 0x0EA0E713, 0xBB0FF0EA, 0xB0F193C0,
    0xA0A2F58E, 0x5A9C0A08, 0xF98D8B74, 0x8FB9460F, 0xD39128FD, 0x59269016, 0x95090595,
    0x10983513, 0x2A269859, 0x6EF42954, 0x04984708, 0x399C98C1, 0x6FF93194, 0xC30B920C,
    0xE0B38F00, 0x0FF0BC0B, 0xAA0AD0A0, 0x10DC0FF0, 0x96C0E00E, 0xD40CE0FF, 0x79D50D30,
    0x0D00FF9D, 0xC40D50D7, 0x89446FD3, 0xA2A989A4, 0x2D91195A, 0x3A286F5A, 0x4115B83D,
    0x3696072F, 0x23C7904A, 0x94CA2299, 0x3B99620C, 0xD8F29999, 0x3A243FA1, 0x1197FA63,
    0x7948A1F1, 0xA246DEA6, 0x7D95F954, 0xF5A39415, 0xA7A58BA5, 0xBC92A6FC, 0x86C79943,
    0x56B9878D, 0xF9A38A5B, 0xF2620F61, 0x8EDA4209, 0xFF0C10C2, 0x10B40A70, 0x0B60FF0B,
    0x180BE0C1, 0xE0CF0C81, 0x0AEA400C, 0xCB0FF0AB, 0x30D10D20, 0xA64930A5, 0x0E957932,
    0x498BA589, 0x8F6AA870, 0x52923AAA, 0x3909A689, 0x99CA1C75, 0x84934551, 0x8A6A10CA,
    0x8F393D99, 0xE3A28A56, 0x4132A806, 0x55CA21A7, 0x7F38794B, 0x76EC98FA, 0x6CC981A2,
    0xC494D201, 0xD90636CA, 0xA3592D6C, 0x17A37A54, 0xF0DA1867, 0x0FF0DE0D, 0x9E0A2092,
    0x20B79E70, 0x0B00A39E, 0xA10FF0AE, 0xF0AC0AF0, 0x0EB0E80F, 0xC0A970EB, 0x0AEE0BF0,
    0x0B70FFA1, 0xABAF40C2, 0x6AA6AE50, 0xACE99098, 0x25AD16AB, 0xD988AD3A, 0x6DE3E9A5,
    0x2FAC06AC, 0x5ACDAFE9, 0xAC93E9A6, 0x39ABCAD0, 0xEABFA6B9, 0x7145A2A6, 0xDC31A294,
    0xE14D2692, 0x0E20E30D, 0xC70C89A8, 0x128726C0, 0x32926D67, 0x1B68F2DA, 0xB29B2CFB,
    0x25B5CC11, 0x3AB2731D, 0xD657B293, 0x2D12CA2A, 0xC16972CC, 0x89EF2B52, 0xB2A627B3,
    0x192A0B37, 0x0B2B64AB, 0x2326752B, 0xA70C60BD, 0xBAF4AF88, 0x0ED0EB26, 0xB19710ED,
    0xE0D99740, 0xA120DD0D, 0xFF0C30C5, 0xEA4E09F0, 0x0A80AB09, 0xDFADD0FF, 0xA2D75FBA,
    0x5D92D428, 0xA6B41B3A, 0x4B3358E5, 0xB6A2DB29, 0x18B2E2DD, 0x4B3129E6, 0xB6EB282C,
    0xD9B2CB40, 0x565CB432, 0xB47264B4, 0x3DB652C1, 0x928AB42B, 0xAF10EC0E, 0xF40F2ABE,
    0xE2BB0F30, 0xB68B3E28, 0x942C0B72, 0x0B7DB7F2, 0x114B1CB7, 0xA9B20B1E, 0xFB82B239,
    0xB7BB3267, 0x7AB6FB7A, 0xB8B4B67B, 0x3FFB3B44, 0xEEBA42A8, 0xFB74B795, 0xB2FBA5B8,
    0x9F344B25, 0xD678BAAB, 0xBAEB3061, 0xA3B24B2D, 0x268DBB8B, 0x9DE0F123, 0xF361D28F,
    0x24919DA0, 0xAEF14726, 0xECA15AF1, 0x6B648DB8, 0xB8DBB1BB, 0x75BD064B, 0x1B772E0B,
    0xBB7BACBA, 0x91B46B84, 0x332EBD2B, 0xBA2BBCBB, 0x94BD9BAD, 0x0BE1BBAB, 0xB81653BD,
    0xD492ABCE, 0xCB88B862, 0xB8CB8A93, 0xDB2D2B7E, 0x9AFDA71B, 0x8FA945A8, 0x01AC8A75,
    0x43B637E7, 0x6C6B00B0, 0xCCAD2194, 0xCA2FA62A, 0x926C0139, 0xF6956AB6, 0xDA66A48B,
    0x72F3AA6D, 0xF4AAF91F, 0x85A5B068, 0xB0A5B9B0, 0x02AD5914, 0x4A61AB5B, 0x928BF443,
    0xA5A66539, 0xCABD24C6, 0xC1899A93, 0x14A6F102, 0x8A87C1F2, 0x949AC611, 0xC2B10594,
    0xDC28C026, 0xAB3C1499, 0x03B03534, 0x2C05C1EC, 0xA2BA3198, 0xD86CFA33, 0xAA72AA7A,
    0x59441AA8, 0x0C3D6A32, 0xCC2A960B, 0xC34AAC6D, 0x30543A1D, 0x953FABBA, 0xBFEA88C0,
    0x325A5B0E, 0x5118709C, 0x99BA6CC2, 0xCFAC1716, 0xFC2C194A, 0xBF8C2EAF, 0xFB94AA21,
    0xB37FAB4B, 0xC4D58DC3, 0x5098A6D3, 0x1AB1C43C, 0xC08C7295, 0x4BA81C1D, 0xB718A7BC,
    0xA79C39C6, 0x375BCC6E, 0x5C80C77C, 0x53BC52C3, 0x86C78C4F, 0x1AC3C66A, 0xB0F917C8,
    0x24546C7E, 0x7ABEB13C, 0xA7CB16C2, 0xFD49A440, 0x447E9C79, 0xB219A69A, 0x0966A9AB,
    0x1A04C9CA, 0x9F69BE9C, 0x189F49BB, 0x9CA34A3A, 0xCAAA059C, 0x4CCA89AF, 0x29F5CA66,
    0xCB2498A0, 0xA59FECB4, 0xCA0C9FAC, 0xCB5CBDCA, 0xF8CB7A0B, 0x6CB1CB99, 0xA07CC4CB,
    0xBBCC2CBE, 0x39B7A06C, 0xCCACBCCC, 0xFCCD0A19, 0xECA7CB09, 0xCADCCCCC, 0xD1CA4CC0,
    0xB14F695C, 0xA11A0F13, 0xCA8F0A13, 0x1CC5A17B, 0x5B1CC7CC, 0x76C2FC35, 0xDA78AB2A,
    0xC70C35C6, 0x85C89958, 0xEC3DC51C, 0xA59C3FC4, 0x2EC56985, 0x1916C44A, 0x993C9099,
    0x8E6EAA7E, 0x8A7D71EC, 0xCF4CEE90, 0x53D05A82, 0xBC61AD79, 0x599C0BB0, 0x4857F933,
    0x5C5EA2BC, 0xB17AB8B1, 0x12AD9A70, 0xACEA105C, 0xD06D00C6, 0xFFD03BFD, 0x1C6CD0AB,
    0xCF5C86D2, 0x0CCF2C71, 0x8D0EA55D, 0xC22AA9C5, 0x41CF8D13, 0x3CFD97EC, 0xD01C53D2,
    0x7AC7B95B, 0xDCF7A57C, 0xD0D95CAA, 0x40D0F914, 0xBD1FC7D9, 0xC94995D0, 0x17A6DC26,
    0xCD19C98D, 0xC67C20CF, 0x31C69947, 0xAC33C91C, 0xD18CEFC3, 0x02C54C3E, 0x7D09D27D,
    0xA85D20D3, 0xE5D3AD5C, 0x5CEBCFF6, 0xD4BCF3D5, 0x3DA29C75, 0x3D40C73D, 0xD5AD3C56,
    0x8DCFAD39, 0xBD11D57C, 0x59DD3159, 0x68C96D15, 0x26265BDD, 0x61F60562, 0x09569807,
    0x3A2080B8, 0x1145E3B2, 0xAB60862A, 0xED7F5DFB, 0x62462D62, 0x905F3267, 0xCBA6D8FB,
    0xD8B6325E, 0x94602D94, 0x1C368B2D, 0x60560A13, 0x0E114D89, 0x65F1D8D6, 0x606DA4D9,
    0x30D7EDA7, 0x861AD816, 0xDA1DA762, 0x1C62C5FA, 0x262F3166, 0x626D9161, 0xB05F3D93,
    0xCD8CB3FD, 0x806D9A5F, 0x15DA9DB7, 0x25EA9936, 0xD97619B9, 0x9D62661B, 0xAD94DC02,
    0xD94DB6DA, 0xBB5FCDA3, 0xA147DD1D, 0xABEB5197, 0x478AF8AD, 0x5D92DCF1, 0x6865C6BA,
    0xA0694759, 0xF0DE2326, 0x318B9BC9, 0x9EB47D9E, 0xC69C67E6, 0x75A9F4DE, 0x74698DE0,
    0x768C6606, 0xBB568B67, 0x019ACDF3, 0x2DF79C08, 0x69968164, 0x00DE2683, 0x9693DF98,
    0x2FEDE3DF, 0xF8DEE68A, 0xADF569BD, 0x69F68FDE, 0x0266BE03, 0x9E0DE008, 0xE16DED68,
    0x85E02DEF, 0xDCB3DFB6, 0xDFF694DF, 0x1069ADF9, 0xEE04E1BE, 0xE1CDF046, 0x0BE0CDF2,
    0x134ADF6E, 0xA11AEF13, 0xECBBFABE, 0x764A32B8, 0xE09697E0, 0x0DC655A5, 0xEC68A73B,
    0x924BFA8F, 0x35ACBC06, 0x7B01755C, 0xD6DD67C0, 0x4AE45B6C, 0xCC0E568E, 0xD42AB0C8,
    0x0092BD66, 0x4A1DE42C, 0xE57E53E4, 0x49E50D39, 0xDBFCE58E, 0xE3BE4D39, 0x5FD2EE4F,
    0x2E5DD5ED, 0xABE75FB1, 0x554507FC, 0x1AC24FCE, 0xD50D22E6, 0xD1C30A20, 0x6C49C5B8,
    0x94FD6253, 0x6BCF1C63, 0x9D08D56D, 0xE63A5ED6, 0xE8C79C55, 0x5D44C97C, 0xD26C5CD6,
    0x38D6FE84, 0x8E62A5AD, 0xC88D5DB1, 0x8BC1CE7B, 0x7E8EE60C, 0xC5ACFEE7, 0x0DE87D54,
    0x8D4AD791, 0x6A4CF0E4, 0xEA12DE82, 0x1C38E3FC, 0xE93E81B1, 0x5DE56960, 0xAD2BC1B3,
    0xE4BC62AD, 0x5F3CDEAB, 0xB5E7C4CE, 0xAB2D32E6, 0x72902E5F, 0x6C1A6F8D, 0xD1BEADE4,
    0x07E7ED10, 0x3371AB2B, 0xEBBC7CEB, 0xCAD24D46, 0x7938104A, 0xE6993CE6, 0x79E6CE43,
    0xFDEBDF1E, 0xBE4E247F, 0xE79A7DE5, 0x565331DD, 0xDF7BBDED, 0x11639EDE, 0xCE29758E,
    0xE20DF9E2, 0x59EE1E2A, 0xC692EE36, 0x696E14DF, 0xB4EF0E18, 0x1E25E27C, 0xEECE06DE,
    0x21EEE6A1, 0x5EFBE01E, 0xE1AEF3E1, 0xFDE13EF2, 0xDEE8EF2E, 0xE35E0F69, 0x38BD1EF8,
    0x7E1FEDFE, 0xF07EE0B4, 0x26F01ED6, 0xBDF1F00E, 0xF10EDDF0, 0xEEDB4F0C, 0x4CE00EBA,
    0xE330F3A1, 0x3618BDC3, 0x4F0BBDBE, 0x6266FA80, 0x0CD83D9C, 0xED87A758, 0xDB2DD311,
    0x9BE2DDCA, 0xFDC8DACD, 0xB735FDD9, 0x315C9DAD, 0x6DCCF1AF, 0x5CBF36F2, 0xAEDC8F38,
    0xD61DDBED, 0x2FCD7DF3, 0xD1DA7F46, 0x9232DC5D, 0xDDFDBDF3, 0xFCF49F32, 0x1629DB95,
    0xD988B3F4, 0xA7F27DB3, 0xF236D9CD, 0xF56D88F3, 0x3BF5862B, 0x4F28F4DF, 0xDCE605F3,
    0x61F668B1, 0x0DAFDC9F, 0xF6CD8AF6, 0x14DD63CB, 0x58B0DD9B, 0xDA2F5C60, 0x0AEB5DDE,
    0xDD1CE8DC, 0xE74E86E3, 0x5AC4AC40, 0xAEB8E9FE, 0xC74E88D2, 0x68E80E8A, 0xCBF5EC85,
    0xD30EC1F7, 0x76F81935, 0x6D58F83E, 0xE8CD70CF, 0x79A5CE6F, 0xFE91AABE, 0xE95C45EB,
    0x00D75F94, 0x7C93D779, 0xC60D4999, 0xF5D43D2C, 0x3F8EFA08, 0xD53D1D10, 0x17D25D45,
    0x6C83E89C, 0xF88FB3F8, 0x09F9DFB5, 0x5FA1E7DB, 0xF87CEDE8, 0x29F95E9E, 0x8FB7FBAD,
    0xD59C87E4, 0xA1CF9E8B, 0x6F99F7BE, 0xF97552C4, 0xA8E9C0FF, 0x4EC1EAEF, 0x2694ED38,
    0x044C2516, 0x74DBCEA1, 0x4F64E34E, 0x244E751E, 0x250B50C5, 0x51F6D651, 0x23521993,
    0xF4CC5234, 0x4E7FE550, 0xE95022A8, 0xF5064F9F, 0xFD95D350, 0xE451950F, 0x150C527F,
    0xFDF4F6FE, 0xDFCFF100, 0x0CFD7FC0, 0x5097D3FE, 0xFE81C120, 0x17DC3A16, 0x8A3F1FF1,
    0xFC362D0F, 0x45E34F97, 0xF60D0486, 0x1F7117FB, 0x13188747, 0x10782BEA, 0x088FC623,
    0x08ECAE15, 0x7E452251, 0xCC6A2D24, 0xCD231367, 0x96611290, 0x25327824, 0x5D439FC8,
    0x518A2CA2, 0x04A4D1A4, 0xC824EA71, 0x87536793, 0x151AA94F, 0xCB68557A, 0x12B32DA2,
    0xDFF617FC, 0xFEEE773A, 0xEC2622E9, 0xF67B3D5F, 0x6A2D86C4, 0x96C5A635, 0x7C6EE34B,
    0xDEDF14FA, 0x65F17B7F, 0xE7381BC4, 0x7380C23D, 0x4DDA5D3E, 0xBF7FB85C, 0x47FB1990,
    0xC351DEF8, 0x15E70B91, 0x6572595C, 0xF6FD39CC, 0xF359DBE5, 0x3FA2CCE5, 0x19BA7E8B,
    0xA5D56675, 0xED3E7E37, 0xC3E9307A, 0x96876199, 0xEAF2796D, 0xE777B3DA, 0xF39BDD8E,
    0xF5C2D6C6, 0xDC3E0DF2, 0xC5E672B7, 0x9E66D39B, 0xE469BA1A, 0x9F138DB8, 0xF3FADC0E,
    0x3B85D7DF, 0xB937FEA7, 0xABB6F068, 0x3BADE5FF, 0xC2ECEB3A, 0xEF2EE7C7, 0xE5FAFCFE,
    0xE777A5E9, 0xFE347F4E, 0xA9D4E7A5, 0xB49767FB, 0x1E4789FE, 0x891A7A79, 0x7B98F6BC,
    0x1EB7A72F, 0x8AAA4B14, 0x29F08420, 0xA7041F0A, 0x01FE721C, 0x8C641926, 0x83AB70BA,
    0xA149342E, 0x8A5ADF0B, 0xB2C513A7, 0x96A34570, 0x8C329172, 0x44CA0C48, 0xD09C5317,
    0xE45716C5, 0xBF174609, 0x87468872, 0xA9718A84, 0xF1DC6296, 0x426B0944, 0xBD18A372,
    0x114571B2, 0xB2921241, 0x71CA6722, 0xAC2F24C5, 0x8118A24B, 0x9444B13C, 0x45491CAE,
    0xB1749D1D, 0x7C7F2F47, 0xC50A2C09, 0x1041118C, 0xCD3074CF, 0x12D4CC8A, 0x1CA115CD,
    0x8E944D93, 0x3EC6B37C, 0x2CCB2421, 0xCB62D4B4, 0xDCEB26AD, 0xE31F20F3, 0x38C9707C,
    0xC330A4B7, 0x10FC3B0D, 0x0CA70850, 0x7138D053, 0x38CE9264, 0xCBE91CBB, 0x740C8336,
    0x044F28C8, 0x953D5275, 0x13D1E8AC, 0xC892ACC1, 0xF4D2251B, 0x94214A44, 0x0F51D174,
    0x854A55B2, 0x4E0B7D72, 0x52CA2732, 0x74B957D7, 0x91614D74, 0x63421372, 0xD9332D6F,
    0x348D770B, 0x3D676756, 0x075553B5, 0x11D71565, 0x57B18D45, 0x33E45D72, 0x5D6B4E4E,
    0xCF42ADF5, 0x5751301A, 0x54913D19, 0x37342948, 0xB42F7A4F, 0x2D0F4394, 0x535BB10A,
    0x55B0BDC1, 0x6A54635F, 0xF60980DB, 0x432C53F3, 0x354E37D5, 0x5EF86D4B, 0xF805A92B,
    0xB4EF1E5C, 0x8B6C4296, 0x8CE074F5, 0x5AB8759F, 0x37DD9588, 0xB6453955, 0x4D70D838,
    0x6D62579E, 0xE5B92629, 0x2BB42976, 0xAE65945A, 0x119DCF09, 0x19E1B90E, 0xAC90A62B,
    0xAB55E077, 0x760EC9AD, 0x1F9A6131, 0x896A2A5C, 0xEA4A1284, 0xCCFA9691, 0x19BCA36A,
    0x90C69C47, 0x89B2A639, 0x6AABFB41, 0xFACEB489, 0xDC13B56C, 0xD7ABECC8, 0xAD4E5B26,
    0x6E0C4285, 0x1AD2F9AC, 0x96EDBC6D, 0xE5B729E8, 0xB7ED2896, 0x6EACA707, 0x2BFEEFC2,
    0x0705B86F, 0xFE9CED9C, 0xBE713BD6, 0x215AA247, 0x7C6247C5, 0x5F32856E, 0x3BC82F9C,
    0xBEF0FC97, 0xF27CB6DB, 0x7C2F2FB8, 0x1739D423, 0x5BBDF31D, 0xC273ECAF, 0x6E939759,
    0x3DA74DD5, 0xF4E5C6F2, 0x73D874BB, 0xD9215A9F, 0xF29E04E5, 0xFDDF5FCD, 0x9711E1B3,
    0xD874A793, 0x7FE90759, 0x1E67A1E4, 0x1DD26B64, 0xD273DB75, 0x2A9E525A, 0x8AFBC947,
    0x25B4933E, 0x2C76BDAE, 0x5524A16C, 0x9949215E, 0xDEA6BF17, 0xE7B8E7C3, 0x3F8AF9F1,
    0x67F29CF5, 0xF1C0FDFC, 0x62F5DFAF, 0xF200BEE7, 0xAC057C84, 0xC804F6C8, 0xCFF9E43E,
    0xDFC97F75, 0xC01816FC, 0x82CFDA03, 0x021F5994, 0x09C1526B, 0x1C2844D6, 0x08D96064,
    0x69F84178, 0xBBF48070, 0x980101A0, 0xE42A04C2, 0x606BFC29, 0x42C84B00, 0x850E91F9,
    0xFE5B8C1D, 0xD0C30814, 0x388670E1, 0x710F2094, 0x8DE13DF8, 0x0C208710, 0x785707E0,
    0x7119CF44, 0xC9AC486C, 0xA877A2F3, 0x886F55E9, 0x09A083F7, 0x72C01877, 0x27635171,
    0x46A617EA, 0x1175A0F6, 0xFB06456D, 0x2240B322, 0xEC5985AB, 0x587C6762, 0x3D88BACC,
    0x65C85185, 0xB118C32C, 0x7C2CB63C, 0x4F623179, 0x36CDD602, 0xD571AC35, 0xBB3E630C,
    0x64211F16, 0xEA423C2F, 0x238B388E, 0xC74636D3, 0x66D1B5A7, 0x9190830C, 0x3B33F90A,
    0xF592505D, 0xC926240A, 0x992A4848, 0xA2EF41F1, 0xA93EBCA2, 0x31B995C6, 0xE4E2F92C,
    0x2FF8C28A, 0x8CA5863D, 0x1C1A0AD2, 0x0A4AB661, 0x3A685231, 0x4AB7D903, 0xF25E4026,
    0x12EA666A, 0x991ECA98, 0xF2957149, 0x66249D8F, 0x4A197B1A, 0x9369FE64, 0xCBE37CC1,
    0x46CCF9A2, 0x838EB1AD, 0x70AA1684, 0x2163CC29, 0x33298F72, 0x39AB2526, 0x132E6C46,
    0x2394CD6A, 0x2159CD34, 0x5D283E45, 0x45A5AB22, 0x78CA89DC, 0xDD9E25B5, 0x612BA56C,
    0x62744899, 0xCAFE4348, 0x72C535D4, 0x4B4D07CB, 0x258CDB97, 0x4959BD28, 0x9C73263B,
    0x24582CD9, 0x80CE99A5, 0xDE9C2B72, 0x472428C2, 0x7CB7CF92, 0x8C7E422C, 0xF9DD2B28,
    0x53B68616, 0xE5DCEF90, 0x2CD9ED46, 0x9D93828A, 0x36A87162, 0x9320F9E7, 0x2298F2BE,
    0x5745DA1B, 0x3A4D162A, 0xF47DA533, 0x72DF4CA4, 0x33E586CF, 0x8B2AEEA7, 0x54A954F4,
    0x9E92F8B1, 0x835B89DE, 0xC49CC3F6, 0x906BE62C, 0xE3028CD9, 0xDCF49F93, 0x59F03D06,
    0x33F47850, 0x47E10556, 0x8FBD5AAD, 0xD6BACC7E, 0xADC7B2B4, 0x562B39E6, 0xF9D65AE3,
    0x76AD87A4, 0xB15DEBA5, 0xF4CF57FA, 0x6582AD15, 0xAADA76D0, 0x476ABD9F, 0x2C058A3C,
    0x579B1F5F, 0xB1A6E6B2, 0x646B8558, 0xBF371B1D, 0x9E3BF5D6, 0xB07D8F55, 0xF8891FC3,
    0x52024007, 0xB40E814B, 0xB68AC594, 0x29ED94B0, 0xD0698B0E, 0xA152D14B, 0x482524B6,
    0x34161A87, 0x1DA25372, 0x1B50636D, 0x769D4B9A, 0x033CAAEC, 0xCA3F456E, 0x08A2512E,
    0xCB7DB39E, 0x2BF9CF40, 0x3296EC90, 0x466E254E, 0x565BEA63, 0x37B951B2, 0x45764B85,
    0x2282510A, 0x1536BBD7, 0x4B9B4BAE, 0xD552E7D4, 0xEEE549A7, 0x52FBAB34, 0xB996E696,
    0xA227E227, 0xAADD09FC, 0x2795D4D2, 0x2151EF85, 0x1A84CB34, 0x34D6BEB5, 0xCBC54E30,
    0x780AEDCC, 0x675D13A6, 0xCE4C157A, 0xA7971A43, 0x4FEE8CF5, 0xE1CE297F, 0x24A1F852,
    0x01861165, 0x02E9DE4C, 0xC5EBBE72, 0x4B75822D, 0xB836F65F, 0x2E4E26BC, 0xE139817A,
    0xC4983290, 0x8C639DC7, 0x655CB947, 0xEDB798B2, 0x77472F8D, 0x8711525A, 0xC324A985,
    0x194F87AE, 0xD7EE54D5, 0x26FA2FBD, 0xE21C8772, 0xC085BE82, 0x9269B273, 0x1BD30C6B,
    0xFDCAB85F, 0x67503085, 0x6B255E1C, 0xFE44CBB7, 0x7C757BF2, 0xD3EAF060, 0x710272C7,
    0x5D2B6258, 0x9A8BA31C, 0x9BADBCF8, 0x0B3CDC51, 0x6BBA99A3, 0x3B45A85E, 0xBAE765BC,
    0x3CD0C4B8, 0xFCDB8EEF, 0xB9C6F1D4, 0xB29E3932, 0x958C8B9C, 0xC694B737, 0x3F7264FB,
    0xAB64F515, 0xF628D83E, 0x0FA7EA2A, 0x709699F2, 0x20DF2085, 0x15FC38C7, 0xC4B750B6,
    0x0906A270, 0xEA529E4F, 0xD5ABA483, 0x5F351EB5, 0x7B120D68, 0x62D76A71, 0x65AFC8AB,
    0xDB5F91BD, 0x26BF9E1A, 0xED79AE35, 0x1B2B5F5F, 0x761135D8, 0x6CED6645, 0xDA948F63,
    0xB14BFEB9, 0x65EB7BFD, 0xDDD92483, 0x60E3B286, 0x4757946D, 0xD13986BB, 0x7CE189F6,
    0x9AEB6F6D, 0xEDCBB5DD, 0x6C185FAF, 0xDD1B0F67, 0xBA5BFEEF, 0xA5EFA6E3, 0xF3DC1B34,
    0xA5BD7686, 0xFB7C6C4D, 0xF92DDC6E, 0xA6B5B8F6, 0x9AFF84EF, 0xB70704B5, 0x6D676FAB,
    0xDC7BDB70, 0x3B0768EA, 0x7A8F37A4, 0xD8E0E3D0, 0xFD8438FE, 0xA222C18D, 0xA3E03444,
    0x2E4B584C, 0x35786F40, 0x1E0BA77D, 0x7283924A, 0x25108A25, 0xCAF99F28, 0x9BF2A285,
    0x3638FCAF, 0x9BCCB927, 0x53A273D8, 0xAF457DA6, 0xAE8309F9, 0xF0BB9A1E, 0xFA5F4335,
    0xF49E97D1, 0xB29D0BA6, 0xCEBA6755, 0xAC1C2E9D, 0x3FE97CF3, 0xAEF1FA9F, 0x87AB75AE,
    0x4412ECBD, 0x0E51D7B9, 0xBD4BAC76, 0xF9C750ED, 0x185EBA76, 0xEC9DB3AC, 0xD53AC371,
    0xA4739EEF, 0x3EED442B, 0x27DCCCCF, 0x03A7F753, 0xEF63EE3D, 0x86FFD97B, 0x769C758E,
    0xC798F140, 0x729E636A, 0xEEDDDFBA, 0x9F748745, 0x2BA71CA9, 0x6DC5384B, 0x35397366,
    0x5B34E92F, 0xBFA1D124, 0xEB7B7597, 0xB3AB0CE0, 0x963CEB91, 0xE52CBDE7, 0x69BA3FD1,
    0xECB4560B, 0x48D0FF51, 0x4E457EDB, 0xDFEA7D83, 0x2317FABC, 0x0FBBCA3A, 0xBF7543FE,
    0xAC9FCCF9, 0x3DC7BFF8, 0xF9FF979B, 0x7A4D190B, 0x4827FB4A, 0xDACF2F4F, 0xF9CAF6C7,
    0x099934B4, 0x3333E2DD, 0xDE61CEDF, 0x4CF78AF0, 0x124F43FC, 0x3D979FD0, 0xE8EC6DA3,
    0xF9F4A339, 0xF86EB5BF, 0xDECC3CF5, 0xC1D3B6C7, 0x13FDB31D, 0x3F6BE73D, 0xF4BD7158,
    0xA163AFBB, 0xB3A349BF, 0xFCB183D9, 0x131EAFB3, 0x0B5AFCA5, 0x4F320B27, 0x5A803EDB,
    0xBCEBD2C0, 0x5315A400, 0x8C0DB332, 0xBD7BE013, 0x3CB46C12, 0xCB1833F3, 0xA9B3F1C0,
    0x14C00BD8, 0x632C3F3C, 0x34AB4BBD, 0x3CA4B341, 0x83F34142, 0x29A8993E, 0x61A4BB45,
    0x3C154112, 0x33DB40BE, 0xE9B333E4, 0x0ADFBDF3, 0xB32BDBC2, 0x774242EF, 0x38778762,
    0x87B22A1A, 0xF12FE0C2, 0xE3C2344B, 0xBBFBBBBA, 0xC6396A1B, 0x49C63978, 0x395BB43A,
    0xB9432BB7, 0x4BCAC2E3, 0xC330CABC, 0xA8C36BA2, 0x4433BB83, 0x42EBA5BC, 0xBDBBC1FD,
    0x7C35C38B, 0x21B431C3, 0xAFC3543A, 0x1C34BC0B, 0x3C2BCB3C, 0x3EACE3B3, 0xE4433C3C,
    0xC41C3F3B, 0x383CBC35, 0x8C3C3C34, 0xC3443744, 0x0543AC35, 0x33B143DA, 0xC3D3BD44,
    0x4344644A, 0x0441393C, 0xBB7C42C5, 0xC5C44C39, 0x7900BC6B, 0xB783C8BC, 0xADC4BBA2,
    0xE3A2C3BB, 0x3861B837, 0x8C370377, 0x13553540, 0x08719CA3, 0x7A1FDB89, 0x9883B88B,
    0x375460B6, 0x868A336E, 0x8A0F3813, 0xC61B6737, 0x7D0CAB90, 0x4377B7CB, 0xB7EB7046,
    0x8837FC65, 0xFC66C673, 0x380A0CB5, 0x77B7DC62, 0xB469385B, 0x38B371C5, 0x6CB8037D,
    0x346DC6C4, 0xB6CC5AB8, 0x15222377, 0x7C74B793, 0xB6746D37, 0x69C6F468, 0x9AEBB70C,
    0x46F47947, 0x6E37047A, 0x047C4674, 0x367363B5, 0x72883C81, 0xEC5FC7DC, 0x466B71C6,
    0x76B8C08C, 0xA87A1E90, 0x8BA07A87, 0x73471C82, 0x6B7047D3, 0x97E3ECBE, 0xF0231283,
    0x1AA1408B, 0xC0BA8D3E, 0x312FF2A5, 0xE2F530E3, 0x4063EEBF, 0xF899230D, 0x5BF23E8B,
    0xBFEBF742, 0x8F323B49, 0x1C954192, 0xAF948E40, 0x009193F7, 0xAB2E3F94, 0x1B1BD2C9,
    0x203F5494, 0xA3D241CC, 0x33AC023D, 0x15408280, 0xC9A2C04B, 0xB4DAFD34, 0x9F330492,
    0x840690BC, 0x48F25941, 0xCFB04ADB, 0x23D532D3, 0xB23419C9, 0x09404493, 0x74A6495C,
    0x41EBE340, 0x98CA03E9, 0xE38A31FC, 0xC9632149, 0x36C9BBD2, 0x3491CB0A, 0xADFCA6B4,
    0x9140FC15, 0xBCB1BF5C, 0x4974ACBF, 0x26C9849C, 0x4CA4BF9C, 0xB00CC242, 0xA7C9ECC1,
    0x9C1A49CC, 0xC0ACA9C0, 0x7CCB24AA, 0x90A292AC, 0x0B90AB94, 0x2BB0E0A0, 0x00B98A9B,
    0xA6A0AEC8, 0xCE1568AB, 0x18AB88DC, 0x2DC8AD1B, 0xD33754D2, 0xCCD4238C, 0x263CD509,
    0xD73154D6, 0x34CF49FC, 0x0B9CD8BD, 0x9B4CE88C, 0x74DECCD8, 0x4E14D48A, 0xA8477887,
    0xD4D0A610, 0x4D8174CD, 0xE44D9198, 0xB45E2014, 0x4DC14E09, 0xD38A2930, 0xA0CACE2C,
    0x4DB2754D, 0xE60B98A6, 0xD4EC4ECC, 0xCE24DF4D, 0x008A8CF1, 0xCCEDCE7A, 0xA234E9CD,
    0x8C16ECEA, 0xA4880B20, 0x07B07A22, 0xECCF4C8A, 0x94D14D5C, 0x44FC58C4, 0x302AEB94,
    0xDCFE0C7C, 0xBB58E839, 0xF9C452D1, 0x7C51BC10, 0x8E74432D, 0x528E5D02, 0x7B95D014,
    0x44CBB2C5, 0xC0506894, 0x24FDD07B, 0xC3FC5743, 0xACD0AC44, 0x4D034473, 0x2ADD0DD0,
    0x09D0FD08, 0xA43CC365, 0x5070FC3C, 0x08C48510, 0x7C46D0E5, 0x502513D1, 0x44D1750C,
    0x9501D124, 0x51550F51, 0x1BD0551D, 0x2D1AD095, 0x1BFAB8BA, 0xD4C552D3, 0x5C569032,
    0xC37D1FD1, 0x7FC2BC8D, 0x2494C0E8, 0x3F14C8C2, 0xB5CBDCB1, 0xB4BF2804, 0xCB4CC941,
    0x0396CCB9, 0x44B7C97C, 0x499B2E4C, 0xA53F2C03, 0x841AC07C, 0xC9DCB44A, 0x273FFD27,
    0xE4165314, 0x4ADCB533, 0x1E52D533, 0xFCA44C03, 0xD3352C4A, 0xC833FCA1, 0x34C9CBCC,
    0x52F34D14, 0x813F4493, 0xEC02BFB2, 0x302ADAC8, 0x2ED40C9C, 0x0CB35375, 0x52AD2953,
    0xC5D33CBD, 0xFC9F2A44, 0x52B341CB, 0xC653E52A, 0xBBFF54BC, 0xB3CAA6B3, 0x50B42CA0,
    0x354949B5, 0x54ED38CC, 0x3BB1CD3F, 0x5CB8345D, 0x53AD3DB7, 0x18D57550, 0xC552D404,
    0x2A9C0A3D, 0x43CB2CAB, 0xD4AD2375, 0xCAEA3041, 0xFD3D910F, 0xBAF1C233, 0x421D2CD5,
    0xC0BE13EA, 0xCD5AD2BC, 0x41FD5854, 0xFACC5405, 0x7D454C6B, 0xD4F5354C, 0x65B1B564,
    0x9566CBED, 0xCA156F53, 0x712F34BA, 0x3D722EB5, 0x55B532D5, 0x3DD34D73, 0xAD29D643,
    0xD5ECCA3E, 0xB8D37CCB, 0x58D11DDC, 0x9569E41D, 0xB09AF9F3, 0x59E64739, 0x21E9FF9E,
    0xDC57D1AD, 0x51B857E1, 0x582D7DA0, 0xC2586D84, 0x49DFD839, 0x0CCD8858, 0xEC9DDD86,
    0xB2091D31, 0xD889D458, 0x255899E4, 0xDCFE1E2D, 0x58AD8CD8, 0xC9D8BD87, 0x858C357B,
    0xD8E1E61D, 0x87D89592, 0xF5971DE5, 0xD9559858, 0x91C3ED7C, 0xAD8E9D85, 0x3B29CD59,
    0x07D81D8B, 0x607FD39A, 0x101A29BC, 0xEB079879, 0x35961B19, 0x1B85859D, 0x21395A1D,
    0xF8839F2A, 0x57F9AE86, 0x579490CA, 0x0367A073, 0x5A843EA2, 0x85387A13, 0x89EE5ADC,
    0xC739B1A3, 0x135AE5AB, 0xDA012282, 0x8CAA1A22, 0x8DDB2156, 0xBB89A088, 0x225A0DA2,
    0xAEB94149, 0x9DAFDB65, 0xDB1DB6DB, 0x00DB2A63, 0xADB4220A, 0x1D1A195B, 0xB15B35B0,
    0x35B79985, 0x5B3DC05C, 0xE3DB4DB6, 0xC20BDBFC, 0xB715C25A, 0xBDDC45B8, 0x2A16DC7D,
    0xA1F5BC3A, 0xC9DBDDC2, 0xE5CF2125, 0x1E789689, 0x79A2A38D, 0x6B8F07A0, 0x5BDA1089,
    0x355615BD, 0x4545D703, 0x3FD336A8, 0xD8BF2553, 0x3D5849ED, 0x3F65394C, 0xDD5504BE,
    0x654B5565, 0xDDC54CD7, 0x24551A45, 0x3D31D5D3, 0x40C5E2D5, 0x4AD2DD74, 0x25545685,
    0x4004C2CA, 0x1C40154A, 0x6D61D5A4, 0xDED53A57, 0x90D7856D, 0xE297D4E4, 0x57B40B55,
    0xD798B555, 0x2B01D4C5, 0x5DAD4F56, 0x4FDDED59, 0xC5E75DCD, 0x5E8DF0D3, 0x4D3D6DE6,
    0xFDEBDE7D, 0xC96D6BB0, 0xEF5DF56C, 0x6D47A75D, 0x5F334E53, 0xDB5EB538, 0x1DFB57C5,
    0x603DE031, 0xCBB205F7, 0x55515F44, 0xD3DB3C37, 0xCB5F1CA0, 0x05E34C74, 0xD44DF3D6,
    0x465F55FF, 0xD5F05F65, 0x5EE30AC9, 0x06418D34, 0xF601CA3E, 0xDF94E8D5, 0x995E5558,
    0xF5E9DFE4, 0x60D5F85F, 0x17DF6D39, 0x3D77604E, 0xDDDE16E1, 0xF761D556, 0xDCA8E143,
    0x6135ECDF, 0xFC61FC9A, 0x960156E3, 0xCAA541CA, 0x44CABD42, 0x1C43AC93, 0xCFF905AC,
    0xCBD172AF, 0x32B92C0A, 0x2C4629AC, 0x2EACEAB5, 0xDACC2CD6, 0xE312CAE2, 0xD762CAC9,
    0x42CDAD0A, 0x2ACAC263, 0x302CFAC8, 0xB633E37E, 0xE28D032C, 0x398FAACC, 0x662FD05E,
    0x2CDE3A2C, 0x32E34E3B, 0x96326296, 0x635E4063, 0x3D2B9AD0, 0xFE38642E, 0x139641E3,
    0x022D4523, 0xC5BD5249, 0x0D1628E2, 0xEFB27D27, 0xE61BB3CB, 0xD6D4C15D, 0x36620E1A,
    0x56276125, 0x53CE4FB3, 0xF4E2256B, 0xFDE952FD, 0xDE4E24D3, 0xEED68E4D, 0x0E1E64F5,
    0xE23DFADE, 0x06E595DF, 0x1559E1C6, 0xE022D965, 0x0AD52609, 0x257A55DE, 0xDE6E0CDF,
    0x12BEE4B6, 0x5B2B6546, 0xD35C905F, 0x48E50E52, 0x4E0360DD, 0xB27E67E1, 0x674B3CA2,
    0x8E5DE21E, 0x65F554E0, 0x5CE605E4, 0x8D695F7E, 0xBF866365, 0x6AD48659, 0x16695EA6,
    0x5E1A6D62, 0x096644BA, 0xE61161B6, 0xE26579D2, 0x7BE5DCCA, 0xC65DCA5D, 0x64D53DE4,
    0x6E677E1F, 0x1DF9DFC6, 0xE5ABDE61, 0x69E5BD69, 0xDD27E6CE, 0x5E3CBB66, 0x7F656D47,
    0x9602E7BE, 0x65765C31, 0x83D57E55, 0x8C10E75E, 0xE6867CE1, 0xFA666E78, 0xC6612FC5,
    0x662E4EE0, 0x95C7AC25, 0xCC674763, 0x65D355C5, 0x83E96C5F, 0xF37BB81C, 0xE94B9446,
    0x781E3459, 0x05B84664, 0xC840A346, 0x71694699, 0x969F476C, 0x201484C5, 0x4A47F48C,
    0x44630943, 0xE93C8BB8, 0xA647BEA1, 0x35CF46A6, 0x6A35B947, 0x8CEA447E, 0x1483371C,
    0x37246B39, 0xD5C85E9D, 0xA467698D, 0xEB16A069, 0xAF47EE9F, 0x86B0C726, 0xEB2475C6,
    0xACEA76B5, 0x3C7CEA86, 0xC636976A, 0xD28A369E, 0x0DD4B8DD, 0x35EEB8C8, 0xE1596EAE,
    0xE8CD9D40, 0xC2F88A9E, 0x82594DAA, 0x49C25BE5, 0x6C3D8B3A, 0x926C11C2, 0x359F505D,
    0x59EC6AB8, 0x396C6D93, 0x2D9CEC79, 0x6A4EC944, 0xC644D585, 0xC9AD5B96, 0x6C059B6C,
    0x836D19D3, 0x0ECB6CDD, 0xD90BC9ED, 0xC2EC08E4, 0x7B986BF8, 0x59E0CA59, 0xC4D99ED9,
    0x86D39DDE, 0xECE3B01B, 0xD56C59DD, 0x86D15CBE, 0x6DC6C86D, 0xD88C9EE1, 0xA8EDED6E,
    0x3C61DA6D, 0xC822A100, 0xD1D83C93, 0x6C8D8A6D, 0x15E4B1B8, 0xB555D28E, 0xDDBD755E,
    0x6A28BE82, 0x5E73D67D, 0x24366B61, 0x2454B247, 0x668B56C4, 0x3D068F42, 0x15E6A579,
    0x94104126, 0xEEF56FEF, 0xF36F0654, 0xEEF45E8E, 0x41DEF55F, 0xF1427EF6, 0x1C298775,
    0xD27BEC10, 0x19D446F8, 0x9ADA6716, 0x54F6665D, 0x8EDA0E04, 0x56906526, 0xE74E0861,
    0x76684689, 0x2EEBE8DE, 0xE8766EDF, 0x0568AE6F, 0x7680D5FE, 0xEFAF0FE1, 0x81F08DFB,
    0x4E53F0C6, 0x6ABF106F, 0xA870BE0A, 0x257AE7AA, 0x4AC54366, 0xB1712DE2, 0x4DEAEEC8,
    0xE7FAE2BE, 0x9C7033D2, 0xA6F7D6AC, 0xF2053070, 0xEDC176F0, 0x6654E586, 0xD4DF0771,
    0x1EE8241B, 0xB882411F, 0x33371DEF, 0x22EF2716, 0xD261EFC7, 0xEFE6196F, 0xF9E75717,
    0xBF008B1E, 0xE5DC2ABE, 0x16B3671F, 0xEBBF51CD, 0xEC2A2350, 0x45D15430, 0xD51EC4C4,
    0xD0B734C4, 0xD4D21F35, 0xE6B0DC4E, 0x19EDBE87, 0xA7A2D5D1, 0x55C8A135, 0x5B67405B,
    0x3F205DD0, 0xB1BFDB67, 0xDBBDBDDC, 0xACEB4740, 0x738B7415, 0xF47DBF5B, 0xB0F42224,
    0x5BC9DD6D, 0xF3CF3DF4, 0x3FDCAF3E, 0x5F475CDF, 0xF46A0BDC, 0xEF5B9742, 0x521C7441,
    0x75575174, 0x2073A458, 0x8505F3BD, 0xC47D1250, 0x47C548FE, 0xF5256E86, 0x51842D51,
    0xAB9AD4E8, 0x58AB8CF8, 0xCE523169, 0xDC6B409B, 0xF8A2CDFC, 0x0BE7601B, 0x9587DCF5,
    0x81EB1EA3, 0x0771EB3C, 0x781A3878, 0x2769D248, 0x07FDA390, 0x6A903F6C, 0x176D1A3F,
    0x877F6E77, 0x7F878078, 0xD878F728, 0x76E76976, 0x29076F6D, 0xD778F75C, 0x774774F6,
    0x76F79773, 0xDF7076E7, 0x771903F6, 0x7D524F69, 0xAF78F7EF, 0xF6F773F6, 0xEB77677B,
    0xC770F779, 0x77AF83F7, 0x80F7A775, 0x87787777, 0x782F6E78, 0x81785F80, 0xCF6EF6B7,
    0x75CF8BF6, 0x79F8678B, 0xE787F7B7, 0x784F6E78, 0x8AF78F83, 0x9F738787, 0xF8CF6A78,
    0x94780F82, 0x278FF857, 0x078F90F7, 0x7FF95F75, 0xA79BF928, 0xF95772F7, 0x85796791,
    0xFF8E77E7, 0xF9BF8CF7, 0xA0F8D781, 0x87847997, 0x07FF68F7, 0x81C8A489, 0xFF66091C,
    0xF64CF50B, 0x644738B9, 0x788B4F2F, 0x489C88C8, 0xFC760CE8, 0xD09B5CD4, 0xF1C719F0,
    0x7964CE65, 0xDEF15E86, 0x871907F0, 0x7F873073, 0xE85F05A0, 0x85E07F85, 0xE9062062,
    0x186285D9, 0x7BD10086, 0xBCFBDFBE, 0x022A7BDF, 0x7C27BF7C, 0xBF8627C1, 0x1FC1FBEF,
    0xFC4FC17C, 0xC3FBFFC6, 0x2FC77BC7, 0x7C5101FC, 0xC09017C9, 0xEFCB7CA7, 0xFC47C37B,
    0xB57C9FCE, 0xBFCF7CA8, 0x7D14557C, 0xD2FC67C9, 0x07CC7C7F, 0x7D57CD7D, 0xD0FC27D2,
    0x77D47C8F, 0xFD87D1FD, 0xD97CCFC5, 0x7FD3FD1F, 0xFDBFCDFD, 0xDD7CF7DD, 0xF7CE7D87,
    0x7E1FCFFD, 0xE07C87C5, 0xAFDEFD37, 0xFE57E3FD, 0xC87D07E4, 0x57DE7DAF, 0x86185C8B,
    0xD1FBDFD5, 0x7F286985, 0x687E00D7, 0x61F18F00, 0x5F1A6926, 0x67D5EDC1, 0xB467E7B3,
    0x8F1E5FD7, 0x86E2A368, 0xC2070070, 0x64560787, 0x48807707, 0x82F82F7A, 0x87F9F7A7,
    0xD24FFBC8, 0xB37F9F82, 0xEF768800, 0x2783FDDE, 0x83C15E4F, 0x6BAE1109, 0x44216ED7,
    0x12050883, 0x5DCEE764, 0xFF128544, 0x2EF87C59, 0x088CBC5E, 0x28EC863F, 0x8C45A212,
    0x3BC1E0EE, 0xC3E1B1B9, 0x5215328D, 0x267357FC, 0x5D0E97C2, 0xD2D95C2A, 0xEE944E24,
    0x493CFA1E, 0x9E50E392, 0xBC61DF13, 0xC329F1A9, 0x1637419D, 0xA914A94D, 0xB5469047,
    0xB42A8502, 0x30ABC0E9, 0x3AB4D6AF, 0xA3C9A155, 0xCD768F41, 0xD16AD5E9, 0x11E113B3,
    0xDD0E972C, 0x74BC593F, 0x6FB7D3FD, 0x827EBFDF, 0xFC2BF1FF, 0xC4E230EF, 0x5631F388,
    0xE0B0F8EC, 0xE9F57D7E, 0x7298EC7B, 0xFF96C8E5, 0xB93CEBF9, 0x1775E5B2, 0x2622E9FF,
    0x7B3D5FEC, 0x662736F6, 0x62358F9C, 0x5B017FCD, 0xB38FECEE, 0x66FFDB67, 0xFB8BFEEF,
    0xAEDD6EB6, 0x0DD7137F, 0x7FB91CAE, 0x232192C0, 0x67B8386C, 0x15F9CECC, 0x3A59ACF7,
    0x799EE074, 0x7C4ECEF3, 0x85DD77B8, 0x06DE27A9, 0x60BFDBEE, 0x1E298EC9, 0xFBB1DB0F,
    0xBF759AF7, 0xB75EFDE7, 0xDFF1F7E6, 0x3ACFDE23, 0xD9877F3F, 0xF5779B07, 0x61E016F9,
    0xE08821FE, 0xE7CDFC6D, 0x21B48199, 0x0A6B2008, 0x7C9867EA, 0xDB07D183, 0x37E1AC80,
    0x21E485D8, 0x1463E168, 0x3D1BA7F2, 0xCFF3D0F5, 0xF2F0BE2C, 0x4D943E4F, 0xE1601BD7,
    0x6E5E0679, 0x1EE6DE39, 0x27123A76, 0x5DDE7B9D, 0xEA77A327, 0x69DA54EC, 0x9D53D629,
    0xD8E1B776, 0x75EE7124, 0x5574A3E7, 0x960F88CA, 0xCF8960FF, 0x774A5D67, 0xCA5E980F,
    0xE2375BC8, 0x2FCFF384, 0x65A318C8, 0x06565696, 0x36303DE6, 0xAF9CA6E6, 0x656629C1,
    0x26C96DD2, 0x197E7497, 0x0E329FA6, 0xDE365EE7, 0x31E68374, 0xE776F26B, 0x770E7470,
    0xED95A067, 0xFFA467B9, 0x9D232A40, 0x2989B693, 0xD9866FA7, 0x5A75A768, 0x7AA8680A,
    0x9E6A0966, 0x2A9A5EA1, 0x5ABA7CAB, 0x8A78AAE9, 0x3574AB1A, 0x388E337D, 0x2746BE6C,
    0x93A8E76F, 0x94E23FCE, 0x9AAC8E53, 0x8CAA59F6, 0x1C3A86B4, 0x1A7AC8A5, 0x3E5DAA27,
    0xA6AB6A4A, 0xB56CD972, 0x6C5B52C9, 0xCACAD896, 0x36CB9CE8, 0x7BAB6CA6, 0x6368EB46,
    0xCBD2CD35, 0x26A9A82E, 0x9274B7AA, 0xDBB8ED2B, 0xB869FBCA, 0xA71B4EAD, 0xA3AABD74,
    0x98BB3FCE, 0x0A3C4FF6, 0xBF2F76F1, 0x69BBC2CE, 0xF8CE21AB, 0x1DEF70DE, 0xCD72DC97,
    0x8C99786D, 0x9BA62626, 0x0C3E1174, 0x99B0C45B, 0xE0384DC8, 0x629EB728, 0x18F8363D,
    0x4C7A32C7, 0x572163A2, 0x9B875868, 0xCB59BCA9, 0x9BE871A0, 0xBCC98FC6, 0x2B307B99,
    0x4365B3F6, 0x2F18ECEA, 0x4FF310C5, 0x6274A024, 0xE19B5D13, 0xAD27D8D3, 0x2A0FF2A8,
    0x19B2C4B4, 0xD6B7426F, 0xD5928658, 0xB5CDD96C, 0xCEF2E745, 0x18BD7980, 0x7949B0D8,
    0xB3656C20, 0x5CD1F67C, 0xDBA20CF3, 0xCF43C986, 0xD8BA2C3C, 0xAD9A6197, 0x25D1187D,
    0xDC71D60E, 0x58EDE319, 0x86899BCE, 0xAEEE9211, 0xFFDB7106, 0x88F6360D, 0xAA2E0D86,
    0xDC9DC8C4, 0x4987CF5C, 0x688FFA17, 0x8635D6B3, 0xD9BE5720, 0xCC9F25C8, 0x3372625E,
    0xA87F374E, 0xE15AC88B, 0x370CD1F0, 0xD62BB26B, 0x7FA4C7B8, 0x9BC39AFD, 0xD198AEE7,
    0xE613D98A, 0xF2B8B028, 0x80A4254F, 0xCD314FF2, 0x3099B330, 0x8FF324CA, 0xF26C9E23,
    0xCCB52A4F, 0xA9DAB762, 0xD17ABEE3, 0x336EF397, 0x7DCE1091, 0xFBA7E21B, 0x8FFA3C66,
    0xEF612DBD, 0x3B3CAFED, 0x1F93A6FC, 0xA680E2D8, 0xEDF9BE37, 0x8C017E45, 0x99B7CCE0,
    0x792625E3, 0x005D19EF, 0xD0BB4637, 0xA972268C, 0x88EA9C99, 0xE1F53227, 0xA233416F,
    0x3359069F, 0x8711CFB1, 0x4FE3FA6C, 0xC207FEFC, 0x30EFB913, 0x72D93324, 0x14988760,
    0x967FD03E, 0x187F1DB1, 0x75DD9FD8, 0xAB7584D0, 0x331F0FE1, 0xB02DFDA5, 0xCEA3BA7E,
    0x4133EF0A, 0x1C2374C4, 0x2C853267, 0x648FF160, 0xFF19E340, 0x7514A280, 0x84BC1763,
    0xC0FF164F, 0x81803085, 0xF1785A35, 0xA053F386, 0x18787465, 0x630E60DD, 0x1BDFCC41,
    0xCA438315, 0xD4805085, 0x6CCA6369, 0x2638B898, 0x71FC8866, 0x54543CCF, 0x6E04DDC0,
    0xD0244930, 0xC258E112, 0x1DCE3A3D, 0x6A9C8904, 0x164744E2, 0xDC8F06DE, 0x866F0DF1,
    0x024B9C33, 0xE27B3786, 0x81BC8CE4, 0x8321342B, 0xE1E2833D, 0xC984C6AB, 0x93E60255,
    0xA891A4AC, 0x4F4AF936, 0x07940966, 0xB4649C53, 0x906F0DF1, 0x71EE4D72, 0xE9672765,
    0x32C2601B, 0x65918D93, 0x2AE99330, 0x9346FA5B, 0x2B66618D, 0x654C296B, 0xD1941312,
    0xA732E682, 0x6A67CB19, 0xB4AF9AB3, 0x73250016, 0x43FE308B, 0xC528A813, 0x8B915C3F,
    0x1283FC5B, 0x399E98A3, 0x751781D4, 0xAB13B3B2, 0xB718C540, 0x88528956, 0x40E41FE2,
    0x333839C7, 0x06BE6718, 0xA654DE9B, 0xCC599074, 0x99534A61, 0x2C283CA0, 0x6A50BA09,
    0xDBA27436, 0x2B37682C, 0xBE8D506A, 0x94DA9AF2, 0xD25A47CE, 0x4AC517A2, 0x28E50FA1,
    0x5284AF42, 0xA36B7A58, 0x3A21C25C, 0xD20EB1D6, 0xF31E4C0C, 0x58FDDBE8, 0x565BD2BA,
    0x4467A4C6, 0x430CE6A2, 0x9505A0A7, 0x2A227B3A, 0xAB733475, 0x3D89A55C, 0xA8AA2A40,
    0x53D39870, 0xA2AAB502, 0x7F55AA7A, 0xCEA42599, 0xD19A3788, 0xB4D32F25, 0x9428CD26,
    0x0D7CC6AB, 0xAD13DD57, 0xA8E61AB4, 0xA091E550, 0xEA56C31A, 0x67AA1594, 0xEB5420FD,
    0x6EB6D59A, 0x0D42ADF5, 0x155D5CD5, 0xE6F9AED8, 0x05389928, 0x8B71FE29, 0x18F18860,
    0x3FC668CE, 0x548AC13E, 0xC617A3FC, 0xCC389A58, 0x706BAC41, 0x5CE91B63, 0x512E3FE7,
    0x23FC518A, 0x17C9DFA4, 0xD6EAAD5E, 0x99B1DEBD, 0x5D2BF572, 0xAB3E59B9, 0x6BB035DE,
    0x7160AD85, 0xB2D7D09B, 0xED3FAA36, 0x8AD16BAA, 0x16A6BD5B, 0xA9194796, 0xD58A2AA5,
    0xB8B6AEE2, 0x6CAE1DB5, 0x19DC1B9D, 0xEC1D2669, 0x4F4D0760, 0xAC7A0F33, 0xEDAA8128,
    0x6B9F6F6B, 0xD1A1E2D5, 0xCC544667, 0x48B62B08, 0x67138637, 0x79615489, 0x06DE7663,
    0xAF85F006, 0x2E6563CB, 0x9AF7BBF6, 0x9C39B671, 0xDB5B0EEF, 0x4EF8C7C3, 0x23E3FD82,
    0x411ABC62, 0x32D1D6F3, 0xD8649D5C, 0xEC246388, 0x305122FE, 0xB8361FF2, 0x1C7F8DC1,
    0xB59019E3, 0xE50F2E8C, 0xF10B0FD8, 0xA0DE10B7, 0xC3E7D6D8, 0xC0F7EEF5, 0x067133B5,
    0x204A88FF, 0x5DC0D8B2, 0x8F82DE0A, 0xF5BD8FF8, 0x28FAB90D, 0x690E64A4, 0x90D0C939,
    0xAF74B8C0, 0xB908298B, 0xBFF05D81, 0x8A5C360D, 0x07A01C07, 0x1473B8C7, 0x577B4E63,
    0xA5E03A4C, 0x7BE763ED, 0x075A63B2, 0xEEC59443, 0x724B6684, 0x476C8597, 0xBF58D258,
    0x63B22E36, 0x13992C14, 0x55BC6CCF, 0x198DF146, 0xB3241B89, 0x1DF1CD39, 0xC321801A,
    0xF99A1441, 0x9F96797D, 0x66BCE97F, 0xBF39963C, 0xA1F16C2D, 0x00BC27FF, 0x3E8098EF,
    0xFF905BBE, 0x1D073C3C, 0xDDEC8F4E, 0xF3117F32, 0xCE4AECB8, 0xB28E9CC9, 0xE5431307,
    0xBD27F32F, 0x041D2658, 0x38686CE7, 0xACC611CB, 0xEC62708E, 0xD6FC431B, 0x465C3118,
    0x088B97B4, 0x98CCF6E6, 0x75F66E91, 0x698CE3A8, 0x76F79353, 0xA4753EC2, 0xD2E39CB5,
    0xB6405821, 0x1984767E, 0xF342C33D, 0x4E25DA2F, 0xDCEB9DF4, 0x7B1E4567, 0x35AEC98D,
    0xEAADA702, 0x6FF8F601, 0x9AAF6C07, 0x4F6447D3, 0x68D41AF3, 0x2DA8CC76, 0xCB452EF8,
    0x7E80DC7B, 0x758D448D, 0x0BE159D1, 0xD32DD1E0, 0xAA3AC499, 0x5EB4F584, 0x31D73271,
    0x82A76E23, 0x12E05C0F, 0x99C0D7DD, 0x147FDE05, 0x561550EA, 0x25A0B694, 0xCB8381F1,
    0x157726E0, 0xF18E4FC4, 0xE2C6378B, 0xCBF95F23, 0x8AF25E47, 0x3154EB77, 0x55C8B89F,
    0x739AF2DE, 0xCF38E589, 0x34B9CEF8, 0x29DE8973, 0x78A724E5, 0xF40E79C0, 0xE51CFB9F,
    0xD339D736, 0x9CF04E6F, 0x4F5AFCD3, 0x93D53A3F, 0x4B9EF56E, 0x475DE87D, 0x0E5DC8FA,
    0x04095175, 0x576AEB23, 0x30BED23D, 0xEBDD8FAE, 0xDB93DF27, 0xA6AAEEA7, 0xBE3A5CCF,
    0x4EC09602, 0xEC909063, 0xD771E4F6, 0xDE07D6F8, 0xC6C3666C, 0x769BA0D5, 0xEB4CF9AC,
    0x8D3B5E3E, 0x6232CE81, 0x6DE64D74, 0x6F889A3B, 0xC1E337A6, 0x09BCEB7D, 0x5795D098,
    0xBC13E63C, 0x0FDB7E6C, 0x730FA4AA, 0x25DDABA7, 0xD95B7B7B, 0x9E36C79A, 0x6E3A2DB5,
    0xA2F17A43, 0x87E3BD7F, 0xB7CEEEBD, 0x5797D1F9, 0x4C659CBD, 0xF20F1E1D, 0xFC25DEDD,
    0xEF5F5B90, 0x315AFF69, 0xBEF266F9, 0x41BACDC3, 0xA6413AA7, 0x0C65BE97, 0x334FF90F,
    0x0F99DCFF, 0x9DA1EE76, 0xBAEDD76B, 0xF736BDD1, 0xECEBE1BD, 0xF64C3BF0, 0xB5372A5D,
    0x596E9C5F, 0xF1DC5E6F, 0xCDF2B286, 0x5BDB7ACF, 0x860CF71B, 0x1815FBB6, 0x7F5FA5B2,
    0xB5FFDF81, 0xD9198A9F, 0xFE599DE7, 0xADF6DEDD, 0xDA5DF119, 0x7CA019FC, 0xA46C5EE9,
    0xE03DE79E, 0x92D235EF, 0x9D93469D, 0x5F50F21E, 0xEC1AADF5, 0xA8E79089, 0x191DAC17,
    0x2C1FF203, 0xD1EA1FDD, 0x9E483FDF, 0x02DECD0C, 0xF5E85ACE, 0x9BFE051F, 0x8ADBF9F0,
    0x15F35FC1, 0xCE11A4E0, 0xFB2091E3, 0x260B19E1, 0x60DE0ADE, 0xDB1A6A0B, 0xA1A0A058,
    0xDE59EF5A, 0x0E203600, 0xF1B120BA, 0xDEF9B19E, 0xF060B214, 0xBA044C39, 0x2159B2DB,
    0xEA9BCD7D, 0xC60DA121, 0x6079F3D1, 0xF4DFD5F4, 0x69FE2141, 0x1E24F246, 0x9EC60E1C,
    0x2864A1CC, 0xC38C3915, 0x591F057D, 0xCE1D9745, 0x61FC70C8, 0x1FD91E1C, 0xFC60E1D6,
    0x07991FA1, 0x2943007A, 0xB55517B5, 0xE205F1D7, 0x19224E21, 0xB6251EAE, 0xE1DE2197,
    0x2089D225, 0x416F140E, 0x861921A2, 0x29226A24, 0x5E1F0976, 0x62822662, 0x2A21CA25,
    0xA2274966, 0xA2AA28DA, 0x24604D6F, 0xCA2BC636, 0x22A62DCA, 0x2B06FA22, 0xFE23E296,
    0xE2D95AA1, 0x66620E25, 0xBA1D230C, 0x22AE31A2, 0x2FE25E21, 0xBE30625E, 0xCBF62DC6,
    0x6022EDB2, 0x4D20DDEC, 0x4601460D, 0x3222622F, 0x4C6CA296, 0x142A2DD7, 0x30A28A30,
    0x6175A296, 0x576CE457, 0xABA32577, 0xC94F22DC, 0x14493622, 0x2809DD36, 0x983787B9,
    0x52992909, 0xB1D2D174, 0x81280654, 0xA2CD4B93, 0x47E3D145, 0x18A108C9, 0x48CD2F4A,
    0x4A937D4B, 0xB14654BD, 0x4A2D459D, 0xD063C498, 0xF5432445, 0xE40145A3, 0x36A29A43,
    0x4D43A411, 0x93294F97, 0x43646647, 0x6134645A, 0x64963FE4, 0x2C248640, 0x4A4A648D,
    0x245D4ECA, 0x4C14A647, 0x9E47E43A, 0x928240A4, 0x4554764B, 0xD234A406, 0x64DD3664,
    0x4163F92A, 0x20514656, 0x5C30F2D7, 0x4ED2BE4E, 0xF1442502, 0x92C24CE3, 0x4C57FA46,
    0xD8E454CD, 0x92A14E14, 0x3609C644, 0xBE0F254D, 0x6135BAA1, 0x96209DAB, 0xB2009FC1,
    0x60C18E17, 0xA3C87A11, 0x2DAF20C9, 0x1E8E1708, 0x005FE1F6, 0x79F9A12A, 0x5A6DFC0C,
    0xE7210584, 0x2A011220, 0x65BDF9E0, 0x1AA5F197, 0xAA5D4711, 0x660E5DDE, 0x13A10A58,
    0x7A61D7B6, 0x88D65C50, 0x2058EA16, 0xED7208AD, 0xC75873C3, 0x5B25F662, 0xE57B25FE,
    0x066DE525, 0x2405843B, 0xDA080985, 0xD839FB98, 0xF661B1F5, 0xA9E1E591, 0x9FEE6865,
    0x5CD1CE12, 0xB862A0BE, 0xA0F9F9A5, 0x18A039F7, 0xEA63DA52, 0x9BA267A6, 0xE7201E06,
    0x1A5EA0E5, 0x619A12A1, 0x71A645B7, 0x25B6658A, 0x664E67E0, 0x130CBDFC, 0xFE0EE632,
    0xE0FE74A6, 0x8EA70DB7, 0x8501A17D, 0x66727561, 0x25A1521B, 0x29FA26C2, 0xDA961A5F,
    0x1BA05E69, 0x12111AA6, 0x995E09A7, 0x59E748C5, 0xF5B0260A, 0xDF8E79E6, 0x7920CD98,
    0x867DE626, 0x14E66167, 0x6CE5C18D, 0x5A6806FA, 0xE7ADE0E7, 0xED211E79, 0xAE7518AD,
    0xD9F27C1F, 0xF0281263, 0xAA845E35, 0x122A1626, 0x605FCA85, 0x543FDEEE, 0x666120E6,
    0x84228552, 0x91252056, 0xA7FA7D67, 0xA91239F3, 0x49F21245, 0x0ACA7428, 0xAA8BC538,
    0x012A8BD8, 0x2425C206, 0x55CB3E8D, 0x55CFCB89, 0x8AC1C08A, 0x27CBEA8D, 0x28A21C76,
    0x9DD46F8A, 0x8C9C2E8F, 0x768EE496, 0x28CCBCC9, 0xC50B7CC3, 0x2A8FCBA1, 0xA909D18C,
    0xD28A6652, 0x36936969, 0x0B6695DC, 0x5C4B9E8F, 0x04B72985, 0x0A56979E, 0x93297DE0,
    0x32982992, 0xA9769029, 0x93A944C2, 0xF4C2294E, 0x299A9128, 0x9AA44297, 0x20584BA6,
    0x69AD9357, 0xC60C2A9A, 0xCA9BA979, 0x83FE4FDD, 0xDE4C00AF, 0xF0C18C11, 0xE96E8D62,
    0x8DA93E92, 0x552BD254, 0xEA60A8AA, 0x0921E437, 0x3C8F8905, 0xE27890A2, 0xA26A606B,
    0x3093EA62, 0x86E88CC9, 0x7097DE22, 0xF6AA2394, 0x48FAA8E1, 0x7791F8E3, 0xBC77AA9C,
    0x6ABAA9A3, 0xAD864AA8, 0x66418952, 0x6AB49426, 0x76EB06AC, 0xDEACAA5C, 0xEA606348,
    0xB0AAA233, 0xC08FAAA6, 0x0ED2B32A, 0xB26A7EAD, 0xFC8EEB1E, 0xAAD5E7D5, 0xA8AB3EAE,
    0x4D8BAB46, 0x2AF4942B, 0xB5AAE094, 0x4083CA8A, 0xEA92B409, 0xA60752AF, 0xCC3C46F2,
    0x57204203, 0x4E46548C, 0xA2B0EAE5, 0xABDAB2EA, 0xB76BA2B3, 0x0C91C916, 0x6B78928C,
    0x5C2B96A6, 0x7A74AB65, 0x83FE7E28, 0x3160A5F6, 0xFA74E79A, 0x20EA8959, 0xF91E45AE,
    0x060D1E4D, 0x5EBA76A6, 0x71682273, 0x2E14A762, 0xA04E8168, 0xC8D71682, 0xC923103A,
    0x9FFA8566, 0x94A73E89, 0x92701AF1, 0x66B26D27, 0xFB2865E9, 0x8672E0D4, 0x615E772C,
    0x77DBB5A5, 0x9AC2604E, 0x2CA122AC, 0x07DA9207, 0xB60866D6, 0xE7BD07A7, 0x6B9E16CA,
    0xD672ACE6, 0x6CFECE1E, 0x7F2C557D, 0x0AC667D6, 0x680AC267, 0xC3EC7E16, 0x0E611B02,
    0xE0966AE1, 0xC3683278, 0x6ECA5E82, 0x9BF678AC, 0x872D9AC7, 0x91B16D66, 0x26EE765B,
    0x7BA5F20E, 0x805F6778, 0x671EC467, 0xCE6D91F2, 0x167A6D5E, 0x26DACCAD, 0xB1ABB51E,
    0x5493AB12, 0x2C2AE1AB, 0xA797AAA7, 0xED72C942, 0x6B95FBEA, 0xB8AB96A9, 0xFEB00946,
    0x5AEEAAC3, 0x712B9CE3, 0x12BA8F24, 0x6E2664A5, 0xA411B2E5, 0x1EE3AC1A, 0x8CDEE419,
    0xE52E3C77, 0x82AA2E22, 0x2B2ABE92, 0xEB2C0AB6, 0x7E256BF2, 0x2EC52C2E, 0xEE2ECEC1,
    0xDC87EE72, 0xA8DEE4AE, 0xB16E8EC0, 0xAD09E36A, 0x2B72EA2E, 0xED6E26F0, 0xBC61ABE2,
    0xCDFEBC6B, 0xE85702BC, 0x5EF12AA2, 0xAEA8752B, 0xE5EBD2AF, 0x42E46EFE, 0x443C9189,
    0xEEAC04E4, 0xA2C16E26, 0xE9A29C6A, 0x2C290E9C, 0x3DC2A8E5, 0x29E692A9, 0x9CA9629C,
    0xDAFCE98A, 0xE91A23EF, 0x9564B5C7, 0xD69D292E, 0xA95250E4, 0xFD4B96A4, 0xD29E300A,
    0x4A869E8B, 0x00EFAE96, 0x32904977, 0x2A169B30, 0x04AA1F04, 0xDB03EFDB, 0x1DAEFFEF,
    0x022FC69B, 0x0699EFDF, 0x2FA69DF0, 0x01EA4708, 0xEF079DF7, 0xA92F0969, 0xA94BB0BA,
    0xF7042A09, 0x0AB70269, 0xAC709B07, 0x0EA30BF8, 0xC420C14C, 0x0349FE3B, 0x2693F09B,
    0x8626D1A8, 0x0AA599F6, 0xBEC419EA, 0xE7E9BF68, 0x80E8A97D, 0x6AC8F11E, 0x082286AD,
    0x636D7E73, 0x22C4AD2A, 0x71161967, 0xDE67FF14, 0xC5EC673A, 0x311B1328, 0xCCF12317,
    0x4A5A257E, 0xADD2CE31, 0xCF120461, 0x6688D212, 0xEC73175A, 0x16A726DE, 0xEAD0ECD7,
    0xEE05061B, 0x0EF19AD1, 0x22C2EC26, 0xE06F1D5E, 0x6DB19283, 0x85F9712A, 0xA732CBE6,
    0x0F6CD2DB, 0x7E706DC6, 0x31F7151F, 0xD01E3E80, 0x0ED0B1DA, 0x208AC972, 0xD5ED72DC,
    0xF322DF7E, 0x323A6E71, 0xB8A5CACC, 0x91F1ADD9, 0x31EF2128, 0x63F25EDF, 0x8A1AEDFC,
    0x7182DAAD, 0xD371FA71, 0xDACDB112, 0xF1072631, 0xC5B21A14, 0xE66867F6, 0xE87B1EB2,
    0xD9F1A284, 0x96DA681E, 0x3276C271, 0x2FEC1F22, 0x9F2EB13B, 0x880730E7, 0x15724F22,
    0x132E2DBF, 0x331A8733, 0x65D20920, 0x16DA1902, 0xAC5F32A6, 0x689AD331, 0xB8C0D23C,
    0x6E92C8E8, 0xC9E452CB, 0xBE44E3CD, 0x528E3D92, 0x4E252D53, 0x3E3BEA26, 0x23EE4825,
    0x50B3BE53, 0x25581E26, 0x24A156A4, 0x3B300243, 0x72FF33D3, 0xE3C936A5, 0xE973DA46,
    0x7945A506, 0x65365094, 0x3B9C8D44, 0xAB3F7423, 0xEF0B42E4, 0x6024C73D, 0xF3447428,
    0xA9BE4933, 0x43746A4F, 0x534773E3, 0xB4824774, 0x50B4A135, 0x7253A42E, 0xA57701F4,
    0xDFE51C8C, 0x57076524, 0x303A49B4, 0x54B4BB4C, 0xCA553462, 0x2561A914, 0x4E741EF5,
    0xEA3D709B, 0x56E16016, 0x5287B973, 0x262373C9, 0x554AB2DE, 0x57227D5E, 0x2D6F56DD,
    0xDB2D5DB5, 0xB41C2B3F, 0xDAB2D606, 0x16056A16, 0x09955B56, 0xF570B583, 0x574357D6,
    0x40571636, 0x2EECF5B3, 0xF58D5717, 0x55623F50, 0xBB599747, 0xCAED6C75, 0xC6D732BA,
    0xDD74D599, 0xE39357B5, 0x55971759, 0x049735F3, 0x35AB5956, 0x5522035E, 0x0F5AF54F,
    0x23586F96, 0x5C131172, 0x4F5C9543, 0x155B6355, 0x60F5D571, 0x9175D757, 0x9BC23A9A,
    0x5675F765, 0x6F60F49F, 0xEDEB158E, 0xC3B1FC66, 0x3B20AD3E, 0xE7B33073, 0xCBAD526D,
    0xC734F28A, 0xD8EB31F1, 0xD9E126DA, 0xC7103246, 0x68A213F6, 0xD4616F20, 0xCA003352,
    0x76DB38B2, 0x8076E371, 0x7323735A, 0x92C2DCB2, 0xA59F0F1A, 0x1ACFA181, 0x72AD8477,
    0x2BA705F7, 0xD32C31E7, 0x19AF7225, 0xD3A7CB2D, 0x9EC8370E, 0x20C2D4AD, 0x74332E6E,
    0x626F323F, 0xB786C777, 0x177746D5, 0xDAC9333E, 0x9B4B2577, 0x12F386C3, 0x6B253732,
    0xF2271DB2, 0x30B2A772, 0x128377CB, 0x666F6F77, 0xCF7756DD, 0x377FB68A, 0x26037E66,
    0x055BE32B, 0xAF779F41, 0x26A678E5, 0x7C77C332, 0xF6C31092, 0xE85B7D2D, 0x2F1F26D2,
    0x9F81ED57, 0xF872DB77, 0x2DF819A2, 0x6ED7B6BF, 0x19DB7366, 0x7697B770, 0xAB8877EF,
    0xDF2715EC, 0x8A619F6E, 0xB275F83F, 0x78DEC638, 0x29A87ACE, 0xCB812DE3, 0xADB38D76,
    0x2BED1192, 0xB72C7863, 0x1D89D36C, 0x991C0DD6, 0x95C2A3E0, 0xDDCDC394, 0xD85D65D7,
    0x81DD1D29, 0x9C6DDBDD, 0x3ED85AFE, 0xC9D2DC84, 0xF97B945D, 0x94B99F96, 0x4F981D13,
    0xDE079A79, 0x98797397, 0xC79AF9CB, 0xB9AE9B79, 0x9D39B9D1, 0xCB9EF06B, 0xE9AF9BDC,
    0xA05DFB9F, 0x0B9F7A03, 0x39F1D5FA, 0xDB79AB9B, 0x1FA13A15, 0xFA0E457A, 0x8B1D95C0,
    0x2DDA70B0, 0xF9CE9AC9, 0xA39D0BA2, 0xC3A4A933, 0xC6575E9C, 0xC0CC05DD, 0x8309A358,
    0xF969D57A, 0x77F6FDC9, 0x5B6B063B, 0x328F76F2, 0x1276AB8E, 0xD67EEC53, 0xFACB7B76,
    0x232D8337, 0x2780A603, 0xFAD2C3B8, 0x7678FB86, 0xF2D971D3, 0x791F8F76, 0xCDF8EF70,
    0x638AB7A2, 0xB7B32828, 0xB0662B92, 0x5F915A53, 0x7163B137, 0x8AF84E06, 0xE27AA076,
    0x482F8037, 0x87717B6A, 0x1F2020A3, 0x73038F3B, 0x8971EFAB, 0x8FB4FAE3, 0xB73B13F1,
    0xB0372BAF, 0xCF7DFB2B, 0x9F738478, 0x1577BB8D, 0xB7783B3E, 0xFAE7286C, 0x143BDB6D,
    0x73BD3807, 0x6CEF74E8, 0x267919F1, 0x718D2257, 0xB774627B, 0xC0237FBD, 0xBBBFB2CF,
    0x387ED37A, 0x7A676FC4, 0x07ADF6CF, 0x1FABBBFC, 0x747BCBC1, 0xC31A7C0A, 0x3AEFAF38,
    0xC3E06384, 0x37C0FAC7, 0xB727B47C, 0xB67B3BC9, 0x43B31B8B, 0x217BBD6D, 0xB62DDF83,
    0x9FCB3C4F, 0x9BD3CBAC, 0x1E3CC3C4, 0x5F527621, 0xF1BF6935, 0x1EAA6C3F, 0x0B541AEE,
    0x76137F76, 0x50C8CF66, 0x0FD27529, 0x879EAAB5, 0x57623472, 0xF75A56E3, 0xFD23D556,
    0xAB752361, 0x43D7B600, 0xEE83D1BD, 0x5BF65F62, 0x5F5C75EF, 0xB69F663D, 0x673D4763,
    0x1BD8762F, 0xA3BF62F6, 0xDA35A3D9, 0x88977D93, 0x3DB7DC35, 0x5D16F6F7, 0xD3DA762B,
    0x4427697D, 0x72B64C48, 0x8F68FDB9, 0xB5FD787D, 0x66B54B62, 0xA367A27F, 0xF684C0E3,
    0x6435B5F1, 0x0FE0EF1B, 0x73B74AF5, 0xD4745B4A, 0x3553128B, 0xF4F24B22, 0x43E4DB46,
    0xAA40F4B3, 0xD7B740F4, 0x4289AB56, 0x3A4BE94E, 0xF41BE874, 0x47B4E656, 0x97EA945B,
    0xF4434C34, 0xEE7E93EB, 0xCBEEE4B3, 0xB4674134, 0xEF3EAFDA, 0xC24EBEBB, 0xBF1A503E,
    0xE7B4324C, 0x8A4F3EE3, 0xFF1347FE, 0xF4FEEBF5, 0x9BF57493, 0xE5174C74, 0x8BF4D03F,
    0x8F51B4D8, 0xB4FB517E, 0xEC7EDBEF, 0x547B9313, 0x14DF5065, 0xF42927E8, 0xBF48B4F7,
    0xF93BAB12, 0x7D310C63, 0x8A7DB89E, 0x13FC403C, 0xC0A09038, 0x260B077C, 0x618FE814,
    0xC0E16E41, 0xEC9603FD, 0x177D4158, 0x78146615, 0x82469F51, 0xE3FE40FB, 0xC0E3D237,
    0x0A4AF983, 0x1FEFC91C, 0xFF297DC8, 0x50498471, 0xFF974A66, 0x7983FA18, 0x914AE612,
    0x4CF25324, 0xC6263399, 0x1E893294, 0xA6F25A35, 0x8708A0CF, 0xB4A9BC36, 0x4B8DCCE3,
    0x5A05363D, 0x9F496852, 0xB4D64B53, 0xC86B362A, 0xCE495EA4, 0xDFEF47AB, 0x5ABC5F2C,
    0xF737EC92, 0x28A7CB27, 0xAB24EAA3, 0xAA436911, 0x7BB9D71A, 0x31174FF7, 0xD9ECFF61,
    0x6A13B7BB, 0xAD60A249, 0x94E9F40E, 0xE08F636B, 0x989391CA, 0xB9BFD90C, 0xFC62F8FD,
    0xDEE891AC, 0x56ED5A8C, 0x34725BC6, 0x271187C1, 0xF189A456, 0x58D2C26C, 0xBC66A235,
    0x90D7E3DA, 0xBC176B7F, 0xC869955D, 0x927BAAD6, 0xABB1ECA4, 0x15A6D1C7, 0xBB6E3CCA,
    0xBA84F421, 0x0D1CC2EF, 0xE0C77439, 0xC4C38704, 0xEFA53C8E, 0x2AFDC097, 0x38D36F0C,
    0xE524F2DD, 0xF6D79BD1, 0xA0F6CC6D, 0xAE6C6B97, 0x8CEE39F1, 0xAF53DDD2, 0x0A06FF3A,
    0x0CC21DCC, 0xC49EA7FB, 0x2E8ED376, 0x6C9B22EE, 0xD32CE62B, 0x26CD3308, 0x81BBECEA,
    0x2FE9D2FA, 0x2FCA5EF6, 0x0A32FDA3, 0x64CDB5EE, 0x8F4090E3, 0x3C682BAC, 0xC8D3790A,
    0xFC5A983C, 0xCD17456F, 0xE4C60F4A, 0x21511258, 0xEBD40512, 0xD3BCD9C6, 0x42F13BB1,
    0x202131F2, 0xC209D3A9, 0x928486E8, 0x9C42B2B8, 0x330ABF50, 0xB1C3F19A, 0xA6E814A2,
    0x49F2130D, 0x82DCB62D, 0xE8B91F6B, 0x2620B252, 0xCA701B85, 0xE8B4C123, 0x2B06C0C6,
    0x16C4B0EC, 0x96AF2ECB, 0xA8B2937E, 0x47C2648E, 0xC5047FD0, 0x6543CFE7, 0x445147BA,
    0x1BA9BA31, 0xE7F9C271, 0x218C6417, 0x12995074, 0x4C27945D, 0xD149E231, 0x140538C5,
    0x352D42A3, 0x3F43D434, 0x8ED45465, 0x1B29F278, 0xF491BA6F, 0x4A6F4A98, 0x648C5375,
    0x53D174F2, 0x54941D03, 0x154A315E, 0xFD488ED8, 0x3B63D715, 0x53A115B2, 0xD9150575,
    0x75BD4943, 0x7D775455, 0x8D4357B6, 0x6753369D, 0x2114EDB5, 0x560DAD61, 0x6D9F65D3,
    0x7D685556, 0x64DC34BD, 0x5596EDAB, 0x573DB284, 0xFD989459, 0x8B73DDB6, 0x7FDDD755,
    0x5F37DDC0, 0x77E5D753, 0xB5CB7A5C, 0x077C5B97, 0x9E5CF695, 0xB06779DA, 0x90641532,
    0xED4F78A7, 0xCF836177, 0x851FF6DD, 0xA658C573, 0x4751CE9B, 0x49767F9D, 0xE4791E30,
    0x8AA5F3E1, 0x2517ADEF, 0x886B698A, 0xFA42C7A8, 0x759BE652, 0x9C6669E2, 0x9C86F9FF,
    0xCCEB3671, 0x46739DA7, 0x479E6868, 0xF0E799D3, 0xE889D66C, 0x9A7A118D, 0xDA75A3A8,
    0x506F9C06, 0x9E21A9C4, 0xE93A7B46, 0x537697A8, 0xE69DAD65, 0xD1B2E959, 0xB243BB1E,
    0x2E9AE6D9, 0x9B3AE9AF, 0x9EA8F0ED, 0xD3B06B7B, 0xA6A5F9A6, 0x251A2EED, 0x49F21BA9,
    0x255BA7EE, 0x06D06E09, 0x1CE3F9E7, 0xEFFBBEF3, 0x69F275B0, 0xEA11C26D, 0xF1B8E91A,
    0xBE50FBEE, 0x71BC7F29, 0xDC9A4327, 0xBEDFCAEC, 0x9FCCF11C, 0xA7EEA996, 0x9E678A6F,
    0x07CE2194, 0xF70119A3, 0x49A9F35B, 0xC2ED3C4A, 0x6A1C3A59, 0xA756D9CF, 0x3C10769D,
    0xEA7A9E8C, 0xE571DDB9, 0x74492723, 0xD1EAD10F, 0x54B111CA, 0x3DA0C2E8, 0x0021300B,
    0x6C512B41, 0x306AEB10, 0x39FEE9A5, 0x8B393631, 0x43C9BE84, 0x209357C1, 0x32D20936,
    0x6FD598AC, 0x4F37C853, 0xE8FC4FBB, 0x3593D7C3, 0xA2A46ABA, 0x3C0F908E, 0x954E2BDF,
    0xF1A3B352, 0x2C4FA83D, 0x0C691FA2, 0x91FEA548, 0xDA4F7F87, 0x77BD03DE, 0x84BD243F,
    0x49F10A80, 0x3103E67C, 0x7DCFE87D, 0x425EA926, 0x3F3F83FA, 0x11217075, 0x8A81882C,
    0xEC7B3117, 0x3C0C39CF, 0xA324644F, 0xE8FDE99C, 0x5EB90584, 0xBDD7B286, 0x7ECF6A15,
    0x04E03BEC, 0x213FF815, 0x147CC40A, 0x32DB1F41, 0x96094468, 0x8C3D20D0, 0xF82C491F,
    0xE7AD38C4, 0x935C2C88, 0xBD287315, 0x46295E04, 0x8D214919, 0x02C44344, 0x3B7DB06A,
    0x0518125C, 0xC2069DB2, 0x946B48C9, 0xE4926E62, 0xD1962FC6, 0xA39C5883, 0x4107D28D,
    0x8D517636, 0x0809647D, 0x119703E9, 0x7A4D3072, 0x831C1F7C, 0x46404778, 0x822D8A32,
    0x48649DA1, 0x50D8FE43, 0x09BB9A90, 0xBC0746E7, 0x680CF9A7, 0xF2DD5C40, 0xAF93A94E,
    0x8D715289, 0x74E69C62, 0x59BAB6C7, 0x23E76B0D, 0xE340DB8A, 0xED3258BB, 0xA2839D95,
    0x1366C721, 0x25A9060C, 0x179B1A5D, 0x09490269, 0x903FF18D, 0xF7318EDC, 0x1625C691,
    0xBC2A9A31, 0x19991E4E, 0x5272671D, 0x628C8198, 0x44883735, 0x8FD328BE, 0x2E1F5C37,
    0xE64AB735, 0x6C952A1D, 0x12EBDD6B, 0x95D9B1B9, 0x0CBB9D73, 0x1750EE5C, 0x32E9F0CA,
    0x1E113A9F, 0xBC7930F1, 0x77EED5E4, 0x03DC94EB, 0x7FAA73C3, 0x075C4C65, 0x1E4BDC5B,
    0x5549A915, 0x5AEC660A, 0x85BCC70C, 0x34298B12, 0xD7DB005D, 0x12860B46, 0x564AC157,
    0xBCC90AB4, 0x96D1A62E, 0xC3A16B3D, 0x4862D812, 0x09E4BC85, 0xA0F31FF4, 0x91E65B07,
    0x94D07FD3, 0x87FBC8A7, 0x537A743C, 0xA674F4B6, 0x4EA9FD37, 0x9F53BA65, 0xF3A81526,
    0x7D50E9B1, 0xCEA3D44A, 0xCD34AA47, 0x1A8353A9, 0xAF26AD55, 0xE99D4AA9, 0xD46A7756,
    0xA69526AE, 0x51AA0D4C, 0xACD62AAB, 0x4979357E, 0xD14D6ABD, 0x769A560A, 0x7D3EAAB5,
    0x0A914D6A, 0x3552B356, 0x2B0D6CAE, 0x57AAC65B, 0xAEF4EABB, 0xCA2AE575, 0xAB589ABC,
    0x52ADF51A, 0x5D4A1525, 0x128652F6, 0xDAE4612B, 0x161CC317, 0x70CD05CF, 0xD792FA62,
    0x2E65A74A, 0xA4364297, 0x3FE87D92, 0x92B23642, 0x2964E61D, 0x1CCB09BB, 0xF28ADA5A,
    0xE3D49C48, 0xC87E4263, 0xD05E5F99, 0x26B3F39B, 0xC9A8108E, 0x2206BA28, 0x0E122359,
    0x6C25E803, 0xE234083A, 0x08394728, 0x90608C33, 0x18BA412C, 0x3BAC61C6, 0x376E4C5D,
    0x6E90B941, 0xDD8BB90C, 0xB5777EEE, 0x77AF2C31, 0xF55D5BB5, 0xE830F78A, 0xFF7AEF05,
    0x86F75EE1, 0x85F38637, 0x5BDD7D2F, 0xF79E4C5E, 0x6FC5F4BB, 0xDFABE17F, 0xBDF75EFE,
    0x802F8DEC, 0xF25FEC0D, 0xE9BC7806, 0xF37C304D, 0x22EC60CB, 0xA5F5C0B8, 0x3C2782B0,
    0xE2E2F2DD, 0x19F8E0BB, 0xDBF4B925, 0x3DF2BC8F, 0x3C53192C, 0xD192C4CF, 0x7BA0B150,
    0xE2A1EE4A, 0xD08CDC74, 0x12788629, 0x58B3312E, 0xF0F2DAE2, 0x0CCC3EB6, 0xBDC8BCF6,
    0x45F8BE4E, 0x43AE6464, 0xE3BE7B8A, 0xF985D16E, 0x4102EE0B, 0x4D58AAF3, 0x21811FF4,
    0xB1D83A5E, 0x63A07F8E, 0x72B3231D, 0xBC41DD95, 0x1D2C872F, 0x3B1E10FF, 0x30E62CC0,
    0x681DB99B, 0xE19A7326, 0x8F2E65FC, 0xFE68CB0C, 0x6CD79DF2, 0x1994748E, 0x1E266ACF,
    0xE80CB597, 0xCBF9AF38, 0xA133E67A, 0x76657D11, 0x871D59B8, 0xDF9C33F6, 0x1341E75C,
    0x6DA8CCEF, 0x3CE1A4F4, 0x43AF2DE7, 0xB3867DCB, 0x65FD31A1, 0xD3DA6B49, 0xA574BE63,
    0x37E95D3D, 0x9ACD7A97, 0x13A772C6, 0x873DE9FD, 0xDE9CD5BA, 0x0CD797B4, 0x83746BC4,
    0xF264911B, 0xCC882520, 0x3E54FE30, 0x24645CF1, 0x81BF0C59, 0x5E2FF19C, 0x2E4262E4,
    0xDECDA099, 0xC4DC8518, 0x98744AA1, 0x98F637CD, 0x50D5B920, 0xDBB8051D, 0xDB51B523,
    0x8989847B, 0x1944EC92, 0x2C9C3887, 0xFF948D1E, 0x34678C71, 0xB6E023C3, 0x8DEB214D,
    0xB0E0EA6F, 0x76BC8E1F, 0xEE5DFA41, 0xDFFBE912, 0xBF216F05, 0x73A02DFD, 0x0C3D5C13,
    0xF9BD881F, 0x137DF093, 0x78D01904, 0xDD763FC6, 0x36F3616B, 0xF61E319C, 0x62962189,
    0xD95C4B64, 0x1D36042D, 0x19C937EB, 0x8AC6A60B, 0xFF3536A1, 0xD64F9038, 0xE13C242C,
    0x76F7CCC9, 0x3CE145D2, 0x8718DE7C, 0x8C852438, 0xE69CDC8A, 0xCF17892F, 0x9E3635AF,
    0x3C211CDB, 0x8BCED5AF, 0xFE6936AE, 0xF0DF1B43, 0xE52BD046, 0x3D47A393, 0x39AF4EE9,
    0x739E8842, 0x1AED8FB1, 0x3FC6D8DC, 0x658CD18A, 0x285DFD48, 0x3DD1BAF1, 0x0D51F48B,
    0x3D05EDEA, 0x5553CF2D, 0x3DBBA677, 0x4C5F4FEA, 0x1C6EBBDD, 0x1DE6807E, 0x3FC788F1,
    0xA00E31C0, 0x99F06D71, 0xFFCFC6FC, 0xC46E8D91, 0x5740F1EA, 0x070462CC, 0x5DF7A6F6,
    0xFCE77FE7, 0xF4AA32CA, 0xEC7D6FA8, 0x4DFB1F81, 0xB1FA2EF6, 0x05F3FDE7, 0x8C3FFB0D,
    0xFC660CE1, 0xD0DB1AA3, 0x8F55DB06, 0xDDE9432E, 0x2D7FB9B2, 0x0F01DC46, 0xF2F3C478,
    0xDE3D7A87, 0xD0FA7EA1, 0xADF77DCD, 0x49EE3D27, 0x63411BA5, 0xF233ABB2, 0xDF97EFBB,
    0x332FDCF6, 0xBEE2A77C, 0xFDC507D6, 0x6B3290D1, 0xDF3CCE42, 0xDE7DCB83, 0xD3A0EF85,
    0x48493EFA, 0xF0967EF5, 0x1112968D, 0x2FD7247A, 0xDFEA82FF, 0x3F66D9FB, 0xD781B049,
    0xC2ED4ED0, 0x423AF7E7, 0xFFCFF2FF, 0xC4FFEFCE, 0x8B967368, 0x014FE2FB, 0xF459E79D,
    0x701B022F, 0x288FD646, 0xA6184172, 0x813E120B, 0xD241FE13, 0x741FA1FC, 0xE1FEE322,
    0x14614C12, 0x6414A1FE, 0xA1FE15E1, 0x16C17015, 0x86F821FE, 0x47548CEF, 0x02C25902,
    0x10F71019, 0x307068D0, 0x01B0127C, 0xE102EFFB, 0xF07EFC2F, 0xF6234654, 0xCC1ACDE8,
    0x507B076E, 0x95D08EA4, 0x6EFFCFD2, 0xE1E41E02, 0xFA2620F0, 0x8B09CFB2, 0x0FF450D0,
    0x08907CF5, 0x9EF5E259, 0x43467C30, 0xAEE930D2, 0x74FFCC56, 0x700CFB90, 0xD92C7507,
    0x40DBC920, 0xAD9CE38C, 0xC8A214C9, 0x88C8EB6C, 0x2DD4360B, 0x924D7EDC, 0xA0B970F2,
    0x2A48F22C, 0x208F66F6, 0x509CC8E4, 0x49A0C86E, 0x37A7FC7F, 0x37122FFC, 0xA2F63409,
    0x160146F8, 0x2C1FE15C, 0x61421481, 0x208ED700, 0xA61AA19A, 0x01441FE1, 0xEA815A16,
    0x16310EAC, 0x6C76C9C5, 0xC72C8EDB, 0x9EE21100, 0x8C9E8F0D, 0xFE712482, 0x9CDE48E6,
    0xD004E301, 0xE1EE5272, 0x189AF0F0, 0x04B29A69, 0x844D8A3E, 0xB6E48848, 0x498D12C9,
    0x4E692AC8, 0x3040E1DC, 0xD128DA69, 0xDAEDAC8C, 0xAD1AEFB8, 0x2880C7E7, 0x1A2EA6C8,
    0x08E64C45, 0x112B1711, 0x4530FCC9, 0xD6B7A2AE, 0xB1CAC887, 0x10ECA286, 0xB2E00F65,
    0x31C511B1, 0xC5AE369C, 0xB90E47F8, 0x0800D96D, 0x1C01B837, 0x6614C222, 0x41FE1601,
    0xE2E1A219, 0x3D05A14C, 0x61FF0463, 0x1A21A619, 0x521341FE, 0x21FE14C1, 0x15815C14,
    0x701581FE, 0xCA6A16A1, 0x16EC46A7, 0x831E0893, 0xCDC4B7C9, 0x2451DC8E, 0xB8C74C8D,
    0x517D1783, 0x18274AE3, 0x89104E4D, 0x04650EB1, 0x3EA84319, 0x050D6D8E, 0x31FA84CE,
    0x1C71FE8E, 0x5126D1D7, 0x51A91A52, 0x1B8418DA, 0x64340DAB, 0xD15F1731, 0x0E8252DC,
    0xF48071C1, 0x915D16AD, 0x2070F91F, 0x611D489A, 0x1187168C, 0x11529EB8, 0x10DE02E9,
    0x32991E91, 0x102E38E6, 0x79289253, 0xB2B0B68C, 0x16E2F710, 0x2617E184, 0xE14A1512,
    0x1FF06305, 0x3E144124, 0xE1D81FE1, 0xD3440E1D, 0xD01D21CA, 0xE1CC1FE1, 0x14A12ED2,
    0x80E7D054, 0xD2C3242E, 0x2D116327, 0x4F2BC8B5, 0xF1992AEE, 0x17700B27, 0x4D17A19F,
    0x511D1810, 0xE1C98F28, 0x949107C2, 0xB31F18B1, 0xD993308A, 0x2B0EF24C, 0x31A640B3,
    0x1CD1AC86, 0x192CF1B1, 0x225091F1, 0xF52A16F5, 0x1CF525CC, 0xCEB05205, 0x552F42B4,
    0x6EF345A2, 0x13746313, 0xF5CA3A5E, 0x8EB25388, 0x908C65F3, 0x17EDE025, 0x2C98A566,
    0xF38B394A, 0x3973993A, 0x2A5B0B3B, 0x0E6F3B0B, 0xF6C1AC23, 0x981881FE, 0xB3AC1941,
    0x3773CE54, 0x513973B5, 0xB393370A, 0x3BD3B339, 0x0D3B9391, 0x73DB3E05, 0x1D85E339,
    0x4E13135E, 0xE1FE1481, 0x2E818216, 0xE819C18F, 0x215813CD, 0x1841FE15, 0xFE192194,
    0xA1AC1A01, 0x1501FE1A, 0xFE164168, 0x818C17A1, 0x1381FE18, 0xAA14E152, 0x13A8786C,
    0x3E53873D, 0xD6A2EA55, 0x33E939D3, 0x44B38D3D, 0xB13EEA0B, 0x94553733, 0xA5AA0B45,
    0x66ED2230, 0x21B41A8F, 0x454F8C1B, 0x414453D5, 0x14513BB4, 0x40E61A25, 0x7617A166,
    0x6A7C4141, 0x3E347F3B, 0x06B1F3CF, 0xD3853CE6, 0xB48F5347, 0xCACDE23A, 0xAF0F0ACB,
    0x4613B063, 0xF5443491, 0xB2DF29D1, 0xDF52A424, 0xD69891A0, 0xB2494BF2, 0x166B8D0F,
    0xCA6D9265, 0x4DDEE254, 0x1FD1131E, 0xD88D1369, 0x32D688B1, 0x0E0E4B1F, 0xBA215365,
    0xED2A2E86, 0xDE02F67C, 0xF9212192, 0xF0945065, 0x16E1561F, 0x481B123C, 0x127B32CA,
    0x36B32335, 0x1F2D44FD, 0x90F72551, 0x33933525, 0x3F26317F, 0xF3451F13, 0x27519283,
    0xBB51F197, 0x6C5F1CF4, 0x28D1DB4C, 0xF7293361, 0x524B2974, 0x55B1271B, 0x74430C7D,
    0x72D92A6B, 0x5494DB26, 0xB67C94F3, 0xD4D73272, 0x52D1072A, 0xE12C74F9, 0xD55F2CD4,
    0x52B4C91E, 0xF72A156F, 0xEC8F32F1, 0x31C98D54, 0xC4E5EDC9, 0xE3D14E72, 0x31D2BF57,
    0x7526F4CD, 0xB25D1191, 0x1EF53933, 0x411F27C1, 0x69152735, 0x4E55A954, 0x5889F269,
    0xEDA35D73, 0x24736230, 0xB929B1CF, 0x354B34F5, 0x18D4C223, 0x735DADC3, 0x72815D35,
    0xC9757A9A, 0xB55D7328, 0x35894DD5, 0x5AB2CA56, 0x2892758F, 0x026F3415, 0x01A9C94F,
    0xA158D295, 0x715D4EB5, 0x6156275A, 0xC46D75E9, 0xF5B688F5, 0x20B5BB18, 0x735375BE,
    0xF4EF6069, 0xD8D27153, 0xD56335D0, 0x159762B5, 0x5DE20558, 0xFF658933, 0x8D876175,
    0x077086FB, 0x404D10DB, 0xAE82F2A3, 0x9F0FCC22, 0xC70B8FD0, 0x50770C30, 0x39F02A61,
    0xE961F077, 0x8EAEEAA2, 0xFF49D4CA, 0x6F0CB016, 0x27C30C50, 0xED2D7B00, 0x8D030ED6,
    0xF6BAFD56, 0x08108307, 0x1B0A6F37, 0x90856770, 0x6CD07F02, 0xC225F333, 0xC3750176,
    0x6AD0A16B, 0x956D3691, 0x2EBF0DD6, 0x6BF68F6E, 0xC56F56E5, 0x900E6096, 0x0A30BD6E,
    0x09092FB5, 0xF0981FF4, 0xFFD6A4EC, 0xC76856DA, 0xC2430BE6, 0x0B49E34A, 0xAD6FEF41,
    0x16B96D56, 0x71B6E16F, 0xCF68D6C1, 0x4862D170, 0x6CA91B0D, 0xEF0A96EB, 0x8F5B6976,
    0xF58F5B37, 0x1827CEC6, 0x8A0B67F3, 0x0DEF9EE7, 0x50F73725, 0x425EF957, 0xC84F7375,
    0xC137F69C, 0x9772A3CF, 0x6D8FB0F9, 0x6EEEB397, 0x4ECEECA2, 0xF8EF716B, 0xB8EE2F46,
    0xCF79780E, 0xB257836F, 0x5375F79E, 0x977EF937, 0x76CF9AB2, 0xB6EEF3D1, 0x6E977670,
    0xF3D7AB78, 0xA34B17B4, 0x97BB7637, 0x7C174EEE, 0xBF7717B5, 0xA26F3B17, 0xF6EF6B2C,
    0xB0EE50DE, 0x06157A37, 0xF84F80F0, 0x945EEF88, 0x97ACF577, 0xEEB3D17E, 0x037277CC,
    0x2FA6B978, 0x9E8FAA41, 0xB57F0FAE, 0xD0E2E78F, 0x1BD2A359, 0x384E2B75, 0x563124D6,
    0x4D08ED56, 0x22DC2C95, 0x910B5FA3, 0x6351E72C, 0x66DD7325, 0x0E3D2D62, 0x57581F52,
    0x7D660921, 0x0DDB4E95, 0x2A431D82, 0xD9839846, 0x15E582ED, 0x64758F64, 0x1D5F5261,
    0x8C4961C8, 0x3EAE407F, 0xFD278E44, 0x719F4D95, 0x30187A98, 0x6C930E58, 0x69DD668C,
    0x8614BD86, 0x3B82432F, 0x55F956C3, 0x58763F59, 0xB8DD3601, 0xD55382B2, 0x55D58587,
    0x6B4E3841, 0xD8916376, 0x6215954E, 0x311B7331, 0x988F65B8, 0x88D8792A, 0x775ED8C1,
    0xD6191EB5, 0x8A58B164, 0x3725B5EB, 0x0D7D25F3, 0x5C98E78F, 0x4734B653, 0x3353854E,
    0x8AF85D28, 0xE3664B7F, 0xC8952575, 0x8D826766, 0x691BB86F, 0x14D6DBF5, 0x63CDC755,
    0xD13678F5, 0xB2874D31, 0x5498F383, 0x436114DF, 0x18CD5D58, 0x16D84934, 0xDD36D597,
    0x98B59372, 0x3455F760, 0xCB8DB83D, 0x99618A18, 0x91D86CE3, 0x358F929D, 0x2B918775,
    0xC4D6518D, 0xE35CC1EA, 0xB742C542, 0x89335588, 0x5E92D1F3, 0x75591AB3, 0x8B355D92,
    0x3F464A5C, 0x26C6B40A, 0xB6737EA2, 0x474734A4, 0xD47AB274, 0x9BCB6177, 0xA73A2FD7,
    0x4A30FAF3, 0x5A6A40FB, 0xC05CF9A8, 0x8F6F3C29, 0x7C93CD3C, 0xCE634B67, 0x39AD4579,
    0x6359EAE9, 0x473939A6, 0xD3839EEB, 0x9EB3AB9F, 0xD6602B45, 0x0A5D9C39, 0x4499FB49,
    0xD37C97A1, 0x37B6F759, 0xE93A1A60, 0xFF4685E2, 0x947146C1, 0xA22B237C, 0x439F3781,
    0x861E61AF, 0xA1093348, 0xEDA01A21, 0x65A6B639, 0x5EFA199B, 0xA8644640, 0xEB10B58B,
    0x6039F50D, 0x085A7A44, 0xCA08E78A, 0xC3C2749E, 0xDAB7494A, 0x09D36AA9, 0xB4EA0991,
    0x649FBA6E, 0xA21A70C9, 0x6C496CFE, 0xCE28933C, 0x70A5A828, 0xF619469D, 0x587711E6,
    0xF8E479E2, 0xA9A6D38F, 0x8C9D8721, 0xAA0A73FA, 0xA86EA3A6, 0xDA6D2959, 0xDA7C9D6F,
    0x979AAC77, 0x0DAC425E, 0x2903026A, 0xAD0EA675, 0x3A95AA11, 0x87769402, 0x2BA6F5A8,
    0x23874275, 0x421BA969, 0xACFABDAB, 0x5C27DAB7, 0x43EA7607, 0x6C772876, 0xCAA09AD8,
    0x5B13A9DA, 0x21BADFAE, 0x688629FE, 0xDB07B16A, 0x852735AD, 0x8F5F388B, 0xD85F9144,
    0x8992B390, 0x3B85B8B8, 0x360C8A76, 0x8C99491E, 0x4F94E923, 0xB35B1852, 0x95B8D526,
    0xDF2E1B41, 0x95B365F8, 0x5998C31C, 0x1BB59249, 0x588BB3F6, 0xAF82047E, 0x55B4B33D,
    0x290F9059, 0x34D5CF54, 0x5D55D8B3, 0xEE57B3D6, 0x99F5E128, 0x6D5E7975, 0x7B978719,
    0x4C4C8156, 0xE75931C3, 0x5615BAF8, 0x2870FF83, 0x39973B7B, 0x958D8C79, 0x53D901B7,
    0x359598E3, 0xB99389DB, 0xB6F2ABB3, 0xD9B72B95, 0x1BD191FB, 0x2C1B5F5B, 0x78D87BBD,
    0x9B878FD9, 0xB69BD5BB, 0x95545348, 0xBB9995DB, 0x215B9DB6, 0x2FBA5918, 0x1325BED5,
    0xB2CEC7AC, 0x4CFC9ABD, 0xC6DBABA9, 0xAC69F968, 0x2FAEDAAF, 0x9AA9C35C, 0x89DA906D,
    0x4A74896E, 0xC731AB36, 0xC386C5C3, 0x05001A99, 0x3C3F2CB0, 0xAFC797AD, 0xCDC4E955,
    0x677A9E7A, 0xF73A71AB, 0x16735C24, 0x49DBAD90, 0xB15B1C9D, 0xEFAF1C71, 0xFAE7AAA9,
    0xC64251AE, 0x35C88FF1, 0x0797C548, 0xDE3AF5C2, 0xCD70F8FA, 0x9A995390, 0x9DBC83C6,
    0xE3B0BAF2, 0xB6D7B109, 0x6FBC989E, 0x336F1CC1, 0x3B26735C, 0xC76A7CA0, 0xE9B1A69F,
    0x344E611A, 0x2439D9A1, 0xB4CA99B0, 0x9A3349B9, 0x59FA0BA4, 0x46FB5A34, 0xB9CACA6A,
    0xB4AEECF3, 0xBD9BFCF6, 0x13C59DB9, 0xA1F9B99E, 0x53A159AB, 0xC5FD380A, 0x9F7CE147,
    0x4DA47CDF, 0x1D07D13A, 0xD2B9FFA5, 0x0CB3DA25, 0xD3E7A65A, 0x9D1D319B, 0x299F9CF1,
    0x99D79BDD, 0xD13A2DA2, 0x37D35CEF, 0xCB22F7BA, 0x624B35A3, 0x3DD23D3E, 0x5CE19BBD,
    0xA9E63DA0, 0xFFA5AB53, 0x2F70B5A1, 0xD66B2FA6, 0x69BDA59F, 0xB8A8F119, 0xE21BC5B5,
    0x8B51D952, 0xF1D2DD18, 0xBBB94089, 0x5310DB4F, 0xF96DBCFB, 0x583D96C5, 0xEBB75D91,
    0xB571BF75, 0x96F84FB6, 0x63B67735, 0x15247418, 0x93586B97, 0x33977DC9, 0x3345BFD5,
    0xDC326BDB, 0x502D64C5, 0x78E9678C, 0xCA3BE585, 0x96DA04B7, 0xB89A4138, 0xE0BAF3D8,
    0xF1BA7924, 0x38ADD8F5, 0x60BBF3BE, 0xC11CFDD7, 0xB8CF8BBB, 0x945DA528, 0x55AA560F,
    0x785D117B, 0xBD6909BC, 0xA1D89321, 0x1C116299, 0x0F5DC5E1, 0xEF10B2DB, 0x7BC953BB,
    0xDDA82BE4, 0x4DC4B5C1, 0x1C05E33E, 0xC0A3EDB9, 0x9B939E17, 0x1999E41B, 0xD8B667BA,
    0x46E0BE2D, 0xB823B623, 0x8BDDB792, 0x44D83933, 0xDD9D4F33, 0xDE1DC593, 0xA31F32C1,
    0x1B5D613A, 0xDD3DA192, 0xEBDC2825, 0xBE7C3418, 0xE85DC5DC, 0x9DE5CE55, 0x582DE038,
    0xE8BB8BEA, 0xFB87319B, 0x797FE578, 0x2D3DE58E, 0x2CE43DE8, 0x2DEBEAFC, 0x4FDEB5DF,
    0xEB2FE232, 0x115BDFC4, 0xDCF865E0, 0x737D57ED, 0xCB977D8F, 0x52737C1B, 0xC76C6E86,
    0x5A1D8066, 0xEEB47D80, 0x6CF4BEFC, 0xB779DA9C, 0xF1901D0B, 0xEBF117EB, 0xCEEBF093,
    0xFA114178, 0x0D7B57D0, 0xCF72F4DF, 0x7D3817F0, 0xA37C37DD, 0x9761F3D7, 0x80BF2579,
    0x49F13F39, 0xFF55819F, 0xF35F51F3, 0x2BF617B3, 0x4F91687F, 0xD3F7697C, 0xE31E77DF,
    0xD800F717, 0x77D7977B, 0xF7069F5D, 0x77FA60F7, 0xF65F166C, 0x65F637CB, 0xE7C3F377,
    0x81230F80, 0xC7F43643, 0xAEA379D7, 0x06C2E9E7, 0x41FB0EA7, 0x3FB85483, 0x89DC2892,
    0x69EAD698, 0xB90BFC7C, 0xFB5FBEA0, 0xE9AFB8E1, 0xFFB3FBC2, 0x2894E7FD, 0xA2FEFA90,
    0x1889DED6, 0x091FBE9D, 0xB1FCFFE1, 0x5E2DF336, 0x9FA200C8, 0xFB027FC0, 0xF087CBFD,
    0x0485BF1F, 0xC8541607, 0x86BE6110, 0xD43E1F06, 0xBEDFF187, 0x3E3B148D, 0xA3D207C4,
    0xF51881C3, 0xB2292BFD, 0xF89040E2, 0xB8DC7652, 0x8B4260EF, 0xD44A0D23, 0xC1A691C8,
    0x4E64FC85, 0x2511D9D4, 0x431FA04A, 0xF3081D12, 0x36A3C664, 0xF974F213, 0xA55326F4,
    0xECD68726, 0xC5E5D2C8, 0x1A0D568C, 0x9FECD673, 0x20DB6E35, 0x340A7CF6, 0x5BABCCEA,
    0x9A9D264D, 0x78BC9E14, 0xE2F966FF, 0xC25F0FF5, 0x665D5DA4, 0x2A51CB85, 0xDF3781DB,
    0xF3FB555E, 0xEC763A6A, 0x2622E9FE, 0x7A3CDFEC, 0x34AEB4F4, 0xDF20D83A, 0x79FF57A5,
    0xF3DA0CFE, 0x4BA1A7D3, 0x1A5D34FB, 0xFF72395C, 0xD6432580, 0xD45F7511, 0x45536AFE,
    0x69293A1D, 0xF26F9EDC, 0x3410FCF6, 0x5AE070B7, 0x1A163B27, 0xB87F6D65, 0x8DFEDF97,
    0xDE6EE633, 0xEB79B30F, 0xB32BEFF6, 0x42930FD0, 0xB69B6E57, 0x0DD0946D, 0xFBEE841B,
    0xE88668F7, 0x0B6F4213, 0xD37076FD, 0xD6FFC9CE, 0x7A77FECD, 0x7E3948EA, 0x789CE8CF,
    0x1BC7D9CC, 0x37B53D7E, 0x794F7411, 0xCF31D9F7, 0x35CFF338, 0x51D370DD, 0x978DF480,
    0x0D397951, 0xF24320A4, 0x5D4F33D0, 0xE1B49D77, 0xB6F1F94E, 0xFDF06E1F, 0xA57CD267,
    0x69A197D5, 0x8EE3B8EB, 0x53FD9063, 0xC4F63D8F, 0xDD6A2297, 0x695F17F9, 0x9FA81DEB,
    0xC6A9BF7B, 0xB9B06B9A, 0xE7671C98, 0x8C60385D, 0x1D347205, 0xC69A3D6E, 0xB9C570E4,
    0xE0681C86, 0x73E02731, 0xA27951F7, 0x8759038A, 0xF5DC769D, 0xFF8B1317, 0x8622F858,
    0x13186926, 0x295DA893, 0xDE554324, 0x067F2469, 0x6CDB8811, 0xA5398A50, 0x99F26574,
    0xB63F6FE0, 0xCD97DBF9, 0x819F19C9, 0xDB89F63C, 0xE67A4082, 0x52128420, 0xA19C10F8,
    0x98E738C1, 0x9EA87275, 0x888A2087, 0x9A253E97, 0x7EA26935, 0x93A81A12, 0x6A19468F,
    0xF8D63369, 0xEE396458, 0x8B7C6088, 0x90DA6906, 0xA487C6AD, 0x5A924B71, 0xD271A09A,
    0x4499291A, 0x8FA948A2, 0xA5A3FEC8, 0xE67E5D71, 0xA2809868, 0x677BE926, 0x339D975E,
    0xABF681DB, 0x06829E9B, 0x3EA3B36A, 0x95ABAD09, 0x69A879EA, 0xA7F9AE78, 0x38B6B194,
    0xD290BA2D, 0xDEAF6AC9, 0xBDE9766A, 0xE837866F, 0x5B75A6B6, 0x59A8BBA4, 0x0FA628CA,
    0x85613846, 0x2B0B2F04, 0xD876ABB7, 0xDA2187D3, 0x7BAAEA95, 0x962D1BCA, 0x9B2BD924,
    0xFC92DAAC, 0xEA36ADD9, 0xFFB0E3AA, 0x42656746, 0x5594DD37, 0x9CCB3356, 0x9F3A3DD0,
    0xDD471856, 0x3FCE1388, 0xCC6320BF, 0x73DF3A3F, 0xE7474DDB, 0x13CDD215, 0xD3B3BD29,
    0x34DCD5A7, 0x1CE74FCE, 0x8753D5B5, 0x429A14F4, 0x3EF4531F, 0xB52D2D19, 0x4D6B64D4,
    0xA3364873, 0xD64657BD, 0x3FCDA36C, 0x0CA330C4, 0x7DAD09CF, 0x6746DE75, 0x7CD9B7AD,
    0xD5113DDB, 0x358D952D, 0xAE0784D2, 0x177DD576, 0x2DD6F80E, 0x44F8BD9D, 0x38DDE34B,
    0xDE0B8DE4, 0xB386D438, 0x944B788D, 0xE479BDFB, 0x352E59A7, 0x1DEF87E6, 0xB789E5FA,
    0xCD315284, 0x368D476C, 0xF9CD24D9, 0xBE977FE8, 0xB75AE67A, 0xB38D4EF3, 0x8E0BB2F4,
    0xB8E3D4F5, 0xAEDF92EE, 0x6BA45675, 0x95E2B93D, 0x39D28DFF, 0xCFF3A4EA, 0x45CA372E,
    0x1BA04A17, 0xCEE7B4CF, 0xB09A0BCF, 0x9AB91EFE, 0xA699B1B1, 0x8AFBA7EC, 0xBEC26FDA,
    0x972D5B4E, 0xEE749A4D, 0xFBF2E967, 0x81D56971, 0x6CB926AB, 0x82AD97C6, 0xE634ACA1,
    0xCEA95971, 0x80D2AF12, 0x6A2B33EF, 0x5804C994, 0xAC20DC2B, 0x5803EF5D, 0x3F5604C3,
    0x7D8FDCCF, 0x02CD0BF9, 0x0610C345, 0x11534C39, 0xD4C4D4EB, 0xD97E2CC7, 0xDAA65829,
    0xB55488D8, 0xF051F6AE, 0xD64B0665, 0xC3562903, 0x572CA87F, 0xF8D96A38, 0x6042255E,
    0xCF811081, 0x4CC2998A, 0x9D252C53, 0x62F886CF, 0x762B0A17, 0xF0196730, 0x8E21C255,
    0x3037E6B5, 0x5D504130, 0x41933432, 0x063FD6BC, 0x275CCFFE, 0xD90A57D4, 0x860542B7,
    0x9C444F2C, 0xC8A117A2, 0x313E25C4, 0xA099A087, 0x33082B11, 0x87116A32, 0x148DCC03,
    0x9EAEF87F, 0x10533104, 0x3240D4E3, 0xFA384598, 0xFB188631, 0x4550C687, 0x107618A2,
    0x4A0C1F34, 0xC374BD1F, 0x8771F639, 0x1B95DA37, 0x214165A5, 0xEE7C4686, 0x5F124E83,
    0x3592C893, 0xAC8A5671, 0xA94B1FE3, 0x5159F828, 0x5FA4987E, 0xBEB7ED19, 0x94117542,
    0xB8A300FF, 0x074795E8, 0x6F8F72DA, 0x17220E84, 0x22514096, 0x6B2F88B3, 0xF8D2B026,
    0x724E2144, 0x120CCE50, 0x4728CD34, 0x6187C544, 0xA6E421A1, 0x6C4C4624, 0x0533F1D2,
    0x3923E46C, 0x3C861276, 0x59BD9A64, 0xE9C73023, 0x7389F499, 0x595A3493, 0x4B05790F,
    0x7C2C0620, 0x50A36AF0, 0xA7AC2232, 0x0F584564, 0xC438C6F9, 0x20643421, 0xEAC22507,
    0x98DAA199, 0x34049B94, 0x68610922, 0xD035593F, 0x21F3728B, 0x3786F8DA, 0x87B45A12,
    0xDC559438, 0x2946A894, 0xB68B113A, 0x88D85851, 0xC196EB98, 0xE53A88AA, 0x29193DA4,
    0x8F42065A, 0xA5A3FDE1, 0xB806C214, 0x6F53DA7C, 0x701AA850, 0x854EE42B, 0x0EA0D47A,
    0x4D40A8B5, 0x52C753AA, 0xB53AA816, 0x6A254DA9, 0xD48AA8DC, 0xAAD37EA6, 0x522ACD53,
    0xAC4DFAB9, 0x5C6234F6, 0xC953EB15, 0x9AB41622, 0xF54CAAB5, 0x5A8D52AA, 0x153AB756,
    0xEB3311AC, 0x56FADB56, 0x1B47629F, 0x3FC6F8E0, 0x75B3B1EC, 0x2BA3345E, 0x66F68865,
    0x84AF93DA, 0x14DCAA45, 0x32274C96, 0x45EE9AD1, 0xC8E58E8A, 0x1741E23B, 0x2423DE74,
    0x6449319B, 0x8BD247BB, 0x6DB4363E, 0x0368A9B4, 0x2C8ED883, 0x9C3C31A6, 0xDB04654C,
    0x1692C246, 0x28852666, 0xDA329247, 0x2396E291, 0x43EDD959, 0x838DE1FF, 0xB5391426,
    0x50565985, 0x72E35BE2, 0x8DCCB6F6, 0x9B92D489, 0x1736DE94, 0xE8E97D66, 0x34568747,
    0x26372E81, 0x76EDE5D0, 0xC9DB9661, 0xFCBA7772, 0xD035C8E0, 0x66EA52FA, 0x95D3B744,
    0x8B7F78EE, 0x83FDE98E, 0xC5E8BF16, 0x4BF18A3F, 0x2C2B86C5, 0x76DD75E9, 0xB0600BF4,
    0x3019B7DC, 0xF458700D, 0xFF01DFEB, 0x1E06C057, 0x0C198170, 0xCEBEFE61, 0x30ADFBC1,
    0x60AC0982, 0xC298470B, 0x76F0BE0D, 0x0161DC12, 0x12C437A7, 0x498630FE, 0x370DE1CC,
    0x1E1BC478, 0x26EA32C6, 0x45C9118B, 0xEF6D311E, 0xB52B9377, 0xAB89A8A7, 0x72AF7DDD,
    0xD3E3D25B, 0x9DBA7720, 0xE165C7FD, 0x1DA5C901, 0x76412276, 0x8239782F, 0xF7272712,
    0xF26919B8, 0x8E829F77, 0x9EC3AC75, 0x884798F2, 0x1BDD19B8, 0x4FC9774F, 0x7732E0BE,
    0xA623B209, 0x444A7CA6, 0x03790064, 0xF3B1984F, 0x613CC634, 0x45838EFC, 0x8ED9C621,
    0xB8E5F4BC, 0x374A18C4, 0xED1D4741, 0x8C2F0C68, 0x45120C41, 0x393D9F90, 0x092EB626,
    0x0FA26CE8, 0x4DB41AD2, 0x13162D10, 0xD2E67B47, 0x35C9EC2C, 0x41E8BD1F, 0xBAD399CD,
    0x743CCD44, 0x00610B8A, 0x25391FE3, 0x3CCF201E, 0xF37D87C8, 0x97E471D3, 0xEA7559A6,
    0x1F73A0CA, 0x9CC81CEA, 0x3841B92B, 0xB596E803, 0xDE3B9196, 0x211D4F61, 0x2283D5ED,
    0xCA2F36F5, 0x092735EC, 0x2794D095, 0xCBA8CF2E, 0x7FD3BB5F, 0xB5F3BC4E, 0x70EC39E2,
    0xE495BB5E, 0xFCABE57C, 0xD7674C0C, 0x21A69532, 0x51766374, 0x441C618C, 0x19866A1F,
    0x407CE9AD, 0xDEB1AB34, 0x703726DC, 0x4FEC490E, 0xBF8A2BE5, 0xF7BA768D, 0xB261E939,
    0xAA1F8732, 0x9D57AA4C, 0x3A764FEB, 0xAA342398, 0x695DC509, 0x62F04B6C, 0x0BE73EF9,
    0x4A4C963D, 0x71D23B6E, 0x15C676B6, 0xE19649C9, 0x0CB18673, 0x6B6732B4, 0x129CD070,
    0xE4CE6E36, 0x404D433C, 0x9AB1DF73, 0x9E259AD4, 0x60C88DD7, 0x2DE0BC6F, 0x7EFF25EF,
    0x83756FD9, 0x71A5DD9A, 0x4DE2F577, 0x33977AA2, 0xB7D4819F, 0xEFFE4707, 0x36FCE17A,
    0xB20780EE, 0xC5949911, 0x43D5B96C, 0xF9C8BAB7, 0x83A5691D, 0xDB27E15B, 0xF1FFC37A,
    0xA3D4C970, 0x0CFF3132, 0xCDE34137, 0xE31C0212, 0xC733DC89, 0x8F14F379, 0x26D7AAA7,
    0xF19A6662, 0x8F9B7258, 0xBF29873B, 0x37F5C4A7, 0x26DF84C6, 0x67FBB9CA, 0x33B6C655,
    0x67ECC3CB, 0x682EC341, 0xCD396F35, 0x5CF26E6B, 0x34E608C9, 0x9725128F, 0x2926F2FE,
    0x017ED7AE, 0x7E859836, 0x1A43B6F5, 0x795F4DEA, 0x6DD723DA, 0xDD5B9D45, 0xD6654CC8,
    0x6EF666DF, 0xD23988E1, 0xE67B0E81, 0x136232B4, 0x6F6EE53E, 0x9F0BD512, 0x9D63DEF2,
    0xFB6F31EE, 0x719EB7EB, 0x2A06097D, 0x520A7133, 0xEE1D83FC, 0x18410768, 0x98768EB2,
    0xB0B8159F, 0xC13C3FC5, 0xB3D8548A, 0xFEA131BC, 0x3A8FB5E7, 0x6D3F0BD2, 0xF63E9FD6,
    0xA37B4C85, 0xC1316437, 0x0B86C83B, 0x476AB0C1, 0x27C25EA6, 0x5A07FA7D, 0xB16197C1,
    0x26643B33, 0x7F87DC7A, 0x377F87CF, 0x01758051, 0x7FC78E3C, 0x997A980E, 0xE8168197,
    0x3D07D17F, 0x30824801, 0x50EA3D23, 0x0F12373D, 0x456340F2, 0x57D98216, 0x79A3C582,
    0x968381CE, 0xC8395D36, 0x1371CE1C, 0x3348983C, 0xA8402AC7, 0x1CE48952, 0xCE8401F0,
    0x68495C91, 0x1B683F1B, 0xDB255840, 0x91C873E0, 0x4897172E, 0x5984F848, 0xA84B1CC8,
    0x2DF85785, 0xCE4BF269, 0xC84C4DC1, 0x1CE8562C, 0xF274F762, 0x012D17A6, 0x86429284,
    0x5C86F1A7, 0xE85D83D8, 0x87687384, 0x0828E2AC, 0x20A51261, 0x0FF0AF0B, 0x9B09F08F,
    0x30CC0FF0, 0x1A10D20D, 0xC50C70BE, 0x709A0FF0, 0x7460A50A, 0xF80F61D8, 0x26E36CB0,
    0x84D84687, 0x6386F109, 0xB85585D8, 0x75A85E77, 0xF11CC896, 0xA77065E0, 0x2A748E6A,
    0xA087586F, 0x94896658, 0x12A48677, 0x58840106, 0x284A8778, 0x10C89945, 0xE86348A2,
    0x15000F94, 0x8A811C42, 0xB689B898, 0x428E14F1, 0x68D8BC84, 0x898B285D, 0xC7E66844,
    0x8BF76443, 0x828B6113, 0xD85D8BE4, 0x7686DA48, 0xD584219E, 0xE72778A6, 0x72C74477,
    0xDE19F454, 0x97414586, 0x6737654B, 0x7F8603FB, 0x87147596, 0x74E70141, 0xBB6F26F0,
    0xB77F7528, 0x8D66F973, 0xE22F6788, 0xF0E60E60, 0x0B10A50F, 0xCA0C187E, 0x60AB1B00,
    0x1A10B40B, 0xC10C30B9, 0x10980FF0, 0x0A40FF7E, 0x946498FF, 0xF78767C2, 0x1B776A32,
    0xA071A8E4, 0x177176F1, 0x44377571, 0xE08AD8EC, 0x669470B4, 0x46C7036F, 0x1D42E8A6,
    0xF7214923, 0x40D49519, 0x6D4AB6C2, 0xE78D1B87, 0x12F91E49, 0xA4241708, 0x91B88DF4,
    0x1F29268D, 0x9874293E, 0x892D7666, 0x8E247275, 0x41701740, 0xB9437049, 0x7496E88E,
    0xEE8E3938, 0x38696858, 0x48C8F36F, 0xF7667754, 0x82F64348, 0x7F088A88, 0xAB8A40A8,
    0x10A40956, 0x0B778E0A, 0xC10B78A5, 0xA0FF0BF0, 0x0FF18C0E, 0x8E0A8099, 0xE57E8528,
    0x8F58DE4A, 0x926664FA, 0xB48A9187, 0x91D8A591, 0xB1713958, 0xC8AE4246, 0x75B49A74,
    0x27692975, 0xD4917829, 0x74595A11, 0x5B931789, 0xE9341A84, 0x79098043, 0x6D31472E,
    0xC8C183B8, 0x86F1591C, 0xE30E40DF, 0x014F3E60, 0x8B15C584, 0xB38C0184, 0xF1492908,
    0x99C99E86, 0xDB1B6843, 0xE45757D0, 0x8B016B83, 0xD09A88C8, 0xA8039AF8, 0x10C8628A,
    0xDB2E153C, 0x413D9BE1, 0x9B68748C, 0xF08688AA, 0xC0F91798, 0x84585D86, 0x711219AD,
    0xD9AA99B8, 0x9AF9D199, 0x9E11C1A3, 0x00AB09D4, 0x0B50AA96, 0x6B969906, 0x20A60989,
    0x0D60CE7E, 0xC30FF0D4, 0x10FF3740, 0x0AC0AE0A, 0xD70D00FF, 0x40FF0D50, 0x0A00A209,
    0x7E9B20FF, 0xF9AC2E95, 0x9D49AE9A, 0xAA6E1878, 0x79B71828, 0x9A952287, 0x218A2489,
    0x048F7712, 0x9FF9CD9D, 0x0E1CE645, 0x756858B2, 0x89EA0453, 0xC69BB870, 0x183A9BD8,
    0x4908B813, 0xBD59E8BB, 0x69FD6998, 0x1889FF9D, 0xA61CCA1B, 0xA14D9B89, 0x8CB8779B,
    0x0D53A7CA, 0xAA00A28A, 0x8152E970, 0xF97D037A, 0xE14F79A7, 0x5C33400D, 0xCE7CE7F7,
    0xEA40A373, 0x83581880, 0x5DA4380D, 0x479E2E03, 0x1375A07F, 0x9A810A43, 0x980EA467,
    0x1655E714, 0x21A525E9, 0x9A53A391, 0x0EA0E614, 0x913D67C9, 0x411A7CB1, 0x7A0A537B,
    0x52835822, 0x8A48A393, 0x11C59082, 0xE50E60E3, 0xB0B00FF0, 0x0FF0B90B, 0x0F6C875D,
    0x00D10CB1, 0x0BE0FF0D, 0xF348E688, 0x65F46340, 0x0B00A35F, 0xCA0FF0AD, 0xB57FA7C0,
    0xA4E7CD33, 0x6C7F8515, 0xA3777FBA, 0xA42817A5, 0x49A44A6D, 0xA77181B1, 0x7FF83280,
    0x2881FA39, 0x354563B1, 0xA95A505A, 0xF8A66835, 0xE4E881B7, 0xA9181449, 0xA180EA68,
    0x0383A96A, 0x39DA93A9, 0xE71491B7, 0x182C0EB0, 0xA1E0FFA6, 0x34826A9E, 0xDA94AB18,
    0x7306FE7F, 0x421A1996, 0x276C7358, 0x98E45C4C, 0x4729E1BC, 0x467A74B9, 0x99791392,
    0x079C188F, 0xC6C59797, 0x7508EF1B, 0xFC75B2AA, 0xF76777C9, 0x93646D6C, 0x4592B8B4,
    0x78D21864, 0x7641CE41, 0xED92C3F8, 0x64898D74, 0x6EB67191, 0xA56A86A6, 0xE0F60F48,
    0x68CAC86A, 0x7A6B2920, 0xC68244E7, 0x14F6B73E, 0x846BB711, 0x06F894C7, 0x4AA8E5AD,
    0x93B0F985, 0x06C96C79, 0x44F78F58, 0xF1959916, 0xC93CB0CA, 0x93E6D466, 0xD577D726,
    0xC4DA73AA, 0x76BAD273, 0x2278BB10, 0x767D976B, 0x9378EA74, 0x7C951ADB, 0x48F1ADE7,
    0x6F77656F, 0x7793A495, 0x899046F9, 0xAF0927B2, 0x786FB786, 0x2A01AF54, 0x97D1A893,
    0x16773710, 0x6982AFFB, 0xB4589D71, 0xFF93E468, 0xD43098BA, 0x92EB1A98, 0x4B987323,
    0xE94A4D07, 0x72B99419, 0x184BCB3A, 0xCACF713B, 0x7346D3B1, 0x0266F701, 0x3B4AB3F3,
    0x25B4556E, 0xC0AD373F, 0x8B0B8DD2, 0x94FAD98E, 0xBFAF7B30, 0x568AB331, 0xB386B776,
    0x29AFE6D0, 0x7B27B59B, 0xB64B736B, 0x83B43261, 0x71A6679B, 0x7713C46A, 0x3BAFDAFB,
    0x13016B09, 0xB796F792, 0x0CB064BA, 0xC223B081, 0x6BE69E6B, 0x5C945B56, 0x4400B31B,
    0x43C6C84F, 0x27B6C6CA, 0xF6416CE7, 0x11C6445C, 0x3C5C934B, 0x4343A3EA, 0x63F5E25C,
    0xF65C91A7, 0xCAB46567, 0x131BAE64, 0xA7BBEBB0, 0xC1D8A4A1, 0xAC55DEA4, 0x407B8BB9,
    0x1394BC16, 0x5E8A5564, 0x52645618, 0x3BC76366, 0x3D43D25D, 0xC4239ABD, 0x13CE6533,
    0x63EBC064, 0xDFBBD5E1, 0x1A8DBAFB, 0xBBB81FBC, 0xC9663BB8, 0xC6618ABB, 0xBE3BEBBB,
    0xC4BAC348, 0x0131BEEA, 0x126BC881, 0x5D131BD2, 0x1A9C6487, 0xBDD6480F, 0xCEBE2BE4,
    0x3149BF25, 0x645371AA, 0xE75A0BF6, 0x863E5ECB, 0x490A7812, 0x0BAC2580, 0xBC007B2C,
    0xBCBBE68A, 0xF7BE9BDC, 0x9131662B, 0x43C82C65, 0xDEBC1ABF, 0x3C03C15B, 0x79A1535D,
    0x9917F153, 0x29A00FF1, 0x4053F29A, 0x50BE1A06, 0xF1965281, 0x14017D89, 0x2F8AFC29,
    0xD9F9489B, 0x11D50A1A, 0x331695C5, 0x37B4C26C, 0x9C014F15, 0x362BF5E9, 0x7C40157C,
    0x954C1D15, 0x4034859B, 0x8C289BC1, 0x1F0C3919, 0x38C39C4A, 0x35A312EC, 0xC4152315,
    0x56189592, 0x293FB58C, 0xC5A106C6, 0x5D645140, 0x7C5F5A9C, 0x6331401A, 0x82ABE961,
    0x35A0C34A, 0x10F7C2C4, 0x1758C5FA, 0xEA03C6C5, 0xC2AC5312, 0x90173171, 0x8C3117A4,
    0xC55158C5, 0x615A8C5B, 0x7C5E12EC, 0x18AC4218, 0x945AD96E, 0x7C67C731, 0x19DC6915,
    0xF3BA08F6, 0xAB0393DA, 0xB0E940B5, 0x22B8794B, 0x38536DD3, 0x6D9B4FBA, 0x61B75B1A,
    0xDB898EB4, 0x95396CB3, 0x368F29CA, 0x3944B51B, 0xB24B39AF, 0x2092498F, 0x1988BA2B,
    0xCACAD7CA, 0x9270C97B, 0x6B4991CC, 0x20D77793, 0xC0984B01, 0x3989B2AC, 0x49042D4A,
    0xA95B0B54, 0xAB8A496B, 0xB5DC9FCB, 0x3543C4F5, 0x0CABB819, 0xC998D595, 0xC7B1D2C4,
    0xBB27C9C2, 0x48BCD6B5, 0xA3B57B0B, 0x76DFB6FC, 0xCE694991, 0x5FCA8CBC, 0xD769B768,
    0x48874BCD, 0x42CB1B7B, 0x6945B379, 0xCB8914CB, 0x218DBCD4, 0x7AD6B8AB, 0x2E5AF375,
    0x6ECCAACD, 0x497ECC27, 0x635CDBCC, 0x4EB95417, 0x3B50992B, 0x319CCDCE, 0xD04E82A7,
    0x5B40B9FC, 0xD008F8D0, 0x5B47C991, 0x7CD2B5E7, 0xCFD4CAD0, 0xE14A6B41, 0x9CD5CCCC,
    0xCDFCA091, 0x48CA4B25, 0x5CBB77C9, 0xCA2B2B6E, 0x3ACAAAD8, 0x81B847AC, 0xCFAB35CF,
    0xE5AF2CB3, 0xF945CE8A, 0xCE530292, 0x36739D2D, 0xF745D28D, 0xD48CEECB, 0x0C70E8A6,
    0x6D0E774D, 0x6B3715CC, 0x5CD21D12, 0x87816F74, 0xAE971FD1, 0x6EC63D24, 0x8728D472,
    0x99549DCD, 0xCED37B92, 0x53A63A76, 0xC313CB3B, 0x3A52079C, 0x738C5EBA, 0xD6B7A33A,
    0x6E3A73B9, 0xFBC338DD, 0xBEF36135, 0xB5D763DC, 0xE3A2D6C3, 0x37136FA8, 0xED3AA7BE,
    0x1BAD3DBB, 0x7C97C713, 0x737CCC91, 0x4D75AB7D, 0xD78D82D7, 0xA23C8D6D, 0x2D81D7A3,
    0xD89D9C3A, 0x99D9AA8F, 0x2D79D9FD, 0xBB6D8BDA, 0x46808A9B, 0xC5B4BFD6, 0xBF2DA53C,
    0x12609810, 0x5D9D3B48, 0x39F3A2D9, 0xEF81A3B5, 0x13BF6C80, 0x3C43C33C, 0xA9DA481F,
    0xB3DCDC13, 0x3A9DADD9, 0xBC0EAAB9, 0xC83082EA, 0xDC537ADA, 0xB5DB737A, 0x2D68B80C,
    0x8DA3E66D, 0xCE986D20, 0xE745D2FA, 0xD32457B6, 0x23D03923, 0x494ED37B, 0xCF6B78CF,
    0x506EF952, 0xB94BCF97, 0xAE6DD2CF, 0x0FD45D0E, 0xBB86D63B, 0xBA4978DD, 0x46D59ACC,
    0x3B48D0BB, 0xD16DD4D5, 0x22983B00, 0x5CB2CA69, 0xD5DB52D1, 0x1CB5592A, 0xF95BAE4D,
    0xBA2CCCD1, 0x25936D23, 0xDD274A2D, 0xB66D29D1, 0x34D01D2B, 0xBDE269BD, 0x19E8E0D4,
    0x33CEDB26, 0x4D35CEFD, 0xD39CF2B7, 0xF7B0FE12, 0xEDEACAFC, 0xD40DECD3, 0x0AB19B1B,
    0x7D1ECB4E, 0xE23E0EDF, 0x15D4AE1B, 0x4CF5D4CE, 0x97CD4EB8, 0xFD772DFB, 0x4981B60D,
    0xE02CC8B9, 0x5B4BAD13, 0x0D19CCED, 0xD41E3272, 0xF3E35E0C, 0x62B84F4D, 0xCB7B5FD6,
    0xC772FDF6, 0xA732DFEA, 0xE36ACBD2, 0x4ECF0D4F, 0xDD37E30E, 0xDE74B4DD, 0x24E60E22,
    0x8E268E7E, 0xE35DE574, 0xF2DE9E2A, 0xC9562AA6, 0xC98E0BCC, 0xC6CE9E62, 0x4AC6D2CA,
    0xE58E76D0, 0xC1E3ED08, 0x2DFCD51C, 0x0FFD55CD, 0x03A16D11, 0x6E05D3FE, 0x42FE4B77,
    0xEFCBDB0A, 0x6E19CB94, 0xDF0E10CE, 0xC2927791, 0x0534A3B2, 0x1035045A, 0xB4508506,
    0x650D1B7B, 0xC02595BB, 0x4FE9C590, 0x22C2A1AA, 0x11DEA65B, 0x30D715D6, 0x75A7672C,
    0xA275B3EA, 0x4759F845, 0x9525618C, 0xC31EA45A, 0x4E0FFA15, 0xB580ABDC, 0x5170FD23,
    0x605855A6, 0xA5C9C73C, 0xC68C8AC6, 0x8CC73C89, 0x5A27C39C, 0xC65925C7, 0x50C7FC6D,
    0x81402B26, 0xBC2D0FEC, 0x74597B0A, 0x1A1E7711, 0x12D5DACD, 0x3A66459F, 0xCC796026,
    0x5A35B3AE, 0xAEEDD59F, 0xF649EE1E, 0x583EB85D, 0xEFE9C35C, 0x4EA5EA9E, 0xB9EEDFEB,
    0xF65AB106, 0x8E095AE4, 0xEC9EF851, 0xE5EEEDCD, 0x1BE735AB, 0x5C110CBB, 0xC5C3EEA1,
    0x7AB8BEC5, 0xC01BCAC1, 0xC1F0C38D, 0x151E5D5B, 0xBF3EE6D7, 0x7ABF1BB7, 0x6A4D6413,
    0x645614A5, 0x0DF13C12, 0xDBD613DF, 0xEDE72A82, 0x177C5EAA, 0x465064AF, 0xA4FEB5F3,
    0x3398CF35, 0xAF387F5F, 0xF38A5A34, 0xED59BB54, 0x3F43B54B, 0xF3BF3E36, 0x8CDA0F47,
    0xBD7FF439, 0xAA1F48F4, 0xC1F29F38, 0x35BFF51B, 0x3D8771EF, 0x23F53EDE, 0x1EEA37AC,
    0xF5263C16, 0x30F1FBD3, 0xA128F1FF, 0xAB059A80, 0x63BC6F28, 0xCBE7BEAF, 0xF16BDB9B,
    0x5B6592AC, 0xF65D43C6, 0xF20A2265, 0xABD92F1A, 0x5E78CDE8, 0xB68ACA94, 0xE6B97D01,
    0x14C5DF58, 0xB71D318E, 0x62DF4241, 0xCE6DCA9E, 0xB27DE6CD, 0x72E2B683, 0x8E04362E,
    0x0E70E21B, 0xC99018FC, 0x94BF5F70, 0x37360960, 0x19F9F530, 0xA613F9D6, 0xFA33759E,
    0xC41DB626, 0x2FA56180, 0x609613FA, 0x30FAAFAC, 0xC619FA75, 0xF9EFB3FA, 0xB8FA00CC,
    0x9FA8FADF, 0xFB69EB9E, 0xBDFB1613, 0xCFAEFA6F, 0x0CD613F9, 0x750D30D4, 0xD1A1F87E,
    0x97FC70B8, 0xEDE8DE5B, 0x6E46778E, 0xE8BFCBB9, 0x296A7D42, 0x3E9011A9, 0xB3C4BCF8,
    0x6578C6F7, 0x8E11F2ED, 0xDD3E13E9, 0x4ADD5E43, 0xCE5FDD74, 0xE3AE1CE7, 0xEB73DE1E,
    0x2E48CA5C, 0xE6CF8AF8, 0x7AE28B2E, 0xB954DE89, 0xCCCD49DE, 0xD91000D2, 0x45DBFDB0,
    0x6DBFD86C, 0xD3FDBEE0, 0xC3616DF6, 0x75BCD985, 0x229127FB, 0x330E8644, 0x8DC6BC2E,
    0x189046DB, 0xD6BBFE2D, 0x944623ED, 0x1C7E4B1A, 0x37E1F299, 0x864D246F, 0xDB1188D4,
    0x5D388AC4, 0xB278EC82, 0x3888CFE6, 0x98C4CDB1, 0x6AB3A170, 0x02DDFED4, 0x3F1FEBF6,
    0x4EA4FFA8, 0xEACFEA9D, 0xA9597D3F, 0x8F47956E, 0xC25C3FDE, 0x579BFD80, 0xB456EBEF,
    0x4DADF2FF, 0xBF2A168B, 0x6EE4FB7F, 0x99CCF63D, 0xF4FD8FCC, 0xAFEAB3FE, 0x59ABDF2D,
    0x2AA5FDF5, 0xBD3FDF77, 0x7D4F08FD, 0xBCEBAFFB, 0xF17CB37F, 0xF1373AC2, 0xD58BA8D3,
    0x597CD5B2, 0x5AAC66B0, 0x1BEDB72F, 0x75DB707A, 0xD3FDDCEE, 0xBFD84C45, 0x81ECF67A,
    0xF40F9B5E, 0xDBA0C365, 0xB9AC6E36, 0xE5706836, 0x9603FDC8, 0xBF60190C, 0x4E8F3F6D,
    0xE6B3FA5B, 0xFF17BDDC, 0x2A97BCEE, 0x88E070B7, 0xF5B63B27, 0x81D26973, 0x2B9F42B5,
    0x91E5F09D, 0xDB319CC6, 0xE6AE7B8A, 0xAADB57EF, 0x608EC76B, 0xF6FA83B1, 0x9B9B6ED9,
    0xB0695A39, 0x68192819, 0x40EDD19E, 0x47E1FAAA, 0x5BC10431, 0x20DD41CC, 0x05B6F044,
    0x9D8CD427, 0x27F9DE77, 0x89826117, 0xC4E6B32B, 0xAEAD2D3B, 0xAC6B0ABE, 0x8E9ACEB2,
    0xCB1AC5B3, 0xEEBAAA8D, 0xBDB1095A, 0xB9CAA424, 0xCFF316F1, 0x7AA316BA, 0xB0C9B22C,
    0xF332EA8A, 0x316EA2FE, 0x6C2472CF, 0xC2D11E39, 0x52B44471, 0xD7B5AD5B, 0x37CD9B62,
    0xEBDBCF16, 0xF33EB4C5, 0xC27C2B52, 0x7C4C27FC, 0xB3F3334C, 0x533CD27B, 0x11BADCAB,
    0x7E7F9C27, 0x3198C641, 0xB072FCC5, 0xCCD9EF34, 0xFCF73BB6, 0x0735CF27, 0x3ACC8CFB,
    0x4D13ECCB, 0xEAD50F35, 0xFA46C6B7, 0x733A18E5, 0x3EB7F403, 0xD154037F, 0x142508C1,
    0xB2B4B8D3, 0xFE6D1B66, 0x65198621, 0x536DFCCC, 0x949CFAAD, 0x3372CFAD, 0x58741D27,
    0x7A1FED73, 0x5054EACC, 0x94A4C530, 0xB1A0B9CD, 0x8E7F9AA6, 0xA799A671, 0x5353EAE6,
    0x1655533B, 0x359744CF, 0x9D4BAD95, 0xB8414C6D, 0xBC0C1D76, 0x8ADDA8AB, 0x2D73631F,
    0xC35C5513, 0x7159B3FD, 0xD9353B3F, 0x072B72C1, 0xF2C3589D, 0x9F721ED0, 0x74DA2DB5,
    0x43276AE7, 0xEB1982B0, 0x89E0789D, 0xE9715BAB, 0x8250B40D, 0x19A62B3F, 0x26D1A8F6,
    0xB607791B, 0x7563DE34, 0x7359D565, 0xC357BAD9, 0x05D97A59, 0x04FCD936, 0x2B816077,
    0x67621685, 0xD9D3E557, 0x756CF587, 0x51D473AE, 0xE359A087, 0x7F9E4791, 0x6495530F,
    0xD435CF71, 0xBE7AD2D9, 0x7CA8B78A, 0xDFB4ABF6, 0xB82ADE84, 0xAE3B8AE1, 0x633FA331,
    0x92D2AAFA, 0xAA6A16BC, 0xE82E8AFD, 0xD3F391AA, 0xC48AD2E8, 0xBAE2BB4E, 0xCBCCCC03,
    0x6B5A968C, 0x8B86AFA3, 0x4AF76CEB, 0xDDAD3D6F, 0xBF6D8C3B, 0x68CC5498, 0x05DBECFA,
    0x29F331BF, 0xB0F2ED33, 0x00AA6E4E, 0xAEC64207, 0x7A068CAD, 0x6689ACED, 0xE0CC6E8B,
    0xABE86F8A, 0x9FE76B52, 0x505D434F, 0xE249CDB1, 0x9A7F9B66, 0x7F9A66A1, 0x9FEA499E,
    0x87F9AF00, 0x534B7F5F, 0x8BACED1B, 0xA8E7FC6E, 0xAFD6C372, 0x6759D874, 0x9B2B0EC3,
    0x8DB3EB3B, 0xC81E678E, 0x1E8C744E, 0x9D9720C8, 0xAF23E672, 0xF118EB59, 0xB2B151A2,
    0x726BDF81, 0x9C0EADD8, 0x8EBBE6F1, 0x259A786A, 0xD11F2CA3, 0xFBBA7AF6, 0xDECF96AB,
    0xD69BEC79, 0xAFB8FCEA, 0xCB68CAD7, 0x1E8D7D65, 0xDF59EA7B, 0x7720DB71, 0x69A32E71,
    0xC9B47166, 0xB6905FD2, 0xC7FCE39D, 0x5063E53B, 0xA4A6EA88, 0x4B0EBCBF, 0x935241FC,
    0xEDB5F1FE, 0xD82A6D88, 0xA103CF0B, 0xDDEDC114, 0x51386DC6, 0xB4127B0F, 0x862CF7A5,
    0xDBC96983, 0x3E420E7D, 0x4765A20A, 0x8090040D, 0xBBB871FE, 0x452A3008, 0xB491CB6A,
    0xA4E798A4, 0x70C116E5, 0xA13A234A, 0xE7D07F89, 0x86098D72, 0x8B438E32, 0x1FE2F45F,
    0x2D85C8AD, 0x4E08B1FE, 0xD9210260, 0x57048AD9, 0xC8BFC314, 0xB3B92FED, 0xB43C870F,
    0x4939100F, 0x11297F44, 0x1642C458, 0x34104D5B, 0x8C0C3BFF, 0xEAA33710, 0x1E8D4E0D,
    0xE70CCD04, 0x01E423E8, 0x39FA3289, 0x68D2910C, 0xE5B602A4, 0xE061F06C, 0x17F49E5F,
    0x638FD929, 0x08A2FA57, 0x0B25C315, 0x9A8E500D, 0xA6D2A039, 0x39BB17F7, 0x39E48D72,
    0xE2D30F0C, 0x105A30C3, 0xE05B6183, 0xB898F4D4, 0x8428EA51, 0xDBCC51D5, 0x3115384C,
    0x7F95D232, 0x5EDEE169, 0x16300FA8, 0xA41632C6, 0x886EE14A, 0xC5C21D38, 0xA5416894,
    0x41F7E522, 0x1446CDFC, 0x3FC568B0, 0x528A8134, 0xBE341020, 0xFE314638, 0xE45E0B11,
    0x2CC279E2, 0x987B21E5, 0x32E4B4C8, 0xDDE4642D, 0x9559CD19, 0x8DA5A258, 0x01114BC6,
    0x3A169983, 0x338EE5D2, 0xF928A3C9, 0x264FCA48, 0x119078F3, 0xB93EEF9F, 0x3334ADCC,
    0x4D04CF9D, 0x26683B28, 0x9246A8D4, 0x25A7B256, 0xEA50B9C7, 0x7E8C45ED, 0x0865CCBC,
    0x5D1AD1D5, 0x18E41BE5, 0xD76D3687, 0x6A30C536, 0x69651A48, 0xD202D94A, 0x5791028C,
    0x27E96C85, 0x43535A3F, 0x0A210F72, 0x7ED6A0C9, 0x5E4FD303, 0x6538A7E6, 0xDA6B4728,
    0xA47CA198, 0x4A8D0784, 0xD43A8128, 0x772D6A98, 0x9FE5F3BB, 0x863514C4, 0x7EA42B4E,
    0x5435C689, 0x45F8C387, 0x8D463735, 0x9AA34DEA, 0xA55DADD3, 0xC530FF27, 0x0FF1602C,
    0x1A434C66, 0x2BC518FF, 0xAA2849AA, 0x9BA243B0, 0xFE298548, 0xA4750E61, 0x1D453DA3,
    0x62F759E6, 0xAF9AA0D6, 0x0D2C8DA9, 0x3BB1D63A, 0xAB141A33, 0xD6AA8551, 0x37BC5786,
    0x36B268FE, 0xC5501315, 0x10BABD88, 0x739F1ACB, 0x5A6B679E, 0x6D3AA059, 0xE4BD86A5,
    0x4827C05C, 0xB4D3E53F, 0x502C418A, 0xD3DA831B, 0xACAB45B2, 0x953F53F5, 0xE95AC928,
    0xED1EB36F, 0xB739512C, 0x96A4C1D8, 0x8B628B62, 0x338640B5, 0x4DE379E7, 0x77464271,
    0x419B9508, 0xE564EC49, 0xB2C0D6F2, 0xDEE7265B, 0x1DCB654C, 0xE2E14A8C, 0xEB1539A8,
    0x14E31F30, 0xA8459D7B, 0x372B3AF0, 0xC1DD011D, 0xF7303196, 0xCFBF2B8A, 0x0B60996B,
    0xE87ABF72, 0x75DDBB0F, 0x657C636E, 0x8B0D608D, 0xEE6B287D, 0xACCBB8C4, 0x63D83EF1,
    0x2C8C55C5, 0x7F8BA178, 0xA314C260, 0x6629C7F8, 0x447F8B11, 0xF89E1422, 0x23843847,
    0xF684AB95, 0xC588A51F, 0xE036082B, 0xBBD7694C, 0x8319095A, 0x6E31F36D, 0xB155E61D,
    0xC43DEC8A, 0x7D336304, 0x80EE372C, 0xC78CBF87, 0x7789E7BA, 0xCB65E5C6, 0xD4B2E775,
    0xC92A806A, 0xBA320DE4, 0x4E632BF0, 0x18C8867D, 0x666C920B, 0x3EEDB153, 0x6B0EC237,
    0x062395B2, 0x795F075E, 0x8C3AF567, 0xDFF65F63, 0xC816BC7F, 0xC8B31A88, 0x08666C6E,
    0x9BAA333A, 0x8D274C5C, 0xCCE535A3, 0xCC2A3E4F, 0xC8FC5A73, 0x6848C267, 0xE569DC95,
    0x98488284, 0x96B05743, 0x8DDB8424, 0x504982AE, 0xEE4A3A16, 0xF5555CB8, 0x027241F3,
    0x04C62D3A, 0x5A032D2B, 0x6CF4CD49, 0x5998EF2F, 0x8A414F86, 0x1931347F, 0x360698FF,
    0x6AD75754, 0xC418C2F6, 0x7A2C4AD8, 0xFC596181, 0xAD858FBA, 0x9D2FAA68, 0x36FCB442,
    0xE5D39DB2, 0x3FA2CA5F, 0x7F4FE1E3, 0x6B654E88, 0xE89538EB, 0x849891E1, 0x01A1B506,
    0x34C26838, 0x45059EA6, 0x78154EA3, 0x11EE7BCE, 0xF516DCA2, 0x0FFD3492, 0x4A88690C,
    0x9E9780FF, 0x4C50FDA8, 0xA8934823, 0x3D89F3EE, 0x360DEE79, 0x068FA1F2, 0x72E13961,
    0x656E8C6F, 0xCA941AD1, 0xF04DD50A, 0xD252CCFE, 0x52C8A139, 0x86ED684F, 0x5495FA0A,
    0x60DF25E9, 0xD753F058, 0xECC870D0, 0xC9DF3291, 0xA5A2744C, 0xD0C17B6C, 0x0D11926A,
    0x1C091503, 0x33E928E3, 0x69B5A57E, 0x3C82E408, 0xA9422805, 0xA39550A2, 0x6168DAF0,
    0xEDCB6467, 0xD3CB75CF, 0x51446E2B, 0xEE1DF91C, 0x583B21DA, 0x56CBCCFB, 0x6F710E91,
    0x7F8694D3, 0x0A8FCC69, 0x1EA6484A, 0x4ED5E9FC, 0xBB93A67A, 0x73C7146D, 0x379C912B,
    0x741E1145, 0x2A485B8C, 0xD00C26FB, 0x3FEFE68F, 0xBBE028DB, 0x4FCF766C, 0xC394D09E,
    0x6F0B95F2, 0x2EB08234, 0x90FF8943, 0x38B0FFE2, 0xF19E852E, 0x3EBB276E, 0xE44CAC2E,
    0xA977A79F, 0x50E49A90, 0xEBD8B937, 0xFF4B781E, 0x0208DD93, 0x5E0DE238, 0x7B1D07EC,
    0xDDC95BE8, 0xF68D96E5, 0xD9E5A7C2, 0xFE55A74C, 0x8BF9CF50, 0xFCDB254A, 0x2DA07D84,
    0x6FCC6A1F, 0x0EE32A2B, 0xFAD1D1D2, 0xDD97C176, 0x3B36EDF7, 0xEF21DD24, 0xDEC6C959,
    0xD4B7C763, 0xDFA9C2A9, 0xD06037C4, 0xE1748C6D, 0xFEEB2724, 0xD3046018, 0xACD7957E,
    0xAA713F66, 0x6DD339C6, 0xA74BD054, 0xCA8C171C, 0xF9325DDF, 0xF9A7DCD3, 0xE8BED692,
    0xD99E2F6F, 0x82FE8FFC, 0xB0130019, 0x34AF83F1, 0x406B3033, 0x96216130, 0xF4B3B0C6,
    0x6BF5B2A3, 0xBFA2FD98, 0xFEBFA3F2, 0x5330B343, 0xB32BEC32, 0x1DBF8BF3, 0x2AAB276B,
    0xAEEB3993, 0x220C3995, 0x4237B233, 0xB08411B2, 0xE799D3FA, 0x5B293281, 0x32A3FE41,
    0x2A40CC03, 0xF2D832DB, 0x3F5186B2, 0xF93302E8, 0xAC1AB093, 0x0B9B213F, 0x38C01337,
    0x919A079B, 0xC1F408C1, 0xE7AD9AFD, 0x2ACE2B21, 0x8D2AA013, 0x56292A91, 0x80DFC282,
    0x297ADBC2, 0x281332E3, 0xEC292E24, 0x0BF293C2, 0xEA8E20A3, 0x30BD8BE2, 0xAE3AC4AF,
    0xDE42E16A, 0x0C35C12A, 0xAA487FBB, 0xB43FD87F, 0xB2DCAA72, 0xA4FADD8D, 0x1F3CA2A9,
    0xFAE513D3, 0xAAEA7EBF, 0xE19C42AF, 0x3C2FBC7A, 0xC38B0A2C, 0xCA2DC0DC, 0x8C2D0B62,
    0x24FC470A, 0xDAC27438, 0x5B2029AA, 0xC228C539, 0xB39F22C8, 0xB42DAA8B, 0x8782A00D,
    0xD79E3356, 0x0445C4B2, 0x0BDC4CAA, 0xD22D10F5, 0x92DB201A, 0xC38C3744, 0xB9C3144A,
    0x8078C520, 0x3F687C2D, 0xDA4448B8, 0x9C4C454A, 0x2A14498B, 0xB4362448, 0x9A16C49A,
    0xC5427AB7, 0x55ADE430, 0x7392B3DC, 0x0AA3E41E, 0xD8204B3F, 0x52072873, 0x4693ACBE,
    0xEBB84BEA, 0xBA5622E0, 0xBABB8D3E, 0x9F350B77, 0xAB49BC73, 0xC7434AB8, 0xE0BE6BDC,
    0x4A5FB8EB, 0x3D29E7B7, 0x52A1B1D4, 0x9473C68B, 0x38BBBB0A, 0x6C0B446D, 0x9C72B87C,
    0x3CFA2F0A, 0x7CC7E479, 0xEBA28A9B, 0xB81BA39D, 0x37A79B77, 0x6469A7AA, 0xBD5BA88C,
    0xBBBAABDD, 0x3BE4B701, 0x36222EBC, 0x20BAF281, 0xEBE3BB01, 0xC7EBB846, 0x47C7A346,
    0x9929A88A, 0xC913B715, 0xB93B8AC1, 0x2A443BA3, 0xC6A8A9C8, 0xD8492B90, 0x3A59493B,
    0x0A9488BD, 0x713C2205, 0xD46CBBE4, 0x3BFC7348, 0x96A5B47E, 0x7BE5C76C, 0xAAABC9C7,
    0xAD388B8F, 0xCBCD0BFB, 0xCA247C21, 0x7647D8A9, 0xABBDCA14, 0xC923D0C9, 0x98BCF4A1,
    0x9483A8B4, 0x2774848A, 0x8D4ADCAF, 0x53A6C874, 0xA3FC8211, 0x8ABA945E, 0xC3E0A4FC,
    0xC8E34849, 0x41C8F468, 0xF47CB622, 0xCAC35313, 0xA0B81386, 0x53B6377C, 0xC9D98EC9,
    0xB64B0A4F, 0xE4B5C6E4, 0xC6B3BC24, 0xAD49FBAD, 0x9BD12784, 0x49649D4B, 0xB046FC7E,
    0x8CC73461, 0x4A73B54B, 0xA33B24C4, 0xEBEB4834, 0x8F9C7192, 0x6038F4A5, 0x8C7A3902,
    0x4BCCC3CA, 0x8649E4AB, 0xDC93366A, 0x4D646DCC, 0xC34763D0, 0x290FCB1C, 0x4924C84B,
    0xB52384B4, 0xC2814AFC, 0x27ECB7B4, 0x813DFC86, 0xB3DC4BA4, 0xC90A834B, 0xBE389CC4,
    0x949B9B24, 0xCC08A94D, 0xCA4C192A, 0xD4D0CDB4, 0x0DB306C7, 0xECC20C17, 0xF93D191B,
    0x9368DB4E, 0x3A1392E9, 0xF1949811, 0x13F4F113, 0x0E4F5193, 0xDCEE3FB4, 0x4F0971CE,
    0x46CF5CF9, 0x894789C1, 0xCF68B994, 0xFECF4CF5, 0x94EF4F7C, 0x14F07DCF, 0x52951B29,
    0x8D00CF59, 0x935179CF, 0x581574F5, 0x5879A491, 0xB26501BA, 0x3F40C33B, 0x015F0DC9,
    0x2F540596, 0x8250ACF7, 0xF9D727E9, 0x0BF1F436, 0xF5D11122, 0x85103F89, 0x5131F31A,
    0xC11219C5, 0x44319F59, 0xCF7C349B, 0x758B9C2C, 0xA9DE9769, 0xCFF186B7, 0x8B8C3C62,
    0xE97D97C9, 0x4F59E797, 0x644F94FE, 0x4983BF39, 0xD1D98598, 0x0ECF04FA, 0xFD0B309D,
    0x87FD054F, 0x8D98CD1F, 0x71FD3009, 0xCF9D2941, 0x06522527, 0xA505D26D, 0x52F529CF,
    0x972FE531, 0x49994231, 0xCF946142, 0x1A531D22, 0xA2814F95, 0x0A94994D, 0x7FC7A49A,
    0x9480C8CC, 0xCE04818A, 0x9ECCB389, 0x348C4CCA, 0x9E7D3CCB, 0x4A0F44BC, 0x74D725BB,
    0x4A037749, 0xD31BD3C9, 0x1348C794, 0x4A9263B9, 0x688DFBC5, 0x0B0406C0, 0xD4D33F86,
    0x58CF30E3, 0x3D5054DB, 0x8602560E, 0x4FD4E433, 0x25530E3D, 0x2560EED5, 0x3FD54D53,
    0x8D53D553, 0x433D54D5, 0x58D50D56, 0x3D5754F5, 0x1601D30E, 0xAA4CEC4F, 0xFC85CDC0,
    0x4DEBCB4B, 0xB9CEBC88, 0x927D489B, 0xC8BBDE11, 0xE7CDDB8D, 0x03AE548C, 0x48FA8224,
    0xED4AA4BC, 0x1C4A539C, 0x54153BD4, 0x49CEA3CB, 0x33B23DBA, 0x56B538CC, 0xA4B7ECBD,
    0xF53B33F9, 0x4E34804B, 0x3E3C4D3D, 0x0D3F3C55, 0xCCD4ACC7, 0x42B904C8, 0x4475211D,
    0xCECCC2D4, 0x474A5346, 0x84D4478D, 0x3534D53D, 0xBE05E140, 0x19033979, 0xBC59CC89,
    0x3A091891, 0x69D02D1C, 0x58509C58, 0x8943A2D2, 0xA09B88A5, 0xD8658808, 0xFA09C589,
    0x5D8B9E92, 0x091D85BC, 0x68869065, 0x786190D8, 0x28413517, 0xB93BFD60, 0xA3A58AE4,
    0x4E6BA727, 0xAA4E13DC, 0x7540CB83, 0xD68541D6, 0xD85690DD, 0xD2C54902, 0xD6C3944D,
    0xD8B76245, 0xF0AA57AC, 0xD9A495D6, 0x71D7E570, 0xC3C05A15, 0xBE0C9A56, 0x764D156D,
    0x6D725425, 0xC9F49ACD, 0x9DBC34AC, 0x5B3D5775, 0x544BC6BC, 0xD05AB4A4, 0x0BCAC780,
    0x1C24C6D8, 0xB23CDC7B, 0x4D4F1403, 0x91BBC987, 0x038D23DE, 0x9354D669, 0xA0B5BBBB,
    0x5E87FB78, 0x5B7A26E8, 0x4DC3BFD9, 0xD93D1B66, 0x2BDA300B, 0xD9EDA656, 0x9BC8A4B6,
    0x03E5C7A5, 0x4895C828, 0x698B28B8, 0x0BE30CA5, 0xDB0D705A, 0x7F5AACE7, 0x6A894C04,
    0xDC5CEB3B, 0x01CC3384, 0xD969406A, 0x0B85D521, 0xBDC58142, 0x107186F0, 0x0EEAEA07,
    0x1FBCEAD5, 0x6A7C9E74, 0x9E75D5DD, 0xC6DDDB02, 0x45DB5DD0, 0x88CC4FDD, 0x6D54F098,
    0xCBD3B271, 0x5D39315D, 0xE6330C59, 0xF8B9A655, 0x27CBFDD8, 0xE4201C35, 0x28A8DDC5,
    0x97833842, 0xDEA018D9, 0xE8CB8B85, 0x3054052A, 0x2C27CAB4, 0xF26E07FD, 0x073871A6,
    0x0A8C4073, 0x95EC5E01, 0xDE7AEE9C, 0xDCA01DDB, 0x3DEF5F80, 0x8A8DDF40, 0x0F90AA8F,
    0xA35D85E4, 0x5DADC415, 0x008B85F7, 0xD45DAC8C, 0xDFBDDDC3, 0x7E5DDDE5, 0xC1E345C9,
    0x1E75F8C5, 0x1F5DDDFC, 0xFC568C6C, 0x950B2887, 0xE5210AB1, 0xFDFA132D, 0x0CFE09DE,
    0x1F282B24, 0xA3F1C113, 0x5DB60ADF, 0x2B6153FA, 0x58B9DFB3, 0xC3E87FE1, 0x1711FB79,
    0x95FDE106, 0xE09D0A5F, 0x190C3E0E, 0xB9E7E1EC, 0x5D823F5E, 0xD8BF05D9, 0xF421D642,
    0x6060B9E1, 0x27625614, 0x03FFC07E, 0xE25E2162, 0x868CBBF1, 0x3AF4DE28, 0x627BF15E,
    0x27E2DE29, 0x0330D35C, 0x5E88E8E2, 0xFB866BFD, 0xE51AE202, 0xE2F618E2, 0xC15ED1D7,
    0x75645EEC, 0xDF123F2E, 0x8D962304, 0x38C3E2A2, 0x87FDF4DF, 0x055F6DF5, 0xFE26C5F9,
    0x2EE99B62, 0xF3E35E31, 0x62F5E42B, 0x62863C63, 0xCEE20E41, 0x0507DFF2, 0x2E0B5E60,
    0xA88CB42F, 0x49DEE038, 0xE2E9E760, 0x078C6E40, 0xCE093DA6, 0x646E2627, 0x0CC35320,
    0x5E5260DE, 0x644651E4, 0x12656646, 0x56135216, 0xE5632013, 0xFA627E29, 0xC32B65A3,
    0x0A8E4EE5, 0x1B159E1A, 0x5655D646, 0xE34BF1E5, 0xDD8B75B2, 0xCDA9CC25, 0xD464BED9,
    0xA83BF3E6, 0x4CDAB9FD, 0x4703C1BB, 0x03DAECCC, 0x4DB05679, 0xCEC4CFB8, 0xC5DC8BD5,
    0x034FDB40, 0xD81BE0B9, 0xF4DB8389, 0x8507872A, 0x597D6C66, 0x76BE6251, 0x75706676,
    0x5BF8FB2C, 0x9651D26F, 0xEBD70765, 0x545DC29D, 0x68DC4367, 0xCCC23DB3, 0x56523B27,
    0xAFB4748B, 0x15BC1DFD, 0x15B5CABE, 0x5F2D5B72, 0xD668A53D, 0x4D1DCE9A, 0xE834DDC3,
    0xDBE90D7B, 0xD7146CDC, 0xAAE67CA9, 0xBD3AE88C, 0xD3C57546, 0x7453DDAB, 0xAD78A8B3,
    0x677665E6, 0x6CE92D9D, 0x266DC91E, 0x695E8E5B, 0xA6BCA57F, 0x6CD4C6DC, 0x5B7D4A5B,
    0x7454F135, 0xB675158E, 0x9E24B3A8, 0x9207007F, 0xD0A94C7A, 0x2E90D5AB, 0xCE3B05D9,
    0x70ADD394, 0xE7A1DDAC, 0x7B93D5C0, 0x7485232E, 0x1E3CE959, 0x99C87913, 0x6D68D77D,
    0x1E9564DC, 0x61D614E2, 0x0C8ECA7B, 0x56ABAFA4, 0xA156BE8F, 0xE691D6D6, 0xC94BE756,
    0xD224A4EA, 0x0E99C91D, 0x5A1E87EB, 0xA44C58E0, 0x95B5CE86, 0xCE3CAE66, 0xA3EBBCC9,
    0x3386694C, 0xEC1D7A13, 0xCF6C2D76, 0xCCA3D43C, 0x0ADB876B, 0xD369BDB4, 0x56726BFB,
    0x93A6733C, 0xB7DA8E9F, 0x8379BCB6, 0x04E04F84, 0xD5877B78, 0xFE220AA0, 0x85705007,
    0x5107F855, 0x00560578, 0x12298D92, 0x30688693, 0x05BF6A92, 0x5C1E7B5C, 0xB26AC076,
    0x8D61C724, 0xCDFEAF59, 0xB74E0E93, 0x2BAB5C7C, 0x6BFEB3CE, 0xB53AE8AA, 0x84A7D6A6,
    0xEB84BDEA, 0x946E55E1, 0x05D1A89C, 0xCC35B0EB, 0x2ED23D2D, 0x9D06D275, 0xBEF9370B,
    0xF04F42F4, 0xFCF65306, 0xD0F53752, 0x32EF4D04, 0xA4FBD2DD, 0x9E7CFC88, 0xFAD27D36,
    0x6B194EEE, 0x150D0250, 0xF6D27D03, 0x7529D1E6, 0x465158D0, 0x0F2E6509, 0xC8C3EF0D,
    0xD0E63AD0, 0xAF8AF52D, 0x126F35D0, 0x07287287, 0xEFEF56FA, 0x49E7D30E, 0xED86D753,
    0x5204A87F, 0x107F8508, 0x35986586, 0x6285E8AA, 0xA854B5E0, 0x9D405905, 0x4CB0406B,
    0xEA6C8538, 0x6FFE51C3, 0x720DCD27, 0xF8738738, 0x0C764907, 0x7FF046A2, 0x523751B8,
    0xD32331D9, 0x7C17B50B, 0x6D213239, 0x70B53644, 0x8F931504, 0x507FD249, 0xC1F18618,
    0x37EFBD2D, 0xAD27EF5D, 0x52B85F52, 0xF452D5C4, 0x0529F2B6, 0x52DD2E73, 0x37D33700,
    0x5356998B, 0x726730F2, 0xFF6F6EF4, 0x79E55DD9, 0x46A1AC9C, 0x479F86BF, 0x6512514A,
    0x9D79C828, 0xD99FEF38, 0xE3ADF3C9, 0xF3C695DA, 0xC025DD6C, 0x31C83D39, 0xAB0D7CD4,
    0xC073FBCB, 0xDDB43E66, 0x9B1932C5, 0x094A7D10, 0xB9F5ECC9, 0xF1FECD8C, 0x3F68FE74,
    0xDF42D11F, 0x0534328A, 0x652B7859, 0x6B15B980, 0x8BD8A7B1, 0x17F1C975, 0x284B26CF,
    0x87F85105, 0x0918DB60, 0x474773E1, 0xEAA6DC25, 0x5FA9D6DD, 0x04106DED, 0x73D1B33B,
    0xC4ACBC8C, 0x51A36AFD, 0xEB15169F, 0x9ACE18D2, 0x3299761A, 0xA3F1ACF4, 0x630B8737,
    0x455EA82F, 0x1FDF3EAB, 0x476C5294, 0xF768250F, 0xDA5257F6, 0x39764EEC, 0x0A317507,
    0x68D68EA0, 0xC0E908B4, 0xCEEB747E, 0x6CF6BDC9, 0x7EEC4353, 0x3E896655, 0x4A2F79E8,
    0x6ECD06C8, 0x669ADB36, 0xCCE4D3CA, 0x9DB91E9C, 0xEF4DDB0E, 0xEE9EA0EC, 0x6105BA0F,
    0xCA6B8A70, 0x05C056A6, 0x51049A2B, 0x387F84F8, 0xA27710C1, 0x8A05607F, 0x506B1D2F,
    0xED6ED58A, 0x8DAC7873, 0x9EE2EC74, 0x6DD75D8A, 0x96DA85C1, 0x9E765A85, 0xEAFD620A,
    0xE3D9AF94, 0x4D9C6826, 0xECBF98E8, 0xB8563DC9, 0x65A04E50, 0xCA3F7643, 0xB36EAE89,
    0xB5D16EBE, 0x5D2EED6B, 0x64775DA7, 0x3574EBF6, 0x6E5D75FA, 0xA9D3EEE9, 0x07717410,
    0x5727AA6E, 0x75BC6745, 0xA7A6E994, 0xECAF7C69, 0x8169C781, 0x4A8B69D5, 0x6B774EF8,
    0x368CBB53, 0x426C04CA, 0x90210F59, 0x5604F8BB, 0x67AF0548, 0x1520618F, 0x570582BA,
    0x506187F8, 0x717B5986, 0xD3075B58, 0xF77DF93E, 0x7936A8F4, 0x31F96F5E, 0xD3A37602,
    0x7995A46A, 0xA1CB4EE1, 0x1D646E6F, 0xEE4EB268, 0x9EF797A3, 0x67A01E77, 0x4ABFCBEB,
    0x9DEBFEEA, 0x577207FF, 0xDA6F9AFA, 0x555B277F, 0x8C66645C, 0x6A3C2A43, 0x5E42C24F,
    0x8C50C38C, 0xC5F1B3C3, 0xF842ED0F, 0xE2564329, 0xAA3AD29F, 0xC27DD5E6, 0x8458CA7A,
    0xAA5C3A0B, 0xA7A46C3B, 0xEA7EFDBA, 0x43FAAAC3, 0x022A8C34, 0x9AAF441E, 0x2B474654,
    0x7254F41D, 0x2F1C8740, 0x45044B8D, 0x228B8517, 0x093A26E4, 0x06C23013, 0x8A2B02D2,
    0x786185CA, 0x859F148D, 0x6B86807F, 0xAACD9600, 0x9422DE45, 0x979968D2, 0xA8806AA1,
    0xBFDECF67, 0xF5FF067D, 0x1FCFF84B, 0xD6E87FC3, 0x4B97FBAD, 0xF97FB058, 0xA843E9F4,
    0x7FBF6130, 0xCC5A3EFC, 0x47634F97, 0xC1FEFE86, 0x1FEF2793, 0xFEC0612E, 0x6F37A3C9,
    0x248E4361, 0x79CBE1FF, 0x834DA6D3, 0xCC61FEFB, 0xB3D8F368, 0x71ACFF66, 0xE411E6DB,
    0x153B7E47, 0x92390D36, 0x418546E7, 0xDAA53A97, 0x94C1E7B1, 0xE2F9672B, 0xD6245385,
    0xDEB50782, 0xE9EF89CC, 0x4520A856, 0xB6DAAD4E, 0x68B2DC23, 0x69B5D6A3, 0x76BA9FF2,
    0x22E9FEED, 0xBD1FEC26, 0x9E68F57A, 0xED96AB44, 0xD157A8CD, 0x518C8D5E, 0x19C9652A,
    0x0CCBDEAF, 0xA8D5F274, 0xFB85C4DE, 0x31D90BF7, 0x6E7D3A9C, 0xAEB8B8CE, 0xD7A6D4E5,
    0x9ABABEB6, 0x99C9EC6C, 0x5D6EB9EF, 0x6F366A37, 0x332344DD, 0xE2E0F1E6, 0x172797DA,
    0xB55CACE7, 0x5DCBD64E, 0xF496BEA3, 0xCB66B15F, 0x69D72372, 0x8CF635FC, 0x4BA9CFA8,
    0x60976FF7, 0xFECD86C2, 0xF69CDD94, 0xACD0A8E4, 0xCC77FB59, 0xCFC75199, 0xFFCBD2AF,
    0x62B1D7CF, 0x5399C4FF, 0xDD4E70CE, 0x756B9DD6, 0x54519725, 0x1A862DA8, 0xE3ECFC6A,
    0xD98B5454, 0x236CDAD4, 0x8420576D, 0xCE83954E, 0x62F0FF39, 0xCCFF310C, 0xEC3E9FA3,
    0x39907829, 0x1123A0E9, 0x73DCFF45, 0x6A1641DA, 0xEE6A1937, 0x4B0F03C0, 0x4EF3FD2E,
    0xD6A23A3C, 0x21DB815A, 0xC64FE328, 0x3FCCC338, 0x0DA36CD5, 0x08D2067A, 0xADD97DDB,
    0x5771DCE5, 0x3BE386BA, 0xD62580FF, 0x655D0440, 0x4E4381A5, 0x52915199, 0x955EA962,
    0xE0981A28, 0x33A90F93, 0xCCF1795E, 0xCF9052B3, 0x64E69991, 0x8FF81A67, 0xEA027F9F,
    0x12815A27, 0x81A1594A, 0x390E5382, 0x0C9300FF, 0xEA1D8832, 0xFE915EDA, 0x7EA51579,
    0xA5D366A2, 0xE93A5E86, 0x0384DC9F, 0x94C9A30E, 0x9BA5A806, 0xA6D93A56, 0xA9B42A99,
    0x86FDBD4D, 0xEE8092A4, 0xA7A6DA85, 0x3B17DA52, 0x0C3315E3, 0x7464FB50, 0x129E6A28,
    0xD8360D2A, 0x33CC83FC, 0x57BAA0CE, 0x5709A381, 0xD6818B0E, 0xB85EE9B5, 0xB1157A46,
    0x297ADA91, 0x5A6D1DAB, 0xED0F9FED, 0xFF7F4BDA, 0x5459A3D8, 0xCEC3A687, 0xC455135F,
    0x6297AB4F, 0xE73CE80B, 0x3D12F4B5, 0x2E161CF6, 0xD42AAE59, 0x31449FAD, 0xDC374D7B,
    0xAA6A84DC, 0xD07B7AC1, 0x754687B6, 0x865C3D2F, 0x7B481055, 0xB8A97A69, 0x2AAB5E94,
    0x5C3D93BD, 0xAE7FBD68, 0x8196CEE3, 0x6E10360D, 0x6D648EFE, 0xFC2B0243, 0x6AEB5467,
    0x89A5EFFC, 0xA3E8DA2E, 0xD8FA1319, 0xCC7684BC, 0x820FA530, 0x3CAA32C4, 0xA8AA03FF,
    0x30BA968D, 0x8D0687A4, 0x014ED12A, 0xFFAC11DC, 0x57281AC8, 0x720ADD3F, 0xCAEA8743,
    0xB517AFAB, 0x91B7E85C, 0xB2AC8B1A, 0xA30D56CC, 0x23FED066, 0x6E808B6D, 0x1E85691C,
    0xB833EA53, 0xEABD4685, 0x5A533441, 0x991143F7, 0xB8BA5FEB, 0xBB681E01, 0x316BC2BB,
    0x34377CD5, 0x4D319FB2, 0xF5892F9C, 0xA6F46C4C, 0x28041E0B, 0x8C18FFC1, 0x435F5EB0,
    0x81E4927E, 0x4839BD9E, 0xE5D4CD31, 0x94D93FAF, 0x1F75A1F8, 0xD9D0B77A, 0xAD34864E,
    0x681DE391, 0x5600EEC9, 0x6904409E, 0x31D6E97E, 0xD1733A13, 0x294D9735, 0x1A3EA3CE,
    0x92DA9FB2, 0xC2EF779E, 0xE07CBDFE, 0x7D5C7336, 0xDA8EA1D2, 0x7FD7F4F4, 0x3FF63B7C,
    0xABE85EC9, 0xA49D61BE, 0x5E7701D6, 0xEF61AA3E, 0xE6AFEBD2, 0xF7AC39FF, 0xEC9DA680,
    0xE42B47B2, 0xDB89C159, 0xBDC6E8B4, 0x7E4DF548, 0xE9D40BD2, 0x54353500, 0xE51D6E09,
    0xCAE1D73A, 0xB9772EC2, 0xC33F5E28, 0x74FCFE1D, 0x6E58C3AF, 0x1E62FD5F, 0xB5C738AA,
    0x742E7D3F, 0x03598B09, 0xFDB0E7F3, 0x98622A15, 0x225898A3, 0x3CB71594, 0x43CD7B6E,
    0xC83AEF20, 0xA49DAF40, 0xD92C3E6B, 0x32A3CAC9, 0x6A101E08, 0xCF5F8B2F, 0x311944CA,
    0xAD334909, 0x80680CF1, 0xCD069A53, 0x71954A88, 0xB4A29BA2, 0x24653D45, 0x0DDA064F,
    0x3706AE48, 0x8BA4FD08, 0x1811AC5C, 0xE7C503B9, 0xD53A6744, 0xB7178EB9, 0x1CEF46A3,
    0x21E23C04, 0x730A60CF, 0x67313B98, 0x0F6A184D, 0x9F13DE48, 0x2407D4F9, 0xC4CFC9DB,
    0x001FE3F8, 0x6E436809, 0xC8910DC3, 0x5CD74772, 0xB45A4A08, 0xD9067AE3, 0x28450D46,
    0x91021E6E, 0x22644888, 0x1FF1A136, 0xE3516A2B, 0x4016B462, 0x4F8D9262, 0x2C8E91C2,
    0x652023E7, 0xD4624B11, 0xD48D258D, 0xC931253E, 0x24E4B049, 0x49D4DA9A, 0x4B49624D,
    0x40D74434, 0x28A6C40E, 0x7C89D252, 0x8D17E622, 0xC0D44C59, 0xA1837829, 0xE9729D93,
    0x673E5D9A, 0xCB41CF9A, 0x18A6CEF1, 0x2F65A09D, 0x38C720DF, 0xB424F7C2, 0x7386F190,
    0xE45E4B29, 0x98AB9372, 0xB2149ED3, 0xD62EEC67, 0xE3806D90, 0x0D228C1B, 0x18C9E2A4,
    0x9C8590BC, 0x59D1B156, 0x7FC389F8, 0xD460E360, 0x14634EB1, 0xD2873B99, 0x19512C85,
    0x15DA49DD, 0x2AF9FA91, 0x87F1B846, 0x12033B3D, 0x1A23FE8E, 0xFF86B8D7, 0x7B318598,
    0xFB4BE546, 0xC8708639, 0xECBDCA31, 0x74FD9E73, 0xAA753D4A, 0xE5DE8310, 0x28C5092D,
    0xD3EA1A74, 0x2F662C8E, 0xE15389E0, 0x77BC31FE, 0x832F642A, 0x2E45A0CB, 0xC6844F3A,
    0xF10D24A5, 0xC25E8ECA, 0x739D483A, 0x08ED0A2D, 0x24826D2C, 0x294520A3, 0x456793CA,
    0xA3D43A79, 0x44A366BA, 0x5D570D45, 0x828D0F87, 0xCCF39F24, 0x9A8541E9, 0x1500C655,
    0xC9BD7EA6, 0x17F2FA5E, 0x30E79994, 0x3A2BDD2F, 0xA11F6A89, 0x127553D8, 0x68A45ED2,
    0xE9E7A857, 0x844D856C, 0xD680DEEC, 0x2F65A943, 0xA24D9587, 0x34A836FA, 0x880A9A77,
    0xF91D343C, 0xB6C0F508, 0x806B2D5D, 0x9F109E3A, 0xD9FA2856, 0x32DC6C8F, 0xD58EDA2A,
    0x44760052, 0xE02AD180, 0x284B2C9F, 0x6F4AA1DB, 0xDF8D75AB, 0xD9D947D0, 0x4DAA701D,
    0xFCFA60AD, 0xDAAA38B4, 0xA3A7B970, 0x972F575C, 0xF64A1239, 0x9FFB9E49, 0x44AA510A,
    0x81D0FAD6, 0x57EF0DBC, 0xDE0FA75D, 0xA45A9756, 0xD342EE3D, 0xFDDD34EB, 0xFA848129,
    0xDA9D369D, 0x8A51E2CC, 0x1993C8B6, 0x285B9E53, 0x514A22E3, 0x0FECD3D0, 0xD6DDAB39,
    0x48B42EA2, 0x4CED61DC, 0x35A63E0B, 0x459EB6D8, 0x2B9D682F, 0x8D5B0ADA, 0x93FDB27D,
    0x9197AD80, 0x6C76E205, 0xEC2DDA27, 0xDF3FC836, 0x93B80DAE, 0xE770CE04, 0xBEF9DC56,
    0x1C2D23AF, 0xCBCF839F, 0x787986DC, 0x2E9281C2, 0x6065D505, 0x7183B2EB, 0x8FAECA2F,
    0xCB1FABB7, 0xE1C84C49, 0x250A5985, 0xFDD2DE6A, 0xBDEB6878, 0x5BDA5454, 0xB8B6F7B1,
    0x2035F123, 0x63AB4B85, 0xC7B6A708, 0x7F1D3E58, 0x0054FB11, 0x277E0DDB, 0xC659701C,
    0x1332E37C, 0x628EB30A, 0x9CF99A70, 0xD736CFDF, 0x9E35E082, 0xAA469785, 0xBF502F0C,
    0x8074162C, 0xF75B1215, 0xC9CE56CD, 0xDAD82D55, 0x56902BB1, 0xED50CF15, 0x17FAA06C,
    0x9709B6E3, 0x13F63680, 0x667D2382, 0xCFD8F2FB, 0xF9F4527E, 0x1DDA3D18, 0x4670D912,
    0xAE9FF0FE, 0xAF2638DB, 0x85DCC219, 0x0CE10CF2, 0x5132BCB1, 0x72B5E43E, 0x6AB86154,
    0xD5590340, 0x4CB2EA81, 0xB6E60996, 0xA7D0994A, 0x379AB49E, 0xB9DF659C, 0xFE6E7803,
    0x4D1BA0EF, 0xF99360D6, 0x4B5CBD89, 0x9752EB26, 0x2C9B9902, 0x1546E9A8, 0x73C626E1,
    0x23DD266B, 0xE98CF791, 0x9372A7CD, 0xB1B8DE05, 0xEAF1BE77, 0xF4D23314, 0xF7EEE849,
    0x6F5E09BC, 0x98FBDF83, 0x465606F4, 0x7FA4D1AC, 0xCA198310, 0xF048F710, 0x0C91F05D,
    0x5A4FE2FC, 0x344B7F65, 0xC6538631, 0x4F7A4FE2, 0x9F43E435, 0xDF84FCE1, 0x3FE46123,
    0x478C617A, 0xA8404396, 0x29C538C9, 0xDF7A7343, 0xC964E11C, 0x2317C6B9, 0x893F7D72,
    0x50F8E71C, 0x8F244688, 0xE164CC79, 0x8B50E907, 0x0A91525A, 0x34E0C986, 0xDE3B4D47,
    0x045C25B1, 0x7CB725E3, 0xADE3CE12, 0xC4EB8AF3, 0x1251D54D, 0x0D91B435, 0xEBBC6A67,
    0x1BBAFEFB, 0xAAEF4ED9, 0xBD16696F, 0xCF5385D8, 0x1F9527C3, 0xF752ED3C, 0xCE31DEBA,
    0xBD039AF7, 0x4751DF2E, 0x8FF8F89C, 0xEB84A878, 0xDF7B5F7B, 0x1C70AE43, 0xD1AD6A03,
    0x33829E76, 0x5F54DF8B, 0x7E3B521E, 0x0D499D92, 0x1A171279, 0x344B4EB5, 0x641CB5B2,
    0xD2188AD8, 0x9CF6FFA0, 0x40E1A839, 0x8BD338A5, 0xFFA7BCCE, 0x0DB7EA07, 0xD5F26E50,
    0x7DAFA9B4, 0x1936E528, 0x7D3C2ECD, 0x91021DB7, 0x71191796, 0xAD87FE47, 0x567CBC97,
    0x3CD769FF, 0x63AFB28A, 0x57D6EC08, 0x0EC57459, 0xBD51AE62, 0xAEAE12E9, 0xEF85EED9,
    0xB6DDC3F2, 0xFBBF03CD, 0xF0B36A91, 0x1943ADB8, 0x92790CC3, 0x2B9E7EAF, 0xA7CCE7BF,
    0x7F9BDADF, 0x1C41F16D, 0x5862D3A8, 0x871E919F, 0xDDE75A05, 0xCACDA3C4, 0x029789EA,
    0x585F1B19, 0x5EC8D89A, 0x8C1ED1AB, 0xCDB49ED9, 0x9EE8DC18, 0xFA2059F9, 0x7983D559,
    0x1FBDFF1F, 0xBA1AC4A0, 0x34A45ACC, 0x9AE0455F, 0x93DAE9FE, 0xF5AF5F4D, 0x0539F59F,
    0xF69B0D95, 0xA9F71569, 0x196E07E0, 0xEB5B30A5, 0xD1F9197C, 0x9A94A11A, 0xB5607202,
    0xF9F40A05, 0xC60DFB09, 0x7D20BD9B, 0x119D1EB1, 0xE0FD64D8, 0x9E8AA1EE, 0xE6034F45,
    0x6015EB9F, 0x8C99DDF9, 0x1E000A01, 0xCF219F5A, 0xEA1849FE, 0x09EA8AC5, 0xD6D5A48A,
    0x7BDEC4A6, 0x14EA50F9, 0x8DB1ED4A, 0x065A8605, 0x09F9E16E, 0x98B9E61F, 0xE721DCC8,
    0xC1E7A169, 0x574DF1DA, 0x92905E09, 0xFDA71069, 0x507D841E, 0x12DF5177, 0x05F5DB2E,
    0xDB29F64A, 0x9860E221, 0xDC539F81, 0xD0D60F0B, 0xFD143D49, 0xE9EF98E5, 0xE24A1121,
    0x7F5B65FA, 0x5A086129, 0x613DFC60, 0x9614599D, 0xE884DE48, 0x84D9E484, 0x38838C37,
    0x155C5158, 0x89B85398, 0x2BC831E4, 0xA13D22DE, 0xE2BA2F52, 0x2AE2B45A, 0x7C3684DE,
    0x18D03783, 0x16048042, 0x71099905, 0x95BA30C7, 0x1422A927, 0xC8E81CA2, 0xA3394511,
    0x331E4941, 0x15F1A34E, 0x03DD119E, 0x989E4841, 0xA84755C8, 0xDC9D21D4, 0x6A05954E,
    0x3861C618, 0xC38C3FD2, 0x32839839, 0x522F1522, 0x233D63E3, 0x2CA32E30, 0xD87122E9,
    0xA3923763, 0x3A22E23E, 0x4D2A8BC9, 0x9625D14B, 0x4805662D, 0xDD5511E8, 0x63F448D5,
    0x5463FD1A, 0x743C48F8, 0x24584A57, 0x3DE3E634, 0x9D41A312, 0x9C3C92D5, 0x33984883,
    0x0646862E, 0xC53A4724, 0xEB9D6C55, 0x9893D2B8, 0x646E4A14, 0x3749AA37, 0xAE42A306,
    0x630A3FA3, 0x7F547A2F, 0x9646957D, 0x1DA240A4, 0x14E5024D, 0x524FE501, 0x62CE2C63,
    0x5964BE2D, 0xEE2F62E4, 0xE4C24E62, 0x51A37649, 0x1A0764CA, 0x4DC23223, 0x2BE3EC3F,
    0x5223A48D, 0x22FE38E5, 0x7492A64C, 0xDD1CC528, 0xDBF9CA64, 0xE492FA54, 0xA5E19365,
    0xA2F6388B, 0x39C4DA54, 0xA8519C9A, 0xE3B633A3, 0x3C47F94C, 0xD656A3CE, 0x65565423,
    0x40E50920, 0xC6445316, 0xA34A5824, 0x4764124F, 0xFA421C70, 0xA56A4D91, 0xE3643453,
    0x3E37643C, 0xE4563765, 0x4643CA45, 0xF644E516, 0x2441D462, 0x30647C48, 0x9103C93D,
    0xA35267A4, 0x4A9E4A54, 0xB532C8FE, 0x26292C24, 0x16A6AE60, 0x9E4DA6C5, 0xC4826CA6,
    0x4F56224E, 0x526E2676, 0x63766823, 0x10601978, 0xBE1509FA, 0x1E659F5E, 0x70DB2618,
    0x7219DE7E, 0x8F8A179A, 0x87589A27, 0x21FF9AB9, 0xE2A22760, 0x03CFCD66, 0xCE1B1A62,
    0x09FA1E5E, 0x1CD8CA1C, 0xCA1F9712, 0xA24610D8, 0x146164D3, 0x8C9FA06A, 0xDF2D7520,
    0x78047A09, 0x24A1D866, 0x977DAF22, 0x23A0EA29, 0x10C1578E, 0x4FCA219B, 0x2A6725A0,
    0x6532E25E, 0x62723A97, 0x80E7A1EF, 0x20A1DAA6, 0x0F117FA1, 0x765EF623, 0x1E756256,
    0x83FE7358, 0x075FE271, 0x5A24DF06, 0x28621028, 0xA22740D2, 0x4611A035, 0xDB79F118,
    0x84E75C3F, 0x1E27DB82, 0x9EC49FE7, 0x7D20496E, 0xE1EDE78A, 0x27960606, 0x82E8A4A7,
    0xAA89A7D2, 0xDABE8A17, 0xF2620190, 0x25F3287D, 0x5F3D7619, 0xF48A05E9, 0xEA22C4BD,
    0x6338A467, 0x7FCE827F, 0x11B1E8BE, 0x6831B260, 0x58997859, 0x4A821B3C, 0xE1EE10A8,
    0x95CEEE15, 0x6E28D7EE, 0xA93E845B, 0x70A851FF, 0x468EA1F6, 0xA7D2868A, 0x9919FE98,
    0xF2731A2E, 0xE176029E, 0xE8E17D85, 0xB692A895, 0x4FAE019A, 0x73A02619, 0xA274E02E,
    0xD6E67718, 0x8028CD8A, 0xC6052786, 0xE7928A61, 0xFFA9B9A8, 0x6AA328F1, 0x60821EDE,
    0x90E08A90, 0x45F29ACA, 0x60A67CA1, 0xF469FA11, 0xBA9360B1, 0x60C29A60, 0x7F9B0AA1,
    0x5E9560D2, 0xE96687AA, 0x0F9B3A0E, 0x4D9C5E4E, 0x9E656D5B, 0x29683E89, 0xB629EA6A,
    0x9BD279A8, 0xC69E3DCF, 0xB093DCF1, 0x9BB8685B, 0xCEC9ED14, 0x15DE1DFD, 0x5209C26B,
    0xC1AB3DCD, 0xFCFB06F1, 0x5CCDC042, 0xB4DDB2B3, 0x0DB91D82, 0x9C80652B, 0xC44939C3,
    0x7EB05C4D, 0x1C19E06B, 0x621C706A, 0x2C415C79, 0x1CD6B76B, 0x5D5C906A, 0xB1DE2356,
    0x9CC1CB9C, 0xB7041DCC, 0x12B69CDA, 0x2B4ABFAB, 0xE02B42C0, 0xA06E2B31, 0x5D11D086,
    0xD25D1D29, 0xA1D2EBA5, 0x08BDD343, 0xBF6BBDD4, 0xC093DE32, 0x5D644B1D, 0x6A26B48F,
    0xC2C11DA8, 0x0335D8AB, 0x505D99D9, 0x8EC0AB3A, 0xDCD6C79B, 0xDD1DCAC5, 0x6DDDE5E5,
    0x6BFEC9AB, 0xDE2C76C5, 0x0C48EBA1, 0x536E37DE, 0xE2EC71E2, 0x6ACBACA1, 0x67B29B1C,
    0xFD8CDE1D, 0x729C4CED, 0x67ADFAA8, 0x9E5FFAAD, 0x52745A1A, 0x2A8A0D8F, 0x9B69FE9D,
    0x612FEA06, 0x2892AFE7, 0x8C4A02D4, 0xE4D9AA2A, 0xEA39A760, 0x8D5EE278, 0x7A1DA149,
    0xA95A9C9F, 0x91689E74, 0x11F1A7B6, 0x17C6910E, 0xD4A926A8, 0xB4E56A92, 0xEAF67E60,
    0xB060C67E, 0xDD95E241, 0x6AC67520, 0xEBE261F8, 0x82825B40, 0xED1ED9E9, 0x99A83ADF,
    0xE69A1FB6, 0xAAAAADAD, 0xE1A2F5FC, 0x2AD12716, 0x2ABEDA1B, 0x1729DA75, 0x3A9E8A7E,
    0xED21A52D, 0xA3AD5272, 0xDEA0DEA5, 0xA9F29AC4, 0xA228C2D6, 0xB9A698AA, 0xAD84B161,
    0x1D23268D, 0x52D9A79A, 0xE8F9B29B, 0xD0ADB1F0, 0x92A727BA, 0xE09AA7A0, 0xDCDADE91,
    0x7A932DAA, 0xAAA6D297, 0x612AB20C, 0xBADD29F5, 0x9AFED7D6, 0xE1296C53, 0x32AA297A,
    0x6722DAA1, 0x11EA8E8F, 0xBA126E32, 0x29AAE3DF, 0xF22D02D6, 0xDCCE5FD6, 0xE282D19F,
    0xEEAF629C, 0x30DB6E6E, 0xDAA5E8AD, 0xA62FAE00, 0xAE05A18C, 0x6EA202A8, 0xD72036FC,
    0xFEF42EBA, 0xE1CA8D49, 0xD95A8AD8, 0x2698606E, 0x2FCED46E, 0xED6D061F, 0xC2EF8B8E,
    0xAF0AA7E7, 0x89A145F3, 0x8293AF1A, 0xEDF2F26F, 0xEAAF320C, 0x5218ADA6, 0x5F897B22,
    0xFB62520F, 0xA6E23016, 0xD843076F, 0x742F7EAE, 0x52A129DD, 0x269558E8, 0x6315856F,
    0xB93B13A8, 0x46515E93, 0x4F5B4239, 0xC93E0631, 0x16353EB0, 0x5EC5455C, 0x01409400,
    0x45ED4103, 0x60D55D60, 0x530FB0C1, 0x45395062, 0x598C2144, 0xC172A3AA, 0xD4A25A53,
    0x489A624E, 0x1149442D, 0x85ED5B12, 0x8C54B14A, 0xC9B4162D, 0xD4D47F14, 0x5F04354D,
    0xC0653141, 0xF0EF12B0, 0x4330F55B, 0xAF10313D, 0x1E115285, 0x53C7F9F2, 0x1D548D9D,
    0x845CE35F, 0x0F23A983, 0x595705EF, 0xB19557CE, 0x3DB1B04D, 0x7C429595, 0x63755A64,
    0x6430F31B, 0x1C63317C, 0x4C555D4F, 0x0C233A65, 0x4712F22F, 0x14FF25A3, 0x27D60B16,
    0xF56194A3, 0xB22663D7, 0xB4327442, 0x8D7D7159, 0x8865C191, 0x4822B2B2, 0x12B2067C,
    0xE4CE3714, 0x25C9611B, 0xE133F2E5, 0x72BF2C89, 0xB59BFE68, 0xF12A500E, 0xB31C73F2,
    0x5172EA60, 0x4E5904FD, 0xB3211D14, 0x1C11D52C, 0xC87851E5, 0x079D1F49, 0x52D2652C,
    0xE2BD121A, 0xDCB06B63, 0x80D2423B, 0xC12558CC, 0xF32D352B, 0x2FB3A24C, 0x84BA92EB,
    0xEC248752, 0x89EBB488, 0x4C8AA624, 0xD2F92B4B, 0x4FD1E334, 0x6D7752D0, 0x06D930DD,
    0xBC12F330, 0x90D646A2, 0x51B74026, 0x349DB732, 0xB932876E, 0x67014C8E, 0x1B53472C,
    0x1F425389, 0xB1393513, 0x3717F637, 0x4F2FE4B9, 0x47733074, 0xE52F8C6B, 0x2EE5A866,
    0x6EDED3D0, 0xB6F492D2, 0x2A72A88C, 0x2EEF078C, 0x0BAFDEA1, 0x5E7AAF8B, 0xCB021ACA,
    0x00102DA2, 0xD300AA43, 0xAE86A4EE, 0x4BA036A1, 0x469B69D7, 0x6A7504D0, 0xBBCBB691,
    0x58BC88BC, 0x0BDB512A, 0x0888F0BE, 0xEE236F91, 0x7069F62D, 0xD7B54296, 0x4502C4FE,
    0x9944481E, 0x26E81CEB, 0x9B0A297E, 0x356EABB0, 0xCA5116F7, 0x8F554BA4, 0xCCC6E469,
    0x497595FC, 0x5AD16F9B, 0x2EE6E62A, 0x0B2902FD, 0xB49FAE73, 0xA07AEA74, 0x886E8EE6,
    0xB6D5EA09, 0xEA1E285E, 0xFF6A25A5, 0xC61BF07E, 0x68DF006E, 0x7128E5EE, 0x9F0193DE,
    0x74AAE670, 0x20A7BDF1, 0x4EDC8A16, 0x44C2F6F0, 0x492F1EA9, 0x650A2947, 0xB605F6F5,
    0x57E7D280, 0x73086F4B, 0x3492AD29, 0xF6AAE172, 0xA83FEF3A, 0x75F2E3B0, 0xE42FE4DB,
    0x1B652966, 0xEE8349E7, 0x53F6AA74, 0xBD72EEF3, 0xF4DA006F, 0x0EA1EF4B, 0xBED5A193,
    0x5E6E01B5, 0x7745FCC9, 0xBAF474E6, 0xD72F4F2A, 0x398DC062, 0x235A7358, 0x0FEB6EF0,
    0x786DAF07, 0x5CB62965, 0x64B55357, 0xE37299E6, 0xDA753538, 0x0BEB54A7, 0x02B5B8BF,
    0xAEDF4E26, 0x97868BB6, 0x0CE81357, 0x253FB589, 0x70337DC6, 0x00B6D4F7, 0x6372750E,
    0xD3335ADB, 0xAD75CCCA, 0x5739AD60, 0x48193413, 0x3BAB1C54, 0x905ED2A5, 0xCA6449D4,
    0x49C3F4C5, 0x3AE9F85C, 0x317786CC, 0x2A788387, 0x7788310D, 0xDC838778, 0x88389B89,
    0x14A06647, 0x4A9540F1, 0x250B0110, 0x30495492, 0xB1757BF2, 0x02B8EB8D, 0xD78F78E7,
    0x9441B1F8, 0x45031944, 0xFCF1F8FD, 0x0C0791F8, 0xAB45EB8E, 0xDB168434, 0xE32A5BCF,
    0xBB845E6F, 0x5E5CD906, 0xA5DA3A03, 0x5E52C223, 0x0438E3BE, 0x83E25308, 0x48841EC1,
    0x911687A6, 0xC9B893F9, 0x99E4CF99, 0xB1C6ACEF, 0x2F023FB9, 0x3C92933C, 0x971D089B,
    0x07888AC5, 0x3665A665, 0x6447728A, 0x1B3721E6, 0x2BB8F84D, 0x3B8EF923, 0xD2205EE3,
    0x19474C51, 0xE2E9FA17, 0x78FFA378, 0xA4053B86, 0x2241446F, 0x0C5457DE, 0x88B8970F,
    0x9EBBBA73, 0x7A778AD8, 0xA80B0F8B, 0x693BF887, 0x5A4BA878, 0x29BA0B87, 0x23471E1B,
    0xD34B3974, 0x0C06B6C9, 0x0727399F, 0xC3FF06F1, 0x6213B53A, 0x1B9E39C9, 0xA5F08B64,
    0xC708B528, 0x05EE9C71, 0x0B4FB13B, 0x3CDD09C7, 0x3BAFEC22, 0xB979C73B, 0xB0FB2080,
    0xFDD30B4F, 0x433A4343, 0x3FC34C35, 0xF82FC2D4, 0xE55C3FC2, 0x93836892, 0x3B33B240,
    0xC3572433, 0xC2294FB8, 0x8944BB98, 0x9C3C51EB, 0xBC874FB9, 0x2C30C52B, 0xC2E5C4C3,
    0xB9C45430, 0xBC52836F, 0xA35BBD3B, 0xB93BA452, 0x6074FBCF, 0xC330FB03, 0x32C34834,
    0x44328E7D, 0x43FC3403, 0x33C34032, 0x4BC50FF8, 0xFC4FC5BC, 0xC3C32C3F, 0x0C30C603,
    0x8C542D06, 0x32432830, 0x1C3043FC, 0x0697C99A, 0x2C6327CA, 0x5C2E82F0, 0x3C883FF3,
    0xCA9863C9, 0xCDA1FCD3, 0x7CE3C9FC, 0xC97CE7CA, 0xEBCD59EF, 0xBCDCA3FC, 0xCF3D0BCE,
    0x13CFBD0F, 0xFCC53BBD, 0x2EC2C7CB, 0x046882E4, 0xFC8BD19C, 0xFD82F4A3, 0xC7CBBCB0,
    0x03403298, 0xC47C5FC4, 0x6D8C7C67, 0x7C43C4BD, 0xCB42CBA2, 0x4395D4A7, 0x7C00FB14,
    0xC01D9FBA, 0x3FB93BF3, 0x7C77149C, 0xB9FD2445, 0xD052BC17, 0xBBABC0BD, 0xC23BB50A,
    0x3BBC3C0C, 0xC74FDB09, 0x2C2BCFBB, 0x7C7FFD30, 0xD4D9EEB9, 0xB3BB4C87, 0x27B3FB47,
    0x2C3FB57B, 0xE3A3CA3C, 0x4FB03B53, 0xBE2FB5BE, 0xB13E4FB1, 0xFFC1F35B, 0x479D0846,
    0x2825FF2A, 0x0E31198B, 0xEE4D970A, 0xFD64375C, 0x142F060C, 0x9A6088C3, 0x620AFBEB,
    0xDBED5A61, 0xFED1A60F, 0xEC8FDBED, 0xD9E1BCBF, 0x7ED4AFBE, 0x043EFC79, 0x97F00792,
    0x7EC3EB87, 0xC642DC79, 0xFC2FFF2E, 0x3F3FF343, 0xF3BF4BF3, 0xBDD63F43, 0x84BFF2C4,
    0xF53F47F5, 0x5FF70307, 0x84BFF4BF, 0xF383002D, 0x18457BE7, 0x7F6FF61E, 0xFAFFA3FA,
    0xB7FABF4F, 0x5620E5BF, 0xFC9777F9, 0xD8E5BF6B, 0x56208F3F, 0x44BBEBFC, 0xBFFD3FEC,
    0xC400E348, 0xFEC362AE, 0xEC2622E9, 0xC161AE5F, 0xC612E1FE, 0x85C42150, 0x6C462709,
    0x45180C28, 0x662F1B89, 0xE2B1F8B4, 0x431E8744, 0x50F5CC72, 0x0C904A63, 0x35D4721D,
    0x8240A10C, 0x9CAA5B16, 0xC1E63068, 0xF2473B8E, 0xA7B298C4, 0x08C45DC5, 0xF1960AE2,
    0xFF5FB056, 0xCA3D2292, 0xA7542954, 0x4D2EA153, 0xCE2A356A, 0x76AB3082, 0x6BD52A6D,
    0xC754A354, 0xF54AC586, 0xBC9DD5EB, 0x27FB118A, 0xAD4EADF3, 0x8C0EE970, 0x5D6ED31B,
    0xB2B57082, 0x6FD76BDD, 0xDF7A9D41, 0xD73BB5E6, 0xC39D5EF0, 0x77FB2996, 0x7FBE1F0F,
    0xA6572993, 0x65B30F9C, 0x75FB93CD, 0xF40FB7FE, 0xF99AD166, 0x9A57FE95, 0x98CAE8DF,
    0x8CA6A9F3, 0x7BBDF1FA, 0x33EF4DA7, 0xEC9EDB0D, 0xE9F4F97F, 0xB4DB5CFB, 0x67B9D0BE,
    0x9C6D4EDB, 0x92C86A76, 0xFE4665F1, 0xE93DB4DC, 0x4F038FBF, 0xF2F7DBCD, 0x79F9DBE4,
    0xF4192D96, 0x43B5EE3E, 0x19ED3F7F, 0xFF7EF776, 0x3EC6B722, 0xE9EE353E, 0x7E9B6FE9,
    0x76FE1EDF, 0xF3F3E1F3, 0x3B9EAF37, 0xAC9322CF, 0x8BE8DF37, 0x0EDD3A2F, 0xDDB58EDC,
    0x37105B32, 0x3043EB05, 0xFBC4E135, 0xBCDBBD0F, 0x0B1FEC83, 0x438F2BFB, 0x6E6C0508,
    0x8BF2DEB7, 0x83083670, 0x003F1133, 0xBC10DBD3, 0x2FF416D0, 0x1BE30CC4, 0x55133DD1,
    0x7B3A4D4C, 0x458FB44E, 0x2D8C7712, 0x0466F146, 0x35173251, 0x0EC06F5C, 0x3B709C75,
    0x0D33FAE2, 0x835AE6BF, 0xA71E3AAE, 0x12BA92C3, 0x3612937D, 0xB2F450E4, 0x943CE44B,
    0x1AE44B92, 0x0735CD84, 0xB3B36B5B, 0xCD13F8F5, 0xA9FF3A9F, 0xED3B9FD3, 0x3D4F105C,
    0xC1B3ECF3, 0x33B5053E, 0x6B42CFCE, 0xE942BF93, 0xD74E73B4, 0x1FAD09F8, 0xB02CDAD3,
    0x3BD13BCD, 0xFAFE35D4, 0x46CF345C, 0xBC547B55, 0xD49B5348, 0xC36F3844, 0xED4D3554,
    0xF150D4E4, 0x1FCD84DA, 0xCDF548CF, 0xAD44D4D4, 0x3D385294, 0x44B474F3, 0x4FACDD65,
    0xF503455E, 0xAD675B9F, 0x88F5D634, 0x4DB43445, 0xD5146581, 0xD63588D1, 0xBB5D5CB5,
    0x9B4CD975, 0x4ED9F55D, 0xB4D61DA3, 0xF52D7164, 0x55A8D35A, 0x0AD55ACD, 0x5F337615,
    0x5D9563D7, 0xD583F8CD, 0x0CF341CE, 0xF37CDEB4, 0x3FDB775C, 0xE0106CED, 0x326C9ACC,
    0x9332F760, 0xDD82B66D, 0x85E1185C, 0xE1586E15, 0x588B7587, 0xB3D8C8E0, 0x20D6E298,
    0x8BE32CCE, 0x6378EB25, 0x78664583, 0x6E4D8E62, 0x4F8C606D, 0x95E13926, 0x63F94C1B,
    0x792E0F97, 0xAE5B8964, 0x7198E2B9, 0x8764391E, 0x67D8F673, 0x9A0E699E, 0xEC1BA265,
    0x15A1E419, 0x99E8D986, 0x651A6E6F, 0xED8E7795, 0x5EABA4E9, 0xA1AB6999, 0x99609A06,
    0x6BDA5635, 0x7AEEB7AA, 0x1EC1A3EA, 0x91B0EB39, 0xA2ECFB56, 0x6AFB363B, 0x9B262DB4,
    0xA6A7B5E8, 0xA157C1BB, 0x6BEF336C, 0x34774B3D, 0xD6ED154A, 0x35B55A49, 0xFAD75252,
    0x6FD77756, 0x59D4A337, 0x5C04FACD, 0x571379B4, 0xC969716D, 0x135AB573, 0x726CB723,
    0xF4855372, 0xD333CC57, 0x334AC29B, 0xCA50AD17, 0x7071371F, 0x5D74EDC9, 0x1F2F195E,
    0xF8CDF56D, 0xCF5D1CE5, 0x4F3D95B9, 0x7CB32DDA, 0x774FC554, 0x7D71F374, 0xD0B2BCFB,
    0xD43D1F0D, 0xFE3F71D2, 0x7F8DBDD3, 0x84F5EFD3, 0xE9704CAF, 0xF45D9F9B, 0xDE2F6D5E,
    0x9719EC74, 0xF34AF1D3, 0xD1FA9BE3, 0xB45F0F5A, 0x7C25DFEE, 0x5788CA7B, 0x1BD67DBD,
    0xDF703DE7, 0x7EB564F3, 0xFB1FB993, 0xE3B9A7CC, 0x9032CFA4, 0xC0E81DFB, 0x45F13CF7,
    0xABE57E6F, 0x580882D4, 0x3039F7A9, 0x1EF3B07E, 0xA9579C65, 0x82CF5E01, 0x9F9E53FB,
    0x6641D346, 0xD37C8E21, 0x0EB8202B, 0x2E0CBFC8, 0x0BDF816F, 0x45DAFC1E, 0x70AA08BA,
    0xA093D682, 0x4347B4AE, 0x84AFC8CD, 0xF8E0F40D, 0xDD42B7F0, 0xA47D5089, 0xDB0E21D1,
    0xC1CEC264, 0x1C4F7FF0, 0x888B0A22, 0xF0B9682C, 0xE2941875, 0x4153B709, 0x846AB5D4,
    0x14DC2C49, 0xE445E849, 0xAA8CCEA5, 0xA0EF6271, 0x75C3C618, 0xBC4D8411, 0x17D51BA2,
    0x29B23040, 0xE304678E, 0x4768C4B1, 0x87714555, 0x2063C47A, 0x63C428D5, 0x648B2F31,
    0x5D13241C, 0xF2042C13, 0xA1A19207, 0x2922A514, 0xA934FE1E, 0x14A46092, 0xC9934326,
    0x93B25126, 0x270F14A1, 0x85CA292D, 0xA994B281, 0xF9265A04, 0xB106A943, 0x04A43432,
    0x062D27A3, 0x0834F14B, 0x658CB395, 0x4AD94D2D, 0x97B2BD13, 0x2E922CBB, 0x4CCB2955,
    0xB644D2E2, 0xC530655C, 0x3264C933, 0xF4AA9973, 0x698C6FA5, 0x533E4FCA, 0x2624C29A,
    0x1BE99118, 0x95535665, 0x340E74AE, 0x72CDB9C1, 0xDB9BA7F2, 0xB32F24BC, 0x8E5FCCD9,
    0x54D19DB3, 0x999536A7, 0x73B26FCE, 0x26F4CE9B, 0xC999F139, 0x9AD3F64D, 0x2D25CB16,
    0xCCCC7987, 0x05A0340C, 0xAF38A535, 0xE921CA64, 0x69A947F3, 0x948343D2, 0xE6C12824,
    0xD282069A, 0x3A9A2D3A, 0x4873A4D8, 0x8AE8BD13, 0x91A6B37C, 0x479F144A, 0x86C5A51A,
    0x0293D204, 0x7D25A28D, 0xA9654BD0, 0xD4BE9852, 0x12A51EA5, 0xD10A5F4B, 0xA754F290,
    0x4468AA15, 0x9B294681, 0xA7A8348A, 0x9152527C, 0x12861F9A, 0xD51BA6B5, 0xBA0F4B66,
    0x7534FCD4, 0xE9FD4F41, 0xD40A8F30, 0xAAF4E4FC, 0x89AACD5E, 0xB1534A8C, 0x1D9FE942,
    0xAC812B0D, 0xE2A9A61A, 0x651AAAB4, 0xA75359EA, 0x10B225D5, 0xDF74848E, 0x40987CDF,
    0x85F0DE0D, 0x689DBC8A, 0xDD99A772, 0x95B111D1, 0x0D1A6223, 0x262DC7B8, 0xAC368FD1,
    0x491717E2, 0xB11DFCD8, 0xE1FC5A87, 0x435B135F, 0x7F56322A, 0x66E2C3E8, 0xD2BFDB35,
    0x7A83E656, 0xFCBC6BC5, 0x7244DA7A, 0xAC8C7631, 0x78F31A1E, 0x2B1E31BE, 0x62E581B6,
    0x58533765, 0x8C561CCB, 0x6D60A195, 0x11D99AF7, 0x868396E6, 0x356F237C, 0x00CADBE3,
    0x9C888A57, 0x981F1ACC, 0x9636D957, 0x5FF3DCB5, 0x5AE86710, 0xB4164A01, 0x6AAE199B,
    0x295B7B31, 0xCABA7722, 0x2D759E85, 0x22C3446B, 0x4C31BA31, 0x9B05776E, 0xB0E6F6C2,
    0x2E942385, 0xC10B2309, 0xB0720AC9, 0x7BECBDBD, 0x0DDB8B8D, 0xBBBE97A6, 0x0773700D,
    0x06F85BFC, 0x0DC47D77, 0x58D979AD, 0xB7F303AC, 0xAC5DA8AF, 0x5D9BCB84, 0x901028CA,
    0x72F04C0C, 0xE7196C17, 0x877926BE, 0x29822F94, 0x9AF45D5C, 0xBDC2C1B8, 0xEBDD80AB,
    0xF79F02E0, 0xEC55D4C2, 0x604C3967, 0xBF7856C7, 0x7B2CF63A, 0x1FDF8C69, 0xF48CB69B,
    0xC0FDB0CB, 0x8CCB63DB, 0xB613BAF7, 0xB8011023, 0x98F2EFE4, 0x6D663EBC, 0x00000040,
    0x00000000

  EW_BITMAP_CLUT_EMPTY( CutterPaper, Default )

  EW_BITMAPS_TABLE( CutterPaper )
    EW_BITMAP( CutterPaper, Default )
EW_END_OF_BITMAP_RES( CutterPaper )

/* Table with links to derived variants of the bitmap resource : 'Cutter::Paper' */
EW_RES_WITHOUT_VARIANTS( CutterPaper )

/* Initializer for the class 'Cutter::PaperCut' */
void CutterPaperCut__Init( CutterPaperCut _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->Image, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->BorderBottom, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->BorderTop, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->BorderLeft, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->BorderRight, &_this->_XObject, 0 );
  CutterVertSlider__Init( &_this->VertSlider, &_this->_XObject, 0 );
  CutterHorzSlider__Init( &_this->HorzSlider, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeEffect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterPaperCut );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const004A );
  CoreRectView__OnSetBounds( &_this->Image, _Const004A );
  ViewsImage_OnSetAutoSize( &_this->Image, 1 );
  CoreRectView__OnSetBounds( &_this->BorderBottom, _Const004B );
  ViewsRectangle_OnSetColor( &_this->BorderBottom, CutterTextColor );
  CoreRectView__OnSetBounds( &_this->BorderTop, _Const004C );
  ViewsRectangle_OnSetColor( &_this->BorderTop, CutterTextColor );
  CoreRectView__OnSetBounds( &_this->BorderLeft, _Const004D );
  ViewsRectangle_OnSetColor( &_this->BorderLeft, CutterTextColor );
  CoreRectView__OnSetBounds( &_this->BorderRight, _Const004E );
  ViewsRectangle_OnSetColor( &_this->BorderRight, CutterTextColor );
  CoreRectView__OnSetBounds( &_this->VertSlider, _Const004F );
  CutterVertSlider_OnSetCurrentValue( &_this->VertSlider, 740 );
  CutterVertSlider_OnSetMinValue( &_this->VertSlider, 740 );
  CutterVertSlider_OnSetMaxValue( &_this->VertSlider, 4200 );
  CoreRectView__OnSetBounds( &_this->HorzSlider, _Const0050 );
  CutterHorzSlider_OnSetCurrentValue( &_this->HorzSlider, 2970 );
  CutterHorzSlider_OnSetMinValue( &_this->HorzSlider, 520 );
  CutterHorzSlider_OnSetMaxValue( &_this->HorzSlider, 2970 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeEffect, 300 );
  CoreGroup__Add( _this, ((CoreView)&_this->Image ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BorderBottom ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BorderTop ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BorderLeft ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BorderRight ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->VertSlider ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->HorzSlider ), 0 );
  ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &CutterPaper, ResourcesBitmap ));
  CutterVertSlider_OnSetOutlet( &_this->VertSlider, EwNewRef( EwGetAutoObject( &CutterDevice, 
  CutterDeviceClass ), CutterDeviceClass_OnGetPaperHeight, CutterDeviceClass_OnSetPaperHeight ));
  CutterHorzSlider_OnSetOutlet( &_this->HorzSlider, EwNewRef( EwGetAutoObject( &CutterDevice, 
  CutterDeviceClass ), CutterDeviceClass_OnGetPaperWidth, CutterDeviceClass_OnSetPaperWidth ));
  _this->FadeEffect.Outlet = EwNewRef( _this, CoreGroup_OnGetOpacity, CoreGroup__OnSetOpacity );
}

/* Re-Initializer for the class 'Cutter::PaperCut' */
void CutterPaperCut__ReInit( CutterPaperCut _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->Image );
  ViewsRectangle__ReInit( &_this->BorderBottom );
  ViewsRectangle__ReInit( &_this->BorderTop );
  ViewsRectangle__ReInit( &_this->BorderLeft );
  ViewsRectangle__ReInit( &_this->BorderRight );
  CutterVertSlider__ReInit( &_this->VertSlider );
  CutterHorzSlider__ReInit( &_this->HorzSlider );
  EffectsInt32Effect__ReInit( &_this->FadeEffect );
}

/* Finalizer method for the class 'Cutter::PaperCut' */
void CutterPaperCut__Done( CutterPaperCut _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterPaperCut );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->Image );
  ViewsRectangle__Done( &_this->BorderBottom );
  ViewsRectangle__Done( &_this->BorderTop );
  ViewsRectangle__Done( &_this->BorderLeft );
  ViewsRectangle__Done( &_this->BorderRight );
  CutterVertSlider__Done( &_this->VertSlider );
  CutterHorzSlider__Done( &_this->HorzSlider );
  EffectsInt32Effect__Done( &_this->FadeEffect );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Cutter::PaperCut.OnSetPaperWidth()' */
void CutterPaperCut_OnSetPaperWidth( CutterPaperCut _this, XRef value )
{
  CutterHorzSlider_OnSetOutlet( &_this->HorzSlider, value );

  if ( !EwCompRef( value, _this->PaperWidth ))
    return;

  if ( _this->PaperWidth.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterPaperCut_onUpdate ), _this->PaperWidth, 
      0 );

  _this->PaperWidth = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterPaperCut_onUpdate ), value, 0 );

  EwPostSignal( EwNewSlot( _this, CutterPaperCut_onUpdate ), ((XObject)_this ));
}

/* 'C' function for method : 'Cutter::PaperCut.OnSetPaperHeight()' */
void CutterPaperCut_OnSetPaperHeight( CutterPaperCut _this, XRef value )
{
  CutterVertSlider_OnSetOutlet( &_this->VertSlider, value );

  if ( !EwCompRef( value, _this->PaperHeight ))
    return;

  if ( _this->PaperHeight.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterPaperCut_onUpdate ), _this->PaperHeight, 
      0 );

  _this->PaperHeight = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterPaperCut_onUpdate ), value, 0 );

  EwPostSignal( EwNewSlot( _this, CutterPaperCut_onUpdate ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void CutterPaperCut_onUpdate( CutterPaperCut _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperWidth.Object != 0 )
  {
    XInt32 x = (( EwOnGetInt32( _this->PaperWidth ) / 20 ) * 168 ) / 297;
    CoreRectView__OnSetBounds( &_this->BorderLeft, EwSetRectOrigin( _this->BorderLeft.Super1.Bounds, 
    EwSetPointX( _this->BorderLeft.Super1.Bounds.Point1, 96 - x )));
    CoreRectView__OnSetBounds( &_this->BorderRight, EwSetRectOrigin( _this->BorderRight.Super1.Bounds, 
    EwSetPointX( _this->BorderRight.Super1.Bounds.Point1, 96 + x )));
  }

  if ( _this->PaperHeight.Object != 0 )
  {
    XInt32 y = 257 - ((( EwOnGetInt32( _this->PaperHeight ) / 10 ) * 241 ) / 420 );
    CoreRectView__OnSetBounds( &_this->BorderTop, EwSetRectOrigin( _this->BorderTop.Super1.Bounds, 
    EwSetPointY( _this->BorderTop.Super1.Bounds.Point1, y )));
  }
}

/* Slot method to start fade-in of component. */
void CutterPaperCut_FadeIn( CutterPaperCut _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeEffect, 0 );
  _this->FadeEffect.Value2 = 255;
  CoreGroup_OnSetEnabled((CoreGroup)&_this->HorzSlider, 1 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->VertSlider, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeEffect, 1 );
}

/* Slot method to start fade-out of component. */
void CutterPaperCut_FadeOut( CutterPaperCut _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Super1.Opacity == 0 )
    return;

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeEffect, 0 );
  _this->FadeEffect.Value2 = 0;
  CoreGroup_OnSetEnabled((CoreGroup)&_this->HorzSlider, 0 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->VertSlider, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeEffect, 1 );
}

/* Variants derived from the class : 'Cutter::PaperCut' */
EW_DEFINE_CLASS_VARIANTS( CutterPaperCut )
EW_END_OF_CLASS_VARIANTS( CutterPaperCut )

/* Virtual Method Table (VMT) for the class : 'Cutter::PaperCut' */
EW_DEFINE_CLASS( CutterPaperCut, CoreGroup, PaperWidth, PaperWidth, PaperWidth, 
                 Image, _None, "Cutter::PaperCut" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterPaperCut )

/* Initializer for the class 'Cutter::VertSlider' */
void CutterVertSlider__Init( CutterVertSlider _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->Thumb, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandler, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterVertSlider );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0051 );
  CoreView_OnSetLayout((CoreView)&_this->Thumb, 0 );
  CoreRectView__OnSetBounds( &_this->Thumb, _Const0052 );
  ViewsImage_OnSetAutoSize( &_this->Thumb, 1 );
  ViewsImage_OnSetColor( &_this->Thumb, CutterTextColor );
  ViewsImage_OnSetAlignment( &_this->Thumb, ViewsImageAlignmentAlignHorzCenter | 
  ViewsImageAlignmentAlignVertCenter );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, _Const0053 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, _Const0054 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, _Const0055 );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, _Const0056 );
  _this->CurrentValue = 50;
  _this->MaxValue = 100;
  CoreGroup__Add( _this, ((CoreView)&_this->Thumb ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
  ViewsImage_OnSetBitmap( &_this->Thumb, EwLoadResource( &CutterThumb, ResourcesBitmap ));
  _this->SimpleTouchHandler.OnDrag = EwNewSlot( _this, CutterVertSlider_onDragSlot );
  _this->SimpleTouchHandler.OnRelease = EwNewSlot( _this, CutterVertSlider_onReleaseSlot );
  _this->SimpleTouchHandler.OnPress = EwNewSlot( _this, CutterVertSlider_onPressSlot );
}

/* Re-Initializer for the class 'Cutter::VertSlider' */
void CutterVertSlider__ReInit( CutterVertSlider _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->Thumb );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandler );
}

/* Finalizer method for the class 'Cutter::VertSlider' */
void CutterVertSlider__Done( CutterVertSlider _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterVertSlider );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->Thumb );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandler );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
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
void CutterVertSlider_UpdateViewState( CutterVertSlider _this, XSet aState )
{
  XInt32 pos = 0;

  if ((( _this->MaxValue > _this->MinValue ) && ( _this->CurrentValue >= _this->MinValue )) 
      && ( _this->CurrentValue <= _this->MaxValue ))
  {
    pos = (( EwGetRectH( _this->Super2.Bounds ) - EwGetRectH( _this->Thumb.Super1.Bounds )) 
    * ( _this->CurrentValue - _this->MinValue )) / ( _this->MaxValue - _this->MinValue );
  }

  CoreRectView__OnSetBounds( &_this->Thumb, EwSetRectOrigin( _this->Thumb.Super1.Bounds, 
  EwSetPointY( _this->Thumb.Super1.Bounds.Point1, ( EwGetRectH( _this->Super2.Bounds ) 
  - pos ) - EwGetRectH( _this->Thumb.Super1.Bounds ))));
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, EwNewPoint( 
  _this->Thumb.Super1.Bounds.Point1.X, _this->Thumb.Super1.Bounds.Point1.Y ));
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, EwNewPoint( 
  _this->Thumb.Super1.Bounds.Point2.X, _this->Thumb.Super1.Bounds.Point1.Y ));
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, EwNewPoint( 
  _this->Thumb.Super1.Bounds.Point2.X, _this->Thumb.Super1.Bounds.Point2.Y ));
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, EwNewPoint( 
  _this->Thumb.Super1.Bounds.Point1.X, _this->Thumb.Super1.Bounds.Point2.Y ));

  if ((( aState & CoreViewStateEnabled ) == CoreViewStateEnabled ))
  {
    ViewsImage_OnSetVisible( &_this->Thumb, 1 );

    if ( _this->SimpleTouchHandler.Down )
    {
      ViewsImage_OnSetColor( &_this->Thumb, CutterSelectColor );
    }
    else
    {
      ViewsImage_OnSetColor( &_this->Thumb, CutterTextColor );
    }
  }
  else
    ViewsImage_OnSetVisible( &_this->Thumb, 0 );
}

/* This is a slot method. */
void CutterVertSlider_onDragSlot( CutterVertSlider _this, XObject sender )
{
  XInt32 oldValue;
  XInt32 ofs;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldValue = _this->CurrentValue;
  ofs = _this->SimpleTouchHandler.HittingPos.Y - _this->SimpleTouchHandler.CurrentPos.Y;

  if ((( _this->MaxValue > _this->MinValue ) && ( _this->CurrentValue >= _this->MinValue )) 
      && ( _this->CurrentValue <= _this->MaxValue ))
  {
    ofs = ( ofs * ( _this->MaxValue - _this->MinValue )) / ( EwGetRectH( _this->Super2.Bounds ) 
    - EwGetRectH( _this->Thumb.Super1.Bounds ));
  }
  else
    ofs = 0;

  CutterVertSlider_OnSetCurrentValue( _this, _this->startValue + ofs );

  if ( _this->CurrentValue != oldValue )
  {
    if ( _this->Outlet.Object != 0 )
      EwOnSetInt32( _this->Outlet, _this->CurrentValue );

    EwNotifyRefObservers( _this->Outlet, 0 );
    EwIdleSignal( _this->OnChange, ((XObject)_this ));
  }
}

/* 'C' function for method : 'Cutter::VertSlider.OnSetCurrentValue()' */
void CutterVertSlider_OnSetCurrentValue( CutterVertSlider _this, XInt32 value )
{
  if ( value < _this->MinValue )
    value = _this->MinValue;

  if ( value > _this->MaxValue )
    value = _this->MaxValue;

  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Cutter::VertSlider.OnSetMinValue()' */
void CutterVertSlider_OnSetMinValue( CutterVertSlider _this, XInt32 value )
{
  if ( _this->MinValue == value )
    return;

  _this->MinValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Cutter::VertSlider.OnSetMaxValue()' */
void CutterVertSlider_OnSetMaxValue( CutterVertSlider _this, XInt32 value )
{
  if ( _this->MaxValue == value )
    return;

  _this->MaxValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void CutterVertSlider_outletSlot( CutterVertSlider _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    CutterVertSlider_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'Cutter::VertSlider.OnSetOutlet()' */
void CutterVertSlider_OnSetOutlet( CutterVertSlider _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  EwDetachRefObserver( EwNewSlot( _this, CutterVertSlider_outletSlot ), _this->Outlet, 
    0 );
  _this->Outlet = value;
  EwAttachRefObserver( EwNewSlot( _this, CutterVertSlider_outletSlot ), value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, CutterVertSlider_outletSlot ), ((XObject)_this ));
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterVertSlider_onPressSlot( CutterVertSlider _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  _this->startValue = _this->CurrentValue;
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterVertSlider_onReleaseSlot( CutterVertSlider _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnApply, ((XObject)_this ));
}

/* Variants derived from the class : 'Cutter::VertSlider' */
EW_DEFINE_CLASS_VARIANTS( CutterVertSlider )
EW_END_OF_CLASS_VARIANTS( CutterVertSlider )

/* Virtual Method Table (VMT) for the class : 'Cutter::VertSlider' */
EW_DEFINE_CLASS( CutterVertSlider, CoreGroup, OnApply, OnApply, Outlet, Thumb, startValue, 
                 "Cutter::VertSlider" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CutterVertSlider_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterVertSlider )

/* Initializer for the class 'Cutter::HorzSlider' */
void CutterHorzSlider__Init( CutterHorzSlider _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->Thumb, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandler, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterHorzSlider );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0057 );
  CoreView_OnSetLayout((CoreView)&_this->Thumb, 0 );
  CoreRectView__OnSetBounds( &_this->Thumb, _Const0058 );
  ViewsImage_OnSetAutoSize( &_this->Thumb, 1 );
  ViewsImage_OnSetColor( &_this->Thumb, CutterTextColor );
  ViewsImage_OnSetAlignment( &_this->Thumb, ViewsImageAlignmentAlignHorzCenter | 
  ViewsImageAlignmentAlignVertCenter );
  _this->CurrentValue = 50;
  _this->MaxValue = 100;
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, _Const0059 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, _Const005A );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, _Const005B );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, _Const005C );
  CoreGroup__Add( _this, ((CoreView)&_this->Thumb ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
  ViewsImage_OnSetBitmap( &_this->Thumb, EwLoadResource( &CutterThumb, ResourcesBitmap ));
  _this->SimpleTouchHandler.OnDrag = EwNewSlot( _this, CutterHorzSlider_onDragSlot );
  _this->SimpleTouchHandler.OnRelease = EwNewSlot( _this, CutterHorzSlider_onReleaseSlot );
  _this->SimpleTouchHandler.OnPress = EwNewSlot( _this, CutterHorzSlider_onPressSlot );
}

/* Re-Initializer for the class 'Cutter::HorzSlider' */
void CutterHorzSlider__ReInit( CutterHorzSlider _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->Thumb );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandler );
}

/* Finalizer method for the class 'Cutter::HorzSlider' */
void CutterHorzSlider__Done( CutterHorzSlider _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterHorzSlider );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->Thumb );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandler );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
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
void CutterHorzSlider_UpdateViewState( CutterHorzSlider _this, XSet aState )
{
  XInt32 pos = 0;

  if ((( _this->MaxValue > _this->MinValue ) && ( _this->CurrentValue >= _this->MinValue )) 
      && ( _this->CurrentValue <= _this->MaxValue ))
  {
    pos = (( EwGetRectW( _this->Super2.Bounds ) - EwGetRectW( _this->Thumb.Super1.Bounds )) 
    * ( _this->CurrentValue - _this->MinValue )) / ( _this->MaxValue - _this->MinValue );
  }

  CoreRectView__OnSetBounds( &_this->Thumb, EwSetRectOrigin( _this->Thumb.Super1.Bounds, 
  EwSetPointX( _this->Thumb.Super1.Bounds.Point1, pos )));
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, EwNewPoint( 
  _this->Thumb.Super1.Bounds.Point1.X, _this->Thumb.Super1.Bounds.Point1.Y ));
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, EwNewPoint( 
  _this->Thumb.Super1.Bounds.Point2.X, _this->Thumb.Super1.Bounds.Point1.Y ));
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, EwNewPoint( 
  _this->Thumb.Super1.Bounds.Point2.X, _this->Thumb.Super1.Bounds.Point2.Y ));
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, EwNewPoint( 
  _this->Thumb.Super1.Bounds.Point1.X, _this->Thumb.Super1.Bounds.Point2.Y ));

  if ((( aState & CoreViewStateEnabled ) == CoreViewStateEnabled ))
  {
    ViewsImage_OnSetVisible( &_this->Thumb, 1 );

    if ( _this->SimpleTouchHandler.Down )
    {
      ViewsImage_OnSetColor( &_this->Thumb, CutterSelectColor );
    }
    else
    {
      ViewsImage_OnSetColor( &_this->Thumb, CutterTextColor );
    }
  }
  else
    ViewsImage_OnSetVisible( &_this->Thumb, 0 );
}

/* 'C' function for method : 'Cutter::HorzSlider.OnSetCurrentValue()' */
void CutterHorzSlider_OnSetCurrentValue( CutterHorzSlider _this, XInt32 value )
{
  if ( value < _this->MinValue )
    value = _this->MinValue;

  if ( value > _this->MaxValue )
    value = _this->MaxValue;

  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Cutter::HorzSlider.OnSetMinValue()' */
void CutterHorzSlider_OnSetMinValue( CutterHorzSlider _this, XInt32 value )
{
  if ( _this->MinValue == value )
    return;

  _this->MinValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Cutter::HorzSlider.OnSetMaxValue()' */
void CutterHorzSlider_OnSetMaxValue( CutterHorzSlider _this, XInt32 value )
{
  if ( _this->MaxValue == value )
    return;

  _this->MaxValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterHorzSlider_onPressSlot( CutterHorzSlider _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  _this->startValue = _this->CurrentValue;
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterHorzSlider_onDragSlot( CutterHorzSlider _this, XObject sender )
{
  XInt32 oldValue;
  XInt32 ofs;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldValue = _this->CurrentValue;
  ofs = _this->SimpleTouchHandler.CurrentPos.X - _this->SimpleTouchHandler.HittingPos.X;

  if ((( _this->MaxValue > _this->MinValue ) && ( _this->CurrentValue >= _this->MinValue )) 
      && ( _this->CurrentValue <= _this->MaxValue ))
  {
    ofs = ( ofs * ( _this->MaxValue - _this->MinValue )) / ( EwGetRectW( _this->Super2.Bounds ) 
    - EwGetRectW( _this->Thumb.Super1.Bounds ));
  }
  else
    ofs = 0;

  CutterHorzSlider_OnSetCurrentValue( _this, _this->startValue + ofs );

  if ( _this->CurrentValue != oldValue )
  {
    if ( _this->Outlet.Object != 0 )
      EwOnSetInt32( _this->Outlet, _this->CurrentValue );

    EwNotifyRefObservers( _this->Outlet, 0 );
    EwIdleSignal( _this->OnChange, ((XObject)_this ));
  }
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void CutterHorzSlider_outletSlot( CutterHorzSlider _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    CutterHorzSlider_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'Cutter::HorzSlider.OnSetOutlet()' */
void CutterHorzSlider_OnSetOutlet( CutterHorzSlider _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  EwDetachRefObserver( EwNewSlot( _this, CutterHorzSlider_outletSlot ), _this->Outlet, 
    0 );
  _this->Outlet = value;
  EwAttachRefObserver( EwNewSlot( _this, CutterHorzSlider_outletSlot ), value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, CutterHorzSlider_outletSlot ), ((XObject)_this ));
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterHorzSlider_onReleaseSlot( CutterHorzSlider _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnApply, ((XObject)_this ));
}

/* Variants derived from the class : 'Cutter::HorzSlider' */
EW_DEFINE_CLASS_VARIANTS( CutterHorzSlider )
EW_END_OF_CLASS_VARIANTS( CutterHorzSlider )

/* Virtual Method Table (VMT) for the class : 'Cutter::HorzSlider' */
EW_DEFINE_CLASS( CutterHorzSlider, CoreGroup, OnApply, OnApply, Outlet, Thumb, CurrentValue, 
                 "Cutter::HorzSlider" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CutterHorzSlider_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterHorzSlider )

/* This is a bitmap resource. */
EW_DEFINE_BITMAP_RES( CutterThumb )
  EW_BITMAP_FRAMES( CutterThumb, Default, EW_DRIVER_VARIANT_ALPHA8, 21, 21, 0 )
    EW_BITMAP_FRAME( 5, 5, 12, 12, 0x00000000, 0x00000000 )

  EW_BITMAP_PIXEL( CutterThumb, Default )                       /* ratio 57.14 % */
    0x1C080100, 0x83C0A008, 0x5030190A, 0xA0803058, 0x20068143, 0x4190B850, 0x160C8643,
    0xF0102C26, 0x0A80C010, 0x390C3816, 0x329A0BE4, 0x58606858, 0xE1C0F0C0, 0x6E150900,
    0xB347FAC0, 0x7A74BFE6, 0x3968503E, 0x228502B0, 0xFD0A6CE7, 0x84446AD7, 0xD02F1006,
    0x28743728, 0x02140D27, 0xE1E08000, 0x5EA6D356, 0x40303824, 0xABBF88E1, 0xB8719343,
    0x23C2E0F0, 0x05F143B0, 0x43216089, 0xC743B430, 0x0D48A84C, 0x7337D0AE, 0x6BB33835,
    0x0A04CD93, 0xADF16218, 0x85A34589, 0xB5FF8338, 0xC0612183, 0x06F21984, 0x0170B28B,
    0x7E0B8300, 0x11CBC6F8, 0x0C478101, 0xDDD9561D, 0xC0C2A142, 0x804A1307, 0x4C2F1B02,
    0x08060886, 0x21B09042, 0x367BB18B, 0x41B269D3, 0x8781C241, 0x107B4A0E, 0x1A4DA612,
    0x4C32312C, 0x23BB10A8, 0x24110400, 0x413E984C, 0x0187012B, 0x00000001, 0x00000000

  EW_BITMAP_CLUT_EMPTY( CutterThumb, Default )

  EW_BITMAPS_TABLE( CutterThumb )
    EW_BITMAP( CutterThumb, Default )
EW_END_OF_BITMAP_RES( CutterThumb )

/* Table with links to derived variants of the bitmap resource : 'Cutter::Thumb' */
EW_RES_WITHOUT_VARIANTS( CutterThumb )

/* Initializer for the class 'Cutter::FormatDialog' */
void CutterFormatDialog__Init( CutterFormatDialog _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  CutterButton__Init( &_this->ButtonA3, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextW, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextH, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextWidth, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextHeight, &_this->_XObject, 0 );
  CutterCancelButton__Init( &_this->CancelButton, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextW0, &_this->_XObject, 0 );
  CutterButton__Init( &_this->ButtonA4, &_this->_XObject, 0 );
  CutterButton__Init( &_this->ButtonA5, &_this->_XObject, 0 );
  CutterButton__Init( &_this->ButtonA6, &_this->_XObject, 0 );
  CutterButton__Init( &_this->ButtonA7, &_this->_XObject, 0 );
  CutterButton__Init( &_this->ButtonA8, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterFormatDialog );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const005D );
  CoreView_OnSetLayout((CoreView)&_this->ButtonA3, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->ButtonA3, _Const005E );
  CutterButton_OnSetCaption( &_this->ButtonA3, EwLoadString( &_Const005F ));
  CoreView_OnSetLayout((CoreView)&_this->TextW, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->TextW, _Const0060 );
  ViewsText_OnSetWrapText( &_this->TextW, 0 );
  ViewsText_OnSetAlignment( &_this->TextW, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextW, EwLoadString( &_Const0061 ));
  ViewsText_OnSetColor( &_this->TextW, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->TextH, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->TextH, _Const0062 );
  ViewsText_OnSetWrapText( &_this->TextH, 0 );
  ViewsText_OnSetAlignment( &_this->TextH, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextH, EwLoadString( &_Const0063 ));
  ViewsText_OnSetColor( &_this->TextH, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->TextWidth, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->TextWidth, _Const0064 );
  ViewsText_OnSetWrapText( &_this->TextWidth, 0 );
  ViewsText_OnSetAlignment( &_this->TextWidth, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextWidth, EwLoadString( &_Const0019 ));
  ViewsText_OnSetColor( &_this->TextWidth, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->TextHeight, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->TextHeight, _Const0065 );
  ViewsText_OnSetWrapText( &_this->TextHeight, 0 );
  ViewsText_OnSetAlignment( &_this->TextHeight, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextHeight, EwLoadString( &_Const0019 ));
  ViewsText_OnSetColor( &_this->TextHeight, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->CancelButton, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->CancelButton, _Const0066 );
  CoreView_OnSetLayout((CoreView)&_this->TextW0, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->TextW0, _Const0067 );
  ViewsText_OnSetWrapText( &_this->TextW0, 0 );
  ViewsText_OnSetAlignment( &_this->TextW0, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextW0, EwLoadString( &_Const0068 ));
  ViewsText_OnSetColor( &_this->TextW0, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->ButtonA4, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->ButtonA4, _Const0069 );
  CutterButton_OnSetCaption( &_this->ButtonA4, EwLoadString( &_Const006A ));
  CoreView_OnSetLayout((CoreView)&_this->ButtonA5, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->ButtonA5, _Const006B );
  CutterButton_OnSetCaption( &_this->ButtonA5, EwLoadString( &_Const006C ));
  CoreView_OnSetLayout((CoreView)&_this->ButtonA6, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->ButtonA6, _Const006D );
  CutterButton_OnSetCaption( &_this->ButtonA6, EwLoadString( &_Const006E ));
  CoreView_OnSetLayout((CoreView)&_this->ButtonA7, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->ButtonA7, _Const006F );
  CutterButton_OnSetCaption( &_this->ButtonA7, EwLoadString( &_Const0070 ));
  CoreView_OnSetLayout((CoreView)&_this->ButtonA8, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->ButtonA8, _Const0071 );
  CutterButton_OnSetCaption( &_this->ButtonA8, EwLoadString( &_Const0072 ));
  CoreGroup__Add( _this, ((CoreView)&_this->ButtonA3 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextW ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextWidth ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextHeight ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->CancelButton ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextW0 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->ButtonA4 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->ButtonA5 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->ButtonA6 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->ButtonA7 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->ButtonA8 ), 0 );
  _this->ButtonA3.OnChange = EwNewSlot( _this, CutterFormatDialog_selectA3 );
  ViewsText_OnSetFont( &_this->TextW, EwLoadResource( &CutterDescrFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextH, EwLoadResource( &CutterDescrFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextWidth, EwLoadResource( &CutterMediumDigitFont, 
  ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextHeight, EwLoadResource( &CutterMediumDigitFont, 
  ResourcesFont ));
  _this->CancelButton.OnChange = EwNewSlot( _this, CutterFormatDialog_onCancel );
  ViewsText_OnSetFont( &_this->TextW0, EwLoadResource( &CutterDescrFont, ResourcesFont ));
  _this->ButtonA4.OnChange = EwNewSlot( _this, CutterFormatDialog_selectA4 );
  _this->ButtonA5.OnChange = EwNewSlot( _this, CutterFormatDialog_selectA5 );
  _this->ButtonA6.OnChange = EwNewSlot( _this, CutterFormatDialog_selectA6 );
  _this->ButtonA7.OnChange = EwNewSlot( _this, CutterFormatDialog_selectA7 );
  _this->ButtonA8.OnChange = EwNewSlot( _this, CutterFormatDialog_selectA8 );

  /* Call the user defined constructor */
  CutterFormatDialog_Init( _this, aArg );
}

/* Re-Initializer for the class 'Cutter::FormatDialog' */
void CutterFormatDialog__ReInit( CutterFormatDialog _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CutterButton__ReInit( &_this->ButtonA3 );
  ViewsText__ReInit( &_this->TextW );
  ViewsText__ReInit( &_this->TextH );
  ViewsText__ReInit( &_this->TextWidth );
  ViewsText__ReInit( &_this->TextHeight );
  CutterCancelButton__ReInit( &_this->CancelButton );
  ViewsText__ReInit( &_this->TextW0 );
  CutterButton__ReInit( &_this->ButtonA4 );
  CutterButton__ReInit( &_this->ButtonA5 );
  CutterButton__ReInit( &_this->ButtonA6 );
  CutterButton__ReInit( &_this->ButtonA7 );
  CutterButton__ReInit( &_this->ButtonA8 );
}

/* Finalizer method for the class 'Cutter::FormatDialog' */
void CutterFormatDialog__Done( CutterFormatDialog _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterFormatDialog );

  /* Finalize all embedded objects */
  CutterButton__Done( &_this->ButtonA3 );
  ViewsText__Done( &_this->TextW );
  ViewsText__Done( &_this->TextH );
  ViewsText__Done( &_this->TextWidth );
  ViewsText__Done( &_this->TextHeight );
  CutterCancelButton__Done( &_this->CancelButton );
  ViewsText__Done( &_this->TextW0 );
  CutterButton__Done( &_this->ButtonA4 );
  CutterButton__Done( &_this->ButtonA5 );
  CutterButton__Done( &_this->ButtonA6 );
  CutterButton__Done( &_this->ButtonA7 );
  CutterButton__Done( &_this->ButtonA8 );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void CutterFormatDialog_Init( CutterFormatDialog _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  CoreRectView__OnSetBounds( _this, EwSetRectSize( _this->Super2.Bounds, EwNewPoint( 
  CutterScreenSize.X - ( 2 * CutterBorder.X ), (( CutterScreenSize.Y - CutterBorder.Y ) 
  - 22 ) - 32 )));
}

/* 'C' function for method : 'Cutter::FormatDialog.OnSetPaperWidth()' */
void CutterFormatDialog_OnSetPaperWidth( CutterFormatDialog _this, XRef value )
{
  if ( !EwCompRef( value, _this->PaperWidth ))
    return;

  if ( _this->PaperWidth.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterFormatDialog_onUpdate ), _this->PaperWidth, 
      0 );

  _this->PaperWidth = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterFormatDialog_onUpdate ), value, 
      0 );

  EwPostSignal( EwNewSlot( _this, CutterFormatDialog_onUpdate ), ((XObject)_this ));
}

/* 'C' function for method : 'Cutter::FormatDialog.OnSetPaperHeight()' */
void CutterFormatDialog_OnSetPaperHeight( CutterFormatDialog _this, XRef value )
{
  if ( !EwCompRef( value, _this->PaperHeight ))
    return;

  if ( _this->PaperHeight.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterFormatDialog_onUpdate ), _this->PaperHeight, 
      0 );

  _this->PaperHeight = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterFormatDialog_onUpdate ), value, 
      0 );

  EwPostSignal( EwNewSlot( _this, CutterFormatDialog_onUpdate ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void CutterFormatDialog_onUpdate( CutterFormatDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperWidth.Object != 0 )
    ViewsText_OnSetString( &_this->TextWidth, EwConcatString( EwConcatString( EwNewStringInt( 
    EwOnGetInt32( _this->PaperWidth ) / 10, 0, 10 ), EwLoadString( &_Const001B )), 
    EwNewStringInt( EwOnGetInt32( _this->PaperWidth ) % 10, 0, 10 )));

  if ( _this->PaperHeight.Object != 0 )
    ViewsText_OnSetString( &_this->TextHeight, EwConcatString( EwConcatString( EwNewStringInt( 
    EwOnGetInt32( _this->PaperHeight ) / 10, 0, 10 ), EwLoadString( &_Const001B )), 
    EwNewStringInt( EwOnGetInt32( _this->PaperHeight ) % 10, 0, 10 )));

  if (( _this->PaperWidth.Object != 0 ) && ( _this->PaperHeight.Object != 0 ))
  {
    XInt32 w = EwOnGetInt32( _this->PaperWidth );
    XInt32 h = EwOnGetInt32( _this->PaperHeight );
    CutterButton_OnSetActive( &_this->ButtonA3, (XBool)(( w == 2970 ) && ( h == 
    4200 )));
    CutterButton_OnSetActive( &_this->ButtonA4, (XBool)(( w == 2100 ) && ( h == 
    2970 )));
    CutterButton_OnSetActive( &_this->ButtonA5, (XBool)(( w == 1480 ) && ( h == 
    2100 )));
    CutterButton_OnSetActive( &_this->ButtonA6, (XBool)(( w == 1050 ) && ( h == 
    1480 )));
    CutterButton_OnSetActive( &_this->ButtonA7, (XBool)(( w == 740 ) && ( h == 1050 )));
    CutterButton_OnSetActive( &_this->ButtonA8, (XBool)(( w == 520 ) && ( h == 740 )));
  }
}

/* Slot method to forward the cancel event. */
void CutterFormatDialog_onCancel( CutterFormatDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( _this->OnCancel, ((XObject)_this ));
}

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA3( CutterFormatDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperWidth.Object != 0 )
    EwOnSetInt32( _this->PaperWidth, 2970 );

  if ( _this->PaperHeight.Object != 0 )
    EwOnSetInt32( _this->PaperHeight, 4200 );
}

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA4( CutterFormatDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperWidth.Object != 0 )
    EwOnSetInt32( _this->PaperWidth, 2100 );

  if ( _this->PaperHeight.Object != 0 )
    EwOnSetInt32( _this->PaperHeight, 2970 );
}

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA5( CutterFormatDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperWidth.Object != 0 )
    EwOnSetInt32( _this->PaperWidth, 1480 );

  if ( _this->PaperHeight.Object != 0 )
    EwOnSetInt32( _this->PaperHeight, 2100 );
}

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA6( CutterFormatDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperWidth.Object != 0 )
    EwOnSetInt32( _this->PaperWidth, 1050 );

  if ( _this->PaperHeight.Object != 0 )
    EwOnSetInt32( _this->PaperHeight, 1480 );
}

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA7( CutterFormatDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperWidth.Object != 0 )
    EwOnSetInt32( _this->PaperWidth, 740 );

  if ( _this->PaperHeight.Object != 0 )
    EwOnSetInt32( _this->PaperHeight, 1050 );
}

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA8( CutterFormatDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperWidth.Object != 0 )
    EwOnSetInt32( _this->PaperWidth, 520 );

  if ( _this->PaperHeight.Object != 0 )
    EwOnSetInt32( _this->PaperHeight, 740 );
}

/* Variants derived from the class : 'Cutter::FormatDialog' */
EW_DEFINE_CLASS_VARIANTS( CutterFormatDialog )
EW_END_OF_CLASS_VARIANTS( CutterFormatDialog )

/* Virtual Method Table (VMT) for the class : 'Cutter::FormatDialog' */
EW_DEFINE_CLASS( CutterFormatDialog, CoreGroup, OnCancel, OnCancel, PaperWidth, 
                 ButtonA3, _None, "Cutter::FormatDialog" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterFormatDialog )

/* Initializer for the class 'Cutter::Button' */
void CutterButton__Init( CutterButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  ViewsBorder__Init( &_this->Border, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterButton );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0073 );
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->TouchHandler, _Const0074 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->TouchHandler, _Const0075 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->TouchHandler, _Const0076 );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->TouchHandler, _Const0004 );
  CoreView_OnSetLayout((CoreView)&_this->Rectangle, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const0073 );
  ViewsRectangle_OnSetColor( &_this->Rectangle, CutterButtonBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0073 );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0077 ));
  CoreView_OnSetLayout((CoreView)&_this->Border, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Border, _Const0073 );
  ViewsBorder_OnSetWidth( &_this->Border, 3 );
  ViewsBorder_OnSetColor( &_this->Border, CutterButtonBgColor );
  CoreGroup__Add( _this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Border ), 0 );
  _this->TouchHandler.OnLeave = EwNewSlot( _this, CutterButton_enterLeaveSlot );
  _this->TouchHandler.OnEnter = EwNewSlot( _this, CutterButton_enterLeaveSlot );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, CutterButton_pressReleaseSlot );
  _this->TouchHandler.OnPress = EwNewSlot( _this, CutterButton_pressReleaseSlot );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &CutterButtonFont, ResourcesFont ));
}

/* Re-Initializer for the class 'Cutter::Button' */
void CutterButton__ReInit( CutterButton _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
  ViewsRectangle__ReInit( &_this->Rectangle );
  ViewsText__ReInit( &_this->Text );
  ViewsBorder__ReInit( &_this->Border );
}

/* Finalizer method for the class 'Cutter::Button' */
void CutterButton__Done( CutterButton _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterButton );

  /* Finalize all embedded objects */
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );
  ViewsRectangle__Done( &_this->Rectangle );
  ViewsText__Done( &_this->Text );
  ViewsBorder__Done( &_this->Border );

  /* Release all used strings */
  EwReleaseString( &_this->Caption );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
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
void CutterButton_UpdateViewState( CutterButton _this, XSet aState )
{
  if ((( aState & CoreViewStateEnabled ) == CoreViewStateEnabled ))
  {
    if ( _this->TouchHandler.Down && _this->TouchHandler.Inside )
    {
      ViewsText_OnSetColor( &_this->Text, CutterSelectColor );
      ViewsRectangle_OnSetColor( &_this->Rectangle, CutterSelectColor );
      ViewsBorder_OnSetColor( &_this->Border, CutterButtonBgColor );
    }
    else
      if ( _this->Active )
      {
        ViewsText_OnSetColor( &_this->Text, CutterTextColor );
        ViewsRectangle_OnSetColor( &_this->Rectangle, CutterBackColor );
        ViewsBorder_OnSetColor( &_this->Border, CutterButtonBgColor );
      }
      else
      {
        ViewsText_OnSetColor( &_this->Text, CutterButtonTextColor );
        ViewsRectangle_OnSetColor( &_this->Rectangle, CutterButtonBgColor );
        ViewsBorder_OnSetColor( &_this->Border, CutterButtonBgColor );
      }
  }
  else
  {
    ViewsText_OnSetColor( &_this->Text, CutterSelectColor );
    ViewsRectangle_OnSetColor( &_this->Rectangle, CutterBackColor );
    ViewsBorder_OnSetColor( &_this->Border, CutterBackColor );
  }
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterButton_enterLeaveSlot( CutterButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterButton_pressReleaseSlot( CutterButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->TouchHandler.Down && _this->TouchHandler.Inside )
  {
    EwIdleSignal( _this->OnChange, ((XObject)_this ));
  }
}

/* 'C' function for method : 'Cutter::Button.OnSetActive()' */
void CutterButton_OnSetActive( CutterButton _this, XBool value )
{
  if ( _this->Active == value )
    return;

  _this->Active = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Cutter::Button.OnSetCaption()' */
void CutterButton_OnSetCaption( CutterButton _this, XString value )
{
  EwRetainString( &_this->Caption, value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Cutter::Button' */
EW_DEFINE_CLASS_VARIANTS( CutterButton )
EW_END_OF_CLASS_VARIANTS( CutterButton )

/* Virtual Method Table (VMT) for the class : 'Cutter::Button' */
EW_DEFINE_CLASS( CutterButton, CoreGroup, OnChange, OnChange, TouchHandler, TouchHandler, 
                 Caption, "Cutter::Button" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CutterButton_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterButton )

/* Color constant according UI design. */
const XColor CutterButtonBgColor = { 0x3B, 0x5E, 0x54, 0xFF };

/* Color constant according UI design. */
const XColor CutterButtonTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

/* This is a font resource. */
EW_DEFINE_FONT_RES( CutterButtonFont, 15, 4, 0, 16, 0x0001, 0x003F )
  EW_GLYPH( 0x0001, 0, -11, 7, 11, 7, 0x00000000 ),
  EW_GLYPH( 0x0030, 0, -11, 9, 11, 9, 0x00000101 ),             /* '0' */
  EW_GLYPH( 0x0031, 1, -11, 5, 11, 9, 0x00000220 ),             /* '1' */
  EW_GLYPH( 0x0032, 0, -11, 9, 11, 9, 0x00000291 ),             /* '2' */
  EW_GLYPH( 0x0033, 0, -11, 9, 11, 9, 0x000003E0 ),             /* '3' */
  EW_GLYPH( 0x0034, 0, -11, 9, 11, 9, 0x00000537 ),             /* '4' */
  EW_GLYPH( 0x0035, 1, -11, 8, 11, 9, 0x00000637 ),             /* '5' */
  EW_GLYPH( 0x0036, 0, -11, 9, 11, 9, 0x0000076C ),             /* '6' */
  EW_GLYPH( 0x0037, 0, -11, 9, 11, 9, 0x000008B8 ),             /* '7' */
  EW_GLYPH( 0x0038, 0, -11, 9, 11, 9, 0x000009D2 ),             /* '8' */
  EW_GLYPH( 0x0039, 0, -11, 9, 11, 9, 0x00000B36 ),             /* '9' */
  EW_GLYPH( 0x0041, 0, -11, 11, 11, 11, 0x00000C8B ),           /* 'A' */
  EW_GLYPH( 0x0042, 1, -11, 9, 11, 10, 0x00000DE9 ),            /* 'B' */
  EW_GLYPH( 0x0043, 0, -11, 10, 11, 10, 0x00000EFA ),           /* 'C' */
  EW_GLYPH( 0x0044, 1, -11, 9, 11, 10, 0x00001067 ),            /* 'D' */
  EW_GLYPH( 0x0045, 1, -11, 8, 11, 9, 0x00001170 ),             /* 'E' */
  EW_GLYPH( 0x0046, 1, -11, 8, 11, 9, 0x00001247 ),             /* 'F' */
  EW_GLYPH( 0x0047, 0, -11, 10, 11, 11, 0x00001307 ),           /* 'G' */
  EW_GLYPH( 0x0048, 1, -11, 10, 11, 11, 0x00001464 ),           /* 'H' */
  EW_GLYPH( 0x0049, 1, -11, 3, 11, 5, 0x0000151B ),             /* 'I' */
  EW_GLYPH( 0x004A, 0, -11, 8, 11, 9, 0x0000154A ),             /* 'J' */
  EW_GLYPH( 0x004B, 1, -11, 10, 11, 10, 0x000015FC ),           /* 'K' */
  EW_GLYPH( 0x004C, 1, -11, 8, 11, 9, 0x00001726 ),             /* 'L' */
  EW_GLYPH( 0x004D, 1, -11, 12, 11, 14, 0x000017A3 ),           /* 'M' */
  EW_GLYPH( 0x004E, 0, -11, 10, 11, 11, 0x00001909 ),           /* 'N' */
  EW_GLYPH( 0x004F, 0, -11, 11, 11, 11, 0x00001A00 ),           /* 'O' */
  EW_GLYPH( 0x0050, 1, -11, 9, 11, 10, 0x00001B6B ),            /* 'P' */
  EW_GLYPH( 0x0051, 0, -11, 11, 13, 11, 0x00001C60 ),           /* 'Q' */
  EW_GLYPH( 0x0052, 1, -11, 9, 11, 10, 0x00001E1E ),            /* 'R' */
  EW_GLYPH( 0x0053, 0, -11, 10, 11, 10, 0x00001F46 ),           /* 'S' */
  EW_GLYPH( 0x0054, 0, -11, 10, 11, 10, 0x000020EC ),           /* 'T' */
  EW_GLYPH( 0x0055, 0, -11, 10, 11, 10, 0x000021A9 ),           /* 'U' */
  EW_GLYPH( 0x0056, 0, -11, 10, 11, 10, 0x00002287 ),           /* 'V' */
  EW_GLYPH( 0x0057, 0, -11, 14, 11, 14, 0x000023CE ),           /* 'W' */
  EW_GLYPH( 0x0058, 0, -11, 10, 11, 10, 0x000025AC ),           /* 'X' */
  EW_GLYPH( 0x0059, 0, -11, 10, 11, 10, 0x0000271B ),           /* 'Y' */
  EW_GLYPH( 0x005A, 0, -11, 10, 11, 10, 0x00002833 ),           /* 'Z' */
  EW_GLYPH( 0x0061, 0, -8, 9, 8, 9, 0x00002979 ),               /* 'a' */
  EW_GLYPH( 0x0062, 1, -12, 8, 12, 9, 0x00002A77 ),             /* 'b' */
  EW_GLYPH( 0x0063, 0, -8, 8, 8, 8, 0x00002B66 ),               /* 'c' */
  EW_GLYPH( 0x0064, 0, -12, 8, 12, 9, 0x00002C5E ),             /* 'd' */
  EW_GLYPH( 0x0065, 0, -8, 9, 8, 9, 0x00002D4A ),               /* 'e' */
  EW_GLYPH( 0x0066, 0, -12, 6, 12, 6, 0x00002E5A ),             /* 'f' */
  EW_GLYPH( 0x0067, 0, -8, 9, 11, 9, 0x00002EF9 ),              /* 'g' */
  EW_GLYPH( 0x0068, 1, -12, 7, 12, 9, 0x00003042 ),             /* 'h' */
  EW_GLYPH( 0x0069, 1, -11, 3, 11, 4, 0x000030E7 ),             /* 'i' */
  EW_GLYPH( 0x006A, -1, -11, 4, 14, 4, 0x00003124 ),            /* 'j' */
  EW_GLYPH( 0x006B, 1, -12, 8, 12, 8, 0x00003193 ),             /* 'k' */
  EW_GLYPH( 0x006C, 1, -12, 2, 12, 4, 0x0000327C ),             /* 'l' */
  EW_GLYPH( 0x006D, 1, -8, 12, 8, 14, 0x0000329E ),             /* 'm' */
  EW_GLYPH( 0x006E, 1, -8, 7, 8, 9, 0x0000337B ),               /* 'n' */
  EW_GLYPH( 0x006F, 0, -8, 9, 8, 9, 0x00003402 ),               /* 'o' */
  EW_GLYPH( 0x0070, 1, -8, 8, 11, 9, 0x00003506 ),              /* 'p' */
  EW_GLYPH( 0x0071, 0, -8, 8, 11, 9, 0x0000360D ),              /* 'q' */
  EW_GLYPH( 0x0072, 1, -8, 5, 8, 6, 0x00003706 ),               /* 'r' */
  EW_GLYPH( 0x0073, 0, -8, 8, 8, 8, 0x00003752 ),               /* 's' */
  EW_GLYPH( 0x0074, -1, -10, 6, 10, 5, 0x00003869 ),            /* 't' */
  EW_GLYPH( 0x0075, 1, -8, 7, 8, 9, 0x000038F1 ),               /* 'u' */
  EW_GLYPH( 0x0076, 0, -8, 8, 8, 8, 0x00003968 ),               /* 'v' */
  EW_GLYPH( 0x0077, 0, -8, 12, 8, 12, 0x00003A47 ),             /* 'w' */
  EW_GLYPH( 0x0078, 0, -8, 8, 8, 8, 0x00003B9A ),               /* 'x' */
  EW_GLYPH( 0x0079, 0, -8, 8, 11, 8, 0x00003C8F ),              /* 'y' */
  EW_GLYPH( 0x007A, 0, -8, 8, 8, 8, 0x00003DB4 ),               /* 'z' */

  EW_FONT_PIXEL( CutterButtonFont, 0x00003E9B )                 /* ratio 71.01 % */
    0xCF7BDCE3, 0xFEF7BBF0, 0xE498C398, 0x2D6B1334, 0x279093C8, 0x9C85AD61, 0x31873266,
    0x7F7BDDF9, 0x5FFBFF58, 0x7BB7DD58, 0x93C78EF1, 0x99898C9F, 0x00480611, 0x9900C200,
    0xBC333131, 0x63F331E3, 0x2F5FDF75, 0xF3E8FF98, 0xF4BE2FE6, 0x000005BD, 0xEF8C0000,
    0xFF137FDC, 0xF5BFBFF5, 0x998678AA, 0x031A73CB, 0x640B1981, 0x275E4367, 0x037F3790,
    0xC41FB271, 0x3905CC7F, 0x5DEF7BCC, 0xFFDCEFC6, 0x7FF3FE22, 0xF5636EBF, 0x7DF23CB0,
    0xE0FCB000, 0xDC267EDE, 0x302F3EFB, 0x3DFA8CC7, 0x3D3A7CC0, 0xE7FE4663, 0x882FDDBE,
    0x05F80773, 0xE2093CF0, 0xFCF814F7, 0x880EACC0, 0xA8DE09AE, 0xEBFDA77B, 0xF69DEF7B,
    0xB880000F, 0xA57BDEF7, 0x535EF7BE, 0x75C13482, 0x9ED2F9CE, 0xD5757CE7, 0x02CAD43B,
    0x8E1FEC47, 0x1AC65AA8, 0xFFBBFDD5, 0x9DFA87F5, 0xDC6FF41F, 0x0EAD6A1F, 0xC9E05EB8,
    0x2297E73E, 0x73FF9CF2, 0x5958CB51, 0x1E3111C0, 0x1B47531C, 0x8DFA86AB, 0xEB3F5FFB,
    0xBDEF7BDE, 0xDEF7BACF, 0x18C0A23B, 0x07EBC02B, 0x3F04A9F2, 0x051AC417, 0x1BC07C66,
    0xC098CC13, 0xFFD02F17, 0xFAF46FFB, 0x33356FDD, 0x001315A6, 0x5E332C6F, 0xD6FBE8D6,
    0xFBBF97E2, 0x4F593E7B, 0x10C9E7A9, 0x52A8C608, 0xDEBFE955, 0x788FAEFB, 0xFF85DDCF,
    0x6FDFDFFA, 0x2B0D6318, 0xC8CB301B, 0xDC23D435, 0x03FE7FE7, 0x1DE73BF9, 0x01E32011,
    0xCF440E7E, 0xE3BDE0F9, 0x9DDF817B, 0xC07F2C80, 0x9E00D467, 0xF9503F6C, 0xFC704E54,
    0x6A20F13C, 0x33073F0C, 0x255BBDC6, 0xEF7BDC5F, 0x4172A7CF, 0xA0D8E3C5, 0xDEF7FEB2,
    0x7B9C17BB, 0x05681BAB, 0x87DA0084, 0x85ACEF73, 0xD9EFEF73, 0x01233D40, 0x392AA780,
    0x30FAF6F7, 0x6FFF73BE, 0xBEF77F52, 0x17D786EB, 0x591AE66A, 0x94CF7DF0, 0x1748CF90,
    0x4A602E90, 0x17239188, 0x8EF1FBBC, 0xA98CD42F, 0xD77DEEFE, 0xFDEF7B8E, 0xA7BDC077,
    0x1FF18276, 0x0955102F, 0x035C05A4, 0x4C3006B8, 0x180D4F90, 0xF7B8BC67, 0x7BFF4ED6,
    0x7177BDEF, 0x0177BDEE, 0xEE700000, 0x7B9C7FBD, 0x00001FEF, 0x7BDEE700, 0xF7BDFFDF,
    0xEE70FBDE, 0x0001F7BD, 0x7BDCE000, 0xDEF7387F, 0x0000001F, 0xDFFDE200, 0x5D887BF3,
    0xD4DF6EE7, 0xFAF90EF9, 0xF3C8515B, 0x01100C45, 0x1FEF7B9E, 0x9912F73C, 0xE8098F00,
    0xD8B2C5D1, 0xEFD9EFB3, 0xBF01BFF7, 0x0000003B, 0xF7380000, 0xEE705BDE, 0x0000B7BD,
    0xF8000000, 0x000001DD, 0x7FB88000, 0x00000000, 0xF5800000, 0xE656187F, 0xEF3CED09,
    0xF57E37BA, 0x177D81BF, 0xC0B98EF0, 0xD182F59C, 0x3FF207E3, 0x400C700E, 0x8CE01DFC,
    0x5860BC77, 0x13803E63, 0x8DF204BF, 0x0006FFD7, 0x00000000, 0x00000000, 0xEF7BDCE0,
    0xF03FDFF8, 0xA0B27FBF, 0xFF0FF212, 0x7AC16784, 0x27878BE0, 0x9C3AC83E, 0xCAFF101A,
    0xE2B03FAC, 0x51026279, 0x4E00B7DF, 0x32A01FE6, 0x019EE20B, 0x005C4FF7, 0xDF320163,
    0x02F1BC04, 0xBC04D66A, 0xE37801D9, 0x0053D403, 0x1F8800BF, 0xBFEFFF44, 0xDCEBB103,
    0xBCE83B53, 0xACD9B8C3, 0x8CB9181C, 0x02466098, 0x88C40010, 0x95A46609, 0x9D172303,
    0x9B371867, 0xA7B9CFD8, 0xDEF7B876, 0xF7137FFB, 0xC03753DE, 0x000272F8, 0x7B878BE3,
    0xB93B59EF, 0x04BFFEF7, 0x00000000, 0xFFBFFBC4, 0xCE7EC40E, 0xF83FCF3D, 0x3BC61DE5,
    0x2A072A5E, 0x8C1311A6, 0x60000008, 0x78446098, 0xF162A072, 0x778C3BCB, 0x739FB134,
    0xE204F9FF, 0xCCF3DFFD, 0xD7D7C01F, 0xFF7BDEFF, 0xF6F7381D, 0x33CF03FC, 0x3CF00006,
    0xFDEE7063, 0xBEE71FD6, 0x19CCC022, 0x302F17C0, 0x19880663, 0x773BF52F, 0xBCBF3C4F,
    0xBCBEF7F7, 0x998D617E, 0x5E9D9C08, 0xB3C1C8A7, 0x02BFEFE7, 0xFBFFEFC6, 0x253C47DD,
    0xF0BF1EE7, 0xE8C8CC35, 0x1C9EA1D1, 0x7DEE37D6, 0x7BDEF6D7, 0x3EF7BDEF, 0xDE47BDEF,
    0x00001F7B, 0x00000000, 0x00000000, 0xC1FDC600, 0x00000EEF, 0x00000000, 0xC4600000,
    0x1DE91582, 0xB0DE358B, 0x7ECE7DDE, 0xD4077D97, 0xFC1F3FDD, 0x6582A6FD, 0xA2A0E546,
    0x77F1F1C2, 0xF158A278, 0x58A8E560, 0x3EFEF9F0, 0x072B5918, 0xE03E32F8, 0x89DF41E1,
    0x77DD41FF, 0xB14664BB, 0x0C50F2ED, 0x760C6586, 0x54C9E6D6, 0x325DAA86, 0x62357F16,
    0xE5365A97, 0xFEC46A5F, 0x1ED7222B, 0x5122AAA1, 0x23D8FCF9, 0x4A28AE0B, 0x20C4A1FC,
    0x0FF75153, 0xD99ACEFB, 0xC75351CC, 0x12F1D11C, 0x998EE8BF, 0x3666AA78, 0xF3407CB8,
    0xE66A3981, 0xDF78E88B, 0xBC5F839C, 0x93CF5DA9, 0xFAC6A78B, 0x7DDF82EE, 0x1A8C5E3A,
    0xB458CE4F, 0xA8D78DE6, 0xD58DE22A, 0x35BE561A, 0x1F88E807, 0x00024650, 0xEE780000,
    0xBBDEF7BD, 0x7BDEF73D, 0xAACC07CB, 0x05E3A209, 0x3C1378BE, 0x76881635, 0x4039CE81,
    0xD7817CCD, 0x77E93C0B, 0x4437BDEF, 0xF1BFEFFF, 0x56FEF7AD, 0x0CB29DF3, 0x931EFFF4,
    0x107BEAFF, 0x99101F23, 0xFCF385E9, 0xFB8637FF, 0x00000007, 0x20BE73E8, 0x8FABE7FF,
    0x00D5547C, 0xFC00137F, 0xAAA8F94D, 0xABE7FEA1, 0xDDCF788F, 0xF7DAFF0F, 0x5F19EAF9,
    0x4BFEE5FA, 0x7F00DFF2, 0x33D6FC49, 0xFC35D4BE, 0x15FCDF6B, 0x0000BFF0, 0xA73BE200,
    0xFE77D5F1, 0x27955469, 0xB0007F4D, 0x955641FC, 0x3BEAF927, 0x7BBF50BB, 0xEDF9E0BE,
    0x1D3FAFFB, 0x446358CB, 0x5A77BDC6, 0xBDEF7B8C, 0x41023915, 0x9E6F42FB, 0x90793DCF,
    0xCF39B3BF, 0x093C62DE, 0xB5DD4758, 0x000BBA8E, 0xC4000000, 0xBBFBCE7B, 0xF9DF5BE1,
    0x52AA8C3F, 0x000FE983, 0x3E40FE58, 0x6F819E55, 0x780F677D, 0x784579CF, 0xE5F2B0BF,
    0xBBDFDEE7, 0x00003FDC, 0xFDFFCC00, 0x5DFF1CA5, 0x7C8C7CAF, 0x00000080, 0xFD87FD80,
    0x00002F77, 0xFF037FC0, 0x0000BDCD, 0x9E108000, 0x7FBE8CF7, 0x00000000, 0x9AC2FBDE,
    0x9F7A8BCF, 0x868BE30F, 0x5C85E378, 0xAD660D5D, 0xF17C5F02, 0xC00000BF, 0x2FEE7DDD,
    0xCA5FDCFB, 0xCFB57E7D, 0x946BD5F9, 0x03E55312, 0x00000080, 0xB8000000, 0x2FEFFD7B,
    0x7AEFFC8D, 0x03E463E5, 0x00000004, 0xBFF7FEB0, 0xEDEFBDF0, 0x2A9E33F7, 0xFD393D4F,
    0x4C00B7F1, 0x3C6B7F07, 0xE93D54AB, 0xDF7DEF9B, 0xE7BDFDCF, 0x3FF3C17C, 0x9463E7BF,
    0xB6C12AA7, 0xCB0C05C0, 0x1A554F28, 0x3E7BF3FF, 0x017CE7BC, 0xE77C4000, 0xEAFAFBBC,
    0x55187B3B, 0x1FD349E5, 0x307F4C00, 0x7C93CAAA, 0xC49D9DF5, 0x000BCE77, 0xF3AEFFC0,
    0x004CBCE2, 0xBF180000, 0xFE257E73, 0x191BF5EE, 0x5473B7C4, 0xF0CCDFDD, 0x37D5CDFB,
    0x57BD9AC7, 0xCD21DD2C, 0xE27B8BF1, 0x00DDF837, 0x8DE72DC4, 0x000BCE5B, 0xFC4A4000,
    0x1FEEFEC6, 0x00000BFF, 0xE1CA8000, 0xF3ED6F49, 0x9E1F7D4F, 0xEB9B3FDB, 0xC6A2C637,
    0xF8AA8BF0, 0x26FFFD31, 0x38155551, 0x889E0799, 0x7FC5BE87, 0x0CFD7753, 0x6ADA4723,
    0x8F1156C4, 0xBF1EA1CA, 0xB8A33F55, 0x8EFFC731, 0xEA3A5369, 0x443C5F89, 0x03E4F145,
    0xD4D8E5DB, 0x59DDF1FD, 0xD4F53D4F, 0xE37E8BF4, 0x1D997E23, 0xC7B20418, 0xFA379378,
    0x1ACD663A, 0x3EF7AC73, 0x873FB73C, 0x89F237E3, 0xC4F0C678, 0xF9F1F8AA, 0xA8E498DF,
    0x2E27C0AA, 0xC20788AC, 0xB887CB04, 0xEE312FDF, 0x67DEF7BD, 0xE35F7BDC, 0x781C7641,
    0x6AAB13A3, 0xC1F1BC62, 0x19E05F8D, 0x02EF7BBE

  EW_FONT_KERNING_CODES( CutterButtonFont )
  EW_FONT_KERNING_VALUES( CutterButtonFont )
EW_END_OF_FONT_RES( CutterButtonFont )

/* Table with links to derived variants of the font resource : 'Cutter::ButtonFont' */
EW_RES_WITHOUT_VARIANTS( CutterButtonFont )

/* This is a bitmap resource. */
EW_DEFINE_BITMAP_RES( CutterBarRaster )
  EW_BITMAP_FRAMES( CutterBarRaster, Default, EW_DRIVER_VARIANT_ALPHA8, 418, 71, 0 )
    EW_BITMAP_FRAME( 378, 0, 39, 6, 0x00000000, 0x00000000 )

  EW_BITMAP_PIXEL( CutterBarRaster, Default )                   /* ratio 7.41 % */
    0x1C080100, 0xC1A0B048, 0x2A130883, 0xA1B0C85C, 0x231087C3, 0xB1489C4A, 0x188BC5A2,
    0xA8D46A33, 0x8FC79FE6, 0x1C8A4320, 0xAAA3F249, 0xAA53288D, 0xA5B2C95C, 0x633097CB,
    0xB258EC66, 0x27924DA6, 0xC9DCEA65, 0x9FCFA7B3, 0xBCD27F40, 0xCE66F449, 0x2A9348A0,
    0x29B4CA5D, 0xA345A1CB, 0xB4EA3C8E, 0x58ABD5AA, 0x2A84F2B3, 0xAA47ABB5, 0x1D8AC35A,
    0xC12CB64B, 0x7AD35E38, 0xADB665AC, 0x7B70B7DB, 0xB741E866, 0x78BBDDAE, 0x9BADEAF3,
    0xBFDFAE22, 0xD0A29580, 0xE1B0B843, 0x2B1388C3, 0x0005FC2E, 0x2390358B, 0xB94C9E4B,
    0x47C8E5B2, 0xCCDE6B02, 0xB516A5B9, 0x4001AB69, 0xADABBA0B, 0xAB53A8CE, 0x74A5488E,
    0xB80063D7, 0x7609B6BA, 0xAADBEDB5, 0x0A2ECF5B, 0x9C6EF65B, 0xE70B83B8, 0xCDB7FBAC,
    0x0E8DC6DE, 0xEDFCCE5F, 0x4535E4E7, 0xF24E8693, 0xD8EBF374, 0xC3E8A6B3, 0xEFF7BBBD,
    0xEF8BC3E0, 0x8D00062E, 0x8FD3E8F1, 0xBDBD929F, 0x8010847B, 0xBF4F9FC8, 0xF8FBFDBE,
    0xF000DFE7, 0xFFFF85C0, 0xE0280600, 0x47FE0480, 0x14130002, 0x0582E0A8, 0x83DEF07E,
    0x5D41160F, 0x9000E549, 0x4D2484C5, 0x181C0003, 0x840B448E, 0x44176620, 0x03612287,
    0x7A247140, 0x268A5C84, 0x7322E885, 0x91E8B223, 0xB8A62889, 0xBDB58CA2, 0x316A63A8,
    0x8CA348CA, 0xE406C62A, 0x68EE393F, 0x0C4466A4, 0x58000E39, 0x000D190C, 0xCB290C6F,
    0x06765491, 0x66582F83, 0x5C96E5A9, 0x9665E976, 0xC3E000A1, 0xF9966497, 0x3C002525,
    0x6C9AE661, 0x9BE551FA, 0xC150112C, 0x69D6749C, 0xE6789DE7, 0x0004E749, 0x9FC07008,
    0xE82A0680, 0x0A1684A0, 0x1A7D01A8, 0x86A2E8AA, 0x00C00016, 0xA92A468C, 0xBA5670A3,
    0x64F70BD1, 0x9CA6E9AA, 0xA7E9EA76, 0x4003829A, 0x6A96A00F, 0x464FA82A, 0xACAAEA71,
    0x19C00306, 0x6A5AC6AC, 0x8AD69629, 0xA6430C11, 0x43100003, 0x1540036A, 0xC0032643,
    0x62B2431A, 0xE114B1EB, 0x0039E40A, 0x8FE430FC, 0x62C8DE40, 0x8B56C891, 0x3232B290,
    0xD0902CEB, 0x8A6D28CA, 0xEE2B5AD4, 0xB90C3140, 0x7243AECE, 0xCA90EBF3, 0x90C6C000,
    0x2F2B8CAD, 0x63B42D42, 0xE6F8BDEF, 0xFCBEEFAB, 0x000D7BDE, 0xF02BF43B, 0xEB0AFDC0,
    0x2708C104, 0x63000B9C, 0xC0B0EC24, 0x712BCC9E, 0x6C540141, 0x6718C5F1, 0x17C6F1AC,
    0xC7F1C40B, 0x71EC7F20, 0x3C9724C8, 0x7328C972, 0x1A6DF13C, 0xCBF2E33C, 0xF32CC730,
    0x6CBF34CC, 0x3023018C, 0x3CCEF3A3, 0xCFF3ECF7, 0xB0BCEF40, 0x6D17410C, 0x0EF1D074,
    0x4CD2F471, 0xD32BF27F, 0xC88D1750, 0x3085B6CA, 0xB8083FA4, 0x003C2430, 0x38643108,
    0xE4315C00, 0x31B80033, 0xAF542C64, 0xF490C206, 0x90C2C020, 0xE27B3EDA, 0x1892DF90,
    0xAD7EDA6E, 0x6FDB640D, 0xDCACDB2F, 0x77589235, 0x0DE37787, 0xF232DE97, 0xDCDFF70D,
    0x90AD1DCE, 0xB86E1640, 0x990F565B, 0x74F190F5, 0x439335FD, 0xD9E50342, 0x40567793,
    0x6E97A408, 0xA7A8E9FA, 0x20E97AAE, 0xEAC06004, 0xBABEC7B0, 0x20200007, 0x73B8EDFB,
    0xB802C000, 0x06BB0EF7, 0xFBE03800, 0x105BA8F0, 0x500B4BFA, 0xCCF2C9B2, 0xF3FCEF37,
    0xFCCF47D0, 0x62400E03, 0x67D8F5FD, 0xDCF6FDAF, 0x0148AF5F, 0xFE2F7419, 0x020BDDF8,
    0xA7E40900, 0x001EBEAF, 0xFBFEA0BC, 0x0001C3E2, 0xAFD7F00F, 0xBBC7183D, 0xFF90C134,
    0xFF01B06C, 0x90C17809, 0x218001EC, 0x161CC90C, 0x4635121A, 0xE4B62433, 0xA012258F,
    0x903290C7, 0x6DB9B601, 0x91F2DB71, 0x3905BCE3, 0x82103D1C, 0x05120412, 0x6CB7D82D,
    0x81834E31, 0x30E0D0EB, 0x4A0FB924, 0xAC138224, 0xE82EDEE0, 0xA8B20CB7, 0xE15A2C5B,
    0x42E85A84, 0x486FEC88, 0x00A003FE, 0x000F8481, 0xE8480924, 0x6473C000, 0x0C391A24,
    0xDC3D3AC9, 0x90AA01C8, 0xF15A2A85, 0xA2CC588A, 0xC568B716, 0x8B802404, 0x17230C60,
    0x078030AB, 0x688CF188, 0x2C000284, 0x8236C680, 0xE8000971, 0x98E71B80, 0xA00022C5,
    0x63CC7404, 0x31307715, 0x0BA15401, 0x20A40C80, 0x42C84907, 0x1090321A, 0x9A124010,
    0x3A46C8C0, 0x8C9091F2, 0x891B24A4, 0xF24C0400, 0x649C9892, 0x80D00412, 0x93F27A4C,
    0x27C09C76, 0x20C97947, 0xA4032000, 0x7924254C, 0xA8100000, 0x9C8C95F2, 0x700AC4C1,
    0x20BD218F, 0xE20BFC01, 0xA4380290, 0x7170147B, 0x1351644B, 0x73CB389F, 0x0805890C,
    0x468CA108, 0xB212C338, 0x4B8D6E28, 0x48748621, 0xA9BE1E1D, 0x1204C600, 0x22C99532,
    0x868E2666, 0x8A61B426, 0x0EA36870, 0xC798AB4E, 0x490C664C, 0xD1C04188, 0x9AE09CCD,
    0x896C3D65, 0x6FFE204B, 0x51944397, 0x24097E3D, 0x48085E04, 0x8B0E9A1A, 0x01AF381C,
    0x0813A238, 0x1742A84D, 0x3A86D0CA, 0x281BA134, 0x4A1E2580, 0xF43E8AD1, 0xA8B00F01,
    0x896A3746, 0xA3606C02, 0x23A8A520, 0x90827000, 0x85A1949A, 0x5749CF30, 0x05F6508A,
    0x452BA581, 0xB0486DC3, 0xF4DA9A82, 0xA9CD38A6, 0xD36A774E, 0x02E00005, 0x50AA0C14,
    0xA2D44A87, 0x04A8551A, 0x8E060008, 0x1EA6D4CA, 0x70020055, 0x8AA754AA, 0x60100081,
    0xEA6558AA, 0x80600005, 0xA8957AAC, 0x57DDA815, 0x3E414AC7, 0x63AC8029, 0xBC6DC08D,
    0xBA532431, 0x402D4863, 0xA4863EC0, 0x690C0D01, 0xC168081F, 0x8001DC90, 0x1BC8B11E,
    0x90C2B000, 0x3680019A, 0xB715E90C, 0x7E90C6C1, 0xCA6C8000, 0x6CEB9B68, 0xF9A33B66,
    0x30B0DF4E, 0x6C6D8BB1, 0x2299B93A, 0x7148B26F, 0xE3E77435, 0xCC43BB2C, 0x8EB18902,
    0x45641165, 0x3A6FC328, 0x44D04533, 0x64876561, 0xA37CB7CD, 0xC77800AD, 0x00805C90,
    0xAED5D07E, 0xE5C7E240, 0xBE8EF480, 0x611BE90E, 0xCA33921B, 0xDC582436, 0x88D616A6,
    0x4BA774AE, 0xF75AEADD, 0x01ADD2BA, 0xDD818400, 0xBB177AEE, 0x33403825, 0xF2DE4BBE,
    0xE8BCA9CE, 0x0E336EED, 0xBAF48250, 0x00C2BAB7, 0x4BDE0D40, 0xC5B2E8DF, 0xEFA5F508,
    0xDF942C29, 0x27A23EF7, 0x80802003, 0x02E04C07, 0x08C0F81B, 0xA200701E, 0xCD07E0C8,
    0x8E101CC2, 0x4C2784B0, 0x385B0AE1, 0x6ED84CC2, 0xE1CC3639, 0xC3F87B0E, 0x87310E20,
    0x68000073, 0x28C4F888, 0x0E0A3116, 0xBB148DC0, 0xB016C5F8, 0x7C678BC0, 0x001C6E62,
    0xF1CE3405, 0x03973D87, 0xC378FB1D, 0x8FC265F1, 0x000B3C7D, 0x3AC8617C, 0x984F44CE,
    0x002B2431, 0x02431BA0, 0x01D80002, 0xE3E80457, 0x47D243B7, 0x0EC4875E, 0x1BA90174,
    0x33121D81, 0x572435DC, 0x4318E4F4, 0x140002A2, 0x6ACD5A7B, 0x65AC9D53, 0x3B296867,
    0x399EDA5A, 0x6CCE6BCD, 0xDAB4536A, 0xB3F6B665, 0x09E67CE0, 0x1CC1B86F, 0x3D6ED3C2,
    0xB39E0641, 0xA4745991, 0x06719E73, 0x9D023D52, 0x938A24A2, 0x7428D944, 0xAC31908A,
    0x27A2AD21, 0xA887E8C6, 0x52EA4D47, 0xA8D4FA9B, 0xAE7B351E, 0xBB56EA91, 0x2D8CD53A,
    0x4D67ABC7, 0x83CA646B, 0x356EB8D6, 0x6BCD7305, 0x6DC3CB53, 0xA8B60EBD, 0x6103F42B,
    0x535FFB07, 0x8DAF7630, 0x28348001, 0xE8A9D79B, 0x40180289, 0xFB5B6AED, 0xB6CED8DA,
    0x804DABB6, 0xDBFB6DDD, 0x00F6DEE0, 0x72EE1DBC, 0x00005B97, 0xC1BAB730, 0xBB75B15D,
    0x7A7DDB3B, 0xEDA9BCB7, 0x5BCC01D3, 0xC0253EDE, 0x00ADDFBD, 0xDF601400, 0x2E4046EE,
    0x82F04E06, 0x08E0FC1B, 0x9BC1784F, 0xC8458000, 0x8F10E1E2, 0x4E27C4B8, 0x9C438AF1,
    0xB8B0978F, 0x716E37C6, 0x14400016, 0xC8B90F1C, 0x5580603F, 0x1AE4FC88, 0x5004408F,
    0xAF89F2CE, 0x681F00C2, 0x2E1FCCB9, 0x5CCCC70A, 0xD80097E6, 0x08BE5BCD, 0xE7612800,
    0xFF03BF2C, 0x218D27E4, 0xE4000165, 0x00109218, 0xF11259C0, 0xE5757F96, 0xA71370DF,
    0x23912039, 0xC6924081, 0x0B348686, 0x0001A9FE, 0x9A920668, 0xD4232CD9, 0xAEB2366E,
    0xEB6CAC9D, 0x83AF391C, 0x6764CF1D, 0xDCDFD991, 0x36790EF4, 0xA70DA045, 0xDA483968,
    0xEC3DBA6A, 0x9EBB1A32, 0xB4E91ECF, 0x7CED79CF, 0xE1DFEDA7, 0xA3BA783E, 0x25AFD01D,
    0x2D096C51, 0x135109CD, 0x0808AA3A, 0x00000000

  EW_BITMAP_CLUT_EMPTY( CutterBarRaster, Default )

  EW_BITMAPS_TABLE( CutterBarRaster )
    EW_BITMAP( CutterBarRaster, Default )
EW_END_OF_BITMAP_RES( CutterBarRaster )

/* Table with links to derived variants of the bitmap resource : 'Cutter::BarRaster' */
EW_RES_WITHOUT_VARIANTS( CutterBarRaster )

/* Initializer for the class 'Cutter::BarGraph' */
void CutterBarGraph__Init( CutterBarGraph _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandler, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->BarLeft, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->BarRight, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterBarGraph );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0078 );
  CoreView_OnSetLayout((CoreView)&_this->SimpleTouchHandler, CoreLayoutAlignToBottom 
  | CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, _Const0079 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, _Const007A );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, _Const007B );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, _Const0004 );
  CoreView_OnSetLayout((CoreView)&_this->BarLeft, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft );
  CoreRectView__OnSetBounds( &_this->BarLeft, _Const0078 );
  ViewsImage_OnSetAlignment( &_this->BarLeft, ViewsImageAlignmentAlignHorzLeft | 
  ViewsImageAlignmentAlignVertBottom );
  CoreView_OnSetLayout((CoreView)&_this->BarRight, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft );
  CoreRectView__OnSetBounds( &_this->BarRight, _Const0078 );
  ViewsImage_OnSetColor( &_this->BarRight, CutterSelectColor );
  ViewsImage_OnSetAlignment( &_this->BarRight, ViewsImageAlignmentAlignHorzRight 
  | ViewsImageAlignmentAlignVertBottom );
  _this->CurrentValue = 50;
  _this->MinValue = 50;
  _this->MaxValue = 150;
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BarLeft ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BarRight ), 0 );
  _this->SimpleTouchHandler.OnDrag = EwNewSlot( _this, CutterBarGraph_onDragSlot );
  _this->SimpleTouchHandler.OnRelease = EwNewSlot( _this, CutterBarGraph_onReleaseSlot );
  _this->SimpleTouchHandler.OnPress = EwNewSlot( _this, CutterBarGraph_onDragSlot );
  ViewsImage_OnSetBitmap( &_this->BarLeft, EwLoadResource( &CutterBarRaster, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->BarRight, EwLoadResource( &CutterBarRaster, ResourcesBitmap ));

  /* Call the user defined constructor */
  CutterBarGraph_Init( _this, aArg );
}

/* Re-Initializer for the class 'Cutter::BarGraph' */
void CutterBarGraph__ReInit( CutterBarGraph _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandler );
  ViewsImage__ReInit( &_this->BarLeft );
  ViewsImage__ReInit( &_this->BarRight );
}

/* Finalizer method for the class 'Cutter::BarGraph' */
void CutterBarGraph__Done( CutterBarGraph _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterBarGraph );

  /* Finalize all embedded objects */
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandler );
  ViewsImage__Done( &_this->BarLeft );
  ViewsImage__Done( &_this->BarRight );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
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
void CutterBarGraph_UpdateViewState( CutterBarGraph _this, XSet aState )
{
  XInt32 pos;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aState );

  pos = 0;

  if ((( _this->MaxValue > _this->MinValue ) && ( _this->CurrentValue >= _this->MinValue )) 
      && ( _this->CurrentValue <= _this->MaxValue ))
  {
    pos = ( EwGetRectW( _this->Super2.Bounds ) * ( _this->CurrentValue - _this->MinValue )) 
    / ( _this->MaxValue - _this->MinValue );
  }

  CoreRectView__OnSetBounds( &_this->BarLeft, EwSetRectX2( _this->BarLeft.Super1.Bounds, 
  pos ));
  CoreRectView__OnSetBounds( &_this->BarRight, EwSetRectX1( _this->BarRight.Super1.Bounds, 
  pos ));
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void CutterBarGraph_Init( CutterBarGraph _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  CoreRectView__OnSetBounds( _this, EwSetRectSize( _this->Super2.Bounds, _this->BarLeft.Bitmap->FrameSize ));
  CoreRectView__OnSetBounds( &_this->BarLeft, EwNewRect( 0, 0, _this->BarLeft.Bitmap->FrameSize.X, 
  _this->BarLeft.Bitmap->FrameSize.Y ));
  CoreRectView__OnSetBounds( &_this->BarRight, _this->BarLeft.Super1.Bounds );
}

/* 'C' function for method : 'Cutter::BarGraph.OnSetCurrentValue()' */
void CutterBarGraph_OnSetCurrentValue( CutterBarGraph _this, XInt32 value )
{
  if ( value < _this->MinValue )
    value = _this->MinValue;

  if ( value > _this->MaxValue )
    value = _this->MaxValue;

  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Cutter::BarGraph.OnSetMinValue()' */
void CutterBarGraph_OnSetMinValue( CutterBarGraph _this, XInt32 value )
{
  if ( _this->MinValue == value )
    return;

  _this->MinValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Cutter::BarGraph.OnSetMaxValue()' */
void CutterBarGraph_OnSetMaxValue( CutterBarGraph _this, XInt32 value )
{
  if ( _this->MaxValue == value )
    return;

  _this->MaxValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterBarGraph_onDragSlot( CutterBarGraph _this, XObject sender )
{
  XInt32 oldValue;
  XInt32 ofs;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldValue = _this->CurrentValue;
  ofs = _this->SimpleTouchHandler.CurrentPos.X;

  if ((( _this->MaxValue > _this->MinValue ) && ( _this->CurrentValue >= _this->MinValue )) 
      && ( _this->CurrentValue <= _this->MaxValue ))
  {
    ofs = (( ofs * 10 ) / EwGetRectW( _this->Super2.Bounds )) + 1;
    ofs = (( ofs * ( _this->MaxValue - _this->MinValue )) / 10 ) + _this->MinValue;
  }
  else
    ofs = 0;

  CutterBarGraph_OnSetCurrentValue( _this, ofs );

  if ( _this->CurrentValue != oldValue )
  {
    if ( _this->Outlet.Object != 0 )
      EwOnSetInt32( _this->Outlet, _this->CurrentValue );

    EwNotifyRefObservers( _this->Outlet, 0 );
    EwIdleSignal( _this->OnChange, ((XObject)_this ));
  }
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void CutterBarGraph_outletSlot( CutterBarGraph _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    CutterBarGraph_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'Cutter::BarGraph.OnSetOutlet()' */
void CutterBarGraph_OnSetOutlet( CutterBarGraph _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  EwDetachRefObserver( EwNewSlot( _this, CutterBarGraph_outletSlot ), _this->Outlet, 
    0 );
  _this->Outlet = value;
  EwAttachRefObserver( EwNewSlot( _this, CutterBarGraph_outletSlot ), value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, CutterBarGraph_outletSlot ), ((XObject)_this ));
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterBarGraph_onReleaseSlot( CutterBarGraph _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnApply, ((XObject)_this ));
}

/* Variants derived from the class : 'Cutter::BarGraph' */
EW_DEFINE_CLASS_VARIANTS( CutterBarGraph )
EW_END_OF_CLASS_VARIANTS( CutterBarGraph )

/* Virtual Method Table (VMT) for the class : 'Cutter::BarGraph' */
EW_DEFINE_CLASS( CutterBarGraph, CoreGroup, OnApply, OnApply, Outlet, SimpleTouchHandler, 
                 CurrentValue, "Cutter::BarGraph" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CutterBarGraph_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterBarGraph )

/* Initializer for the class 'Cutter::SpeedDialog' */
void CutterSpeedDialog__Init( CutterSpeedDialog _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextSpeed, &_this->_XObject, 0 );
  CutterBarGraph__Init( &_this->BarGraph, &_this->_XObject, 0 );
  CutterSpeedButton__Init( &_this->SpeedButtonL, &_this->_XObject, 0 );
  CutterSpeedButton__Init( &_this->SpeedButtonR, &_this->_XObject, 0 );
  CutterCancelButton__Init( &_this->CancelButton, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text0, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text1, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text2, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text3, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text4, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text5, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text6, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text7, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text8, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text9, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterSpeedDialog );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const005D );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const007C );
  ViewsText_OnSetWrapText( &_this->Text, 0 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0031 ));
  ViewsText_OnSetColor( &_this->Text, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->TextSpeed, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->TextSpeed, _Const007D );
  ViewsText_OnSetWrapText( &_this->TextSpeed, 0 );
  ViewsText_OnSetAlignment( &_this->TextSpeed, ViewsTextAlignmentAlignHorzCenter 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextSpeed, EwLoadString( &_Const007E ));
  ViewsText_OnSetColor( &_this->TextSpeed, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->BarGraph, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->BarGraph, _Const007F );
  CutterBarGraph_OnSetCurrentValue( &_this->BarGraph, 3000 );
  CutterBarGraph_OnSetMinValue( &_this->BarGraph, 600 );
  CutterBarGraph_OnSetMaxValue( &_this->BarGraph, 3600 );
  CoreView_OnSetLayout((CoreView)&_this->SpeedButtonL, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->SpeedButtonL, _Const0080 );
  CutterSpeedButton_OnSetIconIndex( &_this->SpeedButtonL, 0 );
  CoreView_OnSetLayout((CoreView)&_this->SpeedButtonR, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->SpeedButtonR, _Const0081 );
  CutterSpeedButton_OnSetIconIndex( &_this->SpeedButtonR, 1 );
  CoreView_OnSetLayout((CoreView)&_this->CancelButton, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->CancelButton, _Const0066 );
  CoreView_OnSetLayout((CoreView)&_this->Text0, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text0, _Const0082 );
  ViewsText_OnSetString( &_this->Text0, EwLoadString( &_Const0083 ));
  ViewsText_OnSetColor( &_this->Text0, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text1, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text1, _Const0084 );
  ViewsText_OnSetString( &_this->Text1, EwLoadString( &_Const0085 ));
  ViewsText_OnSetColor( &_this->Text1, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text2, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text2, _Const0086 );
  ViewsText_OnSetString( &_this->Text2, EwLoadString( &_Const0087 ));
  ViewsText_OnSetColor( &_this->Text2, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text3, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text3, _Const0088 );
  ViewsText_OnSetString( &_this->Text3, EwLoadString( &_Const0089 ));
  ViewsText_OnSetColor( &_this->Text3, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text4, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text4, _Const008A );
  ViewsText_OnSetString( &_this->Text4, EwLoadString( &_Const008B ));
  ViewsText_OnSetColor( &_this->Text4, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text5, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text5, _Const008C );
  ViewsText_OnSetString( &_this->Text5, EwLoadString( &_Const008D ));
  ViewsText_OnSetColor( &_this->Text5, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text6, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text6, _Const008E );
  ViewsText_OnSetString( &_this->Text6, EwLoadString( &_Const008F ));
  ViewsText_OnSetColor( &_this->Text6, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text7, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text7, _Const0090 );
  ViewsText_OnSetString( &_this->Text7, EwLoadString( &_Const0091 ));
  ViewsText_OnSetColor( &_this->Text7, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text8, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text8, _Const0092 );
  ViewsText_OnSetString( &_this->Text8, EwLoadString( &_Const0093 ));
  ViewsText_OnSetColor( &_this->Text8, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text9, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text9, _Const0094 );
  ViewsText_OnSetString( &_this->Text9, EwLoadString( &_Const0095 ));
  ViewsText_OnSetColor( &_this->Text9, CutterTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextSpeed ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BarGraph ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SpeedButtonL ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SpeedButtonR ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->CancelButton ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text0 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text1 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text2 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text3 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text4 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text5 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text6 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text7 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text8 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text9 ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &CutterDescrFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextSpeed, EwLoadResource( &CutterMediumDigitFont, 
  ResourcesFont ));
  CutterBarGraph_OnSetOutlet( &_this->BarGraph, EwNewRef( EwGetAutoObject( &CutterDevice, 
  CutterDeviceClass ), CutterDeviceClass_OnGetPaperSpeed, CutterDeviceClass_OnSetPaperSpeed ));
  _this->SpeedButtonL.OnChange = EwNewSlot( _this, CutterSpeedDialog_onDecrement );
  _this->SpeedButtonR.OnChange = EwNewSlot( _this, CutterSpeedDialog_onIncrement );
  _this->CancelButton.OnChange = EwNewSlot( _this, CutterSpeedDialog_onCancel );
  ViewsText_OnSetFont( &_this->Text0, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text1, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text2, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text3, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text4, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text5, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text6, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text7, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text8, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text9, EwLoadResource( &CutterButtonFont, ResourcesFont ));

  /* Call the user defined constructor */
  CutterSpeedDialog_Init( _this, aArg );
}

/* Re-Initializer for the class 'Cutter::SpeedDialog' */
void CutterSpeedDialog__ReInit( CutterSpeedDialog _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
  ViewsText__ReInit( &_this->TextSpeed );
  CutterBarGraph__ReInit( &_this->BarGraph );
  CutterSpeedButton__ReInit( &_this->SpeedButtonL );
  CutterSpeedButton__ReInit( &_this->SpeedButtonR );
  CutterCancelButton__ReInit( &_this->CancelButton );
  ViewsText__ReInit( &_this->Text0 );
  ViewsText__ReInit( &_this->Text1 );
  ViewsText__ReInit( &_this->Text2 );
  ViewsText__ReInit( &_this->Text3 );
  ViewsText__ReInit( &_this->Text4 );
  ViewsText__ReInit( &_this->Text5 );
  ViewsText__ReInit( &_this->Text6 );
  ViewsText__ReInit( &_this->Text7 );
  ViewsText__ReInit( &_this->Text8 );
  ViewsText__ReInit( &_this->Text9 );
}

/* Finalizer method for the class 'Cutter::SpeedDialog' */
void CutterSpeedDialog__Done( CutterSpeedDialog _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterSpeedDialog );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );
  ViewsText__Done( &_this->TextSpeed );
  CutterBarGraph__Done( &_this->BarGraph );
  CutterSpeedButton__Done( &_this->SpeedButtonL );
  CutterSpeedButton__Done( &_this->SpeedButtonR );
  CutterCancelButton__Done( &_this->CancelButton );
  ViewsText__Done( &_this->Text0 );
  ViewsText__Done( &_this->Text1 );
  ViewsText__Done( &_this->Text2 );
  ViewsText__Done( &_this->Text3 );
  ViewsText__Done( &_this->Text4 );
  ViewsText__Done( &_this->Text5 );
  ViewsText__Done( &_this->Text6 );
  ViewsText__Done( &_this->Text7 );
  ViewsText__Done( &_this->Text8 );
  ViewsText__Done( &_this->Text9 );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void CutterSpeedDialog_Init( CutterSpeedDialog _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  CoreRectView__OnSetBounds( _this, EwSetRectSize( _this->Super2.Bounds, EwNewPoint( 
  CutterScreenSize.X - ( 2 * CutterBorder.X ), (( CutterScreenSize.Y - CutterBorder.Y ) 
  - 22 ) - 32 )));
}

/* 'C' function for method : 'Cutter::SpeedDialog.OnSetPaperSpeed()' */
void CutterSpeedDialog_OnSetPaperSpeed( CutterSpeedDialog _this, XRef value )
{
  if ( !EwCompRef( value, _this->PaperSpeed ))
    return;

  if ( _this->PaperSpeed.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterSpeedDialog_onUpdate ), _this->PaperSpeed, 
      0 );

  _this->PaperSpeed = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterSpeedDialog_onUpdate ), value, 
      0 );

  EwPostSignal( EwNewSlot( _this, CutterSpeedDialog_onUpdate ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void CutterSpeedDialog_onUpdate( CutterSpeedDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperSpeed.Object != 0 )
    ViewsText_OnSetString( &_this->TextSpeed, EwNewStringInt( EwOnGetInt32( _this->PaperSpeed ), 
    0, 10 ));
}

/* Slot metod to change the paper speed, when user press arrow buttons. */
void CutterSpeedDialog_onIncrement( CutterSpeedDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperSpeed.Object != 0 )
    EwOnSetInt32( _this->PaperSpeed, (( EwOnGetInt32( _this->PaperSpeed ) / 10 ) 
    * 10 ) + 10 );
}

/* Slot metod to change the paper speed, when user press arrow buttons. */
void CutterSpeedDialog_onDecrement( CutterSpeedDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperSpeed.Object != 0 )
    EwOnSetInt32( _this->PaperSpeed, ((( EwOnGetInt32( _this->PaperSpeed ) + 9 ) 
    / 10 ) * 10 ) - 10 );
}

/* Slot method to forward the cancel event. */
void CutterSpeedDialog_onCancel( CutterSpeedDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( _this->OnCancel, ((XObject)_this ));
}

/* Variants derived from the class : 'Cutter::SpeedDialog' */
EW_DEFINE_CLASS_VARIANTS( CutterSpeedDialog )
EW_END_OF_CLASS_VARIANTS( CutterSpeedDialog )

/* Virtual Method Table (VMT) for the class : 'Cutter::SpeedDialog' */
EW_DEFINE_CLASS( CutterSpeedDialog, CoreGroup, OnCancel, OnCancel, PaperSpeed, Text, 
                 _None, "Cutter::SpeedDialog" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterSpeedDialog )

/* Initializer for the class 'Cutter::CancelButton' */
void CutterCancelButton__Init( CutterCancelButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterCancelButton );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0073 );
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->TouchHandler, _Const0074 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->TouchHandler, _Const0075 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->TouchHandler, _Const0076 );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->TouchHandler, _Const0004 );
  CoreView_OnSetLayout((CoreView)&_this->Rectangle, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const0073 );
  ViewsRectangle_OnSetColor( &_this->Rectangle, CutterButtonBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToLeft );
  CoreRectView__OnSetBounds( &_this->Text, _Const0096 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0097 ));
  CoreGroup__Add( _this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  _this->TouchHandler.OnLeave = EwNewSlot( _this, CutterCancelButton_enterLeaveSlot );
  _this->TouchHandler.OnEnter = EwNewSlot( _this, CutterCancelButton_enterLeaveSlot );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, CutterCancelButton_pressReleaseSlot );
  _this->TouchHandler.OnPress = EwNewSlot( _this, CutterCancelButton_pressReleaseSlot );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &CutterButtonFont, ResourcesFont ));
}

/* Re-Initializer for the class 'Cutter::CancelButton' */
void CutterCancelButton__ReInit( CutterCancelButton _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
  ViewsRectangle__ReInit( &_this->Rectangle );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Cutter::CancelButton' */
void CutterCancelButton__Done( CutterCancelButton _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterCancelButton );

  /* Finalize all embedded objects */
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );
  ViewsRectangle__Done( &_this->Rectangle );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
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
void CutterCancelButton_UpdateViewState( CutterCancelButton _this, XSet aState )
{
  if ((( aState & CoreViewStateEnabled ) == CoreViewStateEnabled ))
  {
    if ( _this->TouchHandler.Down && _this->TouchHandler.Inside )
    {
      ViewsText_OnSetColor( &_this->Text, CutterTextColor );
      ViewsRectangle_OnSetColor( &_this->Rectangle, CutterSelectColor );
    }
    else
    {
      ViewsText_OnSetColor( &_this->Text, CutterButtonTextColor );
      ViewsRectangle_OnSetColor( &_this->Rectangle, CutterButtonBgColor );
    }
  }
  else
  {
    ViewsText_OnSetColor( &_this->Text, CutterSelectColor );
    ViewsRectangle_OnSetColor( &_this->Rectangle, CutterBackColor );
  }
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterCancelButton_enterLeaveSlot( CutterCancelButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterCancelButton_pressReleaseSlot( CutterCancelButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->TouchHandler.Down && _this->TouchHandler.Inside )
  {
    EwIdleSignal( _this->OnChange, ((XObject)_this ));
  }
}

/* Variants derived from the class : 'Cutter::CancelButton' */
EW_DEFINE_CLASS_VARIANTS( CutterCancelButton )
EW_END_OF_CLASS_VARIANTS( CutterCancelButton )

/* Virtual Method Table (VMT) for the class : 'Cutter::CancelButton' */
EW_DEFINE_CLASS( CutterCancelButton, CoreGroup, OnChange, OnChange, TouchHandler, 
                 TouchHandler, _None, "Cutter::CancelButton" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CutterCancelButton_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterCancelButton )

/* Initializer for the class 'Cutter::BatchDialog' */
void CutterBatchDialog__Init( CutterBatchDialog _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextBatch, &_this->_XObject, 0 );
  CutterBarGraph__Init( &_this->BarGraph, &_this->_XObject, 0 );
  CutterSpeedButton__Init( &_this->SpeedButtonL, &_this->_XObject, 0 );
  CutterSpeedButton__Init( &_this->SpeedButtonR, &_this->_XObject, 0 );
  CutterCancelButton__Init( &_this->CancelButton, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text0, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text1, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text2, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text3, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text4, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text5, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text6, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text7, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text8, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text9, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterBatchDialog );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const005D );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const007C );
  ViewsText_OnSetWrapText( &_this->Text, 0 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0027 ));
  ViewsText_OnSetColor( &_this->Text, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->TextBatch, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->TextBatch, _Const007D );
  ViewsText_OnSetWrapText( &_this->TextBatch, 0 );
  ViewsText_OnSetAlignment( &_this->TextBatch, ViewsTextAlignmentAlignHorzCenter 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextBatch, EwLoadString( &_Const0029 ));
  ViewsText_OnSetColor( &_this->TextBatch, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->BarGraph, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->BarGraph, _Const007F );
  CutterBarGraph_OnSetCurrentValue( &_this->BarGraph, 500 );
  CutterBarGraph_OnSetMinValue( &_this->BarGraph, 0 );
  CutterBarGraph_OnSetMaxValue( &_this->BarGraph, 1000 );
  CoreView_OnSetLayout((CoreView)&_this->SpeedButtonL, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->SpeedButtonL, _Const0080 );
  CutterSpeedButton_OnSetIconIndex( &_this->SpeedButtonL, 0 );
  CoreView_OnSetLayout((CoreView)&_this->SpeedButtonR, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->SpeedButtonR, _Const0081 );
  CutterSpeedButton_OnSetIconIndex( &_this->SpeedButtonR, 1 );
  CoreView_OnSetLayout((CoreView)&_this->CancelButton, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->CancelButton, _Const0066 );
  CoreView_OnSetLayout((CoreView)&_this->Text0, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text0, _Const0082 );
  ViewsText_OnSetString( &_this->Text0, EwLoadString( &_Const0098 ));
  ViewsText_OnSetColor( &_this->Text0, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text1, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text1, _Const0084 );
  ViewsText_OnSetString( &_this->Text1, EwLoadString( &_Const0099 ));
  ViewsText_OnSetColor( &_this->Text1, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text2, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text2, _Const0086 );
  ViewsText_OnSetString( &_this->Text2, EwLoadString( &_Const009A ));
  ViewsText_OnSetColor( &_this->Text2, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text3, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text3, _Const0088 );
  ViewsText_OnSetString( &_this->Text3, EwLoadString( &_Const009B ));
  ViewsText_OnSetColor( &_this->Text3, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text4, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text4, _Const008A );
  ViewsText_OnSetString( &_this->Text4, EwLoadString( &_Const009C ));
  ViewsText_OnSetColor( &_this->Text4, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text5, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text5, _Const008C );
  ViewsText_OnSetString( &_this->Text5, EwLoadString( &_Const009D ));
  ViewsText_OnSetColor( &_this->Text5, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text6, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text6, _Const008E );
  ViewsText_OnSetString( &_this->Text6, EwLoadString( &_Const009E ));
  ViewsText_OnSetColor( &_this->Text6, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text7, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text7, _Const0090 );
  ViewsText_OnSetString( &_this->Text7, EwLoadString( &_Const009F ));
  ViewsText_OnSetColor( &_this->Text7, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text8, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text8, _Const0092 );
  ViewsText_OnSetString( &_this->Text8, EwLoadString( &_Const0083 ));
  ViewsText_OnSetColor( &_this->Text8, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text9, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text9, _Const0094 );
  ViewsText_OnSetString( &_this->Text9, EwLoadString( &_Const00A0 ));
  ViewsText_OnSetColor( &_this->Text9, CutterTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TextBatch ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BarGraph ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SpeedButtonL ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SpeedButtonR ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->CancelButton ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text0 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text1 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text2 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text3 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text4 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text5 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text6 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text7 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text8 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text9 ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &CutterDescrFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextBatch, EwLoadResource( &CutterMediumDigitFont, 
  ResourcesFont ));
  CutterBarGraph_OnSetOutlet( &_this->BarGraph, EwNewRef( EwGetAutoObject( &CutterDevice, 
  CutterDeviceClass ), CutterDeviceClass_OnGetPaperBatch, CutterDeviceClass_OnSetPaperBatch ));
  _this->SpeedButtonL.OnChange = EwNewSlot( _this, CutterBatchDialog_onDecrement );
  _this->SpeedButtonR.OnChange = EwNewSlot( _this, CutterBatchDialog_onIncrement );
  _this->CancelButton.OnChange = EwNewSlot( _this, CutterBatchDialog_onCancel );
  ViewsText_OnSetFont( &_this->Text0, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text1, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text2, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text3, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text4, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text5, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text6, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text7, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text8, EwLoadResource( &CutterButtonFont, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Text9, EwLoadResource( &CutterButtonFont, ResourcesFont ));

  /* Call the user defined constructor */
  CutterBatchDialog_Init( _this, aArg );
}

/* Re-Initializer for the class 'Cutter::BatchDialog' */
void CutterBatchDialog__ReInit( CutterBatchDialog _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
  ViewsText__ReInit( &_this->TextBatch );
  CutterBarGraph__ReInit( &_this->BarGraph );
  CutterSpeedButton__ReInit( &_this->SpeedButtonL );
  CutterSpeedButton__ReInit( &_this->SpeedButtonR );
  CutterCancelButton__ReInit( &_this->CancelButton );
  ViewsText__ReInit( &_this->Text0 );
  ViewsText__ReInit( &_this->Text1 );
  ViewsText__ReInit( &_this->Text2 );
  ViewsText__ReInit( &_this->Text3 );
  ViewsText__ReInit( &_this->Text4 );
  ViewsText__ReInit( &_this->Text5 );
  ViewsText__ReInit( &_this->Text6 );
  ViewsText__ReInit( &_this->Text7 );
  ViewsText__ReInit( &_this->Text8 );
  ViewsText__ReInit( &_this->Text9 );
}

/* Finalizer method for the class 'Cutter::BatchDialog' */
void CutterBatchDialog__Done( CutterBatchDialog _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterBatchDialog );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );
  ViewsText__Done( &_this->TextBatch );
  CutterBarGraph__Done( &_this->BarGraph );
  CutterSpeedButton__Done( &_this->SpeedButtonL );
  CutterSpeedButton__Done( &_this->SpeedButtonR );
  CutterCancelButton__Done( &_this->CancelButton );
  ViewsText__Done( &_this->Text0 );
  ViewsText__Done( &_this->Text1 );
  ViewsText__Done( &_this->Text2 );
  ViewsText__Done( &_this->Text3 );
  ViewsText__Done( &_this->Text4 );
  ViewsText__Done( &_this->Text5 );
  ViewsText__Done( &_this->Text6 );
  ViewsText__Done( &_this->Text7 );
  ViewsText__Done( &_this->Text8 );
  ViewsText__Done( &_this->Text9 );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void CutterBatchDialog_Init( CutterBatchDialog _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  CoreRectView__OnSetBounds( _this, EwSetRectSize( _this->Super2.Bounds, EwNewPoint( 
  CutterScreenSize.X - ( 2 * CutterBorder.X ), (( CutterScreenSize.Y - CutterBorder.Y ) 
  - 22 ) - 32 )));
}

/* 'C' function for method : 'Cutter::BatchDialog.OnSetPaperBatch()' */
void CutterBatchDialog_OnSetPaperBatch( CutterBatchDialog _this, XRef value )
{
  if ( !EwCompRef( value, _this->PaperBatch ))
    return;

  if ( _this->PaperBatch.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterBatchDialog_onUpdate ), _this->PaperBatch, 
      0 );

  _this->PaperBatch = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterBatchDialog_onUpdate ), value, 
      0 );

  EwPostSignal( EwNewSlot( _this, CutterBatchDialog_onUpdate ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void CutterBatchDialog_onUpdate( CutterBatchDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperBatch.Object != 0 )
    ViewsText_OnSetString( &_this->TextBatch, EwNewStringInt( EwOnGetInt32( _this->PaperBatch ), 
    0, 10 ));
}

/* Slot metod to change the paper speed, when user press arrow buttons. */
void CutterBatchDialog_onIncrement( CutterBatchDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperBatch.Object != 0 )
    EwOnSetInt32( _this->PaperBatch, (( EwOnGetInt32( _this->PaperBatch ) / 10 ) 
    * 10 ) + 10 );
}

/* Slot metod to change the paper speed, when user press arrow buttons. */
void CutterBatchDialog_onDecrement( CutterBatchDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperBatch.Object != 0 )
    EwOnSetInt32( _this->PaperBatch, ((( EwOnGetInt32( _this->PaperBatch ) + 9 ) 
    / 10 ) * 10 ) - 10 );
}

/* Slot method to forward the cancel event. */
void CutterBatchDialog_onCancel( CutterBatchDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( _this->OnCancel, ((XObject)_this ));
}

/* Variants derived from the class : 'Cutter::BatchDialog' */
EW_DEFINE_CLASS_VARIANTS( CutterBatchDialog )
EW_END_OF_CLASS_VARIANTS( CutterBatchDialog )

/* Virtual Method Table (VMT) for the class : 'Cutter::BatchDialog' */
EW_DEFINE_CLASS( CutterBatchDialog, CoreGroup, OnCancel, OnCancel, PaperBatch, Text, 
                 _None, "Cutter::BatchDialog" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterBatchDialog )

/* Initializer for the class 'Cutter::PaperFeed' */
void CutterPaperFeed__Init( CutterPaperFeed _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->StripeRight, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->StripeLeft, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterPaperFeed );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const00A1 );
  CoreView_OnSetLayout((CoreView)&_this->StripeRight, CoreLayoutAlignToLeft | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz );
  CoreRectView__OnSetBounds( &_this->StripeRight, _Const00A2 );
  ViewsImage_OnSetAlignment( &_this->StripeRight, ViewsImageAlignmentAlignHorzRight 
  | ViewsImageAlignmentAlignVertCenter );
  CoreRectView__OnSetBounds( &_this->StripeLeft, _Const00A3 );
  ViewsImage_OnSetAlignment( &_this->StripeLeft, ViewsImageAlignmentAlignHorzLeft 
  | ViewsImageAlignmentAlignVertCenter );
  CoreGroup__Add( _this, ((CoreView)&_this->StripeRight ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->StripeLeft ), 0 );
  ViewsImage_OnSetBitmap( &_this->StripeRight, EwLoadResource( &CutterPaperStripe, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->StripeLeft, EwLoadResource( &CutterPaperStripe, 
  ResourcesBitmap ));
}

/* Re-Initializer for the class 'Cutter::PaperFeed' */
void CutterPaperFeed__ReInit( CutterPaperFeed _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->StripeRight );
  ViewsImage__ReInit( &_this->StripeLeft );
}

/* Finalizer method for the class 'Cutter::PaperFeed' */
void CutterPaperFeed__Done( CutterPaperFeed _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterPaperFeed );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->StripeRight );
  ViewsImage__Done( &_this->StripeLeft );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void CutterPaperFeed_UpdateLayout( CutterPaperFeed _this, XPoint aSize )
{
  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );
  ViewsImage_OnSetVisible( &_this->StripeLeft, (XBool)( aSize.X <= _this->StripeRight.Bitmap->FrameSize.X ));
}

/* 'C' function for method : 'Cutter::PaperFeed.OnSetType()' */
void CutterPaperFeed_OnSetType( CutterPaperFeed _this, XInt32 value )
{
  if ( _this->Type < 0 )
    CutterPaperFeed_OnSetType( _this, 0 );

  if ( _this->Type > 3 )
    CutterPaperFeed_OnSetType( _this, 3 );

  _this->Type = value;
  ViewsImage_OnSetFrameNumber( &_this->StripeLeft, value );
  ViewsImage_OnSetFrameNumber( &_this->StripeRight, value );
}

/* Variants derived from the class : 'Cutter::PaperFeed' */
EW_DEFINE_CLASS_VARIANTS( CutterPaperFeed )
EW_END_OF_CLASS_VARIANTS( CutterPaperFeed )

/* Virtual Method Table (VMT) for the class : 'Cutter::PaperFeed' */
EW_DEFINE_CLASS( CutterPaperFeed, CoreGroup, StripeRight, StripeRight, StripeRight, 
                 StripeRight, Type, "Cutter::PaperFeed" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CutterPaperFeed_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterPaperFeed )

/* Initializer for the class 'Cutter::FeedDialog' */
void CutterFeedDialog__Init( CutterFeedDialog _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  CutterCancelButton__Init( &_this->CancelButton, &_this->_XObject, 0 );
  CutterFeedButton__Init( &_this->FeedButton0, &_this->_XObject, 0 );
  CutterFeedButton__Init( &_this->FeedButton1, &_this->_XObject, 0 );
  CutterFeedButton__Init( &_this->FeedButton2, &_this->_XObject, 0 );
  CutterFeedButton__Init( &_this->FeedButton3, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterFeedDialog );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const005D );
  CoreView_OnSetLayout((CoreView)&_this->CancelButton, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->CancelButton, _Const0066 );
  CoreView_OnSetLayout((CoreView)&_this->FeedButton0, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->FeedButton0, _Const00A4 );
  CoreView_OnSetLayout((CoreView)&_this->FeedButton1, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->FeedButton1, _Const00A5 );
  CutterFeedButton_OnSetType( &_this->FeedButton1, 1 );
  CoreView_OnSetLayout((CoreView)&_this->FeedButton2, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->FeedButton2, _Const00A6 );
  CutterFeedButton_OnSetType( &_this->FeedButton2, 2 );
  CoreView_OnSetLayout((CoreView)&_this->FeedButton3, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->FeedButton3, _Const00A7 );
  CutterFeedButton_OnSetType( &_this->FeedButton3, 3 );
  CoreGroup__Add( _this, ((CoreView)&_this->CancelButton ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->FeedButton0 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->FeedButton1 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->FeedButton2 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->FeedButton3 ), 0 );
  _this->CancelButton.OnChange = EwNewSlot( _this, CutterFeedDialog_onCancel );
  _this->FeedButton0.OnChange = EwNewSlot( _this, CutterFeedDialog_onSelect );
  _this->FeedButton1.OnChange = EwNewSlot( _this, CutterFeedDialog_onSelect );
  _this->FeedButton2.OnChange = EwNewSlot( _this, CutterFeedDialog_onSelect );
  _this->FeedButton3.OnChange = EwNewSlot( _this, CutterFeedDialog_onSelect );

  /* Call the user defined constructor */
  CutterFeedDialog_Init( _this, aArg );
}

/* Re-Initializer for the class 'Cutter::FeedDialog' */
void CutterFeedDialog__ReInit( CutterFeedDialog _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CutterCancelButton__ReInit( &_this->CancelButton );
  CutterFeedButton__ReInit( &_this->FeedButton0 );
  CutterFeedButton__ReInit( &_this->FeedButton1 );
  CutterFeedButton__ReInit( &_this->FeedButton2 );
  CutterFeedButton__ReInit( &_this->FeedButton3 );
}

/* Finalizer method for the class 'Cutter::FeedDialog' */
void CutterFeedDialog__Done( CutterFeedDialog _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterFeedDialog );

  /* Finalize all embedded objects */
  CutterCancelButton__Done( &_this->CancelButton );
  CutterFeedButton__Done( &_this->FeedButton0 );
  CutterFeedButton__Done( &_this->FeedButton1 );
  CutterFeedButton__Done( &_this->FeedButton2 );
  CutterFeedButton__Done( &_this->FeedButton3 );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void CutterFeedDialog_Init( CutterFeedDialog _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  CoreRectView__OnSetBounds( _this, EwSetRectSize( _this->Super2.Bounds, EwNewPoint( 
  CutterScreenSize.X - ( 2 * CutterBorder.X ), (( CutterScreenSize.Y - CutterBorder.Y ) 
  - 22 ) - 32 )));
}

/* 'C' function for method : 'Cutter::FeedDialog.OnSetPaperType()' */
void CutterFeedDialog_OnSetPaperType( CutterFeedDialog _this, XRef value )
{
  if ( !EwCompRef( value, _this->PaperType ))
    return;

  if ( _this->PaperType.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, CutterFeedDialog_onUpdate ), _this->PaperType, 
      0 );

  _this->PaperType = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, CutterFeedDialog_onUpdate ), value, 0 );

  EwPostSignal( EwNewSlot( _this, CutterFeedDialog_onUpdate ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void CutterFeedDialog_onUpdate( CutterFeedDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->PaperType.Object != 0 )
  {
    CutterFeedButton_OnSetSelected( &_this->FeedButton0, (XBool)( EwOnGetInt32( 
    _this->PaperType ) == 0 ));
    CutterFeedButton_OnSetSelected( &_this->FeedButton1, (XBool)( EwOnGetInt32( 
    _this->PaperType ) == 1 ));
    CutterFeedButton_OnSetSelected( &_this->FeedButton2, (XBool)( EwOnGetInt32( 
    _this->PaperType ) == 2 ));
    CutterFeedButton_OnSetSelected( &_this->FeedButton3, (XBool)( EwOnGetInt32( 
    _this->PaperType ) == 3 ));
  }
}

/* Slot method to forward the cancel event. */
void CutterFeedDialog_onCancel( CutterFeedDialog _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( _this->OnCancel, ((XObject)_this ));
}

/* This is a slot method. */
void CutterFeedDialog_onSelect( CutterFeedDialog _this, XObject sender )
{
  CutterFeedButton button = EwCastObject( sender, CutterFeedButton );

  if (( button != 0 ) && ( _this->PaperType.Object != 0 ))
    EwOnSetInt32( _this->PaperType, button->Type );
}

/* Variants derived from the class : 'Cutter::FeedDialog' */
EW_DEFINE_CLASS_VARIANTS( CutterFeedDialog )
EW_END_OF_CLASS_VARIANTS( CutterFeedDialog )

/* Virtual Method Table (VMT) for the class : 'Cutter::FeedDialog' */
EW_DEFINE_CLASS( CutterFeedDialog, CoreGroup, OnCancel, OnCancel, PaperType, CancelButton, 
                 _None, "Cutter::FeedDialog" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterFeedDialog )

/* Initializer for the class 'Cutter::FeedButton' */
void CutterFeedButton__Init( CutterFeedButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* ... then construct all embedded objects */
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  CutterPaperFeed__Init( &_this->PaperFeed, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( CutterFeedButton );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const00A8 );
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->TouchHandler, _Const00A9 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->TouchHandler, _Const00AA );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->TouchHandler, _Const00AB );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->TouchHandler, _Const0004 );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const00A8 );
  ViewsRectangle_OnSetColor( &_this->Rectangle, CutterBackColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const00AC );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const000C ));
  ViewsText_OnSetColor( &_this->Text, CutterTextColor );
  CoreView_OnSetLayout((CoreView)&_this->PaperFeed, CoreLayoutResizeHorz );
  CoreRectView__OnSetBounds( &_this->PaperFeed, _Const00AD );
  CutterPaperFeed_OnSetType( &_this->PaperFeed, 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->PaperFeed ), 0 );
  _this->TouchHandler.OnLeave = EwNewSlot( _this, CutterFeedButton_enterLeaveSlot );
  _this->TouchHandler.OnEnter = EwNewSlot( _this, CutterFeedButton_enterLeaveSlot );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, CutterFeedButton_pressReleaseSlot );
  _this->TouchHandler.OnPress = EwNewSlot( _this, CutterFeedButton_pressReleaseSlot );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &CutterDescrFont, ResourcesFont ));
}

/* Re-Initializer for the class 'Cutter::FeedButton' */
void CutterFeedButton__ReInit( CutterFeedButton _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
  ViewsRectangle__ReInit( &_this->Rectangle );
  ViewsText__ReInit( &_this->Text );
  CutterPaperFeed__ReInit( &_this->PaperFeed );
}

/* Finalizer method for the class 'Cutter::FeedButton' */
void CutterFeedButton__Done( CutterFeedButton _this )
{
  /* Finalize this class */
  _this->_VMT = EW_CLASS( CutterFeedButton );

  /* Finalize all embedded objects */
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );
  ViewsRectangle__Done( &_this->Rectangle );
  ViewsText__Done( &_this->Text );
  CutterPaperFeed__Done( &_this->PaperFeed );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
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
void CutterFeedButton_UpdateViewState( CutterFeedButton _this, XSet aState )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aState );

  if ( _this->TouchHandler.Down && _this->TouchHandler.Inside )
  {
    ViewsText_OnSetColor( &_this->Text, CutterButtonTextColor );
    ViewsRectangle_OnSetColor( &_this->Rectangle, CutterSelectColor );
  }
  else
  {
    ViewsText_OnSetColor( &_this->Text, CutterButtonTextColor );
    ViewsRectangle_OnSetColor( &_this->Rectangle, CutterBackColor );
  }

  if ( _this->Selected )
  {
    ViewsText_OnSetColor( &_this->Text, CutterTextColor );
    ViewsRectangle_OnSetColor( &_this->Rectangle, CutterSelectColor );
  }
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterFeedButton_enterLeaveSlot( CutterFeedButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterFeedButton_pressReleaseSlot( CutterFeedButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->TouchHandler.Down && _this->TouchHandler.Inside )
  {
    EwSignal( _this->OnChange, ((XObject)_this ));
  }
}

/* 'C' function for method : 'Cutter::FeedButton.OnSetType()' */
void CutterFeedButton_OnSetType( CutterFeedButton _this, XInt32 value )
{
  if ( _this->Type < 0 )
    CutterFeedButton_OnSetType( _this, 0 );

  if ( _this->Type > 3 )
    CutterFeedButton_OnSetType( _this, 3 );

  _this->Type = value;
  CutterPaperFeed_OnSetType( &_this->PaperFeed, value );

  switch ( value )
  {
    case 0 :
      ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const000C ));
    break;

    case 1 :
      ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const000F ));
    break;

    case 2 :
      ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0010 ));
    break;

    case 3 :
      ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0011 ));
    break;

    default : 
      ;
  }
}

/* 'C' function for method : 'Cutter::FeedButton.OnSetSelected()' */
void CutterFeedButton_OnSetSelected( CutterFeedButton _this, XBool value )
{
  _this->Selected = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* Variants derived from the class : 'Cutter::FeedButton' */
EW_DEFINE_CLASS_VARIANTS( CutterFeedButton )
EW_END_OF_CLASS_VARIANTS( CutterFeedButton )

/* Virtual Method Table (VMT) for the class : 'Cutter::FeedButton' */
EW_DEFINE_CLASS( CutterFeedButton, CoreGroup, OnChange, OnChange, TouchHandler, 
                 TouchHandler, Type, "Cutter::FeedButton" )
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
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CutterFeedButton_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_Restack,
  CoreGroup_Add,
EW_END_OF_CLASS( CutterFeedButton )

/* Embedded Wizard */
