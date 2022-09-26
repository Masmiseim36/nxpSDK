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
*   This header belongs to the low level software driver for target platforms
*   based on the RGB565 screen and RGBA8888 native pixel format whereby the
*   screen pixel format does correspond exclusively to the format of the frame
*   buffer.
*
*   This implementation includes following special options:
*
*   > Pre-multiplied or non pre-multiplied pixel RGB color channels. Depending
*     on the graphics hardware, it may be expected that RGB color channels are
*     stored and calculated as already alpha pre-multiplied values.
*
*     This option is controlled by the definition EW_PREMULTIPLY_COLOR_CHANNELS.
*     If this macro is defined, the pixel driver handles the RGB color channels
*     as already pre-multiplied by the corresponding pixel alpha value.
*
*     To select the pre-multiplied color format take following line into your
*     'ewextgfx.h' file:
*
*       #define EW_PREMULTIPLY_COLOR_CHANNELS 1
*
*     If your graphics hardware doesn't work with pre-multiplied color channels
*     set the following line into your 'ewextgfx.h' file:
*
*       #define EW_PREMULTIPLY_COLOR_CHANNELS 0
*
*     Please note, this option affects the native pixel format RGBA8888 only.
*     The screen pixel format does not store any alpha information.
*
*   > Custom specific color channel order of the native pixel format. Depending
*     on the graphics hardware, it may be necessary to adapt the order in which
*     the color channels are stored in the video memory of a native surface.
*
*     The format name 'RGBA8888' refers to the generic 32 bit color format
*     where the color and alpha channels are stored within a 32 bit value
*     in the following manner:
*
*              31           24           16             8            0
*               +------------+---------------------------------------+
*               |   alpha    |    blue    |    green    |     red    |
*               +------------+---------------------------------------+
*
*
*     To control the order of channels, specify the bit start position of each
*     channel. For this purpose the macros EW_COLOR_CHANNEL_BIT_OFFSET_XXX are
*     available (XXX stands for RED, GREEN, BLUE and ALPHA). For example to
*     adapt this pixel driver to graphics hardware using BGRA8888 color format,
*     add following lines to your 'ewextgfx.h' file:
*
*       #define EW_COLOR_CHANNEL_BIT_OFFSET_RED    16
*       #define EW_COLOR_CHANNEL_BIT_OFFSET_GREEN   8
*       #define EW_COLOR_CHANNEL_BIT_OFFSET_BLUE    0
*       #define EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA  24
*
*     Please note, these options affect the native pixel format RGBA8888 only.
*
*   > Custom specific color channel order of the screen pixel format. Depending
*     on the graphics hardware, it may be necessary to adapt the order in which
*     the color channels are stored in the video memory of a screen surface.
*
*     The format name 'RGB565' refers to the generic 16 bit color format where
*     the color channels are stored within a 16 bit value in the following
*     manner:
*
*              15           11             5            0
*               +---------------------------------------+
*               |    blue    |    green    |     red    |
*               +---------------------------------------+
*
*
*     Note, the green information always occupies the middle 6 bit wide channel.
*     This can not be changed.
*
*     To control the order of channels, specify the bit start position of each
*     channel. For this purpose the macros EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_XXX
*     are available (XXX stands for RED and BLUE - green channel position is
*     fixed as mentioned above). For example to adapt this pixel driver to
*     graphics hardware using BGR565 color format, add following lines to your
*     'ewextgfx.h' file:
*
*       #define EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_RED   11
*       #define EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_BLUE  0
*
*     Please note, these options affect the screen pixel format RGB565 only.
*
*   > The RGB565 bitmap source format configuration does correspond to the of
*     the SCREEN format as described above.
*
*******************************************************************************/


/* If not explicitly defined, assume default bit positions for the RGBA channels
   within a pixel memory. */
#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_RED
  #define EW_COLOR_CHANNEL_BIT_OFFSET_RED    0
#endif

#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_GREEN
  #define EW_COLOR_CHANNEL_BIT_OFFSET_GREEN  8
#endif

#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_BLUE
  #define EW_COLOR_CHANNEL_BIT_OFFSET_BLUE  16
#endif

#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA
  #define EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA 24
#endif


