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

#ifndef Core_H
#define Core_H

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

/* Global constant containing the preferred size of the screen in pixel. */
extern const XPoint EwScreenSize;

/* Global constant containing the main application class. */
extern const XClass EwApplicationClass;

/* Global constant containing the user defined application title. */
extern const char* EwApplicationTitle;

/* The definition Core::ViewState determines the set of possible states, a Core::View 
   can assume at its lifetime. These states are used internally by all GUI components. 
   They determine whether components are visible, able to react to user events, 
   etc. */
typedef enum
{
  CoreViewStateVisible                  = 0x00000001,
  CoreViewStateAlphaBlended             = 0x00000002,
  CoreViewStateFocusable                = 0x00000004,
  CoreViewStateTouchable                = 0x00000008,
  CoreViewStateEnabled                  = 0x00000010,
  CoreViewStateSelected                 = 0x00000020,
  CoreViewStateFocused                  = 0x00000040,
  CoreViewStateModal                    = 0x00000080,
  CoreViewStateFastReshape              = 0x00000100,
  CoreViewStateIsOutline                = 0x00000200,
  CoreViewStateEmbedded                 = 0x00000400,
  CoreViewStateRequestLayout            = 0x00000800,
  CoreViewStateUpdateLayout             = 0x00001000,
  CoreViewStateUpdatingLayout           = 0x00002000,
  CoreViewStatePendingLayout            = 0x00004000,
  CoreViewStatePendingViewState         = 0x00008000,
  CoreViewStateDialog                   = 0x00010000,
  CoreViewStatePendingFader             = 0x00020000,
  CoreViewStateRunningFader             = 0x00040000
} CoreViewState;

/* The definition Core::Layout determines the set of available arrangement constraints 
   to apply on views during the automatic GUI arrangement. Each view can determine 
   its own set of constraints how it want to behave when e.g. its owner changes 
   the size.
   The constraints @ResizeVert and @ResizeHorz e.g. determine the resize behavior 
   of the view in response to the size modification of its owner. Views with these 
   constraints disabled always will keep their size unchanged. In conflict cases 
   when the alignment and resize constraints do contradict, the view will maintain 
   its size and will be pulled into the middle area resulting by the alignment constraints.
   In case of views automatically arranged in rows or columns within a Core::Outline, 
   the alignment constraints determine the vertical or horizontal alignment of the 
   view within the corresponding row or column. The resize constraints in this case 
   enable the view to fill the entire height of a column or the entire width of 
   a row. */
typedef enum
{
  CoreLayoutResizeHorz                  = 0x00000001,
  CoreLayoutResizeVert                  = 0x00000002,
  CoreLayoutAlignToLeft                 = 0x00000004,
  CoreLayoutAlignToRight                = 0x00000008,
  CoreLayoutAlignToTop                  = 0x00000010,
  CoreLayoutAlignToBottom               = 0x00000020
} CoreLayout;

/* The definition Core::Formation determines the available arrangement modes to 
   apply on all views embedded within a Core::Outline view. Depending on the mode, 
   the embedded views can be arranged in rows or columns. If the mode == Core::Formation.None, 
   no automatic row/column arrangement is performed and the views are simply aligned 
   inside the outline boundary area. */
typedef enum
{
  CoreFormationNone                     = 0,
  CoreFormationTopToBottom              = 1,
  CoreFormationBottomToTop              = 2,
  CoreFormationLeftToRight              = 3,
  CoreFormationRightToLeft              = 4
} CoreFormation;

/* The enumeration Core::KeyCode provides a set of predefined keyboard codes very 
   common to mobile and remote control devices, e.g. Menu, Left, Up, Ok, Exit, Play, 
   Record, etc. The usage of these predefined key codes increases the platform independence 
   of your GUI application. For special key codes, which are not included in this 
   set, a range of UserXX key codes is provided.
   Beside the key codes the enumeration also provides some few key categories like 
   AlphaKeys, DigitKeys or CursorKeys. They stand for an entire range of key codes 
   and are used when key codes are evaluated or filtered by the Core::KeyPressHandler. */
