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
*   This module implements the interface between the Graphics Engine and the
*   target specific graphics subsystem.
*   All graphics operations that can be accelerated by a graphics hardware are
*   delegated to the corresponding hardware functionality.
*   This module is responsible to manage the framebuffer(s) and to support the
*   synchronization between CPU, display controller and graphics accelerator.
*
*******************************************************************************/

#include "fsl_cache.h"
#include "ewrte.h"
#include "ewgfx.h"
#include "ewextpxl_RGB565.h"

#include "ew_bsp_display.h"

#ifdef EW_USE_GRAPHICS_ACCELERATOR

  #include "ew_bsp_graphics.h"

  /* flag to switch on/off the graphics accelerator usage during runtime */
  static unsigned char UseGraphicsAccelerator = 1;

  /* variables to store the source and destination surface accessed by hardware */
  static unsigned long DstSurfaceUsedByHardware = 0;
  static unsigned long SrcSurfaceUsedByHardware = 0;

  static void GfxFlushGraphics( void );
  static void GfxSelectSurfaces( unsigned long aDstSurfaceHandle, unsigned long aSrcSurfaceHandle );

#endif

/* The color format of the framebuffer has to correspond to color format of the
   Graphics Engine, because the Graphics engine is prepared and optimized for
   one dedicated color format. */
#if ( EW_FRAME_BUFFER_COLOR_FORMAT != EW_FRAME_BUFFER_COLOR_FORMAT_RGB565 )
  #error The given EW_FRAME_BUFFER_COLOR_FORMAT is not supported! Use RGB565 within your makefile!
#endif

/* Error messages */
#define Err01 "Invalid framebuffer address!"
#define Err02 "Size of framebuffer device (display size) does not match with given application size!"
#define Err03 "Could not allocate memory!"
#define Err04 "Invalid double-buffer address!"
#define Err05 "Requested operation with graphics accelerator failed!"
#define Err06 "Full screen update cannot be done within scratch-pad buffer!"
#define Err07 "Partial screen update cannot be combined with double buffering!"

/* Flags to indicate the current status of a surface */
#define EW_SURFACE_PREALLOCATED           0x01
#define EW_SURFACE_MODIFIED_BY_CPU        0x02
#define EW_SURFACE_MODIFIED_BY_GA         0x04

/* Size of a single cache line within CPU cache */
#define EW_CACHE_LINE_SIZE                32


/* Descriptor of a target specific surface. This type is used for framebuffers and
   internal surfaces (bitmaps). The pixel memory of the surface may be preallocated
   for framebuffers or direct access bitmaps. In all other cases, the pixel memory
   is allocated and freed dynamically. */
typedef struct
{
  int   Width;
  int   Height;
  int   Flags;
  int   BytesPerPixel;
  int   Format;
  int   AllocSize;
  void* Pixel;
  int   OrigHeight;
  void* OrigPixel;
} XGfxSurface;


/* Descriptor of the target specific viewport. It contains pointers to the different
   surfaces (framebuffers) that are used for the display update. */
typedef struct
{
  XGfxSurface* FrameBuffer;
  XGfxSurface* DoubleBuffer;
} XGfxViewport;


/* Memory usage profiler */
extern int EwResourcesMemory;
extern int EwResourcesMemoryPeak;
extern int EwObjectsMemory;
extern int EwStringsMemory;
extern int EwMemoryPeak;

/* Helper function to track the maximum memory pressure */
static void TrackMemoryUsage( void )
{
  if ( EwResourcesMemory > EwResourcesMemoryPeak )
    EwResourcesMemoryPeak = EwResourcesMemory;

  if (( EwObjectsMemory + EwStringsMemory + EwResourcesMemory ) > EwMemoryPeak )
    EwMemoryPeak = EwObjectsMemory + EwStringsMemory + EwResourcesMemory;
}


/*******************************************************************************
* FUNCTION:
*   GfxInitGfx
*
* DESCRIPTION:
*   The function GfxInitGfx is called from the Graphics Engine during the
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
int GfxInitGfx( void* aArgs )
{
  EW_UNUSED_ARG( aArgs );

  /* configure caches, queues and memory allocation strategies of Graphics Engine
     and Runtime Environment according to makefile settings */
  EwMaxSurfaceCacheSize                     = EW_MAX_SURFACE_CACHE_SIZE;
  EwMaxGlyphSurfaceWidth                    = EW_MAX_GLYPH_SURFACE_WIDTH;
  EwMaxGlyphSurfaceHeight                   = EW_MAX_GLYPH_SURFACE_HEIGHT;
  EwMaxIssueTasks                           = EW_MAX_ISSUE_TASKS;
  EwLazyLoadBitmaps                         = EW_LAZY_LOAD_BITMAPS;
  EwLazyLoadBitmapsIfAnimatedOnly           = EW_LAZY_LOAD_BITMAPS_IF_ANIMATED_ONLY;
  EwDiscardBitmaps                          = EW_DISCARD_BITMAPS;
  EwDiscardBitmapsIfAnimatedOnly            = EW_DISCARD_BITMAPS_IF_ANIMATED_ONLY;
  EwDiscardBitmapsIfNotUsedInCurrentUpdate  = EW_DISCARD_BITMAPS_IF_NOT_USED_IN_CURRENT_UPDATE;
  EwDiscardBitmapsIfNotUsedInRecentUpdates  = EW_DISCARD_BITMAPS_IF_NOT_USED_IN_RECENT_UPDATES;
  EwMaxStringCacheSize                      = EW_MAX_STRING_CACHE_SIZE;

  /* In case of pure double-buffering mode, the Mosaic class library has to
     combine the dirty rectangles of two consecutive screen updates.
     To achieve this, the variable EwPreserveFramebufferContent has to be set to 0.
     Normally, the variable EwPreserveFramebufferContent is set to 1, which means
     that the graphics subsystem retains the content of the framebuffer between
     two consecutive screen update frames. */
  #if EW_USE_DOUBLE_BUFFER
    EwPreserveFramebufferContent = 0;
  #endif

  return 1;
}


