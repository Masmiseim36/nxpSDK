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
*   This file contains general configuration settings for the target system,
*   like memory ranges and display parameter and configuration settings for
*   the Embedded Wizard Graphics Engine and Runtime Environment.
*
*******************************************************************************/

#include "ewdef.h"


#ifndef EWCONFIG_H
#define EWCONFIG_H


/* ******************************************************************************
   Following macros configure the application.

   PLATFORM_STRING - This macro contains the name of the development platform.

   SDRAM_BASE_ADDR, SDRAM_SIZE_BYTES - These macros define the start address and
   the size of the SDRAM that can be used for framebuffer and heap manager.

   EW_USE_TERMINAL_INPUT - Flag to switch on/off the support of keyboard events
   received from a connected serial terminal.

   EW_CPU_LOAD_MEASURING - Flag to switch on/off the measuring of the CPU load
   caused by the GUI application and all graphics operations. The CPU load can
   be read by using the function EwBspClockGetCpuLoad().

   RTC_MINIMUM_TIME, RTC_DEFAULT_TIME - Macros to define the initialization value
   of the realtime clock. The value RTC_DEFAULT_TIME is used as start value for
   the RTC in case that it is not already initialized with a value higher than
   RTC_MINIMUM_TIME.
   **************************************************************************** */
#define PLATFORM_STRING       "IMXRT1170-EVK"

#define SDRAM_BASE_ADDR       0x80000000
#define SDRAM_SIZE_BYTES      (48 * 1024 * 1024)

#define EW_USE_TERMINAL_INPUT 0
#define EW_CPU_LOAD_MEASURING 1

#define RTC_MINIMUM_TIME      978307200      /* 2001-01-01 0:00 */
#define RTC_DEFAULT_TIME      1605193200     /* 2020-11-12 15:00 */


/* ******************************************************************************
   Following macros configure the display integration, the framebuffer access and
   the usage of the graphics accelerator.
   For details about different framebuffer concepts please have a look to
   https://doc.embedded-wizard.de/framebuffer-concepts

   EW_USE_SCRATCHPAD_BUFFER - Flag to switch on/off the usage of a small
   scratch-pad buffer used for the graphics composition instead of a framebuffer.
   As a result the screen of the display is updated in partial mode.
   This mode is used to reduce the memory footprint to a minimum, but it may cause
   some tearing or flickering effects. This mode is useful in combination with
   displays containing its own graphics memory (GRAM).
   Please note, that this option is not supported for each display integration.

   EW_USE_DOUBLE_BUFFER - Flag to switch on/off the usage of an additional
   framebuffer, so that the screen is operated in double-buffering mode.
   If double-buffering is activated, the exchange between front-buffer and
   back-buffer has to be done at V-sync. This has to be implemented in the
   function EwBspDsipalyCommitBuffer().
   In case of a single buffered configuration, the graphics composition is done
   directly into the framebuffer. This makes only sense, if you are using an LCD
   with its own pixelbuffer (e.g. some MIPI-DSI displays). Otherwise, the
   composition of the framebuffer content has to be synchronized with the
   LCD update in order to avoid flickering.

   EW_USE_OFFSCREEN_BUFFER - Flag to switch on/off the usage of an additional
   off-screen buffer. This is an internal buffer, allocated by the Graphics Engine
   and used to prepare the graphical content (composition buffer).
   The usage of an separate off-screen buffer might be advantageous on systems
   where the read access from the physical framebuffer is very slow. In this
   case, the off-screen buffer is used for composition (read and write access)
   and the final graphics is then copied into the framebuffer (write access).
   Please note, that this option is not supported for each display integration.

   FRAME_BUFFER_WIDTH and FRAME_BUFFER_HEIGHT - This macros define the physical
   size of the framebuffer in pixel. Typically this size corresponds to the
   attribute ScreenSize defined within the profile of the GUI application.

   FRAME_BUFFER_ADDR, FRAME_BUFFER_SIZE - These macros define the start address
   and the size (in bytes) of the framebuffer.

   DOUBLE_BUFFER_ADDR, DOUBLE_BUFFER_SIZE - These macros define the start address
   and the size (in bytes) of the optional double-buffer.

   EW_USE_GRAPHICS_ACCELERATOR - Flag to switch on/off the usage of the graphics
   accelerator within the target (if available).
   Please note: Changing this macro requires the complete source code of the
   Graphics Engine to be recompiled with the new setting. As such it works only
   for customers who have licensed the 'Professional' edition.
   **************************************************************************** */
