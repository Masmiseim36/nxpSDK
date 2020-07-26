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
* Version  : 9.30
* Date     : 14.02.2020  8:00:50
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
  0x000003BC, /* ratio 52.30 % */
  0xB8001500, 0x80086452, 0x00E00030, 0x0D2003A0, 0xDC003780, 0x60789800, 0x00231160,
  0x400464CA, 0x0A802266, 0x4800A116, 0x0009C002, 0xA400458C, 0xA0400224, 0x4E5294C8,
  0x20027802, 0x54001066, 0x39600254, 0x91C84331, 0x5CA66525, 0x4D2832D9, 0xBC002D93,
  0xA69329AC, 0x793A9C4D, 0xD249FC7E, 0x539D1324, 0xA9756400, 0xA7462633, 0x24E66E00,
  0xD06A93DA, 0x6EB329AB, 0xE9341A35, 0x9933AF52, 0x11EA8D8A, 0x759A4582, 0x90042616,
  0x99082197, 0x6800E510, 0x44EF5073, 0x00AF1604, 0x89139965, 0x1D1604C7, 0x047924A7,
  0x879CE380, 0xB000E62D, 0xCFE70599, 0xF4580913, 0x90A0999C, 0x00C70CC2, 0xE7F8F340,
  0x3D5AB7CD, 0x79FCD01A, 0x160644F3, 0x8C246A63, 0xDF44A1D0, 0x4478CE00, 0x42AB304C,
  0x4E3B1A8D, 0xE4504BB4, 0xFFB1AEEB, 0xD7F000BE, 0x4C2ACD31, 0x01945A29, 0x1A000660,
  0xB70C6A00, 0x42219598, 0x17B50CC1, 0xE1510BD7, 0x017702C4, 0x304F192C, 0x3AEE0006,
  0x28F77FBA, 0xF7FE609A, 0xDDE4DD16, 0xB6240D44, 0xFD8E5418, 0x0C60C5B5, 0x0391E176,
  0x9944E991, 0xC8295065, 0x4E0E6120, 0x2C458144, 0x529D4555, 0xD3088161, 0xD5315F87,
  0x65398844, 0x37412005, 0x1162B781, 0xE2F78937, 0x0478364D, 0xDC008B1F, 0x00806360,
  0x4551E0E0, 0x11A4E504, 0x0001861E, 0x71F61164, 0x008B9DE7, 0x66D046CC, 0xA5100030,
  0x27DE487A, 0xDA5495E5, 0x53945E27, 0x7D91691A, 0xE3F9723E, 0x27D80076, 0x81DC9526,
  0x0497A677, 0x9AE3A771, 0x66277A47, 0x07EA7197, 0xD5DE45A7, 0x046426B8, 0x44908419,
  0x86C469AA, 0xD95E5F6C, 0x704593A5, 0x72A226B7, 0x45A4D962, 0xE88A3A6A, 0xA45A9296,
  0xBD04A567, 0x61116989, 0x99A47A3A, 0x6A247A86, 0x00000040, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XRect _Const0000 = {{ 0, 0 }, { 380, 200 }};
