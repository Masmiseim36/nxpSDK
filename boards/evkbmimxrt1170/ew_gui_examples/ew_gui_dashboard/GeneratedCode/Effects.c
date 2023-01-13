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

#include "ewlocale.h"
#include "_CoreGroup.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_EffectsEffect.h"
#include "_EffectsEffectTimerClass.h"
#include "_EffectsFadeInOutTransition.h"
#include "_EffectsFader.h"
#include "_EffectsFaderTask.h"
#include "_EffectsFloatEffect.h"
#include "_EffectsInt32Effect.h"
#include "_EffectsOpacityFader.h"
#include "_EffectsPointEffect.h"
#include "_EffectsPositionFader.h"
#include "_EffectsShowHideTransition.h"
#include "_EffectsTimingList.h"
#include "_EffectsTransition.h"
#include "_EffectsVisibilityFader.h"
#include "Core.h"
#include "Effects.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x0000019E, /* ratio 53.14 % */
  0xB8008500, 0x000A8452, 0x00F20039, 0x0DC00348, 0x40003380, 0xE003A000, 0x0083C44D,
  0x536800CA, 0x8C9D8011, 0x345CC317, 0x19244003, 0x92250890, 0xA3916844, 0x29129144,
  0x60039C8A, 0x1716000D, 0xB34000EF, 0x9D98C010, 0x7CD21917, 0x802273E9, 0xB001253A,
  0x22F09A51, 0x1D0EA442, 0x11087432, 0x03140000, 0xA8D5020B, 0x8D49E932, 0xB49C0129,
  0xC3A691A8, 0x8A254613, 0x22D4B901, 0x2D148BD5, 0x142A04C6, 0x369A526E, 0x6AC8F6AD,
  0xA9C2A110, 0xF5CAED0F, 0x48E4362A, 0x1B0560C2, 0xE6B33B9E, 0x8B749CCD, 0x975894F2,
  0x488A4FE8, 0xAA5D2E45, 0x86D3E250, 0x055701A0, 0xD6EF8260, 0x5E5126C3, 0x2DB1AB5C,
  0x93438868, 0x31A8452E, 0x83A36775, 0x9C6D1A61, 0x80EFAAD7, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XPoint _Const0000 = { 100, 100 };
static const XStringRes _Const0001 = { _StringsDefault0, 0x0002 };
static const XStringRes _Const0002 = { _StringsDefault0, 0x0044 };
static const XStringRes _Const0003 = { _StringsDefault0, 0x006C };
static const XStringRes _Const0004 = { _StringsDefault0, 0x00A9 };

#ifndef EW_DONT_CHECK_INDEX
  /* This function is used to check the indices when accessing an array.
     If you don't want this verification add the define EW_DONT_CHECK_INDEX
     to your Makefile or project settings. */
  static int EwCheckIndex( int aIndex, int aRange, const char* aFile, int aLine )
  {
    if (( aIndex < 0 ) || ( aIndex >= aRange ))
    {
      EwPrint( "[FATAL ERROR in %s:%d] Array index %d out of bounds %d",
                aFile, aLine, aIndex, aRange );
      EwPanic();
    }
    return aIndex;
  }

  #define EwCheckIndex( aIndex, aRange ) \
    EwCheckIndex( aIndex, aRange, __FILE__, __LINE__ )
#else
  #define EwCheckIndex( aIndex, aRange ) aIndex
#endif

/* Initializer for the class 'Effects::Effect' */
void EffectsEffect__Init( EffectsEffect _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsEffect );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsEffect );

  /* ... and initialize objects, variables, properties, etc. */
  _this->invCycleDuration = 0.001000f;
  _this->Elasticity = 0.500000f;
  _this->Bounces = 3;
  _this->Oscillations = 3;
  _this->Amplitude = 0.500000f;
  _this->Exponent = 3.000000f;
  _this->Timing = EffectsTimingLinear;
  _this->CycleDuration = 1000;
}

