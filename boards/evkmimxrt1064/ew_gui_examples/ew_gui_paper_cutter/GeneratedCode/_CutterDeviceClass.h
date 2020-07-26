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

#ifndef _CutterDeviceClass_H
#define _CutterDeviceClass_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x0009001E
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x0009001E
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

#include "_CoreTimer.h"

/* Forward declaration of the class Cutter::DeviceClass */
#ifndef _CutterDeviceClass_
  EW_DECLARE_CLASS( CutterDeviceClass )
#define _CutterDeviceClass_
#endif


/* Controller class, containing all data set values and all current values of the 
   paper cutter. */
EW_DEFINE_FIELDS( CutterDeviceClass, XObject )
  EW_OBJECT  ( Timer,           CoreTimer )
  EW_PROPERTY( PaperBatch,      XInt32 )
  EW_PROPERTY( PaperSpeed,      XInt32 )
  EW_PROPERTY( PaperWidth,      XInt32 )
  EW_PROPERTY( PaperHeight,     XInt32 )
  EW_PROPERTY( Remaining,       XInt32 )
  EW_PROPERTY( PaperType,       XInt32 )
  EW_PROPERTY( Running,         XBool )
EW_END_OF_FIELDS( CutterDeviceClass )

/* Virtual Method Table (VMT) for the class : 'Cutter::DeviceClass' */
EW_DEFINE_METHODS( CutterDeviceClass, XObject )
EW_END_OF_METHODS( CutterDeviceClass )

/* 'C' function for method : 'Cutter::DeviceClass.OnSetPaperBatch()' */
void CutterDeviceClass_OnSetPaperBatch( CutterDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Cutter::DeviceClass.OnSetPaperSpeed()' */
void CutterDeviceClass_OnSetPaperSpeed( CutterDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Cutter::DeviceClass.OnSetPaperWidth()' */
void CutterDeviceClass_OnSetPaperWidth( CutterDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Cutter::DeviceClass.OnSetPaperHeight()' */
void CutterDeviceClass_OnSetPaperHeight( CutterDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Cutter::DeviceClass.OnSetRunning()' */
void CutterDeviceClass_OnSetRunning( CutterDeviceClass _this, XBool value );

/* 'C' function for method : 'Cutter::DeviceClass.OnSetRemaining()' */
void CutterDeviceClass_OnSetRemaining( CutterDeviceClass _this, XInt32 value );

/* This is a slot method. */
void CutterDeviceClass_Animate( CutterDeviceClass _this, XObject sender );

/* 'C' function for method : 'Cutter::DeviceClass.OnSetPaperType()' */
void CutterDeviceClass_OnSetPaperType( CutterDeviceClass _this, XInt32 value );

/* Default onget method for the property 'PaperBatch' */
XInt32 CutterDeviceClass_OnGetPaperBatch( CutterDeviceClass _this );

/* Default onget method for the property 'PaperSpeed' */
XInt32 CutterDeviceClass_OnGetPaperSpeed( CutterDeviceClass _this );

/* Default onget method for the property 'PaperWidth' */
XInt32 CutterDeviceClass_OnGetPaperWidth( CutterDeviceClass _this );

/* Default onget method for the property 'PaperHeight' */
XInt32 CutterDeviceClass_OnGetPaperHeight( CutterDeviceClass _this );

/* Default onget method for the property 'Running' */
XBool CutterDeviceClass_OnGetRunning( CutterDeviceClass _this );

/* Default onget method for the property 'Remaining' */
XInt32 CutterDeviceClass_OnGetRemaining( CutterDeviceClass _this );

/* Default onget method for the property 'PaperType' */
XInt32 CutterDeviceClass_OnGetPaperType( CutterDeviceClass _this );

#ifdef __cplusplus
  }
#endif

#endif /* _CutterDeviceClass_H */

/* Embedded Wizard */
