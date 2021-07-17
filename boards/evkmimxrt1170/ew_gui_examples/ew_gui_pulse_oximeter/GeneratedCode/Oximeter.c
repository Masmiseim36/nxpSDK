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
#include "_CorePropertyObserver.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreSlideTouchHandler.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_EffectsFloatEffect.h"
#include "_EffectsInt32Effect.h"
#include "_EffectsRectEffect.h"
#include "_GraphicsArcPath.h"
#include "_GraphicsPath.h"
#include "_OximeterAnalogDisplay.h"
#include "_OximeterBackground.h"
#include "_OximeterDataStorage.h"
#include "_OximeterDeviceClass.h"
#include "_OximeterGraph.h"
#include "_OximeterGraphPanel.h"
#include "_OximeterMainMenu.h"
#include "_OximeterNumberDisplay.h"
#include "_OximeterOximeter.h"
#include "_OximeterOxygenPanel.h"
#include "_OximeterOxygenSettings.h"
#include "_OximeterPlotter.h"
#include "_OximeterPulsePanel.h"
#include "_OximeterPulseSettings.h"
#include "_OximeterValueDisplay.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "_ViewsFillPath.h"
#include "_ViewsImage.h"
#include "_ViewsLine.h"
#include "_ViewsRectangle.h"
#include "_ViewsStrokePath.h"
#include "_ViewsText.h"
#include "_ViewsWallpaper.h"
#include "_ViewsWarpImage.h"
#include "_WidgetSetHorizontalSlider.h"
#include "_WidgetSetHorizontalSliderConfig.h"
#include "_WidgetSetPushButton.h"
#include "_WidgetSetPushButtonConfig.h"
#include "_WidgetSetRotaryKnob.h"
#include "_WidgetSetRotaryKnobConfig.h"
#include "_WidgetSetToggleButton.h"
#include "_WidgetSetToggleButtonConfig.h"
#include "Core.h"
#include "Effects.h"
#include "Graphics.h"
#include "Oximeter.h"
#include "Views.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x00000414, /* ratio 54.79 % */
  0xB8001500, 0x80086452, 0x00E00030, 0x0D2003A0, 0xDC003780, 0x60989800, 0x00211160,
  0x08DC2CEA, 0x0344E251, 0x78004116, 0x47E44001, 0x0212628B, 0x00113368, 0x00508B02,
  0x04C00154, 0x450014C0, 0x78B01E5A, 0x0013E153, 0x24565A32, 0x800B13F8, 0xE08E002C,
  0x25A4E004, 0x112C8248, 0x916A71E2, 0x008F43A5, 0x02A4584A, 0x32800D00, 0x79CA5525,
  0x58EC12A8, 0x1AC27986, 0x2C7638D4, 0x450C8958, 0xD6139C3A, 0x640041ED, 0xC00476C2,
  0x10DA4006, 0xFC581F2D, 0x99ED3539, 0x2AC311AF, 0xA1508B1D, 0x600E89D7, 0x800BE466,
  0x8B91582E, 0x6A163AEF, 0x003A6311, 0x1E221A7A, 0xC581713B, 0x3AF308CC, 0xE271BB7C,
  0xC85A824E, 0x51396442, 0x850060C2, 0x2D661C01, 0xAD48B137, 0x0CFC013D, 0xC130513A,
  0x5AFB1A98, 0x6C357B0E, 0x01B4854A, 0x03022D3C, 0xAEEAF540, 0x0E6CD62D, 0x20C5B8B6,
  0x658E1194, 0xC8A25BD9, 0x133863E5, 0xA9BE0834, 0xB3359CC1, 0xDE09BC13, 0x1089D2CA,
  0x8CAF2724, 0xDC546458, 0x5D14A9CE, 0xA64DC77E, 0x892B7F5C, 0xCF001E66, 0x05513825,
  0x9BB6E5B7, 0x4118006F, 0x84774419, 0x245F52C5, 0x0512D7A1, 0x1BA5FA11, 0x455A0B62,
  0xA9C056A1, 0xA64B5C34, 0x5DC4A411, 0x00065D12, 0x07756411, 0x01DF0E00, 0x607A5E90,
  0x489745D1, 0x1AB6B179, 0x2600005E, 0xF5124FD0, 0x66489695, 0x48963590, 0x90C46908,
  0xE58D1057, 0xE5606850, 0xAA621FE5, 0x5F5778E9, 0x16642922, 0x48C57B7F, 0x11DE6019,
  0x1B8904D4, 0x4DA60421, 0x63646E6D, 0xB704458B, 0x460D4922, 0x17674117, 0x0025A592,
  0xE3E5D931, 0xB6417664, 0x892C6215, 0x1B630009, 0x695885B1, 0x26E41916, 0xD789799B,
  0x726A9ED2, 0x937A5A29, 0x6205248A, 0xE44955E9, 0x14F00C44, 0x6D734327, 0xA6451FCA,
  0x7A696582, 0x80D0018F, 0x90003145, 0x00350003, 0x2D000370, 0x0D000758, 0xC00502BC,
  0x54BD2D0C, 0x318995A0, 0x49459F84, 0x0E8008CC, 0x141ADD61, 0x7506BB8E, 0xD22FB12B,
  0x014B52D8, 0x00000001, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XRect _Const0000 = {{ 0, 0 }, { 390, 390 }};
static const XRect _Const0001 = {{ 0, 0 }, { 120, 120 }};
static const XColor _Const0002 = { 0xFF, 0xFF, 0xFF, 0x80 };
static const XColor _Const0003 = { 0xFF, 0xFF, 0xFF, 0xFF };
static const XPoint _Const0004 = { -60, 120 };
static const XPoint _Const0005 = { 0, 120 };
static const XPoint _Const0006 = { 0, 0 };
static const XPoint _Const0007 = { -60, 0 };
static const XPoint _Const0008 = { 150, 150 };
static const XRect _Const0009 = {{ 0, 0 }, { 260, 260 }};
static const XRect _Const000A = {{ 0, 0 }, { 240, 240 }};
static const XRect _Const000B = {{ 120, 0 }, { 240, 45 }};
static const XStringRes _Const000C = { _StringsDefault0, 0x0002 };
static const XRect _Const000D = {{ 75, 45 }, { 165, 47 }};
static const XRect _Const000E = {{ 75, 195 }, { 165, 197 }};
static const XRect _Const000F = {{ 0, 195 }, { 240, 240 }};
static const XRect _Const0010 = {{ 75, 0 }, { 120, 45 }};
static const XStringRes _Const0011 = { _StringsDefault0, 0x000C };
static const XRect _Const0012 = {{ 80, 63 }, { 155, 183 }};
static const XRect _Const0013 = {{ 30, 63 }, { 105, 183 }};
static const XRect _Const0014 = {{ 118, 63 }, { 193, 183 }};
static const XStringRes _Const0015 = { _StringsDefault0, 0x0015 };
static const XRect _Const0016 = {{ 0, 0 }, { 640, 720 }};
static const XRect _Const0017 = {{ 125, 120 }, { 515, 525 }};
static const XStringRes _Const0018 = { _StringsDefault0, 0x001B };
static const XRect _Const0019 = {{ 425, 75 }, { 515, 165 }};
static const XRect _Const001A = {{ 95, 67 }, { 177, 150 }};
static const XRect _Const001B = {{ 215, 45 }, { 425, 90 }};
static const XStringRes _Const001C = { _StringsDefault0, 0x0021 };
static const XRect _Const001D = {{ 50, 45 }, { 245, 90 }};
static const XRect _Const001E = {{ 50, 532 }, { 590, 712 }};
static const XPoint _Const001F = { 50, 540 };
static const XPoint _Const0020 = { 590, 540 };
static const XPoint _Const0021 = { 590, 705 };
static const XPoint _Const0022 = { 50, 705 };
static const XPoint _Const0023 = { 110, 120 };
static const XPoint _Const0024 = { 530, 120 };
static const XPoint _Const0025 = { 530, 525 };
static const XPoint _Const0026 = { 110, 525 };
static const XPoint _Const0027 = { 50, 15 };
static const XPoint _Const0028 = { 500, 15 };
static const XPoint _Const0029 = { 500, 225 };
static const XPoint _Const002A = { 50, 225 };
static const XPoint _Const002B = { 320, 90 };
static const XPoint _Const002C = { 605, 90 };
static const XPoint _Const002D = { 605, 705 };
static const XPoint _Const002E = { 320, 705 };
static const XRect _Const002F = {{ 0, 0 }, { 1280, 720 }};
static const XColor _Const0030 = { 0x3C, 0x3C, 0x3C, 0xFF };
static const XRect _Const0031 = {{ 640, 0 }, { 1280, 720 }};
static const XStringRes _Const0032 = { _StringsDefault0, 0x0029 };
static const XRect _Const0033 = {{ 125, 75 }, { 215, 165 }};
static const XStringRes _Const0034 = { _StringsDefault0, 0x0030 };
static const XRect _Const0035 = {{ 395, 45 }, { 590, 90 }};
static const XPoint _Const0036 = { 590, 15 };
static const XPoint _Const0037 = { 590, 225 };
static const XPoint _Const0038 = { 35, 15 };
static const XPoint _Const0039 = { 320, 15 };
static const XPoint _Const003A = { 35, 705 };
static const XRect _Const003B = {{ 0, 0 }, { 540, 180 }};
static const XRect _Const003C = {{ 0, 15 }, { 540, 165 }};
static const XColor _Const003D = { 0xFF, 0xFF, 0xFF, 0x00 };
static const XPoint _Const003E = { 539, 15 };
static const XPoint _Const003F = { 28, 15 };
static const XPoint _Const0040 = { 539, 93 };
static const XPoint _Const0041 = { 28, 93 };
static const XPoint _Const0042 = { 539, 150 };
static const XPoint _Const0043 = { 28, 150 };
static const XPoint _Const0044 = { 539, 165 };
static const XPoint _Const0045 = { 28, 165 };
static const XRect _Const0046 = {{ 0, 0 }, { 30, 20 }};
static const XStringRes _Const0047 = { _StringsDefault0, 0x0039 };
static const XRect _Const0048 = {{ 0, 90 }, { 30, 110 }};
static const XStringRes _Const0049 = { _StringsDefault0, 0x003F };
static const XPoint _Const004A = { 33, 180 };
static const XPoint _Const004B = { 33, 0 };
static const XPoint _Const004C = { 158, 180 };
static const XPoint _Const004D = { 158, 0 };
static const XPoint _Const004E = { 283, 180 };
static const XPoint _Const004F = { 283, 0 };
static const XPoint _Const0050 = { 408, 180 };
static const XPoint _Const0051 = { 408, 0 };
static const XPoint _Const0052 = { 533, 180 };
static const XPoint _Const0053 = { 533, 0 };
static const XStringRes _Const0054 = { _StringsDefault0, 0x0045 };
static const XRect _Const0055 = {{ 0, 0 }, { 480, 270 }};
static const XPoint _Const0056 = { 45, 45 };
static const XPoint _Const0057 = { 80, 80 };
static const XPoint _Const0058 = { 240, 240 };
static const XPoint _Const0059 = { 0, 80 };
static const XRect _Const005A = {{ 0, 0 }, { 1280, 480 }};
static const XRect _Const005B = {{ 980, 90 }, { 1145, 135 }};
static const XStringRes _Const005C = { _StringsDefault0, 0x006A };
static const XRect _Const005D = {{ 1162, 90 }, { 1260, 135 }};
static const XRect _Const005E = {{ 980, 150 }, { 1145, 195 }};
static const XStringRes _Const005F = { _StringsDefault0, 0x0079 };
static const XRect _Const0060 = {{ 1162, 150 }, { 1260, 195 }};
static const XRect _Const0061 = {{ 980, 210 }, { 1145, 255 }};
static const XStringRes _Const0062 = { _StringsDefault0, 0x0087 };
static const XRect _Const0063 = {{ 1162, 210 }, { 1260, 255 }};
static const XRect _Const0064 = {{ 980, 270 }, { 1145, 315 }};
static const XStringRes _Const0065 = { _StringsDefault0, 0x0090 };
static const XRect _Const0066 = {{ 1162, 270 }, { 1260, 315 }};
static const XRect _Const0067 = {{ 980, 330 }, { 1145, 375 }};
static const XStringRes _Const0068 = { _StringsDefault0, 0x009B };
static const XRect _Const0069 = {{ 75, 10 }, { 325, 70 }};
static const XStringRes _Const006A = { _StringsDefault0, 0x00A2 };
static const XPoint _Const006B = { 893, 70 };
static const XPoint _Const006C = { 75, 70 };
static const XColor _Const006D = { 0x80, 0x80, 0x80, 0xFF };
static const XPoint _Const006E = { 893, 191 };
static const XPoint _Const006F = { 75, 191 };
static const XPoint _Const0070 = { 893, 313 };
static const XPoint _Const0071 = { 75, 313 };
static const XPoint _Const0072 = { 893, 434 };
static const XPoint _Const0073 = { 75, 434 };
static const XPoint _Const0074 = { 75, 433 };
static const XPoint _Const0075 = { 279, 433 };
static const XPoint _Const0076 = { 279, 70 };
static const XPoint _Const0077 = { 484, 433 };
static const XPoint _Const0078 = { 484, 70 };
static const XPoint _Const0079 = { 690, 433 };
static const XPoint _Const007A = { 690, 70 };
static const XPoint _Const007B = { 893, 433 };
static const XRect _Const007C = {{ 75, 70 }, { 895, 435 }};
static const XColor _Const007D = { 0x80, 0x80, 0x80, 0x00 };
static const XRect _Const007E = {{ 645, 10 }, { 895, 70 }};
static const XStringRes _Const007F = { _StringsDefault0, 0x00B4 };
static const XRect _Const0080 = {{ 1050, 330 }, { 1260, 375 }};
static const XRect _Const0081 = {{ 15, 60 }, { 60, 90 }};
static const XStringRes _Const0082 = { _StringsDefault0, 0x00C3 };
static const XRect _Const0083 = {{ 15, 417 }, { 60, 447 }};
static const XRect _Const0084 = {{ 905, 60 }, { 950, 90 }};
static const XRect _Const0085 = {{ 905, 415 }, { 950, 445 }};
static const XRect _Const0086 = {{ 1215, 20 }, { 1260, 65 }};
static const XRect _Const0087 = {{ 980, 390 }, { 1145, 435 }};
static const XStringRes _Const0088 = { _StringsDefault0, 0x00C9 };
static const XRect _Const0089 = {{ 1050, 390 }, { 1260, 435 }};
static const XPoint _Const008A = { 98, 45 };
static const XPoint _Const008B = { 1000, 45 };
static const XPoint _Const008C = { 90, 45 };
static const XRect _Const008D = {{ 1170, 20 }, { 1200, 65 }};
static const XRect _Const008E = {{ 0, 0 }, { 900, 720 }};
static const XRect _Const008F = {{ 75, 60 }, { 315, 300 }};
static const XRect _Const0090 = {{ 75, 360 }, { 315, 600 }};
static const XRect _Const0091 = {{ 375, 630 }, { 855, 675 }};
static const XRect _Const0092 = {{ 375, 570 }, { 540, 615 }};
static const XStringRes _Const0093 = { _StringsDefault0, 0x00D0 };
static const XRect _Const0094 = {{ 210, 630 }, { 345, 675 }};
static const XStringRes _Const0095 = { _StringsDefault0, 0x00DF };
static const XRect _Const0096 = {{ 45, 630 }, { 165, 675 }};
static const XStringRes _Const0097 = { _StringsDefault0, 0x00E8 };
static const XRect _Const0098 = {{ 90, 15 }, { 300, 60 }};
static const XStringRes _Const0099 = { _StringsDefault0, 0x00EF };
static const XRect _Const009A = {{ 90, 315 }, { 300, 360 }};
static const XStringRes _Const009B = { _StringsDefault0, 0x00FF };
static const XRect _Const009C = {{ 120, 135 }, { 270, 225 }};
static const XRect _Const009D = {{ 120, 435 }, { 270, 525 }};
static const XRect _Const009E = {{ 525, 555 }, { 675, 615 }};
static const XColor _Const009F = { 0x00, 0x00, 0x00, 0x80 };
static const XPoint _Const00A0 = { 980, 45 };
static const XRect _Const00A1 = {{ 0, 0 }, { 135, 90 }};
static const XRect _Const00A2 = {{ 60, 15 }, { 98, 75 }};
static const XRect _Const00A3 = {{ 10, 15 }, { 48, 75 }};
static const XRect _Const00A4 = {{ 585, 60 }, { 825, 300 }};
static const XRect _Const00A5 = {{ 585, 360 }, { 825, 600 }};
static const XRect _Const00A6 = {{ 45, 630 }, { 525, 675 }};
static const XRect _Const00A7 = {{ 45, 570 }, { 210, 615 }};
static const XRect _Const00A8 = {{ 720, 630 }, { 855, 675 }};
static const XRect _Const00A9 = {{ 555, 630 }, { 690, 675 }};
static const XRect _Const00AA = {{ 585, 15 }, { 825, 60 }};
static const XStringRes _Const00AB = { _StringsDefault0, 0x010F };
static const XRect _Const00AC = {{ 585, 315 }, { 825, 360 }};
static const XStringRes _Const00AD = { _StringsDefault0, 0x0124 };
static const XRect _Const00AE = {{ 630, 135 }, { 780, 225 }};
static const XRect _Const00AF = {{ 630, 435 }, { 780, 525 }};
static const XRect _Const00B0 = {{ 225, 555 }, { 375, 615 }};
static const XColor _Const00B1 = { 0xFF, 0xFF, 0xFF, 0x40 };
static const XPoint _Const00B2 = { 120, 120 };
static const XPoint _Const00B3 = { 180, 180 };
static const XRect _Const00B4 = {{ 575, 20 }, { 620, 65 }};
static const XRect _Const00B5 = {{ 105, 30 }, { 285, 75 }};
static const XStringRes _Const00B6 = { _StringsDefault0, 0x0138 };
static const XRect _Const00B7 = {{ 90, 615 }, { 540, 715 }};
static const XStringRes _Const00B8 = { _StringsDefault0, 0x0143 };
static const XRect _Const00B9 = {{ 105, 190 }, { 225, 310 }};
static const XStringRes _Const00BA = { _StringsDefault0, 0x017B };
static const XRect _Const00BB = {{ 255, 190 }, { 375, 310 }};
static const XStringRes _Const00BC = { _StringsDefault0, 0x018A };
static const XRect _Const00BD = {{ 405, 190 }, { 525, 310 }};
static const XStringRes _Const00BE = { _StringsDefault0, 0x0195 };
static const XRect _Const00BF = {{ 105, 330 }, { 225, 450 }};
static const XStringRes _Const00C0 = { _StringsDefault0, 0x01A6 };
static const XRect _Const00C1 = {{ 255, 330 }, { 375, 450 }};
static const XStringRes _Const00C2 = { _StringsDefault0, 0x01B4 };
static const XRect _Const00C3 = {{ 405, 330 }, { 525, 450 }};
static const XStringRes _Const00C4 = { _StringsDefault0, 0x01BF };
static const XRect _Const00C5 = {{ 105, 470 }, { 225, 590 }};
static const XStringRes _Const00C6 = { _StringsDefault0, 0x01D0 };
static const XRect _Const00C7 = {{ 255, 470 }, { 375, 590 }};
static const XStringRes _Const00C8 = { _StringsDefault0, 0x01DC };
static const XRect _Const00C9 = {{ 105, 75 }, { 232, 120 }};
static const XStringRes _Const00CA = { _StringsDefault0, 0x01E5 };
static const XRect _Const00CB = {{ 405, 470 }, { 525, 590 }};
static const XStringRes _Const00CC = { _StringsDefault0, 0x01F2 };
static const XRect _Const00CD = {{ 105, 120 }, { 232, 165 }};
static const XStringRes _Const00CE = { _StringsDefault0, 0x01FA };

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

/* Initializer for the class 'Oximeter::AnalogDisplay' */
void OximeterAnalogDisplay__Init( OximeterAnalogDisplay _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterAnalogDisplay );

  /* ... then construct all embedded objects */
  EffectsFloatEffect__Init( &_this->RotateEffect, &_this->_XObject, 0 );
  GraphicsArcPath__Init( &_this->BackPath, &_this->_XObject, 0 );
  ViewsStrokePath__Init( &_this->BackView, &_this->_XObject, 0 );
  GraphicsArcPath__Init( &_this->IndicatorPath, &_this->_XObject, 0 );
  ViewsStrokePath__Init( &_this->IndicatorView, &_this->_XObject, 0 );
  ViewsWarpImage__Init( &_this->Glow, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->MinMaxPath, &_this->_XObject, 0 );
  ViewsStrokePath__Init( &_this->MinMaxView, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterAnalogDisplay );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0000 );
  _this->MaxRange = 100;
  _this->CurrentValue = 50;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->RotateEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->RotateEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->RotateEffect, 200 );
  GraphicsArcPath_OnSetEndAngle( &_this->BackPath, 360.000000f );
  GraphicsArcPath_OnSetStartAngle( &_this->BackPath, 0.000000f );
  GraphicsArcPath_OnSetRadiusY( &_this->BackPath, 100.000000f );
  GraphicsArcPath_OnSetRadiusX( &_this->BackPath, 100.000000f );
  GraphicsArcPath_OnSetStyle( &_this->BackPath, GraphicsArcStyleArc );
  CoreRectView__OnSetBounds( &_this->BackView, _Const0001 );
  ViewsStrokePath_OnSetColor( &_this->BackView, _Const0002 );
  ViewsStrokePath_OnSetWidth( &_this->BackView, 35.000000f );
  GraphicsArcPath_OnSetEndAngle( &_this->IndicatorPath, 0.000000f );
  GraphicsArcPath_OnSetStartAngle( &_this->IndicatorPath, -80.000000f );
  GraphicsArcPath_OnSetRadiusY( &_this->IndicatorPath, 50.000000f );
  GraphicsArcPath_OnSetRadiusX( &_this->IndicatorPath, 50.000000f );
  GraphicsArcPath_OnSetStyle( &_this->IndicatorPath, GraphicsArcStyleArc );
  CoreRectView__OnSetBounds( &_this->IndicatorView, _Const0001 );
  ViewsStrokePath_OnSetColor( &_this->IndicatorView, _Const0003 );
  ViewsStrokePath_OnSetMiterLimit( &_this->IndicatorView, 13.000000f );
  ViewsStrokePath_OnSetEndCap( &_this->IndicatorView, GraphicsPathCapRound );
  ViewsStrokePath_OnSetStartCap( &_this->IndicatorView, GraphicsPathCapRound );
  ViewsStrokePath_OnSetWidth( &_this->IndicatorView, 19.000000f );
  CoreQuadView__OnSetPoint4( &_this->Glow, _Const0004 );
  CoreQuadView__OnSetPoint3( &_this->Glow, _Const0005 );
  CoreQuadView__OnSetPoint2( &_this->Glow, _Const0006 );
  CoreQuadView__OnSetPoint1( &_this->Glow, _Const0007 );
  ViewsWarpView_OnSetSourceAnchor((ViewsWarpView)&_this->Glow, _Const0008 );
  ViewsWarpImage_OnSetColor( &_this->Glow, _Const0003 );
  CoreRectView__OnSetBounds( &_this->MinMaxView, _Const0009 );
  ViewsStrokePath_OnSetWidth( &_this->MinMaxView, 3.000000f );
  _this->MinValue = 25;
  _this->MaxValue = 70;
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BackView ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->IndicatorView ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Glow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MinMaxView ), 0 );
  _this->RotateEffect.Super1.OnAnimate = EwNewSlot( _this, OximeterAnalogDisplay_rotateSlot );
  ViewsStrokePath_OnSetPath( &_this->BackView, ((GraphicsPath)&_this->BackPath ));
  ViewsStrokePath_OnSetPath( &_this->IndicatorView, ((GraphicsPath)&_this->IndicatorPath ));
  ViewsWarpImage_OnSetBitmap( &_this->Glow, EwLoadResource( &OximeterGlow, ResourcesBitmap ));
  ViewsStrokePath_OnSetPath( &_this->MinMaxView, &_this->MinMaxPath );
}

/* Re-Initializer for the class 'Oximeter::AnalogDisplay' */
void OximeterAnalogDisplay__ReInit( OximeterAnalogDisplay _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  EffectsFloatEffect__ReInit( &_this->RotateEffect );
  GraphicsArcPath__ReInit( &_this->BackPath );
  ViewsStrokePath__ReInit( &_this->BackView );
  GraphicsArcPath__ReInit( &_this->IndicatorPath );
  ViewsStrokePath__ReInit( &_this->IndicatorView );
  ViewsWarpImage__ReInit( &_this->Glow );
  GraphicsPath__ReInit( &_this->MinMaxPath );
  ViewsStrokePath__ReInit( &_this->MinMaxView );
}

/* Finalizer method for the class 'Oximeter::AnalogDisplay' */
void OximeterAnalogDisplay__Done( OximeterAnalogDisplay _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  EffectsFloatEffect__Done( &_this->RotateEffect );
  GraphicsArcPath__Done( &_this->BackPath );
  ViewsStrokePath__Done( &_this->BackView );
  GraphicsArcPath__Done( &_this->IndicatorPath );
  ViewsStrokePath__Done( &_this->IndicatorView );
  ViewsWarpImage__Done( &_this->Glow );
  GraphicsPath__Done( &_this->MinMaxPath );
  ViewsStrokePath__Done( &_this->MinMaxView );

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
void OximeterAnalogDisplay_UpdateLayout( OximeterAnalogDisplay _this, XPoint aSize )
{
  XInt32 cX;
  XInt32 cY;

  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );

  if ( aSize.X < aSize.Y )
    _this->radius = aSize.X / 2;
  else
    _this->radius = aSize.Y / 2;

  cX = aSize.X / 2;
  cY = aSize.Y / 2;
  CoreRectView__OnSetBounds( &_this->BackView, EwNewRect2Point( _Const0006, aSize ));
  ViewsStrokePath_OnSetOffset( &_this->BackView, EwNewPoint( cX, cY ));
  GraphicsArcPath_OnSetRadiusX( &_this->BackPath, (XFloat)(( _this->radius - ((XInt32)_this->BackView.Width 
  / 2 )) - 1 ));
  GraphicsArcPath_OnSetRadiusY( &_this->BackPath, _this->BackPath.RadiusX );
  CoreRectView__OnSetBounds( &_this->IndicatorView, EwNewRect2Point( _Const0006, 
  aSize ));
  ViewsStrokePath_OnSetOffset( &_this->IndicatorView, EwNewPoint( cX, cY ));
  GraphicsArcPath_OnSetRadiusX( &_this->IndicatorPath, _this->BackPath.RadiusX );
  GraphicsArcPath_OnSetRadiusY( &_this->IndicatorPath, _this->BackPath.RadiusY );
  CoreRectView__OnSetBounds( &_this->MinMaxView, EwNewRect2Point( _Const0006, aSize ));
  ViewsStrokePath_OnSetOffset( &_this->MinMaxView, EwNewPoint( cX, cY ));
  OximeterAnalogDisplay_updateMinMaxPath( _this );
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
void OximeterAnalogDisplay_UpdateViewState( OximeterAnalogDisplay _this, XSet aState )
{
  XFloat angle;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aState );

  angle = 350.000000f;

  if ((( _this->MaxRange > _this->MinRange ) && ( _this->CurrentValue >= _this->MinRange )) 
      && ( _this->CurrentValue <= _this->MaxRange ))
  {
    angle = ( 350.000000f * (XFloat)( _this->CurrentValue - _this->MinRange )) / 
    (XFloat)( _this->MaxRange - _this->MinRange );
  }

  if ( angle != _this->RotateEffect.Value )
  {
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->RotateEffect, 0 );
    _this->RotateEffect.Value = (( _this->RotateEffect.Value * 9.000000f ) + angle ) 
    / 10.000000f;
    _this->RotateEffect.Value1 = _this->RotateEffect.Value;
    _this->RotateEffect.Value2 = angle;
    EffectsEffect_OnSetEnabled((EffectsEffect)&_this->RotateEffect, CoreGroup_OnGetEnabled((CoreGroup)_this ));
  }
}

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetMaxRange()' */
void OximeterAnalogDisplay_OnSetMaxRange( OximeterAnalogDisplay _this, XInt32 value )
{
  if ( _this->MaxRange == value )
    return;

  _this->MaxRange = value;
  OximeterAnalogDisplay_updateMinMaxPath( _this );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetCurrentValue()' */
void OximeterAnalogDisplay_OnSetCurrentValue( OximeterAnalogDisplay _this, XInt32 
  value )
{
  if ( value < _this->MinRange )
    value = _this->MinRange;

  if ( value > _this->MaxRange )
    value = _this->MaxRange;

  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;

  if ( CoreGroup_OnGetEnabled((CoreGroup)_this ))
    CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void OximeterAnalogDisplay_outletSlot( OximeterAnalogDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    OximeterAnalogDisplay_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetOutlet()' */
void OximeterAnalogDisplay_OnSetOutlet( OximeterAnalogDisplay _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, OximeterAnalogDisplay_outletSlot ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, OximeterAnalogDisplay_outletSlot ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, OximeterAnalogDisplay_outletSlot ), ((XObject)_this ));
}

/* Slot method to rotate the needle according calculated value from float effect. */
void OximeterAnalogDisplay_rotateSlot( OximeterAnalogDisplay _this, XObject sender )
{
  XFloat angle;
  XPoint p;
  XFloat scale;
  XInt32 brightness;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  angle = _this->RotateEffect.Value;

  if ( CoreGroup_OnGetEnabled((CoreGroup)_this ) == 0 )
    return;

  if ( angle > 350.000000f )
    angle = 350.000000f;

  if ( angle < 0.000000f )
    angle = 0.000000f;

  GraphicsArcPath_OnSetStartAngle( &_this->IndicatorPath, -85.000000f );
  GraphicsArcPath_OnSetEndAngle( &_this->IndicatorPath, -85.000000f + angle );
  p = EwMovePointNeg( EwGetRectCenter( _this->Super2.Bounds ), _this->Super2.Bounds.Point1 );
  scale = _this->BackPath.RadiusX / (XFloat)_this->Glow.Bitmap->FrameSize.X;
  ViewsWarpView_RotateAndScale((ViewsWarpView)&_this->Glow, p, -8.000000f - angle, 
  scale, scale );
  brightness = (XInt32)(( 255.000000f * ( angle - 60.000000f )) / 150.000000f );

  if ( brightness < 0 )
    brightness = 0;

  if ( brightness > 255 )
    brightness = 255;

  ViewsWarpImage_OnSetOpacity( &_this->Glow, brightness );
}

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetMinValue()' */
void OximeterAnalogDisplay_OnSetMinValue( OximeterAnalogDisplay _this, XInt32 value )
{
  if ( value < _this->MinRange )
    value = _this->MinRange;

  if ( value > _this->MaxRange )
    value = _this->MaxRange;

  if ( _this->MinValue == value )
    return;

  _this->MinValue = value;
  OximeterAnalogDisplay_updateMinMaxPath( _this );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMin has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterAnalogDisplay_outletMinSlot( OximeterAnalogDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->OutletMin.Object != 0 )
    OximeterAnalogDisplay_OnSetMinValue( _this, EwOnGetInt32( _this->OutletMin ));
}

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetOutletMin()' */
void OximeterAnalogDisplay_OnSetOutletMin( OximeterAnalogDisplay _this, XRef value )
{
  if ( !EwCompRef( _this->OutletMin, value ))
    return;

  if ( _this->OutletMin.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, OximeterAnalogDisplay_outletMinSlot ), 
      _this->OutletMin, 0 );

  _this->OutletMin = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, OximeterAnalogDisplay_outletMinSlot ), 
      value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, OximeterAnalogDisplay_outletMinSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetMaxValue()' */