/* Verify the position of the red, green, blue and alpha channel */
#if (( EW_COLOR_CHANNEL_BIT_OFFSET_RED != 0  ) &&                              \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_RED != 8  ) &&                              \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_RED != 16 ) &&                              \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_RED != 24 ))
  #error Wrong 'red' channel bit position. Only '0', '8', '16' or '24' allowed.
#endif

#if (( EW_COLOR_CHANNEL_BIT_OFFSET_GREEN != 0  ) &&                            \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_GREEN != 8  ) &&                            \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_GREEN != 16 ) &&                            \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_GREEN != 24 ))
  #error Wrong 'green' channel bit position. Only '0', '8', '16' or '24' allowed.
#endif

#if (( EW_COLOR_CHANNEL_BIT_OFFSET_BLUE != 0  ) &&                             \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_BLUE != 8  ) &&                             \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_BLUE != 16 ) &&                             \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_BLUE != 24 ))
  #error Wrong 'blue' channel bit position. Only '0', '8', '16' or '24' allowed.
#endif

#if (( EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA != 0  ) &&                            \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA != 8  ) &&                            \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA != 16 ) &&                            \
     ( EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA != 24 ))
  #error Wrong 'alpha' channel bit position. Only '0', '8', '16' or '24' allowed.
#endif

#if ( EW_COLOR_CHANNEL_BIT_OFFSET_RED   == EW_COLOR_CHANNEL_BIT_OFFSET_BLUE  ) ||\
    ( EW_COLOR_CHANNEL_BIT_OFFSET_RED   == EW_COLOR_CHANNEL_BIT_OFFSET_GREEN ) ||\
    ( EW_COLOR_CHANNEL_BIT_OFFSET_RED   == EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA ) ||\
    ( EW_COLOR_CHANNEL_BIT_OFFSET_GREEN == EW_COLOR_CHANNEL_BIT_OFFSET_BLUE  ) ||\
    ( EW_COLOR_CHANNEL_BIT_OFFSET_GREEN == EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA ) ||\
    ( EW_COLOR_CHANNEL_BIT_OFFSET_BLUE  == EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA )
  #error Bit offset conflict in the color channel definition.
#endif


/* If not explicitly defined, assume default bit positions for the RGB channels
   of the screen format within a pixel memory. */
#ifndef EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_RED
  #define EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_RED    0
#endif

#ifndef EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_GREEN
  #define EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_GREEN  5
#endif

#ifndef EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_BLUE
  #define EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_BLUE  11
#endif


/* Verify the position of the red, green and blue channels of the screen 
   format */
#if (( EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_RED != 0  ) &&                       \
     ( EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_RED != 11 ))
  #error Wrong 'red' channel bit position. Only '0' or '11' allowed.
#endif

#if EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_GREEN != 5
  #error Wrong 'green' channel bit position. Only '5' allowed.
#endif

#if (( EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_BLUE != 0  ) &&                      \
     ( EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_BLUE != 11 ))
  #error Wrong 'blue' channel bit position. Only '0' or '11' allowed.
#endif

#if EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_RED ==                                  \
    EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_BLUE
  #error Bit offset conflict in the color channel definition.
#endif


/* This pixel driver expects the channel order of the RGB565 and SCREEN surfaces
   to be identical. The channel order of the RGB565 surface can't be configured
   individually. */
#define EW_RGB565_COLOR_CHANNEL_BIT_OFFSET_RED                                 \
  EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_RED

#define EW_RGB565_COLOR_CHANNEL_BIT_OFFSET_GREEN                               \
  EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_GREEN

#define EW_RGB565_COLOR_CHANNEL_BIT_OFFSET_BLUE                                \
  EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_BLUE 


/* The following mode can be enabled for Index8 target systems only. RGB565 / 
   RGBA8888 don't support the global CLUT/pallete. */
#ifdef EW_ENABLE_COLOR_TABLE
  #error Please remove the macro 'EW_ENABLE_COLOR_TABLE' from your make file.
#endif


/* RGB565 / RGBA8888 expects the 'screen' color format to be enabled in the
   Graphics Engine. Verify whether the correct macro is defined. */
#ifndef EW_USE_PIXEL_FORMAT_SCREEN
  #error Please ensure the macro 'EW_USE_PIXEL_FORMAT_SCREEN' is defined.
#endif


/* pba */
