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
#include "_CorePropertyObserver.h"
#include "_CoreRoot.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreSlideTouchHandler.h"
#include "_CoreTimer.h"
#include "_CoreVerticalList.h"
#include "_CoreView.h"
#include "_EffectsInt32Effect.h"
#include "_EffectsRectEffect.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "_ViewsBorder.h"
#include "_ViewsFrame.h"
#include "_ViewsImage.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"
#include "_WasherDeviceClass.h"
#include "_WasherHourItemL.h"
#include "_WasherHourItemS.h"
#include "_WasherMinItemL.h"
#include "_WasherMinItemS.h"
#include "_WasherOptItemL.h"
#include "_WasherOptItemS.h"
#include "_WasherOptList.h"
#include "_WasherProgItemL.h"
#include "_WasherProgItemS.h"
#include "_WasherProgList.h"
#include "_WasherSpinItemL.h"
#include "_WasherSpinItemS.h"
#include "_WasherSpinList.h"
#include "_WasherStartButton.h"
#include "_WasherTempItemL.h"
#include "_WasherTempItemS.h"
#include "_WasherTempList.h"
#include "_WasherTimeList.h"
#include "_WasherWasher.h"
#include "Core.h"
#include "Effects.h"
#include "Graphics.h"
#include "Views.h"
#include "Washer.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x0000031A, /* ratio 54.91 % */
  0xB8002700, 0x80090452, 0x00E80037, 0xC8600178, 0x1E6C0041, 0x065001CA, 0x20001900,
  0x03080206, 0x34000E60, 0x18324200, 0xC6C8A458, 0x60F2008E, 0x6272786C, 0x89249145,
  0x20088450, 0x2731800D, 0x18F45615, 0x29C901B2, 0x4C48E000, 0x4CCEF288, 0x4243138E,
  0x21E57920, 0x0D1988C2, 0x11637472, 0x1E8E5180, 0x40348641, 0x844E24D2, 0x04C41121,
  0x4DA39130, 0xFC0F21AA, 0x00135A0C, 0x096A005A, 0x7179A57E, 0x0019C92B, 0x15800060,
  0x82570012, 0xEDEEF234, 0x0D48B7FB, 0x12ED7CC1, 0x309881B4, 0x0E001CC9, 0x924B8627,
  0x2FAA422A, 0x98258801, 0xC542A875, 0xD5EB0245, 0x60871A0B, 0xC60994D2, 0x2D382D38,
  0x5832D5C9, 0x400D56BB, 0x11006B74, 0x43C45648, 0x8F4AECB4, 0x6DBACF42, 0xDF4CF54B,
  0xBA852407, 0x8002D004, 0x3B27AB17, 0x54AE370B, 0x0ADE0037, 0x289C677D, 0xBEF4E3DC,
  0xE51A4814, 0xCD0D7C3E, 0x7A67DB8A, 0xE6F53A27, 0x480A90CC, 0xD0FB2142, 0x7D36EFB8,
  0x32A116E3, 0xCDDF7B13, 0x9F84CDFB, 0x47FC2E50, 0x010B1552, 0xF0709F80, 0x5B04F491,
  0x1974D90A, 0xB0ED214A, 0x4D134216, 0x2B0B12A4, 0x4D920429, 0x5174651E, 0x9421F845,
  0x09424711, 0x384D9115, 0x43E11471, 0xE2D46554, 0x95119670, 0x812855D1, 0x084A9194,
  0x8C9F41D5, 0xA1E42134, 0x6814007A, 0x90095F64, 0x6D710464, 0x01121675, 0x57D0E924,
  0xE8F96305, 0x4A138BDB, 0x3A6C5199, 0x829916D6, 0x0054864C, 0x4819A349, 0x05FC5010,
  0x00148001, 0x5F57D7F0, 0x00000101, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XStringRes _Const0000 = { _StringsDefault0, 0x0002 };
static const XStringRes _Const0001 = { _StringsDefault0, 0x0015 };
static const XStringRes _Const0002 = { _StringsDefault0, 0x0021 };
static const XStringRes _Const0003 = { _StringsDefault0, 0x002D };
static const XStringRes _Const0004 = { _StringsDefault0, 0x003A };
static const XStringRes _Const0005 = { _StringsDefault0, 0x004A };
static const XStringRes _Const0006 = { _StringsDefault0, 0x0050 };
static const XStringRes _Const0007 = { _StringsDefault0, 0x0058 };
static const XStringRes _Const0008 = { _StringsDefault0, 0x005F };
static const XStringRes _Const0009 = { _StringsDefault0, 0x0066 };
static const XStringRes _Const000A = { _StringsDefault0, 0x006C };
static const XStringRes _Const000B = { _StringsDefault0, 0x0072 };
static const XStringRes _Const000C = { _StringsDefault0, 0x0078 };
static const XStringRes _Const000D = { _StringsDefault0, 0x007E };
static const XStringRes _Const000E = { _StringsDefault0, 0x0084 };
static const XStringRes _Const000F = { _StringsDefault0, 0x008B };
static const XStringRes _Const0010 = { _StringsDefault0, 0x0093 };
static const XStringRes _Const0011 = { _StringsDefault0, 0x0099 };
static const XStringRes _Const0012 = { _StringsDefault0, 0x009F };
static const XStringRes _Const0013 = { _StringsDefault0, 0x00A6 };
static const XStringRes _Const0014 = { _StringsDefault0, 0x00AD };
static const XStringRes _Const0015 = { _StringsDefault0, 0x00B4 };
static const XStringRes _Const0016 = { _StringsDefault0, 0x00C5 };
static const XStringRes _Const0017 = { _StringsDefault0, 0x00D4 };
static const XStringRes _Const0018 = { _StringsDefault0, 0x00E4 };
static const XRect _Const0019 = {{ 0, 0 }, { 126, 240 }};
static const XPoint _Const001A = { 0, 96 };
static const XPoint _Const001B = { 0, 240 };
static const XPoint _Const001C = { 126, 240 };
static const XPoint _Const001D = { 126, 0 };
static const XPoint _Const001E = { 0, 0 };
static const XRect _Const001F = {{ 0, 72 }, { 126, 168 }};
static const XRect _Const0020 = {{ 0, 168 }, { 126, 240 }};
static const XRect _Const0021 = {{ 0, 0 }, { 126, 72 }};
static const XRect _Const0022 = {{ 10, 70 }, { 110, 100 }};
static const XStringRes _Const0023 = { _StringsDefault0, 0x00F8 };
static const XRect _Const0024 = {{ 0, 0 }, { 220, 160 }};
static const XRect _Const0025 = {{ 20, 0 }, { 220, 160 }};
static const XStringRes _Const0026 = { _StringsDefault0, 0x0102 };
static const XRect _Const0027 = {{ 0, 0 }, { 220, 40 }};
static const XRect _Const0028 = {{ 0, 0 }, { 2, 40 }};
static const XRect _Const0029 = {{ 218, 0 }, { 220, 40 }};
static const XRect _Const002A = {{ 10, 0 }, { 210, 40 }};
static const XRect _Const002B = {{ 0, 0 }, { 46, 240 }};
static const XRect _Const002C = {{ 0, 0 }, { 80, 400 }};
static const XColor _Const002D = { 0x00, 0x00, 0x00, 0x00 };
static const XPoint _Const002E = { 46, 240 };
static const XPoint _Const002F = { 46, 0 };
static const XRect _Const0030 = {{ 0, 72 }, { 46, 168 }};
static const XRect _Const0031 = {{ 0, 168 }, { 46, 240 }};
static const XRect _Const0032 = {{ 0, 0 }, { 46, 72 }};
static const XRect _Const0033 = {{ 5, 70 }, { 45, 100 }};
static const XStringRes _Const0034 = { _StringsDefault0, 0x0109 };
static const XRect _Const0035 = {{ 0, 0 }, { 80, 160 }};
static const XColor _Const0036 = { 0xFF, 0xFF, 0xFF, 0xFF };
static const XRect _Const0037 = {{ 0, 0 }, { 80, 40 }};
static const XRect _Const0038 = {{ 78, 0 }, { 80, 40 }};
static const XRect _Const0039 = {{ 10, 0 }, { 80, 40 }};
static const XStringRes _Const003A = { _StringsDefault0, 0x0111 };
static const XRect _Const003B = {{ 10, 0 }, { 80, 160 }};
static const XRect _Const003C = {{ 0, 0 }, { 92, 240 }};
static const XPoint _Const003D = { 92, 240 };
static const XPoint _Const003E = { 92, 0 };
static const XRect _Const003F = {{ 0, 72 }, { 92, 168 }};
static const XRect _Const0040 = {{ 0, 168 }, { 92, 240 }};
static const XRect _Const0041 = {{ 0, 0 }, { 92, 72 }};
static const XRect _Const0042 = {{ 5, 70 }, { 85, 100 }};
static const XStringRes _Const0043 = { _StringsDefault0, 0x0119 };
static const XRect _Const0044 = {{ 0, 0 }, { 160, 160 }};
static const XRect _Const0045 = {{ 10, 0 }, { 160, 160 }};
static const XRect _Const0046 = {{ 0, 0 }, { 160, 60 }};
static const XRect _Const0047 = {{ 158, 0 }, { 160, 60 }};
static const XRect _Const0048 = {{ 10, 0 }, { 150, 60 }};
static const XRect _Const0049 = {{ 0, 0 }, { 480, 272 }};
static const XColor _Const004A = { 0x22, 0x61, 0x73, 0xFF };
static const XColor _Const004B = { 0x00, 0x00, 0x00, 0xFF };
static const XRect _Const004C = {{ 12, 90 }, { 460, 200 }};
static const XColor _Const004D = { 0x00, 0x00, 0x00, 0x80 };
static const XRect _Const004E = {{ 12, 18 }, { 138, 258 }};
static const XRect _Const004F = {{ 138, 18 }, { 184, 258 }};
static const XRect _Const0050 = {{ 184, 18 }, { 230, 258 }};
static const XRect _Const0051 = {{ 230, 18 }, { 322, 258 }};
static const XRect _Const0052 = {{ 322, 18 }, { 448, 234 }};
static const XRect _Const0053 = {{ 12, 90 }, { 448, 188 }};
static const XColor _Const0054 = { 0xFF, 0xC2, 0x00, 0xFF };
static const XRect _Const0055 = {{ 137, 103 }, { 139, 178 }};
static const XColor _Const0056 = { 0x40, 0x40, 0x40, 0x40 };
static const XRect _Const0057 = {{ 183, 103 }, { 185, 178 }};
static const XRect _Const0058 = {{ 229, 103 }, { 231, 178 }};
static const XRect _Const0059 = {{ 321, 103 }, { 323, 178 }};
static const XRect _Const005A = {{ 322, 190 }, { 458, 234 }};
static const XRect _Const005B = {{ 322, 234 }, { 458, 278 }};
static const XRect _Const005C = {{ 10, 240 }, { 320, 270 }};
static const XStringRes _Const005D = { _StringsDefault0, 0x0123 };
static const XRect _Const005E = {{ 0, 0 }, { 110, 40 }};
static const XRect _Const005F = {{ 109, 0 }, { 110, 40 }};
static const XRect _Const0060 = {{ 0, 0 }, { 126, 216 }};
static const XRect _Const0061 = {{ 0, 0 }, { 63, 216 }};
static const XPoint _Const0062 = { 0, 216 };
static const XPoint _Const0063 = { 63, 216 };
static const XPoint _Const0064 = { 63, 0 };
static const XRect _Const0065 = {{ 0, 168 }, { 63, 216 }};
static const XRect _Const0066 = {{ 0, 72 }, { 63, 168 }};
static const XRect _Const0067 = {{ 0, 0 }, { 63, 72 }};
static const XRect _Const0068 = {{ 5, 70 }, { 55, 100 }};
static const XStringRes _Const0069 = { _StringsDefault0, 0x015E };
static const XRect _Const006A = {{ 63, 0 }, { 126, 216 }};
static const XPoint _Const006B = { 126, 216 };
static const XRect _Const006C = {{ 63, 168 }, { 126, 216 }};
static const XRect _Const006D = {{ 63, 72 }, { 126, 168 }};
static const XRect _Const006E = {{ 63, 0 }, { 126, 72 }};
static const XRect _Const006F = {{ 53, 72 }, { 73, 168 }};
static const XStringRes _Const0070 = { _StringsDefault0, 0x0167 };
static const XRect _Const0071 = {{ 20, 140 }, { 130, 170 }};
static const XStringRes _Const0072 = { _StringsDefault0, 0x016B };
static const XRect _Const0073 = {{ 0, 0 }, { 110, 160 }};
static const XRect _Const0074 = {{ 0, 0 }, { 100, 160 }};
static const XStringRes _Const0075 = { _StringsDefault0, 0x0180 };
static const XRect _Const0076 = {{ 10, 0 }, { 110, 160 }};
static const XRect _Const0077 = {{ 0, 0 }, { 1, 40 }};
static const XRect _Const0078 = {{ 108, 0 }, { 110, 40 }};
static const XRect _Const0079 = {{ 0, 0 }, { 136, 44 }};
static const XRect _Const007A = {{ 0, 0 }, { 138, 48 }};
static const XPoint _Const007B = { 0, 36 };
static const XPoint _Const007C = { 126, 36 };
static const XRect _Const007D = {{ 0, 0 }, { 126, 36 }};
static const XColor _Const007E = { 0xFF, 0xB6, 0x5F, 0xFF };
static const XStringRes _Const007F = { _StringsDefault0, 0x0185 };

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

/* Initializer for the class 'Washer::DeviceClass' */
void WasherDeviceClass__Init( WasherDeviceClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherDeviceClass );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherDeviceClass );

  /* ... and initialize objects, variables, properties, etc. */
  _this->WashingProgram[ 0 ] = EwShareString( EwLoadString( &_Const0000 ));
  _this->WashingProgram[ 1 ] = EwShareString( EwLoadString( &_Const0001 ));
  _this->WashingProgram[ 2 ] = EwShareString( EwLoadString( &_Const0002 ));
  _this->WashingProgram[ 3 ] = EwShareString( EwLoadString( &_Const0003 ));
  _this->WashingProgram[ 4 ] = EwShareString( EwLoadString( &_Const0004 ));
  _this->WashingProgram[ 5 ] = EwShareString( EwLoadString( &_Const0005 ));
  _this->WashingProgram[ 6 ] = EwShareString( EwLoadString( &_Const0006 ));
  _this->WashingProgram[ 7 ] = EwShareString( EwLoadString( &_Const0007 ));
  _this->WashingTemp[ 0 ] = EwShareString( EwLoadString( &_Const0008 ));
  _this->WashingTemp[ 1 ] = EwShareString( EwLoadString( &_Const0009 ));
  _this->WashingTemp[ 2 ] = EwShareString( EwLoadString( &_Const000A ));
  _this->WashingTemp[ 3 ] = EwShareString( EwLoadString( &_Const000B ));
  _this->WashingTemp[ 4 ] = EwShareString( EwLoadString( &_Const000C ));
  _this->WashingTemp[ 5 ] = EwShareString( EwLoadString( &_Const000D ));
  _this->SpinTurn[ 0 ] = EwShareString( EwLoadString( &_Const000E ));
  _this->SpinTurn[ 1 ] = EwShareString( EwLoadString( &_Const000F ));
  _this->SpinTurn[ 2 ] = EwShareString( EwLoadString( &_Const0010 ));
  _this->SpinTurn[ 3 ] = EwShareString( EwLoadString( &_Const0011 ));
  _this->SpinTurn[ 4 ] = EwShareString( EwLoadString( &_Const0012 ));
  _this->SpinTurn[ 5 ] = EwShareString( EwLoadString( &_Const0013 ));
  _this->SpinTurn[ 6 ] = EwShareString( EwLoadString( &_Const0014 ));
  _this->Options[ 0 ] = EwShareString( EwLoadString( &_Const000E ));
  _this->Options[ 1 ] = EwShareString( EwLoadString( &_Const000F ));
  _this->Options[ 2 ] = EwShareString( EwLoadString( &_Const0015 ));
  _this->Options[ 3 ] = EwShareString( EwLoadString( &_Const0016 ));
  _this->Options[ 4 ] = EwShareString( EwLoadString( &_Const0017 ));
  _this->Options[ 5 ] = EwShareString( EwLoadString( &_Const0018 ));
}

/* Re-Initializer for the class 'Washer::DeviceClass' */
void WasherDeviceClass__ReInit( WasherDeviceClass _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Washer::DeviceClass' */
void WasherDeviceClass__Done( WasherDeviceClass _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::DeviceClass.OnSetProgramNumber()' */
void WasherDeviceClass_OnSetProgramNumber( WasherDeviceClass _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value >= 8 )
    value = 7;

  if ( value == _this->ProgramNumber )
    return;

  _this->ProgramNumber = value;
  EwNotifyRefObservers( EwNewRef( _this, WasherDeviceClass_OnGetProgramNumber, WasherDeviceClass_OnSetProgramNumber ), 
    0 );
}

/* 'C' function for method : 'Washer::DeviceClass.OnSetTempNumber()' */
void WasherDeviceClass_OnSetTempNumber( WasherDeviceClass _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value >= 6 )
    value = 5;

  if ( value == _this->TempNumber )
    return;

  _this->TempNumber = value;
  EwNotifyRefObservers( EwNewRef( _this, WasherDeviceClass_OnGetTempNumber, WasherDeviceClass_OnSetTempNumber ), 
    0 );
}

