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

  static SemaphoreHandle_t    LcdUpdateSemaphore;

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

static __IO uint32_t     PendingFramebuffer = 0;

#if EW_USE_DOUBLE_BUFFER == 0
  static int               DisplayHeight      = 0;
#endif

#if EW_USE_DOUBLE_BUFFER == 1


#else

  static volatile unsigned long    VSyncTime          = 0;
  static volatile unsigned long    VSyncDuration      = 0;

#endif




/* Initialize the LCD_DISP. */
void BOARD_InitLcd(void)
{
    volatile uint32_t i = 0x100U;

    gpio_pin_config_t config = {
        kGPIO_DigitalOutput, 0, kGPIO_NoIntmode,
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


void BOARD_InitLcdifPixelClock(void)
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


/*******************************************************************************
* FUNCTION:
*   EwBspConfigDisplay
*
* DESCRIPTION:
*   Configures the display hardware.
*
* ARGUMENTS:
*   aWidth   - Width of the framebuffer in pixel.
*   aHeight  - Height of the framebuffer in pixel.
*   aAddress - Startaddress of the framebuffer.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConfigDisplay( int aWidth, int aHeight, void* aAddress )
{
  #if EW_USE_FREE_RTOS == 1

  LcdUpdateSemaphore = xSemaphoreCreateBinary();

  #endif

  BOARD_InitLcdifPixelClock();
  BOARD_InitLcd();

  const elcdif_rgb_mode_config_t config =
  {
    .panelWidth = aWidth,
    .panelHeight = aHeight,
    .hsw = LCD_HSW,
    .hfp = LCD_HFP,
    .hbp = LCD_HBP,
    .vsw = LCD_VSW,
    .vfp = LCD_VFP,
    .vbp = LCD_VBP,
    .polarityFlags = LCD_POL_FLAGS,
    .bufferAddr = ( uint32_t ) aAddress,
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

#if EW_USE_DOUBLE_BUFFER == 0
  DisplayHeight      = aHeight;
#endif

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
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayWaitForCompletion
*
* DESCRIPTION:
*   The function EwBspDisplayWaitForCompletion returns as soon as the LCD update
*   has been completed.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
#if EW_USE_DOUBLE_BUFFER == 1
void EwBspDisplayWaitForCompletion( void )
{
  CPU_LOAD_SET_IDLE();

  #if EW_USE_FREE_RTOS == 1

    xSemaphoreTake(LcdUpdateSemaphore, 1000 / portTICK_PERIOD_MS);

  #else

    while( PendingFramebuffer );

  #endif

  CPU_LOAD_SET_ACTIVE();
}
#endif


/*******************************************************************************
* FUNCTION:
*   EwBspSyncOnDisplayLine
*
* DESCRIPTION:
*   The function EwBspSyncOnDisplayLine returns as soon as the display is updating
*   the requested line number.
*
* ARGUMENTS:
*   aLine - Number of the display line to be reached by LTDC
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
#if EW_USE_DOUBLE_BUFFER == 0
void EwBspSyncOnDisplayLine( int aLine )
{
  unsigned long ticksTillRequestedLine;
  unsigned long localVSyncTime;


  if ( aLine == 0 )
    aLine += DisplayHeight;

  /* calculate time tick that corresponds to requested line */
  ticksTillRequestedLine = VSyncDuration / DisplayHeight * (aLine + V_SYNC_OFFSET);

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
*   LCD_IRQHandler
*
* DESCRIPTION:
*   LCD Interrupt Handler.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void LCDIF_IRQHandler(void)
{
  uint32_t intStatus;

  intStatus = ELCDIF_GetInterruptStatus(LCDIF);
  ELCDIF_ClearInterruptStatus(LCDIF, intStatus);

  if (intStatus & kELCDIF_CurFrameDone)
  {

#if EW_USE_DOUBLE_BUFFER

    if  ( PendingFramebuffer )
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
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}


/*******************************************************************************
* FUNCTION:
*   EwBspSetFramebufferAddress
*
* DESCRIPTION:
*   The function EwBspSetFramebufferAddress is called from the Graphics Engine
*   in order to change the currently active framebuffer address. If the display
*   is running in a double-buffering mode, the function is called after each
*   screen update.
*   Changing the framebuffer address should be synchronized with V-sync.
*   In case of double-buffering, the function has to wait and return after
*   the V-sync was detected.
*
* ARGUMENTS:
*   aAddress - New address of the framebuffer to be shown on the display.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspSetFramebufferAddress( unsigned long aAddress )
{
#if EW_USE_DOUBLE_BUFFER == 1

  DisableIRQ( LCDIF_IRQn );
  /* set pending framebuffer address to be used on next V-sync */
  ELCDIF_SetNextBufferAddr( LCDIF, aAddress );
  PendingFramebuffer = aAddress;
  EnableIRQ( LCDIF_IRQn );

#else


#endif
}


/*******************************************************************************
* FUNCTION:
*   EwBspSetFramebufferClut
*
* DESCRIPTION:
*   The function EwBspSetFramebufferClut is called from the Graphics Engine
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
void EwBspSetFramebufferClut( unsigned long* aClut )
{
  int i;
  static uint32_t clut[ 256 ];

  for ( i=0; i < 256; i++ )
    clut[i] = ((( aClut[i] & 0x00F80000 ) >> (3 + 16)) << 11 ) |
              ((( aClut[i] & 0x0000F800 ) >> (2 +  8)) <<  5 ) |
              ((( aClut[i] & 0x000000F8 ) >> (3 +  0)) <<  0 );

  /* Load the LUT data. */
  ELCDIF_UpdateLut(LCDIF, kELCDIF_Lut0, 0, clut, ELCDIF_LUT_ENTRY_NUM);
  ELCDIF_UpdateLut(LCDIF, kELCDIF_Lut1, 0, clut, ELCDIF_LUT_ENTRY_NUM);

  ELCDIF_EnableLut(LCDIF, true);
}


/* mli */
