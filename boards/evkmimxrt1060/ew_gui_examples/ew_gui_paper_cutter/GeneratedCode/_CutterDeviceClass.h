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

#ifndef _CutterDeviceClass_H
#define _CutterDeviceClass_H

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
