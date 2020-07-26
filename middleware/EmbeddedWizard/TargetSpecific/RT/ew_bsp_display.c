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

#include "fsl_elcdif.h"
#include "pin_mux.h"
#include "fsl_gpio.h"

#include "ewconfig.h"
#include "ewrte.h"
#include "ewgfx.h"
#include "ewextgfx.h"
#include "ewgfxdefs.h"

#include "ew_bsp_display.h"
#include "ew_bsp_clock.h"

#if EW_USE_FREE_RTOS == 1

  #include "FreeRTOS.h"
  #include "task.h"
  #include "semphr.h"

  static SemaphoreHandle_t      LcdUpdateSemaphore;

#endif

#if EW_USE_DOUBLE_BUFFER == 1

  static __IO uint32_t          PendingFramebuffer = 0;

#else

  static volatile unsigned long VSyncTime          = 0;
  static volatile unsigned long VSyncDuration      = 0;

#endif


/* Macros for the LCD controller. */
#define LCD_HSW 41
#define LCD_HFP 4
#define LCD_HBP 8
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 2
#define LCD_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)

/* LCD RESET */
#define LCD_RESET_GPIO      GPIO1
#define LCD_RESET_GPIO_PIN  2

/* LCD Back light. */
#define LCD_BL_GPIO         GPIO2
#define LCD_BL_GPIO_PIN     31

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)

/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)

#define V_SYNC_OFFSET  10


/* Initialize the LCD_DISP. */
void BOARD_InitLcd( void )
{
    volatile uint32_t i = 0x1000U;

    gpio_pin_config_t config = {
        kGPIO_DigitalOutput, 0,
    };

    /* Reset the LCD. */
    GPIO_PinInit(LCD_RESET_GPIO, LCD_RESET_GPIO_PIN, &config);
    GPIO_WritePinOutput(LCD_RESET_GPIO, LCD_RESET_GPIO_PIN, 0);
    while (i--);
    GPIO_WritePinOutput(LCD_RESET_GPIO, LCD_RESET_GPIO_PIN, 1);

    /* Backlight. */
    config.outputLogic = 1;
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
}


