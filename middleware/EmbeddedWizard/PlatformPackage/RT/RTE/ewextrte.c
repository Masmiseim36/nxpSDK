/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software and related documentation ("Software") are intellectual
* property owned by TARA Systems and are copyright of TARA Systems.
* Any modification, copying, reproduction or redistribution of the Software in
* whole or in part by any means not in accordance with the End-User License
* Agreement for Embedded Wizard is expressly prohibited. The removal of this
* preamble is expressly prohibited.
*
********************************************************************************
*
* DESCRIPTION:
*   This file belongs to the external platform adaptation layer of the Embedded
*   Wizard Runtime Environment. It abstracts the interface between the platform
*   independent core of Runtime Environment and the underlying, target specific
*   operating system and the used 'C' compiler version.
*
*   This file implements the interface by using the GCC compiler environment.
*
* SUBROUTINES:
*   EwAlloc
*   EwFree
*   EwIsMemory
*   EwPanic
*   EwConsoleOutput
*   EwSaveRegister
*   EwGetTicks
*   EwGetTime
*   EwGetPerfCounter
*   EwZero
*   EwFill
*   EwMove
*   EwCopy
*   EwStrCpy
*   EwStrCat
*   EwStrCmp
*   EwStrLen
*   EwMathSin
*   EwMathCos
*   EwMathArcSin
*   EwMathArcCos
*   EwMathSqrt
*   EwMathRandInt32
*   EwMathRandFloat
*   EwMathPow
*   EwMathRound
*   EwMathFloor
*   EwMathCeil
*
*******************************************************************************/

#include "ewrte.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>

#include "ew_bsp_console.h"
#include "ew_bsp_clock.h"

/* Flag to identify whether the random generator has been initialized */
static int RandInitialized = 0;


/*******************************************************************************
* FUNCTION:
*   EwAlloc
*
* DESCRIPTION:
*   The function EwAlloc() will be called by the EWRTE each time a new memory
*   block should be reserved. For example, the function EwNewObject() calls
*   EwAlloc() to reserve memory for new objects.
*
*   EwAlloc() has to reserve a new memory block with the given size aSize. The
*   function has to return a pointer to the memory block or NULL if there is no
*   memory available.
*
* ARGUMENTS:
*   aSize - Desired size of the new memory block in bytes.
*
* RETURN VALUE:
*   EwAlloc() has to return a pointer to the reserved memory block or NULL if
*   there is no memory available.
*
*******************************************************************************/
void* EwAlloc( int aSize )
{
  void* mem = EwAllocHeapBlock( aSize );
  return mem;
}


/*******************************************************************************
* FUNCTION:
*   EwFree
*
* DESCRIPTION:
*   The function EwFree() should be implemented by the programer. The function
*   will be called by the EWRTE each time a memory block should be released.
*   The function EwFree() will be called from the Garbage Collector if garbage
*   has been found.
*
* ARGUMENTS:
*   aMemory - Address of the memory block to release.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFree( void* aMemory )
{
  EwFreeHeapBlock( aMemory );
}


/*******************************************************************************
* FUNCTION:
*   EwIsMemory
*
* DESCRIPTION:
*   The function EwIsMemory() should be implemented together with the both
*   above functions EwAlloc() and EwFree(). EwIsMemory() will be called by
*   the EWRTE in order to test whether the given pointer does address within
*   the memory area used by the heap (used for EwAlloc() operations).
*
* ARGUMENTS:
*   aPtr - Address to test.
*
* RETURN VALUE:
*   EwIsMemory() has to return != 0 if the given pointer aPtr addresses within
*   the memory area used by the heap manager. Otherwise 0 should be returned.
*
*******************************************************************************/
int EwIsMemory( void* aPtr )
{
  return EwIsHeapPtr( aPtr );
}


/*******************************************************************************
* FUNCTION:
*   EwPanic
*
* DESCRIPTION:
*   The function EwPanic() will be called by the Run Time Environment if a fatal
*   system failure is detected.
*
*   This function can be adapted to decide what to do when a system failure
*   is occured. Generally EwPanic() should log the system failure and restart
*   the system.
*
*   For debugging purpose EwPanic() can run into a for-ever-loop or it can
*   cause a monitor breakpoint, etc.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   EwPanic() will never return.
*
*******************************************************************************/
void EwPanic( void )
{
  /* Print out the message ... */
  EwPrint( "\nPANIC: System halted\n" );

  /* ... and stay in the never ending loop */
  for ( ;; )
   ;
}


