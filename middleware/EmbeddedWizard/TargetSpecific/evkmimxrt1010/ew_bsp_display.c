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

#include "ewconfig.h"
#include "ewrte.h"
#include "ewgfx.h"
#include "ewextgfx.h"
#include "ewgfxdefs.h"

#include "ew_bsp_display.h"
#include "ew_bsp_clock.h"

#include "fsl_common.h"

#include <stdint.h>
#include <string.h>

#if EW_USE_FREE_RTOS == 1

  #include "FreeRTOS.h"

#endif

/* flags to choose one of the provided display drivers */
#define USE_ILI9341

#if defined USE_ILI9341

  #include "ili9341_spi.h"

  #define  DispDrvWriteDataWindow       ILI9341_WriteDataWindow
  #define  DispDrvWaitForCompletion     ILI9341_WaitForCompletion

#endif

#if (( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_Index8 ) \
  || ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_LumA44 ))
  static unsigned short      Clut[ 256 ];
#endif

/* allocate SRAM for the scratch-pad buffer */
unsigned char ScratchPad[ SCRATCHPAD_WIDTH * SCRATCHPAD_HEIGHT * FRAME_BUFFER_DEPTH ];
#define FRAME_BUFFER_ADDR ScratchPad

#if EW_USE_DOUBLE_BUFFER == 1

  #error EW_USE_DOUBLE_BUFFER is not necessary/supported because of separate transfer buffer for DMA/SPI

#endif

#define DOUBLE_BUFFER_ADDR 0

/* allocate SRAM for the transfer buffer */
EW_MEM_SECTION( OC_RAM ) unsigned short TransferBuffer[ SCRATCHPAD_WIDTH * SCRATCHPAD_HEIGHT ];


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
  #if defined USE_ILI9341

    /* initialize ILI9341 driver... */
    ILI9341_Init();

    /* ...and try to receive expected ID */
//    if ( ILI9341_ReadID() != ILI9341_ID )
//      EwPrint( "EwBspDisplayInit: Failed to initialize ILI9341 display driver...\n" );

  #endif

  /* return the current display configuration */
  if ( aDisplayInfo )
  {
    memset( aDisplayInfo, 0, sizeof( XDisplayInfo ));
    aDisplayInfo->FrameBuffer   = (void*)FRAME_BUFFER_ADDR;
    aDisplayInfo->DoubleBuffer  = (void*)DOUBLE_BUFFER_ADDR;
    aDisplayInfo->BufferWidth   = SCRATCHPAD_WIDTH;
    aDisplayInfo->BufferHeight  = SCRATCHPAD_HEIGHT;
    aDisplayInfo->DisplayWidth  = DISPLAY_WIDTH;
    aDisplayInfo->DisplayHeight = DISPLAY_HEIGHT;
    aDisplayInfo->UpdateMode    = EW_BSP_DISPLAY_UPDATE_SCRATCHPAD;
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
  static int h     = 0;
  static int dh    = 0;
  int        width = aUpdateRect->Point2.X - aUpdateRect->Point1.X;

  if ( width <= 0 )
    return 0;

  if ( dh == 0 )
  {
    h = aUpdateRect->Point2.Y - aUpdateRect->Point1.Y;

    /* determine the number of lines that fit into the scratch-pad */
    dh = SCRATCHPAD_WIDTH * SCRATCHPAD_HEIGHT /
         ( aUpdateRect->Point2.X - aUpdateRect->Point1.X );
  }
  else
    aUpdateRect->Point1.Y += dh;

  if ( dh > h )
    dh = h;

  aUpdateRect->Point2.Y = aUpdateRect->Point1.Y + dh;
  h -= dh;

  if ( dh > 0 )
    return 1;

  dh = 0;
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
  /* ensure that previous transfer is already finished */
  DispDrvWaitForCompletion();

  /* update display content depending on source color format */
  #if (( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_Index8 ) \
    || ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_LumA44 ))
  {
    register int i;
    register uint16_t* dst = (uint16_t*)TransferBuffer;
    register uint8_t*  src = (uint8_t*)aAddress;

    /* take index from scratch-pad and convert it via clut into color -
       colors are already stored byte swapped and suitable for the transfer buffer */
    for( i = aWidth * aHeight; i > 0; i--, dst++, src++ )
      *dst = Clut[ *src ];
  }
  #else
  {
    register int i;
    register uint16_t* dst = (uint16_t*)TransferBuffer;
    register uint16_t* src = (uint16_t*)aAddress;

    /* copy content of scratch-pad into transfer buffer and make the byte swap */
    for( i = aWidth * aHeight; i > 0; i--, dst++, src++ )
      *dst = ( *src >> 8 ) | ( *src << 8 );
  }
  #endif

  DispDrvWriteDataWindow( aX, aY, aWidth, aHeight, (uint8_t*)TransferBuffer );
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
  #if ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_Index8 )

    /* create a color lookup table to update display with RGB565 color format */
    register int i;
    for ( i = 0; i < 256; i++ )
    {
      /* clut content is stored with byte swapped order */
      register unsigned long c = *aClut++;
      Clut[ i ] = (( c & 0x00F80000 ) >> 16 ) | (( c & 0x0000E000 ) >> 13 ) | (( c & 0x00001C00 ) << 3 ) | (( c & 0x000000F8 ) << 5 );
    }

  #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_LumA44 )

    /* create a color lookup table to update display with RGB565 color format */
    register int i;
    for ( i = 0; i < 256; i++ )
    {
      /* clut content is stored with byte swapped order */
      register unsigned long l = ( i & 0x0F ) | (( i & 0x0F ) << 4 );
      Clut[ i ] = (( l & 0xF8 ) << 0 ) | (( l & 0xE0 ) >> 5 ) | (( l & 0x1C ) << 11 ) | (( l & 0xF8 ) << 5 );
    }

  #else

    EwPrint( "EwBspDisplaySetClut: Could not load CLUT!\n" );

  #endif
}


/* msy */
