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

#include "ewlocale.h"
#include "_BrickGameBallPosition.h"
#include "_BrickGameBrickGame.h"
#include "_BrickGameBrickRow.h"
#include "_BrickGameDataClass.h"
#include "_BrickGameLcdDisplay.h"
#include "_CoreKeyPressHandler.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "_ViewsImage.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"
#include "BrickGame.h"
#include "Core.h"
#include "Views.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x00000038, /* ratio 92.86 % */
  0xB8001300, 0x800A6452, 0x00DE0031, 0x0CA00390, 0x22001D00, 0xA4C8240A, 0x163B0021,
  0x088C3CD9, 0x22C0C100, 0x01CE1515, 0x47236740, 0x0001018E, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XRect _Const0000 = {{ 0, 0 }, { 480, 24 }};
static const XRect _Const0001 = {{ 0, 0 }, { 60, 24 }};
static const XRect _Const0002 = {{ 60, 0 }, { 120, 24 }};
static const XRect _Const0003 = {{ 120, 0 }, { 180, 24 }};
static const XRect _Const0004 = {{ 180, 0 }, { 240, 24 }};
static const XRect _Const0005 = {{ 240, 0 }, { 300, 24 }};
static const XRect _Const0006 = {{ 300, 0 }, { 360, 24 }};
static const XRect _Const0007 = {{ 360, 0 }, { 420, 24 }};
static const XRect _Const0008 = {{ 420, 0 }, { 480, 24 }};
static const XRect _Const0009 = {{ 0, 0 }, { 480, 272 }};
static const XRect _Const000A = {{ 0, 228 }, { 480, 272 }};
static const XColor _Const000B = { 0x00, 0x00, 0x00, 0xFF };
static const XRect _Const000C = {{ 0, 0 }, { 480, 228 }};
static const XRect _Const000D = {{ 0, 192 }, { 480, 228 }};
static const XRect _Const000E = {{ 0, 40 }, { 480, 64 }};
static const XRect _Const000F = {{ 0, 64 }, { 480, 88 }};
static const XRect _Const0010 = {{ 0, 88 }, { 480, 112 }};
static const XRect _Const0011 = {{ 0, 112 }, { 480, 136 }};
static const XRect _Const0012 = {{ 0, 136 }, { 480, 160 }};
static const XPoint _Const0013 = { 0, 200 };
static const XPoint _Const0014 = { 480, 200 };
static const XPoint _Const0015 = { 480, 272 };
static const XPoint _Const0016 = { 0, 272 };
static const XRect _Const0017 = {{ 120, 220 }, { 144, 244 }};
static const XRect _Const0018 = {{ 200, 230 }, { 290, 254 }};
static const XRect _Const0019 = {{ 200, 5 }, { 340, 30 }};
static const XColor _Const001A = { 0xCC, 0xCC, 0xCC, 0xFF };
static const XRect _Const001B = {{ 145, 4 }, { 195, 24 }};
static const XStringRes _Const001C = { _StringsDefault0, 0x0002 };
static const XRect _Const001D = {{ 60, 5 }, { 120, 30 }};
static const XRect _Const001E = {{ 5, 4 }, { 55, 24 }};
static const XStringRes _Const001F = { _StringsDefault0, 0x000B };
static const XRect _Const0020 = {{ 370, 5 }, { 430, 30 }};
static const XRect _Const0021 = {{ 315, 4 }, { 365, 24 }};
static const XStringRes _Const0022 = { _StringsDefault0, 0x0014 };
static const XRect _Const0023 = {{ 0, 0 }, { 100, 30 }};
static const XRect _Const0024 = {{ 75, 0 }, { 92, 24 }};
static const XRect _Const0025 = {{ 60, 0 }, { 77, 24 }};
static const XRect _Const0026 = {{ 45, 0 }, { 62, 24 }};
static const XRect _Const0027 = {{ 30, 0 }, { 47, 24 }};
static const XRect _Const0028 = {{ 15, 0 }, { 32, 24 }};
static const XRect _Const0029 = {{ 0, 0 }, { 17, 24 }};
static const XColor _Const002A = { 0xFF, 0xFF, 0xFF, 0xFF };

/* Include a file containing the font resource : 'BrickGame::DescrFont' */
#include "_BrickGameDescrFont.h"

/* Table with links to derived variants of the font resource : 'BrickGame::DescrFont' */
EW_RES_WITHOUT_VARIANTS( BrickGameDescrFont )

/* Include a file containing the bitmap resource : 'BrickGame::Bricks' */
#include "_BrickGameBricks.h"

/* Table with links to derived variants of the bitmap resource : 'BrickGame::Bricks' */
EW_RES_WITHOUT_VARIANTS( BrickGameBricks )