void OximeterAnalogDisplay_OnSetMaxValue( OximeterAnalogDisplay _this, XInt32 value )
{
  if ( value < _this->MinRange )
    value = _this->MinRange;

  if ( value > _this->MaxRange )
    value = _this->MaxRange;

  if ( _this->MaxValue == value )
    return;

  _this->MaxValue = value;
  OximeterAnalogDisplay_updateMinMaxPath( _this );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMax has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterAnalogDisplay_outletMaxSlot( OximeterAnalogDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->OutletMax.Object != 0 )
    OximeterAnalogDisplay_OnSetMaxValue( _this, EwOnGetInt32( _this->OutletMax ));
}

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetOutletMax()' */
void OximeterAnalogDisplay_OnSetOutletMax( OximeterAnalogDisplay _this, XRef value )
{
  if ( !EwCompRef( _this->OutletMax, value ))
    return;

  if ( _this->OutletMax.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, OximeterAnalogDisplay_outletMaxSlot ), 
      _this->OutletMax, 0 );

  _this->OutletMax = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, OximeterAnalogDisplay_outletMaxSlot ), 
      value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, OximeterAnalogDisplay_outletMaxSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Oximeter::AnalogDisplay.updateMinMaxPath()' */
void OximeterAnalogDisplay_updateMinMaxPath( OximeterAnalogDisplay _this )
{
  XInt32 inner = _this->radius - 61;
  XInt32 outer = _this->radius - 41;

  if ( inner < 0 )
    inner = 0;

  if ((((( _this->MaxRange > _this->MinRange ) && ( _this->MaxValue > _this->MinValue )) 
      && ( _this->MinValue >= _this->MinRange )) && ( _this->MaxValue <= _this->MaxRange )) 
      && ( outer > 0 ))
  {
    XFloat minAngle = (( 356.000000f * (XFloat)( _this->MinValue - _this->MinRange )) 
      / (XFloat)( _this->MaxRange - _this->MinRange )) + 2.000000f;
    XFloat maxAngle = (( 356.000000f * (XFloat)( _this->MaxValue - _this->MinRange )) 
      / (XFloat)( _this->MaxRange - _this->MinRange )) + 2.000000f;
    GraphicsPath_InitSubPath( &_this->MinMaxPath, 0, 84 );
    GraphicsPath_Rotate( &_this->MinMaxPath, minAngle - 90.000000f );
    GraphicsPath_Begin( &_this->MinMaxPath, 0, (XFloat)inner, 0.000000f );
    GraphicsPath_AddLine( &_this->MinMaxPath, 0, (XFloat)outer, 0.000000f );
    GraphicsPath_AddArc( &_this->MinMaxPath, 0, 0.000000f, 0.000000f, (XFloat)outer, 
    (XFloat)outer, 0.000000f, maxAngle - minAngle, 80 );
    GraphicsPath_Rotate( &_this->MinMaxPath, maxAngle - minAngle );
    GraphicsPath_AddLine( &_this->MinMaxPath, 0, (XFloat)inner, 0.000000f );
    GraphicsPath_Rotate( &_this->MinMaxPath, 90.000000f - maxAngle );
  }
  else
  {
    GraphicsPath_InitSubPath( &_this->MinMaxPath, 0, 54 );
  }
}

/* Variants derived from the class : 'Oximeter::AnalogDisplay' */
EW_DEFINE_CLASS_VARIANTS( OximeterAnalogDisplay )
EW_END_OF_CLASS_VARIANTS( OximeterAnalogDisplay )

/* Virtual Method Table (VMT) for the class : 'Oximeter::AnalogDisplay' */
EW_DEFINE_CLASS( OximeterAnalogDisplay, CoreGroup, Outlet, Outlet, Outlet, RotateEffect, 
                 MinRange, MinRange, "Oximeter::AnalogDisplay" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  OximeterAnalogDisplay_UpdateLayout,
  OximeterAnalogDisplay_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterAnalogDisplay )

/* Include a file containing the bitmap resource : 'Oximeter::Glow' */
#include "_OximeterGlow.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::Glow' */
EW_RES_WITHOUT_VARIANTS( OximeterGlow )

/* Initializer for the class 'Oximeter::ValueDisplay' */
void OximeterValueDisplay__Init( OximeterValueDisplay _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterValueDisplay );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->CaptionText, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Separator, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Separator1, &_this->_XObject, 0 );
  ViewsText__Init( &_this->RangeText, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->DigitOnes, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->DigitTens, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->DigitHuns, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->DigitPercent, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterValueDisplay );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const000A );
  _this->CurrentValue = 50;
  CoreView_OnSetLayout((CoreView)&_this->CaptionText, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->CaptionText, _Const000B );
  ViewsText_OnSetAutoSize( &_this->CaptionText, 0 );
  ViewsText_OnSetAlignment( &_this->CaptionText, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->CaptionText, EwLoadString( &_Const000C ));
  CoreView_OnSetLayout((CoreView)&_this->Separator, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->Separator, _Const000D );
  CoreView_OnSetLayout((CoreView)&_this->Separator1, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->Separator1, _Const000E );
  CoreView_OnSetLayout((CoreView)&_this->RangeText, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->RangeText, _Const000F );
  _this->MinValue = 25;
  _this->MaxValue = 70;
  CoreView_OnSetLayout((CoreView)&_this->Image, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->Image, _Const0010 );
  _this->Caption = EwShareString( EwLoadString( &_Const0011 ));
  _this->HeaderOpacity = 255;
  CoreView_OnSetLayout((CoreView)&_this->DigitOnes, CoreLayoutAlignToBottom | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->DigitOnes, _Const0012 );
  CoreView_OnSetLayout((CoreView)&_this->DigitTens, CoreLayoutAlignToBottom | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->DigitTens, _Const0013 );
  CoreView_OnSetLayout((CoreView)&_this->DigitHuns, CoreLayoutAlignToBottom | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->DigitHuns, _Const0012 );
  CoreView_OnSetLayout((CoreView)&_this->DigitPercent, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->DigitPercent, _Const0014 );
  ViewsImage_OnSetFrameNumber( &_this->DigitPercent, 10 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CaptionText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Separator ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Separator1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RangeText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DigitOnes ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DigitTens ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DigitHuns ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DigitPercent ), 0 );
  ViewsText_OnSetFont( &_this->CaptionText, EwLoadResource( &OximeterFontM, ResourcesFont ));
  ViewsText_OnSetFont( &_this->RangeText, EwLoadResource( &OximeterFontM, ResourcesFont ));
  ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &OximeterHeartSmall, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->DigitOnes, EwLoadResource( &OximeterDigitsLarge, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->DigitTens, EwLoadResource( &OximeterDigitsLarge, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->DigitHuns, EwLoadResource( &OximeterDigitsLarge, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->DigitPercent, EwLoadResource( &OximeterDigitsLarge, 
  ResourcesBitmap ));
}

/* Re-Initializer for the class 'Oximeter::ValueDisplay' */
void OximeterValueDisplay__ReInit( OximeterValueDisplay _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->CaptionText );
  ViewsRectangle__ReInit( &_this->Separator );
  ViewsRectangle__ReInit( &_this->Separator1 );
  ViewsText__ReInit( &_this->RangeText );
  ViewsImage__ReInit( &_this->Image );
  ViewsImage__ReInit( &_this->DigitOnes );
  ViewsImage__ReInit( &_this->DigitTens );
  ViewsImage__ReInit( &_this->DigitHuns );
  ViewsImage__ReInit( &_this->DigitPercent );
}

/* Finalizer method for the class 'Oximeter::ValueDisplay' */
void OximeterValueDisplay__Done( OximeterValueDisplay _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->CaptionText );
  ViewsRectangle__Done( &_this->Separator );
  ViewsRectangle__Done( &_this->Separator1 );
  ViewsText__Done( &_this->RangeText );
  ViewsImage__Done( &_this->Image );
  ViewsImage__Done( &_this->DigitOnes );
  ViewsImage__Done( &_this->DigitTens );
  ViewsImage__Done( &_this->DigitHuns );
  ViewsImage__Done( &_this->DigitPercent );

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
void OximeterValueDisplay_UpdateViewState( OximeterValueDisplay _this, XSet aState )
{
  XInt32 x;
  XInt32 w;
  XInt32 noOfDigits;
  XInt32 number;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  x = EwGetRectCenter( _this->Separator.Super1.Bounds ).X;
  w = EwGetRectW( ResourcesFont_GetTextExtent( _this->CaptionText.Font, _this->CaptionText.String, 
  0, 100 ));

  if ( _this->Image.Bitmap != 0 )
  {
    w = w + EwGetRectW( _this->Image.Super1.Bounds );
    CoreRectView__OnSetBounds( &_this->Image, EwSetRectX( _this->Image.Super1.Bounds, 
    ( x - ( w / 2 )) - 4 ));
    CoreRectView__OnSetBounds( &_this->CaptionText, EwSetRectX( _this->CaptionText.Super1.Bounds, 
    _this->Image.Super1.Bounds.Point2.X + 2 ));
  }
  else
    CoreRectView__OnSetBounds( &_this->CaptionText, EwSetRectX( _this->CaptionText.Super1.Bounds, 
    x - ( w / 2 )));

  ViewsText_OnSetString( &_this->RangeText, EwConcatString( EwConcatString( EwNewStringInt( 
  _this->MinValue, 2, 10 ), EwLoadString( &_Const0015 )), EwNewStringInt( _this->MaxValue, 
  2, 10 )));
  noOfDigits = 1;
  number = _this->CurrentValue;
  ViewsImage_OnSetFrameNumber( &_this->DigitOnes, number % 10 );
  number = number / 10;

  if ( number > 0 )
  {
    noOfDigits = 2;
    ViewsImage_OnSetFrameNumber( &_this->DigitTens, number % 10 );
  }

  number = number / 10;

  if ( number > 0 )
  {
    noOfDigits = 3;
    ViewsImage_OnSetFrameNumber( &_this->DigitHuns, number % 10 );
  }

  ViewsImage_OnSetVisible( &_this->DigitPercent, _this->Percent );

  if ( _this->Percent )
    noOfDigits = 2;

  x = ( EwGetRectW( _this->Super2.Bounds ) - ( noOfDigits * EwGetRectW( _this->DigitOnes.Super1.Bounds ))) 
  / 2;

  if ( _this->Percent )
    x = x - 10;

  if ( noOfDigits > 2 )
  {
    CoreRectView__OnSetBounds( &_this->DigitHuns, EwSetRectX( _this->DigitHuns.Super1.Bounds, 
    x ));
    ViewsImage_OnSetVisible( &_this->DigitHuns, 1 );
    x = x + EwGetRectW( _this->DigitOnes.Super1.Bounds );
  }
  else
    ViewsImage_OnSetVisible( &_this->DigitHuns, 0 );

  if ( noOfDigits > 1 )
  {
    CoreRectView__OnSetBounds( &_this->DigitTens, EwSetRectX( _this->DigitTens.Super1.Bounds, 
    x ));
    ViewsImage_OnSetVisible( &_this->DigitTens, 1 );
    x = x + EwGetRectW( _this->DigitOnes.Super1.Bounds );
  }
  else
    ViewsImage_OnSetVisible( &_this->DigitTens, 0 );

  CoreRectView__OnSetBounds( &_this->DigitOnes, EwSetRectX( _this->DigitOnes.Super1.Bounds, 
  x ));
  x = x + EwGetRectW( _this->DigitOnes.Super1.Bounds );

  if ( _this->Percent )
    CoreRectView__OnSetBounds( &_this->DigitPercent, EwSetRectX( _this->DigitPercent.Super1.Bounds, 
    x ));
}

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetCurrentValue()' */
void OximeterValueDisplay_OnSetCurrentValue( OximeterValueDisplay _this, XInt32 
  value )
{
  if ( value < 0 )
    value = 0;

  if ( value > 999 )
    value = 999;

  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void OximeterValueDisplay_outletSlot( OximeterValueDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    OximeterValueDisplay_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetOutlet()' */
void OximeterValueDisplay_OnSetOutlet( OximeterValueDisplay _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, OximeterValueDisplay_outletSlot ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, OximeterValueDisplay_outletSlot ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, OximeterValueDisplay_outletSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetMinValue()' */
void OximeterValueDisplay_OnSetMinValue( OximeterValueDisplay _this, XInt32 value )
{
  if ( _this->MinValue == value )
    return;

  _this->MinValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMin has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterValueDisplay_outletMinSlot( OximeterValueDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->OutletMin.Object != 0 )
    OximeterValueDisplay_OnSetMinValue( _this, EwOnGetInt32( _this->OutletMin ));
}

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetOutletMin()' */
void OximeterValueDisplay_OnSetOutletMin( OximeterValueDisplay _this, XRef value )
{
  if ( !EwCompRef( _this->OutletMin, value ))
    return;

  if ( _this->OutletMin.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, OximeterValueDisplay_outletMinSlot ), 
      _this->OutletMin, 0 );

  _this->OutletMin = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, OximeterValueDisplay_outletMinSlot ), 
      value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, OximeterValueDisplay_outletMinSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetMaxValue()' */
void OximeterValueDisplay_OnSetMaxValue( OximeterValueDisplay _this, XInt32 value )
{
  if ( _this->MaxValue == value )
    return;

  _this->MaxValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMax has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterValueDisplay_outletMaxSlot( OximeterValueDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->OutletMax.Object != 0 )
    OximeterValueDisplay_OnSetMaxValue( _this, EwOnGetInt32( _this->OutletMax ));
}

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetOutletMax()' */
void OximeterValueDisplay_OnSetOutletMax( OximeterValueDisplay _this, XRef value )
{
  if ( !EwCompRef( _this->OutletMax, value ))
    return;

  if ( _this->OutletMax.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, OximeterValueDisplay_outletMaxSlot ), 
      _this->OutletMax, 0 );

  _this->OutletMax = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, OximeterValueDisplay_outletMaxSlot ), 
      value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, OximeterValueDisplay_outletMaxSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetIcon()' */
void OximeterValueDisplay_OnSetIcon( OximeterValueDisplay _this, ResourcesBitmap 
  value )
{
  _this->Icon = value;
  ViewsImage_OnSetBitmap( &_this->Image, _this->Icon );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetCaption()' */
void OximeterValueDisplay_OnSetCaption( OximeterValueDisplay _this, XString value )
{
  _this->Caption = EwShareString( value );
  ViewsText_OnSetString( &_this->CaptionText, value );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetHeaderOpacity()' */
void OximeterValueDisplay_OnSetHeaderOpacity( OximeterValueDisplay _this, XInt32 
  value )
{
  if ( value < 0 )
    value = 0;

  if ( value > 255 )
    value = 255;

  if ( _this->HeaderOpacity == value )
    return;

  _this->HeaderOpacity = value;
  ViewsRectangle_OnSetColor( &_this->Separator, EwSetColorAlpha( ViewsRectangle_OnGetColor( 
  &_this->Separator ), (XUInt8)value ));
  ViewsImage_OnSetOpacity( &_this->Image, value );
  ViewsText_OnSetOpacity( &_this->CaptionText, value );
}

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetPercent()' */
void OximeterValueDisplay_OnSetPercent( OximeterValueDisplay _this, XBool value )
{
  if ( _this->Percent == value )
    return;

  _this->Percent = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* Variants derived from the class : 'Oximeter::ValueDisplay' */
EW_DEFINE_CLASS_VARIANTS( OximeterValueDisplay )
EW_END_OF_CLASS_VARIANTS( OximeterValueDisplay )

/* Virtual Method Table (VMT) for the class : 'Oximeter::ValueDisplay' */
EW_DEFINE_CLASS( OximeterValueDisplay, CoreGroup, Icon, Outlet, Outlet, CaptionText, 
                 Caption, CurrentValue, "Oximeter::ValueDisplay" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  OximeterValueDisplay_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterValueDisplay )

/* Include a file containing the font resource : 'Oximeter::FontCaption' */
#include "_OximeterFontCaption.h"

/* Table with links to derived variants of the font resource : 'Oximeter::FontCaption' */
EW_RES_WITHOUT_VARIANTS( OximeterFontCaption )

/* Include a file containing the font resource : 'Oximeter::FontM' */
#include "_OximeterFontM.h"

/* Table with links to derived variants of the font resource : 'Oximeter::FontM' */
EW_RES_WITHOUT_VARIANTS( OximeterFontM )

/* Include a file containing the bitmap resource : 'Oximeter::HeartLarge' */
#include "_OximeterHeartLarge.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::HeartLarge' */
EW_RES_WITHOUT_VARIANTS( OximeterHeartLarge )

/* Include a file containing the bitmap resource : 'Oximeter::HeartSmall' */
#include "_OximeterHeartSmall.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::HeartSmall' */
EW_RES_WITHOUT_VARIANTS( OximeterHeartSmall )

/* Include a file containing the bitmap resource : 'Oximeter::LungLarge' */
#include "_OximeterLungLarge.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::LungLarge' */
EW_RES_WITHOUT_VARIANTS( OximeterLungLarge )

/* Include a file containing the bitmap resource : 'Oximeter::LungSmall' */
#include "_OximeterLungSmall.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::LungSmall' */
EW_RES_WITHOUT_VARIANTS( OximeterLungSmall )

/* Initializer for the class 'Oximeter::PulsePanel' */
void OximeterPulsePanel__Init( OximeterPulsePanel _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterPulsePanel );

  /* ... then construct all embedded objects */
  EffectsRectEffect__Init( &_this->RectEffect, &_this->_XObject, 0 );
  OximeterAnalogDisplay__Init( &_this->AnalogDisplay, &_this->_XObject, 0 );
  OximeterValueDisplay__Init( &_this->ValueDisplay, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeOutEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeInEffect, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->IconTop, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->IconLeft, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextTop, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextLeft, &_this->_XObject, 0 );
  OximeterPlotter__Init( &_this->Plotter, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerChart, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerAdjust, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerOverviewTop, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerOverviewSide, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterPulsePanel );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0016 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->RectEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->RectEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->RectEffect, 500 );
  _this->Mode = OximeterOperationModeOverview;
  CoreRectView__OnSetBounds( &_this->AnalogDisplay, _Const0017 );
  CoreGroup__OnSetBuffered( &_this->AnalogDisplay, 1 );
  OximeterAnalogDisplay_OnSetMaxRange( &_this->AnalogDisplay, 200 );
  OximeterAnalogDisplay_OnSetCurrentValue( &_this->AnalogDisplay, 90 );
  CoreRectView__OnSetBounds( &_this->ValueDisplay, _Const0017 );
  OximeterValueDisplay_OnSetCaption( &_this->ValueDisplay, EwLoadString( &_Const0018 ));
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeOutEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeOutEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeOutEffect, 200 );
  _this->FadeOutEffect.Value2 = 0;
  _this->FadeOutEffect.Value1 = 255;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeInEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeInEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeInEffect, 1000 );
  CoreRectView__OnSetBounds( &_this->IconTop, _Const0019 );
  CoreRectView__OnSetBounds( &_this->IconLeft, _Const001A );
  CoreRectView__OnSetBounds( &_this->TextTop, _Const001B );
  ViewsText_OnSetString( &_this->TextTop, EwLoadString( &_Const001C ));
  CoreRectView__OnSetBounds( &_this->TextLeft, _Const001D );
  ViewsText_OnSetAlignment( &_this->TextLeft, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextLeft, EwLoadString( &_Const001C ));
  CoreRectView__OnSetBounds( &_this->Plotter, _Const001E );
  OximeterPlotter_OnSetMinRange( &_this->Plotter, 30 );
  OximeterPlotter_OnSetMaxRange( &_this->Plotter, 150 );
  CoreQuadView__OnSetPoint4( &_this->SimpleTouchHandlerChart, _Const001F );
  CoreQuadView__OnSetPoint3( &_this->SimpleTouchHandlerChart, _Const0020 );
  CoreQuadView__OnSetPoint2( &_this->SimpleTouchHandlerChart, _Const0021 );
  CoreQuadView__OnSetPoint1( &_this->SimpleTouchHandlerChart, _Const0022 );
  CoreQuadView__OnSetPoint4( &_this->SimpleTouchHandlerAdjust, _Const0023 );
  CoreQuadView__OnSetPoint3( &_this->SimpleTouchHandlerAdjust, _Const0024 );
  CoreQuadView__OnSetPoint2( &_this->SimpleTouchHandlerAdjust, _Const0025 );
  CoreQuadView__OnSetPoint1( &_this->SimpleTouchHandlerAdjust, _Const0026 );
  CoreQuadView__OnSetPoint4( &_this->SimpleTouchHandlerOverviewTop, _Const0027 );
  CoreQuadView__OnSetPoint3( &_this->SimpleTouchHandlerOverviewTop, _Const0028 );
  CoreQuadView__OnSetPoint2( &_this->SimpleTouchHandlerOverviewTop, _Const0029 );
  CoreQuadView__OnSetPoint1( &_this->SimpleTouchHandlerOverviewTop, _Const002A );
  CoreQuadView__OnSetPoint4( &_this->SimpleTouchHandlerOverviewSide, _Const002B );
  CoreQuadView__OnSetPoint3( &_this->SimpleTouchHandlerOverviewSide, _Const002C );
  CoreQuadView__OnSetPoint2( &_this->SimpleTouchHandlerOverviewSide, _Const002D );
  CoreQuadView__OnSetPoint1( &_this->SimpleTouchHandlerOverviewSide, _Const002E );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->AnalogDisplay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ValueDisplay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->IconTop ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->IconLeft ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextTop ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextLeft ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Plotter ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandlerChart ), 
  0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandlerAdjust ), 
  0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandlerOverviewTop ), 
  0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandlerOverviewSide ), 
  0 );
  _this->RectEffect.Super1.OnFinished = EwNewSlot( &_this->FadeInEffect, EffectsEffect_StartEffect );
  _this->RectEffect.Outlet = EwNewRef( &_this->ValueDisplay, CoreRectView_OnGetBounds, 
  CoreRectView__OnSetBounds );
  OximeterAnalogDisplay_OnSetOutlet( &_this->AnalogDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetCurrentPulse, 
  OximeterDeviceClass_OnSetCurrentPulse ));
  OximeterAnalogDisplay_OnSetOutletMin( &_this->AnalogDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMinPulse, OximeterDeviceClass_OnSetMinPulse ));
  OximeterAnalogDisplay_OnSetOutletMax( &_this->AnalogDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMaxPulse, OximeterDeviceClass_OnSetMaxPulse ));
  OximeterValueDisplay_OnSetOutlet( &_this->ValueDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetCurrentPulse, 
  OximeterDeviceClass_OnSetCurrentPulse ));
  OximeterValueDisplay_OnSetOutletMin( &_this->ValueDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMinPulse, OximeterDeviceClass_OnSetMinPulse ));
  OximeterValueDisplay_OnSetOutletMax( &_this->ValueDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMaxPulse, OximeterDeviceClass_OnSetMaxPulse ));
  OximeterValueDisplay_OnSetIcon( &_this->ValueDisplay, EwLoadResource( &OximeterHeartSmall, 
  ResourcesBitmap ));
  _this->FadeOutEffect.Super1.OnFinished = EwNewSlot( &_this->RectEffect, EffectsEffect_StartEffect );
  _this->FadeOutEffect.Super1.OnAnimate = EwNewSlot( _this, OximeterPulsePanel_fadeOut );
  _this->FadeInEffect.Super1.OnFinished = EwNewSlot( _this, OximeterPulsePanel_finished );
  _this->FadeInEffect.Super1.OnAnimate = EwNewSlot( _this, OximeterPulsePanel_fadeIn );
  ViewsImage_OnSetBitmap( &_this->IconTop, EwLoadResource( &OximeterHeartLarge, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->IconLeft, EwLoadResource( &OximeterHeartLarge, 
  ResourcesBitmap ));
  ViewsText_OnSetFont( &_this->TextTop, EwLoadResource( &OximeterFontCaption, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextLeft, EwLoadResource( &OximeterFontCaption, ResourcesFont ));
  OximeterPlotter_OnSetOutletMin( &_this->Plotter, EwNewRef( EwGetAutoObject( &OximeterDevice, 
  OximeterDeviceClass ), OximeterDeviceClass_OnGetMinPulse, OximeterDeviceClass_OnSetMinPulse ));
  OximeterPlotter_OnSetOutletMax( &_this->Plotter, EwNewRef( EwGetAutoObject( &OximeterDevice, 
  OximeterDeviceClass ), OximeterDeviceClass_OnGetMaxPulse, OximeterDeviceClass_OnSetMaxPulse ));
  OximeterPlotter_OnSetDataStorage( &_this->Plotter, &EwGetAutoObject( &OximeterDevice, 
  OximeterDeviceClass )->PulseStorage );

  /* Call the user defined constructor */
  OximeterPulsePanel_Init( _this, aArg );
}

/* Re-Initializer for the class 'Oximeter::PulsePanel' */
void OximeterPulsePanel__ReInit( OximeterPulsePanel _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  EffectsRectEffect__ReInit( &_this->RectEffect );
  OximeterAnalogDisplay__ReInit( &_this->AnalogDisplay );
  OximeterValueDisplay__ReInit( &_this->ValueDisplay );
  EffectsInt32Effect__ReInit( &_this->FadeOutEffect );
  EffectsInt32Effect__ReInit( &_this->FadeInEffect );
  ViewsImage__ReInit( &_this->IconTop );
  ViewsImage__ReInit( &_this->IconLeft );
  ViewsText__ReInit( &_this->TextTop );
  ViewsText__ReInit( &_this->TextLeft );
  OximeterPlotter__ReInit( &_this->Plotter );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerChart );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerAdjust );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerOverviewTop );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerOverviewSide );
}

/* Finalizer method for the class 'Oximeter::PulsePanel' */
void OximeterPulsePanel__Done( OximeterPulsePanel _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  EffectsRectEffect__Done( &_this->RectEffect );
  OximeterAnalogDisplay__Done( &_this->AnalogDisplay );
  OximeterValueDisplay__Done( &_this->ValueDisplay );
  EffectsInt32Effect__Done( &_this->FadeOutEffect );
  EffectsInt32Effect__Done( &_this->FadeInEffect );
  ViewsImage__Done( &_this->IconTop );
  ViewsImage__Done( &_this->IconLeft );
  ViewsText__Done( &_this->TextTop );
  ViewsText__Done( &_this->TextLeft );
  OximeterPlotter__Done( &_this->Plotter );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerChart );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerAdjust );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerOverviewTop );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerOverviewSide );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void OximeterPulsePanel_Init( OximeterPulsePanel _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  ViewsImage_OnSetOpacity( &_this->IconLeft, 0 );
  ViewsImage_OnSetOpacity( &_this->IconTop, 0 );
  ViewsText_OnSetOpacity( &_this->TextLeft, 0 );
  ViewsText_OnSetOpacity( &_this->TextTop, 0 );
  CoreGroup__OnSetOpacity( &_this->AnalogDisplay, 0 );
  CoreGroup__OnSetOpacity( &_this->ValueDisplay, 0 );
  CoreGroup__OnSetOpacity( &_this->Plotter, 0 );
  OximeterPulsePanel_ChangeMode( _this, OximeterOperationModeOverview );
}

