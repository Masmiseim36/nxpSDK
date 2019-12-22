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
*   Please note: The implementation of this module is partially based on
*   examples that are provided within the NXP MCUXpresso software. In case you
*   want to adapt this module to your custom specific hardware, please adapt the
*   hardware initialization code according your needs.
*   This file contains an interface to the PXP hardware of the NXP target
*   in order to provide graphics acceleration for the Embedded Wizard generated
*   UI applications.
*   This interface is intended to be used only by the Graphics Engine of
*   Embedded Wizard.
*
*******************************************************************************/

#include <string.h>

#include "ew_bsp_graphics.h"
#include "ew_bsp_clock.h"

#if EW_USE_FREE_RTOS == 1

  #include "FreeRTOS.h"
  #include "task.h"
  #include "semphr.h"

  static SemaphoreHandle_t    AcceleratorSemaphore;

#endif


static volatile char       TransferInProgress  = 0;
static char                ConcurrentOperation = 1;

static pxp_output_buffer_config_t OutputBufferConfig;
static pxp_ps_buffer_config_t     ProcessSurfaceConfig;
static pxp_as_buffer_config_t     AlphaSurfaceConfig;
static pxp_as_blend_config_t      BlendConfig;

/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsInit
*
* DESCRIPTION:
*   Initialize the PXP graphics accelerator.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   If successful, returns != 0.
*
*******************************************************************************/
int EwBspGraphicsInit( void )
{
  #if EW_USE_FREE_RTOS == 1

    /* Create the accelerator semaphore */
    AcceleratorSemaphore = xSemaphoreCreateBinary();

  #endif

  /* initialize the PXP graphics accelerator */
  PXP_Init( PXP );

  PXP_EnableAlphaSurfaceOverlayColorKey( PXP, false );
  PXP_EnableOverWrittenAlpha( PXP, false );

  /* disable CSC1 */
  PXP_EnableCsc1( PXP, false );

  #ifdef EW_USE_PXP_INTERRUPT_MODE

    /* enable interrupt for PXP completion */
    EnableIRQ( PXP_IRQn );
    NVIC_SetPriority( PXP_IRQn, 8 );
    PXP_EnableInterrupts( PXP, kPXP_CompleteInterruptEnable );

  #endif

  return 1;
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsDone
*
* DESCRIPTION:
*   Deinitialize the PXP graphics accelerator.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsDone( void )
{
  /* deinitialize the PXP graphics accelerator */
  PXP_Deinit( PXP );
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsWaitForCompletion
*
* DESCRIPTION:
*   The function EwBspGraphicsWaitForCompletion returns as soon as the PXP has
*   completed a pending graphics instruction. If the PXP is in idle mode,
*   the function returns immediately.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsWaitForCompletion()
{
  /* return immediately if no PXP transfer is ongoing */
  if ( TransferInProgress == 0 )
    return;

  CPU_LOAD_SET_IDLE();

#ifdef EW_USE_PXP_INTERRUPT_MODE

  #if EW_USE_FREE_RTOS == 1

    xSemaphoreTake( AcceleratorSemaphore, 1000 / portTICK_PERIOD_MS );
    TransferInProgress = 0;

  #else

    /* wait until PXP transfer is done */
    while( TransferInProgress )
      ;

  #endif

#else

  /* wait until PXP transfer is done */
  while (!(PXP_GetStatusFlags( PXP ) & kPXP_CompleteFlag ))
    ;

  PXP_ClearStatusFlags( PXP, kPXP_CompleteFlag );

  TransferInProgress = 0;

#endif

  CPU_LOAD_SET_ACTIVE();
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsConcurrentOperation
*
* DESCRIPTION:
*   The function EwBspGraphicsConcurrentOperation configures the operation mode
*   of PXP and CPU. If concurrent operation is enabled, the CPU will work in
*   parallel while the PXP is transferring data. If concurrent operation is
*   disabled, the CPU will wait everytime the PXP is active.
*   This feature is intended to limit the memory bandwidth, e.g. during display
*   update or other bandwidth consuming activities.
*
* ARGUMENTS:
*   aEnable - flag to switch on/off concurrent operation mode.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsConcurrentOperation( int aEnable )
{
  ConcurrentOperation = (char)aEnable;
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsFill
*
* DESCRIPTION:
*   The function EwBspGraphicsFill is used by the Graphics Engine, when a
*   rectangular area should be filled with a constant color by using the PXP
*   functionality.
*
* ARGUMENTS:
*   aDstAddr      - Destination address of the transfer.
*   aDstPitch     - Number of bytes per line within the destination bitmap.
*   aWidth,
*   aHeight       - Size of the area to fill.
*   aDstColorMode - Colorformat of the destination.
*   aSrcColor     - Color to fill the rectangular area.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsFill( uint32_t aDstAddr, uint32_t aDstPitch, uint32_t aWidth,
  uint32_t aHeight, uint32_t aDstColorMode, uint32_t aSrcColor )
{
  /* ensure that previous PXP instruction is finished */
  EwBspGraphicsWaitForCompletion();

  /* configure alpha surface settings */
  PXP_SetAlphaSurfacePosition( PXP, 1, 1, 0, 0 );

  /* configure process surface settings - background color does not support alpha */
  PXP_SetProcessSurfaceBackGroundColor( PXP, aSrcColor );
  PXP_SetProcessSurfacePosition(  PXP, 1, 1, 0, 0 );

  /* configure output surface settings */
  OutputBufferConfig.pixelFormat    = (pxp_output_pixel_format_t)aDstColorMode;
  OutputBufferConfig.interlacedMode = kPXP_OutputProgressive;
  OutputBufferConfig.buffer0Addr    = aDstAddr;
  OutputBufferConfig.buffer1Addr    = 0;
  OutputBufferConfig.pitchBytes     = aDstPitch;
  OutputBufferConfig.width          = aWidth;
  OutputBufferConfig.height         = aHeight;

  PXP_SetOutputBufferConfig( PXP, &OutputBufferConfig );

  /* set the flag for PXP transfer */
  TransferInProgress = 1;

  /* start the transfer */
  PXP_Start( PXP );

  /* check for immediate completion of drawing operation */
  if ( !ConcurrentOperation )
    EwBspGraphicsWaitForCompletion();
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsCopy
*
* DESCRIPTION:
*   The function EwBspGraphicsCopy is used by the Graphics Engine, when a source
*   bitmap should be copied into a destination bitmap by using the PXP
*   functionality.
*
* ARGUMENTS:
*   aDstAddr      - Destination address of the transfer.
*   aSrcAddr      - Source address of the transfer.
*   aDstPitch     - Number of bytes per line within the destination bitmap.
*   aSrcPitch     - Number of bytes per line within the source bitmap.
*   aWidth,
*   aHeight       - Size of the area to transfer the bitmap.
*   aDstColorMode - Colorformat of the destination bitmap
*   aSrcColorMode - Colorformat of the source bitmap.
*   aSrcColor     - Color/alpha value to be used for the transfer.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsCopy( uint32_t aDstAddr, uint32_t aSrcAddr, uint32_t aDstPitch,
  uint32_t aSrcPitch, uint32_t aWidth, uint32_t aHeight, uint32_t aDstColorMode,
  uint32_t aSrcColorMode, uint32_t aSrcColor )
{
  unsigned char alpha = (unsigned char)( aSrcColor >> 24 );

  /* ensure that previous PXP instruction is finished */
  EwBspGraphicsWaitForCompletion();

  /* in case of any Y8/YUV source color format the process surface has to be used */
  if ( aSrcColorMode & 0x10 )
  {
    BlendConfig.alpha       = 0x00;
    BlendConfig.invertAlpha = false;
    BlendConfig.alphaMode   = kPXP_AlphaOverride;
    BlendConfig.ropMode     = kPXP_RopMaskAs;

    /* configure alpha surface settings */
    PXP_SetAlphaSurfaceBlendConfig( PXP, &BlendConfig );
    PXP_SetAlphaSurfacePosition( PXP, 1, 1, 0, 0 );

    /* configure process surface settings - it does not support alpha */
    ProcessSurfaceConfig.pixelFormat = (pxp_ps_pixel_format_t)aSrcColorMode;
    ProcessSurfaceConfig.swapByte    = 0;
    ProcessSurfaceConfig.bufferAddr  = aSrcAddr;
    ProcessSurfaceConfig.bufferAddrU = 0;
    ProcessSurfaceConfig.bufferAddrV = 0;
    ProcessSurfaceConfig.pitchBytes  = aSrcPitch;

    PXP_SetProcessSurfaceBufferConfig( PXP, &ProcessSurfaceConfig );
    PXP_SetProcessSurfacePosition( PXP, 0, 0, aWidth, aHeight );
  }
  else
  {
    /* configure alpha surface settings */
    AlphaSurfaceConfig.pixelFormat = (pxp_as_pixel_format_t)aSrcColorMode;
    AlphaSurfaceConfig.bufferAddr  = aSrcAddr;
    AlphaSurfaceConfig.pitchBytes  = aSrcPitch;

    BlendConfig.alpha       = alpha;
    BlendConfig.invertAlpha = false;
    BlendConfig.alphaMode   = (alpha == 0xFF) ? kPXP_AlphaEmbedded : kPXP_AlphaMultiply;
    BlendConfig.ropMode     = kPXP_RopMaskAs;

    PXP_SetAlphaSurfaceBufferConfig( PXP, &AlphaSurfaceConfig );
    PXP_SetAlphaSurfaceBlendConfig( PXP, &BlendConfig );
    PXP_SetAlphaSurfacePosition( PXP, 0, 0, aWidth, aHeight );

    /* configure process surface settings */
    PXP_SetProcessSurfaceBackGroundColor( PXP, 0x00000000 );
    PXP_SetProcessSurfacePosition( PXP, 1, 1, 0, 0 );
  }

  /* configure output surface settings */
  OutputBufferConfig.pixelFormat    = (pxp_output_pixel_format_t)aDstColorMode;
  OutputBufferConfig.interlacedMode = kPXP_OutputProgressive;
  OutputBufferConfig.buffer0Addr    = aDstAddr;
  OutputBufferConfig.buffer1Addr    = 0;
  OutputBufferConfig.pitchBytes     = aDstPitch;
  OutputBufferConfig.width          = aWidth;
  OutputBufferConfig.height         = aHeight;

  PXP_SetOutputBufferConfig( PXP, &OutputBufferConfig );

  /* set the flag for PXP transfer */
  TransferInProgress = 1;

  /* start the transfer */
  PXP_Start( PXP );

  /* check for immediate completion of drawing operation */
  if ( !ConcurrentOperation )
    EwBspGraphicsWaitForCompletion();
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsCopyBlend
*
* DESCRIPTION:
*   The function EwBspGraphicsCopyBlend is used by the Graphics Engine, when a
*   source bitmap should be blended over a destination bitmap by using the PXP
*   functionality.
*
* ARGUMENTS:
*   aDstAddr      - Destination address of the transfer.
*   aSrcAddr      - Source address of the transfer.
*   aDstPitch     - Number of bytes per line within the destination bitmap.
*   aSrcPitch     - Number of bytes per line within the source bitmap.
*   aWidth,
*   aHeight       - Size of the area to transfer the bitmap.
*   aDstColorMode - Colorformat of the destination bitmap
*   aSrcColorMode - Colorformat of the source bitmap.
*   aSrcColor     - Color/alpha value to be used for the transfer.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsCopyBlend( uint32_t aDstAddr, uint32_t aSrcAddr, uint32_t aDstPitch,
  uint32_t aSrcPitch, uint32_t aWidth, uint32_t aHeight, uint32_t aDstColorMode,
  uint32_t aSrcColorMode, uint32_t aSrcColor )
{
  unsigned char alpha = (unsigned char)( aSrcColor >> 24 );

  /* ensure that previous PXP instruction is finished */
  EwBspGraphicsWaitForCompletion();

  /* configure alpha surface settings */
  AlphaSurfaceConfig.pixelFormat = (pxp_as_pixel_format_t)aSrcColorMode;
  AlphaSurfaceConfig.bufferAddr  = aSrcAddr;
  AlphaSurfaceConfig.pitchBytes  = aSrcPitch;

  BlendConfig.alpha       = alpha;
  BlendConfig.invertAlpha = false;
  BlendConfig.alphaMode   = (alpha == 0xFF) ? kPXP_AlphaEmbedded : kPXP_AlphaMultiply;
  BlendConfig.ropMode     = kPXP_RopMaskAs;

  PXP_SetAlphaSurfaceBufferConfig( PXP, &AlphaSurfaceConfig );
  PXP_SetAlphaSurfaceBlendConfig( PXP, &BlendConfig );
  PXP_SetAlphaSurfacePosition( PXP, 0, 0, aWidth, aHeight );

  /* configure process surface settings -
     attention: it does not support alpha and does not support packed RGB888 */
  ProcessSurfaceConfig.pixelFormat = (pxp_ps_pixel_format_t)aDstColorMode;
  ProcessSurfaceConfig.swapByte    = 0;
  ProcessSurfaceConfig.bufferAddr  = aDstAddr;
  ProcessSurfaceConfig.bufferAddrU = 0;
  ProcessSurfaceConfig.bufferAddrV = 0;
  ProcessSurfaceConfig.pitchBytes  = aDstPitch;

  PXP_SetProcessSurfaceBufferConfig( PXP, &ProcessSurfaceConfig );
  PXP_SetProcessSurfacePosition( PXP, 0, 0, aWidth, aHeight );

  /* configure output surface settings */
  OutputBufferConfig.pixelFormat    = (pxp_output_pixel_format_t)aDstColorMode;
  OutputBufferConfig.interlacedMode = kPXP_OutputProgressive;
  OutputBufferConfig.buffer0Addr    = aDstAddr;
  OutputBufferConfig.buffer1Addr    = 0;
  OutputBufferConfig.pitchBytes     = aDstPitch;
  OutputBufferConfig.width          = aWidth;
  OutputBufferConfig.height         = aHeight;

  PXP_SetOutputBufferConfig( PXP, &OutputBufferConfig );

  /* set the flag for PXP transfer */
  TransferInProgress = 1;

  /* start the transfer */
  PXP_Start( PXP );

  /* check for immediate completion of drawing operation */
  if ( !ConcurrentOperation )
    EwBspGraphicsWaitForCompletion();
}


#ifdef EW_USE_PXP_INTERRUPT_MODE

/*******************************************************************************
* FUNCTION:
*   PXP_IRQHandler
*
* DESCRIPTION:
*   PXP Interrupt Handler.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void PXP_IRQHandler( void )
{
  if (PXP_GetStatusFlags( PXP ) & kPXP_CompleteFlag )
  {
    PXP_ClearStatusFlags( PXP, kPXP_CompleteFlag );

    #if EW_USE_FREE_RTOS == 1

      xSemaphoreGiveFromISR( AcceleratorSemaphore, NULL );

    #else

      TransferInProgress = 0;

    #endif
  }
}

#endif

/* msy */