/* Re-Initializer for the class 'Effects::Effect' */
void EffectsEffect__ReInit( EffectsEffect _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::Effect' */
void EffectsEffect__Done( EffectsEffect _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Effects::Effect.timerSlot()' */
void EffectsEffect_timerSlot( EffectsEffect _this, XObject sender )
{
  XBool done;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->timer == 0 )
    return;

  if ( _this->startDirection == 0 )
  {
    if ( _this->Reversed )
      _this->startDirection = -1;
    else
      _this->startDirection = 1;

    _this->direction = _this->startDirection;
    _this->startTime = _this->timer->Time;
    _this->cycleCounter = 0;
    _this->lastFrame = -1.000000f;
  }

  if (( _this->startDirection > 0 ) && ( _this->direction > 0 ))
    done = EffectsEffect_runFwdFwd( _this );
  else
    if (( _this->startDirection < 0 ) && ( _this->direction < 0 ))
      done = EffectsEffect_runRevRev( _this );
    else
      if ( _this->startDirection > 0 )
        done = EffectsEffect_runFwdRev( _this );
      else
        done = EffectsEffect_runRevFwd( _this );

  if ( done )
  {
    EffectsEffect_OnSetEnabled( _this, 0 );
    EwSignal( _this->privateOnFinished, ((XObject)_this ));
    EwSignal( _this->OnFinished, ((XObject)_this ));
  }
}

/* 'C' function for method : 'Effects::Effect.run()' */
void EffectsEffect_run( EffectsEffect _this, XFloat aFrame )
{
  if (( _this->timingList == 0 ) && ((( _this->Timing == EffectsTimingBounce_In ) 
      || ( _this->Timing == EffectsTimingBounce_Out )) || ( _this->Timing == EffectsTimingBounce_InOut )))
  {
    XInt32 bounces = _this->Bounces + 1;
    XFloat elasticity = EwMathSqrt( _this->Elasticity );
    XFloat sum = 0.500000f;
    XInt32 i;
    XFloat invSum;
    _this->timingList = EwNewObject( EffectsTimingList, 0 );
    _this->timingList->spans[ 0 ] = 1.000000f;
    _this->timingList->amps[ 0 ] = 1.000000f;

    for ( i = 1; i < bounces; i = i + 1 )
    {
      _this->timingList->spans[ EwCheckIndex( i, 12 )] = _this->timingList->spans[ 
      EwCheckIndex( i - 1, 12 )] * elasticity;
      _this->timingList->amps[ EwCheckIndex( i, 12 )] = _this->timingList->amps[ 
      EwCheckIndex( i - 1, 12 )] * _this->Elasticity;
      sum = sum + _this->timingList->spans[ EwCheckIndex( i, 12 )];
    }

    invSum = 1.000000f / sum;
    _this->timingList->spans[ 0 ] = 0.500000f;
    sum = 0.000000f;

    for ( i = 0; i < bounces; i = i + 1 )
    {
      _this->timingList->spans[ EwCheckIndex( i, 12 )] = _this->timingList->spans[ 
      EwCheckIndex( i, 12 )] * invSum;
      sum = sum + _this->timingList->spans[ EwCheckIndex( i, 12 )];
    }

    _this->timingList->spans[ EwCheckIndex( bounces, 12 )] = _this->timingList->spans[ 
    EwCheckIndex( bounces, 12 )] + ( 1.000000f - sum );
  }

  if ( _this->Symmetric )
  {
    if ( aFrame < 0.500000f )
      aFrame = aFrame * 2.000000f;
    else
      aFrame = 2.000000f - ( aFrame * 2.000000f );
  }

  switch ( _this->Timing )
  {
    case EffectsTimingPower_In :
    {
      aFrame = EwMathPow( aFrame, _this->Exponent );
    }
    break;

    case EffectsTimingPower_Out :
    {
      aFrame = 1.000000f - aFrame;
      aFrame = 1.000000f - EwMathPow( aFrame, _this->Exponent );
    }
    break;

    case EffectsTimingPower_InOut :
    {
      aFrame = aFrame * 2.000000f;

      if ( aFrame < 1.000000f )
        aFrame = EwMathPow( aFrame, _this->Exponent ) / 2.000000f;
      else
      {
        aFrame = 2.000000f - aFrame;
        aFrame = ( 2.000000f - EwMathPow( aFrame, _this->Exponent )) * 0.500000f;
      }
    }
    break;

    case EffectsTimingExp_In :
    {
      aFrame = ( EwMathPow( 2.718000f, _this->Exponent * aFrame ) - 1.000000f ) 
      / ( EwMathPow( 2.718000f, _this->Exponent ) - 1.000000f );
    }
    break;

    case EffectsTimingExp_Out :
    {
      aFrame = 1.000000f - aFrame;
      aFrame = 1.000000f - (( EwMathPow( 2.718000f, _this->Exponent * aFrame ) - 
      1.000000f ) / ( EwMathPow( 2.718000f, _this->Exponent ) - 1.000000f ));
    }
    break;

    case EffectsTimingExp_InOut :
    {
      aFrame = aFrame * 2.000000f;

      if ( aFrame < 1.000000f )
        aFrame = (( EwMathPow( 2.718000f, _this->Exponent * aFrame ) - 1.000000f ) 
        / ( EwMathPow( 2.718000f, _this->Exponent ) - 1.000000f )) / 2.000000f;
      else
      {
        aFrame = 2.000000f - aFrame;
        aFrame = ( 2.000000f - (( EwMathPow( 2.718000f, _this->Exponent * aFrame ) 
        - 1.000000f ) / ( EwMathPow( 2.718000f, _this->Exponent ) - 1.000000f ))) 
        * 0.500000f;
      }
    }
    break;

    case EffectsTimingSine_In :
    {
      aFrame = 1.000000f - EwMathCos( aFrame * 90.000000f );
    }
    break;

    case EffectsTimingSine_Out :
    {
      aFrame = EwMathSin( aFrame * 90.000000f );
    }
    break;

    case EffectsTimingSine_InOut :
    {
      aFrame = aFrame * 2.000000f;

      if ( aFrame < 1.000000f )
      {
        aFrame = 1.000000f - EwMathCos( aFrame * 90.000000f );
        aFrame = aFrame * 0.500000f;
      }
      else
      {
        aFrame = 2.000000f - aFrame;
        aFrame = 1.000000f - EwMathCos( aFrame * 90.000000f );
        aFrame = ( 2.000000f - aFrame ) * 0.500000f;
      }
    }
    break;

    case EffectsTimingCircle_In :
    {
      aFrame = 1.000000f - EwMathSqrt( 1.000000f - ( aFrame * aFrame ));
    }
    break;

    case EffectsTimingCircle_Out :
    {
      aFrame = 1.000000f - aFrame;
      aFrame = EwMathSqrt( 1.000000f - ( aFrame * aFrame ));
    }
    break;

    case EffectsTimingCircle_InOut :
    {
      aFrame = aFrame * 2.000000f;

      if ( aFrame < 1.000000f )
      {
        aFrame = 1.000000f - EwMathSqrt( 1.000000f - ( aFrame * aFrame ));
        aFrame = aFrame * 0.500000f;
      }
      else
      {
        aFrame = 2.000000f - aFrame;
        aFrame = 1.000000f - EwMathSqrt( 1.000000f - ( aFrame * aFrame ));
        aFrame = ( 2.000000f - aFrame ) * 0.500000f;
      }
    }
    break;

    case EffectsTimingBack_In :
    {
      aFrame = (( aFrame * aFrame ) * aFrame ) - (( aFrame * _this->Amplitude ) 
      * EwMathSin( aFrame * 180.000000f ));
    }
    break;

    case EffectsTimingBack_Out :
    {
      aFrame = 1.000000f - aFrame;
      aFrame = (( aFrame * aFrame ) * aFrame ) - (( aFrame * _this->Amplitude ) 
      * EwMathSin( aFrame * 180.000000f ));
      aFrame = 1.000000f - aFrame;
    }
    break;

    case EffectsTimingBack_InOut :
    {
      aFrame = aFrame * 2.000000f;

      if ( aFrame < 1.000000f )
      {
        aFrame = (( aFrame * aFrame ) * aFrame ) - (( aFrame * _this->Amplitude ) 
        * EwMathSin( aFrame * 180.000000f ));
        aFrame = aFrame * 0.500000f;
      }
      else
      {
        aFrame = 2.000000f - aFrame;
        aFrame = (( aFrame * aFrame ) * aFrame ) - (( aFrame * _this->Amplitude ) 
        * EwMathSin( aFrame * 180.000000f ));
        aFrame = ( 2.000000f - aFrame ) * 0.500000f;
      }
    }
    break;

    case EffectsTimingElastic_In :
    {
      aFrame = (( aFrame * aFrame ) * aFrame ) * EwMathSin(( aFrame * 90.000000f ) 
      * (XFloat)( 1 + ( 4 * _this->Oscillations )));
    }
    break;

    case EffectsTimingElastic_Out :
    {
      aFrame = 1.000000f - aFrame;
      aFrame = 1.000000f - ((( aFrame * aFrame ) * aFrame ) * EwMathSin(( aFrame 
      * 90.000000f ) * (XFloat)( 1 + ( 4 * _this->Oscillations ))));
    }
    break;

    case EffectsTimingElastic_InOut :
    {
      aFrame = aFrame * 2.000000f;

      if ( aFrame < 1.000000f )
      {
        aFrame = (( aFrame * aFrame ) * aFrame ) * EwMathSin(( aFrame * 90.000000f ) 
        * (XFloat)( 1 + ( 4 * _this->Oscillations )));
        aFrame = aFrame * 0.500000f;
      }
      else
      {
        aFrame = 2.000000f - aFrame;
        aFrame = (( aFrame * aFrame ) * aFrame ) * EwMathSin(( aFrame * 90.000000f ) 
        * (XFloat)( 1 + ( 4 * _this->Oscillations )));
        aFrame = ( 2.000000f - aFrame ) * 0.500000f;
      }
    }
    break;

    case EffectsTimingBounce_In :
    {
      EffectsTimingList list = _this->timingList;
      XInt32 i = 0;
      XFloat t1 = 0.000000f;
      XFloat t2 = list->spans[ 0 ];
      XFloat t = 1.000000f - aFrame;

      while ( t > t2 )
      {
        i = i + 1;
        t1 = t2;
        t2 = t2 + list->spans[ EwCheckIndex( i, 12 )];
      }

      aFrame = ( t - t1 ) / ( t2 - t1 );

      if ( i == 0 )
        aFrame = 1.000000f - ( aFrame * aFrame );
      else
      {
        aFrame = ( 2.000000f * aFrame ) - 1.000000f;
        aFrame = list->amps[ EwCheckIndex( i, 12 )] * ( 1.000000f - ( aFrame * aFrame ));
      }
    }
    break;

    case EffectsTimingBounce_Out :
    {
      EffectsTimingList list = _this->timingList;
      XInt32 i = 0;
      XFloat t1 = 0.000000f;
      XFloat t2 = list->spans[ 0 ];

      while ( aFrame > t2 )
      {
        i = i + 1;
        t1 = t2;
        t2 = t2 + list->spans[ EwCheckIndex( i, 12 )];
      }

      aFrame = ( aFrame - t1 ) / ( t2 - t1 );

      if ( i == 0 )
        aFrame = aFrame * aFrame;
      else
      {
        aFrame = ( 2.000000f * aFrame ) - 1.000000f;
        aFrame = 1.000000f - ( list->amps[ EwCheckIndex( i, 12 )] * ( 1.000000f 
        - ( aFrame * aFrame )));
      }
    }
    break;

    case EffectsTimingBounce_InOut :
    {
      EffectsTimingList list = _this->timingList;
      XInt32 i = 0;
      XFloat t1 = 0.000000f;
      XFloat t2 = list->spans[ 0 ];
      XFloat f = aFrame * 2.000000f;
      XFloat t = f - 1.000000f;

      if ( f < 1.000000f )
        t = -t;

      while ( t > t2 )
      {
        i = i + 1;
        t1 = t2;
        t2 = t2 + list->spans[ EwCheckIndex( i, 12 )];
      }

      t = ( t - t1 ) / ( t2 - t1 );

      if ( i == 0 )
        t = t * t;
      else
      {
        t = ( 2.000000f * t ) - 1.000000f;
        t = 1.000000f - ( list->amps[ EwCheckIndex( i, 12 )] * ( 1.000000f - ( t 
        * t )));
      }

      if ( f < 1.000000f )
        aFrame = 0.500000f * ( 1.000000f - t );
      else
        aFrame = 0.500000f * ( 1.000000f + t );
    }
    break;

    default : 
    {
      if ( _this->useBezier2 )
      {
        XFloat f = aFrame;
        XFloat nf = 1.000000f - f;
        aFrame = (( nf * f ) * ( _this->curveFactor1 + 1.000000f )) + ( f * f );
      }
      else
        if ( _this->useBezier3 )
        {
          XFloat f = aFrame;
          XFloat nf = 1.000000f - f;
          XFloat f2 = f * f;
          XFloat nf2 = nf * nf;
          aFrame = ((( nf2 * f ) * ( _this->curveFactor1 + 1.000000f )) + (( nf 
          * f2 ) * ( _this->curveFactor2 + 2.000000f ))) + ( f2 * f );
        }
    }
  }

  if ((( _this->Noise > 0.000000f ) && ( aFrame > 0.000000f )) && ( aFrame < 1.000000f ))
    aFrame = aFrame + EwMathRandFloat( -_this->Noise * 0.100000f, _this->Noise * 
    0.100000f );

  EffectsEffect__Animate( _this, aFrame );
  EwSignal( _this->OnAnimate, ((XObject)_this ));
}

/* 'C' function for method : 'Effects::Effect.runRevRev()' */
XBool EffectsEffect_runRevRev( EffectsEffect _this )
{
  XInt32 time = (XInt32)( _this->timer->Time - _this->startTime );
  XInt32 delay;
  XInt32 period0;
  XInt32 periodN;
  XBool done;
  XFloat frame;

  if ( time < 0 )
    time = -time;

  delay = _this->InitialDelay;
  period0 = _this->InitialDelay + _this->CycleDuration;
  periodN = _this->InterCycleDelay + _this->CycleDuration;
  done = 0;
  frame = _this->lastFrame;

  if (( _this->cycleCounter == 0 ) && ( time >= period0 ))
  {
    _this->cycleCounter = 1;
    time = time - period0;
    _this->startTime = _this->startTime + period0;
  }

  if (( _this->cycleCounter > 0 ) && ( time >= periodN ))
  {
    XInt32 cycles = time / periodN;
    _this->cycleCounter = _this->cycleCounter + cycles;
    time = time - ( cycles * periodN );
    _this->startTime = _this->startTime + ( cycles * periodN );
  }

  if (( _this->cycleCounter > 2 ) && ( _this->NoOfCycles == 0 ))
    _this->cycleCounter = 1;

  if ( _this->cycleCounter > 0 )
    delay = _this->InterCycleDelay;

  if (( _this->cycleCounter >= _this->NoOfCycles ) && ( _this->NoOfCycles > 0 ))
  {
    done = 1;
    frame = 0.000000f;
  }
  else
    if ( time >= delay )
      frame = 1.000000f - ((XFloat)( time - delay ) * _this->invCycleDuration );
    else
      if ( frame >= 0.000000f )
        frame = 0.000000f;

  if ( frame != _this->lastFrame )
  {
    _this->lastFrame = frame;
    EffectsEffect_run( _this, frame );
  }

  return done;
}

/* 'C' function for method : 'Effects::Effect.runRevFwd()' */
XBool EffectsEffect_runRevFwd( EffectsEffect _this )
{
  XInt32 time = (XInt32)( _this->startTime - _this->timer->Time );
  XInt32 delay = _this->InitialDelay;
  XInt32 period0 = _this->InitialDelay + _this->CycleDuration;
  XInt32 periodN = _this->InterCycleDelay + _this->CycleDuration;
  XBool done = 0;
  XFloat frame = _this->lastFrame;

  if (( _this->cycleCounter > 1 ) && ( time < 0 ))
  {
    XInt32 cycles = (( -time + periodN ) - 1 ) / periodN;

    if (( _this->cycleCounter - cycles ) <= 0 )
      cycles = _this->cycleCounter - 1;

    _this->cycleCounter = _this->cycleCounter - cycles;
    time = time + ( cycles * periodN );
    _this->startTime = _this->startTime + ( cycles * periodN );
  }

  if ((( _this->cycleCounter == 1 ) && ( time < 0 )) && ( _this->NoOfCycles > 0 ))
  {
    _this->cycleCounter = 0;
    time = time + period0;
    _this->startTime = _this->startTime + period0;
  }

  if ((( _this->cycleCounter == 1 ) && ( time < 0 )) && ( _this->NoOfCycles == 0 ))
  {
    XInt32 cycles = (( -time + periodN ) - 1 ) / periodN;
    time = time + ( cycles * periodN );
    _this->startTime = _this->startTime + ( cycles * periodN );
  }

  if ( _this->cycleCounter > 0 )
    delay = _this->InterCycleDelay;

  if ( time < 0 )
  {
    done = 1;
    frame = 1.000000f;
  }
  else
    if ( time >= delay )
      frame = 1.000000f - ((XFloat)( time - delay ) * _this->invCycleDuration );
    else
      if ( frame >= 0.000000f )
        frame = 1.000000f;

  if ( frame != _this->lastFrame )
  {
    _this->lastFrame = frame;
    EffectsEffect_run( _this, frame );
  }

  return done;
}

/* 'C' function for method : 'Effects::Effect.runFwdRev()' */
XBool EffectsEffect_runFwdRev( EffectsEffect _this )
{
  XInt32 time = (XInt32)( _this->startTime - _this->timer->Time );
  XInt32 delay = _this->InitialDelay;
  XInt32 period0 = _this->InitialDelay + _this->CycleDuration;
  XInt32 periodN = _this->InterCycleDelay + _this->CycleDuration;
  XBool done = 0;
  XFloat frame = _this->lastFrame;

  if (( _this->cycleCounter > 1 ) && ( time < 0 ))
  {
    XInt32 cycles = (( -time + periodN ) - 1 ) / periodN;

    if (( _this->cycleCounter - cycles ) <= 0 )
      cycles = _this->cycleCounter - 1;

    _this->cycleCounter = _this->cycleCounter - cycles;
    time = time + ( cycles * periodN );
    _this->startTime = _this->startTime + ( cycles * periodN );
  }

  if ((( _this->cycleCounter == 1 ) && ( time < 0 )) && ( _this->NoOfCycles > 0 ))
  {
    _this->cycleCounter = 0;
    time = time + period0;
    _this->startTime = _this->startTime + period0;
  }

  if ((( _this->cycleCounter == 1 ) && ( time < 0 )) && ( _this->NoOfCycles == 0 ))
  {
    XInt32 cycles = (( -time + periodN ) - 1 ) / periodN;
    time = time + ( cycles * periodN );
    _this->startTime = _this->startTime + ( cycles * periodN );
  }

  if ( _this->cycleCounter > 0 )
    delay = _this->InterCycleDelay;

  if ( time < 0 )
  {
    done = 1;
    frame = 0.000000f;
  }
  else
    if ( time >= delay )
      frame = (XFloat)( time - delay ) * _this->invCycleDuration;
    else
      if ( frame >= 0.000000f )
        frame = 0.000000f;

  if ( frame != _this->lastFrame )
  {
    _this->lastFrame = frame;
    EffectsEffect_run( _this, frame );
  }

  return done;
}

/* 'C' function for method : 'Effects::Effect.runFwdFwd()' */
XBool EffectsEffect_runFwdFwd( EffectsEffect _this )
{
  XInt32 time = (XInt32)( _this->timer->Time - _this->startTime );
  XInt32 delay;
  XInt32 period0;
  XInt32 periodN;
  XBool done;
  XFloat frame;

  if ( time < 0 )
    time = -time;

  delay = _this->InitialDelay;
  period0 = _this->InitialDelay + _this->CycleDuration;
  periodN = _this->InterCycleDelay + _this->CycleDuration;
  done = 0;
  frame = _this->lastFrame;

  if (( _this->cycleCounter == 0 ) && ( time >= period0 ))
  {
    _this->cycleCounter = 1;
    time = time - period0;
    _this->startTime = _this->startTime + period0;
  }

  if (( _this->cycleCounter > 0 ) && ( time >= periodN ))
  {
    XInt32 cycles = time / periodN;
    _this->cycleCounter = _this->cycleCounter + cycles;
    time = time - ( cycles * periodN );
    _this->startTime = _this->startTime + ( cycles * periodN );
  }

  if (( _this->cycleCounter > 2 ) && ( _this->NoOfCycles == 0 ))
    _this->cycleCounter = 1;

  if ( _this->cycleCounter > 0 )
    delay = _this->InterCycleDelay;

  if (( _this->cycleCounter >= _this->NoOfCycles ) && ( _this->NoOfCycles > 0 ))
  {
    done = 1;
    frame = 1.000000f;
  }
  else
    if ( time >= delay )
      frame = (XFloat)( time - delay ) * _this->invCycleDuration;
    else
      if ( frame >= 0.000000f )
        frame = 1.000000f;

  if ( frame != _this->lastFrame )
  {
    _this->lastFrame = frame;
    EffectsEffect_run( _this, frame );
  }

  return done;
}

/* 'C' function for method : 'Effects::Effect.OnSetReversed()' */
void EffectsEffect_OnSetReversed( EffectsEffect _this, XBool value )
{
  if ( _this->Reversed == value )
    return;

  _this->Reversed = value;

  if ( !_this->Enabled || ( _this->startDirection == 0 ))
    return;

  if ( value )
    _this->direction = -1;
  else
    _this->direction = 1;

  _this->startTime = ( CoreTimer_GetCurrentTime( _this->timer ) * 2 ) - _this->startTime;
}

/* 'C' function for method : 'Effects::Effect.OnSetTimingCustom2()' */
void EffectsEffect_OnSetTimingCustom2( EffectsEffect _this, XFloat value )
{
  if ( _this->TimingCustom2 == value )
    return;

  _this->TimingCustom2 = value;

  if ( _this->Timing == EffectsTimingCustom )
  {
    _this->curveFactor2 = value;
    _this->useBezier2 = (XBool)(( _this->curveFactor1 == _this->curveFactor2 ) && 
    ( _this->curveFactor1 != 0.000000f ));
    _this->useBezier3 = (XBool)( !_this->useBezier2 && ( _this->curveFactor1 != 
    _this->curveFactor2 ));
  }
}

/* 'C' function for method : 'Effects::Effect.OnSetTimingCustom1()' */
void EffectsEffect_OnSetTimingCustom1( EffectsEffect _this, XFloat value )
{
  if ( _this->TimingCustom1 == value )
    return;

  _this->TimingCustom1 = value;

  if ( _this->Timing == EffectsTimingCustom )
  {
    _this->curveFactor1 = value;
    _this->useBezier2 = (XBool)(( _this->curveFactor1 == _this->curveFactor2 ) && 
    ( _this->curveFactor1 != 0.000000f ));
    _this->useBezier3 = (XBool)( !_this->useBezier2 && ( _this->curveFactor1 != 
    _this->curveFactor2 ));
  }
}

/* 'C' function for method : 'Effects::Effect.OnSetTiming()' */
void EffectsEffect_OnSetTiming( EffectsEffect _this, XEnum value )
{
  if ( _this->Timing == value )
    return;

  _this->Timing = value;
  _this->timingList = 0;

  switch ( value )
  {
    case EffectsTimingEaseIn_EaseOut :
    {
      _this->curveFactor1 = -1.100000f;
      _this->curveFactor2 = 1.100000f;
    }
    break;

    case EffectsTimingEaseIn_FastOut :
    {
      _this->curveFactor1 = -1.000000f;
      _this->curveFactor2 = -2.000000f;
    }
    break;

    case EffectsTimingFastIn_EaseOut :
    {
      _this->curveFactor1 = 2.000000f;
      _this->curveFactor2 = 1.000000f;
    }
    break;

    case EffectsTimingFastIn_FastOut :
    {
      _this->curveFactor1 = 1.100000f;
      _this->curveFactor2 = -1.100000f;
    }
    break;

    case EffectsTimingLinear :
    {
      _this->curveFactor1 = 0.000000f;
      _this->curveFactor2 = 0.000000f;
    }
    break;

    default : 
    {
      _this->curveFactor1 = _this->TimingCustom1;
      _this->curveFactor2 = _this->TimingCustom2;
    }
  }

  _this->useBezier2 = (XBool)(( _this->curveFactor1 == _this->curveFactor2 ) && 
  ( _this->curveFactor1 != 0.000000f ));
  _this->useBezier3 = (XBool)( !_this->useBezier2 && ( _this->curveFactor1 != _this->curveFactor2 ));
}

/* 'C' function for method : 'Effects::Effect.OnSetNoOfCycles()' */
void EffectsEffect_OnSetNoOfCycles( EffectsEffect _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  _this->NoOfCycles = value;
}

/* 'C' function for method : 'Effects::Effect.OnSetCycleDuration()' */
void EffectsEffect_OnSetCycleDuration( EffectsEffect _this, XInt32 value )
{
  if ( value < 15 )
    value = 15;

  _this->CycleDuration = value;
  _this->invCycleDuration = 1.000000f / (XFloat)value;
}

/* 'C' function for method : 'Effects::Effect.OnSetInitialDelay()' */
void EffectsEffect_OnSetInitialDelay( EffectsEffect _this, XInt32 value )
{
  if ( value < 0 )
    value = 0;

  _this->InitialDelay = value;
}

/* 'C' function for method : 'Effects::Effect.OnSetEnabled()' */
void EffectsEffect_OnSetEnabled( EffectsEffect _this, XBool value )
{
  if ( _this->Enabled == value )
    return;

  _this->Enabled = value;

  if ( !value && ( _this->timer != 0 ))
  {
    EwDetachObjObserver( EwNewSlot( _this, EffectsEffect_timerSlot ), (XObject)_this->timer, 
      0 );
    _this->timer = 0;
  }

  if ( value )
  {
    _this->timer = ((CoreTimer)EwGetAutoObject( &EffectsEffectTimer, EffectsEffectTimerClass ));
    EwAttachObjObserver( EwNewSlot( _this, EffectsEffect_timerSlot ), (XObject)_this->timer, 
      0 );
    _this->startDirection = 0;
    EwPostSignal( EwNewSlot( _this, EffectsEffect_timerSlot ), ((XObject)_this ));
  }
}

/* 'C' function for method : 'Effects::Effect.Animate()' */
void EffectsEffect_Animate( EffectsEffect _this, XFloat aProgress )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
  EW_UNUSED_ARG( aProgress );
}

