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
*   This header file contains the default configuration and its validation rules
*   for the Embedded Wizard Graphics Engine (EWGFX).
*
*   Generally, the Graphics Engine is configured by macro definitions within
*   your make file or by the platform specific header file 'ewextgfx.h'. The
*   default configuration values serve only as a kind of fallback if no other
*   definitions are defined.
*
*   On the other hand, the rules implemented here validate the platform specific
*   configuration parameters and report errors if invalid values are specified.
*
*   This file is NOT intended for the editing nor for any custom adaptations!
*   The adaptations should always occur in:
*
*   1. your make file, if you are the customer who wants to adapt some values.
*      In this case simply define the desired macro in the make file. Your
*      definitions will override the default values.
*
*      or in
*
*   2. the platform specific header file 'ewextgfx.h', if you are integrating
*      the graphics engine within your specific graphics subsystem. In this
*      case you are responsable for the platform specific header and the code 
*      file.
*
*******************************************************************************/

#ifndef EWGFXDEFS_H
#define EWGFXDEFS_H


/* If not explicitly specified, assume following default values for the maximum
   surface size */
#ifndef EW_MAX_SURFACE_WIDTH
  #define EW_MAX_SURFACE_WIDTH   2048
#endif

#ifndef EW_MAX_SURFACE_HEIGHT
  #define EW_MAX_SURFACE_HEIGHT  2048
#endif

/* Validate the maximum surface size */
#if (( EW_MAX_SURFACE_WIDTH < 128 ) || ( EW_MAX_SURFACE_WIDTH   > 4096 ))
  #error "The maximum surface width out of range 128 .. 4096."
#endif

#if (( EW_MAX_SURFACE_HEIGHT < 128 ) || ( EW_MAX_SURFACE_HEIGHT > 4096 ))
  #error "The maximum surface height out of range 128 .. 4096."
#endif


/* The following mode determines how images are stored within the surface pixel
   memory. This setting should always reflect the orientation of the display in
   your device in relation to the orientation of the framebuffer itself. If not
   explicitly specified, assume that all surfaces store the images in their
   natural, not rotated orientation. The mode is expressed in degree. */
#ifndef EW_SURFACE_ROTATION
  #define EW_SURFACE_ROTATION  0
#endif 

/* Validate the surface rotation mode. ONly the discrete values 0, 90, 180 and
   270 degree are allowed. */
#if ( EW_SURFACE_ROTATION != 0   ) && ( EW_SURFACE_ROTATION != 90  ) &&        \
    ( EW_SURFACE_ROTATION != 180 ) && ( EW_SURFACE_ROTATION != 270 )
  #error "The surface rotation can be either 0, 90, 180 or 270 degree."
#endif


/* If not explicitly specified, assume following default value for the maximum
   surface cache size (in bytes) */
#ifndef EW_MAX_SURFACE_CACHE_SIZE
  #define EW_MAX_SURFACE_CACHE_SIZE  0x00800000
#endif

/* Validate the maximum surface cache size */
#if (( EW_MAX_SURFACE_CACHE_SIZE < 0x00000000 ) ||                             \
     ( EW_MAX_SURFACE_CACHE_SIZE > 0x20000000 ))
  #error "The maximum surface cache size out of range."
#endif


/* If not explicitly specified, assume following default modes:

   EW_CACHE_OFFSCREEN_SURFACES == 1 forces the Graphics Engine to maintain
   all dynamically created surfaces in the surface cache. This is useful, 
   when the creation/destruction of surfaces is an expensive task for the
   underlying graphics subsystem. With this mode activated these surfaces are
   handled like any other bitmap resource. They are discarded when the surface
   cache overflows. If this mode is disabled == 0, the off-screen surfaces
   are discarded immediately when they are not used anymore. */
#ifndef EW_CACHE_OFFSCREEN_SURFACES
  #define EW_CACHE_OFFSCREEN_SURFACES  1
#endif

#if EW_CACHE_OFFSCREEN_SURFACES == 0
  #undef EW_CACHE_OFFSCREEN_SURFACES
#endif


/* If not explicitly specified, assume following default mode:

   EW_LAZY_LOAD_BITMAPS = 1 determines that surfaces are created only when
   they are accessed for the first time. This defers the creation and loading
   of the respective bitmaps. This corresponds to the default behavior of
   Embedded Wizard >= 9.30.

   If EW_LAZY_LOAD_BITMAPS = 0, the surfaces are created immediately just in
   the moment when the superior bitmap is created or loaded as resource. This
   corresponds to the default behavior of Embedded Wizard <= 9.20.

   EW_LAZY_LOAD_BITMAPS_IF_ANIMATED_ONLY : Configuring this macro with the value
   1 limits the lazy map operation to surfaces belonging to multi-frame animated
   bitmaps. Other surfaces are loaded immediately memory similarly to version
   <= 9.20. Using this macro implies that EW_DISCARD_BITMAPS_IF_ANIMATED_ONLY
   is also defined with the value 1. */