/* Initializer for the class 'BrickGame::BrickRow' */
void BrickGameBrickRow__Init( BrickGameBrickRow _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( BrickGameBrickRow );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->Image1, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image2, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image3, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image4, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image5, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image6, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image7, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image8, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( BrickGameBrickRow );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0000 );
  CoreRectView__OnSetBounds( &_this->Image1, _Const0001 );
  ViewsImage_OnSetAutoSize( &_this->Image1, 1 );
  ViewsImage_OnSetFrameNumber( &_this->Image1, 0 );
  CoreRectView__OnSetBounds( &_this->Image2, _Const0002 );
  ViewsImage_OnSetAutoSize( &_this->Image2, 1 );
  ViewsImage_OnSetFrameNumber( &_this->Image2, 0 );
  CoreRectView__OnSetBounds( &_this->Image3, _Const0003 );
  ViewsImage_OnSetAutoSize( &_this->Image3, 1 );
  ViewsImage_OnSetFrameNumber( &_this->Image3, 0 );
  CoreRectView__OnSetBounds( &_this->Image4, _Const0004 );
  ViewsImage_OnSetAutoSize( &_this->Image4, 1 );
  ViewsImage_OnSetFrameNumber( &_this->Image4, 0 );
  CoreRectView__OnSetBounds( &_this->Image5, _Const0005 );
  ViewsImage_OnSetAutoSize( &_this->Image5, 1 );
  ViewsImage_OnSetFrameNumber( &_this->Image5, 0 );
  CoreRectView__OnSetBounds( &_this->Image6, _Const0006 );
  ViewsImage_OnSetAutoSize( &_this->Image6, 1 );
  ViewsImage_OnSetFrameNumber( &_this->Image6, 0 );
  CoreRectView__OnSetBounds( &_this->Image7, _Const0007 );
  ViewsImage_OnSetAutoSize( &_this->Image7, 1 );
  ViewsImage_OnSetFrameNumber( &_this->Image7, 0 );
  CoreRectView__OnSetBounds( &_this->Image8, _Const0008 );
  ViewsImage_OnSetAutoSize( &_this->Image8, 1 );
  ViewsImage_OnSetFrameNumber( &_this->Image8, 0 );
  _this->noOfBricks = 8;
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image5 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image6 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image7 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image8 ), 0 );
  ViewsImage_OnSetBitmap( &_this->Image1, EwLoadResource( &BrickGameBricks, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Image2, EwLoadResource( &BrickGameBricks, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Image3, EwLoadResource( &BrickGameBricks, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Image4, EwLoadResource( &BrickGameBricks, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Image5, EwLoadResource( &BrickGameBricks, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Image6, EwLoadResource( &BrickGameBricks, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Image7, EwLoadResource( &BrickGameBricks, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Image8, EwLoadResource( &BrickGameBricks, ResourcesBitmap ));
}

/* Re-Initializer for the class 'BrickGame::BrickRow' */
void BrickGameBrickRow__ReInit( BrickGameBrickRow _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->Image1 );
  ViewsImage__ReInit( &_this->Image2 );
  ViewsImage__ReInit( &_this->Image3 );
  ViewsImage__ReInit( &_this->Image4 );
  ViewsImage__ReInit( &_this->Image5 );
  ViewsImage__ReInit( &_this->Image6 );
  ViewsImage__ReInit( &_this->Image7 );
  ViewsImage__ReInit( &_this->Image8 );
}

/* Finalizer method for the class 'BrickGame::BrickRow' */
void BrickGameBrickRow__Done( BrickGameBrickRow _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->Image1 );
  ViewsImage__Done( &_this->Image2 );
  ViewsImage__Done( &_this->Image3 );
  ViewsImage__Done( &_this->Image4 );
  ViewsImage__Done( &_this->Image5 );
  ViewsImage__Done( &_this->Image6 );
  ViewsImage__Done( &_this->Image7 );
  ViewsImage__Done( &_this->Image8 );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method 'CheckCollision' checks for collisions of the given ball with a brick 
   of the row. In case of a collision, the direction of the ball will change, the 
   brick becomes invisble and the score is counted. The method returns the number 
   of remaining active bricks. */
XInt32 BrickGameBrickRow_CheckCollision( BrickGameBrickRow _this, BrickGameBallPosition 
  aBall )
{
  XRect overlapp = EwIntersectRect2( aBall->Bounds, _this->Super2.Bounds );
  CoreView view;

  if ( EwIsRectEmpty( overlapp ))
    return _this->noOfBricks;

  view = _this->Super1.first;

  while ( view != 0 )
  {
    ViewsImage image = EwCastObject( view, ViewsImage );

    if (( image != 0 ) && ViewsImage_OnGetVisible( image ))
    {
      XRect bounds = EwMoveRectPos( image->Super1.Bounds, _this->Super2.Bounds.Point1 );
      overlapp = EwIntersectRect2( aBall->Bounds, bounds );

      if ( !EwIsRectEmpty( overlapp ))
      {
        if ( EwGetRectW( overlapp ) < EwGetRectH( overlapp ))
        {
          if (( aBall->Bounds.Point1.X < bounds.Point1.X ) && ( aBall->SpeedX > 
              0.000000f ))
            aBall->SpeedX = -aBall->SpeedX;

          if (( aBall->Bounds.Point2.X > bounds.Point2.X ) && ( aBall->SpeedX < 
              0.000000f ))
            aBall->SpeedX = -aBall->SpeedX;
        }
        else
        {
          if (( aBall->Bounds.Point1.Y < bounds.Point1.Y ) && ( aBall->SpeedY > 
              0.000000f ))
            aBall->SpeedY = -aBall->SpeedY;

          if (( aBall->Bounds.Point2.Y > bounds.Point2.Y ) && ( aBall->SpeedY < 
              0.000000f ))
            aBall->SpeedY = -aBall->SpeedY;
        }

        if ( image->FrameNumber == 9 )
          return _this->noOfBricks;

        if ( image->FrameNumber == 7 )
          ViewsImage_OnSetFrameNumber( image, image->FrameNumber - 1 );
        else
          if ( image->FrameNumber == 6 )
            ViewsImage_OnSetFrameNumber( image, image->FrameNumber - 1 );
          else
          {
            BrickGameDataClass_OnSetScore( EwGetAutoObject( &BrickGameData, BrickGameDataClass ), 
            ( EwGetAutoObject( &BrickGameData, BrickGameDataClass )->Score + image->FrameNumber ) 
            + 1 );
            ViewsImage_OnSetVisible( image, 0 );
            _this->noOfBricks = _this->noOfBricks - 1;
          }

        return _this->noOfBricks;
      }
    }

    view = view->next;
  }

  return _this->noOfBricks;
}

/* The method 'PrepareBricks' initializes all bricks of the row. The given parameter 
   'aType' determines the style of the bricks. */
void BrickGameBrickRow_PrepareBricks( BrickGameBrickRow _this, XEnum aType )
{
  XBool visible = (XBool)( aType != BrickGameRowTypeHidden );

  ViewsImage_OnSetVisible( &_this->Image1, visible );
  ViewsImage_OnSetVisible( &_this->Image2, visible );
  ViewsImage_OnSetVisible( &_this->Image3, visible );
  ViewsImage_OnSetVisible( &_this->Image4, visible );
  ViewsImage_OnSetVisible( &_this->Image5, visible );
  ViewsImage_OnSetVisible( &_this->Image6, visible );
  ViewsImage_OnSetVisible( &_this->Image7, visible );
  ViewsImage_OnSetVisible( &_this->Image8, visible );

  switch ( aType )
  {
    case BrickGameRowTypeGray :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 0 );
      _this->noOfBricks = 8;
    }
    break;

    case BrickGameRowTypeBlue :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 1 );
      _this->noOfBricks = 8;
    }
    break;

    case BrickGameRowTypeGreen :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 2 );
      _this->noOfBricks = 8;
    }
    break;

    case BrickGameRowTypeYellow :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 3 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 3 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 3 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 3 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 3 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 3 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 3 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 3 );
      _this->noOfBricks = 8;
    }
    break;

    case BrickGameRowTypeWhite :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 4 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 4 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 4 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 4 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 4 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 4 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 4 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 4 );
      _this->noOfBricks = 8;
    }
    break;

    case BrickGameRowTypeConcrete :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 7 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 7 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 7 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 7 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 7 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 7 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 7 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 7 );
      _this->noOfBricks = 8;
    }
    break;

    case BrickGameRowTypeEmWi :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 8 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 8 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 8 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 8 );
      _this->noOfBricks = 8;
    }
    break;

    case BrickGameRowTypeTwoBlocks :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 9 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 9 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 0 );
      _this->noOfBricks = 6;
    }
    break;

    case BrickGameRowTypeTwoDoors :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 9 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 9 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 9 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 0 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 9 );
      _this->noOfBricks = 4;
    }
    break;

    case BrickGameRowTypeBlueGreen :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 1 );
      _this->noOfBricks = 8;
    }
    break;

    case BrickGameRowTypeWhiteEmWi :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 4 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 8 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 4 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 8 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 8 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 4 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 8 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 4 );
      _this->noOfBricks = 8;
    }
    break;

    case BrickGameRowTypeGreenBlue :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 2 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 1 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 2 );
      _this->noOfBricks = 8;
    }
    break;

    case BrickGameRowTypeOneDoor :
    {
      ViewsImage_OnSetFrameNumber( &_this->Image1, 9 );
      ViewsImage_OnSetFrameNumber( &_this->Image2, 9 );
      ViewsImage_OnSetFrameNumber( &_this->Image3, 9 );
      ViewsImage_OnSetFrameNumber( &_this->Image4, 7 );
      ViewsImage_OnSetFrameNumber( &_this->Image5, 7 );
      ViewsImage_OnSetFrameNumber( &_this->Image6, 9 );
      ViewsImage_OnSetFrameNumber( &_this->Image7, 9 );
      ViewsImage_OnSetFrameNumber( &_this->Image8, 9 );
      _this->noOfBricks = 2;
    }
    break;

    default : 
      _this->noOfBricks = 0;
  }
}

/* Variants derived from the class : 'BrickGame::BrickRow' */
EW_DEFINE_CLASS_VARIANTS( BrickGameBrickRow )
EW_END_OF_CLASS_VARIANTS( BrickGameBrickRow )