/*******************************************************************************
* FUNCTION:
*   GfxInitViewport
*
* DESCRIPTION:
*   The function GfxInitViewport is called from the Graphics Engine,
*   to create a new viewport on the target. The function uses the given
*   buffers passed in the arguments aDisplay1 and aDisplay2.
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
*   aDisplay1     - Address of the framebuffer / scratch-pad buffer.
*   aDisplay2     - Address of the back-buffer in case of double-buffering.
*   aDisplay3     - not used.
*
* RETURN VALUE:
*   Handle of the surface descriptor (viewport).
*
*******************************************************************************/
unsigned long GfxInitViewport( int aWidth, int aHeight, int aExtentX,
  int aExtentY, int aExtentWidth, int aExtentHeight, int aOrient, int aOpacity,
  void* aDisplay1, void* aDisplay2, void* aDisplay3 )
{
  EW_UNUSED_ARG( aExtentX );
  EW_UNUSED_ARG( aExtentY );
  EW_UNUSED_ARG( aOrient );
  EW_UNUSED_ARG( aOpacity );
  EW_UNUSED_ARG( aDisplay3 );

  #if !EW_USE_SCRATCHPAD_BUFFER

    /* compare metrics of display with metrics of application */
    if (( aWidth > aExtentWidth ) || ( aHeight != aExtentHeight ))
    {
      EW_ERROR( Err02 );
      return 0;
    }

  #endif

  /* verify that the given framebuffer or scratch-pad buffer address is valid */
  if ( !aDisplay1 )
  {
    EW_ERROR( Err01 );
    return 0;
  }

  /* verify that the given back-buffer address matchs the choosen configuration */
  #if EW_USE_DOUBLE_BUFFER

    if ( !aDisplay2 )
    {
      EW_ERROR( Err04 );
      return 0;
    }

  #endif

  /* allocate memory for the descriptor structure */
  XGfxViewport* viewport = (XGfxViewport*)EwAlloc( sizeof( XGfxViewport ));
  if ( !viewport )
  {
    EW_ERROR( Err03 );
    return 0;
  }
  viewport->FrameBuffer      = 0;
  viewport->DoubleBuffer     = 0;

  /* allocate memory for the framebuffer descriptor */
  viewport->FrameBuffer = (XGfxSurface*)EwAlloc( sizeof( XGfxSurface ));
  if ( !viewport->FrameBuffer )
  {
    EW_ERROR( Err03 );
    return 0;
  }

  /* initialize the framebuffer descriptor */
  viewport->FrameBuffer->Width         = aExtentWidth;
  viewport->FrameBuffer->Height        = aExtentHeight;
  viewport->FrameBuffer->Flags         = EW_SURFACE_PREALLOCATED;
  viewport->FrameBuffer->BytesPerPixel = sizeof( short );
  viewport->FrameBuffer->Format        = EW_PIXEL_FORMAT_SCREEN;
  viewport->FrameBuffer->AllocSize     = 0;
  viewport->FrameBuffer->Pixel         = aDisplay1;
  viewport->FrameBuffer->OrigHeight    = aExtentHeight;
  viewport->FrameBuffer->OrigPixel     = aDisplay1;

  #if EW_USE_DOUBLE_BUFFER

    /* allocate memory for the double-buffer descriptor */
    viewport->DoubleBuffer = (XGfxSurface*)EwAlloc( sizeof( XGfxSurface ));
    if ( !viewport->DoubleBuffer )
    {
      EW_ERROR( Err03 );
      return 0;
    }

    /* initialize the double-buffer descriptor */
    viewport->DoubleBuffer->Width         = aExtentWidth;
    viewport->DoubleBuffer->Height        = aExtentHeight;
    viewport->DoubleBuffer->Flags         = EW_SURFACE_PREALLOCATED;
    viewport->DoubleBuffer->BytesPerPixel = sizeof( short );
    viewport->DoubleBuffer->Format        = EW_PIXEL_FORMAT_SCREEN;
    viewport->DoubleBuffer->AllocSize     = 0;
    viewport->DoubleBuffer->Pixel         = aDisplay2;
    viewport->DoubleBuffer->OrigHeight    = aExtentHeight;
    viewport->DoubleBuffer->OrigPixel     = aDisplay2;

  #endif

  #if EW_USE_SCRATCHPAD_BUFFER

    /* modify the framebuffer descriptor in case of scratch-pad usage:
       width and height of scratch-pad will be provided for each update */
    viewport->FrameBuffer->Width         = 0;
    viewport->FrameBuffer->Height        = 0;
    viewport->FrameBuffer->OrigHeight    = 0;

  #endif

  #ifdef EW_USE_GRAPHICS_ACCELERATOR

    /* initialize the graphics accelerator */
    if ( !EwBspGraphicsInit())
    {
      EW_ERROR( Err05 );
      return 0;
    }

  #endif

  EwResourcesMemory += sizeof( XGfxViewport );
  EwResourcesMemory += sizeof( XGfxSurface );

  #if EW_USE_DOUBLE_BUFFER
    EwResourcesMemory += sizeof( XGfxSurface );
  #endif

  /* track maximum memory pressure */
  TrackMemoryUsage();

  return (unsigned long)viewport;
}