/* The function 'ChangeMode' is responsible to make the transission between the 
   different modes of the window and to start the corresponding animations. */
void OximeterPulsePanel_ChangeMode( OximeterPulsePanel _this, XEnum aMode )
{
  XInt32 x;
  XInt32 y;
  XInt32 w;
  XInt32 h;

  _this->Mode = aMode;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeOutEffect, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->RectEffect, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeInEffect, 0 );
  _this->RectEffect.Value1 = _this->ValueDisplay.Super2.Bounds;
  x = _this->AnalogDisplay.Super2.Bounds.Point1.X;
  y = _this->AnalogDisplay.Super2.Bounds.Point1.Y;
  w = EwGetRectW( _this->AnalogDisplay.Super2.Bounds );
  h = EwGetRectH( _this->AnalogDisplay.Super2.Bounds );

  if (( aMode == OximeterOperationModeMenus ) || ( aMode == OximeterOperationModeOxygenSettings ))
  {
    x = EwGetRectCenter( _this->IconTop.Super1.Bounds ).X - ( w / 2 );
  }

  if ( aMode == OximeterOperationModeGraphs )
  {
    y = EwGetRectCenter( _this->IconLeft.Super1.Bounds ).Y - ( h / 2 );
  }

  _this->RectEffect.Value2 = EwNewRect( x, y, x + w, y + h );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->AnalogDisplay, 0 );
  CoreGroup__OnSetBuffered( &_this->AnalogDisplay, 1 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->Plotter, 0 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerOverviewSide, 0 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerOverviewTop, 0 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerAdjust, 0 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerChart, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeOutEffect, 1 );
}

/* 'C' function for method : 'Oximeter::PulsePanel.fadeOut()' */
void OximeterPulsePanel_fadeOut( OximeterPulsePanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Mode != OximeterOperationModeOverview ) && ( _this->Plotter.Super1.Opacity 
      > _this->FadeOutEffect.Value ))
    CoreGroup__OnSetOpacity( &_this->Plotter, _this->FadeOutEffect.Value );

  if ( _this->ValueDisplay.HeaderOpacity > _this->FadeOutEffect.Value )
    OximeterValueDisplay_OnSetHeaderOpacity( &_this->ValueDisplay, _this->FadeOutEffect.Value );

  if ( _this->Mode == OximeterOperationModePulseSettings )
  {
    if ( _this->AnalogDisplay.Super1.Opacity > ( _this->FadeOutEffect.Value + 128 ))
      CoreGroup__OnSetOpacity( &_this->AnalogDisplay, _this->FadeOutEffect.Value 
      + 128 );
  }
  else
    if (( _this->Mode != OximeterOperationModeOverview ) && ( _this->AnalogDisplay.Super1.Opacity 
        > _this->FadeOutEffect.Value ))
      CoreGroup__OnSetOpacity( &_this->AnalogDisplay, _this->FadeOutEffect.Value );

  if (( _this->Mode == OximeterOperationModePulseSettings ) && ( _this->ValueDisplay.Super1.Opacity 
      > ( _this->FadeOutEffect.Value + 128 )))
    CoreGroup__OnSetOpacity( &_this->ValueDisplay, _this->FadeOutEffect.Value + 
    128 );

  if ( _this->IconTop.Opacity > _this->FadeOutEffect.Value )
    ViewsImage_OnSetOpacity( &_this->IconTop, _this->FadeOutEffect.Value );

  if ( _this->IconLeft.Opacity > _this->FadeOutEffect.Value )
    ViewsImage_OnSetOpacity( &_this->IconLeft, _this->FadeOutEffect.Value );

  if ( _this->TextTop.Opacity > _this->FadeOutEffect.Value )
    ViewsText_OnSetOpacity( &_this->TextTop, _this->FadeOutEffect.Value );

  if ( _this->TextLeft.Opacity > _this->FadeOutEffect.Value )
    ViewsText_OnSetOpacity( &_this->TextLeft, _this->FadeOutEffect.Value );
}

/* 'C' function for method : 'Oximeter::PulsePanel.fadeIn()' */
void OximeterPulsePanel_fadeIn( OximeterPulsePanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Mode == OximeterOperationModeOverview ) && ( _this->Plotter.Super1.Opacity 
      < _this->FadeInEffect.Value ))
    CoreGroup__OnSetOpacity( &_this->Plotter, _this->FadeInEffect.Value );

  if (( _this->Mode == OximeterOperationModePulseSettings ) || ( _this->Mode == 
      OximeterOperationModeOverview ))
  {
    OximeterValueDisplay_OnSetIcon( &_this->ValueDisplay, EwLoadResource( &OximeterHeartSmall, 
    ResourcesBitmap ));

    if ( _this->ValueDisplay.HeaderOpacity < _this->FadeInEffect.Value )
      OximeterValueDisplay_OnSetHeaderOpacity( &_this->ValueDisplay, _this->FadeInEffect.Value );
  }
  else
    if (( _this->Mode == OximeterOperationModeMenus ) || ( _this->Mode == OximeterOperationModeOxygenSettings ))
    {
      OximeterValueDisplay_OnSetIcon( &_this->ValueDisplay, 0 );

      if ( _this->ValueDisplay.HeaderOpacity < _this->FadeInEffect.Value )
        OximeterValueDisplay_OnSetHeaderOpacity( &_this->ValueDisplay, _this->FadeInEffect.Value );
    }

  if (( _this->Mode == OximeterOperationModeOverview ) && ( _this->AnalogDisplay.Super1.Opacity 
      < _this->FadeInEffect.Value ))
    CoreGroup__OnSetOpacity( &_this->AnalogDisplay, _this->FadeInEffect.Value );

  if (( _this->Mode == OximeterOperationModePulseSettings ) && ( _this->AnalogDisplay.Super1.Opacity 
      < ( _this->FadeInEffect.Value - 128 )))
    CoreGroup__OnSetOpacity( &_this->AnalogDisplay, _this->FadeInEffect.Value - 
    128 );

  if (( _this->Mode != OximeterOperationModePulseSettings ) && ( _this->ValueDisplay.Super1.Opacity 
      < _this->FadeInEffect.Value ))
    CoreGroup__OnSetOpacity( &_this->ValueDisplay, _this->FadeInEffect.Value );

  if (( _this->Mode == OximeterOperationModeOverview ) && ( _this->TextLeft.Opacity 
      < _this->FadeInEffect.Value ))
    ViewsText_OnSetOpacity( &_this->TextLeft, _this->FadeInEffect.Value );

  if (( _this->Mode == OximeterOperationModePulseSettings ) && ( _this->TextTop.Opacity 
      < ( _this->FadeInEffect.Value - 128 )))
    ViewsText_OnSetOpacity( &_this->TextTop, _this->FadeInEffect.Value - 128 );

  if ((( _this->Mode == OximeterOperationModeMenus ) || ( _this->Mode == OximeterOperationModeOxygenSettings )) 
      && ( _this->IconTop.Opacity < _this->FadeInEffect.Value ))
    ViewsImage_OnSetOpacity( &_this->IconTop, _this->FadeInEffect.Value );

  if (( _this->Mode == OximeterOperationModeGraphs ) && ( _this->IconLeft.Opacity 
      < _this->FadeInEffect.Value ))
    ViewsImage_OnSetOpacity( &_this->IconLeft, _this->FadeInEffect.Value );

  if ( _this->Mode == OximeterOperationModeOverview )
  {
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerAdjust, 1 );
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerChart, 1 );
  }

  if ( _this->Mode == OximeterOperationModeGraphs )
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerOverviewTop, 
    1 );

  if (( _this->Mode == OximeterOperationModeMenus ) || ( _this->Mode == OximeterOperationModeOxygenSettings ))
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerOverviewSide, 
    1 );
}

/* 'C' function for method : 'Oximeter::PulsePanel.finished()' */
void OximeterPulsePanel_finished( OximeterPulsePanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Mode == OximeterOperationModeOverview ) || ( _this->Mode == OximeterOperationModePulseSettings ))
    CoreGroup_OnSetEnabled((CoreGroup)&_this->AnalogDisplay, 1 );

  if ( _this->Mode == OximeterOperationModeOverview )
    CoreGroup_OnSetEnabled((CoreGroup)&_this->Plotter, 1 );
}

/* 'C' function for method : 'Oximeter::PulsePanel.OnSetOnChart()' */
void OximeterPulsePanel_OnSetOnChart( OximeterPulsePanel _this, XSlot value )
{
  if ( !EwCompSlot( _this->OnChart, value ))
    return;

  _this->OnChart = value;
  _this->SimpleTouchHandlerChart.OnPress = value;
}

/* 'C' function for method : 'Oximeter::PulsePanel.OnSetOnAdjust()' */
void OximeterPulsePanel_OnSetOnAdjust( OximeterPulsePanel _this, XSlot value )
{
  if ( !EwCompSlot( _this->OnAdjust, value ))
    return;

  _this->OnAdjust = value;
  _this->SimpleTouchHandlerAdjust.OnPress = value;
}

/* 'C' function for method : 'Oximeter::PulsePanel.OnSetOnOverview()' */
void OximeterPulsePanel_OnSetOnOverview( OximeterPulsePanel _this, XSlot value )
{
  if ( !EwCompSlot( _this->OnOverview, value ))
    return;

  _this->OnOverview = value;
  _this->SimpleTouchHandlerOverviewTop.OnPress = value;
  _this->SimpleTouchHandlerOverviewSide.OnPress = value;
}

/* Variants derived from the class : 'Oximeter::PulsePanel' */
EW_DEFINE_CLASS_VARIANTS( OximeterPulsePanel )
EW_END_OF_CLASS_VARIANTS( OximeterPulsePanel )

/* Virtual Method Table (VMT) for the class : 'Oximeter::PulsePanel' */
EW_DEFINE_CLASS( OximeterPulsePanel, CoreGroup, OnChart, OnChart, RectEffect, RectEffect, 
                 Mode, Mode, "Oximeter::PulsePanel" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterPulsePanel )

/* Initializer for the class 'Oximeter::Background' */
void OximeterBackground__Init( OximeterBackground _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterBackground );

  /* ... then construct all embedded objects */
  EffectsRectEffect__Init( &_this->RectEffectOxygen, &_this->_XObject, 0 );
  EffectsRectEffect__Init( &_this->RectEffectPulse, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  ViewsWallpaper__Init( &_this->OxygenGradient, &_this->_XObject, 0 );
  ViewsWallpaper__Init( &_this->PulseGradient, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterBackground );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const002F );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->RectEffectOxygen, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->RectEffectOxygen, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->RectEffectOxygen, 700 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->RectEffectOxygen, 100 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->RectEffectPulse, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->RectEffectPulse, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->RectEffectPulse, 700 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->RectEffectPulse, 100 );
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const002F );
  ViewsRectangle_OnSetColor( &_this->Rectangle, _Const0030 );
  CoreView_OnSetLayout((CoreView)&_this->OxygenGradient, CoreLayoutAlignToBottom 
  | CoreLayoutAlignToLeft | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->OxygenGradient, _Const0016 );
  CoreView_OnSetLayout((CoreView)&_this->PulseGradient, CoreLayoutAlignToBottom 
  | CoreLayoutAlignToLeft | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->PulseGradient, _Const0031 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->OxygenGradient ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PulseGradient ), 0 );
  _this->RectEffectOxygen.Outlet = EwNewRef( &_this->OxygenGradient, CoreRectView_OnGetBounds, 
  CoreRectView__OnSetBounds );
  _this->RectEffectPulse.Outlet = EwNewRef( &_this->PulseGradient, CoreRectView_OnGetBounds, 
  CoreRectView__OnSetBounds );
  ViewsWallpaper_OnSetBitmap( &_this->OxygenGradient, EwLoadResource( &OximeterOxygenGradient, 
  ResourcesBitmap ));
  ViewsWallpaper_OnSetBitmap( &_this->PulseGradient, EwLoadResource( &OximeterPulseGradient, 
  ResourcesBitmap ));
}

/* Re-Initializer for the class 'Oximeter::Background' */
void OximeterBackground__ReInit( OximeterBackground _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  EffectsRectEffect__ReInit( &_this->RectEffectOxygen );
  EffectsRectEffect__ReInit( &_this->RectEffectPulse );
  ViewsRectangle__ReInit( &_this->Rectangle );
  ViewsWallpaper__ReInit( &_this->OxygenGradient );
  ViewsWallpaper__ReInit( &_this->PulseGradient );
}

/* Finalizer method for the class 'Oximeter::Background' */
void OximeterBackground__Done( OximeterBackground _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  EffectsRectEffect__Done( &_this->RectEffectOxygen );
  EffectsRectEffect__Done( &_this->RectEffectPulse );
  ViewsRectangle__Done( &_this->Rectangle );
  ViewsWallpaper__Done( &_this->OxygenGradient );
  ViewsWallpaper__Done( &_this->PulseGradient );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The function 'ChangeMode' is responsible to make the transission between the 
   different modes of the window and to start the corresponding animations. */
void OximeterBackground_ChangeMode( OximeterBackground _this, XEnum aMode )
{
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->RectEffectOxygen, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->RectEffectPulse, 0 );
  _this->RectEffectOxygen.Value1 = _this->OxygenGradient.Super1.Bounds;
  _this->RectEffectPulse.Value1 = _this->PulseGradient.Super1.Bounds;

  switch ( aMode )
  {
    case OximeterOperationModeOverview :
    {
      _this->RectEffectOxygen.Value2 = EwNewRect( 0, 0, EwGetRectW( _this->Super2.Bounds ) 
      / 2, EwGetRectH( _this->Super2.Bounds ));
      _this->RectEffectPulse.Value2 = EwNewRect( EwGetRectW( _this->Super2.Bounds ) 
      / 2, 0, EwGetRectW( _this->Super2.Bounds ), EwGetRectH( _this->Super2.Bounds ));
    }
    break;

    case OximeterOperationModeGraphs :
    {
      _this->RectEffectOxygen.Value2 = EwNewRect( 0, -EwGetRectH( _this->Super2.Bounds ) 
      / 3, EwGetRectW( _this->Super2.Bounds ) / 2, EwGetRectH( _this->Super2.Bounds ) 
      / 3 );
      _this->RectEffectPulse.Value2 = EwNewRect( EwGetRectW( _this->Super2.Bounds ) 
      / 2, -EwGetRectH( _this->Super2.Bounds ) / 3, EwGetRectW( _this->Super2.Bounds ), 
      EwGetRectH( _this->Super2.Bounds ) / 3 );
    }
    break;

    case OximeterOperationModeMenus :
    {
      _this->RectEffectOxygen.Value2 = EwNewRect( 0, 0, EwGetRectW( _this->Super2.Bounds ) 
      / 4, EwGetRectH( _this->Super2.Bounds ));
      _this->RectEffectPulse.Value2 = EwNewRect(( 3 * EwGetRectW( _this->Super2.Bounds )) 
      / 4, 0, EwGetRectW( _this->Super2.Bounds ), EwGetRectH( _this->Super2.Bounds ));
    }
    break;

    case OximeterOperationModeOxygenSettings :
    {
      _this->RectEffectOxygen.Value2 = EwNewRect( 0, 0, ( 3 * EwGetRectW( _this->Super2.Bounds )) 
      / 4, EwGetRectH( _this->Super2.Bounds ));
      _this->RectEffectPulse.Value2 = EwNewRect(( 3 * EwGetRectW( _this->Super2.Bounds )) 
      / 4, 0, EwGetRectW( _this->Super2.Bounds ), EwGetRectH( _this->Super2.Bounds ));
    }
    break;

    case OximeterOperationModePulseSettings :
    {
      _this->RectEffectOxygen.Value2 = EwNewRect( 0, 0, EwGetRectW( _this->Super2.Bounds ) 
      / 4, EwGetRectH( _this->Super2.Bounds ));
      _this->RectEffectPulse.Value2 = EwNewRect( EwGetRectW( _this->Super2.Bounds ) 
      / 4, 0, EwGetRectW( _this->Super2.Bounds ), EwGetRectH( _this->Super2.Bounds ));
    }
    break;

    default : 
      ;
  }

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->RectEffectOxygen, 1 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->RectEffectPulse, 1 );
}

/* Variants derived from the class : 'Oximeter::Background' */
EW_DEFINE_CLASS_VARIANTS( OximeterBackground )
EW_END_OF_CLASS_VARIANTS( OximeterBackground )

/* Virtual Method Table (VMT) for the class : 'Oximeter::Background' */
EW_DEFINE_CLASS( OximeterBackground, CoreGroup, RectEffectOxygen, RectEffectOxygen, 
                 RectEffectOxygen, RectEffectOxygen, _None, _None, "Oximeter::Background" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterBackground )

/* Initializer for the class 'Oximeter::OxygenPanel' */
void OximeterOxygenPanel__Init( OximeterOxygenPanel _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterOxygenPanel );

  /* ... then construct all embedded objects */
  EffectsRectEffect__Init( &_this->RectEffect, &_this->_XObject, 0 );
  OximeterAnalogDisplay__Init( &_this->AnalogDisplay, &_this->_XObject, 0 );
  OximeterValueDisplay__Init( &_this->PercentDisplay, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeOutEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeInEffect, &_this->_XObject, 0 );
  OximeterPlotter__Init( &_this->Plotter, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->IconTop, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->IconLeft, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextTop, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextRight, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerChart, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerAdjust, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerOverviewTop, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerOverviewSide, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterOxygenPanel );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0016 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->RectEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->RectEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->RectEffect, 500 );
  _this->Mode = OximeterOperationModeOverview;
  CoreRectView__OnSetBounds( &_this->AnalogDisplay, _Const0017 );
  CoreGroup__OnSetBuffered( &_this->AnalogDisplay, 1 );
  OximeterAnalogDisplay_OnSetCurrentValue( &_this->AnalogDisplay, 90 );
  CoreRectView__OnSetBounds( &_this->PercentDisplay, _Const0017 );
  OximeterValueDisplay_OnSetCaption( &_this->PercentDisplay, EwLoadString( &_Const0032 ));
  OximeterValueDisplay_OnSetPercent( &_this->PercentDisplay, 1 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeOutEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeOutEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeOutEffect, 200 );
  _this->FadeOutEffect.Value2 = 0;
  _this->FadeOutEffect.Value1 = 255;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->FadeInEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeInEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeInEffect, 1000 );
  CoreRectView__OnSetBounds( &_this->Plotter, _Const001E );
  OximeterPlotter_OnSetMinValue( &_this->Plotter, 70 );
  OximeterPlotter_OnSetMinRange( &_this->Plotter, 70 );
  OximeterPlotter_OnSetMaxRange( &_this->Plotter, 100 );
  CoreRectView__OnSetBounds( &_this->IconTop, _Const0033 );
  CoreRectView__OnSetBounds( &_this->IconLeft, _Const001A );
  CoreRectView__OnSetBounds( &_this->TextTop, _Const001B );
  ViewsText_OnSetString( &_this->TextTop, EwLoadString( &_Const0034 ));
  CoreRectView__OnSetBounds( &_this->TextRight, _Const0035 );
  ViewsText_OnSetAlignment( &_this->TextRight, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextRight, EwLoadString( &_Const0034 ));
  CoreQuadView__OnSetPoint4( &_this->SimpleTouchHandlerChart, _Const001F );
  CoreQuadView__OnSetPoint3( &_this->SimpleTouchHandlerChart, _Const0020 );
  CoreQuadView__OnSetPoint2( &_this->SimpleTouchHandlerChart, _Const0021 );
  CoreQuadView__OnSetPoint1( &_this->SimpleTouchHandlerChart, _Const0022 );
  CoreQuadView__OnSetPoint4( &_this->SimpleTouchHandlerAdjust, _Const0023 );
  CoreQuadView__OnSetPoint3( &_this->SimpleTouchHandlerAdjust, _Const0024 );
  CoreQuadView__OnSetPoint2( &_this->SimpleTouchHandlerAdjust, _Const0025 );
  CoreQuadView__OnSetPoint1( &_this->SimpleTouchHandlerAdjust, _Const0026 );
  CoreQuadView__OnSetPoint4( &_this->SimpleTouchHandlerOverviewTop, _Const0027 );
  CoreQuadView__OnSetPoint3( &_this->SimpleTouchHandlerOverviewTop, _Const0036 );
  CoreQuadView__OnSetPoint2( &_this->SimpleTouchHandlerOverviewTop, _Const0037 );
  CoreQuadView__OnSetPoint1( &_this->SimpleTouchHandlerOverviewTop, _Const002A );
  CoreQuadView__OnSetPoint4( &_this->SimpleTouchHandlerOverviewSide, _Const0038 );
  CoreQuadView__OnSetPoint3( &_this->SimpleTouchHandlerOverviewSide, _Const0039 );
  CoreQuadView__OnSetPoint2( &_this->SimpleTouchHandlerOverviewSide, _Const002E );
  CoreQuadView__OnSetPoint1( &_this->SimpleTouchHandlerOverviewSide, _Const003A );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->AnalogDisplay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PercentDisplay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Plotter ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->IconTop ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->IconLeft ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextTop ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextRight ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandlerChart ), 
  0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandlerAdjust ), 
  0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandlerOverviewTop ), 
  0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandlerOverviewSide ), 
  0 );
  _this->RectEffect.Super1.OnFinished = EwNewSlot( &_this->FadeInEffect, EffectsEffect_StartEffect );
  _this->RectEffect.Outlet = EwNewRef( &_this->PercentDisplay, CoreRectView_OnGetBounds, 
  CoreRectView__OnSetBounds );
  OximeterAnalogDisplay_OnSetOutlet( &_this->AnalogDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetCurrentOxygen, 
  OximeterDeviceClass_OnSetCurrentOxygen ));
  OximeterAnalogDisplay_OnSetOutletMin( &_this->AnalogDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMinOxygen, OximeterDeviceClass_OnSetMinOxygen ));
  OximeterAnalogDisplay_OnSetOutletMax( &_this->AnalogDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMaxOxygen, OximeterDeviceClass_OnSetMaxOxygen ));
  OximeterValueDisplay_OnSetOutlet( &_this->PercentDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetCurrentOxygen, 
  OximeterDeviceClass_OnSetCurrentOxygen ));
  OximeterValueDisplay_OnSetOutletMin( &_this->PercentDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMinOxygen, OximeterDeviceClass_OnSetMinOxygen ));
  OximeterValueDisplay_OnSetOutletMax( &_this->PercentDisplay, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMaxOxygen, OximeterDeviceClass_OnSetMaxOxygen ));
  OximeterValueDisplay_OnSetIcon( &_this->PercentDisplay, EwLoadResource( &OximeterLungSmall, 
  ResourcesBitmap ));
  _this->FadeOutEffect.Super1.OnFinished = EwNewSlot( &_this->RectEffect, EffectsEffect_StartEffect );
  _this->FadeOutEffect.Super1.OnAnimate = EwNewSlot( _this, OximeterOxygenPanel_fadeOut );
  _this->FadeInEffect.Super1.OnFinished = EwNewSlot( _this, OximeterOxygenPanel_finished );
  _this->FadeInEffect.Super1.OnAnimate = EwNewSlot( _this, OximeterOxygenPanel_fadeIn );
  OximeterPlotter_OnSetOutletMin( &_this->Plotter, EwNewRef( EwGetAutoObject( &OximeterDevice, 
  OximeterDeviceClass ), OximeterDeviceClass_OnGetMinOxygen, OximeterDeviceClass_OnSetMinOxygen ));
  OximeterPlotter_OnSetOutletMax( &_this->Plotter, EwNewRef( EwGetAutoObject( &OximeterDevice, 
  OximeterDeviceClass ), OximeterDeviceClass_OnGetMaxOxygen, OximeterDeviceClass_OnSetMaxOxygen ));
  OximeterPlotter_OnSetDataStorage( &_this->Plotter, &EwGetAutoObject( &OximeterDevice, 
  OximeterDeviceClass )->OxygenStorage );
  ViewsImage_OnSetBitmap( &_this->IconTop, EwLoadResource( &OximeterLungLarge, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->IconLeft, EwLoadResource( &OximeterLungLarge, 
  ResourcesBitmap ));
  ViewsText_OnSetFont( &_this->TextTop, EwLoadResource( &OximeterFontCaption, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextRight, EwLoadResource( &OximeterFontCaption, 
  ResourcesFont ));

  /* Call the user defined constructor */
  OximeterOxygenPanel_Init( _this, aArg );
}

/* Re-Initializer for the class 'Oximeter::OxygenPanel' */
void OximeterOxygenPanel__ReInit( OximeterOxygenPanel _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  EffectsRectEffect__ReInit( &_this->RectEffect );
  OximeterAnalogDisplay__ReInit( &_this->AnalogDisplay );
  OximeterValueDisplay__ReInit( &_this->PercentDisplay );
  EffectsInt32Effect__ReInit( &_this->FadeOutEffect );
  EffectsInt32Effect__ReInit( &_this->FadeInEffect );
  OximeterPlotter__ReInit( &_this->Plotter );
  ViewsImage__ReInit( &_this->IconTop );
  ViewsImage__ReInit( &_this->IconLeft );
  ViewsText__ReInit( &_this->TextTop );
  ViewsText__ReInit( &_this->TextRight );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerChart );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerAdjust );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerOverviewTop );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerOverviewSide );
}

/* Finalizer method for the class 'Oximeter::OxygenPanel' */
void OximeterOxygenPanel__Done( OximeterOxygenPanel _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  EffectsRectEffect__Done( &_this->RectEffect );
  OximeterAnalogDisplay__Done( &_this->AnalogDisplay );
  OximeterValueDisplay__Done( &_this->PercentDisplay );
  EffectsInt32Effect__Done( &_this->FadeOutEffect );
  EffectsInt32Effect__Done( &_this->FadeInEffect );
  OximeterPlotter__Done( &_this->Plotter );
  ViewsImage__Done( &_this->IconTop );
  ViewsImage__Done( &_this->IconLeft );
  ViewsText__Done( &_this->TextTop );
  ViewsText__Done( &_this->TextRight );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerChart );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerAdjust );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerOverviewTop );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerOverviewSide );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void OximeterOxygenPanel_Init( OximeterOxygenPanel _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  ViewsImage_OnSetOpacity( &_this->IconLeft, 0 );
  ViewsImage_OnSetOpacity( &_this->IconTop, 0 );
  ViewsText_OnSetOpacity( &_this->TextRight, 0 );
  ViewsText_OnSetOpacity( &_this->TextTop, 0 );
  CoreGroup__OnSetOpacity( &_this->AnalogDisplay, 0 );
  CoreGroup__OnSetOpacity( &_this->PercentDisplay, 0 );
  CoreGroup__OnSetOpacity( &_this->Plotter, 0 );
  OximeterOxygenPanel_ChangeMode( _this, OximeterOperationModeOverview );
}

/* The function 'ChangeMode' is responsible to make the transission between the 
   different modes of the window and to start the corresponding animations. */
void OximeterOxygenPanel_ChangeMode( OximeterOxygenPanel _this, XEnum aMode )
{
  XInt32 x;
  XInt32 y;
  XInt32 w;
  XInt32 h;

  _this->Mode = aMode;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeOutEffect, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->RectEffect, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeInEffect, 0 );
  _this->RectEffect.Value1 = _this->PercentDisplay.Super2.Bounds;
  x = _this->AnalogDisplay.Super2.Bounds.Point1.X;
  y = _this->AnalogDisplay.Super2.Bounds.Point1.Y;
  w = EwGetRectW( _this->AnalogDisplay.Super2.Bounds );
  h = EwGetRectH( _this->AnalogDisplay.Super2.Bounds );

  if (( aMode == OximeterOperationModeMenus ) || ( aMode == OximeterOperationModePulseSettings ))
  {
    x = EwGetRectCenter( _this->IconTop.Super1.Bounds ).X - ( w / 2 );
  }

  if ( aMode == OximeterOperationModeGraphs )
  {
    y = EwGetRectCenter( _this->IconLeft.Super1.Bounds ).Y - ( h / 2 );
  }

  _this->RectEffect.Value2 = EwNewRect( x, y, x + w, y + h );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->AnalogDisplay, 0 );
  CoreGroup__OnSetBuffered( &_this->AnalogDisplay, 1 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->Plotter, 0 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerOverviewSide, 0 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerOverviewTop, 0 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerAdjust, 0 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerChart, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeOutEffect, 1 );
}

