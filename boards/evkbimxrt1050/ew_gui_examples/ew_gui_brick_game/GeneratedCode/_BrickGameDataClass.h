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

#ifndef _BrickGameDataClass_H
#define _BrickGameDataClass_H

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

#include "_TemplatesDeviceClass.h"

/* Forward declaration of the class BrickGame::DataClass */
#ifndef _BrickGameDataClass_
  EW_DECLARE_CLASS( BrickGameDataClass )
#define _BrickGameDataClass_
#endif


/* Deklaration of class : 'BrickGame::DataClass' */
EW_DEFINE_FIELDS( BrickGameDataClass, TemplatesDeviceClass )
  EW_PROPERTY( Score,           XInt32 )
  EW_PROPERTY( Lost,            XInt32 )
  EW_PROPERTY( Level,           XInt32 )
EW_END_OF_FIELDS( BrickGameDataClass )

/* Virtual Method Table (VMT) for the class : 'BrickGame::DataClass' */
EW_DEFINE_METHODS( BrickGameDataClass, TemplatesDeviceClass )
EW_END_OF_METHODS( BrickGameDataClass )

/* 'C' function for method : 'BrickGame::DataClass.OnSetScore()' */
void BrickGameDataClass_OnSetScore( BrickGameDataClass _this, XInt32 value );

/* 'C' function for method : 'BrickGame::DataClass.OnSetLost()' */
void BrickGameDataClass_OnSetLost( BrickGameDataClass _this, XInt32 value );

/* 'C' function for method : 'BrickGame::DataClass.OnSetLevel()' */
void BrickGameDataClass_OnSetLevel( BrickGameDataClass _this, XInt32 value );

/* Default onget method for the property 'Score' */
XInt32 BrickGameDataClass_OnGetScore( BrickGameDataClass _this );

/* Default onget method for the property 'Lost' */
XInt32 BrickGameDataClass_OnGetLost( BrickGameDataClass _this );

/* Default onget method for the property 'Level' */
XInt32 BrickGameDataClass_OnGetLevel( BrickGameDataClass _this );

#ifdef __cplusplus
  }
#endif

#endif /* _BrickGameDataClass_H */

/* Embedded Wizard */