#define EW_USE_DOUBLE_BUFFER            1

#define FRAME_BUFFER_WIDTH              720
#define FRAME_BUFFER_HEIGHT             1280

/* calculated addresses for framebuffer(s) */
#define FRAME_BUFFER_ADDR     SDRAM_BASE_ADDR
#define FRAME_BUFFER_SIZE     FRAME_BUFFER_WIDTH * FRAME_BUFFER_HEIGHT * FRAME_BUFFER_DEPTH

#if EW_USE_DOUBLE_BUFFER == 1

  #define DOUBLE_BUFFER_ADDR  (FRAME_BUFFER_ADDR + FRAME_BUFFER_SIZE)
  #define DOUBLE_BUFFER_SIZE  FRAME_BUFFER_SIZE

#else

  #define DOUBLE_BUFFER_ADDR  0
  #define DOUBLE_BUFFER_SIZE  0
  #define NUMBER_OF_FIELDS    3

#endif

#define EW_USE_GRAPHICS_ACCELERATOR     1


/* ******************************************************************************
   Following macros configure the memory area used for the Embedded Wizard heap
   manager. Optionally, an additional extra memory pool can be defined.

   EW_MEMORY_POOL_SECTION - This macro has to be defined if the memory pool shall
   be declared as an array. Optionally a linker section name can be assigned to
   this macro to define where this array shall be located by the linker.
   Alternatively, a fixed memory address can be defined by setting the macro
   EW_MEMORY_POOL_ADDR.

   EW_MEMORY_POOL_ADDR - This macro defines the start address of the memory pool
   to a fixed address. Take care that the memory area is not used by the linker.
   Do not use this define in combination with EW_MEMORY_POOL_SECTION.

   EW_MEMORY_POOL_SIZE - This macro defines the size of the memory pool in bytes
   used for the Embedded Wizard heap manager.

   EW_EXTRA_POOL_SECTION, EW_EXTRA_POOL_ADDR and EW_EXTRA_POOL_SIZE - These
   macros are used to define a second (additional) memory pool.
   **************************************************************************** */
#define EW_MEMORY_POOL_ADDR      (FRAME_BUFFER_ADDR + FRAME_BUFFER_SIZE + DOUBLE_BUFFER_SIZE)
#define EW_MEMORY_POOL_SIZE      SDRAM_SIZE_BYTES - FRAME_BUFFER_SIZE - DOUBLE_BUFFER_SIZE

#define EW_EXTRA_POOL_ADDR       0
#define EW_EXTRA_POOL_SIZE       0