/*******************************************************************************
* FUNCTION:
*   EwConsoleOutput
*
* DESCRIPTION:
*   The function EwConsoleOutput() prints the message to the debug console.
*   This function should be implemented by the programer in order to output
*   trace messages on the debug console, etc.
*
* ARGUMENTS:
*   aMessage - Zero-terminated string containing the message to output.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwConsoleOutput( const char* aMessage )
{
  while( *aMessage )
  {
    /* add carriage return in case of newline */
    if ( *aMessage == '\n' )
      EwBspConsolePutCharacter( '\r' );

    EwBspConsolePutCharacter( *aMessage++ );
  }
}


/*******************************************************************************
* FUNCTION:
*   EwSaveRegister
*
* DESCRIPTION:
*   The function EwSaveRegister() has the job to copy all general purpose CPU
*   register to the memory area specified in the parameter aMemory.
*
* ARGUMENTS:
*   aBuffer - Pointer to a memory area where to save the register contents.
*     The capacity of the memory area is limited to 32 registers. This means
*     on a 32-bit CPU it is 128 bytes large. On a 64-bit CPU it is 256 bytes
*     large.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSaveRegister( void* aBuffer )
{
  /* The clib function setjmp() copies all relevant CPU register to a memory
     area. This is exactly what we need. */
  setjmp( *(jmp_buf*)aBuffer );
}


/*******************************************************************************
* FUNCTION:
*   EwGetTicks
*
* DESCRIPTION:
*   The function EwGetTicks() should return the number of milliseconds that have
*   been elapsed since the system was started. It is limited to the resolution
*   of the system timer. The ticks are used as base for EmWi timers.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Number of milliseconds since the system was started.
*
*******************************************************************************/
unsigned long EwGetTicks( void )
{
  return EwBspClockGetTicks();
}


/*******************************************************************************
* FUNCTION:
*   EwGetTime
*
* DESCRIPTION:
*   The function EwGetTime() should return the current time as the number of
*   seconds elapsed since midnight 1.1.1970. If possible the real time clock
*   should be queried in order to determinate the current time. This function
*   is used by the Mosaic class Core::Time and should be adapted to the real
*   time clock or to the system timer.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Returns the number of seconds elapsed since midnight 1.1.1970.
*
*******************************************************************************/
unsigned long EwGetTime( void )
{
  return EwBspClockGetTime();
}


#ifdef EW_PRINT_PERF_COUNTERS

/*******************************************************************************
* FUNCTION:
*   EwGetPerfCounter
*
* DESCRIPTION:
*   The function EwGetPerfCounter() should return the current time as number of
*   seconds and microseconds. The reference time point is nonrelevant.
*
* ARGUMENTS:
*   aSeconds         - Pointer to a variable where the current time is stored.
*   aMicroseconds    - Pointer to a variable where the current time is stored.
*   aCPUMicroseconds - Pointer to a variable where the real CPU usage is stored.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwGetPerfCounter( unsigned long* aSeconds, unsigned long* aMicroSeconds,
  unsigned long* aCPUMicroseconds )
{
/*
  struct timeval time;

  gettimeofday( &time, NULL );
  *aSeconds      = time.tv_sec;
  *aMicroSeconds = time.tv_usec;

  if ( aCPUMicroseconds )
    *aCPUMicroseconds = clock();
*/
// msy: to be implemented
}

#endif