/* Virtual Method Table (VMT) for the class : 'BrickGame::BrickRow' */
EW_DEFINE_CLASS( BrickGameBrickRow, CoreGroup, Image1, Image1, Image1, Image1, noOfBricks, 
                 noOfBricks, "BrickGame::BrickRow" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( BrickGameBrickRow )

/* Initializer for the class 'BrickGame::BallPosition' */
void BrickGameBallPosition__Init( BrickGameBallPosition _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( BrickGameBallPosition );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( BrickGameBallPosition );
}

/* Re-Initializer for the class 'BrickGame::BallPosition' */
void BrickGameBallPosition__ReInit( BrickGameBallPosition _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'BrickGame::BallPosition' */
void BrickGameBallPosition__Done( BrickGameBallPosition _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* Variants derived from the class : 'BrickGame::BallPosition' */
EW_DEFINE_CLASS_VARIANTS( BrickGameBallPosition )
EW_END_OF_CLASS_VARIANTS( BrickGameBallPosition )

/* Virtual Method Table (VMT) for the class : 'BrickGame::BallPosition' */
EW_DEFINE_CLASS( BrickGameBallPosition, XObject, _None, _None, _None, _None, _None, 
                 _None, "BrickGame::BallPosition" )
EW_END_OF_CLASS( BrickGameBallPosition )

/* Initializer for the class 'BrickGame::BrickGame' */
void BrickGameBrickGame__Init( BrickGameBrickGame _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( BrickGameBrickGame );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->AnimationTimer, &_this->_XObject, 0 );
  BrickGameBallPosition__Init( &_this->BallPosition, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Rectangle, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Background, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Landscape, &_this->_XObject, 0 );
  BrickGameBrickRow__Init( &_this->BrickRow1, &_this->_XObject, 0 );
  BrickGameBrickRow__Init( &_this->BrickRow2, &_this->_XObject, 0 );
  BrickGameBrickRow__Init( &_this->BrickRow3, &_this->_XObject, 0 );
  BrickGameBrickRow__Init( &_this->BrickRow4, &_this->_XObject, 0 );
  BrickGameBrickRow__Init( &_this->BrickRow5, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandler, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Ball, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Paddle, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeyLeftHandler, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeyRightHandler, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeySpaceHandler, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->PaddleTimer, &_this->_XObject, 0 );
  BrickGameLcdDisplay__Init( &_this->ScoreDisplay, &_this->_XObject, 0 );
  ViewsText__Init( &_this->ScoreDescr, &_this->_XObject, 0 );
  BrickGameLcdDisplay__Init( &_this->LevelDisplay, &_this->_XObject, 0 );
  ViewsText__Init( &_this->LevelDescr, &_this->_XObject, 0 );
  BrickGameLcdDisplay__Init( &_this->LostDisplay, &_this->_XObject, 0 );
  ViewsText__Init( &_this->LostDescr, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->AutoDemoTimer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( BrickGameBrickGame );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0009 );
  CoreTimer_OnSetPeriod( &_this->AnimationTimer, 15 );
  _this->speedFactor = 0.150000f;
  CoreRectView__OnSetBounds( &_this->Rectangle, _Const000A );
  ViewsRectangle_OnSetColor( &_this->Rectangle, _Const000B );
  CoreRectView__OnSetBounds( &_this->Background, _Const000C );
  ViewsImage_OnSetAlignment( &_this->Background, ViewsImageAlignmentAlignHorzLeft 
  | ViewsImageAlignmentAlignVertCenter );
  ViewsImage_OnSetVisible( &_this->Background, 1 );
  CoreRectView__OnSetBounds( &_this->Landscape, _Const000D );
  ViewsImage_OnSetAlignment( &_this->Landscape, ViewsImageAlignmentAlignHorzLeft 
  | ViewsImageAlignmentAlignVertTop );
  ViewsImage_OnSetVisible( &_this->Landscape, 1 );
  CoreRectView__OnSetBounds( &_this->BrickRow1, _Const000E );
  CoreRectView__OnSetBounds( &_this->BrickRow2, _Const000F );
  CoreRectView__OnSetBounds( &_this->BrickRow3, _Const0010 );
  CoreRectView__OnSetBounds( &_this->BrickRow4, _Const0011 );
  CoreRectView__OnSetBounds( &_this->BrickRow5, _Const0012 );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->SimpleTouchHandler, _Const0013 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->SimpleTouchHandler, _Const0014 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->SimpleTouchHandler, _Const0015 );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->SimpleTouchHandler, _Const0016 );
  CoreRectView__OnSetBounds( &_this->Ball, _Const0017 );
  CoreRectView__OnSetBounds( &_this->Paddle, _Const0018 );
  ViewsImage_OnSetAutoSize( &_this->Paddle, 1 );
  _this->KeyLeftHandler.Filter = CoreKeyCodeLeft;
  _this->KeyRightHandler.Filter = CoreKeyCodeRight;
  _this->KeySpaceHandler.Filter = CoreKeyCodeSpace;
  CoreTimer_OnSetPeriod( &_this->PaddleTimer, 15 );
  _this->ballMove = 1;
  _this->paddlePos = 400;
  CoreRectView__OnSetBounds( &_this->ScoreDisplay, _Const0019 );
  BrickGameLcdDisplay_OnSetNoOfDigits( &_this->ScoreDisplay, 5 );
  BrickGameLcdDisplay_OnSetTextColor( &_this->ScoreDisplay, _Const001A );
  CoreRectView__OnSetBounds( &_this->ScoreDescr, _Const001B );
  ViewsText_OnSetAlignment( &_this->ScoreDescr, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertTop );
  ViewsText_OnSetString( &_this->ScoreDescr, EwLoadString( &_Const001C ));
  ViewsText_OnSetColor( &_this->ScoreDescr, _Const001A );
  CoreRectView__OnSetBounds( &_this->LevelDisplay, _Const001D );
  BrickGameLcdDisplay_OnSetNoOfDigits( &_this->LevelDisplay, 2 );
  BrickGameLcdDisplay_OnSetTextColor( &_this->LevelDisplay, _Const001A );
  CoreRectView__OnSetBounds( &_this->LevelDescr, _Const001E );
  ViewsText_OnSetAlignment( &_this->LevelDescr, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertTop );
  ViewsText_OnSetString( &_this->LevelDescr, EwLoadString( &_Const001F ));
  ViewsText_OnSetColor( &_this->LevelDescr, _Const001A );
  CoreRectView__OnSetBounds( &_this->LostDisplay, _Const0020 );
  BrickGameLcdDisplay_OnSetNoOfDigits( &_this->LostDisplay, 2 );
  BrickGameLcdDisplay_OnSetTextColor( &_this->LostDisplay, _Const001A );
  CoreRectView__OnSetBounds( &_this->LostDescr, _Const0021 );
  ViewsText_OnSetAlignment( &_this->LostDescr, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertTop );
  ViewsText_OnSetString( &_this->LostDescr, EwLoadString( &_Const0022 ));
  ViewsText_OnSetColor( &_this->LostDescr, _Const001A );
  CoreTimer_OnSetPeriod( &_this->AutoDemoTimer, 10 );
  CoreTimer_OnSetBegin( &_this->AutoDemoTimer, 0 );
  CoreTimer_OnSetEnabled( &_this->AutoDemoTimer, 1 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Rectangle ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Background ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Landscape ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BrickRow1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BrickRow2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BrickRow3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BrickRow4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BrickRow5 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Ball ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Paddle ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ScoreDisplay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ScoreDescr ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LevelDisplay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LevelDescr ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LostDisplay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LostDescr ), 0 );
  _this->AnimationTimer.OnTrigger = EwNewSlot( _this, BrickGameBrickGame_Animate );
  ViewsImage_OnSetBitmap( &_this->Background, EwLoadResource( &BrickGameStars, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Landscape, EwLoadResource( &BrickGameLandscape, 
  ResourcesBitmap ));
  _this->SimpleTouchHandler.OnDrag = EwNewSlot( _this, BrickGameBrickGame_onDragPaddle );
  _this->SimpleTouchHandler.OnRelease = EwNewSlot( _this, BrickGameBrickGame_onStartBall );
  _this->SimpleTouchHandler.OnPress = EwNewSlot( _this, BrickGameBrickGame_onDragPaddle );
  ViewsImage_OnSetBitmap( &_this->Ball, EwLoadResource( &BrickGameBall, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Paddle, EwLoadResource( &BrickGamePaddle, ResourcesBitmap ));
  _this->Data = EwGetAutoObject( &BrickGameData, BrickGameDataClass );
  _this->KeyLeftHandler.OnRelease = EwNewSlot( _this, BrickGameBrickGame_keyUpHandler );
  _this->KeyLeftHandler.OnPress = EwNewSlot( _this, BrickGameBrickGame_leftKeyDown );
  _this->KeyRightHandler.OnRelease = EwNewSlot( _this, BrickGameBrickGame_keyUpHandler );
  _this->KeyRightHandler.OnPress = EwNewSlot( _this, BrickGameBrickGame_rightKeyDown );
  _this->KeySpaceHandler.OnPress = EwNewSlot( _this, BrickGameBrickGame_onStartBall );
  _this->PaddleTimer.OnTrigger = EwNewSlot( _this, BrickGameBrickGame_updatePaddle );
  BrickGameLcdDisplay_OnSetOutlet( &_this->ScoreDisplay, EwNewRef( EwGetAutoObject( 
  &BrickGameData, BrickGameDataClass ), BrickGameDataClass_OnGetScore, BrickGameDataClass_OnSetScore ));
  ViewsText_OnSetFont( &_this->ScoreDescr, EwLoadResource( &BrickGameDescrFont, 
  ResourcesFont ));
  BrickGameLcdDisplay_OnSetOutlet( &_this->LevelDisplay, EwNewRef( EwGetAutoObject( 
  &BrickGameData, BrickGameDataClass ), BrickGameDataClass_OnGetLevel, BrickGameDataClass_OnSetLevel ));
  ViewsText_OnSetFont( &_this->LevelDescr, EwLoadResource( &BrickGameDescrFont, 
  ResourcesFont ));
  BrickGameLcdDisplay_OnSetOutlet( &_this->LostDisplay, EwNewRef( EwGetAutoObject( 
  &BrickGameData, BrickGameDataClass ), BrickGameDataClass_OnGetLost, BrickGameDataClass_OnSetLost ));
  ViewsText_OnSetFont( &_this->LostDescr, EwLoadResource( &BrickGameDescrFont, ResourcesFont ));
  _this->AutoDemoTimer.OnTrigger = EwNewSlot( _this, BrickGameBrickGame_AutoDemoSlot );
}

/* Re-Initializer for the class 'BrickGame::BrickGame' */
void BrickGameBrickGame__ReInit( BrickGameBrickGame _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->AnimationTimer );
  BrickGameBallPosition__ReInit( &_this->BallPosition );
  ViewsRectangle__ReInit( &_this->Rectangle );
  ViewsImage__ReInit( &_this->Background );
  ViewsImage__ReInit( &_this->Landscape );
  BrickGameBrickRow__ReInit( &_this->BrickRow1 );
  BrickGameBrickRow__ReInit( &_this->BrickRow2 );
  BrickGameBrickRow__ReInit( &_this->BrickRow3 );
  BrickGameBrickRow__ReInit( &_this->BrickRow4 );
  BrickGameBrickRow__ReInit( &_this->BrickRow5 );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandler );
  ViewsImage__ReInit( &_this->Ball );
  ViewsImage__ReInit( &_this->Paddle );
  CoreKeyPressHandler__ReInit( &_this->KeyLeftHandler );
  CoreKeyPressHandler__ReInit( &_this->KeyRightHandler );
  CoreKeyPressHandler__ReInit( &_this->KeySpaceHandler );
  CoreTimer__ReInit( &_this->PaddleTimer );
  BrickGameLcdDisplay__ReInit( &_this->ScoreDisplay );
  ViewsText__ReInit( &_this->ScoreDescr );
  BrickGameLcdDisplay__ReInit( &_this->LevelDisplay );
  ViewsText__ReInit( &_this->LevelDescr );
  BrickGameLcdDisplay__ReInit( &_this->LostDisplay );
  ViewsText__ReInit( &_this->LostDescr );
  CoreTimer__ReInit( &_this->AutoDemoTimer );
}