/* 'C' function for method : 'Washer::DeviceClass.OnSetOptionNumber()' */
void WasherDeviceClass_OnSetOptionNumber( WasherDeviceClass _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value >= 6 )
    value = 5;

  if ( value == _this->OptionNumber )
    return;

  _this->OptionNumber = value;
  EwNotifyRefObservers( EwNewRef( _this, WasherDeviceClass_OnGetOptionNumber, WasherDeviceClass_OnSetOptionNumber ), 
    0 );
}

/* 'C' function for method : 'Washer::DeviceClass.OnSetHour()' */
void WasherDeviceClass_OnSetHour( WasherDeviceClass _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value > 23 )
    value = 23;

  if ( value == _this->Hour )
    return;

  _this->Hour = value;
  EwNotifyRefObservers( EwNewRef( _this, WasherDeviceClass_OnGetHour, WasherDeviceClass_OnSetHour ), 
    0 );
}

/* 'C' function for method : 'Washer::DeviceClass.OnSetMinute()' */
void WasherDeviceClass_OnSetMinute( WasherDeviceClass _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value > 59 )
    value = 59;

  if ( value == _this->Minute )
    return;

  _this->Minute = value;
  EwNotifyRefObservers( EwNewRef( _this, WasherDeviceClass_OnGetMinute, WasherDeviceClass_OnSetMinute ), 
    0 );
}

/* 'C' function for method : 'Washer::DeviceClass.OnSetSpinNumber()' */
void WasherDeviceClass_OnSetSpinNumber( WasherDeviceClass _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( value >= 7 )
    value = 6;

  if ( value == _this->SpinNumber )
    return;

  _this->SpinNumber = value;
  EwNotifyRefObservers( EwNewRef( _this, WasherDeviceClass_OnGetSpinNumber, WasherDeviceClass_OnSetSpinNumber ), 
    0 );
}

/* Default onget method for the property 'ProgramNumber' */
XInt32 WasherDeviceClass_OnGetProgramNumber( WasherDeviceClass _this )
{
  return _this->ProgramNumber;
}

/* Default onget method for the property 'TempNumber' */
XInt32 WasherDeviceClass_OnGetTempNumber( WasherDeviceClass _this )
{
  return _this->TempNumber;
}

/* Default onget method for the property 'OptionNumber' */
XInt32 WasherDeviceClass_OnGetOptionNumber( WasherDeviceClass _this )
{
  return _this->OptionNumber;
}

/* Default onget method for the property 'Hour' */
XInt32 WasherDeviceClass_OnGetHour( WasherDeviceClass _this )
{
  return _this->Hour;
}

/* Default onget method for the property 'Minute' */
XInt32 WasherDeviceClass_OnGetMinute( WasherDeviceClass _this )
{
  return _this->Minute;
}

/* Default onget method for the property 'SpinNumber' */
XInt32 WasherDeviceClass_OnGetSpinNumber( WasherDeviceClass _this )
{
  return _this->SpinNumber;
}

/* Variants derived from the class : 'Washer::DeviceClass' */
EW_DEFINE_CLASS_VARIANTS( WasherDeviceClass )
EW_END_OF_CLASS_VARIANTS( WasherDeviceClass )

/* Virtual Method Table (VMT) for the class : 'Washer::DeviceClass' */
EW_DEFINE_CLASS( WasherDeviceClass, XObject, WashingProgram, WashingProgram, WashingProgram, 
                 WashingProgram, WashingProgram, ProgramNumber, "Washer::DeviceClass" )
EW_END_OF_CLASS( WasherDeviceClass )

/* Initializer for the class 'Washer::ProgList' */
void WasherProgList__Init( WasherProgList _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherProgList );

  /* ... then construct all embedded objects */
  CoreSlideTouchHandler__Init( &_this->SlideTouchHandler, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->BottomList, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->MainList, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->TopList, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListIn, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Caption, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeCaption, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListOut, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->Int32Effect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->AutoDemoEffect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherProgList );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0019 );
  CoreRectView__OnSetBounds( &_this->SlideTouchHandler, _Const0019 );
  CoreSlideTouchHandler_OnSetSnapNext( &_this->SlideTouchHandler, _Const001A );
  CoreSlideTouchHandler_OnSetFriction( &_this->SlideTouchHandler, 0.100000f );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, _Const001B );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, _Const001C );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, _Const001D );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, _Const001E );
  CoreView_OnSetLayout((CoreView)&_this->Rectangle, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const001F );
  ViewsRectangle_OnSetColor( &_this->Rectangle, WasherButtonBgColor );
  CoreRectView__OnSetBounds( &_this->BottomList, _Const0020 );
  CoreGroup__OnSetOpacity( &_this->BottomList, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->BottomList, 24 );
  CoreVerticalList_OnSetNoOfItems( &_this->BottomList, 8 );
  CoreVerticalList_OnSetItemClass( &_this->BottomList, EW_CLASS( WasherProgItemS ));
  CoreRectView__OnSetBounds( &_this->MainList, _Const001F );
  CoreVerticalList_OnSetItemHeight( &_this->MainList, 96 );
  CoreVerticalList_OnSetNoOfItems( &_this->MainList, 8 );
  CoreVerticalList_OnSetItemClass( &_this->MainList, EW_CLASS( WasherProgItemL ));
  CoreRectView__OnSetBounds( &_this->TopList, _Const0021 );
  CoreGroup__OnSetOpacity( &_this->TopList, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->TopList, 24 );
  CoreVerticalList_OnSetNoOfItems( &_this->TopList, 8 );
  CoreVerticalList_OnSetItemClass( &_this->TopList, EW_CLASS( WasherProgItemS ));
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListIn, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListIn, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListIn, 300 );
  CoreRectView__OnSetBounds( &_this->Caption, _Const0022 );
  ViewsText_OnSetAlignment( &_this->Caption, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Caption, EwLoadString( &_Const0023 ));
  ViewsText_OnSetColor( &_this->Caption, WasherCaptionColor );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeCaption, EffectsTimingFastIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeCaption, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeCaption, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListOut, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListOut, 1000 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->FadeListOut, 2000 );
  _this->FadeListOut.Value2 = 0;
  _this->FadeListOut.Value1 = 255;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->Int32Effect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->Int32Effect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->Int32Effect, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->AutoDemoEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->AutoDemoEffect, 300 );
  CoreGroup__Add( _this, ((CoreView)&_this->SlideTouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BottomList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->MainList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TopList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Caption ), 0 );
  _this->SlideTouchHandler.OnEnd = EwNewSlot( _this, WasherProgList_OnEnd );
  _this->SlideTouchHandler.OnStart = EwNewSlot( _this, WasherProgList_OnStart );
  _this->SimpleTouchHandler.OnDrag = EwNewSlot( _this, WasherProgList_OnDrag );
  _this->SimpleTouchHandler.OnRelease = EwNewSlot( _this, WasherProgList_OnRelease );
  _this->SimpleTouchHandler.OnPress = EwNewSlot( _this, WasherProgList_OnStart );
  _this->BottomList.OnLoadItem = EwNewSlot( _this, WasherProgList_OnLoadBottom );
  _this->MainList.OnUpdate = EwNewSlot( _this, WasherProgList_OnUpdate );
  _this->MainList.OnLoadItem = EwNewSlot( _this, WasherProgList_OnLoadMain );
  CoreVerticalList_OnSetSlideHandler( &_this->MainList, &_this->SlideTouchHandler );
  _this->TopList.OnLoadItem = EwNewSlot( _this, WasherProgList_OnLoadTop );
  _this->FadeListIn.Super1.OnAnimate = EwNewSlot( _this, WasherProgList_onAnimate );
  _this->FadeListIn.Outlet = EwNewRef( &_this->TopList, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  ViewsText_OnSetFont( &_this->Caption, EwLoadResource( &WasherDescrFont, ResourcesFont ));
  _this->FadeCaption.Outlet = EwNewRef( &_this->Caption, ViewsText_OnGetOpacity, 
  ViewsText_OnSetOpacity );
  _this->FadeListOut.Super1.OnAnimate = EwNewSlot( _this, WasherProgList_onAnimate );
  _this->FadeListOut.Outlet = EwNewRef( &_this->TopList, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
}

/* Re-Initializer for the class 'Washer::ProgList' */
void WasherProgList__ReInit( WasherProgList _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSlideTouchHandler__ReInit( &_this->SlideTouchHandler );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandler );
  ViewsRectangle__ReInit( &_this->Rectangle );
  CoreVerticalList__ReInit( &_this->BottomList );
  CoreVerticalList__ReInit( &_this->MainList );
  CoreVerticalList__ReInit( &_this->TopList );
  EffectsInt32Effect__ReInit( &_this->FadeListIn );
  ViewsText__ReInit( &_this->Caption );
  EffectsInt32Effect__ReInit( &_this->FadeCaption );
  EffectsInt32Effect__ReInit( &_this->FadeListOut );
  EffectsInt32Effect__ReInit( &_this->Int32Effect );
  EffectsInt32Effect__ReInit( &_this->AutoDemoEffect );
}

/* Finalizer method for the class 'Washer::ProgList' */
void WasherProgList__Done( WasherProgList _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreSlideTouchHandler__Done( &_this->SlideTouchHandler );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandler );
  ViewsRectangle__Done( &_this->Rectangle );
  CoreVerticalList__Done( &_this->BottomList );
  CoreVerticalList__Done( &_this->MainList );
  CoreVerticalList__Done( &_this->TopList );
  EffectsInt32Effect__Done( &_this->FadeListIn );
  ViewsText__Done( &_this->Caption );
  EffectsInt32Effect__Done( &_this->FadeCaption );
  EffectsInt32Effect__Done( &_this->FadeListOut );
  EffectsInt32Effect__Done( &_this->Int32Effect );
  EffectsInt32Effect__Done( &_this->AutoDemoEffect );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Slot method to load content of main scroll list. */
void WasherProgList_OnLoadMain( WasherProgList _this, XObject sender )
{
  WasherProgItemL itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->MainList.View, WasherProgItemL );
  itemNo = _this->MainList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 8 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->WashingProgram[ 
    EwCheckIndex( itemNo, 8 )];

  WasherProgItemL_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->MainList.Super2.Bounds ), _this->MainList.ItemHeight )));
}

/* Slot method to load content of upper scroll list. */
void WasherProgList_OnLoadTop( WasherProgList _this, XObject sender )
{
  WasherProgItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->TopList.View, WasherProgItemS );
  itemNo = _this->TopList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 8 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->WashingProgram[ 
    EwCheckIndex( itemNo, 8 )];

  WasherProgItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->TopList.Super2.Bounds ), _this->TopList.ItemHeight )));
}

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void WasherProgList_OnUpdate( WasherProgList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreVerticalList_OnSetScrollOffset( &_this->TopList, (( _this->MainList.ScrollOffset 
  * _this->TopList.ItemHeight ) / _this->MainList.ItemHeight ) + EwGetRectH( _this->TopList.Super2.Bounds ));
  CoreVerticalList_OnSetScrollOffset( &_this->BottomList, (( _this->MainList.ScrollOffset 
  * _this->BottomList.ItemHeight ) / _this->MainList.ItemHeight ) - _this->BottomList.ItemHeight );
}

/* Slot method to load content of lower scroll list. */
void WasherProgList_OnLoadBottom( WasherProgList _this, XObject sender )
{
  WasherProgItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->BottomList.View, WasherProgItemS );
  itemNo = _this->BottomList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 8 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->WashingProgram[ 
    EwCheckIndex( itemNo, 8 )];

  WasherProgItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->BottomList.Super2.Bounds ), _this->BottomList.ItemHeight )));
}

/* Slot method to change the selection, when the item has reached its parking position. */
void WasherProgList_OnEnd( WasherProgList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  WasherDeviceClass_OnSetProgramNumber( EwGetAutoObject( &WasherDevice, WasherDeviceClass ), 
  -_this->MainList.ScrollOffset / _this->MainList.ItemHeight );
  _this->FadeListOut.Value1 = _this->TopList.Super1.Opacity;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 255;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandler, 1 );
}

/* Slot method to make the upper/lower list visible. */
void WasherProgList_OnStart( WasherProgList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->TopList.Super1.Opacity != 255 ) && !_this->FadeListIn.Super1.Enabled )
  {
    _this->FadeListIn.Value1 = _this->TopList.Super1.Opacity;
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListIn, 1 );
  }

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 0;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
}

/* This is a slot method. */
void WasherProgList_onAnimate( WasherProgList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup__OnSetOpacity( &_this->BottomList, _this->TopList.Super1.Opacity );
}

/* 'C' function for method : 'Washer::ProgList.OnDrag()' */
void WasherProgList_OnDrag( WasherProgList _this, XObject sender )
{
  XPoint delta;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  delta = EwMovePointNeg( _this->SimpleTouchHandler.CurrentPos, _this->SimpleTouchHandler.HittingPos );

  if (( delta.Y > 8 ) || ( delta.Y < -8 ))
  {
    CoreRoot_DeflectCursor( CoreView__GetRoot( _this ), ((CoreView)&_this->SlideTouchHandler ), 
    _Const001E );
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandler, 0 );
  }
}

/* 'C' function for method : 'Washer::ProgList.OnRelease()' */
void WasherProgList_OnRelease( WasherProgList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->SimpleTouchHandler.AutoDeflected && ( _this->TopList.Super1.Opacity 
      == 255 ))
  {
    XInt32 delta = 0;

    if ( _this->SimpleTouchHandler.CurrentPos.Y < _this->MainList.Super2.Bounds.Point1.Y )
      delta = (( _this->SimpleTouchHandler.CurrentPos.Y - _this->MainList.Super2.Bounds.Point1.Y ) 
      / _this->TopList.ItemHeight ) - 1;
    else
      if ( _this->SimpleTouchHandler.CurrentPos.Y > _this->MainList.Super2.Bounds.Point2.Y )
        delta = (( _this->SimpleTouchHandler.CurrentPos.Y - _this->MainList.Super2.Bounds.Point2.Y ) 
        / _this->BottomList.ItemHeight ) + 1;

    if ( delta != 0 )
    {
      XInt32 item = ( -_this->MainList.ScrollOffset / _this->MainList.ItemHeight ) 
        + delta;
      CoreVerticalList_EnsureVisible( &_this->MainList, item, 1, &_this->Int32Effect, 
      EwNullSlot );
      WasherDeviceClass_OnSetProgramNumber( EwGetAutoObject( &WasherDevice, WasherDeviceClass ), 
      item );
    }
  }

  _this->FadeListOut.Value1 = _this->TopList.Super1.Opacity;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 255;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
}

/* Slot method to trigger the next phase of the auto demo. */
void WasherProgList_AutoDemo( WasherProgList _this, XObject sender )
{
  XInt32 val;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( EwNewSlot( _this, WasherProgList_OnStart ), ((XObject)_this ));
  val = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->ProgramNumber + 1;

  if ( val >= 8 )
    val = 0;

  CoreVerticalList_EnsureVisible( &_this->MainList, val, 1, &_this->AutoDemoEffect, 
  EwNewSlot( _this, WasherProgList_OnEnd ));
}

/* Variants derived from the class : 'Washer::ProgList' */
EW_DEFINE_CLASS_VARIANTS( WasherProgList )
EW_END_OF_CLASS_VARIANTS( WasherProgList )

/* Virtual Method Table (VMT) for the class : 'Washer::ProgList' */
EW_DEFINE_CLASS( WasherProgList, CoreGroup, SlideTouchHandler, SlideTouchHandler, 
                 SlideTouchHandler, SlideTouchHandler, _None, _None, "Washer::ProgList" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherProgList )

