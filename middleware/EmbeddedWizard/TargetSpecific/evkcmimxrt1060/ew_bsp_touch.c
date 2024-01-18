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

#include "ewconfig.h"
#include "ewrte.h"
#include "ewgfxdriver.h"
#include "ewextgfx.h"

#include "ew_bsp_clock.h"
#include "ew_bsp_touch.h"
#include "board.h"
#include "fsl_video_common.h"

#include "fsl_lpi2c.h"
#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
#include "fsl_gt911.h"
#else
#include "fsl_ft5406_rt.h"
#endif


#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
#define NO_OF_FINGERS                   GT911_MAX_TOUCHES
#else
#define NO_OF_FINGERS                   FT5406_RT_MAX_TOUCHES
#endif

#define DELTA_TOUCH                     16
#define DELTA_TIME                      500

/* additional touch flag to indicate idle state */
#define EW_BSP_TOUCH_IDLE               0

/* additional touch flag to indicate hold state */
#define EW_BSP_TOUCH_HOLD               4

/* structure to store internal touch information for one finger */
typedef struct
{
  int           XPos;      /* horizontal position in pixel */
  int           YPos;      /* vertical position in pixel */
  unsigned long Ticks;     /* time of recent touch event */
  unsigned char TouchId;   /* constant touch ID provided by touch controller */
  unsigned char State;     /* current state within a touch cycle */
} XTouchData;

static int           TouchAreaWidth  = 0;
static int           TouchAreaHeight = 0;

static XTouchEvent   TouchEvent[ NO_OF_FINGERS ];
static XTouchData    TouchData[ NO_OF_FINGERS ];


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
#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
static void BOARD_PullTouchResetPin(bool pullUp);
static void BOARD_ConfigTouchIntPin(gt911_int_pin_mode_t mode);

static gt911_handle_t s_touchHandle;
static const gt911_config_t s_touchConfig = {
    .I2C_SendFunc     = BOARD_Touch_I2C_Send,
    .I2C_ReceiveFunc  = BOARD_Touch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullTouchResetPin,
    .intPinFunc       = BOARD_ConfigTouchIntPin,
    .timeDelayMsFunc  = VIDEO_DelayMs,
    .touchPointNum    = 1,
    .i2cAddrMode      = kGT911_I2cAddrMode0,
    .intTrigMode      = kGT911_IntRisingEdge,
};

#else
static ft5406_rt_handle_t touchHandle;
#endif
static touch_point_t      touchArray[ NO_OF_FINGERS ];

#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
/*******************************************************************************
* FUNCTION:
*   BOARD_PullTouchResetPin
*
* DESCRIPTION:
*   Controls touch reset pin.
*
* ARGUMENTS:
*   pullUp  - set state of reset pin.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/

static void BOARD_PullTouchResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PIN, 0);
    }
}

static void BOARD_ConfigTouchIntPin(gt911_int_pin_mode_t mode)
{
    if (mode == kGT911_IntPinInput)
    {
        BOARD_TOUCH_INT_GPIO->GDIR &= ~(1UL << BOARD_TOUCH_INT_PIN);
    }
    else
    {
        if (mode == kGT911_IntPinPullDown)
        {
            GPIO_PinWrite(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PIN, 0);
        }
        else
        {
            GPIO_PinWrite(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PIN, 1);
        }

        BOARD_TOUCH_INT_GPIO->GDIR |= (1UL << BOARD_TOUCH_INT_PIN);
    }
}
#endif

/*******************************************************************************
* FUNCTION:
*   EwBspTouchInit
*
* DESCRIPTION:
*   Initializes the touch driver.
*
* ARGUMENTS:
*   aWidth  - Width of the toucharea (framebuffer) in pixel.
*   aHeight - Height of the toucharea (framebuffer) in pixel.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspTouchInit( int aWidth, int aHeight )
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

#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
  GT911_Init(&s_touchHandle, &s_touchConfig);
#else
  /* Initialize the touch handle. */
  FT5406_RT_Init(&touchHandle, BOARD_TOUCH_I2C);
#endif

  TouchAreaWidth  = aWidth;
  TouchAreaHeight = aHeight;

  /* clear all touch state variables */
  memset( TouchData, 0, sizeof( TouchData ));
}


