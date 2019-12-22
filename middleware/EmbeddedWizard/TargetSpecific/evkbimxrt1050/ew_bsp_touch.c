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
*   This template is responsible to initialize the touch driver of the display
*   hardware and to receive the touch events for the UI application.
*
*******************************************************************************/

#include "fsl_lpi2c.h"
#include "fsl_ft5406_rt.h"

#include "ewrte.h"
#include "ewgfxdriver.h"
#include "ewextgfx.h"

#include "ew_bsp_clock.h"
#include "ew_bsp_touch.h"

#if ( EW_SURFACE_ROTATION == 90 ) || ( EW_SURFACE_ROTATION == 180 )
  static int                    TouchAreaWidth  = 0;
#endif

#if ( EW_SURFACE_ROTATION == 180 ) || ( EW_SURFACE_ROTATION == 270 )
  static int                    TouchAreaHeight = 0;
#endif



/* Touch */
#define EXAMPLE_I2C_MASTER_BASE (I2C2_BASE)
#define I2C_MASTER_CLOCK_FREQUENCY (12000000)

#define EXAMPLE_I2C_MASTER ((I2C_Type *)EXAMPLE_I2C_MASTER_BASE)
#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_BAUDRATE 100000U

/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)

/* Macros for the touch touch controller. */
#define BOARD_TOUCH_I2C LPI2C1

#define BOARD_TOUCH_I2C_CLOCK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
#define BOARD_TOUCH_I2C_BAUDRATE 100000U

 
/* Touch driver handle. */
static ft5406_rt_handle_t touchHandle;

  
/*******************************************************************************
* FUNCTION:
*   EwBspConfigTouch
*
* DESCRIPTION:
*   Configure the touch driver.
*
* ARGUMENTS:
*   aWidth  - Width of the toucharea (framebuffer) in pixel.
*   aHeight - Height of the toucharea (framebuffer) in pixel.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConfigTouch( int aWidth, int aHeight )
{
  lpi2c_master_config_t masterConfig = {0};
  /*
  * masterConfig.debugEnable = false;
  * masterConfig.ignoreAck = false;
  * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
  * masterConfig.baudRate_Hz = 100000U;
  * masterConfig.busIdleTimeout_ns = 0;
  * masterConfig.pinLowTimeout_ns = 0;
  * masterConfig.sdaGlitchFilterWidth_ns = 0;
  * masterConfig.sclGlitchFilterWidth_ns = 0;
  */
  LPI2C_MasterGetDefaultConfig(&masterConfig);

  /* Change the default baudrate configuration */
  masterConfig.baudRate_Hz = BOARD_TOUCH_I2C_BAUDRATE;

  /* Initialize the LPI2C master peripheral */
  LPI2C_MasterInit(BOARD_TOUCH_I2C, &masterConfig, BOARD_TOUCH_I2C_CLOCK_FREQ);

  /* Initialize the touch handle. */
  FT5406_RT_Init(&touchHandle, BOARD_TOUCH_I2C);
    
#if ( EW_SURFACE_ROTATION == 90 ) || ( EW_SURFACE_ROTATION == 180 )
  TouchAreaWidth  = aWidth;
#endif

#if ( EW_SURFACE_ROTATION == 180 ) || ( EW_SURFACE_ROTATION == 270 )
  TouchAreaHeight = aHeight;
#endif    
}


/*******************************************************************************
* FUNCTION:
*   EwBspGetTouchPosition
*
* DESCRIPTION:
*   The function EwBspGetTouchPosition reads the current touch position from the
*   touch driver and returns the current position and status. The orientation
*   of the touch positions is adjusted to match GUI coordinates.
*
* ARGUMENTS:
*   aPos - Pointer to XPoint structure to return the current position.
*
* RETURN VALUE:
*   Returns 1 if a touch event is detected, otherwise 0.
*
*******************************************************************************/
int EwBspGetTouchPosition( XPoint* aPos )
{
  static int    lastCursorPosX = 0;
  static int    lastCursorPosY = 0;
  int           retval         = 0;
  int           cursorPosX;
  int           cursorPosY;
  status_t      status;
  touch_event_t touch_event;  
  
  CPU_LOAD_SET_IDLE();
  /* access touch driver to receive current touch status and position */
  status = FT5406_RT_GetSingleTouch(&touchHandle, 
    &touch_event, &cursorPosY, &cursorPosX);
  CPU_LOAD_SET_ACTIVE();

  if ( status != kStatus_Success )
  {
    EwPrint( "error reading touch controller\r\n" );
  }
  else if ( touch_event == kTouch_Contact )
  {
    #if ( EW_SURFACE_ROTATION == 90 )

      lastCursorPosX = cursorPosY;
      lastCursorPosY = TouchAreaWidth - cursorPosX;

    #elif ( EW_SURFACE_ROTATION == 270 )

      lastCursorPosX = TouchAreaHeight - cursorPosY;
      lastCursorPosY = cursorPosX;

    #elif ( EW_SURFACE_ROTATION == 180 )

      lastCursorPosX = TouchAreaWidth  - cursorPosX;
      lastCursorPosY = TouchAreaHeight - cursorPosY;

    #else

      lastCursorPosX = cursorPosX;
      lastCursorPosY = cursorPosY;

    #endif
    
    retval = 1;
  }
 
  /* return valid touch event */
  aPos->X = lastCursorPosX;
  aPos->Y = lastCursorPosY;  
  return retval;
}

/* mli */