/* ******************************************************************************
   Following macros configure advance aspects of an Embedded Wizard application
   used for profiling, tracing or optimization purpose. In order to enable the
   functionality uncomment the respective macro definition below.

   EW_PRINT_MEMORY_USAGE - If this macro is defined, a statistic information is
   printed reporting the amount of memory allocated by Chora objects, strings and
   resources. Additionally, the current total memory allocation and the maximum
   peak value is printed.

   EW_DUMP_HEAP - If this macro is defined, the contents of all memory pools are
   evaluated and the associated information as well as the existing blocks are
   reported.

   IMPORTANT : activating the following macros requires the complete source code
   of the Graphics Engine to be recompiled with the new setting. As such it works
   for customers who have licensed the 'Professional' edition only. For customers
   using the 'Small Business' edition or users working with the Free edition, the
   macros will not have any effect!

   EW_SUPPORT_GFX_TASK_TRACING - If this macro is defined, the functions of the
   Graphics Engine are enhanced by code to track and protocol all drawing tasks
   executed at the runtime. Once the functionality is available, you control the
   outputs of the traced drawing operations by using the EwEnableGfxTaskTracing()
   function.

   EW_PRINT_GFX_TASKS - Defining this macro activates the above described tracing
   of drawing tasks as if the macro EW_SUPPORT_GFX_TASK_TRACING were defined and
   the function EwEnableGfxTaskTracing() to enable the trace outputs were called.
   This is a convenience macro to simply activate the tracing without needing to
   modify your application code by additional function calls.

   EW_PRINT_PERF_COUNTERS - If this macro is defined, all essential functions of
   the Graphics Engine and Runtime Environment are enhanced by additional code to
   to track and measure their runtime. To display this collected information you
   use the function EwPrintPerfCounters().

   EW_USE_IMMEDIATE_GARBAGE_COLLECTION - If this macro is defined, the process of
   detection and disposal of unused Chora objects is allowed to run at any time,
   even when this would interfere with the actually executed Chora code. If this
   macro is not defined, the Garbage Collection is limited to run when no Chora
   code is executed only. Using this feature expects, that your application uses
   the function EwSetStackBaseAddress() to inform the Garbage Collector about the
   memory area occupied by the CPU stack.
   **************************************************************************** */
// #define EW_PRINT_MEMORY_USAGE
// #define EW_DUMP_HEAP

// #define EW_SUPPORT_GFX_TASK_TRACING
// #define EW_PRINT_GFX_TASKS
// #define EW_PRINT_PERF_COUNTERS
// #define EW_USE_IMMEDIATE_GARBAGE_COLLECTION


/* ******************************************************************************
   Following macros configure the size of diverse caches and buffers. Generally,
   the larger a cache or buffer, the better the performance of the application.
   On the other hand, large caches and buffers will occupy more RAM. Configure
   the macros prudently according to the available RAM and expected performance.

   EW_MAX_STRING_CACHE_SIZE - This macro specifies the threshold value for the
   capacity of the string cache. The string cache stores frequently used string
   constants preventing them from being reloaded repeatedly. Embedded Wizard
   tracks the amount of memory occupied by all string constants and as soon as
   this size exceeds the value specified in the macro, discards oldest unused
   strings from the cache automatically. The value for this macro is expressed
   in bytes as an integer number lying between 0 and 0x800000.

   EW_MAX_SURFACE_CACHE_SIZE - This macro specifies the threshold value for the
   capacity of the surface cache. This cache stores often used bitmap resources
   and off-screen image buffers preventing them from being reloaded or recreated
   repeatedly. Embedded Wizard tracks the amount of memory occupied by all cached
   surfaces and as soon as this size exceeds the value specified in the macro,
   discards oldest unused surfaces from the cache automatically. The value for
   this macro is expressed in bytes as an integer number lying between 0 and
   0x20000000.

   EW_MAX_GLYPH_SURFACE_WIDTH, EW_MAX_GLYPH_SURFACE_HEIGHT - These macros specify
   the dimension of the glyph cache surface. This surface stores all glyphs used
   during text drawing operations. The larger the surface the more glyphs can be
   reused without needing to decompress or rasterize them again. The values for
   the macros are expressed in pixel as integer numbers lying between 64 and the
   maximum surface size supported on the graphics hardware. You should ensure,
   that the glyph surface is at least as large as the biggest glyph you want to
   display at the runtime. The following quotation can be used to calculate the
   approximated RAM usage resulting from the configuration of the macros:
   ( EW_MAX_GLYPH_SURFACE_WIDTH * EW_MAX_GLYPH_SURFACE_HEIGHT ) * 1 Byte.

   EW_MAX_ISSUE_TASKS - This macro specifies the capacity of instruction buffers
   used to store drawing operations. Larger buffers improve the optimization and
   elimination of superfluous drawing operations. In turn, small buffers cause
   the drawing process to be often interrupted to flush the prepared operations.
   The value for this macro is expressed in 'tasks' as an integer number lying
   between 1 and 8192. You should ensure that the issue buffer is at least as
   large as the most complex drawing operation performed in your application.
   The following quotation can be used to calculate the approximated RAM usage
   resulting from the configuration of the macro:
   (( EW_MAX_ISSUE_TASKS * 64 ) + 512 ) * 1 Byte.
   **************************************************************************** */