/* Finalizer method for the class 'BrickGame::BrickGame' */
void BrickGameBrickGame__Done( BrickGameBrickGame _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreTimer__Done( &_this->AnimationTimer );
  BrickGameBallPosition__Done( &_this->BallPosition );
  ViewsRectangle__Done( &_this->Rectangle );
  ViewsImage__Done( &_this->Background );
  ViewsImage__Done( &_this->Landscape );
  BrickGameBrickRow__Done( &_this->BrickRow1 );
  BrickGameBrickRow__Done( &_this->BrickRow2 );
  BrickGameBrickRow__Done( &_this->BrickRow3 );
  BrickGameBrickRow__Done( &_this->BrickRow4 );
  BrickGameBrickRow__Done( &_this->BrickRow5 );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandler );
  ViewsImage__Done( &_this->Ball );
  ViewsImage__Done( &_this->Paddle );
  CoreKeyPressHandler__Done( &_this->KeyLeftHandler );
  CoreKeyPressHandler__Done( &_this->KeyRightHandler );
  CoreKeyPressHandler__Done( &_this->KeySpaceHandler );
  CoreTimer__Done( &_this->PaddleTimer );
  BrickGameLcdDisplay__Done( &_this->ScoreDisplay );
  ViewsText__Done( &_this->ScoreDescr );
  BrickGameLcdDisplay__Done( &_this->LevelDisplay );
  ViewsText__Done( &_this->LevelDescr );
  BrickGameLcdDisplay__Done( &_this->LostDisplay );
  ViewsText__Done( &_this->LostDescr );
  CoreTimer__Done( &_this->AutoDemoTimer );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visibility or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void BrickGameBrickGame_UpdateViewState( BrickGameBrickGame _this, XSet aState )
{
  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  EwPostSignal( EwNewSlot( _this, BrickGameBrickGame_PrepareLevel ), ((XObject)_this ));
  CoreRectView__OnSetBounds( &_this->Ball, EwSetRectOrigin( _this->Ball.Super1.Bounds, 
  EwSetPointX( _this->Ball.Super1.Bounds.Point1, EwGetRectCenter( _this->Paddle.Super1.Bounds ).X 
  - ( EwGetRectW( _this->Ball.Super1.Bounds ) / 2 ))));
  CoreRectView__OnSetBounds( &_this->Ball, EwSetRectOrigin( _this->Ball.Super1.Bounds, 
  EwSetPointY( _this->Ball.Super1.Bounds.Point1, _this->Paddle.Super1.Bounds.Point1.Y 
  - EwGetRectH( _this->Ball.Super1.Bounds ))));
  _this->BallPosition.SpeedX = 0.200000f;
  _this->BallPosition.SpeedY = -1.000000f;
  CoreTimer_OnSetEnabled( &_this->AnimationTimer, 1 );
  _this->lastTime = _this->AnimationTimer.Time;
  _this->paddlePos = EwGetRectCenter( _this->Paddle.Super1.Bounds ).X;
}

/* This is a slot method. */
void BrickGameBrickGame_onDragPaddle( BrickGameBrickGame _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreTimer_OnSetEnabled( &_this->AutoDemoTimer, 0 );
  _this->paddlePos = _this->SimpleTouchHandler.CurrentPos.X;

  if ( _this->paddlePos < 0 )
    _this->paddlePos = 0;

  if ( _this->paddlePos > EwGetRectW( _this->Super2.Bounds ))
    _this->paddlePos = EwGetRectW( _this->Super2.Bounds );
}

/* The slot method 'Animate' is timer driven and calculates the next position of 
   ball, paddle and background. */
