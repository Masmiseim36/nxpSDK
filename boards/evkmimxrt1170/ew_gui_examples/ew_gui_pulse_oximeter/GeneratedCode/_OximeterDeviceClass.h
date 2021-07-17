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

#ifndef _OximeterDeviceClass_H
#define _OximeterDeviceClass_H

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

#include "_CoreTimer.h"
#include "_OximeterDataStorage.h"
#include "_TemplatesDeviceClass.h"

/* Forward declaration of the class Oximeter::DeviceClass */
#ifndef _OximeterDeviceClass_
  EW_DECLARE_CLASS( OximeterDeviceClass )
#define _OximeterDeviceClass_
#endif


/* Deklaration of class : 'Oximeter::DeviceClass' */
EW_DEFINE_FIELDS( OximeterDeviceClass, TemplatesDeviceClass )
  EW_OBJECT  ( Timer,           CoreTimer )
  EW_OBJECT  ( PulseStorage,    OximeterDataStorage )
  EW_OBJECT  ( OxygenStorage,   OximeterDataStorage )
  EW_PROPERTY( CurrentPulse,    XInt32 )
  EW_PROPERTY( CurrentOxygen,   XInt32 )
  EW_PROPERTY( MaxPulse,        XInt32 )
  EW_PROPERTY( MinPulse,        XInt32 )
  EW_PROPERTY( MaxOxygen,       XInt32 )
  EW_PROPERTY( MinOxygen,       XInt32 )
  EW_VARIABLE( simulationPulse, XInt32 )
  EW_VARIABLE( simulationOxygen, XInt32 )
  EW_PROPERTY( AlarmTime,       XInt32 )
  EW_PROPERTY( OxygenFlow,      XInt32 )
EW_END_OF_FIELDS( OximeterDeviceClass )

/* Virtual Method Table (VMT) for the class : 'Oximeter::DeviceClass' */
EW_DEFINE_METHODS( OximeterDeviceClass, TemplatesDeviceClass )
EW_END_OF_METHODS( OximeterDeviceClass )

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void OximeterDeviceClass_UpdatePulse( OximeterDeviceClass _this, XInt32 aNewValue );

/* Wrapper function for the non virtual method : 'Oximeter::DeviceClass.UpdatePulse()' */
void OximeterDeviceClass__UpdatePulse( void* _this, XInt32 aNewValue );

/* The following define announces the presence of the method Oximeter::DeviceClass.UpdatePulse(). */
#define _OximeterDeviceClass__UpdatePulse_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void OximeterDeviceClass_UpdateOxygen( OximeterDeviceClass _this, XInt32 aNewValue );

/* Wrapper function for the non virtual method : 'Oximeter::DeviceClass.UpdateOxygen()' */
void OximeterDeviceClass__UpdateOxygen( void* _this, XInt32 aNewValue );

/* The following define announces the presence of the method Oximeter::DeviceClass.UpdateOxygen(). */
#define _OximeterDeviceClass__UpdateOxygen_

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetMaxPulse()' */
void OximeterDeviceClass_OnSetMaxPulse( OximeterDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetMinPulse()' */
void OximeterDeviceClass_OnSetMinPulse( OximeterDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetMaxOxygen()' */
void OximeterDeviceClass_OnSetMaxOxygen( OximeterDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetMinOxygen()' */
void OximeterDeviceClass_OnSetMinOxygen( OximeterDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Oximeter::DeviceClass.Simulation()' */
void OximeterDeviceClass_Simulation( OximeterDeviceClass _this, XObject sender );

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void OximeterDeviceClass_Init( OximeterDeviceClass _this, XHandle aArg );

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetAlarmTime()' */
void OximeterDeviceClass_OnSetAlarmTime( OximeterDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Oximeter::DeviceClass.OnSetOxygenFlow()' */
void OximeterDeviceClass_OnSetOxygenFlow( OximeterDeviceClass _this, XInt32 value );

/* Default onget method for the property 'CurrentPulse' */
XInt32 OximeterDeviceClass_OnGetCurrentPulse( OximeterDeviceClass _this );

/* Default onset method for the property 'CurrentPulse' */
void OximeterDeviceClass_OnSetCurrentPulse( OximeterDeviceClass _this, XInt32 value );

/* Default onget method for the property 'CurrentOxygen' */
XInt32 OximeterDeviceClass_OnGetCurrentOxygen( OximeterDeviceClass _this );

/* Default onset method for the property 'CurrentOxygen' */
void OximeterDeviceClass_OnSetCurrentOxygen( OximeterDeviceClass _this, XInt32 value );

/* Default onget method for the property 'MaxPulse' */
XInt32 OximeterDeviceClass_OnGetMaxPulse( OximeterDeviceClass _this );

/* Default onget method for the property 'MinPulse' */
XInt32 OximeterDeviceClass_OnGetMinPulse( OximeterDeviceClass _this );

/* Default onget method for the property 'MaxOxygen' */
XInt32 OximeterDeviceClass_OnGetMaxOxygen( OximeterDeviceClass _this );

/* Default onget method for the property 'MinOxygen' */
XInt32 OximeterDeviceClass_OnGetMinOxygen( OximeterDeviceClass _this );

/* Default onget method for the property 'AlarmTime' */
XInt32 OximeterDeviceClass_OnGetAlarmTime( OximeterDeviceClass _this );

/* Default onget method for the property 'OxygenFlow' */
XInt32 OximeterDeviceClass_OnGetOxygenFlow( OximeterDeviceClass _this );

#ifdef __cplusplus
  }
#endif

#endif /* _OximeterDeviceClass_H */

/* Embedded Wizard */