typedef enum
{
  CoreKeyCodeNoKey                      = 0,
  CoreKeyCodeOk                         = 1,
  CoreKeyCodeExit                       = 2,
  CoreKeyCodeMenu                       = 3,
  CoreKeyCodeUp                         = 4,
  CoreKeyCodeDown                       = 5,
  CoreKeyCodeLeft                       = 6,
  CoreKeyCodeRight                      = 7,
  CoreKeyCodePageUp                     = 8,
  CoreKeyCodePageDown                   = 9,
  CoreKeyCodeKey0                       = 10,
  CoreKeyCodeKey1                       = 11,
  CoreKeyCodeKey2                       = 12,
  CoreKeyCodeKey3                       = 13,
  CoreKeyCodeKey4                       = 14,
  CoreKeyCodeKey5                       = 15,
  CoreKeyCodeKey6                       = 16,
  CoreKeyCodeKey7                       = 17,
  CoreKeyCodeKey8                       = 18,
  CoreKeyCodeKey9                       = 19,
  CoreKeyCodeRed                        = 20,
  CoreKeyCodeGreen                      = 21,
  CoreKeyCodeBlue                       = 22,
  CoreKeyCodeYellow                     = 23,
  CoreKeyCodeWhite                      = 24,
  CoreKeyCodeMagenta                    = 25,
  CoreKeyCodeF1                         = 26,
  CoreKeyCodeF2                         = 27,
  CoreKeyCodeF3                         = 28,
  CoreKeyCodeF4                         = 29,
  CoreKeyCodeF5                         = 30,
  CoreKeyCodeF6                         = 31,
  CoreKeyCodeF7                         = 32,
  CoreKeyCodeF8                         = 33,
  CoreKeyCodeF9                         = 34,
  CoreKeyCodeF10                        = 35,
  CoreKeyCodeChannelUp                  = 36,
  CoreKeyCodeChannelDown                = 37,
  CoreKeyCodeDisplay                    = 38,
  CoreKeyCodeSkipPrev                   = 39,
  CoreKeyCodeSkipNext                   = 40,
  CoreKeyCodeHome                       = 41,
  CoreKeyCodeEnd                        = 42,
  CoreKeyCodeInsert                     = 43,
  CoreKeyCodeDelete                     = 44,
  CoreKeyCodeClear                      = 45,
  CoreKeyCodeVolumeUp                   = 46,
  CoreKeyCodeVolumeDown                 = 47,
  CoreKeyCodeShow                       = 48,
  CoreKeyCodeHide                       = 49,
  CoreKeyCodePlay                       = 50,
  CoreKeyCodePause                      = 51,
  CoreKeyCodeRecord                     = 52,
  CoreKeyCodeStop                       = 53,
  CoreKeyCodeRev                        = 54,
  CoreKeyCodeFwd                        = 55,
  CoreKeyCodeSlowRev                    = 56,
  CoreKeyCodeSlowFwd                    = 57,
  CoreKeyCodeSkipBwd                    = 58,
  CoreKeyCodeSkipFwd                    = 59,
  CoreKeyCodeRepeat                     = 60,
  CoreKeyCodeEject                      = 61,
  CoreKeyCodeHelp                       = 62,
  CoreKeyCodeTV                         = 63,
  CoreKeyCodeDVD                        = 64,
  CoreKeyCodeVCR                        = 65,
  CoreKeyCodeEPG                        = 66,
  CoreKeyCodeOSD                        = 67,
  CoreKeyCodeText                       = 68,
  CoreKeyCodePIP                        = 69,
  CoreKeyCodeAudio                      = 70,
  CoreKeyCodeClock                      = 71,
  CoreKeyCodeTimer                      = 72,
  CoreKeyCodeNavigation                 = 73,
  CoreKeyCodeKaraoke                    = 74,
  CoreKeyCodeGame                       = 75,
  CoreKeyCodeSubtitle                   = 76,
  CoreKeyCodeZoom                       = 77,
  CoreKeyCodeIndex                      = 78,
  CoreKeyCodeInfo                       = 79,
  CoreKeyCodePower                      = 80,
  CoreKeyCodeSetup                      = 81,
  CoreKeyCodeAngle                      = 82,
  CoreKeyCodeMode                       = 83,
  CoreKeyCodeMute                       = 84,
  CoreKeyCodeUser0                      = 85,
  CoreKeyCodeUser1                      = 86,
  CoreKeyCodeUser2                      = 87,
  CoreKeyCodeUser3                      = 88,
  CoreKeyCodeUser4                      = 89,
  CoreKeyCodeUser5                      = 90,
  CoreKeyCodeUser6                      = 91,
  CoreKeyCodeUser7                      = 92,
  CoreKeyCodeUser8                      = 93,
  CoreKeyCodeUser9                      = 94,
  CoreKeyCodeUser10                     = 95,
  CoreKeyCodeUser11                     = 96,
  CoreKeyCodeUser12                     = 97,
  CoreKeyCodeUser13                     = 98,
  CoreKeyCodeUser14                     = 99,
  CoreKeyCodeUser15                     = 100,
  CoreKeyCodeUser16                     = 101,
  CoreKeyCodeUser17                     = 102,
  CoreKeyCodeUser18                     = 103,
  CoreKeyCodeUser19                     = 104,
  CoreKeyCodeKeyA                       = 105,
  CoreKeyCodeKeyB                       = 106,
  CoreKeyCodeKeyC                       = 107,
  CoreKeyCodeKeyD                       = 108,
  CoreKeyCodeKeyE                       = 109,
  CoreKeyCodeKeyF                       = 110,
  CoreKeyCodeKeyG                       = 111,
  CoreKeyCodeKeyH                       = 112,
  CoreKeyCodeKeyI                       = 113,
  CoreKeyCodeKeyJ                       = 114,
  CoreKeyCodeKeyK                       = 115,
  CoreKeyCodeKeyL                       = 116,
  CoreKeyCodeKeyM                       = 117,
  CoreKeyCodeKeyN                       = 118,
  CoreKeyCodeKeyO                       = 119,
  CoreKeyCodeKeyP                       = 120,
  CoreKeyCodeKeyQ                       = 121,
  CoreKeyCodeKeyR                       = 122,
  CoreKeyCodeKeyS                       = 123,
  CoreKeyCodeKeyT                       = 124,
  CoreKeyCodeKeyU                       = 125,
  CoreKeyCodeKeyV                       = 126,
  CoreKeyCodeKeyW                       = 127,
  CoreKeyCodeKeyX                       = 128,
  CoreKeyCodeKeyY                       = 129,
  CoreKeyCodeKeyZ                       = 130,
  CoreKeyCodeSpace                      = 131,
  CoreKeyCodePlus                       = 132,
  CoreKeyCodeMinus                      = 133,
  CoreKeyCodeMultiply                   = 134,
  CoreKeyCodeDivide                     = 135,
  CoreKeyCodeEquals                     = 136,
  CoreKeyCodePeriod                     = 137,
  CoreKeyCodeComma                      = 138,
  CoreKeyCodeColon                      = 139,
  CoreKeyCodeSemicolon                  = 140,
  CoreKeyCodeAlphaKeys                  = 141,
  CoreKeyCodeAlphaOrDigitKeys           = 142,
  CoreKeyCodeDigitKeys                  = 143,
  CoreKeyCodeHexDigitKeys               = 144,
  CoreKeyCodeCharacterKeys              = 145,
  CoreKeyCodeControlKeys                = 146,
  CoreKeyCodeCursorKeys                 = 147,
  CoreKeyCodeAnyKey                     = 148,
  CoreKeyCodeEnter                      = 149,
  CoreKeyCodeEscape                     = 150,
  CoreKeyCodeBackspace                  = 151,
  CoreKeyCodeTab                        = 152,
  CoreKeyCodeCtrlKeyA                   = 153,
  CoreKeyCodeCtrlKeyB                   = 154,
  CoreKeyCodeCtrlKeyC                   = 155,
  CoreKeyCodeCtrlKeyD                   = 156,
  CoreKeyCodeCtrlKeyE                   = 157,
  CoreKeyCodeCtrlKeyF                   = 158,
  CoreKeyCodeCtrlKeyG                   = 159,
  CoreKeyCodeCtrlKeyH                   = 160,
  CoreKeyCodeCtrlKeyI                   = 161,
  CoreKeyCodeCtrlKeyJ                   = 162,
  CoreKeyCodeCtrlKeyK                   = 163,
  CoreKeyCodeCtrlKeyL                   = 164,
  CoreKeyCodeCtrlKeyM                   = 165,
  CoreKeyCodeCtrlKeyN                   = 166,
  CoreKeyCodeCtrlKeyO                   = 167,
  CoreKeyCodeCtrlKeyP                   = 168,
  CoreKeyCodeCtrlKeyQ                   = 169,
  CoreKeyCodeCtrlKeyR                   = 170,
  CoreKeyCodeCtrlKeyS                   = 171,
  CoreKeyCodeCtrlKeyT                   = 172,
  CoreKeyCodeCtrlKeyU                   = 173,
  CoreKeyCodeCtrlKeyV                   = 174,
  CoreKeyCodeCtrlKeyW                   = 175,
  CoreKeyCodeCtrlKeyX                   = 176,
  CoreKeyCodeCtrlKeyY                   = 177,
  CoreKeyCodeCtrlKeyZ                   = 178,
  CoreKeyCodeCtrlSpace                  = 179,
  CoreKeyCodeCtrlKey0                   = 180,
  CoreKeyCodeCtrlKey1                   = 181,
  CoreKeyCodeCtrlKey2                   = 182,
  CoreKeyCodeCtrlKey3                   = 183,
  CoreKeyCodeCtrlKey4                   = 184,
  CoreKeyCodeCtrlKey5                   = 185,
  CoreKeyCodeCtrlKey6                   = 186,
  CoreKeyCodeCtrlKey7                   = 187,
  CoreKeyCodeCtrlKey8                   = 188,
  CoreKeyCodeCtrlKey9                   = 189,
  CoreKeyCodeCtrlF1                     = 190,
  CoreKeyCodeCtrlF2                     = 191,
  CoreKeyCodeCtrlF3                     = 192,
  CoreKeyCodeCtrlF4                     = 193,
  CoreKeyCodeCtrlF5                     = 194,
  CoreKeyCodeCtrlF6                     = 195,
  CoreKeyCodeCtrlF7                     = 196,
  CoreKeyCodeCtrlF8                     = 197,
  CoreKeyCodeCtrlF9                     = 198,
  CoreKeyCodeCtrlF10                    = 199,
  CoreKeyCodeCtrlEnter                  = 200,
  CoreKeyCodeCtrlEscape                 = 201,
  CoreKeyCodeCtrlUp                     = 202,
  CoreKeyCodeCtrlDown                   = 203,
  CoreKeyCodeCtrlLeft                   = 204,
  CoreKeyCodeCtrlRight                  = 205,
  CoreKeyCodeCtrlPageUp                 = 206,
  CoreKeyCodeCtrlPageDown               = 207,
  CoreKeyCodeCtrlBackspace              = 208,
  CoreKeyCodeCtrlInsert                 = 209,
  CoreKeyCodeCtrlDelete                 = 210,
  CoreKeyCodeCtrlHome                   = 211,
  CoreKeyCodeCtrlEnd                    = 212,
  CoreKeyCodeCtrlTab                    = 213,
  CoreKeyCodeCtrlShiftKeyA              = 214,
  CoreKeyCodeCtrlShiftKeyB              = 215,
  CoreKeyCodeCtrlShiftKeyC              = 216,
  CoreKeyCodeCtrlShiftKeyD              = 217,
  CoreKeyCodeCtrlShiftKeyE              = 218,
  CoreKeyCodeCtrlShiftKeyF              = 219,
  CoreKeyCodeCtrlShiftKeyG              = 220,
  CoreKeyCodeCtrlShiftKeyH              = 221,
  CoreKeyCodeCtrlShiftKeyI              = 222,
  CoreKeyCodeCtrlShiftKeyJ              = 223,
  CoreKeyCodeCtrlShiftKeyK              = 224,
  CoreKeyCodeCtrlShiftKeyL              = 225,
  CoreKeyCodeCtrlShiftKeyM              = 226,
  CoreKeyCodeCtrlShiftKeyN              = 227,
  CoreKeyCodeCtrlShiftKeyO              = 228,
  CoreKeyCodeCtrlShiftKeyP              = 229,
  CoreKeyCodeCtrlShiftKeyQ              = 230,
  CoreKeyCodeCtrlShiftKeyR              = 231,
  CoreKeyCodeCtrlShiftKeyS              = 232,
  CoreKeyCodeCtrlShiftKeyT              = 233,
  CoreKeyCodeCtrlShiftKeyU              = 234,
  CoreKeyCodeCtrlShiftKeyV              = 235,
  CoreKeyCodeCtrlShiftKeyW              = 236,
  CoreKeyCodeCtrlShiftKeyX              = 237,
  CoreKeyCodeCtrlShiftKeyY              = 238,
  CoreKeyCodeCtrlShiftKeyZ              = 239,
  CoreKeyCodeCtrlShiftSpace             = 240,
  CoreKeyCodeCtrlShiftKey0              = 241,
  CoreKeyCodeCtrlShiftKey1              = 242,
  CoreKeyCodeCtrlShiftKey2              = 243,
  CoreKeyCodeCtrlShiftKey3              = 244,
  CoreKeyCodeCtrlShiftKey4              = 245,
  CoreKeyCodeCtrlShiftKey5              = 246,
  CoreKeyCodeCtrlShiftKey6              = 247,
  CoreKeyCodeCtrlShiftKey7              = 248,
  CoreKeyCodeCtrlShiftKey8              = 249,
  CoreKeyCodeCtrlShiftKey9              = 250,
  CoreKeyCodeCtrlShiftF1                = 251,
  CoreKeyCodeCtrlShiftF2                = 252,
  CoreKeyCodeCtrlShiftF3                = 253,
  CoreKeyCodeCtrlShiftF4                = 254,
  CoreKeyCodeCtrlShiftF5                = 255,
  CoreKeyCodeCtrlShiftF6                = 256,
  CoreKeyCodeCtrlShiftF7                = 257,
  CoreKeyCodeCtrlShiftF8                = 258,
  CoreKeyCodeCtrlShiftF9                = 259,
  CoreKeyCodeCtrlShiftF10               = 260,
  CoreKeyCodeCtrlShiftEnter             = 261,
  CoreKeyCodeCtrlShiftEscape            = 262,
  CoreKeyCodeCtrlShiftUp                = 263,
  CoreKeyCodeCtrlShiftDown              = 264,
  CoreKeyCodeCtrlShiftLeft              = 265,
  CoreKeyCodeCtrlShiftRight             = 266,
  CoreKeyCodeCtrlShiftPageUp            = 267,
  CoreKeyCodeCtrlShiftPageDown          = 268,
  CoreKeyCodeCtrlShiftBackspace         = 269,
  CoreKeyCodeCtrlShiftInsert            = 270,
  CoreKeyCodeCtrlShiftDelete            = 271,
  CoreKeyCodeCtrlShiftHome              = 272,
  CoreKeyCodeCtrlShiftEnd               = 273,
  CoreKeyCodeCtrlShiftTab               = 274,
  CoreKeyCodeAltF1                      = 275,
  CoreKeyCodeAltF2                      = 276,
  CoreKeyCodeAltF3                      = 277,
  CoreKeyCodeAltF4                      = 278,
  CoreKeyCodeAltF5                      = 279,
  CoreKeyCodeAltF6                      = 280,
  CoreKeyCodeAltF7                      = 281,
  CoreKeyCodeAltF8                      = 282,
  CoreKeyCodeAltF9                      = 283,
  CoreKeyCodeAltF10                     = 284,
  CoreKeyCodeAltEnter                   = 285,
  CoreKeyCodeAltEscape                  = 286,
  CoreKeyCodeAltUp                      = 287,
  CoreKeyCodeAltDown                    = 288,
  CoreKeyCodeAltLeft                    = 289,
  CoreKeyCodeAltRight                   = 290,
  CoreKeyCodeAltPageUp                  = 291,
  CoreKeyCodeAltPageDown                = 292,
  CoreKeyCodeAltBackspace               = 293,
  CoreKeyCodeAltInsert                  = 294,
  CoreKeyCodeAltDelete                  = 295,
  CoreKeyCodeAltHome                    = 296,
  CoreKeyCodeAltEnd                     = 297,
  CoreKeyCodeAltTab                     = 298,
  CoreKeyCodeAltShiftF1                 = 299,
  CoreKeyCodeAltShiftF2                 = 300,
  CoreKeyCodeAltShiftF3                 = 301,
  CoreKeyCodeAltShiftF4                 = 302,
  CoreKeyCodeAltShiftF5                 = 303,
  CoreKeyCodeAltShiftF6                 = 304,
  CoreKeyCodeAltShiftF7                 = 305,
  CoreKeyCodeAltShiftF8                 = 306,
  CoreKeyCodeAltShiftF9                 = 307,
  CoreKeyCodeAltShiftF10                = 308,
  CoreKeyCodeAltShiftEnter              = 309,
  CoreKeyCodeAltShiftEscape             = 310,
  CoreKeyCodeAltShiftUp                 = 311,
  CoreKeyCodeAltShiftDown               = 312,
  CoreKeyCodeAltShiftLeft               = 313,
  CoreKeyCodeAltShiftRight              = 314,
  CoreKeyCodeAltShiftPageUp             = 315,
  CoreKeyCodeAltShiftPageDown           = 316,
  CoreKeyCodeAltShiftBackspace          = 317,
  CoreKeyCodeAltShiftInsert             = 318,
  CoreKeyCodeAltShiftDelete             = 319,
  CoreKeyCodeAltShiftHome               = 320,
  CoreKeyCodeAltShiftEnd                = 321,
  CoreKeyCodeAltShiftTab                = 322,
  CoreKeyCodeShiftF1                    = 323,
  CoreKeyCodeShiftF2                    = 324,
  CoreKeyCodeShiftF3                    = 325,
  CoreKeyCodeShiftF4                    = 326,
  CoreKeyCodeShiftF5                    = 327,
  CoreKeyCodeShiftF6                    = 328,
  CoreKeyCodeShiftF7                    = 329,
  CoreKeyCodeShiftF8                    = 330,
  CoreKeyCodeShiftF9                    = 331,
  CoreKeyCodeShiftF10                   = 332,
  CoreKeyCodeShiftEnter                 = 333,
  CoreKeyCodeShiftEscape                = 334,
  CoreKeyCodeShiftUp                    = 335,
  CoreKeyCodeShiftDown                  = 336,
  CoreKeyCodeShiftLeft                  = 337,
  CoreKeyCodeShiftRight                 = 338,
  CoreKeyCodeShiftPageUp                = 339,
  CoreKeyCodeShiftPageDown              = 340,
  CoreKeyCodeShiftBackspace             = 341,
  CoreKeyCodeShiftInsert                = 342,
  CoreKeyCodeShiftDelete                = 343,
  CoreKeyCodeShiftHome                  = 344,
  CoreKeyCodeShiftEnd                   = 345,
  CoreKeyCodeShiftTab                   = 346
} CoreKeyCode;

/* The definition Core::RetargetReason determines the set of possible reasons for 
   a touch handler to resign and pass over the current event processing to another 
   handler. */
typedef enum
{
  CoreRetargetReasonWipeUp              = 0x00000001,
  CoreRetargetReasonWipeDown            = 0x00000002,
  CoreRetargetReasonWipeLeft            = 0x00000004,
  CoreRetargetReasonWipeRight           = 0x00000008,
  CoreRetargetReasonLongPress           = 0x00000010
} CoreRetargetReason;

#ifdef __cplusplus
  }
#endif

#endif /* Core_H */

/* Embedded Wizard */