/*******************************************************************************
* FUNCTION:
*   GfxDoneViewport
*
* DESCRIPTION:
*   The function GfxDoneViewport is called from the Graphics Engine, to
*   release a previously created viewport on the target.
*
* ARGUMENTS:
*   aHandle - Handle of the surface descriptor (viewport).
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void GfxDoneViewport( unsigned long aHandle )
{
  XGfxViewport* viewport = (XGfxViewport*)aHandle;

  #ifdef EW_USE_GRAPHICS_ACCELERATOR

    /* ensure that any hardware accelerated drawing operation is finished */
    GfxFlushGraphics();

    /* deinitialize the graphics accelerator */
    EwBspGraphicsDone();

  #endif

  /* destroy the double-buffer descriptor */
  if ( viewport->DoubleBuffer )
    EwFree( viewport->DoubleBuffer );

  /* destroy the framebuffer descriptor */
  if ( viewport->FrameBuffer )
    EwFree( viewport->FrameBuffer );

  /* destroy the viewport */
  EwFree( viewport );

  EwResourcesMemory -= sizeof( XGfxViewport );
  EwResourcesMemory -= sizeof( XGfxSurface );

  #if EW_USE_DOUBLE_BUFFER
    EwResourcesMemory -= sizeof( XGfxSurface );
  #endif
}


/*******************************************************************************
* FUNCTION:
*   GfxBeginUpdate
*
* DESCRIPTION:
*   The function GfxBeginUpdate is called from the Graphics Engine, to
*   initiate the screen update cycle.
*
* ARGUMENTS:
*   aHandle - Handle of the surface descriptor (viewport).
*
* RETURN VALUE:
*   Handle of the destination surface, used for all drawing operations.
*
*******************************************************************************/
unsigned long GfxBeginUpdate( unsigned long aHandle )
{
  #ifdef EW_USE_GRAPHICS_ACCELERATOR

    /* ensure that any hardware accelerated drawing operation is finished */
    GfxFlushGraphics();

  #endif

  /* ensure that display controller is finished with previous buffer */
  EwBspDisplayWaitForCompletion();

  #if EW_USE_SCRATCHPAD_BUFFER

    EW_ERROR( Err06 );
    return 0;

  #elif EW_USE_DOUBLE_BUFFER

    XGfxViewport* viewport = (XGfxViewport*)aHandle;
    return (unsigned long)viewport->DoubleBuffer;

  #else

    XGfxViewport* viewport = (XGfxViewport*)aHandle;
    return (unsigned long)viewport->FrameBuffer;

  #endif
}


/*******************************************************************************
* FUNCTION:
*   GfxBeginUpdateArea
*
* DESCRIPTION:
*   The function GfxBeginUpdateArea is called from the Graphics Engine, to
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
unsigned long GfxBeginUpdateArea( unsigned long aHandle, int aX, int aY,
  int aWidth, int aHeight )
{
  #ifdef EW_USE_GRAPHICS_ACCELERATOR

    /* ensure that any hardware accelerated drawing operation is finished */
    GfxFlushGraphics();

  #endif

  /* ensure that display controller is finished with previous buffer */
  EwBspDisplayWaitForCompletion();

  #if EW_USE_SCRATCHPAD_BUFFER

    #if EW_USE_DOUBLE_BUFFER
    {
      XGfxViewport* viewport = (XGfxViewport*)aHandle;

      /* adapt size of double-scratch-pad buffer */
      viewport->DoubleBuffer->Width = aWidth;
      viewport->DoubleBuffer->Height = aHeight;

      /* return handle of double-scratch-pad buffer */
      return (unsigned long)viewport->DoubleBuffer;
    }
    #else
    {
      XGfxViewport* viewport = (XGfxViewport*)aHandle;

      /* adapt size of scratch-pad buffer */
      viewport->FrameBuffer->Width = aWidth;
      viewport->FrameBuffer->Height = aHeight;

      /* return handle of scratch-pad buffer */
      return (unsigned long)viewport->FrameBuffer;
    }
    #endif

  #else

    #if EW_USE_DOUBLE_BUFFER

      EW_ERROR( Err07 );
      return 0;

    #else

      XGfxViewport* viewport = (XGfxViewport*)aHandle;

      /* adapt size and start address of framebuffer */
      viewport->FrameBuffer->Height = aHeight;
      viewport->FrameBuffer->Pixel  = (unsigned char *)viewport->FrameBuffer->OrigPixel
        + (( aY * viewport->FrameBuffer->Width ) + aX ) * viewport->FrameBuffer->BytesPerPixel;

      return (unsigned long)viewport->FrameBuffer;

    #endif

  #endif
}


