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

#ifndef _GraphicsSystemDeviceClass_H
#define _GraphicsSystemDeviceClass_H

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

#include "_TemplatesDeviceClass.h"

/* Forward declaration of the class GraphicsSystem::DeviceClass */
#ifndef _GraphicsSystemDeviceClass_
  EW_DECLARE_CLASS( GraphicsSystemDeviceClass )
#define _GraphicsSystemDeviceClass_
#endif


/* Deklaration of class : 'GraphicsSystem::DeviceClass' */
EW_DEFINE_FIELDS( GraphicsSystemDeviceClass, TemplatesDeviceClass )
  EW_PROPERTY( AcceleratorActive, XBool )
EW_END_OF_FIELDS( GraphicsSystemDeviceClass )

/* Virtual Method Table (VMT) for the class : 'GraphicsSystem::DeviceClass' */
EW_DEFINE_METHODS( GraphicsSystemDeviceClass, TemplatesDeviceClass )
EW_END_OF_METHODS( GraphicsSystemDeviceClass )

/* 'C' function for method : 'GraphicsSystem::DeviceClass.OnSetAcceleratorActive()' */
void GraphicsSystemDeviceClass_OnSetAcceleratorActive( GraphicsSystemDeviceClass _this, 
  XBool value );

/* The method 'GetCpuLoad' returns the current CPU usage in percent since last call 
   of this method. */
XInt32 GraphicsSystemDeviceClass_GetCpuLoad( GraphicsSystemDeviceClass _this );

/* 'C' function for method : 'GraphicsSystem::DeviceClass.Init()' */
void GraphicsSystemDeviceClass_Init( GraphicsSystemDeviceClass _this, XHandle aArg );

/* 'C' function for method : 'GraphicsSystem::DeviceClass.Done()' */
void GraphicsSystemDeviceClass_Done( GraphicsSystemDeviceClass _this );

/* Default onget method for the property 'AcceleratorActive' */
XBool GraphicsSystemDeviceClass_OnGetAcceleratorActive( GraphicsSystemDeviceClass _this );

#ifdef __cplusplus
  }
#endif

#endif /* _GraphicsSystemDeviceClass_H */

/* Embedded Wizard */