void BrickGameBrickGame_Animate( BrickGameBrickGame _this, XObject sender )
{
  XInt32 deltaX;
  XInt32 deltaY;
  XUInt32 deltaTime;
  XInt32 offset;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  deltaTime = _this->AnimationTimer.Time - _this->lastTime;
  _this->lastTime = _this->AnimationTimer.Time;

  if ( _this->ballMove == 1 )
  {
    deltaX = (XInt32)EwMathRound(((XFloat)deltaTime * _this->BallPosition.SpeedX ) 
    * _this->speedFactor );
    deltaY = (XInt32)EwMathRound(((XFloat)deltaTime * _this->BallPosition.SpeedY ) 
    * _this->speedFactor );
    CoreRectView__OnSetBounds( &_this->Ball, EwSetRectOrigin( _this->Ball.Super1.Bounds, 
    EwSetPointX( _this->Ball.Super1.Bounds.Point1, _this->Ball.Super1.Bounds.Point1.X 
    + deltaX )));
    CoreRectView__OnSetBounds( &_this->Ball, EwSetRectOrigin( _this->Ball.Super1.Bounds, 
    EwSetPointY( _this->Ball.Super1.Bounds.Point1, _this->Ball.Super1.Bounds.Point1.Y 
    + deltaY )));
    _this->BallPosition.Bounds = _this->Ball.Super1.Bounds;
    BrickGameBrickGame_CheckCollision( _this, &_this->BallPosition );
  }
  else
  {
    CoreRectView__OnSetBounds( &_this->Ball, EwSetRectOrigin( _this->Ball.Super1.Bounds, 
    EwSetPointX( _this->Ball.Super1.Bounds.Point1, EwGetRectCenter( _this->Paddle.Super1.Bounds ).X 
    - ( EwGetRectW( _this->Ball.Super1.Bounds ) / 2 ))));
    CoreRectView__OnSetBounds( &_this->Ball, EwSetRectOrigin( _this->Ball.Super1.Bounds, 
    EwSetPointY( _this->Ball.Super1.Bounds.Point1, _this->Paddle.Super1.Bounds.Point1.Y 
    - EwGetRectH( _this->Ball.Super1.Bounds ))));
    _this->BallPosition.Bounds = _this->Ball.Super1.Bounds;
  }

  deltaX = ( EwGetRectCenter( _this->Paddle.Super1.Bounds ).X - _this->paddlePos ) 
  / 2;
  CoreRectView__OnSetBounds( &_this->Paddle, EwSetRectOrigin( _this->Paddle.Super1.Bounds, 
  EwSetPointX( _this->Paddle.Super1.Bounds.Point1, _this->Paddle.Super1.Bounds.Point1.X 
  - deltaX )));
  offset = (( EwGetRectW( _this->Background.Super1.Bounds ) - _this->Background.Bitmap->FrameSize.X ) 
  * EwGetRectCenter( _this->Paddle.Super1.Bounds ).X ) / EwGetRectW( _this->Super2.Bounds );
  deltaX = ( _this->Background.ScrollOffset.X - offset ) / 4;
  ViewsImage_OnSetScrollOffset( &_this->Background, EwSetPointX( _this->Background.ScrollOffset, 
  _this->Background.ScrollOffset.X - deltaX ));
  offset = (( EwGetRectW( _this->Landscape.Super1.Bounds ) - _this->Landscape.Bitmap->FrameSize.X ) 
  * EwGetRectCenter( _this->Paddle.Super1.Bounds ).X ) / EwGetRectW( _this->Super2.Bounds );
  deltaX = ( _this->Landscape.ScrollOffset.X - offset ) / 4;
  ViewsImage_OnSetScrollOffset( &_this->Landscape, EwSetPointX( _this->Landscape.ScrollOffset, 
  _this->Landscape.ScrollOffset.X - deltaX ));
}

/* The slot method 'PrepareLevel' is used to initialize all brick rows according 
   the reached game level. */
void BrickGameBrickGame_PrepareLevel( BrickGameBrickGame _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  switch ( _this->Data->Level % 10 )
  {
    case 1 :
    {
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow1, BrickGameRowTypeGray );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow2, BrickGameRowTypeHidden );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow3, BrickGameRowTypeBlue );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow4, BrickGameRowTypeHidden );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow5, BrickGameRowTypeGray );
    }
    break;

    case 2 :
    {
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow1, BrickGameRowTypeGray );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow2, BrickGameRowTypeBlue );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow3, BrickGameRowTypeGreen );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow4, BrickGameRowTypeYellow );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow5, BrickGameRowTypeWhite );
    }
    break;

    case 3 :
    {
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow1, BrickGameRowTypeEmWi );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow2, BrickGameRowTypeHidden );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow3, BrickGameRowTypeConcrete );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow4, BrickGameRowTypeBlue );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow5, BrickGameRowTypeHidden );
    }
    break;

    case 4 :
    {
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow1, BrickGameRowTypeEmWi );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow2, BrickGameRowTypeHidden );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow3, BrickGameRowTypeConcrete );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow4, BrickGameRowTypeBlue );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow5, BrickGameRowTypeTwoBlocks );
    }
    break;

    case 5 :
    {
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow1, BrickGameRowTypeGray );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow2, BrickGameRowTypeHidden );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow3, BrickGameRowTypeTwoDoors );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow4, BrickGameRowTypeHidden );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow5, BrickGameRowTypeTwoDoors );
    }
    break;

    case 6 :
    {
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow1, BrickGameRowTypeEmWi );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow2, BrickGameRowTypeTwoBlocks );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow3, BrickGameRowTypeYellow );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow4, BrickGameRowTypeHidden );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow5, BrickGameRowTypeTwoDoors );
    }
    break;

    case 7 :
    {
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow1, BrickGameRowTypeConcrete );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow2, BrickGameRowTypeConcrete );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow3, BrickGameRowTypeHidden );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow4, BrickGameRowTypeTwoBlocks );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow5, BrickGameRowTypeTwoDoors );
    }
    break;

    case 8 :
    {
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow1, BrickGameRowTypeWhiteEmWi );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow2, BrickGameRowTypeEmWi );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow3, BrickGameRowTypeWhiteEmWi );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow4, BrickGameRowTypeHidden );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow5, BrickGameRowTypeTwoDoors );
    }
    break;

    case 9 :
    {
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow1, BrickGameRowTypeBlueGreen );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow2, BrickGameRowTypeGreenBlue );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow3, BrickGameRowTypeBlueGreen );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow4, BrickGameRowTypeGreenBlue );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow5, BrickGameRowTypeOneDoor );
    }
    break;

    default : 
    {
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow1, BrickGameRowTypeEmWi );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow2, BrickGameRowTypeTwoDoors );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow3, BrickGameRowTypeWhiteEmWi );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow4, BrickGameRowTypeGray );
      BrickGameBrickRow_PrepareBricks( &_this->BrickRow5, BrickGameRowTypeOneDoor );
    }
  }
}

/* The method 'CheckCollision' is responsible to detect any ball collision with 
   borders, paddle or other elements. */