/*******************************************************************************
* FUNCTION:
*   GfxEndUpdate
*
* DESCRIPTION:
*   The function GfxEndUpdate is called from the Graphics Engine, to
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
void GfxEndUpdate( unsigned long aHandle, int aX, int aY, int aWidth, int aHeight )
{
  XGfxViewport* viewport = (XGfxViewport*)aHandle;

  /* nothing to do */
  if (( aWidth <= 0 ) || ( aHeight <= 0 ))
    return;

  #ifdef EW_USE_GRAPHICS_ACCELERATOR

    /* ensure that any hardware accelerated drawing operation is finished */
    GfxFlushGraphics();

  #endif

  #if EW_USE_DOUBLE_BUFFER
  {
    /* exchange front- and back-buffer objects */
    XGfxSurface* tmp = viewport->DoubleBuffer;
    viewport->DoubleBuffer = viewport->FrameBuffer;
    viewport->FrameBuffer = tmp;
  }
  #endif

  /* check if the framebuffer was previously modified by CPU */
  if ( viewport->FrameBuffer->Flags & EW_SURFACE_MODIFIED_BY_CPU )
  {
    /* writeback the cache for the address range of the pixel data */
    L1CACHE_CleanDCacheByRange((uint32_t)viewport->FrameBuffer->Pixel,
      viewport->FrameBuffer->Width * viewport->FrameBuffer->Height * viewport->FrameBuffer->BytesPerPixel );

    /* clear the flag */
    viewport->FrameBuffer->Flags &= ~EW_SURFACE_MODIFIED_BY_CPU;
  }

  /* restore framebuffer parameter (patched for synchronous single-buffer) */
  viewport->FrameBuffer->Height = viewport->FrameBuffer->OrigHeight;
  viewport->FrameBuffer->Pixel  = viewport->FrameBuffer->OrigPixel;

  /* inform display driver that buffer content is now updated */
  EwBspDisplayCommitBuffer( viewport->FrameBuffer->Pixel, aX, aY, aWidth, aHeight );
}


/*******************************************************************************
* FUNCTION:
*   GfxCreateSurface
*
* DESCRIPTION:
*   The function GfxCreateSurface() reserves pixel memory for a new surface
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
unsigned long GfxCreateSurface( int aFormat, int aWidth, int aHeight )
{
  XGfxSurface* surface       = 0;
  int          bytesPerPixel = 0;
  int          bitmapSize    = 0;

  /* Important: Due to the fact that pixel data can be accessed by CPU and GA,
     it has to be ensured that the pixel data will never be in the same CPU cache
     line than any other data => pixel memory has to be aligned. */

  /* determine the size of one pixel, the entire bitmap and the optional clut */
  if ( aFormat == EW_PIXEL_FORMAT_NATIVE )
    bytesPerPixel = sizeof( long );
  else if ( aFormat == EW_PIXEL_FORMAT_RGB565 )
    bytesPerPixel = sizeof( short );
  else if ( aFormat == EW_PIXEL_FORMAT_SCREEN )
    bytesPerPixel = sizeof( short );
  bitmapSize = aWidth * aHeight * bytesPerPixel + 2 * EW_CACHE_LINE_SIZE;

  /* try to allocate the memory for the entire surface */
  surface = (XGfxSurface*)EwAllocVideo( sizeof( XGfxSurface ) + bitmapSize );

  /* fill all members of the surface descriptor */
  if ( surface )
  {
    surface->Width         = aWidth;
    surface->Height        = aHeight;
    surface->Flags         = 0;
    surface->BytesPerPixel = bytesPerPixel;
    surface->Format        = aFormat;
    surface->AllocSize     = bitmapSize;
    surface->Pixel         = (void*)(((unsigned long)( surface + 1 )
                           + EW_CACHE_LINE_SIZE - 1 ) & ~( EW_CACHE_LINE_SIZE - 1 ));

    /* adjust memory usage */
    EwResourcesMemory += sizeof( XGfxSurface ) + surface->AllocSize;
    TrackMemoryUsage();
  }
  return (unsigned long)surface;
}