static const XPoint _Const0001 = { 0, 200 };
static const XPoint _Const0002 = { 380, 200 };
static const XPoint _Const0003 = { 380, 0 };
static const XPoint _Const0004 = { 0, 0 };
static const XRect _Const0005 = {{ 0, 0 }, { 300, 40 }};
static const XRect _Const0006 = {{ 20, 0 }, { 300, 40 }};
static const XStringRes _Const0007 = { _StringsDefault0, 0x0002 };
static const XRect _Const0008 = {{ 0, 0 }, { 230, 114 }};
static const XRect _Const0009 = {{ 0, 0 }, { 300, 140 }};
static const XStringRes _Const000A = { _StringsDefault0, 0x000C };
static const XRect _Const000B = {{ 110, 80 }, { 230, 114 }};
static const XStringRes _Const000C = { _StringsDefault0, 0x0013 };
static const XRect _Const000D = {{ 0, 20 }, { 194, 92 }};
static const XPoint _Const000E = { 0, 22 };
static const XStringRes _Const000F = { _StringsDefault0, 0x0028 };
static const XStringRes _Const0010 = { _StringsDefault0, 0x0041 };
static const XStringRes _Const0011 = { _StringsDefault0, 0x0056 };
static const XStringRes _Const0012 = { _StringsDefault0, 0x006F };
static const XRect _Const0013 = {{ 0, 50 }, { 92, 99 }};
static const XRect _Const0014 = {{ 96, 46 }, { 225, 71 }};
static const XStringRes _Const0015 = { _StringsDefault0, 0x007F };
static const XRect _Const0016 = {{ 96, 76 }, { 225, 102 }};
static const XStringRes _Const0017 = { _StringsDefault0, 0x0087 };
static const XRect _Const0018 = {{ 139, 30 }, { 230, 76 }};
static const XStringRes _Const0019 = { _StringsDefault0, 0x0090 };
static const XRect _Const001A = {{ 139, 60 }, { 230, 108 }};
static const XStringRes _Const001B = { _StringsDefault0, 0x0098 };
static const XRect _Const001C = {{ 0, 0 }, { 480, 272 }};
static const XRect _Const001D = {{ 0, 0 }, { 100, 100 }};
static const XColor _Const001E = { 0x75, 0x7C, 0x62, 0xFF };
static const XRect _Const001F = {{ 480, 20 }, { 750, 220 }};
static const XRect _Const0020 = {{ 310, 60 }, { 580, 260 }};
static const XRect _Const0021 = {{ 210, 120 }, { 470, 320 }};
static const XRect _Const0022 = {{ 260, 190 }, { 560, 390 }};
static const XRect _Const0023 = {{ 360, 260 }, { 780, 460 }};
static const XStringRes _Const0024 = { _StringsDefault0, 0x009C };
static const XRect _Const0025 = {{ 25, 20 }, { 85, 110 }};
static const XRect _Const0026 = {{ 103, 80 }, { 224, 98 }};
static const XStringRes _Const0027 = { _StringsDefault0, 0x00A4 };
static const XRect _Const0028 = {{ 101, 15 }, { 230, 89 }};
static const XStringRes _Const0029 = { _StringsDefault0, 0x00AD };
static const XRect _Const002A = {{ 101, 25 }, { 196, 80 }};
static const XStringRes _Const002B = { _StringsDefault0, 0x00B3 };
static const XRect _Const002C = {{ 20, 80 }, { 61, 94 }};
static const XRect _Const002D = {{ 20, 20 }, { 101, 82 }};
static const XRect _Const002E = {{ 0, 0 }, { 308, 114 }};
static const XStringRes _Const002F = { _StringsDefault0, 0x00BF };
static const XRect _Const0030 = {{ 140, 85 }, { 221, 99 }};
static const XStringRes _Const0031 = { _StringsDefault0, 0x00C7 };
static const XRect _Const0032 = {{ 67, 40 }, { 119, 92 }};
static const XRect _Const0033 = {{ 140, 25 }, { 248, 87 }};
static const XStringRes _Const0034 = { _StringsDefault0, 0x00D7 };
static const XRect _Const0035 = {{ 2, 24 }, { 42, 111 }};
static const XRect _Const0036 = {{ 266, 24 }, { 306, 111 }};
static const XRect _Const0037 = {{ 0, 0 }, { 155, 155 }};
static const XRect _Const0038 = {{ 15, 15 }, { 155, 155 }};
static const XStringRes _Const0039 = { _StringsDefault0, 0x00DE };
static const XPoint _Const003A = { 15, 155 };
static const XPoint _Const003B = { 155, 155 };
static const XPoint _Const003C = { 155, 15 };
static const XPoint _Const003D = { 15, 15 };
static const XStringRes _Const003E = { _StringsDefault0, 0x00E6 };
static const XStringRes _Const003F = { _StringsDefault0, 0x00ED };
static const XRect _Const0040 = {{ 0, 0 }, { 400, 50 }};
static const XRect _Const0041 = {{ 0, 0 }, { 133, 50 }};
static const XStringRes _Const0042 = { _StringsDefault0, 0x00F2 };
static const XRect _Const0043 = {{ 133, 0 }, { 189, 50 }};
static const XRect _Const0044 = {{ 184, 0 }, { 240, 50 }};
static const XStringRes _Const0045 = { _StringsDefault0, 0x00FA };
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
static const XStringRes _Const005F = { _StringsDefault0, 0x00FE };
static const XRect _Const0060 = {{ 380, 200 }, { 550, 230 }};
static const XStringRes _Const0061 = { _StringsDefault0, 0x0103 };
static const XRect _Const0062 = {{ 560, 200 }, { 730, 230 }};
static const XStringRes _Const0063 = { _StringsDefault0, 0x0110 };
static const XRect _Const0064 = {{ 380, 225 }, { 550, 295 }};
static const XRect _Const0065 = {{ 560, 225 }, { 730, 295 }};
static const XRect _Const0066 = {{ 463, 317 }, { 770, 362 }};
static const XRect _Const0067 = {{ 380, 10 }, { 720, 40 }};
static const XStringRes _Const0068 = { _StringsDefault0, 0x011E };
static const XRect _Const0069 = {{ 560, 40 }, { 730, 76 }};
static const XStringRes _Const006A = { _StringsDefault0, 0x0132 };
static const XRect _Const006B = {{ 380, 86 }, { 550, 126 }};
static const XStringRes _Const006C = { _StringsDefault0, 0x0137 };
static const XRect _Const006D = {{ 560, 86 }, { 730, 126 }};
static const XStringRes _Const006E = { _StringsDefault0, 0x013C };
static const XRect _Const006F = {{ 380, 136 }, { 550, 172 }};
static const XStringRes _Const0070 = { _StringsDefault0, 0x0141 };
static const XRect _Const0071 = {{ 560, 136 }, { 730, 172 }};
static const XStringRes _Const0072 = { _StringsDefault0, 0x0146 };
static const XRect _Const0073 = {{ 0, 0 }, { 170, 50 }};
static const XPoint _Const0074 = { 0, 50 };
static const XPoint _Const0075 = { 170, 50 };
static const XPoint _Const0076 = { 170, 0 };
static const XStringRes _Const0077 = { _StringsDefault0, 0x014B };
static const XRect _Const0078 = {{ 0, 0 }, { 698, 118 }};
static const XPoint _Const0079 = { 0, 118 };
static const XPoint _Const007A = { 698, 118 };
static const XPoint _Const007B = { 698, 0 };
static const XRect _Const007C = {{ 310, 90 }, { 460, 120 }};
static const XRect _Const007D = {{ 310, 20 }, { 460, 90 }};
static const XStringRes _Const007E = { _StringsDefault0, 0x0152 };
static const XRect _Const007F = {{ 37, 130 }, { 735, 251 }};
static const XRect _Const0080 = {{ 250, 29 }, { 310, 89 }};
static const XRect _Const0081 = {{ 460, 29 }, { 520, 89 }};
static const XRect _Const0082 = {{ 36, 263 }, { 106, 283 }};
static const XStringRes _Const0083 = { _StringsDefault0, 0x0159 };
static const XRect _Const0084 = {{ 106, 263 }, { 176, 283 }};
static const XStringRes _Const0085 = { _StringsDefault0, 0x015F };
static const XRect _Const0086 = {{ 176, 263 }, { 246, 283 }};
static const XStringRes _Const0087 = { _StringsDefault0, 0x0166 };
static const XRect _Const0088 = {{ 246, 263 }, { 316, 283 }};
static const XStringRes _Const0089 = { _StringsDefault0, 0x016D };
static const XRect _Const008A = {{ 316, 263 }, { 386, 283 }};
static const XStringRes _Const008B = { _StringsDefault0, 0x0174 };
static const XRect _Const008C = {{ 386, 263 }, { 456, 283 }};
static const XStringRes _Const008D = { _StringsDefault0, 0x017B };
static const XRect _Const008E = {{ 456, 263 }, { 526, 283 }};
static const XStringRes _Const008F = { _StringsDefault0, 0x0182 };
static const XRect _Const0090 = {{ 526, 263 }, { 596, 283 }};
static const XStringRes _Const0091 = { _StringsDefault0, 0x0189 };
static const XRect _Const0092 = {{ 596, 263 }, { 666, 283 }};
static const XStringRes _Const0093 = { _StringsDefault0, 0x0190 };
static const XRect _Const0094 = {{ 666, 263 }, { 736, 283 }};
static const XStringRes _Const0095 = { _StringsDefault0, 0x0197 };
static const XRect _Const0096 = {{ 30, 0 }, { 170, 50 }};
static const XStringRes _Const0097 = { _StringsDefault0, 0x019E };
static const XStringRes _Const0098 = { _StringsDefault0, 0x01A7 };
static const XStringRes _Const0099 = { _StringsDefault0, 0x01AD };
static const XStringRes _Const009A = { _StringsDefault0, 0x01B3 };
static const XStringRes _Const009B = { _StringsDefault0, 0x01B9 };
static const XStringRes _Const009C = { _StringsDefault0, 0x01BF };
static const XStringRes _Const009D = { _StringsDefault0, 0x01C5 };
static const XStringRes _Const009E = { _StringsDefault0, 0x01CB };
static const XStringRes _Const009F = { _StringsDefault0, 0x01D1 };
static const XStringRes _Const00A0 = { _StringsDefault0, 0x01D7 };
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

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterWindow );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Background ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CaptionRect ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CaptionText ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 Mode, Mode, "Cutter::Window" )
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
  CutterWindow_ChangeMode,
  CutterWindow_FadeAnimation,
