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

#include "fsl_cache.h"
#include "ewgfxdriver.h"
#include "ewextgfx.h"
#include "ewgfxdefs.h"
#include "ewextpxl_RGB565.h"
#include "ewrte.h"

#include "ew_bsp_display.h"

#ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR

  #include "ew_bsp_graphics.h"

  /* flag to switch on/off PXP graphics accelerator during runtime */
  static unsigned char UsePXP = 1;

  /* variables to store the source and destination surface accessed by PXP */
  static unsigned long DstSurfaceUsedByPXP = 0;
  static unsigned long SrcSurfaceUsedByPXP = 0;

#endif

/* Error messages */
#define Err01 "Invalid address of framebuffer (front-buffer)!"
#define Err02 "Size of framebuffer device (display size) does not match with given application size!"
#define Err03 "Could not allocate memory!"
#define Err04 "Invalid address of back-buffer!"
#define Err05 "Requested operation with graphics accelerator PXP failed!"
#define Err06 "Full screen update cannot be done within scratch-pad buffer!"
#define Err07 "Partial screen update cannot be combined with double buffering!"
#define Err08 "Invalid address of scratch-pad buffer!"

/* Flags to indicate the current status of a surface */
#define EW_SURFACE_PREALLOCATED           0x01
#define EW_SURFACE_MODIFIED_BY_CPU        0x02
#define EW_SURFACE_MODIFIED_BY_PXP        0x04

/* Size of a single cache line within CPU cache */
#define CACHE_LINE_SIZE                   32

/* Descriptor of a NxpRt surface. This type is used for framebuffers and all
   internal surfaces (bitmaps). The pixel memory of the surface may be preallocated
   for framebuffers. In all other cases, the pixel memory is allocated and freed
   dynamically. */
typedef struct
{
  int   Width;
  int   Height;
  int   Flags;
  int   BytesPerPixel;
  int   Format;
  void* Pixel;
  int   OrigHeight;
  void* OrigPixel;
} XNxpRtSurface;


/* Descriptor of an NxpRt viewport. Due to the different modes that are supported,
   an additional off-screen canvas and/or double-buffering will be used. */
typedef struct
{
  XNxpRtSurface* FrameBuffer;
  XNxpRtSurface* DoubleBuffer;
  XNxpRtSurface* ScratchPadBuffer;
} XNxpRtViewport;


/* Memory usage profiler */
extern int EwResourcesMemory;
extern int EwResourcesMemoryPeak;
extern int EwObjectsMemory;
extern int EwStringsMemory;
extern int EwMemoryPeak;