/*******************************************************************************
* FUNCTION:
*   GfxCreateConstSurface
*
* DESCRIPTION:
*   The function GfxCreateConstSurface() creates a surface structure
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
unsigned long GfxCreateConstSurface( int aFormat, int aWidth, int aHeight,
  XSurfaceMemory* aMemory )
{
  XGfxSurface* surface       = 0;
  int          bytesPerPixel = 0;

  /* determine expected size of one pixel */
  if ( aFormat == EW_PIXEL_FORMAT_NATIVE )
    bytesPerPixel = sizeof( long );
  else if ( aFormat == EW_PIXEL_FORMAT_RGB565 )
    bytesPerPixel = sizeof( short );

  /* verify superficially the passed pixel memory */
  if ( !aMemory || !aMemory->Pixel1 || ( aMemory->Pitch1X != bytesPerPixel ) ||
     ( aMemory->Pitch1Y < (int)( aWidth * bytesPerPixel )))
    return 0;

  if ( bytesPerPixel > 0 )
  {
    /* allocate memory only for the administration structure */
    surface = (XGfxSurface*)EwAllocVideo( sizeof( XGfxSurface ));

    if ( surface )
    {
      surface->Width         = aWidth;
      surface->Height        = aHeight;
      surface->Flags         = EW_SURFACE_PREALLOCATED;
      surface->BytesPerPixel = bytesPerPixel;
      surface->Format        = aFormat;
      surface->AllocSize     = 0;
      surface->Pixel         = (void*)( aMemory->Pixel1 );

      /* adjust memory usage */
      EwResourcesMemory += sizeof( XGfxSurface );
      TrackMemoryUsage();
    }
  }

  return (unsigned long)surface;
}


/*******************************************************************************
* FUNCTION:
*   GfxDestroySurface
*
* DESCRIPTION:
*   The function GfxDestroySurface() frees the resources of the given surface.
*   This function is a counterpart to GfxCreateSurface().
*
* ARGUMENTS:
*   aHandle - Handle to the surface to free.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void GfxDestroySurface( unsigned long aHandle )
{
  XGfxSurface* surface = (XGfxSurface*)aHandle;

  #ifdef EW_USE_GRAPHICS_ACCELERATOR

    /* check if the hardware is currently accessing the given surface */
    if (( aHandle == DstSurfaceUsedByHardware ) || ( aHandle == SrcSurfaceUsedByHardware ))
    {
      /* wait until hardware accelerated drawing operation is finished */
      GfxFlushGraphics();
    }

  #endif

  EwResourcesMemory -= sizeof( XGfxSurface ) + surface->AllocSize;

  EwFreeVideo((void*)aHandle );
}


/*******************************************************************************
* FUNCTION:
*   GfxLockSurface
*
* DESCRIPTION:
*   The function GfxLockSurface() provides a direct access to the pixel memory of
*   the given surface. The function returns a lock object containing pointers to
*   memory, where the caller can read/write the surface pixel values. Additional
*   pitch values also returned in the object allow the caller to calculate the
*   desired pixel addresses.
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
unsigned long GfxLockSurface( unsigned long aHandle, int aX, int aY,
  int aWidth, int aHeight, int aIndex, int aCount, int aReadPixel, int aWritePixel,
  int aReadClut, int aWriteClut, XSurfaceMemory* aMemory )
{
  XGfxSurface* surface = (XGfxSurface*)aHandle;

  EW_UNUSED_ARG( aWidth );
  EW_UNUSED_ARG( aHeight );
  EW_UNUSED_ARG( aIndex );
  EW_UNUSED_ARG( aCount );
  EW_UNUSED_ARG( aReadPixel );
  EW_UNUSED_ARG( aReadClut );
  EW_UNUSED_ARG( aWriteClut );

#ifdef EW_USE_GRAPHICS_ACCELERATOR

  /* check if the hardware is currently accessing the given surface */
  if (( aHandle == DstSurfaceUsedByHardware ) || ( aWritePixel && ( aHandle == SrcSurfaceUsedByHardware )))
  {
    /* wait until hardware accelerated drawing operation is finished */
    GfxFlushGraphics();
  }

  /* check if the given surface was previously modified by hardware graphics accelerator */
  if ( surface->Flags & EW_SURFACE_MODIFIED_BY_GA )
  {
    /* invalidate the cache for the address range of the pixel data */
    L1CACHE_InvalidateDCacheByRange((uint32_t)surface->Pixel,
      surface->Width * surface->Height * surface->BytesPerPixel );

    /* clear the flag */
    surface->Flags &= ~EW_SURFACE_MODIFIED_BY_GA;
  }

#endif

  /* sign the surface as modified by CPU */
  if ( aWritePixel )
    surface->Flags |= EW_SURFACE_MODIFIED_BY_CPU;

  /* return the details of the surface */
  aMemory->Pixel1  = (unsigned char *)surface->Pixel + (( aY * surface->Width ) + aX ) * surface->BytesPerPixel;
  aMemory->Pitch1Y = surface->Width * surface->BytesPerPixel;
  aMemory->Pitch1X = surface->BytesPerPixel;
  aMemory->Pixel2  = 0;
  aMemory->Pitch2Y = 0;
  aMemory->Pitch2X = 0;
  aMemory->Clut    = 0;

  return 1;
}


