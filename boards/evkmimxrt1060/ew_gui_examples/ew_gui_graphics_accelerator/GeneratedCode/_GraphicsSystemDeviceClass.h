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

#ifndef _GraphicsSystemDeviceClass_H
#define _GraphicsSystemDeviceClass_H

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