#ifndef EW_LAZY_LOAD_BITMAPS
  #define EW_LAZY_LOAD_BITMAPS 1
#endif

#ifndef EW_LAZY_LOAD_BITMAPS_IF_ANIMATED_ONLY
  #define EW_LAZY_LOAD_BITMAPS_IF_ANIMATED_ONLY 0
#endif


/* If not explicitly specified, assume following default mode:

   EW_DISCARD_BITMAPS = 1 determines, that the system can discard surfaces if
   the surface cache overflows, the surfaces are not involved in any drawing
   operation and the content of the affected surface can be restored again.
   This corresponds to the default behavior of Embedded Wizard >= 9.30.
   
   If EW_DISCARD_BITMAPS = 0, the surfaces remain in memory as long as they
   are owned by a bitmap. This corresponds to the default behavior of Embedded
   Wizard <= 9.20.

   If the Graphics Engine is configured to automatically discard surfaces (the
   macro EW_DISCARD_BITMAPS == 1), following macros can additionally be used
   to configure the expected discard behavior more precisely:

   EW_DISCARD_BITMAPS_IF_ANIMATED_ONLY : Configuring this macro with the value
   1 limits the discard operation to surfaces belonging to multi-frame animated
   bitmaps. Other surfaces are retained in memory similarly to version <= 9.20.

   EW_DISCARD_BITMAPS_IF_NOT_USED_IN_CURRENT_UPDATE : Configuring this macro
   with the value 1 limits the discard operation to surfaces which have NOT been
   used in the actual screen update cycle.

   EW_DISCARD_BITMAPS_IF_NOT_USED_IN_RECENT_UPDATES : Configuring this macro
   with the value 1 limits the discard operation to surfaces which have NOT been
   used in the actual and in the preceding screen update cycles. */
#ifndef EW_DISCARD_BITMAPS
  #define EW_DISCARD_BITMAPS 1
#endif

#ifndef EW_DISCARD_BITMAPS_IF_ANIMATED_ONLY
  #define EW_DISCARD_BITMAPS_IF_ANIMATED_ONLY 0
#endif

#ifndef EW_DISCARD_BITMAPS_IF_NOT_USED_IN_CURRENT_UPDATE
  #define EW_DISCARD_BITMAPS_IF_NOT_USED_IN_CURRENT_UPDATE 0
#endif

#ifndef EW_DISCARD_BITMAPS_IF_NOT_USED_IN_RECENT_UPDATES
  #define EW_DISCARD_BITMAPS_IF_NOT_USED_IN_RECENT_UPDATES 0
#endif


/* If not explicitly specified, assume following default values for the maximum
   glyph surface size (in pixel). Glyph surface is an alpha8 surface, where all
   rasterized glyphs are stored. It is a kind of cache. */
#ifndef EW_MAX_GLYPH_SURFACE_WIDTH
  #define EW_MAX_GLYPH_SURFACE_WIDTH   512
#endif

#ifndef EW_MAX_GLYPH_SURFACE_HEIGHT
  #define EW_MAX_GLYPH_SURFACE_HEIGHT  512
#endif

/* Validate the maximum glyph surface size */
#if ( EW_MAX_GLYPH_SURFACE_WIDTH < 64 )
  #error "The glyph surface width is less than 64."
#endif

#if ( EW_MAX_GLYPH_SURFACE_WIDTH > EW_MAX_SURFACE_WIDTH )
  #error "The glyph surface size exceeds the maximum surface size."
#endif

#if ( EW_MAX_GLYPH_SURFACE_HEIGHT < 64 )
  #error "The glyph surface height is less than 64."
#endif

#if ( EW_MAX_GLYPH_SURFACE_HEIGHT > EW_MAX_SURFACE_HEIGHT )
  #error "The glyph surface size exceeds the maximum surface size."
#endif


/* If not explicitly specified, assume following default modes:

   EW_BORDER_AROUND_GLYPHS == 1 forces the Graphics Engine to reserve
   additional one pixel wide border around each glyph within the glyph
   cache. This is useful, when glyphs are scaled by the GPU HW. */
#ifndef EW_BORDER_AROUND_GLYPHS
  #define EW_BORDER_AROUND_GLYPHS 0
#endif

#if EW_BORDER_AROUND_GLYPHS == 0
  #undef EW_BORDER_AROUND_GLYPHS
#endif


/* If not explicitly specified, assume following default number of tasks,
   which can wait for execution in a so-called 'issue'. Large number of
   tasks can promote the automatic elimination of drawing tasks. */
#ifndef EW_MAX_ISSUE_TASKS
  #define EW_MAX_ISSUE_TASKS 100
#endif

#if (( EW_MAX_ISSUE_TASKS < 1 ) || ( EW_MAX_ISSUE_TASKS > 8196 ))
  #error "The maximum number of issue tasks out of range 1 .. 8192."