/* Initializer for the class 'Washer::ProgItemL' */
void WasherProgItemL__Init( WasherProgItemL _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherProgItemL );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherProgItemL );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0024 );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0025 );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0026 ));
  ViewsText_OnSetColor( &_this->Text, WasherButtonTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontL, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::ProgItemL' */
void WasherProgItemL__ReInit( WasherProgItemL _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::ProgItemL' */
void WasherProgItemL__Done( WasherProgItemL _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::ProgItemL.OnSetCaption()' */
void WasherProgItemL_OnSetCaption( WasherProgItemL _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Washer::ProgItemL' */
EW_DEFINE_CLASS_VARIANTS( WasherProgItemL )
EW_END_OF_CLASS_VARIANTS( WasherProgItemL )

/* Virtual Method Table (VMT) for the class : 'Washer::ProgItemL' */
EW_DEFINE_CLASS( WasherProgItemL, CoreGroup, Text, Text, Text, Text, Caption, _None, 
                 "Washer::ProgItemL" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherProgItemL )

/* Include a file containing the font resource : 'Washer::ListFontS' */
#include "_WasherListFontS.h"

/* Table with links to derived variants of the font resource : 'Washer::ListFontS' */
EW_RES_WITHOUT_VARIANTS( WasherListFontS )

/* Include a file containing the font resource : 'Washer::DescrFont' */
#include "_WasherDescrFont.h"

/* Table with links to derived variants of the font resource : 'Washer::DescrFont' */
EW_RES_WITHOUT_VARIANTS( WasherDescrFont )

/* Include a file containing the font resource : 'Washer::ListFontM' */
#include "_WasherListFontM.h"

/* Table with links to derived variants of the font resource : 'Washer::ListFontM' */
EW_RES_WITHOUT_VARIANTS( WasherListFontM )

/* Include a file containing the font resource : 'Washer::ListFontL' */
#include "_WasherListFontL.h"

/* Table with links to derived variants of the font resource : 'Washer::ListFontL' */
EW_RES_WITHOUT_VARIANTS( WasherListFontL )

/* Color constant according UI design. */
const XColor WasherButtonBgColor = { 0xE7, 0xEA, 0xEA, 0xFF };

/* Color constant according UI design. */
const XColor WasherCaptionColor = { 0x70, 0x90, 0xA6, 0xFF };

/* Color constant according UI design. */
const XColor WasherButtonTextColor = { 0x1B, 0x4C, 0x59, 0xFF };

/* User defined auto object: 'Washer::Device' */
EW_DEFINE_AUTOOBJECT( WasherDevice, WasherDeviceClass )

/* Initializer for the auto object 'Washer::Device' */
void WasherDevice__Init( WasherDeviceClass _this )
{
  EW_UNUSED_ARG( _this );
}

/* Table with links to derived variants of the auto object : 'Washer::Device' */
EW_DEFINE_AUTOOBJECT_VARIANTS( WasherDevice )
EW_END_OF_AUTOOBJECT_VARIANTS( WasherDevice )

/* Initializer for the class 'Washer::ProgItemS' */
void WasherProgItemS__Init( WasherProgItemS _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherProgItemS );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Back, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Left, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Right, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherProgItemS );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0027 );
  CoreView_OnSetLayout((CoreView)&_this->Back, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Back, _Const0027 );
  ViewsRectangle_OnSetColor( &_this->Back, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Left, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Left, _Const0028 );
  ViewsRectangle_OnSetColor( &_this->Left, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Right, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Right, _Const0029 );
  ViewsRectangle_OnSetColor( &_this->Right, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const002A );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0026 ));
  ViewsText_OnSetColor( &_this->Text, WasherListTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Back ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Left ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Right ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontS, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::ProgItemS' */
void WasherProgItemS__ReInit( WasherProgItemS _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Back );
  ViewsRectangle__ReInit( &_this->Left );
  ViewsRectangle__ReInit( &_this->Right );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::ProgItemS' */
void WasherProgItemS__Done( WasherProgItemS _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Back );
  ViewsRectangle__Done( &_this->Left );
  ViewsRectangle__Done( &_this->Right );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::ProgItemS.OnSetCaption()' */
void WasherProgItemS_OnSetCaption( WasherProgItemS _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Washer::ProgItemS' */
EW_DEFINE_CLASS_VARIANTS( WasherProgItemS )
EW_END_OF_CLASS_VARIANTS( WasherProgItemS )

/* Virtual Method Table (VMT) for the class : 'Washer::ProgItemS' */
EW_DEFINE_CLASS( WasherProgItemS, CoreGroup, Back, Back, Back, Back, Caption, _None, 
                 "Washer::ProgItemS" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherProgItemS )

/* Color constant according UI design. */
const XColor WasherListBgColor = { 0xFF, 0xFF, 0xFF, 0x40 };

/* Color constant according UI design. */
const XColor WasherListTextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

/* Initializer for the class 'Washer::TempList' */
void WasherTempList__Init( WasherTempList _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherTempList );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Back, &_this->_XObject, 0 );
  CoreSlideTouchHandler__Init( &_this->SlideTouchHandler, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->BottomList, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->MainList, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->TopList, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListIn, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Caption, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeCaption, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListOut, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->Int32Effect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->AutoDemoEffect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherTempList );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const002B );
  CoreView_OnSetLayout((CoreView)&_this->Back, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Back, _Const002C );
  ViewsRectangle_OnSetColor( &_this->Back, _Const002D );
  CoreRectView__OnSetBounds( &_this->SlideTouchHandler, _Const002B );
  CoreSlideTouchHandler_OnSetSnapNext( &_this->SlideTouchHandler, _Const001A );
  CoreSlideTouchHandler_OnSetFriction( &_this->SlideTouchHandler, 0.100000f );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, _Const001B );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, _Const002E );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, _Const002F );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, _Const001E );
  CoreView_OnSetLayout((CoreView)&_this->Rectangle, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const0030 );
  ViewsRectangle_OnSetColor( &_this->Rectangle, WasherButtonBgColor );
  CoreRectView__OnSetBounds( &_this->BottomList, _Const0031 );
  CoreGroup__OnSetOpacity( &_this->BottomList, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->BottomList, 24 );
  CoreVerticalList_OnSetNoOfItems( &_this->BottomList, 6 );
  CoreVerticalList_OnSetItemClass( &_this->BottomList, EW_CLASS( WasherTempItemS ));
  CoreRectView__OnSetBounds( &_this->MainList, _Const0030 );
  CoreGroup__OnSetOpacity( &_this->MainList, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->MainList, 96 );
  CoreVerticalList_OnSetNoOfItems( &_this->MainList, 6 );
  CoreVerticalList_OnSetItemClass( &_this->MainList, EW_CLASS( WasherTempItemL ));
  CoreRectView__OnSetBounds( &_this->TopList, _Const0032 );
  CoreGroup__OnSetOpacity( &_this->TopList, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->TopList, 24 );
  CoreVerticalList_OnSetNoOfItems( &_this->TopList, 6 );
  CoreVerticalList_OnSetItemClass( &_this->TopList, EW_CLASS( WasherTempItemS ));
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListIn, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListIn, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListIn, 300 );
  CoreRectView__OnSetBounds( &_this->Caption, _Const0033 );
  ViewsText_OnSetAlignment( &_this->Caption, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Caption, EwLoadString( &_Const0034 ));
  ViewsText_OnSetColor( &_this->Caption, WasherCaptionColor );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeCaption, EffectsTimingFastIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeCaption, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeCaption, 200 );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0030 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwGetAutoObject( &WasherDevice, WasherDeviceClass )->WashingTemp[ 
  0 ]);
  ViewsText_OnSetColor( &_this->Text, EwBlendColor( WasherTempColor0, WasherTempColor1 ));
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListOut, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListOut, 1000 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->FadeListOut, 2000 );
  _this->FadeListOut.Value2 = 0;
  _this->FadeListOut.Value1 = 255;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->Int32Effect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->Int32Effect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->Int32Effect, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->AutoDemoEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->AutoDemoEffect, 300 );
  CoreGroup__Add( _this, ((CoreView)&_this->Back ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SlideTouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BottomList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->MainList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TopList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Caption ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  _this->SlideTouchHandler.OnEnd = EwNewSlot( _this, WasherTempList_OnEnd );
  _this->SlideTouchHandler.OnStart = EwNewSlot( _this, WasherTempList_OnStart );
  _this->SimpleTouchHandler.OnDrag = EwNewSlot( _this, WasherTempList_OnDrag );
  _this->SimpleTouchHandler.OnRelease = EwNewSlot( _this, WasherTempList_OnRelease );
  _this->SimpleTouchHandler.OnPress = EwNewSlot( _this, WasherTempList_OnStart );
  _this->BottomList.OnLoadItem = EwNewSlot( _this, WasherTempList_OnLoadBottom );
  _this->MainList.OnUpdate = EwNewSlot( _this, WasherTempList_OnUpdate );
  _this->MainList.OnLoadItem = EwNewSlot( _this, WasherTempList_OnLoadMain );
  CoreVerticalList_OnSetSlideHandler( &_this->MainList, &_this->SlideTouchHandler );
  _this->TopList.OnLoadItem = EwNewSlot( _this, WasherTempList_OnLoadTop );
  _this->FadeListIn.Super1.OnAnimate = EwNewSlot( _this, WasherTempList_onAnimate );
  _this->FadeListIn.Outlet = EwNewRef( &_this->TopList, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  ViewsText_OnSetFont( &_this->Caption, EwLoadResource( &WasherDescrFont, ResourcesFont ));
  _this->FadeCaption.Outlet = EwNewRef( &_this->Caption, ViewsText_OnGetOpacity, 
  ViewsText_OnSetOpacity );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontL, ResourcesFont ));
  _this->FadeListOut.Super1.OnAnimate = EwNewSlot( _this, WasherTempList_onAnimate );
  _this->FadeListOut.Outlet = EwNewRef( &_this->TopList, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
}

/* Re-Initializer for the class 'Washer::TempList' */
void WasherTempList__ReInit( WasherTempList _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Back );
  CoreSlideTouchHandler__ReInit( &_this->SlideTouchHandler );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandler );
  ViewsRectangle__ReInit( &_this->Rectangle );
  CoreVerticalList__ReInit( &_this->BottomList );
  CoreVerticalList__ReInit( &_this->MainList );
  CoreVerticalList__ReInit( &_this->TopList );
  EffectsInt32Effect__ReInit( &_this->FadeListIn );
  ViewsText__ReInit( &_this->Caption );
  EffectsInt32Effect__ReInit( &_this->FadeCaption );
  ViewsText__ReInit( &_this->Text );
  EffectsInt32Effect__ReInit( &_this->FadeListOut );
  EffectsInt32Effect__ReInit( &_this->Int32Effect );
  EffectsInt32Effect__ReInit( &_this->AutoDemoEffect );
}

/* Finalizer method for the class 'Washer::TempList' */
void WasherTempList__Done( WasherTempList _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Back );
  CoreSlideTouchHandler__Done( &_this->SlideTouchHandler );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandler );
  ViewsRectangle__Done( &_this->Rectangle );
  CoreVerticalList__Done( &_this->BottomList );
  CoreVerticalList__Done( &_this->MainList );
  CoreVerticalList__Done( &_this->TopList );
  EffectsInt32Effect__Done( &_this->FadeListIn );
  ViewsText__Done( &_this->Caption );
  EffectsInt32Effect__Done( &_this->FadeCaption );
  ViewsText__Done( &_this->Text );
  EffectsInt32Effect__Done( &_this->FadeListOut );
  EffectsInt32Effect__Done( &_this->Int32Effect );
  EffectsInt32Effect__Done( &_this->AutoDemoEffect );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Slot method to load content of main scroll list. */
void WasherTempList_OnLoadMain( WasherTempList _this, XObject sender )
{
  WasherTempItemL itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->MainList.View, WasherTempItemL );
  itemNo = _this->MainList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 6 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->WashingTemp[ 
    EwCheckIndex( itemNo, 6 )];

  WasherTempItemL_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->MainList.Super2.Bounds ), _this->MainList.ItemHeight )));

  switch ( itemNo )
  {
    case 0 :
    {
      WasherTempItemL_OnSetColorTop( itemView, WasherTempColor0 );
      WasherTempItemL_OnSetColorBottom( itemView, WasherTempColor1 );
    }
    break;

    case 1 :
    {
      WasherTempItemL_OnSetColorTop( itemView, WasherTempColor1 );
      WasherTempItemL_OnSetColorBottom( itemView, WasherTempColor2 );
    }
    break;

    case 2 :
    {
      WasherTempItemL_OnSetColorTop( itemView, WasherTempColor2 );
      WasherTempItemL_OnSetColorBottom( itemView, WasherTempColor3 );
    }
    break;

    case 3 :
    {
      WasherTempItemL_OnSetColorTop( itemView, WasherTempColor3 );
      WasherTempItemL_OnSetColorBottom( itemView, WasherTempColor4 );
    }
    break;

    case 4 :
    {
      WasherTempItemL_OnSetColorTop( itemView, WasherTempColor4 );
      WasherTempItemL_OnSetColorBottom( itemView, WasherTempColor5 );
    }
    break;

    case 5 :
    {
      WasherTempItemL_OnSetColorTop( itemView, WasherTempColor5 );
      WasherTempItemL_OnSetColorBottom( itemView, WasherTempColor6 );
    }
    break;

    default : 
      ;
  }
}

/* Slot method to load content of upper scroll list. */
void WasherTempList_OnLoadTop( WasherTempList _this, XObject sender )
{
  WasherTempItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->TopList.View, WasherTempItemS );
  itemNo = _this->TopList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 6 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->WashingTemp[ 
    EwCheckIndex( itemNo, 6 )];

  WasherTempItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->TopList.Super2.Bounds ), _this->TopList.ItemHeight )));
}

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void WasherTempList_OnUpdate( WasherTempList _this, XObject sender )
{
  XInt32 itemNo;
  XInt32 dx;
  XColor c1;
  XColor c2;
  XInt32 r;
  XInt32 g;
  XInt32 b;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreVerticalList_OnSetScrollOffset( &_this->TopList, (( _this->MainList.ScrollOffset 
  * _this->TopList.ItemHeight ) / _this->MainList.ItemHeight ) + EwGetRectH( _this->TopList.Super2.Bounds ));
  CoreVerticalList_OnSetScrollOffset( &_this->BottomList, (( _this->MainList.ScrollOffset 
  * _this->BottomList.ItemHeight ) / _this->MainList.ItemHeight ) - _this->BottomList.ItemHeight );
  itemNo = ( -_this->MainList.ScrollOffset + ( _this->MainList.ItemHeight / 2 )) 
  / _this->MainList.ItemHeight;
  dx = ((( -_this->MainList.ScrollOffset + ( _this->MainList.ItemHeight / 2 )) % 
  _this->MainList.ItemHeight ) * 255 ) / _this->MainList.ItemHeight;

  if ( dx < 0 )
    dx = 0;

  c1 = WasherTempColor6;
  c2 = WasherTempColor6;

  switch ( itemNo )
  {
    case 0 :
    {
      c1 = WasherTempColor0;
      c2 = WasherTempColor1;
    }
    break;

    case 1 :
    {
      c1 = WasherTempColor1;
      c2 = WasherTempColor2;
    }
    break;

    case 2 :
    {
      c1 = WasherTempColor2;
      c2 = WasherTempColor3;
    }
    break;

    case 3 :
    {
      c1 = WasherTempColor3;
      c2 = WasherTempColor4;
    }
    break;

    case 4 :
    {
      c1 = WasherTempColor4;
      c2 = WasherTempColor5;
    }
    break;

    case 5 :
    {
      c1 = WasherTempColor5;
      c2 = WasherTempColor6;
    }
    break;

    default : 
      dx = 0;
  }

  r = (( c1.Red * ( 255 - dx )) + ( c2.Red * dx )) / 255;
  g = (( c1.Green * ( 255 - dx )) + ( c2.Green * dx )) / 255;
  b = (( c1.Blue * ( 255 - dx )) + ( c2.Blue * dx )) / 255;
  ViewsText_OnSetColor( &_this->Text, EwNewColor((XUInt8)r, (XUInt8)g, (XUInt8)b, 
  255 ));

  if ( _this->ColorOutlet.Object != 0 )
    EwOnSetColor( _this->ColorOutlet, ViewsText_OnGetColor( &_this->Text ));
}

/* Slot method to load content of lower scroll list. */
void WasherTempList_OnLoadBottom( WasherTempList _this, XObject sender )
{
  WasherTempItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->BottomList.View, WasherTempItemS );
  itemNo = _this->BottomList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 6 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->WashingTemp[ 
    EwCheckIndex( itemNo, 6 )];

  WasherTempItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->BottomList.Super2.Bounds ), _this->BottomList.ItemHeight )));
}

/* Slot method to change the selection, when the item has reached its parking position. */
void WasherTempList_OnEnd( WasherTempList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  WasherDeviceClass_OnSetTempNumber( EwGetAutoObject( &WasherDevice, WasherDeviceClass ), 
  -_this->MainList.ScrollOffset / _this->MainList.ItemHeight );
  ViewsText_OnSetString( &_this->Text, EwGetAutoObject( &WasherDevice, WasherDeviceClass )->WashingTemp[ 
  EwCheckIndex( EwGetAutoObject( &WasherDevice, WasherDeviceClass )->TempNumber, 
  6 )]);
  _this->FadeListOut.Value1 = _this->TopList.Super1.Opacity;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 255;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandler, 1 );
}

/* Slot method to make the upper/lower list visible. */
void WasherTempList_OnStart( WasherTempList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->TopList.Super1.Opacity != 255 ) && !_this->FadeListIn.Super1.Enabled )
  {
    _this->FadeListIn.Value1 = _this->TopList.Super1.Opacity;
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListIn, 1 );
  }

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 0;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
}