/* 'C' function for method : 'Oximeter::OxygenPanel.fadeOut()' */
void OximeterOxygenPanel_fadeOut( OximeterOxygenPanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Mode != OximeterOperationModeOverview ) && ( _this->Plotter.Super1.Opacity 
      > _this->FadeOutEffect.Value ))
    CoreGroup__OnSetOpacity( &_this->Plotter, (XUInt8)_this->FadeOutEffect.Value );

  if ( _this->PercentDisplay.HeaderOpacity > _this->FadeOutEffect.Value )
    OximeterValueDisplay_OnSetHeaderOpacity( &_this->PercentDisplay, _this->FadeOutEffect.Value );

  if ( _this->Mode == OximeterOperationModeOxygenSettings )
  {
    if ( _this->AnalogDisplay.Super1.Opacity > ( _this->FadeOutEffect.Value + 128 ))
      CoreGroup__OnSetOpacity( &_this->AnalogDisplay, _this->FadeOutEffect.Value 
      + 128 );
  }
  else
    if (( _this->Mode != OximeterOperationModeOverview ) && ( _this->AnalogDisplay.Super1.Opacity 
        > _this->FadeOutEffect.Value ))
      CoreGroup__OnSetOpacity( &_this->AnalogDisplay, _this->FadeOutEffect.Value );

  if (( _this->Mode == OximeterOperationModeOxygenSettings ) && ( _this->PercentDisplay.Super1.Opacity 
      > ( _this->FadeOutEffect.Value + 128 )))
    CoreGroup__OnSetOpacity( &_this->PercentDisplay, _this->FadeOutEffect.Value 
    + 128 );

  if ( _this->IconLeft.Opacity > _this->FadeOutEffect.Value )
    ViewsImage_OnSetOpacity( &_this->IconLeft, _this->FadeOutEffect.Value );

  if ( _this->IconTop.Opacity > _this->FadeOutEffect.Value )
    ViewsImage_OnSetOpacity( &_this->IconTop, _this->FadeOutEffect.Value );

  if ( _this->TextTop.Opacity > _this->FadeOutEffect.Value )
    ViewsText_OnSetOpacity( &_this->TextTop, _this->FadeOutEffect.Value );

  if ( _this->TextRight.Opacity > _this->FadeOutEffect.Value )
    ViewsText_OnSetOpacity( &_this->TextRight, _this->FadeOutEffect.Value );
}

/* 'C' function for method : 'Oximeter::OxygenPanel.fadeIn()' */
void OximeterOxygenPanel_fadeIn( OximeterOxygenPanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Mode == OximeterOperationModeOverview ) && ( _this->Plotter.Super1.Opacity 
      < _this->FadeInEffect.Value ))
    CoreGroup__OnSetOpacity( &_this->Plotter, (XUInt8)_this->FadeInEffect.Value );

  if (( _this->Mode == OximeterOperationModeOxygenSettings ) || ( _this->Mode == 
      OximeterOperationModeOverview ))
  {
    OximeterValueDisplay_OnSetIcon( &_this->PercentDisplay, EwLoadResource( &OximeterLungSmall, 
    ResourcesBitmap ));

    if ( _this->PercentDisplay.HeaderOpacity < _this->FadeInEffect.Value )
      OximeterValueDisplay_OnSetHeaderOpacity( &_this->PercentDisplay, _this->FadeInEffect.Value );
  }
  else
    if (( _this->Mode == OximeterOperationModeMenus ) || ( _this->Mode == OximeterOperationModePulseSettings ))
    {
      OximeterValueDisplay_OnSetIcon( &_this->PercentDisplay, 0 );

      if ( _this->PercentDisplay.HeaderOpacity < _this->FadeInEffect.Value )
        OximeterValueDisplay_OnSetHeaderOpacity( &_this->PercentDisplay, _this->FadeInEffect.Value );
    }

  if (( _this->Mode == OximeterOperationModeOverview ) && ( _this->AnalogDisplay.Super1.Opacity 
      < _this->FadeInEffect.Value ))
    CoreGroup__OnSetOpacity( &_this->AnalogDisplay, _this->FadeInEffect.Value );

  if (( _this->Mode == OximeterOperationModeOxygenSettings ) && ( _this->AnalogDisplay.Super1.Opacity 
      < ( _this->FadeInEffect.Value - 128 )))
    CoreGroup__OnSetOpacity( &_this->AnalogDisplay, _this->FadeInEffect.Value - 
    128 );

  if (( _this->Mode != OximeterOperationModeOxygenSettings ) && ( _this->PercentDisplay.Super1.Opacity 
      < _this->FadeInEffect.Value ))
    CoreGroup__OnSetOpacity( &_this->PercentDisplay, _this->FadeInEffect.Value );

  if (( _this->Mode == OximeterOperationModeOverview ) && ( _this->TextRight.Opacity 
      < _this->FadeInEffect.Value ))
    ViewsText_OnSetOpacity( &_this->TextRight, _this->FadeInEffect.Value );

  if (( _this->Mode == OximeterOperationModeOxygenSettings ) && ( _this->TextTop.Opacity 
      < ( _this->FadeInEffect.Value - 128 )))
    ViewsText_OnSetOpacity( &_this->TextTop, _this->FadeInEffect.Value - 128 );

  if ((( _this->Mode == OximeterOperationModeMenus ) || ( _this->Mode == OximeterOperationModePulseSettings )) 
      && ( _this->IconTop.Opacity < _this->FadeInEffect.Value ))
    ViewsImage_OnSetOpacity( &_this->IconTop, _this->FadeInEffect.Value );

  if (( _this->Mode == OximeterOperationModeGraphs ) && ( _this->IconLeft.Opacity 
      < _this->FadeInEffect.Value ))
    ViewsImage_OnSetOpacity( &_this->IconLeft, _this->FadeInEffect.Value );

  if ( _this->Mode == OximeterOperationModeOverview )
  {
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerAdjust, 1 );
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerChart, 1 );
  }

  if ( _this->Mode == OximeterOperationModeGraphs )
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerOverviewTop, 
    1 );

  if (( _this->Mode == OximeterOperationModeMenus ) || ( _this->Mode == OximeterOperationModePulseSettings ))
    CoreSimpleTouchHandler_OnSetEnabled( &_this->SimpleTouchHandlerOverviewSide, 
    1 );
}

/* 'C' function for method : 'Oximeter::OxygenPanel.finished()' */
void OximeterOxygenPanel_finished( OximeterOxygenPanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Mode == OximeterOperationModeOverview ) || ( _this->Mode == OximeterOperationModeOxygenSettings ))
    CoreGroup_OnSetEnabled((CoreGroup)&_this->AnalogDisplay, 1 );

  if ( _this->Mode == OximeterOperationModeOverview )
    CoreGroup_OnSetEnabled((CoreGroup)&_this->Plotter, 1 );
}

/* 'C' function for method : 'Oximeter::OxygenPanel.OnSetOnChart()' */
void OximeterOxygenPanel_OnSetOnChart( OximeterOxygenPanel _this, XSlot value )
{
  if ( !EwCompSlot( _this->OnChart, value ))
    return;

  _this->OnChart = value;
  _this->SimpleTouchHandlerChart.OnPress = value;
}

/* 'C' function for method : 'Oximeter::OxygenPanel.OnSetOnAdjust()' */
void OximeterOxygenPanel_OnSetOnAdjust( OximeterOxygenPanel _this, XSlot value )
{
  if ( !EwCompSlot( _this->OnAdjust, value ))
    return;

  _this->OnAdjust = value;
  _this->SimpleTouchHandlerAdjust.OnPress = value;
}

/* 'C' function for method : 'Oximeter::OxygenPanel.OnSetOnOverview()' */
void OximeterOxygenPanel_OnSetOnOverview( OximeterOxygenPanel _this, XSlot value )
{
  if ( !EwCompSlot( _this->OnOverview, value ))
    return;

  _this->OnOverview = value;
  _this->SimpleTouchHandlerOverviewSide.OnPress = value;
  _this->SimpleTouchHandlerOverviewTop.OnPress = value;
}

/* Variants derived from the class : 'Oximeter::OxygenPanel' */
EW_DEFINE_CLASS_VARIANTS( OximeterOxygenPanel )
EW_END_OF_CLASS_VARIANTS( OximeterOxygenPanel )

/* Virtual Method Table (VMT) for the class : 'Oximeter::OxygenPanel' */
EW_DEFINE_CLASS( OximeterOxygenPanel, CoreGroup, OnChart, OnChart, RectEffect, RectEffect, 
                 Mode, Mode, "Oximeter::OxygenPanel" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterOxygenPanel )

/* Initializer for the class 'Oximeter::DeviceClass' */
void OximeterDeviceClass__Init( OximeterDeviceClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  TemplatesDeviceClass__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterDeviceClass );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->Timer, &_this->_XObject, 0 );
  OximeterDataStorage__Init( &_this->PulseStorage, &_this->_XObject, 0 );
  OximeterDataStorage__Init( &_this->OxygenStorage, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterDeviceClass );

  /* ... and initialize objects, variables, properties, etc. */
  _this->MaxPulse = 130;
  _this->MinPulse = 50;
  _this->MaxOxygen = 99;
  _this->MinOxygen = 85;
  CoreTimer_OnSetPeriod( &_this->Timer, 1000 );
  CoreTimer_OnSetEnabled( &_this->Timer, 1 );
  _this->simulationPulse = 75;
  _this->simulationOxygen = 95;
  _this->AlarmTime = 60;
  _this->OxygenFlow = 60;
  _this->Timer.OnTrigger = EwNewSlot( _this, OximeterDeviceClass_Simulation );

  /* Call the user defined constructor */
  OximeterDeviceClass_Init( _this, aArg );
}

/* Re-Initializer for the class 'Oximeter::DeviceClass' */
void OximeterDeviceClass__ReInit( OximeterDeviceClass _this )
{
  /* At first re-initialize the super class ... */
  TemplatesDeviceClass__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->Timer );
  OximeterDataStorage__ReInit( &_this->PulseStorage );
  OximeterDataStorage__ReInit( &_this->OxygenStorage );
}

/* Finalizer method for the class 'Oximeter::DeviceClass' */
void OximeterDeviceClass__Done( OximeterDeviceClass _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( TemplatesDeviceClass );

  /* Finalize all embedded objects */
  CoreTimer__Done( &_this->Timer );
  OximeterDataStorage__Done( &_this->PulseStorage );
  OximeterDataStorage__Done( &_this->OxygenStorage );

  /* Don't forget to deinitialize the super class ... */
  TemplatesDeviceClass__Done( &_this->_Super );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void OximeterDeviceClass_UpdatePulse( OximeterDeviceClass _this, XInt32 aNewValue )
{
  if ( aNewValue > 250 )
    aNewValue = 250;

  if ( aNewValue < 0 )
    aNewValue = 0;

  OximeterDataStorage_AddData( &_this->PulseStorage, (XFloat)aNewValue );

  if ( aNewValue != _this->CurrentPulse )
  {
    _this->CurrentPulse = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, OximeterDeviceClass_OnGetCurrentPulse, 
      OximeterDeviceClass_OnSetCurrentPulse ), 0 );
  }
}

/* Wrapper function for the non virtual method : 'Oximeter::DeviceClass.UpdatePulse()' */
void OximeterDeviceClass__UpdatePulse( void* _this, XInt32 aNewValue )
{
  OximeterDeviceClass_UpdatePulse((OximeterDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void OximeterDeviceClass_UpdateOxygen( OximeterDeviceClass _this, XInt32 aNewValue )
{
  if ( aNewValue > 99 )
    aNewValue = 99;

  if ( aNewValue < 0 )
    aNewValue = 0;

  OximeterDataStorage_AddData( &_this->OxygenStorage, (XFloat)aNewValue );

  if ( aNewValue != _this->CurrentOxygen )
  {
    _this->CurrentOxygen = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, OximeterDeviceClass_OnGetCurrentOxygen, 
      OximeterDeviceClass_OnSetCurrentOxygen ), 0 );
  }
}

/* Wrapper function for the non virtual method : 'Oximeter::DeviceClass.UpdateOxygen()' */
void OximeterDeviceClass__UpdateOxygen( void* _this, XInt32 aNewValue )
{
  OximeterDeviceClass_UpdateOxygen((OximeterDeviceClass)_this, aNewValue );
}

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetMaxPulse()' */
void OximeterDeviceClass_OnSetMaxPulse( OximeterDeviceClass _this, XInt32 value )
{
  if ( _this->MaxPulse == value )
    return;

  _this->MaxPulse = value;
  {
    /*
       TO DO:

       You can call a function of your own device API or you simply
       modify a variable existing in your middleware to reflect the
       new value:

       YourDevice_SetSomeValue( value );

       or

       YourDevice_SomeVariable = value;
    */
  }
  EwNotifyRefObservers( EwNewRef( _this, OximeterDeviceClass_OnGetMaxPulse, OximeterDeviceClass_OnSetMaxPulse ), 
    0 );
}

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetMinPulse()' */
void OximeterDeviceClass_OnSetMinPulse( OximeterDeviceClass _this, XInt32 value )
{
  if ( _this->MinPulse == value )
    return;

  _this->MinPulse = value;
  {
    /*
       TO DO:

       You can call a function of your own device API or you simply
       modify a variable existing in your middleware to reflect the
       new value:

       YourDevice_SetSomeValue( value );

       or

       YourDevice_SomeVariable = value;
    */
  }
  EwNotifyRefObservers( EwNewRef( _this, OximeterDeviceClass_OnGetMinPulse, OximeterDeviceClass_OnSetMinPulse ), 
    0 );
}

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetMaxOxygen()' */
void OximeterDeviceClass_OnSetMaxOxygen( OximeterDeviceClass _this, XInt32 value )
{
  if ( _this->MaxOxygen == value )
    return;

  _this->MaxOxygen = value;
  {
    /*
       TO DO:

       You can call a function of your own device API or you simply
       modify a variable existing in your middleware to reflect the
       new value:

       YourDevice_SetSomeValue( value );

       or

       YourDevice_SomeVariable = value;
    */
  }
  EwNotifyRefObservers( EwNewRef( _this, OximeterDeviceClass_OnGetMaxOxygen, OximeterDeviceClass_OnSetMaxOxygen ), 
    0 );
}

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetMinOxygen()' */
void OximeterDeviceClass_OnSetMinOxygen( OximeterDeviceClass _this, XInt32 value )
{
  if ( _this->MinOxygen == value )
    return;

  _this->MinOxygen = value;
  {
    /*
       TO DO:

       You can call a function of your own device API or you simply
       modify a variable existing in your middleware to reflect the
       new value:

       YourDevice_SetSomeValue( value );

       or

       YourDevice_SomeVariable = value;
    */
  }
  EwNotifyRefObservers( EwNewRef( _this, OximeterDeviceClass_OnGetMinOxygen, OximeterDeviceClass_OnSetMinOxygen ), 
    0 );
}

/* 'C' function for method : 'Oximeter::DeviceClass.Simulation()' */
void OximeterDeviceClass_Simulation( OximeterDeviceClass _this, XObject sender )
{
  XInt32 up;
  XInt32 dn;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  up = _this->simulationPulse + (( 120 - _this->simulationPulse ) / 2 );
  dn = _this->simulationPulse - (( _this->simulationPulse - 60 ) / 2 );
  _this->simulationPulse = EwMathRandInt32( dn, up );
  OximeterDeviceClass_UpdatePulse( _this, _this->simulationPulse );
  up = _this->simulationOxygen + (( 99 - _this->simulationOxygen ) / 2 );
  dn = _this->simulationOxygen - (( _this->simulationOxygen - 85 ) / 2 );
  _this->simulationOxygen = EwMathRandInt32( dn, up );
  OximeterDeviceClass_UpdateOxygen( _this, _this->simulationOxygen );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void OximeterDeviceClass_Init( OximeterDeviceClass _this, XHandle aArg )
{
  XInt32 i;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  for ( i = 0; i < 1000; i = i + 1 )
  {
    EwSignal( EwNewSlot( _this, OximeterDeviceClass_Simulation ), ((XObject)_this ));
  }
}

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetAlarmTime()' */
void OximeterDeviceClass_OnSetAlarmTime( OximeterDeviceClass _this, XInt32 value )
{
  if ( _this->AlarmTime == value )
    return;

  _this->AlarmTime = value;
  {
    /*
       TO DO:

       You can call a function of your own device API or you simply
       modify a variable existing in your middleware to reflect the
       new value:

       YourDevice_SetSomeValue( value );

       or

       YourDevice_SomeVariable = value;
    */
  }
  EwNotifyRefObservers( EwNewRef( _this, OximeterDeviceClass_OnGetAlarmTime, OximeterDeviceClass_OnSetAlarmTime ), 
    0 );
}

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetOxygenFlow()' */
void OximeterDeviceClass_OnSetOxygenFlow( OximeterDeviceClass _this, XInt32 value )
{
  if ( _this->OxygenFlow == value )
    return;

  _this->OxygenFlow = value;
  {
    /*
       TO DO:

       You can call a function of your own device API or you simply
       modify a variable existing in your middleware to reflect the
       new value:

       YourDevice_SetSomeValue( value );

       or

       YourDevice_SomeVariable = value;
    */
  }
  EwNotifyRefObservers( EwNewRef( _this, OximeterDeviceClass_OnGetOxygenFlow, OximeterDeviceClass_OnSetOxygenFlow ), 
    0 );
}

/* Default onget method for the property 'CurrentPulse' */
XInt32 OximeterDeviceClass_OnGetCurrentPulse( OximeterDeviceClass _this )
{
  return _this->CurrentPulse;
}

/* Default onset method for the property 'CurrentPulse' */
void OximeterDeviceClass_OnSetCurrentPulse( OximeterDeviceClass _this, XInt32 value )
{
  _this->CurrentPulse = value;
}

/* Default onget method for the property 'CurrentOxygen' */
XInt32 OximeterDeviceClass_OnGetCurrentOxygen( OximeterDeviceClass _this )
{
  return _this->CurrentOxygen;
}

/* Default onset method for the property 'CurrentOxygen' */
void OximeterDeviceClass_OnSetCurrentOxygen( OximeterDeviceClass _this, XInt32 value )
{
  _this->CurrentOxygen = value;
}

/* Default onget method for the property 'MaxPulse' */
XInt32 OximeterDeviceClass_OnGetMaxPulse( OximeterDeviceClass _this )
{
  return _this->MaxPulse;
}

/* Default onget method for the property 'MinPulse' */
XInt32 OximeterDeviceClass_OnGetMinPulse( OximeterDeviceClass _this )
{
  return _this->MinPulse;
}

/* Default onget method for the property 'MaxOxygen' */
XInt32 OximeterDeviceClass_OnGetMaxOxygen( OximeterDeviceClass _this )
{
  return _this->MaxOxygen;
}

/* Default onget method for the property 'MinOxygen' */
XInt32 OximeterDeviceClass_OnGetMinOxygen( OximeterDeviceClass _this )
{
  return _this->MinOxygen;
}

/* Default onget method for the property 'AlarmTime' */
XInt32 OximeterDeviceClass_OnGetAlarmTime( OximeterDeviceClass _this )
{
  return _this->AlarmTime;
}

/* Default onget method for the property 'OxygenFlow' */
XInt32 OximeterDeviceClass_OnGetOxygenFlow( OximeterDeviceClass _this )
{
  return _this->OxygenFlow;
}

/* Variants derived from the class : 'Oximeter::DeviceClass' */
EW_DEFINE_CLASS_VARIANTS( OximeterDeviceClass )
EW_END_OF_CLASS_VARIANTS( OximeterDeviceClass )

/* Virtual Method Table (VMT) for the class : 'Oximeter::DeviceClass' */
EW_DEFINE_CLASS( OximeterDeviceClass, TemplatesDeviceClass, Timer, Timer, Timer, 
                 Timer, CurrentPulse, CurrentPulse, "Oximeter::DeviceClass" )
EW_END_OF_CLASS( OximeterDeviceClass )

/* User defined auto object: 'Oximeter::Device' */
EW_DEFINE_AUTOOBJECT( OximeterDevice, OximeterDeviceClass )

/* Initializer for the auto object 'Oximeter::Device' */
void OximeterDevice__Init( OximeterDeviceClass _this )
{
  EW_UNUSED_ARG( _this );
}

/* Table with links to derived variants of the auto object : 'Oximeter::Device' */
EW_DEFINE_AUTOOBJECT_VARIANTS( OximeterDevice )
EW_END_OF_AUTOOBJECT_VARIANTS( OximeterDevice )

/* Include a file containing the bitmap resource : 'Oximeter::OxygenGradient' */
#include "_OximeterOxygenGradient.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::OxygenGradient' */
EW_RES_WITHOUT_VARIANTS( OximeterOxygenGradient )

/* Include a file containing the bitmap resource : 'Oximeter::PulseGradient' */
#include "_OximeterPulseGradient.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::PulseGradient' */
EW_RES_WITHOUT_VARIANTS( OximeterPulseGradient )

/* Initializer for the class 'Oximeter::Plotter' */
void OximeterPlotter__Init( OximeterPlotter _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterPlotter );

  /* ... then construct all embedded objects */
  ViewsStrokePath__Init( &_this->StrokePath, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->Path, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->MaxRangeLine, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->MaxValueLine, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->MinValueLine, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->MinRangeLine, &_this->_XObject, 0 );
  ViewsText__Init( &_this->MaxValueText, &_this->_XObject, 0 );
  ViewsText__Init( &_this->MinValueText, &_this->_XObject, 0 );
  CorePropertyObserver__Init( &_this->DataObserver, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->Line1, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->Line2, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->Line3, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->Line4, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->Line5, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterPlotter );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const003B );
  CoreView_OnSetLayout((CoreView)&_this->StrokePath, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->StrokePath, _Const003C );
  ViewsStrokePath_OnSetColorBL( &_this->StrokePath, _Const003D );
  ViewsStrokePath_OnSetColorTL( &_this->StrokePath, _Const003D );
  ViewsStrokePath_OnSetFlipY( &_this->StrokePath, 1 );
  ViewsStrokePath_OnSetWidth( &_this->StrokePath, 3.020000f );
  _this->LineWidth = 3.000000f;
  _this->MinValue = 25;
  _this->MaxValue = 120;
  CoreLineView_OnSetPoint2((CoreLineView)&_this->MaxRangeLine, _Const003E );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->MaxRangeLine, _Const003F );
  ViewsLine_OnSetColor( &_this->MaxRangeLine, _Const0002 );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->MaxValueLine, _Const0040 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->MaxValueLine, _Const0041 );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->MinValueLine, _Const0042 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->MinValueLine, _Const0043 );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->MinRangeLine, _Const0044 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->MinRangeLine, _Const0045 );
  ViewsLine_OnSetColor( &_this->MinRangeLine, _Const0002 );
  CoreView_OnSetLayout((CoreView)&_this->MaxValueText, CoreLayoutAlignToLeft | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->MaxValueText, _Const0046 );
  ViewsText_OnSetAlignment( &_this->MaxValueText, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->MaxValueText, EwLoadString( &_Const0047 ));
  CoreView_OnSetLayout((CoreView)&_this->MinValueText, CoreLayoutAlignToLeft | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->MinValueText, _Const0048 );
  ViewsText_OnSetAlignment( &_this->MinValueText, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->MinValueText, EwLoadString( &_Const0049 ));
  _this->MaxRange = 250;
  _this->PixelPerValue = 20;
  CoreLineView_OnSetPoint2((CoreLineView)&_this->Line1, _Const004A );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->Line1, _Const004B );
  ViewsLine_OnSetColor( &_this->Line1, _Const0002 );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->Line2, _Const004C );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->Line2, _Const004D );
  ViewsLine_OnSetColor( &_this->Line2, _Const0002 );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->Line3, _Const004E );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->Line3, _Const004F );
  ViewsLine_OnSetColor( &_this->Line3, _Const0002 );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->Line4, _Const0050 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->Line4, _Const0051 );
  ViewsLine_OnSetColor( &_this->Line4, _Const0002 );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->Line5, _Const0052 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->Line5, _Const0053 );
  ViewsLine_OnSetColor( &_this->Line5, _Const0002 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->StrokePath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MaxRangeLine ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MaxValueLine ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MinValueLine ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MinRangeLine ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MaxValueText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MinValueText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Line1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Line2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Line3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Line4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Line5 ), 0 );
  ViewsStrokePath_OnSetPath( &_this->StrokePath, &_this->Path );
  ViewsText_OnSetFont( &_this->MaxValueText, EwLoadResource( &OximeterFontM, ResourcesFont ));
  ViewsText_OnSetFont( &_this->MinValueText, EwLoadResource( &OximeterFontM, ResourcesFont ));
  _this->DataObserver.OnEvent = EwNewSlot( _this, OximeterPlotter_onDataAdded );
  CorePropertyObserver_OnSetOutlet( &_this->DataObserver, EwNullRef );
}

/* Re-Initializer for the class 'Oximeter::Plotter' */
void OximeterPlotter__ReInit( OximeterPlotter _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsStrokePath__ReInit( &_this->StrokePath );
  GraphicsPath__ReInit( &_this->Path );
  ViewsLine__ReInit( &_this->MaxRangeLine );
  ViewsLine__ReInit( &_this->MaxValueLine );
  ViewsLine__ReInit( &_this->MinValueLine );
  ViewsLine__ReInit( &_this->MinRangeLine );
  ViewsText__ReInit( &_this->MaxValueText );
  ViewsText__ReInit( &_this->MinValueText );
  CorePropertyObserver__ReInit( &_this->DataObserver );
  ViewsLine__ReInit( &_this->Line1 );
  ViewsLine__ReInit( &_this->Line2 );
  ViewsLine__ReInit( &_this->Line3 );
  ViewsLine__ReInit( &_this->Line4 );
  ViewsLine__ReInit( &_this->Line5 );
}

/* Finalizer method for the class 'Oximeter::Plotter' */
void OximeterPlotter__Done( OximeterPlotter _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsStrokePath__Done( &_this->StrokePath );
  GraphicsPath__Done( &_this->Path );
  ViewsLine__Done( &_this->MaxRangeLine );
  ViewsLine__Done( &_this->MaxValueLine );
  ViewsLine__Done( &_this->MinValueLine );
  ViewsLine__Done( &_this->MinRangeLine );
  ViewsText__Done( &_this->MaxValueText );
  ViewsText__Done( &_this->MinValueText );
  CorePropertyObserver__Done( &_this->DataObserver );
  ViewsLine__Done( &_this->Line1 );
  ViewsLine__Done( &_this->Line2 );
  ViewsLine__Done( &_this->Line3 );
  ViewsLine__Done( &_this->Line4 );
  ViewsLine__Done( &_this->Line5 );

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
void OximeterPlotter_UpdateLayout( OximeterPlotter _this, XPoint aSize )
{
  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );
  OximeterPlotter_arrangeMinMax( _this );
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
void OximeterPlotter_UpdateViewState( OximeterPlotter _this, XSet aState )
{
  XInt32 inx;
  XInt32 x;
  XBool firstRun;
  XInt32 noOfEdges;
  XInt32 current;
  XInt32 noOfData;
  XFloat data;
  XFloat slope;
  XFloat lastData;
  XFloat lastSlope;
  XInt32 dx;
  XFloat ratio;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );

  if (( _this->DataStorage == 0 ) || ( _this->DataStorage->NoOfData < 2 ))
    return;

  firstRun = 1;
  noOfEdges = ( _this->PixelPerValue / 4 ) + 1;
  current = _this->DataStorage->Current;
  noOfData = _this->DataStorage->NoOfData;
  inx = ( EwGetRectW( _this->StrokePath.Super1.Bounds ) / _this->PixelPerValue ) 
  + 1;
  x = ( EwGetRectW( _this->StrokePath.Super1.Bounds ) % _this->PixelPerValue ) - 
  _this->PixelPerValue;
  GraphicsPath_InitSubPath( &_this->Path, 0, ( noOfEdges * inx ) + 4 );
  lastData = 0.000000f;
  lastSlope = 0.000000f;
  dx = _this->PixelPerValue / 2;
  ratio = 0.250000f;

  while (( x < ( EwGetRectW( _this->StrokePath.Super1.Bounds ) + _this->PixelPerValue )) 
         && ( inx >= 0 ))
  {
    if ( inx < noOfData )
    {
      data = OximeterDataStorage_GetData( _this->DataStorage, current - inx );
      slope = OximeterDataStorage_GetSlope( _this->DataStorage, current - inx );
      data = ((XFloat)EwGetRectH( _this->StrokePath.Super1.Bounds ) * ( data - (XFloat)_this->MinRange )) 
      / (XFloat)( _this->MaxRange - _this->MinRange );

      if ( firstRun == 1 )
      {
        GraphicsPath_Begin( &_this->Path, 0, (XFloat)( x - (XInt32)_this->LineWidth ), 
        (XFloat)( EwGetRectH( _this->StrokePath.Super1.Bounds ) / 2 ));
        GraphicsPath_AddLine( &_this->Path, 0, (XFloat)x, data );
        firstRun = 0;
      }
      else
      {
        GraphicsPath_AddBezier3( &_this->Path, 0, (XFloat)( x - dx ), lastData + 
        ( lastSlope * ratio ), (XFloat)( x - dx ), data - ( slope * ratio ), (XFloat)x, 
        data, noOfEdges );
      }

      lastSlope = slope;
      lastData = data;
    }

    x = x + _this->PixelPerValue;
    inx = inx - 1;
  }

  GraphicsPath_AddLine( &_this->Path, 0, (XFloat)( x + (XInt32)_this->LineWidth ), 
  (XFloat)( EwGetRectH( _this->StrokePath.Super1.Bounds ) / 2 ));
}

