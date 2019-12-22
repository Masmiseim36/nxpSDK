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

#ifndef _BrickGameDataClass_H
#define _BrickGameDataClass_H

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