EW_END_OF_CLASS( CutterWindow )

/* Constant to define the spacing between the windows. */
const XPoint CutterSpacing = { 4, 4 };

/* Constant to define the border around the screen. */
const XPoint CutterBorder = { 8, 8 };

/* Include a file containing the font resource : 'Cutter::CaptionFont' */
#include "_CutterCaptionFont.h"

/* Table with links to derived variants of the font resource : 'Cutter::CaptionFont' */
EW_RES_WITHOUT_VARIANTS( CutterCaptionFont )

/* Include a file containing the font resource : 'Cutter::SmallDigitFont' */
#include "_CutterSmallDigitFont.h"

/* Table with links to derived variants of the font resource : 'Cutter::SmallDigitFont' */
EW_RES_WITHOUT_VARIANTS( CutterSmallDigitFont )

/* Include a file containing the font resource : 'Cutter::DescrFont' */
#include "_CutterDescrFont.h"

/* Table with links to derived variants of the font resource : 'Cutter::DescrFont' */
EW_RES_WITHOUT_VARIANTS( CutterDescrFont )

/* Initializer for the class 'Cutter::FeedWindow' */
void CutterFeedWindow__Init( CutterFeedWindow _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CutterWindow__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterFeedWindow );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PaperFeed ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CutterWindow );

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
      CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->dialog ), 0 );
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
                 _None, _None, "Cutter::FeedWindow" )
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
  CutterWindow_ChangeMode,
  CutterFeedWindow_FadeAnimation,