/* 'C' function for method : 'Oximeter::Plotter.OnSetMinValue()' */
void OximeterPlotter_OnSetMinValue( OximeterPlotter _this, XInt32 value )
{
  if ( _this->MinValue == value )
    return;

  _this->MinValue = value;
  OximeterPlotter_arrangeMinMax( _this );
}

/* 'C' function for method : 'Oximeter::Plotter.OnSetMaxValue()' */
void OximeterPlotter_OnSetMaxValue( OximeterPlotter _this, XInt32 value )
{
  if ( _this->MaxValue == value )
    return;

  _this->MaxValue = value;
  OximeterPlotter_arrangeMinMax( _this );
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMin has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterPlotter_outletMinSlot( OximeterPlotter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->OutletMin.Object != 0 )
    OximeterPlotter_OnSetMinValue( _this, EwOnGetInt32( _this->OutletMin ));
}

/* 'C' function for method : 'Oximeter::Plotter.OnSetOutletMin()' */
void OximeterPlotter_OnSetOutletMin( OximeterPlotter _this, XRef value )
{
  if ( !EwCompRef( _this->OutletMin, value ))
    return;

  if ( _this->OutletMin.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, OximeterPlotter_outletMinSlot ), _this->OutletMin, 
      0 );

  _this->OutletMin = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, OximeterPlotter_outletMinSlot ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, OximeterPlotter_outletMinSlot ), ((XObject)_this ));
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMax has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterPlotter_outletMaxSlot( OximeterPlotter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->OutletMax.Object != 0 )
    OximeterPlotter_OnSetMaxValue( _this, EwOnGetInt32( _this->OutletMax ));
}

/* 'C' function for method : 'Oximeter::Plotter.OnSetOutletMax()' */
void OximeterPlotter_OnSetOutletMax( OximeterPlotter _this, XRef value )
{
  if ( !EwCompRef( _this->OutletMax, value ))
    return;

  if ( _this->OutletMax.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, OximeterPlotter_outletMaxSlot ), _this->OutletMax, 
      0 );

  _this->OutletMax = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, OximeterPlotter_outletMaxSlot ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, OximeterPlotter_outletMaxSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'Oximeter::Plotter.OnSetMinRange()' */
void OximeterPlotter_OnSetMinRange( OximeterPlotter _this, XInt32 value )
{
  if ( _this->MinRange == value )
    return;

  _this->MinRange = value;
  OximeterPlotter_arrangeMinMax( _this );
}

/* 'C' function for method : 'Oximeter::Plotter.OnSetMaxRange()' */
void OximeterPlotter_OnSetMaxRange( OximeterPlotter _this, XInt32 value )
{
  if ( _this->MaxRange == value )
    return;

  _this->MaxRange = value;
  OximeterPlotter_arrangeMinMax( _this );
}

/* 'C' function for method : 'Oximeter::Plotter.arrangeMinMax()' */
void OximeterPlotter_arrangeMinMax( OximeterPlotter _this )
{
  XInt32 y;

  if (((( _this->MaxRange > _this->MinRange ) && ( _this->MaxValue > _this->MinValue )) 
      && ( _this->MinValue >= _this->MinRange )) && ( _this->MaxValue <= _this->MaxRange ))
  {
    y = _this->StrokePath.Super1.Bounds.Point2.Y - (( EwGetRectH( _this->StrokePath.Super1.Bounds ) 
    * ( _this->MaxValue - _this->MinRange )) / ( _this->MaxRange - _this->MinRange ));
    CoreLineView_OnSetPoint1((CoreLineView)&_this->MaxValueLine, EwSetPointY( _this->MaxValueLine.Super1.Point1, 
    y ));
    CoreLineView_OnSetPoint2((CoreLineView)&_this->MaxValueLine, EwSetPointY( _this->MaxValueLine.Super1.Point2, 
    y ));
    y = y - ( EwGetRectH( _this->MaxValueText.Super1.Bounds ) / 2 );

    if ( y < 0 )
      y = 0;

    CoreRectView__OnSetBounds( &_this->MaxValueText, EwSetRectY( _this->MaxValueText.Super1.Bounds, 
    y ));
    ViewsText_OnSetString( &_this->MaxValueText, EwNewStringInt( _this->MaxValue, 
    0, 10 ));
    y = _this->StrokePath.Super1.Bounds.Point2.Y - (( EwGetRectH( _this->StrokePath.Super1.Bounds ) 
    * ( _this->MinValue - _this->MinRange )) / ( _this->MaxRange - _this->MinRange ));
    CoreLineView_OnSetPoint1((CoreLineView)&_this->MinValueLine, EwSetPointY( _this->MinValueLine.Super1.Point1, 
    y ));
    CoreLineView_OnSetPoint2((CoreLineView)&_this->MinValueLine, EwSetPointY( _this->MinValueLine.Super1.Point2, 
    y ));
    y = y + ( EwGetRectH( _this->MinValueText.Super1.Bounds ) / 2 );

    if ( y > EwGetRectH( _this->Super2.Bounds ))
      y = EwGetRectH( _this->Super2.Bounds );

    CoreRectView__OnSetBounds( &_this->MinValueText, EwSetRectY( _this->MinValueText.Super1.Bounds, 
    y - EwGetRectH( _this->MinValueText.Super1.Bounds )));
    ViewsText_OnSetString( &_this->MinValueText, EwNewStringInt( _this->MinValue, 
    0, 10 ));
  }
  else
  {
    ViewsText_OnSetString( &_this->MinValueText, 0 );
    ViewsText_OnSetString( &_this->MaxValueText, 0 );
    CoreLineView_OnSetPoint1((CoreLineView)&_this->MinValueLine, EwSetPointY( _this->MinValueLine.Super1.Point1, 
    _this->MinRangeLine.Super1.Point1.Y ));
    CoreLineView_OnSetPoint2((CoreLineView)&_this->MinValueLine, EwSetPointY( _this->MinValueLine.Super1.Point2, 
    _this->MinRangeLine.Super1.Point2.Y ));
    CoreLineView_OnSetPoint1((CoreLineView)&_this->MaxValueLine, EwSetPointY( _this->MaxValueLine.Super1.Point1, 
    _this->MaxRangeLine.Super1.Point1.Y ));
    CoreLineView_OnSetPoint2((CoreLineView)&_this->MaxValueLine, EwSetPointY( _this->MaxValueLine.Super1.Point2, 
    _this->MaxRangeLine.Super1.Point2.Y ));
  }
}

/* 'C' function for method : 'Oximeter::Plotter.OnSetDataStorage()' */
void OximeterPlotter_OnSetDataStorage( OximeterPlotter _this, OximeterDataStorage 
  value )
{
  if ( _this->DataStorage == value )
    return;

  _this->DataStorage = value;
  CorePropertyObserver_OnSetOutlet( &_this->DataObserver, EwNewRef( value, OximeterDataStorage_OnGetCurrent, 
  OximeterDataStorage_OnSetCurrent ));
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This slot method is executed when the associated property observer 'DataObserver' 
   is notified. */
void OximeterPlotter_onDataAdded( OximeterPlotter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* Variants derived from the class : 'Oximeter::Plotter' */
EW_DEFINE_CLASS_VARIANTS( OximeterPlotter )
EW_END_OF_CLASS_VARIANTS( OximeterPlotter )

/* Virtual Method Table (VMT) for the class : 'Oximeter::Plotter' */
EW_DEFINE_CLASS( OximeterPlotter, CoreGroup, DataStorage, OutletMin, OutletMin, 
                 StrokePath, LineWidth, LineWidth, "Oximeter::Plotter" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  OximeterPlotter_UpdateLayout,
  OximeterPlotter_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterPlotter )

/* Initializer for the class 'Oximeter::DataStorage' */
void OximeterDataStorage__Init( OximeterDataStorage _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterDataStorage );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterDataStorage );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Current = -1;
}

/* Re-Initializer for the class 'Oximeter::DataStorage' */
void OximeterDataStorage__ReInit( OximeterDataStorage _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Oximeter::DataStorage' */
void OximeterDataStorage__Done( OximeterDataStorage _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Oximeter::DataStorage.OnSetCurrent()' */
void OximeterDataStorage_OnSetCurrent( OximeterDataStorage _this, XInt32 value )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( value );

  EwTrace( "%s", EwLoadString( &_Const0054 ));
}

/* 'C' function for method : 'Oximeter::DataStorage.AddData()' */
void OximeterDataStorage_AddData( OximeterDataStorage _this, XFloat aData )
{
  XInt32 last = _this->Current;
  XInt32 prelast;

  _this->Current = _this->Current + 1;

  if ( _this->Current >= 1000 )
    _this->Current = 0;

  _this->Data[ EwCheckIndex( _this->Current, 1000 )] = aData;
  _this->Slope[ EwCheckIndex( _this->Current, 1000 )] = 0.000000f;

  if ( _this->NoOfData < 1000 )
    _this->NoOfData = _this->NoOfData + 1;

  prelast = last - 1;

  if ( prelast < 0 )
    prelast = _this->NoOfData - 1;

  if ( _this->NoOfData > 2 )
    _this->Slope[ EwCheckIndex( last, 1000 )] = ( aData - _this->Data[ EwCheckIndex( 
    prelast, 1000 )]) / 2.000000f;

  EwNotifyRefObservers( EwNewRef( _this, OximeterDataStorage_OnGetCurrent, OximeterDataStorage_OnSetCurrent ), 
    0 );
}

/* 'C' function for method : 'Oximeter::DataStorage.GetData()' */
XFloat OximeterDataStorage_GetData( OximeterDataStorage _this, XInt32 aIndex )
{
  XInt32 i = aIndex;

  if ( i < 0 )
    i = i + 1000;

  if (( i < 0 ) || ( i >= _this->NoOfData ))
    return 0.000000f;
  else
    return _this->Data[ EwCheckIndex( i, 1000 )];
}

/* 'C' function for method : 'Oximeter::DataStorage.GetSlope()' */
XFloat OximeterDataStorage_GetSlope( OximeterDataStorage _this, XInt32 aIndex )
{
  if (( aIndex < 0 ) || ( aIndex > ( _this->NoOfData - 1 )))
    return 0.000000f;
  else
    return _this->Slope[ EwCheckIndex( aIndex, 1000 )];
}

/* Default onget method for the property 'Current' */
XInt32 OximeterDataStorage_OnGetCurrent( OximeterDataStorage _this )
{
  return _this->Current;
}

/* Variants derived from the class : 'Oximeter::DataStorage' */
EW_DEFINE_CLASS_VARIANTS( OximeterDataStorage )
EW_END_OF_CLASS_VARIANTS( OximeterDataStorage )

/* Virtual Method Table (VMT) for the class : 'Oximeter::DataStorage' */
EW_DEFINE_CLASS( OximeterDataStorage, XObject, _None, _None, _None, _None, _None, 
                 _None, "Oximeter::DataStorage" )
EW_END_OF_CLASS( OximeterDataStorage )

/* Initializer for the class 'Oximeter::Graph' */
void OximeterGraph__Init( OximeterGraph _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterGraph );

  /* ... then construct all embedded objects */
  ViewsFillPath__Init( &_this->FillPath, &_this->_XObject, 0 );
  ViewsStrokePath__Init( &_this->StrokePath, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->Path, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterGraph );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0055 );
  _this->current = 999;
  _this->noOfData = 1000;
  CoreView_OnSetLayout((CoreView)&_this->FillPath, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->FillPath, _Const0055 );
  ViewsFillPath_OnSetColor( &_this->FillPath, _Const0002 );
  ViewsFillPath_OnSetFlipY( &_this->FillPath, 1 );
  CoreView_OnSetLayout((CoreView)&_this->StrokePath, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->StrokePath, _Const0055 );
  ViewsStrokePath_OnSetFlipY( &_this->StrokePath, 1 );
  ViewsStrokePath_OnSetWidth( &_this->StrokePath, 3.000000f );
  _this->LineColorLeft = _Const0003;
  _this->LineWidth = 3.000000f;
  _this->PixelPerValue = 50;
  _this->MaxRange = 250;
  _this->Bezier = 1;
  _this->LineColorRight = _Const0003;
  _this->FillColorTop = _Const0003;
  _this->FillColorBottom = _Const0003;
  _this->Filled = 1;
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->FillPath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->StrokePath ), 0 );
  ViewsFillPath_OnSetPath( &_this->FillPath, &_this->Path );
  ViewsStrokePath_OnSetPath( &_this->StrokePath, &_this->Path );
}

/* Re-Initializer for the class 'Oximeter::Graph' */
void OximeterGraph__ReInit( OximeterGraph _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsFillPath__ReInit( &_this->FillPath );
  ViewsStrokePath__ReInit( &_this->StrokePath );
  GraphicsPath__ReInit( &_this->Path );
}

