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

#ifndef _WasherDeviceClass_H
#define _WasherDeviceClass_H

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

/* Forward declaration of the class Washer::DeviceClass */
#ifndef _WasherDeviceClass_
  EW_DECLARE_CLASS( WasherDeviceClass )
#define _WasherDeviceClass_
#endif


/* Controller class, containing all data set values and all current values of the 
   washing machine. */
EW_DEFINE_FIELDS( WasherDeviceClass, XObject )
  EW_PROPERTY( ProgramNumber,   XInt32 )
  EW_PROPERTY( TempNumber,      XInt32 )
  EW_PROPERTY( OptionNumber,    XInt32 )
  EW_PROPERTY( Hour,            XInt32 )
  EW_PROPERTY( Minute,          XInt32 )
  EW_PROPERTY( SpinNumber,      XInt32 )
  EW_ARRAY   ( WashingProgram,  XString, [8])
  EW_ARRAY   ( WashingTemp,     XString, [6])
  EW_ARRAY   ( SpinTurn,        XString, [7])
  EW_ARRAY   ( Options,         XString, [6])
EW_END_OF_FIELDS( WasherDeviceClass )

/* Virtual Method Table (VMT) for the class : 'Washer::DeviceClass' */
EW_DEFINE_METHODS( WasherDeviceClass, XObject )
EW_END_OF_METHODS( WasherDeviceClass )

/* 'C' function for method : 'Washer::DeviceClass.OnSetProgramNumber()' */
void WasherDeviceClass_OnSetProgramNumber( WasherDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Washer::DeviceClass.OnSetTempNumber()' */
void WasherDeviceClass_OnSetTempNumber( WasherDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Washer::DeviceClass.OnSetOptionNumber()' */
void WasherDeviceClass_OnSetOptionNumber( WasherDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Washer::DeviceClass.OnSetHour()' */
void WasherDeviceClass_OnSetHour( WasherDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Washer::DeviceClass.OnSetMinute()' */
void WasherDeviceClass_OnSetMinute( WasherDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Washer::DeviceClass.OnSetSpinNumber()' */
void WasherDeviceClass_OnSetSpinNumber( WasherDeviceClass _this, XInt32 value );

/* Default onget method for the property 'ProgramNumber' */
XInt32 WasherDeviceClass_OnGetProgramNumber( WasherDeviceClass _this );

/* Default onget method for the property 'TempNumber' */
XInt32 WasherDeviceClass_OnGetTempNumber( WasherDeviceClass _this );

/* Default onget method for the property 'OptionNumber' */
XInt32 WasherDeviceClass_OnGetOptionNumber( WasherDeviceClass _this );

/* Default onget method for the property 'Hour' */
XInt32 WasherDeviceClass_OnGetHour( WasherDeviceClass _this );

/* Default onget method for the property 'Minute' */
XInt32 WasherDeviceClass_OnGetMinute( WasherDeviceClass _this );

/* Default onget method for the property 'SpinNumber' */
XInt32 WasherDeviceClass_OnGetSpinNumber( WasherDeviceClass _this );

#ifdef __cplusplus
  }
#endif

#endif /* _WasherDeviceClass_H */

/* Embedded Wizard */
