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

#ifndef _DashboardColorsClass_H
#define _DashboardColorsClass_H

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

/* Forward declaration of the class Dashboard::ColorsClass */
#ifndef _DashboardColorsClass_
  EW_DECLARE_CLASS( DashboardColorsClass )
#define _DashboardColorsClass_
#endif


/* Color class which stores the used colors into properties. */
EW_DEFINE_FIELDS( DashboardColorsClass, XObject )
  EW_PROPERTY( Background,      XColor )
  EW_PROPERTY( Text,            XColor )
  EW_PROPERTY( TextLedsOff,     XColor )
  EW_PROPERTY( GaugeGlowV1,     XColor )
  EW_PROPERTY( BatteryGaugeSelectionV1, XColor )
  EW_PROPERTY( LedsOnOrange,    XColor )
  EW_PROPERTY( GaugeGlowV2,     XColor )
  EW_PROPERTY( BatteryGaugeSelectionV2, XColor )
  EW_PROPERTY( WingV2,          XColor )
  EW_PROPERTY( BatteryFrame,    XColor )
  EW_PROPERTY( LedsOnGreen,     XColor )
  EW_PROPERTY( GaugeLabelLinesV1, XColor )
  EW_PROPERTY( GaugeCircle0,    XColor )
  EW_PROPERTY( GaugeCircle1,    XColor )
  EW_PROPERTY( GaugeCircle2,    XColor )
  EW_PROPERTY( GaugeCircle3,    XColor )
  EW_PROPERTY( GaugeCircle4,    XColor )
  EW_PROPERTY( GaugeCircle5,    XColor )
EW_END_OF_FIELDS( DashboardColorsClass )

/* Virtual Method Table (VMT) for the class : 'Dashboard::ColorsClass' */
EW_DEFINE_METHODS( DashboardColorsClass, XObject )
EW_END_OF_METHODS( DashboardColorsClass )

#ifdef __cplusplus
  }
#endif

#endif /* _DashboardColorsClass_H */

/* Embedded Wizard */