#define EW_MAX_STRING_CACHE_SIZE      0x4000
#define EW_MAX_SURFACE_CACHE_SIZE   0x800000
#define EW_MAX_GLYPH_SURFACE_WIDTH       512
#define EW_MAX_GLYPH_SURFACE_HEIGHT      512
#define EW_MAX_ISSUE_TASKS               256


/* ******************************************************************************
   Following macros configure the behavior of the surface cache and treatment
   of bitmap resources when these are generated with in the 'compressed' mode.
   Depending on the configuration, the application will require more or less RAM,
   which affects accordingly the resulting performance.

   EW_LAZY_LOAD_BITMAPS - This macro controls the loading behavior of compressed
   bitmap resources. If this macro is defined with the value 0, the resources are
   loaded immediately and completely into RAM as soon as the application accesses
   them. Thereupon the bitmaps remain in the surface cache (see above). If this
   macro is defined with the value 1, the loading of bitmaps is delayed until the
   application is about to display one frame of the bitmap. In such case this and
   only this frame is loaded into RAM.

   EW_LAZY_LOAD_BITMAPS_IF_ANIMATED_ONLY - This macro, if defined with the value
   1, does limit the effect of the above described macro EW_LAZY_LOAD_BITMAPS to
   resources containing animated bitmaps only. All other bitmaps (single or multi
   frame but not animated) are loaded immediately regardless of the actual value
   for the EW_LAZY_LOAD_BITMAPS macro. If this macro is 0, EW_LAZY_LOAD_BITMAPS
   affects all kinds of bitmap resources.

   EW_DISCARD_BITMAPS - This macro controls the lifetime of bitmaps stored in the
   surface cache (see above). If this macro is configured with the value 1, the
   system is allowed to discard bitmap resources even if these are still in use.
   As soon as the surface cache has exceeded the threshold value specified in the
   macro EW_MAX_SURFACE_CACHE_SIZE and there is no other unused surfaces ready to
   discard from the cache, the system discards the lazy loaded bitmap resources.
   Due to this 'swapping' the application can work with significantly less RAM.
   If this macro is 0, the discarding affects only such surfaces which are really
   not in use anymore.

   EW_DISCARD_BITMAPS_IF_ANIMATED_ONLY - This macro, if defined with the value 1,
   limits the effect of the above described macro EW_DISCARD_BITMAPS to surfaces
   containing animated bitmap resources only. All other bitmaps (single or multi
   frame but not animated) are treated as if the macro EW_DISCARD_BITMAPS were 0.
   If EW_DISCARD_BITMAPS_IF_ANIMATED_ONLY is 0, EW_DISCARD_BITMAPS affects all
   kinds of bitmap in similar manner.

   EW_DISCARD_BITMAPS_IF_NOT_USED_IN_CURRENT_UPDATE - This macro, if defined with
   the value 1, limits the effect of the above described macro EW_DISCARD_BITMAPS
   to surfaces, which have not been used in actual screen update. This prevents
   the system from reloading one the same bitmap resource multiple times during
   the running screen update. If this macro is 0, EW_DISCARD_BITMAPS affects all
   bitmap resources in similar manner.

   EW_DISCARD_BITMAPS_IF_NOT_USED_IN_RECENT_UPDATES - This macro, if defined with
   the value 1, limits the effect of the above described macro EW_DISCARD_BITMAPS
   to surfaces, which have not been used in actual and the immediately preceding
   screen updates. This prevents the system from reloading one the same bitmap
   resource multiple times during running animations. If this macro is 0, the
   macro EW_DISCARD_BITMAPS affects all bitmap resources in similar manner.

   EW_CACHE_OFFSCREEN_SURFACES - This macro controls how to manage surfaces used
   to buffer off-screen contents (e.g. used by a buffered GUI component). If this
   macro is defined with the value 1, the off-screen surfaces are retained in the
   surface cache (see above). Accordingly, frequently used off-screen surfaces
   can be reused faster without needing to create them again. Defining the macro
   with the value 0 disables this function causing the off-screen surfaces to be
   released immediately if not needed anymore.
   **************************************************************************** */
