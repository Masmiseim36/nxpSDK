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

#ifndef BrickGame_H
#define BrickGame_H

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

#include "_BrickGameBallPosition.h"
#include "_BrickGameBrickGame.h"
#include "_BrickGameBrickRow.h"
#include "_BrickGameDataClass.h"
#include "_BrickGameLcdDisplay.h"

/* Font resource : 'BrickGame::DescrFont' */
EW_DECLARE_FONT_RES( BrickGameDescrFont )

/* Bitmap resource : 'BrickGame::Bricks' */
EW_DECLARE_BITMAP_RES( BrickGameBricks )

/* Bitmap resource : 'BrickGame::Paddle' */
EW_DECLARE_BITMAP_RES( BrickGamePaddle )

/* Bitmap resource : 'BrickGame::Ball' */
EW_DECLARE_BITMAP_RES( BrickGameBall )

/* Bitmap resource : 'BrickGame::Landscape' */
EW_DECLARE_BITMAP_RES( BrickGameLandscape )

/* Bitmap resource : 'BrickGame::Stars' */
EW_DECLARE_BITMAP_RES( BrickGameStars )

/* Bitmap resource for displaying the numbers within a simulated LCD. */
EW_DECLARE_BITMAP_RES( BrickGameLcdNumbers )

/* User defined auto object: 'BrickGame::Data' */
EW_DECLARE_AUTOOBJECT( BrickGameData, BrickGameDataClass )

/* User defined enumeration: 'BrickGame::RowType' */
typedef enum
{
  BrickGameRowTypeHidden                = 0,
  BrickGameRowTypeGray                  = 1,
  BrickGameRowTypeBlue                  = 2,
  BrickGameRowTypeGreen                 = 3,
  BrickGameRowTypeYellow                = 4,
  BrickGameRowTypeWhite                 = 5,
  BrickGameRowTypeConcrete              = 6,
  BrickGameRowTypeEmWi                  = 7,
  BrickGameRowTypeTwoBlocks             = 8,
  BrickGameRowTypeTwoDoors              = 9,
  BrickGameRowTypeBlueGreen             = 10,
  BrickGameRowTypeWhiteEmWi             = 11,
  BrickGameRowTypeGreenBlue             = 12,
  BrickGameRowTypeOneDoor               = 13
} BrickGameRowType;

#ifdef __cplusplus
  }
#endif

#endif /* BrickGame_H */

/* Embedded Wizard */