EW_END_OF_CLASS( CutterFeedWindow )

/* Color constant according UI design. */
const XColor CutterTextColor = { 0x3B, 0x5E, 0x54, 0xFF };

/* Include a file containing the bitmap resource : 'Cutter::PaperStripe' */
#include "_CutterPaperStripe.h"

/* Table with links to derived variants of the bitmap resource : 'Cutter::PaperStripe' */
EW_RES_WITHOUT_VARIANTS( CutterPaperStripe )

/* Initializer for the class 'Cutter::FormatWindow' */
void CutterFormatWindow__Init( CutterFormatWindow _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CutterWindow__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterFormatWindow );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Icon ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextW ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextH ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextWidth ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextHeight ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CutterWindow );

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
      CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->dialog ), 0 );
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
                 Icon, _None, _None, "Cutter::FormatWindow" )
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
  CutterWindow_ChangeMode,
  CutterFormatWindow_FadeAnimation,
EW_END_OF_CLASS( CutterFormatWindow )

/* Include a file containing the bitmap resource : 'Cutter::PaperFormat' */
#include "_CutterPaperFormat.h"

/* Table with links to derived variants of the bitmap resource : 'Cutter::PaperFormat' */
EW_RES_WITHOUT_VARIANTS( CutterPaperFormat )