/*******************************************************************************
* FUNCTION:
*   EwZero
*
* DESCRIPTION:
*   The function EwZero() provides a plattform independent version of the 'C'
*   standard library function memset( aDstPtr, 0, aCount ).
*
*   This function fills the given memory area with the constant value zero.
*
* ARGUMENTS:
*   aDstPtr - Pointer to memory area to fill with zero.
*   aCount  - Number of bytes to fill with zero.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwZero( void* aDstPtr, int aCount )
{
  memset( aDstPtr, 0, aCount );
}


/*******************************************************************************
* FUNCTION:
*   EwFill
*
* DESCRIPTION:
*   The function EwFill() provides a plattform independent version of the 'C'
*   standard library function memset( aDstPtr, aValue, aCount ).
*
*   This function fills the given memory area with the constant value aValue.
*
* ARGUMENTS:
*   aDstPtr - Pointer to memory area to fill with the value.
*   aValue  - Byte value to fill the memory.
*   aCount  - Number of bytes to fill with the value.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFill( void* aDstPtr, unsigned char aValue, int aCount )
{
  memset( aDstPtr, aValue, aCount );
}


/*******************************************************************************
* FUNCTION:
*   EwMove
*
* DESCRIPTION:
*   The function EwMove() provides a plattform independent version of the 'C'
*   standard library function memove( aDstPtr, aSrcPtr, aCount ).
*
*   This function copies the given source memory area into the destination area.
*
* ARGUMENTS:
*   aDstPtr - Pointer to the destination memory area where to write the data.
*   aSrcPtr - Pointer to the source memory to read.
*   aCount  - Number of bytes to copy from aSrcPtr to aDstPtr.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwMove( void* aDstPtr, const void* aSrcPtr, int aCount )
{
  memmove( aDstPtr, aSrcPtr, aCount );
}


/*******************************************************************************
* FUNCTION:
*   EwCopy
*
* DESCRIPTION:
*   The function EwCopy() provides a plattform independent version of the 'C'
*   standard library function memcpy( aDstPtr, aSrcPtr, aCount ).
*
*   This function copies the given source memory area into the destination area.
*
* ARGUMENTS:
*   aDstPtr - Pointer to the destination memory area where to write the data.
*   aSrcPtr - Pointer to the source memory to read.
*   aCount  - Number of bytes to copy from aSrcPtr to aDstPtr.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopy( void* aDstPtr, const void* aSrcPtr, int aCount )
{
  memcpy( aDstPtr, aSrcPtr, aCount );
}


/*******************************************************************************
* FUNCTION:
*   EwStrCpy
*
* DESCRIPTION:
*   The function EwStrCpy() provides a plattform independent version of the 'C'
*   standard library function strcpy( aDst, aSrc ).
*
*   This function copies a zero terminated ASCII string from the source to the
*   destination memory.
*
* ARGUMENTS:
*   aDst - Pointer to the destination memory area where to write the string.
*   aSrc - Pointer to the source memory with the string to copy.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwStrCpy( char* aDst, const char* aSrc )
{
  strcpy( aDst, aSrc );
}


/*******************************************************************************
* FUNCTION:
*   EwStrCat
*
* DESCRIPTION:
*   The function EwStrCat() provides a plattform independent version of the 'C'
*   standard library function strcat( aDst, aSrc ).
*
*   This function appends the zero terminated ASCII string from the source at
*   the end of the destination string.
*
* ARGUMENTS:
*   aDst - Pointer to the destination memory area where to write the string.
*   aSrc - Pointer to the source memory with the string to append.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwStrCat( char* aDst, const char* aSrc )
{
  strcat( aDst, aSrc );
}


/*******************************************************************************
* FUNCTION:
*   EwStrCmp
*
* DESCRIPTION:
*   The function EwStrCmp() provides a plattform independent version of the 'C'
*   standard library function strcmp( aStr1, aStr2 ).
*
*   This function compares the both strings and returns:
*     value <  0 if aStr1 is less than aStr2
*     value == 0 if aStr1 is equal aStr2
*     value >  0 if aStr1 is greater than aStr2
*
* ARGUMENTS:
*   aStr1,
*   aStr2 - Pointers to the both strings to compare.
*
* RETURN VALUE:
*   Returns a value indicating the lexicographic relation of the both strings.
*
*******************************************************************************/
int EwStrCmp( const char* aStr1, const char* aStr2 )
{
  return strcmp( aStr1, aStr2 );
}


/*******************************************************************************
* FUNCTION:
*   EwStrLen
*
* DESCRIPTION:
*   The function EwStrLen() provides a plattform independent version of the 'C'
*   standard library function strlen( aStr ).
*
*   This function returns the length of the string in bytes.
*
* ARGUMENTS:
*   aStr - Pointer to the string to obtain its length.
*
* RETURN VALUE:
*   Returns a length of the string.
*
*******************************************************************************/
int EwStrLen( const char* aStr )
{
  return strlen( aStr );
}


/*******************************************************************************
* FUNCTION:
*   EwMathSin
*
* DESCRIPTION:
*   The function EwMathSin() implements the Chora math_sin() function.
*
* ARGUMENTS:
*   aAngle - An angle in degree.
*
* RETURN VALUE:
*   Returns the determinated sine value.
*
*******************************************************************************/
XFloat EwMathSin( XFloat aAngle )
{
  static const float factor = (float)( 3.1415926535 / 180.0 );

  return sinf( aAngle * factor );
}


/*******************************************************************************
* FUNCTION:
*   EwMathCos
*
* DESCRIPTION:
*   The function EwMathCos() implements the Chora math_cos() function.
*
* ARGUMENTS:
*   aAngle - An angle in degree.
*
* RETURN VALUE:
*   Returns the determinated cosine value.
*
*******************************************************************************/
XFloat EwMathCos( XFloat aAngle )
{
  static const float factor = (float)( 3.1415926535 / 180.0 );

  return cosf( aAngle * factor );
}


/*******************************************************************************
* FUNCTION:
*   EwMathArcSin
*
* DESCRIPTION:
*   The function EwMathArcSin() implements the Chora math_asin() function.
*
* ARGUMENTS:
*   aValue - Value to calculate the arc sine. Only values in the range
*     -1.0 .. +1.0 are valid.
*
* RETURN VALUE:
*   Returns the determinated angle in the range -90 .. +90 degress.
*
*******************************************************************************/
XFloat EwMathArcSin( XFloat aValue )
{
  static const float factor = (float)( 180.0 / 3.1415926535 );

  return asinf( aValue ) * factor;
}