void BrickGameBrickGame_CheckCollision( BrickGameBrickGame _this, BrickGameBallPosition 
  aBall )
{
  XInt32 noOfBricks = 0;

  noOfBricks = noOfBricks + BrickGameBrickRow_CheckCollision( &_this->BrickRow1, 
  aBall );
  noOfBricks = noOfBricks + BrickGameBrickRow_CheckCollision( &_this->BrickRow2, 
  aBall );
  noOfBricks = noOfBricks + BrickGameBrickRow_CheckCollision( &_this->BrickRow3, 
  aBall );
  noOfBricks = noOfBricks + BrickGameBrickRow_CheckCollision( &_this->BrickRow4, 
  aBall );
  noOfBricks = noOfBricks + BrickGameBrickRow_CheckCollision( &_this->BrickRow5, 
  aBall );

  if ( noOfBricks == 0 )
  {
    BrickGameDataClass_OnSetScore( _this->Data, _this->Data->Score + 50 );
    BrickGameDataClass_OnSetLevel( _this->Data, _this->Data->Level + 1 );
    EwPostSignal( EwNewSlot( _this, BrickGameBrickGame_PrepareLevel ), ((XObject)_this ));

    if ( _this->Data->Level < 25 )
      _this->speedFactor = _this->speedFactor + 0.005000f;
  }

  if ((( aBall->Bounds.Point1.X <= 0 ) && ( aBall->SpeedX < 0.000000f )) || (( aBall->Bounds.Point2.X 
      >= EwGetRectW( _this->Super2.Bounds )) && ( aBall->SpeedX > 0.000000f )))
    aBall->SpeedX = -aBall->SpeedX;

  if (( aBall->Bounds.Point1.Y <= 0 ) && ( aBall->SpeedY < 0.000000f ))
    aBall->SpeedY = -aBall->SpeedY;

  if ((( aBall->Bounds.Point2.Y > _this->Paddle.Super1.Bounds.Point1.Y ) && ( aBall->Bounds.Point2.Y 
      < _this->Paddle.Super1.Bounds.Point2.Y )) && ( aBall->SpeedY > 0.000000f ))
  {
    if (( aBall->Bounds.Point1.X < _this->Paddle.Super1.Bounds.Point1.X ) && ( aBall->Bounds.Point2.X 
        > _this->Paddle.Super1.Bounds.Point1.X ))
    {
      XInt32 dx = aBall->Bounds.Point2.X - _this->Paddle.Super1.Bounds.Point1.X;
      XInt32 dy = aBall->Bounds.Point2.Y - _this->Paddle.Super1.Bounds.Point1.Y;

      if ((( EwMathPow((XFloat)dx, 2.000000f ) + EwMathPow((XFloat)dy, 2.000000f )) 
          < EwMathPow((XFloat)EwGetRectW( aBall->Bounds ), 2.000000f )) && ( dy 
          < ( EwGetRectH( _this->Paddle.Super1.Bounds ) / 2 )))
      {
        aBall->SpeedY = -aBall->SpeedY;
        aBall->SpeedX = aBall->SpeedX + ((XFloat)( EwGetRectCenter( aBall->Bounds ).X 
        - EwGetRectCenter( _this->Paddle.Super1.Bounds ).X ) / (XFloat)EwGetRectW( 
        _this->Paddle.Super1.Bounds ));
      }
    }

    if (( aBall->Bounds.Point2.X > _this->Paddle.Super1.Bounds.Point2.X ) && ( aBall->Bounds.Point1.X 
        < _this->Paddle.Super1.Bounds.Point2.X ))
    {
      XInt32 dx = _this->Paddle.Super1.Bounds.Point2.X - aBall->Bounds.Point1.X;
      XInt32 dy = aBall->Bounds.Point2.Y - _this->Paddle.Super1.Bounds.Point1.Y;

      if ((( EwMathPow((XFloat)dx, 2.000000f ) + EwMathPow((XFloat)dy, 2.000000f )) 
          < EwMathPow((XFloat)EwGetRectW( aBall->Bounds ), 2.000000f )) && ( dy 
          < ( EwGetRectH( _this->Paddle.Super1.Bounds ) / 2 )))
      {
        aBall->SpeedY = -aBall->SpeedY;
        aBall->SpeedX = aBall->SpeedX + ((XFloat)( EwGetRectCenter( aBall->Bounds ).X 
        - EwGetRectCenter( _this->Paddle.Super1.Bounds ).X ) / (XFloat)EwGetRectW( 
        _this->Paddle.Super1.Bounds ));
      }
    }
    else
      if (( aBall->Bounds.Point1.X > _this->Paddle.Super1.Bounds.Point1.X ) && ( 
          aBall->Bounds.Point2.X < _this->Paddle.Super1.Bounds.Point2.X ))
      {
        aBall->SpeedY = -aBall->SpeedY;
        aBall->SpeedX = aBall->SpeedX + ((XFloat)( EwGetRectCenter( aBall->Bounds ).X 
        - EwGetRectCenter( _this->Paddle.Super1.Bounds ).X ) / (XFloat)EwGetRectW( 
        _this->Paddle.Super1.Bounds ));
      }
  }

  if ( aBall->Bounds.Point1.Y >= _this->Super2.Bounds.Point2.Y )
  {
    _this->ballMove = 0;
    BrickGameDataClass_OnSetLost( _this->Data, _this->Data->Lost + 1 );
    BrickGameDataClass_OnSetScore( _this->Data, _this->Data->Score - 100 );
  }
}

/* 'C' function for method : 'BrickGame::BrickGame.leftKeyDown()' */
void BrickGameBrickGame_leftKeyDown( BrickGameBrickGame _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreTimer_OnSetEnabled( &_this->AutoDemoTimer, 0 );
  _this->paddleDirection = -EwGetRectW( _this->Super2.Bounds ) / 64;
  EwSignal( EwNewSlot( _this, BrickGameBrickGame_updatePaddle ), ((XObject)_this ));
  CoreTimer_OnSetEnabled( &_this->PaddleTimer, 1 );
}

/* 'C' function for method : 'BrickGame::BrickGame.rightKeyDown()' */
void BrickGameBrickGame_rightKeyDown( BrickGameBrickGame _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreTimer_OnSetEnabled( &_this->AutoDemoTimer, 0 );
  _this->paddleDirection = EwGetRectW( _this->Super2.Bounds ) / 64;
  EwSignal( EwNewSlot( _this, BrickGameBrickGame_updatePaddle ), ((XObject)_this ));
  CoreTimer_OnSetEnabled( &_this->PaddleTimer, 1 );
}

/* 'C' function for method : 'BrickGame::BrickGame.keyUpHandler()' */
void BrickGameBrickGame_keyUpHandler( BrickGameBrickGame _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreTimer_OnSetEnabled( &_this->PaddleTimer, 0 );
}

/* 'C' function for method : 'BrickGame::BrickGame.updatePaddle()' */
void BrickGameBrickGame_updatePaddle( BrickGameBrickGame _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ((( _this->paddlePos > 0 ) && ( _this->paddleDirection < 0 )) || (( _this->paddlePos 
      < EwGetRectW( _this->Super2.Bounds )) && ( _this->paddleDirection > 0 )))
    _this->paddlePos = _this->paddlePos + _this->paddleDirection;

  if ( _this->paddlePos < 0 )
    _this->paddlePos = 0;

  if ( _this->paddlePos > EwGetRectW( _this->Super2.Bounds ))
    _this->paddlePos = EwGetRectW( _this->Super2.Bounds );
}

/* Slot method to start non-moving ball. */
void BrickGameBrickGame_onStartBall( BrickGameBrickGame _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->ballMove == 0 )
  {
    XFloat speedy = _this->BallPosition.SpeedY;

    if ( speedy < 0.000000f )
      speedy = -speedy;

    if ( _this->BallPosition.SpeedX < -speedy )
      _this->BallPosition.SpeedX = -speedy;

    if ( _this->BallPosition.SpeedX > speedy )
      _this->BallPosition.SpeedX = speedy;
  }

  _this->ballMove = 1;
}

/* Slot method to trigger the next phase of the auto demo. */
void BrickGameBrickGame_AutoDemoSlot( BrickGameBrickGame _this, XObject sender )
{
  XFloat speedy;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  _this->paddlePos = EwGetRectCenter( _this->BallPosition.Bounds ).X;
  speedy = _this->BallPosition.SpeedY;

  if ( speedy < 0.000000f )
    speedy = -speedy;

  if ( _this->BallPosition.SpeedX < -speedy )
    _this->BallPosition.SpeedX = -speedy;

  if ( _this->BallPosition.SpeedX > speedy )
    _this->BallPosition.SpeedX = speedy;

  _this->BallPosition.SpeedX = _this->BallPosition.SpeedX + EwMathRandFloat( -0.010000f, 
  0.010000f );
}

