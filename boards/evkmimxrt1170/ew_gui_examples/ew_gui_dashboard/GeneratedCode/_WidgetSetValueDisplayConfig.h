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

#ifndef _WidgetSetValueDisplayConfig_H
#define _WidgetSetValueDisplayConfig_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x000A0000
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x000A0000
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

#include "_WidgetSetWidgetConfig.h"

/* Forward declaration of the class Resources::Font */
#ifndef _ResourcesFont_
  EW_DECLARE_CLASS( ResourcesFont )
#define _ResourcesFont_
#endif

/* Forward declaration of the class WidgetSet::ValueDisplayConfig */
#ifndef _WidgetSetValueDisplayConfig_
  EW_DECLARE_CLASS( WidgetSetValueDisplayConfig )
#define _WidgetSetValueDisplayConfig_
#endif


/* This class implements the functionality permitting you to simply customize the 
   look and feel of a 'value display' widget (WidgetSet::ValueDisplay). In the practice, 
   you will create an instance of this class, configure its properties with fonts, 
   colors and other relevant attributes according to your design expectations and 
   assign such prepared configuration object to the property 'Appearance' of every 
   affected value display widget. Thereupon the widgets will use the configuration 
   information provided in the object.
   During its lifetime the value display widget remains always in one of the both 
   states: 'positive' or 'negative'. The state 'positive' is true if the value displayed 
   in the widget is greater than or equal to 0 (zero). The state 'negative' is true, 
   if the value is less than 0 (zero). With the configuration object you can specify 
   the appearance of the value display widget for every state individually. For 
   this purpose you should know from which views the widget is composed of:
   - 'Value' is a text view (Views::Text) displayed per default in the center of 
   the widget. The corresponding text is formatted from the number stored in the 
   property 'CurrentValue' in the value display widget. In the configuration object 
   you can determine the font (@ValueFont), alignment (@ValueAlignment) and margins 
   (@ValueMarginBottom, @ValueMarginLeft, @ValueMarginRight, @ValueMarginTop) to 
   use for the text view. For every widget state you can specify individual text 
   color values (@ValueColorPositive and @ValueColorNegative).
   - 'Unit' is a text view (Views::Text) displayed per default in the center of 
   the widget. The corresponding text is specified in the property 'Unit' in the 
   value display widget. In the configuration object you can determine the font 
   (@UnitFont), alignment (@UnitAlignment) and margins (@UnitMarginBottom, @UnitMarginLeft, 
   @UnitMarginRight, @UnitMarginTop) to use for the text view. For every widget 
   state you can specify individual text color values (@UnitColorPositive and @UnitColorNegative).
   Per default the both views 'Value' and 'Unit' are arranged within the widget 
   area independently. With the property @Layout you can specify other modes to 
   e.g. arrange the unit to appear appended at the right edge of the displayed value; 
   etc.
   The configuration object provides various properties to localize the format operation 
   how the number is presented. In particular, with the properties @FormatDecimalSign 
   and @FormatDigitGroupSign you can specify the signs or strings to be used as 
   decimal separator sign and digit group sign. With the properties @FormatMinusSign 
   and @FormatPlusSign you can determine which signs or strings should appear to 
   sign positive or negative numbers.
   With the properties @WidgetMinSize and @WidgetMaxSize you can configure size 
   constraints for the widget itself. You can, for example, limit the value display 
   widget to not shrink below a specified width or height. */
EW_DEFINE_FIELDS( WidgetSetValueDisplayConfig, WidgetSetWidgetConfig )
  EW_PROPERTY( ValueFont,       ResourcesFont )
  EW_PROPERTY( UnitFont,        ResourcesFont )
  EW_PROPERTY( FormatDigitGroupSign, XString )
  EW_PROPERTY( FormatDecimalSign, XString )
  EW_PROPERTY( FormatMinusSign, XString )
  EW_PROPERTY( FormatPlusSign,  XString )
  EW_PROPERTY( ValueMarginBottom, XInt32 )
  EW_PROPERTY( ValueMarginTop,  XInt32 )
  EW_PROPERTY( ValueMarginRight, XInt32 )
  EW_PROPERTY( ValueMarginLeft, XInt32 )
  EW_PROPERTY( ValueColorNegative, XColor )
  EW_PROPERTY( ValueColorPositive, XColor )
  EW_PROPERTY( ValueAlignment,  XSet )
  EW_PROPERTY( UnitMarginBottom, XInt32 )
  EW_PROPERTY( UnitMarginTop,   XInt32 )
  EW_PROPERTY( UnitMarginRight, XInt32 )
  EW_PROPERTY( UnitMarginLeft,  XInt32 )
  EW_PROPERTY( UnitColorNegative, XColor )
  EW_PROPERTY( UnitColorPositive, XColor )
  EW_PROPERTY( UnitAlignment,   XSet )
  EW_PROPERTY( WidgetMaxSize,   XPoint )
  EW_PROPERTY( WidgetMinSize,   XPoint )
  EW_PROPERTY( Layout,          XEnum )
EW_END_OF_FIELDS( WidgetSetValueDisplayConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::ValueDisplayConfig' */
EW_DEFINE_METHODS( WidgetSetValueDisplayConfig, WidgetSetWidgetConfig )
EW_END_OF_METHODS( WidgetSetValueDisplayConfig )

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetValueMarginRight()' */
void WidgetSetValueDisplayConfig_OnSetValueMarginRight( WidgetSetValueDisplayConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetValueColorNegative()' */
void WidgetSetValueDisplayConfig_OnSetValueColorNegative( WidgetSetValueDisplayConfig _this, 
  XColor value );

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetValueColorPositive()' */
void WidgetSetValueDisplayConfig_OnSetValueColorPositive( WidgetSetValueDisplayConfig _this, 
  XColor value );

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetValueAlignment()' */
void WidgetSetValueDisplayConfig_OnSetValueAlignment( WidgetSetValueDisplayConfig _this, 
  XSet value );

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetValueFont()' */
void WidgetSetValueDisplayConfig_OnSetValueFont( WidgetSetValueDisplayConfig _this, 
  ResourcesFont value );

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetUnitColorNegative()' */
void WidgetSetValueDisplayConfig_OnSetUnitColorNegative( WidgetSetValueDisplayConfig _this, 
  XColor value );

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetUnitColorPositive()' */
void WidgetSetValueDisplayConfig_OnSetUnitColorPositive( WidgetSetValueDisplayConfig _this, 
  XColor value );

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetUnitFont()' */
void WidgetSetValueDisplayConfig_OnSetUnitFont( WidgetSetValueDisplayConfig _this, 
  ResourcesFont value );

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetFormatDecimalSign()' */
void WidgetSetValueDisplayConfig_OnSetFormatDecimalSign( WidgetSetValueDisplayConfig _this, 
  XString value );

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetLayout()' */
void WidgetSetValueDisplayConfig_OnSetLayout( WidgetSetValueDisplayConfig _this, 
  XEnum value );

#ifdef __cplusplus
  }
#endif

#endif /* _WidgetSetValueDisplayConfig_H */

/* Embedded Wizard */
