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

#ifndef _EffectsTimingList_H
#define _EffectsTimingList_H

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