/* Finalizer method for the class 'Oximeter::Graph' */
void OximeterGraph__Done( OximeterGraph _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsFillPath__Done( &_this->FillPath );
  ViewsStrokePath__Done( &_this->StrokePath );
  GraphicsPath__Done( &_this->Path );

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
void OximeterGraph_UpdateLayout( OximeterGraph _this, XPoint aSize )
{
  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );

  if ( _this->DataStorage != 0 )
  {
    _this->current = _this->DataStorage->Current;
    _this->noOfData = _this->DataStorage->NoOfData;
  }
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
void OximeterGraph_UpdateViewState( OximeterGraph _this, XSet aState )
{
  XInt32 inx;
  XInt32 x;
  XBool firstRun;
  XInt32 noOfEdges;
  XFloat data;
  XFloat slope;
  XFloat lastData;
  XFloat lastSlope;
  XInt32 dx;
  XFloat ratio;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );

  if ((( _this->noOfData < 2 ) || ( _this->ScrollOffset < -EwGetRectW( _this->Super2.Bounds ))) 
      || ( _this->DataStorage == 0 ))
    return;

  firstRun = 1;
  noOfEdges = ( _this->PixelPerValue / 4 ) + 1;
  inx = (( _this->ScrollOffset + EwGetRectW( _this->Super2.Bounds )) / _this->PixelPerValue ) 
  + 1;
  x = (( _this->ScrollOffset + EwGetRectW( _this->Super2.Bounds )) % _this->PixelPerValue ) 
  - _this->PixelPerValue;
  GraphicsPath_InitSubPath( &_this->Path, 0, ( noOfEdges * inx ) + 4 );
  lastData = 0.000000f;
  lastSlope = 0.000000f;
  dx = _this->PixelPerValue / 2;
  ratio = 0.250000f;

  while (( x < ( EwGetRectW( _this->Super2.Bounds ) + _this->PixelPerValue )) && 
         ( inx >= 0 ))
  {
    if ( inx < _this->noOfData )
    {
      data = OximeterDataStorage_GetData( _this->DataStorage, _this->current - inx );
      slope = OximeterDataStorage_GetSlope( _this->DataStorage, _this->current - 
      inx );
      data = ((XFloat)EwGetRectH( _this->StrokePath.Super1.Bounds ) * ( data - (XFloat)_this->MinRange )) 
      / (XFloat)( _this->MaxRange - _this->MinRange );

      if ( firstRun == 1 )
      {
        GraphicsPath_Begin( &_this->Path, 0, (XFloat)( x - (XInt32)_this->LineWidth ), 
        0.000000f );
        GraphicsPath_AddLine( &_this->Path, 0, (XFloat)x, data );
        firstRun = 0;
      }
      else
      {
        if ( _this->Bezier )
          GraphicsPath_AddBezier3( &_this->Path, 0, (XFloat)( x - dx ), lastData 
          + ( lastSlope * ratio ), (XFloat)( x - dx ), data - ( slope * ratio ), 
          (XFloat)x, data, noOfEdges );
        else
          GraphicsPath_AddLine( &_this->Path, 0, (XFloat)x, data );
      }

      lastSlope = slope;
      lastData = data;
    }

    x = x + _this->PixelPerValue;
    inx = inx - 1;
  }

  GraphicsPath_AddLine( &_this->Path, 0, (XFloat)( x + (XInt32)_this->LineWidth ), 
  0.000000f );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetLineColorLeft()' */
void OximeterGraph_OnSetLineColorLeft( OximeterGraph _this, XColor value )
{
  if ( !EwCompColor( value, _this->LineColorLeft ))
    return;

  _this->LineColorLeft = value;
  ViewsStrokePath_OnSetColorTL( &_this->StrokePath, value );
  ViewsStrokePath_OnSetColorBL( &_this->StrokePath, value );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetLineWidth()' */
void OximeterGraph_OnSetLineWidth( OximeterGraph _this, XFloat value )
{
  if ( value < 0.500000f )
    value = 0.500000f;

  if ( value > 10.000000f )
    value = 10.000000f;

  _this->LineWidth = value;
  ViewsStrokePath_OnSetWidth( &_this->StrokePath, value );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetPixelPerValue()' */
void OximeterGraph_OnSetPixelPerValue( OximeterGraph _this, XInt32 value )
{
  if ( value < 1 )
    value = 1;

  if ( value == _this->PixelPerValue )
    return;

  _this->PixelPerValue = value;
  _this->GraphWidth = _this->noOfData * _this->PixelPerValue;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetDataStorage()' */
void OximeterGraph_OnSetDataStorage( OximeterGraph _this, OximeterDataStorage value )
{
  if ( _this->DataStorage == value )
    return;

  _this->DataStorage = value;
  _this->current = _this->DataStorage->Current;
  _this->noOfData = _this->DataStorage->NoOfData;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetScrollOffset()' */
void OximeterGraph_OnSetScrollOffset( OximeterGraph _this, XInt32 value )
{
  if ( _this->ScrollOffset == value )
    return;

  _this->ScrollOffset = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetMinRange()' */
void OximeterGraph_OnSetMinRange( OximeterGraph _this, XInt32 value )
{
  if ( _this->MinRange == value )
    return;

  _this->MinRange = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetMaxRange()' */
void OximeterGraph_OnSetMaxRange( OximeterGraph _this, XInt32 value )
{
  if ( _this->MaxRange == value )
    return;

  _this->MaxRange = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetBezier()' */
void OximeterGraph_OnSetBezier( OximeterGraph _this, XBool value )
{
  if ( _this->Bezier == value )
    return;

  _this->Bezier = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetLineColorRight()' */
void OximeterGraph_OnSetLineColorRight( OximeterGraph _this, XColor value )
{
  if ( !EwCompColor( value, _this->LineColorRight ))
    return;

  _this->LineColorRight = value;
  ViewsStrokePath_OnSetColorTR( &_this->StrokePath, value );
  ViewsStrokePath_OnSetColorBR( &_this->StrokePath, value );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetFillColorTop()' */
void OximeterGraph_OnSetFillColorTop( OximeterGraph _this, XColor value )
{
  if ( !EwCompColor( value, _this->FillColorTop ))
    return;

  _this->FillColorTop = value;
  ViewsFillPath_OnSetColorTL( &_this->FillPath, value );
  ViewsFillPath_OnSetColorTR( &_this->FillPath, value );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetFillColorBottom()' */
void OximeterGraph_OnSetFillColorBottom( OximeterGraph _this, XColor value )
{
  if ( !EwCompColor( value, _this->FillColorBottom ))
    return;

  _this->FillColorBottom = value;
  ViewsFillPath_OnSetColorBL( &_this->FillPath, value );
  ViewsFillPath_OnSetColorBR( &_this->FillPath, value );
}

/* 'C' function for method : 'Oximeter::Graph.OnSetFilled()' */
void OximeterGraph_OnSetFilled( OximeterGraph _this, XBool value )
{
  if ( _this->Filled == value )
    return;

  _this->Filled = value;
  ViewsFillPath_OnSetVisible( &_this->FillPath, value );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Oximeter::Graph.OnGetGraphWidth()' */
XInt32 OximeterGraph_OnGetGraphWidth( OximeterGraph _this )
{
  return _this->noOfData * _this->PixelPerValue;
}

/* Variants derived from the class : 'Oximeter::Graph' */
EW_DEFINE_CLASS_VARIANTS( OximeterGraph )
EW_END_OF_CLASS_VARIANTS( OximeterGraph )

/* Virtual Method Table (VMT) for the class : 'Oximeter::Graph' */
EW_DEFINE_CLASS( OximeterGraph, CoreGroup, DataStorage, FillPath, FillPath, FillPath, 
                 current, current, "Oximeter::Graph" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  OximeterGraph_UpdateLayout,
  OximeterGraph_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterGraph )

/* User defined auto object: 'Oximeter::IconButtonConfig' */
EW_DEFINE_AUTOOBJECT( OximeterIconButtonConfig, WidgetSetPushButtonConfig )

/* Initializer for the auto object 'Oximeter::IconButtonConfig' */
void OximeterIconButtonConfig__Init( WidgetSetPushButtonConfig _this )
{
  WidgetSetPushButtonConfig_OnSetIconTintActive( _this, _Const0003 );
  WidgetSetPushButtonConfig_OnSetIconTintDefault( _this, _Const0002 );
  WidgetSetPushButtonConfig_OnSetWidgetMaxSize( _this, _Const0056 );
  WidgetSetPushButtonConfig_OnSetWidgetMinSize( _this, _Const0056 );
}

/* Table with links to derived variants of the auto object : 'Oximeter::IconButtonConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( OximeterIconButtonConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( OximeterIconButtonConfig )

/* Include a file containing the bitmap resource : 'Oximeter::IconButton' */
#include "_OximeterIconButton.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::IconButton' */
EW_RES_WITHOUT_VARIANTS( OximeterIconButton )

/* Include a file containing the bitmap resource : 'Oximeter::RotaryBackground' */
#include "_OximeterRotaryBackground.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::RotaryBackground' */
EW_RES_WITHOUT_VARIANTS( OximeterRotaryBackground )

/* Include a file containing the bitmap resource : 'Oximeter::RotaryThumbMax' */
#include "_OximeterRotaryThumbMax.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::RotaryThumbMax' */
EW_RES_WITHOUT_VARIANTS( OximeterRotaryThumbMax )

/* User defined auto object: 'Oximeter::RotaryMaxConfig' */
EW_DEFINE_AUTOOBJECT( OximeterRotaryMaxConfig, WidgetSetRotaryKnobConfig )

/* Initializer for the auto object 'Oximeter::RotaryMaxConfig' */
void OximeterRotaryMaxConfig__Init( WidgetSetRotaryKnobConfig _this )
{
  WidgetSetRotaryKnobConfig_OnSetThumbMaxAngle( _this, 2.000000f );
  WidgetSetRotaryKnobConfig_OnSetThumbMinAngle( _this, 270.000000f );
  WidgetSetRotaryKnobConfig_OnSetThumbPivot( _this, _Const0057 );
  WidgetSetRotaryKnobConfig_OnSetThumbActive( _this, EwLoadResource( &OximeterRotaryThumbMax, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetThumbFocused( _this, EwLoadResource( &OximeterRotaryThumbMax, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetThumbDefault( _this, EwLoadResource( &OximeterRotaryThumbMax, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleActive( _this, EwLoadResource( &OximeterRotaryBackground, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleFocused( _this, EwLoadResource( &OximeterRotaryBackground, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleDisabled( _this, EwLoadResource( &OximeterRotaryBackground, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleDefault( _this, EwLoadResource( &OximeterRotaryBackground, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetWidgetMaxSize( _this, _Const0058 );
  WidgetSetRotaryKnobConfig_OnSetWidgetMinSize( _this, _Const0058 );
}

/* Table with links to derived variants of the auto object : 'Oximeter::RotaryMaxConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( OximeterRotaryMaxConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( OximeterRotaryMaxConfig )

/* Include a file containing the bitmap resource : 'Oximeter::RotaryThumbMin' */
#include "_OximeterRotaryThumbMin.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::RotaryThumbMin' */
EW_RES_WITHOUT_VARIANTS( OximeterRotaryThumbMin )

/* User defined auto object: 'Oximeter::RotaryMinConfig' */
EW_DEFINE_AUTOOBJECT( OximeterRotaryMinConfig, WidgetSetRotaryKnobConfig )

/* Initializer for the auto object 'Oximeter::RotaryMinConfig' */
void OximeterRotaryMinConfig__Init( WidgetSetRotaryKnobConfig _this )
{
  WidgetSetRotaryKnobConfig_OnSetThumbMaxAngle( _this, 90.000000f );
  WidgetSetRotaryKnobConfig_OnSetThumbMinAngle( _this, 358.000000f );
  WidgetSetRotaryKnobConfig_OnSetThumbPivot( _this, _Const0059 );
  WidgetSetRotaryKnobConfig_OnSetThumbActive( _this, EwLoadResource( &OximeterRotaryThumbMin, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetThumbFocused( _this, EwLoadResource( &OximeterRotaryThumbMin, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetThumbDefault( _this, EwLoadResource( &OximeterRotaryThumbMin, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleActive( _this, EwLoadResource( &OximeterRotaryBackground, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleFocused( _this, EwLoadResource( &OximeterRotaryBackground, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleDisabled( _this, EwLoadResource( &OximeterRotaryBackground, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleDefault( _this, EwLoadResource( &OximeterRotaryBackground, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetWidgetMaxSize( _this, _Const0058 );
  WidgetSetRotaryKnobConfig_OnSetWidgetMinSize( _this, _Const0058 );
}

/* Table with links to derived variants of the auto object : 'Oximeter::RotaryMinConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( OximeterRotaryMinConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( OximeterRotaryMinConfig )

/* Initializer for the class 'Oximeter::GraphPanel' */
void OximeterGraphPanel__Init( OximeterGraphPanel _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterGraphPanel );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->TextOxygen, &_this->_XObject, 0 );
  WidgetSetToggleButton__Init( &_this->ToggleOxygen, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextPulse, &_this->_XObject, 0 );
  WidgetSetToggleButton__Init( &_this->TogglePulse, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextBezier, &_this->_XObject, 0 );
  WidgetSetToggleButton__Init( &_this->ToggleBezier, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextGradient, &_this->_XObject, 0 );
  WidgetSetToggleButton__Init( &_this->ToggleGradient, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextZoom, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextCaptionOxygen, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->L1, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->L2, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->L3, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->L4, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->L5, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->L6, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->L7, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->L8, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->L9, &_this->_XObject, 0 );
  OximeterGraph__Init( &_this->OxygenGraph, &_this->_XObject, 0 );
  OximeterGraph__Init( &_this->PulseGraph, &_this->_XObject, 0 );
  CoreSlideTouchHandler__Init( &_this->SlideTouchHandler, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextCaptionPulse, &_this->_XObject, 0 );
  WidgetSetHorizontalSlider__Init( &_this->ZoomSlider, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextOxygenMax, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextOxygenMin, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextPulseMax, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextPulseMin, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->CloseButton, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextLine, &_this->_XObject, 0 );
  WidgetSetHorizontalSlider__Init( &_this->LineSlider, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterGraphPanel );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const005A );
  CoreRectView__OnSetBounds( &_this->TextOxygen, _Const005B );
  ViewsText_OnSetAlignment( &_this->TextOxygen, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextOxygen, EwLoadString( &_Const005C ));
  CoreRectView__OnSetBounds( &_this->ToggleOxygen, _Const005D );
  WidgetSetToggleButton_OnSetChecked( &_this->ToggleOxygen, 1 );
  CoreRectView__OnSetBounds( &_this->TextPulse, _Const005E );
  ViewsText_OnSetAlignment( &_this->TextPulse, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextPulse, EwLoadString( &_Const005F ));
  CoreRectView__OnSetBounds( &_this->TogglePulse, _Const0060 );
  WidgetSetToggleButton_OnSetChecked( &_this->TogglePulse, 1 );
  CoreRectView__OnSetBounds( &_this->TextBezier, _Const0061 );
  ViewsText_OnSetAlignment( &_this->TextBezier, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextBezier, EwLoadString( &_Const0062 ));
  CoreRectView__OnSetBounds( &_this->ToggleBezier, _Const0063 );
  WidgetSetToggleButton_OnSetChecked( &_this->ToggleBezier, 1 );
  CoreRectView__OnSetBounds( &_this->TextGradient, _Const0064 );
  ViewsText_OnSetAlignment( &_this->TextGradient, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextGradient, EwLoadString( &_Const0065 ));
  CoreRectView__OnSetBounds( &_this->ToggleGradient, _Const0066 );
  CoreRectView__OnSetBounds( &_this->TextZoom, _Const0067 );
  ViewsText_OnSetAlignment( &_this->TextZoom, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextZoom, EwLoadString( &_Const0068 ));
  CoreRectView__OnSetBounds( &_this->TextCaptionOxygen, _Const0069 );
  ViewsText_OnSetAlignment( &_this->TextCaptionOxygen, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextCaptionOxygen, EwLoadString( &_Const006A ));
  ViewsText_OnSetColor( &_this->TextCaptionOxygen, OximeterColorOxygenDark );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->L1, _Const006B );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->L1, _Const006C );
  ViewsLine_OnSetColor( &_this->L1, _Const006D );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->L2, _Const006E );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->L2, _Const006F );
  ViewsLine_OnSetColor( &_this->L2, _Const006D );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->L3, _Const0070 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->L3, _Const0071 );
  ViewsLine_OnSetColor( &_this->L3, _Const006D );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->L4, _Const0072 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->L4, _Const0073 );
  ViewsLine_OnSetColor( &_this->L4, _Const006D );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->L5, _Const0074 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->L5, _Const006C );
  ViewsLine_OnSetColor( &_this->L5, _Const006D );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->L6, _Const0075 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->L6, _Const0076 );
  ViewsLine_OnSetColor( &_this->L6, _Const006D );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->L7, _Const0077 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->L7, _Const0078 );
  ViewsLine_OnSetColor( &_this->L7, _Const006D );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->L8, _Const0079 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->L8, _Const007A );
  ViewsLine_OnSetColor( &_this->L8, _Const006D );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->L9, _Const007B );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->L9, _Const006B );
  ViewsLine_OnSetColor( &_this->L9, _Const006D );
  CoreRectView__OnSetBounds( &_this->OxygenGraph, _Const007C );
  OximeterGraph_OnSetMinRange( &_this->OxygenGraph, 85 );
  OximeterGraph_OnSetMaxRange( &_this->OxygenGraph, 100 );
  OximeterGraph_OnSetFillColorTop( &_this->OxygenGraph, OximeterColorOxygenDark );
  OximeterGraph_OnSetFillColorBottom( &_this->OxygenGraph, _Const007D );
  CoreRectView__OnSetBounds( &_this->PulseGraph, _Const007C );
  OximeterGraph_OnSetMinRange( &_this->PulseGraph, 60 );
  OximeterGraph_OnSetMaxRange( &_this->PulseGraph, 120 );
  OximeterGraph_OnSetFillColorTop( &_this->PulseGraph, OximeterColorPulseDark );
  OximeterGraph_OnSetFillColorBottom( &_this->PulseGraph, _Const007D );
  CoreView_OnSetLayout((CoreView)&_this->SlideTouchHandler, CoreLayoutAlignToBottom 
  | CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->SlideTouchHandler, _Const007C );
  _this->SlideTouchHandler.SlideVert = 0;
  CoreSlideTouchHandler_OnSetRubberBandEffectDuration( &_this->SlideTouchHandler, 
  200 );
  CoreSlideTouchHandler_OnSetFriction( &_this->SlideTouchHandler, 0.100000f );
  CoreRectView__OnSetBounds( &_this->TextCaptionPulse, _Const007E );
  ViewsText_OnSetAlignment( &_this->TextCaptionPulse, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextCaptionPulse, EwLoadString( &_Const007F ));
  ViewsText_OnSetColor( &_this->TextCaptionPulse, OximeterColorPulseDark );
  CoreRectView__OnSetBounds( &_this->ZoomSlider, _Const0080 );
  WidgetSetHorizontalSlider_OnSetMaxValue( &_this->ZoomSlider, 80 );
  WidgetSetHorizontalSlider_OnSetMinValue( &_this->ZoomSlider, 10 );
  WidgetSetHorizontalSlider_OnSetCurrentValue( &_this->ZoomSlider, 50 );
  CoreRectView__OnSetBounds( &_this->TextOxygenMax, _Const0081 );
  ViewsText_OnSetAlignment( &_this->TextOxygenMax, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextOxygenMax, EwLoadString( &_Const0082 ));
  ViewsText_OnSetColor( &_this->TextOxygenMax, OximeterColorOxygenDark );
  CoreRectView__OnSetBounds( &_this->TextOxygenMin, _Const0083 );
  ViewsText_OnSetAlignment( &_this->TextOxygenMin, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextOxygenMin, EwLoadString( &_Const0082 ));
  ViewsText_OnSetColor( &_this->TextOxygenMin, OximeterColorOxygenDark );
  CoreRectView__OnSetBounds( &_this->TextPulseMax, _Const0084 );
  ViewsText_OnSetAlignment( &_this->TextPulseMax, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextPulseMax, EwLoadString( &_Const0082 ));
  ViewsText_OnSetColor( &_this->TextPulseMax, OximeterColorPulseDark );
  CoreRectView__OnSetBounds( &_this->TextPulseMin, _Const0085 );
  ViewsText_OnSetAlignment( &_this->TextPulseMin, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextPulseMin, EwLoadString( &_Const0082 ));
  ViewsText_OnSetColor( &_this->TextPulseMin, OximeterColorPulseDark );
  CoreRectView__OnSetBounds( &_this->CloseButton, _Const0086 );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->CloseButton, 1 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->CloseButton, 1 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->CloseButton, 1 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->CloseButton, 1 );
  CoreRectView__OnSetBounds( &_this->TextLine, _Const0087 );
  ViewsText_OnSetAlignment( &_this->TextLine, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextLine, EwLoadString( &_Const0088 ));
  CoreRectView__OnSetBounds( &_this->LineSlider, _Const0089 );
  WidgetSetHorizontalSlider_OnSetMaxValue( &_this->LineSlider, 100 );
  WidgetSetHorizontalSlider_OnSetMinValue( &_this->LineSlider, 10 );
  WidgetSetHorizontalSlider_OnSetCurrentValue( &_this->LineSlider, 30 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextOxygen ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ToggleOxygen ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextPulse ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TogglePulse ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextBezier ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ToggleBezier ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextGradient ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ToggleGradient ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextZoom ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextCaptionOxygen ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->L1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->L2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->L3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->L4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->L5 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->L6 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->L7 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->L8 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->L9 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->OxygenGraph ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PulseGraph ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SlideTouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextCaptionPulse ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ZoomSlider ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextOxygenMax ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextOxygenMin ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextPulseMax ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextPulseMin ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CloseButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextLine ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LineSlider ), 0 );
  ViewsText_OnSetFont( &_this->TextOxygen, EwLoadResource( &OximeterFontL, ResourcesFont ));
  _this->ToggleOxygen.OnSwitchOn = EwNewSlot( _this, OximeterGraphPanel_update );
  _this->ToggleOxygen.OnSwitchOff = EwNewSlot( _this, OximeterGraphPanel_update );
  WidgetSetToggleButton_OnSetAppearance( &_this->ToggleOxygen, EwGetAutoObject( 
  &OximeterToggleButtonConfig, WidgetSetToggleButtonConfig ));
  ViewsText_OnSetFont( &_this->TextPulse, EwLoadResource( &OximeterFontL, ResourcesFont ));
  _this->TogglePulse.OnSwitchOn = EwNewSlot( _this, OximeterGraphPanel_update );
  _this->TogglePulse.OnSwitchOff = EwNewSlot( _this, OximeterGraphPanel_update );
  WidgetSetToggleButton_OnSetAppearance( &_this->TogglePulse, EwGetAutoObject( &OximeterToggleButtonConfig, 
  WidgetSetToggleButtonConfig ));
  ViewsText_OnSetFont( &_this->TextBezier, EwLoadResource( &OximeterFontL, ResourcesFont ));
  _this->ToggleBezier.OnSwitchOn = EwNewSlot( _this, OximeterGraphPanel_update );
  _this->ToggleBezier.OnSwitchOff = EwNewSlot( _this, OximeterGraphPanel_update );
  WidgetSetToggleButton_OnSetAppearance( &_this->ToggleBezier, EwGetAutoObject( 
  &OximeterToggleButtonConfig, WidgetSetToggleButtonConfig ));
  ViewsText_OnSetFont( &_this->TextGradient, EwLoadResource( &OximeterFontL, ResourcesFont ));
  _this->ToggleGradient.OnSwitchOn = EwNewSlot( _this, OximeterGraphPanel_update );
  _this->ToggleGradient.OnSwitchOff = EwNewSlot( _this, OximeterGraphPanel_update );
  WidgetSetToggleButton_OnSetAppearance( &_this->ToggleGradient, EwGetAutoObject( 
  &OximeterToggleButtonConfig, WidgetSetToggleButtonConfig ));
  ViewsText_OnSetFont( &_this->TextZoom, EwLoadResource( &OximeterFontL, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextCaptionOxygen, EwLoadResource( &OximeterFontL, 
  ResourcesFont ));
  OximeterGraph_OnSetDataStorage( &_this->OxygenGraph, &EwGetAutoObject( &OximeterDevice, 
  OximeterDeviceClass )->OxygenStorage );
  OximeterGraph_OnSetDataStorage( &_this->PulseGraph, &EwGetAutoObject( &OximeterDevice, 
  OximeterDeviceClass )->PulseStorage );
  _this->SlideTouchHandler.OnSlide = EwNewSlot( _this, OximeterGraphPanel_onSlide );
  ViewsText_OnSetFont( &_this->TextCaptionPulse, EwLoadResource( &OximeterFontL, 
  ResourcesFont ));
  _this->ZoomSlider.OnChange = EwNewSlot( _this, OximeterGraphPanel_zoom );
  WidgetSetHorizontalSlider_OnSetAppearance( &_this->ZoomSlider, EwGetAutoObject( 
  &OximeterHorizontalSliderConfig, WidgetSetHorizontalSliderConfig ));
  ViewsText_OnSetFont( &_this->TextOxygenMax, EwLoadResource( &OximeterFontM, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextOxygenMin, EwLoadResource( &OximeterFontM, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextPulseMax, EwLoadResource( &OximeterFontM, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextPulseMin, EwLoadResource( &OximeterFontM, ResourcesFont ));
  WidgetSetPushButton_OnSetIcon( &_this->CloseButton, EwLoadResource( &OximeterIconButton, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->CloseButton, EwGetAutoObject( &OximeterIconButtonConfig, 
  WidgetSetPushButtonConfig ));
  ViewsText_OnSetFont( &_this->TextLine, EwLoadResource( &OximeterFontL, ResourcesFont ));
  _this->LineSlider.OnChange = EwNewSlot( _this, OximeterGraphPanel_update );
  WidgetSetHorizontalSlider_OnSetAppearance( &_this->LineSlider, EwGetAutoObject( 
  &OximeterHorizontalSliderConfig, WidgetSetHorizontalSliderConfig ));
}

/* Re-Initializer for the class 'Oximeter::GraphPanel' */
void OximeterGraphPanel__ReInit( OximeterGraphPanel _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->TextOxygen );
  WidgetSetToggleButton__ReInit( &_this->ToggleOxygen );
  ViewsText__ReInit( &_this->TextPulse );
  WidgetSetToggleButton__ReInit( &_this->TogglePulse );
  ViewsText__ReInit( &_this->TextBezier );
  WidgetSetToggleButton__ReInit( &_this->ToggleBezier );
  ViewsText__ReInit( &_this->TextGradient );
  WidgetSetToggleButton__ReInit( &_this->ToggleGradient );
  ViewsText__ReInit( &_this->TextZoom );
  ViewsText__ReInit( &_this->TextCaptionOxygen );
  ViewsLine__ReInit( &_this->L1 );
  ViewsLine__ReInit( &_this->L2 );
  ViewsLine__ReInit( &_this->L3 );
  ViewsLine__ReInit( &_this->L4 );
  ViewsLine__ReInit( &_this->L5 );
  ViewsLine__ReInit( &_this->L6 );
  ViewsLine__ReInit( &_this->L7 );
  ViewsLine__ReInit( &_this->L8 );
  ViewsLine__ReInit( &_this->L9 );
  OximeterGraph__ReInit( &_this->OxygenGraph );
  OximeterGraph__ReInit( &_this->PulseGraph );
  CoreSlideTouchHandler__ReInit( &_this->SlideTouchHandler );
  ViewsText__ReInit( &_this->TextCaptionPulse );
  WidgetSetHorizontalSlider__ReInit( &_this->ZoomSlider );
  ViewsText__ReInit( &_this->TextOxygenMax );
  ViewsText__ReInit( &_this->TextOxygenMin );
  ViewsText__ReInit( &_this->TextPulseMax );
  ViewsText__ReInit( &_this->TextPulseMin );
  WidgetSetPushButton__ReInit( &_this->CloseButton );
  ViewsText__ReInit( &_this->TextLine );
  WidgetSetHorizontalSlider__ReInit( &_this->LineSlider );
}

/* Finalizer method for the class 'Oximeter::GraphPanel' */
void OximeterGraphPanel__Done( OximeterGraphPanel _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->TextOxygen );
  WidgetSetToggleButton__Done( &_this->ToggleOxygen );
  ViewsText__Done( &_this->TextPulse );
  WidgetSetToggleButton__Done( &_this->TogglePulse );
  ViewsText__Done( &_this->TextBezier );
  WidgetSetToggleButton__Done( &_this->ToggleBezier );
  ViewsText__Done( &_this->TextGradient );
  WidgetSetToggleButton__Done( &_this->ToggleGradient );
  ViewsText__Done( &_this->TextZoom );
  ViewsText__Done( &_this->TextCaptionOxygen );
  ViewsLine__Done( &_this->L1 );
  ViewsLine__Done( &_this->L2 );
  ViewsLine__Done( &_this->L3 );
  ViewsLine__Done( &_this->L4 );
  ViewsLine__Done( &_this->L5 );
  ViewsLine__Done( &_this->L6 );
  ViewsLine__Done( &_this->L7 );
  ViewsLine__Done( &_this->L8 );
  ViewsLine__Done( &_this->L9 );
  OximeterGraph__Done( &_this->OxygenGraph );
  OximeterGraph__Done( &_this->PulseGraph );
  CoreSlideTouchHandler__Done( &_this->SlideTouchHandler );
  ViewsText__Done( &_this->TextCaptionPulse );
  WidgetSetHorizontalSlider__Done( &_this->ZoomSlider );
  ViewsText__Done( &_this->TextOxygenMax );
  ViewsText__Done( &_this->TextOxygenMin );
  ViewsText__Done( &_this->TextPulseMax );
  ViewsText__Done( &_this->TextPulseMin );
  WidgetSetPushButton__Done( &_this->CloseButton );
  ViewsText__Done( &_this->TextLine );
  WidgetSetHorizontalSlider__Done( &_this->LineSlider );

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
void OximeterGraphPanel_UpdateLayout( OximeterGraphPanel _this, XPoint aSize )
{
  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );
  EwPostSignal( EwNewSlot( _this, OximeterGraphPanel_zoom ), ((XObject)_this ));
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
void OximeterGraphPanel_UpdateViewState( OximeterGraphPanel _this, XSet aState )
{
  XBool oxygen;
  XBool pulse;
  XBool bezier;
  XBool gradient;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  oxygen = _this->ToggleOxygen.Checked;
  pulse = _this->TogglePulse.Checked;
  bezier = _this->ToggleBezier.Checked;
  gradient = _this->ToggleGradient.Checked;
  CoreGroup_OnSetVisible((CoreGroup)&_this->OxygenGraph, oxygen );
  CoreGroup_OnSetVisible((CoreGroup)&_this->PulseGraph, pulse );
  OximeterGraph_OnSetBezier( &_this->OxygenGraph, bezier );
  OximeterGraph_OnSetBezier( &_this->PulseGraph, bezier );
  OximeterGraph_OnSetFilled( &_this->OxygenGraph, gradient );
  OximeterGraph_OnSetFilled( &_this->PulseGraph, gradient );
  OximeterGraph_OnSetLineWidth( &_this->OxygenGraph, (XFloat)WidgetSetHorizontalSlider_OnGetCurrentValue( 
  &_this->LineSlider ) / 10.000000f );
  OximeterGraph_OnSetLineWidth( &_this->PulseGraph, (XFloat)WidgetSetHorizontalSlider_OnGetCurrentValue( 
  &_this->LineSlider ) / 10.000000f );
  ViewsText_OnSetVisible( &_this->TextCaptionOxygen, oxygen );
  ViewsText_OnSetVisible( &_this->TextOxygenMin, oxygen );
  ViewsText_OnSetVisible( &_this->TextOxygenMax, oxygen );
  ViewsText_OnSetString( &_this->TextOxygenMin, EwNewStringInt( _this->OxygenGraph.MinRange, 
  2, 10 ));
  ViewsText_OnSetString( &_this->TextOxygenMax, EwNewStringInt( _this->OxygenGraph.MaxRange, 
  2, 10 ));
  ViewsText_OnSetVisible( &_this->TextCaptionPulse, pulse );
  ViewsText_OnSetVisible( &_this->TextPulseMin, pulse );
  ViewsText_OnSetVisible( &_this->TextPulseMax, pulse );
  ViewsText_OnSetString( &_this->TextPulseMin, EwNewStringInt( _this->PulseGraph.MinRange, 
  2, 10 ));
  ViewsText_OnSetString( &_this->TextPulseMax, EwNewStringInt( _this->PulseGraph.MaxRange, 
  2, 10 ));

  if ( gradient )
  {
    OximeterGraph_OnSetLineColorLeft( &_this->OxygenGraph, _Const0003 );
    OximeterGraph_OnSetLineColorRight( &_this->OxygenGraph, _Const0003 );
    OximeterGraph_OnSetLineColorLeft( &_this->PulseGraph, _Const0003 );
    OximeterGraph_OnSetLineColorRight( &_this->PulseGraph, _Const0003 );
  }
  else
  {
    OximeterGraph_OnSetLineColorLeft( &_this->OxygenGraph, OximeterColorOxygenLight );
    OximeterGraph_OnSetLineColorRight( &_this->OxygenGraph, OximeterColorOxygenDark );
    OximeterGraph_OnSetLineColorLeft( &_this->PulseGraph, OximeterColorPulseLight );
    OximeterGraph_OnSetLineColorRight( &_this->PulseGraph, OximeterColorPulseDark );
  }
}

/* 'C' function for method : 'Oximeter::GraphPanel.onSlide()' */
void OximeterGraphPanel_onSlide( OximeterGraphPanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  OximeterGraph_OnSetScrollOffset( &_this->OxygenGraph, _this->SlideTouchHandler.Offset.X );
  OximeterGraph_OnSetScrollOffset( &_this->PulseGraph, _this->SlideTouchHandler.Offset.X );
}

/* 'C' function for method : 'Oximeter::GraphPanel.update()' */
void OximeterGraphPanel_update( OximeterGraphPanel _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Oximeter::GraphPanel.zoom()' */
void OximeterGraphPanel_zoom( OximeterGraphPanel _this, XObject sender )
{
  XInt32 w;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  OximeterGraph_OnSetPixelPerValue( &_this->OxygenGraph, WidgetSetHorizontalSlider_OnGetCurrentValue( 
  &_this->ZoomSlider ));
  OximeterGraph_OnSetPixelPerValue( &_this->PulseGraph, WidgetSetHorizontalSlider_OnGetCurrentValue( 
  &_this->ZoomSlider ));
  w = OximeterGraph_OnGetGraphWidth( &_this->OxygenGraph ) - EwGetRectW( _this->OxygenGraph.Super2.Bounds );

  if ( OximeterGraph_OnGetGraphWidth( &_this->OxygenGraph ) > OximeterGraph_OnGetGraphWidth( 
      &_this->PulseGraph ))
    w = OximeterGraph_OnGetGraphWidth( &_this->PulseGraph ) - EwGetRectW( _this->PulseGraph.Super2.Bounds );

  _this->SlideTouchHandler.MaxOffset = EwNewPoint( w, 0 );
  _this->SlideTouchHandler.Offset = _Const0006;
  OximeterGraph_OnSetScrollOffset( &_this->OxygenGraph, _this->SlideTouchHandler.Offset.X );
  OximeterGraph_OnSetScrollOffset( &_this->PulseGraph, _this->SlideTouchHandler.Offset.X );
}

/* 'C' function for method : 'Oximeter::GraphPanel.OnSetOnClose()' */
void OximeterGraphPanel_OnSetOnClose( OximeterGraphPanel _this, XSlot value )
{
  if ( !EwCompSlot( _this->OnClose, value ))
    return;

  _this->OnClose = value;
  _this->CloseButton.OnActivate = value;
}

/* Variants derived from the class : 'Oximeter::GraphPanel' */
EW_DEFINE_CLASS_VARIANTS( OximeterGraphPanel )
EW_END_OF_CLASS_VARIANTS( OximeterGraphPanel )

/* Virtual Method Table (VMT) for the class : 'Oximeter::GraphPanel' */
EW_DEFINE_CLASS( OximeterGraphPanel, CoreGroup, OnClose, OnClose, TextOxygen, TextOxygen, 
                 _None, _None, "Oximeter::GraphPanel" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  OximeterGraphPanel_UpdateLayout,
  OximeterGraphPanel_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterGraphPanel )

/* Color constants for panels, text items and graphs */
const XColor OximeterColorOxygenLight = { 0x00, 0xE3, 0xF1, 0xFF };

/* Color constants for panels, text items and graphs */
const XColor OximeterColorOxygenDark = { 0x00, 0x9E, 0xE2, 0xFF };

/* Color constants for panels, text items and graphs */
const XColor OximeterColorPulseLight = { 0xFF, 0x73, 0x39, 0xFF };

/* Color constants for panels, text items and graphs */
const XColor OximeterColorPulseDark = { 0xFF, 0x1A, 0x53, 0xFF };

/* User defined auto object: 'Oximeter::ToggleButtonConfig' */
EW_DEFINE_AUTOOBJECT( OximeterToggleButtonConfig, WidgetSetToggleButtonConfig )

/* Initializer for the auto object 'Oximeter::ToggleButtonConfig' */
void OximeterToggleButtonConfig__Init( WidgetSetToggleButtonConfig _this )
{
  WidgetSetToggleButtonConfig_OnSetFaceOnFrameActive( _this, 1 );
  WidgetSetToggleButtonConfig_OnSetFaceOnFrameFocused( _this, 1 );
  WidgetSetToggleButtonConfig_OnSetFaceOnFrameDefault( _this, 1 );
  WidgetSetToggleButtonConfig_OnSetFaceOnActive( _this, EwLoadResource( &OximeterToggleButton, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOnFocused( _this, EwLoadResource( &OximeterToggleButton, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOnDefault( _this, EwLoadResource( &OximeterToggleButton, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOffFrameActive( _this, 0 );
  WidgetSetToggleButtonConfig_OnSetFaceOffFrameFocused( _this, 0 );
  WidgetSetToggleButtonConfig_OnSetFaceOffFrameDefault( _this, 0 );
  WidgetSetToggleButtonConfig_OnSetFaceOffActive( _this, EwLoadResource( &OximeterToggleButton, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOffFocused( _this, EwLoadResource( &OximeterToggleButton, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOffDefault( _this, EwLoadResource( &OximeterToggleButton, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetWidgetMaxSize( _this, _Const008A );
  WidgetSetToggleButtonConfig_OnSetWidgetMinSize( _this, _Const008A );
}

/* Table with links to derived variants of the auto object : 'Oximeter::ToggleButtonConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( OximeterToggleButtonConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( OximeterToggleButtonConfig )

/* Include a file containing the bitmap resource : 'Oximeter::ToggleButton' */
#include "_OximeterToggleButton.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::ToggleButton' */
EW_RES_WITHOUT_VARIANTS( OximeterToggleButton )

/* Include a file containing the font resource : 'Oximeter::FontL' */
#include "_OximeterFontL.h"

/* Table with links to derived variants of the font resource : 'Oximeter::FontL' */
EW_RES_WITHOUT_VARIANTS( OximeterFontL )

/* User defined auto object: 'Oximeter::HorizontalSliderConfig' */
EW_DEFINE_AUTOOBJECT( OximeterHorizontalSliderConfig, WidgetSetHorizontalSliderConfig )

/* Initializer for the auto object 'Oximeter::HorizontalSliderConfig' */
void OximeterHorizontalSliderConfig__Init( WidgetSetHorizontalSliderConfig _this )
{
  WidgetSetHorizontalSliderConfig_OnSetThumbActive( _this, EwLoadResource( &OximeterThumb, 
  ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetThumbFocused( _this, EwLoadResource( &OximeterThumb, 
  ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetThumbDefault( _this, EwLoadResource( &OximeterThumb, 
  ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameActive( _this, 1 );
  WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameFocused( _this, 1 );
  WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameDisabled( _this, 1 );
  WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameDefault( _this, 1 );
  WidgetSetHorizontalSliderConfig_OnSetTrackRightActive( _this, EwLoadResource( 
  &OximeterTrack, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackRightFocused( _this, EwLoadResource( 
  &OximeterTrack, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackRightDisabled( _this, EwLoadResource( 
  &OximeterTrack, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackRightDefault( _this, EwLoadResource( 
  &OximeterTrack, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameActive( _this, 0 );
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameFocused( _this, 0 );
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameDisabled( _this, 0 );
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameDefault( _this, 0 );
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftActive( _this, EwLoadResource( &OximeterTrack, 
  ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftFocused( _this, EwLoadResource( 
  &OximeterTrack, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftDisabled( _this, EwLoadResource( 
  &OximeterTrack, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftDefault( _this, EwLoadResource( 
  &OximeterTrack, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetWidgetMaxSize( _this, _Const008B );
  WidgetSetHorizontalSliderConfig_OnSetWidgetMinSize( _this, _Const008C );
}

/* Table with links to derived variants of the auto object : 'Oximeter::HorizontalSliderConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( OximeterHorizontalSliderConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( OximeterHorizontalSliderConfig )

/* Include a file containing the bitmap resource : 'Oximeter::Thumb' */
#include "_OximeterThumb.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::Thumb' */
EW_RES_WITHOUT_VARIANTS( OximeterThumb )

/* Include a file containing the bitmap resource : 'Oximeter::Track' */
#include "_OximeterTrack.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::Track' */
EW_RES_WITHOUT_VARIANTS( OximeterTrack )

/* Initializer for the class 'Oximeter::Oximeter' */
void OximeterOximeter__Init( OximeterOximeter _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterOximeter );

  /* ... then construct all embedded objects */
  OximeterBackground__Init( &_this->Background, &_this->_XObject, 0 );
  OximeterPulsePanel__Init( &_this->PulsePanel, &_this->_XObject, 0 );
  OximeterOxygenPanel__Init( &_this->OxygenPanel, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->PushButton, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->ShowPanelEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->RemovePanelEffect, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Battery, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->AutoDemoTimer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterOximeter );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const002F );
  CoreView_OnSetLayout((CoreView)&_this->Background, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Background, _Const002F );
  CoreView_OnSetLayout((CoreView)&_this->PulsePanel, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->PulsePanel, _Const0031 );
  CoreView_OnSetLayout((CoreView)&_this->OxygenPanel, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->OxygenPanel, _Const0016 );
  CoreRectView__OnSetBounds( &_this->PushButton, _Const0086 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->ShowPanelEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->ShowPanelEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->ShowPanelEffect, 1000 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->ShowPanelEffect, 700 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->RemovePanelEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->RemovePanelEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->RemovePanelEffect, 200 );
  _this->RemovePanelEffect.Value2 = 0;
  _this->RemovePanelEffect.Value1 = 255;
  _this->Mode = OximeterOperationModeOverview;
  CoreRectView__OnSetBounds( &_this->Battery, _Const008D );
  ViewsImage_OnSetColor( &_this->Battery, _Const0002 );
  ViewsImage_OnSetAnimated( &_this->Battery, 1 );
  CoreTimer_OnSetPeriod( &_this->AutoDemoTimer, 4000 );
  CoreTimer_OnSetBegin( &_this->AutoDemoTimer, 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Background ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PulsePanel ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->OxygenPanel ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PushButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Battery ), 0 );
  OximeterPulsePanel_OnSetOnChart( &_this->PulsePanel, EwNewSlot( _this, OximeterOximeter_slotGraphs ));
  OximeterPulsePanel_OnSetOnAdjust( &_this->PulsePanel, EwNewSlot( _this, OximeterOximeter_slotPulseSettings ));
  OximeterPulsePanel_OnSetOnOverview( &_this->PulsePanel, EwNewSlot( _this, OximeterOximeter_slotOverview ));
  OximeterOxygenPanel_OnSetOnChart( &_this->OxygenPanel, EwNewSlot( _this, OximeterOximeter_slotGraphs ));
  OximeterOxygenPanel_OnSetOnAdjust( &_this->OxygenPanel, EwNewSlot( _this, OximeterOximeter_slotOxiSettings ));
  OximeterOxygenPanel_OnSetOnOverview( &_this->OxygenPanel, EwNewSlot( _this, OximeterOximeter_slotOverview ));
  _this->Device = EwGetAutoObject( &OximeterDevice, OximeterDeviceClass );
  _this->PushButton.OnActivate = EwNewSlot( _this, OximeterOximeter_slotMenus );
  WidgetSetPushButton_OnSetIcon( &_this->PushButton, EwLoadResource( &OximeterIconButton, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->PushButton, EwGetAutoObject( &OximeterIconButtonConfig, 
  WidgetSetPushButtonConfig ));
  _this->RemovePanelEffect.Super1.OnAnimate = EwNewSlot( _this, OximeterOximeter_removePanel );
  ViewsImage_OnSetBitmap( &_this->Battery, EwLoadResource( &OximeterBattery, ResourcesBitmap ));
  _this->AutoDemoTimer.OnTrigger = EwNewSlot( _this, OximeterOximeter_AutoDemoSlot );
}

/* Re-Initializer for the class 'Oximeter::Oximeter' */
void OximeterOximeter__ReInit( OximeterOximeter _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  OximeterBackground__ReInit( &_this->Background );
  OximeterPulsePanel__ReInit( &_this->PulsePanel );
  OximeterOxygenPanel__ReInit( &_this->OxygenPanel );
  WidgetSetPushButton__ReInit( &_this->PushButton );
  EffectsInt32Effect__ReInit( &_this->ShowPanelEffect );
  EffectsInt32Effect__ReInit( &_this->RemovePanelEffect );
  ViewsImage__ReInit( &_this->Battery );
  CoreTimer__ReInit( &_this->AutoDemoTimer );
}

/* Finalizer method for the class 'Oximeter::Oximeter' */
void OximeterOximeter__Done( OximeterOximeter _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  OximeterBackground__Done( &_this->Background );
  OximeterPulsePanel__Done( &_this->PulsePanel );
  OximeterOxygenPanel__Done( &_this->OxygenPanel );
  WidgetSetPushButton__Done( &_this->PushButton );
  EffectsInt32Effect__Done( &_this->ShowPanelEffect );
  EffectsInt32Effect__Done( &_this->RemovePanelEffect );
  ViewsImage__Done( &_this->Battery );
  CoreTimer__Done( &_this->AutoDemoTimer );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Oximeter::Oximeter.slotOverview()' */
void OximeterOximeter_slotOverview( OximeterOximeter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Mode == OximeterOperationModeOverview )
    return;

  _this->Mode = OximeterOperationModeOverview;
  OximeterPulsePanel_ChangeMode( &_this->PulsePanel, _this->Mode );
  OximeterOxygenPanel_ChangeMode( &_this->OxygenPanel, _this->Mode );
  OximeterBackground_ChangeMode( &_this->Background, _this->Mode );
  EwPostSignal( EwNewSlot( &_this->RemovePanelEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
}

/* 'C' function for method : 'Oximeter::Oximeter.slotGraphs()' */
void OximeterOximeter_slotGraphs( OximeterOximeter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Mode == OximeterOperationModeGraphs )
    return;

  _this->Mode = OximeterOperationModeGraphs;
  OximeterPulsePanel_ChangeMode( &_this->PulsePanel, _this->Mode );
  OximeterOxygenPanel_ChangeMode( &_this->OxygenPanel, _this->Mode );
  OximeterBackground_ChangeMode( &_this->Background, _this->Mode );
  EwPostSignal( EwNewSlot( &_this->RemovePanelEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
  _this->graphPanel = EwNewObject( OximeterGraphPanel, 0 );
  CoreRectView__OnSetBounds( _this->graphPanel, EwNewRect( 0, EwGetRectH( _this->Super2.Bounds ) 
  / 3, EwGetRectW( _this->Super2.Bounds ), EwGetRectH( _this->Super2.Bounds )));
  CoreGroup__OnSetOpacity( _this->graphPanel, 0 );
  OximeterGraphPanel_OnSetOnClose( _this->graphPanel, EwNewSlot( _this, OximeterOximeter_slotOverview ));
  CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->graphPanel ), 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->ShowPanelEffect, 0 );
  _this->ShowPanelEffect.Outlet = EwNewRef( _this->graphPanel, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->ShowPanelEffect, 1 );
}

/* 'C' function for method : 'Oximeter::Oximeter.slotMenus()' */
void OximeterOximeter_slotMenus( OximeterOximeter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Mode == OximeterOperationModeMenus )
    return;

  _this->Mode = OximeterOperationModeMenus;
  OximeterPulsePanel_ChangeMode( &_this->PulsePanel, _this->Mode );
  OximeterOxygenPanel_ChangeMode( &_this->OxygenPanel, _this->Mode );
  OximeterBackground_ChangeMode( &_this->Background, _this->Mode );
  EwPostSignal( EwNewSlot( &_this->RemovePanelEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
  _this->mainMenu = EwNewObject( OximeterMainMenu, 0 );
  CoreRectView__OnSetBounds( _this->mainMenu, EwNewRect(( EwGetRectW( _this->Super2.Bounds ) 
  * 1 ) / 4, 0, ( EwGetRectW( _this->Super2.Bounds ) * 3 ) / 4, EwGetRectH( _this->Super2.Bounds )));
  CoreGroup__OnSetOpacity( _this->mainMenu, 0 );
  OximeterMainMenu_OnSetOnClose( _this->mainMenu, EwNewSlot( _this, OximeterOximeter_slotOverview ));
  CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->mainMenu ), 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->ShowPanelEffect, 0 );
  _this->ShowPanelEffect.Outlet = EwNewRef( _this->mainMenu, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->ShowPanelEffect, 1 );
}

/* 'C' function for method : 'Oximeter::Oximeter.slotOxiSettings()' */
void OximeterOximeter_slotOxiSettings( OximeterOximeter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Mode == OximeterOperationModeOxygenSettings )
    return;

  _this->Mode = OximeterOperationModeOxygenSettings;
  OximeterPulsePanel_ChangeMode( &_this->PulsePanel, _this->Mode );
  OximeterOxygenPanel_ChangeMode( &_this->OxygenPanel, _this->Mode );
  OximeterBackground_ChangeMode( &_this->Background, _this->Mode );
  EwPostSignal( EwNewSlot( &_this->RemovePanelEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
  _this->oxygenSettings = EwNewObject( OximeterOxygenSettings, 0 );
  CoreRectView__OnSetBounds( _this->oxygenSettings, EwNewRect( 0, 0, ( EwGetRectW( 
  _this->Super2.Bounds ) * 3 ) / 4, EwGetRectH( _this->Super2.Bounds )));
  CoreGroup__OnSetOpacity( _this->oxygenSettings, 0 );
  OximeterOxygenSettings_OnSetOnClose( _this->oxygenSettings, EwNewSlot( _this, 
  OximeterOximeter_slotOverview ));
  CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->oxygenSettings ), 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->ShowPanelEffect, 0 );
  _this->ShowPanelEffect.Outlet = EwNewRef( _this->oxygenSettings, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->ShowPanelEffect, 1 );
}

/* 'C' function for method : 'Oximeter::Oximeter.slotPulseSettings()' */
void OximeterOximeter_slotPulseSettings( OximeterOximeter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Mode == OximeterOperationModePulseSettings )
    return;

  _this->Mode = OximeterOperationModePulseSettings;
  OximeterPulsePanel_ChangeMode( &_this->PulsePanel, _this->Mode );
  OximeterOxygenPanel_ChangeMode( &_this->OxygenPanel, _this->Mode );
  OximeterBackground_ChangeMode( &_this->Background, _this->Mode );
  EwPostSignal( EwNewSlot( &_this->RemovePanelEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
  _this->pulseSettings = EwNewObject( OximeterPulseSettings, 0 );
  CoreRectView__OnSetBounds( _this->pulseSettings, EwNewRect( EwGetRectW( _this->Super2.Bounds ) 
  / 4, 0, EwGetRectW( _this->Super2.Bounds ), EwGetRectH( _this->Super2.Bounds )));
  CoreGroup__OnSetOpacity( _this->pulseSettings, 0 );
  OximeterPulseSettings_OnSetOnClose( _this->pulseSettings, EwNewSlot( _this, OximeterOximeter_slotOverview ));
  CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->pulseSettings ), 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->ShowPanelEffect, 0 );
  _this->ShowPanelEffect.Outlet = EwNewRef( _this->pulseSettings, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->ShowPanelEffect, 1 );
}

/* 'C' function for method : 'Oximeter::Oximeter.removePanel()' */
void OximeterOximeter_removePanel( OximeterOximeter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->graphPanel != 0 ) && ( _this->Mode != OximeterOperationModeGraphs ))
  {
    CoreGroup__OnSetOpacity( _this->graphPanel, _this->RemovePanelEffect.Value );

    if ( _this->graphPanel->Super1.Opacity == 0 )
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->graphPanel ));
      _this->graphPanel = 0;
    }
  }

  if (( _this->mainMenu != 0 ) && ( _this->Mode != OximeterOperationModeMenus ))
  {
    CoreGroup__OnSetOpacity( _this->mainMenu, _this->RemovePanelEffect.Value );

    if ( _this->mainMenu->Super1.Opacity == 0 )
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->mainMenu ));
      _this->mainMenu = 0;
    }
  }

  if (( _this->pulseSettings != 0 ) && ( _this->Mode != OximeterOperationModePulseSettings ))
  {
    CoreGroup__OnSetOpacity( _this->pulseSettings, _this->RemovePanelEffect.Value );

    if ( _this->pulseSettings->Super1.Opacity == 0 )
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->pulseSettings ));
      _this->pulseSettings = 0;
    }
  }

  if (( _this->oxygenSettings != 0 ) && ( _this->Mode != OximeterOperationModeOxygenSettings ))
  {
    CoreGroup__OnSetOpacity( _this->oxygenSettings, _this->RemovePanelEffect.Value );

    if ( _this->oxygenSettings->Super1.Opacity == 0 )
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->oxygenSettings ));
      _this->oxygenSettings = 0;
    }
  }
}

/* Slot method to trigger the next phase of the auto demo. */
void OximeterOximeter_AutoDemoSlot( OximeterOximeter _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  switch ( _this->AutoState )
  {
    case 0 :
      EwPostSignal( EwNewSlot( _this, OximeterOximeter_slotOverview ), ((XObject)_this ));
    break;

    case 1 :
      EwPostSignal( EwNewSlot( _this, OximeterOximeter_slotGraphs ), ((XObject)_this ));
    break;

    case 2 :
      EwPostSignal( EwNewSlot( _this, OximeterOximeter_slotOverview ), ((XObject)_this ));
    break;

    case 3 :
      EwPostSignal( EwNewSlot( _this, OximeterOximeter_slotOxiSettings ), ((XObject)_this ));
    break;

    case 4 :
      EwPostSignal( EwNewSlot( _this, OximeterOximeter_slotPulseSettings ), ((XObject)_this ));
    break;

    case 5 :
      EwPostSignal( EwNewSlot( _this, OximeterOximeter_slotGraphs ), ((XObject)_this ));
    break;

    case 6 :
      EwPostSignal( EwNewSlot( _this, OximeterOximeter_slotMenus ), ((XObject)_this ));
    break;

    default : 
      ;
  }

  _this->AutoState = _this->AutoState + 1;

  if ( _this->AutoState > 6 )
    _this->AutoState = 0;
}

/* Variants derived from the class : 'Oximeter::Oximeter' */
EW_DEFINE_CLASS_VARIANTS( OximeterOximeter )
EW_END_OF_CLASS_VARIANTS( OximeterOximeter )

/* Virtual Method Table (VMT) for the class : 'Oximeter::Oximeter' */
EW_DEFINE_CLASS( OximeterOximeter, CoreGroup, Device, Background, Background, Background, 
                 AutoState, AutoState, "Oximeter::Oximeter" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterOximeter )

/* Initializer for the class 'Oximeter::PulseSettings' */
void OximeterPulseSettings__Init( OximeterPulseSettings _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterPulseSettings );

  /* ... then construct all embedded objects */
  WidgetSetRotaryKnob__Init( &_this->RotaryKnobMax, &_this->_XObject, 0 );
  WidgetSetRotaryKnob__Init( &_this->RotaryKnobMin, &_this->_XObject, 0 );
  WidgetSetHorizontalSlider__Init( &_this->HorizontalSlider, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextAlarm, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->PushButtonCancel, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->PushButtonSave, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextMax, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextMin, &_this->_XObject, 0 );
  OximeterNumberDisplay__Init( &_this->NumberDisplayMax, &_this->_XObject, 0 );
  OximeterNumberDisplay__Init( &_this->NumberDisplayMin, &_this->_XObject, 0 );
  OximeterNumberDisplay__Init( &_this->NumberDisplayAlarm, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterPulseSettings );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const008E );
  CoreRectView__OnSetBounds( &_this->RotaryKnobMax, _Const008F );
  WidgetSetRotaryKnob_OnSetMaxValue( &_this->RotaryKnobMax, 150 );
  WidgetSetRotaryKnob_OnSetMinValue( &_this->RotaryKnobMax, 90 );
  CoreRectView__OnSetBounds( &_this->RotaryKnobMin, _Const0090 );
  WidgetSetRotaryKnob_OnSetMaxValue( &_this->RotaryKnobMin, 75 );
  WidgetSetRotaryKnob_OnSetMinValue( &_this->RotaryKnobMin, 30 );
  CoreRectView__OnSetBounds( &_this->HorizontalSlider, _Const0091 );
  WidgetSetHorizontalSlider_OnSetMaxValue( &_this->HorizontalSlider, 180 );
  CoreRectView__OnSetBounds( &_this->TextAlarm, _Const0092 );
  ViewsText_OnSetAlignment( &_this->TextAlarm, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextAlarm, EwLoadString( &_Const0093 ));
  CoreRectView__OnSetBounds( &_this->PushButtonCancel, _Const0094 );
  WidgetSetPushButton_OnSetLabel( &_this->PushButtonCancel, EwLoadString( &_Const0095 ));
  CoreRectView__OnSetBounds( &_this->PushButtonSave, _Const0096 );
  WidgetSetPushButton_OnSetLabel( &_this->PushButtonSave, EwLoadString( &_Const0097 ));
  CoreRectView__OnSetBounds( &_this->TextMax, _Const0098 );
  ViewsText_OnSetAlignment( &_this->TextMax, ViewsTextAlignmentAlignHorzCenter | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextMax, EwLoadString( &_Const0099 ));
  CoreRectView__OnSetBounds( &_this->TextMin, _Const009A );
  ViewsText_OnSetAlignment( &_this->TextMin, ViewsTextAlignmentAlignHorzCenter | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextMin, EwLoadString( &_Const009B ));
  CoreRectView__OnSetBounds( &_this->NumberDisplayMax, _Const009C );
  CoreRectView__OnSetBounds( &_this->NumberDisplayMin, _Const009D );
  CoreRectView__OnSetBounds( &_this->NumberDisplayAlarm, _Const009E );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RotaryKnobMax ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RotaryKnobMin ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->HorizontalSlider ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextAlarm ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PushButtonCancel ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PushButtonSave ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextMax ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextMin ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->NumberDisplayMax ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->NumberDisplayMin ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->NumberDisplayAlarm ), 0 );
  WidgetSetRotaryKnob_OnSetOutlet( &_this->RotaryKnobMax, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMaxPulse, OximeterDeviceClass_OnSetMaxPulse ));
  WidgetSetRotaryKnob_OnSetAppearance( &_this->RotaryKnobMax, EwGetAutoObject( &OximeterRotaryMaxConfig, 
  WidgetSetRotaryKnobConfig ));
  WidgetSetRotaryKnob_OnSetOutlet( &_this->RotaryKnobMin, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMinPulse, OximeterDeviceClass_OnSetMinPulse ));
  WidgetSetRotaryKnob_OnSetAppearance( &_this->RotaryKnobMin, EwGetAutoObject( &OximeterRotaryMinConfig, 
  WidgetSetRotaryKnobConfig ));
  WidgetSetHorizontalSlider_OnSetOutlet( &_this->HorizontalSlider, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetAlarmTime, OximeterDeviceClass_OnSetAlarmTime ));
  WidgetSetHorizontalSlider_OnSetAppearance( &_this->HorizontalSlider, EwGetAutoObject( 
  &OximeterHorizontalSliderConfig, WidgetSetHorizontalSliderConfig ));
  ViewsText_OnSetFont( &_this->TextAlarm, EwLoadResource( &OximeterFontL, ResourcesFont ));
  _this->PushButtonCancel.OnActivate = EwNewSlot( _this, OximeterPulseSettings_onCancel );
  WidgetSetPushButton_OnSetAppearance( &_this->PushButtonCancel, EwGetAutoObject( 
  &OximeterPushButtonConfig, WidgetSetPushButtonConfig ));
  WidgetSetPushButton_OnSetAppearance( &_this->PushButtonSave, EwGetAutoObject( 
  &OximeterPushButtonConfig, WidgetSetPushButtonConfig ));
  ViewsText_OnSetFont( &_this->TextMax, EwLoadResource( &OximeterFontL, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextMin, EwLoadResource( &OximeterFontL, ResourcesFont ));
  OximeterNumberDisplay_OnSetOutlet( &_this->NumberDisplayMax, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMaxPulse, OximeterDeviceClass_OnSetMaxPulse ));
  OximeterNumberDisplay_OnSetOutlet( &_this->NumberDisplayMin, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMinPulse, OximeterDeviceClass_OnSetMinPulse ));
  OximeterNumberDisplay_OnSetOutlet( &_this->NumberDisplayAlarm, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetAlarmTime, OximeterDeviceClass_OnSetAlarmTime ));
}

/* Re-Initializer for the class 'Oximeter::PulseSettings' */
void OximeterPulseSettings__ReInit( OximeterPulseSettings _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  WidgetSetRotaryKnob__ReInit( &_this->RotaryKnobMax );
  WidgetSetRotaryKnob__ReInit( &_this->RotaryKnobMin );
  WidgetSetHorizontalSlider__ReInit( &_this->HorizontalSlider );
  ViewsText__ReInit( &_this->TextAlarm );
  WidgetSetPushButton__ReInit( &_this->PushButtonCancel );
  WidgetSetPushButton__ReInit( &_this->PushButtonSave );
  ViewsText__ReInit( &_this->TextMax );
  ViewsText__ReInit( &_this->TextMin );
  OximeterNumberDisplay__ReInit( &_this->NumberDisplayMax );
  OximeterNumberDisplay__ReInit( &_this->NumberDisplayMin );
  OximeterNumberDisplay__ReInit( &_this->NumberDisplayAlarm );
}

/* Finalizer method for the class 'Oximeter::PulseSettings' */
void OximeterPulseSettings__Done( OximeterPulseSettings _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  WidgetSetRotaryKnob__Done( &_this->RotaryKnobMax );
  WidgetSetRotaryKnob__Done( &_this->RotaryKnobMin );
  WidgetSetHorizontalSlider__Done( &_this->HorizontalSlider );
  ViewsText__Done( &_this->TextAlarm );
  WidgetSetPushButton__Done( &_this->PushButtonCancel );
  WidgetSetPushButton__Done( &_this->PushButtonSave );
  ViewsText__Done( &_this->TextMax );
  ViewsText__Done( &_this->TextMin );
  OximeterNumberDisplay__Done( &_this->NumberDisplayMax );
  OximeterNumberDisplay__Done( &_this->NumberDisplayMin );
  OximeterNumberDisplay__Done( &_this->NumberDisplayAlarm );

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
void OximeterPulseSettings_UpdateLayout( OximeterPulseSettings _this, XPoint aSize )
{
  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );
  _this->oldMin = EwGetAutoObject( &OximeterDevice, OximeterDeviceClass )->MinPulse;
  _this->oldMax = EwGetAutoObject( &OximeterDevice, OximeterDeviceClass )->MaxPulse;
  _this->oldAlarm = EwGetAutoObject( &OximeterDevice, OximeterDeviceClass )->AlarmTime;
}

/* 'C' function for method : 'Oximeter::PulseSettings.onCancel()' */
void OximeterPulseSettings_onCancel( OximeterPulseSettings _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  OximeterDeviceClass_OnSetMinPulse( EwGetAutoObject( &OximeterDevice, OximeterDeviceClass ), 
  _this->oldMin );
  OximeterDeviceClass_OnSetMaxPulse( EwGetAutoObject( &OximeterDevice, OximeterDeviceClass ), 
  _this->oldMax );
  OximeterDeviceClass_OnSetAlarmTime( EwGetAutoObject( &OximeterDevice, OximeterDeviceClass ), 
  _this->oldAlarm );
  EwSignal( _this->OnClose, ((XObject)_this ));
}

/* 'C' function for method : 'Oximeter::PulseSettings.OnSetOnClose()' */
void OximeterPulseSettings_OnSetOnClose( OximeterPulseSettings _this, XSlot value )
{
  if ( !EwCompSlot( _this->OnClose, value ))
    return;

  _this->OnClose = value;
  _this->PushButtonSave.OnActivate = value;
}

/* Variants derived from the class : 'Oximeter::PulseSettings' */
EW_DEFINE_CLASS_VARIANTS( OximeterPulseSettings )
EW_END_OF_CLASS_VARIANTS( OximeterPulseSettings )

/* Virtual Method Table (VMT) for the class : 'Oximeter::PulseSettings' */
EW_DEFINE_CLASS( OximeterPulseSettings, CoreGroup, OnClose, OnClose, RotaryKnobMax, 
                 RotaryKnobMax, oldMin, oldMin, "Oximeter::PulseSettings" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  OximeterPulseSettings_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterPulseSettings )

/* User defined auto object: 'Oximeter::PushButtonConfig' */
EW_DEFINE_AUTOOBJECT( OximeterPushButtonConfig, WidgetSetPushButtonConfig )

/* Initializer for the auto object 'Oximeter::PushButtonConfig' */
void OximeterPushButtonConfig__Init( WidgetSetPushButtonConfig _this )
{
  WidgetSetPushButtonConfig_OnSetLabelColorActive( _this, _Const009F );
  WidgetSetPushButtonConfig_OnSetLabelColorFocused( _this, _Const0003 );
  WidgetSetPushButtonConfig_OnSetLabelColorDefault( _this, _Const0003 );
  WidgetSetPushButtonConfig_OnSetLabelFont( _this, EwLoadResource( &OximeterFontL, 
  ResourcesFont ));
  WidgetSetPushButtonConfig_OnSetFaceTintActive( _this, _Const0003 );
  WidgetSetPushButtonConfig_OnSetFaceTintFocused( _this, _Const0003 );
  WidgetSetPushButtonConfig_OnSetFaceTintDisabled( _this, _Const0002 );
  WidgetSetPushButtonConfig_OnSetFaceTintDefault( _this, _Const0003 );
  WidgetSetPushButtonConfig_OnSetFaceFrameActive( _this, 1 );
  WidgetSetPushButtonConfig_OnSetFaceFrameDisabled( _this, 0 );
  WidgetSetPushButtonConfig_OnSetFaceFrameDefault( _this, 0 );
  WidgetSetPushButtonConfig_OnSetFaceActive( _this, EwLoadResource( &OximeterPushButton, 
  ResourcesBitmap ));
  WidgetSetPushButtonConfig_OnSetFaceFocused( _this, EwLoadResource( &OximeterPushButton, 
  ResourcesBitmap ));
  WidgetSetPushButtonConfig_OnSetFaceDisabled( _this, EwLoadResource( &OximeterPushButton, 
  ResourcesBitmap ));
  WidgetSetPushButtonConfig_OnSetFaceDefault( _this, EwLoadResource( &OximeterPushButton, 
  ResourcesBitmap ));
  WidgetSetPushButtonConfig_OnSetWidgetMaxSize( _this, _Const00A0 );
  WidgetSetPushButtonConfig_OnSetWidgetMinSize( _this, _Const008A );
}

/* Table with links to derived variants of the auto object : 'Oximeter::PushButtonConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( OximeterPushButtonConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( OximeterPushButtonConfig )

/* Include a file containing the bitmap resource : 'Oximeter::PushButton' */
#include "_OximeterPushButton.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::PushButton' */
EW_RES_WITHOUT_VARIANTS( OximeterPushButton )

/* Initializer for the class 'Oximeter::NumberDisplay' */
void OximeterNumberDisplay__Init( OximeterNumberDisplay _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterNumberDisplay );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->DigitOnes, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->DigitTens, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->DigitHuns, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterNumberDisplay );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const00A1 );
  _this->CurrentValue = 50;
  CoreView_OnSetLayout((CoreView)&_this->DigitOnes, CoreLayoutAlignToBottom | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->DigitOnes, _Const00A2 );
  CoreView_OnSetLayout((CoreView)&_this->DigitTens, CoreLayoutAlignToBottom | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->DigitTens, _Const00A3 );
  CoreView_OnSetLayout((CoreView)&_this->DigitHuns, CoreLayoutAlignToBottom | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->DigitHuns, _Const00A2 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DigitOnes ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DigitTens ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DigitHuns ), 0 );
  ViewsImage_OnSetBitmap( &_this->DigitOnes, EwLoadResource( &OximeterDigitsSmall, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->DigitTens, EwLoadResource( &OximeterDigitsSmall, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->DigitHuns, EwLoadResource( &OximeterDigitsSmall, 
  ResourcesBitmap ));
}

/* Re-Initializer for the class 'Oximeter::NumberDisplay' */
void OximeterNumberDisplay__ReInit( OximeterNumberDisplay _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->DigitOnes );
  ViewsImage__ReInit( &_this->DigitTens );
  ViewsImage__ReInit( &_this->DigitHuns );
}

/* Finalizer method for the class 'Oximeter::NumberDisplay' */
void OximeterNumberDisplay__Done( OximeterNumberDisplay _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->DigitOnes );
  ViewsImage__Done( &_this->DigitTens );
  ViewsImage__Done( &_this->DigitHuns );

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
void OximeterNumberDisplay_UpdateViewState( OximeterNumberDisplay _this, XSet aState )
{
  XInt32 noOfDigits;
  XInt32 number;
  XInt32 x;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  noOfDigits = 1;
  number = _this->CurrentValue;
  ViewsImage_OnSetFrameNumber( &_this->DigitOnes, number % 10 );
  number = number / 10;

  if ( number > 0 )
  {
    noOfDigits = 2;
    ViewsImage_OnSetFrameNumber( &_this->DigitTens, number % 10 );
  }

  number = number / 10;

  if ( number > 0 )
  {
    noOfDigits = 3;
    ViewsImage_OnSetFrameNumber( &_this->DigitHuns, number % 10 );
  }

  x = ( EwGetRectW( _this->Super2.Bounds ) - ( noOfDigits * EwGetRectW( _this->DigitOnes.Super1.Bounds ))) 
  / 2;

  if ( noOfDigits > 2 )
  {
    CoreRectView__OnSetBounds( &_this->DigitHuns, EwSetRectX( _this->DigitHuns.Super1.Bounds, 
    x ));
    ViewsImage_OnSetVisible( &_this->DigitHuns, 1 );
    x = x + EwGetRectW( _this->DigitOnes.Super1.Bounds );
  }
  else
    ViewsImage_OnSetVisible( &_this->DigitHuns, 0 );

  if ( noOfDigits > 1 )
  {
    CoreRectView__OnSetBounds( &_this->DigitTens, EwSetRectX( _this->DigitTens.Super1.Bounds, 
    x ));
    ViewsImage_OnSetVisible( &_this->DigitTens, 1 );
    x = x + EwGetRectW( _this->DigitOnes.Super1.Bounds );
  }
  else
    ViewsImage_OnSetVisible( &_this->DigitTens, 0 );

  CoreRectView__OnSetBounds( &_this->DigitOnes, EwSetRectX( _this->DigitOnes.Super1.Bounds, 
  x ));
}

/* 'C' function for method : 'Oximeter::NumberDisplay.OnSetCurrentValue()' */
void OximeterNumberDisplay_OnSetCurrentValue( OximeterNumberDisplay _this, XInt32 
  value )
{
  if ( value < 0 )
    value = 0;

  if ( value > 999 )
    value = 999;

  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void OximeterNumberDisplay_outletSlot( OximeterNumberDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    OximeterNumberDisplay_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'Oximeter::NumberDisplay.OnSetOutlet()' */
void OximeterNumberDisplay_OnSetOutlet( OximeterNumberDisplay _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, OximeterNumberDisplay_outletSlot ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, OximeterNumberDisplay_outletSlot ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, OximeterNumberDisplay_outletSlot ), ((XObject)_this ));
}

/* Variants derived from the class : 'Oximeter::NumberDisplay' */
EW_DEFINE_CLASS_VARIANTS( OximeterNumberDisplay )
EW_END_OF_CLASS_VARIANTS( OximeterNumberDisplay )

/* Virtual Method Table (VMT) for the class : 'Oximeter::NumberDisplay' */
EW_DEFINE_CLASS( OximeterNumberDisplay, CoreGroup, Outlet, Outlet, Outlet, DigitOnes, 
                 CurrentValue, CurrentValue, "Oximeter::NumberDisplay" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  OximeterNumberDisplay_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterNumberDisplay )

/* Initializer for the class 'Oximeter::OxygenSettings' */
void OximeterOxygenSettings__Init( OximeterOxygenSettings _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterOxygenSettings );

  /* ... then construct all embedded objects */
  WidgetSetRotaryKnob__Init( &_this->RotaryKnobMax, &_this->_XObject, 0 );
  WidgetSetRotaryKnob__Init( &_this->RotaryKnobMin, &_this->_XObject, 0 );
  WidgetSetHorizontalSlider__Init( &_this->HorizontalSlider, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextAlarm, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->PushButtonCancel, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->PushButtonSave, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextMax, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextMin, &_this->_XObject, 0 );
  OximeterNumberDisplay__Init( &_this->NumberDisplayMax, &_this->_XObject, 0 );
  OximeterNumberDisplay__Init( &_this->NumberDisplayMin, &_this->_XObject, 0 );
  OximeterNumberDisplay__Init( &_this->NumberDisplayAlarm, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterOxygenSettings );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const008E );
  CoreRectView__OnSetBounds( &_this->RotaryKnobMax, _Const00A4 );
  WidgetSetRotaryKnob_OnSetMaxValue( &_this->RotaryKnobMax, 80 );
  WidgetSetRotaryKnob_OnSetMinValue( &_this->RotaryKnobMax, 0 );
  CoreRectView__OnSetBounds( &_this->RotaryKnobMin, _Const00A5 );
  WidgetSetRotaryKnob_OnSetMaxValue( &_this->RotaryKnobMin, 95 );
  WidgetSetRotaryKnob_OnSetMinValue( &_this->RotaryKnobMin, 75 );
  CoreRectView__OnSetBounds( &_this->HorizontalSlider, _Const00A6 );
  WidgetSetHorizontalSlider_OnSetMaxValue( &_this->HorizontalSlider, 180 );
  CoreRectView__OnSetBounds( &_this->TextAlarm, _Const00A7 );
  ViewsText_OnSetAlignment( &_this->TextAlarm, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextAlarm, EwLoadString( &_Const0093 ));
  CoreRectView__OnSetBounds( &_this->PushButtonCancel, _Const00A8 );
  WidgetSetPushButton_OnSetLabel( &_this->PushButtonCancel, EwLoadString( &_Const0095 ));
  CoreRectView__OnSetBounds( &_this->PushButtonSave, _Const00A9 );
  WidgetSetPushButton_OnSetLabel( &_this->PushButtonSave, EwLoadString( &_Const0097 ));
  CoreRectView__OnSetBounds( &_this->TextMax, _Const00AA );
  ViewsText_OnSetAlignment( &_this->TextMax, ViewsTextAlignmentAlignHorzCenter | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextMax, EwLoadString( &_Const00AB ));
  CoreRectView__OnSetBounds( &_this->TextMin, _Const00AC );
  ViewsText_OnSetAlignment( &_this->TextMin, ViewsTextAlignmentAlignHorzCenter | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextMin, EwLoadString( &_Const00AD ));
  CoreRectView__OnSetBounds( &_this->NumberDisplayMax, _Const00AE );
  CoreRectView__OnSetBounds( &_this->NumberDisplayMin, _Const00AF );
  CoreRectView__OnSetBounds( &_this->NumberDisplayAlarm, _Const00B0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RotaryKnobMax ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RotaryKnobMin ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->HorizontalSlider ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextAlarm ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PushButtonCancel ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PushButtonSave ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextMax ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextMin ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->NumberDisplayMax ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->NumberDisplayMin ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->NumberDisplayAlarm ), 0 );
  WidgetSetRotaryKnob_OnSetOutlet( &_this->RotaryKnobMax, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetOxygenFlow, OximeterDeviceClass_OnSetOxygenFlow ));
  WidgetSetRotaryKnob_OnSetAppearance( &_this->RotaryKnobMax, EwGetAutoObject( &OximeterRotaryFlowConfig, 
  WidgetSetRotaryKnobConfig ));
  WidgetSetRotaryKnob_OnSetOutlet( &_this->RotaryKnobMin, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMinOxygen, OximeterDeviceClass_OnSetMinOxygen ));
  WidgetSetRotaryKnob_OnSetAppearance( &_this->RotaryKnobMin, EwGetAutoObject( &OximeterRotaryOxiConfig, 
  WidgetSetRotaryKnobConfig ));
  WidgetSetHorizontalSlider_OnSetOutlet( &_this->HorizontalSlider, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetAlarmTime, OximeterDeviceClass_OnSetAlarmTime ));
  WidgetSetHorizontalSlider_OnSetAppearance( &_this->HorizontalSlider, EwGetAutoObject( 
  &OximeterHorizontalSliderConfig, WidgetSetHorizontalSliderConfig ));
  ViewsText_OnSetFont( &_this->TextAlarm, EwLoadResource( &OximeterFontL, ResourcesFont ));
  _this->PushButtonCancel.OnActivate = EwNewSlot( _this, OximeterOxygenSettings_onCancel );
  WidgetSetPushButton_OnSetAppearance( &_this->PushButtonCancel, EwGetAutoObject( 
  &OximeterPushButtonConfig, WidgetSetPushButtonConfig ));
  WidgetSetPushButton_OnSetAppearance( &_this->PushButtonSave, EwGetAutoObject( 
  &OximeterPushButtonConfig, WidgetSetPushButtonConfig ));
  ViewsText_OnSetFont( &_this->TextMax, EwLoadResource( &OximeterFontL, ResourcesFont ));
  ViewsText_OnSetFont( &_this->TextMin, EwLoadResource( &OximeterFontL, ResourcesFont ));
  OximeterNumberDisplay_OnSetOutlet( &_this->NumberDisplayMax, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetOxygenFlow, OximeterDeviceClass_OnSetOxygenFlow ));
  OximeterNumberDisplay_OnSetOutlet( &_this->NumberDisplayMin, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetMinOxygen, OximeterDeviceClass_OnSetMinOxygen ));
  OximeterNumberDisplay_OnSetOutlet( &_this->NumberDisplayAlarm, EwNewRef( EwGetAutoObject( 
  &OximeterDevice, OximeterDeviceClass ), OximeterDeviceClass_OnGetAlarmTime, OximeterDeviceClass_OnSetAlarmTime ));
}

/* Re-Initializer for the class 'Oximeter::OxygenSettings' */
void OximeterOxygenSettings__ReInit( OximeterOxygenSettings _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  WidgetSetRotaryKnob__ReInit( &_this->RotaryKnobMax );
  WidgetSetRotaryKnob__ReInit( &_this->RotaryKnobMin );
  WidgetSetHorizontalSlider__ReInit( &_this->HorizontalSlider );
  ViewsText__ReInit( &_this->TextAlarm );
  WidgetSetPushButton__ReInit( &_this->PushButtonCancel );
  WidgetSetPushButton__ReInit( &_this->PushButtonSave );
  ViewsText__ReInit( &_this->TextMax );
  ViewsText__ReInit( &_this->TextMin );
  OximeterNumberDisplay__ReInit( &_this->NumberDisplayMax );
  OximeterNumberDisplay__ReInit( &_this->NumberDisplayMin );
  OximeterNumberDisplay__ReInit( &_this->NumberDisplayAlarm );
}

/* Finalizer method for the class 'Oximeter::OxygenSettings' */
void OximeterOxygenSettings__Done( OximeterOxygenSettings _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  WidgetSetRotaryKnob__Done( &_this->RotaryKnobMax );
  WidgetSetRotaryKnob__Done( &_this->RotaryKnobMin );
  WidgetSetHorizontalSlider__Done( &_this->HorizontalSlider );
  ViewsText__Done( &_this->TextAlarm );
  WidgetSetPushButton__Done( &_this->PushButtonCancel );
  WidgetSetPushButton__Done( &_this->PushButtonSave );
  ViewsText__Done( &_this->TextMax );
  ViewsText__Done( &_this->TextMin );
  OximeterNumberDisplay__Done( &_this->NumberDisplayMax );
  OximeterNumberDisplay__Done( &_this->NumberDisplayMin );
  OximeterNumberDisplay__Done( &_this->NumberDisplayAlarm );

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
void OximeterOxygenSettings_UpdateLayout( OximeterOxygenSettings _this, XPoint aSize )
{
  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );
  _this->oldMin = EwGetAutoObject( &OximeterDevice, OximeterDeviceClass )->MinPulse;
  _this->oldMax = EwGetAutoObject( &OximeterDevice, OximeterDeviceClass )->MaxPulse;
  _this->oldAlarm = EwGetAutoObject( &OximeterDevice, OximeterDeviceClass )->AlarmTime;
}

/* 'C' function for method : 'Oximeter::OxygenSettings.onCancel()' */
void OximeterOxygenSettings_onCancel( OximeterOxygenSettings _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  OximeterDeviceClass_OnSetMinPulse( EwGetAutoObject( &OximeterDevice, OximeterDeviceClass ), 
  _this->oldMin );
  OximeterDeviceClass_OnSetMaxPulse( EwGetAutoObject( &OximeterDevice, OximeterDeviceClass ), 
  _this->oldMax );
  OximeterDeviceClass_OnSetAlarmTime( EwGetAutoObject( &OximeterDevice, OximeterDeviceClass ), 
  _this->oldAlarm );
  EwSignal( _this->OnClose, ((XObject)_this ));
}

/* 'C' function for method : 'Oximeter::OxygenSettings.OnSetOnClose()' */
void OximeterOxygenSettings_OnSetOnClose( OximeterOxygenSettings _this, XSlot value )
{
  if ( !EwCompSlot( _this->OnClose, value ))
    return;

  _this->OnClose = value;
  _this->PushButtonSave.OnActivate = value;
}

/* Variants derived from the class : 'Oximeter::OxygenSettings' */
EW_DEFINE_CLASS_VARIANTS( OximeterOxygenSettings )
EW_END_OF_CLASS_VARIANTS( OximeterOxygenSettings )

/* Virtual Method Table (VMT) for the class : 'Oximeter::OxygenSettings' */
EW_DEFINE_CLASS( OximeterOxygenSettings, CoreGroup, OnClose, OnClose, RotaryKnobMax, 
                 RotaryKnobMax, oldMin, oldMin, "Oximeter::OxygenSettings" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  OximeterOxygenSettings_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterOxygenSettings )

/* Include a file containing the bitmap resource : 'Oximeter::Battery' */
#include "_OximeterBattery.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::Battery' */
EW_RES_WITHOUT_VARIANTS( OximeterBattery )

/* Include a file containing the bitmap resource : 'Oximeter::MenuIcons' */
#include "_OximeterMenuIcons.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::MenuIcons' */
EW_RES_WITHOUT_VARIANTS( OximeterMenuIcons )

/* User defined auto object: 'Oximeter::MenuItemConfig' */
EW_DEFINE_AUTOOBJECT( OximeterMenuItemConfig, WidgetSetPushButtonConfig )

/* Initializer for the auto object 'Oximeter::MenuItemConfig' */
void OximeterMenuItemConfig__Init( WidgetSetPushButtonConfig _this )
{
  WidgetSetPushButtonConfig_OnSetLabelColorActive( _this, _Const0003 );
  WidgetSetPushButtonConfig_OnSetLabelColorFocused( _this, _Const0002 );
  WidgetSetPushButtonConfig_OnSetLabelColorDefault( _this, _Const0002 );
  WidgetSetPushButtonConfig_OnSetLabelMarginTop( _this, 75 );
  WidgetSetPushButtonConfig_OnSetIconMarginBottom( _this, 30 );
  WidgetSetPushButtonConfig_OnSetLabelAlignment( _this, ViewsTextAlignmentAlignHorzCenter 
  | ViewsTextAlignmentAlignVertCenter );
  WidgetSetPushButtonConfig_OnSetLabelFont( _this, EwLoadResource( &OximeterFontM, 
  ResourcesFont ));
  WidgetSetPushButtonConfig_OnSetIconAlignment( _this, ViewsImageAlignmentAlignHorzCenter 
  | ViewsImageAlignmentAlignVertCenter );
  WidgetSetPushButtonConfig_OnSetFaceTintFocused( _this, _Const00B1 );
  WidgetSetPushButtonConfig_OnSetFaceTintDefault( _this, _Const00B1 );
  WidgetSetPushButtonConfig_OnSetFaceActive( _this, EwLoadResource( &OximeterMenuButton, 
  ResourcesBitmap ));
  WidgetSetPushButtonConfig_OnSetFaceFocused( _this, EwLoadResource( &OximeterMenuButton, 
  ResourcesBitmap ));
  WidgetSetPushButtonConfig_OnSetFaceDefault( _this, EwLoadResource( &OximeterMenuButton, 
  ResourcesBitmap ));
  WidgetSetPushButtonConfig_OnSetWidgetMaxSize( _this, _Const00B2 );
  WidgetSetPushButtonConfig_OnSetWidgetMinSize( _this, _Const00B3 );
}

/* Table with links to derived variants of the auto object : 'Oximeter::MenuItemConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( OximeterMenuItemConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( OximeterMenuItemConfig )

/* Include a file containing the bitmap resource : 'Oximeter::MenuButton' */
#include "_OximeterMenuButton.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::MenuButton' */
EW_RES_WITHOUT_VARIANTS( OximeterMenuButton )

/* Initializer for the class 'Oximeter::MainMenu' */
void OximeterMainMenu__Init( OximeterMainMenu _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( OximeterMainMenu );

  /* ... then construct all embedded objects */
  WidgetSetPushButton__Init( &_this->CloseButton, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextMax, &_this->_XObject, 0 );
  ViewsText__Init( &_this->AlertText, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->MenuItem1, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->MenuItem2, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->MenuItem3, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->MenuItem4, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->MenuItem5, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->MenuItem6, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->MenuItem7, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->MenuItem8, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextMax1, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->MenuItem9, &_this->_XObject, 0 );
  ViewsText__Init( &_this->TextMax2, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->AlertEffect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( OximeterMainMenu );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0016 );
  CoreRectView__OnSetBounds( &_this->CloseButton, _Const00B4 );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->CloseButton, 1 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->CloseButton, 1 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->CloseButton, 1 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->CloseButton, 1 );
  CoreRectView__OnSetBounds( &_this->TextMax, _Const00B5 );
  ViewsText_OnSetAlignment( &_this->TextMax, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextMax, EwLoadString( &_Const00B6 ));
  CoreRectView__OnSetBounds( &_this->AlertText, _Const00B7 );
  ViewsText_OnSetWrapText( &_this->AlertText, 1 );
  ViewsText_OnSetAlignment( &_this->AlertText, ViewsTextAlignmentAlignHorzCenter 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->AlertText, EwLoadString( &_Const00B8 ));
  ViewsText_OnSetVisible( &_this->AlertText, 0 );
  CoreRectView__OnSetBounds( &_this->MenuItem1, _Const00B9 );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->MenuItem1, 10 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->MenuItem1, 0 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->MenuItem1, 0 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->MenuItem1, 0 );
  WidgetSetPushButton_OnSetLabel( &_this->MenuItem1, EwLoadString( &_Const00BA ));
  CoreRectView__OnSetBounds( &_this->MenuItem2, _Const00BB );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->MenuItem2, 11 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->MenuItem2, 1 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->MenuItem2, 1 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->MenuItem2, 1 );
  WidgetSetPushButton_OnSetLabel( &_this->MenuItem2, EwLoadString( &_Const00BC ));
  CoreRectView__OnSetBounds( &_this->MenuItem3, _Const00BD );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->MenuItem3, 12 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->MenuItem3, 2 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->MenuItem3, 2 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->MenuItem3, 2 );
  WidgetSetPushButton_OnSetLabel( &_this->MenuItem3, EwLoadString( &_Const00BE ));
  CoreRectView__OnSetBounds( &_this->MenuItem4, _Const00BF );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->MenuItem4, 13 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->MenuItem4, 3 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->MenuItem4, 3 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->MenuItem4, 3 );
  WidgetSetPushButton_OnSetLabel( &_this->MenuItem4, EwLoadString( &_Const00C0 ));
  CoreRectView__OnSetBounds( &_this->MenuItem5, _Const00C1 );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->MenuItem5, 14 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->MenuItem5, 4 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->MenuItem5, 4 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->MenuItem5, 4 );
  WidgetSetPushButton_OnSetLabel( &_this->MenuItem5, EwLoadString( &_Const00C2 ));
  CoreRectView__OnSetBounds( &_this->MenuItem6, _Const00C3 );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->MenuItem6, 15 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->MenuItem6, 5 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->MenuItem6, 5 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->MenuItem6, 5 );
  WidgetSetPushButton_OnSetLabel( &_this->MenuItem6, EwLoadString( &_Const00C4 ));
  CoreRectView__OnSetBounds( &_this->MenuItem7, _Const00C5 );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->MenuItem7, 16 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->MenuItem7, 6 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->MenuItem7, 6 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->MenuItem7, 6 );
  WidgetSetPushButton_OnSetLabel( &_this->MenuItem7, EwLoadString( &_Const00C6 ));
  CoreRectView__OnSetBounds( &_this->MenuItem8, _Const00C7 );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->MenuItem8, 17 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->MenuItem8, 7 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->MenuItem8, 7 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->MenuItem8, 7 );
  WidgetSetPushButton_OnSetLabel( &_this->MenuItem8, EwLoadString( &_Const00C8 ));
  CoreRectView__OnSetBounds( &_this->TextMax1, _Const00C9 );
  ViewsText_OnSetAlignment( &_this->TextMax1, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextMax1, EwLoadString( &_Const00CA ));
  CoreRectView__OnSetBounds( &_this->MenuItem9, _Const00CB );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->MenuItem9, 18 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->MenuItem9, 8 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->MenuItem9, 8 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->MenuItem9, 8 );
  WidgetSetPushButton_OnSetLabel( &_this->MenuItem9, EwLoadString( &_Const00CC ));
  CoreRectView__OnSetBounds( &_this->TextMax2, _Const00CD );
  ViewsText_OnSetAlignment( &_this->TextMax2, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->TextMax2, EwLoadString( &_Const00CE ));
  _this->AlertEffect.Super1.Symmetric = 1;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->AlertEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->AlertEffect, 5 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->AlertEffect, 1000 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CloseButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextMax ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->AlertText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MenuItem1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MenuItem2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MenuItem3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MenuItem4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MenuItem5 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MenuItem6 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MenuItem7 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MenuItem8 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextMax1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MenuItem9 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TextMax2 ), 0 );
  WidgetSetPushButton_OnSetIcon( &_this->CloseButton, EwLoadResource( &OximeterIconButton, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->CloseButton, EwGetAutoObject( &OximeterIconButtonConfig, 
  WidgetSetPushButtonConfig ));
  ViewsText_OnSetFont( &_this->TextMax, EwLoadResource( &OximeterFontL, ResourcesFont ));
  ViewsText_OnSetFont( &_this->AlertText, EwLoadResource( &OximeterFontL, ResourcesFont ));
  _this->MenuItem1.OnActivate = EwNewSlot( _this, OximeterMainMenu_menuClick );
  WidgetSetPushButton_OnSetIcon( &_this->MenuItem1, EwLoadResource( &OximeterMenuIcons, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->MenuItem1, EwGetAutoObject( &OximeterMenuItemConfig, 
  WidgetSetPushButtonConfig ));
  _this->MenuItem2.OnActivate = EwNewSlot( _this, OximeterMainMenu_menuClick );
  WidgetSetPushButton_OnSetIcon( &_this->MenuItem2, EwLoadResource( &OximeterMenuIcons, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->MenuItem2, EwGetAutoObject( &OximeterMenuItemConfig, 
  WidgetSetPushButtonConfig ));
  _this->MenuItem3.OnActivate = EwNewSlot( _this, OximeterMainMenu_menuClick );
  WidgetSetPushButton_OnSetIcon( &_this->MenuItem3, EwLoadResource( &OximeterMenuIcons, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->MenuItem3, EwGetAutoObject( &OximeterMenuItemConfig, 
  WidgetSetPushButtonConfig ));
  _this->MenuItem4.OnActivate = EwNewSlot( _this, OximeterMainMenu_menuClick );
  WidgetSetPushButton_OnSetIcon( &_this->MenuItem4, EwLoadResource( &OximeterMenuIcons, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->MenuItem4, EwGetAutoObject( &OximeterMenuItemConfig, 
  WidgetSetPushButtonConfig ));
  _this->MenuItem5.OnActivate = EwNewSlot( _this, OximeterMainMenu_menuClick );
  WidgetSetPushButton_OnSetIcon( &_this->MenuItem5, EwLoadResource( &OximeterMenuIcons, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->MenuItem5, EwGetAutoObject( &OximeterMenuItemConfig, 
  WidgetSetPushButtonConfig ));
  _this->MenuItem6.OnActivate = EwNewSlot( _this, OximeterMainMenu_menuClick );
  WidgetSetPushButton_OnSetIcon( &_this->MenuItem6, EwLoadResource( &OximeterMenuIcons, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->MenuItem6, EwGetAutoObject( &OximeterMenuItemConfig, 
  WidgetSetPushButtonConfig ));
  _this->MenuItem7.OnActivate = EwNewSlot( _this, OximeterMainMenu_menuClick );
  WidgetSetPushButton_OnSetIcon( &_this->MenuItem7, EwLoadResource( &OximeterMenuIcons, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->MenuItem7, EwGetAutoObject( &OximeterMenuItemConfig, 
  WidgetSetPushButtonConfig ));
  _this->MenuItem8.OnActivate = EwNewSlot( _this, OximeterMainMenu_menuClick );
  WidgetSetPushButton_OnSetIcon( &_this->MenuItem8, EwLoadResource( &OximeterMenuIcons, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->MenuItem8, EwGetAutoObject( &OximeterMenuItemConfig, 
  WidgetSetPushButtonConfig ));
  ViewsText_OnSetFont( &_this->TextMax1, EwLoadResource( &OximeterFontL, ResourcesFont ));
  _this->MenuItem9.OnActivate = EwNewSlot( _this, OximeterMainMenu_menuClick );
  WidgetSetPushButton_OnSetIcon( &_this->MenuItem9, EwLoadResource( &OximeterMenuIcons, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->MenuItem9, EwGetAutoObject( &OximeterMenuItemConfig, 
  WidgetSetPushButtonConfig ));
  ViewsText_OnSetFont( &_this->TextMax2, EwLoadResource( &OximeterFontL, ResourcesFont ));
  _this->AlertEffect.Outlet = EwNewRef( &_this->AlertText, ViewsText_OnGetOpacity, 
  ViewsText_OnSetOpacity );
}

/* Re-Initializer for the class 'Oximeter::MainMenu' */
void OximeterMainMenu__ReInit( OximeterMainMenu _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  WidgetSetPushButton__ReInit( &_this->CloseButton );
  ViewsText__ReInit( &_this->TextMax );
  ViewsText__ReInit( &_this->AlertText );
  WidgetSetPushButton__ReInit( &_this->MenuItem1 );
  WidgetSetPushButton__ReInit( &_this->MenuItem2 );
  WidgetSetPushButton__ReInit( &_this->MenuItem3 );
  WidgetSetPushButton__ReInit( &_this->MenuItem4 );
  WidgetSetPushButton__ReInit( &_this->MenuItem5 );
  WidgetSetPushButton__ReInit( &_this->MenuItem6 );
  WidgetSetPushButton__ReInit( &_this->MenuItem7 );
  WidgetSetPushButton__ReInit( &_this->MenuItem8 );
  ViewsText__ReInit( &_this->TextMax1 );
  WidgetSetPushButton__ReInit( &_this->MenuItem9 );
  ViewsText__ReInit( &_this->TextMax2 );
  EffectsInt32Effect__ReInit( &_this->AlertEffect );
}

/* Finalizer method for the class 'Oximeter::MainMenu' */
void OximeterMainMenu__Done( OximeterMainMenu _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  WidgetSetPushButton__Done( &_this->CloseButton );
  ViewsText__Done( &_this->TextMax );
  ViewsText__Done( &_this->AlertText );
  WidgetSetPushButton__Done( &_this->MenuItem1 );
  WidgetSetPushButton__Done( &_this->MenuItem2 );
  WidgetSetPushButton__Done( &_this->MenuItem3 );
  WidgetSetPushButton__Done( &_this->MenuItem4 );
  WidgetSetPushButton__Done( &_this->MenuItem5 );
  WidgetSetPushButton__Done( &_this->MenuItem6 );
  WidgetSetPushButton__Done( &_this->MenuItem7 );
  WidgetSetPushButton__Done( &_this->MenuItem8 );
  ViewsText__Done( &_this->TextMax1 );
  WidgetSetPushButton__Done( &_this->MenuItem9 );
  ViewsText__Done( &_this->TextMax2 );
  EffectsInt32Effect__Done( &_this->AlertEffect );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Oximeter::MainMenu.OnSetOnClose()' */
void OximeterMainMenu_OnSetOnClose( OximeterMainMenu _this, XSlot value )
{
  if ( !EwCompSlot( _this->OnClose, value ))
    return;

  _this->OnClose = value;
  _this->CloseButton.OnActivate = value;
}

/* 'C' function for method : 'Oximeter::MainMenu.menuClick()' */
void OximeterMainMenu_menuClick( OximeterMainMenu _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  ViewsText_OnSetVisible( &_this->AlertText, 1 );
  ViewsText_OnSetOpacity( &_this->AlertText, 0 );
  EwPostSignal( EwNewSlot( &_this->AlertEffect, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* Variants derived from the class : 'Oximeter::MainMenu' */
EW_DEFINE_CLASS_VARIANTS( OximeterMainMenu )
EW_END_OF_CLASS_VARIANTS( OximeterMainMenu )

/* Virtual Method Table (VMT) for the class : 'Oximeter::MainMenu' */
EW_DEFINE_CLASS( OximeterMainMenu, CoreGroup, OnClose, OnClose, CloseButton, CloseButton, 
                 _None, _None, "Oximeter::MainMenu" )
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( OximeterMainMenu )

/* Include a file containing the bitmap resource : 'Oximeter::RotaryBackgroundOxygen' */
#include "_OximeterRotaryBackgroundOxygen.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::RotaryBackgroundOxygen' */
EW_RES_WITHOUT_VARIANTS( OximeterRotaryBackgroundOxygen )

/* User defined auto object: 'Oximeter::RotaryOxiConfig' */
EW_DEFINE_AUTOOBJECT( OximeterRotaryOxiConfig, WidgetSetRotaryKnobConfig )

/* Initializer for the auto object 'Oximeter::RotaryOxiConfig' */
void OximeterRotaryOxiConfig__Init( WidgetSetRotaryKnobConfig _this )
{
  WidgetSetRotaryKnobConfig_OnSetThumbMaxAngle( _this, 0.000000f );
  WidgetSetRotaryKnobConfig_OnSetThumbMinAngle( _this, 90.000000f );
  WidgetSetRotaryKnobConfig_OnSetThumbPivot( _this, _Const0059 );
  WidgetSetRotaryKnobConfig_OnSetThumbActive( _this, EwLoadResource( &OximeterRotaryThumbMin, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetThumbFocused( _this, EwLoadResource( &OximeterRotaryThumbMin, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetThumbDefault( _this, EwLoadResource( &OximeterRotaryThumbMin, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleActive( _this, EwLoadResource( &OximeterRotaryBackgroundOxygen, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleFocused( _this, EwLoadResource( &OximeterRotaryBackgroundOxygen, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleDisabled( _this, EwLoadResource( &OximeterRotaryBackgroundOxygen, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleDefault( _this, EwLoadResource( &OximeterRotaryBackgroundOxygen, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetWidgetMaxSize( _this, _Const0058 );
  WidgetSetRotaryKnobConfig_OnSetWidgetMinSize( _this, _Const0058 );
}

/* Table with links to derived variants of the auto object : 'Oximeter::RotaryOxiConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( OximeterRotaryOxiConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( OximeterRotaryOxiConfig )

/* User defined auto object: 'Oximeter::RotaryFlowConfig' */
EW_DEFINE_AUTOOBJECT( OximeterRotaryFlowConfig, WidgetSetRotaryKnobConfig )

/* Initializer for the auto object 'Oximeter::RotaryFlowConfig' */
void OximeterRotaryFlowConfig__Init( WidgetSetRotaryKnobConfig _this )
{
  WidgetSetRotaryKnobConfig_OnSetThumbMaxAngle( _this, 90.000000f );
  WidgetSetRotaryKnobConfig_OnSetThumbMinAngle( _this, 358.000000f );
  WidgetSetRotaryKnobConfig_OnSetThumbPivot( _this, _Const0059 );
  WidgetSetRotaryKnobConfig_OnSetThumbActive( _this, EwLoadResource( &OximeterRotaryThumbMin, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetThumbFocused( _this, EwLoadResource( &OximeterRotaryThumbMin, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetThumbDefault( _this, EwLoadResource( &OximeterRotaryThumbMin, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleActive( _this, EwLoadResource( &OximeterRotaryBackgroundFlow, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleFocused( _this, EwLoadResource( &OximeterRotaryBackgroundFlow, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleDisabled( _this, EwLoadResource( &OximeterRotaryBackgroundFlow, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetScaleDefault( _this, EwLoadResource( &OximeterRotaryBackgroundFlow, 
  ResourcesBitmap ));
  WidgetSetRotaryKnobConfig_OnSetWidgetMaxSize( _this, _Const0058 );
  WidgetSetRotaryKnobConfig_OnSetWidgetMinSize( _this, _Const0058 );
}

/* Table with links to derived variants of the auto object : 'Oximeter::RotaryFlowConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( OximeterRotaryFlowConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( OximeterRotaryFlowConfig )

/* Include a file containing the bitmap resource : 'Oximeter::RotaryBackgroundFlow' */
#include "_OximeterRotaryBackgroundFlow.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::RotaryBackgroundFlow' */
EW_RES_WITHOUT_VARIANTS( OximeterRotaryBackgroundFlow )

/* Include a file containing the bitmap resource : 'Oximeter::DigitsLarge' */
#include "_OximeterDigitsLarge.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::DigitsLarge' */
EW_RES_WITHOUT_VARIANTS( OximeterDigitsLarge )

/* Include a file containing the bitmap resource : 'Oximeter::DigitsSmall' */
#include "_OximeterDigitsSmall.h"

/* Table with links to derived variants of the bitmap resource : 'Oximeter::DigitsSmall' */
EW_RES_WITHOUT_VARIANTS( OximeterDigitsSmall )

/* Embedded Wizard */