#define EW_LAZY_LOAD_BITMAPS                              1
#define EW_LAZY_LOAD_BITMAPS_IF_ANIMATED_ONLY             1
#define EW_DISCARD_BITMAPS                                1
#define EW_DISCARD_BITMAPS_IF_ANIMATED_ONLY               1
#define EW_DISCARD_BITMAPS_IF_NOT_USED_IN_CURRENT_UPDATE  0
#define EW_DISCARD_BITMAPS_IF_NOT_USED_IN_RECENT_UPDATES  0
#define EW_CACHE_OFFSCREEN_SURFACES                       0


/* ******************************************************************************
   Following macro removes functionality dedicated to verify the correct function
   of the application. Removing this functionality makes sense only if you have
   finalized the development and ensured that your application works correctly.
   In order to remove the functionality uncomment the respective macro definition
   below.

   EW_DONT_CHECK_INDEX - If this macro is defined, the code generated by Embedded
   Wizard (your application code) does not perform any bounds checking when array
   items are addressed. If this macro is not defined, addressing items outside of
   an array will result in an adequate error message. Performing bounds checking
   may affect the performance if the applications uses arrays extensively.
   **************************************************************************** */
// #define EW_DONT_CHECK_INDEX


/* ******************************************************************************
   Following macros removes function blocks from the Graphics Engine reducing so
   the code size of the resulting binary. Removing functionality makes sense only
   if you are sure that it is not needed by your application. If the application
   nevertheless uses the functionality, a runtime error is reported. In order to
   remove a function block just uncomment the respective macro definition below.

   IMPORTANT : activating the following macros requires the complete source code
   of the Graphics Engine to be recompiled with the new setting. As such it works
   for customers who have licensed the 'Professional' edition only. For customers
   using the 'Small Business' edition or users working with the Free edition, the
   macros will not have any effect!

   EW_DONT_USE_WARP_FUNCTIONS - If this macro is defined, all functions needed to
   calculate and display transformed image contents are removed from the binary.
   In particular, it disables the possibility to scale and rotate images as well
   as their perspective projection resulting in 3D visual effects.

   EW_DONT_USE_PATH_FUNCTIONS - If this macro is defined, all functions needed to
   calculate and raster vector graphic (to display polygons and paths) is removed
   from the binary.

   EW_DONT_USE_BIDI_FUNCTIONS - If this macro is defined, all functions needed to
   process bidirectional text, as it is usually required to display right-to-left
   Arabic or Hebrew scripts, are removed.

   EW_DONT_USE_INDEX8_SURFACES - If this macro is defined, the support for INDEX8
   bitmap format is removed. Thereupon creation, loading and displaying of images
   stored in INDEX8 format is not possible anymore.

   EW_DONT_USE_RGB565_SURFACES - If this macro is defined, the support for RGB565
   bitmap format is removed. Thereupon creation, loading and displaying of images
   stored in RGB565 format is not possible anymore.
   **************************************************************************** */
// #define EW_DONT_USE_WARP_FUNCTIONS
// #define EW_DONT_USE_PATH_FUNCTIONS
// #define EW_DONT_USE_BIDI_FUNCTIONS
// #define EW_DONT_USE_INDEX8_SURFACES
// #define EW_DONT_USE_RGB565_SURFACES


#endif /* EWCONFIG_H */

/* pba, mli, msy */

