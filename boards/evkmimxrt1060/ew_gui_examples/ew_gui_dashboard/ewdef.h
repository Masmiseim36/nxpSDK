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
********************************************************************************
*
* DESCRIPTION:
*   This file contains basic defines and useful configuration macros.
*
*******************************************************************************/

#ifndef EWDEF_H
#define EWDEF_H

#if defined ( __ICCARM__ )
  #define EW_MEM_SECTION( aName ) \
    _Pragma(EW_STRINGIZE(location=EW_STRINGIZE(aName)))
#else
  #define EW_MEM_SECTION( aName ) \
    __attribute__((section(EW_STRINGIZE(aName))))
#endif

/* defines for different framebuffer color formats */
#define EW_FRAME_BUFFER_COLOR_FORMAT_Index8   1
#define EW_FRAME_BUFFER_COLOR_FORMAT_LumA44   2
#define EW_FRAME_BUFFER_COLOR_FORMAT_RGB565   3
#define EW_FRAME_BUFFER_COLOR_FORMAT_RGB888   4
#define EW_FRAME_BUFFER_COLOR_FORMAT_RGBA4444 5
#define EW_FRAME_BUFFER_COLOR_FORMAT_RGBA8888 6

#if ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGBA8888 )
  #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "RGBA8888"
  #define FRAME_BUFFER_DEPTH  4
#elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGB888 )
  #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "RGB888"
  #define FRAME_BUFFER_DEPTH  3
#elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGBA4444 )
  #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "RGBA4444"
  #define FRAME_BUFFER_DEPTH  2
#elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGB565 )
  #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "RGB565"
  #define FRAME_BUFFER_DEPTH  2
#elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_Index8 )
  #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "Index8"
  #define FRAME_BUFFER_DEPTH  1
#elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_LumA44 )
  #define EW_FRAME_BUFFER_COLOR_FORMAT_STRING "LumA44"
  #define FRAME_BUFFER_DEPTH  1
#else
  #error "EW_FRAME_BUFFER_COLOR_FORMAT not supported"
#endif

#define EW_STRINGIZE( aArg )      EW_STRINGIZE_ARG( aArg )
#define EW_STRINGIZE_ARG( aArg )  #aArg

#endif

/* mli, msy */