/*******************************************************************************
* FUNCTION:
*   EwBspTouchDone
*
* DESCRIPTION:
*   Terminates the touch driver.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspTouchDone( void )
{
}


/*******************************************************************************
* FUNCTION:
*   EwBspTouchGetEvents
*
* DESCRIPTION:
*   The function EwBspTouchGetEvents reads the current touch positions from the
*   touch driver and returns the current touch position and touch status of the
*   different fingers. The returned number of touch events indicates the number
*   of XTouchEvent that contain position and status information.
*   The orientation of the touch positions is adjusted to match GUI coordinates.
*   If the hardware supports only single touch, the finger number is always 0.
*
* ARGUMENTS:
*   aTouchEvent - Pointer to return array of XTouchEvent.
*
* RETURN VALUE:
*   Returns the number of detected touch events, otherwise 0.
*
*******************************************************************************/
int EwBspTouchGetEvents( XTouchEvent** aTouchEvent )
{
  status_t      status;
#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
  uint8_t       touchCount;
#else
  int           touchCount;
#endif
  int           x, y;
  int           t;
  int           f;
  unsigned long ticks;
  int           noOfEvents = 0;
  int           finger;
  char          identified[ NO_OF_FINGERS ];
  XTouchData*   touch;

  /* access touch driver to receive current touch status and position */
  CPU_LOAD_SET_IDLE();
#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
  touchCount = NO_OF_FINGERS;
  status = GT911_GetMultiTouch(&s_touchHandle, &touchCount, touchArray);
#else
  status = FT5406_RT_GetMultiTouch( &touchHandle, &touchCount, touchArray );
#endif
  CPU_LOAD_SET_ACTIVE();

#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
  if (!(( status == kStatus_Success ) || ( status == kStatus_TOUCHPANEL_NotTouched )))
  {
    EwPrint( "EwBspTouchGetEvents: error reading touch controller\n" );
    return 0;
  }
#else
  if ( status != kStatus_Success )
  {
    EwPrint( "EwBspTouchGetEvents: error reading touch controller\n" );
    return 0;
  }
#endif

  /* all fingers have the state unidentified */
  memset( identified, 0, sizeof( identified ));

  /* get current time in ms */
  ticks = EwGetTicks();

  /* iterate through all touch events from the hardware */
  for ( t = 0; t < touchCount; t++ )
  {
#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
		/* check for valid coordinates - coordinates provided swapped by FT5406 */
	    if (( touchArray[ t ].x > TouchAreaWidth ) || ( touchArray[ t ].y > TouchAreaHeight ))
	      continue;

	    /* apply screen rotation and swap coordinates provided by GT911 touch driver */
	    #if ( EW_SURFACE_ROTATION == 90 )

	      x = touchArray[ t ].y;
	      y = TouchAreaWidth  - touchArray[ t ].x;

	    #elif ( EW_SURFACE_ROTATION == 270 )

	      x = TouchAreaHeight - touchArray[ t ].y;
	      y = touchArray[ t ].x;

	    #elif ( EW_SURFACE_ROTATION == 180 )

	      x = TouchAreaWidth  - touchArray[ t ].x;
	      y = TouchAreaHeight - touchArray[ t ].y;

	    #else

	      x = touchArray[ t ].x;
	      y = touchArray[ t ].y;

	    #endif
#else
	/* check for valid coordinates - coordinates provided swapped by FT5406 */
    if (( touchArray[ t ].TOUCH_Y > TouchAreaWidth ) || ( touchArray[ t ].TOUCH_X > TouchAreaHeight ))
      continue;

    /* apply screen rotation and swap coordinates provided by FT5406 touch driver */
    #if ( EW_SURFACE_ROTATION == 90 )

      x = touchArray[ t ].TOUCH_X;
      y = TouchAreaWidth  - touchArray[ t ].TOUCH_Y;

    #elif ( EW_SURFACE_ROTATION == 270 )

      x = TouchAreaHeight - touchArray[ t ].TOUCH_X;
      y = touchArray[ t ].TOUCH_Y;

    #elif ( EW_SURFACE_ROTATION == 180 )

      x = TouchAreaWidth  - touchArray[ t ].TOUCH_Y;
      y = TouchAreaHeight - touchArray[ t ].TOUCH_X;

    #else

      x = touchArray[ t ].TOUCH_Y;
      y = touchArray[ t ].TOUCH_X;

    #endif
#endif

    /* Important note: The FT5406 driver does not provde down/up status information - the current
       phase within the touch cycle has to be determined by the software */
    /* iterate through all fingers to find a finger that matches with the provided touch event */
    for ( finger = -1, f = 0; f < NO_OF_FINGERS; f++ )
    {
      touch = &TouchData[ f ];

      /* check if the finger is already active */
#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
      if (( touch->State != EW_BSP_TOUCH_IDLE ) && ( touch->TouchId == touchArray[ t ].touchID ))
      {
        finger = f;
        break;
      }
#else
      if (( touch->State != EW_BSP_TOUCH_IDLE ) && ( touch->TouchId == touchArray[ t ].TOUCH_ID ))
      {
        finger = f;
        break;
      }
#endif
      /* check if the finger was used within the recent time span and if the touch position is in the vicinity */
      if (( touch->State == EW_BSP_TOUCH_IDLE ) && ( ticks < touch->Ticks + DELTA_TIME )
        && ( x > touch->XPos - DELTA_TOUCH ) && ( x < touch->XPos + DELTA_TOUCH )
        && ( y > touch->YPos - DELTA_TOUCH ) && ( y < touch->YPos + DELTA_TOUCH ))
        finger = f;

      /* otherwise take the first free finger */
      if (( touch->State == EW_BSP_TOUCH_IDLE ) && ( finger == -1 ))
        finger = f;
    }

    /* determine the state within a touch cycle and assign the touch parameter to the found finger */
    if ( finger >= 0 )
    {
      touch = &TouchData[ finger ];
      identified[ finger ] = 1;

      /* check for start of touch cycle */
      if ( touch->State == EW_BSP_TOUCH_IDLE )
        touch->State = EW_BSP_TOUCH_DOWN;
      else
      {
        /* check if the finger has moved */
        if (( touch->XPos != x ) || ( touch->YPos != y ))
          touch->State = EW_BSP_TOUCH_MOVE;
        else
          touch->State = EW_BSP_TOUCH_HOLD;
      }

      /* store current touch parameter */
      touch->XPos    = x;
      touch->YPos    = y;
#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
      touch->TouchId = touchArray[ t ].touchID;
#else
      touch->TouchId = touchArray[ t ].TOUCH_ID;
#endif
      touch->Ticks   = ticks;
    }
  }

  /* prepare sequence of touch events suitable for Embedded Wizard GUI application */
  for ( f = 0; f < NO_OF_FINGERS; f++ )
  {
    touch = &TouchData[ f ];

    /* begin of a touch cycle */
    if ( identified[ f ] && ( touch->State == EW_BSP_TOUCH_DOWN ))
      TouchEvent[ noOfEvents ].State = EW_BSP_TOUCH_DOWN;

    /* move within a touch cycle */
    else if ( identified[ f ] && ( touch->State == EW_BSP_TOUCH_MOVE ))
      TouchEvent[ noOfEvents ].State = EW_BSP_TOUCH_MOVE;

    /* end of a touch cycle */
    else if ( !identified[ f ] && ( touch->State != EW_BSP_TOUCH_IDLE ))
    {
      TouchEvent[ noOfEvents ].State = EW_BSP_TOUCH_UP;
      touch->State = EW_BSP_TOUCH_IDLE;
    }
    else
      continue;

    TouchEvent[ noOfEvents ].XPos   = touch->XPos;
    TouchEvent[ noOfEvents ].YPos   = touch->YPos;
    TouchEvent[ noOfEvents ].Finger = f;

    // EwPrint( "Touch event for finger %d with state %d ( %4d, %4d )\n", f, TouchEvent[ noOfEvents ].State, TouchEvent[ noOfEvents ].XPos, TouchEvent[ noOfEvents ].YPos );

    noOfEvents++;
  }

  /* return the prepared touch events and the number of prepared touch events */
  if ( aTouchEvent )
    *aTouchEvent = TouchEvent;

  return noOfEvents;
}


/* mli, msy */