/* Wrapper function for the virtual method : 'Effects::Effect.Animate()' */
void EffectsEffect__Animate( void* _this, XFloat aProgress )
{
  ((EffectsEffect)_this)->_VMT->Animate((EffectsEffect)_this, aProgress );
}

/* The slot method 'StopEffect' stops the running effect if a signal is sent to 
   this slot method. This causes the animation to pause at its current position. */
void EffectsEffect_StopEffect( EffectsEffect _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EffectsEffect_OnSetEnabled( _this, 0 );
}

/* The slot method 'StartEffect' re-starts the effect if a signal is sent to this 
   slot method. The effect will start from the beginning. */
void EffectsEffect_StartEffect( EffectsEffect _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Enabled )
    EffectsEffect_OnSetEnabled( _this, 0 );

  EffectsEffect_OnSetEnabled( _this, 1 );
}

/* Variants derived from the class : 'Effects::Effect' */
EW_DEFINE_CLASS_VARIANTS( EffectsEffect )
EW_END_OF_CLASS_VARIANTS( EffectsEffect )

/* Virtual Method Table (VMT) for the class : 'Effects::Effect' */
EW_DEFINE_CLASS( EffectsEffect, XObject, timingList, privateOnFinished, direction, 
                 direction, direction, direction, "Effects::Effect" )
  EffectsEffect_Animate,
EW_END_OF_CLASS( EffectsEffect )

/* Initializer for the class 'Effects::Int32Effect' */
void EffectsInt32Effect__Init( EffectsInt32Effect _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  EffectsEffect__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsInt32Effect );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsInt32Effect );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Value2 = 255;
}

/* Re-Initializer for the class 'Effects::Int32Effect' */
void EffectsInt32Effect__ReInit( EffectsInt32Effect _this )
{
  /* At first re-initialize the super class ... */
  EffectsEffect__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::Int32Effect' */
void EffectsInt32Effect__Done( EffectsInt32Effect _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( EffectsEffect );

  /* Don't forget to deinitialize the super class ... */
  EffectsEffect__Done( &_this->_Super );
}

/* 'C' function for method : 'Effects::Int32Effect.Animate()' */
void EffectsInt32Effect_Animate( EffectsInt32Effect _this, XFloat aProgress )
{
  _this->Value = _this->Value1 + (XInt32)EwMathRound((XFloat)( _this->Value2 - _this->Value1 ) 
  * aProgress );

  if ( _this->Outlet.Object != 0 )
    EwOnSetInt32( _this->Outlet, _this->Value );
}

/* Variants derived from the class : 'Effects::Int32Effect' */
EW_DEFINE_CLASS_VARIANTS( EffectsInt32Effect )
EW_END_OF_CLASS_VARIANTS( EffectsInt32Effect )

/* Virtual Method Table (VMT) for the class : 'Effects::Int32Effect' */
EW_DEFINE_CLASS( EffectsInt32Effect, EffectsEffect, Outlet, Outlet, Outlet, Value, 
                 Value, Value, "Effects::Int32Effect" )
  EffectsInt32Effect_Animate,
EW_END_OF_CLASS( EffectsInt32Effect )

/* Initializer for the class 'Effects::PointEffect' */
void EffectsPointEffect__Init( EffectsPointEffect _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  EffectsEffect__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsPointEffect );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsPointEffect );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Value2 = _Const0000;
}

/* Re-Initializer for the class 'Effects::PointEffect' */
void EffectsPointEffect__ReInit( EffectsPointEffect _this )
{
  /* At first re-initialize the super class ... */
  EffectsEffect__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::PointEffect' */
void EffectsPointEffect__Done( EffectsPointEffect _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( EffectsEffect );

  /* Don't forget to deinitialize the super class ... */
  EffectsEffect__Done( &_this->_Super );
}

/* 'C' function for method : 'Effects::PointEffect.Animate()' */
void EffectsPointEffect_Animate( EffectsPointEffect _this, XFloat aProgress )
{
  XInt32 x = _this->Value1.X;
  XInt32 y = _this->Value1.Y;

  x = x + (XInt32)EwMathRound((XFloat)( _this->Value2.X - x ) * aProgress );
  y = y + (XInt32)EwMathRound((XFloat)( _this->Value2.Y - y ) * aProgress );
  _this->Value = EwNewPoint( x, y );

  if ( _this->Outlet.Object != 0 )
    EwOnSetPoint( _this->Outlet, _this->Value );
}

/* Variants derived from the class : 'Effects::PointEffect' */
EW_DEFINE_CLASS_VARIANTS( EffectsPointEffect )
EW_END_OF_CLASS_VARIANTS( EffectsPointEffect )

/* Virtual Method Table (VMT) for the class : 'Effects::PointEffect' */
EW_DEFINE_CLASS( EffectsPointEffect, EffectsEffect, Outlet, Outlet, Outlet, Value, 
                 Value, Value, "Effects::PointEffect" )
  EffectsPointEffect_Animate,
EW_END_OF_CLASS( EffectsPointEffect )

/* Initializer for the class 'Effects::FloatEffect' */
void EffectsFloatEffect__Init( EffectsFloatEffect _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  EffectsEffect__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsFloatEffect );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsFloatEffect );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Value2 = 1.000000f;
}

/* Re-Initializer for the class 'Effects::FloatEffect' */
void EffectsFloatEffect__ReInit( EffectsFloatEffect _this )
{
  /* At first re-initialize the super class ... */
  EffectsEffect__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::FloatEffect' */
void EffectsFloatEffect__Done( EffectsFloatEffect _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( EffectsEffect );

  /* Don't forget to deinitialize the super class ... */
  EffectsEffect__Done( &_this->_Super );
}

/* 'C' function for method : 'Effects::FloatEffect.Animate()' */
void EffectsFloatEffect_Animate( EffectsFloatEffect _this, XFloat aProgress )
{
  _this->Value = _this->Value1 + (( _this->Value2 - _this->Value1 ) * aProgress );

  if ( _this->Outlet.Object != 0 )
    EwOnSetFloat( _this->Outlet, _this->Value );
}

/* Variants derived from the class : 'Effects::FloatEffect' */
EW_DEFINE_CLASS_VARIANTS( EffectsFloatEffect )
EW_END_OF_CLASS_VARIANTS( EffectsFloatEffect )

/* Virtual Method Table (VMT) for the class : 'Effects::FloatEffect' */
EW_DEFINE_CLASS( EffectsFloatEffect, EffectsEffect, Outlet, Outlet, Outlet, Value, 
                 Value, Value, "Effects::FloatEffect" )
  EffectsFloatEffect_Animate,
EW_END_OF_CLASS( EffectsFloatEffect )

/* Initializer for the class 'Effects::Fader' */
void EffectsFader__Init( EffectsFader _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsFader );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsFader );

  /* ... and initialize objects, variables, properties, etc. */
  _this->RemoveIfHidden = 1;
  _this->RestackTopmost = 1;
  _this->AssignFocus = 1;
  _this->UseCurrentState = 1;
  _this->Visible = 1;
}

