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

#ifndef _DashboardDeviceClass_H
#define _DashboardDeviceClass_H

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

/* Forward declaration of the class Dashboard::DeviceClass */
#ifndef _DashboardDeviceClass_
  EW_DECLARE_CLASS( DashboardDeviceClass )
#define _DashboardDeviceClass_
#endif


/* Deklaration of class : 'Dashboard::DeviceClass' */
EW_DEFINE_FIELDS( DashboardDeviceClass, XObject )
  EW_PROPERTY( Speed,           XInt32 )
  EW_PROPERTY( Energy,          XInt32 )
  EW_PROPERTY( ODO,             XInt32 )
  EW_PROPERTY( Battery,         XInt32 )
  EW_PROPERTY( AutoDemoMode,    XBool )
  EW_PROPERTY( DemoMode,        XBool )
  EW_PROPERTY( Led5,            XBool )
  EW_PROPERTY( Led4,            XBool )
  EW_PROPERTY( Led3,            XBool )
  EW_PROPERTY( Led2,            XBool )
  EW_PROPERTY( Led1,            XBool )
  EW_PROPERTY( Led0,            XBool )
EW_END_OF_FIELDS( DashboardDeviceClass )

/* Virtual Method Table (VMT) for the class : 'Dashboard::DeviceClass' */
EW_DEFINE_METHODS( DashboardDeviceClass, XObject )
EW_END_OF_METHODS( DashboardDeviceClass )

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetSpeed()' */
void DashboardDeviceClass_OnSetSpeed( DashboardDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetEnergy()' */
void DashboardDeviceClass_OnSetEnergy( DashboardDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetODO()' */
void DashboardDeviceClass_OnSetODO( DashboardDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetBattery()' */
void DashboardDeviceClass_OnSetBattery( DashboardDeviceClass _this, XInt32 value );

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed0()' */
void DashboardDeviceClass_OnSetLed0( DashboardDeviceClass _this, XBool value );

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed1()' */
void DashboardDeviceClass_OnSetLed1( DashboardDeviceClass _this, XBool value );

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed2()' */
void DashboardDeviceClass_OnSetLed2( DashboardDeviceClass _this, XBool value );

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed3()' */
void DashboardDeviceClass_OnSetLed3( DashboardDeviceClass _this, XBool value );

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed4()' */
void DashboardDeviceClass_OnSetLed4( DashboardDeviceClass _this, XBool value );

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed5()' */
void DashboardDeviceClass_OnSetLed5( DashboardDeviceClass _this, XBool value );

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetDemoMode()' */
void DashboardDeviceClass_OnSetDemoMode( DashboardDeviceClass _this, XBool value );

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetAutoDemoMode()' */
void DashboardDeviceClass_OnSetAutoDemoMode( DashboardDeviceClass _this, XBool value );

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateSpeed( DashboardDeviceClass _this, XInt32 aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateSpeed()' */
void DashboardDeviceClass__UpdateSpeed( void* _this, XInt32 aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateSpeed(). */
#define _DashboardDeviceClass__UpdateSpeed_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateODO( DashboardDeviceClass _this, XInt32 aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateODO()' */
void DashboardDeviceClass__UpdateODO( void* _this, XInt32 aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateODO(). */
#define _DashboardDeviceClass__UpdateODO_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateEnergy( DashboardDeviceClass _this, XInt32 aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateEnergy()' */
void DashboardDeviceClass__UpdateEnergy( void* _this, XInt32 aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateEnergy(). */
#define _DashboardDeviceClass__UpdateEnergy_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateBattery( DashboardDeviceClass _this, XInt32 aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateBattery()' */
void DashboardDeviceClass__UpdateBattery( void* _this, XInt32 aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateBattery(). */
#define _DashboardDeviceClass__UpdateBattery_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed0( DashboardDeviceClass _this, XBool aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed0()' */
void DashboardDeviceClass__UpdateLed0( void* _this, XBool aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateLed0(). */
#define _DashboardDeviceClass__UpdateLed0_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed1( DashboardDeviceClass _this, XBool aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed1()' */
void DashboardDeviceClass__UpdateLed1( void* _this, XBool aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateLed1(). */
#define _DashboardDeviceClass__UpdateLed1_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed2( DashboardDeviceClass _this, XBool aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed2()' */
void DashboardDeviceClass__UpdateLed2( void* _this, XBool aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateLed2(). */
#define _DashboardDeviceClass__UpdateLed2_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed3( DashboardDeviceClass _this, XBool aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed3()' */
void DashboardDeviceClass__UpdateLed3( void* _this, XBool aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateLed3(). */
#define _DashboardDeviceClass__UpdateLed3_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed4( DashboardDeviceClass _this, XBool aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed4()' */
void DashboardDeviceClass__UpdateLed4( void* _this, XBool aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateLed4(). */
#define _DashboardDeviceClass__UpdateLed4_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed5( DashboardDeviceClass _this, XBool aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed5()' */
void DashboardDeviceClass__UpdateLed5( void* _this, XBool aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateLed5(). */
#define _DashboardDeviceClass__UpdateLed5_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateDemoMode( DashboardDeviceClass _this, XBool aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateDemoMode()' */
void DashboardDeviceClass__UpdateDemoMode( void* _this, XBool aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateDemoMode(). */
#define _DashboardDeviceClass__UpdateDemoMode_

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateAutoDemoMode( DashboardDeviceClass _this, XBool 
  aNewValue );

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateAutoDemoMode()' */
void DashboardDeviceClass__UpdateAutoDemoMode( void* _this, XBool aNewValue );

/* The following define announces the presence of the method Dashboard::DeviceClass.UpdateAutoDemoMode(). */
#define _DashboardDeviceClass__UpdateAutoDemoMode_

/* Default onget method for the property 'Speed' */
XInt32 DashboardDeviceClass_OnGetSpeed( DashboardDeviceClass _this );

/* Default onget method for the property 'Energy' */
XInt32 DashboardDeviceClass_OnGetEnergy( DashboardDeviceClass _this );

/* Default onget method for the property 'ODO' */
XInt32 DashboardDeviceClass_OnGetODO( DashboardDeviceClass _this );

/* Default onget method for the property 'Battery' */
XInt32 DashboardDeviceClass_OnGetBattery( DashboardDeviceClass _this );

/* Default onget method for the property 'Led0' */
XBool DashboardDeviceClass_OnGetLed0( DashboardDeviceClass _this );

/* Default onget method for the property 'Led1' */
XBool DashboardDeviceClass_OnGetLed1( DashboardDeviceClass _this );

/* Default onget method for the property 'Led2' */
XBool DashboardDeviceClass_OnGetLed2( DashboardDeviceClass _this );

/* Default onget method for the property 'Led3' */
XBool DashboardDeviceClass_OnGetLed3( DashboardDeviceClass _this );

/* Default onget method for the property 'Led4' */
XBool DashboardDeviceClass_OnGetLed4( DashboardDeviceClass _this );

/* Default onget method for the property 'Led5' */
XBool DashboardDeviceClass_OnGetLed5( DashboardDeviceClass _this );

/* Default onget method for the property 'DemoMode' */
XBool DashboardDeviceClass_OnGetDemoMode( DashboardDeviceClass _this );

/* Default onget method for the property 'AutoDemoMode' */
XBool DashboardDeviceClass_OnGetAutoDemoMode( DashboardDeviceClass _this );

#ifdef __cplusplus
  }
#endif

#endif /* _DashboardDeviceClass_H */

/* Embedded Wizard */
