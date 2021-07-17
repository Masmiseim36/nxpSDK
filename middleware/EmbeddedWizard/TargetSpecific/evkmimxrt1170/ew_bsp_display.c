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
*   This file is part of the interface (glue layer) between an Embedded Wizard
*   generated UI application and the board support package (BSP) of a dedicated
*   target.
*   This template is responsible to initialize the display hardware of the board
*   and to provide the necessary access to update the display content.
*
*******************************************************************************/

#include "ewrte.h"
#include "ewgfx.h"
#include "ewextgfx.h"
#include "ewgfxdefs.h"

#include "ew_bsp_display.h"
#include "ew_bsp_clock.h"

#include "display_support.h"
#include "vglite_support.h"

#include "fsl_dc_fb.h"
#include "fsl_fbdev.h"
#include "fsl_cache.h"


#if ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGBA8888 )
  #define EW_BSP_DISPLAY_PIXEL_FORMAT kVIDEO_PixelFormatXRGB8888
#elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGB565 )
  #define EW_BSP_DISPLAY_PIXEL_FORMAT kVIDEO_PixelFormatRGB565
#else
  #error "selected EW_FRAME_BUFFER_COLOR_FORMAT not supported"
#endif


static fbdev_t         g_fbdev;
static fbdev_fb_info_t g_fbInfo;