/* Variants derived from the class : 'BrickGame::BrickGame' */
EW_DEFINE_CLASS_VARIANTS( BrickGameBrickGame )
EW_END_OF_CLASS_VARIANTS( BrickGameBrickGame )

/* Virtual Method Table (VMT) for the class : 'BrickGame::BrickGame' */
EW_DEFINE_CLASS( BrickGameBrickGame, CoreGroup, Data, AnimationTimer, AnimationTimer, 
                 AnimationTimer, lastTime, lastTime, "BrickGame::BrickGame" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  BrickGameBrickGame_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( BrickGameBrickGame )

/* Include a file containing the bitmap resource : 'BrickGame::Paddle' */
#include "_BrickGamePaddle.h"

/* Table with links to derived variants of the bitmap resource : 'BrickGame::Paddle' */
EW_RES_WITHOUT_VARIANTS( BrickGamePaddle )

/* Include a file containing the bitmap resource : 'BrickGame::Ball' */
#include "_BrickGameBall.h"

/* Table with links to derived variants of the bitmap resource : 'BrickGame::Ball' */
EW_RES_WITHOUT_VARIANTS( BrickGameBall )

/* Initializer for the class 'BrickGame::DataClass' */
void BrickGameDataClass__Init( BrickGameDataClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  TemplatesDeviceClass__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( BrickGameDataClass );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( BrickGameDataClass );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Level = 1;
}

/* Re-Initializer for the class 'BrickGame::DataClass' */
void BrickGameDataClass__ReInit( BrickGameDataClass _this )
{
  /* At first re-initialize the super class ... */
  TemplatesDeviceClass__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'BrickGame::DataClass' */
void BrickGameDataClass__Done( BrickGameDataClass _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( TemplatesDeviceClass );

  /* Don't forget to deinitialize the super class ... */
  TemplatesDeviceClass__Done( &_this->_Super );
}

/* 'C' function for method : 'BrickGame::DataClass.OnSetScore()' */
void BrickGameDataClass_OnSetScore( BrickGameDataClass _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  if ( _this->Score == value )
    return;

  _this->Score = value;
  EwNotifyRefObservers( EwNewRef( _this, BrickGameDataClass_OnGetScore, BrickGameDataClass_OnSetScore ), 
    0 );
}

/* 'C' function for method : 'BrickGame::DataClass.OnSetLost()' */
void BrickGameDataClass_OnSetLost( BrickGameDataClass _this, XInt32 value )
{
  if ( _this->Lost == value )
    return;

  _this->Lost = value;
  EwNotifyRefObservers( EwNewRef( _this, BrickGameDataClass_OnGetLost, BrickGameDataClass_OnSetLost ), 
    0 );
}

/* 'C' function for method : 'BrickGame::DataClass.OnSetLevel()' */
void BrickGameDataClass_OnSetLevel( BrickGameDataClass _this, XInt32 value )
{
  if ( _this->Level == value )
    return;

  _this->Level = value;
  EwNotifyRefObservers( EwNewRef( _this, BrickGameDataClass_OnGetLevel, BrickGameDataClass_OnSetLevel ), 
    0 );
}

/* Default onget method for the property 'Score' */
XInt32 BrickGameDataClass_OnGetScore( BrickGameDataClass _this )
{
  return _this->Score;
}

/* Default onget method for the property 'Lost' */
XInt32 BrickGameDataClass_OnGetLost( BrickGameDataClass _this )
{
  return _this->Lost;
}

/* Default onget method for the property 'Level' */
XInt32 BrickGameDataClass_OnGetLevel( BrickGameDataClass _this )
{
  return _this->Level;
}

/* Variants derived from the class : 'BrickGame::DataClass' */
EW_DEFINE_CLASS_VARIANTS( BrickGameDataClass )
EW_END_OF_CLASS_VARIANTS( BrickGameDataClass )

/* Virtual Method Table (VMT) for the class : 'BrickGame::DataClass' */
EW_DEFINE_CLASS( BrickGameDataClass, TemplatesDeviceClass, _None, _None, _None, 
                 _None, _None, _None, "BrickGame::DataClass" )
EW_END_OF_CLASS( BrickGameDataClass )

/* User defined auto object: 'BrickGame::Data' */
EW_DEFINE_AUTOOBJECT( BrickGameData, BrickGameDataClass )

/* Initializer for the auto object 'BrickGame::Data' */
void BrickGameData__Init( BrickGameDataClass _this )
{
  EW_UNUSED_ARG( _this );
}

/* Table with links to derived variants of the auto object : 'BrickGame::Data' */
EW_DEFINE_AUTOOBJECT_VARIANTS( BrickGameData )
EW_END_OF_AUTOOBJECT_VARIANTS( BrickGameData )

/* Include a file containing the bitmap resource : 'BrickGame::Landscape' */
#include "_BrickGameLandscape.h"

/* Table with links to derived variants of the bitmap resource : 'BrickGame::Landscape' */
EW_RES_WITHOUT_VARIANTS( BrickGameLandscape )

/* Include a file containing the bitmap resource : 'BrickGame::Stars' */
#include "_BrickGameStars.h"

/* Table with links to derived variants of the bitmap resource : 'BrickGame::Stars' */
EW_RES_WITHOUT_VARIANTS( BrickGameStars )

/* Initializer for the class 'BrickGame::LcdDisplay' */
void BrickGameLcdDisplay__Init( BrickGameLcdDisplay _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( BrickGameLcdDisplay );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->Digit1, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Digit2, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Digit3, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Digit4, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Digit5, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Digit6, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( BrickGameLcdDisplay );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0023 );
  CoreRectView__OnSetBounds( &_this->Digit1, _Const0024 );
  ViewsImage_OnSetAutoSize( &_this->Digit1, 1 );
  CoreRectView__OnSetBounds( &_this->Digit2, _Const0025 );
  ViewsImage_OnSetAutoSize( &_this->Digit2, 1 );
  ViewsImage_OnSetFrameNumber( &_this->Digit2, 0 );
  CoreRectView__OnSetBounds( &_this->Digit3, _Const0026 );
  ViewsImage_OnSetAutoSize( &_this->Digit3, 1 );
  CoreRectView__OnSetBounds( &_this->Digit4, _Const0027 );
  ViewsImage_OnSetAutoSize( &_this->Digit4, 1 );
  CoreRectView__OnSetBounds( &_this->Digit5, _Const0028 );
  ViewsImage_OnSetAutoSize( &_this->Digit5, 1 );
  CoreRectView__OnSetBounds( &_this->Digit6, _Const0029 );
  ViewsImage_OnSetAutoSize( &_this->Digit6, 1 );
  _this->NoOfDigits = 6;
  _this->TextColor = _Const002A;
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Digit1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Digit2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Digit3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Digit4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Digit5 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Digit6 ), 0 );
  ViewsImage_OnSetBitmap( &_this->Digit1, EwLoadResource( &BrickGameLcdNumbers, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Digit2, EwLoadResource( &BrickGameLcdNumbers, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Digit3, EwLoadResource( &BrickGameLcdNumbers, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Digit4, EwLoadResource( &BrickGameLcdNumbers, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Digit5, EwLoadResource( &BrickGameLcdNumbers, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Digit6, EwLoadResource( &BrickGameLcdNumbers, 
  ResourcesBitmap ));
}

/* Re-Initializer for the class 'BrickGame::LcdDisplay' */
void BrickGameLcdDisplay__ReInit( BrickGameLcdDisplay _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->Digit1 );
  ViewsImage__ReInit( &_this->Digit2 );
  ViewsImage__ReInit( &_this->Digit3 );
  ViewsImage__ReInit( &_this->Digit4 );
  ViewsImage__ReInit( &_this->Digit5 );
  ViewsImage__ReInit( &_this->Digit6 );
}

/* Finalizer method for the class 'BrickGame::LcdDisplay' */
void BrickGameLcdDisplay__Done( BrickGameLcdDisplay _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->Digit1 );
  ViewsImage__Done( &_this->Digit2 );
  ViewsImage__Done( &_this->Digit3 );
  ViewsImage__Done( &_this->Digit4 );
  ViewsImage__Done( &_this->Digit5 );
  ViewsImage__Done( &_this->Digit6 );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visibility or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void BrickGameLcdDisplay_UpdateViewState( BrickGameLcdDisplay _this, XSet aState )
{
  XInt32 x;
  XInt32 val;
  XInt32 delta;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aState );

  x = 0;
  val = _this->CurrentValue;
  delta = 2;
  ViewsImage_OnSetFrameNumber( &_this->Digit1, val % 10 );
  ViewsImage_OnSetFrameNumber( &_this->Digit2, ( val / 10 ) % 10 );
  ViewsImage_OnSetFrameNumber( &_this->Digit3, ( val / 100 ) % 10 );
  ViewsImage_OnSetFrameNumber( &_this->Digit4, ( val / 1000 ) % 10 );
  ViewsImage_OnSetFrameNumber( &_this->Digit5, ( val / 10000 ) % 10 );
  ViewsImage_OnSetFrameNumber( &_this->Digit6, ( val / 100000 ) % 10 );

  if ( val < 0 )
  {
    ViewsImage_OnSetFrameNumber( &_this->Digit1, 10 );
    ViewsImage_OnSetFrameNumber( &_this->Digit2, 10 );
    ViewsImage_OnSetFrameNumber( &_this->Digit3, 10 );
    ViewsImage_OnSetFrameNumber( &_this->Digit4, 10 );
    ViewsImage_OnSetFrameNumber( &_this->Digit5, 10 );
    ViewsImage_OnSetFrameNumber( &_this->Digit6, 10 );
  }

  if ( _this->NoOfDigits > 5 )
  {
    ViewsImage_OnSetVisible( &_this->Digit6, 1 );
    CoreRectView__OnSetBounds( &_this->Digit6, EwSetRectOrigin( _this->Digit6.Super1.Bounds, 
    EwSetPointX( _this->Digit6.Super1.Bounds.Point1, x )));
    x = ( x + EwGetRectW( _this->Digit6.Super1.Bounds )) - delta;
  }
  else
    ViewsImage_OnSetVisible( &_this->Digit6, 0 );

  if ( _this->NoOfDigits > 4 )
  {
    ViewsImage_OnSetVisible( &_this->Digit5, 1 );
    CoreRectView__OnSetBounds( &_this->Digit5, EwSetRectOrigin( _this->Digit5.Super1.Bounds, 
    EwSetPointX( _this->Digit5.Super1.Bounds.Point1, x )));
    x = ( x + EwGetRectW( _this->Digit5.Super1.Bounds )) - delta;
  }
  else
    ViewsImage_OnSetVisible( &_this->Digit5, 0 );

  if ( _this->NoOfDigits > 3 )
  {
    ViewsImage_OnSetVisible( &_this->Digit4, 1 );
    CoreRectView__OnSetBounds( &_this->Digit4, EwSetRectOrigin( _this->Digit4.Super1.Bounds, 
    EwSetPointX( _this->Digit4.Super1.Bounds.Point1, x )));
    x = ( x + EwGetRectW( _this->Digit4.Super1.Bounds )) - delta;
  }
  else
    ViewsImage_OnSetVisible( &_this->Digit4, 0 );

  if ( _this->NoOfDigits > 2 )
  {
    ViewsImage_OnSetVisible( &_this->Digit3, 1 );
    CoreRectView__OnSetBounds( &_this->Digit3, EwSetRectOrigin( _this->Digit3.Super1.Bounds, 
    EwSetPointX( _this->Digit3.Super1.Bounds.Point1, x )));
    x = ( x + EwGetRectW( _this->Digit3.Super1.Bounds )) - delta;
  }
  else
    ViewsImage_OnSetVisible( &_this->Digit3, 0 );

  if ( _this->NoOfDigits > 1 )
  {
    ViewsImage_OnSetVisible( &_this->Digit2, 1 );
    CoreRectView__OnSetBounds( &_this->Digit2, EwSetRectOrigin( _this->Digit2.Super1.Bounds, 
    EwSetPointX( _this->Digit2.Super1.Bounds.Point1, x )));
    x = ( x + EwGetRectW( _this->Digit2.Super1.Bounds )) - delta;
  }
  else
    ViewsImage_OnSetVisible( &_this->Digit6, 0 );

  ViewsImage_OnSetVisible( &_this->Digit1, 1 );
  CoreRectView__OnSetBounds( &_this->Digit1, EwSetRectOrigin( _this->Digit1.Super1.Bounds, 
  EwSetPointX( _this->Digit1.Super1.Bounds.Point1, x )));
  x = ( x + EwGetRectW( _this->Digit1.Super1.Bounds )) - delta;
}

/* The onset method of the property 'NoOfDigits' changes the number of currently 
   displayed digits and rearranges the LCD display accordingly. */
void BrickGameLcdDisplay_OnSetNoOfDigits( BrickGameLcdDisplay _this, XInt32 value )
{
  if ( _this->NoOfDigits == value )
    return;

  if ( value < 1 )
    value = 1;

  if ( value > 6 )
    value = 6;

  _this->NoOfDigits = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* The onset method of the property 'Value' changes the currently displayed value 
   of all digits. */
void BrickGameLcdDisplay_OnSetCurrentValue( BrickGameLcdDisplay _this, XInt32 value )
{
  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void BrickGameLcdDisplay_outletSlot( BrickGameLcdDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    BrickGameLcdDisplay_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'BrickGame::LcdDisplay.OnSetOutlet()' */
void BrickGameLcdDisplay_OnSetOutlet( BrickGameLcdDisplay _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, BrickGameLcdDisplay_outletSlot ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, BrickGameLcdDisplay_outletSlot ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, BrickGameLcdDisplay_outletSlot ), ((XObject)_this ));
}

/* 'C' function for method : 'BrickGame::LcdDisplay.OnSetTextColor()' */
void BrickGameLcdDisplay_OnSetTextColor( BrickGameLcdDisplay _this, XColor value )
{
  _this->TextColor = value;
  ViewsImage_OnSetColor( &_this->Digit1, value );
  ViewsImage_OnSetColor( &_this->Digit2, value );
  ViewsImage_OnSetColor( &_this->Digit3, value );
  ViewsImage_OnSetColor( &_this->Digit4, value );
  ViewsImage_OnSetColor( &_this->Digit5, value );
  ViewsImage_OnSetColor( &_this->Digit6, value );
}

/* Variants derived from the class : 'BrickGame::LcdDisplay' */
EW_DEFINE_CLASS_VARIANTS( BrickGameLcdDisplay )
EW_END_OF_CLASS_VARIANTS( BrickGameLcdDisplay )

/* Virtual Method Table (VMT) for the class : 'BrickGame::LcdDisplay' */
EW_DEFINE_CLASS( BrickGameLcdDisplay, CoreGroup, Outlet, Outlet, Outlet, Digit1, 
                 NoOfDigits, NoOfDigits, "BrickGame::LcdDisplay" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  BrickGameLcdDisplay_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( BrickGameLcdDisplay )

/* Include a file containing the bitmap resource : 'BrickGame::LcdNumbers' */
#include "_BrickGameLcdNumbers.h"

/* Table with links to derived variants of the bitmap resource : 'BrickGame::LcdNumbers' */
EW_RES_WITHOUT_VARIANTS( BrickGameLcdNumbers )

/* Embedded Wizard */