/* extern variables to control Graphics Engine */
extern int EwPreserveFramebufferContent;
extern int EwMaxSurfaceCacheSize;
extern int EwMaxGlyphSurfaceWidth;
extern int EwMaxGlyphSurfaceHeight;
extern int EwMaxIssueTasks;


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
int NxpRtInitGfx( void* aArgs )
{
  /* configure bitmap and glyph cache according makefile settings */
  EwMaxSurfaceCacheSize   = EW_MAX_SURFACE_CACHE_SIZE;
  EwMaxGlyphSurfaceWidth  = EW_MAX_GLYPH_SURFACE_WIDTH;
  EwMaxGlyphSurfaceHeight = EW_MAX_GLYPH_SURFACE_HEIGHT;
  EwMaxIssueTasks         = EW_MAX_ISSUE_TASKS;

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
unsigned long NxpRtInitViewport( int aWidth, int aHeight, int aExtentX,
  int aExtentY, int aExtentWidth, int aExtentHeight, int aOrient, int aOpacity,
  void* aDisplay1, void* aDisplay2, void* aDisplay3 )
{
  /* compare metrics of display with metrics of application */
  if (( aWidth > aExtentWidth ) || ( aHeight != aExtentHeight ))
  {
    EW_ERROR( Err02 );
    return 0;
  }

  #if EW_USE_SCRATCHPAD_BUFFER

    /* verify that the given scratch-pad buffer address is valid */
    if ( !aDisplay3 )
    {
      EW_ERROR( Err08 );
      return 0;
    }

  #else

    /* verify that the given framebuffer address is valid */
    if ( !aDisplay1 )
    {
      EW_ERROR( Err01 );
      return 0;
    }

  #endif

  /* verify that the given back-buffer address matchs the choosen configuration */
  #if EW_USE_DOUBLE_BUFFER

    if ( !aDisplay2 )
    {
      EW_ERROR( Err04 );
      return 0;
    }

  #endif

  /* allocate memory for the descriptor structure */
  XNxpRtViewport* viewport = (XNxpRtViewport*)EwAlloc( sizeof( XNxpRtViewport ));
  if ( !viewport )
  {
    EW_ERROR( Err03 );
    return 0;
  }
  viewport->FrameBuffer      = 0;
  viewport->DoubleBuffer     = 0;
  viewport->ScratchPadBuffer = 0;

  #if EW_USE_SCRATCHPAD_BUFFER

    /* allocate memory for the scratch-pad buffer descriptor */
    viewport->ScratchPadBuffer = (XNxpRtSurface*)EwAlloc( sizeof( XNxpRtSurface ));
    if ( !viewport->ScratchPadBuffer )
    {
      EW_ERROR( Err03 );
      return 0;
    }

    /* initialize the scratch-pad buffer descriptor */
    viewport->ScratchPadBuffer->Width         = 0;
    viewport->ScratchPadBuffer->Height        = 0;
    viewport->ScratchPadBuffer->Flags         = EW_SURFACE_PREALLOCATED;
    viewport->ScratchPadBuffer->BytesPerPixel = FRAME_BUFFER_DEPTH;
    viewport->ScratchPadBuffer->Format        = EW_PIXEL_FORMAT_SCREEN;
    viewport->ScratchPadBuffer->Pixel         = aDisplay3;
    viewport->ScratchPadBuffer->OrigHeight    = 0;
    viewport->ScratchPadBuffer->OrigPixel     = aDisplay3;

    #if EW_USE_DOUBLE_BUFFER

      /* allocate memory for the double-buffer descriptor */
      viewport->DoubleBuffer = (XNxpRtSurface*)EwAlloc( sizeof( XNxpRtSurface ));
      if ( !viewport->DoubleBuffer )
      {
        EW_ERROR( Err03 );
        return 0;
      }

      /* initialize the double-buffer descriptor */
      viewport->DoubleBuffer->Width         = 0;
      viewport->DoubleBuffer->Height        = 0;
      viewport->DoubleBuffer->Flags         = EW_SURFACE_PREALLOCATED;
      viewport->DoubleBuffer->BytesPerPixel = FRAME_BUFFER_DEPTH;
      viewport->DoubleBuffer->Format        = EW_PIXEL_FORMAT_SCREEN;
      viewport->DoubleBuffer->Pixel         = aDisplay2;
      viewport->DoubleBuffer->OrigHeight    = 0;
      viewport->DoubleBuffer->OrigPixel     = aDisplay2;

    #endif

  #else

    /* allocate memory for the framebuffer descriptor */
    viewport->FrameBuffer = (XNxpRtSurface*)EwAlloc( sizeof( XNxpRtSurface ));
    if ( !viewport->FrameBuffer )
    {
      EW_ERROR( Err03 );
      return 0;
    }

    /* initialize the framebuffer descriptor */
    viewport->FrameBuffer->Width         = aExtentWidth;
    viewport->FrameBuffer->Height        = aExtentHeight;
    viewport->FrameBuffer->Flags         = EW_SURFACE_PREALLOCATED;
    viewport->FrameBuffer->BytesPerPixel = FRAME_BUFFER_DEPTH;
    viewport->FrameBuffer->Format        = EW_PIXEL_FORMAT_SCREEN;
    viewport->FrameBuffer->Pixel         = aDisplay1;
    viewport->FrameBuffer->OrigHeight    = aExtentHeight;
    viewport->FrameBuffer->OrigPixel     = aDisplay1;

    #if EW_USE_DOUBLE_BUFFER

      /* allocate memory for the double-buffer descriptor */
      viewport->DoubleBuffer = (XNxpRtSurface*)EwAlloc( sizeof( XNxpRtSurface ));
      if ( !viewport->DoubleBuffer )
      {
        EW_ERROR( Err03 );
        return 0;
      }

      /* initialize the double-buffer descriptor */
      viewport->DoubleBuffer->Width         = aExtentWidth;
      viewport->DoubleBuffer->Height        = aExtentHeight;
      viewport->DoubleBuffer->Flags         = EW_SURFACE_PREALLOCATED;
      viewport->DoubleBuffer->BytesPerPixel = FRAME_BUFFER_DEPTH;
      viewport->DoubleBuffer->Format        = EW_PIXEL_FORMAT_SCREEN;
      viewport->DoubleBuffer->Pixel         = aDisplay2;
      viewport->DoubleBuffer->OrigHeight    = aExtentHeight;
      viewport->DoubleBuffer->OrigPixel     = aDisplay2;

    #endif

  #endif

  #ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR

    /* initialize the PXP graphics accelerator */
    if ( !EwBspGraphicsInit())
    {
      EW_ERROR( Err05 );
      return 0;
    }

  #endif

  EwResourcesMemory += sizeof( XNxpRtViewport );
  EwResourcesMemory += sizeof( XNxpRtSurface );

  #if EW_USE_DOUBLE_BUFFER
    EwResourcesMemory += sizeof( XNxpRtSurface );
  #endif

  /* Also track the max. memory pressure */
  if ( EwResourcesMemory > EwResourcesMemoryPeak )
    EwResourcesMemoryPeak = EwResourcesMemory;

  if (( EwObjectsMemory + EwStringsMemory + EwResourcesMemory ) > EwMemoryPeak )
    EwMemoryPeak = EwObjectsMemory + EwStringsMemory + EwResourcesMemory;

  /* ensure that current framebuffer is shown */
  if ( viewport->FrameBuffer )
    EwBspSetFramebufferAddress( (unsigned long)viewport->FrameBuffer->Pixel );

  return (unsigned long)viewport;
}


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
void NxpRtDoneViewport( unsigned long aHandle )
{
  XNxpRtViewport* viewport = (XNxpRtViewport*)aHandle;

  /* destroy the double-buffer descriptor */
  if ( viewport->DoubleBuffer )
    EwFree( viewport->DoubleBuffer );

  /* destroy the framebuffer descriptor */
  if ( viewport->FrameBuffer )
    EwFree( viewport->FrameBuffer );

  /* destroy the scratch-pad buffer descriptor */
  if ( viewport->ScratchPadBuffer )
    EwFree( viewport->ScratchPadBuffer );

  /* destroy the viewport */
  EwFree( viewport );

  #ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR

    /* deinitialize the PXP graphics accelerator */
    EwBspGraphicsDone();

  #endif

  EwResourcesMemory -= sizeof( XNxpRtViewport );
  EwResourcesMemory -= sizeof( XNxpRtSurface );

  #if EW_USE_DOUBLE_BUFFER
    EwResourcesMemory -= sizeof( XNxpRtSurface );
  #endif
}


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
unsigned long NxpRtBeginUpdate( unsigned long aHandle )
{
  #ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR

    /* ensure that any hardware accelerated drawing operation is finished */
    EwBspGraphicsWaitForCompletion();

    /* clear the dst and src handles */
    DstSurfaceUsedByPXP = 0;
    SrcSurfaceUsedByPXP = 0;

  #endif

  #if EW_USE_SCRATCHPAD_BUFFER

    EW_ERROR( Err06 );
    return 0;

  #elif EW_USE_DOUBLE_BUFFER

    XNxpRtViewport* viewport = (XNxpRtViewport*)aHandle;

    /* ensure that the LCD controller is already reading the front-buffer */
    EwBspDisplayWaitForCompletion();

    return (unsigned long)viewport->DoubleBuffer;

  #else

    XNxpRtViewport* viewport = (XNxpRtViewport*)aHandle;
    return (unsigned long)viewport->FrameBuffer;

  #endif
}


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
unsigned long NxpRtBeginUpdateArea( unsigned long aHandle, int aX, int aY,
  int aWidth, int aHeight )
{
  #ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR

    /* ensure that any hardware accelerated drawing operation is finished */
    EwBspGraphicsWaitForCompletion();

    /* clear the dst and src handles */
    DstSurfaceUsedByPXP = 0;
    SrcSurfaceUsedByPXP = 0;

  #endif

  #if EW_USE_SCRATCHPAD_BUFFER

    #if EW_USE_DOUBLE_BUFFER
    {
      XNxpRtViewport* viewport = (XNxpRtViewport*)aHandle;

      /* adapt size of double-scratch-pad buffer */
      viewport->DoubleBuffer->Width = aWidth;
      viewport->DoubleBuffer->Height = aHeight;

      /* return handle of double-scratch-pad buffer */
      return (unsigned long)viewport->DoubleBuffer;
    }
    #else
    {
      XNxpRtViewport* viewport = (XNxpRtViewport*)aHandle;

      /* adapt size of scratch-pad buffer */
      viewport->ScratchPadBuffer->Width = aWidth;
      viewport->ScratchPadBuffer->Height = aHeight;

      /* return handle of scratch-pad buffer */
      return (unsigned long)viewport->ScratchPadBuffer;
    }
    #endif

  #else

    #if EW_USE_DOUBLE_BUFFER

      EW_ERROR( Err07 );
      return 0;

    #else

      XNxpRtViewport* viewport = (XNxpRtViewport*)aHandle;

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
*   NxpRtEndUpdate
*
* DESCRIPTION:
*   The function NxpRtBeginUpdate is called from the Graphics Engine, to
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
void NxpRtEndUpdate( unsigned long aHandle, int aX, int aY, int aWidth,
  int aHeight )
{
  XNxpRtViewport* viewport = (XNxpRtViewport*)aHandle;

  /* nothing to do */
  if (( aWidth <= 0 ) || ( aHeight <= 0 ))
    return;

  #ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR

    /* ensure that any hardware accelerated drawing operation is finished */
    EwBspGraphicsWaitForCompletion();

    /* clear the dst and src handles */
    DstSurfaceUsedByPXP = 0;
    SrcSurfaceUsedByPXP = 0;

  #endif

  #if EW_USE_SCRATCHPAD_BUFFER

    #if EW_USE_DOUBLE_BUFFER
    {
      /* exchange scratch-pad and back-buffer objects */
      XNxpRtSurface* tmp = viewport->DoubleBuffer;
      viewport->DoubleBuffer = viewport->ScratchPadBuffer;
      viewport->ScratchPadBuffer = tmp;
    }
    #endif

    /* check if the scratch-pad buffer was previously modified by CPU */
    if ( viewport->ScratchPadBuffer->Flags & EW_SURFACE_MODIFIED_BY_CPU )
    {
      /* writeback the cache for the address range of the pixel data */
      L1CACHE_CleanDCacheByRange((uint32_t)viewport->ScratchPadBuffer->Pixel,
        viewport->ScratchPadBuffer->Width * viewport->ScratchPadBuffer->Height * viewport->ScratchPadBuffer->BytesPerPixel );

      /* clear the flag */
      viewport->ScratchPadBuffer->Flags &= ~EW_SURFACE_MODIFIED_BY_CPU;
    }

    /* transfer scratch-pad content into display memory */
    EwBspUpdateDisplay( aX, aY, aWidth, aHeight, viewport->ScratchPadBuffer->Pixel );

  #else

    #if EW_USE_DOUBLE_BUFFER
    {
      /* exchange front- and back-buffer objects */
      XNxpRtSurface* tmp = viewport->DoubleBuffer;
      viewport->DoubleBuffer = viewport->FrameBuffer;
      viewport->FrameBuffer = tmp;

      /* check if the framebuffer was previously modified by CPU */
      if ( viewport->FrameBuffer->Flags & EW_SURFACE_MODIFIED_BY_CPU )
      {
        /* writeback the cache for the address range of the pixel data */
        L1CACHE_CleanDCacheByRange((uint32_t)viewport->FrameBuffer->Pixel,
          viewport->FrameBuffer->Width * viewport->FrameBuffer->Height * viewport->FrameBuffer->BytesPerPixel );

        /* clear the flag */
        viewport->FrameBuffer->Flags &= ~EW_SURFACE_MODIFIED_BY_CPU;
      }

      /* make the new front-buffer visible */
      EwBspSetFramebufferAddress( (unsigned long)viewport->FrameBuffer->Pixel );

    }
    #else
    {
      /* check if the framebuffer was previously modified by CPU */
      if ( viewport->FrameBuffer->Flags & EW_SURFACE_MODIFIED_BY_CPU )
      {
        /* writeback the cache for the address range of the pixel data */
        L1CACHE_CleanDCacheByRange((uint32_t)viewport->FrameBuffer->Pixel,
          viewport->FrameBuffer->Width * viewport->FrameBuffer->Height * viewport->FrameBuffer->BytesPerPixel );

        /* clear the flag */
        viewport->FrameBuffer->Flags &= ~EW_SURFACE_MODIFIED_BY_CPU;
      }

      /* restore address and size of framebuffer */
      viewport->FrameBuffer->Height = viewport->FrameBuffer->OrigHeight;
      viewport->FrameBuffer->Pixel  = viewport->FrameBuffer->OrigPixel;
    }
    #endif

  #endif

}


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
unsigned long NxpRtCreateSurface( int aFormat, int aWidth, int aHeight )
{
  XNxpRtSurface* surface       = 0;
  int            bytesPerPixel = 0;
  int            bitmapSize    = 0;

  /* Important: Due to the fact that pixel data can be accessed by CPU and PXP,
     it has to be ensured that the pixel data will never be in the same CPU cache
     line than any other data => pixel memory has to be aligned. */

  /* determine the size of one pixel, the entire bitmap and the optional clut */
  if ( aFormat == EW_PIXEL_FORMAT_NATIVE )
    bytesPerPixel = sizeof( long );
  else if ( aFormat == EW_PIXEL_FORMAT_RGB565 )
    bytesPerPixel = sizeof( short );
  else if ( aFormat == EW_PIXEL_FORMAT_SCREEN )
    bytesPerPixel = sizeof( short );
  bitmapSize = aWidth * aHeight * bytesPerPixel + 2 * CACHE_LINE_SIZE;

  /* try to allocate the memory for the entire surface */
  surface = (XNxpRtSurface*)EwAllocVideo( sizeof( XNxpRtSurface ) + bitmapSize );

  /* fill all members of the surface descriptor */
  if ( surface )
  {
    surface->Width         = aWidth;
    surface->Height        = aHeight;
    surface->Flags         = 0;
    surface->BytesPerPixel = bytesPerPixel;
    surface->Format        = aFormat;
    surface->Pixel         = (void*)(((unsigned long)( surface + 1 )
                             + CACHE_LINE_SIZE - 1 ) & ~( CACHE_LINE_SIZE - 1 ));

    EwResourcesMemory += sizeof( XNxpRtSurface ) +  bitmapSize;

    /* also track the max. memory pressure */
    if ( EwResourcesMemory > EwResourcesMemoryPeak )
      EwResourcesMemoryPeak = EwResourcesMemory;

    if (( EwObjectsMemory + EwStringsMemory + EwResourcesMemory ) > EwMemoryPeak )
      EwMemoryPeak = EwObjectsMemory + EwStringsMemory + EwResourcesMemory;
  }
  return (unsigned long)surface;
}


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
unsigned long NxpRtCreateConstSurface( int aFormat, int aWidth, int aHeight,
  XSurfaceMemory* aMemory )
{
  XNxpRtSurface* surface       = 0;
  int            bytesPerPixel = 0;

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
    surface = (XNxpRtSurface*)EwAllocVideo( sizeof( XNxpRtSurface ));

    if ( surface )
    {
      surface->Width         = aWidth;
      surface->Height        = aHeight;
      surface->Flags         = EW_SURFACE_PREALLOCATED;
      surface->BytesPerPixel = bytesPerPixel;
      surface->Format        = aFormat;
      surface->Pixel         = (void*)( aMemory->Pixel1 );

      EwResourcesMemory += sizeof( XNxpRtSurface );

      /* also track the max. memory pressure */
      if ( EwResourcesMemory > EwResourcesMemoryPeak )
        EwResourcesMemoryPeak = EwResourcesMemory;

      if (( EwObjectsMemory + EwStringsMemory + EwResourcesMemory ) > EwMemoryPeak )
        EwMemoryPeak = EwObjectsMemory + EwStringsMemory + EwResourcesMemory;
    }
  }

  return (unsigned long)surface;
}


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
void NxpRtDestroySurface( unsigned long aHandle )
{
  XNxpRtSurface* surface = (XNxpRtSurface*)aHandle;

  #ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR

    /* check if the PXP is currently accessing the given surface */
    if (( aHandle == DstSurfaceUsedByPXP ) || ( aHandle == SrcSurfaceUsedByPXP ))
    {
      /* wait until hardware accelerated drawing operation is finished */
      EwBspGraphicsWaitForCompletion();

      /* clear the dst and src handles */
      DstSurfaceUsedByPXP = 0;
      SrcSurfaceUsedByPXP = 0;
    }

  #endif

  EwResourcesMemory -= sizeof( XNxpRtSurface ) + (( surface->Flags & EW_SURFACE_PREALLOCATED ) ?
    0 : ( surface->Width * surface->Height * surface->BytesPerPixel + 2 * CACHE_LINE_SIZE ));

  EwFreeVideo((void*)aHandle );
}


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
unsigned long NxpRtLockSurface( unsigned long aHandle, int aX, int aY,
  int aWidth, int aHeight, int aIndex, int aCount, int aReadPixel, int aWritePixel,
  int aReadClut, int aWriteClut, XSurfaceMemory* aMemory )
{
  XNxpRtSurface* surface = (XNxpRtSurface*)aHandle;

#ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR

  /* check if the PXP is currently accessing the given surface */
  if (( aHandle == DstSurfaceUsedByPXP ) || ( aWritePixel && ( aHandle == SrcSurfaceUsedByPXP )))
  {
    /* wait until hardware accelerated drawing operation is finished */
    EwBspGraphicsWaitForCompletion();

    /* clear the dst and src handles */
    DstSurfaceUsedByPXP = 0;
    SrcSurfaceUsedByPXP = 0;
  }

  /* check if the given surface was previously modified by PXP */
  if ( surface->Flags & EW_SURFACE_MODIFIED_BY_PXP )
  {
    /* invalidate the cache for the address range of the pixel data */
    L1CACHE_InvalidateDCacheByRange((uint32_t)surface->Pixel,
      surface->Width * surface->Height * surface->BytesPerPixel );

    /* clear the flag */
    surface->Flags &= ~EW_SURFACE_MODIFIED_BY_PXP;
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
void NxpRtUnlockSurface( unsigned long aSurfaceHandle, unsigned long aLockHandle,
  int aX, int aY, int aWidth, int aHeight, int aIndex, int aCount, int aWritePixel,
  int aWriteClut )
{
}


#ifdef EW_USE_PXP_GRAPHICS_ACCELERATOR

/* helper function to prepare access to surfaces by PXP */
static void NxpRtSelectSurfaces( unsigned long aDstSurfaceHandle, unsigned long aSrcSurfaceHandle )
{
  XNxpRtSurface* dstSurface = (XNxpRtSurface*)aDstSurfaceHandle;
  XNxpRtSurface* srcSurface = (XNxpRtSurface*)aSrcSurfaceHandle;

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

    /* sign the surface now as modified by PXP */
    dstSurface->Flags |= EW_SURFACE_MODIFIED_BY_PXP;
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
  DstSurfaceUsedByPXP = aDstSurfaceHandle;
  SrcSurfaceUsedByPXP = aSrcSurfaceHandle;
}


/*******************************************************************************
* FUNCTION:
*   NxpRtFillDriver
*
* DESCRIPTION:
*   The function NxpRtFillDriver is called from the Graphics Engine, when a
*   rectangular area should be filled by using the PXP functionality.
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
void NxpRtFillDriver( unsigned long aDstHandle, int aDstX, int aDstY,
  int aWidth, int aHeight, int aBlend, unsigned long* aColors )
{
  XNxpRtSurface* dstSurface = (XNxpRtSurface*)aDstHandle;
  void*          dst;
  int            dstPitch;

#ifndef EW_DONT_USE_PXP_SOFTWARE_REPLACEMENT
  /* make a software emulation of the drawing function if the PXP is switched off */
  if ( UsePXP == 0 )
  {
    XSurfaceMemory dstMem;
    XGradient      gradient;
    XFillWorker    worker = EwScreenFillRowSolid;

    /* obtain direct access to the destination buffer */
    NxpRtLockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, &dstMem );

    /* let the Graphics Engine make the drawing operation */
    EwInitColorGradient( aWidth, aHeight, (unsigned int*)aColors, &gradient );
    EwEmulateFill( &dstMem, aDstX, aDstY, aWidth, aHeight, &gradient, aWidth, aHeight, worker );

    /* unlock destination buffer */
    NxpRtUnlockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0 );
  }

  /* otherwise use the PXP hardware... */
  else
#endif
  {
    uint32_t dstColorFormat = kPXP_OutputPixelFormatRGB565;
    uint32_t color = ( EW_ALPHA( aColors[0]) << 24 )
                   | ( EW_RED  ( aColors[0]) << 16 )
                   | ( EW_GREEN( aColors[0]) <<  8 )
                   |   EW_BLUE ( aColors[0]);

    /* obtain direct access to the destination buffer */
    NxpRtSelectSurfaces( aDstHandle, 0 );

    /* get the start address in the destination and the offset */
    dst      = (void*)((unsigned char*)dstSurface->Pixel + (( aDstY * dstSurface->Width ) + aDstX ) * dstSurface->BytesPerPixel );
    dstPitch = dstSurface->Width * dstSurface->BytesPerPixel;

    /* choose the appropriate PXP drawing operation */
    EwBspGraphicsFill( (uint32_t)dst, dstPitch, aWidth, aHeight, dstColorFormat, color );
  }
}