extern const dc_fb_t   g_dc;


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayInit
*
* DESCRIPTION:
*   The function EwBspDisplayInit initializes the display hardware and returns
*   the display parameter.
*
* ARGUMENTS:
*   aDisplayInfo - Display info data structure.
*
* RETURN VALUE:
*   Returns 1 if successful, 0 otherwise.
*
*******************************************************************************/
int EwBspDisplayInit( XDisplayInfo* aDisplayInfo )
{
  status_t        status;
  void*           buffer;

  status = BOARD_PrepareVGLiteController();
  if ( status != kStatus_Success )
  {
    EwPrint( "EwBspDisplayInit: BOARD_PrepareVGLiteController failed.\n" );
    return 0;
  }

  status = BOARD_PrepareDisplayController();
  if ( status != kStatus_Success )
  {
    EwPrint( "EwBspDisplayInit: BOARD_PrepareDisplayController failed.\n" );
    return 0;
  }

  status = FBDEV_Open( &g_fbdev, &g_dc, 0);
  if ( status != kStatus_Success )
  {
    EwPrint( "EwBspDisplayInit: FBDEV_Open failed.\n" );
    return 0;
  }

  g_fbInfo.bufInfo.pixelFormat = EW_BSP_DISPLAY_PIXEL_FORMAT;
  g_fbInfo.bufInfo.width       = FRAME_BUFFER_WIDTH;
  g_fbInfo.bufInfo.height      = FRAME_BUFFER_HEIGHT;
  g_fbInfo.bufInfo.strideBytes = FRAME_BUFFER_WIDTH * FRAME_BUFFER_DEPTH;
  g_fbInfo.bufferCount         = 2;
  g_fbInfo.buffers[ 0 ]        = (void*)FRAME_BUFFER_ADDR;
  g_fbInfo.buffers[ 1 ]        = (void*)DOUBLE_BUFFER_ADDR;

  status = FBDEV_SetFrameBufferInfo( &g_fbdev, &g_fbInfo );
  if ( status != kStatus_Success )
  {
    EwPrint( "EwBspDisplayInit: FBDEV_SetFrameBufferInfo failed.\n" );
    return 0;
  }

  buffer = FBDEV_GetFrameBuffer( &g_fbdev, 0 );
  memset( buffer, 0, FRAME_BUFFER_SIZE );
  L1CACHE_CleanDCacheByRange( (uint32_t)buffer, FRAME_BUFFER_SIZE );
  FBDEV_SetFrameBuffer( &g_fbdev, buffer, 0 );

  FBDEV_Enable( &g_fbdev );

  /* return the current display configuration */
  if ( aDisplayInfo )
  {
    memset( aDisplayInfo, 0, sizeof( XDisplayInfo ));
    aDisplayInfo->FrameBuffer   = (void*)FRAME_BUFFER_ADDR;
    aDisplayInfo->DoubleBuffer  = (void*)DOUBLE_BUFFER_ADDR;
    aDisplayInfo->BufferWidth   = FRAME_BUFFER_WIDTH;
    aDisplayInfo->BufferHeight  = FRAME_BUFFER_HEIGHT;
    aDisplayInfo->DisplayWidth  = FRAME_BUFFER_WIDTH;
    aDisplayInfo->DisplayHeight = FRAME_BUFFER_HEIGHT;

    aDisplayInfo->UpdateMode    = EW_BSP_DISPLAY_UPDATE_NORMAL;
  }
  return 1;
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayDone
*
* DESCRIPTION:
*   The function EwBspDisplayDone deinitializes the display hardware.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplayDone( void )
{
  /* close display */
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayGetUpdateArea
*
* DESCRIPTION:
*   The function EwBspDisplayGetUpdateArea returns the next update area
*   depending on the selected display mode:
*   In case of a synchroneous single-buffer, the function has to return the
*   the rectangular areas that correspond to the horizontal stripes (fields)
*   of the framebuffer.
*   In case of a scratch-pad buffer, the function has to return the subareas
*   that fit into the provided update rectangle.
*   During each display update, this function is called until it returns 0.
*
* ARGUMENTS:
*   aUpdateRect - Rectangular area which should be updated (redrawn).
*
* RETURN VALUE:
*   Returns 1 if a further update area can be provided, 0 otherwise.
*
*******************************************************************************/
int EwBspDisplayGetUpdateArea( XRect* aUpdateRect )
{
  return 0;
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayWaitForCompletion
*
* DESCRIPTION:
*   The function EwBspDisplayWaitForCompletion is called from the Graphics Engine
*   to ensure that all pending activities of the display system are completed, so
*   that the rendering of the next frame can start.
*   In case of a double-buffering system, the function has to wait until the
*   V-sync has occured and the pending buffer is used by the display controller.
*   In case of an external display controller, the function has to wait until
*   the transfer (update) of the graphics data has been completed and there are
*   no pending buffers.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplayWaitForCompletion( void )
{
  #if EW_USE_DOUBLE_BUFFER == 1

    CPU_LOAD_SET_IDLE();

    FBDEV_GetFrameBuffer( &g_fbdev, 0 );

    CPU_LOAD_SET_ACTIVE();

  #endif
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayCommitBuffer
*
* DESCRIPTION:
*   The function EwBspDisplayCommitBuffer is called from the Graphics Engine
*   when the rendering of a certain buffer has been completed.
*   The type of buffer depends on the selected framebuffer concept.
*   If the display is running in a double-buffering mode, the function is called
*   after each buffer update in order to change the currently active framebuffer
*   address. Changing the framebuffer address should be synchronized with V-sync.
*   If the system is using an external graphics controller, this function is
*   responsible to start the transfer of the framebuffer content.
*
* ARGUMENTS:
*   aAddress - Address of the framebuffer to be shown on the display.
*   aX,
*   aY       - Origin of the area which has been updated by the Graphics Engine.
*   aWidth,
*   aHeight  - Size of the area which has been updated by the Graphics Engine.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplayCommitBuffer( void* aAddress, int aX, int aY, int aWidth, int aHeight )
{
  FBDEV_SetFrameBuffer( &g_fbdev, aAddress, 0 );
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplaySetClut
*
* DESCRIPTION:
*   The function EwBspDisplaySetClut is called from the Graphics Engine
*   in order to update the hardware CLUT of the current framebuffer.
*   The function is only called when the color format of the framebuffer is
*   Index8 or LumA44.
*
* ARGUMENTS:
*   aClut - Pointer to a color lookup table with 256 entries.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplaySetClut( unsigned long* aClut )
{
}


/* mli, msy */