/* Include a file containing the bitmap resource : 'Cutter::PaperBatch' */
#include "_CutterPaperBatch.h"

/* Table with links to derived variants of the bitmap resource : 'Cutter::PaperBatch' */
EW_RES_WITHOUT_VARIANTS( CutterPaperBatch )

/* Include a file containing the bitmap resource : 'Cutter::PaperSpeed' */
#include "_CutterPaperSpeed.h"

/* Table with links to derived variants of the bitmap resource : 'Cutter::PaperSpeed' */
EW_RES_WITHOUT_VARIANTS( CutterPaperSpeed )

/* Initializer for the class 'Cutter::Cutter' */
void CutterCutter__Init( CutterCutter _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterCutter );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Background ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->FeedWindow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->FormatWindow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BatchWindow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RemainingWindow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedWindow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MainButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->StatusBar ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PaperCut ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 Background, AutoState, AutoState, "Cutter::Cutter" )
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
EW_END_OF_CLASS( CutterCutter )

/* Constant to define the size of the entire paper cutter application. */
const XPoint CutterScreenSize = { 480, 272 };

/* Initializer for the class 'Cutter::DeviceClass' */
void CutterDeviceClass__Init( CutterDeviceClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterDeviceClass );

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
  _this->_Super._VMT = EW_CLASS( XObject );

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
                 PaperBatch, "Cutter::DeviceClass" )
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

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterBatchWindow );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Icon ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextBatchLarge ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextBatchMedium ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CutterWindow );

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
      CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->dialog ), 0 );
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
                 Icon, _None, _None, "Cutter::BatchWindow" )
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
  CutterWindow_ChangeMode,
  CutterBatchWindow_FadeAnimation,
EW_END_OF_CLASS( CutterBatchWindow )

/* Include a file containing the font resource : 'Cutter::LargeDigitFont' */
#include "_CutterLargeDigitFont.h"

/* Table with links to derived variants of the font resource : 'Cutter::LargeDigitFont' */
EW_RES_WITHOUT_VARIANTS( CutterLargeDigitFont )

/* Include a file containing the font resource : 'Cutter::MediumDigitFont' */
#include "_CutterMediumDigitFont.h"

/* Table with links to derived variants of the font resource : 'Cutter::MediumDigitFont' */
EW_RES_WITHOUT_VARIANTS( CutterMediumDigitFont )

/* Initializer for the class 'Cutter::RemainingWindow' */
void CutterRemainingWindow__Init( CutterRemainingWindow _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CutterWindow__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterRemainingWindow );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextRemaining ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CutterWindow );

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
                 PaperRemaining, Text, _None, _None, "Cutter::RemainingWindow" )
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
  CutterRemainingWindow_ChangeMode,
  CutterWindow_FadeAnimation,
EW_END_OF_CLASS( CutterRemainingWindow )

/* Initializer for the class 'Cutter::SpeedWindow' */
void CutterSpeedWindow__Init( CutterSpeedWindow _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CutterWindow__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterSpeedWindow );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Icon ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextSpeed ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedButtonL ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedButtonR ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CutterWindow );

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
      CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->dialog ), 0 );
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
                 Text, _None, _None, "Cutter::SpeedWindow" )
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
  CutterWindow_ChangeMode,
  CutterSpeedWindow_FadeAnimation,
