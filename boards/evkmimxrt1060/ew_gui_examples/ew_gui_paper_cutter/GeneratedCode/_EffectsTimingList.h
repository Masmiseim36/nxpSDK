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

#ifndef _EffectsTimingList_H
#define _EffectsTimingList_H

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

/* Forward declaration of the class Effects::TimingList */
#ifndef _EffectsTimingList_
  EW_DECLARE_CLASS( EffectsTimingList )
#define _EffectsTimingList_
#endif


/* Deklaration of class : 'Effects::TimingList' */
EW_DEFINE_FIELDS( EffectsTimingList, XObject )
  EW_ARRAY   ( spans,           XFloat, [12])
  EW_ARRAY   ( amps,            XFloat, [12])
EW_END_OF_FIELDS( EffectsTimingList )

/* Virtual Method Table (VMT) for the class : 'Effects::TimingList' */
EW_DEFINE_METHODS( EffectsTimingList, XObject )
EW_END_OF_METHODS( EffectsTimingList )

#ifdef __cplusplus
  }
#endif

#endif /* _EffectsTimingList_H */

/* Embedded Wizard */