void BOARD_InitLcdifPixelClock( void )
{
    /*
     * The desired output frame rate is 60Hz. So the pixel clock frequency is:
     * (480 + 41 + 4 + 18) * (272 + 10 + 4 + 2) * 60 = 9.2M.
     * Here set the LCDIF pixel clock to 9.3M.
     */

    /*
     * Initialize the Video PLL.
     * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 31, .postDivider = 8, .numerator = 0, .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);

    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 4);

    CLOCK_SetDiv(kCLOCK_LcdifDiv, 1);
}

void LCDIF_IRQHandler( void )
{
  uint32_t intStatus;

  intStatus = ELCDIF_GetInterruptStatus( LCDIF );
  ELCDIF_ClearInterruptStatus( LCDIF, intStatus );

  if ( intStatus & kELCDIF_CurFrameDone )
  {
    #if EW_USE_DOUBLE_BUFFER == 1

      if ( PendingFramebuffer )
      {
        #if EW_USE_FREE_RTOS == 1

          xSemaphoreGiveFromISR( LcdUpdateSemaphore, NULL );

        #endif

        PendingFramebuffer = 0;
      }

    #else

      unsigned long curTicks = DWT->CYCCNT;
      VSyncDuration = curTicks - VSyncTime;
      VSyncTime = curTicks;

    #endif
  }
    SDK_ISR_EXIT_BARRIER;
}


/* Helper function to sync on a certain line number updated by the display.
   This function is necessary when the system is used in single framebuffer mode. */
#if EW_USE_DOUBLE_BUFFER == 0
static void SyncOnLine( int aLine )
{
  unsigned long ticksTillRequestedLine;
  unsigned long localVSyncTime;

  if ( aLine == 0 )
    aLine += FRAME_BUFFER_HEIGHT;

  /* calculate time tick that corresponds to requested line */
  ticksTillRequestedLine = VSyncDuration / FRAME_BUFFER_HEIGHT * (aLine + V_SYNC_OFFSET);

  CPU_LOAD_SET_IDLE();

  /* if requested line is missed, wait for next display frame (next V-sync) */
  while (( DWT->CYCCNT - VSyncTime ) > ticksTillRequestedLine )
    ;

  /* wait for requested line in current display frame */
  localVSyncTime = VSyncTime;
  while (( DWT->CYCCNT - localVSyncTime ) < ticksTillRequestedLine )
    ;

  CPU_LOAD_SET_ACTIVE();
}
#endif


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
  #if EW_USE_FREE_RTOS == 1

    /* create semaphore for handling double-buffering on V-sync */
    LcdUpdateSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive( LcdUpdateSemaphore );

  #endif

  BOARD_InitLcdifPixelClock();
  BOARD_InitLcd();

  const elcdif_rgb_mode_config_t config =
  {
    .panelWidth = FRAME_BUFFER_WIDTH,
    .panelHeight = FRAME_BUFFER_HEIGHT,
    .hsw = LCD_HSW,
    .hfp = LCD_HFP,
    .hbp = LCD_HBP,
    .vsw = LCD_VSW,
    .vfp = LCD_VFP,
    .vbp = LCD_VBP,
    .polarityFlags = LCD_POL_FLAGS,
    .bufferAddr = ( uint32_t ) FRAME_BUFFER_ADDR,
    #if   ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGBA8888 )
      .pixelFormat = kELCDIF_PixelFormatXRGB8888,
      .dataBus = kELCDIF_DataBus16Bit
    #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGB888 )
      .pixelFormat = kELCDIF_PixelFormatRGB888,
      .dataBus = kELCDIF_DataBus16Bit
    #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGB565 )
      .pixelFormat = kELCDIF_PixelFormatRGB565,
      .dataBus = kELCDIF_DataBus16Bit
    #elif (( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_Index8 ) || \
           ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_LumA44 ))
      .pixelFormat = kELCDIF_PixelFormatRAW8,
      .dataBus = kELCDIF_DataBus8Bit,
    #endif
  };

  ELCDIF_RgbModeInit(LCDIF, &config);
  EnableIRQ(LCDIF_IRQn);
  NVIC_SetPriority(LCDIF_IRQn, 8);
  ELCDIF_EnableInterrupts(LCDIF, kELCDIF_CurFrameDoneInterruptEnable);
  ELCDIF_RgbModeStart(LCDIF);

  /* In case you get LCD underflows (e.g. flickering of LCD while PXP is active),
     you can increase the read priority for the LCD by modifying the NIC read_qos
     value for it. It is at 0x41044100 (register that isn’t in the header file)
     The default value is 1 which is a pretty low priority.
     If you change this register to a value grater than 1 (up to 5), then that
     will make the LCD higher priority. It should solve the underflow issue.
     If it creates a problem for another master, then you might need to experiment
     with the value to find something that keeps the LCD from underflowing, but
     allows enough bandwidth for other masters.
  */
  /*
  {
    uint32_t* LCD_read_qos = (uint32_t*)0x41044100;
    *LCD_read_qos = 0x02;
  }
  */

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

    #if EW_USE_DOUBLE_BUFFER == 1
      aDisplayInfo->UpdateMode  = EW_BSP_DISPLAY_UPDATE_NORMAL;
    #else
      aDisplayInfo->UpdateMode  = EW_BSP_DISPLAY_UPDATE_PARTIAL;
    #endif
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
  #if EW_USE_DOUBLE_BUFFER == 0

    static int field = 0;

    /* determine rectangular area of current field */
    #if EW_SURFACE_ROTATION == 0
      *aUpdateRect = EwNewRect( 0, field * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS,
        FRAME_BUFFER_WIDTH, ( field + 1 ) * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS );
    #endif

    #if EW_SURFACE_ROTATION == 90
      *aUpdateRect = EwNewRect( field * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS, 0,
        ( field + 1 ) * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS, FRAME_BUFFER_WIDTH );
    #endif

    #if EW_SURFACE_ROTATION == 180
      *aUpdateRect = EwNewRect( 0, FRAME_BUFFER_HEIGHT - ( field + 1 ) * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS,
        FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT - field * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS );
    #endif

    #if EW_SURFACE_ROTATION == 270
      *aUpdateRect = EwNewRect( FRAME_BUFFER_HEIGHT - ( field + 1 ) * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS,
        0, FRAME_BUFFER_HEIGHT - field * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS, FRAME_BUFFER_WIDTH );
    #endif

    /* next field */
    field++;

    /* sync on start line of next field to ensure save drawing operation */
    SyncOnLine(( field % NUMBER_OF_FIELDS ) * FRAME_BUFFER_HEIGHT / NUMBER_OF_FIELDS );

    if ( field <= NUMBER_OF_FIELDS )
      return 1;

    field = 0;

  #endif

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

    #if EW_USE_FREE_RTOS == 1

      /* wait until pending framebuffer is used as current framebuffer and
         use CPU time for other tasks */
      while ( PendingFramebuffer )
      {
        if ( xSemaphoreTake( LcdUpdateSemaphore, 1000 / portTICK_PERIOD_MS ) == pdTRUE )
          xSemaphoreGive( LcdUpdateSemaphore );
      }

    #else

      /* wait until pending framebuffer is used as current framebuffer */
      while( PendingFramebuffer )
        ;

    #endif

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
  #if EW_USE_DOUBLE_BUFFER == 1

    DisableIRQ( LCDIF_IRQn );

    /* set pending framebuffer address to be used on next V-sync */
    PendingFramebuffer = (uint32_t)aAddress;
    ELCDIF_SetNextBufferAddr( LCDIF, (uint32_t)aAddress );

    #if EW_USE_FREE_RTOS == 1
      xSemaphoreTake( LcdUpdateSemaphore, 0 );
    #endif

    EnableIRQ( LCDIF_IRQn );

  #endif
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
  int i;
  static uint32_t clut[ 256 ];

  for ( i = 0; i < 256; i++ )
    clut[ i ] = ((( aClut[ i ] & 0x00F80000 ) >> ( 3 + 16 )) << 11 ) |
                ((( aClut[ i ] & 0x0000F800 ) >> ( 2 +  8 )) <<  5 ) |
                ((( aClut[ i ] & 0x000000F8 ) >> ( 3 +  0 )) <<  0 );

  /* load the LUT data */
  ELCDIF_UpdateLut( LCDIF, kELCDIF_Lut0, 0, clut, ELCDIF_LUT_ENTRY_NUM );
  ELCDIF_UpdateLut( LCDIF, kELCDIF_Lut1, 0, clut, ELCDIF_LUT_ENTRY_NUM );

  ELCDIF_EnableLut( LCDIF, true );
}


/* mli, msy */