/*******************************************************************************
* FUNCTION:
*   EwMathArcCos
*
* DESCRIPTION:
*   The function EwMathArcCos() implements the Chora math_acos() function.
*
* ARGUMENTS:
*   aValue - Value to calculate the arc cosine. Only values in the range
*     -1.0 .. +1.0 are valid.
*
* RETURN VALUE:
*   Returns the determinated angle in the range 0 .. +180 degress.
*
*******************************************************************************/
XFloat EwMathArcCos( XFloat aValue )
{
  static const float factor = (float)( 180.0 / 3.1415926535 );

  return acosf( aValue ) * factor;
}


/*******************************************************************************
* FUNCTION:
*   EwMathSqrt
*
* DESCRIPTION:
*   The function EwMathSqrt() implements the Chora math_sqrt() function. It
*   calculates the square root value.
*
* ARGUMENTS:
*   aValue - The value to calculate square root.
*
* RETURN VALUE:
*   Returns the determinated square root value.
*
*******************************************************************************/
XFloat EwMathSqrt( XFloat aValue )
{
  return sqrtf( aValue );
}


/*******************************************************************************
* FUNCTION:
*   EwMathPow
*
* DESCRIPTION:
*   The function EwMathPow() implements the Chora math_pow() function. It
*   calculates the power a ^ b function.
*
* ARGUMENTS:
*   aA - The value to calculate the pow()
*   aB - The value to calculate the pow()
*
* RETURN VALUE:
*   Returns the determinated power value.
*
*******************************************************************************/
XFloat EwMathPow( XFloat aA, XFloat aB )
{
  return powf( aA, aB );
}


/*******************************************************************************
* FUNCTION:
*   EwMathRandInt32
*
* DESCRIPTION:
*   The function EwMathRandInt32() implements the Chora math_rand() function
*   for int32 operands. It calculates the random value in the range aValue1 ..
*   aValue2.
*
* ARGUMENTS:
*   aValue1,
*   aValue2  - Range for the value.
*
* RETURN VALUE:
*   Returns the determinated random value.
*
*******************************************************************************/
XInt32 EwMathRandInt32( XInt32 aValue1, XInt32 aValue2 )
{
  unsigned int r;

  /* For the first time set the random start value */
  if ( !RandInitialized )
  {
    srand((unsigned)EwGetTime());
    RandInitialized = 1;
  }

  r = rand();
  r = ( r & 0xFFFF ) ^ ( r >> 16 );
  return aValue1 + (((int)r * ( aValue2 - aValue1 )) >> 16 );
}


/*******************************************************************************
* FUNCTION:
*   EwMathRandFloat
*
* DESCRIPTION:
*   The function EwMathRandFloat() implements the Chora math_rand() function
*   for float operands. It calculates the random value in the range aValue1 ..
*   aValue2.
*
* ARGUMENTS:
*   aValue1,
*   aValue2  - Range for the value.
*
* RETURN VALUE:
*   Returns the determinated random value.
*
*******************************************************************************/
XFloat EwMathRandFloat( XFloat aValue1, XFloat aValue2 )
{
  static const float invRandMax = 1.0f / (float)RAND_MAX;
  unsigned int       r;

  /* For the first time set the random start value */
  if ( !RandInitialized )
  {
    srand((unsigned)EwGetTime());
    RandInitialized = 1;
  }

  r = rand();
  return aValue1 + ((float)r * ( aValue2 - aValue1 ) * invRandMax );
}


/*******************************************************************************
* FUNCTION:
*   EwMathRound
*
* DESCRIPTION:
*   The function EwMathRound() implements the Chora math_round() function.
*
* ARGUMENTS:
*   aValue - A value to round.
*
* RETURN VALUE:
*   Returns the correctly rounded value.
*
*******************************************************************************/
XFloat EwMathRound( XFloat aValue )
{
  return floorf( aValue + 0.5f );
}


/*******************************************************************************
* FUNCTION:
*   EwMathFloor
*
* DESCRIPTION:
*   The function EwMathFloor() implements the Chora math_floor() function.
*
* ARGUMENTS:
*   aValue - A value to round.
*
* RETURN VALUE:
*   Returns the floor rounded value.
*
*******************************************************************************/
XFloat EwMathFloor( XFloat aValue )
{
  return floorf( aValue );
}



/*******************************************************************************
* FUNCTION:
*   EwMathCeil
*
* DESCRIPTION:
*   The function EwMathCeil() implements the Chora math_ceil() function.
*
* ARGUMENTS:
*   aValue - A value to round.
*
* RETURN VALUE:
*   Returns the ceil rounded value.
*
*******************************************************************************/
XFloat EwMathCeil( XFloat aValue )
{
  return ceilf( aValue );
}


/* pba, msy */