/* This is a slot method. */
void WasherTempList_onAnimate( WasherTempList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup__OnSetOpacity( &_this->BottomList, _this->TopList.Super1.Opacity );
  CoreGroup__OnSetOpacity( &_this->MainList, _this->TopList.Super1.Opacity );
  ViewsText_OnSetOpacity( &_this->Text, 255 - _this->TopList.Super1.Opacity );
}

/* 'C' function for method : 'Washer::TempList.OnDrag()' */
void WasherTempList_OnDrag( WasherTempList _this, XObject sender )
{
  XPoint delta;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  delta = EwMovePointNeg( _this->SimpleTouchHandler.CurrentPos, _this->SimpleTouchHandler.HittingPos );

  if (( delta.Y > 8 ) || ( delta.Y < -8 ))
  {
    CoreRoot_DeflectCursor( CoreView__GetRoot( _this ), ((CoreView)&_this->SlideTouchHandler ), 
    _Const001E );
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandler, 0 );
  }
}

/* 'C' function for method : 'Washer::TempList.OnRelease()' */
void WasherTempList_OnRelease( WasherTempList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->SimpleTouchHandler.AutoDeflected && ( _this->TopList.Super1.Opacity 
      == 255 ))
  {
    XInt32 delta = 0;

    if ( _this->SimpleTouchHandler.CurrentPos.Y < _this->MainList.Super2.Bounds.Point1.Y )
      delta = (( _this->SimpleTouchHandler.CurrentPos.Y - _this->MainList.Super2.Bounds.Point1.Y ) 
      / _this->TopList.ItemHeight ) - 1;
    else
      if ( _this->SimpleTouchHandler.CurrentPos.Y > _this->MainList.Super2.Bounds.Point2.Y )
        delta = (( _this->SimpleTouchHandler.CurrentPos.Y - _this->MainList.Super2.Bounds.Point2.Y ) 
        / _this->BottomList.ItemHeight ) + 1;

    if ( delta != 0 )
    {
      XInt32 item = ( -_this->MainList.ScrollOffset / _this->MainList.ItemHeight ) 
        + delta;
      CoreVerticalList_EnsureVisible( &_this->MainList, item, 1, &_this->Int32Effect, 
      EwNullSlot );
    }
  }

  EwSignal( EwNewSlot( _this, WasherTempList_OnEnd ), ((XObject)_this ));
}

/* Slot method to trigger the next phase of the auto demo. */
void WasherTempList_AutoDemo( WasherTempList _this, XObject sender )
{
  XInt32 val;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( EwNewSlot( _this, WasherTempList_OnStart ), ((XObject)_this ));
  val = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->TempNumber + 1;

  if ( val >= 6 )
    val = 0;

  CoreVerticalList_EnsureVisible( &_this->MainList, val, 1, &_this->AutoDemoEffect, 
  EwNewSlot( _this, WasherTempList_OnEnd ));
}

/* Variants derived from the class : 'Washer::TempList' */
EW_DEFINE_CLASS_VARIANTS( WasherTempList )
EW_END_OF_CLASS_VARIANTS( WasherTempList )

/* Virtual Method Table (VMT) for the class : 'Washer::TempList' */
EW_DEFINE_CLASS( WasherTempList, CoreGroup, ColorOutlet, ColorOutlet, ColorOutlet, 
                 Back, _None, _None, "Washer::TempList" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherTempList )

/* Initializer for the class 'Washer::TempItemL' */
void WasherTempItemL__Init( WasherTempItemL _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherTempItemL );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherTempItemL );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0035 );
  CoreView_OnSetLayout((CoreView)&_this->Rectangle, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const0035 );
  ViewsRectangle_OnSetColor( &_this->Rectangle, WasherButtonBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0035 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0026 ));
  ViewsText_OnSetColor( &_this->Text, WasherListTextColor );
  _this->ColorTop = _Const0036;
  _this->ColorBottom = _Const0036;
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontL, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::TempItemL' */
void WasherTempItemL__ReInit( WasherTempItemL _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Rectangle );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::TempItemL' */
void WasherTempItemL__Done( WasherTempItemL _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Rectangle );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::TempItemL.OnSetCaption()' */
void WasherTempItemL_OnSetCaption( WasherTempItemL _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* 'C' function for method : 'Washer::TempItemL.OnSetColorTop()' */
void WasherTempItemL_OnSetColorTop( WasherTempItemL _this, XColor value )
{
  _this->ColorTop = value;
  ViewsRectangle_OnSetColorTL( &_this->Rectangle, value );
  ViewsRectangle_OnSetColorTR( &_this->Rectangle, value );
}

/* 'C' function for method : 'Washer::TempItemL.OnSetColorBottom()' */
void WasherTempItemL_OnSetColorBottom( WasherTempItemL _this, XColor value )
{
  _this->ColorBottom = value;
  ViewsRectangle_OnSetColorBL( &_this->Rectangle, value );
  ViewsRectangle_OnSetColorBR( &_this->Rectangle, value );
}

/* Variants derived from the class : 'Washer::TempItemL' */
EW_DEFINE_CLASS_VARIANTS( WasherTempItemL )
EW_END_OF_CLASS_VARIANTS( WasherTempItemL )

/* Virtual Method Table (VMT) for the class : 'Washer::TempItemL' */
EW_DEFINE_CLASS( WasherTempItemL, CoreGroup, Rectangle, Rectangle, Rectangle, Rectangle, 
                 Caption, ColorTop, "Washer::TempItemL" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherTempItemL )

/* Initializer for the class 'Washer::TempItemS' */
void WasherTempItemS__Init( WasherTempItemS _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherTempItemS );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Back, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Right, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherTempItemS );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0037 );
  CoreView_OnSetLayout((CoreView)&_this->Back, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Back, _Const0037 );
  ViewsRectangle_OnSetColor( &_this->Back, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Right, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Right, _Const0038 );
  ViewsRectangle_OnSetColor( &_this->Right, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0039 );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0026 ));
  ViewsText_OnSetColor( &_this->Text, WasherListTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Back ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Right ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontS, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::TempItemS' */
void WasherTempItemS__ReInit( WasherTempItemS _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Back );
  ViewsRectangle__ReInit( &_this->Right );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::TempItemS' */
void WasherTempItemS__Done( WasherTempItemS _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Back );
  ViewsRectangle__Done( &_this->Right );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::TempItemS.OnSetCaption()' */
void WasherTempItemS_OnSetCaption( WasherTempItemS _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Washer::TempItemS' */
EW_DEFINE_CLASS_VARIANTS( WasherTempItemS )
EW_END_OF_CLASS_VARIANTS( WasherTempItemS )

/* Virtual Method Table (VMT) for the class : 'Washer::TempItemS' */
EW_DEFINE_CLASS( WasherTempItemS, CoreGroup, Back, Back, Back, Back, Caption, _None, 
                 "Washer::TempItemS" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherTempItemS )

/* Initializer for the class 'Washer::SpinList' */
void WasherSpinList__Init( WasherSpinList _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherSpinList );

  /* ... then construct all embedded objects */
  CoreSlideTouchHandler__Init( &_this->SlideTouchHandler, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->BottomList, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->MainList, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->TopList, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListIn, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Caption, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeCaption, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListOut, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->Int32Effect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->AutoDemoEffect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherSpinList );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const002B );
  CoreRectView__OnSetBounds( &_this->SlideTouchHandler, _Const002B );
  CoreSlideTouchHandler_OnSetSnapNext( &_this->SlideTouchHandler, _Const001A );
  CoreSlideTouchHandler_OnSetFriction( &_this->SlideTouchHandler, 0.100000f );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, _Const001B );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, _Const002E );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, _Const002F );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, _Const001E );
  CoreView_OnSetLayout((CoreView)&_this->Rectangle, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const0030 );
  ViewsRectangle_OnSetColor( &_this->Rectangle, WasherButtonBgColor );
  CoreRectView__OnSetBounds( &_this->BottomList, _Const0031 );
  CoreGroup__OnSetOpacity( &_this->BottomList, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->BottomList, 24 );
  CoreVerticalList_OnSetNoOfItems( &_this->BottomList, 7 );
  CoreVerticalList_OnSetItemClass( &_this->BottomList, EW_CLASS( WasherSpinItemS ));
  CoreRectView__OnSetBounds( &_this->MainList, _Const0030 );
  CoreVerticalList_OnSetItemHeight( &_this->MainList, 96 );
  CoreVerticalList_OnSetNoOfItems( &_this->MainList, 7 );
  CoreVerticalList_OnSetItemClass( &_this->MainList, EW_CLASS( WasherSpinItemL ));
  CoreRectView__OnSetBounds( &_this->TopList, _Const0032 );
  CoreGroup__OnSetOpacity( &_this->TopList, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->TopList, 24 );
  CoreVerticalList_OnSetNoOfItems( &_this->TopList, 7 );
  CoreVerticalList_OnSetItemClass( &_this->TopList, EW_CLASS( WasherSpinItemS ));
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListIn, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListIn, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListIn, 300 );
  CoreRectView__OnSetBounds( &_this->Caption, _Const0033 );
  ViewsText_OnSetAlignment( &_this->Caption, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Caption, EwLoadString( &_Const003A ));
  ViewsText_OnSetColor( &_this->Caption, WasherCaptionColor );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeCaption, EffectsTimingFastIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeCaption, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeCaption, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListOut, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListOut, 1000 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->FadeListOut, 2000 );
  _this->FadeListOut.Value2 = 0;
  _this->FadeListOut.Value1 = 255;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->Int32Effect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->Int32Effect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->Int32Effect, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->AutoDemoEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->AutoDemoEffect, 300 );
  CoreGroup__Add( _this, ((CoreView)&_this->SlideTouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BottomList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->MainList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TopList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Caption ), 0 );
  _this->SlideTouchHandler.OnEnd = EwNewSlot( _this, WasherSpinList_OnEnd );
  _this->SlideTouchHandler.OnStart = EwNewSlot( _this, WasherSpinList_OnStart );
  _this->SimpleTouchHandler.OnDrag = EwNewSlot( _this, WasherSpinList_OnDrag );
  _this->SimpleTouchHandler.OnRelease = EwNewSlot( _this, WasherSpinList_OnRelease );
  _this->SimpleTouchHandler.OnPress = EwNewSlot( _this, WasherSpinList_OnStart );
  _this->BottomList.OnLoadItem = EwNewSlot( _this, WasherSpinList_OnLoadBottom );
  _this->MainList.OnUpdate = EwNewSlot( _this, WasherSpinList_OnUpdate );
  _this->MainList.OnLoadItem = EwNewSlot( _this, WasherSpinList_OnLoadMain );
  CoreVerticalList_OnSetSlideHandler( &_this->MainList, &_this->SlideTouchHandler );
  _this->TopList.OnLoadItem = EwNewSlot( _this, WasherSpinList_OnLoadTop );
  _this->FadeListIn.Super1.OnAnimate = EwNewSlot( _this, WasherSpinList_onAnimate );
  _this->FadeListIn.Outlet = EwNewRef( &_this->TopList, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  ViewsText_OnSetFont( &_this->Caption, EwLoadResource( &WasherDescrFont, ResourcesFont ));
  _this->FadeCaption.Outlet = EwNewRef( &_this->Caption, ViewsText_OnGetOpacity, 
  ViewsText_OnSetOpacity );
  _this->FadeListOut.Super1.OnAnimate = EwNewSlot( _this, WasherSpinList_onAnimate );
  _this->FadeListOut.Outlet = EwNewRef( &_this->TopList, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
}

/* Re-Initializer for the class 'Washer::SpinList' */
void WasherSpinList__ReInit( WasherSpinList _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSlideTouchHandler__ReInit( &_this->SlideTouchHandler );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandler );
  ViewsRectangle__ReInit( &_this->Rectangle );
  CoreVerticalList__ReInit( &_this->BottomList );
  CoreVerticalList__ReInit( &_this->MainList );
  CoreVerticalList__ReInit( &_this->TopList );
  EffectsInt32Effect__ReInit( &_this->FadeListIn );
  ViewsText__ReInit( &_this->Caption );
  EffectsInt32Effect__ReInit( &_this->FadeCaption );
  EffectsInt32Effect__ReInit( &_this->FadeListOut );
  EffectsInt32Effect__ReInit( &_this->Int32Effect );
  EffectsInt32Effect__ReInit( &_this->AutoDemoEffect );
}

/* Finalizer method for the class 'Washer::SpinList' */
void WasherSpinList__Done( WasherSpinList _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreSlideTouchHandler__Done( &_this->SlideTouchHandler );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandler );
  ViewsRectangle__Done( &_this->Rectangle );
  CoreVerticalList__Done( &_this->BottomList );
  CoreVerticalList__Done( &_this->MainList );
  CoreVerticalList__Done( &_this->TopList );
  EffectsInt32Effect__Done( &_this->FadeListIn );
  ViewsText__Done( &_this->Caption );
  EffectsInt32Effect__Done( &_this->FadeCaption );
  EffectsInt32Effect__Done( &_this->FadeListOut );
  EffectsInt32Effect__Done( &_this->Int32Effect );
  EffectsInt32Effect__Done( &_this->AutoDemoEffect );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Slot method to load content of main scroll list. */
void WasherSpinList_OnLoadMain( WasherSpinList _this, XObject sender )
{
  WasherSpinItemL itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->MainList.View, WasherSpinItemL );
  itemNo = _this->MainList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 7 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->SpinTurn[ EwCheckIndex( 
    itemNo, 7 )];

  WasherSpinItemL_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->MainList.Super2.Bounds ), _this->MainList.ItemHeight )));
}

/* Slot method to load content of upper scroll list. */
void WasherSpinList_OnLoadTop( WasherSpinList _this, XObject sender )
{
  WasherSpinItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->TopList.View, WasherSpinItemS );
  itemNo = _this->TopList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 7 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->SpinTurn[ EwCheckIndex( 
    itemNo, 7 )];

  WasherSpinItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->TopList.Super2.Bounds ), _this->TopList.ItemHeight )));
}

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void WasherSpinList_OnUpdate( WasherSpinList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreVerticalList_OnSetScrollOffset( &_this->TopList, (( _this->MainList.ScrollOffset 
  * _this->TopList.ItemHeight ) / _this->MainList.ItemHeight ) + EwGetRectH( _this->TopList.Super2.Bounds ));
  CoreVerticalList_OnSetScrollOffset( &_this->BottomList, (( _this->MainList.ScrollOffset 
  * _this->BottomList.ItemHeight ) / _this->MainList.ItemHeight ) - _this->BottomList.ItemHeight );
}

/* Slot method to load content of lower scroll list. */
void WasherSpinList_OnLoadBottom( WasherSpinList _this, XObject sender )
{
  WasherSpinItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->BottomList.View, WasherSpinItemS );
  itemNo = _this->BottomList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 7 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->SpinTurn[ EwCheckIndex( 
    itemNo, 7 )];

  WasherSpinItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->BottomList.Super2.Bounds ), _this->BottomList.ItemHeight )));
}

/* Slot method to change the selection, when the item has reached its parking position. */
void WasherSpinList_OnEnd( WasherSpinList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  WasherDeviceClass_OnSetSpinNumber( EwGetAutoObject( &WasherDevice, WasherDeviceClass ), 
  -_this->MainList.ScrollOffset / _this->MainList.ItemHeight );
  _this->FadeListOut.Value1 = _this->TopList.Super1.Opacity;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 255;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandler, 1 );
}

/* Slot method to make the upper/lower list visible. */
void WasherSpinList_OnStart( WasherSpinList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->TopList.Super1.Opacity != 255 ) && !_this->FadeListIn.Super1.Enabled )
  {
    _this->FadeListIn.Value1 = _this->TopList.Super1.Opacity;
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListIn, 1 );
  }

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 0;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
}

/* This is a slot method. */
void WasherSpinList_onAnimate( WasherSpinList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup__OnSetOpacity( &_this->BottomList, _this->TopList.Super1.Opacity );
}

/* 'C' function for method : 'Washer::SpinList.OnDrag()' */
void WasherSpinList_OnDrag( WasherSpinList _this, XObject sender )
{
  XPoint delta;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  delta = EwMovePointNeg( _this->SimpleTouchHandler.CurrentPos, _this->SimpleTouchHandler.HittingPos );

  if (( delta.Y > 8 ) || ( delta.Y < -8 ))
  {
    CoreRoot_DeflectCursor( CoreView__GetRoot( _this ), ((CoreView)&_this->SlideTouchHandler ), 
    _Const001E );
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandler, 0 );
  }
}