/*******************************************************************************
* FUNCTION:
*   GfxUnlockSurface
*
* DESCRIPTION:
*   The function GfxUnlockSurface() has the job to unlock the given surface and
*   if necessary free any temporary used resources.
*   This function is a counterpart to GfxLockSurface().
*
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
void GfxUnlockSurface( unsigned long aSurfaceHandle, unsigned long aLockHandle,
  int aX, int aY, int aWidth, int aHeight, int aIndex, int aCount, int aWritePixel,
  int aWriteClut )
{
  EW_UNUSED_ARG( aSurfaceHandle );
  EW_UNUSED_ARG( aLockHandle );
  EW_UNUSED_ARG( aX );
  EW_UNUSED_ARG( aY );
  EW_UNUSED_ARG( aWidth );
  EW_UNUSED_ARG( aHeight );
  EW_UNUSED_ARG( aIndex );
  EW_UNUSED_ARG( aCount );
  EW_UNUSED_ARG( aWritePixel );
  EW_UNUSED_ARG( aWriteClut );
}


#ifdef EW_USE_GRAPHICS_ACCELERATOR

/* helper function to finalize any hardware accelerated drawing operation */
static void GfxFlushGraphics( void )
{
  /* wait until hardware accelerated drawing operation is finished */
  EwBspGraphicsWaitForCompletion();

  /* now clear the dst and src handles */
  DstSurfaceUsedByHardware = 0;
  SrcSurfaceUsedByHardware = 0;
}


/* helper function to prepare access to surfaces by hardware */
static void GfxSelectSurfaces( unsigned long aDstSurfaceHandle, unsigned long aSrcSurfaceHandle )
{
  XGfxSurface* dstSurface = (XGfxSurface*)aDstSurfaceHandle;
  XGfxSurface* srcSurface = (XGfxSurface*)aSrcSurfaceHandle;

  if ( dstSurface )
  {
    /* check if the given surface was previously modified by CPU */
    if ( dstSurface->Flags & EW_SURFACE_MODIFIED_BY_CPU )
    {
      /* writeback the cache for the address range of the pixel data */
      L1CACHE_CleanDCacheByRange((uint32_t)dstSurface->Pixel,
        dstSurface->Width * dstSurface->Height * dstSurface->BytesPerPixel );

      /* clear the flag */
      dstSurface->Flags &= ~EW_SURFACE_MODIFIED_BY_CPU;
    }

    /* sign the surface now as modified by hardware graphics accelerator */
    dstSurface->Flags |= EW_SURFACE_MODIFIED_BY_GA;
  }

  if ( srcSurface )
  {
    /* check if the given surface was previously modified by CPU */
    if ( srcSurface->Flags & EW_SURFACE_MODIFIED_BY_CPU )
    {
      /* writeback the cache for the address range of the pixel data */
      L1CACHE_CleanDCacheByRange((uint32_t)srcSurface->Pixel,
        srcSurface->Width * srcSurface->Height * srcSurface->BytesPerPixel );

      /* clear the flag */
      srcSurface->Flags &= ~EW_SURFACE_MODIFIED_BY_CPU;
    }
  }

  /* finally, store the affected source and destination handles */
  DstSurfaceUsedByHardware = aDstSurfaceHandle;
  SrcSurfaceUsedByHardware = aSrcSurfaceHandle;
}


/*******************************************************************************
* FUNCTION:
*   GfxFillDriver
*
* DESCRIPTION:
*   The function GfxFillDriver is called from the Graphics Engine, when a
*   rectangular area should be filled by using the graphics hardware.
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
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void GfxFillDriver( unsigned long aDstHandle, int aDstX, int aDstY,
  int aWidth, int aHeight, int aBlend, unsigned long* aColors )
{
  XGfxSurface* dstSurface = (XGfxSurface*)aDstHandle;
  void*        dst;
  int          dstPitch;

#ifndef EW_DONT_USE_GFX_EMULATION

  /* make a software emulation of the drawing function */
  if ( UseGraphicsAccelerator == 0 )
  {
    XSurfaceMemory dstMem;
    XGradient      gradient;
    XFillWorker    worker = EwScreenFillRowSolid;

    /* obtain direct access to the destination buffer */
    GfxLockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, &dstMem );

    /* let the Graphics Engine make the drawing operation */
    EwInitColorGradient( aWidth, aHeight, (unsigned int*)aColors, &gradient );
    EwEmulateFill( &dstMem, aDstX, aDstY, aWidth, aHeight, &gradient, aWidth, aHeight, worker );

    /* unlock destination buffer */
    GfxUnlockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0 );
  }

  /* otherwise use the hardware... */
  else

#endif
  {
    uint32_t dstColorFormat = kPXP_OutputPixelFormatRGB565;
    uint32_t color = ( EW_ALPHA( aColors[0]) << 24 )
                   | ( EW_RED  ( aColors[0]) << 16 )
                   | ( EW_GREEN( aColors[0]) <<  8 )
                   |   EW_BLUE ( aColors[0]);

    /* obtain direct access to the destination buffer */
    GfxSelectSurfaces( aDstHandle, 0 );

    /* get the start address in the destination and the offset */
    dst      = (void*)((unsigned char*)dstSurface->Pixel + (( aDstY * dstSurface->Width ) + aDstX ) * dstSurface->BytesPerPixel );
    dstPitch = dstSurface->Width * dstSurface->BytesPerPixel;

    /* choose the appropriate PXP drawing operation */
    EwBspGraphicsFill( (uint32_t)dst, dstPitch, aWidth, aHeight, dstColorFormat, color );
  }
}