/*******************************************************************************
* PROTOTYPE:
*   NxpRtCopyDriver
*
* DESCRIPTION:
*   The following function performs the 'copy rectangular area' operation from a
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
*     their alpha value == 255, the variant without any modulation is assumed.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void NxpRtCopyDriver( unsigned long aDstHandle, unsigned long aSrcHandle,
  int aDstX, int aDstY, int aSrcX, int aSrcY, int aWidth, int aHeight,
  int aBlend, unsigned long* aColors )
{
  XNxpRtSurface* dstSurface = (XNxpRtSurface*)aDstHandle;
  XNxpRtSurface* srcSurface = (XNxpRtSurface*)aSrcHandle;
  void*          dst;
  int            dstPitch;
  void*          src;
  int            srcPitch;
  int            dstFormat = ((XNxpRtSurface*)aDstHandle)->Format;
  int            srcFormat = ((XNxpRtSurface*)aSrcHandle)->Format;

#ifndef EW_DONT_USE_PXP_SOFTWARE_REPLACEMENT
  /* make a software emulation of the drawing function if the PXP is switched off */
  if ( UsePXP == 0 )
  {
    XSurfaceMemory dstMem;
    XSurfaceMemory srcMem;
    unsigned char  alpha  = EW_ALPHA( aColors[0]);
    XGradient      gradient;
    XCopyWorker    worker = 0;

    /* obtain direct access to the destination and source buffer */
    NxpRtLockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, &dstMem );
    NxpRtLockSurface( aSrcHandle, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, &srcMem );

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
    NxpRtUnlockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0 );
    NxpRtUnlockSurface( aSrcHandle, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
  }

  /* otherwise use the PXP hardware... */
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
    NxpRtSelectSurfaces( aDstHandle, aSrcHandle );

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
void NxpRtBlendDriver( unsigned long aDstHandle, unsigned long aSrcHandle,
  int aDstX, int aDstY, int aSrcX, int aSrcY, int aWidth, int aHeight,
  int aBlend, unsigned long* aColors )
{
  XNxpRtSurface* dstSurface = (XNxpRtSurface*)aDstHandle;
  XNxpRtSurface* srcSurface = (XNxpRtSurface*)aSrcHandle;
  void*          dst;
  int            dstPitch;
  void*          src;
  int            srcPitch;
  int            srcFormat = ((XNxpRtSurface*)aSrcHandle)->Format;

#ifndef EW_DONT_USE_PXP_SOFTWARE_REPLACEMENT
  /* make a software emulation of the drawing function if the PXP is switched off */
  if ( UsePXP == 0 )
  {
    XSurfaceMemory dstMem;
    XSurfaceMemory srcMem;
    unsigned char  alpha  = EW_ALPHA( aColors[0]);
    XGradient      gradient;
    XCopyWorker    worker = 0;

    /* obtain direct access to the destination and source buffer */
    NxpRtLockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, &dstMem );
    NxpRtLockSurface( aSrcHandle, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, &srcMem );

    /* select the necessary drawing function */
    if ( srcFormat == EW_PIXEL_FORMAT_NATIVE )
      worker = (alpha == 0xFF) ? EwScreenCopyNativeRowBlend : EwScreenCopyNativeRowSolidBlend;
    else if ( srcFormat == EW_PIXEL_FORMAT_RGB565 )
      worker = EwScreenCopyRGB565RowSolidBlend;

    /* let the Graphics Engine make the drawing operation */
    EwInitColorGradient( aWidth, aHeight, (unsigned int*)aColors, &gradient );
    EwEmulateCopy( &dstMem, &srcMem, aDstX, aDstY, aWidth, aHeight, aSrcX, aSrcY, &gradient, aWidth, aHeight, worker );

    /* unlock destination and source buffer */
    NxpRtUnlockSurface( aDstHandle, 0, 0, 0, 0, 0, 0, 0, 1, 0 );
    NxpRtUnlockSurface( aSrcHandle, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
  }

  /* otherwise use the PXP hardware... */
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
    NxpRtSelectSurfaces( aDstHandle, aSrcHandle );

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
void NxpRtUsePXP( int aActive )
{
#ifndef EW_DONT_USE_PXP_SOFTWARE_REPLACEMENT
  /* just store the flag */
  UsePXP = aActive;

  /* and put a message to the terminal */
  if ( aActive )
    EwPrint( "PXP is now active...\n" );
  else
    EwPrint( "PXP is now disabled!\n" );
#else
    EwPrint( "PXP is always active.\n" );
#endif
}


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
int NxpRtIsPXPUsed( void )
{
  return UsePXP;
}

#endif /* EW_USE_PXP_GRAPHICS_ACCELERATOR */

/* msy */