/* 'C' function for method : 'Washer::SpinList.OnRelease()' */
void WasherSpinList_OnRelease( WasherSpinList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->SimpleTouchHandler.AutoDeflected && ( _this->TopList.Super1.Opacity 
      == 255 ))
  {
    XInt32 delta = 0;

    if ( _this->SimpleTouchHandler.CurrentPos.Y < _this->MainList.Super2.Bounds.Point1.Y )
      delta = (( _this->SimpleTouchHandler.CurrentPos.Y - _this->MainList.Super2.Bounds.Point1.Y ) 
      / _this->TopList.ItemHeight ) - 1;
    else
      if ( _this->SimpleTouchHandler.CurrentPos.Y > _this->MainList.Super2.Bounds.Point2.Y )
        delta = (( _this->SimpleTouchHandler.CurrentPos.Y - _this->MainList.Super2.Bounds.Point2.Y ) 
        / _this->BottomList.ItemHeight ) + 1;

    if ( delta != 0 )
    {
      XInt32 item = ( -_this->MainList.ScrollOffset / _this->MainList.ItemHeight ) 
        + delta;
      CoreVerticalList_EnsureVisible( &_this->MainList, item, 1, &_this->Int32Effect, 
      EwNullSlot );
    }
  }

  EwSignal( EwNewSlot( _this, WasherSpinList_OnEnd ), ((XObject)_this ));
}

/* Slot method to trigger the next phase of the auto demo. */
void WasherSpinList_AutoDemo( WasherSpinList _this, XObject sender )
{
  XInt32 val;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( EwNewSlot( _this, WasherSpinList_OnStart ), ((XObject)_this ));
  val = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->SpinNumber + 1;

  if ( val >= 7 )
    val = 0;

  CoreVerticalList_EnsureVisible( &_this->MainList, val, 1, &_this->AutoDemoEffect, 
  EwNewSlot( _this, WasherSpinList_OnEnd ));
}

/* Variants derived from the class : 'Washer::SpinList' */
EW_DEFINE_CLASS_VARIANTS( WasherSpinList )
EW_END_OF_CLASS_VARIANTS( WasherSpinList )

/* Virtual Method Table (VMT) for the class : 'Washer::SpinList' */
EW_DEFINE_CLASS( WasherSpinList, CoreGroup, SlideTouchHandler, SlideTouchHandler, 
                 SlideTouchHandler, SlideTouchHandler, _None, _None, "Washer::SpinList" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherSpinList )

/* Initializer for the class 'Washer::SpinItemL' */
void WasherSpinItemL__Init( WasherSpinItemL _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherSpinItemL );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherSpinItemL );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0035 );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const003B );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0026 ));
  ViewsText_OnSetColor( &_this->Text, WasherButtonTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontM, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::SpinItemL' */
void WasherSpinItemL__ReInit( WasherSpinItemL _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::SpinItemL' */
void WasherSpinItemL__Done( WasherSpinItemL _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::SpinItemL.OnSetCaption()' */
void WasherSpinItemL_OnSetCaption( WasherSpinItemL _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Washer::SpinItemL' */
EW_DEFINE_CLASS_VARIANTS( WasherSpinItemL )
EW_END_OF_CLASS_VARIANTS( WasherSpinItemL )

/* Virtual Method Table (VMT) for the class : 'Washer::SpinItemL' */
EW_DEFINE_CLASS( WasherSpinItemL, CoreGroup, Text, Text, Text, Text, Caption, _None, 
                 "Washer::SpinItemL" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherSpinItemL )

/* Initializer for the class 'Washer::SpinItemS' */
void WasherSpinItemS__Init( WasherSpinItemS _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherSpinItemS );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Back, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Right, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherSpinItemS );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0037 );
  CoreView_OnSetLayout((CoreView)&_this->Back, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Back, _Const0037 );
  ViewsRectangle_OnSetColor( &_this->Back, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Right, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Right, _Const0038 );
  ViewsRectangle_OnSetColor( &_this->Right, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0039 );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0026 ));
  ViewsText_OnSetColor( &_this->Text, WasherListTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Back ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Right ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontS, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::SpinItemS' */
void WasherSpinItemS__ReInit( WasherSpinItemS _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Back );
  ViewsRectangle__ReInit( &_this->Right );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::SpinItemS' */
void WasherSpinItemS__Done( WasherSpinItemS _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Back );
  ViewsRectangle__Done( &_this->Right );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::SpinItemS.OnSetCaption()' */
void WasherSpinItemS_OnSetCaption( WasherSpinItemS _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Washer::SpinItemS' */
EW_DEFINE_CLASS_VARIANTS( WasherSpinItemS )
EW_END_OF_CLASS_VARIANTS( WasherSpinItemS )

/* Virtual Method Table (VMT) for the class : 'Washer::SpinItemS' */
EW_DEFINE_CLASS( WasherSpinItemS, CoreGroup, Back, Back, Back, Back, Caption, _None, 
                 "Washer::SpinItemS" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherSpinItemS )

/* Include a file containing the font resource : 'Washer::ListFontXL' */
#include "_WasherListFontXL.h"

/* Table with links to derived variants of the font resource : 'Washer::ListFontXL' */
EW_RES_WITHOUT_VARIANTS( WasherListFontXL )

/* Initializer for the class 'Washer::OptList' */
void WasherOptList__Init( WasherOptList _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherOptList );

  /* ... then construct all embedded objects */
  CoreSlideTouchHandler__Init( &_this->SlideTouchHandler, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->BottomList, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->MainList, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->TopList, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListIn, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Caption, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeCaption, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListOut, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->Int32Effect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->AutoDemoEffect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherOptList );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const003C );
  CoreRectView__OnSetBounds( &_this->SlideTouchHandler, _Const003C );
  CoreSlideTouchHandler_OnSetSnapNext( &_this->SlideTouchHandler, _Const001A );
  CoreSlideTouchHandler_OnSetFriction( &_this->SlideTouchHandler, 0.100000f );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, _Const001B );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, _Const003D );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, _Const003E );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, _Const001E );
  CoreView_OnSetLayout((CoreView)&_this->Rectangle, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const003F );
  ViewsRectangle_OnSetColor( &_this->Rectangle, WasherButtonBgColor );
  CoreRectView__OnSetBounds( &_this->BottomList, _Const0040 );
  CoreGroup__OnSetOpacity( &_this->BottomList, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->BottomList, 36 );
  CoreVerticalList_OnSetNoOfItems( &_this->BottomList, 6 );
  CoreVerticalList_OnSetItemClass( &_this->BottomList, EW_CLASS( WasherOptItemS ));
  CoreRectView__OnSetBounds( &_this->MainList, _Const003F );
  CoreVerticalList_OnSetItemHeight( &_this->MainList, 96 );
  CoreVerticalList_OnSetNoOfItems( &_this->MainList, 6 );
  CoreVerticalList_OnSetItemClass( &_this->MainList, EW_CLASS( WasherOptItemL ));
  CoreRectView__OnSetBounds( &_this->TopList, _Const0041 );
  CoreGroup__OnSetOpacity( &_this->TopList, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->TopList, 36 );
  CoreVerticalList_OnSetNoOfItems( &_this->TopList, 6 );
  CoreVerticalList_OnSetItemClass( &_this->TopList, EW_CLASS( WasherOptItemS ));
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListIn, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListIn, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListIn, 300 );
  CoreRectView__OnSetBounds( &_this->Caption, _Const0042 );
  ViewsText_OnSetAlignment( &_this->Caption, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Caption, EwLoadString( &_Const0043 ));
  ViewsText_OnSetColor( &_this->Caption, WasherCaptionColor );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeCaption, EffectsTimingFastIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeCaption, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeCaption, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListOut, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListOut, 1000 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->FadeListOut, 2000 );
  _this->FadeListOut.Value2 = 0;
  _this->FadeListOut.Value1 = 255;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->Int32Effect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->Int32Effect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->Int32Effect, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->AutoDemoEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->AutoDemoEffect, 300 );
  CoreGroup__Add( _this, ((CoreView)&_this->SlideTouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BottomList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->MainList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TopList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Caption ), 0 );
  _this->SlideTouchHandler.OnEnd = EwNewSlot( _this, WasherOptList_OnEnd );
  _this->SlideTouchHandler.OnStart = EwNewSlot( _this, WasherOptList_OnStart );
  _this->SimpleTouchHandler.OnDrag = EwNewSlot( _this, WasherOptList_OnDrag );
  _this->SimpleTouchHandler.OnRelease = EwNewSlot( _this, WasherOptList_OnRelease );
  _this->SimpleTouchHandler.OnPress = EwNewSlot( _this, WasherOptList_OnStart );
  _this->BottomList.OnLoadItem = EwNewSlot( _this, WasherOptList_OnLoadBottom );
  _this->MainList.OnUpdate = EwNewSlot( _this, WasherOptList_OnUpdate );
  _this->MainList.OnLoadItem = EwNewSlot( _this, WasherOptList_OnLoadMain );
  CoreVerticalList_OnSetSlideHandler( &_this->MainList, &_this->SlideTouchHandler );
  _this->TopList.OnLoadItem = EwNewSlot( _this, WasherOptList_OnLoadTop );
  _this->FadeListIn.Super1.OnAnimate = EwNewSlot( _this, WasherOptList_onAnimate );
  _this->FadeListIn.Outlet = EwNewRef( &_this->TopList, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  ViewsText_OnSetFont( &_this->Caption, EwLoadResource( &WasherDescrFont, ResourcesFont ));
  _this->FadeCaption.Outlet = EwNewRef( &_this->Caption, ViewsText_OnGetOpacity, 
  ViewsText_OnSetOpacity );
  _this->FadeListOut.Super1.OnAnimate = EwNewSlot( _this, WasherOptList_onAnimate );
  _this->FadeListOut.Outlet = EwNewRef( &_this->TopList, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
}

/* Re-Initializer for the class 'Washer::OptList' */
void WasherOptList__ReInit( WasherOptList _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSlideTouchHandler__ReInit( &_this->SlideTouchHandler );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandler );
  ViewsRectangle__ReInit( &_this->Rectangle );
  CoreVerticalList__ReInit( &_this->BottomList );
  CoreVerticalList__ReInit( &_this->MainList );
  CoreVerticalList__ReInit( &_this->TopList );
  EffectsInt32Effect__ReInit( &_this->FadeListIn );
  ViewsText__ReInit( &_this->Caption );
  EffectsInt32Effect__ReInit( &_this->FadeCaption );
  EffectsInt32Effect__ReInit( &_this->FadeListOut );
  EffectsInt32Effect__ReInit( &_this->Int32Effect );
  EffectsInt32Effect__ReInit( &_this->AutoDemoEffect );
}

/* Finalizer method for the class 'Washer::OptList' */
void WasherOptList__Done( WasherOptList _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreSlideTouchHandler__Done( &_this->SlideTouchHandler );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandler );
  ViewsRectangle__Done( &_this->Rectangle );
  CoreVerticalList__Done( &_this->BottomList );
  CoreVerticalList__Done( &_this->MainList );
  CoreVerticalList__Done( &_this->TopList );
  EffectsInt32Effect__Done( &_this->FadeListIn );
  ViewsText__Done( &_this->Caption );
  EffectsInt32Effect__Done( &_this->FadeCaption );
  EffectsInt32Effect__Done( &_this->FadeListOut );
  EffectsInt32Effect__Done( &_this->Int32Effect );
  EffectsInt32Effect__Done( &_this->AutoDemoEffect );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Slot method to load content of main scroll list. */
void WasherOptList_OnLoadMain( WasherOptList _this, XObject sender )
{
  WasherOptItemL itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->MainList.View, WasherOptItemL );
  itemNo = _this->MainList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 6 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->Options[ EwCheckIndex( 
    itemNo, 6 )];

  WasherOptItemL_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->MainList.Super2.Bounds ), _this->MainList.ItemHeight )));
}

/* Slot method to load content of upper scroll list. */
void WasherOptList_OnLoadTop( WasherOptList _this, XObject sender )
{
  WasherOptItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->TopList.View, WasherOptItemS );
  itemNo = _this->TopList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 6 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->Options[ EwCheckIndex( 
    itemNo, 6 )];

  WasherOptItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->TopList.Super2.Bounds ), _this->TopList.ItemHeight )));
}

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void WasherOptList_OnUpdate( WasherOptList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreVerticalList_OnSetScrollOffset( &_this->TopList, (( _this->MainList.ScrollOffset 
  * _this->TopList.ItemHeight ) / _this->MainList.ItemHeight ) + EwGetRectH( _this->TopList.Super2.Bounds ));
  CoreVerticalList_OnSetScrollOffset( &_this->BottomList, (( _this->MainList.ScrollOffset 
  * _this->BottomList.ItemHeight ) / _this->MainList.ItemHeight ) - _this->BottomList.ItemHeight );
}

/* Slot method to load content of lower scroll list. */
void WasherOptList_OnLoadBottom( WasherOptList _this, XObject sender )
{
  WasherOptItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->BottomList.View, WasherOptItemS );
  itemNo = _this->BottomList.Item;
  caption = 0;

  if (( itemNo >= 0 ) && ( itemNo < 6 ))
    caption = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->Options[ EwCheckIndex( 
    itemNo, 6 )];

  WasherOptItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->BottomList.Super2.Bounds ), _this->BottomList.ItemHeight )));
}

/* Slot method to change the selection, when the item has reached its parking position. */
void WasherOptList_OnEnd( WasherOptList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  WasherDeviceClass_OnSetOptionNumber( EwGetAutoObject( &WasherDevice, WasherDeviceClass ), 
  -_this->MainList.ScrollOffset / _this->MainList.ItemHeight );
  _this->FadeListOut.Value1 = _this->TopList.Super1.Opacity;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 255;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandler, 1 );
}

/* Slot method to make the upper/lower list visible. */
void WasherOptList_OnStart( WasherOptList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->TopList.Super1.Opacity != 255 ) && !_this->FadeListIn.Super1.Enabled )
  {
    _this->FadeListIn.Value1 = _this->TopList.Super1.Opacity;
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListIn, 1 );
  }

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 0;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
}

/* This is a slot method. */
void WasherOptList_onAnimate( WasherOptList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup__OnSetOpacity( &_this->BottomList, _this->TopList.Super1.Opacity );
}

/* 'C' function for method : 'Washer::OptList.OnDrag()' */
void WasherOptList_OnDrag( WasherOptList _this, XObject sender )
{
  XPoint delta;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  delta = EwMovePointNeg( _this->SimpleTouchHandler.CurrentPos, _this->SimpleTouchHandler.HittingPos );

  if (( delta.Y > 8 ) || ( delta.Y < -8 ))
  {
    CoreRoot_DeflectCursor( CoreView__GetRoot( _this ), ((CoreView)&_this->SlideTouchHandler ), 
    _Const001E );
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandler, 0 );
  }
}

/* 'C' function for method : 'Washer::OptList.OnRelease()' */
void WasherOptList_OnRelease( WasherOptList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->SimpleTouchHandler.AutoDeflected && ( _this->TopList.Super1.Opacity 
      == 255 ))
  {
    XInt32 delta = 0;

    if ( _this->SimpleTouchHandler.CurrentPos.Y < _this->MainList.Super2.Bounds.Point1.Y )
      delta = (( _this->SimpleTouchHandler.CurrentPos.Y - _this->MainList.Super2.Bounds.Point1.Y ) 
      / _this->TopList.ItemHeight ) - 1;
    else
      if ( _this->SimpleTouchHandler.CurrentPos.Y > _this->MainList.Super2.Bounds.Point2.Y )
        delta = (( _this->SimpleTouchHandler.CurrentPos.Y - _this->MainList.Super2.Bounds.Point2.Y ) 
        / _this->BottomList.ItemHeight ) + 1;

    if ( delta != 0 )
    {
      XInt32 item = ( -_this->MainList.ScrollOffset / _this->MainList.ItemHeight ) 
        + delta;
      CoreVerticalList_EnsureVisible( &_this->MainList, item, 1, &_this->Int32Effect, 
      EwNullSlot );
    }
  }

  EwSignal( EwNewSlot( _this, WasherOptList_OnEnd ), ((XObject)_this ));
}

/* Slot method to trigger the next phase of the auto demo. */
void WasherOptList_AutoDemo( WasherOptList _this, XObject sender )
{
  XInt32 val;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwPostSignal( EwNewSlot( _this, WasherOptList_OnStart ), ((XObject)_this ));
  val = EwGetAutoObject( &WasherDevice, WasherDeviceClass )->OptionNumber + 1;

  if ( val >= 6 )
    val = 0;

  CoreVerticalList_EnsureVisible( &_this->MainList, val, 1, &_this->AutoDemoEffect, 
  EwNewSlot( _this, WasherOptList_OnEnd ));
}

/* Variants derived from the class : 'Washer::OptList' */
EW_DEFINE_CLASS_VARIANTS( WasherOptList )
EW_END_OF_CLASS_VARIANTS( WasherOptList )

/* Virtual Method Table (VMT) for the class : 'Washer::OptList' */
EW_DEFINE_CLASS( WasherOptList, CoreGroup, SlideTouchHandler, SlideTouchHandler, 
                 SlideTouchHandler, SlideTouchHandler, _None, _None, "Washer::OptList" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherOptList )

/* Initializer for the class 'Washer::OptItemL' */
void WasherOptItemL__Init( WasherOptItemL _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherOptItemL );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherOptItemL );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0044 );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0045 );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0026 ));
  ViewsText_OnSetColor( &_this->Text, WasherButtonTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontM, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::OptItemL' */
void WasherOptItemL__ReInit( WasherOptItemL _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::OptItemL' */
void WasherOptItemL__Done( WasherOptItemL _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::OptItemL.OnSetCaption()' */
void WasherOptItemL_OnSetCaption( WasherOptItemL _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Washer::OptItemL' */
EW_DEFINE_CLASS_VARIANTS( WasherOptItemL )
EW_END_OF_CLASS_VARIANTS( WasherOptItemL )

/* Virtual Method Table (VMT) for the class : 'Washer::OptItemL' */
EW_DEFINE_CLASS( WasherOptItemL, CoreGroup, Text, Text, Text, Text, Caption, _None, 
                 "Washer::OptItemL" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherOptItemL )

/* Initializer for the class 'Washer::OptItemS' */
void WasherOptItemS__Init( WasherOptItemS _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherOptItemS );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Back, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Right, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherOptItemS );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0046 );
  CoreView_OnSetLayout((CoreView)&_this->Back, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Back, _Const0046 );
  ViewsRectangle_OnSetColor( &_this->Back, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Right, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Right, _Const0047 );
  ViewsRectangle_OnSetColor( &_this->Right, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0048 );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0026 ));
  ViewsText_OnSetColor( &_this->Text, WasherListTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Back ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Right ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontS, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::OptItemS' */
void WasherOptItemS__ReInit( WasherOptItemS _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Back );
  ViewsRectangle__ReInit( &_this->Right );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::OptItemS' */
void WasherOptItemS__Done( WasherOptItemS _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Back );
  ViewsRectangle__Done( &_this->Right );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::OptItemS.OnSetCaption()' */
void WasherOptItemS_OnSetCaption( WasherOptItemS _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Washer::OptItemS' */
EW_DEFINE_CLASS_VARIANTS( WasherOptItemS )
EW_END_OF_CLASS_VARIANTS( WasherOptItemS )

/* Virtual Method Table (VMT) for the class : 'Washer::OptItemS' */
EW_DEFINE_CLASS( WasherOptItemS, CoreGroup, Back, Back, Back, Back, Caption, _None, 
                 "Washer::OptItemS" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherOptItemS )

/* Initializer for the class 'Washer::Washer' */
void WasherWasher__Init( WasherWasher _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherWasher );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image, &_this->_XObject, 0 );
  ViewsFrame__Init( &_this->Frame, &_this->_XObject, 0 );
  WasherProgList__Init( &_this->ProgList, &_this->_XObject, 0 );
  WasherTempList__Init( &_this->TempList, &_this->_XObject, 0 );
  WasherSpinList__Init( &_this->SpinList, &_this->_XObject, 0 );
  WasherOptList__Init( &_this->OptList, &_this->_XObject, 0 );
  WasherTimeList__Init( &_this->TimeList, &_this->_XObject, 0 );
  ViewsBorder__Init( &_this->Border, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Sep1, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Sep2, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Sep3, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Sep4, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeBgEffect, &_this->_XObject, 0 );
  WasherStartButton__Init( &_this->StartButton, &_this->_XObject, 0 );
  EffectsRectEffect__Init( &_this->MoveStartEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->MessageEffect, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Message, &_this->_XObject, 0 );
  CorePropertyObserver__Init( &_this->ProgramObserver, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->AutoDemoTimer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherWasher );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0049 );
  CoreView_OnSetLayout((CoreView)&_this->Rectangle, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const0049 );
  ViewsRectangle_OnSetColor( &_this->Rectangle, _Const004A );
  CoreRectView__OnSetBounds( &_this->Image, _Const0049 );
  ViewsImage_OnSetColor( &_this->Image, _Const004B );
  CoreRectView__OnSetBounds( &_this->Frame, _Const004C );
  ViewsFrame_OnSetColor( &_this->Frame, _Const004D );
  ViewsFrame_OnSetEdges( &_this->Frame, GraphicsEdgesBottom | GraphicsEdgesLeft 
  | GraphicsEdgesRight | GraphicsEdgesTop );
  CoreRectView__OnSetBounds( &_this->ProgList, _Const004E );
  CoreRectView__OnSetBounds( &_this->TempList, _Const004F );
  CoreRectView__OnSetBounds( &_this->SpinList, _Const0050 );
  CoreRectView__OnSetBounds( &_this->OptList, _Const0051 );
  CoreRectView__OnSetBounds( &_this->TimeList, _Const0052 );
  CoreRectView__OnSetBounds( &_this->Border, _Const0053 );
  ViewsBorder_OnSetWidth( &_this->Border, 2 );
  ViewsBorder_OnSetColor( &_this->Border, _Const0054 );
  CoreRectView__OnSetBounds( &_this->Sep1, _Const0055 );
  ViewsRectangle_OnSetColor( &_this->Sep1, _Const0056 );
  CoreRectView__OnSetBounds( &_this->Sep2, _Const0057 );
  ViewsRectangle_OnSetColor( &_this->Sep2, _Const0056 );
  CoreRectView__OnSetBounds( &_this->Sep3, _Const0058 );
  ViewsRectangle_OnSetColor( &_this->Sep3, _Const0056 );
  CoreRectView__OnSetBounds( &_this->Sep4, _Const0059 );
  ViewsRectangle_OnSetColor( &_this->Sep4, _Const0056 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeBgEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeBgEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeBgEffect, 200 );
  CoreRectView__OnSetBounds( &_this->StartButton, _Const005A );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->MoveStartEffect, EffectsTimingFastIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->MoveStartEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->MoveStartEffect, 200 );
  _this->MoveStartEffect.Value2 = _Const005B;
  _this->MoveStartEffect.Value1 = _Const005A;
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->MessageEffect, 1 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->MessageEffect, 2000 );
  _this->MessageEffect.Value2 = 0;
  _this->MessageEffect.Value1 = 255;
  CoreRectView__OnSetBounds( &_this->Message, _Const005C );
  ViewsText_OnSetAlignment( &_this->Message, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Message, EwLoadString( &_Const005D ));
  ViewsText_OnSetOpacity( &_this->Message, 0 );
  CoreTimer_OnSetPeriod( &_this->AutoDemoTimer, 1500 );
  CoreTimer_OnSetBegin( &_this->AutoDemoTimer, 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Image ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Frame ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->ProgList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TempList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SpinList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->OptList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TimeList ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Border ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Sep1 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Sep2 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Sep3 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Sep4 ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->StartButton ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Message ), 0 );
  ViewsFrame_OnSetBitmap( &_this->Frame, EwLoadResource( &WasherShadow, ResourcesBitmap ));
  _this->TempList.ColorOutlet = EwNewRef( &_this->Border, ViewsBorder_OnGetColor, 
  ViewsBorder_OnSetColor );
  _this->TimeList.OnAppear = EwNewSlot( _this, WasherWasher_OnListAppear );
  _this->TimeList.OnDisappear = EwNewSlot( _this, WasherWasher_OnListDisappear );
  _this->Device = EwGetAutoObject( &WasherDevice, WasherDeviceClass );
  _this->FadeBgEffect.Super1.OnFinished = EwNewSlot( _this, WasherWasher_changeBackground );
  _this->FadeBgEffect.Outlet = EwNewRef( &_this->Image, ViewsImage_OnGetOpacity, 
  ViewsImage_OnSetOpacity );
  _this->StartButton.OnChange = EwNewSlot( _this, WasherWasher_onStart );
  _this->MoveStartEffect.Outlet = EwNewRef( &_this->StartButton, CoreRectView_OnGetBounds, 
  CoreRectView__OnSetBounds );
  _this->MessageEffect.Outlet = EwNewRef( &_this->Message, ViewsText_OnGetOpacity, 
  ViewsText_OnSetOpacity );
  ViewsText_OnSetFont( &_this->Message, EwLoadResource( &WasherDescrFont, ResourcesFont ));
  _this->ProgramObserver.OnEvent = EwNewSlot( _this, WasherWasher_onUpdateProgram );
  CorePropertyObserver_OnSetOutlet( &_this->ProgramObserver, EwNewRef( EwGetAutoObject( 
  &WasherDevice, WasherDeviceClass ), WasherDeviceClass_OnGetProgramNumber, WasherDeviceClass_OnSetProgramNumber ));
  _this->AutoDemoTimer.OnTrigger = EwNewSlot( _this, WasherWasher_AutoDemoSlot );

  /* Call the user defined constructor */
  WasherWasher_Init( _this, aArg );
}

/* Re-Initializer for the class 'Washer::Washer' */
void WasherWasher__ReInit( WasherWasher _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Rectangle );
  ViewsImage__ReInit( &_this->Image );
  ViewsFrame__ReInit( &_this->Frame );
  WasherProgList__ReInit( &_this->ProgList );
  WasherTempList__ReInit( &_this->TempList );
  WasherSpinList__ReInit( &_this->SpinList );
  WasherOptList__ReInit( &_this->OptList );
  WasherTimeList__ReInit( &_this->TimeList );
  ViewsBorder__ReInit( &_this->Border );
  ViewsRectangle__ReInit( &_this->Sep1 );
  ViewsRectangle__ReInit( &_this->Sep2 );
  ViewsRectangle__ReInit( &_this->Sep3 );
  ViewsRectangle__ReInit( &_this->Sep4 );
  EffectsInt32Effect__ReInit( &_this->FadeBgEffect );
  WasherStartButton__ReInit( &_this->StartButton );
  EffectsRectEffect__ReInit( &_this->MoveStartEffect );
  EffectsInt32Effect__ReInit( &_this->MessageEffect );
  ViewsText__ReInit( &_this->Message );
  CorePropertyObserver__ReInit( &_this->ProgramObserver );
  CoreTimer__ReInit( &_this->AutoDemoTimer );
}

/* Finalizer method for the class 'Washer::Washer' */
void WasherWasher__Done( WasherWasher _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Rectangle );
  ViewsImage__Done( &_this->Image );
  ViewsFrame__Done( &_this->Frame );
  WasherProgList__Done( &_this->ProgList );
  WasherTempList__Done( &_this->TempList );
  WasherSpinList__Done( &_this->SpinList );
  WasherOptList__Done( &_this->OptList );
  WasherTimeList__Done( &_this->TimeList );
  ViewsBorder__Done( &_this->Border );
  ViewsRectangle__Done( &_this->Sep1 );
  ViewsRectangle__Done( &_this->Sep2 );
  ViewsRectangle__Done( &_this->Sep3 );
  ViewsRectangle__Done( &_this->Sep4 );
  EffectsInt32Effect__Done( &_this->FadeBgEffect );
  WasherStartButton__Done( &_this->StartButton );
  EffectsRectEffect__Done( &_this->MoveStartEffect );
  EffectsInt32Effect__Done( &_this->MessageEffect );
  ViewsText__Done( &_this->Message );
  CorePropertyObserver__Done( &_this->ProgramObserver );
  CoreTimer__Done( &_this->AutoDemoTimer );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void WasherWasher_Init( WasherWasher _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  EwPostSignal( EwNewSlot( _this, WasherWasher_onUpdateProgram ), ((XObject)_this ));
}

/* Slot method to update the display value. */
void WasherWasher_onUpdateProgram( WasherWasher _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeBgEffect, 0 );
  _this->FadeBgEffect.Value1 = _this->Image.Opacity;
  _this->FadeBgEffect.Value2 = 0;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeBgEffect, 1 );
}

/* This is a slot method. */
void WasherWasher_changeBackground( WasherWasher _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Image.Opacity == 0 )
  {
    switch ( _this->Device->ProgramNumber )
    {
      case 0 :
        ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &WasherBgHot, ResourcesBitmap ));
      break;

      case 1 :
        ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &WasherBgEasyCare, 
        ResourcesBitmap ));
      break;

      case 2 :
        ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &WasherBgDelicates, 
        ResourcesBitmap ));
      break;

      case 3 :
        ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &WasherBgSportswear, 
        ResourcesBitmap ));
      break;

      case 4 :
        ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &WasherBgWool, ResourcesBitmap ));
      break;

      case 5 :
        ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &WasherBgEco, ResourcesBitmap ));
      break;

      case 6 :
        ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &WasherBgDrain, ResourcesBitmap ));
      break;

      case 7 :
        ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &WasherBgSpin, ResourcesBitmap ));
      break;

      default : 
        ViewsImage_OnSetBitmap( &_this->Image, 0 );
    }

    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeBgEffect, 0 );
    _this->FadeBgEffect.Value1 = _this->Image.Opacity;
    _this->FadeBgEffect.Value2 = 255;
    EwPostSignal( EwNewSlot( &_this->FadeBgEffect, EffectsEffect_StartEffect ), 
      ((XObject)_this ));
  }
}

/* Slot to receive signal that time list will appear. */
void WasherWasher_OnListAppear( WasherWasher _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EffectsEffect_OnSetReversed((EffectsEffect)&_this->MoveStartEffect, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->MoveStartEffect, 1 );
}

/* Slot to receive signal that time list has disappeared. */
void WasherWasher_OnListDisappear( WasherWasher _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EffectsEffect_OnSetReversed((EffectsEffect)&_this->MoveStartEffect, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->MoveStartEffect, 1 );
}

/* 'C' function for method : 'Washer::Washer.onStart()' */
void WasherWasher_onStart( WasherWasher _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->MessageEffect, 0 );
  ViewsText_OnSetOpacity( &_this->Message, 255 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->MessageEffect, 1 );
}

/* Slot method to trigger the next phase of the auto demo. */
void WasherWasher_AutoDemoSlot( WasherWasher _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  switch ( _this->AutoState )
  {
    case 1 :
      EwPostSignal( EwNewSlot( &_this->ProgList, WasherProgList_AutoDemo ), ((XObject)_this ));
    break;

    case 2 :
      EwPostSignal( EwNewSlot( &_this->TempList, WasherTempList_AutoDemo ), ((XObject)_this ));
    break;

    case 3 :
      EwPostSignal( EwNewSlot( &_this->SpinList, WasherSpinList_AutoDemo ), ((XObject)_this ));
    break;

    case 4 :
      EwPostSignal( EwNewSlot( &_this->OptList, WasherOptList_AutoDemo ), ((XObject)_this ));
    break;

    case 5 :
    {
      EwPostSignal( EwNewSlot( &_this->TempList, WasherTempList_AutoDemo ), ((XObject)_this ));
      EwPostSignal( EwNewSlot( &_this->SpinList, WasherSpinList_AutoDemo ), ((XObject)_this ));
    }
    break;

    case 6 :
    {
      EwPostSignal( EwNewSlot( &_this->ProgList, WasherProgList_AutoDemo ), ((XObject)_this ));
      EwPostSignal( EwNewSlot( &_this->TempList, WasherTempList_AutoDemo ), ((XObject)_this ));
    }
    break;

    case 7 :
    {
      EwPostSignal( EwNewSlot( &_this->ProgList, WasherProgList_AutoDemo ), ((XObject)_this ));
      EwPostSignal( EwNewSlot( &_this->SpinList, WasherSpinList_AutoDemo ), ((XObject)_this ));
      EwPostSignal( EwNewSlot( &_this->OptList, WasherOptList_AutoDemo ), ((XObject)_this ));
    }
    break;

    default : 
      ;
  }

  _this->AutoState = _this->AutoState + 1;

  if ( _this->AutoState > 7 )
    _this->AutoState = 0;
}

/* Variants derived from the class : 'Washer::Washer' */
EW_DEFINE_CLASS_VARIANTS( WasherWasher )
EW_END_OF_CLASS_VARIANTS( WasherWasher )

/* Virtual Method Table (VMT) for the class : 'Washer::Washer' */
EW_DEFINE_CLASS( WasherWasher, CoreGroup, Device, Rectangle, Rectangle, Rectangle, 
                 AutoState, AutoState, "Washer::Washer" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherWasher )

/* Initializer for the class 'Washer::HourItemS' */
void WasherHourItemS__Init( WasherHourItemS _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherHourItemS );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Back, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Center, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherHourItemS );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const005E );
  CoreView_OnSetLayout((CoreView)&_this->Back, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Back, _Const005E );
  ViewsRectangle_OnSetColor( &_this->Back, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Center, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Center, _Const005F );
  ViewsRectangle_OnSetColor( &_this->Center, WasherButtonTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const005E );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0026 ));
  ViewsText_OnSetColor( &_this->Text, WasherListTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Back ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Center ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontS, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::HourItemS' */
void WasherHourItemS__ReInit( WasherHourItemS _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Back );
  ViewsRectangle__ReInit( &_this->Center );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::HourItemS' */
void WasherHourItemS__Done( WasherHourItemS _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Back );
  ViewsRectangle__Done( &_this->Center );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::HourItemS.OnSetCaption()' */
void WasherHourItemS_OnSetCaption( WasherHourItemS _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Washer::HourItemS' */
EW_DEFINE_CLASS_VARIANTS( WasherHourItemS )
EW_END_OF_CLASS_VARIANTS( WasherHourItemS )

/* Virtual Method Table (VMT) for the class : 'Washer::HourItemS' */
EW_DEFINE_CLASS( WasherHourItemS, CoreGroup, Back, Back, Back, Back, Caption, _None, 
                 "Washer::HourItemS" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherHourItemS )

/* Initializer for the class 'Washer::TimeList' */
void WasherTimeList__Init( WasherTimeList _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherTimeList );

  /* ... then construct all embedded objects */
  CoreSlideTouchHandler__Init( &_this->SlideTouchHandlerH, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerH, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->BottomListH, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->MainListH, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->TopListH, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListIn, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Caption, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeCaption, &_this->_XObject, 0 );
  CoreSlideTouchHandler__Init( &_this->SlideTouchHandlerM, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerM, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->BottomListM, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->MainListM, &_this->_XObject, 0 );
  CoreVerticalList__Init( &_this->TopListM, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );
  ViewsText__Init( &_this->DurationText, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeDuration, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeListOut, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->Int32EffectH, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->Int32EffectM, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherTimeList );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0060 );
  CoreRectView__OnSetBounds( &_this->SlideTouchHandlerH, _Const0061 );
  CoreSlideTouchHandler_OnSetSnapNext( &_this->SlideTouchHandlerH, _Const001A );
  CoreSlideTouchHandler_OnSetFriction( &_this->SlideTouchHandlerH, 0.100000f );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandlerH, _Const0062 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandlerH, _Const0063 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandlerH, _Const0064 );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandlerH, _Const001E );
  CoreView_OnSetLayout((CoreView)&_this->Rectangle, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const001F );
  ViewsRectangle_OnSetColor( &_this->Rectangle, WasherButtonBgColor );
  CoreRectView__OnSetBounds( &_this->BottomListH, _Const0065 );
  CoreGroup__OnSetOpacity( &_this->BottomListH, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->BottomListH, 24 );
  CoreVerticalList_OnSetNoOfItems( &_this->BottomListH, 24 );
  CoreVerticalList_OnSetItemClass( &_this->BottomListH, EW_CLASS( WasherHourItemS ));
  CoreRectView__OnSetBounds( &_this->MainListH, _Const0066 );
  CoreVerticalList_OnSetItemHeight( &_this->MainListH, 96 );
  CoreVerticalList_OnSetNoOfItems( &_this->MainListH, 24 );
  CoreVerticalList_OnSetItemClass( &_this->MainListH, EW_CLASS( WasherHourItemL ));
  CoreRectView__OnSetBounds( &_this->TopListH, _Const0067 );
  CoreGroup__OnSetOpacity( &_this->TopListH, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->TopListH, 24 );
  CoreVerticalList_OnSetNoOfItems( &_this->TopListH, 24 );
  CoreVerticalList_OnSetItemClass( &_this->TopListH, EW_CLASS( WasherHourItemS ));
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListIn, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListIn, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListIn, 300 );
  CoreRectView__OnSetBounds( &_this->Caption, _Const0068 );
  ViewsText_OnSetAlignment( &_this->Caption, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Caption, EwLoadString( &_Const0069 ));
  ViewsText_OnSetColor( &_this->Caption, WasherCaptionColor );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeCaption, EffectsTimingFastIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeCaption, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeCaption, 200 );
  CoreRectView__OnSetBounds( &_this->SlideTouchHandlerM, _Const006A );
  CoreSlideTouchHandler_OnSetSnapNext( &_this->SlideTouchHandlerM, _Const001A );
  CoreSlideTouchHandler_OnSetFriction( &_this->SlideTouchHandlerM, 0.100000f );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandlerM, _Const0063 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandlerM, _Const006B );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandlerM, _Const001D );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandlerM, _Const0064 );
  CoreRectView__OnSetBounds( &_this->BottomListM, _Const006C );
  CoreGroup__OnSetOpacity( &_this->BottomListM, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->BottomListM, 24 );
  CoreVerticalList_OnSetNoOfItems( &_this->BottomListM, 60 );
  CoreVerticalList_OnSetItemClass( &_this->BottomListM, EW_CLASS( WasherMinItemS ));
  CoreRectView__OnSetBounds( &_this->MainListM, _Const006D );
  CoreVerticalList_OnSetItemHeight( &_this->MainListM, 96 );
  CoreVerticalList_OnSetNoOfItems( &_this->MainListM, 60 );
  CoreVerticalList_OnSetItemClass( &_this->MainListM, EW_CLASS( WasherMinItemL ));
  CoreRectView__OnSetBounds( &_this->TopListM, _Const006E );
  CoreGroup__OnSetOpacity( &_this->TopListM, 0 );
  CoreVerticalList_OnSetItemHeight( &_this->TopListM, 24 );
  CoreVerticalList_OnSetNoOfItems( &_this->TopListM, 60 );
  CoreVerticalList_OnSetItemClass( &_this->TopListM, EW_CLASS( WasherMinItemS ));
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const006F );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0070 ));
  ViewsText_OnSetColor( &_this->Text, WasherButtonTextColor );
  CoreRectView__OnSetBounds( &_this->DurationText, _Const0071 );
  ViewsText_OnSetAlignment( &_this->DurationText, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->DurationText, EwLoadString( &_Const0072 ));
  ViewsText_OnSetColor( &_this->DurationText, WasherCaptionColor );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeDuration, EffectsTimingFastIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeDuration, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeDuration, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeListOut, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeListOut, 1000 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->FadeListOut, 2000 );
  _this->FadeListOut.Value2 = 0;
  _this->FadeListOut.Value1 = 255;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->Int32EffectH, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->Int32EffectH, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->Int32EffectH, 200 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->Int32EffectM, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->Int32EffectM, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->Int32EffectM, 200 );
  CoreGroup__Add( _this, ((CoreView)&_this->SlideTouchHandlerH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandlerH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BottomListH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->MainListH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TopListH ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Caption ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SlideTouchHandlerM ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->SimpleTouchHandlerM ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->BottomListM ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->MainListM ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TopListM ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->DurationText ), 0 );
  _this->SlideTouchHandlerH.OnEnd = EwNewSlot( _this, WasherTimeList_OnEnd );
  _this->SlideTouchHandlerH.OnStart = EwNewSlot( _this, WasherTimeList_OnStart );
  _this->SimpleTouchHandlerH.OnDrag = EwNewSlot( _this, WasherTimeList_OnDragH );
  _this->SimpleTouchHandlerH.OnRelease = EwNewSlot( _this, WasherTimeList_OnReleaseH );
  _this->SimpleTouchHandlerH.OnPress = EwNewSlot( _this, WasherTimeList_OnStart );
  _this->BottomListH.OnLoadItem = EwNewSlot( _this, WasherTimeList_OnLoadBottomH );
  _this->MainListH.OnUpdate = EwNewSlot( _this, WasherTimeList_OnUpdateH );
  _this->MainListH.OnLoadItem = EwNewSlot( _this, WasherTimeList_OnLoadMainH );
  CoreVerticalList_OnSetSlideHandler( &_this->MainListH, &_this->SlideTouchHandlerH );
  _this->TopListH.OnLoadItem = EwNewSlot( _this, WasherTimeList_OnLoadTopH );
  _this->FadeListIn.Super1.OnAnimate = EwNewSlot( _this, WasherTimeList_onAnimate );
  _this->FadeListIn.Outlet = EwNewRef( &_this->TopListH, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  ViewsText_OnSetFont( &_this->Caption, EwLoadResource( &WasherDescrFont, ResourcesFont ));
  _this->FadeCaption.Outlet = EwNewRef( &_this->Caption, ViewsText_OnGetOpacity, 
  ViewsText_OnSetOpacity );
  _this->SlideTouchHandlerM.OnEnd = EwNewSlot( _this, WasherTimeList_OnEnd );
  _this->SlideTouchHandlerM.OnStart = EwNewSlot( _this, WasherTimeList_OnStart );
  _this->SimpleTouchHandlerM.OnDrag = EwNewSlot( _this, WasherTimeList_OnDragM );
  _this->SimpleTouchHandlerM.OnRelease = EwNewSlot( _this, WasherTimeList_OnReleaseM );
  _this->SimpleTouchHandlerM.OnPress = EwNewSlot( _this, WasherTimeList_OnStart );
  _this->BottomListM.OnLoadItem = EwNewSlot( _this, WasherTimeList_OnLoadBottomM );
  _this->MainListM.OnUpdate = EwNewSlot( _this, WasherTimeList_OnUpdateM );
  _this->MainListM.OnLoadItem = EwNewSlot( _this, WasherTimeList_OnLoadMainM );
  CoreVerticalList_OnSetSlideHandler( &_this->MainListM, &_this->SlideTouchHandlerM );
  _this->TopListM.OnLoadItem = EwNewSlot( _this, WasherTimeList_OnLoadTopM );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontXL, ResourcesFont ));
  ViewsText_OnSetFont( &_this->DurationText, EwLoadResource( &WasherDescrFont, ResourcesFont ));
  _this->FadeDuration.Outlet = EwNewRef( &_this->DurationText, ViewsText_OnGetOpacity, 
  ViewsText_OnSetOpacity );
  _this->FadeListOut.Super1.OnFinished = EwNewSlot( _this, WasherTimeList_onFinished );
  _this->FadeListOut.Super1.OnAnimate = EwNewSlot( _this, WasherTimeList_onAnimate );
  _this->FadeListOut.Outlet = EwNewRef( &_this->TopListH, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
}

/* Re-Initializer for the class 'Washer::TimeList' */
void WasherTimeList__ReInit( WasherTimeList _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreSlideTouchHandler__ReInit( &_this->SlideTouchHandlerH );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerH );
  ViewsRectangle__ReInit( &_this->Rectangle );
  CoreVerticalList__ReInit( &_this->BottomListH );
  CoreVerticalList__ReInit( &_this->MainListH );
  CoreVerticalList__ReInit( &_this->TopListH );
  EffectsInt32Effect__ReInit( &_this->FadeListIn );
  ViewsText__ReInit( &_this->Caption );
  EffectsInt32Effect__ReInit( &_this->FadeCaption );
  CoreSlideTouchHandler__ReInit( &_this->SlideTouchHandlerM );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerM );
  CoreVerticalList__ReInit( &_this->BottomListM );
  CoreVerticalList__ReInit( &_this->MainListM );
  CoreVerticalList__ReInit( &_this->TopListM );
  ViewsText__ReInit( &_this->Text );
  ViewsText__ReInit( &_this->DurationText );
  EffectsInt32Effect__ReInit( &_this->FadeDuration );
  EffectsInt32Effect__ReInit( &_this->FadeListOut );
  EffectsInt32Effect__ReInit( &_this->Int32EffectH );
  EffectsInt32Effect__ReInit( &_this->Int32EffectM );
}

/* Finalizer method for the class 'Washer::TimeList' */
void WasherTimeList__Done( WasherTimeList _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreSlideTouchHandler__Done( &_this->SlideTouchHandlerH );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerH );
  ViewsRectangle__Done( &_this->Rectangle );
  CoreVerticalList__Done( &_this->BottomListH );
  CoreVerticalList__Done( &_this->MainListH );
  CoreVerticalList__Done( &_this->TopListH );
  EffectsInt32Effect__Done( &_this->FadeListIn );
  ViewsText__Done( &_this->Caption );
  EffectsInt32Effect__Done( &_this->FadeCaption );
  CoreSlideTouchHandler__Done( &_this->SlideTouchHandlerM );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerM );
  CoreVerticalList__Done( &_this->BottomListM );
  CoreVerticalList__Done( &_this->MainListM );
  CoreVerticalList__Done( &_this->TopListM );
  ViewsText__Done( &_this->Text );
  ViewsText__Done( &_this->DurationText );
  EffectsInt32Effect__Done( &_this->FadeDuration );
  EffectsInt32Effect__Done( &_this->FadeListOut );
  EffectsInt32Effect__Done( &_this->Int32EffectH );
  EffectsInt32Effect__Done( &_this->Int32EffectM );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* Slot method to load content of main scroll list. */
void WasherTimeList_OnLoadMainH( WasherTimeList _this, XObject sender )
{
  WasherHourItemL itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->MainListH.View, WasherHourItemL );
  itemNo = _this->MainListH.Item;
  caption = EwNewStringInt( itemNo, 0, 10 );
  WasherHourItemL_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->MainListH.Super2.Bounds ), _this->MainListH.ItemHeight )));
}

/* Slot method to load content of upper scroll list. */
void WasherTimeList_OnLoadTopH( WasherTimeList _this, XObject sender )
{
  WasherHourItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->TopListH.View, WasherHourItemS );
  itemNo = _this->TopListH.Item;
  caption = EwNewStringInt( itemNo, 0, 10 );
  WasherHourItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->TopListH.Super2.Bounds ), _this->TopListH.ItemHeight )));
}

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void WasherTimeList_OnUpdateH( WasherTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreVerticalList_OnSetScrollOffset( &_this->TopListH, (( _this->MainListH.ScrollOffset 
  * _this->TopListH.ItemHeight ) / _this->MainListH.ItemHeight ) + EwGetRectH( _this->TopListH.Super2.Bounds ));
  CoreVerticalList_OnSetScrollOffset( &_this->BottomListH, (( _this->MainListH.ScrollOffset 
  * _this->BottomListH.ItemHeight ) / _this->MainListH.ItemHeight ) - _this->BottomListH.ItemHeight );
}

/* Slot method to load content of lower scroll list. */
void WasherTimeList_OnLoadBottomH( WasherTimeList _this, XObject sender )
{
  WasherHourItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->BottomListH.View, WasherHourItemS );
  itemNo = _this->BottomListH.Item;
  caption = EwNewStringInt( itemNo, 0, 10 );
  WasherHourItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->BottomListH.Super2.Bounds ), _this->BottomListH.ItemHeight )));
}

/* Slot method to change the selection, when the item has reached its parking position. */
void WasherTimeList_OnEnd( WasherTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  WasherDeviceClass_OnSetHour( EwGetAutoObject( &WasherDevice, WasherDeviceClass ), 
  -_this->MainListH.ScrollOffset / _this->MainListH.ItemHeight );
  WasherDeviceClass_OnSetMinute( EwGetAutoObject( &WasherDevice, WasherDeviceClass ), 
  -_this->MainListM.ScrollOffset / _this->MainListM.ItemHeight );
  _this->FadeListOut.Value1 = _this->TopListH.Super1.Opacity;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 255;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerH, 1 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerM, 1 );
}

/* Slot method to make the upper/lower list visible. */
void WasherTimeList_OnStart( WasherTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->TopListH.Super1.Opacity != 255 ) && !_this->FadeListIn.Super1.Enabled )
  {
    _this->FadeListIn.Value1 = _this->TopListH.Super1.Opacity;
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListIn, 1 );
  }

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeListOut, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 0 );
  _this->FadeCaption.Value1 = _this->Caption.Opacity;
  _this->FadeCaption.Value2 = 0;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeCaption, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeDuration, 0 );
  _this->FadeDuration.Value1 = _this->DurationText.Opacity;
  _this->FadeDuration.Value2 = 0;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeDuration, 1 );

  if ( _this->TopListH.Super1.Opacity == 0 )
    EwPostSignal( _this->OnAppear, ((XObject)_this ));
}

/* This is a slot method. */
void WasherTimeList_onAnimate( WasherTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup__OnSetOpacity( &_this->TopListM, _this->TopListH.Super1.Opacity );
  CoreGroup__OnSetOpacity( &_this->BottomListH, _this->TopListH.Super1.Opacity );
  CoreGroup__OnSetOpacity( &_this->BottomListM, _this->TopListH.Super1.Opacity );
}

/* Slot method to load content of main scroll list. */
void WasherTimeList_OnLoadMainM( WasherTimeList _this, XObject sender )
{
  WasherMinItemL itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->MainListM.View, WasherMinItemL );
  itemNo = _this->MainListM.Item;
  caption = EwNewStringInt( itemNo, 2, 10 );
  WasherMinItemL_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->MainListM.Super2.Bounds ), _this->MainListM.ItemHeight )));
}

/* Slot method to load content of upper scroll list. */
void WasherTimeList_OnLoadTopM( WasherTimeList _this, XObject sender )
{
  WasherMinItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->TopListM.View, WasherMinItemS );
  itemNo = _this->TopListM.Item;
  caption = EwNewStringInt( itemNo, 2, 10 );
  WasherMinItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->TopListM.Super2.Bounds ), _this->TopListM.ItemHeight )));
}