/*******************************************************************************
* FUNCTION:
*   GfxCopyDriver
*
* DESCRIPTION:
*   The function GfxCopyDriver is called from the Graphics Engine, when a
*   rectangular bitmap area should be copied by using the graphics hardware.
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
*     their alpha value == 255, the variant without any modulation is assumed.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void GfxCopyDriver( unsigned long aDstHandle, unsigned long aSrcHandle,
  int aDstX, int aDstY, int aSrcX, int aSrcY, int aWidth, int aHeight,
  int aBlend, unsigned long* aColors )
{
  XGfxSurface* dstSurface = (XGfxSurface*)aDstHandle;
  XGfxSurface* srcSurface = (XGfxSurface*)aSrcHandle;
  void*        dst;
  int          dstPitch;
  void*        src;
  int          srcPitch;
  int          dstFormat = dstSurface->Format;
  int          srcFormat = srcSurface->Format;

#ifndef EW_DONT_USE_GFX_EMULATION

  /* make a software emulation of the drawing function */
  if ( UseGraphicsAccelerator == 0 )
  {
    XSurfaceMemory dstMem;
    XSurfaceMemory srcMem;
    unsigned char  alpha  = EW_ALPHA( aColors[0]);
    XGradient      gradient;
    XCopyWorker    worker = 0;

    /* obtain direct access to the destination and source buffer */
    GfxLockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, &dstMem );
    GfxLockSurface( aSrcHandle, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, &srcMem );

    /* select the necessary drawing function */
    if ( srcFormat == EW_PIXEL_FORMAT_NATIVE )
    {
      if ( dstFormat == EW_PIXEL_FORMAT_SCREEN )
        worker = (alpha == 0xFF) ? EwScreenCopyNativeRow : EwScreenCopyNativeRowSolid;
      else
        worker = EwCopyNativeRow;
    }
    else if ( srcFormat == EW_PIXEL_FORMAT_RGB565 )
    {
      if ( dstFormat == EW_PIXEL_FORMAT_SCREEN )
        worker = (alpha == 0xFF) ? EwScreenCopyRGB565Row : EwScreenCopyRGB565RowSolid;
      else
        worker = EwCopyRGB565Row;
    }

    /* let the Graphics Engine make the drawing operation */
    EwInitColorGradient( aWidth, aHeight, (unsigned int*)aColors, &gradient );
    EwEmulateCopy( &dstMem, &srcMem, aDstX, aDstY, aWidth, aHeight, aSrcX, aSrcY, &gradient, aWidth, aHeight, worker );

    /* unlock destination and source buffer */
    GfxUnlockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0 );
    GfxUnlockSurface( aSrcHandle, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
  }

  /* otherwise use the hardware... */
  else

#endif
  {
    uint32_t dstColorFormat = kPXP_OutputPixelFormatARGB8888;
    uint32_t srcColorFormat = kPXP_AsPixelFormatARGB8888;
    uint32_t color          = ( EW_ALPHA( aColors[0]) << 24 )
                            | ( EW_RED  ( aColors[0]) << 16 )
                            | ( EW_GREEN( aColors[0]) <<  8 )
                            |   EW_BLUE ( aColors[0]);

    /* obtain direct access to the destination and source buffer */
    GfxSelectSurfaces( aDstHandle, aSrcHandle );

    /* get the start address in the source and destination and their offsets */
    dst      = (void*)((unsigned char*)dstSurface->Pixel + (( aDstY * dstSurface->Width ) + aDstX ) * dstSurface->BytesPerPixel );
    dstPitch = dstSurface->Width * dstSurface->BytesPerPixel;
    src      = (void*)((unsigned char*)srcSurface->Pixel + (( aSrcY * srcSurface->Width ) + aSrcX ) * srcSurface->BytesPerPixel );
    srcPitch = srcSurface->Width * srcSurface->BytesPerPixel;

    /* change the destination color format, if necessary */
    if ( dstFormat == EW_PIXEL_FORMAT_SCREEN )
      dstColorFormat = kPXP_OutputPixelFormatRGB565;

    /* change the source color format, if necessary */
    if ( srcFormat == EW_PIXEL_FORMAT_RGB565 )
      srcColorFormat =  kPXP_AsPixelFormatRGB565;

    /* choose the appropriate PXP drawing operation */
    EwBspGraphicsCopy( (uint32_t)dst, (uint32_t)src, dstPitch, srcPitch, aWidth, aHeight, dstColorFormat, srcColorFormat, color );
  }
}


