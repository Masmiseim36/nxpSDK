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
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#ifndef Waveform_H
#define Waveform_H

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

#include "_WaveformChannelPanel.h"
#include "_WaveformWaveform.h"

/* Bitmap resource containing the border (frame) of all panels used within the demo. */
EW_DECLARE_BITMAP_RES( WaveformPanelBorder )

/* Bitmap resource containing the icons for the waveform selction buttons. */
EW_DECLARE_BITMAP_RES( WaveformWaveformIcons )

/* Enumeration definition of supported waveforms. */
typedef enum
{
  WaveformWaveformTypeGround            = 0,
  WaveformWaveformTypeSinus             = 1,
  WaveformWaveformTypeSquare            = 2,
  WaveformWaveformTypeRamp              = 3,
  WaveformWaveformTypePulse             = 4,
  WaveformWaveformTypeNoise             = 5
} WaveformWaveformType;

/* User defined constant: 'Waveform::ThemeColor' */
extern const XColor WaveformThemeColor;

#ifdef __cplusplus
  }
#endif

#endif /* Waveform_H */

/* Embedded Wizard */
