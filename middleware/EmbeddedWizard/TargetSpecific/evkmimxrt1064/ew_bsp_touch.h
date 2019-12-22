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

#ifndef EW_BSP_TOUCH_H
#define EW_BSP_TOUCH_H


#ifdef __cplusplus
  extern "C"
  {
#endif


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
void EwBspConfigTouch
( 
  int                         aWidth,
  int                         aHeight
);


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
int EwBspGetTouchPosition
(
  XPoint*                     aPos
);


#ifdef __cplusplus
  }
#endif

#endif /* EW_BSP_TOUCH_H */


/* msy */