/*******************************************************************************
* FUNCTION:
*   GfxBlendDriver
*
* DESCRIPTION:
*   The function GfxBlendDriver is called from the Graphics Engine, when a
*   rectangular bitmap area should be blended by using the graphics hardware.
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
void GfxBlendDriver( unsigned long aDstHandle, unsigned long aSrcHandle,
  int aDstX, int aDstY, int aSrcX, int aSrcY, int aWidth, int aHeight,
  int aBlend, unsigned long* aColors )
{
  XGfxSurface* dstSurface = (XGfxSurface*)aDstHandle;
  XGfxSurface* srcSurface = (XGfxSurface*)aSrcHandle;
  void*        dst;
  int          dstPitch;
  void*        src;
  int          srcPitch;
  int          srcFormat = srcSurface->Format;

#ifndef EW_DONT_USE_GFX_EMULATION

  /* make a software emulation of the drawing function */
  if ( UseGraphicsAccelerator == 0 )
  {
    XSurfaceMemory dstMem;
    XSurfaceMemory srcMem;
    unsigned char  alpha  = EW_ALPHA( aColors[0]);
    XGradient      gradient;
    XCopyWorker    worker = 0;

    /* obtain direct access to the destination and source buffer */
    GfxLockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, &dstMem );
    GfxLockSurface( aSrcHandle, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, &srcMem );

    /* select the necessary drawing function */
    if ( srcFormat == EW_PIXEL_FORMAT_NATIVE )
      worker = (alpha == 0xFF) ? EwScreenCopyNativeRowBlend : EwScreenCopyNativeRowSolidBlend;
    else if ( srcFormat == EW_PIXEL_FORMAT_RGB565 )
      worker = EwScreenCopyRGB565RowSolidBlend;

    /* let the Graphics Engine make the drawing operation */
    EwInitColorGradient( aWidth, aHeight, (unsigned int*)aColors, &gradient );
    EwEmulateCopy( &dstMem, &srcMem, aDstX, aDstY, aWidth, aHeight, aSrcX, aSrcY, &gradient, aWidth, aHeight, worker );

    /* unlock destination and source buffer */
    GfxUnlockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0 );
    GfxUnlockSurface( aSrcHandle, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
  }

  /* otherwise use the hardware... */
  else

#endif
  {
    uint32_t dstColorFormat = kPXP_OutputPixelFormatRGB565;
    uint32_t srcColorFormat = kPXP_AsPixelFormatARGB8888;
    uint32_t color          = ( EW_ALPHA( aColors[0]) << 24 )
                            | ( EW_RED  ( aColors[0]) << 16 )
                            | ( EW_GREEN( aColors[0]) <<  8 )
                            |   EW_BLUE ( aColors[0]);

    /* obtain direct access to the destination and source buffer */
    GfxSelectSurfaces( aDstHandle, aSrcHandle );

    /* get the start address in the source and destination and their offsets */
    dst      = (void*)((unsigned char*)dstSurface->Pixel + (( aDstY * dstSurface->Width ) + aDstX ) * dstSurface->BytesPerPixel );
    dstPitch = dstSurface->Width * dstSurface->BytesPerPixel;
    src      = (void*)((unsigned char*)srcSurface->Pixel + (( aSrcY * srcSurface->Width ) + aSrcX ) * srcSurface->BytesPerPixel );
    srcPitch = srcSurface->Width * srcSurface->BytesPerPixel;

    /* change the source color format, if necessary */
    if ( srcFormat == EW_PIXEL_FORMAT_RGB565 )
      srcColorFormat = kPXP_AsPixelFormatRGB565;

    /* choose the appropriate PXP drawing operation */
    EwBspGraphicsCopyBlend( (uint32_t)dst, (uint32_t)src, dstPitch, srcPitch, aWidth, aHeight, dstColorFormat, srcColorFormat, color );
  }
}


/*******************************************************************************
* FUNCTION:
*   GfxUseGraphicsAccelerator
*
* DESCRIPTION:
*   The function GfxUseGraphicsAccelerator can be used to switch on/off the usage
*   of the graphics accelerator dynamically during runtime. If it is deactivated,
*   the software pixel driver of the Graphics Engine are used to execute the
*   different drawing operations.
*
* ARGUMENTS:
*   aActive     - Flag to control the usage of the graphics accelerator.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void GfxUseGraphicsAccelerator( int aActive )
{
  /* check for any changes */
  if ( aActive == UseGraphicsAccelerator )
    return;

  #ifndef EW_DONT_USE_GFX_EMULATION

    /* just store the flag */
    UseGraphicsAccelerator = aActive;

    /* and put a message to the terminal */
    if ( aActive )
      EwPrint( "Graphics Accelerator is now active...\n" );
    else
      EwPrint( "Graphics Accelerator is now disabled!\n" );

  #else

    EwPrint( "Graphics Accelerator is always active.\n" );

  #endif
}


/*******************************************************************************
* FUNCTION:
*   GfxIsGraphicsAcceleratorUsed
*
* DESCRIPTION:
*   The function GfxIsGraphicsAcceleratorUsed returns a non zero value, if the
*   graphics accelerator hardware is activated.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   A non zero value, if the graphics accelerator hardware is activated.
*
*******************************************************************************/
int GfxIsGraphicsAcceleratorUsed( void )
{
  return UseGraphicsAccelerator;
}

#endif /* EW_USE_GRAPHICS_ACCELERATOR */

/* msy */