/* Re-Initializer for the class 'Effects::Fader' */
void EffectsFader__ReInit( EffectsFader _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::Fader' */
void EffectsFader__Done( EffectsFader _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* The method Complete() is intended to be called by the implementation of your 
   adapted fader as soon as the animation effects used inside the fader has finalized 
   their work. This is essential in order to ensure, that next faders waiting for 
   execution can start with their animations. */
void EffectsFader_Complete( EffectsFader _this )
{
  if ( _this->task != 0 )
    CoreTask__Complete( _this->task );
}

/* The method IsFinished() should return 'true' if the fader has finalized its fading 
   animation and 'false' if the animation is not started or it is still in progress. 
   This method should therefore be overridden in derived classes to evaluate the 
   state of animation effects used inside the fader. */
XBool EffectsFader_IsFinished( EffectsFader _this )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );

  return 1;
}

/* Wrapper function for the virtual method : 'Effects::Fader.IsFinished()' */
XBool EffectsFader__IsFinished( void* _this )
{
  return ((EffectsFader)_this)->_VMT->IsFinished((EffectsFader)_this );
}

/* The method OnEnd() is invoked automatically just in the moment, when this fader 
   and all other faders belonging to the same fading task have signaled that they 
   are finished with their animations. The default implementation of this method 
   simply does nothing.
   Derived classes can override this method and implement there the code to complete 
   the animation (e.g. to remove the group from its owner after it has been faded-out, 
   etc.). The implementation of the overridden OnEnd() method has to use the variables 
   @Group and @Owner to determine on which group the fader has applied the animation. 
   Depending on the kind of the implemented animation, the OnEnd() method can also 
   evaluate the properties @UseCurrentState, @AssignFocus and @RestackTopmost. */
void EffectsFader_OnEnd( EffectsFader _this )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );
}

/* Wrapper function for the virtual method : 'Effects::Fader.OnEnd()' */
void EffectsFader__OnEnd( void* _this )
{
  ((EffectsFader)_this)->_VMT->OnEnd((EffectsFader)_this );
}

/* The method OnStart() is invoked automatically just in the moment, when the fader 
   is triggered to begin its animation. The default implementation of this method 
   simply calls the @Complete() method indicating so, that the fader is done with 
   its work. Thereupon the next fader waiting for its execution can start.
   Derived classes should override this method and implement there the code to prepare 
   and start the desired fade-in/out animation effects. The implementation of the 
   overridden OnStart() method has to use the variables @Group and @Owner to determine 
   on which group the fader should apply the animation. Depending on the kind of 
   the implemented animation, the OnStart() method can also evaluate the properties 
   @UseCurrentState, @AssignFocus and @RestackTopmost.
   Please note, if there is a slot method associated to the property @OnInitialize, 
   this slot method is signaled shortly before the method OnStart() is executed 
   given the slot method a possibility to apply more individual initialization steps 
   on the fader just before the animation begins.
   When the animation reaches its end (e.g. when the animation effects used inside 
   the fader do terminate), your implementation of the fader should invoke the method 
   @Complete() otherwise the animation is considered as still being in progress 
   causing other pending faders to wait. */
void EffectsFader_OnStart( EffectsFader _this )
{
  EffectsFader_Complete( _this );
}

/* Wrapper function for the virtual method : 'Effects::Fader.OnStart()' */
void EffectsFader__OnStart( void* _this )
{
  ((EffectsFader)_this)->_VMT->OnStart((EffectsFader)_this );
}

/* Variants derived from the class : 'Effects::Fader' */
EW_DEFINE_CLASS_VARIANTS( EffectsFader )
EW_END_OF_CLASS_VARIANTS( EffectsFader )

/* Virtual Method Table (VMT) for the class : 'Effects::Fader' */
EW_DEFINE_CLASS( EffectsFader, XObject, task, onCancel, Visible, Visible, Visible, 
                 Visible, "Effects::Fader" )
  EffectsFader_IsFinished,
  EffectsFader_OnEnd,
  EffectsFader_OnStart,
EW_END_OF_CLASS( EffectsFader )

/* Initializer for the class 'Effects::VisibilityFader' */
void EffectsVisibilityFader__Init( EffectsVisibilityFader _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  EffectsFader__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsVisibilityFader );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsVisibilityFader );

  /* ... and initialize objects, variables, properties, etc. */
  _this->ShowAtStart = 1;
}

/* Re-Initializer for the class 'Effects::VisibilityFader' */
void EffectsVisibilityFader__ReInit( EffectsVisibilityFader _this )
{
  /* At first re-initialize the super class ... */
  EffectsFader__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::VisibilityFader' */
void EffectsVisibilityFader__Done( EffectsVisibilityFader _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( EffectsFader );

  /* Don't forget to deinitialize the super class ... */
  EffectsFader__Done( &_this->_Super );
}

/* The method IsFinished() should return 'true' if the fader has finalized its fading 
   animation and 'false' if the animation is not started or it is still in progress. 
   This method should therefore be overridden in derived classes to evaluate the 
   state of animation effects used inside the fader. */
XBool EffectsVisibilityFader_IsFinished( EffectsVisibilityFader _this )
{
  return _this->finished;
}

/* The method OnEnd() is invoked automatically just in the moment, when this fader 
   and all other faders belonging to the same fading task have signaled that they 
   are finished with their animations. The default implementation of this method 
   simply does nothing.
   Derived classes can override this method and implement there the code to complete 
   the animation (e.g. to remove the group from its owner after it has been faded-out, 
   etc.). The implementation of the overridden OnEnd() method has to use the variables 
   @Group and @Owner to determine on which group the fader has applied the animation. 
   Depending on the kind of the implemented animation, the OnEnd() method can also 
   evaluate the properties @UseCurrentState, @AssignFocus and @RestackTopmost. */
void EffectsVisibilityFader_OnEnd( EffectsVisibilityFader _this )
{
  if (( _this->Super1.Visible && !_this->ShowAtStart ) && !_this->Super1.UseCurrentState )
    CoreRectView__OnSetBounds( _this->Super1.Group, EwSetRectOrigin( _this->Super1.Group->Super1.Bounds, 
    _this->Position ));

  if ( _this->Super1.Visible && !_this->ShowAtStart )
  {
    CoreGroup__OnSetOpacity( _this->Super1.Group, 255 );
    CoreGroup_OnSetVisible( _this->Super1.Group, 1 );
  }

  if (( !_this->Super1.Visible && !_this->HideAtStart ) && ( _this->Super1.Group->Super2.Owner 
      != 0 ))
    CoreGroup_OnSetVisible( _this->Super1.Group, 0 );

  if (( !_this->Super1.Visible && _this->Super1.RemoveIfHidden ) && ( _this->Super1.Group->Super2.Owner 
      != 0 ))
    CoreGroup_Remove( _this->Super1.Group->Super2.Owner, ((CoreView)_this->Super1.Group ));
}

/* The method OnStart() is invoked automatically just in the moment, when the fader 
   is triggered to begin its animation. The default implementation of this method 
   simply calls the @Complete() method indicating so, that the fader is done with 
   its work. Thereupon the next fader waiting for its execution can start.
   Derived classes should override this method and implement there the code to prepare 
   and start the desired fade-in/out animation effects. The implementation of the 
   overridden OnStart() method has to use the variables @Group and @Owner to determine 
   on which group the fader should apply the animation. Depending on the kind of 
   the implemented animation, the OnStart() method can also evaluate the properties 
   @UseCurrentState, @AssignFocus and @RestackTopmost.
   Please note, if there is a slot method associated to the property @OnInitialize, 
   this slot method is signaled shortly before the method OnStart() is executed 
   given the slot method a possibility to apply more individual initialization steps 
   on the fader just before the animation begins.
   When the animation reaches its end (e.g. when the animation effects used inside 
   the fader do terminate), your implementation of the fader should invoke the method 
   @Complete() otherwise the animation is considered as still being in progress 
   causing other pending faders to wait. */
void EffectsVisibilityFader_OnStart( EffectsVisibilityFader _this )
{
  if (( _this->Super1.Visible || _this->Super1.AddToOwner ) && ( _this->Super1.Group->Super2.Owner 
      == 0 ))
  {
    CoreGroup_OnSetVisible( _this->Super1.Group, 0 );
    CoreGroup_Add( _this->Super1.Owner, ((CoreView)_this->Super1.Group ), 0 );
  }

  if ( _this->Super1.Visible && _this->Super1.RestackTopmost )
    CoreGroup_RestackTop( _this->Super1.Group->Super2.Owner, ((CoreView)_this->Super1.Group ));

  if ( _this->Super1.Visible && _this->Super1.AssignFocus )
    CoreGroup__OnSetFocus( _this->Super1.Group->Super2.Owner, ((CoreView)_this->Super1.Group ));

  if (( _this->Super1.Visible && _this->ShowAtStart ) && !_this->Super1.UseCurrentState )
    CoreRectView__OnSetBounds( _this->Super1.Group, EwSetRectOrigin( _this->Super1.Group->Super1.Bounds, 
    _this->Position ));

  if ( _this->Super1.Visible && _this->ShowAtStart )
  {
    CoreGroup__OnSetOpacity( _this->Super1.Group, 255 );
    CoreGroup_OnSetVisible( _this->Super1.Group, 1 );
  }

  if (( !_this->Super1.Visible && _this->HideAtStart ) && ( _this->Super1.Group->Super2.Owner 
      != 0 ))
    CoreGroup_OnSetVisible( _this->Super1.Group, 0 );

  if ( !_this->Super1.Visible && ( _this->Super1.Owner->Focus == (CoreView)_this->Super1.Group ))
    CoreGroup__OnSetFocus( _this->Super1.Owner, 0 );

  _this->finished = 1;
  EffectsFader_Complete((EffectsFader)_this );
}

/* Variants derived from the class : 'Effects::VisibilityFader' */
EW_DEFINE_CLASS_VARIANTS( EffectsVisibilityFader )
EW_END_OF_CLASS_VARIANTS( EffectsVisibilityFader )

/* Virtual Method Table (VMT) for the class : 'Effects::VisibilityFader' */
EW_DEFINE_CLASS( EffectsVisibilityFader, EffectsFader, _None, _None, _None, _None, 
                 _None, _None, "Effects::VisibilityFader" )
  EffectsVisibilityFader_IsFinished,
  EffectsVisibilityFader_OnEnd,
  EffectsVisibilityFader_OnStart,
EW_END_OF_CLASS( EffectsVisibilityFader )

/* Initializer for the class 'Effects::OpacityFader' */
void EffectsOpacityFader__Init( EffectsOpacityFader _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  EffectsFader__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsOpacityFader );

  /* ... then construct all embedded objects */
  EffectsInt32Effect__Init( &_this->Effect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsOpacityFader );
}

/* Re-Initializer for the class 'Effects::OpacityFader' */
void EffectsOpacityFader__ReInit( EffectsOpacityFader _this )
{
  /* At first re-initialize the super class ... */
  EffectsFader__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  EffectsInt32Effect__ReInit( &_this->Effect );
}

/* Finalizer method for the class 'Effects::OpacityFader' */
void EffectsOpacityFader__Done( EffectsOpacityFader _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( EffectsFader );

  /* Finalize all embedded objects */
  EffectsInt32Effect__Done( &_this->Effect );

  /* Don't forget to deinitialize the super class ... */
  EffectsFader__Done( &_this->_Super );
}

/* The method IsFinished() should return 'true' if the fader has finalized its fading 
   animation and 'false' if the animation is not started or it is still in progress. 
   This method should therefore be overridden in derived classes to evaluate the 
   state of animation effects used inside the fader. */
XBool EffectsOpacityFader_IsFinished( EffectsOpacityFader _this )
{
  return _this->finished;
}

/* The method OnEnd() is invoked automatically just in the moment, when this fader 
   and all other faders belonging to the same fading task have signaled that they 
   are finished with their animations. The default implementation of this method 
   simply does nothing.
   Derived classes can override this method and implement there the code to complete 
   the animation (e.g. to remove the group from its owner after it has been faded-out, 
   etc.). The implementation of the overridden OnEnd() method has to use the variables 
   @Group and @Owner to determine on which group the fader has applied the animation. 
   Depending on the kind of the implemented animation, the OnEnd() method can also 
   evaluate the properties @UseCurrentState, @AssignFocus and @RestackTopmost. */