EW_END_OF_CLASS( CutterSpeedWindow )

/* Initializer for the class 'Cutter::MainButton' */
void CutterMainButton__Init( CutterMainButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterMainButton );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandler ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 Mode, "Cutter::MainButton" )
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
EW_END_OF_CLASS( CutterMainButton )

/* Include a file containing the bitmap resource : 'Cutter::ButtonBig' */
#include "_CutterButtonBig.h"

/* Table with links to derived variants of the bitmap resource : 'Cutter::ButtonBig' */
EW_RES_WITHOUT_VARIANTS( CutterButtonBig )

/* Include a file containing the font resource : 'Cutter::MainButtonFont' */
#include "_CutterMainButtonFont.h"

/* Table with links to derived variants of the font resource : 'Cutter::MainButtonFont' */
EW_RES_WITHOUT_VARIANTS( CutterMainButtonFont )

/* Initializer for the class 'Cutter::StatusBar' */
void CutterStatusBar__Init( CutterStatusBar _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterStatusBar );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TimeText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Icon1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Icon2 ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 _None, _None, "Cutter::StatusBar" )
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
EW_END_OF_CLASS( CutterStatusBar )

/* Include a file containing the bitmap resource : 'Cutter::StatusIcons' */
#include "_CutterStatusIcons.h"

/* Table with links to derived variants of the bitmap resource : 'Cutter::StatusIcons' */
EW_RES_WITHOUT_VARIANTS( CutterStatusIcons )

/* Include a file containing the bitmap resource : 'Cutter::Arrows' */
#include "_CutterArrows.h"

/* Table with links to derived variants of the bitmap resource : 'Cutter::Arrows' */
EW_RES_WITHOUT_VARIANTS( CutterArrows )

/* Initializer for the class 'Cutter::SpeedButton' */
void CutterSpeedButton__Init( CutterSpeedButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterSpeedButton );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Background ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 TouchHandler, IconIndex, IconIndex, "Cutter::SpeedButton" )
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
EW_END_OF_CLASS( CutterSpeedButton )

/* Include a file containing the bitmap resource : 'Cutter::Paper' */
#include "_CutterPaper.h"

/* Table with links to derived variants of the bitmap resource : 'Cutter::Paper' */
EW_RES_WITHOUT_VARIANTS( CutterPaper )

/* Initializer for the class 'Cutter::PaperCut' */
void CutterPaperCut__Init( CutterPaperCut _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterPaperCut );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BorderBottom ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BorderTop ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BorderLeft ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BorderRight ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->VertSlider ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->HorzSlider ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 Image, _None, _None, "Cutter::PaperCut" )
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
EW_END_OF_CLASS( CutterPaperCut )

/* Initializer for the class 'Cutter::VertSlider' */
void CutterVertSlider__Init( CutterVertSlider _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterVertSlider );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Thumb ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 startValue, "Cutter::VertSlider" )
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
EW_END_OF_CLASS( CutterVertSlider )

/* Initializer for the class 'Cutter::HorzSlider' */
void CutterHorzSlider__Init( CutterHorzSlider _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterHorzSlider );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Thumb ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 CurrentValue, "Cutter::HorzSlider" )
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
EW_END_OF_CLASS( CutterHorzSlider )

/* Include a file containing the bitmap resource : 'Cutter::Thumb' */
#include "_CutterThumb.h"

/* Table with links to derived variants of the bitmap resource : 'Cutter::Thumb' */
EW_RES_WITHOUT_VARIANTS( CutterThumb )

/* Initializer for the class 'Cutter::FormatDialog' */
void CutterFormatDialog__Init( CutterFormatDialog _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterFormatDialog );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ButtonA3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextW ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextH ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextWidth ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextHeight ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CancelButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextW0 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ButtonA4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ButtonA5 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ButtonA6 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ButtonA7 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ButtonA8 ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 ButtonA3, _None, _None, "Cutter::FormatDialog" )
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
EW_END_OF_CLASS( CutterFormatDialog )

