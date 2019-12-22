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

#ifndef Washer_H
#define Washer_H

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

#include "_WasherDeviceClass.h"
#include "_WasherHourItemL.h"
#include "_WasherHourItemS.h"
#include "_WasherMinItemL.h"
#include "_WasherMinItemS.h"
#include "_WasherOptItemL.h"
#include "_WasherOptItemS.h"
#include "_WasherOptList.h"
#include "_WasherProgItemL.h"
#include "_WasherProgItemS.h"
#include "_WasherProgList.h"
#include "_WasherSpinItemL.h"
#include "_WasherSpinItemS.h"
#include "_WasherSpinList.h"
#include "_WasherStartButton.h"
#include "_WasherTempItemL.h"
#include "_WasherTempItemS.h"
#include "_WasherTempList.h"
#include "_WasherTimeList.h"
#include "_WasherWasher.h"

/* This is a font resource. */
EW_DECLARE_FONT_RES( WasherListFontS )

/* This is a font resource. */
EW_DECLARE_FONT_RES( WasherDescrFont )

/* This is a font resource. */
EW_DECLARE_FONT_RES( WasherListFontM )

/* This is a font resource. */
EW_DECLARE_FONT_RES( WasherListFontL )

/* This is a font resource. */
EW_DECLARE_FONT_RES( WasherListFontXL )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( WasherBgHot )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( WasherBgDelicates )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( WasherBgSportswear )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( WasherBgWool )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( WasherBgEco )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( WasherShadow )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( WasherBgSpin )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( WasherBgDrain )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( WasherBgEasyCare )

/* Color constant according UI design. */
extern const XColor WasherButtonBgColor;

/* Color constant according UI design. */
extern const XColor WasherCaptionColor;

/* Color constant according UI design. */
extern const XColor WasherButtonTextColor;

/* User defined auto object: 'Washer::Device' */
EW_DECLARE_AUTOOBJECT( WasherDevice, WasherDeviceClass )

/* Color constant according UI design. */
extern const XColor WasherListBgColor;

/* Color constant according UI design. */
extern const XColor WasherListTextColor;

/* Color constant according UI design. */
extern const XColor WasherTempColor0;

/* Color constant according UI design. */
extern const XColor WasherTempColor2;

/* Color constant according UI design. */
extern const XColor WasherTempColor3;

/* Color constant according UI design. */
extern const XColor WasherTempColor4;

/* Color constant according UI design. */
extern const XColor WasherTempColor5;

/* Color constant according UI design. */
extern const XColor WasherTempColor6;

/* Color constant according UI design. */
extern const XColor WasherTempColor1;

/* Color constant according UI design. */
extern const XColor WasherStartColor;

/* Color constant according UI design. */
extern const XColor WasherStopColor;

#ifdef __cplusplus
  }
#endif

#endif /* Washer_H */

/* Embedded Wizard */
