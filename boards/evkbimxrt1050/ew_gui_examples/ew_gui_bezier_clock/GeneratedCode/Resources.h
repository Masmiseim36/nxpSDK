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

#ifndef Resources_H
#define Resources_H

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

#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"

/* Bitmap resource : 'Resources::WhiteBitmap' */
EW_DECLARE_BITMAP_RES( ResourcesWhiteBitmap )

/* Bitmap resource : 'Resources::WhiteBitmapStripe' */
EW_DECLARE_BITMAP_RES( ResourcesWhiteBitmapStripe )

/* Bitmap resource containing diverse icons you can use in your project. Usually 
   the icons are used to represent navigation buttons, etc.. The icons have the 
   size 36x36 pixel. */
EW_DECLARE_BITMAP_RES( ResourcesNavigationIconsMedium )

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesMonday;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesTuesday;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesWednesday;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesThursday;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesFriday;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesSaturday;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesSunday;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesJanuary;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesFebruary;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesMarch;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesApril;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesMay;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesJune;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesJuly;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesAugust;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesSeptember;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesOctober;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesNovember;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesDecember;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesMondayAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesTuesdayAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesWednesdayAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesThursdayAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesFridayAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesSaturdayAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesSundayAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesJanuaryAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesFebruaryAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesMarchAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesAprilAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesMayAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesJuneAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesJulyAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesAugustAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesSeptemberAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesOctoberAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesNovemberAbbr;

/* Constant string used by Core::Time class to format time and date. Per default 
   the constant is initialized in English language. If necessary, you can derive 
   a variant from the interesting constant, store the variant in one of your project 
   units and initialize it with other eventually multi-lingual localized text. */
extern const XStringRes ResourcesDecemberAbbr;

#ifdef __cplusplus
  }
#endif

#endif /* Resources_H */

/* Embedded Wizard */