/* Initializer for the class 'Cutter::Button' */
void CutterButton__Init( CutterButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterButton );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Border ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );
  ViewsRectangle__Done( &_this->Rectangle );
  ViewsText__Done( &_this->Text );
  ViewsBorder__Done( &_this->Border );

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
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Cutter::Button' */
EW_DEFINE_CLASS_VARIANTS( CutterButton )
EW_END_OF_CLASS_VARIANTS( CutterButton )

/* Virtual Method Table (VMT) for the class : 'Cutter::Button' */
EW_DEFINE_CLASS( CutterButton, CoreGroup, OnChange, OnChange, TouchHandler, TouchHandler, 
                 Caption, Active, "Cutter::Button" )
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
EW_END_OF_CLASS( CutterButton )

/* Color constant according UI design. */
const XColor CutterButtonBgColor = { 0x3B, 0x5E, 0x54, 0xFF };

/* Color constant according UI design. */
const XColor CutterButtonTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

/* Include a file containing the font resource : 'Cutter::ButtonFont' */
#include "_CutterButtonFont.h"

/* Table with links to derived variants of the font resource : 'Cutter::ButtonFont' */
EW_RES_WITHOUT_VARIANTS( CutterButtonFont )

/* Include a file containing the bitmap resource : 'Cutter::BarRaster' */
#include "_CutterBarRaster.h"

/* Table with links to derived variants of the bitmap resource : 'Cutter::BarRaster' */
EW_RES_WITHOUT_VARIANTS( CutterBarRaster )

/* Initializer for the class 'Cutter::BarGraph' */
void CutterBarGraph__Init( CutterBarGraph _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterBarGraph );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BarLeft ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BarRight ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 CurrentValue, CurrentValue, "Cutter::BarGraph" )
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
EW_END_OF_CLASS( CutterBarGraph )

/* Initializer for the class 'Cutter::SpeedDialog' */
void CutterSpeedDialog__Init( CutterSpeedDialog _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterSpeedDialog );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextSpeed ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BarGraph ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedButtonL ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedButtonR ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CancelButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text0 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text5 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text6 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text7 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text8 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text9 ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 _None, _None, "Cutter::SpeedDialog" )
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
EW_END_OF_CLASS( CutterSpeedDialog )

/* Initializer for the class 'Cutter::CancelButton' */
void CutterCancelButton__Init( CutterCancelButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterCancelButton );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 TouchHandler, _None, _None, "Cutter::CancelButton" )
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
EW_END_OF_CLASS( CutterCancelButton )

/* Initializer for the class 'Cutter::BatchDialog' */
void CutterBatchDialog__Init( CutterBatchDialog _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterBatchDialog );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextBatch ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BarGraph ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedButtonL ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedButtonR ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CancelButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text0 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text5 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text6 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text7 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text8 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text9 ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 _None, _None, "Cutter::BatchDialog" )
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
EW_END_OF_CLASS( CutterBatchDialog )

/* Initializer for the class 'Cutter::PaperFeed' */
void CutterPaperFeed__Init( CutterPaperFeed _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterPaperFeed );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->StripeRight ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->StripeLeft ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 StripeRight, Type, Type, "Cutter::PaperFeed" )
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
EW_END_OF_CLASS( CutterPaperFeed )

/* Initializer for the class 'Cutter::FeedDialog' */
void CutterFeedDialog__Init( CutterFeedDialog _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterFeedDialog );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CancelButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->FeedButton0 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->FeedButton1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->FeedButton2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->FeedButton3 ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 _None, _None, "Cutter::FeedDialog" )
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
EW_END_OF_CLASS( CutterFeedDialog )

/* Initializer for the class 'Cutter::FeedButton' */
void CutterFeedButton__Init( CutterFeedButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( CutterFeedButton );

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
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PaperFeed ), 0 );
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
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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
                 TouchHandler, Type, Type, "Cutter::FeedButton" )
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
EW_END_OF_CLASS( CutterFeedButton )

/* Embedded Wizard */
