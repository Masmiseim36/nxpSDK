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

#ifndef Accelerator_H
#define Accelerator_H

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

#include "_AcceleratorAccelerator.h"
#include "_AcceleratorGraphicsFactory.h"

/* Font resource for large font - used for the caption text items. */
EW_DECLARE_FONT_RES( AcceleratorFontLarge )

/* Bitmap resource for the Embedded Wizard logo. */
EW_DECLARE_BITMAP_RES( AcceleratorLogo )

/* Font resource for small font - used for the description text items. */
EW_DECLARE_FONT_RES( AcceleratorFontSmall )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgBlend1 )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgBlend2 )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgBlend3 )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgBlend4 )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgBlend5 )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgBlend6 )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgBlend7 )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgCopy1 )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgCopy2 )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgCopy3 )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgCopy4 )

/* Bitmap resource for small photo images. */
EW_DECLARE_BITMAP_RES( AcceleratorImgCopy5 )

/* Font resource for large font - used for the caption text items. */
EW_DECLARE_FONT_RES( AcceleratorFontMedium )

/* Enumeration of all graphics objects supported by @GraphicsFactory. */
typedef enum
{
  AcceleratorGraphicsTypeRectFill       = 0,
  AcceleratorGraphicsTypeRectBlend      = 1,
  AcceleratorGraphicsTypeRGB565Bmp      = 2,
  AcceleratorGraphicsTypeRGB565BmpOpacity = 3,
  AcceleratorGraphicsTypeNativeBmp      = 4,
  AcceleratorGraphicsTypeNativeBmpOpacity = 5,
  AcceleratorGraphicsTypeAlpha8Bmp      = 6
} AcceleratorGraphicsType;

#ifdef __cplusplus
  }
#endif

#endif /* Accelerator_H */

/* Embedded Wizard */