void EffectsOpacityFader_OnEnd( EffectsOpacityFader _this )
{
  if ( _this->Super1.Visible )
    CoreGroup__OnSetOpacity( _this->Super1.Group, _this->Effect.Value2 );

  if ( !_this->Super1.Visible && ( _this->Super1.Group->Opacity == 0 ))
    CoreGroup_OnSetVisible( _this->Super1.Group, 0 );

  if (( !_this->Super1.Visible && _this->Super1.RemoveIfHidden ) && ( _this->Super1.Group->Super2.Owner 
      != 0 ))
    CoreGroup_Remove( _this->Super1.Group->Super2.Owner, ((CoreView)_this->Super1.Group ));

  if ( _this->Buffered )
    CoreGroup__OnSetBuffered( _this->Super1.Group, _this->wasBuffered );
}

/* The method OnStart() is invoked automatically just in the moment, when the fader 
   is triggered to begin its animation. The default implementation of this method 
   simply calls the @Complete() method indicating so, that the fader is done with 
   its work. Thereupon the next fader waiting for its execution can start.
   Derived classes should override this method and implement there the code to prepare 
   and start the desired fade-in/out animation effects. The implementation of the 
   overridden OnStart() method has to use the variables @Group and @Owner to determine 
   on which group the fader should apply the animation. Depending on the kind of 
   the implemented animation, the OnStart() method can also evaluate the properties 
   @UseCurrentState, @AssignFocus and @RestackTopmost.
   Please note, if there is a slot method associated to the property @OnInitialize, 
   this slot method is signaled shortly before the method OnStart() is executed 
   given the slot method a possibility to apply more individual initialization steps 
   on the fader just before the animation begins.
   When the animation reaches its end (e.g. when the animation effects used inside 
   the fader do terminate), your implementation of the fader should invoke the method 
   @Complete() otherwise the animation is considered as still being in progress 
   causing other pending faders to wait. */
void EffectsOpacityFader_OnStart( EffectsOpacityFader _this )
{
  XBool isHidden;
  XBool isHidden2;

  if ( _this->Super1.UseCurrentState )
  {
    if ( CoreGroup_OnGetVisible( _this->Super1.Group ) && ( _this->Super1.Group->Super2.Owner 
        != 0 ))
      _this->Effect.Value1 = _this->Super1.Group->Opacity;
    else
      _this->Effect.Value1 = 0;
  }

  isHidden = (XBool)(((( _this->Super1.Group->Super2.Owner == 0 ) || ( _this->Super1.Group->Opacity 
  == 0 )) || !CoreGroup_OnGetVisible( _this->Super1.Group )) || EwIsRectEmpty( EwIntersectRect( 
  _this->Super1.Group->Super1.Bounds, EwGetRectORect( _this->Super1.Owner->Super1.Bounds ))));
  isHidden2 = (XBool)((( !_this->Super1.Visible && ((( _this->Super1.Group->Super2.Owner 
  == 0 ) || !CoreGroup_OnGetVisible( _this->Super1.Group )) || _this->Super1.RemoveIfHidden )) 
  || ( _this->Effect.Value2 == 0 )) || EwIsRectEmpty( EwIntersectRect( _this->Super1.Group->Super1.Bounds, 
  EwGetRectORect( _this->Super1.Owner->Super1.Bounds ))));

  if ( isHidden && isHidden2 )
  {
    _this->Effect.Value1 = 0;
    _this->Effect.Value2 = 0;
  }

  _this->wasBuffered = CoreGroup_OnGetBuffered( _this->Super1.Group );

  if (( _this->Super1.Visible || _this->Super1.AddToOwner ) && ( _this->Super1.Group->Super2.Owner 
      == 0 ))
  {
    CoreGroup_OnSetVisible( _this->Super1.Group, 0 );
    CoreGroup_Add( _this->Super1.Owner, ((CoreView)_this->Super1.Group ), 0 );
  }

  if ( _this->Super1.Visible && _this->Super1.RestackTopmost )
    CoreGroup_RestackTop( _this->Super1.Group->Super2.Owner, ((CoreView)_this->Super1.Group ));

  if ( _this->Super1.Visible && _this->Super1.AssignFocus )
    CoreGroup__OnSetFocus( _this->Super1.Group->Super2.Owner, ((CoreView)_this->Super1.Group ));

  if ( !_this->Super1.Visible && ( _this->Super1.Owner->Focus == (CoreView)_this->Super1.Group ))
    CoreGroup__OnSetFocus( _this->Super1.Owner, 0 );

  if ( _this->Super1.Visible && !CoreGroup_OnGetVisible( _this->Super1.Group ))
  {
    CoreGroup__OnSetOpacity( _this->Super1.Group, _this->Effect.Value1 );
    CoreGroup_OnSetVisible( _this->Super1.Group, 1 );
  }

  if ( !_this->Super1.Visible && ((( _this->Super1.Group->Super2.Owner == 0 ) || 
      !CoreGroup_OnGetVisible( _this->Super1.Group )) || ( _this->Super1.Group->Opacity 
      == 0 )))
  {
    _this->finished = 1;
    EffectsFader_Complete((EffectsFader)_this );
    return;
  }

  if ( !_this->Super1.Visible && ( _this->Effect.Value1 == _this->Effect.Value2 ))
    CoreGroup__OnSetOpacity( _this->Super1.Group, _this->Effect.Value1 );

  if ( _this->Super1.Group->Opacity == _this->Effect.Value2 )
  {
    _this->finished = 1;
    EffectsFader_Complete((EffectsFader)_this );
    return;
  }

  if ( _this->Effect.Value1 == _this->Effect.Value2 )
  {
    _this->finished = 1;
    EffectsFader_Complete((EffectsFader)_this );
    return;
  }

  if ( _this->Effect.Super1.NoOfCycles == 0 )
    EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->Effect, 1 );

  if ( _this->Buffered )
    CoreGroup__OnSetBuffered( _this->Super1.Group, 1 );

  _this->Effect.Outlet = EwNewRef( _this->Super1.Group, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  EffectsEffect_OnSetReversed((EffectsEffect)&_this->Effect, 0 );
  _this->Effect.Super1.Symmetric = 0;
  _this->Effect.Super1.OnFinished = EwNewSlot( _this, EffectsOpacityFader_onFinished );
  _this->Effect.Super1.OnAnimate = EwNullSlot;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->Effect, 1 );
}

/* 'C' function for method : 'Effects::OpacityFader.onFinished()' */
void EffectsOpacityFader_onFinished( EffectsOpacityFader _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  _this->finished = 1;
  EffectsFader_Complete((EffectsFader)_this );
}

/* Variants derived from the class : 'Effects::OpacityFader' */
EW_DEFINE_CLASS_VARIANTS( EffectsOpacityFader )
EW_END_OF_CLASS_VARIANTS( EffectsOpacityFader )

/* Virtual Method Table (VMT) for the class : 'Effects::OpacityFader' */
EW_DEFINE_CLASS( EffectsOpacityFader, EffectsFader, Effect, Effect, Effect, Effect, 
                 Buffered, Buffered, "Effects::OpacityFader" )
  EffectsOpacityFader_IsFinished,
  EffectsOpacityFader_OnEnd,
  EffectsOpacityFader_OnStart,
EW_END_OF_CLASS( EffectsOpacityFader )

/* Initializer for the class 'Effects::PositionFader' */
void EffectsPositionFader__Init( EffectsPositionFader _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  EffectsFader__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsPositionFader );

  /* ... then construct all embedded objects */
  EffectsInt32Effect__Init( &_this->OpacityEffect, &_this->_XObject, 0 );
  EffectsPointEffect__Init( &_this->PositionEffect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsPositionFader );
}

/* Re-Initializer for the class 'Effects::PositionFader' */
void EffectsPositionFader__ReInit( EffectsPositionFader _this )
{
  /* At first re-initialize the super class ... */
  EffectsFader__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  EffectsInt32Effect__ReInit( &_this->OpacityEffect );
  EffectsPointEffect__ReInit( &_this->PositionEffect );
}

/* Finalizer method for the class 'Effects::PositionFader' */
void EffectsPositionFader__Done( EffectsPositionFader _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( EffectsFader );

  /* Finalize all embedded objects */
  EffectsInt32Effect__Done( &_this->OpacityEffect );
  EffectsPointEffect__Done( &_this->PositionEffect );

  /* Don't forget to deinitialize the super class ... */
  EffectsFader__Done( &_this->_Super );
}

/* The method IsFinished() should return 'true' if the fader has finalized its fading 
   animation and 'false' if the animation is not started or it is still in progress. 
   This method should therefore be overridden in derived classes to evaluate the 
   state of animation effects used inside the fader. */
XBool EffectsPositionFader_IsFinished( EffectsPositionFader _this )
{
  return _this->finished;
}

/* The method OnEnd() is invoked automatically just in the moment, when this fader 
   and all other faders belonging to the same fading task have signaled that they 
   are finished with their animations. The default implementation of this method 
   simply does nothing.
   Derived classes can override this method and implement there the code to complete 
   the animation (e.g. to remove the group from its owner after it has been faded-out, 
   etc.). The implementation of the overridden OnEnd() method has to use the variables 
   @Group and @Owner to determine on which group the fader has applied the animation. 
   Depending on the kind of the implemented animation, the OnEnd() method can also 
   evaluate the properties @UseCurrentState, @AssignFocus and @RestackTopmost. */
void EffectsPositionFader_OnEnd( EffectsPositionFader _this )
{
  if ( _this->Super1.Visible )
  {
    CoreGroup__OnSetOpacity( _this->Super1.Group, _this->OpacityEffect.Value2 );
    CoreRectView__OnSetBounds( _this->Super1.Group, EwSetRectOrigin( _this->Super1.Group->Super1.Bounds, 
    _this->PositionEffect.Value2 ));
  }

  if ( !_this->Super1.Visible && ( _this->Super1.Group->Opacity == 0 ))
    CoreGroup_OnSetVisible( _this->Super1.Group, 0 );

  if (( !_this->Super1.Visible && _this->Super1.RemoveIfHidden ) && ( _this->Super1.Group->Super2.Owner 
      != 0 ))
    CoreGroup_Remove( _this->Super1.Group->Super2.Owner, ((CoreView)_this->Super1.Group ));

  if ( _this->Buffered )
    CoreGroup__OnSetBuffered( _this->Super1.Group, _this->wasBuffered );
}

/* The method OnStart() is invoked automatically just in the moment, when the fader 
   is triggered to begin its animation. The default implementation of this method 
   simply calls the @Complete() method indicating so, that the fader is done with 
   its work. Thereupon the next fader waiting for its execution can start.
   Derived classes should override this method and implement there the code to prepare 
   and start the desired fade-in/out animation effects. The implementation of the 
   overridden OnStart() method has to use the variables @Group and @Owner to determine 
   on which group the fader should apply the animation. Depending on the kind of 
   the implemented animation, the OnStart() method can also evaluate the properties 
   @UseCurrentState, @AssignFocus and @RestackTopmost.
   Please note, if there is a slot method associated to the property @OnInitialize, 
   this slot method is signaled shortly before the method OnStart() is executed 
   given the slot method a possibility to apply more individual initialization steps 
   on the fader just before the animation begins.
   When the animation reaches its end (e.g. when the animation effects used inside 
   the fader do terminate), your implementation of the fader should invoke the method 
   @Complete() otherwise the animation is considered as still being in progress 
   causing other pending faders to wait. */