/* Slot method to load content of lower scroll list. */
void WasherTimeList_OnLoadBottomM( WasherTimeList _this, XObject sender )
{
  WasherMinItemS itemView;
  XInt32 itemNo;
  XString caption;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  itemView = EwCastObject( _this->BottomListM.View, WasherMinItemS );
  itemNo = _this->BottomListM.Item;
  caption = EwNewStringInt( itemNo, 2, 10 );
  WasherMinItemS_OnSetCaption( itemView, caption );
  CoreRectView__OnSetBounds( itemView, EwSetRectSize( itemView->Super2.Bounds, EwNewPoint( 
  EwGetRectW( _this->BottomListM.Super2.Bounds ), _this->BottomListM.ItemHeight )));
}

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void WasherTimeList_OnUpdateM( WasherTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreVerticalList_OnSetScrollOffset( &_this->TopListM, (( _this->MainListM.ScrollOffset 
  * _this->TopListM.ItemHeight ) / _this->MainListM.ItemHeight ) + EwGetRectH( _this->TopListM.Super2.Bounds ));
  CoreVerticalList_OnSetScrollOffset( &_this->BottomListM, (( _this->MainListM.ScrollOffset 
  * _this->BottomListM.ItemHeight ) / _this->MainListM.ItemHeight ) - _this->BottomListM.ItemHeight );
}

/* This is a slot method. */
void WasherTimeList_onFinished( WasherTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->FadeListOut.Value == 0 )
  {
    EwPostSignal( _this->OnDisappear, ((XObject)_this ));
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeDuration, 0 );
    _this->FadeDuration.Value1 = _this->Caption.Opacity;
    _this->FadeDuration.Value2 = 255;
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeDuration, 1 );
  }
}

/* 'C' function for method : 'Washer::TimeList.OnDragH()' */
void WasherTimeList_OnDragH( WasherTimeList _this, XObject sender )
{
  XPoint delta;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  delta = EwMovePointNeg( _this->SimpleTouchHandlerH.CurrentPos, _this->SimpleTouchHandlerH.HittingPos );

  if (( delta.Y > 8 ) || ( delta.Y < -8 ))
  {
    CoreRoot_DeflectCursor( CoreView__GetRoot( _this ), ((CoreView)&_this->SlideTouchHandlerH ), 
    _Const001E );
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerH, 0 );
  }
}

/* 'C' function for method : 'Washer::TimeList.OnReleaseH()' */
void WasherTimeList_OnReleaseH( WasherTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->SimpleTouchHandlerH.AutoDeflected && ( _this->TopListH.Super1.Opacity 
      == 255 ))
  {
    XInt32 delta = 0;

    if ( _this->SimpleTouchHandlerH.CurrentPos.Y < _this->MainListH.Super2.Bounds.Point1.Y )
      delta = (( _this->SimpleTouchHandlerH.CurrentPos.Y - _this->MainListH.Super2.Bounds.Point1.Y ) 
      / _this->TopListH.ItemHeight ) - 1;
    else
      if ( _this->SimpleTouchHandlerH.CurrentPos.Y > _this->MainListH.Super2.Bounds.Point2.Y )
        delta = (( _this->SimpleTouchHandlerH.CurrentPos.Y - _this->MainListH.Super2.Bounds.Point2.Y ) 
        / _this->BottomListH.ItemHeight ) + 1;

    if ( delta != 0 )
    {
      XInt32 item = ( -_this->MainListH.ScrollOffset / _this->MainListH.ItemHeight ) 
        + delta;
      CoreVerticalList_EnsureVisible( &_this->MainListH, item, 1, &_this->Int32EffectH, 
      EwNullSlot );
    }
  }

  EwSignal( EwNewSlot( _this, WasherTimeList_OnEnd ), ((XObject)_this ));
}

/* 'C' function for method : 'Washer::TimeList.OnDragM()' */
void WasherTimeList_OnDragM( WasherTimeList _this, XObject sender )
{
  XPoint delta;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  delta = EwMovePointNeg( _this->SimpleTouchHandlerM.CurrentPos, _this->SimpleTouchHandlerM.HittingPos );

  if (( delta.Y > 8 ) || ( delta.Y < -8 ))
  {
    CoreRoot_DeflectCursor( CoreView__GetRoot( _this ), ((CoreView)&_this->SlideTouchHandlerM ), 
    _Const001E );
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerM, 0 );
  }
}

/* 'C' function for method : 'Washer::TimeList.OnReleaseM()' */
void WasherTimeList_OnReleaseM( WasherTimeList _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->SimpleTouchHandlerM.AutoDeflected && ( _this->TopListM.Super1.Opacity 
      == 255 ))
  {
    XInt32 delta = 0;

    if ( _this->SimpleTouchHandlerM.CurrentPos.Y < _this->MainListM.Super2.Bounds.Point1.Y )
      delta = (( _this->SimpleTouchHandlerM.CurrentPos.Y - _this->MainListM.Super2.Bounds.Point1.Y ) 
      / _this->TopListM.ItemHeight ) - 1;
    else
      if ( _this->SimpleTouchHandlerM.CurrentPos.Y > _this->MainListM.Super2.Bounds.Point2.Y )
        delta = (( _this->SimpleTouchHandlerM.CurrentPos.Y - _this->MainListM.Super2.Bounds.Point2.Y ) 
        / _this->BottomListM.ItemHeight ) + 1;

    if ( delta != 0 )
    {
      XInt32 item = ( -_this->MainListM.ScrollOffset / _this->MainListM.ItemHeight ) 
        + delta;
      CoreVerticalList_EnsureVisible( &_this->MainListM, item, 1, &_this->Int32EffectM, 
      EwNullSlot );
    }
  }

  EwSignal( EwNewSlot( _this, WasherTimeList_OnEnd ), ((XObject)_this ));
}

/* Variants derived from the class : 'Washer::TimeList' */
EW_DEFINE_CLASS_VARIANTS( WasherTimeList )
EW_END_OF_CLASS_VARIANTS( WasherTimeList )

/* Virtual Method Table (VMT) for the class : 'Washer::TimeList' */
EW_DEFINE_CLASS( WasherTimeList, CoreGroup, OnAppear, OnAppear, SlideTouchHandlerH, 
                 SlideTouchHandlerH, _None, _None, "Washer::TimeList" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherTimeList )

/* Initializer for the class 'Washer::HourItemL' */
void WasherHourItemL__Init( WasherHourItemL _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherHourItemL );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherHourItemL );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0073 );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0074 );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzRight | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0075 ));
  ViewsText_OnSetColor( &_this->Text, WasherButtonTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontXL, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::HourItemL' */
void WasherHourItemL__ReInit( WasherHourItemL _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::HourItemL' */
void WasherHourItemL__Done( WasherHourItemL _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::HourItemL.OnSetCaption()' */
void WasherHourItemL_OnSetCaption( WasherHourItemL _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Washer::HourItemL' */
EW_DEFINE_CLASS_VARIANTS( WasherHourItemL )
EW_END_OF_CLASS_VARIANTS( WasherHourItemL )

/* Virtual Method Table (VMT) for the class : 'Washer::HourItemL' */
EW_DEFINE_CLASS( WasherHourItemL, CoreGroup, Text, Text, Text, Text, Caption, _None, 
                 "Washer::HourItemL" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherHourItemL )

/* Initializer for the class 'Washer::MinItemL' */
void WasherMinItemL__Init( WasherMinItemL _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherMinItemL );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherMinItemL );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0073 );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const0076 );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0075 ));
  ViewsText_OnSetColor( &_this->Text, WasherButtonTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontXL, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::MinItemL' */
void WasherMinItemL__ReInit( WasherMinItemL _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::MinItemL' */
void WasherMinItemL__Done( WasherMinItemL _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::MinItemL.OnSetCaption()' */
void WasherMinItemL_OnSetCaption( WasherMinItemL _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Washer::MinItemL' */
EW_DEFINE_CLASS_VARIANTS( WasherMinItemL )
EW_END_OF_CLASS_VARIANTS( WasherMinItemL )

/* Virtual Method Table (VMT) for the class : 'Washer::MinItemL' */
EW_DEFINE_CLASS( WasherMinItemL, CoreGroup, Text, Text, Text, Text, Caption, _None, 
                 "Washer::MinItemL" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherMinItemL )

/* Initializer for the class 'Washer::MinItemS' */
void WasherMinItemS__Init( WasherMinItemS _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherMinItemS );

  /* ... then construct all embedded objects */
  ViewsRectangle__Init( &_this->Back, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Center, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Right, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherMinItemS );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const005E );
  CoreView_OnSetLayout((CoreView)&_this->Back, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Back, _Const005E );
  ViewsRectangle_OnSetColor( &_this->Back, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Center, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Center, _Const0077 );
  ViewsRectangle_OnSetColor( &_this->Center, WasherButtonTextColor );
  CoreView_OnSetLayout((CoreView)&_this->Right, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Right, _Const0078 );
  ViewsRectangle_OnSetColor( &_this->Right, WasherListBgColor );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const005E );
  ViewsText_OnSetWrapText( &_this->Text, 1 );
  ViewsText_OnSetAlignment( &_this->Text, ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const0026 ));
  ViewsText_OnSetColor( &_this->Text, WasherListTextColor );
  CoreGroup__Add( _this, ((CoreView)&_this->Back ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Center ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Right ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontS, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::MinItemS' */
void WasherMinItemS__ReInit( WasherMinItemS _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsRectangle__ReInit( &_this->Back );
  ViewsRectangle__ReInit( &_this->Center );
  ViewsRectangle__ReInit( &_this->Right );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::MinItemS' */
void WasherMinItemS__Done( WasherMinItemS _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsRectangle__Done( &_this->Back );
  ViewsRectangle__Done( &_this->Center );
  ViewsRectangle__Done( &_this->Right );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Washer::MinItemS.OnSetCaption()' */
void WasherMinItemS_OnSetCaption( WasherMinItemS _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->Text, value );
}

/* Variants derived from the class : 'Washer::MinItemS' */
EW_DEFINE_CLASS_VARIANTS( WasherMinItemS )
EW_END_OF_CLASS_VARIANTS( WasherMinItemS )

/* Virtual Method Table (VMT) for the class : 'Washer::MinItemS' */
EW_DEFINE_CLASS( WasherMinItemS, CoreGroup, Back, Back, Back, Back, Caption, _None, 
                 "Washer::MinItemS" )
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
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherMinItemS )

/* Include a file containing the bitmap resource : 'Washer::BgHot' */
#include "_WasherBgHot.h"

/* Table with links to derived variants of the bitmap resource : 'Washer::BgHot' */
EW_RES_WITHOUT_VARIANTS( WasherBgHot )

/* Include a file containing the bitmap resource : 'Washer::BgDelicates' */
#include "_WasherBgDelicates.h"

/* Table with links to derived variants of the bitmap resource : 'Washer::BgDelicates' */
EW_RES_WITHOUT_VARIANTS( WasherBgDelicates )

/* Include a file containing the bitmap resource : 'Washer::BgSportswear' */
#include "_WasherBgSportswear.h"

/* Table with links to derived variants of the bitmap resource : 'Washer::BgSportswear' */
EW_RES_WITHOUT_VARIANTS( WasherBgSportswear )

/* Include a file containing the bitmap resource : 'Washer::BgWool' */
#include "_WasherBgWool.h"

/* Table with links to derived variants of the bitmap resource : 'Washer::BgWool' */
EW_RES_WITHOUT_VARIANTS( WasherBgWool )

/* Include a file containing the bitmap resource : 'Washer::BgEco' */
#include "_WasherBgEco.h"

/* Table with links to derived variants of the bitmap resource : 'Washer::BgEco' */
EW_RES_WITHOUT_VARIANTS( WasherBgEco )

/* Color constant according UI design. */
const XColor WasherTempColor0 = { 0x2A, 0x73, 0x94, 0xFF };

/* Color constant according UI design. */
const XColor WasherTempColor2 = { 0xBC, 0xE4, 0x3F, 0xFF };

/* Color constant according UI design. */
const XColor WasherTempColor3 = { 0xFD, 0xFD, 0x34, 0xFF };

/* Color constant according UI design. */
const XColor WasherTempColor4 = { 0xFE, 0xC1, 0x2B, 0xFF };

/* Color constant according UI design. */
const XColor WasherTempColor5 = { 0xFD, 0x55, 0x21, 0xFF };

/* Color constant according UI design. */
const XColor WasherTempColor6 = { 0xFD, 0x1A, 0x1F, 0xFF };

/* Color constant according UI design. */
const XColor WasherTempColor1 = { 0x67, 0xA5, 0x6C, 0xFF };

/* Include a file containing the bitmap resource : 'Washer::Shadow' */
#include "_WasherShadow.h"

/* Table with links to derived variants of the bitmap resource : 'Washer::Shadow' */
EW_RES_WITHOUT_VARIANTS( WasherShadow )

/* Initializer for the class 'Washer::StartButton' */
void WasherStartButton__Init( WasherStartButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WasherStartButton );

  /* ... then construct all embedded objects */
  ViewsFrame__Init( &_this->Frame, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Background, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WasherStartButton );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0079 );
  CoreRectView__OnSetBounds( &_this->Frame, _Const007A );
  ViewsFrame_OnSetColor( &_this->Frame, _Const004D );
  ViewsFrame_OnSetEdges( &_this->Frame, GraphicsEdgesBottom | GraphicsEdgesLeft 
  | GraphicsEdgesRight | GraphicsEdgesTop );
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->TouchHandler, _Const007B );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->TouchHandler, _Const007C );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->TouchHandler, _Const001D );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->TouchHandler, _Const001E );
  CoreView_OnSetLayout((CoreView)&_this->Background, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Background, _Const007D );
  ViewsRectangle_OnSetColor( &_this->Background, _Const007E );
  CoreView_OnSetLayout((CoreView)&_this->Text, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Text, _Const007D );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const007F ));
  CoreGroup__Add( _this, ((CoreView)&_this->Frame ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Background ), 0 );
  CoreGroup__Add( _this, ((CoreView)&_this->Text ), 0 );
  ViewsFrame_OnSetBitmap( &_this->Frame, EwLoadResource( &WasherShadow, ResourcesBitmap ));
  _this->TouchHandler.OnLeave = EwNewSlot( _this, WasherStartButton_enterLeaveSlot );
  _this->TouchHandler.OnEnter = EwNewSlot( _this, WasherStartButton_enterLeaveSlot );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, WasherStartButton_pressReleaseSlot );
  _this->TouchHandler.OnPress = EwNewSlot( _this, WasherStartButton_pressReleaseSlot );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &WasherListFontM, ResourcesFont ));
}

/* Re-Initializer for the class 'Washer::StartButton' */
void WasherStartButton__ReInit( WasherStartButton _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsFrame__ReInit( &_this->Frame );
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
  ViewsRectangle__ReInit( &_this->Background );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Washer::StartButton' */
void WasherStartButton__Done( WasherStartButton _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsFrame__Done( &_this->Frame );
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );
  ViewsRectangle__Done( &_this->Background );
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
void WasherStartButton_UpdateViewState( WasherStartButton _this, XSet aState )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aState );

  if ( _this->TouchHandler.Down && _this->TouchHandler.Inside )
    ViewsRectangle_OnSetColor( &_this->Background, WasherStopColor );
  else
    ViewsRectangle_OnSetColor( &_this->Background, WasherStartColor );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void WasherStartButton_enterLeaveSlot( WasherStartButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void WasherStartButton_pressReleaseSlot( WasherStartButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->TouchHandler.Down && _this->TouchHandler.Inside )
  {
    EwSignal( _this->OnChange, ((XObject)_this ));
  }
}

/* Variants derived from the class : 'Washer::StartButton' */
EW_DEFINE_CLASS_VARIANTS( WasherStartButton )
EW_END_OF_CLASS_VARIANTS( WasherStartButton )

/* Virtual Method Table (VMT) for the class : 'Washer::StartButton' */
EW_DEFINE_CLASS( WasherStartButton, CoreGroup, OnChange, OnChange, Frame, Frame, 
                 _None, _None, "Washer::StartButton" )
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
  WasherStartButton_UpdateViewState,
  CoreGroup_InvalidateArea,
  CoreGroup_FindSiblingView,
  CoreGroup_Add,
EW_END_OF_CLASS( WasherStartButton )

/* Color constant according UI design. */
const XColor WasherStartColor = { 0x8B, 0xD1, 0x36, 0xFF };

/* Color constant according UI design. */
const XColor WasherStopColor = { 0xD1, 0x3B, 0x00, 0xFF };

/* Include a file containing the bitmap resource : 'Washer::BgSpin' */
#include "_WasherBgSpin.h"

/* Table with links to derived variants of the bitmap resource : 'Washer::BgSpin' */
EW_RES_WITHOUT_VARIANTS( WasherBgSpin )

/* Include a file containing the bitmap resource : 'Washer::BgDrain' */
#include "_WasherBgDrain.h"

/* Table with links to derived variants of the bitmap resource : 'Washer::BgDrain' */
EW_RES_WITHOUT_VARIANTS( WasherBgDrain )

/* Include a file containing the bitmap resource : 'Washer::BgEasyCare' */
#include "_WasherBgEasyCare.h"

/* Table with links to derived variants of the bitmap resource : 'Washer::BgEasyCare' */
EW_RES_WITHOUT_VARIANTS( WasherBgEasyCare )

/* Embedded Wizard */
