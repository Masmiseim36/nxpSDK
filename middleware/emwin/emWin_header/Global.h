/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2016  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.38 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  NXP Semiconductors USA, Inc.  whose
registered  office  is  situated  at 411 E. Plumeria Drive, San  Jose,
CA 95134, USA  solely for  the  purposes  of  creating  libraries  for
NXPs M0, M3/M4 and  ARM7/9 processor-based  devices,  sublicensed  and
distributed under the terms and conditions of the NXP End User License
Agreement.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information

Licensor:                 SEGGER Microcontroller Systems LLC
Licensed to:              NXP Semiconductors, 1109 McKay Dr, M/S 76, San Jose, CA 95131, USA
Licensed SEGGER software: emWin
License number:           GUI-00186
License model:            emWin License Agreement, dated August 20th 2011
Licensed product:         -
Licensed platform:        NXP's ARM 7/9, Cortex-M0,M3,M4
Licensed number of seats: -
----------------------------------------------------------------------
File    : Global.h
Purpose : Global types
          In case your application already has a Global.h, you should
          merge the files. In order to use Segger code, the types
          U8, U16, U32, I8, I16, I32 need to be defined in Global.h;
          additional definitions do not hurt.
Revision: $Rev: 6050 $
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GLOBAL_H            // Guard against multiple inclusion
#define GLOBAL_H

#define U8    unsigned char
#define I8    signed char
#define U16   unsigned short
#define I16   signed short
#ifdef __x86_64__
#define U32   unsigned
#define I32   int
#else
#define U32   unsigned long
#define I32   signed long
#endif

#ifdef _WIN32
  //
  // Microsoft VC6 compiler related
  //
  #ifdef __MINGW32__
    #define U64   unsigned long long
    #define I64   long long
  #else
    #define U64   unsigned __int64
    #define U128  unsigned __int128
    #define I64   __int64
    #define I128  __int128
    #if _MSC_VER <= 1200
      #define U64_C(x) x##UI64
    #else
      #define U64_C(x) x##ULL
    #endif
  #endif
#else
  //
  // C99 compliant compiler
  //
  #define U64   unsigned long long
  #define I64   signed long long
  #define U64_C(x) x##ULL
#endif

#endif                      // Avoid multiple inclusion

/*************************** End of file ****************************/