void EffectsPositionFader_OnStart( EffectsPositionFader _this )
{
  XBool isHidden;
  XBool isHidden2;

  if ( _this->Super1.UseCurrentState )
  {
    if ( CoreGroup_OnGetVisible( _this->Super1.Group ) && ( _this->Super1.Group->Super2.Owner 
        != 0 ))
      _this->OpacityEffect.Value1 = _this->Super1.Group->Opacity;
    else
      _this->OpacityEffect.Value1 = 0;

    _this->PositionEffect.Value1 = _this->Super1.Group->Super1.Bounds.Point1;
  }

  isHidden = (XBool)(((( _this->Super1.Group->Super2.Owner == 0 ) || ( _this->Super1.Group->Opacity 
  == 0 )) || !CoreGroup_OnGetVisible( _this->Super1.Group )) || EwIsRectEmpty( EwIntersectRect( 
  _this->Super1.Group->Super1.Bounds, EwGetRectORect( _this->Super1.Owner->Super1.Bounds ))));
  isHidden2 = (XBool)((( !_this->Super1.Visible && ((( _this->Super1.Group->Super2.Owner 
  == 0 ) || !CoreGroup_OnGetVisible( _this->Super1.Group )) || _this->Super1.RemoveIfHidden )) 
  || ( _this->OpacityEffect.Value2 == 0 )) || EwIsRectEmpty( EwIntersectRect( EwMoveRectPos( 
  EwGetRectORect( _this->Super1.Group->Super1.Bounds ), _this->PositionEffect.Value2 ), 
  EwGetRectORect( _this->Super1.Owner->Super1.Bounds ))));

  if ( isHidden && isHidden2 )
  {
    _this->OpacityEffect.Value1 = 0;
    _this->OpacityEffect.Value2 = 0;
    _this->PositionEffect.Value1 = _this->PositionEffect.Value2;
  }

  _this->wasBuffered = CoreGroup_OnGetBuffered( _this->Super1.Group );

  if (( _this->Super1.Visible || _this->Super1.AddToOwner ) && ( _this->Super1.Group->Super2.Owner 
      == 0 ))
  {
    CoreGroup_OnSetVisible( _this->Super1.Group, 0 );
    CoreGroup_Add( _this->Super1.Owner, ((CoreView)_this->Super1.Group ), 0 );
  }

  if ( _this->Super1.Visible && _this->Super1.RestackTopmost )
    CoreGroup_RestackTop( _this->Super1.Group->Super2.Owner, ((CoreView)_this->Super1.Group ));

  if ( _this->Super1.Visible && _this->Super1.AssignFocus )
    CoreGroup__OnSetFocus( _this->Super1.Group->Super2.Owner, ((CoreView)_this->Super1.Group ));

  if ( !_this->Super1.Visible && ( _this->Super1.Owner->Focus == (CoreView)_this->Super1.Group ))
    CoreGroup__OnSetFocus( _this->Super1.Owner, 0 );

  if ( _this->Super1.Visible && !CoreGroup_OnGetVisible( _this->Super1.Group ))
  {
    CoreRectView__OnSetBounds( _this->Super1.Group, EwSetRectOrigin( _this->Super1.Group->Super1.Bounds, 
    _this->PositionEffect.Value1 ));
    CoreGroup__OnSetOpacity( _this->Super1.Group, _this->OpacityEffect.Value1 );
    CoreGroup_OnSetVisible( _this->Super1.Group, 1 );
  }

  if ( !_this->Super1.Visible && ((( _this->Super1.Group->Super2.Owner == 0 ) || 
      !CoreGroup_OnGetVisible( _this->Super1.Group )) || ( _this->Super1.Group->Opacity 
      == 0 )))
  {
    _this->finished = 1;
    EffectsFader_Complete((EffectsFader)_this );
    return;
  }

  if ( !_this->Super1.Visible && !EwCompPoint( _this->PositionEffect.Value1, _this->PositionEffect.Value2 ))
    CoreRectView__OnSetBounds( _this->Super1.Group, EwSetRectOrigin( _this->Super1.Group->Super1.Bounds, 
    _this->PositionEffect.Value1 ));

  if ( !_this->Super1.Visible && ( _this->OpacityEffect.Value1 == _this->OpacityEffect.Value2 ))
    CoreGroup__OnSetOpacity( _this->Super1.Group, _this->OpacityEffect.Value1 );

  if (( _this->Super1.Group->Opacity == _this->OpacityEffect.Value2 ) && !EwCompPoint( 
      _this->Super1.Group->Super1.Bounds.Point1, _this->PositionEffect.Value2 ))
  {
    _this->finished = 1;
    EffectsFader_Complete((EffectsFader)_this );
    return;
  }

  if (( _this->OpacityEffect.Value1 == _this->OpacityEffect.Value2 ) && !EwCompPoint( 
      _this->PositionEffect.Value1, _this->PositionEffect.Value2 ))
  {
    _this->finished = 1;
    EffectsFader_Complete((EffectsFader)_this );
    return;
  }

  if ( _this->OpacityEffect.Super1.NoOfCycles == 0 )
    EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->OpacityEffect, 1 );

  if ( _this->PositionEffect.Super1.NoOfCycles == 0 )
    EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->PositionEffect, 1 );

  if ( _this->Buffered )
    CoreGroup__OnSetBuffered( _this->Super1.Group, 1 );

  _this->PositionEffect.Outlet = EwNullRef;
  EffectsEffect_OnSetReversed((EffectsEffect)&_this->PositionEffect, 0 );
  _this->PositionEffect.Super1.Symmetric = 0;
  _this->PositionEffect.Super1.OnFinished = EwNewSlot( _this, EffectsPositionFader_onFinished );
  _this->PositionEffect.Super1.OnAnimate = EwNewSlot( _this, EffectsPositionFader_onAnimate );
  _this->OpacityEffect.Outlet = EwNewRef( _this->Super1.Group, CoreGroup_OnGetOpacity, 
  CoreGroup__OnSetOpacity );
  EffectsEffect_OnSetReversed((EffectsEffect)&_this->OpacityEffect, 0 );
  _this->OpacityEffect.Super1.Symmetric = 0;
  _this->OpacityEffect.Super1.OnFinished = EwNewSlot( _this, EffectsPositionFader_onFinished );
  _this->OpacityEffect.Super1.OnAnimate = EwNullSlot;
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->PositionEffect, (XBool)EwCompPoint( 
  _this->PositionEffect.Value1, _this->PositionEffect.Value2 ));
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->OpacityEffect, (XBool)( _this->OpacityEffect.Value1 
  != _this->OpacityEffect.Value2 ));
}

/* 'C' function for method : 'Effects::PositionFader.onFinished()' */
void EffectsPositionFader_onFinished( EffectsPositionFader _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  _this->finished = (XBool)( !_this->OpacityEffect.Super1.Enabled && !_this->PositionEffect.Super1.Enabled );
  EffectsFader_Complete((EffectsFader)_this );
}

/* 'C' function for method : 'Effects::PositionFader.onAnimate()' */
void EffectsPositionFader_onAnimate( EffectsPositionFader _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreRectView__OnSetBounds( _this->Super1.Group, EwSetRectOrigin( _this->Super1.Group->Super1.Bounds, 
  _this->PositionEffect.Value ));
}

/* Variants derived from the class : 'Effects::PositionFader' */
EW_DEFINE_CLASS_VARIANTS( EffectsPositionFader )
EW_END_OF_CLASS_VARIANTS( EffectsPositionFader )

/* Virtual Method Table (VMT) for the class : 'Effects::PositionFader' */
EW_DEFINE_CLASS( EffectsPositionFader, EffectsFader, OpacityEffect, OpacityEffect, 
                 OpacityEffect, OpacityEffect, Buffered, Buffered, "Effects::PositionFader" )
  EffectsPositionFader_IsFinished,
  EffectsPositionFader_OnEnd,
  EffectsPositionFader_OnStart,
EW_END_OF_CLASS( EffectsPositionFader )

/* Initializer for the class 'Effects::Transition' */
void EffectsTransition__Init( EffectsTransition _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsTransition );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsTransition );
}

/* Re-Initializer for the class 'Effects::Transition' */
void EffectsTransition__ReInit( EffectsTransition _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::Transition' */
void EffectsTransition__Done( EffectsTransition _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* The method CreatePresentFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (present) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to present) a GUI component 
   when the user navigates to it. The method has to be overridden and implemented 
   in derived classes. The actual implementation does nothing and returns 'null'. */
EffectsFader EffectsTransition_CreatePresentFader( EffectsTransition _this )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );

  return 0;
}

/* Wrapper function for the virtual method : 'Effects::Transition.CreatePresentFader()' */
EffectsFader EffectsTransition__CreatePresentFader( void* _this )
{
  return ((EffectsTransition)_this)->_VMT->CreatePresentFader((EffectsTransition)_this );
}

/* The method CreateDismissFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (dismiss) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to hide (to dismiss) a GUI 
   component when the user leaves it while he/she navigates in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   does nothing and returns 'null'. */
EffectsFader EffectsTransition_CreateDismissFader( EffectsTransition _this )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );

  return 0;
}

/* Wrapper function for the virtual method : 'Effects::Transition.CreateDismissFader()' */
EffectsFader EffectsTransition__CreateDismissFader( void* _this )
{
  return ((EffectsTransition)_this)->_VMT->CreateDismissFader((EffectsTransition)_this );
}

/* The method CreateRestoreFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (restore) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to restore the original 
   visible state) of a GUI component, which has been obscured (overlaid) by other 
   GUI components during the user navigates in the GUI system. The method has to 
   be overridden and implemented in derived classes. The actual implementation of 
   this method redirects the operation to the method @CreatePresentFader(). */
EffectsFader EffectsTransition_CreateRestoreFader( EffectsTransition _this )
{
  return EffectsTransition__CreatePresentFader( _this );
}

/* Wrapper function for the virtual method : 'Effects::Transition.CreateRestoreFader()' */
EffectsFader EffectsTransition__CreateRestoreFader( void* _this )
{
  return ((EffectsTransition)_this)->_VMT->CreateRestoreFader((EffectsTransition)_this );
}

/* The method CreateOverlayFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (overlay) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to temporarily hide (to overlay) 
   a GUI component when the user navigates a deeper level in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   of this method redirects the operation to the method @CreateDismissFader(). */
EffectsFader EffectsTransition_CreateOverlayFader( EffectsTransition _this )
{
  return EffectsTransition__CreateDismissFader( _this );
}

/* Wrapper function for the virtual method : 'Effects::Transition.CreateOverlayFader()' */
EffectsFader EffectsTransition__CreateOverlayFader( void* _this )
{
  return ((EffectsTransition)_this)->_VMT->CreateOverlayFader((EffectsTransition)_this );
}

/* Variants derived from the class : 'Effects::Transition' */
EW_DEFINE_CLASS_VARIANTS( EffectsTransition )
EW_END_OF_CLASS_VARIANTS( EffectsTransition )

/* Virtual Method Table (VMT) for the class : 'Effects::Transition' */
EW_DEFINE_CLASS( EffectsTransition, XObject, _None, _None, _None, _None, _None, 
                 _None, "Effects::Transition" )
  EffectsTransition_CreatePresentFader,
  EffectsTransition_CreateDismissFader,
  EffectsTransition_CreateRestoreFader,
  EffectsTransition_CreateOverlayFader,
EW_END_OF_CLASS( EffectsTransition )

/* Initializer for the class 'Effects::ShowHideTransition' */
void EffectsShowHideTransition__Init( EffectsShowHideTransition _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  EffectsTransition__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsShowHideTransition );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsShowHideTransition );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Alignment = EffectsDialogAlignmentAlignHorzCenter | EffectsDialogAlignmentAlignVertCenter;
  _this->ShowAtStart = 1;
}