#endif


/* If not explicitly specified, assume following default optimization modes.

   EW_ELIMINATE_TASKS == 1 forces the Graphics Engine to detect and eliminate
   superfluous drawing operations if these are covered by opaque areas. If
   necessary, drawing operations will be clipped or even divided in several
   smaller operations. */
#ifndef EW_ELIMINATE_TASKS
  #define EW_ELIMINATE_TASKS    1
#endif

/* EW_REORDER_TASKS == 1 forces the Graphics Engine to change the order of
   drawing operations in order to put together the operations executed by the
   graphics hardware GPU. This should reduce the synchronization overhead when
   drawing operations are executed alternately by the CPU and GPU. */
#ifndef EW_REORDER_TASKS
  #define EW_REORDER_TASKS      1
#endif

/* Minimum opaque area edge length to perform the elimination of the underlying
   tasks. An opaque drawing operation can force the elimination of underlying
   drawing operations if its edges have at least the given length. Smaller
   opaque areas are ignored by the elimination algorithm. This should reduce
   the elimination overhead. */
#ifndef EW_ELIMINATE_MIN_EDGE
  #define EW_ELIMINATE_MIN_EDGE 16
#endif

#if ( EW_ELIMINATE_MIN_EDGE < 8 )
  #error "The minimum opaque area edge length less than 8."
#endif

#if EW_ELIMINATE_TASKS == 0
  #undef EW_ELIMINATE_TASKS
#endif

#if EW_REORDER_TASKS == 0
  #undef EW_REORDER_TASKS
#endif


/* If not explicitly specified, assume following color premultiplication mode.
   The selected mode should match with the native pixel format of the underlying
   graphics subsystem. If your target calculates with pre-multiplied colors, set
   this macro to 1 in your make file. */
#ifndef EW_PREMULTIPLY_COLOR_CHANNELS
  #define EW_PREMULTIPLY_COLOR_CHANNELS 1
#endif

#if EW_PREMULTIPLY_COLOR_CHANNELS == 0
  #undef EW_PREMULTIPLY_COLOR_CHANNELS
#endif


/* If not explicitly specified, assume following screen update modes to be used
   the Mosaic framework.

   EW_PERFORM_FULLSCREEN_UPDATE == 1 means that the underlying graphics subsystem
   expects the entire screen to be redrawn each time screen update is performed.
   Tipically this is the case of sofisticated 3D hardware accelerated systems or
   if the framebuffer content is not guaranting to be preserved between. In the
   case the macro is 0, only dirty screen areas are redrawn.

   EW_PRESERVE_FRAMEBUFFER_CONTENT == 1 means that the graphics subsystem retains
   the content of the framebuffer between two consecutive screen update frames.

   EW_PERFORM_FULLOFFSCREENBUFFER_UPDATE == 1 means that the underlying graphics
   subsystem expects every off-screen buffer being cleared when drawing in it.
   The effect is similar to the above described EW_PERFORM_FULLSCREEN_UPDATE
   with the difference, that it applies to off-screen buffers only and not to the
   framebuffer itself. */
#ifndef EW_PERFORM_FULLSCREEN_UPDATE
  #define EW_PERFORM_FULLSCREEN_UPDATE 0
#endif

#if ( EW_PERFORM_FULLSCREEN_UPDATE != 0 )
  #undef  EW_PERFORM_FULLSCREEN_UPDATE
  #define EW_PERFORM_FULLSCREEN_UPDATE 1
#endif

#if EW_PERFORM_FULLSCREEN_UPDATE && ( defined EW_PRESERVE_FRAMEBUFFER_CONTENT )
  #undef EW_PRESERVE_FRAMEBUFFER_CONTENT
#endif

#ifndef EW_PRESERVE_FRAMEBUFFER_CONTENT
  #if EW_PERFORM_FULLSCREEN_UPDATE
    #define EW_PRESERVE_FRAMEBUFFER_CONTENT 0
  #else
    #define EW_PRESERVE_FRAMEBUFFER_CONTENT 1
  #endif
#endif

#if ( EW_PRESERVE_FRAMEBUFFER_CONTENT != 0 )
  #undef  EW_PRESERVE_FRAMEBUFFER_CONTENT
  #define EW_PRESERVE_FRAMEBUFFER_CONTENT   1
#endif

#ifndef EW_PERFORM_FULLOFFSCREENBUFFER_UPDATE
  #define EW_PERFORM_FULLOFFSCREENBUFFER_UPDATE 0
#endif

#if ( EW_PERFORM_FULLOFFSCREENBUFFER_UPDATE != 0 )
  #undef  EW_PERFORM_FULLOFFSCREENBUFFER_UPDATE
  #define EW_PERFORM_FULLOFFSCREENBUFFER_UPDATE 1
#endif


#endif /* EWGFXDEFS_H */


/* pba */
