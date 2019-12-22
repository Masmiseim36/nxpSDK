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

#ifndef _CoreTime_H
#define _CoreTime_H

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

/* Forward declaration of the class Core::Time */
#ifndef _CoreTime_
  EW_DECLARE_CLASS( CoreTime )
#define _CoreTime_
#endif


/* The class Core::Time is used for date and time operations. Objects of this class 
   can store the time, convert it and format strings with the time, etc. */
EW_DEFINE_FIELDS( CoreTime, XObject )
  EW_PROPERTY( Second,          XInt32 )
  EW_PROPERTY( Minute,          XInt32 )
  EW_PROPERTY( Hour,            XInt32 )
  EW_PROPERTY( Day,             XInt32 )
  EW_PROPERTY( Month,           XInt32 )
  EW_PROPERTY( Year,            XInt32 )
EW_END_OF_FIELDS( CoreTime )

/* Virtual Method Table (VMT) for the class : 'Core::Time' */
EW_DEFINE_METHODS( CoreTime, XObject )
EW_END_OF_METHODS( CoreTime )

/* 'C' function for method : 'Core::Time.getMonthName()' */
XString CoreTime_getMonthName( CoreTime _this, XInt32 aMonth, XBool aAbbreviated );

/* 'C' function for method : 'Core::Time.getDayOfWeekName()' */
XString CoreTime_getDayOfWeekName( CoreTime _this, XInt32 aDayOfWeek, XBool aAbbreviated );

/* 'C' function for method : 'Core::Time.getCurrentTime()' */
XUInt32 CoreTime_getCurrentTime( CoreTime _this );

/* 'C' function for method : 'Core::Time.days2MonthLeapYear()' */
XInt32 CoreTime_days2MonthLeapYear( CoreTime _this, XInt32 aMonth );

/* 'C' function for method : 'Core::Time.days2Month()' */
XInt32 CoreTime_days2Month( CoreTime _this, XInt32 aMonth );

/* 'C' function for method : 'Core::Time.OnGetTime()' */
XUInt32 CoreTime_OnGetTime( CoreTime _this );

/* 'C' function for method : 'Core::Time.OnSetTime()' */
void CoreTime_OnSetTime( CoreTime _this, XUInt32 value );

/* 'C' function for method : 'Core::Time.OnGetCurrentTime()' */
CoreTime CoreTime_OnGetCurrentTime( CoreTime _this );

/* 'C' function for method : 'Core::Time.OnGetDayOfWeek()' */
XInt32 CoreTime_OnGetDayOfWeek( CoreTime _this );

/* 'C' function for method : 'Core::Time.OnSetSecond()' */
void CoreTime_OnSetSecond( CoreTime _this, XInt32 value );

/* 'C' function for method : 'Core::Time.OnSetMinute()' */
void CoreTime_OnSetMinute( CoreTime _this, XInt32 value );

/* 'C' function for method : 'Core::Time.OnSetHour()' */
void CoreTime_OnSetHour( CoreTime _this, XInt32 value );

/* 'C' function for method : 'Core::Time.OnSetDay()' */
void CoreTime_OnSetDay( CoreTime _this, XInt32 value );

/* 'C' function for method : 'Core::Time.OnSetMonth()' */
void CoreTime_OnSetMonth( CoreTime _this, XInt32 value );

/* 'C' function for method : 'Core::Time.OnSetYear()' */
void CoreTime_OnSetYear( CoreTime _this, XInt32 value );

/* The method Initialize() initializes this Core::Time object with the given time 
   as number of seconds since 1.1.1970 and returns this object. */
CoreTime CoreTime_Initialize( CoreTime _this, XUInt32 aTime );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreTime_H */

/* Embedded Wizard */