/* Re-Initializer for the class 'Effects::ShowHideTransition' */
void EffectsShowHideTransition__ReInit( EffectsShowHideTransition _this )
{
  /* At first re-initialize the super class ... */
  EffectsTransition__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::ShowHideTransition' */
void EffectsShowHideTransition__Done( EffectsShowHideTransition _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( EffectsTransition );

  /* Don't forget to deinitialize the super class ... */
  EffectsTransition__Done( &_this->_Super );
}

/* The method CreatePresentFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (present) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to present) a GUI component 
   when the user navigates to it. The method has to be overridden and implemented 
   in derived classes. The actual implementation does nothing and returns 'null'. */
EffectsFader EffectsShowHideTransition_CreatePresentFader( EffectsShowHideTransition _this )
{
  EffectsVisibilityFader fader = EwNewObject( EffectsVisibilityFader, 0 );

  fader->Super1.Visible = 1;
  fader->Super1.AssignFocus = 0;
  fader->Super1.RestackTopmost = 1;
  fader->Super1.UseCurrentState = 0;
  fader->ShowAtStart = _this->ShowAtStart;
  fader->Super1.OnInitialize = EwNewSlot( _this, EffectsShowHideTransition_onInitializeIn );
  return ((EffectsFader)fader );
}

/* The method CreateDismissFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (dismiss) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to hide (to dismiss) a GUI 
   component when the user leaves it while he/she navigates in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   does nothing and returns 'null'. */
EffectsFader EffectsShowHideTransition_CreateDismissFader( EffectsShowHideTransition _this )
{
  EffectsVisibilityFader fader = EwNewObject( EffectsVisibilityFader, 0 );

  fader->Super1.Visible = 0;
  fader->HideAtStart = _this->HideAtStart;
  fader->Super1.RemoveIfHidden = 1;
  return ((EffectsFader)fader );
}

/* The method CreateRestoreFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (restore) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to restore the original 
   visible state) of a GUI component, which has been obscured (overlaid) by other 
   GUI components during the user navigates in the GUI system. The method has to 
   be overridden and implemented in derived classes. The actual implementation of 
   this method redirects the operation to the method @CreatePresentFader(). */
EffectsFader EffectsShowHideTransition_CreateRestoreFader( EffectsShowHideTransition _this )
{
  EffectsFader fader = EffectsTransition_CreateRestoreFader((EffectsTransition)_this );

  fader->RestackTopmost = 0;
  return fader;
}

/* The method CreateOverlayFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (overlay) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to temporarily hide (to overlay) 
   a GUI component when the user navigates a deeper level in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   of this method redirects the operation to the method @CreateDismissFader(). */
EffectsFader EffectsShowHideTransition_CreateOverlayFader( EffectsShowHideTransition _this )
{
  EffectsVisibilityFader fader = EwNewObject( EffectsVisibilityFader, 0 );

  fader->Super1.Visible = 0;
  fader->Super1.AddToOwner = 1;
  fader->Super1.RemoveIfHidden = 0;
  fader->HideAtStart = _this->HideAtStart;
  return ((EffectsFader)fader );
}

/* 'C' function for method : 'Effects::ShowHideTransition.onInitializeIn()' */
void EffectsShowHideTransition_onInitializeIn( EffectsShowHideTransition _this, 
  XObject sender )
{
  EffectsVisibilityFader fader = EwCastObject( sender, EffectsVisibilityFader );
  XSet align = _this->Alignment;
  XRect bounds = EwGetRectORect( fader->Super1.Owner->Super1.Bounds );
  XPoint size = EwGetRectSize( fader->Super1.Group->Super1.Bounds );
  XPoint pos;

  bounds.Point1.X = ( bounds.Point1.X + _this->MarginLeft );
  bounds.Point2.X = ( bounds.Point2.X - _this->MarginRight );
  bounds.Point1.Y = ( bounds.Point1.Y + _this->MarginTop );
  bounds.Point2.Y = ( bounds.Point2.Y - _this->MarginBottom );
  pos = bounds.Point1;

  if ((( align & EffectsDialogAlignmentAlignHorzRight ) == EffectsDialogAlignmentAlignHorzRight ))
    pos.X = ( bounds.Point2.X - size.X );
  else
    if ((( align & EffectsDialogAlignmentAlignHorzCenter ) == EffectsDialogAlignmentAlignHorzCenter ))
      pos.X = (( bounds.Point1.X + ( EwGetRectW( bounds ) / 2 )) - ( size.X / 2 ));

  if ((( align & EffectsDialogAlignmentAlignVertBottom ) == EffectsDialogAlignmentAlignVertBottom ))
    pos.Y = ( bounds.Point2.Y - size.Y );
  else
    if ((( align & EffectsDialogAlignmentAlignVertCenter ) == EffectsDialogAlignmentAlignVertCenter ))
      pos.Y = (( bounds.Point1.Y + ( EwGetRectH( bounds ) / 2 )) - ( size.Y / 2 ));

  fader->Position = pos;
}

/* Variants derived from the class : 'Effects::ShowHideTransition' */
EW_DEFINE_CLASS_VARIANTS( EffectsShowHideTransition )
EW_END_OF_CLASS_VARIANTS( EffectsShowHideTransition )

/* Virtual Method Table (VMT) for the class : 'Effects::ShowHideTransition' */
EW_DEFINE_CLASS( EffectsShowHideTransition, EffectsTransition, _None, _None, _None, 
                 _None, _None, _None, "Effects::ShowHideTransition" )
  EffectsShowHideTransition_CreatePresentFader,
  EffectsShowHideTransition_CreateDismissFader,
  EffectsShowHideTransition_CreateRestoreFader,
  EffectsShowHideTransition_CreateOverlayFader,
EW_END_OF_CLASS( EffectsShowHideTransition )

/* Initializer for the class 'Effects::FadeInOutTransition' */
void EffectsFadeInOutTransition__Init( EffectsFadeInOutTransition _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  EffectsTransition__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsFadeInOutTransition );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsFadeInOutTransition );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Alignment = EffectsDialogAlignmentAlignHorzCenter | EffectsDialogAlignmentAlignVertCenter;
  _this->Duration = 500;
}

/* Re-Initializer for the class 'Effects::FadeInOutTransition' */
void EffectsFadeInOutTransition__ReInit( EffectsFadeInOutTransition _this )
{
  /* At first re-initialize the super class ... */
  EffectsTransition__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::FadeInOutTransition' */
void EffectsFadeInOutTransition__Done( EffectsFadeInOutTransition _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( EffectsTransition );

  /* Don't forget to deinitialize the super class ... */
  EffectsTransition__Done( &_this->_Super );
}

/* The method CreatePresentFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (present) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to present) a GUI component 
   when the user navigates to it. The method has to be overridden and implemented 
   in derived classes. The actual implementation does nothing and returns 'null'. */
EffectsFader EffectsFadeInOutTransition_CreatePresentFader( EffectsFadeInOutTransition _this )
{
  EffectsPositionFader fader = EwNewObject( EffectsPositionFader, 0 );

  fader->Super1.Visible = 1;
  fader->Super1.AssignFocus = 0;
  fader->Super1.RestackTopmost = 1;
  fader->Super1.UseCurrentState = 1;
  fader->Buffered = _this->Buffered;
  fader->Super1.OnInitialize = EwNewSlot( _this, EffectsFadeInOutTransition_onInitializeIn );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&fader->OpacityEffect, _this->Duration );
  fader->OpacityEffect.Value1 = 0;
  fader->OpacityEffect.Value2 = 255;
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&fader->PositionEffect, _this->Duration );
  EffectsEffect_OnSetTiming((EffectsEffect)&fader->PositionEffect, EffectsTimingFastIn_EaseOut );
  return ((EffectsFader)fader );
}

/* The method CreateDismissFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (dismiss) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to hide (to dismiss) a GUI 
   component when the user leaves it while he/she navigates in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   does nothing and returns 'null'. */
EffectsFader EffectsFadeInOutTransition_CreateDismissFader( EffectsFadeInOutTransition _this )
{
  EffectsOpacityFader fader = EwNewObject( EffectsOpacityFader, 0 );

  fader->Super1.Visible = 0;
  fader->Super1.RemoveIfHidden = 1;
  fader->Super1.UseCurrentState = 1;
  fader->Buffered = _this->Buffered;
  fader->Effect.Value1 = 255;
  fader->Effect.Value2 = 0;
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&fader->Effect, _this->Duration );
  return ((EffectsFader)fader );
}

/* The method CreateRestoreFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (restore) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to restore the original 
   visible state) of a GUI component, which has been obscured (overlaid) by other 
   GUI components during the user navigates in the GUI system. The method has to 
   be overridden and implemented in derived classes. The actual implementation of 
   this method redirects the operation to the method @CreatePresentFader(). */
EffectsFader EffectsFadeInOutTransition_CreateRestoreFader( EffectsFadeInOutTransition _this )
{
  EffectsFader fader = EffectsTransition_CreateRestoreFader((EffectsTransition)_this );

  fader->RestackTopmost = 0;
  return fader;
}

/* The method CreateOverlayFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (overlay) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to temporarily hide (to overlay) 
   a GUI component when the user navigates a deeper level in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   of this method redirects the operation to the method @CreateDismissFader(). */
EffectsFader EffectsFadeInOutTransition_CreateOverlayFader( EffectsFadeInOutTransition _this )
{
  EffectsFader fader = EffectsTransition_CreateOverlayFader((EffectsTransition)_this );

  fader->AddToOwner = 1;
  fader->RemoveIfHidden = 0;
  return fader;
}

/* 'C' function for method : 'Effects::FadeInOutTransition.onInitializeIn()' */
void EffectsFadeInOutTransition_onInitializeIn( EffectsFadeInOutTransition _this, 
  XObject sender )
{
  EffectsPositionFader fader = EwCastObject( sender, EffectsPositionFader );
  XSet align = _this->Alignment;
  XRect bounds = EwGetRectORect( fader->Super1.Owner->Super1.Bounds );
  XPoint size = EwGetRectSize( fader->Super1.Group->Super1.Bounds );
  XPoint pos;

  bounds.Point1.X = ( bounds.Point1.X + _this->MarginLeft );
  bounds.Point2.X = ( bounds.Point2.X - _this->MarginRight );
  bounds.Point1.Y = ( bounds.Point1.Y + _this->MarginTop );
  bounds.Point2.Y = ( bounds.Point2.Y - _this->MarginBottom );
  pos = bounds.Point1;

  if ((( align & EffectsDialogAlignmentAlignHorzRight ) == EffectsDialogAlignmentAlignHorzRight ))
    pos.X = ( bounds.Point2.X - size.X );
  else
    if ((( align & EffectsDialogAlignmentAlignHorzCenter ) == EffectsDialogAlignmentAlignHorzCenter ))
      pos.X = (( bounds.Point1.X + ( EwGetRectW( bounds ) / 2 )) - ( size.X / 2 ));

  if ((( align & EffectsDialogAlignmentAlignVertBottom ) == EffectsDialogAlignmentAlignVertBottom ))
    pos.Y = ( bounds.Point2.Y - size.Y );
  else
    if ((( align & EffectsDialogAlignmentAlignVertCenter ) == EffectsDialogAlignmentAlignVertCenter ))
      pos.Y = (( bounds.Point1.Y + ( EwGetRectH( bounds ) / 2 )) - ( size.Y / 2 ));

  fader->PositionEffect.Value2 = pos;

  if (((( fader->Super1.Group->Super2.Owner == 0 ) || !CoreGroup_OnGetVisible( fader->Super1.Group )) 
      || ( fader->Super1.Group->Opacity == 0 )) || EwIsRectEmpty( EwIntersectRect( 
      fader->Super1.Group->Super1.Bounds, EwGetRectORect( fader->Super1.Owner->Super1.Bounds ))))
    CoreRectView__OnSetBounds( fader->Super1.Group, EwSetRectOrigin( fader->Super1.Group->Super1.Bounds, 
    pos ));
}

/* Variants derived from the class : 'Effects::FadeInOutTransition' */
EW_DEFINE_CLASS_VARIANTS( EffectsFadeInOutTransition )
EW_END_OF_CLASS_VARIANTS( EffectsFadeInOutTransition )

/* Virtual Method Table (VMT) for the class : 'Effects::FadeInOutTransition' */
EW_DEFINE_CLASS( EffectsFadeInOutTransition, EffectsTransition, _None, _None, _None, 
                 _None, _None, _None, "Effects::FadeInOutTransition" )
  EffectsFadeInOutTransition_CreatePresentFader,
  EffectsFadeInOutTransition_CreateDismissFader,
  EffectsFadeInOutTransition_CreateRestoreFader,
  EffectsFadeInOutTransition_CreateOverlayFader,
EW_END_OF_CLASS( EffectsFadeInOutTransition )

/* Initializer for the class 'Effects::EffectTimerClass' */
void EffectsEffectTimerClass__Init( EffectsEffectTimerClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreTimer__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsEffectTimerClass );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsEffectTimerClass );
}

/* Re-Initializer for the class 'Effects::EffectTimerClass' */
void EffectsEffectTimerClass__ReInit( EffectsEffectTimerClass _this )
{
  /* At first re-initialize the super class ... */
  CoreTimer__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::EffectTimerClass' */
void EffectsEffectTimerClass__Done( EffectsEffectTimerClass _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreTimer );

  /* Don't forget to deinitialize the super class ... */
  CoreTimer__Done( &_this->_Super );
}

/* The method Trigger() will be invoked when the timer is expired (when the interval 
   defined in @Begin or @Period is elapsed). The method can be overridden and implemented 
   in derived classes. The default implementation of this method sends a signal 
   to the slot method stored in the @OnTrigger property. */
void EffectsEffectTimerClass_Trigger( EffectsEffectTimerClass _this )
{
  CoreTimer_Trigger((CoreTimer)_this );
  EwNotifyObjObservers((XObject)_this, 0 );
}

