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

#ifndef _BrickGameBallPosition_H
#define _BrickGameBallPosition_H

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

/* Forward declaration of the class BrickGame::BallPosition */
#ifndef _BrickGameBallPosition_
  EW_DECLARE_CLASS( BrickGameBallPosition )
#define _BrickGameBallPosition_
#endif


/* This is a class. */
EW_DEFINE_FIELDS( BrickGameBallPosition, XObject )
  EW_PROPERTY( Bounds,          XRect )
  EW_PROPERTY( SpeedX,          XFloat )
  EW_PROPERTY( SpeedY,          XFloat )
EW_END_OF_FIELDS( BrickGameBallPosition )

/* Virtual Method Table (VMT) for the class : 'BrickGame::BallPosition' */
EW_DEFINE_METHODS( BrickGameBallPosition, XObject )
EW_END_OF_METHODS( BrickGameBallPosition )

#ifdef __cplusplus
  }
#endif

#endif /* _BrickGameBallPosition_H */

/* Embedded Wizard */
