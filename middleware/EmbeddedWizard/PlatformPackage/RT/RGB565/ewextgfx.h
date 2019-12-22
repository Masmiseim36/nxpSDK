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
*   This header file contains Graphics Engine configuration parameters and the
*   adaptation for the NxpRt graphics subsystem.
*
*******************************************************************************/

#ifndef EWEXTGFX_H
#define EWEXTGFX_H

//#ifdef __UVISION_VERSION
//#include "RTE_Components.h"
//#endif


#ifdef __cplusplus
  extern "C"
  {
#endif

/* Defines for different framebuffer color formats */
#define EW_FRAME_BUFFER_COLOR_FORMAT_Index8   1
#define EW_FRAME_BUFFER_COLOR_FORMAT_LumA44   2
#define EW_FRAME_BUFFER_COLOR_FORMAT_RGB565   3
#define EW_FRAME_BUFFER_COLOR_FORMAT_RGB888   4
#define EW_FRAME_BUFFER_COLOR_FORMAT_RGBA4444 5
#define EW_FRAME_BUFFER_COLOR_FORMAT_RGBA8888 6


/*
   The color format of the framebuffer has to correspond to color format of the
   Graphics Engine - otherwise the Graphics Engine cannot draw directly into
   this framebuffer, because the Graphics engine is prepared and optimized for
   one dedicated color format.

   The color format of the LCD / framebuffer can be defined within the makefile
   by setting the macro EW_FRAME_BUFFER_COLOR_FORMAT.

   According your makefile settings, the resulting color depth is determined.
*/
#if ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGB565 )
  #define FRAME_BUFFER_DEPTH    2
#else
  #error The given EW_FRAME_BUFFER_COLOR_FORMAT is not supported! Use RGB565 within your makefile!
#endif


/*
  EW_USE_DOUBLE_BUFFER - Flag to switch on/off the usage of an additional
  framebuffer, so that the screen is operated in double-buffering mode.
  Per default, the usage of double-buffering is disabled. To activate the
  double-buffering mode, please set the macro EW_USE_DOUBLE_BUFFER to 1
  within your makefile. This can be achieved by using the compiler flag
  -DEW_USE_DOUBLE_BUFFER=1
  If double-buffering is activated, the exchange between front-buffer and
  back-buffer has to be done at V-sync. This has to be implemented in the
  extern function EwBspSetFramebufferAddress().
*/
#ifndef EW_USE_DOUBLE_BUFFER
  #define EW_USE_DOUBLE_BUFFER 0
#endif

#if EW_USE_DOUBLE_BUFFER == 0
  #undef EW_USE_DOUBLE_BUFFER
#endif


/*
  EW_USE_SCRATCHPAD_BUFFER - Flag to switch on/off the usage of a small
  scratch-pad buffer used for the graphics composition instead of a framebuffer.
  As a result the screen of the display is updated in partial mode.
  This mode is used to reduce the memory footprint to a minimum, but it may cause
  some tearing or flickering effects. This mode is useful in combination with
  displays containing its own graphics memory (GRAM).
  Per default, the usage of scratch-pad buffer is disabled. To activate the
  scratch-pad buffer mode, please set the macro EW_USE_SCRATCHPAD_BUFFER to 1
  within your makefile. This can be achieved by using the compiler flag
  -DEW_USE_SCRATCHPAD_BUFFER=1
*/
#ifndef EW_USE_SCRATCHPAD_BUFFER
  #define EW_USE_SCRATCHPAD_BUFFER 0
#endif

#if EW_USE_SCRATCHPAD_BUFFER == 0
  #undef EW_USE_SCRATCHPAD_BUFFER
#endif


/*
  EW_USE_PXP_GRAPHICS_ACCELERATOR - Flag to switch on/off the usage of the
  graphics accelerator within the NxpRt target (PXP).
  Per default, the usage of the PXP is enabled. To switch off the usage of
  the PXP, please set the macro EW_USE_PXP_GRAPHICS_ACCELERATOR to 0 within
  your makefile. This can be achieved by using the compiler flag
  -DEW_USE_PXP_GRAPHICS_ACCELERATOR=0
*/
#ifndef EW_USE_PXP_GRAPHICS_ACCELERATOR
  #define EW_USE_PXP_GRAPHICS_ACCELERATOR 1
#endif

#if EW_USE_PXP_GRAPHICS_ACCELERATOR == 0
  #undef EW_USE_PXP_GRAPHICS_ACCELERATOR
#endif


/* The following macros override the default color channel allocation to the
   order (bit31) A..R..G..B (bit0). If your graphics hardware supports other
   color channel order, you can define the following macros in your make file. */
#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_RED
  #define EW_COLOR_CHANNEL_BIT_OFFSET_RED    16
#endif

#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_GREEN
  #define EW_COLOR_CHANNEL_BIT_OFFSET_GREEN   8
#endif

#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_BLUE
  #define EW_COLOR_CHANNEL_BIT_OFFSET_BLUE    0
#endif

#ifndef EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA
  #define EW_COLOR_CHANNEL_BIT_OFFSET_ALPHA  24
#endif


/* NxpRt Pixelpipeline does not operate with premultiplied colors */
#define EW_PREMULTIPLY_COLOR_CHANNELS         0


/* The RGB565 Platform Package uses a screen color format when drawing into the
   framebuffer - internally created buffers will use the RGBA8888 color format
   in order to achive best quality and to support alpha channel. */
#define EW_USE_PIXEL_FORMAT_SCREEN


/* The following macros override the default color channel allocation to the
   order (bit15) R..G..B (bit0). If your graphics hardware supports other
   color channel order, you can define the following macros in your make file. */
#ifndef EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_RED
  #define EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_RED    11
#endif

#ifndef EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_GREEN
  #define EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_GREEN   5
#endif

#ifndef EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_BLUE
  #define EW_SCREEN_COLOR_CHANNEL_BIT_OFFSET_BLUE    0
#endif


/*******************************************************************************
* FUNCTION:
*   NxpRtInitGfx
*
* DESCRIPTION:
*   The function NxpRtInitGfx is called from the Graphics Engine during the
*   initialization in order to make target specific configurations of the
*   Graphics Engine
*
* ARGUMENTS:
*   aArgs - Optional argument passed to the Graphics Engine init function.
*
* RETURN VALUE:
*   If successful, returns != 0.
*
*******************************************************************************/
int NxpRtInitGfx
(
  void*             aArgs
);


/*******************************************************************************
* FUNCTION:
*   NxpRtInitViewport
*
* DESCRIPTION:
*   The function NxpRtInitViewport is called from the Graphics Engine,
*   to create a new viewport on the target. The function uses the given
*   buffers passed in the arguments aDisplay1, aDisplay2 and aDisplay3.
*
* ARGUMENTS:
*   aWidth,
*   aHeight       - Size of the application in pixel.
*   aExtentX,
*   aExtentY      - not used.
*   aExtentWidth,
*   aExtentHeight - Size of the physical or virtual framebuffer in pixel.
*   aOrient       - not used.
*   aOpacity      - not used.
*   aDisplay1     - Address of the framebuffer / front-buffer.
*   aDisplay2     - Address of the back-buffer in case of double-buffering.
*   aDisplay3     - Address of the scratch-pad buffer.
*
* RETURN VALUE:
*   Handle of the surface descriptor (viewport).
*
*******************************************************************************/
unsigned long NxpRtInitViewport
(
  int               aWidth,
  int               aHeight,
  int               aExtentX,
  int               aExtentY,
  int               aExtentWidth,
  int               aExtentHeight,
  int               aOrient,
  int               aOpacity,
  void*             aDisplay1,
  void*             aDisplay2,
  void*             aDisplay3
);


/*******************************************************************************
* FUNCTION:
*   NxpRtDoneViewport
*
* DESCRIPTION:
*   The function NxpRtDoneViewport is called from the Graphics Engine, to
*   release a previously created viewport on the NxpRt target.
*
* ARGUMENTS:
*   aHandle - Handle of the surface descriptor (viewport).
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void NxpRtDoneViewport
(
  unsigned long     aHandle
);


/*******************************************************************************
* FUNCTION:
*   NxpRtBeginUpdate
*
* DESCRIPTION:
*   The function NxpRtBeginUpdate is called from the Graphics Engine, to
*   initiate the screen update cycle.
*
* ARGUMENTS:
*   aHandle - Handle of the surface descriptor (viewport).
*
* RETURN VALUE:
*   Handle of the destination surface, used for all drawing operations.
*
*******************************************************************************/
unsigned long NxpRtBeginUpdate
(
  unsigned long     aHandle
);


/*******************************************************************************
* FUNCTION:
*   NxpRtBeginUpdateArea
*
* DESCRIPTION:
*   The function NxpRtBeginUpdateArea is called from the Graphics Engine, to
*   initiate a partial screen update cycle.
*
* ARGUMENTS:
*   aHandle - Handle of the surface descriptor (viewport).
*   aX,
*   aY,
*   aWidth,
*   aHeight - Position and size of the area affected by the screen update
*     (dirty rectangle).
*
* RETURN VALUE:
*   Handle of the destination surface, used for all drawing operations.
*
*******************************************************************************/
unsigned long NxpRtBeginUpdateArea
(
  unsigned long     aHandle,
  int               aX,
  int               aY,
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   NxpRtEndUpdate
*
* DESCRIPTION:
*   The function NxpRtEndUpdate is called from the Graphics Engine, to
*   finalize the screen update cycle.
*
* ARGUMENTS:
*   aHandle - Handle of the surface descriptor (viewport).
*   aX,
*   aY,
*   aWidth,
*   aHeight - Position and size of the area affected from the screen update
*     (dirty rectangle).
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void NxpRtEndUpdate
(
  unsigned long     aHandle,
  int               aX,
  int               aY,
  int               aWidth,
  int               aHeight
);


/* Redirect the following operations to the functions within this module */
#define EwGfxInit            NxpRtInitGfx
#define EwGfxInitViewport    NxpRtInitViewport
#define EwGfxDoneViewport    NxpRtDoneViewport
#define EwGfxBeginUpdate     NxpRtBeginUpdate
#define EwGfxBeginUpdateArea NxpRtBeginUpdateArea
#define EwGfxEndUpdate       NxpRtEndUpdate


/*******************************************************************************
* FUNCTION:
*   NxpRtCreateSurface
*
* DESCRIPTION:
*   The function NxpRtCreateSurface() reserves pixel memory for a new surface
*   with the given size and color format. The function returns a handle to the
*   new surface.
*
* ARGUMENTS:
*   aFormat  - Color format of the surface. (See EW_PIXEL_FORMAT_XXX).
*   aWidth,
*   aHeight  - Size of the surface in pixel to create.
*
* RETURN VALUE:
*   The function returns a handle to the created surface. This can be a pointer
*   to a dynamically allocated data structure, an index in a list of surfaces,
*   or a handle returned by the lower level API.
*
*   If the creation is failed, the function should return 0.
*
*******************************************************************************/
unsigned long NxpRtCreateSurface
(
  int               aFormat,
  int               aWidth,
  int               aHeight
);


/*******************************************************************************
* FUNCTION:
*   NxpRtCreateConstSurface
*
* DESCRIPTION:
*   The function NxpRtCreateConstSurface() creates a surface structure
*   that refers to a constant pixel memory. The function returns a handle to the
*   new surface.
*
* ARGUMENTS:
*   aFormat - Color format of the surface. (See EW_PIXEL_FORMAT_XXX).
*   aWidth,
*   aHeight - Size of the surface in pixel.
*   aMemory - Pointer to constant pixel memory.
*
* RETURN VALUE:
*   The function returns a handle to the created surface.
*   If the creation is failed, the function should return 0.
*
*******************************************************************************/
unsigned long NxpRtCreateConstSurface
(
  int               aFormat,
  int               aWidth,
  int               aHeight,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   NxpRtDestroySurface
*
* DESCRIPTION:
*   The function NxpRtDestroySurface() frees the resources of the given surface.
*   This function is a counterpart to NxpRtCreateSurface().
*
* ARGUMENTS:
*   aHandle - Handle to the surface to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void NxpRtDestroySurface
(
  unsigned long     aHandle
);


/*******************************************************************************
* FUNCTION:
*   NxpRtLockSurface
*
* DESCRIPTION:
*   The function LockSurface() provides a direct access to the pixel memory of
*   the given surface. The function returns a lock object containing pointers to
*   memory, where the caller can read/write the surface pixel values. Additional
*   pitch values also returned in the object allow the caller to calculate the
*   desired pixel addresses.
*
*   When finished the access cycle, the function UnlockSurface() should be used
*   in order to release the lock, update the affected surface, flush CPU caches,
*   etc.
*
* ARGUMENTS:
*   aHandle     - Handle to the surface to obtain the direct memory access.
*   aX, aY,
*   aWidth,
*   aHeight     - Area within the surface affected by the access operation.
*     (Relative to the top-left corner of the surface). This is the area, the
*     caller wish to read/write the pixel data.
*   aIndex,
*   Count       - Optional start index and number of entries within the CLUT,
*     the caller wish to read/write. These paramaters are used for surfaces
*     with the index8 color format only.
*   aReadPixel  - Is != 0, if the caller intends to read the pixel information
*     from the surface memory. If == 0, the memory content may remain undefined
*     depending on the underlying graphics sub-system and its video-memory
*     management.
*   aWritePixel - Is != 0, if the caller intends to modify the pixel information
*     within the surface memory. If == 0, any modifications within the memory
*     may remain ignored depending on the underlying graphics sub-system and its
*     video-memory management.
*   aReadClut   - Is != 0, if the caller intends to read the CLUT information.
*     If == 0, the CLUT content may remain undefined.
*   aWriteClut  - Is != 0, if the caller intends to modify the CLUT information.
*     If == 0, any modifications within the memory may remain ignored depending
*     on the underlying graphics sub-system and its video-memory management.
*   aMemory     - Pointer to an object, where the desired surface pointers
*     should be stored.
*
* RETURN VALUE:
*   If successful, the function should return a kind of a lock object. This
*   object can contain additional information needed when the surface is
*   unlocked again. If you don't want to return additional information, return
*   any value != 0.
*
*   If there was not possible to lock the surface, or the desired access mode
*   is just not supported by the underlying graphics sub-system, the function
*   fails and returns zero. (e.g. OpenGL based sub-systems usually allow the
*   write access to surfaces (textures) only. Read access may fail in this
*   case).
*
*******************************************************************************/
unsigned long NxpRtLockSurface
(
  unsigned long     aHandle,
  int               aX,
  int               aY,
  int               aWidth,
  int               aHeight,
  int               aIndex,
  int               aCount,
  int               aReadPixel,
  int               aWritePixel,
  int               aReadClut,
  int               aWriteClut,
  XSurfaceMemory*   aMemory
);


/*******************************************************************************
* FUNCTION:
*   NxpRtUnlockSurface
*
* DESCRIPTION:
*   The function NxpRtUnlockSurface() has the job to unlock the given surface and
*   if necessary free any temporary used resources.
*   This function is a counterpart to NxpRtLockSurface().
**
* ARGUMENTS:
*   aSurfaceHandle - Handle to the surface to release the direct memory access.
*   aLockHandle    - value returned by the corresponding LockSurface() call.
*     If LockSurface() has allocated memory for the lock object, you will need
*     to free it now.
*   aX, aY,
*   aWidth,
*   aHeight     - Area within the surface affected by the access operation.
*     (Relative to the top-left corner of the surface). This is the area, the
*     caller wished to read/write the pixel data.
*   aIndex,
*   Count       - Optional start index and number of entries within the CLUT,
*     the caller wished to read/write. These paramaters are used for surfaces
*     with the index8 color format only.
*   aWritePixel - Is != 0, if the caller has modified the pixel information
*     within the surface memory. If == 0, no modification took place, so no
*     surface updates are needed.
*   aWriteClut  - Is != 0, if the caller has modified the CLUT information.
*     If == 0, no modification took place, so no surface updates are needed.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void NxpRtUnlockSurface
(
  unsigned long     aSurfaceHandle,
  unsigned long     aLockHandle,
  int               aX,
  int               aY,
  int               aWidth,
  int               aHeight,
  int               aIndex,
  int               aCount,
  int               aWritePixel,
  int               aWriteClut
);


/* Macros to redirect the Graphics Engine operations to the above functions. */
#define EwGfxCreateNativeSurface      NxpRtCreateSurface
#define EwGfxCreateConstNativeSurface NxpRtCreateConstSurface
#define EwGfxDestroyNativeSurface     NxpRtDestroySurface
#define EwGfxLockNativeSurface        NxpRtLockSurface
#define EwGfxUnlockNativeSurface      NxpRtUnlockSurface

#define EwGfxCreateRGB565Surface      NxpRtCreateSurface
#define EwGfxCreateConstRGB565Surface NxpRtCreateConstSurface
#define EwGfxDestroyRGB565Surface     NxpRtDestroySurface
#define EwGfxLockRGB565Surface        NxpRtLockSurface
#define EwGfxUnlockRGB565Surface      NxpRtUnlockSurface

#define EwGfxLockScreenSurface        NxpRtLockSurface
#define EwGfxUnlockScreenSurface      NxpRtUnlockSurface


/*******************************************************************************
* PROTOTYPE:
*   NxpRtFillDriver
*
* DESCRIPTION:
*   The following function performs the 'fill rectangular area' operation by
*   using solid or gradient color values.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the destination surface (native/screen color format).
*      See the function CreateSurface().
*   aDstX,
*   aDstY       - Origin of the area to fill (relative to the top-left corner
*      of the destination surface).
*   aWidth,
*   aHeight     - Size of the area to fill.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 4 RGBA8888 color values. The four color values do
*     correspond to the four corners of the area: top-left, top-right, bottom-
*     right and bottom-left.
*     If all colors are equal, the solid variant of the operation is assumed.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void NxpRtFillDriver
(
  unsigned long     aDstHandle,
  int               aDstX,
  int               aDstY,
  int               aWidth,
  int               aHeight,
  int               aBlend,
  unsigned long*    aColors
);


/* Macros to redirect the Graphics Engine operations to the above functions. */
#ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR
  #define EwGfxScreenFillSolid        NxpRtFillDriver
#endif


/*******************************************************************************
* PROTOTYPE:
*   NxpRtCopyDriver
*
* DESCRIPTION:
*   The following function performs the 'copy rectangular area' operation from a
*   source surface to a native surface.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the destination surface (native/screen color format).
*      See the function CreateSurface().
*   aSrcHandle  - Handle to the source surface (native/index8/alpha8/rgb565 color
*      format). See the function CreateSurface().
*   aDstX,
*   aDstY       - Origin of the area to fill with the copied source surface
*     pixel (relative to the top-left corner of the destination surface).
*   aWidth,
*   aHeight     - Size of the area to fill with the copied source surface pixel.
*   aSrcX,
*   aSrcY       - Origin of the area to copy from the source surface.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 4 color values. These four values do correspond
*     to the four corners of the area: top-left, top-right, bottom-right and
*     bottom-left.
*     In case of an alpha8 source surface if all colors are equal, the solid
*     variant of the operation is assumed.
*     In case of native and index8 source surfaces if all colors are equal but
*     their alpha value < 255, the solid variant of the operation is assumed.
*     In case of native and index8 source surfaces if all colors are equal and
*     their alpha value == 255, the variant withouto any modulation is assumed.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void NxpRtCopyDriver
(
  unsigned long     aDstHandle,
  unsigned long     aSrcHandle,
  int               aDstX,
  int               aDstY,
  int               aSrcX,
  int               aSrcY,
  int               aWidth,
  int               aHeight,
  int               aBlend,
  unsigned long*    aColors
);


/* Macros to redirect the Graphics Engine operations to the above function. */
#ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR
  #define EwGfxCopyNative                 NxpRtCopyDriver
  #define EwGfxCopyRGB565                 NxpRtCopyDriver
  #define EwGfxScreenCopyNative           NxpRtCopyDriver
  #define EwGfxScreenCopyNativeSolid      NxpRtCopyDriver
  #define EwGfxScreenCopyRGB565           NxpRtCopyDriver
  #define EwGfxScreenCopyRGB565Solid      NxpRtCopyDriver
#endif


/*******************************************************************************
* PROTOTYPE:
*   NxpRtBlendDriver
*
* DESCRIPTION:
*   The following function performs the 'blend rectangular area' operation from a
*   native or alpha8 surface to a native surface.
*
* ARGUMENTS:
*   aDstHandle  - Handle to the destination surface (native/screen color format).
*      See the function CreateSurface().
*   aSrcHandle  - Handle to the source surface (native/index8/alpha8/rgb565 color
*      format). See the function CreateSurface().
*   aDstX,
*   aDstY       - Origin of the area to fill with the copied source surface
*     pixel (relative to the top-left corner of the destination surface).
*   aWidth,
*   aHeight     - Size of the area to fill with the copied source surface pixel.
*   aSrcX,
*   aSrcY       - Origin of the area to copy from the source surface.
*   aBlend      - != 0 if the operation should be performed with alpha blending.
*   aColors     - Array with 4 color values. These four values do correspond
*     to the four corners of the area: top-left, top-right, bottom-right and
*     bottom-left.
*     In case of an alpha8 source surface if all colors are equal, the solid
*     variant of the operation is assumed.
*     In case of native and index8 source surfaces if all colors are equal but
*     their alpha value < 255, the solid variant of the operation is assumed.
*     In case of native and index8 source surfaces if all colors are equal and
*     their alpha value == 255, the variant withouto any modulation is assumed.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void NxpRtBlendDriver
(
  unsigned long     aDstHandle,
  unsigned long     aSrcHandle,
  int               aDstX,
  int               aDstY,
  int               aSrcX,
  int               aSrcY,
  int               aWidth,
  int               aHeight,
  int               aBlend,
  unsigned long*    aColors
);


/* Macros to redirect the Graphics Engine operations to the above function. */
#ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR
  #define EwGfxScreenCopyNativeBlend      NxpRtBlendDriver
  #define EwGfxScreenCopyNativeSolidBlend NxpRtBlendDriver
  #define EwGfxScreenCopyRGB565SolidBlend NxpRtBlendDriver
#endif


/*******************************************************************************
* FUNCTION:
*   NxpRtUsePXP
*
* DESCRIPTION:
*   The function NxpRtUsePXP can be used to switch on/off the usage of the
*   PXP dynamically during runtime. If the PXP is deactivated, the software
*   pixel driver of the Graphics Engine is used to execute the different
*   drawing operations.
*
* ARGUMENTS:
*   aActive     - Flag to control the usage of the PXP.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
#ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR
  void NxpRtUsePXP
  (
    int               aActive
  );
#endif


/*******************************************************************************
* FUNCTION:
*   NxpRtIsPXPUsed
*
* DESCRIPTION:
*   The function NxpRtIsPXPUsed returns a non zero value, if the PXP is
*   activated.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   A non zero value, if the PXP is activated.
*
*******************************************************************************/
#ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR
  int NxpRtIsPXPUsed
  (
    void
  );
#endif


#ifdef __cplusplus
  }
#endif

#endif /* EWEXTGFX_H */


/* msy */