/* Variants derived from the class : 'Effects::EffectTimerClass' */
EW_DEFINE_CLASS_VARIANTS( EffectsEffectTimerClass )
EW_END_OF_CLASS_VARIANTS( EffectsEffectTimerClass )

/* Virtual Method Table (VMT) for the class : 'Effects::EffectTimerClass' */
EW_DEFINE_CLASS( EffectsEffectTimerClass, CoreTimer, _None, _None, _None, _None, 
                 _None, _None, "Effects::EffectTimerClass" )
  EffectsEffectTimerClass_Trigger,
EW_END_OF_CLASS( EffectsEffectTimerClass )

/* The global autoobject Effects::EffectTimer triggers all actually active animation 
   effects. In this way all effects will run simultanously. Per default the timer 
   is configured with 15 ms period (~60 FPS). By overriding the Effects::EffectTimer 
   autoobject by a variant you can specify another resolution to drive the animations. */
EW_DEFINE_AUTOOBJECT( EffectsEffectTimer, EffectsEffectTimerClass )

/* Initializer for the auto object 'Effects::EffectTimer' */
void EffectsEffectTimer__Init( EffectsEffectTimerClass _this )
{
  CoreTimer_OnSetPeriod((CoreTimer)_this, 15 );
  CoreTimer_OnSetEnabled((CoreTimer)_this, 1 );
}

/* Table with links to derived variants of the auto object : 'Effects::EffectTimer' */
EW_DEFINE_AUTOOBJECT_VARIANTS( EffectsEffectTimer )
EW_END_OF_AUTOOBJECT_VARIANTS( EffectsEffectTimer )

/* Initializer for the class 'Effects::TimingList' */
void EffectsTimingList__Init( EffectsTimingList _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsTimingList );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsTimingList );
}

/* Re-Initializer for the class 'Effects::TimingList' */
void EffectsTimingList__ReInit( EffectsTimingList _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::TimingList' */
void EffectsTimingList__Done( EffectsTimingList _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* Variants derived from the class : 'Effects::TimingList' */
EW_DEFINE_CLASS_VARIANTS( EffectsTimingList )
EW_END_OF_CLASS_VARIANTS( EffectsTimingList )

/* Virtual Method Table (VMT) for the class : 'Effects::TimingList' */
EW_DEFINE_CLASS( EffectsTimingList, XObject, _None, _None, _None, _None, _None, 
                 _None, "Effects::TimingList" )
EW_END_OF_CLASS( EffectsTimingList )

/* Initializer for the class 'Effects::FaderTask' */
void EffectsFaderTask__Init( EffectsFaderTask _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreTask__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsFaderTask );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsFaderTask );
}

/* Re-Initializer for the class 'Effects::FaderTask' */
void EffectsFaderTask__ReInit( EffectsFaderTask _this )
{
  /* At first re-initialize the super class ... */
  CoreTask__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::FaderTask' */
void EffectsFaderTask__Done( EffectsFaderTask _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreTask );

  /* Don't forget to deinitialize the super class ... */
  CoreTask__Done( &_this->_Super );
}

/* The method OnComplete() is called when the task is done with its work. The default 
   implementation of this method does nothing. You can override this method in derived 
   task classes and implement what to do when the task is finished. For example, 
   you can release resources used temporarily during animations.
   To complete a task you should call explicitly the method @Complete(). The parameter 
   aQueue refers to the queue this task belonged to. It can be used e.g. to schedule 
   again a task to the same queue, etc. */
void EffectsFaderTask_OnComplete( EffectsFaderTask _this, CoreTaskQueue aQueue )
{
  EffectsFader fader;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aQueue );

  fader = _this->first;

  while ( fader != 0 )
  {
    EffectsFader__OnEnd( fader );
    fader->Group->Super2.viewState = fader->Group->Super2.viewState & ~CoreViewStateRunningFader;
    fader = fader->next;
  }

  fader = _this->first;

  while ( fader != 0 )
  {
    EwPostSignal( fader->onComplete, ((XObject)fader ));
    fader = fader->next;
  }
}

/* The method OnCancel() is called when the task is canceled after being started. 
   The default implementation of this method does nothing. You can override this 
   method in derived task classes and implement what to do when the task is prematurely 
   aborted. For example, you can stop running timers and effects started in the 
   preceding @OnStart() method.
   To cancel a task you should call explicitly the method @Cancel(). The parameter 
   aQueue refers to the queue this task belonged to. It can be used e.g. to schedule 
   again a task to the same queue, etc. */
void EffectsFaderTask_OnCancel( EffectsFaderTask _this, CoreTaskQueue aQueue )
{
  EffectsFader fader;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aQueue );

  fader = _this->first;

  while ( fader != 0 )
  {
    EwPostSignal( fader->onCancel, ((XObject)fader ));
    fader = fader->next;
  }

  _this->first = 0;
  _this->last = 0;
}

/* The method OnStart() is called at the begin of the execution of this task. The 
   default implementation of the method simply cancels the task causing the next 
   available task in the task queue to be started. You should override this method 
   in derived task classes to implement what the task should do.
   There are three typical application cases how to implement the OnStart() method:
   - In its simplest case the entire task algorithm is implemented in the OnStart() 
   method. In this case the method @Complete() should be called before leaving OnStart().
   - If the task does take long time for execution by using timers or effects, you 
   should put in OnStart() the code necessary to start the timers/effects. Don't 
   forget to call @Complete() when all timers/effects are done.
   - If the task is divided in many small execution steps, the OnStart() method 
   should call @Continue() to request the @OnContinue() method to be executed after 
   a short delay (usually after the next screen update). In @OnContinue() you can 
   perform the next step of the task. If necessary, @OnContinue() can also request 
   to be called again after a short delay. At the end of the task, after the last 
   step is terminated, don't forget to call @Complete().
   The parameter aQueue refers to the queue this task belongs to. It can be used 
   to schedule more task to execute later. */
void EffectsFaderTask_OnStart( EffectsFaderTask _this, CoreTaskQueue aQueue )
{
  EffectsFader fader;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aQueue );

  if ( _this->first == 0 )
    CoreTask__Complete( _this );

  fader = _this->first;

  while ( fader != 0 )
  {
    fader->Group->Super2.viewState = ( fader->Group->Super2.viewState | CoreViewStateRunningFader ) 
    & ~CoreViewStatePendingFader;
    fader->Group->pendingFader = 0;
    fader = fader->next;
  }

  fader = _this->first;

  while ( fader != 0 )
  {
    EwSignal( fader->OnInitialize, ((XObject)fader ));
    EffectsFader__OnStart( fader );
    fader = fader->next;
  }
}

/* The method Complete() informs the task queue about the completion of this task. 
   Thereupon the next available task in the queue can be executed. This method is 
   usually called in context of the @OnStart() or @OnContinue() method when the 
   task has finalized its work. Calling the method for a not current task has no 
   effect. */
void EffectsFaderTask_Complete( EffectsFaderTask _this )
{
  EffectsFader fader = _this->first;

  while (( fader != 0 ) && EffectsFader__IsFinished( fader ))
    fader = fader->next;

  if ( fader == 0 )
    CoreTask_Complete((CoreTask)_this );
}

/* 'C' function for method : 'Effects::FaderTask.RemoveFader()' */
void EffectsFaderTask_RemoveFader( EffectsFaderTask _this, EffectsFader aFader )
{
  if ( aFader == 0 )
    return;

  if ( CoreTask_IsCurrent((CoreTask)_this ))
  {
    EwThrow( EwLoadString( &_Const0001 ));
    return;
  }

  if ( aFader->task != _this )
  {
    EwThrow( EwLoadString( &_Const0002 ));
    return;
  }

  if ( aFader->next != 0 )
    aFader->next->prev = aFader->prev;
  else
    _this->last = aFader->prev;

  if ( aFader->prev != 0 )
    aFader->prev->next = aFader->next;
  else
    _this->first = aFader->next;

  aFader->task = 0;
  aFader->next = 0;
  aFader->prev = 0;

  if ( aFader->Group != 0 )
    aFader->Group->pendingFader = 0;

  EwPostSignal( aFader->onCancel, ((XObject)aFader ));

  if ( _this->first == 0 )
    CoreTask_Cancel((CoreTask)_this );
}

/* 'C' function for method : 'Effects::FaderTask.AddFader()' */
void EffectsFaderTask_AddFader( EffectsFaderTask _this, EffectsFader aFader )
{
  if ( aFader == 0 )
    return;

  if ( CoreTask_IsCurrent((CoreTask)_this ))
  {
    EwThrow( EwLoadString( &_Const0003 ));
    return;
  }

  if ( aFader->task != 0 )
  {
    EwThrow( EwLoadString( &_Const0004 ));
    return;
  }

  aFader->prev = _this->last;
  aFader->next = 0;

  if ( _this->last != 0 )
    _this->last->next = aFader;
  else
    _this->first = aFader;

  _this->last = aFader;
  aFader->task = _this;
}

/* Variants derived from the class : 'Effects::FaderTask' */
EW_DEFINE_CLASS_VARIANTS( EffectsFaderTask )
EW_END_OF_CLASS_VARIANTS( EffectsFaderTask )

/* Virtual Method Table (VMT) for the class : 'Effects::FaderTask' */
EW_DEFINE_CLASS( EffectsFaderTask, CoreTask, last, _None, _None, _None, _None, _None, 
                 "Effects::FaderTask" )
  EffectsFaderTask_OnComplete,
  EffectsFaderTask_OnCancel,
  EffectsFaderTask_OnStart,
  EffectsFaderTask_Complete,
EW_END_OF_CLASS( EffectsFaderTask )

/* The global autoobject Effects::ShowHideCentered represents the most primitive 
   fade-in/out operation affecting the pure visibility of a given GUI component. 
   When using the transition for the fade-in operation, the GUI component is automatically 
   arranged to appear centered within the area of its owner component. This transition 
   is thus ideal wherever one GUI component should instantly appear or disappear 
   in context of another component without any animation effects.
   Please note, with this transition the fade-in operation is performed just at 
   the beginning of all other transitions running simultaneously, in other words, 
   the GUI component appears during the start phase of the transition. In turn, 
   to fade-out the component, the operation is executed during the end phase of 
   the transition. 
   This object exists for your convenience permitting you to simply refer the transition 
   wherever it is required in your implementation without having to take care of 
   the creation and configuration of the object. If you require the transition to 
   run with other configuration (e.g. to show the GUI component at the end instead 
   of the start phase), create a copy of this object and adapt its properties accordingly. */
EW_DEFINE_AUTOOBJECT( EffectsShowHideCentered, EffectsShowHideTransition )

/* Initializer for the auto object 'Effects::ShowHideCentered' */
void EffectsShowHideCentered__Init( EffectsShowHideTransition _this )
{
  EW_UNUSED_ARG( _this );
}

/* Table with links to derived variants of the auto object : 'Effects::ShowHideCentered' */
EW_DEFINE_AUTOOBJECT_VARIANTS( EffectsShowHideCentered )
EW_END_OF_AUTOOBJECT_VARIANTS( EffectsShowHideCentered )

/* The global autoobject Effects::FadeInOutCentered represents the fade-in/out operation 
   affecting the opacity of a given GUI component. When using the transition for 
   the fade-in operation, the GUI component is automatically arranged to appear 
   centered within the area of its owner component. This transition is thus ideal 
   wherever one GUI component should smoothly appear or disappear in context of 
   another component. The duration of the transition is configured per default to 
   take 500 ms.
   This object exists for your convenience permitting you to simply refer the transition 
   wherever it is required in your implementation without having to take care of 
   the creation and configuration of the object. If you require the transition to 
   run with other configuration (e.g. other timing parameters), create a copy of 
   this object and adapt its properties accordingly. */
EW_DEFINE_AUTOOBJECT( EffectsFadeInOutCentered, EffectsFadeInOutTransition )

/* Initializer for the auto object 'Effects::FadeInOutCentered' */
void EffectsFadeInOutCentered__Init( EffectsFadeInOutTransition _this )
{
  EW_UNUSED_ARG( _this );
}

/* Table with links to derived variants of the auto object : 'Effects::FadeInOutCentered' */
EW_DEFINE_AUTOOBJECT_VARIANTS( EffectsFadeInOutCentered )
EW_END_OF_AUTOOBJECT_VARIANTS( EffectsFadeInOutCentered )

/* Embedded Wizard */
